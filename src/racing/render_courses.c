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
#include "OverKartHooks.h"

s16 startfog = 995;
s16 endfog = 1000;
UNUSED s32 imageuls = 0;
s32 imageult = 0;
UNUSED s32 imageuls2 = 0;
s32 imageult2 = 0;
s32 imageuls3 = 0;
s32 imageult3 = 0;
s16 light_pitch = 0;
s16 light_yaw = 0;
s16 g_monitorCounter = 0;

s32 check_ura_polygon(Camera* camera) {
    if (camera->bump.flag_zx == 0) {
        return 1;
    }
    if ((camera->bump.flag_xy == 1) && (camera->bump.distance_xy < 3.0f)) {
        return 1;
    }
    if ((camera->bump.flag_yz == 1) && (camera->bump.distance_yz < 3.0f)) {
        return 1;
    }
    return 0;
}

void SearchListFile(uintptr_t addr) {
    s32 segment = SEGMENT_NUMBER2(addr);
    s32 offset = SEGMENT_OFFSET(addr);
    SEARCHLIST* section = (SEARCHLIST*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);

    while (section->address != 0) {
        if (section->flag & 0x8000) {
            wall_flag = 1;
        } else {
            wall_flag = 0;
        }
        if (section->flag & 0x2000) {
            flat_flag = 1;
        } else {
            flat_flag = 0;
        }
        if (section->flag & 0x4000) {
            D_8015F5A4 = 1;
        } else {
            D_8015F5A4 = 0;
        }
        search_list3(section->address, section->status, section->area);
        section++;
    }
}

extern u32 isFlycam;

void DisplayGroupmap(uintptr_t addr, Screen* arg1) {
    Player* player = arg1->kart;
    Camera* camera = arg1->camera;
    u32 segment = SEGMENT_NUMBER2(addr);
    u32 offset = SEGMENT_OFFSET(addr);
    //! @todo Should be Gfx*
    s32* gfx = (s32*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    s16 direction;
    s16 index;
    s16 sp1E;
    s16 temp_v0_3;
    CheckDirectionFunc(0, arg1);
    direction = arg1->camera_direction;

    if (camera_mode[camera - camera1] == 1) {
        sp1E = CheckArea(camera->bump.last_zx);
        temp_v0_3 = CheckArea(player->bump.last_zx);
        index = sp1E - temp_v0_3;
        if ((index < 2) && (index >= -1)) {
            if (sp1E == 255) {
                if (temp_v0_3 == 255) {
                    index = arg1->camera_point;
                } else if (player->bump.distance_zx > 30.0f) {
                    index = arg1->camera_point;
                } else {
                    index = temp_v0_3;
                }
            } else if (camera->bump.distance_zx > 30.0f) {
                index = arg1->camera_point;
            } else {
                index = sp1E;
            }
        } else {

            switch (g_courseID) {
                case COURSE_BOWSER_CASTLE:
                    if ((temp_v0_3 >= 0x11) && (temp_v0_3 < 0x18)) {
                        index = temp_v0_3;
                    } else if ((temp_v0_3 == 255) && (sp1E != 255)) {
                        index = sp1E;
                    } else if ((temp_v0_3 != 255) && (sp1E == 255)) {
                        index = temp_v0_3;
                    } else {
                        index = arg1->camera_point;
                    }
                    break;
                case COURSE_CHOCO_MOUNTAIN:
                    if ((temp_v0_3 >= 0xE) && (temp_v0_3 < 0x16)) {
                        index = temp_v0_3;
                    } else if ((temp_v0_3 == 255) && (sp1E != 255)) {
                        index = sp1E;
                    } else if ((temp_v0_3 != 255) && (sp1E == 255)) {
                        index = temp_v0_3;
                    } else {
                        index = arg1->camera_point;
                    }
                    break;
                default:
                    if (temp_v0_3 == 255) {
                        index = arg1->camera_point;
                    } else if (player->bump.distance_zx > 30.0f) {
                        index = arg1->camera_point;
                    } else {
                        index = temp_v0_3;
                    }
                    break;
            }
        }
    } else {
        index = CheckArea(camera->bump.last_zx);
        if (camera->bump.distance_zx > 30.0f) {
            index = arg1->camera_point;
        } else if (index == 255) {
            index = arg1->camera_point;
        }
    }

    arg1->camera_point = index;
    index = ((index - 1) * 4) + direction;
    gSPDisplayList(gDisplayListHead++, gfx[index]);
}

void display_mush(void) {
    // d_course_mario_raceway_packed_dl_1140
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07001140));
}

void display_pipe(void) {
    if (g_ScreenSplitB == SCREEN_MODE_1P) {
        // d_course_mario_raceway_packed_dl_8E8
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070008E8));
    } else {
        // d_course_mario_raceway_packed_dl_2D68
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07002D68));
    }
}

void DisplayMap2(Screen* arg0, s32 playerId) {
    UNUSED s32 pad;
    Player* player = arg0->kart;
    Mat4 matrix;
    Vec3f vector;
    u16 pathCounter;
    u16 cameraRot;
    s16 playerDirection;

    InitRDP();
    pathCounter = (u16) arg0->camera_point;
    cameraRot = (u16) arg0->camera->camera_direction[1];
    playerDirection = arg0->camera_direction;
    switch (playerId) {
        case PLAYER_ONE:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[PLAYER_ONE]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[PLAYER_ONE]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_TWO:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[PLAYER_TWO]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[PLAYER_TWO]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_THREE:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[PLAYER_THREE]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[PLAYER_THREE]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
        case PLAYER_FOUR:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxPersp[PLAYER_FOUR]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxLookAt[PLAYER_FOUR]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
            break;
    }
    LoadIdentAffineMtx(matrix);
    SetMatrix(matrix, 0);
    switch (g_courseID) {
        case COURSE_BOWSER_CASTLE:
            if (g_ScreenSplitA != SCREEN_MODE_1P) {
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
            gSPDisplayList(gDisplayListHead++, d_course_bowsers_castle_dl_9228);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            gDPPipeSync(gDisplayListHead++);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            // d_course_banshee_boardwalk_packed_dl_878
            gSPDisplayList(gDisplayListHead++, 0x07000878);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_KOOPA_BEACH:

            gDPPipeSync(gDisplayListHead++);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);

            switch (pathCounter) {
                case 22:
                case 23:
                case 29:
                case 30:
                case 31:
                case 37:
                    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
                    // d_course_koopa_troopa_beach_packed_dl_9E70
                    gSPDisplayList(gDisplayListHead++, 0x07009E70);
                    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
                    break;
            }
            vector[0] = 0.0f;
            vector[1] = g_waterHeight;
            vector[2] = 0.0f;
            CreateTransAffineMtx(matrix, vector);
            SetMatrix(matrix, 0);

            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            DisplayGroupmap((uintptr_t) d_course_koopa_troopa_beach_dl_list2, arg0);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_SHERBET_LAND:

            gDPPipeSync(gDisplayListHead++);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

            LoadIdentAffineMtx(matrix);
            SetMatrix(matrix, 0);
            DisplayGroupmap((uintptr_t) sherbet_land_dls_2, arg0);

            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            if ((check_ura_polygon(arg0->camera) == 1) && (CheckWaterLevel(player) < player->position[1])) {
                gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
                gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
                gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
                // d_course_sherbet_land_packed_dl_2B48
                gSPDisplayList(gDisplayListHead++, 0x07002B48);
            }
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_RAINBOW_ROAD:
            gDPPipeSync(gDisplayListHead++);
            LoadIdentAffineMtx(matrix);
            SetMatrix(matrix, 0);
            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            DisplayGroupmap((uintptr_t) &d_course_rainbow_road_dl_list, arg0);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_WARIO_STADIUM:
            gDPPipeSync(gDisplayListHead++);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

            LoadIdentAffineMtx(matrix);
            SetMatrix(matrix, 0);

            gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
            gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, 0xFF, 0x00, 0xFF);
            // d_course_wario_stadium_packed_dl_EC0
            gSPDisplayList(gDisplayListHead++, 0x07000EC0);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
        case COURSE_DK_JUNGLE:
            gDPPipeSync(gDisplayListHead++);
            gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
            gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);

            LoadIdentAffineMtx(matrix);
            SetMatrix(matrix, 0);

            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);

            if (pathCounter < 17) {
                gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                // d_course_dks_jungle_parkway_packed_dl_3E40
                gSPDisplayList(gDisplayListHead++, 0x07003E40);
                // d_course_dks_jungle_parkway_packed_dl_3EB0
                gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                if ((pathCounter >= 6) && (pathCounter < 13)) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                    // d_course_dks_jungle_parkway_packed_dl_3DD0
                    gSPDisplayList(gDisplayListHead++, 0x07003DD0);
                }
            } else if ((pathCounter == 21) || (pathCounter == 22)) {
                if (playerDirection == 3) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, 0x070036A8);
                }
                if ((playerDirection == 1) || (playerDirection == 0)) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, 0x070036A8);
                } else {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
                    // d_course_dks_jungle_parkway_packed_dl_3F30
                    gSPDisplayList(gDisplayListHead++, 0x07003F30);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, 0x070036A8);
                }
            } else if (pathCounter == 24) {
                if ((playerDirection == 0) || (playerDirection == 3)) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, 0x070036A8);
                }
            } else if (pathCounter == 23) {
                if (playerDirection == 3) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, 0x070036A8);
                } else if (playerDirection == 0) {
                    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
                    // d_course_dks_jungle_parkway_packed_dl_36A8
                    gSPDisplayList(gDisplayListHead++, 0x070036A8);
                }
            }
            gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
            switch (pathCounter) {
                case 5:
                    if (playerDirection != 3) {
                        // d_course_dks_jungle_parkway_packed_dl_3DD0
                        gSPDisplayList(gDisplayListHead++, 0x07003DD0);
                    }
                    break;
                case 17:
                    switch (playerDirection) {
                        case 0:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                            break;
                        case 1:
                            // d_course_dks_jungle_parkway_packed_dl_3DD0
                            gSPDisplayList(gDisplayListHead++, 0x07003DD0);
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                            break;
                        case 2:
                            // d_course_dks_jungle_parkway_packed_dl_
                            gSPDisplayList(gDisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                            // d_course_dks_jungle_parkway_packed_dl_3F30
                            gSPDisplayList(gDisplayListHead++, 0x07003F30);
                            break;
                        case 3:
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                            // d_course_dks_jungle_parkway_packed_dl_3F30
                            gSPDisplayList(gDisplayListHead++, 0x07003F30);
                            break;
                    }
                    break;
                case 18:
                    switch (playerDirection) {
                        case 0:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                            break;
                        case 1:
                            // d_course_dks_jungle_parkway_packed_dl_3DD0
                            gSPDisplayList(gDisplayListHead++, 0x07003DD0);
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                            break;
                        case 2:
                            // d_course_dks_jungle_parkway_packed_dl_3E40
                            gSPDisplayList(gDisplayListHead++, 0x07003E40);
                            // d_course_dks_jungle_parkway_packed_dl_3EB0
                            gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                            break;
                    }
                    break;
                case 21:
                    if ((playerDirection == 0) || (playerDirection == 1)) {
                        // d_course_dks_jungle_parkway_packed_dl_3E40
                        gSPDisplayList(gDisplayListHead++, 0x07003E40);
                        // d_course_dks_jungle_parkway_packed_dl_3EB0
                        gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(gDisplayListHead++, 0x07003F30);
                    } else {
                        // d_course_dks_jungle_parkway_packed_dl_3EB0
                        gSPDisplayList(gDisplayListHead++, 0x07003EB0);
                    }
                    break;
                case 22:
                    if (playerDirection == 0) {
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(gDisplayListHead++, 0x07003F30);
                    }
                    break;
                case 23:
                    if (playerDirection != 1) {
                        // d_course_dks_jungle_parkway_packed_dl_3F30
                        gSPDisplayList(gDisplayListHead++, 0x07003F30);
                    }
                    break;
            }
            gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
            gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
            gDPPipeSync(gDisplayListHead++);
            break;
    }
    XLUDisplay(arg0);
}

void DisplayKT1(Screen* arg0) {
    UNUSED s32 pad;
    u16 sp22 = arg0->camera_point;
    u16 playerDirection = arg0->camera_direction;

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_mario_raceway_packed_dl_3050
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07003050));
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    switch (sp22) {
        case 1:
        case 2:
        case 17:
            if ((playerDirection == 2) || (playerDirection == 1)) {
                display_pipe();
            }
            break;
        case 3:
            if (playerDirection != 0) {
                display_pipe();
            }
            break;
        case 4:
            if (playerDirection == 0) {
                display_mush();
            } else {
                if (playerDirection == 1) {
                    display_mush();
                }
                display_pipe();
            }
            break;
        case 5:
        case 6:
            if ((playerDirection == 2) || (playerDirection == 3)) {
                display_pipe();
            } else {
                display_mush();
            }
            break;
        case 7:
            display_mush();
            if ((playerDirection == 2) || (playerDirection == 3)) {
                display_pipe();
            }
            break;
        case 8:
        case 9:
            if (playerDirection != 1) {
                display_pipe();
            }
            /* fallthrough */
        case 10:
            if (playerDirection != 2) {
                display_mush();
            }
            break;
        case 11:
            if (playerDirection == 0) {
                display_pipe();
                display_mush();
            } else if (playerDirection == 3) {
                display_pipe();
            }
            break;
        case 12:
            if ((playerDirection == 0) || (playerDirection == 3)) {
                display_pipe();
            }
            break;
        case 13:
        case 14:
            if (playerDirection != 1) {
                case 15:
                case 16:
                    display_pipe();
            }
            break;
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_mario_raceway_packed_dl_3508
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07003508));
    // d_course_mario_raceway_packed_dl_3240
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07003240));
    // d_course_mario_raceway_packed_dl_14A0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070014A0));

    DisplayGroupmap((uintptr_t) mario_raceway_dls, arg0);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_mario_raceway_packed_dl_450
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000450));
    // d_course_mario_raceway_packed_dl_240
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000240));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_mario_raceway_packed_dl_E0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070000E0));
    // d_course_mario_raceway_packed_dl_160
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000160));
}

void display_KT2(Screen* arg0) {
    UNUSED s32 pad[13];

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_choco_mountain_packed_dl_4608
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07004608));
    }
    gDPSetCycleType(gDisplayListHead++, G_CYC_2CYCLE);
    gDPSetFogColor(gDisplayListHead++, fog_red, fog_blue, fog_green, 0xFF);
    gSPFogPosition(gDisplayListHead++, startfog, endfog);

    gDPPipeSync(gDisplayListHead++);
    gSPSetGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATERGB, G_CC_PASS2);

    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    // d_course_choco_mountain_packed_dl_5A70
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005A70));
    // d_course_choco_mountain_packed_dl_828
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000828));
    // d_course_choco_mountain_packed_dl_8E0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070008E0));
    // d_course_choco_mountain_packed_dl_5868
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005868));
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    DisplayGroupmap((uintptr_t) choco_mountain_dls, arg0);

    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_PASS2);
    // d_course_choco_mountain_packed_dl_448
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000448));
    // d_course_choco_mountain_packed_dl_5D8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070005D8));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_choco_mountain_packed_dl_718
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000718));
    gSPClearGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPPipeSync(gDisplayListHead++);
}

void display_KT3(Screen* arg0) {

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_bowsers_castle_packed_dl_6A80
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07006A80));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    imageult++;
    if (imageult > 255) {
        imageult = 0;
    }
    DisplayGroupmap((uintptr_t) bowsers_castle_dls, arg0);

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_bowsers_castle_packed_dl_248
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000248));
}

void display_KT4(Screen* arg0) {
    Camera* camera = arg0->camera;
    Mat4 spCC;
    UNUSED s32 pad[6];
    Vec3f spA8;
    UNUSED s32 pad2[6];

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    // d_course_banshee_boardwalk_packed_dl_7228
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07007228));

    gSPFogPosition(gDisplayListHead++, startfog, endfog);

    gDPPipeSync(gDisplayListHead++);

    gSPClearGeometryMode(gDisplayListHead++,
                         G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_banshee_boardwalk_packed_dl_5CD0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005CD0));
    // d_course_banshee_boardwalk_packed_dl_4E60
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07004E60));
    // d_course_banshee_boardwalk_packed_dl_69B0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070069B0));

    DisplayGroupmap((uintptr_t) banshee_boardwalk_dls, arg0);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_SHADING_SMOOTH);
    // d_course_banshee_boardwalk_packed_dl_580
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000580));
    // d_course_banshee_boardwalk_packed_dl_60
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000060));
    // d_course_banshee_boardwalk_packed_dl_540
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000540));

    if (camera->camera_pos[1] < -20.0f) {
        // d_course_banshee_boardwalk_packed_dl_6310
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07006310));
    }
    spA8[0] = camera->camera_pos[0];
    spA8[1] = -82.0f;
    spA8[2] = camera->camera_pos[2];
    CreateTransAffineMtx(spCC, spA8);
    SetMatrix(spCC, 0);

    gSPDisplayList(gDisplayListHead++, d_course_banshee_boardwalk_dl_B278);
    gDPPipeSync(gDisplayListHead++);
}

void display_KT5(Screen* arg0) {

    gDPPipeSync(gDisplayListHead++);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    DisplayGroupmap((uintptr_t) d_course_yoshi_valley_dl_list, arg0);
    gDPPipeSync(gDisplayListHead++);
}

void display_KT6(Screen* arg0) {

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_frappe_snowland_packed_dl_65E0
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070065E0));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    DisplayGroupmap((uintptr_t) d_course_frappe_snowland_dl_list, arg0);
}

void display_KT7(Screen* arg0) {

    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_koopa_troopa_beach_packed_dl_9CC0
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07009CC0));
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_koopa_troopa_beach_packed_dl_9688
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07009688));
    DisplayGroupmap((uintptr_t) d_course_koopa_troopa_beach_dl_list1, arg0);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_koopa_troopa_beach_packed_dl_2C0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070002C0));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPPipeSync(gDisplayListHead++);
}

void display_KT8(Screen* arg0) {

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_royal_raceway_packed_dl_B030
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x0700B030));
    }
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_royal_raceway_packed_dl_A648
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x0700A648));

    DisplayGroupmap((uintptr_t) royal_raceway_dls, arg0);

    // d_course_royal_raceway_packed_dl_11A8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070011A8));
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_royal_raceway_packed_dl_8A0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070008A0));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void display_KT9(Screen* arg0) {

    UNUSED s32 pad;
    u16 sp22 = (u16) arg0->camera_point;
    s16 prevFrame;

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_luigi_raceway_packed_dl_9EC0
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07009EC0));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    DisplayGroupmap((uintptr_t) luigi_raceway_dls, arg0);

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_luigi_raceway_packed_dl_E0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070000E0));
    // d_course_luigi_raceway_packed_dl_68
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000068));

    D_800DC5DC = 88;
    D_800DC5E0 = 72;

    // Render only the first player camera onto the television billboard. Screen agnostic screens of other players).
    if ((g_ScreenSplitA == SCREEN_MODE_1P) && (sp22 >= 10) && (sp22 < 17)) {

        prevFrame = (s16) g_DispFrame - 1;

        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }
        g_monitorCounter++;
        if (g_monitorCounter >= 6) {
            g_monitorCounter = 0;
        }
        /**
         * The jumbo television screen is split into six sections each section is copied one at a time.
         * This is done to fit within the n64's texture size requirements; 64x32
         */
        switch (g_monitorCounter) {
            case 0:
                GetFramebuffer(D_800DC5DC, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xF800));
                break;
            case 1:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x10800));
                break;
            case 2:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x11800));
                break;
            case 3:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x12800));
                break;
            case 4:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x13800));
                break;
            case 5:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x14800));
                break;
        }
    }
}

// Missing {} around if statements necessary for matching.
void display_KT10(Screen* arg0) {
    UNUSED s32 pad[13];
    s16 temp_s0 = arg0->camera_point;
    s16 playerDirection = arg0->camera_direction;

    MakeLight(D_800DC610, light_yaw, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_moo_moo_farm_packed_dl_4DF8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07004DF8));
    // d_course_moo_moo_farm_packed_dl_5640
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005640));
    gSPFogPosition(gDisplayListHead++, startfog, endfog);

    DisplayGroupmap((uintptr_t) moo_moo_farm_dls, arg0);

    if ((temp_s0 < 14) && (temp_s0 > 10)) {
        if ((playerDirection == 2) || (playerDirection == 3) || (playerDirection == 1))
            //
            gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_13FF8);

    } else if (temp_s0 < 16) {
        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_13FF8);
    } else if (temp_s0 < 19) {
        if (playerDirection != 2)
            gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_13FF8);

    } else if (temp_s0 < 20) {
        if (playerDirection == 0)
            gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_13FF8);
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    if ((temp_s0 >= 16) && (temp_s0 < 24)) {
        if ((playerDirection == 2) || (playerDirection == 3))
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005410));

    } else if (temp_s0 < 9) {
        if (playerDirection == 2)
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07005410));
    }
    if (temp_s0 < 4) {
        if (playerDirection != 0)
            gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_14060);

    } else if (temp_s0 < 8) {
        if (playerDirection == 2)
            gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_14060);

    } else if (temp_s0 >= 22) {
        gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_14060);
    } else if (temp_s0 >= 18) {
        if ((playerDirection == 0) || (playerDirection == 3))
            gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_14060);
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_moo_moo_farm_packed_dl_10C0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070010C0));
}

void display_KT11(Screen* arg0) {
    UNUSED s32 pad[13];

    MakeLight(D_800DC610, light_yaw, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetFogColor(gDisplayListHead++, fog_red, fog_blue, fog_green, 0xFF);
    gDPSetCycleType(gDisplayListHead++, G_CYC_2CYCLE);
    gSPFogPosition(gDisplayListHead++, startfog, endfog);
    gSPSetGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_PASS2);
    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);

    DisplayGroupmap((uintptr_t) d_course_toads_turnpike_dl_list, arg0);

    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_PASS2);
    // d_course_toads_turnpike_packed_dl_0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000000));
    // d_course_toads_turnpike_packed_dl_68
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000068));
    // d_course_toads_turnpike_packed_dl_D8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070000D8));
    gSPClearGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void display_KT12(Screen* arg0) {

    MakeLight(D_800DC610, light_yaw, 0, 1);

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_kalimari_desert_packed_dl_71C8
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070071C8));
    }

    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    DisplayGroupmap((uintptr_t) kalimari_desert_dls, arg0);
    // d_course_kalimari_desert_packed_dl_1ED8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07001ED8));
    // d_course_kalimari_desert_packed_dl_1B18
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07001B18));
    // d_course_kalimari_desert_packed_dl_8330
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07008330));
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_kalimari_desert_packed_dl_998
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000998));
    // d_course_kalimari_desert_packed_dl_270
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000270));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void display_KT13(Screen* arg0) {

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    DisplayGroupmap((uintptr_t) sherbet_land_dls, arg0);
}

void display_KT14(UNUSED Screen* arg0) {

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
}

void display_KT15(Screen* arg0) {
    s16 prevFrame;

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {

        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_wario_stadium_packed_dl_A0C8
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x0700A0C8));
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATERGBA, G_CC_MODULATERGBA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    DisplayGroupmap((uintptr_t) wario_stadium_dls, arg0);

    // d_course_wario_stadium_packed_dl_A228
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x0700A228));
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_wario_stadium_packed_dl_A88
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000A88));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);

    D_800DC5DC = 88;
    D_800DC5E0 = 72;
    if (g_ScreenSplitA == SCREEN_MODE_1P) {
        prevFrame = (s16) g_DispFrame - 1;
        if (prevFrame < 0) {
            prevFrame = 2;
        } else if (prevFrame >= 3) {
            prevFrame = 0;
        }
        g_monitorCounter++;
        if (g_monitorCounter > 5) {
            g_monitorCounter = 0;
        }
        switch (g_monitorCounter) {
            case 0:
                GetFramebuffer(D_800DC5DC, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x8800));
                break;
            case 1:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0x9800));
                break;
            case 2:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xA800));
                break;
            case 3:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 32, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xB800));
                break;
            case 4:
                GetFramebuffer(D_800DC5DC, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xC800));
                break;
            case 5:
                GetFramebuffer(D_800DC5DC + 64, D_800DC5E0 + 64, 64, 32,
                                 (u16*) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[prevFrame]),
                                 (u16*) PHYSICAL_TO_VIRTUAL(SegmentTable[5] + 0xD800));
                break;
        }
    }
}

void DisplayKT16(UNUSED Screen* arg0) {

    MakeLight(D_800DC610, light_yaw, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_block_fort_packed_dl_15C0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070015C0));
}

void display_KT17(UNUSED Screen* arg0) {
    MakeLight(D_800DC610, light_yaw, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    // d_course_skyscraper_packed_dl_FE8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000FE8));
    // d_course_skyscraper_packed_dl_C60
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000C60));
    // d_course_skyscraper_packed_dl_B70
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000B70));
    // d_course_skyscraper_packed_dl_6B8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070006B8));
    // d_course_skyscraper_packed_dl_570
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000570));
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_10C8
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070010C8));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_skyscraper_packed_dl_258
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000258));
}

void display_KT18(UNUSED Screen* arg0) {

    MakeLight(D_800DC610, light_yaw, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    // d_course_double_deck_packed_dl_738
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000738));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void display_KT19(Screen* arg0) {

    MakeLight(D_800DC610, light_yaw, 0, 1);
    MakeLight(&D_800DC610[1], light_yaw, light_pitch, 1);

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK | G_LIGHTING);

    if (check_ura_polygon(arg0->camera) == 1) {
        gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_dks_jungle_parkway_packed_dl_92D8
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x070092D8));
    }

    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    DisplayGroupmap((uintptr_t) d_course_dks_jungle_parkway_unknown_dl_list, arg0);

    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
}

void display_KT20(Screen* arg0) {

    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    MakeLight(D_800DC610, light_yaw, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (check_ura_polygon(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_big_donut_packed_dl_DE8
        gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000DE8));
    }
    // d_course_big_donut_packed_dl_450
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000450));
    // d_course_big_donut_packed_dl_AC0
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000AC0));
    // d_course_big_donut_packed_dl_D20
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000D20));
    // d_course_big_donut_packed_dl_230
    gSPDisplayList(gDisplayListHead++, ((uintptr_t) 0x07000230));
}

/**
 * @brief The game has two versions of each course. One for normal gameplay and another for credits.
 * The credits version is not split into segments so that the game camera can fly around the whole course.
 *
 */
void display_all_map(void) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            if (HotSwapID == 0) {
                gSPDisplayList(gDisplayListHead++, d_course_mario_raceway_dl_9348);
            }
            break;
        case COURSE_CHOCO_MOUNTAIN:
            gSPDisplayList(gDisplayListHead++, d_course_choco_mountain_dl_71B8);
            break;
        case COURSE_BOWSER_CASTLE:
            gSPDisplayList(gDisplayListHead++, d_course_bowsers_castle_dl_9148);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            gSPDisplayList(gDisplayListHead++, d_course_banshee_boardwalk_dl_B308);
            break;
        case COURSE_YOSHI_VALLEY:
            gSPDisplayList(gDisplayListHead++, d_course_yoshi_valley_dl_18020);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            gSPDisplayList(gDisplayListHead++, d_course_frappe_snowland_dl_76A0);
            break;
        case COURSE_KOOPA_BEACH:
            gSPDisplayList(gDisplayListHead++, d_course_koopa_troopa_beach_dl_18D68);
            break;
        case COURSE_ROYAL_RACEWAY:
            gSPDisplayList(gDisplayListHead++, d_course_royal_raceway_dl_D8E8);
            break;
        case COURSE_LUIGI_RACEWAY:
            gSPDisplayList(gDisplayListHead++, d_course_luigi_raceway_dl_FD40);
            break;
        case COURSE_MOO_MOO_FARM:
            gSPDisplayList(gDisplayListHead++, d_course_moo_moo_farm_dl_14088);
            break;
        case COURSE_TOADS_TURNPIKE:
            gSPDisplayList(gDisplayListHead++, d_course_toads_turnpike_dl_23930);
            break;
        case COURSE_KALAMARI_DESERT:
            gSPDisplayList(gDisplayListHead++, d_course_kalimari_desert_dl_22E00);
            break;
        case COURSE_SHERBET_LAND:
            gSPDisplayList(gDisplayListHead++, d_course_sherbet_land_dl_9AE8);
            break;
        case COURSE_RAINBOW_ROAD:
            gSPDisplayList(gDisplayListHead++, d_course_rainbow_road_dl_16220);
            break;
        case COURSE_WARIO_STADIUM:
            gSPDisplayList(gDisplayListHead++, d_course_wario_stadium_dl_CA78);
            break;
        case COURSE_BLOCK_FORT:
            if (HotSwapID == 0) {
                gSPDisplayList(gDisplayListHead++, d_course_sherbet_land_dl_0);
            }
            break;
        case COURSE_SKYSCRAPER:
            gSPDisplayList(gDisplayListHead++, d_course_sherbet_land_dl_0);
            break;
        case COURSE_DOUBLE_DECK:
            gSPDisplayList(gDisplayListHead++, d_course_sherbet_land_dl_0);
            break;
        case COURSE_DK_JUNGLE:
            gSPDisplayList(gDisplayListHead++, d_course_dks_jungle_parkway_dl_13C30);
            break;
        case COURSE_BIG_DONUT:
            gSPDisplayList(gDisplayListHead++, d_course_sherbet_land_dl_0);
            break;
    }
#else

#endif
}

void display_map(Screen* arg0) {

    MakeLight(D_800DC610, light_yaw, 0, 1);
    if (g_DisplayFlag) {
        display_all_map();
        return;
    }

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            DisplayKT1Hook(arg0);
            break;
        case COURSE_CHOCO_MOUNTAIN:
            display_KT2(arg0);
            break;
        case COURSE_BOWSER_CASTLE:
            display_KT3(arg0);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            display_KT4(arg0);
            break;
        case COURSE_YOSHI_VALLEY:
            display_KT5(arg0);
            break;
        case COURSE_FRAPPE_SNOWLAND:
            display_KT6(arg0);
            break;
        case COURSE_KOOPA_BEACH:
            display_KT7(arg0);
            break;
        case COURSE_ROYAL_RACEWAY:
            display_KT8(arg0);
            break;
        case COURSE_LUIGI_RACEWAY:
            display_KT9(arg0);
            break;
        case COURSE_MOO_MOO_FARM:
            display_KT10(arg0);
            break;
        case COURSE_TOADS_TURNPIKE:
            display_KT11(arg0);
            break;
        case COURSE_KALAMARI_DESERT:
            display_KT12(arg0);
            break;
        case COURSE_SHERBET_LAND:
            display_KT13(arg0);
            break;
        case COURSE_RAINBOW_ROAD:
            display_KT14(arg0);
            break;
        case COURSE_WARIO_STADIUM:
            display_KT15(arg0);
            break;
        case COURSE_BLOCK_FORT:
            DisplayKT16Hook(arg0);
            break;
        case COURSE_SKYSCRAPER:
            display_KT17(arg0);
            break;
        case COURSE_DOUBLE_DECK:
            display_KT18(arg0);
            break;
        case COURSE_DK_JUNGLE:
            display_KT19(arg0);
            break;
        case COURSE_BIG_DONUT:
            display_KT20(arg0);
            break;
    }
#else

#endif
}

void initial_bump_kart(s32 playerIndex) {
    Player* player = &gPlayers[playerIndex];
    InitialBump(&player->bump);
    player->tire_FR.LastAxis = 0;
    player->tire_FL.LastAxis = 0;
    player->tire_RR.LastAxis = 0;
    player->tire_RL.LastAxis = 0;

    player->tire_FR.LastPointer = 0x1388;
    player->tire_FL.LastPointer = 0x1388;
    player->tire_RR.LastPointer = 0x1388;
    player->tire_RL.LastPointer = 0x1388;
}

void MakeCollision(void) {
    maximum_x += 20;
    maximum_z += 20;
    minimum_x += -20;
    minimum_z += -20;
    minimum_y += -20;
    make_collision_table();
    FreeMemoryPointer += ALIGN16(collision_pointer * sizeof(u16));
    CheckCourseMemoryHighWater();
}

UNUSED void func_80295D50(s16 arg0, s16 arg1) {
    maxlimmity = arg1;
    minlimmity = arg0;
}

void reset_limmity(void) {
    maxlimmity = 3000;
    minlimmity = -3000;
}

/**
 * @brief Also sets vertex colours for the course vtx data
 *
 */
void InitialMap(void) {
    g_simpleObjectCount = 0;

    minimum_x = 0;
    minimum_y = 0;
    minimum_z = 0;

    maximum_x = 0;
    maximum_y = 0;
    maximum_z = 0;

    wall_flag = 0;
    flat_flag = 0;
    reset_limmity();
    facenumber = 0;
    g_courseFaceMaxIndex = 0;
    gFaceBuffer = (FaceStruct*) FACE_BUFFER_START;
    g_skyToggle = 0;
    g_fadeOutCounter2 = 0;
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            // d_course_mario_raceway_packed_dl_1140
            SearchListHook(0x07001140);
            if (g_ScreenSplitB == SCREEN_MODE_1P) {
                // d_course_mario_raceway_packed_dl_8E8
                SearchListHook(0x070008E8);
            } else {
                // d_course_mario_raceway_packed_dl_2D68
                SearchListHook(0x07002D68);
            }
            SearchListFileHook((unsigned int) d_course_mario_raceway_addr);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_CHOCO_MOUNTAIN:
            g_skyToggle = 1;
            fog_red = 255;
            fog_blue = 255;
            fog_green = 255;
            startfog = 0x3E3;
            endfog = 0x3E8;
            light_yaw = 0x71C;
            light_pitch = 0xE38;

            // Spawn guardrail only for CC_50 and time trials.
            if ((g_raceClass != CC_50) && (g_gameMode != TIME_TRIALS)) {
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
            // D_8015F590 is only used here, so this seems meaningless
            InitialLight(DEGREES(50), DEGREES(70), D_8015F590);
            MakeCollision();
            g_waterHeight = -80.0f;
            break;
        case COURSE_BOWSER_CASTLE:
            SearchListFile((uintptr_t) d_course_bowsers_castle_addr);
            MakeCollision();
            // d_course_bowsers_castle_packed_dl_1350
            MakeWaterVertex((uintptr_t) 0x07001350, 0x32, 0, 0, 0);
            g_waterHeight = -50.0f;
            break;
        case COURSE_BANSHEE_BOARDWALK:
            g_skyToggle = 1;
            fog_red = 0;
            fog_blue = 0;
            fog_green = 0;
            SearchListFile((uintptr_t) d_course_banshee_boardwalk_track_sections);
            MakeCollision();
            // d_course_banshee_boardwalk_packed_dl_878
            MakeWaterVertex((uintptr_t) 0x07000878, 128, 0, 0, 0);
            g_waterHeight = -80.0f;
            break;
        case COURSE_YOSHI_VALLEY:
            MakeLight(&d_course_yoshi_valley_lights4, -0x38F0, 0x1C70, 1);
            SearchListFile((uintptr_t) d_course_yoshi_valley_addr);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_FRAPPE_SNOWLAND:
            SearchListFile((uintptr_t) d_course_frappe_snowland_addr);
            MakeCollision();
            g_waterHeight = -50.0f;
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
            g_waterHeight = -60.0f;
            break;
        case COURSE_LUIGI_RACEWAY:
            SearchListFile((uintptr_t) d_course_luigi_raceway_addr);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_MOO_MOO_FARM:
            SearchListFile((uintptr_t) d_course_moo_moo_farm_addr);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_TOADS_TURNPIKE:
            fog_red = 43;
            fog_blue = 13;
            fog_green = 4;
            startfog = 993;
            endfog = 1000;
            SearchListFile((uintptr_t) d_course_toads_turnpike_addr);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_KALAMARI_DESERT:
            SearchListFile((uintptr_t) d_course_kalimari_desert_addr);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_SHERBET_LAND:
            SearchListFile((uintptr_t) d_course_sherbet_land_addr);
            MakeCollision();
            g_waterHeight = -18.0f;
            // d_course_sherbet_land_packed_dl_1EB8
            MakeWaterVertex((uintptr_t) 0x07001EB8, 180, 255, 255, 255);
            // d_course_sherbet_land_packed_dl_2308
            MakeWaterVertex((uintptr_t) 0x07002308, 150, 255, 255, 255);
            break;
        case COURSE_RAINBOW_ROAD:
            g_fadeOutCounter2 = 1;
            SearchListFile((uintptr_t) d_course_rainbow_road_addr);
            MakeCollision();
            g_waterHeight = 0.0f;
            // d_course_rainbow_road_packed_dl_2068
            MakeWaterVertex((uintptr_t) 0x07002068, 150, 255, 255, 255);
            // d_course_rainbow_road_packed_dl_1E18
            MakeWaterVertex((uintptr_t) 0x07001E18, 150, 255, 255, 255);
            // d_course_rainbow_road_packed_dl_1318
            MakeWaterVertex((uintptr_t) 0x07001318, 255, 255, 255, 0);
            if (gGamestate != CREDITS_SEQUENCE) {
                // d_course_rainbow_road_packed_dl_1FB8
                MakeWaterVertex((uintptr_t) 0x07001FB8, 150, 255, 255, 255);
            }
            break;
        case COURSE_WARIO_STADIUM:
            SearchListFile((uintptr_t) d_course_wario_stadium_addr);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
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
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_SKYSCRAPER:
            // d_course_skyscraper_packed_dl_1110
            SearchList2((Gfx*) 0x07001110, 1);
            // d_course_skyscraper_packed_dl_258
            SearchList2((Gfx*) 0x07000258, 1);
            MakeCollision();

            g_waterHeight = -480.0f;
            break;
        case COURSE_DOUBLE_DECK:
            // d_course_double_deck_packed_dl_738
            SearchList2((Gfx*) 0x07000738, 1);
            MakeCollision();
            g_waterHeight = minimum_y - 10.0f;
            break;
        case COURSE_DK_JUNGLE:
            SearchListFile((uintptr_t) d_course_dks_jungle_parkway_addr);
            MakeCollision();
            g_waterHeight = -475.0f;
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
            g_waterHeight = 100.0f;
            break;
    }
#else

#endif
}

void map_strategy(void) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_KOOPA_BEACH:
            // clang-format off
            if (g_waterVelocity < 0.0f) {
                if (g_waterHeight < -20.0f) { g_waterVelocity *= -1.0f; }
            } else {
                if (g_waterHeight > 0.0f) { g_waterVelocity *= -1.0f; }
            }
            // clang-format on
            g_waterHeight += g_waterVelocity;

            imageult += 9;
            if (imageult > 255) {
                imageult = 0;
            }
            imageult2 += 3;
            if (imageult2 > 255) {
                imageult2 = 0;
            }
            // waterfall animation
            // d_course_koopa_troopa_beach_packed_dl_9D58
            ScrollMapImage((uintptr_t) 0x07009D58, 0, imageult);
            // d_course_koopa_troopa_beach_packed_dl_9CD0
            ScrollMapImage((uintptr_t) 0x07009CD0, 0, imageult2);
            imageult3 = MakeRandomLimmit(300) / 40;
            if (imageuls3 < 0) {
                imageuls3 = MakeRandomLimmit(300) / 40;
            } else {
                imageuls3 = -(MakeRandomLimmit(300) / 40);
            }
            // Waterfall bubbling effect? (unused)
            // d_course_koopa_troopa_beach_packed_dl_2E8
            ScrollMapImage((uintptr_t) 0x070002E8, imageuls3, imageult3);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            imageult++;

            if (imageult >= 0x100) {
                imageult = 0;
            }
            ScrollMapImage((uintptr_t) d_course_banshee_boardwalk_dl_B278, 0, imageult);
            break;
        case COURSE_ROYAL_RACEWAY:
            imageult -= 20;
            if (imageult < 0) {
                imageult = 0xFF;
            }
            // d_course_royal_raceway_packed_dl_A6A8
            ScrollMapImage((uintptr_t) 0x0700A6A8, 0, imageult);
            // d_course_royal_raceway_packed_dl_A648
            ScrollMapImage((uintptr_t) 0x0700A648, 0, imageult);
            break;
        case COURSE_DK_JUNGLE:
            imageult += 2;
            if (imageult > 255) {
                imageult = 0;
            }
            // d_course_dks_jungle_parkway_packed_dl_3DD0
            ScrollMapImage((uintptr_t) 0x07003DD0, 0, imageult);
            // d_course_dks_jungle_parkway_packed_dl_3E40
            ScrollMapImage((uintptr_t) 0x07003E40, 0, imageult);
            // d_course_dks_jungle_parkway_packed_dl_3EB0
            ScrollMapImage((uintptr_t) 0x07003EB0, 0, imageult);
            // d_course_dks_jungle_parkway_packed_dl_3F30
            ScrollMapImage((uintptr_t) 0x07003F30, 0, imageult);
            // d_course_dks_jungle_parkway_packed_dl_36A8
            ScrollMapImage((uintptr_t) 0x070036A8, 0, imageult);
            imageult2 -= 20;
            if (imageult2 < 0) {
                imageult2 = 0xFF;
            }
            // d_course_dks_jungle_parkway_packed_dl_9880
            ScrollMapImage((uintptr_t) 0x07009880, 0, imageult2);
            check_jungle_tree_collision();
            break;
    }
#else

#endif
}

void initial_map2(void) {

    switch (g_courseID) {
        case COURSE_MOO_MOO_FARM:
            g_mooSoundPointer = 0;
            g_mooSoundCounter = 200;
            break;
        case COURSE_KOOPA_BEACH:
            g_waterVelocity = -0.1f;
            g_waterHeight = 0.0f;
            break;
    }
}
