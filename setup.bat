@echo off
setlocal

REM === Step 1: Bootstrap vcpkg if needed ===
cd /d "%~dp0third-party\vcpkg"
if not exist vcpkg.exe (
    echo Bootstrapping vcpkg...
    bootstrap-vcpkg.bat
) else (
    echo vcpkg already bootstrapped.
)

REM === Step 2: Update vcpkg ===
echo Updating vcpkg...
git pull

REM === Step 3: Install dependencies via vcpkg ===
echo Installing required packages via vcpkg...
vcpkg.exe install sdl3:x64-windows
vcpkg.exe install imgui[sdl3-binding,opengl3-binding]:x64-windows
vcpkg.exe install gl3w:x64-windows
vcpkg.exe install libosmium:x64-windows
vcpkg.exe install boost:x64-windows
vcpkg.exe install expat:x64-windows

REM === Step 4: Pull latest changes in main repo ===
cd /d "%~dp0"
echo Pulling latest changes in main repo...
git pull

REM === Step 5: Initialize and update all submodules recursively ===
echo Initializing and updating git submodules...
git submodule update --init --recursive

REM === Step 6: Copy ImGui backend files ===
set PROJECT_ROOT=%~dp0
if "%PROJECT_ROOT:~-1%"=="\" set PROJECT_ROOT=%PROJECT_ROOT:~0,-1%

set IMGUI_BACKENDS_DIR=%PROJECT_ROOT%\third-party\imgui\backends
set DEST_BACKENDS_DIR=%PROJECT_ROOT%\imgui_backends

if not exist "%DEST_BACKENDS_DIR%" (
    echo Creating directory: %DEST_BACKENDS_DIR%
    mkdir "%DEST_BACKENDS_DIR%"
)

echo Copying ImGui backend files from:
echo   %IMGUI_BACKENDS_DIR%
echo to:
echo   %DEST_BACKENDS_DIR%

copy /Y "%IMGUI_BACKENDS_DIR%\imgui_impl_sdl3.cpp" "%DEST_BACKENDS_DIR%"
copy /Y "%IMGUI_BACKENDS_DIR%\imgui_impl_sdl3.h"   "%DEST_BACKENDS_DIR%"
copy /Y "%IMGUI_BACKENDS_DIR%\imgui_impl_opengl3.cpp" "%DEST_BACKENDS_DIR%"
copy /Y "%IMGUI_BACKENDS_DIR%\imgui_impl_opengl3.h"   "%DEST_BACKENDS_DIR%"

echo Copy complete.

echo Setup complete!

endlocal
pause
