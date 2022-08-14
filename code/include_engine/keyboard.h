/**
 * ================================================================================
 * @file keyboard.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with keyboard
 * input to the game.
 * @version 0.1
 * @date 2022-07-11
 * ================================================================================ 
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

/* Game engine includes: */
#include "utils.h"

/**
 * @brief Enumerator holding the keyboard button names. This enumerator lists the 
 * buttons to be used in the user keyboard input treatment.  
 */
enum Key_Type
{
    KEY_LEFT,  /**< Left arrow key. */
    KEY_RIGHT,  /**< Right arrow key. */
    KEY_UP,  /**< Up arrow key. */
    KEY_DOWN,  /**< Down arrow key. */
    KEY_ALT,  /**< ALT key. */
    KEY_COUNT,  /**< Number of defined keys. */
};
typedef enum Key_Type Key_Type_t;

/**
 * @brief Structure for storing key states. Holding the states of the keys is 
 * necessary for implementing different functionality of pressing and holding a key.
 */
struct Key 
{
    u32 vk_code;  /**< Scan code of the keyboard physical key. */
    b32 is_pressed;  /**< Flag showing if key is pressed right now or not. */
    b32 process_flag;  /**< Flag showing if the imput from key requires processing. */
};
typedef struct Key Key_t;

/**
 * @brief Structure for holding user keyboard input.
 */
struct Keyboard
{
    u32 temp_key_vk_code;  /**< Temporary key vk_vode. */
    b32 temp_key_was_down;  /**< Temporary key was_down bit flag. */
    b32 temp_key_is_down;  /**< Temporary key is_down bit flag. */
    Key_t keys[KEY_COUNT];  /**< Array holding all defined keys states. */
};
typedef struct Keyboard Keyboard_t;

/**
 * @brief Constructor of the Keyboard class.
 * @return Keyboard_t* Pointer to the Keyboard structure.
 */
Keyboard_t*
keyboard_constructor(void);

/**
 * @brief Destructor of the Keyboard class.
 * @param keyboard Pointer to the Keyboard structure.
 */
void
keyboard_destructor(Keyboard_t *keyboard);

/**
 * @brief Determination of the temp key data from the message parameters.
 * @param keyboard Pointer to the Keyboard_Input structure.
 * @param w_param Additional windows message parameter.
 * @param l_param Additional windows message parameter.
 */
void
keyboard_define_temp_key_data(Keyboard_t *keyboard, WPARAM w_param, LPARAM l_param);

/**
 * @brief Set of preparations for a new user keyboard input. These preparations are 
 * nesessary for the determination of the way how the key is pressing.
 * @param keyboard Pointer to the Keyboard structure.
 */
void 
keyboard_process_keys(Keyboard_t *keyboard);

/**
 * @brief Determination if the checked key was pressed and currently holding. 
 * @param keyboard Pointer to the Keyboard structure.
 * @param key Checked key name.
 * @return b32 Flag showing if the key was pressed and holding.
 */
b32 
keyboard_is_key_pressed_continuously(Keyboard_t *keyboard, Key_Type_t key);

/**
 * @brief Determination if the checked key was pressed and then released.
 * @param keyboard Pointer to the Keyboard structure.
 * @param key Checked key name.
 * @return b32 Flag showing if the key was pressed and then released.
 */
b32 
keyboard_is_key_pressed_discretely(Keyboard_t *keyboard, Key_Type_t key);

#endif /* KEYBOARD_H_ */
