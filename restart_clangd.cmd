@echo off
echo 重启 clangd 服务以更新索引...
echo.
echo 请在 VS Code 中执行以下操作:
echo 1. 按 Ctrl+Shift+P 打开命令面板
echo 2. 输入 "clangd: restart" 并执行
echo 3. 等待索引完成
echo.
echo 或者你可以:
echo 1. 重启 VS Code
echo 2. 等待 clangd 重新索引项目
echo.
echo compile_commands.json 已生成，.clangd 配置已更新
echo 现在 clangd 应该能正确索引所有C++模块了
pause