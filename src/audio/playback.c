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

void Nas_smzSetVol(struct Note* note, f32 velocity, u8 pan, u8 reverbVol) {
    struct NoteSubEu* sub = &note->noteSubEu;
    f32 volRight, volLeft;
    u8 strongRight;
    u8 strongLeft;
    s32 smallPanIndex;
    u16 unkMask = ~0x80;

    pan &= unkMask;

    if (note->noteSubEu.stereoHeadsetEffects && gAudioLibSoundMode == SOUND_MODE_HEADSET) {
        smallPanIndex = pan >> 3;
        if (smallPanIndex >= ARRAY_COUNT(gHeadsetPanQuantization)) {
            smallPanIndex = ARRAY_COUNT(gHeadsetPanQuantization) - 1;
        }

        sub->headsetPanLeft = gHeadsetPanQuantization[smallPanIndex];
        sub->headsetPanRight = gHeadsetPanQuantization[ARRAY_COUNT(gHeadsetPanQuantization) - 1 - smallPanIndex];
        sub->stereoStrongRight = false;
        sub->stereoStrongLeft = false;
        sub->usesHeadsetPanEffects = true;

        volLeft = gHeadsetPanVolume[pan];
        volRight = gHeadsetPanVolume[127 - pan];
    } else if (sub->stereoHeadsetEffects && gAudioLibSoundMode == SOUND_MODE_STEREO) {
        strongRight = false;
        strongLeft = false;
        sub->headsetPanRight = 0;
        sub->headsetPanLeft = 0;

        sub->usesHeadsetPanEffects = false;

        volLeft = gStereoPanVolume[pan];
        volRight = gStereoPanVolume[127 - pan];
        if (pan < 0x20) {
            strongLeft = true;
        } else if (pan > 0x60) {
            strongRight = true;
        }

        sub->stereoStrongRight = strongRight;
        sub->stereoStrongLeft = strongLeft;

    } else if (gAudioLibSoundMode == SOUND_MODE_MONO) {
        volLeft = 0.707f;
        volRight = 0.707f;
    } else {
        volLeft = gDefaultPanVolume[pan];
        volRight = gDefaultPanVolume[127 - pan];
    }

    if (velocity < 0.0f) {
        stubbed_printf("Audio: setvol: volume minus %f\n", velocity);
        velocity = 0.0f;
    }
    if (velocity > 1.0f) {
        stubbed_printf("Audio: setvol: volume overflow %f\n", velocity);
        velocity = 1.0f;
    }

    sub->targetVolLeft = ((s32) (velocity * volLeft * 4095.999f));
    sub->targetVolRight = ((s32) (velocity * volRight * 4095.999f));

    //! @bug for the change to UQ0.7, the if statement should also have been changed accordingly
    if (sub->reverbVol != reverbVol) {
        sub->reverbVol = reverbVol;
        sub->envMixerNeedsInit = true;
        return;
    }

    if (sub->needsInit) {
        sub->envMixerNeedsInit = true;
    } else {
        sub->envMixerNeedsInit = false;
    }
}

void Nas_smzSetPitch(struct Note* note, f32 resamplingRateInput) {
    f32 resamplingRate = 0.0f;
    struct NoteSubEu* tempSub = &note->noteSubEu;

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
    note->noteSubEu.resamplingRateFixedPoint = (s32) (resamplingRate * 32768.0f);
}

struct AudioBankSound* NoteToVoice(struct Instrument* instrument, s32 semitone) {
    struct AudioBankSound* sound;
    if (semitone < instrument->normalRangeLo) {
        sound = &instrument->lowNotesSound;
    } else if (semitone <= instrument->normalRangeHi) {
        sound = &instrument->normalNotesSound;
    } else {
        sound = &instrument->highNotesSound;
    }
    return sound;
}

struct Instrument* ProgToVp(s32 bankId, s32 instId) {
    struct Instrument* inst;

    if (IS_BANK_LOAD_COMPLETE(bankId) == false) {
        stubbed_printf("Audio: voiceman: No bank error %d\n", bankId);
        gAudioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (instId >= g_sfxPointer[bankId].numInstruments) {
        stubbed_printf("Audio: voiceman: progNo. overflow %d,%d\n", instId, g_sfxPointer[bankId].numInstruments);
        gAudioErrorFlags = ((bankId << 8) + instId) + 0x3000000;
        return NULL;
    }

    inst = g_sfxPointer[bankId].instruments[instId];
    if (inst == NULL) {
        stubbed_printf("Audio: voiceman: progNo. undefined %d,%d\n", bankId, instId);
        gAudioErrorFlags = ((bankId << 8) + instId) + 0x1000000;
        return inst;
    }
    return inst;
}

struct Drum* PercToPp(s32 bankId, s32 drumId) {
    struct Drum* drum;

    if (IS_BANK_LOAD_COMPLETE(bankId) == false) {
        stubbed_printf("Audio: voiceman: No bank error %d\n", bankId);
        gAudioErrorFlags = bankId + 0x10000000;
        return NULL;
    }

    if (drumId >= g_sfxPointer[bankId].numDrums) {
        stubbed_printf("Audio: voiceman: Percussion Overflow %d,%d\n", drumId, g_sfxPointer[bankId].numDrums);
        gAudioErrorFlags = ((bankId << 8) + drumId) + 0x4000000;
        return NULL;
    }

    if ((uintptr_t) g_sfxPointer[bankId].drums < 0x80000000U) {
        stubbed_printf("Audio: voiceman: Percussion table pointer (bank %d) is irregular.\n");
        return NULL;
    }

    drum = g_sfxPointer[bankId].drums[drumId];
    if (drum == NULL) {
        stubbed_printf("Audio: voiceman: Percpointer NULL %d,%d\n", bankId, drumId);
        gAudioErrorFlags = ((bankId << 8) + drumId) + 0x5000000;
    }
    // Don't know where these actually belong, putting them here because why not
    // Really, we just need these so the format strings could put into the .data section
    stubbed_printf("--4 %x\n", gAudioErrorFlags);
    stubbed_printf("Stoped Voice\n");
    return drum;
}

void Nas_StartVoice(struct Note* note) {
    if (note->parentLayer->adsr.releaseRate == 0) {
        Nas_Modulator(&note->adsr, note->parentLayer->seqChannel->adsr.envelope, &note->adsrVolScale);
    } else {
        Nas_Modulator(&note->adsr, note->parentLayer->adsr.envelope, &note->adsrVolScale);
    }
    note->adsr.state = ADSR_STATE_INITIAL;
    note->noteSubEu = gDefaultNoteSub;
}

void Nas_StopVoice(struct Note* note) {
    if (note->noteSubEu.needsInit == true) {
        note->noteSubEu.needsInit = false;
    } else {
        Nas_smzSetVol(note, 0, 0x40, 0);
    }
    note->priority = NOTE_PRIORITY_DISABLED;
    note->parentLayer = NO_LAYER;
    note->prevParentLayer = NO_LAYER;
    note->noteSubEu.enabled = false;
    note->noteSubEu.finished = false;
}

void Nas_UpdateChannel(void) {
    f32 scale;
    f32 frequency;
    f32 velocity;
    struct Note* note;
    struct NotePlaybackState* playbackState;
    struct NoteSubEu* noteSubEu;
    UNUSED u8 pad[12];
    u8 reverbVol;
    UNUSED u8 pad3;
    u8 pan;
    u8 bookOffset;
    struct NoteAttributes* attributes;
    s32 i;

    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        note = &gNotes[i];
        playbackState = (struct NotePlaybackState*) &note->priority;
        if (note->parentLayer != NO_LAYER) {
#ifndef NO_SEGMENTED_MEMORY
            if ((uintptr_t) playbackState->parentLayer < 0x7fffffffU) {
                continue;
            }
#endif

#ifdef VERSION_EU_V10
            // Just threw these in here. They are probably wrong place and variables.
            stubbed_printf("----------------------Double-Error CH: %x %f\n", &note, note->parentLayer->seqChannel);
            stubbed_printf("----------------------Double-Error NT: %x\n", &note);
            if (note != playbackState->parentLayer->note && playbackState->priority != 1) {
                playbackState->adsr.action |= ADSR_ACTION_RELEASE;
                playbackState->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
                playbackState->priority = 1;
                goto d;
            }
#endif
            if (!playbackState->parentLayer->enabled && playbackState->priority >= NOTE_PRIORITY_MIN) {
                goto c;
            } else if (playbackState->parentLayer->seqChannel->seqPlayer == NULL) {
                eu_stubbed_printf_0("CAUTION:SUB IS SEPARATED FROM GROUP");
                Nas_InitSubTrack(playbackState->parentLayer->seqChannel);
                playbackState->priority = NOTE_PRIORITY_STOPPING;
                continue;
            } else if (playbackState->parentLayer->seqChannel->seqPlayer->muted) {
                if ((playbackState->parentLayer->seqChannel->muteBehavior &
                     (MUTE_BEHAVIOR_STOP_SCRIPT | MUTE_BEHAVIOR_STOP_NOTES))) {
                    goto c;
                }
            }
            goto d;
        c:
            L_loopout(playbackState->parentLayer);
            Nas_DeAllocAllVoices(&note->listItem);
            Nas_InitChannelList(&note->listItem.pool->decaying, &note->listItem);
            playbackState->priority = NOTE_PRIORITY_STOPPING;
        } else if (playbackState->priority >= NOTE_PRIORITY_MIN) {
            continue;
        }
    d:
        if (playbackState->priority != NOTE_PRIORITY_DISABLED) {
            noteSubEu = &note->noteSubEu;
            if (playbackState->priority == NOTE_PRIORITY_STOPPING || noteSubEu->finished) {
                if (playbackState->adsr.state == ADSR_STATE_DISABLED || noteSubEu->finished) {
                    if (playbackState && playbackState) {}
                    if (playbackState->wantedParentLayer != NO_LAYER) {
                        Nas_StopVoice(note);
                        if (playbackState->wantedParentLayer->seqChannel != NULL) {
                            Nas_AddListHead(note, playbackState->wantedParentLayer);
                            Nas_ModTableRead(note);
                            Nas_DeAllocAllVoices(&note->listItem);
                            Nas_AllocSub(&note->listItem.pool->active, &note->listItem);
                            playbackState->wantedParentLayer = NO_LAYER;
                            // don't skip
                        } else {
                            eu_stubbed_printf_0("Error:Wait Track disappear\n");
                            Nas_StopVoice(note);
                            Nas_DeAllocAllVoices(&note->listItem);
                            Nas_AllocSub(&note->listItem.pool->disabled, &note->listItem);
                            playbackState->wantedParentLayer = NO_LAYER;
                            goto skip;
                        }
                    } else {
                        Nas_StopVoice(note);
                        Nas_DeAllocAllVoices(&note->listItem);
                        Nas_AllocSub(&note->listItem.pool->disabled, &note->listItem);
                        goto skip;
                    }
                }
            } else if (playbackState->adsr.state == ADSR_STATE_DISABLED) {
                Nas_StopVoice(note);
                Nas_DeAllocAllVoices(&note->listItem);
                Nas_AllocSub(&note->listItem.pool->disabled, &note->listItem);
                goto skip;
            }

            scale = Nas_ChannelModulation(&playbackState->adsr);
            Nas_SweepCalculator(note);
            attributes = &playbackState->attributes;
            if (playbackState->priority == NOTE_PRIORITY_STOPPING) {
                frequency = attributes->freqScale;
                velocity = attributes->velocity;
                pan = attributes->pan;
                reverbVol = attributes->reverbVol;
                if (1) {}
                bookOffset = noteSubEu->bookOffset;
            } else {
                frequency = playbackState->parentLayer->noteFreqScale;
                velocity = playbackState->parentLayer->noteVelocity;
                pan = playbackState->parentLayer->notePan;
                reverbVol = playbackState->parentLayer->seqChannel->reverbVol;
                bookOffset = playbackState->parentLayer->seqChannel->bookOffset & 0x7;
            }

            frequency *= playbackState->vibratoFreqScale * playbackState->portamentoFreqScale;
            velocity = velocity * scale;
            Nas_smzSetPitch(note, frequency);
            Nas_smzSetVol(note, velocity, pan, reverbVol);
            noteSubEu->bookOffset = bookOffset;
        skip:;
        }
    }
}

void L_ReleaseEmergency(struct SequenceChannelLayer* seqLayer, s32 target) {
    struct Note* note;
    struct NoteAttributes* attributes;

    if ((seqLayer == NO_LAYER) || (seqLayer->note == NULL)) {
        return;
    }

    note = seqLayer->note;
    attributes = &note->attributes;

    if (note->wantedParentLayer == seqLayer) {
        note->wantedParentLayer = NO_LAYER;
    }

    if (note->parentLayer != seqLayer) {
        if (note->parentLayer == NO_LAYER && note->wantedParentLayer == NO_LAYER && note->prevParentLayer == seqLayer &&
            target != ADSR_STATE_DECAY) {
            // Just guessing that this printf goes here... it's hard to parse.
            eu_stubbed_printf_0("Slow Release Batting\n");
            note->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
            note->adsr.action |= ADSR_ACTION_RELEASE;
        }
    } else {
        seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
        if (note->adsr.state != ADSR_STATE_DECAY) {
            attributes->freqScale = seqLayer->noteFreqScale;
            attributes->velocity = seqLayer->noteVelocity;
            attributes->pan = seqLayer->notePan;
            if (seqLayer->seqChannel != NULL) {
                attributes->reverbVol = seqLayer->seqChannel->reverbVol;
            }
            note->priority = NOTE_PRIORITY_STOPPING;
            note->prevParentLayer = note->parentLayer;
            note->parentLayer = NO_LAYER;
            if (target == ADSR_STATE_RELEASE) {
                note->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
                note->adsr.action |= ADSR_ACTION_RELEASE;
            } else {
                note->adsr.action |= ADSR_ACTION_DECAY;
                if (seqLayer->adsr.releaseRate == 0) {
                    note->adsr.fadeOutVel =
                        seqLayer->seqChannel->adsr.releaseRate * gAudioBufferParameters.unkUpdatesPerFrameScaled;
                } else {
                    note->adsr.fadeOutVel =
                        seqLayer->adsr.releaseRate * gAudioBufferParameters.unkUpdatesPerFrameScaled;
                }
                note->adsr.sustain = (FLOAT_CAST(seqLayer->seqChannel->adsr.sustain) * note->adsr.current) / 256.0f;
            }
        }

        if (target == ADSR_STATE_DECAY) {
            Nas_DeAllocAllVoices(&note->listItem);
            Nas_InitChannelList(&note->listItem.pool->decaying, &note->listItem);
        }
    }
}

void L_CheckTerminate(struct SequenceChannelLayer* seqLayer) {
    L_ReleaseEmergency(seqLayer, ADSR_STATE_DECAY);
}

void L_loopout(struct SequenceChannelLayer* seqLayer) {
    L_ReleaseEmergency(seqLayer, ADSR_STATE_RELEASE);
}

const u8 D_800E98F4[4] = { 0x40, 0x20, 0x10, 0x08 };

s32 Nas_Release_Channel_Main(struct Note* note, struct SequenceChannelLayer* seqLayer, s32 waveId) {
    f32 freqScale;
    f32 ratio;
    u8 sampleCountIndex;

    if (waveId < 128) {
        stubbed_printf("Audio:Wavemem: Bad voiceno (%d)\n", waveId);
        waveId = 128;
    }

    freqScale = seqLayer->freqScale;
    if (seqLayer->portamento.mode != 0 && 0.0f < seqLayer->portamento.extent) {
        freqScale *= (seqLayer->portamento.extent + 1.0f);
    }
    if (freqScale < 1.0f) {
        sampleCountIndex = 0;
        ratio = 1.0465f;
    } else if (freqScale < 2.0f) {
        sampleCountIndex = 1;
        ratio = 0.52325f;
    } else if (freqScale < 4.0f) {
        sampleCountIndex = 2;
        ratio = 0.26263f;
    } else {
        sampleCountIndex = 3;
        ratio = 0.13081f;
    }
    seqLayer->freqScale *= ratio;
    note->waveId = waveId;
    note->sampleCountIndex = sampleCountIndex;

    note->noteSubEu.sound.samples = &gWaveSamples[waveId - 128][sampleCountIndex * 64];

    return sampleCountIndex;
}

void Nas_Release_Channel(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    s32 sampleCountIndex;
    s32 waveSampleCountIndex;
    s32 waveId = seqLayer->instOrWave;
    if (waveId == 0xff) {
        waveId = seqLayer->seqChannel->instOrWave;
    }
    sampleCountIndex = note->sampleCountIndex;
    waveSampleCountIndex = Nas_Release_Channel_Main(note, seqLayer, waveId);
    note->synthesisState.samplePosInt =
        note->synthesisState.samplePosInt * D_800E98F4[waveSampleCountIndex] / D_800E98F4[sampleCountIndex];
}

void Nas_Release_Channel_Force(struct AudioListItem* list) {
    list->prev = list;
    list->next = list;
    list->u.count = 0;
}

void Nas_WaveMemoryMake(struct NotePool* pool) {
    Nas_Release_Channel_Force(&pool->disabled);
    Nas_Release_Channel_Force(&pool->decaying);
    Nas_Release_Channel_Force(&pool->releasing);
    Nas_Release_Channel_Force(&pool->active);
    pool->disabled.pool = pool;
    pool->decaying.pool = pool;
    pool->releasing.pool = pool;
    pool->active.pool = pool;
}

void Nas_ContinueWaveMemory(void) {
    s32 i;

    Nas_WaveMemoryMake(&gNoteFreeLists);
    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        gNotes[i].listItem.u.value = &gNotes[i];
        gNotes[i].listItem.prev = NULL;
        Nas_AllocSub(&gNoteFreeLists.disabled, &gNotes[i].listItem);
    }
}

void Nas_InitList(struct NotePool* pool) {
    s32 i;
    struct AudioListItem* source;
    struct AudioListItem* cur;
    struct AudioListItem* dest;
    UNUSED s32 j; // unused in EU

    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                source = &pool->disabled;
                dest = &gNoteFreeLists.disabled;
                break;

            case 1:
                source = &pool->decaying;
                dest = &gNoteFreeLists.decaying;
                break;

            case 2:
                source = &pool->releasing;
                dest = &gNoteFreeLists.releasing;
                break;

            case 3:
                source = &pool->active;
                dest = &gNoteFreeLists.active;
                break;
        }

        for (;;) {
            cur = source->next;
            if (cur == source) {
                break;
            }
            if (cur == NULL) {
                eu_stubbed_printf_0("Audio: C-Alloc : Dealloc voice is NULL\n");
                break;
            }
            Nas_DeAllocAllVoices(cur);
            Nas_AllocSub(dest, cur);
        }
    }
}

void Nas_InitChNode(struct NotePool* pool, s32 count) {
    s32 i;
    s32 j;
    struct Note* note;
    struct AudioListItem* source;
    struct AudioListItem* dest;

    Nas_InitList(pool);

    for (i = 0, j = 0; j < count; i++) {
        if (i == 4) {
            eu_stubbed_printf_1("Alloc Error:Dim voice-Alloc %d", count);
            return;
        }

        switch (i) {
            case 0:
                source = &gNoteFreeLists.disabled;
                dest = &pool->disabled;
                break;

            case 1:
                source = &gNoteFreeLists.decaying;
                dest = &pool->decaying;
                break;

            case 2:
                source = &gNoteFreeLists.releasing;
                dest = &pool->releasing;
                break;

            case 3:
                source = &gNoteFreeLists.active;
                dest = &pool->active;
                break;
        }

        while (j < count) {
            note = Nas_DeAllocSub(source);
            if (note == NULL) {
                break;
            }
            Nas_AllocSub(dest, &note->listItem);
            j++;
        }
    }
}

void Nas_InitChannelList(struct AudioListItem* list, struct AudioListItem* item) {
    // add 'item' to the front of the list given by 'list', if it's not in any list
    if (item->prev != NULL) {
        eu_stubbed_printf_0("Error:Same List Add\n");
    } else {
        item->prev = list;
        item->next = list->next;
        list->next->prev = item;
        list->next = item;
        list->u.count++;
        item->pool = list->pool;
    }
}

void Nas_DeAllocAllVoices(struct AudioListItem* item) {
    // remove 'item' from the list it's in, if any
    if (item->prev == NULL) {
        eu_stubbed_printf_0("Already Cut\n");
    } else {
        item->prev->next = item->next;
        item->next->prev = item->prev;
        item->prev = NULL;
    }
}

struct Note* Nas_AllocVoices(struct AudioListItem* list, s32 limit) {
    struct AudioListItem* cur = list->next;
    struct AudioListItem* best;

    if (cur == list) {
        return NULL;
    }

    for (best = cur; cur != list; cur = cur->next) {
        if (((struct Note*) best->u.value)->priority >= ((struct Note*) cur->u.value)->priority) {
            best = cur;
        }
    }

    if (best == NULL) {
        return NULL;
    }

    if (limit <= ((struct Note*) best->u.value)->priority) {
        return NULL;
    }

    Nas_DeAllocAllVoices(best);
    return best->u.value;
}

void Nas_AddListHead(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    UNUSED s32 pad[4];
    s16 instId;
    struct NoteSubEu* sub = &note->noteSubEu;

    note->prevParentLayer = NO_LAYER;
    note->parentLayer = seqLayer;
    note->priority = seqLayer->seqChannel->notePriority;
    seqLayer->notePropertiesNeedInit = true;
    seqLayer->status = SOUND_LOAD_STATUS_DISCARDABLE; // "loaded"
    seqLayer->note = note;
    seqLayer->seqChannel->noteUnused = note;
    seqLayer->seqChannel->layerUnused = seqLayer;
    seqLayer->noteVelocity = 0.0f;
    Nas_StartVoice(note);
    instId = seqLayer->instOrWave;
    if (instId == 0xff) {
        instId = seqLayer->seqChannel->instOrWave;
    }
    sub->sound.audioBankSound = seqLayer->sound;

    if (instId >= 0x80) {
        sub->isSyntheticWave = true;
    } else {
        sub->isSyntheticWave = false;
    }

    if (sub->isSyntheticWave) {
        Nas_Release_Channel_Main(note, seqLayer, instId);
    }
    sub->bankId = seqLayer->seqChannel->bankId;
    sub->stereoHeadsetEffects = seqLayer->seqChannel->stereoHeadsetEffects;
    sub->reverbIndex = seqLayer->seqChannel->reverbIndex & 3;
}

void Nas_CutList(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    L_loopout(note->parentLayer);
    note->wantedParentLayer = seqLayer;
}

void Nas_GetLowerPrio(struct Note* note, struct SequenceChannelLayer* seqLayer) {
    note->wantedParentLayer = seqLayer;
    note->priority = NOTE_PRIORITY_STOPPING;
    note->adsr.fadeOutVel = gAudioBufferParameters.updatesPerFrameInv;
    note->adsr.action |= ADSR_ACTION_RELEASE;
}

struct Note* Nas_EntryTrack(struct NotePool* pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* note = Nas_DeAllocSub(&pool->disabled);
    if (note != NULL) {
        Nas_AddListHead(note, seqLayer);
        Nas_InitChannelList(&pool->active, &note->listItem);
    }
    return note;
}

struct Note* Nas_InterTrack(struct NotePool* pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* note = Nas_DeAllocSub(&pool->decaying);
    if (note != NULL) {
        Nas_GetLowerPrio(note, seqLayer);
        Nas_AllocSub(&pool->releasing, &note->listItem);
    }
    return note;
}

struct Note* Nas_InterReleaseTrack(struct NotePool* pool, struct SequenceChannelLayer* seqLayer) {
    struct Note* aNote;

    aNote = Nas_AllocVoices(&pool->active, seqLayer->seqChannel->notePriority);

    if (aNote == NULL) {
        eu_stubbed_printf_0("Audio: C-Alloc : lowerPrio is NULL\n");
    } else {
        Nas_CutList(aNote, seqLayer);
        Nas_AllocSub(&pool->releasing, &aNote->listItem);
    }

    return aNote;
}

struct Note* Nas_ChLookFree(struct SequenceChannelLayer* seqLayer) {
    struct Note* ret;
    u32 policy = seqLayer->seqChannel->noteAllocPolicy;

    if (policy & NOTE_ALLOC_LAYER) {
        ret = seqLayer->note;
        if (ret != NULL && ret->prevParentLayer == seqLayer && ret->wantedParentLayer == NO_LAYER) {
            Nas_GetLowerPrio(ret, seqLayer);
            Nas_DeAllocAllVoices(&ret->listItem);
            Nas_AllocSub(&ret->listItem.pool->releasing, &ret->listItem);
            return ret;
        }
    }

    if (policy & NOTE_ALLOC_CHANNEL) {
        if (!(ret = Nas_EntryTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = Nas_InterTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = Nas_InterReleaseTrack(&seqLayer->seqChannel->notePool, seqLayer))) {
            eu_stubbed_printf_0("Sub Limited Warning: Drop Voice");
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (policy & NOTE_ALLOC_SEQ) {
        if (!(ret = Nas_EntryTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = Nas_EntryTrack(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
            !(ret = Nas_InterTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = Nas_InterTrack(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
            !(ret = Nas_InterReleaseTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
            !(ret = Nas_InterReleaseTrack(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer))) {
            eu_stubbed_printf_0("Warning: Drop Voice");
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (policy & NOTE_ALLOC_GLOBAL_FREELIST) {
        if (!(ret = Nas_EntryTrack(&gNoteFreeLists, seqLayer)) &&
            !(ret = Nas_InterTrack(&gNoteFreeLists, seqLayer)) &&
            !(ret = Nas_InterReleaseTrack(&gNoteFreeLists, seqLayer))) {
            eu_stubbed_printf_0("Warning: Drop Voice");
            seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
            return NULL;
        }
        return ret;
    }

    if (!(ret = Nas_EntryTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = Nas_EntryTrack(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = Nas_EntryTrack(&gNoteFreeLists, seqLayer)) &&
        !(ret = Nas_InterTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = Nas_InterTrack(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = Nas_InterTrack(&gNoteFreeLists, seqLayer)) &&
        !(ret = Nas_InterReleaseTrack(&seqLayer->seqChannel->notePool, seqLayer)) &&
        !(ret = Nas_InterReleaseTrack(&seqLayer->seqChannel->seqPlayer->notePool, seqLayer)) &&
        !(ret = Nas_InterReleaseTrack(&gNoteFreeLists, seqLayer))) {
        eu_stubbed_printf_0("Warning: Drop Voice");
        seqLayer->status = SOUND_LOAD_STATUS_NOT_LOADED;
        return NULL;
    }
    return ret;
}

void Nas_ChLookRelease(void) {
    struct Note* note;
    s32 i;

    for (i = 0; i < gMaxSimultaneousNotes; i++) {
        note = &gNotes[i];
        note->noteSubEu = gZeroNoteSub;
        note->priority = NOTE_PRIORITY_DISABLED;
        note->parentLayer = NO_LAYER;
        note->wantedParentLayer = NO_LAYER;
        note->prevParentLayer = NO_LAYER;
        note->waveId = 0;
        note->attributes.velocity = 0.0f;
        note->adsrVolScale = 0;
        note->adsr.state = ADSR_STATE_DISABLED;
        note->adsr.action = 0;
        note->vibratoState.active = false;
        note->portamento.cur = 0.0f;
        note->portamento.speed = 0.0f;
        // This only works if NoteSynthesisBuffers are size 0xA0. See internal.h
        note->synthesisState.synthesisBuffers = Nas_HeapAlloc(&gNotesAndBuffersPool, sizeof(struct NoteSynthesisBuffers));
    }
}
