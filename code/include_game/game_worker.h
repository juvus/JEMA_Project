/**
 * ================================================================================
 * @file game_worker.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with game
 *  worker. Worker procedure is executes in a separate thread.
 * @version 0.1
 * @date 2022-07-22
 * ================================================================================
 */

#ifndef GAME_WORKER_H_
#define GAME_WORKER_H_

/* Standard library includes. */
#include <windows.h>

/* Game engine includes. */
#include "utils.h"

/* Predefined structures: */
typedef struct Game Game_t;
typedef struct Keyboard Keyboard_t;
typedef struct Mouse Mouse_t;
typedef struct Render Render_t;

/**
 * @brief Structure with data, that need to be pased to the thread (multithreading).
 */
struct Game_Worker
{
    Game_t *game;  /**< Pointer to the Game structure. */
    Keyboard_t *keyboard;  /**< Pointer to the Keyboard structure. */
    Mouse_t *mouse;  /**< Pointer to the Mouse structure. */
    Render_t *render;  /**< Pointer to the Render structure. */
};
typedef struct Game_Worker Game_Worker_t;

/**
 * @brief Constructor of the Game_Worker class.
 * @return Game_Worker_t* Pointer to the Game_Worker structure. 
 */
Game_Worker_t*
game_worker_constructor(void);

/**
 * @brief Destructor of the Game_Worker object.
 * @param game_worker Pointer to the Game_Worker structure.
 */
void
game_worker_destructor(Game_Worker_t *game_worker);

/**
 * @brief Initialization of the Game_Worker object.
 * @param game_worker Pointer to the Game_Worker structure.
 */
void
game_worker_init(Game_Worker_t *game_worker, Game_t *game, Keyboard_t *keyboard,
    Mouse_t *mouse, Render_t *render);

/**
 * @brief Game worker procedure to be executed in the separate thread. 
 */
DWORD WINAPI 
game_worker_proc(void *game_worker);

#endif //GAME_WORKER_H_
