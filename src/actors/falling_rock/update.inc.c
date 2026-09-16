#include <actors.h>
#include <code_800029B0.h>
#include <main.h>
#include "courses/choco_mountain/course_data.h"

void reset_iwa(struct ActorSpawnData* spawnData, struct FallingRock* rock) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 offset = SEGMENT_OFFSET(spawnData);
    struct ActorSpawnData* temp_v0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    Vec3s sp24 = { 60, 120, 180 };
    temp_v0 += rock->unk_06;
    rock->respawnTimer = sp24[rock->unk_06]; // * 2
    rock->position[0] = (f32) temp_v0->pos[0] * g_mirrorValue;
    rock->position[1] = (f32) temp_v0->pos[1] + 10.0f;
    rock->position[2] = (f32) temp_v0->pos[2];
    vec3f_set(rock->velocity, 0, 0, 0);
    SetSVector(rock->angle, 0, 0, 0);
}

/**
 * @brief Spawns falling rocks.
 * Used in Choco Mountain.
 *
 * @param spawnData
 */
void set_iwa(struct ActorSpawnData* spawnData) {
    s32 addr = SEGMENT_NUMBER2(spawnData);
    s32 offset = SEGMENT_OFFSET(spawnData);
    // Casting this to prevent warning does not work.
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[addr] + offset);
    struct FallingRock* temp_v1;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s16 temp;

    while (temp_s0->pos[0] != -0x8000) {
        startingPos[0] = temp_s0->pos[0] * g_mirrorValue;
        startingPos[1] = temp_s0->pos[1] + 10.0f;
        startingPos[2] = temp_s0->pos[2];
        vec3f_set(startingVelocity, 0, 0, 0);
        SetSVector(startingRot, 0, 0, 0);
        temp = addObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_FALLING_ROCK);
        temp_v1 = (struct FallingRock*) &g_SimpleObjectArray[temp];

        temp_v1->unk_06 = temp_s0->someId;
        InitialBump((Collision*) &temp_v1->bump);
        temp_s0++;
    }
}

/**
 * @brief Updates the falling rock actor.
 * Actor used in Choco Mountain.
 *
 * @param rock
 */
void update_actor_falling_rocks(struct FallingRock* rock) {
    Vec3f unkVec;
    f32 pad0;
    f32 pad1;

    if (rock->respawnTimer != 0) {
        rock->respawnTimer -= 1;
        return;
    }
    if (rock->position[1] < g_waterHeight) {
        reset_iwa(d_course_choco_mountain_falling_rock_spawns, rock);
    }
    rock->angle[0] += (s16) ((rock->velocity[2] * 5461.0f) / 20.0f); // DEGREES(30) is 5460
    rock->angle[2] += (s16) ((rock->velocity[0] * 5461.0f) / 20.0f);
    rock->velocity[1] -= 0.1;
    if (rock->velocity[1] < (-2.0f)) {
        rock->velocity[1] = -2.0f;
    }
    rock->position[0] += rock->velocity[0];
    rock->position[1] += rock->velocity[1];
    rock->position[2] += rock->velocity[2];
    pad1 = rock->velocity[1];
    CheckBump(&rock->bump, 10.0f, rock->position[0], rock->position[1], rock->position[2]);
    pad0 = rock->bump.distance_zx;
    if (pad0 < 0.0f) {
        unkVec[0] = -rock->bump.bump_zx[0];
        unkVec[1] = -rock->bump.bump_zx[1];
        unkVec[2] = -rock->bump.bump_zx[2];
        rock->position[0] += unkVec[0] * rock->bump.distance_zx;
        rock->position[1] += unkVec[1] * rock->bump.distance_zx;
        rock->position[2] += unkVec[2] * rock->bump.distance_zx;
        BumpVelocity(unkVec, pad0, rock->velocity, 2.0f);
        rock->velocity[1] = -1.2f * pad1;
        NAEnmTrgStart(rock->position, rock->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
    }
    pad0 = rock->bump.distance_xy;
    if (pad0 < 0.0f) {
        unkVec[1] = -rock->bump.bump_xy[1];
        if (unkVec[1] == 0.0f) {
            rock->velocity[1] *= -1.2f;
            return;
        } else {
            unkVec[0] = -rock->bump.bump_xy[0];
            unkVec[2] = -rock->bump.bump_xy[2];
            rock->position[0] += unkVec[0] * rock->bump.distance_xy;
            rock->position[1] += unkVec[1] * rock->bump.distance_xy;
            rock->position[2] += unkVec[2] * rock->bump.distance_xy;
            BumpVelocity(unkVec, pad0, rock->velocity, 2.0f);
            rock->velocity[1] = -1.2f * pad1;
            NAEnmTrgStart(rock->position, rock->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
    pad0 = rock->bump.distance_yz;
    if (pad0 < 0.0f) {
        unkVec[1] = -rock->bump.bump_yz[1];
        if (unkVec[1] == 0.0f) {
            rock->velocity[1] *= -1.2f;
        } else {
            unkVec[0] = -rock->bump.bump_yz[0];
            unkVec[2] = -rock->bump.bump_yz[2];
            rock->position[0] += unkVec[0] * rock->bump.distance_yz;
            rock->position[1] += unkVec[1] * rock->bump.distance_yz;
            rock->position[2] += unkVec[2] * rock->bump.distance_yz;
            pad1 = rock->velocity[1];
            BumpVelocity(unkVec, pad0, rock->velocity, 2.0f);
            rock->velocity[1] = -1.2f * pad1;
            NAEnmTrgStart(rock->position, rock->velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}
