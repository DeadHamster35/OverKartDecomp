#include <actors.h>
#include <PR/gbi.h>
#include <main.h>
#include <defines.h>
#include "courses/toads_turnpike/course_data.h"

/**
 * @brief Renders the box truck actor.
 * Actor used in Toad's Turnpike.
 * His update are made in vehicle.
 *
 * @param arg0
 * @param arg1
 */
void display_truck1(Camera* arg0, Object* arg1) {
    UNUSED s32 pad[6];
    Mat4 spD8;
    UNUSED s32 pad2[32];
    f32 temp_f0 =
        CheckDisplayRange(arg0->camera_pos, arg1->position, arg0->camera_direction[1], 2500.0f, gCameraZoom[arg0 - camera1], 9000000.0f);
    if (temp_f0 < 0.0f) {
        return;
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(spD8, arg1->position, arg1->angle);
    if (SetMatrix(spD8, 0) != 0) {

        switch (arg1->sparam) {
            case 0:
                gSPDisplayList(gDisplayListHead++, &d_course_toads_turnpike_dl_23858);
                break;
            case 1:
                gSPDisplayList(gDisplayListHead++, &d_course_toads_turnpike_dl_238A0);
                break;
            case 2:
                gSPDisplayList(gDisplayListHead++, &d_course_toads_turnpike_dl_238E8);
                break;
        }

        if (g_ScreenSplitA == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {
                gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_0);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_1);
            } else {
                gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_2);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_1);
        } else {
            gSPDisplayList(gDisplayListHead++, &toads_turnpike_dl_2);
        }
    }
}
