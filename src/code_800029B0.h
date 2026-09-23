#ifndef CODE_800029B0_H
#define CODE_800029B0_H

#include <macros.h>
#include <ultra64.h>
#include <actor_types.h>
#include "camera.h"

typedef struct Screen {
    /* 0x00 */ Controller* Cont; // Controllers ptr 800F6910
    /* 0x04 */ Camera* Camera;   // Player Camera ptr
    /* 0x08 */ Player* Kart;     // Player ptr 800F6990
    /* 0x0C */ s32* Matrix;      // Library AffineMtx*; keep s32* (D_8015F790)
    /* 0x10 */ Viewport IsInViewport;
    /* 0x20 */ f32 ScreenAspect;
    /* 0x24 */ f32 ScreenAngle;
    /* 0x28 */ s16 Horizon;
    /* 0x2A */ s16 pad;
    /* 0x2C */ s16 Width;
    /* 0x2E */ s16 Height;
    /* 0x30 */ s16 Posx;
    /* 0x32 */ s16 Posy;
    /* 0x34 */ s16 Flag;
    /* 0x36 */ s16 CameraDirection;
    /* 0x38 */ s16 CameraPoint;
    /* 0x3A */ s16 Unk42;
    /* 0x3C */ s32 Pad2;
} Screen; // Size = 0x40

/* Function Prototypes */

void SetMusicVolume(void);
void InitialRaceSequence(void);
void DelayInitialMap(void);
void InitializeFirstOnce(void);
void InitialEndingMap(void);

extern s16 LoadedcourseFlag;
extern u16 MusicUserVolumeFlag;
extern s32 D_800DC5AC;
extern u16 ClearCFBFlag;
extern u16 BackgroundFlag;
extern u16 TwoDFlag;
extern u16 SkyToggle;
extern u16 FadeOutFlag;
extern u16 FadeOutCounter;
extern u16 FadeOutCounter2;
extern u16 D_800DC5CC;
extern s32 D_800DC5D0;
extern s32 D_800DC5D4;
extern s32 D_800DC5D8;
extern s32 D_800DC5DC;
extern s32 D_800DC5E0;
// This is tracking which credit "State" we're in, decides which credits are shown (and probably other stuff)
extern u16 D_800DC5E4;
extern s32 WinKart;

extern Screen D_8015F480[4];
extern Screen* D_800DC5EC;
extern Screen* D_800DC5F0;
extern Screen* D_800DC5F4;
extern Screen* D_800DC5F8;
extern u16 PauseFlag;
extern u8* PAppNmiBuffer;
extern s32 ScreenFlip; // D_800DC604
extern s16 CreditsCourseId;
extern s16 ItemSetFlag;

extern FaceStruct* FaceBuffer;
extern u16* CollisionTable;
extern u16 CourseFaceMaxIndex;
extern u16 CollisionPointer;
extern u32 Facenumber;

extern Vec3f D_8015F590;
extern s32 WallFlag;
extern s32 FlatFlag;
extern s32 D_8015F5A4;

extern Vtx* VertexBuffer2[];
extern s16 MaximumX;
extern s16 MinimumX;

extern s16 MaximumY;
extern s16 MinimumY;

extern s16 MaximumZ;
extern s16 MinimumZ;
extern s16 Maxlimmity;
extern s16 Minlimmity;
extern u16 PointNumber;
extern s16 Shadowface;
extern s16 Outsideface;
extern u16 KameCounter;

extern u16 MooSoundCounter;
extern u16 MooSoundPointer;
extern f32 MooSoundLastDistance;
extern Vec3f MooSoundPosition;
extern UNUSED u32 D_8015F718[3];
extern size_t FreeMemorySize;
extern uintptr_t FreeMemoryPointer;
extern uintptr_t LastMemoryPointer;
extern uintptr_t VertexMemoryPointer;
extern uintptr_t ObjectMemoryPointer;
extern u8 ok_ObjectPoolStart[];
extern u8 ok_ObjectPoolEnd[];

#define FACE_BUFFER_START 0x80625800
#define FACE_BUFFER_END 0x80700000
#define VERTEX_MEMORY_TOP 0x80700000
#define VERTEX_MEMORY_FLOOR 0x80625800
#define LAST_MEMORY_TOP 0x80800000
#define LAST_MEMORY_FLOOR 0x80725800

extern u32 MapMemoryPointer;
extern uintptr_t StaticMemoryPointer;
extern Vec3f WaterfallSoundPos;
extern Vec3f CrowdSoundPos1;
extern Vec3f CrowdSoundPos2;
extern Vec3f CrowdSoundPos3;
extern Vec3f CrowdSoundPos4;

extern f32 MirrorValue;
extern s32 ScreenCounter;

extern s32 D_8015F790[];
extern u16 ReplayFlag;
extern u16 RetryFlag;
extern u16 FinishZoomFlag;
extern f32 TimePlayerLastTouchedFinishLine[];

extern u8* Wincount2P;
extern u8* Wincount3P;
extern u8* Wincount4P;
extern u8* Battlewincount2P;
extern u8* Battlewincount3P;
extern u8* Battlewincount4P;

extern Vec3f GoalBannerPos;
extern s32 ItemboxFlag;

extern s32 TreeFlag;
extern f32 WaterHeight;
extern f32 WaterVelocity;
extern s16 RankBuffer[]; // Player Index at each Position
extern u16 StaticObjectCount;

extern UNUSED u8 D_80162578[];
extern s16 DebugPathCount;
extern s16 SIsController1Unplugged;
extern s32 FogRed;
extern s32 FogGreen;
extern s32 FogBlue;
extern s32 D_801625F8;
extern f32 D_801625FC;

#endif
