#include <actors.h>
#include <Main.h>
#include <code_800029B0.h>
#include <defines.h>
#include "collision.h"

/**
 * @brief Updates the Green shell actor.
 *
 * @Param shell
 */
void MoveGshell(struct ShellActor* shell) {
    Player* Player;
    UNUSED f32 pad9;
    UNUSED f32 PadA;
    Vec3f somePos2;
    Vec3f somePosVel;
    f32 var_f2;
    struct Controller* controller;
    TripleShellParent* parent;
    f32 Height;
    f32 z;
    UNUSED f32 Pad2;
    UNUSED f32 Pad3;
    UNUSED f32 pad4;
    UNUSED f32 pad5;
    f32 y;
    UNUSED f32 pad7;
    UNUSED f32 pad8;

    Height = shell->Position[0];
    y = shell->Position[1];
    z = shell->Position[2];
    if ((z < MinimumZ) || (MaximumZ < z) || (Height < MinimumX) || (MaximumX < Height) ||
        (y < MinimumY)) {
        KillObject((Object*) shell);
    }
    shell->RotVelocity += DEGREES(10);
    switch (shell->Sparam) {
        case HELD_SHELL:
            Player = &GamePlayers[shell->PlayerId];
            CopyCollisionBump(&Player->Bump, &shell->Bump);
            somePosVel[0] = 0.0f;
            somePosVel[1] = Player->Radius;
            somePosVel[2] = -(Player->Radius + shell->Radius + 2.0f);
            MultipleMatrixByVector(somePosVel, Player->align);
            shell->Position[0] = Player->Position[0] + somePosVel[0];
            Pad2 = Player->Position[1] - somePosVel[1];
            shell->Position[2] = Player->Position[2] + somePosVel[2];
            Height = CalcHeight(shell->Position[0], Pad2, shell->Position[2], Player->Bump.LastZX);
            z = Pad2 - Height;
            if ((z < 5.0f) && (z > -5.0f)) {
                shell->Position[1] = shell->Radius + Height;
            } else {
                shell->Position[1] = Pad2;
            }
            if ((Player->Flag & IS_PLAYER) != 0) {
                controller = &Controllers[shell->PlayerId];
                if ((controller->ButtonReleased & Z_TRIG) != 0) {
                    controller->ButtonReleased &= ~Z_TRIG;
                    if (controller->AnalogY < -0x2D) {
                        var_f2 = 8.0f;
                        if (Player->Speed > 8.0f) {
                            var_f2 = Player->Speed * 1.2f;
                        }
                        somePosVel[0] = 0.0f;
                        somePosVel[1] = 0.0f;
                        somePosVel[2] = -var_f2;
                        MakeAlignVector(somePosVel, Player->Direction[1] + Player->Slipang);
                        shell->Velocity[0] = somePosVel[0];
                        shell->Velocity[1] = somePosVel[1];
                        shell->Velocity[2] = somePosVel[2];
                        shell->Sparam = 2;
                        NAPlyTrgStart(shell->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        NAPlyVoiceStart(shell->PlayerId,
                                      (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        KameBlueHibasiraSet((Object*) shell - SimpleObjectArray);
                        return;
                    } else {
                        shell->Sparam = 1;
                        if (Player->Slipang > 0) {
                            shell->RotAngle = DEGREES(170);
                        } else {
                            // The minus 1 is almost certainly just due to rounding the other way
                            shell->RotAngle = -DEGREES(170) - 1;
                        }
                    }
                }
            }
            break;
        case RELEASED_SHELL:
            Player = &GamePlayers[shell->PlayerId];
            if (shell->RotAngle > 0) {
                shell->RotAngle -= DEGREES(20);
                if (shell->RotAngle < 0) {
                    shell->Sparam = 2;
                    shell->SomeTimer = 0x001E;
                    NAPlyTrgStart(shell->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->PlayerId,
                                  (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    KameBlueHibasiraSet((Object*) shell - SimpleObjectArray);
                }
            } else {
                shell->RotAngle += DEGREES(20);
                if (shell->RotAngle > 0) {
                    shell->Sparam = 2;
                    shell->SomeTimer = 0x001E;
                    NAPlyTrgStart(shell->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->PlayerId,
                                  (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    KameBlueHibasiraSet((Object*) shell - SimpleObjectArray);
                }
            }
            if (shell->Sparam == 2) {
                var_f2 = 8.0f;
                if (Player->Speed > 8.0f) {
                    var_f2 = Player->Speed * 1.2f;
                }
                somePosVel[0] = 0.0f;
                somePosVel[1] = 0.0f;
                somePosVel[2] = var_f2;
                MakeAlignVector(somePosVel, Player->Direction[1] + Player->Slipang);
                shell->Velocity[0] = somePosVel[0];
                shell->Velocity[1] = somePosVel[1];
                shell->Velocity[2] = somePosVel[2];
            } else {
                somePosVel[0] = SinT(shell->RotAngle) * 6.0f;
                somePosVel[1] = shell->Radius - Player->Radius;
                somePosVel[2] = CosT(shell->RotAngle) * 6.0f;
                MultipleMatrixByVector(somePosVel, Player->align);
                shell->Position[0] = Player->Position[0] + somePosVel[0];
                shell->Position[1] = Player->Position[1] + somePosVel[1];
                shell->Position[2] = Player->Position[2] + somePosVel[2];
            }
            break;
        case MOVING_SHELL:
            if (shell->ParentIndex > 0) {
                shell->ParentIndex -= 1;
                if (shell->ParentIndex == 0) {
                    shell->Flag &= ~MYOBJ;
                }
            }
            shell->Velocity[1] -= 0.5f;
            if (shell->Velocity[1] < -2.0f) {
                shell->Velocity[1] = -2.0f;
            }
            somePos2[0] = shell->Position[0];
            somePos2[1] = shell->Position[1];
            somePos2[2] = shell->Position[2];
            shell->Position[0] += shell->Velocity[0];
            shell->Position[1] += shell->Velocity[1];
            shell->Position[2] += shell->Velocity[2];
            CheckBump2(&shell->Bump, 4.0f, shell->Position[0], shell->Position[1], shell->Position[2], somePos2[0],
                                    somePos2[1], somePos2[2]);
            BumpObject((Object*) shell);
            if ((shell->Bump.DistanceXY < 0.0f) || (shell->Bump.DistanceYZ < 0.0f)) {
                CalcBumpVelocity(&shell->Bump, shell->Velocity);
                NAEnmTrgStart(shell->Position, shell->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
                shell->Flag |= SEKAMEOUTOBJ;
            }
            break;
        case TRIPLE_GREEN_SHELL:
            Player = &GamePlayers[shell->PlayerId];
            parent = (TripleShellParent*) &SimpleObjectArray[shell->ParentIndex];
            if (parent->Category != ACTOR_TRIPLE_GREEN_SHELL) {
                KillObject((Object*) shell);
            } else {
                shell->RotAngle += parent->RotVelocity;
                somePosVel[0] = SinT(shell->RotAngle) * 8.0f;
                somePosVel[1] = shell->Radius - Player->Radius;
                somePosVel[2] = CosT(shell->RotAngle) * 8.0f;
                MultipleMatrixByVector(somePosVel, Player->align);
                somePos2[0] = shell->Position[0];
                somePos2[1] = shell->Position[1];
                somePos2[2] = shell->Position[2];
                shell->Position[0] = Player->Position[0] + somePosVel[0];
                shell->Position[1] = Player->Position[1] + somePosVel[1];
                shell->Position[2] = Player->Position[2] + somePosVel[2];
                CheckBump2(&shell->Bump, 4.0f, shell->Position[0], shell->Position[1], shell->Position[2], somePos2[0],
                                        somePos2[1], somePos2[2]);
                BumpObject((Object*) shell);
            }
            break;
        case GREEN_SHELL_HIT_A_RACER:
            // Somehow, this fake match affects Stack management up in case 2
            shell->Velocity[1] -= (0, 0.3f);
            if (shell->Velocity[1] < -5.0f) {
                shell->Velocity[1] = -5.0f;
            }
            shell->RotAngle += DEGREES(8);
            shell->SomeTimer -= 1;
            shell->Position[1] += shell->Velocity[1];
            if (shell->SomeTimer == 0) {
                DeleteObjectBuffer((Object*) shell);
            }
            break;
        default:
            break;
    }
}
