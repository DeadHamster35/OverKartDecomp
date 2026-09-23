#ifndef CODE_8003DC40_H
#define CODE_8003DC40_H

#include <common_structs.h>

/* Function Prototypes */

void NormGravity(Player*);
void AddXygravity(Player*, Vec3f);
void AddYzgravity(Player*, Vec3f);
void Map0(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void Map1(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void Map2(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void Map8(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void Map14(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void Map18(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void CheckMapBGZX(Player*, Vec3f, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void CheckMapBGXY(Player*, Vec3f, Vec3f, f32*, f32*, f32*, f32*);
void CheckMapBGYZ(Player*, Vec3f, Vec3f, f32*, f32*, f32*, f32*);

#endif
