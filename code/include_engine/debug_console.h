/**
 * ================================================================================
 * @file debug_console.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with debug
 * console.
 * @version 0.2
 * @date 2022-01-04
 * ================================================================================
 */

#ifndef DEBUG_CONSOLE_H_
#define DEBUG_CONSOLE_H_

/* Program includes: */
#include <utils.h>

/* Predefined structures: */
typedef struct Render Render_t;
typedef struct Font Font_t;
typedef struct Color Color_t;

/**
 * @brief Structure for a single message in the debug console.
 */
struct Message 
{
    char *msg_str;  /**< Message string. */
    Color_t *color;  /**< Message font color. */
};
typedef struct Message Message_t;

/**
 * @brief Structure for the DConsole object.
 */
struct DConsole 
{
    u32 x;  /**< BL corner x coordinate of the console. */
    u32 y;  /**< BL corner y coordinate of the console. */
    u32 height;  /**< Height of the debug console. */
    u32 width;  /**< Width of the debug console. */
    u32 messages_num;  /**< Amount of the debug messages in debug console. */
    u32 max_msg_length;  /**< Maximum length of the message. */
    Color_t *bkg_color;  /**< Pointer to the background color of the debug console. */
    Color_t *brd_color;  /**< Pointer to the console boarders color. */
    u32 margin_width;  /**< Top, left, bottom and right margins width. */
    /* TODO: Later on game_bkg_color should be deleted (use render layers). */
    Color_t *game_bkg_color;  /**< Pointer to the game background color. */
    u32 message_index;  /**< Pointer to the current available message line in the console. */
    Message_t *messages;  /**< Pointer to the debug console message array. */
    Font_t *font;  /**< Pointer to the font. */
};
typedef struct DConsole DConsole_t;

/**
 * @brief Constructor of the DConsole class.
 * @param messages_num Amount of the debug messages in debug console.
 * @param max_msg_length Maximum length of the message.
 * @return DConsole_t* Pointer to the DConsole structure. 
 */
DConsole_t*
dconsole_constructor(u32 messages_num, u32 max_msg_length);

/**
 * @brief Destructor of the DConsole object.
 * @param dconsole Pointer to the DConsole structure.
 */
void
dconsole_destructor(DConsole_t *dconsole);

/**
 * @brief Initialization of the debug console.
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
dconsole_init(DConsole_t *dconsole, u32 x, u32 y, u32 width, Color_t *bkg_color, 
    Color_t *brd_color, u32 margin_width, Color_t *game_bkg_color, Font_t *font);

/**
 * @brief Adding a single message line to the debug console.
 * @param dconsole Pointer to the DConsole structure.
 * @param msg_str Message string.
 * @param color Pointer to the color of the message (ARGB).
 */
void 
dconsole_add_message(DConsole_t *dconsole, char *msg_str, Color_t *color);

/**
 * @brief Clear all messages in the debug console.
 * @param dconsole Pointer to the DConsole structure.
 */
void 
dconsole_clear_messages(DConsole_t *dconsole);

/**
 * @brief Clear the debug console window. 
 * @param dconsole Pointer to the DConsole structure.
 * @param render Pointer to the Render structure.
 */
void 
dconsole_clear_console(DConsole_t *dconsole, Render_t *render);

/**
 * @brief Hide debug console from the game window.
 * @param dconsole Pointer to the DConsole structure.
 * @param render Pointer to the Render structure.
 */
void 
dconsole_hide(DConsole_t *dconsole, Render_Buffer_t *render);

/**
 * @brief Render the debug console with all messages in the game window. 
 * @param dconsole Pointer to the DConsole structure.
 * @param font_symbols Pointer to the font symbols.
 * @param render Pointer to the Render_Buffer structure.
 */
void 
dconsole_render(DConsole_t *dconsole, Render_t *render);

#endif // DEBUG_CONSOLE_H_
