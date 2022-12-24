/**
 * ================================================================================
 * @file include_engine/image.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with images.
 * @version 0.2
 * @date 2022-12-05
 * ================================================================================
 */

#ifndef JEMA_ENGINE_IMAGE_H_
#define JEMA_ENGINE_IMAGE_H_

#include "include_engine/utils.h"

typedef struct MemObject_ MemObject;

/**
 * @brief Structure for the uncompressed image holding in memory.
 */
struct Image_ 
{
    u32 *data;  /**< Pointer to the uncompressed image data (set of bytes). */
    u32 width;  /**< Width of the uncompresed image in pixels. */
    u32 height;  /**< Height of the uncompressed image in pixels. */
    u32 channels;  /**< Number of channels of the uncompressed image (4 - RGBA) */
};
typedef struct Image_ Image;

/**
 * @brief Object constructor.
 * @return Image* Pointer to the Image structure.
 */
Image*
Image_Constructor(void);

/**
 * @brief Object destructor.
 * @param image Pointer to the Image structure.
 */
void
Image_Destructor(Image *image);

/**
 * @brief Initialization of the image by the loaded raw image data.
 * @param image Pointer to the Image structure.
 * @param mem_object Pointer to the memory object with raw png file data.
 */
void
Image_Init(Image *image, MemObject *mem_object);

#endif /* JEMA_ENGINE_IMAGE_H_ */
