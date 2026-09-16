#ifndef SPAWN_PLAYERS_H
#define SPAWN_PLAYERS_H

#include "camera.h"
#include <common_structs.h>

/* Function Prototypes */
/** @cond */
void initializePlayer(Player*, s8, f32, f32, f32, f32, u16, s16);
void set_up_view_distance(void);
void make_rank(void);
void mario_gp1P(f32*, f32*, f32);
void time_attack(f32*, f32*, f32);
void mario_gp2P(f32* arg0, f32* arg1, f32);
void vs_race2P(f32*, f32*, f32);
void battle2P(f32*, f32*, f32);
void vs_race3P(f32*, f32*, f32);
void battle3P(f32*, f32*, f32);
void vs_race4P(f32*, f32*, f32);
void battle4P(f32*, f32*, f32);
void result_kart_pos(void);
void reset_pos(void);
void result_pos(void);
void init_draw(Player*, Camera*, s8, s8);
void initial_game(void);
void init_result_kart(void);

extern f32 grid_x[];
extern f32 grid_z[];
extern s16 kartrank[];
extern f32 accele_count[];
extern f32 g_waterlevelPlayer[];
extern s32 sterr_counter[];
extern s32 green_timer[];
extern s16 echo_flag[];
extern u16 jugemu_line[];
extern s16 g_playerPathPointCopy[];
extern s16 g_offroadFlagPlayer1[];
extern s16 tire_hight;
extern Player* hitPtr[];
extern bool accele_onoff_flag[];
extern s32 accele_inc_flag[];
extern s32 accele_time[];
extern s32 accele_flag[];
extern bool power_band_accele[];
extern s32 power_band_timer[];
extern bool brake_onoff_flag[];
extern s32 brake_inc_flag[];
extern s32 brake_timer[];
extern s32 brake_flag[];
extern bool full_brake_accele[];
extern s32 full_brake_timer[];
extern s16 cpu_chooseCharacters[];
extern s16 D_8016556E;
extern s16 select_dmaKart;
extern s16 select_reardmaKart;
extern s16 select_NondmaKart;
extern s16 select_rearNondmaKart;
extern s16 select_viewKart;
extern s16 select_rearviewKart;
extern s16 select_bumpKart;
extern s16 select_rearbumpKart;
extern s16 select_viewObj;
extern s16 select_rearviewObj;
/** @endcond */

#endif
