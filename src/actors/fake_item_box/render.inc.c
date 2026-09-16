#include <actors.h>
#include <code_800029B0.h>
#include <PR/gbi.h>
#include <main.h>

/**
 * @brief Renders the fake item box actor.
 *
 * @param camera
 * @param fakeItemBox
 */
void display_fake_itembox(Camera* camera, struct FakeItemBox* fakeItemBox) {
    Vec3s someRot;
    UNUSED s32 pad[3];
    Vec3f someVec;
    Mat4 someMatrix2;
    Mat4 someMatrix3;
    UNUSED s32 pad2[12];
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f2;
    f32 thing;
    f32 temp_f2_2;
    f32 someMultiplier;

    if (CheckDisplayRange(camera->camera_pos, fakeItemBox->position, camera->camera_direction[1], 2500.0f, gCameraZoom[camera - camera1],
                                1000000.0f) < 0) {
        out_of_screen(camera, (Object*) fakeItemBox);
        return;
    }
    if (((f32) maximum_y + 800.0f) < fakeItemBox->position[1]) {
        out_of_screen(camera, (Object*) fakeItemBox);
        return;
    }
    if (fakeItemBox->position[1] < ((f32) minimum_y - 800.0f)) {
        out_of_screen(camera, (Object*) fakeItemBox);
        return;
    }

    on_screen(camera, (Object*) fakeItemBox);
    someRot[0] = 0;
    someRot[1] = fakeItemBox->angle[1];
    someRot[2] = 0;
    CreateModelingMatrix(someMatrix2, fakeItemBox->position, someRot);
    ScalingMatrix(someMatrix2, fakeItemBox->sizeScaling);
    if (fakeItemBox->sparam != 2) {

        if (!SetMatrix(someMatrix2, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, common_model_fake_itembox);
        CreateModelingMatrix(someMatrix2, fakeItemBox->position, fakeItemBox->angle);
        ScalingMatrix(someMatrix2, fakeItemBox->sizeScaling);

        if (!SetMatrix(someMatrix2, 0)) {
            return;
        }

        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
        // unclear why it is 14.95 degrees instead of 15. Perhaps just a typo when entering the value as a s16.
        if ((fakeItemBox->angle[1] < DEGREES(14.95f)) && (fakeItemBox->angle[1] > 0)) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->angle[1] > (150 * DEGREES(1))) && (fakeItemBox->angle[1] < (165 * DEGREES(1)))) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->angle[1] > (80 * DEGREES(1))) && (fakeItemBox->angle[1] < (95 * DEGREES(1)))) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->angle[1] > (280 * DEGREES(1))) && (fakeItemBox->angle[1] < (295 * DEGREES(1)))) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        }
        gSPDisplayList(gDisplayListHead++, D_0D003090);
    } else {
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
        gDPSetBlendMask(gDisplayListHead++, 0xFF);
        thing = fakeItemBox->someTimer;
        CreateModelingMatrix(someMatrix2, fakeItemBox->position, fakeItemBox->angle);
        if (thing < 10.0f) {
            someMultiplier = 1.0f;
        } else {
            someMultiplier = 1.0f - ((thing - 10.0f) * 0.1f);
        }
        ScalingMatrix(someMatrix2, someMultiplier);
        if (fakeItemBox->someTimer & 1) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
        }
        temp_f2 = 2.0f * thing;
        someVec[0] = 0.0f;
        someVec[1] = temp_f2;
        someVec[2] = thing;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D003158);
        temp_f2_2 = 0.8f * thing;
        temp_f12 = 0.5f * thing;
        someVec[0] = temp_f2_2;
        someVec[1] = 2.3f * thing;
        someVec[2] = temp_f12;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D0031B8);
        temp_f0_2 = -0.5f * thing;
        someVec[0] = temp_f2_2;
        someVec[1] = 1.2f * thing;
        someVec[2] = temp_f0_2;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D003128);
        if (!(fakeItemBox->someTimer & 1)) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
        }
        someVec[0] = 0.0f;
        someVec[1] = 1.8f * thing;
        someVec[2] = -1.0f * thing;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D0031E8);
        temp_f0_3 = -0.8f * thing;
        someVec[0] = temp_f0_3;
        someVec[1] = 0.6f * thing;
        someVec[2] = temp_f0_2;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D003188);
        someVec[0] = temp_f0_3;
        someVec[1] = temp_f2;
        someVec[2] = temp_f12;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D0030F8);
        gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    }
}
