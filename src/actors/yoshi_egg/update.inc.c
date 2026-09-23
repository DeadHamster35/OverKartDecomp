#include <actors.h>

/**
 * @brief Updates the Yoshi egg actor.
 *
 * @Param egg
 */
void EggStrategy(struct YoshiValleyEgg* egg) {
    egg->PathRot += 0x5B; // half a degree
    egg->Position[0] = egg->PathCenter[0] + (SinT(egg->PathRot) * egg->PathRadius);
    egg->Position[2] = egg->PathCenter[2] + (CosT(egg->PathRot) * egg->PathRadius);
    if ((egg->Flag & FLYINGOBJ) != 0) {
        egg->PathCenter[1] -= 0.12;
        if (egg->PathCenter[1] < -3.0f) {
            egg->PathCenter[1] = -3.0f;
        }
        egg->Position[1] += egg->PathCenter[1];
        if (egg->Position[1] < 0.0f) {
            egg->Position[1] = 0.0f;
            egg->PathCenter[1] = 0.0f;
            egg->Flag &= ~(1 << 10);
        }
        egg->EggRot -= DEGREES(7);
    }
    egg->EggRot -= DEGREES(3);
}
