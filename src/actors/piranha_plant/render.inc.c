#include <actors.h>
#include <Main.h>
#include <PR/gbi.h>
#include "courses/mario_raceway/course_data.h"
#include "courses/royal_raceway/course_data.h"

/**
 * @brief Renders the piranha plant actor.
 * Actor Used in Mario Raceway and Royal Raceway.
 *
 * @Param arg0
 * @Param arg1
 * @Param Arg2
 */
void DisplayPakkun(Camera* arg0, Mat4 arg1, PakkunObject* Arg2) {
    UNUSED s32 pad;
    u8* Addr;
    s16 temp_lo = arg0 - Camera1;
    s16 animationFrame; // unconfirmed
    s16 temp = Arg2->Flag;
    f32 temp_f0;
    s32 maxObjectsReached;

    if (temp & 0x800) {
        return;
    }

    temp_f0 = CheckDisplayRange(arg0->CameraPos, Arg2->Position, arg0->CameraDirection[1], 0, CameraZoom[arg0 - Camera1], 1000000.0f);

    if (temp_f0 < 0.0f) {

        switch (temp_lo) {
            case 0:
                Arg2->VisibilityStates[0] = -1;
                break;
            case 1:
                Arg2->VisibilityStates[1] = -1;
                break;
            case 2:
                Arg2->VisibilityStates[2] = -1;
                break;
            case 3:
                Arg2->VisibilityStates[3] = -1;
                break;
        }
        return;
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];
    maxObjectsReached = SetMatrix(arg1, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (90000.0f < temp_f0) {

        switch (temp_lo) {
            case 0:
                Arg2->VisibilityStates[0] = 0;
                break;
            case 1:
                Arg2->VisibilityStates[1] = 0;
                break;
            case 2:
                Arg2->VisibilityStates[2] = 0;
                break;
            case 3:
                Arg2->VisibilityStates[3] = 0;
                break;
        }
        animationFrame = 0;

    } else {
        switch (temp_lo) {
            case 0:
                Arg2->VisibilityStates[0] = 1;
                break;
            case 1:
                Arg2->VisibilityStates[1] = 1;
                break;
            case 2:
                Arg2->VisibilityStates[2] = 1;
                break;
            case 3:
                Arg2->VisibilityStates[3] = 1;
                break;
        }

        switch (temp_lo) {
            case 0:
                animationFrame = Arg2->Timers[0];
                break;
            case 1:
                animationFrame = Arg2->Timers[1];
                break;
            case 2:
                animationFrame = Arg2->Timers[2];
                break;
            case 3:
                animationFrame = Arg2->Timers[3];
                break;
        }
    }
    animationFrame /= 6;

    if (animationFrame > 8) {
        animationFrame = 8;
    }
    Addr = D_802BA058 + (animationFrame << 0xB);
    gDPLoadTextureBlock(DisplayListHead++, VIRTUAL_TO_PHYSICAL(Addr), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 64, 0,
                        G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);

    if (CourseID == COURSE_MARIO_RACEWAY) {
        gSPDisplayList(DisplayListHead++, &d_course_mario_raceway_dl_piranha_plant);
    } else {
        gSPDisplayList(DisplayListHead++, &d_course_royal_raceway_dl_piranha_plant);
    }
}
