#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <actor_types.h>
#include "code_800029B0.h"
#include "math_util.h"
#include "memory.h"
#include "path.h"
#include "cpu_vehicles_camera_path.h"
#include "menu_items.h"
#include "collision.h"
#include "actors.h"
#include "actors_extended.h"
#include "audio/external.h"
#include "update_objects.h"
#include "effects.h"
#include "sounds.h"
#include "OverKartHooks.h"

void CopyCollisionBump(Collision* src, Collision* dest) {
    dest->FlagXY = src->FlagXY;
    dest->FlagYZ = src->FlagYZ;
    dest->FlagZX = src->FlagZX;
    dest->LastXY = src->LastXY;
    dest->LastYZ = src->LastYZ;
    dest->LastZX = src->LastZX;
    dest->DistanceXY = src->DistanceXY;
    dest->DistanceYZ = src->DistanceYZ;
    dest->DistanceZX = src->DistanceZX;

    CopyVector(dest->BumpXY, src->BumpXY);
    CopyVector(dest->BumpYZ, src->BumpYZ);
    CopyVector(dest->BumpZX, src->BumpZX);
}

void DeleteRollingShell(struct ShellActor* shell, s32 shellType) {
    TripleShellParent* parent = (TripleShellParent*) &SimpleObjectArray[shell->ParentIndex];

    parent->ShellsAvailable--;

    switch ((s16) shell->shellId) {
        case 0:
            parent->ShellIndices[0] = -1.0f;
            break;
        case 1:
            parent->ShellIndices[1] = -1.0f;
            break;
        case 2:
            parent->ShellIndices[2] = -1.0f;
            break;
    }

    shell->Flag = EXISTOBJ; // bitflag
    shell->RotAngle = 0;
    shell->Velocity[1] = 3.0f;
    shell->SomeTimer = 60;

    switch (shellType) {
        case ACTOR_GREEN_SHELL:
            shell->Sparam = GREEN_SHELL_HIT_A_RACER;
            break;
        case ACTOR_RED_SHELL:
            shell->Sparam = DESTROYED_SHELL;
            break;
    }
}

// Sets velocities for a banana, Used when a racer runs into
// a banana bunch.
void CrushBanana(struct BananaActor* banana) {
    banana->Sparam = DROPPED_BANANA;
    banana->Counter = 0x00B4;
    banana->Velocity[0] = ((f32) (MakeRandomLimmit(0x00C8) - 0x64) * 0.015);
    banana->Velocity[1] = ((f32) MakeRandomLimmit(0x00C8)) * 0.015;
    banana->Velocity[2] = ((f32) (MakeRandomLimmit(0x00C8) - 0x64) * 0.015);
}

void ClearBackward(s16 bananaIndex) {
    struct BananaActor* banana;

    if (bananaIndex != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[bananaIndex];
        CrushBanana(banana);
        ClearBackward(banana->YoungerIndex);
    }
}

void ClearForward(UNUSED struct BananaActor* arg0, s16 bananaIndex) {
    struct BananaActor* banana;

    if (bananaIndex != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[bananaIndex];
        CrushBanana(banana);
        ClearForward(banana, banana->ElderIndex);
    }
}

// Handle a banana being ran over while it is still part of a banana bunch
void CleanUpBanana(struct BananaActor* banana) {
    struct BananaBunchParent* temp_v0_2;

    ClearBackward(banana->YoungerIndex);
    ClearForward(banana, banana->ElderIndex);
    if ((GamePlayers[banana->PlayerId].Flag & IS_PLAYER) != 0) {
        NAPlyTrgStart(banana->PlayerId, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
    }
    banana->Flag = -0x8000;
    banana->Counter = 0x003C;
    banana->Sparam = DESTROYED_BANANA;
    banana->Velocity[1] = 3.0f;
    temp_v0_2 = (struct BananaBunchParent*) &SimpleObjectArray[banana->ParentIndex];
    temp_v0_2->BananaIndices[0] = -1;
    temp_v0_2->BananaIndices[1] = -1;
    temp_v0_2->BananaIndices[2] = -1;
    temp_v0_2->BananaIndices[3] = -1;
    temp_v0_2->BananaIndices[4] = -1;
}

// Drop a banana from a banana bunch?
void ReleaseBanana(struct BananaBunchParent* banana_bunch) {
    s16 ElderIndex;
    struct BananaActor* banana;

    banana_bunch->BananasAvailable -= 1;
    if (banana_bunch->BananaIndices[4] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[4]];
        banana_bunch->BananaIndices[4] = -1;
    } else if (banana_bunch->BananaIndices[3] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[3]];
        banana_bunch->BananaIndices[3] = -1;
    } else if (banana_bunch->BananaIndices[2] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[2]];
        banana_bunch->BananaIndices[2] = -1;
    } else if (banana_bunch->BananaIndices[1] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[1]];
        banana_bunch->BananaIndices[1] = -1;
    } else if (banana_bunch->BananaIndices[0] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[0]];
        banana_bunch->BananaIndices[0] = -1;
    } else {
        return;
    }

    banana->Sparam = DROPPED_BANANA;
    banana->Counter = 0x00B4;
    banana->Velocity[0] = 0.0f;
    banana->Velocity[1] = 1.5f;
    banana->Velocity[2] = 0.0f;
    ElderIndex = banana->ElderIndex;
    if (ElderIndex != -1) {
        ((struct BananaActor*) &SimpleObjectArray[ElderIndex])->YoungerIndex = -1;
    }
}

// Drop a banana from a banana bunch and throw it forward?
void ThrowBanana(s16 AnalogY, struct BananaBunchParent* banana_bunch, Player* Player) {
    Vec3f Velocity;
    struct BananaActor* banana;
    struct BananaActor* elderBanana;
    f32 var_f0;
    f32 var_f12;

    banana_bunch->BananasAvailable -= 1;
    if (banana_bunch->BananaIndices[4] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[4]];
        banana_bunch->BananaIndices[4] = -1;
    } else if (banana_bunch->BananaIndices[3] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[3]];
        banana_bunch->BananaIndices[3] = -1;
    } else if (banana_bunch->BananaIndices[2] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[2]];
        banana_bunch->BananaIndices[2] = -1;
    } else if (banana_bunch->BananaIndices[1] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[1]];
        banana_bunch->BananaIndices[1] = -1;
    } else if (banana_bunch->BananaIndices[0] != -1) {
        banana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[0]];
        banana_bunch->BananaIndices[0] = -1;
    } else {
        return;
    }

    banana->Sparam = DROPPED_BANANA;
    banana->Counter = 0x001E;
    if (banana->ElderIndex != -1) {
        elderBanana = (struct BananaActor*) &SimpleObjectArray[banana->ElderIndex];
        elderBanana->YoungerIndex = -1;
    }
    if (Player->Speed < 2.0f) {
        var_f0 = ((AnalogY - 30.0f) / 20.0f) + 1.5f;
        var_f12 = 4.0f;
    } else {
        var_f0 = ((AnalogY - 30.0f) / 20.0f) + 1.5f;
        var_f12 = (Player->Speed * 0.75f) + 4.5f + var_f0;
    }
    Vec3FSet(Velocity, 0.0f, var_f0, var_f12);
    MakeAlignVector(Velocity, Player->Direction[1] + Player->Slipang);
    banana->Velocity[0] = Velocity[0];
    banana->Velocity[1] = Velocity[1];
    banana->Velocity[2] = Velocity[2];
}

s32 CheckSuperBananaNumber(s16 bananaId) {
    struct BananaActor* banana;
    if (bananaId == -1) {
        return 0;
    }
    banana = (struct BananaActor*) &SimpleObjectArray[bananaId];
    if (banana->Sparam == FIRST_BANANA_BUNCH_BANANA) {
        return 1;
    }
    if (banana->Sparam == BANANA_BUNCH_BANANA) {
        return 1;
    }
    return 0;
}

void SuperBananaStrategy(struct BananaBunchParent* banana_bunch) {
    UNUSED s32 pad[2];
    Player* Owner;
    struct Controller* controller;
    s32 someCount;

    Owner = &GamePlayers[banana_bunch->PlayerId];
    switch (banana_bunch->Sparam) {
        case 0:
            PutSBanana(banana_bunch, Owner, 0);
            banana_bunch->Counter = 4;
            banana_bunch->Sparam = 1;
            banana_bunch->BananasAvailable = 1;
            break;
        case 1:
            banana_bunch->Counter -= 1;
            if (banana_bunch->Counter == 0) {
                PutSBanana(banana_bunch, Owner, 1);
                banana_bunch->Counter = 4;
                banana_bunch->Sparam = 2;
                banana_bunch->BananasAvailable += 1;
            }
            break;
        case 2:
            banana_bunch->Counter -= 1;
            if (banana_bunch->Counter == 0) {
                PutSBanana(banana_bunch, Owner, 2);
                banana_bunch->Counter = 4;
                banana_bunch->Sparam = 3;
                banana_bunch->BananasAvailable += 1;
            }
            break;
        case 3:
            banana_bunch->Counter -= 1;
            if (banana_bunch->Counter == 0) {
                PutSBanana(banana_bunch, Owner, 3);
                banana_bunch->Counter = 4;
                banana_bunch->Sparam = 4;
                banana_bunch->BananasAvailable += 1;
            }
            break;
        case 4:
            banana_bunch->Counter -= 1;
            if (banana_bunch->Counter == 0) {
                PutSBanana(banana_bunch, Owner, 4);
                banana_bunch->Counter = 4;
                banana_bunch->Sparam = 5;
                banana_bunch->BananasAvailable += 1;
            }
            break;
        case 5:
            banana_bunch->Sparam = 6;
            // Unnecessary Type-casting done here purely to help with understanding.
            // We're setting the ->flags of BananaActors, not plain Actors.
            ((struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[0]])->Flag |= 0x5000;
            ((struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[1]])->Flag |= 0x5000;
            ((struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[2]])->Flag |= 0x5000;
            ((struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[3]])->Flag |= 0x5000;
            ((struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[4]])->Flag |= 0x5000;
            break;
        case 6:
            someCount = 0;
            if (CheckSuperBananaNumber(banana_bunch->BananaIndices[0]) == 1) {
                someCount = 1;
            }
            if (CheckSuperBananaNumber(banana_bunch->BananaIndices[1]) == 1) {
                someCount += 1;
            }
            if (CheckSuperBananaNumber(banana_bunch->BananaIndices[2]) == 1) {
                someCount += 1;
            }
            if (CheckSuperBananaNumber(banana_bunch->BananaIndices[3]) == 1) {
                someCount += 1;
            }
            if (CheckSuperBananaNumber(banana_bunch->BananaIndices[4]) == 1) {
                someCount += 1;
            }
            if (someCount == 0) {
                DeleteObjectBuffer((Object*) banana_bunch);
                Owner->Weapon &= ~USE_BANANA;
            } else if ((Owner->Flag & IS_PLAYER) != 0) {
                controller = &Controllers[banana_bunch->PlayerId];
                if ((controller->ButtonPressed & Z_TRIG) != 0) {
                    controller->ButtonPressed &= ~Z_TRIG;
                    NAPlyTrgStart(Owner - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    if ((controller->AnalogY >= 0x1F) &&
                        ((controller->AnalogX < 0x28) && (controller->AnalogX >= -0x27))) {
                        ThrowBanana(controller->AnalogY, banana_bunch, Owner);
                    } else {
                        ReleaseBanana(banana_bunch);
                    }
                }
            }
            break;
        default:
            break;
    }
}

bool CheckSuperShellNumber(s16 arg0) {
    struct ShellActor* actor;
    if (arg0 < 0) {
        return false;
    }
    actor = (struct ShellActor*) &SimpleObjectArray[arg0];
    if (actor->Category == ACTOR_GREEN_SHELL) {
        if (actor->Sparam == TRIPLE_GREEN_SHELL) {
            return true;
        }
        return false;
    }
    if (actor->Sparam == TRIPLE_RED_SHELL) {
        return true;
    }
    return false;
}

void SuperShellStrategy(TripleShellParent* parent, s16 shellType) {
    UNUSED s32 pad[2];
    s16 PlayerId;
    UNUSED s32 Pad2;
    struct ShellActor* shell;
    Vec3f someVelocity;
    UNUSED s32 Pad3;
    s16 shellCount;
    u16 someRotAngle;
    Player* Player;

    PlayerId = parent->PlayerId;
    Player = &GamePlayers[PlayerId];
    parent->RotAngle += parent->RotVelocity;
    someRotAngle = parent->RotAngle;
    switch (parent->Sparam) {
        case SPAWN_FIRST_SHELL:
            if (SetRollingShell(parent, &GamePlayers[PlayerId], shellType, 0U) != -1) {
                NAPlyTrgStart(PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                parent->ShellsAvailable += 1;
            }
            parent->Sparam = SPAWN_SECOND_SHELL;
            break;
        case SPAWN_SECOND_SHELL:
            if (parent->RotVelocity > 0) {
                if (someRotAngle > DEGREES(300)) {
                    if (SetRollingShell(parent, &GamePlayers[PlayerId], shellType, 1U) != -1) {
                        NAPlyTrgStart(PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->ShellsAvailable += 1;
                    }
                    parent->Sparam = SPAWN_THIRD_SHELL;
                }
            } else {
                if (someRotAngle < DEGREES(60)) {
                    if (SetRollingShell(parent, &GamePlayers[PlayerId], shellType, 1U) != -1) {
                        NAPlyTrgStart(PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->ShellsAvailable += 1;
                    }
                    parent->Sparam = SPAWN_THIRD_SHELL;
                }
            }
            break;
        case SPAWN_THIRD_SHELL:
            if (parent->RotVelocity > 0) {
                if ((someRotAngle > DEGREES(60)) && (someRotAngle < DEGREES(70))) {
                    if (SetRollingShell(parent, &GamePlayers[PlayerId], shellType, 2U) != -1) {
                        NAPlyTrgStart(PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->ShellsAvailable += 1;
                    }
                    parent->Sparam = 3;
                }
            } else if ((someRotAngle < DEGREES(300)) && (someRotAngle > DEGREES(290))) {
                if (SetRollingShell(parent, &GamePlayers[PlayerId], shellType, 2U) != -1) {
                    NAPlyTrgStart(PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    parent->ShellsAvailable += 1;
                }
                parent->Sparam = 3;
            }
            break;
        case 3:
            parent->Sparam = 4;
            shell = (struct ShellActor*) &SimpleObjectArray[(s16) parent->ShellIndices[0]];
            shell->Flag |= HITOBJ;
            shell = (struct ShellActor*) &SimpleObjectArray[(s16) parent->ShellIndices[1]];
            shell->Flag |= HITOBJ;
            shell = (struct ShellActor*) &SimpleObjectArray[(s16) parent->ShellIndices[2]];
            shell->Flag |= HITOBJ;
            break;
        case 4:
            shellCount = 0;
            if (CheckSuperShellNumber(parent->ShellIndices[0]) == 1) {
                shellCount = 1;
            } else {
                parent->ShellIndices[0] = -1.0f;
            }
            if (CheckSuperShellNumber(parent->ShellIndices[1]) == 1) {
                shellCount++;
            } else {
                parent->ShellIndices[1] = -1.0f;
            }
            if (CheckSuperShellNumber(parent->ShellIndices[2]) == 1) {
                shellCount++;
            } else {
                parent->ShellIndices[2] = -1.0f;
            }
            if (shellCount == 0) {
                DeleteObjectBuffer((Object*) parent);
                break;
            }
            if ((Controllers[parent->PlayerId].ButtonPressed & Z_TRIG) != 0) {
                parent->Fparam += 1.0f;
                Controllers[parent->PlayerId].ButtonPressed &= ~Z_TRIG;
            }
            if (parent->Fparam > 0.0f) {
                if (parent->ShellIndices[0] > 0.0f) {
                    shell = (struct ShellActor*) &SimpleObjectArray[(s16) parent->ShellIndices[0]];
                    /**
                     * Forces shell 1 to only fire inside a cone of -5 to +5 degrees in-front of the Player.
                     * However, always evaluates to true, so the feature does not appear during gameplay.
                     * If Enabled, pressing Z would not immediately release the shell. Instead,
                     * it waits until it's inside the cone. (See shell 2 and 3 farther down for same issue)
                     */
                    if ((shell->RotAngle < DEGREES(5)) || (shell->RotAngle > -DEGREES(5))) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        MakeAlignVector(someVelocity, Player->Direction[1] + Player->Slipang);
                        shell->Velocity[0] = someVelocity[0];
                        shell->Velocity[1] = someVelocity[1];
                        shell->Velocity[2] = someVelocity[2];
                        shell->Sparam = MOVING_SHELL;
                        shell->SomeTimer = 0x001E;
                        NAPlyTrgStart(parent->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        NAPlyVoiceStart(parent->PlayerId,
                                      (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        if (parent->Category == ACTOR_TRIPLE_RED_SHELL) {
                            KameHibasiraSet(parent->ShellIndices[0]);
                        } else {
                            KameBlueHibasiraSet(parent->ShellIndices[0]);
                        }
                        parent->ShellIndices[0] = -1.0f;
                        parent->ShellsAvailable -= 1;
                        parent->Fparam -= 1.0f;
                        break;
                    }
                }
                if (parent->ShellIndices[1] > 0.0f) {
                    shell = (struct ShellActor*) &SimpleObjectArray[(s16) parent->ShellIndices[1]];
                    /**
                     * Forces shell 2 to fire inside a cone of 5 to 14.95 degrees (always evaluates to true, thus the feature is skipped)
                     * Unclear why it is 14.95 degrees instead of 15. Perhaps just a typo when entering the Value as a s16.
                     */
                    if ((shell->RotAngle < DEGREES(14.95)) || (shell->RotAngle > DEGREES(5))) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        MakeAlignVector(someVelocity, Player->Direction[1] + Player->Slipang);
                        shell->Velocity[0] = someVelocity[0];
                        shell->Velocity[1] = someVelocity[1];
                        shell->Velocity[2] = someVelocity[2];
                        shell->Sparam = MOVING_SHELL;
                        shell->SomeTimer = 0x001E;
                        NAPlyVoiceStart(parent->PlayerId,
                                      (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        NAPlyTrgStart(parent->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        if (parent->Category == ACTOR_TRIPLE_RED_SHELL) {
                            KameHibasiraSet(parent->ShellIndices[1]);
                        } else {
                            KameBlueHibasiraSet(parent->ShellIndices[1]);
                        }
                        parent->ShellIndices[1] = -1.0f;
                        parent->ShellsAvailable -= 1;
                        parent->Fparam -= 1.0f;
                        break;
                    }
                }
                if (parent->ShellIndices[2] > 0.0f) {
                    shell = (struct ShellActor*) &SimpleObjectArray[(s16) parent->ShellIndices[2]];
                    /**
                     * Forces shell 3 to fire inside a cone of -5 to -10 degrees.
                     * However, after testing, 10 should probably be replaced with 14.95 because
                     * the shell does multiple loops before it fires.
                     * Always evaluates to true, thus this feature is skipped.
                     */
                    if ((shell->RotAngle < -DEGREES(5)) || (shell->RotAngle > -DEGREES(10))) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        MakeAlignVector(someVelocity, Player->Direction[1] + Player->Slipang);
                        shell->Velocity[0] = someVelocity[0];
                        shell->Velocity[1] = someVelocity[1];
                        shell->Velocity[2] = someVelocity[2];
                        shell->Sparam = MOVING_SHELL;
                        shell->SomeTimer = 0x001E;
                        NAPlyTrgStart(parent->PlayerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        NAPlyVoiceStart(parent->PlayerId,
                                      (Player->Kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        if (parent->Category == ACTOR_TRIPLE_RED_SHELL) {
                            KameHibasiraSet(parent->ShellIndices[2]);
                        } else {
                            KameBlueHibasiraSet(parent->ShellIndices[2]);
                        }
                        parent->ShellIndices[2] = -1.0f;
                        parent->ShellsAvailable -= 1;
                        parent->Fparam -= 1.0f;
                        break;
                    }
                }
            }
            break;
        default:
            break;
    }
}

s32 SetSuperBanana(Player* Player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos = { 0.0f, 0.0f, 0.0f };
    s16 ActorIndex;
    struct BananaBunchParent* bananaBunch;

    // this have a animation of spawning so see SuperBananaStrategy
    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_BANANA_BUNCH);
    if (ActorIndex < 0) {
        return ActorIndex;
    }
    bananaBunch = (struct BananaBunchParent*) &SimpleObjectArray[ActorIndex];
    bananaBunch->Sparam = 0;
    bananaBunch->PlayerId = Player - PlayerOne;
    Player->Weapon |= USE_BANANA;
    return ActorIndex;
}

s32 SetSuperShell(Player* Player, s16 tripleShellType) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos = { 0.0f, 0.0f, 0.0f };
    s16 ActorIndex;
    TripleShellParent* parent;

    // this have a animation of spawning so see SuperShellStrategy
    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, tripleShellType);
    if (ActorIndex < 0) {
        return ActorIndex;
    }
    parent = (TripleShellParent*) &SimpleObjectArray[ActorIndex];
    parent->Sparam = 0;
    parent->RotVelocity = DEGREES(8);
    parent->RotAngle = - DEGREES(180);
    parent->PlayerId = Player - PlayerOne;
    parent->ShellsAvailable = 0;
    parent->Fparam = 0.0f;
    return ActorIndex;
}

s32 SetRollingShell(TripleShellParent* parent, Player* Player, s16 shellType, u16 shellId) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 ActorIndex;
    struct ShellActor* shell;

    startingPos[0] = 0.0f;
    startingPos[1] = -Player->Radius;
    startingPos[2] = Player->Radius - 4.0f;
    MultipleMatrixByVector(startingPos, Player->align);
    startingPos[0] += Player->Position[0];
    startingPos[1] += Player->Position[1];
    startingPos[2] += Player->Position[2];

    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, shellType);
    if (ActorIndex < 0) {
        parent->ShellIndices[shellId] = -1.0f;
        return -1;
    }

    shell = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
    startingPos[0] = Player->Position[0];
    startingPos[1] = Player->Position[1];
    startingPos[2] = Player->Position[2];
    CheckBump2(&shell->Bump, shell->Radius + 1.0f, shell->Position[0], shell->Position[1], shell->Position[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    BumpObject((Object*) shell);
    shell->Flag = 0x9000;
    switch (shellType) {
        case ACTOR_GREEN_SHELL:
            shell->Sparam = TRIPLE_GREEN_SHELL;
            break;
        case ACTOR_RED_SHELL:
            shell->Sparam = TRIPLE_RED_SHELL;
            break;
    }
    shell->RotVelocity = 0;
    shell->RotAngle = -DEGREES(180);
    shell->PlayerId = Player - PlayerOne;
    shell->ParentIndex = (Object*) parent - SimpleObjectArray;
    shell->shellId = shellId;
    parent->ShellIndices[shellId] = (Object*) shell - SimpleObjectArray;
    return 1;
}

s32 SetGshell(Player* Player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 ActorIndex;
    struct ShellActor* shell;

    // place behind Player
    startingPos[0] = 0.0f;
    startingPos[1] = -Player->Radius;
    startingPos[2] = Player->Radius - 4.0f;

    // rotate to match Player orientation
    MultipleMatrixByVector(startingPos, Player->align);

    // move to Player Position
    startingPos[0] += Player->Position[0];
    startingPos[1] += Player->Position[1];
    startingPos[2] += Player->Position[2];

    // spawn the shell
    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_GREEN_SHELL);
    if (ActorIndex < 0) {
        return ActorIndex;
    }

    shell = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
    startingPos[0] = Player->Position[0];
    startingPos[1] = Player->Position[1];
    startingPos[2] = Player->Position[2];
    CheckBump2(&shell->Bump, shell->Radius + 1.0f, shell->Position[0], shell->Position[1], shell->Position[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    BumpObject((Object*) shell);
    shell->Sparam = HELD_SHELL;
    shell->RotVelocity = 0;
    shell->RotAngle = -DEGREES(180);
    shell->PlayerId = Player - PlayerOne;
    return ActorIndex;
}

s32 SetRshell(Player* Player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 ActorIndex;
    struct ShellActor* shell;

    // place behind Player
    startingPos[0] = 0.0f;
    startingPos[1] = -Player->Radius;
    startingPos[2] = Player->Radius - 4.0f;

    // rotate to match Player orientation
    MultipleMatrixByVector(startingPos, Player->align);

    // move to Player Position
    startingPos[0] += Player->Position[0];
    startingPos[1] += Player->Position[1];
    startingPos[2] += Player->Position[2];

    // spawn the shell
    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_RED_SHELL);
    if (ActorIndex < 0) {
        return ActorIndex;
    }

    shell = (struct ShellActor*) &SimpleObjectArray[ActorIndex];
    startingPos[0] = Player->Position[0];
    startingPos[1] = Player->Position[1];
    startingPos[2] = Player->Position[2];
    CheckBump2(&shell->Bump, shell->Radius + 1.0f, shell->Position[0], shell->Position[1], shell->Position[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    BumpObject((Object*) shell);
    shell->Sparam = HELD_SHELL;
    shell->RotVelocity = 0;
    shell->RotAngle = Player->Direction[1] - DEGREES(180);
    shell->PlayerId = Player - PlayerOne;
    return ActorIndex;
}

// Interestingly Blue shells Start their life as a red shell,
// and then just change the Type from red to Blue shell
void SetTshell(Player* Player) {
    SimpleObjectArray[SetRshell(Player)].Category = ACTOR_BLUE_SPINY_SHELL;
}

#include "actors/banana/update.inc.c"

// This function could reasonably be called "spawn_bananas_for_banana_bunch" or similar
void PutSBanana(struct BananaBunchParent* banana_bunch, Player* Player, s16 bananaId) {
    s16 ActorIndex;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;
    UNUSED s32 pad;
    UNUSED s32 Pad2;
    struct BananaActor* newBanana;
    struct BananaActor* tempBanana;

    startingPos[0] = 0.0f;
    startingPos[1] = -Player->Radius;
    startingPos[2] = -(Player->Radius + 4.0f);
    MultipleMatrixByVector(startingPos, Player->align);
    startingPos[0] += Player->Position[0];
    startingPos[1] += Player->Position[1];
    startingPos[2] += Player->Position[2];
    startingVelocity[0] = Player->Velocity[0];
    startingVelocity[1] = Player->Velocity[1];
    startingVelocity[2] = Player->Velocity[2];
    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;
    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_BANANA);
    if (ActorIndex >= 0) {
        newBanana = (struct BananaActor*) &SimpleObjectArray[ActorIndex];
        startingPos[0] = Player->Position[0];
        startingPos[1] = Player->Position[1];
        startingPos[2] = Player->Position[2];
        CheckBump2(&newBanana->Bump, newBanana->Radius + 1.0f, newBanana->Position[0],
                                newBanana->Position[1], newBanana->Position[2], startingPos[0], startingPos[1], startingPos[2]);
        BumpObject((Object*) newBanana);
        newBanana->Flag = 0x9000;
        newBanana->PlayerId = Player - PlayerOne;
        newBanana->ParentIndex = (Object*) banana_bunch - SimpleObjectArray;
        newBanana->YoungerIndex = -1;
        newBanana->Counter = 0x0014;
        newBanana->bananaId = bananaId;
        switch (bananaId) {
            case 0:
                newBanana->Sparam = 2;
                banana_bunch->BananaIndices[0] = ActorIndex;
                newBanana->ElderIndex = -1;
                break;
            case 1:
                newBanana->Sparam = 3;
                banana_bunch->BananaIndices[1] = ActorIndex;
                newBanana->ElderIndex = banana_bunch->BananaIndices[0];
                tempBanana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[0]];
                tempBanana->YoungerIndex = ActorIndex;
                break;
            case 2:
                newBanana->Sparam = 3;
                banana_bunch->BananaIndices[2] = ActorIndex;
                newBanana->ElderIndex = banana_bunch->BananaIndices[1];
                tempBanana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[1]];
                tempBanana->YoungerIndex = ActorIndex;
                break;
            case 3:
                newBanana->Sparam = 3;
                banana_bunch->BananaIndices[3] = ActorIndex;
                newBanana->ElderIndex = banana_bunch->BananaIndices[2];
                tempBanana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[2]];
                tempBanana->YoungerIndex = ActorIndex;
                break;
            case 4:
                newBanana->Sparam = 3;
                banana_bunch->BananaIndices[4] = ActorIndex;
                newBanana->ElderIndex = banana_bunch->BananaIndices[3];
                tempBanana = (struct BananaActor*) &SimpleObjectArray[banana_bunch->BananaIndices[3]];
                tempBanana->YoungerIndex = ActorIndex;
                break;
        }
        if ((Player->Flag & IS_PLAYER) != 0) {
            NAPlyTrgStart(Player - PlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
        }
    }
}

s32 PutFakeItembox(Player* Player) {
    struct FakeItemBox* itemBox;
    UNUSED s32 pad[5];
    s16 ActorIndex;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;

    // place behind Player
    startingPos[0] = 0.0f;
    startingPos[1] = -Player->Radius;
    startingPos[2] = -(Player->Radius + 4.0f);

    // rotate to match Player orientation
    MultipleMatrixByVector(startingPos, Player->align);

    // move to Player Position
    startingPos[0] += Player->Position[0];
    startingPos[1] += Player->Position[1];
    startingPos[2] += Player->Position[2];

    // set the itembox's Velocity to the Player's Velocity
    startingVelocity[0] = Player->Velocity[0];
    startingVelocity[1] = Player->Velocity[1];
    startingVelocity[2] = Player->Velocity[2];

    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;

    // spawn the itembox
    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_FAKE_ITEM_BOX);
    if (ActorIndex < 0) {
        return ActorIndex;
    }
    itemBox = (struct FakeItemBox*) &SimpleObjectArray[ActorIndex];
    itemBox->PlayerId = (Player - PlayerOne);
    itemBox->Sparam = HELD_FAKE_ITEM_BOX;
    Player->Weapon |= USE_BANANA;
    return ActorIndex;
}

s32 PutBanana(Player* Player) {
    UNUSED s32 pad[6];
    u16 PlayerId;
    s16 ActorIndex;
    struct BananaActor* banana;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;

    PlayerId = Player - PlayerOne;
    if (PlayerId >= 8) {
        return -1;
    }
    // Extremely weird fake match to fix a tiny Stack difference
    // place the banana in back of the Player
    startingPos[0, 0] = 0.0f;
    startingPos[1] = -Player->Radius;
    startingPos[2] = -(Player->Radius + 4.0f);

    // apply the Player's orientation to the banana
    MultipleMatrixByVector(startingPos, Player->align);

    // add the Player's Position to the banana's Position
    startingPos[0] += Player->Position[0];
    startingPos[1] += Player->Position[1];
    startingPos[2] += Player->Position[2];

    // set the banana's Velocity to the Player's Velocity
    startingVelocity[0] = Player->Velocity[0];
    startingVelocity[1] = Player->Velocity[1];
    startingVelocity[2] = Player->Velocity[2];
    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;

    ActorIndex = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_BANANA); // spawn banana
    if (ActorIndex < 0) {
        return ActorIndex;
    }
    banana = (struct BananaActor*) &SimpleObjectArray[ActorIndex];
    banana->PlayerId = PlayerId;
    banana->Sparam = HELD_BANANA;
    banana->Counter = 0x0014;
    Player->Weapon |= USE_BANANA;
    return ActorIndex;
}

/**
 * Strikes players with Thunder
 *
 * @Param Activating Player (not to be struck)
 */
void UseThunder(Player* player) {
    s32 Index;
    Player* otherPlayer;

    ThunderWorld();
    if ((player->Flag & IS_PLAYER) != 0) {
        // Play Sound.
        NaPlyThunderStart(player - PlayerOne);
    }

    for (Index = 0; Index < NUM_PLAYERS; Index++) {
        otherPlayer = &GamePlayers[Index];
        if (player != otherPlayer) {
            otherPlayer->Weapon |= HIT_LIGHTNING;
        }
    }
}

// Handles Item use?
void ExecuteItem(Player* Player) {
    s32 PlayerId = Player - PlayerOne;

    switch (Player->Item) {
        case ITEM_GREEN_SHELL:
            SetGshell(Player);
            break;
        case ITEM_RED_SHELL:
            SetRshell(Player);
            break;
        case ITEM_BLUE_SPINY_SHELL:
            SetTshell(Player);
            break;
        case ITEM_BANANA:
            PutBanana(Player);
            break;
        case ITEM_BANANA_BUNCH:
            SetSuperBanana(Player);
            break;
        case ITEM_MUSHROOM:
            Player->Weapon |= USE_MUSHROOM;
            break;
        case ITEM_DOUBLE_MUSHROOM:
            Player->Weapon |= USE_MUSHROOM;
            break;
        case ITEM_TRIPLE_MUSHROOM:
            Player->Weapon |= USE_MUSHROOM;
            break;
        case ITEM_SUPER_MUSHROOM:
            Player->Weapon |= USE_MUSHROOM;
            break;
        case ITEM_BOO:
            Player->Weapon |= USE_BOO;
            break;
        case ITEM_STAR:
            Player->Weapon |= USE_STAR;
            break;
        case ITEM_THUNDERBOLT:
            UseThunder(Player);
            break;
        case ITEM_FAKE_ITEM_BOX:
            PutFakeItembox(Player);
            break;
        case ITEM_TRIPLE_GREEN_SHELL:
            SetSuperShell(Player, ACTOR_TRIPLE_GREEN_SHELL);
            break;
        case ITEM_TRIPLE_RED_SHELL:
            SetSuperShell(Player, ACTOR_TRIPLE_RED_SHELL);
            break;
    }
    KwanmUseditem(PlayerId);
}

// Check if a Player is using an Item?
void ItemCheck(void) {
    Player* Player;
    struct Controller* Target;
    struct Controller* controller;
    struct Controller* loopController;

    for (Player = &GamePlayers[0], loopController = &Controllers[0], Target = &Controllers[4]; loopController != Target;
         Player++, loopController++) {
        controller = loopController;
        if (CheckItembutton(Player) == false) {
            if ((Player->Flag & IS_GHOST) != 0) {
                if ((Player - PlayerTwo) == 0) {
                    controller = ControllerSix;
                } else if ((Player - PlayerThree) == 0) {
                    controller = ControllerSeven;
                } else {
                    if ((Player - PlayerOne) == 0) {
                        controller = ControllerEight;
                    }
                }
            }

            if (((Player->Flag & IS_PLAYER) != 0) && (Player->Item != ITEM_NONE) &&
                ((Player->Flag & NO_CONTROLS) == 0)) {
                if ((controller->ButtonPressed & Z_TRIG) != 0) {
                    controller->ButtonPressed &= ~Z_TRIG;
                    ExecuteItemHookDefault(Player);
                }
            }
        }
    }
}

#include "actors/green_shell/update.inc.c"

#include "actors/blue_and_red_shells/update.inc.c"

void BumpObject(Object* arg0) {
    if ((arg0->Bump.DistanceZX < 0.0f) && (arg0->Bump.FlagZX == 1)) {
        arg0->Position[0] -= (arg0->Bump.BumpZX[0] * arg0->Bump.DistanceZX);
        arg0->Position[1] -= (arg0->Bump.BumpZX[1] * arg0->Bump.DistanceZX);
        arg0->Position[2] -= (arg0->Bump.BumpZX[2] * arg0->Bump.DistanceZX);
    }
    if ((arg0->Bump.DistanceXY < 0.0f) && (arg0->Bump.FlagXY == 1)) {
        arg0->Position[0] -= (arg0->Bump.BumpXY[0] * arg0->Bump.DistanceXY);
        arg0->Position[1] -= (arg0->Bump.BumpXY[1] * arg0->Bump.DistanceXY);
        arg0->Position[2] -= (arg0->Bump.BumpXY[2] * arg0->Bump.DistanceXY);
    }
    if ((arg0->Bump.DistanceYZ < 0.0f) && (arg0->Bump.FlagYZ == 1)) {
        arg0->Position[0] -= (arg0->Bump.BumpYZ[0] * arg0->Bump.DistanceYZ);
        arg0->Position[1] -= (arg0->Bump.BumpYZ[1] * arg0->Bump.DistanceYZ);
        arg0->Position[2] -= (arg0->Bump.BumpYZ[2] * arg0->Bump.DistanceYZ);
    }
}
