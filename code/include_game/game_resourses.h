/**
 * ================================================================================
 * @file game_resourses.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of different resourses used by the game.  
 * @version 0.1
 * @date 2022-07-22
 * ================================================================================
 */

#ifndef GAME_RESOURSES_H_
#define GAME_RESOURSES_H_

/* Game engine includes. */
#include <utils.h>

/* Predefined structures. */
typedef struct File File_t;
typedef struct Image Image_t;
typedef struct Sound Sound_t;
typedef struct Color Color_t;

/**
 * @brief Enumerator for the set of files used in the game.
 */
enum Game_Resourse_File
{
    GF_FONT_PNG,  /**< PNG file with font symbol data. */
    GF_SMILE_FACE_PNG,  /**< PNG file with smiley face image. */
    GF_BACKGROUND_WAV,  /**< WAV file for the background music. */
    GF_FILES_NUM  /**< Total amount of the files used in the game. */
};
typedef enum Game_Resourse_File Game_Resourse_File_t;

/**
 * @brief Enumerator for the set of images used in the game.
 */
enum Game_Resourse_Image
{
    GI_FONT,  /**< Image with font symbols. */
    GI_SMILE_FACE,  /**< Temporary test image with smiley face. */
    GI_IMAGES_NUM  /**< Total amount of the images used in the game.  */
};
typedef enum Game_Resourse_Image Game_Resourse_Image_t;

/**
 * @brief Enumerator for the set of sounds used in the game.
 */
enum Game_Resourse_Sound
{
    GS_EMPTY,  /**< Empty sound necessary for the mixing purposes. */
    GS_BACKGROUND,  /**< Sound for the background music. */
    GS_SOUNDS_NUM  /**< Total amount of the sounds used in the game. */
};
typedef enum Game_Resourse_Sound Game_Resourse_Sound_t;

/**
 * @brief Enumerator for the game colors used in the game.
 */
enum Game_Resourse_Color
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
typedef enum Game_Resourse_Color Game_Resourse_Color_t;

/**
 * @brief Structure for the game resourses.
 */
struct Game_Resourses
{
    File_t *files[GF_FILES_NUM];  /* Array of pointers to the File structures. */
    Image_t *images[GI_IMAGES_NUM];  /* Array of pointers to the Image structures.  */
    Sound_t *sounds[GS_SOUNDS_NUM];  /* Array of pointers to the Sound structure. */
    Color_t *colors[GC_COLORS_NUM];  /* Array of pointers to the Color structure. */ 
};
typedef struct Game_Resourses Game_Resourses_t; 

/**
 * @brief Constructor of the Game_Resourses class.
 * @return Game_Resourses_t* Pointer to the Game_Resourses structure. 
 */
Game_Resourses_t*
game_resourses_constructor(void);

/**
 * @brief Destructor of the Game_Resourses object.
 * @param game_resourses Pointer to the Game_Resourses structure.
 */
void
game_resourses_destructor(Game_Resourses_t *game_resourses);

#endif //GAME_RESOURSES_H_