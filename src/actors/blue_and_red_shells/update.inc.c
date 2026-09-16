#include <actors.h>
#include <path.h>
#include <defines.h>
#include <main.h>
#include <actors_extended.h>
#include <code_800029B0.h>

void search_point(struct ShellActor* shell) {
    u16 currentPathPoint;
    u16 nextPathPoint;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12_3;
    f32 temp_f14_2;
    f32 temp_f16_2;
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f28;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f12_0;
    f32 temp_f12_1;
    f32 temp_f12_2;
    f32 temp_f18_3;
    f32 temp_f16_3;
    f32 temp_f26;
    Vec3f origPos;

    currentPathPoint = shell->pathIndex;
    temp_f2 = g_pathPointPointer[currentPathPoint].pointx;
    temp_f12 = g_pathPointPointer[currentPathPoint].pointy;
    temp_f28 = g_pathPointPointer[currentPathPoint].pointz;
    nextPathPoint = currentPathPoint + 1;

    if (nextPathPoint >= center_line_max) {
        nextPathPoint -= center_line_max;
    }

    temp_f20 = temp_f2 - shell->position[0];
    temp_f22 = temp_f12 - shell->position[1];
    temp_f24 = temp_f28 - shell->position[2];
    temp_f0 = (temp_f20 * temp_f20) + (temp_f22 * temp_f22) + (temp_f24 * temp_f24);
    if (temp_f0 > 400.0f) {
        temp_f18_3 = g_pathPointPointer[nextPathPoint].pointx;
        temp_f16_3 = g_pathPointPointer[nextPathPoint].pointy;
        temp_f26 = g_pathPointPointer[nextPathPoint].pointz;

        temp_f12_0 = temp_f18_3 - shell->position[0];
        temp_f12_1 = temp_f16_3 - shell->position[1];
        temp_f12_2 = temp_f26 - shell->position[2];

        temp_f0_3 = (temp_f12_0 * temp_f12_0) + (temp_f12_1 * temp_f12_1) + (temp_f12_2 * temp_f12_2);
        if (temp_f0_3 < temp_f0) {
            shell->pathIndex = nextPathPoint;
        } else {
            temp_f0_2 = sqrtf(temp_f0) * 4.0f;
            temp_f20 /= temp_f0_2;
            temp_f22 /= temp_f0_2;
            temp_f24 /= temp_f0_2;

            temp_f12_3 = shell->velocity[0];
            temp_f14_2 = shell->velocity[1];
            temp_f16_2 = shell->velocity[2];

            temp_f12_3 += temp_f20;
            temp_f14_2 += temp_f22;
            temp_f16_2 += temp_f24;
            temp_f0 = sqrtf((temp_f12_3 * temp_f12_3) + (temp_f14_2 * temp_f14_2) + (temp_f16_2 * temp_f16_2));
            if (temp_f0 > 6.0f) {
                temp_f0 /= 6.0f;
                temp_f12_3 /= temp_f0;
                temp_f14_2 /= temp_f0;
                temp_f16_2 /= temp_f0;
            }
            shell->velocity[0] = temp_f12_3;
            shell->velocity[1] = temp_f14_2;
            shell->velocity[2] = temp_f16_2;

            origPos[0] = shell->position[0];
            origPos[1] = shell->position[1];
            origPos[2] = shell->position[2];

            shell->position[0] += temp_f12_3;
            shell->position[1] += temp_f14_2;
            shell->position[2] += temp_f16_2;
            CheckBump2(&shell->bump, 4.0f, shell->position[0], shell->position[1], shell->position[2], origPos[0],
                                    origPos[1], origPos[2]);
            BumpObject((Object*) shell);
        }
    } else {
        if (temp_f0 > 5.0f) {
            shell->position[0] = temp_f2;
            shell->position[1] = shell->radius + temp_f12;
            shell->position[2] = temp_f28;
            shell->pathIndex = nextPathPoint;
        } else {
            temp_f18_3 = g_pathPointPointer[nextPathPoint].pointx;
            temp_f16_3 = g_pathPointPointer[nextPathPoint].pointy;
            temp_f26 = g_pathPointPointer[nextPathPoint].pointz;

            shell->position[0] = (temp_f2 + temp_f18_3) * 0.5f;
            shell->position[1] = ((temp_f12 + temp_f16_3) * 0.5f) + shell->radius;
            shell->position[2] = (temp_f28 + temp_f26) * 0.5f;

            shell->velocity[0] = (temp_f18_3 - temp_f2) * 0.5f;
            shell->velocity[1] = (temp_f16_3 - temp_f12) * 0.5f;
            shell->velocity[2] = (temp_f26 - temp_f28) * 0.5f;
        }
    }
}

void search_enemy(struct ShellActor* shell, Player* player) {
    f32 x_velocity;
    f32 z_velocity;
    f32 xz_dist;
    Vec3f newPosition;

    x_velocity = player->position[0];
    x_velocity -= shell->position[0];
    z_velocity = player->position[2];
    z_velocity -= shell->position[2];
    xz_dist = sqrtf((x_velocity * x_velocity) + (z_velocity * z_velocity)) / 8;
    if (xz_dist == 0.0f) {
        x_velocity = 0.0f;
        z_velocity = 0.0f;
    } else {
        x_velocity /= xz_dist;
        z_velocity /= xz_dist;
    }

    newPosition[0] = shell->position[0];
    newPosition[1] = shell->position[1];
    newPosition[2] = shell->position[2];
    shell->position[0] += x_velocity;
    shell->position[1] -= 2.0f;
    shell->position[2] += z_velocity;
    shell->velocity[0] = x_velocity;
    shell->velocity[1] = -2.0f;
    shell->velocity[2] = z_velocity;

    if (player->slip_flag & TERESA) {
        KillObject((Object*) shell);
    } else {
        CheckBump2(&shell->bump, 4.0f, shell->position[0], shell->position[1], shell->position[2], newPosition[0],
                                newPosition[1], newPosition[2]);
        BumpObject((Object*) shell);
        check_kill_shell(shell);
    }
}

/**
 * Only used in Battle mode
 * Likely trying to find the nearest player that is not the shell's owner and is not dead
 **/
s16 select_battle_enemy(Player* owner, struct ShellActor* shell) {
    Player* player;
    s32 playerIndex;
    f32 playerToShellDistance;
    s16 playerId = -1;
    f32 smallestDistance = 25000000.0f;

    for (playerIndex = 0; playerIndex < 4; playerIndex++) {
        player = &gPlayers[playerIndex];
        if ((player->flag & EXISTS) == 0) {
            continue;
        }
        if (player == owner) {
            continue;
        }
        if (BalloonCount[playerIndex] < 0) {
            continue;
        }
        // CalcDistance is not quite a 3D distance function, it doubles (rather than squares) the Z difference of the
        // positions
        playerToShellDistance = CalcDistance(player->position, shell->position);
        if (playerToShellDistance < smallestDistance) {
            smallestDistance = playerToShellDistance;
            playerId = player - gPlayerOne;
        }
    }

    return playerId;
}

void check_kill_shell(struct ShellActor* shell) {
    if ((shell->bump.distance_xy < 0.0f) &&
        ((shell->bump.bump_xy[1] < 0.25f) || (shell->bump.bump_xy[1] > -0.25f))) {
        KillObject((Object*) shell);
        NAEnmTrgStart(shell->position, shell->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
        shell->flag |= SEKAMEOUTOBJ;
    } else if ((shell->bump.distance_yz < 0.0f) &&
               ((shell->bump.bump_yz[1] < 0.25f) || (shell->bump.bump_yz[1] < -0.25f))) {
        KillObject((Object*) shell);
        NAEnmTrgStart(shell->position, shell->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
        shell->flag |= SEKAMEOUTOBJ;
    }
}

/**
 * @brief Update the red and blue shell actors
 *
 * @param shell
 */
void move_rshell(struct ShellActor* shell) {
    UNUSED f32 pad9;
    Player* player;
    f32 height;
    UNUSED f32 temp_f14;
    f32 temp_f2;
    s16 temp_v0;
    UNUSED s16 pad3;
    Vec3f somePosVel;
    struct Controller* controller;
    TripleShellParent* parent;
    UNUSED f32 pad0;
    UNUSED f32 pad1;
    UNUSED f32 pad2;
    UNUSED f32 pad4;
    UNUSED f32 pad5;
    UNUSED f32 pad6;
    UNUSED f32 pad7;
    UNUSED f32 pad8;
    UNUSED f32 pad10;
    UNUSED f32 pad11;
    UNUSED f32 pad12;
    UNUSED s16 pad13;
    UNUSED s16 pad13_2;
    UNUSED f32 pad14;
    UNUSED f32 pad15;
    UNUSED f32 pad16;
    UNUSED f32 pad17;
    Vec3f origPos;

    pad1 = shell->position[0];
    pad0 = shell->position[2];
    pad2 = shell->position[1];
    pad13 = shell->category;
    if ((pad0 < (f32) minimum_z) || ((f32) maximum_z < pad0) || (pad1 < (f32) minimum_x) ||
        ((f32) maximum_x < pad1) || (pad2 < (f32) minimum_y)) {
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
            pad7 = player->position[1] - somePosVel[1];
            shell->position[2] = player->position[2] + somePosVel[2];
            height = CalcHeight(shell->position[0], pad7, shell->position[2], player->bump.last_zx);
            temp_f2 = pad7 - height;

            if ((temp_f2 < 5.0f) && (temp_f2 > -5.0f)) {
                shell->position[1] = shell->radius + height;
            } else {
                shell->position[1] = pad7;
            }

            if ((player->flag & IS_PLAYER) != 0) {
                if (g_DemoFlag) {
                    controller = gControllerOne;
                } else {
                    controller = &gControllers[shell->playerId];
                }
            } else {
                controller = gControllerOne;
            }

            if ((controller->ButtonReleased & Z_TRIG) != 0) {
                controller->ButtonReleased &= ~Z_TRIG;
                shell->sparam = RELEASED_SHELL;
                if (player->slipang > 0) {
                    shell->rotAngle = DEGREES(170);
                } else {
                    // The minus 1 is almost certainly just due to rounding the other way
                    shell->rotAngle = -DEGREES(170) - 1;
                }
            }
            break;
        case RELEASED_SHELL:
            player = &gPlayers[shell->playerId];
            if (shell->rotAngle > 0) {
                shell->rotAngle -= DEGREES(10);
                if (shell->rotAngle < 0) {
                    shell->sparam = MOVING_SHELL;
                    NAPlyTrgStart(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->playerId,
                                  (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    if (pad13 == ACTOR_RED_SHELL) {
                        kame_hibasira_set((Object*) shell - g_SimpleObjectArray);
                    } else {
                        kame_toge_hibasira_set((Object*) shell - g_SimpleObjectArray);
                        NaSceneLevelStart(shell->position, shell->velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                    }
                }
            } else {
                shell->rotAngle += DEGREES(10);
                if (shell->rotAngle > 0) {
                    shell->sparam = MOVING_SHELL;
                    NAPlyTrgStart(shell->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->playerId,
                                  (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    if (pad13 == ACTOR_RED_SHELL) {
                        kame_hibasira_set((Object*) shell - g_SimpleObjectArray);
                    } else {
                        kame_toge_hibasira_set((Object*) shell - g_SimpleObjectArray);
                        NaSceneLevelStart(shell->position, shell->velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                    }
                }
            }
            if (shell->sparam == MOVING_SHELL) {
                shell->someTimer = 0x001E;
                height = 8.0f;
                if (player->speed > 8.0f) {
                    height = player->speed * 1.2f;
                }
                somePosVel[0] = 0.0f;
                somePosVel[1] = 0.0f;
                somePosVel[2] = height;
                MakeAlignVector(somePosVel, (s16) (player->direction[1] + player->slipang));
                shell->velocity[0] = somePosVel[0];
                shell->velocity[1] = somePosVel[1];
                shell->velocity[2] = somePosVel[2];
            } else {
                somePosVel[0] = sinT(shell->rotAngle) * 8.0f;
                somePosVel[1] = shell->radius - player->radius;
                somePosVel[2] = cosT(shell->rotAngle) * 8.0f;
                MultipleMatrixByVector(somePosVel, player->align);
                shell->position[0] = player->position[0] + somePosVel[0];
                shell->position[1] = player->position[1] + somePosVel[1];
                shell->position[2] = player->position[2] + somePosVel[2];
            }
            break;
        case MOVING_SHELL:
            player = &gPlayers[shell->playerId];
            shell->someTimer -= 1;
            if (shell->someTimer == 0) {
                shell->flag &= 0xEFFF;
                if (shell->category == ACTOR_BLUE_SPINY_SHELL) {
                    shell->targetPlayer = rank_buffer[0];
                    shell->sparam = BLUE_SHELL_LOCK_ON;
                    shell->shellId = 1000.0f;
                    temp_v0 = gNearestPathPointByPlayerId[player - gPlayerOne] + 8;
                    if ((s32) center_line_max < temp_v0) {
                        temp_v0 -= center_line_max;
                    }
                    shell->pathIndex = temp_v0;
                } else if (g_gameMode == BATTLE) {
                    shell->shellId = 1000.0f;
                    shell->targetPlayer = select_battle_enemy(player, shell);
                    if (shell->targetPlayer < 0) {
                        shell->flag = EXISTOBJ;
                        shell->velocity[1] = 3.0f;
                        shell->pathIndex = 0;
                        shell->someTimer = 0x003C;
                        shell->sparam = DESTROYED_SHELL;
                    } else {
                        shell->sparam = RED_SHELL_LOCK_ON;
                    }
                } else {
                    if (player->rank == 0) {
                        shell->sparam = TRIPLE_GREEN_SHELL;
                        shell->someTimer = 0x0258;
                        temp_v0 = gNearestPathPointByPlayerId[player - gPlayerOne] + 8;
                        if ((s32) center_line_max < temp_v0) {
                            temp_v0 -= center_line_max;
                        }
                        shell->pathIndex = temp_v0;
                    } else if (player->rank >= 5) {
                        shell->sparam = GREEN_SHELL_HIT_A_RACER;
                        shell->shellId = 1000.0f;
                        temp_v0 = gNearestPathPointByPlayerId[player - gPlayerOne] + 8;
                        if ((s32) center_line_max < temp_v0) {
                            temp_v0 -= center_line_max;
                        }
                        shell->pathIndex = temp_v0;
                        shell->targetPlayer = rank_buffer[player->rank - 1];
                    } else {
                        shell->sparam = RED_SHELL_LOCK_ON;
                        shell->shellId = 1000.0f;
                        shell->targetPlayer = rank_buffer[player->rank - 1];
                    }
                }
            }
            shell->velocity[1] -= 0.5;
            if (shell->velocity[1] < -2.0f) {
                shell->velocity[1] = -2.0f;
            }
            origPos[0] = shell->position[0];
            origPos[1] = shell->position[1];
            origPos[2] = shell->position[2];
            shell->position[0] += shell->velocity[0];
            shell->position[1] += shell->velocity[1];
            shell->position[2] += shell->velocity[2];
            CheckBump2(&shell->bump, 4.0f, shell->position[0], shell->position[1], shell->position[2], origPos[0],
                                    origPos[1], origPos[2]);
            BumpObject((Object*) shell);
            check_kill_shell(shell);
            break;
        case RED_SHELL_LOCK_ON:
            search_enemy(shell, &gPlayers[shell->targetPlayer]);
            break;
        case TRIPLE_GREEN_SHELL:
            search_point(shell);
            if (shell->someTimer == 0) {
                if ((shell->flag & 0xF) == 0) {
                    KillObject((Object*) shell);
                } else {
                    shell->someTimer -= 1;
                }
            }
            break;
        case GREEN_SHELL_HIT_A_RACER:
            search_point(shell);
            player = &gPlayers[shell->targetPlayer];
            height = player->position[0];
            height -= shell->position[0];
            temp_f2 = player->position[2];
            temp_f2 -= shell->position[2];
            if (((height * height) + (temp_f2 * temp_f2)) < 40000.0f) {
                shell->sparam = RED_SHELL_LOCK_ON;
            }
            break;
        case TRIPLE_RED_SHELL:
            player = &gPlayers[shell->playerId];
            parent = (TripleShellParent*) &g_SimpleObjectArray[shell->parentIndex];
            if (parent->category != ACTOR_TRIPLE_RED_SHELL) {
                KillObject((Object*) shell);
            } else {
                shell->rotAngle += parent->rotVelocity;
                somePosVel[0] = sinT(shell->rotAngle) * 8.0f;
                somePosVel[1] = shell->radius - player->radius;
                somePosVel[2] = cosT(shell->rotAngle) * 8.0f;
                MultipleMatrixByVector(somePosVel, player->align);
                origPos[0] = shell->position[0];
                origPos[1] = shell->position[1];
                origPos[2] = shell->position[2];
                shell->position[0] = player->position[0] + somePosVel[0];
                shell->position[1] = player->position[1] + somePosVel[1];
                shell->position[2] = player->position[2] + somePosVel[2];
                CheckBump2(&shell->bump, 4.0f, shell->position[0], shell->position[1], shell->position[2], origPos[0],
                                        origPos[1], origPos[2]);
                BumpObject((Object*) shell);
            }
            break;
        case DESTROYED_SHELL:
            shell->velocity[1] -= 0.3f;
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
        case BLUE_SHELL_LOCK_ON:
            search_point(shell);
            shell->targetPlayer = rank_buffer[0];
            player = &gPlayers[rank_buffer[0]];
            height = player->position[0];
            height -= shell->position[0];
            temp_f2 = player->position[2];
            temp_f2 -= shell->position[2];
            if (((height * height) + (temp_f2 * temp_f2)) < 40000.0f) {
                shell->sparam = BLUE_SHELL_TARGET_ELIMINATED;
            }
            break;
        case 9:
            search_enemy(shell, &gPlayers[shell->targetPlayer]);
            break;
        default:
            break;
    }
}
