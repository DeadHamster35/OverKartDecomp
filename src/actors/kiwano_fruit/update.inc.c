#include <actors.h>
#include <main.h>
#include <defines.h>
#include <path.h>

/**
 * @brief Updates the kiwano fruit actor.
 * Actor used in DK's Jungle Parkway.
 *
 * @param fruit
 */
void nuts_strategy(struct KiwanoFruit* fruit) {
    Player* player;
    f32 temp_f2;
    f32 temp_f16;
    f32 temp_f14;
    f32 temp_f12;
    s32 nearestPathPoint;

    player = &gPlayers[fruit->targetPlayer];
    if (((player->flag & IS_CPU_PLAYER) != 0) || (player->bump.flag_zx == 0)) {
        fruit->sparam = 0;
        return;
    }
    switch (fruit->sparam) { /* irregular */
        case 0:
            if ((check_status(player->bump.last_zx) & 0xFF) != GRASS) {
                return;
            }
            fruit->sparam = 1;
            fruit->velocity[0] = 80.0f;
        case 1:
            nearestPathPoint = gNearestPathPointByPlayerId[(u16) (player - gPlayerOne)];
            temp_f2 = player->position[0] - g_pathPointPointer[nearestPathPoint].pointx;
            temp_f16 = player->position[1] - g_pathPointPointer[nearestPathPoint].pointy;
            temp_f14 = player->position[2] - g_pathPointPointer[nearestPathPoint].pointz;
            temp_f12 = fruit->velocity[0] / sqrtf((temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f14 * temp_f14));
            temp_f2 *= temp_f12;
            temp_f16 *= temp_f12;
            temp_f14 *= temp_f12;
            fruit->position[0] = player->position[0] + temp_f2;
            fruit->position[1] = player->position[1] + temp_f16;
            fruit->position[2] = player->position[2] + temp_f14;
            fruit->velocity[0] -= 2.0f;
            if (fruit->velocity[0] <= 0.0f) {
                fruit->sparam = 2;
                fruit->bonkTimer = 30.0f;
                fruit->velocity[0] = 0.0f;
                fruit->velocity[1] = 2.3f;
                fruit->velocity[2] = 0.0f;
                if ((player->slip_flag & STAR) != 0) {
                    NAPlyTrgStart(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x52));
                } else {
                    player->slip_flag |= CARHIT;
                    player->position[0] -= temp_f2 * 4.0f;
                    player->position[2] -= temp_f14 * 4.0f;
                    player->velocity[0] -= temp_f2 * 0.7f;
                    player->velocity[2] -= temp_f14 * 0.7f;
                    NAPlyTrgStart(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
                    if (g_gameMode != GRAND_PRIX) {
                        lost_ghost = 1;
                    }
                }
            }
            break;
        case 2:
            fruit->velocity[1] -= 0.3f;
            fruit->position[0] += fruit->velocity[0];
            fruit->position[1] += fruit->velocity[1];
            fruit->position[2] += fruit->velocity[2];
            fruit->bonkTimer -= 1.0f;
            if (fruit->bonkTimer < 0.0f) {
                fruit->sparam = 0;
            }
            break;
        default:
            break;
    }
    if (fruit->sparam != 0) {
        fruit->animTimer += 1;
        if (fruit->animTimer == 8) {
            fruit->animTimer = 0;
            fruit->animState += 1;
            if (fruit->animState == 3) {
                fruit->animState = 0;
            }
        }
    }
}
