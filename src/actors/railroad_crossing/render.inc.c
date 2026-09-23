#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include "courses/kalimari_desert/course_data.h"

/**
 * @brief Renders the railroad crossing actor.
 * Actor Used in Kalimari Desert.
 *
 * @Param arg0
 * @Param rr_crossing
 */
void DisplayFumikiri(Camera* arg0, struct RailroadCrossing* rr_crossing) {
    UNUSED Vec3s sp80 = { 0, 0, 0 };
    Mat4 sp40;
    f32 unk = CheckDisplayRange(arg0->CameraPos, rr_crossing->Position, arg0->CameraDirection[1], 0.0f, CameraZoom[arg0 - Camera1],
                                      4000000.0f);

    if (!(unk < 0.0f)) {
        CreateModelingMatrix(sp40, rr_crossing->Position, rr_crossing->Angle);

        if (SetMatrix(sp40, 0) != 0) {
            gSPSetGeometryMode(DisplayListHead++, G_LIGHTING);
            gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);

            if (FumikiriMode[rr_crossing->CrossingId]) {

                if (rr_crossing->SomeTimer < 20) {
                    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_crossing_right_active);
                } else {
                    gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_crossing_left_active);
                }
            } else {
                gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_crossing_both_inactive);
            }
            gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
        }
    }
}
