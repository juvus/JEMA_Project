/**
 * ================================================================================
 * @file color.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with color.
 * @version 0.1
 * @date 2022-05-29
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

#ifndef COLOR_H_
#define COLOR_H_

/* Standard includes: */

/* Program includes: */
#include <utils.h>

/**
 * @brief Structure for the color different representation in ARGB format. This is
 * main color format used in the entire engine.
 */
struct Color_ARGB
{
    union
    {
        u32 color; /**< 32 bit color (ARGB). */
        u8 components[4];  /**< Array of 8 bit color components. */
        struct
        {
            u8 alpha;  /**< Alpha channel of the color. */
            u8 red;  /**< Red component of the color. */
            u8 green;  /**< Green component of the color. */
            u8 blue;  /**< Blue component of the color. */
        };
    };
};
typedef struct Color_ARGB Color_ARGB_t;

/**
 * @brief Constructor of the Color_ARGB class.
 * @return Color_ARGB_t* Pointer to the Color_ARGB structure.
 */
Color_ARGB_t*
color_argb_constructor(void);

/**
 * @brief Deconstructor of the Color_ARGB class.
 * @param color_argb Pointer to the Color_ARGB structure.
 */
void
color_argb_deconstructor(Color_ARGB_t *color_argb);

/**
 * @brief Setting color from the u32 integer representing a color in RGBA format.
 * @param color_argb Pointer to the Color_ARGB structure.
 * @param color_rgba Color in the RGBA format as u32 integer.
 */
void
color_argb_set_from_u32_rgba(Color_ARGB_t *color_argb, u32 color_rgba);

/**
 * @brief Setting color from the u32 integer representing a color in ARGB format.
 * @param color_argb Pointer to the Color_ARGB structure.
 * @param color_argb Color in the RGBA format as u32 integer.
 */
void
color_argb_set_from_u32_argb(Color_ARGB_t *color_argb, u32 color_argb);

/**
 * @brief Setting color from the set of u8 integers representing color components.
 * @param color_argb Pointer to the Color_ARGB structure.
 * @param alpha Color alpha channel.
 * @param red Color red channel.
 * @param green Color green channel.
 * @param blue Color blue channel.
 */
void
color_argb_set_from_comp(Color_ARGB_t *color_argb, u8 alpha, u8 red, u8 green, u8 blue);

/**
 * @brief Setting gray color determined by value (0 - 255).
 * @param color_argb Pointer to the Color_ARGB structure.
 * @param value Value of the gray color gradation. 0 - black, 255 - white.
 */
void
color_argb_set_gray(Color_ARGB_t *color_argb, u8 value);

/**
 * @brief Making the blended color using linear interpolation algorithm.
 * @param color_argb_result Pointer to the result Color_ARGB structure.
 * @param color_argb_1 Pointer to the color 1 Color_ARGB structure.
 * @param color_argb_2 Pointer to the color 2 Color_ARGB structure.
 */
void
color_argb_make_alpha_blending(Color_ARGB_t *color_argb_result, Color_ARGB_t *color_argb_1,
    Color_ARGB_t *color_argb_2);

#endif /* COLOR_H_ */
