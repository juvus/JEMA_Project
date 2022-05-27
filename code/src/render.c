/**
 * ================================================================================
 * @file render.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions related to the software rendering. 
 * @version 0.2
 * @date 2022-05-26
 * @copyright GNU General Public License (GPL) v3.0
 * ================================================================================ 
 */

#define _USE_MATH_DEFINES

/* Standard includes: */
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Program includes: */
#include <render.h>
#include <utils.h>
#include <image.h>
#include <helper_functions.h>
#include <color.h>

/* Static functions */
/**
 * @brief Drawing a pixelized line from one point to another by using 
 * Brezenham algorithm. This version also record the X coordinates for 
 * further rasterization of filling of the triangle.
 * @param v0 Vector for the beginning point.
 * @param v1 Vector for the ending point.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render_buffer structure.
 * @param SX_array Left/Right Side X array.
 * @param SX_i Left/Right Side X array index.
 * @return void.
 */
static void 
draw_line_extended(V2_u32_t v0, V2_u32_t v1, u32 color, Render_t *render, 
    u32 *SX_array, u32 *SX_i);

/**
 * @brief Drawing a filled bottom/top flat triangle.
 * @param v1 Top/Bottom most vertix of the triangle.
 * @param v2 Left most vertix of the triangle.
 * @param v3 Right most vertix of the triangle.
 * @param side_flag Flag inicating flat side.
 * @param color A 32 bit color (ARGB).
 * @param render Pointer to the render structure.
 */
static void 
draw_fill_side_flat_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, 
    Flat_Side_Flag_t side_flag, u32 color, Render_t *render);

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
draw_bitmap_extended(u32 x, u32 y, Image_t* image, u32 scale, b32 is_mask, u32 mask_color, 
    Render_t *render);

Render_t*
render_constructor(void)
{
    Render_t *render = NULL;  /* Pointer to the render. */
    Render_Buffer_t *buffer = NULL;  /* Pointer to the render buffer. */
    Triangle_Data_t *triangle_data = NULL;  /* Pointer to the triangle data. */

    /* Allocate the memory for the render buffer object. */
    buffer = (Render_Buffer_t *)malloc(1 * sizeof(Render_Buffer_t));

    /* Allocate the memory for the triangle filling data. */
    triangle_data = (Triangle_Data_t*)malloc(1 * sizeof(Triangle_Data_t));

    /* Allocate the memory for the render object. */
    render = (Render_t *)malloc(1 * sizeof(Render_t));

    render->buffer = buffer;
    render->triangle_data = triangle_data;
    return render;
}

void
render_destructor(Render_t *render)
{
    /* Release memory allocated for the bitmap memory. */
    VirtualFree(render->buffer->bitmap_memory, 0, MEM_RELEASE);
    
    /* Release drawing content. */
    ReleaseDC(render->window, render->hdc);

    /* Release memory allocated for the render buffer object. */
    free(render->buffer);

    /* Release memory allocated for the triangle data. */
    free(render->triangle_data);

    /* Release memory allocated for the render object. */
    free(render);
}

void
render_init(Render_t *render, HWND window)
{
    RECT rect;  /* Rectangle for holding the info about the graphic window. */
    u32 rect_width;  /* Rectangle width. */
    u32 rect_height;  /* Rectangle height. */
    SIZE_T mem_size;  /* Render buffer memory size. */

    /* Get and save header to the window and its drawing context. */
    render->window = window;
    render->hdc = GetDC(window);

    /* Calculate width and height of the window (client area) for the buffer. */
    GetClientRect(window, &rect);
    rect_width = (u32)(rect.right - rect.left);
    rect_height = (u32)(rect.bottom - rect.top);
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
    mem_size = sizeof(u32) * rect_width * rect_height;
    render->buffer->bitmap_memory = VirtualAlloc(0, mem_size, MEM_COMMIT|MEM_RESERVE, 
        PAGE_READWRITE);

    /* Fill the bitmap_info structure */
    render->bitmap_info.bmiHeader.biSize = sizeof(render->bitmap_info.bmiHeader);
    render->bitmap_info.bmiHeader.biWidth = rect_width;
    render->bitmap_info.bmiHeader.biHeight = rect_height;
    render->bitmap_info.bmiHeader.biPlanes = 1;
    render->bitmap_info.bmiHeader.biBitCount = 32;
    render->bitmap_info.bmiHeader.biCompression = BI_RGB;
}

void
render_clear_screen(u32 color, Render_t *render)
{
    u32 pitch = render->width * 4;  /* Size of a row in bytes (4 bytes for a color) */
    u32 width = render->width;  /* Width of the bitmap field */
    u32 height = render->height;  /* Height of the bitmap field */
    u8 *row = (u8*)render->buffer->bitmap_memory;  /* Pointer to a byte starting new row */
    u32 *pixel = NULL;  /* Pointer to a pixel in a bitmap */
    u32 x, y;  /* Coordinates of a pixel */

    /* Procedure to fill the pixeld in optimized way */
    for (y = 0; y < height; y++)
    {
        pixel = (u32*)row;
        for (x = 0; x < width; x++)
        {
            *(pixel) = color;
            ++pixel;
        }
        row += pitch;
    }
}

void
render_put_pixel(u32 x, u32 y, u32 color, Render_t *render)
{    
    u32 *pixel = (u32*)render->buffer->bitmap_memory; /* Pointer to the pixel in the buffer. */
    
    /* Define color of a pixel with desired coordinates. */
    *(pixel + x + render->width * y) = color;    
}

u32
render_get_pixel_color(u32 x, u32 y, Render_t *render)
{
    u32 color;  /* A 32 bit color (ARGB). */
    u32 *pixel = (u32*)render->buffer->bitmap_memory; /* Pointer to the pixel in the buffer. */
    
    color = *(pixel + x + render->width * y);
    return color;
}

void
render_draw_line(u32 x0, u32 y0, u32 x1, u32 y1, u32 color, Render_t *render)
{
    /* TODO: add posibility to draw a line with the desired width. */
    V2_u32_t v0;  /* Vector for the beginning point. */
    V2_u32_t v1;  /* Vector for the ending point. */
    u32 *LSX_array;  /* Left Side X array. */
    u32 *pLSX_i;  /* Left Side X array index. */

    /* Initialization of the local variables. */
    v0.x = x0;
    v0.y = y0;
    v1.x = x1;
    v1.y = y1;
    LSX_array = render->triangle_data->LSX_array;
    pLSX_i = &(render->triangle_data->LSX_i);

    /* Draw line in the extended mode. */
    draw_line_extended(v0, v1, color, render, LSX_array, pLSX_i);
}

void
render_draw_hor_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_t *render)
{
    u32 *pixel;  /* Pointer to the pixel in the buffer. */
    u32 x, y;  /* Coordinates of the pixel. */

    pixel = (u32*)render->buffer->bitmap_memory;

    /* Draw a horisontal line. */
    for (x = x0; x < (x0 + length); ++x)
    {
        for (y = y0; y < (y0 + width); ++y)
        {
            *(pixel + x + render->width * y) = color;
        }
    }
}

void
render_draw_ver_line(u32 x0, u32 y0, u32 length, u32 width, u32 color, Render_t *render)
{   
    u32 *pixel;  /* Pointer to the pixel in the buffer. */
    u32 x, y;  /* Coordinates of the pixel. */

    pixel = (u32*)render->buffer->bitmap_memory;

    /* Draw a vertical line. */
    for (y = y0; y < (y0 + length); ++y)
    {
        for (x = x0; x < (x0 + width); ++x)
        {
            *(pixel + x + render->width * y) = color;
        }
    }
}

static void
draw_line_extended(V2_u32_t v0, V2_u32_t v1, u32 color, Render_t *render,
    u32 *SX_array, u32 *SX_i)
{
    u32 *pixel;  /* Pointer to the pixel in the buffer. */
    s32 x_err, y_err;  /* Errors using in the algorithm. */
    s32 dx, dy;  /* Length of the line projection on the X and Y axis. */
    s32 d;  /* Length of the line projection. */
    s32 inc_x, inc_y;  /* Coordinate increments. */
    s32 x, y;  /* Pixel coordinate. */ 
    s32 i;  /* Index. */ 

    /* Initialization of the local variables. */
    pixel = (u32*)render->buffer->bitmap_memory;
    x_err = 0;
    y_err = 0;
    dx = v1.x - v0.x;
    dy = v1.y - v0.y;
    *SX_i = 0;

    /* Deterime increment */
    if (dx > 0) /* I or IV quarter */ 
        inc_x = 1;
    else if (dx < 0) /* II or III quarter */
        inc_x = -1;
    else /* Oy axis */ 
        inc_x = 0;
 
    if (dy > 0) /* I or II quarter */
        inc_y = 1;
    else if (dy < 0) /* III or IV quarter */
        inc_y = -1;
    else /* Ox axis */
        inc_y = 0;

    /* Detrmine absolute values of the projections length. */
    dx = abs(dx); 
    dy = abs(dy);
 
    /* Determine the slope of the line. */ 
    if (dx > dy) 
        /* Line is closer to Ox axis. */
        d = dx; 
    else 
        /* Line is closer to Oy axis. */
        d = dy;

    /* Render the line. */
    x = v0.x; 
    y = v0.y; 
    for (i = 0; i <= d; ++i)
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

        *(pixel + x + render->width * y) = color;
    }
    (*SX_i)--;
}

void
render_draw_rect(u32 x0, u32 y0, u32 width, u32 height, u32 color, Render_t *render)
{
    u32 *pixel;  /* Pointer to the pixel in the buffer. */
    u32 x, y;  /* Coordinates of the pixel. */

    pixel = (u32*)render->buffer->bitmap_memory;

    /* Draw a rectangle. */
    for(y = y0; y < height + y0; y++) {
        for (x = x0; x < width + x0; x++) {
            *(pixel + x + render->width * y) = color;
        }
    }
}

void
render_draw_rect_with_brd(u32 x0, u32 y0, u32 width, u32 height, u32 brd_width, 
    u32 color, u32 brd_color, Render_t *render)
{
    /* Render the inner rectangle. */
    render_draw_rect(x0 + brd_width, y0 + brd_width, width - brd_width * 2, 
        height - brd_width * 2, color, render);

    /* Render the boarders with desired width and color. */
    if (brd_width) {
        render_draw_rect(x0, y0, width, brd_width, brd_color, render);
        render_draw_rect(x0, y0 + height - brd_width, width, brd_width, brd_color, render);
        render_draw_rect(x0, y0, brd_width, height, brd_color, render);
        render_draw_rect(x0 + width -brd_width, y0, brd_width, height, brd_color, render);
    }
}

static void 
draw_fill_side_flat_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, 
    Flat_Side_Flag_t side_flag, u32 color, Render_t *render)
{
    u32 y;  /* Y coordinate of the horizontal line for the filling. */
    u32 i;  /* Index. */
    u32 length;  /* Length of the horizontal line for the filling. */
    u32 *LSX_array;  /* Pointer to the left side X array. */
    u32 *RSX_array;  /* Pointer to the right side X array. */
    u32 *pLSX_i;  /* Pointer to the left side X array index. */
    u32 *pRSX_i;  /* Pointer to the right side X array index. */
    u32 delta;  /* Delta in the coordinate change. */  

    /* Initialize the local variables. */
    LSX_array = render->triangle_data->LSX_array;
    RSX_array = render->triangle_data->RSX_array;
    pLSX_i = &(render->triangle_data->LSX_i);
    pRSX_i = &(render->triangle_data->RSX_i); 

    /* Draw the side lines of the triangle. */
    draw_line_extended(v1, v2, color, render, LSX_array, pLSX_i);
    draw_line_extended(v1, v3, color, render, RSX_array, pRSX_i);

    /* Determine value for the delta varuable. */
    delta = (side_flag == SF_BOTTOM_FLAT) ? -1: 1;

    /* Fill the rectangle with horisontal lines. */
    i = 0;
    y = v1.y + delta; /* Start from the 1 line lower/upper then top/bottom  point. */
    for (i = 0; i <= *pLSX_i; ++i)
    {
        length = RSX_array[i] - LSX_array[i];
        render_draw_hor_line(LSX_array[i], y, length, 1, color, render);
        y += delta;
    }
}

void 
render_draw_fill_triangle(V2_u32_t v1, V2_u32_t v2, V2_u32_t v3, u32 color,
    Render_t *render)
{
    f32 temp;  /* Temporary value. */
    V2_u32_t v4;  /* Additional vertix. */
    
    /* Sort the three vertices by y-coordinate so that v1 is the topmost vertix,
    v2 is the middle vertix and v3 is bottommost vertix (bubble sort). */
    if (v1.y < v2.y) swap_V2_u32(&v1, &v2);
    if (v2.y < v3.y) swap_V2_u32(&v2, &v3);
    if (v1.y < v2.y) swap_V2_u32(&v1, &v2);

    /* Check for different triangle shapes to draw. */
    /* Case 1. Simple point. */
    if ((v1.y == v2.y) && (v2.y  == v3.y) && (v1.x == v2.x) && (v2.x == v3.x))
    {
        render_put_pixel(v1.x, v1.y, color, render);
    }

    /* Case 2. Horisontal or vertical line. */
    else if (((v1.y == v2.y) && (v2.y  == v3.y)) || ((v1.x == v2.x) && (v2.x == v3.x)))
    {
        render_draw_line(v1.x, v1.y, v2.x, v2.y, color, render);
        render_draw_line(v2.x, v2.y, v3.x, v3.y, color, render);       
    }

    /* Case 3. Bottom-flat triangle. */
    else if (v2.y == v3.y)
    {
        (v2.x < v3.x) ?
            /* v1 - top vertex, v2 - left vertex, v3 - right vertex. */
            draw_fill_side_flat_triangle(v1, v2, v3, SF_BOTTOM_FLAT, color, render):
            /* v1 - top vertex, v3 - left vertex, v2 - right vertex. */
            draw_fill_side_flat_triangle(v1, v3, v2, SF_BOTTOM_FLAT, color, render);    
    }

    /* Case 4. Top-flat triangle. */
    else if (v1.y == v2.y)
    {
        (v1.x < v2.x) ?
            /* v3 - bottom vertex, v1 - left vertex, v2 - right vertex. */
            draw_fill_side_flat_triangle(v3, v1, v2, SF_TOP_FLAT, color, render):
            /* v3 - bottom vertex, v2 - left vertex, v1 - right vertex. */
            draw_fill_side_flat_triangle(v3, v2, v1, SF_TOP_FLAT, color, render);
    }

    /* Case 5. General case, split the triangle in a top-flat and bottom-flat. */
    else {
        temp = ((f32)v3.x - (f32)v1.x) * ((f32)v1.y - (f32)v2.y) / ((f32)v1.y - (f32)v3.y);
        v4.x = (u32)(roundf(temp + (f32)v1.x));
        v4.y = v2.y;

        /* Draw top triangle which is bottom-flat triangle. */
        (v2.x < v4.x) ? 
            /* v1 - top vertex, v2 - left vertex, v3 - right vertex. */
            draw_fill_side_flat_triangle(v1, v2, v4, SF_BOTTOM_FLAT, color, render):
            /* v1 - top vertex, v4 - left vertex, v2 - right vertex. */
            draw_fill_side_flat_triangle(v1, v4, v2, SF_BOTTOM_FLAT, color, render);
        
        /* Draw bottom triangle which is top-flat triangle. */
        (v2.x < v4.x) ?
            /* v3 - bottom vertex, v2 - left vertex, v4 - right vertex. */
            draw_fill_side_flat_triangle(v3, v2, v4, SF_TOP_FLAT, color, render):
            /* v3 - bottom vertex, v4 - left vertex, v2 - right vertex. */
            draw_fill_side_flat_triangle(v3, v4, v2, SF_TOP_FLAT, color, render);
    }

    /* Clean-up. Draw additional lines to to each vertices. */
    render_draw_line(v1.x, v1.y, v2.x, v2.y, color, render);
    render_draw_line(v2.x, v2.y, v1.x, v1.y, color, render);

    render_draw_line(v2.x, v2.y, v3.x, v3.y, color, render);
    render_draw_line(v3.x, v3.y, v2.x, v2.y, color, render);

    render_draw_line(v1.x, v1.y, v3.x, v3.y, color, render);
    render_draw_line(v3.x, v3.y, v1.x, v1.y, color, render);  
}

void 
render_draw_rotated_rect(u32 x0, u32 y0, u32 width, u32 height, f32 angle, u32 color,
    Render_t *render)
{   
    f32 angle_rad; /* Angle in radians. */
    f32 xc, yc; /* Coordinates of the center point of rectangle. */
        
    V2_s32_t BL, BR, UL, UR; /* Initial points of the rectangle. */
    V2_s32_t BL_c, BR_c, UL_c, UR_c; /* Coordinates with respect to the rect center. */
    V2_u32_t BL_tr, BR_tr, UL_tr, UR_tr; /* Points after rotation transformation. */

    /* Initialize the points with the data. */
    BL.x = x0; BL.y = y0;
    BR.x = x0 + width; BR.y = y0;
    UL.x = x0; UL.y = y0 + height;
    UR.x = x0 + width; UR.y = y0 + height;
    
    /* Transform degrees to radians. */
    angle_rad = angle * (f32)M_PI / 180.0f;

    /* Determine the center point of the rectangle. */
    xc = x0 + width / 2.0f;
    yc = y0 + height / 2.0f;

    /* Determine new points coordinates (with recpect to the rect center). */
    BL_c.x = BL.x - (s32)roundf(xc); BL_c.y = BL.y - (s32)roundf(yc);
    BR_c.x = BR.x - (s32)roundf(xc); BR_c.y = BR.y - (s32)roundf(yc);
    UL_c.x = UL.x - (s32)roundf(xc); UL_c.y = UL.y - (s32)roundf(yc);
    UR_c.x = UR.x - (s32)roundf(xc); UR_c.y = UR.y - (s32)roundf(yc);

    /* Transform the points by rotation. */
    BL_tr.x = (u32)roundf(xc + (BL_c.x * cosf(angle_rad) - BL_c.y * sinf(angle_rad)));
    BL_tr.y = (u32)roundf(yc + (BL_c.x * sinf(angle_rad) + BL_c.y * cosf(angle_rad)));
    BR_tr.x = (u32)roundf(xc + (BR_c.x * cosf(angle_rad) - BR_c.y * sinf(angle_rad)));
    BR_tr.y = (u32)roundf(yc + (BR_c.x * sinf(angle_rad) + BR_c.y * cosf(angle_rad)));
    UL_tr.x = (u32)roundf(xc + (UL_c.x * cosf(angle_rad) - UL_c.y * sinf(angle_rad)));
    UL_tr.y = (u32)roundf(yc + (UL_c.x * sinf(angle_rad) + UL_c.y * cosf(angle_rad)));
    UR_tr.x = (u32)roundf(xc + (UR_c.x * cosf(angle_rad) - UR_c.y * sinf(angle_rad)));
    UR_tr.y = (u32)roundf(yc + (UR_c.x * sinf(angle_rad) + UR_c.y * cosf(angle_rad)));

    /* Draw the rectangle (by drawing two triangles). */
    render_draw_fill_triangle(BL_tr, BR_tr, UR_tr, color, render);
    render_draw_fill_triangle(BL_tr, UL_tr, UR_tr, color, render);        
}

void
render_draw_circle(u32 x0, u32 y0, u32 radius, b32 is_filled ,u32 color, Render_t *render)
{
    s32 x;  /* Pixel x coordinate. */ 
    s32 y;  /* Pixel y coordinate. */
    s32 gap;  /*  */
    s32 delta;

    /* Initialization of the local variables. */
    x = 0;
    y = (s32)radius;
    gap = 0;
    delta = (2 - 2 * (s32)radius);

    while (y >= 0) {
        /* Draw points lying on a circle */
        render_put_pixel(x0 + (u32)x, y0 + (u32)y, color, render);
        render_put_pixel(x0 + (u32)x, y0 - (u32)y, color, render);
        render_put_pixel(x0 - (u32)x, y0 - (u32)y, color, render);
        render_put_pixel(x0 - (u32)x, y0 + (u32)y, color, render);

        /* Fill the circle if it is necessary (with lines from top to bottom)*/
        if (is_filled)
        {    
            render_draw_line(x0 + (u32)x, y0 + (u32)y, x0 + (u32)x, y0 - (u32)y,  color, render);
            render_draw_line(x0 - (u32)x, y0 + (u32)y, x0 - (u32)x, y0 - (u32)y,  color, render);
        }

        gap = 2 * (delta + y) - 1;
        if ((delta < 0) && (gap <= 0)) {
            x++;
            delta += 2 * x + 1;
            continue;
        }

        if ((delta > 0) && (gap > 0)) {
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
draw_bitmap_extended(u32 x, u32 y, Image_t* image, u32 scale, b32 is_mask, u32 mask_color, 
    Render_t *render)
{
    u32 color;  /* An actual 32 bit pixel color (ARGB). */
    u32 x_scaled, y_scaled;  /* Pixel coordinates in the scaled image. */
    u32 i, j; /* Coordinates of the unscaled image. */
    u32 m, n; /* Coordinates inside a scaled single image pixel. */
    u32 index;  /* Pixel index in the image buffer. */
    u32 *pixel;  /* Pointer to the pixel in the buffer. */

    /* Initialization of the local variables. */
    pixel = (u32*)render->buffer->bitmap_memory;
    index = 0;
    
    for (i = 0; i < image->height; ++i)
    {
        for (j = 0; j < image->width; ++j)
        {
            /* Get the color of the pixel. */
            color = convert_RGBA_to_ARGB(*(image->data + index));

            /* Check the alpha channel. */
            if (get_color_alpha(color) != 0x00)
            {
                /* Use color from loaded bitmap or from the mask color. */
                if (is_mask) color = mask_color;
                
                /* Draw scaled pixel */
                for (n = 0; n < scale; ++n)
                {
                    for (m = 0; m < scale; ++m)
                    {
                        x_scaled = x + j * scale + m;
                        y_scaled = y + i * scale + n;
                        *(pixel + x_scaled + render->buffer->width * y_scaled) = color;                    
                    }        
                }
            }
            index++;
        }
    }
}

void
render_draw_bitmap(u32 x, u32 y, Image_t* image, u32 scale, Render_t *render)
{
    draw_bitmap_extended(x, y, image, scale, false, 0xffffff, render);
}

void
render_draw_bitmap_by_mask(u32 x, u32 y, Image_t *mask, u32 color, u32 scale, Render_t *render)
{
    draw_bitmap_extended(x, y, mask, scale, true, color, render);
}
