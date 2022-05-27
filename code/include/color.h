/**
 * ================================================================================
 * @file color.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of functions necessary for work with color.
 * @version 0.1
 * @date 2022-05-28
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

#ifndef COLOR_H_
#define COLOR_H_

/* Standard includes: */

/* Program includes: */
#include <utils.h>

/**
 * @brief Structure for the color different representation in ARGB format.
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
 * @brief Structure for the color different representation in RGBA format.
 */
struct Color_RGBA
{
    union
    {
        u32 color; /**< 32 bit color (RGBA). */
        u8 components[4];  /**< Array of 8 bit color components. */
        struct
        {
            u8 red;  /**< Red component of the color. */
            u8 green;  /**< Green component of the color. */
            u8 blue;  /**< Blue component of the color. */
            u8 alpha;  /**< Alpha channel of the color. */
        };
    };
};
typedef struct Color_RGBA Color_RGBA_t;



/* TODO: Use color object everywhere!!! */




/* Function to convert RGBA color to the ARGB color */



u32
convert_RGBA_to_ARGB(u32 color);

/* Function to get the alpha of the color */
u8 get_color_alpha(u32 color);

/* Function for setting the alpha channel to the color */
void set_color_alpha(u32 *color, u8 alpha);

/* Function to get the red channel of the color */
u8 get_color_red(u32 color);

/* Function for setting the red channel to the color */
void set_color_red(u32 *color, u8 red);

/* Function to get the green channel of the color */
u8 get_color_green(u32 color);

/* Function for setting the green channel to the color */
void set_color_green(u32 *color, u8 green);

/* Function to get the blue channel of the color */
u8 get_color_blue(u32 color);

/* Function for setting the blue channel to the color */
void set_color_blue(u32 *color, u8 blue);

/* Function to set the color in ARGB format */
void set_color(u32 *color, u8 alpha, u8 red, u8 green, u8 blue);

/* Function to get the transparent color (linear interpolation algorithm) */
u32 get_alpha_blending_color(u32 color_1, u32 color_2, f32 alpha);

/* Function to get the gray color */
u32 get_gray_color(u8 value);

#endif /* COLOR_H_ */
