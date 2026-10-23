#include <actors.h>
#include <code_800029B0.h>
#include <PR/gbi.h>
#include <Main.h>

/**
 * @brief Renders the fake Item box actor.
 *
 * @Param Camera
 * @Param fakeItemBox
 */
void DisplayFakeItembox(Camera* Camera, struct FakeItemBox* fakeItemBox) {
    Vec3s SomeRot;
    UNUSED s32 pad[3];
    Vec3f someVec;
    Mat4 someMatrix2;
    Mat4 someMatrix3;
    UNUSED s32 Pad2[12];
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f2;
    f32 thing;
    f32 temp_f2_2;
    f32 someMultiplier;

    if (CheckDisplayRange(Camera->CameraPos, fakeItemBox->Position, Camera->CameraDirection[1], 2500.0f, CameraZoom[Camera - Camera1],
                                1000000.0f) < 0) {
        OutOfScreen(Camera, (Object*) fakeItemBox);
        return;
    }
    if (((f32) MaximumY + 800.0f) < fakeItemBox->Position[1]) {
        OutOfScreen(Camera, (Object*) fakeItemBox);
        return;
    }
    if (fakeItemBox->Position[1] < ((f32) MinimumY - 800.0f)) {
        OutOfScreen(Camera, (Object*) fakeItemBox);
        return;
    }

    OnScreen(Camera, (Object*) fakeItemBox);
    SomeRot[0] = 0;
    SomeRot[1] = fakeItemBox->Angle[1];
    SomeRot[2] = 0;
    CreateModelingMatrix(someMatrix2, fakeItemBox->Position, SomeRot);
    ScalingMatrix(someMatrix2, fakeItemBox->SizeScaling);
    if (fakeItemBox->Sparam != 2) {

        if (!SetMatrix(someMatrix2, 0)) {
            return;
        }

        gSPDisplayList(DisplayListHead++, CommonModelFakeItembox);
        CreateModelingMatrix(someMatrix2, fakeItemBox->Position, fakeItemBox->Angle);
        ScalingMatrix(someMatrix2, fakeItemBox->SizeScaling);

        if (!SetMatrix(someMatrix2, 0)) {
            return;
        }

        gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
        gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
        // unclear why it is 14.95 degrees instead of 15. Perhaps just a typo when entering the Value as a s16.
        if ((fakeItemBox->Angle[1] < DEGREES(14.95f)) && (fakeItemBox->Angle[1] > 0)) {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->Angle[1] > (150 * DEGREES(1))) && (fakeItemBox->Angle[1] < (165 * DEGREES(1)))) {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->Angle[1] > (80 * DEGREES(1))) && (fakeItemBox->Angle[1] < (95 * DEGREES(1)))) {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->Angle[1] > (280 * DEGREES(1))) && (fakeItemBox->Angle[1] < (295 * DEGREES(1)))) {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
        }
        gSPDisplayList(DisplayListHead++, D_0D003090);
    } else {
        gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
        gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
        gDPSetBlendMask(DisplayListHead++, 0xFF);
        thing = fakeItemBox->SomeTimer;
        CreateModelingMatrix(someMatrix2, fakeItemBox->Position, fakeItemBox->Angle);
        if (thing < 10.0f) {
            someMultiplier = 1.0f;
        } else {
            someMultiplier = 1.0f - ((thing - 10.0f) * 0.1f);
        }
        ScalingMatrix(someMatrix2, someMultiplier);
        if (fakeItemBox->SomeTimer & 1) {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
        }
        temp_f2 = 2.0f * thing;
        someVec[0] = 0.0f;
        someVec[1] = temp_f2;
        someVec[2] = thing;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(DisplayListHead++, D_0D003158);
        temp_f2_2 = 0.8f * thing;
        temp_f12 = 0.5f * thing;
        someVec[0] = temp_f2_2;
        someVec[1] = 2.3f * thing;
        someVec[2] = temp_f12;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(DisplayListHead++, D_0D0031B8);
        temp_f0_2 = -0.5f * thing;
        someVec[0] = temp_f2_2;
        someVec[1] = 1.2f * thing;
        someVec[2] = temp_f0_2;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(DisplayListHead++, D_0D003128);
        if (!(fakeItemBox->SomeTimer & 1)) {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
        }
        someVec[0] = 0.0f;
        someVec[1] = 1.8f * thing;
        someVec[2] = -1.0f * thing;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(DisplayListHead++, D_0D0031E8);
        temp_f0_3 = -0.8f * thing;
        someVec[0] = temp_f0_3;
        someVec[1] = 0.6f * thing;
        someVec[2] = temp_f0_2;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(DisplayListHead++, D_0D003188);
        someVec[0] = temp_f0_3;
        someVec[1] = temp_f2;
        someVec[2] = temp_f12;
        TranslateMatrix(someMatrix2, someMatrix3, someVec);

        if (!SetMatrix(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(DisplayListHead++, D_0D0030F8);
        gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    }
}
