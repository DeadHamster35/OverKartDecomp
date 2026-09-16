#include <ultra64.h>
#include <macros.h>
#include "code_80057C60.h"

s16 D_8018CFE0;
f32 D_8018CFE4;
s16 D_8018CFE8;
f32 D_8018CFEC;
s16 D_8018CFF0;
f32 D_8018CFF4;
s16 D_8018CFF8;
UNUSED s32 D_8018CFFC;
s16 D_8018D000;
UNUSED s32 D_8018D004;
s16 D_8018D008;
f32 D_8018D00C;
s16 D_8018D010;
UNUSED s32 D_8018D014;
s16 D_8018D018;
f32 xOrientation;
s16 D_8018D020;
f32 panelx[8];
s16 D_8018D048;
f32 g_hudCharpicRankY1[8];
s16 D_8018D070;
f32 D_8018D078[8];
s16 D_8018D098;
f32 D_8018D0A0[8];
s16 D_8018D0C0;
f32 D_8018D0C8[8];
s16 D_8018D0E8;
f32 D_8018D0F0[8];
s16 D_8018D110;
s32 D_8018D114;
UNUSED s32 D_8018D118[2];
s32 modelingnum;
UNUSED s32 D_8018D128[6];
s32 D_8018D140;
UNUSED s32 D_8018D148[2];
s32 D_8018D150;
UNUSED s32 D_8018D154;
s32 kwkartnum;
UNUSED s32 D_8018D15C;
s32 D_8018D160;
UNUSED s32 D_8018D164;
s32 D_8018D168;
s16 D_8018D16C;
s32 D_8018D170;
//! Used for cycling through snowflakes in kwanm_snow_search_3D
s16 D_8018D174;
s32 D_8018D178;
//! Also next free spot in g_CloudAllocate?
s16 D_8018D17C;
s32 D_8018D180;
s16 D_8018D184;
s32 g_KWScreenEnable;
s16 D_8018D18C;
s32 D_8018D190;
/**
 * Exact use unknown, something related to the mole groups
 * Maybe be indicating that a given mole in a given group is active?
 */
s8 D_8018D198[NUM_GROUP1_MOLES];
s32 D_8018D1A0;
s8 D_8018D1A8[NUM_GROUP2_MOLES];
s32 D_8018D1B4;
s8 D_8018D1B8[NUM_GROUP3_MOLES];
s32 D_8018D1C4;
/**
 * These seem to be limits on different object types in Moo Moo Farm
 * See kwalloc_effectram_1p in code_8006E9C0.c
 * Maybe max number of active moles in a given group of moles?
 */
s32 D_8018D1C8;
s32 D_8018D1CC;
s32 D_8018D1D0;
s32 D_8018D1D4;
s32 D_8018D1D8;
s32 D_8018D1DC;
u8* logoptr;
UNUSED s32 D_8018D1E4;
f32 D_8018D1E8;
s32 D_8018D1EC;
//! Number of used spots in g_CloudAllocate?
s32 CloudCount1P;
UNUSED s32 D_8018D1F4;
//! Next free spot in g_CloudAllocate?
s32 CloudCount2P;
s32 D_8018D1FC;
s16 D_8018D200;
s32 D_8018D204;
s16 D_8018D208;
s32 D_8018D20C;
s16 D_8018D210;
bool D_8018D214;
s16 D_8018D218;
s32 D_8018D21C;
/**
 * Destination for decompressed exhaust textures (I think)
 * I'm not how much I like this "pointer to an array" setup, but if it matches it matches
 */
u8 (*kumoptr)[1024];
s32 D_8018D224;
u8 kumotype;
s32 D_8018D22C;
//! Something related to the display of the clouds/stars?
s8 kwhoshisw;
UNUSED s32 D_8018D238[2];
s32 g_mapTexture;
u8* D_8018D248[22];
f32 g_mapScale;
s32 D_8018D2A4;
UNUSED s32 D_8018D2A8;
s32 D_8018D2AC;
s16 g_mapWidth;
s32 D_8018D2B4;
s16 g_mapHeight;
s32 D_8018D2BC;
s16 g_mapX[4];
s32 D_8018D2C8[4];
s16 g_mapY[4];
s16 g_startX;
UNUSED s32 D_8018D2E4;
s16 g_startY;
UNUSED s32 D_8018D2EC;
s16 rzerox;
UNUSED s32 D_8018D2F4;
s16 rzeroy;
UNUSED s32 D_8018D2FC;
u16 g_mapR;
UNUSED s32 D_8018D304;
u16 g_mapG;
UNUSED s32 D_8018D30C;
u16 g_mapB;
s32 D_8018D314;
u16 ralpha;
UNUSED s32 D_8018D31C;
s32 kwgoalcnt;
UNUSED s32 D_8018D328[22];
s32 D_8018D380;
s32 D_8018D384;
s32 D_8018D388;
UNUSED s32 D_8018D390[11];
//! Controls number of come object type placed into EffectAllocArray2 on Frappe Snowland. So, maybe snowmen/snowflakes?
s32 D_8018D3BC;
//! Limit on some object type (ice chips?) in Sherbet Land
s32 D_8018D3C0;
//! Some sort of limiter on how many of some object type can spawn
s32 D_8018D3C4;
UNUSED s32 D_8018D3C8[3];
s32 kwr;
s32 kwg;
s32 kwb;
//! some alpha
s32 kwa;
//! some red
s32 wakur;
//! some green
s32 wakug;
//! some blue
s32 wakub;
s32 wakua;
s32 wakuanmtmmax;
s32 wakuanmptr;
s32 kwvideoframetimer;

s32 kwgameframetimer;
UNUSED s32 kwsyoriframe;
UNUSED s32 kwsyoriframeback;
s32 D_8018D40C;
s32 D_8018D410;
UNUSED s32 D_8018D418[2];
u8* drift_smoke_V;
u8* drift_smoke_E;
u8* D_8018D428;
u8* D_8018D42C;
u8* D_8018D430;
u8* D_8018D434;
u8* dobon1;
u8* dobon2;
u8* dobon3;
u8* dobon4;
u8* dobon5;
u8* dobon6;
u8* dobon7;
u8* dobon8;
u8* dobon9;
u8* dobon10;
u8* dobon11;
u8* dobon12;
u8* dobon13;
u8* dobon14;
u8* dobon15;
u8* kage_bitmap;
u8* D_8018D478;
UNUSED u8* D_8018D47C;
u8* fukidashi2;
u8* onpu;
u8* hoshi;
u8* D_8018D48C;
u8* D_8018D490;
u8* smoke;
u8* grs;
u8* bubble;
u8* boing;
u8* poomp1;
u8* poomp2;
u8* whir1;
u8* whir2;
u8* crash1;
u8* crash2;
u8* balloon1;
u8* balloon2;
u8* thunderA;
u8* thunderB;
Vec3f gPlayerBalloonPosX[8];
Vec3f gPlayerBalloonPosY[8];
Vec3f gPlayerBalloonPosZ[8];
u16 gPlayerBalloonStatus[8][3];
Vec3s D_8018D620[8];
Vec3f D_8018D650[8];
Vec3f D_8018D6B0[8];
Vec3f D_8018D710[8];
Vec3s D_8018D770[8];
Vec3s D_8018D7A0[8];
Vec3s D_8018D7D0[8];

Vec3s D_8018D800[8];
Vec3s D_8018D830[8];
Vec3s gPlayerBalloonRotation[8];
Vec3s D_8018D890[8];
s16 BalloonCount[8];
Vec3s gPlayerBalloonDepartingTimer[8];
