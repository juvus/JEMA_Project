/**
 * ================================================================================
 * @file include_engine/audio.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with audio
 * system of the game.
 * @version 0.2
 * @date 2022-11-27
 * ================================================================================
 */

#ifndef JEMA_ENGINE_AUDIO_H_
#define JEMA_ENGINE_AUDIO_H_

#include <windows.h>
#include <dsound.h>

#include "include_engine/utils.h"

typedef struct Sound_ Sound;

/**
 * @brief Structure for the Audio object.
 */
struct Audio_ 
{
    LPDIRECTSOUNDBUFFER primary_buffer;  /**< Primary buffer. */
    LPDIRECTSOUNDBUFFER buffer;  /**< Secondary buffer: Uncompressed wav data ready to play. */
    u32 size;  /**< Buffer size in bytes. */
    u32 channels_num;  /**< Number of channels. */
    u32 samples_per_second;  /**< Number of samples per second. */
    u32 bytes_per_sample;  /**< Number of bytes per sample. */
    DWORD current_play_cursor;  /**< Offset in bytes of the current play cursor. */
    DWORD current_write_cursor;  /**< Offset in bytes of current write (safe) cursor. */
    DWORD update_cursor;  /**< Offset in bytes from we will write new write_size batch of data. */
    DWORD border_cursor;  /**< Offset in bytes after witch the new batch should be loaded. */
    DWORD write_size;  /**< Size in bytes of the loading batch of data. */
    u32 s16_array_size;  /**< Amount of the s16 elements in arrays (temp_array and mix_array). */
    u32 s32_array_size;  /**< Amount of the s32 elements (=samples) in arrays. */
    s16 *temp_array;  /**< Temparory array for help sound mixing routine (0.1 sec). */
    s16 *mix_array;  /**< Array of mix of all played sound ready to upload to the buffer. */
};
typedef struct Audio_ Audio;

/* Declare pointer to the function from dll. */
typedef HRESULT direct_sound_create_t(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter); 

/**
 * @brief Object constructor.
 * @return Audio* Pointer to the Audio structure. 
 */
Audio*
Audio_Constructor(void);

/**
 * @brief Object destructor.
 * @param audio Pointer to the Audio structure.
 * @return Audio* Pointer to the Audio structure. 
 */
Audio*
Audio_Destructor(Audio *audio);

/**
 * @brief Object initialization.
 * @param audio Pointer to the Audio structure.
 * @param window Handle to the game window.
 */
void
Audio_Init(Audio *audio, HWND window);

/**
 * @brief Cleaning the audio buffer.
 * @param audio Pointer to the Audio structure.
 */
void
Audio_CleanBuffer(Audio *audio);

/**
 * @brief Play the game sounds.
 * @param audio Pointer to the Audio structure.
 */
void
Audio_PlaySounds(Audio *audio);

/**
 * @brief Updating the sounds data in the audio buffer.
 * @param audio Pointer to the Audio structure.
 * @param sounds Pointer to the array of pointers to Sound structures.
 * @param sound_num Amount of sound structures in the array.
 */
void
Audio_UpdateBuffer(Audio *audio, Sound *sounds[], u32 sound_num);

#endif  /* JEMA_ENGINE_AUDIO_H_ */
