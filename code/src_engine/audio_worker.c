/**
 * ================================================================================
 * @file src_engine/audio_worker.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with audio worker. Worker
 * procedure is executes in a separate thread.
 * the game.
 * @version 0.2
 * @date 2022-11-27
 * ================================================================================ 
 */

#include "include_engine/audio_worker.h"

#include <windows.h>

#include "include_engine/audio.h"
#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/sound.h"
#include "include_engine/utils.h"

AudioWorker*
AudioWorker_Constructor(void)
{
    size_t size = sizeof(AudioWorker);
    AudioWorker *audio_worker = (AudioWorker *)HelperFcn_MemAllocate(size);
    return audio_worker;
}

AudioWorker*
AudioWorker_Destructor(AudioWorker *audio_worker)
{
    HelperFcn_MemFree(audio_worker);
    return NULL;
}

void
AudioWorker_Init(AudioWorker *audio_worker, Audio *audio, Sound *sounds[], 
    u32 sound_num)
{
    audio_worker->audio = audio;
    audio_worker->sounds = sounds;
    audio_worker->sound_num = sound_num;
}

DWORD WINAPI 
AudioWorker_ThreadProc(void *audio_worker)
{
    Audio *audio = ((AudioWorker *)audio_worker)->audio;
    Sound **sounds = ((AudioWorker *)audio_worker)->sounds;
    u32 sound_num = ((AudioWorker *)audio_worker)->sound_num;

    // NOTE: Try to make a normal end of the procedure. For example we can check the 
    // variable showing the current run of the game. If the game stopped than we end the 
    // the current procedure.

    /* Make continious update of the bufer in the separate thread. */
    while (true)
    {
        Audio_UpdateBuffer(audio, sounds, sound_num);
        /* Audio buffer should be updated for every 0.1 sec (100 miliseconds). */
        Sleep(80);  /* Sleep for 80 ms to be safe (less than 100 miliseconds). */
    }
    return 0;
}
