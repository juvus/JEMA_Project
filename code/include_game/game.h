/**
 * ================================================================================
 * @file include_game/game.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the game.
 * @version 0.3
 * @date 2022-12-23
 * ================================================================================
 */

#ifndef JEMA_GAME_H_
#define JEMA_GAME_H_

#include <windows.h>
#include "include_engine/utils.h"

typedef struct Win32Platform_ Win32Platform;

/**
 * @brief Enumerator for different states of the game. 
 */
enum GameState_
{
    GST_OBJECTS_DEF,  /**< State for definition of engine objects. */
    GST_MEMORY_ALLOCATION,  /**< State for dynamic memory allocation. */
    GST_LOAD_RESOURCES,  /**< State for loading different game resources. */
    GST_INITIALIZATION,  /**< State for the game initialization procedure. */
    GST_INIT_RENDER,  /**< State for the initial render procedure. */
    GST_PROCESS_GAME_TICK,  /**< State for processing game ticks. */
    GST_FINALIZATION,  /**< State for finalization procedure. */
    GST_RELEASE_MEMORY   /**< State for free the allocated dynamic memory. */
};
typedef enum GameState_ GameState;

/**
 * @brief Enumerator for the different game modes. 
 */
enum GameMode_
{
    GM_NORMAL,  /**< Normal game mode. */
    GM_IMMORTAL /**< Immortality mode for testing. */
};
typedef enum GameMode_ GameMode;

/**
 * @brief Structure for the Game properties.
 */
struct Game_
{
    LARGE_INTEGER begin_counter;  /**< Union for for dtime calculation. */
    LARGE_INTEGER end_counter;  /**< Union for for dtime calculation. */
    LARGE_INTEGER frequency_counter;  /**< Union for for dtime calculation. */

    f32 dtime; /**< Time delta between the game ticks (in sec). */
    f32 cum_time;  /**< Cumulative time of the game (in sec). */
    b32 is_running;  /**< Flag showing is the main loop is running or not. */
    GameState game_state;  /**< Current state of the game. */
    GameMode game_mode;  /**< Current game mode. */
};
typedef struct Game_ Game; 

/**
 * @brief Object constructor.
 * @return Game* Pointer to the Game structure.
 */
Game*
Game_Constructor(void);

/**
 * @brief Object destructor.
 * @param game Pointer to the Game structure.
 * @return Game* Pointer to the Game structure.
 */
Game*
Game_Destructor(Game *game);

/**
 * @brief Initialization of the Game object.
 * @param game Pointer to the Game structure.
 * @param game_state Initial game state.
 * @param game_mode Game mode.
 */
void
Game_Init(Game *game, GameState game_state, GameMode game_mode);

/**
 * @brief Simulation of a single game tick.
 * @param game Pointer to the Game structure.
 * @param win32_platform Pointer to the Win32Platform structure.
 */
void
Game_SimulateTick(Game *game, Win32Platform *win32_platform);

/**
 * @brief Calculation of the game delta time.
 * @param game Pointer to the Game structure.
 */
void
Game_CalculateDTime(Game *game);

#endif  /* JEMA_GAME_H_ */