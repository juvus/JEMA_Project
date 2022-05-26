/**
 * ================================================================================
 * @file mouse_input.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Mouse_Input class methods.
 * @version 0.2
 * @date 2022-01-10
 * @copyright JuvuSoft (c) 2022
 * ================================================================================ 
 */

#ifndef MOUSE_INPUT_H_
#define MOUSE_INPUT_H_

/* Program includes: */
#include <utils.h>

/* Predefined structure */
typedef struct Game Game_t;

/**
 * @brief Structure for holding user mouse input.
 */
struct Mouse_Input
{
    V2_u32_t cursor;  /**< Vector holding current position of a cursor. */
    POINT raw_cursor;  /**< Structure with raw cursor point data. */
};
typedef struct Mouse_Input Mouse_Input_t;

/**
 * @brief Preparation cursor data for futher use in the game.
 * @param mouse_input Pointer to the Mouse_Input structure data.
 * @param game Poiner to the Game structure data.
 */
void 
mouse_input_prepare_input(Mouse_Input_t *mouse_input, Game_t *game);

#endif /* MOUSE_INPUT_H_ */
