#ifndef AUDIO_PLAYBACK_H
#define AUDIO_PLAYBACK_H

#include <PR/ultratypes.h>

#include "internal.h"

// Mask bits denoting where to allocate notes from, according to a channel's
// noteAllocPolicy. Despite being checked as bitmask bits, the bits are not
// orthogonal; rather, the smallest bit wins, except for NOTE_ALLOC_LAYER,
// which *is* orthogonal to the other. SEQ implicitly includes CHANNEL.
// If none of the CHANNEL/SEQ/GLOBAL_FREELIST bits are set, all three locations
// are tried.
#define NOTE_ALLOC_LAYER 1
#define NOTE_ALLOC_CHANNEL 2
#define NOTE_ALLOC_SEQ 4
#define NOTE_ALLOC_GLOBAL_FREELIST 8

void Nas_smzSetVol(struct Note* note, f32 velocity, u8 pan, u8 reverbVol);
void Nas_smzSetPitch(struct Note* note, f32 resamplingRateInput);
s32 Nas_Release_Channel_Main(struct Note* note, struct SequenceChannelLayer* seqLayer, s32 waveId);
void Nas_Release_Channel(struct Note* note, struct SequenceChannelLayer* seqLayer);
void Nas_Release_Channel_Force(struct AudioListItem* list);
void Nas_WaveMemoryMake(struct NotePool* pool);
void Nas_ContinueWaveMemory(void);
void Nas_InitList(struct NotePool* pool);
void Nas_InitChNode(struct NotePool* pool, s32 count);
void Nas_InitChannelList(struct AudioListItem* list, struct AudioListItem* item);
void Nas_DeAllocAllVoices(struct AudioListItem* item);
struct Note* Nas_AllocVoices(struct AudioListItem* list, s32 limit);
void Nas_AddListHead(struct Note* note, struct SequenceChannelLayer* seqLayer);
void Nas_StopVoice(struct Note* note);
void Nas_UpdateChannel(void);
struct AudioBankSound* NoteToVoice(struct Instrument* instrument, s32 semitone);
struct Instrument* ProgToVp(s32 bankId, s32 instId);
struct Drum* PercToPp(s32 bankId, s32 drumId);
void Nas_StartVoice(struct Note* note);
void L_ReleaseEmergency(struct SequenceChannelLayer* seqLayer, s32 target);
void L_CheckTerminate(struct SequenceChannelLayer* seqLayer);
void L_loopout(struct SequenceChannelLayer* seqLayer);
void Nas_CutList(struct Note* note, struct SequenceChannelLayer* seqLayer);
void Nas_GetLowerPrio(struct Note* note, struct SequenceChannelLayer* seqLayer);
struct Note* Nas_EntryTrack(struct NotePool* pool, struct SequenceChannelLayer* seqLayer);
struct Note* Nas_InterTrack(struct NotePool* pool, struct SequenceChannelLayer* seqLayer);
struct Note* Nas_InterReleaseTrack(struct NotePool* pool, struct SequenceChannelLayer* seqLayer);
struct Note* Nas_ChLookFree(struct SequenceChannelLayer* seqLayer);
void Nas_ChLookRelease(void);

#endif // AUDIO_PLAYBACK_H
