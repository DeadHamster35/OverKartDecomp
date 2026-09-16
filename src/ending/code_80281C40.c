#include <ultra64.h>
#include <PR/gu.h>
#include <macros.h>
#include <course.h>

#include "code_80281C40.h"
#include "main.h"
#include "math_util.h"
#include "skybox_and_splitscreen.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "menu_items.h"
#include "podium_ceremony_actors.h"
#include "ceremony_and_credits.h"
#include "podium_ceremony_actors.h"
#include "render_player.h"
#include "defines.h"

struct UnkStruct80287560 {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
}; // 0xC

/** bss **/
s32 gGotoMenu;
s32 D_80287554;
s32 D_80281C40_pad[2];
struct UnkStruct80287560 D_80287560[30];

void func_80281C40(void) {
    s32 i;

    for (i = 0; i < D_802874FC; i++) {
        kwfprintd(D_80287560[i].unk0, D_80287560[i].unk2, (char*) D_80287560[i].unk8, D_80287560[i].unk4);
    }
}

void func_80281CB4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (D_802874FC < 0x1E) {
        D_80287560[D_802874FC].unk0 = arg0;
        D_80287560[D_802874FC].unk2 = arg1;
        D_80287560[D_802874FC].unk4 = arg3;
        D_80287560[D_802874FC].unk8 = arg2;
        D_802874FC++;
    }
}

extern Gfx D_80284F70[];
extern Gfx D_80284EE0[];

void draw_result_screen(void) {
    Camera* camera = &cameras[0];
    UNUSED s32 pad[3];
    u16 perspNorm;
    Mat4 matrix;
    UNUSED s32 pad2[3];

    InitialDrawCommon();
    InitRDP();
    if (gGotoMenu != 0xFFFF) {
        SoftwareBlanking(0);
        if (D_80287554 >= 4) {
            g_fadeOutFlag = 0;
            g_NewSequenceMode = gGotoMenu;
        }
        D_80287554++;
        return;
    }
    func_8028150C();
    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    guPerspective((Mtx*) &gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], g_aspectRatio, screen_near_point,
                  g_farClip, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt((Mtx*) &gDynamicP->mtxLookAt[0], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    LoadIdentAffineMtx(matrix);
    SetMatrix(matrix, 0);
    gSPDisplayList(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284F70));
    DrawKart();
    gSPDisplayList(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284EE0));
    update_actors_loop();
    kwdisplay3D(PLAYER_ONE + SCREEN_MODE_1P);
    DrawKartAfter();
    gSPDisplayList(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284EE0));
    kawanoDrawResult();
    ceremony_transition_sliding_borders();
    func_80281C40();
    InitRDP();
}
