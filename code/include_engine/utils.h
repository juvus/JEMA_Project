/**
 * ================================================================================
 * @file utils.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Header file with different common deffinitions.
 * @version 0.1
 * @date 2022-07-23
 * ================================================================================ 
 */

#ifndef UTILS_H_
#define UTILS_H_

/* Standard library includes. */
#include <stdint.h>
#include <windows.h>

/* Window constants */
#define WINDOW_WIDTH 1280  /* Width of the whole windows window. */
#define WINDOW_HEIGHT 720  /* Height of the whole windows window. */

/* Redefined types. */
typedef int8_t s8;  /* 8 bits (1 byte) signed integer. [-128, 127]. */
typedef uint8_t u8;  /* 8 bits (1 byte) unsigned integer. [0, 255]. */
typedef int16_t s16;  /* 16 bits (2 bytes) signed integer. [-32768, 32767]. */
typedef uint16_t u16;  /* 16 bits (2 bytes) unsigned integer. [0, 65535]. */
typedef int32_t s32;  /* 32 bits (4 bytes) signed integer. [-2147483648, 2147483647]. */
typedef uint32_t u32;  /* 32 bits (4 bytes) unsigned integer. [0, 4294967295]. */
typedef int64_t s64;  /* 64 bits (8 bytes) signed integer. [-2^(64-1), 2^(64-1)-1]. */
typedef uint64_t u64;  /* 64 bits (8 bytes) unsigned integer. [0, 2^64]. */
typedef int b32;  /* Boolean type. True (other than 0) or false (0). */
typedef float f32;  /* 32 bits floating point number. [1.2E-38, 3.4E+38]. 8 signif. digits. */
typedef double f64;  /* 64 bits floating point number. [2.3E-308, 1.7E+308]. 16 signif. digits. */

#define true 1  /* Boolean true. */
#define false 0  /* Boolean false. */

// Macro to suppress "unused function parameter".
#define UNUSED(x) (void)(x)

/* Vector of 2 f32 elements. */
struct V2_f32
{
    union
    {
        struct
        {
            f32 x;  /**< Vector x coordinate. */
            f32 y;  /**< Vector y coordinate. */
        };
        f32 crd_arr[2];  /**< Coordinates array. */
    };
};
typedef struct V2_f32 V2_f32_t;

/* Vector of 2 u32 elements. */
struct V2_u32
{
    union
    {
        struct
        {
            u32 x;  /**< Vector x coordinate. */
            u32 y;  /**< Vector y coordinate. */
        };
        u32 crd_arr[2];  /**< Coordinates array. */
    };
};
typedef struct V2_u32 V2_u32_t;

/* Vector of 2 s32 elements. */
struct V2_s32
{
    union
    {
        struct
        {
            s32 x;  /**< Vector x coordinate. */
            s32 y;  /**< Vector y coordinate. */
        };
        s32 crd_arr[2];
    };
};
typedef struct V2_s32 V2_s32_t;

/* Addition of two vectors of V2_f32 type. */
inline V2_f32_t 
add_V2_f32(V2_f32_t a, V2_f32_t b)
{
    V2_f32_t result;
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

/* Element-wise multiplication of vector V2_f32 with number */
inline V2_f32_t
mul_V2_f32(V2_f32_t a, f32 num) {
    V2_f32_t result;

    result.x = a.x * num;
    result.y = a.y * num;
    return result;
}

#endif // UTILS_H_
