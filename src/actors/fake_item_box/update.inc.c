#include <actors.h>
#include <Main.h>

/**
 * @brief Updates the fake Item box actor.
 *
 * @Param fake_item_box
 */
void MoveFakeItembox(struct FakeItemBox* fake_item_box) {
    u32 temp_v1 = fake_item_box->PlayerId;
    Player* temp_v0_4 = &GamePlayers[temp_v1];
    struct Controller* temp_v1_3;

    UNUSED s32 pad[7];
    f32 temp_f2_2;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    UNUSED s32 Pad2[3];

    switch (fake_item_box->Sparam) {
        case 0:
            fake_item_box->Radius = fake_item_box->SizeScaling * 5.5f;
            fake_item_box->Angle[0] -= DEGREES(1);
            fake_item_box->Angle[1] += DEGREES(2);
            fake_item_box->Angle[2] -= DEGREES(1);

            temp_f14 = temp_v0_4->Position[0] - fake_item_box->Position[0];
            temp_f16 = temp_v0_4->Position[1] - fake_item_box->Position[1];
            temp_f18 = temp_v0_4->Position[2] - fake_item_box->Position[2];

            temp_f2_2 = sqrtf((temp_f14 * temp_f14) + (temp_f16 * temp_f16) + (temp_f18 * temp_f18)) / 10.0f;
            temp_f14 /= temp_f2_2;
            temp_f16 /= temp_f2_2;
            temp_f18 /= temp_f2_2;
            fake_item_box->Position[0] = temp_v0_4->Position[0] - temp_f14;
            fake_item_box->Position[1] = (temp_v0_4->Position[1] - temp_f16) - 1.0f;
            fake_item_box->Position[2] = temp_v0_4->Position[2] - temp_f18;
            CheckBump(&fake_item_box->Bump, fake_item_box->Radius, fake_item_box->Position[0],
                                     fake_item_box->Position[1], fake_item_box->Position[2]);
            BumpObject((Object*) fake_item_box);
            temp_v1_3 = &Controllers[temp_v1];
            if ((temp_v0_4->Flag & IS_PLAYER) != 0) {

                if ((temp_v1_3->ButtonReleased & Z_TRIG) != 0) {
                    temp_v1_3->ButtonReleased &= ~Z_TRIG;
                    ReleaseFakeItembox(fake_item_box);
                    temp_v0_4->Weapon &= ~USE_BANANA;
                    NAPlyTrgStart((u8) (temp_v0_4 - PlayerOne), SOUND_ARG_LOAD(0x19, 0x00, 0x80, 0x12));
                }
            }
            break;
        case 1:
            if (fake_item_box->SizeScaling < 1.0f) {
                fake_item_box->SizeScaling += 0.05f;
            } else if (fake_item_box->SizeScaling >= 1.0f) {
                fake_item_box->SizeScaling = 1.0f;
            }

            fake_item_box->Radius = fake_item_box->SizeScaling * 5.5f;
            if (fake_item_box->TargetY <= fake_item_box->Position[1]) {
                fake_item_box->Position[1] = fake_item_box->TargetY;
            } else {
                fake_item_box->Position[1] += 0.2f;
            }
            if ((fake_item_box->Flag & MYOBJ) != 0) {
                if ((fake_item_box->SomeTimer <= 0) || (fake_item_box->SomeTimer >= 0x12D)) {
                    fake_item_box->Flag &= 0xEFFF;
                    fake_item_box->SomeTimer = 0;
                } else {
                    fake_item_box->SomeTimer--;
                }
            }
            fake_item_box->Angle[0] -= DEGREES(1);
            fake_item_box->Angle[1] += DEGREES(2);
            fake_item_box->Angle[2] -= DEGREES(1);
            break;

        case 2:
            if ((fake_item_box->SomeTimer >= 0x14) || (fake_item_box->SomeTimer < 0)) {
                DeleteObjectBuffer((Object*) fake_item_box);
            } else {
                fake_item_box->SomeTimer++;
                fake_item_box->Angle[0] += DEGREES(6);
                fake_item_box->Angle[1] -= DEGREES(4);
                fake_item_box->Angle[2] += DEGREES(2);
            }
            break;
        default:
            DeleteObjectBuffer((Object*) fake_item_box);
            break;
    }
}
