#ifndef ACTORS_EXTENDED_H
#define ACTORS_EXTENDED_H

#include <common_structs.h>
#include "actor_types.h"

/** @cond */

void CopyCollisionBump(Collision*, Collision*);
void DeleteRollingShell(struct ShellActor*, s32);
void CrushBanana(struct BananaActor*);
void ClearBackward(s16);
void ClearForward(struct BananaActor*, s16);
void CleanUpBanana(struct BananaActor*);
void ReleaseBanana(struct BananaBunchParent*);
void ThrowBanana(s16, struct BananaBunchParent*, Player*);
s32 CheckSuperBananaNumber(s16);
void SuperBananaStrategy(struct BananaBunchParent*);
bool CheckSuperShellNumber(s16);
void SuperShellStrategy(TripleShellParent*, s16);
s32 SetSuperBanana(Player*);
s32 SetSuperShell(Player*, s16);
s32 SetRollingShell(TripleShellParent*, Player*, s16, u16);
s32 SetGshell(Player*);
s32 SetRshell(Player*);
void SetTshell(Player*);
void MoveBanana(struct BananaActor*);
void PutSBanana(struct BananaBunchParent*, Player*, s16);
s32 PutFakeItembox(Player*);
s32 PutBanana(Player*);
void UseThunder(Player*);
void ExecuteItem(Player*);
void ItemCheck(void);
void MoveGshell(struct ShellActor*);
void SearchPoint(struct ShellActor*);
void SearchEnemy(struct ShellActor*, Player*);
s16 SelectBattleEnemy(Player*, struct ShellActor*);
void CheckKillShell(struct ShellActor*);
void MoveRshell(struct ShellActor*);
void BumpObject(Object*);

// audio/external.c
extern void NaPlyThunderStart(u8);

extern f32 D_802B9F68;

extern s16 BalloonCount[];

/** @endcond */

#endif // ACTORS_EXTENDED_H
