/**
 * ================================================================================
 * @file src_engine/sound.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the Sound class methods.
 * @version 0.3
 * @date 2022-12-22
 * ================================================================================
 */

#include "include_engine/sound.h"

#include <math.h>
#include <stdlib.h>

#include "include_engine/audio.h"
#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/memory_object.h"
#include "include_engine/utils.h"
#include "include_engine/wav_decoder.h"

Sound*
Sound_Constructor(void)
{
    size_t size = sizeof(Sound);
    Sound *sound = (Sound *)HelperFcn_MemAllocate(size);
    return sound;
}

Sound*
Sound_Destructor(Sound *sound)
{
    HelperFcn_MemFree(sound->s16_array);
    HelperFcn_MemFree(sound);
    return NULL;
}

void
Sound_InitByMemObject(Sound *sound, const MemObject *wav_mem_object)
{
    /* Allocate memory for the buffer (s16_array). */
    size_t size = wav_mem_object->size * sizeof(s8);
    sound->s16_array = (s16 *)HelperFcn_MemAllocate(size);

    /* Decode data form memory object into the buffer. */
    WavDecoder_Decode(sound->s16_array, &(sound->channels_num), &(sound->samples_data_size), 
        &(sound->samples_per_second), wav_mem_object);

    /* Define sound parameters. */
    sound->bytes_per_sample = sound->channels_num * sizeof(s16);
    sound->sample_count = sound->samples_data_size / sound->bytes_per_sample;
    sound->s16_array_size = sound->sample_count * sound->channels_num; 
    sound->duration = (f32)sound->sample_count / sound->samples_per_second;
    sound->sample_index = 0;
    sound->is_playing = false;  /* Default */
    sound->is_looping = false;  /* Default */
}

void
Sound_LoadFromFile(Sound *sound, const char *file_path)
{
    MemObject *mem_object = MemObject_Constructor();
    MemObject_InitByFile(mem_object, file_path);
    Sound_InitByMemObject(sound, mem_object);
    MemObject_Destructor(mem_object);
}

void
Sound_PrepareEmptySound(Sound *sound)
{
    /* Parameters of empty sound [0.1 sec - 4410 samples]. */
    sound->sample_count = 4410;
    sound->channels_num = 2;
    sound->samples_data_size = 17640;  /* [bytes]. */
    sound->bytes_per_sample = 4;  /* [bytes/sample]. */
    sound->samples_per_second = 44100;  /* [samples/second]. */
    sound->duration = 0.1f;  /* [seconds]. */
    sound->is_looping = true; 
    sound->pan = S_PAN_BOTH;  /* Use both channels as a default. */
    sound->sample_index = 0;
    sound->s16_array_size = 8820;  /* Number of s16 (0xAABB) elements in s16_array. */

    /* Create empty array with s16 elements. */
    size_t size = sound->s16_array_size * sizeof(s16);
    sound->s16_array = (s16 *)HelperFcn_MemAllocate(size);
}

void
Sound_PlaySoundOnce(Sound *sound, SoundPan pan, f32 volume)
{
    sound->is_looping = false;
    sound->sample_index = 0;
    sound->is_playing = true;
    sound->pan = pan;
    sound->volume = volume;
}

void 
Sound_PlaySoundContiniously(Sound *sound, SoundPan pan, f32 volume)
{
    sound->is_looping = true;
    sound->sample_index = 0;
    sound->is_playing = true;
    sound->pan = pan;
    sound->volume = volume;
}

void 
Sound_StopSound(Sound *sound)
{
    sound->is_playing = false;
}

void
Sound_ContinueSound(Sound *sound, SoundPan pan, f32 volume)
{
    sound->is_playing = true;
    sound->pan = pan;
    sound->volume = volume;
}
