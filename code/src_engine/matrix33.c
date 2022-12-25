/**
 * ================================================================================
 * @file src_engine/matrix33.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 3-by-3 matrix with 
 * elements of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-09
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/matrix33.h"

#include <math.h>

#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/math_functions.h"
#include "include_engine/utils.h"
#include "include_engine/vector3.h"

Mat33*
Mat33_Constructor(void)
{
    size_t size = sizeof(Mat33);
    Mat33 *mat33 = (Mat33 *)HelperFcn_MemAllocate(size);
    return mat33;
}

Mat33*
Mat33_Destructor(Mat33 *mat33)
{
    HelperFcn_MemFree(mat33);
    return NULL;
}

void
Mat33_InitByF32(Mat33 *mat33, f32 a11, f32 a21, f32 a31, f32 a12, f32 a22, f32 a32,
    f32 a13, f32 a23, f32 a33)
{
    mat33->a11 = a11; mat33->a12 = a12; mat33->a13 = a13;
    mat33->a21 = a21; mat33->a22 = a22; mat33->a23 = a23;
    mat33->a31 = a31; mat33->a32 = a32; mat33->a33 = a33;
}

void
Mat33_InitByVec3(Mat33 *mat33, const Vec3 *col1, const Vec3 *col2, const Vec3 *col3)
{
    Mat33_InitByF32(mat33, col1->x, col1->y, col1->z, col2->x, col2->y, col2->z, 
        col3->x, col3->y, col3->z);
}

void
Mat33_InitByMat33(Mat33 *mat33, const Mat33 *mat33_src)
{
    *mat33 = *mat33_src;
}

b32
Mat33_IsValid(const Mat33 *mat33)
{
    for (u32 i = 0; i < 9; ++i)
    {
        if (!isfinite(mat33->elem_arr[i])) return false;
    }
    return true;
}

b32
Mat33_IsNearlyZero(const Mat33 *mat33)
{
    for (u32 i = 0; i < 9; ++i)
    {
        if (!Math_IsNearlyEqual(mat33->elem_arr[i], 0.0f)) return false;
    }
    return true;
}

void
Mat33_SetZero(Mat33 *mat33)
{
    Mat33_InitByF32(mat33, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

void
Mat33_SetIdentity(Mat33 *mat33)
{
    Mat33_InitByF32(mat33, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

void
Mat33_MultiplyScalar(Mat33 *mat33, f32 scalar)
{
    for (u32 i = 0; i < 9; ++i)
    {
        mat33->elem_arr[i] *= scalar;
    }
}

f32
Mat33_CalcDeterminant(const Mat33 *mat33)
{
    f32 a = mat33->a11; f32 b = mat33->a12; f32 c = mat33->a13;
    f32 d = mat33->a21; f32 e = mat33->a22; f32 f = mat33->a23;
    f32 g = mat33->a31; f32 h = mat33->a32; f32 i = mat33->a33;

    f32 det_mat22_a = e * i - f * h;
    f32 det_mat22_b = d * i - f * g;
    f32 det_mat22_c = d * h - e * g;

    f32 det = a * det_mat22_a - b * det_mat22_b + c * det_mat22_c;
    return det;
}

void
Mat33_MakeCofactor(Mat33 *mat33)
{
    f32 a = mat33->a11; f32 b = mat33->a12; f32 c = mat33->a13;
    f32 d = mat33->a21; f32 e = mat33->a22; f32 f = mat33->a23;
    f32 g = mat33->a31; f32 h = mat33->a32; f32 i = mat33->a33;

    /* Cofactor coefficients: c_ij = (-1)^(i+j) * m_ij. Term m_ij is a determinant of the 
    minor matrix. */
    f32 c11 = e * i - f * h; f32 c12 = -(d * i - f * g); f32 c13 = d * h - e * g;
    f32 c21 = -(b * i - c * h); f32 c22 = a * i - c * g; f32 c23 = -(a * h - b * g);
    f32 c31 = b * f - c * e; f32 c32 = -(a * f - c * d); f32 c33 = a * e - b * d;    
    
    Mat33_InitByF32(mat33, c11, c21, c31, c12, c22, c32, c13, c23, c33);
}

void
Mat33_MakeTranspose(Mat33 *mat33)
{
    f32 a = mat33->a11; f32 b = mat33->a12; f32 c = mat33->a13;
    f32 d = mat33->a21; f32 e = mat33->a22; f32 f = mat33->a23;
    f32 g = mat33->a31; f32 h = mat33->a32; f32 i = mat33->a33;
    
    Mat33_InitByF32(mat33, a, b, c, d, e, f, g, h, i);
}

b32
Mat33_MakeInverse(Mat33 *mat33)
{
    f32 det = Mat33_CalcDeterminant(mat33);
    if (Math_IsNearlyEqual(det, 0.0f))
    {
        /* Not possible to get the inverse matrix. */
        return false;
    }
    else
    {
        /* Inverse matrix: M^(-1) = 1/det(M) * adj(A) = 1/det(M) * cof(A)^(T). */
        f32 det_rev = 1.0f / det;
        Mat33_MakeCofactor(mat33);
        Mat33_MakeTranspose(mat33);
        Mat33_MultiplyScalar(mat33, det_rev);
        return true;
    }
}
