#ifndef cpu_vehicles_camera_path_H
#define cpu_vehicles_camera_path_H

#include "vehicles.h"
#include "camera.h"
#include "path.h"
#include <assets/common_data.h>

struct unexpiredActors {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u16 unkC;
    /* 0x0E */ u16 actorIndex;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ // s16 compilerPadding;
    /* 0x14 */ u32 unk14;
    /* 0x18 */ s32 unk18;
}; // size = 0x1C

typedef struct {
    /* 0x00 */ f32 lane;
    /* 0x04 */ f32 lane_target;
    /* 0x08 */ f32 speed;
    /* 0x0C */ f32 lane_free_drive;
} SOUKOU_ZURE_TYPE; // size = 0x10
typedef SOUKOU_ZURE_TYPE TrackPositionFactorInstruction;

// Something related to CPU item usage
typedef struct {
    /* 0x00 */ s16 item_num;
    /* 0x02 */ s16 obj_num;
    /* 0x04 */ s16 time;
    /* 0x06 */ s16 limit;
    /* 0x08 */ s16 count1;
    /* 0x0A */ s16 count2;
    /* 0x0C */ s16 count3;
    /* 0x0E */ s16 count4;
} EnemyItem; // size = 0x10
typedef EnemyItem CpuItemStrategyData;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} UnkStruct_46D0;

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
s16 oga_calc_direction(Vec3f, Vec3f);

s32 oga_hit_check(f32, f32, f32, f32, f32, f32, f32, f32);
void oga_guruguru_sub(Vec3f, Vec3f, s16);
s32 check_player_near(Vec3f, f32, s32);
void oga_reverse_chk(s32, Player*);
void sort_ogawa_rank(void);

void sort_goal_rank(void);
void sort_goal_rank_2PGP(void);
s32 exist_area_chk(u16, u16, u16, u16, u16);
void enemy_tekagen_sub(s32, Player*);
void oga_result_accel(s32, Player*, f32);

void oga_accele(s32, f32, Player*);
bool make_kart_distance(s32, s16, s16);
void new_bipas_num_init(s32);
s32 kyousei_bipas_set(s32, s32);
void pylon_count_reflesh(s32);

void maze_enter_or_exit_check(s32);
void maze_bipas_change_check(s32, Player*);
f32 oga_get_laptime(s32, f32, f32);
void CheckLapCount(s32, Player*);
void EtcEnemyDrive(void);
void oga_enemy_voice(s32, Player*);
void oga_enemy_voice_overtake(s32, Player*);
void drive_enemy_kart(s32);

void avoid_crash(s32);
s32 straight_chk(s32, u16);
bool course_out_chk(s32);
f32 zure_center_sub_BP(f32, f32, u16, s32);
void zure_chk_BP(s32, u16, s32);
void obj_calc_from_zure_dist_BP(u16, f32, f32, s16);
void obj_calc_from_zure_BP(u16, f32, s16);
s16 check_oga_area_BP(f32, f32, f32, s32);

s16 CalcOGAAreaSubBP(f32, f32, f32, u16, s32*);
s16 get_oga_area_group_sub_BP(f32, f32, f32, s16, s32, u16);
s16 get_oga_area_sub_BP(f32, f32, f32, s16, s32);
void kyousei_area_set_chk_enemy(f32, f32, f32, s16*, s32);
void WrapPathIndexAtFinish(f32, f32, f32, s16*, s32);
s16 calc_oga_area_kart(f32, f32, f32, Player*, s32, s32*);
s16 get_oga_area_kart(f32, f32, f32, s16, Player*, s32, s32);

s16 get_oga_ex_area(f32, f32, f32, s16);
s16 calc_oga_area_BP(f32, f32, f32, s32*);
s16 get_oga_area_BP(f32, f32, f32, s16, s32);
s16 get_oga_cam_area_BP(f32, f32, f32, s16, s32);
f32 soukou_zure_move(s32);
void mokuhyou_set_center(s32, u16);
s16 hanyou_soukou_sub_BP(Vec3f, s16*, f32, f32, s16, s16);
s16 hanyou_gyaku_soukou_sub_BP(Vec3f, s16*, f32, f32, s16);
s16 hanyou_soukou_sub_EX(Vec3f, s16*, f32);
void init_BOM(void);
void move_BOM(s32);

s32 search_set_hibasira_sub(s32, s16);
s32 kame_hibasira_set(s32);
s32 kame_blue_hibasira_set(s32);
s32 kame_toge_hibasira_set(s32);
void kame_hibasira_clear(s32);
void kame_hibasira_init(void);
void kame_hibasira_move(void);

void init_enemy_game_mode(void);
void set_rival_car_name(void);
void clear_point_data(CenterPathStruct*, size_t);
void InitCenterLine(void);
void init_enemy_kart(void);

void AreaBunkatuBP(s32);
void SidePointCalcBP(s32);
f32 curve_chk_BP(s32, u16);
void CurveDataCalcBP(s32);
s16 angle_chk_BP(s32, s32);
void AngleDataCalcBP(s32);
void ShortcutDataCalcBP(s32);
f32 calc_cy_init(f32, f32, f32, s32, s32);
f32 calc_cy(f32, f32, f32, s32, s32);

s32 area_bunkatu_sub(CenterPathStruct*, CenterPathStruct*, s32, s32);
s32 area_read_rom_sub(CenterPathStruct*, CenterPathStruct*);
s32 area_bunkatu_SL_SHIP_sub(Path2D*, CenterPathStruct*, s32);
void init_act_dat_table(void);
void action_flg_sleep(s32);
void action_flg_wakeup(s32);
void ActionStartCheck(s32, Player*);
void action_end_check(s32, Player*);
void action_start_end_chk(s32);
void act_drift_start(s32, Player*, s32, u16);

void area_bunkatu_SL(void);
void area_bunkatu_SHIP(void);
void obj_init_highway_sub(VehicleStuff*);
void ogawa_hanyou_object_init(void);
void clear_SL_tab(TrainCarStuff*, Path2D*, u16);
void init_SL(void);
void put_object_buf_SL(TrainCarStuff*, s16);
void move_SL(void);
void hit_check_SL(s32, Player*);

void fumikiri_move(void);
void fumikiri_wait_check(s32);
void init_SHIP(void);
void move_SHIP(void);
void hit_check_SHIP(Player*);
void init_HIGHWAY_CAR_sub(f32, f32, s32, s32, VehicleStuff*, CenterPathStruct*);
f32 shasen_chk_sub(s16, s16);
void move_HIGHWAY_CAR_sub(VehicleStuff*);
void hit_check_HIGHWAY_CAR_sub(s32, Player*, VehicleStuff*, f32, f32, s32, u32);

f32 avoid_HIGHWAY_sub(s16, f32, s16);
void avoid_HIGHWAY_CAR_sub(s32, s32, VehicleStuff*);
void init_TRUCK(void);
void move_TRUCK(void);
void hit_check_TRUCK(s32, Player*);
void avoid_TRUCK(s32);
void init_BUS(void);
void move_BUS(void);
void hit_check_BUS(s32, Player*);
void avoid_BUS(s32);
void init_TANK(void);
void move_TANK(void);
void hit_check_TANK(s32, Player*);
void avoid_TANK(s32);
void init_RV(void);
void move_RV(void);
void hit_check_RV(s32, Player*);
void avoid_RV(s32);
void cam_check_bump(s32, s32);
void ogawa_camera_init(s32);
f32 ogawa_gakaku(f32, s32);

void oga_camera_rotate(Camera*, s32);
void func_8001530C(void);
void ogawa_video_init(s32, f32, s32);
void ogawa_videoview(Camera*, Player*, s32);
void ogawa_kotei_init(s32, f32, s32, s32);
void ogawa_koteiview(Camera*, UNUSED Player*, s32, s32);
void ogawa_oikake_init(s32, f32, s32, s16);
void ogawa_oikakeview(Camera*, Player*, s32, s32);

void ogawa_side_init(s32, f32, s32, s16);
void ogawa_sideview(Camera*, Player*, s32, s32);
void ogawa_quick_init(s32, f32, s32);

void ogawa_face_init(s32, f32, s32, s16);
void ogawa_faceview(Camera*, Player*, s32, s32);
void ogawa_back_init(s32, f32, s32, s16);

void ogawa_backview(Camera*, Player*, s32, s32);
void ogawa_kaiten_init(s32, f32, s32, s16);
void ogawa_kaitenview(Camera*, Player*, s32, s32);

void ogawa_goal_init(s32, f32, s32, s16);
void ogawa_goalview(Camera*, UNUSED Player*, s32, s32);
void oga_gp_rose_ErhnamDjinn(void);
void ogawa_nopoint_init(s32, f32, s32, s16);
void ogawa_nopointview(Camera*, UNUSED Player*, s32, s32);
void oga_camera_mode_init(s32, s32);
void oga_mokuhyou_angle_move(s32, u16);
void oga_gakaku_set(s32);
void oga_bank_roll(Camera*, Player*, s32);
void oga_drivers_point_camera_init(void);
void oga_vs_winner_camera_init(void);
void oga_timeattack_camera_init(void);
void oga_battle_camera_init(void);
void OGAContTrgChk(s32);

void oga_game_camera_all(u16*, Camera*, Player*, s8, s32);
void oga_game_camera(u16*, Camera*, Player*, s8, s32);
void goal_camera_set_GP(s32, s32);
s32 oga_demo_camera_area_chk_sub(s32, s32);
void oga_demo_camera_re_init(s32, f32, s32);
void oga_demo_camera_sound_init(s32, f32, s32);
void oga_demo_camera_inc_change_check(s32, s32, s32);
void oga_demo_camera_rank_set(s32, s32, s32);
void Func_8001A588(u16*, Camera*, Player*, s8, s32);
void ogawa_sound_camera_set(s16, s16, s16);
void enemy_item_init(void);
void enemy_item_number_set(s32, s16*, s32);
void enemy_item_debug_BANDOU(s32, EnemyItem*);
void check_item_number(EnemyItem*);
void enemy_item_move(s32);

void oga_hyousyou_car_init_sub(void);

void oga_hyousyou_init(void);
void oga_hyousyou_move(void);
void ogawa_draw(s32);
void ogawa_debug_draw(void);

/* This is where I'd put my static data, if I had any */

extern Collision D_80162E70;
extern s16 D_80162EB0; // Possibly a float.
extern s16 D_80162EB2; // possibly [3]
extern ActionData* gCoursesCPUBehaviour[];
extern s16 pylon_num[];
extern s16 pylon_num2[];
extern Vec3f object;
extern Vec3f object1;
extern Vec3f object2;
extern s16 gTrainSmokeTimer;
extern s16 biri_flg;
extern f32 rap_dat[]; // D_80162FD8
extern s16 avoid_mode[];
extern s16 avoid_count[];
extern f32 speedlevel[];
extern s16 old_eos_angle_oga[];
extern f32 zure_dist[];
extern bool straight_flg[];
extern u16 oga_area_org;
extern s16 oga_area_chg_flg;
extern s16 g_driftFlg[];
extern s16 gFerrySmokeTimer;
extern s32 enemy_time_count[];
extern s32 enemy_dist[];
extern s32 enemy_limdist[];
extern f32 enemy_object_x[];
extern f32 enemy_object_z[];
extern s16 sVehicleSoundRenderCounter;
extern s32 best_time;
extern u16 tenuki_flg[];
extern u16 old_tenuki_flg[];
extern f32 limmitspeed;
extern f32 old_limmitspeed[];
extern s32 oga_camera_watch_num;
extern u16 rap_count_flg[];
extern u16 oga_reverse_count[];
extern u16 g_wrongwayFlagPlayer1[];
extern s32 old_pylon_count[];
// Exact pointer type unknown
extern ActionData* ActPointPtr;
extern u16 next_action_num[];
extern u16 old_action_num[];
extern u16 g_actionFlg[];

enum { CPU_BEHAVIOUR_STATE_NONE, CPU_BEHAVIOUR_STATE_START, CPU_BEHAVIOUR_STATE_RUNNING };

extern s16 oga_angle[];
extern u16 oga_car_stat[];
extern u16 enemy_rival_num[];
extern u16 enemy_rival_name[];
extern u16 enemy_rival_aim_rank[];
extern u16 enemy_accel_mode[];

enum { SPEED_CPU_BEHAVIOUR_NORMAL, SPEED_CPU_BEHAVIOUR_FAST, SPEED_CPU_BEHAVIOUR_SLOW, SPEED_CPU_BEHAVIOUR_MAX };

extern s32 oga_frame_counter;
extern s32 oga_etc_frame_counter;
extern s16 mokuhyou_add_area[];
extern s16 enemy_voice_count_SPIN[];
extern s16 enemy_voice_count_DAMAGE[];
extern s16 D_801633C8[];
extern s16 oga_hamidasi_map_flg[];
extern s16 g_noSimpleKartFlag[];
extern s16 oga_near_hyousyou_flg[];
extern f32 oga_hyousyou_mx[];
extern f32 oga_hyousyou_my[];
extern f32 oga_hyousyou_mz[];
extern f32 oga_old_rap_chk_z[];
extern s16 g_EnemyTargetPlayer;
// 0 or 1, only 1 when when in extra (mirror) mode
extern s16 highway_gyakusou_flg;
extern s16 oga_hyousyou_demo_trg;
extern s16 oga_nopoint_demo_trg;
extern s32 oga_hyousyou_demo_counter;
extern s32 oga_nopoint_demo_counter;
extern s32 oga_bump_chk_counter;
extern s16 g_rankUpdateFinishFlag;
extern s16 g_rivalOvertakeAllowFlag[];
extern s16 fumikiri_stop_flg[];
extern s16 ogawa_karakara_short_cut_ok_flg;
extern s32 oga_calc_loop_count;
extern s32 old_oga_calc_loop_count;
extern SOUKOU_ZURE_TYPE LaneData[];
extern Path2D* center_line_EX;
extern s32 g_CourseObstacle;
extern u16 fumikiri_mode[];
extern u16 fumikiri_counter[];
extern s32 D_80163DD8[];
extern struct unexpiredActors kame_hibasira[];
extern EnemyItem enemy_item[];
extern s16 enemy_free_item_debug_flg;
extern s16 enemy_free_item_debug_inum;
extern s16 enemy_free_item_debug_num;
extern s16 rank_to_num[]; // D_80164360
extern s16 real_rank_to_num[];          // D_80164378
extern s32 gLapCountByPlayerId[];          // D_80164390
extern s32 gGPCurrentRaceRankByPlayerId[]; // D_801643B8
extern s32 old_ogawa_rank[];
extern s32 real_rank[];
extern s16 name_to_num[];
extern s32 D_8016448C;
extern f32 cam_offset[];
extern f32 rate_count_F[];    // D_801644A8
extern f32 rap_count_F[]; // D_801644D0
extern f32 enemy_tenuki_y[];
extern s16 enemy_geta_rank[];
extern s32 D_801645D0[];
extern s32 D_801645E8[];
extern f32 old_oga_cam_x[];
extern s32 D_80164608[];
extern f32 old_oga_cam_y[];
extern s32 D_80164628[];
extern f32 old_oga_cam_z[];
extern f32 cam_speed[];
extern f32 cam_speed_objct[];
extern s16 gakaku_mode[];
extern s16 kotei_gakaku_mode[];
extern s16 ogawa_camera_mode[];
extern f32 oga_camera_zure[];
extern f32 oga_camera_bird_x;
extern f32 oga_camera_bird_y;
extern f32 oga_camera_bird_z;
extern s16 katamuki_theta[];
extern u32 oga_goal_counter;
extern u16 D_801646CC;
extern UnkStruct_46D0 oga_sound_camera[];

// See bss_80005FD0.s
extern f32 rap_dat[NUM_PLAYERS];
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
