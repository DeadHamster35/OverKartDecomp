#ifndef RACE_LOGIC_H
#define RACE_LOGIC_H

/* Function Prototypes */

void check_col(Player*, Player*);
void save_key_status(void);
void LoadKeyStatus(void);
void battle_end(void);
void check_battle_status(void);
void check_goalin_time(void);
void CheckFinish(void);
void finish_vs_zoom(void);
void finish_zoom(void);
void call_next_sequence(s32);
void start_race_BGM(s32);
void start_racing(void);
f32 get_laptime(s32);
void goalin_subroutin(s32);
void check_kart_rap(void);
void func_8028F3E8(void);
void check_kart_rank(void);
void enemy_control(void);
void race_reset_check(void);
void set_vp_scale(void);
void zoom_out_screen(void);
void zoom_in_screen(void);
void check_pause(void);
void start_fadeout(void);
void check_title_demo(void);
void race_control(void);
void goto_title(void);
void goto_gameselect(void);
void GoToGameSelect(void);
void goto_mapselect(void);
void goto_racing(void);
void collision_kart_to_kart(void);
void pause_sequence(void);

extern f32 rate_count_F[];
extern s32 gGPCurrentRaceRankByPlayerId[]; // D_801643B8 (position for each player)

#endif
