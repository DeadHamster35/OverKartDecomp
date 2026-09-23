#ifndef AUDIO_EXTERNAL_H
#define AUDIO_EXTERNAL_H

#include <common_structs.h>
#include "camera.h"

#define ADSR_STATE_DISABLED 0
#define ADSR_STATE_INITIAL 1
#define ADSR_STATE_START_LOOP 2
#define ADSR_STATE_LOOP 3
#define ADSR_STATE_FADE 4
#define ADSR_STATE_HANG 5
#define ADSR_STATE_DECAY 6
#define ADSR_STATE_RELEASE 7
#define ADSR_STATE_SUSTAIN 8

#define ADSR_ACTION_RELEASE 0x10
#define ADSR_ACTION_DECAY 0x20
#define ADSR_ACTION_HANG 0x40

#define ADSR_DISABLE 0
#define ADSR_HANG -1
#define ADSR_GOTO -2
#define ADSR_RESTART -3

// Sequence arguments, passed to seq_player_play_sequence. SEQId may be bit-OR'ed with
// SEQ_VARIATION; this will load the same sequence, but set a variation
// bit which may be read by the sequence script.
#define SEQUENCE_ARGS(Priority, SEQId) ((Priority << 8) | SEQId)

#define SOUND_MODE_STEREO 0
#define SOUND_MODE_MONO 3
#define SOUND_MODE_HEADSET 1

#define SEQ_PLAYER_LEVEL 0 // Level background music
#define SEQ_PLAYER_ENV 1   // Misc music like the puzzle jingle
#define SEQ_PLAYER_SFX 2   // Sound effects

#define SOUND_BANK_UNK0 0
#define SOUND_BANK_UNK1 1
#define SOUND_BANK_UNK2 2
#define SOUND_BANK_UNK3 3
#define SOUND_BANK_UNK4 4
#define SOUND_BANK_UNK5 5
#define SOUND_BANK_COUNT 6

// Almost certainly an expanded version of struct Sound from SM64
struct Sound {
    /* 0x00 */ u32 SoundBits;
    /* 0x04 */ Vec3f* Position;
    /* 0x08 */ u8 CameraId; // PlayerId? some indicator of "who" the Sound is desitned for
    /* 0x09 */              // u8  compilerPadding0[3];
    /* 0x0C */ f32* unk0C;
    /* 0x10 */ f32* unk10;
    /* 0x14 */ s8* unk14;
}; // Size = 0x18

struct Unk_800E9F7C {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ s8 unk_14;
    /* 0x15 */ // s8 comilerPadding[3];
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ f32 unk_24;
    /* 0x28 */ f32 unk_28;
    /* 0x2C */ f32 unk_2C;
    /* 0x30 */ f32 unk_30;
    /* 0x34 */ f32 unk_34;
    /* 0x38 */ f32 unk_38;
}; // Size = 0x3C

// I'm speculating that this struct has something to do with
// the doppler effect that happens to some sounds
struct Unk_8018EFD8 {
    // Pointers to a Position
    /* 0x00 */ f32* PosX;
    /* 0x04 */ f32* PosY;
    /* 0x08 */ f32* PosZ;
    // Pointers to a Velocity
    /* 0x0C */ f32* VelX;
    /* 0x10 */ f32* VelY;
    /* 0x14 */ f32* VelZ;
    /* 0x18 */ Vec3f unk18;
    /* 0x24 */ f32* unk24;  // Some Type of multiplier/scalar
                            // Indexes in D_8018EFD8, not really sure what they're for though
    /* 0x28 */ u8 CameraId; // PlayerId? some indicator of "who" the Sound is desitned for
    /* 0x29 */ u8 Prev;
    /* 0x2A */ u8 Next;
    /* 0x2B */ // u8 compilerPadding00;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ // u8 compilerPadding01[3];
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u32 SoundBits;
}; // Size = 0x3C

// This is almost certainly an expanded version of SoundCharacteristics from SM64
struct SoundCharacteristics {
    // These f32 pointers appear to Point to the `Vec3f unk18` members in a struct Unk_8018EFD8
    /* 0x00 */ Vec3f* unk00;
    /* 0x04 */ f32* unk04;
    /* 0x08 */ f32* unk08;
    /* 0x0C */ u8 CameraId; // PlayerId? some indicator of "who" the Sound is desitned for
    /* 0x0D */              // u8 compilerPadding0[3];
    /* 0x10 */ f32* unk10;
    /* 0x14 */ f32* unk14;
    /* 0x18 */ s8* unk18;
    /* 0x1C */ f32 Distance; // XZ Distance from the Camera
    /* 0x20 */ u32 Priority;
    /* 0x24 */ u32 SoundBits;
    /* 0x28 */ u8 SoundStatus;
    /* 0x29 */ u8 Freshness;
    /* 0x2A */ u8 Prev;
    /* 0x2B */ u8 Next;
    /* 0x2C */ u8 unk2C;
}; // Size = 0x30

struct Unk_800EA06C {
    /* 0x00 */ Vec3f unk00;
    /* 0x0C */ u8 unk0C;
    /* 0x0D */ // u8 compilerPadding0[3];
}; // Size = 0x10

typedef struct {
    /* 0x00 */ f32 unk_00;
    /* 0x04 */ f32 unk_04;
    /* 0x06 */ // u16 CompilerPadding;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ // u16 CompilerPadding;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ f32 unk_14;
    /* 0x16 */ // u16 CompilerPadding;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ u16 unk_1C;
    /* 0x1E */             // u16 CompilerPadding;
} struct_D_801930D0_inner; // Size = 0x20

typedef struct {
    // This will need lots of expanding/documenting
    // Its 147 words of space being Used
    /* 0x000 */ f32 Unk000;
    /* 0x004 */ f32 Unk004;
    /* 0x008 */ f32 Unk008;
    /* 0x00C */ u16 Unk00C;
    /* 0x00E */ u8 Unk00E[3]; // Array Size guessed
    /* 0x011 */ u8 Unk011;
    /* 0x012 */ u8 Unk012;
    /* 0x013 */ u8 Unk013;
    /* 0x014 */ u32 Unk014;
    /* 0x018 */ u16 Unk018;
    /* 0x01A */ u16 Unk01A;
    /* 0x01C */ f32 Unk01C;
    /* 0x020 */ f32 Unk020;
    /* 0x024 */ f32 Unk024;
    /* 0x028 */ u16 Unk028;
    /* 0x02A */ u16 Unk02A;
    /* 0x02C */ u32 Unk02C[5]; // Array Size guessed
    /* 0x040 */ u8 Unk040;
    /* 0x041 */ u8 Unk041;
    /* 0x042 */ u8 Unk042;
    /* 0x043 */ u8 Unk043;
    /* 0x044 */ struct_D_801930D0_inner Unk044[16];
    /* 0x244 */ u16 Unk244;
    /* 0x246 */ u16 Unk246;
    /* 0x248 */ u16 Unk248;
    /* 0x24A */ u16 Unk24A;
} struct_D_801930D0_entry; // Size = 0x24C

// Not quite the same as the struct found in SM64,
// not even sure that the struct or its members are even
// correct in the MK64 context
struct ChannelVolumeScaleFade {
    /* 0x000 */ f32 Current;
    /* 0x004 */ f32 Target;
    /* 0x008 */ f32 Velocity;
    /* 0x00C */ u16 RemainingFrames;
    /* 0x00A */ // u16 CompilerPadding;
}; // Size = 0x10

typedef struct {
    f32 Unk0;
    f32 unk4;
    u8 Unk8;
    u8 Unk9;
} UnkStruct8018EF18; // Size = 0xC

typedef struct {
    u8 Thing0;
    u8 Thing1;
} struct_D_80192CA8_entry;

/** @cond */

void NaPlyEngStart(u8);
void NaPlyLandStart(u8, f32);
void func_800C13F0(void);
void NaaFxmixSet(OSMesg);
f32 NaaPanSet(u8, u8);
s8 NaaTuneSet(u8, u8, u8);
s8 NaaPlaySeParaSet(f32, f32, u8);
f32 NaaEntryCameraPosWork(u8, u8);
void NaaCalcCameraPos(u8, u8, u8);
struct Unk_8018EFD8* NaaCalcdoppler(u8, Vec3f, Vec3f, f32*, u8, u32);
void NaaCheckCameraPos(Camera*, Vec3s, struct Unk_8018EFD8*);
void NaaWposptToCpospt(Camera*, Vec3f, struct Unk_8018EFD8*);
void NaaCheckSubTrackPort(void);

Vec3f* NaaInitApliWork(Vec3f, u32);
void NaiStartSeqSet(u8);
void NaiStopSeqSet(void);
void NaiSeqControl(u8, u8, u8, u16);
void func_800C29B4(u8, u16);

void NaiSeqGameFrameWork(void);
void NAISeqFlagEntry(u32);
void NaiSeqBufFlagCheck(u32);
void NaiGetPlayingSeqFlag(void);
u16 NAIGetPlayingSeqFlag(u8);
void NaiClearSeqECheckFlag(u8, u8);
u8 NaiCheckSeqFlagBuffer2(s32);
void NaiClearAllSeqFlag(u8);
void NaiSetSeqLineVolMove(u8, u8, u8, u8);
void NaiSEQInterInit(void);

void NaiSEQInterInitSpec(void);
void NaiFxSetLockFlag(u16);
void NaiFxClearBgmMuteFlag(u8);
void NAIFxFlagEntry(u32, Vec3f*, u8, f32*, f32*, s8*);
void NaiStopBufferFx(u8, struct SoundCharacteristics*);
void NaiEntryBufferFx(void);
void NaiFxFlagFree(u8, u8);
void NaiFxNextPlayCheck(u8);
void NaiFxNowPlayCheck(u8);

void NaiStopAllHandleFx(u8);
void NaiStopAllObjHandleFx2(u8, Vec3f*);
void NaiStopAllObjHandleFx(u8, Vec3f*);
void NaiStopAllObjFx(Vec3f*);
void NaiStopObjFlagFx(Vec3f*, u32);
void NaiStopFx(u32);
void NaiFxBufFlagCheck(void);
void NaiSetFxHandleVolume(u8, u8, u16);
void NaiFxHandleFrameCheck(u8);
void NaiFxPlayCheck(void);
void NaiFxInterInit(void);
void NaaPlyEngWaveLoad(void);
void func_800C5C40(void);
void NaiInitInterface(void);
void AccelOnoff(u8);
void AccelVolume(u8);

void EngineFreqSet(u8);
void LevelVolumeSet(u8);
void LevelFreqSet(u8);
void LevelSoundTrigger(u8);

void MakeLevelSoundTrg(u8);
void GoalRoutine(u8);

void DiveRoutine(u8);
void MycarThunderOkidoki(u8);
void ThunderedRoutine(u8);
void ThunderStopRoutine(void);
void KanseiLowStopRoutine(void);
void TunnelKanseiKinshi(void);
void EnemyEngineVolume(u8);
void MakeSoundProgram(void);
void PlaySound(s32);
void NaSeqStart(u16);
void NaFanStart(u16);
void NAMusicVolume(u8);
void NaPlyLevelStart(u8, u32);

void NaPlyLevelStop(u8, u32);
void NAPlyTrgStart(u8, u32);
void NAPlyVoiceStart(u8, u32);
void NaPlyDashStart(u8);
void NaEnmVoiceStart(u8, u32);
void NaPlyEngStart(u8);
void NaPlyEngStop(u8);
void NAEnmTrgStart(Vec3f, Vec3f, u32);
void NAEnmTrgStop(Vec3f, s32);
void NaEnmLevelStart(u8);
void NaEnmLevelStop(u8);
void NaSceneLevelStart(Vec3f, Vec3f, u32);
void NaSceneLevelStop(Vec3f, u32);
void NaPauseSet(u8);

void NaChangeSoundMode(u8, u8);
void NaSeMuteTitle(void);
void NaSeMuteTitleOff(void);
void NaSeMuteHyoushou(void);
void AllLevelSEStop(void);
void NaSeMuteGoal(u8);
void NaSeMuteOff(u8);
void NaPanDeforume(void);
void NAPlyEchoOn(u8, s8);
void NAPlyEchoOff(u8);
void NaEnmEchoOn(u8, s8);
void NaEnmEchoOff(u8);
void NaSeqFadeout(u8);
void NaSEFadeout(u8);
void NaGoalFanfareSet(u16, u16);
void NaFanfareStart(u8);
void NaStarBGMStart(u8);
void NaEnmMutekiStart(u8);
void NaEnmMutekiStop(u8);
void NaPlyThunderStart(u8);
void NaPlyThunderStop(void);
void NaEnmThunderStart(u8);
void NaEnmThunderStop(s32);
void NaPlyCrushStart(u8, f32);
void NaPlyObakeStart(u8);

void NaFourthInit(void);
void NaFourthRoutine(void);
void NasAudioInput(void);
void NapSendStart(void);

// This is some from other file, its definitely not part of audio's data/bss
extern s8 D_801657E5;

// Based on SM64, this should be part of data.c's BSS section. Somehow.
extern s32 AudioErrorFlags;

extern s8 D_8018EF10;
extern UnkStruct8018EF18 D_8018EF18[16];
extern struct Unk_8018EFD8 D_8018EFD8[];
// These are indexes for D_8018EFD8, but their purpose is Unknown
extern u8 D_8018FB90;
extern u8 D_8018FB91;
extern Camera* CopyCamera[4];
extern Vec3f VelocityCamera[4];
extern Vec3f CameraLastPos[4];
extern u8 D_8018FC08;
extern s16 D_8018FC10[4][2];

extern struct SoundCharacteristics SSoundBanks[SOUND_BANK_COUNT][20];
extern u8 SSoundBankUsedListBack[SOUND_BANK_COUNT];
extern u8 SSoundBankFreeListFront[SOUND_BANK_COUNT];
extern u8 SNumSoundsInBank[SOUND_BANK_COUNT];
extern u8 D_80192AB8[SOUND_BANK_COUNT][8][8];
extern u8 D_80192C38;
extern u8 SSoundBankDisabled[SOUND_BANK_COUNT];
extern struct ChannelVolumeScaleFade D_80192C48[SOUND_BANK_COUNT];
extern struct_D_80192CA8_entry D_80192CA8[3][5];
extern u8 D_80192CC6[3];
extern u32 D_80192CD0[256];
extern struct_D_801930D0_entry D_801930D0[3];
extern u8 SNumProcessedSoundRequests;
extern u8 SSoundRequestCount;
extern struct Sound SSoundRequests[0x100];

// Data Entries
extern u8 D_800E9DA0;
extern s32 D_800E9DB4[];
extern f32 D_800E9DC4[4]; // = { 1.0f, 1.0f, 1.0f, 1.0f };
extern f32 D_800E9DD4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9DE4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9DF4[8]; // = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
extern s32 D_800E9E14[4]; // = { 0, 0, 0, 0 };
extern s32 D_800E9E24[4]; // = { 0, 0, 0, 0 };
extern s32 D_800E9E34[8];
extern f32 D_800E9E54[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f }; /* const */
extern f32 D_800E9E64[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern s32 PlayerSurfaceSoundID[4]; // = { 0, 0, 0, 0 };          /* const */
extern s32 D_800E9E84[4];
extern u32 D_800E9E94[4];
extern s32 D_800E9EA4[4]; // = { 0, 0, 0, 0 };
extern f32 D_800E9EB4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9EC4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9ED4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9EE4[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800E9EF4[4];
extern f32 D_800E9F04[4]; // = { 1.0f, 1.0f, 1.0f, 1.0f };
extern f32 D_800E9F14[4]; // = { 1.0f, 1.0f, 1.0f, 1.0f }; /* const */
extern u8 LightningFlagPlayer1[8];  // = { 0, 0, 0, 0, 0, 0, 0, 0 };
extern u8 D_800E9F2C[8];  // = { 0, 0, 0, 0, 0, 0, 0, 0 };
extern f32 D_800E9F34[8]; // = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; /* const */
extern f32 D_800E9F54[8]; // = { 1.0f, 1.0f, 1.0f, 1.0f }; /* const */
extern u8 AsmDisableEngineSound[4];
extern u8 D_800E9F78[4];
extern struct Unk_800E9F7C D_800E9F7C[4];
extern u8 D_800E9F90[];
extern struct Unk_800EA06C D_800EA06C[8];
extern u8 D_800EA0EC[];
extern u8 D_800EA0F0; // = 0;
extern u8 D_800EA0F4;
extern u8 D_800EA104;
extern u8 D_800EA108; // = 0;
extern u8 D_800EA10C[];
extern f32 D_800EA110[4]; // = { 0.0f, 0.0f, 0.0f, 0.0f };
extern f32 D_800EA120[4];
extern f32 D_800EA130[8];
extern f32 D_800EA150; // = 1.4f;
extern u8 D_800EA154[];
extern u16 D_800EA15C;
extern u16 D_800EA160;
extern u8 D_800EA164;
extern s8 LightningFlag;
extern u8 D_800EA170[];
extern u16 D_800EA174; // = 0;
extern f32 D_800EA178;
extern f32 D_800EA17C;
extern u16 D_800EA180; // = 0;                          /* const */
extern u16 D_800EA184;
extern u8 D_800EA188[][6];
extern u8 D_800EA1A0[][6];
extern u8 D_800EA1C0; // = 0;
extern u16 D_800EA1C4;
// Most similar to gGlobalSoundSource from SM64, but I don't know if its really
// a Sound Source, its usage makes it look like a 0'd Vec3f for general usage
extern Vec3f D_800EA1C8; // = {0.0f, 0.0f, 0.0f}
extern f32 D_800EA1D4;
extern s8 D_800EA1DC; // = 0;
extern u8 D_800EA1E4;
extern u8 D_800EA1E8;
extern u8 D_800EA1EC;
extern u8 D_800EA1F0[];
extern u8 D_800EA1F4[];
extern u8 D_800EA244;

extern s8 GamePausedFlag;

extern OSMesgQueue* D_800EA3B0;
extern OSMesgQueue* D_800EA3B4;

/** @endcond */

#endif
