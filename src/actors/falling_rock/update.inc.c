#include <actors.h>
#include <code_800029B0.h>
#include <Main.h>
#include "courses/choco_mountain/course_data.h"

void ResetIwa(struct ActorSpawnData* spawnData, struct FallingRock* rock) {
    s32 segment = SEGMENT_NUMBER2(spawnData);
    s32 Offset = SEGMENT_OFFSET(spawnData);
    struct ActorSpawnData* temp_v0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    Vec3s sp24 = { 60, 120, 180 };
    temp_v0 += rock->Unk06;
    rock->RespawnTimer = sp24[rock->Unk06]; // * 2
    rock->Position[0] = (f32) temp_v0->pos[0] * MirrorValue;
    rock->Position[1] = (f32) temp_v0->pos[1] + 10.0f;
    rock->Position[2] = (f32) temp_v0->pos[2];
    Vec3FSet(rock->Velocity, 0, 0, 0);
    SetSVector(rock->Angle, 0, 0, 0);
}

/**
 * @brief Spawns falling rocks.
 * Used in Choco Mountain.
 *
 * @Param spawnData
 */
void SetIwa(struct ActorSpawnData* spawnData) {
    s32 Addr = SEGMENT_NUMBER2(spawnData);
    s32 Offset = SEGMENT_OFFSET(spawnData);
    // Casting this to prevent warning does not work.
    struct ActorSpawnData* temp_s0 = (struct ActorSpawnData*) VIRTUAL_TO_PHYSICAL2(SegmentTable[Addr] + Offset);
    struct FallingRock* temp_v1;
    Vec3f startingPos;
    Vec3f startingVelocity;
    Vec3s startingRot;
    s16 temp;

    while (temp_s0->pos[0] != -0x8000) {
        startingPos[0] = temp_s0->pos[0] * MirrorValue;
        startingPos[1] = temp_s0->pos[1] + 10.0f;
        startingPos[2] = temp_s0->pos[2];
        Vec3FSet(startingVelocity, 0, 0, 0);
        SetSVector(startingRot, 0, 0, 0);
        temp = AddObjectBuffer(startingPos, startingRot, startingVelocity, ACTOR_FALLING_ROCK);
        temp_v1 = (struct FallingRock*) &SimpleObjectArray[temp];

        temp_v1->Unk06 = temp_s0->SomeId;
        InitialBump((Collision*) &temp_v1->Bump);
        temp_s0++;
    }
}

/**
 * @brief Updates the falling rock actor.
 * Actor Used in Choco Mountain.
 *
 * @Param rock
 */
void UpdateActorFallingRocks(struct FallingRock* rock) {
    Vec3f unkVec;
    f32 Pad0;
    f32 pad1;

    if (rock->RespawnTimer != 0) {
        rock->RespawnTimer -= 1;
        return;
    }
    if (rock->Position[1] < WaterHeight) {
        ResetIwa(d_course_choco_mountain_falling_rock_spawns, rock);
    }
    rock->Angle[0] += (s16) ((rock->Velocity[2] * 5461.0f) / 20.0f); // DEGREES(30) is 5460
    rock->Angle[2] += (s16) ((rock->Velocity[0] * 5461.0f) / 20.0f);
    rock->Velocity[1] -= 0.1;
    if (rock->Velocity[1] < (-2.0f)) {
        rock->Velocity[1] = -2.0f;
    }
    rock->Position[0] += rock->Velocity[0];
    rock->Position[1] += rock->Velocity[1];
    rock->Position[2] += rock->Velocity[2];
    pad1 = rock->Velocity[1];
    CheckBump(&rock->Bump, 10.0f, rock->Position[0], rock->Position[1], rock->Position[2]);
    Pad0 = rock->Bump.DistanceZX;
    if (Pad0 < 0.0f) {
        unkVec[0] = -rock->Bump.BumpZX[0];
        unkVec[1] = -rock->Bump.BumpZX[1];
        unkVec[2] = -rock->Bump.BumpZX[2];
        rock->Position[0] += unkVec[0] * rock->Bump.DistanceZX;
        rock->Position[1] += unkVec[1] * rock->Bump.DistanceZX;
        rock->Position[2] += unkVec[2] * rock->Bump.DistanceZX;
        BumpVelocity(unkVec, Pad0, rock->Velocity, 2.0f);
        rock->Velocity[1] = -1.2f * pad1;
        NAEnmTrgStart(rock->Position, rock->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
    }
    Pad0 = rock->Bump.DistanceXY;
    if (Pad0 < 0.0f) {
        unkVec[1] = -rock->Bump.BumpXY[1];
        if (unkVec[1] == 0.0f) {
            rock->Velocity[1] *= -1.2f;
            return;
        } else {
            unkVec[0] = -rock->Bump.BumpXY[0];
            unkVec[2] = -rock->Bump.BumpXY[2];
            rock->Position[0] += unkVec[0] * rock->Bump.DistanceXY;
            rock->Position[1] += unkVec[1] * rock->Bump.DistanceXY;
            rock->Position[2] += unkVec[2] * rock->Bump.DistanceXY;
            BumpVelocity(unkVec, Pad0, rock->Velocity, 2.0f);
            rock->Velocity[1] = -1.2f * pad1;
            NAEnmTrgStart(rock->Position, rock->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
    Pad0 = rock->Bump.DistanceYZ;
    if (Pad0 < 0.0f) {
        unkVec[1] = -rock->Bump.BumpYZ[1];
        if (unkVec[1] == 0.0f) {
            rock->Velocity[1] *= -1.2f;
        } else {
            unkVec[0] = -rock->Bump.BumpYZ[0];
            unkVec[2] = -rock->Bump.BumpYZ[2];
            rock->Position[0] += unkVec[0] * rock->Bump.DistanceYZ;
            rock->Position[1] += unkVec[1] * rock->Bump.DistanceYZ;
            rock->Position[2] += unkVec[2] * rock->Bump.DistanceYZ;
            pad1 = rock->Velocity[1];
            BumpVelocity(unkVec, Pad0, rock->Velocity, 2.0f);
            rock->Velocity[1] = -1.2f * pad1;
            NAEnmTrgStart(rock->Position, rock->Velocity, SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x0F));
        }
    }
}
