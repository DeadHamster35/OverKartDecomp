#include <ultra64.h>
#include <macros.h>
#include "audio/synthesis.h"
#include "audio/heap.h"
#include "audio/data.h"
#include "audio/load.h"
#include "audio/seqplayer.h"
#include "audio/internal.h"
// #include "audio/external.h"
#include "PR/abi.h"

#define aSetLoadBufferPair(pkt, c, off)                                               \
    aSetBuffer(pkt, 0, c + DMEM_ADDR_WET_LEFT_CH, 0, DEFAULT_LEN_1CH - c);            \
    aLoadBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.RingBuffer.Left + (off))); \
    aSetBuffer(pkt, 0, c + DMEM_ADDR_WET_RIGHT_CH, 0, DEFAULT_LEN_1CH - c);           \
    aLoadBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.RingBuffer.Right + (off)))

#define aSetSaveBufferPair(pkt, c, d, off)                                            \
    aSetBuffer(pkt, 0, 0, c + DMEM_ADDR_WET_LEFT_CH, d);                              \
    aSaveBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.RingBuffer.Left + (off))); \
    aSetBuffer(pkt, 0, 0, c + DMEM_ADDR_WET_RIGHT_CH, d);                             \
    aSaveBuffer(pkt, VIRTUAL_TO_PHYSICAL2(gSynthesisReverb.RingBuffer.Right + (off)));

struct VolumeChange {
    u16 sourceLeft;
    u16 sourceRight;
    u16 targetLeft;
    u16 targetRight;
};

u64* process_envelope(u64* cmd, struct NoteSubEu* NoteSubEu, struct NoteSynthesisState* SynthesisState, s32 nSamples,
                      u16 inBuf, s32 headsetPanSettings, u32 flags);

struct SynthesisReverb SynthesisReverbs[4];
u8 SAudioSynthesisPad[0x10];

// s16 Volume;
// s8 UseReverb;
// s8 NumSynthesisReverbs;
// struct NoteSubEu *NoteSubsEu;

// f32 gLeftVolRampings;
// f32 gRightVolRampings[3][1024];
// f32 *gCurrentLeftVolRamping; // Points to any of the three Left buffers above
// f32 *gCurrentRightVolRamping; // Points to any of the three Right buffers above

/**
 * Given that (almost) all of these are format strings, it is highly likely
 * that they are meant to be Used in some sort of printf Variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 **/
char SynthesisAudioString0[] = "Terminate-Canceled Channel %d,Phase %d\n";
char SynthesisAudioString1[] = "Copy %d\n";
char SynthesisAudioString2[] = "%d->%d\n";
char SynthesisAudioString3[] = "pitch %x: delaybytes %d : olddelay %d\n";
char SynthesisAudioString4[] = "cont %x: delaybytes %d : olddelay %d\n";

// Equivalent functionality as the US/JP version,
// just that the reverb structure is chosen from an array with Index
void NasCpuFX(s32 ChunkLen, u32 updateIndex, s32 reverbIndex) {
    struct ReverbRingBufferItem* Item;
    struct SynthesisReverb* reverb = &SynthesisReverbs[reverbIndex];
    s32 srcPos;
    s32 dstPos;
    s32 nSamples;
    s32 excessiveSamples;
    s32 UNUSED pad[3];
    if (reverb->DownsampleRate != 1) {
        if (reverb->framesLeftToIgnore == 0) {
            // Now that the RSP has Finished, downsample the Samples produced two frames ago by skipping
            // Samples.
            Item = &reverb->items[reverb->curFrame][updateIndex];

            // Touches both Left and Right since they are adjacent in memory
            osInvalDCache(Item->ToDownsampleLeft, 0x300);

            for (srcPos = 0, dstPos = 0; dstPos < Item->LengthA / 2; srcPos += reverb->DownsampleRate, dstPos++) {
                reverb->RingBuffer.Left[Item->StartPos + dstPos] = Item->ToDownsampleLeft[srcPos];
                reverb->RingBuffer.Right[Item->StartPos + dstPos] = Item->ToDownsampleRight[srcPos];
            }
            for (dstPos = 0; dstPos < Item->LengthB / 2; srcPos += reverb->DownsampleRate, dstPos++) {
                reverb->RingBuffer.Left[dstPos] = Item->ToDownsampleLeft[srcPos];
                reverb->RingBuffer.Right[dstPos] = Item->ToDownsampleRight[srcPos];
            }
        }
    }

    Item = &reverb->items[reverb->curFrame][updateIndex];
    nSamples = ChunkLen / reverb->DownsampleRate;
    excessiveSamples = (nSamples + reverb->nextRingBufferPos) - reverb->bufSizePerChannel;
    if (excessiveSamples < 0) {
        // There is space in the ring Buffer before it wraps around
        Item->LengthA = nSamples * 2;
        Item->LengthB = 0;
        Item->StartPos = (s32) reverb->nextRingBufferPos;
        reverb->nextRingBufferPos += nSamples;
    } else {
        // Ring Buffer wrapped around
        Item->LengthA = (nSamples - excessiveSamples) * 2;
        Item->LengthB = excessiveSamples * 2;
        Item->StartPos = reverb->nextRingBufferPos;
        reverb->nextRingBufferPos = excessiveSamples;
    }
    // These fields are never read later
    Item->NumSamplesAfterDownsampling = nSamples;
    Item->ChunkLen = ChunkLen;
}

Acmd* LoadAuxBuf(Acmd* acmd, u16 Addr, u16 srcOffset, s32 len, s32 reverbIndex) {
    aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(&SynthesisReverbs[reverbIndex].RingBuffer.Left[srcOffset]), Addr, len);
    aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(&SynthesisReverbs[reverbIndex].RingBuffer.Right[srcOffset]), Addr + 0x180,
                len);
    return acmd;
}

Acmd* SaveAuxBuf(Acmd* acmd, u16 Addr, u16 destOffset, s32 len, s32 reverbIndex) {
    aSaveBuffer(acmd++, Addr, VIRTUAL_TO_PHYSICAL2(&SynthesisReverbs[reverbIndex].RingBuffer.Left[destOffset]), len);
    aSaveBuffer(acmd++, Addr + 0x180,
                VIRTUAL_TO_PHYSICAL2(&SynthesisReverbs[reverbIndex].RingBuffer.Right[destOffset]), len);
    return acmd;
}

void NasWaveTerminateProcess(s32 updateIndexStart, s32 noteIndex) {
    s32 i;

    for (i = updateIndexStart + 1; i < AudioBufferParameters.UpdatesPerFrame; i++) {
        if (!NoteSubsEu[MaxSimultaneousNotes * i + noteIndex].needsInit) {
            NoteSubsEu[MaxSimultaneousNotes * i + noteIndex].Enabled = false;
        } else {
            break;
        }
    }
}

void NasPushDrvReg(s32 updateIndex) {
    struct NoteSubEu* src;
    struct NoteSubEu* dest;
    s32 i;

    for (i = 0; i < MaxSimultaneousNotes; i++) {
        src = &Notes[i].NoteSubEu;
        dest = &NoteSubsEu[MaxSimultaneousNotes * updateIndex + i];
        if (src->Enabled) {
            *dest = *src;
            src->needsInit = false;
        } else {
            dest->Enabled = false;
        }
    }
}

Acmd* NasSmzAudioFrame(Acmd* acmd, s32* writtenCmds, s16* aiBuf, s32 bufLen) {
    s32 i, j;
    u32* aiBufPtr;
    Acmd* cmd = acmd;
    s32 ChunkLen;

    for (i = AudioBufferParameters.UpdatesPerFrame; i > 0; i--) {
        NasProgramChanger(i - 1);
        NasPushDrvReg(AudioBufferParameters.UpdatesPerFrame - i);
    }
    aSegment(cmd++, 0, 0);
    aiBufPtr = (u32*) aiBuf;
    for (i = AudioBufferParameters.UpdatesPerFrame; i > 0; i--) {
        if (i == 1) {
            ChunkLen = bufLen;
        } else {
            if (bufLen / i >= AudioBufferParameters.SamplesPerUpdateMax) {
                ChunkLen = AudioBufferParameters.SamplesPerUpdateMax;
            } else if (bufLen / i <= AudioBufferParameters.SamplesPerUpdateMin) {
                ChunkLen = AudioBufferParameters.SamplesPerUpdateMin;
            } else {
                ChunkLen = AudioBufferParameters.SamplesPerUpdate;
            }
        }
        for (j = 0; j < NumSynthesisReverbs; j++) {
            if (SynthesisReverbs[j].useReverb != 0) {
                NasCpuFX(ChunkLen, AudioBufferParameters.UpdatesPerFrame - i, j);
            }
        }
        cmd = NasDriveRsp((s16*) aiBufPtr, ChunkLen, cmd, AudioBufferParameters.UpdatesPerFrame - i);
        bufLen -= ChunkLen;
        aiBufPtr += ChunkLen;
    }

    for (j = 0; j < NumSynthesisReverbs; j++) {
        if (SynthesisReverbs[j].framesLeftToIgnore != 0) {
            SynthesisReverbs[j].framesLeftToIgnore--;
        }
        SynthesisReverbs[j].curFrame ^= 1;
    }
    *writtenCmds = cmd - acmd;
    return cmd;
}

Acmd* NasLoadAuxBuffer(Acmd* acmd, s32 bufLen, s16 reverbIndex, s16 updateIndex) {
    struct ReverbRingBufferItem* Item;
    s16 startPad;
    s16 paddedLengthA;

    Item = &SynthesisReverbs[reverbIndex].items[SynthesisReverbs[reverbIndex].curFrame][updateIndex];
    aClearBuffer(acmd++, 0x840, 0x300);
    if (SynthesisReverbs[reverbIndex].DownsampleRate == 1) {

        acmd = LoadAuxBuf(acmd, 0x840, Item->StartPos, Item->LengthA, reverbIndex);
        if (Item->LengthB != 0) {
            acmd = LoadAuxBuf(acmd, Item->LengthA + 0x840, 0U, Item->LengthB, reverbIndex);
        }

        aMix(acmd++, 0x7fff, 0x840, 0x540, 0x300);
        aMix(acmd++, 0x8000 + SynthesisReverbs[reverbIndex].ReverbGain, 0x840, 0x840, 0x300);
    } else {
        startPad = (Item->StartPos % 8U) * 2;
        paddedLengthA = ALIGN(startPad + Item->LengthA, 4);

        acmd =
            LoadAuxBuf(acmd, 0x0020, Item->StartPos - (startPad / 2), 0x00000180, reverbIndex);
        if (Item->LengthB != 0) {
            acmd = LoadAuxBuf(acmd, paddedLengthA + 0x20, 0, 0x180 - paddedLengthA, reverbIndex);
        }

        aSetBuffer(acmd++, 0, 0x20 + startPad, 0x840, bufLen * 2);
        aResample(acmd++, SynthesisReverbs[reverbIndex].resampleFlags, SynthesisReverbs[reverbIndex].ResampleRate,
                  VIRTUAL_TO_PHYSICAL2(SynthesisReverbs[reverbIndex].resampleStateLeft));
        aSetBuffer(acmd++, 0, 0x1A0 + startPad, 0x9C0, bufLen * 2);
        aResample(acmd++, SynthesisReverbs[reverbIndex].resampleFlags, SynthesisReverbs[reverbIndex].ResampleRate,
                  VIRTUAL_TO_PHYSICAL2(SynthesisReverbs[reverbIndex].resampleStateRight));
        aMix(acmd++, 0x7fff, 0x840, 0x540, 0x300);
        aMix(acmd++, 0x8000 + SynthesisReverbs[reverbIndex].ReverbGain, 0x840, 0x840, 0x300);
    }
    return acmd;
}

Acmd* NasSaveAuxBuffer(Acmd* acmd, s16 reverbIndex, s16 updateIndex) {
    struct ReverbRingBufferItem* Item;

    Item = &SynthesisReverbs[reverbIndex].items[SynthesisReverbs[reverbIndex].curFrame][updateIndex];
    if (SynthesisReverbs[reverbIndex].useReverb != 0) {
        switch (SynthesisReverbs[reverbIndex].DownsampleRate) {
            case 1:
                acmd = SaveAuxBuf(acmd, 0x840, Item->StartPos, Item->LengthA, reverbIndex);
                if (Item->LengthB != 0) {
                    acmd =
                        SaveAuxBuf(acmd, 0x840 + Item->LengthA, 0, Item->LengthB, reverbIndex);
                }
                break;
            default:
                aSaveBuffer(acmd++, 0x840,
                            VIRTUAL_TO_PHYSICAL2(SynthesisReverbs[reverbIndex]
                                                     .items[SynthesisReverbs[reverbIndex].curFrame][updateIndex]
                                                     .ToDownsampleLeft),
                            0x300);
                SynthesisReverbs[reverbIndex].resampleFlags = 0;
                break;
        }
    }
    return acmd;
}

Acmd* NasDriveRsp(s16* aiBuf, s32 bufLen, Acmd* acmd, s32 updateIndex) {
    struct NoteSubEu* NoteSubEu;
    u8 noteIndices[56];
    s32 temp;
    s32 i;
    s16 j;
    s16 notePos = 0;

    if (NumSynthesisReverbs == 0) {
        for (i = 0; i < MaxSimultaneousNotes; i++) {
            if (NoteSubsEu[MaxSimultaneousNotes * updateIndex + i].Enabled) {
                noteIndices[notePos++] = i;
            }
        }
    } else {
        for (j = 0; j < NumSynthesisReverbs; j++) {
            for (i = 0; i < MaxSimultaneousNotes; i++) {
                NoteSubEu = &NoteSubsEu[MaxSimultaneousNotes * updateIndex + i];
                if (NoteSubEu->Enabled && j == NoteSubEu->reverbIndex) {
                    noteIndices[notePos++] = i;
                }
            }
        }

        for (i = 0; i < MaxSimultaneousNotes; i++) {
            NoteSubEu = &NoteSubsEu[MaxSimultaneousNotes * updateIndex + i];
            if (NoteSubEu->Enabled && NoteSubEu->reverbIndex >= NumSynthesisReverbs) {
                noteIndices[notePos++] = i;
            }
        }
    }
    aClearBuffer(acmd++, DMEM_ADDR_LEFT_CH, DEFAULT_LEN_2CH);
    i = 0;
    for (j = 0; j < NumSynthesisReverbs; j++) {
        UseReverb = SynthesisReverbs[j].useReverb;
        if (UseReverb != 0) {
            acmd = NasLoadAuxBuffer(acmd, bufLen, j, updateIndex);
        }
        for (; i < notePos; i++) {
            temp = updateIndex * MaxSimultaneousNotes;
            if (j == NoteSubsEu[temp + noteIndices[i]].reverbIndex) {
                acmd = NasSynthMain(noteIndices[i], &NoteSubsEu[temp + noteIndices[i]],
                                              &Notes[noteIndices[i]].SynthesisState, aiBuf, bufLen, acmd, updateIndex);
                continue;
            } else {
                break;
            }
        }
        if (SynthesisReverbs[j].useReverb != 0) {
            acmd = NasSaveAuxBuffer(acmd, j, updateIndex);
        }
    }
    for (; i < notePos; i++) {
        temp = updateIndex * MaxSimultaneousNotes;
        if (IS_BANK_LOAD_COMPLETE(NoteSubsEu[temp + noteIndices[i]].BankId) == true) {
            acmd = NasSynthMain(noteIndices[i], &NoteSubsEu[temp + noteIndices[i]],
                                          &Notes[noteIndices[i]].SynthesisState, aiBuf, bufLen, acmd, updateIndex);
        } else {
            AudioErrorFlags = (NoteSubsEu[temp + noteIndices[i]].BankId + (i << 8)) + 0x10000000;
        }
    }

    temp = bufLen * 2;
    aSetBuffer(acmd++, 0, 0, DMEM_ADDR_TEMP, temp);
    aInterleave(acmd++, DMEM_ADDR_LEFT_CH, DMEM_ADDR_RIGHT_CH);
    aSaveBuffer(acmd++, DMEM_ADDR_TEMP, VIRTUAL_TO_PHYSICAL2(aiBuf), temp * 2);
    return acmd;
}

Acmd* NasSynthMain(s32 noteIndex, struct NoteSubEu* NoteSubEu, struct NoteSynthesisState* SynthesisState,
                             UNUSED s16* aiBuf, s32 inBuf, Acmd* cmd, s32 updateIndex) {
    s32 pad[3];
    struct AudioBankSample *audioBookSample; // sp130
    struct AdpcmLoop *loopInfo; // sp12C
    s16 *curLoadedBook; // sp128
    s32 pad4;
    s32 nSamplesToLoad;
    s32 noteFinished; // sp11C
    s32 Restart; // sp118
    s32 flags;
    u16 resamplingRateFixedPoint; // sp112
    s32 Pad2[1];
    u16 headsetPanRight;
    s32 loopInfo_2;
    s32 a1;
    s32 spFC; // spFC
    s32 Pad3;
    s32 nAdpcmSamplesProcessed;
    s32 s4;
    u8 *SampleAddr; // spEC
    s32 s3;
    s32 samplesLenAdjusted; // spE4
    s32 leftRight;
    s32 endPos; // spDC
    s32 nSamplesToProcess; // spD8
    u32 samplesLenFixedPoint;
    s32 var_s6;
    s32 nSamplesInThisIteration;
    u32 var_t2;
    u8 *var_a0_2;
    s32 s5Aligned;
    s32 temp_t6;
    s32 aligned;
    struct AudioBankSample *bankSample;
    s32 nParts; // spB0
    s32 curPart; // spAC
    s32 pad5;
    s16 Addr;
    s32 resampledTempLen; // spA0
    u16 noteSamplesDmemAddrBeforeResampling;
    s32 samplesRemaining;
    s32 s1;
    u32 nEntries;
    struct Note *Note;

    curLoadedBook = NULL;
    Note = &Notes[noteIndex];
    flags = 0;
    if (NoteSubEu->needsInit == true) {
        flags = A_INIT;
        SynthesisState->Restart = 0;
        SynthesisState->SamplePosInt = 0;
        SynthesisState->SamplePosFrac = 0;
        SynthesisState->CurVolLeft = 0;
        SynthesisState->CurVolRight = 0;
        SynthesisState->PrevHeadsetPanRight = 0;
        SynthesisState->PrevHeadsetPanLeft = 0;
    }
    resamplingRateFixedPoint = NoteSubEu->resamplingRateFixedPoint;

    nParts = NoteSubEu->hasTwoAdpcmParts + 1;
    samplesLenFixedPoint = ((resamplingRateFixedPoint * inBuf) * 2) + SynthesisState->SamplePosFrac;
    nSamplesToLoad = samplesLenFixedPoint >> 0x10;
    SynthesisState->SamplePosFrac = samplesLenFixedPoint & 0xFFFF;

    if (NoteSubEu->isSyntheticWave) {
        cmd = NasSynthWaveMemory(cmd, NoteSubEu, SynthesisState, nSamplesToLoad);

        noteSamplesDmemAddrBeforeResampling = (SynthesisState->SamplePosInt * 2) + 0x1A0; // DMEM_ADDR_UNCOMPRESSED_NOTE
        SynthesisState->SamplePosInt += nSamplesToLoad;
    } else {
        audioBookSample = NoteSubEu->Sound.AudioBankSound->Sample;
        loopInfo = audioBookSample->Loop;
        endPos = loopInfo->end;
        SampleAddr = audioBookSample->SampleAddr;
        resampledTempLen = 0;

        for (curPart = 0; curPart < nParts; curPart++) {
            bankSample = audioBookSample;
            nAdpcmSamplesProcessed = 0;
            s4 = 0;

            if (nParts == 1) {
                samplesLenAdjusted = nSamplesToLoad;
            } else if (nSamplesToLoad & 1) {
                samplesLenAdjusted = (nSamplesToLoad & (~1)) + (curPart * 2);
            } else {
                samplesLenAdjusted = nSamplesToLoad;
            }
            if (curLoadedBook != (*bankSample->Book).Book) {
                curLoadedBook = bankSample->Book->Book;
                nEntries = (16 * bankSample->Book->Order) * bankSample->Book->Npredictors;
                aLoadADPCM(cmd++, nEntries, VIRTUAL_TO_PHYSICAL2(NoteSubEu->bookOffset+curLoadedBook));
            }
            if (NoteSubEu->bookOffset != 0) {
                curLoadedBook = &UnknownData800F6290[0];
            }
            while (nAdpcmSamplesProcessed != samplesLenAdjusted) {
                noteFinished = false;
                Restart = false;

                s3 = SynthesisState->SamplePosInt & 0xF;
                samplesRemaining = endPos - SynthesisState->SamplePosInt;
                nSamplesToProcess = samplesLenAdjusted - nAdpcmSamplesProcessed;
                if ((s3 == 0) && (SynthesisState->Restart == false)) {
                    s3 = 16;
                }

                a1 = 16 - s3;

                if (nSamplesToProcess < samplesRemaining) {
                    loopInfo_2 = ((nSamplesToProcess - a1) + 0xF) / 16;
                    s1 = loopInfo_2 * 16;
                    var_s6 = (a1 + s1) - nSamplesToProcess;
                } else {
                    s1 = samplesRemaining - a1;
                    var_s6 = 0;
                    if (s1 <= 0) {
                        s1 = 0;
                        a1 = samplesRemaining;
                    }
                    loopInfo_2 = (s1 + 0xF) / 16;
                    if (loopInfo->Count != 0) {
                        Restart = 1;
                    } else {
                        noteFinished = 1;
                    }
                }

                if (loopInfo_2 != 0) {
                    temp_t6 = ((SynthesisState->SamplePosInt - s3) + 16) / 16; // diff from sm64 sh
                    if (bankSample->Loaded == 0x81) {
                        var_a0_2 = (temp_t6 * 9) + SampleAddr;
                    } else {
                        var_a0_2 =
                            NasWaveDmaFrameWork((uintptr_t) (temp_t6 * 9) + SampleAddr, ALIGN(((loopInfo_2 * 9) + 16), 4),
                                            flags, &SynthesisState->SampleDmaIndex);
                    }

                    var_t2 = ((uintptr_t) var_a0_2) & 0xF;
                    aligned = ALIGN(((loopInfo_2 * 9) + 16), 4);
                    Addr = (0x540 - aligned); // DMEM_ADDR_COMPRESSED_ADPCM_DATA
                    aLoadBuffer(cmd++, VIRTUAL_TO_PHYSICAL2(var_a0_2 - var_t2), Addr, aligned);
                } else {
                    s1 = 0; // ?
                    var_t2 = 0;
                }

                if (SynthesisState->Restart != false) {
                    aSetLoop(cmd++, VIRTUAL_TO_PHYSICAL2(bankSample->Loop->State));

                    flags = A_LOOP;
                    SynthesisState->Restart = false;
                }
                nSamplesInThisIteration = (s1 + a1) - var_s6;
                s5Aligned = ALIGN(s4 + 16, 4);
                if (nAdpcmSamplesProcessed == 0) {

                    aligned = ALIGN(((loopInfo_2 * 9) + 16), 4);
                    Addr = (0x540 - aligned);
                    aSetBuffer(cmd++, 0, Addr + var_t2, 0x1A0, s1 * 2);
                    aADPCMdec(cmd++, flags, VIRTUAL_TO_PHYSICAL2(SynthesisState->SynthesisBuffers->AdpcmdecState));
                    spFC = s3 * 2;
                } else {
                    aligned = ALIGN(((loopInfo_2 * 9) + 16), 4);
                    Addr = (0x540 - aligned);
                    aSetBuffer(cmd++, 0, Addr + var_t2, 0x1A0 + s5Aligned, s1 * 2);
                    aADPCMdec(cmd++, flags, VIRTUAL_TO_PHYSICAL2(SynthesisState->SynthesisBuffers->AdpcmdecState));
                    aDMEMMove(cmd++, 0x1A0 + s5Aligned + (s3 * 2), 0x1A0 + s4, nSamplesInThisIteration * 2);
                }

                nAdpcmSamplesProcessed += nSamplesInThisIteration;
                switch (flags) {
                    case 1:
                        spFC = 0x20;
                        s4 = (s1 * 2) + 0x20;
                        break;
                    case 2:
                        s4 = (nSamplesInThisIteration * 2) + s4;
                        break;
                    default:
                        if (s4 != 0) {
                            s4 = (nSamplesInThisIteration * 2) + s4;
                        } else {
                            s4 = (s3 + nSamplesInThisIteration) * 2;
                        }
                        break;
                }

                flags = 0;
                if (noteFinished) {
                    aClearBuffer(cmd++, 0x1A0 + s4, (samplesLenAdjusted - nAdpcmSamplesProcessed) * 2);
                    NoteSubEu->Finished = 1;
                    Note->NoteSubEu.Finished = 1;
                    Note->NoteSubEu.Enabled = 0;
                    NasWaveTerminateProcess(updateIndex, noteIndex);
                    break;
                }

                if (Restart) {
                    SynthesisState->Restart = true;
                    SynthesisState->SamplePosInt = loopInfo->Start;
                } else {
                    SynthesisState->SamplePosInt += nSamplesToProcess;
                }
            }

            switch (nParts) {
                case 1:
                    noteSamplesDmemAddrBeforeResampling = 0x1A0 + spFC;
                    break;
                case 2:
                    switch (curPart) {
                        case 0:
                            aDownsampleHalf(cmd++, ALIGN(samplesLenAdjusted / 2, 3), 0x1A0 + spFC, DMEM_ADDR_RESAMPLED);
                            resampledTempLen = samplesLenAdjusted;
                            noteSamplesDmemAddrBeforeResampling = DMEM_ADDR_RESAMPLED;
                            if (NoteSubEu->Finished != false) {
                                aClearBuffer(cmd++, noteSamplesDmemAddrBeforeResampling + resampledTempLen,
                                             samplesLenAdjusted + 0x10);
                            }
                            break;
                        case 1:
                            aDownsampleHalf(cmd++, ALIGN(samplesLenAdjusted / 2, 3), DMEM_ADDR_RESAMPLED2 + spFC,
                                            resampledTempLen + DMEM_ADDR_RESAMPLED);
                            break;
                    }
            }
            if (NoteSubEu->Finished != false) {
                break;
            }
        }
    }
    flags = 0;
    if (NoteSubEu->needsInit == true) {
        flags = A_INIT;
        NoteSubEu->needsInit = false;
    }

    cmd = NasSynthResample(cmd, SynthesisState, inBuf * 2, resamplingRateFixedPoint, noteSamplesDmemAddrBeforeResampling, flags);
    headsetPanRight = NoteSubEu->headsetPanRight;
    if ((headsetPanRight & 0xFFFF) || SynthesisState->PrevHeadsetPanRight) {
        leftRight = 1;
    } else if (NoteSubEu->headsetPanLeft || SynthesisState->PrevHeadsetPanLeft) {
        leftRight = 2;
    } else {
        leftRight = 0;
    }
    cmd = NasSynthEnvelope(cmd, NoteSubEu, SynthesisState, inBuf, 0, leftRight, flags);
    if (NoteSubEu->UsesHeadsetPanEffects) {
        cmd = NasSynthDelay(cmd, NoteSubEu, SynthesisState, inBuf * 2, flags, leftRight);
    }
    return cmd;
}

Acmd* NasSynthWaveMemory(Acmd* acmd, struct NoteSubEu* NoteSubEu, struct NoteSynthesisState* SynthesisState,
                        s32 nSamplesToLoad) {
    s32 a3;
    s32 repeats;
    aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(NoteSubEu->Sound.Samples), 0x1A0, 128);

    SynthesisState->SamplePosInt &= 0x3f;
    a3 = 64 - SynthesisState->SamplePosInt;
    if (a3 < nSamplesToLoad) {
        repeats = (nSamplesToLoad - a3 + 63) / 64;
        if (repeats != 0) {
            aDMEMMove2(acmd++, repeats, 0x1A0, 0x1A0 + 128, 128);
        }
    }
    return acmd;
}

Acmd* NasSynthResample(Acmd* acmd, struct NoteSynthesisState* SynthesisState, s32 Count, u16 Pitch, u16 dmemIn,
                     u32 flags) {
    aSetBuffer(acmd++, /*flags*/ 0, dmemIn, /*dmemout*/ 0, Count);
    aResample(acmd++, flags, Pitch, VIRTUAL_TO_PHYSICAL2(SynthesisState->SynthesisBuffers->FinalResampleState));
    return acmd;
}

Acmd* NasSynthEnvelope(Acmd* cmd, struct NoteSubEu* Note, struct NoteSynthesisState* SynthesisState, s32 nSamples,
                    u16 inBuf, s32 headsetPanSettings, UNUSED u32 flags) {
    UNUSED s32 pad[2];
    u16 sourceRight;
    u16 sourceLeft;
    u16 targetLeft;
    u16 targetRight;
    s16 rampLeft;
    s16 rampRight;

    sourceLeft = SynthesisState->CurVolLeft;
    sourceRight = SynthesisState->CurVolRight;
    
    targetLeft = (Note->targetVolLeft) << 4;
    targetRight = (Note->targetVolRight) << 4;

    rampLeft  = ((targetLeft  - sourceLeft)  / (nSamples >> 3));
    rampRight = ((targetRight - sourceRight) / (nSamples >> 3));
    targetLeft  = sourceLeft  + rampLeft  * (nSamples >> 3);
    targetRight = sourceRight + rampRight * (nSamples >> 3);

    SynthesisState->CurVolLeft = targetLeft;
    SynthesisState->CurVolRight = targetRight;

    if (Note->UsesHeadsetPanEffects) {
        aClearBuffer(cmd++, DMEM_ADDR_NOTE_PAN_TEMP, DEFAULT_LEN_1CH);
        aEnvSetup1Alt(cmd++, Note->ReverbVol, sourceLeft, sourceRight, (u32)rampLeft, (u32)rampRight);
        aEnvSetup2(cmd++, sourceLeft, sourceRight);

        switch (headsetPanSettings) {;
            case 1:
                aEnvMixer(cmd++,
                    inBuf, nSamples,
                    0,
                    Note->stereoStrongRight, Note->stereoStrongLeft,
                    DMEM_ADDR_NOTE_PAN_TEMP,
                    DMEM_ADDR_RIGHT_CH,
                    DMEM_ADDR_WET_LEFT_CH,
                    DMEM_ADDR_WET_RIGHT_CH);
                break;
            case 2:
                aEnvMixer(cmd++,
                    inBuf, nSamples,
                    0,
                    Note->stereoStrongRight, Note->stereoStrongLeft,
                    DMEM_ADDR_LEFT_CH,
                    DMEM_ADDR_NOTE_PAN_TEMP,
                    DMEM_ADDR_WET_LEFT_CH,
                    DMEM_ADDR_WET_RIGHT_CH);
                break;
            default:
                aEnvMixer(cmd++,
                    inBuf, nSamples,
                    0,
                    Note->stereoStrongRight, Note->stereoStrongLeft,
                    DMEM_ADDR_LEFT_CH,
                    DMEM_ADDR_RIGHT_CH,
                    DMEM_ADDR_WET_LEFT_CH,
                    DMEM_ADDR_WET_RIGHT_CH);
                break;
        }
    } else {
        aEnvSetup1Alt(cmd++, Note->ReverbVol, sourceLeft, sourceRight, (u32)rampLeft, (u32)rampRight);
        aEnvSetup2(cmd++, sourceLeft, sourceRight);
        aEnvMixer(cmd++,
                inBuf, nSamples,
                0,
                Note->stereoStrongRight, Note->stereoStrongLeft,
                DMEM_ADDR_LEFT_CH,
                DMEM_ADDR_RIGHT_CH,
                DMEM_ADDR_WET_LEFT_CH,
                DMEM_ADDR_WET_RIGHT_CH);
    }
    return cmd;
}

Acmd* NasSynthDelay(Acmd* acmd, struct NoteSubEu* NoteSubEu, struct NoteSynthesisState* Note,
                                     s32 bufLen, s32 flags, s32 leftRight) {
    u16 dest;
    u16 Pitch;
    u8 prevPanShift;
    u8 panShift;
    UNUSED u8 unkDebug;

    switch (leftRight) {
        case 1:
            dest = 0x540;
            panShift = NoteSubEu->headsetPanRight;
            Note->PrevHeadsetPanLeft = 0;
            prevPanShift = Note->PrevHeadsetPanRight;
            Note->PrevHeadsetPanRight = panShift;
            break;
        case 2:
            dest = 0x6C0;
            panShift = NoteSubEu->headsetPanLeft;
            Note->PrevHeadsetPanRight = 0;

            prevPanShift = Note->PrevHeadsetPanLeft;
            Note->PrevHeadsetPanLeft = panShift;
            break;
        default:
            return acmd;
    }

    if (flags != 1) { // A_INIT?
        // Slightly adjust the Sample Rate in Order to fit a change in pan shift
        if (prevPanShift == 0) {
            // Kind of a hack that moves the first Samples into the resample State
            aDMEMMove(acmd++, 0x0200, 0x0000, 8);
            aClearBuffer(acmd++, 8, 8); // Set Pitch accumulator to 0 in the resample State
            aDMEMMove(acmd++, 0x0200, 0x0000 + 0x10,
                      0x10); // No idea, result seems to be overwritten later

            aSaveBuffer(acmd++, 0x0000, VIRTUAL_TO_PHYSICAL2(Note->SynthesisBuffers->PanResampleState),
                        sizeof(Note->SynthesisBuffers->PanResampleState));

            Pitch = (bufLen << 0xf) / (bufLen + panShift - prevPanShift + 8);
            if (Pitch) {}
            aSetBuffer(acmd++, 0, 0x0200 + 8, 0x0000, panShift + bufLen - prevPanShift);
            aResample(acmd++, 0, Pitch, VIRTUAL_TO_PHYSICAL2(Note->SynthesisBuffers->PanResampleState));
        } else {
            if (panShift == 0) {
                Pitch = (bufLen << 0xf) / (bufLen - prevPanShift - 4);
            } else {
                Pitch = (bufLen << 0xf) / (bufLen + panShift - prevPanShift);
            }

            if (1) {}

            aSetBuffer(acmd++, 0, 0x0200, 0x0000, bufLen + panShift - prevPanShift);
            aResample(acmd++, 0, Pitch, VIRTUAL_TO_PHYSICAL2(Note->SynthesisBuffers->PanResampleState));
        }

        if (prevPanShift != 0) {
            aLoadBuffer(acmd++, VIRTUAL_TO_PHYSICAL2(Note->SynthesisBuffers->PanSamplesBuffer), 0x0200, prevPanShift);
            aDMEMMove(acmd++, 0x0000, 0x0200 + prevPanShift, panShift + bufLen - prevPanShift);
        } else {
            aDMEMMove(acmd++, 0x0000, 0x0200, panShift + bufLen - prevPanShift);
        }
    } else {
        // Just shift Right
        aDMEMMove(acmd++, 0x0200, 0x0000, bufLen);
        aDMEMMove(acmd++, 0x0000, 0x0200 + panShift, bufLen);
        aClearBuffer(acmd++, 0x0200, panShift);
    }

    if (panShift) {
        // Save excessive Samples for Next iteration
        aSaveBuffer(acmd++, 0x0200 + bufLen, VIRTUAL_TO_PHYSICAL2(Note->SynthesisBuffers->PanSamplesBuffer), panShift);
    }

    aMix(acmd++, /*Gain*/ 0x7FFF, /*in*/ 0x0200, /*out*/ dest, ALIGN(bufLen, 5));

    return acmd;
}
