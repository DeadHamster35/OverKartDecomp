#include <actors.h>
#include <Main.h>
#include <defines.h>

/**
 * @brief Renders the car actor.
 * Actor Used in Toad's Turnpike.
 * His update are made in vehicle.
 *
 * @Param arg0
 * @Param arg1
 */
void DisplayRv(Camera* arg0, Object* arg1) {
    UNUSED s32 pad[6];
    Mat4 spC8;
    UNUSED s32 Pad2[32];
    f32 temp_f0 =
        CheckDisplayRange(arg0->CameraPos, arg1->Position, arg0->CameraDirection[1], 2500.0f, CameraZoom[arg0 - Camera1], 9000000.0f);

    if (!(temp_f0 < 0.0f)) {

        gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

        CreateModelingMatrix(spC8, arg1->Position, arg1->Angle);
        ScalingMatrix(spC8, 0.1f);
        if (SetMatrix(spC8, 0) != 0) {

            if (ScreenSplitA == SCREEN_MODE_1P) {
                if (temp_f0 < 160000.0f) {
                    gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_9);
                } else if (temp_f0 < 640000.0f) {
                    gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_10);
                } else {
                    gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_11);
                }
            } else if (temp_f0 < 160000.0f) {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_10);
            } else {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_11);
            }
        }
    }
}
