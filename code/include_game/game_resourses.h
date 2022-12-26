/**
 * ================================================================================
 * @file include_game/game_resourses.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of different resourses used by the game.  
 * @version 0.1
 * @date 2022-12-23
 * ================================================================================
 */

#ifndef JEMA_GAME_RESOURSES_H_
#define JEMA_GAME_RESOURSES_H_

#include "include_engine/utils.h"

typedef struct Color_ Color;
typedef struct Image_ Image;
typedef struct Sound_ Sound;

/**
 * @brief Enumerator for the set of images used in the game.
 */
enum GameResourseImage_
{
    GI_FONT,  /**< Image with font symbols. */
    GI_SMILE_FACE,  /**< Temporary test image with smiley face. */
    GI_IMAGES_NUM  /**< Total amount of the images used in the game.  */
};
typedef enum GameResourseImage_ GameResourseImage;

/**
 * @brief Enumerator for the set of sounds used in the game.
 */
enum GameResourseSound_
{
    GS_EMPTY,  /**< Empty sound necessary for the mixing purposes. */
    GS_BACKGROUND,  /**< Sound for the background music. */
    GS_SOUNDS_NUM  /**< Total amount of the sounds used in the game. */
};
typedef enum GameResourseSound_ GameResourseSound;

/**
 * @brief Enumerator for the game colors used in the game.
 */
enum GameResourseColor_
{
    GC_WHITE,  /**< White color (0x00ffffff). */
    GC_BLACK,  /**< Black color (0x00000000). */
    GC_RED,  /**< Red color (0x00ff0000). */
    GC_GREEN,  /**< Green color (0x0000ff00). */
    GC_BLUE,  /**< Blue color (0x000000ff). */
    GC_DCONSOLE_BKG,  /**< Debug console background color (0x00ffffff). */
    GC_DCONSOLE_BRD,  /**< Debug console border color (0x00b4b4b4). */
    GC_BKG_COLOR,  /**< Game background color. */
    GC_COLORS_NUM  /**< Total number of the colors used in the game. */
};
typedef enum GameResourseColor_ GameResourseColor;

/**
 * @brief Structure for the game resourses.
 */
struct GameResourses_
{
    Image *images[GI_IMAGES_NUM];  /* Array of pointers to the Image structures.  */
    Sound *sounds[GS_SOUNDS_NUM];  /* Array of pointers to the Sound structure. */
    Color *colors[GC_COLORS_NUM];  /* Array of pointers to the Color structure. */ 
};
typedef struct GameResourses_ GameResourses; 

/**
 * @brief Object constructor.
 * @return GameResourses* Pointer to the GameResourses structure.
 */
GameResourses*
GameResourses_Constructor(void);

/**
 * @brief Object destructor.
 * @param game_resourses Pointer to the GameResourses structure.
 * @return GameResourses* Pointer to the GameResourses structure.
 */
GameResourses*
GameResourses_Destructor(GameResourses *game_resourses);

/**
 * @brief Free all game resourses in one place.
 * @param game_resourses Pointer to the GameResourses structure.
 */
void
GameResources_FreeResources(GameResourses *game_resourses);

#endif  /* JEMA_GAME_RESOURSES_H_ */
