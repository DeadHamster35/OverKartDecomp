#include <actors.h>
#include <code_800029B0.h>
#include <defines.h>
#include <Main.h>

/**
 * @brief Update the banana actor
 *
 * @Param banana
 */
void MoveBanana(struct BananaActor* banana) {
    UNUSED f32 pad;
    Player* Player;
    struct BananaActor* elderBanana;
    struct Controller* controller;
    Vec3f someOtherVelocity;
    Vec3f someVelocity;
    f32 temp_f0;
    UNUSED f32 var_f8;
    UNUSED f32 Pad2;
    UNUSED f32 Pad3;
    UNUSED f32 pad4[2];
    f32 temp_f12;
    f32 temp_f2;
    f32 temp_f14;
    f32 temp_f16;
    f32 unkX;
    f32 unkY;
    f32 unkZ;

    Player = &GamePlayers[banana->Angle[0]];
    switch (banana->Sparam) {
        case HELD_BANANA:
            temp_f2 = Player->Position[0] - banana->Position[0];
            temp_f14 = Player->Position[1] - banana->Position[1];
            temp_f16 = Player->Position[2] - banana->Position[2];
            temp_f12 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16)) / 10.0f;
            if (temp_f12 == 0.0f) {
                banana->Position[0] = Player->Position[0] + 0.2f;
                banana->Position[1] = Player->Position[1] + 0.2f;
                banana->Position[2] = Player->Position[2] + 0.2f;
            } else {
                temp_f2 /= temp_f12;
                temp_f14 /= temp_f12;
                temp_f16 /= temp_f12;
                banana->Position[0] = Player->Position[0] - temp_f2;
                banana->Position[1] = Player->Position[1] - temp_f14 - 2.0f;
                banana->Position[2] = Player->Position[2] - temp_f16;
            }
            CheckBump(&banana->Bump, banana->Radius + 1.0f, banana->Position[0], banana->Position[1],
                                     banana->Position[2]);
            BumpObject((Object*) banana);
            if ((Player->Flag & IS_PLAYER) != 0) {
                if (DemoFlag) {
                    controller = ControllerOne;
                } else {
                    controller = &Controllers[banana->Angle[0]];
                }
                if ((controller->ButtonReleased & Z_TRIG) != 0) {
                    controller->ButtonReleased &= ~Z_TRIG;
                    banana->Sparam = DROPPED_BANANA;
                    banana->Counter = 0x00B4;
                    Player->Weapon &= ~USE_BANANA;
                    NAPlyTrgStart(Player - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    Pad3 = controller->AnalogY;
                    if ((Pad3 > 30.0f) && (controller->AnalogX < 10) && (controller->AnalogX >= -9)) {
                        Pad3 = Pad3 - ((f32) 30);
                        Pad3 = (Pad3 / 20.0f) + 0.5f;
                        if (Player->Speed < 2.0f) {
                            temp_f0 = 4.0f;
                        } else {
                            temp_f0 = (Player->Speed * 0.75f) + 3.5f + Pad3;
                        }
                        Vec3FSet(someVelocity, 0, Pad3, temp_f0);
                        MakeAlignVector(someVelocity, Player->Direction[1] + Player->Slipang);
                        banana->Velocity[0] = someVelocity[0];
                        banana->Velocity[1] = someVelocity[1];
                        banana->Velocity[2] = someVelocity[2];
                    } else {
                        banana->Velocity[0] = 0;
                        banana->Velocity[1] = 1.5f;
                        banana->Velocity[2] = 0;
                    }
                }
            }
            break;
        case DROPPED_BANANA:
            if (banana->Counter != 0) {
                banana->Counter -= 1;
                if (banana->Counter == 0) {
                    banana->Flag &= ~MYOBJ;
                }
            }
            banana->Position[0] += banana->Velocity[0];
            banana->Position[2] += banana->Velocity[2];
            if (banana->Velocity[1] > -1.0f) {
                banana->Velocity[1] -= 0.15f;
            }
            banana->Position[1] += banana->Velocity[1];
            if ((banana->Position[2] < (f32) MinimumZ) || ((f32) MaximumZ < banana->Position[2]) ||
                (banana->Position[0] < (f32) MinimumX) || ((f32) MaximumX < banana->Position[0]) ||
                (banana->Position[1] < (f32) MinimumY)) {
                KillObject((Object*) banana);
            } else {
                CheckBump(&banana->Bump, banana->Radius + 1.0f, banana->Position[0], banana->Position[1],
                                         banana->Position[2]);
                banana->Bump.FlagZX = 1;
                if ((banana->Bump.FlagZX != 0) && (banana->Bump.DistanceZX < 0.0f)) {
                    someOtherVelocity[0] = -banana->Bump.BumpZX[0];
                    someOtherVelocity[1] = -banana->Bump.BumpZX[1];
                    someOtherVelocity[2] = -banana->Bump.BumpZX[2];
                    banana->Position[0] += someOtherVelocity[0] * banana->Bump.DistanceZX;
                    banana->Position[1] += someOtherVelocity[1] * banana->Bump.DistanceZX;
                    banana->Position[2] += someOtherVelocity[2] * banana->Bump.DistanceZX;
                    banana->Flag &= ~MYOBJ;
                    banana->Sparam = 4;
                }
            }
            break;
        case FIRST_BANANA_BUNCH_BANANA:
            someVelocity[0] = 0.0f;
            someVelocity[1] = 0.0f;
            someVelocity[2] = -5.0f;
            MakeAlignVector(someVelocity, Player->Direction[1] + Player->Slipang);
            unkX = Player->Position[0] + someVelocity[0];
            unkY = Player->Position[1] + someVelocity[1];
            unkZ = Player->Position[2] + someVelocity[2];
            temp_f2 = unkX - banana->Position[0];
            temp_f14 = unkY - banana->Position[1];
            temp_f16 = unkZ - banana->Position[2];
            temp_f0 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16));
            if (temp_f0 == 0.0f) {
                banana->Position[0] = Player->Position[0] + 0.2f;
                banana->Position[1] = Player->Position[1] + 0.2f;
                banana->Position[2] = Player->Position[2] + 0.2f;
            } else {
                temp_f2 /= temp_f0;
                temp_f14 /= temp_f0;
                temp_f16 /= temp_f0;
                banana->Position[0] = someVelocity[0] + (unkX - temp_f2);
                banana->Position[1] = unkY - temp_f14 - 2.0f;
                banana->Position[2] = unkZ - temp_f16;
            }
            CheckBump(&banana->Bump, banana->Radius + 1.0f, banana->Position[0], banana->Position[1],
                                     banana->Position[2]);
            BumpObject((Object*) banana);
            break;
        case BANANA_BUNCH_BANANA:
            elderBanana = (struct BananaActor*) &SimpleObjectArray[banana->ElderIndex];
            temp_f2 = elderBanana->Position[0] - banana->Position[0];
            temp_f14 = elderBanana->Position[1] - banana->Position[1];
            temp_f16 = elderBanana->Position[2] - banana->Position[2];
            temp_f12 = sqrtf((temp_f2 * temp_f2) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16)) / 5.0f;
            if (temp_f12 == 0.0f) {
                banana->Position[0] = elderBanana->Position[0] + 0.2f;
                banana->Position[1] = elderBanana->Position[1] + 0.2f;
                banana->Position[2] = elderBanana->Position[2] + 0.2f;
            } else {
                temp_f2 /= temp_f12;
                temp_f14 /= temp_f12;
                temp_f16 /= temp_f12;
                banana->Position[0] = elderBanana->Position[0] - temp_f2;
                banana->Position[1] = elderBanana->Position[1] - temp_f14 - 2.0f;
                banana->Position[2] = elderBanana->Position[2] - temp_f16;
            }
            var_f8 = banana->Position[2];
            CheckBump(&banana->Bump, banana->Radius + 1.0f, banana->Position[0], banana->Position[1],
                                     banana->Position[2]);
            BumpObject((Object*) banana);
            break;
        case DESTROYED_BANANA:
            banana->Velocity[1] -= 0.3f;
            if (banana->Velocity[1] < -5.0f) {
                banana->Velocity[1] = -5.0f;
            }
            banana->Position[1] += banana->Velocity[1];
            banana->Angle[0] += DEGREES(2);
            banana->Angle[1] -= DEGREES(8);
            banana->Angle[2] += DEGREES(5);
            banana->Counter -= 1;
            if (banana->Counter == 0) {
                DeleteObjectBuffer((Object*) banana);
            }
            break;
        case BANANA_ON_GROUND:
            banana->Flag |= 0xC000;
            banana->Flag &= ~MYOBJ;
            break;
        default:
            break;
    }
}
