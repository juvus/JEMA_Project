/**
 * ================================================================================
 * @file image.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with images.
 * @version 0.2
 * @date 2022-07-10
 * ================================================================================
 */

/* Game engine includes: */
#include <image.h>
#include <file_io.h>
#include <utils.h>
#define STB_IMAGE_IMPLEMENTATION  /* Necessary definition for the stb_image.h. */
#define STBI_ONLY_PNG  /* Use only PNG images. */
#define STBI_NO_STDIO  /* Don't use stdio.h. */
#define STBI_NO_FAILURE_STRINGS  /* Generate no error messages. */
#include <stb_image.h>  /* single file library stb_image.h. */

Image_t*
image_constructor(void)
{
    Image_t *image;  /* Pointer to the Image structure. */

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
    /* Free memory allocated for the image data. */
    free(image->data);  /* NOTE: Test this, because mem allocated in stbi module. */
    image->data = NULL;
    
    /* Free memory allocated for the Image structure. */
    free(image);
    image = NULL;
}

void
image_init(Image_t *image, File_t *file)
{
    char *file_data;  /* Data of the compressed png file. */
    int file_size;  /* Size of the png file. */
    int img_width;  /* Width of the uncomressed image. */
    int img_height;  /* Height of the uncompressed image. */
    int img_channels;  /* Number of channels of the uncompressed image. */

    file_data = (char *)file->data;
    file_size = (int)file->size;

    /* Uncompress data from loaded png file to the image. */
    /* Coordinates origin in the BL corner. */
    stbi_set_flip_vertically_on_load(1);
    
    image->data = (u32*)stbi_load_from_memory((void*)file_data, file_size, 
        &img_width, &img_height, &img_channels, 4);
    image->width = img_width;
    image->height = img_height;
    image->channels = img_channels;
}
