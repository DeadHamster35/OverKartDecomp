#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include "courses/kalimari_desert/course_data.h"

/**
 * @brief Renders the train engine actor.
 * Actor Used in Kalimari Desert.
 * His update Position are made in vehicle.
 *
 * @Param Camera
 * @Param actor
 */
void DisplaySl1(Camera* Camera, struct TrainCar* actor) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3f sp160;
    Mat4 sp120;
    Mat4 spE0;
    Mat4 spA0;

    f32 Distance = CheckDisplayRange(Camera->CameraPos, actor->Position, Camera->CameraDirection[1], 2500.0f,
                                           CameraZoom[Camera - Camera1], 9000000.0f);

    if (Distance < 0.0f) {
        return;
    }

    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(sp120, actor->Position, actor->Angle);
    maxObjectsReached = SetMatrix(sp120, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (Distance < 122500.0f) {

        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1C0F0);
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1B978);

    } else if (Distance < 640000.0f) {

        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1D670);
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1D160);
    } else {
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1E910);
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1E480);
    }
    if (1440000.0f < Distance) {
        return;
    }

    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22D28);

    CreateMtxRotateX(sp120, actor->WheelRot);
    Vec3FSet(sp160, 17.0f, 6.0f, 32.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, actor->WheelRot);
    Vec3FSet(sp160, -17.0, 6.0f, 32.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(2)));
    Vec3FSet(sp160, 17.0f, 6.0f, 16.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(2)));
    Vec3FSet(sp160, -17.0f, 6.0f, 16.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(6)));
    Vec3FSet(sp160, 17.0f, 12.0f, -12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(6)));
    Vec3FSet(sp160, -17.0f, 12.0f, -12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(4)));
    Vec3FSet(sp160, 17.0f, 12.0f, -34.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(4)));
    Vec3FSet(sp160, -17.0f, 12.0f, -34.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    maxObjectsReached = SetMatrix(spA0, 3) == 0;
    if (maxObjectsReached) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22D70);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void DisplaySl2(Camera* Camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = CheckDisplayRange(Camera->CameraPos, actor->Position, Camera->CameraDirection[1], 625.0f,
                                          CameraZoom[Camera - Camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(sp120, actor->Position, actor->Angle);
    if (SetMatrix(sp120, 0) == 0) {
        return;
    }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1F228);

    } else if (temp_f0 < 1000000.0f) {
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1F708);
    } else {
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_1FAF8);
    }
    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22D28);

    CreateMtxRotateX(sp120, actor->WheelRot);
    Vec3FSet(sp160, 17.0f, 6.0f, 8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, actor->WheelRot);
    Vec3FSet(sp160, -17.0, 6.0f, 8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(6)));
    Vec3FSet(sp160, 17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(6)));
    Vec3FSet(sp160, -17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }
    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void DisplaySl3(Camera* Camera, struct TrainCar* actor) {
    Mat4 sp120;
    Vec3f sp160;
    Mat4 spE0;
    Mat4 spA0;

    f32 temp_f0 = CheckDisplayRange(Camera->CameraPos, actor->Position, Camera->CameraDirection[1], 2025.0f,
                                          CameraZoom[Camera - Camera1], 9000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(sp120, actor->Position, actor->Angle);

    if (SetMatrix(sp120, 0) == 0) {
        return;
    }

    if (temp_f0 < 250000.0f) {

        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_20A20);
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_20A08);

    } else if (temp_f0 < 1000000.0f) {

        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_21550);
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_21220);
    } else {
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_21C90);
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_21A80);
    }
    if (1440000.0f < temp_f0) {
        return;
    }

    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22D28);

    CreateMtxRotateX(sp120, actor->WheelRot);
    Vec3FSet(sp160, 17.0f, 6.0f, 28.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, actor->WheelRot);
    Vec3FSet(sp160, -17.0, 6.0f, 28.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(3)));
    Vec3FSet(sp160, 17.0f, 6.0f, 12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(3)));
    Vec3FSet(sp160, -17.0f, 6.0f, 12.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(8)));
    Vec3FSet(sp160, 17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(8)));
    Vec3FSet(sp160, -17.0f, 6.0f, -8.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(2)));
    Vec3FSet(sp160, 17.0f, 6.0f, -24.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);

    CreateMtxRotateX(sp120, (s16) (actor->WheelRot + DEGREES(2)));
    Vec3FSet(sp160, -17.0f, 6.0f, -24.0f);
    CreateTransAffineMtx(spE0, sp160);
    MultiAffineMtx(spA0, sp120, spE0);

    if (SetMatrix(spA0, 3) == 0) {
        return;
    }

    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22DB8);
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}
