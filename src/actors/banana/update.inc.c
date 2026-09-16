#include <actors.h>
#include <code_800029B0.h>
#include <defines.h>
#include <main.h>

/**
 * @brief Update the banana actor
 *
 * @param banana
 */
void move_banana(struct BananaActor* banana) {
    UNUSED f32 pad;
    Player* player;
    struct BananaActor* elderBanana;
    struct Controller* controller;
    Vec3f someOtherVelocity;
    Vec3f someVelocity;
    f32 temp_f0;
    UNUSED f32 var_f8;
    UNUSED f32 pad2;
    UNUSED f32 pad3;
    UNUSED f32 pad4[2];
    f32 temp_f12;
    f32 temp_f2;
    f32 temp_f14;
    f32 temp_f16;
    f32 unkX;
    f32 unkY;
    f32 unkZ;

    player = &gPlayers[banana->angle[0]];
    switch (banana->sparam) {
        case HELD_BANANA:
            temp_f2 = player->position[0] - banana->position[0];
            temp_f14 = player->position[1] - banana->position[1];
            temp_f16 = player->position[2] - banana->position[2];
            temp_f12 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16)) / 10.0f;
            if (temp_f12 == 0.0f) {
                banana->position[0] = player->position[0] + 0.2f;
                banana->position[1] = player->position[1] + 0.2f;
                banana->position[2] = player->position[2] + 0.2f;
            } else {
                temp_f2 /= temp_f12;
                temp_f14 /= temp_f12;
                temp_f16 /= temp_f12;
                banana->position[0] = player->position[0] - temp_f2;
                banana->position[1] = player->position[1] - temp_f14 - 2.0f;
                banana->position[2] = player->position[2] - temp_f16;
            }
            CheckBump(&banana->bump, banana->radius + 1.0f, banana->position[0], banana->position[1],
                                     banana->position[2]);
            BumpObject((Object*) banana);
            if ((player->flag & IS_PLAYER) != 0) {
                if (g_DemoFlag) {
                    controller = gControllerOne;
                } else {
                    controller = &gControllers[banana->angle[0]];
                }
                if ((controller->ButtonReleased & Z_TRIG) != 0) {
                    controller->ButtonReleased &= ~Z_TRIG;
                    banana->sparam = DROPPED_BANANA;
                    banana->counter = 0x00B4;
                    player->weapon &= ~USE_BANANA;
                    NAPlyTrgStart(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    pad3 = controller->AnalogY;
                    if ((pad3 > 30.0f) && (controller->AnalogX < 10) && (controller->AnalogX >= -9)) {
                        pad3 = pad3 - ((f32) 30);
                        pad3 = (pad3 / 20.0f) + 0.5f;
                        if (player->speed < 2.0f) {
                            temp_f0 = 4.0f;
                        } else {
                            temp_f0 = (player->speed * 0.75f) + 3.5f + pad3;
                        }
                        vec3f_set(someVelocity, 0, pad3, temp_f0);
                        MakeAlignVector(someVelocity, player->direction[1] + player->slipang);
                        banana->velocity[0] = someVelocity[0];
                        banana->velocity[1] = someVelocity[1];
                        banana->velocity[2] = someVelocity[2];
                    } else {
                        banana->velocity[0] = 0;
                        banana->velocity[1] = 1.5f;
                        banana->velocity[2] = 0;
                    }
                }
            }
            break;
        case DROPPED_BANANA:
            if (banana->counter != 0) {
                banana->counter -= 1;
                if (banana->counter == 0) {
                    banana->flag &= ~MYOBJ;
                }
            }
            banana->position[0] += banana->velocity[0];
            banana->position[2] += banana->velocity[2];
            if (banana->velocity[1] > -1.0f) {
                banana->velocity[1] -= 0.15f;
            }
            banana->position[1] += banana->velocity[1];
            if ((banana->position[2] < (f32) minimum_z) || ((f32) maximum_z < banana->position[2]) ||
                (banana->position[0] < (f32) minimum_x) || ((f32) maximum_x < banana->position[0]) ||
                (banana->position[1] < (f32) minimum_y)) {
                KillObject((Object*) banana);
            } else {
                CheckBump(&banana->bump, banana->radius + 1.0f, banana->position[0], banana->position[1],
                                         banana->position[2]);
                banana->bump.flag_zx = 1;
                if ((banana->bump.flag_zx != 0) && (banana->bump.distance_zx < 0.0f)) {
                    someOtherVelocity[0] = -banana->bump.bump_zx[0];
                    someOtherVelocity[1] = -banana->bump.bump_zx[1];
                    someOtherVelocity[2] = -banana->bump.bump_zx[2];
                    banana->position[0] += someOtherVelocity[0] * banana->bump.distance_zx;
                    banana->position[1] += someOtherVelocity[1] * banana->bump.distance_zx;
                    banana->position[2] += someOtherVelocity[2] * banana->bump.distance_zx;
                    banana->flag &= ~MYOBJ;
                    banana->sparam = 4;
                }
            }
            break;
        case FIRST_BANANA_BUNCH_BANANA:
            someVelocity[0] = 0.0f;
            someVelocity[1] = 0.0f;
            someVelocity[2] = -5.0f;
            MakeAlignVector(someVelocity, player->direction[1] + player->slipang);
            unkX = player->position[0] + someVelocity[0];
            unkY = player->position[1] + someVelocity[1];
            unkZ = player->position[2] + someVelocity[2];
            temp_f2 = unkX - banana->position[0];
            temp_f14 = unkY - banana->position[1];
            temp_f16 = unkZ - banana->position[2];
            temp_f0 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16));
            if (temp_f0 == 0.0f) {
                banana->position[0] = player->position[0] + 0.2f;
                banana->position[1] = player->position[1] + 0.2f;
                banana->position[2] = player->position[2] + 0.2f;
            } else {
                temp_f2 /= temp_f0;
                temp_f14 /= temp_f0;
                temp_f16 /= temp_f0;
                banana->position[0] = someVelocity[0] + (unkX - temp_f2);
                banana->position[1] = unkY - temp_f14 - 2.0f;
                banana->position[2] = unkZ - temp_f16;
            }
            CheckBump(&banana->bump, banana->radius + 1.0f, banana->position[0], banana->position[1],
                                     banana->position[2]);
            BumpObject((Object*) banana);
            break;
        case BANANA_BUNCH_BANANA:
            elderBanana = (struct BananaActor*) &g_SimpleObjectArray[banana->elderIndex];
            temp_f2 = elderBanana->position[0] - banana->position[0];
            temp_f14 = elderBanana->position[1] - banana->position[1];
            temp_f16 = elderBanana->position[2] - banana->position[2];
            temp_f12 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16)) / 5.0f;
            if (temp_f12 == 0.0f) {
                banana->position[0] = elderBanana->position[0] + 0.2f;
                banana->position[1] = elderBanana->position[1] + 0.2f;
                banana->position[2] = elderBanana->position[2] + 0.2f;
            } else {
                temp_f2 /= temp_f12;
                temp_f14 /= temp_f12;
                temp_f16 /= temp_f12;
                banana->position[0] = elderBanana->position[0] - temp_f2;
                banana->position[1] = elderBanana->position[1] - temp_f14 - 2.0f;
                banana->position[2] = elderBanana->position[2] - temp_f16;
            }
            var_f8 = banana->position[2];
            CheckBump(&banana->bump, banana->radius + 1.0f, banana->position[0], banana->position[1],
                                     banana->position[2]);
            BumpObject((Object*) banana);
            break;
        case DESTROYED_BANANA:
            banana->velocity[1] -= 0.3f;
            if (banana->velocity[1] < -5.0f) {
                banana->velocity[1] = -5.0f;
            }
            banana->position[1] += banana->velocity[1];
            banana->angle[0] += DEGREES(2);
            banana->angle[1] -= DEGREES(8);
            banana->angle[2] += DEGREES(5);
            banana->counter -= 1;
            if (banana->counter == 0) {
                deleteObjectBuffer((Object*) banana);
            }
            break;
        case BANANA_ON_GROUND:
            banana->flag |= 0xC000;
            banana->flag &= ~MYOBJ;
            break;
        default:
            break;
    }
}
