/**
 * ================================================================================
 * @file include_engine/audio_worker.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with audio
 * worker. Worker procedure is executing in a separate thread.
 * @version 0.2
 * @date 2022-11-27
 * ================================================================================
 */

#ifndef JEMA_ENGINE_AUDIO_WORKER_H_
#define JEMA_ENGINE_AUDIO_WORKER_H_

#include <windows.h>
#include "include_engine/utils.h"

typedef struct Audio_ Audio;
typedef struct Sound_ Sound;

/**
 * @brief Structure with data, that need to be pased to the thread (multithreading).
 */
struct AudioWorker_
{
    Audio *audio;  /**< Pointer to the Audio structure. */
    Sound **sounds;  /**< Pointer to the array of pointers to Sound structures. */
    u32 sound_num;  /**< Amount of sound structures in the array. */
};
typedef struct AudioWorker_ AudioWorker;

/**
 * @brief Object constructor.
 * @return AudioWorker* Pointer to the AudioWorker structure. 
 */
AudioWorker*
AudioWorker_Constructor(void);

/**
 * @brief Object destructor.
 * @param audio_worker AudioWorker* Pointer to the AudioWorker structure.
 * @return AudioWorker* Pointer to the AudioWorker structure.
 */
AudioWorker*
AudioWorker_Destructor(AudioWorker *audio_worker);

/**
 * @brief Object initialization.
 * @param audio_worker Pointer to the AudioWorker structure.
 * @param audio Pointer to the Audio structure.
 * @param sounds Array of pointers to Sound structures.
 * @param sound_num Number of passed sound objects.
 */
void
AudioWorker_Init(AudioWorker *audio_worker, Audio *audio, Sound *sounds[], 
    u32 sound_num);

/**
 * @brief Audio worker procedure to be executed in the separated thread.
 * @param audio_worker Pointer to the data sent to the thread.
 */
DWORD WINAPI 
AudioWorker_ThreadProc(void *audio_worker);

#endif  /* JEMA_ENGINE_AUDIO_WORKER_H_ */
