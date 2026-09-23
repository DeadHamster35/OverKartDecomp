#ifndef podium_ceremony_actors_H
#define podium_ceremony_actors_H

#include <common_structs.h>

enum CeremonyActorType { Initial, Balloon, FireworkCone, FireworkBurst };

// unk
typedef struct {
    /* 0x00 */ s32 Mode; // Library short+pad; keep s32
    /* 0x04 */ s32 Timer;
    /* 0x08 */ s32 TotalTimer;
    /* 0x0C */ s32 Timeend;
    /* 0x10 */ s32 pos[3];
    /* 0x1C */ s8 Carmove;
    /* 0x1D */ s8 Rank;
    /* 0x1E */ s8 Kart; // CharacterSelections
} GpResult;
typedef GpResult struct_D_802874D8;

typedef struct {
    s16 Type;
    Vec3s Unk2;
    s8 Unk8;
    s8 Unk9;
    s8 UnkA;
    s8 UnkB;
} ActorInitParams;

// A generic actor, usually a balloon.
typedef struct {
    /* 0x00 */ s32 IsActive;
    /* 0x04 */ s16 Type;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s8 Unk8;
    /* 0x09 */ s8 Unk9;
    /* 0x0A */ s16 UnkA;
    /* 0x0C */ s16 UnkC;
    /* 0x0E */ s16 UnkE;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ ActorInitParams* InitParams;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 Unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 Unk36;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s32 Timer; // Used to delete the actor after some Time.
    /* 0x40 */ s32 Unk40;
    /* 0x44 */ s32 Unk44;
    /* 0x48 */ s32 unk48;
} CeremonyActor;

typedef struct {
    /* 0x00 */ s32 IsActive;
    /* 0x04 */ s16 Type;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s8 Unk8;
    /* 0x09 */ s8 Unk9;
    /* 0x0A */ s16 UnkA;
    /* 0x0C */ s16 UnkC;
    /* 0x0E */ s16 UnkE;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ ActorInitParams* InitParams;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30; // s32 instead of s16
    /* 0x34 */ f32 unk34; // f32 instead of s16
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 Unk40;
    /* 0x44 */ s32 Unk44;
    /* 0x48 */ s32 unk48;
} Firework;

void UpdateActorsLoop(void);
void func_80280650(void);
void SetInitialPosition(CeremonyActor*);
CeremonyActor* FindAvailableEntry(void);
CeremonyActor* NewActor(ActorInitParams*);
u16 RandomU16Credits(void);
f32 RandomFloatBetween0And1(void);
f32 RandomWhoKnows(f32);
void func_80280884(void);
void FireworkConeUpdateAndSpawnBurst(Firework*);
void func_80280A28(Vec3f, Vec3s, f32);
void RenderFireworks(Vec3f, f32, s32, s16);
void BalloonsAndFireworksInit(void);
void func_80280FFC(void);
void func_8028100C(s32, s32, s32);
void func_8028150C(void);
void func_80281520(void);
void func_80281528(void);
void func_80281530(void);
void func_80281538(void);
void func_80281540(void);
void ResultsSequence(void);

extern GpResult Resultstat;
extern CeremonyActor* SPodiumActorList;
extern s32 D_802874FC;

extern ActorInitParams InitBalloon;
extern ActorInitParams InitCone;
extern ActorInitParams initInitUnk;
extern ActorInitParams InitBurst;

extern Gfx* D_802874D4;
extern s32 D_802874FC;
extern Mat4 D_80287500;

#endif
