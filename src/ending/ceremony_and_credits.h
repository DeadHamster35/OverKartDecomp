#ifndef CEREMONY_AND_CREDITS_H
#define CEREMONY_AND_CREDITS_H

#include <common_structs.h>
#include "camera.h"

struct struct_80283431 {
    Vec3f Unk0;
    Vec3s UnkC;
};

struct struct_80283430 {
    s8 Unk0;  // Index
    s8 Unk1;  // Speed
    u16 Unk2; // Point
    s8 unk4;
    s8 Unk5;
    Vec3s unk6;
};

struct credits_data_1FA0 {
    Vec3f Unk0;
    s8 pad[0x10];
    s8 unk1C;
    s8 Unk1D;
    s16 unk1E;
    f32 unk20;
    s8 Pad2[0x24];
    s16 unk48[3];
    s16 Unk4E[2];
    s16 Un52;
    s16 Unk54[2];
    s16 Unk58;
    s16 Unk5A[2];
    s16 Unk5E;
    s16 Unk60;
    s16 Unk62;
    f32 Unk64;
    f32 Unk68;
    s16 Unk6C;
    s16 Unk6E;
};

// Cinematic Camera? Camera rails?
// CutsceneVariable?
/**
 * Struct Used to store Cutscene info, like the Camera's Target Position/focus.
 *
 * See the sCutsceneVars[] array in Camera.c for more details.
 */
typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f LookAt;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8 Cutscene;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ Vec3f unk24;
    /* 0x30 */ Vec3f unk30;
    /* 0x3C */ Vec3f unk3C;
    /* 0x48 */ Vec3s unk48;
    /* 0x4E */ Vec3s Unk4E;
    /* 0x54 */ Vec3s Unk54;
    /* 0x5A */ Vec3s Unk5A;
    /* 0x60 */ s16 Unk60;
    /* 0x62 */ s16 Unk62;
    /* 0x64 */ f32 Unk64;
    /* 0x68 */ f32 Unk68;
    /* 0x6C */ s16 Unk6C;
    /* 0x6E */ s16 Unk6E;
} CinematicCamera; // Size = 0x70

struct struct_80282C40 {
    s8 Unk0;
    s8 Unk1;
    s8 Unk2;
    u8 Unk3;
    s8 unk4;
    s8 Unk5;
    Vec3s unk6;
};

struct struct_80285D80 {
    u8 Unk0[6];
    Vec3s unk6;
};

struct struct_80286A04 {
    u8 Unk0;
    u8 Unk1;
    struct struct_80285D80* unk4;
    struct struct_80285D80* Unk8;
    u16 UnkC;
};

struct CutsceneSplinePoint {
    s8 Index;
    u8 Speed;
    Vec3s Point;
};

struct Cutscene {
    /* 0x0 */ void (*shot)(CinematicCamera*);
    /* 0x4 */ s16 Duration;
    /* 0x6 */ // s16 pad;
}; /* Size = 0x8 */

typedef void (*CameraEvent)(CinematicCamera* c);
typedef CameraEvent CutsceneShot;

/* Function Prototypes */

void GrouCamdemoInit(void);
s32 GrouCamdemoMain(Camera*);
void Vec3FSetDupe(Vec3f, f32, f32, f32);
void Vec3SSetDupe(Vec3s, s16, s16, s16);
void Vec3FClear(Vec3f);
void Vec3SClear(Vec3s);
void Vec3FCopyReturnDupe(Vec3f, Vec3f);
void Vec3SCopyDupe(Vec3s, Vec3s);
void func_80282040(void);
void func_80282048(void);
void RotateYVec3F(Vec3f, Vec3f, s16);
void RotateXVec3F(Vec3f, Vec3f, s16);
s32 F32Lerp(f32*, f32, f32);
s32 EaseOutTransition(s16*, s16, s16);
s32 AdjustF32ValueTransition(f32*, f32, f32);
s32 AdjustS16ValueTransition(s16*, s16, s16);
void ResetSpline(void);
void ResetSplineWrap(CinematicCamera*);
void CalculateDistanceAngleYAndAngleYToXz(Vec3f, Vec3f, f32*, s16*, s16*);
void ApplyDistanceAngleYAndAngleYToXz(Vec3f, Vec3f, f32, s16, s16);
void AbortingCinematicFunction(Vec3f, Vec3f, Vec3f, Vec3s);
void EvaluateCubicSpline(f32, Vec3f, f32*, f32[], f32[], f32[], f32[]);
s32 MovePointAlongSpline(Vec3f, f32*, struct struct_80283430[], s16*, f32*);
void func_80282BE4(struct struct_80283430*, s8, u8, s8, Vec3s, s32);
void func_80282C40(struct struct_80283430*, struct struct_80282C40*, s32);
s32 MoveCinematicCameraAlongSpline(CinematicCamera*, struct struct_80286A04*, struct struct_80286A04*, s32);
void func_80282E58(CinematicCamera*, struct struct_80282C40*, s32);
void func_80282EAC(s32, CinematicCamera*, s16, s16, s16);
void func_80282F00(s16*, s16);
void func_80282F44(s32, CinematicCamera*, Camera*);
void func_802830B4(CinematicCamera*, s16, s16, s16);
void func_80283100(CinematicCamera*, f32*);
void func_80283240(s16);
s32 CutsceneEvent(CameraEvent event, CinematicCamera*, s16, s16);
s32 func_80283330(s32);
s32 func_8028336C(CinematicCamera*, Camera*);
s32 CinematicStub(void);
void EvBalloon(CinematicCamera*);
void EvKartmove(CinematicCamera*);
void WinOpen(CinematicCamera*);
void WinClose(CinematicCamera*);
void CreditsGone(CinematicCamera*);
void CamDEMOSEThisIsMarioKart(CinematicCamera*);
void CamDEMOSeMuteHyoushou(CinematicCamera*);
void CamDEMOSECongratulations(CinematicCamera*);
void CamDEMOSEHANABI(CinematicCamera*);
void CamDEMOSEPUKUPUKUBIGGER(CinematicCamera*);
void CamDEMOSEPUKUPUKUDOWN(CinematicCamera*);
void CamDEMOSEPUKUPUKUFIRE(CinematicCamera*);
void CamDEMOSEDODAIUP(CinematicCamera*);
void CamDEMOSESTARBURST(CinematicCamera*);
void func_80283A54(CinematicCamera*);
void func_80283A7C(CinematicCamera*);
void CamDEMOSEBIGCRUSH(CinematicCamera*);
void CamDEMOSEVoice(CinematicCamera*);
void CamDEMOPRIZEABGM(CinematicCamera*);
void CamDEMOPRIZEBBGM(CinematicCamera*);
void CamDEMOFourthInit(CinematicCamera*);
void CamDEMOFourthRoutine(CinematicCamera*);
void CamDEMOENDINGBGM(CinematicCamera*);
void CamDEMOChangeSoundModeResult(CinematicCamera*);
void CamDEMOChangeSoundModeEnding(CinematicCamera*);
void CamDEMOSETHEEND(CinematicCamera*);
void DemoFirework0(CinematicCamera*);
void func_80283EA0(CinematicCamera*);
void CopyPlayerTwoInCamera(CinematicCamera*);
void LerpPlayerTwoInCamera(CinematicCamera*);
void func_80283F6C(CinematicCamera*);
void CopyPlayerThreeInCamera(CinematicCamera*);
void LerpPlayerThreeInCamera(CinematicCamera*);
void func_80284068(CinematicCamera*);
void func_802840C8(CinematicCamera*);
void func_80284154(CinematicCamera*);
void func_80284184(CinematicCamera*);
void func_802841E8(CinematicCamera*);
void func_8028422C(CinematicCamera*);
void func_802842A8(CinematicCamera*);
void func_802842D8(CinematicCamera*);
void func_80284308(CinematicCamera*);
void func_80284418(CinematicCamera*);
void func_80284494(CinematicCamera*);
void func_802844FC(CinematicCamera*);
void func_8028454C(CinematicCamera*);
void func_802845EC(CinematicCamera*);
void func_8028461C(CinematicCamera*);
void func_80284648(CinematicCamera*);
void func_802846AC(void);
void func_802846B4(CinematicCamera*);
void func_802846E4(CinematicCamera*);
void func_802847CC(CinematicCamera*);
void PlayCutscene(CinematicCamera*);
void CeremonyTransitionSlidingBorders(void);

/* This is where I'd put my static data, if I had any */

extern s32 D_80283FCC;
extern s32 D_80283FF4;
extern f32 D_802856B0;
extern f32 D_802856B4;
extern f32 OrderedSizeSlidingBorders;
extern f32 D_802856BC;
extern f32 SizeSlidingBorders;
extern s32 D_802856C4;
extern s32 D_802856C8[]; // Padding?
extern s16 SCutsceneShot;
extern s16 CutsceneShotTimer;
extern s32 D_802876D4;
extern s32 D_802876D8;
extern s32 D_802876DC; // fake/Padding? Or D8 is array?
extern CinematicCamera D_802876E0;
extern struct struct_80283431 D_80287750[];
extern struct struct_80283430 D_80287818[];
extern struct struct_80283430 D_80287998[];
extern f32 SCutsceneSplineSegmentProgress;
extern s16 SCutsceneSplineSegment;
extern s16 D_80287B1E;
extern s8 D_80287B20;
extern struct struct_80282C40 D_802856DC[];
extern struct struct_80282C40 D_80285718[];
extern struct struct_80282C40 D_80285754[];
extern struct struct_80282C40 D_80285784[];
extern struct struct_80282C40 D_802857B4[];
extern struct struct_80282C40 D_802857CC[];
extern struct struct_80282C40 D_802857F0[];
extern struct struct_80282C40 D_80285850[];
extern struct struct_80282C40 D_802858B0[];
extern struct struct_80282C40 D_802858C8[];
extern struct struct_80282C40 D_80285910[];
extern struct struct_80282C40 D_80285928[];
extern struct struct_80282C40 D_80285940[];
extern struct struct_80282C40 D_80285A10[];
extern struct struct_80282C40 D_80285A4C[];
extern struct struct_80282C40 D_80285A88[];
extern struct struct_80282C40 D_80285AB8[];
extern struct struct_80282C40 D_80285AE8[];
extern struct struct_80282C40 D_80285B00[];
extern struct struct_80282C40 D_80285B18[];
extern struct struct_80282C40 D_80285B54[];
extern struct struct_80282C40 D_80285B90[];
extern struct struct_80282C40 D_80285BA8[];
extern struct struct_80282C40 D_80285C38[];
extern struct struct_80282C40 D_80285C74[];
extern struct Cutscene CutScene[];
extern s16 D_80285D14;
extern s32 D_802876D4;
extern s32 D_802876D8;
// extern struct credits_data_1FA0 *D_802876E0;

#endif // CEREMONY_AND_CREDITS_H
