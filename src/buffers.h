#ifndef BUFFERS_H
#define BUFFERS_H

#include <PR/ultratypes.h>
#include <mk64.h>
#include <common_structs.h>

/*
 * This type could reasonably be called decodedTexture or similar
 * These are textures that have been passed through decodeMIO0
 *
 * According to http://ultra64.ca/files/documentation/online-manuals/man/app/te/us/tmf.html
 * This is technically an array of u32's, but each u32 contains 8/4 pixels depending
 * on pixel depth of the texure (4/8 bits respectively) so representing it as u8's should be fine
 */
typedef struct {
    u8 pixel_index_array[0x1000];
} struct_D_802BFB80_8; // size = 0x1000

typedef struct {
    u8 pixel_index_array[0x2000];
} struct_D_802BFB80_4; // size = 0x1000

/*
 * In render_player, spawn_players, and replays D_802BFB80 is the arraySize8 entry
 * But in menu_item its the arraySize4 entry
 * The only way to unify those 2 things is to use a union
 */
typedef union {
    struct_D_802BFB80_4 arraySize4[2][2][4];
    struct_D_802BFB80_8 arraySize8[2][2][8];
} union_D_802BFB80;

/*
 * Texture prior to be being decoded via decodeMIO0
 * Likely over-sized due to encoded textures having variable size
 */
typedef struct {
    char unk_00[0x920];
} struct_D_802DFB80; // size = 0x920

typedef struct {
    u16 red : 5;
    u16 green : 5;
    u16 blue : 5;
    u16 alpha : 1;
} RGBA5551;

/*
 * This type could reasonably be called activeCharacterPalette or similar
 *
 * Appears to be a combination of 2 different palettes:
 *   kart_palette contains the palette for all non-wheel elements of the kart (including the driver).
 *   wheel_palette contains the palette for the wheels.
 *
 * kart palette sets a defined palette based on the character while
 * wheels palette sets a dynamic palette as you drive around with the kart.
 *
 * The term "palette" appears to be synonymous with "texture lookup table (TLUT)",
 * at least as far as the N64 texture engine is concerned
 * According to http://ultra64.ca/files/documentation/online-manuals/man/app/te/us/tlf.html
 * palettes are technically arrays of u32's, but I feel using a more meaningful data type
 * helps with understanding.
 */
typedef struct {
    /* 0x000 */ RGBA5551 kart_palette[0xC0];
    /* 0x180 */ RGBA5551 wheel_palette[0x40];
} struct_D_802F1F80; // size = 0x200

extern u16 g_RNG;
extern u8 randomSeedPadding[216];
extern union_D_802BFB80 D_802BFB80;
extern struct_D_802DFB80 gEncodedKartTexture[][2][8];

/**
 * It would be nice to define gPlayerPalettesList as "struct_D_802F1F80 gPlayerPalettesList[2][4][8]".
 * But due to register allocation issues in Iinitcol_dma / change_tirecol_Pa
 * we have to define it in a different manner to match those functions.
 * If AVOID_UB is defined, the struct is properly defined with their correct pointers.
 **/
#ifdef AVOID_UB
extern struct_D_802F1F80 gPlayerPalettesList[2][4][8];
#else
extern u16 gPlayerPalettesList[][4][0x100 * 8];
#endif
extern u16 gZBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 gFramebuffer0[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 gFramebuffer1[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 gFramebuffer2[SCREEN_WIDTH * SCREEN_HEIGHT];

#endif // BUFFERS_H
