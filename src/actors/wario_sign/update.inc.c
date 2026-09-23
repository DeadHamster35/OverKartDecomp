#include <actors.h>

/**
 * @brief Updates the Wario sign actor.
 * Used in Wario Stadium.
 *
 * @Param arg0
 */
void WkanbanStrategy(Object* arg0) {
    arg0->Angle[1] += DEGREES(1);
}
