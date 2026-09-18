@echo off
setlocal EnableExtensions EnableDelayedExpansion
cd /d "%~dp0"

rem OverKartDecomp Windows build helper (errors-focused)
rem Same actions as build.bat, but console output drops unused-variable
rem noise and other low-signal unused* warnings. Full log is still saved.
rem Default make flags: AVOID_UB=1 COMPARE=0
rem Original full-output launcher: build.bat

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

echo ==== OverKartDecomp Windows build (errors-focused) ====
echo Root:    %CD%
echo Action:  %ACTION%
echo Version: %VERSION%
echo Flags:   AVOID_UB=1 COMPARE=0
echo Note:    unused-variable warnings hidden; full log -^> _build_full.log
echo.

set "RUNSH=_build_run.sh"
set "FULLLOG=_build_full.log"
set "ROOT_POSIX=%CD:\=/%"

> "%RUNSH%" (
  echo #!/bin/sh
  echo set -e
  echo export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
  echo cd "%ROOT_POSIX%"
  echo echo START:$(date^)
)

if /I "%ACTION%"=="rom" (
  >> "%RUNSH%" echo make AVOID_UB=1 COMPARE=0 -j VERSION=%VERSION%
  echo Cmd: make AVOID_UB=1 COMPARE=0 -j VERSION=%VERSION%
)
if /I "%ACTION%"=="build" (
  >> "%RUNSH%" echo make AVOID_UB=1 COMPARE=0 -j VERSION=%VERSION%
  echo Cmd: make AVOID_UB=1 COMPARE=0 -j VERSION=%VERSION%
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
  >> "%RUNSH%" echo make AVOID_UB=1 COMPARE=0 -j VERSION=%VERSION%
  echo Cmd: make -C tools -j ; make assets -j ; make AVOID_UB=1 COMPARE=0 -j
)

>> "%RUNSH%" echo echo MAKE_EXIT:$?
>> "%RUNSH%" echo echo DONE:$(date^)

echo.
echo sh %RUNSH% ^& exit | "%W64DEVKIT%" > "%FULLLOG%" 2>&1
set "EC=!ERRORLEVEL!"

del /q "%RUNSH%" 2>nul

echo.
echo ---- filtered console (serious / non-unused) ----
rem Drop unused-variable / unused-parameter style noise; keep errors, make stops, OK/FAIL, other warnings.
powershell -NoProfile -Command ^
  "$p='-Wunused-variable|-Wunused-parameter|-Wunused-but-set|-Wunused-value|unused variable|unused parameter|set but not used|unused but set';" ^
  "Get-Content -LiteralPath '%FULLLOG%' | ForEach-Object { $_ -replace '\x1b\[[0-9;]*m','' } | Where-Object { $_ -notmatch $p }"

echo.
echo ---- end filtered console ----
echo Full log: %FULLLOG%

if not "!EC!"=="0" (
  echo [fail] w64devkit exited !EC!
  echo Note: check MAKE_EXIT / compiler errors in the filtered output or %FULLLOG%.
  exit /b !EC!
)

echo [done] Look for MAKE_EXIT:0 and mk64.%VERSION%: OK above ^(or in %FULLLOG%^).
exit /b 0

:help
echo Usage: build_errors.bat [action] [version]
echo.
echo Same actions as build.bat, but console output hides unused-variable
echo (and similar unused*) warnings. Full unfiltered log: _build_full.log
echo Default make flags: AVOID_UB=1 COMPARE=0
echo.
echo Actions:
echo   rom      Build ROM only   (default)   make AVOID_UB=1 COMPARE=0 -j
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
echo   build_errors.bat
echo   build_errors.bat rom us
echo   build_errors.bat full us
echo.
echo OverKart-flagged build: build_overkart.bat
echo Original full-output launcher: build.bat
echo Requires: tools\mingw64\w64devkit.exe
echo           baserom.^<version^>.z64 for assets/rom builds
exit /b 0
