#include <actors.h>

/**
 * @brief Updates the railroad crossing actor.
 * Actor Used in Kalimari Desert.
 * Make the Sound of the bell when the train is close.
 *
 * @Param crossing
 */
void FumikiriStrategy(struct RailroadCrossing* crossing) {
    // If train close?
    if (FumikiriMode[crossing->CrossingId] != 0) {
        // Timer++
        crossing->SomeTimer++;
        // Reset Timer
        if (crossing->SomeTimer > 40) {
            crossing->SomeTimer = 1;
        }
        // Play Bell Sound when Timer hits 20 or 1.
        if ((crossing->SomeTimer == 1) || (crossing->SomeTimer == 20)) {
            NAEnmTrgStart(crossing->Position, crossing->Velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x16));
        }
    }
}
