#include <actors.h>
#include <main.h>
#include <macros.h>

/**
 * @brief Renders the item box actor.
 *
 * @param camera
 * @param item_box
 */
void display_itembox(Camera* camera, struct ItemBox* item_box) {
    UNUSED s32 pad[2];
    Vec3f someVec1;
    Vec3f someVec2;
    Vec3s someRot;
    f32 thing;
    UNUSED s32 pad2;
    Mat4 someMatrix1;
    Mat4 someMatrix2;
    UNUSED s32 pad3[4];
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 someMultiplier;

    temp_f0 = CheckDisplayRange(camera->camera_pos, item_box->position, camera->camera_direction[1], 0.0f, gCameraZoom[camera - camera1],
                                      4000000.0f);
    if (!(temp_f0 < 0.0f) && !(600000.0f < temp_f0)) {
        if ((item_box->sparam == 2) && (temp_f0 < 100000.0f)) {
            someRot[0] = 0;
            someRot[1] = item_box->angle[1];
            someRot[2] = 0;
            someVec2[0] = item_box->position[0];
            someVec2[1] = item_box->resetDistance + 2.0f;
            someVec2[2] = item_box->position[2];
            CreateModelingMatrix(someMatrix1, someVec2, someRot);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, D_0D002EE8);
            someRot[1] = item_box->angle[1] * 2;
            someVec2[1] = item_box->position[1];
            CreateModelingMatrix(someMatrix1, someVec2, someRot);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, itemBoxQuestionMarkModel);
        }
        if (item_box->sparam == 5) {
            CreateModelingMatrix(someMatrix1, item_box->position, item_box->angle);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, itemBoxQuestionMarkModel);
        }
        if (item_box->sparam != 3) {
            CreateModelingMatrix(someMatrix1, item_box->position, item_box->angle);

            if (!SetMatrix(someMatrix1, 0)) {
                return;
            }

            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            // unclear why it is 14.95 degrees instead of 15. Perhaps just a typo when entering the value as a s16.
            if ((item_box->angle[1] < DEGREES(14.95f)) && (item_box->angle[1] > 0)) {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else if ((item_box->angle[1] > (150 * DEGREES(1))) && (item_box->angle[1] < (165 * DEGREES(1)))) {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else if ((item_box->angle[1] > (80 * DEGREES(1))) && (item_box->angle[1] < (95 * DEGREES(1)))) {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else if ((item_box->angle[1] > (280 * DEGREES(1))) && (item_box->angle[1] < (295 * DEGREES(1)))) {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else {
                gDPSetBlendMask(gDisplayListHead++, 0xFF);
                gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
            }
            gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
            gSPDisplayList(gDisplayListHead++, D_0D003090);
        } else {
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            thing = item_box->someTimer;
            CreateModelingMatrix(someMatrix1, item_box->position, item_box->angle);
            if (thing < 10.0f) {
                someMultiplier = 1.0f;
            } else {
                someMultiplier = 1.0f - ((thing - 10.0f) * 0.1f);
            }
            ScalingMatrix(someMatrix1, someMultiplier);
            if (item_box->someTimer & 1) {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            }
            temp_f2 = 2.0f * thing;
            someVec1[0] = 0.0f;
            someVec1[1] = temp_f2;
            someVec1[2] = thing;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, D_0D003158);
            temp_f2_2 = 0.8f * thing;
            temp_f12 = 0.5f * thing;
            someVec1[0] = temp_f2_2;
            someVec1[1] = 2.3f * thing;
            someVec1[2] = temp_f12;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, D_0D0031B8);
            temp_f0_2 = -0.5f * thing;
            someVec1[0] = temp_f2_2;
            someVec1[1] = 1.2f * thing;
            someVec1[2] = temp_f0_2;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, D_0D003128);
            if (!(item_box->someTimer & 1)) {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            } else {
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            }
            someVec1[0] = 0.0f;
            someVec1[1] = 1.8f * thing;
            someVec1[2] = -1.0f * thing;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, D_0D0031E8);
            temp_f0_3 = -0.8f * thing;
            someVec1[0] = temp_f0_3;
            someVec1[1] = 0.6f * thing;
            someVec1[2] = temp_f0_2;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, D_0D003188);
            someVec1[0] = temp_f0_3;
            someVec1[1] = temp_f2;
            someVec1[2] = temp_f12;
            TranslateMatrix(someMatrix1, someMatrix2, someVec1);

            if (!SetMatrix(someMatrix2, 0)) {
                return;
            }

            gSPDisplayList(gDisplayListHead++, D_0D0030F8);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
        }
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    }
}
