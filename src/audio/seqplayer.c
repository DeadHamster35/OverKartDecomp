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
 * that they are meant to be Used in some sort of printf Variant. But I don't
 * care to try and figure out which function gets which string(s)
 * So I've place them all here instead.
 * This means some printf stubs have been commented out to avoid duplication
 * of some strings (i.e SeqplayerUnusedString00)
 **/
char SeqplayerUnusedString00[] = "Audio:Track:Warning: No Free Notetrack\n";
char SeqplayerUnusedString01[] = "SUBTRACK DIM\n";
char SeqplayerUnusedString02[] = "Audio:Track: Warning :SUBTRACK had been stolen by other Group.\n";
char SeqplayerUnusedString03[] = "SEQID %d,BANKID %d\n";
char SeqplayerUnusedString04[] = "ERR:SUBTRACK %d NOT ALLOCATED\n";
char SeqplayerUnusedString05[] = "Error:Same List Add\n";
#ifdef VERSION_EU_V10
char seqplayer_unused_string_eu_01[] = "Wait Time out!\n";
#endif
char SeqplayerUnusedString06[] = "Macro Level Over Error!\n";
char SeqplayerUnusedString07[] = "Macro Level Over Error!\n";
char SeqplayerUnusedString08[] = "WARNING: NPRG: cannot change %d\n";
char SeqplayerUnusedString09[] = "Audio:Track:NOTE:UNDEFINED NOTE COM. %x\n";
char SeqplayerUnusedString10[] = "Audio: Note:Velocity Error %d\n";
char SeqplayerUnusedString11[] = "Error: Subtrack no prg.\n";
char SeqplayerUnusedString12[] = "ERR %x\n";
char SeqplayerUnusedString13[] = "Error: Your assignchannel is stolen.\n";
char SeqplayerUnusedString14[] = "Audio:Track :Call Macro Level Over Error!\n";
char SeqplayerUnusedString15[] = "Audio:Track :Loops Macro Level Over Error!\n";
char SeqplayerUnusedString16[] = "SUB:ERR:BANK %d NOT CACHED.\n";
char SeqplayerUnusedString17[] = "SUB:ERR:BANK %d NOT CACHED.\n";
char SeqplayerUnusedString18[] = "Audio:Track: CTBLCALL Macro Level Over Error!\n";
char SeqplayerUnusedString19[] = "[%2x] \n";
char SeqplayerUnusedString20[] = "Err :Sub %x ,address %x:Undefined SubTrack Function %x";
char SeqplayerUnusedString21[] = "Disappear Sequence or Bank %d\n";
char SeqplayerUnusedString22[] = "Macro Level Over Error!\n";
char SeqplayerUnusedString23[] = "Macro Level Over Error!\n";
char SeqplayerUnusedString24[] = "Group:Undefine upper C0h command (%x)\n";
char SeqplayerUnusedString25[] = "Group:Undefined Command\n";

// NasChannelModInit
void NasChannelModInit(struct SequenceChannel* SEQChannel) {
    s32 i;

    SEQChannel->Enabled = false;
    SEQChannel->Finished = false;
    SEQChannel->stopScript = false;
    SEQChannel->stopSomething2 = false;
    SEQChannel->hasInstrument = false;
    SEQChannel->StereoHeadsetEffects = false;
    SEQChannel->Transposition = 0;
    SEQChannel->largeNotes = false;
    SEQChannel->bookOffset = 0;
    SEQChannel->Changes.AsU8 = 0xff;
    SEQChannel->ScriptState.Depth = 0;
    SEQChannel->newPan = 0x40;
    SEQChannel->panChannelWeight = 0x80;
    SEQChannel->noteUnused = NULL;
    SEQChannel->reverbIndex = 0;
    SEQChannel->ReverbVol = 0;
    SEQChannel->notePriority = NOTE_PRIORITY_DEFAULT;
    SEQChannel->Delay = 0;
    SEQChannel->Adsr.Envelope = DefaultEnvelope;
    SEQChannel->Adsr.ReleaseRate = 0x20;
    SEQChannel->Adsr.Sustain = 0;
    SEQChannel->vibratoRateTarget = 0x800;
    SEQChannel->vibratoRateStart = 0x800;
    SEQChannel->vibratoExtentTarget = 0;
    SEQChannel->vibratoExtentStart = 0;
    SEQChannel->vibratoRateChangeDelay = 0;
    SEQChannel->vibratoExtentChangeDelay = 0;
    SEQChannel->vibratoDelay = 0;
    SEQChannel->Volume = 1.0f;
    SEQChannel->volumeScale = 1.0f;
    SEQChannel->FreqScale = 1.0f;
    for (i = 0; i < 8; i++) {
        SEQChannel->soundScriptIO[i] = -1;
    }
    SEQChannel->Unused = false;
    NasWaveMemoryMake(&SEQChannel->NotePool);
}

s32 NasEnvInit(struct SequenceChannel* SEQChannel, s32 layerIndex) {
    struct SequenceChannelLayer* layer;

    if (SEQChannel->layers[layerIndex] == NULL) {
        struct SequenceChannelLayer* layer;
        layer = NasDeAllocSub(&LayerFreeList);
        SEQChannel->layers[layerIndex] = layer;
        if (layer == NULL) {
            SEQChannel->layers[layerIndex] = NULL;
            return -1;
        }
    } else {
        LCheckTerminate(SEQChannel->layers[layerIndex]);
    }

    layer = SEQChannel->layers[layerIndex];
    layer->SEQChannel = SEQChannel;
    layer->Adsr = SEQChannel->Adsr;
    layer->Adsr.ReleaseRate = 0;
    layer->Enabled = true;
    layer->StopSomething = false;
    layer->ContinuousNotes = false;
    layer->Finished = false;
    layer->IgnoreDrumPan = false;
    layer->Portamento.Mode = 0;
    layer->ScriptState.Depth = 0;
    layer->Status = SOUND_LOAD_STATUS_NOT_LOADED;
    layer->NoteDuration = 0x80;
    layer->pan = 0x40;
    layer->Transposition = 0;
    layer->Delay = 0;
    layer->Duration = 0;
    layer->DelayUnused = 0;
    layer->Note = NULL;
    layer->Instrument = NULL;
    layer->FreqScale = 1.0f;
    layer->VelocitySquare = 0.0f;
    layer->InstOrWave = 0xff;
    return 0;
}

void NasEnvProcess(struct SequenceChannelLayer* layer) {
    if (layer != NULL) {
        LCheckTerminate(layer);
        layer->Enabled = false;
        layer->Finished = true;
    }
}

void ENV_RETRY(struct SequenceChannel* SEQChannel, s32 layerIndex) {
    struct SequenceChannelLayer* layer = SEQChannel->layers[layerIndex];

    if (layer != NULL) {
        NasAllocSub(&LayerFreeList, &layer->ListItem);
        NasEnvProcess(layer);
        SEQChannel->layers[layerIndex] = NULL;
    }
}

void NasInitSubTrack(struct SequenceChannel* SEQChannel) {
    s32 i;
    for (i = 0; i < LAYERS_MAX; i++) {
        ENV_RETRY(SEQChannel, i);
    }

    NasInitList(&SEQChannel->NotePool);
    SEQChannel->Enabled = false;
    SEQChannel->Finished = true;
}

struct SequenceChannel* NasEntryNoteTrack(void) {
    s32 i;
    for (i = 0; i < SEQUENCE_CHANNELS; i++) {
        if (SequenceChannels[i].seqPlayer == NULL) {
            return &SequenceChannels[i];
        }
    }
    return &SequenceChannelNone;
}

void NasReleaseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits) {
    struct SequenceChannel* SEQChannel;
    s32 i;

    for (i = 0; i < CHANNELS_MAX; i++) {
        if (channelBits & 1) {
            SEQChannel = seqPlayer->Channels[i];
            if (IS_SEQUENCE_CHANNEL_VALID(SEQChannel) == true && SEQChannel->seqPlayer == seqPlayer) {
                NasInitSubTrack(SEQChannel);
                SEQChannel->seqPlayer = NULL;
            }
            SEQChannel = NasEntryNoteTrack();
            if (IS_SEQUENCE_CHANNEL_VALID(SEQChannel) == false) {
                // eu_stubbed_printf_0("Audio:Track:Warning: No Free Notetrack\n");
                AudioErrorFlags = i + 0x10000;
                seqPlayer->Channels[i] = SEQChannel;
            } else {
                NasChannelModInit(SEQChannel);
                seqPlayer->Channels[i] = SEQChannel;
                SEQChannel->seqPlayer = seqPlayer;
                SEQChannel->BankId = seqPlayer->DefaultBank[0];
                SEQChannel->MuteBehavior = seqPlayer->MuteBehavior;
                SEQChannel->NoteAllocPolicy = seqPlayer->NoteAllocPolicy;
            }
        }
        channelBits = channelBits >> 1;
    }
}

void NasCloseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits) {
    struct SequenceChannel* SEQChannel;
    s32 i;

    // eu_stubbed_printf_0("SUBTRACK DIM\n");
    for (i = 0; i < CHANNELS_MAX; i++) {
        if (channelBits & 1) {
            SEQChannel = seqPlayer->Channels[i];
            if (IS_SEQUENCE_CHANNEL_VALID(SEQChannel) == true) {
                if (SEQChannel->seqPlayer == seqPlayer) {
                    NasInitSubTrack(SEQChannel);
                    SEQChannel->seqPlayer = NULL;
                } else {
                    // stubbed_printf("Audio:Track: Warning SUBTRACK PARENT CHANGED\n");
                }
                seqPlayer->Channels[i] = &SequenceChannelNone;
            }
        }
        channelBits = channelBits >> 1;
    }
}

void NasReleaseSubTrack(struct SequencePlayer* seqPlayer, u8 channelIndex, void* script) {
    struct SequenceChannel* SEQChannel = seqPlayer->Channels[channelIndex];
    s32 i;
    if (IS_SEQUENCE_CHANNEL_VALID(SEQChannel) == false) {
        // stubbed_printf("SEQID %d,BANKID %d\n", seqPlayer->SEQId, seqPlayer->DefaultBank[0]);
        // stubbed_printf("ERR:SUBTRACK %d NOT ALLOCATED\n", channelIndex);
    } else {
        SEQChannel->Enabled = true;
        SEQChannel->Finished = false;
        SEQChannel->ScriptState.Depth = 0;
        SEQChannel->ScriptState.pc = script;
        SEQChannel->Delay = 0;
        for (i = 0; i < LAYERS_MAX; i++) {
            if (SEQChannel->layers[i] != NULL) {
                ENV_RETRY(SEQChannel, i);
            }
        }
    }
}

void NasGetNewSubTrack(struct SequencePlayer* seqPlayer) {
    NasCloseNoteTrack(seqPlayer, 0xffff);
    NasInitList(&seqPlayer->NotePool);
    seqPlayer->Finished = true;
    seqPlayer->Enabled = false;

    if (IS_SEQ_LOAD_COMPLETE(seqPlayer->SEQId) && SeqLoadStatus[seqPlayer->SEQId] != 5) {
        SeqLoadStatus[seqPlayer->SEQId] = SOUND_LOAD_STATUS_DISCARDABLE;
    }

    if (IS_BANK_LOAD_COMPLETE(seqPlayer->DefaultBank[0]) && BankLoadStatus[seqPlayer->DefaultBank[0]] != 5) {
        BankLoadStatus[seqPlayer->DefaultBank[0]] = 4;
    }

    // (Note that if this is called from NasSzAutoDivide, the side will get swapped
    // later in that function. Thus, we signal that we want to load into the slot
    // of the bank that we no longer need.)
    if (seqPlayer->DefaultBank[0] == BankLoadedPool.Temporary.Entries[0].id) {
        BankLoadedPool.Temporary.NextSide = 1;
    } else if (seqPlayer->DefaultBank[0] == BankLoadedPool.Temporary.Entries[1].id) {
        BankLoadedPool.Temporary.NextSide = 0;
    }
}

void NasAllocSub(struct AudioListItem* list, struct AudioListItem* Item) {
    if (Item->Prev != NULL) {
        // eu_stubbed_printf_0("Error:Same List Add\n");
    } else {
        list->Prev->Next = Item;
        Item->Prev = list->Prev;
        Item->Next = list;
        list->Prev = Item;
        list->u.Count++;
        Item->Pool = list->Pool;
    }
}

void* NasDeAllocSub(struct AudioListItem* list) {
    struct AudioListItem* Item = list->Prev;
    if (Item == list) {
        return NULL;
    }
    Item->Prev->Next = list;
    list->Prev = Item->Prev;
    Item->Prev = NULL;
    list->u.Count--;
    return Item->u.Value;
}

void NasOpenSub(void) {
    s32 i;

    LayerFreeList.Prev = &LayerFreeList;
    LayerFreeList.Next = &LayerFreeList;
    LayerFreeList.u.Count = 0;
    LayerFreeList.Pool = NULL;

    for (i = 0; i < ARRAY_COUNT(SequenceLayers); i++) {
        SequenceLayers[i].ListItem.u.Value = &SequenceLayers[i];
        SequenceLayers[i].ListItem.Prev = NULL;
        NasAllocSub(&LayerFreeList, &SequenceLayers[i].ListItem);
    }
}

u8 NasReleaseGroup(struct M64ScriptState* State) {
    return *(State->pc++);
}

s16 NasAddList(struct M64ScriptState* State) {
    s16 ret = *(State->pc++) << 8;
    ret = *(State->pc++) | ret;
    return ret;
}

u16 NasGetList(struct M64ScriptState* State) {
    u16 ret = *(State->pc++);
    if (ret & 0x80) {
        ret = (ret << 8) & 0x7f00;
        ret = *(State->pc++) | ret;
    }
    return ret;
}

void NasInitNoteList(struct SequenceChannelLayer* layer) {
    struct SequencePlayer* seqPlayer;
    struct SequenceChannel* SEQChannel;
    UNUSED u32 Pad0;
    struct M64ScriptState* State;
    struct Portamento* Portamento;
    struct AudioBankSound* Sound;
    struct Instrument* Instrument;
    struct Drum* drum;
    s32 temp_a0_5;
    u16 sp3A;
    s32 sameSound;
    UNUSED u32 pad1;
    u8 cmd;
    UNUSED u8 cmdSemitone;
    f32 Tuning;
    s32 vel;
    UNUSED s32 usedSemitone;
    f32 FreqScale;
    f32 temp_f12;
    f32 temp_f2;

    sameSound = true;
    if (layer->Enabled == false) {
        return;
    }

    if (layer->Delay > 1) {
        layer->Delay--;
        if (!layer->StopSomething && layer->Delay <= layer->Duration) {
            LCheckTerminate(layer);
            layer->StopSomething = true;
        }
        return;
    }

    if (!layer->ContinuousNotes) {
        LCheckTerminate(layer);
    }
#ifdef VERSION_EU_V10
    else {
        if ((layer->Note != 0) && (layer == layer->Note->WantedParentLayer)) {
            LCheckTerminate(layer);
        }
    }
#endif

    if (PORTAMENTO_MODE(layer->Portamento) == PORTAMENTO_MODE_1 ||
        PORTAMENTO_MODE(layer->Portamento) == PORTAMENTO_MODE_2) {
        layer->Portamento.Mode = 0;
    }

    SEQChannel = layer->SEQChannel;
    seqPlayer = SEQChannel->seqPlayer;
    layer->NotePropertiesNeedInit = true;

    for (;;) {
        State = &layer->ScriptState;
        cmd = NasReleaseGroup(State);

        if (cmd <= 0xc0) {
            break;
        }

        switch (cmd) {
            case 0xff: // layer_end; function return or end of script
                if (State->Depth == 0) {
                    // N.B. this function call is *not* inlined even though it's
                    // within the same file, unlike in the rest of this function.
                    NasEnvProcess(layer);
                    return;
                }
                State->pc = State->Stack[--State->Depth];
                break;

            case 0xfc: // layer_call
                if (0 && State->Depth >= 4) {}
                sp3A = NasAddList(State);
                State->Stack[State->Depth++] = State->pc;
                State->pc = seqPlayer->SEQData + sp3A;
                break;

            case 0xf8: // layer_loop; Loop Start, N iterations (or 256 if N = 0)
                if (0 && State->Depth >= 4) {}
                State->RemLoopIters[State->Depth] = NasReleaseGroup(State);
                State->Stack[State->Depth++] = State->pc;
                break;

            case 0xf7: // layer_loopend
                if (--State->RemLoopIters[State->Depth - 1] != 0) {
                    State->pc = State->Stack[State->Depth - 1];
                } else {
                    State->Depth--;
                }
                break;

            case 0xfb: // layer_jump
                sp3A = NasAddList(State);
                State->pc = seqPlayer->SEQData + sp3A;
                break;

            case 0xf4:
                State->pc += (s8) NasReleaseGroup(State);
                break;

            case 0xc1: // layer_setshortnotevelocity
            case 0xca: // layer_setpan
                temp_a0_5 = *(State->pc++);
                if (cmd == 0xc1) {
                    layer->VelocitySquare = (f32) (temp_a0_5 * temp_a0_5) / 16129.0f;
                } else {
                    layer->pan = temp_a0_5;
                }
                break;

            case 0xc2: // layer_transpose; set Transposition in semitones
            case 0xc9: // layer_setshortnoteduration
                temp_a0_5 = *(State->pc++);
                if (cmd == 0xc9) {
                    layer->NoteDuration = temp_a0_5;
                } else {
                    layer->Transposition = temp_a0_5;
                }
                break;

            case 0xc4: // layer_somethingon
            case 0xc5: // layer_somethingoff
                if (cmd == 0xc4) {
                    layer->ContinuousNotes = true;
                } else {
                    layer->ContinuousNotes = false;
                }
                LCheckTerminate(layer);
                break;

            case 0xc3: // layer_setshortnotedefaultplaypercentage
                sp3A = NasGetList(State);
                layer->ShortNoteDefaultPlayPercentage = sp3A;
                break;

            case 0xc6: // layer_setinstr
                cmd = NasReleaseGroup(State);
                if (cmd >= 0x7f) {
                    if (cmd == 0x7f) {
                        layer->InstOrWave = 0;
                    } else {
                        layer->InstOrWave = cmd;
                        layer->Instrument = NULL;
                    }

                    if (1) {}

                    if (cmd == 0xff) {
                        layer->Adsr.ReleaseRate = 0;
                    }
                    break;
                }

                if ((layer->InstOrWave = NasReadByteData(SEQChannel, cmd, &layer->Instrument, &layer->Adsr)) == 0) {
                    layer->InstOrWave = 0xff;
                }
                break;

            case 0xc7: // layer_portamento
                layer->Portamento.Mode = NasReleaseGroup(State);

                // cmd is reused for the Portamento's semitone
                cmd = NasReleaseGroup(State) + SEQChannel->Transposition + layer->Transposition + seqPlayer->Transposition;

                if (cmd >= 0x80) {
                    cmd = 0;
                }

                layer->PortamentoTargetNote = cmd;

                // If special, the Next Param is u8 instead of var
                if (PORTAMENTO_IS_SPECIAL(layer->Portamento)) {
                    layer->PortamentoTime = *((State)->pc++);
                    break;
                }

                sp3A = NasGetList(State);
                layer->PortamentoTime = sp3A;
                break;

            case 0xc8: // layer_disableportamento
                layer->Portamento.Mode = 0;
                break;

            case 0xcb:
                sp3A = NasAddList(State);
                layer->Adsr.Envelope = (struct AdsrEnvelope*) (seqPlayer->SEQData + sp3A);
                layer->Adsr.ReleaseRate = NasReleaseGroup(State);
                break;

            case 0xcc:
                layer->IgnoreDrumPan = true;
                break;

            default:
                switch (cmd & 0xf0) {
                    case 0xd0: // layer_setshortnotevelocityfromtable
                        sp3A = seqPlayer->ShortNoteVelocityTable[cmd & 0xf];
                        layer->VelocitySquare = (f32) (sp3A * sp3A) / 16129.0f;
                        break;
                    case 0xe0: // layer_setshortnotedurationfromtable
                        layer->NoteDuration = seqPlayer->ShortNoteDurationTable[cmd & 0xf];
                        break;
                    default:
                        break;
                }
        }
    }

    if (cmd == 0xc0) { // layer_delay
        layer->Delay = NasGetList(State);
        layer->StopSomething = true;
    } else {
        layer->StopSomething = false;

        if (SEQChannel->largeNotes == true) {
            switch (cmd & 0xc0) {
                case 0x00: // layer_note0 (play percentage, Velocity, Duration)
                    sp3A = NasGetList(State);
                    vel = *(State->pc++);
                    layer->NoteDuration = *(State->pc++);
                    layer->PlayPercentage = sp3A;
                    break;

                case 0x40: // layer_note1 (play percentage, Velocity)
                    sp3A = NasGetList(State);
                    vel = *(State->pc++);
                    layer->NoteDuration = 0;
                    layer->PlayPercentage = sp3A;
                    break;

                case 0x80: // layer_note2 (Velocity, Duration; uses last play percentage)
                    sp3A = layer->PlayPercentage;
                    vel = *(State->pc++);
                    layer->NoteDuration = *(State->pc++);
                    break;
            }
            if ((vel >= 0x80) || (vel < 0)) {
                vel = 0x0000007F;
            }
            // the remaining bits are Used for the semitone
            cmd -= (cmd & 0xc0);
            layer->VelocitySquare = ((f32) (vel) * (f32) vel) / 16129.0f;
        } else {
            switch (cmd & 0xc0) {
                case 0x00: // play Note, Type 0 (play percentage)
                    sp3A = NasGetList(State);
                    layer->PlayPercentage = sp3A;
                    break;

                case 0x40: // play Note, Type 1 (uses default play percentage)
                    sp3A = layer->ShortNoteDefaultPlayPercentage;
                    break;

                case 0x80: // play Note, Type 2 (uses last play percentage)
                    sp3A = layer->PlayPercentage;
                    break;
            }

            // the remaining bits are Used for the semitone
            cmd -= cmd & 0xc0;
        }

        layer->Delay = sp3A;
        layer->Duration = layer->NoteDuration * sp3A >> 8;
        if ((seqPlayer->Muted && (SEQChannel->MuteBehavior & MUTE_BEHAVIOR_STOP_NOTES) != 0) ||
            SEQChannel->stopSomething2) {
            layer->StopSomething = true;

        } else {
            s32 temp = layer->InstOrWave;
            if (temp == 0xff) {
                if (!SEQChannel->hasInstrument) {
                    return;
                }
                temp = SEQChannel->InstOrWave;
            }
            if (temp == 0) { // drum
                // cmd is reused for the drum semitone
                cmd += SEQChannel->Transposition + layer->Transposition;

                drum = PercToPp(SEQChannel->BankId, cmd);
                if (drum == NULL) {
                    layer->StopSomething = true;
                    layer->DelayUnused = layer->Delay;
                    return;
                } else {
                    layer->Adsr.Envelope = drum->Envelope;
                    layer->Adsr.ReleaseRate = drum->ReleaseRate;
                    if (!layer->IgnoreDrumPan) {
                        layer->pan = drum->pan;
                    }
                    layer->Sound = &drum->Sound;
                    layer->FreqScale = layer->Sound->Tuning;
                }
            } else { // Instrument
                // cmd is reused for the Instrument semitone
                cmd += seqPlayer->Transposition + SEQChannel->Transposition + layer->Transposition;

                if (cmd >= 0x80) {
                    layer->StopSomething = true;
                } else {
                    if (layer->InstOrWave == 0xffu) {
                        Instrument = SEQChannel->Instrument;
                    } else {
                        Instrument = layer->Instrument;
                    }

                    if (layer->Portamento.Mode != 0) {
                        if (layer->PortamentoTargetNote < cmd) {
                            vel = cmd;
                        } else {
                            vel = layer->PortamentoTargetNote;
                        }

                        if (Instrument != NULL) {
                            Sound = NoteToVoice(Instrument, vel);
                            sameSound = (Sound == layer->Sound);
                            layer->Sound = Sound;
                            Tuning = Sound->Tuning;
                        } else {
                            layer->Sound = NULL;
                            Tuning = 1.0f;
                        }

                        temp_f2 = NoteFrequencies[cmd] * Tuning;
                        temp_f12 = NoteFrequencies[layer->PortamentoTargetNote] * Tuning;

                        Portamento = &layer->Portamento;
                        switch (PORTAMENTO_MODE(layer->Portamento)) {
                            case PORTAMENTO_MODE_1:
                            case PORTAMENTO_MODE_3:
                            case PORTAMENTO_MODE_5:
                                FreqScale = temp_f12;
                                break;

                            case PORTAMENTO_MODE_2:
                            case PORTAMENTO_MODE_4:
                            default:
                                FreqScale = temp_f2;
                                break;
                        }

                        Portamento->Extent = temp_f2 / FreqScale - 1.0f;

                        if (PORTAMENTO_IS_SPECIAL(layer->Portamento)) {
                            Portamento->Speed = US_FLOAT(32512.0) * FLOAT_CAST(seqPlayer->Tempo) /
                                                ((f32) layer->Delay * (f32) TempoInternalToExternal *
                                                 FLOAT_CAST(layer->PortamentoTime));
                        } else {
                            Portamento->Speed = US_FLOAT(127.0) / FLOAT_CAST(layer->PortamentoTime);
                        }
                        Portamento->cur = 0.0f;
                        layer->FreqScale = FreqScale;
                        if (PORTAMENTO_MODE(layer->Portamento) == PORTAMENTO_MODE_5) {
                            layer->PortamentoTargetNote = cmd;
                        }
                    } else if (Instrument != NULL) {
                        Sound = NoteToVoice(Instrument, cmd);
                        sameSound = (Sound == layer->Sound);
                        layer->Sound = Sound;
                        layer->FreqScale = NoteFrequencies[cmd] * Sound->Tuning;
                    } else {
                        layer->Sound = NULL;
                        layer->FreqScale = NoteFrequencies[cmd];
                    }
                }
            }
            layer->DelayUnused = layer->Delay;
        }
    }

    if (layer->StopSomething == true) {
        if (layer->Note != NULL || layer->ContinuousNotes) {
            LCheckTerminate(layer);
        }
        return;
    }

    cmd = false;
    if (!layer->ContinuousNotes) {
        cmd = true;
    } else if (layer->Note == NULL || layer->Status == SOUND_LOAD_STATUS_NOT_LOADED) {
        cmd = true;
    } else if (sameSound == false) {
        LCheckTerminate(layer);
        cmd = true;
    } else if (layer != layer->Note->ParentLayer) {
        cmd = true;
    } else if (layer->Sound == NULL) {
        NasReleaseChannel(layer->Note, layer);
    }

    if (cmd != false) {
        layer->Note = NasChLookFree(layer);
    }

    if (layer->Note != NULL && layer->Note->ParentLayer == layer) {
        NasModTableRead(layer->Note);
    }
    if (SEQChannel) {}
}

u8 NasReadByteData(struct SequenceChannel* SEQChannel, u8 instId, struct Instrument** instOut,
                  struct AdsrSettings* Adsr) {
    struct Instrument* inst;
    inst = ProgToVp(SEQChannel->BankId, instId);
    if (inst == NULL) {
        *instOut = NULL;
        return 0;
    }
    Adsr->Envelope = inst->Envelope;
    Adsr->ReleaseRate = inst->ReleaseRate;
    *instOut = inst;
    instId++;
    return instId;
}

void NasReadWordData(struct SequenceChannel* SEQChannel, u8 instId) {
    if (instId >= 0x80) {
        SEQChannel->InstOrWave = instId;
        SEQChannel->Instrument = NULL;
    } else if (instId == 0x7f) {
        SEQChannel->InstOrWave = 0;
        SEQChannel->Instrument = (struct Instrument*) 1;
    } else {
        if ((SEQChannel->InstOrWave = NasReadByteData(SEQChannel, instId, &SEQChannel->Instrument, &SEQChannel->Adsr)) ==
            0) {
            SEQChannel->hasInstrument = false;
            return;
        }
    }
    SEQChannel->hasInstrument = true;
}

void NasReadLengthData(struct SequenceChannel* SEQChannel, u8 Volume) {
    SEQChannel->Volume = FLOAT_CAST(Volume) / US_FLOAT(127.0);
}

void NasNoteSeq(struct SequenceChannel* SEQChannel) {
    struct M64ScriptState* State;
    struct SequencePlayer* seqPlayer;
    u8 cmd;
    s8 temp;
    u8 loBits;
    u16 sp5A;
    s32 sp38;
    s8 Value;
    s32 i;
    u8* SEQData;

    if (!SEQChannel->Enabled) {
        return;
    }
    if (SEQChannel->stopScript) {
        for (i = 0; i < LAYERS_MAX; i++) {
            if (SEQChannel->layers[i] != NULL) {
                NasInitNoteList(SEQChannel->layers[i]);
            }
        }
        return;
    }
    seqPlayer = SEQChannel->seqPlayer;
    if (seqPlayer->Muted && ((SEQChannel->MuteBehavior & 0x80) != 0)) {
        return;
    }
    if (SEQChannel->Delay != 0) {
        SEQChannel->Delay--;
    }
    State = &SEQChannel->ScriptState;
    if (SEQChannel->Delay == 0) {
        for (;;) {
            cmd = NasReleaseGroup(State);
            if (cmd > 0xc0) {
                switch (cmd) {
                    case 0xFF:
                        if (State->Depth) {};
                        if (1) {};
                        if (1) {};
                        if (1) {};
                        if (State->Depth == 0) {
                            NasInitSubTrack(SEQChannel);
                            goto out;
                        } else {
                            State->pc = State->Stack[--State->Depth];
                        }
                        break;

                    case 0xFE:
                        goto out;

                    case 0xFD:
                        SEQChannel->Delay = NasGetList(State);
                        goto out;

                    case 0xEA:
                        SEQChannel->stopScript = 1;
                        goto out;

                    case 0xFC:
                        sp5A = NasAddList(State);
                        State->Stack[State->Depth++] = State->pc;
                        State->pc = seqPlayer->SEQData + sp5A;
                        break;

                    case 0xF8:
                        State->RemLoopIters[State->Depth] = NasReleaseGroup(State);
                        State->Stack[State->Depth] = State->pc;
                        State->Depth += 1;
                        break;

                    case 0xF7:
                        State->RemLoopIters[State->Depth - 1]--;
                        if (State->RemLoopIters[State->Depth - 1] != 0) {
                            State->pc = State->Stack[State->Depth - 1];
                        } else {
                            State->Depth--;
                        }
                        break;

                    case 0xF6:
                        State->Depth -= 1;
                        break;

                    case 0xF5:
                    case 0xF9:
                    case 0xFA:
                    case 0xFB:
                        sp5A = NasAddList(State);
                        if ((cmd == 0xFA) && (Value != 0)) {
                            break;
                        }
                        if ((cmd == 0xF9) && (Value >= 0)) {
                            break;
                        }
                        if ((cmd == 0xF5) && (Value < 0)) {
                            break;
                        }
                        State->pc = seqPlayer->SEQData + sp5A;
                        break;

                    case 0xF2:
                    case 0xF3:
                    case 0xF4:
                        temp = NasReleaseGroup(State);
                        if ((cmd == 0xF3) && (Value != 0)) {
                            break;
                        }
                        if ((cmd == 0xF2) && (Value >= 0)) {
                            break;
                        }
                        State->pc += temp;
                        break;

                    case 0xF1:
                        NasInitList(&SEQChannel->NotePool);
                        NasInitChNode(&SEQChannel->NotePool, NasReleaseGroup(State));
                        break;

                    case 0xF0:
                        NasInitList(&SEQChannel->NotePool);
                        break;

                    case 0xC2:
                        sp5A = NasAddList(State);
                        SEQChannel->dynTable = (void*) (seqPlayer->SEQData + sp5A);
                        break;

                    case 0xC5:
                        if (Value != (-1)) {
                            SEQData = (*SEQChannel->dynTable)[Value];
                            sp38 = (u16) ((SEQData[0] << 8) + SEQData[1]);
                            SEQChannel->dynTable = (void*) (seqPlayer->SEQData + sp38);
                        }
                        break;

                    case 0xEB:
                        cmd = NasReleaseGroup(State);
                        sp38 = ((u16*) AlBankSets)[seqPlayer->SEQId];
                        loBits = *(sp38 + AlBankSets);
                        cmd = AlBankSets[(((s32) sp38) + loBits) - cmd];
                        if (NasSzHeapAlloc(1, 2, cmd) != NULL) {
                            SEQChannel->BankId = cmd;
                        }

                    case 0xC1:
                        NasReadWordData(SEQChannel, NasReleaseGroup(State));
                        break;

                    case 0xC3:
                        SEQChannel->largeNotes = 0;
                        break;

                    case 0xC4:
                        SEQChannel->largeNotes = 1;
                        break;

                    case 0xDF:
                        NasReadLengthData(SEQChannel, NasReleaseGroup(State));
                        SEQChannel->Changes.AsBitfields.Volume = 1;
                        break;

                    case 0xE0:
                        SEQChannel->volumeScale = ((f32) ((s32) NasReleaseGroup(State))) / 128.0f;
                        SEQChannel->Changes.AsBitfields.Volume = 1;
                        break;

                    case 0xDE:
                        sp5A = NasAddList(State);
                        SEQChannel->FreqScale = ((f32) ((s32) sp5A)) / 32768.0f;
                        SEQChannel->Changes.AsBitfields.FreqScale = 1;
                        break;

                    case 0xD3:
                        cmd = NasReleaseGroup(State) + 127;
                        SEQChannel->FreqScale = PitchBendFrequencyScale[cmd];
                        SEQChannel->Changes.AsBitfields.FreqScale = 1;
                        break;

                    case 0xDD:
                        SEQChannel->newPan = NasReleaseGroup(State);
                        SEQChannel->Changes.AsBitfields.pan = 1;
                        break;

                    case 0xDC:
                        SEQChannel->panChannelWeight = NasReleaseGroup(State);
                        SEQChannel->Changes.AsBitfields.pan = 1;
                        break;

                    case 0xDB:
                        temp = *(State->pc++);
                        SEQChannel->Transposition = temp;
                        break;

                    case 0xDA:
                        sp5A = NasAddList(State);
                        SEQChannel->Adsr.Envelope = (struct AdsrEnvelope*) (seqPlayer->SEQData + sp5A);
                        break;

                    case 0xD9:
                        SEQChannel->Adsr.ReleaseRate = NasReleaseGroup(State);
                        break;

                    case 0xD8:
                        SEQChannel->vibratoExtentTarget = NasReleaseGroup(State) * 8;
                        SEQChannel->vibratoExtentStart = 0;
                        SEQChannel->vibratoExtentChangeDelay = 0;
                        break;

                    case 0xD7:
                        SEQChannel->vibratoRateStart = SEQChannel->vibratoRateTarget = NasReleaseGroup(State) * 32;
                        SEQChannel->vibratoRateChangeDelay = 0;
                        break;

                    case 0xE2:
                        SEQChannel->vibratoExtentStart = NasReleaseGroup(State) * 8;
                        SEQChannel->vibratoExtentTarget = NasReleaseGroup(State) * 8;
                        SEQChannel->vibratoExtentChangeDelay = NasReleaseGroup(State) * 0x10;
                        break;

                    case 0xE1:
                        SEQChannel->vibratoRateStart = NasReleaseGroup(State) << 5;
                        SEQChannel->vibratoRateTarget = NasReleaseGroup(State) << 5;
                        SEQChannel->vibratoRateChangeDelay = NasReleaseGroup(State) * 0x10;
                        break;

                    case 0xE3:
                        SEQChannel->vibratoDelay = NasReleaseGroup(State) * 0x10;
                        break;

                    case 0xD4:
                        SEQChannel->ReverbVol = NasReleaseGroup(State);
                        break;

                    case 0xC6:
                        cmd = NasReleaseGroup(State);
                        sp5A = ((u16*) AlBankSets)[seqPlayer->SEQId];
                        loBits = *(sp5A + AlBankSets);
                        cmd = AlBankSets[(sp5A + loBits) - cmd];
                        if (NasSzHeapAlloc(1, 2, cmd) != NULL) {
                            SEQChannel->BankId = cmd;
                        }
                        break;

                    case 0xC7:
                        cmd = NasReleaseGroup(State);
                        sp5A = NasAddList(State);
                        SEQData = seqPlayer->SEQData + sp5A;
                        *SEQData = ((u8) Value) + cmd;
                        break;

                    case 0xC8:
                    case 0xC9:
                    case 0xCC:
                        temp = NasReleaseGroup(State);
                        if (cmd == 0xC8) {
                            Value -= temp;
                        } else if (cmd == 0xCC) {
                            Value = temp;
                        } else {
                            Value &= temp;
                        }
                        break;

                    case 0xCA:
                        SEQChannel->MuteBehavior = NasReleaseGroup(State);
                        break;

                    case 0xCB:
                        sp38 = ((u16) NasAddList(State)) + Value;
                        Value = seqPlayer->SEQData[sp38];
                        break;

                    case 0xD0:
                        SEQChannel->StereoHeadsetEffects = NasReleaseGroup(State);
                        break;

                    case 0xD1:
                        SEQChannel->NoteAllocPolicy = NasReleaseGroup(State);
                        break;

                    case 0xD2:
                        SEQChannel->Adsr.Sustain = NasReleaseGroup(State);
                        break;

                    case 0xE5:
                        SEQChannel->reverbIndex = NasReleaseGroup(State);
                        break;

                    case 0xE4:
                        if (Value != (-1)) {
                            if (State->Depth) {};
                            SEQData = (*SEQChannel->dynTable)[Value];
                            State->Stack[State->Depth++] = State->pc;
                            sp38 = (u16) ((SEQData[0] << 8) + SEQData[1]);
                            State->pc = seqPlayer->SEQData + sp38;
                        }
                        break;

                    case 0xE6:
                        SEQChannel->bookOffset = NasReleaseGroup(State);
                        break;

                    case 0xE7:
                        sp5A = NasAddList(State);
                        SEQData = seqPlayer->SEQData + sp5A;
                        SEQChannel->MuteBehavior = *(SEQData++);
                        SEQChannel->NoteAllocPolicy = *(SEQData++);
                        SEQChannel->notePriority = *(SEQData++);
                        SEQChannel->Transposition = (s8) (*(SEQData++));
                        SEQChannel->newPan = *(SEQData++);
                        SEQChannel->panChannelWeight = *(SEQData++);
                        SEQChannel->ReverbVol = *(SEQData++);
                        SEQChannel->reverbIndex = *(SEQData++);
                        SEQChannel->Changes.AsBitfields.pan = 1;
                        break;

                    case 0xE8:
                        SEQChannel->MuteBehavior = NasReleaseGroup(State);
                        SEQChannel->NoteAllocPolicy = NasReleaseGroup(State);
                        SEQChannel->notePriority = NasReleaseGroup(State);
                        SEQChannel->Transposition = (s8) NasReleaseGroup(State);
                        SEQChannel->newPan = NasReleaseGroup(State);
                        SEQChannel->panChannelWeight = NasReleaseGroup(State);
                        SEQChannel->ReverbVol = NasReleaseGroup(State);
                        SEQChannel->reverbIndex = NasReleaseGroup(State);
                        SEQChannel->Changes.AsBitfields.pan = 1;
                        break;

                    case 0xEC:
                        SEQChannel->vibratoExtentTarget = 0;
                        SEQChannel->vibratoExtentStart = 0;
                        SEQChannel->vibratoExtentChangeDelay = 0;
                        SEQChannel->vibratoRateTarget = 0;
                        SEQChannel->vibratoRateStart = 0;
                        SEQChannel->vibratoRateChangeDelay = 0;
                        SEQChannel->FreqScale = 1.0f;
                        break;

                    case 0xE9:
                        SEQChannel->notePriority = NasReleaseGroup(State);
                        break;

                    case 0xEF:
                        NasAddList(State);
                        NasReleaseGroup(State);
                        break;
                }
            } else {
                loBits = cmd & 0xf;
                switch (cmd & 0xF0) {
                    case 0x0:
                        if (SEQChannel->layers[loBits] != NULL) {
                            Value = SEQChannel->layers[loBits]->Finished;
                        } else {
                            Value = -1;
                        }
                        break;

                    case 0x70:
                        SEQChannel->soundScriptIO[loBits] = Value;
                        break;

                    case 0x80:
                        Value = SEQChannel->soundScriptIO[loBits];
                        if (loBits < 4) {
                            SEQChannel->soundScriptIO[loBits] = -1;
                        }
                        break;

                    case 0x50:
                        Value -= SEQChannel->soundScriptIO[loBits];
                        break;

                    case 0x60:
                        SEQChannel->Delay = loBits;
                        goto out;

                    case 0x90:
                        sp5A = NasAddList(State);
                        if (NasEnvInit(SEQChannel, loBits) == 0) {
                            SEQChannel->layers[loBits]->ScriptState.pc = seqPlayer->SEQData + sp5A;
                        }
                        break;

                    case 0xA0:
                        ENV_RETRY(SEQChannel, loBits);
                        break;

                    case 0xB0:
                        if ((Value != (-1)) && (NasEnvInit(SEQChannel, loBits) != (-1))) {
                            SEQData = (*SEQChannel->dynTable)[Value];
                            sp5A = (SEQData[0] << 8) + SEQData[1];
                            SEQChannel->layers[loBits]->ScriptState.pc = seqPlayer->SEQData + sp5A;
                        }
                        break;

                    case 0x10:
                        sp5A = NasAddList(State);
                        NasReleaseSubTrack(seqPlayer, loBits, seqPlayer->SEQData + sp5A);
                        break;

                    case 0x20:
                        NasInitSubTrack(seqPlayer->Channels[loBits]);
                        break;

                    case 0x30:
                        cmd = NasReleaseGroup(State);
                        seqPlayer->Channels[loBits]->soundScriptIO[cmd] = Value;
                        break;

                    case 0x40:
                        cmd = NasReleaseGroup(State);
                        Value = seqPlayer->Channels[loBits]->soundScriptIO[cmd];
                        break;
                }
            }
        }
    }
out:
    for (i = 0; i < LAYERS_MAX; i++) {
        if (SEQChannel->layers[i] != 0) {
            NasInitNoteList(SEQChannel->layers[i]);
        }
    }
}

void ERROR_EXIT(struct SequencePlayer* seqPlayer) {
    u8 cmd;
    u8 loBits;
    u8 temp;
    s32 Value;
    s32 i;
    u16 u16v;
    u8* SEQData;
    struct M64ScriptState* State;
    s32 temp32;

    if (seqPlayer->Enabled == false) {
        return;
    }

    if (seqPlayer->BankDmaInProgress == true) {
        if (osRecvMesg(&seqPlayer->BankDmaMesgQueue, NULL, 0) == -1) {
            return;
        }
        if (seqPlayer->BankDmaRemaining == 0) {
            seqPlayer->BankDmaInProgress = false;
            NasSmzBnkOfsToAdd(seqPlayer->LoadingBankId);
            if (BankLoadStatus[seqPlayer->LoadingBankId] != 5) {
                BankLoadStatus[seqPlayer->LoadingBankId] = 2;
            }
        } else {
            NasSlowRomCopy(&seqPlayer->BankDmaCurrDevAddr, &seqPlayer->BankDmaCurrMemAddr,
                                         &seqPlayer->BankDmaRemaining, &seqPlayer->BankDmaMesgQueue,
                                         &seqPlayer->BankDmaIoMesg);
        }
        return;
    }

    if (seqPlayer->SEQDmaInProgress == true) {
        if (osRecvMesg(&seqPlayer->SEQDmaMesgQueue, NULL, 0) == -1) {
            return;
        }
        seqPlayer->SEQDmaInProgress = false;
        if (SeqLoadStatus[seqPlayer->SEQId] != 5) {
            SeqLoadStatus[seqPlayer->SEQId] = 2;
        }
    }

    // If discarded, bail out.
    temp32 = 2; // I beg your pardon?
    if (IS_SEQ_LOAD_COMPLETE(seqPlayer->SEQId) == false ||
        (IS_BANK_LOAD_COMPLETE(seqPlayer->DefaultBank[0]) == false)) {
        NasGetNewSubTrack(seqPlayer);
        return;
    }

    // Remove possible SOUND_LOAD_STATUS_DISCARDABLE marks.
    if (SeqLoadStatus[seqPlayer->SEQId] != 5) {
        SeqLoadStatus[seqPlayer->SEQId] = temp32;
    }

    if (BankLoadStatus[seqPlayer->DefaultBank[0]] != 5) {
        BankLoadStatus[seqPlayer->DefaultBank[0]] = temp32;
    }

    if (seqPlayer->Muted && (seqPlayer->MuteBehavior & MUTE_BEHAVIOR_STOP_SCRIPT) != 0) {
        return;
    }

    // Check if we surpass the Number of ticks needed for a tatum, else stop.
    seqPlayer->TempoAcc += seqPlayer->Tempo;
    if (seqPlayer->TempoAcc < TempoInternalToExternal) {
        return;
    }
    seqPlayer->TempoAcc -= (u16) TempoInternalToExternal;

    State = &seqPlayer->ScriptState;
    if (seqPlayer->Delay > 1) {
        seqPlayer->Delay--;
    } else {
        seqPlayer->RecalculateVolume = 1;
        for (;;) {
            cmd = NasReleaseGroup(State);
            if (cmd == 0xff) { // seq_end
                if (State->Depth == 0) {
                    NasGetNewSubTrack(seqPlayer);
                    break;
                }
                State->pc = State->Stack[--State->Depth];
            }

            if (cmd == 0xfd) { // seq_delay
                seqPlayer->Delay = NasGetList(State);
                break;
            }

            if (cmd == 0xfe) { // seq_delay1
                seqPlayer->Delay = 1;
                break;
            }

            if (cmd >= 0xc0) {
                switch (cmd) {
                    case 0xff: // seq_end
                        break;

                    case 0xfc: // seq_call
                        u16v = NasAddList(State);
                        if (0 && State->Depth >= 4) {}
                        State->Stack[State->Depth++] = State->pc;
                        State->pc = seqPlayer->SEQData + u16v;
                        break;

                    case 0xf8: // seq_loop; Loop Start, N iterations (or 256 if N = 0)
                        if (0 && State->Depth >= 4) {}
                        State->RemLoopIters[State->Depth] = NasReleaseGroup(State);
                        State->Stack[State->Depth++] = State->pc;
                        break;

                    case 0xf7: // seq_loopend
                        State->RemLoopIters[State->Depth - 1]--;
                        if (State->RemLoopIters[State->Depth - 1] != 0) {
                            State->pc = State->Stack[State->Depth - 1];
                        } else {
                            State->Depth--;
                        }
                        break;

                    case 0xfb: // seq_jump
                    case 0xfa: // seq_beqz; Jump if == 0
                    case 0xf9: // seq_bltz; Jump if < 0
                    case 0xf5: // seq_bgez; Jump if >= 0
                        u16v = NasAddList(State);
                        if (cmd == 0xfa && Value != 0) {
                            break;
                        }
                        if (cmd == 0xf9 && Value >= 0) {
                            break;
                        }
                        if (cmd == 0xf5 && Value < 0) {
                            break;
                        }
                        State->pc = seqPlayer->SEQData + u16v;
                        break;

                    case 0xf4:
                    case 0xf3:
                    case 0xf2:
                        temp = NasReleaseGroup(State);
                        if (cmd == 0xf3 && Value != 0) {
                            break;
                        }
                        if (cmd == 0xf2 && Value >= 0) {
                            break;
                        }
                        State->pc += (s8) temp;
                        break;

                    case 0xf1: // seq_reservenotes
                        NasInitList(&seqPlayer->NotePool);
                        NasInitChNode(&seqPlayer->NotePool, NasReleaseGroup(State));
                        break;

                    case 0xf0: // seq_unreservenotes
                        NasInitList(&seqPlayer->NotePool);
                        break;

                    case 0xdf: // seq_transpose; set Transposition in semitones
                        seqPlayer->Transposition = 0;
                        // fallthrough

                    case 0xde: // seq_transposerel; add Transposition
                        seqPlayer->Transposition += (s8) NasReleaseGroup(State);
                        break;

                    case 0xdc:
                    case 0xdd: // seq_settempo (bpm)
                        temp = NasReleaseGroup(State);
                        if (cmd == 0xdd) {
                            seqPlayer->Tempo = temp * TEMPO_SCALE;
                        } else {
                            seqPlayer->Tempo += (s8) temp * TEMPO_SCALE;
                        }

                        if (seqPlayer->Tempo > TempoInternalToExternal) {
                            seqPlayer->Tempo = TempoInternalToExternal;
                        }

                        // if (cmd) {}

                        if ((s16) seqPlayer->Tempo <= 0) {
                            seqPlayer->Tempo = 1;
                        }
                        break;

                    case 0xda:
                        cmd = NasReleaseGroup(State);
                        u16v = NasAddList(State);
                        switch (cmd) {
                            case SEQUENCE_PLAYER_STATE_0:
                            case SEQUENCE_PLAYER_STATE_FADE_OUT:
                                if (seqPlayer->State != SEQUENCE_PLAYER_STATE_2) {
                                    seqPlayer->FadeTimerUnkEu = u16v;
                                    seqPlayer->State = cmd;
                                }
                                break;
                            case SEQUENCE_PLAYER_STATE_2:
                                seqPlayer->FadeRemainingFrames = u16v;
                                seqPlayer->State = cmd;
                                seqPlayer->FadeVelocity = (0.0f - seqPlayer->FadeVolume) / (s32) (u16v & 0xFFFFu);
                                break;
                        }
                        break;

                    case 0xdb:
                        temp32 = NasReleaseGroup(State);
                        switch (seqPlayer->State) {
                            case SEQUENCE_PLAYER_STATE_2:
                                break;
                            case SEQUENCE_PLAYER_STATE_FADE_OUT:
                                seqPlayer->State = SEQUENCE_PLAYER_STATE_0;
                                seqPlayer->FadeVolume = 0.0f;
                                // fallthrough
                            case SEQUENCE_PLAYER_STATE_0:
                                seqPlayer->FadeRemainingFrames = seqPlayer->FadeTimerUnkEu;
                                if (seqPlayer->FadeTimerUnkEu != 0) {
                                    seqPlayer->FadeVelocity = (temp32 / 127.0f - seqPlayer->FadeVolume) /
                                                              FLOAT_CAST(seqPlayer->FadeRemainingFrames);
                                } else {
                                    seqPlayer->FadeVolume = temp32 / 127.0f;
                                }
                        }
                        break;

                    case 0xd9:
                        temp = NasReleaseGroup(State);
                        seqPlayer->FadeVolumeScale = (s8) temp / 127.0f;
                        break;

                    case 0xd7: // seq_initchannels
                        u16v = NasAddList(State);
                        NasReleaseNoteTrack(seqPlayer, u16v);
                        break;

                    case 0xd6: // seq_disablechannels
                        u16v = NasAddList(State);
                        NasCloseNoteTrack(seqPlayer, u16v);
                        break;

                    case 0xd5: // seq_setmutescale
                        temp = NasReleaseGroup(State);
                        seqPlayer->MuteVolumeScale = (f32) (s8) temp / US_FLOAT(127.0);
                        break;

                    case 0xd4: // seq_mute
                        seqPlayer->Muted = true;
                        break;

                    case 0xd3: // seq_setmutebhv
                        seqPlayer->MuteBehavior = NasReleaseGroup(State);
                        break;

                    case 0xd2: // seq_setshortnotevelocitytable
                    case 0xd1: // seq_setshortnotedurationtable
                        u16v = NasAddList(State);
                        SEQData = seqPlayer->SEQData + u16v;
                        if (cmd == 0xd2) {
                            seqPlayer->ShortNoteVelocityTable = SEQData;
                        } else {
                            seqPlayer->ShortNoteDurationTable = SEQData;
                        }
                        break;

                    case 0xd0: // seq_setnoteallocationpolicy
                        seqPlayer->NoteAllocPolicy = NasReleaseGroup(State);
                        break;

                    case 0xcc: // seq_setval
                        Value = NasReleaseGroup(State);
                        break;

                    case 0xc9: // seq_bitand
                        Value &= NasReleaseGroup(State);
                        break;

                    case 0xc8: // seq_subtract
                        Value = Value - NasReleaseGroup(State);
                        break;

                    default:
                        // eu_stubbed_printf_1("Group:Undefine upper C0h command (%x)\n", cmd);
                        break;
                }
            } else {
                loBits = cmd & 0xf;
                switch (cmd & 0xf0) {
                    case 0x00: // seq_testchdisabled
                        Value = seqPlayer->Channels[loBits]->Finished;
                        break;
                    case 0x10:
                        break;
                    case 0x20:
                        break;
                    case 0x40:
                        break;
                    case 0x50: // seq_subvariation
                        Value -= seqPlayer->SEQVariationEu[0];
                        break;
                    case 0x60:
                        break;
                    case 0x70: // seq_setvariation
                        seqPlayer->SEQVariationEu[0] = Value;
                        break;
                    case 0x80: // seq_getvariation
                        Value = seqPlayer->SEQVariationEu[0];
                        break;
                    case 0x90: // seq_startchannel
                        u16v = NasAddList(State);
                        NasReleaseSubTrack(seqPlayer, loBits, seqPlayer->SEQData + u16v);
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
        if (IS_SEQUENCE_CHANNEL_VALID(seqPlayer->Channels[i]) == true) {
            NasNoteSeq(seqPlayer->Channels[i]);
        }
    }
}

void NasProgramChanger(UNUSED s32 iterationsRemaining) {
    s32 i;
    for (i = 0; i < SEQUENCE_PLAYERS; i++) {
        if (SeqPlayer[i].Enabled == true) {
            ERROR_EXIT(&SeqPlayer[i]);
            NasAllocationOnRequest(&SeqPlayer[i]);
        }
    }
    NasUpdateChannel();
}

void NasSubVoiceSet(u32 Player) {
    struct SequencePlayer* seqPlayer = &SeqPlayer[Player];
    NasGetNewSubTrack(seqPlayer);
    seqPlayer->Delay = 0;
    seqPlayer->State = 1;
    seqPlayer->FadeRemainingFrames = 0;
    seqPlayer->FadeTimerUnkEu = 0;
    seqPlayer->TempoAcc = 0;
    seqPlayer->Tempo = 120 * TEMPO_SCALE; // 120 BPM
    seqPlayer->Transposition = 0;
    seqPlayer->NoteAllocPolicy = 0;
    seqPlayer->ShortNoteVelocityTable = DefaultShortNoteVelocityTable;
    seqPlayer->ShortNoteDurationTable = DefaultShortNoteDurationTable;
    seqPlayer->FadeVolume = 1.0f;
    seqPlayer->FadeVolumeScale = 1.0f;
    seqPlayer->FadeVelocity = 0.0f;
    seqPlayer->Volume = 0.0f;
    seqPlayer->MuteVolumeScale = 0.5f;
}

void NasSubVolumeSet(void) {
    // Initialization function, called from NasInitAudio
    s32 i, j;

    for (i = 0; i < ARRAY_COUNT(SequenceChannels); i++) {
        SequenceChannels[i].seqPlayer = NULL;
        SequenceChannels[i].Enabled = false;
        /**
         * @bug Size of wrong array. Zeroes out second half of SequenceChannels[0],
         * all of SequenceChannels[1..31], and part of SequenceLayers[0].
         * However, this is only called at startup, so it's harmless.
         */
#ifdef AVOID_UB
#define LAYERS_SIZE LAYERS_MAX
#else
#define LAYERS_SIZE ARRAY_COUNT(SequenceLayers)
#endif
        for (j = 0; j < LAYERS_SIZE; j++) {
            SequenceChannels[i].layers[j] = NULL;
        }
    }

    NasOpenSub();

    for (i = 0; i < ARRAY_COUNT(SequenceLayers); i++) {
        SequenceLayers[i].SEQChannel = NULL;
        SequenceLayers[i].Enabled = false;
    }

    for (i = 0; i < SEQUENCE_PLAYERS; i++) {
        for (j = 0; j < CHANNELS_MAX; j++) {
            SeqPlayer[i].Channels[j] = &SequenceChannelNone;
        }

        SeqPlayer[i].SEQVariationEu[0] = -1;
        SeqPlayer[i].MuteBehavior = MUTE_BEHAVIOR_STOP_SCRIPT | MUTE_BEHAVIOR_STOP_NOTES | MUTE_BEHAVIOR_SOFTEN;
        SeqPlayer[i].Enabled = false;
        SeqPlayer[i].Muted = false;
        SeqPlayer[i].BankDmaInProgress = false;
        SeqPlayer[i].SEQDmaInProgress = false;
        NasWaveMemoryMake(&SeqPlayer[i].NotePool);
        NasSubVoiceSet(i);
    }
}
