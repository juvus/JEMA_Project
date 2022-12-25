/**
 * ================================================================================
 * @file src_engine/matrix22.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with 2-by-2 matrix with 
 * elements of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-06
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/matrix22.h"

#include <math.h>

#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/math_functions.h"
#include "include_engine/vector2.h"
#include "include_engine/utils.h"

Mat22*
Mat22_Constructor(void)
{
    size_t size = sizeof(Mat22);
    Mat22 *mat22 = (Mat22 *)HelperFcn_MemAllocate(size);
    return mat22;
}

Mat22*
Mat22_Destructor(Mat22 *mat22)
{
    HelperFcn_MemFree(mat22);
    return NULL;
}

void
Mat22_InitByF32(Mat22 *mat22, f32 a11, f32 a21, f32 a12, f32 a22)
{
    mat22->a11 = a11; mat22->a12 = a12;    
    mat22->a21 = a21; mat22->a22 = a22;
}

void
Mat22_InitByVec2(Mat22 *mat22, const Vec2 *col1, const Vec2 *col2)
{
    Mat22_InitByF32(mat22, col1->x, col1->y, col2->x, col2->y);
}

void
Mat22_InitByMat22(Mat22 *mat22, const Mat22 *mat22_src)
{
    *mat22 = *mat22_src;
}

b32
Mat22_IsValid(const Mat22 *mat22)
{
    for (u32 i = 0; i < 4; ++i)
    {
        if (!isfinite(mat22->elem_arr[i])) return false;
    }
    return true;
}

b32
Mat22_IsNearlyZero(const Mat22 *mat22)
{
    for (u32 i = 0; i < 4; ++i)
    {
        if (!Math_IsNearlyEqual(mat22->elem_arr[i], 0.0f)) return false;
    }
    return true;
}

void
Mat22_SetZero(Mat22 *mat22)
{
    Mat22_InitByF32(mat22, 0.0f, 0.0f, 0.0f, 0.0f);
}

void
Mat22_SetIdentity(Mat22 *mat22)
{
    Mat22_InitByF32(mat22, 1.0f, 0.0f, 0.0f, 1.0f);
}

void
Mat22_MultiplyScalar(Mat22 *mat22, f32 scalar)
{
    for (u32 i = 0; i < 4; ++i)
    {
        mat22->elem_arr[i] *= scalar;
    }
}

f32
Mat22_CalcDeterminant(const Mat22 *mat22)
{
    f32 det = mat22->a11 * mat22->a22 - mat22->a12 * mat22->a21;
    return det;
}

void
Mat22_MakeCofactor(Mat22 *mat22)
{
    f32 a = mat22->a11; f32 b = mat22->a12;
    f32 c = mat22->a21; f32 d = mat22->a22;

    /* Cofactor coefficients: c_ij = (-1)^(i+j) * m_ij. Term m_ij is a determinant of the 
    minor matrix. */
    f32 c11 = d; f32 c12 = -c; 
    f32 c21 = -b; f32 c22 = a;
    
    Mat22_InitByF32(mat22, c11, c21, c12, c22);
}

void
Mat22_MakeTranspose(Mat22 *mat22)
{
    f32 a = mat22->a11; f32 b = mat22->a12;
    f32 c = mat22->a21; f32 d = mat22->a22;

    Mat22_InitByF32(mat22, a, b, c, d);
}

b32
Mat22_MakeInverse(Mat22 *mat22)
{       
    f32 det = Mat22_CalcDeterminant(mat22);
    if (Math_IsNearlyEqual(det, 0.0f))
    {
        /* Not possible to get the inverse matrix. */
        return false;
    }
    else
    {
        /* Inverse matrix: M^(-1) = 1/det(M) * adj(A) = 1/det(M) * cof(A)^(T). */
        f32 det_rev = 1.0f / det;
        Mat22_MakeCofactor(mat22);
        Mat22_MakeTranspose(mat22);
        Mat22_MultiplyScalar(mat22, det_rev);
        return true;
    }
}
