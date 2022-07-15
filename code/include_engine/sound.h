/**
 * ================================================================================
 * @file sound.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Sound class methods.
 * @version 0.1
 * @date 2022-01-09
 * @copyright Copyright (c) 2022
 * ================================================================================ 
 */

#ifndef SOUND_H_
#define SOUND_H_

/* Program includes: */
#include <utils.h>

/* Predefined structures: */
typedef struct File File_t;

/**
 * @brief Enumerator for sound pan (what channel is used to play the sound. 
 */
enum Sound_Pan
{
    S_PAN_LEFT, /* Use only left channel */
    S_PAN_RIGHT, /* Use only right channel */
    S_PAN_BOTH, /* Use both channels */
};
typedef enum Sound_Pan Sound_Pan_t;

/**
 * @brief Structure for uncompressed sound holding the memory.
 */
struct Sound 
{
    u32 sample_count;  /**< Number of samples (number of 0xAABBCCDD elements). */
    u32 channels_num;  /**< Number of channels (2). */   
    u32 samples_data_size;  /**< Sound size in bytes (number of 0xAA elements). */
    u32 bytes_per_sample;  /** Number of bytes per sample. */
    u32 samples_per_second;  /**< Number of samples per second play. */
    f32 duration;  /**< Duration of the sound in seconds. */
    b32 is_playing;  /**< Flag to check whether the sound is playing or not at the moment. */
    b32 is_looping;  /**< Flag to determine whether the sound is continuous or not. */
    u32 sample_index;  /**< Index of the currently playing sample (0xAABBCCDD element). */
    s16 *s16_array;  /**< Array of uncompressed samples of the sound in memory. */
    u32 s16_array_size;  /**< Number of s16 (0xAABB) elements in s16_array. */
    Sound_Pan_t pan;  /**< Currently playing channel. */
    f32 volume;  /**< Volume of the sound (1.0 - normal default volume). */
};
typedef struct Sound Sound_t;

/**
 * @brief Constructor of the Sound class.
 * @return Sound_t* Pointer to the Sound structure.
 */
Sound_t*
sound_constructor(void);

/**
 * @brief Destructor of the Sound class.
 * @param sound Pointer to the Sound structure.
 */
void
sound_destructor(Sound_t *sound);

/**
 * @brief Uncompress wav file into Sound data suitable for processing.
 * @param sound Pointer to the Sound structure.
 * @param file Pointer to the File structure of loaded wav file.
 */
void
/* TODO: sound_init */
sound_uncompress_wav(Sound_t *sound, File_t *file);

/**
 * @brief Preparation of the empty sound.
 * @param sound Pointer to the Sound structure with empty sound.
 */
void sound_prepare_empty_sound(Sound_t *sound);

/**
 * @brief Play of the sound one time only.
 * @param sound Pointer to the Sound structure.
 * @param pan Sound pan (left, right ot both channels).
 * @param volume Volume of the sound.
 */
void sound_play_sound_once(Sound_t *sound, Sound_Pan_t pan, f32 volume);

/**
 * @brief Play of the sound continiously.
 * @param sound Pointer to the Sound structure.
 * @param pan Sound pan (left, right ot both channels).
 * @param volume Volume of the sound.
 */
void sound_play_sound_continiously(Sound_t *sound, Sound_Pan_t pan, f32 volume);

/**
 * @brief Immidiately stop playing of the sound.
 * @param sound Pointer to the Sound structure.
 */
void sound_stop_sound(Sound_t *sound);

/**
 * @brief Continue playing of the sound with possible new play parameters.
 * @param sound Pointer to the Sound structure.
 * @param pan Sound pan (left, right ot both channels).
 * @param volume Volume of the sound.
 */
void sound_continue_sound(Sound_t *sound, Sound_Pan_t pan, f32 volume);

#endif //SOUND_H
