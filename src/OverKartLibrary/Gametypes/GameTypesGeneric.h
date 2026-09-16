#include "../MainInclude.h"

#define SPAWN_POINT      0
#define FLAG_POINT       1
#define BASE_POINT       2

extern int ObjectiveMapID;
extern void DisplayGameScore(void);
extern void BalloonCheck(Player* player, f32 arg1, f32 arg2, s8 playerIndex, s8 balloonIndex, s16 rotation);
extern void ResetFlag(int ThisFlag);
extern void DrawGameFlags(Camera* LocalCamera);
extern void DrawGameBase(Camera* LocalCamera);
extern void PlacePlayerSpawn(const SVector Position, int PlayerID);
extern void PlaceFlagSpawn(const SVector Position, int PlayerID);
extern void PlaceBaseSpawn(const SVector Position, int PlayerID);
extern void DropFlag(int PlayerIndex);

extern bool CheckExecuteItem(int PlayerIndex);
