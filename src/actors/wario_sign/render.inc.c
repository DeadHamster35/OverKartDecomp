#include <actors.h>
#include <PR/gbi.h>
#include <main.h>
#include "courses/wario_stadium/course_data.h"

/**
 * @brief Renders the Wario sign actor.
 * Used in Wario Stadium.
 *
 * @param arg0
 * @param arg1
 */
void display_wkanban(Camera* arg0, Object* arg1) {
    Mat4 sp38;
    f32 unk = CheckDisplayRange(arg0->camera_pos, arg1->position, arg0->camera_direction[1], 0, gCameraZoom[arg0 - camera1], 16000000.0f);

    if (!(unk < 0.0f)) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

        CreateModelingMatrix(sp38, arg1->position, arg1->angle);
        if (SetMatrix(sp38, 0) != 0) {

            gSPDisplayList(gDisplayListHead++, d_course_wario_stadium_dl_sign);
        }
    }
}
