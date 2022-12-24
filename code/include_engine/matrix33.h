/**
 * ================================================================================
 * @file include_engine/matrix33.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 
 * 3-by-3 matrix with elements of f32 (32 bits) floating point type.
 * @version 0.2
 * @date 2022-12-06
 * ================================================================================ 
 */

#ifndef JEMA_ENGINE_MATRIX33_H_
#define JEMA_ENGINE_MATRIX33_H_

#include "include_engine/utils.h"

typedef struct Vec3_ Vec3;

/**
 * @brief Structure to store the 3-by-3 matrix with elements of f32 type.
 */
struct Mat33_
{
    union
    {
        struct
        {
            f32 a11;  /**< Matrix 1st element of the 1st column. */
            f32 a21;  /**< Matrix 2nd element of the 1st column. */ 
            f32 a31;  /**< Matrix 3rd element of the 1st column. */
            f32 a12;  /**< Matrix 1st element of the 2nd column. */
            f32 a22;  /**< Matrix 2nd element of the 2nd column. */
            f32 a32;  /**< Matrix 3rd element of the 2nd column. */
            f32 a13;  /**< Matrix 1st element of the 3rd column. */
            f32 a23;  /**< Matrix 2nd element of the 3rd column. */
            f32 a33;  /**< Matrix 3rd element of the 3rd column. */
        };
        f32 elem_arr[9];  /* Array of the matrix elements. */
    };
};
typedef struct Mat33_ Mat33;

/**
 * @brief Object constructor.
 * @return Mat33* Pointer to the Mat33 structure.
 */
Mat33*
Mat33_Constructor(void);

/**
 * @brief Object descructor.
 * @param mat33 Pointer to the Mat33 structure.
 */
void
Mat33_Destructor(Mat33 *mat33);

/**
 * @brief Initialization of the matrix by element values.
 * @param mat33 Pointer to the Mat33 structure.
 * @param a11 Matrix 1st element of the 1st column.
 * @param a21 Matrix 2nd element of the 1st column.
 * @param a31 Matrix 3rd element of the 1st column.
 * @param a12 Matrix 1st element of the 2nd column.
 * @param a22 Matrix 2nd element of the 2nd column.
 * @param a32 Matrix 3rd element of the 2nd column.
 * @param a13 Matrix 1st element of the 3rd column.
 * @param a23 Matrix 2nd element of the 3rd column.
 * @param a33 Matrix 3rd element of the 3rd column.
 */
void
Mat33_InitByF32(Mat33 *mat33, f32 a11, f32 a21, f32 a31, f32 a12, f32 a22, f32 a32, 
    f32 a13, f32 a23, f32 a33);

/**
 * @brief Initialization of the matrix by column vectors.
 * @param mat33 Pointer to the Mat33 structure.
 * @param col1 Pointer to the Vec3 structure (first column).
 * @param col2 Pointer to the Vec3 structure (second column).
 * @param col3 Pointer to the Vec3 structure (third column).
 */
void
Mat33_InitByVec3(Mat33 *mat33, const Vec3 *col1, const Vec3 *col2, const Vec3 *col3);

/**
 * @brief Initialization of the matrix by data from the source matrix.
 * @param mat33 Pointer to Mat33 structure (destination).
 * @param mat33_src Pointer to Mat33 structure (source).
 */
void
Mat33_InitByMat33(Mat33 *mat33, const Mat33 *mat33_src);

/**
 * @brief Determination if matrix contains finite elements. 
 * @param mat33 Pointer to Mat33 structure.
 * @return b32 True - valid matrix. False - not valid matrix.
 */
b32
Mat33_IsValid(const Mat33 *mat33);

/**
 * @brief Determination if matrix is a nearly zero matrix.
 * @param mat33 Pointer to Mat33 structure.
 * @return b32 Result of the check.
 */
b32
Mat33_IsNearlyZero(const Mat33 *mat33);

/**
 * @brief Set all matrix elements to zeros.
 * @param mat33 Pointer to Mat33 structure.
 */
void
Mat33_SetZero(Mat33 *mat33);

/**
 * @brief Set matrix to the identity matrix.
 * @param mat33 Pointer to Mat33 structure.
 */
void
Mat33_SetIdentity(Mat33 *mat33);

/**
 * @brief Multiply the matrix by a scalar.
 * @param mat33 Pointer to Mat33 structure.
 * @param scalar Arbitrary scalar.
 */
void
Mat33_MultiplyScalar(Mat33 *mat33, f32 scalar);

/**
 * @brief Calculation of the 3-by-3 matrix determinant.
 * @param mat33 Pointer to Mat33 structure.
 * @return f32 Matrix determinant value. 
 */
f32
Mat33_CalcDeterminant(const Mat33 *mat33);

/**
 * @brief Make transposition of the matrix.
 * @param mat33 Pointer to Mat33 structure.
 */
void
Mat33_MakeTranspose(Mat33 *mat33);

/**
 * @brief Make cofactor matrix from the given matrix.
 * @param mat33 Pointer to Mat33 structure.
 */
void
Mat33_MakeCofactor(Mat33 *mat33);

/**
 * @brief Make invesion of the matrix.
 * @param mat33 Pointer to Mat33 structure.
 * @return b32 Flag showing if the invesion was successful or not. 
 */
b32
Mat22_MakeInverse(Mat33 *mat33);

#endif  /* JEMA_ENGINE_MATRIX33_H_ */
