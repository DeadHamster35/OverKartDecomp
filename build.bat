@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

rem KimuraDecomp Windows build helper
rem Drives the bundled MinGW toolchain via tools\mingw64\w64devkit.exe

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

rem Optional git on PATH for tools/submodule steps (SmartGit install)
if exist "C:\Program Files\SmartGit\git\cmd\git.exe" (
  set "PATH=C:\Program Files\SmartGit\git\cmd;%PATH%"
)

echo ==== KimuraDecomp Windows build ====
echo Root:    %CD%
echo Action:  %ACTION%
echo Version: %VERSION%
echo.

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
  >> "%RUNSH%" echo make -j VERSION=%VERSION%
  echo Cmd: make -j VERSION=%VERSION%
)
if /I "%ACTION%"=="build" (
  >> "%RUNSH%" echo make -j VERSION=%VERSION%
  echo Cmd: make -j VERSION=%VERSION%
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
  >> "%RUNSH%" echo make -j VERSION=%VERSION%
  echo Cmd: make -C tools -j ; make assets -j ; make -j
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
echo Usage: build.bat [action] [version]
echo.
echo Actions:
echo   rom      Build ROM only   (default)   make -j
echo   build    Same as rom
echo   assets   Extract assets               make assets -j
echo   tools    Build host tools             make -C tools -j
echo   clean    Clean VERSION build tree     make clean
echo   full     tools + assets + rom
echo   help     This text
echo.
echo Version: us (default), eu.v10, eu.v11, ...
echo.
echo Examples:
echo   build.bat
echo   build.bat rom us
echo   build.bat assets
echo   build.bat full us
echo.
echo Requires: tools\mingw64\w64devkit.exe
echo           baserom.^<version^>.z64 for assets/rom builds
exit /b 0
