/**
 * ================================================================================
 * @file font.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with font.
 * @version 0.2
 * @date 2022-07-09
 * ================================================================================
 */

/* Game engine includes: */
#include <font.h>
#include <utils.h>
#include <image.h>
#include <color.h>
#include <render.h>

Font_t*
font_constructor(void)
{
    Font_t *font;  /* Pointer to the Font structure. */

    /* Memory allocation for the font object and font symbols. */
    font = (Font_t*)malloc(1 * sizeof(Font_t));
    font->symbols_data = NULL;
    return font;
}

void
font_destructor(Font_t *font)
{
    /* Free memory allocated for the symbols data. */
    free(font->symbols_data);
    font->symbols_data = NULL;
    
    /* Free memory allocated for the Font object. */
    free(font);
    font = NULL;
}

void
font_init(Font_t *font, s32 rows_num, s32 cols_num, s32 sym_width, s32 sym_height, 
    Image_t *img_font)
{
    Symbol_data_t *symbols_data;  /* Pointer to the symbols data array. */
    Color_t color;  /* Color of the symbols pixels in font image. */
    s32 win1251_code;  /* Current Win1251 code of the reading symbol. */
    s32 i_0, j_0;  /* Pixel coordinates of the BL corner of the symbol. */
    s32 i, j;  /* Coordinates of the pixels in downloaded image. */
    s32 sym_i, sym_j;  /* Coordinates of symbols in the symbols table in font_img. */
    u32 index;  /* Index of the point in font_img pixel array. */
    u32 p;  /* Index of the symbol_array in font_symbols. */
    u32 width_max;  /* Maximum width of the current symbol. */
    u32 width_on_line;  /* Width of the symbol on current line. */
    u32 shift_left;  /* How much to shift the next symbol to the left. */
    u32 shift_bottom;  /* How much to thift the symbol to bottom. */
    u32 black_color;  /* Black color (pixel of the font symbol). */
    u32 red_color;  /* Red color (defines shift to bottom). */

    black_color = 0xff000000;
    red_color = 0xffff0000;

    /* Allocate memory for the all symbols data. */
    symbols_data = (Symbol_data_t*)calloc(rows_num * cols_num, sizeof(Symbol_data_t));
    font->symbols_data = symbols_data;
    font->rows_num = rows_num;
    font->cols_num = cols_num;
    font->sym_width = sym_width;
    font->sym_height = sym_height;

    /* Extract symbols from the font image file. */
    for (sym_i = (rows_num - 1); sym_i >= 0; --sym_i) 
    {
        for (sym_j = 0; sym_j < cols_num; ++sym_j) 
        {
            /* Determine the current symbol Win1251 code (0 is for upper-left symbol). */
            win1251_code = (rows_num * cols_num) - sym_i * cols_num - (cols_num - sym_j);
            symbols_data[win1251_code].symbol = (u8)win1251_code;

            /* Determine the pixels coordinate of the BL corner of the symbol. Aslo take 
            into account the title of the table. */
            i_0 = img_font->height - (sym_height + 1) * (win1251_code / cols_num) \
                - sym_height - (sym_height + 1);
            j_0 = (sym_width + 1) * (win1251_code % cols_num) + (sym_width + 1);

            /* Copying the symbol from font_img to font_symbols. */
            p = 0;
            width_max = 0;
            shift_left = 0;
            shift_bottom = 0;

            for (i = i_0; i < (i_0 + sym_height); ++i) 
            {
                width_on_line = 0;
                for (j = j_0; j < (j_0 + sym_width); ++j) 
                {
                    /* Define the color of the pixel in the font image. */
                    index = i * img_font->width + j;
                    color_set_from_image_color_data(&color, *(img_font->data + index));

                    if (color.color == black_color)
                    {
                        symbols_data[win1251_code].symbol_array[p] = (u8)1;
                        width_on_line = j - j_0 + 1;
                    }
                    else if (color.color == red_color)
                    {
                        symbols_data[win1251_code].symbol_array[p] = (u8)0;
                        shift_bottom = i - i_0;
                    }
                    else /* Other color (light yellow in the image). */ 
                    {
                        symbols_data[win1251_code].symbol_array[p] = (u8)0;
                    }
                    p++;
                }
                
                /* Save the maximum line width of the symbol. */
                if (width_on_line > width_max) width_max = width_on_line;
            }
            
            /* Define the shifts of the symbol. */
            symbols_data[win1251_code].shift_left = (win1251_code == 32) ? 
                3 : sym_width - width_max;
            
            symbols_data[win1251_code].shift_bottom = shift_bottom;
        }
    }
}

void
font_draw_string(Font_t *font, char *str, s32 str_max_width, u32 x, u32 y, 
    u32 size, Color_t *color, Render_t *render)
{
    u32 x_init;  /* Initial x position of the very first symbol. */
    u32 str_width;  /* Current width of the string in pixels. */
    u32 char_index;  /* Index of the char in str. */
    u32 p;  /* Index of the symbol_array in symbols_data. */
    u32 i, j;  /* Coordinates of pixels in symbol box. */
    u32 x_pos, y_pos;  /* Coordinates of pixels taking into account the pixel size. */
    Symbol_data_t *symbols_data;  /* Pointer to the font symbols data. */
    Symbol_data_t *symbol;  /* Pointer to the current symbol. */
    u32 sym_width;  /* Symbol width in pixels. */
    u32 sym_height;  /* Symbol height in pixels. */
    b32 stop_print;  /* Flag to stop print the symbols. */

    symbols_data = font->symbols_data;
    sym_width = font->sym_width;
    sym_height = font->sym_height;
    x_init = x;
    char_index = 0;
    str_width = 0;
    stop_print = false;
    
    while (str[char_index] != '\0') 
    {
        if ((str_max_width > 0) && (str_width >= str_max_width - (sym_width + 1))) 
        {
            /* Print "..." and then stop */
            symbol = &(symbols_data[133]);
            stop_print = true;
        }
        else 
        {
            symbol = &(symbols_data[(str[char_index])]);
        }

        /* Add necessary bottom shift */
        y -= (symbol->shift_bottom) * size;
        
        p = 0;
        for (i = 0; i < sym_height; ++i) 
        {
            for (j = 0; j < sym_width; ++j) 
            {
                x_pos = x + j * size;
                y_pos = y + i * size;

                if (symbol->symbol_array[p]) 
                {
                    render_draw_rect(render, x_pos, y_pos, size, size, color);
                }
                p++;
            }
        }

        /* Restore the normal y position. */
        y += (symbol->shift_bottom) * size;

        /* Determine the x position of the next symbol in str. */
        x += ((sym_width + 1) - symbol->shift_left) * size;
        str_width = x - x_init;
        char_index++;
        
        /* Check to exit the printing procedure. */
        if (stop_print) break;
    }  
}
