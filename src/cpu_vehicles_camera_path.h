#ifndef cpu_vehicles_camera_path_H
#define cpu_vehicles_camera_path_H

#include "vehicles.h"
#include "camera.h"
#include "path.h"
#include <assets/common_data.h>

struct unexpiredActors {
    /* 0x00 */ s32 Unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 Unk8;
    /* 0x0C */ u16 UnkC;
    /* 0x0E */ u16 ActorIndex;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ // s16 CompilerPadding;
    /* 0x14 */ u32 unk14;
    /* 0x18 */ s32 unk18;
}; // Size = 0x1C

typedef struct {
    /* 0x00 */ f32 Lane;
    /* 0x04 */ f32 LaneTarget;
    /* 0x08 */ f32 Speed;
    /* 0x0C */ f32 LaneFreeDrive;
} SOUKOU_ZURE_TYPE; // Size = 0x10
typedef SOUKOU_ZURE_TYPE TrackPositionFactorInstruction;

// Something related to CPU Item usage
typedef struct {
    /* 0x00 */ s16 ItemNum;
    /* 0x02 */ s16 ObjNum;
    /* 0x04 */ s16 Time;
    /* 0x06 */ s16 Limit;
    /* 0x08 */ s16 Count1;
    /* 0x0A */ s16 Count2;
    /* 0x0C */ s16 Count3;
    /* 0x0E */ s16 Count4;
} EnemyItem; // Size = 0x10
typedef EnemyItem CpuItemStrategyData;

typedef struct {
    s16 trg;
    s16 Mode;
    s16 Target;
    s16 Dummy;
} UnkStruct46D0;

typedef struct {
    s16 x;
    s16 z;
} Path2D;

enum CpuItemStrategyEnum {
    CPU_STRATEGY_WAIT_NEXT_ITEM = 0,

    CPU_STRATEGY_ITEM_BANANA,
    CPU_STRATEGY_HOLD_BANANA,
    CPU_STRATEGY_DROP_BANANA,

    CPU_STRATEGY_ITEM_GREEN_SHELL,
    CPU_STRATEGY_HOLD_GREEN_SHELL,
    CPU_STRATEGY_THROW_GREEN_SHELL,

    CPU_STRATEGY_ITEM_RED_SHELL,
    CPU_STRATEGY_HOLD_RED_SHELL,
    CPU_STRATEGY_THROW_RED_SHELL,

    CPU_STRATEGY_ITEM_BANANA_BUNCH,
    CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH,
    CPU_STRATEGY_DROP_BANANA_BUNCH,

    CPU_STRATEGY_ITEM_FAKE_ITEM_BOX,
    CPU_STRATEGY_HOLD_FAKE_ITEM_BOX,
    CPU_STRATEGY_THROW_FAKE_ITEM_BOX,

    CPU_STRATEGY_ITEM_THUNDERBOLT = 0x16,
    CPU_STRATEGY_END_THUNDERBOLT,

    CPU_STRATEGY_ITEM_STAR = 0x19,
    CPU_STRATEGY_END_ITEM_STAR,

    CPU_STRATEGY_ITEM_BOO,
    CPU_STRATEGY_WAIT_END_BOO,

    CPU_STRATEGY_ITEM_MUSHROOM,
    CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM,
    CPU_STRATEGY_ITEM_TRIPLE_MUSHROOM,
    CPU_STRATEGY_ITEM_SUPER_MUSHROOM,
    CPU_STRATEGY_USE_SUPER_MUSHROOM,

    CPU_STRATEGY_THROW_BANANA,
    CPU_STRATEGY_HOLD_THROW_BANANA,
    CPU_STRATEGY_END_THROW_BANANA
};

/* Function Prototypes */
s16 OgaCalcDirection(Vec3f, Vec3f);

s32 OgaHitCheck(f32, f32, f32, f32, f32, f32, f32, f32);
void OgaGuruguruSub(Vec3f, Vec3f, s16);
s32 CheckPlayerNear(Vec3f, f32, s32);
void OgaReverseChk(s32, Player*);
void SortOgawaRank(void);

void SortGoalRank(void);
void SortGoalRank2PGP(void);
s32 ExistAreaChk(u16, u16, u16, u16, u16);
void EnemyTekagenSub(s32, Player*);
void OgaResultAccel(s32, Player*, f32);

void OgaAccele(s32, f32, Player*);
bool MakeKartDistance(s32, s16, s16);
void NewBipasNumInit(s32);
s32 KyouseiBipasSet(s32, s32);
void PylonCountReflesh(s32);

void MazeEnterOrExitCheck(s32);
void MazeBipasChangeCheck(s32, Player*);
f32 OgaGetLaptime(s32, f32, f32);
void CheckLapCount(s32, Player*);
void EtcEnemyDrive(void);
void OgaEnemyVoice(s32, Player*);
void OgaEnemyVoiceOvertake(s32, Player*);
void DriveEnemyKart(s32);

void AvoidCrash(s32);
s32 StraightChk(s32, u16);
bool CourseOutChk(s32);
f32 ZureCenterSubBP(f32, f32, u16, s32);
void ZureChkBP(s32, u16, s32);
void ObjCalcFromZureDistBP(u16, f32, f32, s16);
void ObjCalcFromZureBP(u16, f32, s16);
s16 CheckOgaAreaBP(f32, f32, f32, s32);

s16 CalcOGAAreaSubBP(f32, f32, f32, u16, s32*);
s16 GetOgaAreaGroupSubBP(f32, f32, f32, s16, s32, u16);
s16 GetOgaAreaSubBP(f32, f32, f32, s16, s32);
void KyouseiAreaSetChkEnemy(f32, f32, f32, s16*, s32);
void WrapPathIndexAtFinish(f32, f32, f32, s16*, s32);
s16 CalcOgaAreaKart(f32, f32, f32, Player*, s32, s32*);
s16 GetOgaAreaKart(f32, f32, f32, s16, Player*, s32, s32);

s16 GetOgaExArea(f32, f32, f32, s16);
s16 CalcOgaAreaBP(f32, f32, f32, s32*);
s16 GetOgaAreaBP(f32, f32, f32, s16, s32);
s16 GetOgaCamAreaBP(f32, f32, f32, s16, s32);
f32 SoukouZureMove(s32);
void MokuhyouSetCenter(s32, u16);
s16 HanyouSoukouSubBP(Vec3f, s16*, f32, f32, s16, s16);
s16 HanyouGyakuSoukouSubBP(Vec3f, s16*, f32, f32, s16);
s16 HanyouSoukouSubEX(Vec3f, s16*, f32);
void InitBOM(void);
void MoveBOM(s32);

s32 SearchSetHibasiraSub(s32, s16);
s32 KameHibasiraSet(s32);
s32 KameBlueHibasiraSet(s32);
s32 KameTogeHibasiraSet(s32);
void KameHibasiraClear(s32);
void KameHibasiraInit(void);
void KameHibasiraMove(void);

void InitEnemyGameMode(void);
void SetRivalCarName(void);
void ClearPointData(CenterPathStruct*, size_t);
void InitCenterLine(void);
void InitEnemyKart(void);

void AreaBunkatuBP(s32);
void SidePointCalcBP(s32);
f32 CurveChkBP(s32, u16);
void CurveDataCalcBP(s32);
s16 AngleChkBP(s32, s32);
void AngleDataCalcBP(s32);
void ShortcutDataCalcBP(s32);
f32 CalcCyInit(f32, f32, f32, s32, s32);
f32 CalcCy(f32, f32, f32, s32, s32);

s32 AreaBunkatuSub(CenterPathStruct*, CenterPathStruct*, s32, s32);
s32 AreaReadROMSub(CenterPathStruct*, CenterPathStruct*);
s32 AreaBunkatuSLSHIPSub(Path2D*, CenterPathStruct*, s32);
void InitActDatTable(void);
void ActionFlgSleep(s32);
void ActionFlgWakeup(s32);
void ActionStartCheck(s32, Player*);
void ActionEndCheck(s32, Player*);
void ActionStartEndChk(s32);
void ActDriftStart(s32, Player*, s32, u16);

void AreaBunkatuSL(void);
void AreaBunkatuSHIP(void);
void ObjInitHighwaySub(VehicleStuff*);
void OgawaHanyouObjectInit(void);
void ClearSLTab(TrainCarStuff*, Path2D*, u16);
void InitSL(void);
void PutObjectBufSL(TrainCarStuff*, s16);
void MoveSL(void);
void HitCheckSL(s32, Player*);

void FumikiriMove(void);
void FumikiriWaitCheck(s32);
void InitSHIP(void);
void MoveSHIP(void);
void HitCheckSHIP(Player*);
void InitHIGHWAYCARSub(f32, f32, s32, s32, VehicleStuff*, CenterPathStruct*);
f32 ShasenChkSub(s16, s16);
void MoveHIGHWAYCARSub(VehicleStuff*);
void HitCheckHIGHWAYCARSub(s32, Player*, VehicleStuff*, f32, f32, s32, u32);

f32 AvoidHIGHWAYSub(s16, f32, s16);
void AvoidHIGHWAYCARSub(s32, s32, VehicleStuff*);
void InitTRUCK(void);
void MoveTRUCK(void);
void HitCheckTRUCK(s32, Player*);
void AvoidTRUCK(s32);
void InitBUS(void);
void MoveBUS(void);
void HitCheckBUS(s32, Player*);
void AvoidBUS(s32);
void InitTANK(void);
void MoveTANK(void);
void HitCheckTANK(s32, Player*);
void AvoidTANK(s32);
void InitRV(void);
void MoveRV(void);
void HitCheckRV(s32, Player*);
void AvoidRV(s32);
void CamCheckBump(s32, s32);
void OgawaCameraInit(s32);
f32 OgawaGakaku(f32, s32);

void OgaCameraRotate(Camera*, s32);
void func_8001530C(void);
void OgawaVideoInit(s32, f32, s32);
void OgawaVideoview(Camera*, Player*, s32);
void OgawaKoteiInit(s32, f32, s32, s32);
void OgawaKoteiview(Camera*, UNUSED Player*, s32, s32);
void OgawaOikakeInit(s32, f32, s32, s16);
void OgawaOikakeview(Camera*, Player*, s32, s32);

void OgawaSideInit(s32, f32, s32, s16);
void OgawaSideview(Camera*, Player*, s32, s32);
void OgawaQuickInit(s32, f32, s32);

void OgawaFaceInit(s32, f32, s32, s16);
void OgawaFaceview(Camera*, Player*, s32, s32);
void OgawaBackInit(s32, f32, s32, s16);

void OgawaBackview(Camera*, Player*, s32, s32);
void OgawaKaitenInit(s32, f32, s32, s16);
void OgawaKaitenview(Camera*, Player*, s32, s32);

void OgawaGoalInit(s32, f32, s32, s16);
void OgawaGoalview(Camera*, UNUSED Player*, s32, s32);
void OgaGPRoseErhnamDjinn(void);
void OgawaNopointInit(s32, f32, s32, s16);
void OgawaNopointview(Camera*, UNUSED Player*, s32, s32);
void OgaCameraModeInit(s32, s32);
void OgaMokuhyouAngleMove(s32, u16);
void OgaGakakuSet(s32);
void OgaBankRoll(Camera*, Player*, s32);
void OgaDriversPointCameraInit(void);
void OgaVSWinnerCameraInit(void);
void OgaTimeattackCameraInit(void);
void OgaBattleCameraInit(void);
void OGAContTrgChk(s32);

void OgaGameCameraAll(u16*, Camera*, Player*, s8, s32);
void OgaGameCamera(u16*, Camera*, Player*, s8, s32);
void GoalCameraSetGP(s32, s32);
s32 OgaDemoCameraAreaChkSub(s32, s32);
void OgaDemoCameraReInit(s32, f32, s32);
void OgaDemoCameraSoundInit(s32, f32, s32);
void OgaDemoCameraIncChangeCheck(s32, s32, s32);
void OgaDemoCameraRankSet(s32, s32, s32);
void Func8001A588(u16*, Camera*, Player*, s8, s32);
void OgawaSoundCameraSet(s16, s16, s16);
void EnemyItemInit(void);
void EnemyItemNumberSet(s32, s16*, s32);
void EnemyItemDebugBANDOU(s32, EnemyItem*);
void CheckItemNumber(EnemyItem*);
void EnemyItemMove(s32);

void OgaHyousyouCarInitSub(void);

void OgaHyousyouInit(void);
void OgaHyousyouMove(void);
void OgawaDraw(s32);
void OgawaDebugDraw(void);

/* This is where I'd put my static data, if I had any */

extern Collision D_80162E70;
extern s16 D_80162EB0; // Possibly a float.
extern s16 D_80162EB2; // possibly [3]
extern ActionData* CoursesCPUBehaviour[];
extern s16 PylonNum[];
extern s16 PylonNum2[];
extern Vec3f object;
extern Vec3f Object1;
extern Vec3f Object2;
extern s16 TrainSmokeTimer;
extern s16 BiriFlg;
extern f32 RapDat[]; // D_80162FD8
extern s16 AvoidMode[];
extern s16 AvoidCount[];
extern f32 Speedlevel[];
extern s16 OldEosAngleOga[];
extern f32 ZureDist[];
extern bool StraightFlg[];
extern u16 OgaAreaOrg;
extern s16 OgaAreaChgFlg;
extern s16 DriftFlg[];
extern s16 FerrySmokeTimer;
extern s32 EnemyTimeCount[];
extern s32 EnemyDist[];
extern s32 EnemyLimdist[];
extern f32 EnemyObjectX[];
extern f32 EnemyObjectZ[];
extern s16 SVehicleSoundRenderCounter;
extern s32 BestTime;
extern u16 TenukiFlg[];
extern u16 OldTenukiFlg[];
extern f32 Limmitspeed;
extern f32 OldLimmitspeed[];
extern s32 OgaCameraWatchNum;
extern u16 RapCountFlg[];
extern u16 OgaReverseCount[];
extern u16 WrongwayFlagPlayer1[];
extern s32 OldPylonCount[];
// Exact Pointer Type Unknown
extern ActionData* ActPointPtr;
extern u16 NextActionNum[];
extern u16 OldActionNum[];
extern u16 ActionFlg[];

enum { CPU_BEHAVIOUR_STATE_NONE, CPU_BEHAVIOUR_STATE_START, CPU_BEHAVIOUR_STATE_RUNNING };

extern s16 OgaAngle[];
extern u16 OgaCarStat[];
extern u16 EnemyRivalNum[];
extern u16 EnemyRivalName[];
extern u16 EnemyRivalAimRank[];
extern u16 EnemyAccelMode[];

enum { SPEED_CPU_BEHAVIOUR_NORMAL, SPEED_CPU_BEHAVIOUR_FAST, SPEED_CPU_BEHAVIOUR_SLOW, SPEED_CPU_BEHAVIOUR_MAX };

extern s32 OgaFrameCounter;
extern s32 OgaEtcFrameCounter;
extern s16 MokuhyouAddArea[];
extern s16 EnemyVoiceCountSPIN[];
extern s16 EnemyVoiceCountDAMAGE[];
extern s16 D_801633C8[];
extern s16 OgaHamidasiMapFlg[];
extern s16 NoSimpleKartFlag[];
extern s16 OgaNearHyousyouFlg[];
extern f32 OgaHyousyouMx[];
extern f32 OgaHyousyouMy[];
extern f32 OgaHyousyouMz[];
extern f32 OgaOldRapChkZ[];
extern s16 EnemyTargetPlayer;
// 0 or 1, only 1 when when in extra (mirror) Mode
extern s16 HighwayGyakusouFlg;
extern s16 OgaHyousyouDemoTrg;
extern s16 OgaNopointDemoTrg;
extern s32 OgaHyousyouDemoCounter;
extern s32 OgaNopointDemoCounter;
extern s32 OgaBumpChkCounter;
extern s16 RankUpdateFinishFlag;
extern s16 RivalOvertakeAllowFlag[];
extern s16 FumikiriStopFlg[];
extern s16 OgawaKarakaraShortCutOKFlg;
extern s32 OgaCalcLoopCount;
extern s32 OldOgaCalcLoopCount;
extern SOUKOU_ZURE_TYPE LaneData[];
extern Path2D* CenterLineEX;
extern s32 CourseObstacle;
extern u16 FumikiriMode[];
extern u16 FumikiriCounter[];
extern s32 D_80163DD8[];
extern struct unexpiredActors KameHibasira[];
extern EnemyItem EnemyItemTable[];
extern s16 EnemyFreeItemDebugFlg;
extern s16 EnemyFreeItemDebugInum;
extern s16 EnemyFreeItemDebugNum;
extern s16 RankToNum[]; // D_80164360
extern s16 RealRankToNum[];          // D_80164378
extern s32 LapCountByPlayerId[];          // D_80164390
extern s32 GPCurrentRaceRankByPlayerId[]; // D_801643B8
extern s32 OldOgawaRank[];
extern s32 RealRank[];
extern s16 NameToNum[];
extern s32 D_8016448C;
extern f32 CamOffset[];
extern f32 RateCountF[];    // D_801644A8
extern f32 RapCountF[]; // D_801644D0
extern f32 EnemyTenukiY[];
extern s16 EnemyGetaRank[];
extern s32 D_801645D0[];
extern s32 D_801645E8[];
extern f32 OldOgaCamX[];
extern s32 D_80164608[];
extern f32 OldOgaCamY[];
extern s32 D_80164628[];
extern f32 OldOgaCamZ[];
extern f32 CamSpeed[];
extern f32 CamSpeedObjct[];
extern s16 GakakuMode[];
extern s16 KoteiGakakuMode[];
extern s16 OgawaCameraMode[];
extern f32 OgaCameraZure[];
extern f32 OgaCameraBirdX;
extern f32 OgaCameraBirdY;
extern f32 OgaCameraBirdZ;
extern s16 KatamukiTheta[];
extern u32 OgaGoalCounter;
extern u16 D_801646CC;
extern UnkStruct46D0 OgaSoundCamera[];

// See bss_80005FD0.s
extern f32 RapDat[NUM_PLAYERS];
extern s32 D_8016448C;
extern u16 D_801637BE;
extern u16 D_80163E2A;

#define SEVERE_WRONG_DIRECTION_MIN 136
#define SEVERE_WRONG_DIRECTION_MAX 225
#define SEVERE_CORRECT_DIRECTION_MIN 45
#define SEVERE_CORRECT_DIRECTION_MAX 316
#define WRONG_DIRECTION_FRAMES_LIMIT 5

// extern Gfx D_0D0076F8[];

#endif
