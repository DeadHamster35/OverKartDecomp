#include "MainInclude.h"
#include "save.h"
#include "replays.h"

void EnemyTekagenSub(s32 PlayerId, Player* Player);
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
	LoadMapData(InputID);
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

	*(int*) (&PlayerOK) = 0;
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

void EventDisplayAfterHookDefault(int Player)
{
	(void) Player;
}
