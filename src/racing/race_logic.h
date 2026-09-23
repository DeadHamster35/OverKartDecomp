#ifndef RACE_LOGIC_H
#define RACE_LOGIC_H

/* Function Prototypes */

void CheckCol(Player*, Player*);
void SaveKeyStatus(void);
void LoadKeyStatus(void);
void BattleEnd(void);
void CheckBattleStatus(void);
void CheckGoalinTime(void);
void CheckFinish(void);
void FinishVSZoom(void);
void FinishZoom(void);
void CallNextSequence(s32);
void StartRaceBGM(s32);
void StartRacing(void);
f32 GetLaptime(s32);
void GoalinSubroutin(s32);
void CheckKartRap(void);
void func_8028F3E8(void);
void CheckKartRank(void);
void EnemyControl(void);
void RaceResetCheck(void);
void SetVpScale(void);
void ZoomOutScreen(void);
void ZoomInScreen(void);
void CheckPause(void);
void StartFadeout(void);
void CheckTitleDemo(void);
void RaceControl(void);
void GotoTitle(void);
void GotoGameselect(void);
void GoToGameSelect(void);
void GotoMapselect(void);
void GotoRacing(void);
void CollisionKartToKart(void);
void PauseSequence(void);

extern f32 RateCountF[];
extern s32 GPCurrentRaceRankByPlayerId[]; // D_801643B8 (Position for each Player)

#endif
