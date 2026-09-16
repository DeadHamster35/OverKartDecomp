#ifndef RENDER_COURSES_H
#define RENDER_COURSES_H

#include "code_800029B0.h"

void DisplayMap2(Screen*, s32);
s32 check_ura_polygon(Camera*);
void SearchListFile(uintptr_t);
void DisplayGroupmap(uintptr_t, Screen*);
void display_mush(void);
void display_pipe(void);
void DisplayKT1(Screen*);
void display_KT2(Screen*);
void display_KT3(Screen*);
void display_KT4(Screen*);
void display_KT5(Screen*);
void display_KT6(Screen*);
void display_KT7(Screen*);
void display_KT8(Screen*);
void display_KT9(Screen*);
void display_KT11(Screen*);
void display_KT12(Screen*);
void display_KT13(Screen*);
void display_KT14(Screen*);
void display_KT15(Screen*);
void DisplayKT16(Screen*);
void display_KT17(Screen*);
void display_KT18(Screen*);
void display_KT19(Screen*);
void display_KT20(Screen*);
void display_all_map(void);
void display_map(Screen*);
void initial_bump_kart(s32);
void MakeCollision(void);
void func_80295D50(s16, s16);
void reset_limmity(void);
void InitialMap(void);
void map_strategy(void);
void initial_map2(void);

extern s32 wall_flag;

extern s32 imageult2;
extern s32 imageuls3;
extern s32 imageult3;
extern s32 imageult;

extern Lights1 D_800DC610[];

extern Lights1 D_800DC610[]; // Light data?

extern u16 collision_pointer;

#endif
