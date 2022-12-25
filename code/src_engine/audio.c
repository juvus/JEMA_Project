/**
 * ================================================================================
 * @file src_engine/audio.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with audio system of
 * the game.
 * @version 0.6
 * @date 2022-11-30
 * ================================================================================ 
 */

#include "include_engine/audio.h"

#include <dsound.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/sound.h"
#include "include_engine/utils.h"

/**
 * @brief Mixing sound data into audio mix array.
 * @param mix_array Pointer to the mix array.
 * @param sound Pointer to the sound array.
 * @param sample_count Number of samples.
 */
static void 
MixTempArrayToMixArray(s16 *mix_array, s16 *sound, u32 sample_count);

/**
 * @brief Applying the pan for the sound.
 * @param sound Pointer to the sound array.
 * @param sample_count Number of samples.
 * @param pan Sound pan (left of right channel).
 */
static void
ApplySoundPan(s16 *sound, u32 sample_count, SoundPan pan);

/**
 * @brief Applying the volume for the sound.
 * @param sound Pointer to the sound array.
 * @param sample_count Number of samples.
 * @param volume Sound volume.
 */
static void
ApplySoundVolume(s16 *sound, u32 sample_count, f32 volume);

/**
 * @brief Cleaning audio buffer - s16 array.  
 * @param audio Pointer to the Audio structure.
 * @param array Pointer to sound array.
 */
static void
CleanS16Array(Audio *audio, s16 *array);

Audio*
Audio_Constructor(void)
{
    size_t size = sizeof(Audio);
    Audio *audio = (Audio *)HelperFcn_MemAllocate(size);
    return audio;
}

Audio*
Audio_Destructor(Audio *audio)
{
    HelperFcn_MemFree(audio->temp_array);
    HelperFcn_MemFree(audio->mix_array);
    HelperFcn_MemFree(audio);    
    return NULL;
}

void
Audio_Init(Audio *audio, HWND window)
{
    u32 channels_num = 2;
    u32 samples_per_second = 44100;
    u32 bytes_per_sample = channels_num * sizeof(s16);
    u32 bits_per_sample_for_channel = 8 * bytes_per_sample / channels_num; /* 16 bits. */
    u32 buffer_time = 1;  /* Duration of the audio buffer in seconds. */
    f32 temp_array_time = 0.1f;  /* Seconds. */
    u32 size = buffer_time * samples_per_second * bytes_per_sample;  /* Buffer size. */

    /* Load dll and assign it to a hangle. */
    HMODULE dsound_dll = LoadLibraryA("dsound.dll");
    if (!dsound_dll) dbg_error("%s", "File dsound.dll not found!");

    /* Get the procedure adress from the loaded dll. */
    direct_sound_create_t *direct_sound_create;
    direct_sound_create = (direct_sound_create_t*)GetProcAddress(dsound_dll, "DirectSoundCreate");
    if (!direct_sound_create) dbg_error("%s", "Direct_sound_create failed!"); 
    
    LPDIRECTSOUND direct_sound = 0;
    if (SUCCEEDED(direct_sound_create(0, &direct_sound, 0)))
    {
        if (SUCCEEDED(direct_sound->lpVtbl->SetCooperativeLevel(direct_sound, window,
            DSSCL_PRIORITY)))
        {
            /* Sound buffer specification (for 1 second buffer). */
            audio->channels_num = channels_num;
            audio->samples_per_second = samples_per_second;
            audio->bytes_per_sample = bytes_per_sample;
            audio->size = size;          
            
            /* Description of the primary sound buffer. */
            DSBUFFERDESC buffer_description = {0};
            buffer_description.dwSize = sizeof(buffer_description);
            buffer_description.dwFlags = DSBCAPS_PRIMARYBUFFER;

            /* Creating the primary sound buffer */
            if (!SUCCEEDED(direct_sound->lpVtbl->CreateSoundBuffer(direct_sound, 
                &buffer_description, &audio->primary_buffer, 0))) 
            {
                dbg_error("%s", "Unable to create primary sound buffer!");
            }

            /* Wav format specification. */
            WAVEFORMATEX wave_format = {0};
            wave_format.wFormatTag = WAVE_FORMAT_PCM;
            wave_format.nChannels = (WORD)channels_num;
            wave_format.nSamplesPerSec = samples_per_second;
            wave_format.wBitsPerSample = (WORD)bits_per_sample_for_channel;
            wave_format.nBlockAlign = wave_format.nChannels * wave_format.wBitsPerSample / 8;            
            wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;

            /* Description of the secondary sound buffer. */
            buffer_description = (DSBUFFERDESC){0};
            buffer_description.dwSize = sizeof(buffer_description);
            buffer_description.dwFlags = DSBCAPS_GLOBALFOCUS;
            buffer_description.dwBufferBytes = size;
            buffer_description.lpwfxFormat = &wave_format;

            /* Creating the secondary buffer. */
            if (!SUCCEEDED(direct_sound->lpVtbl->CreateSoundBuffer(direct_sound, 
                &buffer_description, &audio->buffer, 0))) 
            {
                dbg_error("%s", "Unable to create secondary sound buffer!");
            }

            /* Allocate memory for mix_array and temp_array (for 0.1 seconds, 4410 samples). */
            audio->s16_array_size = (u32)(temp_array_time * samples_per_second * 2);
            audio->s32_array_size = audio->s16_array_size / 2;
            audio->mix_array = (s16 *)calloc(audio->s16_array_size, sizeof(s16));
            audio->temp_array = (s16 *)calloc(audio->s16_array_size, sizeof(s16)); 

            /* Initialization of the additional parameters. */
            audio->write_size = (DWORD)(temp_array_time * samples_per_second * 4);  /* 17640 bytes. */
            audio->border_cursor = 0;
            audio->update_cursor = audio->border_cursor + audio->write_size; 
        }   
    }
}

void
Audio_CleanBuffer(Audio *audio)
{
    void *region_1;  /* Pointer to the region 1. */
    DWORD region_1_size;  /* Region 1 size. */
    void *region_2;  /* Pointer to the region 2. */
    DWORD region_2_size;  /* Region 2 size. */
    
    /* Lock the buffer starting from offset 0 byte from the begining and lock all 176400 bytes. */
    if (SUCCEEDED(audio->buffer->lpVtbl->Lock(audio->buffer, 0, audio->size,
        &region_1, &region_1_size, &region_2, &region_2_size, 0))) 
    {
        /* Get pointer to sample's channel value s16 (-32768...32767). */
        s16 *at = (s16 *)region_1;
        
        DWORD region_1_sample_count = region_1_size / audio->bytes_per_sample;
        for (DWORD i = 0; i < region_1_sample_count; ++i) 
        {            
            *at++ = 0;
            *at++ = 0;
        }

        /* Unlock the buffer after writing. */
        audio->buffer->lpVtbl->Unlock(audio->buffer, region_1, region_1_size,
            region_2, region_2_size);
    }
}

void
Audio_PlaySounds(Audio *audio)
{
    audio->buffer->lpVtbl->SetCurrentPosition(audio->buffer, 0);
    
    /* Play the sound in the buffer. */
    if (!SUCCEEDED(audio->buffer->lpVtbl->Play(audio->buffer, 0, 0, DSBPLAY_LOOPING)))
    {
        dbg_error("%s", "Impossible to play a sound!");
    }
}

void
Audio_UpdateBuffer(Audio *audio, Sound *sounds[], u32 sound_num)
{
    /* Get number of elements in array. */
    u32 s16_array_size = audio->s16_array_size;
    u32 s32_array_size = audio->s32_array_size;

    /* Get the cursors position for the analysis. */
    audio->buffer->lpVtbl->GetCurrentPosition(audio->buffer, &(audio->current_play_cursor), 
        &(audio->current_write_cursor));

    if (audio->current_write_cursor >= audio->border_cursor)
    {       
        /* Need to load a new batch of sound samples into the buffer. */
        /* Special case. */
        if ((audio->border_cursor == 0) && 
            (audio->current_write_cursor >= (audio->size - audio->write_size)))
        {
            return;
        }
        
        /* Clean the array mix_array of the buffer. */
        CleanS16Array(audio, audio->mix_array);

        /* Prepare the mix of all sounds that than will be loaded to the buffer. */
        for (u32 i = 0; i < sound_num; ++i)
        {
            /* Get pointer to the sound for the treatment. */
            Sound *sound = sounds[i]; 

            if (sound->is_playing)
            {
                /* Fill the temporary array for futher mixing. */
                /* Calculate the rest amount of samples that need to be loaded. */
                u32 rest_sample_num = sound->sample_count - sound->sample_index; 
                
                /* Clean the temp_array of the buffer. */
                CleanS16Array(audio, audio->temp_array);

                if (rest_sample_num >= s32_array_size)
                {
                    /* A lot of available samples, just copy them into temp_array. */
                    for (u32 j = 0; j < s16_array_size; ++j)
                    {
                        u32 k = sound->sample_index * 2  + j;
                        audio->temp_array[j] = sound->s16_array[k];
                    }

                    /* Update the sample_index (shit forward for 2205 samples). */
                    sound->sample_index += s32_array_size;
                }
                else if ((rest_sample_num < s32_array_size) && (sound->is_looping == false))
                {
                    /* Low amount of samples rest. Copy them to the buffer. Also sound 
                    should be stopped playing after that. */

                    for (u32 j = 0; j < (rest_sample_num * 2); ++j)
                    {
                        /* Copy remaining samples to the temp_buffer. */
                        u32 k = sound->sample_index * 2 + j;
                        audio->temp_array[j] = sound->s16_array[k];
                    }

                    /* Disable playing sound because no more samples available in the array */
                    sound->sample_index = 0;
                    sound->is_playing = false;
                }
                else if ((rest_sample_num < s32_array_size) && (sound->is_looping == true))
                {
                    /* Low samples available, temp_array should be filled with samples in the 
                    beginning of sound smaples array. */

                    for (u32 j = 0; j < s16_array_size; ++j)
                    {
                        /* Copy remaining samples as usual. */
                        if (j < (rest_sample_num * 2))
                        {
                            u32 k = sound->sample_index * 2 + j;
                            audio->temp_array[j] = sound->s16_array[k];
                        }
                        /* Copy remaining samples from the beginning of the samples array. */
                        else
                        {
                            u32 k = j - (rest_sample_num * 2);
                            audio->temp_array[j] = sound->s16_array[k];
                        }
                    }

                    /* Update the sample index. */
                    sound->sample_index = s32_array_size - rest_sample_num;
                }

                /* Add additional treatment of the sound. */
                ApplySoundPan(audio->temp_array, s32_array_size, sound->pan);
                ApplySoundVolume(audio->temp_array, s32_array_size, sound->volume);

                /* Mix sound from temp_array buffer into mix_array buffer. */
                MixTempArrayToMixArray(audio->mix_array, audio->temp_array, s32_array_size);
            }
        }
        
        /* Load the sounds mix to the buffer. */     
        void *region_1;  /* Pointer to the region 1. */
        DWORD region_1_size;  /* Region 1 size. */
        void *region_2;  /* Pointer to the region 2. */
        DWORD region_2_size;  /* Region 2 size. */
        
        /* Lock the buffer starting from update_buffer offset for bytes_to_lock bytes. */  
        if (SUCCEEDED(audio->buffer->lpVtbl->Lock(audio->buffer, audio->update_cursor, 
            audio->write_size, &region_1, &region_1_size, &region_2, &region_2_size, 0))) 
        {
            /* Get pointers to sample's channel values s16 (-32768...32767). */
            s16 *at_dest = (s16 *)region_1;
            s16 *at_src = audio->mix_array;
        
            for (u32 i = 0; i < s16_array_size; ++i) 
            {            
                /* Copy from the destination to the source. */
                *at_dest = *at_src;
                at_dest++;
                at_src++;
            }

            /* Unlock the buffer. */
            audio->buffer->lpVtbl->Unlock(audio->buffer, region_1, region_1_size,
                region_2, region_2_size);
        }

        /* Update the buffer cursors. */
        audio->border_cursor = audio->update_cursor;
        audio->update_cursor += audio->write_size;
        audio->update_cursor %= audio->size; /* To avoid overflow. */
    }    
}

static void
CleanS16Array(Audio *audio, s16 *array)
{
    s16 *at = array;
    for (u32 i = 0; i < audio->s16_array_size; ++i)
    {
        *at++ = 0x0000;
    }
}

static void 
ApplySoundPan(s16 *sound, u32 sample_count, SoundPan pan)
{
    /* Use both channels for the sound. Do nothing. */
    if (pan == S_PAN_BOTH) return;
    
    /* Determine the offset (0 for the left channel, 1 for the right channel). */
    u32 offset = (pan == S_PAN_LEFT) ? 1 : 0;
    
    for (u32 i = 0; i < sample_count; ++i)
    {
        /* Calculation of the s16 element index in the array. */
        u32 j = i * 2 + offset;  /* s16 element index of the channel in array. */
        sound[j] = 0x0000;
    }
}

static void 
ApplySoundVolume(s16 *sound, u32 sample_count, f32 volume)
{
    for (u32 i = 0; i < sample_count; ++i)
    {
        for (u32 offset = 0; offset < 2; ++offset)
        {    
            /* Calculation of the s16 element index in the array. Offset of the word
            index in the array means: 0 for channel 1; 1 for channel 2. */
            u32 elem_index = i * 2 + offset;       
            s32 temp = (s32)(round(sound[elem_index] * volume));  

            if ((temp <= 32767) && (temp >= -32768))
            {
                /* No overflow and underflow cases (most probable). */
                sound[elem_index] = (s16)temp;
            }
            else if (temp > 32767)
            {
                /* Overflow case. */
                sound[elem_index] = 32767;
            } 
            else
            {
                /* Underflow case. */
                sound[elem_index] = -32768;
            }
        }
    }
}

static void
MixTempArrayToMixArray(s16 *mix_array, s16 *sound, u32 sample_count)
{
    for (u32 i = 0; i < sample_count; ++i)
    {
        for (u32 offset = 0; offset < 2; ++offset)
        {    
            /* Calculation of the s16 element index in the array. Offset of the word
            index in the array means: 0 for channel 1; 1 for channel 2. */
            u32 elem_index = i * 2 + offset;  /* s16 element index of the channel in array. */        
            s32 temp = (s32)mix_array[elem_index] + (s32)sound[elem_index];

            if ((temp <= 32767) && (temp >= -32768))
            {
                /* No overflow and underflow cases (most probable). */
                mix_array[elem_index] = (s16)temp;
            }
            else if (temp > 32767)
            {
                /* Overflow case. */
                mix_array[elem_index] = 32767;
            }
            else
            {
                /* Underflow case. */
                mix_array[elem_index] = -32768;
            }
        }
    }
}
