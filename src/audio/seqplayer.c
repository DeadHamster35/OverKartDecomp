#include <ultra64.h>
#include <macros.h>

#include "audio/data.h"
#include "audio/internal.h"
#include "audio/seqplayer.h"
#include "audio/load.h"
#include "audio/heap.h"
#include "audio/effects.h"
#include "audio/playback.h"
#include "audio/external.h"

/**
 * Given that (almost) all of these are format strings, it is highly likely
 * that they are meant to be used in some sort of printf variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 * This means some printf stubs have been commented out to avoid duplication
 * of some strings (i.e seqplayer_unused_string00)
 **/
char seqplayer_unused_string00[] = "Audio:Track:Warning: No Free Notetrack\n";
char seqplayer_unused_string01[] = "SUBTRACK DIM\n";
char seqplayer_unused_string02[] = "Audio:Track: Warning :SUBTRACK had been stolen by other Group.\n";
char seqplayer_unused_string03[] = "SEQID %d,BANKID %d\n";
char seqplayer_unused_string04[] = "ERR:SUBTRACK %d NOT ALLOCATED\n";
char seqplayer_unused_string05[] = "Error:Same List Add\n";
#ifdef VERSION_EU_V10
char seqplayer_unused_string_eu_01[] = "Wait Time out!\n";
#endif
char seqplayer_unused_string06[] = "Macro Level Over Error!\n";
char seqplayer_unused_string07[] = "Macro Level Over Error!\n";
char seqplayer_unused_string08[] = "WARNING: NPRG: cannot change %d\n";
char seqplayer_unused_string09[] = "Audio:Track:NOTE:UNDEFINED NOTE COM. %x\n";
char seqplayer_unused_string10[] = "Audio: Note:Velocity Error %d\n";
char seqplayer_unused_string11[] = "Error: Subtrack no prg.\n";
char seqplayer_unused_string12[] = "ERR %x\n";
char seqplayer_unused_string13[] = "Error: Your assignchannel is stolen.\n";
char seqplayer_unused_string14[] = "Audio:Track :Call Macro Level Over Error!\n";
char seqplayer_unused_string15[] = "Audio:Track :Loops Macro Level Over Error!\n";
char seqplayer_unused_string16[] = "SUB:ERR:BANK %d NOT CACHED.\n";
char seqplayer_unused_string17[] = "SUB:ERR:BANK %d NOT CACHED.\n";
char seqplayer_unused_string18[] = "Audio:Track: CTBLCALL Macro Level Over Error!\n";
char seqplayer_unused_string19[] = "[%2x] \n";
char seqplayer_unused_string20[] = "Err :Sub %x ,address %x:Undefined SubTrack Function %x";
char seqplayer_unused_string21[] = "Disappear Sequence or Bank %d\n";
char seqplayer_unused_string22[] = "Macro Level Over Error!\n";
char seqplayer_unused_string23[] = "Macro Level Over Error!\n";
char seqplayer_unused_string24[] = "Group:Undefine upper C0h command (%x)\n";
char seqplayer_unused_string25[] = "Group:Undefined Command\n";

// Nas_ChannelModInit
void Nas_ChannelModInit(struct SequenceChannel* seqChannel) {
    s32 i;

    seqChannel->enabled = false;
    seqChannel->finished = false;
    seqChannel->stopScript = false;
    seqChannel->stopSomething2 = false;
    seqChannel->hasInstrument = false;
    seqChannel->stereoHeadsetEffects = false;
    seqChannel->transposition = 0;
    seqChannel->largeNotes = false;
    seqChannel->bookOffset = 0;
    seqChannel->changes.as_u8 = 0xff;
    seqChannel->scriptState.depth = 0;
    seqChannel->newPan = 0x40;
    seqChannel->panChannelWeight = 0x80;
    seqChannel->noteUnused = NULL;
    seqChannel->reverbIndex = 0;
    seqChannel->reverbVol = 0;
    seqChannel->notePriority = NOTE_PRIORITY_DEFAULT;
    seqChannel->delay = 0;
    seqChannel->adsr.envelope = gDefaultEnvelope;
    seqChannel->adsr.releaseRate = 0x20;
    seqChannel->adsr.sustain = 0;
    seqChannel->vibratoRateTarget = 0x800;
    seqChannel->vibratoRateStart = 0x800;
    seqChannel->vibratoExtentTarget = 0;
    seqChannel->vibratoExtentStart = 0;
    seqChannel->vibratoRateChangeDelay = 0;
    seqChannel->vibratoExtentChangeDelay = 0;
    seqChannel->vibratoDelay = 0;
    seqChannel->volume = 1.0f;
    seqChannel->volumeScale = 1.0f;
    seqChannel->freqScale = 1.0f;
    for (i = 0; i < 8; i++) {
        seqChannel->soundScriptIO[i] = -1;
    }
    seqChannel->unused = false;
    Nas_WaveMemoryMake(&seqChannel->notePool);
}

s32 Nas_EnvInit(struct SequenceChannel* seqChannel, s32 layerIndex) {
    struct SequenceChannelLayer* layer;

    if (seqChannel->layers[layerIndex] == NULL) {
        struct SequenceChannelLayer* layer;
        layer = Nas_DeAllocSub(&gLayerFreeList);
        seqChannel->layers[layerIndex] = layer;
        if (layer == NULL) {
            seqChannel->layers[layerIndex] = NULL;
            return -1;
        }
    } else {
        L_CheckTerminate(seqChannel->layers[layerIndex]);
    }

    layer = seqChannel->layers[layerIndex];
    layer->seqChannel = seqChannel;
    layer->adsr = seqChannel->adsr;
    layer->adsr.releaseRate = 0;
    layer->enabled = true;
    layer->stopSomething = false;
    layer->continuousNotes = false;
    layer->finished = false;
    layer->ignoreDrumPan = false;
    layer->portamento.mode = 0;
    layer->scriptState.depth = 0;
    layer->status = SOUND_LOAD_STATUS_NOT_LOADED;
    layer->noteDuration = 0x80;
    layer->pan = 0x40;
    layer->transposition = 0;
    layer->delay = 0;
    layer->duration = 0;
    layer->delayUnused = 0;
    layer->note = NULL;
    layer->instrument = NULL;
    layer->freqScale = 1.0f;
    layer->velocitySquare = 0.0f;
    layer->instOrWave = 0xff;
    return 0;
}

void Nas_EnvProcess(struct SequenceChannelLayer* layer) {
    if (layer != NULL) {
        L_CheckTerminate(layer);
        layer->enabled = false;
        layer->finished = true;
    }
}

void ENV_RETRY(struct SequenceChannel* seqChannel, s32 layerIndex) {
    struct SequenceChannelLayer* layer = seqChannel->layers[layerIndex];

    if (layer != NULL) {
        Nas_AllocSub(&gLayerFreeList, &layer->listItem);
        Nas_EnvProcess(layer);
        seqChannel->layers[layerIndex] = NULL;
    }
}

void Nas_InitSubTrack(struct SequenceChannel* seqChannel) {
    s32 i;
    for (i = 0; i < LAYERS_MAX; i++) {
        ENV_RETRY(seqChannel, i);
    }

    Nas_InitList(&seqChannel->notePool);
    seqChannel->enabled = false;
    seqChannel->finished = true;
}

struct SequenceChannel* Nas_EntryNoteTrack(void) {
    s32 i;
    for (i = 0; i < SEQUENCE_CHANNELS; i++) {
        if (gSequenceChannels[i].seqPlayer == NULL) {
            return &gSequenceChannels[i];
        }
    }
    return &gSequenceChannelNone;
}

void Nas_ReleaseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits) {
    struct SequenceChannel* seqChannel;
    s32 i;

    for (i = 0; i < CHANNELS_MAX; i++) {
        if (channelBits & 1) {
            seqChannel = seqPlayer->channels[i];
            if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) == true && seqChannel->seqPlayer == seqPlayer) {
                Nas_InitSubTrack(seqChannel);
                seqChannel->seqPlayer = NULL;
            }
            seqChannel = Nas_EntryNoteTrack();
            if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) == false) {
                // eu_stubbed_printf_0("Audio:Track:Warning: No Free Notetrack\n");
                gAudioErrorFlags = i + 0x10000;
                seqPlayer->channels[i] = seqChannel;
            } else {
                Nas_ChannelModInit(seqChannel);
                seqPlayer->channels[i] = seqChannel;
                seqChannel->seqPlayer = seqPlayer;
                seqChannel->bankId = seqPlayer->defaultBank[0];
                seqChannel->muteBehavior = seqPlayer->muteBehavior;
                seqChannel->noteAllocPolicy = seqPlayer->noteAllocPolicy;
            }
        }
        channelBits = channelBits >> 1;
    }
}

void Nas_CloseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits) {
    struct SequenceChannel* seqChannel;
    s32 i;

    // eu_stubbed_printf_0("SUBTRACK DIM\n");
    for (i = 0; i < CHANNELS_MAX; i++) {
        if (channelBits & 1) {
            seqChannel = seqPlayer->channels[i];
            if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) == true) {
                if (seqChannel->seqPlayer == seqPlayer) {
                    Nas_InitSubTrack(seqChannel);
                    seqChannel->seqPlayer = NULL;
                } else {
                    // stubbed_printf("Audio:Track: Warning SUBTRACK PARENT CHANGED\n");
                }
                seqPlayer->channels[i] = &gSequenceChannelNone;
            }
        }
        channelBits = channelBits >> 1;
    }
}

void Nas_ReleaseSubTrack(struct SequencePlayer* seqPlayer, u8 channelIndex, void* script) {
    struct SequenceChannel* seqChannel = seqPlayer->channels[channelIndex];
    s32 i;
    if (IS_SEQUENCE_CHANNEL_VALID(seqChannel) == false) {
        // stubbed_printf("SEQID %d,BANKID %d\n", seqPlayer->seqId, seqPlayer->defaultBank[0]);
        // stubbed_printf("ERR:SUBTRACK %d NOT ALLOCATED\n", channelIndex);
    } else {
        seqChannel->enabled = true;
        seqChannel->finished = false;
        seqChannel->scriptState.depth = 0;
        seqChannel->scriptState.pc = script;
        seqChannel->delay = 0;
        for (i = 0; i < LAYERS_MAX; i++) {
            if (seqChannel->layers[i] != NULL) {
                ENV_RETRY(seqChannel, i);
            }
        }
    }
}

void Nas_GetNewSubTrack(struct SequencePlayer* seqPlayer) {
    Nas_CloseNoteTrack(seqPlayer, 0xffff);
    Nas_InitList(&seqPlayer->notePool);
    seqPlayer->finished = true;
    seqPlayer->enabled = false;

    if (IS_SEQ_LOAD_COMPLETE(seqPlayer->seqId) && gSeqLoadStatus[seqPlayer->seqId] != 5) {
        gSeqLoadStatus[seqPlayer->seqId] = SOUND_LOAD_STATUS_DISCARDABLE;
    }

    if (IS_BANK_LOAD_COMPLETE(seqPlayer->defaultBank[0]) && gBankLoadStatus[seqPlayer->defaultBank[0]] != 5) {
        gBankLoadStatus[seqPlayer->defaultBank[0]] = 4;
    }

    // (Note that if this is called from Nas_SzAutoDivide, the side will get swapped
    // later in that function. Thus, we signal that we want to load into the slot
    // of the bank that we no longer need.)
    if (seqPlayer->defaultBank[0] == gBankLoadedPool.temporary.entries[0].id) {
        gBankLoadedPool.temporary.nextSide = 1;
    } else if (seqPlayer->defaultBank[0] == gBankLoadedPool.temporary.entries[1].id) {
        gBankLoadedPool.temporary.nextSide = 0;
    }
}

void Nas_AllocSub(struct AudioListItem* list, struct AudioListItem* item) {
    if (item->prev != NULL) {
        // eu_stubbed_printf_0("Error:Same List Add\n");
    } else {
        list->prev->next = item;
        item->prev = list->prev;
        item->next = list;
        list->prev = item;
        list->u.count++;
        item->pool = list->pool;
    }
}

void* Nas_DeAllocSub(struct AudioListItem* list) {
    struct AudioListItem* item = list->prev;
    if (item == list) {
        return NULL;
    }
    item->prev->next = list;
    list->prev = item->prev;
    item->prev = NULL;
    list->u.count--;
    return item->u.value;
}

void Nas_OpenSub(void) {
    s32 i;

    gLayerFreeList.prev = &gLayerFreeList;
    gLayerFreeList.next = &gLayerFreeList;
    gLayerFreeList.u.count = 0;
    gLayerFreeList.pool = NULL;

    for (i = 0; i < ARRAY_COUNT(gSequenceLayers); i++) {
        gSequenceLayers[i].listItem.u.value = &gSequenceLayers[i];
        gSequenceLayers[i].listItem.prev = NULL;
        Nas_AllocSub(&gLayerFreeList, &gSequenceLayers[i].listItem);
    }
}

u8 Nas_ReleaseGroup(struct M64ScriptState* state) {
    return *(state->pc++);
}

s16 Nas_AddList(struct M64ScriptState* state) {
    s16 ret = *(state->pc++) << 8;
    ret = *(state->pc++) | ret;
    return ret;
}

u16 Nas_GetList(struct M64ScriptState* state) {
    u16 ret = *(state->pc++);
    if (ret & 0x80) {
        ret = (ret << 8) & 0x7f00;
        ret = *(state->pc++) | ret;
    }
    return ret;
}

void Nas_InitNoteList(struct SequenceChannelLayer* layer) {
    struct SequencePlayer* seqPlayer;
    struct SequenceChannel* seqChannel;
    UNUSED u32 pad0;
    struct M64ScriptState* state;
    struct Portamento* portamento;
    struct AudioBankSound* sound;
    struct Instrument* instrument;
    struct Drum* drum;
    s32 temp_a0_5;
    u16 sp3A;
    s32 sameSound;
    UNUSED u32 pad1;
    u8 cmd;
    UNUSED u8 cmdSemitone;
    f32 tuning;
    s32 vel;
    UNUSED s32 usedSemitone;
    f32 freqScale;
    f32 temp_f12;
    f32 temp_f2;

    sameSound = true;
    if (layer->enabled == false) {
        return;
    }

    if (layer->delay > 1) {
        layer->delay--;
        if (!layer->stopSomething && layer->delay <= layer->duration) {
            L_CheckTerminate(layer);
            layer->stopSomething = true;
        }
        return;
    }

    if (!layer->continuousNotes) {
        L_CheckTerminate(layer);
    }
#ifdef VERSION_EU_V10
    else {
        if ((layer->note != 0) && (layer == layer->note->wantedParentLayer)) {
            L_CheckTerminate(layer);
        }
    }
#endif

    if (PORTAMENTO_MODE(layer->portamento) == PORTAMENTO_MODE_1 ||
        PORTAMENTO_MODE(layer->portamento) == PORTAMENTO_MODE_2) {
        layer->portamento.mode = 0;
    }

    seqChannel = layer->seqChannel;
    seqPlayer = seqChannel->seqPlayer;
    layer->notePropertiesNeedInit = true;

    for (;;) {
        state = &layer->scriptState;
        cmd = Nas_ReleaseGroup(state);

        if (cmd <= 0xc0) {
            break;
        }

        switch (cmd) {
            case 0xff: // layer_end; function return or end of script
                if (state->depth == 0) {
                    // N.B. this function call is *not* inlined even though it's
                    // within the same file, unlike in the rest of this function.
                    Nas_EnvProcess(layer);
                    return;
                }
                state->pc = state->stack[--state->depth];
                break;

            case 0xfc: // layer_call
                if (0 && state->depth >= 4) {}
                sp3A = Nas_AddList(state);
                state->stack[state->depth++] = state->pc;
                state->pc = seqPlayer->seqData + sp3A;
                break;

            case 0xf8: // layer_loop; loop start, N iterations (or 256 if N = 0)
                if (0 && state->depth >= 4) {}
                state->remLoopIters[state->depth] = Nas_ReleaseGroup(state);
                state->stack[state->depth++] = state->pc;
                break;

            case 0xf7: // layer_loopend
                if (--state->remLoopIters[state->depth - 1] != 0) {
                    state->pc = state->stack[state->depth - 1];
                } else {
                    state->depth--;
                }
                break;

            case 0xfb: // layer_jump
                sp3A = Nas_AddList(state);
                state->pc = seqPlayer->seqData + sp3A;
                break;

            case 0xf4:
                state->pc += (s8) Nas_ReleaseGroup(state);
                break;

            case 0xc1: // layer_setshortnotevelocity
            case 0xca: // layer_setpan
                temp_a0_5 = *(state->pc++);
                if (cmd == 0xc1) {
                    layer->velocitySquare = (f32) (temp_a0_5 * temp_a0_5) / 16129.0f;
                } else {
                    layer->pan = temp_a0_5;
                }
                break;

            case 0xc2: // layer_transpose; set transposition in semitones
            case 0xc9: // layer_setshortnoteduration
                temp_a0_5 = *(state->pc++);
                if (cmd == 0xc9) {
                    layer->noteDuration = temp_a0_5;
                } else {
                    layer->transposition = temp_a0_5;
                }
                break;

            case 0xc4: // layer_somethingon
            case 0xc5: // layer_somethingoff
                if (cmd == 0xc4) {
                    layer->continuousNotes = true;
                } else {
                    layer->continuousNotes = false;
                }
                L_CheckTerminate(layer);
                break;

            case 0xc3: // layer_setshortnotedefaultplaypercentage
                sp3A = Nas_GetList(state);
                layer->shortNoteDefaultPlayPercentage = sp3A;
                break;

            case 0xc6: // layer_setinstr
                cmd = Nas_ReleaseGroup(state);
                if (cmd >= 0x7f) {
                    if (cmd == 0x7f) {
                        layer->instOrWave = 0;
                    } else {
                        layer->instOrWave = cmd;
                        layer->instrument = NULL;
                    }

                    if (1) {}

                    if (cmd == 0xff) {
                        layer->adsr.releaseRate = 0;
                    }
                    break;
                }

                if ((layer->instOrWave = Nas_ReadByteData(seqChannel, cmd, &layer->instrument, &layer->adsr)) == 0) {
                    layer->instOrWave = 0xff;
                }
                break;

            case 0xc7: // layer_portamento
                layer->portamento.mode = Nas_ReleaseGroup(state);

                // cmd is reused for the portamento's semitone
                cmd = Nas_ReleaseGroup(state) + seqChannel->transposition + layer->transposition + seqPlayer->transposition;

                if (cmd >= 0x80) {
                    cmd = 0;
                }

                layer->portamentoTargetNote = cmd;

                // If special, the next param is u8 instead of var
                if (PORTAMENTO_IS_SPECIAL(layer->portamento)) {
                    layer->portamentoTime = *((state)->pc++);
                    break;
                }

                sp3A = Nas_GetList(state);
                layer->portamentoTime = sp3A;
                break;

            case 0xc8: // layer_disableportamento
                layer->portamento.mode = 0;
                break;

            case 0xcb:
                sp3A = Nas_AddList(state);
                layer->adsr.envelope = (struct AdsrEnvelope*) (seqPlayer->seqData + sp3A);
                layer->adsr.releaseRate = Nas_ReleaseGroup(state);
                break;

            case 0xcc:
                layer->ignoreDrumPan = true;
                break;

            default:
                switch (cmd & 0xf0) {
                    case 0xd0: // layer_setshortnotevelocityfromtable
                        sp3A = seqPlayer->shortNoteVelocityTable[cmd & 0xf];
                        layer->velocitySquare = (f32) (sp3A * sp3A) / 16129.0f;
                        break;
                    case 0xe0: // layer_setshortnotedurationfromtable
                        layer->noteDuration = seqPlayer->shortNoteDurationTable[cmd & 0xf];
                        break;
                    default:
                        break;
                }
        }
    }

    if (cmd == 0xc0) { // layer_delay
        layer->delay = Nas_GetList(state);
        layer->stopSomething = true;
    } else {
        layer->stopSomething = false;

        if (seqChannel->largeNotes == true) {
            switch (cmd & 0xc0) {
                case 0x00: // layer_note0 (play percentage, velocity, duration)
                    sp3A = Nas_GetList(state);
                    vel = *(state->pc++);
                    layer->noteDuration = *(state->pc++);
                    layer->playPercentage = sp3A;
                    break;

                case 0x40: // layer_note1 (play percentage, velocity)
                    sp3A = Nas_GetList(state);
                    vel = *(state->pc++);
                    layer->noteDuration = 0;
                    layer->playPercentage = sp3A;
                    break;

                case 0x80: // layer_note2 (velocity, duration; uses last play percentage)
                    sp3A = layer->playPercentage;
                    vel = *(state->pc++);
                    layer->noteDuration = *(state->pc++);
                    break;
            }
            if ((vel >= 0x80) || (vel < 0)) {
                vel = 0x0000007F;
            }
            // the remaining bits are used for the semitone
            cmd -= (cmd & 0xc0);
            layer->velocitySquare = ((f32) (vel) * (f32) vel) / 16129.0f;
        } else {
            switch (cmd & 0xc0) {
                case 0x00: // play note, type 0 (play percentage)
                    sp3A = Nas_GetList(state);
                    layer->playPercentage = sp3A;
                    break;

                case 0x40: // play note, type 1 (uses default play percentage)
                    sp3A = layer->shortNoteDefaultPlayPercentage;
                    break;

                case 0x80: // play note, type 2 (uses last play percentage)
                    sp3A = layer->playPercentage;
                    break;
            }

            // the remaining bits are used for the semitone
            cmd -= cmd & 0xc0;
        }

        layer->delay = sp3A;
        layer->duration = layer->noteDuration * sp3A >> 8;
        if ((seqPlayer->muted && (seqChannel->muteBehavior & MUTE_BEHAVIOR_STOP_NOTES) != 0) ||
            seqChannel->stopSomething2) {
            layer->stopSomething = true;

        } else {
            s32 temp = layer->instOrWave;
            if (temp == 0xff) {
                if (!seqChannel->hasInstrument) {
                    return;
                }
                temp = seqChannel->instOrWave;
            }
            if (temp == 0) { // drum
                // cmd is reused for the drum semitone
                cmd += seqChannel->transposition + layer->transposition;

                drum = PercToPp(seqChannel->bankId, cmd);
                if (drum == NULL) {
                    layer->stopSomething = true;
                    layer->delayUnused = layer->delay;
                    return;
                } else {
                    layer->adsr.envelope = drum->envelope;
                    layer->adsr.releaseRate = drum->releaseRate;
                    if (!layer->ignoreDrumPan) {
                        layer->pan = drum->pan;
                    }
                    layer->sound = &drum->sound;
                    layer->freqScale = layer->sound->tuning;
                }
            } else { // instrument
                // cmd is reused for the instrument semitone
                cmd += seqPlayer->transposition + seqChannel->transposition + layer->transposition;

                if (cmd >= 0x80) {
                    layer->stopSomething = true;
                } else {
                    if (layer->instOrWave == 0xffu) {
                        instrument = seqChannel->instrument;
                    } else {
                        instrument = layer->instrument;
                    }

                    if (layer->portamento.mode != 0) {
                        if (layer->portamentoTargetNote < cmd) {
                            vel = cmd;
                        } else {
                            vel = layer->portamentoTargetNote;
                        }

                        if (instrument != NULL) {
                            sound = NoteToVoice(instrument, vel);
                            sameSound = (sound == layer->sound);
                            layer->sound = sound;
                            tuning = sound->tuning;
                        } else {
                            layer->sound = NULL;
                            tuning = 1.0f;
                        }

                        temp_f2 = gNoteFrequencies[cmd] * tuning;
                        temp_f12 = gNoteFrequencies[layer->portamentoTargetNote] * tuning;

                        portamento = &layer->portamento;
                        switch (PORTAMENTO_MODE(layer->portamento)) {
                            case PORTAMENTO_MODE_1:
                            case PORTAMENTO_MODE_3:
                            case PORTAMENTO_MODE_5:
                                freqScale = temp_f12;
                                break;

                            case PORTAMENTO_MODE_2:
                            case PORTAMENTO_MODE_4:
                            default:
                                freqScale = temp_f2;
                                break;
                        }

                        portamento->extent = temp_f2 / freqScale - 1.0f;

                        if (PORTAMENTO_IS_SPECIAL(layer->portamento)) {
                            portamento->speed = US_FLOAT(32512.0) * FLOAT_CAST(seqPlayer->tempo) /
                                                ((f32) layer->delay * (f32) gTempoInternalToExternal *
                                                 FLOAT_CAST(layer->portamentoTime));
                        } else {
                            portamento->speed = US_FLOAT(127.0) / FLOAT_CAST(layer->portamentoTime);
                        }
                        portamento->cur = 0.0f;
                        layer->freqScale = freqScale;
                        if (PORTAMENTO_MODE(layer->portamento) == PORTAMENTO_MODE_5) {
                            layer->portamentoTargetNote = cmd;
                        }
                    } else if (instrument != NULL) {
                        sound = NoteToVoice(instrument, cmd);
                        sameSound = (sound == layer->sound);
                        layer->sound = sound;
                        layer->freqScale = gNoteFrequencies[cmd] * sound->tuning;
                    } else {
                        layer->sound = NULL;
                        layer->freqScale = gNoteFrequencies[cmd];
                    }
                }
            }
            layer->delayUnused = layer->delay;
        }
    }

    if (layer->stopSomething == true) {
        if (layer->note != NULL || layer->continuousNotes) {
            L_CheckTerminate(layer);
        }
        return;
    }

    cmd = false;
    if (!layer->continuousNotes) {
        cmd = true;
    } else if (layer->note == NULL || layer->status == SOUND_LOAD_STATUS_NOT_LOADED) {
        cmd = true;
    } else if (sameSound == false) {
        L_CheckTerminate(layer);
        cmd = true;
    } else if (layer != layer->note->parentLayer) {
        cmd = true;
    } else if (layer->sound == NULL) {
        Nas_Release_Channel(layer->note, layer);
    }

    if (cmd != false) {
        layer->note = Nas_ChLookFree(layer);
    }

    if (layer->note != NULL && layer->note->parentLayer == layer) {
        Nas_ModTableRead(layer->note);
    }
    if (seqChannel) {}
}

u8 Nas_ReadByteData(struct SequenceChannel* seqChannel, u8 instId, struct Instrument** instOut,
                  struct AdsrSettings* adsr) {
    struct Instrument* inst;
    inst = ProgToVp(seqChannel->bankId, instId);
    if (inst == NULL) {
        *instOut = NULL;
        return 0;
    }
    adsr->envelope = inst->envelope;
    adsr->releaseRate = inst->releaseRate;
    *instOut = inst;
    instId++;
    return instId;
}

void Nas_ReadWordData(struct SequenceChannel* seqChannel, u8 instId) {
    if (instId >= 0x80) {
        seqChannel->instOrWave = instId;
        seqChannel->instrument = NULL;
    } else if (instId == 0x7f) {
        seqChannel->instOrWave = 0;
        seqChannel->instrument = (struct Instrument*) 1;
    } else {
        if ((seqChannel->instOrWave = Nas_ReadByteData(seqChannel, instId, &seqChannel->instrument, &seqChannel->adsr)) ==
            0) {
            seqChannel->hasInstrument = false;
            return;
        }
    }
    seqChannel->hasInstrument = true;
}

void Nas_ReadLengthData(struct SequenceChannel* seqChannel, u8 volume) {
    seqChannel->volume = FLOAT_CAST(volume) / US_FLOAT(127.0);
}

void Nas_NoteSeq(struct SequenceChannel* seqChannel) {
    struct M64ScriptState* state;
    struct SequencePlayer* seqPlayer;
    u8 cmd;
    s8 temp;
    u8 loBits;
    u16 sp5A;
    s32 sp38;
    s8 value;
    s32 i;
    u8* seqData;

    if (!seqChannel->enabled) {
        return;
    }
    if (seqChannel->stopScript) {
        for (i = 0; i < LAYERS_MAX; i++) {
            if (seqChannel->layers[i] != NULL) {
                Nas_InitNoteList(seqChannel->layers[i]);
            }
        }
        return;
    }
    seqPlayer = seqChannel->seqPlayer;
    if (seqPlayer->muted && ((seqChannel->muteBehavior & 0x80) != 0)) {
        return;
    }
    if (seqChannel->delay != 0) {
        seqChannel->delay--;
    }
    state = &seqChannel->scriptState;
    if (seqChannel->delay == 0) {
        for (;;) {
            cmd = Nas_ReleaseGroup(state);
            if (cmd > 0xc0) {
                switch (cmd) {
                    case 0xFF:
                        if (state->depth) {};
                        if (1) {};
                        if (1) {};
                        if (1) {};
                        if (state->depth == 0) {
                            Nas_InitSubTrack(seqChannel);
                            goto out;
                        } else {
                            state->pc = state->stack[--state->depth];
                        }
                        break;

                    case 0xFE:
                        goto out;

                    case 0xFD:
                        seqChannel->delay = Nas_GetList(state);
                        goto out;

                    case 0xEA:
                        seqChannel->stopScript = 1;
                        goto out;

                    case 0xFC:
                        sp5A = Nas_AddList(state);
                        state->stack[state->depth++] = state->pc;
                        state->pc = seqPlayer->seqData + sp5A;
                        break;

                    case 0xF8:
                        state->remLoopIters[state->depth] = Nas_ReleaseGroup(state);
                        state->stack[state->depth] = state->pc;
                        state->depth += 1;
                        break;

                    case 0xF7:
                        state->remLoopIters[state->depth - 1]--;
                        if (state->remLoopIters[state->depth - 1] != 0) {
                            state->pc = state->stack[state->depth - 1];
                        } else {
                            state->depth--;
                        }
                        break;

                    case 0xF6:
                        state->depth -= 1;
                        break;

                    case 0xF5:
                    case 0xF9:
                    case 0xFA:
                    case 0xFB:
                        sp5A = Nas_AddList(state);
                        if ((cmd == 0xFA) && (value != 0)) {
                            break;
                        }
                        if ((cmd == 0xF9) && (value >= 0)) {
                            break;
                        }
                        if ((cmd == 0xF5) && (value < 0)) {
                            break;
                        }
                        state->pc = seqPlayer->seqData + sp5A;
                        break;

                    case 0xF2:
                    case 0xF3:
                    case 0xF4:
                        temp = Nas_ReleaseGroup(state);
                        if ((cmd == 0xF3) && (value != 0)) {
                            break;
                        }
                        if ((cmd == 0xF2) && (value >= 0)) {
                            break;
                        }
                        state->pc += temp;
                        break;

                    case 0xF1:
                        Nas_InitList(&seqChannel->notePool);
                        Nas_InitChNode(&seqChannel->notePool, Nas_ReleaseGroup(state));
                        break;

                    case 0xF0:
                        Nas_InitList(&seqChannel->notePool);
                        break;

                    case 0xC2:
                        sp5A = Nas_AddList(state);
                        seqChannel->dynTable = (void*) (seqPlayer->seqData + sp5A);
                        break;

                    case 0xC5:
                        if (value != (-1)) {
                            seqData = (*seqChannel->dynTable)[value];
                            sp38 = (u16) ((seqData[0] << 8) + seqData[1]);
                            seqChannel->dynTable = (void*) (seqPlayer->seqData + sp38);
                        }
                        break;

                    case 0xEB:
                        cmd = Nas_ReleaseGroup(state);
                        sp38 = ((u16*) gAlBankSets)[seqPlayer->seqId];
                        loBits = *(sp38 + gAlBankSets);
                        cmd = gAlBankSets[(((s32) sp38) + loBits) - cmd];
                        if (Nas_SzHeapAlloc(1, 2, cmd) != NULL) {
                            seqChannel->bankId = cmd;
                        }

                    case 0xC1:
                        Nas_ReadWordData(seqChannel, Nas_ReleaseGroup(state));
                        break;

                    case 0xC3:
                        seqChannel->largeNotes = 0;
                        break;

                    case 0xC4:
                        seqChannel->largeNotes = 1;
                        break;

                    case 0xDF:
                        Nas_ReadLengthData(seqChannel, Nas_ReleaseGroup(state));
                        seqChannel->changes.as_bitfields.volume = 1;
                        break;

                    case 0xE0:
                        seqChannel->volumeScale = ((f32) ((s32) Nas_ReleaseGroup(state))) / 128.0f;
                        seqChannel->changes.as_bitfields.volume = 1;
                        break;

                    case 0xDE:
                        sp5A = Nas_AddList(state);
                        seqChannel->freqScale = ((f32) ((s32) sp5A)) / 32768.0f;
                        seqChannel->changes.as_bitfields.freqScale = 1;
                        break;

                    case 0xD3:
                        cmd = Nas_ReleaseGroup(state) + 127;
                        seqChannel->freqScale = gPitchBendFrequencyScale[cmd];
                        seqChannel->changes.as_bitfields.freqScale = 1;
                        break;

                    case 0xDD:
                        seqChannel->newPan = Nas_ReleaseGroup(state);
                        seqChannel->changes.as_bitfields.pan = 1;
                        break;

                    case 0xDC:
                        seqChannel->panChannelWeight = Nas_ReleaseGroup(state);
                        seqChannel->changes.as_bitfields.pan = 1;
                        break;

                    case 0xDB:
                        temp = *(state->pc++);
                        seqChannel->transposition = temp;
                        break;

                    case 0xDA:
                        sp5A = Nas_AddList(state);
                        seqChannel->adsr.envelope = (struct AdsrEnvelope*) (seqPlayer->seqData + sp5A);
                        break;

                    case 0xD9:
                        seqChannel->adsr.releaseRate = Nas_ReleaseGroup(state);
                        break;

                    case 0xD8:
                        seqChannel->vibratoExtentTarget = Nas_ReleaseGroup(state) * 8;
                        seqChannel->vibratoExtentStart = 0;
                        seqChannel->vibratoExtentChangeDelay = 0;
                        break;

                    case 0xD7:
                        seqChannel->vibratoRateStart = seqChannel->vibratoRateTarget = Nas_ReleaseGroup(state) * 32;
                        seqChannel->vibratoRateChangeDelay = 0;
                        break;

                    case 0xE2:
                        seqChannel->vibratoExtentStart = Nas_ReleaseGroup(state) * 8;
                        seqChannel->vibratoExtentTarget = Nas_ReleaseGroup(state) * 8;
                        seqChannel->vibratoExtentChangeDelay = Nas_ReleaseGroup(state) * 0x10;
                        break;

                    case 0xE1:
                        seqChannel->vibratoRateStart = Nas_ReleaseGroup(state) << 5;
                        seqChannel->vibratoRateTarget = Nas_ReleaseGroup(state) << 5;
                        seqChannel->vibratoRateChangeDelay = Nas_ReleaseGroup(state) * 0x10;
                        break;

                    case 0xE3:
                        seqChannel->vibratoDelay = Nas_ReleaseGroup(state) * 0x10;
                        break;

                    case 0xD4:
                        seqChannel->reverbVol = Nas_ReleaseGroup(state);
                        break;

                    case 0xC6:
                        cmd = Nas_ReleaseGroup(state);
                        sp5A = ((u16*) gAlBankSets)[seqPlayer->seqId];
                        loBits = *(sp5A + gAlBankSets);
                        cmd = gAlBankSets[(sp5A + loBits) - cmd];
                        if (Nas_SzHeapAlloc(1, 2, cmd) != NULL) {
                            seqChannel->bankId = cmd;
                        }
                        break;

                    case 0xC7:
                        cmd = Nas_ReleaseGroup(state);
                        sp5A = Nas_AddList(state);
                        seqData = seqPlayer->seqData + sp5A;
                        *seqData = ((u8) value) + cmd;
                        break;

                    case 0xC8:
                    case 0xC9:
                    case 0xCC:
                        temp = Nas_ReleaseGroup(state);
                        if (cmd == 0xC8) {
                            value -= temp;
                        } else if (cmd == 0xCC) {
                            value = temp;
                        } else {
                            value &= temp;
                        }
                        break;

                    case 0xCA:
                        seqChannel->muteBehavior = Nas_ReleaseGroup(state);
                        break;

                    case 0xCB:
                        sp38 = ((u16) Nas_AddList(state)) + value;
                        value = seqPlayer->seqData[sp38];
                        break;

                    case 0xD0:
                        seqChannel->stereoHeadsetEffects = Nas_ReleaseGroup(state);
                        break;

                    case 0xD1:
                        seqChannel->noteAllocPolicy = Nas_ReleaseGroup(state);
                        break;

                    case 0xD2:
                        seqChannel->adsr.sustain = Nas_ReleaseGroup(state);
                        break;

                    case 0xE5:
                        seqChannel->reverbIndex = Nas_ReleaseGroup(state);
                        break;

                    case 0xE4:
                        if (value != (-1)) {
                            if (state->depth) {};
                            seqData = (*seqChannel->dynTable)[value];
                            state->stack[state->depth++] = state->pc;
                            sp38 = (u16) ((seqData[0] << 8) + seqData[1]);
                            state->pc = seqPlayer->seqData + sp38;
                        }
                        break;

                    case 0xE6:
                        seqChannel->bookOffset = Nas_ReleaseGroup(state);
                        break;

                    case 0xE7:
                        sp5A = Nas_AddList(state);
                        seqData = seqPlayer->seqData + sp5A;
                        seqChannel->muteBehavior = *(seqData++);
                        seqChannel->noteAllocPolicy = *(seqData++);
                        seqChannel->notePriority = *(seqData++);
                        seqChannel->transposition = (s8) (*(seqData++));
                        seqChannel->newPan = *(seqData++);
                        seqChannel->panChannelWeight = *(seqData++);
                        seqChannel->reverbVol = *(seqData++);
                        seqChannel->reverbIndex = *(seqData++);
                        seqChannel->changes.as_bitfields.pan = 1;
                        break;

                    case 0xE8:
                        seqChannel->muteBehavior = Nas_ReleaseGroup(state);
                        seqChannel->noteAllocPolicy = Nas_ReleaseGroup(state);
                        seqChannel->notePriority = Nas_ReleaseGroup(state);
                        seqChannel->transposition = (s8) Nas_ReleaseGroup(state);
                        seqChannel->newPan = Nas_ReleaseGroup(state);
                        seqChannel->panChannelWeight = Nas_ReleaseGroup(state);
                        seqChannel->reverbVol = Nas_ReleaseGroup(state);
                        seqChannel->reverbIndex = Nas_ReleaseGroup(state);
                        seqChannel->changes.as_bitfields.pan = 1;
                        break;

                    case 0xEC:
                        seqChannel->vibratoExtentTarget = 0;
                        seqChannel->vibratoExtentStart = 0;
                        seqChannel->vibratoExtentChangeDelay = 0;
                        seqChannel->vibratoRateTarget = 0;
                        seqChannel->vibratoRateStart = 0;
                        seqChannel->vibratoRateChangeDelay = 0;
                        seqChannel->freqScale = 1.0f;
                        break;

                    case 0xE9:
                        seqChannel->notePriority = Nas_ReleaseGroup(state);
                        break;

                    case 0xEF:
                        Nas_AddList(state);
                        Nas_ReleaseGroup(state);
                        break;
                }
            } else {
                loBits = cmd & 0xf;
                switch (cmd & 0xF0) {
                    case 0x0:
                        if (seqChannel->layers[loBits] != NULL) {
                            value = seqChannel->layers[loBits]->finished;
                        } else {
                            value = -1;
                        }
                        break;

                    case 0x70:
                        seqChannel->soundScriptIO[loBits] = value;
                        break;

                    case 0x80:
                        value = seqChannel->soundScriptIO[loBits];
                        if (loBits < 4) {
                            seqChannel->soundScriptIO[loBits] = -1;
                        }
                        break;

                    case 0x50:
                        value -= seqChannel->soundScriptIO[loBits];
                        break;

                    case 0x60:
                        seqChannel->delay = loBits;
                        goto out;

                    case 0x90:
                        sp5A = Nas_AddList(state);
                        if (Nas_EnvInit(seqChannel, loBits) == 0) {
                            seqChannel->layers[loBits]->scriptState.pc = seqPlayer->seqData + sp5A;
                        }
                        break;

                    case 0xA0:
                        ENV_RETRY(seqChannel, loBits);
                        break;

                    case 0xB0:
                        if ((value != (-1)) && (Nas_EnvInit(seqChannel, loBits) != (-1))) {
                            seqData = (*seqChannel->dynTable)[value];
                            sp5A = (seqData[0] << 8) + seqData[1];
                            seqChannel->layers[loBits]->scriptState.pc = seqPlayer->seqData + sp5A;
                        }
                        break;

                    case 0x10:
                        sp5A = Nas_AddList(state);
                        Nas_ReleaseSubTrack(seqPlayer, loBits, seqPlayer->seqData + sp5A);
                        break;

                    case 0x20:
                        Nas_InitSubTrack(seqPlayer->channels[loBits]);
                        break;

                    case 0x30:
                        cmd = Nas_ReleaseGroup(state);
                        seqPlayer->channels[loBits]->soundScriptIO[cmd] = value;
                        break;

                    case 0x40:
                        cmd = Nas_ReleaseGroup(state);
                        value = seqPlayer->channels[loBits]->soundScriptIO[cmd];
                        break;
                }
            }
        }
    }
out:
    for (i = 0; i < LAYERS_MAX; i++) {
        if (seqChannel->layers[i] != 0) {
            Nas_InitNoteList(seqChannel->layers[i]);
        }
    }
}

void ERROR_EXIT(struct SequencePlayer* seqPlayer) {
    u8 cmd;
    u8 loBits;
    u8 temp;
    s32 value;
    s32 i;
    u16 u16v;
    u8* seqData;
    struct M64ScriptState* state;
    s32 temp32;

    if (seqPlayer->enabled == false) {
        return;
    }

    if (seqPlayer->bankDmaInProgress == true) {
        if (osRecvMesg(&seqPlayer->bankDmaMesgQueue, NULL, 0) == -1) {
            return;
        }
        if (seqPlayer->bankDmaRemaining == 0) {
            seqPlayer->bankDmaInProgress = false;
            Nas_smzBnkOfsToAdd(seqPlayer->loadingBankId);
            if (gBankLoadStatus[seqPlayer->loadingBankId] != 5) {
                gBankLoadStatus[seqPlayer->loadingBankId] = 2;
            }
        } else {
            Nas_SlowRomCopy(&seqPlayer->bankDmaCurrDevAddr, &seqPlayer->bankDmaCurrMemAddr,
                                         &seqPlayer->bankDmaRemaining, &seqPlayer->bankDmaMesgQueue,
                                         &seqPlayer->bankDmaIoMesg);
        }
        return;
    }

    if (seqPlayer->seqDmaInProgress == true) {
        if (osRecvMesg(&seqPlayer->seqDmaMesgQueue, NULL, 0) == -1) {
            return;
        }
        seqPlayer->seqDmaInProgress = false;
        if (gSeqLoadStatus[seqPlayer->seqId] != 5) {
            gSeqLoadStatus[seqPlayer->seqId] = 2;
        }
    }

    // If discarded, bail out.
    temp32 = 2; // I beg your pardon?
    if (IS_SEQ_LOAD_COMPLETE(seqPlayer->seqId) == false ||
        (IS_BANK_LOAD_COMPLETE(seqPlayer->defaultBank[0]) == false)) {
        Nas_GetNewSubTrack(seqPlayer);
        return;
    }

    // Remove possible SOUND_LOAD_STATUS_DISCARDABLE marks.
    if (gSeqLoadStatus[seqPlayer->seqId] != 5) {
        gSeqLoadStatus[seqPlayer->seqId] = temp32;
    }

    if (gBankLoadStatus[seqPlayer->defaultBank[0]] != 5) {
        gBankLoadStatus[seqPlayer->defaultBank[0]] = temp32;
    }

    if (seqPlayer->muted && (seqPlayer->muteBehavior & MUTE_BEHAVIOR_STOP_SCRIPT) != 0) {
        return;
    }

    // Check if we surpass the number of ticks needed for a tatum, else stop.
    seqPlayer->tempoAcc += seqPlayer->tempo;
    if (seqPlayer->tempoAcc < gTempoInternalToExternal) {
        return;
    }
    seqPlayer->tempoAcc -= (u16) gTempoInternalToExternal;

    state = &seqPlayer->scriptState;
    if (seqPlayer->delay > 1) {
        seqPlayer->delay--;
    } else {
        seqPlayer->recalculateVolume = 1;
        for (;;) {
            cmd = Nas_ReleaseGroup(state);
            if (cmd == 0xff) { // seq_end
                if (state->depth == 0) {
                    Nas_GetNewSubTrack(seqPlayer);
                    break;
                }
                state->pc = state->stack[--state->depth];
            }

            if (cmd == 0xfd) { // seq_delay
                seqPlayer->delay = Nas_GetList(state);
                break;
            }

            if (cmd == 0xfe) { // seq_delay1
                seqPlayer->delay = 1;
                break;
            }

            if (cmd >= 0xc0) {
                switch (cmd) {
                    case 0xff: // seq_end
                        break;

                    case 0xfc: // seq_call
                        u16v = Nas_AddList(state);
                        if (0 && state->depth >= 4) {}
                        state->stack[state->depth++] = state->pc;
                        state->pc = seqPlayer->seqData + u16v;
                        break;

                    case 0xf8: // seq_loop; loop start, N iterations (or 256 if N = 0)
                        if (0 && state->depth >= 4) {}
                        state->remLoopIters[state->depth] = Nas_ReleaseGroup(state);
                        state->stack[state->depth++] = state->pc;
                        break;

                    case 0xf7: // seq_loopend
                        state->remLoopIters[state->depth - 1]--;
                        if (state->remLoopIters[state->depth - 1] != 0) {
                            state->pc = state->stack[state->depth - 1];
                        } else {
                            state->depth--;
                        }
                        break;

                    case 0xfb: // seq_jump
                    case 0xfa: // seq_beqz; jump if == 0
                    case 0xf9: // seq_bltz; jump if < 0
                    case 0xf5: // seq_bgez; jump if >= 0
                        u16v = Nas_AddList(state);
                        if (cmd == 0xfa && value != 0) {
                            break;
                        }
                        if (cmd == 0xf9 && value >= 0) {
                            break;
                        }
                        if (cmd == 0xf5 && value < 0) {
                            break;
                        }
                        state->pc = seqPlayer->seqData + u16v;
                        break;

                    case 0xf4:
                    case 0xf3:
                    case 0xf2:
                        temp = Nas_ReleaseGroup(state);
                        if (cmd == 0xf3 && value != 0) {
                            break;
                        }
                        if (cmd == 0xf2 && value >= 0) {
                            break;
                        }
                        state->pc += (s8) temp;
                        break;

                    case 0xf1: // seq_reservenotes
                        Nas_InitList(&seqPlayer->notePool);
                        Nas_InitChNode(&seqPlayer->notePool, Nas_ReleaseGroup(state));
                        break;

                    case 0xf0: // seq_unreservenotes
                        Nas_InitList(&seqPlayer->notePool);
                        break;

                    case 0xdf: // seq_transpose; set transposition in semitones
                        seqPlayer->transposition = 0;
                        // fallthrough

                    case 0xde: // seq_transposerel; add transposition
                        seqPlayer->transposition += (s8) Nas_ReleaseGroup(state);
                        break;

                    case 0xdc:
                    case 0xdd: // seq_settempo (bpm)
                        temp = Nas_ReleaseGroup(state);
                        if (cmd == 0xdd) {
                            seqPlayer->tempo = temp * TEMPO_SCALE;
                        } else {
                            seqPlayer->tempo += (s8) temp * TEMPO_SCALE;
                        }

                        if (seqPlayer->tempo > gTempoInternalToExternal) {
                            seqPlayer->tempo = gTempoInternalToExternal;
                        }

                        // if (cmd) {}

                        if ((s16) seqPlayer->tempo <= 0) {
                            seqPlayer->tempo = 1;
                        }
                        break;

                    case 0xda:
                        cmd = Nas_ReleaseGroup(state);
                        u16v = Nas_AddList(state);
                        switch (cmd) {
                            case SEQUENCE_PLAYER_STATE_0:
                            case SEQUENCE_PLAYER_STATE_FADE_OUT:
                                if (seqPlayer->state != SEQUENCE_PLAYER_STATE_2) {
                                    seqPlayer->fadeTimerUnkEu = u16v;
                                    seqPlayer->state = cmd;
                                }
                                break;
                            case SEQUENCE_PLAYER_STATE_2:
                                seqPlayer->fadeRemainingFrames = u16v;
                                seqPlayer->state = cmd;
                                seqPlayer->fadeVelocity = (0.0f - seqPlayer->fadeVolume) / (s32) (u16v & 0xFFFFu);
                                break;
                        }
                        break;

                    case 0xdb:
                        temp32 = Nas_ReleaseGroup(state);
                        switch (seqPlayer->state) {
                            case SEQUENCE_PLAYER_STATE_2:
                                break;
                            case SEQUENCE_PLAYER_STATE_FADE_OUT:
                                seqPlayer->state = SEQUENCE_PLAYER_STATE_0;
                                seqPlayer->fadeVolume = 0.0f;
                                // fallthrough
                            case SEQUENCE_PLAYER_STATE_0:
                                seqPlayer->fadeRemainingFrames = seqPlayer->fadeTimerUnkEu;
                                if (seqPlayer->fadeTimerUnkEu != 0) {
                                    seqPlayer->fadeVelocity = (temp32 / 127.0f - seqPlayer->fadeVolume) /
                                                              FLOAT_CAST(seqPlayer->fadeRemainingFrames);
                                } else {
                                    seqPlayer->fadeVolume = temp32 / 127.0f;
                                }
                        }
                        break;

                    case 0xd9:
                        temp = Nas_ReleaseGroup(state);
                        seqPlayer->fadeVolumeScale = (s8) temp / 127.0f;
                        break;

                    case 0xd7: // seq_initchannels
                        u16v = Nas_AddList(state);
                        Nas_ReleaseNoteTrack(seqPlayer, u16v);
                        break;

                    case 0xd6: // seq_disablechannels
                        u16v = Nas_AddList(state);
                        Nas_CloseNoteTrack(seqPlayer, u16v);
                        break;

                    case 0xd5: // seq_setmutescale
                        temp = Nas_ReleaseGroup(state);
                        seqPlayer->muteVolumeScale = (f32) (s8) temp / US_FLOAT(127.0);
                        break;

                    case 0xd4: // seq_mute
                        seqPlayer->muted = true;
                        break;

                    case 0xd3: // seq_setmutebhv
                        seqPlayer->muteBehavior = Nas_ReleaseGroup(state);
                        break;

                    case 0xd2: // seq_setshortnotevelocitytable
                    case 0xd1: // seq_setshortnotedurationtable
                        u16v = Nas_AddList(state);
                        seqData = seqPlayer->seqData + u16v;
                        if (cmd == 0xd2) {
                            seqPlayer->shortNoteVelocityTable = seqData;
                        } else {
                            seqPlayer->shortNoteDurationTable = seqData;
                        }
                        break;

                    case 0xd0: // seq_setnoteallocationpolicy
                        seqPlayer->noteAllocPolicy = Nas_ReleaseGroup(state);
                        break;

                    case 0xcc: // seq_setval
                        value = Nas_ReleaseGroup(state);
                        break;

                    case 0xc9: // seq_bitand
                        value &= Nas_ReleaseGroup(state);
                        break;

                    case 0xc8: // seq_subtract
                        value = value - Nas_ReleaseGroup(state);
                        break;

                    default:
                        // eu_stubbed_printf_1("Group:Undefine upper C0h command (%x)\n", cmd);
                        break;
                }
            } else {
                loBits = cmd & 0xf;
                switch (cmd & 0xf0) {
                    case 0x00: // seq_testchdisabled
                        value = seqPlayer->channels[loBits]->finished;
                        break;
                    case 0x10:
                        break;
                    case 0x20:
                        break;
                    case 0x40:
                        break;
                    case 0x50: // seq_subvariation
                        value -= seqPlayer->seqVariationEu[0];
                        break;
                    case 0x60:
                        break;
                    case 0x70: // seq_setvariation
                        seqPlayer->seqVariationEu[0] = value;
                        break;
                    case 0x80: // seq_getvariation
                        value = seqPlayer->seqVariationEu[0];
                        break;
                    case 0x90: // seq_startchannel
                        u16v = Nas_AddList(state);
                        Nas_ReleaseSubTrack(seqPlayer, loBits, seqPlayer->seqData + u16v);
                        break;
                    case 0xa0:
                        break;

                    default:
                        // eu_stubbed_printf_0("Group:Undefined Command\n");
                        break;
                }
            }
        }
    }

    for (i = 0; i < CHANNELS_MAX; i++) {
        if (IS_SEQUENCE_CHANNEL_VALID(seqPlayer->channels[i]) == true) {
            Nas_NoteSeq(seqPlayer->channels[i]);
        }
    }
}

void Nas_ProgramChanger(UNUSED s32 iterationsRemaining) {
    s32 i;
    for (i = 0; i < SEQUENCE_PLAYERS; i++) {
        if (SeqPlayer[i].enabled == true) {
            ERROR_EXIT(&SeqPlayer[i]);
            Nas_AllocationOnRequest(&SeqPlayer[i]);
        }
    }
    Nas_UpdateChannel();
}

void Nas_SubVoiceSet(u32 player) {
    struct SequencePlayer* seqPlayer = &SeqPlayer[player];
    Nas_GetNewSubTrack(seqPlayer);
    seqPlayer->delay = 0;
    seqPlayer->state = 1;
    seqPlayer->fadeRemainingFrames = 0;
    seqPlayer->fadeTimerUnkEu = 0;
    seqPlayer->tempoAcc = 0;
    seqPlayer->tempo = 120 * TEMPO_SCALE; // 120 BPM
    seqPlayer->transposition = 0;
    seqPlayer->noteAllocPolicy = 0;
    seqPlayer->shortNoteVelocityTable = gDefaultShortNoteVelocityTable;
    seqPlayer->shortNoteDurationTable = gDefaultShortNoteDurationTable;
    seqPlayer->fadeVolume = 1.0f;
    seqPlayer->fadeVolumeScale = 1.0f;
    seqPlayer->fadeVelocity = 0.0f;
    seqPlayer->volume = 0.0f;
    seqPlayer->muteVolumeScale = 0.5f;
}

void Nas_SubVolumeSet(void) {
    // Initialization function, called from Nas_InitAudio
    s32 i, j;

    for (i = 0; i < ARRAY_COUNT(gSequenceChannels); i++) {
        gSequenceChannels[i].seqPlayer = NULL;
        gSequenceChannels[i].enabled = false;
        /**
         * @bug Size of wrong array. Zeroes out second half of gSequenceChannels[0],
         * all of gSequenceChannels[1..31], and part of gSequenceLayers[0].
         * However, this is only called at startup, so it's harmless.
         */
#ifdef AVOID_UB
#define LAYERS_SIZE LAYERS_MAX
#else
#define LAYERS_SIZE ARRAY_COUNT(gSequenceLayers)
#endif
        for (j = 0; j < LAYERS_SIZE; j++) {
            gSequenceChannels[i].layers[j] = NULL;
        }
    }

    Nas_OpenSub();

    for (i = 0; i < ARRAY_COUNT(gSequenceLayers); i++) {
        gSequenceLayers[i].seqChannel = NULL;
        gSequenceLayers[i].enabled = false;
    }

    for (i = 0; i < SEQUENCE_PLAYERS; i++) {
        for (j = 0; j < CHANNELS_MAX; j++) {
            SeqPlayer[i].channels[j] = &gSequenceChannelNone;
        }

        SeqPlayer[i].seqVariationEu[0] = -1;
        SeqPlayer[i].muteBehavior = MUTE_BEHAVIOR_STOP_SCRIPT | MUTE_BEHAVIOR_STOP_NOTES | MUTE_BEHAVIOR_SOFTEN;
        SeqPlayer[i].enabled = false;
        SeqPlayer[i].muted = false;
        SeqPlayer[i].bankDmaInProgress = false;
        SeqPlayer[i].seqDmaInProgress = false;
        Nas_WaveMemoryMake(&SeqPlayer[i].notePool);
        Nas_SubVoiceSet(i);
    }
}
