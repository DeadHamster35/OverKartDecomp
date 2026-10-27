# OverKartDecomp build warnings summary

**Build:** 2026-10-25 11:58 ET (MobileFOB, `sh _build_run.sh`)
**Result:** SUCCESS (MAKE_EXIT:0), 2 file(s) compiled, ROM linked
**ROM:** `build/us/OverKart_V8.z64` SHA1 `8ab7ddf7a85c1e5eb95bab47022121918fbd251b` (previous `10365730c5bb2fb011b8843d3e86648666608d92`)

## Files compiled
- `src/main.c`
- `src/OverKartLibrary/SharedFunctions.c`

## Totals
- GCC warnings: 1
- Compile errors: 0
- Undefined references: 0
- Multiple definitions: 0
- Linker warnings: 0

## GCC warnings
- `src/OverKartLibrary/SharedFunctions.c:357:52`: unused parameter 'v_buff' [-Wunused-parameter]

## IDO cfe notices (non-fatal)
- Warning 513 CRLF "Unknown control character \015" notices in `src/OverKartLibrary/OKRumble.h`
- cfe output was garbled/interleaved ("Too many errors... goodbye.") for `OKRumble.h`; non-fatal, build continued and linked.

_Only the files listed above recompiled this run; warnings from untouched files are not repeated here (see previous full-build summaries)._
