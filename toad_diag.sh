#!/bin/sh
set -e
export PATH="C:/Program Files/SmartGit/git/cmd:$PATH"
cd "C:/Users/AuditorFOB/Desktop/Code/LibraryDecomp"
ELF=build/us/mk64.us.elf
NM=mips64-elf-nm
OD=mips64-elf-objdump

echo "==== PC SYMBOLIZE ===="
# nearest symbol <= address using python for reliability
python3 - <<'PY'
import subprocess
elf="build/us/mk64.us.elf"
out=subprocess.check_output(["mips64-elf-nm","-n",elf], text=True, errors="replace")
syms=[]
for line in out.splitlines():
    parts=line.split()
    if len(parts)>=3 and all(c in "0123456789abcdefABCDEF" for c in parts[0]):
        addr=int(parts[0],16)
        typ=parts[1]
        name=parts[2]
        syms.append((addr,typ,name))
for target in (0x80026AB4, 0x80026464):
    cand=None
    for a,t,n in syms:
        if a<=target: cand=(a,t,n)
        else: break
    print(f"PC 0x{target:08X} -> {cand[2]} @ 0x{cand[0]:08X} typ={cand[1]} delta=+0x{target-cand[0]:X}")
    # next symbols
    idx=next(i for i,s in enumerate(syms) if s[0]==cand[0] and s[2]==cand[2])
    for j in range(idx, min(idx+4,len(syms))):
        a,t,n=syms[j]
        print(f"  {a:08x} {t} {n}")

print("\n==== KART KEY SYMBOLS ====")
want=["gKartToadPalette","gKartToad000","gKartToad000Wheel0","gKartToad000Wheel1","gKartToad000Wheel2","gKartToad000Wheel3",
"gKartWarioPalette","gKartWario000","gKartWario000Wheel0","gKartWario000Wheel1",
"gKartPeachPalette","gKartPeach000","gKartPeach000Wheel0","gKartPeach000Wheel1",
"gKartToadWheels0","gKartWheels0"]
found={n:(a,t) for a,t,n in syms}
for w in want:
    if w in found:
        a,t=found[w]
        print(f"{a:08x} {t} {w}")
    else:
        print(f"MISSING {w}")

# bank check: first/last toad vs neighbors
print("\n==== CHARACTER BANK RANGES (first frame + palette) ====")
for ch in ["Luigi","Mario","Yoshi","Peach","Wario","Toad","DK","Bowser","Donkey"]:
    keys=[n for a,t,n in syms if n.startswith(f"gKart{ch}")]
    if not keys:
        # DK might be DonkeyKong style
        continue
    addrs=[found[n][0] for n in keys if n in found]
    print(f"{ch}: count={len(keys)} min=0x{min(addrs):08x} max=0x{max(addrs):08x}")

# specifically list Toad palette and wheel0 and 000
print("\n==== ALL gKartToad* (first 30 + palette/wheels) ====")
toad=[(a,t,n) for a,t,n in syms if n.startswith("gKartToad")]
for a,t,n in toad[:25]:
    print(f"{a:08x} {t} {n}")
print("...")
for a,t,n in toad:
    if "Palette" in n or "Wheel" in n or n.endswith("000"):
        if n.endswith("000") or "Palette" in n or "000Wheel" in n:
            print(f"{a:08x} {t} {n}")
# unique wheel0 and palette
for a,t,n in toad:
    if n in ("gKartToadPalette","gKartToad000","gKartToad000Wheel0","gKartToad000Wheel1","gKartToad000Wheel2","gKartToad000Wheel3","gKartToadWheels0"):
        print(f"KEY {a:08x} {t} {n}")
print("toad_symbol_count", len(toad))

print("\n==== objdump ra site ====")
PY
$OD -d "$ELF" --start-address=0x80026440 --stop-address=0x80026490
echo "==== change_tirecol_Pa bounds ===="
$NM -n "$ELF" | grep -E 'change_tirecol_Pa|tirecol'
