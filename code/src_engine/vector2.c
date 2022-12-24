/**
 * ================================================================================
 * @file src_engine/vector2.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 2D column vectors 
 * with 2 coordinates of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-21
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/vector2.h"

#include <math.h>

#include "include_engine/dbg.h"
#include "include_engine/math_functions.h"
#include "include_engine/utils.h"

Vec2*
Vec2_Constructor(void)
{
    Vec2 *vec2 = (Vec2 *)malloc(1 * sizeof(Vec2));
    if (vec2 == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return vec2;
}

void
Vec2_Destructor(Vec2 *vec2)
{
    if (vec2 == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    free(vec2);
    vec2 = NULL;
}

void
Vec2_InitByF32(Vec2 *vec2, f32 x, f32 y)
{
    vec2->x = x;
    vec2->y = y;    
}

void
Vec2_InitByVec2(Vec2 *vec2, const Vec2 *vec2_src)
{
    *vec2 = *vec2_src;
}

b32
Vec2_IsValid(Vec2 *vec2)
{
    b32 result = isfinite(vec2->x) && isfinite(vec2->y); 
    return result;
}

b32
Vec2_IsNearlyZero(Vec2 *vec2)
{
    b32 result = Math_IsNearlyEqual(vec2->x, 0.0f) && Math_IsNearlyEqual(vec2->y, 0.0f); 
    return result;
}

void
Vec2_SetZero(Vec2 *vec2)
{
    vec2->x = 0.0f;
    vec2->y = 0.0f;
}

void
Vec2_Negate(Vec2 *vec2)
{
    vec2->x *= -1.0;
    vec2->y *= -1.0;
}

void
Vec2_AddVec2(Vec2 *vec2, const Vec2 *vec2_add)
{
    vec2->x += vec2_add->x;
    vec2->y += vec2_add->y;
}

void
Vec2_SubstractVec2(Vec2 *vec2, const Vec2 *vec2_sub)
{
    vec2->x -= vec2_sub->x;
    vec2->y -= vec2_sub->y;
}

void
Vec2_MultiplyScalar(Vec2 *vec2, f32 scalar)
{
    vec2->x *= scalar;
    vec2->y *= scalar;
}

f32
Vec2_GetLengthSquared(Vec2 *vec2)
{
    f32 result = vec2->x * vec2->x + vec2->y * vec2->y;
    return result;
}

f32
Vec2_GetLength(Vec2 *vec2)
{
    f32 result = Vec2_GetLengthSquared(vec2);
    result = sqrtf(result);
    return result;
}

b32
Vec2_Normalize(Vec2 *vec2)
{
    f32 length = Vec2_GetLength(vec2);    
    if (Math_IsNearlyEqual(length, 0.0f))
    {
        /* It is not possible to normalize the vector. */
        return false;
    }
    else
    {
        /* Normalization could be successfully performed. */
        f32 inverse_length = 1.0f / length;
        Vec2_MultiplyScalar(vec2, inverse_length);
        return true;
    }
}
