#include "MainInclude.h"
#include "save.h"
#include "replays.h"

void enemy_tekagen_sub(s32 playerId, Player* player);
void RubberBandMan(s32 playerId, Player* player);

#define OK_DBG_BASE 0x80550000

void OkDbgLoad(u32 stage, s32 courseId, u32 extra0, u32 extra1, const char *name)
{
	volatile u32 *d;
	volatile u32 *row0;
	volatile u32 *row;
	volatile char *fn;
	s32 rowN;
	int i;

	d = (volatile u32 *) OK_DBG_BASE;
	d[1] = stage;

	fn = (volatile char *) (OK_DBG_BASE + 0x48);
	if (name != NULL)
	{
		for (i = 0; i < 15; i++)
		{
			if (name[i] == 0)
			{
				break;
			}
			fn[i] = name[i];
		}
	}
	else
	{
		i = 0;
	}
	for (; i < 16; i++)
	{
		fn[i] = 0;
	}

	rowN = courseId;
	if ((rowN < 0) || (rowN >= 20))
	{
		rowN = 0;
	}

	/* GameOffsets.h types g_courseTable as long; CourseTable rows are 0x30. */
	row0 = (volatile u32 *) &g_courseTable;
	row = row0 + (rowN * 12);

	d[0] = 0x4F4B4442;
	d[2] = (u32) (s32) HotSwapID;
	d[3] = (u32) (s32) g_courseID;
	d[4] = (u32) (s32) g_loadedcourseFlag;
	d[5] = (u32) (s32) courseValue;
	d[6] = (u32) (s32) gpCourseIndex;
	d[7] = (u32) courseId;
	d[8] = row0[0];
	d[9] = row0[2];
	d[10] = row0[6];
	d[11] = row[0];
	d[12] = row[2];
	d[13] = row[6];
	d[14] = row[7];
	d[15] = (u32) (s32) g_gameMode;
	d[16] = extra0;
	d[17] = extra1;
}

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
	OkDbgLoad(2, InputID, 0, 0, "MapStartup");
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
	OkDbgLoad(3, InputID, 0, 0, "LoadHeader");
	LoadCustomHeader(courseValue + gpCourseIndex);
	OkDbgLoad(4, InputID, 0, 0, "SetCustData");
	SetCustomData();
	OkDbgLoad(5, InputID, 0, 0, "setSky");
	setSky();
	OkDbgLoad(6, InputID, 0, 0, "setWater");
	setWater();
	OkDbgLoad(7, InputID, 0, 0, "load_course");
	LoadMapData(InputID);
	OkDbgLoad(18, InputID, 0, 0, "LoadIceKage");
	LoadIceKage();
	OkDbgLoad(19, InputID, 0, 0, "CheckHighW");
	CheckCourseMemoryHighWater();

	if (HotSwapID > 0)
	{
		OkDbgLoad(20, InputID, 0, 0, "texScroll");
		loadTextureScrollTranslucent();
		OkDbgLoad(21, InputID, 0, 0, "KillDisp");
		runKillDisplayObjects();
		OkDbgLoad(22, InputID, 0, 0, "loadMinimap");
		loadMinimap();
	}
	OkDbgLoad(23, InputID, 0, 0, "MapDone");
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
