#!/bin/sh
cd "C:/Users/AuditorFOB/Desktop/Code/LibraryDecomp"
mips64-elf-nm -n build/us/mk64.us.elf | grep -E 'gKart(Toad|Peach|Wario)00[0-3]Wheel0$'
echo "---"
mips64-elf-nm -n build/us/mk64.us.elf | grep -E 'gKartToad(000|001|002)Wheel[0-3]$'
