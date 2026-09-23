#ifndef CustomSoundsH
#define CustomSoundsH
#include "../MainInclude.h"
extern void PlayCustomSound(void);
extern void MapDelayInit(void);
extern void SetLevelSounds(void);
extern void PlayStarMusicHook(Uchar PlayerID);
extern void StopStarMusicHook(Uchar PlayerID);
extern void PlayFinalLapMusicHook(Uchar PlayerID);
#endif
