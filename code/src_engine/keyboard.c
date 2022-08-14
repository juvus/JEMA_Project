/**
 * ================================================================================
 * @file keyboard.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with keyboard input to
 * the game.
 * @version 0.1
 * @date 2022-07-11
 * ================================================================================
 */

/* Standard library includes: */
#include <windows.h>
#include <stdio.h>

/* Game engine includes: */
#include "keyboard.h"
#include "utils.h"

/* Static functions. */
/**
 * @brief Make processing of a single keyboard key.
 * @param keyboard Pointer to the Keyboard structure.
 * @param vk_code Checking vk code of the key.
 * @param key Checking key name.
 */
static void
keyboard_process_key(Keyboard_t *keyboard, u32 vk_code, Key_Type_t key);

Keyboard_t*
keyboard_constructor(void)
{
    Keyboard_t *keyboard;  /* Pointer to the Keyboard structure. */

    /* Allocation the memory for the Keyboard structure. */
    keyboard = (Keyboard_t*)malloc(1 * sizeof(Keyboard_t));
    return keyboard;
}

void
keyboard_destructor(Keyboard_t *keyboard)
{
    /* Free memory allocated for the Keyboard structure. */
    free(keyboard);
    keyboard = NULL;
}

void
keyboard_define_temp_key_data(Keyboard_t *keyboard, WPARAM w_param, LPARAM l_param)
{
    keyboard->temp_key_vk_code = (u32)w_param;
    keyboard->temp_key_was_down = ((l_param & (1 << 30)) != 0);
    keyboard->temp_key_is_down = ((l_param & (1 << 31)) == 0);
}

void 
keyboard_process_keys(Keyboard_t *keyboard)
{
    /* Check every defined key one by one. */
    keyboard_process_key(keyboard, VK_LEFT, KEY_LEFT);
    keyboard_process_key(keyboard, VK_RIGHT, KEY_RIGHT);
    keyboard_process_key(keyboard, VK_UP, KEY_UP);
    keyboard_process_key(keyboard, VK_DOWN, KEY_DOWN);
    keyboard_process_key(keyboard, VK_MENU, KEY_ALT);
}

static void
keyboard_process_key(Keyboard_t *keyboard, u32 vk_code, Key_Type_t key)
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
keyboard_is_key_pressed_continuously(Keyboard_t *keyboard, Key_Type_t key)
{
    return (keyboard->keys[key].is_pressed);
}

b32 
keyboard_is_key_pressed_discretely(Keyboard_t *keyboard, Key_Type_t key)
{
    b32 result = false;  /* Result of the check. */
    
    if (keyboard->keys[key].is_pressed && keyboard->keys[key].process_flag)
    {
        result = true;
        keyboard->keys[key].process_flag = false;
    }
    return result;
}
