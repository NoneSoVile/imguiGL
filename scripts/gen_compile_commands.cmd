@echo off
setlocal enableextensions enabledelayedexpansion

call "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 (
  echo Failed to set up MSVC environment.
  exit /b 1
)

cmake -S "%~dp0.." -B "%~dp0..\build-cc" -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
if errorlevel 1 exit /b 1

echo compile_commands.json generated at build-cc\compile_commands.json
