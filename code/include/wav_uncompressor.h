/**
 * ================================================================================
 * @file wav_uncompressor.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Wav_uncompressor class methods.
 * file spec: .wav PCM 16-bits per sample, 1 or 2 channels (interleaved rlrlrlr)
 * @version 0.2
 * @date 2022-01-09
 * @copyright Copyright (c) 2022
 * ================================================================================ 
 */

#ifndef WAV_UNCOMPRESSOR_H_
#define WAV_UNCOMPRESSOR_H_

/* Program includes: */
#include <utils.h>

/* Predefined structures: */
typedef struct File File_t;
typedef struct Sound Sound_t;

/* Specification of the packing alignment for structure. */
#pragma pack(push, 1)

/**
 * @brief Structure for Wav_header. 
 */
struct Wav_header {  
    u32 riff_id;  /**< Id of the riff. */
    u32 size;  /**< Size. */
    u32 wave_id;  /**< Id of the wave. */
};
typedef struct Wav_header Wav_header_t;

/* Macros for the riff code. */
#define RIFF_CODE(a, b, c, d) \
    (((u32)(a) << 0) | ((u32)(b) << 8) | ((u32)(c) << 16) | ((u32)(d) << 24))

/**
 * @brief Enumerator for holding different ID's.
 */
enum {
    Wav_chunk_id_fmt = RIFF_CODE('f', 'm', 't', ' '),
    Wav_chunk_id_riff = RIFF_CODE('R', 'I', 'F', 'F'),
    Wav_chunk_id_wave = RIFF_CODE('W', 'A', 'V', 'E'),
    Wav_chunk_id_data = RIFF_CODE('d', 'a', 't', 'a'),
};

/**
 * @brief Structure for Wav_Chunk.
 */
struct Wav_chunk {
    u32 id;  /**< Id of the chunk. */
    u32 size;  /**< Chunk size. */
};
typedef struct Wav_chunk Wav_chunk_t;

/**
 * @brief Structure for Wav format. 
 */
struct Wav_format {
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
typedef struct Wav_format Wav_format_t;

/* Remove the record from the top of the internal compiler stack. */
#pragma pack(pop)

/**
 * @brief Structure for Riff Iterator.
 */
struct Riff_iterator {
    u8* at;  /**< Pointer to current position. */
    u8* stop;  /**< Pointer to stop position. */
};
typedef struct Riff_iterator Riff_iterator_t;

/**
 * @brief Uncompressing the data from wav file into sound data for processing.
 * @param sound Pointer to the Sound structure.
 * @param file Pointer to the File structure.
 */
void wav_uncompressor_uncompress(Sound_t *sound, File_t *file);

#endif //WAV_UNCOMPRESSOR_H_
