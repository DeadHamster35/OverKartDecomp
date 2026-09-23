#include <actors.h>

/**
 * @brief Updates the Mario sign actor.
 *
 * @Param arg0
 */
void MkanbanStrategy(Object* arg0) {
    if ((arg0->Flag & HIDEOBJ) == 0) {
        if ((arg0->Flag & FLYINGOBJ) != 0) {
            arg0->Position[1] += 4.0f;
            if (arg0->Position[1] > 800.0f) {
                arg0->Flag |= HIDEOBJ;
                arg0->Angle[1] += DEGREES(10);
            }
        } else {
            arg0->Angle[1] += DEGREES(1);
        }
    }
}
