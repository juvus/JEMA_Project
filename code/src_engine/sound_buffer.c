/**
 * ================================================================================
 * @file sound_buffer.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the Sound_Buffer class methods.
 * @version 0.5
 * @date 2021-12-19
 * ================================================================================ 
 */

/* Standard includes: */
#include <dsound.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Program includes: */
#include <sound_buffer.h>
#include <utils.h>
#include <stdio.h>
#include <sound.h>

/* Static functions: */
/* Function to mix sound into buffer mix array */
static void mix_temp_array_to_mix_array(s16 *mix_array, s16 *sound, u32 sample_count);
/* Function to apply the pan for the sound (use the channel) */
static void apply_sound_pan(s16 *sound, u32 sample_count, Sound_Pan_t pan);
/* Function to apply the volume for the sound */
static void apply_sound_volume(s16 *sound, u32 sample_count, f32 volume);

void
sound_buffer_init(HWND window, Sound_buffer_t *sound_buffer)
{
    /* Function to initialize the audio (create sound buffer) */
    
    /* Load library dinamically using dll library */
    HMODULE dsound_dll = LoadLibraryA("dsound.dll");
    if (!dsound_dll) {
        assert(0 && "dsound.dll not found!");
    }

    /* Take only one procedure from the loaded dll file */
    direct_sound_create_t *direct_sound_create = \
        (direct_sound_create_t*) GetProcAddress(dsound_dll, "DirectSoundCreate");
    if (!direct_sound_create) {
        assert(0 && "Error: direct_sound_create failed!");
    }

    LPDIRECTSOUND direct_sound = 0;
    if (SUCCEEDED(direct_sound_create(0, &direct_sound, 0)))
    {
        if (SUCCEEDED(direct_sound->lpVtbl->SetCooperativeLevel(direct_sound, window, DSSCL_PRIORITY)))
        {
            /* Sound buffer specification (for 1 second buffer) */
            sound_buffer->channels_num = 2;
            sound_buffer->samples_per_second = 44100;
            sound_buffer->bytes_per_sample = sound_buffer->channels_num * sizeof(s16); 
            sound_buffer->size = 1 * sound_buffer->samples_per_second * sound_buffer->bytes_per_sample;           
            
            /* Description of the primary sound buffer */
            DSBUFFERDESC buffer_description = {0};
            buffer_description.dwSize = sizeof(buffer_description);
            buffer_description.dwFlags = DSBCAPS_PRIMARYBUFFER;

            /* Creating the primary sound buffer */
            if (!SUCCEEDED(direct_sound->lpVtbl->CreateSoundBuffer(direct_sound, &buffer_description,
                &sound_buffer->primary_buffer, 0))) 
            {
                assert(0 && "Error: unable to create primary sound buffer!");
            }

            /* Wav format specification */
            WAVEFORMATEX wave_format = {0};
            wave_format.wFormatTag = WAVE_FORMAT_PCM;
            wave_format.nChannels = (WORD)sound_buffer->channels_num;
            wave_format.nSamplesPerSec = sound_buffer->samples_per_second;
            wave_format.wBitsPerSample = 16; /* 16 bits (2 bytes) for one channel */
            wave_format.nBlockAlign = wave_format.nChannels * wave_format.wBitsPerSample / 8;            
            wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;

            /* Description of the secondary sound buffer */
            buffer_description = (DSBUFFERDESC){0}; /* clear */
            buffer_description.dwSize = sizeof(buffer_description);
            buffer_description.dwFlags = DSBCAPS_GLOBALFOCUS;
            buffer_description.dwBufferBytes = sound_buffer->size;
            buffer_description.lpwfxFormat = &wave_format;

            /* Creating the secondary buffer */
            if (!SUCCEEDED(direct_sound->lpVtbl->CreateSoundBuffer(direct_sound, &buffer_description,
                &sound_buffer->buffer, 0))) 
            {
                assert(0 && "Error: unable to create secondary sound buffer!");
            }

            /* Allocate memory for mix_array and temp_array (for 0.1 seconds, 4410 samples)*/
            sound_buffer->s16_array_size = (u32)(0.1 * sound_buffer->samples_per_second * 2);
            sound_buffer->s32_array_size = sound_buffer->s16_array_size / 2;
            sound_buffer->mix_array = (s16 *)calloc(sound_buffer->s16_array_size, sizeof(s16));
            sound_buffer->temp_array = (s16 *)calloc(sound_buffer->s16_array_size, sizeof(s16)); 

            /* Init additional parameters */
            sound_buffer->write_size = (DWORD)(0.1 * sound_buffer->samples_per_second * 4); /* 17640 bytes */
            sound_buffer->border_cursor = 0;
            sound_buffer->update_cursor = sound_buffer->border_cursor + sound_buffer->write_size; 
        }   
    }
}

void
sound_buffer_clear_buffer(Sound_buffer_t *sound_buffer)
{
    /* Function to clear the sound_buffer buffer */

    DWORD i;
    s16 *at; /* Pointer to sample's channel value s16 (-32768...32767) */
    void *region_1;
    DWORD region_1_size;
    void *region_2;
    DWORD region_2_size;
    
    /* Lock the buffer starting from offset 0 byte from the begining and lock all 176400 bytes */
    if (SUCCEEDED(sound_buffer->buffer->lpVtbl->Lock(sound_buffer->buffer, 0, sound_buffer->size,
        &region_1, &region_1_size, &region_2, &region_2_size, 0))) 
    {
        at = (s16 *)region_1;
        DWORD region_1_sample_count = region_1_size / sound_buffer->bytes_per_sample;
        for (i = 0; i < region_1_sample_count; ++i) 
        {            
            *at++ = 0;
            *at++ = 0;
        }

        /* Unlock the buffer */
        sound_buffer->buffer->lpVtbl->Unlock(sound_buffer->buffer, region_1, region_1_size,
            region_2, region_2_size);
    }
}

void
sound_buffer_clear_s16_array(Sound_buffer_t *sound_buffer, s16 *array)
{
    /* Function to clear the specific s16 array of the buffer */
    size_t i;
    s16 *at = array;

    for (i = 0; i < sound_buffer->s16_array_size; i++)
    {
        *at++ = 0x0000;
    }
}

void
sound_buffer_play(Sound_buffer_t *sound_buffer)
{
    /* Function to play the entire buffer with sounds (should be started once) */

    sound_buffer->buffer->lpVtbl->SetCurrentPosition(sound_buffer->buffer, 0);
    
    /* Play the buffer */
    if (!SUCCEEDED(sound_buffer->buffer->lpVtbl->Play(sound_buffer->buffer, 0, 0, DSBPLAY_LOOPING)))
    {
        assert(0 && "Impossible to play a sound!");
    }
}

void
sound_buffer_update(Sound_buffer_t *sound_buffer, Sound_t *sounds[], u32 e_sound_num)
{
    /* Function to update the sounds that need to be played in the buffer */
    Sound_t *sound;  /* Pointer to the treated sound */
    u32 i, j, k;  /* Temp indexes */
    u32 rest_sample_num;  /* Amount of samples, remained from sample_index to the end of array */
    u32 s16_array_size = sound_buffer->s16_array_size; /* Number of s16 elements of in arrays */
    u32 s32_array_size = sound_buffer->s32_array_size; /* Number of s32 elements of in arrays */
    s16 *at_dest; /* Pointer to sample's channel value s16 (-32768...32767) at destination */
    s16 *at_src; /* Pointer to sample's channel value s16 (-32768...32767) at source */
    void *region_1;
    DWORD region_1_size;
    void *region_2;
    DWORD region_2_size;

    /* Get cursors positions for the analysis */
    sound_buffer->buffer->lpVtbl->GetCurrentPosition(sound_buffer->buffer,
        &(sound_buffer->current_play_cursor), &(sound_buffer->current_write_cursor));
 

    if (sound_buffer->current_write_cursor >= sound_buffer->border_cursor)
    {       
        /* Need to load a new batch of sound samples into the buffer */
        
        /* Special case: add description */
        if ((sound_buffer->border_cursor == 0) && 
            (sound_buffer->current_write_cursor >= (sound_buffer->size - sound_buffer->write_size)))
        {
            return;
        }
        
        /* 1. Clear the array mix_array of the buffer */
        sound_buffer_clear_s16_array(sound_buffer, sound_buffer->mix_array);

        /* 2. Prepare the mix of all sounds that than will be loaded to the buffer */
        for (i = 0; i < e_sound_num; i++)
        {
            /* Get pointer to the sound for the treatment */
            sound = sounds[i]; 

            if (sound->is_playing)
            {
                /* Fill the temporary array for futher mixing */
                /* Calculate the rest amount of samples that need to be loaded */
                rest_sample_num = sound->sample_count - sound->sample_index; 
                
                /* Clear the temp_array of the buffer */
                sound_buffer_clear_s16_array(sound_buffer, sound_buffer->temp_array);

                if (rest_sample_num >= s32_array_size)
                {
                    /* A lot of available samples, just copy them into temp_array */
                    for (j = 0; j < s16_array_size; j++) 
                    {
                        k = sound->sample_index * 2  + j;
                        sound_buffer->temp_array[j] = sound->s16_array[k];
                    }

                    /* Update the sample_index (shit forward for 2205 samples) */
                    sound->sample_index += s32_array_size;
                }
                else if ((rest_sample_num < s32_array_size) &&
                         (sound->is_looping == false))
                {
                    /* Low amount of samples rest. Copy them to the buffer. Also sound 
                       should be stopped playing after that */

                    for (j = 0; j < (rest_sample_num * 2); j++)
                    {
                        /* Copy remaining samples to the temp_buffer */
                        k = sound->sample_index * 2 + j;
                        sound_buffer->temp_array[j] = sound->s16_array[k];
                    }

                    /* Disable playing sound because no more samples available in the array */
                    sound->sample_index = 0;
                    sound->is_playing = false;
                }
                else if ((rest_sample_num < s32_array_size) && 
                         (sound->is_looping == true))
                {
                    /* Low samples available, temp_array should be filled with samples in the 
                       beginning of sound smaples array */

                    for (j = 0; j < s16_array_size; j++)
                    {
                        /* Copy remaining samples as usual */
                        if (j < (rest_sample_num * 2))
                        {
                            k = sound->sample_index * 2 + j;
                            sound_buffer->temp_array[j] = sound->s16_array[k];
                        }
                        /* Copy remaining samples from the beginning of the samples array */
                        else
                        {
                            k = j - (rest_sample_num * 2);
                            sound_buffer->temp_array[j] = sound->s16_array[k];
                        }
                    }

                    /* Update the sample index */
                    sound->sample_index = s32_array_size - rest_sample_num;
                }

                /* Add additional treatment of the sound */
                apply_sound_pan(sound_buffer->temp_array, s32_array_size, sound->pan);
                apply_sound_volume(sound_buffer->temp_array, s32_array_size, sound->volume);

                /* Mix sound from temp_array buffer into mix_array buffer */
                mix_temp_array_to_mix_array(sound_buffer->mix_array, sound_buffer->temp_array,
                     s32_array_size);
            }
        }
        
        /* 3. Load the sounds mix to the buffer */     
        /* Lock the buffer starting from update_buffer offset for bytes_to_lock bytes */  
        if (SUCCEEDED(sound_buffer->buffer->lpVtbl->Lock(sound_buffer->buffer,
             sound_buffer->update_cursor, sound_buffer->write_size, &region_1, &region_1_size, 
             &region_2, &region_2_size, 0))) 
        {
            at_dest = (s16 *)region_1;
            at_src = sound_buffer->mix_array;
        
            for (i = 0; i < s16_array_size; ++i) 
            {            
                /* Copy from the destination to the source */
                *at_dest = *at_src;
                at_dest++;
                at_src++;
            }

            /* Unlock the buffer */
            sound_buffer->buffer->lpVtbl->Unlock(sound_buffer->buffer, region_1, region_1_size,
                region_2, region_2_size);
        }

        /* 4. Update the buffer cursors */
        sound_buffer->border_cursor = sound_buffer->update_cursor;
        sound_buffer->update_cursor += sound_buffer->write_size;
        sound_buffer->update_cursor %= sound_buffer->size; /* To avoid overflow */
    }    
}

/* Realization of the buffer static functions */

static void 
apply_sound_pan(s16 *sound, u32 sample_count, Sound_Pan_t pan)
{
    /* Function to apply the pan for the sound (use the channel) */

    /* Use both channels for the sound. Do nothing */
    if (pan == S_PAN_BOTH) return; 

    u32 offset;  /* Offset of the word index in the array (0 for channel 1; 1 for channel 2) */
    u32 i;  /* Sample index in the sound array */
    u32 j;  /* s16 element (16 bit) index in the sound array */ 
    
    /* Determine the offset (0 for the left channel, 1 for the right channel) */
    offset = (pan == S_PAN_LEFT) ? 1 : 0;
    
    for (i = 0; i < sample_count; i++)
    {
        /* Calculation of the s16 element index in the array */
        j = i * 2 + offset;  /* s16 element index of the channel in array */
        sound[j] = 0x0000;
    }
}

static void 
apply_sound_volume(s16 *sound, u32 sample_count, f32 volume)
{
    /* Function to apply the volume for the sound */

    u32 i;  /* Sample index in the sound array */
    u32 j;  /* s16 element (16 bit) index in the sound array */
    u32 offset;  /* Offset of the word index in the array (0 for channel 1; 1 for channel 2) */
    s32 temp;  /* Temporary s32 variable to hold the sum of two s16 values */

    for (i = 0; i < sample_count; i++) {
        for (offset = 0; offset < 2; offset++) {
            
            /* Calculation of the s16 element index in the array */
            j = i * 2 + offset;  /* s16 element index of the channel in array */        
            temp = (s32)(round(sound[j] * volume));  

            if ((temp <= 32767) && (temp >= -32768)) {
                /* No overflow and underflow cases (most probable) */
                sound[j] = (s16)temp;
            } else if (temp > 32767) {
                /* Overflow case */
                sound[j] = 32767;
            } else {
                /* Underflow case */
                sound[j] = -32768;
            }
        }
    }
}

static void
mix_temp_array_to_mix_array(s16 *mix_array, s16 *sound, u32 sample_count)
{
    /* Function to mix sound into buffer mix array. Realization of a mixer */

    u32 i;  /* Sample index in the sound array */
    u32 j;  /* s16 element (16 bit) index in the sound array */ 
    u32 offset;  /* Offset of the word index in the array (0 for channel 1; 1 for channel 2) */
    s32 temp;  /* Temporary s32 variable to hold the sum of two s16 values */

    for (i = 0; i < sample_count; i++) {
        for (offset = 0; offset < 2; offset++) {
            
            /* Calculation of the s16 element index in the array */
            j = i * 2 + offset;  /* s16 element index of the channel in array */        
            temp = (s32)mix_array[j] + (s32)sound[j];

            if ((temp <= 32767) && (temp >= -32768)) {
                /* No overflow and underflow cases (most probable) */
                mix_array[j] = (s16)temp;
            } else if (temp > 32767) {
                /* Overflow case */
                mix_array[j] = 32767;
            } else {
                /* Underflow case */
                mix_array[j] = -32768;
            }
        }
    }
}

DWORD WINAPI 
sound_buffer_worker_proc(void *worker_data)
{
    /* Function of the thread procedure */
    /* Unpacking the worker_data */
    Sound_buffer_t *sound_buffer = ((Sound_Buffer_Worker_Data_t *)worker_data)->sound_buffer;
    Sound_t **sounds = ((Sound_Buffer_Worker_Data_t *)worker_data)->sounds;
    u32 e_snd_num = ((Sound_Buffer_Worker_Data_t *)worker_data)->e_sound_num;

    // Do continious update of the bufer
    while (true)
    {
        sound_buffer_update(sound_buffer, sounds, e_snd_num);
        /* Sound buffer should be update every 0.1 sec (100 miliseconds) */
        Sleep(80); /* 80 ms to be saved */
    }
    
    return 0;
}