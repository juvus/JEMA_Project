/**
 * ================================================================================
 * @file include_engine/mouse.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with mouse
 * input to the game.
 * @version 0.2
 * @date 2022-12-04
 * ================================================================================ 
 */

#ifndef JEMA_ENGINE_MOUSE_H_
#define JEMA_ENGINE_MOUSE_H_

#include "include_engine/utils.h"

typedef struct Render_ Render;
typedef struct Vec2_ Vec2;

/**
 * @brief Structure for holding user mouse input.
 */
struct Mouse_
{
    u32 cur_x;  /**< X coordinate of the mouse cursor. */
    u32 cur_y;  /**< Y coordinate of the mouse cursor. */
    POINT raw_cursor;  /**< Structure with raw cursor point data. */
};
typedef struct Mouse_ Mouse;

/**
 * @brief Object constructor.
 * @return Mouse* Pointer to the Mouse structure.
 */
Mouse*
Mouse_Constructor(void);

/**
 * @brief Object destructor.
 * @param mouse Pointer to the Mouse structure.
 * @return Mouse* Pointer to the Mouse structure.
 */
Mouse*
Mouse_Destructor(Mouse *mouse);

/**
 * @brief Preparation cursor data for futher use in the game.
 * @param mouse Pointer to the Mouse structure.
 * @param render Poiner to the Render structure.
 */
void 
Mouse_PrepareInput(Mouse *mouse, Render *render);

#endif  /* JEMA_ENGINE_MOUSE_H_ */
