/**
 * ================================================================================
 * @file game.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with the game main logic.  
 * @version 0.2
 * @date 2021-12-10
 * ================================================================================
 */

#define _USE_MATH_DEFINES

/* Standard library includes. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Game engine includes. */
#include <render.h>
#include <utils.h>
#include <keyboard.h>
#include <mouse.h>
#include <file_io.h>
#include <image.h>
#include <color.h>
#include <font.h>
#include <dbg.h>
#include <debug_console.h>
#include <sound.h>
#include <audio.h>
#include <audio_worker.h>
#include <engine_constants.h>
#include <random.h>

/* Game includes. */
#include <game.h>
#include <game_constants.h>
#include <game_resourses.h>

/* Objects of the game system. */
static Audio_t *audio;  /* Game audio. */
static Font_t *font;  /* Font for the game. */
static Audio_Worker_t *audio_worker;  /* Audio worker data. */
static DConsole_t *dconsole;  /* Debug console for debugging messages. */
static Game_Resourses_t *gres;  /* Game resourses. */

/* Define different game logic variables: */
u32 box_x;
u32 box_y;

Game_t*
game_constructor(void)
{
    Game_t *game;  /* Pointer to the Game structure. */

    /* Allocate the memory for the audio object. */
    game = (Game_t *)malloc(1 * sizeof(Game_t));

    return game;
}

void
game_destructor(Game_t *game)
{
    /* Release memory allocated for the Game object. */
    free(game);
    game = NULL;
}

void
game_init(Game_t *game, Game_State_t game_state, Game_Mode_t game_mode)
{
    game->cum_time = 0.0f;
    game->is_running = false;
    game->game_state = game_state;
    game->game_mode = game_mode;
}

void 
game_simulate_tick(Game_t *game, Keyboard_t *keyboard, Mouse_t *mouse, Render_t *render)
{   
    //game.dtime = 0.001f;  // For testing purposes.

    /* Processing of the game states. */
    switch (game->game_state) 
    {
    /* State for dynamic memory allocation for different game objects. */
    case GST_MEMORY_ALLOCATION:
    {           
        /* Object holding the resourses of the game. */
        gres = game_resourses_constructor();

        /* Objects for the game system. */
        font = font_constructor();
        dconsole = dconsole_constructor(DCONSOLE_MESSAGES, DCONSOLE_MAX_MSG_LENGTH);
        audio = audio_constructor();
        audio_worker = audio_worker_constructor();

        /* Jump to the next game stage. */      
        game->game_state = GST_LOAD_RESOURCES;
    } break;

    /* Stage for loading different game resources. */
    case GST_LOAD_RESOURCES:
    {
        /* Loading different resource data for IMAGES. */
        file_load_to_memory(gres->files[GF_FONT_PNG], "..\\data\\font.png");
        image_init(gres->images[GI_FONT], gres->files[GF_FONT_PNG]);
        file_destructor(gres->files[GF_FONT_PNG]);

        file_load_to_memory(gres->files[GF_SMILE_FACE_PNG], "..\\data\\smile_face.png");
        image_init(gres->images[GI_SMILE_FACE], gres->files[GF_SMILE_FACE_PNG]);
        file_destructor(gres->files[GF_SMILE_FACE_PNG]);

        /* Loading different resource data for SOUNDS. */        
        file_load_to_memory(gres->files[GF_BACKGROUND_WAV], "..\\data\\background.wav");
        sound_init(gres->sounds[GS_BACKGROUND], gres->files[GF_BACKGROUND_WAV]);
        file_destructor(gres->files[GF_BACKGROUND_WAV]);
 
        /* Jump to the next game stage. */
        game->game_state = GST_INITIALIZATION;
    } break;

    /* State for the game initialization procedure. */
    case GST_INITIALIZATION:
    {          
        /* Initialization of the game colors. */
        gres->colors[GC_WHITE]->color = 0x00ffffff;
        gres->colors[GC_BLACK]->color = 0x00000000;
        gres->colors[GC_RED]->color = 0x00ff0000;
        gres->colors[GC_GREEN]->color = 0x0000ff00;
        gres->colors[GC_BLUE]->color = 0x000000ff;
        gres->colors[GC_DCONSOLE_BKG]->color = DCONSOLE_BKG_COLOR;
        gres->colors[GC_DCONSOLE_BRD]->color = DCONSOLE_BRD_COLOR;
        gres->colors[GC_BKG_COLOR]->color = BKG_COLOR;
        
        /* Prepare the font to use in the game. */
        font_init(font, FONT_ROWS_NUM, FONT_COLS_NUM, FONT_SYM_WIDTH, FONT_SYM_HEIGHT,
            gres->images[GI_FONT]);
        
        /* Initialization of the debuf console. */
        dconsole_init(dconsole, 100, 50, 100, gres->colors[GC_DCONSOLE_BKG], 
            gres->colors[GC_DCONSOLE_BRD], DCONSOLE_MARGINS, gres->colors[GC_BKG_COLOR],
            font);
        dconsole_clear_messages(dconsole);

        /* Initialization of the audio system. */
        audio_init(audio, render->window);
        audio_clean_buffer(audio);
        audio_play_sounds(audio);

        /* Preparation of the empty sound. */
        sound_prepare_empty_sound(gres->sounds[GS_EMPTY]);

        /* Initialization of the audio worker and starting the new thread. */
        audio_worker_init(audio_worker, audio, gres->sounds, GS_SOUNDS_NUM);
        CreateThread(0, 0, audio_worker_proc, audio_worker, 0, 0);

        /* Preparation of the game timer. */
        QueryPerformanceCounter(&(game->begin_counter));
        QueryPerformanceFrequency(&(game->frequency_counter));
        game->dtime = 0.01666f; /* Initial dtime value. */
        game->cum_time = 0.0f;  /* Initial cumulative time of the game. */

        /* Initialization of the game randomization. */
        random_randomize(true, 0);

        /* Initialization of the game logic related variables. */
        box_x = 100;
        box_y = 100;

        /* Jump to the next game stage. */
        game->game_state = GST_INIT_RENDER;
    } break;
    
    /* State for the initial render procedure. */
    case GST_INIT_RENDER:
    {
        /* Initial render of the game elements. */
        render_clear_screen(render, gres->colors[GC_BKG_COLOR]);
        render_draw_rect(render, box_x, box_y, 200, 200, gres->colors[GC_WHITE]);
        render_draw_bitmap(render, 500, 500, gres->images[GI_SMILE_FACE], 3);
        
        /* Start to play the gaem music music. */
        sound_play_sound_continiously(gres->sounds[GS_EMPTY], S_PAN_BOTH, 1.0f);
        sound_play_sound_continiously(gres->sounds[GS_BACKGROUND], S_PAN_BOTH, 1.0f);

        /* State for the initial render procedure. */
        game->game_state = GST_PROCESS_GAME_TICK;
    } break;

    /* State for processing game ticks. */
    case GST_PROCESS_GAME_TICK:
    {
        /* Clear all previous debug console messages. */
        dconsole_clear_messages(dconsole);
        
        if (keyboard_is_key_pressed_discretely(keyboard, KEY_UP))
        {
            render_draw_rect(render, box_x, box_y, 200, 200, gres->colors[GC_BKG_COLOR]);
            box_x += 50;
            render_draw_rect(render, box_x, box_y, 200, 200, gres->colors[GC_WHITE]);
        }
        
        dconsole_add_message(dconsole, "Test red string!", gres->colors[GC_RED]);
        dconsole_add_message(dconsole, "Test blue string!", gres->colors[GC_BLUE]);
        dconsole_render(dconsole, render);
        
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
    case GST_RELEASE_MEMORY:
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