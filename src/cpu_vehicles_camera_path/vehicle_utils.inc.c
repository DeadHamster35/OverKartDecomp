#define GET_PATH_LENGTH(pathPoint)               \
    for (i = 0;; i++) {                          \
        if ((u16) pathPoint[i].pointx == 0x8000) { \
            break;                               \
        }                                        \
    }

void area_bunkatu_SL(void) {
    s32 i;
    Path2D* temp;
    CenterPathStruct* pathPoint =
        (CenterPathStruct*) VIRTUAL_TO_PHYSICAL2(SegmentTable[SEGMENT_NUMBER2(d_course_kalimari_desert_train_path)] +
                                               SEGMENT_OFFSET(d_course_kalimari_desert_train_path));

    GET_PATH_LENGTH(pathPoint)

    temp = center_line_EX;
    g_CourseObstacle = area_bunkatu_SL_SHIP_sub(temp, pathPoint, i - 1);
    D_80162EB0 = CheckHight(temp[0].x, 2000.0f, temp[0].z);
}

void area_bunkatu_SHIP(void) {
    CenterPathStruct* pathPoint;
    s32 i;

    pathPoint =
        (CenterPathStruct*) VIRTUAL_TO_PHYSICAL2(SegmentTable[SEGMENT_NUMBER2(d_course_dks_jungle_parkway_ferry_path)] +
                                               (SEGMENT_OFFSET(d_course_dks_jungle_parkway_ferry_path)));

    GET_PATH_LENGTH(pathPoint)

    g_CourseObstacle = area_bunkatu_SL_SHIP_sub(center_line_EX, pathPoint, i - 1);
    D_80162EB2 = -40;
}

void obj_init_highway_sub(VehicleStuff* vehicle) {
    f32 origXPos;
    UNUSED f32 pad;
    f32 origZPos;

    origXPos = vehicle->position[0];
    origZPos = vehicle->position[2];
    if (highway_gyakusou_flg == false) {
        hanyou_soukou_sub_BP(vehicle->position, (s16*) &vehicle->pathPointIndex, vehicle->speed,
                      vehicle->someMultiplierTheSequel, 0, 3);
        vehicle->rotation[0] = 0;
        vehicle->rotation[1] = -DEGREES(180);
        vehicle->rotation[2] = 0;
    } else {
        hanyou_gyaku_soukou_sub_BP(vehicle->position, (s16*) &vehicle->pathPointIndex, vehicle->speed,
                      vehicle->someMultiplierTheSequel, 0);
        vehicle->rotation[0] = 0;
        vehicle->rotation[1] = 0;
        vehicle->rotation[2] = 0;
    }
    vehicle->velocity[0] = vehicle->position[0] - origXPos;
    vehicle->velocity[2] = vehicle->position[2] - origZPos;
}

void ogawa_hanyou_object_init(void) {
    s16 trainCarYRot;
    UNUSED Vec3f pad;
    TrainCarStuff* tempLocomotive;
    TrainCarStuff* tempTender;
    TrainCarStuff* tempPassengerCar;
    Vec3s trainCarRot;
    VehicleStuff* tempBoxTruck;
    VehicleStuff* tempSchoolBus;
    VehicleStuff* tempTankerTruck;
    VehicleStuff* tempCar;
    PaddleBoatStuff* tempPaddleWheelBoat;
    Vec3s paddleWheelBoatRot;
    s32 loopIndex;
    s32 loopIndex2;
    f32 origXPos;
    f32 origZPos;

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_KALAMARI_DESERT:
            for (loopIndex = 0; loopIndex < NUM_TRAINS; loopIndex++) {
                tempLocomotive = &SL[loopIndex].locomotive;
                origXPos = tempLocomotive->position[0];
                origZPos = tempLocomotive->position[2];
                trainCarYRot = hanyou_soukou_sub_EX(
                    tempLocomotive->position, (s16*) &tempLocomotive->pathPointIndex, SL[loopIndex].speed);
                tempLocomotive->velocity[0] = tempLocomotive->position[0] - origXPos;
                tempLocomotive->velocity[2] = tempLocomotive->position[2] - origZPos;
                SetSVector(trainCarRot, 0, trainCarYRot, 0);
                tempLocomotive->actorIndex = addObjectBuffer(tempLocomotive->position, trainCarRot,
                                                                     tempLocomotive->velocity, ACTOR_TRAIN_ENGINE);

                tempTender = &SL[loopIndex].tender;
                if (tempTender->isActive == 1) {
                    origXPos = tempTender->position[0];
                    origZPos = tempTender->position[2];
                    trainCarYRot = hanyou_soukou_sub_EX(
                        tempTender->position, (s16*) &tempTender->pathPointIndex, SL[loopIndex].speed);
                    tempTender->velocity[0] = tempTender->position[0] - origXPos;
                    tempTender->velocity[2] = tempTender->position[2] - origZPos;
                    SetSVector(trainCarRot, 0, trainCarYRot, 0);
                    tempTender->actorIndex = addObjectBuffer(tempTender->position, trainCarRot,
                                                                     tempTender->velocity, ACTOR_TRAIN_TENDER);
                }

                for (loopIndex2 = 0; loopIndex2 < NUM_PASSENGER_CAR_ENTRIES; loopIndex2++) {
                    tempPassengerCar = &SL[loopIndex].passengerCars[loopIndex2];
                    if (tempPassengerCar->isActive == 1) {
                        origXPos = tempPassengerCar->position[0];
                        origZPos = tempPassengerCar->position[2];
                        trainCarYRot = hanyou_soukou_sub_EX(tempPassengerCar->position,
                                                                     (s16*) &tempPassengerCar->pathPointIndex,
                                                                     SL[loopIndex].speed);
                        tempPassengerCar->velocity[0] = tempPassengerCar->position[0] - origXPos;
                        tempPassengerCar->velocity[2] = tempPassengerCar->position[2] - origZPos;
                        SetSVector(trainCarRot, 0, trainCarYRot, 0);
                        tempPassengerCar->actorIndex =
                            addObjectBuffer(tempPassengerCar->position, trainCarRot, tempPassengerCar->velocity,
                                                    ACTOR_TRAIN_PASSENGER_CAR);
                    }
                }
            }
            break;
        case COURSE_DK_JUNGLE:
            for (loopIndex = 0; loopIndex < NUM_ACTIVE_PADDLE_BOATS; loopIndex++) {
                tempPaddleWheelBoat = &ship[loopIndex];
                if (tempPaddleWheelBoat->isActive == 1) {
                    origXPos = tempPaddleWheelBoat->position[0];
                    origZPos = tempPaddleWheelBoat->position[2];
                    tempPaddleWheelBoat->rotY = hanyou_soukou_sub_EX(
                        tempPaddleWheelBoat->position, (s16*) &tempPaddleWheelBoat->pathPointIndex,
                        tempPaddleWheelBoat->speed);
                    tempPaddleWheelBoat->velocity[0] = tempPaddleWheelBoat->position[0] - origXPos;
                    tempPaddleWheelBoat->velocity[2] = tempPaddleWheelBoat->position[2] - origZPos;
                    SetSVector(paddleWheelBoatRot, 0, tempPaddleWheelBoat->rotY, 0);
                    tempPaddleWheelBoat->actorIndex =
                        addObjectBuffer(tempPaddleWheelBoat->position, paddleWheelBoatRot,
                                                tempPaddleWheelBoat->velocity, ACTOR_PADDLE_BOAT);
                }
            }
            break;
        case COURSE_TOADS_TURNPIKE:
            for (loopIndex = 0; loopIndex < NUM_RACE_BOX_TRUCKS; loopIndex++) {
                tempBoxTruck = &TRUCK[loopIndex];
                obj_init_highway_sub(tempBoxTruck);
                tempBoxTruck->actorIndex = addObjectBuffer(tempBoxTruck->position, tempBoxTruck->rotation,
                                                                   tempBoxTruck->velocity, ACTOR_BOX_TRUCK);
            }
            for (loopIndex = 0; loopIndex < NUM_RACE_SCHOOL_BUSES; loopIndex++) {
                tempSchoolBus = &bus[loopIndex];
                obj_init_highway_sub(tempSchoolBus);
                tempSchoolBus->actorIndex = addObjectBuffer(tempSchoolBus->position, tempSchoolBus->rotation,
                                                                    tempSchoolBus->velocity, ACTOR_SCHOOL_BUS);
            }
            for (loopIndex = 0; loopIndex < NUM_RACE_TANKER_TRUCKS; loopIndex++) {
                tempTankerTruck = &tanklorry[loopIndex];
                obj_init_highway_sub(tempTankerTruck);
                tempTankerTruck->actorIndex =
                    addObjectBuffer(tempTankerTruck->position, tempTankerTruck->rotation,
                                            tempTankerTruck->velocity, ACTOR_TANKER_TRUCK);
            }
            for (loopIndex = 0; loopIndex < NUM_RACE_CARS; loopIndex++) {
                tempCar = &rv_car[loopIndex];
                obj_init_highway_sub(tempCar);
                tempCar->actorIndex =
                    addObjectBuffer(tempCar->position, tempCar->rotation, tempCar->velocity, ACTOR_CAR);
            }
            break;
    }
#else

#endif
}

void clear_SL_tab(TrainCarStuff* trainCar, Path2D* posXZ, u16 pathPoint) {
    trainCar->position[0] = (f32) posXZ->x;
    trainCar->position[1] = (f32) D_80162EB0;
    trainCar->position[2] = (f32) posXZ->z;
    trainCar->actorIndex = -1;
    trainCar->pathPointIndex = pathPoint;
    trainCar->isActive = 0;
    trainCar->velocity[0] = 0.0f;
    trainCar->velocity[1] = 0.0f;
    trainCar->velocity[2] = 0.0f;
}

/**
 * Set pathPoint spawn locations for each rolling stock
 * The railroad has 465 path
 */
void init_SL(void) {
    u16 pathPointOffset;
    TrainCarStuff* ptr1;
    Path2D* pos;
    s32 i;
    s32 j;

    for (i = 0; i < NUM_TRAINS; i++) {
        // outputs 160 or 392 depending on the train.
        // Wraps the value around to always output a valid pathPoint.
        pathPointOffset = (((i * g_CourseObstacle) / NUM_TRAINS) + 160) % g_CourseObstacle;

        // 120.0f is about the maximum usable value
        SL[i].speed = 5.0f;
        for (j = 0; j < NUM_PASSENGER_CAR_ENTRIES; j++) {
            pathPointOffset += 4;
            ptr1 = &SL[i].passengerCars[j];
            pos = &center_line_EX[pathPointOffset];
            clear_SL_tab(ptr1, pos, pathPointOffset);
        }
        // Smaller offset for the tender
        pathPointOffset += 3;
        ptr1 = &SL[i].tender;
        pos = &center_line_EX[pathPointOffset];
        clear_SL_tab(ptr1, pos, pathPointOffset);

        pathPointOffset += 4;
        ptr1 = &SL[i].locomotive;
        pos = &center_line_EX[pathPointOffset];
        clear_SL_tab(ptr1, pos, pathPointOffset);

        // Only use locomotive unless overwritten below.
        SL[i].numCars = LOCOMOTIVE_ONLY;
    }

    // Spawn all rolling stock in single player mode.
    switch (g_ScreenSplitB) {
        case SCREEN_MODE_1P: // single player
            for (i = 0; i < NUM_TRAINS; i++) {
                SL[i].tender.isActive = 1;

                // clang-format off
                // Same line required for matching...
                for (j = 0; j < NUM_PASSENGER_CAR_ENTRIES; j++) { SL[i].passengerCars[j].isActive = 1; }
                // clang-format on

                SL[i].numCars = NUM_TENDERS + NUM_PASSENGER_CAR_ENTRIES;
            }
            break;

        // Spawn locomotive, tender, and one passenger car in versus 2/3 player mode.
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL: // multiplayer fall-through
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (g_gameMode != GRAND_PRIX) {
                for (i = 0; i < NUM_TRAINS; i++) {
                    SL[i].tender.isActive = 1;
                    SL[i].passengerCars[4].isActive = 1;
                    SL[i].numCars = NUM_TENDERS + NUM_2P_PASSENGER_CARS;
                }
            }
            break;
    }

    gTrainSmokeTimer = 0;
}
/**
 * @brief sync the train components vehicle with the actor
 *
 * @param trainCar
 * @param orientationY
 */
void put_object_buf_SL(TrainCarStuff* trainCar, s16 orientationY) {
    struct TrainCar* trainCarActor;

    trainCarActor = (struct TrainCar*) &g_SimpleObjectArray[trainCar->actorIndex];
    trainCarActor->position[0] = trainCar->position[0];
    trainCarActor->position[1] = trainCar->position[1];
    trainCarActor->position[2] = trainCar->position[2];
    if (g_ScreenFlip != 0) {
        trainCarActor->angle[1] = -orientationY;
    } else {
        trainCarActor->angle[1] = orientationY;
    }
    trainCarActor->velocity[0] = trainCar->velocity[0];
    trainCarActor->velocity[2] = trainCar->velocity[2];
}

void move_SL(void) {
    UNUSED s32 pad[3];
    f32 temp_f20;
    TrainCarStuff* car;
    u16 oldPathPointIndex;
    s16 orientationYUpdate;
    f32 temp_f22;
    s32 i;
    s32 j;
    Vec3f smokePos;

    gTrainSmokeTimer += 1;

    for (i = 0; i < NUM_TRAINS; i++) {
        oldPathPointIndex = (u16) SL[i].locomotive.pathPointIndex;

        temp_f20 = SL[i].locomotive.position[0];
        temp_f22 = SL[i].locomotive.position[2];

        orientationYUpdate = hanyou_soukou_sub_EX(
            SL[i].locomotive.position, (s16*) &SL[i].locomotive.pathPointIndex, SL[i].speed);

        SL[i].locomotive.velocity[0] = SL[i].locomotive.position[0] - temp_f20;
        SL[i].locomotive.velocity[2] = SL[i].locomotive.position[2] - temp_f22;

        put_object_buf_SL(&SL[i].locomotive, orientationYUpdate);

        if ((oldPathPointIndex != SL[i].locomotive.pathPointIndex) &&
            ((SL[i].locomotive.pathPointIndex == 0x00BE) ||
             (SL[i].locomotive.pathPointIndex == 0x0140))) { // play crossing bell sound
            NAEnmTrgStart(SL[i].locomotive.position, SL[i].locomotive.velocity,
                          SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0E));
        } else if (MakeRandomLimmit(100) == 0) { // play train whistle sound
            NAEnmTrgStart(SL[i].locomotive.position, SL[i].locomotive.velocity,
                          SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0D));
        }

        SL[i].someFlags = check_player_near(
            SL[i].locomotive.position, TRAIN_SMOKE_RENDER_DISTANCE, SL[i].someFlags);
        // Renders locomotive smoke on all screens if any player is within range.
        if ((((s16) gTrainSmokeTimer % 5) == 0) && (SL[i].someFlags != 0)) {
            smokePos[0] = SL[i].locomotive.position[0];
            smokePos[1] = (f32) ((f64) SL[i].locomotive.position[1] + 65.0);
            smokePos[2] = (f32) ((f64) SL[i].locomotive.position[2] + 25.0);
            oga_guruguru_sub(smokePos, SL[i].locomotive.position, orientationYUpdate);
            kwget_train_smoke(i, smokePos, 1.1f);
        }

        car = &SL[i].tender;

        if (car->isActive == 1) {
            temp_f20 = car->position[0];
            temp_f22 = car->position[2];
            orientationYUpdate =
                hanyou_soukou_sub_EX(car->position, (s16*) &car->pathPointIndex, SL[i].speed);
            car->velocity[0] = car->position[0] - temp_f20;
            car->velocity[2] = car->position[2] - temp_f22;
            put_object_buf_SL(car, orientationYUpdate);
        }

        for (j = 0; j < NUM_PASSENGER_CAR_ENTRIES; j++) {
            car = &SL[i].passengerCars[j];
            if (car->isActive == 1) {
                temp_f20 = car->position[0];
                temp_f22 = car->position[2];

                orientationYUpdate =
                    hanyou_soukou_sub_EX(car->position, (s16*) &car->pathPointIndex, SL[i].speed);
                car->velocity[0] = car->position[0] - temp_f20;
                car->velocity[2] = car->position[2] - temp_f22;
                put_object_buf_SL(car, orientationYUpdate);
            }
        }
    }
}

void hit_check_SL(s32 playerId, Player* player) {
    TrainCarStuff* trainCar;
    f32 playerPosX;
    f32 playerPosZ;
    f32 x_dist;
    f32 z_dist;
    s32 trainIndex;
    s32 passengerCarIndex;

    if (tenuki_flg[playerId] != true) {
        if (!(player->slip_flag & EXPLODE)) {
            playerPosX = player->position[0];
            playerPosZ = player->position[2];
            for (trainIndex = 0; trainIndex < NUM_TRAINS; trainIndex++) {
                trainCar = &SL[trainIndex].locomotive;
                x_dist = playerPosX - trainCar->position[0];
                z_dist = playerPosZ - trainCar->position[2];
                if ((x_dist > -100.0) && (x_dist < 100.0)) {
                    if ((z_dist > -100.0) && (z_dist < 100.0)) {
                        if (oga_hit_check(trainCar->position[0], trainCar->position[2], trainCar->velocity[0],
                                                    trainCar->velocity[2], 60.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                            player->weapon |= HIT_BOMB;
                        }
                        trainCar = &SL[trainIndex].tender;
                        if (trainCar->isActive == 1) {
                            if (oga_hit_check(trainCar->position[0], trainCar->position[2],
                                                        trainCar->velocity[0], trainCar->velocity[2], 30.0f, 20.0f,
                                                        playerPosX, playerPosZ) == 1) {
                                player->weapon |= HIT_BOMB;
                            }
                        }
                    }
                }

                for (passengerCarIndex = 0; passengerCarIndex < NUM_PASSENGER_CAR_ENTRIES; passengerCarIndex++) {
                    trainCar = &SL[trainIndex].passengerCars[passengerCarIndex];
                    x_dist = playerPosX - trainCar->position[0];
                    z_dist = playerPosZ - trainCar->position[2];
                    if (trainCar->isActive == 1) {
                        if ((x_dist > -100.0) && (x_dist < 100.0)) {
                            if ((z_dist > -100.0) && (z_dist < 100.0)) {
                                if (oga_hit_check(trainCar->position[0], trainCar->position[2],
                                                            trainCar->velocity[0], trainCar->velocity[2], 30.0f, 20.0f,
                                                            playerPosX, playerPosZ) == 1) {
                                    player->weapon |= HIT_BOMB;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
 * Appears to check if the train is close to the crossing.
 * Implements fumikiri_counter as a counter
 */
void fumikiri_move(void) {
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f12;
    s32 i;
    fumikiri_mode[0] = 0;
    fumikiri_mode[1] = 0;

    for (i = 0; i < NUM_TRAINS; i++) {
        temp_f16 = SL[i].locomotive.pathPointIndex / ((f32) g_CourseObstacle);
        temp_f18 = 0.72017354f;
        temp_f12 = 0.42299348f;

        if (((temp_f12 - 0.1) < temp_f16) &&
            (temp_f16 < ((((f64) SL[i].numCars) * 0.01) + (temp_f12 + 0.01)))) {

            fumikiri_mode[0] = 1;
        }
        if (((temp_f18 - 0.1) < temp_f16) &&
            (temp_f16 < ((((f64) SL[i].numCars) * 0.01) + (temp_f18 + 0.01)))) {

            fumikiri_mode[1] = 1;
        }
    }

    for (i = 0; i < NUM_CROSSINGS; i++) {
        if (fumikiri_mode[i] == 1) {
            fumikiri_counter[i] += 1;
        } else {
            fumikiri_counter[i] = 0;
        }
    }
}

void fumikiri_wait_check(s32 playerId) {
    fumikiri_stop_flg[playerId] = 0;
    if (g_courseID == COURSE_KALAMARI_DESERT) {
        if ((!(tenuki_flg[playerId] != false)) ||
            (check_player_near(gPlayers[playerId].position, TRAIN_CROSSING_AI_DISTANCE, 0))) {

            if ((fumikiri_mode[1] == 1) && ((fumikiri_counter[1]) > FRAMES_SINCE_CROSSING_ACTIVATED)) {

                if ((OGAArea > 176) && (OGAArea < 182)) {
                    fumikiri_stop_flg[playerId] = 1;
                }
            }
            if ((fumikiri_mode[0] == 1) && ((fumikiri_counter[0]) > FRAMES_SINCE_CROSSING_ACTIVATED)) {
                if ((OGAArea >= 306) && (OGAArea < 310)) {
                    fumikiri_stop_flg[playerId] = 1;
                }
            }
        }
    }
}

void init_SHIP(void) {
    PaddleBoatStuff* paddleBoat;
    s32 i;
    Path2D* temp_a2;
    u16 temp;
    for (i = 0; i < NUM_ACTIVE_PADDLE_BOATS; i++) {
        temp = i * 0xB4;
        paddleBoat = &ship[i];
        temp_a2 = &center_line_EX[temp];
        paddleBoat->position[0] = temp_a2->x;
        paddleBoat->position[1] = D_80162EB2;
        paddleBoat->position[2] = temp_a2->z;
        paddleBoat->pathPointIndex = i * 0xB4;
        paddleBoat->actorIndex = -1;

        if (g_menuMultiplayerSelection >= 3) {
            paddleBoat->isActive = 0;
        } else {
            paddleBoat->isActive = 1;
        }
        paddleBoat->velocity[0] = 0.0f;
        paddleBoat->velocity[1] = 0.0f;
        paddleBoat->velocity[2] = 0.0f;
        paddleBoat->speed = 1.6666666f;
        paddleBoat->rotY = 0;
    }
    gFerrySmokeTimer = 0;
}

void move_SHIP(void) {
    PaddleBoatStuff* paddleBoat;
    Path2D* pathPoint;
    s32 i;
    Object* paddleBoatActor;
    f32 temp_f26;
    f32 temp_f28;
    f32 temp_f30;
    s16 temp_a1;
    s32 temp;
    s16 var_v1;
    Vec3f sp94;
    Vec3f sp88;
    UNUSED s32 pad;
    Vec3f smokePos;
    UNUSED s32 pad2;
    gFerrySmokeTimer += 1;
    for (i = 0; i < NUM_ACTIVE_PADDLE_BOATS; i++) {
        paddleBoat = &ship[i];
        if (paddleBoat->isActive == 1) {
            temp_f26 = paddleBoat->position[0];
            temp_f28 = paddleBoat->position[1];
            temp_f30 = paddleBoat->position[2];
            hanyou_soukou_sub_EX(paddleBoat->position, (s16*) &paddleBoat->pathPointIndex, paddleBoat->speed);
            paddleBoat->someFlags = check_player_near(paddleBoat->position, BOAT_SMOKE_RENDER_DISTANCE,
                                                                      paddleBoat->someFlags);
            if ((((s16) gFerrySmokeTimer % 10) == 0) && (paddleBoat->someFlags != 0)) {
                smokePos[0] = (f32) ((f64) paddleBoat->position[0] - 30.0);
                smokePos[1] = (f32) ((f64) paddleBoat->position[1] + 180.0);
                smokePos[2] = (f32) ((f64) paddleBoat->position[2] + 45.0);
                oga_guruguru_sub(smokePos, paddleBoat->position, paddleBoat->rotY);
                kwget_ship_smoke(i, smokePos, 1.1f);
                smokePos[0] = (f32) ((f64) paddleBoat->position[0] + 30.0);
                smokePos[1] = (f32) ((f64) paddleBoat->position[1] + 180.0);
                smokePos[2] = (f32) ((f64) paddleBoat->position[2] + 45.0);
                oga_guruguru_sub(smokePos, paddleBoat->position, paddleBoat->rotY);
                kwget_ship_smoke(i, smokePos, 1.1f);
            }
            if (MakeRandomLimmit(100) == 0) {
                if (MakeRandomLimmit(2) == 0) {
                    NAEnmTrgStart(paddleBoat->position, paddleBoat->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x47));
                } else {
                    NAEnmTrgStart(paddleBoat->position, paddleBoat->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x48));
                }
            }
            sp94[0] = temp_f26;
            sp94[1] = temp_f28;
            sp94[2] = temp_f30;
            pathPoint = &center_line_EX[(paddleBoat->pathPointIndex + 5) % g_CourseObstacle];
            sp88[0] = (f32) pathPoint->x;
            sp88[1] = (f32) D_80162EB0;
            sp88[2] = (f32) pathPoint->z;
            temp_a1 = oga_calc_direction(sp94, sp88);
            temp = temp_a1 - paddleBoat->rotY;
            var_v1 = temp;
            if (var_v1 < 0) {
                var_v1 = -var_v1;
            }
            if (var_v1 >= 0x1771) {
                if (paddleBoat->speed > 0.2) {
                    paddleBoat->speed -= 0.04;
                }
                if (var_v1 >= 0x3D) {
                    var_v1 = 0x003C;
                }
            } else {
                if (paddleBoat->speed < 2.0) {
                    paddleBoat->speed += 0.02;
                }
                if (var_v1 >= 0x1F) {
                    var_v1 = 0x001E;
                }
            }
            if (temp >= 0x8000) {
                paddleBoat->rotY -= var_v1;
            } else if (temp > 0) {
                paddleBoat->rotY += var_v1;
            } else if (temp < -0x7FFF) {
                paddleBoat->rotY += var_v1;
            } else if (temp < 0) {
                paddleBoat->rotY -= var_v1;
            }
            paddleBoat->velocity[0] = paddleBoat->position[0] - temp_f26;
            paddleBoat->velocity[1] = paddleBoat->position[1] - temp_f28;
            paddleBoat->velocity[2] = paddleBoat->position[2] - temp_f30;
            paddleBoatActor = &g_SimpleObjectArray[paddleBoat->actorIndex];
            paddleBoatActor->position[0] = paddleBoat->position[0];
            paddleBoatActor->position[1] = paddleBoat->position[1];
            paddleBoatActor->position[2] = paddleBoat->position[2];
            if (g_ScreenFlip != 0) {
                paddleBoatActor->angle[1] = -paddleBoat->rotY;
            } else {
                paddleBoatActor->angle[1] = paddleBoat->rotY;
            }
            paddleBoatActor->velocity[0] = paddleBoat->velocity[0];
            paddleBoatActor->velocity[1] = paddleBoat->velocity[1];
            paddleBoatActor->velocity[2] = paddleBoat->velocity[2];
        }
    }
}

void hit_check_SHIP(Player* player) {
    s32 someIndex;
    PaddleBoatStuff* tempPaddleWheelBoat;
    f32 x_diff;
    f32 y_diff;
    f32 z_diff;
    f32 playerX;
    f32 playerZ;
    f32 playerY;

    if (!((player->slip_flag & EXPLODE)) && (!(player->slip_flag & THROW_EXPLODE))) {
        playerX = player->position[0];
        playerY = player->position[1];
        playerZ = player->position[2];
        for (someIndex = 0; someIndex < NUM_ACTIVE_PADDLE_BOATS; someIndex++) {
            tempPaddleWheelBoat = &ship[someIndex];
            if (tempPaddleWheelBoat->isActive == 1) {
                x_diff = playerX - tempPaddleWheelBoat->position[0];
                y_diff = playerY - tempPaddleWheelBoat->position[1];
                z_diff = playerZ - tempPaddleWheelBoat->position[2];
                if ((x_diff > -300.0) && (x_diff < 300.0)) {
                    if ((z_diff > -300.0) && (z_diff < 300.0)) {
                        if ((oga_hit_check(tempPaddleWheelBoat->position[0], tempPaddleWheelBoat->position[2],
                                                     tempPaddleWheelBoat->velocity[0], tempPaddleWheelBoat->velocity[2],
                                                     200.0f, 60.0f, playerX, playerZ) == 1) &&
                            (y_diff < 60.0)) {
                            player->weapon |= HIT_BOAT;
                        }
                    }
                }
            }
        }
    }
}

void init_HIGHWAY_CAR_sub(f32 speedA, f32 speedB, s32 numVehicles, s32 arg3, VehicleStuff* vehicleList,
                                       CenterPathStruct* pathPointList) {
    VehicleStuff* veh;
    CenterPathStruct* temp_v0;
    s32 i;
    u16 pathPointOffset;
    s32 numPathPoints = g_courseTotalPathPoints[0];
    for (i = 0; i < numVehicles; i++) {
        pathPointOffset = (((i * numPathPoints) / numVehicles) + arg3) % numPathPoints;
        veh = &vehicleList[i];
        temp_v0 = &pathPointList[pathPointOffset];
        veh->position[0] = (f32) temp_v0->pointx;
        veh->position[1] = (f32) temp_v0->pointy;
        veh->position[2] = (f32) temp_v0->pointz;
        veh->actorIndex = -1;
        veh->pathPointIndex = pathPointOffset;
        veh->unused = 0;
        veh->velocity[0] = 0.0f;
        veh->velocity[1] = 0.0f;
        veh->velocity[2] = 0.0f;
        veh->someFlags = 0;
        veh->someFlagsTheSequel = 0;
        if (g_gameMode == TIME_TRIALS) {
            veh->someType = (i % 3);
        } else {
            veh->someType = MakeRandomLimmit(3);
        }
        veh->someMultiplierTheSequel = (f32) ((f64) (f32) (veh->someType - 1) * 0.6);
        if (((g_raceClass > CC_50) || (g_gameMode == TIME_TRIALS)) && (veh->someType == 2)) {
            veh->speed = speedA;
        } else {
            veh->speed = speedB;
        }
        veh->rotation[0] = 0;
        veh->rotation[2] = 0;
        if (highway_gyakusou_flg == false) {
            veh->rotation[1] = hanyou_soukou_sub_BP(veh->position, (s16*) &veh->pathPointIndex, veh->speed,
                                             veh->someMultiplierTheSequel, 0, 3);
        } else {
            veh->rotation[1] =
                hanyou_gyaku_soukou_sub_BP(veh->position, (s16*) &veh->pathPointIndex, veh->speed, veh->someMultiplierTheSequel, 0);
        }
    }
    sVehicleSoundRenderCounter = 10;
}

f32 shasen_chk_sub(s16 someType, s16 pathPointIndex) {
    f32 var_f2;

    var_f2 = 0.0f;
    if (pathPointIndex < 0x28A) {
        switch (someType) {
            case 0:
                var_f2 = -0.7f;
                break;
            case 1:
                break;
            case 2:
                var_f2 = 0.7f;
                break;
            default:
                break;
        }
    } else {
        switch (someType) {
            case 0:
            case 1:
                var_f2 = -0.5f;
                break;
            case 2:
                var_f2 = 0.5f;
                break;
            default:
                break;
        }
    }
    return var_f2;
}

void move_HIGHWAY_CAR_sub(VehicleStuff* vehicle) {
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 temp_f2_2;
    s16 var_a1;
    s16 thing;
    Vec3f sp40;
    Vec3f sp34;
    Object* vehicleActor;

    sp5C = vehicle->position[0];
    sp58 = vehicle->position[1];
    sp54 = vehicle->position[2];
    sp40[0] = sp58;
    sp40[1] = 0.0f;
    sp40[2] = 0.0f;
    temp_f0_2 = shasen_chk_sub(vehicle->someType, vehicle->pathPointIndex);
    if (vehicle->someMultiplierTheSequel < temp_f0_2) {
        vehicle->someMultiplierTheSequel = vehicle->someMultiplierTheSequel + 0.06;
        if (temp_f0_2 < vehicle->someMultiplierTheSequel) {
            vehicle->someMultiplierTheSequel = temp_f0_2;
        }
    }
    if (temp_f0_2 < vehicle->someMultiplierTheSequel) {
        vehicle->someMultiplierTheSequel = vehicle->someMultiplierTheSequel - 0.06;
        if (vehicle->someMultiplierTheSequel < temp_f0_2) {
            vehicle->someMultiplierTheSequel = temp_f0_2;
        }
    }
    if (highway_gyakusou_flg == false) {
        var_a1 = hanyou_soukou_sub_BP(vehicle->position, (s16*) &vehicle->pathPointIndex, vehicle->speed,
                               vehicle->someMultiplierTheSequel, 0, 3);
    } else {
        var_a1 = hanyou_gyaku_soukou_sub_BP(vehicle->position, (s16*) &vehicle->pathPointIndex, vehicle->speed,
                               vehicle->someMultiplierTheSequel, 0);
    }
    ChaseDir(&vehicle->rotation[1], var_a1, 100);
    temp_f0_3 = vehicle->position[0] - sp5C;
    temp_f2_2 = vehicle->position[2] - sp54;
    sp34[0] = vehicle->position[1];
    sp34[1] = 0.0f;
    sp34[2] = sqrtf((temp_f0_3 * temp_f0_3) + (temp_f2_2 * temp_f2_2));
    thing = CalcDirection(sp40, sp34);
    ChaseDir(&vehicle->rotation[0], -thing, 100);
    vehicle->velocity[0] = vehicle->position[0] - sp5C;
    vehicle->velocity[1] = vehicle->position[1] - sp58;
    vehicle->velocity[2] = vehicle->position[2] - sp54;
    vehicleActor = &g_SimpleObjectArray[vehicle->actorIndex];
    vehicleActor->position[0] = vehicle->position[0];
    vehicleActor->position[1] = vehicle->position[1];
    vehicleActor->position[2] = vehicle->position[2];
    vehicleActor->angle[0] = vehicle->rotation[0];
    if (g_ScreenFlip != 0) {
        vehicleActor->angle[1] = -vehicle->rotation[1];
    } else {
        vehicleActor->angle[1] = vehicle->rotation[1];
    }
    vehicleActor->angle[2] = vehicle->rotation[2];
    vehicleActor->velocity[0] = vehicle->velocity[0];
    vehicleActor->velocity[1] = vehicle->velocity[1];
    vehicleActor->velocity[2] = vehicle->velocity[2];
}

void hit_check_HIGHWAY_CAR_sub(s32 playerId, Player* player, VehicleStuff* vehicle, f32 distanceX, f32 distanceY,
                                 s32 vehicleCount, u32 soundBits) {
    f32 deltaX;
    f32 deltaZ;
    f32 deltaY;

    s32 i;

    f32 playerX;
    f32 playerY;
    f32 playerZ;

    if (((tenuki_flg[playerId] != true) || ((((player->flag & IS_PLAYER) != 0)) && !(player->flag & IS_CPU_PLAYER))) &&
        !(player->slip_flag & EXPLODE)) {

        playerX = player->position[0];
        playerY = player->position[1];
        playerZ = player->position[2];

        for (i = 0; i < vehicleCount; i++) {
            deltaX = playerX - vehicle->position[0];
            deltaY = playerY - vehicle->position[1];
            deltaZ = playerZ - vehicle->position[2];

            if (((deltaX) > -100.0) && ((deltaX) < 100.0)) {
                if ((deltaY > -20.0) && (deltaY < 20.0)) {
                    if (((deltaZ) > -100.0) && ((deltaZ) < 100.0)) {
                        if (oga_hit_check(vehicle->position[0], vehicle->position[2], vehicle->velocity[0],
                                                    vehicle->velocity[2], distanceX, distanceY, playerX,
                                                    playerZ) == (s32) 1) {
                            player->weapon |= HIT_BOMB;
                        }
                    }
                }
            }

            // Human player specific interactions
            if ((player->flag & IS_PLAYER) && !(player->flag & IS_CPU_PLAYER)) {
                if (((deltaX) > -300.0) && ((deltaX) < 300.0) && ((deltaY > -20.0)) && (deltaY < 20.0) &&
                    (((deltaZ) > -300.0)) && ((deltaZ) < 300.0)) {
                    if ((sVehicleSoundRenderCounter > 0) && (vehicle->someFlags == 0)) {
                        sVehicleSoundRenderCounter--;
                        vehicle->someFlags |= (RENDER_VEHICLE << playerId);
                        NaSceneLevelStart(vehicle->position, vehicle->velocity, soundBits);
                    }
                } else {
                    if (vehicle->someFlags != 0) {
                        vehicle->someFlags &= ~(RENDER_VEHICLE << playerId);
                        if (vehicle->someFlags == 0) {
                            sVehicleSoundRenderCounter++;
                            NaSceneLevelStop(vehicle->position, soundBits);
                        }
                    }
                }

                if (((deltaX) > -200.0) && ((deltaX) < 200.0) && ((deltaY > -20.0)) && (deltaY < 20.0) &&
                    (((deltaZ) > -200.0)) && ((deltaZ) < 200.0)) {
                    if (!(vehicle->someFlagsTheSequel & ((1 << playerId)))) {

                        bool shouldInteract = false;
                        u16 path = g_courseTotalPathPoints[0];
                        s32 t1;
                        s32 t2;

                        switch (highway_gyakusou_flg) {
                            case false:
                                t1 = exist_area_chk(vehicle->pathPointIndex,
                                                            gNearestPathPointByPlayerId[playerId], 10, 0, path);
                                if ((g_wrongwayFlagPlayer1[playerId] == 0) && (t1 > 0) &&
                                    (player->speed < vehicle->speed)) {
                                    shouldInteract = true;
                                }
                                if ((g_wrongwayFlagPlayer1[playerId] == 1) && (t1 > 0)) {
                                    shouldInteract = true;
                                }
                                break;
                            case true:
                                t2 = exist_area_chk(vehicle->pathPointIndex,
                                                            gNearestPathPointByPlayerId[playerId], 0, 10, path);
                                if (t2 > 0) {
                                    if (MakeRandomLimmit(2) == 0) {
                                        // temp_v1_2 = g_wrongwayFlagPlayer1[playerId];
                                        if (g_wrongwayFlagPlayer1[playerId] == 0) {
                                            shouldInteract = true;
                                        }
                                        if ((g_wrongwayFlagPlayer1[playerId] == 1) &&
                                            (player->speed < vehicle->speed)) {
                                            shouldInteract = true;
                                        }
                                    } else {
                                        vehicle->someFlagsTheSequel |= ((1 << playerId));
                                    }
                                }
                                break;
                        }
                        if (shouldInteract == true) {

                            u32 soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3B);

                            switch (soundBits) {
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x05):
                                    soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3B);
                                    if (MakeRandomLimmit(4) == 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3C);
                                    }
                                    break;
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x02):
                                    if (MakeRandomLimmit(2) != 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3D);
                                    } else {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3E);
                                    }
                                    break;
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x03):
                                    if (MakeRandomLimmit(2) != 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3F);
                                    } else {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x40);
                                    }
                                    break;
                                case SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x04):
                                    if (MakeRandomLimmit(2) != 0) {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x41);
                                    } else {
                                        soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x42);
                                    }
                                    break;
                            }
                            vehicle->someFlagsTheSequel |= ((1 << playerId));
                            NAEnmTrgStart(vehicle->position, vehicle->velocity, soundBits2);
                        }
                    }
                } else {
                    if (vehicle->someFlagsTheSequel & ((1 << playerId))) {
                        vehicle->someFlagsTheSequel &= ~((1 << playerId));
                    }
                }
            }
            vehicle++;
        }
    }
}

f32 avoid_HIGHWAY_sub(s16 someType, f32 arg1, s16 pathIndex) {
    if (pathIndex < 0x28A) {
        switch (someType) {
            case 0:
                if (arg1 < 0.0) {
                    arg1 = 0.0f;
                }
                break;
            case 1:
                if (arg1 < 0.0) {
                    arg1 = -0.8f;
                } else {
                    arg1 = 0.8f;
                }
                break;
            case 2:
                if (arg1 >= 0.0) {
                    arg1 = 0.0f;
                }
                break;
            default:
                break;
        }
    } else {
        switch (someType) {
            case 0:
            case 1:
                arg1 = 0.5f;
                break;
            case 2:
                arg1 = -0.5f;
                break;
            default:
                break;
        }
    }
    return arg1;
}

void avoid_HIGHWAY_CAR_sub(s32 playerId, s32 vehicleCount, VehicleStuff* vehicle) {
    UNUSED s32 var_v1;
    s32 pathPointOffset;
    s32 var_s2;
    s32 pathPointCount;
    u16 vehiclePathPoint;
    UNUSED VehicleStuff* tempVehicle;

    pathPointCount = g_courseTotalPathPoints[0];
    if (!(gPlayers[playerId].speed < 1.6666666666666667)) {
        for (var_s2 = 0; var_s2 < vehicleCount; var_s2++, vehicle++) {
            vehiclePathPoint = vehicle->pathPointIndex;
            for (pathPointOffset = 0; pathPointOffset < 0x18; pathPointOffset += 3) {
                if (((OGAArea + pathPointOffset) % pathPointCount) == vehiclePathPoint) {
                    LaneData[playerId].lane_target = avoid_HIGHWAY_sub(
                        vehicle->someType, zure_data[playerId], vehiclePathPoint);
                    return;
                }
            }
        }
    }
}

void init_TRUCK(void) {
    f32 a = ((g_raceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((g_raceClass * 90.0) / 216.0f) + 2.9166666666666665;
    s32 numTrucks = NUM_RACE_BOX_TRUCKS;
    if (g_gameMode == TIME_TRIALS) {
        numTrucks = NUM_TIME_TRIAL_BOX_TRUCKS;
    }
    init_HIGHWAY_CAR_sub(a, b, numTrucks, 0, TRUCK, &CenterPathBP[0][0]);
}

void move_TRUCK(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_BOX_TRUCKS; loopIndex++) {
        move_HIGHWAY_CAR_sub(&TRUCK[loopIndex]);
    }
}

void hit_check_TRUCK(s32 playerId, Player* player) {
    hit_check_HIGHWAY_CAR_sub(playerId, player, TRUCK, 55.0f, 12.5f, NUM_RACE_BOX_TRUCKS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x03));
}

void avoid_TRUCK(s32 playerId) {
    avoid_HIGHWAY_CAR_sub(playerId, NUM_RACE_BOX_TRUCKS, TRUCK);
}

void init_BUS(void) {
    s32 numBusses;
    f32 a = ((g_raceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((g_raceClass * 90.0) / 216.0f) + 2.9166666666666665;

    numBusses = NUM_RACE_SCHOOL_BUSES;
    if (g_gameMode == TIME_TRIALS) {
        numBusses = NUM_TIME_TRIAL_SCHOOL_BUSES;
    }
    init_HIGHWAY_CAR_sub(a, b, numBusses, 75, bus, &CenterPathBP[0][0]);
}

void move_BUS(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_SCHOOL_BUSES; loopIndex++) {
        move_HIGHWAY_CAR_sub(&bus[loopIndex]);
    }
}

void hit_check_BUS(s32 playerId, Player* player) {
    hit_check_HIGHWAY_CAR_sub(playerId, player, bus, 70.0f, 12.5f, NUM_RACE_SCHOOL_BUSES,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x02));
}

void avoid_BUS(s32 playerId) {
    avoid_HIGHWAY_CAR_sub(playerId, NUM_RACE_SCHOOL_BUSES, bus);
}

void init_TANK(void) {
    s32 numTrucks;
    f32 a = ((g_raceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((g_raceClass * 90.0) / 216.0f) + 2.9166666666666665;

    numTrucks = NUM_RACE_TANKER_TRUCKS;
    if (g_gameMode == TIME_TRIALS) {
        numTrucks = NUM_TIME_TRIAL_TANKER_TRUCKS;
    }
    init_HIGHWAY_CAR_sub(a, b, numTrucks, 50, tanklorry, &CenterPathBP[0][0]);
}

void move_TANK(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_TANKER_TRUCKS; loopIndex++) {
        move_HIGHWAY_CAR_sub(&tanklorry[loopIndex]);
    }
}

void hit_check_TANK(s32 playerId, Player* player) {
    hit_check_HIGHWAY_CAR_sub(playerId, player, tanklorry, 55.0f, 12.5f, NUM_RACE_TANKER_TRUCKS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x04));
}

void avoid_TANK(s32 playerId) {
    avoid_HIGHWAY_CAR_sub(playerId, NUM_RACE_TANKER_TRUCKS, tanklorry);
}

void init_RV(void) {
    s32 numCars;
    f32 a = ((g_raceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((g_raceClass * 90.0) / 216.0f) + 2.9166666666666665;

    numCars = NUM_RACE_CARS;
    if (g_gameMode == TIME_TRIALS) {
        numCars = NUM_TIME_TRIAL_CARS;
    }
    init_HIGHWAY_CAR_sub(a, b, numCars, 25, rv_car, &CenterPathBP[0][0]);
}

void move_RV(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_CARS; loopIndex++) {
        move_HIGHWAY_CAR_sub(&rv_car[loopIndex]);
    }
}

void hit_check_RV(s32 playerId, Player* player) {
    hit_check_HIGHWAY_CAR_sub(playerId, player, rv_car, 11.5f, 8.5f, NUM_RACE_CARS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x05));
}

void avoid_RV(s32 playerId) {
    avoid_HIGHWAY_CAR_sub(playerId, NUM_RACE_CARS, rv_car);
}
