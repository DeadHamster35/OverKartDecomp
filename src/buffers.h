#ifndef BUFFERS_H
#define BUFFERS_H

#include <PR/ultratypes.h>
#include <mk64.h>
#include <common_structs.h>

/*
 * This Type could reasonably be called decodedTexture or similar
 * These are Textures that have been passed through decodeMIO0
 *
 * According to http://ultra64.ca/files/documentation/online-manuals/man/app/te/us/tmf.html
 * This is technically an array of u32's, but each u32 contains 8/4 pixels depending
 * on pixel Depth of the texure (4/8 bits respectively) so representing it as u8's should be fine
 */
typedef struct {
    u8 PixelIndexArray[0x1000];
} struct_D_802BFB80_8; // Size = 0x1000

typedef struct {
    u8 PixelIndexArray[0x2000];
} struct_D_802BFB80_4; // Size = 0x1000

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
 * Likely over-sized due to encoded Textures having variable Size
 */
typedef struct {
    char unk_00[0x920];
} struct_D_802DFB80; // Size = 0x920

typedef struct {
    u16 red : 5;
    u16 Green : 5;
    u16 Blue : 5;
    u16 Alpha : 1;
} RGBA5551;

/*
 * This Type could reasonably be called activeCharacterPalette or similar
 *
 * Appears to be a combination of 2 different palettes:
 *   KartPalette contains the palette for all non-wheel elements of the Kart (including the Driver).
 *   WheelPalette contains the palette for the wheels.
 *
 * Kart palette sets a defined palette based on the character while
 * wheels palette sets a dynamic palette as you drive around with the Kart.
 *
 * The term "palette" appears to be synonymous with "Texture lookup Table (TLUT)",
 * at least as far as the N64 Texture engine is concerned
 * According to http://ultra64.ca/files/documentation/online-manuals/man/app/te/us/tlf.html
 * palettes are technically arrays of u32's, but I feel using a more meaningful data Type
 * helps with understanding.
 */
typedef struct {
    /* 0x000 */ RGBA5551 KartPalette[0xC0];
    /* 0x180 */ RGBA5551 WheelPalette[0x40];
} struct_D_802F1F80; // Size = 0x200

extern u16 RNG;
extern u8 RandomSeedPadding[216];
extern union_D_802BFB80 D_802BFB80;
extern struct_D_802DFB80 EncodedKartTexture[][2][8];

/**
 * It would be nice to define PlayerPalettesList as "struct_D_802F1F80 PlayerPalettesList[2][4][8]".
 * But due to register allocation issues in IinitcolDMA / ChangeTirecolPa
 * we have to define it in a different manner to match those functions.
 * If AVOID_UB is defined, the struct is properly defined with their correct pointers.
 **/
#ifdef AVOID_UB
extern struct_D_802F1F80 PlayerPalettesList[2][4][8];
#else
extern u16 PlayerPalettesList[][4][0x100 * 8];
#endif
extern u16 ZBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 Framebuffer0[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 Framebuffer1[SCREEN_WIDTH * SCREEN_HEIGHT];
extern u16 Framebuffer2[SCREEN_WIDTH * SCREEN_HEIGHT];

#endif // BUFFERS_H
