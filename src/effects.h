#ifndef EFFECTS_H
#define EFFECTS_H

#include <common_structs.h>

/* Function Prototypes */

void kwGetTimeInit(void);
s32 kwStartTime(s32*);
void kwGetTime(s32*, s32);
void kwAddTime(s32);
void kwGetLine(s32);
void kwSetTime(void);
void kwdebug(void);
void kwdebug_items(void);
void kwdebug_rcp(void);
void debugsyori_test(void);
void kwdebug_anmbreak(void);
void kwdebug_mtxkazu(void);
void kwdebug_gptrkazu(void);
void kwdebug_kumoram(void);
void kwdebug_rmonprint_1p(void);
void debugsyori_1p(void);
void debugsyori_2p_lr(void);
void debugsyori_2p_ud(void);
void debugsyori_4p_kleft(void);
void debugsyori_4p_kright(void);
void debugsyori_4p_kup(void);
void debugsyori_4p_kdown(void);
void debugsyori_4p(void);
void kwdisplay_debug(void);
void kwdebug_print_common(void);
void kwdebugprint_player_sub(void);
void kwdebugprint_ogawa(void);
void kwdebugprint_player(s32, s32);
void kwdebugprint_syori(void);
void kwdebugprint_test(void);
void kwdebugprint_1p(void);
void kwdebugprint_2p_left(void);
void kwdebugprint_2p_right(void);
void kwdebugprint_2p_up(void);
void kwdebugprint_2p_down(void);
void kwdebugprint_4p_upleft(void);
void kwdebugprint_4p_upright(void);
void kwdebugprint_4p_downleft(void);
void kwdebugprint_4p_downright(void);
void check_hit_letter(Player*);
void stop_strat(void);
void kill_strat(Player*, s8);
void SetRollover(Player*, s8);
void rollover(Player*, s8);
void reset_rollover(Player*, s8);
void SetSpin(Player*, s8);
void reset_spin(Player*, s8);
void spin(Player*, s8);
void SetWheelspin(Player*, s8);
void wheelspin(Player*, s8);
void reset_wheelspin(Player*, s8);
void SetProWheelSpin(Player*, s8);
void pro_wheelspin(Player*, s8);
void reset_pro_wheelspin(Player*, s8);
void SetTurbo(Player*, s8);
void now_turbo(Player*);
void reset_turbo(Player*);
void SetWing(Player*, s8);
void wing(Player*, s8);
void ResetWing(Player*);
void SetStorm(Player*, s8);
void storm(Player*, s8);
void reset_storm(Player*);
void roll_brokenkart(Player*);
void SetBroken(Player*, s8);
void broken(Player*, s8);
void SetThunder(Player*, s8);
void thunder(Player*, s8);
void reset_thunder(Player*, s8);
void bomb(Player*, s8);
void SetBombRollover(Player*, s8);
void reset_bomb(Player*, s8);
void bomb_throw(Player*, s8);
void SetBombThrowRollover(Player*, s8);
void reset_bomb_throw(Player*, s8);
void set_dashjump(Player*, s8);
void now_dashjump(Player*);
void reset_dashjump(Player*);
void set_dash_midjump(Player*, s8);
void now_dash_midjump(Player*);
void reset_dash_midjump(Player*);
void set_rapidacc(Player*, s8);
void rapidacc(Player*, s8);
void reset_rapidacc(Player*);
void rollover_fall(Player*, s8);
void SetRolloverFall(Player*, s8);
void ResetRolloverFall(Player*, s8);
void star(Player*, s8);
void SetStar(Player*, s8);
void ResetStar(Player*, s8);
void vs_ghost(Player*, s8);
void SetVSGhost(Player*, s8);
void ResetVSGhost(Player*, s8);
void kill_bomb_kart(Player*);
void change_bomb(Player*, s8);
void real_bomb(Player*, s8);
void set_change_bomb(Player*, s8);
void set_real_bomb(Player*, s8);
void SetHeightJump(Player*, s8);
void height_jump(Player*, s8);
void reset_height_jump(Player*, s8);
void make_chasepoint(Player*, s8);
void func_80090178(Player*, s8, Vec3f, Vec3f);
void SetLakitu(Player*);
void func_80090868(Player*);
void HangLakitu(Player*, s8, s8);
bool check_itembutton(Player*);
void set_result_jump(Player*, s8);
void result_jump(Player*, s8);

// audio/external.c
void Na_PlyDash_Start(u8);
void Na_StarBGM_Start(u8);
void Na_StarBGM_Stop(u8);
void Na_EnmMuteki_Start(u8);
void Na_PlyObake_Start(u8);
void Na_PlyObake_Stop(u8);

extern s32 D_8018D900[];
extern s16 D_8018D920[];
extern s32 g_StarUseCounter[];
extern s32 g_GhostUseCounter[];
extern s32 g_GhostUseTimer[];
extern s32 D_8018D990[];

/* This is where I'd put my static data, if I had any */

extern f32 D_800E3710[];
extern f32 D_800E3730[];
extern f32 D_800E3750[];
extern f32 D_800E3770[];
extern f32 D_800E37B0[];
extern f32 D_800E3790[];
extern f32 D_800E37D0[];
extern f32 D_800E37F0[];
extern s16 compel_flag[4][8];
extern f32 accele_count[];
extern s32 accele_time[];

#endif
