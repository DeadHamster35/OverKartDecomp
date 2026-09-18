#ifndef OKMenuH
#define OKMenuH
#include "MainInclude.h"
#include "OKInclude.h"
extern void printAnticheat();
extern void printMap(int devParameter);
extern void printDetails();
extern void printGPTime(float printTime, int OffsetY);
extern void printTime(float printTime);
extern void PrintMenuTest();
extern void TitleMenuHandler(short PlayerIndex);
extern void GameOptionsHandler(short PlayerIndex);

extern void PlayerSelectSetup();
extern void PlayerSelectMenu(short StatsMode, short PlayerIndex);
extern void PlayerSelectMenuAfter();
extern void PlayerSelectMenuBefore();
extern void MapSelectMenu(short PlayerIndex);
extern void DrawPlayerSelect(short StatsMode);
extern void DrawMapSelect();
extern void DrawGameSelect();
extern void GameSelectMenu(short PlayerIndex);


extern void PlayerSelectSwitch(OSContPad *pad,u16 i, u16 newbutton);
extern void MapSelectSwitch(Controller *controller, u16 i);
extern void GameSelectSwitch(Controller *controller, u16 i);
extern void TitleMenuSwitch(Controller *controller, u16 i);
extern void DataMenuController(Controller *controller, u16 i);
extern short TitleSwitch, TitleCheck;

#endif