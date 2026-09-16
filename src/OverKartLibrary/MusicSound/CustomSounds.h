#ifndef CustomSoundsH
#define CustomSoundsH
#include "../MainInclude.h"
extern void PlayCustomSound(void);
extern void MapDelayInit(void);
extern void SetLevelSounds(void);
extern void PlayStarMusicHook(uchar playerID);
extern void StopStarMusicHook(uchar playerID);
extern void PlayFinalLapMusicHook(uchar playerID);
#endif
