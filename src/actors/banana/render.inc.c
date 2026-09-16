#include <actors.h>
#include <code_800029B0.h>
#include <PR/gbi.h>

/**
 * @brief Render the banana actor
 *
 * @param camera
 * @param arg1
 * @param banana
 */
void display_banana(Camera* camera, UNUSED Mat4 arg1, struct BananaActor* banana) {
    UNUSED s32 pad[2];
    s32 maxObjectsReached;
    Vec3s sp7C;
    Mat4 sp3C;

    f32 temp =
        CheckDisplayRange(camera->camera_pos, banana->position, camera->camera_direction[1], 0, gCameraZoom[camera - camera1], 490000.0f);
    if (temp < 0.0f) {
        out_of_screen(camera, (Object*) banana);
        return;
    }

    if ((banana->position[1] > maximum_y + 800.0f)) {
        out_of_screen(camera, (Object*) banana);
        return;
    }
    if (banana->position[1] < (minimum_y - 800.0f)) {
        out_of_screen(camera, (Object*) banana);
        return;
    }

    on_screen(camera, (Object*) banana);

    if (banana->sparam == 5) {
        CreateModelingMatrix(sp3C, banana->position, banana->angle);
    } else {
        sp7C[0] = 0;
        sp7C[1] = 0;
        sp7C[2] = 0;
        CreateModelingMatrix(sp3C, banana->position, sp7C);
    }

    maxObjectsReached = SetMatrix(sp3C, 0) == 0;
    if (maxObjectsReached) {
        return;
    }

    if (banana->sparam != 5) {
        gSPDisplayList(gDisplayListHead++, &common_model_banana);
    } else {
        gSPDisplayList(gDisplayListHead++, &common_model_flat_banana);
    }
}
