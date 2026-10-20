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

extern s32 BalloonColorArray[8];
extern s32 BalloonAdjustArray[8];
extern s32 BalloonColorArrayB[8];
extern s32 BalloonAdjustArrayB[8];

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
u16 KwflashR;
UNUSED s32 D_801656C8[2];
u16 KwflashG;
UNUSED s32 D_801656D8[2];
u16 KwflashB;
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
//! Tracking a Count of some object Type, don't know what object Type yet
s16 D_80165738;
UNUSED s32 D_8016573C;
s16 D_80165740;
UNUSED s32 D_80165744;
s16 D_80165748;
UNUSED s32 D_8016574C;

s16 NumActiveThwomps;
s32 D_80165754;
ThwompSpawn* ThowmpSpawnList;

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
//! Something related to the Rotation(?) of ice in Sherbet Land
u16 D_801657A2;
UNUSED s32 D_801657A4;
UNUSED s16 D_801657A8[3];
s8 D_801657AE;
UNUSED s8 D_801657AF;
//! HUD related
s8 KWDBDispSW;
UNUSED s8 D_801657B1;
s8 D_801657B2;
UNUSED s8 D_801657B3;
s8 D_801657B4;
s8 D_801657B8[16];
s8 CloudsToggle;
s8 D_801657D0[8];
s8 KWDemoSW;
UNUSED s16 D_801657DA[2];
UNUSED s8 D_801657E0;
s8 D_801657E1;
s8 Kwgoalpanelsw;
s8 D_801657E3;
s8 HUDLapToggle;
s8 D_801657E5;
bool8 HUDSpeedToggle2;
u8 D_801657E7;
bool8 HUDMapToggle2;
UNUSED s32 D_801657EC;
bool8 BlueLineRankToggle;
UNUSED s32 D_801657F4;
bool8 KWLapSW;
s32 D_801657FC;
s8 D_80165800[2];
s32 D_80165804;
s8 HUDToggleFlag;
s32 D_8016580C;
bool8 HUDSpeedToggle;
s32 D_80165814;
bool8 D_80165818;
s32 D_8016581C;
s8 D_80165820;
UNUSED s32 D_80165824;
s8 D_80165828;
Vec3su D_8016582C;
s8 HUDToggleFlagP2[2];
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
s8 Balloonsw;
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
u8 SRandomItemIndex;
s8 D_801658FE;
u8 ControllerRandom;
s16 D_80165900;
UNUSED s32 D_80165904;
s8 D_80165908;
UNUSED s32 D_80165910[96];
s8 D_80165A90;
UNUSED s32 D_80165AA0[95];
UNUSED s32 D_80165C14;
AnmObject DynamicObjects[OBJECT_LIST_SIZE];
UNUSED s32 D_80183D58;
s32 ObjectListSize;
Mtx D_80183D60;
/**
 * Use Unknown. An object is reserved and its Index is saved to
 * this variable, but it appears to go unreferenced
 **/
s32 D_80183DA0;
f32 D_80183DA8[4];
//! Lakitu?
s32 Jugemuallocptr[4];
f32 D_80183DC8[4];
//! Indexes for the objects associated with the Bomb Karts
s32 Bomballocptr[NUM_BOMB_KARTS_MAX];
UNUSED s32 D_80183DF8[16];
//! Next Free spot in EffectAllocArray1? Wraps back around to 0 if it gets bigger than gObjectParticle1_SIZE
s32 NextFreeObjectParticle1;
Vec3f D_80183E40;
//! Next Free spot in EffectAllocArray2? Wraps back around to 0 if it gets bigger than gObjectParticle2_SIZE
s32 NextFreeObjectParticle2;
Vec3f D_80183E50;
//! Next Free spot in EffectAllocArray3?
s32 NextFreeObjectParticle3;
UNUSED s32 D_80183E60[3];
//! Next Free spot in FireParticleAllocArray? Wraps back around to 0 if it gets bigger than gObjectParticle4_SIZE
s32 FireParticleCounter;
Vec3f D_80183E70;
//! Next Free spot in Leafallocptr? Wraps back around to 0 if it gets bigger than gLeafParticle_SIZE
s32 NextFreeLeafParticle;
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
s32 Objallocptr[32];
UNUSED s32 D_80183F20[2];
/**
 * Snowmen heads in Frappe Snowland
 * Chain Chomps in RaindbowRoad?
 * Trophy in award ceremony?
 * Seagulls in Koopa Troopa Beach?
 * Hedgehogs in Yoshi Valley?
 * Spawn for big fire breath in Bowser's Castle
 */
s32 Objallocptr2[32];
/**
 * Seemingly a list of Textures for Lakitu
 * Never explicitly given data, data appears to be placed here
 * via some Type of DMA.
 * I'm also not certain about its dimensions
 * I think the entires in this array are way over-sized
 */
u8 D_80183FA8[4][0x2000];
/**
 * Boos in Banshee Boardwalk
 * Spawners for the 4 small fire breaths inside Bowser's Castle
 */
s32 Objallocptr3[32];
//! Seemingly a Pointer to Lakitu Texture(s)
u8* LakituTexturePtr;
/**
 * Unused list of object indices
 */
s32 Objallocptr4[32];
//! Array of (4) Collisions?
Collision D_8018C0B0[4];
/**
 * List of object list indices Used for:
 *   Moles in Moo Moo Farm
 *   Snow flakes in Frappe Snowland
 *   Segments of the fire breath from the statues in Bowser's Castle
 *   Potentially other things
 */
s32 EffectAllocArray1[gObjectParticle2_SIZE];
Collision D_8018C3B0;
/**
 * List of object list indices Used for:
 * - Bats in Banshee's Boardwalk (but only 1 Player Mode?)
 * - Train Index 0 smoke in Kalimari Desert
 * - Ferry Index 0 smoke in DK Jungle
 */
s32 EffectAllocArray2[gObjectParticle2_SIZE];
// Maybe some Unused Collision?
UNUSED Collision D_8018C5F0;
/**
 * List of object list indices Used for:
 * - Train Index 1 smoke in Kalimari Desert
 * - Ferry Index 1 smoke in DK Jungle
 */
s32 EffectAllocArray3[gObjectParticle3_SIZE];
Collision D_8018C830;
/**
 * List of object list indices. Used both for the fires in the DK Jungle cave
 * and, seemingly for the trail that shells leave behind them.
 * I think they're using the same Texture, which would explain the dual use
 */
s32 FireParticleAllocArray[gObjectParticle4_SIZE];
/**
 * Seemingly a list of object list indices Used for the leaves that sometimes fall
 * trees when you bonk into them
 */
s32 Leafallocptr[gLeafParticle_SIZE];
Hud PlayerHUD[4];
/**
 * List of object list indices Used by the clouds and stars in some stages
 * Also Used for snowflakes like EffectAllocArray1? Not sure what's up with that
 */
s32 CloudAllocate[D_8018CC80_SIZE];
struct_D_8018CE10 D_8018CE10[8];
//! Unknown object Index, only set for Kalimari Desert, never read
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
//! This may be a list of tilemap flags on a per-Camera basis
s16 D_8018CF68[8];
s16 D_8018CF78;
/**
 * List of half-word character IDs indicating each character's
 * place in the Current Grand Prix race's standings
 */
s16 GPCurrentRaceCharacterIdByRank[8];
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
                     // I'm not convinced these aren't just Padding, but stuff doesn't match
                     // without them :/
                     0x0000 };

u8** D_800E4770[] = {
    &DriftSmokeV, &DriftSmokeE, &D_8018D428, &D_8018D428, &D_8018D42C, &D_8018D42C,
    &D_8018D430, &D_8018D430, &D_8018D434, &D_8018D434, &D_8018D434, &D_8018D434,
};

u8** D_800E47A0[] = {
    &Dobon1, &Dobon2, &Dobon3, &Dobon4, &Dobon5, &Dobon6, &Dobon7, &Dobon8,
    &Dobon9, &Dobon10, &Dobon11, &Dobon12, &Dobon13, &Dobon14, &Dobon15,
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
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_80183D60), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
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

void KwdrawSafetyframe(void) {
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

void KwdisplayKumo(u32 arg0) {
    UNUSED Gfx* temp_v1;

    if ((KWDBDispSW != 0)) {
        return;
    }
    gSPDisplayList(DisplayListHead++, &D_0D0076F8);
    KW2DMatrixInit();

    if ((CloudsToggle != 0)) {
        return;
    }

    switch (arg0) {
        case 0:
            KwdisplayKumo1P();
            break;
        case 1:
            KwdisplayKumo2Pl();
            break;
        case 2:
            KwdisplayKumo2Pr();
            break;
        case 3:
            KwdisplayKumo2Pu();
            break;
        case 4:
            KwdisplayKumo2Pd();
            break;
    }
}

void Kwdisplay3D(u32 arg0) {
    UNUSED Gfx* temp_v1;

    if (KWDBDispSW != 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, &D_0D0076F8);

    if (D_8018D22C != 0) {
        return;
    }

    switch (arg0) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            Kwdisplay3D1P();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
            Kwdisplay3D1P();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
            Kwdisplay3D2P();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
            Kwdisplay3D1P();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            Kwdisplay3D2P();
            break;
        case 5:
            Kwdisplay3D1P();
            break;
        case 6:
            Kwdisplay3D2P();
            break;
        case 7:
            Kwdisplay3D3P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
            Kwdisplay3D1P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
            Kwdisplay3D2P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
            Kwdisplay3D3P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            Kwdisplay3D4P();
            break;
    }
}

void Kwdisplay3D1P(void) {

    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    OgawaDraw(PLAYER_ONE);
    if (GameState == ENDING) {
        KwdisplayDai(PLAYER_ONE);
        KwdisplayCup(PLAYER_ONE);
        KwdisplayPukupuku(PLAYER_ONE);
        KWDisplayStar(PLAYER_ONE);
        return;
    }
    if (!DemoFlag) {
        ExplorerKWDisplayJugemu(PLAYER_ONE);
    }
    EventDisplay(PLAYER_ONE);
}

void Kwdisplay3D2P(void) {

    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    OgawaDraw(PLAYER_TWO);
    if (!DemoFlag) {
        ExplorerKWDisplayJugemu(PLAYER_TWO);
    }
    EventDisplay(PLAYER_TWO);
}

void Kwdisplay3D3P(void) {
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[2]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    OgawaDraw(PLAYER_THREE);
    if (!DemoFlag) {
        ExplorerKWDisplayJugemu(PLAYER_THREE);
    }
    EventDisplay(PLAYER_THREE);
}

void Kwdisplay3D4P(void) {

    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[3]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    OgawaDraw(PLAYER_FOUR);
    if ((!DemoFlag) && (PlayerCount == 4)) {
        ExplorerKWDisplayJugemu(PLAYER_FOUR);
    }
    EventDisplay(PLAYER_FOUR);
}

void Kwdisplay3DAfter(u32 arg0) {
    UNUSED Gfx* temp_v1;

    if (KWDBDispSW != 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, &D_0D0076F8);

    if (D_8018D22C != 0) {
        return;
    }
    switch (arg0) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            Kwdisplay3DAfter1P();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
            Kwdisplay3DAfter1P();
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
            Kwdisplay3DAfter2P();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
            Kwdisplay3DAfter1P();
            break;
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            Kwdisplay3DAfter2P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
            Kwdisplay3DAfter1P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
            Kwdisplay3DAfter2P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
            Kwdisplay3DAfter3P();
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            Kwdisplay3DAfter4P();
            break;
    }
}

void Kwdisplay3DAfter1P(void) {
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    if (GameState != ENDING) {
        EventDisplayAfter(PLAYER_ONE);
    }
}

void Kwdisplay3DAfter2P(void) {
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    EventDisplayAfter(PLAYER_TWO);
}

void Kwdisplay3DAfter3P(void) {
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[2]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    EventDisplayAfter(PLAYER_THREE);
}

void Kwdisplay3DAfter4P(void) {
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[3]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    EventDisplayAfter(PLAYER_FOUR);
}

void KWDisplayEvent(s32 CameraId) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            break;
        case COURSE_CHOCO_MOUNTAIN:
            break;
        case COURSE_BOWSER_CASTLE:
            KwdisplayDoshin(CameraId);
            KwdisplayFire(CameraId);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (GameState != CREDITS_SEQUENCE) {
                KwdisplayKanoke(CameraId);
                KwdisplayBat(CameraId);
                KwdisplayPukupuku(CameraId);
                KwdisplayTellesa(CameraId);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            KwdisplayGoalflag(CameraId);
            if (GameState != CREDITS_SEQUENCE) {
                KwdisplayHarinezumi(CameraId);
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (GameState != CREDITS_SEQUENCE) {
                KwdisplaySnowman(CameraId);
            }
            break;
        case COURSE_KOOPA_BEACH:
            if (GameState != CREDITS_SEQUENCE) {
                KwdisplayKani(CameraId);
            }
            if (GameState != CREDITS_SEQUENCE) {

                if ((MenuMultiplayerSelection == 1) || (MenuMultiplayerSelection == 2)) {
                    KwdisplayKamome(CameraId);
                }
            } else {
                KwdisplayKamome(CameraId);
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            break;
        case COURSE_LUIGI_RACEWAY:
            if (Balloonsw != 0) {
                KwdisplayBalloon(CameraId);
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (GameState != CREDITS_SEQUENCE) {
                KwdisplayChoropu(CameraId);
            }
            break;
        case COURSE_TOADS_TURNPIKE:
            break;
        case COURSE_KALAMARI_DESERT:
            KwdisplayTrainSmoke(CameraId);
            break;
        case COURSE_SHERBET_LAND:
            if (GameState != CREDITS_SEQUENCE) {
                KWDisplayIceBlockShadow(CameraId);
            }
            KwdisplayPing(CameraId);
            break;
        case COURSE_RAINBOW_ROAD:
            if (GameState != CREDITS_SEQUENCE) {
                KwdisplayNeon(CameraId);
                KwdisplayWanwan(CameraId);
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
            if (GameState != CREDITS_SEQUENCE) {
                KwdisplayShipSmoke(CameraId);
            }
            break;
    }
#else

#endif

    KwdisplayKameFire(CameraId);
    KwdisplayLeaf(CameraId);

    if (D_80165730 != 0) {
        KwdisplayFuusen(CameraId);
    }
    if (GameMode == BATTLE) {
        KWDisplayBombKartBT(CameraId);
    }
}

void RenderSnowingEffect(s32 arg0) {
    switch (Snow3DCourseID) {
        case COURSE_FRAPPE_SNOWLAND:
            if (GameState != 9) {
                if ((FinishZoomFlag == 0) && (PlayerCount == 1)) {
                    KwdisplaySnow();
                }
            } else {
                KwdisplaySnow();
            }
            break;
        case COURSE_SHERBET_LAND:
            KWDisplayIceBlock(arg0);
            break;
    }
}

void Kwdisplay2DInit(void) {
    gSPDisplayList(DisplayListHead++, &D_0D0076F8);
}

void KWDisplay2D(u32 arg0) {

    D_8018D21C = arg0;
    gSPDisplayList(DisplayListHead++, &D_0D0076F8);

    if (D_8018D22C == 0) {
        switch (arg0) {
            case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
                Kwdisplay2D1P();
                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
                if (!DemoFlag) {
                    Kwdisplay2D2PLeft();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
                if (!DemoFlag) {
                    Kwdisplay2D2PRight();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
                if (!DemoFlag) {
                    Kwdisplay2D2PUp();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
                if (!DemoFlag) {
                    Kwdisplay2D2PDown();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
                if (!DemoFlag) {
                    Kwdisplay2D4PUpleft();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
                if (!DemoFlag) {
                    Kwdisplay2D4PUpright();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
                if (!DemoFlag) {
                    Kwdisplay2D4PDownleft();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
                if ((!DemoFlag) && (PlayerCount == 4)) {
                    Kwdisplay2D4PDownright();
                }
                break;
        }
    }
}

void KWDisplay2DAfter(u32 arg0) {

    D_8018D21C = arg0;
    gSPDisplayList(DisplayListHead++, &D_0D0076F8);
    if (D_8018D22C == 0) {
        switch (arg0) {
            case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
                Kwdisplay2D1PAfter();
                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
                if (!DemoFlag) {
                    KWDisplay2D2PLeftAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
                if (!DemoFlag) {
                    Kwdisplay2D2PRightAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
                if (!DemoFlag) {
                    Kwdisplay2D2PUpAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
                if (!DemoFlag) {
                    Kwdisplay2D2PDownAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
                if (!DemoFlag) {
                    Kwdisplay2D4PUpleftAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
                if (!DemoFlag) {
                    Kwdisplay2D4PUprightAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
                if (!DemoFlag) {
                    Kwdisplay2D4PDownleftAfter();
                    break;
                }

                break;
            case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
                if ((!DemoFlag) && (PlayerCount == 4)) {
                    Kwdisplay2D4PDownrightAfter();
                }
                break;
        }
    }
}

void Kwdisplay2D1P(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void Kwdisplay2D1PAfter(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
        if ((!DemoFlag) && (KWScreenEnable != 0) && (KWDemoSW == 0)) {
            KWDisplayItembox(PLAYER_ONE);
            if (HUDLapToggle != 2) {
                KWDisplayTotalTime(PLAYER_ONE);
                Zanzou2(PLAYER_ONE);
                if (HUDSpeedToggle2 != false) {
                    KwdisplaySpeedmeter(0);
                }
            }
        }
    }
}

void Kwdisplay1PWipe(void) {
}

void KwdisplayOrders(void) {

    if (D_8018D2AC != 0) {
        switch (PlayerCount) {
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

void KwdisplayRanks(void) {
    if (D_8018D2A4 != 0) {
        if (GameMode != BATTLE) {
            switch (PlayerCount) {
                case 1:
                    if (GameMode != TIME_TRIALS) {
                        KWDisplayRank(PLAYER_ONE);
                        break;
                    }
                    break;
                case 2:
                    KWDisplayRank(PLAYER_ONE);
                    KWDisplayRank(PLAYER_TWO);
                    break;
                case 3:
                    KwdisplaySrank(PLAYER_ONE);
                    KwdisplaySrank(PLAYER_TWO);
                    KwdisplaySrank(PLAYER_THREE);
                    break;
                case 4:
                    KwdisplaySrank(PLAYER_ONE);
                    KwdisplaySrank(PLAYER_TWO);
                    KwdisplaySrank(PLAYER_THREE);
                    KwdisplaySrank(PLAYER_FOUR);
                    break;
            }
        }
    }
}

void KwdisplayToppri(void) {

    if ((KWDBDispSW == 0) && (TwoDFlag != 0)) {
        KWSetViewportFull();
        gSPDisplayList(DisplayListHead++, &D_0D0076F8);

        if (KWScreenEnable != 0) {
            if (KWDemoSW == 0) {
                if (BlueLineRankToggle != false) {
                    KwdisplayEnemycar();
                }
                if ((!DemoFlag) && (HUDMapToggle2 != false)) {
                    if (D_80165800[0] != 0) {
                        KwdisplayRadarmap(0);
                        if (GameMode != BATTLE) {
                            KwdisplayRadarsline(0);
                        }
                        KwdisplayRadarplayer(0);
                    }
                    if ((ScreenSplitB == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) && (D_80165800[1] != 0)) {
                        KwdisplayRadarmap(1);
                        if (GameMode != BATTLE) {
                            KwdisplayRadarsline(1);
                        }
                        KwdisplayRadarplayer(1);
                    }
                }
            }
            if ((HUDLapToggle != 2) && (GameMode == GRAND_PRIX) && (D_8018D2BC != 0)) {
                KwdisplayKaopanel();
            }
            KwdisplayRanks();
        }
        KwdisplayOrders();
        KwdrawSafetyframe();
    }
}

void Kwdisplay2D2PLeft(void) {
}

void KWDisplay2D2PLeftAfter(void) {
    if (KWDBDispSW == 0) {
        KWDisplay2P1LR();
    }
}

void Kwdisplay2D2PRight(void) {
}

void Kwdisplay2D2PRightAfter(void) {
    if (KWDBDispSW == 0) {
        KWDisplay2P2LR();
    }
}

void Kwdisplay2D2PSub(s32 PlayerId) {
    if ((GameMode != BATTLE) && (D_80165800[PlayerId] == 0) && (KWScreenEnable != 0)) {
        KWDisplayTotalTime(PlayerId);
        KwdisplaySyuukai(PlayerId);
    }
    KWDisplayItembox(PlayerId);
}

void Kwdisplay2D2PUp(void) {
}

void Kwdisplay2D2PUpAfter(void) {
    if (KWDBDispSW == 0) {
        KWDisplay2P1UD();
    }
}

void Kwdisplay2D2PDown(void) {
}

void Kwdisplay2D2PDownAfter(void) {
    if (KWDBDispSW == 0) {
        KWDisplay2P2UD();
    }
}

void KWDisplayAfter4PSub(s32 PlayerId) {
    if (GameMode != BATTLE) {
        if (KWLapSW && KWScreenEnable) {
            Kwsprite32X8(PlayerHUD[PlayerId].cx, PlayerHUD[PlayerId].cy, common_texture_hud_lap);
            KwdisplayLap88(PlayerHUD[PlayerId].cx - 12, PlayerHUD[PlayerId].cy + 4,
                           PlayerHUD[PlayerId].Lapcnt);
        }
        if (HUDLapToggle == 2) {
            if (PlayerHUD[PlayerId].Bomb && KWFlash8) {
                KWTexture2DCI8BL(PlayerHUD[PlayerId].Bombx, PlayerHUD[PlayerId].Bomby, 0, 1.0f,
                              (u8*) common_tlut_portrait_bomb_kart_and_question_mark, common_texture_portrait_bomb_kart,
                              Vtx_KTile32x32, 0x20, 0x20, 0x20, 0x20);
            }
        }
    }
    KWDisplayItemboxs(PlayerId);
}

void Kwdisplay2D4PUpleft(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void Kwdisplay2D4PUpleftAfter(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
        KWDisplay4P1();
    }
}

void Kwdisplay2D4PUpright(void) {
}

void Kwdisplay2D4PUprightAfter(void) {
    if (KWDBDispSW == 0) {
        KWDisplay4P2();
    }
}

void Kwdisplay2D4PDownleft(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void Kwdisplay2D4PDownleftAfter(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
        KWDisplay4P3();
    }
}

void Kwdisplay2D4PDownright(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
    }
}

void Kwdisplay2D4PDownrightAfter(void) {
    if (KWDBDispSW == 0) {
        KW2DMatrixInit();
        KWDisplay4P4();
    }
}

void Kwplayerparams(s32 PlayerId) {
    Hud* temp_v0;

    D_8018CF1C = &PlayerOne[PlayerId];
    D_8018CF14 = &Camera1[PlayerId];
    temp_v0 = &PlayerHUD[PlayerId];
    temp_v0->plx = (s32) D_8018CF1C->Position[0];
    temp_v0->ply = (s32) D_8018CF1C->Position[1];
    temp_v0->plz = (s32) D_8018CF1C->Position[2];
}

void KwcheckRandomcnt(s32 arg0) {
    struct Controller* controller = &ControllerOne[arg0];

    if ((controller->ButtonHeld & A_BUTTON) != 0) {
        ControllerRandom++;
    }
    if ((controller->ButtonHeld & B_BUTTON) != 0) {
        ControllerRandom++;
    }
    if ((controller->ButtonHeld & R_TRIG) != 0) {
        ControllerRandom++;
    }
}

void KWResultscreenStart(void) {
    D_8018D214 = true;
}

void KwcheckGhostkartItem(void) {
    s32 i;
    Player* Player = PlayerOne;
    for (i = 0; i < 4; i++) {
        if ((D_80165890 != 0) && (Player->Flag & IS_GHOST)) {
            Player->Item = ITEM_MUSHROOM;

            PlayerHUD[i].Ghostcnt = 2;
        }
        if ((Player->Flag & IS_GHOST) && (Player->Item == ITEM_NONE)) {
            if (PlayerHUD[i].Ghostcnt) {
                Player->Item = ITEM_MUSHROOM;
                --PlayerHUD[i].Ghostcnt;
            }
        }
        ++Player;
    }
    D_80165890 = 0;
}

void KwsyoriSub(s32 PlayerId) {
    Kwplayerparams(PlayerId);
    if (!DemoFlag) {
        KwchartJugemuVf(PlayerId);
        KwchartItemboxanm(PlayerId);
    }
}

void KWVideoFramesYori(void) {
    s32 i;

    if (PauseFlag == false) {
        KWStartTime(&D_80165678);
        Kwvideoframetimer++;
        for (i = 0; i < NUM_PLAYERS; i++) {
            D_8018CF68[i] = KwcheckMapareaCamera(&Camera1[i]);
            KwchaseBoundvelZero(i);
        }
        switch (ScreenSplitB) {
            case SCREEN_MODE_1P:
                if (GameState != 9) {
                    KwsyoriSub(PLAYER_ONE);
                    if (GameMode == TIME_TRIALS) {
                        KwcheckGhostkartItem();
                    }
                } else {
                    Kwplayerparams(PLAYER_ONE);
                }
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                KwsyoriSub(PLAYER_ONE);
                KwsyoriSub(PLAYER_TWO);
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                KwsyoriSub(PLAYER_ONE);
                KwsyoriSub(PLAYER_TWO);
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                KwsyoriSub(PLAYER_ONE);
                KwsyoriSub(PLAYER_TWO);
                KwsyoriSub(PLAYER_THREE);
                KwsyoriSub(PLAYER_FOUR);
                break;
        }
        KWGameEventCommonVF();
    }
}

void Kwpausesyori(void) {
    s32 someIndex;
    s32 PlayerId;

    KwcheckCommand();
    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        PlayerId = RankToNum[someIndex];
        // I hate this dumb Pointer access here
        GPCurrentRaceCharacterIdByRank[someIndex] = (PlayerOne + PlayerId)->Kart;
    }
    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        KWRank[someIndex] = GPCurrentRaceRankByPlayerId[someIndex];
    }
}

void KwgameframesyoriSub(void) {

    KwcheckOpeningStart();
    KwcheckCommand();
    KwsetKaopanel();

    if (D_801657AE == 0) {
        switch (ScreenSplitB) {
            case SCREEN_MODE_1P:
                KwcheckRandomcnt(PLAYER_ONE);
                if (D_8018D214 == false) {
                    Kwplayerparams(PLAYER_ONE);
                    KwchartOpening();
                    if (!DemoFlag) {
                        KwchartJugemu(0);
                    }
                    KWChartKumoHookDefault(0);
                    if (PlayerHUD[PLAYER_ONE].Goalsw == 0) {
                        Convkartspeedtoangle((PlayerOneCopy->Speed / 18.0f) * 216.0f);
                    }
                    Kwlapcnt(PLAYER_ONE);
                } else {
                    Kwplayerparams(PLAYER_ONE);
                    KWChartKumoHookDefault(1);
                    Kwplayerparams(PLAYER_TWO);
                    KWChartKumoHookDefault(2);
                }
                KWGameEventCommon();
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                KwcheckRandomcnt(PLAYER_ONE);
                KwcheckRandomcnt(PLAYER_TWO);
                Kwplayerparams(PLAYER_ONE);
                Kwlapcnt(PLAYER_ONE);
                if (!DemoFlag) {
                    KwchartJugemu(0);
                }
                KWChartKumoHookDefault(1);
                KwcheckNearbomb(0);
                Kwplayerparams(PLAYER_TWO);
                Kwlapcnt(PLAYER_TWO);
                if (!DemoFlag) {
                    KwchartJugemu(1);
                }
                KWChartKumoHookDefault(2);
                KwcheckNearbomb(1);
                KWGameEventCommon();
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                KwcheckRandomcnt(PLAYER_ONE);
                KwcheckRandomcnt(PLAYER_TWO);
                Kwplayerparams(PLAYER_ONE);
                Kwlapcnt(PLAYER_ONE);
                if (!DemoFlag) {
                    KwchartJugemu(0);
                }
                KWChartKumoHookDefault(3);
                KwcheckNearbomb(0);
                Kwplayerparams(PLAYER_TWO);
                Kwlapcnt(PLAYER_TWO);
                if (!DemoFlag) {
                    KwchartJugemu(1);
                }
                KWChartKumoHookDefault(4);
                KwcheckNearbomb(1);
                KWGameEventCommon();
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                KwcheckRandomcnt(PLAYER_ONE);
                KwcheckRandomcnt(PLAYER_TWO);
                KwcheckRandomcnt(PLAYER_THREE);
                KwcheckRandomcnt(PLAYER_FOUR);
                Kwplayerparams(PLAYER_ONE);
                Kwlapcnt(PLAYER_ONE);
                if (!DemoFlag) {
                    KwchartJugemu(0);
                }
                KwcheckNearbomb(0);
                Kwplayerparams(PLAYER_TWO);
                Kwlapcnt(PLAYER_TWO);
                if (!DemoFlag) {
                    KwchartJugemu(1);
                }
                KwcheckNearbomb(1);
                Kwplayerparams(PLAYER_THREE);
                Kwlapcnt(PLAYER_THREE);
                if (!DemoFlag) {
                    KwchartJugemu(2);
                }
                KwcheckNearbomb(2);
                if (PlayerCount == 4) {
                    Kwplayerparams(PLAYER_FOUR);
                    Kwlapcnt(PLAYER_FOUR);
                    if ((!DemoFlag) && (PlayerCount == 4)) {
                        KwchartJugemu(3);
                    }
                    KwcheckNearbomb(3);
                }
                KWGameEventCommon();
                break;
        }
        KwdmaTexture();
    }
}

void Kwgameframesyori(void) {
    KWStartTime(&D_80165678);
    Modelingnum = 0;
    D_801655C0 = 0;
    KwsetProjection2DMatrix();
    if (PauseFlag == false) {
        CheckGftimer();
        if (GameState == ENDING) {
            KwchartDai();
            KwchartCup();
            KwchartPukupuku(1);
            KWChartStar();
        } else if (GameState == CREDITS_SEQUENCE) {
            Kwplayerparams(PLAYER_ONE);
            KWChartKumoHookDefault(0);
            KWGameEventCommon();
        } else {
            KwgameframesyoriSub();
        }
    }
    Kwdebug();
    KWGetTime(&D_80165678, (s32) &D_801655F0);
}

void KwcheckKaopanel(s32 PlayerId) {
    s32 ObjectIndex;
    s32 lapCount;
    Player* Player;
    UNUSED s32 stackPadding;

    Player = &PlayerOne[PlayerId];
    ObjectIndex = D_8018CE10[PlayerId].ObjectIndex;
    lapCount = LapCountByPlayerId[PlayerId];
    if (Player->Flag & EXISTS) {
        if (Player->SlipFlag &
            (THUNDER_SPIN | ROLLOVER | SPIN_L | SPIN_R)) {
            DynamicObjects[ObjectIndex].ang[2] += 0x1000;
        } else {
            if (DynamicObjects[ObjectIndex].ang[2] != 0) {
                DynamicObjects[ObjectIndex].ang[2] += 0x1000;
            }
        }
        if (Player->SlipFlag & THUNDER) {
            KWChaseFVal(&DynamicObjects[ObjectIndex].Scale, 0.3f, 0.02f);
        } else {
            KWChaseFVal(&DynamicObjects[ObjectIndex].Scale, 0.6f, 0.02f);
        }
        if (Player->SlipFlag & BROKEN) {
            KwaddUsval(&DynamicObjects[ObjectIndex].ang[0], 0x0C00U, 0x0100U);
        } else {
            KwsubUsval(&DynamicObjects[ObjectIndex].ang[0], 0, 0x00000100);
        }
        if (Player->SlipFlag & (THROW_EXPLODE | EXPLODE)) {
            KwmvNageageGround(ObjectIndex, 6.0f, 1.5f, 0.0f);
        } else {
            KWChaseFVal(&DynamicObjects[ObjectIndex].PosSpline[1], 0.0f, 1.0f);
        }
        if ((Player->Flag & IS_GHOST) || (Player->SlipFlag & TERESA)) {
            DynamicObjects[ObjectIndex].Alpha = 0x0050;
        } else {
            DynamicObjects[ObjectIndex].Alpha = 0x00FF;
        }
        if (lapCount >= 3) {
            DynamicObjects[ObjectIndex].ang[2] = 0;
            DynamicObjects[ObjectIndex].ang[1] = 0;
            DynamicObjects[ObjectIndex].ang[0] = 0;
            DynamicObjects[ObjectIndex].PosSpline[2] = 0.0f;
            DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
            DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
            DynamicObjects[ObjectIndex].Scale = 0.6f;
            DynamicObjects[ObjectIndex].Alpha = 0x00FF;
        }
    }
}

void KwsetKaopanel(void) {
    s32 temp_s0;
    for (temp_s0 = 0; temp_s0 < NUM_PLAYERS; ++temp_s0) {
        KwcheckKaopanel(temp_s0);
    }
}

void KwcheckCommand(void) {
    bool b = false;
    if ((GameState != ENDING) && (GameState != CREDITS_SEQUENCE) && !D_8018D204) {
        switch (PlayerCount) {
            case 1:
                if (ControllerOne->ButtonPressed & R_CBUTTONS) {
                    if (++HUDLapToggle >= 3) {
                        HUDLapToggle = 0;
                    }
                    if (HUDLapToggle == 2) {
                        HUDMapToggle2 = false;
                        HUDSpeedToggle2 = false;
                        BlueLineRankToggle = true;
                    } else if (HUDLapToggle == 1) {
                        HUDMapToggle2 = false;
                        HUDSpeedToggle2 = true;
                        BlueLineRankToggle = false;
                    } else {
                        HUDMapToggle2 = true;
                        HUDSpeedToggle2 = false;
                        BlueLineRankToggle = false;
                    }
                    b = true;
                }
                break;
            case 2:
                if (GameMode != BATTLE) {
                    if (ControllerOne->ButtonPressed & R_CBUTTONS) {
                        D_80165800[0] = (D_80165800[0] + 1) & 1;
                        b = true;
                    }
                    if (ControllerTwo->ButtonPressed & R_CBUTTONS) {
                        D_80165800[1] = (D_80165800[1] + 1) & 1;
                        b = true;
                    }
                    if (D_80165800[0] && D_80165800[1]) {
                        BlueLineRankToggle = false;
                    } else {
                        BlueLineRankToggle = true;
                    }
                    if (DemoFlag) {
                        BlueLineRankToggle = false;
                    }
                }
                break;
            case 3:
                if ((ControllerOne->ButtonPressed & R_CBUTTONS) || (ControllerTwo->ButtonPressed & R_CBUTTONS) ||
                    (ControllerThree->ButtonPressed & R_CBUTTONS)) {
                    if (GameMode != BATTLE) {
                        BlueLineRankToggle = (BlueLineRankToggle + 1) & 1;
                    }
                    HUDLapToggle = (HUDLapToggle + 1) & 1;
                    b = true;
                }
                break;
            case 4:
                if ((ControllerOne->ButtonPressed & R_CBUTTONS) || (ControllerTwo->ButtonPressed & R_CBUTTONS) ||
                    (ControllerThree->ButtonPressed & R_CBUTTONS) || (ControllerFour->ButtonPressed & R_CBUTTONS)) {
                    HUDLapToggle = (HUDLapToggle + 1) & 1;
                    KWLapSW = (KWLapSW + 1) & 1;
                    D_80165800[0] = (D_80165800[0] + 1) & 1;
                    if (GameMode != BATTLE) {
                        BlueLineRankToggle = (BlueLineRankToggle + 1) & 1;
                    }
                    b = true;
                }
                break;
        }
        if (b) {
            KwbackupGamestatus(1);
        }
    }
}

void KWGameEventCommonVF(void) {
    if (CourseID == COURSE_BOWSER_CASTLE) {
        KwchartDoshinVf();
    }
}

extern void MoveCustomParticle(void);

void KWGameEventCommon(void) {
    if (HotSwapID != 0) {
        if (GameState != CREDITS_SEQUENCE) {
            KWChartIceBlockB();
        }
    }
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
        case COURSE_CHOCO_MOUNTAIN:
            break;
        case COURSE_BOWSER_CASTLE:
            KwchartDoshin();
            KwchartFire();
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (GameState != CREDITS_SEQUENCE) {
                KwchartKanoke();
                KwchartDanro();
                if (GameMode != TIME_TRIALS) {
                    KwchartBat();
                }
                KwchartTellesa();
                KwchartPukupuku(0);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            KwchartGoalflag();
            if (GameState != CREDITS_SEQUENCE) {
                KwchartHarinezumi();
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (GameState != CREDITS_SEQUENCE) {
                KwchartSnowman();
            }
            KWChartSnow();
            break;
        case COURSE_KOOPA_BEACH:
            if (GameState != CREDITS_SEQUENCE) {
                KwchartKani();
            }
            if ((MenuMultiplayerSelection == 1) || (MenuMultiplayerSelection == 2) || (GameState == CREDITS_SEQUENCE)) {
                KwchartKamome();
            }
            break;
        case COURSE_LUIGI_RACEWAY:
            if (Balloonsw != 0) {
                KwchartBalloon();
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (GameState != CREDITS_SEQUENCE) {
                KwchartChoropu();
            }
            break;
        case COURSE_KALAMARI_DESERT:
            KwchartTrainSmoke();
            break;
        case COURSE_SHERBET_LAND:
            if (GameState != CREDITS_SEQUENCE) {
                KWChartIceBlockB();
            }
            KwchartPing();
            break;
        case COURSE_RAINBOW_ROAD:
            if (GameState != CREDITS_SEQUENCE) {
                KwchartNeon();
                KwchartWanwan();
            }
            break;
        case COURSE_DK_JUNGLE:
            KwchartShipSmoke();
            break;
    }
#else

#endif

    if (D_80165730 != 0) {
        KwchartFuusen();
    }
    KwchartKameFire();
    if ((s16) CourseID != COURSE_FRAPPE_SNOWLAND) {
        KwchartLeaf();
    }
    if (HotSwapID > 0) {
        MoveCustomParticle();
    }
}

void KwcheckOpeningStart(void) {
    if (D_8018D170 == 0) {
        if (D_8018D178 == 0) {
            if (PlayerCount == 1) {
                KwchartOpeningStart();
            }
            if (PlayerCount == 3) {
                HUDMapToggle2 = true;
            }
            KWScreenEnable = (s32) 1;
            D_8018D170 = (s32) 1;
            D_8018D190 = (s32) 1;
            D_8018D204 = 0;
            return;
        }
        --D_8018D178;
    }
}

void KwchartOpeningStart(void) {
    D_8018D1CC = 1;
    D_8018D1A0 = 0;
}

void KwchartOpeningNext(void) {
    ++D_8018D1CC;
    D_8018D1A0 = 0;
}

void KwchartOpeningJump(s32 arg0) {
    D_8018D1CC = arg0;
    D_8018D1A0 = 0;
}

void KwchartOpeningFinish(void) {
    D_8018D1CC = 0;
    D_8018D1A0 = 0;
}

void KwchartOpeningWait(s32 arg0) {
    if (D_8018D1A0 == 0) {
        D_8018D1D4 = arg0;
        D_8018D1A0 = 1;
    }

    --D_8018D1D4;
    if (D_8018D1D4 < 0) {
        D_8018D1A0 = 0;
        KwchartOpeningNext();
    }
}

void KwchartOpeningInit(void) {
    D_8018D1B4 = 1;
    D_8018D1A0 = 0;
    KwchartOpeningNext();
}

void KwchartOpeningPanelStart(void) {
    if ((GameMode == GRAND_PRIX) && (PlayerCount == 1)) {
        KwchartOpeningJump(0x14);
    }
}

void KwchartOpeningAnime(void) {
    switch (PlayerHUD[PLAYER_ONE].Mptr) {
        case 0:
            break;
        case 1:
            KWChaseSVal(&PlayerHUD[PLAYER_ONE].mx, 0x106, 0x10);
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].my, 182, 0x10) != 0) {
                PlayerHUD[PLAYER_ONE].Mptr++;
                PlayerHUD[PLAYER_ONE].Hptr = 1;
            }
            break;
        case 2:
            KWChaseSVal(&PlayerHUD[PLAYER_ONE].mx, 0x116, 4);
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].my, 0xC6, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Mptr++;
            }
            break;
        case 3:
            KWChaseSVal(&PlayerHUD[PLAYER_ONE].mx, 0x106, 4);
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].my, 182, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Mptr++;
            }
            break;
        case 4:
            KWChaseSVal(&PlayerHUD[PLAYER_ONE].mx, 0x10E, 4);
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].my, 0xBE, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Mptr++;
            }
            break;
        case 5:
            KWChaseSVal(&PlayerHUD[PLAYER_ONE].mx, 0x106, 4);
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].my, 182, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Mptr++;
            }
            break;
        case 6:
            KWChaseSVal(&PlayerHUD[PLAYER_ONE].mx, 0x10A, 2);
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].my, 0xBA, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Mptr++;
            }
            break;
        case 7:
            KWChaseSVal(&PlayerHUD[PLAYER_ONE].mx, 0x106, 2);
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].my, 182, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Mptr++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Mptr = 0;
            break;
    }
    if ((PlayerHUD[PLAYER_ONE].Hptr != 0) && (PlayerHUD[PLAYER_ONE].Hptr == 1)) {
        if (++D_801657E7 >= 0x10) {
            D_801657E7 = 0;
            D_8016579E = 0xDD00;
            PlayerHUD[PLAYER_ONE].Hptr = 0U;
        } else {
            D_8016579E = D_800E55B0[D_801657E7] + 0xDD00;
        }
    }
    switch (PlayerHUD[PLAYER_ONE].Iptr) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Addiy, 0x40, 8) != 0) {
                PlayerHUD[PLAYER_ONE].Iptr++;
            }
            break;
        case 2:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Addiy, 0x38, 8) != 0) {
                PlayerHUD[PLAYER_ONE].Iptr++;
            }
            break;
        case 3:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Addiy, 0x40, 8) != 0) {
                PlayerHUD[PLAYER_ONE].Iptr++;
            }
            break;
        case 4:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Addiy, 0x38, 8) != 0) {
                PlayerHUD[PLAYER_ONE].Iptr++;
            }
            break;
        case 5:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Addiy, 0x40, 8) != 0) {
                PlayerHUD[PLAYER_ONE].Iptr++;
            }
            break;
        case 6:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Addiy, 0x38, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Iptr++;
            }
            break;
        case 7:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Addiy, 0x40, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Iptr++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Iptr = 0;
            break;
    }
    switch (PlayerHUD[PLAYER_ONE].Tptr) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].timerX, 0xE4, 0x10) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr++;
            }
            break;
        case 2:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].timerX, 0xF4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr++;
            }
            break;
        case 3:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].timerX, 0xE4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr++;
            }
            break;
        case 4:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].timerX, 0xEC, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr++;
            }
            break;
        case 5:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].timerX, 0xE4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr++;
            }
            break;
        case 6:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].timerX, 0xE8, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr++;
            }
            break;
        case 7:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].timerX, 0xE4, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Tptr = 0;
            break;
    }
    switch (PlayerHUD[PLAYER_ONE].Cptr) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx, 0x53, 0x10) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr++;
            }
            break;
        case 2:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx, 0x43, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr++;
            }
            break;
        case 3:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx, 0x53, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr++;
            }
            break;
        case 4:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx, 0x4B, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr++;
            }
            break;
        case 5:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx, 0x53, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr++;
            }
            break;
        case 6:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx, 0x4F, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr++;
            }
            break;
        case 7:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx, 0x53, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Cptr = 0;
            break;
    }
    D_8018CFEC = (f32) (PlayerHUD[PLAYER_ONE].mx + 0x18);
    D_8018CFF4 = (f32) (PlayerHUD[PLAYER_ONE].my + 6);
    switch (PlayerHUD[PLAYER_ONE].Tptr2) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xE4, 0x10) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr2++;
            }
            break;
        case 2:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xF4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr2++;
            }
            break;
        case 3:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xE4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr2++;
            }
            break;
        case 4:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xEC, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr2++;
            }
            break;
        case 5:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xE4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr2++;
            }
            break;
        case 6:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xE8, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr2++;
            }
            break;
        case 7:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xE4, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr2++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Tptr2 = 0;
            break;
    }
    switch (PlayerHUD[PLAYER_ONE].Cptr2) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx2, 0x53, 0x10) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr2++;
            }
            break;
        case 2:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx2, 0x43, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr2++;
            }
            break;
        case 3:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx2, 0x53, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr2++;
            }
            break;
        case 4:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx2, 0x4B, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr2++;
            }
            break;
        case 5:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx2, 0x53, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr2++;
            }
            break;
        case 6:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx2, 0x4F, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr2++;
            }
            break;
        case 7:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx2, 0x53, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr2++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Cptr2 = 0;
            break;
    }
    switch (PlayerHUD[PLAYER_ONE].Tptr3) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xE4, 0x10) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr3++;
            }
            break;
        case 2:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xF4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr3++;
            }
            break;
        case 3:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xE4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr3++;
            }
            break;
        case 4:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xEC, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr3++;
            }
            break;
        case 5:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xE4, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr3++;
            }
            break;
        case 6:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xE8, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr3++;
            }
            break;
        case 7:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xE4, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Tptr3++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Tptr3 = 0;
            break;
    }
    switch (PlayerHUD[PLAYER_ONE].Cptr3) {
        case 0:
            break;
        case 1:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx3, 0x53, 0x10) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr3++;
            }
            break;
        case 2:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx3, 0x43, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr3++;
            }
            break;
        case 3:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx3, 0x53, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr3++;
            }
            break;
        case 4:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx3, 0x4B, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr3++;
            }
            break;
        case 5:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx3, 0x53, 4) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr3++;
            }
            break;
        case 6:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx3, 0x4F, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr3++;
            }
            break;
        case 7:
            if (KWChaseSVal(&PlayerHUD[PLAYER_ONE].cx3, 0x53, 2) != 0) {
                PlayerHUD[PLAYER_ONE].Cptr3++;
            }
            break;
        case 8:
            PlayerHUD[PLAYER_ONE].Cptr3 = 0;
            break;
    }
}

void KwchartGoalAnime(void) {
    f32 temp_f0;
    f32* temp_s2;
    f32* temp_s3;
    f32* temp_s4;
    UNUSED f32* var_s1;
    s32 var_s0;

    KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap1CompletionTimeX, 0xE4, 0x10);
    KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap2CompletionTimeX, 0xE4, 0x10);
    KWChaseSVal(&PlayerHUD[PLAYER_ONE].Lap3CompletionTimeX, 0xE4, 0x10);
    KWChaseSVal(&PlayerHUD[PLAYER_ONE].totalTimeX, 0xE4, 0x10);
    for (var_s0 = 0; var_s0 < NUM_PLAYERS; var_s0++) {
        temp_s2 = &Panelx[var_s0];
        temp_s3 = &D_8018D0C8[var_s0];
        temp_s4 = &D_8018D078[var_s0];
        if (HUDCharpicRankY1[var_s0] >= 0.0f) {
            KWChaseFVal(temp_s2, *temp_s3, *temp_s4);
            temp_f0 = *temp_s2;
            if (temp_f0 == *temp_s3) {
                *temp_s4 = 0.0f;
            }
            if ((f64) temp_f0 <= -32.0) {
                HUDCharpicRankY1[var_s0] = -32.0f;
            }
        }
    }
}

void KwchartOpening(void) {
    s32 i;
    UNUSED s32 unk;

    switch (D_8018D1CC) {
        case 0:
            break;
        case 0x1:
            KwchartOpeningInit();
            break;
        case 0x2:
            if (GameMode == TIME_TRIALS) {
                PlayerHUD[PLAYER_ONE].Iptr = 1;
            }
            PlayerHUD[PLAYER_ONE].Mptr = 1;
            PlayerHUD[PLAYER_ONE].Tptr = 1;
            PlayerHUD[PLAYER_ONE].Cptr = 1;
            KwchartOpeningNext();
            break;
        case 0x3:
            KwchartOpeningWait(0);
            break;
        case 0x4:
            PlayerHUD[PLAYER_ONE].Tptr2 = 1;
            PlayerHUD[PLAYER_ONE].Cptr2 = 1;
            KwchartOpeningNext();
            break;
        case 0x5:
            KwchartOpeningWait(0);
            break;
        case 0x6:
            PlayerHUD[PLAYER_ONE].Tptr3 = 1;
            PlayerHUD[PLAYER_ONE].Cptr3 = 1;
            KwchartOpeningNext();
            KwchartOpeningFinish();
            break;
        case 0x14:
            D_8018D078[0] = 16.0f;
            KwchartOpeningNext();
            break;
        case 0x15:
            KwchartOpeningWait(4);
            break;
        case 0x16:
            D_8018D078[1] = 16.0f;
            KwchartOpeningNext();
            break;
        case 0x17:
            KwchartOpeningWait(4);
            break;
        case 0x18:
            D_8018D078[2] = 16.0f;
            KwchartOpeningNext();
            break;
        case 0x19:
            KwchartOpeningWait(4);
            break;
        case 0x1A:
            D_8018D078[3] = 16.0f;
            KwchartOpeningNext();
            break;
        case 0x1B:
            KwchartOpeningWait(0xA);
            break;
        case 0x1C:
            KwchartOpeningFinish();
            break;
        case 0x64:
            KwchartOpeningNext();
            break;
        case 0x65:
            KwchartOpeningWait(0x3A);
            break;
        case 0x66:
            D_8018D078[0] = -8.0f;
            D_8018D0C8[0] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x67:
            KwchartOpeningWait(4);
            break;
        case 0x68:
            D_8018D078[1] = -8.0f;
            D_8018D0C8[1] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x69:
            KwchartOpeningWait(4);
            break;
        case 0x6A:
            D_8018D078[2] = -8.0f;
            D_8018D0C8[2] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x6B:
            KwchartOpeningWait(4);
            break;
        case 0x6C:
            D_8018D078[3] = -8.0f;
            D_8018D0C8[3] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x6D:
            KwchartOpeningWait(0xA);
            break;
        case 0x6E:
            for (i = 0; i < NUM_PLAYERS; i += 4) {
                HUDCharpicRankY1[i] = -32.0f;
                HUDCharpicRankY1[i + 1] = -32.0f;
                HUDCharpicRankY1[i + 2] = -32.0f;
                HUDCharpicRankY1[i + 3] = -32.0f;
            }
            Panelx[0] = 360.0f;
            HUDCharpicRankY1[0] = 110.0f;
            D_8018D0C8[0] = 44.0f;
            D_8018D078[0] = -16.0f;
            Kwgoalpanelsw = 1;
            KwchartOpeningNext();
            break;
        case 0x6F:
            KwchartOpeningWait(4);
            break;
        case 0x70:
            Panelx[1] = 360.0f;
            HUDCharpicRankY1[1] = 110.0f;
            D_8018D0C8[1] = 76.0f;
            D_8018D078[1] = -16.0f;
            KwchartOpeningNext();
            break;
        case 0x71:
            KwchartOpeningWait(4);
            break;
        case 0x72:
            Panelx[2] = 360.0f;
            HUDCharpicRankY1[2] = 110.0f;
            D_8018D0C8[2] = 108.0f;
            D_8018D078[2] = -16.0f;
            KwchartOpeningNext();
            break;
        case 0x73:
            KwchartOpeningWait(4);
            break;
        case 0x74:
            Panelx[3] = 360.0f;
            HUDCharpicRankY1[3] = 110.0f;
            D_8018D0C8[3] = 140.0f;
            D_8018D078[3] = -16.0f;
            KwchartOpeningNext();
            break;
        case 0x75:
            KwchartOpeningWait(4);
            break;
        case 0x76:
            Panelx[4] = 360.0f;
            HUDCharpicRankY1[4] = 110.0f;
            D_8018D0C8[4] = 180.0f;
            D_8018D078[4] = -16.0f;
            KwchartOpeningNext();
            break;
        case 0x77:
            KwchartOpeningWait(4);
            break;
        case 0x78:
            Panelx[5] = 360.0f;
            HUDCharpicRankY1[5] = 110.0f;
            D_8018D0C8[5] = 212.0f;
            D_8018D078[5] = -16.0f;
            KwchartOpeningNext();
            break;
        case 0x79:
            KwchartOpeningWait(4);
            break;
        case 0x7A:
            Panelx[6] = 360.0f;
            HUDCharpicRankY1[6] = 110.0f;
            D_8018D0C8[6] = 244.0f;
            D_8018D078[6] = -16.0f;
            KwchartOpeningNext();
            break;
        case 0x7B:
            KwchartOpeningWait(4);
            break;
        case 0x7C:
            Panelx[7] = 360.0f;
            HUDCharpicRankY1[7] = 110.0f;
            D_8018D0C8[7] = 276.0f;
            D_8018D078[7] = -16.0f;
            KwchartOpeningNext();
            break;
        case 0x7D:
            KwchartOpeningWait(0xA);
            break;
        case 0x7E:
            for (i = 0; i < NUM_PLAYERS; i++) {
                D_8018D078[i] = 0.0f;
            }
            KwchartOpeningNext();
            break;
        case 0x7F:
            KwchartOpeningWait(0x82);
            break;
        case 0x80:
            if (GPCurrentRaceRankByPlayerId[0] < 4) {
                KwchartOpeningJump(0x8C);
            } else {
                KwchartOpeningJump(0x82);
            }
            break;
        case 0x82:
            KwchartOpeningFinish();
            break;
        case 0x8C:
            D_8018D078[0] = -16.0f;
            D_8018D0C8[0] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x8D:
            KwchartOpeningWait(4);
            break;
        case 0x8E:
            D_8018D078[1] = -16.0f;
            D_8018D0C8[1] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x8F:
            KwchartOpeningWait(4);
            break;
        case 0x90:
            D_8018D078[2] = -16.0f;
            D_8018D0C8[2] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x91:
            KwchartOpeningWait(4);
            break;
        case 0x92:
            D_8018D078[3] = -16.0f;
            D_8018D0C8[3] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x93:
            KwchartOpeningWait(4);
            break;
        case 0x94:
            D_8018D078[4] = -16.0f;
            D_8018D0C8[4] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x95:
            KwchartOpeningWait(4);
            break;
        case 0x96:
            D_8018D078[5] = -16.0f;
            D_8018D0C8[5] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x97:
            KwchartOpeningWait(4);
            break;
        case 0x98:
            D_8018D078[6] = -16.0f;
            D_8018D0C8[6] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x99:
            KwchartOpeningWait(4);
            break;
        case 0x9A:
            D_8018D078[7] = -16.0f;
            D_8018D0C8[7] = -32.0f;
            KwchartOpeningNext();
            break;
        case 0x9B:
            KwchartOpeningWait(0x14);
            break;
        case 0x9C:
            KwchartOpeningFinish();
            break;
    }
    if (D_8018D1CC < 0x64) {
        KwchartOpeningAnime();
    } else if (D_8018D1CC < 0xC8) {
        KwchartGoalAnime();
    }
    if ((D_8018D1CC != 0) && (D_8018D1CC >= 0x14) && (D_8018D1CC < 0x1E)) {
        for (i = 0; i < 4; i++) {
            KWChaseFVal(&Panelx[i], D_8018D0C8[i], D_8018D078[i]);
            if (Panelx[i] == D_8018D0C8[i]) {
                D_8018D078[i] = 0.0f;
            }
        }
    }
}

void Convkartspeedtoangle(f32 arg0) {
    if (!PlayerHUD[PLAYER_ONE].Hptr) {
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

void Kwdisplayon(UNUSED s32* arg0) {
}

void Kwdisplayoff(s32* arg0) {
    *arg0 = 0;
}

void Displayfinish(s32 arg0) {
    D_8018D2C8[arg0] = 1;
}

void FlashcolorTable(s8 Index, s16* x, s16* y, s16* z) {
    s16* src = &D_800E4730[Index * 3];
    *x = *src++;
    *y = *src++;
    *z = *src++;
}

void KwgetColor(s8 arg0, s16* arg1, s16* Arg2, s16* Arg3) {
    switch (arg0) {
        case 0:
            *arg1 = 0xFF;
            *Arg2 = 0x40;
            *Arg3 = 0x40;
            break;
        case 1:
            *arg1 = 0xFF;
            *Arg2 = 0xFF;
            *Arg3 = 0x40;
            break;
        case 2:
            *arg1 = 0x40;
            *Arg2 = 0x40;
            *Arg3 = 0xFF;
            break;
    }
}

void CheckGftimer(void) {
    s16 x;
    s16 y;
    s16 z;
    s32 temp_t7;

    temp_t7 = ++Kwgameframetimer;
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
    FlashcolorTable(D_801658A8, &x, &y, &z);
    KwflashR = x / 2;
    KwflashG = y / 2;
    KwflashB = z / 2;
    KwcheckRank();
}

void KwcheckRank(void) {
    s32 var_v0;
    s32 sp0;
    s32 temp_v1;
    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        temp_v1 = GPCurrentRaceRankByPlayerId[var_v0];
        if (KW16GFTimer == 0) {
            KWRank[var_v0] = temp_v1;
        }
        D_8018CF28[temp_v1] = &PlayerOne[sp0];
        if (sp0 == 0) {
            D_80165794 = temp_v1;
        }
    }

    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        sp0 = RankToNum[var_v0];
        D_8018CF50[var_v0] = sp0;
        if (KW16GFTimer == 0) {
            GPCurrentRaceCharacterIdByRank[var_v0] = (PlayerOne + sp0)->Kart;
        }
    }

    if (--D_8018D314 <= 0) {
        D_8018D314 = Wakuanmtmmax;
        Wakur = D_800E55D0[Wakuanmptr][0];
        Wakug = D_800E55D0[Wakuanmptr][1];
        Wakub = D_800E55D0[Wakuanmptr][2];
        if (++Wakuanmptr == 6) {
            Wakuanmptr = 0;
        }
    }
}

void Kwlapcntsub(s32 PlayerId, s32 lapCount) {
    s32 temp_a0_2;
    UNUSED s32 stackPadding;
    s8* huh;
    s8* huhthedeuce;
    Player* Player;

    Player = &PlayerOne[PlayerId];
    huh = &PlayerHUD[PlayerId].Lapcnt;
    huhthedeuce = &PlayerHUD[PlayerId].Goal;
    if (PlayerHUD[PlayerId].Goal < Kwgoalcnt) {
        PlayerHUD[PlayerId].Totaltime = (u32) (s32) (GameTimer * 100.0f);
        if (*huh < lapCount) {
            temp_a0_2 = TimePlayerLastTouchedFinishLine[PlayerId] * 100.0f;
            PlayerHUD[PlayerId].Gettime = temp_a0_2;
            PlayerHUD[PlayerId].Timebuf[*huh] = temp_a0_2;
            if (*huh == 0) {
                PlayerHUD[PlayerId].Laptimebuf[*huh] = PlayerHUD[PlayerId].Gettime;
            } else {
                PlayerHUD[PlayerId].Laptimebuf[*huh] =
                    PlayerHUD[PlayerId].Timebuf[*huh] - PlayerHUD[PlayerId].Timebuf[*huh - 1];
            }
            PlayerHUD[PlayerId].Laptime = PlayerHUD[PlayerId].Laptimebuf[*huh];
            PlayerHUD[PlayerId].Flashtm = 0x003C;
            if (lapCount == 3) {
                PlayerHUD[PlayerId].Totaltime = PlayerHUD[PlayerId].Timebuf[*huh];
            }
            if (GameMode == (s32) 1) {
                if (D_80165638 >= PlayerHUD[PlayerId].Laptime) {
                    if (D_80165638 != PlayerHUD[PlayerId].Laptime) {
                        D_80165658[0] = D_80165658[1] = 0;
                    }
                    NAPlyVoiceStart(0U, (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    D_80165638 = PlayerHUD[PlayerId].Laptime;
                    D_80165658[lapCount - 1] = 1;
                    D_801657E3 = 1;
                }
                if ((lapCount == 3) && ((u32) PlayerHUD[PlayerId].Totaltime < (u32) D_80165648)) {
                    D_801657E5 = 1;
                }
            }
            *huh += 1;
            if (Kwgoalcnt == *huh) {
                *huh = Kwgoalcnt - 1;
            }
            *huhthedeuce += 1;
            if (1) {}
            switch (*huhthedeuce) { /* switch 1; irregular */
                case 0:             /* switch 1 */
                    break;
                case 1: /* switch 1 */
                    Kwlap2Jugemu(PlayerId);
                    NAPlyTrgStart(PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0xF0, 0x15));
                    if ((CourseID == COURSE_LUIGI_RACEWAY) && (Balloonsw == 0) &&
                        (GameMode != (s32) 1)) {
                        Balloonsw = 1;
                    }
                    break;
                case 2: /* switch 1 */
                    KwfinalJugemu(PlayerId);
                    break;
                case 3: /* switch 1 */
                    if ((D_8018D114 == 0) || (D_8018D114 == 1)) {
                        HUDLapToggle = 0;
                        HUDSpeedToggle2 = 0;
                        BlueLineRankToggle = 0;
                        HUDMapToggle2 = 1;
                        D_80165800[0] = 1;
                        D_80165800[1] = 1;
                        D_8018D204 = (s32) 1;
                    }
                    PlayerHUD[PlayerId].Goalsw = 1;
                    if (D_8018D114 == 2) {
                        D_80165800[PlayerId] = 0;
                    }
                    if (CourseID == COURSE_YOSHI_VALLEY) {
                        PlayerHUD[PlayerId].Ranksw = 1;
                    }
                    PlayerHUD[PlayerId].Lap1CompletionTimeX = 0x0140;
                    PlayerHUD[PlayerId].Lap2CompletionTimeX = 0x01E0;
                    PlayerHUD[PlayerId].Lap3CompletionTimeX = 0x0280;
                    PlayerHUD[PlayerId].totalTimeX = 0x0320;
                    D_8016587C = (s32) 1;
                    if (D_8018D20C == 0) {
                        KwgoalJugemu(PlayerId);
                        D_8018D20C = 1;
                        if (MenuMultiplayerSelection == (s8) 1) {
                            D_8018D1CC = 0x00000064;
                        }
                    }
                    break;
            }
        }
    } else {
        KWChaseFVal(&PlayerHUD[PlayerId].rs, 1.0f, 0.125f);
        switch (ScreenSplitB) { /* irregular */
            case 0:
                KWChaseSVal(&PlayerHUD[PlayerId].rgx, 0x001C, 7);
                if (D_8018D1FC != 0) {
                    KWChaseSVal(&PlayerHUD[PlayerId].rgy, -0x0028, 1);
                } else {
                    KWChaseSVal(&PlayerHUD[PlayerId].rgy, -0x0010, 4);
                }
                break;
            case 2:
                KWChaseSVal(&PlayerHUD[PlayerId].rgx, 0x001C, 7);
                KWChaseSVal(&PlayerHUD[PlayerId].rgy, -0x0010, 4);
                break;
            case 1:
                KWChaseSVal(&PlayerHUD[PlayerId].rgx, 0x001C, 7);
                KWChaseSVal(&PlayerHUD[PlayerId].rgy, -0x0010, 4);
                KWChaseSVal(&PlayerHUD[PlayerId].Lap1CompletionTimeX, 0x00E4, 0x0010);
                KWChaseSVal(&PlayerHUD[PlayerId].Lap2CompletionTimeX, 0x00E4, 0x0010);
                KWChaseSVal(&PlayerHUD[PlayerId].Lap3CompletionTimeX, 0x00E4, 0x0010);
                KWChaseSVal(&PlayerHUD[PlayerId].totalTimeX, 0x00E4, 0x0010);
                break;
            case 3:
                if ((PlayerId & 1) == 1) {
                    KWChaseSVal(&PlayerHUD[PlayerId].rgx, -8, 2);
                } else {
                    KWChaseSVal(&PlayerHUD[PlayerId].rgx, 8, 2);
                }
                KWChaseSVal(&PlayerHUD[PlayerId].rgy, -0x0010, 4);
                break;
        }
    }
    if (PlayerHUD[PlayerId].Flashtm == 0) {
        PlayerHUD[PlayerId].Laptime = PlayerHUD[PlayerId].Totaltime;
        D_801657E3 = 0;
        return;
    }
    if (KW8GFTimer == 0) {
        PlayerHUD[PlayerId].Dispsw += 1;
        PlayerHUD[PlayerId].Dispsw &= 1;
    }
    PlayerHUD[PlayerId].Flashtm -= 1;
    if (PlayerHUD[PlayerId].Flashtm == 0) {
        PlayerHUD[PlayerId].Dispsw = 0;
    }
}

void Kwlapcnt(s32 PlayerId) {
    if (GameMode != BATTLE) {
        switch (PlayerId) { /* irregular */
            case PLAYER_ONE:
                Kwlapcntsub(PlayerId, LapCountByPlayerId[PLAYER_ONE]);
                break;
            case PLAYER_TWO:
                Kwlapcntsub(PlayerId, LapCountByPlayerId[PLAYER_TWO]);
                break;
            case PLAYER_THREE:
                Kwlapcntsub(PlayerId, LapCountByPlayerId[PLAYER_THREE]);
                break;
            case PLAYER_FOUR:
                Kwlapcntsub(PlayerId, LapCountByPlayerId[PLAYER_FOUR]);
                break;
        }
    }
}

void KwgoalRankScroll(void) {
    if ((GameMode == GRAND_PRIX) && (PlayerCount == TIME_TRIALS)) {
        KWDemoSW = 1;
        D_8018D2BC = 0;
        D_8018D2A4 = 0;
        if (GPCurrentRaceRankByPlayerId[0] >= 4) {
            D_8018D1FC = 1;
            D_8018D2A4 = 1;
            D_8018D2BC = 1;
        }
    }
}

void KwcheckNearbomb(s32 arg0) {
    s32 playerPathPoint;
    s32 bombPathPoint;
    s32 var_a2;
    s32 pathPointDiff;

    if (GameMode == 2) {
        playerPathPoint = NearestPathPointByPlayerId[arg0];
        PlayerHUD[arg0].Bomb = 0;
        for (var_a2 = 0; var_a2 < NUM_BOMB_KARTS_VERSUS; var_a2++) {
            if ((Bom[var_a2].State == BOMB_STATE_EXPLODED) ||
                (Bom[var_a2].State == BOMB_STATE_INACTIVE)) {
                continue;
            }
            bombPathPoint = Bom[var_a2].PathPointIndex;
            pathPointDiff = bombPathPoint - playerPathPoint;
            if ((pathPointDiff < -5) || (pathPointDiff > 0x1E)) {
                continue;
            }
            PlayerHUD[arg0].Bomb = 1;
            break;
        }
    }
}

// Appears to load GP Mode race staging balloons and Kart shadows.
void Imageinit(void) {
    Hoshi = DecodeMapImage1(gTexture69C80C, 0x400, 0x400);
    KageBitmap = DecodeMapImage1(gTextureKartShadow, 0x1000, 0x1000);
    DriftSmokeV = DecodeMapImage1(gTexture69B03C, 0x100, 0x100);
    DriftSmokeE = DecodeMapImage1(gTexture69B140, 0x400, 0x400);
    D_8018D478 = DecodeMapImage1(gTexture69C1E8, 0x200, 0x200);
    Fukidashi2 = DecodeMapImage1(gTextureSpeechBubble, 0x400, 0x400);
    Onpu = DecodeMapImage1(gTextureMusicNote, 0x400, 0x400);
    D_8018D48C = DecodeMapImage1(gTextureSmokePuff, 0x400, 0x400);
    smoke = DecodeMapImage1(gGroundDust, 0x400, 0x400);
    D_8018D490 = D_8018D48C;
    Grs = DecodeMapImage1(gGrassParticle, 0x1000, 0x1000);
    Balloon1 = DecodeMapImage1(gTextureBalloon1, 0x800, 0x800);
    Balloon2 = DecodeMapImage1(gTextureBalloon2, 0x800, 0x800);
    Bubble = DecodeMapImage1(gTexture69C9C4, 0x200, 0x200);
    Boing = DecodeMapImage1(gTextureBoingExclamation, 0x800, 0x800);
    Poomp1 = DecodeMapImage1(gTextureOnomatopoeiaPoomp1, 0x800, 0x800);
    Poomp2 = DecodeMapImage1(gTextureOnomatopoeiaPoomp2, 0x800, 0x800);
    Whir1 = DecodeMapImage1(gTextureOnomatopoeiaWhrrrr1, 0x800, 0x800);
    Whir2 = DecodeMapImage1(gTextureOnomatopoeiaWhrrrr2, 0x800, 0x800);
    Crash1 = DecodeMapImage1(gTextureOnomatopoeiaCrash1, 0x800, 0x800);
    Crash2 = DecodeMapImage1(gTextureOnomatopoeiaCrash2, 0x800, 0x800);
    Dobon1 = DecodeMapImage1(gTexture69CB84, 0x800, 0x800);
    Dobon2 = DecodeMapImage1(gTexture69CCEC, 0x800, 0x800);
    Dobon3 = DecodeMapImage1(gTexture69CEB8, 0x800, 0x800);
    Dobon4 = DecodeMapImage1(gTexture69D148, 0x800, 0x800);
    Dobon5 = DecodeMapImage1(gTexture69D4E0, 0x800, 0x800);
    Dobon6 = DecodeMapImage1(gTexture69D8FC, 0x800, 0x800);
    Dobon7 = DecodeMapImage1(gTexture69DCB4, 0x800, 0x800);
    Dobon8 = DecodeMapImage1(gTexture69DFA0, 0x800, 0x800);
    Dobon9 = DecodeMapImage1(gTexture69E25C, 0x800, 0x800);
    Dobon10 = DecodeMapImage1(gTexture69E518, 0x800, 0x800);
    Dobon11 = DecodeMapImage1(gTexture69E7A8, 0x800, 0x800);
    Dobon12 = DecodeMapImage1(gTexture69EA18, 0x800, 0x800);
    Dobon13 = DecodeMapImage1(gTexture69EC54, 0x800, 0x800);
    Dobon14 = DecodeMapImage1(gTexture69EE38, 0x800, 0x800);
    Dobon15 = DecodeMapImage1(gTexture69EFE0, 0x800, 0x800);
    ThunderA = DecodeMapImage1(gTextureLightningBolt0, 0x800, 0x800);
    ThunderB = DecodeMapImage1(gTextureLightningBolt1, 0x800, 0x800);
}

void Effectinit(Player* Player) {
    s32 temp_v0;

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        Player->Gass[temp_v0].Flag = 0;
        Player->Gass[temp_v0].Timer = 0;
        Player->Gass[temp_v0].Number = NO_PARTICLE;
    }

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        Player->Spark[temp_v0].Flag = 0;
        Player->Spark[temp_v0].Timer = 0;
        Player->Spark[temp_v0].Number = NO_PARTICLE;
    }

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        Player->smoke[temp_v0].Flag = 0;
        Player->smoke[temp_v0].Timer = 0;
        Player->smoke[temp_v0].Number = NO_PARTICLE;
    }

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        Player->Letter[temp_v0].Flag = 0;
        Player->Letter[temp_v0].Timer = 0;
        Player->Letter[temp_v0].Number = NO_PARTICLE;
    }
}

void MakePos(Player* Player, Particle* arg1, f32 x, f32 y, f32 z, s8 surfaceType, s8 arg6) {
    arg1->trz = z;
    arg1->trx = x;
    arg1->try = y;
    arg1->ang = -Player->Direction[1];
    arg1->Type = surfaceType;
    arg1->Lrflag = arg6;
}

s32 MakeStartup(Particle* arg0, s8 Type, f32 Arg2) {
    arg0->Flag = true;
    arg0->Number = Type;
    arg0->Timer = 0;
    arg0->Scale = Arg2;
}

s32 MakeRDP(Particle* arg0, s32 color, s16 Alpha) {
    arg0->Swork1 = (u8) (color >> 16);
    arg0->Swork2 = (u8) (color >> 8);
    arg0->Swork3 = (u8) color;
    arg0->Swork4 = Alpha;
}

s32 MakeRandomRDP(Particle* arg0, s32 arg1, s16 Alpha) {
    s32 temp_v0;
    temp_v0 = MakeRandomLimmit(48);

    arg0->Swork1 = (u8) ((u8) (arg1 >> 16) - temp_v0);
    arg0->Swork2 = (u8) ((u8) (arg1 >> 8) - temp_v0);
    arg0->Swork3 = (u8) ((u8) arg1 - temp_v0);
    arg0->Swork4 = Alpha;
}

void InitdriftSmokeSet(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 temp_lo;

    if (Player->Slipang >= 0) {
        MakePos(Player, &Player->smoke[arg1], Player->TireRL.Position[0],
                      Player->TireRL.Height + 2.0f, Player->TireRL.Position[2],
                      Player->TireRL.Status, 1);
    } else {
        MakePos(Player, &Player->smoke[arg1], Player->TireRR.Position[0],
                      Player->TireRR.Height + 2.0f, Player->TireRR.Position[2],
                      Player->TireRR.Status, 0);
    }

    temp_lo = Player->Slipang / DEGREES(1);
    if ((temp_lo >= 7) || (temp_lo < -6)) {
        MakeStartup(&Player->smoke[arg1], DRIFT_PARTICLE, 0.35f);
        if (Player->PowerstackCount == 0) {
            MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x70);
        }

        if (Player->PowerstackCount == 1) {
            MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0x00), 0x70);
        }

        if (Player->PowerstackCount >= 2) {
            MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0x96, 0x00), 0x70);
        }

        if (Player->PowerstackCount >= 2) {
            // Why not put this in previous if statement?
            Player->smoke[arg1].Swork5 = 2;
            return;
        }

        Player->smoke[arg1].Swork5 = Player->PowerstackCount;
    }
}

void InitdriftSmoke(Player* Player, s16 arg1, s32 Arg2, s8 Arg3, s8 arg4) {
    if ((arg1 == 0) && ((Player->smoke[Arg2].Timer >= 3) || (Player->smoke[Arg2].Flag == 0))) {
        InitdriftSmokeSet(Player, arg1, Arg2, Arg3, arg4);
    } else if (Player->smoke[Arg2].Timer >= 3) {
        InitdriftSmokeSet(Player, arg1, Arg2, Arg3, arg4);
    }
}

UNUSED void SetInitSmoke(void) {
}

void SetUpSmoke(Particle* particle, s16 arg1, s16 Arg2, s16 Arg3) {
    particle->Swork1 = arg1;
    particle->Swork4 = Arg3;
    particle->Swork5 = Arg2;
}

void InitRndSmoke(Player* Player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
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
        tire_x = Player->TireRL.Position[0];
        tire_y = Player->TireRL.Height + 2.0f;
        tire_z = Player->TireRL.Position[2];
        var_t3 = 1;
        surfaceType = Player->TireRL.Status;
    }
    if ((randval == 2) || (randval == 6)) {
        tire_x = Player->TireRR.Position[0];
        tire_y = Player->TireRR.Height + 2.0f;
        tire_z = Player->TireRR.Position[2];
        var_t3 = 0;
        surfaceType = Player->TireRR.Status;
    }
    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    if ((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_ROYAL_RACEWAY)) {
                        SetUpSmoke(&Player->smoke[arg1], 1, 0, 0x0080);
                    }
                    if (CourseID == COURSE_KALAMARI_DESERT) {
                        SetUpSmoke(&Player->smoke[arg1], 7, 0, 0x0080);
                    }
                    if (CourseID == COURSE_MOO_MOO_FARM) {
                        SetUpSmoke(&Player->smoke[arg1], 8, 0, 0x0080);
                    }
                    if (CourseID == COURSE_WARIO_STADIUM) {
                        SetUpSmoke(&Player->smoke[arg1], 9, 0, 0x0080);
                    }
                    if (CourseID == COURSE_YOSHI_VALLEY) {
                        SetUpSmoke(&Player->smoke[arg1], 10, 0, 0x0080);
                    }
                    if (CourseID == COURSE_DK_JUNGLE) {
                        SetUpSmoke(&Player->smoke[arg1], 11, 0, 0x0080);
                    }
                    Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                if ((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_ROYAL_RACEWAY)) {
                    SetUpSmoke(&Player->smoke[arg1], 1, 0, 0x0080);
                }
                if (CourseID == COURSE_KALAMARI_DESERT) {
                    SetUpSmoke(&Player->smoke[arg1], 7, 0, 0x0080);
                }
                if (CourseID == COURSE_MOO_MOO_FARM) {
                    SetUpSmoke(&Player->smoke[arg1], 8, 0, 0x0080);
                }
                if (CourseID == COURSE_WARIO_STADIUM) {
                    SetUpSmoke(&Player->smoke[arg1], 9, 0, 0x0080);
                }
                if (CourseID == COURSE_YOSHI_VALLEY) {
                    SetUpSmoke(&Player->smoke[arg1], 10, 0, 0x0080);
                }
                if (CourseID == COURSE_DK_JUNGLE) {
                    SetUpSmoke(&Player->smoke[arg1], 11, 0, 0x0080);
                }
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GRASS_PARTICLE, 1.0f);
                    MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                    Player->smoke[arg1].Swork1 -= arg1 * 8;
                    Player->smoke[arg1].Swork2 -= arg1 * 8;
                    Player->smoke[arg1].Swork3 -= arg1 * 8;
                }
            } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GRASS_PARTICLE, 1.0f);
                MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                Player->smoke[arg1].Swork1 -= arg1 * 8;
                Player->smoke[arg1].Swork2 -= arg1 * 8;
                Player->smoke[arg1].Swork3 -= arg1 * 8;
            }
            Player->smoke[arg1].try -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    SetUpSmoke(&Player->smoke[arg1], 2, 1, 0x00A8);
                    Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 2, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    SetUpSmoke(&Player->smoke[arg1], 3, 1, 0x00A8);
                    Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 3, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    SetUpSmoke(&Player->smoke[arg1], 4, 1, 0x00A8);
                    Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 4, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    SetUpSmoke(&Player->smoke[arg1], 5, 1, 0x00A8);
                    Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                MakeRandomRDP(&Player->smoke[arg1], 0x00FFA54F, 0x00AF);
                SetUpSmoke(&Player->smoke[arg1], 5, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    SetUpSmoke(&Player->smoke[arg1], 6, 1, 0x00A8);
                    Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 6, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                if (((((Player->Speed / 18.0f) * 216.0f) >= 30.0f) &&
                     ((((Player->Slipang / DEGREES(1)) > 0x14) || ((Player->Slipang / DEGREES(1)) < (-0x14))))) ||
                    ((Player->OldSpeed - Player->Speed) >= 0.04)) {
                    MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                    MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                    SetUpSmoke(&Player->smoke[arg1], 0, 0, 0x0080);
                    Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
                }
            } else if ((Player->smoke[Arg2].Timer > 0) &&
                       (((((Player->Speed / 18.0f) * 216.0f) >= 30.0f) &&
                         (((Player->Slipang / DEGREES(1)) >= 0x15) || ((Player->Slipang / DEGREES(1)) < -0x14))) ||
                        ((Player->OldSpeed - Player->Speed) >= 0.04))) {
                MakePos(Player, &Player->smoke[arg1], tire_x, tire_y, tire_z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], GROUND_PARTICLE, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 0, 0, 0x0080);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        default:
            break;
    }
}

void InitrndSpl(Player* Player, s16 arg1, s32 Arg2, s8 Arg3, UNUSED s8 arg4) {
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
        if ((WaterlevelPlayer[Arg3] - Player->TireRL.Height) >= 3.5) {
            x = Player->TireRL.Position[0];
            y = Player->TireRL.Height + 2.0f;
            z = Player->TireRL.Position[2];
            var_t1 = 1;
            surfaceType = 0;
        }
    }
    if ((temp_v0 == 2) || (temp_v0 == 6)) {
        if ((WaterlevelPlayer[Arg3] - Player->TireRR.Height) >= 3.5) {
            x = Player->TireRR.Position[0];
            y = Player->TireRR.Height + 2.0f;
            z = Player->TireRR.Position[2];
            var_t1 = 0;
            surfaceType = 0;
        }
    }
    if (1) {}
    if (surfaceType == 0) {
        if ((arg1 == 0) && ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
            if (((Player->Speed / 18.0f) * 216.0f) >= 10.0f) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 9, 0.8f);
                MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00AF);
            }
        } else if ((Player->smoke[Arg2].Timer > 0) && (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
            MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
            MakeStartup(&Player->smoke[arg1], 9, 0.8f);
            MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00AF);
        }
    }
}

void InitRapidSmoke(Player* Player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
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
        x = Player->TireRL.Position[0];
        y = Player->TireRL.Height + 2.0f;
        z = Player->TireRL.Position[2];
        var_t3 = 1;
        surfaceType = Player->TireRL.Status;
    }
    if ((temp_v0 == 0) || (temp_v0 == 6)) {
        x = Player->TireRR.Position[0];
        y = Player->TireRR.Height + 2.0f;
        z = Player->TireRR.Position[2];
        var_t3 = 0;
        surfaceType = Player->TireRR.Status;
    }

    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                if ((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_ROYAL_RACEWAY)) {
                    SetUpSmoke(&Player->smoke[arg1], 1, 0, 0x0080);
                }
                if (CourseID == COURSE_KALAMARI_DESERT) {
                    SetUpSmoke(&Player->smoke[arg1], 7, 0, 0x0080);
                }
                if (CourseID == COURSE_MOO_MOO_FARM) {
                    SetUpSmoke(&Player->smoke[arg1], 8, 0, 0x0080);
                }
                if (CourseID == COURSE_WARIO_STADIUM) {
                    SetUpSmoke(&Player->smoke[arg1], 9, 0, 0x0080);
                }
                if (CourseID == COURSE_YOSHI_VALLEY) {
                    SetUpSmoke(&Player->smoke[arg1], 10, 0, 0x0080);
                }
                if (CourseID == COURSE_DK_JUNGLE) {
                    SetUpSmoke(&Player->smoke[arg1], 11, 0, 0x0080);
                }
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                if ((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_ROYAL_RACEWAY)) {
                    SetUpSmoke(&Player->smoke[arg1], 1, 0, 0x0080);
                }
                if (CourseID == COURSE_KALAMARI_DESERT) {
                    SetUpSmoke(&Player->smoke[arg1], 7, 0, 0x0080);
                }
                if (CourseID == COURSE_MOO_MOO_FARM) {
                    SetUpSmoke(&Player->smoke[arg1], 8, 0, 0x0080);
                }
                if (CourseID == COURSE_WARIO_STADIUM) {
                    SetUpSmoke(&Player->smoke[arg1], 9, 0, 0x0080);
                }
                if (CourseID == COURSE_YOSHI_VALLEY) {
                    SetUpSmoke(&Player->smoke[arg1], 0x000A, 0, 0x0080);
                }
                if (CourseID == COURSE_DK_JUNGLE) {
                    SetUpSmoke(&Player->smoke[arg1], 0x000B, 0, 0x0080);
                }
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.1f);
                MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                Player->smoke[arg1].Swork1 -= arg1 * 8;
                Player->smoke[arg1].Swork2 -= arg1 * 8;
                Player->smoke[arg1].Swork3 -= arg1 * 8;
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.1f);
                MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                Player->smoke[arg1].Swork1 -= arg1 * 8;
                Player->smoke[arg1].Swork2 -= arg1 * 8;
                Player->smoke[arg1].Swork3 -= arg1 * 8;
            }
            Player->smoke[arg1].try -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 2, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 2, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 3, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 3, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 4, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 4, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 5, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 5, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 6, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 6, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 0, 0, 0x0080);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, (s8) surfaceType, (s8) var_t3);
                MakeStartup(&Player->smoke[arg1], 5, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 0, 0, 0x0080);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        default:
            break;
    }
}

void InitSpinSmoke(Player* Player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 var_t1;
    u8 surfaceType;
    f32 x;
    f32 y;
    f32 z;

    var_t1 = 0;
    if ((Player->SlipFlag & SPIN_L) == SPIN_L) {
        x = Player->Position[0];
        y = Player->Position[1] - Player->Radius;
        z = Player->Position[2];
        var_t1 = 1;
        surfaceType = Player->TireRL.Status & 0xFF;
    } else {
        x = Player->Position[0];
        y = Player->Position[1] - Player->Radius;
        z = Player->Position[2];
        surfaceType = Player->TireRR.Status & 0xFF;
    }
    switch (surfaceType) {
        case DIRT:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                if ((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_ROYAL_RACEWAY)) {
                    SetUpSmoke(&Player->smoke[arg1], 1, 0, 0x0080);
                }
                if (CourseID == COURSE_KALAMARI_DESERT) {
                    SetUpSmoke(&Player->smoke[arg1], 7, 0, 0x0080);
                }
                if (CourseID == COURSE_MOO_MOO_FARM) {
                    SetUpSmoke(&Player->smoke[arg1], 8, 0, 0x0080);
                }
                if (CourseID == COURSE_WARIO_STADIUM) {
                    SetUpSmoke(&Player->smoke[arg1], 9, 0, 0x0080);
                }
                if (CourseID == COURSE_YOSHI_VALLEY) {
                    SetUpSmoke(&Player->smoke[arg1], 0x000A, 0, 0x0080);
                }
                if (CourseID == COURSE_DK_JUNGLE) {
                    SetUpSmoke(&Player->smoke[arg1], 0x000B, 0, 0x0080);
                }
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                if ((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_ROYAL_RACEWAY)) {
                    SetUpSmoke(&Player->smoke[arg1], 1, 0, 0x0080);
                }
                if (CourseID == COURSE_KALAMARI_DESERT) {
                    SetUpSmoke(&Player->smoke[arg1], 7, 0, 0x0080);
                }
                if (CourseID == COURSE_MOO_MOO_FARM) {
                    SetUpSmoke(&Player->smoke[arg1], 8, 0, 0x0080);
                }
                if (CourseID == COURSE_WARIO_STADIUM) {
                    SetUpSmoke(&Player->smoke[arg1], 9, 0, 0x0080);
                }
                if (CourseID == COURSE_YOSHI_VALLEY) {
                    SetUpSmoke(&Player->smoke[arg1], 0x000A, 0, 0x0080);
                }
                if (CourseID == COURSE_DK_JUNGLE) {
                    SetUpSmoke(&Player->smoke[arg1], 0x000B, 0, 0x0080);
                }
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case GRASS:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.1f);
                MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                Player->smoke[arg1].Swork1 -= arg1 * 8;
                Player->smoke[arg1].Swork2 -= arg1 * 8;
                Player->smoke[arg1].Swork3 -= arg1 * 8;
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.1f);
                MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
                Player->smoke[arg1].Swork1 -= arg1 * 8;
                Player->smoke[arg1].Swork2 -= arg1 * 8;
                Player->smoke[arg1].Swork3 -= arg1 * 8;
            }
            Player->smoke[arg1].try -= 1.5;
            break;
        case SAND_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 2, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 2, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SAND:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 3, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 3, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case WET_SAND:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 4, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 4, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case DIRT_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 5, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 5, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case SNOW:
        case SNOW_OFFROAD:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 6, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 6, 1, 0x00A8);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            if ((arg1 == 0) &&
                ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 0, 0, 0x0080);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            } else if (Player->smoke[Arg2].Timer > 0) {
                MakePos(Player, &Player->smoke[arg1], x, y, z, surfaceType, var_t1);
                MakeStartup(&Player->smoke[arg1], 4, 0.46f);
                SetUpSmoke(&Player->smoke[arg1], 0, 0, 0x0080);
                Player->smoke[arg1].Swork2 = MakeRandomLimmit(0x0010U);
            }
            break;
        default:
            break;
    }
}

void InitengineSmoke(Player* Player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    UNUSED s32 Thing1;
    s16 thing2;
    UNUSED s32 thing3;
    f32 x;
    f32 y;
    f32 z;
    f32 var_f0;
    s32 var_v0;
    s32 temp_v0;
    UNUSED s32 test;

    if ((Player->HandlingFlag & ACCELERATE) == ACCELERATE) {
        var_v0 = 5;
    } else {
        var_v0 = 0xE;
    }
    temp_v0 = MakeRandomLimmit(var_v0);
    if ((temp_v0 == 1) || (temp_v0 == 2) || (temp_v0 == 3)) {
        if ((arg1 == 0) && ((Player->Gass[Arg2].Timer > 0) || (Player->Gass[Arg2].Flag == 0))) {
            y = Player->Position[1] - 2.5;
            z = Player->Position[2];
            x = Player->Position[0];
            MakePos(Player, &Player->Gass[arg1], x, y, z, 0, 0);
            MakeStartup(&Player->Gass[arg1], 1, 0.5f);
        } else if (Player->Gass[Arg2].Timer > 0) {
            y = Player->Position[1] - 2.5;
            z = Player->Position[2];
            x = Player->Position[0];
            MakePos(Player, &Player->Gass[arg1], x, y, z, 0, 0);
            MakeStartup(&Player->Gass[arg1], 1, 0.5f);
        }
    }
    Player->Gass[arg1].Fwork1 = 0.0f;
    if ((Player->HandlingFlag & ACCELERATE) == ACCELERATE) {
        Player->Gass[arg1].Swork5 = 0;
        if ((Player->SlipFlag & TURBO) == TURBO) {
            MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0x00), 0x0080);
            Player->Gass[arg1].Swork1 = 1;
        } else {
            MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0070);
            Player->Gass[arg1].Swork1 = 0;
        }
    } else {
        Player->Gass[arg1].Swork5 = 1;
        if ((Player->SlipFlag & TURBO) == TURBO) {
            MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0x00), 0x0080);
            Player->Gass[arg1].Swork1 = 1;
        } else {
            MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0070);
            Player->Gass[arg1].Swork1 = 0;
        }
    }
    thing2 = (Player->Gass[arg1].ang - (Player->Slipang / 2));
    if (Player->Gass[arg1].Swork5 == 0) {
        var_f0 = -((Player->Taco / 3000.0f) + 0.1);
    } else {
        var_f0 = -((Player->Taco / 5000.0f) + 0.1);
    }
    MakeSmokepos(&x, &y, &z, 0.0f, 4.5f, (Player->Gass[arg1].Timer * var_f0) + -5.5, -thing2,
                  -Player->Kartroll * 2);
    Player->Gass[arg1].trx = Player->Position[0] + x;
    y = y + (Player->Position[1] - Player->Radius);
    Player->Gass[arg1].trz = Player->Position[2] + z;
    Player->Gass[arg1].try = Player->Gass[arg1].Fwork1 + y;
    Player->Gass[arg1].Lrflag = 0;
}

void InitRingSet(Player* Player, s16 arg1, UNUSED s32 Arg2, s8 Arg3, UNUSED s8 arg4) {
    f32 var_f0;
    f32 sp50;
    f32 sp4C;
    f32 sp48;

    var_f0 = 8.0f - (WaterlevelPlayer[Arg3] - Player->Position[1]);
    if ((f64) var_f0 <= 0.0) {
        var_f0 = 0.0f;
    }
    sp4C = (WaterlevelPlayer[Arg3] - Player->Position[1]) - 3.0f;
    if ((Player->WaterFlag & SUBMERGED) && (CourseID != COURSE_KOOPA_BEACH)) {
        var_f0 = 2.5f;
        sp4C = (f32) ((f64) (WaterlevelPlayer[Arg3] - Player->Position[1]) + 0.1);
    }
    MakePos(Player, &Player->Gass[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
    MakeStartup(&Player->Gass[arg1], 3, var_f0);
    if ((CourseID == COURSE_BOWSER_CASTLE) || (CourseID == COURSE_BIG_DONUT)) {
        MakeRDP(&Player->Gass[arg1], RGB32(0x0, 0x0, 0x0), 0x00AF);
    } else {
        MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00CF);
    }
    MakeSmokepos(&sp50, &sp4C, &sp48, 0.0f, sp4C,
                  ((-Player->Gass[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 10.0f) + -4.0f,
                  -Player->Gass[arg1].ang, -Player->Kartroll * 2);
    Player->Gass[arg1].trx = Player->Position[0] + sp50;
    Player->Gass[arg1].trz = Player->Position[2] + sp48;
    Player->Gass[arg1].try = Player->Position[1] + sp4C;
}

void InitRing(Player* Player, s16 arg1, s32 Arg2, s8 Arg3, s8 arg4) {
    if ((CourseID != COURSE_SKYSCRAPER) && (CourseID != COURSE_RAINBOW_ROAD)) {
        if ((arg1 == 0) && ((Player->Gass[Arg2].Timer > 0) || (Player->Gass[Arg2].Flag == 0))) {
            InitRingSet(Player, arg1, Arg2, Arg3, arg4);
        } else if (Player->Gass[Arg2].Timer > 0) {
            InitRingSet(Player, arg1, Arg2, Arg3, arg4);
        }
    }
}

void InitBubble(Player* Player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 sp54;
    UNUSED s32 pad;
    s32 sp4C;
    f32 sp48;
    f32 sp44;

    if (CourseID == COURSE_SKYSCRAPER) {
        return;
    }
    if (CourseID == COURSE_RAINBOW_ROAD) {
        return;
    }
    sp54 = MakeRandomLimmit(0x0168U) - 0xB4;
    sp4C = MakeRandomLimmit(6U);
    sp44 = MakeRandomLimmit(6U);
    sp48 = MakeRandomLimmit(3U);
    if (Player != PlayerOne) {
        return;
    }
    if ((arg1 == 0) && ((Player->smoke[Arg2].Timer > 0) || (Player->smoke[Arg2].Flag == 0))) {
        MakePos(Player, &Player->smoke[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&Player->smoke[arg1], 0x0B, 0.4f);
        MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        Player->smoke[arg1].trz = Player->Position[2] + (CosT(sp54 * DEGREES(1)) * -1.8);
        Player->smoke[arg1].trx = Player->Position[0] + (SinT(sp54 * DEGREES(1)) * -1.8);
        Player->smoke[arg1].try = (Player->Position[1] - Player->Radius) + sp4C + 2.0f;
        Player->smoke[arg1].dev = sp44 + 1.0f;
        Player->smoke[arg1].Scale = (sp48 + 2.0f) / 10.0f;
    } else if (Player->smoke[Arg2].Timer > 0) {
        MakePos(Player, &Player->smoke[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&Player->smoke[arg1], 0x0B, 0.4f);
        MakeRDP(&Player->smoke[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        Player->smoke[arg1].trz = Player->Position[2] + (CosT(sp54 * DEGREES(1)) * -1.8);
        Player->smoke[arg1].trx = Player->Position[0] + (SinT(sp54 * DEGREES(1)) * -1.8);
        Player->smoke[arg1].try = (Player->Position[1] - Player->Radius) + sp4C + 2.0f;
        Player->smoke[arg1].dev = sp44 + 1.0f;
        Player->smoke[arg1].Scale = (sp48 + 2.0f) / 10.0f;
    }
}

void InitDobon(Player* Player, s16 arg1, UNUSED s32 Arg2, s8 Arg3, UNUSED s8 arg4) {
    MakePos(Player, &Player->Gass[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
    MakeStartup(&Player->Gass[arg1], 5, 4.0f);

    if ((CourseID == COURSE_BOWSER_CASTLE) || (CourseID == COURSE_BIG_DONUT)) {
        MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0x00, 0x00), 0xFF);
    } else {
        MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0xFF);
    }

    Player->Gass[arg1].trz = Player->Position[2] + (CosT(Player->Gass[arg1].ang) * -5.8);
    Player->Gass[arg1].trx = Player->Position[0] + (SinT(Player->Gass[arg1].ang) * -5.8);
    Player->Gass[arg1].try = WaterlevelPlayer[Arg3];
    Player->WaterFlag &= ~SPLASH_DIVE;
}

void InitThunder(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    if (arg1 == 0) {
        MakePos(Player, &Player->Gass[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
        MakeStartup(&Player->Gass[arg1], 6, 3.8f);
        MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0xFF);
        Player->Gass[arg1].Swork1 = 0;
        Player->Gass[arg1].Swork2 = 0;
        Player->Gass[arg1].Swork3 = 0;
    }
}

void InitTeresaSet(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    MakePos(Player, &Player->Gass[arg1], 0.0f, 0.0f, 0.0f, 0, 0);
    MakeStartup(&Player->Gass[arg1], 7, 0.6f);
    MakeRDP(&Player->Gass[arg1], RGB32(0xFF, 0xFF, 0xFF), 0xD0);

    Player->Gass[arg1].trz = Player->Position[2] + (CosT(Player->Gass[arg1].ang) * 6.0f);
    Player->Gass[arg1].trx = Player->Position[0] + (SinT(Player->Gass[arg1].ang) * 6.0f);
    Player->Gass[arg1].try = Player->Position[1] - 5.0f;
    Player->Gass[arg1].Swork5 = 0;
    Player->Gass[arg1].Fwork1 = 0.0f;
}

void InitTeresa(Player* Player, s16 arg1, s32 Arg2, s8 Arg3, s8 arg4) {
    if ((arg1 == 0) && ((Player->Gass[Arg2].Timer > 0) || (Player->Gass[arg1].Flag == 0))) {
        InitTeresaSet(Player, arg1, Arg2, Arg3, arg4);
    } else if (Player->Gass[Arg2].Timer >= 2) {
        InitTeresaSet(Player, arg1, Arg2, Arg3, arg4);
        if (arg1 == 9) {
            Player->HandlingFlag &= ~BOO_SMOKE_EFFECT;
        }
    }
}

void InitstarDust(Player* Player, UNUSED s32 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 var_s2;

    for (var_s2 = 0; var_s2 < 10; var_s2++) {
        Player->Spark[var_s2].Flag = 1;
        Player->Spark[var_s2].Fwork2 = Player->Position[1] + 5.0f;
        Player->Spark[var_s2].ang = (40 * DEGREES(1) * var_s2) - Player->Direction[1];
        Player->Spark[var_s2].Fwork1 = (MakeRandomLimmit(0x0064U) / 100.0f) + 1.5;
        Player->Spark[var_s2].Swork2 = 0;
        Player->Spark[var_s2].Number = 1;
        Player->Spark[var_s2].Timer = 0;
        Player->Spark[var_s2].Swork4 = 0x00FF;
        Player->Spark[var_s2].trz = Player->Position[2];
        Player->Spark[var_s2].trx = Player->Position[0];
    }
    Player->HitFlag &= ~0x0008;
}

void InitresultStar(Player* Player, UNUSED s32 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 var_s2;

    for (var_s2 = 0; var_s2 < 7; var_s2++) {
        Player->Spark[var_s2].Flag = 1;
        Player->Spark[var_s2].Fwork2 = Player->Position[1] - 4.0f;
        Player->Spark[var_s2].ang = (40 * DEGREES(1) * var_s2) - Player->Direction[1];
        // ???
        Player->Spark[var_s2].Fwork1 = (MakeRandomLimmit(0x0064U) / 100.0f) + 1.9;
        Player->Spark[var_s2].Fwork1 = (MakeRandomLimmit(0x0064U) / 100.0f) + 1.5;
        Player->Spark[var_s2].Swork2 = 0;
        Player->Spark[var_s2].Number = 9;
        Player->Spark[var_s2].Timer = 0;
        Player->Spark[var_s2].Swork4 = 0x00FF;
        Player->Spark[var_s2].trz = Player->Position[2];
        Player->Spark[var_s2].trx = Player->Position[0];
    }
    Player->HandlingFlag &= ~TROPHYJUMP_STARS;
}

void InitpowderDust(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 test = 2;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    f32 temp_f0;
    f32 sp28[10] = { (f32) -DEGREES(1), (f32) DEGREES(1), (f32) DEGREES(2), (f32) -DEGREES(2), DEGREES(3), -DEGREES(3), DEGREES(4), -DEGREES(4), DEGREES(5), -DEGREES(5) };

    if (MakeRandomLimmit(3U) == 2.0f) {
        Player->Spark[arg1].Flag = 1;
        Player->Spark[arg1].trx = Player->Position[0];
        Player->Spark[arg1].trz = Player->Position[2];
        Player->Spark[arg1].ang = -Player->Direction[1] + sp28[arg1];
        Player->Spark[arg1].dev = MakeRandomLimmit(1U) + 2.0f;
        temp_f0 = MakeRandomLimmit(4U);
        temp_f0 -= test;
        Player->Spark[arg1].Type = temp_f0;
        Player->Spark[arg1].try = Player->Position[1] + temp_f0;
        Player->Spark[arg1].Scale = 0.15f;
        Player->Spark[arg1].Number = 5;
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Swork4 = 0x00FF;
        Player->Spark[arg1].Swork1 = 0;
    }
}

void InitburnDustSet(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s32 Arg3, UNUSED s32 arg4) {
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
    MakePos(Player, &Player->Spark[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
    MakeStartup(&Player->Spark[arg1], 6, 1.0f);
    if ((Player->SlipFlag & THROW_EXPLODE) || ((Player->SlipFlag) & EXPLODE) ||
        ((Player->SlipFlag) & ROLLOVER) || ((Player->SlipFlag) & TERESA)) {
        MakeRDP(&Player->Spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00A0);
        Player->Spark[arg1].Swork1 -= temp_s1;
        Player->Spark[arg1].Swork2 -= temp_s1;
        Player->Spark[arg1].Swork3 -= temp_s1;
    } else {
        MakeRDP(&Player->Spark[arg1], RGB32(0x0, 0x0, 0x0), 0x00A0);
        Player->Spark[arg1].Swork1 += temp_s1;
        Player->Spark[arg1].Swork2 += temp_s1;
        Player->Spark[arg1].Swork3 += temp_s1;
    }
    Player->Spark[arg1].trz = Player->Position[2] + (CosT(sp54 * DEGREES(1)) * -5.0f);
    Player->Spark[arg1].trx = Player->Position[0] + (SinT(sp54 * DEGREES(1)) * -5.0f);
    Player->Spark[arg1].try = (Player->Position[1] - Player->Radius) + sp4C + 2.0f;
    Player->Spark[arg1].dev = sp44 + 1.0f;
    Player->Spark[arg1].Scale = sp48 + 1.0f;
}

void InitburnDust(Player* Player, s16 arg1, s32 Arg2, s8 Arg3, s8 arg4) {
    if ((arg1 == 0) && ((Player->Spark[Arg2].Timer > 0) || (Player->Spark[Arg2].Flag == 0))) {
        InitburnDustSet(Player, arg1, Arg2, (s32) Arg3, arg4);
    } else if (Player->Spark[Arg2].Timer > 0) {
        InitburnDustSet(Player, arg1, Arg2, (s32) Arg3, arg4);
    }
}

void InitwaterDust(Player* Player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 sp54;
    UNUSED s32 stackPadding0;
    s32 sp4C;
    f32 sp48;
    UNUSED s32 stackPadding1;

    sp54 = MakeRandomLimmit(0x0168U) - 0xB4;
    sp4C = MakeRandomLimmit(6U);
    MakeRandomLimmit(6U);
    sp48 = (f32) MakeRandomLimmit(3U);
    if ((arg1 == 0) && ((Player->Spark[Arg2].Timer > 0) || (Player->Spark[Arg2].Flag == 0))) {
        MakePos(Player, &Player->Spark[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&Player->Spark[arg1], 7, 1.0f);
        MakeRDP(&Player->Spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        Player->Spark[arg1].trz = Player->Position[2] + (CosT(sp54 * DEGREES(1)) * -2.0);
        Player->Spark[arg1].trx = Player->Position[0] + (SinT(sp54 * DEGREES(1)) * -2.0);
        Player->Spark[arg1].try = (Player->Position[1] - Player->Radius) + sp4C + 2.0f;
        Player->Spark[arg1].Scale = (sp48 + 2.0f) / 10.0f;
    } else if (Player->Spark[Arg2].Timer > 0) {
        MakePos(Player, &Player->Spark[arg1], 0.0f, 0.0f, 0.0f, (s8) 0, (s8) 0);
        MakeStartup(&Player->Spark[arg1], 7, 1.0f);
        MakeRDP(&Player->Spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x00FF);
        Player->Spark[arg1].trz = Player->Position[2] + (CosT(sp54 * DEGREES(1)) * -2.0);
        Player->Spark[arg1].trx = Player->Position[0] + (SinT(sp54 * DEGREES(1)) * -2.0);
        Player->Spark[arg1].try = (Player->Position[1] - Player->Radius) + (f32) sp4C + 2.0f;
        Player->Spark[arg1].Scale = (sp48 + 2.0f) / 10.0f;
    }
}

void InittailDust(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 test = 2;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    f32 sp20[10] = { (f32) -DEGREES(1), (f32) DEGREES(1), (f32) DEGREES(2), (f32) -DEGREES(2), DEGREES(3), -DEGREES(3), DEGREES(4), -DEGREES(4), DEGREES(5), -DEGREES(5) };

    if (MakeRandomLimmit(3U) == 2.0f) {
        Player->Spark[arg1].Flag = 1;
        Player->Spark[arg1].trx = Player->Position[0];
        Player->Spark[arg1].try = Player->Position[1] + 2.0f;
        Player->Spark[arg1].trz = Player->Position[2];
        Player->Spark[arg1].ang = -Player->Direction[1] + sp20[arg1];
        Player->Spark[arg1].dev = MakeRandomLimmit(3U) + 2.0f;
        Player->Spark[arg1].Type = MakeRandomLimmit(4U);
        Player->Spark[arg1].Type -= test;
        Player->Spark[arg1].Scale = 0.4f;
        Player->Spark[arg1].Number = 2;
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Swork4 = 0x00FF;
    }
}

void InitSlipstream(Player* Player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
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
        y = Player->Position[1];
    }
    if (temp_v1 == 0) {
        var_t1 = 0;
        var_t0 = 0;
        y = Player->Position[1];
    }
    if (var_t0 == 0) {
        if ((arg1 == 0) &&
            ((Player->Spark[Arg2].Timer > 0) || (Player->Spark[Arg2].Flag == 0))) {
            MakePos(Player, &Player->Spark[arg1], 0.0f, y, 0.0f, (s8) var_t0, (s8) var_t1);
            MakeStartup(&Player->Spark[arg1], 3, 0.5f);
            MakeRDP(&Player->Spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0060);
            Player->Spark[arg1].ang = 0;
            if (Player->Spark[arg1].Lrflag == 1) {
                Player->Spark[arg1].ang += DEGREES(12);
            } else {
                Player->Spark[arg1].ang -= DEGREES(12);
            }
            Player->Spark[arg1].trz =
                Player->Position[2] +
                (CosT(Player->Spark[arg1].ang - Player->Direction[1] - Player->Slipang) * 5.0f);
            Player->Spark[arg1].trx =
                Player->Position[0] +
                (SinT(Player->Spark[arg1].ang - Player->Direction[1] - Player->Slipang) * 5.0f);
        } else if (Player->Spark[Arg2].Timer > 0) {
            MakePos(Player, &Player->Spark[arg1], 0.0f, y, 0.0f, (s8) var_t0, (s8) var_t1);
            MakeStartup(&Player->Spark[arg1], 3, 0.5f);
            MakeRDP(&Player->Spark[arg1], RGB32(0xFF, 0xFF, 0xFF), 0x0060);
            Player->Spark[arg1].ang = 0;
            if (Player->Spark[arg1].Lrflag == 1) {
                Player->Spark[arg1].ang += DEGREES(12);
            } else {
                Player->Spark[arg1].ang -= DEGREES(12);
            }
            Player->Spark[arg1].trz =
                Player->Position[2] +
                (CosT(Player->Spark[arg1].ang - Player->Direction[1] - Player->Slipang) * 5.0f);
            Player->Spark[arg1].trx =
                Player->Position[0] +
                (SinT(Player->Spark[arg1].ang - Player->Direction[1] - Player->Slipang) * 5.0f);
        }
    }
}

void InitTurbostream(Player* player, s16 arg1, s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
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
        y = player->Position[1];
    }

    if (temp_v1 == 0) {
        phi_t1 = 0;
        if (1) {
            phi_t0 = 0;
        }
        y = player->Position[1];
    }

    if (phi_t0 == 0) {
        if ((arg1 == 0) && ((player->Spark[Arg2].Timer > 0) || (player->Spark[Arg2].Flag == 0))) {
            MakePos(player, &player->Spark[arg1], 0.0f, y, 0.0f, phi_t0, phi_t1);
            MakeStartup(&player->Spark[arg1], 8, 1.0f);
            MakeRDP(&player->Spark[arg1], RGB32(0xFF, 0xFF, 0x20), 0xFF);

            player->Spark[arg1].ang = 0;
            if (player->Spark[arg1].Lrflag == 1) {
                player->Spark[arg1].ang += DEGREES(12);
            } else {
                player->Spark[arg1].ang -= DEGREES(12);
            }

            player->Spark[arg1].trz =
                player->Position[2] +
                (CosT((player->Spark[arg1].ang - player->Direction[1]) - player->Slipang) * 5.0f);
            player->Spark[arg1].trx =
                player->Position[0] +
                (SinT((player->Spark[arg1].ang - player->Direction[1]) - player->Slipang) * 5.0f);
            return;
        }

        new_var2 = player;
        if (new_var2->Spark[Arg2].Timer > 0) {
            MakePos(new_var2, &new_var2->Spark[arg1], 0.0f, y, 0.0f, phi_t0, phi_t1);
            MakeStartup(&new_var2->Spark[arg1], 8, 1.0f);
            MakeRDP(&new_var2->Spark[arg1], RGB32(0xFF, 0xFF, 0x20), 0xFF);
            new_var2->Spark[arg1].ang = 0;
            if (new_var2->Spark[arg1].Lrflag == 1) {
                new_var2->Spark[arg1].ang += DEGREES(12);
            } else {
                new_var2->Spark[arg1].ang -= DEGREES(12);
            }

            new_var = new_var2;
            new_var->Spark[arg1].trz =
                new_var->Position[2] +
                (CosT((new_var->Spark[arg1].ang - new_var->Direction[1]) - new_var->Slipang) * 5.0f);
            new_var->Spark[arg1].trx =
                new_var->Position[0] +
                (SinT((new_var->Spark[arg1].ang - new_var->Direction[1]) - new_var->Slipang) * 5.0f);
        }
    }
}

void MakeLand(Player* Player, Particle* arg1, s32 Arg2) {
    arg1->Flag = 1;
    arg1->try = Player->Ground + 1.0f;
    arg1->trz = Player->Position[2];
    arg1->trx = Player->Position[0];
    arg1->ang = (Arg2 * (36 * DEGREES(1))) - Player->Direction[1];
    arg1->Number = 4;
    arg1->Timer = 0;
}

void InitLand(Player* Player, UNUSED s32 arg1, UNUSED s32 Arg2, UNUSED s8 Arg3, UNUSED s8 arg4) {
    s32 var_s1;

    switch (Player->BumpStatus) {
        case DIRT:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                if ((CourseID == COURSE_CHOCO_MOUNTAIN) || (CourseID == COURSE_ROYAL_RACEWAY)) {
                    SetUpSmoke(&Player->Spark[var_s1], 1, 0, 0x00A8);
                }
                if (CourseID == COURSE_KALAMARI_DESERT) {
                    SetUpSmoke(&Player->Spark[var_s1], 7, 0, 0x00A8);
                }
                if (CourseID == COURSE_MOO_MOO_FARM) {
                    SetUpSmoke(&Player->Spark[var_s1], 8, 0, 0x00A8);
                }
                if (CourseID == COURSE_WARIO_STADIUM) {
                    SetUpSmoke(&Player->Spark[var_s1], 9, 0, 0x00A8);
                }
                if (CourseID == COURSE_YOSHI_VALLEY) {
                    SetUpSmoke(&Player->Spark[var_s1], 0x000A, 0, 0x00A8);
                }
                if (CourseID == COURSE_DK_JUNGLE) {
                    SetUpSmoke(&Player->Spark[var_s1], 0x000B, 0, 0x00A8);
                }
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        case GRASS:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 2, 1, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        case SAND_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 2, 1, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        case SAND:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 3, 1, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        case WET_SAND:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 4, 1, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        case DIRT_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 5, 1, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        case SNOW:
        case SNOW_OFFROAD:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 6, 1, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        case ASPHALT:
        case STONE:
        case BRIDGE:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 0, 0, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
        default:
            for (var_s1 = 0; var_s1 < 10; var_s1++) {
                SetUpSmoke(&Player->Spark[var_s1], 0, 0, 0x00A8);
                MakeLand(Player, &Player->Spark[var_s1], var_s1);
            }
            Player->HandlingFlag &= ~LANDING_DUST;
            break;
    }
}

void InitcrashLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    Player->Letter[Index].Flag = 1;
    Player->Letter[Index].ang = -Player->Direction[1];
    Player->Letter[Index].Number = 2;
    Player->Letter[Index].Timer = 0;
    Player->Letter[Index].Scale = 0.2f;
}

void InithitLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    Player->Letter[Index].Flag = 1;
    Player->Letter[Index].ang = -Player->Direction[1];
    Player->Letter[Index].Number = 4;
    Player->Letter[Index].Timer = 0;
    Player->Letter[Index].Scale = 1.0f;
}

void InitboingLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    Player->Letter[Index].Flag = 1;
    Player->Letter[Index].ang = -Player->Direction[1];
    Player->Letter[Index].Number = 5;
    Player->Letter[Index].Timer = 0;
    Player->Letter[Index].Scale = 0.2f;
}

void InitpoompLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    Player->Letter[Index].Flag = 1;
    Player->Letter[Index].ang = -Player->Direction[1];
    Player->Letter[Index].Number = 6;
    Player->Letter[Index].Timer = 0;
    Player->Letter[Index].Scale = 0.2f;
    Player->Letter[Index].try = 0.0f;
}

void InitspinLetter(Player* Player, s8 arg1, UNUSED s8 Arg2, s8 Index) {
    Player->Letter[Index].Flag = 1;
    Player->Letter[Index].Number = 3;
    Player->Letter[arg1 /* arg1 instead of Index */].Scale = 0.2f;
    Player->Letter[Index].Timer = 1;
    Player->Letter[Index].ang = 0;
    Player->Talk &= ~WHIRRR;
    Player->Letter[Index].trz = Player->Position[2];
    Player->Letter[Index].trx = Player->Position[0];
    Player->Letter[Index].try = (Player->Position[1] + 4.0f);
}

void InitonpuLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    Player->Letter[Index].Flag = 1;
    Player->Letter[Index].Number = 5;
    Player->Letter[Index].Scale = 0.1f;
    Player->Letter[Index].Timer = 0;
    Player->Letter[Index].try = (Player->Position[1] + Player->Radius) - 2.5;
}

void MakeSmokepos(f32* arg0, f32* arg1, f32* Arg2, f32 Arg3, f32 arg4, f32 arg5, u16 arg6, u16 arg7) {
    UNUSED f32 pad;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 temp_f20;

    //  Apply the Matrix multiplication:
    //  Matrix is the Jacobian for cartesian to spherical coordinates?
    //  Get the change in r, theta, and phi for a change in x, y, z?
    //
    // | arg0 |     | cos(arg6)*cos(arg7)  cos(arg6)*sin(arg7)   -sin(arg6) |     | Arg3 |
    // |      |     |                                                       |     |      |
    // | arg1 |  =  | -sin(arg7)           cos(arg7)                 0      |  X  | arg4 |
    // |      |     |                                                       |     |      |
    // | Arg2 |     | sin(arg6)*cos(arg7)  sin(arg6)*sin(arg7)    cos(arg6) |     | arg5 |
    //
    sp28 = SinT(arg7);
    sp2C = CosT(arg6);
    sp30 = CosT(arg7);
    temp_f20 = CosT(arg6);
    //    = Arg3 * cos(arg6) * cos(arg7) + arg4 * cos(arg6) * sin(arg7) - arg5 * sin(arg6)
    *arg0 = (((Arg3 * temp_f20) * sp30) + (arg4 * sp2C) * sp28) - (SinT(arg6) * arg5);

    temp_f20 = SinT(arg7);
    //    = -Arg3 * sin(arg7) + arg4 * cos(arg7)
    *arg1 = (CosT(arg7) * arg4) - (Arg3 * temp_f20);

    sp28 = SinT(arg7);
    sp2C = SinT(arg6);
    sp30 = CosT(arg7);
    temp_f20 = SinT(arg6);
    //    = Arg3 * sin(arg6) * cos(arg7) + arg4 * sin(arg6) * sin(arg7) + arg5 * cos(arg6)
    *Arg2 = (CosT(arg6) * arg5) + (((Arg3 * temp_f20) * sp30) + ((arg4 * sp2C) * sp28));
}

void EngineSmoke(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s32 Arg3) {
    f32 sp48[8] = { 4.5f, 4.5f, 4.5f, 4.5f, 4.5f, 5.5f, 4.5f, 6.5f };
    f32 var_f6;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    s16 thing;

    Player->Gass[arg1].Timer++;
    if (Player->Gass[arg1].Timer == 0x000C) {
        Player->Gass[arg1].Flag = 0;
        Player->Gass[arg1].Timer = 0;
        Player->Gass[arg1].Number = NO_PARTICLE;
    }
    Player->Gass[arg1].dev = 2.0f;
    if (Player->Gass[arg1].Swork5 == 0) {
        Player->Gass[arg1].Scale = Player->Gass[arg1].Scale + 0.07;
        Player->Gass[arg1].Fwork1 = Player->Gass[arg1].Fwork1 + 0.3;
        if (Player->Gass[arg1].Timer >= 3) {
            Player->Gass[arg1].Swork4 -= 3;
        }
        if (Player->Gass[arg1].Swork4 <= 0) {
            Player->Gass[arg1].Swork4 = 0;
        }
    } else {
        Player->Gass[arg1].Scale = Player->Gass[arg1].Scale + 0.1;
        Player->Gass[arg1].Fwork1 = Player->Gass[arg1].Fwork1 + 0.3;
        if (Player->Gass[arg1].Timer >= 3) {
            Player->Gass[arg1].Swork4 -= 2;
        }
        if (Player->Gass[arg1].Swork4 <= 0) {
            Player->Gass[arg1].Swork4 = 0;
        }
    }
    thing = Player->Gass[arg1].ang - (Player->Slipang / 2);
    if (Player->Gass[arg1].Swork5 == 0) {
        var_f6 = -((Player->Taco / 5000.0f) + 0.1);
    } else {
        var_f6 = -((Player->Taco / 6000.0f) + 0.1);
    }
    if (((Player->SlipFlag & TURBO) == TURBO) && (Player->Gass[arg1].Timer >= 6)) {
        Player->Gass[arg1].Scale = Player->Gass[arg1].Scale + 0.06;
    }
    Player->Gass[arg1].Lrflag++;
    if (Player->Gass[arg1].Lrflag >= 3) {
        Player->Gass[arg1].Lrflag = 0;
    }
    MakeSmokepos(&sp40, &sp38, &sp3C, 0.0f, sp48[Player->Kart],
                  (Player->Gass[arg1].Timer * var_f6) + -5.5, -thing, -Player->Kartroll * 2);
    Player->Gass[arg1].trx = Player->Position[0] + sp40;
    sp38 = (Player->Position[1] - Player->Radius) + sp38;
    Player->Gass[arg1].trz = Player->Position[2] + sp3C;
    Player->Gass[arg1].try = Player->Gass[arg1].Fwork1 + sp38;
}

void RandomBubble(Player* Player, s16 arg1, s8 Arg2, UNUSED s8 Arg3) {
    f32 temp_f0;

    temp_f0 = Player->smoke[arg1].dev / 10.0f;
    ++Player->smoke[arg1].Timer;
    Player->smoke[arg1].try += temp_f0;
    if ((Player->JugemuFlag & IS_IN_WATER) == IS_IN_WATER) {
        Player->smoke[arg1].try += (temp_f0 + 0.3);
        if ((Player->smoke[arg1].Timer == 0x10) ||
            ((WaterlevelPlayer[Arg2] - Player->smoke[arg1].try) < 3.0f)) {
            Player->smoke[arg1].Flag = 0;
            Player->smoke[arg1].Timer = 0;
            Player->smoke[arg1].Number = NO_PARTICLE;
        }
    } else if ((Player->smoke[arg1].Timer == 0xA) ||
               ((WaterlevelPlayer[Arg2] - Player->smoke[arg1].try) < 3.0f)) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }
}

void DobonSplash(Player* Player, s16 arg1, s8 Arg2, UNUSED s8 Arg3) {
    ++Player->Gass[arg1].Timer;
    Player->Gass[arg1].trz = Player->Position[2] + CosT(Player->Gass[arg1].ang) * -5.8;
    Player->Gass[arg1].trx = Player->Position[0] + SinT(Player->Gass[arg1].ang) * -5.8;
    Player->Gass[arg1].try = WaterlevelPlayer[Arg2];
    if (Player->Gass[arg1].Timer == 15) {
        Player->Gass[arg1].Flag = 0;
        Player->Gass[arg1].Timer = 0;
        Player->Gass[arg1].Number = NO_PARTICLE;
    }
}

void ThunderEffect(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    ++Player->Gass[arg1].Timer;
    if ((s32) Player->Gass[arg1].Timer < 9) {
        if ((Player->Gass[arg1].Timer & 1) != 0) {
            Player->Gass[arg1].Swork1 = 8;
        } else {
            Player->Gass[arg1].Swork1 = 0;
        }
    } else if (((Player->Gass[arg1].Timer & 1) != 0) ||
               ((Player->Gass[arg1].Timer >= 9) && (Player->Gass[arg1].Timer < 12))) {
        Player->Gass[arg1].Swork1 = 0xFF;
    } else if ((Player->Gass[arg1].Timer & 2) != 0) {
        Player->Gass[arg1].Swork1 = 8;
    } else {
        Player->Gass[arg1].Swork1 = 0;
    }
    Player->Gass[arg1].Swork2 = 0;
    Player->Gass[arg1].Swork3 = 0;
    if ((s32) Player->Gass[arg1].Timer >= 0x19) {
        Player->Gass[arg1].Flag = 0;
        Player->Gass[arg1].Timer = 0;
        Player->Gass[arg1].Number = NO_PARTICLE;
    }
}

void TeresaEffect(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    if (Player->Gass[arg1].Timer >= 0x1E) {
        Player->Gass[arg1].Swork5 += 0x1FFE;
    } else {
        Player->Gass[arg1].Swork5 += 0x1554;
    }

    Player->Gass[arg1].Fwork1 += 0.25;
    Player->Gass[arg1].trz =
        Player->Position[2] + (CosT((Player->Gass[arg1].ang + Player->Gass[arg1].Swork5)) * 5.5);
    Player->Gass[arg1].trx =
        Player->Position[0] + (SinT((Player->Gass[arg1].ang + Player->Gass[arg1].Swork5)) * 5.5);
    Player->Gass[arg1].try = ((Player->Position[1] - 5.0f) + Player->Gass[arg1].Fwork1);
    ++Player->Gass[arg1].Timer;
    Player->Gass[arg1].Scale += 0.05;
    Player->Gass[arg1].Swork4 -= 5;

    if ((s32) Player->Gass[arg1].Swork4 <= 0) {
        Player->Gass[arg1].Swork4 = 0;
    }

    if ((s32) Player->Gass[arg1].Timer >= 0x28) {
        Player->Gass[arg1].Flag = 0;
        Player->Gass[arg1].Timer = 0;
        Player->Gass[arg1].Number = NO_PARTICLE;
    }
}

void DriftSmoke(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    if (Player->smoke[arg1].Lrflag == 1) {
        Player->smoke[arg1].trz =
            Player->TireRL.Position[2] +
            (Player->smoke[arg1].Timer * -7) * CosT(Player->smoke[arg1].ang);
        Player->smoke[arg1].trx =
            Player->TireRL.Position[0] +
            (Player->smoke[arg1].Timer * -7) * SinT(Player->smoke[arg1].ang);
    } else {
        Player->smoke[arg1].trz =
            Player->TireRR.Position[2] +
            (Player->smoke[arg1].Timer * -7) * CosT(Player->smoke[arg1].ang);
        Player->smoke[arg1].trx =
            Player->TireRR.Position[0] +
            (Player->smoke[arg1].Timer * -7) * SinT(Player->smoke[arg1].ang);
    }

    ++Player->smoke[arg1].Timer;
    Player->smoke[arg1].try += 1.0f;

    if (((Player->SlipFlag & SPIN_L) != 0) || ((Player->SlipFlag & SPIN_R) != 0)) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
    }

    if (Player->smoke[arg1].Timer == 8) {
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }

    Player->smoke[arg1].Scale += 0.08;
    if (Player->smoke[arg1].Timer >= 4) {
        Player->smoke[arg1].Swork4 -= 16;
    }

    if (Player->smoke[arg1].Swork4 <= 0) {
        Player->smoke[arg1].Swork4 = 0;
    }
}

void RandomSmoke(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    if (Player->smoke[arg1].Lrflag == 1) {
        if ((Player->SlipFlag & THUNDER)) {
            MakeSmokepos(&sp44, &sp40, &sp3C, -2.0f, 0.0f,
                          (-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 16,
                          -Player->smoke[arg1].ang, 2 * -Player->Kartroll);
            Player->smoke[arg1].trx = Player->TireRL.Position[0] + sp44;
            Player->smoke[arg1].trz = Player->TireRL.Position[2] + sp3C;
        } else {
            Player->smoke[arg1].trz =
                Player->TireRL.Position[2] +
                ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 16) *
                    CosT(Player->smoke[arg1].ang);
            Player->smoke[arg1].trx =
                Player->TireRL.Position[0] +
                ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 16) *
                    SinT(Player->smoke[arg1].ang);
        }
    } else if ((Player->SlipFlag & THUNDER)) {
        MakeSmokepos(&sp44, &sp40, &sp3C, 2.0f, 0.0f,
                      (-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 16,
                      -Player->smoke[arg1].ang, 2 * -Player->Kartroll);
        Player->smoke[arg1].trx = Player->TireRR.Position[0] + sp44;
        Player->smoke[arg1].trz = Player->TireRR.Position[2] + sp3C;
    } else {
        Player->smoke[arg1].trz =
            Player->TireRR.Position[2] +
            ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 16) *
                CosT(Player->smoke[arg1].ang);
        Player->smoke[arg1].trx =
            Player->TireRR.Position[0] +
            ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 16) *
                SinT(Player->smoke[arg1].ang);
    }

    ++Player->smoke[arg1].Timer;
    Player->smoke[arg1].try += 0.2;
    if (((Player->SlipFlag & SPIN_L) != 0) || ((Player->SlipFlag & SPIN_R) != 0)) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
    }

    if (Player->smoke[arg1].Timer == 8) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }

    Player->smoke[arg1].Scale += 0.1;
    if (Player->smoke[arg1].Swork5 == 0) {
        if (Player->smoke[arg1].Timer >= 4) {
            Player->smoke[arg1].Swork4 -= 12;
        }
        if (Player->smoke[arg1].Swork4 <= 0) {
            Player->smoke[arg1].Swork4 = 0;
        }
    } else {
        if (Player->smoke[arg1].Timer >= 4) {
            Player->smoke[arg1].Swork4 -= 16;
        }
        if (Player->smoke[arg1].Swork4 <= 0) {
            Player->smoke[arg1].Swork4 = 0;
        }
    }
}

void RapidSmoke(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    if (Player->smoke[arg1].Lrflag == 1) {
        Player->smoke[arg1].trz =
            Player->TireRL.Position[2] +
            (-1.8f * Player->smoke[arg1].Timer) * CosT(Player->smoke[arg1].ang);
        Player->smoke[arg1].trx =
            Player->TireRL.Position[0] +
            (-1.8f * Player->smoke[arg1].Timer) * SinT(Player->smoke[arg1].ang);
    } else {
        Player->smoke[arg1].trz =
            Player->TireRR.Position[2] +
            (-1.8f * Player->smoke[arg1].Timer) * CosT(Player->smoke[arg1].ang);
        Player->smoke[arg1].trx =
            Player->TireRR.Position[0] +
            (-1.8f * Player->smoke[arg1].Timer) * SinT(Player->smoke[arg1].ang);
    }
    ++Player->smoke[arg1].Timer;
    Player->smoke[arg1].try += 0.3;
    if (Player->smoke[arg1].Timer == 8) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }

    Player->smoke[arg1].Scale += 0.15;
    if (Player->smoke[arg1].Swork5 == 0) {
        if ((s32) Player->smoke[arg1].Timer >= 4) {
            --Player->smoke[arg1].Swork4;
        }
        if ((s32) Player->smoke[arg1].Swork4 <= 0) {
            Player->smoke[arg1].Swork4 = 0;
        }
    } else {
        if ((s32) Player->smoke[arg1].Timer >= 4) {
            Player->smoke[arg1].Swork4 -= 16;
        }
        if ((s32) Player->smoke[arg1].Swork4 <= 0) {

            Player->smoke[arg1].Swork4 = 0;
        }
    }
}

void RandomGrass(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    if (Player->smoke[arg1].Lrflag == 1) {
        Player->smoke[arg1].trz =
            Player->TireRL.Position[2] +
            (-2 * Player->smoke[arg1].Timer * CosT(Player->smoke[arg1].ang));
        Player->smoke[arg1].trx =
            Player->TireRL.Position[0] +
            (-2 * Player->smoke[arg1].Timer * SinT(Player->smoke[arg1].ang));
    } else {
        Player->smoke[arg1].trz =
            Player->TireRR.Position[2] +
            (-2 * Player->smoke[arg1].Timer * CosT(Player->smoke[arg1].ang));
        Player->smoke[arg1].trx =
            Player->TireRR.Position[0] +
            (-2 * Player->smoke[arg1].Timer * SinT(Player->smoke[arg1].ang));
    }

    ++Player->smoke[arg1].Timer;
    Player->smoke[arg1].try += 0.2;
    if (Player->smoke[arg1].Timer == 8) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }

    Player->smoke[arg1].dev = 2.0f;
    Player->smoke[arg1].Scale -= 0.06;
}

void RandomSpl(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    if (Player->smoke[arg1].Lrflag == 1) {
        Player->smoke[arg1].trz =
            Player->TireRL.Position[2] +
            ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 20.0f) *
                CosT(Player->smoke[arg1].ang);
        Player->smoke[arg1].trx =
            Player->TireRL.Position[0] +
            ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 20.0f) *
                SinT(Player->smoke[arg1].ang);
    } else {
        Player->smoke[arg1].trz =
            Player->TireRR.Position[2] +
            ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 20.0f) *
                CosT(Player->smoke[arg1].ang);
        Player->smoke[arg1].trx =
            Player->TireRR.Position[0] +
            ((-Player->smoke[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 20.0f) *
                SinT(Player->smoke[arg1].ang);
    }

    ++Player->smoke[arg1].Timer;
    if (Player->smoke[arg1].Timer == 8) {
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }

    Player->smoke[arg1].Scale += 0.2;
    if (Player->smoke[arg1].Timer >= 4) {
        Player->smoke[arg1].Swork4 -= 18;
        Player->smoke[arg1].try -= 0.1;
    } else {
        Player->smoke[arg1].try += 0.4;
    }

    if (Player->smoke[arg1].Swork4 <= 0) {
        Player->smoke[arg1].Swork4 = 0;
    }
}

void RandomTurbo(Player* Player, s16 arg1, UNUSED s32 Arg2, UNUSED s32 Arg3) {
    f32 sp3C;
    f32 sp38;
    f32 sp34;

    if (Player->smoke[arg1].Lrflag == 1) {
        MakeSmokepos(&sp3C, &sp34, &sp38, 3.0f, 0.0f,
                      -5.5 - (Player->smoke[arg1].Timer * (((Player->Speed / 18.0f) * 216.0f) / 15.0f)),
                      -Player->smoke[arg1].ang, 0);
    } else {
        MakeSmokepos(&sp3C, &sp34, &sp38, -3.0f, 0.0f,
                      -5.5 - (Player->smoke[arg1].Timer * (((Player->Speed / 18.0f) * 216.0f) / 15.0f)),
                      -Player->smoke[arg1].ang, 0);
    }
    Player->smoke[arg1].trx = Player->Position[0] + sp3C;
    Player->smoke[arg1].trz = Player->Position[2] + sp38;
    Player->smoke[arg1].try = (Player->Position[1] - Player->Radius) + sp34;
    Player->smoke[arg1].Timer++;
    if (Player->smoke[arg1].Timer == 6) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }
}

void RandomRing(Player* Player, s16 arg1, s8 Arg2, UNUSED s8 Arg3) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;

    sp40 = WaterlevelPlayer[Arg2] - Player->Position[1] - 3.0f;
    if (((Player->WaterFlag & SUBMERGED) != 0) && (CourseID != COURSE_KOOPA_BEACH)) {
        sp40 = WaterlevelPlayer[Arg2] - Player->Position[1] + 0.1;
    }

    MakeSmokepos(&sp44, &sp40, &sp3C, 0.0f, sp40,
                  -4.0f + ((-Player->Gass[arg1].Timer * (Player->Speed / 18.0f) * 216.0f) / 10.0f),
                  -Player->Gass[arg1].ang, 2 * -Player->Kartroll);
    Player->Gass[arg1].trx = Player->Position[0] + sp44;
    Player->Gass[arg1].trz = Player->Position[2] + sp3C;
    Player->Gass[arg1].try = Player->Position[1] + sp40;
    ++Player->Gass[arg1].Timer;
    if ((Player->Gass[arg1].Timer == 12) || (WaterlevelPlayer[Arg2] <= (Player->Position[1] - Player->Radius))) {
        Player->Gass[arg1].Flag = 0;
        Player->Gass[arg1].Timer = 0;
        Player->Gass[arg1].Number = NO_PARTICLE;
    }
    Player->Gass[arg1].dev = 2.0f;
    Player->Gass[arg1].Scale -= 0.35;
    if (Player->Gass[arg1].Scale < 0.0f) {
        Player->Gass[arg1].Scale = 0.0f;
    }

    Player->Gass[arg1].Swork4 -= 22;
    if (Player->Gass[arg1].Swork4 <= 0) {
        Player->Gass[arg1].Swork4 = 0;
    }
}

void SpinSmoke(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    Player->smoke[arg1].trz =
        Player->Position[2] + (-1.2 * Player->smoke[arg1].Timer * CosT(Player->smoke[arg1].ang));
    Player->smoke[arg1].trx =
        Player->Position[0] + (-1.2 * Player->smoke[arg1].Timer * SinT(Player->smoke[arg1].ang));
    Player->smoke[arg1].try = Player->smoke[arg1].try + 0.5;

    ++Player->smoke[arg1].Timer;
    if (Player->smoke[arg1].Timer == 10) {
        Player->smoke[arg1].Flag = 0;
        Player->smoke[arg1].Timer = 0;
        Player->smoke[arg1].Number = NO_PARTICLE;
    }

    Player->smoke[arg1].Scale += 0.2;
    Player->smoke[arg1].Swork4 -= 8;
    if (Player->smoke[arg1].Swork4 <= 0) {
        Player->smoke[arg1].Swork4 = 0;
    }
}

void StarDust(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    f32 thing2;
    UNUSED s32 stackPadding0;
    s32 thing;
    UNUSED s32 stackPadding1;

    if (Player->Spark[arg1].Timer >= 9) {
        Player->Spark[arg1].Timer = 9;
    }
    thing2 = Player->Spark[arg1].Fwork1;
    thing = Player->Spark[arg1].Timer;
    Player->Spark[arg1].trz = Player->Position[2] + (CosT(Player->Spark[arg1].ang) * (-0.7 * thing));
    Player->Spark[arg1].trx = Player->Position[0] + (SinT(Player->Spark[arg1].ang) * (-0.7 * thing));
    Player->Spark[arg1].Timer++;
    Player->Spark[arg1].try =
        Player->Spark[arg1].Fwork2 + (f32) ((thing * thing2) - (0.2 * (thing * thing)));
    if (Player->Spark[arg1].Timer == 0x000A) {
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }
    if (Player->Spark[arg1].Timer >= 7) {
        Player->Spark[arg1].Swork4 -= 0x60;
        if (Player->Spark[arg1].Swork4 <= 0) {
            Player->Spark[arg1].Swork4 = 0;
        }
    }
}

void ResultStar(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    f32 temp_f4;
    UNUSED s32 stackPadding0;
    s32 temp_v1;
    UNUSED s32 stackPadding1;

    temp_v1 = Player->Spark[arg1].Timer;
    temp_f4 = Player->Spark[arg1].Fwork1;
    Player->Spark[arg1].trz =
        Player->Position[2] + (CosT(Player->Spark[arg1].ang) * (-0.6 * temp_v1));
    Player->Spark[arg1].trx =
        Player->Position[0] + (SinT(Player->Spark[arg1].ang) * (-0.6 * temp_v1));
    Player->Spark[arg1].Timer++;
    Player->Spark[arg1].try =
        Player->Spark[arg1].Fwork2 + (f32) ((temp_v1 * temp_f4) - (0.1 * (temp_v1 * temp_v1)));
    if (Player->Spark[arg1].Timer == 0x0019) {
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }
    if (Player->Spark[arg1].Timer >= 7) {
        Player->Spark[arg1].Swork4 -= 0x6;
        if (Player->Spark[arg1].Swork4 <= 0) {
            Player->Spark[arg1].Swork4 = 0;
        }
    }
}

void LandSmoke(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {

    ++Player->Spark[arg1].Timer;
    Player->Spark[arg1].trz =
        Player->Position[2] + ((-0.8 * (Player->Spark[arg1].Timer)) * CosT(Player->Spark[arg1].ang));
    Player->Spark[arg1].trx =
        Player->Position[0] + ((-0.8 * (Player->Spark[arg1].Timer)) * SinT(Player->Spark[arg1].ang));
    Player->Spark[arg1].try = (Player->Ground + 2.0f);

    if (Player->Spark[arg1].Timer == 14) {
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }

    Player->Spark[arg1].Swork4 -= 12;
    if (Player->Spark[arg1].Swork4 <= 0) {
        Player->Spark[arg1].Swork4 = 0;
    }
}

void BurnDust(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    ++Player->Spark[arg1].Timer;
    Player->Spark[arg1].Scale -= 0.06;
    Player->Spark[arg1].try += 0.1;
    Player->Spark[arg1].Swork4 -= 12;

    if (Player->Spark[arg1].Swork4 <= 0) {
        Player->Spark[arg1].Swork4 = 0;
    }

    if (Player->Spark[arg1].Timer == 10) {
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }
}

void WaterDust(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    ++Player->Spark[arg1].Timer;
    Player->Spark[arg1].try -= 0.3;

    if (Player->Spark[arg1].Timer == 10) {
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }
}

void TailDust(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    f32 temp;
    temp = Player->Spark[arg1].dev;

    Player->Spark[arg1].trz =
        Player->SparkZ + (((-temp) * Player->Spark[arg1].Timer) * CosT(Player->Spark[arg1].ang));
    Player->Spark[arg1].trx =
        Player->SparkX + (((-temp) * Player->Spark[arg1].Timer) * SinT(Player->Spark[arg1].ang));
    Player->Spark[arg1].try = Player->Position[1] + Player->Spark[arg1].Type;
    Player->Spark[arg1].Scale += 0.04;

    ++Player->Spark[arg1].Timer;
    if (Player->Spark[arg1].Timer == 12) {
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }

    if (Player->Spark[arg1].Timer >= 9) {
        Player->Spark[arg1].Swork4 -= 0x10;
        if (Player->Spark[arg1].Swork4 <= 0) {
            Player->Spark[arg1].Swork4 = 0;
        }
    }
}

void PowderDust(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {
    f32 temp;
    temp = Player->Spark[arg1].dev * 1.2;

    Player->Spark[arg1].trz =
        (Player->Position[2] + (-temp * Player->Spark[arg1].Timer) * (CosT(Player->Spark[arg1].ang)));
    Player->Spark[arg1].trx =
        (Player->Position[0] + (-temp * Player->Spark[arg1].Timer) * (SinT(Player->Spark[arg1].ang)));
    Player->Spark[arg1].try += 0.1;

    ++Player->Spark[arg1].Timer;
    if (Player->Spark[arg1].Timer == 10) {
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }

    Player->Spark[arg1].Swork1 += DEGREES(10);
    if (Player->Spark[arg1].Timer >= 6) {
        Player->Spark[arg1].Swork4 -= 16;
        if (Player->Spark[arg1].Swork4 <= 0) {
            Player->Spark[arg1].Swork4 = 0;
        }
    }
}

void SlipStream(Player* Player, s16 arg1, UNUSED s8 Arg2, UNUSED s8 Arg3) {

    if (Player->Spark[arg1].Lrflag == 1) {
        Player->Spark[arg1].ang += DEGREES(12);
    } else {
        Player->Spark[arg1].ang -= DEGREES(12);
    }

    Player->Spark[arg1].trz =
        Player->Position[2] + (CosT(Player->Spark[arg1].ang - Player->Direction[1] - Player->Slipang) * 5.0f);
    Player->Spark[arg1].trx =
        Player->Position[0] + (SinT(Player->Spark[arg1].ang - Player->Direction[1] - Player->Slipang) * 5.0f);
    Player->Spark[arg1].try = Player->Position[1] - 1.0f;
    Player->Spark[arg1].Scale += 0.4;
    ++Player->Spark[arg1].Timer;

    if (Player->Spark[arg1].Timer == 10) {
        Player->Spark[arg1].Timer = 0;
        Player->Spark[arg1].Flag = 0;
        Player->Spark[arg1].Number = NO_PARTICLE;
    }
    if (Player->Spark[arg1].Timer >= 5) {
        Player->Spark[arg1].Swork4 -= 20;
        if (Player->Spark[arg1].Swork4 <= 0) {
            Player->Spark[arg1].Swork4 = 0;
        }
    }
}

void CrashLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {

    Player->Letter[Index].try = Player->Position[1];
    ++Player->Letter[Index].Timer;

    if (Player->Letter[Index].Timer == 9) {
        Player->Talk &= ~CRASH;
        Player->Letter[Index].Flag = 0;
        Player->Letter[Index].Timer = 0;
        Player->Letter[Index].Number = NO_PARTICLE;
    }

    Player->Letter[Index].Scale += 0.8;
    if (Player->Letter[Index].Scale >= (f64) 2.5) {
        Player->Letter[Index].Scale = 2.5f;
    }
}

void HitLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    ++Player->Letter[Index].Timer;
    if (Player->Letter[Index].Timer < 4) {
        Player->Letter[Index].Scale += 1.2;
        if (Player->Letter[Index].Scale >= 3.5) {
            Player->Letter[Index].Scale = 3.5f;
        }
    } else {
        Player->Letter[Index].Scale -= 1.8;
        if (Player->Letter[Index].Scale <= 0.0f) {
            Player->Talk &= ~FLASH;
            Player->Letter[Index].Flag = 0;
            Player->Letter[Index].Timer = 0;
            Player->Letter[Index].Number = NO_PARTICLE;
        }
    }
}

void BoingLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    ++Player->Letter[Index].Timer;
    Player->Letter[Index].Scale += 0.15;

    if (1.2 <= Player->Letter[Index].Scale) {
        Player->Letter[Index].Scale = 1.2f;
    }
    if (Player->Letter[Index].Timer >= 12) {
        Player->Talk &= ~BOING;
        Player->Letter[Index].Flag = 0;
        Player->Letter[Index].Timer = 0;
        Player->Letter[Index].Number = NO_PARTICLE;
    }
}

void PoompLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    ++Player->Letter[Index].Timer;

    Player->Letter[Index].try += 0.8;
    Player->Letter[Index].Scale += 0.4;
    if (Player->Letter[Index].Scale >= (f64) 1.5) {
        Player->Letter[Index].Scale = 1.5f;
    }

    if (Player->Letter[Index].Timer >= 12) {
        Player->Talk &= ~POOMP;
        Player->Letter[Index].Flag = 0;
        Player->Letter[Index].Timer = 0;
        Player->Letter[Index].Number = NO_PARTICLE;
    }
}

void SpinLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    Player->Letter[Index].trz = (f32) Player->Position[2];
    Player->Letter[Index].trx = (f32) Player->Position[0];
    Player->Letter[Index].try = (f32) (Player->Position[1] + 4.0f);
    if ((Player->SlipFlag & SPIN_L) == SPIN_L) {
        Player->Letter[Index].ang += 26 * DEGREES(1);
    } else {
        Player->Letter[Index].ang -= 26 * DEGREES(1);
    }

    if (((Player->SlipFlag & SPIN_L) != SPIN_L) &&
        ((Player->SlipFlag & SPIN_R) != SPIN_R)) {
        Player->Letter[Index].Flag = 0;
        Player->Letter[Index].Timer = 0;
        Player->Letter[Index].Number = NO_PARTICLE;
    }

    Player->Letter[Index].Scale += 0.08;
    if (Player->Letter[Index].Scale >= 1.5) {
        Player->Letter[Index].Scale = 1.5f;
    }
}

void OnpuLetter(Player* Player, UNUSED s8 arg1, UNUSED s8 Arg2, s8 Index) {
    ++Player->Letter[Index].Timer;
    if (Player->Letter[Index].Timer < 8) {
        Player->Letter[Index].Scale += 0.2;
        if (1.2 <= Player->Letter[Index].Scale) {
            Player->Letter[Index].Scale = 1.2f;
        }
    } else {
        Player->Letter[Index].Scale -= 0.4;
        if (Player->Letter[Index].Scale <= 0.0f) {
            Player->Talk &= ~MUSIC_NOTE;
            Player->Letter[Index].Flag = 0;
            Player->Letter[Index].Timer = 0;
            Player->Letter[Index].Number = NO_PARTICLE;
        }
    }
}

void CommonMTXCalc(Vec3f arg0, Vec3s arg1, f32 Scale) {
    Mat4 sp20;

    Createaffine(sp20, arg0, arg1);
    ScaleMatrix(sp20, Scale);
    Affine2Mtx(&DynamicP->MTXEffect[Effectcount], sp20);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void SetGassCalc(Player* Player, s8 playerIndex, s16 Arg2, s8 Arg3) {
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

    if (Player->Gass[Arg2].Flag == 1) {
        spB4[0] = Player->Gass[Arg2].trx;
        spB4[1] = Player->Gass[Arg2].try;
        spB4[2] = Player->Gass[Arg2].trz;
        spAC[0] = 0;
        spAC[1] = Player->AngleY[Arg3];
        spAC[2] = 0;
        if ((Player->SlipFlag & STAR) &&
            (((s32) GameTimer - StarUseCounter[playerIndex]) < STAR_EFFECT_DURATION - 1)) {
            primRed = (primColors[1] >> 0x10) & 0xFF;
            primGreen = (primColors[1] >> 0x08) & 0xFF;
            primBlue = (primColors[1] >> 0x00) & 0xFF;
            envRed = (envColors[1] >> 0x10) & 0xFF;
            envGreen = (envColors[1] >> 0x08) & 0xFF;
            envBlue = (envColors[1] >> 0x00) & 0xFF;
            primAlpha = Player->Gass[Arg2].Swork4;
            CommonMTXCalc(spB4, spAC, ((Player->Gass[Arg2].Scale * Player->Offsetsize) * 1.4));
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, common_texture_particle_smoke[Player->Gass[Arg2].Lrflag],
                                G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetAlphaCompare(DisplayListHead++, G_AC_DITHER);
            gSPDisplayList(DisplayListHead++, D_0D008E48);
        } else {
            primRed = (primColors[Player->Gass[Arg2].Swork1] >> 0x10) & 0xFF;
            primGreen = (primColors[Player->Gass[Arg2].Swork1] >> 0x08) & 0xFF;
            primBlue = (primColors[Player->Gass[Arg2].Swork1] >> 0x00) & 0xFF;
            envRed = (envColors[Player->Gass[Arg2].Swork1] >> 0x10) & 0xFF;
            envGreen = (envColors[Player->Gass[Arg2].Swork1] >> 0x08) & 0xFF;
            envBlue = (envColors[Player->Gass[Arg2].Swork1] >> 0x00) & 0xFF;
            primAlpha = Player->Gass[Arg2].Swork4;
            CommonMTXCalc(spB4, spAC, Player->Gass[Arg2].Scale * Player->Offsetsize);
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, common_texture_particle_smoke[Player->Gass[Arg2].Lrflag],
                                G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gSPDisplayList(DisplayListHead++, D_0D008E48);
        }
        Effectcount += 1;
    }
}

void SetTeresaCalc(Player* Player, UNUSED s8 playerIndex, s16 Arg2, s8 Arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 Green;
    s16 Blue;
    s16 Alpha;

    if (Player->Gass[Arg2].Flag == 1) {
        red = Player->Gass[Arg2].Swork1;
        Green = Player->Gass[Arg2].Swork2;
        Blue = Player->Gass[Arg2].Swork3;
        Alpha = Player->Gass[Arg2].Swork4;
        sp54[0] = Player->Gass[Arg2].trx;
        sp54[1] = Player->Gass[Arg2].try;
        sp54[2] = Player->Gass[Arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = Player->AngleY[Arg3];
        sp4C[2] = 0;
        CommonMTXCalc(sp54, sp4C, Player->Gass[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, Green, Blue, Alpha);
        gSPDisplayList(DisplayListHead++, D_0D008E48);
        Effectcount += 1;
    }
}

void SetDriftCalc(Player* Player, UNUSED s8 playerIndex, s16 Arg2, s8 Arg3) {
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
    if (Player->smoke[Arg2].Flag == 1) {
        if (Player->DriftCount >= 50) {
            var_s0 = 1;
        } else {
            var_s0 = 0;
        }
        primRed = Player->smoke[Arg2].Swork1;
        primGreen = Player->smoke[Arg2].Swork2;
        primBlue = Player->smoke[Arg2].Swork3;
        primAlpha = Player->smoke[Arg2].Swork4;
        envRed = (sp8C[Player->smoke[Arg2].Swork5] >> 0x10) & 0xFF;
        envGreen = (sp8C[Player->smoke[Arg2].Swork5] >> 0x08) & 0xFF;
        envBlue = (sp8C[Player->smoke[Arg2].Swork5] >> 0x00) & 0xFF;
        spB4[0] = Player->smoke[Arg2].trx;
        spB4[1] = Player->smoke[Arg2].try;
        spB4[2] = Player->smoke[Arg2].trz;
        spAC[0] = 0;
        spAC[1] = Player->AngleY[Arg3];
        spAC[2] = 0;
        CommonMTXCalc(spB4, spAC, Player->smoke[Arg2].Scale * Player->Offsetsize);
        if (var_s0 == 0) {
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, *D_800E4770[var_s0], G_IM_FMT_I, G_IM_SIZ_8b, 16, 16, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(DisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(DisplayListHead++, D_0D008DF8);
        } else {
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, *D_800E4770[var_s0], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
            gDPSetRenderMode(DisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
            gSPDisplayList(DisplayListHead++, D_0D008E48);
        }
        Effectcount += 1;
    }
}

void SetSmokeCalc(Player* Player, UNUSED s8 playerIndex, s16 Arg2, s8 Arg3) {
    Vec3f pos;
    Vec3s spD4;
    s16 primRed;
    s16 primGreen;
    s16 primBlue;
    s16 primAlpha;
    s16 envRed;
    s16 envGreen;
    s16 envBlue;

    if ((Player->smoke[Arg2].Flag == 1) && (Player->smoke[Arg2].Timer != 0)) {
        pos[0] = Player->smoke[Arg2].trx;
        pos[1] = Player->smoke[Arg2].try;
        pos[2] = Player->smoke[Arg2].trz;
        spD4[0] = 0;
        spD4[1] = Player->AngleY[Arg3];
        spD4[2] = 0;
        CommonMTXCalc(pos, spD4, Player->smoke[Arg2].Scale * Player->Offsetsize);
        if ((s32) Player->smoke[Arg2].Type != GRASS) {
            primRed =
                ((NORM_SMOKE_IN[Player->smoke[Arg2].Swork1] >> 0x10) & 0xFF) - Player->smoke[Arg2].Swork2;
            primGreen =
                ((NORM_SMOKE_IN[Player->smoke[Arg2].Swork1] >> 0x08) & 0xFF) - Player->smoke[Arg2].Swork2;
            primBlue =
                ((NORM_SMOKE_IN[Player->smoke[Arg2].Swork1] >> 0x00) & 0xFF) - Player->smoke[Arg2].Swork2;
            envRed = ((NORM_SMOKE_OUT[Player->smoke[Arg2].Swork1] >> 0x10) & 0xFF) - Player->smoke[Arg2].Swork2;
            envGreen =
                ((NORM_SMOKE_OUT[Player->smoke[Arg2].Swork1] >> 0x08) & 0xFF) - Player->smoke[Arg2].Swork2;
            envBlue =
                ((NORM_SMOKE_OUT[Player->smoke[Arg2].Swork1] >> 0x00) & 0xFF) - Player->smoke[Arg2].Swork2;
            primAlpha = Player->smoke[Arg2].Swork4;
            if (Player->smoke[Arg2].Swork5 == 0) {
                gSPDisplayList(DisplayListHead++, D_0D008DB8);
                gDPLoadTextureBlock(DisplayListHead++, smoke, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
                Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
                gSPDisplayList(DisplayListHead++, D_0D008E48);
            } else {
                gSPDisplayList(DisplayListHead++, D_0D008DB8);
                gDPLoadTextureBlock(DisplayListHead++, smoke, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                    G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                    G_TX_NOLOD, G_TX_NOLOD);
                Color2Mode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, primAlpha);
                gDPSetAlphaCompare(DisplayListHead++, G_AC_DITHER);
                gSPDisplayList(DisplayListHead++, D_0D008E48);
            }
        } else {
            primRed = Player->smoke[Arg2].Swork1;
            primGreen = Player->smoke[Arg2].Swork2;
            primBlue = Player->smoke[Arg2].Swork3;
            gSPDisplayList(DisplayListHead++, D_0D008C90);
            gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
            gDPLoadTextureBlock(DisplayListHead++, Grs, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            ToumeIMode(primRed, primGreen, primBlue, 0x000000FF);
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
            gSPVertex(DisplayListHead++, D_800E8C00, 4, 0);
            gSPDisplayList(DisplayListHead++, D_0D008DA0);
        }
        Effectcount += 1;
    }
}

void SetSplCalc(Player* Player, UNUSED s8 playerIndex, s16 Arg2, s8 Arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 Green;
    s16 Blue;
    s16 Alpha;

    if (Player->smoke[Arg2].Flag == 1) {
        red = Player->smoke[Arg2].Swork1;
        Green = Player->smoke[Arg2].Swork2;
        Blue = Player->smoke[Arg2].Swork3;
        Alpha = Player->smoke[Arg2].Swork4;
        sp54[0] = Player->smoke[Arg2].trx;
        sp54[1] = Player->smoke[Arg2].try;
        sp54[2] = Player->smoke[Arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = Player->AngleY[Arg3];
        sp4C[2] = 0;
        CommonMTXCalc(sp54, sp4C, Player->smoke[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPSetAlphaCompare(DisplayListHead++, G_AC_DITHER);
        gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, Green, Blue, Alpha);
        gSPDisplayList(DisplayListHead++, D_0D008E48);
        Effectcount += 1;
    }
}

void SetTurboCalc(Player* Player, UNUSED s32 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 red;
    s16 Green;
    s16 Blue;
    s16 Alpha;

    if (Player->smoke[Arg2].Flag == 1) {
        red = Player->smoke[Arg2].Swork1;
        Green = Player->smoke[Arg2].Swork2;
        Blue = Player->smoke[Arg2].Swork3;
        Alpha = Player->smoke[Arg2].Swork4;
        sp5C[0] = Player->smoke[Arg2].trx;
        sp5C[1] = Player->smoke[Arg2].try;
        sp5C[2] = Player->smoke[Arg2].trz;
        sp54[0] = 0;
        sp54[1] = Player->AngleY[Arg3];
        sp54[2] = 0;
        CommonMTXCalc(sp5C, sp54, Player->smoke[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008C90);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(DisplayListHead++, common_texture_particle_fire, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(red, Green, Blue, Alpha);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
        gSPVertex(DisplayListHead++, D_800E8B00, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void SetRingCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 Green;
    s16 Blue;
    s16 Alpha;

    if (Player->Gass[Arg2].Flag == 1) {
        red = Player->Gass[Arg2].Swork1;
        Green = Player->Gass[Arg2].Swork2;
        Blue = Player->Gass[Arg2].Swork3;
        Alpha = Player->Gass[Arg2].Swork4;
        sp54[0] = Player->Gass[Arg2].trx;
        sp54[1] = Player->Gass[Arg2].try;
        sp54[2] = Player->Gass[Arg2].trz;
        sp4C[0] = 0x4000;
        sp4C[1] = Player->AngleY[Arg3];
        sp4C[2] = 0;
        CommonMTXCalc(sp54, sp4C, Player->Gass[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, Green, Blue, Alpha);
        gSPDisplayList(DisplayListHead++, D_0D008E48);
        Effectcount += 1;
    }
}

void SetThunderCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f spDC;
    Vec3s spD4;
    UNUSED s32 stackPadding;

    if ((Player->Gass[Arg2].Flag == 1) && (Player->Gass[Arg2].Swork1 != 0x00FF)) {

        if (Player->Bump.DistanceZX >= 300.0f) {
            spDC[1] = Player->Position[1] + 5.0f;
        } else {
            spDC[1] = Player->Position[1] - 3.0f;
        }
        spDC[2] = Player->Position[2] + (CosT(Player->AngleY[Arg3]) * -10.0f);
        spDC[0] = Player->Position[0] + (SinT(Player->AngleY[Arg3]) * -10.0f);
        if (Player->Bump.DistanceZX >= 300.0f) {
            spD4[0] = Cameras[Arg3].CameraDirection[0] - DEGREES(90);
        } else {
            spD4[0] = 0;
        }
        spD4[1] = Player->AngleY[Arg3];
        spD4[2] = 0;
        CommonMTXCalc(spDC, spD4, Player->Gass[Arg2].Scale * Player->Offsetsize);
        if (Player->Gass[Arg2].Swork1 == 0) {
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            gDPLoadTextureBlock(DisplayListHead++, ThunderA, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(DisplayListHead++, &D_800E8900[0][Player->Gass[Arg2].Swork1], 4, 0);
            gSPDisplayList(DisplayListHead++, common_square_plain_render);
            gDPLoadTextureBlock(DisplayListHead++, ThunderB, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(DisplayListHead++, &D_800E8900[1][Player->Gass[Arg2].Swork1], 4, 0);
            gSPDisplayList(DisplayListHead++, D_0D008DA0);
        } else {
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            gDPLoadTextureBlock(DisplayListHead++, ThunderB, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(DisplayListHead++, &D_800E8900[0][Player->Gass[Arg2].Swork1], 4, 0);
            gSPDisplayList(DisplayListHead++, common_square_plain_render);
            gDPLoadTextureBlock(DisplayListHead++, ThunderA, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPVertex(DisplayListHead++, &D_800E8900[1][Player->Gass[Arg2].Swork1], 4, 0);
            gSPDisplayList(DisplayListHead++, D_0D008DA0);
        }
        Effectcount += 1;
    }
}

void SetSlipstreamCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp7C;
    Vec3s sp74;
    s16 red;
    s16 Green;
    s16 Blue;
    s16 Alpha;

    if (Player->Spark[Arg2].Flag == 1) {
        red = Player->Spark[Arg2].Swork1;
        Green = Player->Spark[Arg2].Swork2;
        Blue = Player->Spark[Arg2].Swork3;
        Alpha = Player->Spark[Arg2].Swork4;
        sp7C[0] = Player->Spark[Arg2].trx;
        sp7C[1] = Player->Spark[Arg2].try;
        sp7C[2] = Player->Spark[Arg2].trz;
        sp74[0] = -0x071C; // DEGREES(10)?
        sp74[2] = 0;
        if (Player->Spark[Arg2].Lrflag == 1) {
            sp74[1] = Player->AngleY[Arg3] - 0x2000;
            CommonMTXCalc(sp7C, sp74, Player->Spark[Arg2].Scale * Player->Offsetsize);
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            ToumeIMode(red, Green, Blue, Alpha);
            gSPDisplayList(DisplayListHead++, D_0D008E70);
        } else {
            sp74[1] = Player->AngleY[Arg3] + 0x2000;
            CommonMTXCalc(sp7C, sp74, Player->Spark[Arg2].Scale * Player->Offsetsize);
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            ToumeIMode(red, Green, Blue, Alpha & 0xFFFFFFFF); // huh?
            gSPDisplayList(DisplayListHead++, D_0D008E48);
        }
        Effectcount += 1;
    }
}

void SetTurbostreamCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp8C;
    Vec3s sp84;
    UNUSED s32 stackPadding[4];

    if (Player->Spark[Arg2].Flag == 1) {
        sp8C[0] = Player->Spark[Arg2].trx;
        sp8C[1] = Player->Spark[Arg2].try;
        sp8C[2] = Player->Spark[Arg2].trz;
        sp84[0] = 0;
        sp84[1] = Player->AngleY[Arg3];
        sp84[2] = 0;
        CommonMTXCalc(sp8C, sp84, Player->Spark[Arg2].Scale * Player->Offsetsize);
        if (Player->Spark[Arg2].Lrflag == 1) {
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, common_texture_particle_spark, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(0x000000FF, 0x000000FF, 0x000000DF, 0x000000FF, 0x0000005F, 0, 0x00000060);
            gSPDisplayList(DisplayListHead++, D_0D008E70);
        } else {
            gSPDisplayList(DisplayListHead++, D_0D008DB8);
            gDPLoadTextureBlock(DisplayListHead++, common_texture_particle_spark, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                                G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            Color2Mode(0x000000FF, 0x000000FF, 0x000000DF, 0x000000FF, 0x0000005F, 0, 0x00000060);
            gSPDisplayList(DisplayListHead++, D_0D008E48);
        }
        Effectcount += 1;
    }
}

void SetSpinCalc(Player* Player, UNUSED s8 arg1, f32 Arg2, UNUSED s8 Arg3, s8 arg4) {
    Vec3f sp9C;
    Vec3s sp94;
    UNUSED s32 stackPadding[2];

    if (Player->Letter[arg4].Flag == 1) {
        sp9C[0] = Player->Letter[arg4].trx;
        sp9C[1] = Player->Letter[arg4].try;
        sp9C[2] = Player->Letter[arg4].trz;
        sp94[0] = 0;
        sp94[1] = Player->Letter[arg4].ang;
        sp94[2] = 0;
        CommonMTXCalc(sp9C, sp94, Player->Offsetsize * Arg2);
        gSPDisplayList(DisplayListHead++, D_0D008C90);
        gSPClearGeometryMode(DisplayListHead++, G_CULL_BOTH);
        gDPLoadTLUT_pal256(DisplayListHead++, LetterPAL);
        gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(DisplayListHead++, Whir1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E8840, 4, 0);
        gSPDisplayList(DisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(DisplayListHead++, Whir2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E8800, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void SetFukidashiCalc(Player* Player, s8 arg1, u8* Texture, s8 Arg3, f32 arg4, s32 arg5) {
    Vec3f sp7C;
    Vec3s sp74;
    f32 sp54[8] = { 0.0f, -1.2f, 0.1f, 1.2f, -1.7f, -0.8f, -0.2f, -1.9f };
    // ????????????????????????????????????????
    s16 red = ((arg5 >> 0x10) & 0xFF) & 0xFF;
    s16 Green = ((arg5 >> 0x08) & 0xFF) & 0xFF;
    s16 Blue = ((arg5 >> 0x00) & 0xFF) & 0xFF;
    // ????????????????????????????????????????

    if (Player->Letter[Arg3].Flag == 1) {
        sp74[0] = 0;
        sp74[1] = Player->AngleY[arg1];
        sp74[2] = 0;
        sp7C[0] = Player->Position[0] + (SinT((0x4000 & 0xFFFFFFFF) - (Player->Direction[1] + Player->Slipang)) * arg4);
        sp7C[1] = Player->Position[1] + Player->Radius - sp54[Player->Kart] - 2.0f;
        sp7C[2] = Player->Position[2] + (CosT((0x4000 & 0xFFFFFFFF) - (Player->Direction[1] + Player->Slipang)) * arg4);
        CommonMTXCalc(sp7C, sp74, Player->Letter[Arg3].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        SplashMode(red, Green, Blue, 0x000000FF);
        gSPDisplayList(DisplayListHead++, D_0D008E20);
        Effectcount += 1;
    }
}

void SetWordCalc(Player* Player, s8 arg1, u8* Texture, s8 Arg3, f32 arg4, s32 arg5) {
    Vec3f sp7C;
    Vec3s sp74;
    f32 sp54[8] = { -0.7f, -1.9f, -0.6f, 0.4f, -2.5f, -1.6f, -0.95f, -2.7f };
    // ????????????????????????????????????????
    s16 red = ((arg5 >> 0x10) & 0xFF) & 0xFF;
    s16 Green = ((arg5 >> 0x08) & 0xFF) & 0xFF;
    s16 Blue = ((arg5 >> 0x00) & 0xFF) & 0xFF;
    // ????????????????????????????????????????

    if (Player->Letter[Arg3].Flag == 1) {
        sp74[0] = 0;
        sp74[1] = Player->AngleY[arg1];
        sp74[2] = 0;
        sp7C[0] = Player->Position[0] + (SinT((0x4000 & 0xFFFFFFFF) - (Player->Direction[1] + Player->Slipang)) * arg4);
        sp7C[1] = Player->Position[1] + Player->Radius - sp54[Player->Kart] - 2.0f;
        sp7C[2] = Player->Position[2] + (CosT((0x4000 & 0xFFFFFFFF) - (Player->Direction[1] + Player->Slipang)) * arg4);
        CommonMTXCalc(sp7C, sp74, Player->Letter[Arg3].Scale * Player->Offsetsize * 0.8);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(DisplayListHead++, Texture, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        SplashMode(red, Green, Blue, 0x000000FF);
        gSPDisplayList(DisplayListHead++, D_0D008E20);
        Effectcount += 1;
    }
}

void SetCrashCalc(Player* Player, UNUSED s8 arg1, UNUSED f32 Arg2, s8 Arg3, s8 arg4) {
    UNUSED s32 stackPadding[16]; // huh?
    Vec3f sp9C;
    Vec3s sp94;

    if (Player->Letter[arg4].Flag == 1) {
        sp9C[1] = Player->Letter[arg4].try;
        sp9C[2] = Player->Position[2] + (CosT(Player->AngleY[Arg3]) * -10.0f);
        sp9C[0] = Player->Position[0] + (SinT(Player->AngleY[Arg3]) * -10.0f);
        sp94[0] = 0;
        sp94[1] = Player->AngleY[Arg3];
        sp94[2] = 0;
        CommonMTXCalc(sp9C, sp94, Player->Letter[arg4].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008C90);
        gDPLoadTLUT_pal256(DisplayListHead++, LetterPAL);
        gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(DisplayListHead++, Crash1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E8880, 4, 0);
        gSPDisplayList(DisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(DisplayListHead++, Crash2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E88C0, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void SetHitCalc(Player* Player, UNUSED s8 arg1, UNUSED f32 Arg2, s8 Arg3, s8 arg4) {
    UNUSED s32 stackPadding[16]; // huh?
    Vec3f sp84;
    Vec3s sp7C;

    if (Player->Letter[arg4].Flag == 1) {
        sp84[1] = Player->Position[1] - 3.0f;
        sp84[2] = Player->Position[2] + (CosT(Player->AngleY[Arg3]) * -10.0f);
        sp84[0] = Player->Position[0] + (SinT(Player->AngleY[Arg3]) * -10.0f);
        sp7C[0] = 0;
        sp7C[1] = Player->AngleY[Arg3];
        sp7C[2] = 0;
        CommonMTXCalc(sp84, sp7C, Player->Letter[arg4].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gDPLoadTextureBlock(DisplayListHead++, ThunderA, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E8A00, 4, 0);
        gSPDisplayList(DisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(DisplayListHead++, ThunderB, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E8A40, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void SetBoingCalc(Player* Player, UNUSED s8 arg1, UNUSED f32 Arg2, s8 Arg3, s8 arg4) {
    Vec3f sp64;
    Vec3s sp5C;

    if ((Player->Letter[arg4].Flag == 1) && (Player->dir[Arg3] < 0xD)) {
        sp64[1] = Player->Position[1] - 3.0f;
        sp64[2] = Player->Position[2] + ((-2.5 * Player->Letter[arg4].Timer) * CosT(Player->AngleY[Arg3]));
        sp64[0] = Player->Position[0] + ((-2.5 * Player->Letter[arg4].Timer) * SinT(Player->AngleY[Arg3]));
        sp5C[0] = 0;
        sp5C[1] = Player->AngleY[Arg3];
        sp5C[2] = 0;
        CommonMTXCalc(sp64, sp5C, Player->Letter[arg4].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(DisplayListHead++, Boing, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(DisplayListHead++, D_800E8B40, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void SetPoompCalc(Player* Player, UNUSED s8 arg1, UNUSED f32 Arg2, s8 Arg3, s8 arg4) {
    Vec3f sp9C;
    Vec3s sp94;

    if ((Player->Letter[arg4].Flag == 1) && ((s32) Player->dir[Arg3] < 0xD)) {
        sp9C[1] = (Player->Position[1] - 3.0f) + Player->Letter[arg4].try;
        sp9C[2] = Player->Position[2] + (CosT(Player->AngleY[Arg3]) * -10.0f);
        sp9C[0] = Player->Position[0] + (SinT(Player->AngleY[Arg3]) * -10.0f);
        sp94[0] = 0;
        sp94[1] = Player->AngleY[Arg3];
        sp94[2] = 0;
        CommonMTXCalc(sp9C, sp94, Player->Letter[arg4].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008C90);
        gDPLoadTLUT_pal256(DisplayListHead++, LetterPAL);
        gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
        gDPLoadTextureBlock(DisplayListHead++, Poomp1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E8B80, 4, 0);
        gSPDisplayList(DisplayListHead++, common_square_plain_render);
        gDPLoadTextureBlock(DisplayListHead++, Poomp2, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSPVertex(DisplayListHead++, D_800E8BC0, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void SetStarCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 Alpha;

    if (Player->Spark[Arg2].Flag == 1) {
        Alpha = Player->Spark[Arg2].Swork4;
        sp5C[0] = Player->Spark[Arg2].trx;
        sp5C[1] = Player->Spark[Arg2].try;
        sp5C[2] = Player->Spark[Arg2].trz;
        sp54[0] = 0;
        sp54[1] = Player->AngleY[Arg3];
        Player->Spark[Arg2].Swork2 += 0x1C71;
        sp54[2] = Player->Spark[Arg2].Swork2;
        CommonMTXCalc(sp5C, sp54, Player->Offsetsize * 0.5);
        gSPDisplayList(DisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(DisplayListHead++, Hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0xFF, 0xFF, 0, Alpha);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(DisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount++;
    }
}

void LandCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
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
    if (Player->Spark[Arg2].Flag == 1) {
        primRed = (NORM_SMOKE_IN[Player->Spark[Arg2].Swork1] >> 0x10) & 0xFF;
        primGreen = (NORM_SMOKE_IN[Player->Spark[Arg2].Swork1] >> 8) & 0xFF;
        primBlue = NORM_SMOKE_IN[Player->Spark[Arg2].Swork1] & 0xFF;

        envRed = (NORM_SMOKE_OUT[Player->Spark[Arg2].Swork1] >> 0x10) & 0xFF;
        envGreen = (NORM_SMOKE_OUT[Player->Spark[Arg2].Swork1] >> 8) & 0xFF;
        envBlue = NORM_SMOKE_OUT[Player->Spark[Arg2].Swork1] & 0xFF;
        primAlpha = Player->Spark[Arg2].Swork4;

        sp74[0] = Player->Spark[Arg2].trx;
        sp74[1] = Player->Spark[Arg2].try;
        sp74[2] = Player->Spark[Arg2].trz;
        sp6C[0] = 0;
        sp6C[1] = Player->AngleY[Arg3];
        sp6C[2] = 0;
        CommonMTXCalc(sp74, sp6C, Player->Offsetsize * 1.5);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(DisplayListHead++, smoke, G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        // `test` MUST be a u16
        // `test` MUST be set to `envRed`s Value
        // `test` MUST be typecast to s16 when passed as a function parameter
        // What. the. fuck.
        test = envRed;
        Color2Mode(primRed, primGreen, primBlue, (s16) test, envGreen, envBlue, primAlpha);
        gSPDisplayList(DisplayListHead++, D_0D008E48);
        Effectcount += 1;
    }
}

void SetStartailCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3, f32 arg4) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 Alpha;

    if (Player->Spark[Arg2].Flag == 1) {
        Alpha = Player->Spark[Arg2].Swork4;
        sp5C[0] = Player->Spark[Arg2].trx;
        sp5C[1] = Player->Spark[Arg2].try;
        sp5C[2] = Player->Spark[Arg2].trz;
        sp54[0] = 0;
        sp54[1] = Player->AngleY[Arg3];
        sp54[2] = 0;
        CommonMTXCalc(sp5C, sp54, Player->Offsetsize * arg4);
        gSPDisplayList(DisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(DisplayListHead++, Hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0, Alpha);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(DisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void PowderCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 Alpha;

    if (Player->Spark[Arg2].Flag == 1) {
        Alpha = Player->Spark[Arg2].Swork4;
        sp5C[0] = Player->Spark[Arg2].trx;
        sp5C[1] = Player->Spark[Arg2].try;
        sp5C[2] = Player->Spark[Arg2].trz;
        sp54[0] = 0;
        sp54[1] = Player->AngleY[Arg3];
        sp54[2] = Player->Spark[Arg2].Swork1;
        CommonMTXCalc(sp5C, sp54, Player->Spark[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(DisplayListHead++, Hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0, Alpha);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(DisplayListHead++, D_800E87C0, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void BurnCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp54;
    Vec3s sp4C;
    s16 red;
    s16 Green;
    s16 Blue;
    s16 Alpha;

    if (Player->Spark[Arg2].Flag == 1) {
        red = Player->Spark[Arg2].Swork1;
        Green = Player->Spark[Arg2].Swork2;
        Blue = Player->Spark[Arg2].Swork3;
        Alpha = Player->Spark[Arg2].Swork4;
        sp54[0] = Player->Spark[Arg2].trx;
        sp54[1] = Player->Spark[Arg2].try;
        sp54[2] = Player->Spark[Arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = Player->AngleY[Arg3];
        sp4C[2] = 0;
        CommonMTXCalc(sp54, sp4C, Player->Spark[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008DB8);
        gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        ToumeIMode(red, Green, Blue, Alpha);
        gSPDisplayList(DisplayListHead++, D_0D008E48);
        Effectcount += 1;
    }
}

void WaterCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    UNUSED s32 stackPadding[2];

    if (Player->Spark[Arg2].Flag == 1) {
        sp5C[0] = Player->Spark[Arg2].trx;
        sp5C[1] = Player->Spark[Arg2].try;
        sp5C[2] = Player->Spark[Arg2].trz;
        sp54[0] = 0;
        sp54[1] = Player->AngleY[Arg3];
        sp54[2] = 0;
        CommonMTXCalc(sp5C, sp54, Player->Spark[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(DisplayListHead++, Bubble, G_IM_FMT_IA, G_IM_SIZ_16b, 16, 16, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(DisplayListHead++, D_800E8740, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void SetBubbleCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp54;
    Vec3s sp4C;

    if (Player->Gass[Arg2].Flag == 1) {
        sp54[0] = Player->smoke[Arg2].trx;
        sp54[1] = Player->smoke[Arg2].try;
        sp54[2] = Player->smoke[Arg2].trz;
        sp4C[0] = 0;
        sp4C[1] = Player->AngleY[Arg3];
        sp4C[2] = 0;
        CommonMTXCalc(sp54, sp4C, Player->smoke[Arg2].Scale * Player->Offsetsize);

        gSPDisplayList(DisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock(DisplayListHead++, Bubble, G_IM_FMT_IA, G_IM_SIZ_16b, 16, 16, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                            G_TX_NOLOD, G_TX_NOLOD);
        ToumeIMode(0xFF, 0xFF, 0xFF, 0xFF);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(DisplayListHead++, D_0D008B78, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount++;
    }
}

void SetDobonCalc(Player* Player, UNUSED s8 arg1, s16 Arg2, s8 Arg3) {
    Vec3f sp5C;
    Vec3s sp54;
    s16 red;
    s16 Green;
    s16 Blue;

    if (Player->Gass[Arg2].Flag == 1) {
        red = Player->Gass[Arg2].Swork1;
        Green = Player->Gass[Arg2].Swork2;
        Blue = Player->Gass[Arg2].Swork3;
        sp5C[0] = Player->Gass[Arg2].trx;
        sp5C[1] = Player->Gass[Arg2].try;
        sp5C[2] = Player->Gass[Arg2].trz;
        sp54[0] = 0;
        sp54[1] = Player->AngleY[Arg3];
        sp54[2] = 0;
        CommonMTXCalc(sp5C, sp54, Player->Gass[Arg2].Scale * Player->Offsetsize);
        gSPDisplayList(DisplayListHead++, D_0D008D58);
        gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
        gDPLoadTextureBlock_4b(DisplayListHead++, *D_800E47A0[Player->Gass[Arg2].Timer], G_IM_FMT_I, 64, 64,
                               0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                               G_TX_NOLOD, G_TX_NOLOD);
        SplashMode(red, Green, Blue, 0x000000FF);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        gSPVertex(DisplayListHead++, D_800E8780, 4, 0);
        gSPDisplayList(DisplayListHead++, D_0D008DA0);
        Effectcount += 1;
    }
}

void BalloonDisp(Player* Player, f32 arg1, f32 Arg2, s8 playerIndex, s8 balloonIndex, s16 Rotation) {
    f32 someX;
    f32 someY;
    f32 someZ;

    PlayerBalloonStatus[playerIndex][balloonIndex] = BALLOON_STATUS_GONE;
    D_8018D650[playerIndex][balloonIndex] = 0.1f;
    D_8018D6B0[playerIndex][balloonIndex] = 0.0f;
    D_8018D710[playerIndex][balloonIndex] = 0.0f;
    D_8018D770[playerIndex][balloonIndex] = 0;
    D_8018D7A0[playerIndex][balloonIndex] = 0;
    D_8018D7D0[playerIndex][balloonIndex] = 0;
    D_8018D800[playerIndex][balloonIndex] = 5;
    D_8018D830[playerIndex][balloonIndex] = 1;
    D_8018D620[playerIndex][balloonIndex] = -Player->Direction[1] - Player->Slipang;
    MakeSmokepos(&someX, &someY, &someZ, arg1, 4.0f, Arg2 + -3.8, -Player->Direction[1], 0);
    PlayerBalloonPosX[playerIndex][balloonIndex] = Player->Position[0] + someX;
    PlayerBalloonPosZ[playerIndex][balloonIndex] = Player->Position[2] + someZ;
    PlayerBalloonPosY[playerIndex][balloonIndex] = (Player->Position[1] - Player->Radius) + someY;
    PlayerBalloonStatus[playerIndex][balloonIndex] |= BALLOON_STATUS_PRESENT;
    PlayerBalloonRotation[playerIndex][balloonIndex] = Rotation; // Sprite Rotation
    D_8018D890[playerIndex][balloonIndex] = 0;
}

void BalloonStrat(Player* Player, f32 arg1, f32 Arg2, s8 PlayerId, s8 balloonId) {
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

    sp6C = (-(Player->Speed / 18.0f) * 216.0f) / 10.0f;
    if ((PlayerBalloonStatus[PlayerId][balloonId] & 2) != 2) {
        D_8018D650[PlayerId][balloonId] += -0.003 + (-Player->Speed * 0.0006);
        if (D_8018D650[PlayerId][balloonId] >= 0.05) {
            D_8018D650[PlayerId][balloonId] = 0.05f;
        }
        if (D_8018D650[PlayerId][balloonId] <= -0.05) {
            D_8018D650[PlayerId][balloonId] = -0.05f;
        }
        D_8018D6B0[PlayerId][balloonId] += D_8018D650[PlayerId][balloonId];
        if (D_8018D6B0[PlayerId][balloonId] >= 0.06) {
            D_8018D6B0[PlayerId][balloonId] = 0.06f;
        }
        if (D_8018D6B0[PlayerId][balloonId] <= -0.06) {
            D_8018D6B0[PlayerId][balloonId] = -0.06f;
        }
        D_8018D710[PlayerId][balloonId] += D_8018D6B0[PlayerId][balloonId];
        if (D_8018D710[PlayerId][balloonId] < 0.0f) {
            D_8018D650[PlayerId][balloonId] = MakeRandomLimmit(0x000BU) / 10;
            D_8018D6B0[PlayerId][balloonId] = 0.0f;
            D_8018D710[PlayerId][balloonId] = 0.0f;
        }
        D_8018D620[PlayerId][balloonId] = -Player->Direction[1] - Player->Slipang;
        ChaseSnumber(&D_8018D890[PlayerId][balloonId], Player->Speed * (f32) DEGREES(1), 0.1f);
    }
    if (D_8018D830[PlayerId][balloonId] == 1) {
        D_8018D770[PlayerId][balloonId] += D_8018D800[PlayerId][balloonId] - Player->Speed;
    } else {
        D_8018D770[PlayerId][balloonId] += D_8018D800[PlayerId][balloonId] + Player->Speed;
    }
    if (D_8018D770[PlayerId][balloonId] >= 0xB) {
        D_8018D770[PlayerId][balloonId] = 0x000B;
    }
    if (D_8018D770[PlayerId][balloonId] < -0xA) {
        D_8018D770[PlayerId][balloonId] = -0x000B;
    }
    D_8018D7A0[PlayerId][balloonId] += D_8018D770[PlayerId][balloonId];
    if (D_8018D7A0[PlayerId][balloonId] >= 0x29) {
        D_8018D7A0[PlayerId][balloonId] = 0x0029;
    }
    if (D_8018D7A0[PlayerId][balloonId] < -0x28) {
        D_8018D7A0[PlayerId][balloonId] = -0x0029;
    }
    D_8018D7D0[PlayerId][balloonId] += D_8018D7A0[PlayerId][balloonId];
    if (D_8018D7D0[PlayerId][balloonId] >= 0x38E) { // 0x38E is DEGREES(5) possibly
        D_8018D800[PlayerId][balloonId] = -MakeRandomLimmit(8U);
        if (D_8018D830[PlayerId][balloonId] != 1) {
            D_8018D830[PlayerId][balloonId] = 1;
        }
    }
    if (D_8018D7D0[PlayerId][balloonId] < -0x38D) {
        D_8018D800[PlayerId][balloonId] = MakeRandomLimmit(8U);
        if (D_8018D830[PlayerId][balloonId] != -1) {
            D_8018D830[PlayerId][balloonId] = -1;
        }
    }
    MakeSmokepos(&someX, &someY, &someZ, arg1, sp80[Player->Kart] - D_8018D710[PlayerId][balloonId],
                  Arg2 + -3.2 + (sp6C * 1), -D_8018D620[PlayerId][balloonId], -Player->Kartroll * 2);
    if ((PlayerBalloonStatus[PlayerId][balloonId] & 2) != 2) {
        PlayerBalloonPosY[PlayerId][balloonId] = (Player->Position[1] - Player->Radius) + someY;
        PlayerBalloonPosX[PlayerId][balloonId] = Player->Position[0] + someX;
        PlayerBalloonPosZ[PlayerId][balloonId] = Player->Position[2] + someZ;
        PlayerBalloonDepartingTimer[PlayerId][balloonId] = 0;
    } else {
        PlayerBalloonPosY[PlayerId][balloonId] += 0.2;
        PlayerBalloonDepartingTimer[PlayerId][balloonId] += 1;
        ChaseSnumber(&D_8018D890[PlayerId][balloonId], 0, 0.1f);
        ChaseSnumber(&PlayerBalloonRotation[PlayerId][balloonId], 0, 0.1f);
        if (PlayerBalloonDepartingTimer[PlayerId][balloonId] >= 0x78) {
            KillBalloon((s32) Player, PlayerId, balloonId);
        }
    }
}

void MakeBalloonCalc(Player* Player, s8 playerIndex, s16 balloonIndex, s8 screenId) {
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
    primRed = (BalloonColorArray[Player->Kart] >> 0x10) & 0xFF;
    primGreen = (BalloonColorArray[Player->Kart] >> 0x08) & 0xFF;
    primBlue = (BalloonColorArray[Player->Kart] >> 0x00) & 0xFF;
    envRed = (BalloonAdjustArray[Player->Kart] >> 0x10) & 0xFF;
    envGreen = (BalloonAdjustArray[Player->Kart] >> 0x08) & 0xFF;
    envBlue = (BalloonAdjustArray[Player->Kart] >> 0x00) & 0xFF;
    temp_t1 = (((Player->AngleY[screenId] + Player->Direction[1] + Player->Slipang) & 0xFFFF) / 128);
    temp_t1 <<= 7;
    if (screenId == playerIndex) {
        var_f20 = 0.3f;
    } else {
        // wut?
        xdiff = (var_f20 = Player->Position[0] - Cameras[screenId].CameraPos[0]);
        zdiff = Player->Position[2] - Cameras[screenId].CameraPos[2];
        if (ScreenSplitA != 3) {
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
    sp134[0] = PlayerBalloonPosX[playerIndex][balloonIndex];
    sp134[1] = PlayerBalloonPosY[playerIndex][balloonIndex];
    sp134[2] = PlayerBalloonPosZ[playerIndex][balloonIndex];
    sp12C[0] = -((D_8018D890[playerIndex][balloonIndex] * 4) * CosT(temp_t1));
    sp12C[1] = Player->AngleY[screenId];
    sp12C[2] = D_8018D7D0[playerIndex][balloonIndex] -
               (PlayerBalloonRotation[playerIndex][balloonIndex] * CosT(temp_t1)) -
               ((D_8018D890[playerIndex][balloonIndex] * 8) * SinT(temp_t1));
    Createaffine(sp140, sp134, sp12C);
    ScaleMatrix(sp140, var_f20);
    Affine2Mtx(&DynamicP->MTXEffect[Effectcount], sp140);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, D_0D008DB8);
    gDPLoadTLUT_pal256(DisplayListHead++, LetterPAL);
    gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);

    IceMode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, 0x000000D8);

    gDPSetRenderMode(DisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gDPLoadTextureBlock(DisplayListHead++, Balloon1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(DisplayListHead++, BalloonVertexPlane1, 4, 0);
    gSPDisplayList(DisplayListHead++, common_square_plain_render);
    gDPLoadTextureBlock(DisplayListHead++, Balloon2 - 0x40, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, BalloonVertexPlane2, 4, 0);
    gSPDisplayList(DisplayListHead++, common_square_plain_render);
    gSPTexture(DisplayListHead++, 0x0001, 0x0001, 0, G_TX_RENDERTILE, G_OFF);
    Effectcount++;
}

void InitAllPlayerBalloons(Player* Player, s8 playerIndex) {
    BalloonDisp(Player, 0.0f, 0.0f, playerIndex, (s8) 0, (s16) 0);
    BalloonCheck(Player, 1.5f, 2.0f, playerIndex, (s8) 1, (s16) 0x1C70);
    BalloonCheck(Player, -1.5f, 2.0f, playerIndex, (s8) 2, (s16) -0x1C70);
    BalloonCount[playerIndex] = 2;
}

void AllkillBalloon(UNUSED Player* Player, s8 playerIndex) {
    PlayerBalloonStatus[playerIndex][0] = BALLOON_STATUS_GONE;
    PlayerBalloonStatus[playerIndex][1] = BALLOON_STATUS_GONE;
    PlayerBalloonStatus[playerIndex][2] = BALLOON_STATUS_GONE;
}

void CutPlayerBalloon(Player* Player, s8 playerIndex) {
    if (BalloonCount[playerIndex] >= 0) {
        PlayerBalloonStatus[playerIndex][BalloonCount[playerIndex]] &= ~BALLOON_STATUS_PRESENT;
        PlayerBalloonStatus[playerIndex][BalloonCount[playerIndex]] |= BALLOON_STATUS_DEPARTING;
        BalloonCount[playerIndex]--;
        NAPlyTrgStart(playerIndex, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x51));
        if (BalloonCount[playerIndex] < 0) {
            SetChangeBomb(Player, playerIndex);
        }
    }
}

void KillBalloon(UNUSED s32 arg0, s8 playerIndex, s8 balloonIndex) {
    if (BalloonCount[playerIndex] >= 0) {
        PlayerBalloonStatus[playerIndex][balloonIndex] = BALLOON_STATUS_GONE;
    }
}

void Balloonset(Player* Player, s8 PlayerId) {
    if (PlayerBalloonStatus[PlayerId][0] != BALLOON_STATUS_GONE) {
        BalloonStrat(Player, 0.0f, 0.0f, PlayerId, 0);
    }

    if (PlayerBalloonStatus[PlayerId][1] != BALLOON_STATUS_GONE) {
        BalloonStrat(Player, 1.8f, 2.6f, PlayerId, 1);
    }

    if (PlayerBalloonStatus[PlayerId][2] != BALLOON_STATUS_GONE) {
        BalloonStrat(Player, -1.8f, 2.6f, PlayerId, 2);
    }
}

void SetBalloonCalc(Player* Player, s8 playerIndex, s8 screenId) {
    if (PlayerBalloonStatus[playerIndex][0] != BALLOON_STATUS_GONE) {
        MakeBalloonCalc(Player, playerIndex, 0, screenId);
    }
    if (PlayerBalloonStatus[playerIndex][1] != BALLOON_STATUS_GONE) {
        MakeBalloonCalc(Player, playerIndex, 1, screenId);
    }
    if (PlayerBalloonStatus[playerIndex][2] != BALLOON_STATUS_GONE) {
        MakeBalloonCalc(Player, playerIndex, 2, screenId);
    }
}

/**
 * Used in podium ceremony.
 */
void DrawBalloon(Vec3f arg0, f32 arg1, s16 Arg2, s16 Arg3) {
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
    primRed = (BalloonColorArrayB[Arg3] >> 0x10) & 0xFF;
    primGreen = (BalloonColorArrayB[Arg3] >> 0x08) & 0xFF;
    primBlue = (BalloonColorArrayB[Arg3] >> 0x00) & 0xFF;
    envRed = (BalloonAdjustArrayB[Arg3] >> 0x10) & 0xFF;
    envGreen = (BalloonAdjustArrayB[Arg3] >> 0x08) & 0xFF;
    envBlue = (BalloonAdjustArrayB[Arg3] >> 0x00) & 0xFF;
    spFC[0] = arg0[0];
    spFC[1] = arg0[1];
    spFC[2] = arg0[2];
    spF4[0] = 0;
    spF4[1] = Camera1->CameraDirection[1];
    spF4[2] = Arg2;
    Createaffine(sp108, spFC, spF4);
    ScaleMatrix(sp108, arg1);
    Affine2Mtx(&DynamicP->MTXEffect[Effectcount], sp108);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, D_0D008DB8);
    gDPLoadTLUT_pal256(DisplayListHead++, LetterPAL);
    gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
    IceMode(primRed, primGreen, primBlue, envRed, envGreen, envBlue, 0x000000D8);
    gDPSetRenderMode(DisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
    gDPLoadTextureBlock(DisplayListHead++, Balloon1, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPVertex(DisplayListHead++, BalloonVertexPlane1, 4, 0);
    gSPDisplayList(DisplayListHead++, common_square_plain_render);
    // Balloon2 is correct. But interestingly, IDO seems to set "-0x40" to a different register so the Texture still
    // looks fine.
    gDPLoadTextureBlock(DisplayListHead++, Balloon2 - 0x40, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, BalloonVertexPlane2, 4, 0);
    gSPDisplayList(DisplayListHead++, common_square_plain_render);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    Effectcount += 1;
}

void DrawSmoke(Vec3f arg0, f32 arg1, s32 rgb, s16 Alpha) {
    Vec3f sp4C;
    Vec3s sp44;
    s16 red;
    s16 Green;
    s16 Blue;

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = Camera1->CameraDirection[1];
    sp44[2] = 0;
    CommonMTXCalc(sp4C, sp44, arg1);
    gSPDisplayList(DisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    // ???????????????????????????????????
    red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    Green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    Blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????
    ToumeIMode(red, Green, Blue, Alpha);
    gSPDisplayList(DisplayListHead++, D_0D008E48);
    Effectcount += 1;
}

void DrawStar(Vec3f arg0, f32 arg1, s32 rgb, s16 Alpha) {
    Vec3f sp5C;
    Vec3s sp54;
    // ???????????????????????????????????????
    s16 red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    s16 Green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    s16 Blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????????

    sp5C[0] = arg0[0];
    sp5C[1] = arg0[1];
    sp5C[2] = arg0[2];
    sp54[0] = 0;
    sp54[1] = Camera1->CameraDirection[1];
    sp54[2] = 0;
    CommonMTXCalc(sp5C, sp54, arg1);
    gSPDisplayList(DisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(DisplayListHead++, Hoshi, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    ToumeIMode(red, Green, Blue, Alpha);
    gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(DisplayListHead++, D_800E87C0, 4, 0);
    gSPDisplayList(DisplayListHead++, D_0D008DA0);
    Effectcount += 1;
}

void DrawPachipachi(Vec3f arg0, f32 arg1, s32 rgb, s16 Alpha, s16 arg4) {
    Vec3f sp4C;
    Vec3s sp44;
    // ???????????????????????????????????????
    s16 red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    s16 Green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    s16 Blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????????

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = Camera1->CameraDirection[1];
    sp44[2] = 0;
    CommonMTXCalc(sp4C, sp44, arg1);
    gSPDisplayList(DisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(DisplayListHead++, common_texture_particle_spark[arg4], G_IM_FMT_I, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    SplashMode(red, Green, Blue, Alpha);
    gSPDisplayList(DisplayListHead++, D_0D008E48);
    Effectcount += 1;
}

void Smokeset(Player* Player, s16 particleIndex, s8 PlayerId, s8 Arg3) {
    s8 playerIdCopy = PlayerId;
    s32 sp28;

    sp28 = particleIndex - 1;
    if (sp28 < 0) {
        sp28 = 9;
    }
    if (Player->smoke[particleIndex].Flag == 1) {
        switch (Player->smoke[particleIndex].Number) {
            case 1:
                DriftSmoke(Player, particleIndex, playerIdCopy, Arg3);
                break;
            case 2:
                RandomSmoke(Player, particleIndex, playerIdCopy, Arg3);
                break;
            case 3:
                RandomGrass(Player, particleIndex, playerIdCopy, Arg3);
                break;
            case 4:
                SpinSmoke(Player, particleIndex, playerIdCopy, Arg3);
                break;
            case 5:
                RapidSmoke(Player, particleIndex, playerIdCopy, Arg3);
                break;
            case 9:
                RandomSpl(Player, particleIndex, playerIdCopy, Arg3);
                break;
            case 11:
                RandomBubble(Player, particleIndex, playerIdCopy, Arg3);
                break;
            default:
                break;
        }
    } else {
        if (Player->WaterFlag & SUBMERGED) {
            InitBubble(Player, particleIndex, sp28, playerIdCopy, Arg3);
        } else if (!(Player->SlipFlag & N_JUMP) && !(Player->SlipFlag & D_JUMP)) {
            if (((Player->SlipFlag & DRIFT) == DRIFT) &&
                ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
                InitdriftSmoke(Player, particleIndex, sp28, playerIdCopy, Arg3);
            } else if (((f64) (WaterlevelPlayer[playerIdCopy] - Player->TireRR.Height) >= 3.5) ||
                       ((f64) (WaterlevelPlayer[playerIdCopy] - Player->TireRL.Height) >= 3.5)) {
                InitrndSpl(Player, particleIndex, sp28, playerIdCopy, Arg3);
            } else if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
                       ((Player->SlipFlag & SPIN_R) == SPIN_R)) {
                InitSpinSmokeHook(Player, particleIndex, sp28, playerIdCopy, Arg3);
            } else if (((Player->SlipFlag & RAPID_ACC) && !(Player->Flag & NO_CONTROLS)) ||
                       (Player->SlipFlag & WHEELSPIN) || (Player->SlipFlag & SPIN_TURN) ||
                       (Player->HandlingFlag & SPINOUT_SWERVE)) {
                InitRapidSmokeHook(Player, particleIndex, sp28, playerIdCopy, Arg3);
            } else {
                InitRndSmokeHook(Player, particleIndex, sp28, playerIdCopy, Arg3);
            }
        }
    }
}

void Sparkset(Player* Player, s16 arg1, s8 playerIndex, s8 Arg3) {
    UNUSED s32 stackPadding;
    s32 sp28;
    sp28 = arg1 - 1;
    if (sp28 < 0) {
        sp28 = 9;
    }
    if (Player->Spark[arg1].Flag == 1) {
        switch (Player->Spark[arg1].Number) {
            case 1:
                StarDust(Player, arg1, playerIndex, Arg3);
                break;

            case 2:
                TailDust(Player, arg1, playerIndex, Arg3);
                break;

            case 3:
                SlipStream(Player, arg1, playerIndex, Arg3);
                break;

            case 4:
                LandSmoke(Player, arg1, playerIndex, Arg3);
                break;

            case 5:
                PowderDust(Player, arg1, playerIndex, Arg3);
                break;

            case 6:
                BurnDust(Player, arg1, playerIndex, Arg3);
                break;

            case 7:
                WaterDust(Player, arg1, playerIndex, Arg3);
                break;

            case 8:
                SlipStream(Player, arg1, playerIndex, Arg3);
                break;

            case 9:
                ResultStar(Player, arg1, playerIndex, Arg3);
                break;

            default:
                break;
        }
    } else {
        if (Player->HandlingFlag & TROPHYJUMP_STARS) {
            InitresultStar(Player, arg1, sp28, playerIndex, Arg3);
            Player->HandlingFlag &= ~LANDING_DUST;
            return;
        }
        if (((((Player->JugemuFlag & LAVA_EFFECT) == LAVA_EFFECT) ||
              ((Player->BombTimer < 2) && (Player->SlipFlag & EXPLODE))) ||
             ((Player->BombTimer < 2) && (Player->SlipFlag & THROW_EXPLODE))) ||
            (Player->SlipFlag & ROLLOVER)) {
            InitburnDust(Player, arg1, sp28, playerIndex, Arg3);
            Player->HitFlag &= ~0x0008;
            Player->HandlingFlag &= ~LANDING_DUST;
            return;
        }
        if ((Player->JugemuFlag & WATER_EFFECT) == WATER_EFFECT) {
            InitwaterDust(Player, arg1, sp28, playerIndex, Arg3);
            Player->HitFlag &= ~0x0008;
            Player->HandlingFlag &= ~LANDING_DUST;
            return;
        }
        if ((Player->SlipFlag & STAR) &&
            ((((s32) GameTimer) - StarUseCounter[playerIndex]) < STAR_EFFECT_DURATION - 1)) {
            InitpowderDust(Player, arg1, sp28, playerIndex, Arg3);
            Player->HitFlag &= ~0x0008;
            Player->HandlingFlag &= ~LANDING_DUST;
            return;
        }
        if ((Player->HitFlag & 8) == 8) {
            InitstarDust(Player, arg1, sp28, playerIndex, Arg3);
            Player->HandlingFlag &= ~LANDING_DUST;
            return;
        }
        if (((Player->HitFlag & 0x20) == 0x20) && (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
            InittailDust(Player, arg1, sp28, playerIndex, Arg3);
            Player->HitFlag &= ~0x0008;
            Player->HandlingFlag &= ~LANDING_DUST;
            return;
        }
        if ((Player->SlipFlag & TURBO) && (Player->Flag & IS_PLAYER)) {
            InitTurbostream(Player, arg1, sp28, playerIndex, Arg3);
            return;
        }
        if (((Player->SlipFlag & SLIP_STREAM) || (Player->SlipFlag & DASH_JUMP)) &&
            ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
            InitSlipstream(Player, arg1, sp28, playerIndex, Arg3);
            Player->HitFlag &= ~0x0008;
            Player->HandlingFlag &= ~LANDING_DUST;
            return;
        }
        if ((Player->HandlingFlag & LANDING_DUST) == LANDING_DUST) {
            InitLand(Player, arg1, sp28, playerIndex, Arg3);
            Player->HitFlag &= ~0x0008;
        }
    }
}

void Gassset(Player* arg0, s16 arg1, s8 PlayerId, s8 Arg3) {
    UNUSED u16 temp_v0_3;
    s32 sp20 = arg1;
    if (--sp20 < 0) {
        sp20 = 9;
    }
    // Spawn particles when oob    
    if (arg0->Gass[arg1].Flag == 1) {
        switch (arg0->Gass[arg1].Number) {
            case 1:
                EngineSmoke(arg0, arg1, PlayerId, Arg3);
                break;
            case 3:
                RandomRing(arg0, arg1, PlayerId, Arg3);
                break;
            case 5:
                DobonSplash(arg0, arg1, PlayerId, Arg3);
                break;
            case 6:
                ThunderEffect(arg0, arg1, PlayerId, Arg3);
                break;
            case 7:
                TeresaEffect(arg0, arg1, PlayerId, Arg3);
                break;
        }
    } else {
        if ((arg0->HandlingFlag & BOO_SMOKE_EFFECT) && (arg0->Flag & SPINOUT_SWERVE)) {
            InitTeresa(arg0, arg1, sp20, PlayerId, Arg3);
            return;
        } else if (((arg0->SlipFlag & THUNDER) == THUNDER) && (arg0->ThunderTimer < 0x32)) {
            InitThunder(arg0, arg1, sp20, PlayerId, Arg3);
            return;
        } else if ((arg0->Flag & IS_PLAYER) == IS_PLAYER) {
            if ((arg0->WaterFlag & SPLASH_DIVE) == SPLASH_DIVE) {
                InitDobon(arg0, arg1, sp20, PlayerId, Arg3);
                return;
            } else if ((arg0->WaterFlag & HALF_SUBMERGED) || (arg0->WaterFlag & SUBMERGED)) {
                InitRing(arg0, arg1, sp20, PlayerId, Arg3);
                return;
            }
        }
        switch (ScreenSplitA) {
            case SCREEN_MODE_1P:
                if (((arg0->SlipFlag & BROKEN) != BROKEN) &&
                    ((arg0->SlipFlag & ROLLOVER) != ROLLOVER) &&
                    ((arg0->SlipFlag & EXPLODE) != EXPLODE)) {
                    if (((arg0->JugemuFlag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
                        ((arg0->JugemuFlag & IS_FROZEN) != IS_FROZEN) &&
                        !(arg0->JugemuFlag & LAKITU_CAMERA)) {
                        InitengineSmoke(arg0, arg1, sp20, PlayerId, Arg3);
                    }
                }
                break;
            default:
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (((arg0->Flag & IS_PLAYER) != 0) && ((arg0->SlipFlag & BROKEN) != BROKEN) &&
                    ((arg0->SlipFlag & ROLLOVER) != ROLLOVER) &&
                    ((arg0->SlipFlag & EXPLODE) != EXPLODE)) {
                    if (((arg0->JugemuFlag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
                        ((arg0->JugemuFlag & IS_FROZEN) != IS_FROZEN) &&
                        !(arg0->JugemuFlag & LAKITU_CAMERA)) {
                        InitengineSmoke(arg0, arg1, sp20, PlayerId, Arg3);
                    }
                }
            break;
        }
    }
}

void Letterset(Player* Player, s8 playerIndex, s8 Arg2) {
    if (Player->Letter[0].Flag == 1) {
        switch (Player->Letter[0].Number) {
            case 2:
                CrashLetter(Player, playerIndex, Arg2, 0);
                break;
            case 3:
                SpinLetter(Player, playerIndex, Arg2, 0);
                break;
            case 4:
                HitLetter(Player, playerIndex, Arg2, 0);
                break;
            case 5:
                BoingLetter(Player, playerIndex, Arg2, 0);
                break;
            case 6:
                PoompLetter(Player, playerIndex, Arg2, 0);
                break;
        }
    } else {
        if ((Player->Talk & CRASH) == CRASH) {
            InitcrashLetter(Player, playerIndex, Arg2, 0);
        }
        if ((Player->Talk & BOING) == BOING) {
            InitboingLetter(Player, playerIndex, Arg2, 0);
        }
        if ((Player->Talk & FLASH) == FLASH) {
            InithitLetter(Player, playerIndex, Arg2, 0);
        }
        if ((Player->Talk & WHIRRR) == WHIRRR) {
            InitspinLetter(Player, playerIndex, Arg2, 0);
        }
        if ((Player->Talk & POOMP) == POOMP) {
            InitpoompLetter(Player, playerIndex, Arg2, 0);
        }
    }
    if (Player->Letter[1].Flag == 1) {
        if (Player->Letter[1].Number == 5) {
            OnpuLetter(Player, playerIndex, Arg2, 1);
        }
    } else if ((Player->Talk & MUSIC_NOTE) == MUSIC_NOTE) {
        InitonpuLetter(Player, playerIndex, Arg2, 1);
    }
}

void OBJCalculation(Player* Player, s8 PlayerId, s8 screenId) {
    s16 var_s2;
    if ((Player->Status & (P1_DRAW_PARTICLES << (screenId * 4))) == (P1_DRAW_PARTICLES << (screenId * 4))) {
        for (var_s2 = 0; var_s2 < 10; var_s2++) {
            switch (Player->Gass[var_s2].Number) {
                case 1:
                    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == PlayerId) {
                            SetGassCalc(Player, PlayerId, var_s2, screenId);
                        }
                    } else {
                        SetGassCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case 6:
                    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == PlayerId) {
                            SetThunderCalc(Player, PlayerId, var_s2, screenId);
                        }
                    } else if (screenId == PlayerId) {
                        SetThunderCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
            }
            switch (Player->Spark[var_s2].Number) {
                case 1:
                case 9:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        SetStarCalc(Player, PlayerId, var_s2, screenId);
                    } else if (screenId == PlayerId) {
                        SetStarCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case 2:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        SetStartailCalc(Player, PlayerId, var_s2, screenId, Player->Spark[var_s2].Scale);
                    } else if (screenId == PlayerId) {
                        SetStartailCalc(Player, PlayerId, var_s2, screenId, Player->Spark[var_s2].Scale);
                    }
                    break;
                case 3:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        SetSlipstreamCalc(Player, PlayerId, var_s2, screenId);
                    } else if (screenId == PlayerId) {
                        SetSlipstreamCalc(Player, (s32) PlayerId, var_s2, screenId);
                    }
                    break;
                case 4:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        LandCalc(Player, PlayerId, var_s2, screenId);
                    } else if (screenId == PlayerId) {
                        LandCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case 5:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        PowderCalc(Player, PlayerId, var_s2, screenId);
                    } else if (screenId == PlayerId) {
                        PowderCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case 6:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        BurnCalc(Player, PlayerId, var_s2, screenId);
                    } else if (screenId == PlayerId) {
                        BurnCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case 7:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        WaterCalc(Player, PlayerId, var_s2, screenId);
                    } else if (screenId == PlayerId) {
                        WaterCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case 8:
                    if (ScreenSplitA == SCREEN_MODE_1P) {
                        SetTurbostreamCalc(Player, PlayerId, var_s2, screenId);
                    } else if (screenId == PlayerId) {
                        SetTurbostreamCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
            }
            switch (Player->smoke[var_s2].Number) {
                case DRIFT_PARTICLE:
                    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == PlayerId) {
                            SetDriftCalc(Player, PlayerId, var_s2, screenId);
                        }
                    } else {
                        SetDriftCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case GROUND_PARTICLE:
                case GRASS_PARTICLE:
                case 4:
                case 5:
                    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == PlayerId) {
                            SetSmokeCalc(Player, PlayerId, var_s2, screenId);
                        }
                    } else {
                        SetSmokeCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
                case 9:
                    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == PlayerId) {
                            SetSplCalc(Player, (s32) PlayerId, var_s2, screenId);
                        }
                    } else {
                        SetSplCalc(Player, (s32) PlayerId, var_s2, screenId);
                    }
                    break;
                case 11:
                    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                        if (screenId == PlayerId) {
                            SetBubbleCalc(Player, PlayerId, var_s2, screenId);
                        }
                    } else if (screenId == PlayerId) {
                        SetBubbleCalc(Player, PlayerId, var_s2, screenId);
                    }
                    break;
            }
        }
    }
    if ((GameMode == BATTLE) && (Player->Status & (P1_DRAW_SPRITE << (screenId * 4)))) {
        SetBalloonCalc(Player, PlayerId, screenId);
    }
}

void ExceObjCalculation(Player* Player, s8 playerIndex, s8 screenId) {
    s16 i;
    s32 bitwiseMask;

    bitwiseMask = P1_DRAW_PARTICLES << (screenId * 4);
    if (bitwiseMask == (Player->Status & bitwiseMask)) {
        for (i = 0; i < 10; i++) {
            if (Player->Gass[i].Number == 7) {
                SetTeresaCalc(Player, playerIndex, i, screenId);
            }
        }
    }
}

void ObjCalculation1(Player* arg0, s8 arg1, s8 Arg2) {
    s16 temp_s0;
    s32 temp_v0;

    temp_v0 = P1_DRAW_PARTICLES << (Arg2 * 4);
    if (temp_v0 == (arg0->Status & temp_v0)) {
        for (temp_s0 = 0; temp_s0 < 10; ++temp_s0) {
            temp_v0 = arg0->Gass[temp_s0].Number;
            if (temp_v0 != 3) {
                if (temp_v0 == 5) {
                    SetDobonCalc(arg0, arg1, temp_s0, Arg2);
                }
            } else if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
                if (Arg2 == arg1) {
                    SetRingCalc(arg0, arg1, temp_s0, Arg2);
                }
            } else {
                SetRingCalc(arg0, arg1, temp_s0, Arg2);
            }
        }

        if (((arg0->Flag & IS_PLAYER) == IS_PLAYER) && (Arg2 == arg1)) {
            switch (arg0->Letter[0].Number) {
                case 2:
                    SetCrashCalc(arg0, arg1, arg0->Letter[0].Scale, Arg2, 0);
                    break;
                case 3:
                    SetSpinCalc(arg0, arg1, arg0->Letter[0].Scale, Arg2, 0);
                    break;
                case 4:
                    SetHitCalc(arg0, arg1, arg0->Letter[0].Scale, Arg2, 0);
                    break;
                case 5:
                    SetBoingCalc(arg0, arg1, arg0->Letter[0].Scale, Arg2, 0);
                    break;
                case 6:
                    SetPoompCalc(arg0, arg1, arg0->Letter[0].Scale, Arg2, 0);
                    break;
            }
            if (arg0->Letter[1].Number == 5) {
                SetFukidashiCalc(arg0, Arg2, Fukidashi2, 1, 1.6f, 0xFFFFFF);
                SetWordCalc(arg0, Arg2, Onpu, 1, 1.6f, 0xFF);
            }
        }
    }
}

void KartEffectStrategy(void) {
    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            switch (GameMode) {
                case GRAND_PRIX:
                    EffectStrategy(PlayerOne, PLAYER_ONE, 0);
                    EffectStrategy(PlayerTwo, PLAYER_TWO, 0);
                    EffectStrategy(PlayerThree, PLAYER_THREE, 0);
                    EffectStrategy(PlayerFour, PLAYER_FOUR, 0);
                    EffectStrategy(PlayerFive, PLAYER_FIVE, 0);
                    EffectStrategy(PlayerSix, PLAYER_SIX, 0);
                    EffectStrategy(PlayerSeven, PLAYER_SEVEN, 0);
                    EffectStrategy(PlayerEight, PLAYER_EIGHT, 0);

                    break;
                case TIME_TRIALS:
                    EffectStrategy(PlayerOne, PLAYER_ONE, 0);

                    if ((PlayerTwo->Flag & IS_GHOST) == IS_GHOST) {
                        EffectStrategy(PlayerTwo, PLAYER_TWO, 0);
                    }

                    if ((PlayerThree->Flag & IS_GHOST) == IS_GHOST) {
                        EffectStrategy(PlayerThree, PLAYER_THREE, 0);
                        break;
                    }

                    break;
                case VERSUS:
                case BATTLE:
                    EffectStrategy(PlayerOne, PLAYER_ONE, 0);
                    EffectStrategy(PlayerTwo, PLAYER_TWO, 0);

                    if (PlayerCount >= 3) {
                        EffectStrategy(PlayerThree, PLAYER_THREE, 0);
                    }

                    if (PlayerCount == 4) {
                        EffectStrategy(PlayerFour, PLAYER_FOUR, 0);
                        break;
                    }

                    break;
            }

            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            switch (GameMode) {
                case GRAND_PRIX:
                    EffectStrategy(PlayerOne, PLAYER_ONE, 0);
                    EffectStrategy(PlayerTwo, PLAYER_TWO, 0);
                    EffectStrategy(PlayerThree, PLAYER_THREE, 0);
                    EffectStrategy(PlayerFour, PLAYER_FOUR, 0);
                    EffectStrategy(PlayerFive, PLAYER_FIVE, 0);
                    EffectStrategy(PlayerSix, PLAYER_SIX, 0);
                    EffectStrategy(PlayerSeven, PLAYER_SEVEN, 0);
                    EffectStrategy(PlayerEight, PLAYER_EIGHT, 0);

                    break;
                case VERSUS:
                case BATTLE:
                    EffectStrategy(PlayerOne, PLAYER_ONE, 0);
                    EffectStrategy(PlayerTwo, PLAYER_TWO, 0);

                    break;
                case TIME_TRIALS:
                    EffectStrategy(PlayerOne, PLAYER_ONE, 0);

                    if ((PlayerTwo->Flag & EXISTS) == EXISTS) {
                        EffectStrategy(PlayerTwo, PLAYER_TWO, 0);
                        break;
                    }

                    break;
            }

            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if ((VERSUS == GameMode) || (BATTLE == GameMode)) {
                EffectStrategy(PlayerOne, PLAYER_ONE, 0);
                EffectStrategy(PlayerTwo, PLAYER_TWO, 0);
                EffectStrategy(PlayerThree, PLAYER_THREE, 0);

                if (PlayerCount == 4) {
                    EffectStrategy(PlayerFour, PLAYER_FOUR, 0);
                }
            }

            break;
    }
}

void EffectStrategy(Player* Player, s8 playerIndex, s8 Arg2) {
    // Arg2 is always 0
    s16 temp_s0;

    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
            Letterset(Player, playerIndex, Arg2);
        }

        for (temp_s0 = 0; temp_s0 < 10; ++temp_s0) {
            Gassset(Player, temp_s0, playerIndex, Arg2);
            if (((Player->Flag & IS_PLAYER) == IS_PLAYER) || (GameState == ENDING)) {
                Sparkset(Player, temp_s0, playerIndex, Arg2);
            }
            Smokeset(Player, temp_s0, playerIndex, Arg2);
        }

        if (GameMode == BATTLE) {
            Balloonset(Player, playerIndex);
        }
    }
}

void SmokeDisp(Player* Player, s8 PlayerId, s8 screenId) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (PlayerId == screenId) {
                OBJCalculation(Player, PlayerId, screenId);
            }
        } else {
            OBJCalculation(Player, PlayerId, screenId);
        }
        ExceObjCalculation(Player, PlayerId, screenId);
    }
}

void SmokeDisp2P(Player* Player, s8 playerIndex, s8 screenId) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (playerIndex == screenId) {
                OBJCalculation(Player, playerIndex, screenId);
            }
        } else {
            OBJCalculation(Player, playerIndex, screenId);
        }
        ExceObjCalculation(Player, playerIndex, screenId);
    }
}

void SmokeDisp3P(Player* Player, s8 playerIndex, s8 screenId) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (playerIndex == screenId) {
                OBJCalculation(Player, playerIndex, screenId);
            }
        } else {
            OBJCalculation(Player, playerIndex, screenId);
        }
        ExceObjCalculation(Player, playerIndex, screenId);
    }
}

void SmokeDisp4P(Player* Player, s8 playerIndex, s8 screenId) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (playerIndex == screenId) {
                OBJCalculation(Player, playerIndex, screenId);
            }
        } else {
            OBJCalculation(Player, playerIndex, screenId);
        }
        ExceObjCalculation(Player, playerIndex, screenId);
    }
}

void SmokeDispAfter(Player* Player, s8 arg1, s8 Arg2) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (arg1 == Arg2) {
                ObjCalculation1(Player, arg1, Arg2);
            }
        } else {
            ObjCalculation1(Player, arg1, Arg2);
        }
    }
}

void SmokeDispAfter2P(Player* Player, s8 arg1, s8 Arg2) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (arg1 == Arg2) {
                ObjCalculation1(Player, arg1, Arg2);
            }
        } else {
            ObjCalculation1(Player, arg1, Arg2);
        }
    }
}

void SmokeDispAfter3P(Player* Player, s8 arg1, s8 Arg2) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (arg1 == Arg2) {
                ObjCalculation1(Player, arg1, Arg2);
            }
        } else {
            ObjCalculation1(Player, arg1, Arg2);
        }
    }
}

void SmokeDispAfter4P(Player* Player, s8 arg1, s8 Arg2) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (arg1 == Arg2) {
                ObjCalculation1(Player, arg1, Arg2);
            }
        } else {
            ObjCalculation1(Player, arg1, Arg2);
        }
    }
}

/***************************************************************/
/* Just a random 10 laying around in data, nothing to see here */
/***************************************************************/
s32 SomeUnusedData = 10;

#undef MAKE_RGB
