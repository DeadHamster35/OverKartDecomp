#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <path.h>
#include <course.h>
#include "main.h"
#include "cpu_vehicles_camera_path.h"

bool straight_chk(UNUSED s32 arg0, u16 pathPointIndex) {
    s16 thing = short_cut_data[pathPointIndex];
    if (thing > 0) {
        return true;
    }
    return false;
}

bool course_out_chk(s32 playerIndex) {
    f32 value = zure_data[playerIndex];
    if ((1.1f <= value) || (value <= -1.1f)) {
        return true;
    }
    return false;
}

/**
 * Calculates a factor representing where the player is positioned between left and right track boundaries
 * Returns a value between -1.0 and 1.0:
 * -1.0 = On the left boundary
 *  0.0 = In the middle of the track
 *  1.0 = On the right boundary
 *
 * @param posX Player's X position
 * @param posZ Player's Z position
 * @param pathPointIndex Current pathPoint index
 * @param pathIndex Current path/track index
 * @return Position factor between track boundaries
 */
f32 zure_center_sub_BP(f32 posX, f32 posZ, u16 pathPointIndex, s32 pathIndex) {
    f32 leftX;
    f32 leftZ;
    f32 rightX;
    f32 rightZ;
    f32 boundarySquaredDistance;
    f32 positionFactor;
    CenterPathStruct* leftPathPoint;
    CenterPathStruct* rightPathPoint;

    leftPathPoint = &SideLPathBP[pathIndex][pathPointIndex];
    rightPathPoint = &SideRPathBP[pathIndex][pathPointIndex];

    leftX = leftPathPoint->pointx;
    leftZ = leftPathPoint->pointz;
    rightX = rightPathPoint->pointx;
    rightZ = rightPathPoint->pointz;

    boundarySquaredDistance = ((rightX - leftX) * (rightX - leftX)) + ((rightZ - leftZ) * (rightZ - leftZ));

    // Avoid division by zero for very close or identical boundary points
    if (boundarySquaredDistance < 0.01f) {
        return 0.0f;
    }
    // Calculate normalized position factor using vector projection
    // Formula: 2 * (dot product of vectors) / (squared magnitude) - 1
    // This maps the position to a -1 to 1 range
    positionFactor =
        ((2.0f * ((rightX - leftX) * (posX - leftX) + (rightZ - leftZ) * (posZ - leftZ))) / boundarySquaredDistance) -
        1.0f;
    return positionFactor;
}

void zure_chk_BP(s32 playerId, u16 pathPointIndex, s32 pathIndex) {
    UNUSED Vec3f pad;
    zure_data[playerId] = 0.0f;
    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        if ((gPlayers[playerId].flag & EXISTS) != 0) {
            zure_data[playerId] = zure_center_sub_BP(
                gPlayers[playerId].position[0], gPlayers[playerId].position[2], pathPointIndex, pathIndex);
        }
    }
}

void obj_calc_from_zure_dist_BP(u16 pathPointIndex, f32 lerpFactor, f32 offsetDistance, s16 pathIndex) {
    UNUSED s32 pad[4];
    f32 pathPointOneX;
    f32 pathPointOneZ;
    f32 pathPointTwoX;
    f32 pathPointTwoZ;
    UNUSED s32 pad2;
    f32 xdiff;
    f32 zdiff;
    f32 segmentLength;
    UNUSED f32 temp_f12;
    UNUSED f32 temp_f2_2;
    UNUSED CenterPathStruct* path;
    CenterPathStruct* pathPointTwo;
    CenterPathStruct* pathPointOne;

    pathPointOne = &CenterPathBP[pathIndex][pathPointIndex];
    pathPointOneX = pathPointOne->pointx;
    pathPointOneZ = pathPointOne->pointz;
    pathPointTwo = &CenterPathBP[pathIndex][(pathPointIndex + 1) % center_line_max];
    pathPointTwoX = pathPointTwo->pointx;
    pathPointTwoZ = pathPointTwo->pointz;

    // Calculate vector between path point
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
 * Calculates an interpolated position between left and right track paths.
 *
 * @param currentPathPoint Index of the current pathPoint
 * @param trackOffset Value between 0.0 and 1.0 determining position between left (0.0) and right (1.0) path
 * @param pathIndex Index of the track/path segment
 */
void obj_calc_from_zure_BP(u16 pathPointIndex, f32 trackOffset, s16 pathIndex) {
    CenterPathStruct* path1;
    CenterPathStruct* path2;
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

    path1 = &SideLPathBP[pathIndex][pathPointIndex];
    path2 = &SideRPathBP[pathIndex][pathPointIndex];

    x1 = (f32) path1->pointx;
    z1 = (f32) path1->pointz;

    x2 = (f32) path2->pointx;
    z2 = (f32) path2->pointz;

    pathPointIndex += 1;
    pathPointIndex = pathPointIndex % g_courseTotalPathPoints[pathIndex];

    path1 = &SideLPathBP[pathIndex][pathPointIndex];
    path2 = &SideRPathBP[pathIndex][pathPointIndex];

    x3 = (f32) path1->pointx;
    z3 = (f32) path1->pointz;

    x4 = (f32) path2->pointx;
    z4 = (f32) path2->pointz;

    temp_f0 = 0.5f - (trackOffset / 2.0f);
    temp_f12 = 1.0f - temp_f0;
    object[0] = ((temp_f0 * (x1 + x3)) / 2.0f) + ((temp_f12 * (x2 + x4)) / 2.0f);
    object[2] = ((temp_f0 * (z1 + z3)) / 2.0f) + ((temp_f12 * (z2 + z4)) / 2.0f);
}

s16 check_oga_area_BP(f32 posX, f32 posY, f32 posZ, s32 pathIndex) {
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

    pathPathPoints = CenterPathBP[pathIndex];
    pathPathPointCount = g_courseTotalPathPoints[pathIndex];
    considerPathPoint = &pathPathPoints[0];
    x_dist = (f32) considerPathPoint->pointx - posX;
    y_dist = (f32) considerPathPoint->pointy - posY;
    z_dist = (f32) considerPathPoint->pointz - posZ;
    minimumSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
    nearestPathPointIndex = 0;
    for (considerPathPointIndex = 1; considerPathPointIndex < pathPathPointCount;
         considerPathPoint++, considerPathPointIndex++) {
        x_dist = (f32) considerPathPoint->pointx - posX;
        y_dist = (f32) considerPathPoint->pointy - posY;
        z_dist = (f32) considerPathPoint->pointz - posZ;
        considerSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        if (considerSquaredDistance < minimumSquaredDistance) {
            nearestPathPointIndex = considerPathPointIndex;
            minimumSquaredDistance = considerSquaredDistance;
        }
    }
    return nearestPathPointIndex;
}

s16 CalcOGAAreaSubBP(f32 posX, f32 posY, f32 posZ, u16 trackSectionId, s32* pathIndex) {
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
    temp_t0 = *pathIndex;
    nearestPathPointIndex = 0;
    var_t1 = 0;
    var_a1 = 0;
    pathPathPoints = CenterPathBP[temp_t0];
    pathPathPointCount = g_courseTotalPathPoints[temp_t0];
    considerPathPoint = &pathPathPoints[0];
    for (considerPathPointIndex = 0; considerPathPointIndex < pathPathPointCount;
         considerPathPointIndex++, considerPathPoint++) {
        if ((considerPathPoint->unknown == trackSectionId) || (g_courseID == COURSE_AWARD_CEREMONY)) {
            var_t1 = 1;
            x_dist = (f32) considerPathPoint->pointx - posX;
            y_dist = (f32) considerPathPoint->pointy - posY;
            z_dist = (f32) considerPathPoint->pointz - posZ;
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
                pathPathPointCount = g_courseTotalPathPoints[considerPathIndex];
                for (considerPathPointIndex = 0; considerPathPointIndex < pathPathPointCount;
                     considerPathPointIndex++, considerPathPoint++) {
                    if (considerPathPoint->unknown == trackSectionId) {
                        x_dist = (f32) considerPathPoint->pointx - posX;
                        y_dist = (f32) considerPathPoint->pointy - posY;
                        z_dist = (f32) considerPathPoint->pointz - posZ;
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
        pathPathPointCount = g_courseTotalPathPoints[0];
        considerPathPoint = &pathPathPoints[0];
        x_dist = (f32) considerPathPoint->pointx - posX;
        y_dist = (f32) considerPathPoint->pointy - posY;
        z_dist = (f32) considerPathPoint->pointz - posZ;
        minimumSquaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        nearestPathPointIndex = 0;
        for (considerPathPointIndex = 1; considerPathPointIndex < pathPathPointCount;
             considerPathPoint++, considerPathPointIndex++) {
            x_dist = (f32) considerPathPoint->pointx - posX;
            y_dist = (f32) considerPathPoint->pointy - posY;
            z_dist = (f32) considerPathPoint->pointz - posZ;
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
        *pathIndex = var_t4;
    }
    return nearestPathPointIndex;
}

/**
 * Tries to find the path point nearest to (posX, posY, posZ)
 * Only consider path in the same segment as trackSectionId
 * Only considers path within 500 units of(posX, posY, posZ)
 * Looks 3 path behind and 6 path ahead of pathPointIndex
 **/
s16 get_oga_area_group_sub_BP(f32 posX, f32 posY, f32 posZ, s16 pathPointIndex, s32 pathIndex, u16 trackSectionId) {
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
    pathPathPointCount = g_courseTotalPathPoints[pathIndex];
    pathPathPoints = CenterPathBP[pathIndex];
    for (searchIndex = pathPointIndex - 3; searchIndex < pathPointIndex + 7; searchIndex++) {
        // Its possible for searchIndex to be less than 0 or greater than the number of path in a given path
        // This is done to ensure we access CenterPathBP at a valid index
        considerIndex = (searchIndex + pathPathPointCount) % pathPathPointCount;
        considerPathPoint = &pathPathPoints[considerIndex];
        if (considerPathPoint->unknown == trackSectionId) {
            x_dist = considerPathPoint->pointx - posX;
            y_dist = considerPathPoint->pointy - posY;
            z_dist = considerPathPoint->pointz - posZ;
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
 * Tries to find the path point nearest to (posX, posY, posZ)
 * Only considers path within 400 units of (posX, posY, posZ)
 * Looks 3 path behind and 6 path ahead of pathPointIndex
 **/
s16 get_oga_area_sub_BP(f32 posX, f32 posY, f32 posZ, s16 pathPointIndex, s32 pathIndex) {
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
    pathPathPointCount = g_courseTotalPathPoints[pathIndex];
    pathPathPoints = CenterPathBP[pathIndex];
    for (searchIndex = pathPointIndex - 3; searchIndex < pathPointIndex + 7; searchIndex++) {
        // Its possible for searchIndex to be less than 0 or greater than the number of path in a given path
        // This is done to ensure we access CenterPathBP at a valid index
        considerIndex = (searchIndex + pathPathPointCount) % pathPathPointCount;
        considerPathPoint = &pathPathPoints[considerIndex];
        x_dist = considerPathPoint->pointx - posX;
        y_dist = considerPathPoint->pointy - posY;
        z_dist = considerPathPoint->pointz - posZ;
        squaredDistance = (x_dist * x_dist) + (y_dist * y_dist) + (z_dist * z_dist);
        if (squaredDistance < minimumDistance) {
            minimumDistance = squaredDistance;
            nearestPathPointIndex = considerIndex;
            pathPointFound = true;
        }
    }
    if (pathPointFound == false) {
        for (searchIndex = pathPointIndex - 3; searchIndex < pathPointIndex + 7; searchIndex++) {
            considerIndex = ((searchIndex + pathPathPointCount) % pathPathPointCount);
            considerPathPoint = &pathPathPoints[considerIndex];
            /**
             * This fake match is done to stop the compiler from optimzing out considerPathPoint.
             * Maybe if no pathPoint was found some debugging info was printed out, but come
             * production time they removed the debug printing but not the loop?
             **/
            if (considerPathPoint && considerPathPoint) {};
        }
    }
    return nearestPathPointIndex;
}

void kyousei_area_set_chk_enemy(UNUSED f32 posX, f32 posY, UNUSED f32 posZ, s16* pathPointIndex, UNUSED s32 arg4) {
    s16 var_v0;

    var_v0 = *pathPointIndex;
    if ((g_courseID == COURSE_WARIO_STADIUM) && (var_v0 >= 0x475) && (var_v0 < 0x480) && (posY < 0.0f)) {
        var_v0 = 0x0398;
    }
    *pathPointIndex = var_v0;
}

void WrapPathIndexAtFinish(UNUSED f32 posX, UNUSED f32 posY, f32 posZ, s16* pathPointIndex, s32 pathIndex) {
    s16 pathPoint;
    pathPoint = *pathPointIndex;
    if (pathPoint == 0) {
        if (g_finishLineZ < posZ) {
            pathPoint = g_courseTotalPathPoints[pathIndex] - 1;
        }
    } else if (((pathPoint + 1) == g_courseTotalPathPoints[pathIndex]) && (posZ <= g_finishLineZ)) {
        pathPoint = 0;
    }
    *pathPointIndex = pathPoint;
}

s16 calc_oga_area_kart(f32 posX, f32 posY, f32 posZ, Player* player, s32 playerId, s32* pathIndex) {
    u16 trackSectionId;
    s16 ret;

    trackSectionId = CheckArea(player->bump.last_zx);
    if ((trackSectionId <= 0) || (trackSectionId >= 0x33)) {
        trackSectionId = g_lakituSurface[playerId];
    }
    g_lakituSurface[playerId] = trackSectionId;
    ret = CalcOGAAreaSubBP_Wrapper(posX, posY, posZ, trackSectionId, pathIndex);
    CurrentPathID[playerId] = *pathIndex;
    return ret;
}

/**
 * Updates and validates a player's pathPoint position on the track
 * Handles different logic for human players vs AI, and includes recovery mechanisms
 *
 * @param posX Current X position
 * @param posY Current Y position
 * @param posZ Current Z position
 * @param pathPointIndex Current pathPoint index
 * @param player Pointer to player structure
 * @param playerId Player's ID
 * @param pathIndex Current track path index
 * @return New pathPoint index or -1 if invalid
 */
s16 get_oga_area_kart(f32 posX, f32 posY, f32 posZ, s16 pathPointIndex, Player* player, s32 playerId, s32 pathIndex) {
    s16 newPathPoint;
    UNUSED s16 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    CenterPathStruct* temp_v1;

    // Human player handling (non-AI controlled)
    if ((player->flag & IS_PLAYER) && !(player->flag & IS_CPU_PLAYER)) {
        newPathPoint = get_oga_area_group_sub_BP(posX, posY, posZ, pathPointIndex, pathIndex,
                                                    (u16) CheckArea(player->bump.last_zx));
        if (newPathPoint == -1) {
            newPathPoint = calc_oga_area_kart(posX, posY, posZ, player, playerId, &pathIndex);
        }
    } else { // AI or special case player handling
        if (tenuki_flg[playerId] == true) {
            if (player->jugemu_flag & IS_IN_WATER) {
                temp_v1 = &CenterPathBP[pathIndex][pathPointIndex];
                player->position[0] = (f32) temp_v1->pointx;
                player->position[1] = (f32) temp_v1->pointy;
                player->position[2] = (f32) temp_v1->pointz;
                player->jugemu_flag &= ~IS_IN_WATER;
                return pathPointIndex;
            }
            if (playerId == ((s32) oga_bump_chk_counter % 8)) {
                CheckBump(&player->bump, 10.0f, posX, posY, posZ);
                g_lakituSurface[playerId] = CheckArea(player->bump.last_zx);
                newPathPoint = get_oga_area_group_sub_BP(posX, posY, posZ, pathPointIndex, pathIndex,
                                                            g_lakituSurface[playerId]);
                if (newPathPoint == -1) {
                    newPathPoint = get_oga_area_sub_BP(posX, posY, posZ, pathPointIndex, pathIndex);
                }
                if (newPathPoint == -1) {
                    newPathPoint = CalcOGAAreaSubBP_Wrapper(posX, posY, posZ,
                                                                         g_lakituSurface[playerId], &pathIndex);
                    temp_v1 = &CenterPathBP[pathIndex][newPathPoint];
                    player->position[0] = (f32) temp_v1->pointx;
                    player->position[1] = (f32) temp_v1->pointy;
                    player->position[2] = (f32) temp_v1->pointz;
                }
            } else {
                newPathPoint = get_oga_area_sub_BP(posX, posY, posZ, pathPointIndex, pathIndex);
                if (newPathPoint == -1) {
                    newPathPoint = check_oga_area_BP(posX, posY, posZ, pathIndex);
                    temp_v1 = &CenterPathBP[pathIndex][newPathPoint];
                    posX = (f32) temp_v1->pointx;
                    posY = (f32) temp_v1->pointy;
                    posZ = (f32) temp_v1->pointz;
                    player->position[0] = posX;
                    player->position[1] = posY;
                    player->position[2] = posZ;
                    CheckBump(&player->bump, 10.0f, posX, posY, posZ);
                    g_lakituSurface[playerId] = CheckArea(player->bump.last_zx);
                }
            }
        } else {
            newPathPoint = get_oga_area_sub_BP(posX, posY, posZ, pathPointIndex, pathIndex);
            if (newPathPoint == -1) {
                newPathPoint = calc_oga_area_kart(posX, posY, posZ, player, playerId, &pathIndex);
            }
        }
        kyousei_area_set_chk_enemy(posX, posY, posZ, &newPathPoint, pathIndex);
    }
    WrapPathIndexAtFinishCheck(posX, posY, posZ, &newPathPoint, pathIndex);
    return newPathPoint;
}

s16 get_oga_ex_area(f32 xPos, UNUSED f32 yPos, f32 zPos, s16 pathPointIndex) {
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
    for (realIndex = pathPointIndex - 2; realIndex < pathPointIndex + 7; realIndex++) {
        considerIndex = realIndex;
        if (realIndex < 0) {
            considerIndex = realIndex + g_CourseObstacle;
        }
        considerIndex %= g_CourseObstacle;
        considerPathPoint = &center_line_EX[considerIndex];
        xdiff = considerPathPoint->x - xPos;
        zdiff = considerPathPoint->z - zPos;
        considerSquaredDistance = (xdiff * xdiff) + (zdiff * zdiff);
        if (considerSquaredDistance < minimumDistance) {
            minimumDistance = considerSquaredDistance;
            minimumIndex = considerIndex;
        }
    }
    if (minimumIndex == -1) {
        minimumIndex = pathPointIndex;
    }
    return minimumIndex;
}

s16 calc_oga_area_BP(f32 posX, f32 posY, f32 posZ, s32* pathIndex) {
    UNUSED s32 pad;
    Collision sp24;

    CheckBump(&sp24, 10.0f, posX, posY, posZ);
    return CalcOGAAreaSubBP_Wrapper(posX, posY, posZ, CheckArea(sp24.last_zx), pathIndex);
}

s16 get_oga_area_BP(f32 posX, f32 posY, f32 posZ, s16 pathPointIndex, s32 pathIndex) {
    s16 pathPoint;

    pathPoint = get_oga_area_sub_BP(posX, posY, posZ, pathPointIndex, pathIndex);
    if (pathPoint == -1) {
        pathPoint = calc_oga_area_BP(posX, posY, posZ, &pathIndex);
    }
    WrapPathIndexAtFinishCheck(posX, posY, posZ, &pathPoint, pathIndex);
    return pathPoint;
}

s16 get_oga_cam_area_BP(f32 posX, f32 posY, f32 posZ, s16 pathPointIndex, s32 pathIndex) {
    s16 pathPoint;

    pathPoint = get_oga_area_sub_BP(posX, posY, posZ, pathPointIndex, pathIndex);
    if (pathPoint == -1) {
        pathPoint = calc_oga_area_BP(posX, posY, posZ, &pathIndex);
    }
    return pathPoint;
}

f32 soukou_zure_move(s32 playerId) {
    SOUKOU_ZURE_TYPE* temp_v0;
    f32 target;
    f32 current;

    temp_v0 = &LaneData[playerId];
    current = temp_v0->lane;
    target = temp_v0->lane_target;
    if (current < target) {
        current += temp_v0->speed;
        if (target < current) {
            current = target;
        }
    } else if (target < current) {
        current -= temp_v0->speed;
        if (current < target) {
            current = target;
        }
    }
    temp_v0->lane = current;
    return current;
}

void mokuhyou_set_center(s32 playerId, u16 pathPoint) {
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

    curveCount = short_cut_data[pathPoint];
    lookAheadDistance = 6;
    sp2C = soukou_zure_move(playerId);
    thing = pathPoint;

    switch (g_courseID) {
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
        if ((zure_data[playerId] > 0.75f) && (curve_data[thing] == RIGHT_LEANING_CURVE)) {
            lookAheadDistance = 7;
        }
        if ((zure_data[playerId] < -0.75f) && (curve_data[thing] == LEFT_LEANING_CURVE)) {
            lookAheadDistance = 7;
        }
    }
    if (course_out_chk(playerId) == true) {
        lookAheadDistance = 5;
    }
    if (mokuhyou_add_area[playerId] < lookAheadDistance) {
        mokuhyou_add_area[playerId]++;
    }
    if (lookAheadDistance < mokuhyou_add_area[playerId]) {
        mokuhyou_add_area[playerId]--;
    }
    pathPoint = (mokuhyou_add_area[playerId] + pathPoint) % center_line_max;
    obj_calc_from_zure_BP(pathPoint, sp2C, bipas_num);
    sp48 = object[0];
    sp44 = object[2];
    obj_calc_from_zure_BP(((pathPoint + 1) % center_line_max) & 0xFFFF, sp2C, bipas_num);
    stackPadding5 = object[0];
    object[0] = (sp48 + stackPadding5) * 0.5f;
    stackPadding4 = object[2];
    object[2] = (sp44 + stackPadding4) * 0.5f;
}

s16 hanyou_soukou_sub_BP(Vec3f position, s16* pathPointIndex, f32 speed, f32 arg3, s16 pathIndex, s16 arg5) {
    f32 temp1;
    f32 temp2;
    f32 midX;
    UNUSED s16 stackPadding1;
    s16 pathPoint1;
    s16 pathPoint2;
    f32 pad3;
    f32 midY;
    f32 pad4;
    f32 midZ;
    f32 distance;
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

    path = CenterPathBP[pathIndex];
    oldPos[0] = position[0];
    oldPos[1] = position[1];
    oldPos[2] = position[2];
    oldPosX = position[0];
    oldPosY = position[1];
    oldPosZ = position[2];
    temp_v0 = get_oga_area_BP(oldPosX, oldPosY, oldPosZ, *pathPointIndex, (s32) pathIndex);
    *pathPointIndex = temp_v0;
    temp_v1 = temp_v0 + arg5;
    pathPoint1 = temp_v1 % g_courseTotalPathPoints[pathIndex];
    pathPoint2 = (temp_v1 + 1) % g_courseTotalPathPoints[pathIndex];
    obj_calc_from_zure_BP(pathPoint1, arg3, pathIndex);
    pad3 = object[0];
    pad4 = object[2];
    obj_calc_from_zure_BP(pathPoint2, arg3, pathIndex);
    temp1 = object[0];
    temp2 = object[2];
    midY = (path[pathPoint1].pointy + path[pathPoint2].pointy) * 0.5f;
    midX = (pad3 + temp1) * 0.5f;
    midZ = (pad4 + temp2) * 0.5f;
    xdiff = midX - oldPosX;
    ydiff = midY - oldPosY;
    zdiff = midZ - oldPosZ;
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance > 0.01f) {
        var_f2 = ((xdiff * speed) / distance) + oldPosX;
        var_f12 = ((ydiff * speed) / distance) + oldPosY;
        var_f14 = ((zdiff * speed) / distance) + oldPosZ;
    } else {
        var_f2 = oldPosX;
        var_f12 = oldPosY;
        var_f14 = oldPosZ;
    }
    position[0] = var_f2;
    position[1] = var_f12;
    position[2] = var_f14;
    return oga_calc_direction(oldPos, position);
}

s16 hanyou_gyaku_soukou_sub_BP(Vec3f pos, s16* pathPointIndex, f32 speed, f32 arg3, s16 pathIndex) {
    UNUSED f32 pad;
    f32 thing1;
    f32 thing2;
    UNUSED s16 stackPadding1;
    s16 pathPoint1;
    s16 pathPoint2;
    UNUSED s16 stackPadding2;
    f32 pad2;
    f32 midX;
    f32 pad3;
    f32 midY;
    f32 midZ;
    f32 distance;
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
    pathPointCount = g_courseTotalPathPoints[pathIndex];
    temp_f20 = pos[0];
    temp_f22 = pos[1];
    temp_f24 = pos[2];
    temp_v0 = get_oga_area_BP(temp_f20, temp_f22, temp_f24, *pathPointIndex, (s32) pathIndex);
    *pathPointIndex = temp_v0;
    pathPoint1 = ((temp_v0 + pathPointCount) - 3) % pathPointCount;
    pathPoint2 = ((temp_v0 + pathPointCount) - 4) % pathPointCount;
    obj_calc_from_zure_BP(pathPoint1, arg3, pathIndex);
    pad2 = object[0];
    pad3 = object[2];
    obj_calc_from_zure_BP(pathPoint2, arg3, pathIndex);
    thing1 = object[0];
    thing2 = object[2];
    midY = (CenterPathBP[pathIndex][pathPoint1].pointy + CenterPathBP[pathIndex][pathPoint2].pointy) * 0.5f;
    midX = (pad2 + thing1) * 0.5f;
    midZ = (pad3 + thing2) * 0.5f;
    xdiff = midX - temp_f20;
    ydiff = midY - temp_f22;
    zdiff = midZ - temp_f24;
    distance = sqrtf((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff));
    if (distance > 0.01f) {
        var_f2 = ((xdiff * speed) / distance) + temp_f20;
        var_f12 = ((ydiff * speed) / distance) + temp_f22;
        var_f14 = ((zdiff * speed) / distance) + temp_f24;
    } else {
        var_f2 = temp_f20;
        var_f12 = temp_f22;
        var_f14 = temp_f24;
    }
    pos[0] = var_f2;
    pos[1] = var_f12;
    pos[2] = var_f14;
    return oga_calc_direction(sp54, pos);
}

s16 hanyou_soukou_sub_EX(Vec3f pos, s16* pathPointIndex, f32 speed) {
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
    f32 distance;
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
    newPathPointIndex = get_oga_ex_area(origXPos, origYPos, origZPos, *pathPointIndex);
    *pathPointIndex = newPathPointIndex;
    farPathPoint1 = (newPathPointIndex + 3) % g_CourseObstacle;
    farPathPoint2 = (newPathPointIndex + 4) % g_CourseObstacle;
    temp_a0 = &center_line_EX[farPathPoint1];
    temp_a2 = &center_line_EX[farPathPoint2];
    farPathPointAverageX = (temp_a0->x + temp_a2->x) * 0.5f;
    farPathPointAverageZ = (temp_a0->z + temp_a2->z) * 0.5f;
    x_dist = farPathPointAverageX - origXPos;
    y_dist = farPathPointAverageZ - origZPos;
    distance = sqrtf((x_dist * x_dist) + (y_dist * y_dist));
    if (distance > 0.01f) {
        newX = ((x_dist * speed) / distance) + origXPos;
        newZ = ((y_dist * speed) / distance) + origZPos;
    } else {
        newX = origXPos;
        newZ = origZPos;
    }
    pos[0] = newX;
    pos[1] = origYPos;
    pos[2] = newZ;
    return oga_calc_direction(sp38, pos);
}
