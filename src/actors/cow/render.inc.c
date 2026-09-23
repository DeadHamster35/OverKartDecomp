#include <Camera.h>
#include <actors.h>
#include <Main.h>
#include <PR/gbi.h>
#include "courses/moo_moo_farm/course_data.h"

/**
 * @brief Renders the cow actor.
 * Actor Used in Moo Moo Farm.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplayCow(Camera* Camera, Mat4 arg1, Object* Arg2) {
    if (CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1],
                                  4000000.0f) < 0) {
        return;
    }

    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        switch (Arg2->Sparam) {
            case 0:
                gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow1);
                break;
            case 1:
                gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow2);
                break;
            case 2:
                gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow3);
                break;
            case 3:
                gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow4);
                break;
            case 4:
                gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_cow5);
                break;
        }
    }
}
