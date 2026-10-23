#include <ultra64.h>
#include <macros.h>
#include <memory.h>
#include <defines.h>
#include <mk64.h>

#include "code_800029B0.h"
#include "cpu_vehicles_camera_path.h"
#include "code_80057C60.h"
#include "code_8006E9C0.h"
#include "code_80086E70.h"
#include "update_objects.h"
#include "objects.h"
#include "bomb_kart.h"
#include "save.h"
#include <assets/common_data.h>
#include <sounds.h>
#include <decode.h>
#include "audio/external.h"
#include "courses/all_course_data.h"
#include "main.h"
#include "menus.h"
#include "data/other_textures.h"
#include "render_objects.h"
#include "menu_items.h"
#include "src/data/some_data.h"
#include "effects.h"
#include "OverKart5Hooks.h"

void InitializeParams(void) {

    KwclearRAM();
    InitializeParamsCommon();

    switch (ScreenSplitB) {
        case SCREEN_MODE_1P:
            InitializeParams1P();
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            InitializeParams2Plr();
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            InitializeParams2Pud();
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            InitializeParams4P();
            break;
    }
    InitializeParamsCommonAfter();
}

void KwclearRAM(void) {
    s32 i;
    s32 j;
    Kwfreeallocptr();
    Kwfreelist();
    bzero(PlayerHUD, HUD_PLAYERS_SIZE * sizeof(Hud));

    for (i = 0; i < HUD_PLAYERS_SIZE; i++) {
        PlayerHUD[i].Goal = 0;
        PlayerHUD[i].Lapcnt = 0;
        PlayerHUD[i].Ranksw = 0;
    }
    for (j = 0; j < HUD_PLAYERS_SIZE; j++) {
        PlayerHUD[j].Goalsw = 0;
    }
}

void Kwfreeallocptr(void) {
    s32 i;

    for (i = 0; i < gObjectParticle1_SIZE; i++) {
        EffectAllocArray1[i] = NULL_OBJECT_ID;
    }

    for (i = 0; i < gObjectParticle2_SIZE; i++) {
        EffectAllocArray2[i] = NULL_OBJECT_ID;
    }

    for (i = 0; i < gObjectParticle3_SIZE; i++) {
        EffectAllocArray3[i] = NULL_OBJECT_ID;
    }

    for (i = 0; i < gObjectParticle4_SIZE; i++) {
        FireParticleAllocArray[i] = NULL_OBJECT_ID;
    }

    // clang-format off
    // Has to be on one line, because IDO hates you :)
    for (i = 0; i < gLeafParticle_SIZE; i++) { Leafallocptr[i] = NULL_OBJECT_ID; }
    // clang-format on

    D_8018CF18 = D_8018CF20 = D_8018CF48 = D_8018CF60 = D_8018CF78 = D_8018CF90 = D_8018CFA8 = 0;
    D_8018CFB0 = D_8018CFB8 = D_8018CFC0 = D_8018CFC8 = D_8018CFD0 = D_8018CFD8 = D_8018CFE0 = 0;
    D_8018D018 = 0;
    D_8018D010 = 0;
    D_8018D008 = 0;
    D_8018D000 = 0;
    D_8018CFF8 = 0;
    D_8018CFF0 = 0;
    D_8018CFE8 = 0;
    D_8018D110 = 0;
    D_8018D0E8 = 0;
    D_8018D0C0 = 0;
    D_8018D020 = D_8018D048 = D_8018D070 = D_8018D098 = 0;
    NextFreeObjectParticle1 = NextFreeObjectParticle2 = NextFreeObjectParticle3 = FireParticleCounter =
        NextFreeLeafParticle = 0;
}

void Kwfreelist() {
    bzero(DynamicObjects, OBJECT_LIST_SIZE * sizeof(AnmObject));
    ObjectListSize = -1;
}

/**
 * Dma's mario Kart 64 logo and course outline Textures.
 */
u8* KwdmaMelt(u8* devAddr, u8* baseAddress, u32 Size, u32 Offset) {
    u8** tempAddress;
    u8* Address;
    Address = baseAddress + Offset;

    Size = ALIGN16(Size);
    osInvalDCache(Address, (Size));
    osPiStartDma(&DmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[((u32) devAddr) & 0xFFFFFF], Address,
                 Size, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, 1);
    tempAddress = &Address;
    decodeMIO0(*tempAddress, (u8*) baseAddress);
    return baseAddress;
}

void KwmeltTitlelogo(void) {
    Logoptr =
        KwdmaMelt((u8*) &logo_mario_kart_64, (u8*) TexBuff, 0x79E1, 0x20000);
}

// Some Kind of initalization for the Item Window part of the HUD
void KwinitItembox(s32 ObjectIndex) {
    ItemWindowObjects* temp_v0;

    temp_v0 = (ItemWindowObjects*) &DynamicObjects[ObjectIndex];
    temp_v0->Output = ITEM_NONE;
    temp_v0->Anmnumbak = temp_v0->Output;
    temp_v0->Stexaddr = (u8*) CommonTLUTItemWindowNone;
    temp_v0->Texaddr = (u8*) CommonTLUTItemWindowNone;
    temp_v0->Sindexaddr = CommonTextureItemWindowNone;
    temp_v0->Indexaddr = CommonTextureItemWindowNone;
    temp_v0->Timer = -1;
    temp_v0->ix = 0x00A0;  // Screen X Position
    temp_v0->iy = -0x0020; // Screen Y Position
    temp_v0->Scale = 1.0f;
}

void KwmeltRoadmap(s32 courseId) {
    MapTexture = (s32) DecodeMapImage1(CourseOutlineTextures[courseId], D_800E5520[courseId], D_800E5520[courseId]);
    // This is incredibly dumb. D_800E5548 ought to be something more like
    // `u16 D_800E5548[][2]` but that doesn't match for some insane reason
    MapWidth = D_800E5548[courseId * 2];
    MapHeight = D_800E5548[courseId * 2 + 1];
}

void KwmeltKawanoroadmap(void) {
    s32 i;
    // `huh`'s and `i`'s types have to differ, for some reason
    s16 huh;
    u8* wut;

    wut = (u8*) &MeltBuff[0x3FFFC000];
    // clang-format off
    // God forgive me for my SinT...
    huh = 0x14; if (0) {} for (i = 0; i < huh; i++) { D_8018D248[i] = KwdmaMelt(CourseOutlineTextures[i], wut, D_800E5520[i], D_800E5520[i]); wut += D_800E5520[i]; }
    // clang-format on
}

void KwmeltCommon(void) {
    D_801655C8 = 0;
    XOrientation = 1.0f;
    if (ScreenFlip != 0) {
        XOrientation = -1.0f;
    }
    MapX[0] = 257;
    MapY[0] = 170;
    MapR = 255;
    MapG = 255;
    MapB = 255;
    Ralpha = 255;
    if (CourseID < NUM_COURSES - 1) {
        KwmeltRoadmap((s32) CourseID);
    }
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust5, 0x443, 0x1000);
            MapScale = 0.022f;
            StartX = 6;
            StartY = 28;
            MapX[0] = 260;
            MapY[0] = 170;
            D_80165718 = 0;
            D_80165720 = 5;
            D_80165728 = -240;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            MapScale = 0.022f;
            MapX[0] = 265;
            StartX = 19;
            StartY = 37;
            break;
        case COURSE_BOWSER_CASTLE:
            MapX[0] = 265;
            MapScale = 0.0174f;
            StartX = 12;
            StartY = 48;
            break;
        case COURSE_BANSHEE_BOARDWALK:
            D_80165880 = (void*) DecodeMapImage1(gTextureGhosts, 0x4CC2, 0xD980);
            MapScale = 0.016f;
            MapX[0] = 0x0106;
            StartX = 55;
            StartY = 39;
            break;
        case COURSE_YOSHI_VALLEY:
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust0, 0x479, 0xC00);
            MapScale = 0.018f;
            StartX = 61;
            StartY = 38;
            break;
        case COURSE_FRAPPE_SNOWLAND:
            MapX[0] = 262;
            MapScale = 0.016f;
            StartX = 36;
            StartY = 40;
            MapR = 72;
            MapG = 100;
            MapB = 255;
            break;
        case COURSE_KOOPA_BEACH:
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust3, 0x3C8U, 0x1000);
            MapScale = 0.014f;
            MapX[0] = 268;
            StartX = 40;
            StartY = 21;
            break;
        case COURSE_ROYAL_RACEWAY:
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust4, 0x3F8, 0x1000);
            MapX[0] = 262;
            MapScale = 0.014f;
            StartX = 37;
            StartY = 50;
            D_80165718 = -64;
            D_80165720 = 5;
            D_80165728 = -330;
            break;
        case COURSE_LUIGI_RACEWAY:
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust2, 0x4F4U, 0xC00);
            MapScale = 0.0155f;
            MapX[0] = 271;
            StartX = 45;
            StartY = 60;
            D_80165718 = -140;
            D_80165720 = -44;
            D_80165728 = -215;
            break;
        case COURSE_MOO_MOO_FARM:
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust0, 0x479, 0xC00);
            MapScale = 0.0155f;
            MapX[0] = 271;
            StartX = 18;
            StartY = 36;
            break;
        case COURSE_TOADS_TURNPIKE:
            MapScale = 0.013f;
            MapX[0] = 252;
            StartX = 57;
            StartY = 44;
            break;
        case COURSE_KALAMARI_DESERT:
            MapX[0] = 263;
            MapY[0] = 165;
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust5, 0x443, 0x1000);
            MapScale = 0.015f;
            StartX = 55;
            StartY = 27;
            break;
        case COURSE_SHERBET_LAND:
            Kumoptr = (void*) DecodeMapImage1(gTextureExhaust1, 0x485, 0xC00);
            MapScale = 0.015f;
            MapX[0] = 262;
            StartX = 52;
            StartY = 33;
            MapR = 72;
            MapG = 100;
            MapB = 255;
            break;
        case COURSE_RAINBOW_ROAD:
            MapScale = 0.0103f;
            MapX[0] = 261;
            MapY[0] = 166;
            StartX = 39;
            StartY = 55;
            break;
        case COURSE_WARIO_STADIUM:
            MapScale = 0.0155f;
            MapX[0] = 0x0106;
            StartX = 53;
            StartY = 35;
            break;
        case COURSE_BLOCK_FORT:
            MapScale = 0.0335f;
            StartX = 32;
            StartY = 32;
            break;
        case COURSE_SKYSCRAPER:
            MapScale = 0.0445f;
            StartX = 32;
            StartY = 32;
            break;
        case COURSE_DOUBLE_DECK:
            MapScale = 0.0285f;
            StartX = 32;
            StartY = 32;
            break;
        case COURSE_DK_JUNGLE:
            MapScale = 0.0155f;
            MapX[0] = 255;
            StartX = 29;
            StartY = 47;
            break;
        case COURSE_BIG_DONUT:
            MapScale = 0.0257f;
            StartX = 32;
            StartY = 31;
    }
#else

#endif
    if (ScreenFlip != 0) {
        StartX = MapWidth - StartX;
    }
    if (MenuMultiplayerSelection == 4) {
        MapX[0] = 160;
        MapY[0] = 120;
        return;
    }
    if (MenuMultiplayerSelection == 3) {
        MapX[0] = 235;
        MapY[0] = 175;
        return;
    }
    if (MenuMultiplayerSelection == 2) {
        if (CourseID != COURSE_TOADS_TURNPIKE) {
            MapX[1] = 265;
            MapX[0] = MapX[1];
        } else {
            MapX[1] = 255;
            MapX[0] = MapX[1];
        }
        MapY[0] = 65;
        MapY[1] = 180;
    }
}

void KwbackupGamestatus(s32 arg0) {
    HUDToggleFlag = HUDLapToggle;
    HUDSpeedToggle = HUDSpeedToggle2;
    D_80165820 = BlueLineRankToggle;
    D_80165818 = HUDMapToggle2;
    D_80165828 = KWLapSW;
    HUDToggleFlagP2[0] = D_80165800[0];
    HUDToggleFlagP2[1] = D_80165800[1];
    if ((arg0 != 0) && (PauseFlag == 0)) {
        PlaySound(SOUND_ACTION_PING);
    }
}

void KwscreenInit(void) {
    if (InGame == 0) {
        InGame = 1;
        HUDLapToggle = 0;
        HUDSpeedToggle2 = 0;
        BlueLineRankToggle = 0;
        HUDMapToggle2 = 1;
        D_80165800[0] = D_80165800[1] = 1;
        if (MenuMultiplayerSelection == 4) {
            if (GameMode != BATTLE) {
                HUDLapToggle = 1;
                BlueLineRankToggle = 1;
                KWLapSW = 1;
                D_80165800[0] = D_80165800[1] = 0;
            } else {
                KWLapSW = 0;
                D_80165800[0] = D_80165800[1] = 1;
            }
        } else if (MenuMultiplayerSelection == 3) {
            HUDMapToggle2 = 0;
            KWLapSW = 1;
        } else if (MenuMultiplayerSelection == 2) {
            if (GameMode != (s32) BATTLE) {
                HUDLapToggle = 1;
                BlueLineRankToggle = 1;
                D_80165800[0] = D_80165800[1] = 0;
            }
            MapY[0] = 0x0041;
            MapY[1] = 0x00B4;
        }
        KwbackupGamestatus(0);
    } else {
        HUDLapToggle = HUDToggleFlag;
        HUDSpeedToggle2 = HUDSpeedToggle;
        BlueLineRankToggle = D_80165820;
        HUDMapToggle2 = D_80165818;
        KWLapSW = D_80165828;
        D_80165800[0] = HUDToggleFlagP2[0];
        D_80165800[1] = HUDToggleFlagP2[1];
    }
    if (DemoFlag != 0) {
        BlueLineRankToggle = 0;
    }
}

void InitializeParamsCommon(void) {
    s32 i;
    Player *Player;

    KwscreenInit();
    KwmeltCommon();
    osSetTime(0);
    D_8018D170 = 0;
    D_8018D190 = 0;
    KWScreenEnable = 0;
    D_8018D178 = 0;
    D_8018D1CC = 0;
    Kwgoalpanelsw = 0;
    D_80165730 = 0;
    D_801658FE = 0;

    D_801657E1 = D_801657E3 = D_801657E5 = 0;

    D_80165658[0] = D_80165658[1] = D_80165658[2] = 0;
    D_801658BC = D_801658C6 = D_801658CE = D_801658DC = D_801658EC = D_801658F4 = D_801658E4 = D_801658D6 = 0;

    switch (MenuMultiplayerSelection) {
    case 1:
        if (GameMode == GRAND_PRIX) {
            D_8018D114 = 0;
            D_8018D178 = 150;
            D_8018D180 = 240;
        } else {
            D_8018D114 = 1;
            D_8018D178 = 10;
            D_8018D180 = 0;
        }
        break;
    case 2:
        if (ScreenSplitB == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) {
            if (GameMode == GRAND_PRIX) {
                D_8018D114 = 2;
                D_8018D178 = 150;
                D_8018D180 = 240;
                D_8018D2AC = 60;
            } else if (GameMode == VERSUS) {
                D_8018D114 = 3;
                D_8018D178 = 30;
                D_8018D180 = 30;
                D_8018D2AC = 60;
            } else {
                D_8018D114 = 4;
                D_8018D178 = 40;
                D_8018D180 = 40;
                D_8018D2AC = 60;
            }
        } else if (GameMode == GRAND_PRIX) {
            D_8018D114 = 5;
        } else if (GameMode == VERSUS) {
            D_8018D114 = 6;
        } else {
            D_8018D114 = 7;
        }
        break;
    case 3:
        if (GameMode == VERSUS) {
            D_8018D114 = 8;
            D_8018D178 = 0x00000064;
            D_8018D180 = 0x00000096;
            D_8018D2AC = 0x0000003C;
        } else {
            D_8018D114 = 9;
            D_8018D178 = 0x00000064;
            D_8018D180 = 0x00000096;
            D_8018D2AC = 0x0000003C;
        }
        break;
    case 4:
        if (GameMode == VERSUS) {
            D_8018D114 = 0x0000000A;
            D_8018D178 = 0x0000001E;
            D_8018D180 = 0x0000001E;
            D_8018D2AC = 0x0000000A;
        } else {
            D_8018D114 = 0x0000000B;
            D_8018D178 = 0x0000001E;
            D_8018D180 = 0x0000001E;
            D_8018D2AC = 0x0000000A;
        }
        break;
    }
    if (DebugMode == 0) {
        D_8016576A = 0;
        D_8016579C = 0;
    }

    for (i = 0; i < MenuMultiplayerSelection; i++) {
        D_8018CFC4[i] = D_8018CFAC[i] = D_8018CFBC[i] = 0;
        D_8018CFB4[i] = 0;
    }

    D_8018D204 = 1;
    D_8018D1FC = 0;
    D_8018D224 = 0;
    CloudCount1P = CloudCount2P = 0;
    Kumotype = 0xFF;

    D_801655D8 = D_801655E8 = D_801655F8 = KWFlash8 = D_80165618 = D_80165628 = 0;
    D_8018D160 = 0;
    D_8018D1DC = 0;
    D_8018D1C4 = 0;
    D_8018D1B4 = 0;
    D_8018D1A0 = 0;
    D_8018D168 = 0;
    D_801656F0 = 0;
    D_801657AE = KWDBDispSW = D_8018D214 = KWDemoSW = D_801657B2 = 0;
    D_8018D20C = 0;
    Rzerox = Rzeroy = 0;
    Kwgoalcnt = 3;
    D_8018D2AC = 0;
    D_8018D2A4 = D_8018D2B4 = D_8018D2BC = 0;
    D_8018D2C8[0] = D_8018D2C8[1] = D_8018D2C8[2] = D_8018D2C8[3] = 0;
    D_8016581C = 0;
    D_8016580C = 0;
    D_80165814 = 0;
    D_80165804 = 0;
    D_801657FC = 0;
    D_8018D174 = D_8018D17C = D_8018D16C = D_8018D184 = D_8018D18C = -1;
    Player = PlayerOne;
    for (i = 0; i < NUM_PLAYERS; i++) {
        D_8018D0F0[i] = HUDCharpicRankY1[i] = -32.0f;
        D_8018CE10[i].unk_04[0] = D_8018CE10[i].unk_04[1] = D_8018CE10[i].unk_04[2] = 0.0f;
        D_8018CF50[i] = i;
        D_8018CF28[i] = Player;
        Player->LinkDoshin = -1;
        Player++;
    }
}

void InitializeParamsCommonAfter(void) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 8; var_s0++) {
        Kwalloc(&D_8018CE10[var_s0].ObjectIndex);
    }
}

void KwallocObjram(void) {
    s32 loopIndex;

    for (loopIndex = 0; loopIndex < SOME_OBJECT_INDEX_LIST_SIZE; loopIndex++) {
        Kwalloc(&Objallocptr[loopIndex]);
        Kwalloc(&Objallocptr2[loopIndex]);
        Kwalloc(&Objallocptr3[loopIndex]);
        Kwalloc(&Objallocptr4[loopIndex]);
    }

    for (loopIndex = 0; loopIndex < NUM_BOMB_KARTS_VERSUS; loopIndex++) {
        Kwalloc(&Bomballocptr[loopIndex]);
    }
}

void KwinitKumo(s32 ObjectIndex, s32 arg1, CloudData* Arg2) {
    ItemWindowObjects* temp_v0;

    KWAnmStart(ObjectIndex, arg1);
    temp_v0 = (ItemWindowObjects*) &DynamicObjects[ObjectIndex];
    temp_v0->Chartptr = Arg2->SubType;
    temp_v0->Output = ITEM_NONE;
    temp_v0->ang[1] = Arg2->RotY;
    temp_v0->iy = Arg2->PosY;
    temp_v0->Scale = (f32) Arg2->ScalePercent / 100.0;
    temp_v0->Indexaddr = (u8*) &Kumoptr[Arg2->SubType];
    KwanmInitCommon(ObjectIndex, 0x40U, 0x20U, D_0D005FB0);
    temp_v0->Alpha = 0x00FF;
}

void KwsetKumoram(CloudData* cloudList) {
    s32 var_s0 = 0;
    CloudData* test = &cloudList[0];
    do {
        if (1) {}
        KwinitKumo(Kwalloc(&CloudAllocate[CloudCount2P + var_s0]), 1, test);
        var_s0++;
        test++;
    } while (test->RotY != 0xFFFF);
    CloudCount2P += var_s0;
    CloudCount1P = var_s0;
    Kwhoshisw = 0;
}

/**
 * This function is part of the spawning for the "stars" in some stages
 *
 * Arg2 is a Pointer to some Type of spawn data for the stars, although it not super clear
 * what types each element is. It seems like its a bunch of u16's, so maybe a Vec4su?
 *
 * The stars in Wario's Stadium, Toad's Turnpike, and Rainbow Road are not part of the skybox.
 * They are instead objects that seemingly hover in the air around the Player
 * They have no true x/y/z Position, instead they seem to be kept in a Position relative to the
 * Player they hang around. There is however an y Rotation and y Position for where they should be on screen
 * when they are visbile (unk_09E[0] and [1]).
 * SizeScaling is some sort of Size scaling on the Start Texture.
 * unk_0A2 is an Alpha Value, Used to make the Star twinkle.
 **/
void KwinitStar(s32 ObjectIndex, s32 arg1, StarData* Arg2) {
    ItemWindowObjects* temp_v0;

    KWAnmStart(ObjectIndex, arg1);
    temp_v0 = (ItemWindowObjects*) &DynamicObjects[ObjectIndex];
    temp_v0->Chartptr = Arg2->SubType;
    temp_v0->Output = ITEM_BANANA;
    temp_v0->ang[1] = Arg2->RotY;
    temp_v0->iy = Arg2->PosY;                           // screen Y Position
    temp_v0->Scale = (f32) Arg2->ScalePercent / 100.0; // some Type of scaling on the Texture
    temp_v0->Indexaddr = D_0D0293D8;
    KwanmInitCommon(ObjectIndex, 0x10U, 0x10U, VtxKTile16X16);
}

void KwsetStarram(StarData* starList) {
    s32 var_s0 = 0;
    StarData* test = &starList[0];
    do {
        if (1) {}
        KwinitStar(Kwalloc(&CloudAllocate[CloudCount2P + var_s0]), 1, test);
        var_s0++;
        test++;
    } while (test->RotY != 0xFFFF);
    CloudCount2P += var_s0;
    CloudCount1P = var_s0;
    Kwhoshisw = 1;
}

void KWKumoAlloc(void) {
    s32 var_s0;
    s32 var_s4;

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            // Uses Kalimari Desert's clouds for initialization?
            KwsetKumoram(KalimariDesertClouds);
            break;
        case COURSE_YOSHI_VALLEY:
            KwsetKumoram(YoshiValleyMooMooFarmClouds);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (MenuMultiplayerSelection == 1) {
                var_s4 = 0x32;
            } else {
                var_s4 = 0x19;
            }
            for (var_s0 = 0; var_s0 < var_s4; var_s0++) {
                Kwalloc(&CloudAllocate[CloudCount2P + var_s0]);
            }
            CloudCount2P += var_s0;
            CloudCount1P = var_s0;
            break;
        case COURSE_KOOPA_BEACH:
            KwsetKumoram(KoopaTroopaBeachClouds);
            break;
        case COURSE_ROYAL_RACEWAY:
            KwsetKumoram(RoyalRacewayClouds);
            break;
        case COURSE_LUIGI_RACEWAY:
            KwsetKumoram(LuigiRacewayClouds);
            break;
        case COURSE_MOO_MOO_FARM:
            KwsetKumoram(YoshiValleyMooMooFarmClouds);
            break;
        case COURSE_TOADS_TURNPIKE:
            KwsetStarram(ToadsTurnpikeRainbowRoadStars);
            break;
        case COURSE_KALAMARI_DESERT:
            KwsetKumoram(KalimariDesertClouds);
            break;
        case COURSE_SHERBET_LAND:
            KwsetKumoram(SherbetLandClouds);
            break;
        case COURSE_RAINBOW_ROAD:
            KwsetStarram(ToadsTurnpikeRainbowRoadStars);
            break;
        case COURSE_WARIO_STADIUM:
            KwsetStarram(WarioStadiumStars);
            break;
    }
#else

#endif
    KwdebugKumoram();
}

void KwinitFuusen(void) {
    D_80165730 = 1;
    if (MenuMultiplayerSelection == ONE_PLAYERS_SELECTED) {
        D_80165738 = 0x64;
        D_80165740 = 0x3C;
        D_80165748 = 0x1E;
        return;
    }
    D_80165738 = 0x32;
    D_80165740 = 0x1E;
    D_80165748 = 0xA;
}

void KwallocEffectram1P(void) {
    s32 objectId;
    s32 i;

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (Snow3DCourseID) {
        case COURSE_MARIO_RACEWAY:
            if (GameState != 9) {
                if (GameMode == GRAND_PRIX) {
                    KwinitFuusen();
                }
                for (i = 0; i < D_80165738; i++) {
                    Kwalloc(&EffectAllocArray3[i]);
                    KWAnmStart(EffectAllocArray3[i], 0);
                }
            }
            break;
        case COURSE_BOWSER_CASTLE:
            NumActiveThwomps = NUM_THWOMPS_100CC_EXTRA;
            ThowmpSpawnList = ThwompSpawns100CCExtra;
            switch (RaceClass) { /* switch 1; irregular */
                case CC_100:        /* switch 1 */
                case CC_EXTRA:      /* switch 1 */
                    break;
                case CC_50: /* switch 1 */
                    NumActiveThwomps = NUM_THWOMPS_50CC;
                    ThowmpSpawnList = ThomwpSpawns50CC;
                    break;
                case CC_150: /* switch 1 */
                    NumActiveThwomps = NUM_THWOMPS_150CC;
                    ThowmpSpawnList = ThomwpSpawns150CC;
                    break;
            }
            for (i = 0; i < NumActiveThwomps; i++) {
                objectId = Objallocptr[i];
                KWAnmStart(objectId, 0);
                DynamicObjects[objectId].PosStart[0] = ThowmpSpawnList[i].StartX * XOrientation;
                DynamicObjects[objectId].PosStart[2] = ThowmpSpawnList[i].StartZ;
                DynamicObjects[objectId].Chartptr = ThowmpSpawnList[i].unk_4;
                DynamicObjects[objectId].Alpha = ThowmpSpawnList[i].unk_6;
            }
            // Handle the big statue's fire breath
            objectId = Objallocptr2[0];
            KWAnmStart(objectId, 0);
            DynamicObjects[objectId].pos[0] = -68.0 * XOrientation;
            DynamicObjects[objectId].pos[1] = 80.0f;
            DynamicObjects[objectId].pos[2] = -1840.0f;
            // Handle the smaller statues' fire breath
            for (i = 0; i < NUM_FIRE_BREATHS; i++) {
                objectId = Objallocptr3[i];
                KWAnmStart(objectId, 0);
                DynamicObjects[objectId].pos[0] = FireBreathsSpawns[i][0] * XOrientation;
                DynamicObjects[objectId].pos[1] = FireBreathsSpawns[i][1];
                DynamicObjects[objectId].pos[2] = FireBreathsSpawns[i][2];
                DynamicObjects[objectId].ang[1] = 0;
                if (i % 2U) {
                    DynamicObjects[objectId].ang[1] += 0x8000;
                }
            }
            for (i = 0; i < 32; i++) {
                Kwdealloc(&Objallocptr4[i]);
            }
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (GameState != CREDITS_SEQUENCE) {
                objectId = Objallocptr[0];
                KwanmInitCI8(objectId, d_course_banshee_boardwalk_bat_tlut, *d_course_banshee_boardwalk_bat,
                                    0x20U, (u16) 0x00000040);
                DynamicObjects[objectId].Texang[0] = 0;
                DynamicObjects[objectId].Texang[1] = 0;
                DynamicObjects[objectId].Texang[2] = 0x8000;
                KWAnmStart(Objallocptr[1], 0);
                KWAnmStart(Objallocptr[2], 0);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            for (i = 0; i < NUM_YV_FLAG_POLES; i++) {
                KWAnmStart(Objallocptr[i], 0);
            }
            if (GameState != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_HEDGEHOGS; i++) {
                    objectId = Objallocptr2[i];
                    KWAnmStart(objectId, 0);
                    DynamicObjects[objectId].pos[0] = DynamicObjects[objectId].PosStart[0] =
                        HedgehogSpawns[i].Position[0] * XOrientation;
                    DynamicObjects[objectId].pos[1] = DynamicObjects[objectId].Ground =
                        HedgehogSpawns[i].Position[1] + 6.0;
                    DynamicObjects[objectId].pos[2] = DynamicObjects[objectId].PosStart[2] = HedgehogSpawns[i].Position[2];
                    DynamicObjects[objectId].Chartptr = HedgehogSpawns[i].Group;
                    DynamicObjects[objectId].ix = HedgehogPatrolPoints[i][0] * XOrientation;
                    DynamicObjects[objectId].iy = HedgehogPatrolPoints[i][2];
                }
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            for (i = 0; i < NUM_SNOWFLAKES; i++) {
                Kwalloc(&EffectAllocArray1[i]);
            }
            if (GameState != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_SNOWMEN; i++) {
                    objectId = Objallocptr2[i];
                    KWAnmStart(objectId, 0);
                    DynamicObjects[objectId].PosStart[0] = SnowmanSpawns[i].Position[0] * XOrientation;
                    DynamicObjects[objectId].PosStart[1] = SnowmanSpawns[i].Position[1] + 5.0 + 3.0;
                    DynamicObjects[objectId].PosStart[2] = SnowmanSpawns[i].Position[2];
                    objectId = Objallocptr[i];
                    KWAnmStart(objectId, 0);
                    DynamicObjects[objectId].PosStart[0] = SnowmanSpawns[i].Position[0] * XOrientation;
                    DynamicObjects[objectId].PosStart[1] = SnowmanSpawns[i].Position[1] + 3.0;
                    DynamicObjects[objectId].PosStart[2] = SnowmanSpawns[i].Position[2];
                    DynamicObjects[objectId].Chartptr = SnowmanSpawns[i].Group;
                }
            }
            break;
        case COURSE_KOOPA_BEACH:
            if (GameState != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_CRABS; i++) {
                    objectId = Objallocptr[i];
                    KWAnmStart(objectId, 0);
                    DynamicObjects[objectId].pos[0] = DynamicObjects[objectId].PosStart[0] =
                        CrabSpawns[i].StartX * XOrientation;
                    DynamicObjects[objectId].PosEnde[0] = CrabSpawns[i].PatrolX * XOrientation;

                    DynamicObjects[objectId].pos[2] = DynamicObjects[objectId].PosStart[2] = CrabSpawns[i].StartZ;
                    DynamicObjects[objectId].PosEnde[2] = CrabSpawns[i].PatrolZ;
                }
            }
            for (i = 0; i < NUM_SEAGULLS; i++) {
                objectId = Objallocptr2[i];
                KWAnmStart(objectId, 0);
                if (i < (NUM_SEAGULLS / 2)) {
                    DynamicObjects[objectId].Chartptr = 0;
                } else {
                    DynamicObjects[objectId].Chartptr = 1;
                }
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            if (GameState != CREDITS_SEQUENCE) {
                if (GameMode == GRAND_PRIX) {
                    KwinitFuusen();
                }
                for (i = 0; i < D_80165738; i++) {
                    Kwalloc(&EffectAllocArray3[i]);
                    KWAnmStart(EffectAllocArray3[i], 0);
                }
            }
            break;
        case COURSE_LUIGI_RACEWAY:
            if (GameState != CREDITS_SEQUENCE) {
                if (GameMode == GRAND_PRIX) {
                    KwinitFuusen();
                }
                Balloonsw = 0;
                KWAnmStart(Objallocptr[0], 0);
                for (i = 0; i < D_80165738; i++) {
                    Kwalloc(&EffectAllocArray3[i]);
                    KWAnmStart(EffectAllocArray3[i], 0);
                }
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (GameState != CREDITS_SEQUENCE) {
                if ((MenuMultiplayerSelection == 1) || ((MenuMultiplayerSelection == 2) && (GameMode == VERSUS))) {
                    switch (RaceClass) { /* switch 2; irregular */
                        case CC_50:         /* switch 2 */
                            D_8018D1C8 = 4;
                            D_8018D1D0 = 6;
                            D_8018D1D8 = 6;
                            break;
                        case CC_100: /* switch 2 */
                            D_8018D1C8 = 5;
                            D_8018D1D0 = 8;
                            D_8018D1D8 = 8;
                            break;
                        case CC_150: /* switch 2 */
                            D_8018D1C8 = 5;
                            D_8018D1D0 = 8;
                            D_8018D1D8 = 10;
                            break;
                        case CC_EXTRA: /* switch 2 */
                            D_8018D1C8 = 5;
                            D_8018D1D0 = 8;
                            D_8018D1D8 = 8;
                            break;
                    }
                } else {
                    D_8018D1C8 = 4;
                    D_8018D1D0 = 6;
                    D_8018D1D8 = 6;
                }
                for (i = 0; i < NUM_GROUP1_MOLES; i++) {
                    D_8018D198[i] = 0;
                    Kwalloc(&Objallocptr[i]);
                }
                for (i = 0; i < NUM_GROUP2_MOLES; i++) {
                    D_8018D1A8[i] = 0;
                    Kwalloc(&Objallocptr[i]);
                }
                for (i = 0; i < NUM_GROUP3_MOLES; i++) {
                    D_8018D1B8[i] = 0;
                    Kwalloc(&Objallocptr[i]);
                }
                for (i = 0; i < NUM_TOTAL_MOLES; i++) {
                    Kwalloc(&EffectAllocArray1[i]);
                    objectId = EffectAllocArray1[i];
                    KWAnmStart(objectId, 0);
                    DynamicObjects[objectId].pos[0] = MoleSpawns.AsVec3SList[i][0] * XOrientation;
                    DynamicObjects[objectId].pos[2] = MoleSpawns.AsVec3SList[i][2];
                    KwanmSetholeParams(objectId);
                    DynamicObjects[objectId].Scale = 0.7f;
                }
                for (i = 0; i < gObjectParticle2_SIZE; i++) {
                    Kwalloc(&EffectAllocArray2[i]);
                }
            }
            break;
        case COURSE_KALAMARI_DESERT:
            if (GameState != CREDITS_SEQUENCE) {
                Kwalloc(&D_8018CF10);
                KWAnmStart(D_8018CF10, 0);
                for (i = 0; i < 50; i++) {
                    Kwalloc(&EffectAllocArray1[i]);
                }
                for (i = 0; i < 5; i++) {
                    Kwalloc(&EffectAllocArray2[i]);
                }
                for (i = 0; i < 32; i++) {
                    Kwalloc(&EffectAllocArray3[i]);
                }
            }
            break;
        case COURSE_SHERBET_LAND:
            for (i = 0; i < NUM_PENGUINS; i++) {
                KWAnmStart(Objallocptr[i], 0);
            }
            break;
        case COURSE_RAINBOW_ROAD:
            if (GameState != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_NEON_SIGNS; i++) {
                    KWAnmStart(Objallocptr[i], 0);
                }
                for (i = 0; i < NUM_CHAIN_CHOMPS; i++) {
                    KWAnmStart(Objallocptr2[i], 0);
                }
            }
            break;
        case COURSE_DK_JUNGLE:
            for (i = 0; i < NUM_TORCHES; i++) {
                KWGetCaveFire(i);
                // wtf?
                if (D_8018CF10) {}
            }
            break;
        default:
            break;
    }
#else

#endif
}

void InitializeParams1P(void) {
    s32 someIndex;
    f32 something;
    // permuter magic
    long long why;
    s32 one = 1;

    D_8018D140 = 0;
    D_8018D150 = 0;
    D_8018CFCC = 1.0f;
    Kwalloc(&D_80183DA0);
    Kwalloc(&Jugemuallocptr[0]);
    Kwalloc(&Jugemuallocptr[1]);
    Kwalloc(&ItemBoxAllocPtr[0]);
    Kwalloc(&ItemBoxAllocPtr[1]);
    KwallocObjram();
    KWKumoAllocHookDefault();
    KWKumoAllocHookDefault();
    KwallocEffectram1P();
    PlayerHUD[PLAYER_ONE].mx = 0x0156;
    PlayerHUD[PLAYER_ONE].my = 0x0106;
    D_8018CFEC = PlayerHUD[PLAYER_ONE].mx + 0x18;
    D_8018CFF4 = PlayerHUD[PLAYER_ONE].my + 6;
    D_8016579E = 0xDD00;
    PlayerHUD[PLAYER_ONE].rx = 0x0034;
    PlayerHUD[PLAYER_ONE].ry = 0x00C8;
    PlayerHUD[PLAYER_ONE].rgx = 0;
    PlayerHUD[PLAYER_ONE].rgy = 0;
    PlayerHUD[PLAYER_ONE].Rank = GPCurrentRaceRankByPlayerId[0];
    PlayerHUD[PLAYER_ONE].timerX = 0x012C;
    PlayerHUD[PLAYER_ONE].lapCompletionTimeXs[0] = 0x012C;
    PlayerHUD[PLAYER_ONE].lapCompletionTimeXs[1] = 0x012C;
    PlayerHUD[PLAYER_ONE].ty = 0x0011;
    PlayerHUD[PLAYER_ONE].cx = -0x0028;
    PlayerHUD[PLAYER_ONE].cx2 = -0x0028;
    PlayerHUD[PLAYER_ONE].cx3 = -0x0028;
    PlayerHUD[PLAYER_ONE].cy = 0x0019;
    PlayerHUD[PLAYER_ONE].ix = 0x00A0;
    PlayerHUD[PLAYER_ONE].iy = -0x0020;
    PlayerHUD[PLAYER_ONE].Addix = 0;
    PlayerHUD[PLAYER_ONE].Addiy = 0;
    // permuter magic
    why = 0x000000A0;
    KwinitItembox(ItemBoxAllocPtr[0]);
    for (someIndex = 0, something = 35.0f; someIndex < 8; someIndex++, something += 32.0) {
        D_8018D0C8[someIndex] = 40.0f;
        Panelx[someIndex] = -24.0f;
        HUDCharpicRankY1[someIndex] = something;
        D_8018D0F0[someIndex] = something;
        D_8018D0A0[someIndex] = 0.0f;
        D_8018D078[someIndex] = 0.0f;
    }
    D_8018CFD4 = 1.0f;
    Kwr = Kwg = Kwb = 0x000000FF;
    Kwa = why;
    Wakur = 0x000000FF;
    Wakug = 0x000000FF;
    Wakub = 0x000000FF;
    Wakua = 0x000000FF;
    Wakuanmtmmax = one;
    PlayerHUD[PLAYER_ONE].py = 0x0078;
    PlayerHUD[PLAYER_ONE].px = 0x00A0;
    PlayerHUD[PLAYER_ONE].rs = 0.5f;
    D_801656B0 = 0;
    D_80165708 = 0x0028;
    D_8018D00C = 5.0f;
    D_8018D388 = 4;
    D_8018D380 = 0x00A0;
    D_8018D384 = 0x0078;
    D_8018D3C4 = 0x00000032;
    D_8018D3BC = 0x0028;
    D_8018D3C0 = 0x00000050;
    D_801657A2 = (4.5f * DEGREES(1));
    switch (GameMode) { /* irregular */
        case 0:
            Kwkartnum = 8;
            break;
        case 1:
            D_80165638 = (GetRecordLapTime() & 0xFFFFF) - 1;
            D_80165648 = GetRecordTime2(0) & 0xFFFFF;
            D_80165888 = 1;
            D_80165890 = 1;
            Kwkartnum = 1;
            break;
    }
}

void InitializeParams2Plr(void) {
    Kwalloc(&D_80183DA0);

    Kwalloc(&Jugemuallocptr[0]);
    Kwalloc(&Jugemuallocptr[1]);

    Kwalloc(&ItemBoxAllocPtr[0]);
    Kwalloc(&ItemBoxAllocPtr[1]);

    KwallocObjram();
    KWKumoAllocHookDefault();
    KWKumoAllocHookDefault();
    KwallocEffectram1P();

    PlayerHUD[PLAYER_ONE].ix = -0x52;
    PlayerHUD[PLAYER_ONE].iy = 0x32;
    PlayerHUD[PLAYER_ONE].Addix = 0;
    PlayerHUD[PLAYER_ONE].Addiy = 0;
    PlayerHUD[PLAYER_ONE].px = 0x50;
    PlayerHUD[PLAYER_ONE].py = 0x78;
    PlayerHUD[PLAYER_ONE].rx = 0x32;
    PlayerHUD[PLAYER_ONE].ry = 0xD2;
    PlayerHUD[PLAYER_ONE].rgx = 0;
    PlayerHUD[PLAYER_ONE].rgy = 0;
    PlayerHUD[PLAYER_ONE].timerX = 0x4B;
    PlayerHUD[PLAYER_ONE].ty = 0x10;
    PlayerHUD[PLAYER_ONE].cx = 0x67;
    PlayerHUD[PLAYER_ONE].cy = 0x28;
    KwinitItembox(ItemBoxAllocPtr[PLAYER_ONE]);

    PlayerHUD[PLAYER_TWO].ix = 0x43;
    PlayerHUD[PLAYER_TWO].iy = 0x32;
    PlayerHUD[PLAYER_TWO].Addix = 0;
    PlayerHUD[PLAYER_TWO].Addiy = 0;
    PlayerHUD[PLAYER_TWO].px = 0xF0;
    PlayerHUD[PLAYER_TWO].py = 0x78;
    PlayerHUD[PLAYER_TWO].rx = 0xC8;
    PlayerHUD[PLAYER_TWO].ry = 0xD2;
    PlayerHUD[PLAYER_TWO].rgx = 0;
    PlayerHUD[PLAYER_TWO].rgy = 0;
    PlayerHUD[PLAYER_TWO].timerX = 0xDC;
    PlayerHUD[PLAYER_TWO].ty = 0x10;
    PlayerHUD[PLAYER_TWO].cx = 0xF7;
    PlayerHUD[PLAYER_TWO].cy = 0x28;
    KwinitItembox(ItemBoxAllocPtr[PLAYER_TWO]);

    PlayerHUD[PLAYER_ONE].Rank = (s16) GPCurrentRaceRankByPlayerId[0];
    PlayerHUD[PLAYER_TWO].Rank = (s16) GPCurrentRaceRankByPlayerId[1];

    PlayerHUD[PLAYER_ONE].rs = PlayerHUD[PLAYER_TWO].rs = 0.5f;

    D_8018D3C4 = 0x1E;
    D_8018D3BC = 0x18;
    D_8018D3C0 = 0x28;
    D_801657A2 = DEGREES(9);
    switch (GameMode) { /* irregular */
        case GRAND_PRIX:
            Kwkartnum = 8;
            break;
        case VERSUS:
            Kwkartnum = 2;
            break;
        case BATTLE:
            Kwkartnum = 2;
            break;
    }
}

void InitializeParams2Pud() {
    Kwalloc(&D_80183DA0);

    Kwalloc(&Jugemuallocptr[0]);
    Kwalloc(&Jugemuallocptr[1]);

    Kwalloc(&ItemBoxAllocPtr[0]);
    Kwalloc(&ItemBoxAllocPtr[1]);

    KwallocObjram();
    KWKumoAllocHookDefault();
    KWKumoAllocHookDefault();
    KwallocEffectram1P();

    PlayerHUD[PLAYER_ONE].iy = 0x22;
    PlayerHUD[PLAYER_ONE].ix = -0x53;
    PlayerHUD[PLAYER_ONE].Addix = 0;
    PlayerHUD[PLAYER_ONE].Addiy = 0;
    PlayerHUD[PLAYER_ONE].px = 0xA0;
    PlayerHUD[PLAYER_ONE].py = 0x3C;
    PlayerHUD[PLAYER_ONE].rx = 0x34;
    PlayerHUD[PLAYER_ONE].ry = 0x62;
    PlayerHUD[PLAYER_ONE].rgx = 0;
    PlayerHUD[PLAYER_ONE].rgy = 0;
    PlayerHUD[PLAYER_ONE].timerX = 0xEA;
    PlayerHUD[PLAYER_ONE].ty = 0x10;
    PlayerHUD[PLAYER_ONE].cx = 0x101;
    PlayerHUD[PLAYER_ONE].cy = 0x6A;

    PlayerHUD[PLAYER_TWO].ix = -0x53;
    PlayerHUD[PLAYER_TWO].iy = 0x8F;
    PlayerHUD[PLAYER_TWO].Addix = 0;
    PlayerHUD[PLAYER_TWO].Addiy = 0;
    PlayerHUD[PLAYER_TWO].px = 0xA0;
    PlayerHUD[PLAYER_TWO].py = 0xB4;
    PlayerHUD[PLAYER_TWO].rx = 0x34;
    PlayerHUD[PLAYER_TWO].ry = 0xD2;
    PlayerHUD[PLAYER_TWO].rgx = 0;
    PlayerHUD[PLAYER_TWO].rgy = 0;
    PlayerHUD[PLAYER_TWO].timerX = 0xEA;
    PlayerHUD[PLAYER_TWO].ty = 0x7F;
    PlayerHUD[PLAYER_TWO].cx = 0x101;
    PlayerHUD[PLAYER_TWO].cy = 0xDA;

    if (GameMode == BATTLE) {
        PlayerHUD[PLAYER_ONE].iy = 0x5E;
        PlayerHUD[PLAYER_TWO].iy = 0xD0;
    }

    PlayerHUD[PLAYER_ONE].rs = PlayerHUD[PLAYER_TWO].rs = 0.5f;

    PlayerHUD[PLAYER_ONE].Rank = (s16) GPCurrentRaceRankByPlayerId[0];
    PlayerHUD[PLAYER_TWO].Rank = (s16) GPCurrentRaceRankByPlayerId[1];

    KwinitItembox(ItemBoxAllocPtr[0]);
    KwinitItembox((ItemBoxAllocPtr[1]));

    D_8018D3C4 = 0x1E;
    D_8018D3BC = 0x18;
    D_8018D3C0 = 0x28;
    D_801657A2 = DEGREES(9);
    switch (GameMode) { /* irregular */
        case GRAND_PRIX:
            Kwkartnum = 8;
            return;
        case VERSUS:
            Kwkartnum = 2;
            return;
        case BATTLE:
            Kwkartnum = 2;
            return;
    }
}

void InitializeParams4P(void) {
    Kwalloc(&D_80183DA0);

    Kwalloc(&Jugemuallocptr[0]);
    Kwalloc(&Jugemuallocptr[1]);
    Kwalloc(&Jugemuallocptr[2]);
    Kwalloc(&Jugemuallocptr[3]);

    Kwalloc(&ItemBoxAllocPtr[0]);
    Kwalloc(&ItemBoxAllocPtr[1]);
    Kwalloc(&ItemBoxAllocPtr[2]);
    Kwalloc(&ItemBoxAllocPtr[3]);

    KwallocObjram();
    KwallocEffectram1P();

    PlayerHUD[PLAYER_ONE].ix = -0x36;
    PlayerHUD[PLAYER_ONE].iy = 0x36;
    PlayerHUD[PLAYER_ONE].Addix = 0;
    PlayerHUD[PLAYER_ONE].Addiy = 0;
    PlayerHUD[PLAYER_ONE].px = 0x50;
    PlayerHUD[PLAYER_ONE].py = 0x3C;
    PlayerHUD[PLAYER_ONE].rx = 0x25;
    PlayerHUD[PLAYER_ONE].ry = 0x64;
    PlayerHUD[PLAYER_ONE].rgx = 0;
    PlayerHUD[PLAYER_ONE].rgy = 0;
    PlayerHUD[PLAYER_ONE].cx = 0x8C;
    PlayerHUD[PLAYER_ONE].cy = 0x60;
    PlayerHUD[PLAYER_ONE].Bombx = 0xDE;
    PlayerHUD[PLAYER_ONE].Bomby = 0xC8;

    PlayerHUD[PLAYER_TWO].ix = 0x175;
    PlayerHUD[PLAYER_TWO].iy = 0x36;
    PlayerHUD[PLAYER_TWO].Addix = 0;
    PlayerHUD[PLAYER_TWO].Addiy = 0;
    PlayerHUD[PLAYER_TWO].px = 0xF0;
    PlayerHUD[PLAYER_TWO].py = 0x3C;
    PlayerHUD[PLAYER_TWO].rx = 0x11A;
    PlayerHUD[PLAYER_TWO].ry = 0x64;
    PlayerHUD[PLAYER_TWO].rgx = 0;
    PlayerHUD[PLAYER_TWO].rgy = 0;
    PlayerHUD[PLAYER_TWO].cx = 0xB4;
    PlayerHUD[PLAYER_TWO].cy = 0x60;
    PlayerHUD[PLAYER_TWO].Bombx = 0xC8;
    PlayerHUD[PLAYER_TWO].Bomby = 0xC8;

    PlayerHUD[PLAYER_THREE].ix = -0x36;
    PlayerHUD[PLAYER_THREE].iy = 0x2D;
    PlayerHUD[PLAYER_THREE].Addix = 0;
    PlayerHUD[PLAYER_THREE].Addiy = 0;
    PlayerHUD[PLAYER_THREE].px = 0x50;
    PlayerHUD[PLAYER_THREE].py = 0xB4;
    PlayerHUD[PLAYER_THREE].rx = 0x25;
    PlayerHUD[PLAYER_THREE].ry = 0xD2;
    PlayerHUD[PLAYER_THREE].rgx = 0;
    PlayerHUD[PLAYER_THREE].rgy = 0;
    PlayerHUD[PLAYER_THREE].cx = 0x8C;
    PlayerHUD[PLAYER_THREE].cy = 0xD4;
    PlayerHUD[PLAYER_THREE].Bombx = 0xDE;
    PlayerHUD[PLAYER_THREE].Bomby = 0xC0;

    PlayerHUD[PLAYER_FOUR].ix = 0x175;
    PlayerHUD[PLAYER_FOUR].iy = 0x2D;
    PlayerHUD[PLAYER_FOUR].Addix = 0;
    PlayerHUD[PLAYER_FOUR].Addiy = 0;
    PlayerHUD[PLAYER_FOUR].px = 0xF0;
    PlayerHUD[PLAYER_FOUR].py = 0xB4;
    PlayerHUD[PLAYER_FOUR].rx = 0x11A;
    PlayerHUD[PLAYER_FOUR].ry = 0xD2;
    PlayerHUD[PLAYER_FOUR].rgx = 0;
    PlayerHUD[PLAYER_FOUR].rgy = 0;
    PlayerHUD[PLAYER_FOUR].cx = 0xB4;
    PlayerHUD[PLAYER_FOUR].cy = 0xD4;
    PlayerHUD[PLAYER_FOUR].Bombx = 0xC8;
    PlayerHUD[PLAYER_FOUR].Bomby = 0xC0;

    if (GameMode == BATTLE) {
        PlayerHUD[PLAYER_ONE].iy = 0xC8;
        PlayerHUD[PLAYER_TWO].iy = 0xC8;
        PlayerHUD[PLAYER_THREE].iy = 0xB8;
        PlayerHUD[PLAYER_FOUR].iy = 0xB8;
    }

    PlayerHUD[PLAYER_ONE].rs = PlayerHUD[PLAYER_TWO].rs = PlayerHUD[PLAYER_THREE].rs =
        PlayerHUD[PLAYER_FOUR].rs = 0.5f;

    PlayerHUD[PLAYER_ONE].Rank = (s16) GPCurrentRaceRankByPlayerId[0];
    PlayerHUD[PLAYER_TWO].Rank = (s16) GPCurrentRaceRankByPlayerId[1];
    PlayerHUD[PLAYER_THREE].Rank = (s16) GPCurrentRaceRankByPlayerId[2];
    PlayerHUD[PLAYER_FOUR].Rank = (s16) GPCurrentRaceRankByPlayerId[3];

    KwinitItembox(ItemBoxAllocPtr[0]);
    KwinitItembox(ItemBoxAllocPtr[1]);
    KwinitItembox(ItemBoxAllocPtr[2]);
    KwinitItembox(ItemBoxAllocPtr[3]);

    PlayerHUD[PLAYER_ONE].is = PlayerHUD[PLAYER_TWO].is =
        PlayerHUD[PLAYER_THREE].is = PlayerHUD[PLAYER_FOUR].is = 1.5f;

    Kwkartnum = (s32) MenuMultiplayerSelection;
    D_8018D3C4 = 0x00000014;
    D_8018D3BC = 0x00000010;
    D_8018D3C0 = 0x0000001E;
    D_801657A2 = DEGREES(12);
}
