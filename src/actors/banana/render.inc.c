#include <actors.h>
#include <code_800029B0.h>
#include <PR/gbi.h>

/**
 * @brief Render the banana actor
 *
 * @Param Camera
 * @Param arg1
 * @Param banana
 */
void DisplayBanana(Camera* Camera, UNUSED Mat4 arg1, struct BananaActor* banana) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3s sp7C;
    Mat4 sp3C;

    f32 temp =
        CheckDisplayRange(Camera->CameraPos, banana->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 490000.0f);
    if (temp < 0.0f) {
        OutOfScreen(Camera, (Object*) banana);
        return;
    }

    if ((banana->Position[1] > MaximumY + 800.0f)) {
        OutOfScreen(Camera, (Object*) banana);
        return;
    }
    if (banana->Position[1] < (MinimumY - 800.0f)) {
        OutOfScreen(Camera, (Object*) banana);
        return;
    }

    OnScreen(Camera, (Object*) banana);

    if (banana->Sparam == 5) {
        CreateModelingMatrix(sp3C, banana->Position, banana->Angle);
    } else {
        sp7C[0] = 0;
        sp7C[1] = 0;
        sp7C[2] = 0;
        CreateModelingMatrix(sp3C, banana->Position, sp7C);
    }

    maxObjectsReached = SetMatrix(sp3C, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (banana->Sparam != 5) {
        gSPDisplayList(DisplayListHead++, &CommonModelBanana);
    } else {
        gSPDisplayList(DisplayListHead++, &CommonModelFlatBanana);
    }
}
