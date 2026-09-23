#ifndef OverKartVariablesH
#define OverKartVariablesH
#include "MainInclude.h"
#include "OKInclude.h"
//Menuoksetup


#define RACESWITCH		0
#define BATTLESWITCH	1


#define IM_DEFAULT 	0
#define IM_RANDOM 	1
#define IM_BALANCE	2
#define IM_NONE		3


extern SaveData SaveGame;
extern OKMenu   GameOKMenu;
extern OKPanel RacePanel, BattlePanel, MapPanel, RenderPanel;
extern OKOption OKGameOptions[];
extern OKOption OKMapOptions[];
extern OKOption OKRenderOptions[];


extern Uint StartLogoRAM, BackdropRAM, Splash3DRAM, MenuIconsRAM;
/* Host menu decompress dest after OKHeaderEND. Not ok_FreeSpace (DMA Source). */
extern int ok5_MenuStorage;
extern int ok5_MenuStorageEnd;

extern char* GameType;
extern char* RenderMode;
extern char* BattleMode;
extern char* CheatMode;
extern char* LevelSettingsMode;

extern int LoadedProgress;


extern short LastMenuID;
extern Uint ROptionPressed;
extern short MapModeCheck;

extern Uint CartridgeStatus;
extern Uint PrintLog[];
extern Uint PrintCount;

extern char *MenuNames[];
extern int MenuChar[];

extern char *GameOptionNames[];
extern char *GameParameters[][4];
extern int GameLimits[];
extern int MapLimits[];
extern int GameChar[][4];
extern int MapChar[][7];
extern int BattleLimits[];
extern int BattleChar[][3];


extern char *RenderOptionNames[];
extern char *RenderParameters[][3];
extern char *MapParameters[][7];
extern char *BattleOptionNames[];
extern char *MapOptions[];
extern char *BattleParameters[][3];
extern int RenderLimits[];
extern int RenderChar[][3];
extern int SaveStateCourseID;
extern int PageLimit[];

extern float AnalogInput[2];

//PracticeHack

extern int FlyCamPosition[];
extern float FlyCamRadian;
extern float FlyCamHeight;
extern short FlyCamSection, FlyCamSectionCheck;
extern short FlyViewSection, FlyCamViewCheck;
extern float FlyCamBackupPosition[3];
extern char FlyCamToggle, FlyCamCheck;
extern short FlyCamPilot;
extern short FlyCamDirection;
extern float FlyCamLook[2][2];
extern int LocalInput[2];
extern int InputColor[4];
extern char GenericInput, MiniMapMode;
extern short FlyCamSpeed;



//InputDisplay Variables
extern char *Names[];
extern short InputColors[][3];
extern short Coords[][2];
extern char *StickNames[];
extern short StickColor[][3];
extern short StickCoord[][2];
extern short ControllerInputX, ControllerInputY;


// Variables Draw3DRacer
extern short BaseTurn, AddTurn;

//OKCustom Objects





#endif
