#ifndef OKCustomObjectH
#define OKCustomObjectH
#include "../MainInclude.h"
extern void Draw3DRacer(void);
extern void DisplayRedCoin(void *Camera, void *Object);
extern void DisplayCoinSprite(void);
extern void DisplayAfterEffects(void);
extern void DrawOKObjects(Camera* LocalCamera, int ForceRender);
extern void DrawOKObjectLoop(OKModel* ThisModel, int Player, int Type, int ForceRender);
extern void CheckOKObjects(void);
extern void ClearOKObject(short ObjectID);
extern short FindOKObject(void);
extern void SetLocalMatrix(int CurrentObject, int Type, int Player);

extern bool TestCollideBox(float BoxPosition[], float BoxSize[], short BoxAngle[], float TargetPosition[], float TargetRadius);
extern bool TestCollideSphereAngled(float SourcePosition[], float SourceRadius, short SourceAngle[], short BoxPosition[], float TargetPosition[], float TargetRadius);
extern bool TestCollideSVectorSphere(float SourcePosition[], float SourceRadius, SVector TargetPosition, float TargetRadius);
extern bool TestCollideSphere(float SourcePosition[], float SourceRadius, float TargetPosition[], float TargetRadius);
#endif
