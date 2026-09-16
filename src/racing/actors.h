#ifndef ACTORS_H
#define ACTORS_H

#include "actor_types.h"
#include "code_800029B0.h"
#include "camera.h"

#define END_OF_SPAWN_DATA -0x8000

/** @cond */

void kill_kame(struct ShellActor*);
void set_object_buffer(Object*, Vec3f, Vec3s, Vec3f, s16);
void out_of_screen(Camera*, Object*);
void on_screen(Camera*, Object*);
void DisplayFlagGate(Camera*);
void reset_angle(Vec3s);
void calc_face_angle(Collision*, Vec3s);
void calc_shadow_position(Object*, Vec3f);
void set_shadow_flag(Player*);
void reset_shadow_flag(Player*);
void initial_red_shell_color(void);
void DisplayShadow(Vec3f, Vec3s, f32);
void display_move_shadow(Object*, f32);
void display_cow(Camera*, Mat4, Object*);
void egg_strategy(struct YoshiValleyEgg*);
void tree_strategy(Object*);
void nuts_strategy(struct KiwanoFruit*);
void ship_strategy(struct PaddleWheelBoat*);
void sl1_strategy(struct TrainCar*);
void sl2_strategy(struct TrainCar*);
void sl3_strategy(struct TrainCar*);
void pakkun_strategy(PakkunObject*);
void display_pakkun(Camera*, Mat4, PakkunObject*);
void display_farm_cow(Camera*, Mat4, Object*);
void check_col_jungle_tree(Player*);
void check_jungle_tree_collision(void);
void reset_jungle_tree(void);
void display_jungle_tree(Camera*, Mat4, Object*);
void display_tree1(Camera*, Mat4, Object*);
void display_tree2(Camera*, Mat4, Object*);
void display_tree3(Camera*, Mat4, Object*);
void display_tree4(Camera*, Mat4, Object*);
void display_tree5(Camera*, Mat4, Object*);
void display_tree7(Camera*, Mat4, Object*);
void display_tree8(Camera*, Mat4, Object*);
void display_snowtree(Camera*, Mat4, Object*);
void display_saboten1(Camera*, Mat4, Object*);
void display_saboten2(Camera*, Mat4, Object*);
void display_saboten3(Camera*, Mat4, Object*);
void display_nuts(Camera*, Mat4, Object*);
void display_shell(Camera*, Mat4, struct ShellActor*);
void display_gshell(Camera*, Mat4, struct ShellActor*);
void display_rshell(Camera*, Mat4, struct ShellActor*);
void display_tshell(Camera*, Mat4, struct ShellActor*);
void display_banana(Camera*, Mat4, struct BananaActor*);
void wkanban_strategy(Object*);
void fumikiri_strategy(struct RailroadCrossing*);
void mkanban_strategy(Object*);
void display_pylon(Camera*, Mat4, Object*);
void display_ship(Camera*, struct PaddleWheelBoat*, Mat4, u16);
void display_truck1(Camera*, Object*);
void display_bus(Camera*, Object*);
void display_rv(Camera*, Object*);
void display_tanklorry(Camera*, Object*);
void display_sl1(Camera*, struct TrainCar*);
void display_sl2(Camera*, struct TrainCar*);
void display_sl3(Camera*, struct TrainCar*);
void display_iwa(Camera*, struct FallingRock*);
void SetPakkunObject(struct ActorSpawnData*);
void set_palmtree_object(struct ActorSpawnData*);
void reset_iwa(struct ActorSpawnData*, struct FallingRock*);
void set_iwa(struct ActorSpawnData*);
void update_actor_falling_rocks(struct FallingRock*);
void SetTreeObject(struct ActorSpawnData*);
void SetItemBoxObject(struct ActorSpawnData*);
void set_jungle_nuts(void);
void initial_object_buffer(void);
void InitialMapObject(void);
void initial_object(void);
void check_object_se(Object*);
void deleteObjectBuffer(Object*);
s16 swap_object(Vec3f, Vec3s, Vec3f, s16);
s16 addObjectBuffer(Vec3f, Vec3s, Vec3f, s16);
s16 PutObject(Vec3f, s16);
bool CollisionCylinder(Player*, Vec3f, f32, f32, f32);
bool collision_mkanban(Player*, Object*);
bool collision_pakkun(Player*, PakkunObject*);
bool collision_egg(Player*, struct YoshiValleyEgg*);
bool collision_tree(Player*, Object*);
bool CollisionSphere(Player*, Object*);
bool collision_sphere2(Object*, Object*);
void KillObject(Object*);
void check_crush_sound(Object*, Object*);
void collision_check_routin2(Object*, Object*);
void collision_check_routin(Player*, Object*);
void collision_object_to_kart(void);
void collision_object_to_object(void);
void release_fake_itembox(struct FakeItemBox*);
void move_fake_itembox(struct FakeItemBox*);
void initial_special_itembox(f32, f32, f32);
void move_special_itembox(struct ItemBox*);
void move_itembox(struct ItemBox*);
void display_fake_itembox(Camera*, struct FakeItemBox*);
void display_itembox(Camera*, struct ItemBox*);
void display_wkanban(Camera*, Object*);
void display_egg(Camera*, Mat4, struct YoshiValleyEgg*, u16);
void display_mkanban(Camera*, Mat4, Object*);
void display_fumikiri(Camera*, struct RailroadCrossing*);
void display_palmtree(Camera*, Mat4, struct PalmTree*);
void display_object2(Screen*);
void display_object(Screen*);
void object_strategy(void);

// audio/external.c
extern void NAEnmTrgStart(Vec3f, Vec3f, u32);
extern void NAEnmTrgStop(Vec3f, s32);

extern u8* D_802BA050;
extern u8* D_802BA054;
extern u8* D_802BA058;
extern Object* gActorHotAirBalloonItemBox;
extern s8 gTLUTRedShell[]; // tlut 256
extern u16 D_802BA260;     // Box Truck sub-type?

// end of definition of actor.c variables

extern u16 fumikiri_mode[];
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

extern s32 lost_ghost;

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
extern Gfx D_0D002EE8[];
extern Gfx common_model_fake_itembox[];
extern Gfx itemBoxQuestionMarkModel[];
extern Gfx D_0D003090[];
extern Gfx D_0D0030F8[];
extern Gfx D_0D003128[];
extern Gfx D_0D003158[];
extern Gfx D_0D003188[];
extern Gfx D_0D0031B8[];
extern Gfx D_0D0031E8[];
extern Gfx common_model_banana[];
extern Gfx common_model_flat_banana[];

extern s8 D_800DC628[];
extern s8 D_800DC630[];
extern s8 D_802B8864[];

/** @endcond */

#endif // ACTORS_H
