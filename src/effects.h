#ifndef EFFECTS_H
#define EFFECTS_H

#include <common_structs.h>

/* Function Prototypes */

void KWGetTimeInit(void);
s32 KWStartTime(s32*);
void KWGetTime(s32*, s32);
void KWAddTime(s32);
void KWGetLine(s32);
void KWSetTime(void);
void Kwdebug(void);
void KwdebugItems(void);
void KwdebugRcp(void);
void DebugsyoriTest(void);
void KwdebugAnmbreak(void);
void KwdebugMtxkazu(void);
void KwdebugGptrkazu(void);
void KwdebugKumoram(void);
void KwdebugRmonprint1P(void);
void Debugsyori1P(void);
void Debugsyori2PLr(void);
void Debugsyori2PUd(void);
void Debugsyori4PKleft(void);
void Debugsyori4PKright(void);
void Debugsyori4PKup(void);
void Debugsyori4PKdown(void);
void Debugsyori4P(void);
void KwdisplayDebug(void);
void KwdebugPrintCommon(void);
void KwdebugprintPlayerSub(void);
void KwdebugprintOgawa(void);
void KwdebugprintPlayer(s32, s32);
void KwdebugprintSyori(void);
void KwdebugprintTest(void);
void Kwdebugprint1P(void);
void Kwdebugprint2PLeft(void);
void Kwdebugprint2PRight(void);
void Kwdebugprint2PUp(void);
void Kwdebugprint2PDown(void);
void Kwdebugprint4PUpleft(void);
void Kwdebugprint4PUpright(void);
void Kwdebugprint4PDownleft(void);
void Kwdebugprint4PDownright(void);
void CheckHitLetter(Player*);
void StopStrat(void);
void KillStrat(Player*, s8);
void SetRollover(Player*, s8);
void Rollover(Player*, s8);
void ResetRollover(Player*, s8);
void SetSpin(Player*, s8);
void ResetSpin(Player*, s8);
void Spin(Player*, s8);
void SetWheelspin(Player*, s8);
void Wheelspin(Player*, s8);
void ResetWheelspin(Player*, s8);
void SetProWheelSpin(Player*, s8);
void ProWheelspin(Player*, s8);
void ResetProWheelspin(Player*, s8);
void SetTurbo(Player*, s8);
void NowTurbo(Player*);
void ResetTurbo(Player*);
void SetWing(Player*, s8);
void Wing(Player*, s8);
void ResetWing(Player*);
void SetStorm(Player*, s8);
void Storm(Player*, s8);
void ResetStorm(Player*);
void RollBrokenkart(Player*);
void SetBroken(Player*, s8);
void Broken(Player*, s8);
void SetThunder(Player*, s8);
void Thunder(Player*, s8);
void ResetThunder(Player*, s8);
void Bomb(Player*, s8);
void SetBombRollover(Player*, s8);
void ResetBomb(Player*, s8);
void BombThrow(Player*, s8);
void SetBombThrowRollover(Player*, s8);
void ResetBombThrow(Player*, s8);
void SetDashjump(Player*, s8);
void NowDashjump(Player*);
void ResetDashjump(Player*);
void SetDashMidjump(Player*, s8);
void NowDashMidjump(Player*);
void ResetDashMidjump(Player*);
void SetRapidacc(Player*, s8);
void Rapidacc(Player*, s8);
void ResetRapidacc(Player*);
void RolloverFall(Player*, s8);
void SetRolloverFall(Player*, s8);
void ResetRolloverFall(Player*, s8);
void Star(Player*, s8);
void SetStar(Player*, s8);
void ResetStar(Player*, s8);
void VSGhost(Player*, s8);
void SetVSGhost(Player*, s8);
void ResetVSGhost(Player*, s8);
void KillBombKart(Player*);
void ChangeBomb(Player*, s8);
void RealBomb(Player*, s8);
void SetChangeBomb(Player*, s8);
void SetRealBomb(Player*, s8);
void SetHeightJump(Player*, s8);
void HeightJump(Player*, s8);
void ResetHeightJump(Player*, s8);
void MakeChasepoint(Player*, s8);
void func_80090178(Player*, s8, Vec3f, Vec3f);
void SetLakitu(Player*);
void func_80090868(Player*);
void HangLakitu(Player*, s8, s8);
bool CheckItembutton(Player*);
void SetResultJump(Player*, s8);
void ResultJump(Player*, s8);

// audio/external.c
void NaPlyDashStart(u8);
void NaStarBGMStart(u8);
void NaStarBGMStop(u8);
void NaEnmMutekiStart(u8);
void NaPlyObakeStart(u8);
void NaPlyObakeStop(u8);

extern s32 D_8018D900[];
extern s16 D_8018D920[];
extern s32 StarUseCounter[];
extern s32 GhostUseCounter[];
extern s32 GhostUseTimer[];
extern s32 D_8018D990[];

/* This is where I'd put my static data, if I had any */

extern f32 D_800E3710[];
extern f32 D_800E3730[];
extern f32 D_800E3750[];
extern f32 D_800E3770[];
extern f32 D_800E37B0[];
extern f32 D_800E3790[];
extern f32 D_800E37D0[];
extern f32 D_800E37F0[];
extern s16 CompelFlag[4][8];
extern f32 AcceleCount[];
extern s32 AcceleTime[];

#endif
