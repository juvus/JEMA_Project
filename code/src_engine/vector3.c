/**
 * ================================================================================
 * @file src_engine/vector3.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 2D column vectors 
 * with 3 coordinates of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-22
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/vector3.h"

#include <math.h>

#include "include_engine/dbg.h"
#include "include_engine/math_functions.h"
#include "include_engine/utils.h"

Vec3*
Vec3_Constructor(void)
{
    Vec3 *vec3 = (Vec3 *)malloc(1 * sizeof(Vec3));
    if (vec3 == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return vec3;
}

void
Vec3_Destructor(Vec3 *vec3)
{
    if (vec3 == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    free(vec3);
    vec3 = NULL;
}

void
Vec3_InitByF32(Vec3 *vec3, f32 x, f32 y, f32 z)
{
    vec3->x = x;
    vec3->y = y;
    vec3->z = z;
}

void
Vec3_InitByVec3(Vec3 *vec3, const Vec3 *vec3_src)
{
    *vec3 = *vec3_src;
}

b32
Vec3_IsValid(Vec3 *vec3)
{
    b32 result = isfinite(vec3->x) && isfinite(vec3->y) && isfinite(vec3->z); 
    return result;
}

b32
Vec3_IsNearlyZero(Vec3 *vec3)
{
    b32 result = Math_IsNearlyEqual(vec3->x, 0.0f) && Math_IsNearlyEqual(vec3->y, 0.0f) &&
        Math_IsNearlyEqual(vec3->z, 0.0f); 
    return result;
}

void
Vec3_SetZero(Vec3 *vec3)
{
    vec3->x = 0.0f;
    vec3->y = 0.0f;
    vec3->z = 0.0f;
}

void
Vec3_Negate(Vec3 *vec3)
{
    vec3->x *= -1.0;
    vec3->y *= -1.0;
    vec3->z *= -1.0;
}

void
Vec3_AddVec3(Vec3 *vec3, const Vec3 *vec3_add)
{
    vec3->x += vec3_add->x;
    vec3->y += vec3_add->y;
    vec3->z += vec3_add->z;
}

void
Vec3_SubstractVec3(Vec3 *vec3, const Vec3 *vec3_sub)
{
    vec3->x -= vec3_sub->x;
    vec3->y -= vec3_sub->y;
    vec3->z -= vec3_sub->z;
}

void
Vec3_MultiplyScalar(Vec3 *vec3, f32 scalar)
{
    vec3->x *= scalar;
    vec3->y *= scalar;
    vec3->z *= scalar;
}

f32
Vec3_GetLengthSquared(Vec3 *vec3)
{
    f32 result = vec3->x * vec3->x + vec3->y * vec3->y + vec3->z * vec3->z;
    return result;
}

f32
Vec3_GetLength(Vec3 *vec3)
{    
    f32 result = Vec3_GetLengthSquared(vec3);
    result = sqrtf(result);
    return result;
}

b32
Vec3_Normalize(Vec3 *vec3)
{
    f32 length = Vec3_GetLength(vec3);   
    if (Math_IsNearlyEqual(length, 0.0f))
    {
        /* It is not possible to normalize the vector. */
        return false;
    }
    else
    {
        /* Normalization could be successfully performed. */
        f32 inverse_length = 1.0f / length;
        Vec3_MultiplyScalar(vec3, inverse_length);
        return true;
    }
}
