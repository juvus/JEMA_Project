/**
 * ================================================================================
 * @file src_engine/font.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with font.
 * @version 0.3
 * @date 2022-12-04
 * ================================================================================
 */

#include "include_engine/font.h"

#include "include_engine/color.h"
#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/image.h"
#include "include_engine/render.h"
#include "include_engine/utils.h"

Font*
Font_Constructor(void)
{
    size_t size = sizeof(Font);
    Font *font = (Font *)HelperFcn_MemAllocate(size);
    return font;
}

Font*
Font_Destructor(Font *font)
{
    HelperFcn_MemFree(font->symbols_data);
    HelperFcn_MemFree(font);
    return NULL;
}

void
Font_Init(Font *font, s32 rows_num, s32 cols_num, s32 sym_width, s32 sym_height, 
    Image *img_font)
{
    /* Allocate memory for the all symbols data. */
    SymbolData *symbols_data = (SymbolData *)calloc(rows_num * cols_num, 
        sizeof(SymbolData));
    font->symbols_data = symbols_data;
    font->rows_num = rows_num;
    font->cols_num = cols_num;
    font->sym_width = sym_width;
    font->sym_height = sym_height;

    /* Extract symbols from the font image file. */
    for (s32 sym_i = (rows_num - 1); sym_i >= 0; --sym_i) 
    {
        for (s32 sym_j = 0; sym_j < cols_num; ++sym_j) 
        {
            /* Determine the current symbol Win1251 code (0 is for upper-left symbol). */
            s32 win1251_code = (rows_num * cols_num) - sym_i * cols_num - (cols_num - sym_j);
            symbols_data[win1251_code].symbol = (u8)win1251_code;

            /* Determine the pixels coordinate of the BL corner of the symbol. Aslo take 
            into account the title of the table. */
            s32 i_0 = img_font->height - (sym_height + 1) * (win1251_code / cols_num) \
                - sym_height - (sym_height + 1);
            s32 j_0 = (sym_width + 1) * (win1251_code % cols_num) + (sym_width + 1);

            /* Copying the symbol from font_img to font_symbols. */
            u32 p = 0;  /* Index in the symbol_array. */
            u32 width_max = 0;  /* Maximum width of the current symbol. */
            u32 shift_left = 0;
            u32 shift_bottom = 0;
            
            for (s32 i = i_0; i < (i_0 + sym_height); ++i) 
            {
                u32 width_on_line = 0;  /* Width of the symbol on current line. */
                for (s32 j = j_0; j < (j_0 + sym_width); ++j) 
                {
                    Color color;
                    u32 black = 0xff000000;  /* Black color (pixel of the font symbol). */
                    u32 red = 0xffff0000;  /* Red color (defines shift to bottom). */
                    u32 index = i * img_font->width + j;   
                    
                    Color_SetFromImageColorData(&color, *(img_font->data + index));
                    if (color.color == black)
                    {
                        symbols_data[win1251_code].symbol_array[p] = (u8)1;
                        width_on_line = j - j_0 + 1;
                    }
                    else if (color.color == red)
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
Font_DrawString(Font *font, char *str, s32 str_max_width, u32 x, u32 y, 
    u32 size, Color *color, Render *render)
{
    SymbolData *symbols_data = font->symbols_data;
    u32 sym_width = font->sym_width;
    u32 sym_height = font->sym_height;
    u32 x_init = x;  /* Initial x position of the very first symbol. */
    u32 char_index = 0;  /* Index of the char in str. */
    u32 str_width = 0;  /* Current width of the string in pixels. */
    b32 stop_print = false;  /* Flag to stop print the symbols. */
    
    while (str[char_index] != '\0') 
    {
        SymbolData *symbol;  /* Pointer to the current symbol. */
       
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
        
        u32 p = 0;
        for (u32 i = 0; i < sym_height; ++i) 
        {
            for (u32 j = 0; j < sym_width; ++j) 
            {
                u32 x_pos = x + j * size;
                u32 y_pos = y + i * size;

                if (symbol->symbol_array[p]) 
                {
                    Render_DrawRect(render, x_pos, y_pos, size, size, color);
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
