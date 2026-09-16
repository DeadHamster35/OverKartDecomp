#!/bin/sh
set -e
cd /c/Users/AuditorFOB/Desktop/Code/KimuraDecomp
export PATH="/c/Users/AuditorFOB/Desktop/Code/KimuraDecomp/tools/mingw64/bin:$PATH"
echo START:$(date)
make -j
echo MAKE_EXIT:$?
echo DONE:$(date)