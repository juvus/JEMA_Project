/**
 * ================================================================================
 * @file debug_console.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the debug console methods.
 * @version 0.2
 * @date 2022-01-02
 * @copyright JuvuSoft (c) 2021
 * ================================================================================
 */

/* Standard includes: */
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/* Program includes: */
#include <debug_console.h>
#include <utils.h>
#include <render.h>
#include <font.h>

/* Global static variables (parameters of the console): */
extern u32 DCONSOLE_MESSAGES; /* Amount of the debug messages in debug console */
extern u32 DCONSOLE_MAX_MSG_LENGTH; /* Maximum message length */
extern u32 DCONSOLE_BKG_COLOR; /* Background color of the debug console */
extern u32 DCONSOLE_BRD_COLOR; /* Console boarders color */
extern u32 DCONSOLE_MARGINS; /* Top, left, bottom and right margins */
extern u32 BKG_COLOR; /* Background color */

DConsole_t*
dconsole_constructor(void)
{
    DConsole_t *dconsole = NULL;  /* Pointer to the debug console. */
    Message_t *messages = NULL;;  /* Pointer to the console messages array. */ 
    char *tmp_msg_str = NULL;;  /* Temporary string. */
    u32 i;  /* Temporary counter. */
    
    /* Allocation the memory for the debug console object. */
    dconsole = (DConsole_t*)malloc(1 * sizeof(DConsole_t));
    
    /* Allocate the memory for the array of messages. */
    messages = (Message_t *) calloc (DCONSOLE_MESSAGES, sizeof(Message_t));
    dconsole->messages = messages;

    /* Allocate the memory for message strings. */
    for (i = 0; i < DCONSOLE_MESSAGES; ++i)
    {
        tmp_msg_str = (char *) calloc (DCONSOLE_MAX_MSG_LENGTH, sizeof(char));
        dconsole->messages[i].msg_str = tmp_msg_str;
        dconsole->messages[i].msg_str[0] = '\0';
    }
    return dconsole;
}

void
dconsole_destructor(DConsole_t *dconsole)
{
    u32 i;  /* Temporary counter. */
    
    /* Free memory allocated for message strings. */
    for (i = 0; i < DCONSOLE_MESSAGES; ++i)
    {
        free(dconsole->messages[i].msg_str);
    }

    /* Free memory allocated for message structures. */
    free(dconsole->messages);

    /* Free memory allocated for the debug console. */
    free(dconsole);
}

void
dconsole_init(DConsole_t *dconsole, u32 x, u32 y, u32 width, Font_t *font)
{
    /* Calculate and save console parameters. */
    dconsole->x = x;
    dconsole->y = y;
    dconsole->width = width;
    dconsole->height = DCONSOLE_MARGINS * 2 + DCONSOLE_MESSAGES * 7 + \
        (DCONSOLE_MESSAGES - 1) * 7; /* last digit - lines intend in pixels */
    dconsole->font = font;
    dconsole->message_index = 0;  
}

void
dconsole_add_message(DConsole_t *dconsole, char *msg_str, u32 color)
{
    u32 index;  /* Temporary index of the message string. */

    /* Length of the message string should be with correct size. */
    assert(strlen(msg_str) <= DCONSOLE_MAX_MSG_LENGTH);

    index = dconsole->message_index;
    dconsole->messages[index].color = color;
    strcpy_s(dconsole->messages[index].msg_str, DCONSOLE_MAX_MSG_LENGTH, msg_str);
    dconsole->message_index++;
    
    /* For safe in case of adding more messages than possible to store */
    if (dconsole->message_index >= 10) 
    {
        dconsole->message_index = 0;
    }
}

void
dconsole_clear_messages(DConsole_t *dconsole)
{
    u32 i;  /* Temporary index. */

    for (i = 0; i < DCONSOLE_MESSAGES; ++i)
    {
        dconsole->messages[i].msg_str[0] = '\0';
    }
    dconsole->message_index = 0;
}

void
dconsole_clear_console(DConsole_t *dconsole, Render_Buffer_t *render_buffer)
{
    render_draw_rect_with_brd(dconsole->x, dconsole->y, dconsole->width, 
        dconsole->height, 1, DCONSOLE_BKG_COLOR, DCONSOLE_BRD_COLOR, 
        render_buffer);   
}

void
dconsole_hide(DConsole_t *dconsole, Render_Buffer_t *render_buffer)
{
    render_draw_rect(dconsole->x , dconsole->y, dconsole->width, dconsole->height,
        BKG_COLOR, render_buffer);
}

void
dconsole_render(DConsole_t *dconsole, Render_Buffer_t *render_buffer)
{
    u32 x_pos;  /* Temporary x ccordinate of the message string. */
    u32 y_pos;  /* Temporary y ccordinate of the message string. */
    u32 i;  /* Temporary index. */
    u32 max_width;  /* Max width of the message string that could be shown.  */
    
    x_pos = dconsole->x + DCONSOLE_MARGINS; 
    y_pos = dconsole->y + dconsole->height - DCONSOLE_MARGINS - 7;  
    max_width = dconsole->width - DCONSOLE_MARGINS * 2;
    
    dconsole_clear_console(dconsole, render_buffer);
    
    for (i = 0; i < DCONSOLE_MESSAGES; ++i) 
    {
        if (dconsole->messages[i].msg_str[0] != '\0') 
        {    
            font_draw_string(dconsole->font, dconsole->messages[i].msg_str, max_width, 
                x_pos, y_pos, 1, dconsole->messages[i].color, render_buffer);
        }
        y_pos -= (7 + 7);
    }
}
