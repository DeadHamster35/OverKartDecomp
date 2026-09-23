#ifndef SPAWN_PLAYERS_H
#define SPAWN_PLAYERS_H

#include "camera.h"
#include <common_structs.h>

/* Function Prototypes */
/** @cond */
void InitializePlayer(Player*, s8, f32, f32, f32, f32, u16, s16);
void SetUpViewDistance(void);
void MakeRank(void);
void MarioGP1P(f32*, f32*, f32);
void TimeAttack(f32*, f32*, f32);
void MarioGP2P(f32* arg0, f32* arg1, f32);
void VSRace2P(f32*, f32*, f32);
void Battle2P(f32*, f32*, f32);
void VSRace3P(f32*, f32*, f32);
void Battle3P(f32*, f32*, f32);
void VSRace4P(f32*, f32*, f32);
void Battle4P(f32*, f32*, f32);
void ResultKartPos(void);
void ResetPos(void);
void ResultPos(void);
void InitDraw(Player*, Camera*, s8, s8);
void InitialGame(void);
void InitResultKart(void);

extern f32 GridX[];
extern f32 GridZ[];
extern s16 Kartrank[];
extern f32 AcceleCount[];
extern f32 WaterlevelPlayer[];
extern s32 SterrCounter[];
extern s32 GreenTimer[];
extern s16 EchoFlag[];
extern u16 JugemuLine[];
extern s16 PlayerPathPointCopy[];
extern s16 OffroadFlagPlayer1[];
extern s16 TireHight;
extern Player* HitPtr[];
extern bool AcceleOnoffFlag[];
extern s32 AcceleIncFlag[];
extern s32 AcceleTime[];
extern s32 AcceleFlag[];
extern bool PowerBandAccele[];
extern s32 PowerBandTimer[];
extern bool BrakeOnoffFlag[];
extern s32 BrakeIncFlag[];
extern s32 BrakeTimer[];
extern s32 BrakeFlag[];
extern bool FullBrakeAccele[];
extern s32 FullBrakeTimer[];
extern s16 CPUChooseCharacters[];
extern s16 D_8016556E;
extern s16 SelectDMAKart;
extern s16 SelectReardmaKart;
extern s16 SelectNondmaKart;
extern s16 SelectRearNondmaKart;
extern s16 SelectViewKart;
extern s16 SelectRearviewKart;
extern s16 SelectBumpKart;
extern s16 SelectRearbumpKart;
extern s16 SelectViewObj;
extern s16 SelectRearviewObj;
/** @endcond */

#endif
