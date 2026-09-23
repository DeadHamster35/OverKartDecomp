#include <ultra64.h>
#include <macros.h>
#include <actor_types.h>
#include "camera.h"
#include "main.h"
#include "actors.h"
#include "courses/all_course_data.h"
#include <PR/gbi.h>

/**
 * @brief Renders the paddle boat actor.
 * Actor Used in DK's Jungle Parkway.
 *
 * @Param arg0
 * @Param boat
 * @Param Arg2
 * @Param pathCounter
 */
void DisplayShip(Camera* arg0, struct PaddleWheelBoat* boat, UNUSED Mat4 Arg2, u16 pathCounter) {
    UNUSED s32 pad[3];
    Vec3f sp120;
    Mat4 spE0;
    Mat4 spA0;
    Mat4 sp60;
    f32 temp;

    if ((pathCounter > 20) && (pathCounter < 25)) {
        return;
    }

    temp =
        CheckDisplayRange(arg0->CameraPos, boat->Position, arg0->CameraDirection[1], 90000.0f, CameraZoom[arg0 - Camera1], 9000000.0f);

    if (temp < 0.0f) {
        return;
    }

    gSPSetLights1(DisplayListHead++, D_800DC610[1]);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);

    CreateModelingMatrix(spE0, boat->Position, boat->BoatRot);
    if (SetMatrix(spE0, 1) != 0) {

        // Render the boat
        gSPDisplayList(DisplayListHead++, &d_course_dks_jungle_parkway_boat_dl);
        gSPDisplayList(DisplayListHead++, &d_course_dks_jungle_parkway_railings_dl);

        CreateMtxRotateX(spE0, boat->WheelRot);
        Vec3FSet(sp120, 0, 16.0f, -255.0f);
        CreateTransAffineMtx(spA0, sp120);
        MultiAffineMtx(sp60, spE0, spA0);
        if (SetMatrix(sp60, 3) != 0) {
            // Render the paddle wheel
            gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
            gSPDisplayList(DisplayListHead++, &d_course_dks_jungle_parkway_paddle_wheel_dl);
            gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
            gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
        }
    }
}
