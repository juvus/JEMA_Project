/**
 * ================================================================================
 * @file font.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with font.
 * @version 0.2
 * @date 2022-01-09
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

/* Program includes: */
#include <font.h>
#include <utils.h>
#include <image.h>
#include <render.h>

/* Global static variables */
extern u32 SYM_ROWS; /* Amount of symbol rows in font.png */
extern u32 SYM_COLS; /* Amount of sumbol columns in font.png */

Font_t*
font_constructor(void)
{
    Font_t *font = NULL;

    /* Memory allocation for the font object and font symbols. */
    font = (Font_t*)malloc(1 * sizeof(Font_t));
    font->symbols_data = (Symbol_data_t*)calloc(SYM_ROWS * SYM_COLS, sizeof(Symbol_data_t));
    return font;
}

void
font_destructor(Font_t *font)
{
    free(font->symbols_data);
    free(font);
}

void
font_extract_symbols(Font_t *font, Image_t *img_font)
{
    Symbol_data_t *symbols_data;  /* Pointer to the symbols data array. */
    u32 color;  /* Color of the symbols pixels in font image. */
    s32 ASCII_code;  /* Current ASCII code of the reading symbol. */
    s32 sym_num = 0;  /* Number of reading symbol (0 is the Upper-Left symbol). */
    s32 i_0, j_0;  /* Pixel coordinates of the BL corner of the symbol. */
    s32 i, j;  /* Coordinates of the pixels in downloaded image. */
    s32 sym_i, sym_j;  /* Coordinates of symbols in the symbols table in font_img. */
    s32 index;  /* Index of the point in font_img pixel array. */
    s32 p;  /* Index of the symbol_array in font_symbols. */
    s32 width_max;  /* Maximum width of the current symbol. */
    s32 width_on_line;  /* Width of the symbol on current line. */
    
    symbols_data = font->symbols_data;

    for (sym_i = (SYM_ROWS - 1); sym_i >= 0; --sym_i) 
    {
        for (sym_j = 0; sym_j < SYM_COLS; ++sym_j) 
        {
            /* Determine the current symbol number (0 is the Upper-Left symbol). */
            sym_num = (SYM_ROWS * SYM_COLS) - sym_i * SYM_COLS - (SYM_COLS - sym_j);

            /* Determine the ASCII equivalent code for the symbol. */
            ASCII_code = 32 + sym_num;
            symbols_data[sym_num].symbol = (u8)ASCII_code;

            /* Determine the pixels coordinate of the BL corner of the symbol. */
            i_0 = img_font->height - 7 - 8 * (sym_num / SYM_COLS);
            j_0 = 7 * (sym_num % SYM_COLS);

            /* Copying the symbol from font_img to font_symbols. */
            p = 0;
            width_max = 0;
            for (i = i_0; i < (i_0 + 7); ++i) 
            {
                width_on_line = 0;
                for (j = j_0; j < (j_0 + 6); ++j) 
                {
                    /* Index of the pixel in font_img pixel array. */
                    index = i * img_font->width + j;

                    /* Write the color to font_symbols array. */
                    color = convert_RGBA_to_ARGB(*(img_font->data + index));

                    if (color == 0xff000000) /* Black color. */
                    {
                        symbols_data[sym_num].symbol_array[p] = (u8)1;
                        width_on_line = j - j_0 + 1;
                    }
                    else /* Other color (light yellow in the image). */ 
                    {
                        symbols_data[sym_num].symbol_array[p] = (u8)0;
                    }

                    /* Save the maximum line width of the symbol. */
                    if (width_on_line > width_max) 
                    {
                        width_max = width_on_line;
                    }
                    p++;
                }
            }
            
            /* Calculate the shift_left. */
            if (width_max == 0)  /* Only for the Space symbol. */ 
            {
                symbols_data[sym_num].shift_left = 3;
            }
            else 
            {
                symbols_data[sym_num].shift_left = 6 - width_max;
            }

            /* Calculate the shift_bottom for several symbols. */
            switch (symbols_data[sym_num].symbol) 
            {
                case 'g': case 'p': case 'q': case 'y': 
                {
                    symbols_data[sym_num].shift_bottom = 2;
                } break;

                case 'j': 
                {
                    symbols_data[sym_num].shift_bottom = 1;
                } break;

                default:
                {
                    symbols_data[sym_num].shift_bottom = 0;
                }
            }
        }
    }
}

void
font_draw_string(Font_t *font, char *str, s32 str_max_width, u32 x, u32 y, 
    u32 size, u32 color, Render_Buffer_t *render_buffer)
{
    u32 x_init;  /* Initial x position of the very first symbol. */
    s32 str_width;  /* Current width of the string in pixels. */
    u32 char_index;  /* Index of the char in str. */
    s32 p;  /* Index of the symbol_array in symbols_data. */
    u32 i, j;  /* Coordinates of pixels in symbol box. */
    u32 x_pos, y_pos;  /* Coordinates of pixels taking into account the pixel size. */
    Symbol_data_t *symbols_data;  /* Pointer to the font symbols data. */
    Symbol_data_t *symbol_data;  /* Pointer to the current symbol data. */
    b32 stop_print;  /* Flag to stop print the symbols. */

    symbols_data = font->symbols_data;

    x_init = x;
    char_index = 0;
    str_width = 0;
    stop_print = false;
    
    while (str[char_index] != '\0') 
    {
        if ((str_max_width > 0) && (str_width >= str_max_width - 7)) 
        {
            /* Print "..." and then stop */
            symbol_data = &(symbols_data[95]);
            stop_print = true;
        }
        else 
        {
            symbol_data = &(symbols_data[(str[char_index] - 32)]);
        }

        /* Add necessary bottom shift */
        y -= (symbol_data->shift_bottom) * size;
        
        p = 0;
        for (i = 0; i < 7; ++i) 
        {
            for (j = 0; j < 6; ++j) 
            {
                x_pos = x + j * size;
                y_pos = y + i * size;

                if (symbol_data->symbol_array[p]) 
                {
                    render_draw_rect(x_pos, y_pos, size, size, color, render_buffer);
                }
                p++;
            }
        }

        /* Restore the normal y position. */
        y += (symbol_data->shift_bottom) * size;

        /* Determine the x position of the next symbol in str. */
        x += (7 - symbol_data->shift_left) * size;
        str_width = x - x_init;
        char_index++;
        
        /* Check to exit the printing procedure. */
        if (stop_print) 
        {
            break;
        }
    }  
}
