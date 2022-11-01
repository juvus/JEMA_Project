/**
 * ================================================================================
 * @file vector2.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 2D column vectors 
 * that have its coordinates of f32 floating point type. Set of functions and it's 
 * definitions were mostly copied from the box2d library (https://box2d.org/).
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

b32
vec2_is_valid(Vec2_t *vec2)
{
    b32 result;  /* Result of the validation check. */

    result = isfinite(vec2->x) && isfinite(vec2->y);
    return result;
}

void
vec2_set_coords(Vec2_t* vec2, f32 x, f32 y)
{
    vec2->x = x;
    vec2->y = y;    
}

void
vec2_set_zero(Vec2_t* vec2)
{
    vec2_set_coords(vec2, 0.0f, 0.0f);
}

void
vec2_set_copy(Vec2_t* vec2_dest, const Vec2_t* vec2_src)
{
    vec2_set_coords(vec2_dest, vec2_src->x, vec2_src->y);
}

Vec2_t
vec2_get_copy(const Vec2_t* vec2)
{
    Vec2_t result;  /* Resulting vector. */

    vec2_set_coords(&result, vec2->x, vec2->y);
    return result;
}

void
vec2_negate(Vec2_t* vec2)
{
    vec2_set_coords(vec2, -(vec2->x), -(vec2->y));
}

Vec2_t
vec2_get_negative(const Vec2_t* vec2)
{
    Vec2_t result;  /* Resulting vector. */

    vec2_set_copy(&result, vec2);
    vec2_negate(&result);
    return result;
}

void
vec2_add_vec2(Vec2_t* vec2_a, const Vec2_t* vec2_b)
{
    vec2_set_coords(vec2_a, (vec2_a->x + vec2_b->x), (vec2_a->y + vec2_b->y));
}

void
vec2_substract_vec2(Vec2_t* vec2_a, const Vec2_t* vec2_b)
{
    vec2_set_coords(vec2_a, (vec2_a->x - vec2_b->x), (vec2_a->y - vec2_b->y));
}

void
vec2_multiply_scalar(Vec2_t* vec2, f32 scalar)
{
    vec2_set_coords(vec2, (vec2->x * scalar), (vec2->y * scalar));
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
    if (length < MIN_F32_EPSILON)
    {
        return 0.0f;
    }

    inverse_length = 1.0f / length;
    vec2_multiply_scalar(vec2, inverse_length);
    return length;
}



