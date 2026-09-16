#ifndef ACTORS_EXTENDED_H
#define ACTORS_EXTENDED_H

#include <common_structs.h>
#include "actor_types.h"

/** @cond */

void copy_bump(Collision*, Collision*);
void delete_rolling_shell(struct ShellActor*, s32);
void crush_banana(struct BananaActor*);
void clear_backward(s16);
void clear_forward(struct BananaActor*, s16);
void clean_up_banana(struct BananaActor*);
void release_banana(struct BananaBunchParent*);
void throw_banana(s16, struct BananaBunchParent*, Player*);
s32 check_super_banana_number(s16);
void super_banana_strategy(struct BananaBunchParent*);
bool check_super_shell_number(s16);
void super_shell_strategy(TripleShellParent*, s16);
s32 set_super_banana(Player*);
s32 set_super_shell(Player*, s16);
s32 set_rolling_shell(TripleShellParent*, Player*, s16, u16);
s32 set_gshell(Player*);
s32 set_rshell(Player*);
void set_tshell(Player*);
void move_banana(struct BananaActor*);
void put_s_banana(struct BananaBunchParent*, Player*, s16);
s32 put_fake_itembox(Player*);
s32 put_banana(Player*);
void use_thunder(Player*);
void ExecuteItem(Player*);
void item_check(void);
void move_gshell(struct ShellActor*);
void search_point(struct ShellActor*);
void search_enemy(struct ShellActor*, Player*);
s16 select_battle_enemy(Player*, struct ShellActor*);
void check_kill_shell(struct ShellActor*);
void move_rshell(struct ShellActor*);
void BumpObject(Object*);

// audio/external.c
extern void Na_PlyThunder_Start(u8);

extern f32 D_802B9F68;

extern s16 BalloonCount[];

/** @endcond */

#endif // ACTORS_EXTENDED_H
