/**
 * ================================================================================
 * @file win32_platform.c
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Win32 platform layer for creation window and starting main game loop.
 * @version 0.1
 * @date 2021-12-18
 * @copyright JuvuSoft (c) 2021
 * ================================================================================
 */

/* Standard includes: */
#undef UNICODE
#include <windows.h>
#include <assert.h>

/* Program includes: */
#include <utils.h>
#include <render.h>
#include <game.h>
#include <keyboard_input.h>
#include <mouse_input.h>
#include <sound_buffer.h>

/* Static variables: */
static Game_t game = {0};  /* Game object. */
static Render_t *render;  /* Render for the whole window bitmap. */
static Keyboard_Input_t keyboard_input = {0};  /* Structure storing the keyboard input. */
static Mouse_Input_t mouse_input = {0};  /* Structure storing the mouse input. */
static Sound_buffer_t sound_buffer;  /* Buffer for the audio stream. */
static HWND window;  /* Handle to the window. */

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
 * @brief Main game loop procedure organized as a separate thread
 * @param worker_data Pointer to the worker data
 * @return DWORD Standard return from the function
 */
static DWORD WINAPI 
game_main_loop_proc(void *worker_data);

/**
 * @brief Function for creation of a DIB section object.
 * This function is intended to be called only once when the WM_CREATE message
 * is processing resulting in allocation a memory for the bitmap buffer. After 
 * resizing of the window pixels of the bitmap will be streched to a new window
 * size.
 * @param window A handle to the window.
 */
static void 
create_DIB_section(HWND window);

/**
 * @brief Function for processing the resizing a window
 * @param window Handle to the window
 */
static void 
resize_window(HWND window); /* TODO: add pointer to the render. */

/**
 * @brief Function for update the window by stretching DI bits 
 * @param hdc Handle to device context
 */
static void 
update_window(HDC hdc);

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
    /* Avoid warning messages about not used function parameters */
    UNUSED(hPrevInstance);
    UNUSED(lpCmdLine);
    UNUSED(nCmdShow); 

    /* Create a window class */
    WNDCLASS window_class = {0};
    window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    window_class.hInstance = hInstance;
    window_class.lpfnWndProc = window_callback;
    window_class.lpszClassName = "Game";

    /* Register the window class */
    if (!RegisterClass(&window_class))
    {
        assert(0 && "ERROR: Windows class was not registered correctly");
    }

    /* Create a window and return a window handle */
    DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
    window = CreateWindowEx(0, window_class.lpszClassName, "Game", dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, 0);
    /* Check if the window was successfully created */
    if (window == NULL) {
        assert(0 && "ERROR: Window was not successfully created");
    }
    
    /* Initialization of the audio buffer */
    sound_buffer_init(window, &sound_buffer);

    /* Preparations for a message processing loop */
    MSG message;
    BOOL message_result;
    game.game_state = GST_MEMORY_ALLOCATION;
    game.is_running = true;
    
    /* Game update and render loop (as a separate thread) */  
    CreateThread(0, 0, game_main_loop_proc, NULL, 0, 0);

    while (game.is_running)
    {
        /* Get and process messages */
        while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
             /* Handle the message */
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        /* Reduce the CPU usage time */
        Sleep(1);
    }
    
    render_destructor(render);
    return 0;
}

/**
 * @brief Windows callback function for message processing. 
 * @param window A handle for a window
 * @param message Current message for processing
 * @param w_param Additional parameter
 * @param l_param Additional parameter
 * @return LRESULT Windows returning parameter
 */
static LRESULT CALLBACK 
window_callback(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
    LRESULT result = 0;

    /* Handling the messages */
    switch (message)
    {   
    /* Window closing and destroying messages: */
    case WM_CLOSE:
    case WM_DESTROY:
    {
        game.is_running = false;
    } break;

    /* Creating the window message */
    case WM_CREATE:
    {
        /* Create render and initialize it. */
        render = render_constructor();
        render_init(render, window);
    } break;
    
    /* Resizing of the window message: */
    case WM_SIZE:
    {
        /* Resize the window */
        resize_window(window);
    } break;

    /* Processing messages when keys are pressed and released */
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    {   
        /* Calculate the temp key data */
        keyboard_input_define_temp_key_data(&keyboard_input, w_param, l_param);
        
        /* Prepare the user keyboard input */
        keyboard_input_check_keys(&keyboard_input);
    } break;

    /* Processing messages for mouse in the game window */
    case WM_MOUSEMOVE:
    {
        /* Get and prepare the raw cursor data */
        GetCursorPos(&(mouse_input.raw_cursor));
        ScreenToClient(window, &(mouse_input.raw_cursor));
    
        /* Prepare the user mouse input */
        mouse_input_prepare_input(&mouse_input, &game);
    } break;

    /* Default message and uncatched messages: */
    default:
    {
        result = DefWindowProc(window, message, w_param, l_param);
    }}
    return result;
}

/**
 * @brief Main game loop procedure organized as a separate thread
 * @param worker_data Pointer to the worker data
 * @return DWORD Standard return from the function
 */
static DWORD WINAPI 
game_main_loop_proc(void *worker_data)
{
    while (game.is_running)
    {
        /* Game simulation procedure */ 
        game_simulate_tick(&game, &keyboard_input, &mouse_input, render, &sound_buffer);

        /* Update window */
        update_window();

        /* Reduce the CPU usage time */
        Sleep(1);
    }
    return(0);
}

/**
 * @brief Function for creation of a DIB section object.
 * This function is intended to be called only once when the WM_CREATE message
 * is processing resulting in allocation a memory for the bitmap buffer. After 
 * resizing of the window pixels of the bitmap will be streched to a new window
 * size.
 * @param window A handle to the window.
 */
static void 
create_DIB_section(HWND window)
{ 
    /* TODO: This function should be in render_init method. */
    
    //RECT rect;  /* Rectangle for holding the info about the graphic window. */
    //GetClientRect(window, &rect);

    /* Calculate width and height of the window (client area) and render buffer */
    //render_buffer.width = (u32)(rect.right - rect.left);
    //render_buffer.height = (u32)(rect.bottom - rect.top);
    //game.width = render_buffer.width;
    //game.height = render_buffer.height;

    //if (render_buffer.bitmap_memory) 
    //{
    //    /* Delete the bitmap memory if it was previously allocated */
    //    VirtualFree(render_buffer.bitmap_memory, 0, MEM_RELEASE);
    //    render_buffer.bitmap_memory = NULL;
    //}

    /* Create the bitmap memory */
    //SIZE_T mem_size;  /* Render buffer memory size. */
    //mem_size = sizeof(u32) * render_buffer.width * render_buffer.height;
    //render_buffer.bitmap_memory = VirtualAlloc(0, mem_size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

    /* Fill the bitmap_info structure */
    //render_buffer.bitmap_info.bmiHeader.biSize = sizeof(render_buffer.bitmap_info.bmiHeader);
    //render_buffer.bitmap_info.bmiHeader.biWidth = render_buffer.width;
    //render_buffer.bitmap_info.bmiHeader.biHeight = render_buffer.height;
    //render_buffer.bitmap_info.bmiHeader.biPlanes = 1;
    //render_buffer.bitmap_info.bmiHeader.biBitCount = 32;
    //render_buffer.bitmap_info.bmiHeader.biCompression = BI_RGB;
}

/**
 * @brief Function for processing the resizing a window
 * @param window Handle to the window
 */
static void 
resize_window(HWND window)
{    
    RECT rect;
    GetClientRect(window, &rect);
    
    game.width = (u32)(rect.right - rect.left);
    game.height = (u32)(rect.bottom - rect.top);
}

/**
 * @brief Function for update the window by stretching DI bits 
 * @param hdc Handle to device context
 */
static void 
update_window(HDC hdc)
{
    /* Render the calculated bitmap */
    StretchDIBits(hdc, 0, 0, (int)game.width, (int)game.height, 0, 0,
    (int)render_buffer.width, (int)render_buffer.height, render_buffer.bitmap_memory,
    &render_buffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
}