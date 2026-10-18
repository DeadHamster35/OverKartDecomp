#include <ultra64.h>
#include <string.h>
#include <defines.h>
#include <objects.h>
#include <actor_types.h>
#include <vehicles.h>
#include <bomb_kart.h>
#include <path.h>
#include "main.h"
#include "camera.h"
#include "cpu_vehicles_camera_path.h"
#include "code_80057C60.h"
#include "code_800029B0.h"
#include "effects.h"
#include "race_logic.h"
#include "replays.h"

/* Must match ok_SaveSize in OKHeader.h. Do not include OKHeader.h here. */
#define ok_SaveSize 0x2C000

extern int ok_SaveState;
extern char SplitTimerToggle;
extern char SplitTimerToggleSave;
extern float SaveTimer;

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
