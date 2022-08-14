/**
 * ================================================================================
 * @file game_worker.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with game worker. Worker
 * procedure is executes in a separate thread.
 * the game.
 * @version 0.1
 * @date 2022-07-22
 * ================================================================================ 
 */

/* Standard library includes: */
#include <windows.h>

/* Game engine includes: */
#include "game.h"
#include "game_worker.h"
#include "keyboard.h"
#include "mouse.h"
#include "render.h"
#include "utils.h"

Game_Worker_t*
game_worker_constructor(void)
{
    Game_Worker_t *game_worker;  /* Pointer to the Game_Worker structure. */

    /* Allocate the memory for the Game_Worker structure. */
    game_worker = (Game_Worker_t *)malloc(1 * sizeof(Game_Worker_t));
    
    return game_worker;
}

void
game_worker_destructor(Game_Worker_t *game_worker)
{
    /* Release memory allocated for the Game_Worker structure. */
    free(game_worker);
    game_worker = NULL;
}

void
game_worker_init(Game_Worker_t *game_worker, Game_t *game, Keyboard_t *keyboard,
    Mouse_t *mouse, Render_t *render)
{
    game_worker->game = game;
    game_worker->keyboard = keyboard;
    game_worker->mouse = mouse;
    game_worker->render = render;
}

DWORD WINAPI 
game_worker_proc(void *game_worker)
{
    Game_t *game;  /* Pointer to the Game structure. */
    Keyboard_t *keyboard;  /* Pointer to the Keyboard structure. */
    Mouse_t *mouse;  /* Pointer to the Mouse structure. */ 
    Render_t *render;  /* Pointer to the Render structure. */ 
    
    game = ((Game_Worker_t *)game_worker)->game;
    keyboard = ((Game_Worker_t *)game_worker)->keyboard;
    mouse = ((Game_Worker_t *)game_worker)->mouse;
    render = ((Game_Worker_t *)game_worker)->render;

    while (game->is_running)
    {
        game_simulate_tick(game, keyboard, mouse, render);
        render_update_window(render);
        Sleep(1);
    }
    return(0);
}
