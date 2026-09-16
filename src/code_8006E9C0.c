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
#include "OverKartHooks.h"

void initialize_params(void) {

    kwclear_ram();
    initialize_params_common();

    switch (g_ScreenSplitB) {
        case SCREEN_MODE_1P:
            initialize_params_1p();
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            initialize_params_2plr();
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            initialize_params_2pud();
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            initialize_params_4p();
            break;
    }
    initialize_params_common_after();
}

void kwclear_ram(void) {
    s32 i;
    s32 j;
    kwfreeallocptr();
    kwfreelist();
    bzero(playerHUD, HUD_PLAYERS_SIZE * sizeof(Hud));

    for (i = 0; i < HUD_PLAYERS_SIZE; i++) {
        playerHUD[i].goal = 0;
        playerHUD[i].lapcnt = 0;
        playerHUD[i].ranksw = 0;
    }
    for (j = 0; j < HUD_PLAYERS_SIZE; j++) {
        playerHUD[j].goalsw = 0;
    }
}

void kwfreeallocptr(void) {
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
    for (i = 0; i < gLeafParticle_SIZE; i++) { leafallocptr[i] = NULL_OBJECT_ID; }
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
    gNextFreeObjectParticle1 = gNextFreeObjectParticle2 = gNextFreeObjectParticle3 = FireParticleCounter =
        gNextFreeLeafParticle = 0;
}

void kwfreelist() {
    bzero(g_DynamicObjects, OBJECT_LIST_SIZE * sizeof(AnmObject));
    objectListSize = -1;
}

/**
 * Dma's mario kart 64 logo and course outline textures.
 */
u8* kwdma_melt(u8* devAddr, u8* baseAddress, u32 size, u32 offset) {
    u8** tempAddress;
    u8* address;
    address = baseAddress + offset;

    size = ALIGN16(size);
    osInvalDCache(address, (size));
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[((u32) devAddr) & 0xFFFFFF], address,
                 size, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    tempAddress = &address;
    decodeMIO0(*tempAddress, (u8*) baseAddress);
    return baseAddress;
}

void kwmelt_titlelogo(void) {
    logoptr =
        kwdma_melt((u8*) &logo_mario_kart_64, (u8*) tex_buff, 0x79E1, 0x20000);
}

// Some kind of initalization for the Item Window part of the HUD
void kwinit_itembox(s32 objectIndex) {
    ItemWindowObjects* temp_v0;

    temp_v0 = (ItemWindowObjects*) &g_DynamicObjects[objectIndex];
    temp_v0->output = ITEM_NONE;
    temp_v0->anmnumbak = temp_v0->output;
    temp_v0->stexaddr = (u8*) common_tlut_item_window_none;
    temp_v0->texaddr = (u8*) common_tlut_item_window_none;
    temp_v0->sindexaddr = common_texture_item_window_none;
    temp_v0->indexaddr = common_texture_item_window_none;
    temp_v0->timer = -1;
    temp_v0->ix = 0x00A0;  // Screen X position
    temp_v0->iy = -0x0020; // Screen Y position
    temp_v0->scale = 1.0f;
}

void kwmelt_roadmap(s32 courseId) {
    g_mapTexture = (s32) DecodeMapImage1(gCourseOutlineTextures[courseId], D_800E5520[courseId], D_800E5520[courseId]);
    // This is incredibly dumb. D_800E5548 ought to be something more like
    // `u16 D_800E5548[][2]` but that doesn't match for some insane reason
    g_mapWidth = D_800E5548[courseId * 2];
    g_mapHeight = D_800E5548[courseId * 2 + 1];
}

void kwmelt_kawanoroadmap(void) {
    s32 i;
    // `huh`'s and `i`'s types have to differ, for some reason
    s16 huh;
    u8* wut;

    wut = (u8*) &melt_buff[0x3FFFC000];
    // clang-format off
    // God forgive me for my sinT...
    huh = 0x14; if (0) {} for (i = 0; i < huh; i++) { D_8018D248[i] = kwdma_melt(gCourseOutlineTextures[i], wut, D_800E5520[i], D_800E5520[i]); wut += D_800E5520[i]; }
    // clang-format on
}

void kwmelt_common(void) {
    D_801655C8 = 0;
    xOrientation = 1.0f;
    if (g_ScreenFlip != 0) {
        xOrientation = -1.0f;
    }
    g_mapX[0] = 257;
    g_mapY[0] = 170;
    g_mapR = 255;
    g_mapG = 255;
    g_mapB = 255;
    ralpha = 255;
    if (g_courseID < NUM_COURSES - 1) {
        kwmelt_roadmap((s32) g_courseID);
    }
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust5, 0x443, 0x1000);
            g_mapScale = 0.022f;
            g_startX = 6;
            g_startY = 28;
            g_mapX[0] = 260;
            g_mapY[0] = 170;
            D_80165718 = 0;
            D_80165720 = 5;
            D_80165728 = -240;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            g_mapScale = 0.022f;
            g_mapX[0] = 265;
            g_startX = 19;
            g_startY = 37;
            break;
        case COURSE_BOWSER_CASTLE:
            g_mapX[0] = 265;
            g_mapScale = 0.0174f;
            g_startX = 12;
            g_startY = 48;
            break;
        case COURSE_BANSHEE_BOARDWALK:
            D_80165880 = (void*) DecodeMapImage1(gTextureGhosts, 0x4CC2, 0xD980);
            g_mapScale = 0.016f;
            g_mapX[0] = 0x0106;
            g_startX = 55;
            g_startY = 39;
            break;
        case COURSE_YOSHI_VALLEY:
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust0, 0x479, 0xC00);
            g_mapScale = 0.018f;
            g_startX = 61;
            g_startY = 38;
            break;
        case COURSE_FRAPPE_SNOWLAND:
            g_mapX[0] = 262;
            g_mapScale = 0.016f;
            g_startX = 36;
            g_startY = 40;
            g_mapR = 72;
            g_mapG = 100;
            g_mapB = 255;
            break;
        case COURSE_KOOPA_BEACH:
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust3, 0x3C8U, 0x1000);
            g_mapScale = 0.014f;
            g_mapX[0] = 268;
            g_startX = 40;
            g_startY = 21;
            break;
        case COURSE_ROYAL_RACEWAY:
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust4, 0x3F8, 0x1000);
            g_mapX[0] = 262;
            g_mapScale = 0.014f;
            g_startX = 37;
            g_startY = 50;
            D_80165718 = -64;
            D_80165720 = 5;
            D_80165728 = -330;
            break;
        case COURSE_LUIGI_RACEWAY:
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust2, 0x4F4U, 0xC00);
            g_mapScale = 0.0155f;
            g_mapX[0] = 271;
            g_startX = 45;
            g_startY = 60;
            D_80165718 = -140;
            D_80165720 = -44;
            D_80165728 = -215;
            break;
        case COURSE_MOO_MOO_FARM:
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust0, 0x479, 0xC00);
            g_mapScale = 0.0155f;
            g_mapX[0] = 271;
            g_startX = 18;
            g_startY = 36;
            break;
        case COURSE_TOADS_TURNPIKE:
            g_mapScale = 0.013f;
            g_mapX[0] = 252;
            g_startX = 57;
            g_startY = 44;
            break;
        case COURSE_KALAMARI_DESERT:
            g_mapX[0] = 263;
            g_mapY[0] = 165;
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust5, 0x443, 0x1000);
            g_mapScale = 0.015f;
            g_startX = 55;
            g_startY = 27;
            break;
        case COURSE_SHERBET_LAND:
            kumoptr = (void*) DecodeMapImage1(gTextureExhaust1, 0x485, 0xC00);
            g_mapScale = 0.015f;
            g_mapX[0] = 262;
            g_startX = 52;
            g_startY = 33;
            g_mapR = 72;
            g_mapG = 100;
            g_mapB = 255;
            break;
        case COURSE_RAINBOW_ROAD:
            g_mapScale = 0.0103f;
            g_mapX[0] = 261;
            g_mapY[0] = 166;
            g_startX = 39;
            g_startY = 55;
            break;
        case COURSE_WARIO_STADIUM:
            g_mapScale = 0.0155f;
            g_mapX[0] = 0x0106;
            g_startX = 53;
            g_startY = 35;
            break;
        case COURSE_BLOCK_FORT:
            g_mapScale = 0.0335f;
            g_startX = 32;
            g_startY = 32;
            break;
        case COURSE_SKYSCRAPER:
            g_mapScale = 0.0445f;
            g_startX = 32;
            g_startY = 32;
            break;
        case COURSE_DOUBLE_DECK:
            g_mapScale = 0.0285f;
            g_startX = 32;
            g_startY = 32;
            break;
        case COURSE_DK_JUNGLE:
            g_mapScale = 0.0155f;
            g_mapX[0] = 255;
            g_startX = 29;
            g_startY = 47;
            break;
        case COURSE_BIG_DONUT:
            g_mapScale = 0.0257f;
            g_startX = 32;
            g_startY = 31;
    }
#else

#endif
    if (g_ScreenFlip != 0) {
        g_startX = g_mapWidth - g_startX;
    }
    if (g_menuMultiplayerSelection == 4) {
        g_mapX[0] = 160;
        g_mapY[0] = 120;
        return;
    }
    if (g_menuMultiplayerSelection == 3) {
        g_mapX[0] = 235;
        g_mapY[0] = 175;
        return;
    }
    if (g_menuMultiplayerSelection == 2) {
        if (g_courseID != COURSE_TOADS_TURNPIKE) {
            g_mapX[1] = 265;
            g_mapX[0] = g_mapX[1];
        } else {
            g_mapX[1] = 255;
            g_mapX[0] = g_mapX[1];
        }
        g_mapY[0] = 65;
        g_mapY[1] = 180;
    }
}

void kwbackup_gamestatus(s32 arg0) {
    g_hudToggleFlag = g_hudLapToggle;
    g_hudSpeedToggle = g_hudSpeedToggle2;
    D_80165820 = g_blueLineRankToggle;
    D_80165818 = g_hudMapToggle2;
    D_80165828 = g_KWLapSW;
    g_hudToggleFlagP2[0] = D_80165800[0];
    g_hudToggleFlagP2[1] = D_80165800[1];
    if ((arg0 != 0) && (pause_flag == 0)) {
        playSound(SOUND_ACTION_PING);
    }
}

void kwscreen_init(void) {
    if (g_InGame == 0) {
        g_InGame = 1;
        g_hudLapToggle = 0;
        g_hudSpeedToggle2 = 0;
        g_blueLineRankToggle = 0;
        g_hudMapToggle2 = 1;
        D_80165800[0] = D_80165800[1] = 1;
        if (g_menuMultiplayerSelection == 4) {
            if (g_gameMode != BATTLE) {
                g_hudLapToggle = 1;
                g_blueLineRankToggle = 1;
                g_KWLapSW = 1;
                D_80165800[0] = D_80165800[1] = 0;
            } else {
                g_KWLapSW = 0;
                D_80165800[0] = D_80165800[1] = 1;
            }
        } else if (g_menuMultiplayerSelection == 3) {
            g_hudMapToggle2 = 0;
            g_KWLapSW = 1;
        } else if (g_menuMultiplayerSelection == 2) {
            if (g_gameMode != (s32) BATTLE) {
                g_hudLapToggle = 1;
                g_blueLineRankToggle = 1;
                D_80165800[0] = D_80165800[1] = 0;
            }
            g_mapY[0] = 0x0041;
            g_mapY[1] = 0x00B4;
        }
        kwbackup_gamestatus(0);
    } else {
        g_hudLapToggle = g_hudToggleFlag;
        g_hudSpeedToggle2 = g_hudSpeedToggle;
        g_blueLineRankToggle = D_80165820;
        g_hudMapToggle2 = D_80165818;
        g_KWLapSW = D_80165828;
        D_80165800[0] = g_hudToggleFlagP2[0];
        D_80165800[1] = g_hudToggleFlagP2[1];
    }
    if (g_DemoFlag != 0) {
        g_blueLineRankToggle = 0;
    }
}

void initialize_params_common(void) {
    s32 i;
    Player *player;

    kwscreen_init();
    kwmelt_common();
    osSetTime(0);
    D_8018D170 = 0;
    D_8018D190 = 0;
    g_KWScreenEnable = 0;
    D_8018D178 = 0;
    D_8018D1CC = 0;
    kwgoalpanelsw = 0;
    D_80165730 = 0;
    D_801658FE = 0;

    D_801657E1 = D_801657E3 = D_801657E5 = 0;

    D_80165658[0] = D_80165658[1] = D_80165658[2] = 0;
    D_801658BC = D_801658C6 = D_801658CE = D_801658DC = D_801658EC = D_801658F4 = D_801658E4 = D_801658D6 = 0;

    switch (g_menuMultiplayerSelection) {
    case 1:
        if (g_gameMode == GRAND_PRIX) {
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
        if (g_ScreenSplitB == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL) {
            if (g_gameMode == GRAND_PRIX) {
                D_8018D114 = 2;
                D_8018D178 = 150;
                D_8018D180 = 240;
                D_8018D2AC = 60;
            } else if (g_gameMode == VERSUS) {
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
        } else if (g_gameMode == GRAND_PRIX) {
            D_8018D114 = 5;
        } else if (g_gameMode == VERSUS) {
            D_8018D114 = 6;
        } else {
            D_8018D114 = 7;
        }
        break;
    case 3:
        if (g_gameMode == VERSUS) {
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
        if (g_gameMode == VERSUS) {
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
    if (g_DebugMode == 0) {
        D_8016576A = 0;
        D_8016579C = 0;
    }

    for (i = 0; i < g_menuMultiplayerSelection; i++) {
        D_8018CFC4[i] = D_8018CFAC[i] = D_8018CFBC[i] = 0;
        D_8018CFB4[i] = 0;
    }

    D_8018D204 = 1;
    D_8018D1FC = 0;
    D_8018D224 = 0;
    CloudCount1P = CloudCount2P = 0;
    kumotype = 0xFF;

    D_801655D8 = D_801655E8 = D_801655F8 = KWFlash8 = D_80165618 = D_80165628 = 0;
    D_8018D160 = 0;
    D_8018D1DC = 0;
    D_8018D1C4 = 0;
    D_8018D1B4 = 0;
    D_8018D1A0 = 0;
    D_8018D168 = 0;
    D_801656F0 = 0;
    D_801657AE = g_KWDBDispSW = D_8018D214 = g_KWDemoSW = D_801657B2 = 0;
    D_8018D20C = 0;
    rzerox = rzeroy = 0;
    kwgoalcnt = 3;
    D_8018D2AC = 0;
    D_8018D2A4 = D_8018D2B4 = D_8018D2BC = 0;
    D_8018D2C8[0] = D_8018D2C8[1] = D_8018D2C8[2] = D_8018D2C8[3] = 0;
    D_8016581C = 0;
    D_8016580C = 0;
    D_80165814 = 0;
    D_80165804 = 0;
    D_801657FC = 0;
    D_8018D174 = D_8018D17C = D_8018D16C = D_8018D184 = D_8018D18C = -1;
    player = gPlayerOne;
    for (i = 0; i < NUM_PLAYERS; i++) {
        D_8018D0F0[i] = g_hudCharpicRankY1[i] = -32.0f;
        D_8018CE10[i].unk_04[0] = D_8018CE10[i].unk_04[1] = D_8018CE10[i].unk_04[2] = 0.0f;
        D_8018CF50[i] = i;
        D_8018CF28[i] = player;
        player->link_doshin = -1;
        player++;
    }
}

void initialize_params_common_after(void) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 8; var_s0++) {
        kwalloc(&D_8018CE10[var_s0].objectIndex);
    }
}

void kwalloc_objram(void) {
    s32 loopIndex;

    for (loopIndex = 0; loopIndex < SOME_OBJECT_INDEX_LIST_SIZE; loopIndex++) {
        kwalloc(&objallocptr[loopIndex]);
        kwalloc(&objallocptr2[loopIndex]);
        kwalloc(&objallocptr3[loopIndex]);
        kwalloc(&objallocptr4[loopIndex]);
    }

    for (loopIndex = 0; loopIndex < NUM_BOMB_KARTS_VERSUS; loopIndex++) {
        kwalloc(&bomballocptr[loopIndex]);
    }
}

void kwinit_kumo(s32 objectIndex, s32 arg1, CloudData* arg2) {
    ItemWindowObjects* temp_v0;

    KWAnmStart(objectIndex, arg1);
    temp_v0 = (ItemWindowObjects*) &g_DynamicObjects[objectIndex];
    temp_v0->chartptr = arg2->subType;
    temp_v0->output = ITEM_NONE;
    temp_v0->ang[1] = arg2->rotY;
    temp_v0->iy = arg2->posY;
    temp_v0->scale = (f32) arg2->scalePercent / 100.0;
    temp_v0->indexaddr = (u8*) &kumoptr[arg2->subType];
    kwanm_init_common(objectIndex, 0x40U, 0x20U, D_0D005FB0);
    temp_v0->alpha = 0x00FF;
}

void kwset_kumoram(CloudData* cloudList) {
    s32 var_s0 = 0;
    CloudData* test = &cloudList[0];
    do {
        if (1) {}
        kwinit_kumo(kwalloc(&g_CloudAllocate[CloudCount2P + var_s0]), 1, test);
        var_s0++;
        test++;
    } while (test->rotY != 0xFFFF);
    CloudCount2P += var_s0;
    CloudCount1P = var_s0;
    kwhoshisw = 0;
}

/**
 * This function is part of the spawning for the "stars" in some stages
 *
 * arg2 is a pointer to some type of spawn data for the stars, although it not super clear
 * what types each element is. It seems like its a bunch of u16's, so maybe a Vec4su?
 *
 * The stars in Wario's Stadium, Toad's Turnpike, and Rainbow Road are not part of the skybox.
 * They are instead objects that seemingly hover in the air around the player
 * They have no true x/y/z position, instead they seem to be kept in a position relative to the
 * player they hang around. There is however an y rotation and y position for where they should be on screen
 * when they are visbile (unk_09E[0] and [1]).
 * sizeScaling is some sort of size scaling on the start texture.
 * unk_0A2 is an alpha value, used to make the star twinkle.
 **/
void kwinit_star(s32 objectIndex, s32 arg1, StarData* arg2) {
    ItemWindowObjects* temp_v0;

    KWAnmStart(objectIndex, arg1);
    temp_v0 = (ItemWindowObjects*) &g_DynamicObjects[objectIndex];
    temp_v0->chartptr = arg2->subType;
    temp_v0->output = ITEM_BANANA;
    temp_v0->ang[1] = arg2->rotY;
    temp_v0->iy = arg2->posY;                           // screen Y position
    temp_v0->scale = (f32) arg2->scalePercent / 100.0; // some type of scaling on the texture
    temp_v0->indexaddr = D_0D0293D8;
    kwanm_init_common(objectIndex, 0x10U, 0x10U, common_vtx_rectangle);
}

void kwset_starram(StarData* starList) {
    s32 var_s0 = 0;
    StarData* test = &starList[0];
    do {
        if (1) {}
        kwinit_star(kwalloc(&g_CloudAllocate[CloudCount2P + var_s0]), 1, test);
        var_s0++;
        test++;
    } while (test->rotY != 0xFFFF);
    CloudCount2P += var_s0;
    CloudCount1P = var_s0;
    kwhoshisw = 1;
}

void KWKumo_Alloc(void) {
    s32 var_s0;
    s32 var_s4;

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            // Uses Kalimari Desert's clouds for initialization?
            kwset_kumoram(gKalimariDesertClouds);
            break;
        case COURSE_YOSHI_VALLEY:
            kwset_kumoram(gYoshiValleyMooMooFarmClouds);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if (g_menuMultiplayerSelection == 1) {
                var_s4 = 0x32;
            } else {
                var_s4 = 0x19;
            }
            for (var_s0 = 0; var_s0 < var_s4; var_s0++) {
                kwalloc(&g_CloudAllocate[CloudCount2P + var_s0]);
            }
            CloudCount2P += var_s0;
            CloudCount1P = var_s0;
            break;
        case COURSE_KOOPA_BEACH:
            kwset_kumoram(gKoopaTroopaBeachClouds);
            break;
        case COURSE_ROYAL_RACEWAY:
            kwset_kumoram(gRoyalRacewayClouds);
            break;
        case COURSE_LUIGI_RACEWAY:
            kwset_kumoram(gLuigiRacewayClouds);
            break;
        case COURSE_MOO_MOO_FARM:
            kwset_kumoram(gYoshiValleyMooMooFarmClouds);
            break;
        case COURSE_TOADS_TURNPIKE:
            kwset_starram(gToadsTurnpikeRainbowRoadStars);
            break;
        case COURSE_KALAMARI_DESERT:
            kwset_kumoram(gKalimariDesertClouds);
            break;
        case COURSE_SHERBET_LAND:
            kwset_kumoram(gSherbetLandClouds);
            break;
        case COURSE_RAINBOW_ROAD:
            kwset_starram(gToadsTurnpikeRainbowRoadStars);
            break;
        case COURSE_WARIO_STADIUM:
            kwset_starram(gWarioStadiumStars);
            break;
    }
#else

#endif
    kwdebug_kumoram();
}

void kwinit_fuusen(void) {
    D_80165730 = 1;
    if (g_menuMultiplayerSelection == ONE_PLAYERS_SELECTED) {
        D_80165738 = 0x64;
        D_80165740 = 0x3C;
        D_80165748 = 0x1E;
        return;
    }
    D_80165738 = 0x32;
    D_80165740 = 0x1E;
    D_80165748 = 0xA;
}

void kwalloc_effectram_1p(void) {
    s32 objectId;
    s32 i;

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            if (gGamestate != 9) {
                if (g_gameMode == GRAND_PRIX) {
                    kwinit_fuusen();
                }
                for (i = 0; i < D_80165738; i++) {
                    kwalloc(&EffectAllocArray3[i]);
                    KWAnmStart(EffectAllocArray3[i], 0);
                }
            }
            break;
        case COURSE_BOWSER_CASTLE:
            gNumActiveThwomps = NUM_THWOMPS_100CC_EXTRA;
            gThowmpSpawnList = gThwompSpawns100CCExtra;
            switch (g_raceClass) { /* switch 1; irregular */
                case CC_100:        /* switch 1 */
                case CC_EXTRA:      /* switch 1 */
                    break;
                case CC_50: /* switch 1 */
                    gNumActiveThwomps = NUM_THWOMPS_50CC;
                    gThowmpSpawnList = gThomwpSpawns50CC;
                    break;
                case CC_150: /* switch 1 */
                    gNumActiveThwomps = NUM_THWOMPS_150CC;
                    gThowmpSpawnList = gThomwpSpawns150CC;
                    break;
            }
            for (i = 0; i < gNumActiveThwomps; i++) {
                objectId = objallocptr[i];
                KWAnmStart(objectId, 0);
                g_DynamicObjects[objectId].pos_start[0] = gThowmpSpawnList[i].startX * xOrientation;
                g_DynamicObjects[objectId].pos_start[2] = gThowmpSpawnList[i].startZ;
                g_DynamicObjects[objectId].chartptr = gThowmpSpawnList[i].unk_4;
                g_DynamicObjects[objectId].alpha = gThowmpSpawnList[i].unk_6;
            }
            // Handle the big statue's fire breath
            objectId = objallocptr2[0];
            KWAnmStart(objectId, 0);
            g_DynamicObjects[objectId].pos[0] = -68.0 * xOrientation;
            g_DynamicObjects[objectId].pos[1] = 80.0f;
            g_DynamicObjects[objectId].pos[2] = -1840.0f;
            // Handle the smaller statues' fire breath
            for (i = 0; i < NUM_FIRE_BREATHS; i++) {
                objectId = objallocptr3[i];
                KWAnmStart(objectId, 0);
                g_DynamicObjects[objectId].pos[0] = gFireBreathsSpawns[i][0] * xOrientation;
                g_DynamicObjects[objectId].pos[1] = gFireBreathsSpawns[i][1];
                g_DynamicObjects[objectId].pos[2] = gFireBreathsSpawns[i][2];
                g_DynamicObjects[objectId].ang[1] = 0;
                if (i % 2U) {
                    g_DynamicObjects[objectId].ang[1] += 0x8000;
                }
            }
            for (i = 0; i < 32; i++) {
                kwdealloc(&objallocptr4[i]);
            }
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (gGamestate != CREDITS_SEQUENCE) {
                objectId = objallocptr[0];
                kwanm_init_ci8(objectId, d_course_banshee_boardwalk_bat_tlut, *d_course_banshee_boardwalk_bat,
                                    0x20U, (u16) 0x00000040);
                g_DynamicObjects[objectId].texang[0] = 0;
                g_DynamicObjects[objectId].texang[1] = 0;
                g_DynamicObjects[objectId].texang[2] = 0x8000;
                KWAnmStart(objallocptr[1], 0);
                KWAnmStart(objallocptr[2], 0);
            }
            break;
        case COURSE_YOSHI_VALLEY:
            for (i = 0; i < NUM_YV_FLAG_POLES; i++) {
                KWAnmStart(objallocptr[i], 0);
            }
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_HEDGEHOGS; i++) {
                    objectId = objallocptr2[i];
                    KWAnmStart(objectId, 0);
                    g_DynamicObjects[objectId].pos[0] = g_DynamicObjects[objectId].pos_start[0] =
                        gHedgehogSpawns[i].Position[0] * xOrientation;
                    g_DynamicObjects[objectId].pos[1] = g_DynamicObjects[objectId].ground =
                        gHedgehogSpawns[i].Position[1] + 6.0;
                    g_DynamicObjects[objectId].pos[2] = g_DynamicObjects[objectId].pos_start[2] = gHedgehogSpawns[i].Position[2];
                    g_DynamicObjects[objectId].chartptr = gHedgehogSpawns[i].Group;
                    g_DynamicObjects[objectId].ix = gHedgehogPatrolPoints[i][0] * xOrientation;
                    g_DynamicObjects[objectId].iy = gHedgehogPatrolPoints[i][2];
                }
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            for (i = 0; i < NUM_SNOWFLAKES; i++) {
                kwalloc(&EffectAllocArray1[i]);
            }
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_SNOWMEN; i++) {
                    objectId = objallocptr2[i];
                    KWAnmStart(objectId, 0);
                    g_DynamicObjects[objectId].pos_start[0] = gSnowmanSpawns[i].Position[0] * xOrientation;
                    g_DynamicObjects[objectId].pos_start[1] = gSnowmanSpawns[i].Position[1] + 5.0 + 3.0;
                    g_DynamicObjects[objectId].pos_start[2] = gSnowmanSpawns[i].Position[2];
                    objectId = objallocptr[i];
                    KWAnmStart(objectId, 0);
                    g_DynamicObjects[objectId].pos_start[0] = gSnowmanSpawns[i].Position[0] * xOrientation;
                    g_DynamicObjects[objectId].pos_start[1] = gSnowmanSpawns[i].Position[1] + 3.0;
                    g_DynamicObjects[objectId].pos_start[2] = gSnowmanSpawns[i].Position[2];
                    g_DynamicObjects[objectId].chartptr = gSnowmanSpawns[i].Group;
                }
            }
            break;
        case COURSE_KOOPA_BEACH:
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_CRABS; i++) {
                    objectId = objallocptr[i];
                    KWAnmStart(objectId, 0);
                    g_DynamicObjects[objectId].pos[0] = g_DynamicObjects[objectId].pos_start[0] =
                        gCrabSpawns[i].startX * xOrientation;
                    g_DynamicObjects[objectId].pos_ende[0] = gCrabSpawns[i].patrolX * xOrientation;

                    g_DynamicObjects[objectId].pos[2] = g_DynamicObjects[objectId].pos_start[2] = gCrabSpawns[i].startZ;
                    g_DynamicObjects[objectId].pos_ende[2] = gCrabSpawns[i].patrolZ;
                }
            }
            for (i = 0; i < NUM_SEAGULLS; i++) {
                objectId = objallocptr2[i];
                KWAnmStart(objectId, 0);
                if (i < (NUM_SEAGULLS / 2)) {
                    g_DynamicObjects[objectId].chartptr = 0;
                } else {
                    g_DynamicObjects[objectId].chartptr = 1;
                }
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            if (gGamestate != CREDITS_SEQUENCE) {
                if (g_gameMode == GRAND_PRIX) {
                    kwinit_fuusen();
                }
                for (i = 0; i < D_80165738; i++) {
                    kwalloc(&EffectAllocArray3[i]);
                    KWAnmStart(EffectAllocArray3[i], 0);
                }
            }
            break;
        case COURSE_LUIGI_RACEWAY:
            if (gGamestate != CREDITS_SEQUENCE) {
                if (g_gameMode == GRAND_PRIX) {
                    kwinit_fuusen();
                }
                balloonsw = 0;
                KWAnmStart(objallocptr[0], 0);
                for (i = 0; i < D_80165738; i++) {
                    kwalloc(&EffectAllocArray3[i]);
                    KWAnmStart(EffectAllocArray3[i], 0);
                }
            }
            break;
        case COURSE_MOO_MOO_FARM:
            if (gGamestate != CREDITS_SEQUENCE) {
                if ((g_menuMultiplayerSelection == 1) || ((g_menuMultiplayerSelection == 2) && (g_gameMode == VERSUS))) {
                    switch (g_raceClass) { /* switch 2; irregular */
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
                    kwalloc(&objallocptr[i]);
                }
                for (i = 0; i < NUM_GROUP2_MOLES; i++) {
                    D_8018D1A8[i] = 0;
                    kwalloc(&objallocptr[i]);
                }
                for (i = 0; i < NUM_GROUP3_MOLES; i++) {
                    D_8018D1B8[i] = 0;
                    kwalloc(&objallocptr[i]);
                }
                for (i = 0; i < NUM_TOTAL_MOLES; i++) {
                    kwalloc(&EffectAllocArray1[i]);
                    objectId = EffectAllocArray1[i];
                    KWAnmStart(objectId, 0);
                    g_DynamicObjects[objectId].pos[0] = gMoleSpawns.asVec3sList[i][0] * xOrientation;
                    g_DynamicObjects[objectId].pos[2] = gMoleSpawns.asVec3sList[i][2];
                    kwanm_sethole_params(objectId);
                    g_DynamicObjects[objectId].scale = 0.7f;
                }
                for (i = 0; i < gObjectParticle2_SIZE; i++) {
                    kwalloc(&EffectAllocArray2[i]);
                }
            }
            break;
        case COURSE_KALAMARI_DESERT:
            if (gGamestate != CREDITS_SEQUENCE) {
                kwalloc(&D_8018CF10);
                KWAnmStart(D_8018CF10, 0);
                for (i = 0; i < 50; i++) {
                    kwalloc(&EffectAllocArray1[i]);
                }
                for (i = 0; i < 5; i++) {
                    kwalloc(&EffectAllocArray2[i]);
                }
                for (i = 0; i < 32; i++) {
                    kwalloc(&EffectAllocArray3[i]);
                }
            }
            break;
        case COURSE_SHERBET_LAND:
            for (i = 0; i < NUM_PENGUINS; i++) {
                KWAnmStart(objallocptr[i], 0);
            }
            break;
        case COURSE_RAINBOW_ROAD:
            if (gGamestate != CREDITS_SEQUENCE) {
                for (i = 0; i < NUM_NEON_SIGNS; i++) {
                    KWAnmStart(objallocptr[i], 0);
                }
                for (i = 0; i < NUM_CHAIN_CHOMPS; i++) {
                    KWAnmStart(objallocptr2[i], 0);
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

void initialize_params_1p(void) {
    s32 someIndex;
    f32 something;
    // permuter magic
    long long why;
    s32 one = 1;

    D_8018D140 = 0;
    D_8018D150 = 0;
    D_8018CFCC = 1.0f;
    kwalloc(&D_80183DA0);
    kwalloc(&jugemuallocptr[0]);
    kwalloc(&jugemuallocptr[1]);
    kwalloc(&ItemBoxAllocPtr[0]);
    kwalloc(&ItemBoxAllocPtr[1]);
    kwalloc_objram();
    KWKumo_Alloc_Hook_Default();
    KWKumo_Alloc_Hook_Default();
    kwalloc_effectram_1p();
    playerHUD[PLAYER_ONE].mx = 0x0156;
    playerHUD[PLAYER_ONE].my = 0x0106;
    D_8018CFEC = playerHUD[PLAYER_ONE].mx + 0x18;
    D_8018CFF4 = playerHUD[PLAYER_ONE].my + 6;
    D_8016579E = 0xDD00;
    playerHUD[PLAYER_ONE].rx = 0x0034;
    playerHUD[PLAYER_ONE].ry = 0x00C8;
    playerHUD[PLAYER_ONE].rgx = 0;
    playerHUD[PLAYER_ONE].rgy = 0;
    playerHUD[PLAYER_ONE].rank = gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_ONE].timerX = 0x012C;
    playerHUD[PLAYER_ONE].lapCompletionTimeXs[0] = 0x012C;
    playerHUD[PLAYER_ONE].lapCompletionTimeXs[1] = 0x012C;
    playerHUD[PLAYER_ONE].ty = 0x0011;
    playerHUD[PLAYER_ONE].cx = -0x0028;
    playerHUD[PLAYER_ONE].cx2 = -0x0028;
    playerHUD[PLAYER_ONE].cx3 = -0x0028;
    playerHUD[PLAYER_ONE].cy = 0x0019;
    playerHUD[PLAYER_ONE].ix = 0x00A0;
    playerHUD[PLAYER_ONE].iy = -0x0020;
    playerHUD[PLAYER_ONE].addix = 0;
    playerHUD[PLAYER_ONE].addiy = 0;
    // permuter magic
    why = 0x000000A0;
    kwinit_itembox(ItemBoxAllocPtr[0]);
    for (someIndex = 0, something = 35.0f; someIndex < 8; someIndex++, something += 32.0) {
        D_8018D0C8[someIndex] = 40.0f;
        panelx[someIndex] = -24.0f;
        g_hudCharpicRankY1[someIndex] = something;
        D_8018D0F0[someIndex] = something;
        D_8018D0A0[someIndex] = 0.0f;
        D_8018D078[someIndex] = 0.0f;
    }
    D_8018CFD4 = 1.0f;
    kwr = kwg = kwb = 0x000000FF;
    kwa = why;
    wakur = 0x000000FF;
    wakug = 0x000000FF;
    wakub = 0x000000FF;
    wakua = 0x000000FF;
    wakuanmtmmax = one;
    playerHUD[PLAYER_ONE].py = 0x0078;
    playerHUD[PLAYER_ONE].px = 0x00A0;
    playerHUD[PLAYER_ONE].rs = 0.5f;
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
    switch (g_gameMode) { /* irregular */
        case 0:
            kwkartnum = 8;
            break;
        case 1:
            D_80165638 = (GetRecordLapTime() & 0xFFFFF) - 1;
            D_80165648 = GetRecordTime2(0) & 0xFFFFF;
            D_80165888 = 1;
            D_80165890 = 1;
            kwkartnum = 1;
            break;
    }
}

void initialize_params_2plr(void) {
    kwalloc(&D_80183DA0);

    kwalloc(&jugemuallocptr[0]);
    kwalloc(&jugemuallocptr[1]);

    kwalloc(&ItemBoxAllocPtr[0]);
    kwalloc(&ItemBoxAllocPtr[1]);

    kwalloc_objram();
    KWKumo_Alloc_Hook_Default();
    KWKumo_Alloc_Hook_Default();
    kwalloc_effectram_1p();

    playerHUD[PLAYER_ONE].ix = -0x52;
    playerHUD[PLAYER_ONE].iy = 0x32;
    playerHUD[PLAYER_ONE].addix = 0;
    playerHUD[PLAYER_ONE].addiy = 0;
    playerHUD[PLAYER_ONE].px = 0x50;
    playerHUD[PLAYER_ONE].py = 0x78;
    playerHUD[PLAYER_ONE].rx = 0x32;
    playerHUD[PLAYER_ONE].ry = 0xD2;
    playerHUD[PLAYER_ONE].rgx = 0;
    playerHUD[PLAYER_ONE].rgy = 0;
    playerHUD[PLAYER_ONE].timerX = 0x4B;
    playerHUD[PLAYER_ONE].ty = 0x10;
    playerHUD[PLAYER_ONE].cx = 0x67;
    playerHUD[PLAYER_ONE].cy = 0x28;
    kwinit_itembox(ItemBoxAllocPtr[PLAYER_ONE]);

    playerHUD[PLAYER_TWO].ix = 0x43;
    playerHUD[PLAYER_TWO].iy = 0x32;
    playerHUD[PLAYER_TWO].addix = 0;
    playerHUD[PLAYER_TWO].addiy = 0;
    playerHUD[PLAYER_TWO].px = 0xF0;
    playerHUD[PLAYER_TWO].py = 0x78;
    playerHUD[PLAYER_TWO].rx = 0xC8;
    playerHUD[PLAYER_TWO].ry = 0xD2;
    playerHUD[PLAYER_TWO].rgx = 0;
    playerHUD[PLAYER_TWO].rgy = 0;
    playerHUD[PLAYER_TWO].timerX = 0xDC;
    playerHUD[PLAYER_TWO].ty = 0x10;
    playerHUD[PLAYER_TWO].cx = 0xF7;
    playerHUD[PLAYER_TWO].cy = 0x28;
    kwinit_itembox(ItemBoxAllocPtr[PLAYER_TWO]);

    playerHUD[PLAYER_ONE].rank = (s16) gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_TWO].rank = (s16) gGPCurrentRaceRankByPlayerId[1];

    playerHUD[PLAYER_ONE].rs = playerHUD[PLAYER_TWO].rs = 0.5f;

    D_8018D3C4 = 0x1E;
    D_8018D3BC = 0x18;
    D_8018D3C0 = 0x28;
    D_801657A2 = DEGREES(9);
    switch (g_gameMode) { /* irregular */
        case GRAND_PRIX:
            kwkartnum = 8;
            break;
        case VERSUS:
            kwkartnum = 2;
            break;
        case BATTLE:
            kwkartnum = 2;
            break;
    }
}

void initialize_params_2pud() {
    kwalloc(&D_80183DA0);

    kwalloc(&jugemuallocptr[0]);
    kwalloc(&jugemuallocptr[1]);

    kwalloc(&ItemBoxAllocPtr[0]);
    kwalloc(&ItemBoxAllocPtr[1]);

    kwalloc_objram();
    KWKumo_Alloc_Hook_Default();
    KWKumo_Alloc_Hook_Default();
    kwalloc_effectram_1p();

    playerHUD[PLAYER_ONE].iy = 0x22;
    playerHUD[PLAYER_ONE].ix = -0x53;
    playerHUD[PLAYER_ONE].addix = 0;
    playerHUD[PLAYER_ONE].addiy = 0;
    playerHUD[PLAYER_ONE].px = 0xA0;
    playerHUD[PLAYER_ONE].py = 0x3C;
    playerHUD[PLAYER_ONE].rx = 0x34;
    playerHUD[PLAYER_ONE].ry = 0x62;
    playerHUD[PLAYER_ONE].rgx = 0;
    playerHUD[PLAYER_ONE].rgy = 0;
    playerHUD[PLAYER_ONE].timerX = 0xEA;
    playerHUD[PLAYER_ONE].ty = 0x10;
    playerHUD[PLAYER_ONE].cx = 0x101;
    playerHUD[PLAYER_ONE].cy = 0x6A;

    playerHUD[PLAYER_TWO].ix = -0x53;
    playerHUD[PLAYER_TWO].iy = 0x8F;
    playerHUD[PLAYER_TWO].addix = 0;
    playerHUD[PLAYER_TWO].addiy = 0;
    playerHUD[PLAYER_TWO].px = 0xA0;
    playerHUD[PLAYER_TWO].py = 0xB4;
    playerHUD[PLAYER_TWO].rx = 0x34;
    playerHUD[PLAYER_TWO].ry = 0xD2;
    playerHUD[PLAYER_TWO].rgx = 0;
    playerHUD[PLAYER_TWO].rgy = 0;
    playerHUD[PLAYER_TWO].timerX = 0xEA;
    playerHUD[PLAYER_TWO].ty = 0x7F;
    playerHUD[PLAYER_TWO].cx = 0x101;
    playerHUD[PLAYER_TWO].cy = 0xDA;

    if (g_gameMode == BATTLE) {
        playerHUD[PLAYER_ONE].iy = 0x5E;
        playerHUD[PLAYER_TWO].iy = 0xD0;
    }

    playerHUD[PLAYER_ONE].rs = playerHUD[PLAYER_TWO].rs = 0.5f;

    playerHUD[PLAYER_ONE].rank = (s16) gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_TWO].rank = (s16) gGPCurrentRaceRankByPlayerId[1];

    kwinit_itembox(ItemBoxAllocPtr[0]);
    kwinit_itembox((ItemBoxAllocPtr[1]));

    D_8018D3C4 = 0x1E;
    D_8018D3BC = 0x18;
    D_8018D3C0 = 0x28;
    D_801657A2 = DEGREES(9);
    switch (g_gameMode) { /* irregular */
        case GRAND_PRIX:
            kwkartnum = 8;
            return;
        case VERSUS:
            kwkartnum = 2;
            return;
        case BATTLE:
            kwkartnum = 2;
            return;
    }
}

void initialize_params_4p(void) {
    kwalloc(&D_80183DA0);

    kwalloc(&jugemuallocptr[0]);
    kwalloc(&jugemuallocptr[1]);
    kwalloc(&jugemuallocptr[2]);
    kwalloc(&jugemuallocptr[3]);

    kwalloc(&ItemBoxAllocPtr[0]);
    kwalloc(&ItemBoxAllocPtr[1]);
    kwalloc(&ItemBoxAllocPtr[2]);
    kwalloc(&ItemBoxAllocPtr[3]);

    kwalloc_objram();
    kwalloc_effectram_1p();

    playerHUD[PLAYER_ONE].ix = -0x36;
    playerHUD[PLAYER_ONE].iy = 0x36;
    playerHUD[PLAYER_ONE].addix = 0;
    playerHUD[PLAYER_ONE].addiy = 0;
    playerHUD[PLAYER_ONE].px = 0x50;
    playerHUD[PLAYER_ONE].py = 0x3C;
    playerHUD[PLAYER_ONE].rx = 0x25;
    playerHUD[PLAYER_ONE].ry = 0x64;
    playerHUD[PLAYER_ONE].rgx = 0;
    playerHUD[PLAYER_ONE].rgy = 0;
    playerHUD[PLAYER_ONE].cx = 0x8C;
    playerHUD[PLAYER_ONE].cy = 0x60;
    playerHUD[PLAYER_ONE].bombx = 0xDE;
    playerHUD[PLAYER_ONE].bomby = 0xC8;

    playerHUD[PLAYER_TWO].ix = 0x175;
    playerHUD[PLAYER_TWO].iy = 0x36;
    playerHUD[PLAYER_TWO].addix = 0;
    playerHUD[PLAYER_TWO].addiy = 0;
    playerHUD[PLAYER_TWO].px = 0xF0;
    playerHUD[PLAYER_TWO].py = 0x3C;
    playerHUD[PLAYER_TWO].rx = 0x11A;
    playerHUD[PLAYER_TWO].ry = 0x64;
    playerHUD[PLAYER_TWO].rgx = 0;
    playerHUD[PLAYER_TWO].rgy = 0;
    playerHUD[PLAYER_TWO].cx = 0xB4;
    playerHUD[PLAYER_TWO].cy = 0x60;
    playerHUD[PLAYER_TWO].bombx = 0xC8;
    playerHUD[PLAYER_TWO].bomby = 0xC8;

    playerHUD[PLAYER_THREE].ix = -0x36;
    playerHUD[PLAYER_THREE].iy = 0x2D;
    playerHUD[PLAYER_THREE].addix = 0;
    playerHUD[PLAYER_THREE].addiy = 0;
    playerHUD[PLAYER_THREE].px = 0x50;
    playerHUD[PLAYER_THREE].py = 0xB4;
    playerHUD[PLAYER_THREE].rx = 0x25;
    playerHUD[PLAYER_THREE].ry = 0xD2;
    playerHUD[PLAYER_THREE].rgx = 0;
    playerHUD[PLAYER_THREE].rgy = 0;
    playerHUD[PLAYER_THREE].cx = 0x8C;
    playerHUD[PLAYER_THREE].cy = 0xD4;
    playerHUD[PLAYER_THREE].bombx = 0xDE;
    playerHUD[PLAYER_THREE].bomby = 0xC0;

    playerHUD[PLAYER_FOUR].ix = 0x175;
    playerHUD[PLAYER_FOUR].iy = 0x2D;
    playerHUD[PLAYER_FOUR].addix = 0;
    playerHUD[PLAYER_FOUR].addiy = 0;
    playerHUD[PLAYER_FOUR].px = 0xF0;
    playerHUD[PLAYER_FOUR].py = 0xB4;
    playerHUD[PLAYER_FOUR].rx = 0x11A;
    playerHUD[PLAYER_FOUR].ry = 0xD2;
    playerHUD[PLAYER_FOUR].rgx = 0;
    playerHUD[PLAYER_FOUR].rgy = 0;
    playerHUD[PLAYER_FOUR].cx = 0xB4;
    playerHUD[PLAYER_FOUR].cy = 0xD4;
    playerHUD[PLAYER_FOUR].bombx = 0xC8;
    playerHUD[PLAYER_FOUR].bomby = 0xC0;

    if (g_gameMode == BATTLE) {
        playerHUD[PLAYER_ONE].iy = 0xC8;
        playerHUD[PLAYER_TWO].iy = 0xC8;
        playerHUD[PLAYER_THREE].iy = 0xB8;
        playerHUD[PLAYER_FOUR].iy = 0xB8;
    }

    playerHUD[PLAYER_ONE].rs = playerHUD[PLAYER_TWO].rs = playerHUD[PLAYER_THREE].rs =
        playerHUD[PLAYER_FOUR].rs = 0.5f;

    playerHUD[PLAYER_ONE].rank = (s16) gGPCurrentRaceRankByPlayerId[0];
    playerHUD[PLAYER_TWO].rank = (s16) gGPCurrentRaceRankByPlayerId[1];
    playerHUD[PLAYER_THREE].rank = (s16) gGPCurrentRaceRankByPlayerId[2];
    playerHUD[PLAYER_FOUR].rank = (s16) gGPCurrentRaceRankByPlayerId[3];

    kwinit_itembox(ItemBoxAllocPtr[0]);
    kwinit_itembox(ItemBoxAllocPtr[1]);
    kwinit_itembox(ItemBoxAllocPtr[2]);
    kwinit_itembox(ItemBoxAllocPtr[3]);

    playerHUD[PLAYER_ONE].is = playerHUD[PLAYER_TWO].is =
        playerHUD[PLAYER_THREE].is = playerHUD[PLAYER_FOUR].is = 1.5f;

    kwkartnum = (s32) g_menuMultiplayerSelection;
    D_8018D3C4 = 0x00000014;
    D_8018D3BC = 0x00000010;
    D_8018D3C0 = 0x0000001E;
    D_801657A2 = DEGREES(12);
}
