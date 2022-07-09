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

cl -FC -Zi -W4 -WX /I ..\code\include ..\code\src\win32_platform.c ..\code\src\render.c ^
    ..\code\src\game.c ..\code\src\keyboard_input.c ..\code\src\mouse_input.c ^
    ..\code\src\sound.c ..\code\src\sound_buffer.c ..\code\src\wav_uncompressor.c ^
    ..\code\src\image.c ..\code\src\font.c ..\code\src\debug_console.c ^
    ..\code\src\file_io.c ..\code\src\helper_functions.c user32.lib gdi32.lib

:: pop the directory 
popd
