#include <ultra64.h>
#include <macros.h>
#include <PR/gbi.h>
#include <mk64.h>
#include <course.h>

#include "skybox_and_splitscreen.h"
#include "code_800029B0.h"
#include <common_structs.h>
#include "memory.h"
#include "camera.h"
#include <assets/common_data.h>
#include "render_player.h"
#include "code_80057C60.h"
#include "menu_items.h"
#include "actors.h"
#include "render_courses.h"
#include "math_util.h"
#include "main.h"
#include "menus.h"

Vp D_802B8880[] = {
    { { { 640, 480, 511, 0 }, { 640, 480, 511, 0 } } },
};

static Vtx sSkyboxP1[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

static Vtx sSkyboxP2[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

static Vtx sSkyboxP3[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

static Vtx sSkyboxP4[] = {
    { { { SCREEN_WIDTH, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x1E, 0x1E, 0xFF, 0xFF } } },
    { { { 0, SCREEN_HEIGHT, -1 }, 0, { 0, 0 }, { 0xC8, 0xC8, 0xFF, 0xFF } } },
    { { { SCREEN_WIDTH, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
    { { { SCREEN_WIDTH, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 0, -1 }, 0, { 0, 0 }, { 0x78, 0xFF, 0x78, 0xFF } } },
    { { { 0, 120, -1 }, 0, { 0, 0 }, { 0x00, 0xDC, 0x00, 0xFF } } },
};

void SetViewport(Screen* arg0) {
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;
    s32 screenWidth = arg0->width * 2;
    s32 screenHeight = arg0->height * 2;
    s32 screenStartX = arg0->posx * 4;
    s32 screenStartY = arg0->posy * 4;

    arg0->viewport.vp.vscale[0] = screenWidth;
    arg0->viewport.vp.vscale[1] = screenHeight;
    arg0->viewport.vp.vscale[2] = 511;
    arg0->viewport.vp.vscale[3] = 0;

    arg0->viewport.vp.vtrans[0] = screenStartX;
    arg0->viewport.vp.vtrans[1] = screenStartY;
    arg0->viewport.vp.vtrans[2] = 511;
    arg0->viewport.vp.vtrans[3] = 0;

    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&arg0->viewport));

    screenWidth /= 4;
    screenHeight /= 4;

    screenStartX /= 4;
    screenStartY /= 4;

    lrx = screenStartX + screenWidth;
    if (lrx > SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH;
    }

    lry = screenStartY + screenHeight;
    if (lry > SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT;
    }
    ulx = 0;
    uly = 0;

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
}

UNUSED void func_802A38AC(void) {
}

void fadeout_sequence(void) {
    InitRDP();
    g_ClearFramebuffer();

    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);

    if (g_fadeOutCounter != 0) {
        g_fadeOutCounter--;
        return;
    }
    g_NewSequenceMode = next_sequence_mode;
    gGamestate = 255;
    g_fadeOutFlag = 0;
    g_fadeOutCounter = 0;
    KBGChange = FADE_MODE_MAIN;

    switch (next_sequence_mode) {
        case START_MENU_FROM_QUIT:
            if (KBGNumber != LOGO_INTRO_MENU) {
                KBGNumber = START_MENU;
            }
            break;
        case MAIN_MENU_FROM_QUIT:
            KBGNumber = MAIN_MENU;
            break;
        case PLAYER_SELECT_MENU_FROM_QUIT:
            KBGNumber = CHARACTER_SELECT_MENU;
            break;
        case COURSE_SELECT_MENU_FROM_QUIT:
            KBGNumber = COURSE_SELECT_MENU;
            break;
    }
}

void ClearZBuffer2(Screen* arg0) {
    s32 ulx = arg0->posx - (arg0->width / 2);
    s32 uly = arg0->posy - (arg0->height / 2);
    s32 lrx = arg0->posx + (arg0->width / 2);
    s32 lry = arg0->posy + (arg0->height / 2);

    if (ulx < 0) {
        ulx = 0;
    }
    if (uly < 0) {
        uly = 0;
    }
    if (lrx > SCREEN_WIDTH) {
        lrx = SCREEN_WIDTH;
    }
    if (lry > SCREEN_HEIGHT) {
        lry = SCREEN_HEIGHT;
    }
    if (ulx >= lrx) {
        lrx = ulx + 2;
    }
    if (uly >= lry) {
        lry = uly + 2;
    }

    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetDepthImage(gDisplayListHead++, gPhysicalZBuffer);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gPhysicalZBuffer);
    gDPSetFillColor(gDisplayListHead++, 0xFFFCFFFC);
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

    gDPFillRectangle(gDisplayListHead++, ulx, uly, lrx - 1, lry - 1);

    gDPPipeSync(gDisplayListHead++);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(g_CfbPtrs[draw_frame])); // 0x1FFFFFFF
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetDepthSource(gDisplayListHead++, G_ZS_PIXEL);
}

/**
 * Initialize the z-buffer for the current frame.
 */
void ClearZBuffer(void) {
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetDepthImage(gDisplayListHead++, gPhysicalZBuffer);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gPhysicalZBuffer);
    gDPSetFillColor(gDisplayListHead++, 0xFFFCFFFC);
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPFillRectangle(gDisplayListHead++, 0, 0, 319, 239);
    gDPPipeSync(gDisplayListHead++);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(g_CfbPtrs[draw_frame]));
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetDepthSource(gDisplayListHead++, G_ZS_PIXEL);
}

/**
 * Sets the initial RDP (Reality Display Processor) rendering settings.
 **/
void InitRDP(void) {
    gDPPipeSync(gDisplayListHead++);
    gDPPipelineMode(gDisplayListHead++, G_PM_1PRIMITIVE);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
    gDPSetTextureLOD(gDisplayListHead++, G_TL_TILE);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPSetTextureDetail(gDisplayListHead++, G_TD_CLAMP);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    gDPSetTextureConvert(gDisplayListHead++, G_TC_FILT);
    gDPSetCombineKey(gDisplayListHead++, G_CK_NONE);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetBlendMask(gDisplayListHead++, 0xFF);
    gDPSetColorDither(gDisplayListHead++, G_CD_DISABLE);
    gDPPipeSync(gDisplayListHead++);
    gSPClipRatio(gDisplayListHead++, FRUSTRATIO_1);
}

UNUSED void func_802A40A4(void) {
}
UNUSED void func_802A40AC(void) {
}
UNUSED void func_802A40B4(void) {
}
UNUSED void func_802A40BC(void) {
}
UNUSED void func_802A40C4(void) {
}
UNUSED void func_802A40CC(void) {
}
UNUSED void func_802A40D4(void) {
}
UNUSED void func_802A40DC(void) {
}

UNUSED s32 initRSP_full(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_800DC5EC->viewport));
    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++,
                       G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH | G_CLIPPING);
}

void InitRSP(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_802B8880));
    gSPClearGeometryMode(gDisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
}

/**
 * Tells the RDP which of the three framebuffers it shall draw to.
 */
void g_ClearFramebuffer(void) {
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(g_CfbPtrs[draw_frame]));
    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(D_800DC5D0, D_800DC5D4, D_800DC5D8, 1) << 0x10 |
                                            GPACK_RGBA5551(D_800DC5D0, D_800DC5D4, D_800DC5D8, 1));
    gDPPipeSync(gDisplayListHead++);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void make_border(void) {

    if (g_ScreenSplitA == SCREEN_MODE_1P) {
        return;
    }
    if (g_ClearCFBFlag != 0) {
        return;
    }

    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(g_CfbPtrs[draw_frame]));
    gDPSetFillColor(gDisplayListHead++, 0x00010001);
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPPipeSync(gDisplayListHead++);

    switch (g_ScreenSplitA) {
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            gDPFillRectangle(gDisplayListHead++, 159, 0, 161, 239);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            gDPFillRectangle(gDisplayListHead++, 0, 119, 319, 121);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            gDPFillRectangle(gDisplayListHead++, 159, 0, 161, 239);
            gDPFillRectangle(gDisplayListHead++, 0, 119, 319, 121);
            break;
    }
    gDPPipeSync(gDisplayListHead++);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}
/**
 * @note that the second half of the s16 value is truncated (unused). So if you want red, put 255. But the original
 * programmers might have put something like `42,239`, in bytes: b1010010011111111 The extra bits are skipped and the
 * game only reads `11111111` (255)
 */
struct Skybox {
    s16 topRed;
    s16 topGreen;
    s16 topBlue;
    s16 bottomRed;
    s16 bottomGreen;
    s16 bottomBlue;
};

UNUSED Gfx D_802B8A90[] = {
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetFillColor(0x00000000),
    gsDPFillRectangle(0, 0, 319, 239),
    gsDPPipeSync(),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsSPEndDisplayList(),
};

struct Skybox g_skyColorTopTable[] = {
#include "assets/course_metadata/sSkyColors.inc.c"

};

// struct Skybox g_skyColorTopTable[] = {
//     {128, 4280, 6136, 216, 7144, 32248},
//     {255, 255, 255, 255, 255, 255},
//     {48, 1544, 49528, 0, 0, 0},
//     {0, 0, 0, 0, 0, 0},
//     {113, 70, 255, 255, 184, 99},
//     {28, 11, 90, 0, 99, 164},
//     {48, 1688, 54136, 216, 7144, 32248},
//     {238, 144, 255, 255, 224, 240},
//     {128, 4280, 6136, 216, 7144, 32248},
//     {0, 18, 255, 197, 211, 255},
//     {0, 2, 94, 209, 65, 23},
//     {195, 231, 255, 255, 0xc0, 0},
//     {128, 4280, 6136, 216, 7144, 32248},
//     {0, 0, 0, 0, 0, 0},
//     {20, 30, 56, 40, 60, 110},
//     {128, 4280, 6136, 216, 7144, 32248},
//     {0, 0, 0, 0, 0, 0},
//     {113, 70, 255, 255, 184, 99},
//     {255, 174, 0, 255, 229, 124},
//     {0, 0, 0, 0, 0, 0},
//     {238, 144, 255, 255, 224, 240},
// };

struct Skybox g_skyColorBotTable[] = {
#include "assets/course_metadata/sSkyColors2.inc.c"
};

void initial_background(Vtx* skybox) {
    s32 i;

    if (g_skyToggle != 0) {

        if (fog_red < 0) {
            fog_red = 0;
        }

        if (fog_blue < 0) {
            fog_blue = 0;
        }

        if (fog_green < 0) {
            fog_green = 0;
        }

        if (fog_red > 255) {
            fog_red = 255;
        }

        if (fog_blue > 255) {
            fog_blue = 255;
        }

        if (fog_green > 255) {
            fog_green = 255;
        }

        for (i = 0; i < 8; i++) {

            skybox[i].v.cn[0] = (s16) fog_red;
            skybox[i].v.cn[1] = (s16) fog_blue;
            skybox[i].v.cn[2] = (s16) fog_green;
        }
        return;
    }

#if !ENABLE_CUSTOM_COURSE_ENGINE
    skybox[0].v.cn[0] = g_skyColorTopTable[g_courseID].topRed;
    skybox[0].v.cn[1] = g_skyColorTopTable[g_courseID].topGreen;
    skybox[0].v.cn[2] = g_skyColorTopTable[g_courseID].topBlue;

    skybox[1].v.cn[0] = g_skyColorTopTable[g_courseID].bottomRed;
    skybox[1].v.cn[1] = g_skyColorTopTable[g_courseID].bottomGreen;
    skybox[1].v.cn[2] = g_skyColorTopTable[g_courseID].bottomBlue;

    skybox[2].v.cn[0] = g_skyColorTopTable[g_courseID].bottomRed;
    skybox[2].v.cn[1] = g_skyColorTopTable[g_courseID].bottomGreen;
    skybox[2].v.cn[2] = g_skyColorTopTable[g_courseID].bottomBlue;

    skybox[3].v.cn[0] = g_skyColorTopTable[g_courseID].topRed;
    skybox[3].v.cn[1] = g_skyColorTopTable[g_courseID].topGreen;
    skybox[3].v.cn[2] = g_skyColorTopTable[g_courseID].topBlue;

    skybox[4].v.cn[0] = g_skyColorBotTable[g_courseID].topRed;
    skybox[4].v.cn[1] = g_skyColorBotTable[g_courseID].topGreen;
    skybox[4].v.cn[2] = g_skyColorBotTable[g_courseID].topBlue;

    skybox[5].v.cn[0] = g_skyColorBotTable[g_courseID].bottomRed;
    skybox[5].v.cn[1] = g_skyColorBotTable[g_courseID].bottomGreen;
    skybox[5].v.cn[2] = g_skyColorBotTable[g_courseID].bottomBlue;

    skybox[6].v.cn[0] = g_skyColorBotTable[g_courseID].bottomRed;
    skybox[6].v.cn[1] = g_skyColorBotTable[g_courseID].bottomGreen;
    skybox[6].v.cn[2] = g_skyColorBotTable[g_courseID].bottomBlue;

    skybox[7].v.cn[0] = g_skyColorBotTable[g_courseID].topRed;
    skybox[7].v.cn[1] = g_skyColorBotTable[g_courseID].topGreen;
    skybox[7].v.cn[2] = g_skyColorBotTable[g_courseID].topBlue;
#else

#endif
}

// Almost identical to end of DisplayBackground
void display_background2(Vtx* arg0, UNUSED Screen* arg1, UNUSED s32 arg2, UNUSED s32 arg3,
                   UNUSED f32* arg4) {

    InitRDP();
    if (g_courseID != COURSE_RAINBOW_ROAD) {

        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
        guOrtho(&gDynamicP->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
        gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxScreen),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(gDisplayListHead++, &arg0[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

/**
 * @brief Sets skybox horizon. Some coordinate transformations which can affect game physics and display of player
 * sprite
 * @param skybox player skybox
 * @param arg1 something camera related
 * @param arg2 unused
 * @param arg3 unused
 * @parma arg4 unused
 */
void DisplayBackground(Vtx* skybox, Screen* arg1, UNUSED s32 arg2, UNUSED s32 arg3, UNUSED f32* arg4) {
    Camera* camera = arg1->camera;
    s16 horizonRow;
    f32 homogFactor;
    UNUSED s32 pad[2];
    UNUSED u16 pad2;
    u16 sp128;
    Mat4 projMtx;
    Mat4 lookAtMtx;
    Mat4 lookAndProjMtx;
    Vec3f horizonPoint;
    f32 homogScale;

    initial_background(skybox);

    // horizonPoint is an apparently arbitrary point on the horizon (technically, where y = 0). Used for skybox horizon
    horizonPoint[0] = 0.0f;
    horizonPoint[1] = 0.0f;
    horizonPoint[2] = 30000.0f;
    MakeGuPerspective(projMtx, &sp128, camera->screen_view_angle, g_aspectRatio, screen_near_point, g_farClip, 1.0f);
    MakeGuLookAt(lookAtMtx, camera->camera_pos, camera->lookat_pos);
    MultiAffineMtx(lookAndProjMtx, projMtx, lookAtMtx);

    /* math would have been simpler if horizonPoint had an additional homogenous coordinate set to 1. Recreated here in
    extra steps */
    homogScale = ((lookAndProjMtx[0][3] * horizonPoint[0]) + (lookAndProjMtx[1][3] * horizonPoint[1]) +
                  (lookAndProjMtx[2][3] * horizonPoint[2])) +
                 lookAndProjMtx[3][3];
    MultipleAffineMtxByVector(horizonPoint, lookAndProjMtx);

    homogFactor = (1.0 / homogScale);

    horizonPoint[0] *= homogFactor;
    horizonPoint[1] *= homogFactor;

    horizonPoint[0] *= 160.0f; // SCREEN_WIDTH / 2
    horizonPoint[1] *= 120.0f; // SCREEN_HEIGHT / 2

    horizonRow = 120 - (s16) horizonPoint[1];
    arg1->horizon = horizonRow;

    skybox[1].v.ob[1] = horizonRow;
    skybox[2].v.ob[1] = horizonRow;
    skybox[4].v.ob[1] = horizonRow;
    skybox[7].v.ob[1] = horizonRow;

    // this section reders the skybox. Unclear if it does anything else
    InitRDP();
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER | G_LIGHTING);
    guOrtho(&gDynamicP->mtxScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, 0xFFFF);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxScreen),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(gDisplayListHead++, &skybox[0], 4, 0);
    gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    if (g_courseID == COURSE_RAINBOW_ROAD) {
        gSPVertex(gDisplayListHead++, &skybox[4], 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void initital_perspective(void) {
    if (gGamestate != 4) {
        g_farClip = 6800.0f;
        screen_near_point = 3.0f;
    } else {
        switch (g_courseID) {
            case COURSE_BOWSER_CASTLE:
            case COURSE_BANSHEE_BOARDWALK:
            case COURSE_RAINBOW_ROAD:
            case COURSE_BLOCK_FORT:
            case COURSE_SKYSCRAPER:
                g_farClip = 2700.0f;
                screen_near_point = 2.0f;
                break;
            case COURSE_CHOCO_MOUNTAIN:
            case COURSE_DOUBLE_DECK:
                g_farClip = 1500.0f;
                screen_near_point = 2.0f;
                break;
            case COURSE_KOOPA_BEACH:
                g_farClip = 5000.0f;
                screen_near_point = 1.0f;
                break;
            case COURSE_WARIO_STADIUM:
                g_farClip = 4800.0f;
                screen_near_point = 10.0f;
                break;
            case COURSE_MARIO_RACEWAY:
            case COURSE_YOSHI_VALLEY:
            case COURSE_FRAPPE_SNOWLAND:
            case COURSE_ROYAL_RACEWAY:
            case COURSE_LUIGI_RACEWAY:
            case COURSE_MOO_MOO_FARM:
            case COURSE_TOADS_TURNPIKE:
            case COURSE_SHERBET_LAND:
            case COURSE_DK_JUNGLE:
                g_farClip = 4500.0f;
                screen_near_point = 9.0f;
                break;
            case COURSE_KALAMARI_DESERT:
                g_farClip = 7000.0f;
                screen_near_point = 10.0f;
                break;
            default:
                g_farClip = 6800.0f;
                screen_near_point = 3.0f;
                break;
        }
    }
    switch (g_ScreenSplitB) { /* switch 1; irregular */
        case SCREEN_MODE_1P:        /* switch 1 */
            g_aspectRatio = 1.33333334f;
            return;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL: /* switch 1 */
            g_aspectRatio = 0.66666667f;
            return;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL: /* switch 1 */
            g_aspectRatio = 2.66666667f;
            return;
        case SCREEN_MODE_3P_4P_SPLITSCREEN: /* switch 1 */
            g_aspectRatio = 1.33333334f;
            return;
    }
}

void setup_perspective(void) {
    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            make_view_ang(gPlayerOne, &gCameraZoom[0]);
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            make_view_ang(gPlayerOne, &gCameraZoom[0]);
            make_view_ang(gPlayerTwo, &gCameraZoom[1]);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            make_view_ang(gPlayerOne, &gCameraZoom[0]);
            make_view_ang(gPlayerTwo, &gCameraZoom[1]);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            make_view_ang(gPlayerOne, &gCameraZoom[0]);
            make_view_ang(gPlayerTwo, &gCameraZoom[1]);
            make_view_ang(gPlayerThree, &gCameraZoom[2]);
            make_view_ang(gPlayerFour, &gCameraZoom[3]);
            break;
    }
}
// player 2 vertical
void initial_draw_right(void) {

    InitRDP();
    SetViewport(D_800DC5F0);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);

    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    ClearZBuffer2(D_800DC5F0);
    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        kwdisplay_kumo(2);
        display_background2((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        kawanoDrawAfterSky(2);
    }
}
// player 1 vertical
void initial_draw_left(void) {

    InitRDP();
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    ClearZBuffer2(D_800DC5EC);
    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        kwdisplay_kumo(1);
        display_background2((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        kawanoDrawAfterSky(1);
    }
}
// player 1 horizontal
void initial_draw_up(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5EC);
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        kwdisplay_kumo(3);
        display_background2((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[0]);
        kawanoDrawAfterSky(3);
    }
}
// player 2 horizontal
void initial_draw_down(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5F0);
    SetViewport(D_800DC5F0);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        kwdisplay_kumo(4);
        display_background2((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        kawanoDrawAfterSky(4);
    }
}
// player 1 solo
void InitialDrawCommon(void) {

    StoreSegments();
    InitRDP();
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    ClearZBuffer();
    g_ClearFramebuffer();
    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        if (gGamestate != CREDITS_SEQUENCE) {
            kwdisplay_kumo(0);
        }
        display_background2((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        kawanoDrawAfterSky(0);
    }
}
// player 1 3p 4p
void initial_draw_up_left(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5EC);
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        kwdisplay_kumo(8);
        display_background2((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &gCameraZoom[0]);
        kawanoDrawAfterSky(8);
    }
}
// player 2 3p 4p
void initial_draw_up_right(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5F0);
    SetViewport(D_800DC5F0);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        kwdisplay_kumo(9);
        display_background2((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[1]);
        kawanoDrawAfterSky(9);
    }
}
// player 3 3p4p
void initial_draw_down_left(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5F4);
    SetViewport(D_800DC5F4);

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (gBackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP3, D_800DC5F4, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[2]);
        kwdisplay_kumo(10);
        display_background2((Vtx*) sSkyboxP3, D_800DC5F4, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[2]);
        kawanoDrawAfterSky(10);
    }
}

// player 4 3p 4p
void initial_draw_down_right(void) {

    InitRDP();

    gSPClearGeometryMode(gDisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (g_playerCount == 3) {

        gDPPipeSync(gDisplayListHead++);
        ClearZBuffer2(D_800DC5F8);
        gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);
        gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                         VIRTUAL_TO_PHYSICAL(g_CfbPtrs[draw_frame]));
        gDPSetFillColor(gDisplayListHead++, 0x00010001);
        gDPPipeSync(gDisplayListHead++);
        gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 160, 120, SCREEN_WIDTH, SCREEN_HEIGHT);
        gDPFillRectangle(gDisplayListHead++, 160, 120, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        gDPPipeSync(gDisplayListHead++);
        gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);

        SetViewport(D_800DC5F8);

    } else {
        SetViewport(D_800DC5F8);
        ClearZBuffer2(D_800DC5F8);

        if (gBackgroundFlag != 0) {
            DisplayBackground(sSkyboxP4, D_800DC5F8, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[3]);
            kwdisplay_kumo(11);
            display_background2(sSkyboxP4, D_800DC5F8, SCREEN_WIDTH, SCREEN_HEIGHT, &gCameraZoom[3]);
            kawanoDrawAfterSky(11);
        }
    }
}

void draw_full_screen(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[4];
    u16 perspNorm;
    UNUSED s32 pad2[2];
#ifdef VERSION_EU
    f32 sp9C;
#endif
    UNUSED s32 pad3;
    Mat4 matrix;

#ifdef VERSION_EU
    sp9C = g_aspectRatio * 1.2f;
#endif
    InitialDrawCommon();
    InitRDP();
    SetViewport(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&gDynamicP->mtxLookAt[0], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);
    if (g_fadeOutCounter2 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5EC);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5EC);
    kwdisplay3D(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    DrawKart();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    DrawKartAfter();
    display_object2(D_800DC5EC);
    kwdisplay3D_after(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
}

void draw_left_screen(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#else
    UNUSED f32 sp9C;
#endif

    initial_draw_left();
#ifdef VERSION_EU
    sp9C = g_aspectRatio * 1.2f;
#endif
    InitRDP();
    SetViewport(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[0], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);

    if (g_fadeOutCounter2 == 0) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5EC);
    if (g_fadeOutCounter2 == 1) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5EC);
    kwdisplay3D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    DrawKart();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    DrawKartAfter();
    display_object2(D_800DC5EC);
    kwdisplay3D_after(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    g_ScreenCounter += 1;
}

void draw_right_screen(void) {
    Camera* camera = &cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#else
    UNUSED f32 sp9C;
#endif

    initial_draw_right();
    InitRDP();
    SetViewport(D_800DC5F0);
#ifdef VERSION_EU
    sp9C = g_aspectRatio * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[1], &perspNorm, gCameraZoom[1], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[1], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);

    if (g_fadeOutCounter2 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5F0);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5F0);
    kwdisplay3D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    DrawKart2P();
    DisplayMap2(D_800DC5F0, PLAYER_TWO);
    DrawKart2PAfter();
    display_object2(D_800DC5F0);
    kwdisplay2D_init();
    kwdisplay3D_after(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    g_ScreenCounter += 1;
}

void draw_up_screen(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    initial_draw_up();
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    InitRDP();
    SetViewport(D_800DC5EC);
#ifdef VERSION_EU
    sp9C = g_aspectRatio * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[0], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);

    if (g_fadeOutCounter2 == 0) {

        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5EC);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5EC);
    kwdisplay3D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    DrawKart();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    DrawKartAfter();
    display_object2(D_800DC5EC);
    kwdisplay3D_after(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    g_ScreenCounter += 1;
}

void draw_down_screen(void) {
    Camera* camera = &cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    initial_draw_down();
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    InitRDP();
    SetViewport(D_800DC5F0);
#ifdef VERSION_EU
    sp9C = g_aspectRatio * 1.2f;
#endif
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[1], &perspNorm, gCameraZoom[1], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[1], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);

    if (g_fadeOutCounter2 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5F0);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5F0);
    kwdisplay3D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    DrawKart2P();
    DisplayMap2(D_800DC5F0, PLAYER_TWO);
    DrawKart2PAfter();
    display_object2(D_800DC5F0);
    kwdisplay3D_after(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    g_ScreenCounter += 1;
}

void draw_up_left_screen(void) {
    Camera* camera = camera1;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = g_aspectRatio * 1.2f;
#endif

    initial_draw_up_left();
    InitRDP();
    SetViewport(D_800DC5EC);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[0], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);

    if (g_fadeOutCounter2 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5EC);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5EC);
    kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    DrawKart();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    DrawKartAfter();
    display_object2(D_800DC5EC);
    kwdisplay3D_after(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    g_ScreenCounter += 1;
}

void draw_up_right_screen(void) {
    Camera* camera = camera2;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = g_aspectRatio * 1.2f;
#endif

    initial_draw_up_right();
    InitRDP();
    SetViewport(D_800DC5F0);
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[1], &perspNorm, gCameraZoom[1], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[1], &perspNorm, gCameraZoom[1], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&gDynamicP->mtxLookAt[1], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);
    if (g_fadeOutCounter2 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5F0);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5F0);
    kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    DrawKart2P();
    DisplayMap2(D_800DC5F0, PLAYER_TWO);
    DrawKart2PAfter();
    display_object2(D_800DC5F0);
    kwdisplay3D_after(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    g_ScreenCounter += 1;
}

void draw_down_left_screen(void) {
    Camera* camera = camera3;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = g_aspectRatio * 1.2f;
#endif

    initial_draw_down_left();
    InitRDP();
    SetViewport(D_800DC5F4);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[2], &perspNorm, gCameraZoom[2], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[2], &perspNorm, gCameraZoom[2], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[2], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);
    if (g_fadeOutCounter2 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5F4);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5F4);
    kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    DrawKart3P();
    DisplayMap2(D_800DC5F4, PLAYER_THREE);
    DrawKart3PAfter();
    display_object2(D_800DC5F4);
    kwdisplay3D_after(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    g_ScreenCounter += 1;
}

void draw_down_right_screen(void) {
    Camera* camera = camera4;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = g_aspectRatio * 1.2f;
#endif

    initial_draw_down_right();
    if (g_playerCount == 3) {
        kawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        if (two_d_flag != 0) {
            KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        }
        g_ScreenCounter += 1;
        return;
    }

    InitRDP();
    SetViewport(D_800DC5F8);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&gDynamicP->mtxPersp[3], &perspNorm, gCameraZoom[3], sp9C, screen_near_point, g_farClip, 1.0f);
#else
    guPerspective(&gDynamicP->mtxPersp[3], &perspNorm, gCameraZoom[3], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
#endif
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[3], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);
    if (g_fadeOutCounter2 == 0) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    display_map(D_800DC5F8);
    if (g_fadeOutCounter2 == 1) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(matrix);
        SetMatrix(matrix, 0);
    }
    display_object(D_800DC5F8);
    kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    DrawKart4P();
    DisplayMap2(D_800DC5F8, PLAYER_FOUR);
    DrawKart4PAfter();
    display_object2(D_800DC5F8);
    kwdisplay3D_after(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    kwdisplay2D_init();
    if (two_d_flag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    kawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    if (two_d_flag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    g_ScreenCounter += 1;
}

void initial_screen_data(void) {
    Screen* wrapper = &D_8015F480[0];
    Player* player = &gPlayers[0];
    Camera* camera = &cameras[0];
    struct Controller* controller = &gControllers[0];

    // struct? size = 0x10. unk++ doesn't work cause s32 too small.
    s32* unk = &D_8015F790[0];
    s32 i;

    for (i = 0; i < 4; i++) {
        wrapper->cont = controller;
        wrapper->camera = camera;
        wrapper->kart = player;
        wrapper->matrix = unk;
        wrapper->width = 4;
        wrapper->height = 4;
        wrapper->camera_point = 1;

        switch (g_ScreenSplitA) {
            case SCREEN_MODE_1P:
                if (i == 0) {
                    wrapper->posx = 160;
                }
                wrapper->posy = 120;
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                if (i == 0) {
                    wrapper->posx = 80;
                    wrapper->posy = 120;
                } else if (i == 1) {
                    wrapper->posx = 240;
                    wrapper->posy = 120;
                }
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                if (i == 0) {
                    wrapper->posx = 160;
                    wrapper->posy = 60;
                } else if (i == 1) {
                    wrapper->posx = 160;
                    wrapper->posy = 180;
                }
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (i == 0) {
                    wrapper->posx = 80;
                    wrapper->posy = 60;
                } else if (i == 1) {
                    wrapper->posx = 240;
                    wrapper->posy = 60;
                } else if (i == 2) {
                    wrapper->posx = 80;
                    wrapper->posy = 180;
                } else {
                    wrapper->posx = 240;
                    wrapper->posy = 180;
                }
                break;
        }
        player++;
        camera++;
        wrapper++;
        unk += 0x10;
    }
}

void GetFramebuffer(s32 arg0, s32 arg1, s32 width, s32 height, u16* source, u16* target) {
    s32 var_v1;
    s32 var_a1;
    s32 targetIndex;
    s32 sourceIndex;

    targetIndex = 0;
    for (var_v1 = 0; var_v1 < height; var_v1++) {
        sourceIndex = ((arg1 + var_v1) * 320) + arg0;
        for (var_a1 = 0; var_a1 < width; var_a1++, targetIndex++, sourceIndex++) {
            target[targetIndex] = source[sourceIndex];
        }
    }
}

void take_snapshot15(void) {
    s16 temp_v0;

    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
        D_800DC5DC = 0;
    } else {
        D_800DC5DC = 128;
    }
    D_800DC5E0 = 0;
    temp_v0 = (s16) g_DispFrame - 1;
    if (temp_v0 < 0) {
        temp_v0 = 2;
    } else if (temp_v0 > 2) {
        temp_v0 = 0;
    }
    GetFramebuffer(D_800DC5DC, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x8800));
    GetFramebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x9800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xA800));
    GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xB800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xC800));
    GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xD800));
}

void take_snapshot9(void) {
    s16 temp_v0;

    if (g_ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
        D_800DC5DC = 0;
    } else {
        D_800DC5DC = 128;
    }
    D_800DC5E0 = 0;
    temp_v0 = (s16) g_DispFrame - 1;
    if (temp_v0 < 0) {
        temp_v0 = 2;
    } else if (temp_v0 > 2) {
        temp_v0 = 0;
    }
    GetFramebuffer(D_800DC5DC, D_800DC5E0, 0x40, 0x20, (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xF800));
    GetFramebuffer(D_800DC5DC + 0x40, D_800DC5E0, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x10800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 0x20, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x11800));
    GetFramebuffer(D_800DC5DC + 0x40, D_800DC5E0 + 0x20, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x12800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 0x40, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x13800));
    GetFramebuffer(D_800DC5DC + 0x40, D_800DC5E0 + 0x40, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x14800));
}
