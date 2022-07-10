/**
 * ================================================================================
 * @file color.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with color.
 * @version 0.1
 * @date 2022-07-05
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard includes: */

/* Program includes: */
#include <color.h>
#include <utils.h>

Color_t*
color_constructor(void)
{
    Color_t* color = NULL;  /* Pointer to the Color structure. */
    
    /* Allocate memory for the Color structure. */
    color = (Color_t*)malloc(1 * sizeof(Color_t));
    return color;
}

void
color_destructor(Color_t *color)
{
    /* Release memory allocated for the Color structure. */
    free(color);
}

void
color_set_from_u32_rgba(Color_t *color, u32 color_rgba_u32)
{
    /* Get components from the rgba color and set to Color structure. */
    color->red = (u8)((color_rgba_u32 & 0xff000000) >> 24);  /* Color red component. */
    color->green = (u8)((color_rgba_u32 & 0x00ff0000) >> 16);  /* Color green component. */
    color->blue = (u8)((color_rgba_u32 & 0x0000ff00) >> 8);  /* Color blue component. */
    color->alpha = (u8)((color_rgba_u32 & 0x000000ff) >> 0);  /* Color alpha component. */
}

void
color_set_from_comp(Color_t *color, u8 alpha, u8 red, u8 green, u8 blue)
{
    /* Set color by components. */
    color->alpha = alpha;
    color->red = red;
    color->green = green;
    color->blue = blue;
}

void
color_set_gray(Color_t *color, u8 alpha, u8 value)
{
    color_set_from_comp(color, alpha, value, value, value);
}

void
color_make_alpha_blending(Color_t *color_res, Color_t *color_bkg, Color_t *color_frg, f32 alpha)
{
    u8 R_bkg, G_bkg, B_bkg;  /* Background color components. */
    u8 R_frg, G_frg, B_frg;  /* Foreground color components. */
    u8 R_res, G_res, B_res;  /* Result color components. */
    f32 delta;  /* Delta for the color component. */

    R_bkg = color_bkg->red;
    G_bkg = color_bkg->green;
    B_bkg = color_bkg->blue;
    R_frg = color_frg->red;
    G_frg = color_frg->green;
    B_frg = color_frg->blue;
       
    /* Determine the output color components (after mixing). */
    if (R_bkg >= R_frg)
    {
        delta = (f32)(R_bkg - R_frg);
        R_res = R_frg + (u8)(delta * (1.0 - alpha));
    } 
    else
    {
        delta = (f32)(R_frg - R_bkg);
        R_res = R_bkg + (u8)(delta * alpha); 
    }

    if (G_bkg >= G_frg)
    {
        delta = (f32)(G_bkg - G_frg);
        G_res = G_frg + (u8)(delta * (1.0 - alpha));
    }
    else
    {
        delta = (f32)(G_frg - G_bkg);
        G_res = G_bkg + (u8)(delta * alpha); 
    }

    if (B_bkg >= B_frg)
    {
        delta = (f32)(B_bkg - B_frg);
        B_res = B_frg + (u8)(delta * (1.0 - alpha));
    }
    else
    {
        delta = (f32)(B_frg - B_bkg);
        B_res = B_bkg + (u8)(delta * alpha); 
    }

    /* Return the result color. */
    color_res->alpha = 0xff;
    color_res->red = R_res;
    color_res->green = G_res;
    color_res->blue = B_res;
}