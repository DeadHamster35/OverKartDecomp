#include <ultra64.h>
#include <macros.h>
#include <mk64.h>
#include <common_structs.h>
#include <defines.h>
#include <course.h>

#include "render_courses.h"
#include "code_800029B0.h"
#include "main.h"
#include "actors.h"
#include "math_util.h"
#include "memory.h"
#include "code_80281780.h"
#include "collision.h"
#include "skybox_and_splitscreen.h"
#include "courses/all_course_data.h"
#include "courses/all_course_packed.h"
#include "courses/all_course_offsets.h"
#include "OverKart5Hooks.h"

s16 Startfog = 995;
s16 Endfog = 1000;
UNUSED s32 Imageuls = 0;
s32 Imageult = 0;
UNUSED s32 Imageuls2 = 0;
s32 Imageult2 = 0;
s32 Imageuls3 = 0;
s32 Imageult3 = 0;
s16 LightPitch = 0;
s16 LightYaw = 0;
s16 MonitorCounter = 0;

s32 CheckUraPolygon(Camera* Camera) {
    if (Camera->Bump.FlagZX == 0) {
        return 1;
    }
    if ((Camera->Bump.FlagXY == 1) && (Camera->Bump.DistanceXY < 3.0f)) {
        return 1;
    }
    if ((Camera->Bump.FlagYZ == 1) && (Camera->Bump.DistanceYZ < 3.0f)) {
        return 1;
    }
    return 0;
}

void SearchListFile(uintptr_t Addr) {
    s32 segment = SEGMENT_NUMBER2(Addr);
    s32 Offset = SEGMENT_OFFSET(Addr);
    SEARCHLIST* section = (SEARCHLIST*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);

    while (section->Address != 0) {
        if (section->Flag & 0x8000) {
            WallFlag = 1;
        } else {
            WallFlag = 0;
        }
        if (section->Flag & 0x2000) {
            FlatFlag = 1;
        } else {
            FlatFlag = 0;
        }
        if (section->Flag & 0x4000) {
            D_8015F5A4 = 1;
        } else {
            D_8015F5A4 = 0;
        }
        SearchList3(section->Address, section->Status, section->Area);
        section++;
    }
}

extern u32 isFlycam;

void DisplayGroupmap(uintptr_t Addr, Screen* arg1) {
    Player* Player = arg1->Kart;
    Camera* Camera = arg1->Camera;
    u32 segment = SEGMENT_NUMBER2(Addr);
    u32 Offset = SEGMENT_OFFSET(Addr);
    //! @todo Should be Gfx*
    s32* gfx = (s32*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    s16 Direction;
    s16 Index;
    s16 sp1E;
    s16 temp_v0_3;
    CheckDirectionFunc(0, arg1);
    Direction = arg1->CameraDirection;

    if (CameraMode[Camera - Camera1] == 1) {
        sp1E = CheckArea(Camera->Bump.LastZX);
        temp_v0_3 = CheckArea(Player->Bump.LastZX);
        Index = sp1E - temp_v0_3;
        if ((Index < 2) && (Index >= -1)) {
            if (sp1E == 255) {
                if (temp_v0_3 == 255) {
                    Index = arg1->CameraPoint;
                } else if (Player->Bump.DistanceZX > 30.0f) {
                    Index = arg1->CameraPoint;
                } else {
                    Index = temp_v0_3;
                }
            } else if (Camera->Bump.DistanceZX > 30.0f) {
                Index = arg1->CameraPoint;
            } else {
                Index = sp1E;
            }
        } else {

            switch (CourseID) {
                case COURSE_BOWSER_CASTLE:
                    if ((temp_v0_3 >= 0x11) && (temp_v0_3 < 0x18)) {
                        Index = temp_v0_3;
                    } else if ((temp_v0_3 == 255) && (sp1E != 255)) {
                        Index = sp1E;
                    } else if ((temp_v0_3 != 255) && (sp1E == 255)) {
                        Index = temp_v0_3;
                    } else {
                        Index = arg1->CameraPoint;
                    }
                    break;
                case COURSE_CHOCO_MOUNTAIN:
                    if ((temp_v0_3 >= 0xE) && (temp_v0_3 < 0x16)) {
                        Index = temp_v0_3;
                    } else if ((temp_v0_3 == 255) && (sp1E != 255)) {
                        Index = sp1E;
                    } else if ((temp_v0_3 != 255) && (sp1E == 255)) {
                        Index = temp_v0_3;
                    } else {
                        Index = arg1->CameraPoint;
                    }
                    break;
                default:
                    if (temp_v0_3 == 255) {
                        Index = arg1->CameraPoint;
                    } else if (Player->Bump.DistanceZX > 30.0f) {
                        Index = arg1->CameraPoint;
                    } else {
                        Index = temp_v0_3;
                    }
                    break;
            }
        }
    } else {
        Index = CheckArea(Camera->Bump.LastZX);
        if (Camera->Bump.DistanceZX > 30.0f) {
            Index = arg1->CameraPoint;
        } else if (Index == 255) {
            Index = arg1->CameraPoint;
        }
    }

    arg1->CameraPoint = Index;
    if (FlyCamToggle > 0) {
        Index = FlyCamSection;
    } else {
        Index = ((Index - 1) * 4) + Direction;
    }
    gSPDisplayList(DisplayListHead++, gfx[Index]);
}

void DisplayMush(void) {
    // d_course_mario_raceway_packed_dl_1140
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07001140));
}

void DisplayCoursePipe(void) {
    if (ScreenSplitB == SCREEN_MODE_1P) {
        // d_course_mario_raceway_packed_dl_8E8
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070008E8));
    } else {
        // d_course_mario_raceway_packed_dl_2D68
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07002D68));
    }
}

void DisplayMap2(Screen* arg0, s32 PlayerId) {
    UNUSED s32 pad;
    Player* Player = arg0->Kart;
    Mat4 Matrix;
    Vec3f vector;
    u16 pathCounter;
    u16 cameraRot;
    s16 playerDirection;

    InitRDP();
    pathCounter = (u16) arg0->CameraPoint;
    cameraRot = (u16) arg0->Camera->CameraDirection[1];
    playerDirection = arg0->CameraDirection;
    switch (PlayerId) {
        case PLAYER_ONE:
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[PLAYER_ONE]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[PLAYER_ONE]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_TWO:
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[PLAYER_TWO]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[PLAYER_TWO]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_THREE:
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[PLAYER_THREE]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[PLAYER_THREE]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_FOUR:
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXPersp[PLAYER_FOUR]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXLookAt[PLAYER_FOUR]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
    }
    LoadIdentAffineMtx(Matrix);
    SetMatrix(Matrix, 0);
    switch (CourseID) {
        case COURSE_BOWSER_CASTLE:
            if (ScreenSplitA != SCREEN_MODE_1P) {
                return;
            }
            if (pathCounter < 6) {
                return;
            }
            if (pathCounter > 9) {
                return;
            }
            if (pathCounter == 9) {
                if (cameraRot < 0xA000) {
                    return;
                }
                if (cameraRot > 0xE000) {
                    return;
                }
            }
            gSPDisplayList(DisplayListHead++, d_course_bowsers_castle_dl_9228);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            gDPPipeSync(DisplayListHead++);
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            // d_course_banshee_boardwalk_packed_dl_878
            gSPDisplayList(DisplayListHead++, 0x07000878);
            gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
            gDPPipeSync(DisplayListHead++);
            break;
        case COURSE_KOOPA_BEACH:

            gDPPipeSync(DisplayListHead++);
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);

            switch (pathCounter) {
                case 22:
                case 23:
                case 29:
                case 30:
                case 31:
                case 37:
                    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
                    // d_course_koopa_troopa_beach_packed_dl_9E70
                    gSPDisplayList(DisplayListHead++, 0x07009E70);
                    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
                    break;
            }
            vector[0] = 0.0f;
            vector[1] = WaterHeight;
            vector[2] = 0.0f;
            CreateTransAffineMtx(Matrix, vector);
            SetMatrix(Matrix, 0);

            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
            DisplayGroupmap((uintptr_t) d_course_koopa_troopa_beach_dl_list2, arg0);
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
            gDPPipeSync(DisplayListHead++);
            break;
        case COURSE_SHERBET_LAND:

            gDPPipeSync(DisplayListHead++);
            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetTextureFilter(DisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);

            LoadIdentAffineMtx(Matrix);
            SetMatrix(Matrix, 0);
            DisplayGroupmap((uintptr_t) sherbet_land_dls_2, arg0);

            gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
            if ((CheckUraPolygon(arg0->Camera) == 1) && (CheckWaterLevel(Player) < Player->Position[1])) {
                gSPSetGeometryMode(DisplayListHead++, G_ZBUFFER);
                gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
                gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
                // d_course_sherbet_land_packed_dl_2B48
                gSPDisplayList(DisplayListHead++, 0x07002B48);
            }
            gDPPipeSync(DisplayListHead++);
            break;
        case COURSE_RAINBOW_ROAD:
            gDPPipeSync(DisplayListHead++);
            LoadIdentAffineMtx(Matrix);
            SetMatrix(Matrix, 0);
            gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
            DisplayGroupmap((uintptr_t) &d_course_rainbow_road_dl_list, arg0);
            gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
            gDPPipeSync(DisplayListHead++);
            break;
        case COURSE_WARIO_STADIUM:
            gDPPipeSync(DisplayListHead++);
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetTextureFilter(DisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);

            LoadIdentAffineMtx(Matrix);
            SetMatrix(Matrix, 0);

            gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
            gDPSetPrimColor(DisplayListHead++, 0, 0, 0xFF, 0xFF, 0x00, 0xFF);
            // d_course_wario_stadium_packed_dl_EC0
            gSPDisplayList(DisplayListHead++, 0x07000EC0);
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
            gDPPipeSync(DisplayListHead++);
            break;
        case COURSE_DK_JUNGLE:
            gDPPipeSync(DisplayListHead++);
            gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetBlendMask(DisplayListHead++, 0xFF);
            gDPSetTextureFilter(DisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);

            LoadIdentAffineMtx(Matrix);
            SetMatrix(Matrix, 0);

            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);

            if (pathCounter < 17) {
                gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                // d_course_dks_jungle_parkway_packed_dl_3E40
                gSPDisplayList(DisplayListHead++, 0x07003E40);
                // d_course_dks_jungle_parkway_packed_dl_3EB0
                gSPDisplayList(DisplayListHead++, 0x07003EB0);
                if ((pathCounter >= 6) && (pathCounter < 13)) {
                    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                    // d_course_dks_jungle_parkway_packed_dl_3DD0
                    gSPDisplayList(DisplayListHead++, 0x07003DD0);
                }
            } else if ((pathCounter == 21) || (pathCounter == 22)) {
                if (playerDirection == 3) {
                    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(DisplayListHead++, 0x070036A8);
                }
                if ((playerDirection == 1) || (playerDirection == 0)) {
                    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(DisplayListHead++, 0x070036A8);
                } else {
                    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                    // d_course_dks_jungle_parkway_packed_dl_3F30
                    gSPDisplayList(DisplayListHead++, 0x07003F30);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(DisplayListHead++, 0x070036A8);
                }
            } else if (pathCounter == 24) {
                if ((playerDirection == 0) || (playerDirection == 3)) {
                    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(DisplayListHead++, 0x070036A8);
                }
            } else if (pathCounter == 23) {
                if (playerDirection == 3) {
                    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(DisplayListHead++, 0x070036A8);
                } else if (playerDirection == 0) {
                    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(DisplayListHead++, 0x070036A8);
                }
            }
            gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            switch (pathCounter) {
                case 5:
                    if (playerDirection != 3) {
                        // d_course_dks_jungle_parkway_packed_dl_3DD0
                        gSPDisplayList(DisplayListHead++, 0x07003DD0);
                    }
                    break;
                case 17:
                    switch (playerDirection) {
                        case 0:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(DisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(DisplayListHead++, 0x07003EB0);
                            break;
                        case 1:
                            // d_course_dks_jungle_parkway_packed_dl_3DD0
                            gSPDisplayList(DisplayListHead++, 0x07003DD0);
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(DisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(DisplayListHead++, 0x07003EB0);
                            break;
                        case 2:
                            // d_course_dks_jungle_parkway_packed_dl_
                            gSPDisplayList(DisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(DisplayListHead++, 0x07003EB0);
                            // d_course_dks_jungle_parkway_packed_dl_3F30
                            gSPDisplayList(DisplayListHead++, 0x07003F30);
                            break;
                        case 3:
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(DisplayListHead++, 0x07003EB0);
                            // d_course_dks_jungle_parkway_packed_dl_3F30
                            gSPDisplayList(DisplayListHead++, 0x07003F30);
                            break;
                    }
                    break;
                case 18:
                    switch (playerDirection) {
                        case 0:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(DisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(DisplayListHead++, 0x07003EB0);
                            break;
                        case 1:
                            // d_course_dks_jungle_parkway_packed_dl_3DD0
                            gSPDisplayList(DisplayListHead++, 0x07003DD0);
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(DisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(DisplayListHead++, 0x07003EB0);
                            break;
                        case 2:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(DisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(DisplayListHead++, 0x07003EB0);
                            break;
                    }
                    break;
                case 21:
                    if ((playerDirection == 0) || (playerDirection == 1)) {
                        // d_course_dks_jungle_parkway_packed_dl_3E40
                        gSPDisplayList(DisplayListHead++, 0x07003E40);
                        // d_course_dks_jungle_parkway_packed_dl_3EB0
                        gSPDisplayList(DisplayListHead++, 0x07003EB0);
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(DisplayListHead++, 0x07003F30);
                    } else {
                        // d_course_dks_jungle_parkway_packed_dl_3EB0
                        gSPDisplayList(DisplayListHead++, 0x07003EB0);
                    }
                    break;
                case 22:
                    if (playerDirection == 0) {
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(DisplayListHead++, 0x07003F30);
                    }
                    break;
                case 23:
                    if (playerDirection != 1) {
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(DisplayListHead++, 0x07003F30);
                    }
                    break;
            }
            gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
            gDPPipeSync(DisplayListHead++);
            break;
    }
    XLUDisplay(arg0);
}

void DisplayKT1(Screen* arg0) {
    UNUSED s32 pad;
    u16 sp22 = arg0->CameraPoint;
    u16 playerDirection = arg0->CameraDirection;

    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_mario_raceway_packed_dl_3050
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07003050));
    }

    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(DisplayListHead++);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    switch (sp22) {
        case 1:
        case 2:
        case 17:
            if ((playerDirection == 2) || (playerDirection == 1)) {
                DisplayCoursePipe();
            }
            break;
        case 3:
            if (playerDirection != 0) {
                DisplayCoursePipe();
            }
            break;
        case 4:
            if (playerDirection == 0) {
                DisplayMush();
            } else {
                if (playerDirection == 1) {
                    DisplayMush();
                }
                DisplayCoursePipe();
            }
            break;
        case 5:
        case 6:
            if ((playerDirection == 2) || (playerDirection == 3)) {
                DisplayCoursePipe();
            } else {
                DisplayMush();
            }
            break;
        case 7:
            DisplayMush();
            if ((playerDirection == 2) || (playerDirection == 3)) {
                DisplayCoursePipe();
            }
            break;
        case 8:
        case 9:
            if (playerDirection != 1) {
                DisplayCoursePipe();
            }
            /* fallthrough */
        case 10:
            if (playerDirection != 2) {
                DisplayMush();
            }
            break;
        case 11:
            if (playerDirection == 0) {
                DisplayCoursePipe();
                DisplayMush();
            } else if (playerDirection == 3) {
                DisplayCoursePipe();
            }
            break;
        case 12:
            if ((playerDirection == 0) || (playerDirection == 3)) {
                DisplayCoursePipe();
            }
            break;
        case 13:
        case 14:
            if (playerDirection != 1) {
                case 15:
                case 16:
                    DisplayCoursePipe();
            }
            break;
    }
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_mario_raceway_packed_dl_3508
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07003508));
    // d_course_mario_raceway_packed_dl_3240
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07003240));
    // d_course_mario_raceway_packed_dl_14A0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070014A0));

    DisplayGroupmap((uintptr_t) mario_raceway_dls, arg0);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_mario_raceway_packed_dl_450
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000450));
    // d_course_mario_raceway_packed_dl_240
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000240));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_mario_raceway_packed_dl_E0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070000E0));
    // d_course_mario_raceway_packed_dl_160
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000160));
}

void DisplayKT2(Screen* arg0) {
    UNUSED s32 pad[13];

    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_choco_mountain_packed_dl_4608
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07004608));
    }
    gDPSetCycleType(DisplayListHead++, G_CYC_2CYCLE);
    gDPSetFogColor(DisplayListHead++, FogRed, FogBlue, FogGreen, 0xFF);
    gSPFogPosition(DisplayListHead++, Startfog, Endfog);

    gDPPipeSync(DisplayListHead++);
    gSPSetGeometryMode(DisplayListHead++, G_FOG);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATERGB, G_CC_PASS2);

    gDPSetRenderMode(DisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    // d_course_choco_mountain_packed_dl_5A70
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07005A70));
    // d_course_choco_mountain_packed_dl_828
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000828));
    // d_course_choco_mountain_packed_dl_8E0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070008E0));
    // d_course_choco_mountain_packed_dl_5868
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07005868));
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    DisplayGroupmap((uintptr_t) choco_mountain_dls, arg0);

    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    gDPSetRenderMode(DisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_PASS2);
    // d_course_choco_mountain_packed_dl_448
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000448));
    // d_course_choco_mountain_packed_dl_5D8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070005D8));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_choco_mountain_packed_dl_718
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000718));
    gSPClearGeometryMode(DisplayListHead++, G_FOG);
    gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);
    gDPPipeSync(DisplayListHead++);
}

void DisplayKT3(Screen* arg0) {

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_bowsers_castle_packed_dl_6A80
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07006A80));
    }

    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    Imageult++;
    if (Imageult > 255) {
        Imageult = 0;
    }
    DisplayGroupmap((uintptr_t) bowsers_castle_dls, arg0);

    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_bowsers_castle_packed_dl_248
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000248));
}

void DisplayKT4(Screen* arg0) {
    Camera* Camera = arg0->Camera;
    Mat4 spCC;
    UNUSED s32 pad[6];
    Vec3f spA8;
    UNUSED s32 Pad2[6];

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    // d_course_banshee_boardwalk_packed_dl_7228
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07007228));

    gSPFogPosition(DisplayListHead++, Startfog, Endfog);

    gDPPipeSync(DisplayListHead++);

    gSPClearGeometryMode(DisplayListHead++,
                         G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    // d_course_banshee_boardwalk_packed_dl_5CD0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07005CD0));
    // d_course_banshee_boardwalk_packed_dl_4E60
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07004E60));
    // d_course_banshee_boardwalk_packed_dl_69B0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070069B0));

    DisplayGroupmap((uintptr_t) banshee_boardwalk_dls, arg0);

    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(DisplayListHead++, G_SHADE | G_SHADING_SMOOTH);
    // d_course_banshee_boardwalk_packed_dl_580
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000580));
    // d_course_banshee_boardwalk_packed_dl_60
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000060));
    // d_course_banshee_boardwalk_packed_dl_540
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000540));

    if (Camera->CameraPos[1] < -20.0f) {
        // d_course_banshee_boardwalk_packed_dl_6310
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07006310));
    }
    spA8[0] = Camera->CameraPos[0];
    spA8[1] = -82.0f;
    spA8[2] = Camera->CameraPos[2];
    CreateTransAffineMtx(spCC, spA8);
    SetMatrix(spCC, 0);

    gSPDisplayList(DisplayListHead++, d_course_banshee_boardwalk_dl_B278);
    gDPPipeSync(DisplayListHead++);
}

void DisplayKT5(Screen* arg0) {

    gDPPipeSync(DisplayListHead++);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    DisplayGroupmap((uintptr_t) d_course_yoshi_valley_dl_list, arg0);
    gDPPipeSync(DisplayListHead++);
}

void DisplayKT6(Screen* arg0) {

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_frappe_snowland_packed_dl_65E0
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070065E0));
    }

    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    DisplayGroupmap((uintptr_t) d_course_frappe_snowland_dl_list, arg0);
}

void DisplayKT7(Screen* arg0) {

    gDPPipeSync(DisplayListHead++);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_koopa_troopa_beach_packed_dl_9CC0
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07009CC0));
    }
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_koopa_troopa_beach_packed_dl_9688
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07009688));
    DisplayGroupmap((uintptr_t) d_course_koopa_troopa_beach_dl_list1, arg0);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_koopa_troopa_beach_packed_dl_2C0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070002C0));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    gDPPipeSync(DisplayListHead++);
}

void DisplayKT8(Screen* arg0) {

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_royal_raceway_packed_dl_B030
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x0700B030));
    }
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_royal_raceway_packed_dl_A648
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x0700A648));

    DisplayGroupmap((uintptr_t) royal_raceway_dls, arg0);

    // d_course_royal_raceway_packed_dl_11A8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070011A8));
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_royal_raceway_packed_dl_8A0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070008A0));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void DisplayKT9(Screen* arg0) {

    UNUSED s32 pad;
    u16 sp22 = (u16) arg0->CameraPoint;
    s16 prevFrame;

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_luigi_raceway_packed_dl_9EC0
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07009EC0));
    }

    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    DisplayGroupmap((uintptr_t) luigi_raceway_dls, arg0);

    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_luigi_raceway_packed_dl_E0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070000E0));
    // d_course_luigi_raceway_packed_dl_68
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000068));

    D_800DC5DC = 88;
    D_800DC5E0 = 72;

    // Render only the first Player Camera onto the television billboard. Screen agnostic screens of other players).
    if ((ScreenSplitA == SCREEN_MODE_1P) && (sp22 >= 10) && (sp22 < 17)) {

        prevFrame = (s16) DispFrame - 1;

        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }
        MonitorCounter++;
        if (MonitorCounter >= 6) {
            MonitorCounter = 0;
        }
        /**
         * The jumbo television screen is split into six sections each section is copied one at a Time.
         * This is done to fit within the n64's Texture Size requirements; 64x32
         */
        switch (MonitorCounter) {
            case 0:
                GetFramebuffer(D_800DC5DC, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xF800));
                break;
            case 1:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x10800));
                break;
            case 2:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x11800));
                break;
            case 3:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x12800));
                break;
            case 4:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x13800));
                break;
            case 5:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x14800));
                break;
        }
    }
}

// Missing {} around if statements necessary for matching.
void DisplayKT10(Screen* arg0) {
    UNUSED s32 pad[13];
    s16 temp_s0 = arg0->CameraPoint;
    s16 playerDirection = arg0->CameraDirection;

    MakeLight(D_800DC610, LightYaw, 0, 1);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_moo_moo_farm_packed_dl_4DF8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07004DF8));
    // d_course_moo_moo_farm_packed_dl_5640
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07005640));
    gSPFogPosition(DisplayListHead++, Startfog, Endfog);

    DisplayGroupmap((uintptr_t) moo_moo_farm_dls, arg0);

    if ((temp_s0 < 14) && (temp_s0 > 10)) {
        if ((playerDirection == 2) || (playerDirection == 3) || (playerDirection == 1))
            //
            gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_13FF8);

    } else if (temp_s0 < 16) {
        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_13FF8);
    } else if (temp_s0 < 19) {
        if (playerDirection != 2)
            gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_13FF8);

    } else if (temp_s0 < 20) {
        if (playerDirection == 0)
            gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_13FF8);
    }
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    if ((temp_s0 >= 16) && (temp_s0 < 24)) {
        if ((playerDirection == 2) || (playerDirection == 3))
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07005410));

    } else if (temp_s0 < 9) {
        if (playerDirection == 2)
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07005410));
    }
    if (temp_s0 < 4) {
        if (playerDirection != 0)
            gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_14060);

    } else if (temp_s0 < 8) {
        if (playerDirection == 2)
            gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_14060);

    } else if (temp_s0 >= 22) {
        gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_14060);
    } else if (temp_s0 >= 18) {
        if ((playerDirection == 0) || (playerDirection == 3))
            gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_14060);
    }
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_moo_moo_farm_packed_dl_10C0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070010C0));
}

void DisplayKT11(Screen* arg0) {
    UNUSED s32 pad[13];

    MakeLight(D_800DC610, LightYaw, 0, 1);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gDPSetFogColor(DisplayListHead++, FogRed, FogBlue, FogGreen, 0xFF);
    gDPSetCycleType(DisplayListHead++, G_CYC_2CYCLE);
    gSPFogPosition(DisplayListHead++, Startfog, Endfog);
    gSPSetGeometryMode(DisplayListHead++, G_FOG);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEI, G_CC_PASS2);
    gDPSetRenderMode(DisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);

    DisplayGroupmap((uintptr_t) d_course_toads_turnpike_dl_list, arg0);

    gDPSetRenderMode(DisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_PASS2);
    // d_course_toads_turnpike_packed_dl_0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000000));
    // d_course_toads_turnpike_packed_dl_68
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000068));
    // d_course_toads_turnpike_packed_dl_D8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070000D8));
    gSPClearGeometryMode(DisplayListHead++, G_FOG);
    gDPSetCycleType(DisplayListHead++, G_CYC_1CYCLE);
}

void DisplayKT12(Screen* arg0) {

    MakeLight(D_800DC610, LightYaw, 0, 1);

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_kalimari_desert_packed_dl_71C8
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070071C8));
    }

    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    DisplayGroupmap((uintptr_t) kalimari_desert_dls, arg0);
    // d_course_kalimari_desert_packed_dl_1ED8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07001ED8));
    // d_course_kalimari_desert_packed_dl_1B18
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07001B18));
    // d_course_kalimari_desert_packed_dl_8330
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07008330));
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_kalimari_desert_packed_dl_998
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000998));
    // d_course_kalimari_desert_packed_dl_270
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000270));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void DisplayKT13(Screen* arg0) {

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    DisplayGroupmap((uintptr_t) sherbet_land_dls, arg0);
}

void DisplayKT14(UNUSED Screen* arg0) {

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

void DisplayKT15(Screen* arg0) {
    s16 prevFrame;

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {

        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_wario_stadium_packed_dl_A0C8
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x0700A0C8));
    }
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    DisplayGroupmap((uintptr_t) wario_stadium_dls, arg0);

    // d_course_wario_stadium_packed_dl_A228
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x0700A228));
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_wario_stadium_packed_dl_A88
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000A88));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);

    D_800DC5DC = 88;
    D_800DC5E0 = 72;
    if (ScreenSplitA == SCREEN_MODE_1P) {
        prevFrame = (s16) DispFrame - 1;
        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }
        MonitorCounter++;
        if (MonitorCounter > 5) {
            MonitorCounter = 0;
        }
        switch (MonitorCounter) {
            case 0:
                GetFramebuffer(D_800DC5DC, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x8800));
                break;
            case 1:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x9800));
                break;
            case 2:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xA800));
                break;
            case 3:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xB800));
                break;
            case 4:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xC800));
                break;
            case 5:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xD800));
                break;
        }
    }
}

void DisplayKT16(UNUSED Screen* arg0) {

    MakeLight(D_800DC610, LightYaw, 0, 1);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    // d_course_block_fort_packed_dl_15C0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070015C0));
}

void DisplayKT17(UNUSED Screen* arg0) {
    MakeLight(D_800DC610, LightYaw, 0, 1);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    // d_course_skyscraper_packed_dl_FE8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000FE8));
    // d_course_skyscraper_packed_dl_C60
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000C60));
    // d_course_skyscraper_packed_dl_B70
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000B70));
    // d_course_skyscraper_packed_dl_6B8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070006B8));
    // d_course_skyscraper_packed_dl_570
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000570));
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_10C8
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070010C8));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_258
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000258));
}

void DisplayKT18(UNUSED Screen* arg0) {

    MakeLight(D_800DC610, LightYaw, 0, 1);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK);
    // d_course_double_deck_packed_dl_738
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000738));
    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void DisplayKT19(Screen* arg0) {

    MakeLight(D_800DC610, LightYaw, 0, 1);
    MakeLight(&D_800DC610[1], LightYaw, LightPitch, 1);

    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_CULL_BACK | G_LIGHTING);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_dks_jungle_parkway_packed_dl_92D8
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x070092D8));
    }

    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    DisplayGroupmap((uintptr_t) d_course_dks_jungle_parkway_unknown_dl_list, arg0);

    gSPSetGeometryMode(DisplayListHead++, G_CULL_BACK);
}

void DisplayKT20(Screen* arg0) {

    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    MakeLight(D_800DC610, LightYaw, 0, 1);
    gSPTexture(DisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(DisplayListHead++, G_SHADING_SMOOTH);

    if (CheckUraPolygon(arg0->Camera) == 1) {
        gDPSetCombineMode(DisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_big_donut_packed_dl_DE8
        gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000DE8));
    }
    // d_course_big_donut_packed_dl_450
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000450));
    // d_course_big_donut_packed_dl_AC0
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000AC0));
    // d_course_big_donut_packed_dl_D20
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000D20));
    // d_course_big_donut_packed_dl_230
    gSPDisplayList(DisplayListHead++, ((uintptr_t) 0x07000230));
}

/**
 * @brief The game has two versions of each course. One for normal gameplay and another for credits.
 * The credits version is not split into segments so that the game Camera can fly around the whole course.
 *
 */
void DisplayAllMap(void) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            if (HotSwapID == 0) {
                gSPDisplayList(DisplayListHead++, d_course_mario_raceway_dl_9348);
            }
            break;
        case COURSE_CHOCO_MOUNTAIN:
            gSPDisplayList(DisplayListHead++, d_course_choco_mountain_dl_71B8);
            break;
        case COURSE_BOWSER_CASTLE:
            gSPDisplayList(DisplayListHead++, d_course_bowsers_castle_dl_9148);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            gSPDisplayList(DisplayListHead++, d_course_banshee_boardwalk_dl_B308);
            break;
        case COURSE_YOSHI_VALLEY:
            gSPDisplayList(DisplayListHead++, d_course_yoshi_valley_dl_18020);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            gSPDisplayList(DisplayListHead++, d_course_frappe_snowland_dl_76A0);
            break;
        case COURSE_KOOPA_BEACH:
            gSPDisplayList(DisplayListHead++, d_course_koopa_troopa_beach_dl_18D68);
            break;
        case COURSE_ROYAL_RACEWAY:
            gSPDisplayList(DisplayListHead++, d_course_royal_raceway_dl_D8E8);
            break;
        case COURSE_LUIGI_RACEWAY:
            gSPDisplayList(DisplayListHead++, d_course_luigi_raceway_dl_FD40);
            break;
        case COURSE_MOO_MOO_FARM:
            gSPDisplayList(DisplayListHead++, d_course_moo_moo_farm_dl_14088);
            break;
        case COURSE_TOADS_TURNPIKE:
            gSPDisplayList(DisplayListHead++, d_course_toads_turnpike_dl_23930);
            break;
        case COURSE_KALAMARI_DESERT:
            gSPDisplayList(DisplayListHead++, d_course_kalimari_desert_dl_22E00);
            break;
        case COURSE_SHERBET_LAND:
            gSPDisplayList(DisplayListHead++, d_course_sherbet_land_dl_9AE8);
            break;
        case COURSE_RAINBOW_ROAD:
            gSPDisplayList(DisplayListHead++, d_course_rainbow_road_dl_16220);
            break;
        case COURSE_WARIO_STADIUM:
            gSPDisplayList(DisplayListHead++, d_course_wario_stadium_dl_CA78);
            break;
        case COURSE_BLOCK_FORT:
            if (HotSwapID == 0) {
                gSPDisplayList(DisplayListHead++, d_course_sherbet_land_dl_0);
            }
            break;
        case COURSE_SKYSCRAPER:
            gSPDisplayList(DisplayListHead++, d_course_sherbet_land_dl_0);
            break;
        case COURSE_DOUBLE_DECK:
            gSPDisplayList(DisplayListHead++, d_course_sherbet_land_dl_0);
            break;
        case COURSE_DK_JUNGLE:
            gSPDisplayList(DisplayListHead++, d_course_dks_jungle_parkway_dl_13C30);
            break;
        case COURSE_BIG_DONUT:
            gSPDisplayList(DisplayListHead++, d_course_sherbet_land_dl_0);
            break;
    }
#else

#endif
}

void DisplayMap(Screen* arg0) {

    MakeLight(D_800DC610, LightYaw, 0, 1);
    if (DisplayFlag) {
        DisplayAllMap();
        return;
    }

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            DisplayKT1Hook(arg0);
            break;
        case COURSE_CHOCO_MOUNTAIN:
            DisplayKT2(arg0);
            break;
        case COURSE_BOWSER_CASTLE:
            DisplayKT3(arg0);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            DisplayKT4(arg0);
            break;
        case COURSE_YOSHI_VALLEY:
            DisplayKT5(arg0);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            DisplayKT6(arg0);
            break;
        case COURSE_KOOPA_BEACH:
            DisplayKT7(arg0);
            break;
        case COURSE_ROYAL_RACEWAY:
            DisplayKT8(arg0);
            break;
        case COURSE_LUIGI_RACEWAY:
            DisplayKT9(arg0);
            break;
        case COURSE_MOO_MOO_FARM:
            DisplayKT10(arg0);
            break;
        case COURSE_TOADS_TURNPIKE:
            DisplayKT11(arg0);
            break;
        case COURSE_KALAMARI_DESERT:
            DisplayKT12(arg0);
            break;
        case COURSE_SHERBET_LAND:
            DisplayKT13(arg0);
            break;
        case COURSE_RAINBOW_ROAD:
            DisplayKT14(arg0);
            break;
        case COURSE_WARIO_STADIUM:
            DisplayKT15(arg0);
            break;
        case COURSE_BLOCK_FORT:
            DisplayKT16Hook(arg0);
            break;
        case COURSE_SKYSCRAPER:
            DisplayKT17(arg0);
            break;
        case COURSE_DOUBLE_DECK:
            DisplayKT18(arg0);
            break;
        case COURSE_DK_JUNGLE:
            DisplayKT19(arg0);
            break;
        case COURSE_BIG_DONUT:
            DisplayKT20(arg0);
            break;
    }
#else

#endif
}

void InitialBumpKart(s32 playerIndex) {
    Player* Player = &GamePlayers[playerIndex];
    InitialBump(&Player->Bump);
    Player->TireFR.LastAxis = 0;
    Player->TireFL.LastAxis = 0;
    Player->TireRR.LastAxis = 0;
    Player->TireRL.LastAxis = 0;

    Player->TireFR.LastPointer = 0x1388;
    Player->TireFL.LastPointer = 0x1388;
    Player->TireRR.LastPointer = 0x1388;
    Player->TireRL.LastPointer = 0x1388;
}

void MakeCollision(void) {
    MaximumX += 20;
    MaximumZ += 20;
    MinimumX += -20;
    MinimumZ += -20;
    MinimumY += -20;
    MakeCollisionTable();
    FreeMemoryPointer += ALIGN16(CollisionPointer * sizeof(u16));
    CheckCourseMemoryHighWater();
}

UNUSED void func_80295D50(s16 arg0, s16 arg1) {
    Maxlimmity = arg1;
    Minlimmity = arg0;
}

void ResetLimmity(void) {
    Maxlimmity = 3000;
    Minlimmity = -3000;
}

/**
 * @brief Also sets vertex colours for the course vtx data
 *
 */
void InitialMap(void) {
    SimpleObjectCount = 0;

    MinimumX = 0;
    MinimumY = 0;
    MinimumZ = 0;

    MaximumX = 0;
    MaximumY = 0;
    MaximumZ = 0;

    WallFlag = 0;
    FlatFlag = 0;
    ResetLimmity();
    Facenumber = 0;
    CourseFaceMaxIndex = 0;
    FaceBuffer = (FaceStruct*) FACE_BUFFER_START;
    SkyToggle = 0;
    FadeOutCounter2 = 0;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            // d_course_mario_raceway_packed_dl_1140
            SearchListHook(0x07001140);
            if (ScreenSplitB == SCREEN_MODE_1P) {
                // d_course_mario_raceway_packed_dl_8E8
                SearchListHook(0x070008E8);
            } else {
                // d_course_mario_raceway_packed_dl_2D68
                SearchListHook(0x07002D68);
            }
            SearchListFileHook((unsigned int) d_course_mario_raceway_addr);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            SkyToggle = 1;
            FogRed = 255;
            FogBlue = 255;
            FogGreen = 255;
            Startfog = 0x3E3;
            Endfog = 0x3E8;
            LightYaw = 0x71C;
            LightPitch = 0xE38;

            // Spawn guardrail only for CC_50 and Time trials.
            if ((RaceClass != CC_50) && (GameMode != TIME_TRIALS)) {
                // d_course_choco_mountain_packed_dl_0
                KillDisplayList((uintptr_t) 0x07000000);
                // d_course_choco_mountain_packed_dl_98
                KillDisplayList((uintptr_t) 0x07000098);
                // d_course_choco_mountain_packed_dl_178
                KillDisplayList((uintptr_t) 0x07000178);
                // d_course_choco_mountain_packed_dl_280
                KillDisplayList((uintptr_t) 0x07000280);
                // d_course_choco_mountain_packed_dl_340
                KillDisplayList((uintptr_t) 0x07000340);
                // d_course_choco_mountain_packed_dl_3C8
                KillDisplayList((uintptr_t) 0x070003C8);
            }
            SearchListFile((uintptr_t) &d_course_choco_mountain_addr);
            // D_8015F590 is only Used here, so this seems meaningless
            InitialLight(DEGREES(50), DEGREES(70), D_8015F590);
            MakeCollision();
            WaterHeight = -80.0f;
            break;
        case COURSE_BOWSER_CASTLE:
            SearchListFile((uintptr_t) d_course_bowsers_castle_addr);
            MakeCollision();
            // d_course_bowsers_castle_packed_dl_1350
            MakeWaterVertex((uintptr_t) 0x07001350, 0x32, 0, 0, 0);
            WaterHeight = -50.0f;
            break;
        case COURSE_BANSHEE_BOARDWALK:
            SkyToggle = 1;
            FogRed = 0;
            FogBlue = 0;
            FogGreen = 0;
            SearchListFile((uintptr_t) d_course_banshee_boardwalk_track_sections);
            MakeCollision();
            // d_course_banshee_boardwalk_packed_dl_878
            MakeWaterVertex((uintptr_t) 0x07000878, 128, 0, 0, 0);
            WaterHeight = -80.0f;
            break;
        case COURSE_YOSHI_VALLEY:
            MakeLight(&d_course_yoshi_valley_lights4, -0x38F0, 0x1C70, 1);
            SearchListFile((uintptr_t) d_course_yoshi_valley_addr);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_FRAPPE_SNOWLAND:
            SearchListFile((uintptr_t) d_course_frappe_snowland_addr);
            MakeCollision();
            WaterHeight = -50.0f;
            break;
        case COURSE_KOOPA_BEACH:
            SearchListFile((uintptr_t) d_course_koopa_troopa_beach_addr);
            MakeCollision();
            // d_course_koopa_troopa_beach_packed_dl_ADE0
            MakeWaterVertex((uintptr_t) 0x0700ADE0, 150, 255, 255, 255);
            // d_course_koopa_troopa_beach_packed_dl_A540
            MakeWaterVertex((uintptr_t) 0x0700A540, 150, 255, 255, 255);
            // d_course_koopa_troopa_beach_packed_dl_9E70
            MakeWaterVertex((uintptr_t) 0x07009E70, 150, 255, 255, 255);
            // d_course_koopa_troopa_beach_packed_dl_358
            MakeWaterVertex((uintptr_t) 0x07000358, 150, 255, 255, 255);
            break;
        case COURSE_ROYAL_RACEWAY:
            SearchListFile((uintptr_t) d_course_royal_raceway_addr);
            MakeCollision();
            WaterHeight = -60.0f;
            break;
        case COURSE_LUIGI_RACEWAY:
            SearchListFile((uintptr_t) d_course_luigi_raceway_addr);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_MOO_MOO_FARM:
            SearchListFile((uintptr_t) d_course_moo_moo_farm_addr);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_TOADS_TURNPIKE:
            FogRed = 43;
            FogBlue = 13;
            FogGreen = 4;
            Startfog = 993;
            Endfog = 1000;
            SearchListFile((uintptr_t) d_course_toads_turnpike_addr);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_KALAMARI_DESERT:
            SearchListFile((uintptr_t) d_course_kalimari_desert_addr);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_SHERBET_LAND:
            SearchListFile((uintptr_t) d_course_sherbet_land_addr);
            MakeCollision();
            WaterHeight = -18.0f;
            // d_course_sherbet_land_packed_dl_1EB8
            MakeWaterVertex((uintptr_t) 0x07001EB8, 180, 255, 255, 255);
            // d_course_sherbet_land_packed_dl_2308
            MakeWaterVertex((uintptr_t) 0x07002308, 150, 255, 255, 255);
            break;
        case COURSE_RAINBOW_ROAD:
            FadeOutCounter2 = 1;
            SearchListFile((uintptr_t) d_course_rainbow_road_addr);
            MakeCollision();
            WaterHeight = 0.0f;
            // d_course_rainbow_road_packed_dl_2068
            MakeWaterVertex((uintptr_t) 0x07002068, 150, 255, 255, 255);
            // d_course_rainbow_road_packed_dl_1E18
            MakeWaterVertex((uintptr_t) 0x07001E18, 150, 255, 255, 255);
            // d_course_rainbow_road_packed_dl_1318
            MakeWaterVertex((uintptr_t) 0x07001318, 255, 255, 255, 0);
            if (GameState != CREDITS_SEQUENCE) {
                // d_course_rainbow_road_packed_dl_1FB8
                MakeWaterVertex((uintptr_t) 0x07001FB8, 150, 255, 255, 255);
            }
            break;
        case COURSE_WARIO_STADIUM:
            SearchListFile((uintptr_t) d_course_wario_stadium_addr);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            // d_course_wario_stadium_packed_dl_C50
            MakeWaterVertex((uintptr_t) 0x07000C50, 100, 255, 255, 255);
            // d_course_wario_stadium_packed_dl_BD8
            MakeWaterVertex((uintptr_t) 0x07000BD8, 100, 255, 255, 255);
            // d_course_wario_stadium_packed_dl_B60
            MakeWaterVertex((uintptr_t) 0x07000B60, 100, 255, 255, 255);
            // d_course_wario_stadium_packed_dl_AE8
            MakeWaterVertex((uintptr_t) 0x07000AE8, 100, 255, 255, 255);
            // d_course_wario_stadium_packed_dl_CC8
            MakeWaterVertex((uintptr_t) 0x07000CC8, 100, 255, 255, 255);
            // d_course_wario_stadium_packed_dl_D50
            MakeWaterVertex((uintptr_t) 0x07000D50, 100, 255, 255, 255);
            // d_course_wario_stadium_packed_dl_DD0
            MakeWaterVertex((uintptr_t) 0x07000DD0, 100, 255, 255, 255);
            // d_course_wario_stadium_packed_dl_E48
            MakeWaterVertex((uintptr_t) 0x07000E48, 100, 255, 255, 255);
            break;
        case COURSE_BLOCK_FORT:
            // d_course_block_fort_packed_dl_15C0
            SearchList2Hook(0x070015C0, 1);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_SKYSCRAPER:
            // d_course_skyscraper_packed_dl_1110
            SearchList2((Gfx*) 0x07001110, 1);
            // d_course_skyscraper_packed_dl_258
            SearchList2((Gfx*) 0x07000258, 1);
            MakeCollision();

            WaterHeight = -480.0f;
            break;
        case COURSE_DOUBLE_DECK:
            // d_course_double_deck_packed_dl_738
            SearchList2((Gfx*) 0x07000738, 1);
            MakeCollision();
            WaterHeight = MinimumY - 10.0f;
            break;
        case COURSE_DK_JUNGLE:
            SearchListFile((uintptr_t) d_course_dks_jungle_parkway_addr);
            MakeCollision();
            WaterHeight = -475.0f;
            // d_course_dks_jungle_parkway_packed_dl_3FA8
            MakeWaterVertex((uintptr_t) 0x07003FA8, 120, 255, 255, 255);
            break;
        case COURSE_BIG_DONUT:
            // d_course_big_donut_packed_dl_1018
            SearchList2((Gfx*) 0x07001018, 6);
            // d_course_big_donut_packed_dl_450
            SearchList2((Gfx*) 0x07000450, 6);
            // d_course_big_donut_packed_dl_AC0
            SearchList2((Gfx*) 0x07000AC0, 6);
            // d_course_big_donut_packed_dl_B58
            SearchList2((Gfx*) 0x07000B58, 6);
            // d_course_big_donut_packed_dl_230
            SearchList2((Gfx*) 0x07000230, 6);
            MakeCollision();
            WaterHeight = 100.0f;
            break;
    }
#else

#endif
}

void MapStrategy(void) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_KOOPA_BEACH:
            // clang-format off
            if (WaterVelocity < 0.0f) {
                if (WaterHeight < -20.0f) { WaterVelocity *= -1.0f; }
            } else {
                if (WaterHeight > 0.0f) { WaterVelocity *= -1.0f; }
            }
            // clang-format on
            WaterHeight += WaterVelocity;

            Imageult += 9;
            if (Imageult > 255) {
                Imageult = 0;
            }
            Imageult2 += 3;
            if (Imageult2 > 255) {
                Imageult2 = 0;
            }
            // waterfall animation
            // d_course_koopa_troopa_beach_packed_dl_9D58
            ScrollMapImage((uintptr_t) 0x07009D58, 0, Imageult);
            // d_course_koopa_troopa_beach_packed_dl_9CD0
            ScrollMapImage((uintptr_t) 0x07009CD0, 0, Imageult2);
            Imageult3 = MakeRandomLimmit(300) / 40;
            if (Imageuls3 < 0) {
                Imageuls3 = MakeRandomLimmit(300) / 40;
            } else {
                Imageuls3 = -(MakeRandomLimmit(300) / 40);
            }
            // Waterfall bubbling effect? (Unused)
            // d_course_koopa_troopa_beach_packed_dl_2E8
            ScrollMapImage((uintptr_t) 0x070002E8, Imageuls3, Imageult3);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            Imageult++;

            if (Imageult >= 0x100) {
                Imageult = 0;
            }
            ScrollMapImage((uintptr_t) d_course_banshee_boardwalk_dl_B278, 0, Imageult);
            break;
        case COURSE_ROYAL_RACEWAY:
            Imageult -= 20;
            if (Imageult < 0) {
                Imageult = 0xFF;
            }
            // d_course_royal_raceway_packed_dl_A6A8
            ScrollMapImage((uintptr_t) 0x0700A6A8, 0, Imageult);
            // d_course_royal_raceway_packed_dl_A648
            ScrollMapImage((uintptr_t) 0x0700A648, 0, Imageult);
            break;
        case COURSE_DK_JUNGLE:
            Imageult += 2;
            if (Imageult > 255) {
                Imageult = 0;
            }
            // d_course_dks_jungle_parkway_packed_dl_3DD0
            ScrollMapImage((uintptr_t) 0x07003DD0, 0, Imageult);
            // d_course_dks_jungle_parkway_packed_dl_3E40
            ScrollMapImage((uintptr_t) 0x07003E40, 0, Imageult);
            // d_course_dks_jungle_parkway_packed_dl_3EB0
            ScrollMapImage((uintptr_t) 0x07003EB0, 0, Imageult);
            // d_course_dks_jungle_parkway_packed_dl_3F30
            ScrollMapImage((uintptr_t) 0x07003F30, 0, Imageult);
            // d_course_dks_jungle_parkway_packed_dl_36A8
            ScrollMapImage((uintptr_t) 0x070036A8, 0, Imageult);
            Imageult2 -= 20;
            if (Imageult2 < 0) {
                Imageult2 = 0xFF;
            }
            // d_course_dks_jungle_parkway_packed_dl_9880
            ScrollMapImage((uintptr_t) 0x07009880, 0, Imageult2);
            CheckJungleTreeCollision();
            break;
    }
#else

#endif
}

void InitialMap2(void) {

    switch (CourseID) {
        case COURSE_MOO_MOO_FARM:
            MooSoundPointer = 0;
            MooSoundCounter = 200;
            break;
        case COURSE_KOOPA_BEACH:
            WaterVelocity = -0.1f;
            WaterHeight = 0.0f;
            break;
    }
}
