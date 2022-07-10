/**
 * ================================================================================
 * @file random.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with random numbers.
 * @version 0.2
 * @date 2022-07-10
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard includes: */
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Program includes: */
#include <utils.h>

f32
random_f32(void)
{
    f32 result;
    
    result = (f32)((rand() % 32767) / (f32)32767);
    return result;
}

u32
random_range_u32(u32 left_bnd, u32 right_bnd)
{
    assert(left_bnd <= right_bnd);
    u32 result;
    result = left_bnd + (rand() % (right_bnd - left_bnd));
    return result;
}

f32
random_range_f32(f32 left_bnd, f32 right_bnd)
{
    assert(left_bnd <= right_bnd);
    f32 result;
    result = left_bnd + rnd_f32() * (right_bnd - left_bnd);
    return result;
}