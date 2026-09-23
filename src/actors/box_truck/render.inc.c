#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include <defines.h>
#include "courses/toads_turnpike/course_data.h"

/**
 * @brief Renders the box truck actor.
 * Actor Used in Toad's Turnpike.
 * His update are made in vehicle.
 *
 * @Param arg0
 * @Param arg1
 */
void DisplayTruck1(Camera* arg0, Object* arg1) {
    UNUSED s32 pad[6];
    Mat4 spD8;
    UNUSED s32 Pad2[32];
    f32 temp_f0 =
        CheckDisplayRange(arg0->CameraPos, arg1->Position, arg0->CameraDirection[1], 2500.0f, CameraZoom[arg0 - Camera1], 9000000.0f);
    if (temp_f0 < 0.0f) {
        return;
    }

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(spD8, arg1->Position, arg1->Angle);
    if (SetMatrix(spD8, 0) != 0) {

        switch (arg1->Sparam) {
            case 0:
                gSPDisplayList(DisplayListHead++, &d_course_toads_turnpike_dl_23858);
                break;
            case 1:
                gSPDisplayList(DisplayListHead++, &d_course_toads_turnpike_dl_238A0);
                break;
            case 2:
                gSPDisplayList(DisplayListHead++, &d_course_toads_turnpike_dl_238E8);
                break;
        }

        if (ScreenSplitA == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_0);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_1);
            } else {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_2);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_1);
        } else {
            gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_2);
        }
    }
}
