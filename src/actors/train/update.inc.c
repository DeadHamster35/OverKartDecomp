#include <actors.h>

/**
 * @brief Updates the train engine actor.
 * Used in Kalimari Desert.
 * Update wheels and make sound, for his update position it's in vehicle.
 *
 * @param arg0
 */
void sl1_strategy(struct TrainCar* arg0) {
    arg0->wheelRot -= DEGREES(9);

    if (arg0->fparam != 0.0f) {
        arg0->fparam = 0.0f;
        NaSceneLevelStart(arg0->position, arg0->velocity, SOUND_ARG_LOAD(0x51, 0x01, 0x80, 0x00));
    }
}

/**
 * @brief Updates the train tender actor.
 * Used in Kalimari Desert.
 * Update wheels, for his update position it's in vehicle.
 * @param tender
 */
void sl2_strategy(struct TrainCar* tender) {
    tender->wheelRot -= DEGREES(7);
}

/**
 * @brief Updates the train passenger car actor.
 * Used in Kalimari Desert.
 * Update wheels, for his update position it's in vehicle.
 *
 * @param arg0
 */
void sl3_strategy(struct TrainCar* arg0) {
    arg0->wheelRot -= DEGREES(9);
}
