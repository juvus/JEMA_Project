/**
 * ================================================================================
 * @file mouse.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with mouse
 * input to the game.
 * @version 0.2
 * @date 2022-07-11
 * ================================================================================ 
 */

#ifndef MOUSE_H_
#define MOUSE_H_

/* Game engine includes: */
#include "utils.h"

/* Predefined structures: */
typedef struct Render Render_t;

/**
 * @brief Structure for holding user mouse input.
 */
struct Mouse
{
    V2_u32_t cursor;  /**< Vector holding current position of a cursor. */
    POINT raw_cursor;  /**< Structure with raw cursor point data. */
};
typedef struct Mouse Mouse_t;

/**
 * @brief Constructor of the Mouse class.
 * @return Mouse_t* Pointer to the Mouse structure.
 */
Mouse_t*
mouse_constructor(void);

/**
 * @brief Destructor of the Mouse class.
 * @param mouse Pointer to the Mouse structure.
 */
void
mouse_destructor(Mouse_t *mouse);

/**
 * @brief Preparation cursor data for futher use in the game.
 * @param mouse Pointer to the Mouse structure.
 * @param render Poiner to the Render structure.
 */
void 
mouse_prepare_input(Mouse_t *mouse, Render_t *render);

#endif /* MOUSE_H_ */
