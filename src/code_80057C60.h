#ifndef CODE_80057C60_H
#define CODE_80057C60_H

#include <common_structs.h>
#include "objects.h"
#include "render_player.h"
#include "camera.h"

// code_80057C60

#define RENDER_SCREEN_MODE_1P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_1P
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_2P_SPLITSCREEN_VERTICAL + 1
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE PLAYER_ONE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO PLAYER_TWO + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE PLAYER_THREE + SCREEN_MODE_3P_4P_SPLITSCREEN + 5
#define RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR PLAYER_FOUR + SCREEN_MODE_3P_4P_SPLITSCREEN + 5

#define RGB32(r, g, b) ((r << 16) | (g << 8) | (b))

typedef struct {
    char unk_00[0x4];
    Vec3f unk_04;
    char unk_10[0x4];
    s32 ObjectIndex;
    char unk_18[0x8];
} struct_D_8018CE10; // Size = 0x20

/** @cond */

void FlashcolorTable(s8, s16*, s16*, s16*);
void KWSetViewportFull(void);
void KWReturnViewport(void);
void KwdrawSafetyframe(void);
void KwdisplayKumo(u32);

void Kwdisplay3D(u32);
void Kwdisplay3D1P(void);
void Kwdisplay3D2P(void);
void Kwdisplay3D3P(void);
void Kwdisplay3D4P(void);
void Kwdisplay3DAfter(u32);
void Kwdisplay3DAfter1P(void);
void Kwdisplay3DAfter2P(void);
void Kwdisplay3DAfter3P(void);
void Kwdisplay3DAfter4P(void);
void KWDisplayEvent(s32);
void RenderSnowingEffect(s32);
void Kwdisplay2DInit(void);
void KWDisplay2D(u32);
void KWDisplay2DAfter(u32);
void Kwdisplay2D1P(void);
void Kwdisplay2D1PAfter(void);
void KWVideoFramesYori(void);

void Kwdisplay1PWipe(void);
void KwdisplayOrders(void);
void KwdisplayRanks(void);
void KwdisplayToppri(void);
void Kwdisplay2D2PLeft(void);
void KWDisplay2D2PLeftAfter(void);
void Kwdisplay2D2PRight(void);
void Kwdisplay2D2PRightAfter(void);
void Kwdisplay2D2PSub(s32);
void Kwdisplay2D2PUp(void);
void Kwdisplay2D2PUpAfter(void);
void Kwdisplay2D2PDown(void);
void Kwdisplay2D2PDownAfter(void);
void KWDisplayAfter4PSub(s32);
void Kwdisplay2D4PUpleft(void);
void Kwdisplay2D4PUpleftAfter(void);
void Kwdisplay2D4PUpright(void);
void Kwdisplay2D4PUprightAfter(void);
void Kwdisplay2D4PDownleft(void);
void Kwdisplay2D4PDownleftAfter(void);
void Kwdisplay2D4PDownright(void);
void Kwdisplay2D4PDownrightAfter(void);
void Kwplayerparams(s32);
void KwcheckRandomcnt(s32);
void KWResultscreenStart(void);
void KwcheckGhostkartItem(void);
void KwsyoriSub(s32);
void Kwpausesyori(void);
void KwgameframesyoriSub(void);

void Kwgameframesyori(void);
void KwcheckKaopanel(s32);
void KwsetKaopanel(void);
void KwcheckCommand(void);
void KWGameEventCommonVF(void);
void KWGameEventCommon(void);
void KwcheckOpeningStart(void);
void KwchartOpeningStart(void);
void KwchartOpeningNext(void);
void KwchartOpeningJump(s32);
void KwchartOpeningFinish(void);
void KwchartOpeningWait(s32);
void KwchartOpeningInit(void);
void KwchartOpeningPanelStart(void);
void KwchartOpeningAnime(void);

void KwchartGoalAnime(void);

void KwchartOpening(void);

void Convkartspeedtoangle(f32);
void Kwdisplayon(s32*);
void Kwdisplayoff(s32*);
void Displayfinish(s32);
void KwgetColor(s8, s16*, s16*, s16*);
void CheckGftimer(void);
void KwcheckRank(void);
void Kwlapcntsub(s32, s32);

void Kwlapcnt(s32);
void KwgoalRankScroll(void);
void KwcheckNearbomb(s32);

void Imageinit(void);
void Effectinit(Player*);
void MakePos(Player*, Particle*, f32, f32, f32, s8, s8);
s32 MakeStartup(Particle*, s8, f32);
s32 MakeRDP(Particle*, s32, s16);
s32 set_particle_colour_randomly_varried(Particle*, s32, s16);
void InitdriftSmokeSet(Player*, s16, s32, s8, s8);
void InitdriftSmoke(Player*, s16, s32, s8, s8);
void SetInitSmoke(void);
void SetUpSmoke(Particle*, s16, s16, s16);
void InitRndSmoke(Player*, s16, s32, s8, s8);
void InitrndSpl(Player*, s16, s32, s8, s8);
void InitRapidSmoke(Player*, s16, s32, s8, s8);

void InitSpinSmoke(Player*, s16, s32, s8, s8);

void InitengineSmoke(Player*, s16, s32, s8, s8);
void InitRingSet(Player*, s16, s32, s8, s8);
void InitRing(Player*, s16, s32, s8, s8);
void InitBubble(Player*, s16, s32, s8, s8);
void InitDobon(Player*, s16, s32, s8, s8);

void InitThunder(Player*, s16, s32, s8, s8);
void InitTeresaSet(Player*, s16, s32, s8, s8);
void InitTeresa(Player*, s16, s32, s8, s8);
void InitstarDust(Player*, s32, s32, s8, s8);
void InitresultStar(Player*, s32, s32, s8, s8);
void InitpowderDust(Player*, s16, s32, s8, s8);
void InitburnDustSet(Player*, s16, s32, s32, s32);
void InitburnDust(Player*, s16, s32, s8, s8);
void InitwaterDust(Player*, s16, s32, s8, s8);
void InittailDust(Player*, s16, s32, s8, s8);
void InitSlipstream(Player*, s16, s32, s8, s8);

void InitTurbostream(Player*, s16, s32, s8, s8);
void MakeLand(Player*, Particle*, s32);
void InitLand(Player*, s32, s32, s8, s8);
void InitcrashLetter(Player*, s8, s8, s8);
void InithitLetter(Player*, s8, s8, s8);
void InitboingLetter(Player*, s8, s8, s8);
void InitpoompLetter(Player*, s8, s8, s8);
void InitspinLetter(Player*, s8, s8, s8);
void InitonpuLetter(Player*, s8, s8, s8);
void MakeSmokepos(f32*, f32*, f32*, f32, f32, f32, u16, u16);
void EngineSmoke(Player*, s16, s32, s32);
void RandomBubble(Player*, s16, s8, s8);

void DobonSplash(Player*, s16, s8, s8);
void ThunderEffect(Player*, s16, s8, s8);
void TeresaEffect(Player*, s16, s8, s8);
void DriftSmoke(Player*, s16, s8, s8);
void RandomSmoke(Player*, s16, s8, s8);
void RapidSmoke(Player*, s16, s8, s8);
void RandomGrass(Player*, s16, s8, s8);
void RandomSpl(Player*, s16, s8, s8);
void RandomTurbo(Player*, s16, s32, s32);

void RandomRing(Player*, s16, s8, s8);
void SpinSmoke(Player*, s16, s8, s8);
void StarDust(Player*, s16, s8, s8);
void ResultStar(Player*, s16, s8, s8);
void LandSmoke(Player*, s16, s8, s8);
void BurnDust(Player*, s16, s8, s8);
void WaterDust(Player*, s16, s8, s8);
void TailDust(Player*, s16, s8, s8);
void PowderDust(Player*, s16, s8, s8);
void SlipStream(Player*, s16, s8, s8);
void CrashLetter(Player*, s8, s8, s8);
void HitLetter(Player*, s8, s8, s8);
void BoingLetter(Player*, s8, s8, s8);

void PoompLetter(Player*, s8, s8, s8);
void SpinLetter(Player*, s8, s8, s8);
void OnpuLetter(Player*, s8, s8, s8);
void CommonMTXCalc(Vec3f, Vec3s, f32);
void SetGassCalc(Player*, s8, s16, s8);
void SetTeresaCalc(Player*, s8, s16, s8);
void SetDriftCalc(Player*, s8, s16, s8);
void SetSmokeCalc(Player*, s8, s16, s8);

void SetSplCalc(Player*, s8, s16, s8);
void SetRingCalc(Player*, s8, s16, s8);
void SetThunderCalc(Player*, s8, s16, s8);

void SetSlipstreamCalc(Player*, s8, s16, s8);
void SetTurbostreamCalc(Player*, s8, s16, s8);
void SetSpinCalc(Player*, s8, f32, s8, s8);
void SetFukidashiCalc(Player*, s8, u8*, s8, f32, s32);

void SetWordCalc(Player*, s8, u8*, s8, f32, s32);
void SetCrashCalc(Player*, s8, f32, s8, s8);
void SetHitCalc(Player*, s8, f32, s8, s8);
void SetBoingCalc(Player*, s8, f32, s8, s8);
void SetPoompCalc(Player*, s8, f32, s8, s8);

void SetStarCalc(Player*, s8, s16, s8);
void LandCalc(Player*, s8, s16, s8);
void SetStartailCalc(Player*, s8, s16, s8, f32);
void PowderCalc(Player*, s8, s16, s8);
void BurnCalc(Player*, s8, s16, s8);
void WaterCalc(Player*, s8, s16, s8);

void SetBubbleCalc(Player*, s8, s16, s8);
void SetDobonCalc(Player*, s8, s16, s8);
void BalloonDisp(Player*, f32, f32, s8, s8, s16);
void BalloonStrat(Player*, f32, f32, s8, s8);
void MakeBalloonCalc(Player*, s8, s16, s8);

void InitAllPlayerBalloons(Player*, s8);
void AllkillBalloon(Player*, s8);
void CutPlayerBalloon(Player*, s8);
void KillBalloon(s32, s8, s8);
void Balloonset(Player*, s8);
void SetBalloonCalc(Player*, s8, s8);
void DrawBalloon(Vec3f, f32, s16, s16);

void DrawSmoke(Vec3f, f32, s32, s16);
void DrawStar(Vec3f, f32, s32, s16);
void DrawPachipachi(Vec3f, f32, s32, s16, s16);
void Smokeset(Player*, s16, s8, s8);
void Sparkset(Player*, s16, s8, s8);
void Gassset(Player*, s16, s8, s8);

void Letterset(Player*, s8, s8);
void OBJCalculation(Player*, s8, s8);
void ExceObjCalculation(Player*, s8, s8);
void ObjCalculation1(Player*, s8, s8);

void KartEffectStrategy(void);
void EffectStrategy(Player*, s8, s8);
void SmokeDisp(Player*, s8, s8);
void SmokeDisp2P(Player*, s8, s8);
void SmokeDisp3P(Player*, s8, s8);
void SmokeDisp4P(Player*, s8, s8);
void SmokeDispAfter(Player*, s8, s8);
void SmokeDispAfter2P(Player*, s8, s8);
void SmokeDispAfter3P(Player*, s8, s8);
void SmokeDispAfter4P(Player*, s8, s8);
void func_80075CA8(void);
void func_80085214();

extern s16 D_800E4730[];
extern u8** D_800E4770[];
extern u8** D_800E47A0[];
extern s32 NORM_SMOKE_IN[];
extern s32 NORM_SMOKE_OUT[];

extern f32 WaterlevelPlayer[];

extern s32 KW16GFTimer;
extern s32 KW8GFTimer;
extern s32 KW4GFTimer;
extern s32 KW2GFTimer;
extern s32 KW64GFCount;
extern s32 KW32GFCount;
extern s32 KW16GFCount;
extern s32 KW8GFCount;
extern s32 D_801655C0;
extern s32 KW4GFCount;
extern s32 D_801655C8;
extern s32 KW2GFCount;
extern s32 D_801655D8;
extern s32 D_801655E8;
extern s32 D_801655F0;
extern s32 D_801655F8;

extern s32 KWFlash8;
extern s32 D_80165618;
extern s32 D_80165628;
extern u32 D_80165638;
extern u32 D_80165648;
extern u32 D_80165658[];
extern s32 D_80165678;
extern u16 D_801656B0;
extern u16 KwflashR;
extern u16 KwflashG;
extern u16 KwflashB;
extern s16 D_801656F0;
extern s16 D_80165708;
extern s16 D_80165710;
// extern s16 D_80165730; -> objects.h
// extern s16 D_80165738; -> objects.h

extern s16 D_80165740;
extern s16 D_80165748;
extern s16 D_80165718;
extern s16 D_80165720;
extern s16 D_80165728;

// extern s16 NumActiveThwomps; -> objects.h

extern s32 D_80165754;
// extern ThwompSpawn *ThowmpSpawnList; -> objects.h

extern Vec4s D_80165760;
extern s8 D_8016576A;
extern Vec4s D_80165770;
extern s16 D_8016578C;
extern Vec4s D_80165780;

extern s16 D_80165790;
extern s16 D_80165794;
extern s8 D_8016579C;
extern u16 D_8016579E;
extern u16 D_801657A2;
extern s8 D_801657AE;
extern s8 KWDBDispSW;
extern s8 D_801657B2;
extern s8 D_801657B4;
extern s8 D_801657B8[];
extern s8 CloudsToggle;
extern s8 D_801657D0[];
extern s8 KWDemoSW;
extern s8 D_801657E1;
extern s8 Kwgoalpanelsw;
extern s8 D_801657E3;
extern s8 HUDLapToggle;
extern s8 D_801657E5;
extern bool8 HUDSpeedToggle2;
extern u8 D_801657E7;
extern bool8 HUDMapToggle2;
extern bool8 BlueLineRankToggle;
extern bool8 KWLapSW;
extern s32 D_801657FC;

extern s8 D_80165800[2];
extern s32 D_80165804;
extern s8 HUDToggleFlag;
extern s32 D_8016580C;
extern bool8 HUDSpeedToggle;
extern s32 D_80165814;
extern bool8 D_80165818;
extern s32 D_8016581C;
extern s8 D_80165820;
extern s8 D_80165828;
extern Vec3su D_8016582C;
extern s8 HUDToggleFlagP2[2];
extern Vec3su D_80165834;
extern s8 D_80165840[];
extern s32 D_80165860;
extern s32 D_8016586C;
extern s32 D_80165878;
extern s32 D_8016587C;
extern u8* D_80165880;
extern s8 D_80165888;
extern s8 D_80165890;
extern s8 Balloonsw;
extern s32 D_8016589C;
extern s8 D_801658A8;
extern s8 D_801658BC;
extern s8 D_801658C6;
extern s8 D_801658CE;
extern s8 D_801658D6;
extern s8 D_801658DC;
extern s8 D_801658E4;
extern s8 D_801658EC;
extern s8 D_801658F4;
extern u8 SRandomItemIndex;
extern s8 D_801658FE;
extern u8 ControllerRandom;

extern s16 D_80165900;
extern s8 D_80165908;

extern s8 D_80165A90;
// extern AnmObject DynamicObjects[]; -> objects.h

extern s32 ObjectListSize;
extern Mtx D_80183D60;
// extern s32 D_80183DA0; -> objects.h

extern f32 D_80183DA8[];
// extern s32 Jugemuallocptr[]; -> objects.h

extern f32 D_80183DC8[];
// extern s32 Bomballocptr[]; -> bomb_kart.h
// extern s32 NextFreeObjectParticle1; -> objects.h

extern Vec3f D_80183E40;
// extern s32 NextFreeObjectParticle2; -> objects.h

extern Vec3f D_80183E50;
// extern s32 NextFreeObjectParticle3; -> objects.h
// extern s32 FireParticleCounter; -> objects.h

extern Vec3f D_80183E70;
// extern s32 NextFreeLeafParticle; -> objects.h

extern Vec3su D_80183E80;
// extern s32 ItemBoxAllocPtr[]; -> objects.h

extern Vec3su D_80183E98;
// extern s32 Objallocptr[]; -> objects.h
// extern s32 Objallocptr2[]; -> objects.h

extern u8 D_80183FA8[4][0x2000];
// extern s32 Objallocptr3[]; -> objects.h

extern u8* LakituTexturePtr;
// extern s32 Objallocptr4[]; -> objects.h
// extern Collision D_8018C0B0[]; -> objects.h
// extern s32 EffectAllocArray1[]; -> objects.h

extern Collision D_8018C3B0;
// extern s32 EffectAllocArray2[]; -> objects.h
// extern s32 EffectAllocArray3[]; -> objects.h

extern Collision D_8018C830;
// extern s32 FireParticleAllocArray[]; -> objects.h
// extern s32 Leafallocptr[]; -> objects.h

extern Hud PlayerHUD[];
// extern s32 CloudAllocate[]; -> objects.h

extern struct_D_8018CE10 D_8018CE10[];
// extern s32 D_8018CF10; -> objects.h

extern Camera* D_8018CF14;
extern s16 D_8018CF18;
extern Player* D_8018CF1C;
extern s16 D_8018CF20;
extern Player* D_8018CF28[];
extern s16 D_8018CF48;
extern s16 D_8018CF50[];
extern s16 D_8018CF60;
extern s16 D_8018CF68[];
extern s16 D_8018CF78;
extern s16 GPCurrentRaceCharacterIdByRank[];
extern f32 D_8018CFCC;
extern f32 D_8018CFD4;
extern s16 D_8018CF90;
extern s16 KWRank[];
extern s16 D_8018CFA8;
extern s16 D_8018CFB0;
extern s16 D_8018CFB8;
extern s16 D_8018CFC0;
extern s16 D_8018CFC8;
extern s16 D_8018CFD0;
extern s16 D_8018CFD8;
extern u8 D_8018CFAC[];
extern u8 D_8018CFB4[];
extern u8 D_8018CFBC[];
extern u8 D_8018CFC4[];
extern s16 D_8018CFE0;
extern f32 D_8018CFE4;
extern s16 D_8018CFE8;
extern f32 D_8018CFEC;
extern s16 D_8018CFF0;
extern f32 D_8018CFF4;
extern s16 D_8018CFF8;
extern s16 D_8018D000;
extern s16 D_8018D008;
extern f32 D_8018D00C;
extern s16 D_8018D010;
extern s16 D_8018D018;
extern f32 XOrientation;
extern s16 D_8018D020;
extern f32 Panelx[];
extern s16 D_8018D048;
extern f32 HUDCharpicRankY1[];
extern s16 D_8018D070;
extern f32 D_8018D078[];
extern s16 D_8018D098;
extern f32 D_8018D0A0[];
extern s16 D_8018D0C0;
extern f32 D_8018D0C8[];
extern s16 D_8018D0E8;
extern f32 D_8018D0F0[];
extern s16 D_8018D110;
extern s32 D_8018D114;
extern s32 Modelingnum;
extern s32 D_8018D140;
extern s32 D_8018D150;
extern s32 Kwkartnum;
extern s32 D_8018D160;
extern s32 D_8018D168;
extern s16 D_8018D16C;
extern s32 D_8018D170;
// extern s16 D_8018D174; -> objects.h

extern s32 D_8018D178;
// extern s16 D_8018D17C; -> objects.h

extern s32 D_8018D180;
extern s16 D_8018D184;
extern s32 KWScreenEnable;
extern s16 D_8018D18C;
extern s32 D_8018D190;
// extern s8 D_8018D198[]; -> objects.h

extern s32 D_8018D1A0;
// extern s8 D_8018D1A8[]; -> objects.h

extern s32 D_8018D1B4;
// extern s8 D_8018D1B8[]; -> objects.h

extern s32 D_8018D1C4;
// extern s32 D_8018D1C8; -> objects.h

extern s32 D_8018D1CC;
// extern s32 D_8018D1D0; -> objects.h

extern s32 D_8018D1D4;
// extern s32 D_8018D1D8; -> objects.h

extern s32 D_8018D1DC;
extern u8* Logoptr;
extern f32 D_8018D1E8;
extern s32 D_8018D1EC;
// extern s32 CloudCount1P; -> objects.h
// extern s32 CloudCount2P; -> objects.h

extern s32 D_8018D1FC;
extern s16 D_8018D200;
extern s32 D_8018D204;
extern s16 D_8018D208;
extern s32 D_8018D20C;
extern s16 D_8018D210;
extern bool D_8018D214;
extern s16 D_8018D218;
extern s32 D_8018D21C;
extern u8 (*Kumoptr)[1024];
extern s32 D_8018D224;
extern u8 Kumotype;
extern s32 D_8018D22C;
// extern s8 Kwhoshisw; -> objects.h

extern s32 MapTexture;
extern u8* D_8018D248[];
extern f32 MapScale;
extern s32 D_8018D2A4;
extern s32 D_8018D2AC;
extern s16 MapWidth;
extern s32 D_8018D2B4;
extern s16 MapHeight;
extern s32 D_8018D2BC;
extern s16 MapX[];
extern s32 D_8018D2C8[];
extern s16 MapY[];
extern s16 StartX;
extern s16 StartY;
extern s16 Rzerox;
extern s16 Rzeroy;
extern u16 MapR;
extern u16 MapG;
extern u16 MapB;
extern s32 D_8018D314;
extern u16 Ralpha;
extern s32 Kwgoalcnt;
extern s32 D_8018D380;
extern s32 D_8018D384;
extern s32 D_8018D388;
// extern s32 D_8018D3BC; -> objects.h
// extern s32 D_8018D3C0; -> objects.h
// extern s32 D_8018D3C4; -> objects.h

extern s32 Kwr;
extern s32 Kwg;
extern s32 Kwb;
extern s32 Kwa;
extern s32 Wakur;
extern s32 Wakug;
extern s32 Wakub;
extern s32 Wakua;
extern s32 Wakuanmtmmax;
extern s32 Wakuanmptr;
extern s32 Kwvideoframetimer;
extern s32 Kwgameframetimer;
extern s32 D_8018D40C;
extern s32 D_8018D410;
extern u8* DriftSmokeV;
extern u8* DriftSmokeE;
extern u8* D_8018D428;
extern u8* D_8018D42C;
extern u8* D_8018D430;
extern u8* D_8018D434;
extern u8* Dobon1;
extern u8* Dobon2;
extern u8* Dobon3;
extern u8* Dobon4;
extern u8* Dobon5;
extern u8* Dobon6;
extern u8* Dobon7;
extern u8* Dobon8;
extern u8* Dobon9;
extern u8* Dobon10;
extern u8* Dobon11;
extern u8* Dobon12;
extern u8* Dobon13;
extern u8* Dobon14;
extern u8* Dobon15;
extern u8* KageBitmap;
extern u8* D_8018D478;
extern u8* Fukidashi2;
extern u8* Onpu;
extern u8* Hoshi;
extern u8* D_8018D48C;
extern u8* D_8018D490;
extern u8* smoke;
extern u8* Grs;
extern u8* Bubble;
extern u8* Boing;
extern u8* Poomp1;
extern u8* Poomp2;
extern u8* Whir1;
extern u8* Whir2;
extern u8* Crash1;
extern u8* Crash2;
extern u8* Balloon1;
extern u8* Balloon2;
extern u8* ThunderA;
extern u8* ThunderB;
extern Vec3f PlayerBalloonPosX[];
extern Vec3f PlayerBalloonPosY[];
extern Vec3f PlayerBalloonPosZ[];
extern u16 PlayerBalloonStatus[8][3];
extern Vec3s D_8018D620[];
extern Vec3f D_8018D650[];
extern Vec3f D_8018D6B0[];

extern Vec3f D_8018D710[];
extern Vec3s D_8018D770[];
extern Vec3s D_8018D7A0[];
extern Vec3s D_8018D7D0[];

extern Vec3s D_8018D800[];
extern Vec3s D_8018D830[];
extern Vec3s PlayerBalloonRotation[]; // Sprite Rotation for each Player balloon
extern Vec3s D_8018D890[];
extern s16 BalloonCount[]; // D_8018D8C0
extern Vec3s PlayerBalloonDepartingTimer[];

/** @endcond */

#endif
