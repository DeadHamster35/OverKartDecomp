#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include <defines.h>

/**
 * @brief Renders the school Bus actor.
 * Actor Used in Toad's Turnpike.
 * His update are made in vehicle.
 *
 * @Param arg0
 * @Param arg1
 */
void DisplayBus(Camera* arg0, Object* arg1) {
    UNUSED s32 pad[6];
    Mat4 spC8;
    UNUSED s32 Pad2[32];
    f32 temp_f0;

    temp_f0 = CheckDisplayRange(arg0->CameraPos, arg1->Position, arg0->CameraDirection[1], 2500.0f,
                                CameraZoom[arg0 - Camera1], 9000000.0f);
    if (temp_f0 < 0.0f) {
        return;
    }

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    CreateModelingMatrix(spC8, arg1->Position, arg1->Angle);
    if (SetMatrix(spC8, 0) != 0) {

        if (ScreenSplitA == SCREEN_MODE_1P) {
            if (temp_f0 < 160000.0f) {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_3);
            } else if (temp_f0 < 640000.0f) {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_4);
            } else {
                gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_5);
            }
        } else if (temp_f0 < 160000.0f) {
            gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_4);
        } else {
            gSPDisplayList(DisplayListHead++, &toads_turnpike_dl_5);
        }
    }
}
