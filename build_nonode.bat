@echo off
if not exist build mkdir build
if not exist build\objects mkdir build\objects

setlocal enabledelayedexpansion

set FILES=
for /R src %%f in (*.cpp) do (
    set FILES=!FILES! "%%f"
)



cl.exe /Zi /EHsc /nologo /Fobuild\objects\ /Fdbuild\\minesweeper.pdb /Febuild\\minesweeper.exe !FILES!
endlocal
