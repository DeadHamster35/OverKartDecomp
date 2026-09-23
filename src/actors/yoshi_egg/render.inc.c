#include <Camera.h>
#include <actors.h>
#include <defines.h>
#include <Main.h>
#include "courses/yoshi_valley/course_data.h"

/**
 * @brief Renders the Yoshi egg actor.
 * Actor Used in Yoshi Valley.
 *
 * @Param arg0
 * @Param arg1
 * @Param egg
 * @Param Arg3
 */
void DisplayEgg(Camera* arg0, Mat4 arg1, struct YoshiValleyEgg* egg, u16 Arg3) {
    Mat4 sp60;
    Vec3s sp5C;
    Vec3f sp54;
    f32 temp_f0;

    if (GameState != CREDITS_SEQUENCE) {
        temp_f0 = CheckDisplayRange(arg0->CameraPos, egg->Position, arg0->CameraDirection[1], 200.0f, CameraZoom[arg0 - Camera1],
                                          16000000.0f);
        if (temp_f0 < 0.0f) {
            return;
        }
    } else {
        Arg3 = 15;
        temp_f0 = 0.0f;
    }

    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    if ((Arg3 > 12) && (Arg3 < 20)) {
        if (temp_f0 < 640000.0f) {
            sp54[0] = egg->Position[0];
            sp54[1] = 3.0f;
            sp54[2] = egg->Position[2];
            ResetAngle(sp5C);
            DisplayShadow(sp54, sp5C, 10.0f);
        }
        sp5C[0] = 0;
        sp5C[1] = egg->EggRot;
        sp5C[2] = 0;
        CreateModelingMatrix(sp60, egg->Position, sp5C);
        if (SetMatrix(sp60, 0) == 0) {
            return;
        }

        gSPSetGeometryMode(DisplayListHead++, G_LIGHTING);
        gSPDisplayList(DisplayListHead++, d_course_yoshi_valley_dl_16D70);
    } else {
        arg1[3][0] = egg->Position[0];
        arg1[3][1] = egg->Position[1];
        arg1[3][2] = egg->Position[2];

        if (SetMatrix(arg1, 0) != 0) {
            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gSPDisplayList(DisplayListHead++, d_course_yoshi_valley_dl_egg_lod0);
        }
    }
}
