#include <actors.h>
#include <Main.h>
#include "courses/koopa_troopa_beach/course_data.h"

/**
 * @brief Renders the palm tree actor.
 * Actor Used in Koopa Troopa Beach.
 *
 * @Param arg0
 * @Param arg1
 * @Param Arg2
 */
void DisplayPalmtree(Camera* arg0, UNUSED Mat4 arg1, struct PalmTree* Arg2) {
    Vec3s spA8 = { 0, 0, 0 };
    Mat4 sp68;
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800)) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(arg0->CameraPos, Arg2->Position, arg0->CameraDirection[1], 0.0f, CameraZoom[arg0 - Camera1], 4000000.0f);

    if (!(temp_f0 < 0.0f)) {
        if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
            DisplayShadow(Arg2->Position, Arg2->Angle, 2.0f);
        }
        CreateModelingMatrix(sp68, Arg2->Position, spA8);
        if (SetMatrix(sp68, 0) != 0) {

            gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
            gSPSetGeometryMode(DisplayListHead++, G_LIGHTING);

            switch (Arg2->Variant) {
                case 0:
                    gSPDisplayList(DisplayListHead++, &d_course_koopa_troopa_beach_dl_tree_trunk1);
                    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
                    gSPDisplayList(DisplayListHead++, &d_course_koopa_troopa_beach_dl_tree_top1);
                    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
                    break;

                case 1:
                    gSPDisplayList(DisplayListHead++, &d_course_koopa_troopa_beach_dl_tree_trunk2);
                    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
                    gSPDisplayList(DisplayListHead++, &d_course_koopa_troopa_beach_dl_tree_top2);
                    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
                    break;

                case 2:
                    gSPDisplayList(DisplayListHead++, &d_course_koopa_troopa_beach_dl_tree_trunk3);
                    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
                    gSPDisplayList(DisplayListHead++, &d_course_koopa_troopa_beach_dl_tree_top3);
                    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
                    break;
            }
        }
    }
}
