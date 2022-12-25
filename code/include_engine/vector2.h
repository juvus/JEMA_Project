/**
 * ================================================================================
 * @file include_engine/vector2.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 2D
 * column vectors with 2 coordinates of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-21
 * ================================================================================ 
 */

#ifndef JEMA_ENGINE_VECTOR2_H_
#define JEMA_ENGINE_VECTOR2_H_

#include "include_engine/utils.h"

/**
 * @brief Structure to store the 2D vector with 2 coordinates of f32 type.
 */
struct Vec2_
{
    union
    {
        struct
        {
            f32 x;  /**< Vector x coordinate. */
            f32 y;  /**< Vector y coordinate. */
        };
        f32 crd_arr[2];  /**< Array of the coordinates. */
    };
};
typedef struct Vec2_ Vec2;

/**
 * @brief Object constructor.
 * @return Vec2* Pointer to the Vec2 structure.
 */
Vec2*
Vec2_Constructor(void);

/**
 * @brief Class descructor.
 * @param vec2 Pointer to the Vec2 structure.
 * @return Vec2* Pointer to the Vec2 structure.
 */
Vec2*
Vec2_Destructor(Vec2 *vec2);

/**
 * @brief Initialization of the vector by coordinate values.
 * @param vec2 Pointer to the Vec2 structure.
 * @param x Vector x coordinate.
 * @param y Vector y coordinate.
 */
void
Vec2_InitByF32(Vec2 *vec2, f32 x, f32 y);

/**
 * @brief Initialization of the vector by data from the source vector.
 * @param vec2 Pointer to Vec2 structure (destination).
 * @param vec2_src Pointer to Vec2 structure (source).
 */
void
Vec2_InitByVec2(Vec2 *vec2, const Vec2 *vec2_src);

/**
 * @brief Determination if vector contain finite coordinates.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 True - valid vector. False - not valid vector.
 */
b32
Vec2_IsValid(Vec2 *vec2);

/**
 * @brief Determination if vector is a nearly zero vector.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 Result of the check.
 */
b32
Vec2_IsNearlyZero(Vec2 *vec2);

/**
 * @brief Set vector coordinates to zero.
 * @param vec2 Pointer to the Vec2 structure.
 */
void
Vec2_SetZero(Vec2 *vec2);

/**
 * @brief Negate the vector coordinates.
 * @param vec2 Pointer to the Vec2 structure.
 */
void
Vec2_Negate(Vec2 *vec2);

/**
 * @brief Summation with another vector. 
 * @param vec2 Pointer to the Vec2 structure (resulting vector).
 * @param vec2_add Pointer to the adding vector.
 */
void
Vec2_AddVec2(Vec2 *vec2, const Vec2 *vec2_add);

/**
 * @brief Substraction of another vector. 
 * @param vec2 Pointer to the Vec2 structure (resulting vector).
 * @param vec2_sub Pointer to the substracting vector.
 */
void
Vec2_SubstractVec2(Vec2 *vec2, const Vec2 *vec2_sub);

/**
 * @brief Multiply the vector by a scalar.
 * @param vec2 Pointer to the Vec2 structure.
 * @param scalar Arbitrary scalar.
 */
void
Vec2_MultiplyScalar(Vec2 *vec2, f32 scalar);

/**
 * @brief Get the vector squared length (for better performance).
 * @param vec2 Pointer to the Vec2 structure.
 * @return f32 Squared length of the vector.
 */
f32
Vec2_GetLengthSquared(Vec2 *vec2);

/**
 * @brief Get the vector length (the norm).
 * @param vec2 Pointer to the Vec2 structure.
 * @return f32 Length (the norm) of the vector. 
 */
f32
Vec2_GetLength(Vec2 *vec2);

/**
 * @brief Convert the vector into a unit vector and returns its length.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 Flag showing if the normalization was successful or not. 
 */
b32
Vec2_Normalize(Vec2 *vec2);

#endif  /* JEMA_ENGINE_VECTOR2_H_ */
