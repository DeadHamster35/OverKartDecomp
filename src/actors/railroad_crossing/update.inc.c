#include <actors.h>

/**
 * @brief Updates the railroad crossing actor.
 * Actor used in Kalimari Desert.
 * Make the sound of the bell when the train is close.
 *
 * @param crossing
 */
void fumikiri_strategy(struct RailroadCrossing* crossing) {
    // If train close?
    if (fumikiri_mode[crossing->crossingId] != 0) {
        // Timer++
        crossing->someTimer++;
        // Reset timer
        if (crossing->someTimer > 40) {
            crossing->someTimer = 1;
        }
        // Play Bell sound when timer hits 20 or 1.
        if ((crossing->someTimer == 1) || (crossing->someTimer == 20)) {
            NAEnmTrgStart(crossing->position, crossing->velocity, SOUND_ARG_LOAD(0x19, 0x01, 0x70, 0x16));
        }
    }
}
