#include <actors.h>
#include <PR/gbi.h>
#include <Main.h>
#include <assets/common_data.h>
#include "courses/all_course_data.h"

/**
 * @brief Renders the tree actor in Mario rawceay.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplayCourseTree1(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 16000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 3.0f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTTreesImport);
        gSPDisplayList(DisplayListHead++, d_course_mario_raceway_dl_tree);
    }
}

/**
 * @brief Renders the tree actor in Yoshi Valley.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplayCourseTree2(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 2.79999995f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTTreesImport);
        gSPDisplayList(DisplayListHead++, d_course_yoshi_valley_dl_tree);
    }
}

/**
 * @brief Renders the tree actor in Royal Raceway.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplayTree3(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 2.79999995f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTTreesImport);
        gSPDisplayList(DisplayListHead++, d_course_royal_raceway_dl_tree);
    }
}

/**
 * @brief Renders the tree actor in Moo Moo Farm.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplayTree4(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 6250000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 600.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 5.0f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTTreesImport);
        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_tree);
    }
}

// have all the properties of the tree
void DisplayTree5(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 2.79999995f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTTreesImport);
        // Why is a TLUT being Used a DL here? That makes no sense
        // Based on the TLUT being Loaded above, this ought to be be another
        // tree related DL, presumably one found in a course other than Moo Moo farm
        //                                 0x0600FC70
        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_mole_tlut);
    }
}

/**
 * @brief Renders the tree actor in Bowser's Castle.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplayTree7(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 2.79999995f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTTreesImport);
        gSPDisplayList(DisplayListHead++, d_course_royal_raceway_dl_castle_tree);
    }
}

/**
 * @brief Renders the bush actor in Bowser's Castle.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplayTree8(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 640000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 2.79999995f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gDPLoadTLUT_pal256(DisplayListHead++, CommonTLUTTreesImport);
        gSPDisplayList(DisplayListHead++, d_course_bowsers_castle_dl_bush);
    }
}

/**
 * @brief Renders the tree actor in Frappe Snowland.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplaySnowtree(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 250000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 2.79999995f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gSPDisplayList(DisplayListHead++, d_course_frappe_snowland_dl_tree);
    }
}

/**
 * @brief Renders the a first Variant of cactus in Kalimari Desert.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplaySaboten1(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 1.0f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_cactus1);
    }
}

/**
 * @brief Renders the a second Variant of cactus in Kalimari Desert.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplaySaboten2(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 1.0f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_cactus2);
    }
}

/**
 * @brief Renders the a third Variant of cactus in Kalimari Desert.
 *
 * @Param Camera
 * @Param arg1
 * @Param Arg2
 */
void DisplaySaboten3(Camera* Camera, Mat4 arg1, Object* Arg2) {
    f32 temp_f0;
    s16 temp_v0 = Arg2->Flag;

    if ((temp_v0 & 0x800) != 0) {
        return;
    }

    temp_f0 =
        CheckDisplayRange(Camera->CameraPos, Arg2->Position, Camera->CameraDirection[1], 0, CameraZoom[Camera - Camera1], 4000000.0f);

    if (temp_f0 < 0.0f) {
        return;
    }

    if (((temp_v0 & 0x400) == 0) && (temp_f0 < 40000.0f)) {
        DisplayShadow(Arg2->Position, Arg2->Angle, 0.80000001f);
    }
    arg1[3][0] = Arg2->Position[0];
    arg1[3][1] = Arg2->Position[1];
    arg1[3][2] = Arg2->Position[2];

    if (SetMatrix(arg1, 0) != 0) {
        gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_cactus3);
    }
}
