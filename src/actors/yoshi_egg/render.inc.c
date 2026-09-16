#include <camera.h>
#include <actors.h>
#include <defines.h>
#include <main.h>
#include "courses/yoshi_valley/course_data.h"

/**
 * @brief Renders the Yoshi egg actor.
 * Actor used in Yoshi Valley.
 *
 * @param arg0
 * @param arg1
 * @param egg
 * @param arg3
 */
void display_egg(Camera* arg0, Mat4 arg1, struct YoshiValleyEgg* egg, u16 arg3) {
    Mat4 sp60;
    Vec3s sp5C;
    Vec3f sp54;
    f32 temp_f0;

    if (gGamestate != CREDITS_SEQUENCE) {
        temp_f0 = CheckDisplayRange(arg0->camera_pos, egg->position, arg0->camera_direction[1], 200.0f, gCameraZoom[arg0 - camera1],
                                          16000000.0f);
        if (temp_f0 < 0.0f) {
            return;
        }
    } else {
        arg3 = 15;
        temp_f0 = 0.0f;
    }

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    if ((arg3 > 12) && (arg3 < 20)) {
        if (temp_f0 < 640000.0f) {
            sp54[0] = egg->position[0];
            sp54[1] = 3.0f;
            sp54[2] = egg->position[2];
            reset_angle(sp5C);
            DisplayShadow(sp54, sp5C, 10.0f);
        }
        sp5C[0] = 0;
        sp5C[1] = egg->eggRot;
        sp5C[2] = 0;
        CreateModelingMatrix(sp60, egg->position, sp5C);
        if (SetMatrix(sp60, 0) == 0) {
            return;
        }

        gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
        gSPDisplayList(gDisplayListHead++, d_course_yoshi_valley_dl_16D70);
    } else {
        arg1[3][0] = egg->position[0];
        arg1[3][1] = egg->position[1];
        arg1[3][2] = egg->position[2];

        if (SetMatrix(arg1, 0) != 0) {
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPDisplayList(gDisplayListHead++, d_course_yoshi_valley_dl_egg_lod0);
        }
    }
}
