#ifndef STOCK_ALIASES_H
#define STOCK_ALIASES_H

/*
 * Library labels that are not the decomp identifier.
 * Host override names (*Default) are documented in docs/host_overrides.md.
 * gDynamicP is already the decomp name (main.h).
 * DMA / decodeMIO0 / AllocateMemory come from decomp headers.
 *
 * Name-alikes (same spelling as decomp) belong in the decomp header, not here
 * and not as a second GameOffsets prototype. Keep a #define only when it wraps
 * (arity/type) or maps a different OverKart name onto a stock symbol.
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

extern ALSeqFile *g_MUSTablePointer;
extern ALSeqFile *gAlCtlHeader;
extern ALSeqFile *gAlTbl;

extern Hud playerHUD[];
extern s32 gLapCountByPlayerId[];
extern s32 gGPCurrentRaceRankByPlayerId[];
extern void *gCoursesCPUBehaviour[];
extern s16 kotei_gakaku_mode[];
extern struct ObjBlock gMenuItems[];
#define GlobalMenuHUD gMenuItems
extern struct PlayerTextureTable D_800E8320[];
#define SelectPortraitTable D_800E8320
extern Gfx D_0D0079C8[];
extern Gfx D_0D007B00[];
extern Vtx D_0D005AE0[];
extern Vtx D_0D005B20[];
extern u16 common_texture_hud_normal_digit[];
void func_80090868(Player *);
void func_80090178(Player *, s8, Vec3f, Vec3f);
u8 *load_course(s32);
void update_actor_falling_rocks(struct FallingRock *);
void SprDrawClip(s32, s32, s32, s32, s32);

#define InitMKCode init_mkcode
#define ramCopy(dst, src, n) memcpy((void *) (uintptr_t) (dst), (const void *) (uintptr_t) (src), (size_t) (n))
#define MemCpyN64 ramCopy

#define GlobalPlayer gPlayers
#define g_PlayerStructTable gPlayers[0]
#define g_PlayerStruct1 gPlayers[0]
#define g_PlayerStruct2 gPlayers[1]
#define g_PlayerStruct3 gPlayers[2]
#define g_PlayerStruct4 gPlayers[3]
#define g_PlayerStruct5 gPlayers[4]
#define g_PlayerStruct6 gPlayers[5]
#define g_PlayerStruct7 gPlayers[6]
#define g_PlayerStruct8 gPlayers[7]

#define g_PlayerInputTable gControllers
#define g_Controller1 gControllers[0]
#define g_Controller2 gControllers[1]
#define g_Controller3 gControllers[2]
#define g_Controller4 gControllers[3]
#define g_ControllerMenu gControllers[4]
#define g_ControllerGhost1 gControllers[5]
#define g_ControllerGhost2 gControllers[6]
#define g_ControllerGhost3 gControllers[7]

#define g_CameraTable cameras[0]
#define g_Camera1 cameras[0]
#define g_Camera2 cameras[1]
#define g_Camera3 cameras[2]
#define g_Camera4 cameras[3]

#define g_ScreenStruct D_8015F480[0]
#define g_Screen1 D_8015F480[0]
#define g_Screen2 D_8015F480[1]
#define g_Screen3 D_8015F480[2]
#define g_Screen4 D_8015F480[3]

#define g_hudStruct playerHUD
#define g_hudPlayer1 playerHUD[0]
#define g_hudPlayer2 playerHUD[1]
#define g_hudPlayer3 playerHUD[2]
#define g_hudPlayer4 playerHUD[3]
#define KWLap playerHUD

#define GraphPtrOffset gDisplayListHead
#define GraphPtr gDisplayListHead
Gfx* FillRect1ColorF(Gfx*, s32, s32, s32, s32, u32, u32, u32, u32);

#define MapStartup MapStartupDefault
#define KWKumo_Alloc_Hook KWKumo_Alloc_Hook_Default
#define KWChart_Kumo_Hook KWChart_Kumo_Hook_Default

/* libc */
#define Sqrtf sqrtf
#define sinF sinf
#define cosF cosf

/* NTSC TV type lives at 0x80000300 */
#define SYSTEM_Region osTvType

/* kart stats (Mario Luigi Yoshi Toad DK Wario Peach Bowser) */
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
#define cc50_Mario gTopSpeed50cc[0]
#define cc50_Luigi gTopSpeed50cc[1]
#define cc50_Yoshi gTopSpeed50cc[2]
#define cc50_Toad gTopSpeed50cc[3]
#define cc50_DK gTopSpeed50cc[4]
#define cc50_Wario gTopSpeed50cc[5]
#define cc50_Peach gTopSpeed50cc[6]
#define cc50_Bowser gTopSpeed50cc[7]
#define cc100_Mario gTopSpeed100cc[0]
#define cc100_Luigi gTopSpeed100cc[1]
#define cc100_Yoshi gTopSpeed100cc[2]
#define cc100_Toad gTopSpeed100cc[3]
#define cc100_DK gTopSpeed100cc[4]
#define cc100_Wario gTopSpeed100cc[5]
#define cc100_Peach gTopSpeed100cc[6]
#define cc100_Bowser gTopSpeed100cc[7]
#define cc150_Mario gTopSpeed150cc[0]
#define cc150_Luigi gTopSpeed150cc[1]
#define cc150_Yoshi gTopSpeed150cc[2]
#define cc150_Toad gTopSpeed150cc[3]
#define cc150_DK gTopSpeed150cc[4]
#define cc150_Wario gTopSpeed150cc[5]
#define cc150_Peach gTopSpeed150cc[6]
#define cc150_Bowser gTopSpeed150cc[7]
#define ccextra_Mario gTopSpeedExtra[0]
#define ccextra_Luigi gTopSpeedExtra[1]
#define ccextra_Yoshi gTopSpeedExtra[2]
#define ccextra_Toad gTopSpeedExtra[3]
#define ccextra_DK gTopSpeedExtra[4]
#define ccextra_Wario gTopSpeedExtra[5]
#define ccextra_Peach gTopSpeedExtra[6]
#define ccextra_Bowser gTopSpeedExtra[7]
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

/* cups: const table is still RAM on N64; strip const for Library writes */
#define g_CupArray ((s16 *) gCupCourseOrder)
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

/* per-player tables */
#define g_GameLapTable gLapCountByPlayerId
#define g_gameLapPlayer1 gLapCountByPlayerId[0]
#define g_gameLapPlayer2 gLapCountByPlayerId[1]
#define g_gameLapPlayer3 gLapCountByPlayerId[2]
#define g_gameLapPlayer4 gLapCountByPlayerId[3]
#define g_gameLapPlayer5 gLapCountByPlayerId[4]
#define g_gameLapPlayer6 gLapCountByPlayerId[5]
#define g_gameLapPlayer7 gLapCountByPlayerId[6]
#define g_gameLapPlayer8 gLapCountByPlayerId[7]
#define g_playerPathPointTable gNearestPathPointByPlayerId
#define g_PathPointPlayer1 gNearestPathPointByPlayerId[0]
#define g_PathPointPlayer2 gNearestPathPointByPlayerId[1]
#define g_PathPointPlayer3 gNearestPathPointByPlayerId[2]
#define g_PathPointPlayer4 gNearestPathPointByPlayerId[3]
#define g_PathPointPlayer5 gNearestPathPointByPlayerId[4]
#define g_PathPointPlayer6 gNearestPathPointByPlayerId[5]
#define g_PathPointPlayer7 gNearestPathPointByPlayerId[6]
#define g_PathPointPlayer8 gNearestPathPointByPlayerId[7]
#define g_playerPosition1 gGPCurrentRaceRankByPlayerId[0]
#define g_playerPosition2 gGPCurrentRaceRankByPlayerId[1]
#define g_playerPosition3 gGPCurrentRaceRankByPlayerId[2]
#define g_playerPosition4 gGPCurrentRaceRankByPlayerId[3]
#define g_playerPosition5 gGPCurrentRaceRankByPlayerId[4]
#define g_playerPosition6 gGPCurrentRaceRankByPlayerId[5]
#define g_playerPosition7 gGPCurrentRaceRankByPlayerId[6]
#define g_playerPosition8 gGPCurrentRaceRankByPlayerId[7]
#define g_lightningFlagPlayer2 g_lightningFlagPlayer1[1]
#define g_lightningFlagPlayer3 g_lightningFlagPlayer1[2]
#define g_lightningFlagPlayer4 g_lightningFlagPlayer1[3]
#define g_lightningFlagPlayer5 g_lightningFlagPlayer1[4]
#define g_lightningFlagPlayer6 g_lightningFlagPlayer1[5]
#define g_lightningFlagPlayer7 g_lightningFlagPlayer1[6]
#define g_lightningFlagPlayer8 g_lightningFlagPlayer1[7]
#define g_colorPlayer1R g_colorPlayer0R[1]
#define g_colorPlayer2R g_colorPlayer0R[2]
#define g_colorPlayer3R g_colorPlayer0R[3]
#define g_colorPlayer4R g_colorPlayer0R[4]
#define g_colorPlayer5R g_colorPlayer0R[5]
#define g_colorPlayer6R g_colorPlayer0R[6]
#define g_colorPlayer7R g_colorPlayer0R[7]
#define AnimatedLakituStruct ((struct AnmObject_Lakitu *) &g_DynamicObjects[1])

/* camera / HUD / menu */
#define ScreenViewAngle gCameraZoom
#define g_screenViewAngle gCameraZoom
#define g_zoomFOVPlayer1 gCameraZoom[0]
#define g_zoomFOVPlayer2 gCameraZoom[1]
#define g_zoomFOVPlayer3 gCameraZoom[2]
#define g_zoomFOVPlayer4 gCameraZoom[3]
#define g_zoomLevelPlayer1 kotei_gakaku_mode[0]
#define g_zoomLevelPlayer2 kotei_gakaku_mode[1]
#define g_zoomLevelPlayer3 kotei_gakaku_mode[2]
#define g_zoomLevelPlayer4 kotei_gakaku_mode[3]
#define g_resetToggle gGamestate
#define PlayerOK gCharacterGridIsSelected
#define currentMenu gMenuItems[0].kind
#define g_menuPreviewValue1 (((s8 *) &gMenuItems[11].work1)[3])
#define g_menuPreviewValue2 (((s8 *) &gMenuItems[12].work1)[3])
#define g_menuPreviewValue3 (((s8 *) &gMenuItems[13].work1)[3])
#define g_menuPreviewValue4 (((s8 *) &gMenuItems[14].work1)[3])
#define g_BattlePreviewValue1 (((s8 *) &gMenuItems[5].work1)[3])
#define g_BattlePreviewValue2 (((s8 *) &gMenuItems[6].work1)[3])
#define g_BattlePreviewValue3 (((s8 *) &gMenuItems[7].work1)[3])
#define g_BattlePreviewValue4 (((s8 *) &gMenuItems[8].work1)[3])
#define g_player1ScreenWidth (GlobalScreen[0]->width)
#define RadarOn D_80165800
#define g_timeLapChange gTimePlayerLastTouchedFinishLine
#define g_TrialTime gTimePlayerLastTouchedFinishLine
/* Audio headers live on the audio heap (g_MUSTablePointer / gAlCtlHeader / gAlTbl). */
#define g_MUSSequenceTable (*(SequenceTable *) g_MUSTablePointer)
#define g_MUSInstrumentTable (*(InstrumentTable *) gAlCtlHeader)
#define g_MUSRawAudioTable (*(RawAudioTable *) gAlTbl)
#define ActionData_Pointer gCoursesCPUBehaviour
#define g_fogR fog_red
#define g_fogG fog_green
#define g_fogB fog_blue
#define g_fogToggleBanshee g_skyToggle
#define g_playerEcho (((s8 *) D_800E9F7C)[0x14])

/* functions (stock names / leftover D labels) */
#define CallLakitu func_80090868
#define SetFastOoB(car, extra) func_80090868(car)
#define GetLakituSpawnPoint func_80090178
#define KWDisplayEvent_After render_snowing_effect
#define KWSet2Color set_color_render
#define SPRDrawClip SprDrawClip
#define LoadMapData load_course
#define MoveIWA(obj) update_actor_falling_rocks((struct FallingRock *) (obj))

/* common segment 0x0D */
#define KWTextureRGBA_AAZBBL_Init D_0D0079C8
#define KWTextureIA_AAZBBL_Init D_0D007B00
#define Vtx_KTile32x32 ((Vtx_t *) D_0D005AE0)
#define Vtx_KTile32x32XFlip ((Vtx_t *) D_0D005B20)
#define StockNumberSprites common_texture_hud_normal_digit

#endif
