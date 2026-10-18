#include "../MainInclude.h"
#include <segments.h>
#include "buffers.h"
#include "replays.h"
#include "textures.h"

extern u16 *TexBuff;
extern s32 AllTexBlockNum;
extern MenuTexture *D_800E7D74[];
extern MenuTexture *D_800E7DC4[];
extern void *GetTBPointer(const void *Addr);
extern u8 _textures_0aSegmentRomStart[];
extern u8 _textures_0bSegmentRomStart[];

typedef struct {
	u64 *TextureData;
	s32 Offset;
} OKTexMap;

extern OKTexMap SMenuTextureMap[];

static int sStockAudioSaved = 0;
static int sCourseTableSaved = 0;
static int sCustomMenuLive = 0;



unsigned char ScaleXMode = 2, ScaleYMode = 2, ScaleZMode = 2, ScalePad = 0;
float LevelScales[7] =
{
	0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f
};
short ZFLIP, YFLIP;

void FPSCheck(void)
{
	static Uint ClockCycle, OldCycle;
	static float CycleCount;
	static float FPS;
	short FPSCheck;

	ClockCycle = osGetCount();
	CycleCount = (ClockCycle - OldCycle);
	/* CPU2SEC */
	FPS = (1 * 0x2CB5E16) / CycleCount;

	FPSCheck = (short)FPS;
	OldCycle = ClockCycle;

	/* Modifiers for FPS drops */
	/* Those values work well already, but can be tweaked further */
	if (FPSCheck >= 22)
	{
		DynFPSModifier = 2;
	}

	else if (FPSCheck >= 15)
	{
		DynFPSModifier = 3;
	}

	else if (FPSCheck >= 11)
	{
		DynFPSModifier = 4;
	}

	else if (FPSCheck >= 8)
	{
		DynFPSModifier = 5;
	}

	else
	{
		DynFPSModifier = 6;
	}
}

void DynamicTempo(void)
{
	FPSCheck();
}

void StaticTempo(int Tempo)
{
	DynFPSModifier = (short) Tempo;
}

void ApplyRaceTempo(void)
{
	if (OKStaticTempoOnly != 0) {
		DynFPSModifier = 2;
	}
	Framerate = DynFPSModifier;
}

void PreviewRefresh(void)
{
	if (MapIsCurrentlyLoading)
	{
		return;
	}
	// This Value will cause the game to try to reload preview images
	// Used on the Map Select screen when we swap to a new set of custom levels.
	if (GameMode != GAMEMODE_BATTLE)
	{
		g_menuPreviewValue1 = 9;
		g_menuPreviewValue2 = 9;
		g_menuPreviewValue3 = 9;
		g_menuPreviewValue4 = 9;
	}
	else
	{
		g_BattlePreviewValue1 = 9;
		g_BattlePreviewValue2 = 9;
		g_BattlePreviewValue3 = 9;
		g_BattlePreviewValue4 = 9;
	}
}

void CopyCourseTable(int copyMode)
{
	// 0 to restore stock cups, 1 to save. First restore snapshots
	// the live Table so Map-Select enter never copies empty BSS.
	DataLength = 0x28;
	if (copyMode == 0)
	{
		if (sCourseTableSaved == 0)
		{
			*SourceAddress = (long)(&g_CupArray[0]);
			*TargetAddress = (long)&ok_CourseTable;
			RunRAM();
			sCourseTableSaved = 1;
			return;
		}
		*SourceAddress = (long)&ok_CourseTable;
		*TargetAddress = (long)(&g_CupArray[0]);
	}
	else
	{
		*SourceAddress = (long)(&g_CupArray[0]);
		*TargetAddress = (long)&ok_CourseTable;
		sCourseTableSaved = 1;
	}
	RunRAM();
}

void HsTableSet(void)
{
	int Index;

	for (Index = 0; Index < 20; Index++)
	{
		g_CupArray[Index] = Index;
	}
}

/*
void stockTableSet(void)
{
	g_cup0Array0 = 8;
	g_cup0Array1 = 9;
	g_cup0Array2 = 6;
	g_cup0Array3 = 11;
	g_cup1Array0 = 10;
	g_cup1Array1 = 5;
	g_cup1Array2 = 1;
	g_cup1Array3 = 0;
	g_cup2Array0 = 14;
	g_cup2Array1 = 12;
	g_cup2Array2 = 7;
	g_cup2Array3 = 2;
	g_cup3Array0 = 18;
	g_cup3Array1 = 4;
	g_cup3Array2 = 3;
	g_cup3Array3 = 13;
}
*/

void StockASM(void)
{
	// these functions will swap the ASM routines that load Mario's Raceway.
	// they will Point to the custom setup we've implemented for HotSwap courses.
	// this does not swap in the course header Table, only the ASM instructions.

	/* highpolypipe*..surfacemap*: stock LUI/ORI of MR DLs and object lists.
	 * Native: DisplayKT1Hook / InitialMapObjectCode / SearchListFileHook. */

	g_pathLength = 0x0258;
	pathOffset = 0x060057B0;
	pathOffsetB = 0x06005568;

	CopyCourseTable(0);
}
void OverkartASM(void)
{
	// these functions will swap the ASM routines that load Mario's Raceway.
	// they will Point to the custom setup we've implemented for custom courses.
	// this does not swap in the course header Table, only the ASM instructions.

	/* highpolypipe*..unknown*: stock LUI/ORI of MR DLs. Native: DisplayKT1Hook. */

	CopyCourseTable(1);
	g_cup0Array0 = 0;
	g_cup0Array1 = 1;
	g_cup0Array2 = 2;
	g_cup0Array3 = 3;
	g_cup1Array0 = 4;
	g_cup1Array1 = 5;
	g_cup1Array2 = 6;
	g_cup1Array3 = 7;
	g_cup2Array0 = 8;
	g_cup2Array1 = 9;
	g_cup2Array2 = 10;
	g_cup2Array3 = 11;
	g_cup3Array0 = 12;
	g_cup3Array1 = 13;
	g_cup3Array2 = 14;
	g_cup3Array3 = 15;
	g_cupBArray0 = 16;
	g_cupBArray1 = 17;
	g_cupBArray2 = 18;
	g_cupBArray3 = 19;
}



typedef struct OK64AudioTable
{
	Uint SeqPointer;
	Uint SeqSize;
	Uint InsPointer;
	Uint InsSize;
} OK64AudioTable;

void BackupStockAudioTables(void)
{
	if (sStockAudioSaved != 0)
	{
		return;
	}
	if ((MUSTablePointer == NULL) || (AlCtlHeader == NULL) || (AlTbl == NULL))
	{
		return;
	}

	DataLength = 8;
	*SourceAddress = (int)&(*(SequenceTable *)MUSTablePointer).Pointer[3].Address;
	*TargetAddress = (int)&ok_Sequence;
	RunRAM();

	*SourceAddress = (int)&(*(InstrumentTable *)AlCtlHeader).Pointer[3];
	*TargetAddress = (int)&ok_Instrument;
	RunRAM();

	*(long *)(&ok_USAudio) = g_MUSRawAudioTable.Pointer[0].Address;
	*(long *)(&ok_USAudio + 1) = (*(InstrumentTable *)AlCtlHeader).Pointer[0].Address;
	*(long *)(&ok_MRSong) = (*(SequenceTable *)MUSTablePointer).Pointer[3].Address;
	*(long *)(&ok_MRSong + 1) = (*(SequenceTable *)MUSTablePointer).Pointer[3].Length;
	sStockAudioSaved = 1;
}

s32 StartRaceBGMCustom(void)
{
	if ((HotSwapID > 0) && ((Uint)OverKartHeader.Version != 0xFFFFFFFFu) && (OverKartHeader.MusicID < 50))
	{
		NaSeqStart((u16) OverKartHeader.MusicID);
		return 1;
	}
	return 0;
}

void SetSong(void)
{
	OK64AudioTable *SongData;

	BackupStockAudioTables();
	if ((MUSTablePointer == NULL) || (AlCtlHeader == NULL))
	{
		return;
	}

	/* MusicID < 50 is a SeqId; StartRaceBGMCustom plays it. MusicID >= 50 is a ROM Pointer. */
	if ((HotSwapID > 0) && ((Uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{

		if (OverKartHeader.MusicID < 50)
		{
			DataLength = 8;
			*SourceAddress = (int)&ok_Sequence;
			*TargetAddress = (int)&(*(SequenceTable *)MUSTablePointer).Pointer[3].Address;
			RunRAM();
			*SourceAddress = (int)&ok_Instrument;
			*TargetAddress = (int)&(*(InstrumentTable *)AlCtlHeader).Pointer[3].Address;
			RunRAM();
		}
		else
		{
			*TargetAddress = (int)&ok_FreeSpace;
			*SourceAddress = OverKartHeader.MusicID;
			DataLength = 64;
			RunDMA();

			SongData = (OK64AudioTable*)(long)&ok_FreeSpace;

			(*(SequenceTable *)MUSTablePointer).Pointer[3].Address = SongData->SeqPointer;
			(*(SequenceTable *)MUSTablePointer).Pointer[3].Length = SongData->SeqSize;

			(*(InstrumentTable *)AlCtlHeader).Pointer[3].Address = SongData->InsPointer;
			(*(InstrumentTable *)AlCtlHeader).Pointer[3].Length = SongData->InsSize;
		}
	}
	else
	{
		DataLength = 8;
		*SourceAddress = (int)&ok_Sequence;
		*TargetAddress = (int)&(*(SequenceTable *)MUSTablePointer).Pointer[3].Address;
		RunRAM();
		*SourceAddress = (int)&ok_Instrument;
		*TargetAddress = (int)&(*(InstrumentTable *)AlCtlHeader).Pointer[3].Address;
		RunRAM();
	}
}

void SetGhostData(void)
{
	if ((HotSwapID > 0) && ((Uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{
		if (GameMode == GAMEMODE_TT)
		{
			u32 *staffGhost = (u32 *)&D_802BFB80.arraySize8[0][2][3];

			if (OverKartHeader.Ghost != 0)
			{
				*SourceAddress = OverKartHeader.Ghost;
				*TargetAddress = (Uint)(&ok_FreeSpace);
				DataLength = 0x3C00;
				RunDMA();
				*SourceAddress = (Uint)(&ok_FreeSpace);
				*TargetAddress = (Uint)staffGhost;
				RunMIO();
				sCourseGhostReplay = staffGhost;
				sCourseGhostReplayIdx = 0;
				sCourseGhostFramesRemaining = *staffGhost & 0x00FF0000;
				RomghostKart = OverKartHeader.GhostCharacter;
				RomghostStatus = 0;
			}
			else
			{
				int ThisPass;

				for (ThisPass = 0; ThisPass < 64; ThisPass++)
				{
					staffGhost[ThisPass] = 0x00FF0000;
				}

				// Should be 9 Minutes of idle.
			}
		}
	}
}
void SetPath(void)
{
	short *PathLengths = (short *)(&g_pathLength);

	if (CourseID == 0x14)
	{
		return;
	}
	if ((HotSwapID > 0) && ((Uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{

		PathLengths[0] = (short)(OverKartHeader.PathLength[0]);
		PathLengths[1] = (short)(OverKartHeader.PathLength[1]);
		PathLengths[2] = (short)(OverKartHeader.PathLength[2]);
		PathLengths[3] = (short)(OverKartHeader.PathLength[3]);

		PathLengths = (short *)(&g_pathBLength);

		PathLengths[0] = (short)(OverKartHeader.PathLength[0]);

		*SourceAddress = OverKartHeader.PathOffset;
		DataLength = 16;
		if (GameMode != GAMEMODE_BATTLE)
		{
			*TargetAddress = (Uint)(&pathOffset);
			RunDMA();
			pathOffsetB = pathOffset + ((OverKartHeader.PathLength[0] + 1) * 8);
		}
		else
		{
			*TargetAddress = (Uint)(&pathOffsetBlock);
			RunDMA();
		}
	}

	GlobalShortA = 1;
	GlobalShortB = 1;

	if (YFLIP != 0)
	{
		GlobalShortB = -1;
	}
		
	{
	    int ThisPath;
	    int pathCourse;

	pathCourse = CourseID;
	if (HotSwapID > 0)
	{
		pathCourse = 0;
	}
	for (ThisPath = 0; ThisPath < 4; ThisPath++)
	{
		
		Marker *PathArray = (Marker *)GetRealAddress(PathTable[pathCourse][ThisPath]);
		Marker *PathArrayB = (Marker *)GetRealAddress(PathTableB[pathCourse][ThisPath]);
		if (ZFLIP)
		{

			

			GlobalIntA = -1;
			{
			    int ThisMark;
			for (ThisMark = 0; ThisMark < 3000; ThisMark++)
			{
				if (PathArray[ThisMark].Position[0] == -32768)
				{
					break;
				}
				else
				{	
					GlobalIntA++;
				}
			}
			}

			{
			    int ThisMark, HighMark;
			for (ThisMark = 0, HighMark = GlobalIntA ; ThisMark < HighMark; ThisMark++, HighMark--)
			{
				ObjectPosition[0] = GlobalShortA * (short)((float)PathArray[ThisMark].Position[0] * LevelScales[(int)ScaleXMode]);				
				ObjectPosition[1] = GlobalShortB * (short)((float)PathArray[ThisMark].Position[1] * LevelScales[(int)ScaleYMode]);				
				ObjectPosition[2] = -1 * (short)((float)PathArray[ThisMark].Position[2] * LevelScales[(int)ScaleZMode]);


				PathArray[ThisMark].Position[0] = GlobalShortA * (short)((float)PathArray[HighMark].Position[0] * LevelScales[(int)ScaleXMode]);
				PathArray[ThisMark].Position[1] = GlobalShortB * (short)((float)PathArray[HighMark].Position[1] * LevelScales[(int)ScaleYMode]);
				PathArray[ThisMark].Position[2] = -1 * (short)((float)PathArray[HighMark].Position[2] * LevelScales[(int)ScaleZMode]);

				GlobalShortC = PathArray[ThisMark].Group;
				PathArray[ThisMark].Group = PathArray[HighMark].Group;


				PathArray[HighMark].Position[0] = ObjectPosition[0];
				PathArray[HighMark].Position[1] = ObjectPosition[1];
				PathArray[HighMark].Position[2] = ObjectPosition[2];
				PathArray[HighMark].Group = GlobalShortC;

			}
			}

			
			if ((HotSwapID > 0) && (GlobalIntA > 0))
			{
				MakeAlternativePath(PathArray,GlobalIntA,ThisPath);
			}

			GlobalIntA = -1;
			{
			    int ThisMark;
			for (ThisMark = 0; ThisMark < 9999; ThisMark++)
			{
				if (PathArrayB[ThisMark].Position[0] == -32768)
				{
					break;
				}
				else
				{	
					GlobalIntA++;
				}
			}
			}
			
			{
			    int ThisMark, HighMark;
			for (ThisMark = 0, HighMark = GlobalIntA ; ThisMark < HighMark; ThisMark++, HighMark--)
			{
				ObjectPosition[0] = GlobalShortA * (short)((float)PathArrayB[ThisMark].Position[0] * LevelScales[(int)ScaleXMode]);
				ObjectPosition[1] = GlobalShortB * (short)((float)PathArrayB[ThisMark].Position[1] * LevelScales[(int)ScaleYMode]);
				ObjectPosition[2] = -1 * (short)((float)PathArrayB[ThisMark].Position[2] * LevelScales[(int)ScaleZMode]);


				PathArrayB[ThisMark].Position[0] = GlobalShortA * (short)((float)PathArrayB[HighMark].Position[0] * LevelScales[(int)ScaleXMode]);
				PathArrayB[ThisMark].Position[1] = GlobalShortB * (short)((float)PathArrayB[HighMark].Position[1] * LevelScales[(int)ScaleYMode]);
				PathArrayB[ThisMark].Position[2] = -1 * (short)((float)PathArrayB[HighMark].Position[2] * LevelScales[(int)ScaleZMode]);

				GlobalShortC = PathArray[ThisMark].Group;
				PathArray[ThisMark].Group = PathArray[HighMark].Group;

				PathArrayB[HighMark].Position[0] = ObjectPosition[0];
				PathArrayB[HighMark].Position[1] = ObjectPosition[1];
				PathArrayB[HighMark].Position[2] = ObjectPosition[2];				
				PathArray[HighMark].Group = GlobalShortC;

			}
			}
		}
		else
		{	

			
			
			GlobalIntA = -1;
			{
			    int ThisMark;
			for (ThisMark = 0; ThisMark < 3000; ThisMark++)
			{
				if (PathArray[ThisMark].Position[0] == -32768)
				{
					break;
				}
				else
				{	
					GlobalIntA++;
				}
			}
			}

			{
			    int ThisMark;
			for (ThisMark = 0; ThisMark < GlobalIntA; ThisMark++)
			{
				if (PathArray[ThisMark].Position[0] == -32768)
				{
					break;
				}
				else
				{
					PathArray[ThisMark].Position[0] = GlobalShortA * (short)((float)PathArray[ThisMark].Position[0] * LevelScales[(int)ScaleXMode]);
					PathArray[ThisMark].Position[1] = GlobalShortB * (short)((float)PathArray[ThisMark].Position[1] * LevelScales[(int)ScaleYMode]);
					PathArray[ThisMark].Position[2] = (short)((float)PathArray[ThisMark].Position[2] * LevelScales[(int)ScaleZMode]);
				}
				

			}
			}

			if ((HotSwapID > 0) && (GlobalIntA > 0))
			{
				MakeAlternativePath(PathArray,GlobalIntA,ThisPath);
			}

			GlobalIntA = -1;
			{
			    int ThisMark;
			for (ThisMark = 0; ThisMark < 3000; ThisMark++)
			{
				if (PathArrayB[ThisMark].Position[0] == -32768)
				{
					break;
				}
				else
				{	
					GlobalIntA++;
				}
			}
			}

			{
			    int ThisMark;
			for (ThisMark = 0; ThisMark < GlobalIntA; ThisMark++)
			{
				if (PathArrayB[ThisMark].Position[0] == -32768)
				{
					break;
				}
				else
				{	
					PathArrayB[ThisMark].Position[0] = GlobalShortA * (short)((float)PathArrayB[ThisMark].Position[0] * LevelScales[(int)ScaleXMode]);
					PathArrayB[ThisMark].Position[1] = GlobalShortB * (short)((float)PathArrayB[ThisMark].Position[1] * LevelScales[(int)ScaleYMode]);
					PathArrayB[ThisMark].Position[2] = (short)((float)PathArrayB[ThisMark].Position[2] * LevelScales[(int)ScaleZMode]);
				}
			}
			}
		}
		
		

	}
	}

	{
	    int ThisPlayer;
	for (ThisPlayer = 0; ThisPlayer < 8; ThisPlayer++)
	{
		CPUPaths[ThisPlayer].CurrentPath = 0;
		CPUPaths[ThisPlayer].LastPath = -1;
		CPUPaths[ThisPlayer].LastLap = 99;
	}
	}
}

void PlaceIBoxes(long BoxOffset)
{
	Marker *BoxArray;

	if ((ItemSetFlag == 0) || (GameMode == GAMEMODE_TT))
	{
		return;
	}
	GlobalAddressA = GetRealAddress(BoxOffset);
	BoxArray = (Marker *)(GlobalAddressA);

	{
	    int vecAxis;
	for (vecAxis = 0; vecAxis < 3; vecAxis++)
	{
		ObjectVelocity[vecAxis] = 0;
	}
	}
	{
	    int ThisBox;
	for (ThisBox = 0; ThisBox < 999; ThisBox++)
	{
		if (BoxArray[ThisBox].Position[0] == -32768)
		{
			return;
		}
		{
		    int vecAxis;
		for (vecAxis = 0; vecAxis < 3; vecAxis++)
		{
			ObjectPosition[vecAxis] = (float)BoxArray[ThisBox].Position[vecAxis];
			ObjectAngle[vecAxis] = MakeRandom();
		}
		}
		ObjectPosition[0] *= MirrorValue;

        ObjectPosition[0] *= LevelScales[(int)(ScaleXMode)];
        ObjectPosition[1] *= LevelScales[(int)(ScaleYMode)];
        ObjectPosition[2] *= LevelScales[(int)(ScaleZMode)];

        if (ZFLIP)
        {
            ObjectPosition[2] *= -1.0f;
        }
        if (YFLIP)
        {
            ObjectPosition[1] *= -1.0f;
        }

		GlobalIntA = AddObjectBuffer(ObjectPosition, ObjectAngle, ObjectVelocity, IBOX);
		SimpleObjectArray[GlobalIntA].Fparam = CheckHight(ObjectPosition[0], ObjectPosition[1] + 10, ObjectPosition[2]);
		SimpleObjectArray[GlobalIntA].Velocity[0] = ObjectPosition[1];
		SimpleObjectArray[GlobalIntA].Position[1] = SimpleObjectArray[GlobalIntA].Fparam - 20;
		SimpleObjectArray[GlobalIntA].Bump.Dummy = BoxArray[ThisBox].Group; // Used for forced custom items
	}
	}
}

void SetWater(void)
{
	if (HotSwapID > 0)
	{
		WaterHeight = OverKartHeader.WaterLevel;
	}
}

void CheckEcho(void)
{
}

void SetEcho(void)
{
	if ((HotSwapID > 0) && ((Uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{
		OverKartRAMHeader.EchoOffset = 0;
		GlobalUIntA = OverKartHeader.EchoEnd - OverKartHeader.EchoStart;
		OverKartRAMHeader.EchoOffset = LoadOKData(OverKartHeader.EchoStart, GlobalUIntA);
		EchoStart = 0;
		EchoStop = 0;
	}
	else
	{
		OverKartRAMHeader.EchoOffset = 0;
		EchoStart = 0x19B;
		EchoStop = 0x1B9;
	}
}
// Rain
Uint A4Rain[36] = {
	0x00000000,0x00000000,
	0x00000000,0x00000000,
	0x00000000,0x00000000,
	0x00000001,0x00000000,
	0x00000002,0x00000000,
	0x00000004,0x00000000,
	0x00000006,0x00000000,
	0x00000008,0x00000000,
	0x00000008,0x00000000,
	0x0000000A,0x00000000,
	0x0000000C,0x00000000,
	0x0000000B,0x00000000,
	0x0000000A,0x00000000,
	0x00000009,0x00000000,
	0x00000008,0x00000000,
	0x00000006,0x00000000,
	0x00000000,0x00000000,
	0x00000000,0x00000000,
};
// Snow
Uint A4Snow[36] = {
	0x00000000,0x00000000,
	0x00000000,0x00000000,
	0x00000111,0x11100000,
	0x00011223,0x32211000,
	0x00112345,0x54321100,
	0x00124578,0x87542100,
	0x0123589B,0xBA853210,
	0x012479CD,0xDC974210,
	0x01358BDF,0xFDB85310,
	0x01358BDF,0xFDB85310,
	0x012479CD,0xDCA74210,
	0x0123589B,0xBA853210,
	0x00124578,0x87542100,
	0x00112345,0x54321100,
	0x00011223,0x32211000,
	0x00000111,0x11100000,
	0x00000000,0x00000000,
	0x00000000,0x00000000,
};

// Rain
Uint A4Dummy[36] = {
	0x00000001,0x10000000,
	0x00000001,0x10000000,
	0x00000001,0x10000000,
	0x00000001,0x10000000,
	0x00000002,0x20000000,
	0x00000004,0x40000000,
	0x00000006,0x60000000,
	0x00000008,0x80000000,
	0x00000008,0x80000000,
	0x0000000A,0xA0000000,
	0x0000000C,0xC0000000,
	0x0000000B,0xB0000000,
	0x0000000A,0xA0000000,
	0x00000009,0x90000000,
	0x00000008,0x80000000,
	0x00000006,0x60000000,
	0x00000002,0x20000000,
	0x00000001,0x10000000,
};

void SetWeatherType(char WeatherType) // 0 = Snow // 1 = Rain
{
	switch (WeatherType)
	{
		case WEATHER_RAIN:
		{
			{
			    int texarr;
			for (texarr = 0; texarr < 36; texarr++)
			{
				*(Uint*)&g_SnowParticleTex[texarr] = A4Dummy[texarr];
			}
			}
			g_skySnowVelocity = 1.75;
			g_3DSnowScale = 0.3;
			g_3DSnowVelocityUpLim = 1.035;
			g_3DSnowVelocityLowLim = -2.5;
			g_3DSnowSwayMovement = 0.01f;
			g_3DSnowSpawnDistanceMin = 30;
			g_3DSnowSpawnDistanceMax = 600;
			g_3DSnowSpawnHeight = 50;
			g_3DSnowSpawnCone = 0x6000;
			
			break;
		}

		case WEATHER_SNOW:
		default:
		{
			{
			    int texarr;
			for (texarr = 0; texarr < 36; texarr++)
			{
				g_SnowParticleTex[texarr] = A4Snow[texarr];
			}
			}
			g_skySnowVelocity = 1.035;
			g_3DSnowScale = 0.15;
			g_3DSnowVelocityUpLim = 1.035;
			g_3DSnowVelocityLowLim = -1.65;
			g_3DSnowSwayMovement = 1.24;
			g_3DSnowSpawnDistanceMin = 75;
			g_3DSnowSpawnDistanceMax = 600;
			g_3DSnowSpawnHeight = 45;
			g_3DSnowSpawnCone = 0x6000;
			break;
		}
	}
	if (HotSwapID == 0)
	{
		{
		    int texarr;
		for (texarr = 0; texarr < 36; texarr++)
		{
			g_SnowParticleTex[texarr] = A4Snow[texarr];
		}
		}
		g_skySnowVelocity = 1.035;
		g_3DSnowScale = 0.15;
		g_3DSnowVelocityUpLim = 1.035;
		g_3DSnowVelocityLowLim = -1.65;
		g_3DSnowSwayMovement = 1.24;
		g_3DSnowSpawnDistanceMin = 30;
		g_3DSnowSpawnDistanceMax = 300;
		g_3DSnowSpawnHeight = 45;
		g_3DSnowSpawnCone = 0x4000;
	}
}

void SetCloudType(char CloudType) // 0 = None // 1 = MR Clouds // 2 = Stars // 3 = Snow/Rain
{

	if (InGame)
	{
		
		switch (CloudType)
		{
			case SKY_CLEAR:
			{
				CloudCourseID = 2;
				break;
			}
			case SKY_STAR:
			{
				CloudCourseID = 14;
				break;
			}
			case SKY_WEATHER:
			{
				CloudCourseID = 5;
				break;
			}
			case SKY_CLOUD:
			default:
			{
				CloudCourseID = 0;
				break;
			}
			}
	}

	if (HotSwapID == 0)
	{
		CloudCourseID = CourseID;
	}
}


void SetWeather3D(bool Weather3DEnable) // Enables 3D weather effects (snow/rain) for Singleplayer
{
	
	
	if (Weather3DEnable && HotSwapID > 0)
	{
		if (StartingIndicator >= 0 && StartingIndicator < 5 && GamePausedFlag == 0x00 && PlayerCount == 1)
		{
			KWChartSnow();
			
		}
		Snow3DCourseID = 5;
	}
	else
	{
		Snow3DCourseID = CourseID;
	}
}

void SnowCustomCheck(int SnowIndex)
{
	if (DynamicObjects[SnowIndex].pos[1] <= GlobalPlayer[0].Position[1] - 5.0 || DynamicObjects[SnowIndex].pos[1] >= GlobalPlayer[0].Position[1] + 60.0)
	{
		DynamicObjects[SnowIndex].Anmptr = 1;
	}
}

void EventDisplay(int Player)
{
	if (HotSwapID == 0)
	{
		KWDisplayEvent(Player);
	}
	else
	{
		KWDisplayIceBlockShadow(Player);
		KWDisplayBombKartBT(Player);
		KWDisplayEvent(Player);
	}
}

void EventDisplayAfter(int Player)
{
	if (HotSwapID == 0)
	{
		KWDisplayEvent_After(Player);
	}
	else
	{
		KWDisplayIceBlock(Player);
		KWDisplayEvent_After(Player);
	}

	if (HotSwapID > 0)
	{
		DrawCustomParticle(Player);
	}
}


void KumoColorMode(Uint r, Uint g, Uint b)
{
    short OKR;
    short OKG;
    short OKB;

    // Get the Colors from the OK Header
    if (HotSwapID == 0)
    {
        SparkMode(255,255,255);
        return;
    }
    OKR = OverKartHeader.FogRGBA[0];
    OKG = OverKartHeader.FogRGBA[1];
    OKB = OverKartHeader.FogRGBA[2];

    SparkMode(OKR,OKG,OKB);
	
}

void SetSky(void)
{

	if (HotSwapID > 0)
	{
		*TargetAddress = (long)&SkyColorTopTable;
		if (GameMode == GAMEMODE_BATTLE)
		{
			*TargetAddress += 0xB4;
		}
		*SourceAddress = OverKartHeader.Sky;
		DataLength = 0x10;
		RunDMA();
		*TargetAddress = (long)&SkyColorBotTable;
		if (GameMode == GAMEMODE_BATTLE)
		{
			*TargetAddress += 0xB4;
		}
		*SourceAddress = *SourceAddress + 0x10;
		RunDMA();
	}
	else
	{
		*TargetAddress = (long)&SkyColorTopTable;
		*SourceAddress = (long)(SEG_RACING_ROM_START + ((uintptr_t)&SkyColorTopTable - SEG_RACING));
		if (GameMode == GAMEMODE_BATTLE)
		{
			*TargetAddress += 0xB4;
			*SourceAddress += 0xB4;
		}		
		
		DataLength = 0x10;
		RunDMA();
		*TargetAddress = (long)&SkyColorBotTable;
		*SourceAddress = (long)(SEG_RACING_ROM_START + ((uintptr_t)&SkyColorBotTable - SEG_RACING));
		if (GameMode == GAMEMODE_BATTLE)
		{
			*TargetAddress += 0xB4;
			*SourceAddress += 0xB4;
		}		
		
		RunDMA();
	}
}

void LoadTextureScrollTranslucent(void)
{
	*(long *)(&ok_scrolltranslucent) = 0xFFFFFFFF;
	if (HotSwapID > 0)
	{
		*SourceAddress = OverKartHeader.ScrollROM;
		if (*SourceAddress != 0)
		{
			*TargetAddress = (long)&ok_scrolltranslucent;
			DataLength = OverKartHeader.ScrollSize;
			DataLength += 4;
			RunDMA();
		}
	}
	{
	    int ThisScroll;
	for (ThisScroll = 0; ThisScroll < 32; ThisScroll++)
	{
		ScrollValues[ThisScroll][0] = 0;
		ScrollValues[ThisScroll][1] = 0;
	}
	}
}

void RunTextureScroll(void)
{
	GlobalAddressA = (long)(&ok_scrolltranslucent);
	LoopValue = *(long *)(&ok_scrolltranslucent);
	if ((Uint)LoopValue == 0xFFFFFFFFu)
	{
		return;
	}
	
	{
	    int CurrentScroll;
	for (CurrentScroll = 0; CurrentScroll < LoopValue; CurrentScroll++)
	{
		GlobalAddressB = *(long *)(GlobalAddressA + (CurrentScroll * 8) + 4); // Address of the Texture command to scroll.
		GlobalIntA = *(long *)(GlobalAddressA + (CurrentScroll * 8) + 8);
		GlobalShortA = (GlobalIntA >> 16) & (0xFFFF); // S Value
		GlobalShortB = GlobalIntA & 0xFFFF;			  // T Value

		if (GlobalShortA > 0)
        {
            KWChaseIVal(&ScrollValues[CurrentScroll][0],4095,GlobalShortA);
            if (ScrollValues[CurrentScroll][0] >= 4095)
            {
                ScrollValues[CurrentScroll][0] = 0;
            }
        }
        if (GlobalShortA < 0)
        {
            KWChaseIVal(&ScrollValues[CurrentScroll][0],0,GlobalShortA);
            if (ScrollValues[CurrentScroll][0] <= 0)
            {
                ScrollValues[CurrentScroll][0] = 4095;
            }
        }       
		
        // Chase T
        if (GlobalShortB > 0)
        {
            KWChaseIVal(&ScrollValues[CurrentScroll][1],4095,GlobalShortB);
            if (ScrollValues[CurrentScroll][1] >= 4095)
            {
                ScrollValues[CurrentScroll][1] = 0;
            }
        }
        if (GlobalShortB < 0)
        {
            KWChaseIVal(&ScrollValues[CurrentScroll][1],0,GlobalShortB);
            if (ScrollValues[CurrentScroll][1] <= 0)
            {
                ScrollValues[CurrentScroll][1] = 4095;
            }
        }    


		// Run stock func
		ScrollMapImage((uintptr_t) GlobalAddressB, ScrollValues[CurrentScroll][0], ScrollValues[CurrentScroll][1]);
	}
	}
}

/*
void RunWaterVertex(void)
{
	//This handles translucency by setting vertex Alpha.
	//Outdated; vertex Alpha can be Loaded directly now.
	GlobalAddressA = (long)(&ok_scrolltranslucent) + OverKartHeader.WVOffset;
	LoopValue = *(long*)GlobalAddressA;
	if ((Uint)LoopValue == 0xFFFFFFFFu)
	{
		return;
	}
	{
	    int CurrentWater;
	for (CurrentWater = 0; CurrentWater < LoopValue; CurrentWater++)
	{
		GlobalAddressB = *(long*)(GlobalAddressA + (CurrentWater * 8) + 4);
		GlobalCharA = (char)*(long*)(GlobalAddressA + (CurrentWater * 8) + 8);
		MakeWaterVertex(GlobalAddressB,GlobalCharA,0,0,0);
	}
	}
}
*/

typedef struct WaveStruct
{
	Uint Count;
	Uint Address;
} WaveStruct;

void RunWaterVertex(void)
{

	GlobalAddressA = (long)(&ok_scrolltranslucent) + OverKartHeader.WVOffset;
	LoopValue = *(long *)GlobalAddressA;
	if ((Uint)LoopValue == 0xFFFFFFFFu)
	{
		return;
	}

	if (WaveTime > 31000)
	{
		WaveDirection = -1;
	}
	if (WaveTime < -31000)
	{
		WaveDirection = 1;
	}
	WaveTime += WaveDirection;

	if ((WaveTime % 2) == 0)
	{

		{
		    int CurrentWater;
		for (CurrentWater = 0; CurrentWater < LoopValue; CurrentWater++)
		{
			WaveStruct *WaveData;
			GlobalAddressB = (GlobalAddressA + (CurrentWater * 8) + 4);
			WaveData = (WaveStruct *)(GlobalAddressB);
			WaveRace((Vtx_t *)GetRealAddress(WaveData->Address | 0x04000000), WaveData->Count, 45.0);
		}
		}
	}
}

typedef struct ROMVertex
{
	short ob[3];
	short tc[2];
	signed char n[4];
} ROMVertex;



void DecodeSP1TriangleOK(Gfx *gfx,unsigned char *pp,char Code )
{
	unsigned int data,v0,v1,v2;
	data=(Uint)pp[PKCounter++];

	if(ScreenFlip != ZFLIP)
	{
	v2=data&0x1f;
	v1=(data>>5)&0x07;
	data=(Uint)pp[PKCounter++];
	v1|=(data&0x03)<<3;
	v0=(data>>2)&0x1f;
	}
	else
	{
	v0=data&0x1f;
	v1=(data>>5)&0x07;
	data=(Uint)pp[PKCounter++];
	v1|=(data&0x03)<<3;
	v2=(data>>2)&0x1f;
	}
	gfx[GFXCounter].words.w0=(G_TRI1<<24);
	gfx[GFXCounter++].words.w1=(((v0*2)<<16)|((v1*2)<<8)|(v2*2));
}
void DecodeSP2TriangleOK(Gfx *gfx,unsigned char *pp,char Code )
{
	unsigned int data,v0,v1,v2,v3,v4,v5;
	data=(Uint)pp[PKCounter++];

	if(ScreenFlip != ZFLIP)
	{
		v2=data&0x1f;
		v1=(data>>5)&0x07;
		data=(Uint)pp[PKCounter++];
		v1|=(data&0x03)<<3;
		v0=(data>>2)&0x1f;
	}
	else
	{
		v0=data&0x1f;
		v1=(data>>5)&0x07;
		data=(Uint)pp[PKCounter++];	
		v1|=(data&0x03)<<3;
		v2=(data>>2)&0x1f;
	}

	data=(Uint)pp[PKCounter++];

	if(ScreenFlip != ZFLIP)
	{
		v5=data&0x1f;
		v4=(data>>5)&0x07;
		data=(Uint)pp[PKCounter++];
		v4|=(data&0x03)<<3;
		v3=(data>>2)&0x1f;
	}
	else
	{
		v3=data&0x1f;
		v4=(data>>5)&0x07;
		data=(Uint)pp[PKCounter++];
		v4|=(data&0x03)<<3;
		v5=(data>>2)&0x1f;
	}
	gfx[GFXCounter].words.w0=((G_TRI2<<24)|((v0*2)<<16)|((v1*2)<<8)|(v2*2));
	gfx[GFXCounter++].words.w1=(((v3*2)<<16)|((v4*2)<<8)|(v5*2));
}
void DecodeSP1QuadrangleOK(Gfx *gfx,unsigned char *pp,char Code )
{
	unsigned int data,v0,v1,v2,v3;
	data=(Uint)pp[PKCounter++];

	if(ScreenFlip != ZFLIP)
	{
		v3=data&0x1f;
		v2=(data>>5)&0x07;
		data=(Uint)pp[PKCounter++];
		v2|=(data&0x03)<<3;
		v1=(data>>2)&0x1f;
		v0=(data>>7)&0x1;
		data=(Uint)pp[PKCounter++];
		v0|=(data&0x0f)<<1;
	}
	else
	{
		v0=data&0x1f;
		v1=(data>>5)&0x07;
		data=(Uint)pp[PKCounter++];
		v1|=(data&0x03)<<3;
		v2=(data>>2)&0x1f;
		v3=(data>>7)&0x1;
		data=(Uint)pp[PKCounter++];
		v3|=(data&0x0f)<<1;

	}
  
	gfx[GFXCounter].words.w0=(G_LINE3D<<24);
	gfx[GFXCounter++].words.w1=((v3*2)<<24)|((v0*2)<<16)|((v1*2)<<8)|(v2*2);
}


void DecodeVertex2OK(char *ramaddress, Uint Number)
{
	char Flag, n0, n1;
	Uint i;
	Vtx *VtxRAMBuffer;
	ROMVertex *VtxROMData = (ROMVertex*) FreeMemoryPointer;
	int Size = (Number * sizeof(Vtx));

	VertexMemoryPointer -= Size;
	VtxRAMBuffer = (Vtx *)(VertexMemoryPointer);
	for (i = 0; i < Number; i++)
	{
		if (ScreenFlip)
		{
			VtxRAMBuffer->n.ob[0] = -1 * (short)((float)VtxROMData->ob[0] * LevelScales[(int)ScaleXMode]);
		}			
		else
		{
			VtxRAMBuffer->n.ob[0] = (short)((float)VtxROMData->ob[0] * LevelScales[(int)ScaleXMode]);
		}

			
		if (YFLIP)
		{
			VtxRAMBuffer->n.ob[1] = -1 * (short)((float)VtxROMData->ob[1] * ScreenStretch * LevelScales[(int)ScaleYMode]);	
		}
		else
		{
			VtxRAMBuffer->n.ob[1] = (short)((float)VtxROMData->ob[1] * ScreenStretch * LevelScales[(int)ScaleYMode]);
		}
		
		
		if(ZFLIP == 1)
		{
			VtxRAMBuffer->n.ob[2] = -1 * (short)((float)VtxROMData->ob[2]  * LevelScales[(int)ScaleZMode]);
		}
		else
		{
			VtxRAMBuffer->n.ob[2] = (short)((float)VtxROMData->ob[2]  * LevelScales[(int)ScaleZMode]);
		}
		

		n0 = VtxROMData->n[0];
		n1 = VtxROMData->n[1];

		Flag = n0 & 0x3;
		Flag |= (n1 << 2) & 0xc;
		
		VtxRAMBuffer->n.tc[0] = VtxROMData->tc[0];
		VtxRAMBuffer->n.tc[1] = VtxROMData->tc[1];
		VtxRAMBuffer->n.n[0] = n0 & 0xfc;
		VtxRAMBuffer->n.n[1] = n1 & 0xfc;
		VtxRAMBuffer->n.n[2] = VtxROMData->n[2];
		VtxRAMBuffer->n.flag = (unsigned short)Flag;

		if (HotSwapID == 0)
		{
			VtxRAMBuffer->n.a = 255;
		}
		else
		{
			VtxRAMBuffer->n.a = VtxROMData->n[3];
		}
		++VtxRAMBuffer;
		++VtxROMData;
	}
}

void RunDisplayScreen(void)
{
	// Handles the display of the Buffer copy to Texture.
	// Allows for rendering the screen to a set of 6 Textures.
	GlobalAddressA = (long)(&ok_scrolltranslucent) + OverKartHeader.ScreenOffset; ////Parse WaterVertex List
	LoopValue = *(long *)GlobalAddressA;
	if (LoopValue != 6)
	{
		return;
	}
	{
	    int CurrentScreen;
	for (CurrentScreen = 0; CurrentScreen < 6; CurrentScreen++)
	{
		GlobalAddressC = *(long *)(GlobalAddressA + (CurrentScreen * 4) + 4);

		GlobalIntA = 88;
		GlobalIntB = 72;

		GlobalShortA = (short)(DispFrame)-1;
		if (GlobalShortA < 0)
			GlobalShortA = 2;
		else if (GlobalShortA > 2)
			GlobalShortA = 0;
		switch (CurrentScreen)
		{
		case 0:
		{
			GetFramebuffer(GlobalIntA, GlobalIntB, 64, 32, (unsigned short *)PhysToK0((int)CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 1:
		{
			GetFramebuffer(GlobalIntA + 64, GlobalIntB, 64, 32, (unsigned short *)PhysToK0((int)CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 2:
		{
			GetFramebuffer(GlobalIntA, GlobalIntB + 32, 64, 32, (unsigned short *)PhysToK0((int)CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 3:
		{
			GetFramebuffer(GlobalIntA + 64, GlobalIntB + 32, 64, 32, (unsigned short *)PhysToK0((int)CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 4:
		{
			GetFramebuffer(GlobalIntA, GlobalIntB + 64, 64, 32, (unsigned short *)PhysToK0((int)CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 5:
		{
			GetFramebuffer(GlobalIntA + 64, GlobalIntB + 64, 64, 32, (unsigned short *)PhysToK0((int)CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		default:
		{
			break;
		}
		}
	}
	}
}

void RunKillDisplayObjects(void)
{
	// disables specific track sections based on game-Mode and CC Speed. 

	GlobalAddressA = ((Uint)(&ok_scrolltranslucent)) + OverKartHeader.KDOffset;
	LoopValue = *(int *)GlobalAddressA;
	GlobalAddressA += 4; // Pass Loop Count

	{
	    int CurrentKD;
	for (CurrentKD = 0; CurrentKD < LoopValue; CurrentKD++)
	{
		KDKill *DisplayKill = (KDKill *)(GlobalAddressA);

		Uint *MeshList = (Uint *)(GlobalAddressA + 12);
		switch (GameMode)
		{
            case GAMEMODE_GP:
            {
                // GP
                if (DisplayKill->GP == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
            case GAMEMODE_TT:
            {
                // TIME TRIAL
                if (DisplayKill->TT == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
            case GAMEMODE_VS:
            {
                // TIME VS
                if (DisplayKill->VS == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
            case GAMEMODE_BATTLE:
            {
                // BATTLE
                if (DisplayKill->Battle == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
		}

		switch (RaceClass)
		{
            case 0:
            {
                // 50
                if (DisplayKill->Fifty == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
            case 1:
            {
                // 100
                if (DisplayKill->Hundred == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
            case 2:
            {
                // 150
                if (DisplayKill->HundredFifty == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
            case 3:
            {
                // EXTRA
                if (DisplayKill->Extra == 0)
                {
                    {
                        Uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList((uintptr_t) (0x07000000 | MeshList[ThisMesh]));
                    }
                    }
                }
                break;
            }
		}

		// Setup Next Loop
		GlobalAddressA = GlobalAddressA + 12 + (DisplayKill->MeshCount * 4);
	}
	}
}

void SetText(void)
{

	*SourceAddress = OverKartHeader.Credits;
	*TargetAddress = (long)&ok_Credits;
	DataLength = 0x20;
	if ((HotSwapID > 0) && (*SourceAddress != 0x00000000))
	{
		RunDMA();
	}
	else
	{
		*(long *)(&ok_Credits) = 0;
	}

	*SourceAddress = OverKartHeader.CourseName;
	*TargetAddress = (long)&ok_CourseName;
	DataLength = 0x40;
	if ((HotSwapID > 0) && (*SourceAddress != 0x00000000))
	{
		RunDMA();
	}
	else
	{
		*(long *)(&ok_CourseName) = 0;
	}

	*SourceAddress = OverKartHeader.SerialKey;
	*TargetAddress = (long)&ok_SerialKey;
	DataLength = 0x40;
	if ((HotSwapID > 0) && (*SourceAddress != 0x00000000))
	{
		RunDMA();
	}
	else
	{
		*(long *)(&ok_SerialKey) = 0;
	}
}
void SetCourseNames(bool custom)
{
	int strtbl;

	if ((custom == true) && (*SourceAddress != 0x00000000))
	{
		for (strtbl = 0; strtbl < 20; strtbl++)
		{
			StringTableCourse[strtbl] = (long)&ok_CourseName + 0x4;
			StringTableCourseGP[strtbl] = (long)&ok_CourseName + 0x4;
		}
	}
	else
	{
		for (strtbl = 0; strtbl < 20; strtbl++)
		{
			StringTableCourse[strtbl] = (long)StockCourseNames[strtbl];
			StringTableCourseGP[strtbl] = (long)StockCourseNames[strtbl];
		}
	}
}

void SetOKObjects(void)
{
	// This loads the Custom OK Objects from the header.

	// Load data from the course header.
	OverKartRAMHeader.ObjectTypeCount = *(int *)(OverKartRAMHeader.ObjectDataStart);
	GlobalAddressC = OverKartRAMHeader.ObjectDataStart + 4;
	OverKartRAMHeader.ObjectTypeList = (OKObjectType *)(GlobalAddressC);

	GlobalAddressB = OverKartRAMHeader.ObjectDataStart + 4 + (OverKartRAMHeader.ObjectTypeCount * sizeof(OKObjectType)); // 32 bytes Size of ObjectType
	OverKartRAMHeader.ObjectCount = *(int *)(GlobalAddressB);
	GlobalAddressD = GlobalAddressB + 4;
	OverKartRAMHeader.ObjectList = (OKObjectList *)(GlobalAddressD);

    
	{
	    int This;
	for (This = 0; This < OverKartRAMHeader.ObjectCount; This++)
	{
		OKObjectType ThisType;

		// Loop through the object Count and set each individual object in the array.

		OKObjectArray[This].ListIndex = This;
		OKObjectArray[This].TypeIndex = OverKartRAMHeader.ObjectList[This].TypeIndex;
		
		

		OKObjectArray[This].SubBehaviorClass = SUBBEHAVIOR_DOCILE;

		ThisType = OverKartRAMHeader.ObjectTypeList[OKObjectArray[This].TypeIndex];

		OKObjectArray[This].ObjectData.Flag = 0xC000;
		OKObjectArray[This].ObjectData.Radius = (ThisType.BumpRadius / 100.0f); // Used for level calcs BUMP
		

		if (ScreenFlip)
		{
			OverKartRAMHeader.ObjectList[This].OriginPosition[0] *= (-1 * LevelScales[(int)ScaleXMode]);			
		}
		else
		{
			OverKartRAMHeader.ObjectList[This].OriginPosition[0] *= (LevelScales[(int)ScaleXMode]);			
		}

		if (YFLIP)
		{
			OverKartRAMHeader.ObjectList[This].OriginPosition[1] *= (-1 * LevelScales[(int)ScaleYMode]);	
		}
		else
		{
			OverKartRAMHeader.ObjectList[This].OriginPosition[1] *= (LevelScales[(int)ScaleYMode]);			
		}

		if (OverKartRAMHeader.ObjectTypeList[OKObjectArray[This].TypeIndex].GravityToggle)
		{
			OverKartRAMHeader.ObjectList[This].OriginPosition[1] += OKObjectArray[This].ObjectData.Radius;
		}


		if (ZFLIP)
		{
			OverKartRAMHeader.ObjectList[This].OriginPosition[2] *= (-1 * LevelScales[(int)ScaleZMode]);			
		}
		else
		{
			OverKartRAMHeader.ObjectList[This].OriginPosition[2] *= (LevelScales[(int)ScaleZMode]);			
		}

        OKObjectArray[This].PathTarget = -1;

		OKObjectArray[This].ObjectData.Position[0] = OverKartRAMHeader.ObjectList[This].OriginPosition[0];
		OKObjectArray[This].ObjectData.Position[1] = OverKartRAMHeader.ObjectList[This].OriginPosition[1];
		OKObjectArray[This].ObjectData.Position[2] = OverKartRAMHeader.ObjectList[This].OriginPosition[2];

		OKObjectArray[This].ObjectData.Angle[0] = OverKartRAMHeader.ObjectList[This].OriginAngle[0] * DEG1;
		OKObjectArray[This].ObjectData.Angle[1] = -1 * OverKartRAMHeader.ObjectList[This].OriginAngle[1] * DEG1;
		OKObjectArray[This].ObjectData.Angle[2] = OverKartRAMHeader.ObjectList[This].OriginAngle[2] * DEG1;

		OKObjectArray[This].ObjectData.Velocity[0] = (float)(OverKartRAMHeader.ObjectList[This].OriginVelocity[0] * 100);
		OKObjectArray[This].ObjectData.Velocity[1] = (float)(OverKartRAMHeader.ObjectList[This].OriginVelocity[1] * 100);
		OKObjectArray[This].ObjectData.Velocity[2] = (float)(OverKartRAMHeader.ObjectList[This].OriginVelocity[2] * 100);

        
		OKObjectArray[This].AngularVelocity[0] = OverKartRAMHeader.ObjectList[This].OriginAngularVelocity[0] * DEG1;
		OKObjectArray[This].AngularVelocity[1] = OverKartRAMHeader.ObjectList[This].OriginAngularVelocity[1] * DEG1;
		OKObjectArray[This].AngularVelocity[2] = OverKartRAMHeader.ObjectList[This].OriginAngularVelocity[2] * DEG1;

		
		if (ThisType.ObjectAnimations != 0xFFFFFFFF)
		{
			Uint *AnimationOffsets = (Uint *)(GetRealAddress(ObjectSegment | ThisType.ObjectAnimations));
			GlobalAddressA = GetRealAddress(ObjectSegment | AnimationOffsets[0]);
			OKObjectArray[This].AnimationMax = (Uchar) * (int *)(GlobalAddressA);
			// OKObjectArray[This].AnimationFrame = MakeRandomLimmit((Ushort)(OKObjectArray[This].AnimationMax));
		}
	}
	}
}

Gfx* IceKageGfx;

void LoadIceKage(void)
{
	*SourceAddress = (long)&IceKageROM;
	*TargetAddress = FreeMemoryPointer;
	DataLength = (long)&IceKageEnd - (long)&IceKageROM;
	RunDMA();
	IceKageGfx = (Gfx *) FreeMemoryPointer;
	SetSegment(8, (void *) FreeMemoryPointer);
	FreeMemoryPointer += DataLength;
	CheckCourseMemoryHighWater();
}

void KT16ItemBox(void)
{
	if (HotSwapID == 0)
	{
		PlaceIBoxes(0x06000038);
	}
}

void InitialMapObjectCode(void)
{
	if (HotSwapID == 0)
	{
        InitialMapObject();
	}
	else
	{
		StaticObjectCount = 0;
		SimpleObjectCount = 0;
		if (OverKartHeader.Version <= 6)
		{
			PlaceIBoxes(0x06001910);
			SetTreeObject(0x06001B18);
			SetPakkunObject(0x06001D20);
		}
		else
		{
			PlaceIBoxes(0x06000008);
			SetTreeObject(0x06000210);
			SetPakkunObject(0x06000418);
		}
		StaticObjectCount = SimpleObjectCount;
	}
	{
	    int ThisObject;
	for (ThisObject = 0; ThisObject < StaticObjectCount; ThisObject++)
	{

        if (SimpleObjectArray[ThisObject].Category == IBOX)
        {
            continue;
        }


        SimpleObjectArray[ThisObject].Position[0] = ((float)SimpleObjectArray[ThisObject].Position[0] * LevelScales[(int)ScaleXMode]);
        if (YFLIP)
        {
            SimpleObjectArray[ThisObject].Position[1] = -1 * ((float)SimpleObjectArray[ThisObject].Position[1] * LevelScales[(int)ScaleYMode]);
        }
        else
        {
            SimpleObjectArray[ThisObject].Position[1] = ((float)SimpleObjectArray[ThisObject].Position[1] * LevelScales[(int)ScaleYMode]);
        }
        if (ZFLIP)
        {
            SimpleObjectArray[ThisObject].Position[2] = -1 * ((float)SimpleObjectArray[ThisObject].Position[2] * LevelScales[(int)ScaleZMode]);
        }
        else
        {
            SimpleObjectArray[ThisObject].Position[2] = ((float)SimpleObjectArray[ThisObject].Position[2] * LevelScales[(int)ScaleZMode]);
        }
        
        

		

		
	}
	}
}
void InitialMapCode(void)
{
	InitialMap();	
	SetPath();
}

void LoadOKObjects(void)
{
	// Load all the OKObject data from the Course Header.
	GlobalUIntA = OverKartHeader.ObjectDataEnd - OverKartHeader.ObjectModelStart;
	SetSegment(0xA, (void *) LoadOKData(OverKartHeader.ObjectModelStart, GlobalUIntA));
	GlobalUIntA = OverKartHeader.ObjectModelStart - OverKartHeader.ObjectDataStart;
	GlobalAddressA = LoadOKData(OverKartHeader.ObjectDataStart, GlobalUIntA);
	OverKartRAMHeader.ObjectDataStart = GlobalAddressA;
}

void LoadHeaderOffsets(void)
{
	// Load the offsets to the custom course headers.
	*TargetAddress = (long)&ok_HeaderOffsets;
	*SourceAddress = (long)&ok_HeaderROM;
	DataLength = 0x3C00;
	RunDMA();
}

void LoadBomb(void)
{
	if (HotSwapID > 0)
	{
		*SourceAddress = OverKartHeader.BombOffset;
	}
	else
	{
		*SourceAddress = (long)&ok_Bomb;
	}
	*TargetAddress = (int)&BombTable;
	DataLength = 0xA8;
	RunDMA();
}

#define COURSE_TABLE_ROW_SIZE 0x30
#define COURSE_TABLE_BATTLE_OFFSET 0x2D0
#define OK_HEADER_OLD_SIZE 0xC0

/* Amped Up / Version <= 5 packed layout. File-local; do not replace live OKHeader (0xD0). */
typedef struct OKHeaderOld {
	int Version;
	CourseHeader MapHeader;
	Uint SectionViewPosition;
	Uint XLUSectionViewPosition;
	Uint SurfaceMapPosition;
	Uint Sky;
	char SkyType, WeatherType, PathSplit, PathCount;
	Uint Credits;
	Uint CourseName;
	Uint SerialKey;
	Uint Ghost;
	Uint Maps;
	Uint ObjectDataStart;
	Uint ObjectModelStart;
	Uint ObjectAnimationStart;
	Uint ObjectDataEnd;
	Uint BombOffset;
	Uint EchoStart;
	Uint EchoEnd;
	char GoalBannerToggle;
	char BackgroundToggle;
	char ManualTempo;
	char Padding;
	char PathTrigger[4];
	Uint MusicID;
	short PathLength[4];
	char GhostCharacter, WaterType;
	short WaterLevel;
	Uint ScrollROM;
	short WVOffset, ScreenOffset, KDOffset, ScrollSize;
	Uint PathOffset;
	short FogStart, FogStop;
	Uchar FogRGBA[4];
	Uchar Tail[0x18];
} OKHeaderOld;

static void CopyOKHeaderOld(OKHeaderOld *oldHeader)
{
	int padIndex;

	OverKartHeader.Version = oldHeader->Version;
	OverKartHeader.MapHeader = oldHeader->MapHeader;
	OverKartHeader.SectionViewPosition = oldHeader->SectionViewPosition;
	OverKartHeader.XLUSectionViewPosition = oldHeader->XLUSectionViewPosition;
	OverKartHeader.SurfaceMapPosition = oldHeader->SurfaceMapPosition;
	OverKartHeader.Sky = oldHeader->Sky;
	OverKartHeader.SkyType = oldHeader->SkyType;
	OverKartHeader.WeatherType = oldHeader->WeatherType;
	OverKartHeader.Credits = oldHeader->Credits;
	OverKartHeader.CourseName = oldHeader->CourseName;
	OverKartHeader.SerialKey = oldHeader->SerialKey;
	OverKartHeader.Ghost = oldHeader->Ghost;
	OverKartHeader.Maps = oldHeader->Maps;
	OverKartHeader.ObjectDataStart = oldHeader->ObjectDataStart;
	OverKartHeader.ObjectModelStart = oldHeader->ObjectModelStart;
	OverKartHeader.ObjectAnimationStart = oldHeader->ObjectAnimationStart;
	OverKartHeader.ObjectDataEnd = oldHeader->ObjectDataEnd;
	OverKartHeader.BombOffset = oldHeader->BombOffset;
	OverKartHeader.EchoStart = oldHeader->EchoStart;
	OverKartHeader.EchoEnd = oldHeader->EchoEnd;
	OverKartHeader.GoalBannerToggle = oldHeader->GoalBannerToggle;
	OverKartHeader.BackgroundToggle = oldHeader->BackgroundToggle;
	OverKartHeader.ManualTempo = oldHeader->ManualTempo;
	OverKartHeader.MusicID = oldHeader->MusicID;
	OverKartHeader.PathLength[0] = oldHeader->PathLength[0];
	OverKartHeader.GhostCharacter = oldHeader->GhostCharacter;
	OverKartHeader.WaterType = oldHeader->WaterType;
	OverKartHeader.WaterLevel = oldHeader->WaterLevel;
	OverKartHeader.ScrollROM = oldHeader->ScrollROM;
	OverKartHeader.WVOffset = oldHeader->WVOffset;
	OverKartHeader.ScreenOffset = oldHeader->ScreenOffset;
	OverKartHeader.KDOffset = oldHeader->KDOffset;
	OverKartHeader.ScrollSize = oldHeader->ScrollSize;
	OverKartHeader.PathOffset = oldHeader->PathOffset;
	OverKartHeader.FogStart = oldHeader->FogStart;
	OverKartHeader.FogStop = oldHeader->FogStop;
	OverKartHeader.FogRGBA[0] = oldHeader->FogRGBA[0];
	OverKartHeader.FogRGBA[1] = oldHeader->FogRGBA[1];
	OverKartHeader.FogRGBA[2] = oldHeader->FogRGBA[2];
	OverKartHeader.FogRGBA[3] = oldHeader->FogRGBA[3];

	/* PathTrigger[4] is ignored. Live struct has LapCount in this slot. */
	OverKartHeader.LapCount = 3;
	OverKartHeader.PathSplit = 0;
	OverKartHeader.PathCount = oldHeader->PathCount;
	if (OverKartHeader.PathCount < 1)
	{
		OverKartHeader.PathCount = 1;
	}
	if (OverKartHeader.PathCount > 4)
	{
		OverKartHeader.PathCount = 4;
	}
	if (OverKartHeader.PathCount == 1)
	{
		OverKartHeader.PathLength[1] = 0;
		OverKartHeader.PathLength[2] = 0;
		OverKartHeader.PathLength[3] = 0;
	}
	else
	{
		OverKartHeader.PathLength[1] = oldHeader->PathLength[1];
		OverKartHeader.PathLength[2] = oldHeader->PathLength[2];
		OverKartHeader.PathLength[3] = oldHeader->PathLength[3];
	}

	for (padIndex = 0; padIndex < 8; padIndex++)
	{
		OverKartHeader.Padding[padIndex] = 0;
	}
}

static void ClearStockCourseHeader(void)
{
	unsigned char *p;
	int i;

	p = (unsigned char *)&OverKartHeader;
	for (i = 0; i < (int)sizeof(OKHeader); i++)
	{
		p[i] = 0;
	}
	OverKartHeader.Version = 0xFFFFFFFF;
	VersionNumber = 0;
}

void LoadCustomHeader(int inputID)
{
	if ((HotSwapID > 0) && (inputID != -1))
	{
		int version;
		long headerROM;

		headerROM = *(long *)(&ok_HeaderOffsets + ((inputID)*1) + ((HotSwapID - 1) * 0x14));
		if ((Uint)headerROM != 0xFFFFFFFFu)
		{
			*SourceAddress = headerROM;
			*TargetAddress = (long)&version;
			DataLength = 4;
			RunDMA();

			if (version <= 5)
			{
				OKHeaderOld oldHeader;

				*SourceAddress = headerROM;
				*TargetAddress = (long)&oldHeader;
				DataLength = OK_HEADER_OLD_SIZE;
				RunDMA();
				CopyOKHeaderOld(&oldHeader);
			}
			else
			{
				*SourceAddress = headerROM;
				*TargetAddress = (long)&ok_CourseHeader;
				DataLength = sizeof(OKHeader);
				RunDMA();
			}

			VersionNumber = OverKartHeader.Version;

			*TargetAddress = (long)&g_courseTable;

			if (GameMode == GAMEMODE_BATTLE)
			{
				*TargetAddress += COURSE_TABLE_BATTLE_OFFSET;
			}

			*SourceAddress = (long)(&OverKartHeader.MapHeader);
			DataLength = COURSE_TABLE_ROW_SIZE;
			RunRAM();
		}
		else
		{
			ClearStockCourseHeader();
		}
	}
	else
	{
		ClearStockCourseHeader();
		*SourceAddress = (long)(SEG_RACING_ROM_START + ((uintptr_t)&g_courseTable - SEG_RACING));
		*TargetAddress = (long)&g_courseTable;
		DataLength = COURSE_TABLE_ROW_SIZE;

		if (GameMode == GAMEMODE_BATTLE)
		{
			*TargetAddress += COURSE_TABLE_BATTLE_OFFSET;
			*SourceAddress += COURSE_TABLE_BATTLE_OFFSET;
		}

		RunDMA();
	}
}

void SetCustomData(void)
{
	SetText();
	SetEcho();
	SetSong();
	LoadBomb();

	if (((Uint)OverKartHeader.Version != 0xFFFFFFFFu) && (HotSwapID > 0))
	{
		SetCourseNames(true);
	}
	else
	{
		SetCourseNames(false);
	}
}

/* Original: dest = g_CourseBannerOffsets + slot * 5056 (140x18 + pad). */
#define BANNER_STRIDE 5056

static u16 *BannerPixels(int CourseIndex)
{
	MenuTexture *ram;
	int i;

	if ((TexBuff == NULL) || (CourseIndex < 0) || (CourseIndex >= 20))
	{
		return NULL;
	}
	ram = (MenuTexture *) GetTBPointer(D_800E7DC4[CourseIndex]);
	if ((ram == NULL) || (ram->TextureData == NULL))
	{
		return NULL;
	}
	for (i = 0; i < AllTexBlockNum; i++)
	{
		if (SMenuTextureMap[i].TextureData == ram->TextureData)
		{
			return &TexBuff[SMenuTextureMap[i].Offset];
		}
	}
	return NULL;
}

/* Live g_CourseBannerOffsets / g_BattleBannerOffsets: first bank, then +5056. */
static u16 *BannerBank(int slot)
{
	u16 *base;

	if (GameMode == GAMEMODE_BATTLE)
	{
		base = BannerPixels(16);
	}
	else
	{
		base = BannerPixels(8);
	}
	if (base == NULL)
	{
		return NULL;
	}
	return (u16 *) ((char *) base + (slot * BANNER_STRIDE));
}

static void LoadCustomBanner(int slot)
{
	u16 *dest;
	long Length;
	int customIndex;

	dest = BannerBank(slot);
	if (dest == NULL)
	{
		return;
	}

	if (GameMode == GAMEMODE_BATTLE)
	{
		customIndex = 16 + slot;
	}
	else
	{
		customIndex = slot;
	}

	GlobalAddressA = ((long)(&ok_MenuOffsets) + (customIndex * 8) + ((HotSwapID - 1) * 160));
	*SourceAddress = *(long *)(GlobalAddressA);
	if ((*SourceAddress != 0x00000000) && ((Uint)*SourceAddress != 0xFFFFFFFFu))
	{
		GlobalAddressB = (GlobalAddressA + 4);
		Length = (*(long *)(GlobalAddressB) - *SourceAddress) + 16;
		if ((Length < 16) || (Length > 0x8000))
		{
			*SourceAddress = (long)&bannerU;
		}
		else
		{
			*TargetAddress = (long)&ok_FreeSpace;
			DataLength = Length;
			RunDMA();
			*SourceAddress = (long)&ok_FreeSpace;
		}
	}
	if (*SourceAddress == 0x00000000)
	{
		*SourceAddress = (long)&bannerU;
	}
	if ((Uint)*SourceAddress == 0xFFFFFFFFu)
	{
		*SourceAddress = (long)&bannerN;
	}
	*TargetAddress = (long) dest;
	RunMIO();
}

static void RestoreStockBanner(int slot)
{
	MenuTexture *ram;
	u16 *dest;
	int CourseIndex;

	dest = BannerBank(slot);
	if (dest == NULL)
	{
		return;
	}
	if (GameMode == GAMEMODE_BATTLE)
	{
		CourseIndex = g_CupArray[16 + slot];
	}
	else
	{
		CourseIndex = g_CupArray[slot];
	}
	if ((CourseIndex < 0) || (CourseIndex >= 20))
	{
		return;
	}
	ram = (MenuTexture *) GetTBPointer(D_800E7DC4[CourseIndex]);
	if ((ram == NULL) || (ram->TextureData == NULL))
	{
		return;
	}
	DataLength = 0x1000;
	*SourceAddress = (long)(_textures_0bSegmentRomStart + SEGMENT_OFFSET(ram->TextureData));
	*TargetAddress = (long)&ok_FreeSpace;
	RunDMA();
	*SourceAddress = (long)&ok_FreeSpace;
	*TargetAddress = (long) dest;
	RunTKM();
}

void SetBanners(void)
{
	int slot;
	int slotCount;

	if (MapIsCurrentlyLoading)
	{
		return;
	}

	if (GameMode == GAMEMODE_BATTLE)
	{
		slotCount = 4;
	}
	else
	{
		slotCount = 16;
	}

	if (HotSwapID > 0)
	{
		sCustomMenuLive = 1;
		for (slot = 0; slot < slotCount; slot++)
		{
			LoadCustomBanner(slot);
		}
	}
	else
	{
		if (sCustomMenuLive == 0)
		{
			return;
		}
		sCustomMenuLive = 0;
		SetCourseNames(false);
		for (slot = 0; slot < slotCount; slot++)
		{
			RestoreStockBanner(slot);
		}
	}
}

void SetPreviews(void)
{
	MenuTexture *ram;
	int currentCourse;
	long rom;

	if (MapIsCurrentlyLoading)
	{
		return;
	}

	if (HotSwapID > 0)
	{
		for (currentCourse = 0; currentCourse < 20; currentCourse++)
		{
			GlobalAddressA = (long)(&ok_MenuOffsets) + 4 + (currentCourse * 8) + ((HotSwapID - 1) * 160);
			*SourceAddress = *(long *)(GlobalAddressA);
			if (*SourceAddress == 0x00000000)
			{
				*SourceAddress = (long)&previewU;
			}
			if ((Uint)*SourceAddress == 0xFFFFFFFFu)
			{
				*SourceAddress = (long)&previewN;
			}
			rom = *SourceAddress - (long)_textures_0aSegmentRomStart;
			rom |= 0x0A000000;
			ram = (MenuTexture *) GetTBPointer(D_800E7D74[currentCourse]);
			if (ram == NULL)
			{
				continue;
			}
			ram->TextureData = (u64 *) rom;
			ram->Size = 0x4000;
		}
		sCustomMenuLive = 1;
	}
	else
	{
		if (sCustomMenuLive == 0)
		{
			return;
		}
		/* r_CoursePreviewOffsets → cart copy of the packed MenuTexture[2] Table. */
		*SourceAddress = (long)(_data_segment2SegmentRomStart +
				SEGMENT_OFFSET(Seg2MarioRacewayPreviewTexture));
		*TargetAddress = (long) GetTBPointer(Seg2MarioRacewayPreviewTexture);
		DataLength = 0x320;
		RunDMA();
	}
}

void SwapHS(int Direction)
{
	if (MapIsCurrentlyLoading)
	{
		return;
	}
	// This function will swap to a new set of custom levels.
	if (Direction == 0)
	{
		if (HotSwapID > 0)
		{
			if (HotSwapID == 1)
			{
				// StockASM();
				CopyCourseTable(0);
			}
			HotSwapID--;

			PlaySound(0x4900801A);
		}
	}
	else
	{
		if (HotSwapID < 4)
		{
			if (HotSwapID == 0)
			{
				// OverkartASM();
				CopyCourseTable(1);
				{
				    int Index;
				for (Index = 0; Index < 20; Index++)
				{
					g_CupArray[Index] = Index;
				}
				}
			}
			HotSwapID++;

			PlaySound(0x4900801A);
		}
	}

	SetPreviews();
	PreviewRefresh();
	SetBanners();
	CourseValue = -1;
}



typedef struct MiniMapStruct{
	short	MapX, MapY;
	short	StartX, StartY;
	short	LineX, LineY;
	short	Height, Width;
	short	R,G,B,A;
	float	Scale;
	
} MiniMapStruct;
void LoadMinimap(void)
{
	MiniMapStruct *MiniMapData;

	if (HotSwapID == 0)
	{
		return;
	}

	*SourceAddress = OverKartHeader.Maps;

	if (*SourceAddress != 0x00000000)
	{
		*TargetAddress = (long)&ok_FreeSpace;
		DataLength = 0x1000;
		RunDMA();
		MiniMapData = (MiniMapStruct*)(long)&ok_FreeSpace;

		if (PlayerCount == 1)
		{	
			MapX[0] = MiniMapData->MapX;
			MapY[0] = MiniMapData->MapY;
		}
		if (ScreenFlip == 1)
		{
			StartX = MapWidth - StartX;
		}
		RadarLineX = MiniMapData->LineX;
		RadarLineY = MiniMapData->LineY;
		StartX = MiniMapData->StartX;
		StartY = MiniMapData->StartY;				
		MapHeight = MiniMapData->Height;
		MapWidth = MiniMapData->Width;
		MapR = MiniMapData->R;
		MapG = MiniMapData->G;
		MapB = MiniMapData->B;
		MapScale = MiniMapData->Scale * 0.01;
		*SourceAddress = (long)(&ok_FreeSpace) + sizeof(MiniMapStruct);
		*TargetAddress = (long)&ok_MapTextureData;
		RunMIO();
		MapTexture = (long)&ok_MapTextureData;
	}
}

int TempFireParticleMax = 8;

void InitFireParticles(long PathOffset)
{
	*TempPointer = PathOffset;
	*SourceAddress = (long)&FireParticlePositions;
	*TargetAddress = (long)&ObjectPosition;
	{
	    int currentFire;
	for (currentFire = 0; currentFire < TempFireParticleMax; currentFire++)
	{

		FireParticlePositions[currentFire][0] = *(short *)(PathOffset);
		FireParticlePositions[currentFire][1] = *(short *)(PathOffset + 2);
		FireParticlePositions[currentFire][2] = *(short *)(PathOffset + 4);

		if (FireParticlePositions[currentFire][0] == (short)-32768)
		{
			if (currentFire < TempFireParticleMax - 1)
			{
				return;
			}
			else
			{
				break;
			}
		}
		PathOffset = PathOffset + 8;
	}
	}
	{
	    int currentFire;
	for (currentFire = 0; currentFire < TempFireParticleMax; currentFire++)
	{
		KWGetCaveFire(currentFire);

		DynamicObjects[FireParticleAllocArray[currentFire]].PosStart[0] = (float)FireParticlePositions[currentFire][0];
		DynamicObjects[FireParticleAllocArray[currentFire]].PosStart[1] = (float)FireParticlePositions[currentFire][1] + 10;
		DynamicObjects[FireParticleAllocArray[currentFire]].PosStart[2] = (float)FireParticlePositions[currentFire][2];

		DynamicObjects[FireParticleAllocArray[currentFire]].Scale = 0.8;
	}
	}
}

char FireRGBA[8][4] = {{150, 0, 25, 255}, {150, 20, 55, 255}, {50, 50, 255, 255}, {50, 255, 50, 255}, {150, 90, 255, 255}, {250, 55, 55, 155}, {100, 255, 25, 255}, {250, 105, 55, 255}};
// 8 = TempFireParticleMax

void EmptyActionData(void)
{
	GlobalAddressA = (Uint)CoursesCPUBehaviour[19];
	CoursesCPUBehaviour[0] = CoursesCPUBehaviour[19];

	*(Ushort *)(GlobalAddressA) = 1;
	*(Ushort *)(GlobalAddressA + (0x2)) = (Ushort)MaxPathPoints[0];
	*(Ushort *)(GlobalAddressA + (0x6)) = 6;
}

void SearchListHook(Uint Addr)
{
	if (HotSwapID > 0)
	{
		return;
	}
	
	SearchList((Gfx *) Addr);
}

void SearchListFileHook(Uint Addr)
{

    if (HotSwapID > 0)
    {
        SearchListFile(0x06000000 | OverKartHeader.SurfaceMapPosition);
        return;
    }
	SearchListFile(Addr);   
		
    
}

void SearchList2Hook(Uint Addr, char Surface)
{
	if (HotSwapID > 0)
    {
        SearchListFile(0x06000000 | OverKartHeader.SurfaceMapPosition);
        return;
    }
	SearchList2((Gfx *) Addr, Surface);
}

void DisplayKT1Hook(Screen *Display)
{
	ClockCycle[1] = osGetCount();
	CycleCount[1] = (ClockCycle[1] - OldCycle[1]);
	OldCycle[1] = ClockCycle[1];
	if (HotSwapID > 0)
	{
		if (OverKartHeader.FogStart > 0)
		{
			g_fogToggleBanshee = 1;
			g_fogR = (Uint)OverKartHeader.FogRGBA[0];
			g_fogG = (Uint)OverKartHeader.FogRGBA[1];
			g_fogB = (Uint)OverKartHeader.FogRGBA[2];
			gDPSetFogColor(GraphPtrOffset++, (Uint)OverKartHeader.FogRGBA[0], (Uint)OverKartHeader.FogRGBA[1], (Uint)OverKartHeader.FogRGBA[2], 255);
			gSPFogPosition(GraphPtrOffset++, OverKartHeader.FogStart, OverKartHeader.FogStop);
		}
		if (OverKartHeader.Version <= 6)
		{
			gSPClearGeometryMode(GraphPtrOffset++,G_SHADE | G_LIGHTING | G_CULL_BOTH);
			gSPTexture(GraphPtrOffset++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
			gSPSetGeometryMode(GraphPtrOffset++, G_SHADING_SMOOTH | G_ZBUFFER | G_CULL_BACK);
			gDPSetCombineMode(GraphPtrOffset++, G_CC_MODULATERGBA,G_CC_MODULATERGBA);
			gDPSetRenderMode(GraphPtrOffset++, G_RM_AA_ZB_OPA_SURF,G_RM_AA_ZB_OPA_SURF2);
		}

		DisplayGroupmap(SegmentAddress(6, OverKartHeader.SectionViewPosition), Display);
		gSPClearGeometryMode(GraphPtrOffset++, G_FOG );
		gDPSetCycleType(GraphPtrOffset++, G_CYC_1CYCLE);
        gSPSetGeometryMode(GraphPtrOffset++, G_SHADE);
	}
	else
	{
		DisplayKT1(Display);
	}
	
}

void XLUDisplay(Screen *Display)
{
	
	

	if ((OverKartHeader.Version > 4) && (HotSwapID > 0))
	{
		if ((OverKartHeader.XLUSectionViewPosition == 0) || (OverKartHeader.XLUSectionViewPosition == 0xFFFFFFFF))
		{
			return;
		}
		if (GameMode != GAMEMODE_BATTLE)
		{
			if (OverKartHeader.FogStart > 0)
			{
				g_fogToggleBanshee = 1;
				g_fogR = (Uint)OverKartHeader.FogRGBA[0];
				g_fogG = (Uint)OverKartHeader.FogRGBA[1];
				g_fogB = (Uint)OverKartHeader.FogRGBA[2];
				gDPSetFogColor(GraphPtrOffset++, (Uint)OverKartHeader.FogRGBA[0], (Uint)OverKartHeader.FogRGBA[1], (Uint)OverKartHeader.FogRGBA[2], 255);
			    gSPFogPosition(GraphPtrOffset++, OverKartHeader.FogStart, OverKartHeader.FogStop);
				gDPPipeSync(GraphPtrOffset++);
			}
            
            DisplayGroupmap(SegmentAddress(6, OverKartHeader.XLUSectionViewPosition), Display);
            gSPClearGeometryMode(GraphPtrOffset++, G_FOG );
            gDPSetCycleType(GraphPtrOffset++, G_CYC_1CYCLE);
            
		}
		else
		{
			gSPDisplayList(GraphPtrOffset++, (0x06000000 | OverKartHeader.XLUSectionViewPosition));	
		}
	}
	
}

void DisplayKT16Hook(Screen *Display)
{
	ClockCycle[1] = osGetCount();
	CycleCount[1] = (ClockCycle[1] - OldCycle[1]);
	OldCycle[1] = ClockCycle[1];

	if (HotSwapID > 0)
	{
		gSPTexture(GraphPtrOffset++, 0xFFFF,0xFFFF, 0, G_TX_RENDERTILE, G_ON);
		gSPSetGeometryMode(GraphPtrOffset++,G_SHADING_SMOOTH);
		gSPClearGeometryMode(GraphPtrOffset++,G_LIGHTING);
		gSPDisplayList(GraphPtrOffset++, (0x06000000 | OverKartHeader.SectionViewPosition));	
        
		
	}
	else
	{
		DisplayKT16(Display);
	}
}


short RadarLineX, RadarLineY;
void RadarFinishLine(Uint ulx,Uint uly,Ushort *Addr)
{
	KWSprite8x8(ulx + RadarLineX, uly + RadarLineY, Addr);	
}

float HijackScaleMinimapX(float x)
{
	if (ScaleXMode != 2)
	{
		x = x / LevelScales[(int) ScaleXMode];
	}
	return x;
}

float HijackScaleMinimapY(float z)
{
	if (ScaleZMode != 2)
	{
		z = z / LevelScales[(int) ScaleZMode];
	}
	return z;
}

short AddObjectBufferMKANBANHook(Vector pos, SVector angle, Vector velo, short category)
{
	if (HotSwapID != 0) {
		return -1;
	}

	return AddObjectBuffer(pos, angle, velo, category);
}
