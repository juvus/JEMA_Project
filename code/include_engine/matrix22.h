/**
 * ================================================================================
 * @file matrix22.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of structures and functions necessary for the work with 
 * 2-by-2 matrix with elements of f32 (32 bits) floating point type. Set of 
 * functions were mostly copied from the box2d library (https://box2d.org/).
 * @version 0.1
 * @date 2022-11-02
 * ================================================================================ 
 */

#ifndef MATRIX22_H_
#define MATRIX22_H_

/* Game engine includes: */
#include "utils.h"

/* Predefined structures: */
typedef struct Vec2 Vec2_t;

/**
 * @brief Structure to store the 2-by-2 matrix with elements of f32 type.
 */
struct Mat22
{
    f32 a11;  /**< Matrix first element of the first column. */
    f32 a21;  /**< Matrix second element of the first column. */ 
    f32 a12;  /**< Matrix first element of the second column. */
    f32 a22;  /**< Matrix second element of the second column. */
};
typedef struct Mat22 Mat22_t;

/**
 * @brief Class constructor.
 * @return Mat22_t* Pointer to the Mat22 structure.
 */
Mat22_t*
mat22_constructor(void);

/**
 * @brief Class descructor.
 * @param mat22 Pointer to the Mat22 structure.
 */
void
mat22_destructor(Mat22_t* mat22);

/**
 * @brief Initialization of the matrix by element values.
 * @param mat22 Pointer to the Mat22 structure.
 * @param a11 Matrix first element of the first column.
 * @param a21 Matrix second element of the first column.
 * @param a12 Matrix first element of the second column.
 * @param a22 Matrix second element of the second column.
 */
void
mat22_init_by_f32(Mat22_t* mat22, f32 a11, f32 a21, f32 a12, f32 a22);

/**
 * @brief Initialization of the matrix by column vectors.
 * @param mat22 Pointer to the Mat22 structure.
 * @param col_1 Pointer to the Vec2 structure (first column).
 * @param col_2 Pointer to the Vec2 structure (second column).
 */
void
mat22_init_by_vec2(Mat22_t* mat22, const Vec2_t *col_1, const Vec2_t *col_2);

/**
 * @brief Initialization of the matrix by data from the source matrix.
 * @param mat22 Pointer to Mat22 structure (destination).
 * @param mat22_src Pointer to Mat22 structure (source).
 */
void
mat22_init_by_mat22(Mat22_t* mat22, const Mat22_t* mat22_src);

/**
 * @brief Determination if matrix contains finite elements. 
 * @param mat22 Pointer to Mat22 structure.
 * @return b32 True - valid matrix. False - not valid matrix.
 */
b32
mat22_is_valid(Mat22_t *mat22);

/**
 * @brief Determination if matrix is a nearly zero matrix.
 * @param mat22 Pointer to Mat22 structure.
 * @return b32 Result of the check.
 */
b32
mat22_is_nearly_zero(Mat22_t *mat22);

/**
 * @brief Set all matrix elements to zeros.
 * @param mat22 Pointer to Mat22 structure.
 */
void
mat22_set_zero(Mat22_t* mat22);

/**
 * @brief Set matrix to the identity matrix.
 * @param mat22 Pointer to Mat22 structure.
 */
void
mat22_set_identity(Mat22_t *mat22);

/**
 * @brief Make invesion of the matrix.
 * @param mat22 Pointer to Mat22 structure.
 */
void
mat22_make_inverse(Mat22_t *mat22);

/**
 * @brief Make transposition of the matrix.
 * @param mat22 Pointer to Mat22 structure.
 */
void
mat22_make_transpose(Mat22_t *mat22);














#endif // MATRIX22_H_
