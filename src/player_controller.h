#ifndef player_controller_H
#define player_controller_H

#include <common_structs.h>
#include "camera.h"

/* Function Prototypes */

void SpinKart(Player*, Camera*, s8, s8);

s32 check_kart_number(Player*);
void start_kart_engine(Player*, s8);
void kart_engine_echo(Player*, s8);

void TirePosition(Player*, f32, f32, f32);
void openkart(Player* player, Camera* camera, s8 arg2, s8 playerId);

void mariogp_movekart(Player*, Camera*, s8, s8);
void vs_time_movekart(Player*, Camera*, s8, s8);
void battle_movekart(Player*, Camera*, s8, s8);
void movekart(Player*, Camera*, s8, s8);
void func_80028F5C(s32, s32, s32, s32);
void kartpos_control_full(void);

void kartpos_control_ru(void);
void kartpos_control_ld(void);
void kartpost_control_1P(void);
void kartpost_control_2P(void);
void kartpos_control_3P(void);
void kartpos_control_4P(void);
void make_degpict(Player*, s8);
void kart_posset(Player*, Camera*, s8, s8);

void EnemyTirePosition(Player*, f32, f32, f32);
void hit_velocity(Vec3f, f32, Vec3f, f32, f32);
void set_start_dush(Player*, s8);
void drift_turbo(Player*, s8);
void check_drift_turbo(Player*, s8);
void jump_set(Player*);
void DriftJump(Player*);
void AddGravity(Player*);
void set_nature_vector(void);
void check_nature_vector(void);
void check_direction(void);
void SetSlipAngle(Player*, s8, f32, f32, f32, f32);

void enemy_checkdrift(Player*);
void set_weapon(Player*, s8, s8);
void check_weapon(Player*, s8, s8);
void WeaponStatus(Player*, s8, s8);
void set_hitptr(Player*, Player*);
void check_impact(Player*, s8, s32);
void check_impactsterr(Player*);
void check_slipstream(Player*, s8);

void drift_counter(Player*);
void LakituCheck(Player*, s8);
void CheckSplash(Player*, s8);
void CheckKartHit(Player*, s8, s8);
void CheckWall(Player*, s8, Vec3f);
void SetStrategy(Player*, s8, s8);

void opening_kart(Player*, s8);
void RunKart(Player*, Camera*, s8, s8);

void set_hight(Player*);
void RunKartSimple(Player*, Camera*, s8, s8);

void SlipCheck(Player*, s8);
void make_taco(Player*, f32);

f32 PowerCheck(Player*, s8);
void shake_set(Player*);
void check_accele_off(Player*);
void check_accele_on(Player*);
void AccelOn(Player*);
void AccelOff(Player*, f32);
void check_brake_off(Player*);
void check_brake_on(Player*);
void breake_on(Player*);
void player_nocontaccele_on(Player*);
void player_nocontaccele_off(Player*, f32);
void jugemu_accele_on(Player*);
void jugemu_accele_off(Player*, f32);
void empty_accele_on(Player*, s32);
void empty_accele_off(Player*, f32, s32);
void recover_grip(Player*, f32);
void check_sterr_speedgrip(Player*, s32*, s32*, s32, s32, s32, s32);
void check_sterr_speedgrip1(Player*, s32*, s32, s32, f32);
void check_sterr_speed1(Player*, s32*, s32, s32, f32);
void check_sterr_speed(Player*, s32*, s32*, s32, s32, s32, f32);
void ProStickAngle(Player*, struct Controller*, s8);

void enemy_stick_angle(Player*, s16);
void drift_set(Player*);
void drift_reset(Player*);
void setvector_l(Player*, Vec3f, Vec3f);

void setvector_r(Player*, Vec3f, Vec3f);
void enemy_setvector_l(Player*, Vec3f, Vec3f);
void enemy_setvector_r(Player*, Vec3f, Vec3f);
void battle_setvector_l(Player*, Vec3f, Vec3f);
void battle_setvector_r(Player*, Vec3f, Vec3f);
void yoko_g(Player* player, Vec3f);
void manual_stick_drive(Player*, struct Controller*, s8);

void drive_stick_kindcontrol(Player*, struct Controller*, s8);
void drive_stick_control(void);
s16 check_stickX(struct Controller*);
s16 check_stickY(struct Controller*);
void result_accele_on(Player*, s16);
void result_runkart(Player*, Camera*, s8, s8);

/* This is where I'd put my static data, if I had any */

extern s16 cpu_forMario[];
extern s16 cpu_forLuigi[];
extern s16 cpu_forYoshi[];
extern s16 cpu_forToad[];
extern s16 cpu_forDk[];
extern s16 cpu_forWario[];
extern s16 cpu_forPeach[];
extern s16 cpu_forBowser[];
extern s16* cpu_forPlayer[];

extern s16 cpu_forMarioAndLuigi[];
extern s16 cpu_forMarioAndYoshi[];
extern s16 cpu_forMarioAndToad[];
extern s16 cpu_forMarioAndDk[];
extern s16 cpu_forMarioAndWario[];
extern s16 cpu_forMarioAndPeach[];
extern s16 cpu_forMarioAndBowser[];
extern s16 cpu_forLuigiAndMario[];
extern s16 cpu_forLuigiAndYoshi[];
extern s16 cpu_forLuigiAndToad[];
extern s16 cpu_forLuigiAndDk[];
extern s16 cpu_forLuigiAndWario[];
extern s16 cpu_forLuigiAndPeach[];
extern s16 cpu_forLuigiAndBowser[];
extern s16 cpu_forYoshiAndMario[];
extern s16 cpu_forYoshiAndLuigi[];
extern s16 cpu_forYoshiAndToad[];
extern s16 cpu_forYoshiAndDk[];
extern s16 cpu_forYoshiAndWario[];
extern s16 cpu_forYoshiAndPeach[];
extern s16 cpu_forYoshiAndBowser[];
extern s16 cpu_forToadAndMario[];
extern s16 cpu_forToadAndLuigi[];
extern s16 cpu_forToadAndYoshi[];
extern s16 cpu_forToadAndDk[];
extern s16 cpu_forToadAndWario[];
extern s16 cpu_forToadAndPeach[];
extern s16 cpu_forToadAndBowser[];
extern s16 cpu_forDkAndMario[];
extern s16 cpu_forDkAndLuigi[];
extern s16 cpu_forDkAndToad[];
extern s16 cpu_forDkAndYoshi[];
extern s16 cpu_forDkAndWario[];
extern s16 cpu_forDkAndPeach[];
extern s16 cpu_forDkAndBowser[];
extern s16 cpu_forWarioAndMario[];
extern s16 cpu_forWarioAndLuigi[];
extern s16 cpu_forWarioAndYoshi[];
extern s16 cpu_forWarioAndDk[];
extern s16 cpu_forWarioAndToad[];
extern s16 cpu_forWarioAndPeach[];
extern s16 cpu_forWarioAndBowser[];
extern s16 cpu_forPeachAndMario[];
extern s16 cpu_forPeachAndLuigi[];
extern s16 cpu_forPeachAndYoshi[];
extern s16 cpu_forPeachAndDk[];
extern s16 cpu_forPeachAndWario[];
extern s16 cpu_forPeachAndToad[];
extern s16 cpu_forPeachAndBowser[];
extern s16 cpu_forBowserAndMario[];
extern s16 cpu_forBowserAndLuigi[];
extern s16 cpu_forBowserAndYoshi[];
extern s16 cpu_forBowserAndDk[];
extern s16 cpu_forBowserAndWario[];
extern s16 cpu_forBowserAndToad[];
extern s16 cpu_forBowserAndPeach[];
extern s16* gListCPUforMario[];
extern s16* gListCPUforLuigi[];
extern s16* gListCPUforYoshi[];
extern s16* gListCPUforToad[];
extern s16* gListCPUforDk[];
extern s16* gListCPUforWario[];
extern s16* gListCPUforPeach[];
extern s16* gListCPUforBowser[];
extern s16** cpu_forTwoPlayer[];

extern s16 D_801656F0;

#endif
