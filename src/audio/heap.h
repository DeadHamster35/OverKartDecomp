#ifndef AUDIO_HEAP_H
#define AUDIO_HEAP_H

#include <PR/ultratypes.h>

#include "internal.h"

#define SOUND_LOAD_STATUS_NOT_LOADED 0
#define SOUND_LOAD_STATUS_IN_PROGRESS 1
#define SOUND_LOAD_STATUS_COMPLETE 2
#define SOUND_LOAD_STATUS_DISCARDABLE 3
#define SOUND_LOAD_STATUS_4 4
#define SOUND_LOAD_STATUS_5 5

#define IS_BANK_LOAD_COMPLETE(BankId) (BankLoadStatus[BankId] >= SOUND_LOAD_STATUS_COMPLETE)
#define IS_SEQ_LOAD_COMPLETE(SEQId) (SeqLoadStatus[SEQId] >= SOUND_LOAD_STATUS_COMPLETE)

struct SoundAllocPool {
    u8* Start;
    u8* cur;
    u32 Size;
    s32 NumAllocatedEntries;
}; // Size = 0x10

struct SeqOrBankEntry {
    u8* ptr;
    u32 Size;
    s16 PoolIndex;
    s16 id;
}; // Size = 0xC

struct PersistentPool {
    /*0x00*/ u32 NumEntries;
    /*0x04*/ struct SoundAllocPool Pool;
    /*0x14*/ struct SeqOrBankEntry Entries[32];
}; // Size = 0x194

struct TemporaryPool {
    /*EU,   SH*/
    /*0x00, 0x00*/ u32 NextSide;
    /*0x04,     */ struct SoundAllocPool Pool;
    /*0x04,        Pool.Start     */
    /*0x08,        Pool.cur       */
    /*0x0C, 0x0C   Pool.Size      */
    /*0x10, 0x10   Pool.NumAllocatedEntries */
    /*0x14,     */ struct SeqOrBankEntry Entries[2];
    /*0x14, 0x14   Entries[0].ptr */
    /*0x18,        Entries[0].Size*/
    /*0x1C, 0x1E   Entries[0].id  */
    /*0x20, 0x20   Entries[1].ptr */
    /*0x24,        Entries[1].Size*/
    /*0x28, 0x2A   Entries[1].id  */
}; // Size = 0x2C

struct SoundMultiPool {
    /*0x000*/ struct PersistentPool Persistent;
    /*0x194*/ struct TemporaryPool Temporary;
    /*     */ u32 Pad2[4];
}; // Size = 0x1D0

struct Unk1Pool {
    struct SoundAllocPool Pool;
    struct SeqOrBankEntry Entries[32];
};

struct UnkEntry {
    s8 Used;
    s8 Medium;
    s8 BankId;
    u32 pad;
    u8* SrcAddr;
    u8* DstAddr;
    u32 Size;
};

struct UnkPool {
    /*0x00*/ struct SoundAllocPool Pool;
    /*0x10*/ struct UnkEntry Entries[64];
    /*0x510*/ s32 NumEntries;
    /*0x514*/ u32 Unk514;
};

struct PoolSplit {
    u32 WantSeq;
    u32 WantBank;
    u32 WantUnused;
    u32 WantCustom;
}; // Size = 0x10

struct PoolSplit2 {
    u32 WantPersistent;
    u32 WantTemporary;
}; // Size = 0x8

void NasResetIDtable(void);
void NasForceStopChannel(s32 BankId);
void NasForceStopSeq(s32 SEQId);
void* NasHeapAlloc(struct SoundAllocPool* Pool, u32 Size);
void NasHeapInit(struct SoundAllocPool* Pool, void* memAddr, u32 Size);
void NasSzStayClear(struct PersistentPool* Persistent);
void NasSzAutoClear(struct TemporaryPool* Temporary);
void func_800B90E0(struct SoundAllocPool* Pool);
void NasSzCustomClear(s32);
void NasSzHeapReset(struct PoolSplit*);
void NasSzHeapDivide(struct PoolSplit2* a);
void NasSzDataDivide(struct PoolSplit* a);
void NasSzStayDivide(struct PoolSplit* a);
void* NasSzAutoDivide(struct SoundMultiPool*, s32, s32, s32, s32);
void* NasSzHeapAlloc(s32 poolIdx, s32 arg1, s32 id);
void* NasSzCacheCheck(s32 poolIdx, s32 arg1, s32 BankId);
void NasSzCacheCheckInner(f32, f32, u16*);
void NasInitFilterCoef(void);
s32 NasDelayDown(void);
void NasSpecChange(void);
void* NasMemoryReconfig(s32 poolIdx, s32 id);
void EmemOnCheck(s32, s32);

extern s32 D_800EA5D0;

extern s32 MaxAudioCmds;
extern s16 TempoInternalToExternal;
extern f32 D_803B7178;
extern s32 RefreshRate;
extern u32 SampleDmaNumListItems;
extern struct AudioSessionSettingsEU AudioSessionPresets[];

// Note: In some .asm files D_803AFBC8 has been replaced with gLeftVolRampings
// That is almost certainly incorrect, but I don't know how to fix it at this Point
extern s16 Volume; // D_803AFBC0
extern s8 UseReverb;
extern s8 NumSynthesisReverbs; // D_803AFBC3
extern struct NoteSubEu* NoteSubsEu;
extern struct SoundAllocPool AudioSessionPool; // D_803AFBC8
extern struct SoundAllocPool AudioInitPool;    // D_803AFBD8
extern struct SoundAllocPool NotesAndBuffersPool;
extern struct SoundAllocPool PersistentCommonPool; // D_803AFC28
extern struct SoundAllocPool TemporaryCommonPool;  // D_803AFC38
extern struct SoundMultiPool SeqLoadedPool;        // D_803AFC48
extern struct SoundMultiPool BankLoadedPool;       // D_803AFE18
extern struct SoundMultiPool UnusedLoadedPool;
extern struct Unk1Pool UnkPool1;                   // D_803B01B8
extern struct PoolSplit SSessionPoolSplit;          // D_803B0348
extern struct PoolSplit2 SSeqAndBankPoolSplit;      // D_803B0358
extern struct PoolSplit SPersistentCommonPoolSplit; // D_803B0360;
extern struct PoolSplit STemporaryCommonPoolSplit;  // D_803B0370;
extern struct SoundMultiPool UnusedLoadedPool;     // D_803AFFE8
extern struct SoundAllocPool SeqAndBankPool;       // D_803AFC18
extern u8 BankLoadStatus[64];                      // D_803B03C0
extern u8 UnkLoadStatus[64];
extern u8 SeqLoadStatus[256];        // D_803B0400
extern volatile u8 AudioResetStatus; // D_803B0500
extern u8 AudioResetPresetIdToLoad;
extern s32 AudioResetFadeOutFramesLeft; // D_803B0504
extern struct Note* Notes;

#endif // AUDIO_HEAP_H
