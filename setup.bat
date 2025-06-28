@echo off
echo === Setting up project ===

:: Step 1: Initialize submodules (e.g. vcpkg)
git submodule update --init --recursive

:: Step 2: Bootstrap vcpkg
cd third-party\vcpkg
call bootstrap-vcpkg.bat

:: Step 3: Install dependencies
vcpkg install libosmium:x64-windows

:: Step 4: Integrate vcpkg with Visual Studio (one-time)
vcpkg integrate install

cd ../..
echo === Setup complete ===
pause