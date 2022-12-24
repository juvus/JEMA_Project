/**
 * ================================================================================
 * @file src_engine/wav_decoder.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for decoding the audio files encoded 
 * with WAV format.
 * file spec: .wav PCM 16-bits per sample, 1 or 2 channels (interleaved rlrlrlr)
 * @version 0.2
 * @date 2022-12-21
 * ================================================================================ 
 */

#include "include_engine/wav_decoder.h"

#include "include_engine/dbg.h"
#include "include_engine/memory_object.h"
#include "include_engine/sound.h"
#include "include_engine/utils.h"

static inline RiffIterator
ParseChunkAt(void *at, void* stop)
{   
    RiffIterator iterator;   
    iterator.at = (u8*)at;
    iterator.stop = (u8*)stop;
    return iterator;
}

static inline b32
IsRiffIteratorValid(RiffIterator iterator)
{    
    return iterator.at < iterator.stop;
}

static inline RiffIterator
NextChunk(RiffIterator iterator)
{   
    WavChunk *chunk = (WavChunk *)iterator.at;
    u32 size = (chunk->size + 1) & ~1;
    iterator.at += sizeof(WavChunk) + size;
    return iterator;
}

static inline void*
GetChunkData(RiffIterator iterator)
{    
    return iterator.at + sizeof(WavChunk);
}

static inline u32
GetType(RiffIterator iterator)
{  
    WavChunk *chunk = (WavChunk *)iterator.at;
    return chunk->id;
}

static inline u32
GetChunkDataSize(RiffIterator iterator)
{    
    WavChunk *chunk = (WavChunk *)iterator.at;
    return chunk->size;
}

void
WavDecoder_Decode(Sound *sound, MemObject* wav_mem_object)
{       
    WavHeader *header = (WavHeader *)wav_mem_object->data;
    WavFormat *format = NULL;
    dbg_check(header->riff_id == Wav_chunk_id_riff, "");
    dbg_check(header->wave_id == Wav_chunk_id_wave, "");

    s16* sample_data = 0;
    RiffIterator iterator = {0};
    
    for (iterator = ParseChunkAt(header + 1, (u8 *)(header + 1) + header->size - 4); 
        IsRiffIteratorValid(iterator); iterator = NextChunk(iterator)) 
    {    
        switch(GetType(iterator)) 
        {            
            case Wav_chunk_id_fmt: 
            {
                format = (WavFormat *) GetChunkData(iterator);
                dbg_check(format->format_tag == 1, "");  /* pcm. */
                dbg_check(format->samples_per_second == 44100, "");
                dbg_check(format->bits_per_sample == 16, "");
                dbg_check(format->num_channels == 1 || format->num_channels == 2, "");
                sound->channels_num = format->num_channels;
            } break;
            
            case Wav_chunk_id_data: 
            {
                sample_data = (s16 *)GetChunkData(iterator);
                sound->samples_data_size = GetChunkDataSize(iterator);
            } break;
        }
    }
    dbg_check(sound->channels_num && sample_data && sound->samples_data_size, "");
    sound->bytes_per_sample = sound->channels_num * sizeof(s16);
    sound->sample_count = sound->samples_data_size / sound->bytes_per_sample;
    
    /* Allocate memory for s16_array and fill with data. */
    sound->s16_array = (s16 *)calloc(wav_mem_object->size, sizeof(s8));
    memcpy(sound->s16_array, sample_data, (wav_mem_object->size * sizeof(s8)));
    
    sound->s16_array_size = sound->sample_count * sound->channels_num;
    sound->samples_per_second = format->samples_per_second;  /* 44100 */
    sound->duration = (f32)sound->sample_count / sound->samples_per_second;
    sound->sample_index = 0;
    sound->is_playing = false;  /* Default */
    sound->is_looping = false;  /* Default */
}
