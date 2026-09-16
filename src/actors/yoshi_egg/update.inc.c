#include <actors.h>

/**
 * @brief Updates the Yoshi egg actor.
 *
 * @param egg
 */
void egg_strategy(struct YoshiValleyEgg* egg) {
    egg->pathRot += 0x5B; // half a degree
    egg->position[0] = egg->pathCenter[0] + (sinT(egg->pathRot) * egg->pathRadius);
    egg->position[2] = egg->pathCenter[2] + (cosT(egg->pathRot) * egg->pathRadius);
    if ((egg->flag & FLYINGOBJ) != 0) {
        egg->pathCenter[1] -= 0.12;
        if (egg->pathCenter[1] < -3.0f) {
            egg->pathCenter[1] = -3.0f;
        }
        egg->position[1] += egg->pathCenter[1];
        if (egg->position[1] < 0.0f) {
            egg->position[1] = 0.0f;
            egg->pathCenter[1] = 0.0f;
            egg->flag &= ~(1 << 10);
        }
        egg->eggRot -= DEGREES(7);
    }
    egg->eggRot -= DEGREES(3);
}
