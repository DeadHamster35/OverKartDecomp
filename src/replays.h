#ifndef STAFF_GHOSTS_H
#define STAFF_GHOSTS_H

#include <PR/os.h>
#include <common_structs.h>

void NormDataControl(void);
void DMAROMGhost(void);
void ReplayGhost(void);
void ReadGhost(void);
void CheckRomghost(void);
s32 MakePressGhost(void);
void MakeMeltGhost(void);
void SwapGhostbuffer(void);
void InitialGhost(void);
void ReplayreadKeyData(void);
void RomreadKeyData(void);
void ReadKeyData(void);
void func_8000599C(void);
void KillGhost(Player*);
void ReplayDataControl(void);
void KeyDataControl(void);

// mi0decode

extern s32 encpass2(s32 Input, s32, s32);
extern s32 encpass1(void*, s32, s32);

extern s32 ReadPointer;
extern s32 WritePointer;
extern u16 GhostStatus;
extern u16 RomghostStatus;
extern u16 ReplayghostStatus;
extern s32 D_80162E00;
extern s32 GhostKart;
extern s32 RomghostKart;
extern s32 ReplayghostKart;
extern s32 GhostWrite;
extern s32 LostGhost;

#endif /* STAFF_GHOSTS_H */
