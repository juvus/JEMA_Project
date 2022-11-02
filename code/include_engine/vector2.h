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
    union
    {
        struct
        {
            f32 x;  /**< Vector x coordinate. */
            f32 y;  /**< Vector y coordinate. */
        };
        f32 crd_arr[2];  /**< Vector coordinate from from an indexed element. */
    };
};
typedef struct Vec2 Vec2_t;

/**
 * @brief Class constructor.
 * @return Vect2_t* Pointer to the Vec2 structure.
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
 * @brief Determination if a vector contain finite coordinates.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 True - valid vector. False - not valid vector.
 */
b32
vec2_is_valid(Vec2_t *vec2);

/**
 * @brief Set vector coordinates to some specified values.
 * @param vec2 Pointer to the Vec2 structure.
 * @param x Vector x coordinate.
 * @param y Vector y coordinate.
 */
void
vec2_set_coords(Vec2_t* vec2, f32 x, f32 y);

/**
 * @brief Set vector coordinates to zero.
 * @param vec2 Pointer to the Vec2 structure.
 */
void
vec2_set_zero(Vec2_t* vec2);

/**
 * @brief Copy the data from source vecto to the destination vector.
 * @param vec2_dest Pointer to destination vector.
 * @param vec2_src Pointer to source vector.
 */
void
vec2_set_copy(Vec2_t* vec2_dest, const Vec2_t* vec2_src);

/**
 * @brief Get copy of the vector.
 * @param vec2 Pointer to the Vec2 structure.
 * @return Vec2_t Returning Vec2 structure with copied data.
 */
Vec2_t
vec2_get_copy(const Vec2_t* vec2);

/**
 * @brief Negate the vector coordinates.
 * @param vec2 Pointer to the Vec2 structure.
 */
void
vec2_negate(Vec2_t* vec2);

/**
 * @brief Get vector with nehated coordinates.
 * @param vec2 Pointer to the Vec2 structure.
 * @return Vec2_t Returning Vec2 structure with negated coordinates.
 */
Vec2_t
vec2_get_negative(const Vec2_t* vec2);

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
