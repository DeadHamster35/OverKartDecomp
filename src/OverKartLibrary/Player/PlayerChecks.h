#ifndef PlayerCheckH
#define PlayerCheckH
extern short GetCourseLapMax(void);
extern short GetCourseLapIndex(int player);
extern Vector SprintStartBanner;
extern bool SprintFinishArmed;
extern void SetSprintFinish(void);
extern void WrapPathIndexAtFinishCheck(float posX, float posY, float posZ, short *wayPointIndex, int pathIndex);
extern void SprintLapCheck(int playerID, Player *car);
extern void SetLapIndex(void);
extern void CheckPaths(void);
extern void SetWaterType(char WaterType);
extern void NopSplashCheckCode(void);
extern void EffectBGMReplace(void);
extern void NopPlayEffectBGMCode(void);
extern void SetBalloonColor(int PlayerIndex, int R, int G, int B, int AdjR, int AdjG, int AdjB);
extern void SetBalloonTeams(void);
extern void SetBalloonDefaults(void);
extern void CheckJugemuMarker(void);
extern void LakituSpawnBypass(Player *Kart, char PlayerID, float *SpawnVector, float *FacingVector);
#endif
