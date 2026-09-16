#ifndef CODE_8003DC40_H
#define CODE_8003DC40_H

#include <common_structs.h>

/* Function Prototypes */

void norm_gravity(Player*);
void add_xygravity(Player*, Vec3f);
void add_yzgravity(Player*, Vec3f);
void map_0(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void map_1(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void map_2(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void map_8(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void map_14(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void map_18(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void CheckMapBG_ZX(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void CheckMapBG_XY(Player*, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void CheckMapBG_YZ(Player*, Vec3f, Vec3f, f32*, f32*, f32*, f32*);

#endif
