/**
 * ================================================================================
 * @file src_engine/random.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with generation of random
 * numbers.
 * @version 0.2
 * @date 2022-12-15
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/random.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "include_engine/dbg.h"
#include "include_engine/utils.h"

void
Random_Randomize(b32 rnd_flag, u32 seed)
{
    if (rnd_flag) srand((unsigned int)time(NULL));    
    if (!rnd_flag) srand((unsigned int)seed);
}

f32
Random_F32(void)
{
    f32 result = (f32)((rand()) / (f32)RAND_MAX);
    return result;
}

u32
Random_RangeU32(u32 low_bnd, u32 high_bnd)
{   
    /* Different offset are required to achieve the uniformity of the dustribution. */
    f32 tmp = Random_RangeF32((f32)low_bnd + 0.5f, (f32)high_bnd + 1.5f);
    tmp = roundf(tmp);
    u32 result = (u32)(tmp - 1.0f);
    return result;
}

f32
Random_RangeF32(f32 low_bnd, f32 high_bnd)
{   
    dbg_check((low_bnd <= high_bnd), "Lower bound should be lower than upper bound!");
    f32 result = low_bnd + Random_F32() * (high_bnd - low_bnd);
    return result;
}
