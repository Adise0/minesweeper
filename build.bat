@echo off
if not exist build mkdir build
if not exist build\objects mkdir build\objects

setlocal enabledelayedexpansion


call node ./build.js
endlocal

if errorlevel 1 (
  echo Build failed.
  exit /b %errorlevel%
) else (
  echo Build succeeded. 
  cls
)

