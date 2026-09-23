#ifndef CODE_8006E9C0_H
#define CODE_8006E9C0_H

#include <common_structs.h>
#include "update_objects.h"

void InitializeParams(void);
void KwclearRAM(void);
void Kwfreeallocptr(void);
void Kwfreelist(void);
u8* KwdmaMelt(u8*, u8*, u32, u32);
void KwmeltTitlelogo(void);
void KwinitItembox(s32);
void KwmeltRoadmap(s32);
void KwmeltKawanoroadmap(void);
void KwmeltCommon(void);
void KwbackupGamestatus(s32);
void KwscreenInit(void);
void InitializeParamsCommon(void);
void InitializeParamsCommonAfter(void);
void KwallocObjram(void);
void KwinitKumo(s32, s32, CloudData*);
void KwsetKumoram(CloudData*);
void KwinitStar(s32, s32, StarData*);
void KwsetStarram(StarData*);
void KWKumoAlloc(void);
void KwinitFuusen(void);
void KwallocEffectram1P(void);
void InitializeParams1P(void);
void InitializeParams2Plr(void);
void InitializeParams4P(void);
void InitializeParams2Pud(void);

extern s16 D_800E5520[];
extern s16 D_800E5548[];
extern u8* CourseOutlineTextures[0x14]; // 800e54d0

#endif
