#ifndef OVERKARTHOOKS_H
#define OVERKARTHOOKS_H

/*
 * Vanilla call-site declarations only. Do not include MainInclude.h from
 * decomp C. Types come from decomp headers so this can be included after
 * or instead of an existing Player/Camera/Object set.
 */

#include <common_structs.h>
#include "camera.h"
#include <actor_types.h>
#include "code_800029B0.h"
#include <PR/gbi.h>

extern short HotSwapID;
extern short HotSwapGP;
extern short GPCourseIndex;
extern short AsmCupCount;
extern s32 OKCheckSplashEnabled;
extern s32 OKLakituIce;
extern s32 OKForceRoulette;
extern s8 OKPSelSkipCursor[4];
extern s8 OKPSelSkipPortrait[8];
extern s32 OKPSelSkipAnime;
extern s32 OKMusicIn3P4P;
extern Gfx* IceKageGfx;

void MapStartupDefault(short InputID);
void InitialMapCode(void);
void InitialMapObjectCode(void);
void PlaceIBoxes(long BoxOffset);
void GameCodeDefault(void);
void RunHotSwapScroll(void);
void TitleMenuDefault(void);
void DisplayObjectDefault(void* car, Object* actor);
void CollideObjectDefault(Player* car, Object* actor);
void AllRunDefault(void);
void UpdateRumbleHelper(void);
void PrintMenuFunctionDefault(void);
void CheckRomghostDefault(void);
void DMAROMGhostDefault(void);
void SaveFunc800B45E0Default(s32 arg0);
void SaveFunc800B4670Default(void);
void LoadFunc800B4A10Default(void);
void SaveFunc800B559CDefault(s32 arg0);
void SaveFunc800B5948Default(void);
void RubberBandManDefault(s32 PlayerId, Player* Player);

extern s16 EchoStart;
extern s16 EchoStop;
extern short Snow3DCourseID;
extern s32 g_SnowParticleTex[36];
extern f32 g_skySnowScale;
extern f32 g_skySnowVelocity;
extern s32 g_3DSnowSpawnHeight;
extern s32 g_3DSnowSpawnDistanceMin;
extern s32 g_3DSnowSpawnDistanceMax;
extern s32 g_3DSnowSpawnCone;
extern f32 g_3DSnowSwayMovement;
extern f32 g_3DSnowScale;
extern f32 g_3DSnowVelocityUpLim;
extern f32 g_3DSnowVelocityLowLim;

int LoadPressDataBypass(unsigned int RomStart, unsigned int RomEnd);
int LoadDataBypass(unsigned int RomStart, unsigned int RomEnd);
void SearchListHook(unsigned int Addr);
void SearchListFileHook(unsigned int Addr);
void SearchList2Hook(unsigned int Addr, char Surface);

void AddGravityEdit(Player* car);
void SprintLapCheck(int PlayerID, Player* car);
void WrapPathIndexAtFinishCheck(float PosX, float PosY, float PosZ, short* wayPointIndex, int PathIndex);
void InitRndSmokeHook(Player* car, short Count, int kk, char kno, char place);
void InitRapidSmokeHook(Player* car, short Count, int kk, char kno, char place);
void InitSpinSmokeHook(Player* car, short Count, int kk, char kno, char place);
void InitCustomSmoke(Player* car, Vec3f pos_R, Vec3f pos_F, s16 Count, s32 kk, s32 surface, s32 LR_flag);

void DisplayTree1Bypass(Camera* PlayerCamera, AffineMtx Affine, Object* TreeObj);
void DisplayPiranhaBypass(Camera* PlayerCamera, AffineMtx Affine, Object* PiranhaObject);
void PakkunStrategyOverride(Object* PiranhaPlant);

void ExecuteItemHookDefault(Player* Car);
void BalloonCheck(Player* Player, f32 arg1, f32 Arg2, s8 playerIndex, s8 balloonIndex, s16 Rotation);

void KWKumoAllocHookDefault(void);
void KWChartKumoHookDefault(int screen_num);
void KumoColorMode(s32 r, s32 g, s32 b);

void EventDisplay(int Player);
void EventDisplayAfter(int Player);

void DisplayKT1Hook(Screen* Display);
void DisplayKT16Hook(Screen* Display);
void XLUDisplay(Screen* Display);
void SnowCustomCheck(s32 snowIndex);
void SetGhostData(void);
void ProStickAngleHook(Player* Player, Controller* controller, s8 playerIndex);
void BombThrowRolloverWrap(Player* Player, s8 PlayerId);
void RolloverWrap(Player* Player, s8 PlayerId);
void WheelSpinWrap(Player* Player, s8 PlayerId);
void BrokenWrap(Player* Player, s8 PlayerId);
void ThunderWrap(Player* Player, s8 PlayerId);
void SpinWrap(Player* Player, s8 PlayerId);
void BombRolloverWrap(Player* Player, s8 PlayerId);
void ProWheelSpinWrap(Player* Player, s8 PlayerId);
s16 CalcOGAAreaSubBPWrapper(f32 PosX, f32 PosY, f32 PosZ, u16 trackSectionId, s32* PathIndex);
void DisplayFlagGateCheck(Camera* Camera);
void DrawPerScreenDefault(Camera* Camera);
s32 StartRaceBGMCustom(void);
void ApplyRaceTempo(void);

void PlayStarMusicHook(u8 PlayerID);
void StopStarMusicHook(u8 PlayerID);
void PlayFinalLapMusicHook(u8 PlayerID);
void LakituSpawnBypass(Player* Kart, s8 PlayerID, Vec3f SpawnVector, Vec3f FacingVector);
void ExplorerSetJugemu(Player* Player);
void ExplorerCallJugemu(Player* Player);
void ExplorerHangJugemu(Player* Player, s8 PlayerId, s8 Arg2);
void ExplorerKWDisplayJugemu(s32 CameraId);

extern char FlyCamToggle;
extern short FlyCamSection;

void MiniMapDrawDefault(void);
void LoadBigFont(void);
void RadarFinishLine(u32 ulx, u32 uly, u16* Addr);
f32 HijackScaleMinimapX(f32 x);
f32 HijackScaleMinimapY(f32 z);
s16 AddObjectBufferMKANBANHook(Vec3f pos, Vec3s angle, Vec3f velo, s16 category);
void KWDisplay2P1LR(void);
void KWDisplay2P2LR(void);
void KWDisplay2P1UD(void);
void KWDisplay2P2UD(void);
void KWDisplay4P1(void);
void KWDisplay4P2(void);
void KWDisplay4P3(void);
void KWDisplay4P4(void);
void Zanzou2(s32 Player);

void DecodeVertex2OK(char* ramaddress, u32 Number);
void DecodeSP1TriangleOK(Gfx* gfx, u8* pp, s8 Code);
void DecodeSP2TriangleOK(Gfx* gfx, u8* pp, s8 Code);
void DecodeSP1QuadrangleOK(Gfx* gfx, u8* pp, s8 Code);
void CheckDirectionFunc(int Null, Screen* display);

u16 CustomCheckBump2(Collision* Bump, f32 Radius, f32 px, f32 py, f32 pz, f32 lastx, f32 lasty, f32 lastz);
void CustomSpinKart(Player* Car, Camera* Camera, s8 place, s8 kno);
void CheckMapBGZXHook(Player* car, Vec3f normal, Vec3f Velocity, Vec3f g_vector, f32* dist, f32* new_x, f32* new_y, f32* new_z);

#endif
