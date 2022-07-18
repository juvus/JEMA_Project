/**
 * ================================================================================
 * @file audio_worker.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with audio worker. Worker
 * procedure is executes in a separate thread.
 * the game.
 * @version 0.5
 * @date 2022-07-17
 * ================================================================================ 
 */

/* Standard library includes: */




/* Game engine includes: */






DWORD WINAPI 
sound_buffer_worker_proc(void *worker_data)
{
    /* Function of the thread procedure */
    /* Unpacking the worker_data */
    Audio_buffer_t *sound_buffer = ((Sound_Buffer_Worker_Data_t *)worker_data)->sound_buffer;
    Sound_t **sounds = ((Sound_Buffer_Worker_Data_t *)worker_data)->sounds;
    u32 e_snd_num = ((Sound_Buffer_Worker_Data_t *)worker_data)->e_sound_num;

    // Do continious update of the bufer
    while (true)
    {
        sound_buffer_update(sound_buffer, sounds, e_snd_num);
        /* Sound buffer should be update every 0.1 sec (100 miliseconds) */
        Sleep(80); /* 80 ms to be saved */
    }
    
    return 0;
}