#ifndef AUDIO_EFFECTS_H
#define AUDIO_EFFECTS_H

#include "audio/internal.h"

#ifdef TARGET_N64
#define IS_BIG_ENDIAN 1
#endif

#define ADSR_STATE_DISABLED 0
#define ADSR_STATE_INITIAL 1
#define ADSR_STATE_START_LOOP 2
#define ADSR_STATE_LOOP 3
#define ADSR_STATE_FADE 4
#define ADSR_STATE_HANG 5
#define ADSR_STATE_DECAY 6
#define ADSR_STATE_RELEASE 7
#define ADSR_STATE_SUSTAIN 8

#define ADSR_ACTION_RELEASE 0x10
#define ADSR_ACTION_DECAY 0x20
#define ADSR_ACTION_HANG 0x40

#define ADSR_DISABLE 0
#define ADSR_HANG -1
#define ADSR_GOTO -2
#define ADSR_RESTART -3

// Envelopes are always stored as big endian, to match sequence files which are
// byte blobs and can embed envelopes. Hence this byteswapping macro.
#if IS_BIG_ENDIAN
#define BSWAP16(x) (x)
#else
#define BSWAP16(x) (((x) & 0xff) << 8 | (((x) >> 8) & 0xff))
#endif

void Nas_ChLookUse(struct SequenceChannel* seqChannel, s32 recalculateVolume);
void Nas_AllocationOnRequest(struct SequencePlayer* seqPlayer);
f32 Nas_ChannelInit(struct Portamento* p);
s16 Nas_CallWaveProcess_Sub(struct VibratoState* vib);
f32 Nas_MainCtrl(struct VibratoState* vib);
void Nas_SweepCalculator(struct Note* note);
void Nas_ModTableRead(struct Note* note);
void Nas_Modulator(struct AdsrState* adsr, struct AdsrEnvelope* envelope, s16* volOut);
f32 Nas_ChannelModulation(struct AdsrState* adsr);

#endif /* AUDIO_EFFECTS_H */
