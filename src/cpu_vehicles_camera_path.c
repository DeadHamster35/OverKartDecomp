#include <ultra64.h>
#include <macros.h>
#include <defines.h>

#include "cpu_vehicles_camera_path.h"
#include "math_util.h"
#include "code_800029B0.h"
#include "racing/memory.h"
#include "path.h"
#include "camera.h"
#include "actors.h"
#include "actors_extended.h"
#include "render_player.h"
#include "player_controller.h"
#include "update_objects.h"
#include "collision.h"
#include <actor_types.h>
#include "vehicles.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "bomb_kart.h"
#include "courses/all_course_data.h"
#include <assets/common_data.h>
#include <common_structs.h>
#include "main.h"
#include "menus.h"
#include "menu_items.h"
#include "audio/external.h"
#include "ending/podium_ceremony_actors.h"
#include "spawn_players.h"
#include "sounds.h"
#include "data/path_spawn_metadata.h"
#include "math_util_2.h"
#include "OverKartHooks.h"

s32 UnkCPUVehiclesCameraPathPad[24];
Collision D_80162E70;
s16 D_80162EB0; // Possibly a float.
s16 D_80162EB2; // possibly [3]

ActionData* CoursesCPUBehaviour[NUM_COURSES - 1];

// more Padding?
s32 D_80162F08[2];

s16 PylonNum[30];
s16 PylonNum2[30];

// Padding, seemingly?
s32 PylonPos[4];

Vec3f object;
Vec3f Object1;
Vec3f Object2;
s16 TrainSmokeTimer;
s16 OGAArea;
s16 BiriFlg;
f32 RapDat[NUM_PLAYERS];
/*
The Size of many of these arrays is weird. Based on their usage, they would only need
to be 8 Entries long (enough for each Player). But some are 10 or even 12 long.
Its plausible that this is just some decompilation artifact?
Or maybe at some Point in development they had plans for more players?
*/
s16 AvoidMode[12];
s16 AvoidCount[12];
f32 Speedlevel[10];
s16 OldEosAngleOga[12];
f32 ZureData[10];
f32 ZureDist[10];
bool StraightFlg[10];
u16 OgaAreaOrg;
s16 OgaAreaChgFlg;
s16 DriftFlg[10];
s16 FerrySmokeTimer;
s32 EnemyTimeCount[10];
s32 EnemyDist[10];
s32 EnemyLimdist[10];
f32 EnemyObjectX[10];
f32 EnemyObjectZ[10];
s16 SVehicleSoundRenderCounter;
s32 BestTime;
CenterPathStruct* SidePointL;
CenterPathStruct* SidePointR;
s16* CurveData;
s16* AngleData;
u16 TenukiFlg[12];
u16 OldTenukiFlg[10];
f32 Limmitspeed;
f32 OldLimmitspeed[10];
s32 OgaCameraWatchNum;
u16 RapCountFlg[12];
u16 OgaReverseCount[12];
u16 WrongwayFlagPlayer1[12];
s32 OldPylonCount[10];
ActionData* ActPointPtr;
u16 NextActionNum[12];
u16 OldActionNum[12];
u16 ActionFlg[12];
s16 OgaAngle[12];
u16 LakituSurface[12];
u16 OgaCarStat[10];
u16 EnemyRivalNum[2];
u16 EnemyRivalName[2];
u16 EnemyRivalAimRank[2];
u16 EnemyAccelMode[12];
s32 MaxPathPoints[4];
s32 OgaFrameCounter;
s32 OgaEtcFrameCounter;
s16 MokuhyouAddArea[12];
s16 EnemyVoiceCountSPIN[12];
s16 EnemyVoiceCountDAMAGE[12];
s16 EnemyVoiceCountOVERTAKE[12];
s16 OgaHamidasiMapFlg[12];
s16 NoSimpleKartFlag[12];
s16 OgaNearHyousyouFlg[4];
f32 OgaHyousyouMx[4];
f32 OgaHyousyouMy[4];
f32 OgaHyousyouMz[4];
s32 BipasNum;
f32 FinishLineZ;
f32 OgaOldRapChkZ[10];
s16 EnemyTargetPlayer;
s16 HighwayGyakusouFlg;
s16 OgaHyousyouDemoTrg;
s16 OgaNopointDemoTrg;
s32 OgaHyousyouDemoCounter;
s32 OgaNopointDemoCounter;
s32 OgaBumpChkCounter;
s16 RankUpdateFinishFlag;
s16 EnemyEnterTrg[12];
s16 EnemyExitTrg[12];
s16 RivalOvertakeAllowFlag[12];
s16 FumikiriStopFlg[10];
s16 OgawaKarakaraShortCutOKFlg;
s32 OgaCalcLoopCount;
s32 OldOgaCalcLoopCount;
SOUKOU_ZURE_TYPE LaneData[10];
Path2D* CenterLineEX;
s32 CourseObstacle;
TrainStuff SL[NUM_TRAINS];
u16 FumikiriMode[NUM_CROSSINGS];
u16 FumikiriCounter[NUM_CROSSINGS];
PaddleBoatStuff Ship[NUM_PADDLE_BOATS];
VehicleStuff TRUCK[NUM_RACE_BOX_TRUCKS];
VehicleStuff Bus[NUM_RACE_SCHOOL_BUSES];
VehicleStuff Tanklorry[NUM_RACE_TANKER_TRUCKS];
VehicleStuff RvCar[NUM_RACE_CARS];
s32 D_80163DD8[4];
BombKart Bom[NUM_BOMB_KARTS_MAX];
Collision BomBump[NUM_BOMB_KARTS_MAX];
struct unexpiredActors KameHibasira[8];
EnemyItem EnemyItemTable[NUM_PLAYERS];
s16 EnemyFreeItemDebugFlg;
s16 EnemyFreeItemDebugInum;
s16 EnemyFreeItemDebugNum;
s16 RankToNum[12]; // D_80164360
s16 RealRankToNum[12];
s32 LapCountByPlayerId[10];          // D_80164390
s32 GPCurrentRaceRankByPlayerId[10]; // D_801643B8
s32 OldOgawaRank[10];
s32 RealRank[10];
u16 CenterLineMax;
u16 NearestPathPointByPlayerId[12];
s32 PlayerPathPointTotalTable[10];
s16 NameToNum[10];
s32 D_8016448C;
CenterPathStruct* PathPointPointer;
f32 CamOffset[4];
f32 RateCountF[10];    // D_801644A8
f32 RapCountF[10]; // D_801644D0
s16 EnemyEnterMaze[12];
f32 EnemyTenukiY[10];
s16 EnemyGetaRank[12];
CenterPathStruct* CenterPathBP[4];
CenterPathStruct* SideLPathBP[4];
CenterPathStruct* SideRPathBP[4];
s16* CurvePathDataBP[4];
s16* AnglePathDataBP[4];
s16* ShortCutPathDataBP[4];
u16 CurrentPathID[12]; // D_801645B0
u16 CourseTotalPathPoints[4]; // D_801645C8
s32 D_801645D0[4];
s16* ShortCutData;
s32 D_801645E8[4];
f32 OldOgaCamX[4];
s32 D_80164608[4];
f32 OldOgaCamY[4];
s32 D_80164628[4];
f32 OldOgaCamZ[4];
f32 CamSpeed[4];
f32 CamSpeedObjct[4];
s16 OgaCamArea[4];
s16 GakakuMode[4];
s16 KoteiGakakuMode[4];
s16 OgawaCameraMode[4];
f32 OgaCameraZure[4];
f32 OgaCameraBirdX;
f32 OgaCameraBirdY;
f32 OgaCameraBirdZ;
// Start Padding
s32 OgaCameraBirdTx;
s32 OgaCameraBirdTy;
s32 OgaCameraBirdTz;
s32 OgaCameraBirdSpeed;
s32 OgaCameraBirdThetaY;
s32 OgaCameraBirdThetaMy;
s32 D_801646BC;
// end Padding
s16 KatamukiTheta[4];
u32 OgaGoalCounter;
u16 D_801646CC;
UnkStruct46D0 OgaSoundCamera[4];

// Strings, presented by google translate!
// Note that these are EUC-JP encoded, see:
// https://en.wikipedia.org/wiki/Extended_Unix_Code#EUC-JP

// Force sort immediately after Goal
char* D_800EB710 = "ゴール直後の強制ソート\n";
// Forced sort immediately after one Goal in 2PGP
char* D_800EB728 = "2PGPで片方がゴール直後の強制ソート\n";
// Rank calculation error
char* D_800EB74C = "順位計算エラー！！ (num %d) (rank %d) (e_rank %d)\n";
// Bypass switching error!!!(num %d org_bipas %d bipas %d)
char* D_800EB780 = "バイパス切り替え エラー!!!(num %d  org_bipas %d  bipas %d)\n";
char* D_800EB7BC = "(%d) rap %3d  rate_count_F %10.2f  rap_count_F %10.2f  area %5d \n";
// Enter the maze! enemy %d (%d --> %d)
char* D_800EB800 = "迷路に突入！ enemy %d (%d --> %d)\n";
// Out of the maze! enemy %d (%d --> %d)
char* D_800EB824 = "迷路から出た！ enemy %d (%d --> %d)\n";
char* D_800EB84C = "enemy voice set (%d  slip_flag %x  weapon %x)\n";
// Spin Voice! ! (%d , name %d)
char* D_800EB87C = "スピンヴォイス！！(%d , name %d)\n";
// Damage voice! ! (%d, name %d)
char* D_800EB8A0 = "ダメージヴォイス！！(%d, name %d)\n";
char* D_800EB8C4 = "===== ENEMY DRIVE SUB (%d) =====\n";
// omission
char* D_800EB8E8 = "ENEMY END(手抜き)\n\n";
char* D_800EB8FC = "ENEMY END(手抜き)\n\n";
char* D_800EB910 = "(1)enemy stick angle over!! (%d)\n";
char* D_800EB934 = "ENEMY END\n\n";
char* D_800EB940 = "(2)enemy stick angle over!! (%d)\n";
char* D_800EB964 = "ENEMY END\n\n";
// AREA ERR!!! (group not registered at Current centerline %d) %d
char* D_800EB970 = "AREA ERR!!! (現在のセンターライン %d に未登録のグループです) %d\n";
// AREA ERR!!! (Unregistered group) %d
char* D_800EB9B4 = "AREA ERR!!! (未登録のグループです) %d\n";
// GetOgaAreaSubBP() ... Area not found! (b_num = %d)
char* D_800EB9DC = "get_oga_area_sub_BP() ... エリアが見つからないッス！ (b_num = %d)\n";
// Status: (%d, %d, %d)
char* D_800EBA20 = "  状況: (%d, %d, %d) \n";
char* D_800EBA38 = "<%d> (%d, %d, %d) [%d] lng %f\n";
// Wario Stadium Jump failed! ! ! (Area %d, y %7.2f)
char* D_800EBA58 = "ワリオスタジアム  ジャンプ失敗！！！ (area %d, y %7.2f)\n";
// I fell in the water! ! Forced to centerline (num %d: Area %d ) (%d,%d,%d)
char* D_800EBA94 = "水に落ちた！！  センターラインに強制移動しました (num %d: area %d ) (%d,%d,%d)\n";
// Course match! ! (Slacking: with Bump) Forced move to center line (num %d: Area %d ==>%d) (group %d) (%d,%d,%d)
char* D_800EBAE4 = "こーすあうと！！（手抜き中:バンプ有り）  センターラインに強制移動しました (num %d: area %d ==>%d) "
                   "(group %d) (%d,%d,%d)\n";
// Course match! ! (Sitting corners: no Bump) Forced move to center line (num %d: Area %d ==>%d) (group %d) (%d,%d,%d)
char* D_800EBB60 = "こーすあうと！！（手抜き中:バンプ無し）  センターラインに強制移動しました (num %d: area %d ==>%d) "
                   "(group %d) (%d,%d,%d)\n";
// Course match! ! ! Recalculated Area (num %d: Area %d ==>%d)
char* D_800EBBDC = "こーすあうと！！！    エリアを再計算しました (num %d: area %d ==>%d)\n";
// Direct BOM(%d) (%7.2f, %7.2f, %7.2f)
char* D_800EBC24 = "直接指定のBOM(%d) (%7.2f, %7.2f, %7.2f) \n";
char* D_800EBC50 = "BOM HIT CHECK\n";
char* D_800EBC60 = "BOM HIT !!!!! (%d)\n";
// BOM standby
char* D_800EBC74 = "BOM待機\n";
char* D_800EBC80 = "RESULT BOM area(%d)\n";
// BOM dropped.
char* D_800EBC98 = "BOM が 落ちました。\n";
// Tortoise fire pillar SET failed (TABLE IS FULL)
char* D_800EBCB0 = "カメ用火柱 SET 失敗 (TABLE IS FULL)\n";
// Red turtle fire pillar set error! (Category %d)
char* D_800EBCD8 = "赤ガメ火柱セットエラー！ (category %d)\n";
// Blue turtle fire pillar set error! (Category %d)
char* D_800EBD00 = "青ガメ火柱セットエラー！ (category %d)\n";
// Thorn Turtle Fire Pillar Set Error! (Category %d)
char* D_800EBD28 = "トゲガメ火柱セットエラー！ (category %d)\n";
// Turtle Fire Pillar Initialization! !
char* D_800EBD54 = "カメ火柱初期化！！\n";
// Center line initialization
char* D_800EBD68 = "センターライン初期化\n";
char* D_800EBD80 = "MAP NUMBER %d\n";
char* D_800EBD90 = "center_EX ptr      = %x %x (%x)\n";
char* D_800EBDB4 = "\n";
char* D_800EBDB8 = "center_BP[%d] ptr         = %x %x (%x)\n";
char* D_800EBDE0 = "side_point_L_BP[%d] ptr   = %x %x (%x)\n";
char* D_800EBE08 = "side_point_R_BP[%d] ptr   = %x %x (%x)\n";
char* D_800EBE30 = "curve_BP[%d] ptr          = %x %x (%x)\n";
char* D_800EBE58 = "angle_BP[%d] ptr          = %x %x (%x)\n";
char* D_800EBE80 = "short_cut_data_BP[%d] ptr = %x %x (%x)\n";
char* D_800EBEA8 = "\n";
// Ogawa total memory Used = %d
char* D_800EBEAC = "小川の使用メモリー合計 = %d\n";
// Enemy initialization
char* D_800EBECC = "敵初期化\n";
// End of enemy initialization
char* D_800EBED8 = "敵初期化終了\n";
// Bypass CENTER LINE Split Start
char* D_800EBEE8 = "バイパス CENTER LINE 分割開始\n";
// Read centerline from ROM (Map:%d)
char* D_800EBF08 = "センターラインをROMから読みます (map:%d)\n";
char* D_800EBF34 = "ROM center (BP%d) line adr. = %x (%x)\n";
// Calculate centerline (Map:%d)
char* D_800EBF5C = "センターラインを計算します (map:%d)\n";
char* D_800EBF84 = "center (BP%d) line adr. = %x (%x)\n";
char* D_800EBFA8 = "BP center_point_number : %d\n";
// Centerline data error! !
char* D_800EBFC8 = "センターライン データ エラー！！\n";
// Bypass CENTER LINE split end (%d -> %d Number)
char* D_800EBFEC = "バイパス CENTER LINE 分割終了 (%d -> %d 個)\n";
// No center line. (Map: %d)
char* D_800EC01C = "センターラインが ありません。(map:%d)\n";
// side Point calculation (bypass %d)
char* D_800EC044 = "サイドポイント計算 (バイパス %d)\n";
// Curve data calculation (bypass %d)
char* D_800EC068 = "カーブデータ計算 (バイパス %d)\n";
// No center line. (Map: %d)
char* D_800EC088 = "センターラインが ありません。(map:%d)\n";
// Angle data calculation (bypass %d)
char* D_800EC0B0 = "アングルデータ計算 (バイパス %d) \n";
// No center line. (Map: %d)
char* D_800EC0D4 = "センターラインが ありません。(map:%d)\n";
// Shortcut data calculation (bypass %d)
char* D_800EC0FC = "ショートカットデータ計算 (バイパス %d)\n";
char* D_800EC124 = "extern POINT rom_center_KT%d_BP%d[] = {\n";
char* D_800EC150 = "\t{%d,%d,%d,%d},\n";
char* D_800EC164 = "\t0x8000,0x8000,0x8000,0\n};\n\n";
char* D_800EC184 = "area read from ROM (%d)\n";
// Normal Jump! ! ! (%d)
char* D_800EC1A0 = "ノーマルジャンプ！！！(%d)\n";
// Turbo on! ! ! (%d)
char* D_800EC1BC = "ターボオン！！！(%d)\n";
// No cutting corners! ! ! (%d)
char* D_800EC1D4 = "手抜き禁止！！！(%d)\n";
// Action Start data error! (num %d, act %d)
char* D_800EC1EC = "アクション開始データエラー！(num %d, act %d)\n";
// Action end data error! (num %d, act %d, old_act_num %d)
char* D_800EC21C = "アクション終了データエラー！(num %d,  act %d,  old_act_num %d)\n";
char* D_800EC25C = "SL : center_point_number : %d\n";
// SL: CENTER LINE split Start
char* D_800EC27C = "SL: CENTER LINE 分割開始\n";
// SL: CENTER LINE split ended (%d -> %d indivual)
char* D_800EC298 = "SL: CENTER LINE 分割終了 (%d -> %d 個)\n";
char* D_800EC2C0 = "SHIP : center_point_number : %d\n";
// SHIP: CENTER LINE split Start
char* D_800EC2E4 = "SHIP: CENTER LINE 分割開始\n";
// SHIP: CENTER LINE split ended (%d -> %d indivual)
char* D_800EC300 = "SHIP: CENTER LINE 分割終了 (%d -> %d 個)\n";
// General-purpose OBJ character initialization
char* D_800EC32C = "汎用OBJキャラ初期化\n";
// SL OBJ settings
char* D_800EC344 = "SL OBJ設定\n";
// SHIP OBJ settings
char* D_800EC350 = "SHIP OBJ設定\n";
// Track OBJ settings
char* D_800EC360 = "トラックOBJ設定\n";
// Bus OBJ setting
char* D_800EC374 = "バスOBJ設定\n";
// Tank OBJ setting
char* D_800EC384 = "タンクOBJ設定\n";
// RV OBJ settings
char* D_800EC394 = "RV OBJ設定\n";
// Generic OBJ character initialization completed
char* D_800EC3A0 = "汎用OBJキャラ初期化終了\n";
// horn (num %d, permit %d, %d)
char* D_800EC3BC = "クラクション (num %d, permit %d, %d)\n";
char* D_800EC3E4 = "OGA CAMERA INIT (%d)\n";
char* D_800EC3FC = "OGA CAMERA INIT END\n";
// High Speed Camera ERR !!! (ncx = %f)
char* D_800EC414 = "高速カメラ ERR !!! (ncx = %f)\n";
// High Speed Camera ERR !!! (ncz = %f)
char* D_800EC434 = "高速カメラ ERR !!! (ncz = %f)\n";
// High Speed Camera ERR !!! (ecx = %f)
char* D_800EC454 = "高速カメラ ERR !!! (ecx = %f)\n";
// High Speed Camera ERR !!! (ecz = %f)
char* D_800EC474 = "高速カメラ ERR !!! (ecz = %f)\n";
char* D_800EC494 = "OGA DRIVERS POINT CAMERA MODE \n";
char* D_800EC4B4 = "OGA WINNER CAMERA MODE \n";
char* D_800EC4D0 = "OGA TIMEATTACK QUICK CAMERA INIT \n";
char* D_800EC4F4 = "OGA BATTLE CAMERA INIT win(%d)\n";
char* D_800EC514 = "GOAL! <<rank 1>> camera %d  rank %d\n";
char* D_800EC53C = "GOAL! <<rank 2,3,4>> camera %d  rank %d\n";
char* D_800EC568 = "GOAL! <<rank 5,6,7,8>> camera %d  rank %d\n";
// Camera and cart collided! ! !
char* D_800EC594 = "カメラとカートが衝突しました！！！  (%d)\n";
char* D_800EC5C0 = "<<< ITEM OBJ NUMBER ERR !! >>> item %d  obj_num %d \n";
// <<< BANANA SET HOUSE >>> ObjNum %d Zure %f
char* D_800EC5F8 = "<<< BANANA SET 失敗 >>> obj_num %d   zure %f \n";
// BANANA Caught in Owner check. (num %d)
char* D_800EC628 = "BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC65C = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800EC670 = "理由: category \n";
// Reason: Sparam
char* D_800EC684 = "理由: sparam \n";
// Reason: num
char* D_800EC694 = "理由: num \n";
char* D_800EC6A0 = "BANANA HOLD (num %d  time %d   hold_time %d)\n";
// Installation Caught in BANANA Owner check. (num %d)
char* D_800EC6D0 = "設置 BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC708 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800EC71C = "理由: category \n";
// Reason: Sparam
char* D_800EC730 = "理由: sparam \n";
// Reason: num
char* D_800EC740 = "理由: num \n";
// I put BANANA. (num %d)
char* D_800EC74C = "BANANA 置きました。 (num %d)\n";
// <<< BANANA NAGE SET failed >>> ObjNum %d
char* D_800EC76C = "<<< BANANA NAGE SET 失敗 >>> obj_num %d \n";
// BANANA NAGE MOVE Caught in Owner check. (num %d)
char* D_800EC798 = "BANANA NAGE MOVE 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC7D8 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800EC7EC = "理由: category \n";
// Reason: Sparam
char* D_800EC800 = "理由: sparam \n";
// Reason: num
char* D_800EC810 = "理由: num \n";
char* D_800EC81C = "BANANA NAGE END 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC858 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800EC86C = "理由: category \n";
// Reason: Sparam
char* D_800EC880 = "理由: sparam \n";
// Reason: num
char* D_800EC890 = "理由: num \n";
char* D_800EC89C = "G_SHELL HOLD (num %d  time %d   hold_time %d)\n";
// <<< G_SHELL SET failed >>> ObjNum %d
char* D_800EC8CC = "<<< G_SHELL SET 失敗 >>> obj_num %d \n";
// <<< G_SHELL SET failed >>> object_count %d
char* D_800EC8F4 = "<<< G_SHELL SET 失敗 >>> object_count %d \n";
// G_SHELL Caught in Owner check. (num %d)
char* D_800EC920 = "G_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC954 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800EC968 = "理由: category \n";
// Reason: Sparam
char* D_800EC97C = "理由: sparam \n";
// Reason: num
char* D_800EC98C = "理由: num \n";
// Just before launch G_SHELL Caught in Owner check. (num %d)
char* D_800EC998 = "発射直前 G_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC9D8 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800EC9EC = "理由: category \n";
// Reason: Sparam
char* D_800ECA00 = "理由: sparam \n";
// Reason: num
char* D_800ECA10 = "理由: num \n";
// G_SHELL firing (num %d)
char* D_800ECA1C = "G_SHELL 発射 (num %d)\n";
char* D_800ECA34 = "R_SHELL HOLD (num %d  time %d   hold_time %d  obj_num %d)\n";
// <<< R_SHELL SET failed >>> ObjNum %d
char* D_800ECA70 = "<<< R_SHELL SET 失敗 >>> obj_num %d \n";
// <<< R_SHELL SET failed >>> object_count %d
char* D_800ECA98 = "<<< R_SHELL SET 失敗 >>> object_count %d \n";
// R_SHELL Caught in Owner check. (num %d)
char* D_800ECAC4 = "R_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECAF8 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800ECB0C = "理由: category \n";
// Reason: Sparam
char* D_800ECB20 = "理由: sparam \n";
// Reason: num
char* D_800ECB30 = "理由: num \n";
char* D_800ECB3C = "R_SHELL SHOOT (num %d  time %d   hold_time %d  obj_num %d)\n";
// Just before launch R_SHELL Caught in Owner check. (num %d)
char* D_800ECB78 = "発射直前 R_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECBB8 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800ECBCC = "理由: category \n";
// Reason: Sparam
char* D_800ECBE0 = "理由: sparam \n";
// Reason: num
char* D_800ECBF0 = "理由: num \n";
// R_SHELL firing (num %d)
char* D_800ECBFC = "R_SHELL 発射 (num %d)\n";
char* D_800ECC14 = "S_BANANA HOLD (num %d  time %d   hold_time %d)\n";
// <<< SUPER_BANANA SET failed >>> ObjNum %d
char* D_800ECC44 = "<<< SUPER_BANANA SET 失敗 >>> obj_num %d \n";
// <<< SUPER_BANANA SET failed >>> object_count %d
char* D_800ECC70 = "<<< SUPER_BANANA SET 失敗 >>> object_count %d \n";
// S_BANANA Caught in Owner check. (num %d)
char* D_800ECCA0 = "S_BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: Category
char* D_800ECCD8 = "理由: category \n";
// Reason: Sparam
char* D_800ECCEC = "理由: sparam \n";
// Reason: sb_ok
char* D_800ECCFC = "理由: sb_ok \n";
char* D_800ECD0C = "S_BANANA RELEASE (num %d  time %d )\n";
// <<< FAKE IBOX SET failed >>> ObjNum %d
char* D_800ECD34 = "<<< FAKE IBOX SET 失敗 >>> obj_num %d \n";
// IBOX Caught in Owner check. (num %d)
char* D_800ECD5C = "IBOX 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECD90 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800ECDA4 = "理由: category \n";
// Reason: Sparam
char* D_800ECDB8 = "理由: sparam \n";
// Reason: num
char* D_800ECDC8 = "理由: num \n";
char* D_800ECDD4 = "FBOX HOLD (num %d  time %d   hold_time %d)\n";
// Installation IBOX Owner check failed. (num %d)
char* D_800ECE00 = "設置 IBOX 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECE38 = "理由: EXISTOBJ \n";
// Reason: Category
char* D_800ECE4C = "理由: category \n";
// Reason: Sparam
char* D_800ECE60 = "理由: sparam \n";
// Reason: num
char* D_800ECE70 = "理由: num \n";
// Ray START (%d)
char* D_800ECE7C = "雷START (%d)\n";
// Ray END (%d)
char* D_800ECE8C = "雷END (%d)\n";
// ---------- Initialization of commendation Table
char* D_800ECE98 = "---------- 表彰台初期化\n";
// map_number = %d -> 20 Rewriting.
char* D_800ECEB4 = "map_number = %d - > 20 書き換え中。\n";
// OGA Recognition move begins
char* D_800ECEDC = "OGA 表彰 move 開始\n";
// I called the display of the 4th place person.
char* D_800ECEF0 = "４位の人の表示をコールしました。\n";
// Arrive at the podium
char* D_800ECF14 = "表彰台に到着\n";
// Everyone gather!
char* D_800ECF24 = "全員集合！\n";
// Arrive on the road
char* D_800ECF30 = "道路に到着\n";
// 4th place Finished
char* D_800ECF3C = "４位の人終了\n";
// OGA commendation move end
char* D_800ECF4C = "OGA 表彰 move 終了\n";
char* D_800ECF60 = "OGAWA DEBUG DRAW\n";

// utils function path, cpu, vehicle

s16 OgaCalcDirection(Vec3f arg0, Vec3f arg1) {
    s16 temp_ret;
    s16 phi_v1;

    temp_ret = CalcDirection(arg0, arg1);
    phi_v1 = temp_ret;
    if (ScreenFlip != 0) {
        phi_v1 = -temp_ret;
    }
    return phi_v1;
}

bool OgaHitCheck(f32 vehicleX, f32 vehicleZ, f32 vehicleVelocityX, f32 vehicleVelocityZ, f32 distanceX,
                             f32 distanceY, f32 playerX, f32 playerZ) {
    f32 Velocity;
    f32 temp_f18;

    Velocity = sqrtf((vehicleVelocityX * vehicleVelocityX) + (vehicleVelocityZ * vehicleVelocityZ));
    if (Velocity < 0.01f) {
        return false;
    }
    temp_f18 =
        ((vehicleVelocityX / Velocity) * (playerX - vehicleX)) + ((vehicleVelocityZ / Velocity) * (playerZ - vehicleZ));
    if ((-distanceX < temp_f18) && (temp_f18 < distanceX)) {
        temp_f18 = ((vehicleVelocityZ / Velocity) * (playerX - vehicleX)) +
                   (-(vehicleVelocityX / Velocity) * (playerZ - vehicleZ));
        if ((-distanceY < temp_f18) && (temp_f18 < distanceY)) {
            return true;
        }
    }
    return false;
}

void OgaGuruguruSub(Vec3f newPos, Vec3f oldPos, s16 orientationY) {
    f32 x_dist;
    f32 z_dist;
    f32 temp1;
    f32 temp2;
    f32 sine;
    f32 cosine;

    if (ScreenFlip != 0) {
        orientationY = -orientationY;
    }
    x_dist = newPos[0] - oldPos[0];
    z_dist = newPos[2] - oldPos[2];
    sine = SinT(orientationY);
    cosine = CosT(orientationY);
    temp1 = ((x_dist * cosine) + (z_dist * sine));
    temp2 = ((z_dist * cosine) - (x_dist * sine));
    newPos[0] = oldPos[0] + temp1;
    newPos[2] = oldPos[2] + temp2;
}

s32 CheckPlayerNear(Vec3f vehiclePos, f32 renderDistance, s32 flags) {
    Camera* Camera;
    Player* Player;
    f32 x;
    f32 z;
    f32 playerX;
    f32 playerZ;
    s32 i;
    s32 Flag;
    s8 numScreens;

    x = vehiclePos[0];
    z = vehiclePos[2];
    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            numScreens = 1;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            numScreens = 2;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            numScreens = MenuMultiplayerSelection;
            break;
        default:
            numScreens = 1;
            break;
    }
    Flag = flags;
    if (!DemoFlag) {
        Player = PlayerOne;
        // Checks Distance from each Player.
        for (i = 0; i < MenuMultiplayerSelection; i++, Player++) {
            if (((Player->Flag & IS_PLAYER) != 0) && ((Player->Flag & IS_CPU_PLAYER) == 0)) {
                playerX = Player->Position[0];
                playerZ = Player->Position[2];

                // Is Player within render Distance
                if (((playerX - renderDistance) < x) && ((playerX + renderDistance) > x) &&
                    ((playerZ - renderDistance) < z) && ((playerZ + renderDistance) > z)) {
                    // Sets the render Flag to on for each Player.
                    Flag |= (RENDER_VEHICLE << i);
                } else {
                    // Sets the render Flag to off for each Player.
                    Flag &= ~(RENDER_VEHICLE << i);
                }
            }
        }
    } else { // Demo cinematic uses the Camera to check render Distance
        Camera = Camera1;
        for (i = 0; i < numScreens; i++, Camera++) {
            playerX = Camera->CameraPos[0];
            playerZ = Camera->CameraPos[2];
            if (((playerX - renderDistance) < x) && (x < (playerX + renderDistance)) &&
                ((playerZ - renderDistance) < z) && (z < (playerZ + renderDistance))) {
                Flag |= (RENDER_VEHICLE << i);
            } else {
                Flag &= ~(RENDER_VEHICLE << i);
            }
        }
    }
    return Flag;
}

void OgaReverseChk(s32 PlayerId, Player* Player) {
    s16 playerAngle;
    s16 rotationDifference;
    s16 pathPointAngle;
    s16 PathIndex;
    u32 pathPoint;

    PathIndex = (s16) CurrentPathID[PlayerId];
    pathPoint = NearestPathPointByPlayerId[PlayerId];

    playerAngle = (s16) ((s16) Player->Direction[1] / DEGREES(1));
    pathPointAngle = (s16) ((s16) AnglePathDataBP[PathIndex][pathPoint] / DEGREES(1));

    rotationDifference = playerAngle - pathPointAngle;

    if (rotationDifference < 0) {
        rotationDifference = -rotationDifference;
    }

    if ((PlayerPathPointTotalTable[PlayerId] < OldPylonCount[PlayerId]) &&
        (rotationDifference >= SEVERE_WRONG_DIRECTION_MIN) && (rotationDifference < SEVERE_WRONG_DIRECTION_MAX)) {
        OgaReverseCount[PlayerId]++;
        if ((OgaReverseCount[PlayerId]) >= WRONG_DIRECTION_FRAMES_LIMIT) {
            WrongwayFlagPlayer1[PlayerId] = 1;
            OgaReverseCount[PlayerId] = WRONG_DIRECTION_FRAMES_LIMIT;
            GamePlayers[PlayerId].SlipFlag |= REVERSE;
        }
    } else if ((rotationDifference < SEVERE_CORRECT_DIRECTION_MIN) ||
               (rotationDifference >= SEVERE_CORRECT_DIRECTION_MAX)) {
        WrongwayFlagPlayer1[PlayerId] = 0;
        OgaReverseCount[PlayerId] = 0;
        GamePlayers[PlayerId].SlipFlag &= ~REVERSE;
    }
    OldPylonCount[PlayerId] = PlayerPathPointTotalTable[PlayerId];
}

void SortOgawaRank(void) {
    s32 playerIdToSwap;
    f32 courseCompletionToSwap;
    s32 playerIdByRank[8];
    s32 prevPlayerId;
    UNUSED s32 pad;
    s32 numPlayer;
    s32 Rank;
    s32 PlayerId;
    s32 rankHigh;

    switch (GameMode) {
        case BATTLE:
        default:
            return; // HEY! returns, not breaks
        case GRAND_PRIX:
        case TIME_TRIALS:
            numPlayer = NUM_PLAYERS;
            break;
        case VERSUS:
            numPlayer = MenuMultiplayerSelection;
            break;
    }

    if (RankUpdateFinishFlag == 0) {
        for (Rank = 0; Rank < numPlayer; Rank++) {
            PlayerId = RankToNum[Rank];
            playerIdByRank[Rank] = PlayerId;
            RapDat[Rank] = RapCountF[PlayerId];
        }
    } else {
        // Used in grand Prix Mode once all players cross finish line to determine finish Order
        for (Rank = 0; Rank < numPlayer; Rank++) {
            PlayerId = RankToNum[Rank];
            playerIdByRank[Rank] = PlayerId;
            RapDat[Rank] = -TimePlayerLastTouchedFinishLine[PlayerId];
        }
    }

    for (Rank = 0; Rank < numPlayer - 1; Rank++) {
        if ((GamePlayers[RankToNum[Rank]].Flag & IS_RACE_FINISH)) {
            continue;
        }

        for (rankHigh = Rank + 1; rankHigh < numPlayer; rankHigh++) {
            if (RapDat[Rank] < RapDat[rankHigh]) {
                if (!(GamePlayers[RankToNum[rankHigh]].Flag & IS_RACE_FINISH)) {
                    playerIdToSwap = playerIdByRank[Rank];
                    playerIdByRank[Rank] = playerIdByRank[rankHigh];
                    playerIdByRank[rankHigh] = playerIdToSwap;
                    courseCompletionToSwap = RapDat[Rank];
                    RapDat[Rank] = RapDat[rankHigh];
                    RapDat[rankHigh] = courseCompletionToSwap;
                }
            }
        }
    }

    // actually player_id, not Rank
    for (Rank = 0; Rank < NUM_PLAYERS; Rank++) {
        OldOgawaRank[Rank] = GPCurrentRaceRankByPlayerId[Rank];
    }

    for (Rank = 0; Rank < numPlayer; Rank++) {
        RankToNum[Rank] = playerIdByRank[Rank];
        GPCurrentRaceRankByPlayerId[playerIdByRank[Rank]] = Rank;
    }
    for (Rank = 0; Rank < numPlayer; Rank++) {
        prevPlayerId = RealRankToNum[Rank];
        playerIdByRank[Rank] = prevPlayerId;
        RapDat[Rank] = RapCountF[prevPlayerId];
    }

    for (Rank = 0; Rank < numPlayer - 1; Rank++) {
        for (rankHigh = Rank + 1; rankHigh < numPlayer; rankHigh++) {
            if (RapDat[Rank] < RapDat[rankHigh]) {
                playerIdToSwap = playerIdByRank[Rank];
                playerIdByRank[Rank] = playerIdByRank[rankHigh];
                playerIdByRank[rankHigh] = playerIdToSwap;
                courseCompletionToSwap = RapDat[Rank];
                RapDat[Rank] = RapDat[rankHigh];
                RapDat[rankHigh] = courseCompletionToSwap;
            }
        }
    }

    for (Rank = 0; Rank < numPlayer; Rank++) {
        RealRank[playerIdByRank[Rank]] = Rank;
        RealRankToNum[Rank] = playerIdByRank[Rank];
    }
}

void SortGoalRank(void) {
    f32 temp_f0;
    UNUSED s32 pad;
    s32 playerIds[8];
    s32 temp_a0;
    s32 temp_t2_2;
    s32 compareIndex;
    s32 i;
    s32 numRacers;

    switch (GameMode) {
        case BATTLE:
        default:
            return; // HEY! returns, not breaks
        case GRAND_PRIX:
        case TIME_TRIALS:
            numRacers = 8;
            break;
        case VERSUS:
            numRacers = MenuMultiplayerSelection;
            break;
    }

    for (i = 0; i < numRacers; i++) {
        temp_a0 = RankToNum[i];
        playerIds[i] = temp_a0;
        RapDat[i] = -TimePlayerLastTouchedFinishLine[temp_a0];
    }

    for (i = 0; i < (numRacers - 1); i++) {
        for (compareIndex = i + 1; compareIndex < numRacers; compareIndex++) {
            if (RapDat[i] < RapDat[compareIndex]) {
                temp_t2_2 = playerIds[i];
                playerIds[i] = playerIds[compareIndex];
                playerIds[compareIndex] = temp_t2_2;
                temp_f0 = RapDat[i];
                RapDat[i] = RapDat[compareIndex];
                RapDat[compareIndex] = temp_f0;
            }
        }
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        OldOgawaRank[i] = GPCurrentRaceRankByPlayerId[i];
    }

    for (i = 0; i < numRacers; i++) {
        GPCurrentRaceRankByPlayerId[playerIds[i]] = i;
        RankToNum[i] = playerIds[i];
    }
}

void SortGoalRank2PGP(void) {
    f32 temp_a0;
    s32 temp;
    s32 sp68[8];
    UNUSED s32 pad;
    s32 temp_t1;
    s32 i;
    s32 j;
    s32 this_loops_upper_bound_is_brough_to_you_by_the_number = 8;

    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number;) {
        RapDat[i++] = 0.0f;
    }

    for (j = 0, i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        if (GamePlayers[i].Flag & IS_RACE_FINISH) {
            sp68[j] = i;
            RapDat[j] = -TimePlayerLastTouchedFinishLine[i];
            j++;
        }
    }

    temp_t1 = j;
    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        if (!(GamePlayers[i].Flag & IS_RACE_FINISH)) {
            sp68[j] = i;
            RapDat[j] = RapCountF[i];
            j++;
        }
    }

    for (i = 0; i < (temp_t1 - 1); i++) {
        for (j = i + 1; j < temp_t1; j++) {
            if (RapDat[i] < RapDat[j]) {
                temp = sp68[i];
                sp68[i] = sp68[j];
                sp68[j] = temp;
                temp_a0 = RapDat[i];
                RapDat[i] = RapDat[j];
                RapDat[j] = temp_a0;
            }
        }
    }

    for (i = temp_t1; i < (this_loops_upper_bound_is_brough_to_you_by_the_number - 1); i++) {
        for (j = i + 1; j < this_loops_upper_bound_is_brough_to_you_by_the_number; j++) {
            if (RapDat[i] < RapDat[j]) {
                temp = sp68[i];
                sp68[i] = sp68[j];
                sp68[j] = temp;
                temp_a0 = RapDat[i];
                RapDat[i] = RapDat[j];
                RapDat[j] = temp_a0;
            }
        }
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        OldOgawaRank[i] = GPCurrentRaceRankByPlayerId[i];
    }

    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        GPCurrentRaceRankByPlayerId[sp68[i]] = i;
        RankToNum[i] = sp68[i];
    }
}

/**
 * Checks if a path Point is within a valid range of another path Point, accounting for track wrapping
 *
 * @Param pathPoint The path Point to check
 * @Param currentPathPoint The reference path Point
 * @Param backwardRange Number of path to look behind
 * @Param forwardRange Number of path to look ahead
 * @Param totalPathPoints Total Number of path in the track
 * @return
 *   1: path Point is within normal range
 *  -1: path Point is within wrapped range
 *   2: path Point is out of range
 *   0: invalid range parameters
 */
s32 ExistAreaChk(u16 pathPoint, u16 currentPathPoint, u16 backwardRange, u16 forwardRange,
                           u16 totalPathPoints) {
    s32 var_v1;

    var_v1 = 0;
    if ((currentPathPoint >= backwardRange) && (currentPathPoint < (totalPathPoints - forwardRange))) {
        if ((pathPoint >= (currentPathPoint - backwardRange)) && ((currentPathPoint + forwardRange) >= pathPoint)) {
            var_v1 = 1;
        }
    } else if ((((currentPathPoint + forwardRange) % totalPathPoints) < pathPoint) &&
               ((((currentPathPoint + totalPathPoints) - backwardRange) % totalPathPoints) >= pathPoint)) {
        var_v1 = -1;
    } else {
        var_v1 = 2;
    }
    return var_v1;
}

// cpu util functions
#include "cpu_vehicles_camera_path/cpu_speed_control.inc.c"

bool MakeKartDistance(s32 PlayerId, s16 arg1, s16 Arg2) {
    Player* Player;
    s16* temp_a3;
    s32 progress;
    f32 temp_f0;
    s16 var_t1;
    u16* var_a0_3;
    s16 temp;
    s16 temp2;
    s32 i;
    s32 var_a0;
    s16 var_v0;
    s16 var_a0_4;
    s32* var_v1;
    s16 STEMP_V1;
    s16 STEMP_V0;
    s16 Rank;

    EnemyDist[PlayerId] = -1;
    EnemyLimdist[PlayerId] = -1;
    if (GameMode == TIME_TRIALS) {
        return 1;
    }
    if (arg1 < 0) {
        return 1;
    }
    if (arg1 >= 4) {
        arg1 = 3;
    }
    if (OgaCarStat[PlayerId] == 1) {
        return 1;
    }
    Player = &GamePlayers[PlayerId];
    if (Player->Flag & IS_PLAYER) {
        return 1;
    }

    temp_a3 = &GET_COURSE_800DCBB4(arg1 * 8);
    if (Arg2 == 0) {
        if (DemoFlag == 1) {
            STEMP_V0 = PlayerPathPointTotalTable[PlayerId];
            STEMP_V1 = PlayerPathPointTotalTable[RealRankToNum[7]];
            progress = STEMP_V0 - STEMP_V1;
            if (progress < 0) {
                progress = -progress;
            }
            if (arg1 < 3) {
                STEMP_V0 = temp_a3[0];
                STEMP_V1 = temp_a3[8];
                temp_f0 = RateCountF[PlayerId];
                var_a0 = (STEMP_V1 * temp_f0) + (STEMP_V0 * (1.0f - temp_f0));
            } else {
                var_a0 = temp_a3[0];
            }
            EnemyLimdist[PlayerId] = var_a0;
            if ((EnemyLimdist[PlayerId] < progress) && (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
                return 0;
            }
            return 1;
        }

        STEMP_V0 = PlayerPathPointTotalTable[PlayerId];
        STEMP_V1 = PlayerPathPointTotalTable[EnemyTargetPlayer];
        progress = STEMP_V0 - STEMP_V1;
        Rank = GPCurrentRaceRankByPlayerId[EnemyTargetPlayer];
        if (((((CourseTotalPathPoints[0] * 2) / 3)) < progress) && ((Rank) >= 6)) {
            STEMP_V0 = PlayerPathPointTotalTable[PlayerId];
            STEMP_V1 = temp = PlayerPathPointTotalTable[RealRankToNum[Rank - 1]];
            progress = STEMP_V0 - STEMP_V1;
        }
        if (progress < 0) {
            progress = -progress;
        }
        if (arg1 < 3) {
            STEMP_V0 = temp_a3[0];
            STEMP_V1 = temp_a3[8];
            temp_f0 = RateCountF[PlayerId];
            var_a0 = (STEMP_V1 * temp_f0) + (STEMP_V0 * (1.0f - temp_f0));
        } else {
            var_a0 = temp_a3[0];
        }
        EnemyLimdist[PlayerId] = var_a0 = (RaceClass + 1) * var_a0;
        if ((var_a0 < progress) && (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
            return 0;
        }
        return 1;
    }

    temp2 = EnemyRivalNum[0];
    STEMP_V0 = PlayerPathPointTotalTable[PlayerId];
    STEMP_V1 = PlayerPathPointTotalTable[temp2];
    progress = STEMP_V1 - STEMP_V0;
    if (progress < 0) {
        progress = -progress;
    }

    var_v0 = 0;
    for (i = 0; i < 2; i++) {
        if (GPCurrentRaceRankByPlayerId[EnemyRivalNum[i] & 0xFFFF] < Arg2) {
            var_v0++;
        }
    }

    var_a0_4 = 0;
    for (i = 0; i < MenuMultiplayerSelection; i++) {
        if (GPCurrentRaceRankByPlayerId[i] < Arg2) {
            var_a0_4++;
        }
    }

    // FAKE
    var_t1 = (Arg2 - (var_v0 & 0xFFFF)) - var_a0_4;
    Arg2 -= var_v0;

    if ((var_v0 > 0) || (var_a0_4 > 0)) {
        var_t1++;
    }
    EnemyGetaRank[PlayerId] = var_t1;
    if ((var_t1 < 0) || (var_t1 >= 8)) {
        return 0;
    }
    if (arg1 < 3) {
        STEMP_V0 = temp_a3[var_t1 + 0];
        STEMP_V1 = temp_a3[var_t1 + 8];
        temp_f0 = RateCountF[PlayerId];
        var_a0 = (STEMP_V1 * temp_f0) + (STEMP_V0 * (1.0f - temp_f0));
    } else {
        var_a0 = temp_a3[var_t1];
    }
    EnemyDist[PlayerId] = progress;
    EnemyLimdist[PlayerId] = var_a0;
    if (var_a0 < progress) {
        return 1;
    }
    return 0;
}

void NewBipasNumInit(s32 PathIndex) {
    PathPointPointer = CenterPathBP[PathIndex];
    SidePointL = SideLPathBP[PathIndex];
    SidePointR = SideRPathBP[PathIndex];
    CurveData = CurvePathDataBP[PathIndex];
    AngleData = AnglePathDataBP[PathIndex];
    ShortCutData = ShortCutPathDataBP[PathIndex];
    CenterLineMax = CourseTotalPathPoints[PathIndex];
}

s32 KyouseiBipasSet(s32 payerId, s32 PathIndex) {
    f32 PosX;
    f32 PosY;
    f32 PosZ;
    Player* Player;
    s32 trackSectionId;
    UNUSED s32 stackPadding;

    Player = &GamePlayers[payerId];
    PosX = Player->Position[0];
    PosY = Player->Position[1];
    PosZ = Player->Position[2];
    stackPadding = PathIndex;
    trackSectionId = CheckArea(Player->Bump.LastZX);
    LakituSurface[payerId] = trackSectionId;
    OGAArea = CalcOGAAreaSubBPWrapper(PosX, PosY, PosZ, trackSectionId, &PathIndex);
    NearestPathPointByPlayerId[payerId] = OGAArea;
    if (PathIndex) {};
    CurrentPathID[payerId] = PathIndex;
    NewBipasNumInit(PathIndex);
    if (stackPadding) {};
    return PathIndex;
}

void PylonCountReflesh(s32 PlayerId) {
    f32 percent;

    // arbitrary score calculation
    PlayerPathPointTotalTable[PlayerId] =
        (LapCountByPlayerId[PlayerId] * CourseTotalPathPoints[0]) + OGAArea;

    // calculate completion in percent
    percent = (f32) NearestPathPointByPlayerId[PlayerId] / (f32) CourseTotalPathPoints[CurrentPathID[PlayerId]];
    RateCountF[PlayerId] = percent;
    RapCountF[PlayerId] = percent;
    RapCountF[PlayerId] += LapCountByPlayerId[PlayerId];
}

void MazeEnterOrExitCheck(s32 PlayerId) {
    s16 previous;

    previous = EnemyEnterMaze[PlayerId];
    if (OGAArea >= 0x6D) {
        EnemyEnterMaze[PlayerId] = true;
        switch (BipasNum) {
            case 0:
                if (OGAArea >= 0x20F) {
                    EnemyEnterMaze[PlayerId] = false;
                }
                break;
            case 1:
                if (OGAArea >= 0x206) {
                    EnemyEnterMaze[PlayerId] = false;
                }
                break;
            case 2:
                if (OGAArea >= 0x211) {
                    EnemyEnterMaze[PlayerId] = false;
                }
                break;
            case 3:
                if (OGAArea >= 0x283) {
                    EnemyEnterMaze[PlayerId] = false;
                }
                break;
        }
    }
    if ((previous == false) && (EnemyEnterMaze[PlayerId] == true)) {
        EnemyEnterTrg[PlayerId] = true;
    }
    if ((previous == true) && (EnemyEnterMaze[PlayerId] == false)) {
        EnemyExitTrg[PlayerId] = true;
    }
}

void MazeBipasChangeCheck(s32 PlayerId, Player* Player) {
    f32 PosX;
    f32 PosY;
    f32 PosZ;
    UNUSED f32 pad[2];

    PosX = Player->Position[0];
    PosY = Player->Position[1];
    PosZ = Player->Position[2];
    if (EnemyEnterTrg[PlayerId] == 1) {
        BipasNum = KyouseiBipasSet(PlayerId, MakeRandomLimmit(4));
        OGAArea = GetOgaAreaKart(PosX, PosY, PosZ, 0, Player, PlayerId, BipasNum);
        NearestPathPointByPlayerId[PlayerId] = OGAArea;
        PylonCountReflesh(PlayerId);
        EnemyEnterTrg[PlayerId] = 0;
    }
    if (EnemyExitTrg[PlayerId] == 1) {
        BipasNum = KyouseiBipasSet(PlayerId, 0);
        OGAArea = GetOgaAreaKart(PosX, PosY, PosZ, 0, Player, PlayerId, BipasNum);
        NearestPathPointByPlayerId[PlayerId] = OGAArea;
        PylonCountReflesh(PlayerId);
        EnemyExitTrg[PlayerId] = 0;
    }
}

/**
 * Helps calculate Time since Player last touched finishline.
 * Assumes constant z-Speed and subtracts portion of frame where the finish line was already crossed
 **/
f32 OgaGetLaptime(UNUSED s32 PlayerId, f32 previousPlayerZ, f32 playerZ) {
    f32 z_change_after_cross = FinishLineZ - playerZ;
    f32 z_change_before_cross = previousPlayerZ - FinishLineZ;
    return GameTimer - ((COURSE_TIMER_ITER_f * z_change_after_cross) / (z_change_after_cross + z_change_before_cross));
}

void CheckLapCount(s32 PlayerId, Player* Player) {
    f32 playerX;
    f32 playerY;
    f32 playerZ;
    s32 var_v0;
    UNUSED s16 pad;
    f32 previousPlayerZ;

    playerX = Player->Position[0];
    playerY = Player->Position[1];
    playerZ = Player->Position[2];
    previousPlayerZ = OgaOldRapChkZ[PlayerId];
    OgaAreaChgFlg = false;
    RapCountFlg[PlayerId] = 0;
    OGAArea = GetOgaAreaKart(playerX, playerY, playerZ, NearestPathPointByPlayerId[PlayerId], Player,
                                               PlayerId, BipasNum);
    OgaAreaOrg = OGAArea;
    if (NearestPathPointByPlayerId[PlayerId] != OGAArea) {
        NearestPathPointByPlayerId[PlayerId] = OGAArea;
        OgaAreaChgFlg = true;
        PylonCountReflesh(PlayerId);
    }
    if (CourseID == COURSE_AWARD_CEREMONY) {
        ZureChkBP(PlayerId, OGAArea, BipasNum);
        return;
    }
    if ((OGAArea < 0x14) || ((CourseTotalPathPoints[BipasNum] - 0x14) < OGAArea) ||
        (CourseID == COURSE_KALAMARI_DESERT)) {
        s16 var_v1 = 0;
        s16 var_t0 = 0;
        if (CourseID == COURSE_KALAMARI_DESERT) {
            OgawaKarakaraShortCutOKFlg = 0;
            if (Player->SlipFlag & STAR) {
                OgawaKarakaraShortCutOKFlg = 1;
            }
            if (ScreenFlip != 0) {
                if (playerX < 300.0f) {
                    var_v1 = 1;
                    var_t0 = 1;
                } else if ((playerX < 1300.0f) && (LapCountByPlayerId[PlayerId] < 2) && ((OgawaKarakaraShortCutOKFlg == 1))) {
                    var_v1 = 1;
                    var_t0 = 1;
                }
            } else {
                if (playerX > -300.0f) {
                    var_v1 = 1;
                    var_t0 = 1;
                } else {
                    if ((playerX > -1300.0f) && (LapCountByPlayerId[PlayerId] < 2) && (OgawaKarakaraShortCutOKFlg == 1)) {
                        var_v1 = 1;
                        var_t0 = 1;
                    }
                }
            }
        } else {
            var_v1 = 1;
            var_t0 = 1;
        }
        previousPlayerZ = OgaOldRapChkZ[PlayerId];
        if ((var_v1 != 0) && (playerZ <= FinishLineZ)) {
            if (FinishLineZ < previousPlayerZ) {
                LapCountByPlayerId[PlayerId]++;
                if ((GameMode == GRAND_PRIX) && (LapCountByPlayerId[PlayerId] == 5)) {
                    if (RealRank[PlayerId] == 7) {
                        // clang-format off
                        for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) { LapCountByPlayerId[var_v0]--; } // has to be one line to match
                        // clang-format on
                    }
                }
                RapCountFlg[PlayerId] = 1;
                PylonCountReflesh(PlayerId);
                ActionFlgWakeup(PlayerId);
                EnemyItemTable[PlayerId].Limit = 0;
                if ((RankUpdateFinishFlag == 0) && !(Player->Flag & IS_RACE_FINISH)) {
                    TimePlayerLastTouchedFinishLine[PlayerId] = OgaGetLaptime(PlayerId, previousPlayerZ, playerZ);
                }
            }
        }
        if ((var_t0 != 0) && (previousPlayerZ <= FinishLineZ) && (FinishLineZ < playerZ)) {
            LapCountByPlayerId[PlayerId]--;
            PylonCountReflesh(PlayerId);
        }
    }
    OgaOldRapChkZ[PlayerId] = playerZ;
    if ((CourseID == COURSE_YOSHI_VALLEY) && (OgaAreaChgFlg == true)) {
        MazeEnterOrExitCheck(PlayerId);
        if (((Player->Flag & IS_PLAYER) == 0) || (Player->Flag & IS_CPU_PLAYER)) {
            MazeBipasChangeCheck(PlayerId, Player);
        }
    }
    if ((Player->Flag & IS_PLAYER) && !(Player->Flag & IS_CPU_PLAYER)) {
        OgaReverseChk(PlayerId, Player);
        if ((GameMode == GRAND_PRIX) && (MenuMultiplayerSelection == 2) && (PlayerId == 0)) {
            if (RealRank[PLAYER_ONE] < RealRank[PLAYER_TWO]) {
                EnemyTargetPlayer = PLAYER_ONE;
            } else {
                EnemyTargetPlayer = PLAYER_TWO;
            }
        }
    } else {
        //????
    }
    ZureChkBP(PlayerId, OGAArea, BipasNum);
}

void EtcEnemyDrive(void) {
    s32 i;
    KameHibasiraMove();
    OgaEtcFrameCounter++;

    if (CourseID == COURSE_AWARD_CEREMONY) {
        for (i = 0; i < 7; i++) {
            MoveBOM(i);
        }
        return;
    }

    // EtcEnemyDrive is ran twice per frame.
    // This makes vehicles only tick once per frame.
    if (OgaEtcFrameCounter & 1) {
        if (GameMode == VERSUS) {
            for (i = 0; i < 7; i++) {
                MoveBOM(i);
            }
        }
#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (CourseID) {
            case COURSE_KALAMARI_DESERT:
                MoveSL();
                break;
            case COURSE_DK_JUNGLE:
                MoveSHIP();
                break;
            case COURSE_TOADS_TURNPIKE:
                MoveTRUCK();
                MoveBUS();
                MoveTANK();
                MoveRV();
                break;
        }
#else

#endif
    }
}

void OgaEnemyVoice(s32 arg0, Player* Player) {
    if (EnemyVoiceCountSPIN[arg0] >= 0xB) {
        if ((Player->SlipFlag & SPIN_L) || (Player->SlipFlag & SPIN_R) ||
            (Player->SlipFlag & THUNDER_SPIN)) {
            NaEnmVoiceStart(arg0, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0A));
            EnemyVoiceCountSPIN[arg0] = 0;
        }
    }
    if (EnemyVoiceCountDAMAGE[arg0] >= 0xB) {
        if ((Player->Weapon & HIT_BOMB) || (Player->Weapon & HIT_BOMB_THROW) ||
            (Player->Weapon & HIT_REDSHELL) || (Player->Weapon & HIT_GREENSHELL) ||
            (Player->SlipFlag & BROKEN)) {
            NaEnmVoiceStart(arg0, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0B));
            EnemyVoiceCountDAMAGE[arg0] = 0;
        }
    }
}

void OgaEnemyVoiceOvertake(s32 PlayerId, UNUSED Player* Unused) {
    s32 otherPlayerId;

    if (EnemyVoiceCountOVERTAKE[PlayerId] >= 0x65) {
        for (otherPlayerId = 0; otherPlayerId < MenuMultiplayerSelection; otherPlayerId++) {
            // detect swap of positions
            if ((GPCurrentRaceRankByPlayerId[PlayerId] < GPCurrentRaceRankByPlayerId[otherPlayerId]) &&
                (GPCurrentRaceRankByPlayerId[PlayerId] == OldOgawaRank[otherPlayerId]) &&
                (GPCurrentRaceRankByPlayerId[otherPlayerId] == OldOgawaRank[PlayerId])) {
                NaEnmVoiceStart(PlayerId, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                EnemyVoiceCountOVERTAKE[PlayerId] = 0;
            }
        }
    }
    if (EnemyVoiceCountOVERTAKE[PlayerId] < 0x3E8) {
        EnemyVoiceCountOVERTAKE[PlayerId]++;
    }
    if (EnemyVoiceCountSPIN[PlayerId] < 0xC8) {
        EnemyVoiceCountSPIN[PlayerId]++;
    }
    if (EnemyVoiceCountDAMAGE[PlayerId] < 0xC8) {
        EnemyVoiceCountDAMAGE[PlayerId]++;
    }
}

void DriveEnemyKart(s32 PlayerId) {
    UNUSED s32 pad[14];
    s16 var_a0_2;
    s16 newAngle;
    u16 PathIndex;

    f32 distX;
    f32 minAngle;

    s16 Angle;
    s16 steeringSensitivity;


    s32 maxAngle;
    Player* Player;
    UNUSED s32 Pad3[10];
    CenterPathStruct* pathPoint;
    f32 onePointFive = 1.5f;

    Player = &GamePlayers[PlayerId];
    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        EnemyTimeCount[PlayerId] += 1;
        if (PlayerId == 0) {
            OgaFrameCounter++;
            if (OgaFrameCounter & 1) {
                OgaBumpChkCounter += 1;
            }
        }
        if (!(Player->Flag & EXISTS)) {
            PlayerPathPointTotalTable[PlayerId] = -0x00000014;
            RapCountF[PlayerId] = -1000.0f;
            RateCountF[PlayerId] = -1000.0f;
            return;
        }
        OgaHamidasiMapFlg[PlayerId] = 0;
        // clang-format off
        if (Player->Position[0] < MinimumX) {            OgaHamidasiMapFlg[PlayerId] = 1;        }
        if (MaximumX < Player->Position[0]) {            OgaHamidasiMapFlg[PlayerId] = 2;        }
        if (Player->Position[2] < MinimumZ) {            OgaHamidasiMapFlg[PlayerId] = 3;        }
        if (MaximumZ < Player->Position[2]) {            OgaHamidasiMapFlg[PlayerId] = 4;        }
        // clang-format on

        if (!(Player->JugemuFlag & ON_LAKITU_ROD) && !(Player->JugemuFlag & LAKITU_SCENE)) {
            BipasNum = CurrentPathID[PlayerId];
            NewBipasNumInit(BipasNum);
            switch (CourseID) { /* irregular */
                case COURSE_KALAMARI_DESERT:
                    HitCheckSL(PlayerId, Player);
                    if (PlayerId == 0) {
                        FumikiriMove();
                    }
                    break;
                case COURSE_DK_JUNGLE:
                    HitCheckSHIP(Player);
                    break;
                case COURSE_TOADS_TURNPIKE:
                    HitCheckTRUCK(PlayerId, Player);
                    HitCheckBUS(PlayerId, Player);
                    HitCheckTANK(PlayerId, Player);
                    HitCheckRV(PlayerId, Player);
                    break;
            }
            if (Player->Flag & IS_RACE_FINISH) {
                Player->SlipFlag &= ~REVERSE;
                Player->HandlingFlag &= ~REVERSE_GEAR;
            }
            SprintLapCheck(PlayerId, Player);

            // Because this takes place in a per-Player Loop and runs primarily when Player 1 crosses the finish line,
            // Player 1 effectively has a 1 frame advantage when determining places. However, it runs for everyone when
            // they cross the finish line, resulting in lower port numbers always winning ties in versus Mode.
            if ((CourseID != COURSE_AWARD_CEREMONY) && ((RapCountFlg[PlayerId] == 1) || (PlayerId == 0))) {
                SortOgawaRank();
            }
            if (Player->Flag & IS_CPU_PLAYER) {
                if ((OgaAreaChgFlg == true) && (CourseID != COURSE_AWARD_CEREMONY)) {
                    ActionStartEndChk(PlayerId);
                }
                // one update it try to use an Item, the other it doesn't
                if ((PlayerId & 1) != (OgaFrameCounter & 1)) {
                    EnemyItemMove(PlayerId);
                }
                OgaEnemyVoiceOvertake(PlayerId, Player);
                BiriFlg = 0;
                switch (GameMode) { /* switch 1; irregular */
                    case 1:               /* switch 1 */
                    case 2:               /* switch 1 */
                    case 3:               /* switch 1 */
                        break;
                    case 0: /* switch 1 */
                        break;
                }
                TenukiFlg[PlayerId] = false;
                if ((Player->SlipFlag & CENTER_LINE) && (CourseID != COURSE_AWARD_CEREMONY)) {
                    TenukiFlg[PlayerId] = true;
                }
                if ((D_801646CC == 1) || (Player->Flag & IS_RACE_FINISH) ||
                    (CourseID == COURSE_AWARD_CEREMONY)) {
                    if (CourseID != COURSE_TOADS_TURNPIKE) {
                        LaneData[PlayerId].LaneTarget = 0.0f;
                    }
                    LaneData[PlayerId].LaneFreeDrive = 0.0f;
                }
                if (BipasNum > 0) {
                    LaneData[PlayerId].LaneTarget = 0.0f;
                    LaneData[PlayerId].LaneFreeDrive = 0.0f;
                }
                EnemyTenukiY[PlayerId] =
                    CenterPathBP[BipasNum][NearestPathPointByPlayerId[PlayerId]].Pointy + 4.3f;
                if ((OldTenukiFlg[PlayerId] == 1) && (TenukiFlg[PlayerId] == false)) {
                    SetHight(Player);
                }
                if (TenukiFlg[PlayerId] == true) {
                    Player->Position[1] = EnemyTenukiY[PlayerId];
                }
                OldTenukiFlg[PlayerId] = TenukiFlg[PlayerId];
                switch (CourseID) {
                    case COURSE_YOSHI_VALLEY:
                    case COURSE_AWARD_CEREMONY:
                        LaneData[PlayerId].LaneTarget = 0.0f;
                        break;
                    default:
                        break;
                    case COURSE_TOADS_TURNPIKE:
                        AvoidTRUCK(PlayerId);
                        AvoidBUS(PlayerId);
                        AvoidTANK(PlayerId);
                        AvoidRV(PlayerId);
                        break;
                }
                if (TenukiFlg[PlayerId] == true) {
                    DriftFlg[PlayerId] = 0;
                    Player->SlipFlag &= ~DRIFT;
                    if ((PlayerId & 1) != (OgaFrameCounter & 1)) {
                        EnemyStickAngle(Player, 0);
                        OgaAccele(PlayerId, OldLimmitspeed[PlayerId], Player);
                        return;
                    }
                    if ((MenuMultiplayerSelection > 0) && (MenuMultiplayerSelection < 3) && (OgaCarStat[PlayerId] == 1) &&
                        (EnemyRivalAimRank[PlayerId] < GPCurrentRaceRankByPlayerId[PlayerId])) {
                        OldLimmitspeed[PlayerId] = 8.333333f;
                    } else if (BiriFlg == (s16) 1U) {
                        OldLimmitspeed[PlayerId] = GET_COURSE_D_0D0096B8(RaceClass);
                        LaneData[PlayerId].LaneTarget = -0.5f;
                    } else if (ShortCutData[OGAArea] > 0) {
                        OldLimmitspeed[PlayerId] = GET_COURSE_cpu_CurveTargetSpeed(RaceClass);
                    } else {
                        OldLimmitspeed[PlayerId] = GET_COURSE_cpu_NormalTargetSpeed(RaceClass);
                    }
                    FumikiriWaitCheck(PlayerId);
                    SoukouZureMove(PlayerId);
                    MokuhyouSetCenter(PlayerId, OgaAreaOrg);
                    distX = object[0] - Player->Position[0];
                    minAngle = object[2] - Player->Position[2];
                    if (!(Player->SlipFlag & SPIN_L) && !(Player->SlipFlag & SPIN_R) &&
                        !(Player->SlipFlag & WHEELSPIN)) {
                        if (((distX * distX) + (minAngle * minAngle)) > 6400.0f) {
                            if (BipasNum == 0) {
                                AvoidCrash(PlayerId);
                                if (AvoidMode[PlayerId] > 0) {
                                    PathIndex = OgaAreaOrg + 5;
                                    PathIndex %= CenterLineMax;
                                    ObjCalcFromZureBP(PathIndex, ZureDist[PlayerId], BipasNum);
                                }
                            }
                            Player->Direction[1] = -CalcDirection(Player->Position, object);
                        } else {
                            Player->Direction[1] =
                                AnglePathDataBP[BipasNum]
                                                     [(OgaAreaOrg + 4) % CenterLineMax];
                        }
                    }
                    EnemyStickAngle(Player, 0);
                    OgaAccele(PlayerId, OldLimmitspeed[PlayerId], Player);
                    return;
                }
                if ((DriftFlg[PlayerId] == 1) || (DriftFlg[PlayerId] == -1)) {
                    Player->SlipFlag |= DRIFT;
                }
                if (DriftFlg[PlayerId] != 0) {
                    OgaAngle[PlayerId] = -CalcDirection(Player->OldPosition, Player->Position);
                    var_a0_2 =
                        (AngleData[(OGAArea + 2) % CenterLineMax] *
                         0x168) /
                        65535;
                    newAngle = (OgaAngle[PlayerId] * 0x168) / 65535;
                    if (var_a0_2 < -0xB4) {
                        var_a0_2 += 0x168;
                    }
                    if (var_a0_2 > 0xB4) {
                        var_a0_2 -= 0x168;
                    }
                    if (newAngle < -0xB4) {
                        newAngle += 0x168;
                    }
                    if (newAngle > 0xB4) {
                        newAngle -= 0x168;
                    }
                    steeringSensitivity = var_a0_2 - newAngle;
                    if (steeringSensitivity < -0xB4) {
                        steeringSensitivity += 0x168;
                    }
                    if (steeringSensitivity > 0xB4) {
                        steeringSensitivity -= 0x168;
                    }
                    switch (DriftFlg[PlayerId]) {
                        case -1:
                            if (steeringSensitivity > 5) {
                                DriftFlg[PlayerId] = 0;
                                Player->SlipFlag &= ~DRIFT;
                            }
                            break;
                        case 1:
                            if (steeringSensitivity < -5) {
                                DriftFlg[PlayerId] = 0;
                                Player->SlipFlag &= ~DRIFT;
                            }
                            break;
                        default:
                            break;
                    }
                }

                if ((PlayerId & 1) != (OgaFrameCounter & 1)) {
                    EnemyStickAngle(Player, OldEosAngleOga[PlayerId]);
                    OgaAccele(PlayerId, OldLimmitspeed[PlayerId], Player);
                    return;
                }
                StraightFlg[PlayerId] = StraightChk(PlayerId, OGAArea);
                MokuhyouSetCenter(PlayerId, OGAArea);
                if (CourseID != COURSE_AWARD_CEREMONY) {
                    if (PlayerPathPointTotalTable[PlayerId] < 0xB) {
                        PathIndex = OgaAreaOrg;
                        if ((PlayerPathPointTotalTable[PlayerId] > 0) && (CourseID == COURSE_TOADS_TURNPIKE)) {
                            PathIndex += 0x14;
                            PathIndex %= CenterLineMax;
                            ObjCalcFromZureBP(PathIndex, 0.0f, 0);
                            LaneData[PlayerId].LaneTarget = 0.0f;
                        } else {
                            PathIndex += 8;
                            PathIndex %= CenterLineMax;
                            ObjCalcFromZureBP(PathIndex, ZureData[PlayerId], BipasNum);
                            LaneData[PlayerId].Lane = ZureData[PlayerId];
                        }
                    }
                    if ((BiriFlg == 1) && (AvoidMode[PlayerId] == 0)) {
                        PathIndex = OgaAreaOrg + 7;
                        PathIndex %= CenterLineMax;
                        ObjCalcFromZureBP(PathIndex, -0.7f, BipasNum);
                    }
                    if (1) { } if (1) { } if (1) { } if (1) { } if (1) { } if (1) { }
                    if (BipasNum == 0) {
                        AvoidCrash(PlayerId);
                        if (AvoidMode[PlayerId] > 0) {
                            PathIndex = OgaAreaOrg + 5;

                            PathIndex %= CenterLineMax;
                            ObjCalcFromZureBP(PathIndex, ZureDist[PlayerId], BipasNum);
                        }
                    }
                }
                if (CourseID == COURSE_AWARD_CEREMONY) {
                    switch (OgaNearHyousyouFlg[PlayerId]) { /* switch 3; irregular */
                        case 3:                     /* switch 3 */
                            object[0] = OgaHyousyouMx[PlayerId];
                            object[2] = OgaHyousyouMz[PlayerId];
                            break;
                        case 4: /* switch 3 */
                            pathPoint = &CenterPathBP[PlayerId][(NearestPathPointByPlayerId[PlayerId] + 0xA) %
                                                               CourseTotalPathPoints[PlayerId]];
                            object[0] = pathPoint->Pointx;
                            object[2] = pathPoint->Pointz;
                            break;
                    }
                }
                object[0] = (EnemyObjectX[PlayerId] + object[0]) * 0.5f; // average
                object[2] = (EnemyObjectZ[PlayerId] + object[2]) * 0.5f; // average
                EnemyObjectX[PlayerId] = object[0];
                EnemyObjectZ[PlayerId] = object[2];
                minAngle = onePointFive * (f32) DEGREES(1);
                maxAngle = -onePointFive * (f32) DEGREES(1);

                Angle = -CalcDirection(Player->Position, object);
                Angle -= (newAngle = Player->Direction[1]);
                if ((s16) minAngle < Angle) {
                    Angle = minAngle;
                }
                if (Angle < (s16) maxAngle) {
                    Angle = maxAngle;
                }
                steeringSensitivity = GET_COURSE_AISteeringSensitivity;
                switch (CurveData[PlayerId]) { /* switch 4; irregular */
                    case RIGHT_CURVE:                              /* switch 4 */
                        if (ZureData[PlayerId] > (0.5f * 1.0f)) {
                            steeringSensitivity = 0x0014;
                        }
                        if (ZureData[PlayerId] < -0.5f) {
                            steeringSensitivity = 0x0035;
                        }
                        break;
                    case LEFT_CURVE: /* switch 4 */
                        if (ZureData[PlayerId] > 0.5f) {
                            steeringSensitivity = 0x0035;
                        }
                        if (ZureData[PlayerId] < -0.5f) {
                            steeringSensitivity = 0x0014;
                        }
                        break;
                }
                if ((ActionFlg[PlayerId] == CPU_BEHAVIOUR_STATE_RUNNING) &&
                    ((ZureData[PlayerId] > 0.9f) || (ZureData[PlayerId] < -0.9f))) {
                    DriftFlg[PlayerId] = 0;
                    Player->SlipFlag &= ~DRIFT;
                }
                if (Player->SlipFlag & D_JUMP) {
                    switch (DriftFlg[PlayerId]) {
                        case 1:
                            newAngle = 0x0035;
                            break;
                        case -1:
                            newAngle = -0x0035;
                            break;
                        default:
                            newAngle =
                                (OldEosAngleOga[PlayerId] + ((Angle * steeringSensitivity) / minAngle)) / 2;
                            break;
                    }
                } else if (Player->SlipFlag & (HEIGHT_JUMP | N_JUMP | DASH_MIDJUMP)) {
                    newAngle = 0;
                } else {
                    newAngle = (OldEosAngleOga[PlayerId] + ((Angle * steeringSensitivity) / minAngle)) / 2;
                }
                EnemyStickAngle(Player, newAngle);
                OldEosAngleOga[PlayerId] = newAngle;
                if ((StraightFlg[PlayerId] == true) || (DriftFlg[PlayerId] == 1) ||
                    (DriftFlg[PlayerId] == -1) ||
                    (Player->SlipFlag & (HEIGHT_JUMP | N_JUMP | DASH_MIDJUMP))) {
                    Speedlevel[PlayerId] = GET_COURSE_cpu_CurveTargetSpeed(RaceClass);
                } else {
                    Speedlevel[PlayerId] = GET_COURSE_cpu_NormalTargetSpeed(RaceClass);
                }
                if ((ZureData[PlayerId] > 0.9f) || (ZureData[PlayerId] < -0.9f)) {
                    Speedlevel[PlayerId] = GET_COURSE_cpu_OffTrackTargetSpeed(RaceClass);
                }
                if (BiriFlg == 1) {
                    Speedlevel[PlayerId] = GET_COURSE_D_0D0096B8(RaceClass);
                }
                if ((DriftFlg[PlayerId] == 2) || (DriftFlg[PlayerId] == -2) || (DriftFlg[PlayerId] == 3)) {
                    Speedlevel[PlayerId] = 3.3333333f;
                }
                Limmitspeed = Speedlevel[PlayerId];
                Player->SlipFlag &= ~SLIP_STREAM;
                OldLimmitspeed[PlayerId] = Limmitspeed;
                FumikiriWaitCheck(PlayerId);
                OgaAccele(PlayerId, Limmitspeed, Player);
            }
        }
    }
}

void AvoidCrash(s32 PlayerId) {
    s32 i;
    f32 temp_f12;
    f32 temp_f14;
    s32 j;
    f32 temp_f16;
    f32 temp_ft2;
    s16 currPathPoint;
    s16 temp_v1_2;
    f32 temp_f22;
    f32 temp_f0_2;
    f32 var_f18;
    f32 var_f20;
    UNUSED s32 pad[5];
    s16 spB0[8];
    UNUSED f32 Pad2;
    s16 sp9C[8];
    UNUSED f32 Pad3;
    f32 temp_f2;
    f32 sp74[8];
    s32 temp_a1_2;
    Player* Player;
    Player = &GamePlayers[PlayerId];

    if (Player->SlipFlag & DRIFT) {
        return;
    }

    if (DriftFlg[PlayerId] == 1) {
        return;
    }

    if (DriftFlg[PlayerId] == -1) {
        return;
    }

    if (ZureData[PlayerId] < -1.0f) {
        return;
    }

    if (ZureData[PlayerId] > 1.0f) {
        return;
    }

    // Exclude heavyweights
    if (Player->Kart == WARIO) {
        return;
    }

    if (Player->Kart == BOWSER) {
        return;
    }

    if (Player->Kart == DK) {
        return;
    }

    // Skip if Player has Star
    if (Player->SlipFlag & STAR) {
        return;
    }

    currPathPoint = NearestPathPointByPlayerId[PlayerId];
    temp_f22 = (Player->Speed / 18.0f) * 216.0f;
    for (i = 0; i < 8; i++) {
        sp9C[i] = -1;
        spB0[i] = 0x03E8;
    }

    if (AvoidCount[PlayerId] > 0) {
        AvoidCount[PlayerId]--;
        if (AvoidCount[PlayerId] <= 0) {
            AvoidMode[PlayerId] = 0;
        }
    }
    j = 0;
    i = 0;
    while (i < 8) {
        if (i != PlayerId) {
            Player = &GamePlayers[i];
            if ((Player->Flag & EXISTS)) {
                temp_v1_2 = NearestPathPointByPlayerId[i];
                temp_f0_2 = (Player->Speed / 18.0f) * 216.0f;
                temp_f2 = temp_f22 - 5.0f;
                if (temp_f0_2 < temp_f2) {
                    if (ExistAreaChk(temp_v1_2, currPathPoint, 0, 0x0014U, CenterLineMax) > 0) {
                        temp_a1_2 = temp_v1_2 - currPathPoint;
                        sp9C[j] = i;
                        if (temp_a1_2 > 0) {
                            spB0[j] = temp_a1_2;
                        } else {
                            spB0[j] = (temp_v1_2 + CenterLineMax) - currPathPoint;
                        }
                        sp74[j] = temp_f2 - temp_f0_2;
                        j++;
                    }

                }
            }
        }
        i++;
        if (j >= 2) {
            break;
        }
    }

    if (j == 0) {
        return;
    }

    var_f18 = 1.0f;
    var_f20 = -1.0f;
    for (i = 0; i < j; i++) {
        temp_f2 = ZureData[sp9C[i]];
        if ((temp_f2 > (-1.0f)) && (temp_f2 < 1.0f)) {

            temp_f12 = temp_ft2 = ((0.2f * (20.0f / (spB0[i] + 20.0f))) * ((sp74[i]) + 10.0f))  / 20.0f;

            if ((var_f18 == 1.0f) && (var_f20 == (-1.0f))) {
                var_f18 = temp_f2 - temp_f12;
                var_f20 = temp_f2 + temp_f12;
            } else {
                temp_f14 = temp_f2 - temp_f12;
                temp_f16 = temp_f2 + temp_f12;
                if ((temp_f14 < var_f18) && (temp_f16 > var_f18)) {
                    var_f18 = temp_f14;
                }
                if ((temp_f16 > var_f20) && (temp_f14 < var_f20)) {
                    var_f20 = temp_f16;
                }
            }
        }
    }

    if (var_f20 < var_f18) {
        return;
    }

    if (ZureData[PlayerId] < var_f18) {
        return;
    }

    if (var_f20 < ZureData[PlayerId]) {
        return;
    }

    if (var_f20 > 1.0f) {
        var_f20 = 1.0f;
    }
    if (var_f18 < (-1.0f)) {
        var_f18 = -1.0f;
    }
    if ((var_f18 + 1.0f) < (1.0f - var_f20)) {
        AvoidCount[PlayerId] = 0x003C;
        AvoidMode[PlayerId] = 1;
        ZureDist[PlayerId] = var_f20;
    } else {
        AvoidCount[PlayerId] = 0x003C;
        AvoidMode[PlayerId] = 2;
        ZureDist[PlayerId] = var_f18;
    }
}

// utils track Position

#include "cpu_vehicles_camera_path/path_utils.inc.c"

// bomb_kart utils

#include "cpu_vehicles_camera_path/bomb_kart.inc.c"

// actor utils

#include "cpu_vehicles_camera_path/actor_utils.inc.c"

void InitEnemyGameMode(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        GakakuMode[i] = 0;
        KoteiGakakuMode[i] = 0;
    }
}

void SetRivalCarName(void) {
    s32 shouldContinue;
    s32 i, j;

    for (j = 0; j < 2; j++) {
        EnemyRivalName[j] = 0;
    }

    if (DemoFlag == 1) {
        return;
    }
    if (GameMode != GRAND_PRIX) {
        return;
    }

    for (i = 0; i < 2; i++) {

        while (1) {
            EnemyRivalName[i] = MakeRandomLimmit(NUM_PLAYERS);

            if (MenuMultiplayerSelection > 2) {
                break;
            }
            if (MenuMultiplayerSelection < 1) {
                break;
            }

            shouldContinue = false;

            for (j = 0; j < MenuMultiplayerSelection; j++) {
                if (CharacterSelections[j] == EnemyRivalName[i]) {
                    shouldContinue = true;
                }
            }
            for (j = 0; j < i; j++) {
                if (EnemyRivalName[j] == EnemyRivalName[i]) {
                    shouldContinue = true;
                }
            }
            if (shouldContinue == false) {
                break;
            }
        }
    }
}

// Delete track path
void ClearPointData(CenterPathStruct* arg0, size_t Size) {
    bzero((void*) arg0, Size * sizeof(CenterPathStruct));
}

// Appears to allocate memory for each course.
void InitCenterLine(void) {

    struct _struct_gCoursePathSizes_0x10* ptr = &GET_COURSE_PathSizes;
    s32 temp;
    s32 i;

    MaxPathPoints[0] = (s32) ptr->FirstPath;
    MaxPathPoints[1] = (s32) ptr->SecondPath;
    MaxPathPoints[2] = (s32) ptr->ThirdPath;
    MaxPathPoints[3] = (s32) ptr->FourthPath;

    temp = ptr->Unk8;
    CenterLineEX = AllocateMemory(temp * 4);

    // Podium ceremony appears to allocate 1 * 8 bytes of data. Which would be aligned to 0x10.
    for (i = 0; i < 4; i++) {
        CenterPathBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(CenterPathStruct));
        SideLPathBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(CenterPathStruct));
        SideRPathBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(CenterPathStruct));
        CurvePathDataBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(s16));
        AnglePathDataBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(s16));
        ShortCutPathDataBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(s16));
    }

    PathPointPointer = CenterPathBP[0];
    SidePointL = SideLPathBP[0];
    SidePointR = SideRPathBP[0];
    CurveData = CurvePathDataBP[0];
    AngleData = AnglePathDataBP[0];
    ShortCutData = ShortCutPathDataBP[0];

    // zero allocated memory?
    //! @warning does not appear to zero all the above allocated variables.
    for (i = 0; i < 4; i++) {
        ClearPointData(CenterPathBP[i], MaxPathPoints[i]);
        ClearPointData(SideLPathBP[i], MaxPathPoints[i]);
        ClearPointData(SideRPathBP[i], MaxPathPoints[i]);
    }

    // Skip several cpu cycles.
    for (i = 0; i < 4; i++) {}

    for (i = 0; i < 4; i++) {
        if (MaxPathPoints[i] >= 2) {
            AreaBunkatuBP(i);
            SidePointCalcBP(i);
            CurveDataCalcBP(i);
            AngleDataCalcBP(i);
            ShortcutDataCalcBP(i);
        }
    }

    CenterLineMax = *CourseTotalPathPoints;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_KALAMARI_DESERT:
            AreaBunkatuSL();
            InitSL();
            break;
        case COURSE_DK_JUNGLE:
            AreaBunkatuSHIP();
            InitSHIP();
            break;
        case COURSE_TOADS_TURNPIKE:
            InitTRUCK();
            InitBUS();
            InitTANK();
            InitRV();
            break;
    }
#else

#endif
    InitBOM();
    KameHibasiraInit();
}

void InitEnemyKart(void) {

    UNUSED Camera* Camera;
    s32 temp_v0_3;
    s32 i;
    SOUKOU_ZURE_TYPE* var_s5;
    UNUSED s32 temp_v1;
    UNUSED s32 pad;

    for (i = 0; i < NUM_PLAYERS; i++) {
        Player* Player = &PlayerOne[i];

        OldEosAngleOga[i] = 0;
        AvoidMode[i] = 0;
        AvoidCount[i] = 0;
        if (CourseID < (NUM_COURSES - 1)) {
            ZureChkBP(i, 0, 0);
        }
        Speedlevel[i] = GET_COURSE_cpu_CurveTargetSpeed(RaceClass);
        DriftFlg[i] = 0;
        EnemyTimeCount[i] = 0;
        EnemyObjectX[i] = 0.0f;
        EnemyObjectZ[i] = 0.0f;
        OldLimmitspeed[i] = 0.0f;
        LapCountByPlayerId[i] = -1;
        RapCountF[i] = 0.0f;
        TimePlayerLastTouchedFinishLine[i] = 0.0f;
        if (GameMode == GRAND_PRIX) {
            if (1) {};
            if (1) {}; // Maybe some debug Code?
            GPCurrentRaceRankByPlayerId[i] = (s32) Kartrank[i];
            OldOgawaRank[i] = (s32) Kartrank[i];
        } else {
            GPCurrentRaceRankByPlayerId[i] = i;
            OldOgawaRank[i] = i;
        }
        temp_v0_3 = GPCurrentRaceRankByPlayerId[i];
        RankToNum[temp_v0_3] = (s16) i;
        RealRankToNum[temp_v0_3] = (s16) i;
        RealRank[i] = temp_v0_3;
        OgaReverseCount[i] = 0;
        WrongwayFlagPlayer1[i] = 0;
        TenukiFlg[i] = false;
        OldTenukiFlg[i] = 0;
        PlayerPathPointTotalTable[i] = -20;
        OldPylonCount[i] = -20;
        NameToNum[GamePlayers[i].Kart] = (s16) i;
        ZureData[i] = 0.0f;
        ZureDist[i] = 0.0f;
        var_s5 = &LaneData[i];
        var_s5->LaneFreeDrive = GET_COURSE_AIMinimumSeparation * (f32) (((i + 1) % 3) - 1);
        var_s5->LaneTarget = var_s5->LaneFreeDrive;
        var_s5->Lane = 0.0f;
        var_s5->Speed = 0.015f;
        ActionFlgSleep(i);
        EnemyAccelMode[i] = 0;
        EnemyEnterMaze[i] = 0;
        EnemyVoiceCountSPIN[i] = 0;
        EnemyVoiceCountDAMAGE[i] = 0;
        EnemyVoiceCountOVERTAKE[i] = 0;
        NoSimpleKartFlag[i] = 0;
        LakituSurface[i] = 0;
        OgaOldRapChkZ[i] = Player->Position[2];
        MokuhyouAddArea[i] = 6;
        if (GamePlayers[i].Flag & IS_PLAYER) {
            OgaCarStat[i] = 3;

        } else {
            OgaCarStat[i] = 0;
        }

        EnemyEnterTrg[i] = 0;
        EnemyExitTrg[i] = 0;
        EnemyDist[i] = -1;
        EnemyLimdist[i] = -1;
        EnemyGetaRank[i] = -1;
        RivalOvertakeAllowFlag[i] = 0;
        FumikiriStopFlg[i] = 0;
        StraightFlg[i] = true;
    }

#ifdef AVOID_UB
    for (i = 0; i < NUM_CROSSINGS; i++) {
        FumikiriCounter[i] = 0;
    }
#else
    FumikiriCounter[0] = 0;
    FumikiriCounter[1] = 0;
#endif
    if (DemoFlag == DEMO_MODE_INACTIVE) {

        if (GameMode == GRAND_PRIX) {
            for (i = 0; i < 2; i++) {
                EnemyRivalNum[i] = NameToNum[EnemyRivalName[i]];
                OgaCarStat[EnemyRivalNum[i]] = 1;
                EnemyRivalAimRank[EnemyRivalNum[i]] = i;
            }
        }
    }
    if ((DemoPlayFlag == 1) && (CourseID != COURSE_AWARD_CEREMONY)) {
        for (i = 0; i < NUM_PLAYERS; i++) {
            OgaCarStat[i] = 0;
        }
        if (GameMode == VERSUS) {

            EnemyRivalNum[0] = Cameras->Watch;
            OgaCarStat[EnemyRivalNum[0]] = 1;
            EnemyRivalAimRank[EnemyRivalNum[0]] = 0;

            for (i = 1; i < 2; i++) {
                EnemyRivalNum[i] = Cameras[i].Watch;
                OgaCarStat[EnemyRivalNum[i]] = 1;
                EnemyRivalAimRank[EnemyRivalNum[i]] = i;
            }
        }
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        if (1) {};
        CurrentPathID[i] = 0;
        NearestPathPointByPlayerId[i] =
            CourseTotalPathPoints[CurrentPathID[i]] - GPCurrentRaceRankByPlayerId[i] - 4;
    }

    HighwayGyakusouFlg = false;
    if (RaceClass == CC_EXTRA) {
        HighwayGyakusouFlg = true;
    }

    for (i = 0; i < 30; i++) {
        PylonNum[i] = -1;
        PylonNum2[i] = -1;
    }

    BestTime = 100000; // 100,000
    OgaCameraBirdX = 0.0f;
    OgaCameraBirdY = 100.0f;
    OgaCameraBirdZ = 0.0f;
    EnemyFreeItemDebugFlg = 0;
    EnemyFreeItemDebugInum = 1;
    EnemyFreeItemDebugNum = 1;
    EnemyTargetPlayer = PLAYER_ONE;
    OgaFrameCounter = 0;
    OgaEtcFrameCounter = 0;
    FinishLineZ = (f32) CenterPathBP[0][0].Pointz; // [i][2]
    OgaCalcLoopCount = 0;
    OldOgaCalcLoopCount = 0;
    OgaBumpChkCounter = 0;
    RankUpdateFinishFlag = 0;
    OgawaKarakaraShortCutOKFlg = 0;
    EnemyItemInit();
    if (DemoPlayFlag == 1) {
        if (DemoFlag == 1) {

            for (i = 0; i < NUM_PLAYERS; i++) {
                if (OgaCarStat[i] == 1) {
                    GamePlayers[i].Weapon |= USE_STARTDASH;
                }
            }
        }
    }
    InitActDatTable();
}

#include "cpu_vehicles_camera_path/path_calc.inc.c"

#include "cpu_vehicles_camera_path/behaviour_utils.inc.c"

#include "cpu_vehicles_camera_path/vehicle_utils.inc.c"

void CamCheckBump(s32 CameraId, s32 PathIndex) {
    s16 cameraPathPoint;
    CenterPathStruct* temp_v0;

    cameraPathPoint = OgaCamArea[CameraId];
    temp_v0 = &CenterPathBP[PathIndex][cameraPathPoint];
    CheckBump(&Cameras[CameraId].Bump, 10.0f, (f32) temp_v0->Pointx, (f32) temp_v0->Pointy + 30.0f,
                             (f32) temp_v0->Pointz);
}

void OgawaCameraInit(s32 cameraIndex) {
    s32 CameraId;

    D_801646CC = 0;
    KoteiGakakuMode[cameraIndex] = GakakuMode[cameraIndex];
    if ((GameMode != 1) && ((CourseSelect == COURSE_ONE) || (DemoFlag == (u16) 1))) {
        KoteiGakakuMode[cameraIndex] = 0;
    } else if ((KoteiGakakuMode[cameraIndex] != 0) && (KoteiGakakuMode[cameraIndex] != (s16) 1) &&
               (KoteiGakakuMode[cameraIndex] != 2) && (KoteiGakakuMode[cameraIndex] != 3)) {
        KoteiGakakuMode[cameraIndex] = 0;
    }
    OgawaCameraMode[cameraIndex] = -1;
    OgaCameraWatchNum = 0;
    KatamukiTheta[cameraIndex] = 0;
    OgaGoalCounter = 0;
    OgaSoundCamera[cameraIndex].Unk0 = 0;
    OgaSoundCamera[cameraIndex].Unk2 = 0;
    OgaSoundCamera[cameraIndex].unk4 = 0;
    if ((GameMode == 1) && (InGame == 0)) {
        KoteiGakakuMode[cameraIndex] = 0;
    }

    for (CameraId = 0; CameraId < 4; CameraId++) {
        OgaCamArea[CameraId] = 0;
    }
}

f32 OgawaGakaku(f32 arg0, s32 arg1) {
    f32 temp_f0;
    f64 temp_f2;

    temp_f0 = CamOffset[arg1];
    switch (KoteiGakakuMode[arg1]) { /* irregular */
        default:
            arg0 = 40.0f;
            break;
        case 0:
            temp_f2 = 40.0;
            temp_f2 += temp_f0;
            if (temp_f2 < arg0) {
                arg0 -= 1.0;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                }
            }
            if (arg0 < temp_f2) {
                arg0 += 1.0;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                    ;
                }
            }
            break;
        case 1:
            temp_f2 = 60.0;
            temp_f2 += temp_f0;
            if (arg0 < temp_f2) {
                arg0 += 1.0;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                }
            }
            if (temp_f2 < arg0) {
                arg0 -= 1.0;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                    ;
                }
            }
            break;
        case 3:
            temp_f2 = 60.0;
            temp_f2 += temp_f0;
            if (arg0 < temp_f2) {
                arg0 += 0.5;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                }
            }
            if (temp_f2 < arg0) {
                arg0 -= 0.5;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                }
            }
            break;
        case 2:
            temp_f2 = 60.0;
            temp_f2 += temp_f0;
            if (arg0 < temp_f2) {
                arg0 += 1.0;
                if (temp_f2 < arg0) {
                    arg0 = temp_f2;
                }
            }
            if (temp_f2 < arg0) {
                arg0 -= 1.0;
                if (arg0 < temp_f2) {
                    arg0 = temp_f2;
                }
            }
            break;
    }
    return arg0;
}

void OgaCameraRotate(Camera* Camera, s32 cameraIndex) {
    f32 xnorm;
    f32 ynorm;
    f32 znorm;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 Distance;
    f32 sp28;
    u16 thing;

    thing = KatamukiTheta[cameraIndex];
    if (thing == 0) {
        Camera->UpVector[0] = 0.0f;
        Camera->UpVector[2] = 0.0f;
        Camera->UpVector[1] = 1.0f;
    } else {
        xdiff = Camera->LookatPos[0] - Camera->CameraPos[0];
        ydiff = Camera->LookatPos[1] - Camera->CameraPos[1];
        zdiff = Camera->LookatPos[2] - Camera->CameraPos[2];
        Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
        xnorm = xdiff / Distance;
        ynorm = ydiff / Distance;
        znorm = zdiff / Distance;
        sp28 = 1.0 - CosT(thing);
        Camera->UpVector[0] = (sp28 * xnorm * ynorm) - (SinT(thing) * znorm);
        Camera->UpVector[1] = CosT(thing) + (sp28 * ynorm * ynorm);
        Camera->UpVector[2] = (SinT(thing) * xnorm) + (sp28 * ynorm * znorm);
    }
}

UNUSED void func_8001530C(void) {
}

void OgawaVideoInit(s32 PlayerId, UNUSED f32 arg1, s32 CameraId) {
    Camera* temp_a0;
    Player* temp_a1;

    // wtf is up with the Pointer accesses here?
    // What aren't they just doing thing = &some_pointer[some_index]?
    temp_a1 = PlayerOne;
    temp_a0 = Camera1;
    temp_a1 += PlayerId;
    temp_a0 += CameraId;
    temp_a0->ChaseDirection = temp_a1->Direction[1];
    OgawaVideoview(temp_a0, temp_a1, 0);
}

void OgawaVideoview(Camera* Camera, UNUSED Player* player, UNUSED s32 Arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    Player* temp_s1;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED Vec3f Pad2;
    Vec3f sp64;
    UNUSED s32 Pad3[9];
    s16 var_a2;

    /*
    Doing the more sensible:
    temp_s1 = &PlayerOne[Camera->Watch];
    leads to some regalloc differences
    */
    temp_s1 = PlayerOne;
    temp_s1 += Camera->Watch;
    if (temp_s1->Sterrangle == 0) {
        var_a2 = 0x0064;
    } else if (temp_s1->Sterrangle < 0) {
        var_a2 = 0xA0 - (temp_s1->Sterrangle / 16);
    } else {
        var_a2 = 0xA0 + (temp_s1->Sterrangle / 16);
    }
    if (!((temp_s1->SlipFlag & SPIN_L) || (temp_s1->SlipFlag & SPIN_R))) {
        ChaseDir(&Camera->ChaseDirection, temp_s1->Direction[1], var_a2);
    }
    DirectionFixcamera(temp_s1, Camera, sp64, &sp84, &sp80, &sp7C, Camera->ChaseDirection);
    CheckBump(&Camera->Bump, 10.0f, sp84, sp80, sp7C);
    Camera->LookatPos[0] = sp64[0];
    Camera->LookatPos[1] = sp64[1];
    Camera->LookatPos[2] = sp64[2];
    Camera->CameraPos[0] = sp84;
    Camera->CameraPos[1] = sp80;
    Camera->CameraPos[2] = sp7C;
    temp_f12 = Camera->LookatPos[0] - Camera->CameraPos[0];
    sp90 = Camera->LookatPos[1] - Camera->CameraPos[1];
    temp_f14 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(temp_f12, temp_f14);
    Camera->CameraDirection[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    Camera->CameraDirection[2] = 0;
}

void OgawaKoteiInit(s32 PlayerId, f32 arg1, s32 CameraId, s32 PathIndex) {
    Camera* Camera;

    f32 temp_f12;
    f32 temp_f2;
    s32 test = CourseTotalPathPoints[PathIndex];

    OgaCameraZure[CameraId] = arg1;
    Camera = Cameras + CameraId;
    OgaCamArea[CameraId] = (NearestPathPointByPlayerId[PlayerId] + 10) % test;

    ObjCalcFromZureBP(OgaCamArea[CameraId], arg1, PathIndex);

    OldOgaCamX[CameraId] = object[0];
    OldOgaCamZ[CameraId] = object[2];

    temp_f2 = (f32) CenterPathBP[PathIndex][OgaCamArea[CameraId]].Pointy;

    temp_f12 = CheckHight(object[0], (f32) (temp_f2 + 30.0), object[2]);
    if ((temp_f12 < (temp_f2 - 20.0)) || (temp_f12 >= 3000.0)) {
        OldOgaCamY[CameraId] = (f32) (temp_f2 + 10.0);
    } else {
        OldOgaCamY[CameraId] = (f32) (temp_f12 + 10.0);
    }
    CamSpeed[CameraId] = 0.0f;
    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaKoteiview(Camera* Camera, UNUSED Player* playerArg, UNUSED s32 Arg2, s32 CameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 pad;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED s32 Pad2;
    s32 PlayerId;
    UNUSED s32 Pad3[9];
    Player* Player;
    s32 pathPointDiff;
    s32 PathIndex;

    PlayerId = Camera->Watch;
    PathIndex = CurrentPathID[PlayerId];
    Player = PlayerOne;
    Player += PlayerId;
    OgaCamArea[CameraId] =
        GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
    playerPathPoint = NearestPathPointByPlayerId[PlayerId];
    cameraPathPoint = OgaCamArea[CameraId];
    if (ExistAreaChk(playerPathPoint, cameraPathPoint, 0x0032U, 0x000FU, CourseTotalPathPoints[PathIndex]) <=
        0) {
        OgaDemoCameraReInit(CameraId, OgaCameraZure[CameraId], OgawaCameraMode[CameraId]);
    } else {
        if (ZureData[PlayerId] < (-0.7)) {
            pathPointDiff = playerPathPoint - cameraPathPoint;
            if ((OgaCameraZure[CameraId] < (-0.5)) && ((pathPointDiff * pathPointDiff) < 5)) {
                OgaDemoCameraReInit(CameraId, 1.0f, 3);
                goto alable;
            }
        }
        // clang-format off
        // I hate this, but a fakematch is a fakematch
        if (ZureData[PlayerId] > 0.7) { pathPointDiff = playerPathPoint - cameraPathPoint; if ((OgaCameraZure[CameraId] > 0.5) && ((pathPointDiff * pathPointDiff) < 5)) {
                OgaDemoCameraReInit(CameraId, -1.0f, 2);
                // clang-format on
            }
        }
    }
alable:
    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
    Camera->LookatPos[0] = Player->Position[0];
    Camera->LookatPos[1] = Player->Position[1] + 6.0;
    Camera->LookatPos[2] = Player->Position[2];
    CamCheckBump(CameraId, PathIndex);
    xdiff = Camera->LookatPos[0] - Camera->CameraPos[0];
    ydiff = Camera->LookatPos[1] - Camera->CameraPos[1];
    zdiff = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(xdiff, zdiff);
    Camera->CameraDirection[0] = Atan2t(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    Camera->CameraDirection[2] = 0;
}

void OgawaOikakeInit(s32 PlayerId, f32 arg1, s32 CameraId, s16 PathIndex) {
    Camera* Camera = Cameras + CameraId;

    OgaCameraZure[CameraId] = arg1;
    OgaCamArea[CameraId] = NearestPathPointByPlayerId[PlayerId] + 10;
    OgaCamArea[CameraId] = (OgaCamArea[CameraId]) % CourseTotalPathPoints[PathIndex];

    ObjCalcFromZureBP(OgaCamArea[CameraId], arg1, PathIndex);

    OldOgaCamX[CameraId] = object[0];
    OldOgaCamY[CameraId] = (f32) CenterPathBP[PathIndex][OgaCamArea[CameraId]].Pointy;
    OldOgaCamZ[CameraId] = object[2];

    CamSpeed[CameraId] = GamePlayers[PlayerId].Speed / 5.0f;
    if ((f64) CamSpeed[CameraId] < 0.0) {
        CamSpeed[CameraId] = 0.0f;
    }

    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaOikakeview(Camera* Camera, UNUSED Player* unusedPlayer, UNUSED s32 Arg2, s32 CameraId) {
    s16 playerPathPoint;
    s16 cameraPathPoint;
    UNUSED s32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* Player;
    s32 PlayerId;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 Distance;
    UNUSED s32 stackPadding1;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 var_f18;
    UNUSED f32 var_f20;
    f32 temp_f2_2;
    s32 PathIndex;

    PlayerId = Camera->Watch;
    Player = PlayerOne;
    Player += PlayerId;
    OgaCameraWatchNum = PlayerId;
    PathIndex = CurrentPathID[PlayerId];
    OgaCamArea[CameraId] =
        GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
    playerPathPoint = NearestPathPointByPlayerId[PlayerId];
    cameraPathPoint = OgaCamArea[CameraId];
    if (ExistAreaChk(playerPathPoint, cameraPathPoint, 0x0032U, 0x000FU, CourseTotalPathPoints[PathIndex]) <=
        0) {
        OgaDemoCameraReInit(CameraId, OgaCameraZure[CameraId], OgawaCameraMode[CameraId]);
    } else if ((ZureData[PlayerId] < -0.5) && (OgaCameraZure[CameraId] < -0.5)) {
        OgaDemoCameraReInit(CameraId, 1.0f, 7);
    } else if ((ZureData[PlayerId] > 0.5) && (OgaCameraZure[CameraId] > 0.5)) {
        OgaDemoCameraReInit(CameraId, -1.0f, 6);
    }
    pathPoint1 = (OgaCamArea[CameraId] + 1) % CourseTotalPathPoints[PathIndex];
    pathPoint2 = (OgaCamArea[CameraId] + 2) % CourseTotalPathPoints[PathIndex];
    ObjCalcFromZureBP(pathPoint1, OgaCameraZure[CameraId], PathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    ObjCalcFromZureBP(pathPoint2, OgaCameraZure[CameraId], PathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[PathIndex][pathPoint1].Pointy + CenterPathBP[PathIndex][pathPoint2].Pointy) / 2.0;
    xdiff = midX - OldOgaCamX[CameraId];
    ydiff = midY - OldOgaCamY[CameraId];
    zdiff = midZ - OldOgaCamZ[CameraId];
    Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (Distance != 0.0) {
        midX = OldOgaCamX[CameraId] + ((CamSpeed[CameraId] * xdiff) / Distance);
        midY = OldOgaCamY[CameraId] + ((CamSpeed[CameraId] * ydiff) / Distance);
        midZ = OldOgaCamZ[CameraId] + ((CamSpeed[CameraId] * zdiff) / Distance);
    } else {
        midX = OldOgaCamX[CameraId];
        midY = OldOgaCamY[CameraId];
        midZ = OldOgaCamZ[CameraId];
    }
    Camera->CameraPos[0] = midX;
    Camera->CameraPos[2] = midZ;
    temp_f2_2 = CheckHight(midX, midY + 30.0, midZ);
    if ((temp_f2_2 < (midY - 20.0)) || (temp_f2_2 >= 3000.0)) {
        Camera->CameraPos[1] = midY + 10.0;
    } else {
        Camera->CameraPos[1] = temp_f2_2 + 8.0;
    }
    OldOgaCamX[CameraId] = midX;
    OldOgaCamY[CameraId] = midY;
    OldOgaCamZ[CameraId] = midZ;
    Camera->LookatPos[0] = Player->Position[0];
    Camera->LookatPos[1] = Player->Position[1] + 6.0;
    Camera->LookatPos[2] = Player->Position[2];
    CamCheckBump(CameraId, PathIndex);
    xdiff2 = Camera->LookatPos[0] - Camera->CameraPos[0];
    ydiff2 = Camera->LookatPos[1] - Camera->CameraPos[1];
    zdiff2 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(xdiff2, zdiff2);
    Camera->CameraDirection[0] = Atan2t(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    Camera->CameraDirection[2] = 0;
}

void OgawaSideInit(s32 PlayerId, f32 arg1, s32 CameraId, s16 PathIndex) {
    Camera* Camera = Cameras + CameraId;

    OgaCameraZure[CameraId] = arg1;
    OgaCamArea[CameraId] = NearestPathPointByPlayerId[PlayerId];
    OgaCamArea[CameraId] = (OgaCamArea[CameraId]) % CourseTotalPathPoints[PathIndex];

    ObjCalcFromZureBP(OgaCamArea[CameraId], arg1, PathIndex);

    OldOgaCamX[CameraId] = object[0];
    OldOgaCamY[CameraId] = (f32) CenterPathBP[PathIndex][OgaCamArea[CameraId]].Pointy;
    OldOgaCamZ[CameraId] = object[2];

    CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed;
    CamSpeed[CameraId] = GamePlayers[PlayerId].Speed;

    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaSideview(Camera* Camera, UNUSED Player* unusedPlayer, UNUSED s32 Arg2, s32 CameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* Player;
    s32 PlayerId;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 Distance;
    UNUSED f32 sp98;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 sp94;
    UNUSED f32 sp90;
    UNUSED s32 stackPadding1;
    s32 PathIndex;
    f32 temp_f2_5;

    PlayerId = Camera->Watch;
    Player = PlayerOne;
    CamSpeed[CameraId] += ((CamSpeedObjct[CameraId] - CamSpeed[CameraId]) * 0.5f);
    OgaCameraWatchNum = PlayerId;
    PathIndex = CurrentPathID[PlayerId];
    Player += PlayerId;
    OgaCamArea[CameraId] =
        GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
    temp_f2_5 = (ZureData[PlayerId] - OgaCameraZure[CameraId]);
    temp_f2_5 *= temp_f2_5;
    playerPathPoint = NearestPathPointByPlayerId[PlayerId];
    cameraPathPoint = OgaCamArea[CameraId];
    if (ExistAreaChk(playerPathPoint, cameraPathPoint, 0x000FU, 0x000FU, CourseTotalPathPoints[PathIndex]) <=
        0) {
        OgaDemoCameraReInit(CameraId, OgaCameraZure[CameraId], OgawaCameraMode[CameraId]);
    } else {
        if ((ZureData[PlayerId] < 0.0) && (OgaCameraZure[CameraId] < 0.0) && (temp_f2_5 < 0.01)) {
            OgaDemoCameraReInit(CameraId, 1.0f, 5);
        } else {
            if ((ZureData[PlayerId] > 0.0) && (OgaCameraZure[CameraId] > 0.0) && (temp_f2_5 < 0.01)) {
                OgaDemoCameraReInit(CameraId, -1.0f, 4);
            } else {
                if ((cameraPathPoint < playerPathPoint) && ((playerPathPoint - cameraPathPoint) < 0xA)) {
                    CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed + 0.4;
                }
                if ((playerPathPoint < cameraPathPoint) && ((cameraPathPoint - playerPathPoint) < 0xA)) {
                    CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed - 0.4;
                }
                if (CamSpeedObjct[CameraId] > 10.0) {
                    CamSpeedObjct[CameraId] = 10.0f;
                }
                if (CamSpeedObjct[CameraId] < 0.0) {
                    CamSpeedObjct[CameraId] = 0.0f;
                }
            }
        }
    }
    pathPoint1 = (OgaCamArea[CameraId] + 1) % CourseTotalPathPoints[PathIndex];
    pathPoint2 = (OgaCamArea[CameraId] + 2) % CourseTotalPathPoints[PathIndex];
    ObjCalcFromZureBP(pathPoint1, OgaCameraZure[CameraId], PathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    ObjCalcFromZureBP(pathPoint2, OgaCameraZure[CameraId], PathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[PathIndex][pathPoint1].Pointy + CenterPathBP[PathIndex][pathPoint2].Pointy) / 2.0;
    xdiff = midX - OldOgaCamX[CameraId];
    ydiff = midY - OldOgaCamY[CameraId];
    zdiff = midZ - OldOgaCamZ[CameraId];
    Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (Distance != 0.0) {
        midX = OldOgaCamX[CameraId] + ((CamSpeed[CameraId] * xdiff) / Distance);
        midY = OldOgaCamY[CameraId] + ((CamSpeed[CameraId] * ydiff) / Distance);
        midZ = OldOgaCamZ[CameraId] + ((CamSpeed[CameraId] * zdiff) / Distance);
    } else {
        midX = OldOgaCamX[CameraId];
        midY = OldOgaCamY[CameraId];
        midZ = OldOgaCamZ[CameraId];
    }
    Camera->CameraPos[0] = midX;
    Camera->CameraPos[2] = midZ;
    temp_f2_5 = CheckHight(midX, midY + 30.0, midZ);
    if ((temp_f2_5 < (midY - 20.0)) || (temp_f2_5 >= 3000.0)) {
        Camera->CameraPos[1] = midY + 10.0;
    } else {
        Camera->CameraPos[1] = temp_f2_5 + 10.0;
    }
    OldOgaCamX[CameraId] = midX;
    OldOgaCamY[CameraId] = midY;
    OldOgaCamZ[CameraId] = midZ;
    Camera->LookatPos[0] = Player->Position[0];
    Camera->LookatPos[1] = Player->Position[1] + 6.0;
    Camera->LookatPos[2] = Player->Position[2];
    CamCheckBump(CameraId, PathIndex);
    xdiff2 = Camera->LookatPos[0] - Camera->CameraPos[0];
    ydiff2 = Camera->LookatPos[1] - Camera->CameraPos[1];
    zdiff2 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(xdiff2, zdiff2);
    Camera->CameraDirection[0] = Atan2t(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    Camera->CameraDirection[2] = 0;
}

void OgawaQuickInit(UNUSED s32 PlayerId, UNUSED f32 arg1, s32 CameraId) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    f32 temp_f2;
    UNUSED s32 stackPadding2;
    f32 temp_f12;
    CenterPathStruct** path;
    f32 sp54;
    s32 temp_s0;
    s16 sp48;
    s16 sp44;
    Camera* Camera;

    if (MakeRandomLimmit(0x0064U) < 0x32) {
        OgaCameraZure[CameraId] = 0.1f;
    } else {
        OgaCameraZure[CameraId] = -0.1f;
    }
    D_80163DD8[CameraId] = 0;
    if (CourseID == COURSE_YOSHI_VALLEY) {
        D_80163DD8[CameraId] = MakeRandomLimmit(4U);
        OgaCameraZure[CameraId] = 0.0f;
    }
    temp_s0 = CourseTotalPathPoints[D_80163DD8[CameraId]];
    OgaCamArea[CameraId] %= temp_s0;
    ObjCalcFromZureBP(OgaCamArea[CameraId], OgaCameraZure[CameraId], 0);
    OldOgaCamX[CameraId] = object[0];
    OldOgaCamY[CameraId] = CenterPathBP[0][OgaCamArea[CameraId]].Pointy;
    path = CenterPathBP;
    OldOgaCamZ[CameraId] = object[2];
    CamSpeedObjct[CameraId] = 16.666666f;
    CamSpeed[CameraId] = 0.0f;
    sp48 = (OgaCamArea[CameraId] + 0xA) % temp_s0;
    sp44 = (OgaCamArea[CameraId] + 0xB) % temp_s0;
    ObjCalcFromZureBP(sp48, OgaCameraZure[CameraId], 0);
    temp_f2 = object[0] * 0.5;
    temp_f12 = object[2] * 0.5;
    ObjCalcFromZureBP(sp44, OgaCameraZure[CameraId], 0);
    temp_f2 += object[0] * 0.5;
    temp_f12 += object[2] * 0.5;
    sp48 = (OgaCamArea[CameraId] + 0x5) % temp_s0;
    sp44 = (OgaCamArea[CameraId] + 0x6) % temp_s0;
    sp54 = (path[0][sp48].Pointy + path[0][sp44].Pointy) * 0.5f;
    Camera = Cameras;
    Camera += CameraId;
    Camera->LookatPos[0] = temp_f2;
    Camera->LookatPos[2] = temp_f12;
    Camera->LookatPos[1] = sp54 + 8.0;
    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaQuickview(Camera* Camera, UNUSED Player* Player, UNUSED s32 Index, s32 CameraId);
void OgawaQuickview(Camera* Camera, UNUSED Player* Player, UNUSED s32 Index, s32 CameraId) {
    s32 stackPadding0;
    s32 stackPadding1;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    s32 stackPadding2;
    s32 stackPadding3;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    s32 stackPadding7;
    s32 stackPadding8;
    s16 sp6E;
    s16 sp6C;
    f32 stackPadding9;
    s32 PlayerId;
    f32 temp_f0;
    s32 PathIndex;
    s32 sp58;
    s16 sp56;

    PlayerId = Camera->Watch;
    CamSpeed[CameraId] += (CamSpeedObjct[CameraId] - CamSpeed[CameraId]) * 0.5f;
    PathIndex = D_80163DD8[CameraId];
    sp58 = CourseTotalPathPoints[PathIndex];
    OgaCameraWatchNum = PlayerId;
    sp56 = OgaCamArea[CameraId];
    OgaCamArea[CameraId] = GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
    if (CourseID == 4) {
        if ((sp56 != OgaCamArea[CameraId]) && (OgaCamArea[CameraId] == 1)) {
            D_80163DD8[CameraId] = MakeRandomLimmit(4);
            PathIndex = D_80163DD8[CameraId];
            OgaCamArea[CameraId] = GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
        }
    }
    sp6E = (OgaCamArea[CameraId] + 0xA) % sp58;
    sp6C = (OgaCamArea[CameraId] + 0xB) % sp58;
    ObjCalcFromZureBP(sp6E, OgaCameraZure[CameraId], PathIndex);
    sp8C = object[0] * 0.5;
    sp84 = object[2] * 0.5;
    ObjCalcFromZureBP(sp6C, OgaCameraZure[CameraId], PathIndex);
    sp8C += object[0] * 0.5;
    sp84 += object[2] * 0.5;

    sp6E = (OgaCamArea[CameraId] + 5) % sp58;
    sp6C = (OgaCamArea[CameraId] + 6) % sp58;
    sp88 = (CenterPathBP[PathIndex][sp6E].Pointy + CenterPathBP[PathIndex][sp6C].Pointy) * 0.5f;
    sp6E = (OgaCamArea[CameraId] + 1) % sp58;
    sp6C = (OgaCamArea[CameraId] + 2) % sp58;
    ObjCalcFromZureBP(sp6E, OgaCameraZure[CameraId], PathIndex);
    sp98 = object[0] * 0.5;
    sp90 = object[2] * 0.5;
    ObjCalcFromZureBP(sp6C, OgaCameraZure[CameraId], PathIndex);
    sp98 += object[0] * 0.5;
    sp90 += object[2] * 0.5;
    sp94 = (CenterPathBP[PathIndex][sp6E].Pointy + CenterPathBP[PathIndex][sp6C].Pointy) * 0.5f;

    sp80 = sp98 - OldOgaCamX[CameraId];
    sp7C = sp94 - OldOgaCamY[CameraId];
    sp78 = sp90 - OldOgaCamZ[CameraId];
    temp_f0 = sqrtf(((sp80 * sp80) + (sp7C * sp7C)) + (sp78 * sp78));

    if (temp_f0 != 0.0) {
        sp98 = OldOgaCamX[CameraId] + (((stackPadding9 = CamSpeed[CameraId]) * sp80) / temp_f0);
        sp94 = OldOgaCamY[CameraId] + ((CamSpeed[CameraId] * sp7C) / temp_f0);
        sp90 = OldOgaCamZ[CameraId] + ((CamSpeed[CameraId] * sp78) / temp_f0);
    } else {
        sp98 = OldOgaCamX[CameraId];
        sp94 = OldOgaCamY[CameraId];
        sp90 = OldOgaCamZ[CameraId];
    }

    if (sp98 < -10000.0 || sp98 > 10000.0) {
        if (sp8C < -10000.0 || sp8C > 10000.0) {}
    }
    Camera->CameraPos[0] = sp98;
    Camera->CameraPos[2] = sp90;
    Camera->CameraPos[1] = sp94 + 10.0;

    OldOgaCamX[CameraId] = sp98;
    OldOgaCamY[CameraId] = sp94;
    OldOgaCamZ[CameraId] = sp90;

    if (sp8C < -10000.0 || sp8C > 10000.0) {}
    if (sp84 < -10000.0 || sp84 > 10000.0) {}
    Camera->LookatPos[0] = sp8C;
    Camera->LookatPos[1] = sp88 + 8.0;
    Camera->LookatPos[2] = sp84;
    CamCheckBump(CameraId, PathIndex);
    spAC = Camera->LookatPos[0] - Camera->CameraPos[0];
    spA8 = Camera->LookatPos[1] - Camera->CameraPos[1];
    spA4 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(spAC, spA4);
    Camera->CameraDirection[0] = Atan2t(sqrtf((spAC * spAC) + (spA4 * spA4)), spA8);
    Camera->CameraDirection[2] = 0;
}

void OgawaFaceInit(s32 PlayerId, UNUSED f32 arg1, s32 CameraId, s16 PathIndex) {
    Camera* Camera = Cameras + CameraId;
    UNUSED s32 pad;

    OgaCameraZure[CameraId] = ZureData[PlayerId];
    OgaCamArea[CameraId] = NearestPathPointByPlayerId[PlayerId] + 3;
    OgaCamArea[CameraId] = OgaCamArea[CameraId] % CourseTotalPathPoints[PathIndex];

    ObjCalcFromZureBP(OgaCamArea[CameraId], ZureData[PlayerId], PathIndex);

    OldOgaCamX[CameraId] = object[0];
    OldOgaCamY[CameraId] = (f32) CenterPathBP[PathIndex][OgaCamArea[CameraId]].Pointy;
    OldOgaCamZ[CameraId] = object[2];

    CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed;
    CamSpeed[CameraId] = GamePlayers[PlayerId].Speed;

    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaFaceview(Camera* Camera, UNUSED Player* unusedPlayer, UNUSED s32 Arg2, s32 CameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED f32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* Player;
    f32 Distance;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED f32 var_f2;
    UNUSED f32 var_f18;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 stackPadding1;
    UNUSED f32 var_f14;
    s32 PlayerId;
    s32 PathIndex;
    s32 pathPointCount;

    PlayerId = Camera->Watch;
    Player = PlayerOne;
    OgaCameraZure[CameraId] = ZureData[PlayerId];
    CamSpeed[CameraId] += ((CamSpeedObjct[CameraId] - CamSpeed[CameraId]) / 2.0f);
    OgaCameraWatchNum = PlayerId;
    PathIndex = CurrentPathID[PlayerId];
    Player += PlayerId;
    pathPointCount = CourseTotalPathPoints[PathIndex];
    OgaCamArea[CameraId] =
        GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
    playerPathPoint = (NearestPathPointByPlayerId[PlayerId] + 3) % pathPointCount;
    cameraPathPoint = OgaCamArea[CameraId];
    if (ExistAreaChk(playerPathPoint, cameraPathPoint, 0x000DU, 1U, pathPointCount) <= 0) {
        OgaDemoCameraReInit(CameraId, OgaCameraZure[CameraId], OgawaCameraMode[CameraId]);
    } else {
        if ((cameraPathPoint < playerPathPoint) && ((playerPathPoint - cameraPathPoint) < 3)) {
            CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed + 0.1;
        }
        if ((playerPathPoint < cameraPathPoint) && ((cameraPathPoint - playerPathPoint) < 3)) {
            CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed - 0.1;
        }
        if (CamSpeedObjct[CameraId] > 10.0) {
            CamSpeedObjct[CameraId] = 10.0f;
        }
        if (CamSpeedObjct[CameraId] < 0.0) {
            CamSpeedObjct[CameraId] = 0.0f;
        }
    }
    pathPoint1 = (OgaCamArea[CameraId] + 1) % pathPointCount;
    pathPoint2 = (OgaCamArea[CameraId] + 2) % pathPointCount;
    ObjCalcFromZureBP(pathPoint1, OgaCameraZure[CameraId], PathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    ObjCalcFromZureBP(pathPoint2, OgaCameraZure[CameraId], PathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[PathIndex][pathPoint1].Pointy + CenterPathBP[PathIndex][pathPoint2].Pointy) / 2.0;
    xdiff = midX - OldOgaCamX[CameraId];
    ydiff = midY - OldOgaCamY[CameraId];
    zdiff = midZ - OldOgaCamZ[CameraId];
    Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (Distance != 0.0) {
        midX = OldOgaCamX[CameraId] + ((CamSpeed[CameraId] * xdiff) / Distance);
        midY = OldOgaCamY[CameraId] + ((CamSpeed[CameraId] * ydiff) / Distance);
        midZ = OldOgaCamZ[CameraId] + ((CamSpeed[CameraId] * zdiff) / Distance);
    } else {
        midX = OldOgaCamX[CameraId];
        midY = OldOgaCamY[CameraId];
        midZ = OldOgaCamZ[CameraId];
    }
    Camera->CameraPos[0] = midX;
    Camera->CameraPos[2] = midZ;
    Camera->CameraPos[1] = midY + 10.0;
    OldOgaCamX[CameraId] = midX;
    OldOgaCamY[CameraId] = midY;
    OldOgaCamZ[CameraId] = midZ;
    Camera->LookatPos[0] = Player->Position[0];
    Camera->LookatPos[1] = Player->Position[1] + 6.0;
    Camera->LookatPos[2] = Player->Position[2];
    CamCheckBump(CameraId, PathIndex);
    xdiff2 = Camera->LookatPos[0] - Camera->CameraPos[0];
    ydiff2 = Camera->LookatPos[1] - Camera->CameraPos[1];
    zdiff2 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(xdiff2, zdiff2);
    Camera->CameraDirection[0] = Atan2t(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    Camera->CameraDirection[2] = 0;
}

void OgawaBackInit(s32 PlayerId, UNUSED f32 arg1, s32 CameraId, s16 PathIndex) {
    Camera* Camera = Cameras + CameraId;
    s32 test = CourseTotalPathPoints[PathIndex];

    OgaCameraZure[CameraId] = ZureData[PlayerId];
    OgaCamArea[CameraId] = (NearestPathPointByPlayerId[PlayerId] + test) - 2;
    OgaCamArea[CameraId] = OgaCamArea[CameraId] % test;

    ObjCalcFromZureBP(OgaCamArea[CameraId], ZureData[PlayerId], PathIndex);

    OldOgaCamX[CameraId] = object[0];
    OldOgaCamY[CameraId] = (f32) CenterPathBP[PathIndex][OgaCamArea[CameraId]].Pointy;
    OldOgaCamZ[CameraId] = object[2];

    CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed;
    CamSpeed[CameraId] = GamePlayers[PlayerId].Speed;

    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaBackview(Camera* Camera, UNUSED Player* unusedPlayer, UNUSED s32 Arg2, s32 CameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 stackPadding3;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    UNUSED s32 stackPadding4;
    f32 Distance;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    Player* Player;
    s32 PlayerId;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 var_f2;
    UNUSED f32 var_f18;
    UNUSED f32 var_f14;
    s32 PathIndex;
    s32 pathPointCount;

    PlayerId = Camera->Watch;
    Player = PlayerOne;
    OgaCameraZure[CameraId] = ZureData[PlayerId];
    CamSpeed[CameraId] += ((CamSpeedObjct[CameraId] - CamSpeed[CameraId]) * 0.5f);
    OgaCameraWatchNum = PlayerId;
    PathIndex = CurrentPathID[PlayerId];
    pathPointCount = CourseTotalPathPoints[PathIndex];
    Player += PlayerId;
    OgaCamArea[CameraId] =
        GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
    playerPathPoint = ((NearestPathPointByPlayerId[PlayerId] + pathPointCount) - 2) % pathPointCount;
    cameraPathPoint = OgaCamArea[CameraId];
    if (ExistAreaChk(playerPathPoint, cameraPathPoint, 1U, 0x000AU, pathPointCount) <= 0) {
        OgaDemoCameraReInit(CameraId, OgaCameraZure[CameraId], OgawaCameraMode[CameraId]);
    } else {
        if ((cameraPathPoint < playerPathPoint) && ((playerPathPoint - cameraPathPoint) < 3)) {
            CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed + 0.1;
        }
        if ((playerPathPoint < cameraPathPoint) && ((cameraPathPoint - playerPathPoint) < 3)) {
            CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed - 0.1;
        }
        if (CamSpeedObjct[CameraId] > 10.0) {
            CamSpeedObjct[CameraId] = 10.0f;
        }
        if (CamSpeedObjct[CameraId] < 0.0) {
            CamSpeedObjct[CameraId] = 0.0f;
        }
    }
    pathPoint1 = (OgaCamArea[CameraId] + 1) % pathPointCount;
    pathPoint2 = (OgaCamArea[CameraId] + 2) % pathPointCount;
    ObjCalcFromZureBP(pathPoint1, OgaCameraZure[CameraId], PathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    ObjCalcFromZureBP(pathPoint2, OgaCameraZure[CameraId], PathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[PathIndex][pathPoint1].Pointy + CenterPathBP[PathIndex][pathPoint2].Pointy) / 2.0;
    xdiff = midX - OldOgaCamX[CameraId];
    ydiff = midY - OldOgaCamY[CameraId];
    zdiff = midZ - OldOgaCamZ[CameraId];
    Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (Distance != 0.0) {
        midX = OldOgaCamX[CameraId] + ((CamSpeed[CameraId] * xdiff) / Distance);
        midY = OldOgaCamY[CameraId] + ((CamSpeed[CameraId] * ydiff) / Distance);
        midZ = OldOgaCamZ[CameraId] + ((CamSpeed[CameraId] * zdiff) / Distance);
    } else {
        midX = OldOgaCamX[CameraId];
        midY = OldOgaCamY[CameraId];
        midZ = OldOgaCamZ[CameraId];
    }
    Camera->CameraPos[0] = midX;
    Camera->CameraPos[2] = midZ;
    Camera->CameraPos[1] = Player->Position[1] + 10.0;
    OldOgaCamX[CameraId] = midX;
    OldOgaCamY[CameraId] = midY;
    OldOgaCamZ[CameraId] = midZ;
    Camera->LookatPos[0] = Player->Position[0];
    Camera->LookatPos[1] = Player->Position[1] + 6.0;
    Camera->LookatPos[2] = Player->Position[2];
    CamCheckBump(CameraId, PathIndex);
    sp94 = Camera->LookatPos[0] - Camera->CameraPos[0];
    sp90 = Camera->LookatPos[1] - Camera->CameraPos[1];
    sp8C = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(sp94, sp8C);
    Camera->CameraDirection[0] = Atan2t(sqrtf((sp94 * sp94) + (sp8C * sp8C)), sp90);
    Camera->CameraDirection[2] = 0;
}

void OgawaKaitenInit(s32 PlayerId, UNUSED f32 arg1, s32 CameraId, s16 PathIndex) {
    Camera* Camera = Cameras + CameraId;
    s32 test = CourseTotalPathPoints[PathIndex];

    OgaCameraZure[CameraId] = ZureData[PlayerId];
    OgaCamArea[CameraId] = ((NearestPathPointByPlayerId[PlayerId] + test) - 5) % test;

    ObjCalcFromZureDistBP(OgaCamArea[CameraId], ZureData[PlayerId], 60.0f,
                                    PathIndex);

    OldOgaCamX[CameraId] = object[0];
    OldOgaCamY[CameraId] = (f32) CenterPathBP[PathIndex][OgaCamArea[CameraId]].Pointy;
    OldOgaCamZ[CameraId] = object[2];

    CamSpeedObjct[CameraId] = GamePlayers[PlayerId].Speed;
    CamSpeed[CameraId] = GamePlayers[PlayerId].Speed;
    D_8016448C = 1;
    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaKaitenview(Camera* Camera, UNUSED Player* unusePlayer, UNUSED s32 Arg2, s32 CameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 stackPadding0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    Player* Player;
    f32 Distance;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED f32 sp64;
    UNUSED f32 sp60;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 sp5C;
    f32 temp_f2_4;
    s32 PlayerId;
    s32 PathIndex;
    s32 pathPointCount;

    Player = PlayerOne;
    PlayerId = Camera->Watch;
    PathIndex = CurrentPathID[PlayerId];
    pathPointCount = CourseTotalPathPoints[PathIndex];
    CamSpeed[CameraId] = GamePlayers[PlayerId].Speed;
    if (D_8016448C == 0) {
        if (OgaCamArea[CameraId] ==
            (((NearestPathPointByPlayerId[PlayerId] + pathPointCount) - 6) % pathPointCount)) {
            D_8016448C = 1;
        }
        if (OgaCameraZure[CameraId] < (ZureData[PlayerId] - 0.2)) {
            CamSpeed[CameraId] = GamePlayers[PlayerId].Speed * 0.7;
        }
        if ((ZureData[PlayerId] - 0.5) < OgaCameraZure[CameraId]) {
            OgaCameraZure[CameraId] -= 0.01;
        }
        if (OgaCameraZure[CameraId] < -0.9) {
            OgaCameraZure[CameraId] = -0.9f;
            CamSpeed[CameraId] = GamePlayers[PlayerId].Speed * 0.8;
        }
    } else {
        if (OgaCamArea[CameraId] == ((NearestPathPointByPlayerId[PlayerId] + 6) % pathPointCount)) {
            D_8016448C = 0;
        }
        if ((ZureData[PlayerId] + 0.2) < OgaCameraZure[CameraId]) {
            CamSpeed[CameraId] = GamePlayers[PlayerId].Speed * 1.3;
        }
        if (OgaCameraZure[CameraId] < (ZureData[PlayerId] + 0.5)) {
            OgaCameraZure[CameraId] += 0.01;
        }
        if (OgaCameraZure[CameraId] > 0.9) {
            OgaCameraZure[CameraId] = 0.9f;
            CamSpeed[CameraId] = GamePlayers[PlayerId].Speed * 1.2;
        }
    }
    OgaCameraWatchNum = PlayerId;
    Player += PlayerId;
    OgaCamArea[CameraId] =
        GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], PathIndex);
    playerPathPoint = NearestPathPointByPlayerId[PlayerId];
    cameraPathPoint = OgaCamArea[CameraId];
    if (ExistAreaChk(playerPathPoint, cameraPathPoint, 0x000FU, 0x000FU, pathPointCount) <= 0) {
        OgaDemoCameraReInit(CameraId, OgaCameraZure[CameraId], OgawaCameraMode[CameraId]);
    }
    pathPoint1 = (OgaCamArea[CameraId] + 1) % pathPointCount;
    pathPoint2 = (OgaCamArea[CameraId] + 2) % pathPointCount;
    ObjCalcFromZureDistBP(pathPoint1, OgaCameraZure[CameraId], 60.0f, PathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    ObjCalcFromZureDistBP(pathPoint2, OgaCameraZure[CameraId], 60.0f, PathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[PathIndex][pathPoint1].Pointy + CenterPathBP[PathIndex][pathPoint2].Pointy) / 2.0;
    xdiff = midX - OldOgaCamX[CameraId];
    ydiff = midY - OldOgaCamY[CameraId];
    zdiff = midZ - OldOgaCamZ[CameraId];
    Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (Distance != 0.0) {
        midX = OldOgaCamX[CameraId] + ((CamSpeed[CameraId] * xdiff) / Distance);
        midY = OldOgaCamY[CameraId] + ((CamSpeed[CameraId] * ydiff) / Distance);
        midZ = OldOgaCamZ[CameraId] + ((CamSpeed[CameraId] * zdiff) / Distance);
    } else {
        midX = OldOgaCamX[CameraId];
        midY = OldOgaCamY[CameraId];
        midZ = OldOgaCamZ[CameraId];
    }
    Camera->CameraPos[0] = midX;
    Camera->CameraPos[2] = midZ;
    temp_f2_4 = CheckHight(midX, midY + 30.0, midZ);
    if ((temp_f2_4 < (midY - 20.0)) || (temp_f2_4 >= 3000.0)) {
        Camera->CameraPos[1] = midY + 10.0;
    } else {
        Camera->CameraPos[1] = temp_f2_4 + 8.0;
    }
    OldOgaCamX[CameraId] = midX;
    OldOgaCamY[CameraId] = midY;
    OldOgaCamZ[CameraId] = midZ;
    Camera->LookatPos[0] = Player->Position[0];
    Camera->LookatPos[1] = Player->Position[1] + 6.0;
    Camera->LookatPos[2] = Player->Position[2];
    CamCheckBump(CameraId, PathIndex);
    spAC = Camera->LookatPos[0] - Camera->CameraPos[0];
    spA8 = Camera->LookatPos[1] - Camera->CameraPos[1];
    spA4 = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(spAC, spA4);
    Camera->CameraDirection[0] = Atan2t(sqrtf((spAC * spAC) + (spA4 * spA4)), spA8);
    Camera->CameraDirection[2] = 0;
}

void OgawaGoalInit(s32 PlayerId, f32 arg1, s32 CameraId, UNUSED s16 PathIndex) {
    Camera* Camera = Cameras + CameraId;
    s32 test = CourseTotalPathPoints[0];
    f32 temp_f12;
    f32 temp_f2;

    OgaCameraZure[CameraId] = arg1;
    OgaCamArea[CameraId] = NearestPathPointByPlayerId[PlayerId] + 12;
    OgaCamArea[CameraId] = OgaCamArea[CameraId] % test;

    ObjCalcFromZureBP(OgaCamArea[CameraId], arg1, 0);
    OldOgaCamX[CameraId] = object[0];
    OldOgaCamZ[CameraId] = object[2];
    temp_f2 = (f32) CenterPathBP[0][OgaCamArea[CameraId]].Pointy;

    temp_f12 = CheckHight(object[0], (temp_f2 + 30.0), object[2]);

    if ((temp_f12 < (temp_f2 - 20.0)) || (temp_f12 >= 3000.0)) {
        OldOgaCamY[CameraId] = (f32) (temp_f2 + 10.0);
    } else {
        OldOgaCamY[CameraId] = (f32) (temp_f12 + 10.0);
    }
    CamSpeed[CameraId] = 0.0f;
    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
}

void OgawaGoalview(Camera* Camera, UNUSED Player* playerArg, UNUSED s32 Arg2, s32 CameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 pad;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED s32 Pad2;
    s32 PlayerId;
    UNUSED s32 Pad3[10];
    Player* Player;
    CenterPathStruct* pathPoint;
    s32 PathIndex;
    s32 pathPointCount;

    PlayerId = Camera->Watch;
    PathIndex = CurrentPathID[PlayerId];
    Player = PlayerOne;
    Player += PlayerId;
    pathPointCount = CourseTotalPathPoints[PathIndex];
    OgaCamArea[CameraId] =
        GetOgaCamAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], OgaCamArea[CameraId], 0);
    playerPathPoint = NearestPathPointByPlayerId[PlayerId];
    cameraPathPoint = OgaCamArea[CameraId];
    if (ExistAreaChk(playerPathPoint, cameraPathPoint, 0x0032U, 0x0014U, pathPointCount) <= 0) {
        OgaDemoCameraReInit(CameraId, OgaCameraZure[CameraId], OgawaCameraMode[CameraId]);
    } else {
        if ((ZureData[PlayerId] < -0.5) && ((f64) OgaCameraZure[CameraId] < -0.5)) {
            OgaDemoCameraReInit(CameraId, 1.0f, 0x0000000D);
        } else if ((ZureData[PlayerId] > 0.5) && ((f64) OgaCameraZure[CameraId] > 0.5)) {
            OgaDemoCameraReInit(CameraId, -1.0f, 0x0000000C);
        }
    }
    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
    pathPoint = &CenterPathBP[PathIndex][cameraPathPoint];
    Camera->LookatPos[0] = (Player->Position[0] * 0.8) + (0.2 * pathPoint->Pointx);
    Camera->LookatPos[1] = (Player->Position[1] * 0.8) + (0.2 * pathPoint->Pointy);
    Camera->LookatPos[2] = (Player->Position[2] * 0.8) + (0.2 * pathPoint->Pointz);
    CamCheckBump(CameraId, PathIndex);
    xdiff = Camera->LookatPos[0] - Camera->CameraPos[0];
    ydiff = Camera->LookatPos[1] - Camera->CameraPos[1];
    zdiff = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(xdiff, zdiff);
    Camera->CameraDirection[0] = Atan2t(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    Camera->CameraDirection[2] = 0;
}

void OgaGPRoseErhnamDjinn(void) {
    KoteiGakakuMode[0] = 3;
}

void OgawaNopointInit(UNUSED s32 PlayerId, UNUSED f32 arg1, s32 CameraId, UNUSED s16 PathIndex) {
    CenterPathStruct* pathPoint;

    OgaCamArea[CameraId] = CourseTotalPathPoints[0] - 18;

    pathPoint = &CenterPathBP[0][OgaCamArea[CameraId]];

    OldOgaCamX[CameraId] = pathPoint->Pointx;
    //! @bug Adding an (f32) cast Changes asm, why?
    OldOgaCamY[CameraId] = pathPoint->Pointy + 10.0;
    OldOgaCamZ[CameraId] = pathPoint->Pointz;
    CamSpeed[CameraId] = 0.0f;
    KoteiGakakuMode[CameraId] = 0;
}

void OgawaNopointview(Camera* Camera, UNUSED Player* Player, UNUSED s32 Arg2, s32 CameraId) {
    UNUSED s32 pad[2];
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    CenterPathStruct* temp_v1;

    Camera->CameraPos[0] = OldOgaCamX[CameraId];
    Camera->CameraPos[1] = OldOgaCamY[CameraId];
    Camera->CameraPos[2] = OldOgaCamZ[CameraId];
    temp_v1 = &(*CenterPathBP)[OgaCamArea[CameraId]];
    Camera->LookatPos[0] = (f32) temp_v1->Pointx;
    Camera->LookatPos[1] = (f32) temp_v1->Pointy;
    Camera->LookatPos[2] = (f32) temp_v1->Pointz;
    CamCheckBump(CameraId, 0);
    xdiff = Camera->LookatPos[0] - Camera->CameraPos[0];
    ydiff = Camera->LookatPos[1] - Camera->CameraPos[1];
    zdiff = Camera->LookatPos[2] - Camera->CameraPos[2];
    Camera->CameraDirection[1] = Atan2t(xdiff, zdiff);
    Camera->CameraDirection[0] = Atan2t(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    Camera->CameraDirection[2] = 0;
}

void OgaCameraModeInit(s32 PlayerId, s32 CameraId) {
    s32 PathIndex;
    Camera* Camera = Camera1;
    Camera += CameraId;
    Camera->Watch = PlayerId;

    KatamukiTheta[CameraId] = 0;
    PathIndex = CurrentPathID[PlayerId];

    switch (OgawaCameraMode[CameraId]) {
        case 0:
            OgawaVideoInit(PlayerId, 0.0f, CameraId);
            break;
        case 2:
            OgawaKoteiInit(PlayerId, -1.0f, CameraId, PathIndex);
            break;
        case 3:
            OgawaKoteiInit(PlayerId, 1.0f, CameraId, PathIndex);
            break;
        case 6:
            OgawaOikakeInit(PlayerId, -0.6f, CameraId, (s16) PathIndex);
            break;
        case 7:
            OgawaOikakeInit(PlayerId, 0.6f, CameraId, (s16) PathIndex);
            break;
        case 4:
            OgawaSideInit(PlayerId, -1.0f, CameraId, (s16) PathIndex);
            break;
        case 5:
            OgawaSideInit(PlayerId, 1.0f, CameraId, (s16) PathIndex);
            break;
        case 9:
            OgawaQuickInit(PlayerId, 0.0f, CameraId);
            break;
        case 1:
            OgawaFaceInit(PlayerId, 0.0f, CameraId, (s16) PathIndex);
            break;
        case 14:
            OgawaBackInit(PlayerId, 0.0f, CameraId, (s16) PathIndex);
            break;
        case 8:
            OgawaKaitenInit(PlayerId, 0.0f, CameraId, (s16) PathIndex);
            break;
        case 12:
            OgawaGoalInit(PlayerId, -1.0f, CameraId, (s16) PathIndex);
            break;
        case 13:
            OgawaGoalInit(PlayerId, 1.0f, CameraId, (s16) PathIndex);
            break;
        case 15:
            OgawaNopointInit(PlayerId, -1.0f, CameraId, (s16) PathIndex);
            break;
        case 16:
            OgawaNopointInit(PlayerId, 1.0f, CameraId, (s16) PathIndex);
            break;
        default:
            OgawaVideoInit(PlayerId, 0.0f, CameraId);
            break;
    }
    OgaCamArea[CameraId] =
        CheckOgaAreaBP(Camera->CameraPos[0], Camera->CameraPos[1], Camera->CameraPos[2], (s32) PathIndex);
    if ((s16) OgawaCameraMode[CameraId] == 9) {
        D_80163DD8[CameraId] = (s32) PathIndex;
    }
}

void OgaMokuhyouAngleMove(s32 cameraIndex, u16 arg1) {
    u16 var_v0;

    var_v0 = KatamukiTheta[cameraIndex];
    if (var_v0 < arg1) {
        if ((arg1 - var_v0) < 0x8000) {
            var_v0 += 0x5A;
            if (arg1 < var_v0) {
                var_v0 = arg1;
            }
            if (var_v0 < 0x5A) {
                var_v0 = 0;
            }
        } else if ((arg1 - var_v0) >= 0x8000) {
            var_v0 -= 0x5A;
        }
    } else if (arg1 < var_v0) {
        if ((var_v0 - arg1) < 0x8000) {
            var_v0 -= 0x5A;
            if (var_v0 < arg1) {
                var_v0 = arg1;
            }
            if (var_v0 >= 0xFFA6) {
                var_v0 = 0;
            }
        } else if ((var_v0 - arg1) >= 0x8000) {
            var_v0 += 0x5A;
            if (!cameraIndex) {} // wtf?
        }
    }
    KatamukiTheta[cameraIndex] = (s16) var_v0;
}

void OgaGakakuSet(s32 playerIndex) {
    switch (KoteiGakakuMode[playerIndex]) {
        case 0:
            if (D_80164608[playerIndex] == 1) {
                KoteiGakakuMode[playerIndex] = 1;
                NAPlyTrgStart(playerIndex, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x4F));
                GakakuMode[playerIndex] = KoteiGakakuMode[playerIndex];
            }
            break;
        case 1:
            if (D_80164608[playerIndex] == 1) {
                KoteiGakakuMode[playerIndex] = 0;
                NAPlyTrgStart(playerIndex, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x50));
                GakakuMode[playerIndex] = KoteiGakakuMode[playerIndex];
            }
            break;
    }
}

void OgaBankRoll(Camera* Camera, Player* Player, s32 Arg2) {
    s32 PlayerId;
    s32 nearestPathPoint;

    PlayerId = Camera->Watch;
    if ((OgaFrameCounter != 0) && (CourseID == COURSE_LUIGI_RACEWAY)) {
        OgaCameraRotate(Camera, Arg2);
        nearestPathPoint = NearestPathPointByPlayerId[PlayerId];
        if (((nearestPathPoint >= 0x65) && (nearestPathPoint < 0xFA)) ||
            ((nearestPathPoint >= 0x1AF) && (nearestPathPoint < 0x226))) {
            OgaMokuhyouAngleMove(Arg2, (Player->Kartroll * 2));
        } else {
            OgaMokuhyouAngleMove(Arg2, 0U);
        }
    }
}

void OgaDriversPointCameraInit(void) {
    D_801646CC = 1;
}

void OgaVSWinnerCameraInit(void) {
    s32 i;
    s32 PlayerId = RankToNum[0];
    // clang-format off
    // Has to be on a single line to match. Because IDO hates you :)
    for (i = 0; i < 4; i++) { GakakuMode[i] = KoteiGakakuMode[i]; }
    // clang-format on
    Camera1->Watch = PlayerId;
    KoteiGakakuMode[0] = 1;
    D_801646CC = 2;
}

void OgaTimeattackCameraInit(void) {
    OgawaCameraMode[0] = 1;
    OgaCameraModeInit(0, 0);
    GakakuMode[0] = KoteiGakakuMode[0];
    KoteiGakakuMode[0] = 1;
    OgawaCameraMode[1] = 9;
    OgaCameraModeInit(0, 1);
    GakakuMode[1] = KoteiGakakuMode[1];
    KoteiGakakuMode[1] = 0;
}

void OgaBattleCameraInit(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        GakakuMode[i] = KoteiGakakuMode[i];
    }

    RankToNum[0] = (s16) WinKart;

    Camera1->Watch = (s16) WinKart;

    for (i = 0; i < 4; i++) {
        OgawaCameraMode[i] = 0;
        OgawaVideoInit(WinKart, 0, i);
        KoteiGakakuMode[i] = 1;
    }
}

void OGAContTrgChk(s32 CameraId) {
    struct Controller* controller;

    controller = &ControllerOne[CameraId];
    if (controller->ButtonPressed & L_CBUTTONS) {
        D_801645D0[CameraId] += 1;
    } else {
        D_801645D0[CameraId] = 0;
    }
    if (controller->ButtonPressed & D_CBUTTONS) {
        D_801645E8[CameraId] += 1;
    } else {
        D_801645E8[CameraId] = 0;
    }
    if (controller->ButtonPressed & U_CBUTTONS) {
        D_80164608[CameraId] += 1;
    } else {
        D_80164608[CameraId] = 0;
    }
    if (controller->ButtonPressed & R_CBUTTONS) {
        D_80164628[CameraId] += 1;
    } else {
        D_80164628[CameraId] = 0;
    }
}

void OgaGameCameraAll(UNUSED u16* arg0, UNUSED Camera* arg1, UNUSED Player* Arg2, UNUSED s8 Arg3, s32 arg4) {
    OGAContTrgChk(arg4);
    OgaGakakuSet(arg4);
}

void OgaGameCamera(u16* arg0, Camera* arg1, Player* Arg2, s8 Arg3, s32 arg4) {
    OgaGameCameraAll(arg0, arg1, Arg2, Arg3, arg4);
    OgaBankRoll(arg1, Arg2, arg4);
}

void GoalCameraSetGP(s32 arg0, s32 arg1) {
    switch (GPCurrentRaceRankByPlayerId[arg0]) { /* irregular */
        case 0:
            if (MakeRandomLimmit(0x0064U) < 0x32) {
                OgawaCameraMode[arg1] = 0x000C;
            } else {
                OgawaCameraMode[arg1] = 0x000D;
            }
            NaPanDeforume();
            break;
        case 1:
        case 2:
        case 3:
            OgawaCameraMode[arg1] = 8;
            break;
        default:
            if (MakeRandomLimmit(0x0064U) < 0x32) {
                OgawaCameraMode[arg1] = 0x000F;
            } else {
                OgawaCameraMode[arg1] = 0x0010;
            }
            break;
    }
}

void GoalCameraSetVS(UNUSED s32 arg0, s32 CameraId) {
    switch (MakeRandomLimmit(6)) {
        case 0:
            OgawaCameraMode[CameraId] = 4;
            break;
        case 1:
            OgawaCameraMode[CameraId] = 5;
            break;
        case 2:
            OgawaCameraMode[CameraId] = 6;
            break;
        case 3:
            OgawaCameraMode[CameraId] = 7;
            break;
        case 4:
            OgawaCameraMode[CameraId] = 8;
            break;
        case 5:
            OgawaCameraMode[CameraId] = 1;
            break;
        default:
            OgawaCameraMode[CameraId] = 8;
            break;
    }
}

s32 OgaDemoCameraAreaChkSub(s32 pathPoint, s32 arg1) {
    if ((CourseID == COURSE_BOWSER_CASTLE) && (arg1 != 0) && (pathPoint >= 0xE7) && (pathPoint < 0x1C2)) {
        arg1 = 0;
    }
    return arg1;
}

void OgaDemoCameraReInit(s32 CameraId, f32 arg1, s32 Arg2) {
    UNUSED s32 pad;
    s32 PlayerId;

    PlayerId = Cameras[CameraId].Watch;
    OgaCameraZure[CameraId] = arg1;
    OgawaCameraMode[CameraId] = OgaDemoCameraAreaChkSub((s32) OgaCamArea[CameraId], Arg2);
    OgaCameraModeInit(PlayerId, CameraId);
}

void OgaDemoCameraSoundInit(s32 arg0, f32 arg1, s32 Arg2) {
    s32 PlayerId;

    PlayerId = Cameras[arg0].Watch;
    OgaCameraZure[arg0] = arg1;
    if (Arg2 != OgawaCameraMode[arg0]) {
        OgawaCameraMode[arg0] = Arg2;
        OgaCameraModeInit(PlayerId, arg0);
    }
}

void OgaDemoCameraIncChangeCheck(s32 PlayerId, s32 arg1, s32 Arg2) {
    s32 temp_v1;
    s16 pathPoint;
    s32 temp_v0;

    if (!(GamePlayers[PlayerId].SlipFlag & (HEIGHT_JUMP | N_JUMP | DASH_MIDJUMP))) {
        temp_v1 = OgawaCameraMode[arg1];
        pathPoint = OgaCamArea[arg1];
        temp_v0 = OgaDemoCameraAreaChkSub(pathPoint, (temp_v1 + 1) % 10);
        if ((temp_v0 != temp_v1) || (Arg2 != PlayerId)) {
            OgawaCameraMode[arg1] = temp_v0;
            OgaCameraModeInit(Arg2, arg1);
        }
    }
}

void OgaDemoCameraRankSet(s32 arg0, s32 arg1, s32 Arg2) {
    switch (arg1) { /* irregular */
        case 0:
            OgawaCameraMode[Arg2] = 1;
            break;
        case 1:
        case 2:
        case 3:
            OgaDemoCameraIncChangeCheck(arg0, Arg2, arg0);
            break;
        default:
            OgawaCameraMode[Arg2] = 0;
            break;
    }
}

void Func8001A588(UNUSED u16* localD_80152300, Camera* Camera, Player* Player, s8 Index, s32 cameraIndex) {
    s32 var_v1;
    UnkStruct46D0* temp_v0_4;
    s32 sp44;
    s32 PlayerId;
    PlayerId = Camera->Watch;

    if (GameMode == TIME_TRIALS) {
        PlayerId = 0;
    }
    OGAContTrgChk(cameraIndex);

    if ((s32) (Camera->CameraPos[0] * 10.0) == (s32) ((f64) Camera->LookatPos[0] * 10.0)) {

        if ((s32) (Camera->CameraPos[2] * 10.0) == (s32) ((f64) Camera->LookatPos[2] * 10.0)) {
            Camera->CameraPos[0] = (f32) (Camera->CameraPos[0] + 100.0);
            Camera->CameraPos[2] = (f32) (Camera->CameraPos[2] + 100.0);
        }
    }
    if ((GameMode != BATTLE) && (OgawaCameraMode[cameraIndex] == -1) && (Player->Flag & IS_RACE_FINISH) &&
        ((u16) D_801646CC == 0) && (OgaGoalCounter == 0)) {
        if (GameMode == VERSUS) {
            GoalCameraSetVS(PlayerId, cameraIndex);
        } else {
            GoalCameraSetGP((s32) PlayerId, cameraIndex);
        }
        OgaCameraModeInit((s32) PlayerId, cameraIndex);
    }

    if ((OgawaCameraMode[cameraIndex] == 14) || (OgawaCameraMode[cameraIndex] == 0)) {
        OgaBankRoll(Camera, Player, cameraIndex);
    } else {
        KatamukiTheta[cameraIndex] = 0;
        OgaCameraRotate(Camera, cameraIndex);
    }
    switch ((u16) D_801646CC) {
        case 1:
            OgaGoalCounter += 1;
            if (OgaGoalCounter >= 501) {
                OgaGoalCounter = 0;
            }
            if ((cameraIndex == 0) && (((OgaGoalCounter == 10)) || (OgaGoalCounter == 11))) {
                OgaDemoCameraRankSet((s32) PlayerId, GPCurrentRaceRankByPlayerId[PlayerId], 0);
            }
            if ((GameMode != TIME_TRIALS) && (cameraIndex == 1) &&
                (((OgaGoalCounter == 260)) || (OgaGoalCounter == 261))) {

                var_v1 = 0;
                if (MenuMultiplayerSelection == 2) {
                    OgaDemoCameraRankSet((s32) PlayerId, GPCurrentRaceRankByPlayerId[PlayerId], 1);
                } else {
                    sp44 = (s32) PlayerId;
                    while (var_v1 != 8) {
                        PlayerId += 1;
                        var_v1 += 1;
                        if (PlayerId >= 8) {
                            PlayerId = 1;
                        }
                        if ((!(GamePlayers[PlayerId].JugemuFlag & ON_LAKITU_ROD) &&
                             !(GamePlayers[PlayerId].JugemuFlag & LAKITU_SCENE))) {
                            break;
                        }
                    }
                    OgaDemoCameraIncChangeCheck(sp44, cameraIndex, (s32) PlayerId);
                }
            }
            break;
        case 2:
            OgaGoalCounter += 1;
            if (OgaGoalCounter > 250) {
                OgaGoalCounter = 0;
            }
            if ((cameraIndex == 0) && (OgaGoalCounter == 10)) {
                OgaDemoCameraIncChangeCheck((s32) PlayerId, cameraIndex, (s32) PlayerId);
            }
            break;
        default:
            temp_v0_4 = &OgaSoundCamera[cameraIndex];
            if (temp_v0_4->Unk0 == (s16) 1) {
                PlayerId = temp_v0_4->unk4;
                temp_v0_4->Unk0 = 0;
                Cameras[cameraIndex].Watch = PlayerId;
                OgaDemoCameraSoundInit(cameraIndex, 0.0f, (s32) temp_v0_4->Unk2);
            }
            break;
    }
    OgaGakakuSet(cameraIndex);
    switch (OgawaCameraMode[cameraIndex]) {
        case 0:
            OgawaVideoview(Camera, Player, Index);
            break;
        case 2:
        case 3:
            OgawaKoteiview(Camera, Player, Index, cameraIndex);
            break;
        case 6:
        case 7:
            OgawaOikakeview(Camera, Player, Index, cameraIndex);
            break;
        case 4:
        case 5:
            OgawaSideview(Camera, Player, Index, cameraIndex);
            break;
        case 9:
            OgawaQuickview(Camera, Player, Index, cameraIndex);
            break;
        case 1:
            OgawaFaceview(Camera, Player, Index, cameraIndex);
            break;
        case 14:
            OgawaBackview(Camera, Player, Index, cameraIndex);
            break;
        case 8:
            OgawaKaitenview(Camera, Player, Index, cameraIndex);
            break;
        case 12:
        case 13:
            OgawaGoalview(Camera, Player, Index, cameraIndex);
            break;
        case 15:
        case 16:
            OgawaNopointview(Camera, Player, Index, cameraIndex);
            break;
        default:
            OgawaVideoview(Camera, Player, Index);
            break;
    }
}

void OgawaSoundCameraSet(s16 arg0, s16 arg1, s16 Arg2) {
    if (OgaSoundCamera[arg0].Unk0 == 0) {
        OgaSoundCamera[arg0].Unk0 = 1;
        OgaSoundCamera[arg0].Unk2 = arg1;
        OgaSoundCamera[arg0].unk4 = Arg2;
    }
}

#include "cpu_vehicles_camera_path/cpu_item_strategy.inc.c"

void EnemyItemMove(s32 PlayerId) {
    Player* Player = &PlayerOne[PlayerId];
    Object* actor;
    EnemyItem* cpuStrategy = &EnemyItemTable[PlayerId];
    CenterPathStruct* pathPoint;
    bool isValidBanana1;
    bool isValidBanana2;

// Only Used in this function
#define BANANA_ACTOR(actor) ((struct BananaActor*)(actor))
#define SHELL_ACTOR(actor) ((struct ShellActor*)(actor))
#define FAKE_ITEMBOX_ACTOR(actor) ((struct FakeItemBox*)(actor))
#define BANANA_BUNCH_ACTOR(actor) ((struct BananaBunchParent*)(actor))

    if (GameMode == TIME_TRIALS) {
        return;
    }

    if ((u16) D_801646CC == 1) {
        return;
    }

    if (Player->Flag & IS_RACE_FINISH) {
        return;
    }

    switch (cpuStrategy->ItemNum) {
        case CPU_STRATEGY_WAIT_NEXT_ITEM:
            cpuStrategy->ObjNum = -1;
            if ((((PlayerId * 20) + 100) < PlayerPathPointTotalTable[PlayerId]) && (cpuStrategy->Time >= 0x259) &&
                (cpuStrategy->Limit < 3) && (LapCountByPlayerId[PlayerId] < 3)) {
                EnemyItemNumberSet(PlayerId, &cpuStrategy->ItemNum,
                                            KwgetitemEnemy((s16) LapCountByPlayerId[PlayerId],
                                                                GPCurrentRaceRankByPlayerId[PlayerId]));
            } else {
                EnemyItemDebugBANDOU(PlayerId, cpuStrategy);
            }
            break;

        case CPU_STRATEGY_ITEM_BANANA:
            // never true
            if ((LapCountByPlayerId[PlayerId] > 0) && (GPCurrentRaceRankByPlayerId[PlayerId] > GPCurrentRaceRankByPlayerId[EnemyTargetPlayer]) && (GPCurrentRaceRankByPlayerId[EnemyTargetPlayer] == FIRST_PLACE)) {
                switch (Player->Kart) {
                    case DK:
                        if (ExistAreaChk(NearestPathPointByPlayerId[PlayerId],
                                                    NearestPathPointByPlayerId[EnemyTargetPlayer], 40, 2,
                                                    CenterLineMax) > 0) {
                            cpuStrategy->ItemNum = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;

                    case PEACH:
                        if (ExistAreaChk(NearestPathPointByPlayerId[PlayerId],
                                                    NearestPathPointByPlayerId[EnemyTargetPlayer], 4, 2,
                                                    CenterLineMax) > 0) {
                            cpuStrategy->ItemNum = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;

                    default:
                        if (ExistAreaChk(NearestPathPointByPlayerId[PlayerId],
                                                    NearestPathPointByPlayerId[EnemyTargetPlayer], 10, 2,
                                                    CenterLineMax) > 0) {
                            cpuStrategy->ItemNum = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;
                }
            } else if (cpuStrategy->ItemNum == CPU_STRATEGY_ITEM_BANANA) {
                cpuStrategy->ObjNum = PutBanana(Player);
                if ((cpuStrategy->ObjNum >= 0) && (cpuStrategy->ObjNum < 100)) {
                    Player->Weapon |= USE_BANANA;
                    cpuStrategy->ItemNum = CPU_STRATEGY_HOLD_BANANA;
                    cpuStrategy->Time = 0;
                    cpuStrategy->Limit += 1;
                    cpuStrategy->Count4 = (MakeRandomLimmit(3) * 20) + 10;
                } else {
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->Time = 0;
                }
            }
            break;

        case CPU_STRATEGY_HOLD_BANANA:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->Category != ACTOR_BANANA) || (BANANA_ACTOR(actor)->Sparam != HELD_BANANA) ||
                (PlayerId != BANANA_ACTOR(actor)->PlayerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->Category != 6) {}
                if (BANANA_ACTOR(actor)->Sparam != 0) {}
                if (BANANA_ACTOR(actor)->Angle[0] != PlayerId) {}

                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->Time = 0;
                Player->Weapon &= ~USE_BANANA;
            } else if (cpuStrategy->Count4 < cpuStrategy->Time) {
                cpuStrategy->ItemNum = CPU_STRATEGY_DROP_BANANA;
            }
            break;

        case CPU_STRATEGY_DROP_BANANA:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->Category != ACTOR_BANANA)) ||
                    (BANANA_ACTOR(actor)->Sparam != HELD_BANANA)) ||
                (PlayerId != BANANA_ACTOR(actor)->PlayerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->Category != 6) {}
                if (BANANA_ACTOR(actor)->Sparam != 0) {}
                if (BANANA_ACTOR(actor)->Angle[0] != PlayerId) {}

            } else {
                BANANA_ACTOR(actor)->Sparam = DROPPED_BANANA;
                BANANA_ACTOR(actor)->Velocity[0] = 0.0f;
                BANANA_ACTOR(actor)->Velocity[1] = 0.0f;
                BANANA_ACTOR(actor)->Velocity[2] = 0.0f;
                if (TenukiFlg[PlayerId] == true) {
                    BANANA_ACTOR(actor)->Position[1] =
                        CheckHight(Player->Position[0], Player->Position[1] + 30.0, Player->Position[2]) +
                        (BANANA_ACTOR(actor)->Radius + 1.0f);
                }
            }
            Player->Weapon &= ~USE_BANANA;
            cpuStrategy->Time = 0;
            cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            break;

        case CPU_STRATEGY_THROW_BANANA:
            cpuStrategy->ObjNum = PutBanana(Player);
            if ((cpuStrategy->ObjNum >= 0) && (cpuStrategy->ObjNum < 100)) {
                actor = &SimpleObjectArray[cpuStrategy->ObjNum];
                BANANA_ACTOR(actor)->Sparam = BANANA_ON_GROUND;
                Player->Weapon |= USE_BANANA;
                cpuStrategy->ItemNum = CPU_STRATEGY_HOLD_THROW_BANANA;
                cpuStrategy->Time = 0;
                cpuStrategy->Limit += 1;
                pathPoint = &CenterPathBP[CurrentPathID[0]]
                                        [(NearestPathPointByPlayerId[EnemyTargetPlayer] + 30) %
                                            CourseTotalPathPoints[CurrentPathID[EnemyTargetPlayer]]];
                BANANA_ACTOR(actor)->Velocity[0] = (pathPoint->Pointx - Player->Position[0]) / 20.0;
                BANANA_ACTOR(actor)->Velocity[1] = ((pathPoint->Pointy - Player->Position[1]) / 20.0) + 4.0;
                BANANA_ACTOR(actor)->Velocity[2] = (pathPoint->Pointz - Player->Position[2]) / 20.0;
                BANANA_ACTOR(actor)->Position[1] = Player->Position[1];
                NaEnmVoiceStart(PlayerId, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x09));
                NAEnmTrgStart(Player->Position, Player->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x14));
            } else {
                cpuStrategy->Time = 0;
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_THROW_BANANA:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->Category != ACTOR_BANANA)) ||
                    (BANANA_ACTOR(actor)->Sparam != BANANA_ON_GROUND)) ||
                (PlayerId != BANANA_ACTOR(actor)->PlayerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->Category != 6) {}
                if (BANANA_ACTOR(actor)->Sparam != 0) {}
                if (BANANA_ACTOR(actor)->Angle[0] != PlayerId) {}

                cpuStrategy->Time = 0;
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                Player->Weapon &= ~USE_BANANA;
            } else {
                BANANA_ACTOR(actor)->Velocity[1] -= 0.4;
                BANANA_ACTOR(actor)->Position[0] += BANANA_ACTOR(actor)->Velocity[0];
                BANANA_ACTOR(actor)->Position[1] += BANANA_ACTOR(actor)->Velocity[1];
                BANANA_ACTOR(actor)->Position[2] += BANANA_ACTOR(actor)->Velocity[2];
                if (cpuStrategy->Time > 20) {
                    cpuStrategy->ItemNum = CPU_STRATEGY_END_THROW_BANANA;
                }
            }
            break;

        case CPU_STRATEGY_END_THROW_BANANA:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->Category != ACTOR_BANANA)) ||
                    (BANANA_ACTOR(actor)->Sparam != BANANA_ON_GROUND)) ||
                (PlayerId != BANANA_ACTOR(actor)->PlayerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->Flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->Category != 6) {}
                if (BANANA_ACTOR(actor)->Sparam != 0) {}
                if (BANANA_ACTOR(actor)->Angle[0] != PlayerId) {}

            } else {
                BANANA_ACTOR(actor)->Sparam = DROPPED_BANANA;
                BANANA_ACTOR(actor)->Velocity[0] = 0.0f;
                BANANA_ACTOR(actor)->Velocity[1] = 0.0f;
                BANANA_ACTOR(actor)->Velocity[2] = 0.0f;
                BANANA_ACTOR(actor)->Position[1] =
                    CheckHight(BANANA_ACTOR(actor)->Position[0], BANANA_ACTOR(actor)->Position[1] + 30.0, BANANA_ACTOR(actor)->Position[2]) +
                    (BANANA_ACTOR(actor)->Radius + 1.0f);
            }
            Player->Weapon &= ~USE_BANANA;
            cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->Time = 0;
            break;

        case CPU_STRATEGY_ITEM_GREEN_SHELL:
            if (SimpleObjectCount < 80) {
                cpuStrategy->ObjNum = SetGshell(Player);
                if ((cpuStrategy->ObjNum >= 0) && (cpuStrategy->ObjNum < 100)) {
                    cpuStrategy->ItemNum = CPU_STRATEGY_HOLD_GREEN_SHELL;
                    cpuStrategy->Time = 0;
                    cpuStrategy->Limit += 1;
                    cpuStrategy->Count4 = (MakeRandomLimmit(3) * 20) + 10;
                } else {
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_GREEN_SHELL:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(actor->Flag & EXISTOBJ)) || (actor->Category != ACTOR_GREEN_SHELL)) ||
                    (actor->Sparam != HELD_SHELL)) ||
                (PlayerId != actor->Angle[2])) {

                // FAKE
                if (!(actor->Flag & EXISTOBJ)) {}
                if (actor->Category != 7) {}
                if (actor->Sparam != 0) {}
                if (actor->Angle[0] != PlayerId) {}

                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->Time = 0;
            } else if (cpuStrategy->Count4 < cpuStrategy->Time) {
                cpuStrategy->ItemNum = CPU_STRATEGY_THROW_GREEN_SHELL;
                cpuStrategy->Time = 0;
            }
            break;

        case CPU_STRATEGY_THROW_GREEN_SHELL:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(actor->Flag & EXISTOBJ)) || (actor->Category != ACTOR_GREEN_SHELL)) ||
                    (actor->Sparam != HELD_SHELL)) ||
                (PlayerId != actor->Angle[2])) {

                // FAKE
                if (!(actor->Flag & EXISTOBJ)) {}
                if (actor->Category != 7) {}
                if (actor->Sparam != 0) {}
                if (actor->Angle[0] != PlayerId) {}

                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->Time = 0;
            } else {
                actor->Sparam = RELEASED_SHELL;
                cpuStrategy->Time = 0;
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_ITEM_RED_SHELL:
            if (SimpleObjectCount < 80) {
                cpuStrategy->ObjNum = SetRshell(Player);
                if ((cpuStrategy->ObjNum >= 0) && (cpuStrategy->ObjNum < 100)) {
                    cpuStrategy->ItemNum = CPU_STRATEGY_HOLD_RED_SHELL;
                    cpuStrategy->Time = 0;
                    cpuStrategy->Limit += 1;
                    cpuStrategy->Count4 = (MakeRandomLimmit(3) * 20) + 10;
                } else {
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_RED_SHELL:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(SHELL_ACTOR(actor)->Flag & EXISTOBJ)) || (SHELL_ACTOR(actor)->Category != ACTOR_RED_SHELL)) ||
                    (SHELL_ACTOR(actor)->Sparam != HELD_SHELL)) ||
                (PlayerId != SHELL_ACTOR(actor)->PlayerId)) {

                // FAKE
                if (!(actor->Flag & EXISTOBJ)) {}
                if (actor->Category != 8) {}
                if (actor->Sparam != 0) {}
                if (actor->Angle[0] != PlayerId) {}

                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->Time = 0;
            } else if (cpuStrategy->Count4 < cpuStrategy->Time) {
                cpuStrategy->ItemNum = CPU_STRATEGY_THROW_RED_SHELL;
            }
            break;

        case CPU_STRATEGY_THROW_RED_SHELL:
            CheckItemNumber(cpuStrategy);
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(SHELL_ACTOR(actor)->Flag & EXISTOBJ)) || (SHELL_ACTOR(actor)->Category != ACTOR_RED_SHELL)) ||
                    (SHELL_ACTOR(actor)->Sparam != HELD_SHELL)) ||
                (PlayerId != SHELL_ACTOR(actor)->PlayerId)) {

                // FAKE
                if (!(actor->Flag & EXISTOBJ)) {}
                if (actor->Category != 8) {}
                if (actor->Sparam != 0) {}
                if (actor->Angle[0] != PlayerId) {}

                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->Time = 0;
            } else {
                SHELL_ACTOR(actor)->Sparam = RELEASED_SHELL;
                cpuStrategy->Time = 0;
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_ITEM_BANANA_BUNCH:
            if (SimpleObjectCount < 80) {
                cpuStrategy->ObjNum = SetSuperBanana(Player);
                if ((cpuStrategy->ObjNum >= 0) && (cpuStrategy->ObjNum < 100)) {
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH;
                    cpuStrategy->Time = 0;
                    cpuStrategy->Limit += 1;
                    cpuStrategy->Count4 = (MakeRandomLimmit(3) * 20) + 60;
                } else {
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if (BANANA_BUNCH_ACTOR(actor)->Sparam == 6) {

                // FAKE
                if (BANANA_BUNCH_ACTOR(actor)->Sparam != -1) {}
                if (BANANA_BUNCH_ACTOR(actor)->Sparam == 6) {}

                isValidBanana2 = false;

                if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[4] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[3] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[2] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[1] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[0] != (-1)) {
                    isValidBanana2 = true;
                }
                if ((BANANA_BUNCH_ACTOR(actor)->Category != ACTOR_BANANA_BUNCH) || (isValidBanana2 == false)) {
                    if (BANANA_BUNCH_ACTOR(actor)->Category != 14) {}
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->Time = 0;
                } else if (cpuStrategy->Count4 < cpuStrategy->Time) {
                    cpuStrategy->ItemNum = CPU_STRATEGY_DROP_BANANA_BUNCH;
                    cpuStrategy->Count1 = 0;
                    cpuStrategy->Time = 0;
                }
            }
            break;

        case CPU_STRATEGY_DROP_BANANA_BUNCH:
            if (((cpuStrategy->Time) % 10) == 0) {
                if (cpuStrategy->Count1 < 5) {
                    isValidBanana1 = 0;
                    actor = &SimpleObjectArray[cpuStrategy->ObjNum];
                    switch (cpuStrategy->Count1) {
                        case 0:
                            if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[4] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 1:
                            if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[3] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 2:
                            if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[2] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 3:
                            if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[1] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 4:
                            if (BANANA_BUNCH_ACTOR(actor)->BananaIndices[0] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;
                    }

                    if (((BANANA_BUNCH_ACTOR(actor)->Category == ACTOR_BANANA_BUNCH) && (BANANA_BUNCH_ACTOR(actor)->Sparam == 6)) &&
                        (isValidBanana1 == true)) {
                        ReleaseBanana((struct BananaBunchParent*)actor);
                    }
                    cpuStrategy->Count1 += 1;
                } else {
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->Time = 0;
                }
            }
            break;

        case CPU_STRATEGY_ITEM_FAKE_ITEM_BOX:
            cpuStrategy->ObjNum = PutFakeItembox(Player);
            if ((cpuStrategy->ObjNum >= 0) && (cpuStrategy->ObjNum < 100)) {
                cpuStrategy->ItemNum = CPU_STRATEGY_HOLD_FAKE_ITEM_BOX;
                cpuStrategy->Time = 0;
                cpuStrategy->Limit += 1;
                cpuStrategy->Count4 = (MakeRandomLimmit(3) * 20) + 10;
            } else {
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_FAKE_ITEM_BOX:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(FAKE_ITEMBOX_ACTOR(actor)->Flag & EXISTOBJ)) || (FAKE_ITEMBOX_ACTOR(actor)->Category != ACTOR_FAKE_ITEM_BOX)) ||
                    (FAKE_ITEMBOX_ACTOR(actor)->Sparam != 0)) ||
                (PlayerId != ((s32) FAKE_ITEMBOX_ACTOR(actor)->PlayerId))) {

                // FAKE
                if (!(actor->Flag & EXISTOBJ)) {}
                if (actor->Category != 13) {}
                if (actor->Sparam != 0) {}
                if (actor->Angle[0] != PlayerId) {}

                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->Time = 0;
            } else if (cpuStrategy->Count4 < cpuStrategy->Time) {
                cpuStrategy->ItemNum = CPU_STRATEGY_THROW_FAKE_ITEM_BOX;
            }
            break;

        case CPU_STRATEGY_THROW_FAKE_ITEM_BOX:
            actor = &SimpleObjectArray[cpuStrategy->ObjNum];
            if ((((!(FAKE_ITEMBOX_ACTOR(actor)->Flag & EXISTOBJ)) || (FAKE_ITEMBOX_ACTOR(actor)->Category != ACTOR_FAKE_ITEM_BOX)) ||
                    (FAKE_ITEMBOX_ACTOR(actor)->Sparam != 0)) ||
                (PlayerId != ((s32) FAKE_ITEMBOX_ACTOR(actor)->PlayerId))) {

                // FAKE
                if (!(FAKE_ITEMBOX_ACTOR(actor)->Flag & EXISTOBJ)) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->Category != 13) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->Sparam != 0) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->Angle[0] != PlayerId) {}

            } else {
                ReleaseFakeItembox((struct FakeItemBox*)actor);
                if (TenukiFlg[PlayerId] == true) {
                    FAKE_ITEMBOX_ACTOR(actor)->Position[1] =
                        CheckHight(FAKE_ITEMBOX_ACTOR(actor)->Position[0], FAKE_ITEMBOX_ACTOR(actor)->Position[1] + 30.0, FAKE_ITEMBOX_ACTOR(actor)->Position[2]) +
                        FAKE_ITEMBOX_ACTOR(actor)->Radius;
                }
            }
            cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->Time = 0;
            break;

        case CPU_STRATEGY_ITEM_THUNDERBOLT:
            UseThunder(Player);
            NaEnmThunderStart(PlayerId);
            ThunderWorld();
            cpuStrategy->ItemNum = CPU_STRATEGY_END_THUNDERBOLT;
            cpuStrategy->Time = 0;
            cpuStrategy->Limit += 1;
            break;

        case CPU_STRATEGY_END_THUNDERBOLT:
            if (cpuStrategy->Time >= 0xF1) {
                NaEnmThunderStop((s32) ((u8) PlayerId));
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->Time = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_STAR:
            Player->Weapon |= USE_STAR;
            cpuStrategy->ItemNum = CPU_STRATEGY_END_ITEM_STAR;
            cpuStrategy->Time = 0;
            cpuStrategy->Limit += 1;
            break;

        case CPU_STRATEGY_END_ITEM_STAR:
            if (!(Player->SlipFlag & STAR)) {
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            cpuStrategy->Time = 0;
            break;

        case CPU_STRATEGY_ITEM_BOO:
            Player->Weapon |= USE_BOO;
            cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_END_BOO;
            cpuStrategy->Time = 0;
            cpuStrategy->Limit += 1;
            break;

        case CPU_STRATEGY_WAIT_END_BOO:
            if (!(Player->SlipFlag & TERESA)) {
                cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            cpuStrategy->Time = 0;
            break;

        case CPU_STRATEGY_ITEM_MUSHROOM:
            Player->Weapon |= USE_MUSHROOM;
            cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->Time = 0;
            cpuStrategy->Limit += 1;
            break;

        case CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM:
            if (cpuStrategy->Time >= 0x3D) {
                Player->Weapon |= USE_MUSHROOM;
                cpuStrategy->ItemNum = CPU_STRATEGY_ITEM_MUSHROOM;
                cpuStrategy->Time = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_TRIPLE_MUSHROOM:
            if (cpuStrategy->Time >= 0x3D) {
                Player->Weapon |= USE_MUSHROOM;
                cpuStrategy->ItemNum = CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM;
                cpuStrategy->Time = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_SUPER_MUSHROOM:
            cpuStrategy->ItemNum = CPU_STRATEGY_USE_SUPER_MUSHROOM;
            cpuStrategy->Time = 0;
            cpuStrategy->Count4 = 0x0258;
            break;

        case CPU_STRATEGY_USE_SUPER_MUSHROOM:
            if ((((s16) cpuStrategy->Time) % 60) == 0) {
                Player->Weapon |= USE_MUSHROOM;
                if (cpuStrategy->Count4 < cpuStrategy->Time) {
                    cpuStrategy->Time = 0;
                    cpuStrategy->ItemNum = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            }
            break;

        default:
            break;
    }

    if (cpuStrategy->Time < 10000) {
        cpuStrategy->Time += 1;
    }
    if (Player->SlipFlag & (TERESA | TURBO | STAR)) {
        cpuStrategy->Time = 0;
    }
}

#undef BANANA_ACTOR
#undef SHELL_ACTOR
#undef FAKE_ITEMBOX_ACTOR
#undef BANANA_BUNCH

void OgaHyousyouCarInitSub(void) {
    Player* temp_s1;
    CenterPathStruct* temp_s0;
    s32 i;

    InitEnemyKart();
    for (i = 0; i < 4; i++) {
        temp_s1 = &PlayerOne[i];
        temp_s1->Flag &= 0xDFFF;
        CurrentPathID[i] = i;
        LaneData[i].LaneFreeDrive = 0.0f;
        LaneData[i].LaneTarget = 0.0f;
        LaneData[i].Lane = 0.0f;
        switch (i) {
            case 0:
                NearestPathPointByPlayerId[i] = 6;
                break;
            case 1:
                NearestPathPointByPlayerId[i] = 1;
                break;
            case 2:
                NearestPathPointByPlayerId[i] = 6;
                break;
            case 3:
                NearestPathPointByPlayerId[i] = 1;
                break;
        }
        temp_s0 = &CenterPathBP[i][NearestPathPointByPlayerId[i]];
        temp_s1->Position[0] = (f32) temp_s0->Pointx;
        temp_s1->Position[1] =
            CheckHight((f32) temp_s0->Pointx, 2000.0f, (f32) temp_s0->Pointz) + temp_s1->Radius;
        temp_s1->Position[2] = (f32) temp_s0->Pointz;
        temp_s1->Direction[1] = (s16) *AnglePathDataBP[i];
        EnemyStickAngle(temp_s1, 0);
        temp_s1++;
        OgaNearHyousyouFlg[i] = 0;
    }
}

void OgaHyousyouInit(void) {
    CourseID = COURSE_AWARD_CEREMONY;
    OgaHyousyouDemoTrg = 0;
    OgaNopointDemoTrg = 0;
    OgaHyousyouDemoCounter = 0;
    OgaNopointDemoCounter = 0;
    InitCenterLine();
    OgawaCameraInit(0);
    OgaHyousyouCarInitSub();
    OgaHyousyouMx[0] = -3202.475097656f;
    OgaHyousyouMy[0] = 19.166999817f;
    OgaHyousyouMz[0] = -477.623992920f;
    OgaHyousyouMx[1] = -3205.080078125f;
    OgaHyousyouMy[1] = 19.166999817f;
    OgaHyousyouMz[1] = -462.851989746f;
    OgaHyousyouMx[2] = -3199.870117188f;
    OgaHyousyouMy[2] = 19.166999817f;
    OgaHyousyouMz[2] = -492.395996094f;
    OgaHyousyouMx[3] = -2409.197021484f;
    OgaHyousyouMy[3] = 0.0f;
    OgaHyousyouMz[3] = -355.253997803;
}

void OgaHyousyouMove(void) {
    f32 temp_f0;
    f32 temp_f2;
    s32 PlayerId;
    Player* temp_s0;

    if (OgaHyousyouDemoTrg == 1) {
        OgaHyousyouDemoCounter += 1;
    }
    if ((OgaNopointDemoTrg == 1) && (Bom[0].State == 0) && (Resultstat.Rank >= 3)) {
        OgaNopointDemoCounter++;
        if (OgaNopointDemoCounter >= 0xF) {
            OgaNopointDemoCounter = 0;
            OgaNopointDemoTrg = 2;
            StartKawanoResult();
        }
    }
    for (PlayerId = 0; PlayerId < 4; PlayerId++) {
        if ((PlayerId == 3) && (OgaHyousyouDemoTrg == 0)) {
            break;
        }

        temp_s0 = &PlayerOne[PlayerId];
        DriveEnemyKart(PlayerId);
        if (!(temp_s0->Flag & NO_CONTROLS)) {
            temp_f0 = OgaHyousyouMx[PlayerId] - temp_s0->Position[0];
            temp_f2 = OgaHyousyouMz[PlayerId] - temp_s0->Position[2];
            if ((f64) ((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 1.0) {
                if (PlayerId != 3) {
                    if (1) {}
                    // Why oh why is a ternary required here? Who does that?
                    (OgaHyousyouDemoTrg == 0) ? (temp_s0->Flag |= NO_CONTROLS)
                                      : (temp_s0->Flag &= ~NO_CONTROLS);
                    if ((PlayerOne->Flag & NO_CONTROLS) && (PlayerTwo->Flag & NO_CONTROLS) &&
                        (PlayerThree->Flag & NO_CONTROLS)) {
                        OgaHyousyouDemoTrg = 1;
                        OgaHyousyouDemoCounter = 0;
                    }
                } else if (OgaNopointDemoTrg == 0) {
                    if (!(temp_s0->SlipFlag & EXPLODE)) {
                        temp_s0->Flag |= NO_CONTROLS;
                    }
                    OgaNopointDemoTrg = 1;
                    OgaNopointDemoCounter = 0;
                } else if (!(temp_s0->SlipFlag & EXPLODE)) {
                    temp_s0->Flag |= NO_CONTROLS;
                }
            }
        }
    }
}

void OgawaDraw(s32 CameraId) {
    if (CourseID == COURSE_AWARD_CEREMONY) {
        if (Bom[0].PathPointIndex >= 16) {
            KwdisplayBombkart(PLAYER_FOUR);
        }
    } else {
        if (GameMode == VERSUS) {
            KwdisplayBombkart(CameraId);
        }
    }
}

UNUSED void OgawaDebugDraw(void) {
    if (D_800DDB20 == 0) {
        if ((ControllerThree->ButtonPressed & L_TRIG) != 0) {
            D_800DDB20 = 1;
        }
    } else {
        if ((ControllerThree->ButtonPressed & L_TRIG) != 0) {
            D_800DDB20 = 0;
        }
        KWSetViewportFull();
        gSPDisplayList(DisplayListHead++, D_0D0076F8);
        KWReturnViewport();
    }
}
