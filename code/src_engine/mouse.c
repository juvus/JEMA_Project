/**
 * ================================================================================
 * @file src_engine/mouse.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with mouse input to
 * the game.
 * @version 0.2
 * @date 2022-12-04
 * ================================================================================
 */

#include "include_engine/mouse.h"

#include "include_engine/dbg.h"
#include "include_engine/render.h"
#include "include_engine/utils.h"
#include "include_engine/vector2.h"

Mouse*
Mouse_Constructor(void)
{
    Mouse *mouse = (Mouse *)malloc(1 * sizeof(Mouse));
    if (mouse == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return mouse;
}

void
Mouse_Destructor(Mouse *mouse)
{
    if (mouse == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    free(mouse);
    mouse = NULL;
}

void
Mouse_PrepareInput(Mouse *mouse, Render *render)
{
    mouse->cur_x = (u32)mouse->raw_cursor.x;
    mouse->cur_y = render->height - (u32)mouse->raw_cursor.y;
}