#ifndef player_controller_H
#define player_controller_H

#include <common_structs.h>
#include "camera.h"

/* Function Prototypes */

void SpinKart(Player*, Camera*, s8, s8);

s32 CheckKartNumber(Player*);
void StartKartEngine(Player*, s8);
void KartEngineEcho(Player*, s8);

void TirePosition(Player*, f32, f32, f32);
void Openkart(Player* Player, Camera* Camera, s8 Arg2, s8 PlayerId);

void MariogpMovekart(Player*, Camera*, s8, s8);
void VSTimeMovekart(Player*, Camera*, s8, s8);
void BattleMovekart(Player*, Camera*, s8, s8);
void Movekart(Player*, Camera*, s8, s8);
void func_80028F5C(s32, s32, s32, s32);
void KartposControlFull(void);

void KartposControlRu(void);
void KartposControlLd(void);
void KartpostControl1P(void);
void KartpostControl2P(void);
void KartposControl3P(void);
void KartposControl4P(void);
void MakeDegpict(Player*, s8);
void KartPosset(Player*, Camera*, s8, s8);

void EnemyTirePosition(Player*, f32, f32, f32);
void HitVelocity(Vec3f, f32, Vec3f, f32, f32);
void SetStartDush(Player*, s8);
void DriftTurbo(Player*, s8);
void CheckDriftTurbo(Player*, s8);
void JumpSet(Player*);
void DriftJump(Player*);
void AddGravity(Player*);
void SetNatureVector(void);
void CheckNatureVector(void);
void CheckDirection(void);
void SetSlipAngle(Player*, s8, f32, f32, f32, f32);

void EnemyCheckdrift(Player*);
void SetWeapon(Player*, s8, s8);
void CheckWeapon(Player*, s8, s8);
void WeaponStatus(Player*, s8, s8);
void SetHitptr(Player*, Player*);
void CheckImpact(Player*, s8, s32);
void CheckImpactsterr(Player*);
void CheckSlipstream(Player*, s8);

void DriftCounter(Player*);
void LakituCheck(Player*, s8);
void CheckSplash(Player*, s8);
void CheckKartHit(Player*, s8, s8);
void CheckWall(Player*, s8, Vec3f);
void SetStrategy(Player*, s8, s8);

void OpeningKart(Player*, s8);
void RunKart(Player*, Camera*, s8, s8);

void SetHight(Player*);
void RunKartSimple(Player*, Camera*, s8, s8);

void SlipCheck(Player*, s8);
void MakeTaco(Player*, f32);

f32 PowerCheck(Player*, s8);
void ShakeSet(Player*);
void CheckAcceleOff(Player*);
void CheckAcceleOn(Player*);
void AccelOn(Player*);
void AccelOff(Player*, f32);
void CheckBrakeOff(Player*);
void CheckBrakeOn(Player*);
void BreakeOn(Player*);
void PlayerNocontacceleOn(Player*);
void PlayerNocontacceleOff(Player*, f32);
void JugemuAcceleOn(Player*);
void JugemuAcceleOff(Player*, f32);
void EmptyAcceleOn(Player*, s32);
void EmptyAcceleOff(Player*, f32, s32);
void RecoverGrip(Player*, f32);
void CheckSterrSpeedgrip(Player*, s32*, s32*, s32, s32, s32, s32);
void CheckSterrSpeedgrip1(Player*, s32*, s32, s32, f32);
void CheckSterrSpeed1(Player*, s32*, s32, s32, f32);
void CheckSterrSpeed(Player*, s32*, s32*, s32, s32, s32, f32);
void ProStickAngle(Player*, struct Controller*, s8);

void EnemyStickAngle(Player*, s16);
void DriftSet(Player*);
void DriftReset(Player*);
void SetvectorL(Player*, Vec3f, Vec3f);

void SetvectorR(Player*, Vec3f, Vec3f);
void EnemySetvectorL(Player*, Vec3f, Vec3f);
void EnemySetvectorR(Player*, Vec3f, Vec3f);
void BattleSetvectorL(Player*, Vec3f, Vec3f);
void BattleSetvectorR(Player*, Vec3f, Vec3f);
void YokoG(Player* Player, Vec3f);
void ManualStickDrive(Player*, struct Controller*, s8);

void DriveStickKindcontrol(Player*, struct Controller*, s8);
void DriveStickControl(void);
s16 CheckStickX(struct Controller*);
s16 CheckStickY(struct Controller*);
void ResultAcceleOn(Player*, s16);
void ResultRunkart(Player*, Camera*, s8, s8);

/* This is where I'd put my static data, if I had any */

extern s16 CPUForMario[];
extern s16 CPUForLuigi[];
extern s16 CPUForYoshi[];
extern s16 CPUForToad[];
extern s16 CPUForDk[];
extern s16 CPUForWario[];
extern s16 CPUForPeach[];
extern s16 CPUForBowser[];
extern s16* CPUForPlayer[];

extern s16 CPUForMarioAndLuigi[];
extern s16 CPUForMarioAndYoshi[];
extern s16 CPUForMarioAndToad[];
extern s16 CPUForMarioAndDk[];
extern s16 CPUForMarioAndWario[];
extern s16 CPUForMarioAndPeach[];
extern s16 CPUForMarioAndBowser[];
extern s16 CPUForLuigiAndMario[];
extern s16 CPUForLuigiAndYoshi[];
extern s16 CPUForLuigiAndToad[];
extern s16 CPUForLuigiAndDk[];
extern s16 CPUForLuigiAndWario[];
extern s16 CPUForLuigiAndPeach[];
extern s16 CPUForLuigiAndBowser[];
extern s16 CPUForYoshiAndMario[];
extern s16 CPUForYoshiAndLuigi[];
extern s16 CPUForYoshiAndToad[];
extern s16 CPUForYoshiAndDk[];
extern s16 CPUForYoshiAndWario[];
extern s16 CPUForYoshiAndPeach[];
extern s16 CPUForYoshiAndBowser[];
extern s16 CPUForToadAndMario[];
extern s16 CPUForToadAndLuigi[];
extern s16 CPUForToadAndYoshi[];
extern s16 CPUForToadAndDk[];
extern s16 CPUForToadAndWario[];
extern s16 CPUForToadAndPeach[];
extern s16 CPUForToadAndBowser[];
extern s16 CPUForDkAndMario[];
extern s16 CPUForDkAndLuigi[];
extern s16 CPUForDkAndToad[];
extern s16 CPUForDkAndYoshi[];
extern s16 CPUForDkAndWario[];
extern s16 CPUForDkAndPeach[];
extern s16 CPUForDkAndBowser[];
extern s16 CPUForWarioAndMario[];
extern s16 CPUForWarioAndLuigi[];
extern s16 CPUForWarioAndYoshi[];
extern s16 CPUForWarioAndDk[];
extern s16 CPUForWarioAndToad[];
extern s16 CPUForWarioAndPeach[];
extern s16 CPUForWarioAndBowser[];
extern s16 CPUForPeachAndMario[];
extern s16 CPUForPeachAndLuigi[];
extern s16 CPUForPeachAndYoshi[];
extern s16 CPUForPeachAndDk[];
extern s16 CPUForPeachAndWario[];
extern s16 CPUForPeachAndToad[];
extern s16 CPUForPeachAndBowser[];
extern s16 CPUForBowserAndMario[];
extern s16 CPUForBowserAndLuigi[];
extern s16 CPUForBowserAndYoshi[];
extern s16 CPUForBowserAndDk[];
extern s16 CPUForBowserAndWario[];
extern s16 CPUForBowserAndToad[];
extern s16 CPUForBowserAndPeach[];
extern s16* ListCPUforMario[];
extern s16* ListCPUforLuigi[];
extern s16* ListCPUforYoshi[];
extern s16* ListCPUforToad[];
extern s16* ListCPUforDk[];
extern s16* ListCPUforWario[];
extern s16* ListCPUforPeach[];
extern s16* ListCPUforBowser[];
extern s16** CPUForTwoPlayer[];

extern s16 D_801656F0;

#endif
