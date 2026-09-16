#ifndef PATH_H
#define PATH_H

#include <common_structs.h>

typedef struct {
    /* 0x00 */ s16 pointx;
    /* 0x02 */ s16 pointy;
    /* 0x04 */ s16 pointz;
    /* 0x06 */ u16 unknown;
} CenterPathStruct; // size = 0x08
typedef CenterPathStruct TrackPathPoint;

enum {
    RIGHT_LEANING_CURVE = 0,
    LEFT_LEANING_CURVE = 1,
    RIGHT_CURVE = 2, // strong curve
    LEFT_CURVE = 3,  // strong curve
    STRAIGHT = 4
};

/**
 * These are per-path arrays that contain some information relating to path
 * The arrays in CenterPathBP contain X/Y/Z and track segment information
 * The arrays in SideLPathBP and SideRPathBP track some other X/Y/Z, but the track segment is always 0
 * (so, untracked/unused) Its unclear how these arrays relate to each other
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
// Based on AngleDataCalcBP this may be angles between path point
// AnglePathDataBP[i] = atan2(path_point_i, path_point_i+1)?
extern s16* AnglePathDataBP[];
// No idea. Adjacency list?
extern s16* ShortCutPathDataBP[];

/**
 * Certain parts of the path point logic will copy some path/player specific data to a temporary variable.
 * For example: g_pathPointPointer is always a value from CenterPathBP. Depending on which path
 * a given player is on, the specific value may change
 **/
// Shadows values from gNearestPathPointByPlayerId
extern s16 OGAArea;
// Shadows values from CurrentPathID
extern s32 bipas_num;
// Shadows values from SideLPathBP
extern CenterPathStruct* side_point_L;
// Shadows values from SideRPathBP
extern CenterPathStruct* side_point_R;
// Shadows values from CurvePathDataBP
extern s16* curve_data;
// Shadows values from AnglePathDataBP
extern s16* angle_data;
// Shadowd values from g_courseTotalPathPoints
extern u16 center_line_max;
// Shadows values from CenterPathBP
extern CenterPathStruct* g_pathPointPointer;
// Shadows values from ShortCutPathDataBP
extern s16* short_cut_data;

extern u16 gNearestPathPointByPlayerId[]; // D_80164438
// Total path point passed by playerId?
extern s32 g_playerPathPointTotalTable[];
extern u16 CurrentPathID[];  // D_801645B0
extern u16 g_courseTotalPathPoints[]; // D_801645C8
// These values are only used when the camera is in "cinematic" mode
extern s16 oga_cam_area[]; // D_80164668

/**
 * Stuff that may not be directly related to path, but are only referenced in cpu_vehicles_camera_path.
 * So they are at least path point adjacent.
 **/

// Tracks something on a per-player basis, no idea what though
extern f32 zure_data[];
// Track segment by playerId, although it curiously does NOT track values for human players
// So, in 2 Player Grand Prix, the first 2 entries are always 0
extern u16 g_lakituSurface[];
// Seems to be a per-path overcount of the path point count
extern s32 MaxPathPoints[];
// Seemingly the Z position of the 1st path point in the 0th path?
extern f32 g_finishLineZ;
// These seem to track whether a player has entered or exited the "unknown" zone in yoshi's valley
// See maze_enter_or_exit_check and maze_bipas_change_check
// Is 1 when a player is in the "unknown" zone in yoshi's valley
extern s16 enemy_enter_trg[];
// Is 1 when a player leaves the "unknown" zone in yoshi's valley
extern s16 enemy_exit_trg[];
// Tracks whether a given player is in the "unknown" zone of yoshi's valley
extern s16 enemy_enter_maze[];

#endif
