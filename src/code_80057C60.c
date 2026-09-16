/**
 * @file code_80057C60.c
 * @warning there are too many variables here
 */

#include <ultra64.h>
#include <macros.h>
#include <PR/gbi.h>
#include <mk64.h>
#include <course.h>

#include "camera.h"
#include "code_80057C60.h"
#include "main.h"
#include "actors.h"
#include "code_800029B0.h"
#include "racing/memory.h"
#include <defines.h>
#include "math_util.h"
#include "math_util_2.h"
#include "cpu_vehicles_camera_path.h"
#include "render_player.h"
#include "render_objects.h"
#include "code_8006E9C0.h"
#include "update_objects.h"
#include "code_80086E70.h"
#include "effects.h"
#include <assets/data_800E8700.h>
#include "skybox_and_splitscreen.h"
#include <assets/common_data.h>
#include "audio/external.h"
#include "render_player.h"
#include "bomb_kart.h"
#include "menus.h"
#include "data/other_textures.h"
#include "spawn_players.h"
#include "sounds.h"
#include "data/some_data.h"
#include "OverKartHooks.h"

//! @warning this macro is undef'd at the end of this file
#define MAKE_RGB(r, g, b) (((r) << 0x10) | ((g) << 0x08) | (b << 0x00))

s32 KW16GFTimer;
s32 KW8GFTimer;
s32 KW4GFTimer;
s32 KW2GFTimer;
UNUSED s32 D_801655A0;
s32 KW64GFCount;
UNUSED s32 D_801655A8;
s32 KW32GFCount;
UNUSED s32 D_801655B0;
s32 KW16GFCount;
UNUSED s32 D_801655B8;
s32 KW8GFCount;
s32 D_801655C0;
s32 KW4GFCount;
s32 D_801655C8;
s32 KW2GFCount;
UNUSED s32 D_801655D0[2];
s32 D_801655D8;
UNUSED s32 D_801655DC[2];
s32 D_801655E8;
UNUSED s32 D_801655EC;
s32 D_801655F0;
UNUSED s32 D_801655F4;
s32 D_801655F8;
UNUSED s32 D_80165600[2];
s32 KWFlash8;
UNUSED s32 D_80165610[2];
s32 D_80165618;
UNUSED s32 D_80165620[2];
s32 D_80165628;
UNUSED s32 D_80165630[2];
u32 D_80165638;
UNUSED s32 D_80165640[2];
u32 D_80165648;
UNUSED u32 D_80165650[2];
u32 D_80165658[8];
s32 D_80165678;
UNUSED s32 D_80165680[12];
u16 D_801656B0;
UNUSED s32 D_801656B8[2];
u16 kwflash_r;
UNUSED s32 D_801656C8[2];
u16 kwflash_g;
UNUSED s32 D_801656D8[2];
u16 kwflash_b;
UNUSED s32 D_801656E8[2];
s16 D_801656F0;
UNUSED s32 D_801656F8[4];
s16 D_80165708;
UNUSED s32 D_8016570C;
s16 D_80165710;
UNUSED s32 D_80165714;
s16 D_80165718;
UNUSED s32 D_8016571C;
s16 D_80165720;
UNUSED s32 D_80165724;
s16 D_80165728;
UNUSED s32 D_8016572C;
s16 D_80165730;
UNUSED s32 D_80165734;
//! Tracking a count of some object type, don't know what object type yet
s16 D_80165738;
UNUSED s32 D_8016573C;
s16 D_80165740;
UNUSED s32 D_80165744;
s16 D_80165748;
UNUSED s32 D_8016574C;

s16 gNumActiveThwomps;
s32 D_80165754;
ThwompSpawn* gThowmpSpawnList;

Vec4s D_80165760;
UNUSED s16 D_80165768;
s8 D_8016576A;
Vec4s D_80165770;
UNUSED s32 D_80165778;
Vec4s D_80165780;
UNUSED s32 D_80165788;
s16 D_8016578C;
UNUSED s16 D_8016578E;
s16 D_80165790;
UNUSED s16 D_80165792;
s16 D_80165794;
UNUSED s32 D_80165798;
s8 D_8016579C;
u16 D_8016579E;
UNUSED s16 D_801657A0;
//! Something related to the rotation(?) of ice in Sherbet Land
u16 D_801657A2;
UNUSED s32 D_801657A4;
UNUSED s16 D_801657A8[3];
s8 D_801657AE;
UNUSED s8 D_801657AF;
//! HUD related
s8 g_KWDBDispSW;
UNUSED s8 D_801657B1;
s8 D_801657B2;
UNUSED s8 D_801657B3;
s8 D_801657B4;
s8 D_801657B8[16];
s8 g_cloudsToggle;
s8 D_801657D0[8];
s8 g_KWDemoSW;
UNUSED s16 D_801657DA[2];
UNUSED s8 D_801657E0;
s8 D_801657E1;
s8 kwgoalpanelsw;
s8 D_801657E3;
s8 g_hudLapToggle;
s8 D_801657E5;
bool8 g_hudSpeedToggle2;
u8 D_801657E7;
bool8 g_hudMapToggle2;
UNUSED s32 D_801657EC;
bool8 g_blueLineRankToggle;
UNUSED s32 D_801657F4;
bool8 g_KWLapSW;
s32 D_801657FC;
s8 D_80165800[2];
s32 D_80165804;
s8 g_hudToggleFlag;
s32 D_8016580C;
bool8 g_hudSpeedToggle;
s32 D_80165814;
bool8 D_80165818;
s32 D_8016581C;
s8 D_80165820;
UNUSED s32 D_80165824;
s8 D_80165828;
Vec3su D_8016582C;
s8 g_hudToggleFlagP2[2];
Vec3su D_80165834;
UNUSED s32 D_8016583A;
s8 D_80165840[3];
UNUSED s32 D_80165848[6];
s32 D_80165860;
UNUSED s32 D_80165864;
UNUSED s32 D_80165868;
s32 D_8016586C;
UNUSED s32 D_80165870[2];
s32 D_80165878;
s32 D_8016587C;
u8* D_80165880;
UNUSED s32 D_80165884;
s8 D_80165888;
UNUSED s32 D_8016588C;
s8 D_80165890;
UNUSED s32 D_80165894;
s8 balloonsw;
s32 D_8016589C;
UNUSED s32 D_801658A0[2];
s8 D_801658A8;
UNUSED s32 D_801658B0[3];
s8 D_801658BC;
UNUSED s32 D_801658C0;
UNUSED s16 D_801658C4;
s8 D_801658C6;
UNUSED s32 D_801658C8;
UNUSED s16 D_801658CC;
s8 D_801658CE;
UNUSED s32 D_801658D0;
UNUSED s16 D_801658D4;
s8 D_801658D6;
UNUSED s32 D_801658D8;
s8 D_801658DC;
UNUSED s32 D_801658E0;
s8 D_801658E4;
UNUSED s32 D_801658E8;
s8 D_801658EC;
UNUSED s32 D_801658F0;
s8 D_801658F4;
UNUSED s32 D_801658F8;
UNUSED s8 D_801658FC;
u8 sRandomItemIndex;
s8 D_801658FE;
u8 gControllerRandom;
s16 D_80165900;
UNUSED s32 D_80165904;
s8 D_80165908;
UNUSED s32 D_80165910[96];
s8 D_80165A90;
UNUSED s32 D_80165AA0[95];
UNUSED s32 D_80165C14;
AnmObject g_DynamicObjects[OBJECT_LIST_SIZE];
UNUSED s32 D_80183D58;
s32 objectListSize;
Mtx D_80183D60;
/**
 * Use unknown. An object is reserved and its index is saved to
 * this variable, but it appears to go unreferenced
 **/
s32 D_80183DA0;
f32 D_80183DA8[4];
//! Lakitu?
s32 jugemuallocptr[4];
f32 D_80183DC8[4];
//! Indexes for the objects associated with the Bomb Karts
s32 bomballocptr[NUM_BOMB_KARTS_MAX];
UNUSED s32 D_80183DF8[16];
//! Next free spot in EffectAllocArray1? Wraps back around to 0 if it gets bigger than gObjectParticle1_SIZE
s32 gNextFreeObjectParticle1;
Vec3f D_80183E40;
//! Next free spot in EffectAllocArray2? Wraps back around to 0 if it gets bigger than gObjectParticle2_SIZE
s32 gNextFreeObjectParticle2;
Vec3f D_80183E50;
//! Next free spot in EffectAllocArray3?
s32 gNextFreeObjectParticle3;
UNUSED s32 D_80183E60[3];
//! Next free spot in FireParticleAllocArray? Wraps back around to 0 if it gets bigger than gObjectParticle4_SIZE
s32 FireParticleCounter;
Vec3f D_80183E70;
//! Next free spot in leafallocptr? Wraps back around to 0 if it gets bigger than gLeafParticle_SIZE
s32 gNextFreeLeafParticle;
Vec3su D_80183E80;
//! Appears to be a list of object list indices for the Item Window part of the HUD
s32 ItemBoxAllocPtr[4];
Vec3su D_80183E98;
/**
 * Snowmen bodies in FrappeSnowland
 * Crabs in Koopa Troopa Beach
 * Hot air balloon in Luigi Raceway?
 * Neon signs in Rainbow Road?
 * Thwomps in Bower's Castle?
 * Penguins in Sherbet Land?
 * Flag Poles in Yoshi Valley?
 */
s32 objallocptr[32];
UNUSED s32 D_80183F20[2];
/**
 * Snowmen heads in Frappe Snowland
 * Chain Chomps in RaindbowRoad?
 * Trophy in award ceremony?
 * Seagulls in Koopa Troopa Beach?
 * Hedgehogs in Yoshi Valley?
 * Spawn for big fire breath in Bowser's Castle
 */
s32 objallocptr2[32];
/**
 * Seemingly a list of textures for Lakitu
 * Never explicitly given data, data appears to be placed here
 * via some type of DMA.
 * I'm also not certain about its dimensions
 * I think the entires in this array are way over-sized
 */
u8 D_80183FA8[4][0x2000];
/**
 * Boos in Banshee Boardwalk
 * Spawners for the 4 small fire breaths inside Bowser's Castle
 */
s32 objallocptr3[32];
//! Seemingly a pointer to Lakitu texture(s)
u8* gLakituTexturePtr;
/**
 * Unused list of object indices
 */
s32 objallocptr4[32];
//! Array of (4) Collisions?
Collision D_8018C0B0[4];
/**
 * List of object list indices used for:
 *   Moles in Moo Moo Farm
 *   Snow flakes in Frappe Snowland
 *   Segments of the fire breath from the statues in Bowser's Castle
 *   Potentially other things
 */
s32 EffectAllocArray1[gObjectParticle2_SIZE];
Collision D_8018C3B0;
/**
 * List of object list indices used for:
 * - Bats in Banshee's Boardwalk (but only 1 player mode?)
 * - Train index 0 smoke in Kalimari Desert
 * - Ferry index 0 smoke in DK Jungle
 */
s32 EffectAllocArray2[gObjectParticle2_SIZE];
// Maybe some unused Collision?
UNUSED Collision D_8018C5F0;
/**
 * List of object list indices used for:
 * - Train index 1 smoke in Kalimari Desert
 * - Ferry index 1 smoke in DK Jungle
 */
s32 EffectAllocArray3[gObjectParticle3_SIZE];
Collision D_8018C830;
/**
 * List of object list indices. Used both for the fires in the DK Jungle cave
 * and, seemingly for the trail that shells leave behind them.
 * I think they're using the same texture, which would explain the dual use
 */
s32 FireParticleAllocArray[gObjectParticle4_SIZE];
/**
 * Seemingly a list of object list indices used for the leaves that sometimes fall
 * trees when you bonk into them
 */
s32 leafallocptr[gLeafParticle_SIZE];
Hud playerHUD[4];
/**
 * List of object list indices used by the clouds and stars in some stages
 * Also used for snowflakes like EffectAllocArray1? Not sure what's up with that
 */
s32 g_CloudAllocate[D_8018CC80_SIZE];
struct_D_8018CE10 D_8018CE10[8];
//! Unknown object index, only set for Kalimari Desert, never read
s32 D_8018CF10;
Camera* D_8018CF14;
s16 D_8018CF18;
Player* D_8018CF1C;
s16 D_8018CF20;
UNUSED s32 D_8018CF24;
Player* D_8018CF28[8];
s16 D_8018CF48;
s16 D_8018CF50[8];
s16 D_8018CF60;
//! This may be a list of tilemap flags on a per-camera basis
s16 D_8018CF68[8];
s16 D_8018CF78;
/**
 * List of half-word character IDs indicating each character's
 * place in the current Grand Prix race's standings
 */
s16 gGPCurrentRaceCharacterIdByRank[8];
s16 D_8018CF90;
s16 KWRank[8];
s16 D_8018CFA8;
u8 D_8018CFAC[4];
s16 D_8018CFB0;
u8 D_8018CFB4[4];
s16 D_8018CFB8;
u8 D_8018CFBC[4];
s16 D_8018CFC0;
u8 D_8018CFC4[4];
s16 D_8018CFC8;
f32 D_8018CFCC;
s16 D_8018CFD0;
f32 D_8018CFD4;
s16 D_8018CFD8;

s16 D_800E4730[] = { 0x00ff, 0x0000, 0x0000, 0x00ff, 0x00ff, 0x0000, 0x0000, 0x00ff, 0x0000, 0x0032, 0x00ff, 0x00ff,
                     0x0000, 0x0000, 0x00ff, 0x00ff, 0x0032, 0x00ff, 0x00ff, 0x0028, 0x0028, 0x0032, 0x00ff, 0x0064,
                     0x0082, 0x000f, 0x00ff, 0x0000, 0x0000, 0x0000,
                     // I'm not convinced these aren't just padding, but stuff doesn't match
                     // without them :/
                     0x0000 };

u8** D_800E4770[] = {
    &drift_smoke_V, &drift_smoke_E, &D_8018D428, &D_8018D428, &D_8018D42C, &D_8018D42C,
    &D_8018D430, &D_8018D430, &D_8018D434, &D_8018D434, &D_8018D434, &D_8018D434,
};

u8** D_800E47A0[] = {
    &dobon1, &dobon2, &dobon3, &dobon4, &dobon5, &dobon6, &dobon7, &dobon8,
    &dobon9, &dobon10, &dobon11, &dobon12, &dobon13, &dobon14, &dobon15,
};

s32 NORM_SMOKE_IN[] = {
    MAKE_RGB(0xFB, 0xFF, 0xFB), MAKE_RGB(0xA0, 0x60, 0x11), MAKE_RGB(0xE0, 0xC0, 0x90), MAKE_RGB(0xD0, 0xB0, 0x80),
    MAKE_RGB(0x90, 0x70, 0x40), MAKE_RGB(0xC0, 0x70, 0x10), MAKE_RGB(0xD0, 0xF0, 0xFF), MAKE_RGB(0xE0, 0x90, 0x30),
    MAKE_RGB(0xC0, 0x90, 0x30), MAKE_RGB(0x60, 0x40, 0x20), MAKE_RGB(0xF0, 0xD0, 0xB0), MAKE_RGB(0xA0, 0x80, 0x30),
};

s32 NORM_SMOKE_OUT[] = {
    MAKE_RGB(0xB0, 0xB0, 0xB0), MAKE_RGB(0x80, 0x40, 0x11), MAKE_RGB(0xB0, 0x80, 0x50), MAKE_RGB(0xA0, 0x70, 0x40),
    MAKE_RGB(0x60, 0x30, 0x11), MAKE_RGB(0x80, 0x40, 0x10), MAKE_RGB(0x70, 0x90, 0xA0), MAKE_RGB(0xA0, 0x60, 0x30),
    MAKE_RGB(0xA0, 0x70, 0x10), MAKE_RGB(0x30, 0x10, 0x11), MAKE_RGB(0xB0, 0xA0, 0x80), MAKE_RGB(0x80, 0x60, 0x10),
};

// UI Code?
void KWSetViewportFull(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_80183D60), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
}

void KWReturnViewport(void) {
    switch (D_8018D21C) {
        case 0:
            SetViewport(D_800DC5EC);
            break;
        case 1:
            SetViewport(D_800DC5EC);
            break;
        case 2:
            SetViewport(D_800DC5F0);
            break;
        case 3:
            SetViewport(D_800DC5EC);
            break;
        case 4:
            SetViewport(D_800DC5F0);
            break;
        case 8:
            SetViewport(D_800DC5EC);
            break;
        case 9:
            SetViewport(D_800DC5F0);
            break;
        case 10:
            SetViewport(D_800DC5F4);
            break;
        case 11:
            SetViewport(D_800DC5F8);
            break;
    }
}

void kwdraw_safetyframe(void) {
    if (D_801657B2 != 0) {
        DrawLineHorizontal(0xF, 0xB, 0x122, 0, 0xFF, 0, 0xFF);
        DrawLineVertical(0x131, 0xB, 0xDA, 0, 0xFF, 0, 0xFF);
        DrawLineHorizontal(0xF, 0xE5, 0x122, 0, 0xFF, 0, 0xFF);
        DrawLineVertical(0xF, 0xB, 0xDA, 0, 0xFF, 0, 0xFF);
        DrawLineHorizontal(0x16, 0x10, 0x114, 0xFF, 0, 0, 0xFF);
        DrawLineVertical(0x12A, 0x10, 0xD0, 0xFF, 0, 0, 0xFF);
        DrawLineHorizontal(0x16, 0xE0, 0x114, 0xFF, 0, 0, 0xFF);
        DrawLineVertical(0x16, 0x10, 0xD0, 0xFF, 0, 0, 0xFF);
        DrawLineHorizontal(0x18, 0x15, 0x110, 0, 0, 0xFF, 0xFF);
        DrawLineVertical(0x128, 0x15, 0xC4, 0, 0, 0xFF, 0xFF);
        DrawLineHorizontal(0x18, 0xDB, 0x110, 0, 0, 0xFF, 0xFF);
        DrawLineVertical(0x18, 0x15, 0xC4, 0, 0, 0xFF, 0xFF);
    }
}

void kwdisplay_kumo(u32 arg0) {
    UNUSED Gfx* temp_v1;

    if ((g_KWDBDispSW != 0)) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, &D_0D0076F8);
    KW2DMatrixInit();

    if ((g_cloudsToggle != 0)) {
        return;
    }

    switch (arg0) {
        case 0:
            kwdisplay_kumo_1p();
            break;
        case 1:
            kwdisplay_kumo_2pl();
            break;
        case 2:
            kwdisplay_kumo_2pr();
            break;
        case 3:
            kwdisplay_kumo_2pu();
            break;
        case 4:
            kwdisplay_kumo_2pd();
            break;
    }
}

void kwdisplay3D(u32 arg0) {
    UNUSED Gfx* temp_v1;

    if (g_KWDBDispSW != 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, &D_0D0076F8);

    if (D_8018D22C != 0) {
        return;
    }

    switch (arg0) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            kwdisplay3D_1p();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
            kwdisplay3D_1p();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
            kwdisplay3D_2p();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
            kwdisplay3D_1p();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            kwdisplay3D_2p();
            break;
        case 5:
            kwdisplay3D_1p();
            break;
        case 6:
            kwdisplay3D_2p();
            break;
        case 7:
            kwdisplay3D_3p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
            kwdisplay3D_1p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
            kwdisplay3D_2p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
            kwdisplay3D_3p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            kwdisplay3D_4p();
            break;
    }
}

void kwdisplay3D_1p(void) {

    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    ogawa_draw(PLAYER_ONE);
    if (gGamestate == ENDING) {
        kwdisplay_dai(PLAYER_ONE);
        kwdisplay_cup(PLAYER_ONE);
        kwdisplay_pukupuku(PLAYER_ONE);
        KWDisplayStar(PLAYER_ONE);
        return;
    }
    if (!g_DemoFlag) {
        ExplorerKWDisplayJugemu(PLAYER_ONE);
    }
    EventDisplay(PLAYER_ONE);
}

void kwdisplay3D_2p(void) {

    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    ogawa_draw(PLAYER_TWO);
    if (!g_DemoFlag) {
        ExplorerKWDisplayJugemu(PLAYER_TWO);
    }
    EventDisplay(PLAYER_TWO);
}

void kwdisplay3D_3p(void) {
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[2]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    ogawa_draw(PLAYER_THREE);
    if (!g_DemoFlag) {
        ExplorerKWDisplayJugemu(PLAYER_THREE);
    }
    EventDisplay(PLAYER_THREE);
}

void kwdisplay3D_4p(void) {

    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[3]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    ogawa_draw(PLAYER_FOUR);
    if ((!g_DemoFlag) && (g_playerCount == 4)) {
        ExplorerKWDisplayJugemu(PLAYER_FOUR);
    }
    EventDisplay(PLAYER_FOUR);
}

void kwdisplay3D_after(u32 arg0) {
    UNUSED Gfx* temp_v1;

    if (g_KWDBDispSW != 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, &D_0D0076F8);

    if (D_8018D22C != 0) {
        return;
    }
    switch (arg0) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            kwdisplay3D_after_1p();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
            kwdisplay3D_after_1p();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
            kwdisplay3D_after_2p();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
            kwdisplay3D_after_1p();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            kwdisplay3D_after_2p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
            kwdisplay3D_after_1p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
            kwdisplay3D_after_2p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
            kwdisplay3D_after_3p();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            kwdisplay3D_after_4p();
            break;
    }
}

void kwdisplay3D_after_1p(void) {
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    if (gGamestate != ENDING) {
        EventDisplay_After(PLAYER_ONE);
    }
}

void kwdisplay3D_after_2p(void) {
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    EventDisplay_After(PLAYER_TWO);
}

void kwdisplay3D_after_3p(void) {
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[2]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    EventDisplay_After(PLAYER_THREE);
}

void kwdisplay3D_after_4p(void) {
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[3]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    EventDisplay_After(PLAYER_FOUR);
}

void KWDisplayEvent(s32 cameraId) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            break;
        case COURSE_CHOCO_MOUNTAIN:
            break;
        case COURSE_BOWSER_CASTLE:
            kwdisplay_doshin(cameraId);
            kwdisplay_fire(cameraId);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwdisplay_kanoke(cameraId);
                kwdisplay_bat(cameraId);
                kwdisplay_pukupuku(cameraId);
                kwdisplay_tellesa(cameraId);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            kwdisplay_goalflag(cameraId);
            if (gGamestate != CREDITS_SEQUENCE) {
                kwdisplay_harinezumi(cameraId);
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwdisplay_snowman(cameraId);
            }
            break;
        case COURSE_KOOPA_BEACH:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwdisplay_kani(cameraId);
            }
            if (gGamestate != CREDITS_SEQUENCE) {

                if ((g_menuMultiplayerSelection == 1) || (g_menuMultiplayerSelection == 2)) {
                    kwdisplay_kamome(cameraId);
                }
            } else {
                kwdisplay_kamome(cameraId);
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            break;
        case COURSE_LUIGI_RACEWAY:
            if (balloonsw != 0) {
                kwdisplay_balloon(cameraId);
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwdisplay_choropu(cameraId);
            }
            break;
        case COURSE_TOADS_TURNPIKE:
            break;
        case COURSE_KALAMARI_DESERT:
            kwdisplay_train_smoke(cameraId);
            break;
        case COURSE_SHERBET_LAND:
            if (gGamestate != CREDITS_SEQUENCE) {
                KWDisplayIceBlockShadow(cameraId);
            }
            kwdisplay_ping(cameraId);
            break;
        case COURSE_RAINBOW_ROAD:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwdisplay_neon(cameraId);
                kwdisplay_wanwan(cameraId);
            }
            break;
        case COURSE_WARIO_STADIUM:
            break;
        case COURSE_BLOCK_FORT:
            break;
        case COURSE_SKYSCRAPER:
            break;
        case COURSE_DOUBLE_DECK:
            break;
        case COURSE_DK_JUNGLE:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwdisplay_ship_smoke(cameraId);
            }
            break;
    }
#else

#endif

    kwdisplay_kame_fire(cameraId);
    kwdisplay_leaf(cameraId);

    if (D_80165730 != 0) {
        kwdisplay_fuusen(cameraId);
    }
    if (g_gameMode == BATTLE) {
        KWDisplayBombKartBT(cameraId);
    }
}

void render_snowing_effect(s32 arg0) {
    switch (g_courseID) {
        case COURSE_FRAPPE_SNOWLAND:
            if (gGamestate != 9) {
                if ((finish_zoom_flag == 0) && (g_playerCount == 1)) {
                    kwdisplay_snow();
                }
            } else {
                kwdisplay_snow();
            }
            break;
        case COURSE_SHERBET_LAND:
            KWDisplayIceBlock(arg0);
            break;
    }
}

void kwdisplay2D_init(void) {
    gSPDisplayList(gDisplayListHead++, &D_0D0076F8);
}

void KWDisplay2D(u32 arg0) {

    D_8018D21C = arg0;
    gSPDisplayList(gDisplayListHead++, &D_0D0076F8);

    if (D_8018D22C == 0) {
        switch (arg0) {
            case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
                kwdisplay2D_1p();
                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
                if (!g_DemoFlag) {
                    kwdisplay2D_2p_left();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
                if (!g_DemoFlag) {
                    kwdisplay2D_2p_right();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
                if (!g_DemoFlag) {
                    kwdisplay2D_2p_up();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
                if (!g_DemoFlag) {
                    kwdisplay2D_2p_down();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
                if (!g_DemoFlag) {
                    kwdisplay2D_4p_upleft();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
                if (!g_DemoFlag) {
                    kwdisplay2D_4p_upright();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
                if (!g_DemoFlag) {
                    kwdisplay2D_4p_downleft();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
                if ((!g_DemoFlag) && (g_playerCount == 4)) {
                    kwdisplay2D_4p_downright();
                }
                break;
        }
    }
}

void KWDisplay2DAfter(u32 arg0) {

    D_8018D21C = arg0;
    gSPDisplayList(gDisplayListHead++, &D_0D0076F8);
    if (D_8018D22C == 0) {
        switch (arg0) {
            case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
                kwdisplay2D_1p_after();
                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
                if (!g_DemoFlag) {
                    KWDisplay2D2PLeftAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
                if (!g_DemoFlag) {
                    kwdisplay2D_2p_right_after();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
                if (!g_DemoFlag) {
                    kwdisplay2D_2p_up_after();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
                if (!g_DemoFlag) {
                    kwdisplay2D_2p_down_after();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
                if (!g_DemoFlag) {
                    kwdisplay2D_4p_upleft_after();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
                if (!g_DemoFlag) {
                    kwdisplay2D_4p_upright_after();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
                if (!g_DemoFlag) {
                    kwdisplay2D_4p_downleft_after();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
                if ((!g_DemoFlag) && (g_playerCount == 4)) {
                    kwdisplay2D_4p_downright_after();
                }
                break;
        }
    }
}

void kwdisplay2D_1p(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void kwdisplay2D_1p_after(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
        if ((!g_DemoFlag) && (g_KWScreenEnable != 0) && (g_KWDemoSW == 0)) {
            KWDisplayItembox(PLAYER_ONE);
            if (g_hudLapToggle != 2) {
                KWDisplayTotalTime(PLAYER_ONE);
                Zanzou2(PLAYER_ONE);
                if (g_hudSpeedToggle2 != false) {
                    kwdisplay_speedmeter(0);
                }
            }
        }
    }
}

void kwdisplay1p_wipe(void) {
}

void kwdisplay_orders(void) {

    if (D_8018D2AC != 0) {
        switch (g_playerCount) {
            case 2:
                func_8004EB30(PLAYER_ONE);
                func_8004EB30(PLAYER_TWO);
                break;
            case 3:
                func_8004EB30(PLAYER_ONE);
                func_8004EB30(PLAYER_TWO);
                func_8004EB30(PLAYER_THREE);
                break;
            case 4:
                func_8004EB30(PLAYER_ONE);
                func_8004EB30(PLAYER_TWO);
                func_8004EB30(PLAYER_THREE);
                func_8004EB30(PLAYER_FOUR);
                break;
        }
    }
}

void kwdisplay_ranks(void) {
    if (D_8018D2A4 != 0) {
        if (g_gameMode != BATTLE) {
            switch (g_playerCount) {
                case 1:
                    if (g_gameMode != TIME_TRIALS) {
                        KWDisplayRank(PLAYER_ONE);
                        break;
                    }
                    break;
                case 2:
                    KWDisplayRank(PLAYER_ONE);
                    KWDisplayRank(PLAYER_TWO);
                    break;
                case 3:
                    kwdisplay_srank(PLAYER_ONE);
                    kwdisplay_srank(PLAYER_TWO);
                    kwdisplay_srank(PLAYER_THREE);
                    break;
                case 4:
                    kwdisplay_srank(PLAYER_ONE);
                    kwdisplay_srank(PLAYER_TWO);
                    kwdisplay_srank(PLAYER_THREE);
                    kwdisplay_srank(PLAYER_FOUR);
                    break;
            }
        }
    }
}

void kwdisplay_toppri(void) {

    if ((g_KWDBDispSW == 0) && (two_d_flag != 0)) {
        KWSetViewportFull();
        gSPDisplayList(gDisplayListHead++, &D_0D0076F8);

        if (g_KWScreenEnable != 0) {
            if (g_KWDemoSW == 0) {
                if (g_blueLineRankToggle != false) {
                    kwdisplay_enemycar();
                }
                if ((!g_DemoFlag) && (g_hudMapToggle2 != false)) {
                    if (D_80165800[0] != 0) {
                        kwdisplay_radarmap(0);
                        if (g_gameMode != BATTLE) {
                            kwdisplay_radarsline(0);
                        }
                        kwdisplay_radarplayer(0);
                    }
                    if ((g_ScreenSplitB == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) && (D_80165800[1] != 0)) {
                        kwdisplay_radarmap(1);
                        if (g_gameMode != BATTLE) {
                            kwdisplay_radarsline(1);
                        }
                        kwdisplay_radarplayer(1);
                    }
                }
            }
            if ((g_hudLapToggle != 2) && (g_gameMode == GRAND_PRIX) && (D_8018D2BC != 0)) {
                kwdisplay_kaopanel();
            }
            kwdisplay_ranks();
        }
        kwdisplay_orders();
        kwdraw_safetyframe();
    }
}

void kwdisplay2D_2p_left(void) {
}

void KWDisplay2D2PLeftAfter(void) {
    if (g_KWDBDispSW == 0) {
        KWDisplay2P_1LR();
    }
}

void kwdisplay2D_2p_right(void) {
}

void kwdisplay2D_2p_right_after(void) {
    if (g_KWDBDispSW == 0) {
        KWDisplay2P_2LR();
    }
}

void kwdisplay2D_2p_sub(s32 playerId) {
    if ((g_gameMode != BATTLE) && (D_80165800[playerId] == 0) && (g_KWScreenEnable != 0)) {
        KWDisplayTotalTime(playerId);
        kwdisplay_syuukai(playerId);
    }
    KWDisplayItembox(playerId);
}

void kwdisplay2D_2p_up(void) {
}

void kwdisplay2D_2p_up_after(void) {
    if (g_KWDBDispSW == 0) {
        KWDisplay2P_1UD();
    }
}

void kwdisplay2D_2p_down(void) {
}

void kwdisplay2D_2p_down_after(void) {
    if (g_KWDBDispSW == 0) {
        KWDisplay2P_2UD();
    }
}

void KWDisplayAfter4PSub(s32 playerId) {
    if (g_gameMode != BATTLE) {
        if (g_KWLapSW && g_KWScreenEnable) {
            kwsprite32x8(playerHUD[playerId].cx, playerHUD[playerId].cy, common_texture_hud_lap);
            kwdisplay_lap88(playerHUD[playerId].cx - 12, playerHUD[playerId].cy + 4,
                           playerHUD[playerId].lapcnt);
        }
        if (g_hudLapToggle == 2) {
            if (playerHUD[playerId].bomb && KWFlash8) {
                KWTexture2DCI8BL(playerHUD[playerId].bombx, playerHUD[playerId].bomby, 0, 1.0f,
                              (u8*) common_tlut_portrait_bomb_kart_and_question_mark, common_texture_portrait_bomb_kart,
                              D_0D005AE0, 0x20, 0x20, 0x20, 0x20);
            }
        }
    }
    KWDisplayItemboxs(playerId);
}

void kwdisplay2D_4p_upleft(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void kwdisplay2D_4p_upleft_after(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
        KWDisplay4P_1();
    }
}

void kwdisplay2D_4p_upright(void) {
}

void kwdisplay2D_4p_upright_after(void) {
    if (g_KWDBDispSW == 0) {
        KWDisplay4P_2();
    }
}

void kwdisplay2D_4p_downleft(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void kwdisplay2D_4p_downleft_after(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
        KWDisplay4P_3();
    }
}

void kwdisplay2D_4p_downright(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void kwdisplay2D_4p_downright_after(void) {
    if (g_KWDBDispSW == 0) {
        KW2DMatrixInit();
        KWDisplay4P_4();
    }
}

void kwplayerparams(s32 playerId) {
    Hud* temp_v0;

    D_8018CF1C = &gPlayerOne[playerId];
    D_8018CF14 = &camera1[playerId];
    temp_v0 = &playerHUD[playerId];
    temp_v0->plx = (s32) D_8018CF1C->position[0];
    temp_v0->ply = (s32) D_8018CF1C->position[1];
    temp_v0->plz = (s32) D_8018CF1C->position[2];
}

void kwcheck_randomcnt(s32 arg0) {
    struct Controller* controller = &gControllerOne[arg0];

    if ((controller->ButtonHeld & A_BUTTON) != 0) {
        gControllerRandom++;
    }
    if ((controller->ButtonHeld & B_BUTTON) != 0) {
        gControllerRandom++;
    }
    if ((controller->ButtonHeld & R_TRIG) != 0) {
        gControllerRandom++;
    }
}

void kw_resultscreen_start(void) {
    D_8018D214 = true;
}

void kwcheck_ghostkart_item(void) {
    s32 i;
    Player* player = gPlayerOne;
    for (i = 0; i < 4; i++) {
        if ((D_80165890 != 0) && (player->flag & IS_GHOST)) {
            player->item = ITEM_MUSHROOM;

            playerHUD[i].ghostcnt = 2;
        }
        if ((player->flag & IS_GHOST) && (player->item == ITEM_NONE)) {
            if (playerHUD[i].ghostcnt) {
                player->item = ITEM_MUSHROOM;
                --playerHUD[i].ghostcnt;
            }
        }
        ++player;
    }
    D_80165890 = 0;
}

void kwsyori_sub(s32 playerId) {
    kwplayerparams(playerId);
    if (!g_DemoFlag) {
        kwchart_jugemu_vf(playerId);
        kwchart_itemboxanm(playerId);
    }
}

void KWVideoFramesYori(void) {
    s32 i;

    if (pause_flag == false) {
        kwStartTime(&D_80165678);
        kwvideoframetimer++;
        for (i = 0; i < NUM_PLAYERS; i++) {
            D_8018CF68[i] = kwcheck_maparea_camera(&camera1[i]);
            kwchase_boundvel_zero(i);
        }
        switch (g_ScreenSplitB) {
            case SCREEN_MODE_1P:
                if (gGamestate != 9) {
                    kwsyori_sub(PLAYER_ONE);
                    if (g_gameMode == TIME_TRIALS) {
                        kwcheck_ghostkart_item();
                    }
                } else {
                    kwplayerparams(PLAYER_ONE);
                }
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                kwsyori_sub(PLAYER_ONE);
                kwsyori_sub(PLAYER_TWO);
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                kwsyori_sub(PLAYER_ONE);
                kwsyori_sub(PLAYER_TWO);
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                kwsyori_sub(PLAYER_ONE);
                kwsyori_sub(PLAYER_TWO);
                kwsyori_sub(PLAYER_THREE);
                kwsyori_sub(PLAYER_FOUR);
                break;
        }
        KWGameEventCommon_VF();
    }
}

void kwpausesyori(void) {
    s32 someIndex;
    s32 playerId;

    kwcheck_command();
    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        playerId = rank_to_num[someIndex];
        // I hate this dumb pointer access here
        gGPCurrentRaceCharacterIdByRank[someIndex] = (gPlayerOne + playerId)->kart;
    }
    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        KWRank[someIndex] = gGPCurrentRaceRankByPlayerId[someIndex];
    }
}

void kwgameframesyori_sub(void) {

    kwcheck_opening_start();
    kwcheck_command();
    kwset_kaopanel();

    if (D_801657AE == 0) {
        switch (g_ScreenSplitB) {
            case SCREEN_MODE_1P:
                kwcheck_randomcnt(PLAYER_ONE);
                if (D_8018D214 == false) {
                    kwplayerparams(PLAYER_ONE);
                    kwchart_opening();
                    if (!g_DemoFlag) {
                        kwchart_jugemu(0);
                    }
                    KWChart_Kumo_Hook_Default(0);
                    if (playerHUD[PLAYER_ONE].goalsw == 0) {
                        convkartspeedtoangle((gPlayerOneCopy->speed / 18.0f) * 216.0f);
                    }
                    kwlapcnt(PLAYER_ONE);
                } else {
                    kwplayerparams(PLAYER_ONE);
                    KWChart_Kumo_Hook_Default(1);
                    kwplayerparams(PLAYER_TWO);
                    KWChart_Kumo_Hook_Default(2);
                }
                CommonGameEventChart();
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                kwcheck_randomcnt(PLAYER_ONE);
                kwcheck_randomcnt(PLAYER_TWO);
                kwplayerparams(PLAYER_ONE);
                kwlapcnt(PLAYER_ONE);
                if (!g_DemoFlag) {
                    kwchart_jugemu(0);
                }
                KWChart_Kumo_Hook_Default(1);
                kwcheck_nearbomb(0);
                kwplayerparams(PLAYER_TWO);
                kwlapcnt(PLAYER_TWO);
                if (!g_DemoFlag) {
                    kwchart_jugemu(1);
                }
                KWChart_Kumo_Hook_Default(2);
                kwcheck_nearbomb(1);
                CommonGameEventChart();
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                kwcheck_randomcnt(PLAYER_ONE);
                kwcheck_randomcnt(PLAYER_TWO);
                kwplayerparams(PLAYER_ONE);
                kwlapcnt(PLAYER_ONE);
                if (!g_DemoFlag) {
                    kwchart_jugemu(0);
                }
                KWChart_Kumo_Hook_Default(3);
                kwcheck_nearbomb(0);
                kwplayerparams(PLAYER_TWO);
                kwlapcnt(PLAYER_TWO);
                if (!g_DemoFlag) {
                    kwchart_jugemu(1);
                }
                KWChart_Kumo_Hook_Default(4);
                kwcheck_nearbomb(1);
                CommonGameEventChart();
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                kwcheck_randomcnt(PLAYER_ONE);
                kwcheck_randomcnt(PLAYER_TWO);
                kwcheck_randomcnt(PLAYER_THREE);
                kwcheck_randomcnt(PLAYER_FOUR);
                kwplayerparams(PLAYER_ONE);
                kwlapcnt(PLAYER_ONE);
                if (!g_DemoFlag) {
                    kwchart_jugemu(0);
                }
                kwcheck_nearbomb(0);
                kwplayerparams(PLAYER_TWO);
                kwlapcnt(PLAYER_TWO);
                if (!g_DemoFlag) {
                    kwchart_jugemu(1);
                }
                kwcheck_nearbomb(1);
                kwplayerparams(PLAYER_THREE);
                kwlapcnt(PLAYER_THREE);
                if (!g_DemoFlag) {
                    kwchart_jugemu(2);
                }
                kwcheck_nearbomb(2);
                if (g_playerCount == 4) {
                    kwplayerparams(PLAYER_FOUR);
                    kwlapcnt(PLAYER_FOUR);
                    if ((!g_DemoFlag) && (g_playerCount == 4)) {
                        kwchart_jugemu(3);
                    }
                    kwcheck_nearbomb(3);
                }
                CommonGameEventChart();
                break;
        }
        kwdma_texture();
    }
}

void kwgameframesyori(void) {
    kwStartTime(&D_80165678);
    modelingnum = 0;
    D_801655C0 = 0;
    kwset_projection2D_matrix();
    if (pause_flag == false) {
        check_gftimer();
        if (gGamestate == ENDING) {
            kwchart_dai();
            kwchart_cup();
            kwchart_pukupuku(1);
            KWChartStar();
        } else if (gGamestate == CREDITS_SEQUENCE) {
            kwplayerparams(PLAYER_ONE);
            KWChart_Kumo_Hook_Default(0);
            CommonGameEventChart();
        } else {
            kwgameframesyori_sub();
        }
    }
    kwdebug();
    kwGetTime(&D_80165678, (s32) &D_801655F0);
}

void kwcheck_kaopanel(s32 playerId) {
    s32 objectIndex;
    s32 lapCount;
    Player* player;
    UNUSED s32 stackPadding;

    player = &gPlayerOne[playerId];
    objectIndex = D_8018CE10[playerId].objectIndex;
    lapCount = gLapCountByPlayerId[playerId];
    if (player->flag & EXISTS) {
        if (player->slip_flag &
            (THUNDER_SPIN | ROLLOVER | SPIN_L | SPIN_R)) {
            g_DynamicObjects[objectIndex].ang[2] += 0x1000;
        } else {
            if (g_DynamicObjects[objectIndex].ang[2] != 0) {
                g_DynamicObjects[objectIndex].ang[2] += 0x1000;
            }
        }
        if (player->slip_flag & THUNDER) {
            KWChaseFVal(&g_DynamicObjects[objectIndex].scale, 0.3f, 0.02f);
        } else {
            KWChaseFVal(&g_DynamicObjects[objectIndex].scale, 0.6f, 0.02f);
        }
        if (player->slip_flag & BROKEN) {
            kwadd_usval(&g_DynamicObjects[objectIndex].ang[0], 0x0C00U, 0x0100U);
        } else {
            kwsub_usval(&g_DynamicObjects[objectIndex].ang[0], 0, 0x00000100);
        }
        if (player->slip_flag & (THROW_EXPLODE | EXPLODE)) {
            kwmv_nageage_ground(objectIndex, 6.0f, 1.5f, 0.0f);
        } else {
            KWChaseFVal(&g_DynamicObjects[objectIndex].pos_spline[1], 0.0f, 1.0f);
        }
        if ((player->flag & IS_GHOST) || (player->slip_flag & TERESA)) {
            g_DynamicObjects[objectIndex].alpha = 0x0050;
        } else {
            g_DynamicObjects[objectIndex].alpha = 0x00FF;
        }
        if (lapCount >= 3) {
            g_DynamicObjects[objectIndex].ang[2] = 0;
            g_DynamicObjects[objectIndex].ang[1] = 0;
            g_DynamicObjects[objectIndex].ang[0] = 0;
            g_DynamicObjects[objectIndex].pos_spline[2] = 0.0f;
            g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
            g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
            g_DynamicObjects[objectIndex].scale = 0.6f;
            g_DynamicObjects[objectIndex].alpha = 0x00FF;
        }
    }
}

void kwset_kaopanel(void) {
    s32 temp_s0;
    for (temp_s0 = 0; temp_s0 < NUM_PLAYERS; ++temp_s0) {
        kwcheck_kaopanel(temp_s0);
    }
}

void kwcheck_command(void) {
    bool b = false;
    if ((gGamestate != ENDING) && (gGamestate != CREDITS_SEQUENCE) && !D_8018D204) {
        switch (g_playerCount) {
            case 1:
                if (gControllerOne->ButtonPressed & R_CBUTTONS) {
                    if (++g_hudLapToggle >= 3) {
                        g_hudLapToggle = 0;
                    }
                    if (g_hudLapToggle == 2) {
                        g_hudMapToggle2 = false;
                        g_hudSpeedToggle2 = false;
                        g_blueLineRankToggle = true;
                    } else if (g_hudLapToggle == 1) {
                        g_hudMapToggle2 = false;
                        g_hudSpeedToggle2 = true;
                        g_blueLineRankToggle = false;
                    } else {
                        g_hudMapToggle2 = true;
                        g_hudSpeedToggle2 = false;
                        g_blueLineRankToggle = false;
                    }
                    b = true;
                }
                break;
            case 2:
                if (g_gameMode != BATTLE) {
                    if (gControllerOne->ButtonPressed & R_CBUTTONS) {
                        D_80165800[0] = (D_80165800[0] + 1) & 1;
                        b = true;
                    }
                    if (gControllerTwo->ButtonPressed & R_CBUTTONS) {
                        D_80165800[1] = (D_80165800[1] + 1) & 1;
                        b = true;
                    }
                    if (D_80165800[0] && D_80165800[1]) {
                        g_blueLineRankToggle = false;
                    } else {
                        g_blueLineRankToggle = true;
                    }
                    if (g_DemoFlag) {
                        g_blueLineRankToggle = false;
                    }
                }
                break;
            case 3:
                if ((gControllerOne->ButtonPressed & R_CBUTTONS) || (gControllerTwo->ButtonPressed & R_CBUTTONS) ||
                    (gControllerThree->ButtonPressed & R_CBUTTONS)) {
                    if (g_gameMode != BATTLE) {
                        g_blueLineRankToggle = (g_blueLineRankToggle + 1) & 1;
                    }
                    g_hudLapToggle = (g_hudLapToggle + 1) & 1;
                    b = true;
                }
                break;
            case 4:
                if ((gControllerOne->ButtonPressed & R_CBUTTONS) || (gControllerTwo->ButtonPressed & R_CBUTTONS) ||
                    (gControllerThree->ButtonPressed & R_CBUTTONS) || (gControllerFour->ButtonPressed & R_CBUTTONS)) {
                    g_hudLapToggle = (g_hudLapToggle + 1) & 1;
                    g_KWLapSW = (g_KWLapSW + 1) & 1;
                    D_80165800[0] = (D_80165800[0] + 1) & 1;
                    if (g_gameMode != BATTLE) {
                        g_blueLineRankToggle = (g_blueLineRankToggle + 1) & 1;
                    }
                    b = true;
                }
                break;
        }
        if (b) {
            kwbackup_gamestatus(1);
        }
    }
}

void KWGameEventCommon_VF(void) {
    if (g_courseID == COURSE_BOWSER_CASTLE) {
        kwchart_doshin_vf();
    }
}

void KWGameEventCommon(void) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
        case COURSE_CHOCO_MOUNTAIN:
            break;
        case COURSE_BOWSER_CASTLE:
            kwchart_doshin();
            kwchart_fire();
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwchart_kanoke();
                kwchart_danro();
                if (g_gameMode != TIME_TRIALS) {
                    kwchart_bat();
                }
                kwchart_tellesa();
                kwchart_pukupuku(0);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            kwchart_goalflag();
            if (gGamestate != CREDITS_SEQUENCE) {
                kwchart_harinezumi();
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwchart_snowman();
            }
            KWChartSnow();
            break;
        case COURSE_KOOPA_BEACH:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwchart_kani();
            }
            if ((g_menuMultiplayerSelection == 1) || (g_menuMultiplayerSelection == 2) || (gGamestate == CREDITS_SEQUENCE)) {
                kwchart_kamome();
            }
            break;
        case COURSE_LUIGI_RACEWAY:
            if (balloonsw != 0) {
                kwchart_balloon();
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwchart_choropu();
            }
            break;
        case COURSE_KALAMARI_DESERT:
            kwchart_train_smoke();
            break;
        case COURSE_SHERBET_LAND:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwchart_iceblock();
            }
            kwchart_ping();
            break;
        case COURSE_RAINBOW_ROAD:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwchart_neon();
                kwchart_wanwan();
            }
            break;
        case COURSE_DK_JUNGLE:
            kwchart_ship_smoke();
            break;
    }
#else

#endif

    if (D_80165730 != 0) {
        kwchart_fuusen();
    }
    kwchart_kame_fire();
    if ((s16) g_courseID != COURSE_FRAPPE_SNOWLAND) {
        kwchart_leaf();
    }
}

void kwcheck_opening_start(void) {
    if (D_8018D170 == 0) {
        if (D_8018D178 == 0) {
            if (g_playerCount == 1) {
                kwchart_opening_start();
            }
            if (g_playerCount == 3) {
                g_hudMapToggle2 = true;
            }
            g_KWScreenEnable = (s32) 1;
            D_8018D170 = (s32) 1;
            D_8018D190 = (s32) 1;
            D_8018D204 = 0;
            return;
        }
        --D_8018D178;
    }
}

void kwchart_opening_start(void) {
    D_8018D1CC = 1;
    D_8018D1A0 = 0;
}

void kwchart_opening_next(void) {
    ++D_8018D1CC;
    D_8018D1A0 = 0;
}

void kwchart_opening_jump(s32 arg0) {
    D_8018D1CC = arg0;
    D_8018D1A0 = 0;
}

void kwchart_opening_finish(void) {
    D_8018D1CC = 0;
    D_8018D1A0 = 0;
}

void kwchart_opening_wait(s32 arg0) {
    if (D_8018D1A0 == 0) {
        D_8018D1D4 = arg0;
        D_8018D1A0 = 1;
    }

    --D_8018D1D4;
    if (D_8018D1D4 < 0) {
        D_8018D1A0 = 0;
        kwchart_opening_next();
    }
}

void kwchart_opening_init(void) {
    D_8018D1B4 = 1;
    D_8018D1A0 = 0;
    kwchart_opening_next();
}

void kwchart_opening_panel_start(void) {
    if ((g_gameMode == GRAND_PRIX) && (g_playerCount == 1)) {
        kwchart_opening_jump(0x14);
    }
}

void kwchart_opening_anime(void) {
    switch (playerHUD[PLAYER_ONE].mptr) {
        case 0:
            break;
        case 1:
            KWChaseSVal(&playerHUD[PLAYER_ONE].mx, 0x106, 0x10);
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].my, 182, 0x10) != 0) {
                playerHUD[PLAYER_ONE].mptr++;
                playerHUD[PLAYER_ONE].hptr = 1;
            }
            break;
        case 2:
            KWChaseSVal(&playerHUD[PLAYER_ONE].mx, 0x116, 4);
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].my, 0xC6, 4) != 0) {
                playerHUD[PLAYER_ONE].mptr++;
            }
            break;
        case 3:
            KWChaseSVal(&playerHUD[PLAYER_ONE].mx, 0x106, 4);
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].my, 182, 4) != 0) {
                playerHUD[PLAYER_ONE].mptr++;
            }
            break;
        case 4:
            KWChaseSVal(&playerHUD[PLAYER_ONE].mx, 0x10E, 4);
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].my, 0xBE, 4) != 0) {
                playerHUD[PLAYER_ONE].mptr++;
            }
            break;
        case 5:
            KWChaseSVal(&playerHUD[PLAYER_ONE].mx, 0x106, 4);
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].my, 182, 4) != 0) {
                playerHUD[PLAYER_ONE].mptr++;
            }
            break;
        case 6:
            KWChaseSVal(&playerHUD[PLAYER_ONE].mx, 0x10A, 2);
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].my, 0xBA, 2) != 0) {
                playerHUD[PLAYER_ONE].mptr++;
            }
            break;
        case 7:
            KWChaseSVal(&playerHUD[PLAYER_ONE].mx, 0x106, 2);
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].my, 182, 2) != 0) {
                playerHUD[PLAYER_ONE].mptr++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].mptr = 0;
            break;
    }
    if ((playerHUD[PLAYER_ONE].hptr != 0) && (playerHUD[PLAYER_ONE].hptr == 1)) {
        if (++D_801657E7 >= 0x10) {
            D_801657E7 = 0;
            D_8016579E = 0xDD00;
            playerHUD[PLAYER_ONE].hptr = 0U;
        } else {
            D_8016579E = D_800E55B0[D_801657E7] + 0xDD00;
        }
    }
    switch (playerHUD[PLAYER_ONE].iptr) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].addiy, 0x40, 8) != 0) {
                playerHUD[PLAYER_ONE].iptr++;
            }
            break;
        case 2:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].addiy, 0x38, 8) != 0) {
                playerHUD[PLAYER_ONE].iptr++;
            }
            break;
        case 3:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].addiy, 0x40, 8) != 0) {
                playerHUD[PLAYER_ONE].iptr++;
            }
            break;
        case 4:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].addiy, 0x38, 8) != 0) {
                playerHUD[PLAYER_ONE].iptr++;
            }
            break;
        case 5:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].addiy, 0x40, 8) != 0) {
                playerHUD[PLAYER_ONE].iptr++;
            }
            break;
        case 6:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].addiy, 0x38, 4) != 0) {
                playerHUD[PLAYER_ONE].iptr++;
            }
            break;
        case 7:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].addiy, 0x40, 4) != 0) {
                playerHUD[PLAYER_ONE].iptr++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].iptr = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].tptr) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].timerX, 0xE4, 0x10) != 0) {
                playerHUD[PLAYER_ONE].tptr++;
            }
            break;
        case 2:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].timerX, 0xF4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr++;
            }
            break;
        case 3:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].timerX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr++;
            }
            break;
        case 4:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].timerX, 0xEC, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr++;
            }
            break;
        case 5:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].timerX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr++;
            }
            break;
        case 6:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].timerX, 0xE8, 2) != 0) {
                playerHUD[PLAYER_ONE].tptr++;
            }
            break;
        case 7:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].timerX, 0xE4, 2) != 0) {
                playerHUD[PLAYER_ONE].tptr++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].tptr = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].cptr) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx, 0x53, 0x10) != 0) {
                playerHUD[PLAYER_ONE].cptr++;
            }
            break;
        case 2:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx, 0x43, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr++;
            }
            break;
        case 3:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr++;
            }
            break;
        case 4:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx, 0x4B, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr++;
            }
            break;
        case 5:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr++;
            }
            break;
        case 6:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx, 0x4F, 2) != 0) {
                playerHUD[PLAYER_ONE].cptr++;
            }
            break;
        case 7:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx, 0x53, 2) != 0) {
                playerHUD[PLAYER_ONE].cptr++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].cptr = 0;
            break;
    }
    D_8018CFEC = (f32) (playerHUD[PLAYER_ONE].mx + 0x18);
    D_8018CFF4 = (f32) (playerHUD[PLAYER_ONE].my + 6);
    switch (playerHUD[PLAYER_ONE].tptr2) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 0x10) != 0) {
                playerHUD[PLAYER_ONE].tptr2++;
            }
            break;
        case 2:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xF4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr2++;
            }
            break;
        case 3:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr2++;
            }
            break;
        case 4:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xEC, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr2++;
            }
            break;
        case 5:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr2++;
            }
            break;
        case 6:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE8, 2) != 0) {
                playerHUD[PLAYER_ONE].tptr2++;
            }
            break;
        case 7:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 2) != 0) {
                playerHUD[PLAYER_ONE].tptr2++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].tptr2 = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].cptr2) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx2, 0x53, 0x10) != 0) {
                playerHUD[PLAYER_ONE].cptr2++;
            }
            break;
        case 2:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx2, 0x43, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr2++;
            }
            break;
        case 3:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx2, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr2++;
            }
            break;
        case 4:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx2, 0x4B, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr2++;
            }
            break;
        case 5:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx2, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr2++;
            }
            break;
        case 6:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx2, 0x4F, 2) != 0) {
                playerHUD[PLAYER_ONE].cptr2++;
            }
            break;
        case 7:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx2, 0x53, 2) != 0) {
                playerHUD[PLAYER_ONE].cptr2++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].cptr2 = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].tptr3) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 0x10) != 0) {
                playerHUD[PLAYER_ONE].tptr3++;
            }
            break;
        case 2:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xF4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr3++;
            }
            break;
        case 3:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr3++;
            }
            break;
        case 4:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xEC, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr3++;
            }
            break;
        case 5:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 4) != 0) {
                playerHUD[PLAYER_ONE].tptr3++;
            }
            break;
        case 6:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE8, 2) != 0) {
                playerHUD[PLAYER_ONE].tptr3++;
            }
            break;
        case 7:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 2) != 0) {
                playerHUD[PLAYER_ONE].tptr3++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].tptr3 = 0;
            break;
    }
    switch (playerHUD[PLAYER_ONE].cptr3) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx3, 0x53, 0x10) != 0) {
                playerHUD[PLAYER_ONE].cptr3++;
            }
            break;
        case 2:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx3, 0x43, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr3++;
            }
            break;
        case 3:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx3, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr3++;
            }
            break;
        case 4:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx3, 0x4B, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr3++;
            }
            break;
        case 5:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx3, 0x53, 4) != 0) {
                playerHUD[PLAYER_ONE].cptr3++;
            }
            break;
        case 6:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx3, 0x4F, 2) != 0) {
                playerHUD[PLAYER_ONE].cptr3++;
            }
            break;
        case 7:
            if (KWChaseSVal(&playerHUD[PLAYER_ONE].cx3, 0x53, 2) != 0) {
                playerHUD[PLAYER_ONE].cptr3++;
            }
            break;
        case 8:
            playerHUD[PLAYER_ONE].cptr3 = 0;
            break;
    }
}

void kwchart_goal_anime(void) {
    f32 temp_f0;
    f32* temp_s2;
    f32* temp_s3;
    f32* temp_s4;
    UNUSED f32* var_s1;
    s32 var_s0;

    KWChaseSVal(&playerHUD[PLAYER_ONE].lap1CompletionTimeX, 0xE4, 0x10);
    KWChaseSVal(&playerHUD[PLAYER_ONE].lap2CompletionTimeX, 0xE4, 0x10);
    KWChaseSVal(&playerHUD[PLAYER_ONE].lap3CompletionTimeX, 0xE4, 0x10);
    KWChaseSVal(&playerHUD[PLAYER_ONE].totalTimeX, 0xE4, 0x10);
    for (var_s0 = 0; var_s0 < NUM_PLAYERS; var_s0++) {
        temp_s2 = &panelx[var_s0];
        temp_s3 = &D_8018D0C8[var_s0];
        temp_s4 = &D_8018D078[var_s0];
        if (g_hudCharpicRankY1[var_s0] >= 0.0f) {
            KWChaseFVal(temp_s2, *temp_s3, *temp_s4);
            temp_f0 = *temp_s2;
            if (temp_f0 == *temp_s3) {
                *temp_s4 = 0.0f;
            }
            if ((f64) temp_f0 <= -32.0) {
                g_hudCharpicRankY1[var_s0] = -32.0f;
            }
        }
    }
}

void kwchart_opening(void) {
    s32 i;
    UNUSED s32 unk;

    switch (D_8018D1CC) {
        case 0:
            break;
        case 0x1:
            kwchart_opening_init();
            break;
        case 0x2:
            if (g_gameMode == TIME_TRIALS) {
                playerHUD[PLAYER_ONE].iptr = 1;
            }
            playerHUD[PLAYER_ONE].mptr = 1;
            playerHUD[PLAYER_ONE].tptr = 1;
            playerHUD[PLAYER_ONE].cptr = 1;
            kwchart_opening_next();
            break;
        case 0x3:
            kwchart_opening_wait(0);
            break;
        case 0x4:
            playerHUD[PLAYER_ONE].tptr2 = 1;
            playerHUD[PLAYER_ONE].cptr2 = 1;
            kwchart_opening_next();
            break;
        case 0x5:
            kwchart_opening_wait(0);
            break;
        case 0x6:
            playerHUD[PLAYER_ONE].tptr3 = 1;
            playerHUD[PLAYER_ONE].cptr3 = 1;
            kwchart_opening_next();
            kwchart_opening_finish();
            break;
        case 0x14:
            D_8018D078[0] = 16.0f;
            kwchart_opening_next();
            break;
        case 0x15:
            kwchart_opening_wait(4);
            break;
        case 0x16:
            D_8018D078[1] = 16.0f;
            kwchart_opening_next();
            break;
        case 0x17:
            kwchart_opening_wait(4);
            break;
        case 0x18:
            D_8018D078[2] = 16.0f;
            kwchart_opening_next();
            break;
        case 0x19:
            kwchart_opening_wait(4);
            break;
        case 0x1A:
            D_8018D078[3] = 16.0f;
            kwchart_opening_next();
            break;
        case 0x1B:
            kwchart_opening_wait(0xA);
            break;
        case 0x1C:
            kwchart_opening_finish();
            break;
        case 0x64:
            kwchart_opening_next();
            break;
        case 0x65:
            kwchart_opening_wait(0x3A);
            break;
        case 0x66:
            D_8018D078[0] = -8.0f;
            D_8018D0C8[0] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x67:
            kwchart_opening_wait(4);
            break;
        case 0x68:
            D_8018D078[1] = -8.0f;
            D_8018D0C8[1] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x69:
            kwchart_opening_wait(4);
            break;
        case 0x6A:
            D_8018D078[2] = -8.0f;
            D_8018D0C8[2] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x6B:
            kwchart_opening_wait(4);
            break;
        case 0x6C:
            D_8018D078[3] = -8.0f;
            D_8018D0C8[3] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x6D:
            kwchart_opening_wait(0xA);
            break;
        case 0x6E:
            for (i = 0; i < NUM_PLAYERS; i += 4) {
                g_hudCharpicRankY1[i] = -32.0f;
                g_hudCharpicRankY1[i + 1] = -32.0f;
                g_hudCharpicRankY1[i + 2] = -32.0f;
                g_hudCharpicRankY1[i + 3] = -32.0f;
            }
            panelx[0] = 360.0f;
            g_hudCharpicRankY1[0] = 110.0f;
            D_8018D0C8[0] = 44.0f;
            D_8018D078[0] = -16.0f;
            kwgoalpanelsw = 1;
            kwchart_opening_next();
            break;
        case 0x6F:
            kwchart_opening_wait(4);
            break;
        case 0x70:
            panelx[1] = 360.0f;
            g_hudCharpicRankY1[1] = 110.0f;
            D_8018D0C8[1] = 76.0f;
            D_8018D078[1] = -16.0f;
            kwchart_opening_next();
            break;
        case 0x71:
            kwchart_opening_wait(4);
            break;
        case 0x72:
            panelx[2] = 360.0f;
            g_hudCharpicRankY1[2] = 110.0f;
            D_8018D0C8[2] = 108.0f;
            D_8018D078[2] = -16.0f;
            kwchart_opening_next();
            break;
        case 0x73:
            kwchart_opening_wait(4);
            break;
        case 0x74:
            panelx[3] = 360.0f;
            g_hudCharpicRankY1[3] = 110.0f;
            D_8018D0C8[3] = 140.0f;
            D_8018D078[3] = -16.0f;
            kwchart_opening_next();
            break;
        case 0x75:
            kwchart_opening_wait(4);
            break;
        case 0x76:
            panelx[4] = 360.0f;
            g_hudCharpicRankY1[4] = 110.0f;
            D_8018D0C8[4] = 180.0f;
            D_8018D078[4] = -16.0f;
            kwchart_opening_next();
            break;
        case 0x77:
            kwchart_opening_wait(4);
            break;
        case 0x78:
            panelx[5] = 360.0f;
            g_hudCharpicRankY1[5] = 110.0f;
            D_8018D0C8[5] = 212.0f;
            D_8018D078[5] = -16.0f;
            kwchart_opening_next();
            break;
        case 0x79:
            kwchart_opening_wait(4);
            break;
        case 0x7A:
            panelx[6] = 360.0f;
            g_hudCharpicRankY1[6] = 110.0f;
            D_8018D0C8[6] = 244.0f;
            D_8018D078[6] = -16.0f;
            kwchart_opening_next();
            break;
        case 0x7B:
            kwchart_opening_wait(4);
            break;
        case 0x7C:
            panelx[7] = 360.0f;
            g_hudCharpicRankY1[7] = 110.0f;
            D_8018D0C8[7] = 276.0f;
            D_8018D078[7] = -16.0f;
            kwchart_opening_next();
            break;
        case 0x7D:
            kwchart_opening_wait(0xA);
            break;
        case 0x7E:
            for (i = 0; i < NUM_PLAYERS; i++) {
                D_8018D078[i] = 0.0f;
            }
            kwchart_opening_next();
            break;
        case 0x7F:
            kwchart_opening_wait(0x82);
            break;
        case 0x80:
            if (gGPCurrentRaceRankByPlayerId[0] < 4) {
                kwchart_opening_jump(0x8C);
            } else {
                kwchart_opening_jump(0x82);
            }
            break;
        case 0x82:
            kwchart_opening_finish();
            break;
        case 0x8C:
            D_8018D078[0] = -16.0f;
            D_8018D0C8[0] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x8D:
            kwchart_opening_wait(4);
            break;
        case 0x8E:
            D_8018D078[1] = -16.0f;
            D_8018D0C8[1] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x8F:
            kwchart_opening_wait(4);
            break;
        case 0x90:
            D_8018D078[2] = -16.0f;
            D_8018D0C8[2] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x91:
            kwchart_opening_wait(4);
            break;
        case 0x92:
            D_8018D078[3] = -16.0f;
            D_8018D0C8[3] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x93:
            kwchart_opening_wait(4);
            break;
        case 0x94:
            D_8018D078[4] = -16.0f;
            D_8018D0C8[4] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x95:
            kwchart_opening_wait(4);
            break;
        case 0x96:
            D_8018D078[5] = -16.0f;
            D_8018D0C8[5] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x97:
            kwchart_opening_wait(4);
            break;
        case 0x98:
            D_8018D078[6] = -16.0f;
            D_8018D0C8[6] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x99:
            kwchart_opening_wait(4);
            break;
        case 0x9A:
            D_8018D078[7] = -16.0f;
            D_8018D0C8[7] = -32.0f;
            kwchart_opening_next();
            break;
        case 0x9B:
            kwchart_opening_wait(0x14);
            break;
        case 0x9C:
            kwchart_opening_finish();
            break;
    }
    if (D_8018D1CC < 0x64) {
        kwchart_opening_anime();
    } else if (D_8018D1CC < 0xC8) {
        kwchart_goal_anime();
    }
    if ((D_8018D1CC != 0) && (D_8018D1CC >= 0x14) && (D_8018D1CC < 0x1E)) {
        for (i = 0; i < 4; i++) {
            KWChaseFVal(&panelx[i], D_8018D0C8[i], D_8018D078[i]);
            if (panelx[i] == D_8018D0C8[i]) {
                D_8018D078[i] = 0.0f;
            }
        }
    }
}

void convkartspeedtoangle(f32 arg0) {
    if (!playerHUD[PLAYER_ONE].hptr) {
        u16 v;
        if (arg0 < 10.0) {
            v = (u16) (128.0f * arg0) + 0xDD00;
        } else if (arg0 < 20.0) {
            v = (u16) ((arg0 - 10.0) * 256.0) + 0xE200;
        } else {
            v = (u16) ((arg0 - 20.0) * 268.8) + 0xEC00;
        }
        if (arg0 == D_8018CFE4) {
            if (arg0 > 5.0f) {
                if (++D_801657E7 == 8) {
                    D_801657E7 = 0;
                }
            } else {
                D_801657E7 = 0;
            }
        }
        D_8016579E = v + D_800E55A0[D_801657E7];
        D_8018CFE4 = arg0;
    }
}

void kwdisplayon(UNUSED s32* arg0) {
}

void kwdisplayoff(s32* arg0) {
    *arg0 = 0;
}

void displayfinish(s32 arg0) {
    D_8018D2C8[arg0] = 1;
}

void flashcolor_table(s8 index, s16* x, s16* y, s16* z) {
    s16* src = &D_800E4730[index * 3];
    *x = *src++;
    *y = *src++;
    *z = *src++;
}

void kwget_color(s8 arg0, s16* arg1, s16* arg2, s16* arg3) {
    switch (arg0) {
        case 0:
            *arg1 = 0xFF;
            *arg2 = 0x40;
            *arg3 = 0x40;
            break;
        case 1:
            *arg1 = 0xFF;
            *arg2 = 0xFF;
            *arg3 = 0x40;
            break;
        case 2:
            *arg1 = 0x40;
            *arg2 = 0x40;
            *arg3 = 0xFF;
            break;
    }
}

void check_gftimer(void) {
    s16 x;
    s16 y;
    s16 z;
    s32 temp_t7;

    temp_t7 = ++kwgameframetimer;
    D_8018D40C = temp_t7 & 0x3F; // temp_t7 % 64
    D_8018D410 = temp_t7 & 0x1F; // temp_t7 % 32
    KW16GFTimer = temp_t7 & 0xF;  // temp_t7 % 16
    KW8GFTimer = temp_t7 & 7;    // temp_t7 % 8
    KW4GFTimer = temp_t7 & 3;    // temp_t7 % 4
    KW2GFTimer = temp_t7 & 1;    // temp_t7 % 2
    if (D_8018D40C == 0) {
        KW64GFCount += 1;
        D_801655D8 ^= 1;
    }
    if (D_8018D410 == 0) {
        KW32GFCount += 1;
        D_801655E8 ^= 1;
    }
    if (KW16GFTimer == 0) {
        KW16GFCount += 1;
        D_801655F8 ^= 1;
    }
    if (KW8GFTimer == 0) {
        KW8GFCount += 1;
        KWFlash8 ^= 1;
    }
    if (KW4GFTimer == 0) {
        KW4GFCount += 1;
        D_80165618 ^= 1;
    }
    if (KW2GFTimer == 0) {
        KW2GFCount += 1;
        D_80165628 ^= 1;
    }
    if (--D_8018D2AC < 0) {
        D_8018D2AC = 0;
    }
    D_801658A8 += 1;
    if (D_801658A8 >= 7) {
        D_801658A8 = 0;
    }
    flashcolor_table(D_801658A8, &x, &y, &z);
    kwflash_r = x / 2;
    kwflash_g = y / 2;
    kwflash_b = z / 2;
    kwcheck_rank();
}

void kwcheck_rank(void) {
    s32 var_v0;
    s32 sp0;
    s32 temp_v1;
    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        temp_v1 = gGPCurrentRaceRankByPlayerId[var_v0];
        if (KW16GFTimer == 0) {
            KWRank[var_v0] = temp_v1;
        }
        D_8018CF28[temp_v1] = &gPlayerOne[sp0];
        if (sp0 == 0) {
            D_80165794 = temp_v1;
        }
    }

    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        sp0 = rank_to_num[var_v0];
        D_8018CF50[var_v0] = sp0;
        if (KW16GFTimer == 0) {
            gGPCurrentRaceCharacterIdByRank[var_v0] = (gPlayerOne + sp0)->kart;
        }
    }

    if (--D_8018D314 <= 0) {
        D_8018D314 = wakuanmtmmax;
        wakur = D_800E55D0[wakuanmptr][0];
        wakug = D_800E55D0[wakuanmptr][1];
        wakub = D_800E55D0[wakuanmptr][2];
        if (++wakuanmptr == 6) {
            wakuanmptr = 0;
        }
    }
}

void kwlapcntsub(s32 playerId, s32 lapCount) {
    s32 temp_a0_2;
    UNUSED s32 stackPadding;
    s8* huh;
    s8* huhthedeuce;
    Player* player;

    player = &gPlayerOne[playerId];
    huh = &playerHUD[playerId].lapcnt;
    huhthedeuce = &playerHUD[playerId].goal;
    if (playerHUD[playerId].goal < kwgoalcnt) {
        playerHUD[playerId].totaltime = (u32) (s32) (g_gameTimer * 100.0f);
        if (*huh < lapCount) {
            temp_a0_2 = gTimePlayerLastTouchedFinishLine[playerId] * 100.0f;
            playerHUD[playerId].gettime = temp_a0_2;
            playerHUD[playerId].timebuf[*huh] = temp_a0_2;
            if (*huh == 0) {
                playerHUD[playerId].laptimebuf[*huh] = playerHUD[playerId].gettime;
            } else {
                playerHUD[playerId].laptimebuf[*huh] =
                    playerHUD[playerId].timebuf[*huh] - playerHUD[playerId].timebuf[*huh - 1];
            }
            playerHUD[playerId].laptime = playerHUD[playerId].laptimebuf[*huh];
            playerHUD[playerId].flashtm = 0x003C;
            if (lapCount == 3) {
                playerHUD[playerId].totaltime = playerHUD[playerId].timebuf[*huh];
            }
            if (g_gameMode == (s32) 1) {
                if (D_80165638 >= playerHUD[playerId].laptime) {
                    if (D_80165638 != playerHUD[playerId].laptime) {
                        D_80165658[0] = D_80165658[1] = 0;
                    }
                    NAPlyVoiceStart(0U, (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    D_80165638 = playerHUD[playerId].laptime;
                    D_80165658[lapCount - 1] = 1;
                    D_801657E3 = 1;
                }
                if ((lapCount == 3) && ((u32) playerHUD[playerId].totaltime < (u32) D_80165648)) {
                    D_801657E5 = 1;
                }
            }
            *huh += 1;
            if (kwgoalcnt == *huh) {
                *huh = kwgoalcnt - 1;
            }
            *huhthedeuce += 1;
            if (1) {}
            switch (*huhthedeuce) { /* switch 1; irregular */
                case 0:             /* switch 1 */
                    break;
                case 1: /* switch 1 */
                    kwlap2_jugemu(playerId);
                    NAPlyTrgStart(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0xF0, 0x15));
                    if ((g_courseID == COURSE_LUIGI_RACEWAY) && (balloonsw == 0) &&
                        (g_gameMode != (s32) 1)) {
                        balloonsw = 1;
                    }
                    break;
                case 2: /* switch 1 */
                    kwfinal_jugemu(playerId);
                    break;
                case 3: /* switch 1 */
                    if ((D_8018D114 == 0) || (D_8018D114 == 1)) {
                        g_hudLapToggle = 0;
                        g_hudSpeedToggle2 = 0;
                        g_blueLineRankToggle = 0;
                        g_hudMapToggle2 = 1;
                        D_80165800[0] = 1;
                        D_80165800[1] = 1;
                        D_8018D204 = (s32) 1;
                    }
                    playerHUD[playerId].goalsw = 1;
                    if (D_8018D114 == 2) {
                        D_80165800[playerId] = 0;
                    }
                    if (g_courseID == COURSE_YOSHI_VALLEY) {
                        playerHUD[playerId].ranksw = 1;
                    }
                    playerHUD[playerId].lap1CompletionTimeX = 0x0140;
                    playerHUD[playerId].lap2CompletionTimeX = 0x01E0;
                    playerHUD[playerId].lap3CompletionTimeX = 0x0280;
                    playerHUD[playerId].totalTimeX = 0x0320;
                    D_8016587C = (s32) 1;
                    if (D_8018D20C == 0) {
                        kwgoal_jugemu(playerId);
                        D_8018D20C = 1;
                        if (g_menuMultiplayerSelection == (s8) 1) {
                            D_8018D1CC = 0x00000064;
                        }
                    }
                    break;
            }
        }
    } else {
        KWChaseFVal(&playerHUD[playerId].rs, 1.0f, 0.125f);
        switch (g_ScreenSplitB) { /* irregular */
            case 0:
                KWChaseSVal(&playerHUD[playerId].rgx, 0x001C, 7);
                if (D_8018D1FC != 0) {
                    KWChaseSVal(&playerHUD[playerId].rgy, -0x0028, 1);
                } else {
                    KWChaseSVal(&playerHUD[playerId].rgy, -0x0010, 4);
                }
                break;
            case 2:
                KWChaseSVal(&playerHUD[playerId].rgx, 0x001C, 7);
                KWChaseSVal(&playerHUD[playerId].rgy, -0x0010, 4);
                break;
            case 1:
                KWChaseSVal(&playerHUD[playerId].rgx, 0x001C, 7);
                KWChaseSVal(&playerHUD[playerId].rgy, -0x0010, 4);
                KWChaseSVal(&playerHUD[playerId].lap1CompletionTimeX, 0x00E4, 0x0010);
                KWChaseSVal(&playerHUD[playerId].lap2CompletionTimeX, 0x00E4, 0x0010);
                KWChaseSVal(&playerHUD[playerId].lap3CompletionTimeX, 0x00E4, 0x0010);
                KWChaseSVal(&playerHUD[playerId].totalTimeX, 0x00E4, 0x0010);
                break;
            case 3:
                if ((playerId & 1) == 1) {
                    KWChaseSVal(&playerHUD[playerId].rgx, -8, 2);
                } else {
                    KWChaseSVal(&playerHUD[playerId].rgx, 8, 2);
                }
                KWChaseSVal(&playerHUD[playerId].rgy, -0x0010, 4);
                break;
        }
    }
    if (playerHUD[playerId].flashtm == 0) {
        playerHUD[playerId].laptime = playerHUD[playerId].totaltime;
        D_801657E3 = 0;
        return;
    }
    if (KW8GFTimer == 0) {
        playerHUD[playerId].dispsw += 1;
        playerHUD[playerId].dispsw &= 1;
    }
    playerHUD[playerId].flashtm -= 1;
    if (playerHUD[playerId].flashtm == 0) {
        playerHUD[playerId].dispsw = 0;
    }
}

void kwlapcnt(s32 playerId) {
    if (g_gameMode != BATTLE) {
        switch (playerId) { /* irregular */
            case PLAYER_ONE:
                kwlapcntsub(playerId, gLapCountByPlayerId[PLAYER_ONE]);
                break;
            case PLAYER_TWO:
                kwlapcntsub(playerId, gLapCountByPlayerId[PLAYER_TWO]);
                break;
            case PLAYER_THREE:
                kwlapcntsub(playerId, gLapCountByPlayerId[PLAYER_THREE]);
                break;
            case PLAYER_FOUR:
                kwlapcntsub(playerId, gLapCountByPlayerId[PLAYER_FOUR]);
                break;
        }
    }
}

void kwgoal_rank_scroll(void) {
    if ((g_gameMode == GRAND_PRIX) && (g_playerCount == TIME_TRIALS)) {
        g_KWDemoSW = 1;
        D_8018D2BC = 0;
        D_8018D2A4 = 0;
        if (gGPCurrentRaceRankByPlayerId[0] >= 4) {
            D_8018D1FC = 1;
            D_8018D2A4 = 1;
            D_8018D2BC = 1;
        }
    }
}

void kwcheck_nearbomb(s32 arg0) {
    s32 playerPathPoint;
    s32 bombPathPoint;
    s32 var_a2;
    s32 pathPointDiff;

    if (g_gameMode == 2) {
        playerPathPoint = gNearestPathPointByPlayerId[arg0];
        playerHUD[arg0].bomb = 0;
        for (var_a2 = 0; var_a2 < NUM_BOMB_KARTS_VERSUS; var_a2++) {
            if ((bom[var_a2].state == BOMB_STATE_EXPLODED) ||
                (bom[var_a2].state == BOMB_STATE_INACTIVE)) {
                continue;
            }
            bombPathPoint = bom[var_a2].pathPointIndex;
            pathPointDiff = bombPathPoint - playerPathPoint;
            if ((pathPointDiff < -5) || (pathPointDiff > 0x1E)) {
                continue;
            }
            playerHUD[arg0].bomb = 1;
            break;
        }
    }
}

// Appears to load GP Mode race staging balloons and kart shadows.
void imageinit(void) {
    hoshi = DecodeMapImage1(gTexture69C80C, 0x400, 0x400);
    kage_bitmap = DecodeMapImage1(gTextureKartShadow, 0x1000, 0x1000);
    drift_smoke_V = DecodeMapImage1(gTexture69B03C, 0x100, 0x100);
    drift_smoke_E = DecodeMapImage1(gTexture69B140, 0x400, 0x400);
    D_8018D478 = DecodeMapImage1(gTexture69C1E8, 0x200, 0x200);
    fukidashi2 = DecodeMapImage1(gTextureSpeechBubble, 0x400, 0x400);
    onpu = DecodeMapImage1(gTextureMusicNote, 0x400, 0x400);
    D_8018D48C = DecodeMapImage1(gTextureSmokePuff, 0x400, 0x400);
    smoke = DecodeMapImage1(gGroundDust, 0x400, 0x400);
    D_8018D490 = D_8018D48C;
    grs = DecodeMapImage1(gGrassParticle, 0x1000, 0x1000);
    balloon1 = DecodeMapImage1(gTextureBalloon1, 0x800, 0x800);
    balloon2 = DecodeMapImage1(gTextureBalloon2, 0x800, 0x800);
    bubble = DecodeMapImage1(gTexture69C9C4, 0x200, 0x200);
    boing = DecodeMapImage1(gTextureBoingExclamation, 0x800, 0x800);
    poomp1 = DecodeMapImage1(gTextureOnomatopoeiaPoomp1, 0x800, 0x800);
    poomp2 = DecodeMapImage1(gTextureOnomatopoeiaPoomp2, 0x800, 0x800);
    whir1 = DecodeMapImage1(gTextureOnomatopoeiaWhrrrr1, 0x800, 0x800);
    whir2 = DecodeMapImage1(gTextureOnomatopoeiaWhrrrr2, 0x800, 0x800);
    crash1 = DecodeMapImage1(gTextureOnomatopoeiaCrash1, 0x800, 0x800);
    crash2 = DecodeMapImage1(gTextureOnomatopoeiaCrash2, 0x800, 0x800);
    dobon1 = DecodeMapImage1(gTexture69CB84, 0x800, 0x800);
    dobon2 = DecodeMapImage1(gTexture69CCEC, 0x800, 0x800);
    dobon3 = DecodeMapImage1(gTexture69CEB8, 0x800, 0x800);
    dobon4 = DecodeMapImage1(gTexture69D148, 0x800, 0x800);
    dobon5 = DecodeMapImage1(gTexture69D4E0, 0x800, 0x800);
    dobon6 = DecodeMapImage1(gTexture69D8FC, 0x800, 0x800);
    dobon7 = DecodeMapImage1(gTexture69DCB4, 0x800, 0x800);
    dobon8 = DecodeMapImage1(gTexture69DFA0, 0x800, 0x800);
    dobon9 = DecodeMapImage1(gTexture69E25C, 0x800, 0x800);
    dobon10 = DecodeMapImage1(gTexture69E518, 0x800, 0x800);
    dobon11 = DecodeMapImage1(gTexture69E7A8, 0x800, 0x800);
    dobon12 = DecodeMapImage1(gTexture69EA18, 0x800, 0x800);
    dobon13 = DecodeMapImage1(gTexture69EC54, 0x800, 0x800);
    dobon14 = DecodeMapImage1(gTexture69EE38, 0x800, 0x800);
    dobon15 = DecodeMapImage1(gTexture69EFE0, 0x800, 0x800);
    thunderA = DecodeMapImage1(gTextureLightningBolt0, 0x800, 0x800);
    thunderB = DecodeMapImage1(gTextureLightningBolt1, 0x800, 0x800);
}

void effectinit(Player* player) {
    s32 temp_v0;

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        player->gass[temp_v0].flag = 0;
        player->gass[temp_v0].timer = 0;
        player->gass[temp_v0].number = NO_PARTICLE;
    }

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        player->spark[temp_v0].flag = 0;
        player->spark[temp_v0].timer = 0;
        player->spark[temp_v0].number = NO_PARTICLE;
    }

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        player->smoke[temp_v0].flag = 0;
        player->smoke[temp_v0].timer = 0;
        player->smoke[temp_v0].number = NO_PARTICLE;
    }

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        player->letter[temp_v0].flag = 0;
        player->letter[temp_v0].timer = 0;
        player->letter[temp_v0].number = NO_PARTICLE;
    }
}

void MakePos(Player* player, Particle* arg1, f32 x, f32 y, f32 z, s8 surfaceType, s8 arg6) {
    arg1->trz = z;
    arg1->trx = x;
    arg1->try = y;
    arg1->ang = -player->direction[1];
    arg1->type = surfaceType;
    arg1->lrflag = arg6;
}

s32 MakeStartup(Particle* arg0, s8 type, f32 arg2) {
    arg0->flag = true;
    arg0->number = type;
    arg0->timer = 0;
    arg0->scale = arg2;
}

s32 MakeRDP(Particle* arg0, s32 color, s16 alpha) {
    arg0->swork1 = (u8) (color >> 16);
    arg0->swork2 = (u8) (color >> 8);
    arg0->swork3 = (u8) color;
    arg0->swork4 = alpha;
}

s32 MakeRandomRDP(Particle* arg0, s32 arg1, s16 alpha) {
    s32 temp_v0;
    temp_v0 = MakeRandomLimmit(48);

    arg0->swork1 = (u8) ((u8) (arg1 >> 16) - temp_v0);
    arg0->swork2 = (u8) ((u8) (arg1 >> 8) - temp_v0);
    arg0->swork3 = (u8) ((u8) arg1 - temp_v0);
    arg0->swork4 = alpha;
}

void initdrift_smoke_set(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 temp_lo;

    if (player->slipang >= 0) {
        MakePos(player, &player->smoke[arg1], player->tire_RL.Position[0],
                      player->tire_RL.Height + 2.0f, player->tire_RL.Position[2],
                      player->tire_RL.Status, 1);
    } else {
        MakePos(player, &player->smoke[arg1], player->tire_RR.Position[0],
                      player->tire_RR.Height + 2.0f, player->tire_RR.Position[2],
                      player->tire_RR.Status, 0);
    }

    temp_lo = player->slipang / DEGREES(1);
    if ((temp_lo >= 7) || (temp_lo < -6)) {
        MakeStartup(&player->smoke[arg1], DRIFT_PARTICLE, 0.35f);
        if (player->powerstack_count == 0) {
            MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x70);
        }

        if (player->powerstack_count == 1) {
            MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0x00), 0x70);
        }

        if (player->powerstack_count >= 2) {
            MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0x96, 0x00), 0x70);
        }

        if (player->powerstack_count >= 2) {
            // Why not put this in previous if statement?
            player->smoke[arg1].swork5 = 2;
            return;
        }

        player->smoke[arg1].swork5 = player->powerstack_count;
    }
}

void initdrift_smoke(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((arg1 == 0) && ((player->smoke[arg2].timer >= 3) || (player->smoke[arg2].flag == 0))) {
        initdrift_smoke_set(player, arg1, arg2, arg3, arg4);
    } else if (player->smoke[arg2].timer >= 3) {
        initdrift_smoke_set(player, arg1, arg2, arg3, arg4);
    }
}

UNUSED void set_init_smoke(void) {
}

void set_up_smoke(Particle* particle, s16 arg1, s16 arg2, s16 arg3) {
    particle->swork1 = arg1;
    particle->swork4 = arg3;
    particle->swork5 = arg2;
}

void InitRndSmoke(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    UNUSED s32 stackPadding;
    s32 surfaceType;
    s32 var_t3;
    f32 tire_x;
    f32 tire_y;
    f32 tire_z;
    s32 randval;
    static s32 test = 8;

    surfaceType = 0x000000FF;
    randval = MakeRandomLimmit(test);
    if ((randval == 0) || (randval == 4)) {
        tire_x = player->tire_RL.Position[0];
        tire_y = player->tire_RL.Height + 2.0f;
        tire_z = player->tire_RL.Position[2];
        var_t3 = 1;
        surfaceType = player->tire_RL.Status;
    }
    if ((randval == 2) || (randval == 6)) {
        tire_x = player->tire_RR.Position[0];
        tire_y = player->tire_RR.Height + 2.0f;
        tire_z = player->tire_RR.Position[2];
        var_t3 = 0;
        surfaceType = player->tire_RR.Status;
    }
    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    if ((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_ROYAL_RACEWAY)) {
                        set_up_smoke(&player->smoke[arg1], 1, 0, 0x0080);
                    }
                    if (g_courseID == COURSE_KALAMARI_DESERT) {
                        set_up_smoke(&player->smoke[arg1], 7, 0, 0x0080);
                    }
                    if (g_courseID == COURSE_MOO_MOO_FARM) {
                        set_up_smoke(&player->smoke[arg1], 8, 0, 0x0080);
                    }
                    if (g_courseID == COURSE_WARIO_STADIUM) {
                        set_up_smoke(&player->smoke[arg1], 9, 0, 0x0080);
                    }
                    if (g_courseID == COURSE_YOSHI_VALLEY) {
                        set_up_smoke(&player->smoke[arg1], 10, 0, 0x0080);
                    }
                    if (g_courseID == COURSE_DK_JUNGLE) {
                        set_up_smoke(&player->smoke[arg1], 11, 0, 0x0080);
                    }
                    player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                if ((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_ROYAL_RACEWAY)) {
                    set_up_smoke(&player->smoke[arg1], 1, 0, 0x0080);
                }
                if (g_courseID == COURSE_KALAMARI_DESERT) {
                    set_up_smoke(&player->smoke[arg1], 7, 0, 0x0080);
                }
                if (g_courseID == COURSE_MOO_MOO_FARM) {
                    set_up_smoke(&player->smoke[arg1], 8, 0, 0x0080);
                }
                if (g_courseID == COURSE_WARIO_STADIUM) {
                    set_up_smoke(&player->smoke[arg1], 9, 0, 0x0080);
                }
                if (g_courseID == COURSE_YOSHI_VALLEY) {
                    set_up_smoke(&player->smoke[arg1], 10, 0, 0x0080);
                }
                if (g_courseID == COURSE_DK_JUNGLE) {
                    set_up_smoke(&player->smoke[arg1], 11, 0, 0x0080);
                }
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GRASS_PARTICLE, 1.0f);
                    MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                    player->smoke[arg1].swork1 -= arg1 * 8;
                    player->smoke[arg1].swork2 -= arg1 * 8;
                    player->smoke[arg1].swork3 -= arg1 * 8;
                }
            } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GRASS_PARTICLE, 1.0f);
                MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                player->smoke[arg1].swork1 -= arg1 * 8;
                player->smoke[arg1].swork2 -= arg1 * 8;
                player->smoke[arg1].swork3 -= arg1 * 8;
            }
            player->smoke[arg1].try -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    set_up_smoke(&player->smoke[arg1], 2, 1, 0x00A8);
                    player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                set_up_smoke(&player->smoke[arg1], 2, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    set_up_smoke(&player->smoke[arg1], 3, 1, 0x00A8);
                    player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                set_up_smoke(&player->smoke[arg1], 3, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    set_up_smoke(&player->smoke[arg1], 4, 1, 0x00A8);
                    player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                set_up_smoke(&player->smoke[arg1], 4, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    set_up_smoke(&player->smoke[arg1], 5, 1, 0x00A8);
                    player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                MakeRandomRDP(&player->smoke[arg1], 0x00FFA54F, 0x00AF);
                set_up_smoke(&player->smoke[arg1], 5, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    set_up_smoke(&player->smoke[arg1], 6, 1, 0x00A8);
                    player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                set_up_smoke(&player->smoke[arg1], 6, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                if (((((player->speed / 18.0f) * 216.0f) >= 30.0f) &&
                     ((((player->slipang / DEGREES(1)) > 0x14) || ((player->slipang / DEGREES(1)) < (-0x14))))) ||
                    ((player->old_speed - player->speed) >= 0.04)) {
                    MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    set_up_smoke(&player->smoke[arg1], 0, 0, 0x0080);
                    player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((player->smoke[arg2].timer > 0) &&
                       (((((player->speed / 18.0f) * 216.0f) >= 30.0f) &&
                         (((player->slipang / DEGREES(1)) >= 0x15) || ((player->slipang / DEGREES(1)) < -0x14))) ||
                        ((player->old_speed - player->speed) >= 0.04))) {
                MakePos(player, &player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                set_up_smoke(&player->smoke[arg1], 0, 0, 0x0080);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        default:
            break;
    }
}

void initrnd_spl(Player* player, s16 arg1, s32 arg2, s8 arg3, UNUSED s8 arg4) {
    s32 temp_v0;
    s32 surfaceType;
    s32 var_t1;
    f32 x;
    f32 y;
    f32 z;
    static s32 test = 10;

    surfaceType = 0x000000FF;
    temp_v0 = MakeRandomLimmit(test);
    if ((temp_v0 == 0) || (temp_v0 == 8)) {
        if ((g_waterlevelPlayer[arg3] - player->tire_RL.Height) >= 3.5) {
            x = player->tire_RL.Position[0];
            y = player->tire_RL.Height + 2.0f;
            z = player->tire_RL.Position[2];
            var_t1 = 1;
            surfaceType = 0;
        }
    }
    if ((temp_v0 == 2) || (temp_v0 == 6)) {
        if ((g_waterlevelPlayer[arg3] - player->tire_RR.Height) >= 3.5) {
            x = player->tire_RR.Position[0];
            y = player->tire_RR.Height + 2.0f;
            z = player->tire_RR.Position[2];
            var_t1 = 0;
            surfaceType = 0;
        }
    }
    if (1) {}
    if (surfaceType == 0) {
        if ((arg1 == 0) && ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
            if (((player->speed / 18.0f) * 216.0f) >= 10.0f) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 9, 0.8f);
                MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00AF);
            }
        } else if ((player->smoke[arg2].timer > 0) && (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
            MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
            MakeStartup(&player->smoke[arg1], 9, 0.8f);
            MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00AF);
        }
    }
}

void InitRapidSmoke(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 temp_v0;
    s32 surfaceType;
    s32 var_t3;
    f32 x;
    f32 y;
    f32 z;
    static s32 test = 8;

    surfaceType = 0x000000FF;
    temp_v0 = MakeRandomLimmit(test);
    if ((temp_v0 == 2) || (temp_v0 == 4)) {
        x = player->tire_RL.Position[0];
        y = player->tire_RL.Height + 2.0f;
        z = player->tire_RL.Position[2];
        var_t3 = 1;
        surfaceType = player->tire_RL.Status;
    }
    if ((temp_v0 == 0) || (temp_v0 == 6)) {
        x = player->tire_RR.Position[0];
        y = player->tire_RR.Height + 2.0f;
        z = player->tire_RR.Position[2];
        var_t3 = 0;
        surfaceType = player->tire_RR.Status;
    }

    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                if ((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_ROYAL_RACEWAY)) {
                    set_up_smoke(&player->smoke[arg1], 1, 0, 0x0080);
                }
                if (g_courseID == COURSE_KALAMARI_DESERT) {
                    set_up_smoke(&player->smoke[arg1], 7, 0, 0x0080);
                }
                if (g_courseID == COURSE_MOO_MOO_FARM) {
                    set_up_smoke(&player->smoke[arg1], 8, 0, 0x0080);
                }
                if (g_courseID == COURSE_WARIO_STADIUM) {
                    set_up_smoke(&player->smoke[arg1], 9, 0, 0x0080);
                }
                if (g_courseID == COURSE_YOSHI_VALLEY) {
                    set_up_smoke(&player->smoke[arg1], 10, 0, 0x0080);
                }
                if (g_courseID == COURSE_DK_JUNGLE) {
                    set_up_smoke(&player->smoke[arg1], 11, 0, 0x0080);
                }
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                if ((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_ROYAL_RACEWAY)) {
                    set_up_smoke(&player->smoke[arg1], 1, 0, 0x0080);
                }
                if (g_courseID == COURSE_KALAMARI_DESERT) {
                    set_up_smoke(&player->smoke[arg1], 7, 0, 0x0080);
                }
                if (g_courseID == COURSE_MOO_MOO_FARM) {
                    set_up_smoke(&player->smoke[arg1], 8, 0, 0x0080);
                }
                if (g_courseID == COURSE_WARIO_STADIUM) {
                    set_up_smoke(&player->smoke[arg1], 9, 0, 0x0080);
                }
                if (g_courseID == COURSE_YOSHI_VALLEY) {
                    set_up_smoke(&player->smoke[arg1], 0x000A, 0, 0x0080);
                }
                if (g_courseID == COURSE_DK_JUNGLE) {
                    set_up_smoke(&player->smoke[arg1], 0x000B, 0, 0x0080);
                }
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.1f);
                MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                player->smoke[arg1].swork1 -= arg1 * 8;
                player->smoke[arg1].swork2 -= arg1 * 8;
                player->smoke[arg1].swork3 -= arg1 * 8;
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.1f);
                MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                player->smoke[arg1].swork1 -= arg1 * 8;
                player->smoke[arg1].swork2 -= arg1 * 8;
                player->smoke[arg1].swork3 -= arg1 * 8;
            }
            player->smoke[arg1].try -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 2, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 2, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 3, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 3, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 4, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 4, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 5, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 5, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 6, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 6, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 0, 0, 0x0080);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&player->smoke[arg1], 5, 0.46f);
                set_up_smoke(&player->smoke[arg1], 0, 0, 0x0080);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        default:
            break;
    }
}

void InitSpinSmoke(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_t1;
    u8 surfaceType;
    f32 x;
    f32 y;
    f32 z;

    var_t1 = 0;
    if ((player->slip_flag & SPIN_L) == SPIN_L) {
        x = player->position[0];
        y = player->position[1] - player->radius;
        z = player->position[2];
        var_t1 = 1;
        surfaceType = player->tire_RL.Status & 0xFF;
    } else {
        x = player->position[0];
        y = player->position[1] - player->radius;
        z = player->position[2];
        surfaceType = player->tire_RR.Status & 0xFF;
    }
    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                if ((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_ROYAL_RACEWAY)) {
                    set_up_smoke(&player->smoke[arg1], 1, 0, 0x0080);
                }
                if (g_courseID == COURSE_KALAMARI_DESERT) {
                    set_up_smoke(&player->smoke[arg1], 7, 0, 0x0080);
                }
                if (g_courseID == COURSE_MOO_MOO_FARM) {
                    set_up_smoke(&player->smoke[arg1], 8, 0, 0x0080);
                }
                if (g_courseID == COURSE_WARIO_STADIUM) {
                    set_up_smoke(&player->smoke[arg1], 9, 0, 0x0080);
                }
                if (g_courseID == COURSE_YOSHI_VALLEY) {
                    set_up_smoke(&player->smoke[arg1], 0x000A, 0, 0x0080);
                }
                if (g_courseID == COURSE_DK_JUNGLE) {
                    set_up_smoke(&player->smoke[arg1], 0x000B, 0, 0x0080);
                }
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                if ((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_ROYAL_RACEWAY)) {
                    set_up_smoke(&player->smoke[arg1], 1, 0, 0x0080);
                }
                if (g_courseID == COURSE_KALAMARI_DESERT) {
                    set_up_smoke(&player->smoke[arg1], 7, 0, 0x0080);
                }
                if (g_courseID == COURSE_MOO_MOO_FARM) {
                    set_up_smoke(&player->smoke[arg1], 8, 0, 0x0080);
                }
                if (g_courseID == COURSE_WARIO_STADIUM) {
                    set_up_smoke(&player->smoke[arg1], 9, 0, 0x0080);
                }
                if (g_courseID == COURSE_YOSHI_VALLEY) {
                    set_up_smoke(&player->smoke[arg1], 0x000A, 0, 0x0080);
                }
                if (g_courseID == COURSE_DK_JUNGLE) {
                    set_up_smoke(&player->smoke[arg1], 0x000B, 0, 0x0080);
                }
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.1f);
                MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                player->smoke[arg1].swork1 -= arg1 * 8;
                player->smoke[arg1].swork2 -= arg1 * 8;
                player->smoke[arg1].swork3 -= arg1 * 8;
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.1f);
                MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                player->smoke[arg1].swork1 -= arg1 * 8;
                player->smoke[arg1].swork2 -= arg1 * 8;
                player->smoke[arg1].swork3 -= arg1 * 8;
            }
            player->smoke[arg1].try -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 2, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 2, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 3, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 3, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 4, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 4, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 5, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 5, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 6, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 6, 1, 0x00A8);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 0, 0, 0x0080);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            } else if (player->smoke[arg2].timer > 0) {
                MakePos(player, &player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&player->smoke[arg1], 4, 0.46f);
                set_up_smoke(&player->smoke[arg1], 0, 0, 0x0080);
                player->smoke[arg1].swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        default:
            break;
    }
}

void initengine_smoke(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    UNUSED s32 thing1;
    s16 thing2;
    UNUSED s32 thing3;
    f32 x;
    f32 y;
    f32 z;
    f32 var_f0;
    s32 var_v0;
    s32 temp_v0;
    UNUSED s32 test;

    if ((player->handling_flag & ACCELERATE) == ACCELERATE) {
        var_v0 = 5;
    } else {
        var_v0 = 0xE;
    }
    temp_v0 = MakeRandomLimmit(var_v0);
    if ((temp_v0 == 1) || (temp_v0 == 2) || (temp_v0 == 3)) {
        if ((arg1 == 0) && ((player->gass[arg2].timer > 0) || (player->gass[arg2].flag == 0))) {
            y = player->position[1] - 2.5;
            z = player->position[2];
            x = player->position[0];
            MakePos(player, &player->gass[arg1], x, y, z, 0, 0);
            MakeStartup(&player->gass[arg1], 1, 0.5f);
        } else if (player->gass[arg2].timer > 0) {
            y = player->position[1] - 2.5;
            z = player->position[2];
            x = player->position[0];
            MakePos(player, &player->gass[arg1], x, y, z, 0, 0);
            MakeStartup(&player->gass[arg1], 1, 0.5f);
        }
    }
    player->gass[arg1].fwork1 = 0.0f;
    if ((player->handling_flag & ACCELERATE) == ACCELERATE) {
        player->gass[arg1].swork5 = 0;
        if ((player->slip_flag & TURBO) == TURBO) {
            MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0x00), 0x0080);
            player->gass[arg1].swork1 = 1;
        } else {
            MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0070);
            player->gass[arg1].swork1 = 0;
        }
    } else {
        player->gass[arg1].swork5 = 1;
        if ((player->slip_flag & TURBO) == TURBO) {
            MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0x00), 0x0080);
            player->gass[arg1].swork1 = 1;
        } else {
            MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0070);
            player->gass[arg1].swork1 = 0;
        }
    }
    thing2 = (player->gass[arg1].ang - (player->slipang / 2));
    if (player->gass[arg1].swork5 == 0) {
        var_f0 = -((player->taco / 3000.0f) + 0.1);
    } else {
        var_f0 = -((player->taco / 5000.0f) + 0.1);
    }
    make_smokepos(&x, &y, &z, 0.0f, 4.5f, (player->gass[arg1].timer * var_f0) + -5.5, -thing2,
                  -player->kartroll * 2);
    player->gass[arg1].trx = player->position[0] + x;
    y = y + (player->position[1] - player->radius);
    player->gass[arg1].trz = player->position[2] + z;
    player->gass[arg1].try = player->gass[arg1].fwork1 + y;
    player->gass[arg1].lrflag = 0;
}

void init_ring_set(Player* player, s16 arg1, UNUSED s32 arg2, s8 arg3, UNUSED s8 arg4) {
    f32 var_f0;
    f32 sp50;
    f32 sp4C;
    f32 sp48;

    var_f0 = 8.0f - (g_waterlevelPlayer[arg3] - player->position[1]);
    if ((f64) var_f0 <= 0.0) {
        var_f0 = 0.0f;
    }
    sp4C = (g_waterlevelPlayer[arg3] - player->position[1]) - 3.0f;
    if ((player->water_flag & SUBMERGED) && (g_courseID != COURSE_KOOPA_BEACH)) {
        var_f0 = 2.5f;
        sp4C = (f32) ((f64) (g_waterlevelPlayer[arg3] - player->position[1]) + 0.1);
    }
    MakePos(player, &player->gass[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
    MakeStartup(&player->gass[arg1], 3, var_f0);
    if ((g_courseID == COURSE_BOWSER_CASTLE) || (g_courseID == COURSE_BIG_DONUT)) {
        MakeRDP(&player->gass[arg1], RGB32(0x0, 0x0, 0x0), 0x00AF);
    } else {
        MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00CF);
    }
    make_smokepos(&sp50, &sp4C, &sp48, 0.0f, sp4C,
                  ((-player->gass[arg1].timer * (player->speed / 18.0f) * 216.0f) / 10.0f) + -4.0f,
                  -player->gass[arg1].ang, -player->kartroll * 2);
    player->gass[arg1].trx = player->position[0] + sp50;
    player->gass[arg1].trz = player->position[2] + sp48;
    player->gass[arg1].try = player->position[1] + sp4C;
}

void init_ring(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((g_courseID != COURSE_SKYSCRAPER) && (g_courseID != COURSE_RAINBOW_ROAD)) {
        if ((arg1 == 0) && ((player->gass[arg2].timer > 0) || (player->gass[arg2].flag == 0))) {
            init_ring_set(player, arg1, arg2, arg3, arg4);
        } else if (player->gass[arg2].timer > 0) {
            init_ring_set(player, arg1, arg2, arg3, arg4);
        }
    }
}

void init_bubble(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 sp54;
    UNUSED s32 pad;
    s32 sp4C;
    f32 sp48;
    f32 sp44;

    if (g_courseID == COURSE_SKYSCRAPER) {
        return;
    }
    if (g_courseID == COURSE_RAINBOW_ROAD) {
        return;
    }
    sp54 = MakeRandomLimmit(0x0168U) - 0xB4;
    sp4C = MakeRandomLimmit(6U);
    sp44 = MakeRandomLimmit(6U);
    sp48 = MakeRandomLimmit(3U);
    if (player != gPlayerOne) {
        return;
    }
    if ((arg1 == 0) && ((player->smoke[arg2].timer > 0) || (player->smoke[arg2].flag == 0))) {
        MakePos(player, &player->smoke[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&player->smoke[arg1], 0x0B, 0.4f);
        MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        player->smoke[arg1].trz = player->position[2] + (cosT(sp54 * DEGREES(1)) * -1.8);
        player->smoke[arg1].trx = player->position[0] + (sinT(sp54 * DEGREES(1)) * -1.8);
        player->smoke[arg1].try = (player->position[1] - player->radius) + sp4C + 2.0f;
        player->smoke[arg1].dev = sp44 + 1.0f;
        player->smoke[arg1].scale = (sp48 + 2.0f) / 10.0f;
    } else if (player->smoke[arg2].timer > 0) {
        MakePos(player, &player->smoke[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&player->smoke[arg1], 0x0B, 0.4f);
        MakeRDP(&player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        player->smoke[arg1].trz = player->position[2] + (cosT(sp54 * DEGREES(1)) * -1.8);
        player->smoke[arg1].trx = player->position[0] + (sinT(sp54 * DEGREES(1)) * -1.8);
        player->smoke[arg1].try = (player->position[1] - player->radius) + sp4C + 2.0f;
        player->smoke[arg1].dev = sp44 + 1.0f;
        player->smoke[arg1].scale = (sp48 + 2.0f) / 10.0f;
    }
}

void init_dobon(Player* player, s16 arg1, UNUSED s32 arg2, s8 arg3, UNUSED s8 arg4) {
    MakePos(player, &player->gass[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
    MakeStartup(&player->gass[arg1], 5, 4.0f);

    if ((g_courseID == COURSE_BOWSER_CASTLE) || (g_courseID == COURSE_BIG_DONUT)) {
        MakeRDP(&player->gass[arg1], RGB32(0xFF, 0x00, 0x00), 0xFF);
    } else {
        MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0xFF);
    }

    player->gass[arg1].trz = player->position[2] + (cosT(player->gass[arg1].ang) * -5.8);
    player->gass[arg1].trx = player->position[0] + (sinT(player->gass[arg1].ang) * -5.8);
    player->gass[arg1].try = g_waterlevelPlayer[arg3];
    player->water_flag &= ~SPLASH_DIVE;
}

void init_thunder(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    if (arg1 == 0) {
        MakePos(player, &player->gass[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
        MakeStartup(&player->gass[arg1], 6, 3.8f);
        MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0xFF);
        player->gass[arg1].swork1 = 0;
        player->gass[arg1].swork2 = 0;
        player->gass[arg1].swork3 = 0;
    }
}

void init_teresa_set(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    MakePos(player, &player->gass[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
    MakeStartup(&player->gass[arg1], 7, 0.6f);
    MakeRDP(&player->gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0xD0);

    player->gass[arg1].trz = player->position[2] + (cosT(player->gass[arg1].ang) * 6.0f);
    player->gass[arg1].trx = player->position[0] + (sinT(player->gass[arg1].ang) * 6.0f);
    player->gass[arg1].try = player->position[1] - 5.0f;
    player->gass[arg1].swork5 = 0;
    player->gass[arg1].fwork1 = 0.0f;
}

void init_teresa(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((arg1 == 0) && ((player->gass[arg2].timer > 0) || (player->gass[arg1].flag == 0))) {
        init_teresa_set(player, arg1, arg2, arg3, arg4);
    } else if (player->gass[arg2].timer >= 2) {
        init_teresa_set(player, arg1, arg2, arg3, arg4);
        if (arg1 == 9) {
            player->handling_flag &= ~BOO_SMOKE_EFFECT;
        }
    }
}

void initstar_dust(Player* player, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_s2;

    for (var_s2 = 0; var_s2 < 10; var_s2++) {
        player->spark[var_s2].flag = 1;
        player->spark[var_s2].fwork2 = player->position[1] + 5.0f;
        player->spark[var_s2].ang = (40 * DEGREES(1) * var_s2) - player->direction[1];
        player->spark[var_s2].fwork1 = (MakeRandomLimmit(0x0064U) / 100.0f) + 1.5;
        player->spark[var_s2].swork2 = 0;
        player->spark[var_s2].number = 1;
        player->spark[var_s2].timer = 0;
        player->spark[var_s2].swork4 = 0x00FF;
        player->spark[var_s2].trz = player->position[2];
        player->spark[var_s2].trx = player->position[0];
    }
    player->hit_flag &= ~0x0008;
}

void initresult_star(Player* player, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_s2;

    for (var_s2 = 0; var_s2 < 7; var_s2++) {
        player->spark[var_s2].flag = 1;
        player->spark[var_s2].fwork2 = player->position[1] - 4.0f;
        player->spark[var_s2].ang = (40 * DEGREES(1) * var_s2) - player->direction[1];
        // ???
        player->spark[var_s2].fwork1 = (MakeRandomLimmit(0x0064U) / 100.0f) + 1.9;
        player->spark[var_s2].fwork1 = (MakeRandomLimmit(0x0064U) / 100.0f) + 1.5;
        player->spark[var_s2].swork2 = 0;
        player->spark[var_s2].number = 9;
        player->spark[var_s2].timer = 0;
        player->spark[var_s2].swork4 = 0x00FF;
        player->spark[var_s2].trz = player->position[2];
        player->spark[var_s2].trx = player->position[0];
    }
    player->handling_flag &= ~TROPHYJUMP_STARS;
}

void initpowder_dust(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 test = 2;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    f32 temp_f0;
    f32 sp28[10] = { (f32) -DEGREES(1), (f32) DEGREES(1), (f32) DEGREES(2), (f32) -DEGREES(2), DEGREES(3), -DEGREES(3), DEGREES(4), -DEGREES(4), DEGREES(5), -DEGREES(5) };

    if (MakeRandomLimmit(3U) == 2.0f) {
        player->spark[arg1].flag = 1;
        player->spark[arg1].trx = player->position[0];
        player->spark[arg1].trz = player->position[2];
        player->spark[arg1].ang = -player->direction[1] + sp28[arg1];
        player->spark[arg1].dev = MakeRandomLimmit(1U) + 2.0f;
        temp_f0 = MakeRandomLimmit(4U);
        temp_f0 -= test;
        player->spark[arg1].type = temp_f0;
        player->spark[arg1].try = player->position[1] + temp_f0;
        player->spark[arg1].scale = 0.15f;
        player->spark[arg1].number = 5;
        player->spark[arg1].timer = 0;
        player->spark[arg1].swork4 = 0x00FF;
        player->spark[arg1].swork1 = 0;
    }
}

void initburn_dust_set(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s32 arg3, UNUSED s32 arg4) {
    s32 sp54;
    s16 temp_s1;
    s32 sp4C;
    f32 sp48;
    f32 sp44;

    sp54 = MakeRandomLimmit(0x0168U) - 0xB4;
    sp4C = MakeRandomLimmit(6U);
    temp_s1 = MakeRandomLimmit(0x0060U);
    sp44 = MakeRandomLimmit(6U);
    sp48 = MakeRandomLimmit(2U);
    MakePos(player, &player->spark[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
    MakeStartup(&player->spark[arg1], 6, 1.0f);
    if ((player->slip_flag & THROW_EXPLODE) || ((player->slip_flag) & EXPLODE) ||
        ((player->slip_flag) & ROLLOVER) || ((player->slip_flag) & TERESA)) {
        MakeRDP(&player->spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00A0);
        player->spark[arg1].swork1 -= temp_s1;
        player->spark[arg1].swork2 -= temp_s1;
        player->spark[arg1].swork3 -= temp_s1;
    } else {
        MakeRDP(&player->spark[arg1], RGB32(0x0, 0x0, 0x0), 0x00A0);
        player->spark[arg1].swork1 += temp_s1;
        player->spark[arg1].swork2 += temp_s1;
        player->spark[arg1].swork3 += temp_s1;
    }
    player->spark[arg1].trz = player->position[2] + (cosT(sp54 * DEGREES(1)) * -5.0f);
    player->spark[arg1].trx = player->position[0] + (sinT(sp54 * DEGREES(1)) * -5.0f);
    player->spark[arg1].try = (player->position[1] - player->radius) + sp4C + 2.0f;
    player->spark[arg1].dev = sp44 + 1.0f;
    player->spark[arg1].scale = sp48 + 1.0f;
}

void initburn_dust(Player* player, s16 arg1, s32 arg2, s8 arg3, s8 arg4) {
    if ((arg1 == 0) && ((player->spark[arg2].timer > 0) || (player->spark[arg2].flag == 0))) {
        initburn_dust_set(player, arg1, arg2, (s32) arg3, arg4);
    } else if (player->spark[arg2].timer > 0) {
        initburn_dust_set(player, arg1, arg2, (s32) arg3, arg4);
    }
}

void initwater_dust(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 sp54;
    UNUSED s32 stackPadding0;
    s32 sp4C;
    f32 sp48;
    UNUSED s32 stackPadding1;

    sp54 = MakeRandomLimmit(0x0168U) - 0xB4;
    sp4C = MakeRandomLimmit(6U);
    MakeRandomLimmit(6U);
    sp48 = (f32) MakeRandomLimmit(3U);
    if ((arg1 == 0) && ((player->spark[arg2].timer > 0) || (player->spark[arg2].flag == 0))) {
        MakePos(player, &player->spark[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&player->spark[arg1], 7, 1.0f);
        MakeRDP(&player->spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        player->spark[arg1].trz = player->position[2] + (cosT(sp54 * DEGREES(1)) * -2.0);
        player->spark[arg1].trx = player->position[0] + (sinT(sp54 * DEGREES(1)) * -2.0);
        player->spark[arg1].try = (player->position[1] - player->radius) + sp4C + 2.0f;
        player->spark[arg1].scale = (sp48 + 2.0f) / 10.0f;
    } else if (player->spark[arg2].timer > 0) {
        MakePos(player, &player->spark[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&player->spark[arg1], 7, 1.0f);
        MakeRDP(&player->spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        player->spark[arg1].trz = player->position[2] + (cosT(sp54 * DEGREES(1)) * -2.0);
        player->spark[arg1].trx = player->position[0] + (sinT(sp54 * DEGREES(1)) * -2.0);
        player->spark[arg1].try = (player->position[1] - player->radius) + (f32) sp4C + 2.0f;
        player->spark[arg1].scale = (sp48 + 2.0f) / 10.0f;
    }
}

void inittail_dust(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 test = 2;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    f32 sp20[10] = { (f32) -DEGREES(1), (f32) DEGREES(1), (f32) DEGREES(2), (f32) -DEGREES(2), DEGREES(3), -DEGREES(3), DEGREES(4), -DEGREES(4), DEGREES(5), -DEGREES(5) };

    if (MakeRandomLimmit(3U) == 2.0f) {
        player->spark[arg1].flag = 1;
        player->spark[arg1].trx = player->position[0];
        player->spark[arg1].try = player->position[1] + 2.0f;
        player->spark[arg1].trz = player->position[2];
        player->spark[arg1].ang = -player->direction[1] + sp20[arg1];
        player->spark[arg1].dev = MakeRandomLimmit(3U) + 2.0f;
        player->spark[arg1].type = MakeRandomLimmit(4U);
        player->spark[arg1].type -= test;
        player->spark[arg1].scale = 0.4f;
        player->spark[arg1].number = 2;
        player->spark[arg1].timer = 0;
        player->spark[arg1].swork4 = 0x00FF;
    }
}

void init_slipstream(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    UNUSED s32 stackPadding0;
    s32 var_t0 = 0x000000FF;
    s32 var_t1;
    s32 temp_v1;
    f32 y;
    UNUSED s32 stackPadding1;

    if (1) {};
    temp_v1 = MakeRandomLimmit(8U) & 1;
    if (temp_v1 == 1) {
        var_t1 = 1;
        var_t0 = 0;
        y = player->position[1];
    }
    if (temp_v1 == 0) {
        var_t1 = 0;
        var_t0 = 0;
        y = player->position[1];
    }
    if (var_t0 == 0) {
        if ((arg1 == 0) &&
            ((player->spark[arg2].timer > 0) || (player->spark[arg2].flag == 0))) {
            MakePos(player, &player->spark[arg1], 0.0f, y, 0.0f, (s8) var_t0, (s8) var_t1);
            MakeStartup(&player->spark[arg1], 3, 0.5f);
            MakeRDP(&player->spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0060);
            player->spark[arg1].ang = 0;
            if (player->spark[arg1].lrflag == 1) {
                player->spark[arg1].ang += DEGREES(12);
            } else {
                player->spark[arg1].ang -= DEGREES(12);
            }
            player->spark[arg1].trz =
                player->position[2] +
                (cosT(player->spark[arg1].ang - player->direction[1] - player->slipang) * 5.0f);
            player->spark[arg1].trx =
                player->position[0] +
                (sinT(player->spark[arg1].ang - player->direction[1] - player->slipang) * 5.0f);
        } else if (player->spark[arg2].timer > 0) {
            MakePos(player, &player->spark[arg1], 0.0f, y, 0.0f, (s8) var_t0, (s8) var_t1);
            MakeStartup(&player->spark[arg1], 3, 0.5f);
            MakeRDP(&player->spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0060);
            player->spark[arg1].ang = 0;
            if (player->spark[arg1].lrflag == 1) {
                player->spark[arg1].ang += DEGREES(12);
            } else {
                player->spark[arg1].ang -= DEGREES(12);
            }
            player->spark[arg1].trz =
                player->position[2] +
                (cosT(player->spark[arg1].ang - player->direction[1] - player->slipang) * 5.0f);
            player->spark[arg1].trx =
                player->position[0] +
                (sinT(player->spark[arg1].ang - player->direction[1] - player->slipang) * 5.0f);
        }
    }
}

void init_turbostream(Player* player, s16 arg1, s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 temp_v1;
    s32 phi_t0;
    s32 phi_t1;
    Player* new_var;
    f32 y;
    Player* new_var2;

    phi_t0 = 0xFF;
    temp_v1 = MakeRandomLimmit(8) & 1;
    if (temp_v1 == 1) {
        phi_t1 = 1;
        phi_t0 = 0;
        y = player->position[1];
    }

    if (temp_v1 == 0) {
        phi_t1 = 0;
        if (1) {
            phi_t0 = 0;
        }
        y = player->position[1];
    }

    if (phi_t0 == 0) {
        if ((arg1 == 0) && ((player->spark[arg2].timer > 0) || (player->spark[arg2].flag == 0))) {
            MakePos(player, &player->spark[arg1], 0.0f, y, 0.0f, phi_t0, phi_t1);
            MakeStartup(&player->spark[arg1], 8, 1.0f);
            MakeRDP(&player->spark[arg1], RGB32(0xFF, 0xFF, 0x20), 0xFF);

            player->spark[arg1].ang = 0;
            if (player->spark[arg1].lrflag == 1) {
                player->spark[arg1].ang += DEGREES(12);
            } else {
                player->spark[arg1].ang -= DEGREES(12);
            }

            player->spark[arg1].trz =
                player->position[2] +
                (cosT((player->spark[arg1].ang - player->direction[1]) - player->slipang) * 5.0f);
            player->spark[arg1].trx =
                player->position[0] +
                (sinT((player->spark[arg1].ang - player->direction[1]) - player->slipang) * 5.0f);
            return;
        }

        new_var2 = player;
        if (new_var2->spark[arg2].timer > 0) {
            MakePos(new_var2, &new_var2->spark[arg1], 0.0f, y, 0.0f, phi_t0, phi_t1);
            MakeStartup(&new_var2->spark[arg1], 8, 1.0f);
            MakeRDP(&new_var2->spark[arg1], RGB32(0xFF, 0xFF, 0x20), 0xFF);
            new_var2->spark[arg1].ang = 0;
            if (new_var2->spark[arg1].lrflag == 1) {
                new_var2->spark[arg1].ang += DEGREES(12);
            } else {
                new_var2->spark[arg1].ang -= DEGREES(12);
            }

            new_var = new_var2;
            new_var->spark[arg1].trz =
                new_var->position[2] +
                (cosT((new_var->spark[arg1].ang - new_var->direction[1]) - new_var->slipang) * 5.0f);
            new_var->spark[arg1].trx =
                new_var->position[0] +
                (sinT((new_var->spark[arg1].ang - new_var->direction[1]) - new_var->slipang) * 5.0f);
        }
    }
}

void make_land(Player* player, Particle* arg1, s32 arg2) {
    arg1->flag = 1;
    arg1->try = player->ground + 1.0f;
    arg1->trz = player->position[2];
    arg1->trx = player->position[0];
    arg1->ang = (arg2 * (36 * DEGREES(1))) - player->direction[1];
    arg1->number = 4;
    arg1->timer = 0;
}

void init_land(Player* player, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s8 arg3, UNUSED s8 arg4) {
    s32 var_s1;

    switch (player->bump_status) {
        case DIRT:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                if ((g_courseID == COURSE_CHOCO_MOUNTAIN) || (g_courseID == COURSE_ROYAL_RACEWAY)) {
                    set_up_smoke(&player->spark[var_s1], 1, 0, 0x00A8);
                }
                if (g_courseID == COURSE_KALAMARI_DESERT) {
                    set_up_smoke(&player->spark[var_s1], 7, 0, 0x00A8);
                }
                if (g_courseID == COURSE_MOO_MOO_FARM) {
                    set_up_smoke(&player->spark[var_s1], 8, 0, 0x00A8);
                }
                if (g_courseID == COURSE_WARIO_STADIUM) {
                    set_up_smoke(&player->spark[var_s1], 9, 0, 0x00A8);
                }
                if (g_courseID == COURSE_YOSHI_VALLEY) {
                    set_up_smoke(&player->spark[var_s1], 0x000A, 0, 0x00A8);
                }
                if (g_courseID == COURSE_DK_JUNGLE) {
                    set_up_smoke(&player->spark[var_s1], 0x000B, 0, 0x00A8);
                }
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        case GRASS:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 2, 1, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        case SAND_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 2, 1, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        case SAND:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 3, 1, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        case WET_SAND:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 4, 1, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        case DIRT_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 5, 1, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        case SNOW:
        case SNOW_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 6, 1, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 0, 0, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
        default:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                set_up_smoke(&player->spark[var_s1], 0, 0, 0x00A8);
                make_land(player, &player->spark[var_s1], var_s1);
            }
            player->handling_flag &= ~LANDING_DUST;
            break;
    }
}

void initcrash_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->letter[index].flag = 1;
    player->letter[index].ang = -player->direction[1];
    player->letter[index].number = 2;
    player->letter[index].timer = 0;
    player->letter[index].scale = 0.2f;
}

void inithit_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->letter[index].flag = 1;
    player->letter[index].ang = -player->direction[1];
    player->letter[index].number = 4;
    player->letter[index].timer = 0;
    player->letter[index].scale = 1.0f;
}

void initboing_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->letter[index].flag = 1;
    player->letter[index].ang = -player->direction[1];
    player->letter[index].number = 5;
    player->letter[index].timer = 0;
    player->letter[index].scale = 0.2f;
}

void initpoomp_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->letter[index].flag = 1;
    player->letter[index].ang = -player->direction[1];
    player->letter[index].number = 6;
    player->letter[index].timer = 0;
    player->letter[index].scale = 0.2f;
    player->letter[index].try = 0.0f;
}

void initspin_letter(Player* player, s8 arg1, UNUSED s8 arg2, s8 index) {
    player->letter[index].flag = 1;
    player->letter[index].number = 3;
    player->letter[arg1 /* arg1 instead of index */].scale = 0.2f;
    player->letter[index].timer = 1;
    player->letter[index].ang = 0;
    player->talk &= ~WHIRRR;
    player->letter[index].trz = player->position[2];
    player->letter[index].trx = player->position[0];
    player->letter[index].try = (player->position[1] + 4.0f);
}

void initonpu_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->letter[index].flag = 1;
    player->letter[index].number = 5;
    player->letter[index].scale = 0.1f;
    player->letter[index].timer = 0;
    player->letter[index].try = (player->position[1] + player->radius) - 2.5;
}

void make_smokepos(f32* arg0, f32* arg1, f32* arg2, f32 arg3, f32 arg4, f32 arg5, u16 arg6, u16 arg7) {
    UNUSED f32 pad;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 temp_f20;

    //  Apply the matrix multiplication:
    //  Matrix is the Jacobian for cartesian to spherical coordinates?
    //  Get the change in r, theta, and phi for a change in x, y, z?
    //
    // | arg0 |     | cos(arg6)*cos(arg7)  cos(arg6)*sin(arg7)   -sin(arg6) |     | arg3 |
    // |      |     |                                                       |     |      |
    // | arg1 |  =  | -sin(arg7)           cos(arg7)                 0      |  X  | arg4 |
    // |      |     |                                                       |     |      |
    // | arg2 |     | sin(arg6)*cos(arg7)  sin(arg6)*sin(arg7)    cos(arg6) |     | arg5 |
    //
    sp28 = sinT(arg7);
    sp2C = cosT(arg6);
    sp30 = cosT(arg7);
    temp_f20 = cosT(arg6);
    //    = arg3 * cos(arg6) * cos(arg7) + arg4 * cos(arg6) * sin(arg7) - arg5 * sin(arg6)
    *arg0 = (((arg3 * temp_f20) * sp30) + (arg4 * sp2C) * sp28) - (sinT(arg6) * arg5);

    temp_f20 = sinT(arg7);
    //    = -arg3 * sin(arg7) + arg4 * cos(arg7)
    *arg1 = (cosT(arg7) * arg4) - (arg3 * temp_f20);

    sp28 = sinT(arg7);
    sp2C = sinT(arg6);
    sp30 = cosT(arg7);
    temp_f20 = sinT(arg6);
    //    = arg3 * sin(arg6) * cos(arg7) + arg4 * sin(arg6) * sin(arg7) + arg5 * cos(arg6)
    *arg2 = (cosT(arg6) * arg5) + (((arg3 * temp_f20) * sp30) + ((arg4 * sp2C) * sp28));
}

void engine_smoke(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s32 arg3) {
    f32 sp48[8] = { 4.5f, 4.5f, 4.5f, 4.5f, 4.5f, 5.5f, 4.5f, 6.5f };
    f32 var_f6;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    s16 thing;

    player->gass[arg1].timer++;
    if (player->gass[arg1].timer == 0x000C) {
        player->gass[arg1].flag = 0;
        player->gass[arg1].timer = 0;
        player->gass[arg1].number = NO_PARTICLE;
    }
    player->gass[arg1].dev = 2.0f;
    if (player->gass[arg1].swork5 == 0) {
        player->gass[arg1].scale = player->gass[arg1].scale + 0.07;
        player->gass[arg1].fwork1 = player->gass[arg1].fwork1 + 0.3;
        if (player->gass[arg1].timer >= 3) {
            player->gass[arg1].swork4 -= 3;
        }
        if (player->gass[arg1].swork4 <= 0) {
            player->gass[arg1].swork4 = 0;
        }
    } else {
        player->gass[arg1].scale = player->gass[arg1].scale + 0.1;
        player->gass[arg1].fwork1 = player->gass[arg1].fwork1 + 0.3;
        if (player->gass[arg1].timer >= 3) {
            player->gass[arg1].swork4 -= 2;
        }
        if (player->gass[arg1].swork4 <= 0) {
            player->gass[arg1].swork4 = 0;
        }
    }
    thing = player->gass[arg1].ang - (player->slipang / 2);
    if (player->gass[arg1].swork5 == 0) {
        var_f6 = -((player->taco / 5000.0f) + 0.1);
    } else {
        var_f6 = -((player->taco / 6000.0f) + 0.1);
    }
    if (((player->slip_flag & TURBO) == TURBO) && (player->gass[arg1].timer >= 6)) {
        player->gass[arg1].scale = player->gass[arg1].scale + 0.06;
    }
    player->gass[arg1].lrflag++;
    if (player->gass[arg1].lrflag >= 3) {
        player->gass[arg1].lrflag = 0;
    }
    make_smokepos(&sp40, &sp38, &sp3C, 0.0f, sp48[player->kart],
                  (player->gass[arg1].timer * var_f6) + -5.5, -thing, -player->kartroll * 2);
    player->gass[arg1].trx = player->position[0] + sp40;
    sp38 = (player->position[1] - player->radius) + sp38;
    player->gass[arg1].trz = player->position[2] + sp3C;
    player->gass[arg1].try = player->gass[arg1].fwork1 + sp38;
}

void random_bubble(Player* player, s16 arg1, s8 arg2, UNUSED s8 arg3) {
    f32 temp_f0;

    temp_f0 = player->smoke[arg1].dev / 10.0f;
    ++player->smoke[arg1].timer;
    player->smoke[arg1].try += temp_f0;
    if ((player->jugemu_flag & IS_IN_WATER) == IS_IN_WATER) {
        player->smoke[arg1].try += (temp_f0 + 0.3);
        if ((player->smoke[arg1].timer == 0x10) ||
            ((g_waterlevelPlayer[arg2] - player->smoke[arg1].try) < 3.0f)) {
            player->smoke[arg1].flag = 0;
            player->smoke[arg1].timer = 0;
            player->smoke[arg1].number = NO_PARTICLE;
        }
    } else if ((player->smoke[arg1].timer == 0xA) ||
               ((g_waterlevelPlayer[arg2] - player->smoke[arg1].try) < 3.0f)) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }
}

void dobon_splash(Player* player, s16 arg1, s8 arg2, UNUSED s8 arg3) {
    ++player->gass[arg1].timer;
    player->gass[arg1].trz = player->position[2] + cosT(player->gass[arg1].ang) * -5.8;
    player->gass[arg1].trx = player->position[0] + sinT(player->gass[arg1].ang) * -5.8;
    player->gass[arg1].try = g_waterlevelPlayer[arg2];
    if (player->gass[arg1].timer == 15) {
        player->gass[arg1].flag = 0;
        player->gass[arg1].timer = 0;
        player->gass[arg1].number = NO_PARTICLE;
    }
}

void thunder_effect(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    ++player->gass[arg1].timer;
    if ((s32) player->gass[arg1].timer < 9) {
        if ((player->gass[arg1].timer & 1) != 0) {
            player->gass[arg1].swork1 = 8;
        } else {
            player->gass[arg1].swork1 = 0;
        }
    } else if (((player->gass[arg1].timer & 1) != 0) ||
               ((player->gass[arg1].timer >= 9) && (player->gass[arg1].timer < 12))) {
        player->gass[arg1].swork1 = 0xFF;
    } else if ((player->gass[arg1].timer & 2) != 0) {
        player->gass[arg1].swork1 = 8;
    } else {
        player->gass[arg1].swork1 = 0;
    }
    player->gass[arg1].swork2 = 0;
    player->gass[arg1].swork3 = 0;
    if ((s32) player->gass[arg1].timer >= 0x19) {
        player->gass[arg1].flag = 0;
        player->gass[arg1].timer = 0;
        player->gass[arg1].number = NO_PARTICLE;
    }
}

void teresa_effect(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->gass[arg1].timer >= 0x1E) {
        player->gass[arg1].swork5 += 0x1FFE;
    } else {
        player->gass[arg1].swork5 += 0x1554;
    }

    player->gass[arg1].fwork1 += 0.25;
    player->gass[arg1].trz =
        player->position[2] + (cosT((player->gass[arg1].ang + player->gass[arg1].swork5)) * 5.5);
    player->gass[arg1].trx =
        player->position[0] + (sinT((player->gass[arg1].ang + player->gass[arg1].swork5)) * 5.5);
    player->gass[arg1].try = ((player->position[1] - 5.0f) + player->gass[arg1].fwork1);
    ++player->gass[arg1].timer;
    player->gass[arg1].scale += 0.05;
    player->gass[arg1].swork4 -= 5;

    if ((s32) player->gass[arg1].swork4 <= 0) {
        player->gass[arg1].swork4 = 0;
    }

    if ((s32) player->gass[arg1].timer >= 0x28) {
        player->gass[arg1].flag = 0;
        player->gass[arg1].timer = 0;
        player->gass[arg1].number = NO_PARTICLE;
    }
}

void drift_smoke(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->smoke[arg1].lrflag == 1) {
        player->smoke[arg1].trz =
            player->tire_RL.Position[2] +
            (player->smoke[arg1].timer * -7) * cosT(player->smoke[arg1].ang);
        player->smoke[arg1].trx =
            player->tire_RL.Position[0] +
            (player->smoke[arg1].timer * -7) * sinT(player->smoke[arg1].ang);
    } else {
        player->smoke[arg1].trz =
            player->tire_RR.Position[2] +
            (player->smoke[arg1].timer * -7) * cosT(player->smoke[arg1].ang);
        player->smoke[arg1].trx =
            player->tire_RR.Position[0] +
            (player->smoke[arg1].timer * -7) * sinT(player->smoke[arg1].ang);
    }

    ++player->smoke[arg1].timer;
    player->smoke[arg1].try += 1.0f;

    if (((player->slip_flag & SPIN_L) != 0) || ((player->slip_flag & SPIN_R) != 0)) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
    }

    if (player->smoke[arg1].timer == 8) {
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }

    player->smoke[arg1].scale += 0.08;
    if (player->smoke[arg1].timer >= 4) {
        player->smoke[arg1].swork4 -= 16;
    }

    if (player->smoke[arg1].swork4 <= 0) {
        player->smoke[arg1].swork4 = 0;
    }
}

void random_smoke(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    if (player->smoke[arg1].lrflag == 1) {
        if ((player->slip_flag & THUNDER)) {
            make_smokepos(&sp44, &sp40, &sp3C, -2.0f, 0.0f,
                          (-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 16,
                          -player->smoke[arg1].ang, 2 * -player->kartroll);
            player->smoke[arg1].trx = player->tire_RL.Position[0] + sp44;
            player->smoke[arg1].trz = player->tire_RL.Position[2] + sp3C;
        } else {
            player->smoke[arg1].trz =
                player->tire_RL.Position[2] +
                ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 16) *
                    cosT(player->smoke[arg1].ang);
            player->smoke[arg1].trx =
                player->tire_RL.Position[0] +
                ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 16) *
                    sinT(player->smoke[arg1].ang);
        }
    } else if ((player->slip_flag & THUNDER)) {
        make_smokepos(&sp44, &sp40, &sp3C, 2.0f, 0.0f,
                      (-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 16,
                      -player->smoke[arg1].ang, 2 * -player->kartroll);
        player->smoke[arg1].trx = player->tire_RR.Position[0] + sp44;
        player->smoke[arg1].trz = player->tire_RR.Position[2] + sp3C;
    } else {
        player->smoke[arg1].trz =
            player->tire_RR.Position[2] +
            ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 16) *
                cosT(player->smoke[arg1].ang);
        player->smoke[arg1].trx =
            player->tire_RR.Position[0] +
            ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 16) *
                sinT(player->smoke[arg1].ang);
    }

    ++player->smoke[arg1].timer;
    player->smoke[arg1].try += 0.2;
    if (((player->slip_flag & SPIN_L) != 0) || ((player->slip_flag & SPIN_R) != 0)) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
    }

    if (player->smoke[arg1].timer == 8) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }

    player->smoke[arg1].scale += 0.1;
    if (player->smoke[arg1].swork5 == 0) {
        if (player->smoke[arg1].timer >= 4) {
            player->smoke[arg1].swork4 -= 12;
        }
        if (player->smoke[arg1].swork4 <= 0) {
            player->smoke[arg1].swork4 = 0;
        }
    } else {
        if (player->smoke[arg1].timer >= 4) {
            player->smoke[arg1].swork4 -= 16;
        }
        if (player->smoke[arg1].swork4 <= 0) {
            player->smoke[arg1].swork4 = 0;
        }
    }
}

void rapid_smoke(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->smoke[arg1].lrflag == 1) {
        player->smoke[arg1].trz =
            player->tire_RL.Position[2] +
            (-1.8f * player->smoke[arg1].timer) * cosT(player->smoke[arg1].ang);
        player->smoke[arg1].trx =
            player->tire_RL.Position[0] +
            (-1.8f * player->smoke[arg1].timer) * sinT(player->smoke[arg1].ang);
    } else {
        player->smoke[arg1].trz =
            player->tire_RR.Position[2] +
            (-1.8f * player->smoke[arg1].timer) * cosT(player->smoke[arg1].ang);
        player->smoke[arg1].trx =
            player->tire_RR.Position[0] +
            (-1.8f * player->smoke[arg1].timer) * sinT(player->smoke[arg1].ang);
    }
    ++player->smoke[arg1].timer;
    player->smoke[arg1].try += 0.3;
    if (player->smoke[arg1].timer == 8) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }

    player->smoke[arg1].scale += 0.15;
    if (player->smoke[arg1].swork5 == 0) {
        if ((s32) player->smoke[arg1].timer >= 4) {
            --player->smoke[arg1].swork4;
        }
        if ((s32) player->smoke[arg1].swork4 <= 0) {
            player->smoke[arg1].swork4 = 0;
        }
    } else {
        if ((s32) player->smoke[arg1].timer >= 4) {
            player->smoke[arg1].swork4 -= 16;
        }
        if ((s32) player->smoke[arg1].swork4 <= 0) {

            player->smoke[arg1].swork4 = 0;
        }
    }
}

void random_grass(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->smoke[arg1].lrflag == 1) {
        player->smoke[arg1].trz =
            player->tire_RL.Position[2] +
            (-2 * player->smoke[arg1].timer * cosT(player->smoke[arg1].ang));
        player->smoke[arg1].trx =
            player->tire_RL.Position[0] +
            (-2 * player->smoke[arg1].timer * sinT(player->smoke[arg1].ang));
    } else {
        player->smoke[arg1].trz =
            player->tire_RR.Position[2] +
            (-2 * player->smoke[arg1].timer * cosT(player->smoke[arg1].ang));
        player->smoke[arg1].trx =
            player->tire_RR.Position[0] +
            (-2 * player->smoke[arg1].timer * sinT(player->smoke[arg1].ang));
    }

    ++player->smoke[arg1].timer;
    player->smoke[arg1].try += 0.2;
    if (player->smoke[arg1].timer == 8) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }

    player->smoke[arg1].dev = 2.0f;
    player->smoke[arg1].scale -= 0.06;
}

void random_spl(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    if (player->smoke[arg1].lrflag == 1) {
        player->smoke[arg1].trz =
            player->tire_RL.Position[2] +
            ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                cosT(player->smoke[arg1].ang);
        player->smoke[arg1].trx =
            player->tire_RL.Position[0] +
            ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                sinT(player->smoke[arg1].ang);
    } else {
        player->smoke[arg1].trz =
            player->tire_RR.Position[2] +
            ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                cosT(player->smoke[arg1].ang);
        player->smoke[arg1].trx =
            player->tire_RR.Position[0] +
            ((-player->smoke[arg1].timer * (player->speed / 18.0f) * 216.0f) / 20.0f) *
                sinT(player->smoke[arg1].ang);
    }

    ++player->smoke[arg1].timer;
    if (player->smoke[arg1].timer == 8) {
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }

    player->smoke[arg1].scale += 0.2;
    if (player->smoke[arg1].timer >= 4) {
        player->smoke[arg1].swork4 -= 18;
        player->smoke[arg1].try -= 0.1;
    } else {
        player->smoke[arg1].try += 0.4;
    }

    if (player->smoke[arg1].swork4 <= 0) {
        player->smoke[arg1].swork4 = 0;
    }
}

void random_turbo(Player* player, s16 arg1, UNUSED s32 arg2, UNUSED s32 arg3) {
    f32 sp3C;
    f32 sp38;
    f32 sp34;

    if (player->smoke[arg1].lrflag == 1) {
        make_smokepos(&sp3C, &sp34, &sp38, 3.0f, 0.0f,
                      -5.5 - (player->smoke[arg1].timer * (((player->speed / 18.0f) * 216.0f) / 15.0f)),
                      -player->smoke[arg1].ang, 0);
    } else {
        make_smokepos(&sp3C, &sp34, &sp38, -3.0f, 0.0f,
                      -5.5 - (player->smoke[arg1].timer * (((player->speed / 18.0f) * 216.0f) / 15.0f)),
                      -player->smoke[arg1].ang, 0);
    }
    player->smoke[arg1].trx = player->position[0] + sp3C;
    player->smoke[arg1].trz = player->position[2] + sp38;
    player->smoke[arg1].try = (player->position[1] - player->radius) + sp34;
    player->smoke[arg1].timer++;
    if (player->smoke[arg1].timer == 6) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }
}

void random_ring(Player* player, s16 arg1, s8 arg2, UNUSED s8 arg3) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    sp40 = g_waterlevelPlayer[arg2] - player->position[1] - 3.0f;
    if (((player->water_flag & SUBMERGED) != 0) && (g_courseID != COURSE_KOOPA_BEACH)) {
        sp40 = g_waterlevelPlayer[arg2] - player->position[1] + 0.1;
    }

    make_smokepos(&sp44, &sp40, &sp3C, 0.0f, sp40,
                  -4.0f + ((-player->gass[arg1].timer * (player->speed / 18.0f) * 216.0f) / 10.0f),
                  -player->gass[arg1].ang, 2 * -player->kartroll);
    player->gass[arg1].trx = player->position[0] + sp44;
    player->gass[arg1].trz = player->position[2] + sp3C;
    player->gass[arg1].try = player->position[1] + sp40;
    ++player->gass[arg1].timer;
    if ((player->gass[arg1].timer == 12) || (g_waterlevelPlayer[arg2] <= (player->position[1] - player->radius))) {
        player->gass[arg1].flag = 0;
        player->gass[arg1].timer = 0;
        player->gass[arg1].number = NO_PARTICLE;
    }
    player->gass[arg1].dev = 2.0f;
    player->gass[arg1].scale -= 0.35;
    if (player->gass[arg1].scale < 0.0f) {
        player->gass[arg1].scale = 0.0f;
    }

    player->gass[arg1].swork4 -= 22;
    if (player->gass[arg1].swork4 <= 0) {
        player->gass[arg1].swork4 = 0;
    }
}

void spin_smoke(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    player->smoke[arg1].trz =
        player->position[2] + (-1.2 * player->smoke[arg1].timer * cosT(player->smoke[arg1].ang));
    player->smoke[arg1].trx =
        player->position[0] + (-1.2 * player->smoke[arg1].timer * sinT(player->smoke[arg1].ang));
    player->smoke[arg1].try = player->smoke[arg1].try + 0.5;

    ++player->smoke[arg1].timer;
    if (player->smoke[arg1].timer == 10) {
        player->smoke[arg1].flag = 0;
        player->smoke[arg1].timer = 0;
        player->smoke[arg1].number = NO_PARTICLE;
    }

    player->smoke[arg1].scale += 0.2;
    player->smoke[arg1].swork4 -= 8;
    if (player->smoke[arg1].swork4 <= 0) {
        player->smoke[arg1].swork4 = 0;
    }
}

void star_dust(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 thing2;
    UNUSED s32 stackPadding0;
    s32 thing;
    UNUSED s32 stackPadding1;

    if (player->spark[arg1].timer >= 9) {
        player->spark[arg1].timer = 9;
    }
    thing2 = player->spark[arg1].fwork1;
    thing = player->spark[arg1].timer;
    player->spark[arg1].trz = player->position[2] + (cosT(player->spark[arg1].ang) * (-0.7 * thing));
    player->spark[arg1].trx = player->position[0] + (sinT(player->spark[arg1].ang) * (-0.7 * thing));
    player->spark[arg1].timer++;
    player->spark[arg1].try =
        player->spark[arg1].fwork2 + (f32) ((thing * thing2) - (0.2 * (thing * thing)));
    if (player->spark[arg1].timer == 0x000A) {
        player->spark[arg1].timer = 0;
        player->spark[arg1].flag = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }
    if (player->spark[arg1].timer >= 7) {
        player->spark[arg1].swork4 -= 0x60;
        if (player->spark[arg1].swork4 <= 0) {
            player->spark[arg1].swork4 = 0;
        }
    }
}

void result_star(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 temp_f4;
    UNUSED s32 stackPadding0;
    s32 temp_v1;
    UNUSED s32 stackPadding1;

    temp_v1 = player->spark[arg1].timer;
    temp_f4 = player->spark[arg1].fwork1;
    player->spark[arg1].trz =
        player->position[2] + (cosT(player->spark[arg1].ang) * (-0.6 * temp_v1));
    player->spark[arg1].trx =
        player->position[0] + (sinT(player->spark[arg1].ang) * (-0.6 * temp_v1));
    player->spark[arg1].timer++;
    player->spark[arg1].try =
        player->spark[arg1].fwork2 + (f32) ((temp_v1 * temp_f4) - (0.1 * (temp_v1 * temp_v1)));
    if (player->spark[arg1].timer == 0x0019) {
        player->spark[arg1].timer = 0;
        player->spark[arg1].flag = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }
    if (player->spark[arg1].timer >= 7) {
        player->spark[arg1].swork4 -= 0x6;
        if (player->spark[arg1].swork4 <= 0) {
            player->spark[arg1].swork4 = 0;
        }
    }
}

void land_smoke(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {

    ++player->spark[arg1].timer;
    player->spark[arg1].trz =
        player->position[2] + ((-0.8 * (player->spark[arg1].timer)) * cosT(player->spark[arg1].ang));
    player->spark[arg1].trx =
        player->position[0] + ((-0.8 * (player->spark[arg1].timer)) * sinT(player->spark[arg1].ang));
    player->spark[arg1].try = (player->ground + 2.0f);

    if (player->spark[arg1].timer == 14) {
        player->spark[arg1].flag = 0;
        player->spark[arg1].timer = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }

    player->spark[arg1].swork4 -= 12;
    if (player->spark[arg1].swork4 <= 0) {
        player->spark[arg1].swork4 = 0;
    }
}

void burn_dust(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    ++player->spark[arg1].timer;
    player->spark[arg1].scale -= 0.06;
    player->spark[arg1].try += 0.1;
    player->spark[arg1].swork4 -= 12;

    if (player->spark[arg1].swork4 <= 0) {
        player->spark[arg1].swork4 = 0;
    }

    if (player->spark[arg1].timer == 10) {
        player->spark[arg1].flag = 0;
        player->spark[arg1].timer = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }
}

void water_dust(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    ++player->spark[arg1].timer;
    player->spark[arg1].try -= 0.3;

    if (player->spark[arg1].timer == 10) {
        player->spark[arg1].flag = 0;
        player->spark[arg1].timer = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }
}

void tail_dust(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 temp;
    temp = player->spark[arg1].dev;

    player->spark[arg1].trz =
        player->spark_z + (((-temp) * player->spark[arg1].timer) * cosT(player->spark[arg1].ang));
    player->spark[arg1].trx =
        player->spark_x + (((-temp) * player->spark[arg1].timer) * sinT(player->spark[arg1].ang));
    player->spark[arg1].try = player->position[1] + player->spark[arg1].type;
    player->spark[arg1].scale += 0.04;

    ++player->spark[arg1].timer;
    if (player->spark[arg1].timer == 12) {
        player->spark[arg1].timer = 0;
        player->spark[arg1].flag = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }

    if (player->spark[arg1].timer >= 9) {
        player->spark[arg1].swork4 -= 0x10;
        if (player->spark[arg1].swork4 <= 0) {
            player->spark[arg1].swork4 = 0;
        }
    }
}

void powder_dust(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {
    f32 temp;
    temp = player->spark[arg1].dev * 1.2;

    player->spark[arg1].trz =
        (player->position[2] + (-temp * player->spark[arg1].timer) * (cosT(player->spark[arg1].ang)));
    player->spark[arg1].trx =
        (player->position[0] + (-temp * player->spark[arg1].timer) * (sinT(player->spark[arg1].ang)));
    player->spark[arg1].try += 0.1;

    ++player->spark[arg1].timer;
    if (player->spark[arg1].timer == 10) {
        player->spark[arg1].timer = 0;
        player->spark[arg1].flag = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }

    player->spark[arg1].swork1 += DEGREES(10);
    if (player->spark[arg1].timer >= 6) {
        player->spark[arg1].swork4 -= 16;
        if (player->spark[arg1].swork4 <= 0) {
            player->spark[arg1].swork4 = 0;
        }
    }
}

void slip_stream(Player* player, s16 arg1, UNUSED s8 arg2, UNUSED s8 arg3) {

    if (player->spark[arg1].lrflag == 1) {
        player->spark[arg1].ang += DEGREES(12);
    } else {
        player->spark[arg1].ang -= DEGREES(12);
    }

    player->spark[arg1].trz =
        player->position[2] + (cosT(player->spark[arg1].ang - player->direction[1] - player->slipang) * 5.0f);
    player->spark[arg1].trx =
        player->position[0] + (sinT(player->spark[arg1].ang - player->direction[1] - player->slipang) * 5.0f);
    player->spark[arg1].try = player->position[1] - 1.0f;
    player->spark[arg1].scale += 0.4;
    ++player->spark[arg1].timer;

    if (player->spark[arg1].timer == 10) {
        player->spark[arg1].timer = 0;
        player->spark[arg1].flag = 0;
        player->spark[arg1].number = NO_PARTICLE;
    }
    if (player->spark[arg1].timer >= 5) {
        player->spark[arg1].swork4 -= 20;
        if (player->spark[arg1].swork4 <= 0) {
            player->spark[arg1].swork4 = 0;
        }
    }
}

void crash_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {

    player->letter[index].try = player->position[1];
    ++player->letter[index].timer;

    if (player->letter[index].timer == 9) {
        player->talk &= ~CRASH;
        player->letter[index].flag = 0;
        player->letter[index].timer = 0;
        player->letter[index].number = NO_PARTICLE;
    }

    player->letter[index].scale += 0.8;
    if (player->letter[index].scale >= (f64) 2.5) {
        player->letter[index].scale = 2.5f;
    }
}

void hit_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->letter[index].timer;
    if (player->letter[index].timer < 4) {
        player->letter[index].scale += 1.2;
        if (player->letter[index].scale >= 3.5) {
            player->letter[index].scale = 3.5f;
        }
    } else {
        player->letter[index].scale -= 1.8;
        if (player->letter[index].scale <= 0.0f) {
            player->talk &= ~FLASH;
            player->letter[index].flag = 0;
            player->letter[index].timer = 0;
            player->letter[index].number = NO_PARTICLE;
        }
    }
}

void boing_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->letter[index].timer;
    player->letter[index].scale += 0.15;

    if (1.2 <= player->letter[index].scale) {
        player->letter[index].scale = 1.2f;
    }
    if (player->letter[index].timer >= 12) {
        player->talk &= ~BOING;
        player->letter[index].flag = 0;
        player->letter[index].timer = 0;
        player->letter[index].number = NO_PARTICLE;
    }
}

void poomp_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->letter[index].timer;

    player->letter[index].try += 0.8;
    player->letter[index].scale += 0.4;
    if (player->letter[index].scale >= (f64) 1.5) {
        player->letter[index].scale = 1.5f;
    }

    if (player->letter[index].timer >= 12) {
        player->talk &= ~POOMP;
        player->letter[index].flag = 0;
        player->letter[index].timer = 0;
        player->letter[index].number = NO_PARTICLE;
    }
}

void spin_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    player->letter[index].trz = (f32) player->position[2];
    player->letter[index].trx = (f32) player->position[0];
    player->letter[index].try = (f32) (player->position[1] + 4.0f);
    if ((player->slip_flag & SPIN_L) == SPIN_L) {
        player->letter[index].ang += 26 * DEGREES(1);
    } else {
        player->letter[index].ang -= 26 * DEGREES(1);
    }

    if (((player->slip_flag & SPIN_L) != SPIN_L) &&
        ((player->slip_flag & SPIN_R) != SPIN_R)) {
        player->letter[index].flag = 0;
        player->letter[index].timer = 0;
        player->letter[index].number = NO_PARTICLE;
    }

    player->letter[index].scale += 0.08;
    if (player->letter[index].scale >= 1.5) {
        player->letter[index].scale = 1.5f;
    }
}

void onpu_letter(Player* player, UNUSED s8 arg1, UNUSED s8 arg2, s8 index) {
    ++player->letter[index].timer;
    if (player->letter[index].timer < 8) {
        player->letter[index].scale += 0.2;
        if (1.2 <= player->letter[index].scale) {
            player->letter[index].scale = 1.2f;
        }
    } else {
        player->letter[index].scale -= 0.4;
        if (player->letter[index].scale <= 0.0f) {
            player->talk &= ~MUSIC_NOTE;
            player->letter[index].flag = 0;
            player->letter[index].timer = 0;
            player->letter[index].number = NO_PARTICLE;
        }
    }
}

void common_mtx_calc(Vec3f arg0, Vec3s arg1, f32 scale) {
    Mat4 sp20;

    createaffine(sp20, arg0, arg1);
    scale_matrix(sp20, scale);
    Affine2Mtx(&gDynamicP->mtxEffect[effectcount], sp20);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void set_gass_calc(Player* player, s8 playerIndex, s16 arg2, s8 arg3) {
    Vec3f spB4;
    Vec3s spAC;
    s32 primColors[] = { MAKE_RGB(0xFB, 0xFF, 0xFB), MAKE_RGB(0xFF, 0xFB, 0x86) };
    s32 envColors[] = { MAKE_RGB(0x89, 0x62, 0x8F), MAKE_RGB(0xFE, 0x01, 0x09) };
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;

    if (player->gass[arg2].flag == 1) {
        spB4[0] = player->gass[arg2].trx;
        spB4[1] = player->gass[arg2].try;
        spB4[2] = player->gass[arg2].trz;
        spAC[0] = 0;
        spAC[1] = player->angle_y[arg3];
        spAC[2] = 0;
        if ((player->slip_flag & STAR) &&
            (((s32) g_gameTimer - g_StarUseCounter[playerIndex]) < STAR_EFFECT_DURATION - 1)) {
            primRed = (primColors[1] >> 0x10) & 0xFF;
            primGreen = (primColors[1] >> 0x08) & 0xFF;
            primBlue = (primColors[1] >> 0x00) & 0xFF;
            envRed = (envColors[1] >> 0x10) & 0xFF;
            envGreen = (envColors[1] >> 0x08) & 0xFF;
            envBlue = (envColors[1] >> 0x00) & 0xFF;
            primAlpha = player->gass[arg2].swork4;
            common_mtx_calc(spB4, spAC, ((player->gass[arg2].scale * player->offsetsize) * 1.4));
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_smoke[player->gass[arg2].lrflag],
                                G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        } else {
            primRed = (primColors[player->gass[arg2].swork1] >> 0x10) & 0xFF;
            primGreen = (primColors[player->gass[arg2].swork1] >> 0x08) & 0xFF;
            primBlue = (primColors[player->gass[arg2].swork1] >> 0x00) & 0xFF;
            envRed = (envColors[player->gass[arg2].swork1] >> 0x10) & 0xFF;
            envGreen = (envColors[player->gass[arg2].swork1] >> 0x08) & 0xFF;
            envBlue = (envColors[player->gass[arg2].swork1] >> 0x00) & 0xFF;
            primAlpha = player->gass[arg2].swork4;
            common_mtx_calc(spB4, spAC, player->gass[arg2].scale * player->offsetsize);
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_smoke[player->gass[arg2].lrflag],
                                G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        effectcount += 1;
    }
}

void set_teresa_calc(Player* player, UNUSED s8 playerIndex, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->gass[arg2].flag == 1) {
        red = player->gass[arg2].swork1;
        green = player->gass[arg2].swork2;
        blue = player->gass[arg2].swork3;
        alpha = player->gass[arg2].swork4;
        sp54[0] = player->gass[arg2].trx;
        sp54[1] = player->gass[arg2].try;
        sp54[2] = player->gass[arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = player->angle_y[arg3];
        sp4C[2] = 0;
        common_mtx_calc(sp54, sp4C, player->gass[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        effectcount += 1;
    }
}

void set_drift_calc(Player* player, UNUSED s8 playerIndex, s16 arg2, s8 arg3) {
    Vec3f spB4;
    Vec3s spAC;
    s32 var_s0;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    s32 sp8C[] = { 0x00ffffff, 0x00ffff00, 0x00ff9600 };
    if (player->smoke[arg2].flag == 1) {
        if (player->drift_count >= 50) {
            var_s0 = 1;
        } else {
            var_s0 = 0;
        }
        primRed = player->smoke[arg2].swork1;
        primGreen = player->smoke[arg2].swork2;
        primBlue = player->smoke[arg2].swork3;
        primAlpha = player->smoke[arg2].swork4;
        envRed = (sp8C[player->smoke[arg2].swork5] >> 0x10) & 0xFF;
        envGreen = (sp8C[player->smoke[arg2].swork5] >> 0x08) & 0xFF;
        envBlue = (sp8C[player->smoke[arg2].swork5] >> 0x00) & 0xFF;
        spB4[0] = player->smoke[arg2].trx;
        spB4[1] = player->smoke[arg2].try;
        spB4[2] = player->smoke[arg2].trz;
        spAC[0] = 0;
        spAC[1] = player->angle_y[arg3];
        spAC[2] = 0;
        common_mtx_calc(spB4, spAC, player->smoke[arg2].scale * player->offsetsize);
        if (var_s0 == 0) {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, *D_800E4770[var_s0], G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(gDisplayListHead++, D_0D008DF8);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, *D_800E4770[var_s0], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        effectcount += 1;
    }
}

void set_smoke_calc(Player* player, UNUSED s8 playerIndex, s16 arg2, s8 arg3) {
    Vec3f pos;
    Vec3s spD4;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;

    if ((player->smoke[arg2].flag == 1) && (player->smoke[arg2].timer != 0)) {
        pos[0] = player->smoke[arg2].trx;
        pos[1] = player->smoke[arg2].try;
        pos[2] = player->smoke[arg2].trz;
        spD4[0] = 0;
        spD4[1] = player->angle_y[arg3];
        spD4[2] = 0;
        common_mtx_calc(pos, spD4, player->smoke[arg2].scale * player->offsetsize);
        if ((s32) player->smoke[arg2].type != GRASS) {
            primRed =
                ((NORM_SMOKE_IN[player->smoke[arg2].swork1] >> 0x10) & 0xFF) - player->smoke[arg2].swork2;
            primGreen =
                ((NORM_SMOKE_IN[player->smoke[arg2].swork1] >> 0x08) & 0xFF) - player->smoke[arg2].swork2;
            primBlue =
                ((NORM_SMOKE_IN[player->smoke[arg2].swork1] >> 0x00) & 0xFF) - player->smoke[arg2].swork2;
            envRed = ((NORM_SMOKE_OUT[player->smoke[arg2].swork1] >> 0x10) & 0xFF) - player->smoke[arg2].swork2;
            envGreen =
                ((NORM_SMOKE_OUT[player->smoke[arg2].swork1] >> 0x08) & 0xFF) - player->smoke[arg2].swork2;
            envBlue =
                ((NORM_SMOKE_OUT[player->smoke[arg2].swork1] >> 0x00) & 0xFF) - player->smoke[arg2].swork2;
            primAlpha = player->smoke[arg2].swork4;
            if (player->smoke[arg2].swork5 == 0) {
                gSPDisplayList(gDisplayListHead++, D_0D008DB8);
                gDPLoadTextureBlock(gDisplayListHead++, smoke, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
                Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
                gSPDisplayList(gDisplayListHead++, D_0D008E48);
            } else {
                gSPDisplayList(gDisplayListHead++, D_0D008DB8);
                gDPLoadTextureBlock(gDisplayListHead++, smoke, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
                Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
                gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
                gSPDisplayList(gDisplayListHead++, D_0D008E48);
            }
        } else {
            primRed = player->smoke[arg2].swork1;
            primGreen = player->smoke[arg2].swork2;
            primBlue = player->smoke[arg2].swork3;
            gSPDisplayList(gDisplayListHead++, D_0D008C90);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
            gDPLoadTextureBlock(gDisplayListHead++, grs, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            ToumeIMode(primRed, primGreen, primBlue, 0x000000FF);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
            gSPVertex(gDisplayListHead++, D_800E8C00, 4, 0);
            gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        }
        effectcount += 1;
    }
}

void set_spl_calc(Player* player, UNUSED s8 playerIndex, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->smoke[arg2].flag == 1) {
        red = player->smoke[arg2].swork1;
        green = player->smoke[arg2].swork2;
        blue = player->smoke[arg2].swork3;
        alpha = player->smoke[arg2].swork4;
        sp54[0] = player->smoke[arg2].trx;
        sp54[1] = player->smoke[arg2].try;
        sp54[2] = player->smoke[arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = player->angle_y[arg3];
        sp4C[2] = 0;
        common_mtx_calc(sp54, sp4C, player->smoke[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        effectcount += 1;
    }
}

void set_turbo_calc(Player* player, UNUSED s32 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->smoke[arg2].flag == 1) {
        red = player->smoke[arg2].swork1;
        green = player->smoke[arg2].swork2;
        blue = player->smoke[arg2].swork3;
        alpha = player->smoke[arg2].swork4;
        sp5C[0] = player->smoke[arg2].trx;
        sp5C[1] = player->smoke[arg2].try;
        sp5C[2] = player->smoke[arg2].trz;
        sp54[0] = 0;
        sp54[1] = player->angle_y[arg3];
        sp54[2] = 0;
        common_mtx_calc(sp5C, sp54, player->smoke[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_fire, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(red, green, blue, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8B00, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void set_ring_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->gass[arg2].flag == 1) {
        red = player->gass[arg2].swork1;
        green = player->gass[arg2].swork2;
        blue = player->gass[arg2].swork3;
        alpha = player->gass[arg2].swork4;
        sp54[0] = player->gass[arg2].trx;
        sp54[1] = player->gass[arg2].try;
        sp54[2] = player->gass[arg2].trz;
        sp4C[0] = 0x4000;
        sp4C[1] = player->angle_y[arg3];
        sp4C[2] = 0;
        common_mtx_calc(sp54, sp4C, player->gass[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        effectcount += 1;
    }
}

void set_thunder_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f spDC;
    Vec3s spD4;
    UNUSED s32 stackPadding;

    if ((player->gass[arg2].flag == 1) && (player->gass[arg2].swork1 != 0x00FF)) {

        if (player->bump.distance_zx >= 300.0f) {
            spDC[1] = player->position[1] + 5.0f;
        } else {
            spDC[1] = player->position[1] - 3.0f;
        }
        spDC[2] = player->position[2] + (cosT(player->angle_y[arg3]) * -10.0f);
        spDC[0] = player->position[0] + (sinT(player->angle_y[arg3]) * -10.0f);
        if (player->bump.distance_zx >= 300.0f) {
            spD4[0] = cameras[arg3].camera_direction[0] - DEGREES(90);
        } else {
            spD4[0] = 0;
        }
        spD4[1] = player->angle_y[arg3];
        spD4[2] = 0;
        common_mtx_calc(spDC, spD4, player->gass[arg2].scale * player->offsetsize);
        if (player->gass[arg2].swork1 == 0) {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            gDPLoadTextureBlock(gDisplayListHead++, thunderA, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900[0][player->gass[arg2].swork1], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_square_plain_render);
            gDPLoadTextureBlock(gDisplayListHead++, thunderB, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900[1][player->gass[arg2].swork1], 4, 0);
            gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            gDPLoadTextureBlock(gDisplayListHead++, thunderB, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900[0][player->gass[arg2].swork1], 4, 0);
            gSPDisplayList(gDisplayListHead++, common_square_plain_render);
            gDPLoadTextureBlock(gDisplayListHead++, thunderA, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(gDisplayListHead++, &D_800E8900[1][player->gass[arg2].swork1], 4, 0);
            gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        }
        effectcount += 1;
    }
}

void set_slipstream_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp7C;
    Vec3s sp74;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->spark[arg2].flag == 1) {
        red = player->spark[arg2].swork1;
        green = player->spark[arg2].swork2;
        blue = player->spark[arg2].swork3;
        alpha = player->spark[arg2].swork4;
        sp7C[0] = player->spark[arg2].trx;
        sp7C[1] = player->spark[arg2].try;
        sp7C[2] = player->spark[arg2].trz;
        sp74[0] = -0x071C; // DEGREES(10)?
        sp74[2] = 0;
        if (player->spark[arg2].lrflag == 1) {
            sp74[1] = player->angle_y[arg3] - 0x2000;
            common_mtx_calc(sp7C, sp74, player->spark[arg2].scale * player->offsetsize);
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            ToumeIMode(red, green, blue, alpha);
            gSPDisplayList(gDisplayListHead++, D_0D008E70);
        } else {
            sp74[1] = player->angle_y[arg3] + 0x2000;
            common_mtx_calc(sp7C, sp74, player->spark[arg2].scale * player->offsetsize);
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            ToumeIMode(red, green, blue, alpha & 0xFFFFFFFF); // huh?
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        effectcount += 1;
    }
}

void set_turbostream_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp8C;
    Vec3s sp84;
    UNUSED s32 stackPadding[4];

    if (player->spark[arg2].flag == 1) {
        sp8C[0] = player->spark[arg2].trx;
        sp8C[1] = player->spark[arg2].try;
        sp8C[2] = player->spark[arg2].trz;
        sp84[0] = 0;
        sp84[1] = player->angle_y[arg3];
        sp84[2] = 0;
        common_mtx_calc(sp8C, sp84, player->spark[arg2].scale * player->offsetsize);
        if (player->spark[arg2].lrflag == 1) {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_spark, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(0x000000FF, 0x000000FF, 0x000000DF, 0x000000FF, 0x0000005F, 0, 0x00000060);
            gSPDisplayList(gDisplayListHead++, D_0D008E70);
        } else {
            gSPDisplayList(gDisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_spark, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(0x000000FF, 0x000000FF, 0x000000DF, 0x000000FF, 0x0000005F, 0, 0x00000060);
            gSPDisplayList(gDisplayListHead++, D_0D008E48);
        }
        effectcount += 1;
    }
}

void set_spin_calc(Player* player, UNUSED s8 arg1, f32 arg2, UNUSED s8 arg3, s8 arg4) {
    Vec3f sp9C;
    Vec3s sp94;
    UNUSED s32 stackPadding[2];

    if (player->letter[arg4].flag == 1) {
        sp9C[0] = player->letter[arg4].trx;
        sp9C[1] = player->letter[arg4].try;
        sp9C[2] = player->letter[arg4].trz;
        sp94[0] = 0;
        sp94[1] = player->letter[arg4].ang;
        sp94[2] = 0;
        common_mtx_calc(sp9C, sp94, player->offsetsize * arg2);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gSPClearGeometryMode(gDisplayListHead++, G_CULL_BOTH);
        gDPLoadTLUT_pal256(gDisplayListHead++, letter_pal);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(gDisplayListHead++, whir1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8840, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, whir2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8800, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void set_fukidashi_calc(Player* player, s8 arg1, u8* texture, s8 arg3, f32 arg4, s32 arg5) {
    Vec3f sp7C;
    Vec3s sp74;
    f32 sp54[8] = { 0.0f, -1.2f, 0.1f, 1.2f, -1.7f, -0.8f, -0.2f, -1.9f };
    // ????????????????????????????????????????
    s16 red = ((arg5 >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((arg5 >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((arg5 >> 0x00) & 0xFF) & 0xFF;
    // ????????????????????????????????????????

    if (player->letter[arg3].flag == 1) {
        sp74[0] = 0;
        sp74[1] = player->angle_y[arg1];
        sp74[2] = 0;
        sp7C[0] = player->position[0] + (sinT((0x4000 & 0xFFFFFFFF) - (player->direction[1] + player->slipang)) * arg4);
        sp7C[1] = player->position[1] + player->radius - sp54[player->kart] - 2.0f;
        sp7C[2] = player->position[2] + (cosT((0x4000 & 0xFFFFFFFF) - (player->direction[1] + player->slipang)) * arg4);
        common_mtx_calc(sp7C, sp74, player->letter[arg3].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        SplashMode(red, green, blue, 0x000000FF);
        gSPDisplayList(gDisplayListHead++, D_0D008E20);
        effectcount += 1;
    }
}

void set_word_calc(Player* player, s8 arg1, u8* texture, s8 arg3, f32 arg4, s32 arg5) {
    Vec3f sp7C;
    Vec3s sp74;
    f32 sp54[8] = { -0.7f, -1.9f, -0.6f, 0.4f, -2.5f, -1.6f, -0.95f, -2.7f };
    // ????????????????????????????????????????
    s16 red = ((arg5 >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((arg5 >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((arg5 >> 0x00) & 0xFF) & 0xFF;
    // ????????????????????????????????????????

    if (player->letter[arg3].flag == 1) {
        sp74[0] = 0;
        sp74[1] = player->angle_y[arg1];
        sp74[2] = 0;
        sp7C[0] = player->position[0] + (sinT((0x4000 & 0xFFFFFFFF) - (player->direction[1] + player->slipang)) * arg4);
        sp7C[1] = player->position[1] + player->radius - sp54[player->kart] - 2.0f;
        sp7C[2] = player->position[2] + (cosT((0x4000 & 0xFFFFFFFF) - (player->direction[1] + player->slipang)) * arg4);
        common_mtx_calc(sp7C, sp74, player->letter[arg3].scale * player->offsetsize * 0.8);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, texture, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        SplashMode(red, green, blue, 0x000000FF);
        gSPDisplayList(gDisplayListHead++, D_0D008E20);
        effectcount += 1;
    }
}

void set_crash_calc(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    UNUSED s32 stackPadding[16]; // huh?
    Vec3f sp9C;
    Vec3s sp94;

    if (player->letter[arg4].flag == 1) {
        sp9C[1] = player->letter[arg4].try;
        sp9C[2] = player->position[2] + (cosT(player->angle_y[arg3]) * -10.0f);
        sp9C[0] = player->position[0] + (sinT(player->angle_y[arg3]) * -10.0f);
        sp94[0] = 0;
        sp94[1] = player->angle_y[arg3];
        sp94[2] = 0;
        common_mtx_calc(sp9C, sp94, player->letter[arg4].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gDPLoadTLUT_pal256(gDisplayListHead++, letter_pal);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(gDisplayListHead++, crash1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8880, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, crash2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E88C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void set_hit_calc(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    UNUSED s32 stackPadding[16]; // huh?
    Vec3f sp84;
    Vec3s sp7C;

    if (player->letter[arg4].flag == 1) {
        sp84[1] = player->position[1] - 3.0f;
        sp84[2] = player->position[2] + (cosT(player->angle_y[arg3]) * -10.0f);
        sp84[0] = player->position[0] + (sinT(player->angle_y[arg3]) * -10.0f);
        sp7C[0] = 0;
        sp7C[1] = player->angle_y[arg3];
        sp7C[2] = 0;
        common_mtx_calc(sp84, sp7C, player->letter[arg4].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gDPLoadTextureBlock(gDisplayListHead++, thunderA, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8A00, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, thunderB, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8A40, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void set_boing_calc(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    Vec3f sp64;
    Vec3s sp5C;

    if ((player->letter[arg4].flag == 1) && (player->dir[arg3] < 0xD)) {
        sp64[1] = player->position[1] - 3.0f;
        sp64[2] = player->position[2] + ((-2.5 * player->letter[arg4].timer) * cosT(player->angle_y[arg3]));
        sp64[0] = player->position[0] + ((-2.5 * player->letter[arg4].timer) * sinT(player->angle_y[arg3]));
        sp5C[0] = 0;
        sp5C[1] = player->angle_y[arg3];
        sp5C[2] = 0;
        common_mtx_calc(sp64, sp5C, player->letter[arg4].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, boing, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8B40, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void set_poomp_calc(Player* player, UNUSED s8 arg1, UNUSED f32 arg2, s8 arg3, s8 arg4) {
    Vec3f sp9C;
    Vec3s sp94;

    if ((player->letter[arg4].flag == 1) && ((s32) player->dir[arg3] < 0xD)) {
        sp9C[1] = (player->position[1] - 3.0f) + player->letter[arg4].try;
        sp9C[2] = player->position[2] + (cosT(player->angle_y[arg3]) * -10.0f);
        sp9C[0] = player->position[0] + (sinT(player->angle_y[arg3]) * -10.0f);
        sp94[0] = 0;
        sp94[1] = player->angle_y[arg3];
        sp94[2] = 0;
        common_mtx_calc(sp9C, sp94, player->letter[arg4].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008C90);
        gDPLoadTLUT_pal256(gDisplayListHead++, letter_pal);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(gDisplayListHead++, poomp1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8B80, 4, 0);
        gSPDisplayList(gDisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(gDisplayListHead++, poomp2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(gDisplayListHead++, D_800E8BC0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void set_star_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->spark[arg2].flag == 1) {
        alpha = player->spark[arg2].swork4;
        sp5C[0] = player->spark[arg2].trx;
        sp5C[1] = player->spark[arg2].try;
        sp5C[2] = player->spark[arg2].trz;
        sp54[0] = 0;
        sp54[1] = player->angle_y[arg3];
        player->spark[arg2].swork2 += 0x1C71;
        sp54[2] = player->spark[arg2].swork2;
        common_mtx_calc(sp5C, sp54, player->offsetsize * 0.5);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0xFF, 0xFF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount++;
    }
}

void land_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp74;
    Vec3s sp6C;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    u16 test;
    if (player->spark[arg2].flag == 1) {
        primRed = (NORM_SMOKE_IN[player->spark[arg2].swork1] >> 0x10) & 0xFF;
        primGreen = (NORM_SMOKE_IN[player->spark[arg2].swork1] >> 8) & 0xFF;
        primBlue = NORM_SMOKE_IN[player->spark[arg2].swork1] & 0xFF;

        envRed = (NORM_SMOKE_OUT[player->spark[arg2].swork1] >> 0x10) & 0xFF;
        envGreen = (NORM_SMOKE_OUT[player->spark[arg2].swork1] >> 8) & 0xFF;
        envBlue = NORM_SMOKE_OUT[player->spark[arg2].swork1] & 0xFF;
        primAlpha = player->spark[arg2].swork4;

        sp74[0] = player->spark[arg2].trx;
        sp74[1] = player->spark[arg2].try;
        sp74[2] = player->spark[arg2].trz;
        sp6C[0] = 0;
        sp6C[1] = player->angle_y[arg3];
        sp6C[2] = 0;
        common_mtx_calc(sp74, sp6C, player->offsetsize * 1.5);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, smoke, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        // `test` MUST be a u16
        // `test` MUST be set to `envRed`s value
        // `test` MUST be typecast to s16 when passed as a function parameter
        // What. the. fuck.
        test = envRed;
        Color2Mode(primRed, primGreen, primBlue, (s16) test, envGreen, envBlue, primAlpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        effectcount += 1;
    }
}

void set_startail_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3, f32 arg4) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->spark[arg2].flag == 1) {
        alpha = player->spark[arg2].swork4;
        sp5C[0] = player->spark[arg2].trx;
        sp5C[1] = player->spark[arg2].try;
        sp5C[2] = player->spark[arg2].trz;
        sp54[0] = 0;
        sp54[1] = player->angle_y[arg3];
        sp54[2] = 0;
        common_mtx_calc(sp5C, sp54, player->offsetsize * arg4);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void powder_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 alpha;

    if (player->spark[arg2].flag == 1) {
        alpha = player->spark[arg2].swork4;
        sp5C[0] = player->spark[arg2].trx;
        sp5C[1] = player->spark[arg2].try;
        sp5C[2] = player->spark[arg2].trz;
        sp54[0] = 0;
        sp54[1] = player->angle_y[arg3];
        sp54[2] = player->spark[arg2].swork1;
        common_mtx_calc(sp5C, sp54, player->spark[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0, alpha);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void burn_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 green;
    s16 blue;
    s16 alpha;

    if (player->spark[arg2].flag == 1) {
        red = player->spark[arg2].swork1;
        green = player->spark[arg2].swork2;
        blue = player->spark[arg2].swork3;
        alpha = player->spark[arg2].swork4;
        sp54[0] = player->spark[arg2].trx;
        sp54[1] = player->spark[arg2].try;
        sp54[2] = player->spark[arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = player->angle_y[arg3];
        sp4C[2] = 0;
        common_mtx_calc(sp54, sp4C, player->spark[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, green, blue, alpha);
        gSPDisplayList(gDisplayListHead++, D_0D008E48);
        effectcount += 1;
    }
}

void water_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    UNUSED s32 stackPadding[2];

    if (player->spark[arg2].flag == 1) {
        sp5C[0] = player->spark[arg2].trx;
        sp5C[1] = player->spark[arg2].try;
        sp5C[2] = player->spark[arg2].trz;
        sp54[0] = 0;
        sp54[1] = player->angle_y[arg3];
        sp54[2] = 0;
        common_mtx_calc(sp5C, sp54, player->spark[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, bubble, G_IM_FMT_IA, G_IM_SIZ_16b, 16, 16, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8740, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void set_bubble_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp54;
    Vec3s sp4C;

    if (player->gass[arg2].flag == 1) {
        sp54[0] = player->smoke[arg2].trx;
        sp54[1] = player->smoke[arg2].try;
        sp54[2] = player->smoke[arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = player->angle_y[arg3];
        sp4C[2] = 0;
        common_mtx_calc(sp54, sp4C, player->smoke[arg2].scale * player->offsetsize);

        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(gDisplayListHead++, bubble, G_IM_FMT_IA, G_IM_SIZ_16b, 16, 16, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0xFF, 0xFF, 0xFF, 0xFF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_0D008B78, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount++;
    }
}

void set_dobon_calc(Player* player, UNUSED s8 arg1, s16 arg2, s8 arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 red;
    s16 green;
    s16 blue;

    if (player->gass[arg2].flag == 1) {
        red = player->gass[arg2].swork1;
        green = player->gass[arg2].swork2;
        blue = player->gass[arg2].swork3;
        sp5C[0] = player->gass[arg2].trx;
        sp5C[1] = player->gass[arg2].try;
        sp5C[2] = player->gass[arg2].trz;
        sp54[0] = 0;
        sp54[1] = player->angle_y[arg3];
        sp54[2] = 0;
        common_mtx_calc(sp5C, sp54, player->gass[arg2].scale * player->offsetsize);
        gSPDisplayList(gDisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock_4b(gDisplayListHead++, *D_800E47A0[player->gass[arg2].timer], G_IM_FMT_I, 64, 64,
                               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                               G_TX_NOLOD, G_TX_NOLOD);
        SplashMode(red, green, blue, 0x000000FF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(gDisplayListHead++, D_800E8780, 4, 0);
        gSPDisplayList(gDisplayListHead++, D_0D008DA0);
        effectcount += 1;
    }
}

void BalloonDisp(Player* player, f32 arg1, f32 arg2, s8 playerIndex, s8 balloonIndex, s16 rotation) {
    f32 someX;
    f32 someY;
    f32 someZ;

    gPlayerBalloonStatus[playerIndex][balloonIndex] = BALLOON_STATUS_GONE;
    D_8018D650[playerIndex][balloonIndex] = 0.1f;
    D_8018D6B0[playerIndex][balloonIndex] = 0.0f;
    D_8018D710[playerIndex][balloonIndex] = 0.0f;
    D_8018D770[playerIndex][balloonIndex] = 0;
    D_8018D7A0[playerIndex][balloonIndex] = 0;
    D_8018D7D0[playerIndex][balloonIndex] = 0;
    D_8018D800[playerIndex][balloonIndex] = 5;
    D_8018D830[playerIndex][balloonIndex] = 1;
    D_8018D620[playerIndex][balloonIndex] = -player->direction[1] - player->slipang;
    make_smokepos(&someX, &someY, &someZ, arg1, 4.0f, arg2 + -3.8, -player->direction[1], 0);
    gPlayerBalloonPosX[playerIndex][balloonIndex] = player->position[0] + someX;
    gPlayerBalloonPosZ[playerIndex][balloonIndex] = player->position[2] + someZ;
    gPlayerBalloonPosY[playerIndex][balloonIndex] = (player->position[1] - player->radius) + someY;
    gPlayerBalloonStatus[playerIndex][balloonIndex] |= BALLOON_STATUS_PRESENT;
    gPlayerBalloonRotation[playerIndex][balloonIndex] = rotation; // Sprite rotation
    D_8018D890[playerIndex][balloonIndex] = 0;
}

void balloon_strat(Player* player, f32 arg1, f32 arg2, s8 playerId, s8 balloonId) {
    f32 sp80[] = {
        9.0f, 10.0f, 9.0f, 8.0f, 10.0f, 9.5f, 9.5f, 11.0f,
    };
    UNUSED s32 stackPadding0;
    f32 someX;
    f32 someY;
    f32 someZ;
    f32 sp6C;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;

    sp6C = (-(player->speed / 18.0f) * 216.0f) / 10.0f;
    if ((gPlayerBalloonStatus[playerId][balloonId] & 2) != 2) {
        D_8018D650[playerId][balloonId] += -0.003 + (-player->speed * 0.0006);
        if (D_8018D650[playerId][balloonId] >= 0.05) {
            D_8018D650[playerId][balloonId] = 0.05f;
        }
        if (D_8018D650[playerId][balloonId] <= -0.05) {
            D_8018D650[playerId][balloonId] = -0.05f;
        }
        D_8018D6B0[playerId][balloonId] += D_8018D650[playerId][balloonId];
        if (D_8018D6B0[playerId][balloonId] >= 0.06) {
            D_8018D6B0[playerId][balloonId] = 0.06f;
        }
        if (D_8018D6B0[playerId][balloonId] <= -0.06) {
            D_8018D6B0[playerId][balloonId] = -0.06f;
        }
        D_8018D710[playerId][balloonId] += D_8018D6B0[playerId][balloonId];
        if (D_8018D710[playerId][balloonId] < 0.0f) {
            D_8018D650[playerId][balloonId] = MakeRandomLimmit(0x000BU) / 10;
            D_8018D6B0[playerId][balloonId] = 0.0f;
            D_8018D710[playerId][balloonId] = 0.0f;
        }
        D_8018D620[playerId][balloonId] = -player->direction[1] - player->slipang;
        chase_Snumber(&D_8018D890[playerId][balloonId], player->speed * (f32) DEGREES(1), 0.1f);
    }
    if (D_8018D830[playerId][balloonId] == 1) {
        D_8018D770[playerId][balloonId] += D_8018D800[playerId][balloonId] - player->speed;
    } else {
        D_8018D770[playerId][balloonId] += D_8018D800[playerId][balloonId] + player->speed;
    }
    if (D_8018D770[playerId][balloonId] >= 0xB) {
        D_8018D770[playerId][balloonId] = 0x000B;
    }
    if (D_8018D770[playerId][balloonId] < -0xA) {
        D_8018D770[playerId][balloonId] = -0x000B;
    }
    D_8018D7A0[playerId][balloonId] += D_8018D770[playerId][balloonId];
    if (D_8018D7A0[playerId][balloonId] >= 0x29) {
        D_8018D7A0[playerId][balloonId] = 0x0029;
    }
    if (D_8018D7A0[playerId][balloonId] < -0x28) {
        D_8018D7A0[playerId][balloonId] = -0x0029;
    }
    D_8018D7D0[playerId][balloonId] += D_8018D7A0[playerId][balloonId];
    if (D_8018D7D0[playerId][balloonId] >= 0x38E) { // 0x38E is DEGREES(5) possibly
        D_8018D800[playerId][balloonId] = -MakeRandomLimmit(8U);
        if (D_8018D830[playerId][balloonId] != 1) {
            D_8018D830[playerId][balloonId] = 1;
        }
    }
    if (D_8018D7D0[playerId][balloonId] < -0x38D) {
        D_8018D800[playerId][balloonId] = MakeRandomLimmit(8U);
        if (D_8018D830[playerId][balloonId] != -1) {
            D_8018D830[playerId][balloonId] = -1;
        }
    }
    make_smokepos(&someX, &someY, &someZ, arg1, sp80[player->kart] - D_8018D710[playerId][balloonId],
                  arg2 + -3.2 + (sp6C * 1), -D_8018D620[playerId][balloonId], -player->kartroll * 2);
    if ((gPlayerBalloonStatus[playerId][balloonId] & 2) != 2) {
        gPlayerBalloonPosY[playerId][balloonId] = (player->position[1] - player->radius) + someY;
        gPlayerBalloonPosX[playerId][balloonId] = player->position[0] + someX;
        gPlayerBalloonPosZ[playerId][balloonId] = player->position[2] + someZ;
        gPlayerBalloonDepartingTimer[playerId][balloonId] = 0;
    } else {
        gPlayerBalloonPosY[playerId][balloonId] += 0.2;
        gPlayerBalloonDepartingTimer[playerId][balloonId] += 1;
        chase_Snumber(&D_8018D890[playerId][balloonId], 0, 0.1f);
        chase_Snumber(&gPlayerBalloonRotation[playerId][balloonId], 0, 0.1f);
        if (gPlayerBalloonDepartingTimer[playerId][balloonId] >= 0x78) {
            kill_balloon((s32) player, playerId, balloonId);
        }
    }
}

void make_balloon_calc(Player* player, s8 playerIndex, s16 balloonIndex, s8 screenId) {
    Mat4 sp140;
    Vec3f sp134;
    Vec3s sp12C;
    UNUSED s16 stackPadding;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    s16 temp_t1;
    f32 xdiff;
    f32 zdiff;
    f32 var_f20;
    s32 primColors[] = {
        MAKE_RGB(0xC8, 0x01, 0x00), MAKE_RGB(0x00, 0x70, 0x01), MAKE_RGB(0x10, 0x79, 0x51), MAKE_RGB(0x00, 0x59, 0x70),
        MAKE_RGB(0x70, 0x55, 0x00), MAKE_RGB(0x7A, 0x7E, 0x00), MAKE_RGB(0x77, 0x2C, 0x24), MAKE_RGB(0x30, 0x14, 0x58),
    };
    s32 envColors[] = {
        MAKE_RGB(0xDC, 0x00, 0x00), MAKE_RGB(0x00, 0x8C, 0x06), MAKE_RGB(0x00, 0x00, 0x51), MAKE_RGB(0x00, 0x00, 0x00),
        MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00),
    };

    primRed = (primColors[player->kart] >> 0x10) & 0xFF;
    primGreen = (primColors[player->kart] >> 0x08) & 0xFF;
    primBlue = (primColors[player->kart] >> 0x00) & 0xFF;
    envRed = (envColors[player->kart] >> 0x10) & 0xFF;
    envGreen = (envColors[player->kart] >> 0x08) & 0xFF;
    envBlue = (envColors[player->kart] >> 0x00) & 0xFF;
    temp_t1 = (((player->angle_y[screenId] + player->direction[1] + player->slipang) & 0xFFFF) / 128);
    temp_t1 <<= 7;
    if (screenId == playerIndex) {
        var_f20 = 0.3f;
    } else {
        // wut?
        xdiff = (var_f20 = player->position[0] - cameras[screenId].camera_pos[0]);
        zdiff = player->position[2] - cameras[screenId].camera_pos[2];
        if (g_ScreenSplitA != 3) {
            var_f20 = sqrtf((xdiff * xdiff) + (zdiff * zdiff)) / 300.0f;
        } else {
            var_f20 = sqrtf((xdiff * xdiff) + (zdiff * zdiff)) / 200.0f;
        }
        if (var_f20 >= 1.8) {
            var_f20 = 1.8f;
        }
        if (var_f20 <= 0.3) {
            var_f20 = 0.3f;
        }
    }
    sp134[0] = gPlayerBalloonPosX[playerIndex][balloonIndex];
    sp134[1] = gPlayerBalloonPosY[playerIndex][balloonIndex];
    sp134[2] = gPlayerBalloonPosZ[playerIndex][balloonIndex];
    sp12C[0] = -((D_8018D890[playerIndex][balloonIndex] * 4) * cosT(temp_t1));
    sp12C[1] = player->angle_y[screenId];
    sp12C[2] = D_8018D7D0[playerIndex][balloonIndex] -
               (gPlayerBalloonRotation[playerIndex][balloonIndex] * cosT(temp_t1)) -
               ((D_8018D890[playerIndex][balloonIndex] * 8) * sinT(temp_t1));
    createaffine(sp140, sp134, sp12C);
    scale_matrix(sp140, var_f20);
    Affine2Mtx(&gDynamicP->mtxEffect[effectcount], sp140);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTLUT_pal256(gDisplayListHead++, letter_pal);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);

    IceMode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, 0x000000D8);

    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gDPLoadTextureBlock(gDisplayListHead++, balloon1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane1, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gDPLoadTextureBlock(gDisplayListHead++, balloon2 - 0x40, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane2, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    effectcount++;
}

void init_all_player_balloons(Player* player, s8 playerIndex) {
    BalloonDisp(player, 0.0f, 0.0f, playerIndex, (s8) 0, (s16) 0);
    BalloonCheck(player, 1.5f, 2.0f, playerIndex, (s8) 1, (s16) 0x1C70);
    BalloonCheck(player, -1.5f, 2.0f, playerIndex, (s8) 2, (s16) -0x1C70);
    BalloonCount[playerIndex] = 2;
}

void allkill_balloon(UNUSED Player* player, s8 playerIndex) {
    gPlayerBalloonStatus[playerIndex][0] = BALLOON_STATUS_GONE;
    gPlayerBalloonStatus[playerIndex][1] = BALLOON_STATUS_GONE;
    gPlayerBalloonStatus[playerIndex][2] = BALLOON_STATUS_GONE;
}

void cut_balloon(Player* player, s8 playerIndex) {
    if (BalloonCount[playerIndex] >= 0) {
        gPlayerBalloonStatus[playerIndex][BalloonCount[playerIndex]] &= ~BALLOON_STATUS_PRESENT;
        gPlayerBalloonStatus[playerIndex][BalloonCount[playerIndex]] |= BALLOON_STATUS_DEPARTING;
        BalloonCount[playerIndex]--;
        NAPlyTrgStart(playerIndex, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x51));
        if (BalloonCount[playerIndex] < 0) {
            set_change_bomb(player, playerIndex);
        }
    }
}

void kill_balloon(UNUSED s32 arg0, s8 playerIndex, s8 balloonIndex) {
    if (BalloonCount[playerIndex] >= 0) {
        gPlayerBalloonStatus[playerIndex][balloonIndex] = BALLOON_STATUS_GONE;
    }
}

void balloonset(Player* player, s8 playerId) {
    if (gPlayerBalloonStatus[playerId][0] != BALLOON_STATUS_GONE) {
        balloon_strat(player, 0.0f, 0.0f, playerId, 0);
    }

    if (gPlayerBalloonStatus[playerId][1] != BALLOON_STATUS_GONE) {
        balloon_strat(player, 1.8f, 2.6f, playerId, 1);
    }

    if (gPlayerBalloonStatus[playerId][2] != BALLOON_STATUS_GONE) {
        balloon_strat(player, -1.8f, 2.6f, playerId, 2);
    }
}

void set_balloon_calc(Player* player, s8 playerIndex, s8 screenId) {
    if (gPlayerBalloonStatus[playerIndex][0] != BALLOON_STATUS_GONE) {
        make_balloon_calc(player, playerIndex, 0, screenId);
    }
    if (gPlayerBalloonStatus[playerIndex][1] != BALLOON_STATUS_GONE) {
        make_balloon_calc(player, playerIndex, 1, screenId);
    }
    if (gPlayerBalloonStatus[playerIndex][2] != BALLOON_STATUS_GONE) {
        make_balloon_calc(player, playerIndex, 2, screenId);
    }
}

/**
 * Used in podium ceremony.
 */
void draw_balloon(Vec3f arg0, f32 arg1, s16 arg2, s16 arg3) {
    Mat4 sp108;
    Vec3f spFC;
    Vec3s spF4;
    UNUSED s16 stackPadding;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;
    s32 primColors[] = {
        MAKE_RGB(0xC8, 0x01, 0x00), MAKE_RGB(0x00, 0x70, 0x01), MAKE_RGB(0x10, 0x79, 0x51), MAKE_RGB(0x00, 0x59, 0x70),
        MAKE_RGB(0x70, 0x55, 0x00), MAKE_RGB(0x7A, 0x7E, 0x00), MAKE_RGB(0x77, 0x2C, 0x24), MAKE_RGB(0x30, 0x14, 0x58),
    };
    s32 envColors[] = {
        MAKE_RGB(0xDC, 0x00, 0x00), MAKE_RGB(0x00, 0x8C, 0x06), MAKE_RGB(0x00, 0x00, 0x51), MAKE_RGB(0x00, 0x00, 0x00),
        MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00), MAKE_RGB(0x00, 0x00, 0x00),
    };

    primRed = (primColors[arg3] >> 0x10) & 0xFF;
    primGreen = (primColors[arg3] >> 0x08) & 0xFF;
    primBlue = (primColors[arg3] >> 0x00) & 0xFF;
    envRed = (envColors[arg3] >> 0x10) & 0xFF;
    envGreen = (envColors[arg3] >> 0x08) & 0xFF;
    envBlue = (envColors[arg3] >> 0x00) & 0xFF;
    spFC[0] = arg0[0];
    spFC[1] = arg0[1];
    spFC[2] = arg0[2];
    spF4[0] = 0;
    spF4[1] = camera1->camera_direction[1];
    spF4[2] = arg2;
    createaffine(sp108, spFC, spF4);
    scale_matrix(sp108, arg1);
    Affine2Mtx(&gDynamicP->mtxEffect[effectcount], sp108);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTLUT_pal256(gDisplayListHead++, letter_pal);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    IceMode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, 0x000000D8);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gDPLoadTextureBlock(gDisplayListHead++, balloon1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane1, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    // balloon2 is correct. But interestingly, IDO seems to set "-0x40" to a different register so the texture still
    // looks fine.
    gDPLoadTextureBlock(gDisplayListHead++, balloon2 - 0x40, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, gBalloonVertexPlane2, 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    effectcount += 1;
}

void draw_smoke(Vec3f arg0, f32 arg1, s32 rgb, s16 alpha) {
    Vec3f sp4C;
    Vec3s sp44;
    s16 red;
    s16 green;
    s16 blue;

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = camera1->camera_direction[1];
    sp44[2] = 0;
    common_mtx_calc(sp4C, sp44, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(gDisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    // ???????????????????????????????????
    red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????
    ToumeIMode(red, green, blue, alpha);
    gSPDisplayList(gDisplayListHead++, D_0D008E48);
    effectcount += 1;
}

void draw_star(Vec3f arg0, f32 arg1, s32 rgb, s16 alpha) {
    Vec3f sp5C;
    Vec3s sp54;
    // ???????????????????????????????????????
    s16 red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????????

    sp5C[0] = arg0[0];
    sp5C[1] = arg0[1];
    sp5C[2] = arg0[2];
    sp54[0] = 0;
    sp54[1] = camera1->camera_direction[1];
    sp54[2] = 0;
    common_mtx_calc(sp5C, sp54, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(gDisplayListHead++, hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    ToumeIMode(red, green, blue, alpha);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, D_800E87C0, 4, 0);
    gSPDisplayList(gDisplayListHead++, D_0D008DA0);
    effectcount += 1;
}

void draw_pachipachi(Vec3f arg0, f32 arg1, s32 rgb, s16 alpha, s16 arg4) {
    Vec3f sp4C;
    Vec3s sp44;
    // ???????????????????????????????????????
    s16 red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    s16 green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    s16 blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????????

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = camera1->camera_direction[1];
    sp44[2] = 0;
    common_mtx_calc(sp4C, sp44, arg1);
    gSPDisplayList(gDisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(gDisplayListHead++, common_texture_particle_spark[arg4], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    SplashMode(red, green, blue, alpha);
    gSPDisplayList(gDisplayListHead++, D_0D008E48);
    effectcount += 1;
}

void smokeset(Player* player, s16 particleIndex, s8 playerId, s8 arg3) {
    s8 playerIdCopy = playerId;
    s32 sp28;

    sp28 = particleIndex - 1;
    if (sp28 < 0) {
        sp28 = 9;
    }
    if (player->smoke[particleIndex].flag == 1) {
        switch (player->smoke[particleIndex].number) {
            case 1:
                drift_smoke(player, particleIndex, playerIdCopy, arg3);
                break;
            case 2:
                random_smoke(player, particleIndex, playerIdCopy, arg3);
                break;
            case 3:
                random_grass(player, particleIndex, playerIdCopy, arg3);
                break;
            case 4:
                spin_smoke(player, particleIndex, playerIdCopy, arg3);
                break;
            case 5:
                rapid_smoke(player, particleIndex, playerIdCopy, arg3);
                break;
            case 9:
                random_spl(player, particleIndex, playerIdCopy, arg3);
                break;
            case 11:
                random_bubble(player, particleIndex, playerIdCopy, arg3);
                break;
            default:
                break;
        }
    } else {
        if (player->water_flag & SUBMERGED) {
            init_bubble(player, particleIndex, sp28, playerIdCopy, arg3);
        } else if (!(player->slip_flag & N_JUMP) && !(player->slip_flag & D_JUMP)) {
            if (((player->slip_flag & DRIFT) == DRIFT) &&
                ((player->flag & IS_PLAYER) == IS_PLAYER)) {
                initdrift_smoke(player, particleIndex, sp28, playerIdCopy, arg3);
            } else if (((f64) (g_waterlevelPlayer[playerIdCopy] - player->tire_RR.Height) >= 3.5) ||
                       ((f64) (g_waterlevelPlayer[playerIdCopy] - player->tire_RL.Height) >= 3.5)) {
                initrnd_spl(player, particleIndex, sp28, playerIdCopy, arg3);
            } else if (((player->slip_flag & SPIN_L) == SPIN_L) ||
                       ((player->slip_flag & SPIN_R) == SPIN_R)) {
                InitSpinSmokeHook(player, particleIndex, sp28, playerIdCopy, arg3);
            } else if (((player->slip_flag & RAPID_ACC) && !(player->flag & NO_CONTROLS)) ||
                       (player->slip_flag & WHEELSPIN) || (player->slip_flag & SPIN_TURN) ||
                       (player->handling_flag & SPINOUT_SWERVE)) {
                InitRapidSmokeHook(player, particleIndex, sp28, playerIdCopy, arg3);
            } else {
                InitRndSmokeHook(player, particleIndex, sp28, playerIdCopy, arg3);
            }
        }
    }
}

void sparkset(Player* player, s16 arg1, s8 playerIndex, s8 arg3) {
    UNUSED s32 stackPadding;
    s32 sp28;
    sp28 = arg1 - 1;
    if (sp28 < 0) {
        sp28 = 9;
    }
    if (player->spark[arg1].flag == 1) {
        switch (player->spark[arg1].number) {
            case 1:
                star_dust(player, arg1, playerIndex, arg3);
                break;

            case 2:
                tail_dust(player, arg1, playerIndex, arg3);
                break;

            case 3:
                slip_stream(player, arg1, playerIndex, arg3);
                break;

            case 4:
                land_smoke(player, arg1, playerIndex, arg3);
                break;

            case 5:
                powder_dust(player, arg1, playerIndex, arg3);
                break;

            case 6:
                burn_dust(player, arg1, playerIndex, arg3);
                break;

            case 7:
                water_dust(player, arg1, playerIndex, arg3);
                break;

            case 8:
                slip_stream(player, arg1, playerIndex, arg3);
                break;

            case 9:
                result_star(player, arg1, playerIndex, arg3);
                break;

            default:
                break;
        }
    } else {
        if (player->handling_flag & TROPHYJUMP_STARS) {
            initresult_star(player, arg1, sp28, playerIndex, arg3);
            player->handling_flag &= ~LANDING_DUST;
            return;
        }
        if (((((player->jugemu_flag & LAVA_EFFECT) == LAVA_EFFECT) ||
              ((player->bomb_timer < 2) && (player->slip_flag & EXPLODE))) ||
             ((player->bomb_timer < 2) && (player->slip_flag & THROW_EXPLODE))) ||
            (player->slip_flag & ROLLOVER)) {
            initburn_dust(player, arg1, sp28, playerIndex, arg3);
            player->hit_flag &= ~0x0008;
            player->handling_flag &= ~LANDING_DUST;
            return;
        }
        if ((player->jugemu_flag & WATER_EFFECT) == WATER_EFFECT) {
            initwater_dust(player, arg1, sp28, playerIndex, arg3);
            player->hit_flag &= ~0x0008;
            player->handling_flag &= ~LANDING_DUST;
            return;
        }
        if ((player->slip_flag & STAR) &&
            ((((s32) g_gameTimer) - g_StarUseCounter[playerIndex]) < STAR_EFFECT_DURATION - 1)) {
            initpowder_dust(player, arg1, sp28, playerIndex, arg3);
            player->hit_flag &= ~0x0008;
            player->handling_flag &= ~LANDING_DUST;
            return;
        }
        if ((player->hit_flag & 8) == 8) {
            initstar_dust(player, arg1, sp28, playerIndex, arg3);
            player->handling_flag &= ~LANDING_DUST;
            return;
        }
        if (((player->hit_flag & 0x20) == 0x20) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            inittail_dust(player, arg1, sp28, playerIndex, arg3);
            player->hit_flag &= ~0x0008;
            player->handling_flag &= ~LANDING_DUST;
            return;
        }
        if ((player->slip_flag & TURBO) && (player->flag & IS_PLAYER)) {
            init_turbostream(player, arg1, sp28, playerIndex, arg3);
            return;
        }
        if (((player->slip_flag & SLIP_STREAM) || (player->slip_flag & DASH_JUMP)) &&
            ((player->flag & IS_PLAYER) == IS_PLAYER)) {
            init_slipstream(player, arg1, sp28, playerIndex, arg3);
            player->hit_flag &= ~0x0008;
            player->handling_flag &= ~LANDING_DUST;
            return;
        }
        if ((player->handling_flag & LANDING_DUST) == LANDING_DUST) {
            init_land(player, arg1, sp28, playerIndex, arg3);
            player->hit_flag &= ~0x0008;
        }
    }
}

void gassset(Player* arg0, s16 arg1, s8 playerId, s8 arg3) {
    UNUSED u16 temp_v0_3;
    s32 sp20 = arg1;
    if (--sp20 < 0) {
        sp20 = 9;
    }
    // Spawn particles when oob    
    if (arg0->gass[arg1].flag == 1) {
        switch (arg0->gass[arg1].number) {
            case 1:
                engine_smoke(arg0, arg1, playerId, arg3);
                break;
            case 3:
                random_ring(arg0, arg1, playerId, arg3);
                break;
            case 5:
                dobon_splash(arg0, arg1, playerId, arg3);
                break;
            case 6:
                thunder_effect(arg0, arg1, playerId, arg3);
                break;
            case 7:
                teresa_effect(arg0, arg1, playerId, arg3);
                break;
        }
    } else {
        if ((arg0->handling_flag & BOO_SMOKE_EFFECT) && (arg0->flag & SPINOUT_SWERVE)) {
            init_teresa(arg0, arg1, sp20, playerId, arg3);
            return;
        } else if (((arg0->slip_flag & THUNDER) == THUNDER) && (arg0->thunder_timer < 0x32)) {
            init_thunder(arg0, arg1, sp20, playerId, arg3);
            return;
        } else if ((arg0->flag & IS_PLAYER) == IS_PLAYER) {
            if ((arg0->water_flag & SPLASH_DIVE) == SPLASH_DIVE) {
                init_dobon(arg0, arg1, sp20, playerId, arg3);
                return;
            } else if ((arg0->water_flag & HALF_SUBMERGED) || (arg0->water_flag & SUBMERGED)) {
                init_ring(arg0, arg1, sp20, playerId, arg3);
                return;
            }
        }
        switch (g_ScreenSplitA) {
            case SCREEN_MODE_1P:
                if (((arg0->slip_flag & BROKEN) != BROKEN) &&
                    ((arg0->slip_flag & ROLLOVER) != ROLLOVER) &&
                    ((arg0->slip_flag & EXPLODE) != EXPLODE)) {
                    if (((arg0->jugemu_flag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
                        ((arg0->jugemu_flag & IS_FROZEN) != IS_FROZEN) &&
                        !(arg0->jugemu_flag & LAKITU_CAMERA)) {
                        initengine_smoke(arg0, arg1, sp20, playerId, arg3);
                    }
                }
                break;
            default:
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (((arg0->flag & IS_PLAYER) != 0) && ((arg0->slip_flag & BROKEN) != BROKEN) &&
                    ((arg0->slip_flag & ROLLOVER) != ROLLOVER) &&
                    ((arg0->slip_flag & EXPLODE) != EXPLODE)) {
                    if (((arg0->jugemu_flag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
                        ((arg0->jugemu_flag & IS_FROZEN) != IS_FROZEN) &&
                        !(arg0->jugemu_flag & LAKITU_CAMERA)) {
                        initengine_smoke(arg0, arg1, sp20, playerId, arg3);
                    }
                }
            break;
        }
    }
}

void letterset(Player* player, s8 playerIndex, s8 arg2) {
    if (player->letter[0].flag == 1) {
        switch (player->letter[0].number) {
            case 2:
                crash_letter(player, playerIndex, arg2, 0);
                break;
            case 3:
                spin_letter(player, playerIndex, arg2, 0);
                break;
            case 4:
                hit_letter(player, playerIndex, arg2, 0);
                break;
            case 5:
                boing_letter(player, playerIndex, arg2, 0);
                break;
            case 6:
                poomp_letter(player, playerIndex, arg2, 0);
                break;
        }
    } else {
        if ((player->talk & CRASH) == CRASH) {
            initcrash_letter(player, playerIndex, arg2, 0);
        }
        if ((player->talk & BOING) == BOING) {
            initboing_letter(player, playerIndex, arg2, 0);
        }
        if ((player->talk & FLASH) == FLASH) {
            inithit_letter(player, playerIndex, arg2, 0);
        }
        if ((player->talk & WHIRRR) == WHIRRR) {
            initspin_letter(player, playerIndex, arg2, 0);
        }
        if ((player->talk & POOMP) == POOMP) {
            initpoomp_letter(player, playerIndex, arg2, 0);
        }
    }
    if (player->letter[1].flag == 1) {
        if (player->letter[1].number == 5) {
            onpu_letter(player, playerIndex, arg2, 1);
        }
    } else if ((player->talk & MUSIC_NOTE) == MUSIC_NOTE) {
        initonpu_letter(player, playerIndex, arg2, 1);
    }
}

void OBJCalculation(Player* player, s8 playerId, s8 screenId) {
    s16 var_s2;
    if ((player->status & (P1_DRAW_PARTICLES << (screenId * 4))) == (P1_DRAW_PARTICLES << (screenId * 4))) {
        for (var_s2 = 0; var_s2 < 10; var_s2++) {
            switch (player->gass[var_s2].number) {
                case 1:
                    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            set_gass_calc(player, playerId, var_s2, screenId);
                        }
                    } else {
                        set_gass_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case 6:
                    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            set_thunder_calc(player, playerId, var_s2, screenId);
                        }
                    } else if (screenId == playerId) {
                        set_thunder_calc(player, playerId, var_s2, screenId);
                    }
                    break;
            }
            switch (player->spark[var_s2].number) {
                case 1:
                case 9:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        set_star_calc(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        set_star_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case 2:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        set_startail_calc(player, playerId, var_s2, screenId, player->spark[var_s2].scale);
                    } else if (screenId == playerId) {
                        set_startail_calc(player, playerId, var_s2, screenId, player->spark[var_s2].scale);
                    }
                    break;
                case 3:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        set_slipstream_calc(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        set_slipstream_calc(player, (s32) playerId, var_s2, screenId);
                    }
                    break;
                case 4:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        land_calc(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        land_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case 5:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        powder_calc(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        powder_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case 6:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        burn_calc(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        burn_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case 7:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        water_calc(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        water_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case 8:
                    if (g_ScreenSplitA == SCREEN_MODE_1P) {
                        set_turbostream_calc(player, playerId, var_s2, screenId);
                    } else if (screenId == playerId) {
                        set_turbostream_calc(player, playerId, var_s2, screenId);
                    }
                    break;
            }
            switch (player->smoke[var_s2].number) {
                case DRIFT_PARTICLE:
                    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            set_drift_calc(player, playerId, var_s2, screenId);
                        }
                    } else {
                        set_drift_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case GROUND_PARTICLE:
                case GRASS_PARTICLE:
                case 4:
                case 5:
                    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            set_smoke_calc(player, playerId, var_s2, screenId);
                        }
                    } else {
                        set_smoke_calc(player, playerId, var_s2, screenId);
                    }
                    break;
                case 9:
                    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            set_spl_calc(player, (s32) playerId, var_s2, screenId);
                        }
                    } else {
                        set_spl_calc(player, (s32) playerId, var_s2, screenId);
                    }
                    break;
                case 11:
                    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == playerId) {
                            set_bubble_calc(player, playerId, var_s2, screenId);
                        }
                    } else if (screenId == playerId) {
                        set_bubble_calc(player, playerId, var_s2, screenId);
                    }
                    break;
            }
        }
    }
    if ((g_gameMode == BATTLE) && (player->status & (P1_DRAW_SPRITE << (screenId * 4)))) {
        set_balloon_calc(player, playerId, screenId);
    }
}

void exce_obj_calculation(Player* player, s8 playerIndex, s8 screenId) {
    s16 i;
    s32 bitwiseMask;

    bitwiseMask = P1_DRAW_PARTICLES << (screenId * 4);
    if (bitwiseMask == (player->status & bitwiseMask)) {
        for (i = 0; i < 10; i++) {
            if (player->gass[i].number == 7) {
                set_teresa_calc(player, playerIndex, i, screenId);
            }
        }
    }
}

void obj_calculation1(Player* arg0, s8 arg1, s8 arg2) {
    s16 temp_s0;
    s32 temp_v0;

    temp_v0 = P1_DRAW_PARTICLES << (arg2 * 4);
    if (temp_v0 == (arg0->status & temp_v0)) {
        for (temp_s0 = 0; temp_s0 < 10; ++temp_s0) {
            temp_v0 = arg0->gass[temp_s0].number;
            if (temp_v0 != 3) {
                if (temp_v0 == 5) {
                    set_dobon_calc(arg0, arg1, temp_s0, arg2);
                }
            } else if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                if (arg2 == arg1) {
                    set_ring_calc(arg0, arg1, temp_s0, arg2);
                }
            } else {
                set_ring_calc(arg0, arg1, temp_s0, arg2);
            }
        }

        if (((arg0->flag & IS_PLAYER) == IS_PLAYER) && (arg2 == arg1)) {
            switch (arg0->letter[0].number) {
                case 2:
                    set_crash_calc(arg0, arg1, arg0->letter[0].scale, arg2, 0);
                    break;
                case 3:
                    set_spin_calc(arg0, arg1, arg0->letter[0].scale, arg2, 0);
                    break;
                case 4:
                    set_hit_calc(arg0, arg1, arg0->letter[0].scale, arg2, 0);
                    break;
                case 5:
                    set_boing_calc(arg0, arg1, arg0->letter[0].scale, arg2, 0);
                    break;
                case 6:
                    set_poomp_calc(arg0, arg1, arg0->letter[0].scale, arg2, 0);
                    break;
            }
            if (arg0->letter[1].number == 5) {
                set_fukidashi_calc(arg0, arg2, fukidashi2, 1, 1.6f, 0xFFFFFF);
                set_word_calc(arg0, arg2, onpu, 1, 1.6f, 0xFF);
            }
        }
    }
}

void kart_effect_strategy(void) {
    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            switch (g_gameMode) {
                case GRAND_PRIX:
                    effect_strategy(gPlayerOne, PLAYER_ONE, 0);
                    effect_strategy(gPlayerTwo, PLAYER_TWO, 0);
                    effect_strategy(gPlayerThree, PLAYER_THREE, 0);
                    effect_strategy(gPlayerFour, PLAYER_FOUR, 0);
                    effect_strategy(gPlayerFive, PLAYER_FIVE, 0);
                    effect_strategy(gPlayerSix, PLAYER_SIX, 0);
                    effect_strategy(gPlayerSeven, PLAYER_SEVEN, 0);
                    effect_strategy(gPlayerEight, PLAYER_EIGHT, 0);

                    break;
                case TIME_TRIALS:
                    effect_strategy(gPlayerOne, PLAYER_ONE, 0);

                    if ((gPlayerTwo->flag & IS_GHOST) == IS_GHOST) {
                        effect_strategy(gPlayerTwo, PLAYER_TWO, 0);
                    }

                    if ((gPlayerThree->flag & IS_GHOST) == IS_GHOST) {
                        effect_strategy(gPlayerThree, PLAYER_THREE, 0);
                        break;
                    }

                    break;
                case VERSUS:
                case BATTLE:
                    effect_strategy(gPlayerOne, PLAYER_ONE, 0);
                    effect_strategy(gPlayerTwo, PLAYER_TWO, 0);

                    if (g_playerCount >= 3) {
                        effect_strategy(gPlayerThree, PLAYER_THREE, 0);
                    }

                    if (g_playerCount == 4) {
                        effect_strategy(gPlayerFour, PLAYER_FOUR, 0);
                        break;
                    }

                    break;
            }

            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            switch (g_gameMode) {
                case GRAND_PRIX:
                    effect_strategy(gPlayerOne, PLAYER_ONE, 0);
                    effect_strategy(gPlayerTwo, PLAYER_TWO, 0);
                    effect_strategy(gPlayerThree, PLAYER_THREE, 0);
                    effect_strategy(gPlayerFour, PLAYER_FOUR, 0);
                    effect_strategy(gPlayerFive, PLAYER_FIVE, 0);
                    effect_strategy(gPlayerSix, PLAYER_SIX, 0);
                    effect_strategy(gPlayerSeven, PLAYER_SEVEN, 0);
                    effect_strategy(gPlayerEight, PLAYER_EIGHT, 0);

                    break;
                case VERSUS:
                case BATTLE:
                    effect_strategy(gPlayerOne, PLAYER_ONE, 0);
                    effect_strategy(gPlayerTwo, PLAYER_TWO, 0);

                    break;
                case TIME_TRIALS:
                    effect_strategy(gPlayerOne, PLAYER_ONE, 0);

                    if ((gPlayerTwo->flag & EXISTS) == EXISTS) {
                        effect_strategy(gPlayerTwo, PLAYER_TWO, 0);
                        break;
                    }

                    break;
            }

            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if ((VERSUS == g_gameMode) || (BATTLE == g_gameMode)) {
                effect_strategy(gPlayerOne, PLAYER_ONE, 0);
                effect_strategy(gPlayerTwo, PLAYER_TWO, 0);
                effect_strategy(gPlayerThree, PLAYER_THREE, 0);

                if (g_playerCount == 4) {
                    effect_strategy(gPlayerFour, PLAYER_FOUR, 0);
                }
            }

            break;
    }
}

void effect_strategy(Player* player, s8 playerIndex, s8 arg2) {
    // arg2 is always 0
    s16 temp_s0;

    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->flag & IS_PLAYER) == IS_PLAYER) {
            letterset(player, playerIndex, arg2);
        }

        for (temp_s0 = 0; temp_s0 < 10; ++temp_s0) {
            gassset(player, temp_s0, playerIndex, arg2);
            if (((player->flag & IS_PLAYER) == IS_PLAYER) || (gGamestate == ENDING)) {
                sparkset(player, temp_s0, playerIndex, arg2);
            }
            smokeset(player, temp_s0, playerIndex, arg2);
        }

        if (g_gameMode == BATTLE) {
            balloonset(player, playerIndex);
        }
    }
}

void SmokeDisp(Player* player, s8 playerId, s8 screenId) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (playerId == screenId) {
                OBJCalculation(player, playerId, screenId);
            }
        } else {
            OBJCalculation(player, playerId, screenId);
        }
        exce_obj_calculation(player, playerId, screenId);
    }
}

void SmokeDisp2P(Player* player, s8 playerIndex, s8 screenId) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (playerIndex == screenId) {
                OBJCalculation(player, playerIndex, screenId);
            }
        } else {
            OBJCalculation(player, playerIndex, screenId);
        }
        exce_obj_calculation(player, playerIndex, screenId);
    }
}

void SmokeDisp3P(Player* player, s8 playerIndex, s8 screenId) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (playerIndex == screenId) {
                OBJCalculation(player, playerIndex, screenId);
            }
        } else {
            OBJCalculation(player, playerIndex, screenId);
        }
        exce_obj_calculation(player, playerIndex, screenId);
    }
}

void SmokeDisp4P(Player* player, s8 playerIndex, s8 screenId) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (playerIndex == screenId) {
                OBJCalculation(player, playerIndex, screenId);
            }
        } else {
            OBJCalculation(player, playerIndex, screenId);
        }
        exce_obj_calculation(player, playerIndex, screenId);
    }
}

void SmokeDispAfter(Player* player, s8 arg1, s8 arg2) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (arg1 == arg2) {
                obj_calculation1(player, arg1, arg2);
            }
        } else {
            obj_calculation1(player, arg1, arg2);
        }
    }
}

void SmokeDispAfter2P(Player* player, s8 arg1, s8 arg2) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (arg1 == arg2) {
                obj_calculation1(player, arg1, arg2);
            }
        } else {
            obj_calculation1(player, arg1, arg2);
        }
    }
}

void SmokeDispAfter3P(Player* player, s8 arg1, s8 arg2) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (arg1 == arg2) {
                obj_calculation1(player, arg1, arg2);
            }
        } else {
            obj_calculation1(player, arg1, arg2);
        }
    }
}

void SmokeDispAfter4P(Player* player, s8 arg1, s8 arg2) {
    if ((player->flag & EXISTS) == EXISTS) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (arg1 == arg2) {
                obj_calculation1(player, arg1, arg2);
            }
        } else {
            obj_calculation1(player, arg1, arg2);
        }
    }
}

/***************************************************************/
/* Just a random 10 laying around in data, nothing to see here */
/***************************************************************/
s32 some_unused_data = 10;

#undef MAKE_RGB
