void enemy_tekagen_sub(s32 playerId, Player* player) {
    s16 temp_t1;
    s16 temp_t2;
    s32 var_v0;

    temp_t1 = g_playerPathPointTotalTable[g_EnemyTargetPlayer];
    temp_t2 = g_playerPathPointTotalTable[playerId];

    if (gGPCurrentRaceRankByPlayerId[playerId] < 2) {
        s16 val1 = gGPCurrentRaceRankByPlayerId[g_EnemyTargetPlayer];
        s16 val2 = temp_t2 - temp_t1;

        if (val2 > 400 && val1 >= 6) {
            player->slip_flag &= ~SLIP_STREAM;
            AccelOn(player);
            g_rivalOvertakeAllowFlag[playerId] = 4;
            return;
        }
    } else {
        player->slip_flag |= SLIP_STREAM;
        AccelOn(player);
        g_rivalOvertakeAllowFlag[playerId] = 3;
        return;
    }

    switch (g_raceClass) { // WTF, FAKE ?
        case CC_EXTRA:
            break;
    }

    switch (g_raceClass) {
        case CC_50:
            var_v0 = 0;
            if (playerId == enemy_rival_num[0]) {
                var_v0 = 0x14;
            }
            break;

        case CC_100:
            var_v0 = 8;
            if (playerId == enemy_rival_num[0]) {
                var_v0 = 0x18;
            }
            break;

        case CC_150:
            var_v0 = 0x12;
            if (playerId == enemy_rival_num[0]) {
                var_v0 = 0x24;
            }
            break;

        case CC_EXTRA:
            var_v0 = 8;
            if (playerId == enemy_rival_num[0]) {
                var_v0 = 0x18;
            }
            break;

        default:
            var_v0 = 0;
            break;
    }

    if (temp_t2 < temp_t1) {
        player->slip_flag |= SLIP_STREAM;
        AccelOn(player);
        g_rivalOvertakeAllowFlag[playerId] = 1;
    } else if (temp_t2 < (temp_t1 + var_v0 + 0x32)) {
        player->slip_flag &= ~SLIP_STREAM;
        AccelOn(player);
        g_rivalOvertakeAllowFlag[playerId] = 3;
    } else if (tenuki_flg[playerId] == false) {
        player->slip_flag &= ~SLIP_STREAM;
        AccelOn(player);
        g_rivalOvertakeAllowFlag[playerId] = 2;
    } else {
        player->slip_flag &= ~SLIP_STREAM;
        AccelOff(player, 1.0f);
        g_rivalOvertakeAllowFlag[playerId] = -1;
    }
}

void oga_result_accel(s32 playerId, Player* player, f32 arg2) {
    f32 temp_f0;
    f32 dist;
    f32 temp_f2;
    s32 test;

    temp_f0 = oga_hyousyou_mx[playerId] - player->position[0];
    temp_f2 = oga_hyousyou_mz[playerId] - player->position[2];
    dist = (temp_f0 * temp_f0) + (temp_f2 * temp_f2);
    if (playerId == 3) {
        if ((dist < 25.0f) && (oga_near_hyousyou_flg[playerId] < 5)) {
            oga_near_hyousyou_flg[playerId] = 4;
            (arg2 < ((2.0 * 18.0) / 216.0)) ? result_accele_on(player, 1) : AccelOff(player, 1.0f);
        } else if ((dist < 3600.0f) && (oga_near_hyousyou_flg[playerId] < 4)) {
            oga_near_hyousyou_flg[playerId] = 3;
            (arg2 < ((5.0 * 18.0) / 216.0)) ? result_accele_on(player, 1) : AccelOff(player, 5.0f);
        } else {
            (arg2 < ((20.0 * 18.0) / 216.0)) ? result_accele_on(player, 10) : AccelOff(player, 1.0f);
        }
    } else {
        if ((dist < 25.0f) && (oga_near_hyousyou_flg[playerId] < 5)) {
            oga_near_hyousyou_flg[playerId] = 4;
            test = 2;
            (arg2 < ((test * 18.0) / 216.0)) ? result_accele_on(player, 1) : AccelOff(player, 1.0f);
        } else if ((dist < 4900.0f) && (oga_near_hyousyou_flg[playerId] < 4)) {
            oga_near_hyousyou_flg[playerId] = 3;
            test = 5;
            (arg2 < ((test * 18.0) / 216.0)) ? result_accele_on(player, 1) : AccelOff(player, 15.0f);
        } else if ((dist < 22500.0f) && (oga_near_hyousyou_flg[playerId] < 3)) {
            oga_near_hyousyou_flg[playerId] = 2;
            test = 20;
            (arg2 < ((test * 18.0) / 216.0)) ? result_accele_on(player, 5) : AccelOff(player, 1.0f);
        } else if ((dist < 90000.0f) && (oga_near_hyousyou_flg[playerId] < 2)) {
            oga_near_hyousyou_flg[playerId] = 1;
            test = 30;
            (arg2 < ((test * 18.0) / 216.0)) ? result_accele_on(player, 6) : AccelOff(player, 1.0f);
        } else if (oga_near_hyousyou_flg[playerId] == 0) {
            test = 35;
            (arg2 < (((test ^ 0) * 18.0) / 216.0)) ? result_accele_on(player, 2) : AccelOff(player, 1.0f);
        } else {
            AccelOff(player, 1.0f);
        }
    }
}

void oga_accele(s32 playerId, f32 targetSpeed, Player* player) {
    f32 speed;
    f32 var_f0;
    UNUSED s32 thing;
    s32 var_a1;

    speed = player->speed;
    if (!(player->slip_flag & SPIN_L) && !(player->slip_flag & SPIN_R) &&
        !(player->slip_flag & THUNDER_SPIN) && !(player->weapon & HIT_BOMB) &&
        !(player->weapon & HIT_BOMB_THROW) && !(player->weapon & HIT_REDSHELL) &&
        !(player->weapon & HIT_GREENSHELL)) {
        if (g_courseID == COURSE_AWARD_CEREMONY) {
            oga_result_accel(playerId, player, speed);
        } else if ((fumikiri_stop_flg[playerId] == true) && !(player->slip_flag & (STAR | TERESA))) {
            AccelOff(player, 10.0f);
            if (player->accelcount == 0.0) {
                player->velocity[0] = 0.0f;
                player->velocity[2] = 0.0f;
            }
        } else {
            var_f0 = 3.3333333f;
            switch (g_raceClass) { /* irregular */
                case CC_100:
                case CC_EXTRA:
                    break;
                case CC_50:
                    var_f0 = 2.5f;
                    break;
                case CC_150:
                    var_f0 = 3.75f;
                    break;
            }
            if (speed < var_f0) {
                player->slip_flag &= ~SLIP_STREAM;
                AccelOn(player);
            } else if (player->flag & IS_RACE_FINISH) {
                if (speed < targetSpeed) {
                    player->slip_flag &= ~SLIP_STREAM;
                    AccelOn(player);
                } else {
                    player->slip_flag &= ~SLIP_STREAM;
                    AccelOff(player, 1.0f);
                }
            } else if ((tenuki_flg[playerId] == true) && (oga_car_stat[playerId] != 1)) {
                if (make_kart_distance(playerId, gLapCountByPlayerId[playerId], real_rank[playerId]) ==
                    1) {
                    player->slip_flag |= SLIP_STREAM;
                    AccelOn(player);
                } else {
                    player->slip_flag &= ~SLIP_STREAM;
                    AccelOff(player, 1.0f);
                }
            } else {
                var_a1 = 1;
                switch (enemy_accel_mode[playerId]) { /* switch 1; irregular */
                    case SPEED_CPU_BEHAVIOUR_FAST:      /* switch 1 */
                        player->slip_flag &= ~SLIP_STREAM;
                        AccelOn(player);
                        break;
                    case SPEED_CPU_BEHAVIOUR_MAX: /* switch 1 */
                        player->slip_flag |= SLIP_STREAM;
                        AccelOn(player);
                        break;
                    case SPEED_CPU_BEHAVIOUR_SLOW: /* switch 1 */
                        if (((speed / 18.0f) * 216.0f) > 20.0f) {
                            targetSpeed = 1.6666666f;
                        }
                        var_a1 = 0;
                        break;
                    case SPEED_CPU_BEHAVIOUR_NORMAL: /* switch 1 */
                    default:                         /* switch 1 */
                        var_a1 = 0;
                        break;
                }
                if (var_a1 != 1) {
                    if (speed < targetSpeed) {
                        if ((g_DemoFlag == 1) && (g_courseID != COURSE_AWARD_CEREMONY)) {
                            AccelOn(player);
                        } else if (oga_car_stat[playerId] == 1) {
                            RubberBandManDefault(playerId, player);
                        } else if (make_kart_distance(playerId, gLapCountByPlayerId[playerId],
                                                 real_rank[playerId]) == true) {
                            player->slip_flag |= SLIP_STREAM;
                            AccelOn(player);
                        } else {
                            player->slip_flag &= ~SLIP_STREAM;
                            AccelOff(player, 1.0f);
                        }
                    } else {
                        player->slip_flag &= ~SLIP_STREAM;
                        if (targetSpeed > 1.0f) {
                            AccelOff(player, 2.0f);
                        } else {
                            AccelOff(player, 5.0f);
                        }
                    }
                }
            }
        }
    }
}
