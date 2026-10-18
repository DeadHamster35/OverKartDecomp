#ifndef STOCK_ALIASES_H
#define STOCK_ALIASES_H

/*
 * Library labels that are not the decomp identifier.
 * Host override Names (*Default) are documented in docs/host_overrides.md.
 * DynamicP is already the decomp name (Main.h).
 * DMA / decodeMIO0 / AllocateMemory come from decomp headers.
 *
 * Name-alikes (same spelling as decomp) belong in the decomp header, not here
 * and not as a second GameOffsets prototype. Keep a #define only when it wraps
 * (arity/Type) or maps a different OverKart name onto a stock symbol.
 *
 * After adding a #define here, remove the matching GameOffsets / StatsOffsets
 * extern so the preprocessor does not rewrite `extern float name` into
 * `extern float array[0]`.
 */

#include "kart_attributes.h"
#include <math.h>
#include "menus.h"
#include "audio/external.h"
#include <PR/libaudio.h>
#include "code_80057C60.h"
#include "render_objects.h"

extern ALSeqFile *MUSTablePointer;
extern ALSeqFile *AlCtlHeader;
extern ALSeqFile *AlTbl;

extern Hud PlayerHUD[];
extern s32 LapCountByPlayerId[];
extern s32 GPCurrentRaceRankByPlayerId[];
extern ActionData *CoursesCPUBehaviour[];
extern s16 KoteiGakakuMode[];
extern struct ObjBlock MenuItems[];
#define GlobalMenuHUD MenuItems
extern struct PlayerTextureTable D_800E8320[];
#define SelectPortraitTable D_800E8320
extern Gfx D_0D007B00[];
extern u16 common_texture_hud_normal_digit[];
void func_80090868(Player *);
void func_80090178(Player *, s8, Vec3f, Vec3f);
u8 *LoadCourse(s32);
void UpdateActorFallingRocks(struct FallingRock *);
void SprDrawClip(s32, s32, s32, s32, s32);

void InitMKCode(void);
#define ramCopy(dst, src, n) memcpy((void *) (uintptr_t) (dst), (const void *) (uintptr_t) (src), (size_t) (n))
#define MemCpyN64 ramCopy

#define GlobalPlayer GamePlayers
#define g_PlayerStructTable GamePlayers[0]
#define g_PlayerStruct1 GamePlayers[0]
#define g_PlayerStruct2 GamePlayers[1]
#define g_PlayerStruct3 GamePlayers[2]
#define g_PlayerStruct4 GamePlayers[3]
#define g_PlayerStruct5 GamePlayers[4]
#define g_PlayerStruct6 GamePlayers[5]
#define g_PlayerStruct7 GamePlayers[6]
#define g_PlayerStruct8 GamePlayers[7]

#define g_PlayerInputTable Controllers
#define g_Controller1 Controllers[0]
#define g_Controller2 Controllers[1]
#define g_Controller3 Controllers[2]
#define g_Controller4 Controllers[3]
#define g_ControllerMenu Controllers[4]
#define g_ControllerGhost1 Controllers[5]
#define g_ControllerGhost2 Controllers[6]
#define g_ControllerGhost3 Controllers[7]

#define g_CameraTable Cameras[0]
#define g_Camera1 Cameras[0]
#define g_Camera2 Cameras[1]
#define g_Camera3 Cameras[2]
#define g_Camera4 Cameras[3]

#define g_ScreenStruct D_8015F480[0]
#define g_Screen1 D_8015F480[0]
#define g_Screen2 D_8015F480[1]
#define g_Screen3 D_8015F480[2]
#define g_Screen4 D_8015F480[3]

#define g_hudStruct PlayerHUD
#define g_hudPlayer1 PlayerHUD[0]
#define g_hudPlayer2 PlayerHUD[1]
#define g_hudPlayer3 PlayerHUD[2]
#define g_hudPlayer4 PlayerHUD[3]
#define KWLap PlayerHUD

#define GraphPtrOffset DisplayListHead
#define GraphPtr DisplayListHead
Gfx* FillRect1ColorF(Gfx*, s32, s32, s32, s32, u32, u32, u32, u32);

#define MapStartup MapStartupDefault
#define KWKumo_Alloc_Hook KWKumoAllocHookDefault
#define KWChart_Kumo_Hook KWChartKumoHookDefault

/* libc */
#define Sqrtf sqrtf
#define sinF sinf
#define cosF cosf

/* NTSC TV Type lives at 0x80000300 */
#define SYSTEM_Region osTvType

/* Kart stats (Mario Luigi Yoshi Toad DK Wario Peach Bowser) */
#define u11_Mario D_800E24C8[0]
#define u11_Luigi D_800E24C8[1]
#define u11_Yoshi D_800E24C8[2]
#define u11_Toad D_800E24C8[3]
#define u11_DK D_800E24C8[4]
#define u11_Wario D_800E24C8[5]
#define u11_Peach D_800E24C8[6]
#define u11_Bowser D_800E24C8[7]
#define u12_Mario D_800E24E8[0]
#define u12_Luigi D_800E24E8[1]
#define u12_Yoshi D_800E24E8[2]
#define u12_Toad D_800E24E8[3]
#define u12_DK D_800E24E8[4]
#define u12_Wario D_800E24E8[5]
#define u12_Peach D_800E24E8[6]
#define u12_Bowser D_800E24E8[7]
#define u13_Mario D_800E2508[0]
#define u13_Luigi D_800E2508[1]
#define u13_Yoshi D_800E2508[2]
#define u13_Toad D_800E2508[3]
#define u13_DK D_800E2508[4]
#define u13_Wario D_800E2508[5]
#define u13_Peach D_800E2508[6]
#define u13_Bowser D_800E2508[7]
#define u14_Mario D_800E2528[0]
#define u14_Luigi D_800E2528[1]
#define u14_Yoshi D_800E2528[2]
#define u14_Toad D_800E2528[3]
#define u14_DK D_800E2528[4]
#define u14_Wario D_800E2528[5]
#define u14_Peach D_800E2528[6]
#define u14_Bowser D_800E2528[7]
#define cc50_Mario TopSpeed50CC[0]
#define cc50_Luigi TopSpeed50CC[1]
#define cc50_Yoshi TopSpeed50CC[2]
#define cc50_Toad TopSpeed50CC[3]
#define cc50_DK TopSpeed50CC[4]
#define cc50_Wario TopSpeed50CC[5]
#define cc50_Peach TopSpeed50CC[6]
#define cc50_Bowser TopSpeed50CC[7]
#define cc100_Mario TopSpeed100CC[0]
#define cc100_Luigi TopSpeed100CC[1]
#define cc100_Yoshi TopSpeed100CC[2]
#define cc100_Toad TopSpeed100CC[3]
#define cc100_DK TopSpeed100CC[4]
#define cc100_Wario TopSpeed100CC[5]
#define cc100_Peach TopSpeed100CC[6]
#define cc100_Bowser TopSpeed100CC[7]
#define cc150_Mario TopSpeed150CC[0]
#define cc150_Luigi TopSpeed150CC[1]
#define cc150_Yoshi TopSpeed150CC[2]
#define cc150_Toad TopSpeed150CC[3]
#define cc150_DK TopSpeed150CC[4]
#define cc150_Wario TopSpeed150CC[5]
#define cc150_Peach TopSpeed150CC[6]
#define cc150_Bowser TopSpeed150CC[7]
#define ccextra_Mario TopSpeedExtra[0]
#define ccextra_Luigi TopSpeedExtra[1]
#define ccextra_Yoshi TopSpeedExtra[2]
#define ccextra_Toad TopSpeedExtra[3]
#define ccextra_DK TopSpeedExtra[4]
#define ccextra_Wario TopSpeedExtra[5]
#define ccextra_Peach TopSpeedExtra[6]
#define ccextra_Bowser TopSpeedExtra[7]
#define tripleTap_Mario gKartTripleABoost[0]
#define tripleTap_Luigi gKartTripleABoost[1]
#define tripleTap_Yoshi gKartTripleABoost[2]
#define tripleTap_Toad gKartTripleABoost[3]
#define tripleTap_DK gKartTripleABoost[4]
#define tripleTap_Wario gKartTripleABoost[5]
#define tripleTap_Peach gKartTripleABoost[6]
#define tripleTap_Bowser gKartTripleABoost[7]
#define turncontr_Mario gKartHandlingTable[0]
#define turncontr_Luigi gKartHandlingTable[1]
#define turncontr_Yoshi gKartHandlingTable[2]
#define turncontr_Toad gKartHandlingTable[3]
#define turncontr_DK gKartHandlingTable[4]
#define turncontr_Wario gKartHandlingTable[5]
#define turncontr_Peach gKartHandlingTable[6]
#define turncontr_Bowser gKartHandlingTable[7]
#define turncoA_Mario gKartTurnSpeedReductionTable0[0]
#define turncoA_Luigi gKartTurnSpeedReductionTable0[1]
#define turncoA_Yoshi gKartTurnSpeedReductionTable0[2]
#define turncoA_Toad gKartTurnSpeedReductionTable0[3]
#define turncoA_DK gKartTurnSpeedReductionTable0[4]
#define turncoA_Wario gKartTurnSpeedReductionTable0[5]
#define turncoA_Peach gKartTurnSpeedReductionTable0[6]
#define turncoA_Bowser gKartTurnSpeedReductionTable0[7]
#define turncoB_Mario gKartTurnSpeedReductionTable1[0]
#define turncoB_Luigi gKartTurnSpeedReductionTable1[1]
#define turncoB_Yoshi gKartTurnSpeedReductionTable1[2]
#define turncoB_Toad gKartTurnSpeedReductionTable1[3]
#define turncoB_DK gKartTurnSpeedReductionTable1[4]
#define turncoB_Wario gKartTurnSpeedReductionTable1[5]
#define turncoB_Peach gKartTurnSpeedReductionTable1[6]
#define turncoB_Bowser gKartTurnSpeedReductionTable1[7]

/* cups: const Table is still RAM on N64; strip const for Library writes */
#define g_CupArray ((s16 *) CupCourseOrder)
#define g_cup0Array0 g_CupArray[0]
#define g_cup0Array1 g_CupArray[1]
#define g_cup0Array2 g_CupArray[2]
#define g_cup0Array3 g_CupArray[3]
#define g_cup1Array0 g_CupArray[4]
#define g_cup1Array1 g_CupArray[5]
#define g_cup1Array2 g_CupArray[6]
#define g_cup1Array3 g_CupArray[7]
#define g_cup2Array0 g_CupArray[8]
#define g_cup2Array1 g_CupArray[9]
#define g_cup2Array2 g_CupArray[10]
#define g_cup2Array3 g_CupArray[11]
#define g_cup3Array0 g_CupArray[12]
#define g_cup3Array1 g_CupArray[13]
#define g_cup3Array2 g_CupArray[14]
#define g_cup3Array3 g_CupArray[15]
#define g_cupBArray0 g_CupArray[16]
#define g_cupBArray1 g_CupArray[17]
#define g_cupBArray2 g_CupArray[18]
#define g_cupBArray3 g_CupArray[19]

/* per-Player tables */
#define g_GameLapTable LapCountByPlayerId
#define g_gameLapPlayer1 LapCountByPlayerId[0]
#define g_gameLapPlayer2 LapCountByPlayerId[1]
#define g_gameLapPlayer3 LapCountByPlayerId[2]
#define g_gameLapPlayer4 LapCountByPlayerId[3]
#define g_gameLapPlayer5 LapCountByPlayerId[4]
#define g_gameLapPlayer6 LapCountByPlayerId[5]
#define g_gameLapPlayer7 LapCountByPlayerId[6]
#define g_gameLapPlayer8 LapCountByPlayerId[7]
#define g_playerPathPointTable NearestPathPointByPlayerId
#define g_PathPointPlayer1 NearestPathPointByPlayerId[0]
#define g_PathPointPlayer2 NearestPathPointByPlayerId[1]
#define g_PathPointPlayer3 NearestPathPointByPlayerId[2]
#define g_PathPointPlayer4 NearestPathPointByPlayerId[3]
#define g_PathPointPlayer5 NearestPathPointByPlayerId[4]
#define g_PathPointPlayer6 NearestPathPointByPlayerId[5]
#define g_PathPointPlayer7 NearestPathPointByPlayerId[6]
#define g_PathPointPlayer8 NearestPathPointByPlayerId[7]
#define g_playerPosition1 GPCurrentRaceRankByPlayerId[0]
#define g_playerPosition2 GPCurrentRaceRankByPlayerId[1]
#define g_playerPosition3 GPCurrentRaceRankByPlayerId[2]
#define g_playerPosition4 GPCurrentRaceRankByPlayerId[3]
#define g_playerPosition5 GPCurrentRaceRankByPlayerId[4]
#define g_playerPosition6 GPCurrentRaceRankByPlayerId[5]
#define g_playerPosition7 GPCurrentRaceRankByPlayerId[6]
#define g_playerPosition8 GPCurrentRaceRankByPlayerId[7]
#define g_lightningFlagPlayer2 LightningFlagPlayer1[1]
#define g_lightningFlagPlayer3 LightningFlagPlayer1[2]
#define g_lightningFlagPlayer4 LightningFlagPlayer1[3]
#define g_lightningFlagPlayer5 LightningFlagPlayer1[4]
#define g_lightningFlagPlayer6 LightningFlagPlayer1[5]
#define g_lightningFlagPlayer7 LightningFlagPlayer1[6]
#define g_lightningFlagPlayer8 LightningFlagPlayer1[7]
#define g_colorPlayer1R ColorPlayer0R[1]
#define g_colorPlayer2R ColorPlayer0R[2]
#define g_colorPlayer3R ColorPlayer0R[3]
#define g_colorPlayer4R ColorPlayer0R[4]
#define g_colorPlayer5R ColorPlayer0R[5]
#define g_colorPlayer6R ColorPlayer0R[6]
#define g_colorPlayer7R ColorPlayer0R[7]
#define AnimatedLakituStruct ((struct AnmObjectLakitu *) &DynamicObjects[1])

/* Camera / HUD / menu */
#define g_screenViewAngle CameraZoom
#define g_zoomFOVPlayer1 CameraZoom[0]
#define g_zoomFOVPlayer2 CameraZoom[1]
#define g_zoomFOVPlayer3 CameraZoom[2]
#define g_zoomFOVPlayer4 CameraZoom[3]
#define g_zoomLevelPlayer1 KoteiGakakuMode[0]
#define g_zoomLevelPlayer2 KoteiGakakuMode[1]
#define g_zoomLevelPlayer3 KoteiGakakuMode[2]
#define g_zoomLevelPlayer4 KoteiGakakuMode[3]
#define g_resetToggle GameState
#define g_SequenceMode GameState
#define PlayerOK CharacterGridIsSelected
#define currentMenu MenuItems[0].Kind
#define g_menuPreviewValue1 (((s8 *) &MenuItems[11].Work1)[3])
#define g_menuPreviewValue2 (((s8 *) &MenuItems[12].Work1)[3])
#define g_menuPreviewValue3 (((s8 *) &MenuItems[13].Work1)[3])
#define g_menuPreviewValue4 (((s8 *) &MenuItems[14].Work1)[3])
#define g_BattlePreviewValue1 (((s8 *) &MenuItems[5].Work1)[3])
#define g_BattlePreviewValue2 (((s8 *) &MenuItems[6].Work1)[3])
#define g_BattlePreviewValue3 (((s8 *) &MenuItems[7].Work1)[3])
#define g_BattlePreviewValue4 (((s8 *) &MenuItems[8].Work1)[3])
#define g_player1ScreenWidth (GlobalScreen[0]->Width)
#define RadarOn D_80165800
#define g_timeLapChange TimePlayerLastTouchedFinishLine
#define g_TrialTime TimePlayerLastTouchedFinishLine
/* Audio headers live on the audio heap (MUSTablePointer / AlCtlHeader / AlTbl). */
#define g_MUSSequenceTable (*(SequenceTable *) MUSTablePointer)
#define g_MUSInstrumentTable (*(InstrumentTable *) AlCtlHeader)
#define g_MUSRawAudioTable (*(RawAudioTable *) AlTbl)
#define ActionData_Pointer CoursesCPUBehaviour
#define g_fogR FogRed
#define g_fogG FogGreen
#define g_fogB FogBlue
#define g_fogToggleBanshee SkyToggle
#define g_playerEcho (((s8 *) D_800E9F7C)[0x14])

/* functions (stock Names / leftover D labels) */
#define CallLakitu func_80090868
#define SetFastOoB(car, extra) func_80090868(car)
#define GetLakituSpawnPoint func_80090178
#define KWDisplayEvent_After RenderSnowingEffect
#define KWSet2Color SetColorRender
#define SPRDrawClip SprDrawClip
#define LoadMapData LoadCourse
#define MoveIWA(obj) UpdateActorFallingRocks((struct FallingRock *) (obj))

/* common segment 0x0D */
#define KWTextureIA_AAZBBL_Init D_0D007B00
#define StockNumberSprites common_texture_hud_normal_digit

#endif
