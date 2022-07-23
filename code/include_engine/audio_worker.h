/**
 * ================================================================================
 * @file audio_worker.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with audio
 *  worker. Worker procedure is executes in a separate thread.
 * @version 0.1
 * @date 2022-07-19
 * ================================================================================
 */

#ifndef AUDIO_WORKER_H_
#define AUDIO_WORKER_H_

/* Standard library includes. */
#include <windows.h>

/* Game engine includes. */
#include <utils.h>

/* Predefined structures: */
typedef struct Audio Audio_t;
typedef struct Sound Sound_t;

/**
 * @brief Structure with data, that need to be pased to the thread (multithreading).
 */
struct Audio_Worker
{
    Audio_t *audio;  /**< Pointer to the Audio structure. */
    Sound_t **sounds;  /**< Pointer to the array of pointers to Sound structures. */
    u32 sound_num;  /**< Amount of sound structures in the array. */
};
typedef struct Audio_Worker Audio_Worker_t;

/**
 * @brief Constructor of the Audio_Worker class.
 * @return Audio_Worker_t* Pointer to the Audio_Worker structure. 
 */
Audio_Worker_t*
audio_worker_constructor(void);

/**
 * @brief Destructor of the Audio_Worker object.
 * @param audio_worker Pointer to the Audio_Worker structure.
 */
void
audio_worker_destructor(Audio_Worker_t *audio_worker);

/**
 * @brief Initialization of the Audio_Worker object.
 * @param audio_worker Pointer to the Audio_Worker structure.
 */
void
audio_worker_init(Audio_Worker_t *audio_worker, Audio_t *audio, Sound_t **sounds,
    u32 sound_num);

/**
 * @brief Audio worker procedure to be executed in the separate thread. 
 */
DWORD WINAPI 
audio_worker_proc(void *audio_worker);

#endif //AUDIO_WORKER_H_
