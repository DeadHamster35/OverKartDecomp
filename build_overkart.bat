@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

rem OverKartDecomp Windows build with OVERKART_BUILD=1
rem Flags: AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1
rem Deletes build\<version>\mk64.ld first so the linker script regenerates
rem for OverKart (required when switching from OVERKART_BUILD=0).

set "W64DEVKIT=tools\mingw64\w64devkit.exe"
if not exist "%W64DEVKIT%" (
  echo [error] Missing "%W64DEVKIT%"
  echo Unpack / place w64devkit under tools\mingw64 first.
  exit /b 1
)

set "ACTION=%~1"
if "%ACTION%"=="" set "ACTION=rom"

set "VERSION=%~2"
if "%VERSION%"=="" set "VERSION=us"

if /I "%ACTION%"=="/?" goto :help
if /I "%ACTION%"=="help" goto :help
if /I "%ACTION%"=="-h" goto :help

if /I not "%ACTION%"=="rom" if /I not "%ACTION%"=="build" if /I not "%ACTION%"=="assets" if /I not "%ACTION%"=="tools" if /I not "%ACTION%"=="clean" if /I not "%ACTION%"=="full" (
  echo [error] Unknown action: %ACTION%
  echo.
  goto :help
)

if exist "C:\Program Files\SmartGit\git\cmd\git.exe" (
  set "PATH=C:\Program Files\SmartGit\git\cmd;%PATH%"
)

echo ==== OverKartDecomp OVERKART_BUILD=1 ====
echo Root:    %CD%
echo Action:  %ACTION%
echo Version: %VERSION%
echo Flags:   AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1
echo.

rem Regenerate linker script when switching OverKart on
if exist "build\%VERSION%\mk64.ld" (
  echo Removing build\%VERSION%\mk64.ld for OverKart relink...
  del /q "build\%VERSION%\mk64.ld" 2>nul
)

set "RUNSH=_build_run.sh"
set "ROOT_POSIX=%CD:\=/%"

> "%RUNSH%" (
  echo #!/bin/sh
  echo set -e
  echo export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
  echo cd "%ROOT_POSIX%"
  echo echo START:$(date^)
)

if /I "%ACTION%"=="rom" (
  >> "%RUNSH%" echo make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j VERSION=%VERSION%
  echo Cmd: make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j VERSION=%VERSION%
)
if /I "%ACTION%"=="build" (
  >> "%RUNSH%" echo make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j VERSION=%VERSION%
  echo Cmd: make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j VERSION=%VERSION%
)
if /I "%ACTION%"=="assets" (
  >> "%RUNSH%" echo make assets -j VERSION=%VERSION%
  echo Cmd: make assets -j VERSION=%VERSION%
)
if /I "%ACTION%"=="tools" (
  >> "%RUNSH%" echo make -C tools -j
  echo Cmd: make -C tools -j
)
if /I "%ACTION%"=="clean" (
  >> "%RUNSH%" echo make clean VERSION=%VERSION%
  echo Cmd: make clean VERSION=%VERSION%
)
if /I "%ACTION%"=="full" (
  >> "%RUNSH%" echo make -C tools -j
  >> "%RUNSH%" echo make assets -j VERSION=%VERSION%
  >> "%RUNSH%" echo make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j VERSION=%VERSION%
  echo Cmd: make -C tools -j ; make assets -j ; make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j
)

>> "%RUNSH%" echo echo MAKE_EXIT:$?
>> "%RUNSH%" echo echo DONE:$(date^)

echo.
echo sh %RUNSH% ^& exit | "%W64DEVKIT%"
set "EC=!ERRORLEVEL!"

del /q "%RUNSH%" 2>nul

echo.
if not "!EC!"=="0" (
  echo [fail] w64devkit exited !EC!
  echo Note: check the log above for MAKE_EXIT / compiler errors too.
  exit /b !EC!
)

echo [done] Look for MAKE_EXIT:0 and mk64.%VERSION%: OK above.
exit /b 0

:help
echo Usage: build_overkart.bat [action] [version]
echo.
echo OverKart-flagged build. Same actions as build.bat, but runs:
echo   make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j
echo and deletes build\^<version^>\mk64.ld first so the linker script
echo regenerates for OVERKART_BUILD=1.
echo.
echo Actions:
echo   rom      Build ROM only   (default)
echo   build    Same as rom
echo   assets   Extract assets
echo   tools    Build host tools
echo   clean    Clean VERSION build tree
echo   full     tools + assets + rom
echo   help     This text
echo.
echo Version: us (default), eu.v10, eu.v11, ...
echo.
echo Examples:
echo   build_overkart.bat
echo   build_overkart.bat rom us
echo   build_overkart.bat full us
echo.
echo Non-OverKart build: build.bat  ^(AVOID_UB=1 COMPARE=0 only^)
echo Requires: tools\mingw64\w64devkit.exe
echo           baserom.^<version^>.z64 for assets/rom builds
exit /b 0
