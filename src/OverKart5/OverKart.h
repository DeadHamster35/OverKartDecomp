#ifndef OverKartH
#define OverKartH
#include "MainInclude.h"
#include "OKInclude.h"

extern void loadLogo(void);
extern void SetMenuPanels(int SwitchMode);
extern void okSetup(void);
extern bool checkEndGame(void);
extern void startRace(void);
extern void endRace(void);
extern void CheckIFrames(void);
extern void ExecuteItemHook(Player* Car);
extern void CheckBattleCDown(void);
extern void QuickCamCode(Camera* LocalCamera, Player* LocalPlayer);
extern void CameraCheckFunc(Player* player, Camera* camera, s8 index);
extern void KartCheckFuncA(void);
extern void KartCheckFuncB(void);
extern void KartCheckFuncC(void);
extern void KartCheckFuncD(void);
extern void KartCheckFuncA2(void);
extern void KartCheckFuncB2(void);
extern void KartCheckFuncC2(void);
extern void KartCheckFuncD2(void);
extern void ResultsCheck(void);
extern void gameCode(void);
extern void allRun(void);
extern void PrintMenuFunction(void);

#if OverKartBuild
extern void CheatSetup(void);
extern bool CheckCheat(void);
extern void DrawCheatMenu(void);
extern void CheatMenuHandler(void);
extern void ApplyCheats(void);
extern void LoadCheatData(void);
extern void DrawCheat3D(int CurrentPlayer);
#endif

#endif
