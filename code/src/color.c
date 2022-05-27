/**
 * ================================================================================
 * @file color.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for work with color.
 * @version 0.1
 * @date 2022-05-28
 * @copyright Dmitry Safonov (c) 2022
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard includes: */

/* Program includes: */
#include <color.h>
#include <utils.h>

u32
color_convert_RGBA_to_ARGB(u32 rgba_color)
{
    /* Function to convert RGBA color to the ARGB color */

    u32 argb_color;
    u8 alpha, red, green, blue;
    Color_bytes_t read_color;

    read_color.color_word = rgba_color;
    alpha = read_color.a;
    red = read_color.r;
    green = read_color.g;
    blue = read_color.b;
    
    set_color(&argb_color, alpha, red, green, blue);
    
    return argb_color;
}


u32
get_gray_color(u8 value)
{
    /* Function to return 32bit gray color from 8bit value */
    u32 gray_color;
    gray_color = ((value << 0) | (value << 8) | (value << 16));
    return gray_color;
}


u8
get_color_alpha(u32 color)
{
    /* Function to get the alpha of the color */
    return (u8)((color & 0xff000000) >> 24);    
}


void
set_color_alpha(u32 *color, u8 alpha)
{
    /* Function to set the alpha of the color
       color - ARGB 32 bit color
       alpha - u8 (0 - 255) value of the new alpha channel */

    *color = *color | (0xff << 24); /* Set the alpha channel to 0xff */
    *color = *color ^ (0xff << 24); /* Set the alpha channel to 0x00 */
    *color = *color | (alpha << 24); /* Set the value of a new alpha channel */
}


u8
get_color_red(u32 color)
{
    /* Function to get the red channel of the color */
    return (u8)((color & 0x00ff0000) >> 16);
}


void
set_color_red(u32 *color, u8 red)
{
    /* Function to set the red channel of the color
       color - ARGB 32 bit color
       red - u8 (0 - 255) value of the new red channel */

    *color = *color | (0xff << 16); /* Set the R channel to 0xff */
    *color = *color ^ (0xff << 16); /* Set the R channel to 0x00 */
    *color = *color | (red << 16); /* Set the value of a new red channel */
}


u8
get_color_green(u32 color)
{
    /* Function to get the green channel of the color */
    return (u8)((color & 0x0000ff00) >> 8);
}


void
set_color_green(u32 *color, u8 green)
{
    /* Function to set the green channel of the color
       color - ARGB 32 bit color
       green - u8 (0 - 255) value of the new green channel */

    *color = *color | (0xff << 8); /* Set the R channel to 0xff */
    *color = *color ^ (0xff << 8); /* Set the R channel to 0x00 */
    *color = *color | (green << 8); /* Set the value of a new green channel */
}


u8
get_color_blue(u32 color)
{
    /* Function to get the blue channel of the color */
    return (u8)((color & 0x000000ff) >> 0);
}


void
set_color_blue(u32 *color, u8 blue)
{
    /* Function to set the blue channel of the color
       color - ARGB 32 bit color
       blue - u8 (0 - 255) value of the new blue channel */

    *color = *color | (0xff << 0); /* Set the R channel to 0xff */
    *color = *color ^ (0xff << 0); /* Set the R channel to 0x00 */
    *color = *color | (blue << 0); /* Set the value of a new blue channel */
}


void
set_color(u32 *color, u8 alpha, u8 red, u8 green, u8 blue)
{
    /* Function to set the color in ARGB format */
    set_color_alpha(color, alpha);
    set_color_red(color, red);
    set_color_green(color, green);
    set_color_blue(color, blue);   
}


u32
get_alpha_blending_color(u32 color_1, u32 color_2, f32 alpha)
{
    /* Function to get the transparent color (linear interpolation algorithm)
       color_1 - background color
       color_2 - object color 
       alpha - level of the object alpha (0.0 ... 1.0). 0.0 - color_1, 1.0 - clolor_2*/

    UColor_t temp_color_1;
    UColor_t temp_color_2;
    UColor_t return_color;
    u8 R_1, G_1, B_1, R_2, G_2, B_2, R_out, G_out, B_out;
    f32 delta;
    
    /* Determine the color components */
    temp_color_1.color = color_1;
    temp_color_2.color = color_2;
    R_1 = temp_color_1.components[2];
    G_1 = temp_color_1.components[1];
    B_1 = temp_color_1.components[0];
    R_2 = temp_color_2.components[2];
    G_2 = temp_color_2.components[1];
    B_2 = temp_color_2.components[0];
       
    /* Determine the output color components (after mixing) */
    if (R_1 >= R_2) {
        delta = (f32)(R_1 - R_2);
        R_out = R_2 + (u8)(delta * (1.0 - alpha));
    } else {
        delta = (f32)(R_2 - R_1);
        R_out = R_1 + (u8)(delta * alpha); 
    }

    if (G_1 >= G_2) {
        delta = (f32)(G_1 - G_2);
        G_out = G_2 + (u8)(delta * (1.0 - alpha));
    } else {
        delta = (f32)(G_2 - G_1);
        G_out = G_1 + (u8)(delta * alpha); 
    }

    if (B_1 >= B_2) {
        delta = (f32)(B_1 - B_2);
        B_out = B_2 + (u8)(delta * (1.0 - alpha));
    } else {
        delta = (f32)(B_2 - B_1);
        B_out = B_1 + (u8)(delta * alpha); 
    }

    /* Return in format ARGB */
    return_color.components[0] = B_out;
    return_color.components[1] = G_out;
    return_color.components[2] = R_out;
    return_color.components[3] = 0xff;

    return return_color.color;
}

