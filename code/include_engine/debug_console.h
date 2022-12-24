/**
 * ================================================================================
 * @file include_engine/debug_console.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with debug
 * console.
 * @version 0.3
 * @date 2022-11-29
 * ================================================================================
 */

#ifndef JGE_ENGINE_DEBUG_CONSOLE_H_
#define JGE_ENGINE_DEBUG_CONSOLE_H_

#include "include_engine/utils.h"

/* Predefined structures: */
typedef struct Color_ Color;
typedef struct Font_ Font;
typedef struct Render_ Render;

/**
 * @brief Structure for a single message in the debug console.
 */
struct Message_ 
{
    char *msg_str;  /**< Message string. */
    Color *color;  /**< Message font color. */
};
typedef struct Message_ Message;

/**
 * @brief Structure for the DConsole object.
 */
struct DConsole_ 
{
    u32 x;  /**< BL corner x coordinate of the console. */
    u32 y;  /**< BL corner y coordinate of the console. */
    u32 height;  /**< Height of the debug console. */
    u32 width;  /**< Width of the debug console. */
    u32 messages_num;  /**< Amount of the debug messages in debug console. */
    u32 max_msg_length;  /**< Maximum length of the message. */
    Color *bkg_color;  /**< Pointer to the background color of the debug console. */
    Color *brd_color;  /**< Pointer to the console boarders color. */
    u32 margin_width;  /**< Top, left, bottom and right margins width. */
    /* TODO: Later on game_bkg_color should be deleted (use render layers). */
    Color *game_bkg_color;  /**< Pointer to the game background color. */
    u32 message_index;  /**< Pointer to the current available message line in the console. */
    Message *messages;  /**< Pointer to the debug console message array. */
    Font *font;  /**< Pointer to the font. */
};
typedef struct DConsole_ DConsole;

/**
 * @brief Object constructor.
 * @param messages_num Amount of the debug messages in debug console.
 * @param max_msg_length Maximum length of the message.
 * @return DConsole_t* Pointer to the DConsole structure. 
 */
DConsole*
DConsole_Constructor(u32 messages_num, u32 max_msg_length);

/**
 * @brief Object destructor.
 * @param dconsole Pointer to the DConsole structure.
 */
void
DConsole_Destructor(DConsole *dconsole);

/**
 * @brief Object initialization.
 * @param dconsole Pointer to the DConsole structure.
 * @param x X coordinate of the BL corner of the console. 
 * @param y Y coordinate of the BL corner of the console.
 * @param width Width of the console.
 * @param bkg_color Pointer to the background color of the debug console.
 * @param brd_color Pointer to the console boarders color.
 * @param margin_width Top, left, bottom and right margins width.
 * @param game_bkg_color Pointer to the game background color.
 * @param font Pointer to the font.
 */
void 
DConsole_Init(DConsole *dconsole, u32 x, u32 y, u32 width, Color *bkg_color, 
    Color *brd_color, u32 margin_width, Color *game_bkg_color, Font *font);

/**
 * @brief Adding a single message line to the debug console.
 * @param dconsole Pointer to the DConsole structure.
 * @param msg_str Message string.
 * @param color Pointer to the color of the message (ARGB).
 */
void 
DConsole_AddMessage(DConsole *dconsole, char *msg_str, Color *color);

/**
 * @brief Clear all messages in the debug console.
 * @param dconsole Pointer to the DConsole structure.
 */
void 
DConsole_ClearMessages(DConsole *dconsole);

/**
 * @brief Clear the debug console window. 
 * @param dconsole Pointer to the DConsole structure.
 * @param render Pointer to the Render structure.
 */
void 
DConsole_ClearConsole(DConsole *dconsole, Render *render);

/**
 * @brief Hide debug console from the game window.
 * @param dconsole Pointer to the DConsole structure.
 * @param render Pointer to the Render structure.
 */
void 
DConsole_Hide(DConsole *dconsole, Render *render);

/**
 * @brief Render the debug console with all messages in the game window. 
 * @param dconsole Pointer to the DConsole structure.
 * @param font_symbols Pointer to the font symbols.
 * @param render Pointer to the Render_Buffer structure.
 */
void 
DConsole_Render(DConsole *dconsole, Render *render);

#endif  /* JGE_ENGINE_DEBUG_CONSOLE_H_ */
