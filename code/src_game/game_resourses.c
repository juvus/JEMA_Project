/**
 * ================================================================================
 * @file game_resourses.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with the game resourses.  
 * @version 0.1
 * @date 2022-07-22
 * ================================================================================
 */

/* Standard library includes. */
#include <stdlib.h>

/* Game engine includes. */
#include <game_resourses.h>
#include <file_io.h>
#include <image.h>
#include <sound.h>
#include <color.h>
#include <utils.h>

Game_Resourses_t*
gam_resourses_constructor(void)
{
    u32 i;  /* Temporary index. */
    Game_Resourses_t *gres;  /* Pointer to the GResourses structure. */

    /* Allocate memory for the object holding game resourses. */
    gres = (Game_Resourses_t *)malloc(1 * sizeof(Game_Resourses_t));

    /* Allocate memory for the game files. */
    for (i = 0; i < GF_FILES_NUM; ++i)
    {
        gres->files[i] = file_constructor();
    }

    /* Allocate memory for the game images. */
    for (i = 0; i < GI_IMAGES_NUM; ++i)
    {
        gres->images[i] = image_constructor();
    }

    /* Allocate memory for the game sounds. */
    for (i = 0; i < GS_SOUNDS_NUM; ++i)
    {
        gres->sounds[i] = sound_constructor();
    }

    /* Allocate memory for the game colors. */
    for (i = 0; i < GC_COLORS_NUM; ++i)
    {
        gres->colors[i] = color_constructor();
    }

    return gres;
}

void
gresourses_destructor(Game_Resourses_t *gres)
{
    /* Release memory allocated for the GResourses object. */
    free(gres);
    gres = NULL;
}