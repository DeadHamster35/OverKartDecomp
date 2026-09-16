#include <actors.h>
#include <main.h>
#include "courses/choco_mountain/course_data.h"

/**
 * @brief Renders the falling rock actor.
 * Actor used in Choco Mountain.
 *
 * @param camera
 * @param rock
 */
void display_iwa(Camera* camera, struct FallingRock* rock) {
    Vec3s sp98;
    Vec3f sp8C;
    Mat4 sp4C;
    f32 height;
    UNUSED s32 pad[4];

    if (rock->respawnTimer != 0) {
        return;
    }

    height = CheckDisplayRange(camera->camera_pos, rock->position, camera->camera_direction[1], 400.0f, gCameraZoom[camera - camera1],
                                     4000000.0f);

    if (height < 0.0f) {
        return;
    }

    if (height < 250000.0f) {

        if (rock->bump.flag_zx == 1) {
            sp8C[0] = rock->position[0];
            sp8C[2] = rock->position[2];
            height = CalcHeight(sp8C[0], rock->position[1], sp8C[2], rock->bump.last_zx);
            sp98[0] = 0;
            sp98[1] = 0;
            sp98[2] = 0;
            sp8C[1] = height + 2.0f;
            CreateModelingMatrix(sp4C, sp8C, sp98);
            if (SetMatrix(sp4C, 0) == 0) {
                return;
            }
            gSPDisplayList(gDisplayListHead++, d_course_choco_mountain_dl_6F88);
        }
    }
    CreateModelingMatrix(sp4C, rock->position, rock->angle);
    if (SetMatrix(sp4C, 0) == 0) {
        return;
    }
    gSPDisplayList(gDisplayListHead++, d_course_choco_mountain_dl_falling_rock);
}
