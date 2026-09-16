#include <actors.h>

/**
 * @brief Updates the Mario sign actor.
 *
 * @param arg0
 */
void mkanban_strategy(Object* arg0) {
    if ((arg0->flag & HIDEOBJ) == 0) {
        if ((arg0->flag & FLYINGOBJ) != 0) {
            arg0->position[1] += 4.0f;
            if (arg0->position[1] > 800.0f) {
                arg0->flag |= HIDEOBJ;
                arg0->angle[1] += DEGREES(10);
            }
        } else {
            arg0->angle[1] += DEGREES(1);
        }
    }
}
