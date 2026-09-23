#ifndef AUDIO_LOAD_H
#define AUDIO_LOAD_H

#include <PR/ultratypes.h>
#include "audio/internal.h"

#define AUDIO_FRAME_DMA_QUEUE_SIZE 0x40

#define PRELOAD_BANKS 2
#define PRELOAD_SEQUENCE 1

#define IS_SEQUENCE_CHANNEL_VALID(ptr) ((uintptr_t) (ptr) != (uintptr_t) &SequenceChannelNone)

struct SharedDma {
    /*0x0*/ u8* Buffer;       // Target, points to pre-allocated Buffer
    /*0x4*/ uintptr_t Source; // device Address
    /*0x8*/ u16 SizeUnused;   // set to BufSize, never read
    /*0xA*/ u16 BufSize;      // Size of Buffer
    /*0xC*/ u8 Unused2;       // set to 0, never read
    /*0xD*/ u8 ReuseIndex;    // Position in SSampleDmaReuseQueue1/2, if ttl == 0
    /*0xE*/ u8 ttl;           // Duration after which the DMA can be discarded
}; // Size = 0x10

void EmemLoad(u8* devAddr, void* vAddr, size_t nbytes);
void NasFastRomCopy(uintptr_t, void*, size_t, OSMesgQueue*, OSIoMesg*);
void NasSlowRomCopy(uintptr_t*, u8**, ssize_t*, OSMesgQueue*, OSIoMesg*);
void NasSlowRomCopy4096(void);
void* NasWaveDmaFrameWork(uintptr_t, u32, s32, u8*);
void NasWaveDmaCallBack(s32);
s32 NasWaveDmaNew(struct AudioBankSample*);
s32 NasLoadVoice(s32 BankId, s32 instId, s32 Arg2);
void NasBankHeaderInit(ALSeqFile*, u8*);
void NasSmzWaveTouch(struct AudioBankSound* Sound, u8* memBase, u8* offsetBase);
void NasSmzBnkOfsToAdd(s32);
void NasSmzBnkOfsToAddInner(struct AudioBank* mem, u8* Offset, u32 NumInstruments, u32 NumDrums);
struct AudioBank* NasMyBankLoad(s32, s32);
struct AudioBank* NasMyBankSlowLoad(s32, s32, struct SequencePlayer*);
void* NasMySeqLoad(s32, s32);
void* NasMySeqLoadSlow(s32, s32, struct SequencePlayer*);
u8 NasCountBankInSeq(u32 SEQId, s32* nonNullCount, s32* nullCount);
struct AudioBank* NasPreLoadBank(s32, u8*);
void NasPreLoadSeq(u32, u8);
void NasStartMySeq(u32, u32, s32);
void NasStartMySeqInner(u32, u32, s32);

extern struct SequencePlayer SeqPlayer[SEQUENCE_PLAYERS];
extern struct SequenceChannel SequenceChannels[SEQUENCE_CHANNELS];
extern struct SequenceChannelLayer SequenceLayers[SEQUENCE_LAYERS];
extern struct SequenceChannel SequenceChannelNone;
extern struct AudioListItem LayerFreeList;
extern struct NotePool NoteFreeLists;
extern OSMesgQueue CurrAudioFrameDmaQueue;
extern OSMesg CurrAudioFrameDmaMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
extern OSIoMesg CurrAudioFrameDmaIoMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
extern OSMesgQueue D_803B6720;
extern OSMesg D_803B6738;

extern OSIoMesg D_803B6740;
extern struct SharedDma SSampleDmas[0x70];
extern u32 SampleDmaNumListItems;
extern u32 SSampleDmaListSize1;
extern s32 D_803B6E60;
extern s32 LoadBssPad;

extern u8 SSampleDmaReuseQueue1[256];
extern u8 SSampleDmaReuseQueue2[256];
extern u8 SSampleDmaReuseQueueTail1;
extern u8 SSampleDmaReuseQueueTail2;
extern u8 SSampleDmaReuseQueueHead1;
extern u8 SSampleDmaReuseQueueHead2;

extern ALSeqFile* MUSTablePointer;
extern ALSeqFile* AlCtlHeader;
extern ALSeqFile* AlTbl;
extern u8* AlBankSets;
extern u16 SequenceCount;
extern struct CtlEntry* SFXPointer;
extern struct AudioBufferParametersEU AudioBufferParameters;
extern u32 D_803B70A8;
extern s32 MaxAudioCmds;
extern s32 MaxSimultaneousNotes;
extern s16 TempoInternalToExternal;
extern s8 AudioLibSoundMode;
extern volatile s32 AudioFrameCount;
extern s32 CurrAudioFrameDmaCount;

#endif // AUDIO_LOAD_H
