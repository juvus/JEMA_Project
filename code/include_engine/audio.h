/**
 * ================================================================================
 * @file audio.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with audio
 * system of the game.
 * @version 0.2
 * @date 2022-07-16
 * ================================================================================
 */

#ifndef AUDIO_H_
#define AUDIO_H_

/* Standard library includes: */
#include <windows.h>
#include <dsound.h>

/* Game engine includes: */
#include "utils.h"

/* Predefined structures: */
typedef struct Sound Sound_t;

/**
 * @brief Structure for the Audio object.
 */
struct Audio 
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
typedef struct Audio Audio_t; 

/* Declare pointer to the function from dll. */
typedef HRESULT direct_sound_create_t(LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter); 

/**
 * @brief Class constructor.
 * @return Audio_t* Pointer to the Render structure. 
 */
Audio_t*
audio_constructor(void);

/**
 * @brief Class destructor.
 * @param audio Pointer to the Audio structure.
 */
void
audio_destructor(Audio_t *audio);

/**
 * @brief Object initialization.
 * @param audio Pointer to the Audio structure.
 * @param window Handle to the game window.
 */
void
audio_init(Audio_t *audio, HWND window);

/**
 * @brief Cleaning the audio buffer.
 * @param audio Pointer to the Audio structure.
 */
void
audio_clean_buffer(Audio_t *audio);

/**
 * @brief Play the game sounds.
 * @param audio Pointer to the Audio structure.
 */
void
audio_play_sounds(Audio_t *audio);

/**
 * @brief Updating the sounds data in the audio buffer.
 * @param audio Pointer to the Audio structure.
 * @param sounds Pointer to the array of pointers to Sound structures.
 * @param sound_num Amount of sound structures in the array.
 */
void
audio_update_buffer(Audio_t *audio, Sound_t *sounds[], u32 sound_num);

#endif //AUDIO_H_
