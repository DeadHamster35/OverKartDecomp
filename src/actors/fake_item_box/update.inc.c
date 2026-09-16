#include <actors.h>
#include <main.h>

/**
 * @brief Updates the fake item box actor.
 *
 * @param fake_item_box
 */
void move_fake_itembox(struct FakeItemBox* fake_item_box) {
    u32 temp_v1 = fake_item_box->playerId;
    Player* temp_v0_4 = &gPlayers[temp_v1];
    struct Controller* temp_v1_3;

    UNUSED s32 pad[7];
    f32 temp_f2_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    UNUSED s32 pad2[3];

    switch (fake_item_box->sparam) {
        case 0:
            fake_item_box->radius = fake_item_box->sizeScaling * 5.5f;
            fake_item_box->angle[0] -= DEGREES(1);
            fake_item_box->angle[1] += DEGREES(2);
            fake_item_box->angle[2] -= DEGREES(1);

            temp_f14 = temp_v0_4->position[0] - fake_item_box->position[0];
            temp_f16 = temp_v0_4->position[1] - fake_item_box->position[1];
            temp_f18 = temp_v0_4->position[2] - fake_item_box->position[2];

            temp_f2_2 = sqrtf((temp_f14 * temp_f14) + (temp_f16 * temp_f16) + (temp_f18 * temp_f18)) / 10.0f;
            temp_f14 /= temp_f2_2;
            temp_f16 /= temp_f2_2;
            temp_f18 /= temp_f2_2;
            fake_item_box->position[0] = temp_v0_4->position[0] - temp_f14;
            fake_item_box->position[1] = (temp_v0_4->position[1] - temp_f16) - 1.0f;
            fake_item_box->position[2] = temp_v0_4->position[2] - temp_f18;
            CheckBump(&fake_item_box->bump, fake_item_box->radius, fake_item_box->position[0],
                                     fake_item_box->position[1], fake_item_box->position[2]);
            BumpObject((Object*) fake_item_box);
            temp_v1_3 = &gControllers[temp_v1];
            if ((temp_v0_4->flag & IS_PLAYER) != 0) {

                if ((temp_v1_3->ButtonReleased & Z_TRIG) != 0) {
                    temp_v1_3->ButtonReleased &= ~Z_TRIG;
                    release_fake_itembox(fake_item_box);
                    temp_v0_4->weapon &= ~USE_BANANA;
                    NAPlyTrgStart((u8) (temp_v0_4 - gPlayerOne), SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                }
            }
            break;
        case 1:
            if (fake_item_box->sizeScaling < 1.0f) {
                fake_item_box->sizeScaling += 0.05f;
            } else if (fake_item_box->sizeScaling >= 1.0f) {
                fake_item_box->sizeScaling = 1.0f;
            }

            fake_item_box->radius = fake_item_box->sizeScaling * 5.5f;
            if (fake_item_box->targetY <= fake_item_box->position[1]) {
                fake_item_box->position[1] = fake_item_box->targetY;
            } else {
                fake_item_box->position[1] += 0.2f;
            }
            if ((fake_item_box->flag & MYOBJ) != 0) {
                if ((fake_item_box->someTimer <= 0) || (fake_item_box->someTimer >= 0x12D)) {
                    fake_item_box->flag &= 0xEFFF;
                    fake_item_box->someTimer = 0;
                } else {
                    fake_item_box->someTimer--;
                }
            }
            fake_item_box->angle[0] -= DEGREES(1);
            fake_item_box->angle[1] += DEGREES(2);
            fake_item_box->angle[2] -= DEGREES(1);
            break;

        case 2:
            if ((fake_item_box->someTimer >= 0x14) || (fake_item_box->someTimer < 0)) {
                deleteObjectBuffer((Object*) fake_item_box);
            } else {
                fake_item_box->someTimer++;
                fake_item_box->angle[0] += DEGREES(6);
                fake_item_box->angle[1] -= DEGREES(4);
                fake_item_box->angle[2] += DEGREES(2);
            }
            break;
        default:
            deleteObjectBuffer((Object*) fake_item_box);
            break;
    }
}
