@echo off
SETLOCAL

REM setup vcpkg path
set VCPKG_EXE=Vendor\VCPKG\vcpkg.exe

REM check if vcpkg exists
if not exist %VCPKG_EXE% (
	echo [ERROR] vcpkg.exe not found, please run "Vendor/VCPKG/bootstrap-vcpkg.bat" first
	pause
	exit /B 1
)

REM install dependencies
echo Installing dependencies....
%VCPKG_EXE% install sdl3 sdl3-image

if %errorlevel% neq 0 (
	echo [ERROR] Installation failed.
	pause
	exit /B 1
)

echo [SUCCESS] Dependencies installed successfully
ENDLOCAL
PAUSE