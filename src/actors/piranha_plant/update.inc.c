#include <actors.h>

/**
 * @brief Updates the piranha plant actor.
 * Actor Used in Mario Raceway and Royal Raceway.
 *
 * @Param arg0
 */
void PakkunStrategy(PakkunObject* arg0) {
    if ((arg0->Flag & HIDEOBJ) == 0) {
        if ((arg0->Flag & FLYINGOBJ) != 0) {
            arg0->Position[1] += 4.0f;
            if (arg0->Position[1] > 800.0f) {
                arg0->Flag |= HIDEOBJ;
            }
        } else {
            // This has to be an unrolled Loop
            if (arg0->VisibilityStates[0] == 1) {
                arg0->Timers[0]++;
                if (arg0->Timers[0] > 60) {
                    arg0->Timers[0] = 6;
                }
            } else {
                arg0->Timers[0] = 0;
            }
            if (arg0->VisibilityStates[1] == 1) {
                arg0->Timers[1]++;
                if (arg0->Timers[1] > 60) {
                    arg0->Timers[1] = 6;
                }
            } else {
                arg0->Timers[1] = 0;
            }
            if (arg0->VisibilityStates[2] == 1) {
                arg0->Timers[2]++;
                if (arg0->Timers[2] > 60) {
                    arg0->Timers[2] = 6;
                }
            } else {
                arg0->Timers[2] = 0;
            }
            if (arg0->VisibilityStates[3] == 1) {
                arg0->Timers[3]++;
                if (arg0->Timers[3] > 60) {
                    arg0->Timers[3] = 6;
                }
            } else {
                arg0->Timers[3] = 0;
            }
        }
    }
}
