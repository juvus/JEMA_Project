/**
 * ================================================================================
 * @file src_game/game_resourses.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with the game resourses.  
 * @version 0.1
 * @date 2022-12-23
 * ================================================================================
 */

#include "include_game/game_resourses.h"

#include <stdlib.h>

#include "include_engine/color.h"
#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/image.h"
#include "include_engine/sound.h"
#include "include_engine/utils.h"

GameResourses*
GameResourses_Constructor(void)
{
    size_t size = sizeof(GameResourses);
    GameResourses *game_resourses = (GameResourses *)HelperFcn_MemAllocate(size);

    /* Allocate memory for the game images. */
    for (u32 i = 0; i < GI_IMAGES_NUM; ++i)
    {
        game_resourses->images[i] = Image_Constructor();
    }

    /* Allocate memory for the game sounds. */
    for (u32 i = 0; i < GS_SOUNDS_NUM; ++i)
    {
        game_resourses->sounds[i] = Sound_Constructor();
    }

    /* Allocate memory for the game colors. */
    for (u32 i = 0; i < GC_COLORS_NUM; ++i)
    {
        game_resourses->colors[i] = Color_Constructor();
    }
    return game_resourses;
}

GameResourses*
GameResourses_Destructor(GameResourses *game_resourses)
{
    HelperFcn_MemFree(game_resourses);
    return NULL;
}

void
GameResources_FreeResources(GameResourses *game_resourses)
{
    /* Free memory allocated for the game images. */
    for (u32 i = 0; i < GI_IMAGES_NUM; ++i)
    {
        if (game_resourses->images[i] != NULL)
        {
            game_resourses->images[i] = 
                Image_Destructor(game_resourses->images[i]);
        }
    }

    /* Free memory allocated for the game sounds. */
    for (u32 i = 0; i < GS_SOUNDS_NUM; ++i)
    {
        if (game_resourses->sounds[i] != NULL)
        {
            game_resourses->sounds[i] = 
                Sound_Destructor(game_resourses->sounds[i]);
        }
    }

    /* Free memory allocated for the game colors. */
    for (u32 i = 0; i < GC_COLORS_NUM; ++i)
    {
        if (game_resourses->colors[i] != NULL)
        {
            game_resourses->colors[i] = 
                Color_Destructor(game_resourses->colors[i]);
        }
    }
}
