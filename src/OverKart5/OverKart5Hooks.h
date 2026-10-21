#ifndef OVERKART5_HOOKS_H
#define OVERKART5_HOOKS_H

/*
 * OverKart5-only vanilla call-site declarations. Shared Library names stay in
 * OverKartHooks.h. Vanilla OverKart C includes this instead of OverKartHooks.h.
 */

#include "OverKartHooks.h"

extern short RaceStatus;
extern bool ScrollLock;

void EndRace(void);
void DataMenuController(Controller* controller, u16 controllerIdx);
void TitleMenuSwitch(Controller* controller, u16 controllerIdx);
void GameSelectSwitch(Controller* controller, u16 controllerIdx);
void MapSelectSwitch(Controller* controller, u16 controllerIdx);

void CameraCheckFunc(Player* Player, Camera* Camera, s8 Index);
void KartCheckFuncA(void);
void KartCheckFuncB(void);
void KartCheckFuncC(void);
void KartCheckFuncD(void);
void KartCheckFuncA2(void);
void KartCheckFuncB2(void);
void KartCheckFuncC2(void);
void KartCheckFuncD2(void);
void ResultsCheck(void);

#endif
