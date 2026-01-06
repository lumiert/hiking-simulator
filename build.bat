@echo off
REM ============================================================
REM Hiking Simulator - Build Script for SDL2/OpenGL
REM Usage: build.bat [all|clean|test|demo]
REM ============================================================

setlocal enabledelayedexpansion

set "TARGET=%1"
if "%TARGET%"=="" set "TARGET=all"

set "DEPS_DIR=%CD%\dependencies"
set "SDL2_DIR=!DEPS_DIR!\SDL2-2.28.5\x86_64-w64-mingw32"
set "GLEW_DIR=!DEPS_DIR!\glew-2.2.0"
set "GLM_DIR=!DEPS_DIR!\glm"

set "CFLAGS=-std=c++17 -Wall -O2"
set "INCLUDES=-I. -I"!SDL2_DIR!\include" -I"!GLEW_DIR!\include" -I"!GLM_DIR!""
set "LIBS=-L"!SDL2_DIR!\lib" -L"!GLEW_DIR!\lib\Release\x64" -lmingw32 -lSDL2main -lSDL2 -lglew32 -lopengl32 -static-libgcc -static-libstdc++"

echo.
echo ============================================================
echo  Hiking Simulator Build System
echo ============================================================
echo.
echo [INFO] Compiler: g++ (MinGW-W64 15.2.0)
echo [INFO] Target: %TARGET%
echo.

if "%TARGET%"=="all" goto build_all
if "%TARGET%"=="clean" goto clean
if "%TARGET%"=="test" goto build_test
if "%TARGET%"=="demo" goto demo
goto error_target

:build_all
echo [1/2] Compiling main application (hiking.exe)...
g++ main_graphics.cpp %CFLAGS% %INCLUDES% -o hiking.exe %LIBS%
if errorlevel 1 goto end_error
echo [OK] hiking.exe created

echo.
echo [2/2] Compiling test version (test.exe)...
g++ main_test.cpp %CFLAGS% %INCLUDES% -o test.exe %LIBS%
if errorlevel 1 goto end_error
echo [OK] test.exe created

echo.
echo ============================================================
echo  BUILD SUCCESSFUL
echo ============================================================
echo.
goto end_success

:build_test
echo [INFO] Building test version...
g++ main_test.cpp %CFLAGS% %INCLUDES% -o test.exe %LIBS%
if errorlevel 1 goto end_error
echo [OK] test.exe created
goto end_success

:demo
echo [INFO] Running Python demo...
python demo.py
goto end_success

:clean
echo [INFO] Cleaning build files...
if exist hiking.exe del hiking.exe
if exist test.exe del test.exe
echo [OK] Clean complete
goto end_success

:error_target
echo.
echo [ERROR] Unknown target: %TARGET%
echo Usage: build.bat [all^|clean^|test^|demo]
echo.
goto end_error

:end_error
echo [FAILED] Build failed
exit /b 1

:end_success
echo.
exit /b 0
