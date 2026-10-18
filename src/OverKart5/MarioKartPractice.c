#include "MainInclude.h"
#include "OKInclude.h"

extern OSViContext *__osViCurr;
extern OSViContext *__osViNext;


//PracticeHack Variables//
/*
char GlobalCharA;  //DpadInput
short GlobalCharD;  //Cpad Input
char GlobalCharB;   L/R Button
char MenuOverflow;   A/B Buttons
*/



char SplitTimerToggle, SplitTimerToggleSave = 0x00;
float SaveTimer, SplitTimer = 0;

short SplitStartMarker,SplitEndMarker = 0;
/*
static char itemSwap = 0x00;
static char resetToggle = 0x00;



*/

void SplitFunc()
{
	if (SplitTimerToggle == 0x01)
	{
		if (NearestPathPointByPlayerId[0] < SplitStartMarker)
		{
			GameTimer = 0;
		}
		if (NearestPathPointByPlayerId[0] >= SplitStartMarker)
		{
			SplitTimerToggle = 0x02;
		}
	}
	if (SplitTimerToggle == 0x02)
	{
		if (NearestPathPointByPlayerId[0] >= SplitEndMarker)
		{
			SplitTimer = GameTimer;
			SplitTimerToggle = 0x03;
		}
	}
	if (SplitTimerToggle == 0x03)
	{
		GameTimer = SplitTimer;
	}

}




void ModCheck()
{
	if (SaveGame.RenderSettings.AudioMode != AudioLanguage)
	{
		AudioLanguage = SaveGame.RenderSettings.AudioMode;
		if (AudioLanguage == 0)
		{
			g_MUSRawAudioTable.Pointer[0].Address = *(long*)&ok_USAudio;
			g_MUSRawAudioTable.Pointer[0].Length = 0x24C3F0;

			g_MUSInstrumentTable.Pointer[0].Address = *(long*)(&ok_USAudio + 1);
			g_MUSInstrumentTable.Pointer[0].Length = 0x44D0;

		}
		else
		{
			g_MUSRawAudioTable.Pointer[0].Address = (long)&JP_Audio;
			g_MUSRawAudioTable.Pointer[0].Length = 0x24C580;

			g_MUSInstrumentTable.Pointer[0].Address = (long)&JP_Bank;
			g_MUSInstrumentTable.Pointer[0].Length = 0x4540;
			
		}		
		InitMKCode();
	}

	ScaleXMode = SaveGame.LevelSettings.ScaleXMode;
	ScaleYMode = SaveGame.LevelSettings.ScaleYMode;
	ScaleZMode = SaveGame.LevelSettings.ScaleZMode;

	ScreenFlip = SaveGame.LevelSettings.MirrorX;
	YFLIP = SaveGame.LevelSettings.MirrorY;
	ZFLIP = SaveGame.LevelSettings.MirrorZ;

	if ((SaveGame.GameSettings.AIMode > 0x00) && (GameMode != GAMEMODE_TT))
	{
		AISetup();
	}

	if (SaveGame.RenderSettings.ScreenMode > 0x00)
	{
		if ((ScreenSplitA != 0) && (SaveGame.RenderSettings.SplitMode == 0x00))
		{
			AspectRatio = 3.5;
		}
		else
		{
			AspectRatio = 1.77778;
		}
	}
	else
	{
		if ((ScreenSplitA != 0) && (PlayerCount == 2))
		{
			AspectRatio = 2.6666667;
		}
		else
		{
			AspectRatio = 1.333334;
		}
	}
	
	if (SaveGame.RenderSettings.AliasMode > 0x00)
	{
		__osViCurr->features = 0x00013016;
		__osViNext->features = 0x00013016;
	}
	else
	{
		__osViCurr->features = 0x00003216;
		__osViNext->features = 0x00003216;
	}
	//SaveGame.RenderSettings.SplitMode 3D racers;
	if (SaveGame.RenderSettings.SplitMode > 0x00)
	{
		if (PlayerCount == 0x02)
		{
			ScreenSplitA = 2;
			ScreenSplitB = 2;
		}
	}

	if (SaveGame.GameSettings.ItemMode == IM_NONE)
	{
		ItemSetFlag = 0;
	}
	else
	{
		ItemSetFlag = 1;
	}





}

#define SAVE_STATE_CAMERA_COUNT 4
#define SAVE_STATE_RANK_LEN 10
#define SAVE_STATE_PATH_LEN 12
#define SAVE_STATE_FINISH_LEN 8
#define SAVE_STATE_UNEXPIRED_LEN 8

typedef struct SaveStatePayload {
    Object actors[ACTOR_LIST_SIZE];
    AnmObject objects[OBJECT_LIST_SIZE];
    Player players[NUM_PLAYERS];
    Camera cameras[SAVE_STATE_CAMERA_COUNT];
    Hud hud[HUD_PLAYERS_SIZE];
    s32 lapCount[SAVE_STATE_RANK_LEN];
    s32 vehicle2DPathLength;
    TrainStuff trains[NUM_TRAINS];
    u16 crossingTriggered[NUM_CROSSINGS];
    u16 crossingTimer[NUM_CROSSINGS];
    PaddleBoatStuff paddleBoats[NUM_PADDLE_BOATS];
    VehicleStuff boxTrucks[NUM_RACE_BOX_TRUCKS];
    VehicleStuff schoolBuses[NUM_RACE_SCHOOL_BUSES];
    VehicleStuff tankerTrucks[NUM_RACE_TANKER_TRUCKS];
    VehicleStuff cars[NUM_RACE_CARS];
    s32 vehiclePad[4];
    BombKart bombKarts[NUM_BOMB_KARTS_MAX];
    struct unexpiredActors unexpiredActors[SAVE_STATE_UNEXPIRED_LEN];
    EnemyItem cpuItems[NUM_PLAYERS];
    s32 objectListSize;
    s32 objectIndex1[SOME_OBJECT_INDEX_LIST_SIZE];
    s32 objectIndex2[SOME_OBJECT_INDEX_LIST_SIZE];
    s32 objectIndex3[SOME_OBJECT_INDEX_LIST_SIZE];
    s32 objectIndex4[SOME_OBJECT_INDEX_LIST_SIZE];
    s32 lakituIndex[4];
    s32 itemWindowIndex[4];
    s32 bombKartObjectIndex[NUM_BOMB_KARTS_MAX];
    u16 numActors;
    u16 numPermanentActors;
    u16 nearestPathPoint[SAVE_STATE_PATH_LEN];
    u16 trackSection[SAVE_STATE_PATH_LEN];
    u16 pathIndex[SAVE_STATE_PATH_LEN];
    u16 crossedFinishLine[SAVE_STATE_PATH_LEN];
    u16 wrongDirectionCounter[SAVE_STATE_PATH_LEN];
    u16 isWrongDirection[SAVE_STATE_PATH_LEN];
    s32 pathPointsTraversed[SAVE_STATE_RANK_LEN];
    f32 lapCompletion[SAVE_STATE_RANK_LEN];
    f32 courseCompletion[SAVE_STATE_RANK_LEN];
    s32 raceRank[SAVE_STATE_RANK_LEN];
    s32 previousRaceRank[SAVE_STATE_RANK_LEN];
    s32 raceRankDup[SAVE_STATE_RANK_LEN];
    f32 finishLineTime[SAVE_STATE_FINISH_LEN];
    s16 positionLut[SAVE_STATE_FINISH_LEN];
    s32 starEffectStartTime[NUM_PLAYERS];
    s16 cameraPathPoint[SAVE_STATE_CAMERA_COUNT];
    f32 previousPlayerZ[SAVE_STATE_RANK_LEN];
    SOUKOU_ZURE_TYPE laneData[SAVE_STATE_RANK_LEN];
    f32 courseTimer;
    f32 waterHeight;
    f32 waterVelocity;
    SaveStateReplayCursor replayCursor;
} SaveStatePayload;

typedef char SaveStateFits[(sizeof(SaveStatePayload) <= ok_SaveSize) ? 1 : -1];

static SaveStatePayload *GetSaveStatePayload(void) {
    return (SaveStatePayload *) &ok_SaveState;
}

void SaveState(void)
{
    SaveStatePayload *save = GetSaveStatePayload();

    memcpy(save->actors, SimpleObjectArray, sizeof(save->actors));
    memcpy(save->objects, DynamicObjects, sizeof(save->objects));
    memcpy(save->players, GamePlayers, sizeof(save->players));
    memcpy(save->cameras, Cameras, sizeof(save->cameras));
    memcpy(save->hud, PlayerHUD, sizeof(save->hud));
    memcpy(save->lapCount, LapCountByPlayerId, sizeof(save->lapCount));
    save->vehicle2DPathLength = CourseObstacle;
    memcpy(save->trains, SL, sizeof(save->trains));
    memcpy(save->crossingTriggered, FumikiriMode, sizeof(save->crossingTriggered));
    memcpy(save->crossingTimer, FumikiriCounter, sizeof(save->crossingTimer));
    memcpy(save->paddleBoats, Ship, sizeof(save->paddleBoats));
    memcpy(save->boxTrucks, TRUCK, sizeof(save->boxTrucks));
    memcpy(save->schoolBuses, Bus, sizeof(save->schoolBuses));
    memcpy(save->tankerTrucks, Tanklorry, sizeof(save->tankerTrucks));
    memcpy(save->cars, RvCar, sizeof(save->cars));
    memcpy(save->vehiclePad, D_80163DD8, sizeof(save->vehiclePad));
    memcpy(save->bombKarts, Bom, sizeof(save->bombKarts));
    memcpy(save->unexpiredActors, KameHibasira, sizeof(save->unexpiredActors));
    memcpy(save->cpuItems, EnemyItemTable, sizeof(save->cpuItems));
    save->objectListSize = ObjectListSize;
    memcpy(save->objectIndex1, Objallocptr, sizeof(save->objectIndex1));
    memcpy(save->objectIndex2, Objallocptr2, sizeof(save->objectIndex2));
    memcpy(save->objectIndex3, Objallocptr3, sizeof(save->objectIndex3));
    memcpy(save->objectIndex4, Objallocptr4, sizeof(save->objectIndex4));
    memcpy(save->lakituIndex, Jugemuallocptr, sizeof(save->lakituIndex));
    memcpy(save->itemWindowIndex, ItemBoxAllocPtr, sizeof(save->itemWindowIndex));
    memcpy(save->bombKartObjectIndex, Bomballocptr, sizeof(save->bombKartObjectIndex));
    save->numActors = SimpleObjectCount;
    save->numPermanentActors = StaticObjectCount;
    memcpy(save->nearestPathPoint, NearestPathPointByPlayerId, sizeof(save->nearestPathPoint));
    memcpy(save->trackSection, LakituSurface, sizeof(save->trackSection));
    memcpy(save->pathIndex, CurrentPathID, sizeof(save->pathIndex));
    memcpy(save->crossedFinishLine, RapCountFlg, sizeof(save->crossedFinishLine));
    memcpy(save->wrongDirectionCounter, OgaReverseCount, sizeof(save->wrongDirectionCounter));
    memcpy(save->isWrongDirection, WrongwayFlagPlayer1, sizeof(save->isWrongDirection));
    memcpy(save->pathPointsTraversed, PlayerPathPointTotalTable, sizeof(save->pathPointsTraversed));
    memcpy(save->lapCompletion, RateCountF, sizeof(save->lapCompletion));
    memcpy(save->courseCompletion, RapCountF, sizeof(save->courseCompletion));
    memcpy(save->raceRank, GPCurrentRaceRankByPlayerId, sizeof(save->raceRank));
    memcpy(save->previousRaceRank, OldOgawaRank, sizeof(save->previousRaceRank));
    memcpy(save->raceRankDup, RealRank, sizeof(save->raceRankDup));
    memcpy(save->finishLineTime, TimePlayerLastTouchedFinishLine, sizeof(save->finishLineTime));
    memcpy(save->positionLut, RankBuffer, sizeof(save->positionLut));
    memcpy(save->starEffectStartTime, StarUseCounter, sizeof(save->starEffectStartTime));
    memcpy(save->cameraPathPoint, OgaCamArea, sizeof(save->cameraPathPoint));
    memcpy(save->previousPlayerZ, OgaOldRapChkZ, sizeof(save->previousPlayerZ));
    memcpy(save->laneData, LaneData, sizeof(save->laneData));
    save->courseTimer = GameTimer;
    save->waterHeight = WaterHeight;
    save->waterVelocity = WaterVelocity;
    SaveStateGetReplayCursor(&save->replayCursor);

    SaveTimer = GameTimer;
    SplitTimerToggleSave = SplitTimerToggle;
}

void LoadState(void)
{
    SaveStatePayload *save = GetSaveStatePayload();

    memcpy(SimpleObjectArray, save->actors, sizeof(save->actors));
    memcpy(DynamicObjects, save->objects, sizeof(save->objects));
    memcpy(GamePlayers, save->players, sizeof(save->players));
    memcpy(Cameras, save->cameras, sizeof(save->cameras));
    memcpy(PlayerHUD, save->hud, sizeof(save->hud));
    memcpy(LapCountByPlayerId, save->lapCount, sizeof(save->lapCount));
    CourseObstacle = save->vehicle2DPathLength;
    memcpy(SL, save->trains, sizeof(save->trains));
    memcpy(FumikiriMode, save->crossingTriggered, sizeof(save->crossingTriggered));
    memcpy(FumikiriCounter, save->crossingTimer, sizeof(save->crossingTimer));
    memcpy(Ship, save->paddleBoats, sizeof(save->paddleBoats));
    memcpy(TRUCK, save->boxTrucks, sizeof(save->boxTrucks));
    memcpy(Bus, save->schoolBuses, sizeof(save->schoolBuses));
    memcpy(Tanklorry, save->tankerTrucks, sizeof(save->tankerTrucks));
    memcpy(RvCar, save->cars, sizeof(save->cars));
    memcpy(D_80163DD8, save->vehiclePad, sizeof(save->vehiclePad));
    memcpy(Bom, save->bombKarts, sizeof(save->bombKarts));
    memcpy(KameHibasira, save->unexpiredActors, sizeof(save->unexpiredActors));
    memcpy(EnemyItemTable, save->cpuItems, sizeof(save->cpuItems));
    ObjectListSize = save->objectListSize;
    memcpy(Objallocptr, save->objectIndex1, sizeof(save->objectIndex1));
    memcpy(Objallocptr2, save->objectIndex2, sizeof(save->objectIndex2));
    memcpy(Objallocptr3, save->objectIndex3, sizeof(save->objectIndex3));
    memcpy(Objallocptr4, save->objectIndex4, sizeof(save->objectIndex4));
    memcpy(Jugemuallocptr, save->lakituIndex, sizeof(save->lakituIndex));
    memcpy(ItemBoxAllocPtr, save->itemWindowIndex, sizeof(save->itemWindowIndex));
    memcpy(Bomballocptr, save->bombKartObjectIndex, sizeof(save->bombKartObjectIndex));
    SimpleObjectCount = save->numActors;
    StaticObjectCount = save->numPermanentActors;
    memcpy(NearestPathPointByPlayerId, save->nearestPathPoint, sizeof(save->nearestPathPoint));
    memcpy(LakituSurface, save->trackSection, sizeof(save->trackSection));
    memcpy(CurrentPathID, save->pathIndex, sizeof(save->pathIndex));
    memcpy(RapCountFlg, save->crossedFinishLine, sizeof(save->crossedFinishLine));
    memcpy(OgaReverseCount, save->wrongDirectionCounter, sizeof(save->wrongDirectionCounter));
    memcpy(WrongwayFlagPlayer1, save->isWrongDirection, sizeof(save->isWrongDirection));
    memcpy(PlayerPathPointTotalTable, save->pathPointsTraversed, sizeof(save->pathPointsTraversed));
    memcpy(RateCountF, save->lapCompletion, sizeof(save->lapCompletion));
    memcpy(RapCountF, save->courseCompletion, sizeof(save->courseCompletion));
    memcpy(GPCurrentRaceRankByPlayerId, save->raceRank, sizeof(save->raceRank));
    memcpy(OldOgawaRank, save->previousRaceRank, sizeof(save->previousRaceRank));
    memcpy(RealRank, save->raceRankDup, sizeof(save->raceRankDup));
    memcpy(TimePlayerLastTouchedFinishLine, save->finishLineTime, sizeof(save->finishLineTime));
    memcpy(RankBuffer, save->positionLut, sizeof(save->positionLut));
    memcpy(StarUseCounter, save->starEffectStartTime, sizeof(save->starEffectStartTime));
    memcpy(OgaCamArea, save->cameraPathPoint, sizeof(save->cameraPathPoint));
    memcpy(OgaOldRapChkZ, save->previousPlayerZ, sizeof(save->previousPlayerZ));
    memcpy(LaneData, save->laneData, sizeof(save->laneData));
    GameTimer = save->courseTimer;
    WaterHeight = save->waterHeight;
    WaterVelocity = save->waterVelocity;
    SaveStateSetReplayCursor(&save->replayCursor);

    SaveTimer = save->courseTimer;
    SplitTimerToggle = SplitTimerToggleSave;
}

void DrawInputDisplay()
{
	
	GlobalUShortA = 0x8000;
	GraphPtr = FillRect1ColorF(GraphPtr,
	ControllerInputX, ControllerInputY, ControllerInputX + 43, ControllerInputY + 16, //x1, y1, x2, y2
	0, 0, 0, 128); //r, g, b, a


	{ int i; for (i=0; i<16; i++)
	{
		if(Names[i][0])
		{
			InputColor[0] = InputColors[i][0];
			InputColor[1] = InputColors[i][1];
			InputColor[2] = InputColors[i][2];

			if (Controllers[0].ButtonHeld & GlobalUShortA)
			{
				InputColor[3] = 255;
			}
			else
			{
				InputColor[3] = 64;
			}
			AnalogInput[0] = ControllerInputX + (Coords[i][0] * 6);
			AnalogInput[1] = ControllerInputY + (Coords[i][1] * 5);

			GraphPtr = FillRect1ColorF(GraphPtr,
				AnalogInput[0] + 1 , AnalogInput[1] + 1, AnalogInput[0]+6, AnalogInput[1]+5, //x1, y1, x2, y2
				InputColor[0], InputColor[1], InputColor[2], InputColor[3]); //r, g, b, a

		}
			GlobalUShortA >>= 1;
	} }
	{ int i; for (i=0; i<4; i++)
	{
		if(StickNames[i][0])
		{
			InputColor[0] = StickColor[i][0];
			InputColor[1] = StickColor[i][1];
			InputColor[2] = StickColor[i][2];

			if (Controllers[0].ButtonHeld & GlobalUShortA)
			{
				InputColor[3] = 255;
			}
			else
			{
				InputColor[3] = 32;
			}
			AnalogInput[0] = ControllerInputX + (StickCoord[i][0] * 6);
			AnalogInput[1] = ControllerInputY + (StickCoord[i][1] * 5);

			GraphPtr = FillRect1ColorF(GraphPtr,
				AnalogInput[0] + 1 , AnalogInput[1] + 1, AnalogInput[0]+6, AnalogInput[1]+5, //x1, y1, x2, y2
				InputColor[0], InputColor[1], InputColor[2], InputColor[3]); //r, g, b, a

		}
		GlobalUShortA >>= 1;
	} }


	InputColor[0] = StickColor[4][0];
	InputColor[1] = StickColor[4][1];
	InputColor[2] = StickColor[4][2];
	InputColor[3] = 255;
	
	AnalogInput[0] = ControllerInputX + ((StickCoord[4][0] * 6) + (Controllers[0].AnalogX / 127.0f * 6));
	AnalogInput[1] = ControllerInputY + ((StickCoord[4][1] * 5) + (Controllers[0].AnalogY / -127.0f * 5));

	GraphPtr = FillRect1ColorF(GraphPtr,
		AnalogInput[0] + 1 , AnalogInput[1] + 1, AnalogInput[0]+6, AnalogInput[1]+5, //x1, y1, x2, y2
		InputColor[0], InputColor[1], InputColor[2], InputColor[3]); //r, g, b, a

}



void RotateCamera(float inputAngle)
{
	
	FlyCamDirection -= (inputAngle / 360.0) * 65535.0;
	FlyCamRadian = (((float)FlyCamDirection / 65535) * 360) * 0.0174533;

	/*
	FlyCamLook[0][0] = GlobalCamera[0]->LookatPos[0] - GlobalCamera[0]->CameraPos[0];
	FlyCamLook[1][0] = GlobalCamera[0]->LookatPos[2] - GlobalCamera[0]->CameraPos[2];

	FlyCamLook[0][1] = FlyCamLook[0][0] * cosF(FlyCamRadian) - FlyCamLook[1][0] * sinF(FlyCamRadian);
	FlyCamLook[1][1] = FlyCamLook[0][0] * sinF(FlyCamRadian) + FlyCamLook[1][0] * cosF(FlyCamRadian);

	GlobalCamera[0]->LookatPos[0] = GlobalCamera[0]->CameraPos[0] + FlyCamLook[0][1];
	GlobalCamera[0]->LookatPos[2] = GlobalCamera[0]->CameraPos[2] + FlyCamLook[1][1] + 0.01;
	*/
	GlobalCamera[0]->LookatPos[0] = (float)(GlobalCamera[0]->CameraPos[0] + 80 * sinF(FlyCamRadian));
	GlobalCamera[0]->LookatPos[2] = (float)(GlobalCamera[0]->CameraPos[2] + 80 * cosF(FlyCamRadian));

	
}


void MoveCamera(int inputDistance)
{
	FlyCamRadian = (((float)FlyCamDirection / 65535) * 360) * 0.0174533;

	GlobalCamera[0]->CameraPos[0] = (float)(GlobalCamera[0]->CameraPos[0] + inputDistance * sinF(FlyCamRadian));
	GlobalCamera[0]->CameraPos[2] = (float)(GlobalCamera[0]->CameraPos[2] + inputDistance * cosF(FlyCamRadian));

	GlobalCamera[0]->LookatPos[0] = (float)(GlobalCamera[0]->LookatPos[0] + inputDistance * sinF(FlyCamRadian));
	GlobalCamera[0]->LookatPos[2] = (float)(GlobalCamera[0]->LookatPos[2] + inputDistance * cosF(FlyCamRadian));
}




void StrafeCamera(int inputDistance)
{
	FlyCamRadian = ((((float)FlyCamDirection / 65535) * 360) + 90) * 0.0174533;

	GlobalCamera[0]->CameraPos[0] = (float)(GlobalCamera[0]->CameraPos[0] + inputDistance * sinF(FlyCamRadian));
	GlobalCamera[0]->CameraPos[2] = (float)(GlobalCamera[0]->CameraPos[2] + inputDistance * cosF(FlyCamRadian));

	GlobalCamera[0]->LookatPos[0] = (float)(GlobalCamera[0]->LookatPos[0] + inputDistance * sinF(FlyCamRadian));
	GlobalCamera[0]->LookatPos[2] = (float)(GlobalCamera[0]->LookatPos[2] + inputDistance * cosF(FlyCamRadian));
}
void MoveCameraTilt(int inputDistance, int tilt)
{
	FlyCamRadian = (((float)FlyCamDirection / 65535) * 360) * 0.0174533 + tilt;
	if (tilt > 0)
	{
		if (FlyCamRadian > 360)
		FlyCamRadian -= 360;
	}
	else
	{
		if (FlyCamRadian < 0)
		FlyCamRadian += 360;
	}


	FlyCamRadian = (int)FlyCamRadian * -0.0174533;
	GlobalCamera[0]->LookatPos[0] = (float)(GlobalCamera[0]->LookatPos[0] + inputDistance * sinF(FlyCamRadian));
	GlobalCamera[0]->LookatPos[2] = (float)(GlobalCamera[0]->LookatPos[2] + inputDistance * cosF(FlyCamRadian));
}

//SaveGame.ModSettings.PracticeMode is > 0 then
void PracticeHack()
{
	
	GlobalCharA = (char)((Controllers[0].ButtonHeld >> 8) & 0x0F);  // D-pad
	GlobalCharB = (char)((Controllers[0].ButtonHeld & 0xF0) >> 4);  // L/R
	GlobalCharC = (char)((Controllers[0].ButtonHeld >> 12) & 0x0F);  // A/B
	GlobalCharD = (char)(Controllers[0].ButtonHeld & 0x0F);  // C-pad
	if (SaveGame.GameSettings.GameType == 3)
	{
		
		if (SplitTimerToggle > 0x00)
		{
			SplitFunc();

		}
		if (GlobalCharD == 0x02)
		{
			GlobalPlayer[0].Position[1] = GlobalPlayer[0].Position[1] + 3;
			GlobalPlayer[0].Velocity[1] = 0;
			GraphPtr = FillRect1ColorF(GraphPtr, 23, 43, 120, 53, 0, 0,0, 175);
			LoadFont();
			PrintString(5,25, "FLYING");
		}
		switch(GlobalController[0]->ButtonPressed)
		{
			case(BTN_DLEFT):
			{
				SaveStateCourseID = ((HotSwapID * 0x10) + CourseID + 1);
				SaveState();
				break;
			}
			case(BTN_DRIGHT):
			{
				if (SaveStateCourseID == ((HotSwapID * 0x10) + CourseID + 1))
				{
					LoadState();
				}
				break;
			}
			case(BTN_DUP):
			{
				g_gameLapPlayer1 = 0x00;
				GlobalHud[0]->Goalsw = 0x00;
				GlobalHud[0]->Lapcnt = 0x00;				
				GlobalHud[0]->Item = 0;
				GlobalHud[0]->SPItem = 0;
				GameTimer = 90;
				GlobalPlayer[0].Item = 0;
				OKForceRoulette = 1;
				RouletteStart(0,14);
				OKForceRoulette = 0;
				break;
			}
			case(BTN_DDOWN):
			{
				{
				short* PositionArray;
				PositionArray = (short*)GetRealAddress((int) PathTable[CourseID][0]);
				GlobalPlayer[0].Position[0] = (float)PositionArray[0];
				GlobalPlayer[0].Position[1] = (float)PositionArray[1];
				GlobalPlayer[0].Position[2] = (float)PositionArray[2];
				GlobalPlayer[0].Direction[1] = 0x8000;
				}
				break;
			}
	
		}
	}
	


	//devmode

	if (MapModeCheck == 1)
	{
		PrintMap(MiniMapMode);

		if (StartingIndicator >= 3)
		{
			g_GameLapTable[0] = 1;
			{ int ThisPlayer; for (ThisPlayer = 1; ThisPlayer < 8; ThisPlayer++)
			{
				g_GameLapTable[ThisPlayer] = 0;
			} }
		}
		

		if ((GlobalController[0]->ButtonHeld & BTN_R) == BTN_R)
		{
			if ((GlobalController[0]->ButtonPressed & BTN_DDOWN) == BTN_DDOWN)
			{
				if (MiniMapMode < 3)
				{
					MiniMapMode++;
				}
			}
			if ((GlobalController[0]->ButtonPressed & BTN_DUP) == BTN_DUP)
			{
				if (MiniMapMode > 0)
				{
					MiniMapMode--;
				}
			}
		}
		else
		{
			switch(MiniMapMode)
			{
				case 0:
				{
					//Map

					switch(GlobalController[0]->ButtonHeld)
					{

						case BTN_DRIGHT :
						{
							MapX[0]++;
							break;
						}


						case BTN_DLEFT :
						{

							MapX[0]--;
							break;
						}

						case BTN_DUP :
						{
							MapY[0]--;
							break;
						}

						case BTN_DDOWN :
						{
							MapY[0]++;
							break;
						}
					}

					break;
				}



				case 1:
				{
					//Start
					switch(GlobalController[0]->ButtonHeld)
					{

						case BTN_DRIGHT :
						{
							StartX++;
							break;
						}


						case BTN_DLEFT :
						{

							StartX--;
							break;
						}

						case BTN_DUP :
						{
							StartY--;
							break;
						}

						case BTN_DDOWN :
						{
							StartY++;
							break;
						}
					}


					break;

				}



				case 2:
				{
					//Scale
					switch(GlobalController[0]->ButtonHeld)
					{

						case BTN_DRIGHT :
						{
							MapScale = MapScale + .001;
							break;
						}


						case BTN_DLEFT :
						{

							MapScale = MapScale - .001;
							break;
						}

						case BTN_DUP :
						{
							MapScale = MapScale + .0001;
							break;
						}

						case BTN_DDOWN :
						{
							MapScale = MapScale - .0001;
							break;
						}
					}

					break;
				}

				case 3:
				{
					//line
					switch(GlobalController[0]->ButtonHeld)
					{

						case BTN_DRIGHT :
						{
							RadarLineX++;
							break;
						}


						case BTN_DLEFT :
						{

							RadarLineX--;
							break;
						}

						case BTN_DUP :
						{
							RadarLineY++;
							break;
						}

						case BTN_DDOWN :
						{
							RadarLineY--;
							break;
						}
					}


					break;

				}

			}
		}
	}

}


void RunFlyCam()
{
	if ((GlobalController[0]->ButtonPressed & BTN_CLEFT) == BTN_CLEFT)
	{
		FlyCamDirection = GlobalCamera[0]->CameraDirection[1];
		FlyCamSectionCheck = GlobalScreen[0]->CameraPoint;
		FlyCamViewCheck = GlobalScreen[0]->Flag;
		if (FlyCamToggle == 0)
		{
			s16 *hudCoords;
			s16 *camBuf;
			s16 *Backup;
			int i;

			FlyCamToggle = 1;
			GlobalFloatC = GlobalPlayer[0].Offsetsize;
			GlobalPlayer[0].Offsetsize = 0.001;

			hudCoords = &GlobalHud[0]->my;
			camBuf = (s16 *) &ok_FreeCam;
			Backup = (s16 *) &ok_FreeCamBackup;
			for (i = 0; i < 16; i++) {
				Backup[i] = hudCoords[i];
				hudCoords[i] = camBuf[i];
			}
			FlyCamBackupPosition[0] = GlobalPlayer[0].Position[0];
			FlyCamBackupPosition[1] = GlobalPlayer[0].Position[1];
			FlyCamBackupPosition[2] = GlobalPlayer[0].Position[2];
			
		}
		else
		{
			s16 *hudCoords;
			s16 *Backup;
			int i;

			FlyCamToggle = 0;
			GlobalPlayer[0].Offsetsize = GlobalFloatC;
			hudCoords = &GlobalHud[0]->my;
			Backup = (s16 *) &ok_FreeCamBackup;
			for (i = 0; i < 16; i++) {
				hudCoords[i] = Backup[i];
			}
		}
		
	}

	if (FlyCamToggle > 0)
	{
		GlobalScreen[0]->Posx = 160;
		GlobalScreen[0]->Posy = 120;
		GlobalScreen[0]->Width = 320;
		GlobalScreen[0]->Height = 240;
		GlobalScreen[0]->ScreenAspect = 320.0f/ 240.0f;

		if ((GlobalController[0]->ButtonHeld & BTN_L) == BTN_L)
		{
			LoadFont();
			PrintStringNumber(5,25, "SECTION-", FlyCamSectionCheck);
			PrintStringNumber(5,35, "VIEW-", FlyCamViewCheck);
			PrintStringNumber(5,45, "SPEED-", FlyCamSpeed);
			PrintFloat(0,75,(float)(FlyCamSpeed * (float)((float)GlobalController[0]->AnalogY / 100)));
			
			switch(GlobalController[0]->ButtonPressed)
			{
				
				case BTN_DDOWN:
				{
					if (FlyCamSpeed > 5)
					{
						FlyCamSpeed = FlyCamSpeed - 5;
					}
					else
					{
						if (FlyCamSpeed > 1)
						{
							FlyCamSpeed = FlyCamSpeed - 1;
						}
					}
					break;
				}

				case BTN_DUP:
				{
					if (FlyCamSpeed < 5)
					{
						FlyCamSpeed = FlyCamSpeed + 1;
					}
					else
					{							
						FlyCamSpeed = FlyCamSpeed + 5;							
					}
					break;
				}
			}

		}	
		else
		{	
			GlobalPlayer[0].Position[0] = FlyCamBackupPosition[0];
			GlobalPlayer[0].Position[1] = FlyCamBackupPosition[1];
			GlobalPlayer[0].Position[2] = FlyCamBackupPosition[2];
			switch(GlobalController[0]->ButtonPressed)
			{
				case BTN_DLEFT:
				{
					if (FlyCamSectionCheck > 1)
					{
						FlyCamSectionCheck--;
					}
					break;
				}
				case BTN_DRIGHT:
				{
					FlyCamSectionCheck++;
					break;
				}
			
			}
			switch(GlobalController[0]->ButtonHeld)
			{
				case BTN_DUP:
				{
					GlobalCamera[0]->LookatPos[1] += (FlyCamSpeed);
					break;
				}
				case BTN_DDOWN:
				{
					GlobalCamera[0]->LookatPos[1] -= (FlyCamSpeed);
					break;
				}
			}
			if ((GlobalController[0]->AnalogHeld & BTN_DRIGHT) == BTN_DRIGHT)
			{
				RotateCamera((float)(FlyCamSpeed * 0.15 * (float)((float)GlobalController[0]->AnalogX / 100)));
			}
			else if ((GlobalController[0]->AnalogHeld & BTN_DLEFT) == BTN_DLEFT)
			{
				RotateCamera((float)(FlyCamSpeed * 0.15 * (float)((float)GlobalController[0]->AnalogX / 100)));
			}
			
			if (FlyCamPilot == 1)
			{
				MoveCamera(FlyCamSpeed);
			}
			else
			{
				if ((GlobalController[0]->ButtonHeld & BTN_A) == BTN_A)
				{
					MoveCamera(2 * FlyCamSpeed);
				}
				if ((GlobalController[0]->ButtonHeld & BTN_B) == BTN_B)
				{
					MoveCamera(-2 * FlyCamSpeed);
				}
                if ((GlobalController[0]->ButtonHeld & BTN_R) == BTN_R)
				{
					StrafeCamera(2 * FlyCamSpeed);
				}
			}				
			if (((GlobalController[0]->AnalogHeld & BTN_DDOWN) == BTN_DDOWN) || ((GlobalController[0]->AnalogHeld & BTN_DUP) == BTN_DUP))
			{					
				GlobalCamera[0]->LookatPos[1] += (float)(FlyCamSpeed * 0.4 * (float)((float)GlobalController[0]->AnalogY / 100));
				GlobalCamera[0]->CameraPos[1] += (float)(FlyCamSpeed * 0.4 * (float)((float)GlobalController[0]->AnalogY / 100));
			}

		}
	}

	if(ScreenFlip == 1)
	{
		GlobalUShortA=(Ushort)FlyCamDirection;
		if(GlobalUShortA< DEG1 * 45)     FlyCamViewCheck=2;          
		else if(GlobalUShortA<DEG1 * 135)     FlyCamViewCheck=3;          
		else if(GlobalUShortA<DEG1 * 225)     FlyCamViewCheck=0;          
		else if(GlobalUShortA<DEG1 * 315)     FlyCamViewCheck=1;          
		else      FlyCamViewCheck=2;          
	}
	else
	{
		GlobalUShortA=(Ushort)FlyCamDirection;
		if(GlobalUShortA<DEG1 * 45)     FlyCamViewCheck=2;          
		else if(GlobalUShortA<DEG1 * 135)     FlyCamViewCheck=1;          
		else if(GlobalUShortA<DEG1 * 225)     FlyCamViewCheck=0;          
		else if(GlobalUShortA<DEG1 * 315)     FlyCamViewCheck=3;          
		else      FlyCamViewCheck=2;          
	}

	FlyCamSection = ((FlyCamSectionCheck - 1) * 4) + FlyCamViewCheck;
		

	
	



}

void FlyCamInit()
{
	{ int Pass; for (Pass = 0; Pass < 20; Pass++)
	{
		*(Uint*)(&ok_FreeCam + Pass) = 0xFF00FF00;
	} }
}