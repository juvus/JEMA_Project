/**
 * ================================================================================
 * @file src_engine/image.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with images.
 * @version 0.2
 * @date 2022-12-05
 * ================================================================================
 */

#include "include_engine/image.h"

#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#define STB_IMAGE_IMPLEMENTATION  /* Necessary definition for the stb_image.h. */
#define STBI_ONLY_PNG  /* Use only PNG images. */
#define STBI_NO_STDIO  /* Don't use stdio.h. */
#define STBI_NO_FAILURE_STRINGS  /* Generate no error messages. */
#include "include_engine/stb_image.h"
#include "include_engine/memory_object.h"
#include "include_engine/utils.h"

Image*
Image_Constructor(void)
{
    size_t size = sizeof(Image);
    Image *image = (Image *)HelperFcn_MemAllocate(size);
    return image;
}

Image*
Image_Destructor(Image *image)
{
    HelperFcn_MemFree(image->data);  /* NOTE: Test this, because mem allocated in stbi module. */
    HelperFcn_MemFree(image);
    return NULL;
}

void
Image_InitByMemObject(Image *image, MemObject *mem_object)
{
    char *mem_obj_data = (char *)mem_object->data;
    int mem_obj_size = (int)mem_object->size;

    /* Uncompress data from loaded png file to the image. */
    /* Coordinates origin in the BL corner. */
    stbi_set_flip_vertically_on_load(1);
    
    int img_width;  /* Width of the uncomressed image. */
    int img_height;  /* Height of the uncompressed image. */
    int img_channels;  /* Number of channels of the uncompressed image. */

    image->data = (u32 *)stbi_load_from_memory((void *)mem_obj_data, mem_obj_size, 
        &img_width, &img_height, &img_channels, 4);
    image->width = img_width;
    image->height = img_height;
    image->channels = img_channels;
}

void 
Image_LoadFromFile(Image *image, char *file_path)
{
    MemObject *mem_object = MemObject_Constructor();
    MemObject_InitByFile(mem_object, file_path);
    Image_InitByMemObject(image, mem_object);
    MemObject_Destructor(mem_object);
}
