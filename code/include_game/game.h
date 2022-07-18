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

/* Program includes: */
#include <utils.h>

/* Predefined structures */
typedef struct Keyboard_Input Keyboard_Input_t;
typedef struct Mouse_Input Mouse_Input_t;
typedef struct Render_Buffer Render_Buffer_t;
typedef struct Audio_buffer Audio_buffer_t;

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
    GST_MEMORY_FREE   /**< State for free the allocated dynamic memory. */
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
 * @brief Simulation of a single game tick.
 * @param game Pointer to the Game structure.
 * @param keyboard_input Pointer to the Keyboard_Input structure.
 * @param mouse_input  Pointer to the Mouse_Input structure. 
 * @param render_buffer Pointer to the Render_Buffer structure.
 * @param sound_buffer Pointer to the Sound_Buffer structure.
 */
void 
game_simulate_tick(Game_t *game, Keyboard_Input_t *keyboard_input, 
    Mouse_Input_t *mouse_input, Render_Buffer_t *render_buffer,
    Audio_buffer_t *sound_buffer);

/**
 * @brief Calculation of the game delta time.
 * @param game Pointer to the Game structure.
 */
void
game_calculate_dtime(Game_t *game);

#endif // GAME_H_