/**
 * ================================================================================
 * @file font.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with font.
 * @version 0.2
 * @date 2022-07-08
 * ================================================================================
 */

#ifndef FONT_H_
#define FONT_H_

/* Game engine includes: */
#include "utils.h"

/* Predefined structures: */
typedef struct Render Render_t;
typedef struct Image Image_t;
typedef struct Color Color_t;

/**
 * @brief Structure to store the symbol information. 
 */
struct Symbol_data 
{
    u8 symbol;  /**< Representation of the symbol in Win1251 encoding. */
    u8 symbol_array[42];  /**< Array to storing the symbol pixel data. */
    u32 shift_left;  /**< How much to shift the next symbol to the left. */
    u32 shift_bottom;  /**< How much to thift the symbol to bottom. */
};
typedef struct Symbol_data Symbol_data_t;

/**
 * @brief Structure for holding the font properties.
 */
struct Font
{
    s32 rows_num;  /**< Number of rows of symbols in the font image file. */
    s32 cols_num;  /**< Number of columns of symbols in the font image file. */
    s32 sym_width;  /**< Symbol width in pixels (6).  */
    s32 sym_height;  /**< Symbol height in pixels (7). */
    Symbol_data_t *symbols_data;  /**< Pointer to the font symbols data. */
};
typedef struct Font Font_t;

/**
 * @brief Constructor of the Font class.
 * @return Font_t* Pointer to the Font structure.
 */
Font_t*
font_constructor(void);

/**
 * @brief Destructor of the Font class.
 * @param font Pointer to the Font structure.
 */
void
font_destructor(Font_t *font);

/**
 * @brief Font initialization. Extraction of the symbols data from the font image.
 * @param font Pointer to the Font structure.
 * @param rows_num Number of rows of symbols in the font image file.
 * @param cols_num Number of columns of symbols in the font image file.
 * @param sym_width Symbol width in pixels.
 * @param sym_height Symbol height in pixels.
 * @param img_font Pointer to the image containing the font.
 */
void 
font_init(Font_t *font, s32 rows_num, s32 cols_num, s32 sym_width, s32 sym_height, 
    Image_t *img_font);

/**
 * @brief Drawing a string using the current font.
 * @param font Pointer to the Font structure.
 * @param str String to draw.
 * @param str_max_width Maximum width of the string.
 * @param x BL corner x coordinate of the string.
 * @param y BL corner y coordinate of the string.
 * @param size Size of the string (font size).
 * @param color Color of the string.
 * @param render Pointer to the Render structure.
 */
void 
font_draw_string(Font_t *font, char *str, s32 str_max_width, u32 x, u32 y, 
    u32 size, Color_t *color, Render_t *render);

#endif // FONT_H_
