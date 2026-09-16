#include <actors.h>
#include <main.h>
#include <code_800029B0.h>
#include <defines.h>
#include "collision.h"

/**
 * @brief Updates the green shell actor.
 *
 * @param shell
 */
void move_gshell(struct ShellActor* shell) {
    Player* player;
    UNUSED f32 pad9;
    UNUSED f32 padA;
    Vec3f somePos2;
    Vec3f somePosVel;
    f32 var_f2;
    struct Controller* controller;
    TripleShellParent* parent;
    f32 height;
    f32 z;
    UNUSED f32 pad2;
    UNUSED f32 pad3;
    UNUSED f32 pad4;
    UNUSED f32 pad5;
    f32 y;
    UNUSED f32 pad7;
    UNUSED f32 pad8;

    height = shell->position[0];
    y = shell->position[1];
    z = shell->position[2];
    if ((z < minimum_z) || (maximum_z < z) || (height < minimum_x) || (maximum_x < height) ||
        (y < minimum_y)) {
        KillObject((Object*) shell);
    }
    shell->rotVelocity += DEGREES(10);
    switch (shell->sparam) {
        case HELD_SHELL:
            player = &gPlayers[shell->playerId];
            copy_bump(&player->bump, &shell->bump);
            somePosVel[0] = 0.0f;
            somePosVel[1] = player->radius;
            somePosVel[2] = -(player->radius + shell->radius + 2.0f);
            MultipleMatrixByVector(somePosVel, player->align);
            shell->position[0] = player->position[0] + somePosVel[0];
            pad2 = player->position[1] - somePosVel[1];
            shell->position[2] = player->position[2] + somePosVel[2];
            height = CalcHeight(shell->position[0], pad2, shell->position[2], player->bump.last_zx);
            z = pad2 - height;
            if ((z < 5.0f) && (z > -5.0f)) {
                shell->position[1] = shell->radius + height;
            } else {
                shell->position[1] = pad2;
            }
            if ((player->flag & IS_PLAYER) != 0) {
                controller = &gControllers[shell->playerId];
                if ((controller->ButtonReleased & Z_TRIG) != 0) {
                    controller->ButtonReleased &= ~Z_TRIG;
                    if (controller->AnalogY < -0x2D) {
                        var_f2 = 8.0f;
                        if (player->speed > 8.0f) {
                            var_f2 = player->speed * 1.2f;
                        }
                        somePosVel[0] = 0.0f;
                        somePosVel[1] = 0.0f;
                        somePosVel[2] = -var_f2;
                        MakeAlignVector(somePosVel, player->direction[1] + player->slipang);
                        shell->velocity[0] = somePosVel[0];
                        shell->velocity[1] = somePosVel[1];
                        shell->velocity[2] = somePosVel[2];
                        shell->sparam = 2;
                        NAPlyTrgStart(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        NAPlyVoiceStart(shell->playerId,
                                      (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        kame_blue_hibasira_set((Object*) shell - g_SimpleObjectArray);
                        return;
                    } else {
                        shell->sparam = 1;
                        if (player->slipang > 0) {
                            shell->rotAngle = DEGREES(170);
                        } else {
                            // The minus 1 is almost certainly just due to rounding the other way
                            shell->rotAngle = -DEGREES(170) - 1;
                        }
                    }
                }
            }
            break;
        case RELEASED_SHELL:
            player = &gPlayers[shell->playerId];
            if (shell->rotAngle > 0) {
                shell->rotAngle -= DEGREES(20);
                if (shell->rotAngle < 0) {
                    shell->sparam = 2;
                    shell->someTimer = 0x001E;
                    NAPlyTrgStart(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->playerId,
                                  (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    kame_blue_hibasira_set((Object*) shell - g_SimpleObjectArray);
                }
            } else {
                shell->rotAngle += DEGREES(20);
                if (shell->rotAngle > 0) {
                    shell->sparam = 2;
                    shell->someTimer = 0x001E;
                    NAPlyTrgStart(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->playerId,
                                  (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    kame_blue_hibasira_set((Object*) shell - g_SimpleObjectArray);
                }
            }
            if (shell->sparam == 2) {
                var_f2 = 8.0f;
                if (player->speed > 8.0f) {
                    var_f2 = player->speed * 1.2f;
                }
                somePosVel[0] = 0.0f;
                somePosVel[1] = 0.0f;
                somePosVel[2] = var_f2;
                MakeAlignVector(somePosVel, player->direction[1] + player->slipang);
                shell->velocity[0] = somePosVel[0];
                shell->velocity[1] = somePosVel[1];
                shell->velocity[2] = somePosVel[2];
            } else {
                somePosVel[0] = sinT(shell->rotAngle) * 6.0f;
                somePosVel[1] = shell->radius - player->radius;
                somePosVel[2] = cosT(shell->rotAngle) * 6.0f;
                MultipleMatrixByVector(somePosVel, player->align);
                shell->position[0] = player->position[0] + somePosVel[0];
                shell->position[1] = player->position[1] + somePosVel[1];
                shell->position[2] = player->position[2] + somePosVel[2];
            }
            break;
        case MOVING_SHELL:
            if (shell->parentIndex > 0) {
                shell->parentIndex -= 1;
                if (shell->parentIndex == 0) {
                    shell->flag &= ~MYOBJ;
                }
            }
            shell->velocity[1] -= 0.5f;
            if (shell->velocity[1] < -2.0f) {
                shell->velocity[1] = -2.0f;
            }
            somePos2[0] = shell->position[0];
            somePos2[1] = shell->position[1];
            somePos2[2] = shell->position[2];
            shell->position[0] += shell->velocity[0];
            shell->position[1] += shell->velocity[1];
            shell->position[2] += shell->velocity[2];
            CheckBump2(&shell->bump, 4.0f, shell->position[0], shell->position[1], shell->position[2], somePos2[0],
                                    somePos2[1], somePos2[2]);
            BumpObject((Object*) shell);
            if ((shell->bump.distance_xy < 0.0f) || (shell->bump.distance_yz < 0.0f)) {
                CalcBumpVelocity(&shell->bump, shell->velocity);
                NAEnmTrgStart(shell->position, shell->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
                shell->flag |= SEKAMEOUTOBJ;
            }
            break;
        case TRIPLE_GREEN_SHELL:
            player = &gPlayers[shell->playerId];
            parent = (TripleShellParent*) &g_SimpleObjectArray[shell->parentIndex];
            if (parent->category != ACTOR_TRIPLE_GREEN_SHELL) {
                KillObject((Object*) shell);
            } else {
                shell->rotAngle += parent->rotVelocity;
                somePosVel[0] = sinT(shell->rotAngle) * 8.0f;
                somePosVel[1] = shell->radius - player->radius;
                somePosVel[2] = cosT(shell->rotAngle) * 8.0f;
                MultipleMatrixByVector(somePosVel, player->align);
                somePos2[0] = shell->position[0];
                somePos2[1] = shell->position[1];
                somePos2[2] = shell->position[2];
                shell->position[0] = player->position[0] + somePosVel[0];
                shell->position[1] = player->position[1] + somePosVel[1];
                shell->position[2] = player->position[2] + somePosVel[2];
                CheckBump2(&shell->bump, 4.0f, shell->position[0], shell->position[1], shell->position[2], somePos2[0],
                                        somePos2[1], somePos2[2]);
                BumpObject((Object*) shell);
            }
            break;
        case GREEN_SHELL_HIT_A_RACER:
            // Somehow, this fake match affects stack management up in case 2
            shell->velocity[1] -= (0, 0.3f);
            if (shell->velocity[1] < -5.0f) {
                shell->velocity[1] = -5.0f;
            }
            shell->rotAngle += DEGREES(8);
            shell->someTimer -= 1;
            shell->position[1] += shell->velocity[1];
            if (shell->someTimer == 0) {
                deleteObjectBuffer((Object*) shell);
            }
            break;
        default:
            break;
    }
}
