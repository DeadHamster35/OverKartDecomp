#ifndef AUDIO_LOAD_H
#define AUDIO_LOAD_H

#include <PR/ultratypes.h>
#include "audio/internal.h"

#define AUDIO_FRAME_DMA_QUEUE_SIZE 0x40

#define PRELOAD_BANKS 2
#define PRELOAD_SEQUENCE 1

#define IS_SEQUENCE_CHANNEL_VALID(ptr) ((uintptr_t) (ptr) != (uintptr_t) &gSequenceChannelNone)

struct SharedDma {
    /*0x0*/ u8* buffer;       // target, points to pre-allocated buffer
    /*0x4*/ uintptr_t source; // device address
    /*0x8*/ u16 sizeUnused;   // set to bufSize, never read
    /*0xA*/ u16 bufSize;      // size of buffer
    /*0xC*/ u8 unused2;       // set to 0, never read
    /*0xD*/ u8 reuseIndex;    // position in sSampleDmaReuseQueue1/2, if ttl == 0
    /*0xE*/ u8 ttl;           // duration after which the DMA can be discarded
}; // size = 0x10

void EmemLoad(u8* devAddr, void* vAddr, size_t nbytes);
void Nas_FastRomCopy(uintptr_t, void*, size_t, OSMesgQueue*, OSIoMesg*);
void Nas_SlowRomCopy(uintptr_t*, u8**, ssize_t*, OSMesgQueue*, OSIoMesg*);
void Nas_SlowRomCopy4096(void);
void* Nas_WaveDmaFrameWork(uintptr_t, u32, s32, u8*);
void Nas_WaveDmaCallBack(s32);
s32 Nas_WaveDmaNew(struct AudioBankSample*);
s32 Nas_LoadVoice(s32 bankId, s32 instId, s32 arg2);
void Nas_BankHeaderInit(ALSeqFile*, u8*);
void Nas_smzWaveTouch(struct AudioBankSound* sound, u8* memBase, u8* offsetBase);
void Nas_smzBnkOfsToAdd(s32);
void Nas_smzBnkOfsToAdd_Inner(struct AudioBank* mem, u8* offset, u32 numInstruments, u32 numDrums);
struct AudioBank* Nas_MyBankLoad(s32, s32);
struct AudioBank* Nas_MyBankSlowLoad(s32, s32, struct SequencePlayer*);
void* Nas_MySeqLoad(s32, s32);
void* Nas_MySeqLoadSlow(s32, s32, struct SequencePlayer*);
u8 Nas_CountBankInSeq(u32 seqId, s32* nonNullCount, s32* nullCount);
struct AudioBank* Nas_PreLoadBank(s32, u8*);
void Nas_PreLoadSeq(u32, u8);
void Nas_StartMySeq(u32, u32, s32);
void Nas_StartMySeq_Inner(u32, u32, s32);

extern struct SequencePlayer SeqPlayer[SEQUENCE_PLAYERS];
extern struct SequenceChannel gSequenceChannels[SEQUENCE_CHANNELS];
extern struct SequenceChannelLayer gSequenceLayers[SEQUENCE_LAYERS];
extern struct SequenceChannel gSequenceChannelNone;
extern struct AudioListItem gLayerFreeList;
extern struct NotePool gNoteFreeLists;
extern OSMesgQueue gCurrAudioFrameDmaQueue;
extern OSMesg gCurrAudioFrameDmaMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
extern OSIoMesg gCurrAudioFrameDmaIoMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
extern OSMesgQueue D_803B6720;
extern OSMesg D_803B6738;

extern OSIoMesg D_803B6740;
extern struct SharedDma sSampleDmas[0x70];
extern u32 gSampleDmaNumListItems;
extern u32 sSampleDmaListSize1;
extern s32 D_803B6E60;
extern s32 load_bss_pad;

extern u8 sSampleDmaReuseQueue1[256];
extern u8 sSampleDmaReuseQueue2[256];
extern u8 sSampleDmaReuseQueueTail1;
extern u8 sSampleDmaReuseQueueTail2;
extern u8 sSampleDmaReuseQueueHead1;
extern u8 sSampleDmaReuseQueueHead2;

extern ALSeqFile* g_MUSTablePointer;
extern ALSeqFile* gAlCtlHeader;
extern ALSeqFile* gAlTbl;
extern u8* gAlBankSets;
extern u16 gSequenceCount;
extern struct CtlEntry* g_sfxPointer;
extern struct AudioBufferParametersEU gAudioBufferParameters;
extern u32 D_803B70A8;
extern s32 gMaxAudioCmds;
extern s32 gMaxSimultaneousNotes;
extern s16 gTempoInternalToExternal;
extern s8 gAudioLibSoundMode;
extern volatile s32 gAudioFrameCount;
extern s32 gCurrAudioFrameDmaCount;

#endif // AUDIO_LOAD_H
