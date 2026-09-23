#include <actor_types.h>
#include "main.h"
#include "code_800029B0.h"
#include "code_80091440.h"
#include "camera.h"
#include "math_util.h"
#include "collision.h"

void IncCamerapoint(void) {
    CameraPoint = PlayerOneCopy->Point + 7;
    if ((s32) PointNumber < CameraPoint) {
        CameraPoint -= PointNumber;
    }
}

UNUSED void FinishCamera(void) {
    UNUSED Vec3f sp64 = { 0.0f, -20.0f, 150.0f };
    UNUSED Vec3f sp58 = { 0.0f, -6.0f, 4.0f };
    uintptr_t segment = SEGMENT_NUMBER2(D_8015F718[0]);
    uintptr_t Offset = SEGMENT_OFFSET(D_8015F718[0]);
    Camera* Camera = &Cameras[0];
    struct ActorSpawnData* sp48 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    struct ActorSpawnData* tempData;

    s16 temp3 = (s16) CameraPoint;
    s16 temp2 = (s16) PlayerOneCopy->Point;
    s16 temp;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    Camera->UpVector[0] = 0;
    Camera->UpVector[1] = 1;
    Camera->UpVector[2] = 0;

    if (1) {} // waa?

    temp = temp2 - temp3;
    if (temp == 7) {
        IncCamerapoint();
    } else if (temp < 14) {
        temp += (s16) PointNumber;
        if (temp == 7) {
            IncCamerapoint();
        }
    }
    tempData = sp48 + CameraPoint;

    Camera->CameraPos[0] = (f32) (tempData->pos[0] + 10);
    Camera->CameraPos[1] = (f32) (tempData->pos[1] + 7);
    Camera->CameraPos[2] = (f32) (tempData->pos[2] - 20);
    Camera->LookatPos[0] = PlayerOneCopy->Position[0];

    Camera->LookatPos[1] = PlayerOneCopy->Position[1];
    Camera->LookatPos[2] = PlayerOneCopy->Position[2];
    CheckBump(&Camera->Bump, 20.0f, Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2]);
    sp38 = Camera->LookatPos[0] - Camera->CameraPos[0];
    sp34 = Camera->LookatPos[1] - Camera->CameraPos[1];
    sp30 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(sp38, sp30);
    Camera->CameraDirection[0] = Atan2t(sqrtf((sp38 * sp38) + (sp30 * sp30)), sp34);
    Camera->CameraDirection[2] = 0;
}
