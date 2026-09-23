#ifndef SKYBOX_AND_SPLITSCREEN_H
#define SKYBOX_AND_SPLITSCREEN_H

#include <PR/gbi.h>
#include "code_800029B0.h"

#define G_CLEAR_ALL_MODES 0xFFFFFFFF

/* Function Prototypes */

void DisplayBackground(Vtx*, Screen*, s32, s32, f32*);

void SetViewport(Screen*);
void func_802A38AC(void);
void FadeoutSequence(void);
void ClearZBuffer2(Screen*);
void ClearZBuffer(void);
void InitRDP(void);
void func_802A40A4(void);
void func_802A40AC(void);
void func_802A40B4(void);
void func_802A40BC(void);
void func_802A40C4(void);
void func_802A40CC(void);
void func_802A40D4(void);
void func_802A40DC(void);
s32 InitRSPFull(void);
void InitRSP(void);
void ClearFramebuffer(void);
void MakeBorder(void);
void InitialBackground(Vtx*);
void DisplayBackground2(Vtx*, Screen*, s32, s32, f32*);
void InititalPerspective(void);
void SetupPerspective(void);
void InitialDrawRight(void);
void InitialDrawLeft(void);
void InitialDrawUp(void);
void InitialDrawDown(void);
void InitialDrawCommon(void);
void InitialDrawUpLeft(void);
void InitialDrawUpRight(void);
void InitialDrawDownLeft(void);
void InitialDrawDownRight(void);
void DrawFullScreen(void);
void DrawLeftScreen(void);
void DrawRightScreen(void);
void DrawUpScreen(void);
void DrawDownScreen(void);
void DrawUpLeftScreen(void);
void DrawUpRightScreen(void);
void DrawDownLeftScreen(void);
void DrawDownRightScreen(void);
void InitialScreenData(void);
void GetFramebuffer(s32, s32, s32, s32, u16*, u16*);
void TakeSnapshot15(void);
void TakeSnapshot9(void);

extern Vp D_802B8880[];

#endif
