#include <ultra64.h>
#include <macros.h>
#include <PR/ucode.h>

#include "audio/synthesis.h"
#include "audio/seqplayer.h"
#include "audio/port_eu.h"
#include "audio/load.h"
#include "audio/heap.h"
#include "audio/data.h"

OSMesgQueue D_801937C0;
OSMesgQueue D_801937D8;
OSMesgQueue D_801937F0;
OSMesgQueue D_80193808;

struct EuAudioCmd SAudioCmd[0x100];

// Seems oversized by 1
OSMesg D_80194020[2];
OSMesg D_80194028[4];
OSMesg D_80194038[1];
OSMesg D_8019403C[1];

u8 D_800EA3A0[] = { 0, 0, 0, 0 };

u8 D_800EA3A4[] = { 0, 0, 0, 0 };

OSMesgQueue* D_800EA3A8 = &D_801937C0;
OSMesgQueue* D_800EA3AC = &D_801937D8;
OSMesgQueue* D_800EA3B0 = &D_801937F0;
OSMesgQueue* D_800EA3B4 = &D_80193808;

char PortEUUnusedString0[] = "DAC:Lost 1 Frame.\n";
char PortEUUnusedString1[] = "DMA: Request queue over.( %d )\n";
char PortEUUnusedString2[] = "DMA [ %d lines] TIMEOUT\n";
char PortEUUnusedString3[] = "Warning: WaveDmaQ contains %d msgs.\n";
char PortEUUnusedString4[] = "Audio:now-max tasklen is %d / %d\n";
char PortEUUnusedString5[] = "Audio:Warning:ABI Tasklist length over (%d)\n";

s32 D_800EA484 = 128;

char PortEUUnusedString6[] = "AudioSend: %d -> %d (%d)\n";

s32 D_800EA4A4 = 0;

char PortEUUnusedString7[] = "Undefined Port Command %d\n";

struct SPTask* NasAudioMain(void) {
    u32 samplesRemainingInAI;
    s32 writtenCmds;
    s32 Index;
    OSTask_t* Task;
    s32 var_s0;
    s16* currAiBuffer;
    s32 oldDmaCount;
    OSMesg sp58;
    OSMesg sp54;
    s32 writtenCmdsCopy;

    AudioFrameCount++;
    if ((AudioFrameCount % AudioBufferParameters.PresetUnk4) != 0) {
        return NULL;
    }
    osSendMesg(D_800EA3A8, (OSMesg) AudioFrameCount, OS_MESG_NOBLOCK);

    AudioTaskIndex ^= 1;
    CurrAiBufferIndex++;
    CurrAiBufferIndex %= NUMAIBUFFERS;
    Index = (CurrAiBufferIndex + 1) % NUMAIBUFFERS;
    samplesRemainingInAI = osAiGetLength() / 4;

    if (AiBufferLengths[Index] != 0) {
        osAiSetNextBuffer(AiBuffers[Index], AiBufferLengths[Index] * 4);
    }
    oldDmaCount = CurrAudioFrameDmaCount;
    for (var_s0 = 0; var_s0 < CurrAudioFrameDmaCount; var_s0++) {
        if (osRecvMesg(&CurrAudioFrameDmaQueue, NULL, 0) == 0) {
            oldDmaCount -= 1;
        }
    }
    if (oldDmaCount != 0) {
        for (var_s0 = 0; var_s0 < oldDmaCount; var_s0++) {
            osRecvMesg(&CurrAudioFrameDmaQueue, NULL, 1);
        }
    }
    oldDmaCount = CurrAudioFrameDmaQueue.validCount;
    if (oldDmaCount != 0) {
        for (var_s0 = 0; var_s0 < oldDmaCount; var_s0++) {
            osRecvMesg(&CurrAudioFrameDmaQueue, NULL, 0);
        }
    }
    CurrAudioFrameDmaCount = 0;
    NasSlowRomCopy4096();
    if (osRecvMesg(D_800EA3B0, &sp58, 0) != -1) {
        AudioResetPresetIdToLoad = (u8) (u32) sp58;
        AudioResetStatus = 5;
    }
    if (AudioResetStatus != 0) {
        if (NasDelayDown() == 0) {
            if (AudioResetStatus == 0) {
                osSendMesg(D_800EA3B4, (OSMesg) (u32) AudioResetPresetIdToLoad, OS_MESG_NOBLOCK);
            }
            return NULL;
        }
    }

    AudioTask = &AudioTasks[AudioTaskIndex];
    AudioCmd = AudioCmdBuffers[AudioTaskIndex];
    Index = CurrAiBufferIndex;
    currAiBuffer = AiBuffers[Index];
    AiBufferLengths[Index] =
        ((AudioBufferParameters.SamplesPerFrameTarget - samplesRemainingInAI + EXTRA_BUFFERED_AI_SAMPLES_TARGET) &
         ~0xF) +
        SAMPLES_TO_OVERPRODUCE;
    if (AiBufferLengths[Index] < AudioBufferParameters.MinAiBufferLength) {
        AiBufferLengths[Index] = AudioBufferParameters.MinAiBufferLength;
    }
    if (AiBufferLengths[Index] > AudioBufferParameters.MaxAiBufferLength) {
        AiBufferLengths[Index] = AudioBufferParameters.MaxAiBufferLength;
    }
    if (osRecvMesg(D_800EA3AC, &sp54, 0) != -1) {
        NapAudioPortProcess((u32) sp54);
    }
    AudioCmd = NasSmzAudioFrame((Acmd*) AudioCmd, &writtenCmds, currAiBuffer, AiBufferLengths[Index]);
    AudioRandom = osGetCount() * (AudioRandom + AudioFrameCount);
    AudioRandom = AudioRandom + AiBuffers[Index][AudioFrameCount & 0xFF];

    Index = AudioTaskIndex;
    AudioTask->Msgqueue = NULL;
    // wtf?
    writtenCmdsCopy += 0;
    AudioTask->msg = NULL;

    Task = &AudioTask->Task.t;
    Task->type = M_AUDTASK;
    Task->flags = 0;
    Task->ucode_boot = rspF3DBootStart;
    Task->ucode_boot_size = (u8*) rspF3DBootEnd - (u8*) rspF3DBootStart;
    Task->ucode = rspAspMainStart;
    Task->ucode_data = rspAspMainDataStart;
    Task->ucode_size = 0x1000; // (This Size is ignored (according to sm64))
    Task->ucode_data_size = (rspAspMainDataEnd - rspAspMainDataStart) * sizeof(u64);
    Task->dram_stack = NULL;
    Task->dram_stack_size = 0;
    Task->output_buff = NULL;
    Task->output_buff_size = NULL;
    Task->data_ptr = (u64*) AudioCmdBuffers[Index];
    Task->data_size = writtenCmds * sizeof(u64);
    Task->yield_data_ptr = NULL;
    Task->yield_data_size = 0;
    writtenCmdsCopy = writtenCmds;
    if (D_800EA484 < writtenCmds) {
        D_800EA484 = writtenCmdsCopy;
    }
    return AudioTask;
}

void NapAudioSysProcess(struct EuAudioCmd* cmd) {
    s32 i;

    switch (cmd->u.s.op) {
        case 0x81:
            NasPreLoadSeq(cmd->u.s.Arg2, 3);
            break;

        case 0x82:
        case 0x88:
            NasStartMySeq(cmd->u.s.BankId, cmd->u.s.Arg2, cmd->u.s.Arg3);
            NasGroupFadeIn(cmd->u.s.BankId, cmd->u2.AsS32);
            break;

        case 0x83:
            if (SeqPlayer[cmd->u.s.BankId].Enabled != false) {
                if (cmd->u2.AsS32 == 0) {
                    NasGetNewSubTrack(&SeqPlayer[cmd->u.s.BankId]);
                } else {
                    NasGroupFadeOut(cmd->u.s.BankId, cmd->u2.AsS32);
                }
            }
            break;

        case 0xf0:
            AudioLibSoundMode = cmd->u2.AsS32;
            break;

        case 0xf1:
            for (i = 0; i < 4; i++) {
                SeqPlayer[i].Muted = true;
                SeqPlayer[i].RecalculateVolume = true;
            }
            break;

        case 0xf2:
            for (i = 0; i < 4; i++) {
                SeqPlayer[i].Muted = false;
                SeqPlayer[i].RecalculateVolume = true;
            }
            break;
        case 0xF3:
            NasLoadVoice(cmd->u.s.BankId, cmd->u.s.Arg2, cmd->u.s.Arg3);
            break;
    }
}

void NasGroupFadeOut(s32 arg0, s32 fadeOutTime) {
    struct SequencePlayer* Player;

    if (fadeOutTime == 0) {
        fadeOutTime = 1;
    }
    Player = &SeqPlayer[arg0];
    Player->State = 2;
    Player->FadeRemainingFrames = fadeOutTime;
    Player->FadeVelocity = -(Player->FadeVolume / fadeOutTime);
}

void NasGroupFadeIn(s32 playerIndex, s32 fadeInTime) {
    struct SequencePlayer* Player;

    if (fadeInTime != 0) {
        Player = &SeqPlayer[playerIndex];
        Player->State = 1;
        Player->FadeTimerUnkEu = fadeInTime;
        Player->FadeRemainingFrames = fadeInTime;
        Player->FadeVolume = 0.0f;
        Player->FadeVelocity = 0.0f;
    }
}

void NapAudioPortInit(void) {
    D_800EA3A0[0] = 0;
    D_800EA3A4[0] = 0;
    osCreateMesgQueue(D_800EA3A8, D_80194020, 1);
    osCreateMesgQueue(D_800EA3AC, D_80194028, 4);
    osCreateMesgQueue(D_800EA3B0, D_80194038, 1);
    osCreateMesgQueue(D_800EA3B4, D_8019403C, 1);
}

void NapPortSet(s32 arg0, s32* arg1) {
    struct EuAudioCmd* cmd = &SAudioCmd[D_800EA3A0[0] & 0xff];
    cmd->u.first = arg0;
    cmd->u2.AsU32 = *arg1;
    D_800EA3A0[0]++;
}

void NapSetF32(u32 arg0, f32 arg1) {
    NapPortSet(arg0, (s32*) &arg1);
}

void NapSetS32(u32 arg0, u32 arg1) {
    NapPortSet(arg0, (s32*) &arg1);
}

void NapSetS8(u32 arg0, s8 arg1) {
    s32 sp34 = arg1 << 24;
    NapPortSet(arg0, &sp34);
}

//! @todo clenanup, something's weird with the variables. D_800EA4A4 is probably EuAudioCmd bc of the + 0x100
void NapSendStart(void) {
    s32 temp_t6;
    s32 test;
    OSMesg thing;
    temp_t6 = D_800EA3A0[0] - D_800EA3A4[0];
    test = (u8) temp_t6;
    test = (test + 0x100) & 0xFF;
    do {
    } while (0);
    if (D_800EA4A4 < test) {
        D_800EA4A4 = test;
    }
    thing = (OSMesg) ((D_800EA3A0[0] & 0xFF) | ((D_800EA3A4[0] & 0xFF) << 8));
    osSendMesg(D_800EA3AC, thing, 0);
    D_800EA3A4[0] = D_800EA3A0[0];
}

void NapAudioPortProcess(u32 arg0) {
    struct EuAudioCmd* cmd;
    struct SequencePlayer* seqPlayer;
    struct SequenceChannel* chan;
    u8 end = arg0 & 0xff;
    u8 i = (arg0 >> 8) & 0xff;

    for (;;) {
        if (i == end) {
            break;
        }
        cmd = &SAudioCmd[i++ & 0xff];

        if ((cmd->u.s.op & 0xf0) == 0xf0) {
            NapAudioSysProcess(cmd);
            goto why;
        }

        if (cmd->u.s.BankId < SEQUENCE_PLAYERS) {
            seqPlayer = &SeqPlayer[cmd->u.s.BankId];
            if ((cmd->u.s.op & 0x80) != 0) {
                NapAudioSysProcess(cmd);
            } else if ((cmd->u.s.op & 0x40) != 0) {
                switch (cmd->u.s.op) {
                    case 0x41:
                        seqPlayer->FadeVolumeScale = cmd->u2.AsF32;
                        seqPlayer->RecalculateVolume = true;
                        break;

                    case 0x47:
                        seqPlayer->Tempo = cmd->u2.AsS32 * TATUMS_PER_BEAT;
                        break;

                    case 0x48:
                        seqPlayer->Transposition = cmd->u2.AsS8;
                        break;

                    case 0x46:
                        seqPlayer->SEQVariationEu[cmd->u.s.Arg3] = cmd->u2.AsS8;
                        break;
                }
            } else if (seqPlayer->Enabled != false && cmd->u.s.Arg2 < 0x10) {
                chan = seqPlayer->Channels[cmd->u.s.Arg2];
                if (IS_SEQUENCE_CHANNEL_VALID(chan)) {
                    switch (cmd->u.s.op) {
                        case 1:
                            chan->volumeScale = cmd->u2.AsF32;
                            chan->Changes.AsBitfields.Volume = true;
                            break;
                        case 2:
                            chan->Volume = cmd->u2.AsF32;
                            chan->Changes.AsBitfields.Volume = true;
                            break;
                        case 3:
                            chan->newPan = cmd->u2.AsS8;
                            chan->Changes.AsBitfields.pan = true;
                            break;
                        case 4:
                            chan->FreqScale = cmd->u2.AsF32;
                            chan->Changes.AsBitfields.FreqScale = true;
                            break;
                        case 5:
                            chan->ReverbVol = cmd->u2.AsS8;
                            break;
                        case 6:
                            if (cmd->u.s.Arg3 < 8) {
                                chan->soundScriptIO[cmd->u.s.Arg3] = cmd->u2.AsS8;
                            }
                            break;
                        case 8:
                            chan->stopSomething2 = cmd->u2.AsS8;
                    }
                }
            }
        }

    why:
        cmd->u.s.op = 0;
    }
}

void PortEUInit() {
    NapAudioPortInit();
}
