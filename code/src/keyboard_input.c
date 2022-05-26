/**
 * ================================================================================
 * @file keyboard_input.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of the Keyboard_Input class methods.
 * @version 0.1
 * @date 2022-01-10
 * @copyright JuvuSoft (c) 2022
 * ================================================================================
 */

/* Standard includes: */
#include <windows.h>
#include <stdio.h>

/* Program includes: */
#include <keyboard_input.h>
#include <utils.h>

void
keyboard_input_define_temp_key_data(Keyboard_Input_t *keyboard_input, 
    WPARAM w_param, LPARAM l_param)
{
    keyboard_input->temp_key_vk_code = (u32)w_param;
    keyboard_input->temp_key_was_down = ((l_param & (1 << 30)) != 0);
    keyboard_input->temp_key_is_down = ((l_param & (1 << 31)) == 0);
}

void 
keyboard_input_check_keys(Keyboard_Input_t *keyboard_input)
{
    /* Check every defined key one by one: */
    keyboard_input_check_key(keyboard_input, VK_LEFT, KEY_LEFT);
    keyboard_input_check_key(keyboard_input, VK_RIGHT, KEY_RIGHT);
    keyboard_input_check_key(keyboard_input, VK_UP, KEY_UP);
    keyboard_input_check_key(keyboard_input, VK_DOWN, KEY_DOWN);
    keyboard_input_check_key(keyboard_input, VK_MENU, KEY_ALT);
}

void
keyboard_input_check_key(Keyboard_Input_t *keyboard_input, 
    u32 checking_vk_code, Key_Type_t key)
{   
    u32 vk_code = keyboard_input->temp_key_vk_code;
    b32 was_down = keyboard_input->temp_key_was_down;
    b32 is_down = keyboard_input->temp_key_is_down;
    
    b32 is_pressed;  /* Flag showing if key is pressed right now or not. */
    b32 process_flag;  /* Flag showing if the imput from key requires processing. */
    
    if (vk_code == checking_vk_code) {
        
        /* If key is pressed */
        if (is_down)
        {
            is_pressed = true;
            process_flag = true;
        }
        
        /* If key is released */
        if (!is_down) {
            is_pressed = false;
        }
        
        /* If key is pressed and holding untill getting was_down flag */
        if ((is_down) && (was_down)) 
        {
            process_flag = true;
        }

        /* Update the pressed key state */
        keyboard_input->keys[key].vk_code = vk_code;
        keyboard_input->keys[key].is_pressed = is_pressed;
        keyboard_input->keys[key].process_flag = process_flag;
    }
}

b32 
keyboard_input_is_key_pressed_continuously(Keyboard_Input_t *keyboard_input, 
    Key_Type_t key)
{
    return (keyboard_input->keys[key].is_pressed);
}

b32 
keyboard_input_is_key_pressed_discretely(Keyboard_Input_t *keyboard_input, 
    Key_Type_t key)
{
    b32 result = false;
    
    if (keyboard_input->keys[key].is_pressed && keyboard_input->keys[key].process_flag)
    {
        result = true;
        keyboard_input->keys[key].process_flag = false;
    }
    return result;
}
