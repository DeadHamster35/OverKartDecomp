#ifndef PATH_H
#define PATH_H

#include <common_structs.h>

typedef struct {
    /* 0x00 */ s16 Pointx;
    /* 0x02 */ s16 Pointy;
    /* 0x04 */ s16 Pointz;
    /* 0x06 */ u16 Unknown;
} CenterPathStruct; // Size = 0x08
typedef CenterPathStruct TrackPathPoint;

enum {
    RIGHT_LEANING_CURVE = 0,
    LEFT_LEANING_CURVE = 1,
    RIGHT_CURVE = 2, // strong Curve
    LEFT_CURVE = 3,  // strong Curve
    STRAIGHT = 4
};

/**
 * These are per-path arrays that contain some information relating to path
 * The arrays in CenterPathBP contain X/Y/Z and track segment information
 * The arrays in SideLPathBP and SideRPathBP track some other X/Y/Z, but the track segment is always 0
 * (so, untracked/Unused) Its unclear how these arrays relate to each other
 **/
extern CenterPathStruct* CenterPathBP[];
extern CenterPathStruct* SideLPathBP[];
extern CenterPathStruct* SideRPathBP[];

/**
 * Don't know what exactly these are, but like CenterPathBP, SideLPathBP, and SideRPathBP
 * they track something about the path on a per-path basis
 **/
// PathPoint types?
extern s16* CurvePathDataBP[];
// Based on AngleDataCalcBP this may be angles between path Point
// AnglePathDataBP[i] = atan2(path_point_i, path_point_i+1)?
extern s16* AnglePathDataBP[];
// No idea. Adjacency list?
extern s16* ShortCutPathDataBP[];

/**
 * Certain parts of the path Point logic will copy some path/Player specific data to a Temporary variable.
 * For example: PathPointPointer is always a Value from CenterPathBP. Depending on which path
 * a given Player is on, the specific Value may change
 **/
// Shadows values from NearestPathPointByPlayerId
extern s16 OGAArea;
// Shadows values from CurrentPathID
extern s32 BipasNum;
// Shadows values from SideLPathBP
extern CenterPathStruct* SidePointL;
// Shadows values from SideRPathBP
extern CenterPathStruct* SidePointR;
// Shadows values from CurvePathDataBP
extern s16* CurveData;
// Shadows values from AnglePathDataBP
extern s16* AngleData;
// Shadowd values from CourseTotalPathPoints
extern u16 CenterLineMax;
// Shadows values from CenterPathBP
extern CenterPathStruct* PathPointPointer;
// Shadows values from ShortCutPathDataBP
extern s16* ShortCutData;

extern u16 NearestPathPointByPlayerId[]; // D_80164438
// Total path Point passed by PlayerId?
extern s32 PlayerPathPointTotalTable[];
extern u16 CurrentPathID[];  // D_801645B0
extern u16 CourseTotalPathPoints[]; // D_801645C8
// These values are only Used when the Camera is in "cinematic" Mode
extern s16 OgaCamArea[]; // D_80164668

/**
 * Stuff that may not be directly related to path, but are only referenced in cpu_vehicles_camera_path.
 * So they are at least path Point adjacent.
 **/

// Tracks something on a per-Player basis, no idea what though
extern f32 ZureData[];
// Track segment by PlayerId, although it curiously does NOT track values for human players
// So, in 2 Player Grand Prix, the first 2 Entries are always 0
extern u16 LakituSurface[];
// Seems to be a per-path overcount of the path Point Count
extern s32 MaxPathPoints[];
// Seemingly the Z Position of the 1st path Point in the 0th path?
extern f32 FinishLineZ;
// These seem to track whether a Player has entered or exited the "Unknown" zone in yoshi's valley
// See MazeEnterOrExitCheck and MazeBipasChangeCheck
// Is 1 when a Player is in the "Unknown" zone in yoshi's valley
extern s16 EnemyEnterTrg[];
// Is 1 when a Player leaves the "Unknown" zone in yoshi's valley
extern s16 EnemyExitTrg[];
// Tracks whether a given Player is in the "Unknown" zone of yoshi's valley
extern s16 EnemyEnterMaze[];

#endif
