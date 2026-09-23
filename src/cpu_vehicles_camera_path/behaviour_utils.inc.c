void InitActDatTable(void) {
    s32 i;
    for (i = 0; i < NUM_COURSES - 1; i++) {
        CoursesCPUBehaviour[i] = GET_COURSE_AIBehaviour;
    }
}

void ActionFlgSleep(s32 playerIndex) {
    NextActionNum[playerIndex] = 0;
    OldActionNum[playerIndex] = 0;
    ActionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_NONE;
}

void ActionFlgWakeup(s32 playerIndex) {
    NextActionNum[playerIndex] = 0;
    OldActionNum[playerIndex] = 0;
    ActionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
}

void ActionStartCheck(s32 PlayerId, Player* Player) {
    u16 playerPathPoint;
    s16 pathPointStart;
    s16 pathPointEnd;
    s32 behaviourType;
    UNUSED s32 test;

    ActPointPtr = &CoursesCPUBehaviour[CourseID][NextActionNum[PlayerId]];

    playerPathPoint = NearestPathPointByPlayerId[PlayerId];

    pathPointStart = ActPointPtr->Start;
    pathPointEnd = ActPointPtr->end;
    behaviourType = ActPointPtr->Action;

    if ((pathPointStart == -1) && (pathPointEnd == -1)) {
        ActPointPtr = &CoursesCPUBehaviour[CourseID][0];
        ActionFlgSleep(PlayerId);
        return;
    }
    if ((u32) playerPathPoint == (u32) pathPointStart) {
        ActionFlg[PlayerId] = CPU_BEHAVIOUR_STATE_RUNNING;
        OldActionNum[PlayerId] = NextActionNum[PlayerId];
        NextActionNum[PlayerId]++;
        switch (behaviourType) {
            case BEHAVIOUR_1:
                ActDriftStart(PlayerId, Player, Player->Nowsterr >> 16, playerPathPoint);
                break;
            case BEHAVIOUR_HOP:
                JumpSet(Player);
                Player->SlipFlag &= ~DRIFT;
                DriftFlg[PlayerId] = 0;
                break;
            case BEHAVIOUR_DRIVE_CENTER:
                LaneData[PlayerId].LaneTarget = 0.0f;
                break;
            case BEHAVIOUR_DRIVE_LEFT:
                LaneData[PlayerId].LaneTarget = -0.6f;
                break;
            case BEHAVIOUR_DRIVE_OUTER:
                LaneData[PlayerId].LaneTarget = 0.6f;
                break;
            case BEHAVIOUR_NORMAL_SPEED:
                EnemyAccelMode[PlayerId] = SPEED_CPU_BEHAVIOUR_NORMAL;
                break;
            case BEHAVIOUR_FAST_SPEED:
                EnemyAccelMode[PlayerId] = SPEED_CPU_BEHAVIOUR_FAST;
                break;
            case BEHAVIOUR_SLOW_SPEED:
                EnemyAccelMode[PlayerId] = SPEED_CPU_BEHAVIOUR_SLOW;
                break;
            case BEHAVIOUR_MAX_SPEED:
                EnemyAccelMode[PlayerId] = SPEED_CPU_BEHAVIOUR_MAX;
                break;
            case BEHAVIOUR_9:
                NoSimpleKartFlag[PlayerId] = 1;
                TenukiFlg[PlayerId] = false;
                GamePlayers[PlayerId].SlipFlag &= ~CENTER_LINE;
                break;
            case BEHAVIOUR_10:
                NoSimpleKartFlag[PlayerId] = 0;
                break;
        }
    }
}

void ActionEndCheck(s32 playerIndex, Player* Player) {
    u16 nearestPathPoint;
    u32 pathPointEnd;
    s32 behaviourType;

    ActPointPtr = &CoursesCPUBehaviour[CourseID][OldActionNum[playerIndex]];
    nearestPathPoint = NearestPathPointByPlayerId[playerIndex];
    behaviourType = ActPointPtr->Action;
    pathPointEnd = ActPointPtr->end;
    if (nearestPathPoint >= pathPointEnd) {
        switch (behaviourType) {
            case BEHAVIOUR_1:
                Player->SlipFlag &= ~DRIFT;
                DriftFlg[playerIndex] = 0;
                ActionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            case BEHAVIOUR_DRIVE_CENTER:
            case BEHAVIOUR_DRIVE_LEFT:
            case BEHAVIOUR_DRIVE_OUTER:
                LaneData[playerIndex].LaneTarget =
                    LaneData[playerIndex].LaneFreeDrive;
                ActionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            case BEHAVIOUR_HOP:
            case BEHAVIOUR_NORMAL_SPEED:
            case BEHAVIOUR_FAST_SPEED:
            case BEHAVIOUR_SLOW_SPEED:
            case BEHAVIOUR_9:
            case BEHAVIOUR_10:
            case BEHAVIOUR_MAX_SPEED:
                ActionFlg[playerIndex] = CPU_BEHAVIOUR_STATE_START;
                break;
            default:
                break;
        }
    }
}

void ActionStartEndChk(s32 playerIndex) {
    Player* Player = PlayerOne + playerIndex;

    switch (ActionFlg[playerIndex]) {
        case CPU_BEHAVIOUR_STATE_NONE:
            break;
        case CPU_BEHAVIOUR_STATE_START:
            ActionStartCheck(playerIndex, Player);
            break;
        case CPU_BEHAVIOUR_STATE_RUNNING:
            ActionEndCheck(playerIndex, Player);
            break;
    }
}

void ActDriftStart(s32 playerIndex, Player* Player, s32 Arg2, UNUSED u16 Arg3) {
    if ((((Player->Speed / 18.0f) * 216.0f) >= 45.0f) && (DriftFlg[playerIndex] == 0)) {
        switch (CurveData[OGAArea]) {
            case RIGHT_LEANING_CURVE:
            case RIGHT_CURVE:
                if ((Arg2 >= -9) && (AvoidMode[playerIndex] == 0)) {
                    if ((ZureData[playerIndex] > -0.8) && (ZureData[playerIndex] < 0.5)) {
                        JumpSet(Player);
                        Player->SlipFlag |= DRIFT;
                        DriftFlg[playerIndex] = 1;
                        break;
                    }
                }
                DriftFlg[playerIndex] = 2;
                break;
            case LEFT_LEANING_CURVE:
            case LEFT_CURVE:
                if ((Arg2 < 0xA) && (AvoidMode[playerIndex] == 0)) {
                    if ((ZureData[playerIndex] > -0.5) && (ZureData[playerIndex] < 0.8)) {
                        JumpSet(Player);
                        Player->SlipFlag |= DRIFT;
                        DriftFlg[playerIndex] = -1;
                        break;
                    }
                }
                DriftFlg[playerIndex] = -2;
                break;
        }
    } else {
        DriftFlg[playerIndex] = 3;
    }
}
