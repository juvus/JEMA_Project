/**
 * ================================================================================
 * @file src_engine/keyboard.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with keyboard input to
 * the game.
 * @version 0.2
 * @date 2022-12-06
 * ================================================================================
 */

#include "include_engine/keyboard.h"

#include <stdio.h>
#include <windows.h>

#include "include_engine/dbg.h"
#include "include_engine/helper_functions.h"
#include "include_engine/utils.h"

/**
 * @brief Make processing of a single keyboard key.
 * @param keyboard Pointer to the Keyboard structure.
 * @param vk_code Checking vk code of the key.
 * @param key Checking key name.
 */
static void
Keyboard_ProcessKey(Keyboard *keyboard, u32 vk_code, KeyType key);

Keyboard*
Keyboard_Constructor(void)
{
    size_t size = sizeof(Keyboard);
    Keyboard *keyboard = (Keyboard *)HelperFcn_MemAllocate(size);
    return keyboard;
}

Keyboard*
Keyboard_Destructor(Keyboard *keyboard)
{
    HelperFcn_MemFree(keyboard);
    return NULL;
}

void
Keyboard_DefineTempKeyData(Keyboard *keyboard, WPARAM w_param, LPARAM l_param)
{
    keyboard->temp_key_vk_code = (u32)w_param;
    keyboard->temp_key_was_down = ((l_param & (1 << 30)) != 0);
    keyboard->temp_key_is_down = ((l_param & (1 << 31)) == 0);
}

void 
Keyboard_ProcessKeys(Keyboard *keyboard)
{
    /* Check every defined key one by one. */
    Keyboard_ProcessKey(keyboard, VK_LEFT, KEY_LEFT);
    Keyboard_ProcessKey(keyboard, VK_RIGHT, KEY_RIGHT);
    Keyboard_ProcessKey(keyboard, VK_UP, KEY_UP);
    Keyboard_ProcessKey(keyboard, VK_DOWN, KEY_DOWN);
    Keyboard_ProcessKey(keyboard, VK_MENU, KEY_ALT);
}

static void
Keyboard_ProcessKey(Keyboard *keyboard, u32 vk_code, KeyType key)
{   
    u32 temp_vk_code = keyboard->temp_key_vk_code;
    b32 was_down = keyboard->temp_key_was_down;
    b32 is_down = keyboard->temp_key_is_down;
    
    b32 is_pressed = false;  /* Flag showing if key is pressed right now or not. */
    b32 process_flag = false;  /* Flag showing if the imput from key requires processing. */
    
    if (temp_vk_code == vk_code)
    {    
        /* If key is pressed. */
        if (is_down)
        {
            is_pressed = true;
            process_flag = true;
        }
        
        /* If key is released. */
        if (!is_down)
        {
            is_pressed = false;
        }
        
        /* If key is pressed and holding untill getting was_down flag. */
        if ((is_down) && (was_down))
        {
            process_flag = true;
        }

        /* Update the pressed key state. */
        keyboard->keys[key].vk_code = temp_vk_code;
        keyboard->keys[key].is_pressed = is_pressed;
        keyboard->keys[key].process_flag = process_flag;
    }
}

b32 
Keyboard_IsKeyPressedContinuously(Keyboard *keyboard, KeyType key)
{
    return (keyboard->keys[key].is_pressed);
}

b32 
Keyboard_IsKeyPressedDiscretely(Keyboard *keyboard, KeyType key)
{
    b32 result = false;  /* Result of the check. */
    
    if (keyboard->keys[key].is_pressed && keyboard->keys[key].process_flag)
    {
        result = true;
        keyboard->keys[key].process_flag = false;
    }
    return result;
}
