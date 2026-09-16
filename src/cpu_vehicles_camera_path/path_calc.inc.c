// @arg index from 0 to 3.

// Processes course path by index.
// @arg index from 0 to 3.
// Each course can have 1-4 course paths.
void AreaBunkatuBP(s32 pathIndex) {

    CenterPathStruct* ptr;
    CenterPathStruct* pathDest;
    CenterPathStruct* path;
    s32 var_v0;
    s32 sp24;
    UNUSED s32 pad[2];
    s16 bInvalidPath;
    s32 i;

    // cast required
    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        pathDest = CenterPathBP[pathIndex];
        bInvalidPath = 1;
        if (g_courseID != COURSE_AWARD_CEREMONY) {
            var_v0 = area_read_rom_sub(pathDest, GET_COURSE_PathTable2(pathIndex));
            g_courseTotalPathPoints[pathIndex] = (u16) var_v0;
        } else {
            // Course path included in course_data which has already been loaded into memory.
            // This is how we get the addr to our path data.
            path = GET_COURSE_PathTable(pathIndex);
            ptr = path;

            for (i = 0; i < 3000; i++, ptr++) {
                if ((u16) ptr->pointx == 0x8000) {
                    sp24 = i - 1;
                    bInvalidPath = 0;
                    break;
                }
            }

            // If path data higher than 3000 something has gone wrong.
            // Skip processing the data.
            //! @todo Confirm this comment
            if (!bInvalidPath) {
                var_v0 = area_bunkatu_sub(pathDest, path, sp24, pathIndex);
                g_courseTotalPathPoints[pathIndex] = (u16) var_v0;
            }
        }
    }
}

void SidePointCalcBP(s32 pathIndex) {
    f32 pathPointWidth;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 x_dist;
    f32 z_dist;
    f32 neg_x_dist;
    f32 neg_z_dist;
    f32 xz_dist;
    s32 temp_f16;
    s32 pathPointIndex;
    CenterPathStruct* pathPoint;
    CenterPathStruct* nextPathPoint;
    CenterPathStruct* var_s1;
    CenterPathStruct* var_s2;

    if (((s32) GET_COURSE_AIMaximumSeparation) >= 0) {
        pathPointWidth = GET_COURSE_AIMaximumSeparation;
        pathPoint = &CenterPathBP[pathIndex][0];
        var_s1 = &SideLPathBP[pathIndex][0];
        var_s2 = &SideRPathBP[pathIndex][0];
        for (pathPointIndex = 0; pathPointIndex < g_courseTotalPathPoints[pathIndex];
             pathPointIndex++, var_s1++, var_s2++) {
            x1 = pathPoint->pointx;
            y1 = pathPoint->pointy;
            z1 = pathPoint->pointz;
            pathPoint++;
            nextPathPoint = &CenterPathBP[pathIndex][(pathPointIndex + 1) % ((s32) g_courseTotalPathPoints[pathIndex])];
            x2 = nextPathPoint->pointx;
            y2 = nextPathPoint->pointy;
            z2 = nextPathPoint->pointz;
            x_dist = x2 - x1;
            z_dist = z2 - z1;
            neg_x_dist = x1 - x2;
            neg_z_dist = z1 - z2;
            xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
            temp_f16 = (f32) ((y1 + y2) * 0.5);

            // Calculate left boundary position
            // Uses perpendicular vector (Z, -X) normalized by segment length
            var_s1->pointx = ((pathPointWidth * z_dist) / xz_dist) + x1;
            var_s1->pointy = temp_f16;
            var_s1->pointz = ((pathPointWidth * neg_x_dist) / xz_dist) + z1;

            // Calculate right boundary position
            // Uses opposite perpendicular vector (-Z, X)
            var_s2->pointx = ((pathPointWidth * neg_z_dist) / xz_dist) + x1;
            var_s2->pointy = temp_f16;
            var_s2->pointz = ((pathPointWidth * x_dist) / xz_dist) + z1;
        }
    }
}

/**
 * Calculates the track curvature using two sets of three path.
 * Returns a normalized value indicating curve direction and severity:
 * - Positive: Right turn
 * - Negative: Left turn
 * - Near zero: Straight section
 *
 * @param pathIndex Track path index
 * @param pathPointIndex Starting pathPoint for calculation
 * @return Normalized curvature value (-1 to 1)
 */
f32 curve_chk_BP(s32 pathIndex, u16 pathPointIndex) {
    f32 secondVectorX;
    f32 secondVectorZ;
    UNUSED f32 pad;
    CenterPathStruct* pathPathPoints;
    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 x3;
    f32 z3;
    f32 firstVectorX;
    f32 firstVectorZ;
    s32 pathPointCount;
    CenterPathStruct* pathPoint3;
    CenterPathStruct* pathPoint2;
    CenterPathStruct* pathPoint1;
    f32 secondLength;
    f32 firstLength;

    if ((s32) GET_COURSE_AIMaximumSeparation < 0) {
        return 0.0f;
    }
    pathPointCount = g_courseTotalPathPoints[pathIndex];
    pathPathPoints = CenterPathBP[pathIndex];

    pathPoint1 = &pathPathPoints[pathPointIndex];
    pathPoint2 = &pathPathPoints[(pathPointIndex + 1) % pathPointCount];
    pathPoint3 = &pathPathPoints[(pathPointIndex + 2) % pathPointCount];

    x1 = pathPoint1->pointx;
    z1 = pathPoint1->pointz;
    x2 = pathPoint2->pointx;
    z2 = pathPoint2->pointz;
    x3 = pathPoint3->pointx;
    z3 = pathPoint3->pointz;

    firstVectorX = (((x2 + x3) * 0.5) - x1);
    firstVectorZ = (((z2 + z3) * 0.5) - z1);

    pathPoint1 = &pathPathPoints[(pathPointIndex + 3) % pathPointCount];
    pathPoint2 = &pathPathPoints[(pathPointIndex + 4) % pathPointCount];
    pathPoint3 = &pathPathPoints[(pathPointIndex + 5) % pathPointCount];

    x1 = pathPoint1->pointx;
    z1 = pathPoint1->pointz;
    x2 = pathPoint2->pointx;
    z2 = pathPoint2->pointz;
    x3 = pathPoint3->pointx;
    z3 = pathPoint3->pointz;

    secondVectorX = (((x2 + x3) * 0.5) - x1);
    secondVectorZ = (((z2 + z3) * 0.5) - z1);

    firstLength = sqrtf((firstVectorZ * firstVectorZ) + (firstVectorX * firstVectorX));
    secondLength = sqrtf((secondVectorX * secondVectorX) + (secondVectorZ * secondVectorZ));
    return -((firstVectorZ * secondVectorX) - (firstVectorX * secondVectorZ)) / (secondLength * firstLength);
}

void CurveDataCalcBP(s32 pathIndex) {
    f64 sectionCurvature;
    UNUSED s32 pad;
    s32 k;
    s32 i;
    s32 j;
    s16* currentSection;
    s32 pathPointCount;
    s16* nextSection;

    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        pathPointCount = g_courseTotalPathPoints[pathIndex];
        currentSection = CurvePathDataBP[pathIndex];
        for (i = 0; i < pathPointCount; i++, currentSection++) {
            sectionCurvature = curve_chk_BP(pathIndex, i);
            *currentSection = STRAIGHT;
            if (sectionCurvature > 0.1) {
                *currentSection = RIGHT_CURVE;
            }
            if (sectionCurvature < -0.1) {
                *currentSection = LEFT_CURVE;
            }
        }
        currentSection = CurvePathDataBP[pathIndex];
        for (i = 0; i < pathPointCount; i++, currentSection++) {
            if (*currentSection == STRAIGHT) {
                // Look ahead for next curved section
                for (j = 1; j < pathPointCount; j++) {
                    nextSection = &CurvePathDataBP[pathIndex][(i + j) % pathPointCount];
                    switch (*nextSection) {
                        case RIGHT_LEANING_CURVE:
                        case RIGHT_CURVE:
                            for (k = 0; k < j; k++) {
                                CurvePathDataBP[pathIndex][(i + k) % pathPointCount] = RIGHT_LEANING_CURVE;
                            }
                            i += j;
                            currentSection += j;
                            j = pathPointCount;
                            break;
                        case LEFT_LEANING_CURVE:
                        case LEFT_CURVE:
                            for (k = 0; k < j; k++) {
                                CurvePathDataBP[pathIndex][(i + k) % pathPointCount] = LEFT_LEANING_CURVE;
                            }
                            i += j;
                            currentSection += j;
                            j = pathPointCount;
                            break;
                    }
                }
            }
        }
    }
}

// Seemingly calculates the atan2 angle between a pathPoint and its forward neighbor
s16 angle_chk_BP(s32 pathIndex, s32 pathPointIndex) {
    s16 ret;
    Vec3f sp30;
    Vec3f sp24;
    CenterPathStruct* temp_v0;

    temp_v0 = &CenterPathBP[pathIndex][pathPointIndex];
    sp30[0] = temp_v0->pointx;
    sp30[1] = temp_v0->pointy;
    sp30[2] = temp_v0->pointz;
    temp_v0 = &CenterPathBP[pathIndex][(pathPointIndex + 1) % g_courseTotalPathPoints[pathIndex]];
    sp24[0] = temp_v0->pointx;
    sp24[1] = temp_v0->pointy;
    sp24[2] = temp_v0->pointz;
    ret = CalcDirection(sp30, sp24);
    return -ret;
}

// Populates AnglePathDataBP
void AngleDataCalcBP(s32 pathIndex) {
    s32 pathPointIndex;
    u16* angle;

    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        for (angle = (u16*) &AnglePathDataBP[pathIndex][0], pathPointIndex = 0;
             pathPointIndex < g_courseTotalPathPoints[pathIndex]; pathPointIndex++, angle++) {
            *angle = angle_chk_BP(pathIndex, pathPointIndex);
        }
    }
}

void ShortcutDataCalcBP(s32 pathIndex) {
    s16* trackCurveCount;
    s16 curveCount;
    s16 temp_t0;
    s32 pathPointCount;
    s16* trackSectionType;
    s32 i, j;

    pathPointCount = g_courseTotalPathPoints[pathIndex];
    trackSectionType = CurvePathDataBP[pathIndex];
    trackCurveCount = ShortCutPathDataBP[pathIndex];

    for (i = 0; i < pathPointCount; i++) {
        curveCount = 0;
        for (j = 0; j < pathPointCount; j++) {
            temp_t0 = trackSectionType[(i + j) % pathPointCount];
            if ((temp_t0 == LEFT_LEANING_CURVE) || (temp_t0 == RIGHT_LEANING_CURVE)) {
                curveCount += 1;
            } else {
                break;
            }
            if (g_courseID == COURSE_AWARD_CEREMONY) {
                break;
            }
        }
        *trackCurveCount = curveCount;
        trackCurveCount++;
    }
}

f32 calc_cy_init(f32 arg0, f32 arg1, f32 arg2, UNUSED s32 arg3, UNUSED s32 arg4) {
    arg1 = CheckHight(arg0, 2000.0f, arg2);
    CheckBump(&D_80162E70, 1.0f, arg0, arg1, arg2);
    return arg1;
}

f32 calc_cy(f32 arg0, f32 arg1, f32 arg2, UNUSED s32 arg3, UNUSED s32 arg4) {
    arg1 = CheckHight(arg0, (f32) ((f64) arg1 + 30.0), arg2);
    CheckBump(&D_80162E70, 10.0f, arg0, arg1, arg2);
    return arg1;
}

/**
 * @return's the number of items processed.
 */
s32 area_bunkatu_sub(CenterPathStruct* pathDest, CenterPathStruct* path, s32 numPathPoints, UNUSED s32 pathIndex) {
    f32 temp_f24_2;
    f32 temp_f2_3;
    f32 var_f20_2;
    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 x3;
    f32 z3;
    f32 x1_2;
    f32 z1_3;
    f32 temp_f20;
    f32 var_f30;
    f32 temp_f22;
    f32 temp_f16;
    s32 i;

    f32 j;
    s32 var_s0;
    f32 var_f28;

    CenterPathStruct* point1;
    CenterPathStruct* point2;
    CenterPathStruct* point3;
    f32 temp;
    UNUSED CenterPathStruct* dest;
    var_f30 = 0.0f;
    var_s0 = 0;
    temp_f20 = (f32) path[0].pointx;
    temp_f22 = (f32) path[0].pointz;
    var_f28 = calc_cy_init(temp_f20, 2000.0f, temp_f22, g_courseID, 0);

    for (i = 0; i < numPathPoints; i++) {
        point1 = &path[i % numPathPoints];
        point2 = &path[(i + 1) % numPathPoints];
        point3 = &path[(s32) (i + 2) % numPathPoints];
        x1 = (f32) point1->pointx;
        z1 = (f32) point1->pointz;
        x2 = (f32) point2->pointx;
        z2 = (f32) point2->pointz;
        x3 = (f32) point3->pointx;
        z3 = (f32) point3->pointz;

        temp = 0.05 / (sqrtf(((x2 - x1) * (x2 - x1)) + ((z2 - z1) * (z2 - z1))) +
                       (sqrtf(((x3 - x2) * (x3 - x2)) + ((z3 - z2) * (z3 - z2)))));

        for (j = 0.0f; j <= 1.0; j += temp) {

            temp_f2_3 = (f32) ((1.0 - j) * 0.5 * (1.0 - j));
            z1_3 = (f32) (((1.0 - j) * j) + 0.5);
            temp_f16 = (f32) (j * 0.5 * j);

            temp_f24_2 = (temp_f2_3 * x1) + (z1_3 * x2) + (temp_f16 * x3);
            x1_2 = (temp_f2_3 * z1) + (z1_3 * z2) + (temp_f16 * z3);

            var_f30 +=
                sqrtf(((temp_f24_2 - temp_f20) * (temp_f24_2 - temp_f20)) + ((x1_2 - temp_f22) * (x1_2 - temp_f22)));

            temp_f20 = temp_f24_2;
            temp_f22 = x1_2;

            if ((var_f30 > 20.0f) || ((i == 0) && (j == 0.0))) {
                if (g_ScreenFlip) {
                    // temp_f12 = -temp_f24_2;
                    pathDest->pointx = (s16) -temp_f24_2;
                    var_f20_2 = calc_cy(-temp_f24_2, var_f28, x1_2, g_courseID, var_s0);
                } else {
                    pathDest->pointx = (s16) temp_f24_2;
                    var_f20_2 = calc_cy(temp_f24_2, var_f28, x1_2, g_courseID, var_s0);
                }

                pathDest->pointz = (s16) temp_f22;
                pathDest->unknown = CheckArea(D_80162E70.last_zx);

                if (var_f20_2 < -500.0) {
                    var_f20_2 = var_f28;
                } else {

                    switch (g_courseID) {
                        case COURSE_RAINBOW_ROAD:
                            if (var_f20_2 < (var_f28 - 15.0)) {
                                var_f20_2 = (f32) var_f28 - 15.0;
                            }
                            break;
                        case COURSE_WARIO_STADIUM:
                            if ((var_s0 >= 1140) && (var_s0 <= 1152)) {
                                var_f20_2 = var_f28;
                            } else {
                                if (var_f20_2 < (var_f28 - 10.0)) {
                                    var_f20_2 = (f32) (var_f28 - 4.0);
                                }
                            }
                            break;
                        case COURSE_DK_JUNGLE:
                            if ((var_s0 > 204) && (var_s0 < 220)) {
                                var_f20_2 = var_f28;
                            } else {
                                if (var_f20_2 < (var_f28 - 10.0)) {
                                    var_f20_2 = (f32) (var_f28 - 4.0);
                                }
                            }
                            break;
                        default:
                            if (var_f20_2 < (var_f28 - 10.0)) {
                                var_f20_2 = (f32) var_f28 - 10.0;
                            }
                            break;
                    }
                }
                var_f28 = var_f20_2;
                pathDest->pointy = (s16) (s32) var_f20_2;
                var_f30 = 0.0f;
                pathDest++;
                var_s0 += 1;
            }
        }
    }
    return var_s0;
}

// Returns number of path processed.
s32 area_read_rom_sub(CenterPathStruct* dest, CenterPathStruct* src) {
    s16 temp_a0;
    s16 temp_a2;
    s16 temp_a3;
    s32 var_v0;
    s32 var_v1;
    u16 temp_t0;

    var_v1 = 0;
    for (var_v0 = 0; var_v0 < 0x7D0; var_v0++) {
        temp_a0 = src->pointx;
        temp_a2 = src->pointy;
        temp_a3 = src->pointz;
        temp_t0 = src->unknown;
        src++;
        if (((temp_a0 & 0xFFFF) == 0x8000) && ((temp_a2 & 0xFFFF) == 0x8000) && ((temp_a3 & 0xFFFF) == 0x8000)) {
            break;
        }
        if (g_ScreenFlip != 0) {
            dest->pointx = -temp_a0;
        } else {
            dest->pointx = temp_a0;
        }
        var_v1++;
        dest->pointy = temp_a2;
        dest->pointz = temp_a3;
        dest->unknown = temp_t0;
        dest++;
    }
    return var_v1;
}

s32 area_bunkatu_SL_SHIP_sub(Path2D* pathDest, CenterPathStruct* pathSrc, s32 numPathPoints) {
    f32 temp_f14_3;
    f32 temp_f16_2;
    UNUSED s32 pad;

    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 x3;
    f32 z3;

    UNUSED s32 pad2;
    f32 temp_f24;

    f32 spA8;
    f32 temp_f26;
    f32 spA0;

    f32 temp_f2_3;

    CenterPathStruct* point1;
    f32 j;
    CenterPathStruct* point2;
    CenterPathStruct* point3;
    s32 i;
    f32 temp_f6 = 0.0f;
    s32 nbElement;
    f32 sp7C;

    spA8 = pathSrc[0].pointx;
    spA0 = pathSrc[0].pointz;
    nbElement = 0;

    for (i = 0; i < numPathPoints; i++) {
        point1 = &pathSrc[((i % numPathPoints))];
        point2 = &pathSrc[(((i + 1) % numPathPoints))];
        point3 = &pathSrc[(((i + 2) % numPathPoints))];
        x1 = point1->pointx;
        z1 = point1->pointz;
        x2 = point2->pointx;
        z2 = point2->pointz;
        x3 = point3->pointx;
        z3 = point3->pointz;

        sp7C = 0.05 / (sqrtf(((x2 - x1) * (x2 - x1)) + ((z2 - z1) * (z2 - z1))) +
                       sqrtf(((x3 - x2) * (x3 - x2)) + ((z3 - z2) * (z3 - z2))));

        for (j = 0.0f; j <= 1.0; j += sp7C) {
            temp_f2_3 = (1.0 - j) * 0.5 * (1.0 - j);
            temp_f14_3 = ((1.0 - j) * j) + 0.5;
            temp_f16_2 = j * 0.5 * j;

            temp_f24 = (temp_f2_3 * x1) + (temp_f14_3 * x2) + (temp_f16_2 * x3);
            temp_f26 = (temp_f2_3 * z1) + (temp_f14_3 * z2) + (temp_f16_2 * z3);
            temp_f6 += sqrtf(((temp_f24 - spA8) * (temp_f24 - spA8)) + ((temp_f26 - spA0) * (temp_f26 - spA0)));
            spA8 = temp_f24;
            spA0 = temp_f26;
            if ((temp_f6 > 20.0f) || ((i == 0) && (j == 0.0))) {
                if (g_ScreenFlip) {
                    pathDest->x = (s16) -spA8;
                } else {
                    pathDest->x = (s16) spA8;
                }
                pathDest->z = spA0;
                nbElement += 1;
                pathDest++;
                temp_f6 = 0.0f;
            }
        }
    }
    return nbElement;
}
