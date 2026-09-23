#include <ultra64.h>
#include <macros.h>

#include "audio/load.h"
#include "audio/data.h"
#include "audio/heap.h"
#include "audio/internal.h"
#include "audio/playback.h"
#include "audio/synthesis.h"
#include "audio/seqplayer.h"
#include "audio/port_eu.h"
#include "buffers/gfx_output_buffer.h"

#define ALIGN16(val) (((val) + 0xF) & ~0xF)

struct SequencePlayer SeqPlayer[SEQUENCE_PLAYERS];
struct SequenceChannel SequenceChannels[SEQUENCE_CHANNELS];
struct SequenceChannelLayer SequenceLayers[SEQUENCE_LAYERS];
struct SequenceChannel SequenceChannelNone;
// D_803B5F5C
struct AudioListItem LayerFreeList;
struct NotePool NoteFreeLists;
OSMesgQueue CurrAudioFrameDmaQueue;
OSMesg CurrAudioFrameDmaMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
OSIoMesg CurrAudioFrameDmaIoMesgBufs[AUDIO_FRAME_DMA_QUEUE_SIZE];
OSMesgQueue D_803B6720;
OSMesg D_803B6738;

OSIoMesg D_803B6740;
struct SharedDma SSampleDmas[0x70];
u32 SampleDmaNumListItems;
u32 SSampleDmaListSize1;
s32 D_803B6E60;
s32 LoadBssPad;

u8 SSampleDmaReuseQueue1[256]; // SSampleDmaReuseQueue1
u8 SSampleDmaReuseQueue2[256]; // SSampleDmaReuseQueue2
u8 SSampleDmaReuseQueueTail1;  // SSampleDmaReuseQueueTail1
u8 SSampleDmaReuseQueueTail2;  // SSampleDmaReuseQueueTail2
u8 SSampleDmaReuseQueueHead1;  // SSampleDmaReuseQueueHead1
u8 SSampleDmaReuseQueueHead2;  // SSampleDmaReuseQueueHead2

ALSeqFile* MUSTablePointer;
ALSeqFile* AlCtlHeader;
ALSeqFile* AlTbl;
u8* AlBankSets;
u16 SequenceCount;
struct CtlEntry* SFXPointer;
struct AudioBufferParametersEU AudioBufferParameters;
u32 D_803B70A8;
s32 MaxAudioCmds;
s32 MaxSimultaneousNotes;
s16 TempoInternalToExternal;
s8 AudioLibSoundMode;
// If we take SM64 as gospel, these should be in data.c, but that doesn't match
volatile s32 AudioFrameCount;
s32 CurrAudioFrameDmaCount;

/**
 * Given that (almost) all of these are format strings, it is highly likely
 * that they are meant to be Used in some sort of printf Variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 **/
char LoadAudioString00[] = "Romcopy %x -> %x ,size %x\n";
char LoadAudioString01[] = "Romcopyend\n";
char LoadAudioString02[] = "CAUTION:WAVE CACHE FULL %d";
char LoadAudioString03[] = "LOAD  Rom :%x -> Ram :%x  Len:%x\n";
char LoadAudioString04[] = "BASE %x %x\n";
char LoadAudioString05[] = "LOAD %x %x %x\n";
char LoadAudioString06[] = "INSTTOP    %x\n";
char LoadAudioString07[] = "INSTMAP[0] %x\n";
char LoadAudioString08[] = "already flags %d\n";
char LoadAudioString09[] = "already flags %d\n";
char LoadAudioString10[] = "ERR:SLOW BANK DMA BUSY\n";
char LoadAudioString11[] = "ERR:SLOW DMA BUSY\n";
char LoadAudioString12[] = "Check %d  bank %d\n";
char LoadAudioString13[] = "Cache Check\n";
char LoadAudioString14[] = "NO BANK ERROR\n";
char LoadAudioString15[] = "BANK %d LOADING START\n";
char LoadAudioString16[] = "BANK %d LOAD MISS (NO MEMORY)!\n";
char LoadAudioString17[] = "BANK %d ALREADY CACHED\n";
char LoadAudioString18[] = "BANK LOAD MISS! FOR %d\n";
char LoadAudioString19[] = "Seq %d Loading Start\n";
char LoadAudioString20[] = "Heap Overflow Error\n";
char LoadAudioString21[] = "SEQ  %d ALREADY CACHED\n";
char LoadAudioString22[] = "Ok,one bank slow load Start \n";
char LoadAudioString23[] = "Sorry,too many %d bank is none.fast load Start \n";
char LoadAudioString24[] = "Seq %d:Default Load Id is %d\n";
char LoadAudioString25[] = "Seq Loading Start\n";
char LoadAudioString26[] = "Error:Before Sequence-SlowDma remain.\n";
char LoadAudioString27[] = "      Cancel Seq Start.\n";
char LoadAudioString28[] = "SEQ  %d ALREADY CACHED\n";
char LoadAudioString29[] = "Clear Workarea %x -%x size %x \n";
char LoadAudioString30[] = "AudioHeap is %x\n";
char LoadAudioString31[] = "Heap reset.Synth Change %x \n";
char LoadAudioString32[] = "Heap %x %x %x\n";
char LoadAudioString33[] = "Main Heap Initialize.\n";
char LoadAudioString34[] = "---------- Init Completed. ------------\n";
char LoadAudioString35[] = " Syndrv    :[%6d]\n";
char LoadAudioString36[] = " Seqdrv    :[%6d]\n";
char LoadAudioString37[] = " audiodata :[%6d]\n";
char LoadAudioString38[] = "---------------------------------------\n";

/**
 * Performs an immediate DMA copy
 */
void EmemLoad(u8* devAddr, void* vAddr, size_t nbytes) {
    // eu_stubbed_printf_3("Romcopy %x -> %x ,Size %x\n", devAddr, vAddr, nbytes);
    osInvalDCache(vAddr, nbytes);
    osPiStartDma(&D_803B6740, OS_MESG_PRI_HIGH, OS_READ, (uintptr_t) devAddr, vAddr, nbytes, &D_803B6720);
    osRecvMesg(&D_803B6720, NULL, OS_MESG_BLOCK);
    // eu_stubbed_printf_0("Romcopyend\n");
}

/**
 * Performs an asynchronus (normal Priority) DMA copy
 */
void NasFastRomCopy(uintptr_t devAddr, void* vAddr, size_t nbytes, OSMesgQueue* Queue, OSIoMesg* mesg) {
    osInvalDCache(vAddr, nbytes);
    osPiStartDma(mesg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, vAddr, nbytes, Queue);
}

/**
 * Performs a partial asynchronous (normal Priority) DMA copy. This is limited
 * to 0x1000 bytes transfer at once.
 */
void NasSlowRomCopy(uintptr_t* devAddr, u8** vAddr, ssize_t* remaining, OSMesgQueue* Queue,
                                  OSIoMesg* mesg) {
    ssize_t transfer = (*remaining >= 0x1000 ? 0x1000 : *remaining);
    *remaining -= transfer;
    osInvalDCache(*vAddr, transfer);
    osPiStartDma(mesg, OS_MESG_PRI_NORMAL, OS_READ, *devAddr, *vAddr, transfer, Queue);
    *devAddr += transfer;
    *vAddr += transfer;
}

void NasSlowRomCopy4096() {
    u32 i;

    for (i = 0; i < SSampleDmaListSize1; i++) {
        struct SharedDma* temp = &SSampleDmas[i];
        if (temp->ttl != 0) {
            temp->ttl--;
            if (temp->ttl == 0) {
                temp->ReuseIndex = SSampleDmaReuseQueueHead1;
                SSampleDmaReuseQueue1[SSampleDmaReuseQueueHead1++] = (u8) i;
            }
        }
    }

    for (i = SSampleDmaListSize1; i < SampleDmaNumListItems; i++) {
        struct SharedDma* temp = &SSampleDmas[i];
        if (temp->ttl != 0) {
            temp->ttl--;
            if (temp->ttl == 0) {
                temp->ReuseIndex = SSampleDmaReuseQueueHead2;
                SSampleDmaReuseQueue2[SSampleDmaReuseQueueHead2++] = (u8) i;
            }
        }
    }

    D_803B6E60 = 0;
}

void* NasWaveDmaFrameWork(uintptr_t devAddr, u32 Size, s32 Arg2, u8* dmaIndexRef) {
    s32 hasDma = false;
    struct SharedDma* dma;
    uintptr_t dmaDevAddr;
    u32 transfer;
    u32 i;
    u32 dmaIndex;
    ssize_t bufferPos;
    UNUSED u32 pad;

    if (Arg2 != 0 || *dmaIndexRef >= SSampleDmaListSize1) {
        for (i = SSampleDmaListSize1; i < SampleDmaNumListItems; i++) {
            dma = &SSampleDmas[i];
            bufferPos = devAddr - dma->Source;
            if (0 <= bufferPos && (size_t) bufferPos <= dma->BufSize - Size) {
                // We already have a DMA request for this memory range.
                if (dma->ttl == 0 && SSampleDmaReuseQueueTail2 != SSampleDmaReuseQueueHead2) {
                    // Move the DMA out of the reuse Queue, by swapping it with the
                    // tail, and then incrementing the tail.
                    if (dma->ReuseIndex != SSampleDmaReuseQueueTail2) {
                        SSampleDmaReuseQueue2[dma->ReuseIndex] = SSampleDmaReuseQueue2[SSampleDmaReuseQueueTail2];
                        SSampleDmas[SSampleDmaReuseQueue2[SSampleDmaReuseQueueTail2]].ReuseIndex = dma->ReuseIndex;
                    }
                    SSampleDmaReuseQueueTail2++;
                }
                dma->ttl = 60;
                *dmaIndexRef = (u8) i;
                return &dma->Buffer[(devAddr - dma->Source)];
            }
        }

        if ((SSampleDmaReuseQueueTail2 != SSampleDmaReuseQueueHead2) && (Arg2 != 0)) {
            // Allocate a DMA from reuse Queue 2. This Queue can be empty, since
            // TTL 60 is pretty large.
            dmaIndex = SSampleDmaReuseQueue2[SSampleDmaReuseQueueTail2++];
            dma = &SSampleDmas[dmaIndex];
            hasDma = true;
        }
    } else {
        dma = &SSampleDmas[*dmaIndexRef];
        for (i = 0; i < SSampleDmaListSize1; dma = &SSampleDmas[i++]) {
            bufferPos = devAddr - dma->Source;
            if (0 <= bufferPos && (size_t) bufferPos <= dma->BufSize - Size) {
                // We already have DMA for this memory range.
                if (dma->ttl == 0) {
                    // Move the DMA out of the reuse Queue, by swapping it with the
                    // tail, and then incrementing the tail.
                    if (dma->ReuseIndex != SSampleDmaReuseQueueTail1) {
                        if (1) {}
                        SSampleDmaReuseQueue1[dma->ReuseIndex] = SSampleDmaReuseQueue1[SSampleDmaReuseQueueTail1];
                        SSampleDmas[SSampleDmaReuseQueue1[SSampleDmaReuseQueueTail1]].ReuseIndex = dma->ReuseIndex;
                    }
                    SSampleDmaReuseQueueTail1++;
                }
                dma->ttl = 2;
                return dma->Buffer + (devAddr - dma->Source);
            }
        }
    }

    if (!hasDma) {
        // Allocate a DMA from reuse Queue 1. This Queue will hopefully never
        // be empty, since TTL 2 is so small.
        dmaIndex = SSampleDmaReuseQueue1[SSampleDmaReuseQueueTail1++];
        dma = &SSampleDmas[dmaIndex];
        hasDma = true;
    }

    transfer = dma->BufSize;
    dmaDevAddr = devAddr & ~0xF;
    dma->ttl = 2;
    dma->Source = dmaDevAddr;
    dma->SizeUnused = transfer;
    osPiStartDma(&CurrAudioFrameDmaIoMesgBufs[CurrAudioFrameDmaCount++], OS_MESG_PRI_NORMAL, OS_READ, dmaDevAddr,
                 dma->Buffer, transfer, &CurrAudioFrameDmaQueue);
    *dmaIndexRef = dmaIndex;
    return (devAddr - dmaDevAddr) + dma->Buffer;
}

// init_sample_dma_buffers
void NasWaveDmaCallBack(UNUSED s32 arg0) {
    s32 i;
#define j i

    D_803B70A8 = 0x5A0;

    for (i = 0; i < MaxSimultaneousNotes * 3 * AudioBufferParameters.PresetUnk4; i++) {
        SSampleDmas[SampleDmaNumListItems].Buffer = NasHeapAlloc(&NotesAndBuffersPool, D_803B70A8);
        if (SSampleDmas[SampleDmaNumListItems].Buffer == NULL) {
            break;
        }
        SSampleDmas[SampleDmaNumListItems].BufSize = D_803B70A8;
        SSampleDmas[SampleDmaNumListItems].Source = 0;
        SSampleDmas[SampleDmaNumListItems].SizeUnused = 0;
        SSampleDmas[SampleDmaNumListItems].Unused2 = 0;
        SSampleDmas[SampleDmaNumListItems].ttl = 0;
        SampleDmaNumListItems++;
    }

    for (i = 0; (u32) i < SampleDmaNumListItems; i++) {
        SSampleDmaReuseQueue1[i] = (u8) i;
        SSampleDmas[i].ReuseIndex = (u8) i;
    }

    for (j = SampleDmaNumListItems; j < 0x100; j++) {
        SSampleDmaReuseQueue1[j] = 0;
    }

    SSampleDmaReuseQueueTail1 = 0;
    SSampleDmaReuseQueueHead1 = (u8) SampleDmaNumListItems;
    SSampleDmaListSize1 = SampleDmaNumListItems;

    D_803B70A8 = 0x180;
    for (i = 0; i < MaxSimultaneousNotes; i++) {
        SSampleDmas[SampleDmaNumListItems].Buffer = NasHeapAlloc(&NotesAndBuffersPool, D_803B70A8);
        if (SSampleDmas[SampleDmaNumListItems].Buffer == NULL) {
            break;
        }
        SSampleDmas[SampleDmaNumListItems].BufSize = D_803B70A8;
        SSampleDmas[SampleDmaNumListItems].Source = 0;
        SSampleDmas[SampleDmaNumListItems].SizeUnused = 0;
        SSampleDmas[SampleDmaNumListItems].Unused2 = 0;
        SSampleDmas[SampleDmaNumListItems].ttl = 0;
        SampleDmaNumListItems++;
    }

    for (i = SSampleDmaListSize1; (u32) i < SampleDmaNumListItems; i++) {
        SSampleDmaReuseQueue2[i - SSampleDmaListSize1] = (u8) i;
        SSampleDmas[i].ReuseIndex = (u8) (i - SSampleDmaListSize1);
    }

    // This probably meant to touch the range size1..size2 as well... but it
    // doesn't matter, since these values are never read anyway.
    for (j = SampleDmaNumListItems; j < 0x100; j++) {
        SSampleDmaReuseQueue2[j] = SSampleDmaListSize1;
    }

    SSampleDmaReuseQueueTail2 = 0;
    SSampleDmaReuseQueueHead2 = SampleDmaNumListItems - SSampleDmaListSize1;
#undef j
}

// Similar to NasSmzWaveTouch, but not really
s32 NasWaveDmaNew(struct AudioBankSample* Sample) {
    // struct AudioBankSample *Sample = Sound->Sample;
    UNUSED u8* mem;

    if (Sample == (void*) NULL) {
        return -1;
    }

    if (Sample->Loaded == 1) {
        // temp_a1 = Sound->SampleAddr // unk10;
        mem = NasHeapAlloc(&NotesAndBuffersPool, Sample->SampleSize);
        // temp_a1_2 = temp_v0;
        if (mem == (void*) NULL) {
            return -1;
        }
        EmemLoad(Sample->SampleAddr, mem, Sample->SampleSize);
        Sample->Loaded = 0x81;
        Sample->SampleAddr = mem; // Sound->unk4
    }
}

s32 NasLoadVoice(s32 BankId, s32 instId, s32 Arg2) {
    struct Instrument* instr;
    struct Drum* drum;

    if (instId < 0x7F) {
        instr = ProgToVp(BankId, instId);
        if (instr == NULL) {
            return -1;
        }
        if (instr->NormalRangeLo != 0) {
            NasWaveDmaNew(instr->LowNotesSound.Sample);
        }
        NasWaveDmaNew(instr->NormalNotesSound.Sample);
        if (instr->NormalRangeHi != 0x7F) {
            NasWaveDmaNew(instr->HighNotesSound.Sample);
        }
        //! @bug missing return
    } else if (instId == 0x7F) {
        drum = PercToPp(BankId, Arg2);
        if (drum == NULL) {
            return -1;
        }
        NasWaveDmaNew(drum->Sound.Sample);
        return 0;
    }
#ifdef AVOID_UB
    return 0;
#endif
}

// This appears to be a modified version of alSeqFileNew
// from src/os/alBankNew.c
// Or maybe its patch_seq_file from SM64's load_sh.c?
void NasBankHeaderInit(ALSeqFile* f, u8* base) {
#define PATCH(SRC, BASE, TYPE) SRC = (TYPE) ((uintptr_t) SRC + (uintptr_t) BASE)
    int i;
    u8* wut = base;
    for (i = 0; i < f->seqCount; i++) {
        if (f->seqArray[i].len != 0) {
            PATCH(f->seqArray[i].offset, wut, u8*);
        }
    }
#undef PATCH
}

void NasSmzWaveTouch(struct AudioBankSound* Sound, u8* memBase, u8* offsetBase) {
    struct AudioBankSample* Sample;
    void* patched;
    u8* mem;

#define PATCH(x, base) (patched = (void*) ((uintptr_t) (x) + (uintptr_t) base))

    if (Sound->Sample != NULL) {
        Sample = Sound->Sample = (struct AudioBankSample*) PATCH(Sound->Sample, memBase);
        if (Sample->Loaded == 0) {
            Sample->SampleAddr = (u8*) PATCH(Sample->SampleAddr, offsetBase);
            Sample->Loop = (struct AdpcmLoop*) PATCH(Sample->Loop, memBase);
            Sample->Book = (struct AdpcmBook*) PATCH(Sample->Book, memBase);
            Sample->Loaded = 1;
        } else if (Sample->Loaded == 0x80) {
            PATCH(Sample->SampleAddr, offsetBase);
            mem = NasHeapAlloc(&NotesAndBuffersPool, Sample->SampleSize);
            if (mem == NULL) {
                Sample->SampleAddr = (u8*) patched;
                Sample->Loaded = 1;
            } else {
                EmemLoad((u8*) patched, mem, Sample->SampleSize);
                Sample->Loaded = 0x81;
                Sample->SampleAddr = mem;
            }
            Sample->Loop = (struct AdpcmLoop*) PATCH(Sample->Loop, memBase);
            Sample->Book = (struct AdpcmBook*) PATCH(Sample->Book, memBase);
        }
    }

#undef PATCH
}

// There does not appear to an SM64 counterpart to this function
void NasSmzBnkOfsToAdd(s32 BankId) {
    u8* var_a1;

    if (AlTbl->seqArray[BankId].len == 0) {
        var_a1 = AlTbl->seqArray[(s32) AlTbl->seqArray[BankId].offset].offset;
    } else {
        var_a1 = AlTbl->seqArray[BankId].offset;
    }
    // wtf is up with the `SFXPointer[BankId].Instruments - 1` stuff?
    NasSmzBnkOfsToAddInner((struct AudioBank*) (SFXPointer[BankId].Instruments - 1), var_a1,
                     SFXPointer[BankId].NumInstruments, SFXPointer[BankId].NumDrums);
    SFXPointer[BankId].Drums = (struct Drum**) *(SFXPointer[BankId].Instruments - 1);
}

void NasSmzBnkOfsToAddInner(struct AudioBank* mem, u8* Offset, u32 NumInstruments, u32 NumDrums) {
    struct Instrument* Instrument;
    struct Instrument** itInstrs;
    struct Instrument** end;
    struct AudioBank* temp;
    u32 i;
    void* patched;
    struct Drum* drum;
    struct Drum** Drums;
    u32 numDrums2;

#define BASE_OFFSET_REAL(x, base) (void*) ((uintptr_t) (x) + (uintptr_t) base)
#define PATCH(x, base) (patched = BASE_OFFSET_REAL(x, base))
#define PATCH_MEM(x) x = PATCH(x, mem)

#define BASE_OFFSET(x, base) BASE_OFFSET_REAL(base, x)

    Drums = mem->Drums;
    numDrums2 = NumDrums;
    if (Drums != NULL && numDrums2 > 0) {
        mem->Drums = PATCH(Drums, mem);
        for (i = 0; i < numDrums2; i++) {
            patched = mem->Drums[i];
            if (patched != NULL) {
                drum = PATCH(patched, mem);
                mem->Drums[i] = drum;
                if (drum->Loaded == 0) {
                    NasSmzWaveTouch(&drum->Sound, (u8*) mem, Offset);
                    patched = drum->Envelope;
                    drum->Envelope = BASE_OFFSET(mem, patched);
                    drum->Loaded = 1;
                }
            }
        }
    }

    //! Doesn't affect EU, but required for US/JP
    temp = &*mem;
    if (NumInstruments > 0) {
        //! Doesn't affect EU, but required for US/JP
        struct Instrument** tempInst;
        itInstrs = temp->Instruments;
        tempInst = temp->Instruments;
        end = NumInstruments + tempInst;

        do {
            if (*itInstrs != NULL) {
                *itInstrs = BASE_OFFSET(*itInstrs, mem);
                Instrument = *itInstrs;

                if (Instrument->Loaded == 0) {
                    NasSmzWaveTouch(&Instrument->LowNotesSound, (u8*) mem, Offset);
                    NasSmzWaveTouch(&Instrument->NormalNotesSound, (u8*) mem, Offset);
                    NasSmzWaveTouch(&Instrument->HighNotesSound, (u8*) mem, Offset);
                    patched = Instrument->Envelope;
                    Instrument->Envelope = BASE_OFFSET(mem, patched);
                    Instrument->Loaded = 1;
                }
            }
            itInstrs++;
        } while (end != itInstrs);
    }
#undef PATCH_MEM
#undef PATCH
#undef BASE_OFFSET_REAL
#undef BASE_OFFSET
}

struct AudioBank* NasMyBankLoad(s32 BankId, s32 arg1) {
    s32 alloc;
    UNUSED s32 stackPadding0[9];
    struct AudioBank* ret;
    u8* ctlData;

    alloc = AlCtlHeader->seqArray[BankId].len + 0xf;
    alloc = ALIGN16(alloc);
    alloc -= 0x10;
    ctlData = AlCtlHeader->seqArray[BankId].offset;
    ret = NasSzAutoDivide(&BankLoadedPool, 1, alloc, arg1, BankId);
    if (ret == NULL) {
        return NULL;
    }
    EmemLoad(ctlData + 0x10, ret, (u32) alloc);
    SFXPointer[BankId].Instruments = ret->Instruments;
    NasSmzBnkOfsToAdd(BankId);
    if (BankLoadStatus[BankId] != 5) {
        BankLoadStatus[BankId] = 2;
    }
    return ret;
}

struct AudioBank* NasMyBankSlowLoad(s32 BankId, s32 arg1, struct SequencePlayer* seqPlayer) {
    size_t alloc;
    UNUSED s32 stackPadding0[9];
    struct AudioBank* ret;
    u8* ctlData;
    UNUSED s32 stackPadding1[2];

    alloc = AlCtlHeader->seqArray[BankId].len + 0xF;
    alloc = ALIGN16(alloc);
    alloc -= 0x10;
    ctlData = AlCtlHeader->seqArray[BankId].offset;
    ret = NasSzAutoDivide(&BankLoadedPool, 1, alloc, arg1, BankId);
    if (ret == NULL) {
        return NULL;
    }
    seqPlayer->LoadingBankId = BankId;
    SFXPointer[BankId].Instruments = ret->Instruments;
    SFXPointer[BankId].Drums = NULL;
    seqPlayer->BankDmaCurrMemAddr = (u8*) ret;
    seqPlayer->BankDmaCurrDevAddr = (uintptr_t) (ctlData + 0x10);
    seqPlayer->BankDmaRemaining = alloc;
    if (1) {}
    osCreateMesgQueue(&seqPlayer->BankDmaMesgQueue, &seqPlayer->BankDmaMesg, 1);
    seqPlayer->BankDmaInProgress = true;
    NasSlowRomCopy(&seqPlayer->BankDmaCurrDevAddr, &seqPlayer->BankDmaCurrMemAddr,
                                 &seqPlayer->BankDmaRemaining, &seqPlayer->BankDmaMesgQueue, &seqPlayer->BankDmaIoMesg);
    if (BankLoadStatus[BankId] != 5) {
        BankLoadStatus[BankId] = 1;
    }
    return ret;
}

void* NasMySeqLoad(s32 SEQId, s32 arg1) {
    s32 seqLength;
    void* ptr;
    u8* SEQData;

    seqLength = MUSTablePointer->seqArray[SEQId].len;
    seqLength = ALIGN16(seqLength);
    SEQData = MUSTablePointer->seqArray[SEQId].offset;
    ptr = NasSzAutoDivide(&SeqLoadedPool, 1, seqLength, arg1, SEQId);
    if (ptr == NULL) {
        return NULL;
    }

    EmemLoad(SEQData, ptr, seqLength);
    if (SeqLoadStatus[SEQId] != 5) {
        SeqLoadStatus[SEQId] = 2;
    }
    return ptr;
}

void* NasMySeqLoadSlow(s32 SEQId, s32 arg1, struct SequencePlayer* seqPlayer) {
    s32 seqLength;
    void* ptr;
    u8* SEQData;
    OSMesgQueue* mesgQueue;

    seqLength = MUSTablePointer->seqArray[SEQId].len;
    seqLength = ALIGN16(seqLength);
    SEQData = MUSTablePointer->seqArray[SEQId].offset;
    ptr = NasSzAutoDivide(&SeqLoadedPool, 1, seqLength, arg1, SEQId);
    if (ptr == NULL) {
        return NULL;
    }
    if (seqLength < 0x41) {
        EmemLoad(SEQData, ptr, (u32) seqLength);
        if (1) {}
        if (SeqLoadStatus[SEQId] != 5) {
            SeqLoadStatus[SEQId] = 2;
        }
    } else {
        EmemLoad(SEQData, ptr, 0x00000040U);
        mesgQueue = &seqPlayer->SEQDmaMesgQueue;
        osCreateMesgQueue(mesgQueue, &seqPlayer->SEQDmaMesg, 1);
        seqPlayer->SEQDmaInProgress = true;
        NasFastRomCopy((uintptr_t) (SEQData + 0x40), (u8*) ptr + 0x40, seqLength - 0x40, mesgQueue,
                             &seqPlayer->SEQDmaIoMesg);
        if (SeqLoadStatus[SEQId] != 5) {
            SeqLoadStatus[SEQId] = 1;
        }
    }
    return ptr;
}

u8 NasCountBankInSeq(u32 SEQId, s32* nonNullCount, s32* nullCount) {
    void* temp;
    u32 BankId;
    u16 Offset;
    u8 i;
    u8 ret;

    *nullCount = 0;
    *nonNullCount = 0;
    Offset = ((u16*) AlBankSets)[SEQId];
    for (i = AlBankSets[Offset++], ret = 0; i != 0; i--) {
        BankId = AlBankSets[Offset++];

        if (IS_BANK_LOAD_COMPLETE(BankId) == true) {
            temp = NasSzHeapAlloc(1, 2, BankId);
        } else {
            temp = NULL;
        }

        if (temp == NULL) {
            (*nullCount)++;
            ret = BankId;
        } else {
            (*nonNullCount)++;
        }
    }

    return ret;
}

struct AudioBank* NasPreLoadBank(s32 SEQId, u8* outDefaultBank) {
    void* ret;
    u32 BankId;
    u16 Offset;
    u8 i;

    Offset = ((u16*) AlBankSets)[SEQId];
    for (i = AlBankSets[Offset++]; i != 0; i--) {
        BankId = AlBankSets[Offset++];

        if (IS_BANK_LOAD_COMPLETE(BankId) == true) {
            ret = NasSzHeapAlloc(1, 2, BankId);
        } else {
            ret = NULL;
        }

        if (ret == NULL) {
            ret = NasMyBankLoad(BankId, 2);
        }
    }
    *outDefaultBank = BankId;
    return ret;
}

void NasPreLoadSeq(u32 SEQId, u8 preloadMask) {
    void* sequenceData;
    u8 temp;

    if (SEQId >= SequenceCount) {
        return;
    }

    if (MUSTablePointer->seqArray[SEQId].len == 0) {
        SEQId = (u32) MUSTablePointer->seqArray[SEQId].offset;
    }

    AudioLoadLock = AUDIO_LOCK_LOADING;
    if (preloadMask & PRELOAD_BANKS) {
        NasPreLoadBank(SEQId, &temp);
    }

    if (preloadMask & PRELOAD_SEQUENCE) {
        //! @bug should be IS_SEQ_LOAD_COMPLETE
        if (IS_BANK_LOAD_COMPLETE(SEQId) == true) {
            sequenceData = NasSzHeapAlloc(0, 2, SEQId);
        } else {
            sequenceData = NULL;
        }
        if (sequenceData == NULL && NasMySeqLoad(SEQId, 2) == NULL) {
            AudioLoadLock = AUDIO_LOCK_NOT_LOADING;
            return;
        }
    }

    AudioLoadLock = AUDIO_LOCK_NOT_LOADING;
}

void NasStartMySeq(u32 Player, u32 SEQId, s32 loadAsync) {
    if (!loadAsync) {
        AudioLoadLock = AUDIO_LOCK_LOADING;
    }
    NasStartMySeqInner(Player, SEQId, loadAsync);
    if (!loadAsync) {
        AudioLoadLock = AUDIO_LOCK_NOT_LOADING;
    }
}

void NasStartMySeqInner(u32 Player, u32 SEQId, s32 loadAsync) {
    void* sequenceData;
    struct SequencePlayer* seqPlayer = &SeqPlayer[Player];
    UNUSED u32 Padding[2];

    if (SEQId >= SequenceCount) {
        return;
    }

    if (MUSTablePointer->seqArray[SEQId].len == 0) {
        SEQId = (u32) MUSTablePointer->seqArray[SEQId].offset;
    }

    NasGetNewSubTrack(seqPlayer);
    if (loadAsync) {
        s32 numMissingBanks = 0;
        s32 Dummy = 0;
        s32 BankId = NasCountBankInSeq(SEQId, &Dummy, &numMissingBanks);
        if (numMissingBanks == 1) {
            if (NasMyBankSlowLoad(BankId, 2, seqPlayer) == NULL) {
                return;
            }
            /**
             * @bug This should set the last bank (i.e. the first in the JSON)
             * as default, not the missing one. This Code path never gets
             * taken, though -- all sequence loading is synchronous.
             */
            seqPlayer->DefaultBank[0] = BankId;
        } else {
            if (NasPreLoadBank(SEQId, &seqPlayer->DefaultBank[0]) == NULL) {
                return;
            }
        }
    } else if (NasPreLoadBank(SEQId, &seqPlayer->DefaultBank[0]) == NULL) {
        return;
    }

    seqPlayer->SEQId = SEQId;
    sequenceData = NasSzHeapAlloc(0, 2, SEQId);
    if (sequenceData == NULL) {
        if (seqPlayer->SEQDmaInProgress) {
            return;
        }
        if (loadAsync) {
            sequenceData = NasMySeqLoadSlow(SEQId, 2, seqPlayer);
        } else {
            sequenceData = NasMySeqLoad(SEQId, 2);
        }

        if (sequenceData == NULL) {
            return;
        }
    }

    NasSubVoiceSet(Player);
    seqPlayer->ScriptState.Depth = 0;
    seqPlayer->Delay = 0;
    seqPlayer->Enabled = true;
    seqPlayer->SEQData = sequenceData;
    seqPlayer->ScriptState.pc = sequenceData;
}

extern u8 _audio_banksSegmentRomStart;
extern u8 _audio_tablesSegmentRomStart;
extern u8 _instrument_setsSegmentRomStart;
extern u8 _sequencesSegmentRomStart;
/**
 * There is some wild bullshit going on in this function
 *  It is an unholy cross between SM64's EU and Shindou
 *  verions of this function, with the for Loop towards
 *  the bottom resembling stuff from NasMyBankSlowLoad
 */
void NasInitAudio(void) {
    s32 i;
    UNUSED s32 pad[6];
    s32 j, k;
    s32 ctlSeqCount;
    u32 buf[2];
    UNUSED s32 lim2, lim3;
    s32 Size;
    u64* ptr64;
    UNUSED s32 Pad2;
    UNUSED s32 one = 1;
    void* data;

    AudioLoadLock = 0;

    for (i = 0; i < AudioHeapSize / 8; i++) {
        ((u64*) AudioHeap)[i] = 0;
    }

#ifdef TARGET_N64
    // It seems boot.s doesn't clear the .bss Area for audio, so do it here.
    ptr64 = (u64*) ((u8*) GfxSPTaskOutputBuffer + sizeof(GfxSPTaskOutputBuffer));
    for (k = ((uintptr_t) &AudioGlobalsEndMarker - (uintptr_t) ((u64 *)((u8 *) GfxSPTaskOutputBuffer + sizeof(GfxSPTaskOutputBuffer))) ) / 8; k >= 0; k--) {
        *ptr64++ = 0;
    }
#endif

#ifdef VERSION_EU
    D_803B7178 = 20.03042f;
    RefreshRate = 50;
#else // US
    switch (osTvType) {
        case TV_TYPE_PAL:
            D_803B7178 = 20.03042f;
            RefreshRate = 50;
            break;
        case TV_TYPE_MPAL:
            D_803B7178 = 16.546f;
            RefreshRate = 60;
            break;
        case TV_TYPE_NTSC:
        default:
            D_803B7178 = 16.713f;
            RefreshRate = 60;
            break;
    }
#endif
    PortEUInit();
    if (k) {} // fake
    for (i = 0; i < NUMAIBUFFERS; i++) {
        AiBufferLengths[i] = 0xa0;
    }

    AudioTaskIndex = AudioFrameCount = 0;
    CurrAiBufferIndex = 0;
    AudioLibSoundMode = 0;
    AudioTask = NULL;
    AudioTasks[0].Task.t.data_size = 0;
    AudioTasks[1].Task.t.data_size = 0;
    osCreateMesgQueue(&D_803B6720, &D_803B6738, 1);
    osCreateMesgQueue(&CurrAudioFrameDmaQueue, CurrAudioFrameDmaMesgBufs, ARRAY_COUNT(CurrAudioFrameDmaMesgBufs));
    CurrAudioFrameDmaCount = 0;
    SampleDmaNumListItems = 0;

    NasSzCustomClear(AudioInitPoolSize);

    for (i = 0; i < NUMAIBUFFERS; i++) {
        AiBuffers[i] = NasHeapAlloc(&AudioInitPool, AIBUFFER_LEN);

        for (j = 0; j < (s32) (AIBUFFER_LEN / sizeof(s16)); j++) {
            AiBuffers[i][j] = 0;
        }
    }

    AudioResetPresetIdToLoad = 0;
    AudioResetStatus = one;
    NasDelayDown();

    // Load headers for sounds and sequences
    MUSTablePointer = (ALSeqFile*) buf;
    data = &_sequencesSegmentRomStart;
    EmemLoad(data, MUSTablePointer, 0x10);
    SequenceCount = MUSTablePointer->seqCount;
    Size = SequenceCount * sizeof(ALSeqData) + 4;
    Size = ALIGN16(Size);
    MUSTablePointer = NasHeapAlloc(&AudioInitPool, Size);
    EmemLoad(data, MUSTablePointer, Size);
    NasBankHeaderInit(MUSTablePointer, data);

    // Load header for CTL (Instrument metadata)
    AlCtlHeader = (ALSeqFile*) buf;
    data = &_audio_banksSegmentRomStart;
    EmemLoad(data, AlCtlHeader, 0x10);
    ctlSeqCount = AlCtlHeader->seqCount;
    Size = ALIGN16(ctlSeqCount * sizeof(ALSeqData) + 4);
    AlCtlHeader = NasHeapAlloc(&AudioInitPool, Size);
    EmemLoad(data, AlCtlHeader, Size);
    NasBankHeaderInit(AlCtlHeader, data);
    SFXPointer = NasHeapAlloc(&AudioInitPool, ctlSeqCount * sizeof(struct CtlEntry));
    for (i = 0; i < ctlSeqCount; i++) {
        EmemLoad(AlCtlHeader->seqArray[i].offset, buf, 0x10);
        SFXPointer[i].NumInstruments = buf[0];
        SFXPointer[i].NumDrums = buf[1];
    }

    // Load header for TBL (raw Sound data)
    AlTbl = (ALSeqFile*) buf;
    data = &_audio_tablesSegmentRomStart;
    EmemLoad(data, AlTbl, 0x10);
    Size = AlTbl->seqCount * sizeof(ALSeqData) + 4;
    Size = ALIGN16(Size);
    AlTbl = NasHeapAlloc(&AudioInitPool, Size);
    EmemLoad(data, AlTbl, Size);
    NasBankHeaderInit(AlTbl, data);

    // Load bank sets for each sequence
    AlBankSets = NasHeapAlloc(&AudioInitPool, 0x100);
    EmemLoad((u8 *) &_instrument_setsSegmentRomStart, AlBankSets, 0x100);

    NasHeapInit(&UnkPool1.Pool, NasHeapAlloc(&AudioInitPool, (u32) D_800EA5D8), (u32) D_800EA5D8);
    NasSubVolumeSet();
    AudioLoadLock = 0x76557364;
}
