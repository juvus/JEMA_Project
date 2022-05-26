/**
 * ================================================================================
 * @file mouse_input.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the Mouse_Input class methods.
 * @version 0.2
 * @date 2022-01-10
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

/* Program includes: */
#include <mouse_input.h>
#include <game.h>
#include <utils.h>

/**
 * @brief Preparation cursor data for futher use in the game.
 * @param mouse_input Pointer to the Mouse_Input structure data.
 * @param game Poiner to the Game structure data.
 */
void
mouse_input_prepare_input(Mouse_Input_t *mouse_input, Game_t *game)
{
    mouse_input->cursor.x = mouse_input->raw_cursor.x;
    mouse_input->cursor.y = game->height - mouse_input->raw_cursor.y;
}