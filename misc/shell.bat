@echo off
:: open and set-up the shell for building the program.

:: setup the cl compilator.
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

:: create w: disk from directory for easy use.
subst w: /D
subst w: D:\Programming\C\JEMA_Project
w:

:: set additional paths.
set PATH=%PATH%;D:\Programming\C\JEMA_Project\misc\;C:\msys64\mingw64\bin

:: echo the message.
echo Everything was set up successfully!

:: remain the cmd opened.
cmd /k