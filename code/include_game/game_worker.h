/**
 * ================================================================================
 * @file include_game/game_worker.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with game
 *  worker. Worker procedure is executes in a separate thread.
 * @version 0.2
 * @date 2022-12-23
 * ================================================================================
 */

#ifndef JEMA_GAME_WORKER_H_
#define JEMA_GAME_WORKER_H_

#include <windows.h>
#include "include_engine/utils.h"

typedef struct Game_ Game;
typedef struct Win32Platform_ Win32Platform;

/**
 * @brief Structure with data, that need to be pased to the thread (multithreading).
 */
struct GameWorker_
{
    Game *game;  /**< Pointer to the Game structure. */
    Win32Platform *win32_platform;  /**< Pointer to the Win32Platform structure. */
};
typedef struct GameWorker_ GameWorker;

/**
 * @brief Object constructor.
 * @return GameWorker* Pointer to the GameWorker structure.
 */
GameWorker*
GameWorker_Constructor(void);

/**
 * @brief Object destructor.
 * @param game_worker Pointer to the GameWorker structure.
 * @return GameWorker* Pointer to the GameWorker structure.
 */
GameWorker*
GameWorker_Destructor(GameWorker *game_worker);

/**
 * @brief Initialization of the GameWorker object.
 * @param game_worker Pointer to the GameWorker structure.
 */
void
GameWorker_Init(GameWorker *game_worker, Game *game, Win32Platform *win32_platform);

/**
 * @brief Game worker procedure to be executed in the separate thread. 
 */
DWORD WINAPI 
GameWorker_ThreadProc(void *game_worker);

#endif  /* JEMA_GAME_WORKER_H_ */
