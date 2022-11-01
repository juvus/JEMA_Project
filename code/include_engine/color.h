/**
 * ================================================================================
 * @file color.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with color.
 * @version 0.1
 * @date 2022-07-05
 * ================================================================================
 */

#ifndef COLOR_H_
#define COLOR_H_

/* Game engine includes: */
#include "utils.h"

/**
 * @brief Structure for the color different representation in ARGB format. This is
 * main color format used in the entire engine.
 */
struct Color
{
    union
    {
        u32 color; /**< 32 bit color (ARGB). */
        u8 components[4];  /**< Array of 8 bit color components. */
        struct
        {
            u8 blue;  /**< Blue component of the color. */
            u8 green;  /**< Green component of the color. */
            u8 red;  /**< Red component of the color. */
            u8 alpha;  /**< Alpha channel of the color. */
        };
    };
};
typedef struct Color Color_t;

/**
 * @brief Class constructor.
 * @return Color_t* Pointer to the Color structure.
 */
Color_t*
color_constructor(void);

/**
 * @brief Class destructor.
 * @param color Pointer to the Color structure.
 */
void
color_destructor(Color_t *color);

/**
 * @brief Setting color from the u32 image color data.
 * @param color Pointer to the Color structure.
 * @param image_color_data Color of the pixel taken from the loaded image.
 */
void
color_set_from_image_color_data(Color_t *color, u32 image_color_data);

/**
 * @brief Setting color from the set of u8 integers representing color components.
 * @param color Pointer to the Color structure.
 * @param alpha Color alpha channel.
 * @param red Color red channel.
 * @param green Color green channel.
 * @param blue Color blue channel.
 */
void
color_set_from_comp(Color_t *color, u8 alpha, u8 red, u8 green, u8 blue);

/**
 * @brief Setting gray color determined by value (0 - 255).
 * @param color Pointer to the Color structure.
 * @param alpha Color alpha channel.
 * @param value Value of the gray color gradation. 0 - black, 255 - white.
 */
void
color_set_gray(Color_t *color, u8 alpha, u8 value);

/**
 * @brief Making the blended color using linear interpolation algorithm.
 * @param color_res Pointer to the result Color structure.
 * @param color_bkg Pointer to the background color Color structure.
 * @param color_frg Pointer to the foreground color Color structure.
 * @param alpha Level of the foreground alpha (0.0 - 1.0). 0.0 - bkg color, 1.0 - frg color.
 */
void
color_make_alpha_blending(Color_t *color_res, Color_t *color_bkg, Color_t *color_frg, 
    f32 alpha);

#endif /* COLOR_H_ */
