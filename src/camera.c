#include <ultra64.h>
#include <macros.h>
#include <common_structs.h>
#include <defines.h>
#include <libc/math.h>
#include <mk64.h>

#include "camera.h"
#include "code_800029B0.h"
#include "math_util.h"
#include "racing/memory.h"
#include "path.h"
#include "render_player.h"
#include "collision.h"
#include "code_80057C60.h"
#include "cpu_vehicles_camera_path.h"
#include "main.h"
#include "spawn_players.h"

#include <course.h>

f32 D_800DDB30[] = { 0.4f, 0.6f, 0.275f, 0.3f };

Camera cameras[4];
Camera* camera1 = &cameras[0];
Camera* camera2 = &cameras[1];
Camera* camera3 = &cameras[2];
Camera* camera4 = &cameras[3];

UNUSED s32 D_801649D0[2];

f32 D_801649D8[4];
f32 D_801649E8[4];
f32 D_801649F8[4];
s32 D_80164A08[4];
s32 D_80164A18[4];
s32 opening_flag;
s32 D_80164A2C;
f32 D_80164A30;
UNUSED f32 D_80164A34;
f32 D_80164A38[4];
f32 D_80164A48[4];
UNUSED s32 D_80164A58[8];
f32 D_80164A78[4];
s8 D_80164A88;
s8 D_80164A89;
// UNUSED s8 D_80164A8C[3];
f32 D_80164A90[4];
f32 D_80164AA0[4];

extern f32 cam_offset[];
extern s16 kotei_gakaku_mode[];

void initial_camera(f32 posX, f32 posY, f32 posZ, UNUSED s16 rot, u32 arg4, s32 cameraId) {
    Player* player = gPlayerOne;
    Camera* camera = &cameras[cameraId];

    camera_mode[cameraId] = arg4;
    switch (arg4) {
        case 0:
        case 1:
        case 3:
        case 8:
        case 9:
        case 10:
            D_80164A89 = 0;
            camera->camera_pos[0] = posX;
            camera->camera_pos[1] = posY;
            camera->camera_pos[2] = posZ;
            camera->flag = 0;
            camera->lookat_pos[0] = 0.0f;
            camera->lookat_pos[2] = 150.0f;
            camera->lookat_pos[1] = posY - 3.0;
            camera->up_vector[0] = 0.0f;
            camera->up_vector[1] = 1.0f;
            camera->up_vector[2] = 0.0f;
            camera->watch = (s16) cameraId;
            camera->stickdeg = 0;
            camera->offset_look = 0.0f;

            D_801649D8[cameraId] = 20.0f;
            D_801649E8[cameraId] = 10.0f;
            D_801649F8[cameraId] = 7.0f;
            D_80164A2C = 0;
            D_80164A30 = 30.0f;
            D_80164A38[cameraId] = 0.0f;
            D_80164A48[cameraId] = 0.0f;

            D_80164A90[cameraId] = 0.0f;
            D_80164AA0[cameraId] = 0.0f;
            D_80164A78[cameraId] = D_800DDB30[g_ScreenSplitA];
            D_80164A18[cameraId] = 0;
            D_80164A08[cameraId] = 0;
            cam_offset[cameraId] = 0.0f;
            camera->shake.cam_timer = 0;
            camera->shake.cam_firstspeed = 0.0f;

            player += cameraId;
            camera->chase_direction = player->direction[1];
            camera->old_chase_direction = player->direction[1];
            switch (g_ScreenSplitA) {
                case SCREEN_MODE_1P:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    if (g_gameMode == BATTLE) {
                        camera->camera_vector[0] = 0.0f;
                        camera->camera_vector[1] = 11.6f;
                        camera->camera_vector[2] = -38.5f;
                        camera->lookat_vector[0] = 0.0f;
                        camera->lookat_vector[1] = 0.0f;
                        camera->lookat_vector[2] = 19.2f;
                        D_80164A88 = 0;
                    } else {
                        camera->camera_vector[0] = 0.0f;
                        camera->camera_vector[1] = 9.5f;
                        camera->camera_vector[2] = -50.0f;
                        camera->lookat_vector[0] = 0.0f;
                        camera->lookat_vector[1] = 0.0f;
                        camera->lookat_vector[2] = 70.0f;
                    }
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                    if (g_gameMode == BATTLE) {
                        camera->camera_vector[0] = 0.0f;
                        camera->camera_vector[1] = 11.6f;
                        camera->camera_vector[2] = -38.5f;
                        camera->lookat_vector[0] = 0.0f;
                        camera->lookat_vector[1] = 0.0f;
                        camera->lookat_vector[2] = 19.2f;
                    } else {
                        camera->camera_vector[0] = 0.0f;
                        camera->camera_vector[1] = 9.6f;
                        camera->camera_vector[2] = -35.0f;
                        camera->lookat_vector[0] = 0.0f;
                        camera->lookat_vector[1] = 0.0f;
                        camera->lookat_vector[2] = 30.0f;
                    }
                    break;
                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    if (g_gameMode == BATTLE) {
                        camera->camera_vector[0] = 0.0f;
                        camera->camera_vector[1] = 11.6f;
                        camera->camera_vector[2] = -38.5f;
                        camera->lookat_vector[0] = 0.0f;
                        camera->lookat_vector[1] = 0.0f;
                        camera->lookat_vector[2] = 19.2f;
                    } else {
                        camera->camera_vector[0] = 0.0f;
                        camera->camera_vector[1] = 9.0f;
                        camera->camera_vector[2] = -40.0f;
                        camera->lookat_vector[0] = 0.0f;
                        camera->lookat_vector[1] = 0.0f;
                        camera->lookat_vector[2] = 18.0f;
                    }
                    break;
            }

            ogawa_camera_init(cameraId);

            if (kotei_gakaku_mode[cameraId] == 0) {
                if (opening_flag == 1) {
                    gCameraZoom[cameraId] = 80.0f;
                } else {
                    gCameraZoom[cameraId] = 40.0f;
                }
                camera->screen_view_angle = gCameraZoom[cameraId];
            }
            if (kotei_gakaku_mode[cameraId] == 1) {
                if (opening_flag == 1) {
                    gCameraZoom[cameraId] = 100.0f;
                } else {
                    gCameraZoom[cameraId] = 60.0f;
                }
                camera->screen_view_angle = gCameraZoom[cameraId];
            }
            if (kotei_gakaku_mode[cameraId] == 2) {
                if (opening_flag == 1) {
                    gCameraZoom[cameraId] = 100.0f;
                } else {
                    gCameraZoom[cameraId] = 60.0f;
                }
                camera->screen_view_angle = gCameraZoom[cameraId];
                D_80164A38[cameraId] = 20.0f;
                D_80164A48[cameraId] = 1.5f;
                D_80164A78[cameraId] = 1.0f;
            }
            break;
    }
    MakeDirection3D(camera->camera_pos, camera->lookat_pos, camera->camera_direction);
}

void ShakeCamera(Camera* camera) {
    camera->shake.cam_timer = 0;
    camera->shake.cam_firstspeed = 6.0f;
}

void ShakeHitCamera(Player* player, f32 arg1) {
    Camera* camera = &cameras[0];

    if (player == gPlayerTwo) {
        camera += 1;
    }
    if (player == gPlayerThree) {
        camera += 2;
    }
    if (player == gPlayerFour) {
        camera += 3;
    }
    camera->shake.cam_timer = 0;
    camera->shake.cam_firstspeed = arg1;
}

void opendirection_camera(UNUSED Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, UNUSED s32 huh,
                   UNUSED s32 wut) {
    Mat3 sp74;
    Vec3f sp68;
    Vec3f sp5C;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 var_f14;
    f32 temp_f18;
    f32 temp_f16;
    UNUSED s32 pad;
    CenterPathStruct* temp_s2;

    temp_s2 = &CenterPathBP[0][g_courseTotalPathPoints[0] - 10];
    sp68[0] = camera->camera_vector[0];
    sp68[1] = camera->camera_vector[1];
    sp68[2] = camera->camera_vector[2];
    sp5C[0] = camera->lookat_vector[0];
    sp5C[1] = camera->lookat_vector[1];
    sp5C[2] = camera->lookat_vector[2];
    arg2[0] = camera->lookat_pos[0];
    arg2[1] = camera->lookat_pos[1];
    arg2[2] = camera->lookat_pos[2];
    MakeAlignMatrix(sp74, 0, 1, 0, -0x00008000);
    MultipleMatrixByVector(sp5C, sp74);
    if (g_courseID == COURSE_TOADS_TURNPIKE) {
        var_f14 = sp5C[0];
    } else {
        var_f14 = sp5C[0] + temp_s2->pointx;
    }
    temp_f16 = grid_z[7] + sp5C[2];
    temp_f18 = sp5C[1] + (temp_s2->pointy + D_80164A30);
    arg2[0] += (var_f14 - camera->lookat_pos[0]) * 1;
    arg2[1] += (temp_f18 - camera->lookat_pos[1]) * 1;
    arg2[2] += (temp_f16 - camera->lookat_pos[2]) * 1;
    MultipleMatrixByVector(sp68, sp74);
    if (g_courseID == COURSE_TOADS_TURNPIKE) {
        var_f14 = sp68[0];
    } else {
        var_f14 = sp68[0] + temp_s2->pointx;
    }
    temp_f16 = grid_z[7] + sp68[2];
    temp_f18 = sp68[1] + (temp_s2->pointy + D_80164A30 + 6.0f);
    chase_Fnumber(&D_80164A30, 0, 0.02f);
    posX = camera->camera_pos[0];
    *arg3 = ((var_f14 - posX) * 1) + posX;
    posY = camera->camera_pos[1];
    *arg4 = ((temp_f18 - posY) * 1) + posY;
    posZ = camera->camera_pos[2];
    *arg5 = ((temp_f16 - posZ) * 1) + posZ;
}

void direction_camera(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, UNUSED s32* arg6,
                   s16 arg7, s16 index) {
    Mat3 sp9C;
    Vec3f sp90;
    Vec3f sp84;
    UNUSED s32 pad[3];
    f32 x;
    f32 y;
    f32 z;
    UNUSED s32 pad2;
    f32 var_f2;
    s16 var_v1;
    f32 temp_f0;
    f32 var_f0;
    s16 var_v0;
    f32 temp_f12;

    var_v1 = player->sus.gachon_timer;
    var_f2 = player->sus.gachon_firstspeed;

    var_v1++;
    temp_f0 = ((var_f2 * var_v1) - (0.7 * (var_v1 * var_v1)));
    if ((var_v1 != 0) && (temp_f0 < 0)) {
        var_v1 = 0;
        var_f2 *= 0.8;
        if (var_f2 <= 0.1) {
            var_f2 = 0;
        }
    }
    if (temp_f0 <= 0) {
        temp_f0 = 0;
    }
    player->sus.gachon_timer = var_v1;
    player->sus.gachon_firstspeed = var_f2;
    var_v0 = camera->shake.cam_timer;
    var_f0 = camera->shake.cam_firstspeed;
    var_v0++;
    temp_f12 = (var_v0 * var_f0) - (1.25 * (var_v0 * var_v0));
    if ((var_v0 != 0) && (temp_f12 < 0)) {
        var_v0 = 0;
        var_f0 *= 0.9;
        if (var_f0 <= 0.1) {
            var_f0 = 0;
        }
    }
    if (temp_f12 <= 0) {
        temp_f12 = 0;
        // fakematch
        if (!var_v0) {}
    }
    camera->shake.cam_timer = var_v0;
    camera->shake.cam_firstspeed = var_f0;
    if (kotei_gakaku_mode[index] == 2) {
        chase_Fnumber(&D_80164A38[index], 20.0f, 0.1f);
        chase_Fnumber(&D_80164A48[index], 1.5f, 0.1f);
        D_80164A78[index] += 0.1;
        if (D_80164A78[index] >= 1) {
            D_80164A78[index] = 1;
        }

    } else {
        chase_Fnumber(&D_80164A38[index], 0, 0.1f);
        chase_Fnumber(&D_80164A48[index], 0, 0.1f);
        D_80164A78[index] -= 0.1;
        if (D_800DDB30[g_ScreenSplitA] >= D_80164A78[index]) {
            D_80164A78[index] = D_800DDB30[g_ScreenSplitA];
        }
    }
    if ((player->jugemu_flag & LAKITU_CAMERA) == LAKITU_CAMERA) {
        switch (g_ScreenSplitA) {
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                chase_Fnumber(&D_80164A90[index], 20, 0.02f);
                chase_Fnumber(&D_80164AA0[index], 10, 0.02f);
                break;
            default:
                if (g_courseID == COURSE_YOSHI_VALLEY) {
                    chase_Fnumber(&D_80164A90[index], 50, 0.04f);
                    chase_Fnumber(&D_80164AA0[index], 35, 0.04f);
                } else {
                    chase_Fnumber(&D_80164A90[index], 40, 0.02f);
                    chase_Fnumber(&D_80164AA0[index], 20, 0.02f);
                }
                break;
        }
    } else {
        chase_Fnumber(&D_80164A90[index], 0, 0.04f);
        chase_Fnumber(&D_80164AA0[index], 0, 0.04f);
    }
    sp90[0] = camera->camera_vector[0];
    sp90[1] =
        camera->camera_vector[1] + (player->sus.bound * 0.85) - D_80164A48[index] + D_80164AA0[index] + (temp_f12 / 2);
    sp90[2] = camera->camera_vector[2] + temp_f0 + D_80164A38[index];
    sp84[0] = camera->lookat_vector[0];
    sp84[1] = camera->lookat_vector[1] + (player->sus.bound * 0.85) + temp_f12;
    sp84[2] = camera->lookat_vector[2] + temp_f0 - D_80164A90[index];
    arg2[0] = camera->lookat_pos[0];
    arg2[1] = camera->lookat_pos[1];
    arg2[2] = camera->lookat_pos[2];
    if ((player->slip_flag & EXPLODE) == EXPLODE) {
        sp84[2] /= 3.0f;
    }
    MakeAlignMatrix(sp9C, 0, 1, 0, arg7);
    MultipleMatrixByVector(sp84, sp9C);

    x = player->position[0] + sp84[0];
    z = player->position[2] + sp84[2];
    y = player->position[1] + sp84[1];

    arg2[0] += (x - camera->lookat_pos[0]) * D_80164A78[index];
    arg2[2] += ((z - camera->lookat_pos[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->slip_flag & D_JUMP) == D_JUMP)) {
        arg2[1] += ((y - camera->lookat_pos[1]) * 0.02);
    } else {
        arg2[1] += ((y - camera->lookat_pos[1]) * 0.5);
    }
    MultipleMatrixByVector(sp90, sp9C);
    x = player->position[0] + sp90[0];
    z = player->position[2] + sp90[2];
    if ((player->slip_flag & EXPLODE) != EXPLODE) {
        var_f0 = player->position[1] + sp90[1];
        // permute
        y = var_f0;
    } else {
        y = player->ground + player->radius + sp90[1];
    }

    *arg3 = camera->camera_pos[0] + ((x - camera->camera_pos[0]) * D_80164A78[index]);
    *arg5 = camera->camera_pos[2] + ((z - camera->camera_pos[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->slip_flag & D_JUMP) == D_JUMP)) {
        *arg4 = camera->camera_pos[1] + (((y - camera->camera_pos[1]) * 0.01));
    } else {
        *arg4 = camera->camera_pos[1] + (((y - camera->camera_pos[1]) * 0.15));
    }

    if ((player->water_flag & SUBMERGED) != 0) {
        *arg4 = g_waterlevelPlayer[index];
    }
}

void direction_camera_lookup(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, s16 arg6, s16 arg7) {
    Mat3 sp74;
    Vec3f sp68;
    Vec3f sp5C;
    f32 stackPadding0;
    f32 stackPadding1;
    f32 stackPadding2;
    UNUSED f32 pad[4];
    f32 thing;

    if (((u16) player->hangflag == 0) && (camera->offset_look == 0.0f)) {
        camera->offset_look = 0.0f;
    }
    if ((u16) player->hangflag != 4) {
        chase_Fnumber(&camera->offset_look, 20.0f, 0.06f);
    } else {
        chase_Fnumber(&camera->offset_look, 0.0f, 0.06f);
    }
    thing = g_waterlevelPlayer[arg7];
    sp68[0] = camera->camera_vector[0];
    sp68[1] = camera->camera_vector[1];
    sp68[2] = camera->camera_vector[2];
    sp5C[0] = camera->lookat_vector[0];
    sp5C[1] = camera->lookat_vector[1] + camera->offset_look;
    sp5C[2] = camera->lookat_vector[2];
    arg2[0] = camera->lookat_pos[0];
    arg2[1] = camera->lookat_pos[1];
    arg2[2] = camera->lookat_pos[2];
    MakeAlignMatrix(sp74, 0.0f, 1.0f, 0.0f, arg6);
    MultipleMatrixByVector(sp5C, sp74);
    stackPadding0 = player->position[0] + sp5C[0];
    stackPadding2 = player->position[2] + sp5C[2];
    stackPadding1 = player->position[1] + sp5C[1];
    arg2[0] += (stackPadding0 - camera->lookat_pos[0]) * 1;
    arg2[2] += (stackPadding2 - camera->lookat_pos[2]) * 1;
    arg2[1] += (stackPadding1 - camera->lookat_pos[1]) * 1;
    MultipleMatrixByVector(sp68, sp74);
    stackPadding0 = player->position[0] + sp68[0];
    stackPadding2 = player->position[2] + sp68[2];
    stackPadding1 = sp68[1] + (player->ground + 1.5);
    if ((player->jugemu_flag & IS_IN_WATER) == IS_IN_WATER) {
        stackPadding1 = sp68[1] + (thing + 10.0f);
    }
    *arg3 = stackPadding0;
    *arg4 = stackPadding1;
    *arg5 = stackPadding2;
    D_80164A90[arg7] = 0.0f;
    D_80164AA0[arg7] = 0.0f;
}

void direction_fixcamera(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, s16 arg6) {
    Mat3 sp6C;
    Vec3f sp60;
    Vec3f sp54;
    UNUSED f32 stackPadding[4];
    f32 test1;
    f32 test2;
    f32 test3;

    sp60[0] = camera->camera_vector[0];
    sp60[1] = camera->camera_vector[1];
    sp60[2] = camera->camera_vector[2] - 6;

    sp54[0] = camera->lookat_vector[0];
    sp54[1] = camera->lookat_vector[1];
    sp54[2] = camera->lookat_vector[2];

    arg2[0] = camera->lookat_pos[0];
    arg2[1] = camera->lookat_pos[1];
    arg2[2] = camera->lookat_pos[2];

    MakeAlignMatrix(sp6C, 0, 1, 0, arg6);
    MultipleMatrixByVector(sp54, sp6C);

    test1 = player->position[0] + sp54[0];
    test3 = player->position[2] + sp54[2];
    test2 = player->position[1] + sp54[1];
    arg2[0] += (test1 - camera->lookat_pos[0]) * 1;
    arg2[1] += (test2 - camera->lookat_pos[1]) * 1;
    arg2[2] += (test3 - camera->lookat_pos[2]) * 1;

    MultipleMatrixByVector(sp60, sp6C);

    test1 = player->position[0] + sp60[0];
    test3 = player->position[2] + sp60[2];
    test2 = player->position[1] + sp60[1];
    *arg3 = camera->camera_pos[0] + ((test1 - camera->camera_pos[0]) * 1);
    *arg4 = camera->camera_pos[1] + ((test2 - camera->camera_pos[1]) * 1);
    *arg5 = camera->camera_pos[2] + ((test3 - camera->camera_pos[2]) * 1);
}

void direction_battlecamera(Player* player, Camera* camera, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, UNUSED s32* arg6,
                   s16 arg7, s16 index) {
    Mat3 sp9C;
    Vec3f sp90;
    Vec3f sp84;
    UNUSED s32 pad[3];
    f32 x;
    f32 y;
    f32 z;
    UNUSED s32 pad2;
    f32 var_f2;
    s16 var_v1;
    f32 temp_f0;
    f32 var_f0;
    s16 var_v0;
    f32 temp_f12;

    var_v1 = player->sus.gachon_timer;
    var_f2 = player->sus.gachon_firstspeed;

    var_v1++;
    temp_f0 = ((var_f2 * var_v1) - (0.7 * (var_v1 * var_v1)));
    if ((var_v1 != 0) && (temp_f0 < 0)) {
        var_v1 = 0;
        var_f2 *= 0.8;
        if (var_f2 <= 0.1) {
            var_f2 = 0;
        }
    }
    if (temp_f0 <= 0) {
        temp_f0 = 0;
    }
    player->sus.gachon_timer = var_v1;
    player->sus.gachon_firstspeed = var_f2;
    var_v0 = camera->shake.cam_timer;
    var_f0 = camera->shake.cam_firstspeed;
    var_v0++;
    temp_f12 = (var_v0 * var_f0) - (1.25 * (var_v0 * var_v0));
    if ((var_v0 != 0) && (temp_f12 < 0)) {
        var_v0 = 0;
        var_f0 *= 0.9;
        if (var_f0 <= 0.1) {
            var_f0 = 0;
        }
    }
    if (temp_f12 <= 0) {
        temp_f12 = 0;
        // fakematch
        if (!var_v0) {}
    }
    camera->shake.cam_timer = var_v0;
    camera->shake.cam_firstspeed = var_f0;
    if (kotei_gakaku_mode[index] == 2) {
        chase_Fnumber(&D_80164A38[index], 20.0f, 0.1f);
        chase_Fnumber(&D_80164A48[index], 1.5f, 0.1f);
        D_80164A78[index] += 0.1;
        if (D_80164A78[index] >= 1) {
            D_80164A78[index] = 1;
        }

    } else {
        chase_Fnumber(&D_80164A38[index], 0, 0.1f);
        chase_Fnumber(&D_80164A48[index], 0, 0.1f);
        D_80164A78[index] -= 0.1;
        if (D_800DDB30[g_ScreenSplitA] >= D_80164A78[index]) {
            D_80164A78[index] = D_800DDB30[g_ScreenSplitA];
        }
    }
    if ((player->jugemu_flag & LAKITU_CAMERA) == LAKITU_CAMERA) {

        chase_Fnumber(&D_80164A90[index], 15, 0.02f);
        chase_Fnumber(&D_80164AA0[index], 20, 0.02f);
    } else {
        chase_Fnumber(&D_80164A90[index], 0, 0.02f);
        chase_Fnumber(&D_80164AA0[index], 0, 0.02f);
    }
    sp90[0] = camera->camera_vector[0];
    sp90[1] =
        camera->camera_vector[1] + (player->sus.bound * 0.85) - D_80164A48[index] + D_80164AA0[index] + (temp_f12 / 2);
    sp90[2] = camera->camera_vector[2] + temp_f0 + D_80164A38[index] + D_80164AA0[index];
    sp84[0] = camera->lookat_vector[0];
    sp84[1] = camera->lookat_vector[1] + (player->sus.bound * 0.85) + temp_f12;
    sp84[2] = camera->lookat_vector[2] + temp_f0 - D_80164A90[index];
    arg2[0] = camera->lookat_pos[0];
    arg2[1] = camera->lookat_pos[1];
    arg2[2] = camera->lookat_pos[2];
    if ((player->slip_flag & EXPLODE) == EXPLODE) {
        sp84[2] /= 3.0f;
    }
    MakeAlignMatrix(sp9C, 0, 1, 0, arg7);
    MultipleMatrixByVector(sp84, sp9C);

    x = player->position[0] + sp84[0];
    z = player->position[2] + sp84[2];
    y = player->position[1] + sp84[1];

    arg2[0] += (x - camera->lookat_pos[0]) * D_80164A78[index];
    arg2[2] += ((z - camera->lookat_pos[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->slip_flag & D_JUMP) == D_JUMP)) {
        arg2[1] += ((y - camera->lookat_pos[1]) * 0.02);
    } else {
        arg2[1] += ((y - camera->lookat_pos[1]) * 0.5);
    }
    MultipleMatrixByVector(sp90, sp9C);
    x = player->position[0] + sp90[0];
    z = player->position[2] + sp90[2];
    if ((player->slip_flag & EXPLODE) != EXPLODE) {
        var_f0 = player->position[1] + sp90[1];
        // permute
        y = var_f0;
    } else {
        y = player->ground + player->radius + sp90[1];
    }

    *arg3 = camera->camera_pos[0] + ((x - camera->camera_pos[0]) * D_80164A78[index]);
    *arg5 = camera->camera_pos[2] + ((z - camera->camera_pos[2]) * D_80164A78[index]);

    if ((((player->speed / 18) * 216) <= 5.0f) && ((player->slip_flag & D_JUMP) == D_JUMP)) {
        *arg4 = camera->camera_pos[1] + (((y - camera->camera_pos[1]) * 0.01));
    } else {
        *arg4 = camera->camera_pos[1] + (((y - camera->camera_pos[1]) * 0.15));
    }

    if ((player->water_flag & SUBMERGED) != 0) {
        *arg4 = g_waterlevelPlayer[index];
    }
}

void openingview(Camera* camera, Player* player, s8 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp80;
    f32 temp_f14;
    UNUSED s32 pad2;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    Vec3f sp60;
    s16 temp_t7;
    s16 var_a2;
    UNUSED s32 pad3[8];
    s32 test = 3;

    if (player->sterrangle == 0) {
        var_a2 = 0x0064;
    } else if (player->sterrangle < 0) {
        var_a2 = 0x87 - (player->sterrangle / 3);
    } else {
        var_a2 = (player->sterrangle / 3) + 0x87;
    }
    ChaseDir(&camera->chase_direction, player->direction[1], var_a2);
    opendirection_camera(player, camera, sp60, &sp74, &sp70, &sp6C, camera->chase_direction, arg2);
    camera->flag &= ~0x0004;
    temp_t7 = CheckBump(&camera->bump, test, sp74, sp70, sp6C);
    if (camera->bump.distance_zx < 0.0f) {
        sp74 += -camera->bump.bump_zx[0] * camera->bump.distance_zx * 1;
        sp70 += -camera->bump.bump_zx[1] * camera->bump.distance_zx * 0.5;
        sp6C += -camera->bump.bump_zx[2] * camera->bump.distance_zx * 1;
    }
    if (camera->bump.distance_xy < 0.0f) {
        camera->flag = camera->flag | 4 | 2;
        sp74 += -camera->bump.bump_xy[0] * camera->bump.distance_xy * 1.5;
        sp70 += -camera->bump.bump_xy[1] * camera->bump.distance_xy * 1;
        sp6C += -camera->bump.bump_xy[2] * camera->bump.distance_xy * 1.5;
    }
    if (camera->bump.distance_yz < 0.0f) {
        camera->flag = camera->flag | 4 | 2;
        sp74 += -camera->bump.bump_yz[0] * camera->bump.distance_yz * 1.5;
        sp70 += -camera->bump.bump_yz[1] * camera->bump.distance_yz * 1;
        sp6C += -camera->bump.bump_yz[2] * camera->bump.distance_yz * 1.5;
    }
    if ((temp_t7 == 0) && ((camera->flag & 2) != 2)) {
        camera->old_chase_direction = camera->chase_direction;
    }
    camera->lookat_pos[0] = sp60[0];
    camera->lookat_pos[1] = sp60[1];
    camera->lookat_pos[2] = sp60[2];
    camera->camera_pos[0] = sp74;
    camera->camera_pos[1] = sp70;
    camera->camera_pos[2] = sp6C;
    temp_f12 = camera->lookat_pos[0] - camera->camera_pos[0];
    sp80 = camera->lookat_pos[1] - camera->camera_pos[1];
    temp_f14 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(temp_f12, temp_f14);
    camera->camera_direction[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp80);
    camera->camera_direction[2] = 0;
}

// This function has a few stack variables.
void KartView(Camera* camera, Player* player, s8 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    UNUSED s32 pad2;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED s32 pad3[3];
    Vec3f sp64;
    UNUSED s32 pad4[2];
    s32 sp58;
    UNUSED s16 pad5[4];
    s16 var_a3;
    UNUSED s16 pad6;
    s16 temp;

    if ((player->slip_flag & DRIFT) == DRIFT) {
        var_a3 = 100;
        if (player->sterrangle == 0) {
            camera->stickdeg = 0;
        } else {
            if (player->sterrangle < 0) {
                var_a3 = 0xA5 - (player->sterrangle / 2);
                if ((player->slip_flag & STOPSTERR) == STOPSTERR) {
                    chase_Snumber(&camera->stickdeg, -0x0B60, 0.1f);
                } else {
                    chase_Snumber(&camera->stickdeg, -DEGREES(12), 0.1f);
                }
            } else {
                var_a3 = (player->sterrangle / 2) + 0xA5;
                if ((player->slip_flag & STOPSTERR) == STOPSTERR) {
                    chase_Snumber(&camera->stickdeg, 0x0B60, 0.1f);
                } else {
                    chase_Snumber(&camera->stickdeg, DEGREES(12), 0.1f);
                }
            }
        }
    } else {
        chase_Snumber(&camera->stickdeg, 0, 0.05f);
        var_a3 = ((s16) camera->chase_direction / DEGREES(1)) - ((s16) player->direction[1] / DEGREES(1));
        if (player->sterrangle == 0) {
            if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
                var_a3 = 0x02D8;
            } else {
                var_a3 = 0x01F4;
            }
        } else if (player->sterrangle < 0) {
            if ((var_a3 <= -70) || (var_a3 >= 70)) {
                var_a3 = 0xB4 - player->sterrangle;
            } else {
                var_a3 = 0xA5 - (player->sterrangle / 2);
            }
        } else if ((var_a3 <= -70) || (var_a3 >= 0x46)) {
            var_a3 = player->sterrangle + 0xB4;
        } else {
            var_a3 = (player->sterrangle / 2) + 0xA5;
        }
    }
    if (((player->slip_flag & SPIN_L) == SPIN_L) ||
        ((player->slip_flag & SPIN_R) == SPIN_R) ||
        ((player->slip_flag & RAPID_ACC) == RAPID_ACC) ||
        ((player->slip_flag & WING) == WING) ||
        ((player->slip_flag & STORM) == STORM) ||
        (((player->slip_flag & SPIN_TURN) == SPIN_TURN) && (player->sterrangle != 0)) ||
        (player->bump.distance_xy <= 0.0f) || (player->bump.distance_yz <= 0.0f) ||
        ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN)) {
        direction_camera(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->chase_direction, (s32) arg2);
    } else {
        ChaseDir(&camera->chase_direction, (s16) (player->direction[1] + camera->stickdeg), var_a3);
        direction_camera(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->chase_direction, (s32) arg2);
    }
    temp = 3;
    camera->flag &= 0xFFFB;
    CheckBump(&camera->bump, temp, sp84, sp80, sp7C);

    camera->camera_pos[0] = sp84;
    camera->camera_pos[1] = sp80;
    camera->camera_pos[2] = sp7C;

    camera->lookat_pos[0] = sp64[0];
    camera->lookat_pos[1] = sp64[1];
    camera->lookat_pos[2] = sp64[2];

    temp_f12 = camera->lookat_pos[0] - camera->camera_pos[0];
    sp90 = camera->lookat_pos[1] - camera->camera_pos[1];
    temp_f14 = camera->lookat_pos[2] - camera->camera_pos[2];

    camera->camera_direction[1] = Atan2t(temp_f12, temp_f14);
    camera->camera_direction[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    camera->camera_direction[2] = 0;
}

void lookup_view(Camera* camera, Player* player, s8 arg2) {
    UNUSED f32 pad[6];
    f32 temp_f12;
    f32 sp88;
    f32 temp_f14;
    UNUSED f32 pad2;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    UNUSED Vec3f pad3;
    Vec3f sp5C;
    UNUSED f32 pad4[10];

    camera->stickdeg = 0;
    camera->chase_direction = player->direction[1];
    direction_camera_lookup(player, camera, sp5C, &sp7C, &sp78, &sp74, (s16) (s32) player->direction[1], (s16) (s32) arg2);
    CheckBump(&camera->bump, 5.0f, sp7C, sp78, sp74);
    camera->lookat_pos[0] = sp5C[0];
    camera->lookat_pos[1] = sp5C[1];
    camera->lookat_pos[2] = sp5C[2];
    camera->camera_pos[0] = sp7C;
    camera->camera_pos[1] = sp78;
    camera->camera_pos[2] = sp74;
    temp_f12 = camera->lookat_pos[0] - camera->camera_pos[0];
    sp88 = camera->lookat_pos[1] - camera->camera_pos[1];
    temp_f14 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(temp_f12, temp_f14);
    camera->camera_direction[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp88);
    camera->camera_direction[2] = 0;
}

void battleview(Camera* camera, Player* player, s8 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    UNUSED s32 pad2;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED s32 pad3[3];
    Vec3f sp64;
    UNUSED s32 pad4[2];
    s32 sp58;
    UNUSED s16 pad5[4];
    s16 var_a3;
    UNUSED s16 pad6;
    s16 temp;

    if ((player->slip_flag & DRIFT) == DRIFT) {
        var_a3 = 100;
        if (player->sterrangle == 0) {
            camera->stickdeg = 0;
        } else {
            if (player->sterrangle < 0) {
                var_a3 = 0xA5 - (player->sterrangle / 2);

                if ((player->slip_flag & STOPSTERR) == STOPSTERR) {
                    chase_Snumber(&camera->stickdeg, -0x0B60, 0.1f);
                } else {
                    chase_Snumber(&camera->stickdeg, -DEGREES(12), 0.1f);
                }
            } else {
                var_a3 = (player->sterrangle / 2) + 0xA5;
                if ((player->slip_flag & STOPSTERR) == STOPSTERR) {
                    chase_Snumber(&camera->stickdeg, 0x0B60, 0.1f);
                } else {
                    chase_Snumber(&camera->stickdeg, DEGREES(12), 0.1f);
                }
            }
        }
    } else {
        chase_Snumber(&camera->stickdeg, 0, 0.05f);
        var_a3 = ((s16) camera->chase_direction / DEGREES(1)) - ((s16) player->direction[1] / DEGREES(1));
        if (player->sterrangle == 0) {
            if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
                var_a3 = 0x02D8;
            } else {
                var_a3 = 0x01F4;
            }
        } else if (player->sterrangle < 0) {
            if ((var_a3 <= -70) || (var_a3 >= 70)) {
                var_a3 = 0xB4 - player->sterrangle;
            } else {
                var_a3 = 0xA5 - (player->sterrangle / 2);
            }
        } else if ((var_a3 <= -70) || (var_a3 >= 0x46)) {
            var_a3 = player->sterrangle + 0xB4;
        } else {
            var_a3 = (player->sterrangle / 2) + 0xA5;
        }
    }
    if (((player->slip_flag & SPIN_L) == SPIN_L) ||
        ((player->slip_flag & SPIN_R) == SPIN_R) ||
        ((player->slip_flag & RAPID_ACC) == RAPID_ACC) ||
        ((player->slip_flag & WING) == WING) ||
        ((player->slip_flag & STORM) == STORM) ||
        (((player->slip_flag & SPIN_TURN) == SPIN_TURN) && (player->sterrangle != 0)) ||
        (player->bump.distance_xy <= 0.0f) || (player->bump.distance_yz <= 0.0f) ||
        ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN)) {
        direction_battlecamera(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->chase_direction, (s32) arg2);
    } else {
        ChaseDir(&camera->chase_direction, (s16) (player->direction[1] + camera->stickdeg), var_a3);
        direction_battlecamera(player, camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) camera->chase_direction, (s32) arg2);
    }
    temp = 3;
    camera->flag &= 0xFFFB;
    CheckBump(&camera->bump, temp, sp84, sp80, sp7C);

    camera->camera_pos[0] = sp84;
    camera->camera_pos[1] = sp80;
    camera->camera_pos[2] = sp7C;

    camera->lookat_pos[0] = sp64[0];
    camera->lookat_pos[1] = sp64[1];
    camera->lookat_pos[2] = sp64[2];

    temp_f12 = camera->lookat_pos[0] - camera->camera_pos[0];
    sp90 = camera->lookat_pos[1] - camera->camera_pos[1];
    temp_f14 = camera->lookat_pos[2] - camera->camera_pos[2];

    camera->camera_direction[1] = Atan2t(temp_f12, temp_f14);
    camera->camera_direction[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    camera->camera_direction[2] = 0;
}

void CameraControl(Player* player, Camera* camera, s8 index) {
    s32 cameraIndex;

    if (camera == camera1) {
        cameraIndex = 0;
    }
    if (camera == camera2) {
        cameraIndex = 1;
    }
    if (camera == camera3) {
        cameraIndex = 2;
    }
    if (camera == camera4) {
        cameraIndex = 3;
    }
    switch (g_gameMode) {
        case GRAND_PRIX:
            // clang-format off
            if (((player->flag & IS_RACE_FINISH) == IS_RACE_FINISH) || (g_DemoFlag == 1)) { camera_mode[cameraIndex] = 3;
            //             -->                 -->        Scroll right        -->      bit more     -->     ^ Required for matching
                // clang-format on
            } else if (pause_flag == 1) {
                oga_game_camera_all(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
            } else {
                oga_game_camera(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
            }
            break;
        case BATTLE:
            if ((g_DemoFlag == 1) || ((finish_zoom_flag == 2) && (D_80164A89 == 1))) {
                if (D_80164A88 == 0) {
                    oga_battle_camera_init();
                }
                D_80164A88 = 1;
                camera_mode[0] = 3;
                camera_mode[1] = 3;
                camera_mode[2] = 3;
                camera_mode[3] = 3;
            } else {
                D_80164A88 = 0;
                if (pause_flag == 1) {
                    oga_game_camera_all(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
                } else {
                    oga_game_camera(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
                }
                camera_mode[cameraIndex] = 9;
            }
            break;
        case TIME_TRIALS:
            if (((gPlayerOne->flag & IS_RACE_FINISH) == IS_RACE_FINISH) || (g_DemoFlag == 1)) {
                camera_mode[0] = 3;
                camera_mode[1] = 3;
                camera_mode[2] = 3;
                camera_mode[3] = 3;
            } else {
                if (pause_flag == 1) {
                    oga_game_camera_all(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
                } else {
                    oga_game_camera(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
                }
                camera_mode[cameraIndex] = 1;
            }
            break;
        case VERSUS:
            if (((player->flag & IS_RACE_FINISH) == IS_RACE_FINISH) || (g_DemoFlag == 1) ||
                (finish_zoom_flag == 2)) {
                camera_mode[cameraIndex] = 3;
            } else {
                if (pause_flag == 1) {
                    oga_game_camera_all(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
                } else {
                    oga_game_camera(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
                }
                camera_mode[cameraIndex] = 1;
            }
            break;
    }
    if (pause_flag == 0) {
        switch (camera_mode[cameraIndex]) {
            case 3:
                Func_8001A588(&camera_mode[cameraIndex], camera, player, index, cameraIndex);
                break;
            case 1:
                if (((player->jugemu_flag & IS_IN_WATER) == IS_IN_WATER) ||
                    ((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
                    lookup_view(camera, player, index);
                    break;
                }
                KartView(camera, player, index);
                break;
            case 8:
                openingview(camera, player, index);
                change_opneview(cameraIndex);
                break;
            case 9:
                if (((player->jugemu_flag & IS_IN_WATER) == IS_IN_WATER) ||
                    ((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
                    lookup_view(camera, player, index);
                    break;
                }
                battleview(camera, player, index);
                break;
        }
    }
}

void make_view_ang(Player* player, f32* arg1) {
    f32 var_f0;
    UNUSED s32 pad;
    s32 playerIndex;
    UNUSED s32 pad2;
    Camera* camera = &cameras[0];

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }

    if (D_80164A08[playerIndex] == 0) {
        if (player->weapon & USE_BANANA) {
            D_80164A08[playerIndex] = 1;
        }
        if ((player->slip_flag & TURBO) == TURBO) {
            D_80164A08[playerIndex] = 2;
        }
        if ((player->slip_flag & DASH_JUMP) == DASH_JUMP) {
            D_80164A08[playerIndex] = 3;
        }
        if ((player->weapon & PRESS_SQUISH) == PRESS_SQUISH) {
            D_80164A08[playerIndex] = 4;
        }
        if (((player->slip_flag & SPIN_L) == SPIN_L) ||
            ((player->slip_flag & SPIN_R) == SPIN_R)) {
            D_80164A08[playerIndex] = 5;
        }
        cam_offset[playerIndex] = 0.0f;
    }
    switch (D_80164A08[playerIndex]) {
        case 1:
            if (player->weapon & USE_BANANA) {
                chase_Fnumber(&cam_offset[playerIndex], 20.0f, 0.2f);
            } else {
                if (cam_offset[playerIndex] > 1.0f) {
                    cam_offset[playerIndex] -= 1.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    cam_offset[playerIndex] = 0.0f;
                }
            }
            break;
        case 2:
            if ((player->slip_flag & TURBO) == TURBO) {
                if (player->turbo_timer != 0) {
                    chase_Fnumber(&cam_offset[playerIndex], 8.0f, 0.2f);
                }
            } else {
                if (cam_offset[playerIndex] > 1.0f) {
                    cam_offset[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    cam_offset[playerIndex] = 0.0f;
                }
            }
            break;
        case 3:
            if (((player->slip_flag & DASH_JUMP) == DASH_JUMP) &&
                ((player->slip_flag & N_JUMP) == N_JUMP)) {
                chase_Fnumber(&cam_offset[playerIndex], 20.0f, 0.1f);
            } else {
                if (cam_offset[playerIndex] > 1.0f) {
                    cam_offset[playerIndex] -= 1.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    cam_offset[playerIndex] = 0.0f;
                }
            }
            break;
        case 4:
            if ((player->weapon & PRESS_SQUISH) == PRESS_SQUISH) {
                chase_Fnumber(&cam_offset[playerIndex], 25.0f, 1.0f);
            } else {
                if (cam_offset[playerIndex] > 1.0f) {
                    cam_offset[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    cam_offset[playerIndex] = 0.0f;
                }
            }
            break;
        case 5:
            if (((player->slip_flag & SPIN_L) == SPIN_L) ||
                ((player->slip_flag & SPIN_R) == SPIN_R)) {
                chase_Fnumber(&cam_offset[playerIndex], 18.0f, 0.2f);
            } else {
                if (cam_offset[playerIndex] > 1.0f) {
                    cam_offset[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    cam_offset[playerIndex] = 0.0f;
                }
            }
            break;
    }
    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            if (opening_flag == 1) {
                cam_offset[playerIndex] = 40.0f;
            }
            if (opening_flag == 2) {
                if (cam_offset[playerIndex] >= 0.0f) {
                    cam_offset[playerIndex] -= 0.8;
                }
                if (cam_offset[playerIndex] <= 0.0f) {
                    opening_flag = 0;
                    cam_offset[playerIndex] = 0.0f;
                }
            }
            var_f0 = ogawa_gakaku(*arg1, playerIndex);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            var_f0 = ogawa_gakaku(*arg1, playerIndex);
            break;
    }
    *arg1 = var_f0;
    camera += playerIndex; // In 500 words or less, please explain why?
    camera->screen_view_angle = var_f0;
}

void change_opneview(s32 cameraId) {
    s32 playerIndex;
    // Why?
    s32 id = cameraId;

    if (gPlayerOne) {}
    if (g_ScreenSplitA == SCREEN_MODE_1P) {
        if (g_gameMode == GRAND_PRIX) {
            for (playerIndex = 0; playerIndex < NUM_PLAYERS; playerIndex++) {
                if ((gPlayerOne[playerIndex].flag & IS_GP_OPENING) ||
                    (gPlayerOne[playerIndex].flag & PLAYER_UNKNOWN_0x80)) {
                    break;
                }
                if (playerIndex == 7) {
                    D_80164A2C += 1;
                }
                if ((playerIndex == 7) && (D_80164A2C == 0x0000003C)) {
                    opening_flag = 2;
                    camera_mode[id] = 1;
                    cameras[id].camera_direction[1] = gPlayerOne[playerIndex].direction[1];
                    cameras[id].chase_direction = gPlayerOne[playerIndex].direction[1];
                }
            }
        }
    }
}
