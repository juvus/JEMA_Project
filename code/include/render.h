/**
 * ================================================================================
 * @file render.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions related to the software rendering.
 * @version 0.1
 * @date 2021-12-02
 * @copyright JuvuSoft (c) 2021
 * ================================================================================ 
 */

#ifndef RENDER_H_
#define RENDER_H_

/* Standard includes: */
#include <windows.h>

/* Program includes: */
#include <utils.h>
#include <image.h>

/**
 * @brief Structure for the render buffer.
 */
struct Render_Buffer
{
    u32 width;  /**< Allocated buffer width. */
    u32 height;  /**< Allocated buffer height. */
    void *bitmap_memory;  /**< Pointer to the allocated bitmap memory. */
};
typedef struct Render_Buffer Render_Buffer_t;

/**
 * @brief Union for packing and unpacking color double word (32 bits).
 */
union UColor
{
    u32 color;  /**< 32 bit color (ARGB). */
    u8 components[4];  /**< Array of 8 bit color components. */
};
typedef union UColor UColor_t;

/**
 * @brief Enumerator indicating side flat triangles.
 */
enum Flat_Side_Flag
{
    SF_BOTTOM_FLAT,  /**< Flag for the bottom flat triangles. */
    SF_TOP_FLAT  /**< Flag for the top flat triangles. */
};
typedef enum Flat_Side_Flag Flat_Side_Flag_t;

/**
 * @brief Structure holding data necessary for drawing a filled triangles.
 */
struct Triang_Data
{
    u32 LSX_array[WINDOW_WIDTH];  /**< Left Side X array. */
    u32 RSX_array[WINDOW_WIDTH];  /**< Rigth Side X array. */
    u32 LSX_i;  /**< Left Side X array index. */
    u32 RSX_i;  /**< Right Side X array index. */
};
typedef struct Triang_Data Triang_Data_t;

/**
 * @brief Structure for the Render properties.
 */
struct Render
{
    u32 width;  /**< Current width of the graphic window.  */
    u32 height;  /**< Current height of the graphic window. */
    Render_Buffer_t *buffer;  /**< Pointer to the render buffer. */
    HWND window;  /**< Handle to the window. */
    HDC hdc;  /**< Handle to the drawing context. */ 
    BITMAPINFO bitmap_info;  /**< Windows specific bitmapinfo structure. */
    Triang_Data_t *triang_data; /**< Pointer to the triangle fill data.  */ 
};
typedef struct Render Render_t;

/**
 * @brief Constructor of the Render class.
 * @return Render_t* Pointer to the Render structure. 
 */
Render_t*
render_constructor(void);

/**
 * @brief Destructor of the Render object.
 * @param render Pointer to the Render structure.
 */
void
render_destructor(Render_t *render);

/**
 * @brief Initialization of the Render object.
 * @param render Poiner to the Render structure.
 * @param window Handle to the game window.
 */
void
render_init(Render_t *render, HWND window);

/**
 * @brief Clearing the full screen with a background color.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 * @return void.
*/
void 
render_clear_screen(u32 color, Render_t *render);

/**
 * @brief Drawing a pixel on the screen with specific color. 
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 * @return void.
*/
void 
render_put_pixel(u32 x, u32 y, u32 color, Render_t *render);

/**
 * @brief Getting color of a pixel with specified coordinates. 
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param render Pointer to the render structure.
 * @return A 32 bit color (ARGB). 
 */
u32 
render_get_pixel_color(u32 x, u32 y, Render_t *render);

/**
 * @brief Drawing a pixelized line from one point to another.
 * @param x0 X coordinate of the beginning point of the line. 
 * @param y0 Y coordinate of the beginning point of the line.
 * @param x1 X coordinate of the ending point of the line. 
 * @param y1 Y coordinate of the ending point of the line.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 * @return void.
*/
void 
render_draw_line(u32 x0, u32 y0, u32 x1, u32 y1, u32 color, Render_t *render);

/**
 * @brief Drawing a pixelized horizontal line from one point to another.
 * @param x0 X coordinate of the beginning point of the line.
 * @param y0 Y coordinate of the beginning point of the line.
 * @param length Length of the line.
 * @param width Width of the line.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
void 
render_draw_hor_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_t *render);

/**
 * @brief Drawing a pixelized vertical line from one point to another.
 * @param x0 X coordinate of the beginning point of the line.
 * @param y0 Y coordinate of the beginning point of the line.
 * @param length Length of the line.
 * @param width Width of the line.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
void 
render_draw_ver_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_t *render);

/**
 * @brief Drawing a simple rectangle. 
 * @param x0 X coordinate of the bottom-left corner (BL).
 * @param y0 Y coordinate of the bottom-left corner (BL).
 * @param width Width of the rectangle along the Ox axis.
 * @param height Height of the rectangle along the Oy axis.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
void 
render_draw_rect(u32 x0, u32 y0, u32 width, u32 height, u32 color, Render_t *render);

/**
 * @brief Drawing a rectangle with specified lines boarder.
 * @param x0 X coordinate of the bottom-left corner (BL).
 * @param y0 Y coordinate of the bottom-left corner (BL).
 * @param width Width of the rectangle along the Ox axis.
 * @param height Height of the rectangle along the Oy axis.
 * @param brd_width Width of the rectangle boarder.
 * @param color A 32 bit color (ARGB).
 * @param brd_color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
void 
render_draw_rect_with_brd(u32 x0, u32 y0, u32 width, u32 height, u32 brd_width, u32 color, 
    u32 brd_color, Render_t *render);

/**
 * @brief Drawing a filled arbitrary shape triangle.
 * @param v1 First vertix of the triangle.
 * @param v2 Second vertix of the triangle.
 * @param v3 Third vertix of the triangle.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
void 
render_draw_fill_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color, Render_t *render);

/**
 * @brief Drawing a rectangle rotated by specified angle degrees. 
 * @param x0 X coordinate of the bottom-left corner (BL).
 * @param y0 Y coordinate of the bottom-left corner (BL).
 * @param width Width of the rectangle along the Ox axis.
 * @param height Height of the rectangle along the Oy axis.
 * @param angle Angle (in degrres) of the rotation around the rectangle center.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
void 
render_draw_rotated_rect(u32 x0, u32 y0, u32 width, u32 height, f32 angle, u32 color, Render_t *render);

/**
 * @brief Drawing a circle using Brezenhame algorithm.
 * @param x0 X coordinate of the circle center.
 * @param y0 Y coordinate of the circle center.
 * @param radius Radius of the circle. 
 * @param is_filled Flag indicating the fill of the circle.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
void 
render_draw_circle(u32 x0, u32 y0, u32 radius, b32 is_filled ,u32 color, Render_t *render);

/**
 * @brief Drawing the loaded png image.
 * @param x X coordinate of the bottom-left corner of the image (BL).
 * @param y Y coordinate of the bottom-left corner of the image (BL).
 * @param image Pointer to the image to be drawn. 
 * @param scale Scale of the image.
 * @param render Pointer to the render structure.
 */
void 
render_draw_bitmap(u32 x, u32 y, Image_t* image, u32 scale, Render_t *render);

/* Function for rendering the colored object determined by mask */

/**
 * @brief Drawing an image using one specified color and a mask as an image object.  
 * @param x X coordinate of the bottom-left corner of the image (BL).
 * @param y Y coordinate of the bottom-left corner of the image (BL).
 * @param mask Pointer to the image (mask) to be drawn with a color.  
 * @param color A 32 bit color (ARGB).
 * @param scale Scale of the image.
 * @param render Pointer to the render structure.
 */
void 
render_draw_bitmap_by_mask(u32 x, u32 y, Image_t *mask, u32 color, u32 scale, Render_t *render);






/* Function to convert RGBA color to the ARGB color */
u32 convert_RGBA_to_ARGB(u32 color);

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

#endif /* RENDER_H_ */
