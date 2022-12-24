/**
 * ================================================================================
 * @file include_engine/keyboard.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with keyboard
 * input to the game.
 * @version 0.2
 * @date 2022-11-06
 * ================================================================================ 
 */

#ifndef JGE_ENGINE_KEYBOARD_H_
#define JGE_ENGINE_KEYBOARD_H_

#include "include_engine/utils.h"

/**
 * @brief Enumerator holding the keyboard button names. This enumerator lists the 
 * buttons to be used in the user keyboard input treatment.  
 */
enum KeyType_
{
    KEY_LEFT,  /**< Left arrow key. */
    KEY_RIGHT,  /**< Right arrow key. */
    KEY_UP,  /**< Up arrow key. */
    KEY_DOWN,  /**< Down arrow key. */
    KEY_ALT,  /**< ALT key. */
    KEY_COUNT,  /**< Number of defined keys. */
};
typedef enum KeyType_ KeyType;

/**
 * @brief Structure for storing key states. Holding the states of the keys is 
 * necessary for implementing different functionality of pressing and holding a key.
 */
struct Key_ 
{
    u32 vk_code;  /**< Scan code of the keyboard physical key. */
    b32 is_pressed;  /**< Flag showing if key is pressed right now or not. */
    b32 process_flag;  /**< Flag showing if the imput from key requires processing. */
};
typedef struct Key_ Key;

/**
 * @brief Structure for holding user keyboard input.
 */
struct Keyboard_
{
    u32 temp_key_vk_code;  /**< Temporary key vk_vode. */
    b32 temp_key_was_down;  /**< Temporary key was_down bit flag. */
    b32 temp_key_is_down;  /**< Temporary key is_down bit flag. */
    Key keys[KEY_COUNT];  /**< Array holding all defined keys states. */
};
typedef struct Keyboard_ Keyboard;

/**
 * @brief Object constructor.
 * @return Keyboard* Pointer to the Keyboard structure.
 */
Keyboard*
Keyboard_Constructor(void);

/**
 * @brief Object destructor.
 * @param keyboard Pointer to the Keyboard structure.
 */
void
Keyboard_Destructor(Keyboard *keyboard);

/**
 * @brief Determination of the temp key data from the message parameters.
 * @param keyboard Pointer to the Keyboard_Input structure.
 * @param w_param Additional windows message parameter.
 * @param l_param Additional windows message parameter.
 */
void
Keyboard_DefineTempKeyData(Keyboard *keyboard, WPARAM w_param, LPARAM l_param);

/**
 * @brief Set of preparations for a new user keyboard input. These preparations are 
 * nesessary for the determination of the way how the key is pressing.
 * @param keyboard Pointer to the Keyboard structure.
 */
void 
Keyboard_ProcessKeys(Keyboard *keyboard);

/**
 * @brief Determination if the checked key was pressed and currently holding. 
 * @param keyboard Pointer to the Keyboard structure.
 * @param key Checked key name.
 * @return b32 Flag showing if the key was pressed and holding.
 */
b32 
Keyboard_IsKeyPressedContinuously(Keyboard *keyboard, KeyType key);

/**
 * @brief Determination if the checked key was pressed and then released.
 * @param keyboard Pointer to the Keyboard structure.
 * @param key Checked key name.
 * @return b32 Flag showing if the key was pressed and then released.
 */
b32 
Keyboard_IsKeyPressedDiscretely(Keyboard *keyboard, KeyType key);

#endif /* KEYBOARD_H_ */
