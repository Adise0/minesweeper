@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
start conhost.exe "%SCRIPT_DIR%/build/minesweeper.exe"
endlocal
