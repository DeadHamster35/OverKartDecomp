#ifndef MarioKart3DH
#define MarioKart3DH
#include "MainInclude.h"


extern Vtx V1632[];
extern Vtx Tree1V[];

extern void WaveRace(Vtx_t *VertexBuffer, int Count, float Intensity);
extern short WaveTime, WaveDirection;


extern void DrawStereoscopic3D(short FocusValue);
extern void ScaleMatrixXYZFixed(AffineMtx Matrix, SVector Vector);
extern void ScaleMatrixXYZ(AffineMtx Matrix, Vector Vector);
extern void DrawGeometryScale(float ObjectPosition[], short ObjectAngle[], int F3DEXAddress, float Scale);
extern void DrawRedCoin(float ObjectPosition[], short ObjectAngle[], int F3DEXAddress, float Scale);
extern void DrawGeometrySVectorScale(SVector ObjectPosition, short ObjectAngle[], int F3DEXAddress, float Scale);
extern void DrawGeometry(float ObjectPosition[], short ObjectAngle[], int F3DEXAddress);

extern void DrawPaths(long RSPInput);
#endif
