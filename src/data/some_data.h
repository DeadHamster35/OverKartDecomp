#ifndef SOME_DATA_H
#define SOME_DATA_H

#include <PR/gbi.h>
#include "spline.h"
#include "data/other_textures.h"
#include "objects.h"

extern Vtx PlayerOneVtx[];
extern Vtx PlayerTwoVtx[];
extern Vtx PlayerThreeVtx[];
extern Vtx PlayerFourVtx[];
extern Vtx PlayerFiveVtx[];
extern Vtx PlayerSixVtx[];
extern Vtx PlayerSevenVtx[];
extern Vtx PlayerEightVtx[];
extern Vtx D_800E51D0[];
extern Vtx D_800E5210[];
extern Vtx BalloonVertexPlane1[];
extern Vtx BalloonVertexPlane2[];
extern u8 LetterPAL[];
extern u8* CourseOutlineTextures[];
extern s16 D_800E5520[];
extern s16 D_800E5548[];
extern u16 D_800E55A0[];
extern u16 D_800E55B0[];
extern u8 D_800E55D0[][3];
extern s32 D_800E55F8[];
extern s32 D_800E5618[];
extern s8 D_800E5628[];
extern Vec3s FireBreathsSpawns[];
extern s16 D_800E5740[];
extern s16 D_800E579C[];
extern s16 D_800E57F8[];
extern ThwompSpawn ThomwpSpawns50CC[];
extern ThwompSpawn ThwompSpawns100CCExtra[];
extern ThwompSpawn ThomwpSpawns150CC[];
extern f32 D_800E594C[];
extern u64 D_800E5974;
extern s16 D_800E597C[];
extern SplineData D_800E5988;
extern SplineData D_800E5A44;
extern SplineData D_800E5B08;
extern SplineData D_800E5BD4;
extern SplineData D_800E5C90;
extern SplineData D_800E5D54;
extern SplineData D_800E5D78;
extern SplineData* D_800E5D9C[];
extern SplineData* D_800E5DB0;
extern s8 D_800E5DB4[];
extern s16 D_800E5DF4[];
extern HegdehogSpawn HedgehogSpawns[];
extern Vec3s HedgehogPatrolPoints[];
extern SnowmanSpawn SnowmanSpawns[];
extern CrabSpawn CrabSpawns[];
extern s16 D_800E5FD0[];
extern SplineData D_800E6034;
extern SplineData D_800E60F0;
extern SplineData D_800E61B4;
extern SplineData D_800E6280;
extern SplineData* D_800E633C[];
extern Vec3s D_800E634C[];
extern MoleSpawnUnion MoleSpawns;
extern SplineData D_800E641C;
extern SplineData D_800E64D8;
extern SplineData D_800E659C;
extern SplineData D_800E6668;
extern SplineData* D_800E6724[];
extern SplineData* D_800E672C[];
extern float D_800E6734[];
// This should really be `extern Vec3s CaveFirePos[];`
extern s16 CaveFirePos[];
extern SplineData D_800E67B8;
extern SplineData D_800E6834;
extern SplineData D_800E68E0;
extern SplineData D_800E694C;
extern SplineData D_800E69B0;
extern SplineData D_800E69F4;
extern CloudData LuigiRacewayClouds[];
extern CloudData YoshiValleyMooMooFarmClouds[];
extern CloudData KoopaTroopaBeachClouds[];
extern CloudData RoyalRacewayClouds[];
extern CloudData SherbetLandClouds[];
extern CloudData KalimariDesertClouds[];
extern StarData ToadsTurnpikeRainbowRoadStars[];
extern StarData WarioStadiumStars[];
extern u8 D_800E6F30[][3];
extern u8 D_800E6F48[][3];

#endif
