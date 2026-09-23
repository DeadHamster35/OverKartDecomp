#include <actors.h>
#include <Main.h>
#include "courses/mario_raceway/course_data.h"

/**
 * @brief Renders the Mario sign actor.
 * Actor Used in Mario Raceway.
 *
 * @Param arg0
 * @Param arg1
 * @Param Arg2
 */
void DisplayMkanban(Camera* arg0, UNUSED Mat4 arg1, Object* Arg2) {
    Mat4 sp40;
    f32 unk;
    s16 temp = Arg2->Flag;

    if (temp & 0x800) {
        return;
    }

    unk = CheckDisplayRange(arg0->CameraPos, Arg2->Position, arg0->CameraDirection[1], 0, CameraZoom[arg0 - Camera1], 16000000.0f);
    if (!(unk < 0.0f)) {
        gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
        CreateModelingMatrix(sp40, Arg2->Position, Arg2->Angle);
        if (SetMatrix(sp40, 0) != 0) {
            gSPDisplayList(DisplayListHead++, d_course_mario_raceway_dl_sign);
        }
    }
}
