#include <actors.h>
#include <main.h>
#include "courses/mario_raceway/course_data.h"

/**
 * @brief Renders the Mario sign actor.
 * Actor used in Mario Raceway.
 *
 * @param arg0
 * @param arg1
 * @param arg2
 */
void display_mkanban(Camera* arg0, UNUSED Mat4 arg1, Object* arg2) {
    Mat4 sp40;
    f32 unk;
    s16 temp = arg2->flag;

    if (temp & 0x800) {
        return;
    }

    unk = CheckDisplayRange(arg0->camera_pos, arg2->position, arg0->camera_direction[1], 0, gCameraZoom[arg0 - camera1], 16000000.0f);
    if (!(unk < 0.0f)) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        CreateModelingMatrix(sp40, arg2->position, arg2->angle);
        if (SetMatrix(sp40, 0) != 0) {
            gSPDisplayList(gDisplayListHead++, d_course_mario_raceway_dl_sign);
        }
    }
}
