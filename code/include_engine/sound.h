/**
 * ================================================================================
 * @file include_engine/sound.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Sound class methods.
 * @version 0.2
 * @date 2022-12-21
 * ================================================================================ 
 */

#ifndef JEMA_ENGINE_SOUND_H_
#define JEMA_ENGINE_SOUND_H_

#include "include_engine/utils.h"

typedef struct MemObject_ MemObject;

/**
 * @brief Enumerator for sound pan (what channel is used to play the sound. 
 */
enum SoundPan_
{
    S_PAN_LEFT,  /**< Use only left channel. */
    S_PAN_RIGHT,  /**< Use only right channel. */
    S_PAN_BOTH  /**< Use both channels. */
};
typedef enum SoundPan_ SoundPan;

/**
 * @brief Structure for uncompressed sound holding the memory.
 */
struct Sound_ 
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
    SoundPan pan;  /**< Currently playing channel. */
    f32 volume;  /**< Volume of the sound (1.0 - normal default volume). */
};
typedef struct Sound_ Sound;

/**
 * @brief Object constructor.
 * @return Sound* Pointer to the Sound structure.
 */
Sound*
Sound_Constructor(void);

/**
 * @brief Object destructor.
 * @param sound Pointer to the Sound structure.
 */
void
Sound_Destructor(Sound *sound);

/**
 * @brief Initioalization of the sound. Uncompression of the wav file into Sound data
 * suitable for further processing.
 * @param sound Pointer to the Sound structure.
 * @param file Pointer to the File structure of loaded wav file.
 */
void
Sound_Init(Sound *sound, MemObject *wav_object);

/**
 * @brief Preparation of the empty sound.
 * @param sound Pointer to the Sound structure with empty sound.
 */
void Sound_PrepareEmptySound(Sound *sound);

/**
 * @brief Play of the sound one time only.
 * @param sound Pointer to the Sound structure.
 * @param pan Sound pan (left, right ot both channels).
 * @param volume Volume of the sound.
 */
void Sound_PlaySoundOnce(Sound *sound, SoundPan pan, f32 volume);

/**
 * @brief Play of the sound continiously.
 * @param sound Pointer to the Sound structure.
 * @param pan Sound pan (left, right ot both channels).
 * @param volume Volume of the sound.
 */
void Sound_PlaySoundContiniously(Sound *sound, SoundPan pan, f32 volume);

/**
 * @brief Immidiately stop playing of the sound.
 * @param sound Pointer to the Sound structure.
 */
void Sound_StopSound(Sound *sound);

/**
 * @brief Continue playing of the sound with possible new play parameters.
 * @param sound Pointer to the Sound structure.
 * @param pan Sound pan (left, right ot both channels).
 * @param volume Volume of the sound.
 */
void Sound_ContinueSound(Sound *sound, SoundPan pan, f32 volume);

#endif  /* JEMA_ENGINE_SOUND_H */
