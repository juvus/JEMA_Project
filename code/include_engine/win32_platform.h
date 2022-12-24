/**
 * ================================================================================
 * @file include_engine/helper_functions.h
 * @author Dmitry Safonov (juvusoft@gmail.com)
 * @brief Declaration of objects and functions necessary for the work with Win32
 * platform layer. Platform layer contains all supply objects for the game.
 * @version 0.1
 * @date 2022-12-22
 * ================================================================================
 */

#ifndef JEMA_ENGINE_WIN32_PLATFORM_H_
#define JEMA_ENGINE_WIN32_PLATFORM_H_

#include <windows.h>

typedef struct Audio_ Audio;
typedef struct AudioWorker_ AudioWorker;
typedef struct Keyboard_ Keyboard;
typedef struct Mouse_ Mouse;
typedef struct Render_ Render;

/**
 * @brief Structure for the Win32 platform object.
 */
struct Win32Platform_ 
{
    Audio *audio;  /**< Pointer to the Audio structure. */
    AudioWorker *audio_worker;  /**< Pointer to the AudioWorker structure. */
    Keyboard *keyboard;  /**< Pointer to the Keyboard structure. */
    Mouse *mouse;  /**< Pointer to the Mouse structure. */
    Render *render;  /**< Pointer to the Render structure. */
};
typedef struct Win32Platform_ Win32Platform;

/**
 * @brief Object constructor.
 * @return Win32Platform* Pointer to the Win32Platform structure. 
 */
Win32Platform*
Win32Platform_Constructor(void);

/**
 * @brief Object destructor.
 * @param win32_platform Pointer to the Win32Platform structure.
 */
void
Win32Platform_Destructor(Win32Platform *win32_platform);

/**
 * @brief Initialization of the Win32 platform layer.
 * @param win32_platform Pointer to the Win32Platform structure.
 */
void
Win32Platform_Init(Win32Platform *win32_platform);

#endif  /* JEMA_ENGINE_WIN32_PLATFORM_H_ */
