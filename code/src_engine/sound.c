/**
 * ================================================================================
 * @file sound.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the Sound class methods.
 * @version 0.2
 * @date 2022-01-09
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

/* Standard library includes. */
#include <stdlib.h>
#include <math.h>

/* Game engine includes. */
#include <utils.h>
#include <sound.h>
#include <file_io.h>
#include <utils.h>
#include <audio.h>
#include <wav_uncompressor.h>

Sound_t*
sound_constructor(void)
{
    Sound_t *sound;  /* Pointer to the Sound structure. */

    /* Allocation the memory for the image object. */
    sound = (Sound_t*)malloc(1 * sizeof(Sound_t));
    return sound;
}

void
sound_destructor(Sound_t *sound)
{
    free(sound->s16_array);
    free(sound);
}

void
sound_prepare_empty_sound(Sound_t *sound)
{
    /* Parameters of empty sound [0.1 sec - 4410 samples]. */
    sound->sample_count = 4410; /* [samples] */
    sound->channels_num = 2; /* [channels] */ 
    sound->samples_data_size = 17640; /* [bytes] */
    sound->bytes_per_sample = 4; /* [bytes/sample] */
    sound->samples_per_second = 44100; /* [samples/second] */
    sound->duration = 0.1f; /* [seconds] */
    sound->is_looping = true; 
    sound->pan = S_PAN_BOTH; /* Use both channels as a default */
    sound->sample_index = 0;
    sound->s16_array_size = 8820;  /* Number of s16 (0xAABB) elements in s16_array */

    /* Create empty array with s16 elements */
    sound->s16_array = (s16 *)calloc(sound->s16_array_size, sizeof(s16));
}

void
sound_init(Sound_t *sound, File_t *file)
{
    wav_uncompressor_uncompress(sound, file);
}

void
sound_play_sound_once(Sound_t *sound, Sound_Pan_t pan, f32 volume)
{
    sound->is_looping = false;
    sound->sample_index = 0;
    sound->is_playing = true;
    sound->pan = pan;
    sound->volume = volume;
}

void 
sound_play_sound_continiously(Sound_t *sound, Sound_Pan_t pan, f32 volume)
{
    sound->is_looping = true;
    sound->sample_index = 0;
    sound->is_playing = true;
    sound->pan = pan;
    sound->volume = volume;
}

void 
sound_stop_sound(Sound_t *sound)
{
    sound->is_playing = false;
}

void
sound_continue_sound(Sound_t *sound, Sound_Pan_t pan, f32 volume)
{
    sound->is_playing = true;
    sound->pan = pan;
    sound->volume = volume;
}
