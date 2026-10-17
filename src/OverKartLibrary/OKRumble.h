#ifndef OKRumbleH
#define OKRumbleH

extern s32 RumbleActive[4];
extern s32 RumbleTimer[4];
extern s32 RumbleFramesOn[4];
extern s32 RumbleFramesOff[4];

void SetRumble(char PlayerID, s32 framesOn, s32 framesOff, s32 duration);
void ClearRumble(char PlayerID);
void UpdateRumbleHelper(void);

#endif
