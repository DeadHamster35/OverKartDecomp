#ifndef AUDIO_PORT_EU_H
#define AUDIO_PORT_EU_H

#include "audio/internal.h"

#define SAMPLES_TO_OVERPRODUCE 0x10
#define EXTRA_BUFFERED_AI_SAMPLES_TARGET 0x40

void NapAudioSysProcess(struct EuAudioCmd*);
void NasGroupFadeOut(s32 arg0, s32 fadeOutTime);
void NasGroupFadeIn(s32 playerIndex, s32 fadeInTime);
void NapAudioPortInit(void);
void NapPortSet(s32, s32*);
void NapSetF32(u32, f32);
void NapSetS32(u32, u32);
void NapSetS8(u32, s8);
void NapSendStart(void);
void NapAudioPortProcess(u32 arg0);
void PortEUInit(void);

extern OSMesgQueue D_801937C0;
extern OSMesgQueue D_801937D8;
extern OSMesgQueue D_801937F0;
extern OSMesgQueue D_80193808;

extern struct EuAudioCmd SAudioCmd[0x100];

extern OSMesg D_80194020[];
extern OSMesg D_80194028[];
extern OSMesg D_80194038[];
extern OSMesg D_8019403C[];

extern OSMesgQueue* D_800EA3A8;
extern OSMesgQueue* D_800EA3AC;
extern OSMesgQueue* D_800EA3B0;
extern OSMesgQueue* D_800EA3B4;
extern s32 D_800EA484;
extern s32 D_800EA4A4;

#endif
