#ifndef collision_H
#define collision_H

#include <common_structs.h>

#define COLLISION 0x1
#define NO_COLLISION 0x0

void KillDisplayList(uintptr_t);
void InitialBump(Collision*);
f32 CheckWaterLevel(Player*);
s32 CheckTriangleZX(Collision*, f32, f32, f32, f32, u16);
s32 CheckTriangleXY(Collision*, f32, f32, f32, f32, u16);
s32 CheckTriangleYZ(Collision*, f32, f32, f32, f32, u16);
s8 CheckStatus(u16);
s16 CheckArea(u16);
s16 CheckShadow(u16);
s16 CheckSlope(u16);
s16 OoBCheck(u16);
f32 CalcHeight(f32, f32, f32, u16);
f32 GetHight(Collision*, Vec3f);
void CalcBumpVelocity(Collision*, Vec3f);
void BumpRoutin(Vec3f, f32, Vec3f, f32);
u16 CheckTire2(Player*, KartTire*, f32, f32, f32);
void BumpVelocity(Vec3f, f32, Vec3f, f32);
s32 CheckTire(KartTire*);
u16 CheckBump2(Collision*, f32, f32, f32, f32, f32, f32, f32);
u16 CheckBump(Collision*, f32, f32, f32, f32);
f32 CheckHight(f32, f32, f32);
void ReadVertex2(uintptr_t, u32, u32);
s32 CheckSquareLine(s16, s16, s16, s16, s16, s16, s16, s16);
s32 CheckSquareTriangle(s16, s16, s16, s16, u16);
void MakeCollisionTable(void);
void SearchList(Gfx*);
void SearchList2(Gfx*, s8);
void SearchList3(Gfx*, s8, u16);
void ScrollMapImage(uintptr_t, s32, s32);
void WaterVertex(uintptr_t, u32, s32, s8, u8, u8, u8);
void MakeWaterVertex(uintptr_t, s8, u8, u8, u8);
void CollectTirePosition(Vec3f, f32, Vec3f);

#endif
