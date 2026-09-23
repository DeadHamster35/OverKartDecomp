#ifndef RENDER_COURSES_H
#define RENDER_COURSES_H

#include "code_800029B0.h"

void DisplayMap2(Screen*, s32);
s32 CheckUraPolygon(Camera*);
void SearchListFile(uintptr_t);
void DisplayGroupmap(uintptr_t, Screen*);
void DisplayMush(void);
void DisplayCoursePipe(void);
void DisplayKT1(Screen*);
void DisplayKT2(Screen*);
void DisplayKT3(Screen*);
void DisplayKT4(Screen*);
void DisplayKT5(Screen*);
void DisplayKT6(Screen*);
void DisplayKT7(Screen*);
void DisplayKT8(Screen*);
void DisplayKT9(Screen*);
void DisplayKT11(Screen*);
void DisplayKT12(Screen*);
void DisplayKT13(Screen*);
void DisplayKT14(Screen*);
void DisplayKT15(Screen*);
void DisplayKT16(Screen*);
void DisplayKT17(Screen*);
void DisplayKT18(Screen*);
void DisplayKT19(Screen*);
void DisplayKT20(Screen*);
void DisplayAllMap(void);
void DisplayMap(Screen*);
void InitialBumpKart(s32);
void MakeCollision(void);
void func_80295D50(s16, s16);
void ResetLimmity(void);
void InitialMap(void);
void MapStrategy(void);
void InitialMap2(void);

extern s32 WallFlag;

extern s32 Imageult2;
extern s32 Imageuls3;
extern s32 Imageult3;
extern s32 Imageult;

extern Lights1 D_800DC610[];

extern Lights1 D_800DC610[]; // Light data?

extern u16 CollisionPointer;

#endif
