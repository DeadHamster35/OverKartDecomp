#include "MainInclude.h"

Uint GlobalAddressA, GlobalAddressB, GlobalAddressC, GlobalAddressD;
int GlobalIntA, GlobalIntB, GlobalIntC, GlobalIntD;
Int64     GlobalInt64;
Uint64    GlobalUInt64;
Uint GlobalUIntA, GlobalUIntB;

float GlobalFloatA, GlobalFloatB, GlobalFloatC, GlobalFloatD;
char GlobalCharA,GlobalCharB,GlobalCharC,GlobalCharD,GlobalCharE,GlobalFrameChar;

short GlobalShortA, GlobalShortB, GlobalShortC, GlobalShortD;
unsigned short GlobalUShortA, GlobalUShortB;
bool GlobalBoolA, GlobalBoolB, GlobalBoolC, GlobalBoolD;
short GlobalShortA, GlobalShortB;
bool GlobalBoolA, GlobalBoolB, GlobalBoolC, GlobalBoolD;
int LoopValue;
Uint GlobalFrameCount;
Uint ClockCycle[2], OldCycle[2];
Uint CycleCount[2];

bool TempoBool, StopSwop;
char ConsolePlatform, EmulatorPlatform;
Uint CartridgeStatus;

bool SurfaceExplorerMode = false;
bool CustomWaterHeight[8];
short EchoStart = 0x19B;
short EchoStop = 0x1B9;
short CloudCourseID, Snow3DCourseID;
long g_SnowParticleTex[36];
float g_skySnowScale = 0.15f;
float g_skySnowVelocity = 0.75f;
long g_3DSnowSpawnHeight = 0x2D;
long g_3DSnowSpawnDistanceMin = 0x1E;
long g_3DSnowSpawnDistanceMax = 0x12C;
long g_3DSnowSpawnCone = 0x3000;
float g_3DSnowSwayMovement = 1.0f;
float g_3DSnowScale = 0.15f;
float g_3DSnowVelocityUpLim = 1.035f;
float g_3DSnowVelocityLowLim = -0.3f;
s32 OKLakituIce = 0;
s32 OKCheckSplashEnabled = 1;
s32 OKForceRoulette = 0;
short AsmCupCount = 3;
s8 OKPSelSkipCursor[4] = { 0, 0, 0, 0 };
s8 OKPSelSkipPortrait[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
s32 OKPSelSkipAnime = 0;
s32 OKMusicIn3P4P = 0;
char FlyCamToggle = 0;
short FlyCamSection = 0;
char CloudCourseValue, Snow3DCourseValue, WeatherCourseValue, Toggle3DSnow;
short BattleGametype;

short MenuPosition [2];
Ushort PerspectiveValue;

int WholeNumber = 0;
int DecimalNumber = 0;
int PrintOffsetA, PrintOffsetB, PrintOffsetC, PrintOffsetD = 0;
int Minutes = 0;
int Seconds = 0;
Object *GlobalObjectA, *GlobalObjectB;
float ZeroVector[3] = {0.0, 0.0, 0.0};
OKRAMHeader OverKartRAMHeader;

short AnimationTimer = 0;
short DynFPSModifier = 2;
char OKStaticTempoOnly = 1;

Controller *GlobalController[8] = 
{
     &g_Controller1,
     &g_Controller2,
     &g_Controller3,
     &g_Controller4,
     &g_ControllerMenu,
     &g_ControllerGhost1,
     &g_ControllerGhost2,
     &g_ControllerGhost3,
};

Screen *GlobalScreen[4] = 
{
     &g_Screen1,
     &g_Screen2,
     &g_Screen3,
     &g_Screen4
};

Camera *GlobalCamera[4] = 
{
     &g_Camera1,
     &g_Camera2,
     &g_Camera3,
     &g_Camera4
};

u16 *GlobalPath[8] =
{
     &NearestPathPointByPlayerId[0],
     &NearestPathPointByPlayerId[1],
     &NearestPathPointByPlayerId[2],
     &NearestPathPointByPlayerId[3],
     &NearestPathPointByPlayerId[4],
     &NearestPathPointByPlayerId[5],
     &NearestPathPointByPlayerId[6],
     &NearestPathPointByPlayerId[7],
};

s32 *GlobalLap[8] =
{
     &LapCountByPlayerId[0],
     &LapCountByPlayerId[1],
     &LapCountByPlayerId[2],
     &LapCountByPlayerId[3],
     &LapCountByPlayerId[4],
     &LapCountByPlayerId[5],
     &LapCountByPlayerId[6],
     &LapCountByPlayerId[7],
};

Hud *GlobalHud[4] =
{
     &g_hudPlayer1,
     &g_hudPlayer2,
     &g_hudPlayer3,
     &g_hudPlayer4,
};

char *GlobalLightning[8] =
{
     (char *) &LightningFlagPlayer1[0],
     (char *) &LightningFlagPlayer1[1],
     (char *) &LightningFlagPlayer1[2],
     (char *) &LightningFlagPlayer1[3],
     (char *) &LightningFlagPlayer1[4],
     (char *) &LightningFlagPlayer1[5],
     (char *) &LightningFlagPlayer1[6],
     (char *) &LightningFlagPlayer1[7],
};


Playercolor *GlobalPlayercolor[8] =
{
     (Playercolor *) &ColorPlayer0R[0],
     (Playercolor *) &ColorPlayer0R[1],
     (Playercolor *) &ColorPlayer0R[2],
     (Playercolor *) &ColorPlayer0R[3],
     (Playercolor *) &ColorPlayer0R[4],
     (Playercolor *) &ColorPlayer0R[5],
     (Playercolor *) &ColorPlayer0R[6],
     (Playercolor *) &ColorPlayer0R[7],
};

//MarioKart3D Variables
short CoinCount[8] = {0,0,0,0,0,0,0,0};
short IFrames[8] = {0,0,0,0,0,0,0,0};
float AffineMatrix[4][4];
float AffineMatrix2[4][4];
float ObjectPosition[3] = {0,0,0};
Vector ObjectVector[5];
float ObjectVelocity[3] = {0,0,0};
short ObjectAngle[3] = {0,0,0};
short ObjectIndex;
//
//

struct OKEngine EngineClass[3][3];


/*
#define ENGINE_BALANCE	0
#define ENGINE_ACCEL	1
#define ENGINE_SPEED	2
*/
short EngineSpeed[4][3] = { {292,288,296},{312,308,316},{322,318,326},{245,245,245}};
short AccelerationCurve[3][10] = {{20,20,20,16,14,12,10,8,6,4},{20,20,25,26,26,20,15,8,8,8},{20,20,20,16,10,10,10,18,18,12}};
short SteerAngle[3] = {135,125,110};
short SteerValue[3] = {2,0,-2};

short PowerBand[3] = {25, 30, 15};
short EnginePowerDownRT[3][15] = {{0,0,0,3,0,0,0,9,9,0,0,9,0,9,9}, {0,0,0,2,0,0,0,8,8,0,0,8,0,8,8}, {0,0,0,4,0,0,0,10,10,0,0,10,0,10,10}};
short EnginePowerDownFT[3][15] = {{0,0,0,0,0,0,0,3,3,0,0,3,0,3,3}, {0,0,0,0,0,0,0,2,2,0,0,2,0,2,2}, {0,0,0,0,0,0,0,04,04,0,0,04,0,04,04}};


char MenuBackup = 0, MenuChanged = 0, MenuTimer = 0, StatsID = -1;
bool MapIsCurrentlyLoading = false;
char MenuFlash[4] = {0,0,0,0};
char MenuProgress[4] = {0,0,0,0};
char PlayerCharacterSelect[4] = {0,1,2,3};
char PlayerEngineSelect[4] = {0,0,0,0};
char PlayerSteerSelect[4] = {0,0,0,0};
char PlayerShowStats[4] = {0,0,0,0};
struct PlayerTextureTable BackupNamePlateTable;
struct PlayerTextureTable BackupPortraitTable[9];

//Mario Kart Stats



//SharedFunctions
long DataLength = 0; //
int *TargetAddress = &ok_Target;
int *SourceAddress = &ok_Source;
int *TempPointer = &ok_Pointer;
long *GraphPointer = (long*)&GraphPtrOffset;
int *TkmPoint = &ok_TKMSpace;
int RSPNumber;
int RSPOffset;
char *Hex = "0123456789ABCDEF";

Ushort RedTextPalette[4];
Ushort BlueTextPalette[4];
Ushort GreenTextPalette[4];
Ushort WhiteTextPalette[4];

char RedPaletteF3D[0xC0];
char BluePaletteF3D[0xC0];
char GreenPaletteF3D[0xC0];
char WhitePaletteF3D[0xC0];
     
//

//Stock Names
char *StockCourseNames[] = {"Mario Raceway", "Choco Mountain", "Bowser's Castle", "Banshee Boardwalk",
                         "Yoshi Valley", "Frappe Snowland", "Koopa Troopa Beach", "Royal Raceway",
                         "Luigi Raceway", "Moo Moo Farm", "Toad's Turnpike","Kalimari Desert",
                         "Sherbet Land", "Rainbow Road", "Wario Stadium", "Block Fort",
                         "Skyscraper", "Double Deck", "D.K.'s Jungle Parkway", "Big Donut"};
char *StockCharacterNames[] = {"MARIO", "LUIGI", "YOSHI", "TOAD", "D.K.", "WARIO", "PEACH", "BOWSER"};
BalloonColor StockBalloonColors[] = 
{
     {0x00, 0xC8, 0x01, 0x00}, {0x00, 0x00, 0x70, 0x01}, {0x00, 0x10, 0x79, 0x51}, {0x00, 0x00, 0x59, 0x70}, 
     {0x00, 0x70, 0x55, 0x00}, {0x00, 0x7A, 0x7E, 0x00}, {0x00, 0x77, 0x2C, 0x24}, {0x00, 0x30, 0x14, 0x58}
};

BalloonColor StockAdjustColors[] = 
{
     {0x00, 0xDC, 0x00, 0x00}, {0x00, 0x08, 0x8C, 0x06}, {0x00, 0x00, 0x00, 0x51}, {0x00, 0x00, 0x00, 0x00}, 
     {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}
};

BalloonColor BalloonColorArray[8] =
{
     {0x00, 0xC8, 0x01, 0x00}, {0x00, 0x00, 0x70, 0x01}, {0x00, 0x10, 0x79, 0x51}, {0x00, 0x00, 0x59, 0x70},
     {0x00, 0x70, 0x55, 0x00}, {0x00, 0x7A, 0x7E, 0x00}, {0x00, 0x77, 0x2C, 0x24}, {0x00, 0x30, 0x14, 0x58}
};
BalloonColor BalloonAdjustArray[8] =
{
     {0x00, 0xDC, 0x00, 0x00}, {0x00, 0x00, 0x8C, 0x06}, {0x00, 0x00, 0x00, 0x51}, {0x00, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}
};
BalloonColor BalloonColorArrayB[8] =
{
     {0x00, 0xC8, 0x01, 0x00}, {0x00, 0x00, 0x70, 0x01}, {0x00, 0x10, 0x79, 0x51}, {0x00, 0x00, 0x59, 0x70},
     {0x00, 0x70, 0x55, 0x00}, {0x00, 0x7A, 0x7E, 0x00}, {0x00, 0x77, 0x2C, 0x24}, {0x00, 0x30, 0x14, 0x58}
};
BalloonColor BalloonAdjustArrayB[8] =
{
     {0x00, 0xDC, 0x00, 0x00}, {0x00, 0x00, 0x8C, 0x06}, {0x00, 0x00, 0x00, 0x51}, {0x00, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}
};


//Custom Levels

//Main

struct OKObject OKObjectArray[100];


//Multiple AI Paths Race Levels
OKAIPath CPUPaths[8];

//OKCustom Objects
short CoinPositions[8][3]; //8 Coins XYZ

short HsLabel, RaceStatus, StartupSwitch, CourseSwapped = -1;
bool ScrollLock = false;
int CurrentHeaderAddress = 0;

short FireParticlePositions[8][3];

Ushort MenuBlink, AudioLanguage;
short GPCourseIndex, HotSwapID, HotSwapGP, GPTimeCheck, CourseValue = -1;
short MenuAngle[4];

char ButtonHolding[4];
char ButtonTimer[4];
Ushort MenuButtonHeld[4];
bool MenuToggle;
float GPTotalTime[2] = { 0, 0 };


int VersionNumber;

int ParameterIndex, MenuIndex, MenuCup, MenuOverflow;



//
//


//CustomLevels
int ScrollValues[32][2];
//FaceStruct *CourseFaceStruct = (FaceStruct*)(&FaceBuffer);
//
//

char *CupNames[] = {"Mushroom Cup","Flower Cup","Star Cup","Special Cup"};
int CupChar[] = {12,10,8,11};
char *CourseNames[] = {"Mario Raceway", "Choco Mountain", "Bowser Castle", "Banshee Boardwalk","Yoshi Valley", "Frappe Snowland", "Koopa Troopa Beach", "Royal Raceway",
"Luigi Raceway", "Moo Moo Farm", "Toad Turnpike","Kalimari Desert","Sherbet Land","Rainbow Road","Wario Stadium", "Block Fort", "Skyscraper", "Double Deck", "DK Jungle Parkway","Big Donut"};
int CourseChar[] = {13,14,13,17,12,15,18,13,13,12,13,15,11,12,13,10,10,11,17,9};

Vector Origin = {0,0,0,};
ObjectivePlayer       Objectives[4];
ObjectiveObject     GameFlag[4];
ObjectiveObject     GameBase[4];

Marker* PlayerSpawnPoints;
CTFSpawn* ObjectivePoints;
BattleObjectivePoint* CustomObjectivePoints;

float     SpawnPoint[4][3];
char      FlagCount, TeamMode;
char      ScoreToWin, ObjectiveCount;
short     SpawnTime, HitstunTime;
short     TeamScore[2];

/* Two stock tables (20 x SkyColor). Do not overlay Bot onto Top. */
#define SKY_TOP(i) ((SkyColor *) &SkyColorTopTable + (i))
#define SKY_BOT(i) ((SkyColor *) &SkyColorBotTable + (i))

SkyColor *GlobalSkyColorTop[20] = {
	SKY_TOP(0), SKY_TOP(1), SKY_TOP(2), SKY_TOP(3),
	SKY_TOP(4), SKY_TOP(5), SKY_TOP(6), SKY_TOP(7),
	SKY_TOP(8), SKY_TOP(9), SKY_TOP(10), SKY_TOP(11),
	SKY_TOP(12), SKY_TOP(13), SKY_TOP(14), SKY_TOP(15),
	SKY_TOP(16), SKY_TOP(17), SKY_TOP(18), SKY_TOP(19),
};
SkyColor *GlobalSkyColorBot[20] = {
	SKY_BOT(0), SKY_BOT(1), SKY_BOT(2), SKY_BOT(3),
	SKY_BOT(4), SKY_BOT(5), SKY_BOT(6), SKY_BOT(7),
	SKY_BOT(8), SKY_BOT(9), SKY_BOT(10), SKY_BOT(11),
	SKY_BOT(12), SKY_BOT(13), SKY_BOT(14), SKY_BOT(15),
	SKY_BOT(16), SKY_BOT(17), SKY_BOT(18), SKY_BOT(19),
};
