#ifndef PLAYER_PADDING_H
#define PLAYER_PADDING_H

#include <common_structs.h>

/*
 * Decomp Player Padding bytes (sizeof still 0xDD8). Do not rename
 * HootFlag / HampFlag / LitroFlag in include/common_structs.h.
 *
 * Offset | Decomp member        | Amped Up Library name (host only)
 * 0x0A   | char HootFlag[2]    | Ushort LitroFlag
 * 0x12   | s16  HampFlag       | short effect_timer
 * 0x32   | char LitroFlag[2]   | short shellpathinghack
 *
 * Amped Up car->LitroFlag is 0x0A, not decomp LitroFlag at 0x32.
 * These macros are lvalues.
 */

#define PlayerHootFlagU16(p) (*(u16 *) (p)->HootFlag)
#define PlayerHampFlag(p) ((p)->HampFlag)
#define PlayerLitroFlagS16(p) (*(s16 *) (p)->LitroFlag)

#endif
