/**
 * ================================================================================
 * @file debug_console.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the debug console class.
 * @version 0.2
 * @date 2022-01-04
 * @copyright JuvuSoft (c) 2021
 * ================================================================================
 */

#ifndef DEBUG_CONSOLE_H_
#define DEBUG_CONSOLE_H_

/* Program includes: */
#include <utils.h>

/* Predefined structures: */
typedef struct Render_Buffer Render_Buffer_t;
typedef struct Font Font_t;

/**
 * @brief Structure for a single message in the debug console.
 */
struct Message 
{
    char *msg_str;  /**< Message string. */
    u32 color;  /**< Message font color. */
};
typedef struct Message Message_t;

/**
 * @brief Structure for the DConsole properties.
 */
struct DConsole 
{
    u32 x;  /**< BL corner x coordinate of the console. */
    u32 y;  /**< BL corner y coordinate of the console. */
    u32 height;  /**< Height, that  will be determined using DCONSOLE_MESSAGES. */ 
    u32 width;  /**< Width of the debug console. */
    u32 message_index;  /**< Pointer to the current available message line in the console. */
    Message_t *messages;  /**< Pointer to the debug console message array. */
    Font_t *font;  /**< Pointer to the font. */
};
typedef struct DConsole DConsole_t;

/**
 * @brief Constructor of the DConsole class.
 * @return DConsole_t* Pointer to the DConsole structure. 
 */
DConsole_t*
dconsole_constructor(void);

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
 * @param font Pointer to the font.
 */
void 
dconsole_init(DConsole_t *dconsole, u32 x, u32 y, u32 width, Font_t *font);

/**
 * @brief Adding a single message line to the debug console.
 * @param dconsole Pointer to the DConsole structure.
 * @param msg_str Message string.
 * @param color Color of the message (ARGB).
 */
void 
dconsole_add_message(DConsole_t *dconsole, char *msg_str, u32 color);

/**
 * @brief Clear all messages in the debug console.
 * @param dconsole Pointer to the DConsole structure.
 */
void 
dconsole_clear_messages(DConsole_t *dconsole);

/**
 * @brief Clear the debug console window. 
 * @param dconsole Pointer to the DConsole structure.
 * @param render_buffer Pointer to the Render_Buffer structure.
 */
void 
dconsole_clear_console(DConsole_t *dconsole, Render_Buffer_t *render_buffer);

/**
 * @brief Hide debug console from the game window.
 * @param dconsole Pointer to the DConsole structure.
 * @param render_buffer Pointer to the Render_Buffer structure.
 */
void 
dconsole_hide(DConsole_t *dconsole, Render_Buffer_t *render_buffer);

/**
 * @brief Render the debug console with all messages in the game window. 
 * @param dconsole Pointer to the DConsole structure.
 * @param font_symbols Pointer to the font symbols.
 * @param render_buffer Pointer to the Render_Buffer structure.
 */
void 
dconsole_render(DConsole_t *dconsole, Render_Buffer_t *render_buffer);

#endif // DEBUG_CONSOLE_H_
