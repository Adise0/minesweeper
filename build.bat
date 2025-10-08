@echo off
if not exist build mkdir build
if not exist build\objects mkdir build\objects

setlocal enabledelayedexpansion



@REM set FILES=
@REM for /R src %%f in (*.cpp) do (
@REM     set FILES=!FILES! "%%f"
@REM )



@REM cl.exe /Zi /EHsc /nologo /Fobuild\objects\ /Fdbuild\\app.pdb /Febuild\\app.exe !FILES!

call node ./build.js
endlocal

if errorlevel 1 (
  echo Build failed.
  exit /b %errorlevel%
) else (
  echo Build succeeded. 
  cls
)

