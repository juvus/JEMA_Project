/**
 * ================================================================================
 * @file src_engine/math_functions.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for performing different calculations.
 * @version 0.2
 * @date 2022-12-04
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/math_functions.h"

#include <math.h>

#include "include_engine/vector2.h"
#include "include_engine/utils.h"

b32
Math_IsNearlyEqual(f32 x, f32 y)
{
    f32 abs_diff = fabsf(x - y);

    if (abs_diff >= 1.0f)
    {
        f32 abs_max = fmaxf(fabsf(x), fabsf(y));
        return (abs_diff < (MIN_F32_EPSILON * abs_max));
    }
    else
    {
        return (abs_diff < MIN_F32_EPSILON);
    }
}

f32
Math_TrimF32(f32 value, f32 low_bnd, f32 high_bnd)
{
    if (value < low_bnd) return low_bnd;
    if (value > high_bnd) return high_bnd;
    return value;
}

void 
Math_SwapVec2(Vec2 *vec2_a, Vec2 *vec2_b)
{
    f32 x_temp = vec2_a->x;
    f32 y_temp = vec2_a->y;
    vec2_a->x = vec2_b->x;
    vec2_a->y = vec2_b->y;
    vec2_b->x = x_temp;
    vec2_b->y = y_temp;
}

f32
Math_Map(f32 value, f32 lower_bnd_1, f32 upper_bnd_1, f32 lower_bnd_2, f32 upper_bnd_2)
{
    /* Add code here. */
    value = 1.0f;
    lower_bnd_1 = 1.0f;
    upper_bnd_1 = 1.0f;
    lower_bnd_2 = 1.0f;
    upper_bnd_2 = 1.0f;
    return 31337.0;
}