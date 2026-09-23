#ifndef AUDIO_INTERNAL_H
#define AUDIO_INTERNAL_H

#include <ultra64.h>

#define SEQUENCE_PLAYERS 4
#define SEQUENCE_CHANNELS 48
#define SEQUENCE_LAYERS 64

#define LAYERS_MAX 4
#define CHANNELS_MAX 16

#define NO_LAYER ((struct SequenceChannelLayer*) (-1))

#define MUTE_BEHAVIOR_STOP_SCRIPT 0x80 // stop processing sequence/channel scripts
#define MUTE_BEHAVIOR_STOP_NOTES 0x40  // prevent further notes from playing
#define MUTE_BEHAVIOR_SOFTEN 0x20      // lower Volume, by default to half

#define SEQUENCE_PLAYER_STATE_0 0
#define SEQUENCE_PLAYER_STATE_FADE_OUT 1
#define SEQUENCE_PLAYER_STATE_2 2
#define SEQUENCE_PLAYER_STATE_3 3
#define SEQUENCE_PLAYER_STATE_4 4

#define NOTE_PRIORITY_DISABLED 0
#define NOTE_PRIORITY_STOPPING 1
#define NOTE_PRIORITY_MIN 2
#define NOTE_PRIORITY_DEFAULT 3

#define TATUMS_PER_BEAT 48

// abi.h contains more details about the ADPCM and S8 codecs, "skip" skips codec processing
#define CODEC_ADPCM 0
#define CODEC_S8 1
#define CODEC_SKIP 2

#define TEMPO_SCALE TATUMS_PER_BEAT

//! @todo US_FLOAT should probably be renamed to JP_DOUBLE since eu seems to use floats too
#define US_FLOAT(x) x##f
#define US_FLOAT2(x) x

// Convert u8 or u16 to f32. On JP, this uses a u32->f32 conversion,
// resulting in more bloated codegen, while on US it goes through s32.
// Since u8 and u16 fit losslessly in both, behavior is the same.
#define FLOAT_CAST(x) (f32)(s32)(x)

// No-op printf macro which leaves string literals in rodata in IDO. IDO
// doesn't support variadic macros, so instead we let the parameter list
// expand to a no-op comma expression. Another possibility is that it might
// have expanded to something with "if (0)". See also goddard/gd_main.h.
// On US/JP, -sopt optimizes away these except for external.c.
#ifdef __sgi
#define stubbed_printf
#else
#define stubbed_printf(...)
#endif

#define eu_stubbed_printf_0(msg) stubbed_printf(msg)
#define eu_stubbed_printf_1(msg, a) stubbed_printf(msg, a)
#define eu_stubbed_printf_2(msg, a, b) stubbed_printf(msg, a, b)
#define eu_stubbed_printf_3(msg, a, b, c) stubbed_printf(msg, a, b, c)

struct NotePool;

struct AudioListItem {
    // A node in a circularly linked list. Each node is either a head or an Item:
    // - Items can be either detached (Prev = NULL), or attached to a list.
    //   'Value' points to something of interest.
    // - List heads are always attached; if a list is empty, its head points
    //   to itself. 'Count' contains the Size of the list.
    // If the list holds notes, 'Pool' points back to the Pool where it lives.
    // Otherwise, that member is NULL.
    struct AudioListItem* Prev;
    struct AudioListItem* Next;
    union {
        void* Value; // either Note* or SequenceChannelLayer*
        s32 Count;
    } u;
    struct NotePool* Pool;
}; // Size = 0x10

struct NotePool {
    struct AudioListItem Disabled;
    struct AudioListItem Decaying;
    struct AudioListItem Releasing;
    struct AudioListItem Active;
};

struct VibratoState {
    /*0x00, 0x00*/ struct SequenceChannel* SEQChannel;
    /*0x04, 0x04*/ u32 Time;
    /*    , 0x08*/ s16* Curve;
    /*    , 0x0C*/ f32 Extent;
    /*    , 0x10*/ f32 Rate;
    /*    , 0x14*/ u8 Active;
    /*0x12, 0x16*/ u16 RateChangeTimer;
    /*0x14, 0x18*/ u16 ExtentChangeTimer;
    /*0x16, 0x1A*/ u16 Delay;
}; // Size = 0x18, 0x1C on EU

// Pitch sliding by up to one octave in the positive Direction. Negative
// Direction is "supported" by setting Extent to be negative. The Code
// extrapolates exponentially in the wrong Direction in that case, but that
// doesn't prevent seqplayer from doing it, AFAICT.
struct Portamento {
    u8 Mode; // bit 0x80 denotes something; the rest are an Index 0-5
    f32 cur;
    f32 Speed;
    f32 Extent;
}; // Size = 0x10

struct AdsrEnvelope {
    s16 Delay;
    s16 arg;
}; // Size = 0x4

struct AdpcmLoop {
    u32 Start;
    u32 end;
    u32 Count;
    u32 pad;
    s16 State[16]; // only exists if Count != 0. 8-byte aligned
};

struct AdpcmBook {
    s32 Order;
    s32 Npredictors;
    s16 Book[1]; // Size 8 * Order * Npredictors. 8-byte aligned
};

struct AudioBankSample {
    u8 Unused;
    u8 Loaded;
    u8* SampleAddr;
    struct AdpcmLoop* Loop;
    struct AdpcmBook* Book;
    u32 SampleSize; // never read. either 0 or 1 mod 9, depending on Padding
};

struct AudioBankSound {
    struct AudioBankSample* Sample;
    f32 Tuning; // Frequency Scale factor
}; // Size = 0x8

struct Instrument {
    /*0x00*/ u8 Loaded;
    /*0x01*/ u8 NormalRangeLo;
    /*0x02*/ u8 NormalRangeHi;
    /*0x03*/ u8 ReleaseRate;
    /*0x04*/ struct AdsrEnvelope* Envelope;
    /*0x08*/ struct AudioBankSound LowNotesSound;
    /*0x10*/ struct AudioBankSound NormalNotesSound;
    /*0x18*/ struct AudioBankSound HighNotesSound;
}; // Size = 0x20

struct Drum {
    /*0x00*/ u8 ReleaseRate;
    /*0x01*/ u8 pan;
    /*0x02*/ u8 Loaded;
    struct AudioBankSound Sound;
    struct AdsrEnvelope* Envelope;
};

struct AudioBank {
    struct Drum** Drums;
    struct Instrument* Instruments[1];
}; // dynamic Size

struct CtlEntry {
    u8 Unused;
    u8 NumInstruments;
    u8 NumDrums;
    struct Instrument** Instruments;
    struct Drum** Drums;
}; // Size = 0xC

struct M64ScriptState {
    u8* pc;
    u8* Stack[4];
    u8 RemLoopIters[4];
    u8 Depth;
}; // Size = 0x1C

// Also known as a Group, according to debug strings.
struct SequencePlayer {
    /*US/JP, EU,    SH   */
    /*0x000, 0x000, 0x000*/ u8 Enabled : 1;
    /*0x000, 0x000*/ u8 Finished : 1; // never read
    /*0x000, 0x000*/ u8 Muted : 1;
    /*0x000, 0x000*/ u8 SEQDmaInProgress : 1;
    /*0x000, 0x000*/ u8 BankDmaInProgress : 1;
    /*       0x000*/ u8 RecalculateVolume : 1;
    /*0x002, 0x001, 0x001*/ u8 State;
    /*0x003, 0x002*/ u8 NoteAllocPolicy;
    /*0x004, 0x003*/ u8 MuteBehavior;
    /*0x005, 0x004*/ u8 SEQId;
    /*0x006, 0x005*/ u8 DefaultBank[1]; // must be an array to get a comparison
                                        // to match; other u8's might also be part of that array
    /*0x007, 0x006*/ u8 LoadingBankId;
    /*     , 0x007, 0x007*/ s8 SEQVariationEu[1];
    /*0x00A, 0x008*/ u16 Tempo; // beats per minute in JP, tatums per minute in US/EU
    /*0x00C, 0x00A*/ u16 TempoAcc;
    /*0x010, 0x00C, 0x00E*/ s16 Transposition;
    /*0x012, 0x00E, 0x010*/ u16 Delay;
    /*0x00E, 0x010, 0x012*/ u16 FadeRemainingFrames;
    /*     , 0x012, 0x014*/ u16 FadeTimerUnkEu;
    /*0x014, 0x014*/ u8* SEQData;          // Buffer of some sort
    /*0x018, 0x018, 0x1C*/ f32 FadeVolume; // set to 1.0f
    /*0x01C, 0x01C*/ f32 FadeVelocity;     // set to 0.0f
    /*0x020, 0x020, 0x024*/ f32 Volume;    // set to 0.0f
    /*0x024, 0x024*/ f32 MuteVolumeScale;  // set to 0.5f
    /*     , 0x028, 0x02C*/ f32 FadeVolumeScale;
    /*     , 0x02C*/ f32 AppliedFadeVolume;
    /*0x02C, 0x030, 0x034*/ struct SequenceChannel* Channels[CHANNELS_MAX];
    /*0x06C, 0x070*/ struct M64ScriptState ScriptState;
    /*0x088, 0x08C*/ u8* ShortNoteVelocityTable;
    /*0x08C, 0x090*/ u8* ShortNoteDurationTable;
    /*0x090, 0x094*/ struct NotePool NotePool;
    /*0x0D0, 0x0D4*/ OSMesgQueue SEQDmaMesgQueue;
    /*0x0E8, 0x0EC*/ OSMesg SEQDmaMesg;
    /*0x0EC, 0x0F0*/ OSIoMesg SEQDmaIoMesg;
    /*0x100, 0x108*/ OSMesgQueue BankDmaMesgQueue;
    /*0x118, 0x120*/ OSMesg BankDmaMesg;
    /*0x11C, 0x124*/ OSIoMesg BankDmaIoMesg;
    /*0x130, 0x13C*/ u8* BankDmaCurrMemAddr;
    /*0x138, 0x140*/ uintptr_t BankDmaCurrDevAddr;
    /*0x13C, 0x144*/ ssize_t BankDmaRemaining;
}; // Size = 0x140, 0x148 on EU, 0x14C on SH

struct AdsrSettings {
    u8 ReleaseRate;
    u8 Sustain;
    struct AdsrEnvelope* Envelope;
}; // Size = 0x8

struct AdsrState {
    /*0x00, 0x00*/ u8 Action;
    /*0x01, 0x01*/ u8 State;
    /*0x08, 0x02*/ s16 EnvIndex;
    /*0x0A, 0x04*/ s16 Delay;
    /*    , 0x08*/ f32 Sustain;
    /*    , 0x0C*/ f32 Velocity;
    /*    , 0x10*/ f32 FadeOutVel;
    /*    , 0x14*/ f32 Current;
    /*    , 0x18*/ f32 Target;
    s32 Pad1C;
    /*0x1C, 0x20*/ struct AdsrEnvelope* Envelope;
}; // Size = 0x20, 0x24 in EU

struct ReverbBitsData {
    /* 0x00 */ u8 Bit0 : 1;
    /* 0x00 */ u8 Bit1 : 1;
    /* 0x00 */ u8 Bit2 : 1;
    /* 0x00 */ u8 UsesHeadsetPanEffects : 1;
    /* 0x00 */ u8 StereoHeadsetEffects : 2;
    /* 0x00 */ u8 StrongRight : 1;
    /* 0x00 */ u8 StrongLeft : 1;
};

union ReverbBits {
    /* 0x00 */ struct ReverbBitsData s;
    /* 0x00 */ u8 AsByte;
};
struct ReverbInfo {
    u8 ReverbVol;
    u8 SynthesisVolume; // UQ4.4, although 0 <= x < 1 is rounded up to 1
    u8 pan;
    union ReverbBits ReverbBits;
    f32 FreqScale;
    f32 Velocity;
    s32 Unused;
    s16* Filter;
};

struct NoteAttributes {
    u8 ReverbVol;
    u8 pan;
    f32 FreqScale;
    f32 Velocity;
}; // Size = 0x10

// Also known as a SubTrack, according to debug strings.
// Confusingly, a SubTrack is a container of Tracks.
struct SequenceChannel {
    /* U/J, EU,   SH  */
    /*0x00, 0x00*/ u8 Enabled : 1;
    /*0x00, 0x00*/ u8 Finished : 1;
    /*0x00, 0x00*/ u8 stopScript : 1;
    /*0x00, 0x00*/ u8 stopSomething2 : 1; // sets SequenceChannelLayer.StopSomething
    /*0x00, 0x00*/ u8 hasInstrument : 1;
    /*0x00, 0x00*/ u8 StereoHeadsetEffects : 1;
    /*0x00, ????*/ u8 largeNotes : 1; // notes specify Duration and Velocity
    /*0x00, ????*/ u8 Unused : 1;     // never read, set to 0
    /*    , 0x01*/ union {
        struct {
            u8 FreqScale : 1;
            u8 Volume : 1;
            u8 pan : 1;
        } AsBitfields;
        u8 AsU8;
    } Changes;
    /*0x01, 0x02*/ u8 NoteAllocPolicy;
    /*0x02, 0x03, 0x03*/ u8 MuteBehavior;
    /*0x03, 0x04, 0x04*/ u8 ReverbVol; // until EU: Q1.7, after EU: UQ0.8
    /*0x04, ????*/ u8 notePriority;    // 0-3
    /*0x05, 0x06*/ u8 BankId;
    /*    , 0x07*/ u8 reverbIndex;
    /*    , 0x08, 0x09*/ u8 bookOffset;
    /*    , 0x09*/ u8 newPan;
    /*    , 0x0A*/ u8 panChannelWeight;        // proportion of pan that comes from the channel (0..128)
    /*0x08, 0x0C, 0x0E*/ u16 vibratoRateStart; // initially 0x800
    /*0x0A, 0x0E, 0x10*/ u16 vibratoExtentStart;
    /*0x0C, 0x10, 0x12*/ u16 vibratoRateTarget; // initially 0x800
    /*0x0E, 0x12, 0x14*/ u16 vibratoExtentTarget;
    /*0x10, 0x14, 0x16*/ u16 vibratoRateChangeDelay;
    /*0x12, 0x16, 0x18*/ u16 vibratoExtentChangeDelay;
    /*0x14, 0x18, 0x1A*/ u16 vibratoDelay;
    /*0x16, 0x1A, 0x1C*/ u16 Delay;
    /*0x18, 0x1C, 0x1E*/ s16 InstOrWave; // either 0 (none), Instrument Index + 1, or
                                         // 0x80..0x83 for sawtooth/Triangle/sine/square waves.
    /*0x1A, 0x1E, 0x20*/ s16 Transposition;
    /*0x1C, 0x20, 0x24*/ f32 volumeScale;
    /*0x20, 0x24, 0x28*/ f32 Volume;
    /*    , 0x28*/ s32 pan;
    /*    , 0x2C*/ f32 appliedVolume;
    /*0x2C, 0x30*/ f32 FreqScale;
    /*0x30, 0x34*/ u8 (*dynTable)[][2];
    /*0x34, ????*/ struct Note* noteUnused;                  // never read
    /*0x38, ????*/ struct SequenceChannelLayer* layerUnused; // never read
    /*0x3C, 0x40*/ struct Instrument* Instrument;
    /*0x40, 0x44*/ struct SequencePlayer* seqPlayer;
    /*0x44, 0x48*/ struct SequenceChannelLayer* layers[LAYERS_MAX];
    /*0x54, 0x58      */ s8 soundScriptIO[8]; // bridge between Sound script and audio lib. For Player 2,
    // [0] contains Enabled, [4] contains Sound ID, [5] contains reverb adjustment
    /*0x5C, 0x60*/ struct M64ScriptState ScriptState;
    /*0x78, 0x7C*/ struct AdsrSettings Adsr;
    /*0x80, 0x84*/ struct NotePool NotePool;
}; // Size = 0xC0, 0xC4 in EU, 0xD0 in SH

// Also known as a Track, according to debug strings.
struct SequenceChannelLayer {
    /* U/J, EU,  SH */
    /*0x00, 0x00*/ u8 Enabled : 1;
    /*0x00, 0x00*/ u8 Finished : 1;
    /*0x00, 0x00*/ u8 StopSomething : 1;   // ?
    /*0x00, 0x00*/ u8 ContinuousNotes : 1; // keep the same Note for consecutive notes with the same Sound
    /*    , 0x00*/ u8 UnusedEu0B8 : 1;
    /*    , 0x00*/ u8 NotePropertiesNeedInit : 1;
    /*    , 0x00*/ u8 IgnoreDrumPan : 1;
    /*    , 0x01, 0x02*/ u8 InstOrWave;
    /*0x01, 0x02, 0x03*/ u8 Status; // 0x03 in SH
    /*0x02, 0x03*/ u8 NoteDuration; // set to 0x80
    /*0x03, 0x04*/ u8 PortamentoTargetNote;
    /*    , 0x05*/ u8 pan; // 0..128
    /*    , 0x06, 0x07*/ u8 NotePan;
    /*0x04, 0x08*/ struct Portamento Portamento;
    /*0x14, 0x18*/ struct AdsrSettings Adsr;
    /*0x1C, 0x20*/ u16 PortamentoTime;
    /*0x1E, 0x22*/ s16 Transposition; // #semitones added to play commands
                                      // (m64 instruction encoding only allows referring to the limited range
                                      // 0..0x3f; this makes 0x40..0x7f accessible as well)
    /*0x20, 0x24, 0x24*/ f32 FreqScale;
    /*0x24, 0x28, 0x2C*/ f32 VelocitySquare;
    /*0x2C, 0x2C, 0x30*/ f32 NoteVelocity;
    /*0x34, 0x30, 0x34*/ f32 NoteFreqScale;
    /*0x38, 0x34*/ s16 ShortNoteDefaultPlayPercentage;
    /*0x3A, 0x36*/ s16 PlayPercentage; // it's not really a percentage...
    /*0x3C, 0x38*/ s16 Delay;
    /*0x3E, 0x3A*/ s16 Duration;
    /*0x40, 0x3C*/ s16 DelayUnused; // set to 'Delay', never read
    /*0x44, 0x40, 0x44*/ struct Note* Note;
    /*0x48, 0x44*/ struct Instrument* Instrument;
    /*0x4C, 0x48*/ struct AudioBankSound* Sound;
    /*0x50, 0x4C, 0x50*/ struct SequenceChannel* SEQChannel;
    /*0x54, 0x50*/ struct M64ScriptState ScriptState;
    /*0x70, 0x6C*/ struct AudioListItem ListItem;
    u8 Pad2[4];
}; // Size = 0x80

struct NoteSynthesisState {
    /*0x00*/ u8 Restart;
    /*0x01*/ u8 SampleDmaIndex;
    /*0x02*/ u8 PrevHeadsetPanRight;
    /*0x03*/ u8 PrevHeadsetPanLeft;
    /*0x04, 0x06*/ u16 SamplePosFrac;
    /*0x08*/ s32 SamplePosInt;
    /*0x0C*/ struct NoteSynthesisBuffers* SynthesisBuffers;
    /*0x10*/ s16 CurVolLeft;  // UQ0.16 (EU Q1.15)
    /*0x12*/ s16 CurVolRight; // UQ0.16 (EU Q1.15)
};
struct NotePlaybackState {
    /* U/J, EU,   SH  */
    /*0x04, 0x00, 0x00*/ u8 Priority;
    /*      0x01, 0x01*/ u8 WaveId;
    /*      0x02, 0x02*/ u8 SampleCountIndex;
    /*0x08, 0x04, 0x06*/ s16 AdsrVolScale;
    /*0x18, 0x08, 0x08*/ f32 PortamentoFreqScale;
    /*0x1C, 0x0C, 0x0C*/ f32 VibratoFreqScale;
    /*0x28, 0x10,     */ struct SequenceChannelLayer* PrevParentLayer;
    /*0x2C, 0x14, 0x14*/ struct SequenceChannelLayer* ParentLayer;
    /*0x30, 0x18, 0x18*/ struct SequenceChannelLayer* WantedParentLayer;
    /*    , 0x1C, 0x1C*/ struct NoteAttributes Attributes;
    /*0x54, 0x28, 0x2C*/ struct AdsrState Adsr;
    /*0x74, 0x4C,     */ struct Portamento Portamento;
    /*0x84, 0x5C,     */ struct VibratoState VibratoState;
};
struct NoteSubEu {
    /*0x00*/ volatile u8 Enabled : 1;
    /*0x00*/ u8 needsInit : 1;
    /*0x00*/ u8 Finished : 1;
    /*0x00*/ u8 envMixerNeedsInit : 1;
    /*0x00*/ u8 stereoStrongRight : 1;
    /*0x00*/ u8 stereoStrongLeft : 1;
    /*0x00*/ u8 StereoHeadsetEffects : 1;
    /*0x00*/ u8 UsesHeadsetPanEffects : 1;
    /*0x01*/ u8 reverbIndex : 3;
    /*0x01*/ u8 bookOffset : 3;
    /*0x01*/ u8 isSyntheticWave : 1;
    /*0x01*/ u8 hasTwoAdpcmParts : 1;
    /*0x02*/ u8 BankId;
    /*0x03*/ u8 headsetPanRight;
    /*0x04*/ u8 headsetPanLeft;
    /*0x05*/ u8 ReverbVol;                 // UQ0.7 (EU Q1.7)
    /*0x06*/ u16 targetVolLeft;            // UQ0.12 (EU UQ0.10)
    /*0x08*/ u16 targetVolRight;           // UQ0.12 (EU UQ0.10)
    /*0x0A*/ u16 resamplingRateFixedPoint; // stored as signed but Loaded as u16
    /*0x0C*/ union {
        s16* Samples;
        struct AudioBankSound* AudioBankSound;
    } Sound;
};
struct Note {
    /* U/J,  EU,  SH  */
    /*0xA4, 0x00, 0x00*/ struct AudioListItem ListItem;
    /*      0x10, 0x10*/ struct NoteSynthesisState SynthesisState;
    // The Next members are actually part of a struct (NotePlaybackState), but
    // that results in messy US/EU ifdefs. Instead we cast to a struct Pointer
    // when needed... This breaks alignment on non-N64 platforms, which we hack
    // around by skipping the Padding in that case.
    //! @todo use macros or something instead.
#ifdef TARGET_N64
    u8 Pad0[12];
#endif

    /*0x04, 0x30, 0x30*/ u8 Priority;
    /*      0x31, 0x31*/ u8 WaveId;
    /*      0x32, 0x32*/ u8 SampleCountIndex;
    /*0x08, 0x34, 0x36*/ s16 AdsrVolScale;
    /*0x18, 0x38,     */ f32 PortamentoFreqScale;
    /*0x1C, 0x3C,     */ f32 VibratoFreqScale;
    /*0x28, 0x40,     */ struct SequenceChannelLayer* PrevParentLayer;
    /*0x2C, 0x44, 0x44*/ struct SequenceChannelLayer* ParentLayer;
    /*0x30, 0x48, 0x48*/ struct SequenceChannelLayer* WantedParentLayer;
    /*    , 0x4C, 0x4C*/ struct NoteAttributes Attributes;
    /*0x54, 0x58, 0x5C*/ struct AdsrState Adsr;
    /*0x74, 0x7C*/ struct Portamento Portamento;
    /*0x84, 0x8C*/ struct VibratoState VibratoState;
    u8 Pad3[8];
    /*    , 0xB0, 0xB4*/ struct NoteSubEu NoteSubEu;
}; // Size = 0xC0, known to be 0xC8 on SH

// While this struct needs to be Size 0xA0, its not clear
// what innards are necessary. All the possible options from
// SM64 are sized incorrectly so we're probably looking at some
// MK64 weirdness
struct NoteSynthesisBuffers {
    s16 AdpcmdecState[0x10];
    s16 FinalResampleState[0x10];
    s16 PanResampleState[0x10];
    s16 PanSamplesBuffer[0x20];
}; // Size = 0xA0

/*
SM64 NoteSynthesisBuffers, kept here because I'm not sure our Current
version is correct
struct NoteSynthesisBuffers {
    s16 AdpcmdecState[0x10];
    s16 FinalResampleState[0x10];
#ifdef VERSION_SH
    s16 unk[0x10];
    s16 filterBuffer[0x20];
    s16 PanSamplesBuffer[0x20];
#else
    s16 mixEnvelopeState[0x28];
    s16 PanResampleState[0x10];
    s16 PanSamplesBuffer[0x20];
    s16 dummyResampleState[0x10];
#if defined(VERSION_JP) || defined(VERSION_US)
    s16 Samples[0x40];
#endif
#endif
};
*/

struct ReverbSettingsEU {
    u8 DownsampleRate;
    u8 WindowSize; // To be multiplied by 64
    u16 Gain;
};

struct AudioSessionSettingsEU {
    /* 0x00 */ u32 Frequency;
    /* 0x04 */ u8 Unk1; // always 1
    /* 0x05 */ u8 MaxSimultaneousNotes;
    /* 0x06 */ u8 NumReverbs; // always 1
    /* 0x07 */ u8 Unk2;       // always 0
    /* 0x08 */ struct ReverbSettingsEU* ReverbSettings;
    /* 0x0C */ u16 Volume;
    /* 0x0E */ u16 Unk3; // always 0
    /* 0x10 */ u32 PersistentSeqMem;
    /* 0x14 */ u32 PersistentBankMem;
    /* 0x18 */ u32 unk_18;
    /* 0x1C */ u32 TemporarySeqMem;
    /* 0x20 */ u32 TemporaryBankMem;
    /* 0x24 */ u32 unk_24;
}; // 0x30 on shindou

struct AudioSessionSettings {
    /*0x00*/ u32 Frequency;
    /*0x04*/ u8 MaxSimultaneousNotes;
    /*0x05*/ u8 ReverbDownsampleRate; // always 1
    /*0x06*/ u16 ReverbWindowSize;
    /*0x08*/ u16 ReverbGain;
    /*0x0A*/ u16 Volume;
    /*0x0C*/ u32 PersistentSeqMem;
    /*0x10*/ u32 PersistentBankMem;
    /*0x14*/ u32 TemporarySeqMem;
    /*0x18*/ u32 TemporaryBankMem;
}; // Size = 0x1C

struct AudioBufferParametersEU {
    /*0x00*/ s16 PresetUnk4; // audio frames per vsync?
    /*0x02*/ u16 Frequency;
    /*0x04*/ u16 AIFrequency; // ?16
    /*0x06*/ s16 SamplesPerFrameTarget;
    /*0x08*/ s16 MaxAiBufferLength;
    /*0x0A*/ s16 MinAiBufferLength;
    /*0x0C*/ s16 UpdatesPerFrame;
    /*0x0E*/ s16 SamplesPerUpdate;
    /*0x10*/ s16 SamplesPerUpdateMax;
    /*0x12*/ s16 SamplesPerUpdateMin;
    /*0x14*/ f32 ResampleRate;             // contains 32000.0f / Frequency
    /*0x18*/ f32 UpdatesPerFrameInv;       // 1.0f / UpdatesPerFrame
    /*0x1C*/ f32 UnkUpdatesPerFrameScaled; // 3.0f / (1280.0f * UpdatesPerFrame)
};

/**
 * This definition comes from SM64. For reasons not fully understood
 * NapAudioSysProcess does not match properly if this definition
 * is Used.
 * We're either:
 *     Missing a compiler -D for IS_BIG_ENDIAN
 *     or
 *     The version of that function in MK64 is significantly different
 *     from its SM64 counterpart
 * Or we just have a poor understanding of this part of the system.

struct EuAudioCmd {
    union {
#if IS_BIG_ENDIAN
        struct {
            u8 op;
            u8 arg1;
            u8 Arg2;
            u8 Arg3;
        } s;
#else
        struct {
            u8 Arg3;
            u8 Arg2;
            u8 arg1;
            u8 op;
        } s;
#endif
        s32 first;
    } u;
    union {
        s32 AsS32;
        u32 AsU32;
        f32 AsF32;
#if IS_BIG_ENDIAN
        u8 AsU8;
        s8 AsS8;
#else
        struct {
            u8 Pad0[3];
            u8 AsU8;
        };
        struct {
            u8 pad1[3];
            s8 AsS8;
        };
#endif
    } u2;
};
**/

struct EuAudioCmd {
    union {
        struct {
            u8 op;
            u8 BankId;
            u8 Arg2;
            u8 Arg3;
        } s;
        u32 first;
    } u;
    union {
        s32 AsS32;
        u32 AsU32;
        f32 AsF32;
        u8 AsU8;
        s8 AsS8;
    } u2;
};

#endif // AUDIO_INTERNAL_H
