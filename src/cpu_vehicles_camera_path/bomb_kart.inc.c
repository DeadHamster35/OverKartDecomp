void InitBOM(void) {
    UNUSED Collision* var_s2;
    f32 StartingXPos;
    f32 StartingZPos;
    f32 startingYPos;
    s32 var_s3;
    CenterPathStruct* temp_v0;
    UNUSED BombKart* var_s0;
    BombKartSpawn* bombKartSpawn;

    for (var_s3 = 0; var_s3 < NUM_BOMB_KARTS_VERSUS; var_s3++) {
        bombKartSpawn = &BombTable[CourseID][var_s3];
        switch (CourseID) {
            case COURSE_YOSHI_VALLEY:
                StartingXPos = bombKartSpawn->StartingXPos;
                StartingZPos = bombKartSpawn->StartingZPos;
                startingYPos = CheckHight(StartingXPos, 2000.0f, StartingZPos);
                break;
            case COURSE_AWARD_CEREMONY:
                temp_v0 = &CenterPathBP[3][bombKartSpawn->PathPointIndex];
                StartingXPos = temp_v0->Pointx;
                startingYPos = temp_v0->Pointy;
                StartingZPos = temp_v0->Pointz;
                break;
            default:
                temp_v0 = &CenterPathBP[0][bombKartSpawn->PathPointIndex];
                StartingXPos = temp_v0->Pointx;
                startingYPos = temp_v0->Pointy;
                StartingZPos = temp_v0->Pointz;
                break;
        }
        Bom[var_s3].BombPos[0] = StartingXPos;
        Bom[var_s3].BombPos[1] = startingYPos;
        Bom[var_s3].BombPos[2] = StartingZPos;
        Bom[var_s3].Wheel1Pos[0] = StartingXPos;
        Bom[var_s3].Wheel1Pos[1] = startingYPos;
        Bom[var_s3].Wheel1Pos[2] = StartingZPos;
        Bom[var_s3].Wheel2Pos[0] = StartingXPos;
        Bom[var_s3].Wheel2Pos[1] = startingYPos;
        Bom[var_s3].Wheel2Pos[2] = StartingZPos;
        Bom[var_s3].Wheel3Pos[0] = StartingXPos;
        Bom[var_s3].Wheel3Pos[1] = startingYPos;
        Bom[var_s3].Wheel3Pos[2] = StartingZPos;
        Bom[var_s3].Wheel4Pos[0] = StartingXPos;
        Bom[var_s3].Wheel4Pos[1] = startingYPos;
        Bom[var_s3].Wheel4Pos[2] = StartingZPos;
        Bom[var_s3].PathPointIndex = bombKartSpawn->PathPointIndex;
        Bom[var_s3].unk_3C = bombKartSpawn->unk_04;
        Bom[var_s3].BounceTimer = 0;
        Bom[var_s3].CircleTimer = 0;
        Bom[var_s3].State = bombKartSpawn->StartingState;
        Bom[var_s3].Unk4A = 0;
        Bom[var_s3].Unk4C = 1;
        Bom[var_s3].YPos = startingYPos;
        CheckBump(&BomBump[var_s3], 2.0f, StartingXPos, startingYPos, StartingZPos);
    }
}

void MoveBOM(s32 bombKartId) {
    UNUSED s32 stackPadding0;
    f32 sp118;
    f32 var_f18;
    CenterPathStruct* temp_v0_2;
    f32 temp_f0_3;
    f32 sp108;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    f32 temp_f14;
    UNUSED s32 stackPadding4;
    f32 temp_f16;
    UNUSED s32 stackPadding5;
    UNUSED s32 stackPadding6;
    UNUSED s32 stackPadding7;
    UNUSED s32 stackPadding8;
    UNUSED s32 stackPadding9;
    UNUSED s32 stackPaddingA;
    UNUSED s32 stackPaddingB;
    f32 temp_f0;
    f32 temp_f0_4;
    u16 spCA;
    f32 spC4;
    u16 spC2;
    f32 temp_f12;
    f32 temp_f12_3;
    f32 temp_f12_4;
    f32 temp_f14_2;
    f32 spAC;
    UNUSED s32 stackPaddingC;
    f32 temp_f16_2;
    f32 spA0;
    f32 temp_f2;
    f32 temp_f2_4;
    f32 sp94;
    UNUSED s32 stackPaddingD;
    f32 var_f20;
    f32 sp88;
    f32 var_f22;
    f32 var_f24;
    u16 sp7E;
    u16 sp7C;
    UNUSED u16 sp4C;
    u16 temp_t6;
    u16 temp_t7;
    u16 var_s1;
    s32 var_a0;
    UNUSED s32 stackPaddingE;
    CenterPathStruct* temp_v0_4;
    BombKart* bombKart;
    BombKart* bombKart2;
    Collision* temp_a0_4;
    Player* var_v0;

    bombKart = &Bom[bombKartId];

    sp7E = bombKart->State;

    if (sp7E == 0) {
        return;
    }

    if (((bombKart->Unk4A != 1) || (CourseID == COURSE_AWARD_CEREMONY))) {
        var_f22 = bombKart->BombPos[0];
        var_f20 = bombKart->BombPos[1];
        var_f24 = bombKart->BombPos[2];
        spCA = bombKart->PathPointIndex;
        spC4 = bombKart->unk_3C;
        spC2 = bombKart->SomeRot;
        sp7C = bombKart->BounceTimer;
        var_s1 = bombKart->CircleTimer;
        if ((sp7E != 0) && (sp7E != 4)) {
            if (1) {}
            if (CourseID == COURSE_AWARD_CEREMONY) {
                if (OgaNopointDemoTrg == 1) {
                    var_v0 = PlayerFour;
                    temp_f0 = var_f22 - var_v0->Position[0];
                    temp_f2 = var_f20 - var_v0->Position[1];
                    temp_f12 = var_f24 - var_v0->Position[2];
                    if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f12 * temp_f12)) < 25.0f) {
                        var_s1 = 0;
                        sp7E = 4;
                        var_v0->Weapon |= HIT_BOMB;
                        var_v0->Flag &= ~NO_CONTROLS;
                    }
                }
            } else {

                for (var_a0 = 0; var_a0 < MenuMultiplayerSelection; var_a0++) {
                    var_v0 = &GamePlayers[var_a0];
                    if (!(var_v0->SlipFlag & TERESA)) {
                        temp_f0 = var_f22 - var_v0->Position[0];
                        temp_f2 = var_f20 - var_v0->Position[1];
                        temp_f12 = var_f24 - var_v0->Position[2];
                        if ((((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f12 * temp_f12)) < 25.0f) {
                            sp7E = 4;
                            var_s1 = 0;
                            if (CourseID == COURSE_FRAPPE_SNOWLAND) {
                                var_v0->Weapon |= HIT_BOMB_THROW;
                            } else {
                                var_v0->Weapon |= HIT_BOMB;
                            }
                        }
                    }
                }
            }
        }
        switch (sp7E) {
            case 1:
                var_s1 = (var_s1 + 356) % 360;
                temp_t6 = (var_s1 * 0xFFFF) / 360;
                sp118 = CosT(temp_t6) * 25.0;
                temp_f0_3 = SinT(temp_t6) * 25.0;
                temp_v0_2 = &CenterPathBP[0][spCA];
                var_f22 = temp_v0_2->Pointx + sp118;
                var_f20 = bombKart->YPos + 3.5f;
                var_f24 = temp_v0_2->Pointz + temp_f0_3;
                Object1[0] = var_f22;
                Object1[1] = var_f20;
                Object1[2] = var_f24;
                temp_t7 = (((var_s1 + 1) % 360) * 0xFFFF) / 360;
                sp118 = CosT(temp_t7) * 25.0;
                temp_f0_3 = SinT(temp_t7) * 25.0;
                Object2[0] = temp_v0_2->Pointx + sp118;
                Object2[1] = temp_v0_2->Pointy;
                Object2[2] = temp_v0_2->Pointz + temp_f0_3;
                spC2 = (CalcDirection(Object1, Object2) * 0xFFFF) / 65520;
                break;
            case 2:
                var_s1 = (var_s1 + 4) % 360;
                temp_t6 = (var_s1 * 0xFFFF) / 360;
                sp118 = CosT(temp_t6) * 25.0;
                temp_f0_3 = SinT(temp_t6) * 25.0;
                temp_v0_2 = &CenterPathBP[0][spCA];
                var_f22 = temp_v0_2->Pointx + sp118;
                var_f20 = bombKart->YPos + 3.5f;
                var_f24 = temp_v0_2->Pointz + temp_f0_3;
                Object1[0] = var_f22;
                Object1[1] = var_f20;
                Object1[2] = var_f24;
                temp_t7 = (((var_s1 + 1) % 360) * 0xFFFF) / 360;
                sp118 = CosT(temp_t7) * 25.0;
                temp_f0_3 = SinT(temp_t7) * 25.0;
                Object2[0] = temp_v0_2->Pointx + sp118;
                Object2[1] = temp_v0_2->Pointy;
                Object2[2] = temp_v0_2->Pointz + temp_f0_3;
                spC2 = (CalcDirection(Object1, Object2) * 0xFFFF) / 65520;
                break;
            case 3:
                var_f20 = bombKart->YPos + 3.5f;
                spC2 = 0;
                break;

            case 5:
                if ((OgaHyousyouDemoTrg == 0) || (NearestPathPointByPlayerId[3] < 5)) {
                    break;
                } else {
                    spCA = GetOgaAreaBP(var_f22, var_f20, var_f24, spCA, 3);
                    if ((spCA < 0) || (CourseTotalPathPoints[3] < spCA)) {
                        spCA = 0;
                    }
                    if (((s32) spCA) < 0x1A) {
                        temp_v0_2 = &CenterPathBP[3][(spCA + 1) % CourseTotalPathPoints[3]];
                        Object1[0] = temp_v0_2->Pointx;
                        Object1[1] = temp_v0_2->Pointy;
                        Object1[2] = temp_v0_2->Pointz;
                        temp_v0_4 = &CenterPathBP[3][(spCA + 2) % CourseTotalPathPoints[3]];
                        Object2[0] = temp_v0_4->Pointx;
                        Object2[1] = temp_v0_4->Pointy;
                        Object2[2] = temp_v0_4->Pointz;
                        spC2 = (CalcDirection(Object1, Object2) * 0xFFFF) / 65520;
                    } else {
                        Object1[0] = var_f22;
                        Object1[1] = var_f20;
                        Object1[2] = var_f24;
                        Object2[0] = -2409.197f;
                        Object2[1] = 0.0f;
                        Object2[2] = -355.254f;
                        spC2 = (CalcDirection(Object1, Object2) * 0xFFFF) / 65520;
                    }
                    temp_f14 = ((Object1[0] + Object2[0]) * 0.5f) - var_f22;
                    temp_f16 = ((Object1[2] + Object2[2]) * 0.5f) - var_f24;
                    temp_f0_4 = sqrtf((temp_f14 * temp_f14) + (temp_f16 * temp_f16));
                    if (temp_f0_4 > 0.01f) {
                        var_f22 += (bombKart->unk_3C * temp_f14) / temp_f0_4;
                        var_f24 += (bombKart->unk_3C * temp_f16) / temp_f0_4;
                    } else {
                        var_f22 += temp_f14 / 5.0f;
                        var_f24 += temp_f16 / 5.0f;
                    }
                    temp_a0_4 = &BomBump[bombKartId];
                    var_f20 = CalcHeight(var_f22, 2000.0f, var_f24, temp_a0_4->LastZX) + 3.5f;
                    if (var_f20 < (-1000.0)) {
                        var_f20 = bombKart->BombPos[1];
                    }
                    CheckBump(temp_a0_4, 10.0f, var_f22, var_f20, var_f24);
                }
                break;
            case 4:
                temp_v0_2 = &CenterPathBP[0][spCA];
                Object1[0] = temp_v0_2->Pointx;
                Object1[1] = temp_v0_2->Pointy;
                Object1[2] = temp_v0_2->Pointz;
                temp_v0_4 = &CenterPathBP[0][(spCA + 1) % CourseTotalPathPoints[0]];
                Object2[0] = temp_v0_4->Pointx;
                Object2[1] = temp_v0_4->Pointy;
                Object2[2] = temp_v0_4->Pointz;
                var_f20 += 3.0f - (var_s1 * 0.3f);
                spC2 = (CalcDirection(Object1, Object2) * 0xFFFF) / 65520;
                break;
            default:
                break;
        }

        if (sp7E == 4) {
            sp108 = 2.0f * var_s1;
            sp118 = CosT(0xFFFF - spC2) * var_s1;
            var_f18 = SinT(0xFFFF - spC2) * var_s1;
            var_s1++;
            temp_f2_4 = (var_f20 - 2.3f) + (sp108 / 3.0f);
            spAC = temp_f2_4;
            spA0 = temp_f2_4;
            sp94 = temp_f2_4;
            sp88 = temp_f2_4;
            if (var_s1 >= 31) {
                sp7E = 0;
            }
        } else {
            sp118 = CosT(0xFFFF - spC2) * 1.5f;
            var_f18 = SinT(0xFFFF - spC2) * 1.5f;
            temp_f16_2 = var_f20 - 2.3f;
            temp_f12_3 = (sp7C % 3) * 0.15f;
            temp_f14_2 = temp_f16_2 - temp_f12_3;
            temp_f12_4 = temp_f16_2 + temp_f12_3;
            spAC = temp_f14_2;
            sp94 = temp_f14_2;
            spA0 = temp_f12_4;
            sp88 = temp_f12_4;
            var_f20 += SinT((sp7C * 0x13FFEC) / 360);
            sp7C = (sp7C + 1) % 18;
        }
        bombKart2 = bombKart;
        bombKart2->Wheel1Pos[0] = (sp118 - var_f18) + var_f22;
        bombKart2->Wheel1Pos[1] = spAC;
        bombKart2->Wheel1Pos[2] = (var_f18 + sp118) + var_f24;
        bombKart2->Wheel2Pos[0] = (var_f18 + sp118) + var_f22;
        bombKart2->Wheel2Pos[1] = spA0;
        bombKart2->Wheel2Pos[2] = (var_f18 - sp118) + var_f24;
        bombKart2->Wheel3Pos[0] = ((-sp118) - var_f18) + var_f22;
        bombKart2->Wheel3Pos[1] = sp94;
        bombKart2->Wheel3Pos[2] = ((-var_f18) + sp118) + var_f24;
        bombKart2->Wheel4Pos[0] = ((-sp118) + var_f18) + var_f22;
        bombKart2->Wheel4Pos[1] = sp88;
        bombKart2->Wheel4Pos[2] = ((-var_f18) - sp118) + var_f24;
        bombKart2->BombPos[0] = var_f22;
        bombKart2->BombPos[1] = var_f20;
        bombKart2->BombPos[2] = var_f24;
        bombKart2->PathPointIndex = spCA;
        bombKart2->unk_3C = spC4;
        bombKart2->SomeRot = spC2;
        bombKart2->State = sp7E;
        bombKart2->BounceTimer = sp7C;
        bombKart2->CircleTimer = var_s1;
    }
}
