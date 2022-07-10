/**
 * ================================================================================
 * @file random.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with random
 * numbers.
 * @version 0.2
 * @date 2022-07-08
 * ================================================================================
 */

#ifndef RANDOM_H_
#define RANDOM_H_

/* Game engine includes: */
#include <utils.h>

/**
 * @brief Generation of the random number in range [0.0, 1.0).
 * @return f32 Return random number.
 */
f32 
random_f32(void);

/**
 * @brief Generation of random u32 number in the specified range.
 * @param left_bnd Left boundary of the random number. 
 * @param right_bnd Right boundary of the random number.
 * @return u32 Return random number.
 */
u32 
random_range_u32(u32 left_bnd, u32 right_bnd);

/**
 * @brief Generation of random f32 number in the specified range.
 * @param left_bnd Left boundary of the random number.
 * @param right_bnd Right boundary of the random number.
 * @return f32 Return random number.
 */
f32 
random_range_f32(f32 left_bnd, f32 right_bnd);

#endif // RANDOM_H_