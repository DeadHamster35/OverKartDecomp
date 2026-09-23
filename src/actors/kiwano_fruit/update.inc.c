#include <actors.h>
#include <Main.h>
#include <defines.h>
#include <path.h>

/**
 * @brief Updates the kiwano fruit actor.
 * Actor Used in DK's Jungle Parkway.
 *
 * @Param fruit
 */
void NutsStrategy(struct KiwanoFruit* fruit) {
    Player* Player;
    f32 temp_f2;
    f32 temp_f16;
    f32 temp_f14;
    f32 temp_f12;
    s32 nearestPathPoint;

    Player = &GamePlayers[fruit->TargetPlayer];
    if (((Player->Flag & IS_CPU_PLAYER) != 0) || (Player->Bump.FlagZX == 0)) {
        fruit->Sparam = 0;
        return;
    }
    switch (fruit->Sparam) { /* irregular */
        case 0:
            if ((CheckStatus(Player->Bump.LastZX) & 0xFF) != GRASS) {
                return;
            }
            fruit->Sparam = 1;
            fruit->Velocity[0] = 80.0f;
        case 1:
            nearestPathPoint = NearestPathPointByPlayerId[(u16) (Player - PlayerOne)];
            temp_f2 = Player->Position[0] - PathPointPointer[nearestPathPoint].Pointx;
            temp_f16 = Player->Position[1] - PathPointPointer[nearestPathPoint].Pointy;
            temp_f14 = Player->Position[2] - PathPointPointer[nearestPathPoint].Pointz;
            temp_f12 = fruit->Velocity[0] / sqrtf((temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f14 * temp_f14));
            temp_f2 *= temp_f12;
            temp_f16 *= temp_f12;
            temp_f14 *= temp_f12;
            fruit->Position[0] = Player->Position[0] + temp_f2;
            fruit->Position[1] = Player->Position[1] + temp_f16;
            fruit->Position[2] = Player->Position[2] + temp_f14;
            fruit->Velocity[0] -= 2.0f;
            if (fruit->Velocity[0] <= 0.0f) {
                fruit->Sparam = 2;
                fruit->BonkTimer = 30.0f;
                fruit->Velocity[0] = 0.0f;
                fruit->Velocity[1] = 2.3f;
                fruit->Velocity[2] = 0.0f;
                if ((Player->SlipFlag & STAR) != 0) {
                    NAPlyTrgStart(Player - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0xA0, 0x52));
                } else {
                    Player->SlipFlag |= CARHIT;
                    Player->Position[0] -= temp_f2 * 4.0f;
                    Player->Position[2] -= temp_f14 * 4.0f;
                    Player->Velocity[0] -= temp_f2 * 0.7f;
                    Player->Velocity[2] -= temp_f14 * 0.7f;
                    NAPlyTrgStart(Player - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x70, 0x18));
                    if (GameMode != GRAND_PRIX) {
                        LostGhost = 1;
                    }
                }
            }
            break;
        case 2:
            fruit->Velocity[1] -= 0.3f;
            fruit->Position[0] += fruit->Velocity[0];
            fruit->Position[1] += fruit->Velocity[1];
            fruit->Position[2] += fruit->Velocity[2];
            fruit->BonkTimer -= 1.0f;
            if (fruit->BonkTimer < 0.0f) {
                fruit->Sparam = 0;
            }
            break;
        default:
            break;
    }
    if (fruit->Sparam != 0) {
        fruit->AnimTimer += 1;
        if (fruit->AnimTimer == 8) {
            fruit->AnimTimer = 0;
            fruit->AnimState += 1;
            if (fruit->AnimState == 3) {
                fruit->AnimState = 0;
            }
        }
    }
}
