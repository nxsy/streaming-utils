@echo off
set RESIZER_VIMRC=%~dp0\vimrc
start /b cmd /c gvim +"source %~dp0\vimrc"
