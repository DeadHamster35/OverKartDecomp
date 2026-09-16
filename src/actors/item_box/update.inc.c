#include <actors.h>

/**
 * @brief Updates the item box under the hot air balloon object.
 * It used in Luigi Raceway.
 * (here it's just update the rotation of the item box not the position, for the position it's in object)
 *
 * @param itemBox
 */
void move_special_itembox(struct ItemBox* itemBox) {
    switch (itemBox->sparam) {
        case 5:
            itemBox->angle[0] += DEGREES(1);
            itemBox->angle[1] -= DEGREES(2);
            itemBox->angle[2] += DEGREES(1);
            break;
        case 3:
            if (itemBox->someTimer == 0x14) {
                itemBox->sparam = 5;
                itemBox->flag = -0x4000;
            } else {
                itemBox->someTimer++;
                itemBox->angle[0] += DEGREES(6);
                itemBox->angle[1] -= DEGREES(4);
                itemBox->angle[2] += DEGREES(2);
            }
            break;
    }
}

/**
 * @brief Updates the item box actor.
 *
 * @param itemBox
 */
void move_itembox(struct ItemBox* itemBox) {
    switch (itemBox->sparam) {
        case 0:
            itemBox->sparam = 1;
            break;
        case 1:
            if ((itemBox->position[1] - itemBox->origY) < 8.66f) {
                itemBox->position[1] += 0.45f;
            } else {
                itemBox->position[1] = itemBox->origY + 8.66f;
                itemBox->sparam = 2;
                itemBox->flag = 0xC000;
            }
            break;
        case 2:
            itemBox->angle[0] += DEGREES(1);
            itemBox->angle[1] -= DEGREES(2);
            itemBox->angle[2] += DEGREES(1);
            break;
        case 3:
            if (itemBox->someTimer == 20) {
                itemBox->sparam = 0;
                itemBox->position[1] = itemBox->resetDistance - 20.0f;
                itemBox->flag = 0xC000;
            } else {
                itemBox->someTimer++;
                itemBox->angle[0] += DEGREES(6);
                itemBox->angle[1] -= DEGREES(4);
                itemBox->angle[2] += DEGREES(2);
            }
            break;
    }
}
