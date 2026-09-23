// @arg Index from 0 to 3.

// Processes course path by Index.
// @arg Index from 0 to 3.
// Each course can have 1-4 course paths.
void AreaBunkatuBP(s32 PathIndex) {

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
        pathDest = CenterPathBP[PathIndex];
        bInvalidPath = 1;
        if (CourseID != COURSE_AWARD_CEREMONY) {
            var_v0 = AreaReadROMSub(pathDest, GET_COURSE_PathTable2(PathIndex));
            CourseTotalPathPoints[PathIndex] = (u16) var_v0;
        } else {
            // Course path included in course_data which has already been Loaded into memory.
            // This is how we get the Addr to our path data.
            path = GET_COURSE_PathTable(PathIndex);
            ptr = path;

            for (i = 0; i < 3000; i++, ptr++) {
                if ((u16) ptr->Pointx == 0x8000) {
                    sp24 = i - 1;
                    bInvalidPath = 0;
                    break;
                }
            }

            // If path data higher than 3000 something has gone wrong.
            // Skip processing the data.
            //! @todo Confirm this comment
            if (!bInvalidPath) {
                var_v0 = AreaBunkatuSub(pathDest, path, sp24, PathIndex);
                CourseTotalPathPoints[PathIndex] = (u16) var_v0;
            }
        }
    }
}

void SidePointCalcBP(s32 PathIndex) {
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
    s32 PathPointIndex;
    CenterPathStruct* pathPoint;
    CenterPathStruct* nextPathPoint;
    CenterPathStruct* var_s1;
    CenterPathStruct* var_s2;

    if (((s32) GET_COURSE_AIMaximumSeparation) >= 0) {
        pathPointWidth = GET_COURSE_AIMaximumSeparation;
        pathPoint = &CenterPathBP[PathIndex][0];
        var_s1 = &SideLPathBP[PathIndex][0];
        var_s2 = &SideRPathBP[PathIndex][0];
        for (PathPointIndex = 0; PathPointIndex < CourseTotalPathPoints[PathIndex];
             PathPointIndex++, var_s1++, var_s2++) {
            x1 = pathPoint->Pointx;
            y1 = pathPoint->Pointy;
            z1 = pathPoint->Pointz;
            pathPoint++;
            nextPathPoint = &CenterPathBP[PathIndex][(PathPointIndex + 1) % ((s32) CourseTotalPathPoints[PathIndex])];
            x2 = nextPathPoint->Pointx;
            y2 = nextPathPoint->Pointy;
            z2 = nextPathPoint->Pointz;
            x_dist = x2 - x1;
            z_dist = z2 - z1;
            neg_x_dist = x1 - x2;
            neg_z_dist = z1 - z2;
            xz_dist = sqrtf((x_dist * x_dist) + (z_dist * z_dist));
            temp_f16 = (f32) ((y1 + y2) * 0.5);

            // Calculate Left boundary Position
            // Uses perpendicular vector (Z, -X) normalized by segment Length
            var_s1->Pointx = ((pathPointWidth * z_dist) / xz_dist) + x1;
            var_s1->Pointy = temp_f16;
            var_s1->Pointz = ((pathPointWidth * neg_x_dist) / xz_dist) + z1;

            // Calculate Right boundary Position
            // Uses opposite perpendicular vector (-Z, X)
            var_s2->Pointx = ((pathPointWidth * neg_z_dist) / xz_dist) + x1;
            var_s2->Pointy = temp_f16;
            var_s2->Pointz = ((pathPointWidth * x_dist) / xz_dist) + z1;
        }
    }
}

/**
 * Calculates the track curvature using two sets of three path.
 * Returns a normalized Value indicating Curve Direction and severity:
 * - Positive: Right turn
 * - Negative: Left turn
 * - Near zero: Straight section
 *
 * @Param PathIndex Track path Index
 * @Param PathPointIndex Starting pathPoint for calculation
 * @return Normalized curvature Value (-1 to 1)
 */
f32 CurveChkBP(s32 PathIndex, u16 PathPointIndex) {
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
    pathPointCount = CourseTotalPathPoints[PathIndex];
    pathPathPoints = CenterPathBP[PathIndex];

    pathPoint1 = &pathPathPoints[PathPointIndex];
    pathPoint2 = &pathPathPoints[(PathPointIndex + 1) % pathPointCount];
    pathPoint3 = &pathPathPoints[(PathPointIndex + 2) % pathPointCount];

    x1 = pathPoint1->Pointx;
    z1 = pathPoint1->Pointz;
    x2 = pathPoint2->Pointx;
    z2 = pathPoint2->Pointz;
    x3 = pathPoint3->Pointx;
    z3 = pathPoint3->Pointz;

    firstVectorX = (((x2 + x3) * 0.5) - x1);
    firstVectorZ = (((z2 + z3) * 0.5) - z1);

    pathPoint1 = &pathPathPoints[(PathPointIndex + 3) % pathPointCount];
    pathPoint2 = &pathPathPoints[(PathPointIndex + 4) % pathPointCount];
    pathPoint3 = &pathPathPoints[(PathPointIndex + 5) % pathPointCount];

    x1 = pathPoint1->Pointx;
    z1 = pathPoint1->Pointz;
    x2 = pathPoint2->Pointx;
    z2 = pathPoint2->Pointz;
    x3 = pathPoint3->Pointx;
    z3 = pathPoint3->Pointz;

    secondVectorX = (((x2 + x3) * 0.5) - x1);
    secondVectorZ = (((z2 + z3) * 0.5) - z1);

    firstLength = sqrtf((firstVectorZ * firstVectorZ) + (firstVectorX * firstVectorX));
    secondLength = sqrtf((secondVectorX * secondVectorX) + (secondVectorZ * secondVectorZ));
    return -((firstVectorZ * secondVectorX) - (firstVectorX * secondVectorZ)) / (secondLength * firstLength);
}

void CurveDataCalcBP(s32 PathIndex) {
    f64 sectionCurvature;
    UNUSED s32 pad;
    s32 k;
    s32 i;
    s32 j;
    s16* currentSection;
    s32 pathPointCount;
    s16* nextSection;

    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        pathPointCount = CourseTotalPathPoints[PathIndex];
        currentSection = CurvePathDataBP[PathIndex];
        for (i = 0; i < pathPointCount; i++, currentSection++) {
            sectionCurvature = CurveChkBP(PathIndex, i);
            *currentSection = STRAIGHT;
            if (sectionCurvature > 0.1) {
                *currentSection = RIGHT_CURVE;
            }
            if (sectionCurvature < -0.1) {
                *currentSection = LEFT_CURVE;
            }
        }
        currentSection = CurvePathDataBP[PathIndex];
        for (i = 0; i < pathPointCount; i++, currentSection++) {
            if (*currentSection == STRAIGHT) {
                // Look ahead for Next curved section
                for (j = 1; j < pathPointCount; j++) {
                    nextSection = &CurvePathDataBP[PathIndex][(i + j) % pathPointCount];
                    switch (*nextSection) {
                        case RIGHT_LEANING_CURVE:
                        case RIGHT_CURVE:
                            for (k = 0; k < j; k++) {
                                CurvePathDataBP[PathIndex][(i + k) % pathPointCount] = RIGHT_LEANING_CURVE;
                            }
                            i += j;
                            currentSection += j;
                            j = pathPointCount;
                            break;
                        case LEFT_LEANING_CURVE:
                        case LEFT_CURVE:
                            for (k = 0; k < j; k++) {
                                CurvePathDataBP[PathIndex][(i + k) % pathPointCount] = LEFT_LEANING_CURVE;
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

// Seemingly calculates the atan2 Angle between a pathPoint and its forward neighbor
s16 AngleChkBP(s32 PathIndex, s32 PathPointIndex) {
    s16 ret;
    Vec3f sp30;
    Vec3f sp24;
    CenterPathStruct* temp_v0;

    temp_v0 = &CenterPathBP[PathIndex][PathPointIndex];
    sp30[0] = temp_v0->Pointx;
    sp30[1] = temp_v0->Pointy;
    sp30[2] = temp_v0->Pointz;
    temp_v0 = &CenterPathBP[PathIndex][(PathPointIndex + 1) % CourseTotalPathPoints[PathIndex]];
    sp24[0] = temp_v0->Pointx;
    sp24[1] = temp_v0->Pointy;
    sp24[2] = temp_v0->Pointz;
    ret = CalcDirection(sp30, sp24);
    return -ret;
}

// Populates AnglePathDataBP
void AngleDataCalcBP(s32 PathIndex) {
    s32 PathPointIndex;
    u16* Angle;

    if ((s32) GET_COURSE_AIMaximumSeparation >= 0) {
        for (Angle = (u16*) &AnglePathDataBP[PathIndex][0], PathPointIndex = 0;
             PathPointIndex < CourseTotalPathPoints[PathIndex]; PathPointIndex++, Angle++) {
            *Angle = AngleChkBP(PathIndex, PathPointIndex);
        }
    }
}

void ShortcutDataCalcBP(s32 PathIndex) {
    s16* trackCurveCount;
    s16 curveCount;
    s16 temp_t0;
    s32 pathPointCount;
    s16* trackSectionType;
    s32 i, j;

    pathPointCount = CourseTotalPathPoints[PathIndex];
    trackSectionType = CurvePathDataBP[PathIndex];
    trackCurveCount = ShortCutPathDataBP[PathIndex];

    for (i = 0; i < pathPointCount; i++) {
        curveCount = 0;
        for (j = 0; j < pathPointCount; j++) {
            temp_t0 = trackSectionType[(i + j) % pathPointCount];
            if ((temp_t0 == LEFT_LEANING_CURVE) || (temp_t0 == RIGHT_LEANING_CURVE)) {
                curveCount += 1;
            } else {
                break;
            }
            if (CourseID == COURSE_AWARD_CEREMONY) {
                break;
            }
        }
        *trackCurveCount = curveCount;
        trackCurveCount++;
    }
}

f32 CalcCyInit(f32 arg0, f32 arg1, f32 Arg2, UNUSED s32 Arg3, UNUSED s32 arg4) {
    arg1 = CheckHight(arg0, 2000.0f, Arg2);
    CheckBump(&D_80162E70, 1.0f, arg0, arg1, Arg2);
    return arg1;
}

f32 CalcCy(f32 arg0, f32 arg1, f32 Arg2, UNUSED s32 Arg3, UNUSED s32 arg4) {
    arg1 = CheckHight(arg0, (f32) ((f64) arg1 + 30.0), Arg2);
    CheckBump(&D_80162E70, 10.0f, arg0, arg1, Arg2);
    return arg1;
}

/**
 * @return's the Number of items processed.
 */
s32 AreaBunkatuSub(CenterPathStruct* pathDest, CenterPathStruct* path, s32 numPathPoints, UNUSED s32 PathIndex) {
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
    temp_f20 = (f32) path[0].Pointx;
    temp_f22 = (f32) path[0].Pointz;
    var_f28 = CalcCyInit(temp_f20, 2000.0f, temp_f22, CourseID, 0);

    for (i = 0; i < numPathPoints; i++) {
        point1 = &path[i % numPathPoints];
        point2 = &path[(i + 1) % numPathPoints];
        point3 = &path[(s32) (i + 2) % numPathPoints];
        x1 = (f32) point1->Pointx;
        z1 = (f32) point1->Pointz;
        x2 = (f32) point2->Pointx;
        z2 = (f32) point2->Pointz;
        x3 = (f32) point3->Pointx;
        z3 = (f32) point3->Pointz;

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
                if (ScreenFlip) {
                    // temp_f12 = -temp_f24_2;
                    pathDest->Pointx = (s16) -temp_f24_2;
                    var_f20_2 = CalcCy(-temp_f24_2, var_f28, x1_2, CourseID, var_s0);
                } else {
                    pathDest->Pointx = (s16) temp_f24_2;
                    var_f20_2 = CalcCy(temp_f24_2, var_f28, x1_2, CourseID, var_s0);
                }

                pathDest->Pointz = (s16) temp_f22;
                pathDest->Unknown = CheckArea(D_80162E70.LastZX);

                if (var_f20_2 < -500.0) {
                    var_f20_2 = var_f28;
                } else {

                    switch (CourseID) {
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
                pathDest->Pointy = (s16) (s32) var_f20_2;
                var_f30 = 0.0f;
                pathDest++;
                var_s0 += 1;
            }
        }
    }
    return var_s0;
}

// Returns Number of path processed.
s32 AreaReadROMSub(CenterPathStruct* dest, CenterPathStruct* src) {
    s16 temp_a0;
    s16 temp_a2;
    s16 temp_a3;
    s32 var_v0;
    s32 var_v1;
    u16 temp_t0;

    var_v1 = 0;
    for (var_v0 = 0; var_v0 < 0x7D0; var_v0++) {
        temp_a0 = src->Pointx;
        temp_a2 = src->Pointy;
        temp_a3 = src->Pointz;
        temp_t0 = src->Unknown;
        src++;
        if (((temp_a0 & 0xFFFF) == 0x8000) && ((temp_a2 & 0xFFFF) == 0x8000) && ((temp_a3 & 0xFFFF) == 0x8000)) {
            break;
        }
        if (ScreenFlip != 0) {
            dest->Pointx = -temp_a0;
        } else {
            dest->Pointx = temp_a0;
        }
        var_v1++;
        dest->Pointy = temp_a2;
        dest->Pointz = temp_a3;
        dest->Unknown = temp_t0;
        dest++;
    }
    return var_v1;
}

s32 AreaBunkatuSLSHIPSub(Path2D* pathDest, CenterPathStruct* pathSrc, s32 numPathPoints) {
    f32 temp_f14_3;
    f32 temp_f16_2;
    UNUSED s32 pad;

    f32 x1;
    f32 z1;
    f32 x2;
    f32 z2;
    f32 x3;
    f32 z3;

    UNUSED s32 Pad2;
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

    spA8 = pathSrc[0].Pointx;
    spA0 = pathSrc[0].Pointz;
    nbElement = 0;

    for (i = 0; i < numPathPoints; i++) {
        point1 = &pathSrc[((i % numPathPoints))];
        point2 = &pathSrc[(((i + 1) % numPathPoints))];
        point3 = &pathSrc[(((i + 2) % numPathPoints))];
        x1 = point1->Pointx;
        z1 = point1->Pointz;
        x2 = point2->Pointx;
        z2 = point2->Pointz;
        x3 = point3->Pointx;
        z3 = point3->Pointz;

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
                if (ScreenFlip) {
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
