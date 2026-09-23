#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <path.h>
#include <course.h>
#include "main.h"
#include "cpu_vehicles_camera_path.h"

bool StraightChk(UNUSED s32 arg0, u16 PathPointIndex) {
    s16 thing = ShortCutData[PathPointIndex];
    if (thing > 0) {
        return true;
    }
    return false;
}

bool CourseOutChk(s32 playerIndex) {
    f32 Value = ZureData[playerIndex];
    if ((1.1f <= Value) || (Value <= -1.1f)) {
        return true;
    }
    return false;
}

/**
 * Calculates a factor representing where the Player is positioned between Left and Right track boundaries
 * Returns a Value between -1.0 and 1.0:
 * -1.0 = On the Left boundary
 *  0.0 = In the middle of the track
 *  1.0 = On the Right boundary
 *
 * @Param PosX Player's X Position
 * @Param PosZ Player's Z Position
 * @Param PathPointIndex Current pathPoint Index
 * @Param PathIndex Current path/track Index
 * @return Position factor between track boundaries
 */
f32 ZureCenterSubBP(f32 PosX, f32 PosZ, u16 PathPointIndex, s32 PathIndex) {
    f32 leftX;
    f32 leftZ;
    f32 rightX;
    f32 rightZ;
    f32 boundarySquaredDistance;
    f32 positionFactor;
    CenterPathStruct* leftPathPoint;
    CenterPathStruct* rightPathPoint;

    leftPathPoint = &SideLPathBP[PathIndex][PathPointIndex];
    rightPathPoint = &SideRPathBP[PathIndex][PathPointIndex];

    leftX = leftPathPoint->Pointx;
    leftZ = leftPathPoint->Pointz;
    rightX = rightPathPoint->Pointx;
    rightZ = rightPathPoint->Pointz;

    boundarySquaredDistance = ((rightX - leftX) * (rightX - leftX)) + ((rightZ - leftZ) * (rightZ - leftZ));

    // Avoid division by zero for very close or identical boundary points
    if (boundarySquaredDistance < 0.01f) {
        return 0.0f;
    }
    // Calculate normalized Position factor using vector projection
    // Formula: 2 * (dot product of vectors) / (squared magnitude) - 1
    // This maps the Position to a -1 to 1 range
    positionFactor =
        ((2.0f * ((rightX - leftX) * (PosX - leftX) + (rightZ - leftZ) * (PosZ - leftZ))) / boundarySquaredDistance) -
        1.0f;
    return positionFactor;
}

void ZureChkBP(s32 PlayerId, u16 PathPointIndex, s32 PathIndex) {
    UNUSED Vec3f pad;
    ZureData[PlayerId] = 0.0f;
    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        if ((GamePlayers[PlayerId].Flag & EXISTS) != 0) {
            ZureData[PlayerId] = ZureCenterSubBP(
                GamePlayers[PlayerId].Position[0], GamePlayers[PlayerId].Position[2], PathPointIndex, PathIndex);
        }
    }
}

void ObjCalcFromZureDistBP(u16 PathPointIndex, f32 lerpFactor, f32 offsetDistance, s16 PathIndex) {
    UNUSED s32 pad[4];
    f32 pathPointOneX;
    f32 pathPointOneZ;
    f32 pathPointTwoX;
    f32 pathPointTwoZ;
    UNUSED s32 Pad2;
    f32 xdiff;
    f32 zdiff;
    f32 segmentLength;
    UNUSED f32 temp_f12;
    UNUSED f32 temp_f2_2;
    UNUSED CenterPathStruct* path;
    CenterPathStruct* pathPointTwo;
    CenterPathStruct* pathPointOne;

    pathPointOne = &CenterPathBP[PathIndex][PathPointIndex];
    pathPointOneX = pathPointOne->Pointx;
    pathPointOneZ = pathPointOne->Pointz;
    pathPointTwo = &CenterPathBP[PathIndex][(PathPointIndex + 1) % CenterLineMax];
    pathPointTwoX = pathPointTwo->Pointx;
    pathPointTwoZ = pathPointTwo->Pointz;

    // Calculate vector between path Point
    zdiff = pathPointTwoZ - pathPointOneZ;
    xdiff = pathPointTwoX - pathPointOneX;
    if (xdiff && xdiff) {}

    segmentLength = sqrtf((xdiff * xdiff) + (zdiff * zdiff));
    if (segmentLength < 0.01f) {
        object[0] = pathPointTwoX;
        object[2] = pathPointTwoZ;
    } else {
        object[0] =
            ((0.5f - (lerpFactor * 0.5f)) * (((offsetDistance * zdiff) / segmentLength) + pathPointOneX)) +
            ((1.0f - (0.5f - (lerpFactor * 0.5f))) * (((offsetDistance * -zdiff) / segmentLength) + pathPointOneX));
        object[2] =
            ((0.5f - (lerpFactor * 0.5f)) * (((offsetDistance * -xdiff) / segmentLength) + pathPointOneZ)) +
            ((1.0f - (0.5f - (lerpFactor * 0.5f))) * (((offsetDistance * xdiff) / segmentLength) + pathPointOneZ));
    }
}

/**
 * Calculates an interpolated Position between Left and Right track paths.
 *
 * @Param currentPathPoint Index of the Current pathPoint
 * @Param trackOffset Value between 0.0 and 1.0 determining Position between Left (0.0) and Right (1.0) path
 * @Param PathIndex Index of the track/path segment
 */
void ObjCalcFromZureBP(u16 PathPointIndex, f32 trackOffset, s16 PathIndex) {
    CenterPathStruct* Path1;
    CenterPathStruct* Path2;
    f32 x1;
    f32 z1;
    f32 x3;
    f32 z3;
    f32 x2;
    f32 z2;
    f32 x4;
    f32 z4;
    f32 temp_f0;
    f32 temp_f12;

    Path1 = &SideLPathBP[PathIndex][PathPointIndex];
    Path2 = &SideRPathBP[PathIndex][PathPointIndex];

    x1 = (f32) Path1->Pointx;
    z1 = (f32) Path1->Pointz;

    x2 = (f32) Path2->Pointx;
    z2 = (f32) Path2->Pointz;

    PathPointIndex += 1;
    PathPointIndex = PathPointIndex % CourseTotalPathPoints[PathIndex];

    Path1 = &SideLPathBP[PathIndex][PathPointIndex];
    Path2 = &SideRPathBP[PathIndex][PathPointIndex];

    x3 = (f32) Path1->Pointx;
    z3 = (f32) Path1->Pointz;

    x4 = (f32) Path2->Pointx;
    z4 = (f32) Path2->Pointz;

    temp_f0 = 0.5f - (trackOffset / 2.0f);
    temp_f12 = 1.0f - temp_f0;
    object[0] = ((temp_f0 * (x1 + x3)) / 2.0f) + ((temp_f12 * (x2 + x4)) / 2.0f);
    object[2] = ((temp_f0 * (z1 + z3)) / 2.0f) + ((temp_f12 * (z2 + z4)) / 2.0f);
}

s16 CheckOgaAreaBP(f32 PosX, f32 PosY, f32 PosZ, s32 PathIndex) {
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 considerSquaredDistance;
    f32 minimumSquaredDistance;
    s32 considerPathPointIndex;
    s32 pathPathPointCount;
    s16 nearestPathPointIndex;
    CenterPathStruct* pathPathPoints;
    CenterPathStruct* considerPathPoint;

    pathPathPoints = CenterPathBP[PathIndex];
    pathPathPointCount = CourseTotalPathPoints[PathIndex];
    considerPathPoint = &pathPathPoints[0];
    x_dist = (f32) considerPathPoint->Pointx - PosX;
    y_dist = (f32) considerPathPoint->Pointy - PosY;
    z_dist = (f32) considerPathPoint->Pointz - PosZ;
    minimumSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
    nearestPathPointIndex = 0;
    for (considerPathPointIndex = 1; considerPathPointIndex < pathPathPointCount;
         considerPathPoint++, considerPathPointIndex++) {
        x_dist = (f32) considerPathPoint->Pointx - PosX;
        y_dist = (f32) considerPathPoint->Pointy - PosY;
        z_dist = (f32) considerPathPoint->Pointz - PosZ;
        considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        if (considerSquaredDistance < minimumSquaredDistance) {
            nearestPathPointIndex = considerPathPointIndex;
            minimumSquaredDistance = considerSquaredDistance;
        }
    }
    return nearestPathPointIndex;
}

s16 CalcOGAAreaSubBP(f32 PosX, f32 PosY, f32 PosZ, u16 trackSectionId, s32* PathIndex) {
    CenterPathStruct* pathPathPoints;
    CenterPathStruct* considerPathPoint;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 considerSquaredDistance;
    f32 minimumSquaredDistance;
    s32 considerPathPointIndex;
    s32 pathPathPointCount;
    s32 temp_t0;
    s32 var_a1;
    s32 var_t1;
    s32 considerPathIndex;
    s32 var_t4;
    s16 nearestPathPointIndex;

    minimumSquaredDistance = 1000000.0f;
    temp_t0 = *PathIndex;
    nearestPathPointIndex = 0;
    var_t1 = 0;
    var_a1 = 0;
    pathPathPoints = CenterPathBP[temp_t0];
    pathPathPointCount = CourseTotalPathPoints[temp_t0];
    considerPathPoint = &pathPathPoints[0];
    for (considerPathPointIndex = 0; considerPathPointIndex < pathPathPointCount;
         considerPathPointIndex++, considerPathPoint++) {
        if ((considerPathPoint->Unknown == trackSectionId) || (CourseID == COURSE_AWARD_CEREMONY)) {
            var_t1 = 1;
            x_dist = (f32) considerPathPoint->Pointx - PosX;
            y_dist = (f32) considerPathPoint->Pointy - PosY;
            z_dist = (f32) considerPathPoint->Pointz - PosZ;
            considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
            if (considerSquaredDistance < minimumSquaredDistance) {
                nearestPathPointIndex = considerPathPointIndex;
                var_a1 = 1;
                minimumSquaredDistance = considerSquaredDistance;
            }
        }
    }
    if (var_t1 == 0) {
        for (considerPathIndex = 0; considerPathIndex < 4; considerPathIndex++) {
            if ((considerPathIndex != temp_t0) && (MaxPathPoints[considerPathIndex] >= 2)) {
                pathPathPoints = CenterPathBP[considerPathIndex];
                considerPathPoint = &pathPathPoints[0];
                pathPathPointCount = CourseTotalPathPoints[considerPathIndex];
                for (considerPathPointIndex = 0; considerPathPointIndex < pathPathPointCount;
                     considerPathPointIndex++, considerPathPoint++) {
                    if (considerPathPoint->Unknown == trackSectionId) {
                        x_dist = (f32) considerPathPoint->Pointx - PosX;
                        y_dist = (f32) considerPathPoint->Pointy - PosY;
                        z_dist = (f32) considerPathPoint->Pointz - PosZ;
                        considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
                        if (considerSquaredDistance < minimumSquaredDistance) {
                            nearestPathPointIndex = considerPathPointIndex;
                            var_t4 = considerPathIndex;
                            var_a1 = 2;
                            minimumSquaredDistance = considerSquaredDistance;
                        }
                    }
                }
            }
        }
    }
    if (var_a1 == 0) {
        pathPathPoints = CenterPathBP[0];
        pathPathPointCount = CourseTotalPathPoints[0];
        considerPathPoint = &pathPathPoints[0];
        x_dist = (f32) considerPathPoint->Pointx - PosX;
        y_dist = (f32) considerPathPoint->Pointy - PosY;
        z_dist = (f32) considerPathPoint->Pointz - PosZ;
        minimumSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        nearestPathPointIndex = 0;
        for (considerPathPointIndex = 1; considerPathPointIndex < pathPathPointCount;
             considerPathPoint++, considerPathPointIndex++) {
            x_dist = (f32) considerPathPoint->Pointx - PosX;
            y_dist = (f32) considerPathPoint->Pointy - PosY;
            z_dist = (f32) considerPathPoint->Pointz - PosZ;
            considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
            if (considerSquaredDistance < minimumSquaredDistance) {
                nearestPathPointIndex = considerPathPointIndex;
                var_t4 = 0;
                var_a1 = 2;
                minimumSquaredDistance = considerSquaredDistance;
            }
        }
    }
    if (var_a1 == 2) {
        *PathIndex = var_t4;
    }
    return nearestPathPointIndex;
}

/**
 * Tries to find the path Point nearest to (PosX, PosY, PosZ)
 * Only consider path in the same segment as trackSectionId
 * Only considers path within 500 units of(PosX, PosY, PosZ)
 * Looks 3 path behind and 6 path ahead of PathPointIndex
 **/
s16 GetOgaAreaGroupSubBP(f32 PosX, f32 PosY, f32 PosZ, s16 PathPointIndex, s32 PathIndex, u16 trackSectionId) {
    s16 nearestPathPointIndex;
    s16 searchIndex;
    s16 considerIndex;
    s32 pathPathPointCount;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 minimumDistance;
    f32 squaredDistance;
    CenterPathStruct* pathPathPoints;
    CenterPathStruct* considerPathPoint;

    nearestPathPointIndex = -1;
    minimumDistance = 500.0f * 500.0f;
    pathPathPointCount = CourseTotalPathPoints[PathIndex];
    pathPathPoints = CenterPathBP[PathIndex];
    for (searchIndex = PathPointIndex - 3; searchIndex < PathPointIndex + 7; searchIndex++) {
        // Its possible for searchIndex to be less than 0 or greater than the Number of path in a given path
        // This is done to ensure we access CenterPathBP at a valid Index
        considerIndex = (searchIndex + pathPathPointCount) % pathPathPointCount;
        considerPathPoint = &pathPathPoints[considerIndex];
        if (considerPathPoint->Unknown == trackSectionId) {
            x_dist = considerPathPoint->Pointx - PosX;
            y_dist = considerPathPoint->Pointy - PosY;
            z_dist = considerPathPoint->Pointz - PosZ;
            squaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
            if (squaredDistance < minimumDistance) {
                minimumDistance = squaredDistance;
                nearestPathPointIndex = considerIndex;
            }
        }
    }
    return nearestPathPointIndex;
}

/**
 * Tries to find the path Point nearest to (PosX, PosY, PosZ)
 * Only considers path within 400 units of (PosX, PosY, PosZ)
 * Looks 3 path behind and 6 path ahead of PathPointIndex
 **/
s16 GetOgaAreaSubBP(f32 PosX, f32 PosY, f32 PosZ, s16 PathPointIndex, s32 PathIndex) {
    s16 nearestPathPointIndex;
    s16 searchIndex;
    s16 considerIndex;
    bool pathPointFound;
    s32 pathPathPointCount;
    f32 x_dist;
    f32 y_dist;
    f32 z_dist;
    f32 minimumDistance;
    f32 squaredDistance;
    CenterPathStruct* pathPathPoints;
    CenterPathStruct* considerPathPoint;

    pathPointFound = false;
    nearestPathPointIndex = -1;
    minimumDistance = 400.0f * 400.0f;
    pathPathPointCount = CourseTotalPathPoints[PathIndex];
    pathPathPoints = CenterPathBP[PathIndex];
    for (searchIndex = PathPointIndex - 3; searchIndex < PathPointIndex + 7; searchIndex++) {
        // Its possible for searchIndex to be less than 0 or greater than the Number of path in a given path
        // This is done to ensure we access CenterPathBP at a valid Index
        considerIndex = (searchIndex + pathPathPointCount) % pathPathPointCount;
        considerPathPoint = &pathPathPoints[considerIndex];
        x_dist = considerPathPoint->Pointx - PosX;
        y_dist = considerPathPoint->Pointy - PosY;
        z_dist = considerPathPoint->Pointz - PosZ;
        squaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        if (squaredDistance < minimumDistance) {
            minimumDistance = squaredDistance;
            nearestPathPointIndex = considerIndex;
            pathPointFound = true;
        }
    }
    if (pathPointFound == false) {
        for (searchIndex = PathPointIndex - 3; searchIndex < PathPointIndex + 7; searchIndex++) {
            considerIndex = ((searchIndex + pathPathPointCount) % pathPathPointCount);
            considerPathPoint = &pathPathPoints[considerIndex];
            /**
             * This fake match is done to stop the compiler from optimzing out considerPathPoint.
             * Maybe if no pathPoint was found some debugging info was printed out, but come
             * production Time they removed the debug printing but not the Loop?
             **/
            if (considerPathPoint && considerPathPoint) {};
        }
    }
    return nearestPathPointIndex;
}

void KyouseiAreaSetChkEnemy(UNUSED f32 PosX, f32 PosY, UNUSED f32 PosZ, s16* PathPointIndex, UNUSED s32 arg4) {
    s16 var_v0;

    var_v0 = *PathPointIndex;
    if ((CourseID == COURSE_WARIO_STADIUM) && (var_v0 >= 0x475) && (var_v0 < 0x480) && (PosY < 0.0f)) {
        var_v0 = 0x0398;
    }
    *PathPointIndex = var_v0;
}

void WrapPathIndexAtFinish(UNUSED f32 PosX, UNUSED f32 PosY, f32 PosZ, s16* PathPointIndex, s32 PathIndex) {
    s16 pathPoint;
    pathPoint = *PathPointIndex;
    if (pathPoint == 0) {
        if (FinishLineZ < PosZ) {
            pathPoint = CourseTotalPathPoints[PathIndex] - 1;
        }
    } else if (((pathPoint + 1) == CourseTotalPathPoints[PathIndex]) && (PosZ <= FinishLineZ)) {
        pathPoint = 0;
    }
    *PathPointIndex = pathPoint;
}

s16 CalcOgaAreaKart(f32 PosX, f32 PosY, f32 PosZ, Player* Player, s32 PlayerId, s32* PathIndex) {
    u16 trackSectionId;
    s16 ret;

    trackSectionId = CheckArea(Player->Bump.LastZX);
    if ((trackSectionId <= 0) || (trackSectionId >= 0x33)) {
        trackSectionId = LakituSurface[PlayerId];
    }
    LakituSurface[PlayerId] = trackSectionId;
    ret = CalcOGAAreaSubBPWrapper(PosX, PosY, PosZ, trackSectionId, PathIndex);
    CurrentPathID[PlayerId] = *PathIndex;
    return ret;
}

/**
 * Updates and validates a Player's pathPoint Position on the track
 * Handles different logic for human players vs AI, and includes recovery mechanisms
 *
 * @Param PosX Current X Position
 * @Param PosY Current Y Position
 * @Param PosZ Current Z Position
 * @Param PathPointIndex Current pathPoint Index
 * @Param Player Pointer to Player structure
 * @Param PlayerId Player's ID
 * @Param PathIndex Current track path Index
 * @return New pathPoint Index or -1 if invalid
 */
s16 GetOgaAreaKart(f32 PosX, f32 PosY, f32 PosZ, s16 PathPointIndex, Player* Player, s32 PlayerId, s32 PathIndex) {
    s16 newPathPoint;
    UNUSED s16 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    CenterPathStruct* temp_v1;

    // Human Player handling (non-AI controlled)
    if ((Player->Flag & IS_PLAYER) && !(Player->Flag & IS_CPU_PLAYER)) {
        newPathPoint = GetOgaAreaGroupSubBP(PosX, PosY, PosZ, PathPointIndex, PathIndex,
                                                    (u16) CheckArea(Player->Bump.LastZX));
        if (newPathPoint == -1) {
            newPathPoint = CalcOgaAreaKart(PosX, PosY, PosZ, Player, PlayerId, &PathIndex);
        }
    } else { // AI or special case Player handling
        if (TenukiFlg[PlayerId] == true) {
            if (Player->JugemuFlag & IS_IN_WATER) {
                temp_v1 = &CenterPathBP[PathIndex][PathPointIndex];
                Player->Position[0] = (f32) temp_v1->Pointx;
                Player->Position[1] = (f32) temp_v1->Pointy;
                Player->Position[2] = (f32) temp_v1->Pointz;
                Player->JugemuFlag &= ~IS_IN_WATER;
                return PathPointIndex;
            }
            if (PlayerId == ((s32) OgaBumpChkCounter % 8)) {
                CheckBump(&Player->Bump, 10.0f, PosX, PosY, PosZ);
                LakituSurface[PlayerId] = CheckArea(Player->Bump.LastZX);
                newPathPoint = GetOgaAreaGroupSubBP(PosX, PosY, PosZ, PathPointIndex, PathIndex,
                                                            LakituSurface[PlayerId]);
                if (newPathPoint == -1) {
                    newPathPoint = GetOgaAreaSubBP(PosX, PosY, PosZ, PathPointIndex, PathIndex);
                }
                if (newPathPoint == -1) {
                    newPathPoint = CalcOGAAreaSubBPWrapper(PosX, PosY, PosZ,
                                                                         LakituSurface[PlayerId], &PathIndex);
                    temp_v1 = &CenterPathBP[PathIndex][newPathPoint];
                    Player->Position[0] = (f32) temp_v1->Pointx;
                    Player->Position[1] = (f32) temp_v1->Pointy;
                    Player->Position[2] = (f32) temp_v1->Pointz;
                }
            } else {
                newPathPoint = GetOgaAreaSubBP(PosX, PosY, PosZ, PathPointIndex, PathIndex);
                if (newPathPoint == -1) {
                    newPathPoint = CheckOgaAreaBP(PosX, PosY, PosZ, PathIndex);
                    temp_v1 = &CenterPathBP[PathIndex][newPathPoint];
                    PosX = (f32) temp_v1->Pointx;
                    PosY = (f32) temp_v1->Pointy;
                    PosZ = (f32) temp_v1->Pointz;
                    Player->Position[0] = PosX;
                    Player->Position[1] = PosY;
                    Player->Position[2] = PosZ;
                    CheckBump(&Player->Bump, 10.0f, PosX, PosY, PosZ);
                    LakituSurface[PlayerId] = CheckArea(Player->Bump.LastZX);
                }
            }
        } else {
            newPathPoint = GetOgaAreaSubBP(PosX, PosY, PosZ, PathPointIndex, PathIndex);
            if (newPathPoint == -1) {
                newPathPoint = CalcOgaAreaKart(PosX, PosY, PosZ, Player, PlayerId, &PathIndex);
            }
        }
        KyouseiAreaSetChkEnemy(PosX, PosY, PosZ, &newPathPoint, PathIndex);
    }
    WrapPathIndexAtFinishCheck(PosX, PosY, PosZ, &newPathPoint, PathIndex);
    return newPathPoint;
}

s16 GetOgaExArea(f32 xPos, UNUSED f32 YPos, f32 zPos, s16 PathPointIndex) {
    f32 xdiff;
    f32 zdiff;
    f32 minimumDistance;
    f32 considerSquaredDistance;
    s16 realIndex;
    s16 minimumIndex;
    s16 considerIndex;
    Path2D* considerPathPoint;

    minimumDistance = 250000.0f;
    minimumIndex = -1;
    for (realIndex = PathPointIndex - 2; realIndex < PathPointIndex + 7; realIndex++) {
        considerIndex = realIndex;
        if (realIndex < 0) {
            considerIndex = realIndex + CourseObstacle;
        }
        considerIndex %= CourseObstacle;
        considerPathPoint = &CenterLineEX[considerIndex];
        xdiff = considerPathPoint->x - xPos;
        zdiff = considerPathPoint->z - zPos;
        considerSquaredDistance = (xdiff * xdiff) + (zdiff * zdiff);
        if (considerSquaredDistance < minimumDistance) {
            minimumDistance = considerSquaredDistance;
            minimumIndex = considerIndex;
        }
    }
    if (minimumIndex == -1) {
        minimumIndex = PathPointIndex;
    }
    return minimumIndex;
}

s16 CalcOgaAreaBP(f32 PosX, f32 PosY, f32 PosZ, s32* PathIndex) {
    UNUSED s32 pad;
    Collision sp24;

    CheckBump(&sp24, 10.0f, PosX, PosY, PosZ);
    return CalcOGAAreaSubBPWrapper(PosX, PosY, PosZ, CheckArea(sp24.LastZX), PathIndex);
}

s16 GetOgaAreaBP(f32 PosX, f32 PosY, f32 PosZ, s16 PathPointIndex, s32 PathIndex) {
    s16 pathPoint;

    pathPoint = GetOgaAreaSubBP(PosX, PosY, PosZ, PathPointIndex, PathIndex);
    if (pathPoint == -1) {
        pathPoint = CalcOgaAreaBP(PosX, PosY, PosZ, &PathIndex);
    }
    WrapPathIndexAtFinishCheck(PosX, PosY, PosZ, &pathPoint, PathIndex);
    return pathPoint;
}

s16 GetOgaCamAreaBP(f32 PosX, f32 PosY, f32 PosZ, s16 PathPointIndex, s32 PathIndex) {
    s16 pathPoint;

    pathPoint = GetOgaAreaSubBP(PosX, PosY, PosZ, PathPointIndex, PathIndex);
    if (pathPoint == -1) {
        pathPoint = CalcOgaAreaBP(PosX, PosY, PosZ, &PathIndex);
    }
    return pathPoint;
}

f32 SoukouZureMove(s32 PlayerId) {
    SOUKOU_ZURE_TYPE* temp_v0;
    f32 Target;
    f32 Current;

    temp_v0 = &LaneData[PlayerId];
    Current = temp_v0->Lane;
    Target = temp_v0->LaneTarget;
    if (Current < Target) {
        Current += temp_v0->Speed;
        if (Target < Current) {
            Current = Target;
        }
    } else if (Target < Current) {
        Current -= temp_v0->Speed;
        if (Current < Target) {
            Current = Target;
        }
    }
    temp_v0->Lane = Current;
    return Current;
}

void MokuhyouSetCenter(s32 PlayerId, u16 pathPoint) {
    UNUSED s32 stackPadding0;
    f32 sp48;
    f32 sp44;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    f32 stackPadding4;
    f32 stackPadding5;
    f32 sp2C;
    s32 lookAheadDistance;
    s16 curveCount;
    u16 thing;

    curveCount = ShortCutData[pathPoint];
    lookAheadDistance = 6;
    sp2C = SoukouZureMove(PlayerId);
    thing = pathPoint;

    switch (CourseID) {
        case COURSE_AWARD_CEREMONY:
            lookAheadDistance = 1;
            break;
        case COURSE_TOADS_TURNPIKE:
            lookAheadDistance = 7;
            break;
        case COURSE_YOSHI_VALLEY:
            break;
        default:
            if (curveCount < 6) {
                lookAheadDistance = 8;
            } else if (curveCount >= 0x15) {
                lookAheadDistance = 20;
            }
            break;
    }

    if (lookAheadDistance >= 8) {
        if ((ZureData[PlayerId] > 0.75f) && (CurveData[thing] == RIGHT_LEANING_CURVE)) {
            lookAheadDistance = 7;
        }
        if ((ZureData[PlayerId] < -0.75f) && (CurveData[thing] == LEFT_LEANING_CURVE)) {
            lookAheadDistance = 7;
        }
    }
    if (CourseOutChk(PlayerId) == true) {
        lookAheadDistance = 5;
    }
    if (MokuhyouAddArea[PlayerId] < lookAheadDistance) {
        MokuhyouAddArea[PlayerId]++;
    }
    if (lookAheadDistance < MokuhyouAddArea[PlayerId]) {
        MokuhyouAddArea[PlayerId]--;
    }
    pathPoint = (MokuhyouAddArea[PlayerId] + pathPoint) % CenterLineMax;
    ObjCalcFromZureBP(pathPoint, sp2C, BipasNum);
    sp48 = object[0];
    sp44 = object[2];
    ObjCalcFromZureBP(((pathPoint + 1) % CenterLineMax) & 0xFFFF, sp2C, BipasNum);
    stackPadding5 = object[0];
    object[0] = (sp48 + stackPadding5) * 0.5f;
    stackPadding4 = object[2];
    object[2] = (sp44 + stackPadding4) * 0.5f;
}

s16 HanyouSoukouSubBP(Vec3f Position, s16* PathPointIndex, f32 Speed, f32 Arg3, s16 PathIndex, s16 arg5) {
    f32 temp1;
    f32 temp2;
    f32 midX;
    UNUSED s16 stackPadding1;
    s16 pathPoint1;
    s16 pathPoint2;
    f32 Pad3;
    f32 midY;
    f32 pad4;
    f32 midZ;
    f32 Distance;
    f32 oldPosX;
    f32 oldPosY;
    f32 oldPosZ;
    f32 var_f2;
    f32 var_f12;
    f32 var_f14;
    s16 temp_v0;
    s32 temp_v1;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    Vec3f oldPos;
    CenterPathStruct* path;

    path = CenterPathBP[PathIndex];
    oldPos[0] = Position[0];
    oldPos[1] = Position[1];
    oldPos[2] = Position[2];
    oldPosX = Position[0];
    oldPosY = Position[1];
    oldPosZ = Position[2];
    temp_v0 = GetOgaAreaBP(oldPosX, oldPosY, oldPosZ, *PathPointIndex, (s32) PathIndex);
    *PathPointIndex = temp_v0;
    temp_v1 = temp_v0 + arg5;
    pathPoint1 = temp_v1 % CourseTotalPathPoints[PathIndex];
    pathPoint2 = (temp_v1 + 1) % CourseTotalPathPoints[PathIndex];
    ObjCalcFromZureBP(pathPoint1, Arg3, PathIndex);
    Pad3 = object[0];
    pad4 = object[2];
    ObjCalcFromZureBP(pathPoint2, Arg3, PathIndex);
    temp1 = object[0];
    temp2 = object[2];
    midY = (path[pathPoint1].Pointy + path[pathPoint2].Pointy) * 0.5f;
    midX = (Pad3 + temp1) * 0.5f;
    midZ = (pad4 + temp2) * 0.5f;
    xdiff = midX - oldPosX;
    ydiff = midY - oldPosY;
    zdiff = midZ - oldPosZ;
    Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (Distance > 0.01f) {
        var_f2 = ((xdiff * Speed) / Distance) + oldPosX;
        var_f12 = ((ydiff * Speed) / Distance) + oldPosY;
        var_f14 = ((zdiff * Speed) / Distance) + oldPosZ;
    } else {
        var_f2 = oldPosX;
        var_f12 = oldPosY;
        var_f14 = oldPosZ;
    }
    Position[0] = var_f2;
    Position[1] = var_f12;
    Position[2] = var_f14;
    return OgaCalcDirection(oldPos, Position);
}

s16 HanyouGyakuSoukouSubBP(Vec3f pos, s16* PathPointIndex, f32 Speed, f32 Arg3, s16 PathIndex) {
    UNUSED f32 pad;
    f32 Thing1;
    f32 thing2;
    UNUSED s16 stackPadding1;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED s16 stackPadding2;
    f32 Pad2;
    f32 midX;
    f32 Pad3;
    f32 midY;
    f32 midZ;
    f32 Distance;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 var_f2;
    f32 var_f12;
    f32 var_f14;
    s16 temp_v0;
    f32 xdiff;
    f32 ydiff;
    f32 zdiff;
    s32 pathPointCount;
    Vec3f sp54;

    sp54[0] = pos[0];
    sp54[1] = pos[1];
    sp54[2] = pos[2];
    pathPointCount = CourseTotalPathPoints[PathIndex];
    temp_f20 = pos[0];
    temp_f22 = pos[1];
    temp_f24 = pos[2];
    temp_v0 = GetOgaAreaBP(temp_f20, temp_f22, temp_f24, *PathPointIndex, (s32) PathIndex);
    *PathPointIndex = temp_v0;
    pathPoint1 = ((temp_v0 + pathPointCount) - 3) % pathPointCount;
    pathPoint2 = ((temp_v0 + pathPointCount) - 4) % pathPointCount;
    ObjCalcFromZureBP(pathPoint1, Arg3, PathIndex);
    Pad2 = object[0];
    Pad3 = object[2];
    ObjCalcFromZureBP(pathPoint2, Arg3, PathIndex);
    Thing1 = object[0];
    thing2 = object[2];
    midY = (CenterPathBP[PathIndex][pathPoint1].Pointy + CenterPathBP[PathIndex][pathPoint2].Pointy) * 0.5f;
    midX = (Pad2 + Thing1) * 0.5f;
    midZ = (Pad3 + thing2) * 0.5f;
    xdiff = midX - temp_f20;
    ydiff = midY - temp_f22;
    zdiff = midZ - temp_f24;
    Distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (Distance > 0.01f) {
        var_f2 = ((xdiff * Speed) / Distance) + temp_f20;
        var_f12 = ((ydiff * Speed) / Distance) + temp_f22;
        var_f14 = ((zdiff * Speed) / Distance) + temp_f24;
    } else {
        var_f2 = temp_f20;
        var_f12 = temp_f22;
        var_f14 = temp_f24;
    }
    pos[0] = var_f2;
    pos[1] = var_f12;
    pos[2] = var_f14;
    return OgaCalcDirection(sp54, pos);
}

s16 HanyouSoukouSubEX(Vec3f pos, s16* PathPointIndex, f32 Speed) {
    f32 origXPos;
    f32 origYPos;
    f32 origZPos;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    UNUSED s32 stackPadding5;
    UNUSED s32 stackPadding6;
    UNUSED s32 stackPadding7;
    UNUSED s32 stackPadding8;
    f32 farPathPointAverageX;
    f32 farPathPointAverageZ;
    f32 x_dist;
    f32 y_dist;
    f32 Distance;
    f32 newX;
    f32 newZ;
    s16 newPathPointIndex;
    s16 farPathPoint1;
    s16 farPathPoint2;
    Path2D* temp_a0;
    Path2D* temp_a2;
    Vec3f sp38;

    origXPos = pos[0];
    origYPos = pos[1];
    origZPos = pos[2];
    sp38[0] = pos[0];
    sp38[1] = pos[1];
    sp38[2] = pos[2];
    newPathPointIndex = GetOgaExArea(origXPos, origYPos, origZPos, *PathPointIndex);
    *PathPointIndex = newPathPointIndex;
    farPathPoint1 = (newPathPointIndex + 3) % CourseObstacle;
    farPathPoint2 = (newPathPointIndex + 4) % CourseObstacle;
    temp_a0 = &CenterLineEX[farPathPoint1];
    temp_a2 = &CenterLineEX[farPathPoint2];
    farPathPointAverageX = (temp_a0->x + temp_a2->x) * 0.5f;
    farPathPointAverageZ = (temp_a0->z + temp_a2->z) * 0.5f;
    x_dist = farPathPointAverageX - origXPos;
    y_dist = farPathPointAverageZ - origZPos;
    Distance = sqrtf((x_dist * x_dist) + (y_dist * y_dist));
    if (Distance > 0.01f) {
        newX = ((x_dist * Speed) / Distance) + origXPos;
        newZ = ((y_dist * Speed) / Distance) + origZPos;
    } else {
        newX = origXPos;
        newZ = origZPos;
    }
    pos[0] = newX;
    pos[1] = origYPos;
    pos[2] = newZ;
    return OgaCalcDirection(sp38, pos);
}
