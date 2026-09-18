#include "../MainInclude.h"
#include <segments.h>
#include "textures.h"

extern u16 *tex_buff;
extern s32 all_tex_block_num;
extern MenuTexture *D_800E7D74[];
extern MenuTexture *D_800E7DC4[];
extern void *GetTBPointer(const void *addr);
extern u8 _textures_0aSegmentRomStart[];
extern u8 _textures_0bSegmentRomStart[];

typedef struct {
	u64 *textureData;
	s32 offset;
} OKTexMap;

extern OKTexMap sMenuTextureMap[];

static int sStockAudioSaved = 0;
static int sCourseTableSaved = 0;
static int sCustomMenuLive = 0;



unsigned char ScaleXMode = 2, ScaleYMode = 2, ScaleZMode = 2, ScalePad = 0;
float LevelScales[7] =
{
	0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f
};
short ZFLIP, YFLIP;

void FPS_Check(void)
{
	static uint ClockCycle, OldCycle;
	static float CycleCount;
	static float FPS;
	short FPS_Check;

	ClockCycle = osGetCount();
	CycleCount = (ClockCycle - OldCycle);
	/* CPU2SEC */
	FPS = (1 * 0x2CB5E16) / CycleCount;

	FPS_Check = (short)FPS;
	OldCycle = ClockCycle;

	/* Modifiers for FPS drops */
	/* Those values work well already, but can be tweaked further */
	if (FPS_Check >= 22)
	{
		DynFPSModifier = 2;
	}

	else if (FPS_Check >= 15)
	{
		DynFPSModifier = 3;
	}

	else if (FPS_Check >= 11)
	{
		DynFPSModifier = 4;
	}

	else if (FPS_Check >= 8)
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
	FPS_Check();
}

void StaticTempo(int Tempo)
{
	DynFPSModifier = (short) Tempo;
}

void ApplyRaceTempo(void)
{
	if (ok_StaticTempoOnly != 0) {
		DynFPSModifier = 2;
	}
	framerate = DynFPSModifier;
}

void previewRefresh(void)
{
	if (MapIsCurrentlyLoading)
	{
		return;
	}
	// This value will cause the game to try to reload preview images
	// Used on the Map Select screen when we swap to a new set of custom levels.
	if (g_gameMode != GAMEMODE_BATTLE)
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

void copyCourseTable(int copyMode)
{
	// 0 to restore stock cups, 1 to save. First restore snapshots
	// the live table so map-select enter never copies empty BSS.
	dataLength = 0x28;
	if (copyMode == 0)
	{
		if (sCourseTableSaved == 0)
		{
			*sourceAddress = (long)(&g_CupArray[0]);
			*targetAddress = (long)&ok_CourseTable;
			runRAM();
			sCourseTableSaved = 1;
			return;
		}
		*sourceAddress = (long)&ok_CourseTable;
		*targetAddress = (long)(&g_CupArray[0]);
	}
	else
	{
		*sourceAddress = (long)(&g_CupArray[0]);
		*targetAddress = (long)&ok_CourseTable;
		sCourseTableSaved = 1;
	}
	runRAM();
}

void hsTableSet(void)
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

void stockASM(void)
{
	// these functions will swap the ASM routines that load Mario's Raceway.
	// they will point to the custom setup we've implemented for HotSwap courses.
	// this does not swap in the course header table, only the ASM instructions.

	highpolypipeA = 0x3C190700; // 3C190700
	highpolypipeB = 0x373908E8; // 373908E8

	lowpolypipeA = 0x3C0A0700; // 3C0A0700
	lowpolypipeB = 0x354A2D68; // 354A2D68

	uraGrpA = 0x3C0E0700;
	uraGrpB = 0x35CE3050;

	bigmushroomA = 0x3C180700; // 3C180700
	bigmushroomB = 0x37181140; // 37181140

	bigmushroomsurfaceA = 0x3C040700; // 3C040700
	bigmushroomsurfaceB = 0x34841140; // 34841140

	audienceA = 0x3C0F0700; // 3C0F0700
	audienceB = 0x35EF14A0; // 35EF14A0

	audienceflagsA = 0x3C0D0700; // 3C0D0700
	audienceflagsB = 0x35AD0160; // 35AD0160

	billboard1A = 0x3C0C0700; // 3C0C0700
	billboard1B = 0x358C3240; // 358C3240

	billboard2A = 0x3C0F0700; // 3C0E0700
	billboard2B = 0x35EF3508; // 35CE0450

	billboardpost1A = 0x3C190700; // 3C190700
	billboardpost1B = 0x37390240; // 37390240

	billboardpost2A = 0x3C0E0700; // 3C190700
	billboardpost2B = 0x35CE0450; // 37390240

	fencesA = 0x3C180700; // 3C180700
	fencesB = 0x371800E0; // 371800E0

	bigsignA = 0x3C0A0601; // 3C0A0601
	bigsignB = 0x254A9330; // 254A9330

	itemboxesA = 0x3C040601; // 8029DBD4
	itemboxesB = 0x24849498; // 8029DBDC

	battleItemBoxesA = 0x24840038;

	treeslistA = 0x3C040601; // 8029DBBC
	treeslistB = 0x24849570; // 8029DBC4

	treesdisplayA = 0x3C180600; // 802992C8
	treesdisplayB = 0x27186A68; // 802992E0

	piranhalistA = 0x3C040601; // 8029DBC8
	piranhalistB = 0x24849518; // 8029DBD0

	piranhadisplayA = 0x3C0F0600; // 80298668
	piranhadisplayB = 0x25EF6990; // 8029866C

	sectionviewA = 0x3C040900; // 3C040900
	sectionviewB = 0x248401F0; // 248401F0

	battleDisplayA = 0x3C0F0700;
	battleDisplayB = 0x35EF15C0;
	battleSurfaceA = 0x3C040700;
	battleSurfaceB = 0x348415C0;

	surfacemapA = 0x3C040601; // 3C040601
	surfacemapB = 0x24849650; // 24849650

	unknownA1 = 0x3C190601; // 0x802927FC   ;;3C190601 -> 3C190600
	unknownB1 = 0x3C040700; // 0x802927FC   ;;3C190601 -> 3C190600
	unknownC1 = 0x3C040700; // 0x802927FC   ;;3C190601 -> 3C190600
	unknownD1 = 0x3C040700; // 0x802927FC   ;;3C190601 -> 3C190600

	unknownA2 = 0x27399348; // 0x80292810   ;;24849348 -> 24840000

	unknownB = 0x34841140; // 0x802927FC   ;;34841140 -> 34840000
	unknownC = 0x348408E8; // 0x80292810   ;;348408E8 -> 34840000
	unknownD = 0x34842D68; // 0x80295E70   ;;34842D68 -> 34840000

	g_pathLength = 0x0258;
	pathOffset = 0x060057B0;
	pathOffsetB = 0x06005568;

	g_mapStartToggle = 0x0C0132B4;
	asm_GPLevelSelect = 0xA42DC5A0;

	copyCourseTable(0);
}
void overkartASM(void)
{
	// these functions will swap the ASM routines that load Mario's Raceway.
	// they will point to the custom setup we've implemented for custom courses.
	// this does not swap in the course header table, only the ASM instructions.

	highpolypipeA = 0x3C190600; // 3C190700
	highpolypipeB = 0x37390000; // 373908E8

	lowpolypipeA = 0x3C0A0600; // 3C0A0700
	lowpolypipeB = 0x354A0000; // 354A2D68

	uraGrpA = 0x3C0E0600;
	uraGrpB = 0x35CE0000;

	bigmushroomA = 0x3C180600; // 3C180700
	bigmushroomB = 0x37180000; // 37181140

	bigmushroomsurfaceA = 0x3C040600; // 3C040700
	bigmushroomsurfaceB = 0x34840000; // 34841140

	audienceA = 0x3C0F0600; // 3C0F0700
	audienceB = 0x35EF0000; // 35EF14A0

	audienceflagsA = 0x3C0D0600; // 3C0D0700
	audienceflagsB = 0x35AD0000; // 35AD0160

	billboard1A = 0x3C0C0600; //
	billboard1B = 0x358C0000; //

	billboard2A = 0x3C0F0600; //
	billboard2B = 0x35EF0000; //

	billboardpost1A = 0x3C190600; //
	billboardpost1B = 0x37390000; //

	billboardpost2A = 0x3C0E0600; //
	billboardpost2B = 0x35CE0000; //

	fencesA = 0x3C180600; // 3C180700
	fencesB = 0x37180000; // 371800E0

	bigsignA = 0x3C0A0600; // 3C0A0601
	bigsignB = 0x254A0000; // 254A9330

	unknownA1 = 0x3C190600; // 0x802927FC   ;;3C190601 -> 3C190600
	unknownB1 = 0x3C040600; // 0x802927FC   ;;3C190601 -> 3C190600
	unknownC1 = 0x3C040600; // 0x802927FC   ;;3C190601 -> 3C190600
	unknownD1 = 0x3C040600; // 0x802927FC   ;;3C190601 -> 3C190600

	unknownA2 = 0x27390000; // 0x80292810   ;;27399348 -> 27390000

	unknownB = 0x34840000; // 0x802927FC   ;;34841140 -> 34840000
	unknownC = 0x34840000; // 0x80292810   ;;348408E8 -> 34840000
	unknownD = 0x34840000; // 0x80295E70   ;;34842D68 -> 34840000

	g_mapStartToggle = 0x00000000;

	copyCourseTable(1);
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
	asm_GPLevelSelect = 0xA420C5A0;
}



typedef struct OK64AudioTable
{
	uint SeqPointer;
	uint SeqSize;
	uint InsPointer;
	uint InsSize;
} OK64AudioTable;

void BackupStockAudioTables(void)
{
	if (sStockAudioSaved != 0)
	{
		return;
	}
	if ((g_MUSTablePointer == NULL) || (gAlCtlHeader == NULL) || (gAlTbl == NULL))
	{
		return;
	}

	dataLength = 8;
	*sourceAddress = (int)&g_MUSSequenceTable.pointer[3].address;
	*targetAddress = (int)&ok_Sequence;
	runRAM();

	*sourceAddress = (int)&g_MUSInstrumentTable.pointer[3];
	*targetAddress = (int)&ok_Instrument;
	runRAM();

	*(long *)(&ok_USAudio) = g_MUSRawAudioTable.pointer[0].address;
	*(long *)(&ok_USAudio + 1) = g_MUSInstrumentTable.pointer[0].address;
	*(long *)(&ok_MRSong) = g_MUSSequenceTable.pointer[3].address;
	*(long *)(&ok_MRSong + 1) = g_MUSSequenceTable.pointer[3].length;
	sStockAudioSaved = 1;
}

s32 start_race_BGM_custom(void)
{
	if ((HotSwapID > 0) && ((uint)OverKartHeader.Version != 0xFFFFFFFFu) && (OverKartHeader.MusicID < 50))
	{
		NaSeqStart((u16) OverKartHeader.MusicID);
		return 1;
	}
	return 0;
}

void setSong(void)
{
	OK64AudioTable *SongData;

	BackupStockAudioTables();
	if ((g_MUSTablePointer == NULL) || (gAlCtlHeader == NULL))
	{
		return;
	}

	/* MusicID < 50 is a SeqId; start_race_BGM_custom plays it. MusicID >= 50 is a ROM pointer. */
	if ((HotSwapID > 0) && ((uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{

		if (OverKartHeader.MusicID < 50)
		{
			dataLength = 8;
			*sourceAddress = (int)&ok_Sequence;
			*targetAddress = (int)&g_MUSSequenceTable.pointer[3].address;
			runRAM();
			*sourceAddress = (int)&ok_Instrument;
			*targetAddress = (int)&g_MUSInstrumentTable.pointer[3].address;
			runRAM();
		}
		else
		{
			*targetAddress = (int)&ok_FreeSpace;
			*sourceAddress = OverKartHeader.MusicID;
			dataLength = 64;
			runDMA();

			SongData = (OK64AudioTable*)(long)&ok_FreeSpace;

			g_MUSSequenceTable.pointer[3].address = SongData->SeqPointer;
			g_MUSSequenceTable.pointer[3].length = SongData->SeqSize;

			g_MUSInstrumentTable.pointer[3].address = SongData->InsPointer;
			g_MUSInstrumentTable.pointer[3].length = SongData->InsSize;
		}
	}
	else
	{
		dataLength = 8;
		*sourceAddress = (int)&ok_Sequence;
		*targetAddress = (int)&g_MUSSequenceTable.pointer[3].address;
		runRAM();
		*sourceAddress = (int)&ok_Instrument;
		*targetAddress = (int)&g_MUSInstrumentTable.pointer[3].address;
		runRAM();
	}
}

void SetGhostData(void)
{
	if ((HotSwapID > 0) && ((uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{
		if (g_gameMode == GAMEMODE_TT)
		{
			if (OverKartHeader.Ghost != 0)
			{
				*sourceAddress = OverKartHeader.Ghost;
				*targetAddress = (uint)(&ok_FreeSpace);
				dataLength = 0x3C00;
				runDMA();
				*sourceAddress = (uint)(&ok_FreeSpace);
				*targetAddress = (uint)(&KeystockBuffer);
				runMIO();
				KeystockCounter = KeystockBuffer & 0x00FF0000;
			}
			else
			{
				uint *KeystockData = (uint *)(&KeystockBuffer);
				{
				    int ThisPass;
				for (ThisPass = 0; ThisPass < 64; ThisPass++)
				{
					KeystockData[ThisPass] = 0x00FF0000;
				}
				}

				// Should be 9 minutes of idle.
			}
		}
	}
}
void setPath(void)
{
	short *PathLengths = (short *)(&g_pathLength);

	if (g_courseID == 0x14)
	{
		return;
	}
	if ((HotSwapID > 0) && ((uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{

		PathLengths[0] = (short)(OverKartHeader.PathLength[0]);
		PathLengths[1] = (short)(OverKartHeader.PathLength[1]);
		PathLengths[2] = (short)(OverKartHeader.PathLength[2]);
		PathLengths[3] = (short)(OverKartHeader.PathLength[3]);

		PathLengths = (short *)(&g_pathBLength);

		PathLengths[0] = (short)(OverKartHeader.PathLength[0]);

		*sourceAddress = OverKartHeader.PathOffset;
		dataLength = 16;
		if (g_gameMode != GAMEMODE_BATTLE)
		{
			*targetAddress = (uint)(&pathOffset);
			runDMA();
			pathOffsetB = pathOffset + ((OverKartHeader.PathLength[0] + 1) * 8);
		}
		else
		{
			*targetAddress = (uint)(&pathOffsetBlock);
			runDMA();
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
	for (ThisPath = 0; ThisPath < 4; ThisPath++)
	{
		
		Marker *PathArray = (Marker *)GetRealAddress(PathTable[g_courseID][ThisPath]);
		Marker *PathArrayB = (Marker *)GetRealAddress(PathTableB[g_courseID][ThisPath]);
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
				objectPosition[0] = GlobalShortA * (short)((float)PathArray[ThisMark].Position[0] * LevelScales[(int)ScaleXMode]);				
				objectPosition[1] = GlobalShortB * (short)((float)PathArray[ThisMark].Position[1] * LevelScales[(int)ScaleYMode]);				
				objectPosition[2] = -1 * (short)((float)PathArray[ThisMark].Position[2] * LevelScales[(int)ScaleZMode]);


				PathArray[ThisMark].Position[0] = GlobalShortA * (short)((float)PathArray[HighMark].Position[0] * LevelScales[(int)ScaleXMode]);
				PathArray[ThisMark].Position[1] = GlobalShortB * (short)((float)PathArray[HighMark].Position[1] * LevelScales[(int)ScaleYMode]);
				PathArray[ThisMark].Position[2] = -1 * (short)((float)PathArray[HighMark].Position[2] * LevelScales[(int)ScaleZMode]);

				GlobalShortC = PathArray[ThisMark].Group;
				PathArray[ThisMark].Group = PathArray[HighMark].Group;


				PathArray[HighMark].Position[0] = objectPosition[0];
				PathArray[HighMark].Position[1] = objectPosition[1];
				PathArray[HighMark].Position[2] = objectPosition[2];
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
				objectPosition[0] = GlobalShortA * (short)((float)PathArrayB[ThisMark].Position[0] * LevelScales[(int)ScaleXMode]);
				objectPosition[1] = GlobalShortB * (short)((float)PathArrayB[ThisMark].Position[1] * LevelScales[(int)ScaleYMode]);
				objectPosition[2] = -1 * (short)((float)PathArrayB[ThisMark].Position[2] * LevelScales[(int)ScaleZMode]);


				PathArrayB[ThisMark].Position[0] = GlobalShortA * (short)((float)PathArrayB[HighMark].Position[0] * LevelScales[(int)ScaleXMode]);
				PathArrayB[ThisMark].Position[1] = GlobalShortB * (short)((float)PathArrayB[HighMark].Position[1] * LevelScales[(int)ScaleYMode]);
				PathArrayB[ThisMark].Position[2] = -1 * (short)((float)PathArrayB[HighMark].Position[2] * LevelScales[(int)ScaleZMode]);

				GlobalShortC = PathArray[ThisMark].Group;
				PathArray[ThisMark].Group = PathArray[HighMark].Group;

				PathArrayB[HighMark].Position[0] = objectPosition[0];
				PathArrayB[HighMark].Position[1] = objectPosition[1];
				PathArrayB[HighMark].Position[2] = objectPosition[2];				
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

	if ((g_ItemSetFlag == 0) || (g_gameMode == GAMEMODE_TT))
	{
		return;
	}
	GlobalAddressA = GetRealAddress(BoxOffset);
	BoxArray = (Marker *)(GlobalAddressA);

	{
	    int vecAxis;
	for (vecAxis = 0; vecAxis < 3; vecAxis++)
	{
		objectVelocity[vecAxis] = 0;
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
			objectPosition[vecAxis] = (float)BoxArray[ThisBox].Position[vecAxis];
			objectAngle[vecAxis] = MakeRandom();
		}
		}
		objectPosition[0] *= g_mirrorValue;

        objectPosition[0] *= LevelScales[(int)(ScaleXMode)];
        objectPosition[1] *= LevelScales[(int)(ScaleYMode)];
        objectPosition[2] *= LevelScales[(int)(ScaleZMode)];

        if (ZFLIP)
        {
            objectPosition[2] *= -1.0f;
        }
        if (YFLIP)
        {
            objectPosition[1] *= -1.0f;
        }

		GlobalIntA = addObjectBuffer(objectPosition, objectAngle, objectVelocity, IBOX);
		g_SimpleObjectArray[GlobalIntA].fparam = CheckHight(objectPosition[0], objectPosition[1] + 10, objectPosition[2]);
		g_SimpleObjectArray[GlobalIntA].velocity[0] = objectPosition[1];
		g_SimpleObjectArray[GlobalIntA].position[1] = g_SimpleObjectArray[GlobalIntA].fparam - 20;
		g_SimpleObjectArray[GlobalIntA].bump.dummy = BoxArray[ThisBox].Group; // used for forced custom items
	}
	}
}

void setWater(void)
{
	if (HotSwapID > 0)
	{
		g_waterHeight = OverKartHeader.WaterLevel;
	}
}

void CheckEcho(void)
{
}

void setEcho(void)
{
	if ((HotSwapID > 0) && ((uint)OverKartHeader.Version != 0xFFFFFFFFu))
	{
		OverKartRAMHeader.EchoOffset = 0;
		GlobalUIntA = OverKartHeader.EchoEnd - OverKartHeader.EchoStart;
		OverKartRAMHeader.EchoOffset = LoadOKData(OverKartHeader.EchoStart, GlobalUIntA);
		g_EchoStart = 0;
		g_EchoStop = 0;
	}
	else
	{
		OverKartRAMHeader.EchoOffset = 0;
		g_EchoStart = 0x19B;
		g_EchoStop = 0x1B9;
	}
}
// Rain
uint A4Rain[36] = {
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
uint A4Snow[36] = {
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
uint A4Dummy[36] = {
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
				*(uint*)&g_SnowParticleTex[texarr] = A4Dummy[texarr];
			}
			}
			g_skySnowVelocity = 1.75;
			g_3DSnowScale = 0.3;
			g_3DSnowVelocityUpLim = 1.035;
			g_3DSnowVelocityLowLim = -2.5;
			g_3DSnowSwayMovement = 0.01f;
			g_3DSnowSpawnDistanceMin = (g_3DSnowSpawnDistanceMin & 0xFFFF0000) + (30 & 0x0000FFFF);
			g_3DSnowSpawnDistanceMax = (g_3DSnowSpawnDistanceMax & 0xFFFF0000) + (600 & 0x0000FFFF);
			g_3DSnowSpawnHeight = (g_3DSnowSpawnHeight & 0xFFFF0000) + (50 & 0x0000FFFF);
			
			//lol whatever
			g_3DSnowSpawnCone = (g_3DSnowSpawnCone & 0xFFFF0000) + (0x6000 & 0x0000FFFF);
			
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
			g_3DSnowSpawnDistanceMin = (g_3DSnowSpawnDistanceMin & 0xFFFF0000) + (75 & 0x0000FFFF);
			g_3DSnowSpawnDistanceMax = (g_3DSnowSpawnDistanceMax & 0xFFFF0000) + (600 & 0x0000FFFF);
			g_3DSnowSpawnHeight = (g_3DSnowSpawnHeight & 0xFFFF0000) + (45 & 0x0000FFFF);

			//lol whatever
			g_3DSnowSpawnCone = (g_3DSnowSpawnCone & 0xFFFF0000) + (0x6000 & 0x0000FFFF);
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
		g_3DSnowSpawnDistanceMin = (g_3DSnowSpawnDistanceMin & 0xFFFF0000) + (30 & 0x0000FFFF);
		g_3DSnowSpawnDistanceMax = (g_3DSnowSpawnDistanceMax & 0xFFFF0000) + (300 & 0x0000FFFF);
		g_3DSnowSpawnHeight = (g_3DSnowSpawnHeight & 0xFFFF0000) + (45 & 0x0000FFFF);

		//lol whatever
		g_3DSnowSpawnCone = (g_3DSnowSpawnCone & 0xFFFF0000) + (0x4000 & 0x0000FFFF);
	}
}

void SetCloudType(char CloudType) // 0 = None // 1 = MR Clouds // 2 = Stars // 3 = Snow/Rain
{

	if (g_InGame)
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
		CloudCourseID = g_courseID;
	}
}


void SetWeather3D(bool Weather3DEnable) // Enables 3D weather effects (snow/rain) for Singleplayer
{
	
	
	if (Weather3DEnable && HotSwapID > 0)
	{
		if (g_startingIndicator >= 0 && g_startingIndicator < 5 && g_gamePausedFlag == 0x00 && g_playerCount == 1)
		{
			KWChartSnow();
			
		}
		Snow3DCourseID = 5;
	}
	else
	{
		Snow3DCourseID = g_courseID;
	}
}

void SnowCustomCheck(int SnowIndex)
{
	if (g_DynamicObjects[SnowIndex].pos[1] <= GlobalPlayer[0].position[1] - 5.0 || g_DynamicObjects[SnowIndex].pos[1] >= GlobalPlayer[0].position[1] + 60.0)
	{
		g_DynamicObjects[SnowIndex].anmptr = 1;
	}
}

void EventDisplay(int player)
{
	if (HotSwapID == 0)
	{
		KWDisplayEvent(player);
	}
	else
	{
		KWDisplayIceBlockShadow(player);
		KWDisplayBombKartBT(player);
		KWDisplayEvent(player);
	}
}

void EventDisplay_After(int player)
{
	if (HotSwapID == 0)
	{
		KWDisplayEvent_After(player);
	}
	else
	{
		KWDisplayIceBlock(player);
		KWDisplayEvent_After(player);
	}

	if (HotSwapID > 0)
	{
		DrawCustomParticle(player);
	}
}

void CommonGameEventChart(void)
{
	if (HotSwapID == 0)
	{
		KWGameEventCommon();
	}
	else
	{
		if (g_resetToggle != 4)
		{
			KWChartIceBlock();
		}
		KWGameEventCommon();
	}

	if (HotSwapID > 0)
	{
		MoveCustomParticle();
	}
}

void KumoColorMode(uint r, uint g, uint b)
{
    short OKR;
    short OKG;
    short OKB;

    // Get the colors from the OK Header
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

void setSky(void)
{

	if (HotSwapID > 0)
	{
		*targetAddress = (long)&g_skyColorTopTable;
		if (g_gameMode == GAMEMODE_BATTLE)
		{
			*targetAddress += 0xB4;
		}
		*sourceAddress = OverKartHeader.Sky;
		dataLength = 0x10;
		runDMA();
		*targetAddress = (long)&g_skyColorBotTable;
		if (g_gameMode == GAMEMODE_BATTLE)
		{
			*targetAddress += 0xB4;
		}
		*sourceAddress = *sourceAddress + 0x10;
		runDMA();
	}
	else
	{
		*targetAddress = (long)&g_skyColorTopTable;
		*sourceAddress = (long)(SEG_RACING_ROM_START + ((uintptr_t)&g_skyColorTopTable - SEG_RACING));
		if (g_gameMode == GAMEMODE_BATTLE)
		{
			*targetAddress += 0xB4;
			*sourceAddress += 0xB4;
		}		
		
		dataLength = 0x10;
		runDMA();
		*targetAddress = (long)&g_skyColorBotTable;
		*sourceAddress = (long)(SEG_RACING_ROM_START + ((uintptr_t)&g_skyColorBotTable - SEG_RACING));
		if (g_gameMode == GAMEMODE_BATTLE)
		{
			*targetAddress += 0xB4;
			*sourceAddress += 0xB4;
		}		
		
		runDMA();
	}
}

void loadTextureScrollTranslucent(void)
{
	*(long *)(&ok_scrolltranslucent) = 0xFFFFFFFF;
	if (HotSwapID > 0)
	{
		*sourceAddress = OverKartHeader.ScrollROM;
		if (*sourceAddress != 0)
		{
			*targetAddress = (long)&ok_scrolltranslucent;
			dataLength = OverKartHeader.ScrollSize;
			dataLength += 4;
			runDMA();
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

void runTextureScroll(void)
{
	GlobalAddressA = (long)(&ok_scrolltranslucent);
	LoopValue = *(long *)(&ok_scrolltranslucent);
	if ((uint)LoopValue == 0xFFFFFFFFu)
	{
		return;
	}
	
	{
	    int CurrentScroll;
	for (CurrentScroll = 0; CurrentScroll < LoopValue; CurrentScroll++)
	{
		GlobalAddressB = *(long *)(GlobalAddressA + (CurrentScroll * 8) + 4); // address of the texture command to scroll.
		GlobalIntA = *(long *)(GlobalAddressA + (CurrentScroll * 8) + 8);
		GlobalShortA = (GlobalIntA >> 16) & (0xFFFF); // S value
		GlobalShortB = GlobalIntA & 0xFFFF;			  // T value

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
		ScrollMapImage(GlobalAddressB, ScrollValues[CurrentScroll][0], ScrollValues[CurrentScroll][1]);
	}
	}
}

/*
void runWaterVertex(void)
{
	//This handles translucency by setting vertex alpha.
	//Outdated; vertex alpha can be loaded directly now.
	GlobalAddressA = (long)(&ok_scrolltranslucent) + OverKartHeader.WVOffset;
	LoopValue = *(long*)GlobalAddressA;
	if ((uint)LoopValue == 0xFFFFFFFFu)
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
	uint Count;
	uint Address;
} WaveStruct;

void runWaterVertex(void)
{

	GlobalAddressA = (long)(&ok_scrolltranslucent) + OverKartHeader.WVOffset;
	LoopValue = *(long *)GlobalAddressA;
	if ((uint)LoopValue == 0xFFFFFFFFu)
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



void DecodeSP1Triangle_OK(Gfx *gfx,unsigned char *pp,char code )
{
	unsigned int data,v0,v1,v2;
	data=(uint)pp[g_PKCounter++];

	if(g_ScreenFlip != ZFLIP)
	{
	v2=data&0x1f;
	v1=(data>>5)&0x07;
	data=(uint)pp[g_PKCounter++];
	v1|=(data&0x03)<<3;
	v0=(data>>2)&0x1f;
	}
	else
	{
	v0=data&0x1f;
	v1=(data>>5)&0x07;
	data=(uint)pp[g_PKCounter++];
	v1|=(data&0x03)<<3;
	v2=(data>>2)&0x1f;
	}
	gfx[g_GFXCounter].words.w0=(G_TRI1<<24);
	gfx[g_GFXCounter++].words.w1=(((v0*2)<<16)|((v1*2)<<8)|(v2*2));
}
void DecodeSP2Triangle_OK(Gfx *gfx,unsigned char *pp,char code )
{
	unsigned int data,v0,v1,v2,v3,v4,v5;
	data=(uint)pp[g_PKCounter++];

	if(g_ScreenFlip != ZFLIP)
	{
		v2=data&0x1f;
		v1=(data>>5)&0x07;
		data=(uint)pp[g_PKCounter++];
		v1|=(data&0x03)<<3;
		v0=(data>>2)&0x1f;
	}
	else
	{
		v0=data&0x1f;
		v1=(data>>5)&0x07;
		data=(uint)pp[g_PKCounter++];	
		v1|=(data&0x03)<<3;
		v2=(data>>2)&0x1f;
	}

	data=(uint)pp[g_PKCounter++];

	if(g_ScreenFlip != ZFLIP)
	{
		v5=data&0x1f;
		v4=(data>>5)&0x07;
		data=(uint)pp[g_PKCounter++];
		v4|=(data&0x03)<<3;
		v3=(data>>2)&0x1f;
	}
	else
	{
		v3=data&0x1f;
		v4=(data>>5)&0x07;
		data=(uint)pp[g_PKCounter++];
		v4|=(data&0x03)<<3;
		v5=(data>>2)&0x1f;
	}
	gfx[g_GFXCounter].words.w0=((G_TRI2<<24)|((v0*2)<<16)|((v1*2)<<8)|(v2*2));
	gfx[g_GFXCounter++].words.w1=(((v3*2)<<16)|((v4*2)<<8)|(v5*2));
}
void DecodeSP1Quadrangle_OK(Gfx *gfx,unsigned char *pp,char code )
{
	unsigned int data,v0,v1,v2,v3;
	data=(uint)pp[g_PKCounter++];

	if(g_ScreenFlip != ZFLIP)
	{
		v3=data&0x1f;
		v2=(data>>5)&0x07;
		data=(uint)pp[g_PKCounter++];
		v2|=(data&0x03)<<3;
		v1=(data>>2)&0x1f;
		v0=(data>>7)&0x1;
		data=(uint)pp[g_PKCounter++];
		v0|=(data&0x0f)<<1;
	}
	else
	{
		v0=data&0x1f;
		v1=(data>>5)&0x07;
		data=(uint)pp[g_PKCounter++];
		v1|=(data&0x03)<<3;
		v2=(data>>2)&0x1f;
		v3=(data>>7)&0x1;
		data=(uint)pp[g_PKCounter++];
		v3|=(data&0x0f)<<1;

	}
  
	gfx[g_GFXCounter].words.w0=(G_LINE3D<<24);
	gfx[g_GFXCounter++].words.w1=((v3*2)<<24)|((v0*2)<<16)|((v1*2)<<8)|(v2*2);
}


void DecodeVertex2_OK(char *ramaddress, uint number)
{
	char flag, n0, n1;
	uint i;
	Vtx *VtxRAMBuffer;
	ROMVertex *VtxROMData = (ROMVertex*) FreeMemoryPointer;
	int size = (number * sizeof(Vtx));

	VertexMemoryPointer -= size;
	VtxRAMBuffer = (Vtx *)(VertexMemoryPointer);
	for (i = 0; i < number; i++)
	{
		if (g_ScreenFlip)
		{
			VtxRAMBuffer->n.ob[0] = -1 * (short)((float)VtxROMData->ob[0] * LevelScales[(int)ScaleXMode]);
		}			
		else
		{
			VtxRAMBuffer->n.ob[0] = (short)((float)VtxROMData->ob[0] * LevelScales[(int)ScaleXMode]);
		}

			
		if (YFLIP)
		{
			VtxRAMBuffer->n.ob[1] = -1 * (short)((float)VtxROMData->ob[1] * g_ScreenStretch * LevelScales[(int)ScaleYMode]);	
		}
		else
		{
			VtxRAMBuffer->n.ob[1] = (short)((float)VtxROMData->ob[1] * g_ScreenStretch * LevelScales[(int)ScaleYMode]);
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

		flag = n0 & 0x3;
		flag |= (n1 << 2) & 0xc;
		
		VtxRAMBuffer->n.tc[0] = VtxROMData->tc[0];
		VtxRAMBuffer->n.tc[1] = VtxROMData->tc[1];
		VtxRAMBuffer->n.n[0] = n0 & 0xfc;
		VtxRAMBuffer->n.n[1] = n1 & 0xfc;
		VtxRAMBuffer->n.n[2] = VtxROMData->n[2];
		VtxRAMBuffer->n.flag = (unsigned short)flag;

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

void runDisplayScreen(void)
{
	// Handles the display of the buffer copy to texture.
	// Allows for rendering the screen to a set of 6 textures.
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

		GlobalShortA = (short)(g_DispFrame)-1;
		if (GlobalShortA < 0)
			GlobalShortA = 2;
		else if (GlobalShortA > 2)
			GlobalShortA = 0;
		switch (CurrentScreen)
		{
		case 0:
		{
			GetFramebuffer(GlobalIntA, GlobalIntB, 64, 32, (unsigned short *)PhysToK0((int)g_CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 1:
		{
			GetFramebuffer(GlobalIntA + 64, GlobalIntB, 64, 32, (unsigned short *)PhysToK0((int)g_CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 2:
		{
			GetFramebuffer(GlobalIntA, GlobalIntB + 32, 64, 32, (unsigned short *)PhysToK0((int)g_CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 3:
		{
			GetFramebuffer(GlobalIntA + 64, GlobalIntB + 32, 64, 32, (unsigned short *)PhysToK0((int)g_CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 4:
		{
			GetFramebuffer(GlobalIntA, GlobalIntB + 64, 64, 32, (unsigned short *)PhysToK0((int)g_CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
			break;
		}
		case 5:
		{
			GetFramebuffer(GlobalIntA + 64, GlobalIntB + 64, 64, 32, (unsigned short *)PhysToK0((int)g_CfbPtrs[GlobalShortA]), (unsigned short *)(GetRealAddress(GlobalAddressC)));
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

void runKillDisplayObjects(void)
{
	// disables specific track sections based on game-mode and CC speed. 

	GlobalAddressA = ((uint)(&ok_scrolltranslucent)) + OverKartHeader.KDOffset;
	LoopValue = *(int *)GlobalAddressA;
	GlobalAddressA += 4; // Pass Loop Count

	{
	    int CurrentKD;
	for (CurrentKD = 0; CurrentKD < LoopValue; CurrentKD++)
	{
		KDKill *DisplayKill = (KDKill *)(GlobalAddressA);

		uint *MeshList = (uint *)(GlobalAddressA + 12);
		switch (g_gameMode)
		{
            case GAMEMODE_GP:
            {
                // GP
                if (DisplayKill->GP == 0)
                {
                    {
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
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
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
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
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
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
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
                    }
                    }
                }
                break;
            }
		}

		switch (g_raceClass)
		{
            case 0:
            {
                // 50
                if (DisplayKill->Fifty == 0)
                {
                    {
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
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
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
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
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
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
                        uint ThisMesh;
                    for (ThisMesh = 0; ThisMesh < DisplayKill->MeshCount; ThisMesh++)
                    {
                        KillDisplayList(0x07000000 | MeshList[ThisMesh]);
                    }
                    }
                }
                break;
            }
		}

		// Setup next Loop
		GlobalAddressA = GlobalAddressA + 12 + (DisplayKill->MeshCount * 4);
	}
	}
}

void setText(void)
{

	*sourceAddress = OverKartHeader.Credits;
	*targetAddress = (long)&ok_Credits;
	dataLength = 0x20;
	if ((HotSwapID > 0) && (*sourceAddress != 0x00000000))
	{
		runDMA();
	}
	else
	{
		*(long *)(&ok_Credits) = 0;
	}

	*sourceAddress = OverKartHeader.CourseName;
	*targetAddress = (long)&ok_CourseName;
	dataLength = 0x40;
	if ((HotSwapID > 0) && (*sourceAddress != 0x00000000))
	{
		runDMA();
	}
	else
	{
		*(long *)(&ok_CourseName) = 0;
	}

	*sourceAddress = OverKartHeader.SerialKey;
	*targetAddress = (long)&ok_SerialKey;
	dataLength = 0x40;
	if ((HotSwapID > 0) && (*sourceAddress != 0x00000000))
	{
		runDMA();
	}
	else
	{
		*(long *)(&ok_SerialKey) = 0;
	}
}
void SetCourseNames(bool custom)
{
	int strtbl;

	if ((custom == true) && (*sourceAddress != 0x00000000))
	{
		for (strtbl = 0; strtbl < 20; strtbl++)
		{
			g_StringTableCourse[strtbl] = (long)&ok_CourseName + 0x4;
			g_StringTableCourseGP[strtbl] = (long)&ok_CourseName + 0x4;
		}
	}
	else
	{
		for (strtbl = 0; strtbl < 20; strtbl++)
		{
			g_StringTableCourse[strtbl] = (long)stockCourseNames[strtbl];
			g_StringTableCourseGP[strtbl] = (long)stockCourseNames[strtbl];
		}
	}
}

void setOKObjects(void)
{
	// This loads the Custom OK Objects from the header.

	// Load data from the course header.
	OverKartRAMHeader.ObjectTypeCount = *(int *)(OverKartRAMHeader.ObjectDataStart);
	GlobalAddressC = OverKartRAMHeader.ObjectDataStart + 4;
	OverKartRAMHeader.ObjectTypeList = (OKObjectType *)(GlobalAddressC);

	GlobalAddressB = OverKartRAMHeader.ObjectDataStart + 4 + (OverKartRAMHeader.ObjectTypeCount * sizeof(OKObjectType)); // 32 bytes size of ObjectType
	OverKartRAMHeader.ObjectCount = *(int *)(GlobalAddressB);
	GlobalAddressD = GlobalAddressB + 4;
	OverKartRAMHeader.ObjectList = (OKObjectList *)(GlobalAddressD);

    
	{
	    int This;
	for (This = 0; This < OverKartRAMHeader.ObjectCount; This++)
	{
		OKObjectType ThisType;

		// Loop through the object count and set each individual object in the array.

		OKObjectArray[This].ListIndex = This;
		OKObjectArray[This].TypeIndex = OverKartRAMHeader.ObjectList[This].TypeIndex;
		
		

		OKObjectArray[This].SubBehaviorClass = SUBBEHAVIOR_DOCILE;

		ThisType = OverKartRAMHeader.ObjectTypeList[OKObjectArray[This].TypeIndex];

		OKObjectArray[This].ObjectData.flag = 0xC000;
		OKObjectArray[This].ObjectData.radius = (ThisType.BumpRadius / 100.0f); // used for level calcs BUMP
		

		if (g_ScreenFlip)
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
			OverKartRAMHeader.ObjectList[This].OriginPosition[1] += OKObjectArray[This].ObjectData.radius;
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

		OKObjectArray[This].ObjectData.position[0] = OverKartRAMHeader.ObjectList[This].OriginPosition[0];
		OKObjectArray[This].ObjectData.position[1] = OverKartRAMHeader.ObjectList[This].OriginPosition[1];
		OKObjectArray[This].ObjectData.position[2] = OverKartRAMHeader.ObjectList[This].OriginPosition[2];

		OKObjectArray[This].ObjectData.angle[0] = OverKartRAMHeader.ObjectList[This].OriginAngle[0] * DEG1;
		OKObjectArray[This].ObjectData.angle[1] = -1 * OverKartRAMHeader.ObjectList[This].OriginAngle[1] * DEG1;
		OKObjectArray[This].ObjectData.angle[2] = OverKartRAMHeader.ObjectList[This].OriginAngle[2] * DEG1;

		OKObjectArray[This].ObjectData.velocity[0] = (float)(OverKartRAMHeader.ObjectList[This].OriginVelocity[0] * 100);
		OKObjectArray[This].ObjectData.velocity[1] = (float)(OverKartRAMHeader.ObjectList[This].OriginVelocity[1] * 100);
		OKObjectArray[This].ObjectData.velocity[2] = (float)(OverKartRAMHeader.ObjectList[This].OriginVelocity[2] * 100);

        
		OKObjectArray[This].AngularVelocity[0] = OverKartRAMHeader.ObjectList[This].OriginAngularVelocity[0] * DEG1;
		OKObjectArray[This].AngularVelocity[1] = OverKartRAMHeader.ObjectList[This].OriginAngularVelocity[1] * DEG1;
		OKObjectArray[This].AngularVelocity[2] = OverKartRAMHeader.ObjectList[This].OriginAngularVelocity[2] * DEG1;

		
		if (ThisType.ObjectAnimations != 0xFFFFFFFF)
		{
			uint *AnimationOffsets = (uint *)(GetRealAddress(ObjectSegment | ThisType.ObjectAnimations));
			GlobalAddressA = GetRealAddress(ObjectSegment | AnimationOffsets[0]);
			OKObjectArray[This].AnimationMax = (uchar) * (int *)(GlobalAddressA);
			// OKObjectArray[This].AnimationFrame = MakeRandomLimmit((ushort)(OKObjectArray[This].AnimationMax));
		}
	}
	}
}

Gfx* IceKageGfx;

void LoadIceKage(void)
{
	*sourceAddress = (long)&IceKageROM;
	*targetAddress = FreeMemoryPointer;
	dataLength = (long)&IceKageEnd - (long)&IceKageROM;
	runDMA();
	IceKageGfx = (Gfx *) FreeMemoryPointer;
	SetSegment(8, (void *) FreeMemoryPointer);
	FreeMemoryPointer += dataLength;
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
		g_StaticObjectCount = 0;
		g_simpleObjectCount = 0;
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
		g_StaticObjectCount = g_simpleObjectCount;
	}
	{
	    int ThisObject;
	for (ThisObject = 0; ThisObject < g_StaticObjectCount; ThisObject++)
	{

        if (g_SimpleObjectArray[ThisObject].category == IBOX)
        {
            continue;
        }


        g_SimpleObjectArray[ThisObject].position[0] = ((float)g_SimpleObjectArray[ThisObject].position[0] * LevelScales[(int)ScaleXMode]);
        if (YFLIP)
        {
            g_SimpleObjectArray[ThisObject].position[1] = -1 * ((float)g_SimpleObjectArray[ThisObject].position[1] * LevelScales[(int)ScaleYMode]);
        }
        else
        {
            g_SimpleObjectArray[ThisObject].position[1] = ((float)g_SimpleObjectArray[ThisObject].position[1] * LevelScales[(int)ScaleYMode]);
        }
        if (ZFLIP)
        {
            g_SimpleObjectArray[ThisObject].position[2] = -1 * ((float)g_SimpleObjectArray[ThisObject].position[2] * LevelScales[(int)ScaleZMode]);
        }
        else
        {
            g_SimpleObjectArray[ThisObject].position[2] = ((float)g_SimpleObjectArray[ThisObject].position[2] * LevelScales[(int)ScaleZMode]);
        }
        
        

		

		
	}
	}
}
void InitialMapCode(void)
{
	InitialMap();	
	setPath();
}

void loadOKObjects(void)
{
	// Load all the OKObject data from the Course Header.
	GlobalUIntA = OverKartHeader.ObjectDataEnd - OverKartHeader.ObjectModelStart;
	SetSegment(0xA, (void *) LoadOKData(OverKartHeader.ObjectModelStart, GlobalUIntA));
	GlobalUIntA = OverKartHeader.ObjectModelStart - OverKartHeader.ObjectDataStart;
	GlobalAddressA = LoadOKData(OverKartHeader.ObjectDataStart, GlobalUIntA);
	OverKartRAMHeader.ObjectDataStart = GlobalAddressA;
}

void loadHeaderOffsets(void)
{
	// Load the offsets to the custom course headers.
	*targetAddress = (long)&ok_HeaderOffsets;
	*sourceAddress = (long)&ok_HeaderROM;
	dataLength = 0x3C00;
	runDMA();
}

void LoadBomb(void)
{
	if (HotSwapID > 0)
	{
		*sourceAddress = OverKartHeader.BombOffset;
	}
	else
	{
		*sourceAddress = (long)&ok_Bomb;
	}
	*targetAddress = (int)&g_BombTable;
	dataLength = 0xA8;
	runDMA();
}

#define COURSE_TABLE_ROW_SIZE 0x30
#define COURSE_TABLE_BATTLE_OFFSET 0x2D0
#define OK_HEADER_OLD_SIZE 0xC0

/* Amped Up / Version <= 5 packed layout. File-local; do not replace live OKHeader (0xD0). */
typedef struct OKHeaderOld {
	int Version;
	CourseHeader MapHeader;
	uint SectionViewPosition;
	uint XLUSectionViewPosition;
	uint SurfaceMapPosition;
	uint Sky;
	char SkyType, WeatherType, PathSplit, PathCount;
	uint Credits;
	uint CourseName;
	uint SerialKey;
	uint Ghost;
	uint Maps;
	uint ObjectDataStart;
	uint ObjectModelStart;
	uint ObjectAnimationStart;
	uint ObjectDataEnd;
	uint BombOffset;
	uint EchoStart;
	uint EchoEnd;
	char GoalBannerToggle;
	char BackgroundToggle;
	char ManualTempo;
	char Padding;
	char PathTrigger[4];
	uint MusicID;
	short PathLength[4];
	char GhostCharacter, WaterType;
	short WaterLevel;
	uint ScrollROM;
	short WVOffset, ScreenOffset, KDOffset, ScrollSize;
	uint PathOffset;
	short FogStart, FogStop;
	uchar FogRGBA[4];
	uchar Tail[0x18];
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
		if ((uint)headerROM != 0xFFFFFFFFu)
		{
			*sourceAddress = headerROM;
			*targetAddress = (long)&version;
			dataLength = 4;
			runDMA();

			if (version <= 5)
			{
				OKHeaderOld oldHeader;

				*sourceAddress = headerROM;
				*targetAddress = (long)&oldHeader;
				dataLength = OK_HEADER_OLD_SIZE;
				runDMA();
				CopyOKHeaderOld(&oldHeader);
			}
			else
			{
				*sourceAddress = headerROM;
				*targetAddress = (long)&ok_CourseHeader;
				dataLength = sizeof(OKHeader);
				runDMA();
			}

			VersionNumber = OverKartHeader.Version;

			*targetAddress = (long)&g_courseTable;

			if (g_gameMode == GAMEMODE_BATTLE)
			{
				*targetAddress += COURSE_TABLE_BATTLE_OFFSET;
			}

			*sourceAddress = (long)(&OverKartHeader.MapHeader);
			dataLength = COURSE_TABLE_ROW_SIZE;
			runRAM();
		}
		else
		{
			ClearStockCourseHeader();
		}
	}
	else
	{
		ClearStockCourseHeader();
		*sourceAddress = (long)(SEG_RACING_ROM_START + ((uintptr_t)&g_courseTable - SEG_RACING));
		*targetAddress = (long)&g_courseTable;
		dataLength = COURSE_TABLE_ROW_SIZE;

		if (g_gameMode == GAMEMODE_BATTLE)
		{
			*targetAddress += COURSE_TABLE_BATTLE_OFFSET;
			*sourceAddress += COURSE_TABLE_BATTLE_OFFSET;
		}

		runDMA();
	}
}

void SetCustomData(void)
{
	setText();
	setEcho();
	setSong();
	LoadBomb();

	if (((uint)OverKartHeader.Version != 0xFFFFFFFFu) && (HotSwapID > 0))
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

static u16 *BannerPixels(int courseIndex)
{
	MenuTexture *ram;
	int i;

	if ((tex_buff == NULL) || (courseIndex < 0) || (courseIndex >= 20))
	{
		return NULL;
	}
	ram = (MenuTexture *) GetTBPointer(D_800E7DC4[courseIndex]);
	if ((ram == NULL) || (ram->textureData == NULL))
	{
		return NULL;
	}
	for (i = 0; i < all_tex_block_num; i++)
	{
		if (sMenuTextureMap[i].textureData == ram->textureData)
		{
			return &tex_buff[sMenuTextureMap[i].offset];
		}
	}
	return NULL;
}

/* Live g_CourseBannerOffsets / g_BattleBannerOffsets: first bank, then +5056. */
static u16 *BannerBank(int slot)
{
	u16 *base;

	if (g_gameMode == GAMEMODE_BATTLE)
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
	long length;
	int customIndex;

	dest = BannerBank(slot);
	if (dest == NULL)
	{
		return;
	}

	if (g_gameMode == GAMEMODE_BATTLE)
	{
		customIndex = 16 + slot;
	}
	else
	{
		customIndex = slot;
	}

	GlobalAddressA = ((long)(&ok_MenuOffsets) + (customIndex * 8) + ((HotSwapID - 1) * 160));
	*sourceAddress = *(long *)(GlobalAddressA);
	if ((*sourceAddress != 0x00000000) && ((uint)*sourceAddress != 0xFFFFFFFFu))
	{
		GlobalAddressB = (GlobalAddressA + 4);
		length = (*(long *)(GlobalAddressB) - *sourceAddress) + 16;
		if ((length < 16) || (length > 0x8000))
		{
			*sourceAddress = (long)&bannerU;
		}
		else
		{
			*targetAddress = (long)&ok_FreeSpace;
			dataLength = length;
			runDMA();
			*sourceAddress = (long)&ok_FreeSpace;
		}
	}
	if (*sourceAddress == 0x00000000)
	{
		*sourceAddress = (long)&bannerU;
	}
	if ((uint)*sourceAddress == 0xFFFFFFFFu)
	{
		*sourceAddress = (long)&bannerN;
	}
	*targetAddress = (long) dest;
	runMIO();
}

static void RestoreStockBanner(int slot)
{
	MenuTexture *ram;
	u16 *dest;
	int courseIndex;

	dest = BannerBank(slot);
	if (dest == NULL)
	{
		return;
	}
	if (g_gameMode == GAMEMODE_BATTLE)
	{
		courseIndex = g_CupArray[16 + slot];
	}
	else
	{
		courseIndex = g_CupArray[slot];
	}
	if ((courseIndex < 0) || (courseIndex >= 20))
	{
		return;
	}
	ram = (MenuTexture *) GetTBPointer(D_800E7DC4[courseIndex]);
	if ((ram == NULL) || (ram->textureData == NULL))
	{
		return;
	}
	dataLength = 0x1000;
	*sourceAddress = (long)(_textures_0bSegmentRomStart + SEGMENT_OFFSET(ram->textureData));
	*targetAddress = (long)&ok_FreeSpace;
	runDMA();
	*sourceAddress = (long)&ok_FreeSpace;
	*targetAddress = (long) dest;
	runTKM();
}

void setBanners(void)
{
	int slot;
	int slotCount;

	if (MapIsCurrentlyLoading)
	{
		return;
	}

	if (g_gameMode == GAMEMODE_BATTLE)
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

void setPreviews(void)
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
			*sourceAddress = *(long *)(GlobalAddressA);
			if (*sourceAddress == 0x00000000)
			{
				*sourceAddress = (long)&previewU;
			}
			if ((uint)*sourceAddress == 0xFFFFFFFFu)
			{
				*sourceAddress = (long)&previewN;
			}
			rom = *sourceAddress - (long)_textures_0aSegmentRomStart;
			rom |= 0x0A000000;
			ram = (MenuTexture *) GetTBPointer(D_800E7D74[currentCourse]);
			if (ram == NULL)
			{
				continue;
			}
			ram->textureData = (u64 *) rom;
			ram->size = 0x4000;
		}
		sCustomMenuLive = 1;
	}
	else
	{
		if (sCustomMenuLive == 0)
		{
			return;
		}
		/* r_CoursePreviewOffsets → cart copy of the packed MenuTexture[2] table. */
		*sourceAddress = (long)(_data_segment2SegmentRomStart +
				SEGMENT_OFFSET(seg2_mario_raceway_preview_texture));
		*targetAddress = (long) GetTBPointer(seg2_mario_raceway_preview_texture);
		dataLength = 0x320;
		runDMA();
	}
}

void swapHS(int direction)
{
	if (MapIsCurrentlyLoading)
	{
		return;
	}
	// This function will swap to a new set of custom levels.
	if (direction == 0)
	{
		if (HotSwapID > 0)
		{
			if (HotSwapID == 1)
			{
				// stockASM();
				copyCourseTable(0);
			}
			HotSwapID--;

			playSound(0x4900801A);
		}
	}
	else
	{
		if (HotSwapID < 4)
		{
			if (HotSwapID == 0)
			{
				// overkartASM();
				copyCourseTable(1);
				{
				    int Index;
				for (Index = 0; Index < 20; Index++)
				{
					g_CupArray[Index] = Index;
				}
				}
			}
			HotSwapID++;

			playSound(0x4900801A);
		}
	}

	setPreviews();
	previewRefresh();
	setBanners();
	courseValue = -1;
}



typedef struct MiniMapStruct{
	short	MapX, MapY;
	short	StartX, StartY;
	short	LineX, LineY;
	short	Height, Width;
	short	R,G,B,A;
	float	Scale;
	
} MiniMapStruct;
void loadMinimap(void)
{
	MiniMapStruct *MiniMapData;

	if (HotSwapID == 0)
	{
		return;
	}

	*sourceAddress = OverKartHeader.Maps;

	if (*sourceAddress != 0x00000000)
	{
		*targetAddress = (long)&ok_FreeSpace;
		dataLength = 0x1000;
		runDMA();
		MiniMapData = (MiniMapStruct*)(long)&ok_FreeSpace;

		if (g_playerCount == 1)
		{	
			g_mapX[0] = MiniMapData->MapX;
			g_mapY[0] = MiniMapData->MapY;
		}
		if (g_ScreenFlip == 1)
		{
			g_startX = g_mapWidth - g_startX;
		}
		RadarLineX = MiniMapData->LineX;
		RadarLineY = MiniMapData->LineY;
		g_startX = MiniMapData->StartX;
		g_startY = MiniMapData->StartY;				
		g_mapHeight = MiniMapData->Height;
		g_mapWidth = MiniMapData->Width;
		g_mapR = MiniMapData->R;
		g_mapG = MiniMapData->G;
		g_mapB = MiniMapData->B;
		g_mapScale = MiniMapData->Scale * 0.01;
		*sourceAddress = (long)(&ok_FreeSpace) + sizeof(MiniMapStruct);
		*targetAddress = (long)&ok_MapTextureData;
		runMIO();
		g_mapTexture = (long)&ok_MapTextureData;
	}
}

int tempFireParticleMax = 8;

void initFireParticles(long PathOffset)
{
	*tempPointer = PathOffset;
	*sourceAddress = (long)&FireParticlePositions;
	*targetAddress = (long)&objectPosition;
	{
	    int currentFire;
	for (currentFire = 0; currentFire < tempFireParticleMax; currentFire++)
	{

		FireParticlePositions[currentFire][0] = *(short *)(PathOffset);
		FireParticlePositions[currentFire][1] = *(short *)(PathOffset + 2);
		FireParticlePositions[currentFire][2] = *(short *)(PathOffset + 4);

		if (FireParticlePositions[currentFire][0] == (short)-32768)
		{
			if (currentFire < tempFireParticleMax - 1)
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
	for (currentFire = 0; currentFire < tempFireParticleMax; currentFire++)
	{
		KWGetCaveFire(currentFire);

		g_DynamicObjects[FireParticleAllocArray[currentFire]].pos_start[0] = (float)FireParticlePositions[currentFire][0];
		g_DynamicObjects[FireParticleAllocArray[currentFire]].pos_start[1] = (float)FireParticlePositions[currentFire][1] + 10;
		g_DynamicObjects[FireParticleAllocArray[currentFire]].pos_start[2] = (float)FireParticlePositions[currentFire][2];

		g_DynamicObjects[FireParticleAllocArray[currentFire]].scale = 0.8;
	}
	}
}

char FireRGBA[8][4] = {{150, 0, 25, 255}, {150, 20, 55, 255}, {50, 50, 255, 255}, {50, 255, 50, 255}, {150, 90, 255, 255}, {250, 55, 55, 155}, {100, 255, 25, 255}, {250, 105, 55, 255}};
// 8 = tempFireParticleMax

void DisplayFireParticleSub(int num, uchar color, void *Camera)
{
	int realindex = num - FireParticleAllocArray[0];

	if (HotSwapID > 0)
	{
		CaveFireColCheck = 0x24010004;
	}
	else
	{
		CaveFireColCheck = 0x24010003;
	}

	switch (color)
	{
	case 3:
		KWSet2Color(FireRGBA[realindex][0], FireRGBA[realindex][1], FireRGBA[realindex][2], FireRGBA[realindex][0] / 3, FireRGBA[realindex][1] / 3, FireRGBA[realindex][2] / 3, FireRGBA[realindex][3]);
		break;
	}
	KWDisplayFireParticleSub(num, color, Camera);
}

void EmptyActionData(void)
{
	GlobalAddressA = (uint)ActionData_Pointer[19];
	ActionData_Pointer[0] = ActionData_Pointer[19];

	*(ushort *)(GlobalAddressA) = 1;
	*(ushort *)(GlobalAddressA + (0x2)) = (ushort)MaxPathPoints[0];
	*(ushort *)(GlobalAddressA + (0x6)) = 6;
}

void SearchListHook(uint Addr)
{
	if (HotSwapID > 0)
	{
		return;
	}
	
	SearchList(Addr);
}

void SearchListFileHook(uint addr)
{

    if (HotSwapID > 0)
    {
        SearchListFile(0x06000000 | OverKartHeader.SurfaceMapPosition);
        return;
    }
	SearchListFile(addr);   
		
    
}

void SearchList2Hook(uint addr, char Surface)
{
	if (HotSwapID > 0)
    {
        SearchListFile(0x06000000 | OverKartHeader.SurfaceMapPosition);
        return;
    }
	SearchList2(addr, Surface);
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
			g_fogR = (uint)OverKartHeader.FogRGBA[0];
			g_fogG = (uint)OverKartHeader.FogRGBA[1];
			g_fogB = (uint)OverKartHeader.FogRGBA[2];
			gDPSetFogColor(GraphPtrOffset++, (uint)OverKartHeader.FogRGBA[0], (uint)OverKartHeader.FogRGBA[1], (uint)OverKartHeader.FogRGBA[2], 255);
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
		if (g_gameMode != GAMEMODE_BATTLE)
		{
			if (OverKartHeader.FogStart > 0)
			{
				g_fogToggleBanshee = 1;
				g_fogR = (uint)OverKartHeader.FogRGBA[0];
				g_fogG = (uint)OverKartHeader.FogRGBA[1];
				g_fogB = (uint)OverKartHeader.FogRGBA[2];
				gDPSetFogColor(GraphPtrOffset++, (uint)OverKartHeader.FogRGBA[0], (uint)OverKartHeader.FogRGBA[1], (uint)OverKartHeader.FogRGBA[2], 255);
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
void RadarFinishLine(uint ulx,uint uly,ushort *addr)
{
	KWSprite8x8(ulx + RadarLineX, uly + RadarLineY, addr);	
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
