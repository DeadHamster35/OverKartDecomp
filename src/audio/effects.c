#include <ultra64.h>
#include <macros.h>

#include "audio/seqplayer.h"
#include "audio/effects.h"
#include "audio/internal.h"
#include "audio/load.h"
#include "audio/data.h"

void NasChLookUse(struct SequenceChannel* SEQChannel, s32 RecalculateVolume) {
    f32 channelVolume;
    s32 i;

    if (SEQChannel->Changes.AsBitfields.Volume || RecalculateVolume) {
        channelVolume = SEQChannel->Volume * SEQChannel->volumeScale * SEQChannel->seqPlayer->AppliedFadeVolume;
        if (SEQChannel->seqPlayer->Muted && (SEQChannel->MuteBehavior & MUTE_BEHAVIOR_SOFTEN) != 0) {
            channelVolume = SEQChannel->seqPlayer->MuteVolumeScale * channelVolume;
        }
        SEQChannel->appliedVolume = channelVolume * channelVolume;
    }

    if (SEQChannel->Changes.AsBitfields.pan) {
        SEQChannel->pan = SEQChannel->newPan * SEQChannel->panChannelWeight;
    }

    for (i = 0; i < 4; ++i) {
        struct SequenceChannelLayer* layer = SEQChannel->layers[i];
        if (layer != NULL && layer->Enabled && layer->Note != NULL) {
            if (layer->NotePropertiesNeedInit) {
                layer->NoteFreqScale = layer->FreqScale * SEQChannel->FreqScale;
                layer->NoteVelocity = layer->VelocitySquare * SEQChannel->appliedVolume;
                layer->NotePan = (SEQChannel->pan + layer->pan * (0x80 - SEQChannel->panChannelWeight)) >> 7;
                layer->NotePropertiesNeedInit = false;
            } else {
                if (SEQChannel->Changes.AsBitfields.FreqScale) {
                    layer->NoteFreqScale = layer->FreqScale * SEQChannel->FreqScale;
                }
                if (SEQChannel->Changes.AsBitfields.Volume || RecalculateVolume) {
                    layer->NoteVelocity = layer->VelocitySquare * SEQChannel->appliedVolume;
                }
                if (SEQChannel->Changes.AsBitfields.pan) {
                    layer->NotePan = (SEQChannel->pan + layer->pan * (0x80 - SEQChannel->panChannelWeight)) >> 7;
                }
            }
        }
    }
    SEQChannel->Changes.AsU8 = 0;
}

void NasAllocationOnRequest(struct SequencePlayer* seqPlayer) {
    s32 i;

    if (seqPlayer->FadeRemainingFrames != 0) {
        seqPlayer->FadeVolume += seqPlayer->FadeVelocity;
        seqPlayer->RecalculateVolume = true;

        if (seqPlayer->FadeVolume > US_FLOAT2(1)) {
            seqPlayer->FadeVolume = US_FLOAT2(1);
        }
        if (seqPlayer->FadeVolume < 0) {
            seqPlayer->FadeVolume = 0;
        }

        if (--seqPlayer->FadeRemainingFrames == 0) {
            if (seqPlayer->State == 2) {
                NasGetNewSubTrack(seqPlayer);
                return;
            }
        }
    }

    if (seqPlayer->RecalculateVolume) {
        seqPlayer->AppliedFadeVolume = seqPlayer->FadeVolume * seqPlayer->FadeVolumeScale;
    }

    // Process Channels
    for (i = 0; i < CHANNELS_MAX; i++) {
        if (IS_SEQUENCE_CHANNEL_VALID(seqPlayer->Channels[i]) == true && seqPlayer->Channels[i]->Enabled == true) {
            NasChLookUse(seqPlayer->Channels[i], seqPlayer->RecalculateVolume);
        }
    }

    seqPlayer->RecalculateVolume = false;
}

f32 NasChannelInit(struct Portamento* p) {
    u32 v0;
    f32 result;

    p->cur += p->Speed;
    v0 = (u32) p->cur;

    if (v0 > 127) {
        v0 = 127;
    }

    result = US_FLOAT(1.0) + p->Extent * (PitchBendFrequencyScale[v0 + 128] - US_FLOAT(1.0));
    return result;
}

s16 NasCallWaveProcessSub(struct VibratoState* vib) {
    s32 Index;
    vib->Time += (s32) vib->Rate;
    Index = (vib->Time >> 10) & 0x3F;
    return vib->Curve[Index] >> 8;
}

f32 NasMainCtrl(struct VibratoState* vib) {
    s32 pitchChange;
    f32 Extent;
    f32 result;

    if (vib->Delay != 0) {
        vib->Delay--;
        return 1;
    }

    if (vib->ExtentChangeTimer) {
        if (vib->ExtentChangeTimer == 1) {
            vib->Extent = (s32) vib->SEQChannel->vibratoExtentTarget;
        } else {
            vib->Extent += ((s32) vib->SEQChannel->vibratoExtentTarget - vib->Extent) / (s32) vib->ExtentChangeTimer;
        }

        vib->ExtentChangeTimer--;
    } else if (vib->SEQChannel->vibratoExtentTarget != (s32) vib->Extent) {
        if ((vib->ExtentChangeTimer = vib->SEQChannel->vibratoExtentChangeDelay) == 0) {
            vib->Extent = (s32) vib->SEQChannel->vibratoExtentTarget;
        }
    }

    if (vib->RateChangeTimer) {
        if (vib->RateChangeTimer == 1) {
            vib->Rate = (s32) vib->SEQChannel->vibratoRateTarget;
        } else {
            vib->Rate += ((s32) vib->SEQChannel->vibratoRateTarget - vib->Rate) / (s32) vib->RateChangeTimer;
        }

        vib->RateChangeTimer--;
    } else if (vib->SEQChannel->vibratoRateTarget != (s32) vib->Rate) {
        if ((vib->RateChangeTimer = vib->SEQChannel->vibratoRateChangeDelay) == 0) {
            vib->Rate = (s32) vib->SEQChannel->vibratoRateTarget;
        }
    }

    if (vib->Extent == 0) {
        return 1.0f;
    }

    pitchChange = NasCallWaveProcessSub(vib);
    Extent = (f32) vib->Extent / US_FLOAT(4096.0);

    result = US_FLOAT(1.0) + Extent * (PitchBendFrequencyScale[pitchChange + 128] - US_FLOAT(1.0));
    return result;
}

void NasSweepCalculator(struct Note* Note) {
    if (Note->Portamento.Mode != 0) {
        Note->PortamentoFreqScale = NasChannelInit(&Note->Portamento);
    }
    if (Note->VibratoState.Active && Note->ParentLayer != NO_LAYER) {
        Note->VibratoFreqScale = NasMainCtrl(&Note->VibratoState);
    }
}

void NasModTableRead(struct Note* Note) {
    struct VibratoState* vib;
    UNUSED struct SequenceChannel* SEQChannel;
    struct NotePlaybackState* seqPlayerState = (struct NotePlaybackState*) &Note->Priority;

    Note->VibratoFreqScale = 1.0f;
    Note->PortamentoFreqScale = 1.0f;

    vib = &Note->VibratoState;

    vib->Active = true;
    vib->Time = 0;

    vib->Curve = WaveSamples[2];
    vib->SEQChannel = Note->ParentLayer->SEQChannel;
    if ((vib->ExtentChangeTimer = vib->SEQChannel->vibratoExtentChangeDelay) == 0) {
        vib->Extent = FLOAT_CAST(vib->SEQChannel->vibratoExtentTarget);
    } else {
        vib->Extent = FLOAT_CAST(vib->SEQChannel->vibratoExtentStart);
    }

    if ((vib->RateChangeTimer = vib->SEQChannel->vibratoRateChangeDelay) == 0) {
        vib->Rate = FLOAT_CAST(vib->SEQChannel->vibratoRateTarget);
    } else {
        vib->Rate = FLOAT_CAST(vib->SEQChannel->vibratoRateStart);
    }
    vib->Delay = vib->SEQChannel->vibratoDelay;

    seqPlayerState->Portamento = seqPlayerState->ParentLayer->Portamento;
}

void NasModulator(struct AdsrState* Adsr, struct AdsrEnvelope* Envelope, UNUSED s16* volOut) {
    Adsr->Action = 0;
    Adsr->State = ADSR_STATE_DISABLED;
    Adsr->Delay = 0;
    Adsr->Envelope = Envelope;
    Adsr->Sustain = 0.0f;
    Adsr->Current = 0.0f;
}

f32 NasChannelModulation(struct AdsrState* Adsr) {
    u8 Action = Adsr->Action;
    u8 State = Adsr->State;
    switch (State) {
        case ADSR_STATE_DISABLED:
            return 0;

        case ADSR_STATE_INITIAL: {
            if (Action & ADSR_ACTION_HANG) {
                Adsr->State = ADSR_STATE_HANG;
                break;
            }
            // fallthrough
        }

        case ADSR_STATE_START_LOOP:
            Adsr->EnvIndex = 0;
            Adsr->State = ADSR_STATE_LOOP;
            // fallthrough

        Restart:
        case ADSR_STATE_LOOP:
            Adsr->Delay = BSWAP16(Adsr->Envelope[Adsr->EnvIndex].Delay);
            switch (Adsr->Delay) {
                case ADSR_DISABLE:
                    Adsr->State = ADSR_STATE_DISABLED;
                    break;
                case ADSR_HANG:
                    Adsr->State = ADSR_STATE_HANG;
                    break;
                case ADSR_GOTO:
                    Adsr->EnvIndex = BSWAP16(Adsr->Envelope[Adsr->EnvIndex].arg);
                    goto Restart;
                case ADSR_RESTART:
                    Adsr->State = ADSR_STATE_INITIAL;
                    break;

                default:
                    if (Adsr->Delay >= 4) {
                        Adsr->Delay = Adsr->Delay * AudioBufferParameters.UpdatesPerFrame /
                                      AudioBufferParameters.PresetUnk4 / 4;
                    }
                    if (Adsr->Delay == 0) {
                        Adsr->Delay = 1;
                    }
                    Adsr->Target = (f32) BSWAP16(Adsr->Envelope[Adsr->EnvIndex].arg) / 32767.0f;
                    Adsr->Target = Adsr->Target * Adsr->Target;
                    Adsr->Velocity = (Adsr->Target - Adsr->Current) / Adsr->Delay;
                    Adsr->State = ADSR_STATE_FADE;
                    Adsr->EnvIndex++;
                    break;
            }
            if (Adsr->State != ADSR_STATE_FADE) {
                break;
            }
            // fallthrough

        case ADSR_STATE_FADE:
            Adsr->Current += Adsr->Velocity;
            if (--Adsr->Delay <= 0) {
                Adsr->State = ADSR_STATE_LOOP;
            }
            // fallthrough

        case ADSR_STATE_HANG:
            break;

        case ADSR_STATE_DECAY:
        case ADSR_STATE_RELEASE: {
            Adsr->Current -= Adsr->FadeOutVel;
            if (Adsr->Sustain != 0.0f && State == ADSR_STATE_DECAY) {
                if (Adsr->Current < Adsr->Sustain) {
                    Adsr->Current = Adsr->Sustain;
                    Adsr->Delay = 128;
                    Adsr->State = ADSR_STATE_SUSTAIN;
                }
                break;
            }

            if (Adsr->Current < 0.00001f) {
                Adsr->Current = 0.0f;
                Adsr->State = ADSR_STATE_DISABLED;
            }
            break;
        }

        case ADSR_STATE_SUSTAIN:
            Adsr->Delay -= 1;
            if (Adsr->Delay == 0) {
                Adsr->State = ADSR_STATE_RELEASE;
            }
            break;
    }

    if ((Action & ADSR_ACTION_DECAY)) {
        Adsr->State = ADSR_STATE_DECAY;
        Adsr->Action = Action & ~ADSR_ACTION_DECAY;
    }

    if ((Action & ADSR_ACTION_RELEASE)) {
        Adsr->State = ADSR_STATE_RELEASE;
        Adsr->Action = Action & ~ADSR_ACTION_RELEASE;
    }

    if (Adsr->Current < 0.0f) {
        // Don't actually know where this goes, but we need that string
        // somewhere in the .data section for this file
        eu_stubbed_printf_0("Env-Clear 0\n");
        return 0.0f;
    }
    if (Adsr->Current > 1.0f) {
        eu_stubbed_printf_1("Audio:Envp: overflow  %f\n", Adsr->Current);
        return 1.0f;
    }
    return Adsr->Current;
}
