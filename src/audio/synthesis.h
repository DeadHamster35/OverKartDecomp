#ifndef AUDIO_SYNTHESIS_H
#define AUDIO_SYNTHESIS_H

#include "audio/internal.h"
#include <PR/abi.h>

#define DEFAULT_LEN_1CH 0x180
#define DEFAULT_LEN_2CH 0x300

#define DMEM_ADDR_TEMP 0x0
#define DMEM_ADDR_RESAMPLED 0x20
#define DMEM_ADDR_RESAMPLED2 0x1A0
#define DMEM_ADDR_UNCOMPRESSED_NOTE 0x180
#define DMEM_ADDR_NOTE_PAN_TEMP 0x200
#define DMEM_ADDR_STEREO_STRONG_TEMP_DRY 0x200
#define DMEM_ADDR_STEREO_STRONG_TEMP_WET 0x340
#define DMEM_ADDR_COMPRESSED_ADPCM_DATA 0x3f0
#define DMEM_ADDR_LEFT_CH 0x540
#define DMEM_ADDR_RIGHT_CH 0x6C0
#define DMEM_ADDR_WET_LEFT_CH 0x840
#define DMEM_ADDR_WET_RIGHT_CH 0x9C0

/*
Its not clear what values these macros should have. Neither version seem to
line up for MK64. Maybe each game has unique values for these? I don't know
enough about the Audio engine stuff to speculate beyond that.

Non-Shindou SM64
#define DMEM_ADDR_TEMP 0x0
#define DMEM_ADDR_RESAMPLED 0x20
#define DMEM_ADDR_RESAMPLED2 0x160
#define DMEM_ADDR_UNCOMPRESSED_NOTE 0x180
#define DMEM_ADDR_NOTE_PAN_TEMP 0x200
#define DMEM_ADDR_STEREO_STRONG_TEMP_DRY 0x200
#define DMEM_ADDR_STEREO_STRONG_TEMP_WET 0x340
#define DMEM_ADDR_COMPRESSED_ADPCM_DATA 0x3f0
#define DMEM_ADDR_LEFT_CH 0x4c0
#define DMEM_ADDR_RIGHT_CH 0x600
#define DMEM_ADDR_WET_LEFT_CH 0x740
#define DMEM_ADDR_WET_RIGHT_CH 0x880

Shindou SM64
#define DMEM_ADDR_TEMP 0x450
#define DMEM_ADDR_RESAMPLED 0x470
#define DMEM_ADDR_RESAMPLED2 0x5f0
#define DMEM_ADDR_UNCOMPRESSED_NOTE 0x5f0
#define DMEM_ADDR_NOTE_PAN_TEMP 0x650
#define DMEM_ADDR_COMPRESSED_ADPCM_DATA 0x990
#define DMEM_ADDR_LEFT_CH 0x990
#define DMEM_ADDR_RIGHT_CH 0xb10
#define DMEM_ADDR_WET_LEFT_CH 0xc90
#define DMEM_ADDR_WET_RIGHT_CH 0xe10
*/

#define MAX_UPDATES_PER_FRAME 5

struct ReverbRingBufferItem {
    /* 0x00 */ s16 NumSamplesAfterDownsampling;
    /* 0x02 */ s16 ChunkLen; // never read
    /* 0x04 */ s16* ToDownsampleLeft;
    /* 0x08 */ s16* ToDownsampleRight; // data pointed to by Left and Right are adjacent in memory
    /* 0x0C */ s32 StartPos;           // Start pos in ring Buffer
    /* 0x10 */ s16 LengthA;            // first Length in ring Buffer (from StartPos, at most until end)
    /* 0x12 */ s16 LengthB;            // second Length in ring Buffer (from pos 0)
}; // Size = 0x14

struct SynthesisReverb {
    /* 0x00 */ u8 resampleFlags;
    /* 0x01 */ u8 useReverb;
    /* 0x02 */ u8 framesLeftToIgnore;
    /* 0x03 */ u8 curFrame;
    /* 0x04 */ u8 DownsampleRate;
    /* 0x05 */                 // u8 CompilerPadding;
    /* 0x06 */ u16 WindowSize; // same as bufSizePerChannel
    /* 0x08 */ u16 ReverbGain;
    /* 0x0A */ u16 ResampleRate;
    /* 0x0C */ s32 nextRingBufferPos;
    /* 0x10 */ s32 UnkC; // never read
    /* 0x14 */ s32 bufSizePerChannel;
    struct {
        /* 0x18 */ s16* Left;
        /* 0x1C */ s16* Right;
    } RingBuffer;
    /* 0x20 */ s16* resampleStateLeft;
    /* 0x24 */ s16* resampleStateRight;
    /* 0x28 */ s16* unk24; // never read
    /* 0x2C */ s16* unk28; // never read
    /* 0x30 */ struct ReverbRingBufferItem items[2][MAX_UPDATES_PER_FRAME];
    /* 0xF8  */ s16* unkF8;
    /* 0xFC  */ s16* unkFC;
    /* 0x100 */ s16* unk100;
    /* 0x104 */ s16* unk104;
}; // Size = 0x108

#define ALIGN(val, amnt) (((val) + (1 << amnt) - 1) & ~((1 << amnt) - 1))

void NasCpuFX(s32 ChunkLen, u32 updateIndex, s32 reverbIndex);
Acmd* LoadAuxBuf(Acmd*, u16, u16, s32, s32);
Acmd* SaveAuxBuf(Acmd*, u16, u16, s32, s32);
void NasWaveTerminateProcess(s32 updateIndexStart, s32 noteIndex);
void NasPushDrvReg(s32 updateIndex);
Acmd* NasSmzAudioFrame(Acmd*, s32*, s16*, s32);
Acmd* NasLoadAuxBuffer(Acmd*, s32, s16, s16);
Acmd* NasSaveAuxBuffer(Acmd*, s16, s16);
Acmd* NasDriveRsp(s16*, s32, Acmd*, s32);
Acmd* NasSynthMain(s32 noteIndex, struct NoteSubEu* NoteSubEu, struct NoteSynthesisState* SynthesisState,
                             s16* aiBuf, s32 inBuf, Acmd* cmd, s32 updateIndex);
Acmd* NasSynthWaveMemory(Acmd* acmd, struct NoteSubEu* NoteSubEu, struct NoteSynthesisState* SynthesisState,
                        s32 nSamplesToLoad);
Acmd* NasSynthResample(Acmd* acmd, struct NoteSynthesisState* SynthesisState, s32 Count, u16 Pitch, u16 dmemIn,
                     u32 flags);
Acmd* NasSynthEnvelope(Acmd* cmd, struct NoteSubEu* Note, struct NoteSynthesisState* SynthesisState, s32 nSamples,
                    u16 inBuf, s32 headsetPanSettings, u32 flags);
Acmd* NasSynthDelay(Acmd* acmd, struct NoteSubEu* NoteSubEu, struct NoteSynthesisState* Note,
                                     s32 bufLen, s32 flags, s32 leftRight);

extern struct SynthesisReverb SynthesisReverbs[4];

/*
SO
gLeftVolRampings is almost certainly AudioSessionPool (D_803AFBC8) in disguise, but since
synthesis' ro/data has already been Broken out it'll be very hard to fix that.

extern f32 gLeftVolRampings[3][1024];
extern f32 gRightVolRampings[3][1024];
extern f32 *gCurrentLeftVolRamping; // Points to any of the three Left buffers above
extern f32 *gCurrentRightVolRamping; // Points to any of the three Right buffers above
*/

#endif // AUDIO_SYNTHESIS_H
