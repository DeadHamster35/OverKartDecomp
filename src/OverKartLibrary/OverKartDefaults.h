#ifndef OVERKARTDEFAULTS_H
#define OVERKARTDEFAULTS_H

/*
 * Base-project hook layer. Vanilla and Library call these *Default Names.
 * Bodies live in OverKartDefaults.c. Later a host project replaces this
 * file to bypass stock behavior (save, race tick, menu print, …).
 */
#include "MainInclude.h"

extern void GameCodeDefault(void);
extern void TitleMenuDefault(void);
extern void AllRunDefault(void);
extern void PrintMenuFunctionDefault(void);

extern void DisplayObjectDefault(void* car, Object* inputObject);
extern void CollideObjectDefault(Player* Car, Object* Target);
extern void DrawPerScreenDefault(Camera* LocalCamera);
extern void ExecuteItemHookDefault(Player* Car);
extern void MiniMapDrawDefault(void);
extern void BalloonCheckDefault(Player* Player, f32 arg1, f32 Arg2, s8 playerIndex, s8 balloonIndex, s16 Rotation);
extern void ItemboxCollideCheckDefault(Player* Car, Object* Target);

extern void MapStartupDefault(short InputID);
extern void KWKumoAllocHookDefault(void);
extern void KWChartKumoHookDefault(int screen_num);
extern void DrawMapSelectDefault(void);
extern void MapSelectMenuDefault(void);

extern void CheckRomghostDefault(void);
extern void DMAROMGhostDefault(void);

extern void SaveFunc800B45E0Default(s32 arg0);
extern void SaveFunc800B4670Default(void);
extern void LoadFunc800B4A10Default(void);
extern void SaveFunc800B559CDefault(s32 arg0);
extern void SaveFunc800B5948Default(void);

extern void RubberBandManDefault(s32 PlayerId, Player* Player);

#endif
