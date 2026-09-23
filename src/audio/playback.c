#include <ultra64.h>
#include <macros.h>

#include "audio/internal.h"
#include "audio/playback.h"
#include "audio/load.h"
#include "audio/heap.h"
#include "audio/external.h"
#include "audio/effects.h"
#include "audio/data.h"
#include "audio/seqplayer.h"

void NasSmzSetVol(struct Note* Note, f32 Velocity, u8 pan, u8 ReverbVol) {
    struct NoteSubEu* sub = &Note->NoteSubEu;
    f32 volRight, volLeft;
    u8 StrongRight;
    u8 StrongLeft;
    s32 smallPanIndex;
    u16 unkMask = ~0x80;

    pan &= unkMask;

    if (Note->NoteSubEu.StereoHeadsetEffects && AudioLibSoundMode == SOUND_MODE_HEADSET) {
        smallPanIndex = pan >> 3;
        if (smallPanIndex >= ARRAY_COUNT(HeadsetPanQuantization)) {
            smallPanIndex = ARRAY_COUNT(HeadsetPanQuantization) - 1;
        }

        sub->headsetPanLeft = HeadsetPanQuantization[smallPanIndex];
        sub->headsetPanRight = HeadsetPanQuantization[ARRAY_COUNT(HeadsetPanQuantization) - 1 - smallPanIndex];
        sub->stereoStrongRight = false;
        sub->stereoStrongLeft = false;
        sub->UsesHeadsetPanEffects = true;

        volLeft = HeadsetPanVolume[pan];
        volRight = HeadsetPanVolume[127 - pan];
    } else if (sub->StereoHeadsetEffects && AudioLibSoundMode == SOUND_MODE_STEREO) {
        StrongRight = false;
        StrongLeft = false;
        sub->headsetPanRight = 0;
        sub->headsetPanLeft = 0;

        sub->UsesHeadsetPanEffects = false;

        volLeft = StereoPanVolume[pan];
        volRight = StereoPanVolume[127 - pan];
        if (pan < 0x20) {
            StrongLeft = true;
        } else if (pan > 0x60) {
            StrongRight = true;
        }

        sub->stereoStrongRight = StrongRight;
        sub->stereoStrongLeft = StrongLeft;

    } else if (AudioLibSoundMode == SOUND_MODE_MONO) {
        volLeft = 0.707f;
        volRight = 0.707f;
    } else {
        volLeft = DefaultPanVolume[pan];
        volRight = DefaultPanVolume[127 - pan];
    }

    if (Velocity < 0.0f) {
        stubbed_printf("Audio: setvol: volume minus %f\n", Velocity);
        Velocity = 0.0f;
    }
    if (Velocity > 1.0f) {
        stubbed_printf("Audio: setvol: volume overflow %f\n", Velocity);
        Velocity = 1.0f;
    }

    sub->targetVolLeft = ((s32) (Velocity * volLeft * 4095.999f));
    sub->targetVolRight = ((s32) (Velocity * volRight * 4095.999f));

    //! @bug for the change to UQ0.7, the if statement should also have been changed accordingly
    if (sub->ReverbVol != ReverbVol) {
        sub->ReverbVol = ReverbVol;
        sub->envMixerNeedsInit = true;
        return;
    }

    if (sub->needsInit) {
        sub->envMixerNeedsInit = true;
    } else {
        sub->envMixerNeedsInit = false;
    }
}

void NasSmzSetPitch(struct Note* Note, f32 resamplingRateInput) {
    f32 resamplingRate = 0.0f;
    struct NoteSubEu* tempSub = &Note->NoteSubEu;

    if (resamplingRateInput < 0.0f) {
        stubbed_printf("Audio: setpitch: pitch minus %f\n", resamplingRateInput);
        resamplingRateInput = 0.0f;
    }
    if (resamplingRateInput < 2.0f) {
        tempSub->hasTwoAdpcmParts = 0;

        if (1.9999599f < resamplingRateInput) {
            resamplingRate = 1.9999599f;
        } else {
            resamplingRate = resamplingRateInput;
        }

    } else {
        tempSub->hasTwoAdpcmParts = 1;
        if (2 * 1.9999599f < resamplingRateInput) {
            resamplingRate = 1.9999599f;
        } else {
            resamplingRate = resamplingRateInput * 0.5f;
        }
    }
    Note->NoteSubEu.resamplingRateFixedPoint = (s32) (resamplingRate * 32768.0f);
}

struct AudioBankSound* NoteToVoice(struct Instrument* Instrument, s32 semitone) {
    struct AudioBankSound* Sound;
    if (semitone < Instrument->NormalRangeLo) {
        Sound = &Instrument->LowNotesSound;
    } else if (semitone <= Instrument->NormalRangeHi) {
        Sound = &Instrument->NormalNotesSound;
    } else {
        Sound = &Instrument->HighNotesSound;
    }
    return Sound;
}

struct Instrument* ProgToVp(s32 BankId, s32 instId) {
    struct Instrument* inst;

    if (IS_BANK_LOAD_COMPLETE(BankId) == false) {
        stubbed_printf("Audio: voiceman: No bank error %d\n", BankId);
        AudioErrorFlags = BankId + 0x10000000;
        return NULL;
    }

    if (instId >= SFXPointer[BankId].NumInstruments) {
        stubbed_printf("Audio: voiceman: progNo. overflow %d,%d\n", instId, SFXPointer[BankId].NumInstruments);
        AudioErrorFlags = ((BankId << 8) + instId) + 0x3000000;
        return NULL;
    }

    inst = SFXPointer[BankId].Instruments[instId];
    if (inst == NULL) {
        stubbed_printf("Audio: voiceman: progNo. undefined %d,%d\n", BankId, instId);
        AudioErrorFlags = ((BankId << 8) + instId) + 0x1000000;
        return inst;
    }
    return inst;
}

struct Drum* PercToPp(s32 BankId, s32 drumId) {
    struct Drum* drum;

    if (IS_BANK_LOAD_COMPLETE(BankId) == false) {
        stubbed_printf("Audio: voiceman: No bank error %d\n", BankId);
        AudioErrorFlags = BankId + 0x10000000;
        return NULL;
    }

    if (drumId >= SFXPointer[BankId].NumDrums) {
        stubbed_printf("Audio: voiceman: Percussion Overflow %d,%d\n", drumId, SFXPointer[BankId].NumDrums);
        AudioErrorFlags = ((BankId << 8) + drumId) + 0x4000000;
        return NULL;
    }

    if ((uintptr_t) SFXPointer[BankId].Drums < 0x80000000U) {
        stubbed_printf("Audio: voiceman: Percussion table pointer (bank %d) is irregular.\n");
        return NULL;
    }

    drum = SFXPointer[BankId].Drums[drumId];
    if (drum == NULL) {
        stubbed_printf("Audio: voiceman: Percpointer NULL %d,%d\n", BankId, drumId);
        AudioErrorFlags = ((BankId << 8) + drumId) + 0x5000000;
    }
    // Don't know where these actually belong, putting them here because why not
    // Really, we just need these so the format strings could put into the .data section
    stubbed_printf("--4 %x\n", AudioErrorFlags);
    stubbed_printf("Stoped Voice\n");
    return drum;
}

void NasStartVoice(struct Note* Note) {
    if (Note->ParentLayer->Adsr.ReleaseRate == 0) {
        NasModulator(&Note->Adsr, Note->ParentLayer->SEQChannel->Adsr.Envelope, &Note->AdsrVolScale);
    } else {
        NasModulator(&Note->Adsr, Note->ParentLayer->Adsr.Envelope, &Note->AdsrVolScale);
    }
    Note->Adsr.State = ADSR_STATE_INITIAL;
    Note->NoteSubEu = DefaultNoteSub;
}

void NasStopVoice(struct Note* Note) {
    if (Note->NoteSubEu.needsInit == true) {
        Note->NoteSubEu.needsInit = false;
    } else {
        NasSmzSetVol(Note, 0, 0x40, 0);
    }
    Note->Priority = NOTE_PRIORITY_DISABLED;
    Note->ParentLayer = NO_LAYER;
    Note->PrevParentLayer = NO_LAYER;
    Note->NoteSubEu.Enabled = false;
    Note->NoteSubEu.Finished = false;
}

void NasUpdateChannel(void) {
    f32 Scale;
    f32 Frequency;
    f32 Velocity;
    struct Note* Note;
    struct NotePlaybackState* playbackState;
    struct NoteSubEu* NoteSubEu;
    UNUSED u8 pad[12];
    u8 ReverbVol;
    UNUSED u8 Pad3;
    u8 pan;
    u8 bookOffset;
    struct NoteAttributes* Attributes;
    s32 i;

    for (i = 0; i < MaxSimultaneousNotes; i++) {
        Note = &Notes[i];
        playbackState = (struct NotePlaybackState*) &Note->Priority;
        if (Note->ParentLayer != NO_LAYER) {
#ifndef NO_SEGMENTED_MEMORY
            if ((uintptr_t) playbackState->ParentLayer < 0x7fffffffU) {
                continue;
            }
#endif

#ifdef VERSION_EU_V10
            // Just threw these in here. They are probably wrong place and variables.
            stubbed_printf("----------------------Double-Error CH: %x %f\n", &Note, Note->ParentLayer->SEQChannel);
            stubbed_printf("----------------------Double-Error NT: %x\n", &Note);
            if (Note != playbackState->ParentLayer->Note && playbackState->Priority != 1) {
                playbackState->Adsr.Action |= ADSR_ACTION_RELEASE;
                playbackState->Adsr.FadeOutVel = AudioBufferParameters.UpdatesPerFrameInv;
                playbackState->Priority = 1;
                goto d;
            }
#endif
            if (!playbackState->ParentLayer->Enabled && playbackState->Priority >= NOTE_PRIORITY_MIN) {
                goto c;
            } else if (playbackState->ParentLayer->SEQChannel->seqPlayer == NULL) {
                eu_stubbed_printf_0("CAUTION:SUB IS SEPARATED FROM GROUP");
                NasInitSubTrack(playbackState->ParentLayer->SEQChannel);
                playbackState->Priority = NOTE_PRIORITY_STOPPING;
                continue;
            } else if (playbackState->ParentLayer->SEQChannel->seqPlayer->Muted) {
                if ((playbackState->ParentLayer->SEQChannel->MuteBehavior &
                     (MUTE_BEHAVIOR_STOP_SCRIPT | MUTE_BEHAVIOR_STOP_NOTES))) {
                    goto c;
                }
            }
            goto d;
        c:
            LLoopout(playbackState->ParentLayer);
            NasDeAllocAllVoices(&Note->ListItem);
            NasInitChannelList(&Note->ListItem.Pool->Decaying, &Note->ListItem);
            playbackState->Priority = NOTE_PRIORITY_STOPPING;
        } else if (playbackState->Priority >= NOTE_PRIORITY_MIN) {
            continue;
        }
    d:
        if (playbackState->Priority != NOTE_PRIORITY_DISABLED) {
            NoteSubEu = &Note->NoteSubEu;
            if (playbackState->Priority == NOTE_PRIORITY_STOPPING || NoteSubEu->Finished) {
                if (playbackState->Adsr.State == ADSR_STATE_DISABLED || NoteSubEu->Finished) {
                    if (playbackState && playbackState) {}
                    if (playbackState->WantedParentLayer != NO_LAYER) {
                        NasStopVoice(Note);
                        if (playbackState->WantedParentLayer->SEQChannel != NULL) {
                            NasAddListHead(Note, playbackState->WantedParentLayer);
                            NasModTableRead(Note);
                            NasDeAllocAllVoices(&Note->ListItem);
                            NasAllocSub(&Note->ListItem.Pool->Active, &Note->ListItem);
                            playbackState->WantedParentLayer = NO_LAYER;
                            // don't skip
                        } else {
                            eu_stubbed_printf_0("Error:Wait Track disappear\n");
                            NasStopVoice(Note);
                            NasDeAllocAllVoices(&Note->ListItem);
                            NasAllocSub(&Note->ListItem.Pool->Disabled, &Note->ListItem);
                            playbackState->WantedParentLayer = NO_LAYER;
                            goto skip;
                        }
                    } else {
                        NasStopVoice(Note);
                        NasDeAllocAllVoices(&Note->ListItem);
                        NasAllocSub(&Note->ListItem.Pool->Disabled, &Note->ListItem);
                        goto skip;
                    }
                }
            } else if (playbackState->Adsr.State == ADSR_STATE_DISABLED) {
                NasStopVoice(Note);
                NasDeAllocAllVoices(&Note->ListItem);
                NasAllocSub(&Note->ListItem.Pool->Disabled, &Note->ListItem);
                goto skip;
            }

            Scale = NasChannelModulation(&playbackState->Adsr);
            NasSweepCalculator(Note);
            Attributes = &playbackState->Attributes;
            if (playbackState->Priority == NOTE_PRIORITY_STOPPING) {
                Frequency = Attributes->FreqScale;
                Velocity = Attributes->Velocity;
                pan = Attributes->pan;
                ReverbVol = Attributes->ReverbVol;
                if (1) {}
                bookOffset = NoteSubEu->bookOffset;
            } else {
                Frequency = playbackState->ParentLayer->NoteFreqScale;
                Velocity = playbackState->ParentLayer->NoteVelocity;
                pan = playbackState->ParentLayer->NotePan;
                ReverbVol = playbackState->ParentLayer->SEQChannel->ReverbVol;
                bookOffset = playbackState->ParentLayer->SEQChannel->bookOffset & 0x7;
            }

            Frequency *= playbackState->VibratoFreqScale * playbackState->PortamentoFreqScale;
            Velocity = Velocity * Scale;
            NasSmzSetPitch(Note, Frequency);
            NasSmzSetVol(Note, Velocity, pan, ReverbVol);
            NoteSubEu->bookOffset = bookOffset;
        skip:;
        }
    }
}

void LReleaseEmergency(struct SequenceChannelLayer* seqLayer, s32 Target) {
    struct Note* Note;
    struct NoteAttributes* Attributes;

    if ((seqLayer == NO_LAYER) || (seqLayer->Note == NULL)) {
        return;
    }

    Note = seqLayer->Note;
    Attributes = &Note->Attributes;

    if (Note->WantedParentLayer == seqLayer) {
        Note->WantedParentLayer = NO_LAYER;
    }

    if (Note->ParentLayer != seqLayer) {
        if (Note->ParentLayer == NO_LAYER && Note->WantedParentLayer == NO_LAYER && Note->PrevParentLayer == seqLayer &&
            Target != ADSR_STATE_DECAY) {
            // Just guessing that this printf goes here... it's hard to parse.
            eu_stubbed_printf_0("Slow Release Batting\n");
            Note->Adsr.FadeOutVel = AudioBufferParameters.UpdatesPerFrameInv;
            Note->Adsr.Action |= ADSR_ACTION_RELEASE;
        }
    } else {
        seqLayer->Status = SOUND_LOAD_STATUS_NOT_LOADED;
        if (Note->Adsr.State != ADSR_STATE_DECAY) {
            Attributes->FreqScale = seqLayer->NoteFreqScale;
            Attributes->Velocity = seqLayer->NoteVelocity;
            Attributes->pan = seqLayer->NotePan;
            if (seqLayer->SEQChannel != NULL) {
                Attributes->ReverbVol = seqLayer->SEQChannel->ReverbVol;
            }
            Note->Priority = NOTE_PRIORITY_STOPPING;
            Note->PrevParentLayer = Note->ParentLayer;
            Note->ParentLayer = NO_LAYER;
            if (Target == ADSR_STATE_RELEASE) {
                Note->Adsr.FadeOutVel = AudioBufferParameters.UpdatesPerFrameInv;
                Note->Adsr.Action |= ADSR_ACTION_RELEASE;
            } else {
                Note->Adsr.Action |= ADSR_ACTION_DECAY;
                if (seqLayer->Adsr.ReleaseRate == 0) {
                    Note->Adsr.FadeOutVel =
                        seqLayer->SEQChannel->Adsr.ReleaseRate * AudioBufferParameters.UnkUpdatesPerFrameScaled;
                } else {
                    Note->Adsr.FadeOutVel =
                        seqLayer->Adsr.ReleaseRate * AudioBufferParameters.UnkUpdatesPerFrameScaled;
                }
                Note->Adsr.Sustain = (FLOAT_CAST(seqLayer->SEQChannel->Adsr.Sustain) * Note->Adsr.Current) / 256.0f;
            }
        }

        if (Target == ADSR_STATE_DECAY) {
            NasDeAllocAllVoices(&Note->ListItem);
            NasInitChannelList(&Note->ListItem.Pool->Decaying, &Note->ListItem);
        }
    }
}

void LCheckTerminate(struct SequenceChannelLayer* seqLayer) {
    LReleaseEmergency(seqLayer, ADSR_STATE_DECAY);
}

void LLoopout(struct SequenceChannelLayer* seqLayer) {
    LReleaseEmergency(seqLayer, ADSR_STATE_RELEASE);
}

const u8 D_800E98F4[4] = { 0x40, 0x20, 0x10, 0x08 };

s32 NasReleaseChannelMain(struct Note* Note, struct SequenceChannelLayer* seqLayer, s32 WaveId) {
    f32 FreqScale;
    f32 ratio;
    u8 SampleCountIndex;

    if (WaveId < 128) {
        stubbed_printf("Audio:Wavemem: Bad voiceno (%d)\n", WaveId);
        WaveId = 128;
    }

    FreqScale = seqLayer->FreqScale;
    if (seqLayer->Portamento.Mode != 0 && 0.0f < seqLayer->Portamento.Extent) {
        FreqScale *= (seqLayer->Portamento.Extent + 1.0f);
    }
    if (FreqScale < 1.0f) {
        SampleCountIndex = 0;
        ratio = 1.0465f;
    } else if (FreqScale < 2.0f) {
        SampleCountIndex = 1;
        ratio = 0.52325f;
    } else if (FreqScale < 4.0f) {
        SampleCountIndex = 2;
        ratio = 0.26263f;
    } else {
        SampleCountIndex = 3;
        ratio = 0.13081f;
    }
    seqLayer->FreqScale *= ratio;
    Note->WaveId = WaveId;
    Note->SampleCountIndex = SampleCountIndex;

    Note->NoteSubEu.Sound.Samples = &WaveSamples[WaveId - 128][SampleCountIndex * 64];

    return SampleCountIndex;
}

void NasReleaseChannel(struct Note* Note, struct SequenceChannelLayer* seqLayer) {
    s32 SampleCountIndex;
    s32 waveSampleCountIndex;
    s32 WaveId = seqLayer->InstOrWave;
    if (WaveId == 0xff) {
        WaveId = seqLayer->SEQChannel->InstOrWave;
    }
    SampleCountIndex = Note->SampleCountIndex;
    waveSampleCountIndex = NasReleaseChannelMain(Note, seqLayer, WaveId);
    Note->SynthesisState.SamplePosInt =
        Note->SynthesisState.SamplePosInt * D_800E98F4[waveSampleCountIndex] / D_800E98F4[SampleCountIndex];
}

void NasReleaseChannelForce(struct AudioListItem* list) {
    list->Prev = list;
    list->Next = list;
    list->u.Count = 0;
}

void NasWaveMemoryMake(struct NotePool* Pool) {
    NasReleaseChannelForce(&Pool->Disabled);
    NasReleaseChannelForce(&Pool->Decaying);
    NasReleaseChannelForce(&Pool->Releasing);
    NasReleaseChannelForce(&Pool->Active);
    Pool->Disabled.Pool = Pool;
    Pool->Decaying.Pool = Pool;
    Pool->Releasing.Pool = Pool;
    Pool->Active.Pool = Pool;
}

void NasContinueWaveMemory(void) {
    s32 i;

    NasWaveMemoryMake(&NoteFreeLists);
    for (i = 0; i < MaxSimultaneousNotes; i++) {
        Notes[i].ListItem.u.Value = &Notes[i];
        Notes[i].ListItem.Prev = NULL;
        NasAllocSub(&NoteFreeLists.Disabled, &Notes[i].ListItem);
    }
}

void NasInitList(struct NotePool* Pool) {
    s32 i;
    struct AudioListItem* Source;
    struct AudioListItem* cur;
    struct AudioListItem* dest;
    UNUSED s32 j; // Unused in EU

    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                Source = &Pool->Disabled;
                dest = &NoteFreeLists.Disabled;
                break;

            case 1:
                Source = &Pool->Decaying;
                dest = &NoteFreeLists.Decaying;
                break;

            case 2:
                Source = &Pool->Releasing;
                dest = &NoteFreeLists.Releasing;
                break;

            case 3:
                Source = &Pool->Active;
                dest = &NoteFreeLists.Active;
                break;
        }

        for (;;) {
            cur = Source->Next;
            if (cur == Source) {
                break;
            }
            if (cur == NULL) {
                eu_stubbed_printf_0("Audio: C-Alloc : Dealloc voice is NULL\n");
                break;
            }
            NasDeAllocAllVoices(cur);
            NasAllocSub(dest, cur);
        }
    }
}

void NasInitChNode(struct NotePool* Pool, s32 Count) {
    s32 i;
    s32 j;
    struct Note* Note;
    struct AudioListItem* Source;
    struct AudioListItem* dest;

    NasInitList(Pool);

    for (i = 0, j = 0; j < Count; i++) {
        if (i == 4) {
            eu_stubbed_printf_1("Alloc Error:Dim voice-Alloc %d", Count);
            return;
        }

        switch (i) {
            case 0:
                Source = &NoteFreeLists.Disabled;
                dest = &Pool->Disabled;
                break;

            case 1:
                Source = &NoteFreeLists.Decaying;
                dest = &Pool->Decaying;
                break;

            case 2:
                Source = &NoteFreeLists.Releasing;
                dest = &Pool->Releasing;
                break;

            case 3:
                Source = &NoteFreeLists.Active;
                dest = &Pool->Active;
                break;
        }

        while (j < Count) {
            Note = NasDeAllocSub(Source);
            if (Note == NULL) {
                break;
            }
            NasAllocSub(dest, &Note->ListItem);
            j++;
        }
    }
}

void NasInitChannelList(struct AudioListItem* list, struct AudioListItem* Item) {
    // add 'Item' to the front of the list given by 'list', if it's not in any list
    if (Item->Prev != NULL) {
        eu_stubbed_printf_0("Error:Same List Add\n");
    } else {
        Item->Prev = list;
        Item->Next = list->Next;
        list->Next->Prev = Item;
        list->Next = Item;
        list->u.Count++;
        Item->Pool = list->Pool;
    }
}

void NasDeAllocAllVoices(struct AudioListItem* Item) {
    // remove 'Item' from the list it's in, if any
    if (Item->Prev == NULL) {
        eu_stubbed_printf_0("Already Cut\n");
    } else {
        Item->Prev->Next = Item->Next;
        Item->Next->Prev = Item->Prev;
        Item->Prev = NULL;
    }
}

struct Note* NasAllocVoices(struct AudioListItem* list, s32 Limit) {
    struct AudioListItem* cur = list->Next;
    struct AudioListItem* best;

    if (cur == list) {
        return NULL;
    }

    for (best = cur; cur != list; cur = cur->Next) {
        if (((struct Note*) best->u.Value)->Priority >= ((struct Note*) cur->u.Value)->Priority) {
            best = cur;
        }
    }

    if (best == NULL) {
        return NULL;
    }

    if (Limit <= ((struct Note*) best->u.Value)->Priority) {
        return NULL;
    }

    NasDeAllocAllVoices(best);
    return best->u.Value;
}

void NasAddListHead(struct Note* Note, struct SequenceChannelLayer* seqLayer) {
    UNUSED s32 pad[4];
    s16 instId;
    struct NoteSubEu* sub = &Note->NoteSubEu;

    Note->PrevParentLayer = NO_LAYER;
    Note->ParentLayer = seqLayer;
    Note->Priority = seqLayer->SEQChannel->notePriority;
    seqLayer->NotePropertiesNeedInit = true;
    seqLayer->Status = SOUND_LOAD_STATUS_DISCARDABLE; // "Loaded"
    seqLayer->Note = Note;
    seqLayer->SEQChannel->noteUnused = Note;
    seqLayer->SEQChannel->layerUnused = seqLayer;
    seqLayer->NoteVelocity = 0.0f;
    NasStartVoice(Note);
    instId = seqLayer->InstOrWave;
    if (instId == 0xff) {
        instId = seqLayer->SEQChannel->InstOrWave;
    }
    sub->Sound.AudioBankSound = seqLayer->Sound;

    if (instId >= 0x80) {
        sub->isSyntheticWave = true;
    } else {
        sub->isSyntheticWave = false;
    }

    if (sub->isSyntheticWave) {
        NasReleaseChannelMain(Note, seqLayer, instId);
    }
    sub->BankId = seqLayer->SEQChannel->BankId;
    sub->StereoHeadsetEffects = seqLayer->SEQChannel->StereoHeadsetEffects;
    sub->reverbIndex = seqLayer->SEQChannel->reverbIndex & 3;
}

void NasCutList(struct Note* Note, struct SequenceChannelLayer* seqLayer) {
    LLoopout(Note->ParentLayer);
    Note->WantedParentLayer = seqLayer;
}

void NasGetLowerPrio(struct Note* Note, struct SequenceChannelLayer* seqLayer) {
    Note->WantedParentLayer = seqLayer;
    Note->Priority = NOTE_PRIORITY_STOPPING;
    Note->Adsr.FadeOutVel = AudioBufferParameters.UpdatesPerFrameInv;
    Note->Adsr.Action |= ADSR_ACTION_RELEASE;
}

struct Note* NasEntryTrack(struct NotePool* Pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* Note = NasDeAllocSub(&Pool->Disabled);
    if (Note != NULL) {
        NasAddListHead(Note, seqLayer);
        NasInitChannelList(&Pool->Active, &Note->ListItem);
    }
    return Note;
}

struct Note* NasInterTrack(struct NotePool* Pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* Note = NasDeAllocSub(&Pool->Decaying);
    if (Note != NULL) {
        NasGetLowerPrio(Note, seqLayer);
        NasAllocSub(&Pool->Releasing, &Note->ListItem);
    }
    return Note;
}

struct Note* NasInterReleaseTrack(struct NotePool* Pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* aNote;

    aNote = NasAllocVoices(&Pool->Active, seqLayer->SEQChannel->notePriority);

    if (aNote == NULL) {
        eu_stubbed_printf_0("Audio: C-Alloc : lowerPrio is NULL\n");
    } else {
        NasCutList(aNote, seqLayer);
        NasAllocSub(&Pool->Releasing, &aNote->ListItem);
    }

    return aNote;
}

struct Note* NasChLookFree(struct SequenceChannelLayer* seqLayer) {
    struct Note* ret;
    u32 policy = seqLayer->SEQChannel->NoteAllocPolicy;

    if (policy & NOTE_ALLOC_LAYER) {
        ret = seqLayer->Note;
        if (ret != NULL && ret->PrevParentLayer == seqLayer && ret->WantedParentLayer == NO_LAYER) {
            NasGetLowerPrio(ret, seqLayer);
            NasDeAllocAllVoices(&ret->ListItem);
            NasAllocSub(&ret->ListItem.Pool->Releasing, &ret->ListItem);
            return ret;
        }
    }

    if (policy & NOTE_ALLOC_CHANNEL) {
        if (!(ret = NasEntryTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
            !(ret = NasInterTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
            !(ret = NasInterReleaseTrack(&seqLayer->SEQChannel->NotePool, seqLayer))) {
            eu_stubbed_printf_0("Sub Limited Warning: Drop Voice");
            seqLayer->Status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (policy & NOTE_ALLOC_SEQ) {
        if (!(ret = NasEntryTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
            !(ret = NasEntryTrack(&seqLayer->SEQChannel->seqPlayer->NotePool, seqLayer)) &&
            !(ret = NasInterTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
            !(ret = NasInterTrack(&seqLayer->SEQChannel->seqPlayer->NotePool, seqLayer)) &&
            !(ret = NasInterReleaseTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
            !(ret = NasInterReleaseTrack(&seqLayer->SEQChannel->seqPlayer->NotePool, seqLayer))) {
            eu_stubbed_printf_0("Warning: Drop Voice");
            seqLayer->Status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (policy & NOTE_ALLOC_GLOBAL_FREELIST) {
        if (!(ret = NasEntryTrack(&NoteFreeLists, seqLayer)) &&
            !(ret = NasInterTrack(&NoteFreeLists, seqLayer)) &&
            !(ret = NasInterReleaseTrack(&NoteFreeLists, seqLayer))) {
            eu_stubbed_printf_0("Warning: Drop Voice");
            seqLayer->Status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (!(ret = NasEntryTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
        !(ret = NasEntryTrack(&seqLayer->SEQChannel->seqPlayer->NotePool, seqLayer)) &&
        !(ret = NasEntryTrack(&NoteFreeLists, seqLayer)) &&
        !(ret = NasInterTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
        !(ret = NasInterTrack(&seqLayer->SEQChannel->seqPlayer->NotePool, seqLayer)) &&
        !(ret = NasInterTrack(&NoteFreeLists, seqLayer)) &&
        !(ret = NasInterReleaseTrack(&seqLayer->SEQChannel->NotePool, seqLayer)) &&
        !(ret = NasInterReleaseTrack(&seqLayer->SEQChannel->seqPlayer->NotePool, seqLayer)) &&
        !(ret = NasInterReleaseTrack(&NoteFreeLists, seqLayer))) {
        eu_stubbed_printf_0("Warning: Drop Voice");
        seqLayer->Status = SOUND_LOAD_STATUS_NOT_LOADED;
        return NULL;
    }
    return ret;
}

void NasChLookRelease(void) {
    struct Note* Note;
    s32 i;

    for (i = 0; i < MaxSimultaneousNotes; i++) {
        Note = &Notes[i];
        Note->NoteSubEu = ZeroNoteSub;
        Note->Priority = NOTE_PRIORITY_DISABLED;
        Note->ParentLayer = NO_LAYER;
        Note->WantedParentLayer = NO_LAYER;
        Note->PrevParentLayer = NO_LAYER;
        Note->WaveId = 0;
        Note->Attributes.Velocity = 0.0f;
        Note->AdsrVolScale = 0;
        Note->Adsr.State = ADSR_STATE_DISABLED;
        Note->Adsr.Action = 0;
        Note->VibratoState.Active = false;
        Note->Portamento.cur = 0.0f;
        Note->Portamento.Speed = 0.0f;
        // This only works if NoteSynthesisBuffers are Size 0xA0. See internal.h
        Note->SynthesisState.SynthesisBuffers = NasHeapAlloc(&NotesAndBuffersPool, sizeof(struct NoteSynthesisBuffers));
    }
}
