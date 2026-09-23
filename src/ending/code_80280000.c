#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <debug.h>
#include <PR/gu.h>
#include <mk64.h>
#include <course.h>

#include "main.h"
#include <segments.h>
#include "code_800029B0.h"
#include "camera.h"
#include "memory.h"
#include "math_util.h"
#include "code_80280000.h"
#include "code_80281780.h"
#include "skybox_and_splitscreen.h"
#include "menu_items.h"
#include "code_8006E9C0.h"
#include "code_800029B0.h"
#include "ceremony_and_credits.h"
#include "podium_ceremony_actors.h"
#include "code_80281C40.h"
#include "code_80057C60.h"
#include "actors.h"
#include "render_courses.h"
#include "main.h"
#include "render_player.h"

s32 D_802874A0;
// s32 D_802874A4[5];

void func_80280000(void) {
    MapStrategy();
    KWVideoFramesYori();
    KWVideoFramesYori();
    Kwgameframesyori();
}

void func_80280038(void) {
    u16 perspNorm;
    Camera* Camera = &Cameras[0];
    UNUSED s32 pad;
    Mat4 Matrix;

    MatrixCount = 0;
    Effectcount = 0;
    Modelingnum = 0;
    InitRDP();
    InitialDrawCommon();
    InitRDP();
    KwdisplayKumo(0);

    gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    guPerspective(&DynamicP->MTXPersp[0], &perspNorm, CameraZoom[0], AspectRatio, ScreenNearPoint, FarClip,
                  1.0f);
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&DynamicP->MTXLookAt[0], Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0],
             Camera->LookatPos[1], Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    CourseID = CreditsCourseId;
    LoadIdentAffineMtx(Matrix);
    SetMatrix(Matrix, 0);
    DisplayMap(D_800DC5EC);
    DisplayCourseObject(D_800DC5EC);
    Kwdisplay3D(PLAYER_ONE + SCREEN_MODE_1P);
    Kwdisplay3DAfter(PLAYER_ONE + SCREEN_MODE_1P);
    CeremonyTransitionSlidingBorders();
    func_80281C40();
    InitRDP();
    KawanoDrawResult();
    InitRDP();
}

void Mapchange(s32 arg0) {
    FadeOutFlag = 1;
    FadeOutCounter = 5;
    D_802874A0 = 1;
    if ((arg0 < 0) || ((arg0 >= 20))) {
        arg0 = 0;
    }
    CreditsCourseId = arg0;
}

void EndingSequence(void) {
    Camera* Camera = &Cameras[0];

    f32 temp_f12;
    f32 temp;
    f32 temp_f14;

    D_802874A0 = 0;
    if (FadeOutFlag) {
        FadeOutCounter--;
        if (FadeOutCounter == 0) {
            FadeOutFlag = 0;
            NewSequenceMode = CREDITS_SEQUENCE;
            GameState = 255;
        }
    } else {

        D_802874FC = 0;
        GrouCamdemoMain(Camera);
        temp_f12 = Camera->LookatPos[0] - Camera->CameraPos[0];
        temp = Camera->LookatPos[1] - Camera->CameraPos[1];
        temp_f14 = Camera->LookatPos[2] - Camera->CameraPos[2];
        Camera->CameraDirection[1] = Atan2t(temp_f12, temp_f14);
        Camera->CameraDirection[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), temp);
        Camera->CameraDirection[2] = 0;
        if (D_802874A0 != 0) {
            D_800DC5E4++;
        } else {
            func_80280000();
            func_80280038();
#if DVDL
            display_dvdl();
#endif
            gDPFullSync(DisplayListHead++);
            gSPEndDisplayList(DisplayListHead++);
        }
    }
}

void InitializeEndingSequence(void) {
    Camera* Camera = &Cameras[0];

    CourseID = CreditsCourseId;
    BackgroundFlag = 1;
    DisplayFlag = 1;
    InititalPerspective();
    InitialScreenData();
    Camera->ScreenViewAngle = 60.0f;
    CameraZoom[0] = 60.0f;
    D_800DC5EC->Width = SCREEN_WIDTH;
    D_800DC5EC->Height = SCREEN_HEIGHT;
    D_800DC5EC->Posx = 160;
    D_800DC5EC->Posy = 120;
    ScreenSplitB = SCREEN_MODE_1P;
    ScreenSplitA = SCREEN_MODE_1P;
    FreeMemoryPointer = StaticMemoryPointer;
    VertexMemoryPointer = VERTEX_MEMORY_TOP;
    ObjectMemoryPointer = (uintptr_t) ok_ObjectPoolStart;
    LoadCourse(CourseID);
    MapMemoryPointer = FreeMemoryPointer;
    SetSegment(0xB, (void*) LoadPressData((u8*) CEREMONY_DATA_ROM_START, (u8*) CEREMONY_DATA_ROM_END));
    CheckCourseMemoryHighWater();

    MinimumX = -0x15A1;
    MinimumY = -0x15A1;
    MinimumZ = -0x15A1;

    MaximumX = 0x15A1;
    MaximumY = 0x15A1;
    MaximumZ = 0x15A1;
    WallFlag = 0;
    FlatFlag = 0;
    Facenumber = 0;
    CourseFaceMaxIndex = 0;
    SkyToggle = 0;
    FadeOutCounter2 = 0;
    FaceBuffer = (FaceStruct*) FACE_BUFFER_START;
    Camera->CameraPos[0] = 1400.0f;
    Camera->CameraPos[1] = 300.0f;
    Camera->CameraPos[2] = 1400.0f;
    Camera->LookatPos[0] = 0.0f;
    Camera->LookatPos[1] = 0.0f;
    Camera->LookatPos[2] = 0.0f;
    Camera->UpVector[0] = 0.0f;
    Camera->UpVector[1] = 1.0f;
    Camera->UpVector[2] = 0.0f;
    GrouCamdemoInit();
    InitialEndingMap();
    InitializeParams();
    InitKawanoResult();
    StartKawanoEnding();
    if (D_800DC5EC) {}
    D_801625F8 = ((s32) LastMemoryPointer - FreeMemoryPointer);
    D_801625FC = ((f32) D_801625F8 / 1000.0f);
}
