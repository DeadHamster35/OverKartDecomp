#include <actors.h>
#include <Main.h>
#include <defines.h>

/**
 * @brief Renders the tanker truck actor.
 * Actor Used in Toad's Turnpike.
 * His update are made in vehicle.
 *
 * @Param Camera
 * @Param arg1
 */
void DisplayTanklorry(Camera* Camera, Object* arg1) {
    UNUSED s32 pad[6];
    Mat4 spC8;
    UNUSED s32 Pad2[32];
    f32 temp_f0 = CheckDisplayRange(Camera->CameraPos, arg1->Position, Camera->CameraDirection[1], 2500.0f,
                                          CameraZoom[Camera - Camera1], 9000000.0f);

    if (!(temp_f0 < 0.0f)) {

        gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

        CreateModelingMatrix(spC8, arg1->Position, arg1->Angle);
        if (SetMatrix(spC8, 0) != 0) {

            if (ScreenSplitA == SCREEN_MODE_1P) {
                if (temp_f0 < 160000.0f) {

                    gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_6);
                } else if (temp_f0 < 640000.0f) {
                    gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_7);
                } else {
                    gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_8);
                }
            } else if (temp_f0 < 160000.0f) {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_7);
            } else {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_8);
            }
        }
    }
}
