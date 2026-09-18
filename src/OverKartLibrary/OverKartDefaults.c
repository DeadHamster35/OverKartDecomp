#include "MainInclude.h"
#include "save.h"
#include "replays.h"

void enemy_tekagen_sub(s32 playerId, Player* player);
void RubberBandMan(s32 playerId, Player* player);

/*
 * Hook points for projects that include OverKart Library.
 * Keep stock fall-through here until a host project supplies its own.
 */

void gameCodeDefault(void)
{
	gameCode();
}

void titleMenuDefault(void)
{
	titleMenu();
}

void allRunDefault(void)
{
	allRun();
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

void BalloonCheckDefault(Player* player, f32 arg1, f32 arg2, s8 playerIndex, s8 balloonIndex, s16 rotation)
{
	BalloonDisp(player, arg1, arg2, playerIndex, balloonIndex, rotation);
}

void ItemboxCollideCheckDefault(Player* Car, Object* Target)
{
	ItemboxCollideCheck(Car, Target);
}

void KWKumo_Alloc_Hook_Default(void)
{
	if (HotSwapID > 0)
	{
		LoadCustomHeader(courseValue + gpCourseIndex);

		SetCloudType((char) OverKartHeader.SkyType);
		SetWeatherType((char) OverKartHeader.WeatherType);
		GlobalShortA = g_courseID;
		g_courseID = CloudCourseID;
		KWKumo_Alloc();
		g_courseID = GlobalShortA;
	}
	else
	{
		KWKumo_Alloc();
	}
}

void KWChart_Kumo_Hook_Default(int screen_num)
{
	AnmObject* cloud;
	int cloudcount;

	if (HotSwapID > 0)
	{
		GlobalShortA = g_courseID;
		g_courseID = CloudCourseID;
		KWChart_Kumo(screen_num);
		g_courseID = GlobalShortA;
	}
	else
	{
		KWChart_Kumo(screen_num);
	}

	if ((char) OverKartHeader.WeatherType == 0)
	{
		return;
	}

	cloudcount = CloudCount1P;
	if (g_playerCount == 2)
	{
		cloudcount = CloudCount2P;
	}

	{
		int i;
		for (i = 0; i < cloudcount; i++)
		{
			if (g_CloudAllocate[i] == 0)
			{
				return;
			}
			cloud = (AnmObject*) &g_DynamicObjects[g_CloudAllocate[i]];
			cloud->velocity[1] = -5.0f;
			cloud->scale = 0.4;
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
	g_courseFaceMaxIndex = 0;
	if (HotSwapID > 0)
	{
		if (g_gameMode != GAMEMODE_BATTLE)
		{
			g_courseID = 0;
		}
		else
		{
			g_courseID = 15;
		}
		InputID = g_courseID;
		g_loadedcourseFlag = g_courseID;
	}
	LoadCustomHeader(courseValue + gpCourseIndex);
	SetCustomData();
	setSky();
	setWater();
	LoadMapData(InputID);
	LoadIceKage();
	CheckCourseMemoryHighWater();

	if (HotSwapID > 0)
	{
		loadTextureScrollTranslucent();
		runKillDisplayObjects();
		loadMinimap();
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
	if (g_gameMode == GAMEMODE_BATTLE)
	{
		GlobalShortA = 4;
	}
	else
	{
		GlobalShortA = 1;
	}
	if (menuScreenA == GlobalShortA)
	{
		if ((GlobalController[4]->ButtonPressed & BTN_CLEFT) == BTN_CLEFT)
		{
			swapHS(0);
		}
		else if ((GlobalController[4]->ButtonPressed & BTN_CRIGHT) == BTN_CRIGHT)
		{
			swapHS(1);
		}
		LoadCustomHeader(courseValue);

		if ((GlobalController[4]->ButtonPressed & BTN_R) == BTN_R)
		{
			MenuToggle = !MenuToggle;
		}
	}

	*(int*) (&PlayerOK) = 0;
	if (HotSwapID > 0)
	{
		courseValue = -1;
		gpCourseIndex = 0;
		switch (g_gameMode)
		{
			case 0:
			{
				courseValue = (g_cupSelect * 4);
				break;
			}
			case 1:
			case 2:
			case 3:
			{
				courseValue = (g_cupSelect * 4) + g_courseSelect;
				break;
			}
		}
	}
	else
	{
		LoadCustomHeader(-1);
	}
}

void check_romghostDefault(void)
{
	if (HotSwapID == 0)
	{
		check_romghost();
		return;
	}

	romghost_status = 1;
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

void RubberBandManDefault(s32 playerId, Player* player)
{
	RubberBandMan(playerId, player);
}
