/**
 * ================================================================================
 * @file audio_worker.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with audio worker. Worker
 * procedure is executes in a separate thread.
 * the game.
 * @version 0.5
 * @date 2022-07-19
 * ================================================================================ 
 */

/* Standard library includes: */
#include <windows.h>

/* Game engine includes: */
#include "audio.h"
#include "audio_worker.h"
#include "sound.h"
#include "utils.h"

Audio_Worker_t*
audio_worker_constructor(void)
{
    Audio_Worker_t *audio_worker;  /* Pointer to the Audio_Worker structure. */

    /* Allocate the memory for the Audio_Worker structure. */
    audio_worker = (Audio_Worker_t *)malloc(1 * sizeof(Audio_Worker_t));
    
    return audio_worker;
}

void
audio_worker_destructor(Audio_Worker_t *audio_worker)
{
    /* Release memory allocated for the Audio_Worker structure. */
    free(audio_worker);
    audio_worker = NULL;
}

void
audio_worker_init(Audio_Worker_t *audio_worker, Audio_t *audio, Sound_t **sounds,
    u32 sound_num)
{
    audio_worker->audio = audio;
    audio_worker->sounds = sounds;
    audio_worker->sound_num = sound_num;
}

DWORD WINAPI 
audio_worker_proc(void *audio_worker)
{
    Audio_t *audio;  /* Pointer to the Audio structure. */
    Sound_t **sounds;  /* Pointer to the array of pointers to Sound structures. */
    u32 sound_num;  /* Amount of sound structures in the array. */
    
    audio = ((Audio_Worker_t *)audio_worker)->audio;
    sounds = ((Audio_Worker_t *)audio_worker)->sounds;
    sound_num = ((Audio_Worker_t *)audio_worker)->sound_num;

    // NOTE: Try to make a normal end of the procedure. For example we can check the 
    // variable showing the current run of the game. If the game stopped than we end the 
    // the current procedure.

    /* Make continious update of the bufer in the separate thread. */
    while (true)
    {
        audio_update_buffer(audio, sounds, sound_num);
        /* Audio buffer should be updated for every 0.1 sec (100 miliseconds). */
        Sleep(80);  /* Sleep for 80 ms to be safe (less than 100 miliseconds). */
    }
    
    return 0;
}