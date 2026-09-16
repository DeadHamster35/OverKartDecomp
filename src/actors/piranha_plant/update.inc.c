#include <actors.h>

/**
 * @brief Updates the piranha plant actor.
 * Actor used in Mario Raceway and Royal Raceway.
 *
 * @param arg0
 */
void pakkun_strategy(PakkunObject* arg0) {
    if ((arg0->flag & HIDEOBJ) == 0) {
        if ((arg0->flag & FLYINGOBJ) != 0) {
            arg0->position[1] += 4.0f;
            if (arg0->position[1] > 800.0f) {
                arg0->flag |= HIDEOBJ;
            }
        } else {
            // This has to be an unrolled loop
            if (arg0->visibilityStates[0] == 1) {
                arg0->timers[0]++;
                if (arg0->timers[0] > 60) {
                    arg0->timers[0] = 6;
                }
            } else {
                arg0->timers[0] = 0;
            }
            if (arg0->visibilityStates[1] == 1) {
                arg0->timers[1]++;
                if (arg0->timers[1] > 60) {
                    arg0->timers[1] = 6;
                }
            } else {
                arg0->timers[1] = 0;
            }
            if (arg0->visibilityStates[2] == 1) {
                arg0->timers[2]++;
                if (arg0->timers[2] > 60) {
                    arg0->timers[2] = 6;
                }
            } else {
                arg0->timers[2] = 0;
            }
            if (arg0->visibilityStates[3] == 1) {
                arg0->timers[3]++;
                if (arg0->timers[3] > 60) {
                    arg0->timers[3] = 6;
                }
            } else {
                arg0->timers[3] = 0;
            }
        }
    }
}
