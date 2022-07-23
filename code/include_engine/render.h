/**
 * ================================================================================
 * @file render.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions related to the software rendering.
 * @version 0.1
 * @date 2021-12-02
 * ================================================================================ 
 */

#ifndef RENDER_H_
#define RENDER_H_

/* Standard library includes: */
#include <windows.h>

/* Game engine includes: */
#include <utils.h>
#include <image.h>

/* Predefined structures: */
typedef struct Color Color_t;
typedef struct Game Game_t;

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
struct Triangle_Data
{
    u32 LSX_array[WINDOW_WIDTH];  /**< Left Side X array. */
    u32 RSX_array[WINDOW_WIDTH];  /**< Rigth Side X array. */
    u32 LSX_i;  /**< Left Side X array index. */
    u32 RSX_i;  /**< Right Side X array index. */
};
typedef struct Triangle_Data Triangle_Data_t;

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
    Triangle_Data_t *triangle_data; /**< Pointer to the triangle fill data. */
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
 * @brief Processing the resizing of the game window.
 * @param render Poiner to the Render structure.
 * @param game Pointer to the Game structure.
 */
void 
render_resize_window(Render_t *render);

/**
 * @brief Updating the window by stretching DI bits 
 * @param render Poiner to the Render structure.
 */
void 
render_update_window(Render_t *render);

/**
 * @brief Clearing the full screen with a background color.
 * @param render Pointer to the render structure.
 * @param color Pointer to the color structure.
 * @return void.
*/
void 
render_clear_screen(Render_t *render, Color_t *color);

/**
 * @brief Drawing a pixel on the screen with specific color. 
 * @param render Pointer to the render structure.
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param color Pointer to the color structure (background color).
 * @return void.
*/
void 
render_set_pixel_color(Render_t *render, u32 x, u32 y, Color_t * color);

/**
 * @brief Getting color of a pixel with specified coordinates. 
 * @param render Pointer to the render structure.
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param color Structure of the color that will be updated.
 * @return void.
 */
void 
render_get_pixel_color(Render_t *render, u32 x, u32 y, Color_t *color);

/**
 * @brief Drawing a pixelized line from one point to another.
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the beginning point of the line. 
 * @param y0 Y coordinate of the beginning point of the line.
 * @param x1 X coordinate of the ending point of the line. 
 * @param y1 Y coordinate of the ending point of the line.
 * @param color Pointer to the color structure.
 * @return void.
*/
void 
render_draw_line(Render_t *render, u32 x0, u32 y0, u32 x1, u32 y1, Color_t *color);

/**
 * @brief Drawing a pixelized horizontal line from one point to another.
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the beginning point of the line.
 * @param y0 Y coordinate of the beginning point of the line.
 * @param length Length of the line.
 * @param width Width of the line.
 * @param color Pointer to the color structure.
 */
void 
render_draw_hor_line(Render_t *render, u32 x0, u32 y0, u32 length, u32 width, Color_t *color);

/**
 * @brief Drawing a pixelized vertical line from one point to another.
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the beginning point of the line.
 * @param y0 Y coordinate of the beginning point of the line.
 * @param length Length of the line.
 * @param width Width of the line.
 * @param color Pointer to the color structure.
 */
void 
render_draw_ver_line(Render_t *render, u32 x0, u32 y0, u32 length, u32 width, Color_t *color);

/**
 * @brief Drawing a simple rectangle. 
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the bottom-left corner (BL).
 * @param y0 Y coordinate of the bottom-left corner (BL).
 * @param width Width of the rectangle along the Ox axis.
 * @param height Height of the rectangle along the Oy axis.
 * @param color Pointer to the color structure.
 */
void 
render_draw_rect(Render_t *render, u32 x0, u32 y0, u32 width, u32 height, Color_t *color);

/**
 * @brief Drawing a rectangle with specified lines boarder.
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the bottom-left corner (BL).
 * @param y0 Y coordinate of the bottom-left corner (BL).
 * @param width Width of the rectangle along the Ox axis.
 * @param height Height of the rectangle along the Oy axis.
 * @param brd_width Width of the rectangle boarder.
 * @param color Pointer to the color structure (background color).
 * @param brd_color Pointer to the color structure (borders color).
 */
void 
render_draw_rect_with_brd(Render_t *render, u32 x0, u32 y0, u32 width, u32 height, u32 brd_width, 
    Color_t *color, Color_t *brd_color);

/**
 * @brief Drawing a filled arbitrary shape triangle.
 * @param render Pointer to the render structure.
 * @param v1 First vertix of the triangle.
 * @param v2 Second vertix of the triangle.
 * @param v3 Third vertix of the triangle.
 * @param color Pointer to the color structure (background color).
 */
void 
render_draw_fill_triangle(Render_t *render, V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, 
    Color_t *color);

/**
 * @brief Drawing a rectangle rotated by specified angle degrees. 
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the bottom-left corner (BL).
 * @param y0 Y coordinate of the bottom-left corner (BL).
 * @param width Width of the rectangle along the Ox axis.
 * @param height Height of the rectangle along the Oy axis.
 * @param angle Angle (in degrres) of the rotation around the rectangle center.
 * @param color Pointer to the color structure (rectangle color).
 */
void 
render_draw_rotated_rect(Render_t *render, u32 x0, u32 y0, u32 width, u32 height, f32 angle, 
    Color_t *color);

/**
 * @brief Drawing a circle using Brezenhame algorithm.
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the circle center.
 * @param y0 Y coordinate of the circle center.
 * @param radius Radius of the circle. 
 * @param is_filled Flag indicating the fill of the circle.
 * @param color Pointer to the color structure (circle color).
 */
void 
render_draw_circle(Render_t *render, u32 x0, u32 y0, u32 radius, b32 is_filled, Color_t *color);

/**
 * @brief Drawing the loaded png image.
 * @param render Pointer to the render structure.
 * @param x X coordinate of the bottom-left corner of the image (BL).
 * @param y Y coordinate of the bottom-left corner of the image (BL).
 * @param image Pointer to the image to be drawn. 
 * @param scale Scale of the image.
 */
void 
render_draw_bitmap(Render_t *render, u32 x, u32 y, Image_t* image, u32 scale);

/**
 * @brief Drawing an image using one specified color and a mask as an image object.  
 * @param render Pointer to the render structure.
 * @param x X coordinate of the bottom-left corner of the image (BL).
 * @param y Y coordinate of the bottom-left corner of the image (BL).
 * @param mask Pointer to the image (mask) to be drawn with a color.  
 * @param color Pointer to the color structure.
 * @param scale Scale of the image.
 */
void 
render_draw_bitmap_by_mask(Render_t *render, u32 x, u32 y, Image_t *mask, Color_t *color, 
    u32 scale);

#endif /* RENDER_H_ */
