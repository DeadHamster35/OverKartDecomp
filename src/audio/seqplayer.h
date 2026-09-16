#ifndef AUDIO_SEQPLAYER_H
#define AUDIO_SEQPLAYER_H

#include <PR/ultratypes.h>

#include "audio/internal.h"

#define PORTAMENTO_IS_SPECIAL(x) ((x).mode & 0x80)
#define PORTAMENTO_MODE(x) ((x).mode & ~0x80)
#define PORTAMENTO_MODE_1 1
#define PORTAMENTO_MODE_2 2
#define PORTAMENTO_MODE_3 3
#define PORTAMENTO_MODE_4 4
#define PORTAMENTO_MODE_5 5

void Nas_ChannelModInit(struct SequenceChannel* seqChannel);
s32 Nas_EnvInit(struct SequenceChannel* seqChannel, s32 layerIndex);
void Nas_EnvProcess(struct SequenceChannelLayer* layer);
void ENV_RETRY(struct SequenceChannel* seqChannel, s32 layerIndex);
void Nas_InitSubTrack(struct SequenceChannel* seqChannel);
struct SequenceChannel* Nas_EntryNoteTrack(void);
void Nas_ReleaseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits);
void Nas_CloseNoteTrack(struct SequencePlayer* seqPlayer, u16 channelBits);
void Nas_ReleaseSubTrack(struct SequencePlayer* seqPlayer, u8 channelIndex, void* script);
void Nas_GetNewSubTrack(struct SequencePlayer* seqPlayer);
void func_800BEF2C(struct SequencePlayer* seqPlayer);
void Nas_AllocSub(struct AudioListItem* list, struct AudioListItem* item);
void* Nas_DeAllocSub(struct AudioListItem* list);
void Nas_OpenSub(void);
u8 Nas_ReleaseGroup(struct M64ScriptState* state);
s16 Nas_AddList(struct M64ScriptState* state);
u16 Nas_GetList(struct M64ScriptState* state);
u8 Nas_ReadByteData(struct SequenceChannel* seqChannel, u8 instId, struct Instrument** instOut,
                  struct AdsrSettings* adsr);
void Nas_ProgramChanger(s32);
void Nas_ReadWordData(struct SequenceChannel* seqChannel, u8 instId);
void Nas_ReadLengthData(struct SequenceChannel* seqChannel, u8 volume);
void Nas_InitNoteList(struct SequenceChannelLayer* layer);
void ERROR_EXIT(struct SequencePlayer*);
void Nas_NoteSeq(struct SequenceChannel*);
void Nas_ProgramChanger(s32);
void Nas_SubVoiceSet(u32 player);
void Nas_SubVolumeSet(void);

#endif // AUDIO_SEQPLAYER_H
