/*================================================================================*/
/* Header file for the game audio_buffer class                                    */
/*================================================================================*/

/* TODO: Sound_buffer => Audio */



#ifndef SOUND_BUFFER_H_
#define SOUND_BUFFER_H_

#include <windows.h>
#include <dsound.h>
#include <sound.h>

/* Structure for the sound buffer */
struct Sound_buffer {
    LPDIRECTSOUNDBUFFER primary_buffer; /* Primary buffer */
    LPDIRECTSOUNDBUFFER buffer; /* Secondary buffer: Uncompressed wav data ready to play */
    u32 size; /* Buffer size in bytes */
    u32 channels_num; /* Number of channels */
    u32 samples_per_second; /* Number of samples per channel */
    u32 bytes_per_sample; /* Number of bytes per sample */
    DWORD current_play_cursor; /* Offset in bytes of the current play cursor */
    DWORD current_write_cursor; /* Offset in bytes of current write (safe) cursor */
    DWORD update_cursor; /* Offset in bytes from we will write new write_size batch of data */
    DWORD border_cursor; /* Offset in bytes after witch the new batch should be loaded */
    DWORD write_size; /* Size in bytes of the loading batch of data */
    u32 s16_array_size; /* Amount of the s16 elements in arrays (temp_array and mix_array) */
    u32 s32_array_size; /* Amount of the s32 elements (=samples) in arrays */
    s16 *temp_array; /* Temparory array for help sound mixing routine (0.1 sec) */
    s16 *mix_array; /* Array of mix of all played sound ready to upload to the buffer (0.1 sec) */
};
typedef struct Sound_buffer Sound_buffer_t; 

typedef HRESULT direct_sound_create_t (LPGUID lpGuid, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter); 

/* Multithreading */
/* Structure with data, that need to be pased to the thread */
struct Sound_Buffer_Worker_Data
{
    Sound_buffer_t *sound_buffer;  /* Pointer to the sound buffer */
    Sound_t **sounds;  /* Pointer to the array of pointers to sound structures */
    u32 e_sound_num;  /* Amount of sound structures in the array */
};
typedef struct Sound_Buffer_Worker_Data Sound_Buffer_Worker_Data_t;

/* Function to initialize the audio */
void sound_buffer_init(HWND window, Sound_buffer_t *sound_buffer);

/* Function to clear the sound buffer */
void sound_buffer_clear_buffer(Sound_buffer_t *sound_buffer);

/* Function to play the audio */
void sound_buffer_play(Sound_buffer_t *sound_buffer);

/* Function to update the sounds in the buffer */
void sound_buffer_update(Sound_buffer_t *sound_buffer, Sound_t *sounds[], u32 e_sound_num);

/* Function to clear the specific s16 array of the buffer */
void sound_buffer_clear_s16_array(Sound_buffer_t *sound_buffer, s16 *array);

/* Function of the thread procedure */
DWORD WINAPI sound_buffer_worker_proc(void *worker_data); 

#endif //SOUND_BUFFER_H_
