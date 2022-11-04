/**
 * ================================================================================
 * @file vector3.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 2D column vectors 
 * with 3 coordinates of f32 (32 bits) floating point type.
 * @version 0.1
 * @date 2022-07-15
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard library includes: */
#include <math.h>

/* Game engine includes: */
#include "vector3.h"
#include "utils.h"

Vec3_t*
vec3_constructor(void)
{
    Vec3_t *vec3;  /* Pointer to the Vec3 structure. */

    /* Memory allocation for the object. */
    vec3 = (Vec3_t*)malloc(1 * sizeof(Vec3_t));
    return vec3;
}

void
vec3_destructor(Vec3_t *vec3)
{
    /* Free memory allocated for the object. */
    free(vec3);
    vec3 = NULL;
}

void
vec3_init_by_f32(Vec3_t* vec3, f32 x, f32 y, f32 z)
{
    vec3->x = x;
    vec3->y = y;
    vec3->z = z;
}

void
vec3_init_by_vec3(Vec3_t* vec3, const Vec3_t* vec3_src)
{
    *vec3 = *vec3_src;
}

b32
vec3_is_valid(Vec3_t *vec3)
{
    b32 result;  /* Result of the validation check. */

    result = isfinite(vec3->x) && isfinite(vec3->y) && isfinite(vec3->z);
    return result;
}

b32
vec3_is_nearly_zero(Vec3_t *vec3)
{
    b32 result;  /* Result of the check. */

    result = (is_nearly_equal(vec3->x, 0.0f)) && (is_nearly_equal(vec3->y, 0.0f)) &&
        (is_nearly_equal(vec3->z, 0.0f));
    return result;
}

void
vec3_set_zero(Vec3_t* vec3)
{
    vec3_init_by_f32(vec3, 0.0f, 0.0f, 0.0f);
}

void
vec3_negate(Vec3_t* vec3)
{
    vec3_init_by_f32(vec3, -(vec3->x), -(vec3->y), -(vec3->z));
}

void
vec3_add_vec3(Vec3_t* vec3_a, const Vec3_t* vec3_b)
{
    vec3_init_by_f32(vec3_a, (vec3_a->x + vec3_b->x), (vec3_a->y + vec3_b->y),
        (vec3_a->z + vec3_b->z));
}

void
vec3_substract_vec3(Vec3_t* vec3_a, const Vec3_t* vec3_b)
{
    vec3_init_by_f32(vec3_a, (vec3_a->x - vec3_b->x), (vec3_a->y - vec3_b->y),
        (vec3_a->z - vec3_b->z));
}

void
vec3_multiply_scalar(Vec3_t* vec3, f32 scalar)
{
    vec3_init_by_f32(vec3, (vec3->x * scalar), (vec3->y * scalar),
        (vec3->z * scalar));
}

f32
vec3_get_length_squared(Vec3_t* vec3)
{
    f32 result;  /* Result of the calculations. */
    
    result = vec3->x * vec3->x + vec3->y * vec3->y + vec3->z * vec3->z;
    return result;
}

f32
vec3_get_length(Vec3_t* vec3)
{
    f32 result;  /* Result of the calculations. */
    
    result = vec3_get_length_squared(vec3);
    result = sqrtf(result);
    return result;
}

b32
vec3_normalize(Vec3_t* vec3)
{
    f32 length;  /* Vector length. */
    f32 inverse_length;  /* Inverse vector length. */

    length = vec3_get_length(vec3);
    
    if (is_nearly_equal(length, 0.0f))
    {
        /* It is not possible to normalize the vector. */
        return false;
    }
    else
    {
        /* Normalization could be successfully performed. */
        inverse_length = 1.0f / length;
        vec3_multiply_scalar(vec3, inverse_length);
        return true;
    }
}
