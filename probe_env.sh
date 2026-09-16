#!/bin/sh
echo PWD=$(pwd)
echo "=== mounts ==="
mount 2>/dev/null | head -20
ls /c 2>/dev/null | head -5
ls /C 2>/dev/null | head -5
echo "=== find git ==="
ls "/c/Program Files/SmartGit/git/cmd/git.exe" 2>&1
ls "/C/Program Files/SmartGit/git/cmd/git.exe" 2>&1
which git 2>&1
export PATH="/c/Program Files/SmartGit/git/cmd:$PATH"
which git 2>&1
git --version 2>&1
export PATH="/C/Program Files/SmartGit/git/cmd:$PATH"
which git 2>&1
git --version 2>&1
echo done
