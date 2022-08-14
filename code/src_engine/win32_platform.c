/**
 * ================================================================================
 * @file win32_platform.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Definition of functions necessary for the work with Win32 platform layer. 
 * @version 0.2
 * @date 2022-07-19
 * ================================================================================
 */

/* Standard library includes. */
#undef UNICODE
#include <windows.h>

/* Game engine includes. */
#include "utils.h"
#include "game.h"
#include "game_worker.h"
#include "render.h"
#include "keyboard.h"
#include "mouse.h"
#include "dbg.h"

/* Static variables: */
static Game_t *game;  /* Pointer to the Game structure. */
static Game_Worker_t *game_worker;  /* Game worker data. */
static Render_t *render;  /* Pointer to the Render structure. */
static Keyboard_t *keyboard;  /* Pointer to the Keyboard structure. */
static Mouse_t *mouse;  /* Pointer to the Mouse structure. */

/* Static functions declaration */
/**
 * @brief Window callback function for window messages processing.
 * @param window Handle to the window.
 * @param message Particular message code.
 * @param w_param Additional parameter for message processing.
 * @param l_param Additional parameter for message processing.
 * @return LRESULT Standard return from a callback function.
 */
static LRESULT CALLBACK 
window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param);

/**
 * @brief Function for the entry point of the application. 
 * @param hInstance A handle to the current instance of the application.
 * @param hPrevInstance A handle to the previous instance of the application (NULL).
 * @param lpCmdLine The command line for the application, excluding the program name. 
 * @param nCmdShow Controls how the window is to be shown.
 * @return int 
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
    window_class.lpfnWndProc = window_callback;
    window_class.lpszClassName = "Game";

    /* Register the window class. */
    if (!RegisterClass(&window_class))
    {
        dbg_error("%s", "Windows class was not registered correctly!");
    }

    game = game_constructor();
    game_init(game, GST_MEMORY_ALLOCATION, GM_NORMAL);
    game->is_running = true;

    /* Create a window and return a window handle. */
    HWND window;
    DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
    window = CreateWindowEx(0, window_class.lpszClassName, "Game", dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, 0);
    
    /* Check if the window was successfully created. */
    if (window == NULL)
    {
        dbg_error("%s", "Window was not successfully created!");
    }

    /* Preparations for a message processing loop. */
    MSG message;
    
    /* Starting a message processing loop. */
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
    
    /* TODO: Call destructors of the created objects. */
    return 0;
}

static LRESULT CALLBACK 
window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
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
        /* Create different game objects and initialize it. */
        keyboard = keyboard_constructor();
        mouse = mouse_constructor();

        render = render_constructor();
        render_init(render, window);

        game_worker = game_worker_constructor();
        game_worker_init(game_worker, game, keyboard, mouse, render);  
        CreateThread(0, 0, game_worker_proc, game_worker, 0, 0);
    } break;
    
    /* Resizing of the window message. */
    case WM_SIZE:
    {
        render_resize_window(render);
    } break;

    /* Processing messages when keys are pressed and released. */
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {   
        keyboard_define_temp_key_data(keyboard, w_param, l_param);
        keyboard_process_keys(keyboard);
    } break;

    /* Processing messages for mouse in the game window. */
    case WM_MOUSEMOVE:
    {
        GetCursorPos(&(mouse->raw_cursor));
        ScreenToClient(window, &(mouse->raw_cursor));
        mouse_prepare_input(mouse, render);
    } break;

    /* Default message and uncatched messages: */
    default:
    {
        result = DefWindowProc(window, message, w_param, l_param);
    }}
    return result;
}
