/**
 * ================================================================================
 * @file include_engine/render.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions related to the software rendering.
 * @version 0.2
 * @date 2022-12-10
 * ================================================================================ 
 */

#ifndef JEMA_ENGINE_RENDER_H_
#define JEMA_ENGINE_RENDER_H_

#include <windows.h>
#include "include_engine/utils.h"

typedef struct Color_ Color;
typedef struct Game_ Game;
typedef struct Image_ Image;
typedef struct Vec2_ Vec2;

/**
 * @brief Structure for the render buffer.
 */
struct RenderBuffer_
{
    u32 width;  /**< Allocated buffer width. */
    u32 height;  /**< Allocated buffer height. */
    void *bitmap_memory;  /**< Pointer to the allocated bitmap memory. */
};
typedef struct RenderBuffer_ RenderBuffer;

/**
 * @brief Enumerator indicating side flat triangles.
 */
enum FlatSideFlag_
{
    SF_BOTTOM_FLAT,  /**< Flag for the bottom flat triangles. */
    SF_TOP_FLAT  /**< Flag for the top flat triangles. */
};
typedef enum FlatSideFlag_ FlatSideFlag;

/**
 * @brief Structure holding data necessary for drawing a filled triangles.
 */
struct TriangleData_
{
    u32 LSX_array[WINDOW_WIDTH];  /**< Left Side X array. */
    u32 RSX_array[WINDOW_WIDTH];  /**< Rigth Side X array. */
    u32 LSX_i;  /**< Left Side X array index. */
    u32 RSX_i;  /**< Right Side X array index. */
};
typedef struct TriangleData_ TriangleData;

/**
 * @brief Structure for the Render properties.
 */
struct Render_
{
    u32 width;  /**< Current width of the graphic window.  */
    u32 height;  /**< Current height of the graphic window. */
    RenderBuffer *buffer;  /**< Pointer to the render buffer. */
    HWND window;  /**< Handle to the window. */
    HDC hdc;  /**< Handle to the drawing context. */ 
    BITMAPINFO bitmap_info;  /**< Windows specific bitmapinfo structure. */
    TriangleData *triangle_data; /**< Pointer to the triangle fill data. */
};
typedef struct Render_ Render;

/**
 * @brief Object constructor.
 * @return Render* Pointer to the Render structure.
 */
Render*
Render_Constructor(void);

/**
 * @brief Object destructor.
 * @param render Pointer to the Render structure.
 * @return Render* Pointer to the Render structure.
 */
Render*
Render_Destructor(Render *render);

/**
 * @brief Object initialization.
 * @param render Poiner to the Render structure.
 * @param window Handle to the game window.
 */
void
Render_Init(Render *render, HWND window);

/**
 * @brief Processing the resizing of the game window.
 * @param render Poiner to the Render structure.
 */
void 
Render_ResizeWindow(Render *render);

/**
 * @brief Updating the window by stretching DI bits 
 * @param render Poiner to the Render structure.
 */
void 
Render_UpdateWindow(Render *render);

/**
 * @brief Clearing the full screen with a background color.
 * @param render Pointer to the render structure.
 * @param color Pointer to the color structure.
*/
void 
Render_ClearScreen(Render *render, const Color *color);

/**
 * @brief Drawing a pixel on the screen with specific color. 
 * @param render Pointer to the render structure.
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param color Pointer to the color structure (background color).
*/
void 
Render_SetPixelColor(Render *render, u32 x, u32 y, const Color *color);

/**
 * @brief Getting color of a pixel with specified coordinates. 
 * @param render Pointer to the render structure.
 * @param x X coordinate of the pixel.
 * @param y Y coordinate of the pixel.
 * @param color Structure of the color that will be updated.
 */
void 
Render_GetPixelColor(Render *render, u32 x, u32 y, Color *color);

/**
 * @brief Drawing a pixelized line from one point to another.
 * @param render Pointer to the render structure.
 * @param x0 X coordinate of the beginning point of the line. 
 * @param y0 Y coordinate of the beginning point of the line.
 * @param x1 X coordinate of the ending point of the line. 
 * @param y1 Y coordinate of the ending point of the line.
 * @param color Pointer to the color structure.
*/
void 
Render_DrawLine(Render *render, f32 x0, f32 y0, f32 x1, f32 y1, const Color *color);

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
Render_DrawHorLine(Render *render, u32 x0, u32 y0, u32 length, u32 width, const Color *color);

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
Render_DrawVerLine(Render *render, u32 x0, u32 y0, u32 length, u32 width, const Color *color);

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
Render_DrawRect(Render *render, u32 x0, u32 y0, u32 width, u32 height, const Color *color);

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
Render_DrawRectWithBrd(Render *render, u32 x0, u32 y0, u32 width, u32 height, u32 brd_width, 
    const Color *color, const Color *brd_color);

/**
 * @brief Drawing a filled arbitrary shape triangle.
 * @param render Pointer to the render structure.
 * @param v1 First vertex of the triangle.
 * @param v2 Second vertex of the triangle.
 * @param v3 Third vertex of the triangle.
 * @param color Pointer to the color structure (background color).
 */
void 
Render_DrawFillTriangle(Render *render, Vec2 v1, Vec2 v2, Vec2 v3, const Color *color);

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
Render_DrawRotatedRect(Render *render, f32 x0, f32 y0, f32 width, f32 height, f32 angle, 
    const Color *color);

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
Render_DrawCircle(Render *render, f32 x0, f32 y0, f32 radius, b32 is_filled, 
    const Color *color);

/**
 * @brief Drawing the loaded png image.
 * @param render Pointer to the render structure.
 * @param x X coordinate of the bottom-left corner of the image (BL).
 * @param y Y coordinate of the bottom-left corner of the image (BL).
 * @param image Pointer to the image to be drawn. 
 * @param scale Scale of the image.
 */
void 
Render_DrawBitmap(Render *render, u32 x, u32 y, Image *image, u32 scale);

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
Render_DrawBitmapByMask(Render *render, u32 x, u32 y, const Image *mask, const Color *color, 
    u32 scale);

#endif  /* JEMA_ENGINE_RENDER_H_ */
