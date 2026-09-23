#!/bin/sh
set +e
export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/OverKartDecomp"
echo START:$(date)
make -j AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 VERSION=us
echo MAKE_EXIT:$?
echo END:$(date)