/**
 * ================================================================================
 * @file include_engine/math_functions.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for performing 
 * different calculations.
 * @version 0.1
 * @date 2022-12-04
 * ================================================================================ 
 */

#ifndef JEMA_ENGINE_MATH_FUNCTIONS_H_
#define JEMA_ENGINE_MATH_FUNCTIONS_H_

#include "include_engine/utils.h"

typedef struct Vec2_ Vec2;
typedef struct Vec3_ Vec3;

/* Macros for minimum and maximum. */
#define Math_Min(a, b) ((a) < (b) ? (a) : (b))
#define Math_Max(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief Determination if two f32 numbers are nearly equal.
 * @param x First f32 number.
 * @param y Second f32 number.
 * @return b32 Result of the comparison. 
 */
b32
Math_IsNearlyEqual(f32 x, f32 y);

/**
 * @brief Trim the f32 value to the low and hight bound.
 * @param value f32 value to trim.
 * @param low_bnd Low boundary of the value.
 * @param high_bnd High boundary of the value.
 * @return f32 Returning trimmed value.
 */
f32 
Math_TrimF32(f32 value, f32 low_bnd, f32 high_bnd);

/**
 * @brief Swapping coordinates of two Vec2 structures.
 * @param vec2_a Pointer to the first Vec2 structure.
 * @param vec2_b Pointer to the second Vec2 structure.
 */
void 
Math_SwapVec2(Vec2 *vec2_a, Vec2 *vec2_b);

/**
 * @brief Re-maps a number from one range to another. 
 * @param value Incoming value that need to be converted.
 * @param lower_bnd_1 Lower bound of the value's current range. 
 * @param upper_bnd_1 Upper bound of the value's current range.
 * @param lower_bnd_2 Lower bound of the value's target range. 
 * @param upper_bnd_2 Upper bound of the value's target range. 
 * @return f32 Return converted value.
 */
f32
Math_Map(f32 value, f32 lower_bnd_1, f32 upper_bnd_1, f32 lower_bnd_2, f32 upper_bnd_2);





/* TODO: Add swapping for vec3 and matrices */

/* VEctors summation */
/* dot product */

/*
  b2Vec2 Skew() const
     {
         return b2Vec2(-y, x);
     }
*/

/*
vector solve_equations_system(matrix, vector)
*/

#endif  /* JEMA_ENGINE_MATH_FUNCTIONS_H_ */
