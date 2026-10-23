# OverKartDecomp build warnings summary

**Build:** Fri Oct 23 02:29:15 Eastern Daylight Time 2026 to Fri Oct 23 02:30:08 Eastern Daylight Time 2026, on MobileFOB
**Command:** `make -j AVOID_UB=1 COMPARE=0 VERSION=us`
**Result:** SUCCESS (`MAKE_EXIT:0`). No sources recompiled; the ROM was re-linked only.
**ROM:** `build/us/OverKart_V8.z64` (written 02:30)
**SHA1:** `b16ac7b1c8f4555238855851fbd3d1e81b5a06ef` (unchanged from the 00:44 build)

## Errors
None. There are no compile errors, undefined references or multiple definitions.

The warnings below are carried over from the 00:44 build, the last run that compiled anything.

## Compiler warnings
16 warnings (only files recompiled this run). Top groups (count, file, warning):

- 7 x `src/OverKart5/OKMenu.c | comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]`
- 1 x `src/OverKart5/OverKartObjectHandler.c | unused parameter 'HitType' [-Wunused-parameter]`
- 1 x `src/OverKart5/OverKartObjectHandler.c | unused parameter 'Car' [-Wunused-parameter]`
- 1 x `src/OverKart5/OKMenu.c | comparison of integer expressions of different signedness: 'char' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]`
- 1 x `src/OverKart5/OverKart.c | comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]`
- 1 x `src/OverKart5/MarioKartPractice.c | implicit declaration of function 'InitMKCode'; did you mean 'InitMkcode'? [-Wimplicit-function-declaration]`
- 1 x `src/OverKart5/OKMenu.c | comparison of integer expressions of different signedness: 'short int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]`
- 1 x `src/OverKart5/OKMenu.c | unused parameter 'StatsMode' [-Wunused-parameter]`
- 1 x `src/OverKart5/OKMenu.c | unused parameter 'pad' [-Wunused-parameter]`
- 1 x `src/OverKart5/OKMenu.c | unused parameter 'newbutton' [-Wunused-parameter]`

## Warnings by file

- `src/OverKart5/OKMenu.c`: 12
- `src/OverKart5/OverKartObjectHandler.c`: 2
- `src/OverKart5/OverKart.c`: 1
- `src/OverKart5/MarioKartPractice.c`: 1

