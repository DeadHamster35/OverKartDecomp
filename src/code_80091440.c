#include <actor_types.h>
#include "main.h"
#include "code_800029B0.h"
#include "code_80091440.h"
#include "camera.h"
#include "math_util.h"
#include "collision.h"

void inc_camerapoint(void) {
    camera_point = gPlayerOneCopy->point + 7;
    if ((s32) point_number < camera_point) {
        camera_point -= point_number;
    }
}

UNUSED void finish_camera(void) {
    UNUSED Vec3f sp64 = { 0.0f, -20.0f, 150.0f };
    UNUSED Vec3f sp58 = { 0.0f, -6.0f, 4.0f };
    uintptr_t segment = SEGMENT_NUMBER2(D_8015F718[0]);
    uintptr_t offset = SEGMENT_OFFSET(D_8015F718[0]);
    Camera* camera = &cameras[0];
    struct ActorSpawnData* sp48 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    struct ActorSpawnData* tempData;

    s16 temp3 = (s16) camera_point;
    s16 temp2 = (s16) gPlayerOneCopy->point;
    s16 temp;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    camera->up_vector[0] = 0;
    camera->up_vector[1] = 1;
    camera->up_vector[2] = 0;

    if (1) {} // waa?

    temp = temp2 - temp3;
    if (temp == 7) {
        inc_camerapoint();
    } else if (temp < 14) {
        temp += (s16) point_number;
        if (temp == 7) {
            inc_camerapoint();
        }
    }
    tempData = sp48 + camera_point;

    camera->camera_pos[0] = (f32) (tempData->pos[0] + 10);
    camera->camera_pos[1] = (f32) (tempData->pos[1] + 7);
    camera->camera_pos[2] = (f32) (tempData->pos[2] - 20);
    camera->lookat_pos[0] = gPlayerOneCopy->position[0];

    camera->lookat_pos[1] = gPlayerOneCopy->position[1];
    camera->lookat_pos[2] = gPlayerOneCopy->position[2];
    CheckBump(&camera->bump, 20.0f, camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2]);
    sp38 = camera->lookat_pos[0] - camera->camera_pos[0];
    sp34 = camera->lookat_pos[1] - camera->camera_pos[1];
    sp30 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(sp38, sp30);
    camera->camera_direction[0] = Atan2t(sqrtf((sp38 * sp38) + (sp30 * sp30)), sp34);
    camera->camera_direction[2] = 0;
}
