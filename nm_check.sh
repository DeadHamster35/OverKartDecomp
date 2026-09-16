#!/bin/sh
export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/LibraryDecomp"
ELF=build/us/mk64.us.elf
[ -f "$ELF" ] || ELF=build/us/mk64.elf
echo ELF=$ELF
which mips64-elf-nm mips-linux-gnu-nm nm 2>/dev/null
NM=$(command -v mips64-elf-nm || true)
if [ -n "$NM" ] && [ -f "$ELF" ]; then
  echo "--- nm kart toad/wario/peach ---"
  "$NM" -n "$ELF" | grep -E 'gKart(Toad|Wario|Peach)' | head -80
  echo "--- nm around 80026AB4 / 80026464 ---"
  "$NM" -n "$ELF" | awk 'BEGIN{t1=strtonum("0x80026AB4"); t2=strtonum("0x80026464")}
    /^[0-9a-fA-F]+/ {
      a=strtonum("0x"$1); name=$3;
      if(a<=t1){c1=a; n1=name} 
      if(a<=t2){c2=a; n2=name}
    }
    END{printf("PC1 containing %s @ %x\n", n1, c1); printf("PC2 containing %s @ %x\n", n2, c2)}'
  echo "--- objdump -d around 80026AB4 ---"
  OD=$(command -v mips64-elf-objdump || true)
  if [ -n "$OD" ]; then
    "$OD" -d "$ELF" --start-address=0x80026A90 --stop-address=0x80026AE0
    echo "--- objdump around ra 80026464 ---"
    "$OD" -d "$ELF" --start-address=0x80026440 --stop-address=0x80026490
  fi
else
  echo "nm_unavailable"
fi
