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

s32 unk_cpu_vehicles_camera_path_pad[24];
Collision D_80162E70;
s16 D_80162EB0; // Possibly a float.
s16 D_80162EB2; // possibly [3]

ActionData* gCoursesCPUBehaviour[NUM_COURSES - 1];

// more padding?
s32 D_80162F08[2];

s16 pylon_num[30];
s16 pylon_num2[30];

// padding, seemingly?
s32 pylon_pos[4];

Vec3f object;
Vec3f object1;
Vec3f object2;
s16 gTrainSmokeTimer;
s16 OGAArea;
s16 biri_flg;
f32 rap_dat[NUM_PLAYERS];
/*
The size of many of these arrays is weird. Based on their usage, they would only need
to be 8 entries long (enough for each player). But some are 10 or even 12 long.
Its plausible that this is just some decompilation artifact?
Or maybe at some point in development they had plans for more players?
*/
s16 avoid_mode[12];
s16 avoid_count[12];
f32 speedlevel[10];
s16 old_eos_angle_oga[12];
f32 zure_data[10];
f32 zure_dist[10];
bool straight_flg[10];
u16 oga_area_org;
s16 oga_area_chg_flg;
s16 g_driftFlg[10];
s16 gFerrySmokeTimer;
s32 enemy_time_count[10];
s32 enemy_dist[10];
s32 enemy_limdist[10];
f32 enemy_object_x[10];
f32 enemy_object_z[10];
s16 sVehicleSoundRenderCounter;
s32 best_time;
CenterPathStruct* side_point_L;
CenterPathStruct* side_point_R;
s16* curve_data;
s16* angle_data;
u16 tenuki_flg[12];
u16 old_tenuki_flg[10];
f32 limmitspeed;
f32 old_limmitspeed[10];
s32 oga_camera_watch_num;
u16 rap_count_flg[12];
u16 oga_reverse_count[12];
u16 g_wrongwayFlagPlayer1[12];
s32 old_pylon_count[10];
ActionData* ActPointPtr;
u16 next_action_num[12];
u16 old_action_num[12];
u16 g_actionFlg[12];
s16 oga_angle[12];
u16 g_lakituSurface[12];
u16 oga_car_stat[10];
u16 enemy_rival_num[2];
u16 enemy_rival_name[2];
u16 enemy_rival_aim_rank[2];
u16 enemy_accel_mode[12];
s32 MaxPathPoints[4];
s32 oga_frame_counter;
s32 oga_etc_frame_counter;
s16 mokuhyou_add_area[12];
s16 enemy_voice_count_SPIN[12];
s16 enemy_voice_count_DAMAGE[12];
s16 enemy_voice_count_OVERTAKE[12];
s16 oga_hamidasi_map_flg[12];
s16 g_noSimpleKartFlag[12];
s16 oga_near_hyousyou_flg[4];
f32 oga_hyousyou_mx[4];
f32 oga_hyousyou_my[4];
f32 oga_hyousyou_mz[4];
s32 bipas_num;
f32 g_finishLineZ;
f32 oga_old_rap_chk_z[10];
s16 g_EnemyTargetPlayer;
s16 highway_gyakusou_flg;
s16 oga_hyousyou_demo_trg;
s16 oga_nopoint_demo_trg;
s32 oga_hyousyou_demo_counter;
s32 oga_nopoint_demo_counter;
s32 oga_bump_chk_counter;
s16 g_rankUpdateFinishFlag;
s16 enemy_enter_trg[12];
s16 enemy_exit_trg[12];
s16 g_rivalOvertakeAllowFlag[12];
s16 fumikiri_stop_flg[10];
s16 ogawa_karakara_short_cut_ok_flg;
s32 oga_calc_loop_count;
s32 old_oga_calc_loop_count;
SOUKOU_ZURE_TYPE LaneData[10];
Path2D* center_line_EX;
s32 g_CourseObstacle;
TrainStuff SL[NUM_TRAINS];
u16 fumikiri_mode[NUM_CROSSINGS];
u16 fumikiri_counter[NUM_CROSSINGS];
PaddleBoatStuff ship[NUM_PADDLE_BOATS];
VehicleStuff TRUCK[NUM_RACE_BOX_TRUCKS];
VehicleStuff bus[NUM_RACE_SCHOOL_BUSES];
VehicleStuff tanklorry[NUM_RACE_TANKER_TRUCKS];
VehicleStuff rv_car[NUM_RACE_CARS];
s32 D_80163DD8[4];
BombKart bom[NUM_BOMB_KARTS_MAX];
Collision bom_bump[NUM_BOMB_KARTS_MAX];
struct unexpiredActors kame_hibasira[8];
EnemyItem enemy_item[NUM_PLAYERS];
s16 enemy_free_item_debug_flg;
s16 enemy_free_item_debug_inum;
s16 enemy_free_item_debug_num;
s16 rank_to_num[12]; // D_80164360
s16 real_rank_to_num[12];
s32 gLapCountByPlayerId[10];          // D_80164390
s32 gGPCurrentRaceRankByPlayerId[10]; // D_801643B8
s32 old_ogawa_rank[10];
s32 real_rank[10];
u16 center_line_max;
u16 gNearestPathPointByPlayerId[12];
s32 g_playerPathPointTotalTable[10];
s16 name_to_num[10];
s32 D_8016448C;
CenterPathStruct* g_pathPointPointer;
f32 cam_offset[4];
f32 rate_count_F[10];    // D_801644A8
f32 rap_count_F[10]; // D_801644D0
s16 enemy_enter_maze[12];
f32 enemy_tenuki_y[10];
s16 enemy_geta_rank[12];
CenterPathStruct* CenterPathBP[4];
CenterPathStruct* SideLPathBP[4];
CenterPathStruct* SideRPathBP[4];
s16* CurvePathDataBP[4];
s16* AnglePathDataBP[4];
s16* ShortCutPathDataBP[4];
u16 CurrentPathID[12]; // D_801645B0
u16 g_courseTotalPathPoints[4]; // D_801645C8
s32 D_801645D0[4];
s16* short_cut_data;
s32 D_801645E8[4];
f32 old_oga_cam_x[4];
s32 D_80164608[4];
f32 old_oga_cam_y[4];
s32 D_80164628[4];
f32 old_oga_cam_z[4];
f32 cam_speed[4];
f32 cam_speed_objct[4];
s16 oga_cam_area[4];
s16 gakaku_mode[4];
s16 kotei_gakaku_mode[4];
s16 ogawa_camera_mode[4];
f32 oga_camera_zure[4];
f32 oga_camera_bird_x;
f32 oga_camera_bird_y;
f32 oga_camera_bird_z;
// start padding
s32 oga_camera_bird_tx;
s32 oga_camera_bird_ty;
s32 oga_camera_bird_tz;
s32 oga_camera_bird_speed;
s32 oga_camera_bird_theta_y;
s32 oga_camera_bird_theta_my;
s32 D_801646BC;
// end padding
s16 katamuki_theta[4];
u32 oga_goal_counter;
u16 D_801646CC;
UnkStruct_46D0 oga_sound_camera[4];

// Strings, presented by google translate!
// Note that these are EUC-JP encoded, see:
// https://en.wikipedia.org/wiki/Extended_Unix_Code#EUC-JP

// Force sort immediately after goal
char* D_800EB710 = "ゴール直後の強制ソート\n";
// Forced sort immediately after one goal in 2PGP
char* D_800EB728 = "2PGPで片方がゴール直後の強制ソート\n";
// rank calculation error
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
// AREA ERR!!! (group not registered at current centerline %d) %d
char* D_800EB970 = "AREA ERR!!! (現在のセンターライン %d に未登録のグループです) %d\n";
// AREA ERR!!! (Unregistered group) %d
char* D_800EB9B4 = "AREA ERR!!! (未登録のグループです) %d\n";
// get_oga_area_sub_BP() ... Area not found! (b_num = %d)
char* D_800EB9DC = "get_oga_area_sub_BP() ... エリアが見つからないッス！ (b_num = %d)\n";
// Status: (%d, %d, %d)
char* D_800EBA20 = "  状況: (%d, %d, %d) \n";
char* D_800EBA38 = "<%d> (%d, %d, %d) [%d] lng %f\n";
// Wario Stadium Jump failed! ! ! (area %d, y %7.2f)
char* D_800EBA58 = "ワリオスタジアム  ジャンプ失敗！！！ (area %d, y %7.2f)\n";
// I fell in the water! ! Forced to centerline (num %d: area %d ) (%d,%d,%d)
char* D_800EBA94 = "水に落ちた！！  センターラインに強制移動しました (num %d: area %d ) (%d,%d,%d)\n";
// Course match! ! (Slacking: with bump) Forced move to center line (num %d: area %d ==>%d) (group %d) (%d,%d,%d)
char* D_800EBAE4 = "こーすあうと！！（手抜き中:バンプ有り）  センターラインに強制移動しました (num %d: area %d ==>%d) "
                   "(group %d) (%d,%d,%d)\n";
// Course match! ! (Sitting corners: no bump) Forced move to center line (num %d: area %d ==>%d) (group %d) (%d,%d,%d)
char* D_800EBB60 = "こーすあうと！！（手抜き中:バンプ無し）  センターラインに強制移動しました (num %d: area %d ==>%d) "
                   "(group %d) (%d,%d,%d)\n";
// Course match! ! ! Recalculated area (num %d: area %d ==>%d)
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
// Red turtle fire pillar set error! (category %d)
char* D_800EBCD8 = "赤ガメ火柱セットエラー！ (category %d)\n";
// Blue turtle fire pillar set error! (category %d)
char* D_800EBD00 = "青ガメ火柱セットエラー！ (category %d)\n";
// Thorn Turtle Fire Pillar Set Error! (category %d)
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
// Ogawa total memory used = %d
char* D_800EBEAC = "小川の使用メモリー合計 = %d\n";
// Enemy initialization
char* D_800EBECC = "敵初期化\n";
// End of enemy initialization
char* D_800EBED8 = "敵初期化終了\n";
// Bypass CENTER LINE Split start
char* D_800EBEE8 = "バイパス CENTER LINE 分割開始\n";
// Read centerline from ROM (map:%d)
char* D_800EBF08 = "センターラインをROMから読みます (map:%d)\n";
char* D_800EBF34 = "ROM center (BP%d) line adr. = %x (%x)\n";
// Calculate centerline (map:%d)
char* D_800EBF5C = "センターラインを計算します (map:%d)\n";
char* D_800EBF84 = "center (BP%d) line adr. = %x (%x)\n";
char* D_800EBFA8 = "BP center_point_number : %d\n";
// Centerline data error! !
char* D_800EBFC8 = "センターライン データ エラー！！\n";
// Bypass CENTER LINE split end (%d -> %d number)
char* D_800EBFEC = "バイパス CENTER LINE 分割終了 (%d -> %d 個)\n";
// No center line. (map: %d)
char* D_800EC01C = "センターラインが ありません。(map:%d)\n";
// side point calculation (bypass %d)
char* D_800EC044 = "サイドポイント計算 (バイパス %d)\n";
// Curve data calculation (bypass %d)
char* D_800EC068 = "カーブデータ計算 (バイパス %d)\n";
// No center line. (map: %d)
char* D_800EC088 = "センターラインが ありません。(map:%d)\n";
// Angle data calculation (bypass %d)
char* D_800EC0B0 = "アングルデータ計算 (バイパス %d) \n";
// No center line. (map: %d)
char* D_800EC0D4 = "センターラインが ありません。(map:%d)\n";
// Shortcut data calculation (bypass %d)
char* D_800EC0FC = "ショートカットデータ計算 (バイパス %d)\n";
char* D_800EC124 = "extern POINT rom_center_KT%d_BP%d[] = {\n";
char* D_800EC150 = "\t{%d,%d,%d,%d},\n";
char* D_800EC164 = "\t0x8000,0x8000,0x8000,0\n};\n\n";
char* D_800EC184 = "area read from ROM (%d)\n";
// Normal jump! ! ! (%d)
char* D_800EC1A0 = "ノーマルジャンプ！！！(%d)\n";
// Turbo on! ! ! (%d)
char* D_800EC1BC = "ターボオン！！！(%d)\n";
// No cutting corners! ! ! (%d)
char* D_800EC1D4 = "手抜き禁止！！！(%d)\n";
// Action start data error! (num %d, act %d)
char* D_800EC1EC = "アクション開始データエラー！(num %d, act %d)\n";
// Action end data error! (num %d, act %d, old_act_num %d)
char* D_800EC21C = "アクション終了データエラー！(num %d,  act %d,  old_act_num %d)\n";
char* D_800EC25C = "SL : center_point_number : %d\n";
// SL: CENTER LINE split start
char* D_800EC27C = "SL: CENTER LINE 分割開始\n";
// SL: CENTER LINE split ended (%d -> %d indivual)
char* D_800EC298 = "SL: CENTER LINE 分割終了 (%d -> %d 個)\n";
char* D_800EC2C0 = "SHIP : center_point_number : %d\n";
// SHIP: CENTER LINE split start
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
// High speed camera ERR !!! (ncx = %f)
char* D_800EC414 = "高速カメラ ERR !!! (ncx = %f)\n";
// High speed camera ERR !!! (ncz = %f)
char* D_800EC434 = "高速カメラ ERR !!! (ncz = %f)\n";
// High speed camera ERR !!! (ecx = %f)
char* D_800EC454 = "高速カメラ ERR !!! (ecx = %f)\n";
// High speed camera ERR !!! (ecz = %f)
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
// <<< BANANA SET HOUSE >>> obj_num %d zure %f
char* D_800EC5F8 = "<<< BANANA SET 失敗 >>> obj_num %d   zure %f \n";
// BANANA Caught in owner check. (num %d)
char* D_800EC628 = "BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC65C = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC670 = "理由: category \n";
// Reason: sparam
char* D_800EC684 = "理由: sparam \n";
// Reason: num
char* D_800EC694 = "理由: num \n";
char* D_800EC6A0 = "BANANA HOLD (num %d  time %d   hold_time %d)\n";
// Installation Caught in BANANA owner check. (num %d)
char* D_800EC6D0 = "設置 BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC708 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC71C = "理由: category \n";
// Reason: sparam
char* D_800EC730 = "理由: sparam \n";
// Reason: num
char* D_800EC740 = "理由: num \n";
// I put BANANA. (num %d)
char* D_800EC74C = "BANANA 置きました。 (num %d)\n";
// <<< BANANA NAGE SET failed >>> obj_num %d
char* D_800EC76C = "<<< BANANA NAGE SET 失敗 >>> obj_num %d \n";
// BANANA NAGE MOVE Caught in owner check. (num %d)
char* D_800EC798 = "BANANA NAGE MOVE 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC7D8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC7EC = "理由: category \n";
// Reason: sparam
char* D_800EC800 = "理由: sparam \n";
// Reason: num
char* D_800EC810 = "理由: num \n";
char* D_800EC81C = "BANANA NAGE END 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC858 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC86C = "理由: category \n";
// Reason: sparam
char* D_800EC880 = "理由: sparam \n";
// Reason: num
char* D_800EC890 = "理由: num \n";
char* D_800EC89C = "G_SHELL HOLD (num %d  time %d   hold_time %d)\n";
// <<< G_SHELL SET failed >>> obj_num %d
char* D_800EC8CC = "<<< G_SHELL SET 失敗 >>> obj_num %d \n";
// <<< G_SHELL SET failed >>> object_count %d
char* D_800EC8F4 = "<<< G_SHELL SET 失敗 >>> object_count %d \n";
// G_SHELL Caught in owner check. (num %d)
char* D_800EC920 = "G_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC954 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC968 = "理由: category \n";
// Reason: sparam
char* D_800EC97C = "理由: sparam \n";
// Reason: num
char* D_800EC98C = "理由: num \n";
// Just before launch G_SHELL Caught in owner check. (num %d)
char* D_800EC998 = "発射直前 G_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800EC9D8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800EC9EC = "理由: category \n";
// Reason: sparam
char* D_800ECA00 = "理由: sparam \n";
// Reason: num
char* D_800ECA10 = "理由: num \n";
// G_SHELL firing (num %d)
char* D_800ECA1C = "G_SHELL 発射 (num %d)\n";
char* D_800ECA34 = "R_SHELL HOLD (num %d  time %d   hold_time %d  obj_num %d)\n";
// <<< R_SHELL SET failed >>> obj_num %d
char* D_800ECA70 = "<<< R_SHELL SET 失敗 >>> obj_num %d \n";
// <<< R_SHELL SET failed >>> object_count %d
char* D_800ECA98 = "<<< R_SHELL SET 失敗 >>> object_count %d \n";
// R_SHELL Caught in owner check. (num %d)
char* D_800ECAC4 = "R_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECAF8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECB0C = "理由: category \n";
// Reason: sparam
char* D_800ECB20 = "理由: sparam \n";
// Reason: num
char* D_800ECB30 = "理由: num \n";
char* D_800ECB3C = "R_SHELL SHOOT (num %d  time %d   hold_time %d  obj_num %d)\n";
// Just before launch R_SHELL Caught in owner check. (num %d)
char* D_800ECB78 = "発射直前 R_SHELL 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECBB8 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECBCC = "理由: category \n";
// Reason: sparam
char* D_800ECBE0 = "理由: sparam \n";
// Reason: num
char* D_800ECBF0 = "理由: num \n";
// R_SHELL firing (num %d)
char* D_800ECBFC = "R_SHELL 発射 (num %d)\n";
char* D_800ECC14 = "S_BANANA HOLD (num %d  time %d   hold_time %d)\n";
// <<< SUPER_BANANA SET failed >>> obj_num %d
char* D_800ECC44 = "<<< SUPER_BANANA SET 失敗 >>> obj_num %d \n";
// <<< SUPER_BANANA SET failed >>> object_count %d
char* D_800ECC70 = "<<< SUPER_BANANA SET 失敗 >>> object_count %d \n";
// S_BANANA Caught in owner check. (num %d)
char* D_800ECCA0 = "S_BANANA 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: category
char* D_800ECCD8 = "理由: category \n";
// Reason: sparam
char* D_800ECCEC = "理由: sparam \n";
// Reason: sb_ok
char* D_800ECCFC = "理由: sb_ok \n";
char* D_800ECD0C = "S_BANANA RELEASE (num %d  time %d )\n";
// <<< FAKE IBOX SET failed >>> obj_num %d
char* D_800ECD34 = "<<< FAKE IBOX SET 失敗 >>> obj_num %d \n";
// IBOX Caught in owner check. (num %d)
char* D_800ECD5C = "IBOX 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECD90 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECDA4 = "理由: category \n";
// Reason: sparam
char* D_800ECDB8 = "理由: sparam \n";
// Reason: num
char* D_800ECDC8 = "理由: num \n";
char* D_800ECDD4 = "FBOX HOLD (num %d  time %d   hold_time %d)\n";
// Installation IBOX owner check failed. (num %d)
char* D_800ECE00 = "設置 IBOX 所有者チェックに引っ掛かりました。(num %d)\n";
// Reason: EXISTOBJ
char* D_800ECE38 = "理由: EXISTOBJ \n";
// Reason: category
char* D_800ECE4C = "理由: category \n";
// Reason: sparam
char* D_800ECE60 = "理由: sparam \n";
// Reason: num
char* D_800ECE70 = "理由: num \n";
// Ray START (%d)
char* D_800ECE7C = "雷START (%d)\n";
// Ray END (%d)
char* D_800ECE8C = "雷END (%d)\n";
// ---------- Initialization of commendation table
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
// 4th place finished
char* D_800ECF3C = "４位の人終了\n";
// OGA commendation move end
char* D_800ECF4C = "OGA 表彰 move 終了\n";
char* D_800ECF60 = "OGAWA DEBUG DRAW\n";

// utils function path, cpu, vehicle

s16 oga_calc_direction(Vec3f arg0, Vec3f arg1) {
    s16 temp_ret;
    s16 phi_v1;

    temp_ret = CalcDirection(arg0, arg1);
    phi_v1 = temp_ret;
    if (g_ScreenFlip != 0) {
        phi_v1 = -temp_ret;
    }
    return phi_v1;
}

bool oga_hit_check(f32 vehicleX, f32 vehicleZ, f32 vehicleVelocityX, f32 vehicleVelocityZ, f32 distanceX,
                             f32 distanceY, f32 playerX, f32 playerZ) {
    f32 velocity;
    f32 temp_f18;

    velocity = sqrtf((vehicleVelocityX * vehicleVelocityX) + (vehicleVelocityZ * vehicleVelocityZ));
    if (velocity < 0.01f) {
        return false;
    }
    temp_f18 =
        ((vehicleVelocityX / velocity) * (playerX - vehicleX)) + ((vehicleVelocityZ / velocity) * (playerZ - vehicleZ));
    if ((-distanceX < temp_f18) && (temp_f18 < distanceX)) {
        temp_f18 = ((vehicleVelocityZ / velocity) * (playerX - vehicleX)) +
                   (-(vehicleVelocityX / velocity) * (playerZ - vehicleZ));
        if ((-distanceY < temp_f18) && (temp_f18 < distanceY)) {
            return true;
        }
    }
    return false;
}

void oga_guruguru_sub(Vec3f newPos, Vec3f oldPos, s16 orientationY) {
    f32 x_dist;
    f32 z_dist;
    f32 temp1;
    f32 temp2;
    f32 sine;
    f32 cosine;

    if (g_ScreenFlip != 0) {
        orientationY = -orientationY;
    }
    x_dist = newPos[0] - oldPos[0];
    z_dist = newPos[2] - oldPos[2];
    sine = sinT(orientationY);
    cosine = cosT(orientationY);
    temp1 = ((x_dist * cosine) + (z_dist * sine));
    temp2 = ((z_dist * cosine) - (x_dist * sine));
    newPos[0] = oldPos[0] + temp1;
    newPos[2] = oldPos[2] + temp2;
}

s32 check_player_near(Vec3f vehiclePos, f32 renderDistance, s32 flags) {
    Camera* camera;
    Player* player;
    f32 x;
    f32 z;
    f32 playerX;
    f32 playerZ;
    s32 i;
    s32 flag;
    s8 numScreens;

    x = vehiclePos[0];
    z = vehiclePos[2];
    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            numScreens = 1;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            numScreens = 2;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            numScreens = g_menuMultiplayerSelection;
            break;
        default:
            numScreens = 1;
            break;
    }
    flag = flags;
    if (!g_DemoFlag) {
        player = gPlayerOne;
        // Checks distance from each player.
        for (i = 0; i < g_menuMultiplayerSelection; i++, player++) {
            if (((player->flag & IS_PLAYER) != 0) && ((player->flag & IS_CPU_PLAYER) == 0)) {
                playerX = player->position[0];
                playerZ = player->position[2];

                // Is player within render distance
                if (((playerX - renderDistance) < x) && ((playerX + renderDistance) > x) &&
                    ((playerZ - renderDistance) < z) && ((playerZ + renderDistance) > z)) {
                    // Sets the render flag to on for each player.
                    flag |= (RENDER_VEHICLE << i);
                } else {
                    // Sets the render flag to off for each player.
                    flag &= ~(RENDER_VEHICLE << i);
                }
            }
        }
    } else { // Demo cinematic uses the camera to check render distance
        camera = camera1;
        for (i = 0; i < numScreens; i++, camera++) {
            playerX = camera->camera_pos[0];
            playerZ = camera->camera_pos[2];
            if (((playerX - renderDistance) < x) && (x < (playerX + renderDistance)) &&
                ((playerZ - renderDistance) < z) && (z < (playerZ + renderDistance))) {
                flag |= (RENDER_VEHICLE << i);
            } else {
                flag &= ~(RENDER_VEHICLE << i);
            }
        }
    }
    return flag;
}

void oga_reverse_chk(s32 playerId, Player* player) {
    s16 playerAngle;
    s16 rotationDifference;
    s16 pathPointAngle;
    s16 pathIndex;
    u32 pathPoint;

    pathIndex = (s16) CurrentPathID[playerId];
    pathPoint = gNearestPathPointByPlayerId[playerId];

    playerAngle = (s16) ((s16) player->direction[1] / DEGREES(1));
    pathPointAngle = (s16) ((s16) AnglePathDataBP[pathIndex][pathPoint] / DEGREES(1));

    rotationDifference = playerAngle - pathPointAngle;

    if (rotationDifference < 0) {
        rotationDifference = -rotationDifference;
    }

    if ((g_playerPathPointTotalTable[playerId] < old_pylon_count[playerId]) &&
        (rotationDifference >= SEVERE_WRONG_DIRECTION_MIN) && (rotationDifference < SEVERE_WRONG_DIRECTION_MAX)) {
        oga_reverse_count[playerId]++;
        if ((oga_reverse_count[playerId]) >= WRONG_DIRECTION_FRAMES_LIMIT) {
            g_wrongwayFlagPlayer1[playerId] = 1;
            oga_reverse_count[playerId] = WRONG_DIRECTION_FRAMES_LIMIT;
            gPlayers[playerId].slip_flag |= REVERSE;
        }
    } else if ((rotationDifference < SEVERE_CORRECT_DIRECTION_MIN) ||
               (rotationDifference >= SEVERE_CORRECT_DIRECTION_MAX)) {
        g_wrongwayFlagPlayer1[playerId] = 0;
        oga_reverse_count[playerId] = 0;
        gPlayers[playerId].slip_flag &= ~REVERSE;
    }
    old_pylon_count[playerId] = g_playerPathPointTotalTable[playerId];
}

void sort_ogawa_rank(void) {
    s32 playerIdToSwap;
    f32 courseCompletionToSwap;
    s32 playerIdByRank[8];
    s32 prevPlayerId;
    UNUSED s32 pad;
    s32 numPlayer;
    s32 rank;
    s32 playerId;
    s32 rankHigh;

    switch (g_gameMode) {
        case BATTLE:
        default:
            return; // HEY! returns, not breaks
        case GRAND_PRIX:
        case TIME_TRIALS:
            numPlayer = NUM_PLAYERS;
            break;
        case VERSUS:
            numPlayer = g_menuMultiplayerSelection;
            break;
    }

    if (g_rankUpdateFinishFlag == 0) {
        for (rank = 0; rank < numPlayer; rank++) {
            playerId = rank_to_num[rank];
            playerIdByRank[rank] = playerId;
            rap_dat[rank] = rap_count_F[playerId];
        }
    } else {
        // used in grand Prix mode once all players cross finish line to determine finish order
        for (rank = 0; rank < numPlayer; rank++) {
            playerId = rank_to_num[rank];
            playerIdByRank[rank] = playerId;
            rap_dat[rank] = -gTimePlayerLastTouchedFinishLine[playerId];
        }
    }

    for (rank = 0; rank < numPlayer - 1; rank++) {
        if ((gPlayers[rank_to_num[rank]].flag & IS_RACE_FINISH)) {
            continue;
        }

        for (rankHigh = rank + 1; rankHigh < numPlayer; rankHigh++) {
            if (rap_dat[rank] < rap_dat[rankHigh]) {
                if (!(gPlayers[rank_to_num[rankHigh]].flag & IS_RACE_FINISH)) {
                    playerIdToSwap = playerIdByRank[rank];
                    playerIdByRank[rank] = playerIdByRank[rankHigh];
                    playerIdByRank[rankHigh] = playerIdToSwap;
                    courseCompletionToSwap = rap_dat[rank];
                    rap_dat[rank] = rap_dat[rankHigh];
                    rap_dat[rankHigh] = courseCompletionToSwap;
                }
            }
        }
    }

    // actually player_id, not rank
    for (rank = 0; rank < NUM_PLAYERS; rank++) {
        old_ogawa_rank[rank] = gGPCurrentRaceRankByPlayerId[rank];
    }

    for (rank = 0; rank < numPlayer; rank++) {
        rank_to_num[rank] = playerIdByRank[rank];
        gGPCurrentRaceRankByPlayerId[playerIdByRank[rank]] = rank;
    }
    for (rank = 0; rank < numPlayer; rank++) {
        prevPlayerId = real_rank_to_num[rank];
        playerIdByRank[rank] = prevPlayerId;
        rap_dat[rank] = rap_count_F[prevPlayerId];
    }

    for (rank = 0; rank < numPlayer - 1; rank++) {
        for (rankHigh = rank + 1; rankHigh < numPlayer; rankHigh++) {
            if (rap_dat[rank] < rap_dat[rankHigh]) {
                playerIdToSwap = playerIdByRank[rank];
                playerIdByRank[rank] = playerIdByRank[rankHigh];
                playerIdByRank[rankHigh] = playerIdToSwap;
                courseCompletionToSwap = rap_dat[rank];
                rap_dat[rank] = rap_dat[rankHigh];
                rap_dat[rankHigh] = courseCompletionToSwap;
            }
        }
    }

    for (rank = 0; rank < numPlayer; rank++) {
        real_rank[playerIdByRank[rank]] = rank;
        real_rank_to_num[rank] = playerIdByRank[rank];
    }
}

void sort_goal_rank(void) {
    f32 temp_f0;
    UNUSED s32 pad;
    s32 playerIds[8];
    s32 temp_a0;
    s32 temp_t2_2;
    s32 compareIndex;
    s32 i;
    s32 numRacers;

    switch (g_gameMode) {
        case BATTLE:
        default:
            return; // HEY! returns, not breaks
        case GRAND_PRIX:
        case TIME_TRIALS:
            numRacers = 8;
            break;
        case VERSUS:
            numRacers = g_menuMultiplayerSelection;
            break;
    }

    for (i = 0; i < numRacers; i++) {
        temp_a0 = rank_to_num[i];
        playerIds[i] = temp_a0;
        rap_dat[i] = -gTimePlayerLastTouchedFinishLine[temp_a0];
    }

    for (i = 0; i < (numRacers - 1); i++) {
        for (compareIndex = i + 1; compareIndex < numRacers; compareIndex++) {
            if (rap_dat[i] < rap_dat[compareIndex]) {
                temp_t2_2 = playerIds[i];
                playerIds[i] = playerIds[compareIndex];
                playerIds[compareIndex] = temp_t2_2;
                temp_f0 = rap_dat[i];
                rap_dat[i] = rap_dat[compareIndex];
                rap_dat[compareIndex] = temp_f0;
            }
        }
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        old_ogawa_rank[i] = gGPCurrentRaceRankByPlayerId[i];
    }

    for (i = 0; i < numRacers; i++) {
        gGPCurrentRaceRankByPlayerId[playerIds[i]] = i;
        rank_to_num[i] = playerIds[i];
    }
}

void sort_goal_rank_2PGP(void) {
    f32 temp_a0;
    s32 temp;
    s32 sp68[8];
    UNUSED s32 pad;
    s32 temp_t1;
    s32 i;
    s32 j;
    s32 this_loops_upper_bound_is_brough_to_you_by_the_number = 8;

    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number;) {
        rap_dat[i++] = 0.0f;
    }

    for (j = 0, i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        if (gPlayers[i].flag & IS_RACE_FINISH) {
            sp68[j] = i;
            rap_dat[j] = -gTimePlayerLastTouchedFinishLine[i];
            j++;
        }
    }

    temp_t1 = j;
    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        if (!(gPlayers[i].flag & IS_RACE_FINISH)) {
            sp68[j] = i;
            rap_dat[j] = rap_count_F[i];
            j++;
        }
    }

    for (i = 0; i < (temp_t1 - 1); i++) {
        for (j = i + 1; j < temp_t1; j++) {
            if (rap_dat[i] < rap_dat[j]) {
                temp = sp68[i];
                sp68[i] = sp68[j];
                sp68[j] = temp;
                temp_a0 = rap_dat[i];
                rap_dat[i] = rap_dat[j];
                rap_dat[j] = temp_a0;
            }
        }
    }

    for (i = temp_t1; i < (this_loops_upper_bound_is_brough_to_you_by_the_number - 1); i++) {
        for (j = i + 1; j < this_loops_upper_bound_is_brough_to_you_by_the_number; j++) {
            if (rap_dat[i] < rap_dat[j]) {
                temp = sp68[i];
                sp68[i] = sp68[j];
                sp68[j] = temp;
                temp_a0 = rap_dat[i];
                rap_dat[i] = rap_dat[j];
                rap_dat[j] = temp_a0;
            }
        }
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        old_ogawa_rank[i] = gGPCurrentRaceRankByPlayerId[i];
    }

    for (i = 0; i < this_loops_upper_bound_is_brough_to_you_by_the_number; i++) {
        gGPCurrentRaceRankByPlayerId[sp68[i]] = i;
        rank_to_num[i] = sp68[i];
    }
}

/**
 * Checks if a path point is within a valid range of another path point, accounting for track wrapping
 *
 * @param pathPoint The path point to check
 * @param currentPathPoint The reference path point
 * @param backwardRange Number of path to look behind
 * @param forwardRange Number of path to look ahead
 * @param totalPathPoints Total number of path in the track
 * @return
 *   1: path point is within normal range
 *  -1: path point is within wrapped range
 *   2: path point is out of range
 *   0: invalid range parameters
 */
s32 exist_area_chk(u16 pathPoint, u16 currentPathPoint, u16 backwardRange, u16 forwardRange,
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

bool make_kart_distance(s32 playerId, s16 arg1, s16 arg2) {
    Player* player;
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
    s16 rank;

    enemy_dist[playerId] = -1;
    enemy_limdist[playerId] = -1;
    if (g_gameMode == TIME_TRIALS) {
        return 1;
    }
    if (arg1 < 0) {
        return 1;
    }
    if (arg1 >= 4) {
        arg1 = 3;
    }
    if (oga_car_stat[playerId] == 1) {
        return 1;
    }
    player = &gPlayers[playerId];
    if (player->flag & IS_PLAYER) {
        return 1;
    }

    temp_a3 = &GET_COURSE_800DCBB4(arg1 * 8);
    if (arg2 == 0) {
        if (g_DemoFlag == 1) {
            STEMP_V0 = g_playerPathPointTotalTable[playerId];
            STEMP_V1 = g_playerPathPointTotalTable[real_rank_to_num[7]];
            progress = STEMP_V0 - STEMP_V1;
            if (progress < 0) {
                progress = -progress;
            }
            if (arg1 < 3) {
                STEMP_V0 = temp_a3[0];
                STEMP_V1 = temp_a3[8];
                temp_f0 = rate_count_F[playerId];
                var_a0 = (STEMP_V1 * temp_f0) + (STEMP_V0 * (1.0f - temp_f0));
            } else {
                var_a0 = temp_a3[0];
            }
            enemy_limdist[playerId] = var_a0;
            if ((enemy_limdist[playerId] < progress) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
                return 0;
            }
            return 1;
        }

        STEMP_V0 = g_playerPathPointTotalTable[playerId];
        STEMP_V1 = g_playerPathPointTotalTable[g_EnemyTargetPlayer];
        progress = STEMP_V0 - STEMP_V1;
        rank = gGPCurrentRaceRankByPlayerId[g_EnemyTargetPlayer];
        if (((((g_courseTotalPathPoints[0] * 2) / 3)) < progress) && ((rank) >= 6)) {
            STEMP_V0 = g_playerPathPointTotalTable[playerId];
            STEMP_V1 = temp = g_playerPathPointTotalTable[real_rank_to_num[rank - 1]];
            progress = STEMP_V0 - STEMP_V1;
        }
        if (progress < 0) {
            progress = -progress;
        }
        if (arg1 < 3) {
            STEMP_V0 = temp_a3[0];
            STEMP_V1 = temp_a3[8];
            temp_f0 = rate_count_F[playerId];
            var_a0 = (STEMP_V1 * temp_f0) + (STEMP_V0 * (1.0f - temp_f0));
        } else {
            var_a0 = temp_a3[0];
        }
        enemy_limdist[playerId] = var_a0 = (g_raceClass + 1) * var_a0;
        if ((var_a0 < progress) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            return 0;
        }
        return 1;
    }

    temp2 = enemy_rival_num[0];
    STEMP_V0 = g_playerPathPointTotalTable[playerId];
    STEMP_V1 = g_playerPathPointTotalTable[temp2];
    progress = STEMP_V1 - STEMP_V0;
    if (progress < 0) {
        progress = -progress;
    }

    var_v0 = 0;
    for (i = 0; i < 2; i++) {
        if (gGPCurrentRaceRankByPlayerId[enemy_rival_num[i] & 0xFFFF] < arg2) {
            var_v0++;
        }
    }

    var_a0_4 = 0;
    for (i = 0; i < g_menuMultiplayerSelection; i++) {
        if (gGPCurrentRaceRankByPlayerId[i] < arg2) {
            var_a0_4++;
        }
    }

    // FAKE
    var_t1 = (arg2 - (var_v0 & 0xFFFF)) - var_a0_4;
    arg2 -= var_v0;

    if ((var_v0 > 0) || (var_a0_4 > 0)) {
        var_t1++;
    }
    enemy_geta_rank[playerId] = var_t1;
    if ((var_t1 < 0) || (var_t1 >= 8)) {
        return 0;
    }
    if (arg1 < 3) {
        STEMP_V0 = temp_a3[var_t1 + 0];
        STEMP_V1 = temp_a3[var_t1 + 8];
        temp_f0 = rate_count_F[playerId];
        var_a0 = (STEMP_V1 * temp_f0) + (STEMP_V0 * (1.0f - temp_f0));
    } else {
        var_a0 = temp_a3[var_t1];
    }
    enemy_dist[playerId] = progress;
    enemy_limdist[playerId] = var_a0;
    if (var_a0 < progress) {
        return 1;
    }
    return 0;
}

void new_bipas_num_init(s32 pathIndex) {
    g_pathPointPointer = CenterPathBP[pathIndex];
    side_point_L = SideLPathBP[pathIndex];
    side_point_R = SideRPathBP[pathIndex];
    curve_data = CurvePathDataBP[pathIndex];
    angle_data = AnglePathDataBP[pathIndex];
    short_cut_data = ShortCutPathDataBP[pathIndex];
    center_line_max = g_courseTotalPathPoints[pathIndex];
}

s32 kyousei_bipas_set(s32 payerId, s32 pathIndex) {
    f32 posX;
    f32 posY;
    f32 posZ;
    Player* player;
    s32 trackSectionId;
    UNUSED s32 stackPadding;

    player = &gPlayers[payerId];
    posX = player->position[0];
    posY = player->position[1];
    posZ = player->position[2];
    stackPadding = pathIndex;
    trackSectionId = CheckArea(player->bump.last_zx);
    g_lakituSurface[payerId] = trackSectionId;
    OGAArea = CalcOGAAreaSubBP_Wrapper(posX, posY, posZ, trackSectionId, &pathIndex);
    gNearestPathPointByPlayerId[payerId] = OGAArea;
    if (pathIndex) {};
    CurrentPathID[payerId] = pathIndex;
    new_bipas_num_init(pathIndex);
    if (stackPadding) {};
    return pathIndex;
}

void pylon_count_reflesh(s32 playerId) {
    f32 percent;

    // arbitrary score calculation
    g_playerPathPointTotalTable[playerId] =
        (gLapCountByPlayerId[playerId] * g_courseTotalPathPoints[0]) + OGAArea;

    // calculate completion in percent
    percent = (f32) gNearestPathPointByPlayerId[playerId] / (f32) g_courseTotalPathPoints[CurrentPathID[playerId]];
    rate_count_F[playerId] = percent;
    rap_count_F[playerId] = percent;
    rap_count_F[playerId] += gLapCountByPlayerId[playerId];
}

void maze_enter_or_exit_check(s32 playerId) {
    s16 previous;

    previous = enemy_enter_maze[playerId];
    if (OGAArea >= 0x6D) {
        enemy_enter_maze[playerId] = true;
        switch (bipas_num) {
            case 0:
                if (OGAArea >= 0x20F) {
                    enemy_enter_maze[playerId] = false;
                }
                break;
            case 1:
                if (OGAArea >= 0x206) {
                    enemy_enter_maze[playerId] = false;
                }
                break;
            case 2:
                if (OGAArea >= 0x211) {
                    enemy_enter_maze[playerId] = false;
                }
                break;
            case 3:
                if (OGAArea >= 0x283) {
                    enemy_enter_maze[playerId] = false;
                }
                break;
        }
    }
    if ((previous == false) && (enemy_enter_maze[playerId] == true)) {
        enemy_enter_trg[playerId] = true;
    }
    if ((previous == true) && (enemy_enter_maze[playerId] == false)) {
        enemy_exit_trg[playerId] = true;
    }
}

void maze_bipas_change_check(s32 playerId, Player* player) {
    f32 posX;
    f32 posY;
    f32 posZ;
    UNUSED f32 pad[2];

    posX = player->position[0];
    posY = player->position[1];
    posZ = player->position[2];
    if (enemy_enter_trg[playerId] == 1) {
        bipas_num = kyousei_bipas_set(playerId, MakeRandomLimmit(4));
        OGAArea = get_oga_area_kart(posX, posY, posZ, 0, player, playerId, bipas_num);
        gNearestPathPointByPlayerId[playerId] = OGAArea;
        pylon_count_reflesh(playerId);
        enemy_enter_trg[playerId] = 0;
    }
    if (enemy_exit_trg[playerId] == 1) {
        bipas_num = kyousei_bipas_set(playerId, 0);
        OGAArea = get_oga_area_kart(posX, posY, posZ, 0, player, playerId, bipas_num);
        gNearestPathPointByPlayerId[playerId] = OGAArea;
        pylon_count_reflesh(playerId);
        enemy_exit_trg[playerId] = 0;
    }
}

/**
 * Helps calculate time since player last touched finishline.
 * Assumes constant z-speed and subtracts portion of frame where the finish line was already crossed
 **/
f32 oga_get_laptime(UNUSED s32 playerId, f32 previousPlayerZ, f32 playerZ) {
    f32 z_change_after_cross = g_finishLineZ - playerZ;
    f32 z_change_before_cross = previousPlayerZ - g_finishLineZ;
    return g_gameTimer - ((COURSE_TIMER_ITER_f * z_change_after_cross) / (z_change_after_cross + z_change_before_cross));
}

void CheckLapCount(s32 playerId, Player* player) {
    f32 playerX;
    f32 playerY;
    f32 playerZ;
    s32 var_v0;
    UNUSED s16 pad;
    f32 previousPlayerZ;

    playerX = player->position[0];
    playerY = player->position[1];
    playerZ = player->position[2];
    previousPlayerZ = oga_old_rap_chk_z[playerId];
    oga_area_chg_flg = false;
    rap_count_flg[playerId] = 0;
    OGAArea = get_oga_area_kart(playerX, playerY, playerZ, gNearestPathPointByPlayerId[playerId], player,
                                               playerId, bipas_num);
    oga_area_org = OGAArea;
    if (gNearestPathPointByPlayerId[playerId] != OGAArea) {
        gNearestPathPointByPlayerId[playerId] = OGAArea;
        oga_area_chg_flg = true;
        pylon_count_reflesh(playerId);
    }
    if (g_courseID == COURSE_AWARD_CEREMONY) {
        zure_chk_BP(playerId, OGAArea, bipas_num);
        return;
    }
    if ((OGAArea < 0x14) || ((g_courseTotalPathPoints[bipas_num] - 0x14) < OGAArea) ||
        (g_courseID == COURSE_KALAMARI_DESERT)) {
        s16 var_v1 = 0;
        s16 var_t0 = 0;
        if (g_courseID == COURSE_KALAMARI_DESERT) {
            ogawa_karakara_short_cut_ok_flg = 0;
            if (player->slip_flag & STAR) {
                ogawa_karakara_short_cut_ok_flg = 1;
            }
            if (g_ScreenFlip != 0) {
                if (playerX < 300.0f) {
                    var_v1 = 1;
                    var_t0 = 1;
                } else if ((playerX < 1300.0f) && (gLapCountByPlayerId[playerId] < 2) && ((ogawa_karakara_short_cut_ok_flg == 1))) {
                    var_v1 = 1;
                    var_t0 = 1;
                }
            } else {
                if (playerX > -300.0f) {
                    var_v1 = 1;
                    var_t0 = 1;
                } else {
                    if ((playerX > -1300.0f) && (gLapCountByPlayerId[playerId] < 2) && (ogawa_karakara_short_cut_ok_flg == 1)) {
                        var_v1 = 1;
                        var_t0 = 1;
                    }
                }
            }
        } else {
            var_v1 = 1;
            var_t0 = 1;
        }
        previousPlayerZ = oga_old_rap_chk_z[playerId];
        if ((var_v1 != 0) && (playerZ <= g_finishLineZ)) {
            if (g_finishLineZ < previousPlayerZ) {
                gLapCountByPlayerId[playerId]++;
                if ((g_gameMode == GRAND_PRIX) && (gLapCountByPlayerId[playerId] == 5)) {
                    if (real_rank[playerId] == 7) {
                        // clang-format off
                        for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) { gLapCountByPlayerId[var_v0]--; } // has to be one line to match
                        // clang-format on
                    }
                }
                rap_count_flg[playerId] = 1;
                pylon_count_reflesh(playerId);
                action_flg_wakeup(playerId);
                enemy_item[playerId].limit = 0;
                if ((g_rankUpdateFinishFlag == 0) && !(player->flag & IS_RACE_FINISH)) {
                    gTimePlayerLastTouchedFinishLine[playerId] = oga_get_laptime(playerId, previousPlayerZ, playerZ);
                }
            }
        }
        if ((var_t0 != 0) && (previousPlayerZ <= g_finishLineZ) && (g_finishLineZ < playerZ)) {
            gLapCountByPlayerId[playerId]--;
            pylon_count_reflesh(playerId);
        }
    }
    oga_old_rap_chk_z[playerId] = playerZ;
    if ((g_courseID == COURSE_YOSHI_VALLEY) && (oga_area_chg_flg == true)) {
        maze_enter_or_exit_check(playerId);
        if (((player->flag & IS_PLAYER) == 0) || (player->flag & IS_CPU_PLAYER)) {
            maze_bipas_change_check(playerId, player);
        }
    }
    if ((player->flag & IS_PLAYER) && !(player->flag & IS_CPU_PLAYER)) {
        oga_reverse_chk(playerId, player);
        if ((g_gameMode == GRAND_PRIX) && (g_menuMultiplayerSelection == 2) && (playerId == 0)) {
            if (real_rank[PLAYER_ONE] < real_rank[PLAYER_TWO]) {
                g_EnemyTargetPlayer = PLAYER_ONE;
            } else {
                g_EnemyTargetPlayer = PLAYER_TWO;
            }
        }
    } else {
        //????
    }
    zure_chk_BP(playerId, OGAArea, bipas_num);
}

void EtcEnemyDrive(void) {
    s32 i;
    kame_hibasira_move();
    oga_etc_frame_counter++;

    if (g_courseID == COURSE_AWARD_CEREMONY) {
        for (i = 0; i < 7; i++) {
            move_BOM(i);
        }
        return;
    }

    // EtcEnemyDrive is ran twice per frame.
    // This makes vehicles only tick once per frame.
    if (oga_etc_frame_counter & 1) {
        if (g_gameMode == VERSUS) {
            for (i = 0; i < 7; i++) {
                move_BOM(i);
            }
        }
#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (g_courseID) {
            case COURSE_KALAMARI_DESERT:
                move_SL();
                break;
            case COURSE_DK_JUNGLE:
                move_SHIP();
                break;
            case COURSE_TOADS_TURNPIKE:
                move_TRUCK();
                move_BUS();
                move_TANK();
                move_RV();
                break;
        }
#else

#endif
    }
}

void oga_enemy_voice(s32 arg0, Player* player) {
    if (enemy_voice_count_SPIN[arg0] >= 0xB) {
        if ((player->slip_flag & SPIN_L) || (player->slip_flag & SPIN_R) ||
            (player->slip_flag & THUNDER_SPIN)) {
            Na_EnmVoice_Start(arg0, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0A));
            enemy_voice_count_SPIN[arg0] = 0;
        }
    }
    if (enemy_voice_count_DAMAGE[arg0] >= 0xB) {
        if ((player->weapon & HIT_BOMB) || (player->weapon & HIT_BOMB_THROW) ||
            (player->weapon & HIT_REDSHELL) || (player->weapon & HIT_GREENSHELL) ||
            (player->slip_flag & BROKEN)) {
            Na_EnmVoice_Start(arg0, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0B));
            enemy_voice_count_DAMAGE[arg0] = 0;
        }
    }
}

void oga_enemy_voice_overtake(s32 playerId, UNUSED Player* unused) {
    s32 otherPlayerId;

    if (enemy_voice_count_OVERTAKE[playerId] >= 0x65) {
        for (otherPlayerId = 0; otherPlayerId < g_menuMultiplayerSelection; otherPlayerId++) {
            // detect swap of positions
            if ((gGPCurrentRaceRankByPlayerId[playerId] < gGPCurrentRaceRankByPlayerId[otherPlayerId]) &&
                (gGPCurrentRaceRankByPlayerId[playerId] == old_ogawa_rank[otherPlayerId]) &&
                (gGPCurrentRaceRankByPlayerId[otherPlayerId] == old_ogawa_rank[playerId])) {
                Na_EnmVoice_Start(playerId, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                enemy_voice_count_OVERTAKE[playerId] = 0;
            }
        }
    }
    if (enemy_voice_count_OVERTAKE[playerId] < 0x3E8) {
        enemy_voice_count_OVERTAKE[playerId]++;
    }
    if (enemy_voice_count_SPIN[playerId] < 0xC8) {
        enemy_voice_count_SPIN[playerId]++;
    }
    if (enemy_voice_count_DAMAGE[playerId] < 0xC8) {
        enemy_voice_count_DAMAGE[playerId]++;
    }
}

void drive_enemy_kart(s32 playerId) {
    UNUSED s32 pad[14];
    s16 var_a0_2;
    s16 newAngle;
    u16 pathIndex;

    f32 distX;
    f32 minAngle;

    s16 angle;
    s16 steeringSensitivity;


    s32 maxAngle;
    Player* player;
    UNUSED s32 pad3[10];
    CenterPathStruct* pathPoint;
    f32 onePointFive = 1.5f;

    player = &gPlayers[playerId];
    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        enemy_time_count[playerId] += 1;
        if (playerId == 0) {
            oga_frame_counter++;
            if (oga_frame_counter & 1) {
                oga_bump_chk_counter += 1;
            }
        }
        if (!(player->flag & EXISTS)) {
            g_playerPathPointTotalTable[playerId] = -0x00000014;
            rap_count_F[playerId] = -1000.0f;
            rate_count_F[playerId] = -1000.0f;
            return;
        }
        oga_hamidasi_map_flg[playerId] = 0;
        // clang-format off
        if (player->position[0] < minimum_x) {            oga_hamidasi_map_flg[playerId] = 1;        }
        if (maximum_x < player->position[0]) {            oga_hamidasi_map_flg[playerId] = 2;        }
        if (player->position[2] < minimum_z) {            oga_hamidasi_map_flg[playerId] = 3;        }
        if (maximum_z < player->position[2]) {            oga_hamidasi_map_flg[playerId] = 4;        }
        // clang-format on

        if (!(player->jugemu_flag & ON_LAKITU_ROD) && !(player->jugemu_flag & LAKITU_SCENE)) {
            bipas_num = CurrentPathID[playerId];
            new_bipas_num_init(bipas_num);
            switch (g_courseID) { /* irregular */
                case COURSE_KALAMARI_DESERT:
                    hit_check_SL(playerId, player);
                    if (playerId == 0) {
                        fumikiri_move();
                    }
                    break;
                case COURSE_DK_JUNGLE:
                    hit_check_SHIP(player);
                    break;
                case COURSE_TOADS_TURNPIKE:
                    hit_check_TRUCK(playerId, player);
                    hit_check_BUS(playerId, player);
                    hit_check_TANK(playerId, player);
                    hit_check_RV(playerId, player);
                    break;
            }
            if (player->flag & IS_RACE_FINISH) {
                player->slip_flag &= ~REVERSE;
                player->handling_flag &= ~REVERSE_GEAR;
            }
            SprintLapCheck(playerId, player);

            // Because this takes place in a per-player loop and runs primarily when player 1 crosses the finish line,
            // player 1 effectively has a 1 frame advantage when determining places. However, it runs for everyone when
            // they cross the finish line, resulting in lower port numbers always winning ties in versus mode.
            if ((g_courseID != COURSE_AWARD_CEREMONY) && ((rap_count_flg[playerId] == 1) || (playerId == 0))) {
                sort_ogawa_rank();
            }
            if (player->flag & IS_CPU_PLAYER) {
                if ((oga_area_chg_flg == true) && (g_courseID != COURSE_AWARD_CEREMONY)) {
                    action_start_end_chk(playerId);
                }
                // one update it try to use an item, the other it doesn't
                if ((playerId & 1) != (oga_frame_counter & 1)) {
                    enemy_item_move(playerId);
                }
                oga_enemy_voice_overtake(playerId, player);
                biri_flg = 0;
                switch (g_gameMode) { /* switch 1; irregular */
                    case 1:               /* switch 1 */
                    case 2:               /* switch 1 */
                    case 3:               /* switch 1 */
                        break;
                    case 0: /* switch 1 */
                        break;
                }
                tenuki_flg[playerId] = false;
                if ((player->slip_flag & CENTER_LINE) && (g_courseID != COURSE_AWARD_CEREMONY)) {
                    tenuki_flg[playerId] = true;
                }
                if ((D_801646CC == 1) || (player->flag & IS_RACE_FINISH) ||
                    (g_courseID == COURSE_AWARD_CEREMONY)) {
                    if (g_courseID != COURSE_TOADS_TURNPIKE) {
                        LaneData[playerId].lane_target = 0.0f;
                    }
                    LaneData[playerId].lane_free_drive = 0.0f;
                }
                if (bipas_num > 0) {
                    LaneData[playerId].lane_target = 0.0f;
                    LaneData[playerId].lane_free_drive = 0.0f;
                }
                enemy_tenuki_y[playerId] =
                    CenterPathBP[bipas_num][gNearestPathPointByPlayerId[playerId]].pointy + 4.3f;
                if ((old_tenuki_flg[playerId] == 1) && (tenuki_flg[playerId] == false)) {
                    set_hight(player);
                }
                if (tenuki_flg[playerId] == true) {
                    player->position[1] = enemy_tenuki_y[playerId];
                }
                old_tenuki_flg[playerId] = tenuki_flg[playerId];
                switch (g_courseID) {
                    case COURSE_YOSHI_VALLEY:
                    case COURSE_AWARD_CEREMONY:
                        LaneData[playerId].lane_target = 0.0f;
                        break;
                    default:
                        break;
                    case COURSE_TOADS_TURNPIKE:
                        avoid_TRUCK(playerId);
                        avoid_BUS(playerId);
                        avoid_TANK(playerId);
                        avoid_RV(playerId);
                        break;
                }
                if (tenuki_flg[playerId] == true) {
                    g_driftFlg[playerId] = 0;
                    player->slip_flag &= ~DRIFT;
                    if ((playerId & 1) != (oga_frame_counter & 1)) {
                        enemy_stick_angle(player, 0);
                        oga_accele(playerId, old_limmitspeed[playerId], player);
                        return;
                    }
                    if ((g_menuMultiplayerSelection > 0) && (g_menuMultiplayerSelection < 3) && (oga_car_stat[playerId] == 1) &&
                        (enemy_rival_aim_rank[playerId] < gGPCurrentRaceRankByPlayerId[playerId])) {
                        old_limmitspeed[playerId] = 8.333333f;
                    } else if (biri_flg == (s16) 1U) {
                        old_limmitspeed[playerId] = GET_COURSE_D_0D0096B8(g_raceClass);
                        LaneData[playerId].lane_target = -0.5f;
                    } else if (short_cut_data[OGAArea] > 0) {
                        old_limmitspeed[playerId] = GET_COURSE_cpu_CurveTargetSpeed(g_raceClass);
                    } else {
                        old_limmitspeed[playerId] = GET_COURSE_cpu_NormalTargetSpeed(g_raceClass);
                    }
                    fumikiri_wait_check(playerId);
                    soukou_zure_move(playerId);
                    mokuhyou_set_center(playerId, oga_area_org);
                    distX = object[0] - player->position[0];
                    minAngle = object[2] - player->position[2];
                    if (!(player->slip_flag & SPIN_L) && !(player->slip_flag & SPIN_R) &&
                        !(player->slip_flag & WHEELSPIN)) {
                        if (((distX * distX) + (minAngle * minAngle)) > 6400.0f) {
                            if (bipas_num == 0) {
                                avoid_crash(playerId);
                                if (avoid_mode[playerId] > 0) {
                                    pathIndex = oga_area_org + 5;
                                    pathIndex %= center_line_max;
                                    obj_calc_from_zure_BP(pathIndex, zure_dist[playerId], bipas_num);
                                }
                            }
                            player->direction[1] = -CalcDirection(player->position, object);
                        } else {
                            player->direction[1] =
                                AnglePathDataBP[bipas_num]
                                                     [(oga_area_org + 4) % center_line_max];
                        }
                    }
                    enemy_stick_angle(player, 0);
                    oga_accele(playerId, old_limmitspeed[playerId], player);
                    return;
                }
                if ((g_driftFlg[playerId] == 1) || (g_driftFlg[playerId] == -1)) {
                    player->slip_flag |= DRIFT;
                }
                if (g_driftFlg[playerId] != 0) {
                    oga_angle[playerId] = -CalcDirection(player->old_position, player->position);
                    var_a0_2 =
                        (angle_data[(OGAArea + 2) % center_line_max] *
                         0x168) /
                        65535;
                    newAngle = (oga_angle[playerId] * 0x168) / 65535;
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
                    switch (g_driftFlg[playerId]) {
                        case -1:
                            if (steeringSensitivity > 5) {
                                g_driftFlg[playerId] = 0;
                                player->slip_flag &= ~DRIFT;
                            }
                            break;
                        case 1:
                            if (steeringSensitivity < -5) {
                                g_driftFlg[playerId] = 0;
                                player->slip_flag &= ~DRIFT;
                            }
                            break;
                        default:
                            break;
                    }
                }

                if ((playerId & 1) != (oga_frame_counter & 1)) {
                    enemy_stick_angle(player, old_eos_angle_oga[playerId]);
                    oga_accele(playerId, old_limmitspeed[playerId], player);
                    return;
                }
                straight_flg[playerId] = straight_chk(playerId, OGAArea);
                mokuhyou_set_center(playerId, OGAArea);
                if (g_courseID != COURSE_AWARD_CEREMONY) {
                    if (g_playerPathPointTotalTable[playerId] < 0xB) {
                        pathIndex = oga_area_org;
                        if ((g_playerPathPointTotalTable[playerId] > 0) && (g_courseID == COURSE_TOADS_TURNPIKE)) {
                            pathIndex += 0x14;
                            pathIndex %= center_line_max;
                            obj_calc_from_zure_BP(pathIndex, 0.0f, 0);
                            LaneData[playerId].lane_target = 0.0f;
                        } else {
                            pathIndex += 8;
                            pathIndex %= center_line_max;
                            obj_calc_from_zure_BP(pathIndex, zure_data[playerId], bipas_num);
                            LaneData[playerId].lane = zure_data[playerId];
                        }
                    }
                    if ((biri_flg == 1) && (avoid_mode[playerId] == 0)) {
                        pathIndex = oga_area_org + 7;
                        pathIndex %= center_line_max;
                        obj_calc_from_zure_BP(pathIndex, -0.7f, bipas_num);
                    }
                    if (1) { } if (1) { } if (1) { } if (1) { } if (1) { } if (1) { }
                    if (bipas_num == 0) {
                        avoid_crash(playerId);
                        if (avoid_mode[playerId] > 0) {
                            pathIndex = oga_area_org + 5;

                            pathIndex %= center_line_max;
                            obj_calc_from_zure_BP(pathIndex, zure_dist[playerId], bipas_num);
                        }
                    }
                }
                if (g_courseID == COURSE_AWARD_CEREMONY) {
                    switch (oga_near_hyousyou_flg[playerId]) { /* switch 3; irregular */
                        case 3:                     /* switch 3 */
                            object[0] = oga_hyousyou_mx[playerId];
                            object[2] = oga_hyousyou_mz[playerId];
                            break;
                        case 4: /* switch 3 */
                            pathPoint = &CenterPathBP[playerId][(gNearestPathPointByPlayerId[playerId] + 0xA) %
                                                               g_courseTotalPathPoints[playerId]];
                            object[0] = pathPoint->pointx;
                            object[2] = pathPoint->pointz;
                            break;
                    }
                }
                object[0] = (enemy_object_x[playerId] + object[0]) * 0.5f; // average
                object[2] = (enemy_object_z[playerId] + object[2]) * 0.5f; // average
                enemy_object_x[playerId] = object[0];
                enemy_object_z[playerId] = object[2];
                minAngle = onePointFive * (f32) DEGREES(1);
                maxAngle = -onePointFive * (f32) DEGREES(1);

                angle = -CalcDirection(player->position, object);
                angle -= (newAngle = player->direction[1]);
                if ((s16) minAngle < angle) {
                    angle = minAngle;
                }
                if (angle < (s16) maxAngle) {
                    angle = maxAngle;
                }
                steeringSensitivity = GET_COURSE_AISteeringSensitivity;
                switch (curve_data[playerId]) { /* switch 4; irregular */
                    case RIGHT_CURVE:                              /* switch 4 */
                        if (zure_data[playerId] > (0.5f * 1.0f)) {
                            steeringSensitivity = 0x0014;
                        }
                        if (zure_data[playerId] < -0.5f) {
                            steeringSensitivity = 0x0035;
                        }
                        break;
                    case LEFT_CURVE: /* switch 4 */
                        if (zure_data[playerId] > 0.5f) {
                            steeringSensitivity = 0x0035;
                        }
                        if (zure_data[playerId] < -0.5f) {
                            steeringSensitivity = 0x0014;
                        }
                        break;
                }
                if ((g_actionFlg[playerId] == CPU_BEHAVIOUR_STATE_RUNNING) &&
                    ((zure_data[playerId] > 0.9f) || (zure_data[playerId] < -0.9f))) {
                    g_driftFlg[playerId] = 0;
                    player->slip_flag &= ~DRIFT;
                }
                if (player->slip_flag & D_JUMP) {
                    switch (g_driftFlg[playerId]) {
                        case 1:
                            newAngle = 0x0035;
                            break;
                        case -1:
                            newAngle = -0x0035;
                            break;
                        default:
                            newAngle =
                                (old_eos_angle_oga[playerId] + ((angle * steeringSensitivity) / minAngle)) / 2;
                            break;
                    }
                } else if (player->slip_flag & (HEIGHT_JUMP | N_JUMP | DASH_MIDJUMP)) {
                    newAngle = 0;
                } else {
                    newAngle = (old_eos_angle_oga[playerId] + ((angle * steeringSensitivity) / minAngle)) / 2;
                }
                enemy_stick_angle(player, newAngle);
                old_eos_angle_oga[playerId] = newAngle;
                if ((straight_flg[playerId] == true) || (g_driftFlg[playerId] == 1) ||
                    (g_driftFlg[playerId] == -1) ||
                    (player->slip_flag & (HEIGHT_JUMP | N_JUMP | DASH_MIDJUMP))) {
                    speedlevel[playerId] = GET_COURSE_cpu_CurveTargetSpeed(g_raceClass);
                } else {
                    speedlevel[playerId] = GET_COURSE_cpu_NormalTargetSpeed(g_raceClass);
                }
                if ((zure_data[playerId] > 0.9f) || (zure_data[playerId] < -0.9f)) {
                    speedlevel[playerId] = GET_COURSE_cpu_OffTrackTargetSpeed(g_raceClass);
                }
                if (biri_flg == 1) {
                    speedlevel[playerId] = GET_COURSE_D_0D0096B8(g_raceClass);
                }
                if ((g_driftFlg[playerId] == 2) || (g_driftFlg[playerId] == -2) || (g_driftFlg[playerId] == 3)) {
                    speedlevel[playerId] = 3.3333333f;
                }
                limmitspeed = speedlevel[playerId];
                player->slip_flag &= ~SLIP_STREAM;
                old_limmitspeed[playerId] = limmitspeed;
                fumikiri_wait_check(playerId);
                oga_accele(playerId, limmitspeed, player);
            }
        }
    }
}

void avoid_crash(s32 playerId) {
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
    UNUSED f32 pad2;
    s16 sp9C[8];
    UNUSED f32 pad3;
    f32 temp_f2;
    f32 sp74[8];
    s32 temp_a1_2;
    Player* player;
    player = &gPlayers[playerId];

    if (player->slip_flag & DRIFT) {
        return;
    }

    if (g_driftFlg[playerId] == 1) {
        return;
    }

    if (g_driftFlg[playerId] == -1) {
        return;
    }

    if (zure_data[playerId] < -1.0f) {
        return;
    }

    if (zure_data[playerId] > 1.0f) {
        return;
    }

    // Exclude heavyweights
    if (player->kart == WARIO) {
        return;
    }

    if (player->kart == BOWSER) {
        return;
    }

    if (player->kart == DK) {
        return;
    }

    // Skip if player has star
    if (player->slip_flag & STAR) {
        return;
    }

    currPathPoint = gNearestPathPointByPlayerId[playerId];
    temp_f22 = (player->speed / 18.0f) * 216.0f;
    for (i = 0; i < 8; i++) {
        sp9C[i] = -1;
        spB0[i] = 0x03E8;
    }

    if (avoid_count[playerId] > 0) {
        avoid_count[playerId]--;
        if (avoid_count[playerId] <= 0) {
            avoid_mode[playerId] = 0;
        }
    }
    j = 0;
    i = 0;
    while (i < 8) {
        if (i != playerId) {
            player = &gPlayers[i];
            if ((player->flag & EXISTS)) {
                temp_v1_2 = gNearestPathPointByPlayerId[i];
                temp_f0_2 = (player->speed / 18.0f) * 216.0f;
                temp_f2 = temp_f22 - 5.0f;
                if (temp_f0_2 < temp_f2) {
                    if (exist_area_chk(temp_v1_2, currPathPoint, 0, 0x0014U, center_line_max) > 0) {
                        temp_a1_2 = temp_v1_2 - currPathPoint;
                        sp9C[j] = i;
                        if (temp_a1_2 > 0) {
                            spB0[j] = temp_a1_2;
                        } else {
                            spB0[j] = (temp_v1_2 + center_line_max) - currPathPoint;
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
        temp_f2 = zure_data[sp9C[i]];
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

    if (zure_data[playerId] < var_f18) {
        return;
    }

    if (var_f20 < zure_data[playerId]) {
        return;
    }

    if (var_f20 > 1.0f) {
        var_f20 = 1.0f;
    }
    if (var_f18 < (-1.0f)) {
        var_f18 = -1.0f;
    }
    if ((var_f18 + 1.0f) < (1.0f - var_f20)) {
        avoid_count[playerId] = 0x003C;
        avoid_mode[playerId] = 1;
        zure_dist[playerId] = var_f20;
    } else {
        avoid_count[playerId] = 0x003C;
        avoid_mode[playerId] = 2;
        zure_dist[playerId] = var_f18;
    }
}

// utils track position

#include "cpu_vehicles_camera_path/path_utils.inc.c"

// bomb_kart utils

#include "cpu_vehicles_camera_path/bomb_kart.inc.c"

// actor utils

#include "cpu_vehicles_camera_path/actor_utils.inc.c"

void init_enemy_game_mode(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        gakaku_mode[i] = 0;
        kotei_gakaku_mode[i] = 0;
    }
}

void set_rival_car_name(void) {
    s32 shouldContinue;
    s32 i, j;

    for (j = 0; j < 2; j++) {
        enemy_rival_name[j] = 0;
    }

    if (g_DemoFlag == 1) {
        return;
    }
    if (g_gameMode != GRAND_PRIX) {
        return;
    }

    for (i = 0; i < 2; i++) {

        while (1) {
            enemy_rival_name[i] = MakeRandomLimmit(NUM_PLAYERS);

            if (g_menuMultiplayerSelection > 2) {
                break;
            }
            if (g_menuMultiplayerSelection < 1) {
                break;
            }

            shouldContinue = false;

            for (j = 0; j < g_menuMultiplayerSelection; j++) {
                if (g_CharacterSelections[j] == enemy_rival_name[i]) {
                    shouldContinue = true;
                }
            }
            for (j = 0; j < i; j++) {
                if (enemy_rival_name[j] == enemy_rival_name[i]) {
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
void clear_point_data(CenterPathStruct* arg0, size_t size) {
    bzero((void*) arg0, size * sizeof(CenterPathStruct));
}

// Appears to allocate memory for each course.
void InitCenterLine(void) {

    struct _struct_gCoursePathSizes_0x10* ptr = &GET_COURSE_PathSizes;
    s32 temp;
    s32 i;

    MaxPathPoints[0] = (s32) ptr->firstPath;
    MaxPathPoints[1] = (s32) ptr->secondPath;
    MaxPathPoints[2] = (s32) ptr->thirdPath;
    MaxPathPoints[3] = (s32) ptr->fourthPath;

    temp = ptr->unk8;
    center_line_EX = AllocateMemory(temp * 4);

    // Podium ceremony appears to allocate 1 * 8 bytes of data. Which would be aligned to 0x10.
    for (i = 0; i < 4; i++) {
        CenterPathBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(CenterPathStruct));
        SideLPathBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(CenterPathStruct));
        SideRPathBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(CenterPathStruct));
        CurvePathDataBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(s16));
        AnglePathDataBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(s16));
        ShortCutPathDataBP[i] = AllocateMemory(MaxPathPoints[i] * sizeof(s16));
    }

    g_pathPointPointer = CenterPathBP[0];
    side_point_L = SideLPathBP[0];
    side_point_R = SideRPathBP[0];
    curve_data = CurvePathDataBP[0];
    angle_data = AnglePathDataBP[0];
    short_cut_data = ShortCutPathDataBP[0];

    // zero allocated memory?
    //! @warning does not appear to zero all the above allocated variables.
    for (i = 0; i < 4; i++) {
        clear_point_data(CenterPathBP[i], MaxPathPoints[i]);
        clear_point_data(SideLPathBP[i], MaxPathPoints[i]);
        clear_point_data(SideRPathBP[i], MaxPathPoints[i]);
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

    center_line_max = *g_courseTotalPathPoints;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_KALAMARI_DESERT:
            area_bunkatu_SL();
            init_SL();
            break;
        case COURSE_DK_JUNGLE:
            area_bunkatu_SHIP();
            init_SHIP();
            break;
        case COURSE_TOADS_TURNPIKE:
            init_TRUCK();
            init_BUS();
            init_TANK();
            init_RV();
            break;
    }
#else

#endif
    init_BOM();
    kame_hibasira_init();
}

void init_enemy_kart(void) {

    UNUSED Camera* camera;
    s32 temp_v0_3;
    s32 i;
    SOUKOU_ZURE_TYPE* var_s5;
    UNUSED s32 temp_v1;
    UNUSED s32 pad;

    for (i = 0; i < NUM_PLAYERS; i++) {
        Player* player = &gPlayerOne[i];

        old_eos_angle_oga[i] = 0;
        avoid_mode[i] = 0;
        avoid_count[i] = 0;
        if (g_courseID < (NUM_COURSES - 1)) {
            zure_chk_BP(i, 0, 0);
        }
        speedlevel[i] = GET_COURSE_cpu_CurveTargetSpeed(g_raceClass);
        g_driftFlg[i] = 0;
        enemy_time_count[i] = 0;
        enemy_object_x[i] = 0.0f;
        enemy_object_z[i] = 0.0f;
        old_limmitspeed[i] = 0.0f;
        gLapCountByPlayerId[i] = -1;
        rap_count_F[i] = 0.0f;
        gTimePlayerLastTouchedFinishLine[i] = 0.0f;
        if (g_gameMode == GRAND_PRIX) {
            if (1) {};
            if (1) {}; // Maybe some debug code?
            gGPCurrentRaceRankByPlayerId[i] = (s32) kartrank[i];
            old_ogawa_rank[i] = (s32) kartrank[i];
        } else {
            gGPCurrentRaceRankByPlayerId[i] = i;
            old_ogawa_rank[i] = i;
        }
        temp_v0_3 = gGPCurrentRaceRankByPlayerId[i];
        rank_to_num[temp_v0_3] = (s16) i;
        real_rank_to_num[temp_v0_3] = (s16) i;
        real_rank[i] = temp_v0_3;
        oga_reverse_count[i] = 0;
        g_wrongwayFlagPlayer1[i] = 0;
        tenuki_flg[i] = false;
        old_tenuki_flg[i] = 0;
        g_playerPathPointTotalTable[i] = -20;
        old_pylon_count[i] = -20;
        name_to_num[gPlayers[i].kart] = (s16) i;
        zure_data[i] = 0.0f;
        zure_dist[i] = 0.0f;
        var_s5 = &LaneData[i];
        var_s5->lane_free_drive = GET_COURSE_AIMinimumSeparation * (f32) (((i + 1) % 3) - 1);
        var_s5->lane_target = var_s5->lane_free_drive;
        var_s5->lane = 0.0f;
        var_s5->speed = 0.015f;
        action_flg_sleep(i);
        enemy_accel_mode[i] = 0;
        enemy_enter_maze[i] = 0;
        enemy_voice_count_SPIN[i] = 0;
        enemy_voice_count_DAMAGE[i] = 0;
        enemy_voice_count_OVERTAKE[i] = 0;
        g_noSimpleKartFlag[i] = 0;
        g_lakituSurface[i] = 0;
        oga_old_rap_chk_z[i] = player->position[2];
        mokuhyou_add_area[i] = 6;
        if (gPlayers[i].flag & IS_PLAYER) {
            oga_car_stat[i] = 3;

        } else {
            oga_car_stat[i] = 0;
        }

        enemy_enter_trg[i] = 0;
        enemy_exit_trg[i] = 0;
        enemy_dist[i] = -1;
        enemy_limdist[i] = -1;
        enemy_geta_rank[i] = -1;
        g_rivalOvertakeAllowFlag[i] = 0;
        fumikiri_stop_flg[i] = 0;
        straight_flg[i] = true;
    }

#ifdef AVOID_UB
    for (i = 0; i < NUM_CROSSINGS; i++) {
        fumikiri_counter[i] = 0;
    }
#else
    fumikiri_counter[0] = 0;
    fumikiri_counter[1] = 0;
#endif
    if (g_DemoFlag == DEMO_MODE_INACTIVE) {

        if (g_gameMode == GRAND_PRIX) {
            for (i = 0; i < 2; i++) {
                enemy_rival_num[i] = name_to_num[enemy_rival_name[i]];
                oga_car_stat[enemy_rival_num[i]] = 1;
                enemy_rival_aim_rank[enemy_rival_num[i]] = i;
            }
        }
    }
    if ((demo_play_flag == 1) && (g_courseID != COURSE_AWARD_CEREMONY)) {
        for (i = 0; i < NUM_PLAYERS; i++) {
            oga_car_stat[i] = 0;
        }
        if (g_gameMode == VERSUS) {

            enemy_rival_num[0] = cameras->watch;
            oga_car_stat[enemy_rival_num[0]] = 1;
            enemy_rival_aim_rank[enemy_rival_num[0]] = 0;

            for (i = 1; i < 2; i++) {
                enemy_rival_num[i] = cameras[i].watch;
                oga_car_stat[enemy_rival_num[i]] = 1;
                enemy_rival_aim_rank[enemy_rival_num[i]] = i;
            }
        }
    }

    for (i = 0; i < NUM_PLAYERS; i++) {
        if (1) {};
        CurrentPathID[i] = 0;
        gNearestPathPointByPlayerId[i] =
            g_courseTotalPathPoints[CurrentPathID[i]] - gGPCurrentRaceRankByPlayerId[i] - 4;
    }

    highway_gyakusou_flg = false;
    if (g_raceClass == CC_EXTRA) {
        highway_gyakusou_flg = true;
    }

    for (i = 0; i < 30; i++) {
        pylon_num[i] = -1;
        pylon_num2[i] = -1;
    }

    best_time = 100000; // 100,000
    oga_camera_bird_x = 0.0f;
    oga_camera_bird_y = 100.0f;
    oga_camera_bird_z = 0.0f;
    enemy_free_item_debug_flg = 0;
    enemy_free_item_debug_inum = 1;
    enemy_free_item_debug_num = 1;
    g_EnemyTargetPlayer = PLAYER_ONE;
    oga_frame_counter = 0;
    oga_etc_frame_counter = 0;
    g_finishLineZ = (f32) CenterPathBP[0][0].pointz; // [i][2]
    oga_calc_loop_count = 0;
    old_oga_calc_loop_count = 0;
    oga_bump_chk_counter = 0;
    g_rankUpdateFinishFlag = 0;
    ogawa_karakara_short_cut_ok_flg = 0;
    enemy_item_init();
    if (demo_play_flag == 1) {
        if (g_DemoFlag == 1) {

            for (i = 0; i < NUM_PLAYERS; i++) {
                if (oga_car_stat[i] == 1) {
                    gPlayers[i].weapon |= USE_STARTDASH;
                }
            }
        }
    }
    init_act_dat_table();
}

#include "cpu_vehicles_camera_path/path_calc.inc.c"

#include "cpu_vehicles_camera_path/behaviour_utils.inc.c"

#include "cpu_vehicles_camera_path/vehicle_utils.inc.c"

void cam_check_bump(s32 cameraId, s32 pathIndex) {
    s16 cameraPathPoint;
    CenterPathStruct* temp_v0;

    cameraPathPoint = oga_cam_area[cameraId];
    temp_v0 = &CenterPathBP[pathIndex][cameraPathPoint];
    CheckBump(&cameras[cameraId].bump, 10.0f, (f32) temp_v0->pointx, (f32) temp_v0->pointy + 30.0f,
                             (f32) temp_v0->pointz);
}

void ogawa_camera_init(s32 cameraIndex) {
    s32 cameraId;

    D_801646CC = 0;
    kotei_gakaku_mode[cameraIndex] = gakaku_mode[cameraIndex];
    if ((g_gameMode != 1) && ((g_courseSelect == COURSE_ONE) || (g_DemoFlag == (u16) 1))) {
        kotei_gakaku_mode[cameraIndex] = 0;
    } else if ((kotei_gakaku_mode[cameraIndex] != 0) && (kotei_gakaku_mode[cameraIndex] != (s16) 1) &&
               (kotei_gakaku_mode[cameraIndex] != 2) && (kotei_gakaku_mode[cameraIndex] != 3)) {
        kotei_gakaku_mode[cameraIndex] = 0;
    }
    ogawa_camera_mode[cameraIndex] = -1;
    oga_camera_watch_num = 0;
    katamuki_theta[cameraIndex] = 0;
    oga_goal_counter = 0;
    oga_sound_camera[cameraIndex].unk0 = 0;
    oga_sound_camera[cameraIndex].unk2 = 0;
    oga_sound_camera[cameraIndex].unk4 = 0;
    if ((g_gameMode == 1) && (g_InGame == 0)) {
        kotei_gakaku_mode[cameraIndex] = 0;
    }

    for (cameraId = 0; cameraId < 4; cameraId++) {
        oga_cam_area[cameraId] = 0;
    }
}

f32 ogawa_gakaku(f32 arg0, s32 arg1) {
    f32 temp_f0;
    f64 temp_f2;

    temp_f0 = cam_offset[arg1];
    switch (kotei_gakaku_mode[arg1]) { /* irregular */
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

void oga_camera_rotate(Camera* camera, s32 cameraIndex) {
    f32 xnorm;
    f32 ynorm;
    f32 znorm;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 distance;
    f32 sp28;
    u16 thing;

    thing = katamuki_theta[cameraIndex];
    if (thing == 0) {
        camera->up_vector[0] = 0.0f;
        camera->up_vector[2] = 0.0f;
        camera->up_vector[1] = 1.0f;
    } else {
        xdiff = camera->lookat_pos[0] - camera->camera_pos[0];
        ydiff = camera->lookat_pos[1] - camera->camera_pos[1];
        zdiff = camera->lookat_pos[2] - camera->camera_pos[2];
        distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
        xnorm = xdiff / distance;
        ynorm = ydiff / distance;
        znorm = zdiff / distance;
        sp28 = 1.0 - cosT(thing);
        camera->up_vector[0] = (sp28 * xnorm * ynorm) - (sinT(thing) * znorm);
        camera->up_vector[1] = cosT(thing) + (sp28 * ynorm * ynorm);
        camera->up_vector[2] = (sinT(thing) * xnorm) + (sp28 * ynorm * znorm);
    }
}

UNUSED void func_8001530C(void) {
}

void ogawa_video_init(s32 playerId, UNUSED f32 arg1, s32 cameraId) {
    Camera* temp_a0;
    Player* temp_a1;

    // wtf is up with the pointer accesses here?
    // What aren't they just doing thing = &some_pointer[some_index]?
    temp_a1 = gPlayerOne;
    temp_a0 = camera1;
    temp_a1 += playerId;
    temp_a0 += cameraId;
    temp_a0->chase_direction = temp_a1->direction[1];
    ogawa_videoview(temp_a0, temp_a1, 0);
}

void ogawa_videoview(Camera* camera, UNUSED Player* player, UNUSED s32 arg2) {
    UNUSED s32 pad[6];
    f32 temp_f12;
    f32 sp90;
    f32 temp_f14;
    Player* temp_s1;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    UNUSED Vec3f pad2;
    Vec3f sp64;
    UNUSED s32 pad3[9];
    s16 var_a2;

    /*
    Doing the more sensible:
    temp_s1 = &gPlayerOne[camera->watch];
    leads to some regalloc differences
    */
    temp_s1 = gPlayerOne;
    temp_s1 += camera->watch;
    if (temp_s1->sterrangle == 0) {
        var_a2 = 0x0064;
    } else if (temp_s1->sterrangle < 0) {
        var_a2 = 0xA0 - (temp_s1->sterrangle / 16);
    } else {
        var_a2 = 0xA0 + (temp_s1->sterrangle / 16);
    }
    if (!((temp_s1->slip_flag & SPIN_L) || (temp_s1->slip_flag & SPIN_R))) {
        ChaseDir(&camera->chase_direction, temp_s1->direction[1], var_a2);
    }
    direction_fixcamera(temp_s1, camera, sp64, &sp84, &sp80, &sp7C, camera->chase_direction);
    CheckBump(&camera->bump, 10.0f, sp84, sp80, sp7C);
    camera->lookat_pos[0] = sp64[0];
    camera->lookat_pos[1] = sp64[1];
    camera->lookat_pos[2] = sp64[2];
    camera->camera_pos[0] = sp84;
    camera->camera_pos[1] = sp80;
    camera->camera_pos[2] = sp7C;
    temp_f12 = camera->lookat_pos[0] - camera->camera_pos[0];
    sp90 = camera->lookat_pos[1] - camera->camera_pos[1];
    temp_f14 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(temp_f12, temp_f14);
    camera->camera_direction[0] = Atan2t(sqrtf((temp_f12 * temp_f12) + (temp_f14 * temp_f14)), sp90);
    camera->camera_direction[2] = 0;
}

void ogawa_kotei_init(s32 playerId, f32 arg1, s32 cameraId, s32 pathIndex) {
    Camera* camera;

    f32 temp_f12;
    f32 temp_f2;
    s32 test = g_courseTotalPathPoints[pathIndex];

    oga_camera_zure[cameraId] = arg1;
    camera = cameras + cameraId;
    oga_cam_area[cameraId] = (gNearestPathPointByPlayerId[playerId] + 10) % test;

    obj_calc_from_zure_BP(oga_cam_area[cameraId], arg1, pathIndex);

    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_z[cameraId] = object[2];

    temp_f2 = (f32) CenterPathBP[pathIndex][oga_cam_area[cameraId]].pointy;

    temp_f12 = CheckHight(object[0], (f32) (temp_f2 + 30.0), object[2]);
    if ((temp_f12 < (temp_f2 - 20.0)) || (temp_f12 >= 3000.0)) {
        old_oga_cam_y[cameraId] = (f32) (temp_f2 + 10.0);
    } else {
        old_oga_cam_y[cameraId] = (f32) (temp_f12 + 10.0);
    }
    cam_speed[cameraId] = 0.0f;
    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_koteiview(Camera* camera, UNUSED Player* playerArg, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 pad;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED s32 pad2;
    s32 playerId;
    UNUSED s32 pad3[9];
    Player* player;
    s32 pathPointDiff;
    s32 pathIndex;

    playerId = camera->watch;
    pathIndex = CurrentPathID[playerId];
    player = gPlayerOne;
    player += playerId;
    oga_cam_area[cameraId] =
        get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
    playerPathPoint = gNearestPathPointByPlayerId[playerId];
    cameraPathPoint = oga_cam_area[cameraId];
    if (exist_area_chk(playerPathPoint, cameraPathPoint, 0x0032U, 0x000FU, g_courseTotalPathPoints[pathIndex]) <=
        0) {
        oga_demo_camera_re_init(cameraId, oga_camera_zure[cameraId], ogawa_camera_mode[cameraId]);
    } else {
        if (zure_data[playerId] < (-0.7)) {
            pathPointDiff = playerPathPoint - cameraPathPoint;
            if ((oga_camera_zure[cameraId] < (-0.5)) && ((pathPointDiff * pathPointDiff) < 5)) {
                oga_demo_camera_re_init(cameraId, 1.0f, 3);
                goto alable;
            }
        }
        // clang-format off
        // I hate this, but a fakematch is a fakematch
        if (zure_data[playerId] > 0.7) { pathPointDiff = playerPathPoint - cameraPathPoint; if ((oga_camera_zure[cameraId] > 0.5) && ((pathPointDiff * pathPointDiff) < 5)) {
                oga_demo_camera_re_init(cameraId, -1.0f, 2);
                // clang-format on
            }
        }
    }
alable:
    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
    camera->lookat_pos[0] = player->position[0];
    camera->lookat_pos[1] = player->position[1] + 6.0;
    camera->lookat_pos[2] = player->position[2];
    cam_check_bump(cameraId, pathIndex);
    xdiff = camera->lookat_pos[0] - camera->camera_pos[0];
    ydiff = camera->lookat_pos[1] - camera->camera_pos[1];
    zdiff = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(xdiff, zdiff);
    camera->camera_direction[0] = Atan2t(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    camera->camera_direction[2] = 0;
}

void ogawa_oikake_init(s32 playerId, f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;

    oga_camera_zure[cameraId] = arg1;
    oga_cam_area[cameraId] = gNearestPathPointByPlayerId[playerId] + 10;
    oga_cam_area[cameraId] = (oga_cam_area[cameraId]) % g_courseTotalPathPoints[pathIndex];

    obj_calc_from_zure_BP(oga_cam_area[cameraId], arg1, pathIndex);

    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_y[cameraId] = (f32) CenterPathBP[pathIndex][oga_cam_area[cameraId]].pointy;
    old_oga_cam_z[cameraId] = object[2];

    cam_speed[cameraId] = gPlayers[playerId].speed / 5.0f;
    if ((f64) cam_speed[cameraId] < 0.0) {
        cam_speed[cameraId] = 0.0f;
    }

    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_oikakeview(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 playerPathPoint;
    s16 cameraPathPoint;
    UNUSED s32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* player;
    s32 playerId;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 distance;
    UNUSED s32 stackPadding1;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 var_f18;
    UNUSED f32 var_f20;
    f32 temp_f2_2;
    s32 pathIndex;

    playerId = camera->watch;
    player = gPlayerOne;
    player += playerId;
    oga_camera_watch_num = playerId;
    pathIndex = CurrentPathID[playerId];
    oga_cam_area[cameraId] =
        get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
    playerPathPoint = gNearestPathPointByPlayerId[playerId];
    cameraPathPoint = oga_cam_area[cameraId];
    if (exist_area_chk(playerPathPoint, cameraPathPoint, 0x0032U, 0x000FU, g_courseTotalPathPoints[pathIndex]) <=
        0) {
        oga_demo_camera_re_init(cameraId, oga_camera_zure[cameraId], ogawa_camera_mode[cameraId]);
    } else if ((zure_data[playerId] < -0.5) && (oga_camera_zure[cameraId] < -0.5)) {
        oga_demo_camera_re_init(cameraId, 1.0f, 7);
    } else if ((zure_data[playerId] > 0.5) && (oga_camera_zure[cameraId] > 0.5)) {
        oga_demo_camera_re_init(cameraId, -1.0f, 6);
    }
    pathPoint1 = (oga_cam_area[cameraId] + 1) % g_courseTotalPathPoints[pathIndex];
    pathPoint2 = (oga_cam_area[cameraId] + 2) % g_courseTotalPathPoints[pathIndex];
    obj_calc_from_zure_BP(pathPoint1, oga_camera_zure[cameraId], pathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    obj_calc_from_zure_BP(pathPoint2, oga_camera_zure[cameraId], pathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[pathIndex][pathPoint1].pointy + CenterPathBP[pathIndex][pathPoint2].pointy) / 2.0;
    xdiff = midX - old_oga_cam_x[cameraId];
    ydiff = midY - old_oga_cam_y[cameraId];
    zdiff = midZ - old_oga_cam_z[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = old_oga_cam_x[cameraId] + ((cam_speed[cameraId] * xdiff) / distance);
        midY = old_oga_cam_y[cameraId] + ((cam_speed[cameraId] * ydiff) / distance);
        midZ = old_oga_cam_z[cameraId] + ((cam_speed[cameraId] * zdiff) / distance);
    } else {
        midX = old_oga_cam_x[cameraId];
        midY = old_oga_cam_y[cameraId];
        midZ = old_oga_cam_z[cameraId];
    }
    camera->camera_pos[0] = midX;
    camera->camera_pos[2] = midZ;
    temp_f2_2 = CheckHight(midX, midY + 30.0, midZ);
    if ((temp_f2_2 < (midY - 20.0)) || (temp_f2_2 >= 3000.0)) {
        camera->camera_pos[1] = midY + 10.0;
    } else {
        camera->camera_pos[1] = temp_f2_2 + 8.0;
    }
    old_oga_cam_x[cameraId] = midX;
    old_oga_cam_y[cameraId] = midY;
    old_oga_cam_z[cameraId] = midZ;
    camera->lookat_pos[0] = player->position[0];
    camera->lookat_pos[1] = player->position[1] + 6.0;
    camera->lookat_pos[2] = player->position[2];
    cam_check_bump(cameraId, pathIndex);
    xdiff2 = camera->lookat_pos[0] - camera->camera_pos[0];
    ydiff2 = camera->lookat_pos[1] - camera->camera_pos[1];
    zdiff2 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(xdiff2, zdiff2);
    camera->camera_direction[0] = Atan2t(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    camera->camera_direction[2] = 0;
}

void ogawa_side_init(s32 playerId, f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;

    oga_camera_zure[cameraId] = arg1;
    oga_cam_area[cameraId] = gNearestPathPointByPlayerId[playerId];
    oga_cam_area[cameraId] = (oga_cam_area[cameraId]) % g_courseTotalPathPoints[pathIndex];

    obj_calc_from_zure_BP(oga_cam_area[cameraId], arg1, pathIndex);

    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_y[cameraId] = (f32) CenterPathBP[pathIndex][oga_cam_area[cameraId]].pointy;
    old_oga_cam_z[cameraId] = object[2];

    cam_speed_objct[cameraId] = gPlayers[playerId].speed;
    cam_speed[cameraId] = gPlayers[playerId].speed;

    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_sideview(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* player;
    s32 playerId;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    f32 distance;
    UNUSED f32 sp98;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 sp94;
    UNUSED f32 sp90;
    UNUSED s32 stackPadding1;
    s32 pathIndex;
    f32 temp_f2_5;

    playerId = camera->watch;
    player = gPlayerOne;
    cam_speed[cameraId] += ((cam_speed_objct[cameraId] - cam_speed[cameraId]) * 0.5f);
    oga_camera_watch_num = playerId;
    pathIndex = CurrentPathID[playerId];
    player += playerId;
    oga_cam_area[cameraId] =
        get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
    temp_f2_5 = (zure_data[playerId] - oga_camera_zure[cameraId]);
    temp_f2_5 *= temp_f2_5;
    playerPathPoint = gNearestPathPointByPlayerId[playerId];
    cameraPathPoint = oga_cam_area[cameraId];
    if (exist_area_chk(playerPathPoint, cameraPathPoint, 0x000FU, 0x000FU, g_courseTotalPathPoints[pathIndex]) <=
        0) {
        oga_demo_camera_re_init(cameraId, oga_camera_zure[cameraId], ogawa_camera_mode[cameraId]);
    } else {
        if ((zure_data[playerId] < 0.0) && (oga_camera_zure[cameraId] < 0.0) && (temp_f2_5 < 0.01)) {
            oga_demo_camera_re_init(cameraId, 1.0f, 5);
        } else {
            if ((zure_data[playerId] > 0.0) && (oga_camera_zure[cameraId] > 0.0) && (temp_f2_5 < 0.01)) {
                oga_demo_camera_re_init(cameraId, -1.0f, 4);
            } else {
                if ((cameraPathPoint < playerPathPoint) && ((playerPathPoint - cameraPathPoint) < 0xA)) {
                    cam_speed_objct[cameraId] = gPlayers[playerId].speed + 0.4;
                }
                if ((playerPathPoint < cameraPathPoint) && ((cameraPathPoint - playerPathPoint) < 0xA)) {
                    cam_speed_objct[cameraId] = gPlayers[playerId].speed - 0.4;
                }
                if (cam_speed_objct[cameraId] > 10.0) {
                    cam_speed_objct[cameraId] = 10.0f;
                }
                if (cam_speed_objct[cameraId] < 0.0) {
                    cam_speed_objct[cameraId] = 0.0f;
                }
            }
        }
    }
    pathPoint1 = (oga_cam_area[cameraId] + 1) % g_courseTotalPathPoints[pathIndex];
    pathPoint2 = (oga_cam_area[cameraId] + 2) % g_courseTotalPathPoints[pathIndex];
    obj_calc_from_zure_BP(pathPoint1, oga_camera_zure[cameraId], pathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    obj_calc_from_zure_BP(pathPoint2, oga_camera_zure[cameraId], pathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[pathIndex][pathPoint1].pointy + CenterPathBP[pathIndex][pathPoint2].pointy) / 2.0;
    xdiff = midX - old_oga_cam_x[cameraId];
    ydiff = midY - old_oga_cam_y[cameraId];
    zdiff = midZ - old_oga_cam_z[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = old_oga_cam_x[cameraId] + ((cam_speed[cameraId] * xdiff) / distance);
        midY = old_oga_cam_y[cameraId] + ((cam_speed[cameraId] * ydiff) / distance);
        midZ = old_oga_cam_z[cameraId] + ((cam_speed[cameraId] * zdiff) / distance);
    } else {
        midX = old_oga_cam_x[cameraId];
        midY = old_oga_cam_y[cameraId];
        midZ = old_oga_cam_z[cameraId];
    }
    camera->camera_pos[0] = midX;
    camera->camera_pos[2] = midZ;
    temp_f2_5 = CheckHight(midX, midY + 30.0, midZ);
    if ((temp_f2_5 < (midY - 20.0)) || (temp_f2_5 >= 3000.0)) {
        camera->camera_pos[1] = midY + 10.0;
    } else {
        camera->camera_pos[1] = temp_f2_5 + 10.0;
    }
    old_oga_cam_x[cameraId] = midX;
    old_oga_cam_y[cameraId] = midY;
    old_oga_cam_z[cameraId] = midZ;
    camera->lookat_pos[0] = player->position[0];
    camera->lookat_pos[1] = player->position[1] + 6.0;
    camera->lookat_pos[2] = player->position[2];
    cam_check_bump(cameraId, pathIndex);
    xdiff2 = camera->lookat_pos[0] - camera->camera_pos[0];
    ydiff2 = camera->lookat_pos[1] - camera->camera_pos[1];
    zdiff2 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(xdiff2, zdiff2);
    camera->camera_direction[0] = Atan2t(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    camera->camera_direction[2] = 0;
}

void ogawa_quick_init(UNUSED s32 playerId, UNUSED f32 arg1, s32 cameraId) {
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
    Camera* camera;

    if (MakeRandomLimmit(0x0064U) < 0x32) {
        oga_camera_zure[cameraId] = 0.1f;
    } else {
        oga_camera_zure[cameraId] = -0.1f;
    }
    D_80163DD8[cameraId] = 0;
    if (g_courseID == COURSE_YOSHI_VALLEY) {
        D_80163DD8[cameraId] = MakeRandomLimmit(4U);
        oga_camera_zure[cameraId] = 0.0f;
    }
    temp_s0 = g_courseTotalPathPoints[D_80163DD8[cameraId]];
    oga_cam_area[cameraId] %= temp_s0;
    obj_calc_from_zure_BP(oga_cam_area[cameraId], oga_camera_zure[cameraId], 0);
    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_y[cameraId] = CenterPathBP[0][oga_cam_area[cameraId]].pointy;
    path = CenterPathBP;
    old_oga_cam_z[cameraId] = object[2];
    cam_speed_objct[cameraId] = 16.666666f;
    cam_speed[cameraId] = 0.0f;
    sp48 = (oga_cam_area[cameraId] + 0xA) % temp_s0;
    sp44 = (oga_cam_area[cameraId] + 0xB) % temp_s0;
    obj_calc_from_zure_BP(sp48, oga_camera_zure[cameraId], 0);
    temp_f2 = object[0] * 0.5;
    temp_f12 = object[2] * 0.5;
    obj_calc_from_zure_BP(sp44, oga_camera_zure[cameraId], 0);
    temp_f2 += object[0] * 0.5;
    temp_f12 += object[2] * 0.5;
    sp48 = (oga_cam_area[cameraId] + 0x5) % temp_s0;
    sp44 = (oga_cam_area[cameraId] + 0x6) % temp_s0;
    sp54 = (path[0][sp48].pointy + path[0][sp44].pointy) * 0.5f;
    camera = cameras;
    camera += cameraId;
    camera->lookat_pos[0] = temp_f2;
    camera->lookat_pos[2] = temp_f12;
    camera->lookat_pos[1] = sp54 + 8.0;
    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_quickview(Camera* camera, UNUSED Player* player, UNUSED s32 index, s32 cameraId);
void ogawa_quickview(Camera* camera, UNUSED Player* player, UNUSED s32 index, s32 cameraId) {
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
    s32 playerId;
    f32 temp_f0;
    s32 pathIndex;
    s32 sp58;
    s16 sp56;

    playerId = camera->watch;
    cam_speed[cameraId] += (cam_speed_objct[cameraId] - cam_speed[cameraId]) * 0.5f;
    pathIndex = D_80163DD8[cameraId];
    sp58 = g_courseTotalPathPoints[pathIndex];
    oga_camera_watch_num = playerId;
    sp56 = oga_cam_area[cameraId];
    oga_cam_area[cameraId] = get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
    if (g_courseID == 4) {
        if ((sp56 != oga_cam_area[cameraId]) && (oga_cam_area[cameraId] == 1)) {
            D_80163DD8[cameraId] = MakeRandomLimmit(4);
            pathIndex = D_80163DD8[cameraId];
            oga_cam_area[cameraId] = get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
        }
    }
    sp6E = (oga_cam_area[cameraId] + 0xA) % sp58;
    sp6C = (oga_cam_area[cameraId] + 0xB) % sp58;
    obj_calc_from_zure_BP(sp6E, oga_camera_zure[cameraId], pathIndex);
    sp8C = object[0] * 0.5;
    sp84 = object[2] * 0.5;
    obj_calc_from_zure_BP(sp6C, oga_camera_zure[cameraId], pathIndex);
    sp8C += object[0] * 0.5;
    sp84 += object[2] * 0.5;

    sp6E = (oga_cam_area[cameraId] + 5) % sp58;
    sp6C = (oga_cam_area[cameraId] + 6) % sp58;
    sp88 = (CenterPathBP[pathIndex][sp6E].pointy + CenterPathBP[pathIndex][sp6C].pointy) * 0.5f;
    sp6E = (oga_cam_area[cameraId] + 1) % sp58;
    sp6C = (oga_cam_area[cameraId] + 2) % sp58;
    obj_calc_from_zure_BP(sp6E, oga_camera_zure[cameraId], pathIndex);
    sp98 = object[0] * 0.5;
    sp90 = object[2] * 0.5;
    obj_calc_from_zure_BP(sp6C, oga_camera_zure[cameraId], pathIndex);
    sp98 += object[0] * 0.5;
    sp90 += object[2] * 0.5;
    sp94 = (CenterPathBP[pathIndex][sp6E].pointy + CenterPathBP[pathIndex][sp6C].pointy) * 0.5f;

    sp80 = sp98 - old_oga_cam_x[cameraId];
    sp7C = sp94 - old_oga_cam_y[cameraId];
    sp78 = sp90 - old_oga_cam_z[cameraId];
    temp_f0 = sqrtf(((sp80 * sp80) + (sp7C * sp7C)) + (sp78 * sp78));

    if (temp_f0 != 0.0) {
        sp98 = old_oga_cam_x[cameraId] + (((stackPadding9 = cam_speed[cameraId]) * sp80) / temp_f0);
        sp94 = old_oga_cam_y[cameraId] + ((cam_speed[cameraId] * sp7C) / temp_f0);
        sp90 = old_oga_cam_z[cameraId] + ((cam_speed[cameraId] * sp78) / temp_f0);
    } else {
        sp98 = old_oga_cam_x[cameraId];
        sp94 = old_oga_cam_y[cameraId];
        sp90 = old_oga_cam_z[cameraId];
    }

    if (sp98 < -10000.0 || sp98 > 10000.0) {
        if (sp8C < -10000.0 || sp8C > 10000.0) {}
    }
    camera->camera_pos[0] = sp98;
    camera->camera_pos[2] = sp90;
    camera->camera_pos[1] = sp94 + 10.0;

    old_oga_cam_x[cameraId] = sp98;
    old_oga_cam_y[cameraId] = sp94;
    old_oga_cam_z[cameraId] = sp90;

    if (sp8C < -10000.0 || sp8C > 10000.0) {}
    if (sp84 < -10000.0 || sp84 > 10000.0) {}
    camera->lookat_pos[0] = sp8C;
    camera->lookat_pos[1] = sp88 + 8.0;
    camera->lookat_pos[2] = sp84;
    cam_check_bump(cameraId, pathIndex);
    spAC = camera->lookat_pos[0] - camera->camera_pos[0];
    spA8 = camera->lookat_pos[1] - camera->camera_pos[1];
    spA4 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(spAC, spA4);
    camera->camera_direction[0] = Atan2t(sqrtf((spAC * spAC) + (spA4 * spA4)), spA8);
    camera->camera_direction[2] = 0;
}

void ogawa_face_init(s32 playerId, UNUSED f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    UNUSED s32 pad;

    oga_camera_zure[cameraId] = zure_data[playerId];
    oga_cam_area[cameraId] = gNearestPathPointByPlayerId[playerId] + 3;
    oga_cam_area[cameraId] = oga_cam_area[cameraId] % g_courseTotalPathPoints[pathIndex];

    obj_calc_from_zure_BP(oga_cam_area[cameraId], zure_data[playerId], pathIndex);

    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_y[cameraId] = (f32) CenterPathBP[pathIndex][oga_cam_area[cameraId]].pointy;
    old_oga_cam_z[cameraId] = object[2];

    cam_speed_objct[cameraId] = gPlayers[playerId].speed;
    cam_speed[cameraId] = gPlayers[playerId].speed;

    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_faceview(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED f32 stackPadding0;
    f32 xdiff2;
    f32 ydiff2;
    f32 zdiff2;
    Player* player;
    f32 distance;
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
    s32 playerId;
    s32 pathIndex;
    s32 pathPointCount;

    playerId = camera->watch;
    player = gPlayerOne;
    oga_camera_zure[cameraId] = zure_data[playerId];
    cam_speed[cameraId] += ((cam_speed_objct[cameraId] - cam_speed[cameraId]) / 2.0f);
    oga_camera_watch_num = playerId;
    pathIndex = CurrentPathID[playerId];
    player += playerId;
    pathPointCount = g_courseTotalPathPoints[pathIndex];
    oga_cam_area[cameraId] =
        get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
    playerPathPoint = (gNearestPathPointByPlayerId[playerId] + 3) % pathPointCount;
    cameraPathPoint = oga_cam_area[cameraId];
    if (exist_area_chk(playerPathPoint, cameraPathPoint, 0x000DU, 1U, pathPointCount) <= 0) {
        oga_demo_camera_re_init(cameraId, oga_camera_zure[cameraId], ogawa_camera_mode[cameraId]);
    } else {
        if ((cameraPathPoint < playerPathPoint) && ((playerPathPoint - cameraPathPoint) < 3)) {
            cam_speed_objct[cameraId] = gPlayers[playerId].speed + 0.1;
        }
        if ((playerPathPoint < cameraPathPoint) && ((cameraPathPoint - playerPathPoint) < 3)) {
            cam_speed_objct[cameraId] = gPlayers[playerId].speed - 0.1;
        }
        if (cam_speed_objct[cameraId] > 10.0) {
            cam_speed_objct[cameraId] = 10.0f;
        }
        if (cam_speed_objct[cameraId] < 0.0) {
            cam_speed_objct[cameraId] = 0.0f;
        }
    }
    pathPoint1 = (oga_cam_area[cameraId] + 1) % pathPointCount;
    pathPoint2 = (oga_cam_area[cameraId] + 2) % pathPointCount;
    obj_calc_from_zure_BP(pathPoint1, oga_camera_zure[cameraId], pathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    obj_calc_from_zure_BP(pathPoint2, oga_camera_zure[cameraId], pathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[pathIndex][pathPoint1].pointy + CenterPathBP[pathIndex][pathPoint2].pointy) / 2.0;
    xdiff = midX - old_oga_cam_x[cameraId];
    ydiff = midY - old_oga_cam_y[cameraId];
    zdiff = midZ - old_oga_cam_z[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = old_oga_cam_x[cameraId] + ((cam_speed[cameraId] * xdiff) / distance);
        midY = old_oga_cam_y[cameraId] + ((cam_speed[cameraId] * ydiff) / distance);
        midZ = old_oga_cam_z[cameraId] + ((cam_speed[cameraId] * zdiff) / distance);
    } else {
        midX = old_oga_cam_x[cameraId];
        midY = old_oga_cam_y[cameraId];
        midZ = old_oga_cam_z[cameraId];
    }
    camera->camera_pos[0] = midX;
    camera->camera_pos[2] = midZ;
    camera->camera_pos[1] = midY + 10.0;
    old_oga_cam_x[cameraId] = midX;
    old_oga_cam_y[cameraId] = midY;
    old_oga_cam_z[cameraId] = midZ;
    camera->lookat_pos[0] = player->position[0];
    camera->lookat_pos[1] = player->position[1] + 6.0;
    camera->lookat_pos[2] = player->position[2];
    cam_check_bump(cameraId, pathIndex);
    xdiff2 = camera->lookat_pos[0] - camera->camera_pos[0];
    ydiff2 = camera->lookat_pos[1] - camera->camera_pos[1];
    zdiff2 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(xdiff2, zdiff2);
    camera->camera_direction[0] = Atan2t(sqrtf((xdiff2 * xdiff2) + (zdiff2 * zdiff2)), ydiff2);
    camera->camera_direction[2] = 0;
}

void ogawa_back_init(s32 playerId, UNUSED f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    s32 test = g_courseTotalPathPoints[pathIndex];

    oga_camera_zure[cameraId] = zure_data[playerId];
    oga_cam_area[cameraId] = (gNearestPathPointByPlayerId[playerId] + test) - 2;
    oga_cam_area[cameraId] = oga_cam_area[cameraId] % test;

    obj_calc_from_zure_BP(oga_cam_area[cameraId], zure_data[playerId], pathIndex);

    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_y[cameraId] = (f32) CenterPathBP[pathIndex][oga_cam_area[cameraId]].pointy;
    old_oga_cam_z[cameraId] = object[2];

    cam_speed_objct[cameraId] = gPlayers[playerId].speed;
    cam_speed[cameraId] = gPlayers[playerId].speed;

    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_backview(Camera* camera, UNUSED Player* unusedPlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 stackPadding3;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    UNUSED s32 stackPadding4;
    f32 distance;
    f32 midX;
    f32 midY;
    f32 midZ;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    Player* player;
    s32 playerId;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED f32 var_f2;
    UNUSED f32 var_f18;
    UNUSED f32 var_f14;
    s32 pathIndex;
    s32 pathPointCount;

    playerId = camera->watch;
    player = gPlayerOne;
    oga_camera_zure[cameraId] = zure_data[playerId];
    cam_speed[cameraId] += ((cam_speed_objct[cameraId] - cam_speed[cameraId]) * 0.5f);
    oga_camera_watch_num = playerId;
    pathIndex = CurrentPathID[playerId];
    pathPointCount = g_courseTotalPathPoints[pathIndex];
    player += playerId;
    oga_cam_area[cameraId] =
        get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
    playerPathPoint = ((gNearestPathPointByPlayerId[playerId] + pathPointCount) - 2) % pathPointCount;
    cameraPathPoint = oga_cam_area[cameraId];
    if (exist_area_chk(playerPathPoint, cameraPathPoint, 1U, 0x000AU, pathPointCount) <= 0) {
        oga_demo_camera_re_init(cameraId, oga_camera_zure[cameraId], ogawa_camera_mode[cameraId]);
    } else {
        if ((cameraPathPoint < playerPathPoint) && ((playerPathPoint - cameraPathPoint) < 3)) {
            cam_speed_objct[cameraId] = gPlayers[playerId].speed + 0.1;
        }
        if ((playerPathPoint < cameraPathPoint) && ((cameraPathPoint - playerPathPoint) < 3)) {
            cam_speed_objct[cameraId] = gPlayers[playerId].speed - 0.1;
        }
        if (cam_speed_objct[cameraId] > 10.0) {
            cam_speed_objct[cameraId] = 10.0f;
        }
        if (cam_speed_objct[cameraId] < 0.0) {
            cam_speed_objct[cameraId] = 0.0f;
        }
    }
    pathPoint1 = (oga_cam_area[cameraId] + 1) % pathPointCount;
    pathPoint2 = (oga_cam_area[cameraId] + 2) % pathPointCount;
    obj_calc_from_zure_BP(pathPoint1, oga_camera_zure[cameraId], pathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    obj_calc_from_zure_BP(pathPoint2, oga_camera_zure[cameraId], pathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[pathIndex][pathPoint1].pointy + CenterPathBP[pathIndex][pathPoint2].pointy) / 2.0;
    xdiff = midX - old_oga_cam_x[cameraId];
    ydiff = midY - old_oga_cam_y[cameraId];
    zdiff = midZ - old_oga_cam_z[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = old_oga_cam_x[cameraId] + ((cam_speed[cameraId] * xdiff) / distance);
        midY = old_oga_cam_y[cameraId] + ((cam_speed[cameraId] * ydiff) / distance);
        midZ = old_oga_cam_z[cameraId] + ((cam_speed[cameraId] * zdiff) / distance);
    } else {
        midX = old_oga_cam_x[cameraId];
        midY = old_oga_cam_y[cameraId];
        midZ = old_oga_cam_z[cameraId];
    }
    camera->camera_pos[0] = midX;
    camera->camera_pos[2] = midZ;
    camera->camera_pos[1] = player->position[1] + 10.0;
    old_oga_cam_x[cameraId] = midX;
    old_oga_cam_y[cameraId] = midY;
    old_oga_cam_z[cameraId] = midZ;
    camera->lookat_pos[0] = player->position[0];
    camera->lookat_pos[1] = player->position[1] + 6.0;
    camera->lookat_pos[2] = player->position[2];
    cam_check_bump(cameraId, pathIndex);
    sp94 = camera->lookat_pos[0] - camera->camera_pos[0];
    sp90 = camera->lookat_pos[1] - camera->camera_pos[1];
    sp8C = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(sp94, sp8C);
    camera->camera_direction[0] = Atan2t(sqrtf((sp94 * sp94) + (sp8C * sp8C)), sp90);
    camera->camera_direction[2] = 0;
}

void ogawa_kaiten_init(s32 playerId, UNUSED f32 arg1, s32 cameraId, s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    s32 test = g_courseTotalPathPoints[pathIndex];

    oga_camera_zure[cameraId] = zure_data[playerId];
    oga_cam_area[cameraId] = ((gNearestPathPointByPlayerId[playerId] + test) - 5) % test;

    obj_calc_from_zure_dist_BP(oga_cam_area[cameraId], zure_data[playerId], 60.0f,
                                    pathIndex);

    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_y[cameraId] = (f32) CenterPathBP[pathIndex][oga_cam_area[cameraId]].pointy;
    old_oga_cam_z[cameraId] = object[2];

    cam_speed_objct[cameraId] = gPlayers[playerId].speed;
    cam_speed[cameraId] = gPlayers[playerId].speed;
    D_8016448C = 1;
    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_kaitenview(Camera* camera, UNUSED Player* unusePlayer, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 stackPadding0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    Player* player;
    f32 distance;
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
    s32 playerId;
    s32 pathIndex;
    s32 pathPointCount;

    player = gPlayerOne;
    playerId = camera->watch;
    pathIndex = CurrentPathID[playerId];
    pathPointCount = g_courseTotalPathPoints[pathIndex];
    cam_speed[cameraId] = gPlayers[playerId].speed;
    if (D_8016448C == 0) {
        if (oga_cam_area[cameraId] ==
            (((gNearestPathPointByPlayerId[playerId] + pathPointCount) - 6) % pathPointCount)) {
            D_8016448C = 1;
        }
        if (oga_camera_zure[cameraId] < (zure_data[playerId] - 0.2)) {
            cam_speed[cameraId] = gPlayers[playerId].speed * 0.7;
        }
        if ((zure_data[playerId] - 0.5) < oga_camera_zure[cameraId]) {
            oga_camera_zure[cameraId] -= 0.01;
        }
        if (oga_camera_zure[cameraId] < -0.9) {
            oga_camera_zure[cameraId] = -0.9f;
            cam_speed[cameraId] = gPlayers[playerId].speed * 0.8;
        }
    } else {
        if (oga_cam_area[cameraId] == ((gNearestPathPointByPlayerId[playerId] + 6) % pathPointCount)) {
            D_8016448C = 0;
        }
        if ((zure_data[playerId] + 0.2) < oga_camera_zure[cameraId]) {
            cam_speed[cameraId] = gPlayers[playerId].speed * 1.3;
        }
        if (oga_camera_zure[cameraId] < (zure_data[playerId] + 0.5)) {
            oga_camera_zure[cameraId] += 0.01;
        }
        if (oga_camera_zure[cameraId] > 0.9) {
            oga_camera_zure[cameraId] = 0.9f;
            cam_speed[cameraId] = gPlayers[playerId].speed * 1.2;
        }
    }
    oga_camera_watch_num = playerId;
    player += playerId;
    oga_cam_area[cameraId] =
        get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], pathIndex);
    playerPathPoint = gNearestPathPointByPlayerId[playerId];
    cameraPathPoint = oga_cam_area[cameraId];
    if (exist_area_chk(playerPathPoint, cameraPathPoint, 0x000FU, 0x000FU, pathPointCount) <= 0) {
        oga_demo_camera_re_init(cameraId, oga_camera_zure[cameraId], ogawa_camera_mode[cameraId]);
    }
    pathPoint1 = (oga_cam_area[cameraId] + 1) % pathPointCount;
    pathPoint2 = (oga_cam_area[cameraId] + 2) % pathPointCount;
    obj_calc_from_zure_dist_BP(pathPoint1, oga_camera_zure[cameraId], 60.0f, pathIndex);
    midX = object[0] * 0.5;
    midZ = object[2] * 0.5;
    obj_calc_from_zure_dist_BP(pathPoint2, oga_camera_zure[cameraId], 60.0f, pathIndex);
    midX += object[0] * 0.5;
    midZ += object[2] * 0.5;
    midY = (CenterPathBP[pathIndex][pathPoint1].pointy + CenterPathBP[pathIndex][pathPoint2].pointy) / 2.0;
    xdiff = midX - old_oga_cam_x[cameraId];
    ydiff = midY - old_oga_cam_y[cameraId];
    zdiff = midZ - old_oga_cam_z[cameraId];
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance != 0.0) {
        midX = old_oga_cam_x[cameraId] + ((cam_speed[cameraId] * xdiff) / distance);
        midY = old_oga_cam_y[cameraId] + ((cam_speed[cameraId] * ydiff) / distance);
        midZ = old_oga_cam_z[cameraId] + ((cam_speed[cameraId] * zdiff) / distance);
    } else {
        midX = old_oga_cam_x[cameraId];
        midY = old_oga_cam_y[cameraId];
        midZ = old_oga_cam_z[cameraId];
    }
    camera->camera_pos[0] = midX;
    camera->camera_pos[2] = midZ;
    temp_f2_4 = CheckHight(midX, midY + 30.0, midZ);
    if ((temp_f2_4 < (midY - 20.0)) || (temp_f2_4 >= 3000.0)) {
        camera->camera_pos[1] = midY + 10.0;
    } else {
        camera->camera_pos[1] = temp_f2_4 + 8.0;
    }
    old_oga_cam_x[cameraId] = midX;
    old_oga_cam_y[cameraId] = midY;
    old_oga_cam_z[cameraId] = midZ;
    camera->lookat_pos[0] = player->position[0];
    camera->lookat_pos[1] = player->position[1] + 6.0;
    camera->lookat_pos[2] = player->position[2];
    cam_check_bump(cameraId, pathIndex);
    spAC = camera->lookat_pos[0] - camera->camera_pos[0];
    spA8 = camera->lookat_pos[1] - camera->camera_pos[1];
    spA4 = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(spAC, spA4);
    camera->camera_direction[0] = Atan2t(sqrtf((spAC * spAC) + (spA4 * spA4)), spA8);
    camera->camera_direction[2] = 0;
}

void ogawa_goal_init(s32 playerId, f32 arg1, s32 cameraId, UNUSED s16 pathIndex) {
    Camera* camera = cameras + cameraId;
    s32 test = g_courseTotalPathPoints[0];
    f32 temp_f12;
    f32 temp_f2;

    oga_camera_zure[cameraId] = arg1;
    oga_cam_area[cameraId] = gNearestPathPointByPlayerId[playerId] + 12;
    oga_cam_area[cameraId] = oga_cam_area[cameraId] % test;

    obj_calc_from_zure_BP(oga_cam_area[cameraId], arg1, 0);
    old_oga_cam_x[cameraId] = object[0];
    old_oga_cam_z[cameraId] = object[2];
    temp_f2 = (f32) CenterPathBP[0][oga_cam_area[cameraId]].pointy;

    temp_f12 = CheckHight(object[0], (temp_f2 + 30.0), object[2]);

    if ((temp_f12 < (temp_f2 - 20.0)) || (temp_f12 >= 3000.0)) {
        old_oga_cam_y[cameraId] = (f32) (temp_f2 + 10.0);
    } else {
        old_oga_cam_y[cameraId] = (f32) (temp_f12 + 10.0);
    }
    cam_speed[cameraId] = 0.0f;
    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
}

void ogawa_goalview(Camera* camera, UNUSED Player* playerArg, UNUSED s32 arg2, s32 cameraId) {
    s16 cameraPathPoint;
    s16 playerPathPoint;
    UNUSED s32 pad;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    UNUSED s32 pad2;
    s32 playerId;
    UNUSED s32 pad3[10];
    Player* player;
    CenterPathStruct* pathPoint;
    s32 pathIndex;
    s32 pathPointCount;

    playerId = camera->watch;
    pathIndex = CurrentPathID[playerId];
    player = gPlayerOne;
    player += playerId;
    pathPointCount = g_courseTotalPathPoints[pathIndex];
    oga_cam_area[cameraId] =
        get_oga_cam_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], oga_cam_area[cameraId], 0);
    playerPathPoint = gNearestPathPointByPlayerId[playerId];
    cameraPathPoint = oga_cam_area[cameraId];
    if (exist_area_chk(playerPathPoint, cameraPathPoint, 0x0032U, 0x0014U, pathPointCount) <= 0) {
        oga_demo_camera_re_init(cameraId, oga_camera_zure[cameraId], ogawa_camera_mode[cameraId]);
    } else {
        if ((zure_data[playerId] < -0.5) && ((f64) oga_camera_zure[cameraId] < -0.5)) {
            oga_demo_camera_re_init(cameraId, 1.0f, 0x0000000D);
        } else if ((zure_data[playerId] > 0.5) && ((f64) oga_camera_zure[cameraId] > 0.5)) {
            oga_demo_camera_re_init(cameraId, -1.0f, 0x0000000C);
        }
    }
    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
    pathPoint = &CenterPathBP[pathIndex][cameraPathPoint];
    camera->lookat_pos[0] = (player->position[0] * 0.8) + (0.2 * pathPoint->pointx);
    camera->lookat_pos[1] = (player->position[1] * 0.8) + (0.2 * pathPoint->pointy);
    camera->lookat_pos[2] = (player->position[2] * 0.8) + (0.2 * pathPoint->pointz);
    cam_check_bump(cameraId, pathIndex);
    xdiff = camera->lookat_pos[0] - camera->camera_pos[0];
    ydiff = camera->lookat_pos[1] - camera->camera_pos[1];
    zdiff = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(xdiff, zdiff);
    camera->camera_direction[0] = Atan2t(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    camera->camera_direction[2] = 0;
}

void oga_gp_rose_ErhnamDjinn(void) {
    kotei_gakaku_mode[0] = 3;
}

void ogawa_nopoint_init(UNUSED s32 playerId, UNUSED f32 arg1, s32 cameraId, UNUSED s16 pathIndex) {
    CenterPathStruct* pathPoint;

    oga_cam_area[cameraId] = g_courseTotalPathPoints[0] - 18;

    pathPoint = &CenterPathBP[0][oga_cam_area[cameraId]];

    old_oga_cam_x[cameraId] = pathPoint->pointx;
    //! @bug Adding an (f32) cast changes asm, why?
    old_oga_cam_y[cameraId] = pathPoint->pointy + 10.0;
    old_oga_cam_z[cameraId] = pathPoint->pointz;
    cam_speed[cameraId] = 0.0f;
    kotei_gakaku_mode[cameraId] = 0;
}

void ogawa_nopointview(Camera* camera, UNUSED Player* player, UNUSED s32 arg2, s32 cameraId) {
    UNUSED s32 pad[2];
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    CenterPathStruct* temp_v1;

    camera->camera_pos[0] = old_oga_cam_x[cameraId];
    camera->camera_pos[1] = old_oga_cam_y[cameraId];
    camera->camera_pos[2] = old_oga_cam_z[cameraId];
    temp_v1 = &(*CenterPathBP)[oga_cam_area[cameraId]];
    camera->lookat_pos[0] = (f32) temp_v1->pointx;
    camera->lookat_pos[1] = (f32) temp_v1->pointy;
    camera->lookat_pos[2] = (f32) temp_v1->pointz;
    cam_check_bump(cameraId, 0);
    xdiff = camera->lookat_pos[0] - camera->camera_pos[0];
    ydiff = camera->lookat_pos[1] - camera->camera_pos[1];
    zdiff = camera->lookat_pos[2] - camera->camera_pos[2];
    camera->camera_direction[1] = Atan2t(xdiff, zdiff);
    camera->camera_direction[0] = Atan2t(sqrtf((xdiff * xdiff) + (zdiff * zdiff)), ydiff);
    camera->camera_direction[2] = 0;
}

void oga_camera_mode_init(s32 playerId, s32 cameraId) {
    s32 pathIndex;
    Camera* camera = camera1;
    camera += cameraId;
    camera->watch = playerId;

    katamuki_theta[cameraId] = 0;
    pathIndex = CurrentPathID[playerId];

    switch (ogawa_camera_mode[cameraId]) {
        case 0:
            ogawa_video_init(playerId, 0.0f, cameraId);
            break;
        case 2:
            ogawa_kotei_init(playerId, -1.0f, cameraId, pathIndex);
            break;
        case 3:
            ogawa_kotei_init(playerId, 1.0f, cameraId, pathIndex);
            break;
        case 6:
            ogawa_oikake_init(playerId, -0.6f, cameraId, (s16) pathIndex);
            break;
        case 7:
            ogawa_oikake_init(playerId, 0.6f, cameraId, (s16) pathIndex);
            break;
        case 4:
            ogawa_side_init(playerId, -1.0f, cameraId, (s16) pathIndex);
            break;
        case 5:
            ogawa_side_init(playerId, 1.0f, cameraId, (s16) pathIndex);
            break;
        case 9:
            ogawa_quick_init(playerId, 0.0f, cameraId);
            break;
        case 1:
            ogawa_face_init(playerId, 0.0f, cameraId, (s16) pathIndex);
            break;
        case 14:
            ogawa_back_init(playerId, 0.0f, cameraId, (s16) pathIndex);
            break;
        case 8:
            ogawa_kaiten_init(playerId, 0.0f, cameraId, (s16) pathIndex);
            break;
        case 12:
            ogawa_goal_init(playerId, -1.0f, cameraId, (s16) pathIndex);
            break;
        case 13:
            ogawa_goal_init(playerId, 1.0f, cameraId, (s16) pathIndex);
            break;
        case 15:
            ogawa_nopoint_init(playerId, -1.0f, cameraId, (s16) pathIndex);
            break;
        case 16:
            ogawa_nopoint_init(playerId, 1.0f, cameraId, (s16) pathIndex);
            break;
        default:
            ogawa_video_init(playerId, 0.0f, cameraId);
            break;
    }
    oga_cam_area[cameraId] =
        check_oga_area_BP(camera->camera_pos[0], camera->camera_pos[1], camera->camera_pos[2], (s32) pathIndex);
    if ((s16) ogawa_camera_mode[cameraId] == 9) {
        D_80163DD8[cameraId] = (s32) pathIndex;
    }
}

void oga_mokuhyou_angle_move(s32 cameraIndex, u16 arg1) {
    u16 var_v0;

    var_v0 = katamuki_theta[cameraIndex];
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
    katamuki_theta[cameraIndex] = (s16) var_v0;
}

void oga_gakaku_set(s32 playerIndex) {
    switch (kotei_gakaku_mode[playerIndex]) {
        case 0:
            if (D_80164608[playerIndex] == 1) {
                kotei_gakaku_mode[playerIndex] = 1;
                NAPlyTrgStart(playerIndex, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x4F));
                gakaku_mode[playerIndex] = kotei_gakaku_mode[playerIndex];
            }
            break;
        case 1:
            if (D_80164608[playerIndex] == 1) {
                kotei_gakaku_mode[playerIndex] = 0;
                NAPlyTrgStart(playerIndex, SOUND_ARG_LOAD(0x19, 0x00, 0x90, 0x50));
                gakaku_mode[playerIndex] = kotei_gakaku_mode[playerIndex];
            }
            break;
    }
}

void oga_bank_roll(Camera* camera, Player* player, s32 arg2) {
    s32 playerId;
    s32 nearestPathPoint;

    playerId = camera->watch;
    if ((oga_frame_counter != 0) && (g_courseID == COURSE_LUIGI_RACEWAY)) {
        oga_camera_rotate(camera, arg2);
        nearestPathPoint = gNearestPathPointByPlayerId[playerId];
        if (((nearestPathPoint >= 0x65) && (nearestPathPoint < 0xFA)) ||
            ((nearestPathPoint >= 0x1AF) && (nearestPathPoint < 0x226))) {
            oga_mokuhyou_angle_move(arg2, (player->kartroll * 2));
        } else {
            oga_mokuhyou_angle_move(arg2, 0U);
        }
    }
}

void oga_drivers_point_camera_init(void) {
    D_801646CC = 1;
}

void oga_vs_winner_camera_init(void) {
    s32 i;
    s32 playerId = rank_to_num[0];
    // clang-format off
    // Has to be on a single line to match. Because IDO hates you :)
    for (i = 0; i < 4; i++) { gakaku_mode[i] = kotei_gakaku_mode[i]; }
    // clang-format on
    camera1->watch = playerId;
    kotei_gakaku_mode[0] = 1;
    D_801646CC = 2;
}

void oga_timeattack_camera_init(void) {
    ogawa_camera_mode[0] = 1;
    oga_camera_mode_init(0, 0);
    gakaku_mode[0] = kotei_gakaku_mode[0];
    kotei_gakaku_mode[0] = 1;
    ogawa_camera_mode[1] = 9;
    oga_camera_mode_init(0, 1);
    gakaku_mode[1] = kotei_gakaku_mode[1];
    kotei_gakaku_mode[1] = 0;
}

void oga_battle_camera_init(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        gakaku_mode[i] = kotei_gakaku_mode[i];
    }

    rank_to_num[0] = (s16) win_kart;

    camera1->watch = (s16) win_kart;

    for (i = 0; i < 4; i++) {
        ogawa_camera_mode[i] = 0;
        ogawa_video_init(win_kart, 0, i);
        kotei_gakaku_mode[i] = 1;
    }
}

void OGAContTrgChk(s32 cameraId) {
    struct Controller* controller;

    controller = &gControllerOne[cameraId];
    if (controller->ButtonPressed & L_CBUTTONS) {
        D_801645D0[cameraId] += 1;
    } else {
        D_801645D0[cameraId] = 0;
    }
    if (controller->ButtonPressed & D_CBUTTONS) {
        D_801645E8[cameraId] += 1;
    } else {
        D_801645E8[cameraId] = 0;
    }
    if (controller->ButtonPressed & U_CBUTTONS) {
        D_80164608[cameraId] += 1;
    } else {
        D_80164608[cameraId] = 0;
    }
    if (controller->ButtonPressed & R_CBUTTONS) {
        D_80164628[cameraId] += 1;
    } else {
        D_80164628[cameraId] = 0;
    }
}

void oga_game_camera_all(UNUSED u16* arg0, UNUSED Camera* arg1, UNUSED Player* arg2, UNUSED s8 arg3, s32 arg4) {
    OGAContTrgChk(arg4);
    oga_gakaku_set(arg4);
}

void oga_game_camera(u16* arg0, Camera* arg1, Player* arg2, s8 arg3, s32 arg4) {
    oga_game_camera_all(arg0, arg1, arg2, arg3, arg4);
    oga_bank_roll(arg1, arg2, arg4);
}

void goal_camera_set_GP(s32 arg0, s32 arg1) {
    switch (gGPCurrentRaceRankByPlayerId[arg0]) { /* irregular */
        case 0:
            if (MakeRandomLimmit(0x0064U) < 0x32) {
                ogawa_camera_mode[arg1] = 0x000C;
            } else {
                ogawa_camera_mode[arg1] = 0x000D;
            }
            Na_PanDeforume();
            break;
        case 1:
        case 2:
        case 3:
            ogawa_camera_mode[arg1] = 8;
            break;
        default:
            if (MakeRandomLimmit(0x0064U) < 0x32) {
                ogawa_camera_mode[arg1] = 0x000F;
            } else {
                ogawa_camera_mode[arg1] = 0x0010;
            }
            break;
    }
}

void goal_camera_set_VS(UNUSED s32 arg0, s32 cameraId) {
    switch (MakeRandomLimmit(6)) {
        case 0:
            ogawa_camera_mode[cameraId] = 4;
            break;
        case 1:
            ogawa_camera_mode[cameraId] = 5;
            break;
        case 2:
            ogawa_camera_mode[cameraId] = 6;
            break;
        case 3:
            ogawa_camera_mode[cameraId] = 7;
            break;
        case 4:
            ogawa_camera_mode[cameraId] = 8;
            break;
        case 5:
            ogawa_camera_mode[cameraId] = 1;
            break;
        default:
            ogawa_camera_mode[cameraId] = 8;
            break;
    }
}

s32 oga_demo_camera_area_chk_sub(s32 pathPoint, s32 arg1) {
    if ((g_courseID == COURSE_BOWSER_CASTLE) && (arg1 != 0) && (pathPoint >= 0xE7) && (pathPoint < 0x1C2)) {
        arg1 = 0;
    }
    return arg1;
}

void oga_demo_camera_re_init(s32 cameraId, f32 arg1, s32 arg2) {
    UNUSED s32 pad;
    s32 playerId;

    playerId = cameras[cameraId].watch;
    oga_camera_zure[cameraId] = arg1;
    ogawa_camera_mode[cameraId] = oga_demo_camera_area_chk_sub((s32) oga_cam_area[cameraId], arg2);
    oga_camera_mode_init(playerId, cameraId);
}

void oga_demo_camera_sound_init(s32 arg0, f32 arg1, s32 arg2) {
    s32 playerId;

    playerId = cameras[arg0].watch;
    oga_camera_zure[arg0] = arg1;
    if (arg2 != ogawa_camera_mode[arg0]) {
        ogawa_camera_mode[arg0] = arg2;
        oga_camera_mode_init(playerId, arg0);
    }
}

void oga_demo_camera_inc_change_check(s32 playerId, s32 arg1, s32 arg2) {
    s32 temp_v1;
    s16 pathPoint;
    s32 temp_v0;

    if (!(gPlayers[playerId].slip_flag & (HEIGHT_JUMP | N_JUMP | DASH_MIDJUMP))) {
        temp_v1 = ogawa_camera_mode[arg1];
        pathPoint = oga_cam_area[arg1];
        temp_v0 = oga_demo_camera_area_chk_sub(pathPoint, (temp_v1 + 1) % 10);
        if ((temp_v0 != temp_v1) || (arg2 != playerId)) {
            ogawa_camera_mode[arg1] = temp_v0;
            oga_camera_mode_init(arg2, arg1);
        }
    }
}

void oga_demo_camera_rank_set(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg1) { /* irregular */
        case 0:
            ogawa_camera_mode[arg2] = 1;
            break;
        case 1:
        case 2:
        case 3:
            oga_demo_camera_inc_change_check(arg0, arg2, arg0);
            break;
        default:
            ogawa_camera_mode[arg2] = 0;
            break;
    }
}

void Func_8001A588(UNUSED u16* localD_80152300, Camera* camera, Player* player, s8 index, s32 cameraIndex) {
    s32 var_v1;
    UnkStruct_46D0* temp_v0_4;
    s32 sp44;
    s32 playerId;
    playerId = camera->watch;

    if (g_gameMode == TIME_TRIALS) {
        playerId = 0;
    }
    OGAContTrgChk(cameraIndex);

    if ((s32) (camera->camera_pos[0] * 10.0) == (s32) ((f64) camera->lookat_pos[0] * 10.0)) {

        if ((s32) (camera->camera_pos[2] * 10.0) == (s32) ((f64) camera->lookat_pos[2] * 10.0)) {
            camera->camera_pos[0] = (f32) (camera->camera_pos[0] + 100.0);
            camera->camera_pos[2] = (f32) (camera->camera_pos[2] + 100.0);
        }
    }
    if ((g_gameMode != BATTLE) && (ogawa_camera_mode[cameraIndex] == -1) && (player->flag & IS_RACE_FINISH) &&
        ((u16) D_801646CC == 0) && (oga_goal_counter == 0)) {
        if (g_gameMode == VERSUS) {
            goal_camera_set_VS(playerId, cameraIndex);
        } else {
            goal_camera_set_GP((s32) playerId, cameraIndex);
        }
        oga_camera_mode_init((s32) playerId, cameraIndex);
    }

    if ((ogawa_camera_mode[cameraIndex] == 14) || (ogawa_camera_mode[cameraIndex] == 0)) {
        oga_bank_roll(camera, player, cameraIndex);
    } else {
        katamuki_theta[cameraIndex] = 0;
        oga_camera_rotate(camera, cameraIndex);
    }
    switch ((u16) D_801646CC) {
        case 1:
            oga_goal_counter += 1;
            if (oga_goal_counter >= 501) {
                oga_goal_counter = 0;
            }
            if ((cameraIndex == 0) && (((oga_goal_counter == 10)) || (oga_goal_counter == 11))) {
                oga_demo_camera_rank_set((s32) playerId, gGPCurrentRaceRankByPlayerId[playerId], 0);
            }
            if ((g_gameMode != TIME_TRIALS) && (cameraIndex == 1) &&
                (((oga_goal_counter == 260)) || (oga_goal_counter == 261))) {

                var_v1 = 0;
                if (g_menuMultiplayerSelection == 2) {
                    oga_demo_camera_rank_set((s32) playerId, gGPCurrentRaceRankByPlayerId[playerId], 1);
                } else {
                    sp44 = (s32) playerId;
                    while (var_v1 != 8) {
                        playerId += 1;
                        var_v1 += 1;
                        if (playerId >= 8) {
                            playerId = 1;
                        }
                        if ((!(gPlayers[playerId].jugemu_flag & ON_LAKITU_ROD) &&
                             !(gPlayers[playerId].jugemu_flag & LAKITU_SCENE))) {
                            break;
                        }
                    }
                    oga_demo_camera_inc_change_check(sp44, cameraIndex, (s32) playerId);
                }
            }
            break;
        case 2:
            oga_goal_counter += 1;
            if (oga_goal_counter > 250) {
                oga_goal_counter = 0;
            }
            if ((cameraIndex == 0) && (oga_goal_counter == 10)) {
                oga_demo_camera_inc_change_check((s32) playerId, cameraIndex, (s32) playerId);
            }
            break;
        default:
            temp_v0_4 = &oga_sound_camera[cameraIndex];
            if (temp_v0_4->unk0 == (s16) 1) {
                playerId = temp_v0_4->unk4;
                temp_v0_4->unk0 = 0;
                cameras[cameraIndex].watch = playerId;
                oga_demo_camera_sound_init(cameraIndex, 0.0f, (s32) temp_v0_4->unk2);
            }
            break;
    }
    oga_gakaku_set(cameraIndex);
    switch (ogawa_camera_mode[cameraIndex]) {
        case 0:
            ogawa_videoview(camera, player, index);
            break;
        case 2:
        case 3:
            ogawa_koteiview(camera, player, index, cameraIndex);
            break;
        case 6:
        case 7:
            ogawa_oikakeview(camera, player, index, cameraIndex);
            break;
        case 4:
        case 5:
            ogawa_sideview(camera, player, index, cameraIndex);
            break;
        case 9:
            ogawa_quickview(camera, player, index, cameraIndex);
            break;
        case 1:
            ogawa_faceview(camera, player, index, cameraIndex);
            break;
        case 14:
            ogawa_backview(camera, player, index, cameraIndex);
            break;
        case 8:
            ogawa_kaitenview(camera, player, index, cameraIndex);
            break;
        case 12:
        case 13:
            ogawa_goalview(camera, player, index, cameraIndex);
            break;
        case 15:
        case 16:
            ogawa_nopointview(camera, player, index, cameraIndex);
            break;
        default:
            ogawa_videoview(camera, player, index);
            break;
    }
}

void ogawa_sound_camera_set(s16 arg0, s16 arg1, s16 arg2) {
    if (oga_sound_camera[arg0].unk0 == 0) {
        oga_sound_camera[arg0].unk0 = 1;
        oga_sound_camera[arg0].unk2 = arg1;
        oga_sound_camera[arg0].unk4 = arg2;
    }
}

#include "cpu_vehicles_camera_path/cpu_item_strategy.inc.c"

void enemy_item_move(s32 playerId) {
    Player* player = &gPlayerOne[playerId];
    Object* actor;
    EnemyItem* cpuStrategy = &enemy_item[playerId];
    CenterPathStruct* pathPoint;
    bool isValidBanana1;
    bool isValidBanana2;

// Only used in this function
#define BANANA_ACTOR(actor) ((struct BananaActor*)(actor))
#define SHELL_ACTOR(actor) ((struct ShellActor*)(actor))
#define FAKE_ITEMBOX_ACTOR(actor) ((struct FakeItemBox*)(actor))
#define BANANA_BUNCH_ACTOR(actor) ((struct BananaBunchParent*)(actor))

    if (g_gameMode == TIME_TRIALS) {
        return;
    }

    if ((u16) D_801646CC == 1) {
        return;
    }

    if (player->flag & IS_RACE_FINISH) {
        return;
    }

    switch (cpuStrategy->item_num) {
        case CPU_STRATEGY_WAIT_NEXT_ITEM:
            cpuStrategy->obj_num = -1;
            if ((((playerId * 20) + 100) < g_playerPathPointTotalTable[playerId]) && (cpuStrategy->time >= 0x259) &&
                (cpuStrategy->limit < 3) && (gLapCountByPlayerId[playerId] < 3)) {
                enemy_item_number_set(playerId, &cpuStrategy->item_num,
                                            kwgetitem_enemy((s16) gLapCountByPlayerId[playerId],
                                                                gGPCurrentRaceRankByPlayerId[playerId]));
            } else {
                enemy_item_debug_BANDOU(playerId, cpuStrategy);
            }
            break;

        case CPU_STRATEGY_ITEM_BANANA:
            // never true
            if ((gLapCountByPlayerId[playerId] > 0) && (gGPCurrentRaceRankByPlayerId[playerId] > gGPCurrentRaceRankByPlayerId[g_EnemyTargetPlayer]) && (gGPCurrentRaceRankByPlayerId[g_EnemyTargetPlayer] == FIRST_PLACE)) {
                switch (player->kart) {
                    case DK:
                        if (exist_area_chk(gNearestPathPointByPlayerId[playerId],
                                                    gNearestPathPointByPlayerId[g_EnemyTargetPlayer], 40, 2,
                                                    center_line_max) > 0) {
                            cpuStrategy->item_num = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;

                    case PEACH:
                        if (exist_area_chk(gNearestPathPointByPlayerId[playerId],
                                                    gNearestPathPointByPlayerId[g_EnemyTargetPlayer], 4, 2,
                                                    center_line_max) > 0) {
                            cpuStrategy->item_num = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;

                    default:
                        if (exist_area_chk(gNearestPathPointByPlayerId[playerId],
                                                    gNearestPathPointByPlayerId[g_EnemyTargetPlayer], 10, 2,
                                                    center_line_max) > 0) {
                            cpuStrategy->item_num = CPU_STRATEGY_THROW_BANANA;
                        }
                        break;
                }
            } else if (cpuStrategy->item_num == CPU_STRATEGY_ITEM_BANANA) {
                cpuStrategy->obj_num = put_banana(player);
                if ((cpuStrategy->obj_num >= 0) && (cpuStrategy->obj_num < 100)) {
                    player->weapon |= USE_BANANA;
                    cpuStrategy->item_num = CPU_STRATEGY_HOLD_BANANA;
                    cpuStrategy->time = 0;
                    cpuStrategy->limit += 1;
                    cpuStrategy->count4 = (MakeRandomLimmit(3) * 20) + 10;
                } else {
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->time = 0;
                }
            }
            break;

        case CPU_STRATEGY_HOLD_BANANA:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->category != ACTOR_BANANA) || (BANANA_ACTOR(actor)->sparam != HELD_BANANA) ||
                (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->category != 6) {}
                if (BANANA_ACTOR(actor)->sparam != 0) {}
                if (BANANA_ACTOR(actor)->angle[0] != playerId) {}

                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->time = 0;
                player->weapon &= ~USE_BANANA;
            } else if (cpuStrategy->count4 < cpuStrategy->time) {
                cpuStrategy->item_num = CPU_STRATEGY_DROP_BANANA;
            }
            break;

        case CPU_STRATEGY_DROP_BANANA:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->category != ACTOR_BANANA)) ||
                    (BANANA_ACTOR(actor)->sparam != HELD_BANANA)) ||
                (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->category != 6) {}
                if (BANANA_ACTOR(actor)->sparam != 0) {}
                if (BANANA_ACTOR(actor)->angle[0] != playerId) {}

            } else {
                BANANA_ACTOR(actor)->sparam = DROPPED_BANANA;
                BANANA_ACTOR(actor)->velocity[0] = 0.0f;
                BANANA_ACTOR(actor)->velocity[1] = 0.0f;
                BANANA_ACTOR(actor)->velocity[2] = 0.0f;
                if (tenuki_flg[playerId] == true) {
                    BANANA_ACTOR(actor)->position[1] =
                        CheckHight(player->position[0], player->position[1] + 30.0, player->position[2]) +
                        (BANANA_ACTOR(actor)->radius + 1.0f);
                }
            }
            player->weapon &= ~USE_BANANA;
            cpuStrategy->time = 0;
            cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            break;

        case CPU_STRATEGY_THROW_BANANA:
            cpuStrategy->obj_num = put_banana(player);
            if ((cpuStrategy->obj_num >= 0) && (cpuStrategy->obj_num < 100)) {
                actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
                BANANA_ACTOR(actor)->sparam = BANANA_ON_GROUND;
                player->weapon |= USE_BANANA;
                cpuStrategy->item_num = CPU_STRATEGY_HOLD_THROW_BANANA;
                cpuStrategy->time = 0;
                cpuStrategy->limit += 1;
                pathPoint = &CenterPathBP[CurrentPathID[0]]
                                        [(gNearestPathPointByPlayerId[g_EnemyTargetPlayer] + 30) %
                                            g_courseTotalPathPoints[CurrentPathID[g_EnemyTargetPlayer]]];
                BANANA_ACTOR(actor)->velocity[0] = (pathPoint->pointx - player->position[0]) / 20.0;
                BANANA_ACTOR(actor)->velocity[1] = ((pathPoint->pointy - player->position[1]) / 20.0) + 4.0;
                BANANA_ACTOR(actor)->velocity[2] = (pathPoint->pointz - player->position[2]) / 20.0;
                BANANA_ACTOR(actor)->position[1] = player->position[1];
                Na_EnmVoice_Start(playerId, SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x09));
                NAEnmTrgStart(player->position, player->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x14));
            } else {
                cpuStrategy->time = 0;
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_THROW_BANANA:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->category != ACTOR_BANANA)) ||
                    (BANANA_ACTOR(actor)->sparam != BANANA_ON_GROUND)) ||
                (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->category != 6) {}
                if (BANANA_ACTOR(actor)->sparam != 0) {}
                if (BANANA_ACTOR(actor)->angle[0] != playerId) {}

                cpuStrategy->time = 0;
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                player->weapon &= ~USE_BANANA;
            } else {
                BANANA_ACTOR(actor)->velocity[1] -= 0.4;
                BANANA_ACTOR(actor)->position[0] += BANANA_ACTOR(actor)->velocity[0];
                BANANA_ACTOR(actor)->position[1] += BANANA_ACTOR(actor)->velocity[1];
                BANANA_ACTOR(actor)->position[2] += BANANA_ACTOR(actor)->velocity[2];
                if (cpuStrategy->time > 20) {
                    cpuStrategy->item_num = CPU_STRATEGY_END_THROW_BANANA;
                }
            }
            break;

        case CPU_STRATEGY_END_THROW_BANANA:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) || (BANANA_ACTOR(actor)->category != ACTOR_BANANA)) ||
                    (BANANA_ACTOR(actor)->sparam != BANANA_ON_GROUND)) ||
                (playerId != BANANA_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(BANANA_ACTOR(actor)->flag & EXISTOBJ)) {}
                if (BANANA_ACTOR(actor)->category != 6) {}
                if (BANANA_ACTOR(actor)->sparam != 0) {}
                if (BANANA_ACTOR(actor)->angle[0] != playerId) {}

            } else {
                BANANA_ACTOR(actor)->sparam = DROPPED_BANANA;
                BANANA_ACTOR(actor)->velocity[0] = 0.0f;
                BANANA_ACTOR(actor)->velocity[1] = 0.0f;
                BANANA_ACTOR(actor)->velocity[2] = 0.0f;
                BANANA_ACTOR(actor)->position[1] =
                    CheckHight(BANANA_ACTOR(actor)->position[0], BANANA_ACTOR(actor)->position[1] + 30.0, BANANA_ACTOR(actor)->position[2]) +
                    (BANANA_ACTOR(actor)->radius + 1.0f);
            }
            player->weapon &= ~USE_BANANA;
            cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->time = 0;
            break;

        case CPU_STRATEGY_ITEM_GREEN_SHELL:
            if (g_simpleObjectCount < 80) {
                cpuStrategy->obj_num = set_gshell(player);
                if ((cpuStrategy->obj_num >= 0) && (cpuStrategy->obj_num < 100)) {
                    cpuStrategy->item_num = CPU_STRATEGY_HOLD_GREEN_SHELL;
                    cpuStrategy->time = 0;
                    cpuStrategy->limit += 1;
                    cpuStrategy->count4 = (MakeRandomLimmit(3) * 20) + 10;
                } else {
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_GREEN_SHELL:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(actor->flag & EXISTOBJ)) || (actor->category != ACTOR_GREEN_SHELL)) ||
                    (actor->sparam != HELD_SHELL)) ||
                (playerId != actor->angle[2])) {

                // FAKE
                if (!(actor->flag & EXISTOBJ)) {}
                if (actor->category != 7) {}
                if (actor->sparam != 0) {}
                if (actor->angle[0] != playerId) {}

                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->time = 0;
            } else if (cpuStrategy->count4 < cpuStrategy->time) {
                cpuStrategy->item_num = CPU_STRATEGY_THROW_GREEN_SHELL;
                cpuStrategy->time = 0;
            }
            break;

        case CPU_STRATEGY_THROW_GREEN_SHELL:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(actor->flag & EXISTOBJ)) || (actor->category != ACTOR_GREEN_SHELL)) ||
                    (actor->sparam != HELD_SHELL)) ||
                (playerId != actor->angle[2])) {

                // FAKE
                if (!(actor->flag & EXISTOBJ)) {}
                if (actor->category != 7) {}
                if (actor->sparam != 0) {}
                if (actor->angle[0] != playerId) {}

                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->time = 0;
            } else {
                actor->sparam = RELEASED_SHELL;
                cpuStrategy->time = 0;
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_ITEM_RED_SHELL:
            if (g_simpleObjectCount < 80) {
                cpuStrategy->obj_num = set_rshell(player);
                if ((cpuStrategy->obj_num >= 0) && (cpuStrategy->obj_num < 100)) {
                    cpuStrategy->item_num = CPU_STRATEGY_HOLD_RED_SHELL;
                    cpuStrategy->time = 0;
                    cpuStrategy->limit += 1;
                    cpuStrategy->count4 = (MakeRandomLimmit(3) * 20) + 10;
                } else {
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_RED_SHELL:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(SHELL_ACTOR(actor)->flag & EXISTOBJ)) || (SHELL_ACTOR(actor)->category != ACTOR_RED_SHELL)) ||
                    (SHELL_ACTOR(actor)->sparam != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(actor->flag & EXISTOBJ)) {}
                if (actor->category != 8) {}
                if (actor->sparam != 0) {}
                if (actor->angle[0] != playerId) {}

                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->time = 0;
            } else if (cpuStrategy->count4 < cpuStrategy->time) {
                cpuStrategy->item_num = CPU_STRATEGY_THROW_RED_SHELL;
            }
            break;

        case CPU_STRATEGY_THROW_RED_SHELL:
            check_item_number(cpuStrategy);
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(SHELL_ACTOR(actor)->flag & EXISTOBJ)) || (SHELL_ACTOR(actor)->category != ACTOR_RED_SHELL)) ||
                    (SHELL_ACTOR(actor)->sparam != HELD_SHELL)) ||
                (playerId != SHELL_ACTOR(actor)->playerId)) {

                // FAKE
                if (!(actor->flag & EXISTOBJ)) {}
                if (actor->category != 8) {}
                if (actor->sparam != 0) {}
                if (actor->angle[0] != playerId) {}

                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->time = 0;
            } else {
                SHELL_ACTOR(actor)->sparam = RELEASED_SHELL;
                cpuStrategy->time = 0;
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_ITEM_BANANA_BUNCH:
            if (g_simpleObjectCount < 80) {
                cpuStrategy->obj_num = set_super_banana(player);
                if ((cpuStrategy->obj_num >= 0) && (cpuStrategy->obj_num < 100)) {
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH;
                    cpuStrategy->time = 0;
                    cpuStrategy->limit += 1;
                    cpuStrategy->count4 = (MakeRandomLimmit(3) * 20) + 60;
                } else {
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            } else {
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_WAIT_INIT_BANANA_BUNCH:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if (BANANA_BUNCH_ACTOR(actor)->sparam == 6) {

                // FAKE
                if (BANANA_BUNCH_ACTOR(actor)->sparam != -1) {}
                if (BANANA_BUNCH_ACTOR(actor)->sparam == 6) {}

                isValidBanana2 = false;

                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[4] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[3] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[2] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[1] != (-1)) {
                    isValidBanana2 = true;
                }
                if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[0] != (-1)) {
                    isValidBanana2 = true;
                }
                if ((BANANA_BUNCH_ACTOR(actor)->category != ACTOR_BANANA_BUNCH) || (isValidBanana2 == false)) {
                    if (BANANA_BUNCH_ACTOR(actor)->category != 14) {}
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->time = 0;
                } else if (cpuStrategy->count4 < cpuStrategy->time) {
                    cpuStrategy->item_num = CPU_STRATEGY_DROP_BANANA_BUNCH;
                    cpuStrategy->count1 = 0;
                    cpuStrategy->time = 0;
                }
            }
            break;

        case CPU_STRATEGY_DROP_BANANA_BUNCH:
            if (((cpuStrategy->time) % 10) == 0) {
                if (cpuStrategy->count1 < 5) {
                    isValidBanana1 = 0;
                    actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
                    switch (cpuStrategy->count1) {
                        case 0:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[4] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 1:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[3] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 2:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[2] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 3:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[1] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;

                        case 4:
                            if (BANANA_BUNCH_ACTOR(actor)->bananaIndices[0] != (-1)) {
                                isValidBanana1 = true;
                            }
                            break;
                    }

                    if (((BANANA_BUNCH_ACTOR(actor)->category == ACTOR_BANANA_BUNCH) && (BANANA_BUNCH_ACTOR(actor)->sparam == 6)) &&
                        (isValidBanana1 == true)) {
                        release_banana((struct BananaBunchParent*)actor);
                    }
                    cpuStrategy->count1 += 1;
                } else {
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                    cpuStrategy->time = 0;
                }
            }
            break;

        case CPU_STRATEGY_ITEM_FAKE_ITEM_BOX:
            cpuStrategy->obj_num = put_fake_itembox(player);
            if ((cpuStrategy->obj_num >= 0) && (cpuStrategy->obj_num < 100)) {
                cpuStrategy->item_num = CPU_STRATEGY_HOLD_FAKE_ITEM_BOX;
                cpuStrategy->time = 0;
                cpuStrategy->limit += 1;
                cpuStrategy->count4 = (MakeRandomLimmit(3) * 20) + 10;
            } else {
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            break;

        case CPU_STRATEGY_HOLD_FAKE_ITEM_BOX:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(FAKE_ITEMBOX_ACTOR(actor)->flag & EXISTOBJ)) || (FAKE_ITEMBOX_ACTOR(actor)->category != ACTOR_FAKE_ITEM_BOX)) ||
                    (FAKE_ITEMBOX_ACTOR(actor)->sparam != 0)) ||
                (playerId != ((s32) FAKE_ITEMBOX_ACTOR(actor)->playerId))) {

                // FAKE
                if (!(actor->flag & EXISTOBJ)) {}
                if (actor->category != 13) {}
                if (actor->sparam != 0) {}
                if (actor->angle[0] != playerId) {}

                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->time = 0;
            } else if (cpuStrategy->count4 < cpuStrategy->time) {
                cpuStrategy->item_num = CPU_STRATEGY_THROW_FAKE_ITEM_BOX;
            }
            break;

        case CPU_STRATEGY_THROW_FAKE_ITEM_BOX:
            actor = &g_SimpleObjectArray[cpuStrategy->obj_num];
            if ((((!(FAKE_ITEMBOX_ACTOR(actor)->flag & EXISTOBJ)) || (FAKE_ITEMBOX_ACTOR(actor)->category != ACTOR_FAKE_ITEM_BOX)) ||
                    (FAKE_ITEMBOX_ACTOR(actor)->sparam != 0)) ||
                (playerId != ((s32) FAKE_ITEMBOX_ACTOR(actor)->playerId))) {

                // FAKE
                if (!(FAKE_ITEMBOX_ACTOR(actor)->flag & EXISTOBJ)) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->category != 13) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->sparam != 0) {}
                if (FAKE_ITEMBOX_ACTOR(actor)->angle[0] != playerId) {}

            } else {
                release_fake_itembox((struct FakeItemBox*)actor);
                if (tenuki_flg[playerId] == true) {
                    FAKE_ITEMBOX_ACTOR(actor)->position[1] =
                        CheckHight(FAKE_ITEMBOX_ACTOR(actor)->position[0], FAKE_ITEMBOX_ACTOR(actor)->position[1] + 30.0, FAKE_ITEMBOX_ACTOR(actor)->position[2]) +
                        FAKE_ITEMBOX_ACTOR(actor)->radius;
                }
            }
            cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->time = 0;
            break;

        case CPU_STRATEGY_ITEM_THUNDERBOLT:
            use_thunder(player);
            Na_EnmThunder_Start(playerId);
            ThunderWorld();
            cpuStrategy->item_num = CPU_STRATEGY_END_THUNDERBOLT;
            cpuStrategy->time = 0;
            cpuStrategy->limit += 1;
            break;

        case CPU_STRATEGY_END_THUNDERBOLT:
            if (cpuStrategy->time >= 0xF1) {
                Na_EnmThunder_Stop((s32) ((u8) playerId));
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                cpuStrategy->time = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_STAR:
            player->weapon |= USE_STAR;
            cpuStrategy->item_num = CPU_STRATEGY_END_ITEM_STAR;
            cpuStrategy->time = 0;
            cpuStrategy->limit += 1;
            break;

        case CPU_STRATEGY_END_ITEM_STAR:
            if (!(player->slip_flag & STAR)) {
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            cpuStrategy->time = 0;
            break;

        case CPU_STRATEGY_ITEM_BOO:
            player->weapon |= USE_BOO;
            cpuStrategy->item_num = CPU_STRATEGY_WAIT_END_BOO;
            cpuStrategy->time = 0;
            cpuStrategy->limit += 1;
            break;

        case CPU_STRATEGY_WAIT_END_BOO:
            if (!(player->slip_flag & TERESA)) {
                cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            }
            cpuStrategy->time = 0;
            break;

        case CPU_STRATEGY_ITEM_MUSHROOM:
            player->weapon |= USE_MUSHROOM;
            cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
            cpuStrategy->time = 0;
            cpuStrategy->limit += 1;
            break;

        case CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM:
            if (cpuStrategy->time >= 0x3D) {
                player->weapon |= USE_MUSHROOM;
                cpuStrategy->item_num = CPU_STRATEGY_ITEM_MUSHROOM;
                cpuStrategy->time = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_TRIPLE_MUSHROOM:
            if (cpuStrategy->time >= 0x3D) {
                player->weapon |= USE_MUSHROOM;
                cpuStrategy->item_num = CPU_STRATEGY_ITEM_DOUBLE_MUSHROOM;
                cpuStrategy->time = 0;
            }
            break;

        case CPU_STRATEGY_ITEM_SUPER_MUSHROOM:
            cpuStrategy->item_num = CPU_STRATEGY_USE_SUPER_MUSHROOM;
            cpuStrategy->time = 0;
            cpuStrategy->count4 = 0x0258;
            break;

        case CPU_STRATEGY_USE_SUPER_MUSHROOM:
            if ((((s16) cpuStrategy->time) % 60) == 0) {
                player->weapon |= USE_MUSHROOM;
                if (cpuStrategy->count4 < cpuStrategy->time) {
                    cpuStrategy->time = 0;
                    cpuStrategy->item_num = CPU_STRATEGY_WAIT_NEXT_ITEM;
                }
            }
            break;

        default:
            break;
    }

    if (cpuStrategy->time < 10000) {
        cpuStrategy->time += 1;
    }
    if (player->slip_flag & (TERESA | TURBO | STAR)) {
        cpuStrategy->time = 0;
    }
}

#undef BANANA_ACTOR
#undef SHELL_ACTOR
#undef FAKE_ITEMBOX_ACTOR
#undef BANANA_BUNCH

void oga_hyousyou_car_init_sub(void) {
    Player* temp_s1;
    CenterPathStruct* temp_s0;
    s32 i;

    init_enemy_kart();
    for (i = 0; i < 4; i++) {
        temp_s1 = &gPlayerOne[i];
        temp_s1->flag &= 0xDFFF;
        CurrentPathID[i] = i;
        LaneData[i].lane_free_drive = 0.0f;
        LaneData[i].lane_target = 0.0f;
        LaneData[i].lane = 0.0f;
        switch (i) {
            case 0:
                gNearestPathPointByPlayerId[i] = 6;
                break;
            case 1:
                gNearestPathPointByPlayerId[i] = 1;
                break;
            case 2:
                gNearestPathPointByPlayerId[i] = 6;
                break;
            case 3:
                gNearestPathPointByPlayerId[i] = 1;
                break;
        }
        temp_s0 = &CenterPathBP[i][gNearestPathPointByPlayerId[i]];
        temp_s1->position[0] = (f32) temp_s0->pointx;
        temp_s1->position[1] =
            CheckHight((f32) temp_s0->pointx, 2000.0f, (f32) temp_s0->pointz) + temp_s1->radius;
        temp_s1->position[2] = (f32) temp_s0->pointz;
        temp_s1->direction[1] = (s16) *AnglePathDataBP[i];
        enemy_stick_angle(temp_s1, 0);
        temp_s1++;
        oga_near_hyousyou_flg[i] = 0;
    }
}

void oga_hyousyou_init(void) {
    g_courseID = COURSE_AWARD_CEREMONY;
    oga_hyousyou_demo_trg = 0;
    oga_nopoint_demo_trg = 0;
    oga_hyousyou_demo_counter = 0;
    oga_nopoint_demo_counter = 0;
    InitCenterLine();
    ogawa_camera_init(0);
    oga_hyousyou_car_init_sub();
    oga_hyousyou_mx[0] = -3202.475097656f;
    oga_hyousyou_my[0] = 19.166999817f;
    oga_hyousyou_mz[0] = -477.623992920f;
    oga_hyousyou_mx[1] = -3205.080078125f;
    oga_hyousyou_my[1] = 19.166999817f;
    oga_hyousyou_mz[1] = -462.851989746f;
    oga_hyousyou_mx[2] = -3199.870117188f;
    oga_hyousyou_my[2] = 19.166999817f;
    oga_hyousyou_mz[2] = -492.395996094f;
    oga_hyousyou_mx[3] = -2409.197021484f;
    oga_hyousyou_my[3] = 0.0f;
    oga_hyousyou_mz[3] = -355.253997803;
}

void oga_hyousyou_move(void) {
    f32 temp_f0;
    f32 temp_f2;
    s32 playerId;
    Player* temp_s0;

    if (oga_hyousyou_demo_trg == 1) {
        oga_hyousyou_demo_counter += 1;
    }
    if ((oga_nopoint_demo_trg == 1) && (bom[0].state == 0) && (resultstat.rank >= 3)) {
        oga_nopoint_demo_counter++;
        if (oga_nopoint_demo_counter >= 0xF) {
            oga_nopoint_demo_counter = 0;
            oga_nopoint_demo_trg = 2;
            StartKawanoResult();
        }
    }
    for (playerId = 0; playerId < 4; playerId++) {
        if ((playerId == 3) && (oga_hyousyou_demo_trg == 0)) {
            break;
        }

        temp_s0 = &gPlayerOne[playerId];
        drive_enemy_kart(playerId);
        if (!(temp_s0->flag & NO_CONTROLS)) {
            temp_f0 = oga_hyousyou_mx[playerId] - temp_s0->position[0];
            temp_f2 = oga_hyousyou_mz[playerId] - temp_s0->position[2];
            if ((f64) ((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < 1.0) {
                if (playerId != 3) {
                    if (1) {}
                    // Why oh why is a ternary required here? Who does that?
                    (oga_hyousyou_demo_trg == 0) ? (temp_s0->flag |= NO_CONTROLS)
                                      : (temp_s0->flag &= ~NO_CONTROLS);
                    if ((gPlayerOne->flag & NO_CONTROLS) && (gPlayerTwo->flag & NO_CONTROLS) &&
                        (gPlayerThree->flag & NO_CONTROLS)) {
                        oga_hyousyou_demo_trg = 1;
                        oga_hyousyou_demo_counter = 0;
                    }
                } else if (oga_nopoint_demo_trg == 0) {
                    if (!(temp_s0->slip_flag & EXPLODE)) {
                        temp_s0->flag |= NO_CONTROLS;
                    }
                    oga_nopoint_demo_trg = 1;
                    oga_nopoint_demo_counter = 0;
                } else if (!(temp_s0->slip_flag & EXPLODE)) {
                    temp_s0->flag |= NO_CONTROLS;
                }
            }
        }
    }
}

void ogawa_draw(s32 cameraId) {
    if (g_courseID == COURSE_AWARD_CEREMONY) {
        if (bom[0].pathPointIndex >= 16) {
            kwdisplay_bombkart(PLAYER_FOUR);
        }
    } else {
        if (g_gameMode == VERSUS) {
            kwdisplay_bombkart(cameraId);
        }
    }
}

UNUSED void ogawa_debug_draw(void) {
    if (D_800DDB20 == 0) {
        if ((gControllerThree->ButtonPressed & L_TRIG) != 0) {
            D_800DDB20 = 1;
        }
    } else {
        if ((gControllerThree->ButtonPressed & L_TRIG) != 0) {
            D_800DDB20 = 0;
        }
        KWSetViewportFull();
        gSPDisplayList(gDisplayListHead++, D_0D0076F8);
        KWReturnViewport();
    }
}
