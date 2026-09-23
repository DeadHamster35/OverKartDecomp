#include <actors.h>
#include <Main.h>
#include "courses/choco_mountain/course_data.h"

/**
 * @brief Renders the falling rock actor.
 * Actor Used in Choco Mountain.
 *
 * @Param Camera
 * @Param rock
 */
void DisplayIwa(Camera* Camera, struct FallingRock* rock) {
    Vec3s sp98;
    Vec3f sp8C;
    Mat4 sp4C;
    f32 Height;
    UNUSED s32 pad[4];

    if (rock->RespawnTimer != 0) {
        return;
    }

    Height = CheckDisplayRange(Camera->CameraPos, rock->Position, Camera->CameraDirection[1], 400.0f, CameraZoom[Camera - Camera1],
                                     4000000.0f);

    if (Height < 0.0f) {
        return;
    }

    if (Height < 250000.0f) {

        if (rock->Bump.FlagZX == 1) {
            sp8C[0] = rock->Position[0];
            sp8C[2] = rock->Position[2];
            Height = CalcHeight(sp8C[0], rock->Position[1], sp8C[2], rock->Bump.LastZX);
            sp98[0] = 0;
            sp98[1] = 0;
            sp98[2] = 0;
            sp8C[1] = Height + 2.0f;
            CreateModelingMatrix(sp4C, sp8C, sp98);
            if (SetMatrix(sp4C, 0) == 0) {
                return;
            }
            gSPDisplayList(DisplayListHead++, d_course_choco_mountain_dl_6F88);
        }
    }
    CreateModelingMatrix(sp4C, rock->Position, rock->Angle);
    if (SetMatrix(sp4C, 0) == 0) {
        return;
    }
    gSPDisplayList(DisplayListHead++, d_course_choco_mountain_dl_falling_rock);
}
