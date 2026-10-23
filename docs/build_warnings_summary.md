# OverKartDecomp build warnings summary

**Build:** Fri Oct 23 00:44:53 Eastern Daylight Time 2026 to Fri Oct 23 00:45:40 Eastern Daylight Time 2026, on MobileFOB
**Command:** `make -j AVOID_UB=1 COMPARE=0 VERSION=us`
**Result:** SUCCESS (`MAKE_EXIT:0`)
**ROM:** `build/us/OverKart_V8.z64`
**SHA1:** `b16ac7b1c8f4555238855851fbd3d1e81b5a06ef` (previous `990c6cc6d6f7f532f06d27b165ce4ed0503da353`)

## Errors
None. There are 0 compile errors, 0 undefined references and 0 multiple definitions. The 5 multiple definitions from `OverKartDefaults.c` in the 00:09 build are resolved.

## Linker warnings
- none

## IDO compiler noise
The IDO `cfe` step printed "Too many errors... goodbye." 100 times. It did not stop the build.

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
