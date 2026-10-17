#ifndef OKAssemblyH
#define OKAssemblyH
#include "../../MainInclude.h"
// the following all deal with ASM instructions in the game.
// we edit them in the customASM and StockASM routines.


/* nopASM: unused instruction poke */
/* asm_BlinkCheck / asm_DispOBSubPSelCursor1 / jtbl_DispObjPsel / asm_DOBPSel* /
 * asm_PlayerSelect*: player-select JALs and jumptable. Native is OKPSelSkipCursor /
 * OKPSelSkipPortrait / OKPSelSkipAnime in menu_items.c */
/* asm_tempo* / asmTempoFullScreen: RaceSequence Framerate immediates; ApplyRaceTempo */
/* asm_selectA-F / previewReload: course-select immediates / JAL; unused */
/* asm_BorderDraw: splitscreen border SMC; DrawStereoscopic3D no longer pokes */
/* asm_GPLevelSelect: GP course-index SH SMC; StockASM / OverkartASM no longer poke */

/* BSS in LibraryVariables.c. Do not Map onto KartEngineEcho immediates. */
extern short EchoStart;
extern short EchoStop;

/* highpolypipe* / itemboxes* / trees* / piranha* / sectionview* / surfacemap*:
 * stock LUI/ORI of Mario Raceway DLs and object lists. Native HotSwap uses
 * DisplayKT1Hook, InitialMapObjectCode, SearchListFileHook. */

/* Vanilla RAM 0x800DD9D0 / 0x800DDB10. COMPARE=0 those slots are gKartWheels0[3]. */
#define g_pathLength (PathLengthTable[0][0])
#define g_pathBLength (PathLengthTable[20][0])
/* Vanilla PathTable / PathTableB / PathTable[BLOCK_FORT]. */
#define pathOffset (PathTable[0][0])
#define pathOffsetB (PathTableB[0][0])
#define pathOffsetBlock (PathTable[15][0])
#endif
