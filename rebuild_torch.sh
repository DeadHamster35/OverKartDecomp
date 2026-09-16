#!/bin/sh
set -e
export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/KimuraDecomp"
rm -rf tools/torch/cmake-build-release
make -C tools torch -j
echo TORCH_BUILD_EXIT:$?
test -f tools/torch/cmake-build-release/torch.exe
./tools/torch/cmake-build-release/torch.exe code baserom.us.z64
./tools/torch/cmake-build-release/torch.exe header baserom.us.z64
./tools/torch/cmake-build-release/torch.exe modding export baserom.us.z64
echo ASSETS_EXIT:$?
