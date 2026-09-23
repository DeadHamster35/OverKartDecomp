#ifndef MusicControlH
#define MusicControlH
#include "../MainInclude.h"
extern void MusSeqStart(Uchar Group, Ushort SeqID, Uchar Interp);
extern void MusSeqStop(Uchar Group, Uchar Interp);
extern void MusSeqVolumeChange(Uchar Group, Uchar Volume, Uchar Interp);
extern void MusSeqVolumeRecover(Uchar Group, Uchar Interp);
extern void MusSeqPitchChange(Uchar Group, Ushort Pitch, Uchar Interp);
extern void MusSeqPitchRecover(Uchar Group, Uchar Interp);
extern void MusSeqTempoDirect(Uchar Group, Ushort Tempo, Uchar Interp);
extern void MusSeqTempoAdd(Uchar Group, Ushort TempoAdd, Uchar Interp);
extern void MusSeqTempoSub(Uchar Group, Ushort TempoSub, Uchar Interp);
extern void MusSeqTempoPercent(Uchar Group, Ushort TempoPercent, Uchar Interp);
extern void MusSeqTempoRecover(Uchar Group, Uchar Interp);
extern void MusSeqPreventNext(bool Disable);
extern void MusSeqSetNext(Uchar Group, Ushort SeqID, Uchar Interp);
extern void MusSeqSoundMode(Uchar SoundMode);
extern short MusSeqGetCurrentID(Uchar Group);
extern int MusSeqGetLoopCounter(Uchar Group);
extern void MusSeqLoopImmediately(Uchar Group);
extern void MusChanMute(Uchar Group, Uchar Channel, Uchar Mute);
extern float MusChanGetVolume(Uchar Group, Uchar Channel);
extern void MusChanSetVolume(Uchar Group, Uchar Channel, float Volume);
extern float MusChanGetPitch(Uchar Group, Uchar Channel);
extern void MusChanSetPitch(Uchar Group, Uchar Channel, float Pitch);
#endif
