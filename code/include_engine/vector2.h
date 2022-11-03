/**
 * ================================================================================
 * @file vector2.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 2D
 * column vectors with 2 coordinates of f32 (32 bits) floating point type. Set of 
 * functions were mostly copied from the box2d library (https://box2d.org/).
 * @version 0.1
 * @date 2022-10-29
 * ================================================================================ 
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_

/* Game engine includes: */
#include "utils.h"

/**
 * @brief Structure to store the 2D vector with 2 coordinates of f32 type.
 */
struct Vec2
{
    f32 x;  /**< Vector x coordinate. */
    f32 y;  /**< Vector y coordinate. */
};
typedef struct Vec2 Vec2_t;

/**
 * @brief Class constructor.
 * @return Vec2_t* Pointer to the Vec2 structure.
 */
Vec2_t*
vec2_constructor(void);

/**
 * @brief Class descructor.
 * @param vec2 Pointer to the Vec2 structure.
 */
void
vec2_destructor(Vec2_t* vec2);

/**
 * @brief Initialization of the vector by coordinate values.
 * @param vec2 Pointer to the Vec2 structure.
 * @param x Vector x coordinate.
 * @param y Vector y coordinate.
 */
void
vec2_init_by_f32(Vec2_t* vec2, f32 x, f32 y);

/**
 * @brief Initialization of the vector by data from the source vector.
 * @param vec2 Pointer to Vec2 structure (destination).
 * @param vec2_src Pointer to Vec2 structure (source).
 */
void
vec2_init_by_vec2(Vec2_t* vec2, const Vec2_t* vec2_src);

/**
 * @brief Determination if vector contain finite coordinates.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 True - valid vector. False - not valid vector.
 */
b32
vec2_is_valid(Vec2_t *vec2);

/**
 * @brief Determination if vector is a nearly zero vector.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 Result of the check.
 */
b32
vec2_is_nearly_zero(Vec2_t *vec2);

/**
 * @brief Set vector coordinates to zero.
 * @param vec2 Pointer to the Vec2 structure.
 */
void
vec2_set_zero(Vec2_t* vec2);

/**
 * @brief Negate the vector coordinates.
 * @param vec2 Pointer to the Vec2 structure.
 */
void
vec2_negate(Vec2_t* vec2);

/**
 * @brief Summation with another vector. 
 * @param vec2_a Pointer to the first vector (resulting vector).
 * @param vec2_b Pointer to the second vector.
 */
void
vec2_add_vec2(Vec2_t* vec2_a, const Vec2_t* vec2_b);

/**
 * @brief Substraction of another vector. 
 * @param vec2_a Pointer to the first vector (resulting vector).
 * @param vec2_b Pointer to the second vector.
 */
void
vec2_substract_vec2(Vec2_t* vec2_a, const Vec2_t* vec2_b);

/**
 * @brief Multiply the vector by a scalar.
 * @param vec2 Pointer to the Vec2 structure.
 * @param scalar Arbitrary scalar.
 */
void
vec2_multiply_scalar(Vec2_t* vec2, f32 scalar);

/**
 * @brief Get the vector squared length (for better performance).
 * @param vec2 Pointer to the Vec2 structure.
 * @return f32 Squared length of the vector.
 */
f32
vec2_get_length_squared(Vec2_t* vec2);

/**
 * @brief Get the vector length (the norm).
 * @param vec2 Pointer to the Vec2 structure.
 * @return f32 Length (the norm) of the vector. 
 */
f32
vec2_get_length(Vec2_t* vec2);

/**
 * @brief Convert the vector into a unit vector and returns its length.
 * @param vec2 Pointer to the Vec2 structure.
 * @return f32 length (the norm) of the vector. It will return 0.0f if the 
 * normalization was not successful.
 */
f32
vec2_normalize(Vec2_t* vec2);

#endif // VECTOR2_H_
