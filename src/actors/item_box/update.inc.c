#include <actors.h>

/**
 * @brief Updates the Item box under the hot air balloon object.
 * It Used in Luigi Raceway.
 * (here it's just update the Rotation of the Item box not the Position, for the Position it's in object)
 *
 * @Param itemBox
 */
void MoveSpecialItembox(struct ItemBox* itemBox) {
    switch (itemBox->Sparam) {
        case 5:
            itemBox->Angle[0] += DEGREES(1);
            itemBox->Angle[1] -= DEGREES(2);
            itemBox->Angle[2] += DEGREES(1);
            break;
        case 3:
            if (itemBox->SomeTimer == 0x14) {
                itemBox->Sparam = 5;
                itemBox->Flag = -0x4000;
            } else {
                itemBox->SomeTimer++;
                itemBox->Angle[0] += DEGREES(6);
                itemBox->Angle[1] -= DEGREES(4);
                itemBox->Angle[2] += DEGREES(2);
            }
            break;
    }
}

/**
 * @brief Updates the Item box actor.
 *
 * @Param itemBox
 */
void MoveItembox(struct ItemBox* itemBox) {
    switch (itemBox->Sparam) {
        case 0:
            itemBox->Sparam = 1;
            break;
        case 1:
            if ((itemBox->Position[1] - itemBox->OrigY) < 8.66f) {
                itemBox->Position[1] += 0.45f;
            } else {
                itemBox->Position[1] = itemBox->OrigY + 8.66f;
                itemBox->Sparam = 2;
                itemBox->Flag = 0xC000;
            }
            break;
        case 2:
            itemBox->Angle[0] += DEGREES(1);
            itemBox->Angle[1] -= DEGREES(2);
            itemBox->Angle[2] += DEGREES(1);
            break;
        case 3:
            if (itemBox->SomeTimer == 20) {
                itemBox->Sparam = 0;
                itemBox->Position[1] = itemBox->ResetDistance - 20.0f;
                itemBox->Flag = 0xC000;
            } else {
                itemBox->SomeTimer++;
                itemBox->Angle[0] += DEGREES(6);
                itemBox->Angle[1] -= DEGREES(4);
                itemBox->Angle[2] += DEGREES(2);
            }
            break;
    }
}
