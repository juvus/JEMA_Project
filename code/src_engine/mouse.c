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
#include "include_engine/helper_functions.h"
#include "include_engine/render.h"
#include "include_engine/utils.h"
#include "include_engine/vector2.h"

Mouse*
Mouse_Constructor(void)
{
    size_t size = sizeof(Mouse);
    Mouse *mouse = (Mouse *)HelperFcn_MemAllocate(size);
    return mouse;
}

Mouse*
Mouse_Destructor(Mouse *mouse)
{
    HelperFcn_MemFree(mouse);
    return NULL;
}

void
Mouse_PrepareInput(Mouse *mouse, Render *render)
{
    mouse->cur_x = (u32)mouse->raw_cursor.x;
    mouse->cur_y = render->height - (u32)mouse->raw_cursor.y;
}