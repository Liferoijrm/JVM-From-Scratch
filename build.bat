@echo off
REM Windows batch script to build the JVM project
REM Usage: build.bat [all|clean|debug]

setlocal enabledelayedexpansion

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c99 -I %SRC_DIR%/class_loader/loading
set DEBUG_FLAGS=-g -fsanitize=address,undefined
set SRC_DIR=src
set BUILD_DIR=build
set BIN_DIR=bin
set TARGET_NAME=jvm.exe

REM Create directories if they don't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

REM Default target
if "%1"=="" (
    set TARGET=all
) else (
    set TARGET=%1
)

if "%TARGET%"=="all" goto :build
if "%TARGET%"=="clean" goto :clean
if "%TARGET%"=="debug" goto :debug
echo Usage: build.bat [all^|clean^|debug]
goto :end

:build
echo Compiling JVM project...
for /r "%SRC_DIR%" %%F in (*.c) do (
    set "FILE=%%F"
    set "OUTFILE=!FILE:%SRC_DIR%=%BUILD_DIR%!"
    set "OUTFILE=!OUTFILE:.c=.o!"
    
    REM Create subdirectory for object file if needed
    for %%D in ("!OUTFILE!") do set "OUTDIR=%%~dpD"
    if not exist "!OUTDIR!" mkdir "!OUTDIR!"
    
    echo Compiling %%F...
    %CC% %CFLAGS% -c "%%F" -o "!OUTFILE!"
)

echo Gathering object files...
setlocal enabledelayedexpansion
set "OBJS="
for /r "%BUILD_DIR%" %%F in (*.o) do (
    set "OBJS=!OBJS! %%F"
)

echo Linking...
%CC% %CFLAGS% !OBJS! -o "%BIN_DIR%\%TARGET_NAME%"
echo Build complete. Executable: %BIN_DIR%\%TARGET_NAME%
goto :end

:clean
echo Cleaning build artifacts...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
if exist "%BIN_DIR%" rmdir /s /q "%BIN_DIR%"
echo Clean complete.
goto :end

:debug
echo Cleaning previous build...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
if exist "%BIN_DIR%" rmdir /s /q "%BIN_DIR%"
mkdir "%BUILD_DIR%"
mkdir "%BIN_DIR%"
echo Compiling with debug flags...
set "CFLAGS=%CFLAGS% %DEBUG_FLAGS%"
for /r "%SRC_DIR%" %%F in (*.c) do (
    set "FILE=%%F"
    set "OUTFILE=!FILE:%SRC_DIR%=%BUILD_DIR%!"
    set "OUTFILE=!OUTFILE:.c=.o!"
    
    for %%D in ("!OUTFILE!") do set "OUTDIR=%%~dpD"
    if not exist "!OUTDIR!" mkdir "!OUTDIR!"
    
    echo Compiling %%F...
    %CC% %CFLAGS% -c "%%F" -o "!OUTFILE!"
)

echo Linking with debug symbols...
%CC% %CFLAGS% %BUILD_DIR%\*.o -o "%BIN_DIR%\%TARGET_NAME%"
echo Debug build complete. Executable: %BIN_DIR%\%TARGET_NAME%
goto :end

:end
endlocal
