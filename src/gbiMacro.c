#include <ultra64.h>
#include <macros.h>
#include <PR/gu.h>
#include "main.h"
#include <mk64.h>

extern s16 D_800E43A8;
extern Mtx D_0D008E98;

// rsp init
UNUSED void InitOrtho(void) {
    D_800E43A8 = 0;

    gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    guOrtho(&DynamicP->MTXScreen, 0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f, 1.0f);
    gSPPerspNormalize(DisplayListHead++, 0xFFFF);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(DynamicP), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_0D008E98), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}
