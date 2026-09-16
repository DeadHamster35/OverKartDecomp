#ifndef CODE_8006E9C0_H
#define CODE_8006E9C0_H

#include <common_structs.h>
#include "update_objects.h"

void initialize_params(void);
void kwclear_ram(void);
void kwfreeallocptr(void);
void kwfreelist(void);
u8* kwdma_melt(u8*, u8*, u32, u32);
void kwmelt_titlelogo(void);
void kwinit_itembox(s32);
void kwmelt_roadmap(s32);
void kwmelt_kawanoroadmap(void);
void kwmelt_common(void);
void kwbackup_gamestatus(s32);
void kwscreen_init(void);
void initialize_params_common(void);
void initialize_params_common_after(void);
void kwalloc_objram(void);
void kwinit_kumo(s32, s32, CloudData*);
void kwset_kumoram(CloudData*);
void kwinit_star(s32, s32, StarData*);
void kwset_starram(StarData*);
void KWKumo_Alloc(void);
void kwinit_fuusen(void);
void kwalloc_effectram_1p(void);
void initialize_params_1p(void);
void initialize_params_2plr(void);
void initialize_params_4p(void);
void initialize_params_2pud(void);

extern s16 D_800E5520[];
extern s16 D_800E5548[];
extern u8* gCourseOutlineTextures[0x14]; // 800e54d0

#endif
