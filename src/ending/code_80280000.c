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
    map_strategy();
    KWVideoFramesYori();
    KWVideoFramesYori();
    kwgameframesyori();
}

void func_80280038(void) {
    u16 perspNorm;
    Camera* camera = &cameras[0];
    UNUSED s32 pad;
    Mat4 matrix;

    gMatrixCount = 0;
    effectcount = 0;
    modelingnum = 0;
    InitRDP();
    InitialDrawCommon();
    InitRDP();
    kwdisplay_kumo(0);

    gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER | G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    guPerspective(&gDynamicP->mtxPersp[0], &perspNorm, gCameraZoom[0], g_aspectRatio, screen_near_point, g_farClip,
                  1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[0]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    guLookAt(&gDynamicP->mtxLookAt[0], camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], camera->lookat_pos[0],
             camera->lookat_pos[1], camera->lookat_pos[2], camera->up_vector[0], camera->up_vector[1], camera->up_vector[2]);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[0]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
    g_courseID = gCreditsCourseId;
    LoadIdentAffineMtx(matrix);
    SetMatrix(matrix, 0);
    display_map(D_800DC5EC);
    display_object(D_800DC5EC);
    kwdisplay3D(PLAYER_ONE + SCREEN_MODE_1P);
    kwdisplay3D_after(PLAYER_ONE + SCREEN_MODE_1P);
    ceremony_transition_sliding_borders();
    func_80281C40();
    InitRDP();
    kawanoDrawResult();
    InitRDP();
}

void mapchange(s32 arg0) {
    g_fadeOutFlag = 1;
    g_fadeOutCounter = 5;
    D_802874A0 = 1;
    if ((arg0 < 0) || ((arg0 >= 20))) {
        arg0 = 0;
    }
    gCreditsCourseId = arg0;
}

void EndingSequence(void) {
    Camera* camera = &cameras[0];

    f32 temp_f12;
    f32 temp;
    f32 temp_f14;

    D_802874A0 = 0;
    if (g_fadeOutFlag) {
        g_fadeOutCounter--;
        if (g_fadeOutCounter == 0) {
            g_fadeOutFlag = 0;
            g_NewSequenceMode = CREDITS_SEQUENCE;
            gGamestate = 255;
        }
    } else {

        D_802874FC = 0;
        Grou_camdemo_main(camera);
        temp_f12 = camera->lookat_pos[0] - camera->camera_pos[0];
        temp = camera->lookat_pos[1] - camera->camera_pos[1];
        temp_f14 = camera->lookat_pos[2] - camera->camera_pos[2];
        camera->camera_direction[1] = Atan2t(temp_f12, temp_f14);
        camera->camera_direction[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), temp);
        camera->camera_direction[2] = 0;
        if (D_802874A0 != 0) {
            D_800DC5E4++;
        } else {
            func_80280000();
            func_80280038();
#if DVDL
            display_dvdl();
#endif
            gDPFullSync(gDisplayListHead++);
            gSPEndDisplayList(gDisplayListHead++);
        }
    }
}

void InitializeEndingSequence(void) {
    Camera* camera = &cameras[0];

    g_courseID = gCreditsCourseId;
    gBackgroundFlag = 1;
    g_DisplayFlag = 1;
    initital_perspective();
    initial_screen_data();
    camera->screen_view_angle = 60.0f;
    gCameraZoom[0] = 60.0f;
    D_800DC5EC->width = SCREEN_WIDTH;
    D_800DC5EC->height = SCREEN_HEIGHT;
    D_800DC5EC->posx = 160;
    D_800DC5EC->posy = 120;
    g_ScreenSplitB = SCREEN_MODE_1P;
    g_ScreenSplitA = SCREEN_MODE_1P;
    FreeMemoryPointer = StaticMemoryPointer;
    VertexMemoryPointer = VERTEX_MEMORY_TOP;
    ObjectMemoryPointer = (uintptr_t) ok_ObjectPoolStart;
    load_course(g_courseID);
    map_memory_pointer = FreeMemoryPointer;
    SetSegment(0xB, (void*) load_press_data((u8*) CEREMONY_DATA_ROM_START, (u8*) CEREMONY_DATA_ROM_END));
    CheckCourseMemoryHighWater();

    minimum_x = -0x15A1;
    minimum_y = -0x15A1;
    minimum_z = -0x15A1;

    maximum_x = 0x15A1;
    maximum_y = 0x15A1;
    maximum_z = 0x15A1;
    wall_flag = 0;
    flat_flag = 0;
    facenumber = 0;
    g_courseFaceMaxIndex = 0;
    g_skyToggle = 0;
    g_fadeOutCounter2 = 0;
    gFaceBuffer = (FaceStruct*) FACE_BUFFER_START;
    camera->camera_pos[0] = 1400.0f;
    camera->camera_pos[1] = 300.0f;
    camera->camera_pos[2] = 1400.0f;
    camera->lookat_pos[0] = 0.0f;
    camera->lookat_pos[1] = 0.0f;
    camera->lookat_pos[2] = 0.0f;
    camera->up_vector[0] = 0.0f;
    camera->up_vector[1] = 1.0f;
    camera->up_vector[2] = 0.0f;
    Grou_camdemo_init();
    initial_ending_map();
    initialize_params();
    init_kawanoResult();
    StartKawanoEnding();
    if (D_800DC5EC) {}
    D_801625F8 = ((s32) LastMemoryPointer - FreeMemoryPointer);
    D_801625FC = ((f32) D_801625F8 / 1000.0f);
}
