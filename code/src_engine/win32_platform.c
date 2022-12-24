/**
 * ================================================================================
 * @file src_engine/win32_platform.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with Win32 platform layer. 
 * @version 0.2
 * @date 2022-12-22
 * ================================================================================
 */

#undef UNICODE
#include <windows.h>

#include "include_engine/audio.h"
#include "include_engine/audio_worker.h"
#include "include_engine/dbg.h"
#include "include_engine/keyboard.h"
#include "include_engine/mouse.h"
#include "include_engine/render.h"
#include "include_engine/win32_platform.h"
#include "include_engine/utils.h"

#include "include_game/game.h"
#include "include_game/game_worker.h"

static Game *game;  /* Pointer to the Game structure. */
static GameWorker *game_worker;  /* Pointer to the GameWorker structure. */
static Win32Platform *win32_platform;  /* Win32Platform structure. */

/**
 * @brief Window callback function for window messages processing.
 * @param window Handle to the window.
 * @param message Particular message code.
 * @param w_param Additional parameter for message processing.
 * @param l_param Additional parameter for message processing.
 * @return LRESULT Standard return from a callback function.
 */
static LRESULT CALLBACK 
WindowCallback(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

/**
 * @brief Function for the entry point of the application. 
 * @param hInstance A handle to the current instance of the application.
 * @param hPrevInstance A handle to the previous instance of the application (NULL).
 * @param lpCmdLine The command line for the application, excluding the program name. 
 * @param nCmdShow Controls how the window is to be shown.
 * @return int Standard return for Windows application.
 */
int CALLBACK 
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    /* Avoid warning messages about not used function parameters. */
    UNUSED(hPrevInstance);
    UNUSED(lpCmdLine);
    UNUSED(nCmdShow); 

    /* Create a window class. */
    WNDCLASS window_class = {0};
    window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    window_class.hInstance = hInstance;
    window_class.lpfnWndProc = WindowCallback;
    window_class.lpszClassName = "Game";

    /* Register the window class. */
    if (!RegisterClass(&window_class))
    {
        dbg_error("%s", "Windows class was not registered correctly!");
    }

    /* Create Win32 platform and initialize. */
    win32_platform = Win32Platform_Constructor();
    Win32Platform_Init(win32_platform);

    /* Create game and initialize. */
    game = Game_Constructor();
    Game_Init(game, GST_OBJECTS_DEF, GM_NORMAL);
    game->is_running = true;

    /* Create a window and return a window handle. */
    HWND window;
    DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
    window = CreateWindowEx(0, window_class.lpszClassName, "Game", dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, 0);
    if (window == NULL)
    {
        dbg_error("%s", "Window was not successfully created!");
    }
    
    /* Starting a message processing loop. */
    MSG message;
    while (game->is_running)
    {
        /* Get and process messages. */
        while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
             /* Handle the message. */
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        /* Reduce the CPU usage time. */
        Sleep(1);
    }
    
    /* Calling destructors. */
    Game_Destructor(game);
    GameWorker_Destructor(game_worker);
    Win32Platform_Destructor(win32_platform);
    return 0;
}

static LRESULT CALLBACK 
WindowCallback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;

    /* Handling the window messages. */
    switch (message)
    {   
    /* Window closing and destroying messages. */
    case WM_CLOSE:
    case WM_DESTROY:
    {
        game->is_running = false;
    } break;

    /* Creating the window message. */
    case WM_CREATE:
    {
        /* Initialize game render. */
        Render_Init(win32_platform->render, window);
        
        /* Run the game in a separate thread. */
        game_worker = GameWorker_Constructor();
        GameWorker_Init(game_worker, game, win32_platform);  
        CreateThread(0, 0, GameWorker_ThreadProc, game_worker, 0, 0);
    } break;
    
    /* Resizing of the window message. */
    case WM_SIZE:
    {
        Render_ResizeWindow(win32_platform->render);
    } break;

    /* Processing messages when keys are pressed and released. */
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {   
        Keyboard_DefineTempKeyData(win32_platform->keyboard, w_param, l_param);
        Keyboard_ProcessKeys(win32_platform->keyboard);
    } break;

    /* Processing messages for mouse in the game window. */
    case WM_MOUSEMOVE:
    {
        GetCursorPos(&(win32_platform->mouse->raw_cursor));
        ScreenToClient(window, &(win32_platform->mouse->raw_cursor));
        Mouse_PrepareInput(win32_platform->mouse, win32_platform->render);
    } break;

    /* Default message and uncatched messages: */
    default:
    {
        result = DefWindowProc(window, message, w_param, l_param);
    }}
    return result;
}

Win32Platform*
Win32Platform_Constructor(void)
{
    Win32Platform *platform = (Win32Platform *)malloc(1 * sizeof(Win32Platform));
    if (platform == NULL)
    {
        dbg_error("%s", "Memory allocation error!");
    }
    return platform;
}

void
Win32Platform_Destructor(Win32Platform *platform)
{
    if (platform == NULL)
    {
        dbg_error("%s", "Attempt to delete an empty object!");
    }
    Audio_Destructor(platform->audio);
    AudioWorker_Destructor(platform->audio_worker);
    Keyboard_Destructor(platform->keyboard);
    Mouse_Destructor(platform->mouse);
    Render_Destructor(platform->render);
    free(platform);
    platform = NULL;
}

void
Win32Platform_Init(Win32Platform *platform)
{
    platform->audio = Audio_Constructor();
    platform->audio_worker = AudioWorker_Constructor();
    platform->keyboard = Keyboard_Constructor();
    platform->mouse = Mouse_Constructor();
    platform->render = Render_Constructor();
}
