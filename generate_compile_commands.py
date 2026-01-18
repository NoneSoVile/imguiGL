#!/usr/bin/env python3
"""
生成 compile_commands.json 的脚本
用于解决 clangd 索引问题
"""

import json
import os
import glob
from pathlib import Path

def generate_compile_commands():
    project_root = Path(__file__).parent.absolute()
    
    # 基本编译参数
    base_flags = [
        "cl.exe",
        "/std:c++20",
        "/DUNICODE",
        "/D_UNICODE", 
        "/D_CRT_SECURE_NO_WARNINGS",
        f"/I{project_root}/include",
        f"/I{project_root}/include/imgui",
        f"/I{project_root}/include/Graphics",
        f"/I{project_root}/include/Utilities", 
        f"/I{project_root}/include/LinearMath",
        f"/I{project_root}/include/kinematics",
        f"/I{project_root}/include/3dmodel_loader",
        f"/I{project_root}/include/ParticleSystem",
        f"/I{project_root}/extern/ImGuiFileDialog",
        "/Id:/vcpkg/vcpkg/installed/x64-windows/include"
    ]
    
    # 查找所有源文件
    source_files = []
    
    # 添加 main.cpp
    if os.path.exists(project_root / "main.cpp"):
        source_files.append("main.cpp")
    
    # 查找 src 目录下的所有 cpp 文件
    src_pattern = str(project_root / "src" / "**" / "*.cpp")
    for file_path in glob.glob(src_pattern, recursive=True):
        rel_path = os.path.relpath(file_path, project_root).replace("\\", "/")
        source_files.append(rel_path)
    
    # 查找 extern 目录下的 cpp 文件
    extern_pattern = str(project_root / "extern" / "**" / "*.cpp") 
    for file_path in glob.glob(extern_pattern, recursive=True):
        rel_path = os.path.relpath(file_path, project_root).replace("\\", "/")
        source_files.append(rel_path)
    
    # 生成 compile_commands.json 条目
    compile_commands = []
    
    for source_file in source_files:
        command = " ".join(base_flags + [source_file])
        entry = {
            "directory": str(project_root).replace("\\", "/"),
            "file": source_file,
            "command": command
        }
        compile_commands.append(entry)
    
    # 写入文件
    output_file = project_root / "compile_commands.json"
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(compile_commands, f, indent=2, ensure_ascii=False)
    
    print(f"已生成 compile_commands.json，包含 {len(compile_commands)} 个源文件:")
    for file in source_files:
        print(f"  - {file}")

if __name__ == "__main__":
    generate_compile_commands()