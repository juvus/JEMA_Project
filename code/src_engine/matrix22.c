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
    mat22->a11 = a11; mat22->a12 = a12;    
    mat22->a21 = a21; mat22->a22 = a22;
}

void
mat22_init_by_vec2(Mat22_t* mat22, const Vec2_t *col1, const Vec2_t *col2)
{
    mat22_init_by_f32(mat22, col1->x, col1->y, col2->x, col2->y);
}

void
mat22_init_by_mat22(Mat22_t* mat22, const Mat22_t* mat22_src)
{
    *mat22 = *mat22_src;
}

b32
mat22_is_valid(const Mat22_t *mat22)
{
    b32 result;  /* Result of the validation check. */

    result = isfinite(mat22->a11) && isfinite(mat22->a21) && isfinite(mat22->a12) && 
        isfinite(mat22->a22);
    return result;
}

b32
mat22_is_nearly_zero(const Mat22_t *mat22)
{
    b32 result;  /* Result of the check. */

    result = (is_nearly_equal(mat22->a11, 0.0f)) && (is_nearly_equal(mat22->a21, 0.0f)) &&
        (is_nearly_equal(mat22->a12, 0.0f)) && (is_nearly_equal(mat22->a22, 0.0f));
    return result;
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
mat22_multiply_scalar(Mat22_t* mat22, f32 scalar)
{
    mat22_init_by_f32(mat22, (mat22->a11 * scalar), (mat22->a21 * scalar), 
        (mat22->a12 * scalar), (mat22->a22 * scalar));
}

f32
mat22_calc_determinant(const Mat22_t *mat22)
{
    f32 det;  /* Matrix determinant. */

    det = mat22->a11 * mat22->a22 - mat22->a12 * mat22->a21;
    return det;
}

void
mat22_make_cofactor(Mat22_t *mat22)
{
    f32 a, b, c, d;  /* Temporary matrix elements. */
    f32 c11, c21, c12, c22;  /* Cofactor matrix elements. */

    a = mat22->a11; b = mat22->a12;
    c = mat22->a21; d = mat22->a22;

    /* Cofactor coefficients: c_ij = (-1)^(i+j) * m_ij. Term m_ij is a determinant of the 
    minor matrix. */
    c11 = d; c12 = -c; 
    c21 = -b; c22 = a;
    
    mat22_init_by_f32(mat22, c11, c21, c12, c22);
}

void
mat22_make_transpose(Mat22_t *mat22)
{
    f32 a, b, c, d;  /* Temporary matrix elements. */

    a = mat22->a11; b = mat22->a12;
    c = mat22->a21; d = mat22->a22;

    mat22_init_by_f32(mat22, a, b, c, d);
}

b32
mat22_make_inverse(Mat22_t *mat22)
{
    f32 det;  /* Matrix determinant. */
    f32 det_rev;  /* Reverse value of the determinant. */
    
    /* M^(-1) = 1/det(M) * adj(A) = 1/det(M) * cof(A)^(T). */
    det = mat22_calc_determinant(mat22);

    if (is_nearly_equal(det, 0.0f))
    {
        /* Not possible to get the inverse matrix. */
        return false;
    }
    else
    {
        /* Inverse matrix could be successfully calculated. */
        det_rev = 1.0f / det;
        mat22_make_cofactor(mat22);
        mat22_make_transpose(mat22);
        mat22_multiply_scalar(mat22, det_rev);
        return true;
    }
}
