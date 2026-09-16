#ifndef OVERKARTDEFAULTS_H
#define OVERKARTDEFAULTS_H

/*
 * Base-project hook layer. Vanilla and Library call these *Default names.
 * Bodies live in OverKartDefaults.c. Later a host project replaces this
 * file to bypass stock behavior (save, race tick, menu print, …).
 */
#include "MainInclude.h"

extern void gameCodeDefault(void);
extern void titleMenuDefault(void);
extern void allRunDefault(void);
extern void PrintMenuFunctionDefault(void);

extern void DisplayObjectDefault(void* car, Object* inputObject);
extern void CollideObjectDefault(Player* Car, Object* Target);
extern void DrawPerScreenDefault(Camera* LocalCamera);
extern void ExecuteItemHookDefault(Player* Car);
extern void MiniMapDrawDefault(void);
extern void BalloonCheckDefault(Player* player, f32 arg1, f32 arg2, s8 playerIndex, s8 balloonIndex, s16 rotation);
extern void ItemboxCollideCheckDefault(Player* Car, Object* Target);

extern void MapStartupDefault(short InputID);
extern void KWKumo_Alloc_Hook_Default(void);
extern void KWChart_Kumo_Hook_Default(int screen_num);
extern void DrawMapSelectDefault(void);
extern void MapSelectMenuDefault(void);

extern void check_romghostDefault(void);
extern void DMAROMGhostDefault(void);

extern void SaveFunc800B45E0Default(s32 arg0);
extern void SaveFunc800B4670Default(void);
extern void LoadFunc800B4A10Default(void);
extern void SaveFunc800B559CDefault(s32 arg0);
extern void SaveFunc800B5948Default(void);

extern void RubberBandManDefault(s32 playerId, Player* player);

#endif
