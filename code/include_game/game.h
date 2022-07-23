/**
 * ================================================================================
 * @file game.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Game class methods.
 * @version 0.2
 * @date 2022-01-10
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

#ifndef GAME_H_
#define GAME_H_

/* Standard library includes. */
#include <windows.h>

/* Game engine includes. */
#include <utils.h>

/* Predefined structures. */
typedef struct Keyboard Keyboard_t;
typedef struct Mouse Mouse_t;
typedef struct Render Render_t;
typedef struct Audio Audio_t;
typedef struct Audio_Worker_Data Audio_Worker_Data_t;
typedef struct Font Font_t;
typedef struct DConsole DConsole_t;

/**
 * @brief Enumerator for different states of the game. 
 */
enum Game_State
{
    GST_MEMORY_ALLOCATION,  /**< State for dynamic memory allocation. */
    GST_LOAD_RESOURCES,  /**< State for loading different game resources. */
    GST_INITIALIZATION,  /**< State for the game initialization procedure. */
    GST_INIT_RENDER,  /**< State for the initial render procedure. */
    GST_PROCESS_GAME_TICK,  /**< State for processing game ticks. */
    GST_FINALIZATION,  /**< State for finalization procedure. */
    GST_RELEASE_MEMORY   /**< State for free the allocated dynamic memory. */
};
typedef enum Game_State Game_State_t;

/**
 * @brief Enumerator for the different game modes. 
 */
enum Game_Mode
{
    GM_NORMAL,  /**< Normal game mode. */
    GM_IMMORTAL /**< Immortality mode for testing. */
};
typedef enum Game_Mode Game_Mode_t;

/**
 * @brief Structure for the Game properties.
 */
struct Game
{
    LARGE_INTEGER begin_counter;  /**< Union for for dtime calculation. */
    LARGE_INTEGER end_counter;  /**< Union for for dtime calculation. */
    LARGE_INTEGER frequency_counter;  /**< Union for for dtime calculation. */

    f32 dtime; /**< Time delta between the game ticks (in sec). */
    f32 cum_time;  /**< Cumulative time of the game (in sec). */
    b32 is_running;  /**< Flag showing is the main loop is running or not. */
    Game_State_t game_state;  /**< Current state of the game. */
    Game_Mode_t game_mode;  /**< Current game mode. */
};
typedef struct Game Game_t; 

/**
 * @brief Constructor of the Game class.
 * @return Game_t* Pointer to the Game structure. 
 */
Game_t*
game_constructor(void);

/**
 * @brief Destructor of the Game object.
 * @param game Pointer to the Game structure.
 */
void
game_destructor(Game_t *game);

/**
 * @brief Initialization of the Game object.
 * @param game Pointer to the Game structure.
 */
void
game_init(Game_t *game, Game_State_t game_state, Game_Mode_t game_mode);

/**
 * @brief Simulation of a single game tick.
 * @param game Pointer to the Game structure.
 * @param keyboard Pointer to the Keyboard structure.
 * @param mouse  Pointer to the Mouse structure. 
 * @param render Pointer to the Render structure.
 */
void
game_simulate_tick(Game_t *game, Keyboard_t *keyboard, Mouse_t *mouse, Render_t *render);

/**
 * @brief Calculation of the game delta time.
 * @param game Pointer to the Game structure.
 */
void
game_calculate_dtime(Game_t *game);

#endif // GAME_H_