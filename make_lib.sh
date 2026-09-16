#!/bin/sh
set -e
export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/LibraryDecomp"
echo START:$(date)
make -j
echo MAKE_EXIT:$?
echo DONE:$(date)
