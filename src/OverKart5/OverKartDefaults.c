#include "MainInclude.h"
#include "save.h"
#include "replays.h"
#include "crash_screen.h"

extern void CrashScreenPrint(s32 x, s32 y, const char* fmt, ...);

void EnemyTekagenSub(s32 PlayerId, Player* Player);
void DrawPerScreen(Camera* LocalCamera);
void ExecuteItemHook(Player* Car);
void MiniMapDraw(void);
void ItemboxCollideCheck(Player* Car, Object* Target);
void RubberBandMan(s32 PlayerId, Player* Player);

/*
 * Hook points for projects that include OverKart Library.
 * Keep stock fall-through here until a host project supplies its own.
 */

void GameCodeDefault(void)
{
	GameCode();
}

void TitleMenuDefault(void)
{
	TitleMenu();
}

void AllRunDefault(void)
{
	AllRun();
}

void PrintMenuFunctionDefault(void)
{
	PrintMenuFunction();
}

void DisplayObjectDefault(void* car, Object* inputObject)
{
	DisplayObject(car, inputObject);
}

void CollideObjectDefault(Player* Car, Object* Target)
{
	CollideObject(Car, Target);
}

void DrawPerScreenDefault(Camera* LocalCamera)
{
	DrawPerScreen(LocalCamera);
}

void ExecuteItemHookDefault(Player* Car)
{
	ExecuteItemHook(Car);
}

void MiniMapDrawDefault(void)
{
	MiniMapDraw();
}

void BalloonCheckDefault(Player* Player, f32 arg1, f32 Arg2, s8 playerIndex, s8 balloonIndex, s16 Rotation)
{
	BalloonDisp(Player, arg1, Arg2, playerIndex, balloonIndex, Rotation);
}

void ItemboxCollideCheckDefault(Player* Car, Object* Target)
{
	ItemboxCollideCheck(Car, Target);
}

void KWKumoAllocHookDefault(void)
{
	if (HotSwapID > 0)
	{
		LoadCustomHeader(CourseValue + GPCourseIndex);

		SetCloudType((char) OverKartHeader.SkyType);
		SetWeatherType((char) OverKartHeader.WeatherType);
		GlobalShortA = CourseID;
		CourseID = CloudCourseID;
		KWKumoAlloc();
		CourseID = GlobalShortA;
	}
	else
	{
		KWKumoAlloc();
	}
}

void KWChartKumoHookDefault(int screen_num)
{
	AnmObject* cloud;
	int cloudcount;

	if (HotSwapID > 0)
	{
		GlobalShortA = CourseID;
		CourseID = CloudCourseID;
		KWChartKumo(screen_num);
		CourseID = GlobalShortA;
	}
	else
	{
		KWChartKumo(screen_num);
	}

	if ((char) OverKartHeader.WeatherType == 0)
	{
		return;
	}

	cloudcount = CloudCount1P;
	if (PlayerCount == 2)
	{
		cloudcount = CloudCount2P;
	}

	{
		int i;
		for (i = 0; i < cloudcount; i++)
		{
			if (CloudAllocate[i] == 0)
			{
				return;
			}
			cloud = (AnmObject*) &DynamicObjects[CloudAllocate[i]];
			cloud->Velocity[1] = -5.0f;
			cloud->Scale = 0.4;
		}
	}
}

void MapStartupDefault(short InputID)
{
	MapIsCurrentlyLoading = true;
	FreeMemoryPointer = StaticMemoryPointer;
	VertexMemoryPointer = VERTEX_MEMORY_TOP;
	LastMemoryPointer = LAST_MEMORY_TOP;
	ObjectMemoryPointer = (uintptr_t) ok_ObjectPoolStart;
	CourseFaceMaxIndex = 0;
	if (HotSwapID > 0)
	{
		if (GameMode != GAMEMODE_BATTLE)
		{
			CourseID = 0;
		}
		else
		{
			CourseID = 15;
		}
		InputID = CourseID;
	}
	LoadCustomHeader(CourseValue + GPCourseIndex);
	SetCustomData();
	SetSky();
	SetWater();
	LoadCourse(InputID);
	LoadIceKage();
	CheckCourseMemoryHighWater();

	if (HotSwapID > 0)
	{
		LoadTextureScrollTranslucent();
		RunKillDisplayObjects();
		LoadMinimap();
	}
}

void DrawMapSelectDefault(void)
{
	DrawBox(65, 18, 190, 25, 0, 0, 0, 175);
	if (HotSwapID == 0)
	{
		PrintBigText(80, 16, 0.9f, "Original Set");
	}
	else if (HotSwapID < 10)
	{
		PrintBigTextNumberNoGap(80, 16, 0.9f, "Custom Set ", HotSwapID);
	}
	else
	{
		PrintBigTextNumberNoGap(80, 16, 0.9f, "Custom Set", HotSwapID);
	}
	SpriteBtnCLeft(45, 35, 1.0, false);
	SpriteBtnCRight(279, 35, 1.0, false);
}

void MapSelectMenuDefault(void)
{
	if (GameMode == GAMEMODE_BATTLE)
	{
		GlobalShortA = 4;
	}
	else
	{
		GlobalShortA = 1;
	}
	if (MenuScreenA == GlobalShortA)
	{
		if ((GlobalController[4]->ButtonPressed & BTN_CLEFT) == BTN_CLEFT)
		{
			SwapHS(0);
		}
		else if ((GlobalController[4]->ButtonPressed & BTN_CRIGHT) == BTN_CRIGHT)
		{
			SwapHS(1);
		}
		LoadCustomHeader(CourseValue);

		if ((GlobalController[4]->ButtonPressed & BTN_R) == BTN_R)
		{
			MenuToggle = !MenuToggle;
		}
	}

	*(int*) (&CharacterGridIsSelected) = 0;
	if (HotSwapID > 0)
	{
		CourseValue = -1;
		GPCourseIndex = 0;
		switch (GameMode)
		{
			case 0:
			{
				CourseValue = (CupSelect * 4);
				break;
			}
			case 1:
			case 2:
			case 3:
			{
				CourseValue = (CupSelect * 4) + CourseSelect;
				break;
			}
		}
	}
	else
	{
		LoadCustomHeader(-1);
	}
}

void CheckRomghostDefault(void)
{
	if (HotSwapID == 0)
	{
		CheckRomghost();
		return;
	}

	RomghostStatus = 1;
}

void DMAROMGhostDefault(void)
{
	if (HotSwapID == 0)
	{
		DMAROMGhost();
	}
}

void SaveFunc800B45E0Default(s32 arg0)
{
	SaveFunc800B45E0(arg0);
}

void SaveFunc800B4670Default(void)
{
	SaveFunc800B4670();
}

void LoadFunc800B4A10Default(void)
{
	LoadFunc800B4A10();
}

void SaveFunc800B559CDefault(s32 arg0)
{
	SaveFunc800B559C(arg0);
}

void SaveFunc800B5948Default(void)
{
	SaveFunc800B5948();
}

void RubberBandManDefault(s32 PlayerId, Player* Player)
{
	RubberBandMan(PlayerId, Player);
}

bool PlaceIBoxesAllowedDefault(void)
{
	return true;
}

f32 TrickHangGravityDefault(int store)
{
	(void) store;
	return TRICK_GRAVITY;
}

int BannerDestCourseDefault(int slot)
{
	if (GameMode == GAMEMODE_BATTLE)
	{
		return ((s16 *) CupCourseOrder)[16 + slot];
	}
	return ((s16 *) CupCourseOrder)[slot];
}

void EventDisplayAfterHookDefault(int Player)
{
	(void) Player;
}

void FPSCheckDefault(void)
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

void DynamicTempoDefault(void)
{
	FPSCheck();
}

void KumoColorModeDefault(Uint r, Uint g, Uint b)
{
	short OKR;
	short OKG;
	short OKB;

	(void) r;
	(void) g;
	(void) b;

	/* Get the Colors from the OK Header */
	if (HotSwapID == 0)
	{
		SparkMode(255, 255, 255);
		return;
	}
	OKR = OverKartHeader.FogRGBA[0];
	OKG = OverKartHeader.FogRGBA[1];
	OKB = OverKartHeader.FogRGBA[2];

	SparkMode(OKR, OKG, OKB);
}

void ResetLaneDefaultOffsetDefault(int PlayerID)
{
	switch (MakeRandomLimmit(3))
	{
	case 0:
		LaneData[PlayerID].LaneFreeDrive = -(RoadWidth[0] * 0.005f);
		break;
	case 1:
		LaneData[PlayerID].LaneFreeDrive = 0;
		break;
	case 2:
		LaneData[PlayerID].LaneFreeDrive = (RoadWidth[0] * 0.005f);
		break;
	default:
		break;
	}
	LaneData[PlayerID].Speed = 10.0f;
}

void MakeAlternativePathDefault(Marker *altPath, short Length, char pathID)
{
	CenterPathBP[(int)pathID] = (CenterPathStruct *) altPath;
	CourseTotalPathPoints[(int)pathID] = Length;
	MaxPathPoints[(int)pathID] = Length;

	{
		int i;
		for (i = 0; i < Length; i++)
		{
			CenterPathBP[(int)pathID][i].Pointx *= MirrorValue;
		}
	}

	SideLPathBP[(int)pathID] = (CenterPathStruct *) AllocateMemory(MaxPathPoints[(int)pathID]*(sizeof(CenterPathStruct)));
	SideRPathBP[(int)pathID] = (CenterPathStruct *) AllocateMemory(MaxPathPoints[(int)pathID]*(sizeof(CenterPathStruct)));
	CurvePathDataBP[(int)pathID] = (short *)AllocateMemory(MaxPathPoints[(int)pathID]*(sizeof(short)));
	AnglePathDataBP[(int)pathID] = (short *)AllocateMemory(MaxPathPoints[(int)pathID]*(sizeof(short)));
	ShortCutPathDataBP[(int)pathID] = (short *)AllocateMemory(MaxPathPoints[(int)pathID]*(sizeof(short)));

	SidePointCalcBP((int)pathID);
	CurveDataCalcBP((int)pathID);
	AngleDataCalcBP((int)pathID);
	ShortcutDataCalcBP((int)pathID);
}

void DrawButtonInputDisplayDefault(bool FullMenu)
{
	short PosX[4], PosY[4];
	short OffsetX;
	short OffsetY;
	float Scale;
	int i;

	if (StartingIndicator < 3 || StartingIndicator > 4 || PlayerCount > 2)
	{
		return;
	}

	OffsetX = 85;
	OffsetY = 55;
	Scale = 0.5f;
	switch (PlayerCount)
	{
	case 2:
		OffsetY = 30;
		break;
	}

	{
	for (i = 0; i < PlayerCount; i++)
	{

		if (GamePlayers[i].Flag&IS_CPU_PLAYER)
		{
			continue;
		}

		PosX[i] = GlobalScreen[i]->Posx-GlobalScreen[i]->Width/2 +OffsetX;
		PosY[i] = GlobalScreen[i]->Posy-GlobalScreen[i]->Height/2 +OffsetY;

		DisplayListHead = FillRect1ColorF(DisplayListHead,
		PosX[i] - 20, PosY[i] - 22, PosX[i] + 40, PosY[i] + 10,
		0, 0, 0, 64);

		if (GlobalController[i]->ButtonHeld&BTN_A)
		{
			SpriteBtnA(PosX[i]+21,PosY[i]+3,Scale,1);
		}
		else
		{
			SpriteBtnA(PosX[i]+21,PosY[i]+3,Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_B)
		{
			SpriteBtnB(PosX[i]+15,PosY[i]-3,Scale,1);
		}
		else
		{
			SpriteBtnB(PosX[i]+15,PosY[i]-3,Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_START)
		{
			SpriteBtnStart(PosX[i]+4,PosY[i]+5,Scale,1);
		}
		else
		{
			SpriteBtnStart(PosX[i]+4,PosY[i]+5,Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_CDOWN)
		{
			SpriteBtnCDown(PosX[i]+30,PosY[i],Scale,1);
		}
		else
		{
			SpriteBtnCDown(PosX[i]+30,PosY[i],Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_CLEFT)
		{
			SpriteBtnCLeft(PosX[i]+25,PosY[i]-5,Scale,1);
		}
		else
		{
			SpriteBtnCLeft(PosX[i]+25,PosY[i]-5,Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_CRIGHT)
		{
			SpriteBtnCRight(PosX[i]+35,PosY[i]-5,Scale,1);
		}
		else
		{
			SpriteBtnCRight(PosX[i]+35,PosY[i]-5,Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_CUP)
		{
			SpriteBtnCUp(PosX[i]+30,PosY[i]-10,Scale,1);
		}
		else
		{
			SpriteBtnCUp(PosX[i]+30,PosY[i]-10,Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_R)
		{
			SpriteBtnR(PosX[i]+35,PosY[i]-17,Scale,1);
		}
		else
		{
			SpriteBtnR(PosX[i]+35,PosY[i]-17,Scale,0);
		}

		if (GlobalController[i]->ButtonHeld&BTN_Z)
		{
			SpriteBtnZ(PosX[i]+17,PosY[i]-17,Scale,1);
		}
		else
		{
			SpriteBtnZ(PosX[i]+17,PosY[i]-17,Scale,0);
		}

		if (FullMenu)
		{
			SpriteBtnDMid(PosX[i]-10,PosY[i],Scale);

			if (GlobalController[i]->ButtonHeld&BTN_DDOWN)
			{
				SpriteBtnDDown(PosX[i]-10,PosY[i]+7,Scale,1);
			}
			else
			{
				SpriteBtnDDown(PosX[i]-10,PosY[i]+7,Scale,0);
			}

			if (GlobalController[i]->ButtonHeld&BTN_DUP)
			{
				SpriteBtnDUp(PosX[i]-10,PosY[i]-7,Scale,1);
			}
			else
			{
				SpriteBtnDUp(PosX[i]-10,PosY[i]-7,Scale,0);
			}

			if (GlobalController[i]->ButtonHeld&BTN_DLEFT)
			{
				SpriteBtnDLeft(PosX[i]-17,PosY[i],Scale,1);
			}
			else
			{
				SpriteBtnDLeft(PosX[i]-17,PosY[i],Scale,0);
			}

			if (GlobalController[i]->ButtonHeld&BTN_DRIGHT)
			{
				SpriteBtnDRight(PosX[i]-3,PosY[i],Scale,1);
			}
			else
			{
				SpriteBtnDRight(PosX[i]-3,PosY[i],Scale,0);
			}

			if (GlobalController[i]->ButtonHeld&BTN_L)
			{
				SpriteBtnL(PosX[i]-10,PosY[i]-17,Scale,1);
			}
			else
			{
				SpriteBtnL(PosX[i]-10,PosY[i]-17,Scale,0);
			}
		}

		SpriteAnalogStick(PosX[i]+4+(GlobalController[i]->AnalogX*0.05f),PosY[i]-10+(GlobalController[i]->AnalogY*-0.05f),Scale,0);
	}
	}
}

Vtx *AllocCourseVerticesDefault(int Size)
{
	VertexMemoryPointer -= Size;
	return (Vtx *)VertexMemoryPointer;
}

void ShowExpansionPakWarningDefault(void)
{
	u16 *fb = (u16 *) 0x801D6000;
	s32 i;

	for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
	{
		fb[i] = 1;
	}
	SetFaultFrameBuffer(fb);
	CrashScreenPrint(40, 80, "OVERKART");
	CrashScreenPrint(40, 100, "EXPANSION PAK REQUIRED");
	osWritebackDCache(fb, SCREEN_WIDTH * SCREEN_HEIGHT * 2);
	osViSwapBuffer(fb);
	osViBlack(0);
	while (1)
	{
	}
}
