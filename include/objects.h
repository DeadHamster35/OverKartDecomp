#ifndef OBJECTS_H
#define OBJECTS_H

#include "spline.h"
#include <common_structs.h>

#define OBJECT_LIST_SIZE 0x226
#define SOME_OBJECT_INDEX_LIST_SIZE 32

typedef struct {
    /* 0x00 */ f32 scale;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f pos_start;
    /* 0x1C */ Vec3f pos_ende;
    /* 0x28 */ Vec3f pos_spline;
    /* 0x34 */ f32 vel;
    /* 0x38 */ Vec3f velocity; // acceleration
    /* 0x44 */ f32 ground;
    /* 0x48 */ s32 ival;
    /* 0x4C */ s32 timer;
    /* 0x50 */ s32 anmtm;
    /* 0x54 */ s32 flag; // Library uint; keep s32
    /* 0x58 */ s32 evflag;
    /* 0x5C */ s32 evtrflag;
    /* 0x60 */ u8* texaddr;
    /* 0x64 */ u8* indexaddr;
    /**
     * "list" is something of a misnomer for the names here
     * they can be pointers to just 1 tlut/texture, but it is common for one or the other
     * to be a pointer to an array of tluts/textures.
     **/
    /* 0x68 */ u8* stexaddr; // I feel like this should actually be `u8 (*tlutList)[512]`, but that causes mismatches
    /* 0x6C */ u8* sindexaddr;
    /* 0x70 */ Gfx* shape;
    /* 0x74 */ Vtx* vtxaddr;
    /* 0x78 */ s8 unk_078[0x04]; // Library Vtx* svtxaddr is type change
    /* 0x7C */ SplineControlPoint* sptbaddr;
    /* 0x80 */ SplineData* sptbtopaddr;
    /* 0x84 */ s16 stack[0xA]; // Library stack[8]+pointkazu+spptptr; indexed as 10
    /* 0x98 */ u16 spstep;
    /* 0x9A */ u16 spstepadd;
    /* 0x9C */ s16 ix;
    /* 0x9E */ s16 iy;
    /* 0xA0 */ s16 alpha;
    /* 0xA2 */ s16 input;
    /* 0xA4 */ s16 output; // Library type is at 0xD4
    /* 0xA6 */ s16 anmptr;
    /* 0xA8 */ s16 anmptrbak;
    /* 0xAA */ s16 fvaltm;
    /* 0xAC */ s16 svaltm;
    /* 0xAE */ s16 mvptr;
    /* 0xB0 */ s16 mvtm;
    /* 0xB2 */ Vec3su texang; // rotation, I think
    /* 0xB8 */ Vec3su kageang;
    /* 0xBE */ Vec3su ang;
    /* 0xC4 */ u16 addangy;
    /* 0xC6 */ u16 destangy;
    /* 0xC8 */ u16 r; // Library splits alloc at 0xC9; keep u16
    /* 0xCA */ s8 anmtrig;
    /* 0xCB */ bool8 anmloop;
    /* 0xCC */ s8 fvaltrig;
    /* 0xCD */ s8 fvalloop;
    /* 0xCE */ s8 svaltrig;
    /* 0xCF */ s8 svalloop;
    /* 0xD0 */ s8 player;
    /* 0xD1 */ s8 anmnum;
    /* 0xD2 */ s8 anmnumbak;
    /* 0xD3 */ s8 anmcnt;
    /* 0xD4 */ s8 type; // Library uchar; keep s8
    /* 0xD5 */ u8 chartptr;
    /* 0xD6 */ u8 stackptr;
    /* 0xD7 */ u8 anmtype;
    /* 0xD8 */ u8 texsizex;
    /* 0xD9 */ u8 texsizey;
    /* 0xDA */ u8 fvalptr;
    /* 0xDB */ u8 svalptr;
    /* 0xDC */ u8 mvtype;
    /* 0xDD */ u8 spptr;
    /* 0xDE */ s8 mappoint;
    /* 0xDF */ u8 unk_0DF;
} AnmObject; // size = 0xE0

extern AnmObject g_DynamicObjects[];

typedef struct {
    /* 0x00 */ f32 scale;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f pos_start;
    /* 0x1C */ Vec3f pos_ende;
    /* 0x28 */ Vec3f pos_spline;
    /* 0x34 */ f32 vel;
    /* 0x38 */ Vec3f velocity;
    /* 0x44 */ f32 ground;
    /* 0x48 */ s32 ival;
    /* 0x4C */ s32 timer;
    /* 0x50 */ s32 anmtm;
    /* 0x54 */ s32 flag;
    /* 0x58 */ s32 evflag;
    /* 0x5C */ s32 evtrflag;
    /* 0x60 */ u8* texaddr;
    /* 0x64 */ u8* indexaddr;
    /**
     * "list" is something of a misnomer for the names here
     * they can be pointers to just 1 tlut/texture, but it is common for one or the other
     * to be a pointer to an array of tluts/textures.
     **/
    /* 0x68 */ u8* stexaddr; // I feel like this should actually be `u8 (*tlutList)[512]`, but that causes mismatches
    /* 0x6C */ u8* sindexaddr;
    /* 0x70 */ Gfx* shape;
    /* 0x74 */ Vtx* vtxaddr;
    /* 0x78 */ s8 unk_078[0x04];
    /* 0x7C */ Vec4s* sptbaddr;
    /* 0x80 */ Vec4s* sptbtopaddr; // unk_080[][4]?
    /* 0x84 */ s16 stack[0xA];
    /* 0x98 */ u16 spstep;
    /* 0x9A */ u16 spstepadd;
    /* 0x9C */ s16 ix;
    /* 0x9E */ s16 iy;
    /* 0xA0 */ s16 alpha;
    /* 0xA2 */ s16 input;
    /* 0xA4 */ s16 output;
    /* 0xA6 */ s16 anmptr; // Usually a state tracker
    /* 0xA8 */ s16 anmptrbak;
    /* 0xAA */ s16 fvaltm;
    /* 0xAC */ s16 svaltm;
    /* 0xAE */ s16 mvptr;
    /* 0xB0 */ s16 mvtm;
    /* 0xB2 */ Vec3su texang; // rotation, I think
    /* 0xB8 */ Vec3su kageang;
    /* 0xBE */ Vec3su ang;
    /* 0xC4 */ u16 addangy;
    /* 0xC6 */ u16 destangy;
    /* 0xC8 */ u16 r;
    /* 0xCA */ s8 anmtrig;
    /* 0xCB */ s8 anmloop;
    /* 0xCC */ s8 fvaltrig;
    /* 0xCD */ s8 fvalloop;
    /* 0xCE */ s8 svaltrig;
    /* 0xCF */ s8 svalloop;
    /* 0xD0 */ s8 player;
    /* 0xD1 */ s8 anmnum;
    /* 0xD2 */ s8 anmnumbak;
    /* 0xD3 */ s8 anmcnt;
    /* 0xD4 */ s8 type;
    /* 0xD5 */ u8 chartptr;
    /* 0xD6 */ u8 stackptr;
    /* 0xD7 */ u8 anmtype;
    /* 0xD8 */ u8 texsizex;
    /* 0xD9 */ u8 texsizey;
    /* 0xDA */ u8 fvalptr;
    /* 0xDB */ u8 svalptr;
    /* 0xDC */ u8 mvtype;
    /* 0xDD */ u8 spptr;
    /* 0xDE */ s8 mappoint;
    /* 0xDF */ u8 unk_0DF;
} ItemWindowObjects; // size = 0xE0

// This are other lists of indices in g_DynamicObjects.

/**
 * @brief Status flags for objects
 */
#define VISIBLE 0x00040000

/**
 * Use unknown. An object is reserved and its index is saved to
 * this variable, but it appears to go unreferenced
 **/
extern s32 D_80183DA0;

//! Lakitu?
extern s32 jugemuallocptr[];

#define DELETED_OBJECT_ID -1
#define NULL_OBJECT_ID -1

//! Appears to be a list of object list indices for the Item Window part of the HUD
extern s32 ItemBoxAllocPtr[];

// Used for loop bounds when accessing objallocptr
extern s16 D_80165750;

/**
 * These seem to be limits on different object types in Moo Moo Farm
 * See kwalloc_effectram_1p in code_8006E9C0.c
 * Maybe max number of active moles in a given group of moles?
 */
extern s32 D_8018D1C8;
extern s32 D_8018D1D0;
extern s32 D_8018D1D8;

//! Limit on some object type (ice chips?) in Sherbet Land
extern s32 D_8018D3C0;

extern Collision D_8018C0B0[];

/**
 * objallocptr, objallocptr2, objallocptr3, and objallocptr4 are all lists of indices in g_DynamicObjects.
 * kwalloc_objram initializes them in such a way that the indicies in each list are not adjacent.
 * First objallocptr gets an unused index, then objallocptr2, then objallocptr3, then objallocptr4, and
 *then it loops.
 *
 * The objects found at the indices in each list appears to be course dependent
 **/

#define NUM_YV_FLAG_POLES 4

// pos, rot
extern s16 D_800E5DF4[];

#define NUM_CRABS 0xA

typedef struct {
    /* 0x0 */ s16 startX;
    /* 0x2 */ s16 patrolX;
    /* 0x4 */ s16 startZ;
    /* 0x6 */ s16 patrolZ;
} CrabSpawn;

extern CrabSpawn gCrabSpawns[];

#define NUM_THWOMPS_50CC 8
#define NUM_THWOMPS_100CC_EXTRA 11
#define NUM_THWOMPS_150CC 12

// pos x,y,z
extern float D_800E6734[];

typedef struct {
    /* 0x0 */ s16 startX;
    /* 0x2 */ s16 startZ;
    /* 0x4 */ s16 unk_4; // Group Id?
    /* 0x6 */ s16 unk_6; // Starting State?
} ThwompSpawn;           // size = 0x8;

extern ThwompSpawn gThomwpSpawns50CC[];
extern ThwompSpawn gThwompSpawns100CCExtra[];
extern ThwompSpawn gThomwpSpawns150CC[];
extern ThwompSpawn* gThowmpSpawnList;

extern s16 gNumActiveThwomps;

#define NUM_NEON_SIGNS 10
#define NUM_CHAIN_CHOMPS 3

#define NUM_PENGUINS 15

/**
 * Snowmen bodies in FrappeSnowland
 * Crabs in Koopa Troopa Beach
 * Hot air balloon in Luigi Raceway?
 * Neon signs in Rainbow Road?
 * Thwomps in Bower's Castle?
 * Penguins in Sherbet Land?
 * Flag Poles in Yoshi Valley?
 */
extern s32 objallocptr[];

#define NUM_SEAGULLS 10
#define NUM_SNOWMEN 19
#define NUM_HEDGEHOGS 15

typedef struct Marker {
    /* 0x0 */ Vec3s Position;
    /* 0x6 */ s16 Group;
} Marker; // size = 0x8
typedef Marker SnowmanSpawn;
typedef Marker HegdehogSpawn;

extern SnowmanSpawn gSnowmanSpawns[];


extern HegdehogSpawn gHedgehogSpawns[];
extern Vec3s gHedgehogPatrolPoints[];

/**
 * Snowmen heads in Frappe Snowland
 * Chain Chomps in RaindbowRoad?
 * Trophy in award ceremony?
 * Seagulls in Koopa Troopa Beach?
 * Hedgehogs in Yoshi Valley?
 * Spawn for big fire breath in Bowser's Castle
 */
extern s32 objallocptr2[];

#define NUM_BOOS 0xA
#define NUM_FIRE_BREATHS 4

extern Vec3s gFireBreathsSpawns[];

// These should really, really be Vec3s arrays, but that doesn't match
extern s16 D_800E5740[];
extern s16 D_800E579C[];
extern s16 D_800E57F8[];

/**
 * Boos in Banshee Boardwalk
 * Spawners for the 4 small fire breaths inside Bowser's Castle
 */
extern s32 objallocptr3[];

/**
 * Unused list of object indices
 */
extern s32 objallocptr4[];

#define gObjectParticle1_SIZE 128
#define NUM_GROUP1_MOLES 8
#define NUM_GROUP2_MOLES 11
#define NUM_GROUP3_MOLES 12
#define NUM_TOTAL_MOLES (NUM_GROUP1_MOLES + NUM_GROUP2_MOLES + NUM_GROUP3_MOLES)
#define NUM_SNOWFLAKES 0x32

typedef union {
    Vec3s asVec3sList[NUM_TOTAL_MOLES];
    s16 asFlatList[NUM_TOTAL_MOLES * 3];
} MoleSpawnUnion;
extern MoleSpawnUnion gMoleSpawns;

/**
 * Exact use unknown, something related to the mole groups
 * Maybe be indicating that a given mole in a given group is active?
 */
extern s8 D_8018D198[];
extern s8 D_8018D1A8[];
extern s8 D_8018D1B8[];

//! Unknown object index, only set for Kalimari Desert, never read
extern s32 D_8018CF10;

/**
 * List of object list indices used for:
 *   Moles in Moo Moo Farm
 *   Snow flakes in Frappe Snowland
 *   Segments of the fire breath from the statues in Bowser's Castle
 *   Potentially other things
 */
extern s32 EffectAllocArray1[];

//! Next free spot in EffectAllocArray1? Wraps back around to 0 if it gets bigger than gObjectParticle1_SIZE
extern s32 gNextFreeObjectParticle1;

//! Used for cycling through snowflakes in kwanm_snow_search_3D
extern s16 D_8018D174;

#define gObjectParticle2_SIZE 128

/**
 * List of object list indices used for:
 *   Bats in Banshee's Boardwalk (but only 1 player mode?)
 */
extern s32 EffectAllocArray2[];

//! Next free spot in EffectAllocArray2? Wraps back around to 0 if it gets bigger than gObjectParticle2_SIZE
extern s32 gNextFreeObjectParticle2;

//! Controls number of come object type placed into EffectAllocArray2 on Frappe Snowland. So, maybe snowmen/snowflakes?
extern s32 D_8018D3BC;

#define gObjectParticle3_SIZE 128
extern s32 EffectAllocArray3[];
//! Next free spot in EffectAllocArray3?
extern s32 gNextFreeObjectParticle3;
extern s16 D_80165730;

//! Tracking a count of some object type, don't know what object type yet
extern s16 D_80165738;

#define gObjectParticle4_SIZE 0x40

#define NUM_TORCHES 8

// This should really be `extern Vec3s CaveFirePos[];`
//! @todo fix this extern
extern s16 CaveFirePos[];

/**
 * List of object list indices. Used both for the fires in the DK Jungle cave
 * and, seemingly for the trail that shells leave behind them.
 * I think they're using the same texture, which would explain the dual use
 */
extern s32 FireParticleAllocArray[];

//! Next free spot in FireParticleAllocArray? Wraps back around to 0 if it gets bigger than gObjectParticle4_SIZE
extern s32 FireParticleCounter;

// Maximum number of leaves that can be falling?
#define gLeafParticle_SIZE 0x40
// Number of leaves to spawn each bonk?
#define gLeafParticle_SPAWN_SIZE 0x14

/**
 * Seemingly a list of object list indices used for the leaves that sometimes fall
 * trees when you bonk into them
 */
extern s32 leafallocptr[];

//! Next free spot in leafallocptr? Wraps back around to 0 if it gets bigger than gLeafParticle_SIZE
extern s32 gNextFreeLeafParticle;

// Struct used for clouds and stars
// See `func_80070328` and `func_80070250` for cloud initialization
// See `func_800704A0` and `func_800703E0` for star initialization
typedef struct {
    // rotY and posY seem relative to the camera.
    // See `kwanm_visiblecheck_kumo` to see how rotY is used to decide whether and where to display clouds/stars
    // See `func_80078A44` and `func_800789AC` to see how stars and clouds (respectively) are looped over
    /* 0x00 */ u16 rotY;
    /* 0x02 */ u16 posY;
    // Can be bigger than 100!
    /* 0x04 */ u16 scalePercent;
    // There are a couple different cloud shapes, this decides which one is used
    // For stars, this is always 0 since they don't have multiple textures
    /* 0x06 */ u16 subType;
} StarData, CloudData; // size = 0x8

#define D_8018CC80_SIZE 0x64

/**
 * List of object list indices used by the clouds and stars in some stages
 * Also used for snowflakes like EffectAllocArray1? Not sure what's up with that
 */
extern s32 g_CloudAllocate[];

//! Number of used spots in g_CloudAllocate?
extern s32 CloudCount1P;

//! Next free spot in g_CloudAllocate?
extern s32 CloudCount2P;

//! Also next free spot in g_CloudAllocate?
extern s16 D_8018D17C;

//! Something related to the display of the clouds/stars?
extern s8 kwhoshisw;

//! Some sort of limiter on how many of some object type can spawn
extern s32 D_8018D3C4;

#endif
