@echo off
echo 正在更新 clangd 索引配置...
echo.

REM 生成 compile_commands.json
echo 1. 生成 compile_commands.json...
python "%~dp0generate_compile_commands.py"

if %errorlevel% neq 0 (
    echo 错误: 生成 compile_commands.json 失败
    pause
    exit /b 1
)

echo.
echo 2. 配置文件已更新:
echo    - compile_commands.json: 包含所有源文件的编译命令
echo    - .clangd: clangd 配置文件已优化
echo.

echo 3. 建议接下来的步骤:
echo    a) 在 VS Code 中按 Ctrl+Shift+P 
echo    b) 输入 "clangd: restart" 并执行
echo    c) 等待索引完成（可能需要几分钟）
echo.
echo    或者简单地重启 VS Code
echo.

echo 完成后，你应该能够:
echo - 正确跳转到函数定义
echo - 自动补全工作正常
echo - 无需先打开文件即可跳转
echo.

pause