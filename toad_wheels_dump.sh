#!/bin/sh
cd "C:/Users/AuditorFOB/Desktop/Code/LibraryDecomp"
ELF=build/us/mk64.us.elf
NM=mips64-elf-nm
OD=mips64-elf-objdump
echo "==== wheel table symbols ===="
$NM -n "$ELF" | grep -E 'gKart.*Wheels0|gKartWheels0'
echo "==== dump gKartToadWheels0 region (objdump -s) ===="
# read 64 bytes at each character wheels0 if present
python3 - <<'PY'
import subprocess,struct
elf='build/us/mk64.us.elf'
# use objdump -t / readelf; easier: od via objcopy? use mips64-elf-objdump -s -j .main with start
# Get addresses
out=subprocess.check_output(['mips64-elf-nm','-n',elf],text=True,errors='replace')
addrs={}
for line in out.splitlines():
    parts=line.split()
    if len(parts)>=3 and parts[2] in (
        'gKartToadWheels0','gKartWarioWheels0','gKartPeachWheels0','gKartMarioWheels0',
        'gKartLuigiWheels0','gKartYoshiWheels0','gKartDKWheels0','gKartBowserWheels0',
        'gKartDonkeyKongWheels0','gKartWheels0','gKartToad000Wheel0','gKartWario000Wheel0','gKartPeach000Wheel0'):
        addrs[parts[2]]=int(parts[0],16)&0xffffffff
print('addrs:')
for k,v in sorted(addrs.items(), key=lambda x:x[1]):
    print(f'  {k}=0x{v:08x}')

# Extract .main bytes via objdump -s is heavy; use readelf -x if available
# Prefer: mips64-elf-objcopy -O binary --only-section=.main then seek ? too heavy.
# Use python with pyelftools? may not exist.
# Fallback: objdump -s --start-address --stop-address
for name in ['gKartToadWheels0','gKartWarioWheels0','gKartPeachWheels0','gKartMarioWheels0','gKartWheels0']:
    if name not in addrs: 
        print('missing', name); continue
    a=addrs[name]
    cmd=['mips64-elf-objdump','-s','--start-address=0x%X'%a,'--stop-address=0x%X'%(a+0x40),elf]
    print('\nDUMP', name, hex(a))
    print(subprocess.check_output(cmd,text=True,errors='replace'))
PY
