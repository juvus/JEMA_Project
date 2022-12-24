/**
 * ================================================================================
 * @file include_engine/matrix22.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 
 * 2-by-2 matrix with elements of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-06
 * ================================================================================ 
 */

#ifndef JGE_ENGINE_MATRIX22_H_
#define JGE_ENGINE_MATRIX22_H_

#include "include_engine/utils.h"

typedef struct Vec2_ Vec2;

/**
 * @brief Structure to store the 2-by-2 matrix with elements of f32 type.
 */
struct Mat22_
{
    union
    {
        struct
        {
            f32 a11;  /**< Matrix 1st element of the 1st column. */
            f32 a21;  /**< Matrix 2nd element of the 1st column. */ 
            f32 a12;  /**< Matrix 1st element of the 2nd column. */
            f32 a22;  /**< Matrix 2nd element of the 2nd column. */
        };
        f32 elem_arr[4];  /* Array of the matrix elements. */
    };
};
typedef struct Mat22_ Mat22;

/**
 * @brief Object constructor.
 * @return Mat22* Pointer to the Mat22 structure.
 */
Mat22*
Mat22_Constructor(void);

/**
 * @brief Object descructor.
 * @param mat22 Pointer to the Mat22 structure.
 */
void
Mat22_Destructor(Mat22 *mat22);

/**
 * @brief Initialization of the matrix by element values.
 * @param mat22 Pointer to the Mat22 structure.
 * @param a11 Matrix 1st element of the 1st column.
 * @param a21 Matrix 2nd element of the 1st column.
 * @param a12 Matrix 1st element of the 2nd column.
 * @param a22 Matrix second element of the second column.
 */
void
Mat22_InitByF32(Mat22 *mat22, f32 a11, f32 a21, f32 a12, f32 a22);

/**
 * @brief Initialization of the matrix by column vectors.
 * @param mat22 Pointer to the Mat22 structure.
 * @param col1 Pointer to the Vec2 structure (first column).
 * @param col2 Pointer to the Vec2 structure (second column).
 */
void
Mat22_InitByVec2(Mat22 *mat22, const Vec2 *col1, const Vec2 *col2);

/**
 * @brief Initialization of the matrix by data from the source matrix.
 * @param mat22 Pointer to Mat22 structure (destination).
 * @param mat22_src Pointer to Mat22 structure (source).
 */
void
Mat22_InitByMat22(Mat22 *mat22, const Mat22 *mat22_src);

/**
 * @brief Determination if matrix contains finite elements. 
 * @param mat22 Pointer to Mat22 structure.
 * @return b32 True - valid matrix. False - not valid matrix.
 */
b32
Mat22_IsValid(const Mat22 *mat22);

/**
 * @brief Determination if matrix is a nearly zero matrix.
 * @param mat22 Pointer to Mat22 structure.
 * @return b32 Result of the check.
 */
b32
Mat22_IsNearlyZero(const Mat22 *mat22);

/**
 * @brief Set all matrix elements to zeros.
 * @param mat22 Pointer to Mat22 structure.
 */
void
Mat22_SetZero(Mat22 *mat22);

/**
 * @brief Set matrix to the identity matrix.
 * @param mat22 Pointer to Mat22 structure.
 */
void
Mat22_SetIdentity(Mat22 *mat22);

/**
 * @brief Multiply the matrix by a scalar.
 * @param mat22 Pointer to Mat22 structure.
 * @param scalar Arbitrary scalar.
 */
void
Mat22_MultiplyScalar(Mat22 *mat22, f32 scalar);

/**
 * @brief Calculation of the 2-by-2 matrix determinant.
 * @param mat22 Pointer to Mat22 structure.
 * @return f32 Matrix determinant value. 
 */
f32
Mat22_CalcDeterminant(const Mat22 *mat22);

/**
 * @brief Make transposition of the matrix.
 * @param mat22 Pointer to Mat22 structure.
 */
void
Mat22_MakeTranspose(Mat22 *mat22);

/**
 * @brief Make cofactor matrix from the given matrix.
 * @param mat22 Pointer to Mat22 structure.
 */
void
Mat22_MakeCofactor(Mat22 *mat22);

/**
 * @brief Make invesion of the matrix.
 * @param mat22 Pointer to Mat22 structure.
 * @return b32 Flag showing if the invesion was successful or not. 
 */
b32
Mat22_MakeInverse(Mat22 *mat22);

#endif /* JGE_ENGINE_MATRIX22_H_ */
