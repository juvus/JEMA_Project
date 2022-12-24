/**
 * ================================================================================
 * @file src_engine/debug_console.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with debug console.
 * @version 0.3
 * @date 2022-11-30
 * ================================================================================
 */

#include "include_engine/debug_console.h"

#include <stdlib.h>
#include <string.h>

#include "include_engine/color.h"
#include "include_engine/dbg.h"
#include "include_engine/font.h"
#include "include_engine/render.h"
#include "include_engine/utils.h"

DConsole*
DConsole_Constructor(u32 messages_num, u32 max_msg_length)
{    
    /* Allocate memory for the debug console object. */
    DConsole *dconsole = (DConsole *)malloc(1 * sizeof(DConsole));
    if (dconsole == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }

    /* Allocate the memory for the array of messages. */
    dconsole->messages_num = messages_num;
    dconsole->messages = (Message *)calloc(messages_num, sizeof(Message));
    if (dconsole->messages == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }

    /* Allocate the memory for message strings. */
    dconsole->max_msg_length = max_msg_length;
    for (u32 i = 0; i < messages_num; ++i)
    {
        char *tmp_msg_str = (char *)calloc(max_msg_length, sizeof(char));
        if (tmp_msg_str == NULL)
        {
            dbg_error("%s", "Memory allocation error!");
        }    
        dconsole->messages[i].msg_str = tmp_msg_str;
        dconsole->messages[i].msg_str[0] = '\0';
    }
    return dconsole;
}

void
DConsole_Destructor(DConsole *dconsole)
{
    if ((dconsole == NULL) || (dconsole->messages == NULL))
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    
    for (u32 i = 0; i < dconsole->messages_num; ++i)
    {
        if (dconsole->messages[i].msg_str == NULL)
        {
            dbg_error("%s", "Attempt to delete an empty object!");
        }
        free(dconsole->messages[i].msg_str);
    }
    free(dconsole->messages);
    free(dconsole);
    dconsole = NULL;
}

void
DConsole_Init(DConsole *dconsole, u32 x, u32 y, u32 width, Color *bkg_color, 
    Color *brd_color, u32 margin_width, Color *game_bkg_color, Font *font)
{
    /* Initialize different DConsole fields. */
    dconsole->x = x;
    dconsole->y = y;
    dconsole->width = width;
    u32 messages_num = dconsole->messages_num;
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
DConsole_AddMessage(DConsole *dconsole, char *msg_str, Color *color)
{
    /* Length of the message string should be with correct size. */
    if (strlen(msg_str) > dconsole->max_msg_length)
    {
        dbg_error("%s", "Overflow maximum message length.");
    }

    u32 index = dconsole->message_index;
    dconsole->messages[index].color = color;
    strcpy_s(dconsole->messages[index].msg_str, dconsole->max_msg_length, msg_str);
    dconsole->message_index++;
    
    /* For safe in case of adding more messages than possible to store */
    if (dconsole->message_index >= dconsole->messages_num) 
    {
        dconsole->message_index = 0;
    }
}

void
DConsole_ClearMessages(DConsole *dconsole)
{
    for (u32 i = 0; i < dconsole->messages_num; ++i)
    {
        dconsole->messages[i].msg_str[0] = '\0';
    }
    dconsole->message_index = 0;
}

void
DConsole_ClearConsole(DConsole *dconsole, Render *render)
{
    Render_DrawRectWithBrd(render, dconsole->x, dconsole->y, dconsole->width, 
        dconsole->height, 1, dconsole->bkg_color, dconsole->brd_color);   
}

void
DConsole_Hide(DConsole *dconsole, Render *render)
{
    Render_DrawRect(render, dconsole->x , dconsole->y, dconsole->width, dconsole->height,
        dconsole->game_bkg_color);
}

void
DConsole_Render(DConsole *dconsole, Render *render)
{
    u32 x_pos = dconsole->x + dconsole->margin_width; 
    u32 y_pos = dconsole->y + dconsole->height - dconsole->margin_width - 7;  
    u32 max_width = dconsole->width - dconsole->margin_width * 2;
    
    DConsole_ClearConsole(dconsole, render);
    
    for (u32 i = 0; i < dconsole->messages_num; ++i) 
    {
        if (dconsole->messages[i].msg_str[0] != '\0') 
        {    
            Font_DrawString(dconsole->font, dconsole->messages[i].msg_str, max_width, 
                x_pos, y_pos, 1, dconsole->messages[i].color, render);
        }
        y_pos -= (7 + 7);
    }
}
