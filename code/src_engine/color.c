/**
 * ================================================================================
 * @file src_engine/color.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with color.
 * @version 0.2
 * @date 2022-11-29
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_engine/color.h"

#include "include_engine/dbg.h"
#include "include_engine/utils.h"

Color*
Color_Constructor(void)
{
    Color *color = (Color *)malloc(1 * sizeof(Color));
    if (color == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return color;
}

void
Color_Destructor(Color *color)
{
    if (color == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    free(color);
    color = NULL;
}

void
Color_SetFromImageColorData(Color *color, u32 image_color_data)
{
    color->alpha = (u8)((image_color_data & 0xff000000) >> 24);  /* Color alpha component. */
    color->blue = (u8)((image_color_data & 0x00ff0000) >> 16);  /* Color blue component. */
    color->green = (u8)((image_color_data & 0x0000ff00) >> 8);  /* Color green component. */
    color->red = (u8)((image_color_data & 0x000000ff) >> 0);  /* Color red component. */
}

void
ColorSetFromComp(Color *color, u8 alpha, u8 red, u8 green, u8 blue)
{
    color->alpha = alpha;
    color->red = red;
    color->green = green;
    color->blue = blue;
}

void
Color_SetGray(Color *color, u8 alpha, u8 value)
{
    ColorSetFromComp(color, alpha, value, value, value);
}

void
Color_MakeAlphaBlending(Color *color_res, Color *color_bkg, Color *color_frg, f32 alpha)
{
    u8 R_res, G_res, B_res;  /* Result color components. */

    u8 R_bkg = color_bkg->red;
    u8 G_bkg = color_bkg->green;
    u8 B_bkg = color_bkg->blue;
    u8 R_frg = color_frg->red;
    u8 G_frg = color_frg->green;
    u8 B_frg = color_frg->blue;
       
    /* Determine the output color components (after mixing). */
    if (R_bkg >= R_frg)
    {
        f32 delta = (f32)(R_bkg - R_frg);
        R_res = R_frg + (u8)(delta * (1.0 - alpha));
    } 
    else
    {
        f32 delta = (f32)(R_frg - R_bkg);
        R_res = R_bkg + (u8)(delta * alpha); 
    }

    if (G_bkg >= G_frg)
    {
        f32 delta = (f32)(G_bkg - G_frg);
        G_res = G_frg + (u8)(delta * (1.0 - alpha));
    }
    else
    {
        f32 delta = (f32)(G_frg - G_bkg);
        G_res = G_bkg + (u8)(delta * alpha); 
    }

    if (B_bkg >= B_frg)
    {
        f32 delta = (f32)(B_bkg - B_frg);
        B_res = B_frg + (u8)(delta * (1.0 - alpha));
    }
    else
    {
        f32 delta = (f32)(B_frg - B_bkg);
        B_res = B_bkg + (u8)(delta * alpha); 
    }

    /* Return the result color. */
    color_res->alpha = 0xff;
    color_res->red = R_res;
    color_res->green = G_res;
    color_res->blue = B_res;
}