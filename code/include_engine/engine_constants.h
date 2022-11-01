/**
 * ================================================================================
 * @file engine_constants.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of a set of different constants related to the game engine. 
 * @version 0.1
 * @date 2021-07-10
 * ================================================================================
 */

#ifndef GAME_ENGINE_CONSTANTS_H_
#define GAME_ENGINE_CONSTANTS_H_

/* Game engine includes. */
#include "utils.h"

/* Font constants. */
const u32 FONT_ROWS_NUM = 16;  /* Amount of symbol rows in font image. */
const u32 FONT_COLS_NUM = 16;  /* Amount of sumbol columns in font image. */
const u32 FONT_SYM_WIDTH = 6;  /* Symbol width in pixels. */
const u32 FONT_SYM_HEIGHT = 7;  /* Symbol height in pixels. */

/* Debug console constants. */
const u32 DCONSOLE_MESSAGES = 10;  /* Amount of the debuf messages in debug console. */
const u32 DCONSOLE_BKG_COLOR = 0xffffffff;  /* Background color of the debug console. */
const u32 DCONSOLE_BRD_COLOR = 0xffb4b4b4;  /* Console boarders color. */
const u32 DCONSOLE_MARGINS = 10;  /* Top, left, bottom and right margins. */
const u32 DCONSOLE_MAX_MSG_LENGTH = 200;  /* Maximum message length. */

#endif //GANE_ENGINE_CONSTANTS_H_

