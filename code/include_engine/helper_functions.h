/**
 * ================================================================================
 * @file helper_functions.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the different helper functions.
 * @version 0.2
 * @date 2022-01-10
 * ================================================================================
 */

#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

/* Game engine includes. */
#include "utils.h"

/**
 * @brief Trim the f32 value to the low and hight bound.
 * @param value f32 value to trim.
 * @param low_bnd Low boundary of the value.
 * @param high_bnd High boundary of the value.
 * @return f32 Returning trimmed value.
 */
f32 
trim_f32(f32 value, f32 low_bnd, f32 high_bnd);

/**
 * @brief Check the fact of objects collision.
 * Universal function to check the fact of the collision of obj_mv (moving) with
 * obj_st (static) and determining the collision side (r, b, l, t of the static object).
 * @param BL_mv BL point coordinates of the moving object.
 * @param UR_mv UL point coordinates of the moving object.
 * @param BL_st BL point coordinates of the static object.
 * @param UR_st UL point coordinates of the static object.
 * @param collision_side Determined side of the collision.
 * @return b32 Flag of the collision.
 */
b32 
check_collision(V2_f32_t *BL_mv, V2_f32_t *UR_mv, V2_f32_t *BL_st, V2_f32_t *UR_st,
   char *collision_side);

/**
 * @brief Swapping coordinates of two V2_u32_t vertices.
 * @param var_1 Vertix 1.
 * @param var_2 Vertix 2.
 */
void 
swap_V2_u32(V2_u32_t *vertix_1, V2_u32_t *vertix_2);

#endif //HELPER_FUNCTIONS_H_
