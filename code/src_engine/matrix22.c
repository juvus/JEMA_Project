/**
 * ================================================================================
 * @file matrix22.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 2-by-2 matrix with 
 * elements of f32 (32 bits) floating point type.
 * @version 0.1
 * @date 2022-11-02
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard library includes: */
#include <math.h>

/* Game engine includes: */
#include "matrix22.h"
#include "vector2.h"
#include "utils.h"
#include "engine_math.h"

Mat22_t*
mat22_constructor(void)
{
    Mat22_t *mat22;  /* Pointer to the Mat22 structure. */

    /* Memory allocation for the object. */
    mat22 = (Mat22_t*)malloc(1 * sizeof(Mat22_t));
     
    return mat22;
}

void
mat22_destructor(Mat22_t* mat22)
{
    /* Free memory allocated for the object. */
    free(mat22);
    mat22 = NULL;
}

void
mat22_init_by_f32(Mat22_t* mat22, f32 a11, f32 a21, f32 a12, f32 a22)
{
    mat22->a11 = a11;
    mat22->a21 = a21;
    mat22->a12 = a12;
    mat22->a22 = a22;
}

void
mat22_init_by_vec2(Mat22_t* mat22, const Vec2_t *col_1, const Vec2_t *col_2)
{
    mat22_init_by_f32(mat22, col_1->x, col_1->y, col_2->x, col_2->y);
}

void
mat22_init_by_mat22(Mat22_t* mat22, const Mat22_t* mat22_src)
{
    *mat22 = *mat22_src;
}

b32
mat22_is_valid(Mat22_t *mat22)
{

}

b32
mat22_is_nearly_zero(Mat22_t *mat22)
{

    
}



void
mat22_set_zero(Mat22_t* mat22)
{
    mat22_init_by_f32(mat22, 0.0f, 0.0f, 0.0f, 0.0f);
}

void
mat22_set_identity(Mat22_t *mat22)
{
    mat22_init_by_f32(mat22, 1.0f, 0.0f, 0.0f, 1.0f);
}

void
mat22_make_inverse(Mat22_t *mat22)
{
    f32 a, b, c, d;  /* Temporary floats. */
    f32 det;  /* Matrix determinant. */
    f32 det_rev;  /* Reverse value of the determinant. */

    a = mat22->a11; 
    b = mat22->a12;
    c = mat22->a21;
    d = mat22->a22;
    det = a * d - b * c;
    det_rev = 0.0f;

    if (!is_nearly_equal(det, 0.0f))
    {
        det_rev = 1.0f / det;
    }
    mat22_init_by_f32(mat22, det_rev * d, -det_rev * c, -det_rev * b, det_rev * a);
}

