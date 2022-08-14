/**
 * ================================================================================
 * @file random.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with random numbers.
 * @version 0.2
 * @date 2022-07-15
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard includes: */
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>

/* Program includes: */
#include "random.h"
#include "utils.h"

void
random_randomize(b32 rnd_flag, u32 seed)
{
    /* Seed the random numbers. */
    if (rnd_flag) srand((unsigned int)time(NULL));    
    if (!rnd_flag) srand((unsigned int)seed);
}

f32
random_f32(void)
{
    f32 result;  /* Returning random f32 number in range [0.0, 1.0]. */
    
    result = (f32)((rand()) / (f32)RAND_MAX);
    return result;
}

u32
random_range_u32(u32 low_bnd, u32 high_bnd)
{
    f32 tmp_f32;  /* Temporary f32 number. */
    u32 result;  /* Returning random u32 number in range [low_bnd, hi_bnd]. */
    
    /* Getting the random number. Different offset are required to achieve the 
    uniformity of the dustribution. */
    assert(low_bnd <= high_bnd);
    tmp_f32 = random_range_f32((f32)low_bnd + 0.5f, (f32)high_bnd + 1.5f);
    tmp_f32 = roundf(tmp_f32);
    result = (u32)(tmp_f32 - 1.0f);
    return result;
}

f32
random_range_f32(f32 low_bnd, f32 high_bnd)
{
    f32 result;  /* Returning random f32 number in range [low_bnd, hi_bnd]. */
    
    assert(low_bnd <= high_bnd);
    result = low_bnd + random_f32() * (high_bnd - low_bnd);
    return result;
}