@echo off

:: check if the directory "build" is created, if not then create it
mkdir ..\build

:: push the directory
pushd ..\build

:: Build the program.
:: -Zi Add debug information.
:: -FC Show full passes of the possible errors.
:: -W4 4th level of warnings.
:: -WX Treat warnings as errors.
:: /wd Disable warnings.
::      C4201 - Unnamed structures and enumerators.
::      C4189 - Unused local variable.

cl -FC -Zi -W4 -WX ^
    /Fe: Game ^
    /wd4201 /wd4189 ^
    /I ..\code ^
    ..\code\src_engine\audio_worker.c ^
    ..\code\src_engine\audio.c ^
    ..\code\src_engine\color.c ^
    ..\code\src_engine\debug_console.c ^
    ..\code\src_engine\font.c ^
    ..\code\src_engine\helper_functions.c ^
    ..\code\src_engine\image.c ^
    ..\code\src_engine\keyboard.c ^
    ..\code\src_engine\math_functions.c ^
    ..\code\src_engine\matrix22.c ^
    ..\code\src_engine\matrix33.c ^
    ..\code\src_engine\memory_object.c ^
    ..\code\src_engine\mouse.c ^
    ..\code\src_engine\random.c ^
    ..\code\src_engine\render.c ^
    ..\code\src_engine\sound.c ^
    ..\code\src_engine\vector2.c ^
    ..\code\src_engine\vector3.c ^
    ..\code\src_engine\wav_decoder.c ^
    ..\code\src_engine\win32_platform.c ^
    ..\code\src_game\game_resourses.c ^
    ..\code\src_game\game_worker.c ^
    ..\code\src_game\game.c ^
    user32.lib gdi32.lib

:: pop the directory 
popd
