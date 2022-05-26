/**
 * ================================================================================
 * @file font.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Font class methods.
 * @version 0.2
 * @date 2022-01-09
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

#ifndef FONT_H_
#define FONT_H_

/* Program includes: */
#include <utils.h>

/* Predefined structures: */
typedef struct Render_Buffer Render_Buffer_t;
typedef struct Image Image_t;

/**
 * @brief Structure to store the symbol information. 
 */
struct Symbol_data 
{
    u8 symbol;  /**< ASCII representation of the symbol (for 32 - 126 codes). */
    u8 symbol_array[42];  /**< Array to storing the symbol pixel data. */
    u8 shift_left;  /**< How much to shift the next symbol to the left. */
    u8 shift_bottom;  /**< How much to thift the symbol to bottom. */
};
typedef struct Symbol_data Symbol_data_t;

/**
 * @brief Structure for holding the font properties.
 */
struct Font
{
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
 * @brief Extraction of the symbols data from the font image.
 * @param font Pointer to the Font structure.
 * @param img_font Pointer to the image, containing the font. 
 */
void 
font_extract_symbols(Font_t *font, Image_t *img_font);

/**
 * @brief Drawing a string using the current font.
 * @param font Pointer to the Font structure.
 * @param str String to draw.
 * @param str_max_width Maximum width of the string.
 * @param x BL corner x coordinate of the string.
 * @param y BL corner y coordinate of the string.
 * @param size Size of the string (font size).
 * @param color Color of the string.
 * @param render_buffer Pointer to the Render_buffer structure.
 */
void 
font_draw_string(Font_t *font, char *str, s32 str_max_width, u32 x, u32 y, 
    u32 size, u32 color, Render_Buffer_t *render_buffer);

#endif // FONT_H_
