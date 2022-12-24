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
#include "include_engine/memory_object.h"
#include "include_engine/utils.h"
#include "include_engine/wav_decoder.h"

Sound*
Sound_Constructor(void)
{
    Sound *sound = (Sound *)malloc(1 * sizeof(Sound));
    if (sound == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return sound;
}

void
Sound_Destructor(Sound *sound)
{
    if (sound == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    free(sound->s16_array);
    free(sound);
    sound = NULL;
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
    sound->s16_array = (s16 *)calloc(sound->s16_array_size, sizeof(s16));
}

void
Sound_Init(Sound *sound, MemObject *wav_object)
{
    WavDecoder_Decode(sound, wav_object);
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
