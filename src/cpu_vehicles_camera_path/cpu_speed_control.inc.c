void EnemyTekagenSub(s32 PlayerId, Player* Player) {
    s16 temp_t1;
    s16 temp_t2;
    s32 var_v0;

    temp_t1 = PlayerPathPointTotalTable[EnemyTargetPlayer];
    temp_t2 = PlayerPathPointTotalTable[PlayerId];

    if (GPCurrentRaceRankByPlayerId[PlayerId] < 2) {
        s16 val1 = GPCurrentRaceRankByPlayerId[EnemyTargetPlayer];
        s16 val2 = temp_t2 - temp_t1;

        if (val2 > 400 && val1 >= 6) {
            Player->SlipFlag &= ~SLIP_STREAM;
            AccelOn(Player);
            RivalOvertakeAllowFlag[PlayerId] = 4;
            return;
        }
    } else {
        Player->SlipFlag |= SLIP_STREAM;
        AccelOn(Player);
        RivalOvertakeAllowFlag[PlayerId] = 3;
        return;
    }

    switch (RaceClass) { // WTF, FAKE ?
        case CC_EXTRA:
            break;
    }

    switch (RaceClass) {
        case CC_50:
            var_v0 = 0;
            if (PlayerId == EnemyRivalNum[0]) {
                var_v0 = 0x14;
            }
            break;

        case CC_100:
            var_v0 = 8;
            if (PlayerId == EnemyRivalNum[0]) {
                var_v0 = 0x18;
            }
            break;

        case CC_150:
            var_v0 = 0x12;
            if (PlayerId == EnemyRivalNum[0]) {
                var_v0 = 0x24;
            }
            break;

        case CC_EXTRA:
            var_v0 = 8;
            if (PlayerId == EnemyRivalNum[0]) {
                var_v0 = 0x18;
            }
            break;

        default:
            var_v0 = 0;
            break;
    }

    if (temp_t2 < temp_t1) {
        Player->SlipFlag |= SLIP_STREAM;
        AccelOn(Player);
        RivalOvertakeAllowFlag[PlayerId] = 1;
    } else if (temp_t2 < (temp_t1 + var_v0 + 0x32)) {
        Player->SlipFlag &= ~SLIP_STREAM;
        AccelOn(Player);
        RivalOvertakeAllowFlag[PlayerId] = 3;
    } else if (TenukiFlg[PlayerId] == false) {
        Player->SlipFlag &= ~SLIP_STREAM;
        AccelOn(Player);
        RivalOvertakeAllowFlag[PlayerId] = 2;
    } else {
        Player->SlipFlag &= ~SLIP_STREAM;
        AccelOff(Player, 1.0f);
        RivalOvertakeAllowFlag[PlayerId] = -1;
    }
}

void OgaResultAccel(s32 PlayerId, Player* Player, f32 Arg2) {
    f32 temp_f0;
    f32 dist;
    f32 temp_f2;
    s32 test;

    temp_f0 = OgaHyousyouMx[PlayerId] - Player->Position[0];
    temp_f2 = OgaHyousyouMz[PlayerId] - Player->Position[2];
    dist = (temp_f0 * temp_f0) + (temp_f2 * temp_f2);
    if (PlayerId == 3) {
        if ((dist < 25.0f) && (OgaNearHyousyouFlg[PlayerId] < 5)) {
            OgaNearHyousyouFlg[PlayerId] = 4;
            (Arg2 < ((2.0 * 18.0) / 216.0)) ? ResultAcceleOn(Player, 1) : AccelOff(Player, 1.0f);
        } else if ((dist < 3600.0f) && (OgaNearHyousyouFlg[PlayerId] < 4)) {
            OgaNearHyousyouFlg[PlayerId] = 3;
            (Arg2 < ((5.0 * 18.0) / 216.0)) ? ResultAcceleOn(Player, 1) : AccelOff(Player, 5.0f);
        } else {
            (Arg2 < ((20.0 * 18.0) / 216.0)) ? ResultAcceleOn(Player, 10) : AccelOff(Player, 1.0f);
        }
    } else {
        if ((dist < 25.0f) && (OgaNearHyousyouFlg[PlayerId] < 5)) {
            OgaNearHyousyouFlg[PlayerId] = 4;
            test = 2;
            (Arg2 < ((test * 18.0) / 216.0)) ? ResultAcceleOn(Player, 1) : AccelOff(Player, 1.0f);
        } else if ((dist < 4900.0f) && (OgaNearHyousyouFlg[PlayerId] < 4)) {
            OgaNearHyousyouFlg[PlayerId] = 3;
            test = 5;
            (Arg2 < ((test * 18.0) / 216.0)) ? ResultAcceleOn(Player, 1) : AccelOff(Player, 15.0f);
        } else if ((dist < 22500.0f) && (OgaNearHyousyouFlg[PlayerId] < 3)) {
            OgaNearHyousyouFlg[PlayerId] = 2;
            test = 20;
            (Arg2 < ((test * 18.0) / 216.0)) ? ResultAcceleOn(Player, 5) : AccelOff(Player, 1.0f);
        } else if ((dist < 90000.0f) && (OgaNearHyousyouFlg[PlayerId] < 2)) {
            OgaNearHyousyouFlg[PlayerId] = 1;
            test = 30;
            (Arg2 < ((test * 18.0) / 216.0)) ? ResultAcceleOn(Player, 6) : AccelOff(Player, 1.0f);
        } else if (OgaNearHyousyouFlg[PlayerId] == 0) {
            test = 35;
            (Arg2 < (((test ^ 0) * 18.0) / 216.0)) ? ResultAcceleOn(Player, 2) : AccelOff(Player, 1.0f);
        } else {
            AccelOff(Player, 1.0f);
        }
    }
}

void OgaAccele(s32 PlayerId, f32 targetSpeed, Player* Player) {
    f32 Speed;

    Speed = Player->Speed;
    if (!(Player->SlipFlag & SPIN_L) && !(Player->SlipFlag & SPIN_R) &&
        !(Player->SlipFlag & THUNDER_SPIN) && !(Player->Weapon & HIT_BOMB) &&
        !(Player->Weapon & HIT_BOMB_THROW) && !(Player->Weapon & HIT_REDSHELL) &&
        !(Player->Weapon & HIT_GREENSHELL)) {
        if (CourseID == COURSE_AWARD_CEREMONY) {
            OgaResultAccel(PlayerId, Player, Speed);
        }
        RubberBandManDefault(PlayerId, Player);
    }
}
