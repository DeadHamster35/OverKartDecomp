#ifndef AUDIO_SEQPLAYER_H
#define AUDIO_SEQPLAYER_H

#include <PR/ultratypes.h>

#include "audio/internal.h"

#define PORTAMENTO_IS_SPECIAL(x) ((x).Mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).Mode & ~0x80)
#define PORTAMENTO_MODE_1 1
#define PORTAMENTO_MODE_2 2
#define PORTAMENTO_MODE_3 3
#define PORTAMENTO_MODE_4 4
#define PORTAMENTO_MODE_5 5

void NasChannelModInit(struct SequenceChannel* SEQChannel);
s32 NasEnvInit(struct SequenceChannel* SEQChannel, s32 layerIndex);
void NasEnvProcess(struct SequenceChannelLayer* layer);
void ENV_RETRY(struct SequenceChannel* SEQChannel, s32 layerIndex);
void NasInitSubTrack(struct SequenceChannel* SEQChannel);
struct SequenceChannel* NasEntryNoteTrack(void);
void NasReleaseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits);
void NasCloseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits);
void NasReleaseSubTrack(struct SequencePlayer* seqPlayer, u8 channelIndex, void* script);
void NasGetNewSubTrack(struct SequencePlayer* seqPlayer);
void func_800BEF2C(struct SequencePlayer* seqPlayer);
void NasAllocSub(struct AudioListItem* list, struct AudioListItem* Item);
void* NasDeAllocSub(struct AudioListItem* list);
void NasOpenSub(void);
u8 NasReleaseGroup(struct M64ScriptState* State);
s16 NasAddList(struct M64ScriptState* State);
u16 NasGetList(struct M64ScriptState* State);
u8 NasReadByteData(struct SequenceChannel* SEQChannel, u8 instId, struct Instrument** instOut,
                  struct AdsrSettings* Adsr);
void NasProgramChanger(s32);
void NasReadWordData(struct SequenceChannel* SEQChannel, u8 instId);
void NasReadLengthData(struct SequenceChannel* SEQChannel, u8 Volume);
void NasInitNoteList(struct SequenceChannelLayer* layer);
void ERROR_EXIT(struct SequencePlayer*);
void NasNoteSeq(struct SequenceChannel*);
void NasProgramChanger(s32);
void NasSubVoiceSet(u32 Player);
void NasSubVolumeSet(void);

#endif // AUDIO_SEQPLAYER_H
