@echo off
REM setup_vs_env.bat
REM 设置 Visual Studio 2022 开发环境
REM 用法: setup_vs_env.bat [arch]
REM arch: 架构参数，如 x64, x86, arm64 等，默认 x64

if "%1"=="" (
    set ARCH=x64
) else (
    set ARCH=%1
)

echo Setting up Visual Studio 2022 %ARCH% environment...

call "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" %ARCH% 10.0.26100.0

if %errorlevel% neq 0 (
    echo Failed to set up environment.
    exit /b 1
)

set

echo Environment setup complete.