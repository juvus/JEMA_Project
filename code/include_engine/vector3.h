/**
 * ================================================================================
 * @file vector3.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 2D
 * column vectors with 3 coordinates of f32 (32 bits) floating point type.
 * @version 0.1
 * @date 2022-10-29
 * ================================================================================ 
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

/* Game engine includes: */
#include "utils.h"

/**
 * @brief Structure to store the 2D vector with 3 coordinates of f32 type.
 */
struct Vec3
{
    f32 x;  /**< Vector x coordinate. */
    f32 y;  /**< Vector y coordinate. */
    f32 z;  /**< Vector z coordinate. */
};
typedef struct Vec3 Vec3_t;

/**
 * @brief Class constructor.
 * @return Vect3_t* Pointer to the Vec3 structure.
 */
Vec3_t*
vec3_constructor(void);

/**
 * @brief Class descructor.
 * @param vec2 Pointer to the Vec3 structure.
 */
void
vec3_destructor(Vec3_t* vec3);

/**
 * @brief Initialization of the vector by coordinate values.
 * @param vec3 Pointer to the Vec3 structure.
 * @param x Vector x coordinate.
 * @param y Vector y coordinate.
 * @param z Vector z coordinate.
 */
void
vec3_init_by_f32(Vec3_t* vec3, f32 x, f32 y, f32 z);

/**
 * @brief Initialization of the vector by data from the source vector.
 * @param vec3 Pointer to Vec3 structure (destination).
 * @param vec3_src Pointer to Vec3 structure (source).
 */
void
vec3_init_by_vec3(Vec3_t* vec3, const Vec3_t* vec3_src);

/**
 * @brief Determination if a vector contain finite coordinates.
 * @param vec3 Pointer to the Vec3 structure.
 * @return b32 True - valid vector. False - not valid vector.
 */
b32
vec3_is_valid(Vec3_t *vec3);

/**
 * @brief Determination if vector is a nearly zero vector.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 Result of the check.
 */
b32
vec3_is_nearly_zero(Vec3_t *vec3);

/**
 * @brief Set vector coordinates to zero.
 * @param vec3 Pointer to the Vec3 structure.
 */
void
vec3_set_zero(Vec3_t* vec3);

/**
 * @brief Negate the vector coordinates.
 * @param vec3 Pointer to the Vec3 structure.
 */
void
vec3_negate(Vec3_t* vec3);

/**
 * @brief Summation with another vector. 
 * @param vec3_a Pointer to the first vector (resulting vector).
 * @param vec3_b Pointer to the second vector.
 */
void
vec3_add_vec3(Vec3_t* vec3_a, const Vec3_t* vec3_b);

/**
 * @brief Substraction of another vector. 
 * @param vec3_a Pointer to the first vector (resulting vector).
 * @param vec3_b Pointer to the second vector.
 */
void
vec3_substract_vec3(Vec3_t* vec3_a, const Vec3_t* vec3_b);

/**
 * @brief Multiply the vector by a scalar.
 * @param vec3 Pointer to the Vec3 structure.
 * @param scalar Arbitrary scalar.
 */
void
vec3_multiply_scalar(Vec3_t* vec3, f32 scalar);

/**
 * @brief Get the vector squared length (for better performance).
 * @param vec3 Pointer to the Vec3 structure.
 * @return f32 Squared length of the vector.
 */
f32
vec3_get_length_squared(Vec3_t* vec3);

/**
 * @brief Get the vector length (the norm).
 * @param vec3 Pointer to the Vec3 structure.
 * @return f32 Length (the norm) of the vector. 
 */
f32
vec3_get_length(Vec3_t* vec3);

/**
 * @brief Convert the vector into a unit vector and returns its length.
 * @param vec3 Pointer to the Vec3 structure.
 * @return b32 Flag showing if the normalization was successful or not.
 */
b32
vec3_normalize(Vec3_t* vec3);

#endif // VECTOR2_H_
