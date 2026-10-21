#include <ultra64.h>
#include <macros.h>
#include <PR/gu.h>
#include <mk64.h>
#include <debug.h>
#include <common_structs.h>
#include "main.h"
#include "memory.h"
#include <assets/common_data.h>
#include "render_player.h"
#include "render_objects.h"
#include "podium_ceremony_actors.h"
#include "camera_junk.h"
#include "player_controller.h"
#include "code_80057C60.h"
#include "cpu_vehicles_camera_path.h"
#include "code_80281C40.h"
#include "math_util.h"
#include "OverKart5Hooks.h"

s32 FireworkConeColour[] = {
    0x00FF4080, // pink
    0x008040FF, // purple
    0x0040FF40, // Green
    0x00FFFF40  // yellow
};

/**
 * Mainly colours the explosion effect.
 * Appears to colour the cones as well.
 */
s32 FireworkColour[] = {
    0x007F2040, // red
    0x0040207F, // purple
    0x00207F20, // Green
    0x007F7F20  // gold/yellow
};

u16 SRandomSeed16 = 0;

ActorInitParams InitBalloon = {
    Balloon, { 0xF3AF, 34, 0xFE2D }, 0, 0, 0, 0,
};

ActorInitParams InitBurst = {
    FireworkBurst, { 0xF3AF, 34, 0xFE2D }, 0, 0, 0, 0,
};

ActorInitParams InitCone = {
    FireworkCone, { 0xF3AF, 34, 0xFE2D }, 0, 0, 0, 0,
};

// This appears to be a Dummy actor placed in Index 0 of the actor list.
ActorInitParams InitDummy = {
    Initial, { 0xF2CC, 250, 0xFE11 }, 0, 0, 0, 0,
};

// This array appears to always be zero.
// This may not be an array, it appears to contain uninitialized random values to seed the random generator.
// The first Index Used; D_802874BE (7), requires to be an array.
// Based on that, the whole thing is now an array.
s16 D_802874B0[17];

Gfx* D_802874D4; // ptr to DisplayListHead

GpResult Resultstat; // 31 bytes

// s32 sActorTimer;

// u8 D_802874F4;
// u8 D_802874F5;
// u8 D_802874F6;
CeremonyActor* SPodiumActorList;
s32 D_802874FC;

void func_80280650(void) {
}

void SetInitialPosition(CeremonyActor* actor) {
    ActorInitParams* params = actor->InitParams;

    actor->pos[0] = params->Unk2[0];
    actor->pos[1] = params->Unk2[1];
    actor->pos[2] = params->Unk2[2];

    // Place Value in the high bits of s16.
    // Example: 85, 0b01010101 -> 0b0101010100000000
    actor->UnkA = params->Unk8 << 8;
    actor->UnkC = params->Unk9 << 8;
    actor->UnkE = params->UnkA << 8;
    actor->Type = params->Type;
}

CeremonyActor* FindAvailableEntry(void) {
    UNUSED s32 pad[2];
    CeremonyActor* actor = SPodiumActorList;
    s32 i;

    for (i = 0; i < 200; i++) {

        // Find an inactive actor.
        if ((actor->IsActive & 1) == 0) {
            bzero(actor, sizeof(CeremonyActor));
            actor->IsActive = 1;
            actor->unk24 = 1.0f;
            return actor;
        }

        actor++;
    }
    return NULL;
}

CeremonyActor* NewActor(ActorInitParams* arg0) {
    CeremonyActor* actor = FindAvailableEntry();

//! @bug No Null check.
//! @todo More indepth error checking/return Value
#ifdef AVOID_UB
    if (actor == NULL) {
        return (CeremonyActor*) &SPodiumActorList[0]; // Return first actor to prevent crash
    }
#endif

    actor->InitParams = arg0;
    SetInitialPosition(actor);
    return actor;
}

u16 RandomU16Credits(void) {
    u16 temp1, temp2;

    if (SRandomSeed16 == 22026) {
        SRandomSeed16 = 0;
    }

    temp1 = (SRandomSeed16 & 0x00FF) << 8;
    temp1 = temp1 ^ SRandomSeed16;

    SRandomSeed16 = ((temp1 & 0x00FF) << 8) + ((temp1 & 0xFF00) >> 8);

    temp1 = ((temp1 & 0x00FF) << 1) ^ SRandomSeed16;
    temp2 = (temp1 >> 1) ^ 0xFF80;

    if ((temp1 & 1) == 0) {
        if (temp2 == 43605) {
            SRandomSeed16 = 0;
        } else {
            SRandomSeed16 = temp2 ^ 0x1FF4;
        }
    } else {
        SRandomSeed16 = temp2 ^ 0x8180;
    }

    return SRandomSeed16;
}

f32 RandomFloatBetween0And1(void) {
    return RandomU16Credits() / 65536.0f;
}

f32 RandomWhoKnows(f32 arg0) {
    return (RandomFloatBetween0And1() * arg0) - (arg0 * 0.5f);
}

void func_80280884(void) {
}

// extern s16 D_802874CA;

void BalloonUpdate(CeremonyActor* actor) {
    DrawBalloon(actor->pos, 1.0f, actor->unk2E, actor->unk2C);
    actor->pos[1] += 0.8f;
    actor->unk2E = SinT(actor->unk30) * actor->unk34;
    actor->unk30 += actor->Unk32;
    actor->Timer++;

    // Delete actor
    if (actor->Timer > 800) {
        actor->IsActive = 0;
    }
    if (D_802874B0[13] == 1) {
        actor->IsActive = 0;
    }
}

/**
 * Spawns an aerial shell that ascends into the sky.
 * (Note that the explode effect is a different actor)
 */
void FireworkConeUpdateAndSpawnBurst(Firework* cone) {
    if (cone->Unk44 < 30) {
        cone->pos[1] += 2.5f;
        cone->pos[0] += RandomWhoKnows(0.2f);
        cone->pos[2] += RandomWhoKnows(0.2f);
    } else if (cone->unk2C == 4) {
        Firework* burst = (Firework*) NewActor(&InitCone);
        burst->pos[0] = cone->pos[0];
        burst->pos[1] = cone->pos[1];
        burst->pos[2] = cone->pos[2];
        burst->unk30 = FireworkConeColour[cone->unk48];
        burst->unk3C = 0xFF;
        burst->Unk40 = -0x11;
        burst->Unk44 = 0x64;
        burst->unk34 = 1.8700001f;
        burst->unk38 = 1.8700001f;
    }
}

Mat4 D_80287500;

void func_80280A28(Vec3f arg0, Vec3s arg1, f32 Arg2) {
    Mat4 mtx;

    Createaffine(mtx, arg0, arg1);
    mtx[0][0] = D_80287500[0][0] * Arg2;
    mtx[0][1] = D_80287500[1][0] * Arg2;
    mtx[0][2] = D_80287500[2][0] * Arg2;
    mtx[1][0] = D_80287500[0][1] * Arg2;
    mtx[1][1] = D_80287500[1][1] * Arg2;
    mtx[1][2] = D_80287500[2][1] * Arg2;
    mtx[2][0] = D_80287500[0][2] * Arg2;
    mtx[2][1] = D_80287500[1][2] * Arg2;
    mtx[2][2] = D_80287500[2][2] * Arg2;
    Affine2Mtx(&DynamicP->MTXEffect[Effectcount], mtx);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void RenderFireworks(Vec3f arg0, f32 arg1, s32 rgb, s16 Alpha) {
    Vec3f sp4C;
    Vec3s sp44;
    s16 red;
    s16 Green;
    s16 Blue;

    sp4C[0] = arg0[0];
    sp4C[1] = arg0[1];
    sp4C[2] = arg0[2];
    sp44[0] = 0;
    sp44[1] = Camera1->CameraDirection[1];
    sp44[2] = 0;
    func_80280A28(sp4C, sp44, arg1);
    gSPDisplayList(DisplayListHead++, D_0D008DB8);
    gDPLoadTextureBlock(DisplayListHead++, D_8018D48C, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    // ???????????????????????????????????
    red = ((rgb >> 0x10) & 0xFF) & 0xFF;
    Green = ((rgb >> 0x08) & 0xFF) & 0xFF;
    Blue = ((rgb >> 0x00) & 0xFF) & 0xFF;
    // ???????????????????????????????????
    ToumeIMode(red, Green, Blue, Alpha);
    gSPDisplayList(DisplayListHead++, D_0D008E48);
    Effectcount += 1;
}

void FireworkUpdate(Firework* actor) {
    s32 i;
    Vec3f pos;
    if (actor->Unk44 < 30) {
        for (i = 0; i < 10; i++) {
            pos[0] = actor->pos[0];
            pos[1] = actor->pos[1] - i * 2;
            pos[2] = actor->pos[2];
            RenderFireworks(pos, ((10 - i) / 10.0f) * 2, FireworkColour[actor->unk48],
                             (((((30 - actor->Unk44) * 100)) / 30.0f)));
        }

    } else {
        if (actor->unk2C < 5) {
            actor->unk3C += actor->Unk40 * 2;
            actor->unk34 += actor->unk38 * 2;
        } else {
            actor->unk3C += actor->Unk40 / (1.0f + (((actor->unk2C * 7) - 0x23) / 10.0f));
            actor->unk34 += actor->unk38 / (1.0f + (((actor->unk2C * 7) - 0x23) / 10.0f));
            if (actor->unk3C < 0) {
                actor->unk3C = 0;
            }
        }
        actor->unk2C += 1;
        if (((actor->unk3C > 0) && ((actor->unk34 > 0.0f))) || (actor->unk2C < 30)) {
            RenderFireworks(actor->pos, actor->unk34, actor->unk30, (s16) actor->unk3C);
        } else {
            actor->IsActive = 0;
        }
    }
    actor->Unk44 += 1;
}

void Unused80280FA0(UNUSED CeremonyActor* actor) {
}

void Unused80280FA8(UNUSED CeremonyActor* actor) {
}

void BalloonsAndFireworksInit(void) {
    Resultstat.TotalTimer = 0;
    SPodiumActorList = (CeremonyActor*) AllocateMemory(sizeof(CeremonyActor) * 200);
    bzero(SPodiumActorList, (sizeof(CeremonyActor) * 200));
    NewActor(&InitDummy);
}

void func_80280FFC(void) {
    Resultstat.Carmove = 1;
}

void func_8028100C(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 Arg2) {
}

/**
 * Spawn balloons!
 * The floats passed to RandomWhoKnows appears to effect the spread of the balloons
 */
void SpawnBalloons(s32 arg0, s32 arg1, s32 Arg2) {
    s32 i;

    for (i = 0; i < 100; i++) {
        CeremonyActor* balloon;
        balloon = NewActor(&InitBalloon);
        balloon->pos[0] = RandomWhoKnows(200.0f) + arg0;
        balloon->pos[1] = RandomWhoKnows(380.0f) + arg1;
        balloon->pos[2] = RandomWhoKnows(600.0f) + Arg2;
        balloon->unk2C = (s16) (s32) (RandomFloatBetween0And1() * 7.0f);
        balloon->unk30 = RandomU16Credits();
        balloon->Unk32 = (s16) (s32) (RandomWhoKnows(400.0f) + 900.0f);
        balloon->unk34 = (s16) (s32) (RandomWhoKnows(2000.0f) + 3000.0f);
    }
}

// extern s16 D_802874BE[3];
// extern s16 D_802874C6;
// extern s16 D_802874C8;
extern s32 FireworkColour[];

/**
 * The explosive effect when the firework's charge detonates
 * resulting in a small variety of amusing colours.
 * D_802874BE appears to be zero.
 */
void SpawnFireworkCone(s32 arg0, s32 arg1, s32 Arg2) {
    f32 num;
    static u32 D_80287540;

    if (((f32) RandomFloatBetween0And1() * (D_802874B0[7] + 0xD)) < 1.0f) {
        Firework* cone;
        cone = (Firework*) NewActor(&InitBurst);
        cone->pos[0] = RandomWhoKnows(0.0f) + arg0;
        cone->pos[1] = RandomWhoKnows((f32) (D_802874B0[11] + 100)) + (f32) arg1;
        cone->pos[2] = RandomWhoKnows((f32) (D_802874B0[12] + 700)) + (f32) Arg2;

        num = 1.1f;

        // Wrap the Counter from zero to three
        cone->unk48 = D_80287540 % 4U;
        cone->unk30 = FireworkColour[D_80287540 % 4U];
        cone->unk34 = num;
        cone->unk38 = num;
        D_80287540 += 1;
        cone->unk3C = 0xFF;
        cone->Unk40 = -0x11;
    }
}

// extern s16 D_802874C4;
// extern s32 sActorTimer;
extern Mat4 D_80287500;

/**
 * Allegedly controls fireworks movement.
 */
void SpawnTimer(void) {
    Camera* Camera = &Cameras[0];
    f32 lookAtY;

    guLookAtF(D_80287500, Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], Camera->LookatPos[0], Camera->LookatPos[1],
              Camera->LookatPos[2], Camera->UpVector[0], Camera->UpVector[1], Camera->UpVector[2]);
    if (Resultstat.Rank < 3) {
        if (Resultstat.TotalTimer < 300) {
            lookAtY = Camera->LookatPos[1];
            SpawnFireworkCone(-0xE0E, (s32) (((lookAtY - Camera->CameraPos[1]) * 1.5f) + lookAtY), -0x258);
        }
        if (Resultstat.TotalTimer == 120) {
            SpawnBalloons(-0xC6C, (s32) ((f32) D_802874B0[10] + 210.0f), -0x1EF);
        }
    } else if (Resultstat.TotalTimer == 2) {
        SpawnBalloons(-0xC6C, (s32) ((f32) D_802874B0[10] + 210.0f), -0x1EF);
    }

    Resultstat.TotalTimer += 1;
}

void* SUpdate[][3] = {
    // Dummy actor
    { Unused80280FA8, Unused80280FA0, 0 },
    // Balloon
    { Unused80280FA8, BalloonUpdate, 0 },
    // Firework Cone
    { Unused80280FA8, FireworkUpdate, 0 },
    // Firework Burst
    { FireworkConeUpdateAndSpawnBurst, 0, FireworkUpdate },
};

// extern s16 D_802874D0;

void UpdateActorsLoop(void) {
    void (*func)(void*);
    s32 i;
    s32 j;
    SpawnTimer();
    D_802874B0[16] = 0;

    // Why do we Loop over the actor list three times?
    // Actors don't with no Loop. fireworks don't spawn if the iterator is 2.
    for (i = 0; i < 3; i++) {
        CeremonyActor* actor = SPodiumActorList;
        for (j = 0; j < 200; j++) {
            if (actor->IsActive & 1) {

                func = SUpdate[actor->Type][i];

                if (func != 0) {
                    func(actor);
                }
            }
            actor++;
        }
    }
}

void func_8028150C(void) {
    D_802874D4 = DisplayListHead;
}

void func_80281520(void) {
}

void func_80281528(void) {
}

void func_80281530(void) {
}

void func_80281538(void) {
}

void func_80281540(void) {
}

void ResultsSequence(void) {
    MatrixCount = 0;
    D_802874FC = 0;
    ResultCameraControl();
    KartposControlFull();
    KenStartegy();
    KWVideoFramesYori();
    KWVideoFramesYori();
    Kwgameframesyori();
    if (Resultstat.Carmove != 0) {
        OgaHyousyouMove();
        EtcEnemyDrive();
    }
    ResultsCheck();
    DrawResultScreen();
    func_80281540();
#if DVDL
    display_dvdl();
#endif
    gDPFullSync(DisplayListHead++);
    gSPEndDisplayList(DisplayListHead++);
}
