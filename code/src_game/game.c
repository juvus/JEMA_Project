/**
 * ================================================================================
 * @file src_game/game.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with the game main logic.  
 * @version 0.2
 * @date 2021-12-10
 * ================================================================================
 */

#define _USE_MATH_DEFINES

#include "include_game/game.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#include "include_engine/audio.h"
#include "include_engine/audio_worker.h"
#include "include_engine/color.h"
#include "include_engine/dbg.h"
#include "include_engine/debug_console.h"
#include "include_engine/engine_constants.h"
#include "include_engine/font.h"
#include "include_engine/image.h"
#include "include_engine/keyboard.h"
#include "include_engine/memory_object.h"
#include "include_engine/mouse.h"
#include "include_engine/random.h"
#include "include_engine/render.h"
#include "include_engine/sound.h"
#include "include_engine/utils.h"
#include "include_engine/win32_platform.h"

#include "include_game/game_constants.h"
#include "include_game/game_resourses.h"

/* Objects of the engine system. */
static Audio *audio;
static AudioWorker *audio_worker;
static DConsole *dconsole;
static GameResourses *gres;
static Font *font;
static Keyboard *keyboard;
static Mouse *mouse;
static Render *render;

/* Define different game logic variables: */
static u32 box_x;
static u32 box_y;

Game*
Game_Constructor(void)
{
    Game *game = (Game *)malloc(1 * sizeof(Game));
    if (game == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return game;
}

void
Game_Destructor(Game *game)
{
    if (game == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    free(game);
    game = NULL;
}

void
Game_Init(Game *game, GameState game_state, GameMode game_mode)
{
    game->cum_time = 0.0f;
    game->is_running = false;
    game->game_state = game_state;
    game->game_mode = game_mode;
}

void 
Game_SimulateTick(Game *game, Win32Platform *win32_platform)
{   
    //game.dtime = 0.001f;  // For testing purposes.

    /* Processing of the game states. */
    switch (game->game_state) 
    {
    /* State for definition of engine objects. */
    case GST_OBJECTS_DEF:
    {
        audio = win32_platform->audio;
        audio_worker = win32_platform->audio_worker;
        mouse = win32_platform->mouse;
        keyboard = win32_platform->keyboard;
        render = win32_platform->render;

        /* Jump to the next game stage. */
        game->game_state = GST_MEMORY_ALLOCATION;
    } break;
    
    /* State for dynamic memory allocation for different game objects. */
    case GST_MEMORY_ALLOCATION:
    {           
        gres = GameResourses_Constructor();
        font = Font_Constructor();
        dconsole = DConsole_Constructor(DCONSOLE_MESSAGES, DCONSOLE_MAX_MSG_LENGTH);

        /* Jump to the next game stage. */      
        game->game_state = GST_LOAD_RESOURCES;
    } break;

    /* Stage for loading different game resources. */
    case GST_LOAD_RESOURCES:
    {
        /* Loading different resource data for IMAGES. */
        MemObject_InitByFile(gres->mem_objects[GO_FONT_PNG], 
            "..\\data\\font_Win1251.png");
        Image_Init(gres->images[GI_FONT], gres->mem_objects[GO_FONT_PNG]);
        MemObject_Destructor(gres->mem_objects[GO_FONT_PNG]);

        MemObject_InitByFile(gres->mem_objects[GO_SMILE_FACE_PNG], 
            "..\\data\\smile_face.png");
        Image_Init(gres->images[GI_SMILE_FACE], gres->mem_objects[GO_SMILE_FACE_PNG]);
        MemObject_Destructor(gres->mem_objects[GO_SMILE_FACE_PNG]);

        /* Loading different resource data for SOUNDS. */        
        MemObject_InitByFile(gres->mem_objects[GO_BACKGROUND_WAV], 
            "..\\data\\background.wav");
        Sound_Init(gres->sounds[GS_BACKGROUND], gres->mem_objects[GO_BACKGROUND_WAV]);
        MemObject_Destructor(gres->mem_objects[GO_BACKGROUND_WAV]);
 
        /* Jump to the next game stage. */
        game->game_state = GST_INITIALIZATION;
    } break;

    /* State for the game initialization procedure. */
    case GST_INITIALIZATION:
    {          
        /* Initialization of the game colors. */
        gres->colors[GC_WHITE]->color = 0xffffffff;
        gres->colors[GC_BLACK]->color = 0xff000000;
        gres->colors[GC_RED]->color = 0xffff0000;
        gres->colors[GC_GREEN]->color = 0xff00ff00;
        gres->colors[GC_BLUE]->color = 0xff0000ff;
        gres->colors[GC_DCONSOLE_BKG]->color = DCONSOLE_BKG_COLOR;
        gres->colors[GC_DCONSOLE_BRD]->color = DCONSOLE_BRD_COLOR;
        gres->colors[GC_BKG_COLOR]->color = BKG_COLOR;
        
        /* Prepare the font to use in the game. */
        Font_Init(font, (s32)FONT_ROWS_NUM, (s32)FONT_COLS_NUM, (s32)FONT_SYM_WIDTH, 
            (s32)FONT_SYM_HEIGHT, gres->images[GI_FONT]);
        
        /* Initialization of the debuf console. */
        DConsole_Init(dconsole, 100, 50, 200, gres->colors[GC_DCONSOLE_BKG], 
            gres->colors[GC_DCONSOLE_BRD], DCONSOLE_MARGINS, gres->colors[GC_BKG_COLOR],
            font);
        DConsole_ClearMessages(dconsole);

        /* Initialization of the audio system. */
        Audio_Init(audio, render->window);
        Audio_CleanBuffer(audio);
        Audio_PlaySounds(audio);

        /* Preparation of the empty sound. */
        Sound_PrepareEmptySound(gres->sounds[GS_EMPTY]);

        /* Initialization of the audio worker and starting the new thread. */
        AudioWorker_Init(audio_worker, audio, gres->sounds, GS_SOUNDS_NUM);
        CreateThread(0, 0, AudioWorker_ThreadProc, audio_worker, 0, 0);

        /* Preparation of the game timer. */
        QueryPerformanceCounter(&(game->begin_counter));
        QueryPerformanceFrequency(&(game->frequency_counter));
        game->dtime = 0.01666f; /* Initial dtime value. */
        game->cum_time = 0.0f;  /* Initial cumulative time of the game. */

        /* Initialization of the game randomization. */
        Random_Randomize(true, 0);

        /* Initialization of the game logic related variables. */
        box_x = 300;
        box_y = 300;

        /* Jump to the next game stage. */
        game->game_state = GST_INIT_RENDER;
    } break;
    
    /* State for the initial render procedure. */
    case GST_INIT_RENDER:
    {
        /* Initial render of the game elements. */
        Render_ClearScreen(render, gres->colors[GC_BKG_COLOR]);
        Render_DrawRect(render, box_x, box_y, 100, 100, gres->colors[GC_WHITE]);
        Render_DrawBitmap(render, 500, 500, gres->images[GI_SMILE_FACE], 3);
        
        /* Start to play the gaem music music. */
        Sound_PlaySoundContiniously(gres->sounds[GS_EMPTY], S_PAN_BOTH, 1.0f);
        Sound_PlaySoundContiniously(gres->sounds[GS_BACKGROUND], S_PAN_BOTH, 1.0f);

        /* State for the initial render procedure. */
        game->game_state = GST_PROCESS_GAME_TICK;
    } break;

    /* State for processing game ticks. */
    case GST_PROCESS_GAME_TICK:
    {
        /* Clear all previous debug console messages. */
        DConsole_ClearMessages(dconsole);
        
        if (Keyboard_IsKeyPressedDiscretely(keyboard, KEY_RIGHT))
        {
            Render_DrawRect(render, box_x, box_y, 100, 100, gres->colors[GC_BKG_COLOR]);
            box_x += 5;
            Render_DrawRect(render, box_x, box_y, 100, 100, gres->colors[GC_WHITE]);
        }
        
        DConsole_AddMessage(dconsole, "Test red string!", gres->colors[GC_RED]);
        DConsole_AddMessage(dconsole, "Test blue string!", gres->colors[GC_BLUE]);
        DConsole_Render(dconsole, render);
        
        /* Calculate the delta time. */
        Game_CalculateDTime(game);

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
        DConsole_Destructor(dconsole);
    }}
}

void
Game_CalculateDTime(Game *game)
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