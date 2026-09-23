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
f32 XOrientation;
s16 D_8018D020;
f32 Panelx[8];
s16 D_8018D048;
f32 HUDCharpicRankY1[8];
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
s32 Modelingnum;
UNUSED s32 D_8018D128[6];
s32 D_8018D140;
UNUSED s32 D_8018D148[2];
s32 D_8018D150;
UNUSED s32 D_8018D154;
s32 Kwkartnum;
UNUSED s32 D_8018D15C;
s32 D_8018D160;
UNUSED s32 D_8018D164;
s32 D_8018D168;
s16 D_8018D16C;
s32 D_8018D170;
//! Used for cycling through snowflakes in KwanmSnowSearch3D
s16 D_8018D174;
s32 D_8018D178;
//! Also Next Free spot in CloudAllocate?
s16 D_8018D17C;
s32 D_8018D180;
s16 D_8018D184;
s32 KWScreenEnable;
s16 D_8018D18C;
s32 D_8018D190;
/**
 * Exact use Unknown, something related to the mole groups
 * Maybe be indicating that a given mole in a given group is Active?
 */
s8 D_8018D198[NUM_GROUP1_MOLES];
s32 D_8018D1A0;
s8 D_8018D1A8[NUM_GROUP2_MOLES];
s32 D_8018D1B4;
s8 D_8018D1B8[NUM_GROUP3_MOLES];
s32 D_8018D1C4;
/**
 * These seem to be limits on different object types in Moo Moo Farm
 * See KwallocEffectram1P in code_8006E9C0.c
 * Maybe max Number of Active moles in a given group of moles?
 */
s32 D_8018D1C8;
s32 D_8018D1CC;
s32 D_8018D1D0;
s32 D_8018D1D4;
s32 D_8018D1D8;
s32 D_8018D1DC;
u8* Logoptr;
UNUSED s32 D_8018D1E4;
f32 D_8018D1E8;
s32 D_8018D1EC;
//! Number of Used spots in CloudAllocate?
s32 CloudCount1P;
UNUSED s32 D_8018D1F4;
//! Next Free spot in CloudAllocate?
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
 * Destination for decompressed exhaust Textures (I think)
 * I'm not how much I like this "Pointer to an array" setup, but if it matches it matches
 */
u8 (*Kumoptr)[1024];
s32 D_8018D224;
u8 Kumotype;
s32 D_8018D22C;
//! Something related to the display of the clouds/stars?
s8 Kwhoshisw;
UNUSED s32 D_8018D238[2];
s32 MapTexture;
u8* D_8018D248[22];
f32 MapScale;
s32 D_8018D2A4;
UNUSED s32 D_8018D2A8;
s32 D_8018D2AC;
s16 MapWidth;
s32 D_8018D2B4;
s16 MapHeight;
s32 D_8018D2BC;
s16 MapX[4];
s32 D_8018D2C8[4];
s16 MapY[4];
s16 StartX;
UNUSED s32 D_8018D2E4;
s16 StartY;
UNUSED s32 D_8018D2EC;
s16 Rzerox;
UNUSED s32 D_8018D2F4;
s16 Rzeroy;
UNUSED s32 D_8018D2FC;
u16 MapR;
UNUSED s32 D_8018D304;
u16 MapG;
UNUSED s32 D_8018D30C;
u16 MapB;
s32 D_8018D314;
u16 Ralpha;
UNUSED s32 D_8018D31C;
s32 Kwgoalcnt;
UNUSED s32 D_8018D328[22];
s32 D_8018D380;
s32 D_8018D384;
s32 D_8018D388;
UNUSED s32 D_8018D390[11];
//! Controls Number of come object Type placed into EffectAllocArray2 on Frappe Snowland. So, maybe snowmen/snowflakes?
s32 D_8018D3BC;
//! Limit on some object Type (ice chips?) in Sherbet Land
s32 D_8018D3C0;
//! Some sort of limiter on how many of some object Type can spawn
s32 D_8018D3C4;
UNUSED s32 D_8018D3C8[3];
s32 Kwr;
s32 Kwg;
s32 Kwb;
//! some Alpha
s32 Kwa;
//! some red
s32 Wakur;
//! some Green
s32 Wakug;
//! some Blue
s32 Wakub;
s32 Wakua;
s32 Wakuanmtmmax;
s32 Wakuanmptr;
s32 Kwvideoframetimer;

s32 Kwgameframetimer;
UNUSED s32 Kwsyoriframe;
UNUSED s32 Kwsyoriframeback;
s32 D_8018D40C;
s32 D_8018D410;
UNUSED s32 D_8018D418[2];
u8* DriftSmokeV;
u8* DriftSmokeE;
u8* D_8018D428;
u8* D_8018D42C;
u8* D_8018D430;
u8* D_8018D434;
u8* Dobon1;
u8* Dobon2;
u8* Dobon3;
u8* Dobon4;
u8* Dobon5;
u8* Dobon6;
u8* Dobon7;
u8* Dobon8;
u8* Dobon9;
u8* Dobon10;
u8* Dobon11;
u8* Dobon12;
u8* Dobon13;
u8* Dobon14;
u8* Dobon15;
u8* KageBitmap;
u8* D_8018D478;
UNUSED u8* D_8018D47C;
u8* Fukidashi2;
u8* Onpu;
u8* Hoshi;
u8* D_8018D48C;
u8* D_8018D490;
u8* smoke;
u8* Grs;
u8* Bubble;
u8* Boing;
u8* Poomp1;
u8* Poomp2;
u8* Whir1;
u8* Whir2;
u8* Crash1;
u8* Crash2;
u8* Balloon1;
u8* Balloon2;
u8* ThunderA;
u8* ThunderB;
Vec3f PlayerBalloonPosX[8];
Vec3f PlayerBalloonPosY[8];
Vec3f PlayerBalloonPosZ[8];
u16 PlayerBalloonStatus[8][3];
Vec3s D_8018D620[8];
Vec3f D_8018D650[8];
Vec3f D_8018D6B0[8];
Vec3f D_8018D710[8];
Vec3s D_8018D770[8];
Vec3s D_8018D7A0[8];
Vec3s D_8018D7D0[8];

Vec3s D_8018D800[8];
Vec3s D_8018D830[8];
Vec3s PlayerBalloonRotation[8];
Vec3s D_8018D890[8];
s16 BalloonCount[8];
Vec3s PlayerBalloonDepartingTimer[8];
