/**
 * ================================================================================
 * @file src_game/game_worker.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with game worker. Worker
 * procedure is executes in a separate thread.
 * the game.
 * @version 0.2
 * @date 2022-12-23
 * ================================================================================ 
 */

#include "include_game/game_worker.h"

#include <windows.h>

#include "include_engine/dbg.h"
#include "include_engine/render.h"
#include "include_engine/utils.h"
#include "include_engine/win32_platform.h"
#include "include_game/game.h"

GameWorker*
GameWorker_Constructor(void)
{
    GameWorker *game_worker = (GameWorker *)malloc(1 * sizeof(GameWorker));
    if (game_worker == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return game_worker;
}

void
GameWorker_Destructor(GameWorker *game_worker)
{
    if (game_worker == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    free(game_worker);
    game_worker = NULL;
}

void
GameWorker_Init(GameWorker *game_worker, Game *game, Win32Platform *win32_platform)
{
    game_worker->game = game;
    game_worker->win32_platform = win32_platform;
}

DWORD WINAPI 
GameWorker_ThreadProc(void *game_worker)
{
    Game *game;  /* Pointer to the Game structure. */
    Win32Platform *win32_platform;  /* Pointer to the Win32Platform structure. */
    
    game = ((GameWorker *)game_worker)->game;
    win32_platform = ((GameWorker *)game_worker)->win32_platform;
    
    while (game->is_running)
    {
        Game_SimulateTick(game, win32_platform);
        Render_UpdateWindow(win32_platform->render);
        Sleep(1);
    }
    return(0);
}
