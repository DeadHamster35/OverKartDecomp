#ifndef LibraryVariablesH
#define LibraryVariablesH
#include "MainInclude.h"

extern Uint GlobalAddressA, GlobalAddressB, GlobalAddressC, GlobalAddressD;
extern int GlobalIntA, GlobalIntB, GlobalIntC, GlobalIntD;
extern Int64     GlobalInt64;
extern Uint64    GlobalUInt64;
extern unsigned int GlobalUIntA, GlobalUIntB;
extern float GlobalFloatA, GlobalFloatB, GlobalFloatC, GlobalFloatD;
extern char GlobalCharA,GlobalCharB,GlobalCharC,GlobalCharD,GlobalCharE,GlobalFrameChar;

extern short GlobalShortA, GlobalShortB, GlobalShortC, GlobalShortD;
extern unsigned short GlobalUShortA, GlobalUShortB;
extern bool GlobalBoolA, GlobalBoolB, GlobalBoolC, GlobalBoolD;
extern int LoopValue;
extern Uint GlobalFrameCount;
extern Uint ClockCycle[2], OldCycle[2];
extern Uint CycleCount[2];

extern bool TempoBool, StopSwop;
extern char ConsolePlatform, EmulatorPlatform;
extern Uint CartridgeStatus;
extern bool SurfaceExplorerMode;
extern bool CustomWaterHeight[];
extern short EchoStart;
extern short EchoStop;
extern short CloudCourseID, Snow3DCourseID;
extern long g_SnowParticleTex[36];
extern float g_skySnowScale;
extern float g_skySnowVelocity;
extern long g_3DSnowSpawnHeight;
extern long g_3DSnowSpawnDistanceMin;
extern long g_3DSnowSpawnDistanceMax;
extern long g_3DSnowSpawnCone;
extern float g_3DSnowSwayMovement;
extern float g_3DSnowScale;
extern float g_3DSnowVelocityUpLim;
extern float g_3DSnowVelocityLowLim;
extern s32 OKLakituIce;
extern s32 OKCheckSplashEnabled;
extern s32 OKForceRoulette;
extern short AsmCupCount;
extern s8 OKPSelSkipCursor[4];
extern s8 OKPSelSkipPortrait[8];
extern s32 OKPSelSkipAnime;
extern s32 OKMusicIn3P4P;
extern char FlyCamToggle;
extern short FlyCamSection;
extern char CloudCourseValue, Snow3DCourseValue, WeatherCourseValue, Toggle3DSnow;
extern short BattleGametype;

extern short MenuPosition[2];
extern Ushort PerspectiveValue;


extern int WholeNumber;
extern int DecimalNumber;
extern int PrintOffsetA, PrintOffsetB, PrintOffsetC, PrintOffsetD;
extern int Minutes;
extern int Seconds;
extern Object *GlobalObjectA, *GlobalObjectB;
extern OKRAMHeader OverKartRAMHeader;


extern short AnimationTimer;

extern short DynFPSModifier;
extern char OKStaticTempoOnly;

extern float ZeroVector[3];

extern POSXY PlayerSelectPositions[];
extern struct PlayerTextureTable SelectNamePlateTable;
/* Overlay of kart_attributes tables that begin at stock GlobalStat[]. */
#define KartStats (*(struct Stats *) GlobalStat)
extern float GlobalWeight[];


extern struct Controller *GlobalController[];
extern struct Screen *GlobalScreen[];
extern Camera *GlobalCamera[];
extern u16 *GlobalPath[];
extern s32 *GlobalLap[];
extern FaceStruct *CourseFaceStruct;
extern struct PlayerState *GlobalPlayerState[];  
extern Hud *GlobalHud[];
extern char *GlobalLightning[];
extern SkyColor *GlobalSkyColorTop[20];
extern SkyColor *GlobalSkyColorBot[20];
extern struct Playercolor *GlobalPlayercolor[];





//MarioKart3D Variables
extern short CoinCount[8];
extern short IFrames[8];
extern float AffineMatrix[4][4];
extern float AffineMatrix2[4][4];
extern float ObjectPosition[3];
extern Vector ObjectVector[];
extern float ObjectVelocity[3];
extern short ObjectAngle[3];
extern short ObjectIndex;
//
//
extern struct OKEngine EngineClass[3][3];

extern short EngineSpeed[4][3];
extern short AccelerationCurve[3][10];
extern short SteerAngle[3];
extern short SteerValue[3];
extern short PowerBand[3];
extern short EnginePowerDownRT[3][15];
extern short EnginePowerDownFT[3][15];


extern char MenuBackup,MenuChanged,MenuTimer;
extern bool MapIsCurrentlyLoading;
extern char MenuFlash[4];
extern char MenuProgress[4];

extern char PlayerCharacterSelect[4];
extern char PlayerShowStats[4];
extern char PlayerEngineSelect[4];
extern char PlayerSteerSelect[4];
extern struct PlayerTextureTable BackupNamePlateTable;
extern struct PlayerTextureTable BackupPortraitTable[9];


//Mario Kart Stats
extern char StatsID;
//
//





//SharedFunctions
extern long DataLength; //
extern int *TargetAddress;
extern int *SourceAddress;
extern int *TempPointer;
extern long *GraphPointer;
extern int *TkmPoint;
extern int RSPNumber;
extern int RSPOffset;
extern char *Hex;

extern Ushort RedTextPalette[];
extern Ushort BlueTextPalette[];
extern Ushort GreenTextPalette[];
extern Ushort WhiteTextPalette[];

extern char RedPaletteF3D[];
extern char BluePaletteF3D[];
extern char GreenPaletteF3D[];
extern char WhitePaletteF3D[];
//

//Stock Names
extern char *StockCourseNames[];
extern char *StockCharacterNames[];

extern BalloonColor StockBalloonColors[];
extern BalloonColor StockAdjustColors[];

//Custom Levels


//Main

extern struct OKObject OKObjectArray[];
extern OKAIPath CPUPaths[];
extern short HsLabel, RaceStatus, StartupSwitch, CourseSwapped;
extern bool ScrollLock;
extern int CurrentHeaderAddress;
extern short CoinPositions[8][3]; //8 Coins XYZ

extern short FireParticlePositions[8][3];

extern Ushort MenuBlink, AudioLanguage;
extern short GPCourseIndex, HotSwapID, HotSwapGP, GPTimeCheck, CourseValue;
extern short MenuAngle[4];

extern char ButtonHolding[4];
extern char ButtonTimer[4];
extern Ushort MenuButtonHeld[4];
extern bool MenuToggle;
extern float GPTotalTime[2];


extern int VersionNumber;

extern int ParameterIndex, MenuIndex, MenuCup, MenuOverflow;
//
//


//CustomLevels
extern int ScrollValues[32][2];



extern char *CupNames[];
extern int CupChar[];
extern char *CourseNames[];
extern int CourseChar[];


extern Vector Origin;
extern ObjectivePlayer       Objectives[4];
extern ObjectiveObject     GameFlag[4];
extern ObjectiveObject     GameBase[4];

extern Marker* PlayerSpawnPoints;
extern CTFSpawn* ObjectivePoints;
extern BattleObjectivePoint* CustomObjectivePoints;

extern float SpawnPoint[4][3];
extern char      FlagCount, TeamMode;
extern char      ScoreToWin, ObjectiveCount;
extern short     SpawnTime, HitstunTime;
extern short     TeamScore[2];


#define 	BTN_A		0x8000 
#define 	BTN_B		0x4000 
#define 	BTN_Z		0x2000 
#define 	BTN_START	0x1000 
#define 	BTN_DUP		0x0800 
#define 	BTN_DDOWN	0x0400 
#define 	BTN_DLEFT	0x0200 
#define 	BTN_DRIGHT	0x0100 
#define 	BTN_L		0x0020 
#define 	BTN_R		0x0010 
#define 	BTN_CUP		0x0008 
#define 	BTN_CDOWN	0x0004 
#define 	BTN_CLEFT	0x0002 
#define 	BTN_CRIGHT	0x0001

#define KO_PSEL_LOGO	42
#define KO_PSEL_MA	43
#define KO_PSEL_LU	44
#define KO_PSEL_PE	45
#define KO_PSEL_KI	46
#define KO_PSEL_YO	47
#define KO_PSEL_KA	48
#define KO_PSEL_WA	49
#define KO_PSEL_KU	50
#define KO_PSEL_OK	51
#define KO_PSEL_1P	52
#define KO_PSEL_2P	53
#define KO_PSEL_3P	54
#define KO_PSEL_4P	55



#define BTL_BATTLE 	0
#define BTL_CTF		1
#define	BTL_SOCCER	2

#endif
