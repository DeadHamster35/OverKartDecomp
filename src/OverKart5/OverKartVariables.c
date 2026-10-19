#include "MainInclude.h"
#include "OKInclude.h"


//Menu



SaveData SaveGame;

char* GameType = (char*)&SaveGame.GameSettings;
char* RenderMode = (char*)&SaveGame.RenderSettings;
char* BattleMode = (char*)&SaveGame.BattleSettings;
char* CheatMode = (char*)&SaveGame.CheatSettings;
char* LevelSettingsMode = (char*)&SaveGame.LevelSettings;
int LoadedProgress;


short LastMenuID = 0;
Uint ROptionPressed = 0;
short MapModeCheck = 0;


Uint PrintLog[99];
Uint PrintCount;

float AnalogInput[2];

int ItemChance[8];
//
//
//PracticeHack

int FlyCamPosition[3];

char GenericInput, MiniMapMode = 0, FlyCamCheck;
short FlyCamSectionCheck;
short FlyCamView, FlyCamViewCheck;
short FlyCamPilot,FlyCamDirection;
float FlyCamBackupPosition[3];
float FlyCamRadian;
float FlyCamLook[2][2];
int LocalInput[2];
int InputColor[4];

short FlyCamSpeed = 5;



short InputColors[][3] = {
	{  0, 192, 255}, //A: Blue
	{  0, 255,   0}, //B: Green
	{255, 255, 255}, //Z: white
	{255,   0,   0}, //Start: red
	{192, 192, 192}, {192, 192, 192}, //up, down
	{192, 192, 192}, {192, 192, 192}, //Left, Right
	{  0,   0,   0}, {  0,   0,   0}, //Unused
	{255, 255, 255}, {255, 255, 255}, //L, R
	{255, 255,   0}, {255, 255,   0}, //C up, down
	{255, 255,   0}, {255, 255,   0}  //C Left, Right
};
short Coords[][2] = {
	{3, 1}, //A
	{3, 2}, //B
	{5, 1}, //Z
	{3, 0}, //Start
	{1, 0}, {1, 2}, {0, 1}, {2, 1}, //d-pad
	{0, 0}, {0, 0}, //Unused
	{4, 0}, //L
	{6, 0}, //R
	{5, 0}, //C up
	{5, 2}, //C down
	{4, 1}, //C Left
	{6, 1}  //C Right
};
short StickColor[][3] = {
	{192, 192, 192},
	{192, 192, 192},
	{192, 192, 192},
	{192, 192, 192},
	{255, 255, 255}
};
short StickCoord[][2] = {
	{1, 0}, //U
	{1, 2}, //D
	{0, 1}, //L
	{2, 1}, //R
	{1, 1} //stick
};

short ControllerInputX = 247;
short ControllerInputY = 215;




// Variables Draw3DRacer
short BaseTurn, AddTurn = 0;
//
//



int SaveStateCourseID;
int PageLimit[] = {8,9,6,5,3};  // GAME - RENDER - MAP - CUP EDITOR  -  BATTLE

OKPanel RacePanel, BattlePanel, RenderPanel, MapPanel;
OKOption OKGameOptions[8];
OKOption OKRenderOptions[9];
OKOption OKMapOptions[6];

OKMenu GameOKMenu;

Uint StartLogoRAM, BackdropRAM, Splash3DRAM, MenuIconsRAM;
int BattleLimits[] = {2,2,2};
int BattleChar[][3] = {{7,3,6}, {3,2,0},{3,2,0}};


char *MenuNames[] = {"Game Options", "Render Options","Map Options", "Cup Editor"};
int MenuChar[] = {12,14,11,10};

char *GameOptionNames[] = {
	"Game Mode",
	"Racer Stats",
	"GP Always Advance",
	"All Cup",
	"Items",
	"Multiplayer",
	"Explorer Mode",
	"Flycam Mode"
};

char *GameParameters[][4] = {
	{"Default", "Red Coin", "Gold Coin", "Practice"},
	{"Classic", "Equal", "Enhanced", ""},
	{"Off" , "On", "", ""},
	{"Off" , "On", "", ""},
	{"Default","Random","Balanced", "None"},
	{"Classic" , "Bots 2-4", "Bots 3+4", "Bot 4"},
	{"Off" , "On", "", ""},
	{"Off" , "On", "", ""}
};

int GameLimits[] = {
	3,
	2,
	2,
	2,
	4,
	4,
	2,
	2
};
int GameChar[][4] = {
	{7,8,9,8},
	{7,5,8,0},
	{3,2,0,0},
	{3,2,0,0},
	{7,6,8,4},
	{7,8,8,5},
	{3,2,0,0},
	{3,2,0,0}
};


char *BattleOptionNames[] = {"Game Mode", "Team Battle", "Flycam"};
char *BattleParameters[][3] = {{"Default", "CTF", "Soccer"}, {"Off" , "On"}, {"Off" , "On"}};


char *RenderOptionNames[] = {
	"Audio Mode",
	"Widescreen",
	"Anti-Alias",
	"Screen Split",
	"Draw Dist.",
	"Game Tempo",
	"Cull Mode",
	"Show FPS",
	"Input Disp.",
	"Detail Disp."
};
char *RenderParameters[][3] = {
	{"USA", "JP", ""},
	{"Off" , "On", ""},
	{"Off" , "On", ""},
	{"Horizontal", "Vertical", ""},
	{"Default","Extended", ""},
	{"Console","Emulator", ""},
	{"LLE","HLE",""},
	{"Off" , "On", ""},
	{"Off" , "On", ""},
	{"Off" , "On", "Shortcut"}
};

int RenderLimits[] = {
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	3
};
int RenderChar[][3] = {
	{3,2,0},
	{3,2,0},
	{3,2,0},
	{10,8,0},
	{7,8,0},
	{7,8,0},
	{3,3,0},
	{3,2,0},
	{3,2,0},
	{3,2,8}
};


char *MapOptions[] = {
	"X Scale",
	"Y Scale",
	"Z Scale",
	"X Mirror",
	"Y Mirror",
	"Z Mirror"
};
char *MapParameters[][7] = {
	{"0.0", "0.5", "1.0", "1.5", "2.0", "2.5", "3.0"},
	{"0.0", "0.5", "1.0", "1.5", "2.0", "2.5", "3.0"},
	{"0.0", "0.5", "1.0", "1.5", "2.0", "2.5", "3.0"},
	{"Off" , "On","","","","",""},
	{"Off" , "On","","","","",""},
	{"Off" , "On","","","","",""}
};

int MapLimits[] = {
	7,
	7,
	7,
	2,
	2,
	2
};
int MapChar[][7] = {
	{3,3,3,3,3,3,3},
	{3,3,3,3,3,3,3},
	{3,3,3,3,3,3,3},
	{3,2,0,0,0,0,0},
	{3,2,0,0,0,0,0},
	{3,2,0,0,0,0,0}
};



//InputDisplay Variables
char *Names[] = {
	"A", "B", "Z", "S",
	"U", "D", "L", "R", // draw d-pad
	"", "", //Unused bits
	"L", "R",
	"U", "D", "L", "R" //C buttons
};
char *StickNames[] = {
	"U", "D", "L", "R","S"
};
