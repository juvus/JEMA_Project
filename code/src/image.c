/**
 * ================================================================================
 * @file image.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the Image class methods.
 * @version 0.2
 * @date 2022-01-05
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

/* Program includes: */
/* Additional deffinitions prior to stb_image.h load. */
#include <image.h>
#include <file_io.h>
#define STB_IMAGE_IMPLEMENTATION  /* Necessary definition for the stb_image.h. */
#define STBI_ONLY_PNG  /* Use only PNG images. */
#define STBI_NO_STDIO  /* Don't use stdio.h. */
#define STBI_NO_FAILURE_STRINGS  /* Generate no error messages. */
#include <stb_image.h>  /* single file library stb_image.h. */
#include <utils.h>

Image_t*
image_constructor(void)
{
    Image_t *image = NULL;

    /* Allocation the memory for the image object. */
    image = (Image_t*)malloc(1 * sizeof(Image_t));
    image->data = NULL;
    image->width = 0;
    image->height = 0;
    image->channels = 0;
    return image;
}

void
image_destructor(Image_t *image)
{
    free(image->data);  /* NOTE: Test this, because mem allocated in stbi module. */
    free(image);
}

void
image_uncompress_png(Image_t *image, File_t *file)
{
    char *file_data = file->data;  /* Data of the compressed png file. */
    int file_size = (int)file->size;  /* Size of the png file */
    int img_width;  /* Width of the uncomressed image. */
    int img_height;  /* Height of the uncompressed image. */
    int img_channels;  /* Number of channels of the uncompressed image. */

    /* Convert compressed png image to uncompressed */
    stbi_set_flip_vertically_on_load(1);  /* start from the BL */
    image->data = (u32*)stbi_load_from_memory((void*)file_data, file_size, 
        &img_width, &img_height, &img_channels, 4);
    image->width = img_width;
    image->height = img_height;
    image->channels = img_channels;
}
