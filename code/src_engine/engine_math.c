/**
 * ================================================================================
 * @file engine_math.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for performing different calculations.
 * @version 0.2
 * @date 2022-07-15
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard library includes: */
#include <math.h>

/* Game engine includes: */
#include "utils.h"

b32
is_nearly_equal(f32 x, f32 y)
{
    b32 result;  /* Result of the comparison. */
    f32 abs_diff;  /* Absolute difference of the input numbers. */
    f32 abs_max;  /* Absolute maximum number. */

    abs_diff = fabs(x - y);
    if (abs_diff >= 1.0f)
    {
        abs_max = fmax(fabs(x), fabs(y));
        result = abs_diff < (MIN_F32_EPSILON * abs_max);
    }
    else
    {
        result = abs_diff < MIN_F32_EPSILON;
    }
    return result;
}