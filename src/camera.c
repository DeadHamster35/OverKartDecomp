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
#include "OverKart5Hooks.h"

#include <course.h>

f32 D_800DDB30[] = { 0.4f, 0.6f, 0.275f, 0.3f };

Camera Cameras[4];
Camera* Camera1 = &Cameras[0];
Camera* Camera2 = &Cameras[1];
Camera* Camera3 = &Cameras[2];
Camera* Camera4 = &Cameras[3];

UNUSED s32 D_801649D0[2];

f32 D_801649D8[4];
f32 D_801649E8[4];
f32 D_801649F8[4];
s32 D_80164A08[4];
s32 D_80164A18[4];
s32 OpeningFlag;
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

extern f32 CamOffset[];
extern s16 KoteiGakakuMode[];

void InitialCamera(f32 PosX, f32 PosY, f32 PosZ, UNUSED s16 rot, u32 arg4, s32 CameraId) {
    Player* Player = PlayerOne;
    Camera* Camera = &Cameras[CameraId];

    CameraMode[CameraId] = arg4;
    switch (arg4) {
        case 0:
        case 1:
        case 3:
        case 8:
        case 9:
        case 10:
            D_80164A89 = 0;
            Camera->CameraPos[0] = PosX;
            Camera->CameraPos[1] = PosY;
            Camera->CameraPos[2] = PosZ;
            Camera->Flag = 0;
            Camera->LookatPos[0] = 0.0f;
            Camera->LookatPos[2] = 150.0f;
            Camera->LookatPos[1] = PosY - 3.0;
            Camera->UpVector[0] = 0.0f;
            Camera->UpVector[1] = 1.0f;
            Camera->UpVector[2] = 0.0f;
            Camera->Watch = (s16) CameraId;
            Camera->Stickdeg = 0;
            Camera->OffsetLook = 0.0f;

            D_801649D8[CameraId] = 20.0f;
            D_801649E8[CameraId] = 10.0f;
            D_801649F8[CameraId] = 7.0f;
            D_80164A2C = 0;
            D_80164A30 = 30.0f;
            D_80164A38[CameraId] = 0.0f;
            D_80164A48[CameraId] = 0.0f;

            D_80164A90[CameraId] = 0.0f;
            D_80164AA0[CameraId] = 0.0f;
            D_80164A78[CameraId] = D_800DDB30[ScreenSplitA];
            D_80164A18[CameraId] = 0;
            D_80164A08[CameraId] = 0;
            CamOffset[CameraId] = 0.0f;
            Camera->Shake.CamTimer = 0;
            Camera->Shake.CamFirstspeed = 0.0f;

            Player += CameraId;
            Camera->ChaseDirection = Player->Direction[1];
            Camera->OldChaseDirection = Player->Direction[1];
            switch (ScreenSplitA) {
                case SCREEN_MODE_1P:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    if (GameMode == BATTLE) {
                        Camera->CameraVector[0] = 0.0f;
                        Camera->CameraVector[1] = 11.6f;
                        Camera->CameraVector[2] = -38.5f;
                        Camera->LookatVector[0] = 0.0f;
                        Camera->LookatVector[1] = 0.0f;
                        Camera->LookatVector[2] = 19.2f;
                        D_80164A88 = 0;
                    } else {
                        Camera->CameraVector[0] = 0.0f;
                        Camera->CameraVector[1] = 9.5f;
                        Camera->CameraVector[2] = -50.0f;
                        Camera->LookatVector[0] = 0.0f;
                        Camera->LookatVector[1] = 0.0f;
                        Camera->LookatVector[2] = 70.0f;
                    }
                    break;
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                    if (GameMode == BATTLE) {
                        Camera->CameraVector[0] = 0.0f;
                        Camera->CameraVector[1] = 11.6f;
                        Camera->CameraVector[2] = -38.5f;
                        Camera->LookatVector[0] = 0.0f;
                        Camera->LookatVector[1] = 0.0f;
                        Camera->LookatVector[2] = 19.2f;
                    } else {
                        Camera->CameraVector[0] = 0.0f;
                        Camera->CameraVector[1] = 9.6f;
                        Camera->CameraVector[2] = -35.0f;
                        Camera->LookatVector[0] = 0.0f;
                        Camera->LookatVector[1] = 0.0f;
                        Camera->LookatVector[2] = 30.0f;
                    }
                    break;
                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    if (GameMode == BATTLE) {
                        Camera->CameraVector[0] = 0.0f;
                        Camera->CameraVector[1] = 11.6f;
                        Camera->CameraVector[2] = -38.5f;
                        Camera->LookatVector[0] = 0.0f;
                        Camera->LookatVector[1] = 0.0f;
                        Camera->LookatVector[2] = 19.2f;
                    } else {
                        Camera->CameraVector[0] = 0.0f;
                        Camera->CameraVector[1] = 9.0f;
                        Camera->CameraVector[2] = -40.0f;
                        Camera->LookatVector[0] = 0.0f;
                        Camera->LookatVector[1] = 0.0f;
                        Camera->LookatVector[2] = 18.0f;
                    }
                    break;
            }

            OgawaCameraInit(CameraId);

            if (KoteiGakakuMode[CameraId] == 0) {
                if (OpeningFlag == 1) {
                    CameraZoom[CameraId] = 80.0f;
                } else {
                    CameraZoom[CameraId] = 40.0f;
                }
                Camera->ScreenViewAngle = CameraZoom[CameraId];
            }
            if (KoteiGakakuMode[CameraId] == 1) {
                if (OpeningFlag == 1) {
                    CameraZoom[CameraId] = 100.0f;
                } else {
                    CameraZoom[CameraId] = 60.0f;
                }
                Camera->ScreenViewAngle = CameraZoom[CameraId];
            }
            if (KoteiGakakuMode[CameraId] == 2) {
                if (OpeningFlag == 1) {
                    CameraZoom[CameraId] = 100.0f;
                } else {
                    CameraZoom[CameraId] = 60.0f;
                }
                Camera->ScreenViewAngle = CameraZoom[CameraId];
                D_80164A38[CameraId] = 20.0f;
                D_80164A48[CameraId] = 1.5f;
                D_80164A78[CameraId] = 1.0f;
            }
            break;
    }
    MakeDirection3D(Camera->CameraPos, Camera->LookatPos, Camera->CameraDirection);
}

void ShakeCamera(Camera* Camera) {
    Camera->Shake.CamTimer = 0;
    Camera->Shake.CamFirstspeed = 6.0f;
}

void ShakeHitCamera(Player* Player, f32 arg1) {
    Camera* Camera = &Cameras[0];

    if (Player == PlayerTwo) {
        Camera += 1;
    }
    if (Player == PlayerThree) {
        Camera += 2;
    }
    if (Player == PlayerFour) {
        Camera += 3;
    }
    Camera->Shake.CamTimer = 0;
    Camera->Shake.CamFirstspeed = arg1;
}

void OpendirectionCamera(UNUSED Player* Player, Camera* Camera, Vec3f Arg2, f32* Arg3, f32* arg4, f32* arg5, UNUSED s32 huh,
                   UNUSED s32 wut) {
    Mat3 sp74;
    Vec3f sp68;
    Vec3f sp5C;
    f32 PosX;
    f32 PosY;
    f32 PosZ;
    f32 var_f14;
    f32 temp_f18;
    f32 temp_f16;
    UNUSED s32 pad;
    CenterPathStruct* temp_s2;

    temp_s2 = &CenterPathBP[0][CourseTotalPathPoints[0] - 10];
    sp68[0] = Camera->CameraVector[0];
    sp68[1] = Camera->CameraVector[1];
    sp68[2] = Camera->CameraVector[2];
    sp5C[0] = Camera->LookatVector[0];
    sp5C[1] = Camera->LookatVector[1];
    sp5C[2] = Camera->LookatVector[2];
    Arg2[0] = Camera->LookatPos[0];
    Arg2[1] = Camera->LookatPos[1];
    Arg2[2] = Camera->LookatPos[2];
    MakeAlignMatrix(sp74, 0, 1, 0, -0x00008000);
    MultipleMatrixByVector(sp5C, sp74);
    if (CourseID == COURSE_TOADS_TURNPIKE) {
        var_f14 = sp5C[0];
    } else {
        var_f14 = sp5C[0] + temp_s2->Pointx;
    }
    temp_f16 = GridZ[7] + sp5C[2];
    temp_f18 = sp5C[1] + (temp_s2->Pointy + D_80164A30);
    Arg2[0] += (var_f14 - Camera->LookatPos[0]) * 1;
    Arg2[1] += (temp_f18 - Camera->LookatPos[1]) * 1;
    Arg2[2] += (temp_f16 - Camera->LookatPos[2]) * 1;
    MultipleMatrixByVector(sp68, sp74);
    if (CourseID == COURSE_TOADS_TURNPIKE) {
        var_f14 = sp68[0];
    } else {
        var_f14 = sp68[0] + temp_s2->Pointx;
    }
    temp_f16 = GridZ[7] + sp68[2];
    temp_f18 = sp68[1] + (temp_s2->Pointy + D_80164A30 + 6.0f);
    ChaseFnumber(&D_80164A30, 0, 0.02f);
    PosX = Camera->CameraPos[0];
    *Arg3 = ((var_f14 - PosX) * 1) + PosX;
    PosY = Camera->CameraPos[1];
    *arg4 = ((temp_f18 - PosY) * 1) + PosY;
    PosZ = Camera->CameraPos[2];
    *arg5 = ((temp_f16 - PosZ) * 1) + PosZ;
}

void DirectionCamera(Player* Player, Camera* Camera, Vec3f Arg2, f32* Arg3, f32* arg4, f32* arg5, UNUSED s32* arg6,
                   s16 arg7, s16 Index) {
    Mat3 sp9C;
    Vec3f sp90;
    Vec3f sp84;
    UNUSED s32 pad[3];
    f32 x;
    f32 y;
    f32 z;
    UNUSED s32 Pad2;
    f32 var_f2;
    s16 var_v1;
    f32 temp_f0;
    f32 var_f0;
    s16 var_v0;
    f32 temp_f12;

    var_v1 = Player->sus.GachonTimer;
    var_f2 = Player->sus.GachonFirstspeed;

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
    Player->sus.GachonTimer = var_v1;
    Player->sus.GachonFirstspeed = var_f2;
    var_v0 = Camera->Shake.CamTimer;
    var_f0 = Camera->Shake.CamFirstspeed;
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
    Camera->Shake.CamTimer = var_v0;
    Camera->Shake.CamFirstspeed = var_f0;
    if (KoteiGakakuMode[Index] == 2) {
        ChaseFnumber(&D_80164A38[Index], 20.0f, 0.1f);
        ChaseFnumber(&D_80164A48[Index], 1.5f, 0.1f);
        D_80164A78[Index] += 0.1;
        if (D_80164A78[Index] >= 1) {
            D_80164A78[Index] = 1;
        }

    } else {
        ChaseFnumber(&D_80164A38[Index], 0, 0.1f);
        ChaseFnumber(&D_80164A48[Index], 0, 0.1f);
        D_80164A78[Index] -= 0.1;
        if (D_800DDB30[ScreenSplitA] >= D_80164A78[Index]) {
            D_80164A78[Index] = D_800DDB30[ScreenSplitA];
        }
    }
    if ((Player->JugemuFlag & LAKITU_CAMERA) == LAKITU_CAMERA) {
        switch (ScreenSplitA) {
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                ChaseFnumber(&D_80164A90[Index], 20, 0.02f);
                ChaseFnumber(&D_80164AA0[Index], 10, 0.02f);
                break;
            default:
                if (CourseID == COURSE_YOSHI_VALLEY) {
                    ChaseFnumber(&D_80164A90[Index], 50, 0.04f);
                    ChaseFnumber(&D_80164AA0[Index], 35, 0.04f);
                } else {
                    ChaseFnumber(&D_80164A90[Index], 40, 0.02f);
                    ChaseFnumber(&D_80164AA0[Index], 20, 0.02f);
                }
                break;
        }
    } else {
        ChaseFnumber(&D_80164A90[Index], 0, 0.04f);
        ChaseFnumber(&D_80164AA0[Index], 0, 0.04f);
    }
    sp90[0] = Camera->CameraVector[0];
    sp90[1] =
        Camera->CameraVector[1] + (Player->sus.Bound * 0.85) - D_80164A48[Index] + D_80164AA0[Index] + (temp_f12 / 2);
    sp90[2] = Camera->CameraVector[2] + temp_f0 + D_80164A38[Index];
    sp84[0] = Camera->LookatVector[0];
    sp84[1] = Camera->LookatVector[1] + (Player->sus.Bound * 0.85) + temp_f12;
    sp84[2] = Camera->LookatVector[2] + temp_f0 - D_80164A90[Index];
    Arg2[0] = Camera->LookatPos[0];
    Arg2[1] = Camera->LookatPos[1];
    Arg2[2] = Camera->LookatPos[2];
    if ((Player->SlipFlag & EXPLODE) == EXPLODE) {
        sp84[2] /= 3.0f;
    }
    MakeAlignMatrix(sp9C, 0, 1, 0, arg7);
    MultipleMatrixByVector(sp84, sp9C);

    x = Player->Position[0] + sp84[0];
    z = Player->Position[2] + sp84[2];
    y = Player->Position[1] + sp84[1];

    Arg2[0] += (x - Camera->LookatPos[0]) * D_80164A78[Index];
    Arg2[2] += ((z - Camera->LookatPos[2]) * D_80164A78[Index]);

    if ((((Player->Speed / 18) * 216) <= 5.0f) && ((Player->SlipFlag & D_JUMP) == D_JUMP)) {
        Arg2[1] += ((y - Camera->LookatPos[1]) * 0.02);
    } else {
        Arg2[1] += ((y - Camera->LookatPos[1]) * 0.5);
    }
    MultipleMatrixByVector(sp90, sp9C);
    x = Player->Position[0] + sp90[0];
    z = Player->Position[2] + sp90[2];
    if ((Player->SlipFlag & EXPLODE) != EXPLODE) {
        var_f0 = Player->Position[1] + sp90[1];
        // permute
        y = var_f0;
    } else {
        y = Player->Ground + Player->Radius + sp90[1];
    }

    *Arg3 = Camera->CameraPos[0] + ((x - Camera->CameraPos[0]) * D_80164A78[Index]);
    *arg5 = Camera->CameraPos[2] + ((z - Camera->CameraPos[2]) * D_80164A78[Index]);

    if ((((Player->Speed / 18) * 216) <= 5.0f) && ((Player->SlipFlag & D_JUMP) == D_JUMP)) {
        *arg4 = Camera->CameraPos[1] + (((y - Camera->CameraPos[1]) * 0.01));
    } else {
        *arg4 = Camera->CameraPos[1] + (((y - Camera->CameraPos[1]) * 0.15));
    }

    if ((Player->WaterFlag & SUBMERGED) != 0) {
        *arg4 = WaterlevelPlayer[Index];
    }
}

void DirectionCameraLookup(Player* Player, Camera* Camera, Vec3f Arg2, f32* Arg3, f32* arg4, f32* arg5, s16 arg6, s16 arg7) {
    Mat3 sp74;
    Vec3f sp68;
    Vec3f sp5C;
    f32 stackPadding0;
    f32 stackPadding1;
    f32 stackPadding2;
    UNUSED f32 pad[4];
    f32 thing;

    if (((u16) Player->Hangflag == 0) && (Camera->OffsetLook == 0.0f)) {
        Camera->OffsetLook = 0.0f;
    }
    if ((u16) Player->Hangflag != 4) {
        ChaseFnumber(&Camera->OffsetLook, 20.0f, 0.06f);
    } else {
        ChaseFnumber(&Camera->OffsetLook, 0.0f, 0.06f);
    }
    thing = WaterlevelPlayer[arg7];
    sp68[0] = Camera->CameraVector[0];
    sp68[1] = Camera->CameraVector[1];
    sp68[2] = Camera->CameraVector[2];
    sp5C[0] = Camera->LookatVector[0];
    sp5C[1] = Camera->LookatVector[1] + Camera->OffsetLook;
    sp5C[2] = Camera->LookatVector[2];
    Arg2[0] = Camera->LookatPos[0];
    Arg2[1] = Camera->LookatPos[1];
    Arg2[2] = Camera->LookatPos[2];
    MakeAlignMatrix(sp74, 0.0f, 1.0f, 0.0f, arg6);
    MultipleMatrixByVector(sp5C, sp74);
    stackPadding0 = Player->Position[0] + sp5C[0];
    stackPadding2 = Player->Position[2] + sp5C[2];
    stackPadding1 = Player->Position[1] + sp5C[1];
    Arg2[0] += (stackPadding0 - Camera->LookatPos[0]) * 1;
    Arg2[2] += (stackPadding2 - Camera->LookatPos[2]) * 1;
    Arg2[1] += (stackPadding1 - Camera->LookatPos[1]) * 1;
    MultipleMatrixByVector(sp68, sp74);
    stackPadding0 = Player->Position[0] + sp68[0];
    stackPadding2 = Player->Position[2] + sp68[2];
    stackPadding1 = sp68[1] + (Player->Ground + 1.5);
    if ((Player->JugemuFlag & IS_IN_WATER) == IS_IN_WATER) {
        stackPadding1 = sp68[1] + (thing + 10.0f);
    }
    *Arg3 = stackPadding0;
    *arg4 = stackPadding1;
    *arg5 = stackPadding2;
    D_80164A90[arg7] = 0.0f;
    D_80164AA0[arg7] = 0.0f;
}

void DirectionFixcamera(Player* Player, Camera* Camera, Vec3f Arg2, f32* Arg3, f32* arg4, f32* arg5, s16 arg6) {
    Mat3 sp6C;
    Vec3f sp60;
    Vec3f sp54;
    UNUSED f32 stackPadding[4];
    f32 test1;
    f32 test2;
    f32 test3;

    sp60[0] = Camera->CameraVector[0];
    sp60[1] = Camera->CameraVector[1];
    sp60[2] = Camera->CameraVector[2] - 6;

    sp54[0] = Camera->LookatVector[0];
    sp54[1] = Camera->LookatVector[1];
    sp54[2] = Camera->LookatVector[2];

    Arg2[0] = Camera->LookatPos[0];
    Arg2[1] = Camera->LookatPos[1];
    Arg2[2] = Camera->LookatPos[2];

    MakeAlignMatrix(sp6C, 0, 1, 0, arg6);
    MultipleMatrixByVector(sp54, sp6C);

    test1 = Player->Position[0] + sp54[0];
    test3 = Player->Position[2] + sp54[2];
    test2 = Player->Position[1] + sp54[1];
    Arg2[0] += (test1 - Camera->LookatPos[0]) * 1;
    Arg2[1] += (test2 - Camera->LookatPos[1]) * 1;
    Arg2[2] += (test3 - Camera->LookatPos[2]) * 1;

    MultipleMatrixByVector(sp60, sp6C);

    test1 = Player->Position[0] + sp60[0];
    test3 = Player->Position[2] + sp60[2];
    test2 = Player->Position[1] + sp60[1];
    *Arg3 = Camera->CameraPos[0] + ((test1 - Camera->CameraPos[0]) * 1);
    *arg4 = Camera->CameraPos[1] + ((test2 - Camera->CameraPos[1]) * 1);
    *arg5 = Camera->CameraPos[2] + ((test3 - Camera->CameraPos[2]) * 1);
}

void DirectionBattlecamera(Player* Player, Camera* Camera, Vec3f Arg2, f32* Arg3, f32* arg4, f32* arg5, UNUSED s32* arg6,
                   s16 arg7, s16 Index) {
    Mat3 sp9C;
    Vec3f sp90;
    Vec3f sp84;
    UNUSED s32 pad[3];
    f32 x;
    f32 y;
    f32 z;
    UNUSED s32 Pad2;
    f32 var_f2;
    s16 var_v1;
    f32 temp_f0;
    f32 var_f0;
    s16 var_v0;
    f32 temp_f12;

    var_v1 = Player->sus.GachonTimer;
    var_f2 = Player->sus.GachonFirstspeed;

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
    Player->sus.GachonTimer = var_v1;
    Player->sus.GachonFirstspeed = var_f2;
    var_v0 = Camera->Shake.CamTimer;
    var_f0 = Camera->Shake.CamFirstspeed;
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
    Camera->Shake.CamTimer = var_v0;
    Camera->Shake.CamFirstspeed = var_f0;
    if (KoteiGakakuMode[Index] == 2) {
        ChaseFnumber(&D_80164A38[Index], 20.0f, 0.1f);
        ChaseFnumber(&D_80164A48[Index], 1.5f, 0.1f);
        D_80164A78[Index] += 0.1;
        if (D_80164A78[Index] >= 1) {
            D_80164A78[Index] = 1;
        }

    } else {
        ChaseFnumber(&D_80164A38[Index], 0, 0.1f);
        ChaseFnumber(&D_80164A48[Index], 0, 0.1f);
        D_80164A78[Index] -= 0.1;
        if (D_800DDB30[ScreenSplitA] >= D_80164A78[Index]) {
            D_80164A78[Index] = D_800DDB30[ScreenSplitA];
        }
    }
    if ((Player->JugemuFlag & LAKITU_CAMERA) == LAKITU_CAMERA) {

        ChaseFnumber(&D_80164A90[Index], 15, 0.02f);
        ChaseFnumber(&D_80164AA0[Index], 20, 0.02f);
    } else {
        ChaseFnumber(&D_80164A90[Index], 0, 0.02f);
        ChaseFnumber(&D_80164AA0[Index], 0, 0.02f);
    }
    sp90[0] = Camera->CameraVector[0];
    sp90[1] =
        Camera->CameraVector[1] + (Player->sus.Bound * 0.85) - D_80164A48[Index] + D_80164AA0[Index] + (temp_f12 / 2);
    sp90[2] = Camera->CameraVector[2] + temp_f0 + D_80164A38[Index] + D_80164AA0[Index];
    sp84[0] = Camera->LookatVector[0];
    sp84[1] = Camera->LookatVector[1] + (Player->sus.Bound * 0.85) + temp_f12;
    sp84[2] = Camera->LookatVector[2] + temp_f0 - D_80164A90[Index];
    Arg2[0] = Camera->LookatPos[0];
    Arg2[1] = Camera->LookatPos[1];
    Arg2[2] = Camera->LookatPos[2];
    if ((Player->SlipFlag & EXPLODE) == EXPLODE) {
        sp84[2] /= 3.0f;
    }
    MakeAlignMatrix(sp9C, 0, 1, 0, arg7);
    MultipleMatrixByVector(sp84, sp9C);

    x = Player->Position[0] + sp84[0];
    z = Player->Position[2] + sp84[2];
    y = Player->Position[1] + sp84[1];

    Arg2[0] += (x - Camera->LookatPos[0]) * D_80164A78[Index];
    Arg2[2] += ((z - Camera->LookatPos[2]) * D_80164A78[Index]);

    if ((((Player->Speed / 18) * 216) <= 5.0f) && ((Player->SlipFlag & D_JUMP) == D_JUMP)) {
        Arg2[1] += ((y - Camera->LookatPos[1]) * 0.02);
    } else {
        Arg2[1] += ((y - Camera->LookatPos[1]) * 0.5);
    }
    MultipleMatrixByVector(sp90, sp9C);
    x = Player->Position[0] + sp90[0];
    z = Player->Position[2] + sp90[2];
    if ((Player->SlipFlag & EXPLODE) != EXPLODE) {
        var_f0 = Player->Position[1] + sp90[1];
        // permute
        y = var_f0;
    } else {
        y = Player->Ground + Player->Radius + sp90[1];
    }

    *Arg3 = Camera->CameraPos[0] + ((x - Camera->CameraPos[0]) * D_80164A78[Index]);
    *arg5 = Camera->CameraPos[2] + ((z - Camera->CameraPos[2]) * D_80164A78[Index]);

    if ((((Player->Speed / 18) * 216) <= 5.0f) && ((Player->SlipFlag & D_JUMP) == D_JUMP)) {
        *arg4 = Camera->CameraPos[1] + (((y - Camera->CameraPos[1]) * 0.01));
    } else {
        *arg4 = Camera->CameraPos[1] + (((y - Camera->CameraPos[1]) * 0.15));
    }

    if ((Player->WaterFlag & SUBMERGED) != 0) {
        *arg4 = WaterlevelPlayer[Index];
    }
}

void Openingview(Camera* Camera, Player* Player, s8 Arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp80;
    f32 temp_f14;
    UNUSED s32 Pad2;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    Vec3f sp60;
    s16 temp_t7;
    s16 var_a2;
    UNUSED s32 Pad3[8];
    s32 test = 3;

    if (Player->Sterrangle == 0) {
        var_a2 = 0x0064;
    } else if (Player->Sterrangle < 0) {
        var_a2 = 0x87 - (Player->Sterrangle / 3);
    } else {
        var_a2 = (Player->Sterrangle / 3) + 0x87;
    }
    ChaseDir(&Camera->ChaseDirection, Player->Direction[1], var_a2);
    OpendirectionCamera(Player, Camera, sp60, &sp74, &sp70, &sp6C, Camera->ChaseDirection, Arg2);
    Camera->Flag &= ~0x0004;
    temp_t7 = CheckBump(&Camera->Bump, test, sp74, sp70, sp6C);
    if (Camera->Bump.DistanceZX < 0.0f) {
        sp74 += -Camera->Bump.BumpZX[0] * Camera->Bump.DistanceZX * 1;
        sp70 += -Camera->Bump.BumpZX[1] * Camera->Bump.DistanceZX * 0.5;
        sp6C += -Camera->Bump.BumpZX[2] * Camera->Bump.DistanceZX * 1;
    }
    if (Camera->Bump.DistanceXY < 0.0f) {
        Camera->Flag = Camera->Flag | 4 | 2;
        sp74 += -Camera->Bump.BumpXY[0] * Camera->Bump.DistanceXY * 1.5;
        sp70 += -Camera->Bump.BumpXY[1] * Camera->Bump.DistanceXY * 1;
        sp6C += -Camera->Bump.BumpXY[2] * Camera->Bump.DistanceXY * 1.5;
    }
    if (Camera->Bump.DistanceYZ < 0.0f) {
        Camera->Flag = Camera->Flag | 4 | 2;
        sp74 += -Camera->Bump.BumpYZ[0] * Camera->Bump.DistanceYZ * 1.5;
        sp70 += -Camera->Bump.BumpYZ[1] * Camera->Bump.DistanceYZ * 1;
        sp6C += -Camera->Bump.BumpYZ[2] * Camera->Bump.DistanceYZ * 1.5;
    }
    if ((temp_t7 == 0) && ((Camera->Flag & 2) != 2)) {
        Camera->OldChaseDirection = Camera->ChaseDirection;
    }
    Camera->LookatPos[0] = sp60[0];
    Camera->LookatPos[1] = sp60[1];
    Camera->LookatPos[2] = sp60[2];
    Camera->CameraPos[0] = sp74;
    Camera->CameraPos[1] = sp70;
    Camera->CameraPos[2] = sp6C;
    temp_f12 = Camera->LookatPos[0] - Camera->CameraPos[0];
    sp80 = Camera->LookatPos[1] - Camera->CameraPos[1];
    temp_f14 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(temp_f12, temp_f14);
    Camera->CameraDirection[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp80);
    Camera->CameraDirection[2] = 0;
}

// This function has a few Stack variables.
void KartView(Camera* Camera, Player* Player, s8 Arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    UNUSED s32 Pad2;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED s32 Pad3[3];
    Vec3f sp64;
    UNUSED s32 pad4[2];
    s32 sp58;
    UNUSED s16 pad5[4];
    s16 var_a3;
    UNUSED s16 pad6;
    s16 temp;

    if ((Player->SlipFlag & DRIFT) == DRIFT) {
        var_a3 = 100;
        if (Player->Sterrangle == 0) {
            Camera->Stickdeg = 0;
        } else {
            if (Player->Sterrangle < 0) {
                var_a3 = 0xA5 - (Player->Sterrangle / 2);
                if ((Player->SlipFlag & STOPSTERR) == STOPSTERR) {
                    ChaseSnumber(&Camera->Stickdeg, -0x0B60, 0.1f);
                } else {
                    ChaseSnumber(&Camera->Stickdeg, -DEGREES(12), 0.1f);
                }
            } else {
                var_a3 = (Player->Sterrangle / 2) + 0xA5;
                if ((Player->SlipFlag & STOPSTERR) == STOPSTERR) {
                    ChaseSnumber(&Camera->Stickdeg, 0x0B60, 0.1f);
                } else {
                    ChaseSnumber(&Camera->Stickdeg, DEGREES(12), 0.1f);
                }
            }
        }
    } else {
        ChaseSnumber(&Camera->Stickdeg, 0, 0.05f);
        var_a3 = ((s16) Camera->ChaseDirection / DEGREES(1)) - ((s16) Player->Direction[1] / DEGREES(1));
        if (Player->Sterrangle == 0) {
            if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
                var_a3 = 0x02D8;
            } else {
                var_a3 = 0x01F4;
            }
        } else if (Player->Sterrangle < 0) {
            if ((var_a3 <= -70) || (var_a3 >= 70)) {
                var_a3 = 0xB4 - Player->Sterrangle;
            } else {
                var_a3 = 0xA5 - (Player->Sterrangle / 2);
            }
        } else if ((var_a3 <= -70) || (var_a3 >= 0x46)) {
            var_a3 = Player->Sterrangle + 0xB4;
        } else {
            var_a3 = (Player->Sterrangle / 2) + 0xA5;
        }
    }
    if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
        ((Player->SlipFlag & SPIN_R) == SPIN_R) ||
        ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) ||
        ((Player->SlipFlag & WING) == WING) ||
        ((Player->SlipFlag & STORM) == STORM) ||
        (((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) && (Player->Sterrangle != 0)) ||
        (Player->Bump.DistanceXY <= 0.0f) || (Player->Bump.DistanceYZ <= 0.0f) ||
        ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN)) {
        DirectionCamera(Player, Camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) Camera->ChaseDirection, (s32) Arg2);
    } else {
        ChaseDir(&Camera->ChaseDirection, (s16) (Player->Direction[1] + Camera->Stickdeg), var_a3);
        DirectionCamera(Player, Camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) Camera->ChaseDirection, (s32) Arg2);
    }
    temp = 3;
    Camera->Flag &= 0xFFFB;
    CheckBump(&Camera->Bump, temp, sp84, sp80, sp7C);

    Camera->CameraPos[0] = sp84;
    Camera->CameraPos[1] = sp80;
    Camera->CameraPos[2] = sp7C;

    Camera->LookatPos[0] = sp64[0];
    Camera->LookatPos[1] = sp64[1];
    Camera->LookatPos[2] = sp64[2];

    temp_f12 = Camera->LookatPos[0] - Camera->CameraPos[0];
    sp90 = Camera->LookatPos[1] - Camera->CameraPos[1];
    temp_f14 = Camera->LookatPos[2] - Camera->CameraPos[2];

    Camera->CameraDirection[1] = Atan2t(temp_f12, temp_f14);
    Camera->CameraDirection[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    Camera->CameraDirection[2] = 0;
}

void LookupView(Camera* Camera, Player* Player, s8 Arg2) {
    UNUSED f32 pad[6];
    f32 temp_f12;
    f32 sp88;
    f32 temp_f14;
    UNUSED f32 Pad2;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    UNUSED Vec3f Pad3;
    Vec3f sp5C;
    UNUSED f32 pad4[10];

    Camera->Stickdeg = 0;
    Camera->ChaseDirection = Player->Direction[1];
    DirectionCameraLookup(Player, Camera, sp5C, &sp7C, &sp78, &sp74, (s16) (s32) Player->Direction[1], (s16) (s32) Arg2);
    CheckBump(&Camera->Bump, 5.0f, sp7C, sp78, sp74);
    Camera->LookatPos[0] = sp5C[0];
    Camera->LookatPos[1] = sp5C[1];
    Camera->LookatPos[2] = sp5C[2];
    Camera->CameraPos[0] = sp7C;
    Camera->CameraPos[1] = sp78;
    Camera->CameraPos[2] = sp74;
    temp_f12 = Camera->LookatPos[0] - Camera->CameraPos[0];
    sp88 = Camera->LookatPos[1] - Camera->CameraPos[1];
    temp_f14 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(temp_f12, temp_f14);
    Camera->CameraDirection[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp88);
    Camera->CameraDirection[2] = 0;
}

void Battleview(Camera* Camera, Player* Player, s8 Arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    UNUSED s32 Pad2;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED s32 Pad3[3];
    Vec3f sp64;
    UNUSED s32 pad4[2];
    s32 sp58;
    UNUSED s16 pad5[4];
    s16 var_a3;
    UNUSED s16 pad6;
    s16 temp;

    if ((Player->SlipFlag & DRIFT) == DRIFT) {
        var_a3 = 100;
        if (Player->Sterrangle == 0) {
            Camera->Stickdeg = 0;
        } else {
            if (Player->Sterrangle < 0) {
                var_a3 = 0xA5 - (Player->Sterrangle / 2);

                if ((Player->SlipFlag & STOPSTERR) == STOPSTERR) {
                    ChaseSnumber(&Camera->Stickdeg, -0x0B60, 0.1f);
                } else {
                    ChaseSnumber(&Camera->Stickdeg, -DEGREES(12), 0.1f);
                }
            } else {
                var_a3 = (Player->Sterrangle / 2) + 0xA5;
                if ((Player->SlipFlag & STOPSTERR) == STOPSTERR) {
                    ChaseSnumber(&Camera->Stickdeg, 0x0B60, 0.1f);
                } else {
                    ChaseSnumber(&Camera->Stickdeg, DEGREES(12), 0.1f);
                }
            }
        }
    } else {
        ChaseSnumber(&Camera->Stickdeg, 0, 0.05f);
        var_a3 = ((s16) Camera->ChaseDirection / DEGREES(1)) - ((s16) Player->Direction[1] / DEGREES(1));
        if (Player->Sterrangle == 0) {
            if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
                var_a3 = 0x02D8;
            } else {
                var_a3 = 0x01F4;
            }
        } else if (Player->Sterrangle < 0) {
            if ((var_a3 <= -70) || (var_a3 >= 70)) {
                var_a3 = 0xB4 - Player->Sterrangle;
            } else {
                var_a3 = 0xA5 - (Player->Sterrangle / 2);
            }
        } else if ((var_a3 <= -70) || (var_a3 >= 0x46)) {
            var_a3 = Player->Sterrangle + 0xB4;
        } else {
            var_a3 = (Player->Sterrangle / 2) + 0xA5;
        }
    }
    if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
        ((Player->SlipFlag & SPIN_R) == SPIN_R) ||
        ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) ||
        ((Player->SlipFlag & WING) == WING) ||
        ((Player->SlipFlag & STORM) == STORM) ||
        (((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) && (Player->Sterrangle != 0)) ||
        (Player->Bump.DistanceXY <= 0.0f) || (Player->Bump.DistanceYZ <= 0.0f) ||
        ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN)) {
        DirectionBattlecamera(Player, Camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) Camera->ChaseDirection, (s32) Arg2);
    } else {
        ChaseDir(&Camera->ChaseDirection, (s16) (Player->Direction[1] + Camera->Stickdeg), var_a3);
        DirectionBattlecamera(Player, Camera, sp64, &sp84, &sp80, &sp7C, &sp58, (s32) Camera->ChaseDirection, (s32) Arg2);
    }
    temp = 3;
    Camera->Flag &= 0xFFFB;
    CheckBump(&Camera->Bump, temp, sp84, sp80, sp7C);

    Camera->CameraPos[0] = sp84;
    Camera->CameraPos[1] = sp80;
    Camera->CameraPos[2] = sp7C;

    Camera->LookatPos[0] = sp64[0];
    Camera->LookatPos[1] = sp64[1];
    Camera->LookatPos[2] = sp64[2];

    temp_f12 = Camera->LookatPos[0] - Camera->CameraPos[0];
    sp90 = Camera->LookatPos[1] - Camera->CameraPos[1];
    temp_f14 = Camera->LookatPos[2] - Camera->CameraPos[2];

    Camera->CameraDirection[1] = Atan2t(temp_f12, temp_f14);
    Camera->CameraDirection[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    Camera->CameraDirection[2] = 0;
}

void CameraControl(Player* Player, Camera* Camera, s8 Index) {
    s32 cameraIndex;

    if (FlyCamToggle > 0) {
        return;
    }
    if (Camera == Camera1) {
        cameraIndex = 0;
    }
    if (Camera == Camera2) {
        cameraIndex = 1;
    }
    if (Camera == Camera3) {
        cameraIndex = 2;
    }
    if (Camera == Camera4) {
        cameraIndex = 3;
    }
    switch (GameMode) {
        case GRAND_PRIX:
            // clang-format off
            if (((Player->Flag & IS_RACE_FINISH) == IS_RACE_FINISH) || (DemoFlag == 1)) { CameraMode[cameraIndex] = 3;
            //             -->                 -->        Scroll Right        -->      bit more     -->     ^ Required for matching
                // clang-format on
            } else if (PauseFlag == 1) {
                OgaGameCameraAll(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
            } else {
                OgaGameCamera(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
            }
            break;
        case BATTLE:
            if ((DemoFlag == 1) || ((FinishZoomFlag == 2) && (D_80164A89 == 1))) {
                if (D_80164A88 == 0) {
                    OgaBattleCameraInit();
                }
                D_80164A88 = 1;
                CameraMode[0] = 3;
                CameraMode[1] = 3;
                CameraMode[2] = 3;
                CameraMode[3] = 3;
            } else {
                D_80164A88 = 0;
                if (PauseFlag == 1) {
                    OgaGameCameraAll(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
                } else {
                    OgaGameCamera(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
                }
                CameraMode[cameraIndex] = 9;
            }
            break;
        case TIME_TRIALS:
            if (((PlayerOne->Flag & IS_RACE_FINISH) == IS_RACE_FINISH) || (DemoFlag == 1)) {
                CameraMode[0] = 3;
                CameraMode[1] = 3;
                CameraMode[2] = 3;
                CameraMode[3] = 3;
            } else {
                if (PauseFlag == 1) {
                    OgaGameCameraAll(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
                } else {
                    OgaGameCamera(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
                }
                CameraMode[cameraIndex] = 1;
            }
            break;
        case VERSUS:
            if (((Player->Flag & IS_RACE_FINISH) == IS_RACE_FINISH) || (DemoFlag == 1) ||
                (FinishZoomFlag == 2)) {
                CameraMode[cameraIndex] = 3;
            } else {
                if (PauseFlag == 1) {
                    OgaGameCameraAll(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
                } else {
                    OgaGameCamera(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
                }
                CameraMode[cameraIndex] = 1;
            }
            break;
    }
    if (PauseFlag == 0) {
        switch (CameraMode[cameraIndex]) {
            case 3:
                Func8001A588(&CameraMode[cameraIndex], Camera, Player, Index, cameraIndex);
                break;
            case 1:
                if (((Player->JugemuFlag & IS_IN_WATER) == IS_IN_WATER) ||
                    ((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
                    LookupView(Camera, Player, Index);
                    break;
                }
                KartView(Camera, Player, Index);
                break;
            case 8:
                Openingview(Camera, Player, Index);
                ChangeOpneview(cameraIndex);
                break;
            case 9:
                if (((Player->JugemuFlag & IS_IN_WATER) == IS_IN_WATER) ||
                    ((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
                    LookupView(Camera, Player, Index);
                    break;
                }
                Battleview(Camera, Player, Index);
                break;
        }
    }
}

void MakeViewAng(Player* Player, f32* arg1) {
    f32 var_f0;
    UNUSED s32 pad;
    s32 playerIndex;
    UNUSED s32 Pad2;
    Camera* Camera = &Cameras[0];

    if (Player == PlayerOne) {
        playerIndex = 0;
    }
    if (Player == PlayerTwo) {
        playerIndex = 1;
    }
    if (Player == PlayerThree) {
        playerIndex = 2;
    }
    if (Player == PlayerFour) {
        playerIndex = 3;
    }

    if (D_80164A08[playerIndex] == 0) {
        if (Player->Weapon & USE_BANANA) {
            D_80164A08[playerIndex] = 1;
        }
        if ((Player->SlipFlag & TURBO) == TURBO) {
            D_80164A08[playerIndex] = 2;
        }
        if ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP) {
            D_80164A08[playerIndex] = 3;
        }
        if ((Player->Weapon & PRESS_SQUISH) == PRESS_SQUISH) {
            D_80164A08[playerIndex] = 4;
        }
        if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
            ((Player->SlipFlag & SPIN_R) == SPIN_R)) {
            D_80164A08[playerIndex] = 5;
        }
        CamOffset[playerIndex] = 0.0f;
    }
    switch (D_80164A08[playerIndex]) {
        case 1:
            if (Player->Weapon & USE_BANANA) {
                ChaseFnumber(&CamOffset[playerIndex], 20.0f, 0.2f);
            } else {
                if (CamOffset[playerIndex] > 1.0f) {
                    CamOffset[playerIndex] -= 1.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    CamOffset[playerIndex] = 0.0f;
                }
            }
            break;
        case 2:
            if ((Player->SlipFlag & TURBO) == TURBO) {
                if (Player->TurboTimer != 0) {
                    ChaseFnumber(&CamOffset[playerIndex], 8.0f, 0.2f);
                }
            } else {
                if (CamOffset[playerIndex] > 1.0f) {
                    CamOffset[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    CamOffset[playerIndex] = 0.0f;
                }
            }
            break;
        case 3:
            if (((Player->SlipFlag & DASH_JUMP) == DASH_JUMP) &&
                ((Player->SlipFlag & N_JUMP) == N_JUMP)) {
                ChaseFnumber(&CamOffset[playerIndex], 20.0f, 0.1f);
            } else {
                if (CamOffset[playerIndex] > 1.0f) {
                    CamOffset[playerIndex] -= 1.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    CamOffset[playerIndex] = 0.0f;
                }
            }
            break;
        case 4:
            if ((Player->Weapon & PRESS_SQUISH) == PRESS_SQUISH) {
                ChaseFnumber(&CamOffset[playerIndex], 25.0f, 1.0f);
            } else {
                if (CamOffset[playerIndex] > 1.0f) {
                    CamOffset[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    CamOffset[playerIndex] = 0.0f;
                }
            }
            break;
        case 5:
            if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
                ((Player->SlipFlag & SPIN_R) == SPIN_R)) {
                ChaseFnumber(&CamOffset[playerIndex], 18.0f, 0.2f);
            } else {
                if (CamOffset[playerIndex] > 1.0f) {
                    CamOffset[playerIndex] -= 2.0f;
                } else {
                    D_80164A08[playerIndex] = 0;
                    CamOffset[playerIndex] = 0.0f;
                }
            }
            break;
    }
    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            if (OpeningFlag == 1) {
                CamOffset[playerIndex] = 40.0f;
            }
            if (OpeningFlag == 2) {
                if (CamOffset[playerIndex] >= 0.0f) {
                    CamOffset[playerIndex] -= 0.8;
                }
                if (CamOffset[playerIndex] <= 0.0f) {
                    OpeningFlag = 0;
                    CamOffset[playerIndex] = 0.0f;
                }
            }
            var_f0 = OgawaGakaku(*arg1, playerIndex);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            var_f0 = OgawaGakaku(*arg1, playerIndex);
            break;
    }
    *arg1 = var_f0;
    Camera += playerIndex; // In 500 words or less, please explain why?
    Camera->ScreenViewAngle = var_f0;
}

void ChangeOpneview(s32 CameraId) {
    s32 playerIndex;
    // Why?
    s32 id = CameraId;

    if (PlayerOne) {}
    if (ScreenSplitA == SCREEN_MODE_1P) {
        if (GameMode == GRAND_PRIX) {
            for (playerIndex = 0; playerIndex < NUM_PLAYERS; playerIndex++) {
                if ((PlayerOne[playerIndex].Flag & IS_GP_OPENING) ||
                    (PlayerOne[playerIndex].Flag & PLAYER_UNKNOWN_0x80)) {
                    break;
                }
                if (playerIndex == 7) {
                    D_80164A2C += 1;
                }
                if ((playerIndex == 7) && (D_80164A2C == 0x0000003C)) {
                    OpeningFlag = 2;
                    CameraMode[id] = 1;
                    Cameras[id].CameraDirection[1] = PlayerOne[playerIndex].Direction[1];
                    Cameras[id].ChaseDirection = PlayerOne[playerIndex].Direction[1];
                }
            }
        }
    }
}
