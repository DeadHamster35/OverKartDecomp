#!/bin/sh
set -e
export PATH="C:/Program Files/Git/cmd:C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/OverKartDecomp"
echo START:$(date)
make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j
echo MAKE_EXIT:$?
echo DONE:$(date)
