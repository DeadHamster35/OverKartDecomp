#ifndef SKYBOX_AND_SPLITSCREEN_H
#define SKYBOX_AND_SPLITSCREEN_H

#include <PR/gbi.h>
#include "code_800029B0.h"

#define G_CLEAR_ALL_MODES 0xFFFFFFFF

/* Function Prototypes */

void DisplayBackground(Vtx*, Screen*, s32, s32, f32*);

void SetViewport(Screen*);
void func_802A38AC(void);
void fadeout_sequence(void);
void ClearZBuffer2(Screen*);
void ClearZBuffer(void);
void InitRDP(void);
void func_802A40A4(void);
void func_802A40AC(void);
void func_802A40B4(void);
void func_802A40BC(void);
void func_802A40C4(void);
void func_802A40CC(void);
void func_802A40D4(void);
void func_802A40DC(void);
s32 initRSP_full(void);
void InitRSP(void);
void g_ClearFramebuffer(void);
void make_border(void);
void initial_background(Vtx*);
void display_background2(Vtx*, Screen*, s32, s32, f32*);
void initital_perspective(void);
void setup_perspective(void);
void initial_draw_right(void);
void initial_draw_left(void);
void initial_draw_up(void);
void initial_draw_down(void);
void InitialDrawCommon(void);
void initial_draw_up_left(void);
void initial_draw_up_right(void);
void initial_draw_down_left(void);
void initial_draw_down_right(void);
void draw_full_screen(void);
void draw_left_screen(void);
void draw_right_screen(void);
void draw_up_screen(void);
void draw_down_screen(void);
void draw_up_left_screen(void);
void draw_up_right_screen(void);
void draw_down_left_screen(void);
void draw_down_right_screen(void);
void initial_screen_data(void);
void GetFramebuffer(s32, s32, s32, s32, u16*, u16*);
void take_snapshot15(void);
void take_snapshot9(void);

extern Vp D_802B8880[];

#endif
