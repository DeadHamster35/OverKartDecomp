#include <actors.h>

/**
 * @brief Updates the Wario sign actor.
 * Used in Wario Stadium.
 *
 * @param arg0
 */
void wkanban_strategy(Object* arg0) {
    arg0->angle[1] += DEGREES(1);
}
