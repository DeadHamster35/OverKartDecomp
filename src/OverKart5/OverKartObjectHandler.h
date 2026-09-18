#ifndef OverKartObjectHandlerH
#define OverKartObjectHandlerH
#include "MainInclude.h"
#include "OKInclude.h"
extern void DrawPerScreen(Camera* LocalCamera);
extern void loadCoin(void);
extern void DisplayObject(void* Car, Object* InputObject);
extern void RedCoinChallenge(long PathOffset);
extern void PlaceSIBox(long BoxOffset);
extern void GoldCoinChallenge(uint PathOffset, int CoinCount);
extern void CollideObject(Player* Car, Object* Target);
extern int RedCoinCollide(Player* Car, Object* Coin);
extern int GoldCoinCollide(Player* Car, Object* Coin);
extern void CheckHit(int PlayerIndex, int HitType);
extern void Draw3DRacer(uint ModelAddress, uint Player);
#endif
