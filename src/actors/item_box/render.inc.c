#include <actors.h>
#include <Main.h>
#include <macros.h>

/**
 * @brief Renders the Item box actor.
 *
 * @Param Camera
 * @Param item_box
 */
void DisplayItembox(Camera* Camera, struct ItemBox* item_box) {
    UNUSED s32 pad[2];
    Vec3f someVec1;
    Vec3f someVec2;
    Vec3s SomeRot;
    f32 thing;
    UNUSED s32 Pad2;
    Mat4 someMatrix1;
    Mat4 someMatrix2;
    UNUSED s32 Pad3[4];
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 someMultiplier;

    temp_f0 = CheckDisplayRange(Camera->CameraPos, item_box->Position, Camera->CameraDirection[1], 0.0f, CameraZoom[Camera - Camera1],
                                      4000000.0f);
    if (!(temp_f0 < 0.0f) && !(600000.0f < temp_f0)) {
        if ((item_box->Sparam == 2) && (temp_f0 < 100000.0f)) {
            SomeRot[0] = 0;
            SomeRot[1] = item_box->Angle[1];
            SomeRot[2] = 0;
            someVec2[0] = item_box->Position[0];
            someVec2[1] = item_box->ResetDistance + 2.0f;
            someVec2[2] = item_box->Position[2];
            CreateModelingMatrix(someMatrix1, someVec2, SomeRot);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, D_0D002EE8);
            SomeRot[1] = item_box->Angle[1] * 2;
            someVec2[1] = item_box->Position[1];
            CreateModelingMatrix(someMatrix1, someVec2, SomeRot);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, itemBoxQuestionMarkModel);
        }
        if (item_box->Sparam == 5) {
            CreateModelingMatrix(someMatrix1, item_box->Position, item_box->Angle);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, itemBoxQuestionMarkModel);
        }
        if (item_box->Sparam != 3) {
            CreateModelingMatrix(someMatrix1, item_box->Position, item_box->Angle);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            // unclear why it is 14.95 degrees instead of 15. Perhaps just a typo when entering the Value as a s16.
            if ((item_box->Angle[1] < DEGREES(14.95f)) && (item_box->Angle[1] > 0)) {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else if ((item_box->Angle[1] > (150 * DEGREES(1))) && (item_box->Angle[1] < (165 * DEGREES(1)))) {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else if ((item_box->Angle[1] > (80 * DEGREES(1))) && (item_box->Angle[1] < (95 * DEGREES(1)))) {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else if ((item_box->Angle[1] > (280 * DEGREES(1))) && (item_box->Angle[1] < (295 * DEGREES(1)))) {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else {
                gDPSetBlendMask(DisplayListHead++, 0xFF);
                gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            }
            gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
            gSPDisplayList(DisplayListHead++, D_0D003090);
        } else {
            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            thing = item_box->SomeTimer;
            CreateModelingMatrix(someMatrix1, item_box->Position, item_box->Angle);
            if (thing < 10.0f) {
                someMultiplier = 1.0f;
            } else {
                someMultiplier = 1.0f - ((thing - 10.0f) * 0.1f);
            }
            ScalingMatrix(someMatrix1, someMultiplier);
            if (item_box->SomeTimer & 1) {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            }
            temp_f2 = 2.0f * thing;
            someVec1[0] = 0.0f;
            someVec1[1] = temp_f2;
            someVec1[2] = thing;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, D_0D003158);
            temp_f2_2 = 0.8f * thing;
            temp_f12 = 0.5f * thing;
            someVec1[0] = temp_f2_2;
            someVec1[1] = 2.3f * thing;
            someVec1[2] = temp_f12;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, D_0D0031B8);
            temp_f0_2 = -0.5f * thing;
            someVec1[0] = temp_f2_2;
            someVec1[1] = 1.2f * thing;
            someVec1[2] = temp_f0_2;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, D_0D003128);
            if (!(item_box->SomeTimer & 1)) {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else {
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            }
            someVec1[0] = 0.0f;
            someVec1[1] = 1.8f * thing;
            someVec1[2] = -1.0f * thing;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, D_0D0031E8);
            temp_f0_3 = -0.8f * thing;
            someVec1[0] = temp_f0_3;
            someVec1[1] = 0.6f * thing;
            someVec1[2] = temp_f0_2;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, D_0D003188);
            someVec1[0] = temp_f0_3;
            someVec1[1] = temp_f2;
            someVec1[2] = temp_f12;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(DisplayListHead++, D_0D0030F8);
            gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
        }
        gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    }
}
