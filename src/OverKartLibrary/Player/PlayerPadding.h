#ifndef PlayerPaddingH
#define PlayerPaddingH
#include <common_structs.h>

/*
 * Decomp Player padding bytes (sizeof still 0xDD8). Do not rename
 * hoot_flag / hamp_flag / litro_flag in include/common_structs.h.
 *
 * Offset | Decomp member        | Amped Up Library name (host only)
 * 0x0A   | char hoot_flag[2]    | ushort litro_flag
 * 0x12   | s16  hamp_flag       | short effect_timer
 * 0x32   | char litro_flag[2]   | short shellpathinghack
 *
 * Amped Up car->litro_flag is 0x0A, not decomp litro_flag at 0x32.
 * These macros are lvalues.
 */

#define PlayerHootFlagU16(p) (*(u16 *) (p)->hoot_flag)
#define PlayerHampFlag(p) ((p)->hamp_flag)
#define PlayerLitroFlagS16(p) (*(s16 *) (p)->litro_flag)

#endif
