#include <actors.h>
#include <PR/gbi.h>
#include <main.h>
#include "courses/kalimari_desert/course_data.h"

/**
 * @brief Renders the train engine actor.
 * Actor used in Kalimari Desert.
 * His update position are made in vehicle.
 *
 * @param camera
 * @param actor
 */
void display_sl1(Camera* camera, struct TrainCar* actor) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3f sp160;
    Mat4 sp120;
    Mat4 spE0;
    Mat4 spA0;

    f32 distance = CheckDisplayRange(camera->camera_pos, actor->position, camera->camera_direction[1], 2500.0f,
                                           gCameraZoom[camera - camera1], 9000000.0f);

    if (distance < 0.0f) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(sp120, actor->position, actor->angle);
    maxObjectsReached = SetMatrix(sp120, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (distance < 122500.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1C0F0);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1B978);

    } else if (distance < 640000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1D670);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1D160);
    } else {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1E910);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1E480);
    }
    if (1440000.0f < distance) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D28);

    CreateMtxRotateX(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 32.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 32.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(2)));
    vec3f_set(sp160, 17.0f, 6.0f, 16.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(2)));
    vec3f_set(sp160, -17.0f, 6.0f, 16.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(6)));
    vec3f_set(sp160, 17.0f, 12.0f, -12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(6)));
    vec3f_set(sp160, -17.0f, 12.0f, -12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(4)));
    vec3f_set(sp160, 17.0f, 12.0f, -34.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(4)));
    vec3f_set(sp160, -17.0f, 12.0f, -34.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void display_sl2(Camera* camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = CheckDisplayRange(camera->camera_pos, actor->position, camera->camera_direction[1], 625.0f,
                                          gCameraZoom[camera - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(sp120, actor->position, actor->angle);
    if (SetMatrix(sp120, 0) == 0) {
        return;
    }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1F228);

    } else if (temp_f0 < 1000000.0f) {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1F708);
    } else {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_1FAF8);
    }
    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D28);

    CreateMtxRotateX(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(6)));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(6)));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void display_sl3(Camera* camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = CheckDisplayRange(camera->camera_pos, actor->position, camera->camera_direction[1], 2025.0f,
                                          gCameraZoom[camera - camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(sp120, actor->position, actor->angle);

    if (SetMatrix(sp120, 0) == 0) {
        return;
    }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_20A20);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_20A08);

    } else if (temp_f0 < 1000000.0f) {

        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21550);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21220);
    } else {
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21C90);
        gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_21A80);
    }
    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22D28);

    CreateMtxRotateX(sp120, actor->wheelRot);
    vec3f_set(sp160, 17.0f, 6.0f, 28.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, actor->wheelRot);
    vec3f_set(sp160, -17.0, 6.0f, 28.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(3)));
    vec3f_set(sp160, 17.0f, 6.0f, 12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(3)));
    vec3f_set(sp160, -17.0f, 6.0f, 12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(8)));
    vec3f_set(sp160, 17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(8)));
    vec3f_set(sp160, -17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(2)));
    vec3f_set(sp160, 17.0f, 6.0f, -24.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->wheelRot + DEGREES(2)));
    vec3f_set(sp160, -17.0f, 6.0f, -24.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}
