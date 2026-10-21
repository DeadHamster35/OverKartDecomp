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
#include "OverKart5Hooks.h"

struct UnkStruct80287560 {
    s16 Unk0;
    s16 Unk2;
    s32 unk4;
    s32 Unk8;
}; // 0xC

/** bss **/
s32 GotoMenu;
s32 D_80287554;
s32 D80281C40Pad[2];
struct UnkStruct80287560 D_80287560[30];

void func_80281C40(void) {
    s32 i;

    for (i = 0; i < D_802874FC; i++) {
        Kwfprintd(D_80287560[i].Unk0, D_80287560[i].Unk2, (char*) D_80287560[i].Unk8, D_80287560[i].unk4);
    }
}

void func_80281CB4(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3) {
    if (D_802874FC < 0x1E) {
        D_80287560[D_802874FC].Unk0 = arg0;
        D_80287560[D_802874FC].Unk2 = arg1;
        D_80287560[D_802874FC].unk4 = Arg3;
        D_80287560[D_802874FC].Unk8 = Arg2;
        D_802874FC++;
    }
}

extern Gfx D_80284F70[];
extern Gfx D_80284EE0[];

void DrawResultScreen(void) {
    Camera* Camera = &Cameras[0];
    UNUSED s32 pad[3];
    u16 perspNorm;
    Mat4 Matrix;
    UNUSED s32 Pad2[3];

    InitialDrawCommon();
    InitRDP();
    if (GotoMenu != 0xFFFF) {
        SoftwareBlanking(0);
        if (D_80287554 >= 4) {
            FadeOutFlag = 0;
            NewSequenceMode = GotoMenu;
        }
        D_80287554++;
        return;
    }
    func_8028150C();
    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    guPerspective((Mtx*) &DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], AspectRatio, ScreenNearPoint,
                  FarClip, 1.0f);
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt((Mtx*) &DynamicP->MTXLookAt[0], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    LoadIdentAffineMtx(Matrix);
    SetMatrix(Matrix, 0);
    gSPDisplayList(DisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284F70));
    KartCheckFuncA();
    gSPDisplayList(DisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284EE0));
    UpdateActorsLoop();
    Kwdisplay3D(PLAYER_ONE + SCREEN_MODE_1P);
    KartCheckFuncA2();
    gSPDisplayList(DisplayListHead++, VIRTUAL_TO_PHYSICAL2(&D_80284EE0));
    KawanoDrawResult();
    CeremonyTransitionSlidingBorders();
    func_80281C40();
    InitRDP();
}
