#ifndef COURSE_H
#define COURSE_H

#include <ultra64.h>
#include <PR/gbi.h>
#include <macros.h>
#include "path.h"

/**
 * @file Include for course gfx.inc.c.
 */

typedef struct {
    Gfx* Address;
    u8 Status;
    u8 Area;
    u16 Flag;
} SEARCHLIST;
typedef SEARCHLIST TrackSections;

struct _struct_gCoursePathSizes_0x10 {
    /* 0x00 */ u16 FirstPath;
    /* 0x02 */ u16 SecondPath;
    /* 0x04 */ u16 ThirdPath;
    /* 0x06 */ u16 FourthPath;
    /* 0x08 */ u16 Unk8;
    /* 0x0A */ char PadA[6];
}; // Size 0x10

#if !ENABLE_CUSTOM_COURSE_ENGINE
typedef enum {
    /* 0x00 */ COURSE_MARIO_RACEWAY = 0,
    /* 0x01 */ COURSE_CHOCO_MOUNTAIN,
    /* 0x02 */ COURSE_BOWSER_CASTLE,
    /* 0x03 */ COURSE_BANSHEE_BOARDWALK,
    /* 0x04 */ COURSE_YOSHI_VALLEY,
    /* 0x05 */ COURSE_FRAPPE_SNOWLAND,
    /* 0x06 */ COURSE_KOOPA_BEACH,
    /* 0x07 */ COURSE_ROYAL_RACEWAY,
    /* 0x08 */ COURSE_LUIGI_RACEWAY,
    /* 0x09 */ COURSE_MOO_MOO_FARM,
    /* 0x0A */ COURSE_TOADS_TURNPIKE,
    /* 0x0B */ COURSE_KALAMARI_DESERT,
    /* 0x0C */ COURSE_SHERBET_LAND,
    /* 0x0D */ COURSE_RAINBOW_ROAD,
    /* 0x0E */ COURSE_WARIO_STADIUM,
    /* 0x0F */ COURSE_BLOCK_FORT,
    /* 0x10 */ COURSE_SKYSCRAPER,
    /* 0x11 */ COURSE_DOUBLE_DECK,
    /* 0x12 */ COURSE_DK_JUNGLE,
    /* 0x13 */ COURSE_BIG_DONUT,
    /* 0x14 */ COURSE_AWARD_CEREMONY,
    /* 0x15 */ NUM_COURSES
} COURSES;

#else

#define COURSE_MARIO_RACEWAY
#define COURSE_CHOCO_MOUNTAIN
#define COURSE_BOWSER_CASTLE
#define COURSE_BANSHEE_BOARDWALK
#define COURSE_YOSHI_VALLEY
#define COURSE_FRAPPE_SNOWLAND
#define COURSE_KOOPA_BEACH
#define COURSE_ROYAL_RACEWAY
#define COURSE_LUIGI_RACEWAY
#define COURSE_MOO_MOO_FARM
#define COURSE_TOADS_TURNPIKE
#define COURSE_KALAMARI_DESERT
#define COURSE_SHERBET_LAND
#define COURSE_RAINBOW_ROAD
#define COURSE_WARIO_STADIUM
#define COURSE_BLOCK_FORT
#define COURSE_SKYSCRAPER
#define COURSE_DOUBLE_DECK
#define COURSE_DK_JUNGLE
#define COURSE_BIG_DONUT
#define COURSE_AWARD_CEREMONY
#define NUM_COURSES

#endif

#if !ENABLE_CUSTOM_COURSE_ENGINE
extern s16 CourseID;
extern s16* DistanceLimmitPtr[];
#define GET_COURSE_800DCBB4(n) DistanceLimmitPtr[CourseID][n]
extern f32 RoadWidth[];
#define GET_COURSE_AIMaximumSeparation RoadWidth[CourseID]
extern struct _struct_gCoursePathSizes_0x10 PathLengthTable[];
#define GET_COURSE_PathSizes PathLengthTable[CourseID]
extern s16 HandleMaxTab[];
#define GET_COURSE_AISteeringSensitivity HandleMaxTab[CourseID]
extern f32 SoukouZureMax[];
#define GET_COURSE_AIMinimumSeparation SoukouZureMax[CourseID]
extern CenterPathStruct* PathTableB[][4];
#define GET_COURSE_PathTable(p) GetVAddress(PathTableB[CourseID][p])
extern CenterPathStruct* PathTable[][4];
#define GET_COURSE_PathTable2(p) GetVAddress(PathTable[CourseID][p])
extern ActionData* CPUBehaviourLUT[];
#define GET_COURSE_AIBehaviour GetVAddress(CPUBehaviourLUT[i])
extern char* StringTableCourseGP[];
#define GET_COURSE_Name StringTableCourseGP[CourseID]
extern char* StringTableCourse[];
#define GET_COURSE_NameDup StringTableCourse[CupCourseOrder[CupSelect][CourseSelect]]
extern char* CircuitNameShort[];
#define GET_COURSE_debugName CircuitNameShort[CourseID]
extern f32 FarClip;
#define FarClip FarClip
extern f32 ScreenNearPoint;
#define ScreenNearPoint ScreenNearPoint
#define GET_COURSE_D_0D0096B8(cc) *(f32*) GetVAddress(&D_0D0096B8[CourseID][cc])
#define GET_COURSE_cpu_OffTrackTargetSpeed(cc) \
    *(f32*) GetVAddress(&cpu_OffTrackTargetSpeed[CourseID][cc])
#define GET_COURSE_cpu_CurveTargetSpeed(cc) \
    *(f32*) GetVAddress(&cpu_CurveTargetSpeed[CourseID][cc])
#define GET_COURSE_cpu_NormalTargetSpeed(cc) \
    *(f32*) GetVAddress(&cpu_NormalTargetSpeed[CourseID][cc])
#else
#define CourseID
#define GET_COURSE_800DCBB4(n)
#define GET_COURSE_AIMaximumSeparation
#define GET_COURSE_PathSizes
#define GET_COURSE_AISteeringSensitivity
#define GET_COURSE_AIMinimumSeparation
#define GET_COURSE_PathTable(p)
#define GET_COURSE_PathTable2(p)
#define GET_COURSE_AIBehaviour
#define GET_COURSE_Name
#define GET_COURSE_NameDup
#define GET_COURSE_debugName
#define FarClip
#define ScreenNearPoint
#define GET_COURSE_D_0D0096B8(cc)
#define GET_COURSE_cpu_OffTrackTargetSpeed(cc)
#define GET_COURSE_cpu_CurveTargetSpeed(cc)
#define GET_COURSE_cpu_NormalTargetSpeed(cc)
#endif

#endif // COURSE_H
