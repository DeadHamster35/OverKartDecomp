#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include "internal.h"

#define AUDIO_LOCK_UNINITIALIZED 0
#define AUDIO_LOCK_NOT_LOADING 0x76557364
#define AUDIO_LOCK_LOADING 0x19710515

#define NUMAIBUFFERS 3
#ifdef VERSION_EU
#define AIBUFFER_LEN (0xaa * 16)
#else
#define AIBUFFER_LEN (0xa0 * 16)
#endif

// Number of DMAs performed during this frame
extern s32 CurrAudioFrameDmaCount;

extern s16 SSawtoothWaves[256];
extern s16 STriangleWaves[256];
extern s16 SSineWaves[256];
extern s16 SSquareWaves[256];
extern s16 SUnknownWave6[256];
extern s16 UnknownWave7[256];
extern s16* WaveSamples[6];
extern u32 Fill[2];
extern f32 PitchBendFrequencyScale[256];
extern f32 NoteFrequencies[128];
extern u8 DefaultShortNoteVelocityTable[16];
extern u8 DefaultShortNoteDurationTable[16];
extern struct AdsrEnvelope DefaultEnvelope[];
extern u32 Fill2;
extern struct NoteSubEu ZeroNoteSub;
extern struct NoteSubEu DefaultNoteSub;
extern u16 HeadsetPanQuantization[0x10];
extern s16 UnknownData800F6290[];
extern f32 HeadsetPanVolume[128];
extern f32 StereoPanVolume[128];
extern f32 DefaultPanVolume[128];
extern u32 AudioRandom;

extern s16 TatumsPerBeat;          // D_800EA5CC
extern volatile s32 AudioLoadLock; // D_800EA5DC

extern s32 AudioTaskIndex;
extern s32 CurrAiBufferIndex;
extern Acmd* AudioCmdBuffers[2];    // probably wrong
extern Acmd* AudioCmd;              // D_803B70D0
extern struct SPTask* AudioTask;    // D_803B70D4
extern struct SPTask AudioTasks[2]; // D_803B70D8
extern f32 D_803B7178;
extern s32 RefreshRate;
extern s16* AiBuffers[];      // D_803B7180
extern s16 AiBufferLengths[]; // osAiSetNextBuffer nbytes
extern u16 D_803B7192;         // unconfirmed
extern u32 AudioRandom;
extern s32 AudioErrorFlags;
extern u64 AudioGlobalsEndMarker; // unconfirmed
extern u8 AudioHeap[];            // unconfirmed

extern struct ReverbSettingsEU SReverbSettings[];
extern struct AudioSessionSettingsEU AudioSessionPresets[];
extern s8 UnusedCount800EA5C8;
extern s16 TatumsPerBeat;
extern s32 AudioHeapSize;
extern s32 AudioInitPoolSize;
extern s32 D_800EA5D8;
extern volatile s32 AudioLoadLock;

#endif
