#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include "courses/dks_jungle_parkway/course_data.h"

/**
 * @brief Renders the kiwano fruit actor.
 * Actor Used in DK's Jungle Parkway.
 *
 * @Param Camera
 * @Param arg1
 * @Param actor
 */
void DisplayNuts(UNUSED Camera* Camera, Mat4 arg1, Object* actor) {
    uintptr_t Addr;
    s32 maxObjectsReached;

    if (actor->Sparam == 0) {
        return;
    }

    arg1[3][0] = actor->Position[0];
    arg1[3][1] = actor->Position[1];
    arg1[3][2] = actor->Position[2];

    maxObjectsReached = SetMatrix(arg1, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    Addr = (actor->Angle[0] << 0xA) + 0x03009000; // Can this be a real symbol? Doesn't match.
    gDPLoadTextureBlock(DisplayListHead++, VIRTUAL_TO_PHYSICAL(Addr), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPDisplayList(DisplayListHead++, d_course_dks_jungle_parkway_dl_kiwano_fruit);
}
