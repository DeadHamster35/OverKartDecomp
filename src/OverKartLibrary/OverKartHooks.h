#ifndef OVERKARTHOOKS_H
#define OVERKARTHOOKS_H

/*
 * Vanilla call-site declarations only. Do not include MainInclude.h or
 * StockAliases.h from decomp C. Types come from decomp headers so this
 * can be included after or instead of an existing Player/Camera/Object set.
 */

#include <common_structs.h>
#include "camera.h"
#include <actor_types.h>
#include "code_800029B0.h"
#include <PR/gbi.h>

extern short HotSwapID;
extern short HotSwapGP;
extern short gpCourseIndex;
extern short asm_CupCount;
extern Gfx* IceKageGfx;

void MapStartupDefault(short InputID);
void InitialMapCode(void);
void InitialMapObjectCode(void);
void PlaceIBoxes(long BoxOffset);
void gameCodeDefault(void);
void titleMenuDefault(void);
void DisplayObjectDefault(void* car, Object* actor);
void CollideObjectDefault(Player* car, Object* actor);
void allRunDefault(void);
void PrintMenuFunctionDefault(void);
void check_romghostDefault(void);
void DMAROMGhostDefault(void);
void SaveFunc800B45E0Default(s32 arg0);
void SaveFunc800B4670Default(void);
void LoadFunc800B4A10Default(void);
void SaveFunc800B559CDefault(s32 arg0);
void SaveFunc800B5948Default(void);
void RubberBandManDefault(s32 playerId, Player* player);

extern s16 g_EchoStart;
extern s16 g_EchoStop;

int LoadPressDataBypass(unsigned int RomStart, unsigned int RomEnd);
int LoadDataBypass(unsigned int RomStart, unsigned int RomEnd);
void SearchListHook(unsigned int Addr);
void SearchListFileHook(unsigned int addr);
void SearchList2Hook(unsigned int addr, char Surface);

void AddGravityEdit(Player* car);
void SprintLapCheck(int playerID, Player* car);
void WrapPathIndexAtFinishCheck(float posX, float posY, float posZ, short* wayPointIndex, int pathIndex);
void InitRndSmokeHook(Player* car, short count, int kk, char kno, char place);
void InitRapidSmokeHook(Player* car, short count, int kk, char kno, char place);
void InitSpinSmokeHook(Player* car, short count, int kk, char kno, char place);

void DisplayTree1Bypass(Camera* PlayerCamera, AffineMtx Affine, Object* TreeObj);
void DisplayPiranhaBypass(Camera* PlayerCamera, AffineMtx Affine, Object* PiranhaObject);
void PakkunStrategyOverride(Object* PiranhaPlant);

void ExecuteItemHookDefault(Player* Car);
void BalloonCheck(Player* player, f32 arg1, f32 arg2, s8 playerIndex, s8 balloonIndex, s16 rotation);

void KWKumo_Alloc_Hook_Default(void);
void KWChart_Kumo_Hook_Default(int screen_num);
void KumoColorMode(s32 r, s32 g, s32 b);

void EventDisplay(int player);
void EventDisplay_After(int player);
void CommonGameEventChart(void);

void DisplayKT1Hook(Screen* Display);
void DisplayKT16Hook(Screen* Display);
void XLUDisplay(Screen* Display);
void SnowCustomCheck(s32 snowIndex);
void SetGhostData(void);
void ProStickAngleHook(Player* player, Controller* controller, s8 playerIndex);
void BombThrowRolloverWrap(Player* player, s8 playerId);
void RolloverWrap(Player* player, s8 playerId);
void WheelSpinWrap(Player* player, s8 playerId);
void BrokenWrap(Player* player, s8 playerId);
void ThunderWrap(Player* player, s8 playerId);
void SpinWrap(Player* player, s8 playerId);
void BombRolloverWrap(Player* player, s8 playerId);
void ProWheelSpinWrap(Player* player, s8 playerId);
s16 CalcOGAAreaSubBP_Wrapper(f32 posX, f32 posY, f32 posZ, u16 trackSectionId, s32* pathIndex);
void DisplayFlagGateCheck(Camera* camera);
void DrawPerScreenDefault(Camera* camera);

void PlayStarMusicHook(u8 playerID);
void StopStarMusicHook(u8 playerID);
void PlayFinalLapMusicHook(u8 playerID);
void LakituSpawnBypass(Player* Kart, s8 PlayerID, Vec3f SpawnVector, Vec3f FacingVector);
void ExplorerSetJugemu(Player* player);
void ExplorerCallJugemu(Player* player);
void ExplorerHangJugemu(Player* player, s8 playerId, s8 arg2);
void ExplorerKWDisplayJugemu(s32 cameraId);

void MiniMapDrawDefault(void);
void loadBigFont(void);
void RadarFinishLine(u32 ulx, u32 uly, u16* addr);
f32 HijackScaleMinimapX(f32 x);
f32 HijackScaleMinimapY(f32 z);
void KWDisplay2P_1LR(void);
void KWDisplay2P_2LR(void);
void KWDisplay2P_1UD(void);
void KWDisplay2P_2UD(void);
void KWDisplay4P_1(void);
void KWDisplay4P_2(void);
void KWDisplay4P_3(void);
void KWDisplay4P_4(void);
void Zanzou2(s32 player);

void DecodeVertex2_OK(char* ramaddress, u32 number);
void DecodeSP1Triangle_OK(Gfx* gfx, u8* pp, s8 code);
void DecodeSP2Triangle_OK(Gfx* gfx, u8* pp, s8 code);
void DecodeSP1Quadrangle_OK(Gfx* gfx, u8* pp, s8 code);
void CheckDirectionFunc(int Null, Screen* display);

u16 custom_check_bump_2(Collision* bump, f32 radius, f32 px, f32 py, f32 pz, f32 lastx, f32 lasty, f32 lastz);
void custom_SpinKart(Player* Car, Camera* camera, s8 place, s8 kno);
void CheckMapBG_ZX_Hook(Player* car, Vec3f normal, Vec3f velocity, Vec3f g_vector, f32* dist, f32* new_x, f32* new_y, f32* new_z);

#endif
