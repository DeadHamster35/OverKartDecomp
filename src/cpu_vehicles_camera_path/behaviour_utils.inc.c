void init_act_dat_table(void) {
    s32 i;
    for (i = 0; i < NUM_COURSES - 1; i++) {
        gCoursesCPUBehaviour[i] = GET_COURSE_AIBehaviour;
    }
}

void action_flg_sleep(s32 playerIndex) {
    next_action_num[playerIndex] = 0;
    old_action_num[playerIndex] = 0;
    g_actionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_NONE;
}

void action_flg_wakeup(s32 playerIndex) {
    next_action_num[playerIndex] = 0;
    old_action_num[playerIndex] = 0;
    g_actionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
}

void ActionStartCheck(s32 playerId, Player* player) {
    u16 playerPathPoint;
    s16 pathPointStart;
    s16 pathPointEnd;
    s32 behaviourType;
    UNUSED s32 test;

    ActPointPtr = &gCoursesCPUBehaviour[g_courseID][next_action_num[playerId]];

    playerPathPoint = gNearestPathPointByPlayerId[playerId];

    pathPointStart = ActPointPtr->start;
    pathPointEnd = ActPointPtr->end;
    behaviourType = ActPointPtr->action;

    if ((pathPointStart == -1) && (pathPointEnd == -1)) {
        ActPointPtr = &gCoursesCPUBehaviour[g_courseID][0];
        action_flg_sleep(playerId);
        return;
    }
    if ((u32) playerPathPoint == (u32) pathPointStart) {
        g_actionFlg[playerId] = CPU_BEHAVIOUR_STATE_RUNNING;
        old_action_num[playerId] = next_action_num[playerId];
        next_action_num[playerId]++;
        switch (behaviourType) {
            case BEHAVIOUR_1:
                act_drift_start(playerId, player, player->nowsterr >> 16, playerPathPoint);
                break;
            case BEHAVIOUR_HOP:
                jump_set(player);
                player->slip_flag &= ~DRIFT;
                g_driftFlg[playerId] = 0;
                break;
            case BEHAVIOUR_DRIVE_CENTER:
                LaneData[playerId].lane_target = 0.0f;
                break;
            case BEHAVIOUR_DRIVE_LEFT:
                LaneData[playerId].lane_target = -0.6f;
                break;
            case BEHAVIOUR_DRIVE_OUTER:
                LaneData[playerId].lane_target = 0.6f;
                break;
            case BEHAVIOUR_NORMAL_SPEED:
                enemy_accel_mode[playerId] = SPEED_CPU_BEHAVIOUR_NORMAL;
                break;
            case BEHAVIOUR_FAST_SPEED:
                enemy_accel_mode[playerId] = SPEED_CPU_BEHAVIOUR_FAST;
                break;
            case BEHAVIOUR_SLOW_SPEED:
                enemy_accel_mode[playerId] = SPEED_CPU_BEHAVIOUR_SLOW;
                break;
            case BEHAVIOUR_MAX_SPEED:
                enemy_accel_mode[playerId] = SPEED_CPU_BEHAVIOUR_MAX;
                break;
            case BEHAVIOUR_9:
                g_noSimpleKartFlag[playerId] = 1;
                tenuki_flg[playerId] = false;
                gPlayers[playerId].slip_flag &= ~CENTER_LINE;
                break;
            case BEHAVIOUR_10:
                g_noSimpleKartFlag[playerId] = 0;
                break;
        }
    }
}

void action_end_check(s32 playerIndex, Player* player) {
    u16 nearestPathPoint;
    u32 pathPointEnd;
    s32 behaviourType;

    ActPointPtr = &gCoursesCPUBehaviour[g_courseID][old_action_num[playerIndex]];
    nearestPathPoint = gNearestPathPointByPlayerId[playerIndex];
    behaviourType = ActPointPtr->action;
    pathPointEnd = ActPointPtr->end;
    if (nearestPathPoint >= pathPointEnd) {
        switch (behaviourType) {
            case BEHAVIOUR_1:
                player->slip_flag &= ~DRIFT;
                g_driftFlg[playerIndex] = 0;
                g_actionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            case BEHAVIOUR_DRIVE_CENTER:
            case BEHAVIOUR_DRIVE_LEFT:
            case BEHAVIOUR_DRIVE_OUTER:
                LaneData[playerIndex].lane_target =
                    LaneData[playerIndex].lane_free_drive;
                g_actionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            case BEHAVIOUR_HOP:
            case BEHAVIOUR_NORMAL_SPEED:
            case BEHAVIOUR_FAST_SPEED:
            case BEHAVIOUR_SLOW_SPEED:
            case BEHAVIOUR_9:
            case BEHAVIOUR_10:
            case BEHAVIOUR_MAX_SPEED:
                g_actionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            default:
                break;
        }
    }
}

void action_start_end_chk(s32 playerIndex) {
    Player* player = gPlayerOne + playerIndex;

    switch (g_actionFlg[playerIndex]) {
        case CPU_BEHAVIOUR_STATE_NONE:
            break;
        case CPU_BEHAVIOUR_STATE_START:
            ActionStartCheck(playerIndex, player);
            break;
        case CPU_BEHAVIOUR_STATE_RUNNING:
            action_end_check(playerIndex, player);
            break;
    }
}

void act_drift_start(s32 playerIndex, Player* player, s32 arg2, UNUSED u16 arg3) {
    if ((((player->speed / 18.0f) * 216.0f) >= 45.0f) && (g_driftFlg[playerIndex] == 0)) {
        switch (curve_data[OGAArea]) {
            case RIGHT_LEANING_CURVE:
            case RIGHT_CURVE:
                if ((arg2 >= -9) && (avoid_mode[playerIndex] == 0)) {
                    if ((zure_data[playerIndex] > -0.8) && (zure_data[playerIndex] < 0.5)) {
                        jump_set(player);
                        player->slip_flag |= DRIFT;
                        g_driftFlg[playerIndex] = 1;
                        break;
                    }
                }
                g_driftFlg[playerIndex] = 2;
                break;
            case LEFT_LEANING_CURVE:
            case LEFT_CURVE:
                if ((arg2 < 0xA) && (avoid_mode[playerIndex] == 0)) {
                    if ((zure_data[playerIndex] > -0.5) && (zure_data[playerIndex] < 0.8)) {
                        jump_set(player);
                        player->slip_flag |= DRIFT;
                        g_driftFlg[playerIndex] = -1;
                        break;
                    }
                }
                g_driftFlg[playerIndex] = -2;
                break;
        }
    } else {
        g_driftFlg[playerIndex] = 3;
    }
}
