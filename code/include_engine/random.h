/**
 * ================================================================================
 * @file include_engine/random.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with 
 * generation of random numbers.
 * @version 0.3
 * @date 2022-12-10
 * ================================================================================
 */

#ifndef JEMA_ENGINE_RANDOM_H_
#define JEMA_ENGINE_RANDOM_H_

#include "include_engine/utils.h"

/**
 * @brief Randomization of the random number generator.
 * @param rnd_flag Randomization flag for the seed.
 * @param seed Value of the seed (ignored when rnd_flag = true).
 */
void
Random_Randomize(b32 rnd_flag, u32 seed);

/**
 * @brief Generation of the random number in range [0.0, 1.0].
 * @return Random f32 number.
 */
f32 
Random_F32(void);

/**
 * @brief Generation of random u32 number in the range [low_bnd, high_bnd].
 * @param low_bnd Low boundary of the random number. 
 * @param high_bnd High boundary of the random number.
 * @return Random u32 number.
 */
u32 
Random_RangeU32(u32 low_bnd, u32 high_bnd);

/**
 * @brief Generation of random f32 number in the range [low_bnd, high_bnd].
 * @param low_bnd Low boundary of the random number.
 * @param high_bnd High boundary of the random number.
 * @return Random f32 number.
 */
f32 
Random_RangeF32(f32 low_bnd, f32 high_bnd);

#endif  /* JEMA_ENGINE_RANDOM_H_ */
