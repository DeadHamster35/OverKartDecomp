#!/bin/sh
set -e
export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/KimuraDecomp"
echo START:$(date)
echo "=== make clean ==="
make clean
echo CLEAN_EXIT:$?
echo "=== make -j ==="
make -j
echo MAKE_EXIT:$?
echo DONE:$(date)
