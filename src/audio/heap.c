#include <ultra64.h>
#include <macros.h>

#include "audio/data.h"
#include "audio/effects.h"
#include "audio/heap.h"
#include "audio/load.h"
#include "audio/synthesis.h"
#include "audio/playback.h"
#include "audio/seqplayer.h"

s16 Volume;
s8 UseReverb;
s8 NumSynthesisReverbs;
struct NoteSubEu* NoteSubsEu;

struct SoundAllocPool AudioSessionPool;
struct SoundAllocPool AudioInitPool;
struct SoundAllocPool NotesAndBuffersPool;
u8 SAudioHeapPad[0x20]; // probably two Unused pools
struct SoundAllocPool SeqAndBankPool;
struct SoundAllocPool PersistentCommonPool;
struct SoundAllocPool TemporaryCommonPool;

struct SoundMultiPool SeqLoadedPool;
struct SoundMultiPool BankLoadedPool;
struct SoundMultiPool UnusedLoadedPool;

struct Unk1Pool UnkPool1;

struct PoolSplit SSessionPoolSplit;
struct PoolSplit2 SSeqAndBankPoolSplit;
struct PoolSplit SPersistentCommonPoolSplit;
struct PoolSplit STemporaryCommonPoolSplit;

u8 UnkLoadStatus[0x40];
u8 BankLoadStatus[0x40];
u8 SeqLoadStatus[0x100];

volatile u8 AudioResetStatus;
u8 AudioResetPresetIdToLoad;
s32 AudioResetFadeOutFramesLeft;

u8 AudioUnusedBuffer[0x1000];

struct Note* Notes;

/**
 * Given that (almost) all of these are format strings, it is highly likely
 * that they are meant to be Used in some sort of printf Variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 **/
char HeapAudioString00[] = "Warning:Kill Note  %x \n";
char HeapAudioString01[] = "Kill Voice %d (ID %d) %d\n";
char HeapAudioString02[] = "Warning: Running Sequence's data disappear!\n";
char HeapAudioString03[] = "Audio:Memory:Heap OverFlow : Not Allocate %d!\n";
char HeapAudioString04[] = "Audio:Memory:DataHeap Not Allocate \n";
char HeapAudioString05[] = "StayHeap Not Allocate %d\n";
char HeapAudioString06[] = "AutoHeap Not Allocate %d\n";
char HeapAudioString07[] = "Status ID0 : %d  ID1 : %d\n";
char HeapAudioString08[] = "id 0 is Stopping\n";
char HeapAudioString09[] = "id 0 is Stop\n";
char HeapAudioString10[] = "id 1 is Stopping\n";
char HeapAudioString11[] = "id 1 is Stop\n";
char HeapAudioString12[] = "WARNING: NO FREE AUTOSEQ AREA.\n";
char HeapAudioString13[] = "WARNING: NO STOP AUTO AREA.\n";
char HeapAudioString14[] = "         AND TRY FORCE TO STOP SIDE \n";
char HeapAudioString15[] = "Check ID0  (seq ID %d) Useing ...\n";
char HeapAudioString16[] = "Check ID1  (seq ID %d) Useing ...\n";
char HeapAudioString17[] = "No Free Seq area.\n";
char HeapAudioString18[] = "CH %d: ID %d\n";
char HeapAudioString19[] = "TWO SIDES ARE LOADING... ALLOC CANCELED.\n";
char HeapAudioString20[] = "WARNING: Before Area Overlaid After.";
char HeapAudioString21[] = "WARNING: After Area Overlaid Before.";
char HeapAudioString22[] = "MEMORY:SzHeapAlloc ERROR: sza->side %d\n";
char HeapAudioString23[] = "Audio:MEMORY:SzHeap Overflow error. (%d bytes)\n";
char HeapAudioString24[] = "Auto Heap Unhit for ID %d\n";
char HeapAudioString25[] = "Heap Reconstruct Start %x\n";
char HeapAudioString26[] = "AHPBASE %x\n";
char HeapAudioString27[] = "AHPCUR  %x\n";
char HeapAudioString28[] = "HeapTop %x\n";
char HeapAudioString29[] = "SynoutRate %d / %d \n";
char HeapAudioString30[] = "FXSIZE %d\n";
char HeapAudioString31[] = "FXCOMP %d\n";
char HeapAudioString32[] = "FXDOWN %d\n";
char HeapAudioString33[] = "WaveCacheLen: %d\n";
char HeapAudioString34[] = "SpecChange Finished\n";
char HeapAudioString35[] = "Fbank Seq %x\n";
char HeapAudioString36[] = "Already Load Type %d,ID %d\n";
char HeapAudioString37[] = "Warning:Emem Over,not alloc %d\n";
char HeapAudioString38[] = "Write %d\n";

extern u8 UnkLoadStatus[];

void NasResetIDtable(void) {
    s32 i;
    for (i = 0; i < 64; i++) {
        if (BankLoadStatus[i] != 5) {
            BankLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 64; i++) {
        if (UnkLoadStatus[i] != 5) {
            UnkLoadStatus[i] = 0;
        }
    }

    for (i = 0; i < 256; i++) {
        if (SeqLoadStatus[i] != 5) {
            SeqLoadStatus[i] = 0;
        }
    }
}

void NasForceStopChannel(s32 BankId) {
    s32 i;

    for (i = 0; i < MaxSimultaneousNotes; i++) {
        struct Note* Note = &Notes[i];

        if (Note->NoteSubEu.BankId == BankId) {
            // (These prints are unclear. Arguments are picked semi-randomly.)
            // eu_stubbed_printf_1("Warning:Kill Note  %x \n", i);
            if (Note->Priority >= NOTE_PRIORITY_MIN) {
                // eu_stubbed_printf_3("Kill Voice %d (ID %d) %d\n", Note->WaveId, BankId, Note->Priority);
                // eu_stubbed_printf_0("Warning: Running Sequence's data disappear!\n");
                Note->ParentLayer->Enabled = false; // is 0x48, should be 0x44
                Note->ParentLayer->Finished = true;
            }
            NasStopVoice(Note);
            NasDeAllocAllVoices(&Note->ListItem);
            NasAllocSub(&NoteFreeLists.Disabled, &Note->ListItem);
        }
    }
}

void NasForceStopSeq(s32 SEQId) {
    s32 i;

    for (i = 0; i < SEQUENCE_PLAYERS; i++) {
        if (SeqPlayer[i].Enabled && SeqPlayer[i].SEQId == SEQId) {
            NasGetNewSubTrack(&SeqPlayer[i]);
        }
    }
}

void* NasHeapAlloc(struct SoundAllocPool* Pool, u32 Size) {
    u8* Start;
    u8* pos;
    u32 alignedSize = ALIGN16(Size);

    Start = Pool->cur;
    if (Start + alignedSize <= Pool->Start + Pool->Size) {
        Pool->cur += alignedSize;
        for (pos = Start; pos < Pool->cur; pos++) {
            *pos = 0;
        }
    } else {
        // eu_stubbed_printf_1("Heap OverFlow : Not Allocate %d!\n", Size);
        return NULL;
    }
    Pool->NumAllocatedEntries++;
    return Start;
}

void NasHeapInit(struct SoundAllocPool* Pool, void* memAddr, u32 Size) {
    Pool->cur = Pool->Start = (u8*) ALIGN16((uintptr_t) memAddr);
    Pool->Size = Size;
    Pool->NumAllocatedEntries = 0;
}

void NasSzStayClear(struct PersistentPool* Persistent) {
    Persistent->Pool.NumAllocatedEntries = 0;
    Persistent->Pool.cur = Persistent->Pool.Start;
    Persistent->NumEntries = 0;
}

void NasSzAutoClear(struct TemporaryPool* Temporary) {
    Temporary->Pool.NumAllocatedEntries = 0;
    Temporary->Pool.cur = Temporary->Pool.Start;
    Temporary->NextSide = 0;
    Temporary->Entries[0].ptr = Temporary->Pool.Start;
    Temporary->Entries[1].ptr = Temporary->Pool.Start + Temporary->Pool.Size;
    Temporary->Entries[0].id = -1; // should be at 1e not 1c
    Temporary->Entries[1].id = -1;
}

void func_800B90E0(struct SoundAllocPool* Pool) {
    Pool->NumAllocatedEntries = 0;
    Pool->cur = Pool->Start;
}

// inspired by NasSzCustomClear in sm64
void NasSzCustomClear(s32 arg0) {
    NasHeapInit(&AudioInitPool, &AudioHeap, arg0);
    NasHeapInit(&AudioSessionPool, AudioHeap + arg0, AudioHeapSize - arg0);
}

// inspired by session_pools_init in sm64
void NasSzHeapReset(struct PoolSplit* arg0) {
    AudioSessionPool.cur = AudioSessionPool.Start;
    NasHeapInit(&NotesAndBuffersPool, NasHeapAlloc(&AudioSessionPool, arg0->WantSeq), arg0->WantSeq);
    NasHeapInit(&SeqAndBankPool, NasHeapAlloc(&AudioSessionPool, arg0->WantCustom), arg0->WantCustom);
}

void NasSzHeapDivide(struct PoolSplit2* a) {
    SeqAndBankPool.cur = SeqAndBankPool.Start;
    NasHeapInit(&PersistentCommonPool, NasHeapAlloc(&SeqAndBankPool, a->WantPersistent), a->WantPersistent);
    NasHeapInit(&TemporaryCommonPool, NasHeapAlloc(&SeqAndBankPool, a->WantTemporary), a->WantTemporary);
}

void NasSzDataDivide(struct PoolSplit* a) {
    PersistentCommonPool.cur = PersistentCommonPool.Start;
    NasHeapInit(&SeqLoadedPool.Persistent.Pool, NasHeapAlloc(&PersistentCommonPool, a->WantSeq), a->WantSeq);
    NasHeapInit(&BankLoadedPool.Persistent.Pool, NasHeapAlloc(&PersistentCommonPool, a->WantBank),
                          a->WantBank);
    NasHeapInit(&UnusedLoadedPool.Persistent.Pool, NasHeapAlloc(&PersistentCommonPool, a->WantUnused),
                          a->WantUnused);
    NasSzStayClear(&SeqLoadedPool.Persistent);
    NasSzStayClear(&BankLoadedPool.Persistent);
    NasSzStayClear(&UnusedLoadedPool.Persistent);
}

void NasSzStayDivide(struct PoolSplit* a) {
    TemporaryCommonPool.cur = TemporaryCommonPool.Start;
    NasHeapInit(&SeqLoadedPool.Temporary.Pool, NasHeapAlloc(&TemporaryCommonPool, a->WantSeq), a->WantSeq);
    NasHeapInit(&BankLoadedPool.Temporary.Pool, NasHeapAlloc(&TemporaryCommonPool, a->WantBank), a->WantBank);
    NasHeapInit(&UnusedLoadedPool.Temporary.Pool, NasHeapAlloc(&TemporaryCommonPool, a->WantUnused),
                          a->WantUnused);
    NasSzAutoClear(&SeqLoadedPool.Temporary);
    NasSzAutoClear(&BankLoadedPool.Temporary);
    NasSzAutoClear(&UnusedLoadedPool.Temporary);
}

void* NasSzAutoDivide(struct SoundMultiPool* arg0, s32 arg1, s32 Size, s32 Arg3, s32 id) {
    struct TemporaryPool* tp;
    struct SoundAllocPool* Pool;
    void* ret;
    u16 UNUSED _firstVal;
    u16 UNUSED _secondVal;
    u16 firstVal;
    u16 secondVal;
    s32 var_v1_2;
    u8* Table;
    u8 isSound;

    if (Arg3 == 0) {
        tp = &arg0->Temporary;
        // clang-format off
        if (arg0 == &SeqLoadedPool) {
            Table = SeqLoadStatus; isSound = 0;
        } else {
            // wtf?
            firstVal += 0;
            if (arg0 == &BankLoadedPool) {
                isSound = 1; Table = BankLoadStatus;
            } else if (arg0 == &UnusedLoadedPool) {
                Table = UnkLoadStatus; isSound = 2;
            }
        }
        // clang-format on
        if (tp->Entries[0].id == -1) {
            firstVal = 0;
        } else {
            firstVal = Table[tp->Entries[0].id];
        }
        if (tp->Entries[1].id == -1) {
            secondVal = 0;
        } else {
            secondVal = Table[tp->Entries[1].id];
        }
        if (isSound == 1) {
            if (firstVal == 4) {
                for (var_v1_2 = 0; var_v1_2 < MaxSimultaneousNotes; var_v1_2++) {
                    if (Notes[var_v1_2].NoteSubEu.BankId == tp->Entries[0].id && Notes[var_v1_2].NoteSubEu.Enabled) {
                        break;
                    }
                }
                if (var_v1_2 == MaxSimultaneousNotes) {
                    if (BankLoadStatus[tp->Entries[0].id] != SOUND_LOAD_STATUS_5) {
                        BankLoadStatus[tp->Entries[0].id] = SOUND_LOAD_STATUS_DISCARDABLE;
                    }
                    firstVal = SOUND_LOAD_STATUS_DISCARDABLE;
                }
            }
            if (secondVal == 4) {
                for (var_v1_2 = 0; var_v1_2 < MaxSimultaneousNotes; var_v1_2++) {
                    if (Notes[var_v1_2].NoteSubEu.BankId == tp->Entries[1].id && Notes[var_v1_2].NoteSubEu.Enabled) {
                        break;
                    }
                }
                if (var_v1_2 == MaxSimultaneousNotes) {
                    if (BankLoadStatus[tp->Entries[1].id] != SOUND_LOAD_STATUS_5) {
                        BankLoadStatus[tp->Entries[1].id] = SOUND_LOAD_STATUS_DISCARDABLE;
                    }
                    secondVal = SOUND_LOAD_STATUS_DISCARDABLE;
                }
            }
        }
        if (firstVal == 0) {
            tp->NextSide = 0;
        } else if (secondVal == 0) {
            tp->NextSide = 1;
        } else {
            if ((firstVal == 3) && (secondVal == 3)) {
                // ?
            } else if (firstVal == 3) {
                tp->NextSide = 0;
            } else if (secondVal == 3) {
                tp->NextSide = 1;
            } else {
                if (isSound == 0) {
                    if (firstVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < SEQUENCE_PLAYERS; var_v1_2++) {
                            if (SeqPlayer[var_v1_2].Enabled &&
                                SeqPlayer[var_v1_2].SEQId == tp->Entries[0].id) {
                                break;
                            }
                        }
                        if (var_v1_2 == SEQUENCE_PLAYERS) {
                            tp->NextSide = 0;
                            goto out;
                        }
                    }
                    if (secondVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < SEQUENCE_PLAYERS; var_v1_2++) {
                            if (SeqPlayer[var_v1_2].Enabled &&
                                SeqPlayer[var_v1_2].SEQId == tp->Entries[1].id) {
                                break;
                            }
                        }
                        if (var_v1_2 == SEQUENCE_PLAYERS) {
                            tp->NextSide = 1;
                            goto out;
                        }
                    }
                } else if (isSound == 1) {
                    if (firstVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < MaxSimultaneousNotes; var_v1_2++) {
                            if (Notes[var_v1_2].NoteSubEu.BankId == tp->Entries[0].id &&
                                Notes[var_v1_2].NoteSubEu.Enabled) {
                                break;
                            }
                        }
                        if (var_v1_2 == MaxSimultaneousNotes) {
                            tp->NextSide = 0;
                            goto out;
                        }
                    }
                    if (secondVal == SOUND_LOAD_STATUS_COMPLETE) {
                        for (var_v1_2 = 0; var_v1_2 < MaxSimultaneousNotes; var_v1_2++) {
                            if (Notes[var_v1_2].NoteSubEu.BankId == tp->Entries[1].id &&
                                Notes[var_v1_2].NoteSubEu.Enabled) {
                                break;
                            }
                        }
                        if (var_v1_2 == MaxSimultaneousNotes) {
                            tp->NextSide = 1;
                            goto out;
                        }
                    }
                }
                if (tp->NextSide == 0) {
                    if (firstVal == SOUND_LOAD_STATUS_IN_PROGRESS) {
                        if (secondVal != SOUND_LOAD_STATUS_IN_PROGRESS) {
                            tp->NextSide = 1;
                            goto out;
                        }
                    } else {
                        goto out;
                    }
                } else {
                    if (secondVal == SOUND_LOAD_STATUS_IN_PROGRESS) {
                        if (firstVal != SOUND_LOAD_STATUS_IN_PROGRESS) {
                            tp->NextSide = 0;
                            goto out;
                        }
                    } else {
                        goto out;
                    }
                }
                return NULL;
            out:;
            }
        }

        Pool = &arg0->Temporary.Pool;
        if (tp->Entries[tp->NextSide].id != (s8) -1) {
            Table[tp->Entries[tp->NextSide].id] = SOUND_LOAD_STATUS_NOT_LOADED;
            if (isSound == true) {
                NasForceStopChannel(tp->Entries[tp->NextSide].id);
            }
        }
        switch (tp->NextSide) {
            case 0:
                tp->Entries[0].ptr = Pool->Start;
                tp->Entries[0].id = (s16) id;
                tp->Entries[0].Size = (u32) Size;
                Pool->cur = Pool->Start + Size;
                if (tp->Entries[1].ptr < Pool->cur) {
                    Table[tp->Entries[1].id] = 0;
                    switch (isSound) { /* irregular */
                        case 0:
                            NasForceStopSeq((s32) tp->Entries[1].id);
                            break;
                        case 1:
                            NasForceStopChannel((s32) tp->Entries[1].id);
                            break;
                    }
                    tp->Entries[1].id = -1;
                    tp->Entries[1].ptr = Pool->Start + Pool->Size;
                }
                ret = tp->Entries[0].ptr;
                break;
            case 1:
                tp->Entries[1].ptr = Pool->Start + Pool->Size - Size - 0x10;
                tp->Entries[1].id = (s16) id;
                tp->Entries[1].Size = (u32) Size;
                if ((u32) tp->Entries[1].ptr < (u32) Pool->cur) {
                    Table[tp->Entries[0].id] = 0;
                    switch (isSound) { /* switch 1; irregular */
                        case 0:        /* switch 1 */
                            NasForceStopSeq((s32) tp->Entries[0].id);
                            break;
                        case 1: /* switch 1 */
                            NasForceStopChannel((s32) tp->Entries[0].id);
                            break;
                    }
                    tp->Entries[0].id = -1;
                    Pool->cur = Pool->Start;
                }
                ret = tp->Entries[1].ptr;
                break;
            default:
                return NULL;
        }
        tp->NextSide ^= 1;
        return ret;
    }
    ret = NasHeapAlloc(&arg0->Persistent.Pool, arg1 * Size);
    arg0->Persistent.Entries[arg0->Persistent.NumEntries].ptr = ret;
    if (ret == NULL) {
        switch (Arg3) {
            case 2:
                return NasSzAutoDivide(arg0, arg1, Size, 0, id);
            case 1:
            case 0:
                return NULL;
        }
    }
    arg0->Persistent.Entries[arg0->Persistent.NumEntries].id = (s16) id;
    arg0->Persistent.Entries[arg0->Persistent.NumEntries].Size = (u32) Size;
    return arg0->Persistent.Entries[arg0->Persistent.NumEntries++].ptr;
}

void* NasSzHeapAlloc(s32 poolIdx, s32 arg1, s32 id) {
    void* ret;

    ret = NasMemoryReconfig(poolIdx, id);
    if (ret != NULL) {
        return ret;
    }
    return NasSzCacheCheck(poolIdx, arg1, id);
}

void* NasSzCacheCheck(s32 poolIdx, s32 arg1, s32 BankId) {
    u32 i;
    struct SoundMultiPool* loadedPool;
    struct TemporaryPool* Temporary;
    struct PersistentPool* Persistent;

    switch (poolIdx) {
        case 0:
            loadedPool = &SeqLoadedPool;
            break;
        case 1:
            loadedPool = &BankLoadedPool;
            break;
        case 2:
            loadedPool = &UnusedLoadedPool;
            break;
    }

    Temporary = &loadedPool->Temporary;
    if (arg1 == 0) {
        if (Temporary->Entries[0].id == BankId) {
            Temporary->NextSide = 1;
            return Temporary->Entries[0].ptr;
        } else if (Temporary->Entries[1].id == BankId) {
            Temporary->NextSide = 0;
            return Temporary->Entries[1].ptr;
        } else {
            return NULL;
        }
    }

    Persistent = &loadedPool->Persistent;
    for (i = 0; i < Persistent->NumEntries; i++) {
        if (Persistent->Entries[i].id == BankId) {
            return Persistent->Entries[i].ptr;
        }
    }

    if (arg1 == 2) {
        return NasSzHeapAlloc(poolIdx, 0, BankId);
    }
    return NULL;
}

void NasSzCacheCheckInner(f32 arg0, f32 arg1, u16* Arg2) {
    s32 i;
    f32 tmp[16];

    tmp[0] = arg1 * 262159.0f;
    tmp[8] = arg0 * 262159.0f;
    tmp[1] = (arg1 * arg0) * 262159.0f;
    tmp[9] = ((arg0 * arg0) + arg1) * 262159.0f;

    for (i = 2; i < 8; i++) {
        //! @bug they probably meant to store the Value to tmp[i] and tmp[8 + i]
        Arg2[i] = arg1 * tmp[i - 2] + arg0 * tmp[i - 1];
        Arg2[8 + i] = arg1 * tmp[6 + i] + arg0 * tmp[7 + i];
    }

    for (i = 0; i < 16; i++) {
        Arg2[i] = tmp[i];
    }
}

void NasInitFilterCoef(void) {
    s32 i;
    for (i = 0; i < NumSynthesisReverbs; i++) {
        SynthesisReverbs[i].ReverbGain -= SynthesisReverbs[i].ReverbGain / 4;
    }
}

s32 NasDelayDown(void) {
    s32 i;
    s32 j;

    switch (AudioResetStatus) {
        case 5:
            for (i = 0; i < SEQUENCE_PLAYERS; i++) {
                NasGetNewSubTrack(&SeqPlayer[i]);
            }
            AudioResetFadeOutFramesLeft = 4;
            AudioResetStatus--;
            break;
        case 4:
            if (AudioResetFadeOutFramesLeft != 0) {
                AudioResetFadeOutFramesLeft--;
                NasInitFilterCoef();
            } else {
                for (i = 0; i < MaxSimultaneousNotes; i++) {
                    if (Notes[i].NoteSubEu.Enabled && Notes[i].Adsr.State != ADSR_STATE_DISABLED) {
                        Notes[i].Adsr.FadeOutVel = AudioBufferParameters.UpdatesPerFrameInv;
                        Notes[i].Adsr.Action |= ADSR_ACTION_RELEASE;
                    }
                }
                AudioResetFadeOutFramesLeft = 0x00000010;
                AudioResetStatus--;
            }
            break;
        case 3:
            if (AudioResetFadeOutFramesLeft != 0) {
                AudioResetFadeOutFramesLeft--;
                NasInitFilterCoef();
            } else {
                for (i = 0; i < NUMAIBUFFERS; i++) {
                    for (j = 0; j < (s32) (AIBUFFER_LEN / sizeof(s16)); j++) {
                        AiBuffers[i][j] = 0;
                    }
                }
                AudioResetFadeOutFramesLeft = 4;
                AudioResetStatus--;
            }
            break;
        case 2:
            if (AudioResetFadeOutFramesLeft != 0) {
                AudioResetFadeOutFramesLeft--;
            } else {
                AudioResetStatus--;
            }
            break;
        case 1:
            NasSpecChange();
            AudioResetStatus = 0;
            break;
    }
    if (AudioResetStatus < 3) {
        return 0;
    }
    return 1;
}

void NasSpecChange(void) {
    s32 var_s1;
    s32 var_s5;
    s32 temp;
    u32 totalMem;
    u32 temporaryMem;
    u32 persistentMem;
    s16* mem;
    struct SynthesisReverb* reverb;
    struct ReverbSettingsEU* ReverbSettings;
    struct AudioSessionSettingsEU* temp_s6 = &AudioSessionPresets[AudioResetPresetIdToLoad];

    SampleDmaNumListItems = 0;
    AudioBufferParameters.Frequency = temp_s6->Frequency;
    AudioBufferParameters.AIFrequency = osAiSetFrequency(AudioBufferParameters.Frequency);
    AudioBufferParameters.SamplesPerFrameTarget = ALIGN16(AudioBufferParameters.Frequency / RefreshRate);
    AudioBufferParameters.MinAiBufferLength = AudioBufferParameters.SamplesPerFrameTarget - 0x10;
    AudioBufferParameters.MaxAiBufferLength = AudioBufferParameters.SamplesPerFrameTarget + 0x10;
    AudioBufferParameters.UpdatesPerFrame = ((AudioBufferParameters.SamplesPerFrameTarget + 0x10) / 192) + 1;
    AudioBufferParameters.SamplesPerUpdate =
        (AudioBufferParameters.SamplesPerFrameTarget / AudioBufferParameters.UpdatesPerFrame) & ~0x0007;
    AudioBufferParameters.SamplesPerUpdateMax = AudioBufferParameters.SamplesPerUpdate + 8;
    AudioBufferParameters.SamplesPerUpdateMin = AudioBufferParameters.SamplesPerUpdate - 8;
    AudioBufferParameters.ResampleRate = 32000.0f / ((f32) (s32) AudioBufferParameters.Frequency);
    AudioBufferParameters.UnkUpdatesPerFrameScaled = 0.001171875f / AudioBufferParameters.UpdatesPerFrame;
    AudioBufferParameters.UpdatesPerFrameInv = 1.0f / AudioBufferParameters.UpdatesPerFrame;
    MaxSimultaneousNotes = temp_s6->MaxSimultaneousNotes;
    Volume = temp_s6->Volume;
    TempoInternalToExternal =
        (u32) (((AudioBufferParameters.UpdatesPerFrame * 2880000.0f) / TatumsPerBeat) / D_803B7178);
    AudioBufferParameters.PresetUnk4 = temp_s6->Unk1;
    AudioBufferParameters.SamplesPerFrameTarget *= AudioBufferParameters.PresetUnk4;
    AudioBufferParameters.MaxAiBufferLength *= AudioBufferParameters.PresetUnk4;
    AudioBufferParameters.MinAiBufferLength *= AudioBufferParameters.PresetUnk4;
    AudioBufferParameters.UpdatesPerFrame *= AudioBufferParameters.PresetUnk4;
    MaxAudioCmds =
        (MaxSimultaneousNotes * 0x14 * AudioBufferParameters.UpdatesPerFrame) + (temp_s6->NumReverbs * 0x20) + 0x1E0;
    persistentMem = temp_s6->PersistentSeqMem + temp_s6->PersistentBankMem + temp_s6->unk_18;
    temporaryMem = temp_s6->TemporarySeqMem + temp_s6->TemporaryBankMem + temp_s6->unk_24;
    totalMem = persistentMem + temporaryMem;
    temp = (AudioSessionPool.Size - totalMem) - 0x100;
    SSessionPoolSplit.WantSeq = temp;
    SSessionPoolSplit.WantCustom = totalMem;
    NasSzHeapReset(&SSessionPoolSplit);
    SSeqAndBankPoolSplit.WantPersistent = persistentMem;
    SSeqAndBankPoolSplit.WantTemporary = temporaryMem;
    NasSzHeapDivide(&SSeqAndBankPoolSplit);
    SPersistentCommonPoolSplit.WantSeq = temp_s6->PersistentSeqMem;
    SPersistentCommonPoolSplit.WantBank = temp_s6->PersistentBankMem;
    SPersistentCommonPoolSplit.WantUnused = temp_s6->unk_18;
    NasSzDataDivide(&SPersistentCommonPoolSplit);
    STemporaryCommonPoolSplit.WantSeq = temp_s6->TemporarySeqMem;
    STemporaryCommonPoolSplit.WantBank = temp_s6->TemporaryBankMem;
    STemporaryCommonPoolSplit.WantUnused = temp_s6->unk_24;
    NasSzStayDivide(&STemporaryCommonPoolSplit);
    NasResetIDtable();
    Notes = NasHeapAlloc(&NotesAndBuffersPool, MaxSimultaneousNotes * sizeof(struct Note));
    NasChLookRelease();
    NasContinueWaveMemory();
    NoteSubsEu =
        NasHeapAlloc(&NotesAndBuffersPool, AudioBufferParameters.UpdatesPerFrame * MaxSimultaneousNotes * 0x10);
    for (var_s5 = 0; var_s5 != 2; var_s5++) {
        AudioCmdBuffers[var_s5] = NasHeapAlloc(&NotesAndBuffersPool, MaxAudioCmds * sizeof(u64));
    }
    for (var_s5 = 0; var_s5 < 4; var_s5++) {
        SynthesisReverbs[var_s5].useReverb = 0;
    }
    NumSynthesisReverbs = temp_s6->NumReverbs;
    for (var_s5 = 0; var_s5 < NumSynthesisReverbs; var_s5++) {
        reverb = &SynthesisReverbs[var_s5];
        ReverbSettings = &temp_s6->ReverbSettings[var_s5];
        reverb->WindowSize = ReverbSettings->WindowSize * 64;
        reverb->DownsampleRate = ReverbSettings->DownsampleRate;
        reverb->ReverbGain = ReverbSettings->Gain;
        reverb->useReverb = 8;
        reverb->RingBuffer.Left = NasHeapAlloc(&NotesAndBuffersPool, reverb->WindowSize * 2);
        reverb->RingBuffer.Right = NasHeapAlloc(&NotesAndBuffersPool, reverb->WindowSize * 2);
        reverb->nextRingBufferPos = 0;
        reverb->UnkC = 0;
        reverb->curFrame = 0;
        reverb->bufSizePerChannel = reverb->WindowSize;
        reverb->framesLeftToIgnore = 2;
        if (reverb->DownsampleRate != 1) {
            reverb->resampleFlags = 1;
            reverb->ResampleRate = 0x8000 / reverb->DownsampleRate;
            reverb->resampleStateLeft = NasHeapAlloc(&NotesAndBuffersPool, 0x00000020U);
            reverb->resampleStateRight = NasHeapAlloc(&NotesAndBuffersPool, 0x00000020U);
            reverb->unk24 = NasHeapAlloc(&NotesAndBuffersPool, 0x00000020U);
            reverb->unk28 = NasHeapAlloc(&NotesAndBuffersPool, 0x00000020U);
            for (var_s1 = 0; var_s1 < AudioBufferParameters.UpdatesPerFrame; var_s1++) {
                mem = NasHeapAlloc(&NotesAndBuffersPool, 0x00000300U);
                reverb->items[0][var_s1].ToDownsampleLeft = mem;
                reverb->items[0][var_s1].ToDownsampleRight = mem + (DEFAULT_LEN_1CH / sizeof(s16));
                mem = NasHeapAlloc(&NotesAndBuffersPool, 0x00000300U);
                reverb->items[1][var_s1].ToDownsampleLeft = mem;
                reverb->items[1][var_s1].ToDownsampleRight = mem + (DEFAULT_LEN_1CH / sizeof(s16));
            }
        }
    }
    NasWaveDmaCallBack(MaxSimultaneousNotes);
    osWritebackDCacheAll();
}

void* NasMemoryReconfig(s32 poolIdx, s32 id) {
    s32 i;

    for (i = 0; i < UnkPool1.Pool.NumAllocatedEntries; i++) {
        if (UnkPool1.Entries[i].PoolIndex == poolIdx && UnkPool1.Entries[i].id == id) {
            return UnkPool1.Entries[i].ptr;
        }
    }
    return NULL;
}

// SM64 does not appear to have a function
// comparable to this one, not a clue what
// this one is doing.
void EmemOnCheck(s32 poolIdx, s32 id) {
    ALSeqFile* sp3C;
    s32 temp_a2;
    u32 temp_a1;
    u8* var_a3;
    UNUSED u8* temp_v0;
    UNUSED s32 pad;

    switch (poolIdx) { /* irregular */
        case 0:
            sp3C = MUSTablePointer;
            break;
        case 1:
            sp3C = AlCtlHeader;
            break;
        case 2:
            sp3C = AlTbl;
            break;
    }
    if (sp3C->seqArray[id].len == 0) {
        id = (s32) sp3C->seqArray[id].offset;
    }
    if (NasMemoryReconfig(poolIdx, id) == NULL) {
        temp_a2 = UnkPool1.Pool.NumAllocatedEntries;
        temp_a1 = sp3C->seqArray[id].len;
        var_a3 = sp3C->seqArray[id].offset;
        if (poolIdx == 1) {
            var_a3 += 0x10;
        }
        UnkPool1.Entries[temp_a2].ptr = NasHeapAlloc(&UnkPool1.Pool, temp_a1);
        if (UnkPool1.Entries[temp_a2].ptr != NULL) {
            EmemLoad(var_a3, UnkPool1.Entries[temp_a2].ptr, temp_a1);
            UnkPool1.Entries[temp_a2].PoolIndex = poolIdx;
            UnkPool1.Entries[temp_a2].id = id;
            UnkPool1.Entries[temp_a2].Size = temp_a1;
            switch (poolIdx) { /* switch 1; irregular */
                case 0:        /* switch 1 */
                    if (SeqLoadStatus[id] != 5) {
                        SeqLoadStatus[id] = 5;
                    }
                    break;
                case 1: /* switch 1 */
                    SFXPointer[id].Instruments = (struct Instrument**) (UnkPool1.Entries[temp_a2].ptr + 4);
                    NasSmzBnkOfsToAdd(id);
                    if (BankLoadStatus[id] != 5) {
                        BankLoadStatus[id] = 5;
                    }
                    break;
                case 2: /* switch 1 */
                    break;
            }
        }
    }
}
