/**
 * ================================================================================
 * @file include_engine/wav_decoder.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for decoding the audio
 * files encoded with WAV format.
 * file spec: .wav PCM 16-bits per sample, 1 or 2 channels (interleaved rlrlrlr)
 * @version 0.2
 * @date 2022-12-21
 * ================================================================================ 
 */

#ifndef JEMA_ENGINE_WAV_DECODER_H_
#define JEMA_ENGINE_WAV_DECODER_H_

#include "include_engine/utils.h"

typedef struct MemObject_ MemObject;
typedef struct Sound_ Sound;

/* Specification of the packing alignment for structure. */
#pragma pack(push, 1)

/**
 * @brief Structure for Wav_header. 
 */
struct WavHeader_
{  
    u32 riff_id;  /**< Id of the riff. */
    u32 size;  /**< Size. */
    u32 wave_id;  /**< Id of the wave. */
};
typedef struct WavHeader_ WavHeader;

/* Macros for the riff code. */
#define RIFF_CODE(a, b, c, d) \
    (((u32)(a) << 0) | ((u32)(b) << 8) | ((u32)(c) << 16) | ((u32)(d) << 24))

/**
 * @brief Enumerator for holding different ID's.
 */
enum
{
    Wav_chunk_id_fmt = RIFF_CODE('f', 'm', 't', ' '),
    Wav_chunk_id_riff = RIFF_CODE('R', 'I', 'F', 'F'),
    Wav_chunk_id_wave = RIFF_CODE('W', 'A', 'V', 'E'),
    Wav_chunk_id_data = RIFF_CODE('d', 'a', 't', 'a')
};

/**
 * @brief Structure for Wav_Chunk.
 */
struct WavChunk_
{
    u32 id;  /**< Id of the chunk. */
    u32 size;  /**< Chunk size. */
};
typedef struct WavChunk_ WavChunk;

/**
 * @brief Structure for Wav format. 
 */
struct WavFormat_
{
    u16 format_tag;  /**< Wav format tag. */
    u16 num_channels;  /**< Number of channels. */
    u32 samples_per_second;  /**< Number of samples per channel. */
    u32 avg_bytes_per_second;  /**< Average number of bytes per second. */
    u16 block_align;  /**< Align of the block. */
    u16 bits_per_sample;  /**< Number of bits per sample. */
    u16 cb_size;  /**< cd_size. */
    u16 valid_bits_sample;  /**< valid_bits_sample. */
    u32 dw_channel_mask;  /**< dw_channel_mask. */
    u8 sub_format[16];  /**< sub_format array. */
};
typedef struct WavFormat_ WavFormat;

/* Remove the record from the top of the internal compiler stack. */
#pragma pack(pop)

/**
 * @brief Structure for Riff Iterator.
 */
struct RiffIterator_
{
    u8* at;  /**< Pointer to current position. */
    u8* stop;  /**< Pointer to stop position. */
};
typedef struct RiffIterator_ RiffIterator;

/**
 * @brief Decode data raw data from WAV file into the allocated buffer for 
 * further processing.
 * @param s16_array Buffer for decoded audio data.
 * @param channels_num Pointer to channels number.
 * @param samples_data_size Pointer to aound size in bytes (number of 0xAA elements).
 * @param samples_per_second Pointer to number of samples per second play.
 * @param wav_mem_object Pointer to the memory object with raw WAV file data.
 */
void
WavDecoder_Decode(s16 *s16_array, u32 *channels_num, u32 *samples_data_size,
    u32 *samples_per_second, const MemObject* wav_mem_object);

#endif  /* JEMA_ENGINE_WAV_DECODER_H_ */
