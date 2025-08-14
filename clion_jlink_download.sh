#!/bin/bash

WORKSPACE_FILE=".idea/workspace.xml"

echo "Checking workspace file..."
if [ ! -f "$WORKSPACE_FILE" ]; then
    echo "Error: workspace.xml not found in .idea directory."
    exit 1
fi

# 从 workspace.xml 提取 PROGRAM_PARAMS 属性
PROGRAM_PARAMS=$(grep -oP 'PROGRAM_PARAMS="[^"]+"' "$WORKSPACE_FILE" | head -1)
DEVICE_NAME=$(echo "$PROGRAM_PARAMS" | grep -oP '(?<=-device\s)\S+')
SPEED=$(echo "$PROGRAM_PARAMS" | grep -oP '(?<=-speed\s)\d+')
if [ -z "$SPEED" ]; then SPEED="4000"; echo "Speed not found, defaulting to $SPEED."; fi

if [ -z "$DEVICE_NAME" ]; then
    echo "Error: Device name not found in PROGRAM_PARAMS."
    exit 1
fi

# 获取 build 目录
GENERATION_DIR="build"
echo "GENERATION_DIR: $GENERATION_DIR"

# 查找最新的 ELF 文件
LATEST_ELF=$(find "$GENERATION_DIR" -maxdepth 1 -type f -name "*.elf" -printf "%T@ %p\n" | sort -n | tail -1 | awk '{print $2}')

if [ -z "$LATEST_ELF" ]; then
    echo "Error: No ELF file found in $GENERATION_DIR"
    exit 1
fi

echo "Using ELF file: $LATEST_ELF"
echo "Device: $DEVICE_NAME, Speed: $SPEED"

# 烧写操作
if [ "$1" == "ver" ]; then
    echo "Performing verification burn..."
    JLinkExe << EOF
device $DEVICE_NAME
if swd
speed $SPEED
connect
loadfile $LATEST_ELF
verify
r
q
EOF
else
    echo "Performing normal burn..."
    JLinkExe << EOF
device $DEVICE_NAME
if swd
speed $SPEED
connect
loadfile $LATEST_ELF
r
q
EOF
fi
