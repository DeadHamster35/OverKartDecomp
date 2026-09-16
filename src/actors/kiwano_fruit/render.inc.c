#include <actors.h>
#include <PR/gbi.h>
#include <main.h>
#include "courses/dks_jungle_parkway/course_data.h"

/**
 * @brief Renders the kiwano fruit actor.
 * Actor used in DK's Jungle Parkway.
 *
 * @param camera
 * @param arg1
 * @param actor
 */
void display_nuts(UNUSED Camera* camera, Mat4 arg1, Object* actor) {
    uintptr_t addr;
    s32 maxObjectsReached;

    if (actor->sparam == 0) {
        return;
    }

    arg1[3][0] = actor->position[0];
    arg1[3][1] = actor->position[1];
    arg1[3][2] = actor->position[2];

    maxObjectsReached = SetMatrix(arg1, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    addr = (actor->angle[0] << 0xA) + 0x03009000; // Can this be a real symbol? Doesn't match.
    gDPLoadTextureBlock(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(addr), G_IM_FMT_CI, G_IM_SIZ_8b, 32, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_kiwano_fruit);
}
