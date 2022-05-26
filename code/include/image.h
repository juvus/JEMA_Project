/**
 * ================================================================================
 * @file image.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Image class methods.
 * @version 0.2
 * @date 2022-01-05
 * @copyright JuvuSoft (c) 2021
 * ================================================================================
 */

#ifndef IMAGE_H_
#define IMAGE_H_

/* Program includes: */
#include <utils.h>

/* Predefined structures: */
typedef struct File File_t;

/**
 * @brief Structure for the uncompressed image holding in memory.
 */
struct Image 
{
    u32 *data;  /**< Pointer to the uncompressed image data (set of bytes). */
    u32 width;  /**< Width of the uncompresed image in pixels. */
    u32 height;  /**< Height of the uncompressed image in pixels. */
    u32 channels;  /**< Number of channels of the uncompressed image (4 - RGBA) */
};
typedef struct Image Image_t;

/**
 * @brief Structure for the color representation in RGBA format. 
 */
struct Color_bytes 
{
    union 
    {
        struct 
        {
            u8 r; u8 g; u8 b; u8 a;
        };
        u32 color_word;
    };
};
typedef struct Color_bytes Color_bytes_t;

/**
 * @brief Constructor of the Image class.
 * @return Image_t* Pointer to the Image structure.
 */
Image_t*
image_constructor(void);

/**
 * @brief Destructor of the Image class.
 * @param image Pointer to the Image structure.
 */
void
image_destructor(Image_t *image);

/**
 * @brief Uncompress png file into image data suitable for processing.
 * @param image Pointer to the Image structure.
 * @param file Pointer to the File structure of loaded png file.
 */
void
image_uncompress_png(Image_t *image, File_t *file);

#endif /* IMAGE_H_ */
