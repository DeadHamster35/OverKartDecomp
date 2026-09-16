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

void copy_bump(Collision* src, Collision* dest) {
    dest->flag_xy = src->flag_xy;
    dest->flag_yz = src->flag_yz;
    dest->flag_zx = src->flag_zx;
    dest->last_xy = src->last_xy;
    dest->last_yz = src->last_yz;
    dest->last_zx = src->last_zx;
    dest->distance_xy = src->distance_xy;
    dest->distance_yz = src->distance_yz;
    dest->distance_zx = src->distance_zx;

    CopyVector(dest->bump_xy, src->bump_xy);
    CopyVector(dest->bump_yz, src->bump_yz);
    CopyVector(dest->bump_zx, src->bump_zx);
}

void delete_rolling_shell(struct ShellActor* shell, s32 shellType) {
    TripleShellParent* parent = (TripleShellParent*) &g_SimpleObjectArray[shell->parentIndex];

    parent->shellsAvailable--;

    switch ((s16) shell->shellId) {
        case 0:
            parent->shellIndices[0] = -1.0f;
            break;
        case 1:
            parent->shellIndices[1] = -1.0f;
            break;
        case 2:
            parent->shellIndices[2] = -1.0f;
            break;
    }

    shell->flag = EXISTOBJ; // bitflag
    shell->rotAngle = 0;
    shell->velocity[1] = 3.0f;
    shell->someTimer = 60;

    switch (shellType) {
        case ACTOR_GREEN_SHELL:
            shell->sparam = GREEN_SHELL_HIT_A_RACER;
            break;
        case ACTOR_RED_SHELL:
            shell->sparam = DESTROYED_SHELL;
            break;
    }
}

// Sets velocities for a banana, used when a racer runs into
// a banana bunch.
void crush_banana(struct BananaActor* banana) {
    banana->sparam = DROPPED_BANANA;
    banana->counter = 0x00B4;
    banana->velocity[0] = ((f32) (MakeRandomLimmit(0x00C8) - 0x64) * 0.015);
    banana->velocity[1] = ((f32) MakeRandomLimmit(0x00C8)) * 0.015;
    banana->velocity[2] = ((f32) (MakeRandomLimmit(0x00C8) - 0x64) * 0.015);
}

void clear_backward(s16 bananaIndex) {
    struct BananaActor* banana;

    if (bananaIndex != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[bananaIndex];
        crush_banana(banana);
        clear_backward(banana->youngerIndex);
    }
}

void clear_forward(UNUSED struct BananaActor* arg0, s16 bananaIndex) {
    struct BananaActor* banana;

    if (bananaIndex != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[bananaIndex];
        crush_banana(banana);
        clear_forward(banana, banana->elderIndex);
    }
}

// Handle a banana being ran over while it is still part of a banana bunch
void clean_up_banana(struct BananaActor* banana) {
    struct BananaBunchParent* temp_v0_2;

    clear_backward(banana->youngerIndex);
    clear_forward(banana, banana->elderIndex);
    if ((gPlayers[banana->playerId].flag & IS_PLAYER) != 0) {
        NAPlyTrgStart(banana->playerId, SOUND_ARG_LOAD(0x19, 0x01, 0x90, 0x53));
    }
    banana->flag = -0x8000;
    banana->counter = 0x003C;
    banana->sparam = DESTROYED_BANANA;
    banana->velocity[1] = 3.0f;
    temp_v0_2 = (struct BananaBunchParent*) &g_SimpleObjectArray[banana->parentIndex];
    temp_v0_2->bananaIndices[0] = -1;
    temp_v0_2->bananaIndices[1] = -1;
    temp_v0_2->bananaIndices[2] = -1;
    temp_v0_2->bananaIndices[3] = -1;
    temp_v0_2->bananaIndices[4] = -1;
}

// Drop a banana from a banana bunch?
void release_banana(struct BananaBunchParent* banana_bunch) {
    s16 elderIndex;
    struct BananaActor* banana;

    banana_bunch->bananasAvailable -= 1;
    if (banana_bunch->bananaIndices[4] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[4]];
        banana_bunch->bananaIndices[4] = -1;
    } else if (banana_bunch->bananaIndices[3] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[3]];
        banana_bunch->bananaIndices[3] = -1;
    } else if (banana_bunch->bananaIndices[2] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[2]];
        banana_bunch->bananaIndices[2] = -1;
    } else if (banana_bunch->bananaIndices[1] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[1]];
        banana_bunch->bananaIndices[1] = -1;
    } else if (banana_bunch->bananaIndices[0] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[0]];
        banana_bunch->bananaIndices[0] = -1;
    } else {
        return;
    }

    banana->sparam = DROPPED_BANANA;
    banana->counter = 0x00B4;
    banana->velocity[0] = 0.0f;
    banana->velocity[1] = 1.5f;
    banana->velocity[2] = 0.0f;
    elderIndex = banana->elderIndex;
    if (elderIndex != -1) {
        ((struct BananaActor*) &g_SimpleObjectArray[elderIndex])->youngerIndex = -1;
    }
}

// Drop a banana from a banana bunch and throw it forward?
void throw_banana(s16 AnalogY, struct BananaBunchParent* banana_bunch, Player* player) {
    Vec3f velocity;
    struct BananaActor* banana;
    struct BananaActor* elderBanana;
    f32 var_f0;
    f32 var_f12;

    banana_bunch->bananasAvailable -= 1;
    if (banana_bunch->bananaIndices[4] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[4]];
        banana_bunch->bananaIndices[4] = -1;
    } else if (banana_bunch->bananaIndices[3] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[3]];
        banana_bunch->bananaIndices[3] = -1;
    } else if (banana_bunch->bananaIndices[2] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[2]];
        banana_bunch->bananaIndices[2] = -1;
    } else if (banana_bunch->bananaIndices[1] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[1]];
        banana_bunch->bananaIndices[1] = -1;
    } else if (banana_bunch->bananaIndices[0] != -1) {
        banana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[0]];
        banana_bunch->bananaIndices[0] = -1;
    } else {
        return;
    }

    banana->sparam = DROPPED_BANANA;
    banana->counter = 0x001E;
    if (banana->elderIndex != -1) {
        elderBanana = (struct BananaActor*) &g_SimpleObjectArray[banana->elderIndex];
        elderBanana->youngerIndex = -1;
    }
    if (player->speed < 2.0f) {
        var_f0 = ((AnalogY - 30.0f) / 20.0f) + 1.5f;
        var_f12 = 4.0f;
    } else {
        var_f0 = ((AnalogY - 30.0f) / 20.0f) + 1.5f;
        var_f12 = (player->speed * 0.75f) + 4.5f + var_f0;
    }
    vec3f_set(velocity, 0.0f, var_f0, var_f12);
    MakeAlignVector(velocity, player->direction[1] + player->slipang);
    banana->velocity[0] = velocity[0];
    banana->velocity[1] = velocity[1];
    banana->velocity[2] = velocity[2];
}

s32 check_super_banana_number(s16 bananaId) {
    struct BananaActor* banana;
    if (bananaId == -1) {
        return 0;
    }
    banana = (struct BananaActor*) &g_SimpleObjectArray[bananaId];
    if (banana->sparam == FIRST_BANANA_BUNCH_BANANA) {
        return 1;
    }
    if (banana->sparam == BANANA_BUNCH_BANANA) {
        return 1;
    }
    return 0;
}

void super_banana_strategy(struct BananaBunchParent* banana_bunch) {
    UNUSED s32 pad[2];
    Player* owner;
    struct Controller* controller;
    s32 someCount;

    owner = &gPlayers[banana_bunch->playerId];
    switch (banana_bunch->sparam) {
        case 0:
            put_s_banana(banana_bunch, owner, 0);
            banana_bunch->counter = 4;
            banana_bunch->sparam = 1;
            banana_bunch->bananasAvailable = 1;
            break;
        case 1:
            banana_bunch->counter -= 1;
            if (banana_bunch->counter == 0) {
                put_s_banana(banana_bunch, owner, 1);
                banana_bunch->counter = 4;
                banana_bunch->sparam = 2;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 2:
            banana_bunch->counter -= 1;
            if (banana_bunch->counter == 0) {
                put_s_banana(banana_bunch, owner, 2);
                banana_bunch->counter = 4;
                banana_bunch->sparam = 3;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 3:
            banana_bunch->counter -= 1;
            if (banana_bunch->counter == 0) {
                put_s_banana(banana_bunch, owner, 3);
                banana_bunch->counter = 4;
                banana_bunch->sparam = 4;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 4:
            banana_bunch->counter -= 1;
            if (banana_bunch->counter == 0) {
                put_s_banana(banana_bunch, owner, 4);
                banana_bunch->counter = 4;
                banana_bunch->sparam = 5;
                banana_bunch->bananasAvailable += 1;
            }
            break;
        case 5:
            banana_bunch->sparam = 6;
            // Unnecessary type-casting done here purely to help with understanding.
            // We're setting the ->flags of BananaActors, not plain Actors.
            ((struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[0]])->flag |= 0x5000;
            ((struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[1]])->flag |= 0x5000;
            ((struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[2]])->flag |= 0x5000;
            ((struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[3]])->flag |= 0x5000;
            ((struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[4]])->flag |= 0x5000;
            break;
        case 6:
            someCount = 0;
            if (check_super_banana_number(banana_bunch->bananaIndices[0]) == 1) {
                someCount = 1;
            }
            if (check_super_banana_number(banana_bunch->bananaIndices[1]) == 1) {
                someCount += 1;
            }
            if (check_super_banana_number(banana_bunch->bananaIndices[2]) == 1) {
                someCount += 1;
            }
            if (check_super_banana_number(banana_bunch->bananaIndices[3]) == 1) {
                someCount += 1;
            }
            if (check_super_banana_number(banana_bunch->bananaIndices[4]) == 1) {
                someCount += 1;
            }
            if (someCount == 0) {
                deleteObjectBuffer((Object*) banana_bunch);
                owner->weapon &= ~USE_BANANA;
            } else if ((owner->flag & IS_PLAYER) != 0) {
                controller = &gControllers[banana_bunch->playerId];
                if ((controller->ButtonPressed & Z_TRIG) != 0) {
                    controller->ButtonPressed &= ~Z_TRIG;
                    NAPlyTrgStart(owner - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    if ((controller->AnalogY >= 0x1F) &&
                        ((controller->AnalogX < 0x28) && (controller->AnalogX >= -0x27))) {
                        throw_banana(controller->AnalogY, banana_bunch, owner);
                    } else {
                        release_banana(banana_bunch);
                    }
                }
            }
            break;
        default:
            break;
    }
}

bool check_super_shell_number(s16 arg0) {
    struct ShellActor* actor;
    if (arg0 < 0) {
        return false;
    }
    actor = (struct ShellActor*) &g_SimpleObjectArray[arg0];
    if (actor->category == ACTOR_GREEN_SHELL) {
        if (actor->sparam == TRIPLE_GREEN_SHELL) {
            return true;
        }
        return false;
    }
    if (actor->sparam == TRIPLE_RED_SHELL) {
        return true;
    }
    return false;
}

void super_shell_strategy(TripleShellParent* parent, s16 shellType) {
    UNUSED s32 pad[2];
    s16 playerId;
    UNUSED s32 pad2;
    struct ShellActor* shell;
    Vec3f someVelocity;
    UNUSED s32 pad3;
    s16 shellCount;
    u16 someRotAngle;
    Player* player;

    playerId = parent->playerId;
    player = &gPlayers[playerId];
    parent->rotAngle += parent->rotVelocity;
    someRotAngle = parent->rotAngle;
    switch (parent->sparam) {
        case SPAWN_FIRST_SHELL:
            if (set_rolling_shell(parent, &gPlayers[playerId], shellType, 0U) != -1) {
                NAPlyTrgStart(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                parent->shellsAvailable += 1;
            }
            parent->sparam = SPAWN_SECOND_SHELL;
            break;
        case SPAWN_SECOND_SHELL:
            if (parent->rotVelocity > 0) {
                if (someRotAngle > DEGREES(300)) {
                    if (set_rolling_shell(parent, &gPlayers[playerId], shellType, 1U) != -1) {
                        NAPlyTrgStart(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->shellsAvailable += 1;
                    }
                    parent->sparam = SPAWN_THIRD_SHELL;
                }
            } else {
                if (someRotAngle < DEGREES(60)) {
                    if (set_rolling_shell(parent, &gPlayers[playerId], shellType, 1U) != -1) {
                        NAPlyTrgStart(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->shellsAvailable += 1;
                    }
                    parent->sparam = SPAWN_THIRD_SHELL;
                }
            }
            break;
        case SPAWN_THIRD_SHELL:
            if (parent->rotVelocity > 0) {
                if ((someRotAngle > DEGREES(60)) && (someRotAngle < DEGREES(70))) {
                    if (set_rolling_shell(parent, &gPlayers[playerId], shellType, 2U) != -1) {
                        NAPlyTrgStart(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                        parent->shellsAvailable += 1;
                    }
                    parent->sparam = 3;
                }
            } else if ((someRotAngle < DEGREES(300)) && (someRotAngle > DEGREES(290))) {
                if (set_rolling_shell(parent, &gPlayers[playerId], shellType, 2U) != -1) {
                    NAPlyTrgStart(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                    parent->shellsAvailable += 1;
                }
                parent->sparam = 3;
            }
            break;
        case 3:
            parent->sparam = 4;
            shell = (struct ShellActor*) &g_SimpleObjectArray[(s16) parent->shellIndices[0]];
            shell->flag |= HITOBJ;
            shell = (struct ShellActor*) &g_SimpleObjectArray[(s16) parent->shellIndices[1]];
            shell->flag |= HITOBJ;
            shell = (struct ShellActor*) &g_SimpleObjectArray[(s16) parent->shellIndices[2]];
            shell->flag |= HITOBJ;
            break;
        case 4:
            shellCount = 0;
            if (check_super_shell_number(parent->shellIndices[0]) == 1) {
                shellCount = 1;
            } else {
                parent->shellIndices[0] = -1.0f;
            }
            if (check_super_shell_number(parent->shellIndices[1]) == 1) {
                shellCount++;
            } else {
                parent->shellIndices[1] = -1.0f;
            }
            if (check_super_shell_number(parent->shellIndices[2]) == 1) {
                shellCount++;
            } else {
                parent->shellIndices[2] = -1.0f;
            }
            if (shellCount == 0) {
                deleteObjectBuffer((Object*) parent);
                break;
            }
            if ((gControllers[parent->playerId].ButtonPressed & Z_TRIG) != 0) {
                parent->fparam += 1.0f;
                gControllers[parent->playerId].ButtonPressed &= ~Z_TRIG;
            }
            if (parent->fparam > 0.0f) {
                if (parent->shellIndices[0] > 0.0f) {
                    shell = (struct ShellActor*) &g_SimpleObjectArray[(s16) parent->shellIndices[0]];
                    /**
                     * Forces shell 1 to only fire inside a cone of -5 to +5 degrees in-front of the player.
                     * However, always evaluates to true, so the feature does not appear during gameplay.
                     * If enabled, pressing Z would not immediately release the shell. Instead,
                     * it waits until it's inside the cone. (See shell 2 and 3 farther down for same issue)
                     */
                    if ((shell->rotAngle < DEGREES(5)) || (shell->rotAngle > -DEGREES(5))) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        MakeAlignVector(someVelocity, player->direction[1] + player->slipang);
                        shell->velocity[0] = someVelocity[0];
                        shell->velocity[1] = someVelocity[1];
                        shell->velocity[2] = someVelocity[2];
                        shell->sparam = MOVING_SHELL;
                        shell->someTimer = 0x001E;
                        NAPlyTrgStart(parent->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        NAPlyVoiceStart(parent->playerId,
                                      (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        if (parent->category == ACTOR_TRIPLE_RED_SHELL) {
                            kame_hibasira_set(parent->shellIndices[0]);
                        } else {
                            kame_blue_hibasira_set(parent->shellIndices[0]);
                        }
                        parent->shellIndices[0] = -1.0f;
                        parent->shellsAvailable -= 1;
                        parent->fparam -= 1.0f;
                        break;
                    }
                }
                if (parent->shellIndices[1] > 0.0f) {
                    shell = (struct ShellActor*) &g_SimpleObjectArray[(s16) parent->shellIndices[1]];
                    /**
                     * Forces shell 2 to fire inside a cone of 5 to 14.95 degrees (always evaluates to true, thus the feature is skipped)
                     * Unclear why it is 14.95 degrees instead of 15. Perhaps just a typo when entering the value as a s16.
                     */
                    if ((shell->rotAngle < DEGREES(14.95)) || (shell->rotAngle > DEGREES(5))) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        MakeAlignVector(someVelocity, player->direction[1] + player->slipang);
                        shell->velocity[0] = someVelocity[0];
                        shell->velocity[1] = someVelocity[1];
                        shell->velocity[2] = someVelocity[2];
                        shell->sparam = MOVING_SHELL;
                        shell->someTimer = 0x001E;
                        NAPlyVoiceStart(parent->playerId,
                                      (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        NAPlyTrgStart(parent->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        if (parent->category == ACTOR_TRIPLE_RED_SHELL) {
                            kame_hibasira_set(parent->shellIndices[1]);
                        } else {
                            kame_blue_hibasira_set(parent->shellIndices[1]);
                        }
                        parent->shellIndices[1] = -1.0f;
                        parent->shellsAvailable -= 1;
                        parent->fparam -= 1.0f;
                        break;
                    }
                }
                if (parent->shellIndices[2] > 0.0f) {
                    shell = (struct ShellActor*) &g_SimpleObjectArray[(s16) parent->shellIndices[2]];
                    /**
                     * Forces shell 3 to fire inside a cone of -5 to -10 degrees.
                     * However, after testing, 10 should probably be replaced with 14.95 because
                     * the shell does multiple loops before it fires.
                     * Always evaluates to true, thus this feature is skipped.
                     */
                    if ((shell->rotAngle < -DEGREES(5)) || (shell->rotAngle > -DEGREES(10))) {
                        someVelocity[0] = 0;
                        someVelocity[1] = 0;
                        someVelocity[2] = 8;
                        MakeAlignVector(someVelocity, player->direction[1] + player->slipang);
                        shell->velocity[0] = someVelocity[0];
                        shell->velocity[1] = someVelocity[1];
                        shell->velocity[2] = someVelocity[2];
                        shell->sparam = MOVING_SHELL;
                        shell->someTimer = 0x001E;
                        NAPlyTrgStart(parent->playerId, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x04));
                        NAPlyVoiceStart(parent->playerId,
                                      (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x00));
                        if (parent->category == ACTOR_TRIPLE_RED_SHELL) {
                            kame_hibasira_set(parent->shellIndices[2]);
                        } else {
                            kame_blue_hibasira_set(parent->shellIndices[2]);
                        }
                        parent->shellIndices[2] = -1.0f;
                        parent->shellsAvailable -= 1;
                        parent->fparam -= 1.0f;
                        break;
                    }
                }
            }
            break;
        default:
            break;
    }
}

s32 set_super_banana(Player* player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos = { 0.0f, 0.0f, 0.0f };
    s16 actorIndex;
    struct BananaBunchParent* bananaBunch;

    // this have a animation of spawning so see super_banana_strategy
    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_BANANA_BUNCH);
    if (actorIndex < 0) {
        return actorIndex;
    }
    bananaBunch = (struct BananaBunchParent*) &g_SimpleObjectArray[actorIndex];
    bananaBunch->sparam = 0;
    bananaBunch->playerId = player - gPlayerOne;
    player->weapon |= USE_BANANA;
    return actorIndex;
}

s32 set_super_shell(Player* player, s16 tripleShellType) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos = { 0.0f, 0.0f, 0.0f };
    s16 actorIndex;
    TripleShellParent* parent;

    // this have a animation of spawning so see super_shell_strategy
    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, tripleShellType);
    if (actorIndex < 0) {
        return actorIndex;
    }
    parent = (TripleShellParent*) &g_SimpleObjectArray[actorIndex];
    parent->sparam = 0;
    parent->rotVelocity = DEGREES(8);
    parent->rotAngle = - DEGREES(180);
    parent->playerId = player - gPlayerOne;
    parent->shellsAvailable = 0;
    parent->fparam = 0.0f;
    return actorIndex;
}

s32 set_rolling_shell(TripleShellParent* parent, Player* player, s16 shellType, u16 shellId) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 actorIndex;
    struct ShellActor* shell;

    startingPos[0] = 0.0f;
    startingPos[1] = -player->radius;
    startingPos[2] = player->radius - 4.0f;
    MultipleMatrixByVector(startingPos, player->align);
    startingPos[0] += player->position[0];
    startingPos[1] += player->position[1];
    startingPos[2] += player->position[2];

    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, shellType);
    if (actorIndex < 0) {
        parent->shellIndices[shellId] = -1.0f;
        return -1;
    }

    shell = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
    startingPos[0] = player->position[0];
    startingPos[1] = player->position[1];
    startingPos[2] = player->position[2];
    CheckBump2(&shell->bump, shell->radius + 1.0f, shell->position[0], shell->position[1], shell->position[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    BumpObject((Object*) shell);
    shell->flag = 0x9000;
    switch (shellType) {
        case ACTOR_GREEN_SHELL:
            shell->sparam = TRIPLE_GREEN_SHELL;
            break;
        case ACTOR_RED_SHELL:
            shell->sparam = TRIPLE_RED_SHELL;
            break;
    }
    shell->rotVelocity = 0;
    shell->rotAngle = -DEGREES(180);
    shell->playerId = player - gPlayerOne;
    shell->parentIndex = (Object*) parent - g_SimpleObjectArray;
    shell->shellId = shellId;
    parent->shellIndices[shellId] = (Object*) shell - g_SimpleObjectArray;
    return 1;
}

s32 set_gshell(Player* player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 actorIndex;
    struct ShellActor* shell;

    // place behind player
    startingPos[0] = 0.0f;
    startingPos[1] = -player->radius;
    startingPos[2] = player->radius - 4.0f;

    // rotate to match player orientation
    MultipleMatrixByVector(startingPos, player->align);

    // move to player position
    startingPos[0] += player->position[0];
    startingPos[1] += player->position[1];
    startingPos[2] += player->position[2];

    // spawn the shell
    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_GREEN_SHELL);
    if (actorIndex < 0) {
        return actorIndex;
    }

    shell = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
    startingPos[0] = player->position[0];
    startingPos[1] = player->position[1];
    startingPos[2] = player->position[2];
    CheckBump2(&shell->bump, shell->radius + 1.0f, shell->position[0], shell->position[1], shell->position[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    BumpObject((Object*) shell);
    shell->sparam = HELD_SHELL;
    shell->rotVelocity = 0;
    shell->rotAngle = -DEGREES(180);
    shell->playerId = player - gPlayerOne;
    return actorIndex;
}

s32 set_rshell(Player* player) {
    Vec3f startingVelocity = { 0.0f, 0.0f, 0.0f };
    Vec3s startingRot = { 0, 0, 0 };
    Vec3f startingPos;
    s16 actorIndex;
    struct ShellActor* shell;

    // place behind player
    startingPos[0] = 0.0f;
    startingPos[1] = -player->radius;
    startingPos[2] = player->radius - 4.0f;

    // rotate to match player orientation
    MultipleMatrixByVector(startingPos, player->align);

    // move to player position
    startingPos[0] += player->position[0];
    startingPos[1] += player->position[1];
    startingPos[2] += player->position[2];

    // spawn the shell
    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_RED_SHELL);
    if (actorIndex < 0) {
        return actorIndex;
    }

    shell = (struct ShellActor*) &g_SimpleObjectArray[actorIndex];
    startingPos[0] = player->position[0];
    startingPos[1] = player->position[1];
    startingPos[2] = player->position[2];
    CheckBump2(&shell->bump, shell->radius + 1.0f, shell->position[0], shell->position[1], shell->position[2],
                            startingPos[0], startingPos[1], startingPos[2]);
    BumpObject((Object*) shell);
    shell->sparam = HELD_SHELL;
    shell->rotVelocity = 0;
    shell->rotAngle = player->direction[1] - DEGREES(180);
    shell->playerId = player - gPlayerOne;
    return actorIndex;
}

// Interestingly blue shells start their life as a red shell,
// and then just change the type from red to blue shell
void set_tshell(Player* player) {
    g_SimpleObjectArray[set_rshell(player)].category = ACTOR_BLUE_SPINY_SHELL;
}

#include "actors/banana/update.inc.c"

// This function could reasonably be called "spawn_bananas_for_banana_bunch" or similar
void put_s_banana(struct BananaBunchParent* banana_bunch, Player* player, s16 bananaId) {
    s16 actorIndex;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;
    UNUSED s32 pad;
    UNUSED s32 pad2;
    struct BananaActor* newBanana;
    struct BananaActor* tempBanana;

    startingPos[0] = 0.0f;
    startingPos[1] = -player->radius;
    startingPos[2] = -(player->radius + 4.0f);
    MultipleMatrixByVector(startingPos, player->align);
    startingPos[0] += player->position[0];
    startingPos[1] += player->position[1];
    startingPos[2] += player->position[2];
    startingVelocity[0] = player->velocity[0];
    startingVelocity[1] = player->velocity[1];
    startingVelocity[2] = player->velocity[2];
    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;
    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_BANANA);
    if (actorIndex >= 0) {
        newBanana = (struct BananaActor*) &g_SimpleObjectArray[actorIndex];
        startingPos[0] = player->position[0];
        startingPos[1] = player->position[1];
        startingPos[2] = player->position[2];
        CheckBump2(&newBanana->bump, newBanana->radius + 1.0f, newBanana->position[0],
                                newBanana->position[1], newBanana->position[2], startingPos[0], startingPos[1], startingPos[2]);
        BumpObject((Object*) newBanana);
        newBanana->flag = 0x9000;
        newBanana->playerId = player - gPlayerOne;
        newBanana->parentIndex = (Object*) banana_bunch - g_SimpleObjectArray;
        newBanana->youngerIndex = -1;
        newBanana->counter = 0x0014;
        newBanana->bananaId = bananaId;
        switch (bananaId) {
            case 0:
                newBanana->sparam = 2;
                banana_bunch->bananaIndices[0] = actorIndex;
                newBanana->elderIndex = -1;
                break;
            case 1:
                newBanana->sparam = 3;
                banana_bunch->bananaIndices[1] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[0];
                tempBanana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[0]];
                tempBanana->youngerIndex = actorIndex;
                break;
            case 2:
                newBanana->sparam = 3;
                banana_bunch->bananaIndices[2] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[1];
                tempBanana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[1]];
                tempBanana->youngerIndex = actorIndex;
                break;
            case 3:
                newBanana->sparam = 3;
                banana_bunch->bananaIndices[3] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[2];
                tempBanana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[2]];
                tempBanana->youngerIndex = actorIndex;
                break;
            case 4:
                newBanana->sparam = 3;
                banana_bunch->bananaIndices[4] = actorIndex;
                newBanana->elderIndex = banana_bunch->bananaIndices[3];
                tempBanana = (struct BananaActor*) &g_SimpleObjectArray[banana_bunch->bananaIndices[3]];
                tempBanana->youngerIndex = actorIndex;
                break;
        }
        if ((player->flag & IS_PLAYER) != 0) {
            NAPlyTrgStart(player - gPlayerOne, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
        }
    }
}

s32 put_fake_itembox(Player* player) {
    struct FakeItemBox* itemBox;
    UNUSED s32 pad[5];
    s16 actorIndex;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;

    // place behind player
    startingPos[0] = 0.0f;
    startingPos[1] = -player->radius;
    startingPos[2] = -(player->radius + 4.0f);

    // rotate to match player orientation
    MultipleMatrixByVector(startingPos, player->align);

    // move to player position
    startingPos[0] += player->position[0];
    startingPos[1] += player->position[1];
    startingPos[2] += player->position[2];

    // set the itembox's velocity to the player's velocity
    startingVelocity[0] = player->velocity[0];
    startingVelocity[1] = player->velocity[1];
    startingVelocity[2] = player->velocity[2];

    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;

    // spawn the itembox
    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_FAKE_ITEM_BOX);
    if (actorIndex < 0) {
        return actorIndex;
    }
    itemBox = (struct FakeItemBox*) &g_SimpleObjectArray[actorIndex];
    itemBox->playerId = (player - gPlayerOne);
    itemBox->sparam = HELD_FAKE_ITEM_BOX;
    player->weapon |= USE_BANANA;
    return actorIndex;
}

s32 put_banana(Player* player) {
    UNUSED s32 pad[6];
    u16 playerId;
    s16 actorIndex;
    struct BananaActor* banana;
    Vec3f startingVelocity;
    Vec3s startingRot;
    Vec3f startingPos;

    playerId = player - gPlayerOne;
    if (playerId >= 8) {
        return -1;
    }
    // Extremely weird fake match to fix a tiny stack difference
    // place the banana in back of the player
    startingPos[0, 0] = 0.0f;
    startingPos[1] = -player->radius;
    startingPos[2] = -(player->radius + 4.0f);

    // apply the player's orientation to the banana
    MultipleMatrixByVector(startingPos, player->align);

    // add the player's position to the banana's position
    startingPos[0] += player->position[0];
    startingPos[1] += player->position[1];
    startingPos[2] += player->position[2];

    // set the banana's velocity to the player's velocity
    startingVelocity[0] = player->velocity[0];
    startingVelocity[1] = player->velocity[1];
    startingVelocity[2] = player->velocity[2];
    startingRot[0] = 0;
    startingRot[1] = 0;
    startingRot[2] = 0;

    actorIndex = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_BANANA); // spawn banana
    if (actorIndex < 0) {
        return actorIndex;
    }
    banana = (struct BananaActor*) &g_SimpleObjectArray[actorIndex];
    banana->playerId = playerId;
    banana->sparam = HELD_BANANA;
    banana->counter = 0x0014;
    player->weapon |= USE_BANANA;
    return actorIndex;
}

/**
 * Strikes players with thunder
 *
 * @param Activating player (not to be struck)
 */
void use_thunder(Player* player) {
    s32 index;
    Player* otherPlayer;

    ThunderWorld();
    if ((player->flag & IS_PLAYER) != 0) {
        // Play sound.
        Na_PlyThunder_Start(player - gPlayerOne);
    }

    for (index = 0; index < NUM_PLAYERS; index++) {
        otherPlayer = &gPlayers[index];
        if (player != otherPlayer) {
            otherPlayer->weapon |= HIT_LIGHTNING;
        }
    }
}

// Handles item use?
void ExecuteItem(Player* player) {
    s32 playerId = player - gPlayerOne;

    switch (player->item) {
        case ITEM_GREEN_SHELL:
            set_gshell(player);
            break;
        case ITEM_RED_SHELL:
            set_rshell(player);
            break;
        case ITEM_BLUE_SPINY_SHELL:
            set_tshell(player);
            break;
        case ITEM_BANANA:
            put_banana(player);
            break;
        case ITEM_BANANA_BUNCH:
            set_super_banana(player);
            break;
        case ITEM_MUSHROOM:
            player->weapon |= USE_MUSHROOM;
            break;
        case ITEM_DOUBLE_MUSHROOM:
            player->weapon |= USE_MUSHROOM;
            break;
        case ITEM_TRIPLE_MUSHROOM:
            player->weapon |= USE_MUSHROOM;
            break;
        case ITEM_SUPER_MUSHROOM:
            player->weapon |= USE_MUSHROOM;
            break;
        case ITEM_BOO:
            player->weapon |= USE_BOO;
            break;
        case ITEM_STAR:
            player->weapon |= USE_STAR;
            break;
        case ITEM_THUNDERBOLT:
            use_thunder(player);
            break;
        case ITEM_FAKE_ITEM_BOX:
            put_fake_itembox(player);
            break;
        case ITEM_TRIPLE_GREEN_SHELL:
            set_super_shell(player, ACTOR_TRIPLE_GREEN_SHELL);
            break;
        case ITEM_TRIPLE_RED_SHELL:
            set_super_shell(player, ACTOR_TRIPLE_RED_SHELL);
            break;
    }
    kwanm_useditem(playerId);
}

// Check if a player is using an item?
void item_check(void) {
    Player* player;
    struct Controller* target;
    struct Controller* controller;
    struct Controller* loopController;

    for (player = &gPlayers[0], loopController = &gControllers[0], target = &gControllers[4]; loopController != target;
         player++, loopController++) {
        controller = loopController;
        if (check_itembutton(player) == false) {
            if ((player->flag & IS_GHOST) != 0) {
                if ((player - gPlayerTwo) == 0) {
                    controller = gControllerSix;
                } else if ((player - gPlayerThree) == 0) {
                    controller = gControllerSeven;
                } else {
                    if ((player - gPlayerOne) == 0) {
                        controller = gControllerEight;
                    }
                }
            }

            if (((player->flag & IS_PLAYER) != 0) && (player->item != ITEM_NONE) &&
                ((player->flag & NO_CONTROLS) == 0)) {
                if ((controller->ButtonPressed & Z_TRIG) != 0) {
                    controller->ButtonPressed &= ~Z_TRIG;
                    ExecuteItemHookDefault(player);
                }
            }
        }
    }
}

#include "actors/green_shell/update.inc.c"

#include "actors/blue_and_red_shells/update.inc.c"

void BumpObject(Object* arg0) {
    if ((arg0->bump.distance_zx < 0.0f) && (arg0->bump.flag_zx == 1)) {
        arg0->position[0] -= (arg0->bump.bump_zx[0] * arg0->bump.distance_zx);
        arg0->position[1] -= (arg0->bump.bump_zx[1] * arg0->bump.distance_zx);
        arg0->position[2] -= (arg0->bump.bump_zx[2] * arg0->bump.distance_zx);
    }
    if ((arg0->bump.distance_xy < 0.0f) && (arg0->bump.flag_xy == 1)) {
        arg0->position[0] -= (arg0->bump.bump_xy[0] * arg0->bump.distance_xy);
        arg0->position[1] -= (arg0->bump.bump_xy[1] * arg0->bump.distance_xy);
        arg0->position[2] -= (arg0->bump.bump_xy[2] * arg0->bump.distance_xy);
    }
    if ((arg0->bump.distance_yz < 0.0f) && (arg0->bump.flag_yz == 1)) {
        arg0->position[0] -= (arg0->bump.bump_yz[0] * arg0->bump.distance_yz);
        arg0->position[1] -= (arg0->bump.bump_yz[1] * arg0->bump.distance_yz);
        arg0->position[2] -= (arg0->bump.bump_yz[2] * arg0->bump.distance_yz);
    }
}
