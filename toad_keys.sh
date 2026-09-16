#!/bin/sh
cd "C:/Users/AuditorFOB/Desktop/Code/LibraryDecomp"
ELF=build/us/mk64.us.elf
echo "==== Toad wheel keyframes used by gKartToadWheels0 ===="
mips64-elf-nm -n "$ELF" | grep -E 'gKartToad(168|147|126|105|084|063|042|021|000|269|249|229|209|189)Wheel0$'
echo "==== Peach same ===="
mips64-elf-nm -n "$ELF" | grep -E 'gKartPeach(168|147|126|105|084|063|042|021|000|269|249|229|209|189)Wheel0$'
echo "==== map presence ===="
ls -la build/us/*.map 2>/dev/null || echo "no map files"
ls -la build/us/mk64.us.elf
