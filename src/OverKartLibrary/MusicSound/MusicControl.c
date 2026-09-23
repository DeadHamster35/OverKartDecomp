#include "../MainInclude.h"

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//SeqID: ID of song
//Interp: Framecount
void MusSeqStart(Uchar Group, Ushort SeqID, Uchar Interp)
{
	NAISeqFlagEntry(0x00000000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Ushort)SeqID );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Interp: Framecount
void MusSeqStop(Uchar Group, Uchar Interp)
{
	NAISeqFlagEntry(0x100000ff | ((Uchar)Group << 24) | ((Uchar)Interp << 16) );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Volume: Max: 255 - Default: 127 - Half: 75 - Zero: 0
//Interp: Framecount
void MusSeqVolumeChange(Uchar Group, Uchar Volume, Uchar Interp)
{
	NAISeqFlagEntry(0x40000000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Uchar)Volume );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Interp: Framecount
void MusSeqVolumeRecover(Uchar Group, Uchar Interp)
{
	NAISeqFlagEntry(0x4000007f | ((Uchar)Group << 24) | ((Uchar)Interp << 16) );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Pitch: Double: 2000 - Default: 1000 - Half: 500 - Zero: 0
//Interp: Framecount
void MusSeqPitchChange(Uchar Group, Ushort Pitch, Uchar Interp)
{
	NAISeqFlagEntry(0x50000000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Ushort)Pitch );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Interp: Framecount
void MusSeqPitchRecover(Uchar Group, Uchar Interp)
{
	NAISeqFlagEntry(0x50000000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | 1000 );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Tempo: Song-dependent - e.g. Title: 0x1A40 / 0x30 = 0x8C (dec.140)
//Interp: Framecount
void MusSeqTempoDirect(Uchar Group, Ushort Tempo, Uchar Interp)
{
	NAISeqFlagEntry(0xB0000000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Ushort)Tempo );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//TempoAdd: 1: 0x30
//Interp: Framecount
void MusSeqTempoAdd(Uchar Group, Ushort TempoAdd, Uchar Interp)
{
	NAISeqFlagEntry(0xB0001000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Ushort)TempoAdd );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//TempoSub: 1: 0x30
//Interp: Framecount
void MusSeqTempoSub(Uchar Group, Ushort TempoSub, Uchar Interp)
{
	NAISeqFlagEntry(0xB0002000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Ushort)TempoSub );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//TempoPercent: 200: 200%; 100: 100%; 50: 50%
//Interp: Framecount
void MusSeqTempoPercent(Uchar Group, Ushort TempoPercent, Uchar Interp)
{
	NAISeqFlagEntry(0xB0003000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Ushort)TempoPercent );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Interp: Framecount
void MusSeqTempoRecover(Uchar Group, Uchar Interp)
{
	NAISeqFlagEntry(0xB0004000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) );
}

//Disable: Prevent new sequence: 1 - Allow new sequence: 0
void MusSeqPreventNext(bool Disable)
{
	NAISeqFlagEntry(0xe0000100 | (Ushort)Disable );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//SeqID: ID of song
//Interp: Framecount
void MusSeqSetNext(Uchar Group, Ushort SeqID, Uchar Interp)
{
	NAISeqFlagEntry(0x20000000 | ((Uchar)Group << 24) | ((Uchar)Interp << 16) | (Ushort)SeqID );
}

//SoundMode: Wide: 0 - Headphone: 1 - Stereo: 2 - Mono: 3
void MusSeqSoundMode(Uchar SoundMode)
{
	NAISeqFlagEntry(0xe0000000 | (Uchar)SoundMode );
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
short MusSeqGetCurrentID(Uchar Group)
{
	return (short)NAIGetPlayingSeqFlag(Group);
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
int MusSeqGetLoopCounter(Uchar Group)
{
	return SeqPlayer[(int)Group].Loopcount;
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
void MusSeqLoopImmediately(Uchar Group)
{
	SeqPlayer[(int)Group].Loopcount = 0;
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Channel: 0-15 seperate sequence Channels
//Mute: 0: Unmuted - 1: Muted
void MusChanMute(Uchar Group, Uchar Channel, Uchar Mute)
{
	if (Mute == 0)
	{
		SeqPlayer[(int)Group].Channel[(int)Channel]->Playflag ^= CHANNEL_MUTE;
	}
	else
	{
		SeqPlayer[(int)Group].Channel[(int)Channel]->Playflag |= CHANNEL_MUTE;
	}
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Channel: 0-15 seperate sequence Channels
float MusChanGetVolume(Uchar Group, Uchar Channel)
{
	return SeqPlayer[(int)Group].Channel[(int)Channel]->VolumeTarget;
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Channel: 0-15 seperate sequence Channels
//Volume: float
void MusChanSetVolume(Uchar Group, Uchar Channel, float Volume)
{
	SeqPlayer[(int)Group].Channel[(int)Channel]->VolumeTarget = Volume;
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Channel: 0-15 seperate sequence Channels
float MusChanGetPitch(Uchar Group, Uchar Channel)
{
	return SeqPlayer[(int)Group].Channel[(int)Channel]->Pitch;
}

//Group: BGM: 0 - Fanfare: 1 - SFX: 2
//Channel: 0-15 seperate sequence Channels
//Pitch: float
void MusChanSetPitch(Uchar Group, Uchar Channel, float Pitch)
{
	SeqPlayer[(int)Group].Channel[(int)Channel]->Pitch = Pitch;
}
