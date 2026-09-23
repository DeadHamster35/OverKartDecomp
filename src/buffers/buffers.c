#include <ultra64.h>
#include <macros.h>
#include <mk64.h>
#include "buffers.h"

/**
 * @brief look like to be a Buffer of decoded Textures
 */
ALIGNED8 union_D_802BFB80 D_802BFB80;
ALIGNED8 struct_D_802DFB80 EncodedKartTexture[2][2][8];
#ifdef AVOID_UB
ALIGNED8 struct_D_802F1F80 PlayerPalettesList[2][4][8];
#else
ALIGNED8 u16 PlayerPalettesList[2][4][0x100 * 8];
#endif
