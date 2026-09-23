#include <actors.h>

/**
 * @brief Updates the train engine actor.
 * Used in Kalimari Desert.
 * Update wheels and make Sound, for his update Position it's in vehicle.
 *
 * @Param arg0
 */
void Sl1Strategy(struct TrainCar* arg0) {
    arg0->WheelRot -= DEGREES(9);

    if (arg0->Fparam != 0.0f) {
        arg0->Fparam = 0.0f;
        NaSceneLevelStart(arg0->Position, arg0->Velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x00));
    }
}

/**
 * @brief Updates the train Tender actor.
 * Used in Kalimari Desert.
 * Update wheels, for his update Position it's in vehicle.
 * @Param Tender
 */
void Sl2Strategy(struct TrainCar* Tender) {
    Tender->WheelRot -= DEGREES(7);
}

/**
 * @brief Updates the train passenger car actor.
 * Used in Kalimari Desert.
 * Update wheels, for his update Position it's in vehicle.
 *
 * @Param arg0
 */
void Sl3Strategy(struct TrainCar* arg0) {
    arg0->WheelRot -= DEGREES(9);
}
