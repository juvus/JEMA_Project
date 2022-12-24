/**
 * ================================================================================
 * @file include_engine/vector3.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 2D
 * column vectors with 3 coordinates of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-21
 * ================================================================================ 
 */

#ifndef JGE_ENGINE_VECTOR3_H_
#define JGE_ENGINE_VECTOR3_H_

#include "include_engine/utils.h"

/**
 * @brief Structure to store the 2D vector with 3 coordinates of f32 type.
 */
struct Vec3_
{
    union
    {
        struct
        {
            f32 x;  /**< Vector x coordinate. */
            f32 y;  /**< Vector y coordinate. */
            f32 z;  /**< Vector z coordinate. */
        };
        f32 crd_arr[3];  /**< Array of the coordinates. */
    };
};
typedef struct Vec3_ Vec3;

/**
 * @brief Object constructor.
 * @return Vec3* Pointer to the Vec3 structure.
 */
Vec3*
Vec3_Constructor(void);

/**
 * @brief Object descructor.
 * @param vec3 Pointer to the Vec3 structure.
 */
void
Vec3_Destructor(Vec3 *vec3);

/**
 * @brief Initialization of the vector by coordinate values.
 * @param vec3 Pointer to the Vec3 structure.
 * @param x Vector x coordinate.
 * @param y Vector y coordinate.
 * @param z Vector z coordinate.
 */
void
Vec3_InitByF32(Vec3 *vec3, f32 x, f32 y, f32 z);

/**
 * @brief Initialization of the vector by data from the source vector.
 * @param vec3 Pointer to Vec3 structure (destination).
 * @param vec3_src Pointer to Vec3 structure (source).
 */
void
Vec3_InitByVec3(Vec3 *vec3, const Vec3 *vec3_src);

/**
 * @brief Determination if a vector contain finite coordinates.
 * @param vec3 Pointer to the Vec3 structure.
 * @return b32 True - valid vector. False - not valid vector.
 */
b32
Vec3_IsValid(Vec3 *vec3);

/**
 * @brief Determination if vector is a nearly zero vector.
 * @param vec2 Pointer to the Vec2 structure.
 * @return b32 Result of the check.
 */
b32
Vec3_IsNearlyZero(Vec3 *vec3);

/**
 * @brief Set vector coordinates to zero.
 * @param vec3 Pointer to the Vec3 structure.
 */
void
Vec3_SetZero(Vec3 *vec3);

/**
 * @brief Negate the vector coordinates.
 * @param vec3 Pointer to the Vec3 structure.
 */
void
Vec3_Negate(Vec3 *vec3);

/**
 * @brief Summation with another vector. 
 * @param vec3 Pointer to the Vec3 structure (resulting vector).
 * @param vec3_add Pointer to the adding vector.
 */
void
Vec3_AddVec3(Vec3 *vec3, const Vec3 *vec3_add);

/**
 * @brief Substraction of another vector. 
 * @param vec3 Pointer to the Vec3 structure (resulting vector).
 * @param vec3_sub Pointer to the substracting vector.
 */
void
Vec3_SubstractVec3(Vec3 *vec3, const Vec3 *vec3_sub);

/**
 * @brief Multiply the vector by a scalar.
 * @param vec3 Pointer to the Vec3 structure.
 * @param scalar Arbitrary scalar.
 */
void
Vec3_MultiplyScalar(Vec3 *vec3, f32 scalar);

/**
 * @brief Get the vector squared length (for better performance).
 * @param vec3 Pointer to the Vec3 structure.
 * @return f32 Squared length of the vector.
 */
f32
Vec3_GetLengthSquared(Vec3 *vec3);

/**
 * @brief Get the vector length (the norm).
 * @param vec3 Pointer to the Vec3 structure.
 * @return f32 Length (the norm) of the vector. 
 */
f32
Vec3_GetLength(Vec3 *vec3);

/**
 * @brief Convert the vector into a unit vector and returns its length.
 * @param vec3 Pointer to the Vec3 structure.
 * @return b32 Flag showing if the normalization was successful or not.
 */
b32
Vec3_Normalize(Vec3 *vec3);

#endif  /* JGE_ENGINE_VECTOR2_H_ */
