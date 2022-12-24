/**
 * ================================================================================
 * @file include_engine/font.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with font.
 * @version 0.2
 * @date 2022-12-04
 * ================================================================================
 */

#ifndef JEMA_ENGINE_FONT_H_
#define JEMA_ENGINE_FONT_H_

#include "include_engine/utils.h"

typedef struct Render_ Render;
typedef struct Image_ Image;
typedef struct Color_ Color;

/**
 * @brief Structure to store the symbol information. 
 */
struct SymbolData_ 
{
    u8 symbol;  /**< Representation of the symbol in Win1251 encoding. */
    u8 symbol_array[42];  /**< Array to storing the symbol pixel data. */
    u32 shift_left;  /**< How much to shift the next symbol to the left. */
    u32 shift_bottom;  /**< How much to thift the symbol to bottom. */
};
typedef struct SymbolData_ SymbolData;

/**
 * @brief Structure for holding the font properties.
 */
struct Font_
{
    s32 rows_num;  /**< Number of rows of symbols in the font image file. */
    s32 cols_num;  /**< Number of columns of symbols in the font image file. */
    s32 sym_width;  /**< Symbol width in pixels (6).  */
    s32 sym_height;  /**< Symbol height in pixels (7). */
    SymbolData *symbols_data;  /**< Pointer to the font symbols data. */
};
typedef struct Font_ Font;

/**
 * @brief Object constructor.
 * @return Font* Pointer to the Font structure.
 */
Font*
Font_Constructor(void);

/**
 * @brief Object destructor.
 * @param font Pointer to the Font structure.
 */
void
Font_Destructor(Font *font);

/**
 * @brief Object initialization. Extraction of the symbols data from the font image.
 * @param font Pointer to the Font structure.
 * @param rows_num Number of rows of symbols in the font image file.
 * @param cols_num Number of columns of symbols in the font image file.
 * @param sym_width Symbol width in pixels.
 * @param sym_height Symbol height in pixels.
 * @param img_font Pointer to the image containing the font.
 */
void 
Font_Init(Font *font, s32 rows_num, s32 cols_num, s32 sym_width, s32 sym_height, 
    Image *img_font);

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
Font_DrawString(Font *font, char *str, s32 str_max_width, u32 x, u32 y, 
    u32 size, Color *color, Render *render);

#endif  /* JEMA_ENGINE_FONT_H_ */
