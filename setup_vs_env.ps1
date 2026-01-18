# setup_vs_env.ps1
# 设置 Visual Studio 2022 开发环境
# 用法: .\setup_vs_env.ps1 [arch]
# arch: 架构参数，如 x64, x86, arm64 等，默认 x64

param(
    [string]$Arch = "x64"
)

Write-Host "Setting up Visual Studio 2022 $Arch environment..."

# 执行 vcvarsall.bat 并显示环境变量
cmd /c "`"D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat`" $Arch 10.0.26100.0 && set"

Write-Host "Environment setup complete."