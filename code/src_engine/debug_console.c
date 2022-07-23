/**
 * ================================================================================
 * @file debug_console.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with debug console.
 * @version 0.2
 * @date 2022-01-02
 * ================================================================================
 */

/* Standard library includes: */
#include <string.h>
#include <stdlib.h>

/* Game engine includes: */
#include <debug_console.h>
#include <utils.h>
#include <color.h>
#include <render.h>
#include <font.h>
#include <dbg.h>

DConsole_t*
dconsole_constructor(u32 messages_num, u32 max_msg_length)
{
    DConsole_t *dconsole;  /* Pointer to the debug console. */
    char *tmp_msg_str;  /* Temporary string. */
    u32 i;  /* Temporary counter. */
    
    /* Allocation the memory for the debug console object. */
    dconsole = (DConsole_t*)malloc(1 * sizeof(DConsole_t));
    
    /* Allocate the memory for the array of messages. */
    dconsole->messages_num = messages_num;
    dconsole->messages = (Message_t *)calloc(messages_num, sizeof(Message_t));

    /* Allocate the memory for message strings. */
    dconsole->max_msg_length = max_msg_length;
    for (i = 0; i < messages_num; ++i)
    {
        tmp_msg_str = (char *)calloc(max_msg_length, sizeof(char));
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
    for (i = 0; i < dconsole->messages_num; ++i)
    {
        free(dconsole->messages[i].msg_str);
    }

    /* Free memory allocated for the array of messages. */
    free(dconsole->messages);

    /* Free memory allocated for the debug console. */
    free(dconsole);
}

void
dconsole_init(DConsole_t *dconsole, u32 x, u32 y, u32 width, Color_t *bkg_color, 
    Color_t *brd_color, u32 margin_width, Color_t *game_bkg_color, Font_t *font)
{
    u32 messages_num = dconsole->messages_num;  /* Amount of the debug messages. */

    /* Initialize different DConsole fields. */
    dconsole->x = x;
    dconsole->y = y;
    dconsole->width = width;
    dconsole->height = margin_width * 2 + messages_num * 7 + \
        (messages_num - 1) * 7; /* last digit - lines intend in pixels. */
    dconsole->bkg_color = bkg_color;
    dconsole->brd_color = brd_color;
    dconsole->margin_width = margin_width;
    dconsole->game_bkg_color = game_bkg_color;
    dconsole->font = font;
    dconsole->message_index = 0;  
}

void
dconsole_add_message(DConsole_t *dconsole, char *msg_str, Color_t *color)
{
    u32 index;  /* Temporary index of the message string. */

    /* Length of the message string should be with correct size. */
    if (strlen(msg_str) > dconsole->max_msg_length)
    {
        dbg_error("%s", "Overflow maximum message length.");
    }

    index = dconsole->message_index;
    dconsole->messages[index].color = color;
    strcpy_s(dconsole->messages[index].msg_str, dconsole->max_msg_length, msg_str);
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

    for (i = 0; i < dconsole->messages_num; ++i)
    {
        dconsole->messages[i].msg_str[0] = '\0';
    }
    dconsole->message_index = 0;
}

void
dconsole_clear_console(DConsole_t *dconsole, Render_t *render)
{
    render_draw_rect_with_brd(render, dconsole->x, dconsole->y, dconsole->width, 
        dconsole->height, 1, dconsole->bkg_color, dconsole->brd_color);   
}

void
dconsole_hide(DConsole_t *dconsole, Render_t *render)
{
    render_draw_rect(render, dconsole->x , dconsole->y, dconsole->width, dconsole->height,
        dconsole->game_bkg_color);
}

void
dconsole_render(DConsole_t *dconsole, Render_t *render)
{
    u32 x_pos;  /* Temporary x ccordinate of the message string. */
    u32 y_pos;  /* Temporary y ccordinate of the message string. */
    u32 i;  /* Temporary index. */
    u32 max_width;  /* Max width of the message string that could be shown.  */
    
    x_pos = dconsole->x + dconsole->margin_width; 
    y_pos = dconsole->y + dconsole->height - dconsole->margin_width - 7;  
    max_width = dconsole->width - dconsole->margin_width * 2;
    
    dconsole_clear_console(dconsole, render);
    
    for (i = 0; i < dconsole->messages_num; ++i) 
    {
        if (dconsole->messages[i].msg_str[0] != '\0') 
        {    
            font_draw_string(dconsole->font, dconsole->messages[i].msg_str, max_width, 
                x_pos, y_pos, 1, dconsole->messages[i].color, render);
        }
        y_pos -= (7 + 7);
    }
}
