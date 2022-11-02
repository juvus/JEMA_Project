/**
 * ================================================================================
 * @file vector3.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 2D
 * column vectors with 3 coordinates of f32 (32 bits) floating point type. Set of 
 * functions were mostly copied from the box2d library (https://box2d.org/).
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
    union
    {
        struct
        {
            f32 x;  /**< Vector x coordinate. */
            f32 y;  /**< Vector y coordinate. */
            f32 z;  /**< Vector z coordinate. */
        };
        f32 crd_arr[3];  /**< Vector coordinate from from an indexed element. */
    };
};
typedef struct Vec3 Vec3_t;

/**
 * @brief Class constructor.
 * @return Vect3_t* Pointer to the Vec3 structure.
 */
Vec3_t*
vec2_constructor(void);

/**
 * @brief Class descructor.
 * @param vec2 Pointer to the Vec3 structure.
 */
void
vec3_destructor(Vec3_t* vec3);

/**
 * @brief Determination if a vector contain finite coordinates.
 * @param vec3 Pointer to the Vec3 structure.
 * @return b32 True - valid vector. False - not valid vector.
 */
b32
vec3_is_valid(Vec3_t *vec3);

/**
 * @brief Set vector coordinates to some specified values.
 * @param vec3 Pointer to the Vec3 structure.
 * @param x Vector x coordinate.
 * @param y Vector y coordinate.
 * @param z Vector z coordinate.
 */
void
vec3_set_coords(Vec3_t* vec3, f32 x, f32 y, f32 z);

/**
 * @brief Set vector coordinates to zero.
 * @param vec3 Pointer to the Vec3 structure.
 */
void
vec3_set_zero(Vec3_t* vec3);

/**
 * @brief Copy the data from source vecto to the destination vector.
 * @param vec3_dest Pointer to destination vector.
 * @param vec3_src Pointer to source vector.
 */
void
vec3_set_copy(Vec3_t* vec3_dest, const Vec3_t* vec3_src);

/**
 * @brief Get copy of the vector.
 * @param vec3 Pointer to the Vec3 structure.
 * @return Vec3_t Returning Vec3 structure with copied data.
 */
Vec3_t
vec3_get_copy(const Vec3_t* vec3);

/**
 * @brief Negate the vector coordinates.
 * @param vec3 Pointer to the Vec3 structure.
 */
void
vec3_negate(Vec3_t* vec3);

/**
 * @brief Get vector with nehated coordinates.
 * @param vec3 Pointer to the Vec3 structure.
 * @return Vec3_t Returning Vec3 structure with negated coordinates.
 */
Vec3_t
vec3_get_negative(const Vec3_t* vec3);

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
 * @return f32 length (the norm) of the vector. It will return 0.0f if the 
 * normalization was not successful.
 */
f32
vec3_normalize(Vec3_t* vec3);

#endif // VECTOR2_H_