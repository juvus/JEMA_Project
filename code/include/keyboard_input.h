/**
 * ================================================================================
 * @file keyboard_input.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Keyboard_Input class methods.
 * @version 0.1
 * @date 2022-01-10
 * @copyright JuvuSoft (c) 2022
 * ================================================================================ 
 */

#ifndef KEYBOARD_INPUT_H_
#define KEYBOARD_INPUT_H_

/* Program includes: */
#include <utils.h>

/**
 * @brief Enumerator for keyboard button names.
 * This enumerator lists the buttons to be used in the user keyboard
 * input treatment.  
 */
enum Key_Type
{
    KEY_LEFT,  /**< Left arrow key */
    KEY_RIGHT,  /**< Right arrow key */
    KEY_UP,  /**< Up arrow key */
    KEY_DOWN,  /**< Down arrow key */
    KEY_ALT,  /**< ALT key */
    KEY_COUNT,  /**< Number of defined keys */
};
typedef enum Key_Type Key_Type_t;

/**
 * @brief Structure for storing key states.
 * Holding the states of the keys is necessary for implementing different
 * functionality of pressing and holding a key.
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
struct Keyboard_Input 
{
    u32 temp_key_vk_code;  /**< Temporary key vk_vode */
    b32 temp_key_was_down;  /**< Temporary key was_down bit flag */
    b32 temp_key_is_down;  /**< Temporary key is_down bit flag */
    Key_t keys[KEY_COUNT];  /**< Array holding all defined keys states. */
};
typedef struct Keyboard_Input Keyboard_Input_t;

/**
 * @brief Determination of the temp key data from the message parameters.
 * @param keyboard_input Pointer to the Keyboard_Input structure.
 * @param w_param Additional windows message parameter.
 * @param l_param Additional windows message parameter.
 */
void
keyboard_input_define_temp_key_data(Keyboard_Input_t *keyboard_input, 
    WPARAM w_param, LPARAM l_param);

/**
 * @brief Set of preparations for a new user keyboard input.
 * These preparations are nesessary for the determination of the way
 * how the key is pressing.
 * @param keyboard_input Pointer to the Keyboard_Input structure.
 */
void 
keyboard_input_check_keys(Keyboard_Input_t *keyboard_input);

/**
 * @brief Set of preparations for a single key
 * @param keyboard_input Pointer to the Keyboard_Input structure.
 * @param checking_vk_code Checking vk code of the key.
 * @param key Checking key name.
 */
void
keyboard_input_check_key(Keyboard_Input_t *keyboard_input,
    u32 checking_vk_code, Key_Type_t key);

/**
 * @brief Determination if the checked key was pressed and currently holding. 
 * @param keyboard_input Pointer to the Keyboard_Input structure.
 * @param key Checked key name.
 * @return b32 Flag showing if the key was pressed and holding.
 */
b32 
keyboard_input_is_key_pressed_continuously(Keyboard_Input_t *keyboard_input, 
    Key_Type_t key);

/**
 * @brief Determination if the checked key was pressed and then released.
 * @param keyboard_input Pointer to the Keyboard_Input structure.
 * @param key Checked key name.
 * @return b32 Flag showing if the key was pressed and then released.
 */
b32 
keyboard_input_is_key_pressed_discretely(Keyboard_Input_t *keyboard_input, 
    Key_Type_t key);

#endif /* KEYBOARD_INPUT_H_ */
