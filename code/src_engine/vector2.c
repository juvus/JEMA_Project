/**
 * ================================================================================
 * @file vector2.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 2D column vectors 
 * with 2 coordinates of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-07-15
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard library includes: */
#include <math.h>

/* Game engine includes: */
#include "vector2.h"
#include "utils.h"
#include "engine_math.h"

Vec2_t*
vec2_constructor(void)
{
    Vec2_t *vec2;  /* Pointer to the Vec2 structure. */

    /* Memory allocation for the object. */
    vec2 = (Vec2_t*)malloc(1 * sizeof(Vec2_t));
     
    return vec2;
}

void
vec2_destructor(Vec2_t *vec2)
{
    /* Free memory allocated for the object. */
    free(vec2);
    vec2 = NULL;
}

void
vec2_init_by_f32(Vec2_t* vec2, f32 x, f32 y)
{
    vec2->x = x;
    vec2->y = y;    
}

void
vec2_init_by_vec2(Vec2_t* vec2, const Vec2_t* vec2_src)
{
    *vec2 = *vec2_src;
}

b32
vec2_is_valid(Vec2_t *vec2)
{
    b32 result;  /* Result of the validation check. */

    result = isfinite(vec2->x) && isfinite(vec2->y);
    return result;
}

b32
vec2_is_nearly_zero(Vec2_t *vec2)
{
    b32 result;  /* Result of the check. */

    result = (is_nearly_equal(vec2->x, 0.0f)) && (is_nearly_equal(vec2->y, 0.0f));
    return result;
}

void
vec2_set_zero(Vec2_t* vec2)
{
    vec2_init_by_f32(vec2, 0.0f, 0.0f);
}

void
vec2_negate(Vec2_t* vec2)
{
    vec2_init_by_f32(vec2, -(vec2->x), -(vec2->y));
}

void
vec2_add_vec2(Vec2_t* vec2_a, const Vec2_t* vec2_b)
{
    vec2_init_by_f32(vec2_a, (vec2_a->x + vec2_b->x), (vec2_a->y + vec2_b->y));
}

void
vec2_substract_vec2(Vec2_t* vec2_a, const Vec2_t* vec2_b)
{
    vec2_init_by_f32(vec2_a, (vec2_a->x - vec2_b->x), (vec2_a->y - vec2_b->y));
}

void
vec2_multiply_scalar(Vec2_t* vec2, f32 scalar)
{
    vec2_init_by_f32(vec2, (vec2->x * scalar), (vec2->y * scalar));
}

f32
vec2_get_length_squared(Vec2_t* vec2)
{
    f32 result;  /* Result of the calculations. */
    
    result = vec2->x * vec2->x + vec2->y * vec2->y;
    return result;
}

f32
vec2_get_length(Vec2_t* vec2)
{
    f32 result;  /* Result of the calculations. */
    
    result = vec2_get_length_squared(vec2);
    result = sqrtf(result);
    return result;
}

f32
vec2_normalize(Vec2_t* vec2)
{
    f32 length;  /* Vector length. */
    f32 inverse_length;  /* Inverse vector length. */

    length = vec2_get_length(vec2);
    
    if (is_nearly_equal(length, 0.0f))
    {
        return 0.0f;
    }

    inverse_length = 1.0f / length;
    vec2_multiply_scalar(vec2, inverse_length);
    return length;
}
