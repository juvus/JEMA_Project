/**
 * ================================================================================
 * @file audio_worker.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with audio
 *  worker. Worker procedure is executes in a separate thread.
 * @version 0.1
 * @date 2022-07-17
 * ================================================================================
 */

#ifndef AUDIO_WORKER_H_
#define AUDIO_WORKER_H_

/* Game engine includes: */
#include <utils.h>

/* Predefined structures: */
typedef struct Sound Sound_t;

/**
 * @brief Structure with data, that need to be pased to the thread (multithreading).
 */
struct Audio_Worker_Data
{
    Audio_t *audio;  /**< Pointer to the Audio structure. */
    Sound_t **sounds;  /**< Pointer to the array of pointers to Sound structures. */
    u32 sound_num;  /**< Amount of sound structures in the array. */
};
typedef struct Audio_Worker_Data Audio_Worker_Data_t;







/**
 * @brief 
 * 
 */


/* Function of the thread procedure */
DWORD WINAPI sound_buffer_worker_proc(void *worker_data); 















#endif //AUDIO_WORKER_H_

