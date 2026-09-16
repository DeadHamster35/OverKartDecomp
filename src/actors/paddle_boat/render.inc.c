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
 * Actor used in DK's Jungle Parkway.
 *
 * @param arg0
 * @param boat
 * @param arg2
 * @param pathCounter
 */
void display_ship(Camera* arg0, struct PaddleWheelBoat* boat, UNUSED Mat4 arg2, u16 pathCounter) {
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
        CheckDisplayRange(arg0->camera_pos, boat->position, arg0->camera_direction[1], 90000.0f, gCameraZoom[arg0 - camera1], 9000000.0f);

    if (temp < 0.0f) {
        return;
    }

    gSPSetLights1(gDisplayListHead++, D_800DC610[1]);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_LIGHTING | G_SHADING_SMOOTH);

    CreateModelingMatrix(spE0, boat->position, boat->boatRot);
    if (SetMatrix(spE0, 1) != 0) {

        // Render the boat
        gSPDisplayList(gDisplayListHead++, &d_course_dks_jungle_parkway_boat_dl);
        gSPDisplayList(gDisplayListHead++, &d_course_dks_jungle_parkway_railings_dl);

        CreateMtxRotateX(spE0, boat->wheelRot);
        vec3f_set(sp120, 0, 16.0f, -255.0f);
        CreateTransAffineMtx(spA0, sp120);
        MultiAffineMtx(sp60, spE0, spA0);
        if (SetMatrix(sp60, 3) != 0) {
            // Render the paddle wheel
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gSPDisplayList(gDisplayListHead++, &d_course_dks_jungle_parkway_paddle_wheel_dl);
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
        }
    }
}
