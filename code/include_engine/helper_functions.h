/**
 * ================================================================================
 * @file include_engine/helper_functions.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the different helper functions.
 * @version 0.2
 * @date 2022-12-05
 * ================================================================================
 */

#ifndef JEMA_ENGINE_HELPER_FUNCTIONS_H_
#define JEMA_ENGINE_HELPER_FUNCTIONS_H_

#include "include_engine/utils.h"

typedef struct Vec2_ Vec2;

/**
 * @brief Universal function to check the fact of the collision of obj_mv (moving)
 * with obj_st (static) and determining the collision side (r, b, l, t of the 
 * static object).
 * @param bl_mv BL point coordinates of the moving object.
 * @param ur_mv UL point coordinates of the moving object.
 * @param bl_st BL point coordinates of the static object.
 * @param ur_st UL point coordinates of the static object.
 * @param collision_side Determined side of the collision.
 * @return b32 Flag showing the fact of the collision.
 */
b32 
HelperFcn_CheckCollision(Vec2 *bl_mv, Vec2 *ur_mv, Vec2 *bl_st, Vec2 *ur_st,
   char *collision_side);

#endif  /* JEMA_ENGINE_HELPER_FUNCTIONS_H_ */
