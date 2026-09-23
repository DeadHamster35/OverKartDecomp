#define GET_PATH_LENGTH(pathPoint)               \
    for (i = 0;; i++) {                          \
        if ((u16) pathPoint[i].Pointx == 0x8000) { \
            break;                               \
        }                                        \
    }

void AreaBunkatuSL(void) {
    s32 i;
    Path2D* temp;
    CenterPathStruct* pathPoint =
        (CenterPathStruct*) VIRTUAL_TO_PHYSICAL2(SegmentTable[SEGMENT_NUMBER2(d_course_kalimari_desert_train_path)] +
                                               SEGMENT_OFFSET(d_course_kalimari_desert_train_path));

    GET_PATH_LENGTH(pathPoint)

    temp = CenterLineEX;
    CourseObstacle = AreaBunkatuSLSHIPSub(temp, pathPoint, i - 1);
    D_80162EB0 = CheckHight(temp[0].x, 2000.0f, temp[0].z);
}

void AreaBunkatuSHIP(void) {
    CenterPathStruct* pathPoint;
    s32 i;

    pathPoint =
        (CenterPathStruct*) VIRTUAL_TO_PHYSICAL2(SegmentTable[SEGMENT_NUMBER2(d_course_dks_jungle_parkway_ferry_path)] +
                                               (SEGMENT_OFFSET(d_course_dks_jungle_parkway_ferry_path)));

    GET_PATH_LENGTH(pathPoint)

    CourseObstacle = AreaBunkatuSLSHIPSub(CenterLineEX, pathPoint, i - 1);
    D_80162EB2 = -40;
}

void ObjInitHighwaySub(VehicleStuff* vehicle) {
    f32 origXPos;
    UNUSED f32 pad;
    f32 origZPos;

    origXPos = vehicle->Position[0];
    origZPos = vehicle->Position[2];
    if (HighwayGyakusouFlg == false) {
        HanyouSoukouSubBP(vehicle->Position, (s16*) &vehicle->PathPointIndex, vehicle->Speed,
                      vehicle->SomeMultiplierTheSequel, 0, 3);
        vehicle->Rotation[0] = 0;
        vehicle->Rotation[1] = -DEGREES(180);
        vehicle->Rotation[2] = 0;
    } else {
        HanyouGyakuSoukouSubBP(vehicle->Position, (s16*) &vehicle->PathPointIndex, vehicle->Speed,
                      vehicle->SomeMultiplierTheSequel, 0);
        vehicle->Rotation[0] = 0;
        vehicle->Rotation[1] = 0;
        vehicle->Rotation[2] = 0;
    }
    vehicle->Velocity[0] = vehicle->Position[0] - origXPos;
    vehicle->Velocity[2] = vehicle->Position[2] - origZPos;
}

void OgawaHanyouObjectInit(void) {
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
    switch (CourseID) {
        case COURSE_KALAMARI_DESERT:
            for (loopIndex = 0; loopIndex < NUM_TRAINS; loopIndex++) {
                tempLocomotive = &SL[loopIndex].Locomotive;
                origXPos = tempLocomotive->Position[0];
                origZPos = tempLocomotive->Position[2];
                trainCarYRot = HanyouSoukouSubEX(
                    tempLocomotive->Position, (s16*) &tempLocomotive->PathPointIndex, SL[loopIndex].Speed);
                tempLocomotive->Velocity[0] = tempLocomotive->Position[0] - origXPos;
                tempLocomotive->Velocity[2] = tempLocomotive->Position[2] - origZPos;
                SetSVector(trainCarRot, 0, trainCarYRot, 0);
                tempLocomotive->ActorIndex = AddObjectBuffer(tempLocomotive->Position, trainCarRot,
                                                                     tempLocomotive->Velocity, ACTOR_TRAIN_ENGINE);

                tempTender = &SL[loopIndex].Tender;
                if (tempTender->IsActive == 1) {
                    origXPos = tempTender->Position[0];
                    origZPos = tempTender->Position[2];
                    trainCarYRot = HanyouSoukouSubEX(
                        tempTender->Position, (s16*) &tempTender->PathPointIndex, SL[loopIndex].Speed);
                    tempTender->Velocity[0] = tempTender->Position[0] - origXPos;
                    tempTender->Velocity[2] = tempTender->Position[2] - origZPos;
                    SetSVector(trainCarRot, 0, trainCarYRot, 0);
                    tempTender->ActorIndex = AddObjectBuffer(tempTender->Position, trainCarRot,
                                                                     tempTender->Velocity, ACTOR_TRAIN_TENDER);
                }

                for (loopIndex2 = 0; loopIndex2 < NUM_PASSENGER_CAR_ENTRIES; loopIndex2++) {
                    tempPassengerCar = &SL[loopIndex].PassengerCars[loopIndex2];
                    if (tempPassengerCar->IsActive == 1) {
                        origXPos = tempPassengerCar->Position[0];
                        origZPos = tempPassengerCar->Position[2];
                        trainCarYRot = HanyouSoukouSubEX(tempPassengerCar->Position,
                                                                     (s16*) &tempPassengerCar->PathPointIndex,
                                                                     SL[loopIndex].Speed);
                        tempPassengerCar->Velocity[0] = tempPassengerCar->Position[0] - origXPos;
                        tempPassengerCar->Velocity[2] = tempPassengerCar->Position[2] - origZPos;
                        SetSVector(trainCarRot, 0, trainCarYRot, 0);
                        tempPassengerCar->ActorIndex =
                            AddObjectBuffer(tempPassengerCar->Position, trainCarRot, tempPassengerCar->Velocity,
                                                    ACTOR_TRAIN_PASSENGER_CAR);
                    }
                }
            }
            break;
        case COURSE_DK_JUNGLE:
            for (loopIndex = 0; loopIndex < NUM_ACTIVE_PADDLE_BOATS; loopIndex++) {
                tempPaddleWheelBoat = &Ship[loopIndex];
                if (tempPaddleWheelBoat->IsActive == 1) {
                    origXPos = tempPaddleWheelBoat->Position[0];
                    origZPos = tempPaddleWheelBoat->Position[2];
                    tempPaddleWheelBoat->RotY = HanyouSoukouSubEX(
                        tempPaddleWheelBoat->Position, (s16*) &tempPaddleWheelBoat->PathPointIndex,
                        tempPaddleWheelBoat->Speed);
                    tempPaddleWheelBoat->Velocity[0] = tempPaddleWheelBoat->Position[0] - origXPos;
                    tempPaddleWheelBoat->Velocity[2] = tempPaddleWheelBoat->Position[2] - origZPos;
                    SetSVector(paddleWheelBoatRot, 0, tempPaddleWheelBoat->RotY, 0);
                    tempPaddleWheelBoat->ActorIndex =
                        AddObjectBuffer(tempPaddleWheelBoat->Position, paddleWheelBoatRot,
                                                tempPaddleWheelBoat->Velocity, ACTOR_PADDLE_BOAT);
                }
            }
            break;
        case COURSE_TOADS_TURNPIKE:
            for (loopIndex = 0; loopIndex < NUM_RACE_BOX_TRUCKS; loopIndex++) {
                tempBoxTruck = &TRUCK[loopIndex];
                ObjInitHighwaySub(tempBoxTruck);
                tempBoxTruck->ActorIndex = AddObjectBuffer(tempBoxTruck->Position, tempBoxTruck->Rotation,
                                                                   tempBoxTruck->Velocity, ACTOR_BOX_TRUCK);
            }
            for (loopIndex = 0; loopIndex < NUM_RACE_SCHOOL_BUSES; loopIndex++) {
                tempSchoolBus = &Bus[loopIndex];
                ObjInitHighwaySub(tempSchoolBus);
                tempSchoolBus->ActorIndex = AddObjectBuffer(tempSchoolBus->Position, tempSchoolBus->Rotation,
                                                                    tempSchoolBus->Velocity, ACTOR_SCHOOL_BUS);
            }
            for (loopIndex = 0; loopIndex < NUM_RACE_TANKER_TRUCKS; loopIndex++) {
                tempTankerTruck = &Tanklorry[loopIndex];
                ObjInitHighwaySub(tempTankerTruck);
                tempTankerTruck->ActorIndex =
                    AddObjectBuffer(tempTankerTruck->Position, tempTankerTruck->Rotation,
                                            tempTankerTruck->Velocity, ACTOR_TANKER_TRUCK);
            }
            for (loopIndex = 0; loopIndex < NUM_RACE_CARS; loopIndex++) {
                tempCar = &RvCar[loopIndex];
                ObjInitHighwaySub(tempCar);
                tempCar->ActorIndex =
                    AddObjectBuffer(tempCar->Position, tempCar->Rotation, tempCar->Velocity, ACTOR_CAR);
            }
            break;
    }
#else

#endif
}

void ClearSLTab(TrainCarStuff* trainCar, Path2D* posXZ, u16 pathPoint) {
    trainCar->Position[0] = (f32) posXZ->x;
    trainCar->Position[1] = (f32) D_80162EB0;
    trainCar->Position[2] = (f32) posXZ->z;
    trainCar->ActorIndex = -1;
    trainCar->PathPointIndex = pathPoint;
    trainCar->IsActive = 0;
    trainCar->Velocity[0] = 0.0f;
    trainCar->Velocity[1] = 0.0f;
    trainCar->Velocity[2] = 0.0f;
}

/**
 * Set pathPoint spawn locations for each rolling stock
 * The railroad has 465 path
 */
void InitSL(void) {
    u16 pathPointOffset;
    TrainCarStuff* ptr1;
    Path2D* pos;
    s32 i;
    s32 j;

    for (i = 0; i < NUM_TRAINS; i++) {
        // outputs 160 or 392 depending on the train.
        // Wraps the Value around to always Output a valid pathPoint.
        pathPointOffset = (((i * CourseObstacle) / NUM_TRAINS) + 160) % CourseObstacle;

        // 120.0f is about the maximum usable Value
        SL[i].Speed = 5.0f;
        for (j = 0; j < NUM_PASSENGER_CAR_ENTRIES; j++) {
            pathPointOffset += 4;
            ptr1 = &SL[i].PassengerCars[j];
            pos = &CenterLineEX[pathPointOffset];
            ClearSLTab(ptr1, pos, pathPointOffset);
        }
        // Smaller Offset for the Tender
        pathPointOffset += 3;
        ptr1 = &SL[i].Tender;
        pos = &CenterLineEX[pathPointOffset];
        ClearSLTab(ptr1, pos, pathPointOffset);

        pathPointOffset += 4;
        ptr1 = &SL[i].Locomotive;
        pos = &CenterLineEX[pathPointOffset];
        ClearSLTab(ptr1, pos, pathPointOffset);

        // Only use Locomotive unless overwritten below.
        SL[i].NumCars = LOCOMOTIVE_ONLY;
    }

    // Spawn all rolling stock in single Player Mode.
    switch (ScreenSplitB) {
        case SCREEN_MODE_1P: // single Player
            for (i = 0; i < NUM_TRAINS; i++) {
                SL[i].Tender.IsActive = 1;

                // clang-format off
                // Same line required for matching...
                for (j = 0; j < NUM_PASSENGER_CAR_ENTRIES; j++) { SL[i].PassengerCars[j].IsActive = 1; }
                // clang-format on

                SL[i].NumCars = NUM_TENDERS + NUM_PASSENGER_CAR_ENTRIES;
            }
            break;

        // Spawn Locomotive, Tender, and one passenger car in versus 2/3 Player Mode.
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL: // multiplayer fall-through
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (GameMode != GRAND_PRIX) {
                for (i = 0; i < NUM_TRAINS; i++) {
                    SL[i].Tender.IsActive = 1;
                    SL[i].PassengerCars[4].IsActive = 1;
                    SL[i].NumCars = NUM_TENDERS + NUM_2P_PASSENGER_CARS;
                }
            }
            break;
    }

    TrainSmokeTimer = 0;
}
/**
 * @brief sync the train components vehicle with the actor
 *
 * @Param trainCar
 * @Param orientationY
 */
void PutObjectBufSL(TrainCarStuff* trainCar, s16 orientationY) {
    struct TrainCar* trainCarActor;

    trainCarActor = (struct TrainCar*) &SimpleObjectArray[trainCar->ActorIndex];
    trainCarActor->Position[0] = trainCar->Position[0];
    trainCarActor->Position[1] = trainCar->Position[1];
    trainCarActor->Position[2] = trainCar->Position[2];
    if (ScreenFlip != 0) {
        trainCarActor->Angle[1] = -orientationY;
    } else {
        trainCarActor->Angle[1] = orientationY;
    }
    trainCarActor->Velocity[0] = trainCar->Velocity[0];
    trainCarActor->Velocity[2] = trainCar->Velocity[2];
}

void MoveSL(void) {
    UNUSED s32 pad[3];
    f32 temp_f20;
    TrainCarStuff* car;
    u16 oldPathPointIndex;
    s16 orientationYUpdate;
    f32 temp_f22;
    s32 i;
    s32 j;
    Vec3f smokePos;

    TrainSmokeTimer += 1;

    for (i = 0; i < NUM_TRAINS; i++) {
        oldPathPointIndex = (u16) SL[i].Locomotive.PathPointIndex;

        temp_f20 = SL[i].Locomotive.Position[0];
        temp_f22 = SL[i].Locomotive.Position[2];

        orientationYUpdate = HanyouSoukouSubEX(
            SL[i].Locomotive.Position, (s16*) &SL[i].Locomotive.PathPointIndex, SL[i].Speed);

        SL[i].Locomotive.Velocity[0] = SL[i].Locomotive.Position[0] - temp_f20;
        SL[i].Locomotive.Velocity[2] = SL[i].Locomotive.Position[2] - temp_f22;

        PutObjectBufSL(&SL[i].Locomotive, orientationYUpdate);

        if ((oldPathPointIndex != SL[i].Locomotive.PathPointIndex) &&
            ((SL[i].Locomotive.PathPointIndex == 0x00BE) ||
             (SL[i].Locomotive.PathPointIndex == 0x0140))) { // play crossing bell Sound
            NAEnmTrgStart(SL[i].Locomotive.Position, SL[i].Locomotive.Velocity,
                          SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0E));
        } else if (MakeRandomLimmit(100) == 0) { // play train whistle Sound
            NAEnmTrgStart(SL[i].Locomotive.Position, SL[i].Locomotive.Velocity,
                          SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x0D));
        }

        SL[i].SomeFlags = CheckPlayerNear(
            SL[i].Locomotive.Position, TRAIN_SMOKE_RENDER_DISTANCE, SL[i].SomeFlags);
        // Renders Locomotive smoke on all screens if any Player is within range.
        if ((((s16) TrainSmokeTimer % 5) == 0) && (SL[i].SomeFlags != 0)) {
            smokePos[0] = SL[i].Locomotive.Position[0];
            smokePos[1] = (f32) ((f64) SL[i].Locomotive.Position[1] + 65.0);
            smokePos[2] = (f32) ((f64) SL[i].Locomotive.Position[2] + 25.0);
            OgaGuruguruSub(smokePos, SL[i].Locomotive.Position, orientationYUpdate);
            KwgetTrainSmoke(i, smokePos, 1.1f);
        }

        car = &SL[i].Tender;

        if (car->IsActive == 1) {
            temp_f20 = car->Position[0];
            temp_f22 = car->Position[2];
            orientationYUpdate =
                HanyouSoukouSubEX(car->Position, (s16*) &car->PathPointIndex, SL[i].Speed);
            car->Velocity[0] = car->Position[0] - temp_f20;
            car->Velocity[2] = car->Position[2] - temp_f22;
            PutObjectBufSL(car, orientationYUpdate);
        }

        for (j = 0; j < NUM_PASSENGER_CAR_ENTRIES; j++) {
            car = &SL[i].PassengerCars[j];
            if (car->IsActive == 1) {
                temp_f20 = car->Position[0];
                temp_f22 = car->Position[2];

                orientationYUpdate =
                    HanyouSoukouSubEX(car->Position, (s16*) &car->PathPointIndex, SL[i].Speed);
                car->Velocity[0] = car->Position[0] - temp_f20;
                car->Velocity[2] = car->Position[2] - temp_f22;
                PutObjectBufSL(car, orientationYUpdate);
            }
        }
    }
}

void HitCheckSL(s32 PlayerId, Player* Player) {
    TrainCarStuff* trainCar;
    f32 playerPosX;
    f32 playerPosZ;
    f32 x_dist;
    f32 z_dist;
    s32 trainIndex;
    s32 passengerCarIndex;

    if (TenukiFlg[PlayerId] != true) {
        if (!(Player->SlipFlag & EXPLODE)) {
            playerPosX = Player->Position[0];
            playerPosZ = Player->Position[2];
            for (trainIndex = 0; trainIndex < NUM_TRAINS; trainIndex++) {
                trainCar = &SL[trainIndex].Locomotive;
                x_dist = playerPosX - trainCar->Position[0];
                z_dist = playerPosZ - trainCar->Position[2];
                if ((x_dist > -100.0) && (x_dist < 100.0)) {
                    if ((z_dist > -100.0) && (z_dist < 100.0)) {
                        if (OgaHitCheck(trainCar->Position[0], trainCar->Position[2], trainCar->Velocity[0],
                                                    trainCar->Velocity[2], 60.0f, 20.0f, playerPosX, playerPosZ) == 1) {
                            Player->Weapon |= HIT_BOMB;
                        }
                        trainCar = &SL[trainIndex].Tender;
                        if (trainCar->IsActive == 1) {
                            if (OgaHitCheck(trainCar->Position[0], trainCar->Position[2],
                                                        trainCar->Velocity[0], trainCar->Velocity[2], 30.0f, 20.0f,
                                                        playerPosX, playerPosZ) == 1) {
                                Player->Weapon |= HIT_BOMB;
                            }
                        }
                    }
                }

                for (passengerCarIndex = 0; passengerCarIndex < NUM_PASSENGER_CAR_ENTRIES; passengerCarIndex++) {
                    trainCar = &SL[trainIndex].PassengerCars[passengerCarIndex];
                    x_dist = playerPosX - trainCar->Position[0];
                    z_dist = playerPosZ - trainCar->Position[2];
                    if (trainCar->IsActive == 1) {
                        if ((x_dist > -100.0) && (x_dist < 100.0)) {
                            if ((z_dist > -100.0) && (z_dist < 100.0)) {
                                if (OgaHitCheck(trainCar->Position[0], trainCar->Position[2],
                                                            trainCar->Velocity[0], trainCar->Velocity[2], 30.0f, 20.0f,
                                                            playerPosX, playerPosZ) == 1) {
                                    Player->Weapon |= HIT_BOMB;
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
 * Implements FumikiriCounter as a Counter
 */
void FumikiriMove(void) {
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f12;
    s32 i;
    FumikiriMode[0] = 0;
    FumikiriMode[1] = 0;

    for (i = 0; i < NUM_TRAINS; i++) {
        temp_f16 = SL[i].Locomotive.PathPointIndex / ((f32) CourseObstacle);
        temp_f18 = 0.72017354f;
        temp_f12 = 0.42299348f;

        if (((temp_f12 - 0.1) < temp_f16) &&
            (temp_f16 < ((((f64) SL[i].NumCars) * 0.01) + (temp_f12 + 0.01)))) {

            FumikiriMode[0] = 1;
        }
        if (((temp_f18 - 0.1) < temp_f16) &&
            (temp_f16 < ((((f64) SL[i].NumCars) * 0.01) + (temp_f18 + 0.01)))) {

            FumikiriMode[1] = 1;
        }
    }

    for (i = 0; i < NUM_CROSSINGS; i++) {
        if (FumikiriMode[i] == 1) {
            FumikiriCounter[i] += 1;
        } else {
            FumikiriCounter[i] = 0;
        }
    }
}

void FumikiriWaitCheck(s32 PlayerId) {
    FumikiriStopFlg[PlayerId] = 0;
    if (CourseID == COURSE_KALAMARI_DESERT) {
        if ((!(TenukiFlg[PlayerId] != false)) ||
            (CheckPlayerNear(GamePlayers[PlayerId].Position, TRAIN_CROSSING_AI_DISTANCE, 0))) {

            if ((FumikiriMode[1] == 1) && ((FumikiriCounter[1]) > FRAMES_SINCE_CROSSING_ACTIVATED)) {

                if ((OGAArea > 176) && (OGAArea < 182)) {
                    FumikiriStopFlg[PlayerId] = 1;
                }
            }
            if ((FumikiriMode[0] == 1) && ((FumikiriCounter[0]) > FRAMES_SINCE_CROSSING_ACTIVATED)) {
                if ((OGAArea >= 306) && (OGAArea < 310)) {
                    FumikiriStopFlg[PlayerId] = 1;
                }
            }
        }
    }
}

void InitSHIP(void) {
    PaddleBoatStuff* paddleBoat;
    s32 i;
    Path2D* temp_a2;
    u16 temp;
    for (i = 0; i < NUM_ACTIVE_PADDLE_BOATS; i++) {
        temp = i * 0xB4;
        paddleBoat = &Ship[i];
        temp_a2 = &CenterLineEX[temp];
        paddleBoat->Position[0] = temp_a2->x;
        paddleBoat->Position[1] = D_80162EB2;
        paddleBoat->Position[2] = temp_a2->z;
        paddleBoat->PathPointIndex = i * 0xB4;
        paddleBoat->ActorIndex = -1;

        if (MenuMultiplayerSelection >= 3) {
            paddleBoat->IsActive = 0;
        } else {
            paddleBoat->IsActive = 1;
        }
        paddleBoat->Velocity[0] = 0.0f;
        paddleBoat->Velocity[1] = 0.0f;
        paddleBoat->Velocity[2] = 0.0f;
        paddleBoat->Speed = 1.6666666f;
        paddleBoat->RotY = 0;
    }
    FerrySmokeTimer = 0;
}

void MoveSHIP(void) {
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
    UNUSED s32 Pad2;
    FerrySmokeTimer += 1;
    for (i = 0; i < NUM_ACTIVE_PADDLE_BOATS; i++) {
        paddleBoat = &Ship[i];
        if (paddleBoat->IsActive == 1) {
            temp_f26 = paddleBoat->Position[0];
            temp_f28 = paddleBoat->Position[1];
            temp_f30 = paddleBoat->Position[2];
            HanyouSoukouSubEX(paddleBoat->Position, (s16*) &paddleBoat->PathPointIndex, paddleBoat->Speed);
            paddleBoat->SomeFlags = CheckPlayerNear(paddleBoat->Position, BOAT_SMOKE_RENDER_DISTANCE,
                                                                      paddleBoat->SomeFlags);
            if ((((s16) FerrySmokeTimer % 10) == 0) && (paddleBoat->SomeFlags != 0)) {
                smokePos[0] = (f32) ((f64) paddleBoat->Position[0] - 30.0);
                smokePos[1] = (f32) ((f64) paddleBoat->Position[1] + 180.0);
                smokePos[2] = (f32) ((f64) paddleBoat->Position[2] + 45.0);
                OgaGuruguruSub(smokePos, paddleBoat->Position, paddleBoat->RotY);
                KwgetShipSmoke(i, smokePos, 1.1f);
                smokePos[0] = (f32) ((f64) paddleBoat->Position[0] + 30.0);
                smokePos[1] = (f32) ((f64) paddleBoat->Position[1] + 180.0);
                smokePos[2] = (f32) ((f64) paddleBoat->Position[2] + 45.0);
                OgaGuruguruSub(smokePos, paddleBoat->Position, paddleBoat->RotY);
                KwgetShipSmoke(i, smokePos, 1.1f);
            }
            if (MakeRandomLimmit(100) == 0) {
                if (MakeRandomLimmit(2) == 0) {
                    NAEnmTrgStart(paddleBoat->Position, paddleBoat->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x47));
                } else {
                    NAEnmTrgStart(paddleBoat->Position, paddleBoat->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x80, 0x48));
                }
            }
            sp94[0] = temp_f26;
            sp94[1] = temp_f28;
            sp94[2] = temp_f30;
            pathPoint = &CenterLineEX[(paddleBoat->PathPointIndex + 5) % CourseObstacle];
            sp88[0] = (f32) pathPoint->x;
            sp88[1] = (f32) D_80162EB0;
            sp88[2] = (f32) pathPoint->z;
            temp_a1 = OgaCalcDirection(sp94, sp88);
            temp = temp_a1 - paddleBoat->RotY;
            var_v1 = temp;
            if (var_v1 < 0) {
                var_v1 = -var_v1;
            }
            if (var_v1 >= 0x1771) {
                if (paddleBoat->Speed > 0.2) {
                    paddleBoat->Speed -= 0.04;
                }
                if (var_v1 >= 0x3D) {
                    var_v1 = 0x003C;
                }
            } else {
                if (paddleBoat->Speed < 2.0) {
                    paddleBoat->Speed += 0.02;
                }
                if (var_v1 >= 0x1F) {
                    var_v1 = 0x001E;
                }
            }
            if (temp >= 0x8000) {
                paddleBoat->RotY -= var_v1;
            } else if (temp > 0) {
                paddleBoat->RotY += var_v1;
            } else if (temp < -0x7FFF) {
                paddleBoat->RotY += var_v1;
            } else if (temp < 0) {
                paddleBoat->RotY -= var_v1;
            }
            paddleBoat->Velocity[0] = paddleBoat->Position[0] - temp_f26;
            paddleBoat->Velocity[1] = paddleBoat->Position[1] - temp_f28;
            paddleBoat->Velocity[2] = paddleBoat->Position[2] - temp_f30;
            paddleBoatActor = &SimpleObjectArray[paddleBoat->ActorIndex];
            paddleBoatActor->Position[0] = paddleBoat->Position[0];
            paddleBoatActor->Position[1] = paddleBoat->Position[1];
            paddleBoatActor->Position[2] = paddleBoat->Position[2];
            if (ScreenFlip != 0) {
                paddleBoatActor->Angle[1] = -paddleBoat->RotY;
            } else {
                paddleBoatActor->Angle[1] = paddleBoat->RotY;
            }
            paddleBoatActor->Velocity[0] = paddleBoat->Velocity[0];
            paddleBoatActor->Velocity[1] = paddleBoat->Velocity[1];
            paddleBoatActor->Velocity[2] = paddleBoat->Velocity[2];
        }
    }
}

void HitCheckSHIP(Player* Player) {
    s32 someIndex;
    PaddleBoatStuff* tempPaddleWheelBoat;
    f32 x_diff;
    f32 y_diff;
    f32 z_diff;
    f32 playerX;
    f32 playerZ;
    f32 playerY;

    if (!((Player->SlipFlag & EXPLODE)) && (!(Player->SlipFlag & THROW_EXPLODE))) {
        playerX = Player->Position[0];
        playerY = Player->Position[1];
        playerZ = Player->Position[2];
        for (someIndex = 0; someIndex < NUM_ACTIVE_PADDLE_BOATS; someIndex++) {
            tempPaddleWheelBoat = &Ship[someIndex];
            if (tempPaddleWheelBoat->IsActive == 1) {
                x_diff = playerX - tempPaddleWheelBoat->Position[0];
                y_diff = playerY - tempPaddleWheelBoat->Position[1];
                z_diff = playerZ - tempPaddleWheelBoat->Position[2];
                if ((x_diff > -300.0) && (x_diff < 300.0)) {
                    if ((z_diff > -300.0) && (z_diff < 300.0)) {
                        if ((OgaHitCheck(tempPaddleWheelBoat->Position[0], tempPaddleWheelBoat->Position[2],
                                                     tempPaddleWheelBoat->Velocity[0], tempPaddleWheelBoat->Velocity[2],
                                                     200.0f, 60.0f, playerX, playerZ) == 1) &&
                            (y_diff < 60.0)) {
                            Player->Weapon |= HIT_BOAT;
                        }
                    }
                }
            }
        }
    }
}

void InitHIGHWAYCARSub(f32 speedA, f32 speedB, s32 numVehicles, s32 Arg3, VehicleStuff* vehicleList,
                                       CenterPathStruct* pathPointList) {
    VehicleStuff* veh;
    CenterPathStruct* temp_v0;
    s32 i;
    u16 pathPointOffset;
    s32 numPathPoints = CourseTotalPathPoints[0];
    for (i = 0; i < numVehicles; i++) {
        pathPointOffset = (((i * numPathPoints) / numVehicles) + Arg3) % numPathPoints;
        veh = &vehicleList[i];
        temp_v0 = &pathPointList[pathPointOffset];
        veh->Position[0] = (f32) temp_v0->Pointx;
        veh->Position[1] = (f32) temp_v0->Pointy;
        veh->Position[2] = (f32) temp_v0->Pointz;
        veh->ActorIndex = -1;
        veh->PathPointIndex = pathPointOffset;
        veh->Unused = 0;
        veh->Velocity[0] = 0.0f;
        veh->Velocity[1] = 0.0f;
        veh->Velocity[2] = 0.0f;
        veh->SomeFlags = 0;
        veh->SomeFlagsTheSequel = 0;
        if (GameMode == TIME_TRIALS) {
            veh->SomeType = (i % 3);
        } else {
            veh->SomeType = MakeRandomLimmit(3);
        }
        veh->SomeMultiplierTheSequel = (f32) ((f64) (f32) (veh->SomeType - 1) * 0.6);
        if (((RaceClass > CC_50) || (GameMode == TIME_TRIALS)) && (veh->SomeType == 2)) {
            veh->Speed = speedA;
        } else {
            veh->Speed = speedB;
        }
        veh->Rotation[0] = 0;
        veh->Rotation[2] = 0;
        if (HighwayGyakusouFlg == false) {
            veh->Rotation[1] = HanyouSoukouSubBP(veh->Position, (s16*) &veh->PathPointIndex, veh->Speed,
                                             veh->SomeMultiplierTheSequel, 0, 3);
        } else {
            veh->Rotation[1] =
                HanyouGyakuSoukouSubBP(veh->Position, (s16*) &veh->PathPointIndex, veh->Speed, veh->SomeMultiplierTheSequel, 0);
        }
    }
    SVehicleSoundRenderCounter = 10;
}

f32 ShasenChkSub(s16 SomeType, s16 PathPointIndex) {
    f32 var_f2;

    var_f2 = 0.0f;
    if (PathPointIndex < 0x28A) {
        switch (SomeType) {
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
        switch (SomeType) {
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

void MoveHIGHWAYCARSub(VehicleStuff* vehicle) {
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

    sp5C = vehicle->Position[0];
    sp58 = vehicle->Position[1];
    sp54 = vehicle->Position[2];
    sp40[0] = sp58;
    sp40[1] = 0.0f;
    sp40[2] = 0.0f;
    temp_f0_2 = ShasenChkSub(vehicle->SomeType, vehicle->PathPointIndex);
    if (vehicle->SomeMultiplierTheSequel < temp_f0_2) {
        vehicle->SomeMultiplierTheSequel = vehicle->SomeMultiplierTheSequel + 0.06;
        if (temp_f0_2 < vehicle->SomeMultiplierTheSequel) {
            vehicle->SomeMultiplierTheSequel = temp_f0_2;
        }
    }
    if (temp_f0_2 < vehicle->SomeMultiplierTheSequel) {
        vehicle->SomeMultiplierTheSequel = vehicle->SomeMultiplierTheSequel - 0.06;
        if (vehicle->SomeMultiplierTheSequel < temp_f0_2) {
            vehicle->SomeMultiplierTheSequel = temp_f0_2;
        }
    }
    if (HighwayGyakusouFlg == false) {
        var_a1 = HanyouSoukouSubBP(vehicle->Position, (s16*) &vehicle->PathPointIndex, vehicle->Speed,
                               vehicle->SomeMultiplierTheSequel, 0, 3);
    } else {
        var_a1 = HanyouGyakuSoukouSubBP(vehicle->Position, (s16*) &vehicle->PathPointIndex, vehicle->Speed,
                               vehicle->SomeMultiplierTheSequel, 0);
    }
    ChaseDir(&vehicle->Rotation[1], var_a1, 100);
    temp_f0_3 = vehicle->Position[0] - sp5C;
    temp_f2_2 = vehicle->Position[2] - sp54;
    sp34[0] = vehicle->Position[1];
    sp34[1] = 0.0f;
    sp34[2] = sqrtf((temp_f0_3 * temp_f0_3) + (temp_f2_2 * temp_f2_2));
    thing = CalcDirection(sp40, sp34);
    ChaseDir(&vehicle->Rotation[0], -thing, 100);
    vehicle->Velocity[0] = vehicle->Position[0] - sp5C;
    vehicle->Velocity[1] = vehicle->Position[1] - sp58;
    vehicle->Velocity[2] = vehicle->Position[2] - sp54;
    vehicleActor = &SimpleObjectArray[vehicle->ActorIndex];
    vehicleActor->Position[0] = vehicle->Position[0];
    vehicleActor->Position[1] = vehicle->Position[1];
    vehicleActor->Position[2] = vehicle->Position[2];
    vehicleActor->Angle[0] = vehicle->Rotation[0];
    if (ScreenFlip != 0) {
        vehicleActor->Angle[1] = -vehicle->Rotation[1];
    } else {
        vehicleActor->Angle[1] = vehicle->Rotation[1];
    }
    vehicleActor->Angle[2] = vehicle->Rotation[2];
    vehicleActor->Velocity[0] = vehicle->Velocity[0];
    vehicleActor->Velocity[1] = vehicle->Velocity[1];
    vehicleActor->Velocity[2] = vehicle->Velocity[2];
}

void HitCheckHIGHWAYCARSub(s32 PlayerId, Player* Player, VehicleStuff* vehicle, f32 distanceX, f32 distanceY,
                                 s32 vehicleCount, u32 SoundBits) {
    f32 deltaX;
    f32 deltaZ;
    f32 deltaY;

    s32 i;

    f32 playerX;
    f32 playerY;
    f32 playerZ;

    if (((TenukiFlg[PlayerId] != true) || ((((Player->Flag & IS_PLAYER) != 0)) && !(Player->Flag & IS_CPU_PLAYER))) &&
        !(Player->SlipFlag & EXPLODE)) {

        playerX = Player->Position[0];
        playerY = Player->Position[1];
        playerZ = Player->Position[2];

        for (i = 0; i < vehicleCount; i++) {
            deltaX = playerX - vehicle->Position[0];
            deltaY = playerY - vehicle->Position[1];
            deltaZ = playerZ - vehicle->Position[2];

            if (((deltaX) > -100.0) && ((deltaX) < 100.0)) {
                if ((deltaY > -20.0) && (deltaY < 20.0)) {
                    if (((deltaZ) > -100.0) && ((deltaZ) < 100.0)) {
                        if (OgaHitCheck(vehicle->Position[0], vehicle->Position[2], vehicle->Velocity[0],
                                                    vehicle->Velocity[2], distanceX, distanceY, playerX,
                                                    playerZ) == (s32) 1) {
                            Player->Weapon |= HIT_BOMB;
                        }
                    }
                }
            }

            // Human Player specific interactions
            if ((Player->Flag & IS_PLAYER) && !(Player->Flag & IS_CPU_PLAYER)) {
                if (((deltaX) > -300.0) && ((deltaX) < 300.0) && ((deltaY > -20.0)) && (deltaY < 20.0) &&
                    (((deltaZ) > -300.0)) && ((deltaZ) < 300.0)) {
                    if ((SVehicleSoundRenderCounter > 0) && (vehicle->SomeFlags == 0)) {
                        SVehicleSoundRenderCounter--;
                        vehicle->SomeFlags |= (RENDER_VEHICLE << PlayerId);
                        NaSceneLevelStart(vehicle->Position, vehicle->Velocity, SoundBits);
                    }
                } else {
                    if (vehicle->SomeFlags != 0) {
                        vehicle->SomeFlags &= ~(RENDER_VEHICLE << PlayerId);
                        if (vehicle->SomeFlags == 0) {
                            SVehicleSoundRenderCounter++;
                            NaSceneLevelStop(vehicle->Position, SoundBits);
                        }
                    }
                }

                if (((deltaX) > -200.0) && ((deltaX) < 200.0) && ((deltaY > -20.0)) && (deltaY < 20.0) &&
                    (((deltaZ) > -200.0)) && ((deltaZ) < 200.0)) {
                    if (!(vehicle->SomeFlagsTheSequel & ((1 << PlayerId)))) {

                        bool shouldInteract = false;
                        u16 path = CourseTotalPathPoints[0];
                        s32 t1;
                        s32 t2;

                        switch (HighwayGyakusouFlg) {
                            case false:
                                t1 = ExistAreaChk(vehicle->PathPointIndex,
                                                            NearestPathPointByPlayerId[PlayerId], 10, 0, path);
                                if ((WrongwayFlagPlayer1[PlayerId] == 0) && (t1 > 0) &&
                                    (Player->Speed < vehicle->Speed)) {
                                    shouldInteract = true;
                                }
                                if ((WrongwayFlagPlayer1[PlayerId] == 1) && (t1 > 0)) {
                                    shouldInteract = true;
                                }
                                break;
                            case true:
                                t2 = ExistAreaChk(vehicle->PathPointIndex,
                                                            NearestPathPointByPlayerId[PlayerId], 0, 10, path);
                                if (t2 > 0) {
                                    if (MakeRandomLimmit(2) == 0) {
                                        // temp_v1_2 = WrongwayFlagPlayer1[PlayerId];
                                        if (WrongwayFlagPlayer1[PlayerId] == 0) {
                                            shouldInteract = true;
                                        }
                                        if ((WrongwayFlagPlayer1[PlayerId] == 1) &&
                                            (Player->Speed < vehicle->Speed)) {
                                            shouldInteract = true;
                                        }
                                    } else {
                                        vehicle->SomeFlagsTheSequel |= ((1 << PlayerId));
                                    }
                                }
                                break;
                        }
                        if (shouldInteract == true) {

                            u32 soundBits2 = SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x3B);

                            switch (SoundBits) {
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
                            vehicle->SomeFlagsTheSequel |= ((1 << PlayerId));
                            NAEnmTrgStart(vehicle->Position, vehicle->Velocity, soundBits2);
                        }
                    }
                } else {
                    if (vehicle->SomeFlagsTheSequel & ((1 << PlayerId))) {
                        vehicle->SomeFlagsTheSequel &= ~((1 << PlayerId));
                    }
                }
            }
            vehicle++;
        }
    }
}

f32 AvoidHIGHWAYSub(s16 SomeType, f32 arg1, s16 PathIndex) {
    if (PathIndex < 0x28A) {
        switch (SomeType) {
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
        switch (SomeType) {
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

void AvoidHIGHWAYCARSub(s32 PlayerId, s32 vehicleCount, VehicleStuff* vehicle) {
    UNUSED s32 var_v1;
    s32 pathPointOffset;
    s32 var_s2;
    s32 pathPointCount;
    u16 vehiclePathPoint;
    UNUSED VehicleStuff* tempVehicle;

    pathPointCount = CourseTotalPathPoints[0];
    if (!(GamePlayers[PlayerId].Speed < 1.6666666666666667)) {
        for (var_s2 = 0; var_s2 < vehicleCount; var_s2++, vehicle++) {
            vehiclePathPoint = vehicle->PathPointIndex;
            for (pathPointOffset = 0; pathPointOffset < 0x18; pathPointOffset += 3) {
                if (((OGAArea + pathPointOffset) % pathPointCount) == vehiclePathPoint) {
                    LaneData[PlayerId].LaneTarget = AvoidHIGHWAYSub(
                        vehicle->SomeType, ZureData[PlayerId], vehiclePathPoint);
                    return;
                }
            }
        }
    }
}

void InitTRUCK(void) {
    f32 a = ((RaceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((RaceClass * 90.0) / 216.0f) + 2.9166666666666665;
    s32 numTrucks = NUM_RACE_BOX_TRUCKS;
    if (GameMode == TIME_TRIALS) {
        numTrucks = NUM_TIME_TRIAL_BOX_TRUCKS;
    }
    InitHIGHWAYCARSub(a, b, numTrucks, 0, TRUCK, &CenterPathBP[0][0]);
}

void MoveTRUCK(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_BOX_TRUCKS; loopIndex++) {
        MoveHIGHWAYCARSub(&TRUCK[loopIndex]);
    }
}

void HitCheckTRUCK(s32 PlayerId, Player* Player) {
    HitCheckHIGHWAYCARSub(PlayerId, Player, TRUCK, 55.0f, 12.5f, NUM_RACE_BOX_TRUCKS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x03));
}

void AvoidTRUCK(s32 PlayerId) {
    AvoidHIGHWAYCARSub(PlayerId, NUM_RACE_BOX_TRUCKS, TRUCK);
}

void InitBUS(void) {
    s32 numBusses;
    f32 a = ((RaceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((RaceClass * 90.0) / 216.0f) + 2.9166666666666665;

    numBusses = NUM_RACE_SCHOOL_BUSES;
    if (GameMode == TIME_TRIALS) {
        numBusses = NUM_TIME_TRIAL_SCHOOL_BUSES;
    }
    InitHIGHWAYCARSub(a, b, numBusses, 75, Bus, &CenterPathBP[0][0]);
}

void MoveBUS(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_SCHOOL_BUSES; loopIndex++) {
        MoveHIGHWAYCARSub(&Bus[loopIndex]);
    }
}

void HitCheckBUS(s32 PlayerId, Player* Player) {
    HitCheckHIGHWAYCARSub(PlayerId, Player, Bus, 70.0f, 12.5f, NUM_RACE_SCHOOL_BUSES,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x02));
}

void AvoidBUS(s32 PlayerId) {
    AvoidHIGHWAYCARSub(PlayerId, NUM_RACE_SCHOOL_BUSES, Bus);
}

void InitTANK(void) {
    s32 numTrucks;
    f32 a = ((RaceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((RaceClass * 90.0) / 216.0f) + 2.9166666666666665;

    numTrucks = NUM_RACE_TANKER_TRUCKS;
    if (GameMode == TIME_TRIALS) {
        numTrucks = NUM_TIME_TRIAL_TANKER_TRUCKS;
    }
    InitHIGHWAYCARSub(a, b, numTrucks, 50, Tanklorry, &CenterPathBP[0][0]);
}

void MoveTANK(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_TANKER_TRUCKS; loopIndex++) {
        MoveHIGHWAYCARSub(&Tanklorry[loopIndex]);
    }
}

void HitCheckTANK(s32 PlayerId, Player* Player) {
    HitCheckHIGHWAYCARSub(PlayerId, Player, Tanklorry, 55.0f, 12.5f, NUM_RACE_TANKER_TRUCKS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x04));
}

void AvoidTANK(s32 PlayerId) {
    AvoidHIGHWAYCARSub(PlayerId, NUM_RACE_TANKER_TRUCKS, Tanklorry);
}

void InitRV(void) {
    s32 NumCars;
    f32 a = ((RaceClass * 90.0) / 216.0f) + 4.583333333333333;
    f32 b = ((RaceClass * 90.0) / 216.0f) + 2.9166666666666665;

    NumCars = NUM_RACE_CARS;
    if (GameMode == TIME_TRIALS) {
        NumCars = NUM_TIME_TRIAL_CARS;
    }
    InitHIGHWAYCARSub(a, b, NumCars, 25, RvCar, &CenterPathBP[0][0]);
}

void MoveRV(void) {
    s32 loopIndex;
    for (loopIndex = 0; loopIndex < NUM_RACE_CARS; loopIndex++) {
        MoveHIGHWAYCARSub(&RvCar[loopIndex]);
    }
}

void HitCheckRV(s32 PlayerId, Player* Player) {
    HitCheckHIGHWAYCARSub(PlayerId, Player, RvCar, 11.5f, 8.5f, NUM_RACE_CARS,
                                SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x05));
}

void AvoidRV(s32 PlayerId) {
    AvoidHIGHWAYCARSub(PlayerId, NUM_RACE_CARS, RvCar);
}
