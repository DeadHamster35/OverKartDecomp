#ifndef CustomSurfaceH
#define CustomSurfaceH
#include "../MainInclude.h"
//extern void GetSurfaceID();

extern void LavaFloorRecoil(void);
extern void InteractLavaFloor(Bump* bump, ushort pointer, int FlagAxis);
extern float absFloat(float value);
extern int LavaFloorBumpCount[8];
extern bool LavaFloorRecoiling[8];
void CheckDirectionFunc(int Null, Screen *display);
void ExplorerSetJugemu(Player* Target);
void ExplorerCallJugemu(Player* Target);
void ExplorerHangJugemu(Player* Target, char Index, char place);
void ExplorerKWDisplayJugemu(int Player);
extern void custom_SpinKart(Player* Car, Camera* camera, char place, char kno);
extern void SurfaceSFX(Player *car, int SFX_ID, float min_Speed);



extern int LavaFloorBumpCount[];
extern bool LavaFloorRecoilRequired[];
extern bool LavaFloorRecoiling[];


#define TRICK_GRAVITY				4000.0f
#define TRICK_TRIGGER_SPEED_MIN		30

#define GAP_GRAVITY				2000.0f

#define IS_BROKEN (IS_SPINNING_OUT|SPINOUT_LEFT|SPINOUT_RIGHT|IS_MOMENTUM_HIT|IS_VERTICAL_HIT|IS_TUMBLING|IS_WALL_TUMBLING)



#define NOTHING 		0
#define ASPHALT 		1
#define DIRT			2
#define SAND			3
#define STONE			4
#define SNOW                    5
#define WOOD			6
#define SAND_TRAP		7
#define GRASS			8
#define ICE  			9
#define WET_SAND		10
#define WET_SNOW                11
#define ROCK                    12
#define DIRT_TRAP               13
#define RAILWAY                 14
#define CAVE                    15
#define BRIDGE                  16   //rickety
#define KIBASHI                 17




#define DASHMARK2		252	    //for JUNGLE
#define JUMPMARK		253	    //for JUNGLE
#define DASHMARK		254	    //for PEACH CIRCUIT
#define OTHER 			255



#define FastOoB			251
#define Water			250
#define MushroomBoost	249
#define FeatherJump		248
#define TornadoJump		247
#define SpinOutSaveable	246
#define SpinOut			245
#define FailedStart		244
#define GreenShellHit	243
#define RedShellHit		242
#define ObjectHit		241
#define Shrunken		240
#define StarMan			239
#define BooSurface    	238
#define GetItem			237
#define TrickJump		236
#define GapJump			235
#define LavaSurface		234
#define ForceJump		233

#define Mud				18

/* Amped Up extra surface IDs. Numbers do not overlap the Tarmac table.
   Do not #define Boo — OKExternal has extern long Boo; host uses BooSurface. */
#define TrickJump2		150
#define TrickJump3		151
#define DSMud			149
#define CarnivalFan		148

#define STORE_NONE 	0
#define STORE_TRICK 1
#define	STORE_GAP	2

/* Sound IDs for SurfaceSFX. Not Tarmac surface types. */
#define SFX_STOP_ALL			21

#define SFX_SLIP				1
#define SFX_DRIFT				2
#define SFX_MOTOR				3

#define SFX_DIRT_TRAP_STEER		13
#define SFX_SAND_TRAP_STEER		14
#define SFX_STONE				15
#define SFX_SNOW				16
#define SFX_WOOD_STEER			17
#define SFX_SPINOUT_ONCE		18
#define SFX_SPINOUT				19
#define SFX_ICE					20
#define SFX_WOOD_BRIDGE			24
#define SFX_WOOD_BRIDGE2		25
#define SFX_RAILWAY				26
#define SFX_BUBBLES				27

#define SFX_GRASS_CENTER		4
#define SFX_GRASS_LEFT			5
#define SFX_GRASS_RIGHT			6
#define SFX_WET_SAND_CENTER		7
#define SFX_WET_SAND_LEFT		8
#define SFX_WET_SAND_RIGHT		9
#define SFX_SAND_CENTER			10
#define SFX_SAND_LEFT			11
#define SFX_SAND_RIGHT			12
#define SFX_DIRT_CENTER			22
#define SFX_DIRT_LEFT			23
#define SFX_DIRT_RIGHT			24
#define SFX_WET_SNOW_CENTER		28
#define SFX_WET_SNOW_LEFT		29
#define SFX_WET_SNOW_RIGHT		30


#endif
