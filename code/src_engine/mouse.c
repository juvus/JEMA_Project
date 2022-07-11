/**
 * ================================================================================
 * @file mouse.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with mouse input to
 * the game.
 * @version 0.2
 * @date 2022-07-11
 * ================================================================================
 */

/* Game engine includes: */
#include <mouse.h>
#include <render.h>
#include <utils.h>

Mouse_t*
mouse_constructor(void)
{
    Mouse_t *mouse;  /* Pointer to the Mouse structure. */

    /* Allocation the memory for the Mouse structure. */
    mouse = (Mouse_t*)malloc(1 * sizeof(Mouse_t));
    return mouse;
}

void
mouse_destructor(Mouse_t *mouse)
{
    /* Free memory allocated for the Mouse structure. */
    free(mouse);
    mouse = NULL;
}

void
mouse_input_prepare_input(Mouse_t *mouse, Render_t *render)
{
    mouse->cursor.x = mouse->raw_cursor.x;
    mouse->cursor.y = render->height - mouse->raw_cursor.y;
}