#!/bin/sh
set -e
export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/KimuraDecomp"
rm -rf tools/torch/cmake-build-release
make -C tools -j
echo TOOLS_EXIT:$?
