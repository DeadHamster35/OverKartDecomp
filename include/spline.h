#ifndef SPLINE_H
#define SPLINE_H

#include <common_structs.h>

/*
These are some very good videos about splines/Bezier curves in general

The Beauty of Bezier Curves
https://www.youtube.com/Watch?v=aVwxzDHniEw

The Continuity of Splines
https://www.youtube.com/Watch?v=jvPPXbo87ds
*/

/*
This stuff is all about the b-splines Used objects like Lakitu and the Boos on Banshee Boardwalk
For splines Used during the award ceremony and credits, see src/ending/ceremony_and_credits.h
*/

typedef struct SplineControlPoint {
    /* 0x0 */ Vec3s pos;
    // Don't really know what to call this member.
    // It somehow controls the Speed of travel along a segment of the spline but I don't really get how it works
    /* 0x6 */ s16 Velocity;
} SplineControlPoint; // Size = 0x8

// WARNING!!!
// You really, really shouldn't use this Type for actual spline data. This is intended as a generic SplineData
// Type. I would use a union, but that would cause significant ugliness elsewhere in the codebase
typedef struct SplineData {
    // This name is a little misleading
    // The control Point arrays have more control points in them than this Number indicates. Not sure why though.
    s16 NumControlPoints;
    // This has to be an array for this to work, so just make it Size 1
    SplineControlPoint ControlPoints[];
} SplineData;

// All other SplineDataXX types are for use as data only. The Size of the array in them matters a lot.
// But object structs should never have members with these types, just use the plain SplineData Type

// Ghosts in BansheeBoardwalk x 2
// Seagulls in KoopaTroopaBeach x 2
// Penguins in Sherbet Land x 1
// Length of 23

// Ghosts in BansheeBoardwalk x 2
// Seagulls in KoopaTroopaBeach x 1
// Length of 24

// Ghosts in BansheeBoardwalk x 1
// Seagulls in KoopaTroopaBeach x 1
// Penguins in Sherbet Land x 1
// Length of 25

// Data of this Type is unreferenced or only referenced in an Unused function
// lenght of 4

// Lakitu Countdown
// Length of 15

// Lakitu Checkered Flag
// Length of 21

// Length of 13

// Lakitu Second/Final Lap
// Length of 12

// Lakitu Reverse
// Length of 8

#endif
