#ifndef AUDIO_PLAYBACK_H
#define AUDIO_PLAYBACK_H

#include <PR/ultratypes.h>

#include "internal.h"

// Mask bits denoting where to allocate notes from, according to a channel's
// NoteAllocPolicy. Despite being checked as bitmask bits, the bits are not
// orthogonal; rather, the smallest bit wins, except for NOTE_ALLOC_LAYER,
// which *is* orthogonal to the other. SEQ implicitly includes CHANNEL.
// If none of the CHANNEL/SEQ/GLOBAL_FREELIST bits are set, all three locations
// are tried.
#define NOTE_ALLOC_LAYER 1
#define NOTE_ALLOC_CHANNEL 2
#define NOTE_ALLOC_SEQ 4
#define NOTE_ALLOC_GLOBAL_FREELIST 8

void NasSmzSetVol(struct Note* Note, f32 Velocity, u8 pan, u8 ReverbVol);
void NasSmzSetPitch(struct Note* Note, f32 resamplingRateInput);
s32 NasReleaseChannelMain(struct Note* Note, struct SequenceChannelLayer* seqLayer, s32 WaveId);
void NasReleaseChannel(struct Note* Note, struct SequenceChannelLayer* seqLayer);
void NasReleaseChannelForce(struct AudioListItem* list);
void NasWaveMemoryMake(struct NotePool* Pool);
void NasContinueWaveMemory(void);
void NasInitList(struct NotePool* Pool);
void NasInitChNode(struct NotePool* Pool, s32 Count);
void NasInitChannelList(struct AudioListItem* list, struct AudioListItem* Item);
void NasDeAllocAllVoices(struct AudioListItem* Item);
struct Note* NasAllocVoices(struct AudioListItem* list, s32 Limit);
void NasAddListHead(struct Note* Note, struct SequenceChannelLayer* seqLayer);
void NasStopVoice(struct Note* Note);
void NasUpdateChannel(void);
struct AudioBankSound* NoteToVoice(struct Instrument* Instrument, s32 semitone);
struct Instrument* ProgToVp(s32 BankId, s32 instId);
struct Drum* PercToPp(s32 BankId, s32 drumId);
void NasStartVoice(struct Note* Note);
void LReleaseEmergency(struct SequenceChannelLayer* seqLayer, s32 Target);
void LCheckTerminate(struct SequenceChannelLayer* seqLayer);
void LLoopout(struct SequenceChannelLayer* seqLayer);
void NasCutList(struct Note* Note, struct SequenceChannelLayer* seqLayer);
void NasGetLowerPrio(struct Note* Note, struct SequenceChannelLayer* seqLayer);
struct Note* NasEntryTrack(struct NotePool* Pool, struct SequenceChannelLayer* seqLayer);
struct Note* NasInterTrack(struct NotePool* Pool, struct SequenceChannelLayer* seqLayer);
struct Note* NasInterReleaseTrack(struct NotePool* Pool, struct SequenceChannelLayer* seqLayer);
struct Note* NasChLookFree(struct SequenceChannelLayer* seqLayer);
void NasChLookRelease(void);

#endif // AUDIO_PLAYBACK_H
