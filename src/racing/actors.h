#ifndef ACTORS_H
#define ACTORS_H

#include "actor_types.h"
#include "code_800029B0.h"
#include "camera.h"

#define END_OF_SPAWN_DATA -0x8000

/** @cond */

void KillKame(struct ShellActor*);
void SetObjectBuffer(Object*, Vec3f, Vec3s, Vec3f, s16);
void OutOfScreen(Camera*, Object*);
void OnScreen(Camera*, Object*);
void DisplayFlagGate(Camera*);
void ResetAngle(Vec3s);
void CalcFaceAngle(Collision*, Vec3s);
void CalcShadowPosition(Object*, Vec3f);
void SetShadowFlag(Player*);
void ResetShadowFlag(Player*);
void InitialRedShellColor(void);
void DisplayShadow(Vec3f, Vec3s, f32);
void DisplayMoveShadow(Object*, f32);
void DisplayCow(Camera*, Mat4, Object*);
void EggStrategy(struct YoshiValleyEgg*);
void TreeStrategy(Object*);
void NutsStrategy(struct KiwanoFruit*);
void ShipStrategy(struct PaddleWheelBoat*);
void Sl1Strategy(struct TrainCar*);
void Sl2Strategy(struct TrainCar*);
void Sl3Strategy(struct TrainCar*);
void PakkunStrategy(PakkunObject*);
void DisplayPakkun(Camera*, Mat4, PakkunObject*);
void DisplayFarmCow(Camera*, Mat4, Object*);
void CheckColJungleTree(Player*);
void CheckJungleTreeCollision(void);
void ResetJungleTree(void);
void DisplayJungleTree(Camera*, Mat4, Object*);
void DisplayCourseTree1(Camera*, Mat4, Object*);
void DisplayCourseTree2(Camera*, Mat4, Object*);
void DisplayTree3(Camera*, Mat4, Object*);
void DisplayTree4(Camera*, Mat4, Object*);
void DisplayTree5(Camera*, Mat4, Object*);
void DisplayTree7(Camera*, Mat4, Object*);
void DisplayTree8(Camera*, Mat4, Object*);
void DisplaySnowtree(Camera*, Mat4, Object*);
void DisplaySaboten1(Camera*, Mat4, Object*);
void DisplaySaboten2(Camera*, Mat4, Object*);
void DisplaySaboten3(Camera*, Mat4, Object*);
void DisplayNuts(Camera*, Mat4, Object*);
void DisplayShell(Camera*, Mat4, struct ShellActor*);
void DisplayGshell(Camera*, Mat4, struct ShellActor*);
void DisplayRshell(Camera*, Mat4, struct ShellActor*);
void DisplayTshell(Camera*, Mat4, struct ShellActor*);
void DisplayBanana(Camera*, Mat4, struct BananaActor*);
void WkanbanStrategy(Object*);
void FumikiriStrategy(struct RailroadCrossing*);
void MkanbanStrategy(Object*);
void DisplayPylon(Camera*, Mat4, Object*);
void DisplayShip(Camera*, struct PaddleWheelBoat*, Mat4, u16);
void DisplayTruck1(Camera*, Object*);
void DisplayBus(Camera*, Object*);
void DisplayRv(Camera*, Object*);
void DisplayTanklorry(Camera*, Object*);
void DisplaySl1(Camera*, struct TrainCar*);
void DisplaySl2(Camera*, struct TrainCar*);
void DisplaySl3(Camera*, struct TrainCar*);
void DisplayIwa(Camera*, struct FallingRock*);
extern Gfx* FallingRockGFX;
extern Gfx* FallingRockShadowGFX;
void SetPakkunObject(struct ActorSpawnData*);
void SetPalmtreeObject(struct ActorSpawnData*);
void ResetIwa(struct ActorSpawnData*, struct FallingRock*);
void SetIwa(struct ActorSpawnData*);
void UpdateActorFallingRocks(struct FallingRock*);
void SetTreeObject(struct ActorSpawnData*);
void SetItemBoxObject(struct ActorSpawnData*);
void SetJungleNuts(void);
void InitialObjectBuffer(void);
void InitialMapObject(void);
void InitialObject(void);
void CheckObjectSe(Object*);
void DeleteObjectBuffer(Object*);
s16 SwapObject(Vec3f, Vec3s, Vec3f, s16);
s16 AddObjectBuffer(Vec3f, Vec3s, Vec3f, s16);
s16 PutObject(Vec3f, s16);
bool CollisionCylinder(Player*, Vec3f, f32, f32, f32);
bool CollisionMkanban(Player*, Object*);
bool CollisionPakkun(Player*, PakkunObject*);
bool CollisionEgg(Player*, struct YoshiValleyEgg*);
bool CollisionTree(Player*, Object*);
bool CollisionSphere(Player*, Object*);
bool CollisionSphere2(Object*, Object*);
void KillObject(Object*);
void CheckCrushSound(Object*, Object*);
void CollisionCheckRoutin2(Object*, Object*);
void CollisionCheckRoutin(Player*, Object*);
void CollisionObjectToKart(void);
void CollisionObjectToObject(void);
void ReleaseFakeItembox(struct FakeItemBox*);
void MoveFakeItembox(struct FakeItemBox*);
void InitialSpecialItembox(f32, f32, f32);
void MoveSpecialItembox(struct ItemBox*);
void MoveItembox(struct ItemBox*);
void DisplayFakeItembox(Camera*, struct FakeItemBox*);
void DisplayItembox(Camera*, struct ItemBox*);
void DisplayWkanban(Camera*, Object*);
void DisplayEgg(Camera*, Mat4, struct YoshiValleyEgg*, u16);
void DisplayMkanban(Camera*, Mat4, Object*);
void DisplayFumikiri(Camera*, struct RailroadCrossing*);
void DisplayPalmtree(Camera*, Mat4, struct PalmTree*);
void DisplayObject2(Screen*);
void DisplayCourseObject(Screen*);
void ObjectStrategy(void);

// audio/external.c
extern void NAEnmTrgStart(Vec3f, Vec3f, u32);
extern void NAEnmTrgStop(Vec3f, s32);

extern u8* D_802BA050;
extern u8* D_802BA054;
extern u8* D_802BA058;
extern Object* ActorHotAirBalloonItemBox;
extern s8 TLUTRedShell[]; // tlut 256
extern u16 D_802BA260;     // Box Truck sub-Type?

// end of definition of actor.c variables

extern u16 FumikiriMode[];
extern Lights1 D_800DC610[];

extern Gfx D_0D005338[];
extern Gfx D_0D005368[];
extern Gfx D_0D007B20[];

extern Gfx toads_turnpike_dl_0[];
extern Gfx toads_turnpike_dl_1[];
extern Gfx toads_turnpike_dl_2[];
extern Gfx toads_turnpike_dl_3[];
extern Gfx toads_turnpike_dl_4[];
extern Gfx toads_turnpike_dl_5[];
extern Gfx toads_turnpike_dl_6[];
extern Gfx toads_turnpike_dl_7[];
extern Gfx toads_turnpike_dl_8[];
extern Gfx toads_turnpike_dl_9[];
extern Gfx toads_turnpike_dl_10[];
extern Gfx toads_turnpike_dl_11[];

extern s32 LostGhost;

extern Gfx D_0D001750[];
extern Gfx D_0D001780[];
extern Gfx D_0D001798[];
extern Gfx D_0D0017B0[];
extern Gfx D_0D0017C8[];
extern Gfx D_0D0017E0[];
extern Gfx D_0D0017F8[];
extern Gfx D_0D001810[];
extern Gfx D_0D001828[];
extern Gfx D_0D001B90[];
extern Gfx D_0D001BD8[];
extern Gfx D_0D001C20[];
extern Gfx D_0D001C88[];
extern Gfx GFXItemBoxShadow[];
extern Gfx CommonModelFakeItembox[];
extern Gfx ItemBoxQuestionMarkModel[];
extern Gfx D_0D003090[];
extern Gfx D_0D0030F8[];
extern Gfx D_0D003128[];
extern Gfx D_0D003158[];
extern Gfx D_0D003188[];
extern Gfx D_0D0031B8[];
extern Gfx D_0D0031E8[];
extern Gfx CommonModelBanana[];
extern Gfx CommonModelFlatBanana[];

extern s8 D_800DC628[];
extern s8 D_800DC630[];
extern s8 D_802B8864[];

/** @endcond */

#endif // ACTORS_H
