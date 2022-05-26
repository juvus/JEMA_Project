/**
 * ================================================================================
 * @file game.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of the Game class methods.
 * @version 0.2
 * @date 2021-12-10
 * @copyright Copyright (c) 2022
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard includes: */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* Program includes: */
#include <game.h>
#include <render.h>
#include <utils.h>
#include <keyboard_input.h>
#include <mouse_input.h>
#include <file_io.h>
#include <image.h>
#include <font.h>
#include <dbg.h>
#include <debug_console.h>
#include <sound_buffer.h>
#include <sound.h>
#include <game_constants.h>
/* TODO: Make additional file with list of game resources the same way as for 
    game_constants #include <game_resources.h> */

/* Game resources: */
/* Loaded files: */
static File_t *file_font_png;  /* PNG file with font symbol data. */
static File_t *file_smile_face_png;  /* PNG file with smiley face image. */
static File_t *file_background_wav;  /* WAV file for the background music. */

/* Game images: */
static Image_t *img_font; /* Image with font symbols. */
static Image_t *img_smile_face;  /* Temporary test image. */

/* Game sounds: */
enum Sounds_enum {e_snd_empty, e_snd_background, e_snd_num};
static Sound_t *snd_empty;  /* Empty sound designed for mixing purposes. */
static Sound_t *snd_background;  /* Sound of the background music. */
static Sound_t *sounds[e_snd_num]; /* Array of pointers to sound structures. */

/* Define different parameters for game systems: */
static Font_t *font;  /* Main font of the game. */
static Sound_Buffer_Worker_Data_t sound_buffer_worker_data = {0};  /* Structure with buffer worker data. */
static DConsole_t *dconsole;  /* Debug console with debug messages. */

/* Define different game variables: */
// NOTE: define some game classes later on here
u32 box_x;
u32 box_y;

/**
 * @brief Function for simulation a single game tick.
 * @param game Pointer to the Game structure.
 * @param keyboard_input Pointer to the Keyboard_Input structure.
 * @param mouse_input  Pointer to the Mouse_Input structure. 
 * @param render_buffer Pointer to the Render_Buffer structure.
 * @param sound_buffer POinter to the Sound_Buffer structure.
 */
void 
game_simulate_tick(Game_t *game, Keyboard_Input_t *keyboard_input, 
    Mouse_Input_t *mouse_input, Render_Buffer_t *render_buffer, 
    Sound_buffer_t *sound_buffer)
{   
    //game.dtime = 0.001; //for testing


    /* Processing of the game states. */
    switch (game->game_state) 
    {
    /* State for dynamic memory allocation for different game objects. */
    case GST_MEMORY_ALLOCATION:
    {           
        /* Invoke different objects constructors. */
        font = font_constructor();
        dconsole = dconsole_constructor();
        
        file_font_png = file_constructor();
        file_smile_face_png = file_constructor();
        file_background_wav = file_constructor();

        img_font = image_constructor();
        img_smile_face = image_constructor();
        snd_empty = sound_constructor();
        snd_background = sound_constructor();
        
        /* Jump to the next game stage. */      
        game->game_state = GST_LOAD_RESOURCES;
    } break;

    /* Stage for loading different game resources. */
    case GST_LOAD_RESOURCES:
    {
        /* Loading the different resource data IMAGES. */
        file_load_to_memory(file_font_png, "..\\data\\font.png");
        image_uncompress_png(img_font, file_font_png);
        file_destructor(file_font_png);

        file_load_to_memory(file_smile_face_png, "..\\data\\smile_face.png");
        image_uncompress_png(img_smile_face, file_smile_face_png);
        file_destructor(file_smile_face_png);

        /* Loading the different resource data SOUNDS. */   
        sound_prepare_empty_sound(snd_empty);

        file_load_to_memory(file_background_wav, "..\\data\\background.wav");
        sound_uncompress_wav(snd_background, file_background_wav);
        file_destructor(file_background_wav);
        
        /* Form the array of pointers to sounds. */
        sounds[e_snd_empty] = snd_empty;
        sounds[e_snd_background] = snd_background; 
        
        /* Preparing the sound buffer worker data. */
        sound_buffer_worker_data.sound_buffer = sound_buffer;
        sound_buffer_worker_data.sounds = sounds;
        sound_buffer_worker_data.e_sound_num = e_snd_num;

        /* Jump to the next game stage. */
        game->game_state = GST_INITIALIZATION;
    } break;

    /* State for the game initialization procedure. */
    case GST_INITIALIZATION:
    {   
        /* Seed the random numbers. */
        srand(time(NULL));      
               
        /* Prepare the font to use in the game. */
        font_extract_symbols(font, img_font);
        
        /* Initialization of some game parameters. */
        game->game_mode = GM_NORMAL;
        dconsole_init(dconsole, 100, 50, 100, font);
        dconsole_clear_messages(dconsole);

        /* Play the sound buffer. */
        sound_buffer_clear_buffer(sound_buffer);
        sound_buffer_play(sound_buffer);

        /* Play the background music. */
        sound_play_sound_continiously(snd_empty, S_PAN_BOTH, 1.0f);
        //snd_empty.is_playing = false; //test
        sound_play_sound_continiously(snd_background, S_PAN_BOTH, 1.0f);
        //snd_background.is_playing = false; //test   
        
        /* Make a separate thread to update the sounds in sound buffer. */
        CreateThread(0, 0, sound_buffer_worker_proc, &sound_buffer_worker_data, 0, 0);

        /* Function for the thread of main game loop. */
        QueryPerformanceCounter(&(game->begin_counter));
        QueryPerformanceFrequency(&(game->frequency_counter));
        game->dtime = 0.01666f; /* Initial dtime value. */
        game->cum_time = 0.0f;  /* Initial cumulative time of the game. */

        box_x = 100;
        box_y = 100;






        /* Jump to the next game stage. */
        game->game_state = GST_INIT_RENDER;
    } break;
    
    /* State for the initial render procedure. */
    case GST_INIT_RENDER:
    {
        
        render_clear_screen(0x0000AA, render_buffer);

        
        render_draw_rect(box_x, box_y, 200, 200, 0xFFFFFF, render_buffer);
        
        render_draw_bitmap(500, 500, img_smile_face, 3, render_buffer);
        


        /* State for the initial render procedure. */
        game->game_state = GST_PROCESS_GAME_TICK;
    } break;

    /* State for processing game ticks. */
    case GST_PROCESS_GAME_TICK:
    {
        /* Clear all previous debug console messages. */
        dconsole_clear_messages(dconsole);
        
        /* NOTE: test code. */
        if (keyboard_input_is_key_pressed_discretely(keyboard_input, KEY_UP))
        {
            
            
            render_draw_rect(box_x, box_y, 200, 200, 0x0000AA, render_buffer);
            
            box_x += 50;

            render_draw_rect(box_x, box_y, 200, 200, 0xFFFFFF, render_buffer);
        }
        
       
        
        dconsole_add_message(dconsole, "Test red string!", 0xdd0000);
        dconsole_add_message(dconsole, "Test blue string!", 0x0000dd);
        
        
        
        dconsole_render(dconsole, render_buffer);
        




        /* Calculate the delta time. */
        game_calculate_dtime(game);        

        /* Jump to the next game stage. */
	    game->game_state = GST_PROCESS_GAME_TICK;
    } break;

    /* State for finalization procedure. */
    case GST_FINALIZATION: 
    {
        
    } break;
    
    /* State for releasing the allocated memory. */
    case GST_MEMORY_FREE:
    {
        dconsole_destructor(dconsole);
    }}
}

void
game_calculate_dtime(Game_t *game)
{
    LONGLONG begin;  /* Variable for dtime calculation. */
    LONGLONG end;  /* Variable for dtime calculation. */
    LONGLONG frequency;  /* Variable for dtime calculation. */
    
    QueryPerformanceCounter(&game->end_counter);
    
    begin = game->begin_counter.QuadPart;
    end = game->end_counter.QuadPart;
    frequency = game->frequency_counter.QuadPart; 
    game->dtime = (f32)((end - begin) / (f32)frequency);
   
    /* Save guard to dtime (if the window is hold, so there is no large value for dtime). */
    game->dtime = (game->dtime > 0.01666f)? 0.01666f : game->dtime;
    game->cum_time += game->dtime;
    game->begin_counter = game->end_counter;
}