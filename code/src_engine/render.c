/**
 * ================================================================================
 * @file src_engine/render.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with the software 
 * rendering. 
 * @version 0.3
 * @date 2022-12-15
 * ================================================================================ 
 */

#define _USE_MATH_DEFINES

#include "include_engine/render.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "include_engine/color.h"
#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/image.h"
#include "include_engine/math_functions.h"
#include "include_engine/utils.h"
#include "include_engine/vector2.h"

#include "include_game/game.h"

/**
 * @brief Drawing a pixelized line from one point to another by using 
 * Brezenham algorithm. This version also record the X coordinates for 
 * further rasterization of filling of the triangle.
 * @param render Pointer to the render_buffer structure.
 * @param v0 Vector for the beginning point.
 * @param v1 Vector for the ending point.
 * @param color Pointer to the color structure (line color).
 * @param SX_array Left/Right Side X array.
 * @param SX_i Left/Right Side X array index.
 */
static void 
DrawLineExtended(Render *render, Vec2 v0, Vec2 v1, const Color *color, u32 *SX_array, 
    u32 *SX_i);

/**
 * @brief Drawing a filled bottom/top flat triangle.
 * @param render Pointer to the render structure.
 * @param v1 Top/Bottom most vertix of the triangle.
 * @param v2 Left most vertix of the triangle.
 * @param v3 Right most vertix of the triangle.
 * @param side_flag Flag inicating flat side.
 * @param color Pointer to the color structure (triangle color).
 */
static void 
DrawFillSideFlatTriangle(Render *render, Vec2 v1, Vec2 v2, Vec2 v3, FlatSideFlag side_flag,
    const Color *color);

/**
 * @brief Drawing an image in the extended mode. Method could be used for direct copy of the 
 * loaded bitmap into the render buffer as well as for copy of the image while changing all 
 * untransparent pixels with the specified color (is_mask flag should be true).
 * @param x X coordinate of the bottom-left corner of the image (BL).
 * @param y Y coordinate of the bottom-left corner of the image (BL).
 * @param image Pointer to the image (mask) to be drawn.
 * @param scale Scale of the image.
 * @param is_mask Flag indicating the mask.
 * @param mask_color A 32 bit color (ARGB) to be used along with the mask.
 * @param render Pointer to the render structure.
 */
static void
DrawBitmapExtended(Render *render, u32 x, u32 y, const Image *image, u32 scale, b32 is_mask, 
    const Color *mask_color);

Render*
Render_Constructor(void)
{
    RenderBuffer *buffer = (RenderBuffer *)malloc(1 * sizeof(RenderBuffer));
    TriangleData *triangle_data = (TriangleData *)malloc(1 * sizeof(TriangleData));
    Render *render = (Render *)malloc(1 * sizeof(Render));
    
    if ((buffer == NULL) || (triangle_data == NULL) || (render == NULL))
    {
        dbg_error("%s", "Memory allocation error!");
    }
    render->buffer = buffer;
    render->triangle_data = triangle_data;
    return render;
}

void
Render_Destructor(Render *render)
{
    if (render == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }

    VirtualFree(render->buffer->bitmap_memory, 0, MEM_RELEASE);
    ReleaseDC(render->window, render->hdc);
    free(render->buffer);
    free(render->triangle_data);
    free(render);
    render = NULL;
}

void
Render_Init(Render *render, HWND window)
{
    /* Get and save header to the window and its drawing context. */
    render->window = window;
    render->hdc = GetDC(window);

    /* Calculate width and height of the window (client area) for the buffer. */
    RECT rect;
    GetClientRect(window, &rect);
    u32 rect_width = (u32)(rect.right - rect.left);
    u32 rect_height = (u32)(rect.bottom - rect.top);
    render->buffer->width = rect_width;
    render->buffer->height = rect_height;
    render->width = rect_width;
    render->height = rect_height;

     /* Delete the bitmap memory if it was previously allocated. */
    if (render->buffer->bitmap_memory) 
    {
        VirtualFree(render->buffer->bitmap_memory, 0, MEM_RELEASE);
        render->buffer->bitmap_memory = NULL;
    }

    /* Allocate the bitmap memory. */
    SIZE_T mem_size = sizeof(u32) * rect_width * rect_height;
    render->buffer->bitmap_memory = VirtualAlloc(0, mem_size, MEM_COMMIT|MEM_RESERVE, 
        PAGE_READWRITE);

    /* Fill the bitmap_info structure. */
    render->bitmap_info.bmiHeader.biSize = sizeof(render->bitmap_info.bmiHeader);
    render->bitmap_info.bmiHeader.biWidth = rect_width;
    render->bitmap_info.bmiHeader.biHeight = rect_height;
    render->bitmap_info.bmiHeader.biPlanes = 1;
    render->bitmap_info.bmiHeader.biBitCount = 32;
    render->bitmap_info.bmiHeader.biCompression = BI_RGB;
}

void 
Render_ResizeWindow(Render *render)
{    
    RECT rect;
    GetClientRect(render->window, &rect);
    render->width = (u32)(rect.right - rect.left);
    render->height = (u32)(rect.bottom - rect.top);
}

void 
Render_UpdateWindow(Render *render)
{
    StretchDIBits(render->hdc, 0, 0, (int)render->width, (int)render->height, 0, 0,
    (int)render->buffer->width, (int)render->buffer->height, render->buffer->bitmap_memory,
    &render->bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}

void
Render_ClearScreen(Render *render, const Color *color)
{
    u32 pitch = render->buffer->width * 4;
    u32 width = render->buffer->width;
    u32 height = render->buffer->height;
    u8 *row = (u8*)render->buffer->bitmap_memory;

    /* Procedure to fill the pixeld in optimized way. */
    for (u32 y = 0; y < height; y++)
    {
        u32 *pixel = (u32*)row;
        for (u32 x = 0; x < width; x++)
        {
            *(pixel) = color->color;
            ++pixel;
        }
        row += pitch;
    }
}

void
Render_SetPixelColor(Render *render, u32 x, u32 y, const Color *color)
{    
    /* Set color of a pixel with desired coordinates. */
    u32 *pixel = (u32*)render->buffer->bitmap_memory;
    *(pixel + x + render->buffer->width * y) = color->color;
}

void
Render_GetPixelColor(Render *render, u32 x, u32 y, Color *color)
{  
    /* Get color of a pixel with desired coordinates. */
    u32 *pixel = (u32*)render->buffer->bitmap_memory; 
    color->color = *(pixel + x + render->buffer->width * y);
}

void
Render_DrawLine(Render *render, f32 x0, f32 y0, f32 x1, f32 y1, const Color *color)
{
    /* TODO: add posibility to draw a line with the desired width. */
    Vec2 v0;  /* Vector for the beginning point. */
    v0.x = x0;
    v0.y = y0;

    Vec2 v1;  /* Vector for the ending point. */
    v1.x = x1;
    v1.y = y1;

    u32 *LSX_array = render->triangle_data->LSX_array;
    u32 *pLSX_i = &(render->triangle_data->LSX_i);
    DrawLineExtended(render, v0, v1, color, LSX_array, pLSX_i);
}

void
Render_DrawHorLine(Render *render, u32 x0, u32 y0, u32 length, u32 width, const Color *color)
{
    u32 *pixel = (u32*)render->buffer->bitmap_memory;
    for (u32 x = x0; x < (x0 + length); ++x)
    {
        for (u32 y = y0; y < (y0 + width); ++y)
        {
            *(pixel + x + render->buffer->width * y) = color->color;
        }
    }
}

void
Render_DrawVerLine(Render *render, u32 x0, u32 y0, u32 length, u32 width, const Color *color)
{   
    u32 *pixel = (u32*)render->buffer->bitmap_memory;
    for (u32 y = y0; y < (y0 + length); ++y)
    {
        for (u32 x = x0; x < (x0 + width); ++x)
        {
            *(pixel + x + render->buffer->width * y) = color->color;
        }
    }
}

static void
DrawLineExtended(Render *render, Vec2 v0, Vec2 v1, const Color *color, u32 *SX_array, 
    u32 *SX_i)
{
    /* TODO: Изменить алгоритм так, чтобы он рисовал линии только в 1 или 4-й четверти
    т.е линия нарисованная с одной точки до друго будет одинаковая. Если линия рисуется не
    слева направо, то просто поменять координаты местами и нарисовать слева направо.
    Таким образом не будет глитчей, когда линия будет рисоваться задом наперёд. И 
    линии будут полностью совпадать. */

    /* Deterime increments. */
    s32 inc_x;
    s32 dx = (s32)(v1.x - v0.x);
    if (dx > 0) 
        inc_x = 1;  /* I or IV quarter. */
    else if (dx < 0)
        inc_x = -1;  /* II or III quarter */
    else
        inc_x = 0;  /* Oy axis */

    s32 inc_y;
    s32 dy = (s32)(v1.y - v0.y);
    if (dy > 0)
        inc_y = 1;  /* I or II quarter. */
    else if (dy < 0)
        inc_y = -1;  /* III or IV quarter. */
    else
        inc_y = 0;  /* Ox axis. */

    /* Detrmine absolute values of the projections length. */
    dx = abs(dx); 
    dy = abs(dy);
 
    /* Determine the slope of the line. */ 
    s32 d;  /* Length of the line projection. */
    if (dx > dy)
        d = dx;  /* Line is closer to Ox axis. */
    else
        d = dy;  /* Line is closer to Oy axis. */
        
    /* Render the line. */
    u32 *pixel = (u32*)render->buffer->bitmap_memory;
    s32 x = (s32)v0.x; 
    s32 y = (s32)v0.y; 
    s32 x_err = 0;
    s32 y_err = 0;
    *SX_i = 0;
    for (s32 i = 0; i <= d; ++i)
    {          
        x_err += dx;
        y_err += dy;

        if (x_err > d)
        { 
            x_err -= d; 
            x += inc_x; 
        }

        if (y_err > d)
        { 
            y_err -= d; 
            y += inc_y;
            SX_array[*SX_i] = x; /* Record x when y has changed. */
            (*SX_i)++;
        }
        *(pixel + x + render->buffer->width * y) = color->color;
    }
    (*SX_i)--;
}

void
Render_DrawRect(Render *render, u32 x0, u32 y0, u32 width, u32 height, const Color *color)
{
    u32 *pixel = (u32*)render->buffer->bitmap_memory;
    for(u32 y = y0; y < height + y0; y++)
    {
        for (u32 x = x0; x < width + x0; x++)
        {
            *(pixel + x + render->buffer->width * y) = color->color;
        }
    }
}

void
Render_DrawRectWithBrd(Render *render, u32 x0, u32 y0, u32 width, u32 height, 
    u32 brd_width, const Color *color, const Color *brd_color)
{
    /* Draw the inner rectangle. */
    Render_DrawRect(render, x0 + brd_width, y0 + brd_width, width - brd_width * 2, 
        height - brd_width * 2, color);

    /* Draw the boarders with desired width and color. */
    if (brd_width)
    {
        Render_DrawRect(render, x0, y0, width, brd_width, brd_color);
        Render_DrawRect(render, x0, y0 + height - brd_width, width, brd_width, brd_color);
        Render_DrawRect(render, x0, y0, brd_width, height, brd_color);
        Render_DrawRect(render, x0 + width -brd_width, y0, brd_width, height, brd_color);
    }
}

static void 
DrawFillSideFlatTriangle(Render *render, Vec2 v1, Vec2 v2, Vec2 v3, FlatSideFlag side_flag, 
    const Color *color)
{
    /* Draw the side lines of the triangle. */
    u32 *LSX_array = render->triangle_data->LSX_array;
    u32 *RSX_array = render->triangle_data->RSX_array;
    u32 *pLSX_i = &(render->triangle_data->LSX_i);
    u32 *pRSX_i = &(render->triangle_data->RSX_i); 
    DrawLineExtended(render, v1, v2, color, LSX_array, pLSX_i);
    DrawLineExtended(render, v1, v3, color, RSX_array, pRSX_i);

    /* Determine value for the delta varuable. */
    u32 delta = (side_flag == SF_BOTTOM_FLAT) ? -1: 1;

    /* Fill the rectangle with horisontal lines. */
    u32 y = (u32)(v1.y + delta);  /* Start from the 1 line lower/upper then top/bottom  point. */
    for (u32 i = 0; i <= *pLSX_i; ++i)
    {
        u32 length = RSX_array[i] - LSX_array[i];
        Render_DrawHorLine(render, LSX_array[i], y, length, 1, color);
        y += delta;
    }
}

void 
Render_DrawFillTriangle(Render *render, Vec2 v1, Vec2 v2, Vec2 v3, const Color *color)
{   
    /* Sort the three vertices by y-coordinate so that v1 is the topmost vertix,
    v2 is the middle vertix and v3 is bottommost vertix (bubble sort). */
    if (v1.y < v2.y) Math_SwapVec2(&v1, &v2);
    if (v2.y < v3.y) Math_SwapVec2(&v2, &v3);
    if (v1.y < v2.y) Math_SwapVec2(&v1, &v2);

    /* Check for different triangle shapes to draw. */
    /* Case 1. Simple point. */
    if ((v1.y == v2.y) && (v2.y  == v3.y) && (v1.x == v2.x) && (v2.x == v3.x))
    {
        Render_SetPixelColor(render, (u32)v1.x, (u32)v1.y, color);
    }

    /* Case 2. Horisontal or vertical line. */
    else if (((v1.y == v2.y) && (v2.y  == v3.y)) || ((v1.x == v2.x) && (v2.x == v3.x)))
    {
        Render_DrawLine(render, v1.x, v1.y, v2.x, v2.y, color);
        Render_DrawLine(render, v2.x, v2.y, v3.x, v3.y, color);       
    }

    /* Case 3. Bottom-flat triangle. */
    else if (v2.y == v3.y)
    {
        (v2.x < v3.x) ?
            /* v1 - top vertex, v2 - left vertex, v3 - right vertex. */
            DrawFillSideFlatTriangle(render, v1, v2, v3, SF_BOTTOM_FLAT, color):
            /* v1 - top vertex, v3 - left vertex, v2 - right vertex. */
            DrawFillSideFlatTriangle(render, v1, v3, v2, SF_BOTTOM_FLAT, color);    
    }

    /* Case 4. Top-flat triangle. */
    else if (v1.y == v2.y)
    {
        (v1.x < v2.x) ?
            /* v3 - bottom vertex, v1 - left vertex, v2 - right vertex. */
            DrawFillSideFlatTriangle(render, v3, v1, v2, SF_TOP_FLAT, color):
            /* v3 - bottom vertex, v2 - left vertex, v1 - right vertex. */
            DrawFillSideFlatTriangle(render, v3, v2, v1, SF_TOP_FLAT, color);
    }

    /* Case 5. General case, split the triangle in a top-flat and bottom-flat. */
    else
    {
        f32 temp = (v3.x - v1.x) * (v1.y - v2.y) / (v1.y - v3.y);
        Vec2 v4;
        v4.x = roundf(temp + v1.x);
        v4.y = v2.y;

        /* Draw top triangle which is bottom-flat triangle. */
        (v2.x < v4.x) ? 
            /* v1 - top vertex, v2 - left vertex, v3 - right vertex. */
            DrawFillSideFlatTriangle(render, v1, v2, v4, SF_BOTTOM_FLAT, color):
            /* v1 - top vertex, v4 - left vertex, v2 - right vertex. */
            DrawFillSideFlatTriangle(render, v1, v4, v2, SF_BOTTOM_FLAT, color);
        
        /* Draw bottom triangle which is top-flat triangle. */
        (v2.x < v4.x) ?
            /* v3 - bottom vertex, v2 - left vertex, v4 - right vertex. */
            DrawFillSideFlatTriangle(render, v3, v2, v4, SF_TOP_FLAT, color):
            /* v3 - bottom vertex, v4 - left vertex, v2 - right vertex. */
            DrawFillSideFlatTriangle(render, v3, v4, v2, SF_TOP_FLAT, color);
    }

    /* Clean-up. Draw additional lines to to each vertices. */
    /* NOTE: This could be deleted later. */
    Render_DrawLine(render, v1.x, v1.y, v2.x, v2.y, color);
    Render_DrawLine(render, v2.x, v2.y, v1.x, v1.y, color);

    Render_DrawLine(render, v2.x, v2.y, v3.x, v3.y, color);
    Render_DrawLine(render, v3.x, v3.y, v2.x, v2.y, color);

    Render_DrawLine(render, v1.x, v1.y, v3.x, v3.y, color);
    Render_DrawLine(render, v3.x, v3.y, v1.x, v1.y, color);  
}

void 
Render_DrawRotatedRect(Render *render, f32 x0, f32 y0, f32 width, f32 height, f32 angle, 
    const Color *color)
{       
    /* Initialize the points with the data. */
    Vec2 BL, BR, UL, UR; /* Initial points of the rectangle. */
    BL.x = x0; BL.y = y0;
    BR.x = x0 + width; BR.y = y0;
    UL.x = x0; UL.y = y0 + height;
    UR.x = x0 + width; UR.y = y0 + height;
    
    /* Transform degrees to radians. */
    f32 angle_rad = angle * (f32)M_PI / 180.0f;

    /* Determine the center point of the rectangle. */
    f32 xc = x0 + width / 2.0f;
    f32 yc = y0 + height / 2.0f;

    /* Determine new points coordinates (with recpect to the rect center). */
    Vec2 BL_c, BR_c, UL_c, UR_c; /* Coordinates with respect to the rect center. */
    BL_c.x = BL.x - roundf(xc); BL_c.y = BL.y - roundf(yc);
    BR_c.x = BR.x - roundf(xc); BR_c.y = BR.y - roundf(yc);
    UL_c.x = UL.x - roundf(xc); UL_c.y = UL.y - roundf(yc);
    UR_c.x = UR.x - roundf(xc); UR_c.y = UR.y - roundf(yc);

    /* Transform the points by rotation. */
    Vec2 BL_tr, BR_tr, UL_tr, UR_tr; /* Points after rotation transformation. */
    BL_tr.x = roundf(xc + (BL_c.x * cosf(angle_rad) - BL_c.y * sinf(angle_rad)));
    BL_tr.y = roundf(yc + (BL_c.x * sinf(angle_rad) + BL_c.y * cosf(angle_rad)));
    BR_tr.x = roundf(xc + (BR_c.x * cosf(angle_rad) - BR_c.y * sinf(angle_rad)));
    BR_tr.y = roundf(yc + (BR_c.x * sinf(angle_rad) + BR_c.y * cosf(angle_rad)));
    UL_tr.x = roundf(xc + (UL_c.x * cosf(angle_rad) - UL_c.y * sinf(angle_rad)));
    UL_tr.y = roundf(yc + (UL_c.x * sinf(angle_rad) + UL_c.y * cosf(angle_rad)));
    UR_tr.x = roundf(xc + (UR_c.x * cosf(angle_rad) - UR_c.y * sinf(angle_rad)));
    UR_tr.y = roundf(yc + (UR_c.x * sinf(angle_rad) + UR_c.y * cosf(angle_rad)));

    /* Draw the rectangle (by drawing two triangles). */
    Render_DrawFillTriangle(render, BL_tr, BR_tr, UR_tr, color);
    Render_DrawFillTriangle(render, BL_tr, UL_tr, UR_tr, color);        
}

void
Render_DrawCircle(Render *render, f32 x0, f32 y0, f32 radius, b32 is_filled, 
    const Color *color)
{
    s32 x = 0;
    s32 y = (s32)radius;
    s32 delta = (2 - 2 * (s32)radius);

    while (y >= 0)
    {
        /* Draw points lying on a circle */
        Render_SetPixelColor(render, (u32)x0 + (u32)x, (u32)y0 + (u32)y, color);
        Render_SetPixelColor(render, (u32)x0 + (u32)x, (u32)y0 - (u32)y, color);
        Render_SetPixelColor(render, (u32)x0 - (u32)x, (u32)y0 - (u32)y, color);
        Render_SetPixelColor(render, (u32)x0 - (u32)x, (u32)y0 + (u32)y, color);

        /* Fill the circle if it is necessary (with lines from top to bottom)*/
        if (is_filled)
        {    
            Render_DrawLine(render, x0 + (f32)x, y0 + (f32)y, x0 + (f32)x, y0 - (f32)y,  color);
            Render_DrawLine(render, x0 - (f32)x, y0 + (f32)y, x0 - (f32)x, y0 - (f32)y,  color);
        }

        s32 gap = 2 * (delta + y) - 1;
        if ((delta < 0) && (gap <= 0))
        {
            x++;
            delta += 2 * x + 1;
            continue;
        }

        if ((delta > 0) && (gap > 0))
        {
            y--;
            delta -= 2 * y + 1;
            continue;
        }

        x++;
        delta += 2 * (x - y);
        y--;
    }   
}

static void
DrawBitmapExtended(Render *render, u32 x, u32 y, const Image* image, u32 scale, b32 is_mask, 
    const Color *mask_color)
{
    u32 *pixel = (u32*)render->buffer->bitmap_memory;
    u32 index = 0;
    
    for (u32 i = 0; i < image->height; ++i)
    {
        for (u32 j = 0; j < image->width; ++j)
        {
            /* Get the color of the pixel. */
            Color color;
            Color_SetFromImageColorData(&color, *(image->data + index));

            /* Check the alpha channel. */
            if (color.alpha != 0x00)
            {
                /* Use color from loaded bitmap or from the mask color. */
                if (is_mask) color.color = mask_color->color;
                
                /* Draw scaled pixel */
                for (u32 n = 0; n < scale; ++n)
                {
                    for (u32 m = 0; m < scale; ++m)
                    {
                        u32 x_scaled = x + j * scale + m;
                        u32 y_scaled = y + i * scale + n;
                        *(pixel + x_scaled + render->buffer->width * y_scaled) = color.color;                    
                    }        
                }
            }
            index++;
        }
    }
}

void
Render_DrawBitmap(Render *render, u32 x, u32 y, Image *image, u32 scale)
{
    Color color;  /* Color to replace the mask (will not be used). */
    color.color = 0xffffff;
    DrawBitmapExtended(render, x, y, image, scale, false, &color);
}

void
Render_DrawBitmapByMask(Render *render, u32 x, u32 y, const Image *mask, const Color *color,
    u32 scale)
{
    DrawBitmapExtended(render, x, y, mask, scale, true, color);
}
