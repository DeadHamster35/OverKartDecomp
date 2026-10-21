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
#include "OverKart5Hooks.h"

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
    s32 screenWidth = arg0->Width * 2;
    s32 screenHeight = arg0->Height * 2;
    s32 screenStartX = arg0->Posx * 4;
    s32 screenStartY = arg0->Posy * 4;

    arg0->IsInViewport.vp.vscale[0] = screenWidth;
    arg0->IsInViewport.vp.vscale[1] = screenHeight;
    arg0->IsInViewport.vp.vscale[2] = 511;
    arg0->IsInViewport.vp.vscale[3] = 0;

    arg0->IsInViewport.vp.vtrans[0] = screenStartX;
    arg0->IsInViewport.vp.vtrans[1] = screenStartY;
    arg0->IsInViewport.vp.vtrans[2] = 511;
    arg0->IsInViewport.vp.vtrans[3] = 0;

    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&arg0->IsInViewport));

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

    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
}

UNUSED void func_802A38AC(void) {
}

void FadeoutSequence(void) {
    InitRDP();
    ClearFramebuffer();

    gDPFullSync(DisplayListHead++);
    gSPEndDisplayList(DisplayListHead++);

    if (FadeOutCounter != 0) {
        FadeOutCounter--;
        return;
    }
    NewSequenceMode = NextSequenceMode;
    GameState = 255;
    FadeOutFlag = 0;
    FadeOutCounter = 0;
    KBGChange = FADE_MODE_MAIN;

    switch (NextSequenceMode) {
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
    s32 ulx = arg0->Posx - (arg0->Width / 2);
    s32 uly = arg0->Posy - (arg0->Height / 2);
    s32 lrx = arg0->Posx + (arg0->Width / 2);
    s32 lry = arg0->Posy + (arg0->Height / 2);

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

    gDPPipeSync(DisplayListHead++);
    gDPSetCycleType(DisplayListHead++, G_CYC_FILL);
    gDPSetDepthImage(DisplayListHead++, PhysicalZBuffer);
    gDPSetColorImage(DisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, PhysicalZBuffer);
    gDPSetFillColor(DisplayListHead++, 0xFFFCFFFC);
    gDPPipeSync(DisplayListHead++);
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);

    gDPFillRectangle(DisplayListHead++, ulx, uly, lrx - 1, lry - 1);

    gDPPipeSync(DisplayListHead++);
    gDPSetColorImage(DisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(CfbPtrs[DrawFrame])); // 0x1FFFFFFF
    gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);
    gDPSetDepthSource(DisplayListHead++, G_ZS_PIXEL);
}

/**
 * Initialize the z-Buffer for the Current frame.
 */
void ClearZBuffer(void) {
    gDPPipeSync(DisplayListHead++);
    gDPSetCycleType(DisplayListHead++, G_CYC_FILL);
    gDPSetDepthImage(DisplayListHead++, PhysicalZBuffer);
    gDPSetColorImage(DisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, PhysicalZBuffer);
    gDPSetFillColor(DisplayListHead++, 0xFFFCFFFC);
    gDPPipeSync(DisplayListHead++);
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPFillRectangle(DisplayListHead++, 0, 0, 319, 239);
    gDPPipeSync(DisplayListHead++);
    gDPSetColorImage(DisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(CfbPtrs[DrawFrame]));
    gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);
    gDPSetDepthSource(DisplayListHead++, G_ZS_PIXEL);
}

/**
 * Sets the initial RDP (Reality Display Processor) rendering settings.
 **/
void InitRDP(void) {
    gDPPipeSync(DisplayListHead++);
    gDPPipelineMode(DisplayListHead++, G_PM_1PRIMITIVE);
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
    gDPSetTextureLOD(DisplayListHead++, G_TL_TILE);
    gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
    gDPSetTextureDetail(DisplayListHead++, G_TD_CLAMP);
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gDPSetTextureFilter(DisplayListHead++, G_TF_BILERP);
    gDPSetTextureConvert(DisplayListHead++, G_TC_FILT);
    gDPSetCombineKey(DisplayListHead++, G_CK_NONE);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
    gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetBlendMask(DisplayListHead++, 0xFF);
    gDPSetColorDither(DisplayListHead++, G_CD_DISABLE);
    gDPPipeSync(DisplayListHead++);
    gSPClipRatio(DisplayListHead++, FRUSTRATIO_1);
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

UNUSED s32 InitRSPFull(void) {
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_800DC5EC->IsInViewport));
    gSPClearGeometryMode(DisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(DisplayListHead++,
                       G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH | G_CLIPPING);
}

void InitRSP(void) {
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_802B8880));
    gSPClearGeometryMode(DisplayListHead++, G_CLEAR_ALL_MODES);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
}

/**
 * Tells the RDP which of the three framebuffers it shall draw to.
 */
void ClearFramebuffer(void) {
    gDPSetColorImage(DisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(CfbPtrs[DrawFrame]));
    gDPSetFillColor(DisplayListHead++, GPACK_RGBA5551(D_800DC5D0, D_800DC5D4, D_800DC5D8, 1) << 0x10 |
                                            GPACK_RGBA5551(D_800DC5D0, D_800DC5D4, D_800DC5D8, 1));
    gDPPipeSync(DisplayListHead++);
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPFillRectangle(DisplayListHead++, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    gDPPipeSync(DisplayListHead++);
    gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);
}

void MakeBorder(void) {

    if (ScreenSplitA == SCREEN_MODE_1P) {
        return;
    }
    if (ClearCFBFlag != 0) {
        return;
    }

    gDPPipeSync(DisplayListHead++);
    gDPSetCycleType(DisplayListHead++, G_CYC_FILL);
    gDPSetColorImage(DisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     VIRTUAL_TO_PHYSICAL(CfbPtrs[DrawFrame]));
    gDPSetFillColor(DisplayListHead++, 0x00010001);
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_802B8880));
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPPipeSync(DisplayListHead++);

    switch (ScreenSplitA) {
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            gDPFillRectangle(DisplayListHead++, 159, 0, 161, 239);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            gDPFillRectangle(DisplayListHead++, 0, 119, 319, 121);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            gDPFillRectangle(DisplayListHead++, 159, 0, 161, 239);
            gDPFillRectangle(DisplayListHead++, 0, 119, 319, 121);
            break;
    }
    gDPPipeSync(DisplayListHead++);
    gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);
}
/**
 * @Note that the second half of the s16 Value is truncated (Unused). So if you want red, put 255. But the original
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

struct Skybox SkyColorTopTable[] = {
#include "assets/course_metadata/sSkyColors.inc.c"

};

// struct Skybox SkyColorTopTable[] = {
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

struct Skybox SkyColorBotTable[] = {
#include "assets/course_metadata/sSkyColors2.inc.c"
};

void InitialBackground(Vtx* skybox) {
    s32 i;

    if (SkyToggle != 0) {

        if (FogRed < 0) {
            FogRed = 0;
        }

        if (FogBlue < 0) {
            FogBlue = 0;
        }

        if (FogGreen < 0) {
            FogGreen = 0;
        }

        if (FogRed > 255) {
            FogRed = 255;
        }

        if (FogBlue > 255) {
            FogBlue = 255;
        }

        if (FogGreen > 255) {
            FogGreen = 255;
        }

        for (i = 0; i < 8; i++) {

            skybox[i].v.cn[0] = (s16) FogRed;
            skybox[i].v.cn[1] = (s16) FogBlue;
            skybox[i].v.cn[2] = (s16) FogGreen;
        }
        return;
    }

#if !ENABLE_CUSTOM_COURSE_ENGINE
    skybox[0].v.cn[0] = SkyColorTopTable[CourseID].topRed;
    skybox[0].v.cn[1] = SkyColorTopTable[CourseID].topGreen;
    skybox[0].v.cn[2] = SkyColorTopTable[CourseID].topBlue;

    skybox[1].v.cn[0] = SkyColorTopTable[CourseID].bottomRed;
    skybox[1].v.cn[1] = SkyColorTopTable[CourseID].bottomGreen;
    skybox[1].v.cn[2] = SkyColorTopTable[CourseID].bottomBlue;

    skybox[2].v.cn[0] = SkyColorTopTable[CourseID].bottomRed;
    skybox[2].v.cn[1] = SkyColorTopTable[CourseID].bottomGreen;
    skybox[2].v.cn[2] = SkyColorTopTable[CourseID].bottomBlue;

    skybox[3].v.cn[0] = SkyColorTopTable[CourseID].topRed;
    skybox[3].v.cn[1] = SkyColorTopTable[CourseID].topGreen;
    skybox[3].v.cn[2] = SkyColorTopTable[CourseID].topBlue;

    skybox[4].v.cn[0] = SkyColorBotTable[CourseID].topRed;
    skybox[4].v.cn[1] = SkyColorBotTable[CourseID].topGreen;
    skybox[4].v.cn[2] = SkyColorBotTable[CourseID].topBlue;

    skybox[5].v.cn[0] = SkyColorBotTable[CourseID].bottomRed;
    skybox[5].v.cn[1] = SkyColorBotTable[CourseID].bottomGreen;
    skybox[5].v.cn[2] = SkyColorBotTable[CourseID].bottomBlue;

    skybox[6].v.cn[0] = SkyColorBotTable[CourseID].bottomRed;
    skybox[6].v.cn[1] = SkyColorBotTable[CourseID].bottomGreen;
    skybox[6].v.cn[2] = SkyColorBotTable[CourseID].bottomBlue;

    skybox[7].v.cn[0] = SkyColorBotTable[CourseID].topRed;
    skybox[7].v.cn[1] = SkyColorBotTable[CourseID].topGreen;
    skybox[7].v.cn[2] = SkyColorBotTable[CourseID].topBlue;
#else

#endif
}

// Almost identical to end of DisplayBackground
void DisplayBackground2(Vtx* arg0, UNUSED Screen* arg1, UNUSED s32 Arg2, UNUSED s32 Arg3,
                   UNUSED f32* arg4) {

    InitRDP();
    if (CourseID != COURSE_RAINBOW_ROAD) {

        gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gSPClearGeometryMode(DisplayListHead++, G_ZBUFFER | G_LIGHTING);
        guOrtho(&DynamicP->MTXScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
        gSPPerspNormalize(DisplayListHead++, 0xFFFF);
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXScreen),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPVertex(DisplayListHead++, &arg0[4], 4, 0);
        gSP2Triangles(DisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

/**
 * @brief Sets skybox Horizon. Some coordinate transformations which can affect game physics and display of Player
 * sprite
 * @Param skybox Player skybox
 * @Param arg1 something Camera related
 * @Param Arg2 Unused
 * @Param Arg3 Unused
 * @parma arg4 Unused
 */
void DisplayBackground(Vtx* skybox, Screen* arg1, UNUSED s32 Arg2, UNUSED s32 Arg3, UNUSED f32* arg4) {
    Camera* Camera = arg1->Camera;
    s16 horizonRow;
    f32 homogFactor;
    UNUSED s32 pad[2];
    UNUSED u16 Pad2;
    u16 sp128;
    Mat4 projMtx;
    Mat4 lookAtMtx;
    Mat4 lookAndProjMtx;
    Vec3f horizonPoint;
    f32 homogScale;

    InitialBackground(skybox);

    // horizonPoint is an apparently arbitrary Point on the Horizon (technically, where y = 0). Used for skybox Horizon
    horizonPoint[0] = 0.0f;
    horizonPoint[1] = 0.0f;
    horizonPoint[2] = 30000.0f;
    MakeGuPerspective(projMtx, &sp128, Camera->ScreenViewAngle, AspectRatio, ScreenNearPoint, FarClip, 1.0f);
    MakeGuLookAt(lookAtMtx, Camera->CameraPos, Camera->LookatPos);
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
    arg1->Horizon = horizonRow;

    skybox[1].v.ob[1] = horizonRow;
    skybox[2].v.ob[1] = horizonRow;
    skybox[4].v.ob[1] = horizonRow;
    skybox[7].v.ob[1] = horizonRow;

    // this section reders the skybox. Unclear if it does anything else
    InitRDP();
    gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gSPClearGeometryMode(DisplayListHead++, G_ZBUFFER | G_LIGHTING);
    guOrtho(&DynamicP->MTXScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, 0.0f, 5.0f, 1.0f);
    gSPPerspNormalize(DisplayListHead++, 0xFFFF);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXScreen),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPVertex(DisplayListHead++, &skybox[0], 4, 0);
    gSP2Triangles(DisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    if (CourseID == COURSE_RAINBOW_ROAD) {
        gSPVertex(DisplayListHead++, &skybox[4], 4, 0);
        gSP2Triangles(DisplayListHead++, 0, 3, 1, 0, 1, 3, 2, 0);
    }
}

void InititalPerspective(void) {
    if (GameState != 4) {
        FarClip = 6800.0f;
        ScreenNearPoint = 3.0f;
    } else {
        switch (CourseID) {
            case COURSE_BOWSER_CASTLE:
            case COURSE_BANSHEE_BOARDWALK:
            case COURSE_RAINBOW_ROAD:
            case COURSE_BLOCK_FORT:
            case COURSE_SKYSCRAPER:
                FarClip = 2700.0f;
                ScreenNearPoint = 2.0f;
                break;
            case COURSE_CHOCO_MOUNTAIN:
            case COURSE_DOUBLE_DECK:
                FarClip = 1500.0f;
                ScreenNearPoint = 2.0f;
                break;
            case COURSE_KOOPA_BEACH:
                FarClip = 5000.0f;
                ScreenNearPoint = 1.0f;
                break;
            case COURSE_WARIO_STADIUM:
                FarClip = 4800.0f;
                ScreenNearPoint = 10.0f;
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
                FarClip = 4500.0f;
                ScreenNearPoint = 9.0f;
                break;
            case COURSE_KALAMARI_DESERT:
                FarClip = 7000.0f;
                ScreenNearPoint = 10.0f;
                break;
            default:
                FarClip = 6800.0f;
                ScreenNearPoint = 3.0f;
                break;
        }
    }
    switch (ScreenSplitB) { /* switch 1; irregular */
        case SCREEN_MODE_1P:        /* switch 1 */
            AspectRatio = 1.33333334f;
            return;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL: /* switch 1 */
            AspectRatio = 0.66666667f;
            return;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL: /* switch 1 */
            AspectRatio = 2.66666667f;
            return;
        case SCREEN_MODE_3P_4P_SPLITSCREEN: /* switch 1 */
            AspectRatio = 1.33333334f;
            return;
    }
}

void SetupPerspective(void) {
    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            MakeViewAng(PlayerOne, &CameraZoom[0]);
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            MakeViewAng(PlayerOne, &CameraZoom[0]);
            MakeViewAng(PlayerTwo, &CameraZoom[1]);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            MakeViewAng(PlayerOne, &CameraZoom[0]);
            MakeViewAng(PlayerTwo, &CameraZoom[1]);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            MakeViewAng(PlayerOne, &CameraZoom[0]);
            MakeViewAng(PlayerTwo, &CameraZoom[1]);
            MakeViewAng(PlayerThree, &CameraZoom[2]);
            MakeViewAng(PlayerFour, &CameraZoom[3]);
            break;
    }
}
// Player 2 vertical
void InitialDrawRight(void) {

    InitRDP();
    SetViewport(D_800DC5F0);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);

    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    ClearZBuffer2(D_800DC5F0);
    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[1]);
        KwdisplayKumo(2);
        DisplayBackground2((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[1]);
        KawanoDrawAfterSky(2);
    }
}
// Player 1 vertical
void InitialDrawLeft(void) {

    InitRDP();
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    ClearZBuffer2(D_800DC5EC);
    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[0]);
        KwdisplayKumo(1);
        DisplayBackground2((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[0]);
        KawanoDrawAfterSky(1);
    }
}
// Player 1 horizontal
void InitialDrawUp(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5EC);
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[0]);
        KwdisplayKumo(3);
        DisplayBackground2((Vtx*) sSkyboxP1, D_800DC5EC, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[0]);
        KawanoDrawAfterSky(3);
    }
}
// Player 2 horizontal
void InitialDrawDown(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5F0);
    SetViewport(D_800DC5F0);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[1]);
        KwdisplayKumo(4);
        DisplayBackground2((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[1]);
        KawanoDrawAfterSky(4);
    }
}
// Player 1 solo
void InitialDrawCommon(void) {

    StoreSegments();
    InitRDP();
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    ClearZBuffer();
    ClearFramebuffer();
    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &CameraZoom[0]);
        if (GameState != CREDITS_SEQUENCE) {
            KwdisplayKumo(0);
        }
        DisplayBackground2((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &CameraZoom[0]);
        KawanoDrawAfterSky(0);
    }
}
// Player 1 3p 4p
void InitialDrawUpLeft(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5EC);
    SetViewport(D_800DC5EC);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &CameraZoom[0]);
        KwdisplayKumo(8);
        DisplayBackground2((Vtx*) sSkyboxP1, D_800DC5EC, 0x140, 0xF0, &CameraZoom[0]);
        KawanoDrawAfterSky(8);
    }
}
// Player 2 3p 4p
void InitialDrawUpRight(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5F0);
    SetViewport(D_800DC5F0);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[1]);
        KwdisplayKumo(9);
        DisplayBackground2((Vtx*) sSkyboxP2, D_800DC5F0, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[1]);
        KawanoDrawAfterSky(9);
    }
}
// Player 3 3p4p
void InitialDrawDownLeft(void) {

    InitRDP();
    ClearZBuffer2(D_800DC5F4);
    SetViewport(D_800DC5F4);

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (BackgroundFlag != 0) {
        DisplayBackground((Vtx*) sSkyboxP3, D_800DC5F4, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[2]);
        KwdisplayKumo(10);
        DisplayBackground2((Vtx*) sSkyboxP3, D_800DC5F4, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[2]);
        KawanoDrawAfterSky(10);
    }
}

// Player 4 3p 4p
void InitialDrawDownRight(void) {

    InitRDP();

    gSPClearGeometryMode(DisplayListHead++, 0xFFFFFFFF);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH | G_CLIPPING);

    if (PlayerCount == 3) {

        gDPPipeSync(DisplayListHead++);
        ClearZBuffer2(D_800DC5F8);
        gDPSetCycleType(DisplayListHead++, G_CYC_FILL);
        gDPSetColorImage(DisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                         VIRTUAL_TO_PHYSICAL(CfbPtrs[DrawFrame]));
        gDPSetFillColor(DisplayListHead++, 0x00010001);
        gDPPipeSync(DisplayListHead++);
        gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 160, 120, SCREEN_WIDTH, SCREEN_HEIGHT);
        gDPFillRectangle(DisplayListHead++, 160, 120, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
        gDPPipeSync(DisplayListHead++);
        gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);

        SetViewport(D_800DC5F8);

    } else {
        SetViewport(D_800DC5F8);
        ClearZBuffer2(D_800DC5F8);

        if (BackgroundFlag != 0) {
            DisplayBackground(sSkyboxP4, D_800DC5F8, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[3]);
            KwdisplayKumo(11);
            DisplayBackground2(sSkyboxP4, D_800DC5F8, SCREEN_WIDTH, SCREEN_HEIGHT, &CameraZoom[3]);
            KawanoDrawAfterSky(11);
        }
    }
}

void DrawFullScreen(void) {
    Camera* Camera = &Cameras[0];
    UNUSED s32 pad[4];
    u16 perspNorm;
    UNUSED s32 Pad2[2];
#ifdef VERSION_EU
    f32 sp9C;
#endif
    UNUSED s32 Pad3;
    Mat4 Matrix;

#ifdef VERSION_EU
    sp9C = AspectRatio * 1.2f;
#endif
    InitialDrawCommon();
    InitRDP();
    SetViewport(D_800DC5EC);
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&DynamicP->MTXLookAt[0], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
    if (FadeOutCounter2 == 0) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5EC);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5EC);
    Kwdisplay3D(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    KartCheckFuncA();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    KartCheckFuncA2();
    DisplayObject2(D_800DC5EC);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    if ((TwoDFlag != 0) && (FlyCamToggle == 0)) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_1P_PLAYER_ONE);
    }
}

void DrawLeftScreen(void) {
    Camera* Camera = &Cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
#else
    UNUSED f32 sp9C;
#endif

    InitialDrawLeft();
#ifdef VERSION_EU
    sp9C = AspectRatio * 1.2f;
#endif
    InitRDP();
    SetViewport(D_800DC5EC);
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[0], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);

    if (FadeOutCounter2 == 0) {

        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5EC);
    if (FadeOutCounter2 == 1) {

        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5EC);
    Kwdisplay3D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    KartCheckFuncA();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    KartCheckFuncA2();
    DisplayObject2(D_800DC5EC);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE);
    }
    ScreenCounter += 1;
}

void DrawRightScreen(void) {
    Camera* Camera = &Cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
#else
    UNUSED f32 sp9C;
#endif

    InitialDrawRight();
    InitRDP();
    SetViewport(D_800DC5F0);
#ifdef VERSION_EU
    sp9C = AspectRatio * 1.2f;
#endif
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[1], &perspNorm, CameraZoom[1], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[1], &perspNorm, CameraZoom[1], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[1], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);

    if (FadeOutCounter2 == 0) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5F0);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5F0);
    Kwdisplay3D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    KartCheckFuncB();
    DisplayMap2(D_800DC5F0, PLAYER_TWO);
    KartCheckFuncB2();
    DisplayObject2(D_800DC5F0);
    Kwdisplay2DInit();
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO);
    }
    ScreenCounter += 1;
}

void DrawUpScreen(void) {
    Camera* Camera = &Cameras[0];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    InitialDrawUp();
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    InitRDP();
    SetViewport(D_800DC5EC);
#ifdef VERSION_EU
    sp9C = AspectRatio * 1.2f;
#endif
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[0], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);

    if (FadeOutCounter2 == 0) {

        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5EC);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5EC);
    Kwdisplay3D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    KartCheckFuncA();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    KartCheckFuncA2();
    DisplayObject2(D_800DC5EC);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE);
    }
    ScreenCounter += 1;
}

void DrawDownScreen(void) {
    Camera* Camera = &Cameras[1];
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
#endif

    InitialDrawDown();
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH);
    InitRDP();
    SetViewport(D_800DC5F0);
#ifdef VERSION_EU
    sp9C = AspectRatio * 1.2f;
#endif
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[1], &perspNorm, CameraZoom[1], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[1], &perspNorm, CameraZoom[1], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[1], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);

    if (FadeOutCounter2 == 0) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5F0);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5F0);
    Kwdisplay3D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    KartCheckFuncB();
    DisplayMap2(D_800DC5F0, PLAYER_TWO);
    KartCheckFuncB2();
    DisplayObject2(D_800DC5F0);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO);
    }
    ScreenCounter += 1;
}

void DrawUpLeftScreen(void) {
    Camera* Camera = Camera1;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = AspectRatio * 1.2f;
#endif

    InitialDrawUpLeft();
    InitRDP();
    SetViewport(D_800DC5EC);
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[0], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);

    if (FadeOutCounter2 == 0) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5EC);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5EC);
    Kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    KartCheckFuncA();
    DisplayMap2(D_800DC5EC, PLAYER_ONE);
    KartCheckFuncA2();
    DisplayObject2(D_800DC5EC);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE);
    }
    ScreenCounter += 1;
}

void DrawUpRightScreen(void) {
    Camera* Camera = Camera2;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = AspectRatio * 1.2f;
#endif

    InitialDrawUpRight();
    InitRDP();
    SetViewport(D_800DC5F0);
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[1], &perspNorm, CameraZoom[1], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[1], &perspNorm, CameraZoom[1], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[1]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(&DynamicP->MTXLookAt[1], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
    if (FadeOutCounter2 == 0) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5F0);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[1]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5F0);
    Kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    KartCheckFuncB();
    DisplayMap2(D_800DC5F0, PLAYER_TWO);
    KartCheckFuncB2();
    DisplayObject2(D_800DC5F0);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO);
    }
    ScreenCounter += 1;
}

void DrawDownLeftScreen(void) {
    Camera* Camera = Camera3;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = AspectRatio * 1.2f;
#endif

    InitialDrawDownLeft();
    InitRDP();
    SetViewport(D_800DC5F4);

    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[2], &perspNorm, CameraZoom[2], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[2], &perspNorm, CameraZoom[2], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[2]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[2], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
    if (FadeOutCounter2 == 0) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5F4);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[2]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5F4);
    Kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    KartCheckFuncC();
    DisplayMap2(D_800DC5F4, PLAYER_THREE);
    KartCheckFuncC2();
    DisplayObject2(D_800DC5F4);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE);
    }
    ScreenCounter += 1;
}

void DrawDownRightScreen(void) {
    Camera* Camera = Camera4;
    UNUSED s32 pad[2];
    u16 perspNorm;
    Mat4 Matrix;
#ifdef VERSION_EU
    f32 sp9C;
    sp9C = AspectRatio * 1.2f;
#endif

    InitialDrawDownRight();
    if (PlayerCount == 3) {
        KawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        if (TwoDFlag != 0) {
            KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
        }
        ScreenCounter += 1;
        return;
    }

    InitRDP();
    SetViewport(D_800DC5F8);

    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
#ifdef VERSION_EU
    guPerspective(&DynamicP->MTXPersp[3], &perspNorm, CameraZoom[3], sp9C, ScreenNearPoint, FarClip, 1.0f);
#else
    guPerspective(&DynamicP->MTXPersp[3], &perspNorm, CameraZoom[3], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
#endif
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[3]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[3], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
    if (FadeOutCounter2 == 0) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    }
    DisplayMap(D_800DC5F8);
    if (FadeOutCounter2 == 1) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[3]),
                  G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        LoadIdentAffineMtx(Matrix);
        SetMatrix(Matrix, 0);
    }
    DisplayCourseObject(D_800DC5F8);
    Kwdisplay3D(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    KartCheckFuncD();
    DisplayMap2(D_800DC5F8, PLAYER_FOUR);
    KartCheckFuncD2();
    DisplayObject2(D_800DC5F8);
    Kwdisplay3DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    Kwdisplay2DInit();
    if (TwoDFlag != 0) {
        KWDisplay2D(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    KawanoDraw2(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    if (TwoDFlag != 0) {
        KWDisplay2DAfter(RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR);
    }
    ScreenCounter += 1;
}

void InitialScreenData(void) {
    Screen* wrapper = &D_8015F480[0];
    Player* Player = &GamePlayers[0];
    Camera* Camera = &Cameras[0];
    struct Controller* controller = &Controllers[0];

    // struct? Size = 0x10. unk++ doesn't work cause s32 too small.
    s32* unk = &D_8015F790[0];
    s32 i;

    for (i = 0; i < 4; i++) {
        wrapper->Cont = controller;
        wrapper->Camera = Camera;
        wrapper->Kart = Player;
        wrapper->Matrix = unk;
        wrapper->Width = 4;
        wrapper->Height = 4;
        wrapper->CameraPoint = 1;

        switch (ScreenSplitA) {
            case SCREEN_MODE_1P:
                if (i == 0) {
                    wrapper->Posx = 160;
                }
                wrapper->Posy = 120;
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                if (i == 0) {
                    wrapper->Posx = 80;
                    wrapper->Posy = 120;
                } else if (i == 1) {
                    wrapper->Posx = 240;
                    wrapper->Posy = 120;
                }
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                if (i == 0) {
                    wrapper->Posx = 160;
                    wrapper->Posy = 60;
                } else if (i == 1) {
                    wrapper->Posx = 160;
                    wrapper->Posy = 180;
                }
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (i == 0) {
                    wrapper->Posx = 80;
                    wrapper->Posy = 60;
                } else if (i == 1) {
                    wrapper->Posx = 240;
                    wrapper->Posy = 60;
                } else if (i == 2) {
                    wrapper->Posx = 80;
                    wrapper->Posy = 180;
                } else {
                    wrapper->Posx = 240;
                    wrapper->Posy = 180;
                }
                break;
        }
        Player++;
        Camera++;
        wrapper++;
        unk += 0x10;
    }
}

void GetFramebuffer(s32 arg0, s32 arg1, s32 Width, s32 Height, u16* Source, u16* Target) {
    s32 var_v1;
    s32 var_a1;
    s32 targetIndex;
    s32 sourceIndex;

    targetIndex = 0;
    for (var_v1 = 0; var_v1 < Height; var_v1++) {
        sourceIndex = ((arg1 + var_v1) * 320) + arg0;
        for (var_a1 = 0; var_a1 < Width; var_a1++, targetIndex++, sourceIndex++) {
            Target[targetIndex] = Source[sourceIndex];
        }
    }
}

void TakeSnapshot15(void) {
    s16 temp_v0;

    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
        D_800DC5DC = 0;
    } else {
        D_800DC5DC = 128;
    }
    D_800DC5E0 = 0;
    temp_v0 = (s16) DispFrame - 1;
    if (temp_v0 < 0) {
        temp_v0 = 2;
    } else if (temp_v0 > 2) {
        temp_v0 = 0;
    }
    GetFramebuffer(D_800DC5DC, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x8800));
    GetFramebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x9800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xA800));
    GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xB800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32, (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xC800));
    GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                     (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xD800));
}

void TakeSnapshot9(void) {
    s16 temp_v0;

    if (ScreenSplitA == SCREEN_MODE_3P_4P_SPLITSCREEN) {
        D_800DC5DC = 0;
    } else {
        D_800DC5DC = 128;
    }
    D_800DC5E0 = 0;
    temp_v0 = (s16) DispFrame - 1;
    if (temp_v0 < 0) {
        temp_v0 = 2;
    } else if (temp_v0 > 2) {
        temp_v0 = 0;
    }
    GetFramebuffer(D_800DC5DC, D_800DC5E0, 0x40, 0x20, (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xF800));
    GetFramebuffer(D_800DC5DC + 0x40, D_800DC5E0, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x10800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 0x20, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x11800));
    GetFramebuffer(D_800DC5DC + 0x40, D_800DC5E0 + 0x20, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x12800));
    GetFramebuffer(D_800DC5DC, D_800DC5E0 + 0x40, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x13800));
    GetFramebuffer(D_800DC5DC + 0x40, D_800DC5E0 + 0x40, 0x40, 0x20,
                     (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[temp_v0]),
                     (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x14800));
}
