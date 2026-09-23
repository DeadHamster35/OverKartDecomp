#include <actors.h>
#include <path.h>
#include <defines.h>
#include <Main.h>
#include <actors_extended.h>
#include <code_800029B0.h>

void SearchPoint(struct ShellActor* shell) {
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

    currentPathPoint = shell->PathIndex;
    temp_f2 = PathPointPointer[currentPathPoint].Pointx;
    temp_f12 = PathPointPointer[currentPathPoint].Pointy;
    temp_f28 = PathPointPointer[currentPathPoint].Pointz;
    nextPathPoint = currentPathPoint + 1;

    if (nextPathPoint >= CenterLineMax) {
        nextPathPoint -= CenterLineMax;
    }

    temp_f20 = temp_f2 - shell->Position[0];
    temp_f22 = temp_f12 - shell->Position[1];
    temp_f24 = temp_f28 - shell->Position[2];
    temp_f0 = (temp_f20 * temp_f20) + (temp_f22 * temp_f22) + (temp_f24 * temp_f24);
    if (temp_f0 > 400.0f) {
        temp_f18_3 = PathPointPointer[nextPathPoint].Pointx;
        temp_f16_3 = PathPointPointer[nextPathPoint].Pointy;
        temp_f26 = PathPointPointer[nextPathPoint].Pointz;

        temp_f12_0 = temp_f18_3 - shell->Position[0];
        temp_f12_1 = temp_f16_3 - shell->Position[1];
        temp_f12_2 = temp_f26 - shell->Position[2];

        temp_f0_3 = (temp_f12_0 * temp_f12_0) + (temp_f12_1 * temp_f12_1) + (temp_f12_2 * temp_f12_2);
        if (temp_f0_3 < temp_f0) {
            shell->PathIndex = nextPathPoint;
        } else {
            temp_f0_2 = sqrtf(temp_f0) * 4.0f;
            temp_f20 /= temp_f0_2;
            temp_f22 /= temp_f0_2;
            temp_f24 /= temp_f0_2;

            temp_f12_3 = shell->Velocity[0];
            temp_f14_2 = shell->Velocity[1];
            temp_f16_2 = shell->Velocity[2];

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
            shell->Velocity[0] = temp_f12_3;
            shell->Velocity[1] = temp_f14_2;
            shell->Velocity[2] = temp_f16_2;

            origPos[0] = shell->Position[0];
            origPos[1] = shell->Position[1];
            origPos[2] = shell->Position[2];

            shell->Position[0] += temp_f12_3;
            shell->Position[1] += temp_f14_2;
            shell->Position[2] += temp_f16_2;
            CheckBump2(&shell->Bump, 4.0f, shell->Position[0], shell->Position[1], shell->Position[2], origPos[0],
                                    origPos[1], origPos[2]);
            BumpObject((Object*) shell);
        }
    } else {
        if (temp_f0 > 5.0f) {
            shell->Position[0] = temp_f2;
            shell->Position[1] = shell->Radius + temp_f12;
            shell->Position[2] = temp_f28;
            shell->PathIndex = nextPathPoint;
        } else {
            temp_f18_3 = PathPointPointer[nextPathPoint].Pointx;
            temp_f16_3 = PathPointPointer[nextPathPoint].Pointy;
            temp_f26 = PathPointPointer[nextPathPoint].Pointz;

            shell->Position[0] = (temp_f2 + temp_f18_3) * 0.5f;
            shell->Position[1] = ((temp_f12 + temp_f16_3) * 0.5f) + shell->Radius;
            shell->Position[2] = (temp_f28 + temp_f26) * 0.5f;

            shell->Velocity[0] = (temp_f18_3 - temp_f2) * 0.5f;
            shell->Velocity[1] = (temp_f16_3 - temp_f12) * 0.5f;
            shell->Velocity[2] = (temp_f26 - temp_f28) * 0.5f;
        }
    }
}

void SearchEnemy(struct ShellActor* shell, Player* Player) {
    f32 x_velocity;
    f32 z_velocity;
    f32 xz_dist;
    Vec3f newPosition;

    x_velocity = Player->Position[0];
    x_velocity -= shell->Position[0];
    z_velocity = Player->Position[2];
    z_velocity -= shell->Position[2];
    xz_dist = sqrtf((x_velocity * x_velocity) + (z_velocity * z_velocity)) / 8;
    if (xz_dist == 0.0f) {
        x_velocity = 0.0f;
        z_velocity = 0.0f;
    } else {
        x_velocity /= xz_dist;
        z_velocity /= xz_dist;
    }

    newPosition[0] = shell->Position[0];
    newPosition[1] = shell->Position[1];
    newPosition[2] = shell->Position[2];
    shell->Position[0] += x_velocity;
    shell->Position[1] -= 2.0f;
    shell->Position[2] += z_velocity;
    shell->Velocity[0] = x_velocity;
    shell->Velocity[1] = -2.0f;
    shell->Velocity[2] = z_velocity;

    if (Player->SlipFlag & TERESA) {
        KillObject((Object*) shell);
    } else {
        CheckBump2(&shell->Bump, 4.0f, shell->Position[0], shell->Position[1], shell->Position[2], newPosition[0],
                                newPosition[1], newPosition[2]);
        BumpObject((Object*) shell);
        CheckKillShell(shell);
    }
}

/**
 * Only Used in Battle Mode
 * Likely trying to find the nearest Player that is not the shell's Owner and is not dead
 **/
s16 SelectBattleEnemy(Player* Owner, struct ShellActor* shell) {
    Player* Player;
    s32 playerIndex;
    f32 playerToShellDistance;
    s16 PlayerId = -1;
    f32 smallestDistance = 25000000.0f;

    for (playerIndex = 0; playerIndex < 4; playerIndex++) {
        Player = &GamePlayers[playerIndex];
        if ((Player->Flag & EXISTS) == 0) {
            continue;
        }
        if (Player == Owner) {
            continue;
        }
        if (BalloonCount[playerIndex] < 0) {
            continue;
        }
        // CalcDistance is not quite a 3D Distance function, it doubles (rather than squares) the Z difference of the
        // positions
        playerToShellDistance = CalcDistance(Player->Position, shell->Position);
        if (playerToShellDistance < smallestDistance) {
            smallestDistance = playerToShellDistance;
            PlayerId = Player - PlayerOne;
        }
    }

    return PlayerId;
}

void CheckKillShell(struct ShellActor* shell) {
    if ((shell->Bump.DistanceXY < 0.0f) &&
        ((shell->Bump.BumpXY[1] < 0.25f) || (shell->Bump.BumpXY[1] > -0.25f))) {
        KillObject((Object*) shell);
        NAEnmTrgStart(shell->Position, shell->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
        shell->Flag |= SEKAMEOUTOBJ;
    } else if ((shell->Bump.DistanceYZ < 0.0f) &&
               ((shell->Bump.BumpYZ[1] < 0.25f) || (shell->Bump.BumpYZ[1] < -0.25f))) {
        KillObject((Object*) shell);
        NAEnmTrgStart(shell->Position, shell->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x54));
        shell->Flag |= SEKAMEOUTOBJ;
    }
}

/**
 * @brief Update the red and Blue shell actors
 *
 * @Param shell
 */
void MoveRshell(struct ShellActor* shell) {
    UNUSED f32 pad9;
    Player* Player;
    f32 Height;
    UNUSED f32 temp_f14;
    f32 temp_f2;
    s16 temp_v0;
    UNUSED s16 Pad3;
    Vec3f somePosVel;
    struct Controller* controller;
    TripleShellParent* parent;
    UNUSED f32 Pad0;
    UNUSED f32 pad1;
    UNUSED f32 Pad2;
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

    pad1 = shell->Position[0];
    Pad0 = shell->Position[2];
    Pad2 = shell->Position[1];
    pad13 = shell->Category;
    if ((Pad0 < (f32) MinimumZ) || ((f32) MaximumZ < Pad0) || (pad1 < (f32) MinimumX) ||
        ((f32) MaximumX < pad1) || (Pad2 < (f32) MinimumY)) {
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
            pad7 = Player->Position[1] - somePosVel[1];
            shell->Position[2] = Player->Position[2] + somePosVel[2];
            Height = CalcHeight(shell->Position[0], pad7, shell->Position[2], Player->Bump.LastZX);
            temp_f2 = pad7 - Height;

            if ((temp_f2 < 5.0f) && (temp_f2 > -5.0f)) {
                shell->Position[1] = shell->Radius + Height;
            } else {
                shell->Position[1] = pad7;
            }

            if ((Player->Flag & IS_PLAYER) != 0) {
                if (DemoFlag) {
                    controller = ControllerOne;
                } else {
                    controller = &Controllers[shell->PlayerId];
                }
            } else {
                controller = ControllerOne;
            }

            if ((controller->ButtonReleased & Z_TRIG) != 0) {
                controller->ButtonReleased &= ~Z_TRIG;
                shell->Sparam = RELEASED_SHELL;
                if (Player->Slipang > 0) {
                    shell->RotAngle = DEGREES(170);
                } else {
                    // The minus 1 is almost certainly just due to rounding the other way
                    shell->RotAngle = -DEGREES(170) - 1;
                }
            }
            break;
        case RELEASED_SHELL:
            Player = &GamePlayers[shell->PlayerId];
            if (shell->RotAngle > 0) {
                shell->RotAngle -= DEGREES(10);
                if (shell->RotAngle < 0) {
                    shell->Sparam = MOVING_SHELL;
                    NAPlyTrgStart(shell->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->PlayerId,
                                  (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    if (pad13 == ACTOR_RED_SHELL) {
                        KameHibasiraSet((Object*) shell - SimpleObjectArray);
                    } else {
                        KameTogeHibasiraSet((Object*) shell - SimpleObjectArray);
                        NaSceneLevelStart(shell->Position, shell->Velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                    }
                }
            } else {
                shell->RotAngle += DEGREES(10);
                if (shell->RotAngle > 0) {
                    shell->Sparam = MOVING_SHELL;
                    NAPlyTrgStart(shell->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                    NAPlyVoiceStart(shell->PlayerId,
                                  (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                    if (pad13 == ACTOR_RED_SHELL) {
                        KameHibasiraSet((Object*) shell - SimpleObjectArray);
                    } else {
                        KameTogeHibasiraSet((Object*) shell - SimpleObjectArray);
                        NaSceneLevelStart(shell->Position, shell->Velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x08));
                    }
                }
            }
            if (shell->Sparam == MOVING_SHELL) {
                shell->SomeTimer = 0x001E;
                Height = 8.0f;
                if (Player->Speed > 8.0f) {
                    Height = Player->Speed * 1.2f;
                }
                somePosVel[0] = 0.0f;
                somePosVel[1] = 0.0f;
                somePosVel[2] = Height;
                MakeAlignVector(somePosVel, (s16) (Player->Direction[1] + Player->Slipang));
                shell->Velocity[0] = somePosVel[0];
                shell->Velocity[1] = somePosVel[1];
                shell->Velocity[2] = somePosVel[2];
            } else {
                somePosVel[0] = SinT(shell->RotAngle) * 8.0f;
                somePosVel[1] = shell->Radius - Player->Radius;
                somePosVel[2] = CosT(shell->RotAngle) * 8.0f;
                MultipleMatrixByVector(somePosVel, Player->align);
                shell->Position[0] = Player->Position[0] + somePosVel[0];
                shell->Position[1] = Player->Position[1] + somePosVel[1];
                shell->Position[2] = Player->Position[2] + somePosVel[2];
            }
            break;
        case MOVING_SHELL:
            Player = &GamePlayers[shell->PlayerId];
            shell->SomeTimer -= 1;
            if (shell->SomeTimer == 0) {
                shell->Flag &= 0xEFFF;
                if (shell->Category == ACTOR_BLUE_SPINY_SHELL) {
                    shell->TargetPlayer = RankBuffer[0];
                    shell->Sparam = BLUE_SHELL_LOCK_ON;
                    shell->shellId = 1000.0f;
                    temp_v0 = NearestPathPointByPlayerId[Player - PlayerOne] + 8;
                    if ((s32) CenterLineMax < temp_v0) {
                        temp_v0 -= CenterLineMax;
                    }
                    shell->PathIndex = temp_v0;
                } else if (GameMode == BATTLE) {
                    shell->shellId = 1000.0f;
                    shell->TargetPlayer = SelectBattleEnemy(Player, shell);
                    if (shell->TargetPlayer < 0) {
                        shell->Flag = EXISTOBJ;
                        shell->Velocity[1] = 3.0f;
                        shell->PathIndex = 0;
                        shell->SomeTimer = 0x003C;
                        shell->Sparam = DESTROYED_SHELL;
                    } else {
                        shell->Sparam = RED_SHELL_LOCK_ON;
                    }
                } else {
                    if (Player->Rank == 0) {
                        shell->Sparam = TRIPLE_GREEN_SHELL;
                        shell->SomeTimer = 0x0258;
                        temp_v0 = NearestPathPointByPlayerId[Player - PlayerOne] + 8;
                        if ((s32) CenterLineMax < temp_v0) {
                            temp_v0 -= CenterLineMax;
                        }
                        shell->PathIndex = temp_v0;
                    } else if (Player->Rank >= 5) {
                        shell->Sparam = GREEN_SHELL_HIT_A_RACER;
                        shell->shellId = 1000.0f;
                        temp_v0 = NearestPathPointByPlayerId[Player - PlayerOne] + 8;
                        if ((s32) CenterLineMax < temp_v0) {
                            temp_v0 -= CenterLineMax;
                        }
                        shell->PathIndex = temp_v0;
                        shell->TargetPlayer = RankBuffer[Player->Rank - 1];
                    } else {
                        shell->Sparam = RED_SHELL_LOCK_ON;
                        shell->shellId = 1000.0f;
                        shell->TargetPlayer = RankBuffer[Player->Rank - 1];
                    }
                }
            }
            shell->Velocity[1] -= 0.5;
            if (shell->Velocity[1] < -2.0f) {
                shell->Velocity[1] = -2.0f;
            }
            origPos[0] = shell->Position[0];
            origPos[1] = shell->Position[1];
            origPos[2] = shell->Position[2];
            shell->Position[0] += shell->Velocity[0];
            shell->Position[1] += shell->Velocity[1];
            shell->Position[2] += shell->Velocity[2];
            CheckBump2(&shell->Bump, 4.0f, shell->Position[0], shell->Position[1], shell->Position[2], origPos[0],
                                    origPos[1], origPos[2]);
            BumpObject((Object*) shell);
            CheckKillShell(shell);
            break;
        case RED_SHELL_LOCK_ON:
            SearchEnemy(shell, &GamePlayers[shell->TargetPlayer]);
            break;
        case TRIPLE_GREEN_SHELL:
            SearchPoint(shell);
            if (shell->SomeTimer == 0) {
                if ((shell->Flag & 0xF) == 0) {
                    KillObject((Object*) shell);
                } else {
                    shell->SomeTimer -= 1;
                }
            }
            break;
        case GREEN_SHELL_HIT_A_RACER:
            SearchPoint(shell);
            Player = &GamePlayers[shell->TargetPlayer];
            Height = Player->Position[0];
            Height -= shell->Position[0];
            temp_f2 = Player->Position[2];
            temp_f2 -= shell->Position[2];
            if (((Height * Height) + (temp_f2 * temp_f2)) < 40000.0f) {
                shell->Sparam = RED_SHELL_LOCK_ON;
            }
            break;
        case TRIPLE_RED_SHELL:
            Player = &GamePlayers[shell->PlayerId];
            parent = (TripleShellParent*) &SimpleObjectArray[shell->ParentIndex];
            if (parent->Category != ACTOR_TRIPLE_RED_SHELL) {
                KillObject((Object*) shell);
            } else {
                shell->RotAngle += parent->RotVelocity;
                somePosVel[0] = SinT(shell->RotAngle) * 8.0f;
                somePosVel[1] = shell->Radius - Player->Radius;
                somePosVel[2] = CosT(shell->RotAngle) * 8.0f;
                MultipleMatrixByVector(somePosVel, Player->align);
                origPos[0] = shell->Position[0];
                origPos[1] = shell->Position[1];
                origPos[2] = shell->Position[2];
                shell->Position[0] = Player->Position[0] + somePosVel[0];
                shell->Position[1] = Player->Position[1] + somePosVel[1];
                shell->Position[2] = Player->Position[2] + somePosVel[2];
                CheckBump2(&shell->Bump, 4.0f, shell->Position[0], shell->Position[1], shell->Position[2], origPos[0],
                                        origPos[1], origPos[2]);
                BumpObject((Object*) shell);
            }
            break;
        case DESTROYED_SHELL:
            shell->Velocity[1] -= 0.3f;
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
        case BLUE_SHELL_LOCK_ON:
            SearchPoint(shell);
            shell->TargetPlayer = RankBuffer[0];
            Player = &GamePlayers[RankBuffer[0]];
            Height = Player->Position[0];
            Height -= shell->Position[0];
            temp_f2 = Player->Position[2];
            temp_f2 -= shell->Position[2];
            if (((Height * Height) + (temp_f2 * temp_f2)) < 40000.0f) {
                shell->Sparam = BLUE_SHELL_TARGET_ELIMINATED;
            }
            break;
        case 9:
            SearchEnemy(shell, &GamePlayers[shell->TargetPlayer]);
            break;
        default:
            break;
    }
}
