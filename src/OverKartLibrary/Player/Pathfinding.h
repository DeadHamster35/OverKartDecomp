#include "../MainInclude.h"
typedef struct BKPathfinder{
     short	TargetPath, LastPath; //Current and last path indicies
	float	Distance; //Distance from end/beginning of path
     float     MaxSpeed;
	Vector	Target; //Array of floats giving x,y,z Position of Target (e.g. a rival or Flag)
     short     Progression, NearestMarker; //Step along the path
     float	NearestMarkerHeight; //Height of nearest marker (Used for finding falls)
     short     ProgressTimer, ShootingTimer;
     char      Direction, PathType; //Direction along the path, +1 or -1, if the Start of the path is IDed, will be +1, if the end of the path is IDed, will be -1     
     char     Status, RivalPlayer;
     bool		SlowDown; //Boolean denoting if bot should slow down near end of path or not



	
} BKPathfinder;



typedef struct OKAIHeader
{
     Uint CoursePaths; //Uint array of segmented addresses 0x4
     Uint CourseRamps;
     Uint CourseDrops;
     Uint PathLengths; //short array of path lengths 0x2
     Uint RampLengths;
     Uint DropLengths;
     Ushort TotalPaths, TotalRamps, TotalDrops;
} OKAIHeader;



extern short ItemBoxCount;

extern bool PathfinderComplete(BKPathfinder *Pathfinder, short *PathLengths, short *RampLengths, short *DropLengths);

extern BKPathfinder AIPathfinder[4];

extern void GetItemBoxIndexes(void);

extern int FindNearestRampNode(float CurrentPosition[],  float FoundNodePosition[], float TargetY, Marker *PathArray[], short* MarkerCounts, short PathCount, float HeightCheckSquared);

extern int FindNearestDropNode(float CurrentPosition[],  float FoundNodePosition[], float TargetY, Marker *PathArray[], short* MarkerCounts, short PathCount, float HeightCheckSquared);

extern int FindNearestItemBox(float CurrentPosition[], float FoundItemBoxPosition[], float HeightCheckSquared, bool ignoreHeightCheck, short courseType);

extern void FindFurthestNode(float CurrentPosition[],  float FoundNodePosition[], Marker *PathArray[], short* MarkerCounts, short PathCount);


extern void UpdateRacePath(BKPathfinder* Pathfinder, short FirstMarkerDistance, Marker *PathArray[], short* MarkerCounts, short PathCount, short PlayerID);
extern void UpdateBKPath(BKPathfinder *Pathfinder, short FirstMarkerDistance, Marker *PathArray[], short* MarkerCounts, short PathCount, short PlayerID, char TypeOfPath, float HeightCheckSquared);

extern void ResetLaneDefaultOffset(int PlayerID);

extern void ChooseAlternativePathRandm(int PlayerID, short pointCheckMin, short pointCheckMax, Ushort maxPathIDs);

extern void ChangePathAndPoint(int PlayerID, short pointCheckMin, short pointCheckMax, Ushort currentpathID, Ushort newpathID);


extern void MakeAlternativePath(Marker *altPath, short Length, char pathID);




#define RAMPDISTANCESQUARE 40000.0
#define PATHDISTANCECHECK 2.0

#define FLATPATH     0
#define RAMPPATH     1
#define DROPPATH     2

#define SEEKER_AI     0
#define RANDOM_AI       1

#define STOPTURN ((DEG1 * 60) * (DEG1 * 60))
#define DRIFTTURN ((DEG1 * 40) * (DEG1 * 40))
#define WIDETURN ((DEG1 * 25) * (DEG1 * 25))
#define MIDTURN ((DEG1 * 12) * (DEG1 * 12))

#define SHORTTURN ((DEG1 * 5) * (DEG1 * 5))
#define NOTURN ((DEG1 * 2) * (DEG1 * 2))
