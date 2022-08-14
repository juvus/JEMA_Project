/**
 * ================================================================================
 * @file wav_uncompressor.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the Wav_uncompressor class methods.
 * @version 0.2
 * @date 2022-01-09
 * ================================================================================ 
 */

/* Standard includes: */
#include <assert.h>

/* Program includes: */
#include "wav_uncompressor.h"
#include "sound.h"
#include "file_io.h"
#include "utils.h"

static inline Riff_iterator_t
parse_chunk_at(void *at, void* stop)
{   
    Riff_iterator_t iterator;   
    iterator.at = (u8*)at;
    iterator.stop = (u8*)stop;
    return iterator;
}

static inline b32
is_riff_iterator_valid(Riff_iterator_t iterator)
{    
    return iterator.at < iterator.stop;
}

static inline Riff_iterator_t
next_chunk(Riff_iterator_t iterator)
{   
    Wav_chunk_t *chunk = (Wav_chunk_t*)iterator.at;
    u32 size = (chunk->size+1) & ~1;
    iterator.at += sizeof(Wav_chunk_t) + size;
    return iterator;
}

static inline void*
get_chunk_data(Riff_iterator_t iterator)
{    
    return iterator.at + sizeof(Wav_chunk_t);
}

static inline u32
get_type(Riff_iterator_t iterator)
{  
    Wav_chunk_t *chunk = (Wav_chunk_t*)iterator.at;
    return chunk->id;
}

static inline u32
get_chunk_data_size(Riff_iterator_t iterator)
{    
    Wav_chunk_t *chunk = (Wav_chunk_t*)iterator.at;
    return chunk->size;
}

void
wav_uncompressor_uncompress(Sound_t *sound, File_t* file)
{       
    Wav_header_t *header = (Wav_header_t*)file->data;
    Wav_format_t *format = NULL;
    assert(header->riff_id == Wav_chunk_id_riff);
    assert(header->wave_id == Wav_chunk_id_wave);
    

    s16* sample_data = 0;
    Riff_iterator_t iterator = {0};
    
    for (iterator = parse_chunk_at(header+1, (u8*)(header+1) + header->size - 4); 
        is_riff_iterator_valid(iterator); iterator = next_chunk(iterator)) 
    {    
        switch(get_type(iterator)) 
        {            
            case Wav_chunk_id_fmt: 
            {
                format = (Wav_format_t*) get_chunk_data(iterator);
                assert(format->format_tag == 1); //pcm
                assert(format->samples_per_second == 44100);
                assert(format->bits_per_sample == 16); /* per channel */
                //assert(format->block_align == (sizeof(s16)*format->num_channels));
                assert(format->num_channels == 1 || format->num_channels == 2);
                sound->channels_num = format->num_channels;
            } break;
            
            case Wav_chunk_id_data: 
            {
                sample_data = (s16*)get_chunk_data(iterator);
                sound->samples_data_size = get_chunk_data_size(iterator);
            } break;
        }
    }
    assert(sound->channels_num && sample_data && sound->samples_data_size);
    
    sound->bytes_per_sample = sound->channels_num * sizeof(s16);
    sound->sample_count = sound->samples_data_size / sound->bytes_per_sample;
    
    /* Allocate memory for s16_array and fill with data. */
    sound->s16_array = (s16 *)calloc(file->size, sizeof(s8));
    memcpy(sound->s16_array, sample_data, (file->size * sizeof(s8)));
    
    sound->s16_array_size = sound->sample_count * sound->channels_num;
    sound->samples_per_second = format->samples_per_second; /* 44100 */
    sound->duration = (f32)sound->sample_count / sound->samples_per_second;
    sound->sample_index = 0;
    sound->is_playing = false; /* Default */
    sound->is_looping = false; /* Default */
}
