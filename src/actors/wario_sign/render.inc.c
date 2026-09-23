#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include "courses/wario_stadium/course_data.h"

/**
 * @brief Renders the Wario sign actor.
 * Used in Wario Stadium.
 *
 * @Param arg0
 * @Param arg1
 */
void DisplayWkanban(Camera* arg0, Object* arg1) {
    Mat4 sp38;
    f32 unk = CheckDisplayRange(arg0->CameraPos, arg1->Position, arg0->CameraDirection[1], 0, CameraZoom[arg0 - Camera1], 16000000.0f);

    if (!(unk < 0.0f)) {
        gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

        CreateModelingMatrix(sp38, arg1->Position, arg1->Angle);
        if (SetMatrix(sp38, 0) != 0) {

            gSPDisplayList(DisplayListHead++, d_course_wario_stadium_dl_sign);
        }
    }
}
