#include <ultra64.h>
#include <macros.h>
#include <common_structs.h>
#include <defines.h>
#include <mk64.h>
#include <course.h>

#include "code_800029B0.h"
#include "camera.h"
#include "math_util.h"
#include "math_util_2.h"
#include "main.h"
#include "decode.h"
#include "kart_dma.h"
#include "objects.h"
#include "render_player.h"
#include "code_80057C60.h"
#include "effects.h"
#include "buffers.h"
#include "path.h"
#include "player_controller.h"
#include "render_objects.h"
#include <assets/common_data.h>
#include "skybox_and_splitscreen.h"
#include "spawn_players.h"

s8 gRenderingFramebufferByPlayer[] = { 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02 };

s32 gPlayersToRenderCount = 0;

// Can't find anything that actually references these...
UNUSED void* D_800DDB5C[3] = { gFramebuffer0, gFramebuffer1, gFramebuffer2 };

s16 gPlayersToRenderPlayerId[8];
s16 gPlayersToRenderScreenId[8];
Player* buffer_car[8];
s16 effectcount;
s32 D_80164AF4[3];
struct_D_802F1F80* gPlayerPalette;
u8* sKartUpperTexture;
u8* sKartLowerTexture;
u16 g_colorPlayer0R[8];
u16 bright_g[8];
u16 bright_b[8];
u16 adjust_r[8];
u16 adjust_g[8];
u16 adjust_b[8];
// Likely an unused colour effect.
UNUSED u16 adjust_a[8];
s32 color_timer[296];
s16 D_80165020[40];
// Used to calculate difference between previous and current player velocity.
Vec3f old_velocity[8];
s16 old_dir[4][8];
s16 old_deg[4][8];
s16 old_slipcount[4][8];
s16 compel_flag[4][8];
s16 draw_number[4][8];

void check_frame(s32* arg0, s32* arg1) {
    if ((g_DemoFlag == 1) || (opening_flag != 0) || (finish_zoom_flag != 0)) {
        *arg0 = 0xAA;
    } else {
        *arg0 = 0;
    }
    if (opening_flag != 0) {
        *arg1 = 0xAA;
        return;
    }
    *arg1 = 0;
}

void CheckViewport(Player* player, Camera* camera, s8 screenId) {
    UNUSED s32 pad;
    s32 sp30;
    s32 sp2C;
    UNUSED s32 pad2;

    check_kart_number(player);
    check_frame(&sp30, &sp2C);

    player->status &= ~(P1_DRAW_SPRITE << (screenId * 4));
    player->status &= ~(P1_DRAW_PARTICLES << (screenId * 4));

    if (viewport(player, camera, (f32) (select_viewKart + sp30), (f32) (select_rearviewKart + sp2C)) == 1) {
        player->status |= P1_DRAW_SPRITE << (screenId * 4);
    }
    if (viewport(player, camera, (f32) select_viewObj, (f32) select_rearviewObj) == 1) {
        player->status |= P1_DRAW_PARTICLES << (screenId * 4);
    }
}

u16 viewport(Player* player, Camera* camera, f32 arg2, f32 arg3) {
    UNUSED f32 pad[6];
    f32 sp64;
    f32 sp60;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    s16 var_v0;
    u16 ret;

    ret = false;
    switch (g_ScreenSplitA) { /* irregular */
        case SCREEN_MODE_1P:
            var_v0 = 0x293C;
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            var_v0 = 0x3058;
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            var_v0 = 0x1FFE;
            break;
        default:
            if (1) {}
            var_v0 = 0x1FFE;
            break;
    }
    sp4C = (arg2 * cosT((camera->camera_direction[1] - var_v0))) + camera->camera_pos[2];
    sp58 = (arg2 * sinT((camera->camera_direction[1] - var_v0))) + camera->camera_pos[0];
    sp48 = (arg2 * cosT((camera->camera_direction[1] + var_v0))) + camera->camera_pos[2];
    sp54 = (arg2 * sinT((camera->camera_direction[1] + var_v0))) + camera->camera_pos[0];
    sp44 = (arg3 * cosT((camera->camera_direction[1] + (135 * DEGREES(1))))) + camera->camera_pos[2];
    sp50 = (arg3 * sinT((camera->camera_direction[1] + (135 * DEGREES(1))))) + camera->camera_pos[0];

    sp64 = ((sp4C - player->position[2]) * (sp54 - player->position[0])) - ((sp48 - player->position[2]) * (sp58 - player->position[0]));
    sp60 = ((sp48 - player->position[2]) * (sp50 - player->position[0])) - ((sp44 - player->position[2]) * (sp54 - player->position[0]));
    sp5C = ((sp44 - player->position[2]) * (sp58 - player->position[0])) - ((sp4C - player->position[2]) * (sp50 - player->position[0]));

    if (((sp64 >= 0) && (sp60 >= 0) && (sp5C >= 0)) || (((sp64) <= 0) && (sp60 <= 0) && (sp5C <= 0))) {
        ret = true;
    }
    return ret;
}

u16 slipstream_kart(Player* player, f32 posX, UNUSED f32 arg2, f32 posZ) {
    f32 sp64;
    f32 sp60;
    f32 sp5c;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 temp_f14;
    f32 thing0;
    f32 thing1;
    u16 ret;

    ret = false;

    sp58 = (70.0f * cosT(((player->slipang - player->direction[1]) - (10 * DEGREES(1))))) + player->position[2];
    sp64 = (70.0f * sinT(((player->slipang - player->direction[1]) - (10 * DEGREES(1))))) + player->position[0];
    sp54 = (70.0f * cosT(((player->slipang - player->direction[1]) + (10 * DEGREES(1))))) + player->position[2];
    sp60 = (70.0f * sinT(((player->slipang - player->direction[1]) + (10 * DEGREES(1))))) + player->position[0];
    sp50 = (10.0f * cosT(((player->slipang - player->direction[1]) + (40 * DEGREES(1))))) + player->position[2];
    sp5c = (10.0f * sinT(((player->slipang - player->direction[1]) + (40 * DEGREES(1))))) + player->position[0];

    temp_f14 = ((sp58 - posZ) * (sp60 - posX)) - ((sp54 - posZ) * (sp64 - posX));
    thing0 = ((sp54 - posZ) * (sp5c - posX)) - ((sp50 - posZ) * (sp60 - posX));
    thing1 = ((sp50 - posZ) * (sp64 - posX)) - ((sp58 - posZ) * (sp5c - posX));
    if (((temp_f14 >= 0) && (thing0 >= 0) && (thing1 >= 0)) || ((temp_f14 <= 0) && (thing0 <= 0) && (thing1 <= 0))) {
        ret = true;
    }
    return ret;
}

/**
 * @brief one of his function is to put the frame to load in to a list to load it to be only applied for the next frame
 */
void init_render_player(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    UNUSED s32 pad[4];
    s32 sp4C;
    s32 sp48;
    UNUSED s32 pad2;
    s32 temp_v0;
    s32 temp_v0_2;

    if ((player->flag & EXISTS) == EXISTS) {
        CheckViewport(player, camera, screenId);
        temp_v0 = P1_DRAW_SPRITE << (screenId << 2);
        if (temp_v0 == (player->status & temp_v0)) {
            if (!(player->flag & NO_CONTROLS)) {
                kart_posset(player, camera, screenId, playerId);
            } else {
                kart_posset(player, camera, screenId, playerId);
                player->kartpitch = 0;
                player->kartroll = 0;
                player->angle_z[screenId] = 0;
            }
        }
        check_frame(&sp4C, &sp48);
        temp_v0_2 = P1_DMA_SPRITE << (screenId << 2);
        if ((temp_v0 == (player->status & temp_v0)) && (temp_v0_2 == (player->status & temp_v0_2))) {
            if ((viewport(player, camera, select_dmaKart + sp4C, select_reardmaKart + sp48) == 1) & 0xFFFF) {
                gPlayersToRenderPlayerId[gPlayersToRenderCount] = (s16) playerId;
                gPlayersToRenderScreenId[gPlayersToRenderCount] = (s16) screenId;
                buffer_car[gPlayersToRenderCount] = player;
                gPlayersToRenderCount += 1;
                compel_flag[screenId][playerId] = 0;
                old_dir[screenId][playerId] = player->dir[screenId];
                old_deg[screenId][playerId] = player->deg[screenId];
                old_slipcount[screenId][playerId] = player->slipcount;
                draw_number[screenId][playerId] += 1;
                if (draw_number[screenId][playerId] == 2) {
                    draw_number[screenId][playerId] = 0;
                }
            } else {
                if ((viewport(player, camera, select_NondmaKart + sp4C, select_rearNondmaKart) == 1) & 0xFFFF) {
                    if ((draw_frame == gRenderingFramebufferByPlayer[playerId]) ||
                        ((old_dir[screenId][playerId] - player->dir[screenId]) > 0x13) ||
                        ((old_dir[screenId][playerId] - player->dir[screenId]) < -0x13) ||
                        (compel_flag[screenId][playerId] == (s16) 1U)) {
                        gPlayersToRenderPlayerId[gPlayersToRenderCount] = (s16) playerId;
                        gPlayersToRenderScreenId[gPlayersToRenderCount] = (s16) screenId;
                        buffer_car[gPlayersToRenderCount] = player;
                        gPlayersToRenderCount += 1;
                        old_dir[screenId][playerId] = player->dir[screenId];
                        old_deg[screenId][playerId] = player->deg[screenId];
                        old_slipcount[screenId][playerId] = player->slipcount;
                        compel_flag[screenId][playerId] = 0;
                        draw_number[screenId][playerId] += 1;
                        if (draw_number[screenId][playerId] == 2) {
                            draw_number[screenId][playerId] = 0;
                        }
                    }
                } else {
                    if (((old_dir[screenId][playerId] - player->dir[screenId]) > 0x13) ||
                        ((old_dir[screenId][playerId] - player->dir[screenId]) < -0x13) ||
                        (compel_flag[screenId][playerId] == (s16) 1U)) {
                        gPlayersToRenderPlayerId[gPlayersToRenderCount] = (s16) playerId;
                        gPlayersToRenderScreenId[gPlayersToRenderCount] = (s16) screenId;
                        buffer_car[gPlayersToRenderCount] = player;
                        gPlayersToRenderCount += 1;
                        old_dir[screenId][playerId] = (s16) player->dir[screenId];
                        old_deg[screenId][playerId] = player->deg[screenId];
                        old_slipcount[screenId][playerId] = player->slipcount;
                        compel_flag[screenId][playerId] = 0;
                        draw_number[screenId][playerId] += 1;
                        if (draw_number[screenId][playerId] == 2) {
                            draw_number[screenId][playerId] = 0;
                        }
                    }
                }
            }
        }
    }
}

void CheckDMA(void) {
    s16 i;

    Pakartdma(buffer_car[0], gPlayersToRenderPlayerId[0], gPlayersToRenderScreenId[0],
                                   gPlayersToRenderScreenId[0],
                                   draw_number[gPlayersToRenderScreenId[0]][gPlayersToRenderPlayerId[0]]);
    SmokeDisp(gPlayerOneCopy, PLAYER_ONE, PLAYER_ONE);
    SmokeDisp(gPlayerTwo, PLAYER_TWO, PLAYER_ONE);
    SmokeDisp(gPlayerThree, PLAYER_THREE, PLAYER_ONE);
    SmokeDisp(gPlayerFour, PLAYER_FOUR, PLAYER_ONE);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDisp(gPlayerFive, PLAYER_FIVE, PLAYER_ONE);
        SmokeDisp(gPlayerSix, PLAYER_SIX, PLAYER_ONE);
        SmokeDisp(gPlayerSeven, PLAYER_SEVEN, PLAYER_ONE);
        SmokeDisp(gPlayerEight, PLAYER_EIGHT, PLAYER_ONE);
    }
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    for (i = 1; i < gPlayersToRenderCount; i++) {

        Pakartdma(buffer_car[i], gPlayersToRenderPlayerId[i],
                                       gPlayersToRenderScreenId[i], gPlayersToRenderScreenId[i],
                                       draw_number[gPlayersToRenderScreenId[i]][gPlayersToRenderPlayerId[i]]);

        decodeMIO0(
            (u8*) gEncodedKartTexture[draw_number[gPlayersToRenderScreenId[i - 1]][gPlayersToRenderPlayerId[i - 1]]]
                                     [gPlayersToRenderScreenId[i - 1]][gPlayersToRenderPlayerId[i - 1]]
                                         .unk_00,
            D_802BFB80
                .arraySize8[draw_number[gPlayersToRenderScreenId[i - 1]][gPlayersToRenderPlayerId[i - 1]]]
                           [gPlayersToRenderScreenId[i - 1]][gPlayersToRenderPlayerId[i - 1]]
                .pixel_index_array);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    }

    decodeMIO0((u8*) gEncodedKartTexture[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                                   [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                                        [gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                        [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                         [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                              [gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                              [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]
                   .pixel_index_array);
}

void CheckDMA2P(void) {
    s16 var_s0;

    Pakartdma(buffer_car[0], gPlayersToRenderPlayerId[0], gPlayersToRenderScreenId[0],
                                   gPlayersToRenderScreenId[0],
                                   draw_number[gPlayersToRenderScreenId[0]][gPlayersToRenderPlayerId[0]]);
    SmokeDisp2P(gPlayerOneCopy, PLAYER_ONE, PLAYER_TWO);
    SmokeDisp2P(gPlayerTwo, PLAYER_TWO, PLAYER_TWO);
    SmokeDisp2P(gPlayerThree, PLAYER_THREE, PLAYER_TWO);
    SmokeDisp2P(gPlayerFour, PLAYER_FOUR, PLAYER_TWO);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDisp2P(gPlayerFive, PLAYER_FIVE, PLAYER_TWO);
        SmokeDisp2P(gPlayerSix, PLAYER_SIX, PLAYER_TWO);
        SmokeDisp2P(gPlayerSeven, PLAYER_SEVEN, PLAYER_TWO);
        SmokeDisp2P(gPlayerEight, PLAYER_EIGHT, PLAYER_TWO);
    }
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    for (var_s0 = 1; var_s0 < gPlayersToRenderCount; var_s0++) {
        Pakartdma(buffer_car[var_s0], gPlayersToRenderPlayerId[var_s0],
                                       gPlayersToRenderScreenId[var_s0], gPlayersToRenderScreenId[var_s0],
                                       draw_number[gPlayersToRenderScreenId[var_s0]][gPlayersToRenderPlayerId[var_s0]]);
        decodeMIO0(
            (u8*) gEncodedKartTexture[draw_number[gPlayersToRenderScreenId[var_s0 - 1]]
                                                [gPlayersToRenderPlayerId[var_s0 - 1]]]
                                     [gPlayersToRenderScreenId[var_s0 - 1]][gPlayersToRenderPlayerId[var_s0 - 1]]
                                         .unk_00,
            D_802BFB80
                .arraySize8[draw_number[gPlayersToRenderScreenId[var_s0 - 1]][gPlayersToRenderPlayerId[var_s0 - 1]]]
                           [gPlayersToRenderScreenId[var_s0 - 1]][gPlayersToRenderPlayerId[var_s0 - 1]]
                .pixel_index_array);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    }
    decodeMIO0((u8*) gEncodedKartTexture[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                                   [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                                        [gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                        [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                         [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                              [gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                              [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]
                   .pixel_index_array);
}

void CheckDMA3P(void) {
    s16 var_s0;

    Pakartdma(buffer_car[0], gPlayersToRenderPlayerId[0] + 4,
                                   gPlayersToRenderScreenId[0], gPlayersToRenderScreenId[0] - 2,
                                   draw_number[gPlayersToRenderScreenId[0]][gPlayersToRenderPlayerId[0]]);
    SmokeDisp3P(gPlayerOneCopy, PLAYER_ONE, PLAYER_THREE);
    SmokeDisp3P(gPlayerTwo, PLAYER_TWO, PLAYER_THREE);
    SmokeDisp3P(gPlayerThree, PLAYER_THREE, PLAYER_THREE);
    SmokeDisp3P(gPlayerFour, PLAYER_FOUR, PLAYER_THREE);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    for (var_s0 = 1; var_s0 < gPlayersToRenderCount; var_s0++) {
        Pakartdma(buffer_car[var_s0], gPlayersToRenderPlayerId[var_s0] + 4,
                                       gPlayersToRenderScreenId[var_s0], gPlayersToRenderScreenId[var_s0] - 2,
                                       draw_number[gPlayersToRenderScreenId[var_s0]][gPlayersToRenderPlayerId[var_s0]]);
        decodeMIO0(
            (u8*) gEncodedKartTexture
                [draw_number[gPlayersToRenderScreenId[var_s0 - 1]][gPlayersToRenderPlayerId[var_s0 - 1]]]
                [gPlayersToRenderScreenId[var_s0 - 1] - 2][gPlayersToRenderPlayerId[var_s0 - 1] + 4]
                    .unk_00,
            D_802BFB80
                .arraySize8[draw_number[gPlayersToRenderScreenId[var_s0 - 1]][gPlayersToRenderPlayerId[var_s0 - 1]]]
                           [gPlayersToRenderScreenId[var_s0 - 1] - 2][gPlayersToRenderPlayerId[var_s0 - 1] + 4]
                .pixel_index_array);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    }
    decodeMIO0((u8*) gEncodedKartTexture[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                                   [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                                        [gPlayersToRenderScreenId[gPlayersToRenderCount - 1] - 2]
                                        [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1] + 4]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                         [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                              [gPlayersToRenderScreenId[gPlayersToRenderCount - 1] - 2]
                              [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1] + 4]
                   .pixel_index_array);
}

void CheckDMA4P(void) {
    s16 var_s0;

    Pakartdma(buffer_car[0], gPlayersToRenderPlayerId[0] + 4,
                                   gPlayersToRenderScreenId[0], gPlayersToRenderScreenId[0] - 2,
                                   draw_number[gPlayersToRenderScreenId[0]][gPlayersToRenderPlayerId[0]]);
    SmokeDisp4P(gPlayerOneCopy, PLAYER_ONE, PLAYER_FOUR);
    SmokeDisp4P(gPlayerTwo, PLAYER_TWO, PLAYER_FOUR);
    SmokeDisp4P(gPlayerThree, PLAYER_THREE, PLAYER_FOUR);
    SmokeDisp4P(gPlayerFour, PLAYER_FOUR, PLAYER_FOUR);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    for (var_s0 = 1; var_s0 < gPlayersToRenderCount; var_s0++) {
        Pakartdma(buffer_car[var_s0], gPlayersToRenderPlayerId[var_s0] + 4,
                                       gPlayersToRenderScreenId[var_s0], gPlayersToRenderScreenId[var_s0] - 2,
                                       draw_number[gPlayersToRenderScreenId[var_s0]][gPlayersToRenderPlayerId[var_s0]]);
        decodeMIO0(
            (u8*) gEncodedKartTexture
                [draw_number[gPlayersToRenderScreenId[var_s0 - 1]][gPlayersToRenderPlayerId[var_s0 - 1]]]
                [gPlayersToRenderScreenId[var_s0 - 1] - 2][gPlayersToRenderPlayerId[var_s0 - 1] + 4]
                    .unk_00,
            D_802BFB80
                .arraySize8[draw_number[gPlayersToRenderScreenId[var_s0 - 1]][gPlayersToRenderPlayerId[var_s0 - 1]]]
                           [gPlayersToRenderScreenId[var_s0 - 1] - 2][gPlayersToRenderPlayerId[var_s0 - 1] + 4]
                .pixel_index_array);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    }
    decodeMIO0((u8*) gEncodedKartTexture[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                                   [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                                        [gPlayersToRenderScreenId[gPlayersToRenderCount - 1] - 2]
                                        [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1] + 4]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[draw_number[gPlayersToRenderScreenId[gPlayersToRenderCount - 1]]
                                         [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1]]]
                              [gPlayersToRenderScreenId[gPlayersToRenderCount - 1] - 2]
                              [gPlayersToRenderPlayerId[gPlayersToRenderCount - 1] + 4]
                   .pixel_index_array);
}

void try_rendering_player(Player* player, s8 playerId, s8 arg2) {

    if (((player->flag & EXISTS) == EXISTS) && ((player->flag & IS_BOMB) == 0)) {
        if ((player->status & P1_DRAW_SPRITE << (arg2 * 4)) == P1_DRAW_SPRITE << (arg2 * 4)) {
            Doobj(player, playerId, arg2);
        }
    }
}

void DrawKart(void) {
    UNUSED s32 pad;
    UNUSED char* sp3C[8] = {
        "S_MARIO", "S_LUIZI", "S_YOSSY", "S_KINOP", "S_DONKY", "S_WARIO", "S_PEACH", "S_KUPPA",
    };
    UNUSED char* sp1C[8] = {
        "J_MARIO", "J_LUIZI", "J_YOSSY", "J_KINOP", "J_DONKY", "J_WARIO", "J_PEACH", "J_KUPPA",
    };

    gPlayersToRenderCount = 0;
    init_render_player(gPlayerOneCopy, camera1, PLAYER_ONE, PLAYER_ONE);
    init_render_player(gPlayerTwo, camera1, PLAYER_TWO, PLAYER_ONE);
    init_render_player(gPlayerThree, camera1, PLAYER_THREE, PLAYER_ONE);
    init_render_player(gPlayerFour, camera1, PLAYER_FOUR, PLAYER_ONE);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        init_render_player(gPlayerFive, camera1, PLAYER_FIVE, PLAYER_ONE);
        init_render_player(gPlayerSix, camera1, PLAYER_SIX, PLAYER_ONE);
        init_render_player(gPlayerSeven, camera1, PLAYER_SEVEN, PLAYER_ONE);
        init_render_player(gPlayerEight, camera1, PLAYER_EIGHT, PLAYER_ONE);
    }
    try_rendering_player(gPlayerOne, PLAYER_ONE, PLAYER_ONE);
    try_rendering_player(gPlayerTwo, PLAYER_TWO, PLAYER_ONE);
    try_rendering_player(gPlayerThree, PLAYER_THREE, PLAYER_ONE);
    try_rendering_player(gPlayerFour, PLAYER_FOUR, PLAYER_ONE);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        try_rendering_player(gPlayerFive, PLAYER_FIVE, PLAYER_ONE);
        try_rendering_player(gPlayerSix, PLAYER_SIX, PLAYER_ONE);
        try_rendering_player(gPlayerSeven, PLAYER_SEVEN, PLAYER_ONE);
        try_rendering_player(gPlayerEight, PLAYER_EIGHT, PLAYER_ONE);
    }
    if (gPlayersToRenderCount != 0) {
        CheckDMA();
    } else {
        SmokeDisp(gPlayerOneCopy, PLAYER_ONE, PLAYER_ONE);
        SmokeDisp(gPlayerTwo, PLAYER_TWO, PLAYER_ONE);
        SmokeDisp(gPlayerThree, PLAYER_THREE, PLAYER_ONE);
        SmokeDisp(gPlayerFour, PLAYER_FOUR, PLAYER_ONE);
        if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
            SmokeDisp(gPlayerFive, PLAYER_FIVE, PLAYER_ONE);
            SmokeDisp(gPlayerSix, PLAYER_SIX, PLAYER_ONE);
            SmokeDisp(gPlayerSeven, PLAYER_SEVEN, PLAYER_ONE);
            SmokeDisp(gPlayerEight, PLAYER_EIGHT, PLAYER_ONE);
        }
    }
    gPlayersToRenderCount = 0;
}

// Hello, you've found the location of the bulk of the data section for this file
// For reasons beyound human comprehension it MUST be placed somewhere below
// DrawKart

s32 junk[] = { 0, 0, 0 };

Vtx* KartVtx[] = { gPlayerOneVtx, gPlayerTwoVtx, gPlayerThreeVtx, gPlayerFourVtx, gPlayerFiveVtx, gPlayerSixVtx, gPlayerSevenVtx, gPlayerEightVtx };

f32 gCharacterSize[] = { MARIO_SIZE, LUIGI_SIZE, YOSHI_SIZE, TOAD_SIZE, DK_SIZE, WARIO_SIZE, PEACH_SIZE, BOWSER_SIZE };

u8** gKartMarioWheels0[] = { gKartMario168Wheel0, gKartMario147Wheel0, gKartMario126Wheel0,
                             gKartMario105Wheel0, gKartMario084Wheel0, gKartMario063Wheel0,
                             gKartMario042Wheel0, gKartMario021Wheel0, gKartMario000Wheel0 };

u8** gKartMarioWheels1[] = { gKartMario269Wheel0, gKartMario269Wheel0, gKartMario249Wheel0,
                             gKartMario229Wheel0, gKartMario229Wheel0, gKartMario229Wheel0,
                             gKartMario209Wheel0, gKartMario189Wheel0, gKartMario189Wheel0 };

u8** gKartLuigiWheels0[] = { gKartLuigi168Wheel0, gKartLuigi147Wheel0, gKartLuigi126Wheel0,
                             gKartLuigi105Wheel0, gKartLuigi084Wheel0, gKartLuigi063Wheel0,
                             gKartLuigi042Wheel0, gKartLuigi021Wheel0, gKartLuigi000Wheel0 };

u8** gKartLuigiWheels1[] = { gKartLuigi269Wheel0, gKartLuigi269Wheel0, gKartLuigi249Wheel0,
                             gKartLuigi229Wheel0, gKartLuigi229Wheel0, gKartLuigi229Wheel0,
                             gKartLuigi209Wheel0, gKartLuigi189Wheel0, gKartLuigi189Wheel0 };

u8** gKartBowserWheels0[] = { gKartBowser168Wheel0, gKartBowser147Wheel0, gKartBowser126Wheel0,
                              gKartBowser105Wheel0, gKartBowser084Wheel0, gKartBowser063Wheel0,
                              gKartBowser042Wheel0, gKartBowser021Wheel0, gKartBowser000Wheel0 };

u8** gKartBowserWheels1[] = { gKartBowser269Wheel0, gKartBowser269Wheel0, gKartBowser249Wheel0,
                              gKartBowser229Wheel0, gKartBowser229Wheel0, gKartBowser229Wheel0,
                              gKartBowser209Wheel0, gKartBowser189Wheel0, gKartBowser189Wheel0 };

u8** gKartToadWheels0[] = { gKartToad168Wheel0, gKartToad147Wheel0, gKartToad126Wheel0,
                            gKartToad105Wheel0, gKartToad084Wheel0, gKartToad063Wheel0,
                            gKartToad042Wheel0, gKartToad021Wheel0, gKartToad000Wheel0 };

u8** gKartToadWheels1[] = { gKartToad269Wheel0, gKartToad269Wheel0, gKartToad249Wheel0,
                            gKartToad229Wheel0, gKartToad229Wheel0, gKartToad229Wheel0,
                            gKartToad209Wheel0, gKartToad189Wheel0, gKartToad189Wheel0 };

u8** gKartYoshiWheels0[] = { gKartYoshi168Wheel0, gKartYoshi147Wheel0, gKartYoshi126Wheel0,
                             gKartYoshi105Wheel0, gKartYoshi084Wheel0, gKartYoshi063Wheel0,
                             gKartYoshi042Wheel0, gKartYoshi021Wheel0, gKartYoshi000Wheel0 };

u8** gKartYoshiWheels1[] = { gKartYoshi269Wheel0, gKartYoshi269Wheel0, gKartYoshi249Wheel0,
                             gKartYoshi229Wheel0, gKartYoshi229Wheel0, gKartYoshi229Wheel0,
                             gKartYoshi209Wheel0, gKartYoshi189Wheel0, gKartYoshi189Wheel0 };

u8** gKartDKWheels0[] = { gKartDK168Wheel0, gKartDK147Wheel0, gKartDK126Wheel0, gKartDK105Wheel0, gKartDK084Wheel0,
                          gKartDK063Wheel0, gKartDK042Wheel0, gKartDK021Wheel0, gKartDK000Wheel0 };

u8** gKartDKWheels1[] = { gKartDK269Wheel0, gKartDK269Wheel0, gKartDK249Wheel0, gKartDK229Wheel0, gKartDK229Wheel0,
                          gKartDK229Wheel0, gKartDK209Wheel0, gKartDK189Wheel0, gKartDK189Wheel0 };

u8** gKartPeachWheels0[] = { gKartPeach168Wheel0, gKartPeach147Wheel0, gKartPeach126Wheel0,
                             gKartPeach105Wheel0, gKartPeach084Wheel0, gKartPeach063Wheel0,
                             gKartPeach042Wheel0, gKartPeach021Wheel0, gKartPeach000Wheel0 };

u8** gKartPeachWheels1[] = { gKartPeach269Wheel0, gKartPeach269Wheel0, gKartPeach249Wheel0,
                             gKartPeach229Wheel0, gKartPeach229Wheel0, gKartPeach229Wheel0,
                             gKartPeach209Wheel0, gKartPeach189Wheel0, gKartPeach189Wheel0 };

u8** gKartWarioWheels0[] = { gKartWario168Wheel0, gKartWario147Wheel0, gKartWario126Wheel0,
                             gKartWario105Wheel0, gKartWario084Wheel0, gKartWario063Wheel0,
                             gKartWario042Wheel0, gKartWario021Wheel0, gKartWario000Wheel0 };

u8** gKartWarioWheels1[] = { gKartWario269Wheel0, gKartWario269Wheel0, gKartWario249Wheel0,
                             gKartWario229Wheel0, gKartWario229Wheel0, gKartWario229Wheel0,
                             gKartWario209Wheel0, gKartWario189Wheel0, gKartWario189Wheel0 };

u16** gKartWheels0[] = { (u16**) gKartMarioWheels0, (u16**) gKartLuigiWheels0, (u16**) gKartYoshiWheels0,
                         (u16**) gKartToadWheels0,  (u16**) gKartDKWheels0,    (u16**) gKartWarioWheels0,
                         (u16**) gKartPeachWheels0, (u16**) gKartBowserWheels0 };

u16** gKartWheels1[] = { (u16**) gKartMarioWheels1, (u16**) gKartLuigiWheels1, (u16**) gKartYoshiWheels1,
                         (u16**) gKartToadWheels1,  (u16**) gKartDKWheels1,    (u16**) gKartWarioWheels1,
                         (u16**) gKartPeachWheels1, (u16**) gKartBowserWheels1 };

s32 D_800DDE74[] = { 96, 128, 192, 256, 288, 384, 512, 544, 576 };

s32 maybeCompilerPadding = 0;

void DrawKart2P(void) {
    gPlayersToRenderCount = 0;
    init_render_player(gPlayerOneCopy, camera2, PLAYER_ONE, PLAYER_TWO);
    init_render_player(gPlayerTwo, camera2, PLAYER_TWO, PLAYER_TWO);
    init_render_player(gPlayerThree, camera2, PLAYER_THREE, PLAYER_TWO);
    init_render_player(gPlayerFour, camera2, PLAYER_FOUR, PLAYER_TWO);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        init_render_player(gPlayerFive, camera2, PLAYER_FIVE, PLAYER_TWO);
        init_render_player(gPlayerSix, camera2, PLAYER_SIX, PLAYER_TWO);
        init_render_player(gPlayerSeven, camera2, PLAYER_SEVEN, PLAYER_TWO);
        init_render_player(gPlayerEight, camera2, PLAYER_EIGHT, PLAYER_TWO);
    }
    try_rendering_player(gPlayerOne, PLAYER_ONE, PLAYER_TWO);
    try_rendering_player(gPlayerTwo, PLAYER_TWO, PLAYER_TWO);
    try_rendering_player(gPlayerThree, PLAYER_THREE, PLAYER_TWO);
    try_rendering_player(gPlayerFour, PLAYER_FOUR, PLAYER_TWO);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        try_rendering_player(gPlayerFive, PLAYER_FIVE, PLAYER_TWO);
        try_rendering_player(gPlayerSix, PLAYER_SIX, PLAYER_TWO);
        try_rendering_player(gPlayerSeven, PLAYER_SEVEN, PLAYER_TWO);
        try_rendering_player(gPlayerEight, PLAYER_EIGHT, PLAYER_TWO);
    }
    if (gPlayersToRenderCount != 0) {
        CheckDMA2P();
    } else {
        SmokeDisp2P(gPlayerOneCopy, PLAYER_ONE, PLAYER_TWO);
        SmokeDisp2P(gPlayerTwo, PLAYER_TWO, PLAYER_TWO);
        SmokeDisp2P(gPlayerThree, PLAYER_THREE, PLAYER_TWO);
        SmokeDisp2P(gPlayerFour, PLAYER_FOUR, PLAYER_TWO);
        if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
            SmokeDisp2P(gPlayerFive, PLAYER_FIVE, PLAYER_TWO);
            SmokeDisp2P(gPlayerSix, PLAYER_SIX, PLAYER_TWO);
            SmokeDisp2P(gPlayerSeven, PLAYER_SEVEN, PLAYER_TWO);
            SmokeDisp2P(gPlayerEight, PLAYER_EIGHT, PLAYER_TWO);
        }
    }
    gPlayersToRenderCount = 0;
}

void DrawKart3P(void) {
    gPlayersToRenderCount = 0;
    init_render_player(gPlayerOneCopy, camera3, PLAYER_ONE, PLAYER_THREE);
    init_render_player(gPlayerTwo, camera3, PLAYER_TWO, PLAYER_THREE);
    init_render_player(gPlayerThree, camera3, PLAYER_THREE, PLAYER_THREE);
    init_render_player(gPlayerFour, camera3, PLAYER_FOUR, PLAYER_THREE);
    try_rendering_player(gPlayerOne, PLAYER_ONE, PLAYER_THREE);
    try_rendering_player(gPlayerTwo, PLAYER_TWO, PLAYER_THREE);
    try_rendering_player(gPlayerThree, PLAYER_THREE, PLAYER_THREE);
    try_rendering_player(gPlayerFour, PLAYER_FOUR, PLAYER_THREE);
    if (gPlayersToRenderCount != 0) {
        CheckDMA3P();
    } else {
        SmokeDisp3P(gPlayerOneCopy, PLAYER_ONE, PLAYER_THREE);
        SmokeDisp3P(gPlayerTwo, PLAYER_TWO, PLAYER_THREE);
        SmokeDisp3P(gPlayerThree, PLAYER_THREE, PLAYER_THREE);
        SmokeDisp3P(gPlayerFour, PLAYER_FOUR, PLAYER_THREE);
    }
    gPlayersToRenderCount = 0;
}

void DrawKart4P(void) {
    gPlayersToRenderCount = 0;
    init_render_player(gPlayerOneCopy, camera4, PLAYER_ONE, PLAYER_FOUR);
    init_render_player(gPlayerTwo, camera4, PLAYER_TWO, PLAYER_FOUR);
    init_render_player(gPlayerThree, camera4, PLAYER_THREE, PLAYER_FOUR);
    init_render_player(gPlayerFour, camera4, PLAYER_FOUR, PLAYER_FOUR);
    try_rendering_player(gPlayerOne, PLAYER_ONE, PLAYER_FOUR);
    try_rendering_player(gPlayerTwo, PLAYER_TWO, PLAYER_FOUR);
    try_rendering_player(gPlayerThree, PLAYER_THREE, PLAYER_FOUR);
    try_rendering_player(gPlayerFour, PLAYER_FOUR, PLAYER_FOUR);
    if (gPlayersToRenderCount != 0) {
        CheckDMA4P();
    } else {
        SmokeDisp4P(gPlayerOneCopy, PLAYER_ONE, PLAYER_FOUR);
        SmokeDisp4P(gPlayerTwo, PLAYER_TWO, PLAYER_FOUR);
        SmokeDisp4P(gPlayerThree, PLAYER_THREE, PLAYER_FOUR);
        SmokeDisp4P(gPlayerFour, PLAYER_FOUR, PLAYER_FOUR);
    }
    gPlayersToRenderCount = 0;
}

void DrawKartAfter(void) {
    SmokeDispAfter(gPlayerOneCopy, 0, 0);
    SmokeDispAfter(gPlayerTwo, 1, 0);
    SmokeDispAfter(gPlayerThree, 2, 0);
    SmokeDispAfter(gPlayerFour, 3, 0);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDispAfter(gPlayerFive, 4, 0);
        SmokeDispAfter(gPlayerSix, 5, 0);
        SmokeDispAfter(gPlayerSeven, 6, 0);
        SmokeDispAfter(gPlayerEight, 7, 0);
    }
    if (gGamestate == ENDING) {
        if (gPlayerOne->handling_flag & TROPHYJUMP_KEEP) {
            draw_result_shadow(gPlayerOne, 0, 0);
        }
        if (gPlayerTwo->handling_flag & TROPHYJUMP_KEEP) {
            draw_result_shadow(gPlayerTwo, 1, 0);
        }
        if (gPlayerThree->handling_flag & TROPHYJUMP_KEEP) {
            draw_result_shadow(gPlayerThree, 2, 0);
        }
        if (gPlayerFour->handling_flag & TROPHYJUMP_KEEP) {
            draw_result_shadow(gPlayerFour, 3, 0);
        }
    }
}

void DrawKart2PAfter(void) {
    SmokeDispAfter2P(gPlayerOneCopy, 0, 1);
    SmokeDispAfter2P(gPlayerTwo, 1, 1);
    SmokeDispAfter2P(gPlayerThree, 2, 1);
    SmokeDispAfter2P(gPlayerFour, 3, 1);
    if (g_ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDispAfter2P(gPlayerFive, 4, 1);
        SmokeDispAfter2P(gPlayerSix, 5, 1);
        SmokeDispAfter2P(gPlayerSeven, 6, 1);
        SmokeDispAfter2P(gPlayerEight, 7, 1);
    }
}

void DrawKart3PAfter(void) {
    SmokeDispAfter3P(gPlayerOneCopy, 0, 2);
    SmokeDispAfter3P(gPlayerTwo, 1, 2);
    SmokeDispAfter3P(gPlayerThree, 2, 2);
    SmokeDispAfter3P(gPlayerFour, 3, 2);
}

void DrawKart4PAfter(void) {
    SmokeDispAfter4P(gPlayerOneCopy, 0, 3);
    SmokeDispAfter4P(gPlayerTwo, 1, 3);
    SmokeDispAfter4P(gPlayerThree, 2, 3);
    SmokeDispAfter4P(gPlayerFour, 3, 3);
}

void createaffine(Mat4 dest, Vec3f pos, Vec3s orientation) {
    UNUSED f32 pad[3];
    f32 sinX = sinT(orientation[0]);
    f32 cosX = cosT(orientation[0]);
    f32 sinY = sinT(orientation[1]);
    f32 cosY = cosT(orientation[1]);
    f32 sinZ = sinT(orientation[2]);
    f32 cosZ = cosT(orientation[2]);

    dest[0][0] = (cosY * cosZ) + ((sinX * sinY) * sinZ);
    dest[1][0] = (-cosY * sinZ) + ((sinX * sinY) * cosZ);
    dest[2][0] = cosX * sinY;
    dest[3][0] = pos[0];
    dest[0][1] = cosX * sinZ;
    dest[1][1] = cosX * cosZ;
    dest[2][1] = -sinX;
    dest[3][1] = pos[1];
    dest[0][2] = (-sinY * cosZ) + ((sinX * cosY) * sinZ);
    dest[1][2] = (sinY * sinZ) + ((sinX * cosY) * cosZ);
    dest[2][2] = cosX * cosY;
    dest[3][2] = pos[2];
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}

UNUSED void trans_matrix(Mat4 arg0, Vec3f arg1) {
    arg0[3][0] += arg1[0];
    arg0[3][1] += arg1[1];
    arg0[3][2] += arg1[2];
}

void scale_matrix(Mat4 arg0, f32 scale) {
    arg0[0][0] *= scale;
    arg0[1][0] *= scale;
    arg0[2][0] *= scale;
    arg0[0][1] *= scale;
    arg0[1][1] *= scale;
    arg0[2][1] *= scale;
    arg0[0][2] *= scale;
    arg0[1][2] *= scale;
    arg0[2][2] *= scale;
}

/**
 * This function writes a fixed-point value to each Mtx entry. This is not how the Mtx struct works.
 * The first half of Mtx only holds s16 whole numbers and the second half holds the s16 decimal (fractional) parts.
 * See Affine2Mtx() for correct calculations. Note that each Mtx entry is the size of s32.
 * This means each Mtx entry holds two s16 values.
 * The first sixteen entries contain only the integer parts and the second sixteen entries hold only the decimal
 * (fractional) parts.
 */
UNUSED void Affine2normMtx(Mtx* dest, Mat4 src) {
    f32 toFixed = 65536.0f;
    dest->m[0][0] = src[0][0] * toFixed;
    dest->m[0][1] = src[0][1] * toFixed;
    dest->m[0][2] = src[0][2] * toFixed;
    dest->m[0][3] = src[0][3] * toFixed;
    dest->m[1][0] = src[1][0] * toFixed;
    dest->m[1][1] = src[1][1] * toFixed;
    dest->m[1][2] = src[1][2] * toFixed;
    dest->m[1][3] = src[1][3] * toFixed;
    dest->m[2][0] = src[2][0] * toFixed;
    dest->m[2][1] = src[2][1] * toFixed;
    dest->m[2][2] = src[2][2] * toFixed;
    dest->m[2][3] = src[2][3] * toFixed;
    dest->m[3][0] = src[3][0] * toFixed;
    dest->m[3][1] = src[3][1] * toFixed;
    dest->m[3][2] = src[3][2] * toFixed;
    dest->m[3][3] = src[3][3] * toFixed;
}

/**
 * Takes a floating-point matrix and converts it to an s15.16 internal matrix.
 * Each Mtx entry is a size of s32 that holds two values.
 * The first 16 entries hold only the integer values and the second 16 entries hold only the decimal (fractional) parts.
 * In simpler words, the integer and decimal gets split up and stored in their own section.
 * Mtx is setup this way due to hardware restrictions of the n64 or as an optimization.
 *
 * @param Mtx A new internal fixed-point matrix.
 * @param Mat4 An array of f32
 * Mat4 to Mtx explanation: https://blarg.ca/2020/10/11/fixed-point-math.
 */
void Affine2Mtx(Mtx* dest, Mat4 src) {
#ifdef AVOID_UB
    // Use os function guMtxF2L instead. This helps little-endian systems.
    guMtxF2L(src, dest);
#else
    f32 toFixed = 65536.0f; // 2 ^ 16
    dest->m[0][0] = ((s32) (src[0][0] * toFixed) & 0xFFFF0000) | (((s32) (src[0][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[0][1] = ((s32) (src[0][2] * toFixed) & 0xFFFF0000) | (((s32) (src[0][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[0][2] = ((s32) (src[1][0] * toFixed) & 0xFFFF0000) | (((s32) (src[1][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[0][3] = ((s32) (src[1][2] * toFixed) & 0xFFFF0000) | (((s32) (src[1][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][0] = ((s32) (src[2][0] * toFixed) & 0xFFFF0000) | (((s32) (src[2][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][1] = ((s32) (src[2][2] * toFixed) & 0xFFFF0000) | (((s32) (src[2][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][2] = ((s32) (src[3][0] * toFixed) & 0xFFFF0000) | (((s32) (src[3][1] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[1][3] = ((s32) (src[3][2] * toFixed) & 0xFFFF0000) | (((s32) (src[3][3] * toFixed) >> 0x10) & 0xFFFF);
    dest->m[2][0] = ((s32) (src[0][0] * toFixed) << 0x10) | ((s32) (src[0][1] * toFixed) & 0xFFFF);
    dest->m[2][1] = ((s32) (src[0][2] * toFixed) << 0x10) | ((s32) (src[0][3] * toFixed) & 0xFFFF);
    dest->m[2][2] = ((s32) (src[1][0] * toFixed) << 0x10) | ((s32) (src[1][1] * toFixed) & 0xFFFF);
    dest->m[2][3] = ((s32) (src[1][2] * toFixed) << 0x10) | ((s32) (src[1][3] * toFixed) & 0xFFFF);
    dest->m[3][0] = ((s32) (src[2][0] * toFixed) << 0x10) | ((s32) (src[2][1] * toFixed) & 0xFFFF);
    dest->m[3][1] = ((s32) (src[2][2] * toFixed) << 0x10) | ((s32) (src[2][3] * toFixed) & 0xFFFF);
    dest->m[3][2] = ((s32) (src[3][0] * toFixed) << 0x10) | ((s32) (src[3][1] * toFixed) & 0xFFFF);
    dest->m[3][3] = ((s32) (src[3][2] * toFixed) << 0x10) | ((s32) (src[3][3] * toFixed) & 0xFFFF);
#endif
}

bool ChaseDir(s16* angle, s16 targetAngle, s16 step) {
    s16 temp_v0;

    temp_v0 = targetAngle - *angle;
    if (step < 0) { // abs of step
        step *= -1;
    }

    if (temp_v0 > 0) {
        temp_v0 -= step;
        if (temp_v0 >= 0) {
            *angle = targetAngle - temp_v0;
        } else {
            *angle = targetAngle;
        }
    } else {
        temp_v0 += step;
        if (temp_v0 <= 0) {
            *angle = targetAngle - temp_v0;
        } else {
            *angle = targetAngle;
        }
    }
    if (targetAngle == *angle) {
        return false;
    }
    return true;
}

void chase_Inumber(s32* startingValue, s32 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

/**
  * Function: chase_Fnumber

  * Parameters:
  *     f32 *startingValue - Pointer to the float that will be modified
  *     f32 targetValue    - Float value to move startingValue towards
  *     f32 somePercent    - The percent of the difference between startingValue
                            and targetValue to actually move

  * Moves a given startingValue the given somePercent towards the targetValue

  * f32 *thing = 2500;
  * chase_Fnumber(thing, 500, 0.75f);
  * thing now has a value of 1000

  * If after the move startingValue is inside of the range [-0.001, 0.001],
  * force it to exactly 0.0f

  * This is probably a precision thing. The scaling with somePercent likely
  * can't hit exactly 0 with any reliability, so they force it to 0 if you're
  * in a small range around it. Why they only do this for 0 is anyone's guess though
**/
void chase_Fnumber(f32* startingValue, f32 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
    if ((*startingValue < 0.001) && (-0.001 < *startingValue)) {
        *startingValue = 0.0f;
    }
}

void chase_Snumber(s16* startingValue, s16 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

void chase_USnumber(u16* startingValue, s16 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

void ken_startegy(void) {
    kart_effect_strategy();
    vtx_strategy();
}

void vtx_strategy(void) {
    switch (g_ScreenSplitA) { /* irregular */
        case SCREEN_MODE_1P:
            switch (g_gameMode) { /* switch 1; irregular */
                case GRAND_PRIX:      /* switch 1 */
                    vtx_calc(gPlayerOne, 0);
                    vtx_calc(gPlayerTwo, 1);
                    vtx_calc(gPlayerThree, 2);
                    vtx_calc(gPlayerFour, 3);
                    vtx_calc(gPlayerFive, 4);
                    vtx_calc(gPlayerSix, 5);
                    vtx_calc(gPlayerSeven, 6);
                    vtx_calc(gPlayerEight, 7);
                    break;
                case TIME_TRIALS: /* switch 1 */
                    vtx_calc(gPlayerOne, 0);
                    if ((gPlayerTwo->flag & IS_GHOST) == IS_GHOST) {
                        vtx_calc(gPlayerTwo, 1);
                    }
                    if ((gPlayerThree->flag & IS_GHOST) == IS_GHOST) {
                        vtx_calc(gPlayerThree, 2);
                    }
                    break;
                case VERSUS: /* switch 1 */
                case BATTLE: /* switch 1 */
                    vtx_calc(gPlayerOne, 0);
                    vtx_calc(gPlayerTwo, 1);
                    if (g_playerCount >= 3) {
                        vtx_calc(gPlayerThree, 2);
                    }
                    if (g_playerCount == 4) {
                        vtx_calc(gPlayerFour, 3);
                    }
                    break;
            }
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            switch (g_gameMode) { /* switch 2; irregular */
                case GRAND_PRIX:      /* switch 2 */
                    vtx_calc(gPlayerOne, 0);
                    vtx_calc(gPlayerTwo, 1);
                    vtx_calc(gPlayerThree, 2);
                    vtx_calc(gPlayerFour, 3);
                    vtx_calc(gPlayerFive, 4);
                    vtx_calc(gPlayerSix, 5);
                    vtx_calc(gPlayerSeven, 6);
                    vtx_calc(gPlayerEight, 7);
                    break;
                case VERSUS: /* switch 2 */
                case BATTLE: /* switch 2 */
                    vtx_calc(gPlayerOne, 0);
                    vtx_calc(gPlayerTwo, 1);
                    break;
                case TIME_TRIALS: /* switch 2 */
                    vtx_calc(gPlayerOne, 0);
                    break;
            }
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if ((VERSUS == g_gameMode) || (BATTLE == g_gameMode)) {
                vtx_calc(gPlayerOne, 0);
                vtx_calc(gPlayerTwo, 1);
                vtx_calc(gPlayerThree, 2);
                if (g_playerCount == 4) {
                    vtx_calc(gPlayerFour, 3);
                }
            }
            break;
    }
}

void vtx_calc(Player* player, s8 playerIndex) {
    if ((player->flag & EXISTS) == EXISTS) {
        change_tirecol_calc(player, playerIndex);
        bright_calc(player, playerIndex);
        if (((player->slip_flag & BROKEN) == BROKEN) ||
            ((player->slip_flag & RESTORE) == RESTORE)) {
            if ((player->slip_flag & BROKEN) == BROKEN) {
                broken_calc(player, playerIndex);
            }
            if ((player->slip_flag & RESTORE) == RESTORE) {
                restore_calc(player, playerIndex);
            }
        } else {
            bound_calc(player, playerIndex);
        }
        shake_set(player);
    }
}

void broken_calc(Player* player, UNUSED s8 playerIndex) {
    f32 temp_f0;
    s16 var_v0;

    var_v0 = player->sus.broken_timer;
    temp_f0 = player->sus.broken_firstspeed;
    if (var_v0 < 5) {
        var_v0++;
    }

    player->sus.broken = (var_v0 * temp_f0) - (0.7 * (var_v0 * var_v0));
    player->sus.broken_firstspeed = temp_f0;
    player->sus.broken_timer = var_v0;
}

void restore_calc(Player* player, UNUSED s8 playerIndex) {
    f32 temp_f0 = player->sus.broken_firstspeed;
    s16 temp_v0 = player->sus.broken_timer;
    s16 temp_f16;

    temp_v0++;

    temp_f16 = (temp_v0 * temp_f0) - (0.5 * (temp_v0 * temp_v0));

    if ((temp_v0 != 0) && (temp_f16 < 0)) {
        temp_f0 *= 0.8;
        temp_v0 = 0;
        if (temp_f0 <= 0.1) {
            player->slip_flag &= ~RESTORE;
            temp_f0 = 0.0f;
        }
    }
    if (temp_f16 <= 0) {
        temp_f16 = 0;
    }
    player->sus.broken = temp_f16;
    player->sus.broken_firstspeed = temp_f0;
    player->sus.broken_timer = temp_v0;
}

// look like related to flatten animation
void broken_set(Player* player, s8 playerId, UNUSED s8 screenId, s8 flipOffset) {
    s16 temp_v0 = player->sus.broken;

    KartVtx[playerId][flipOffset + 0x0].v.ob[1] = 18 - (temp_v0 * 2.3);
    KartVtx[playerId][flipOffset + 0x1].v.ob[1] = 9 - temp_v0;
    KartVtx[playerId][flipOffset + 0x2].v.ob[1] = 9 - temp_v0;
    KartVtx[playerId][flipOffset + 0x3].v.ob[1] = 18 - (temp_v0 * 2.3);
    KartVtx[playerId][flipOffset + 0x4].v.ob[1] = 9 - temp_v0;
    KartVtx[playerId][flipOffset + 0x7].v.ob[1] = 9 - temp_v0;
}

/**
 * Seems to stretch/warp a specific players texture for a
 * short period of time. Perhaps does not do anything
 **/
void jugemu_set(UNUSED Player* player, s8 playerId, UNUSED s8 screenId, s8 flipOffset) {
    KartVtx[playerId][flipOffset].v.ob[1] = 21;
    KartVtx[playerId][flipOffset + 0x3].v.ob[1] = 21;
}

void bound_calc(Player* player, UNUSED s8 playerIndex) {
    f32 temp_f0 = player->sus.bound_firstspeed;
    s16 temp_v0 = player->sus.bound_timer;
    s16 temp_f16;

    temp_v0++;

    temp_f16 = (temp_v0 * temp_f0) - (0.7 * (temp_v0 * temp_v0));

    if ((temp_v0 != 0) && (temp_f16 < 0)) {
        temp_f0 *= 0.8;
        temp_v0 = 0;
        if (temp_f0 <= 0.1) {
            temp_f0 = 0.0f;
        }
    }
    if (temp_f16 <= 0) {
        temp_f16 = 0;
    }
    player->sus.bound = temp_f16;
    player->sus.bound_firstspeed = temp_f0;
    player->sus.bound_timer = temp_v0;
}

void bound_set(Player* player, s8 playerId, UNUSED s8 screenId, s8 flipOffset) {
    s16 temp_v0 = player->sus.bound;

    KartVtx[playerId][flipOffset + 0x0].v.ob[1] = 18 - temp_v0;
    KartVtx[playerId][flipOffset + 0x1].v.ob[1] = 9 - temp_v0;
    KartVtx[playerId][flipOffset + 0x2].v.ob[1] = 9 - temp_v0;
    KartVtx[playerId][flipOffset + 0x3].v.ob[1] = 18 - temp_v0;
    KartVtx[playerId][flipOffset + 0x4].v.ob[1] = 9 - temp_v0;
    KartVtx[playerId][flipOffset + 0x7].v.ob[1] = 9 - temp_v0;
}

/**
 * Sets player shading/colour.
 */
void MakeBodyColor(UNUSED Player* player, s8 playerIndex, s32 arg2, f32 alpha) {
    g_colorPlayer0R[playerIndex] =
        (s16) ((f32) g_colorPlayer0R[playerIndex] - ((g_colorPlayer0R[playerIndex] - ((arg2 >> 16) & 0xFF)) * alpha));

    bright_g[playerIndex] =
        (s16) ((f32) bright_g[playerIndex] - ((bright_g[playerIndex] - ((arg2 >> 8) & 0xFF)) * alpha));

    bright_b[playerIndex] =
        (s16) ((f32) bright_b[playerIndex] - ((bright_b[playerIndex] - (arg2 & 0xFF)) * alpha));
}

void MakeBodyColorAdjust(UNUSED Player* player, s8 playerIndex, s32 arg2, f32 arg3) {
    chase_USnumber(&adjust_r[playerIndex], (arg2 >> 16) & 0xFF, arg3);
    chase_USnumber(&adjust_g[playerIndex], (arg2 >> 8) & 0xFF, arg3);
    chase_USnumber(&adjust_b[playerIndex], arg2 & 0xFF, arg3);
}

/**
 * Activates in the tunnel to shade the player a bit darker
 * Sort of an atmospheric effect.
 */
bool check_map_bright_before(Player* player, s8 playerIndex) {
    switch (g_courseID) {
        case COURSE_LUIGI_RACEWAY:
            if (((gNearestPathPointByPlayerId[playerIndex] >= 0x14F) && (gNearestPathPointByPlayerId[playerIndex] < 0x158)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x15E) && (gNearestPathPointByPlayerId[playerIndex] < 0x164)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x169) && (gNearestPathPointByPlayerId[playerIndex] < 0x170)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x174) && (gNearestPathPointByPlayerId[playerIndex] < 0x17A)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x17E) &&
                 (gNearestPathPointByPlayerId[playerIndex] < 0x184))) { // under a light in the tunnel
                MakeBodyColor(player, playerIndex, COLOR_LIGHT, 0.3f);
                MakeBodyColorAdjust(player, playerIndex, 0xE0, 0.3f);
                color_timer[playerIndex] = 0;
                return true;
            }
            return false;

        default:
            return false;
    }
}

void check_map_bright_after(Player* player, s8 playerIndex) {
    switch (g_courseID) {
        case COURSE_BOWSER_CASTLE:
            if (((gNearestPathPointByPlayerId[playerIndex] >= 0x15) && (gNearestPathPointByPlayerId[playerIndex] < 0x2A)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x14D) && (gNearestPathPointByPlayerId[playerIndex] < 0x15C)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x1D1) && (gNearestPathPointByPlayerId[playerIndex] < 0x1E4)) ||
                (player->bump.distance_zx >= 500.0f)) { // over lava
                MakeBodyColor(player, playerIndex, COLOR_LAVA, 0.3f);
                MakeBodyColorAdjust(player, playerIndex, 0x004040, 0.3f);
                color_timer[playerIndex] = 0;
            } else if (((gNearestPathPointByPlayerId[playerIndex] >= 0xF1) && (gNearestPathPointByPlayerId[playerIndex] < 0xF5)) ||
                       ((gNearestPathPointByPlayerId[playerIndex] >= 0xFB) && (gNearestPathPointByPlayerId[playerIndex] < 0xFF)) ||
                       ((gNearestPathPointByPlayerId[playerIndex] >= 0x105) && (gNearestPathPointByPlayerId[playerIndex] < 0x109)) ||
                       ((gNearestPathPointByPlayerId[playerIndex] >= 0x10F) && (gNearestPathPointByPlayerId[playerIndex] < 0x113)) ||
                       ((gNearestPathPointByPlayerId[playerIndex] >= 0x145) && (gNearestPathPointByPlayerId[playerIndex] < 0x14A)) ||
                       ((gNearestPathPointByPlayerId[playerIndex] >= 0x15E) &&
                        (gNearestPathPointByPlayerId[playerIndex] < 0x163))) { // under a lamp
                MakeBodyColor(player, playerIndex, COLOR_LIGHT, 0.3f);
                MakeBodyColorAdjust(player, playerIndex, 0xE0, 0.3f);
                color_timer[playerIndex] = 0;
            } else { // normal color
                MakeBodyColor(player, playerIndex, COLOR_BLACK, 0.3f);
                MakeBodyColorAdjust(player, playerIndex, 0, 0.3f);
                color_timer[playerIndex] = 0;
            }
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (((gNearestPathPointByPlayerId[playerIndex] >= 0xD) && (gNearestPathPointByPlayerId[playerIndex] < 0x15)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x29) && (gNearestPathPointByPlayerId[playerIndex] < 0x39)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x46) && (gNearestPathPointByPlayerId[playerIndex] < 0x4E)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x5F) && (gNearestPathPointByPlayerId[playerIndex] < 0x67)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x7B) && (gNearestPathPointByPlayerId[playerIndex] < 0x86)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x9D) && (gNearestPathPointByPlayerId[playerIndex] < 0xA6)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0xB9) && (gNearestPathPointByPlayerId[playerIndex] < 0xC3)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0xB9) && (gNearestPathPointByPlayerId[playerIndex] < 0xC3)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0xD7) && (gNearestPathPointByPlayerId[playerIndex] < 0xE1)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x10E) && (gNearestPathPointByPlayerId[playerIndex] < 0x119)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x154) && (gNearestPathPointByPlayerId[playerIndex] < 0x15F)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x1EF) && (gNearestPathPointByPlayerId[playerIndex] < 0x1F7)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x202) && (gNearestPathPointByPlayerId[playerIndex] < 0x209)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x216) && (gNearestPathPointByPlayerId[playerIndex] < 0x21D)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x230) && (gNearestPathPointByPlayerId[playerIndex] < 0x23A)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x24C) && (gNearestPathPointByPlayerId[playerIndex] < 0x256)) ||
                // This is the second last lamp. This condition is bugged and it does not glow on the player.
                // The value 0x228 was probably meant to be 0x25F.
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x288) && (gNearestPathPointByPlayerId[playerIndex] < 0x269)) ||
                ((gNearestPathPointByPlayerId[playerIndex] >= 0x274) &&
                 (gNearestPathPointByPlayerId[playerIndex] < 0x27E))) { // under a lamp
                MakeBodyColor(player, playerIndex, COLOR_LIGHT, 0.3f);
                MakeBodyColorAdjust(player, playerIndex, 0x0000E0, 0.3f);
                color_timer[playerIndex] = 0;
            } else {
                MakeBodyColor(player, playerIndex, COLOR_BLACK, 0.3f);
                MakeBodyColorAdjust(player, playerIndex, 0, 0.3f);
                color_timer[playerIndex] = 0;
            }
            break;
        default:
            MakeBodyColor(player, playerIndex, COLOR_BLACK, 0.3f);
            MakeBodyColorAdjust(player, playerIndex, 0, 0.3f);
            color_timer[playerIndex] = 0;
            break;
    }
}

void bright_calc(Player* player, s8 playerIndex) {
    s32 time_elapsed;

    if (((player->flag & IS_GHOST) == IS_GHOST) && (player == gPlayerThree)) {
        MakeBodyColor(player, playerIndex, COLOR_LIGHT, 0.3f);
        MakeBodyColorAdjust(player, playerIndex, 0xE0, 0.3f);
        color_timer[playerIndex] = 0;
        return;
    }

    if (((player->jugemu_flag & IS_FROZEN) == IS_FROZEN) &&
        ((player->jugemu_flag & IS_FADING_OUT) == IS_FADING_OUT)) {
        MakeBodyColor(player, playerIndex, 0x646464, 0.5f);
        MakeBodyColorAdjust(player, playerIndex, 0xFF0000, 0.1f);
        return;
    }
    if ((player->jugemu_flag & IS_FADING_OUT) == IS_FADING_OUT) {
        MakeBodyColor(player, playerIndex, COLOR_BLACK, 1.0f);
        MakeBodyColorAdjust(player, playerIndex, 0, 1.0f);
        return;
    }
    if ((player->jugemu_flag & IS_FROZEN) == IS_FROZEN) {
        MakeBodyColor(player, playerIndex, 0x646464, 0.5f);
        MakeBodyColorAdjust(player, playerIndex, 0xFF0000, 0.1f);
        return;
    }
    if ((player->jugemu_flag & IS_MELTING) == IS_MELTING) {
        MakeBodyColor(player, playerIndex, COLOR_BLACK, 0.1f);
        MakeBodyColorAdjust(player, playerIndex, 0, 0.1f);
        return;
    }

    if (((player->slip_flag & THUNDER) == THUNDER) && ((s32) player->thunder_timer < 0x78)) {
        color_timer[playerIndex] += 5;
        if (color_timer[playerIndex] >= 0x1E) {
            color_timer[playerIndex] = 0;
        }
        if ((color_timer[playerIndex] >= 0) && (color_timer[playerIndex] < 0xB)) {
            MakeBodyColor(player, playerIndex, 0x808080, 0.8f);
            MakeBodyColorAdjust(player, playerIndex, 0, 0.8f);
        }
        if ((color_timer[playerIndex] >= 0xB) && (color_timer[playerIndex] < 0x15)) {
            MakeBodyColor(player, playerIndex, 0x70, 0.8f);
            MakeBodyColorAdjust(player, playerIndex, 0, 0.8f);
        }
        if ((color_timer[playerIndex] >= 0x15) && (color_timer[playerIndex] < 0x1F)) {
            MakeBodyColor(player, playerIndex, 0x8F8F00, 0.8f);
            MakeBodyColorAdjust(player, playerIndex, 0, 0.8f);
        }
        return;
    }
    if ((player->slip_flag & STAR) != 0) {
        time_elapsed = (s32) g_gameTimer - g_StarUseCounter[playerIndex];
        if (time_elapsed <= 8) {

            if (time_elapsed >= 7) {
                color_timer[playerIndex] += 10;
            } else {
                color_timer[playerIndex] += 5;
            }
            if (color_timer[playerIndex] >= 40) {
                color_timer[playerIndex] = 0;
            }
            if ((color_timer[playerIndex] >= 0) && (color_timer[playerIndex] <= 10)) {
                MakeBodyColor(player, playerIndex, 0x70, 0.8f);
                MakeBodyColorAdjust(player, playerIndex, 0, 0.8f);
            }
            if ((color_timer[playerIndex] >= 0xB) && (color_timer[playerIndex] <= 20)) {
                MakeBodyColor(player, playerIndex, 0x707000, 0.8f);
                MakeBodyColorAdjust(player, playerIndex, 0, 0.8f);
            }
            if ((color_timer[playerIndex] >= 0x15) && (color_timer[playerIndex] <= 30)) {
                MakeBodyColor(player, playerIndex, 0x700000, 0.8f);
                MakeBodyColorAdjust(player, playerIndex, 0, 0.8f);
            }
            if (color_timer[playerIndex] >= 0x1F) {
                MakeBodyColor(player, playerIndex, 0x7000, 0.8f);
                MakeBodyColorAdjust(player, playerIndex, 0, 0.8f);
            }
            return;
        }
    }
    if (check_map_bright_before(player, playerIndex) != true) {
        if (((player->tire_RR.Dummy & 1) == 1) || ((player->tire_RR.Dummy & 2) == 2) ||
            ((player->tire_FL.Dummy & 3) == 3)) {
            MakeBodyColor(player, playerIndex, COLOR_BLACK, 0.3f);
            MakeBodyColorAdjust(player, playerIndex, 0x6F6F6F, 0.3f);
            return;
        }
        check_map_bright_after(player, playerIndex);
        if ((player->jugemu_flag & LAVA_EFFECT) == LAVA_EFFECT) {
            MakeBodyColor(player, playerIndex, COLOR_BLACK, 0.3f);
            MakeBodyColorAdjust(player, playerIndex, 0xF0F0F0, 0.3f);
        }
    }
}

void check_vtx(Player* player, s8 playerId, s8 screenId, s8 arg3) {
    if (((player->slip_flag & BROKEN) == BROKEN) ||
        ((player->slip_flag & RESTORE) == RESTORE)) {
        broken_set(player, playerId, screenId, arg3);
    } else {
        bound_set(player, playerId, screenId, arg3);
    }
    if ((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) {
        jugemu_set(player, playerId, screenId, arg3);
    }
}

void draw_shadow(Player* player, s8 playerId, s8 screenId) {
    Mat4 sp118;
    UNUSED Mat4 pad;
    Vec3f spCC;
    Vec3s spC4;
    s16 temp_t9;
    s16 spC0;
    Vec3f spB4;
    f32 spB0;
    f32 spAC;
    UNUSED Vec3f pad2;
    f32 var_f2;

    temp_t9 = (u16) (player->angle_y[screenId] + player->direction[1] + player->slipang) / 128; // << 7) & 0xFFFF;
    spC0 = -player->direction[1] - player->slipang;

    spB0 = -cosT(temp_t9 << 7) * 2;
    spAC = -sinT(temp_t9 << 7) * 2;

    if (((player->slip_flag & EXPLODE) == EXPLODE) ||
        ((player->slip_flag & ROLLOVER) == ROLLOVER) ||
        ((player->slip_flag & WING) == WING) ||
        ((player->slip_flag & STORM) == STORM) ||
        ((player->slip_flag & ROLLOVER) == ROLLOVER) ||
        ((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) ||
        ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL) ||
        ((player->slip_flag & N_JUMP) == N_JUMP)) {

        var_f2 = (f32) (1.0 - ((f64) player->bump.distance_zx * 0.02));
        if (var_f2 < 0.0f) {
            var_f2 = 0.0f;
        }
        if (var_f2 > 1.0f) {
            var_f2 = 1.0f;
        }
        spB4[0] = player->bump.bump_zx[0];
        spB4[2] = player->bump.bump_zx[2];
        spB4[1] = player->bump.bump_zx[1];

        spCC[0] = player->position[0] + ((spB0 * sinT(spC0)) + (spAC * cosT(spC0)));
        spCC[1] = player->ground + 1.0f;
        spCC[2] = player->position[2] + ((spB0 * cosT(spC0)) - (spAC * sinT(spC0)));
        kwcreate_rotation_matrix(sp118, spB4, spCC, (spC0 + player->kage_ang),
                             gCharacterSize[player->kart] * player->offsetsize * var_f2);
    } else {
        spC4[0] = player->kartpitch;
        spC4[1] = spC0;
        spC4[2] = player->kartroll * 2;

        spCC[0] = player->position[0] + ((spB0 * sinT(spC0)) + (spAC * cosT(spC0)));
        spCC[1] = player->ground + 1.0f;
        spCC[2] = player->position[2] + ((spB0 * cosT(spC0)) - (spAC * sinT(spC0)));
        createaffine(sp118, spCC, spC4);
        scale_matrix(sp118, gCharacterSize[player->kart] * player->offsetsize);
    }
    Affine2Mtx(&gDynamicP->mtxShadow[playerId + (screenId * 8)], sp118);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxShadow[playerId + (screenId * 8)]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(gDisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(gDisplayListHead++, kage_bitmap, G_IM_FMT_I, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    SplashMode(0, 0, 0, 0xFF);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, &D_800E51D0[0], 4, 0);

    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gDPLoadTextureBlock(gDisplayListHead++, (kage_bitmap + SOME_TEXTURE_POINTER_MATH), G_IM_FMT_I,
                        G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    SplashMode(0, 0, 0, 0xFF);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, &D_800E5210[0], 4, 0);

    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void draw_result_shadow(Player* player, s8 playerId, s8 arg2) {
    Mat4 sp118;
    UNUSED Mat4 pad;
    Vec3f spCC;
    Vec3s spC4;
    s16 temp_t9;
    s16 spC0;
    UNUSED Vec3f pad2;
    f32 spB0;
    f32 spAC;
    UNUSED Vec3f pad3;
    Vec3f sp94 = { 9.0f, 7.0f, 5.0f };

    temp_t9 = (u16) (player->angle_y[arg2] + player->direction[1] + player->slipang) / 128;
    spC0 = -player->direction[1] - player->slipang;

    spB0 = -cosT(temp_t9 << 7) * 3;
    spAC = -sinT(temp_t9 << 7) * 3;

    spC4[0] = 0;
    spC4[1] = spC0;
    spC4[2] = 0;

    spCC[0] = player->position[0] + ((spB0 * sinT(spC0)) + (spAC * cosT(spC0)));
    spCC[2] = player->position[2] + ((spB0 * cosT(spC0)) - (spAC * sinT(spC0)));
    spCC[1] = g_DynamicObjects[objallocptr[playerId]].pos[1] + sp94[playerId];

    createaffine(sp118, spCC, spC4);
    scale_matrix(sp118, gCharacterSize[player->kart] * player->offsetsize);
    Affine2Mtx(&gDynamicP->mtxShadow[playerId + (arg2 * 8)], sp118);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxShadow[playerId + (arg2 * 8)]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(gDisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(gDisplayListHead++, kage_bitmap, G_IM_FMT_I, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    SplashMode(0, 0, 0, 0x000000D0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, &D_800E51D0[0], 4, 0);

    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gDPLoadTextureBlock(gDisplayListHead++, (kage_bitmap + SOME_TEXTURE_POINTER_MATH), G_IM_FMT_I,
                        G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    SplashMode(0, 0, 0, 0x000000D0);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(gDisplayListHead++, &D_800E5210[0], 4, 0);

    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void draw_mainkart(Player* player, s8 playerId, s8 arg2, s8 flipOffset) {
    UNUSED s32 pad;
    Mat4 sp1A4;
    UNUSED s32 pad2[17];
    Vec3f sp154;
    Vec3s sp14C;
    f32 sp148;
    f32 sp144;
    f32 sp140;
    s16 temp_v1;
    s16 thing;

    if (player->handling_flag & TROPHYJUMP_KEEP) {
        sp14C[0] = 0;
        sp14C[1] = player->angle_y[arg2];
        sp14C[2] = 0;
        make_smokepos(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->angle_y[arg2], player->angle_z[arg2]);
        sp154[1] = (player->position[1] - player->radius) + (sp144 - 2.0);
        sp154[0] = player->position[0] + sp148;
        sp154[2] = player->position[2] + sp140;
    } else {
        thing = (u16) (player->angle_y[arg2] + player->direction[1] + player->slipang);
        temp_v1 = player->roll[arg2] * sinT(thing);
        if ((player->slip_flag & N_JUMP) == N_JUMP) {
            sp14C[0] = cameras[arg2].camera_direction[0] - DEGREES(90);
        } else {
            sp14C[0] = -temp_v1 * 0.8;
        }
        sp14C[1] = player->angle_y[arg2];
        sp14C[2] = player->angle_z[arg2];
        if (((s32) player->slip_flag & BROKEN) == BROKEN) {
            make_smokepos(&sp148, &sp144, &sp140, 0.0f, 8.0f, 0.0f, -player->angle_y[arg2], player->angle_z[arg2]);
            sp154[1] = (player->position[1] - player->radius) + player->shake;
            sp154[0] = player->position[0] + sp148;
            sp154[2] = player->position[2] + sp140;
        } else {
            make_smokepos(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -player->angle_y[arg2], player->angle_z[arg2]);
            sp154[1] = (player->position[1] - player->radius) + player->shake + (sp144 - 2.0);
            sp154[0] = player->position[0] + sp148;
            sp154[2] = player->position[2] + sp140;
        }
    }
#ifdef AVOID_UB
    gPlayerPalette = &gPlayerPalettesList[draw_number[arg2][playerId]][arg2][playerId];
#else
    gPlayerPalette = (struct_D_802F1F80*) &gPlayerPalettesList[draw_number[arg2][playerId]][arg2][playerId * 0x100];
#endif
    if ((arg2 == 0) || (arg2 == 1)) {
        sKartUpperTexture = &D_802BFB80.arraySize8[draw_number[arg2][playerId]][arg2][playerId].pixel_index_array[0];
        sKartLowerTexture = &D_802BFB80.arraySize8[draw_number[arg2][playerId]][arg2][playerId].pixel_index_array[0x7C0];
    } else {
        sKartUpperTexture =
            &D_802BFB80.arraySize8[draw_number[arg2][playerId]][arg2 - 1][playerId - 4].pixel_index_array[0];
        sKartLowerTexture =
            &D_802BFB80.arraySize8[draw_number[arg2][playerId]][arg2 - 1][playerId - 4].pixel_index_array[0x7C0];
    }
    createaffine(sp1A4, sp154, sp14C);
    scale_matrix(sp1A4, gCharacterSize[player->kart] * player->offsetsize);
    Affine2Mtx(&gDynamicP->mtxKart[playerId + (arg2 * 8)], sp1A4);

    if ((player->slip_flag & TERESA) == TERESA) {
        if (arg2 == playerId) {
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxKart[playerId + (arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, common_setting_render_character);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            IceMode(g_colorPlayer0R[playerId], bright_g[playerId], bright_b[playerId],
                          adjust_r[playerId], adjust_g[playerId], adjust_b[playerId],
                          (s32) player->erase);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        } else {
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxKart[playerId + (arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(gDisplayListHead++, common_setting_render_character);
            gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
            gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
            IceMode(g_colorPlayer0R[playerId], bright_g[playerId], bright_b[playerId],
                          adjust_r[playerId], adjust_g[playerId], adjust_b[playerId],
                          g_GhostUseTimer[playerId]);
            gDPSetRenderMode(gDisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        }
    } else if (((player->jugemu_flag & IS_FADING_OUT) == IS_FADING_OUT) || (player->weapon & PRESS_IS_BOMB) ||
               (player->weapon & PRESS_BECOME_BOMB)) {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxKart[playerId + (arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, common_setting_render_character);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        IceMode(g_colorPlayer0R[playerId], bright_g[playerId], bright_b[playerId],
                      adjust_r[playerId], adjust_g[playerId], adjust_b[playerId],
                      (s32) player->erase);
        gDPSetAlphaCompare(gDisplayListHead++, G_AC_DITHER);
        gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    } else {
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxKart[playerId + (arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(gDisplayListHead++, common_setting_render_character);
        gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
        gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
        IceMode(g_colorPlayer0R[playerId], bright_g[playerId], bright_b[playerId],
                      adjust_r[playerId], adjust_g[playerId], adjust_b[playerId],
                      (s32) player->erase);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    }

    gDPLoadTextureBlock(gDisplayListHead++, sKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);

    gDPLoadTextureBlock(gDisplayListHead++, sKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset + 4], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
}

void draw_ghostkart(Player* player, s8 playerId, s8 screenId, s8 flipOffset) {
    UNUSED s32 pad;
    Mat4 sp12C;
    UNUSED s32 pad2[17];
    Vec3f spDC;
    Vec3s spD4;
    f32 spD0;
    f32 spCC;
    f32 spC8;
    UNUSED s32 pad3;
    s16 spC2;
    s16 thing;

    if (screenId) {}
    if (replay_flag == 1) {
        spC2 = 0x00FF;
    } else {
        spC2 = 0x0070;
    }
    thing = (u16) (player->angle_y[screenId] - player->direction[1]);
    spD4[0] = (-(s16) (sinT(thing) * (0.0f * 0.0f)) * 0.8);
    spD4[1] = player->angle_y[screenId];
    spD4[2] = player->angle_z[screenId];
    make_smokepos(&spD0, &spCC, &spC8, 0, 1.5f, 0, -player->angle_y[screenId], player->angle_z[screenId]);
    spDC[1] = (player->position[1] - player->radius) + (spCC - 2.0);
    spDC[0] = player->position[0] + spD0;
    spDC[2] = player->position[2] + spC8;
#ifdef AVOID_UB
    gPlayerPalette = &gPlayerPalettesList[draw_number[screenId][playerId]][screenId][playerId];
#else
    gPlayerPalette =
        (struct_D_802F1F80*) &gPlayerPalettesList[draw_number[screenId][playerId]][screenId][playerId * 0x100];
#endif
    if ((screenId == 0) || (screenId == 1)) {
        sKartUpperTexture =
            &D_802BFB80.arraySize8[draw_number[screenId][playerId]][screenId][playerId].pixel_index_array[0];
        sKartLowerTexture =
            &D_802BFB80.arraySize8[draw_number[screenId][playerId]][screenId][playerId].pixel_index_array[0x7C0];
    } else {
        sKartUpperTexture =
            &D_802BFB80.arraySize8[draw_number[screenId][playerId]][screenId - 1][playerId - 4].pixel_index_array[0];
        sKartLowerTexture =
            &D_802BFB80.arraySize8[draw_number[screenId][playerId]][screenId - 1][playerId - 4].pixel_index_array[0x7C0];
    }

    createaffine(sp12C, spDC, spD4);
    scale_matrix(sp12C, gCharacterSize[player->kart] * player->offsetsize);
    Affine2Mtx(&gDynamicP->mtxKart[playerId + (screenId * 8)], sp12C);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxKart[playerId + (screenId * 8)]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, common_setting_render_character);
    gDPLoadTLUT_pal256(gDisplayListHead++, gPlayerPalette);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    IceMode(g_colorPlayer0R[playerId], bright_g[playerId], bright_b[playerId],
                  adjust_r[playerId], adjust_g[playerId], adjust_b[playerId], spC2);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));

    gDPLoadTextureBlock(gDisplayListHead++, sKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);

    gDPLoadTextureBlock(gDisplayListHead++, sKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset + 4], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(gDisplayListHead++, G_AC_NONE);
}

void draw_turbokart(Player* player, s8 playerId, s8 screenId, s8 flipOffset) {
    Mat4 spA8;
    Vec3f sp9C;
    Vec3s sp94;

    sp9C[0] = player->position[0] + (sinT(-player->direction[1]) * -1.5);
    sp9C[1] = ((player->position[1] - player->radius) + player->shake) + 0.1;
    sp9C[2] = player->position[2] + (cosT(-player->direction[1]) * -1.5);
    sp94[0] = -DEGREES(1);
    sp94[1] = player->angle_y[screenId];
    sp94[2] = player->angle_z[screenId];

    createaffine(spA8, sp9C, sp94);
    scale_matrix(spA8, gCharacterSize[player->kart] * player->offsetsize);
    Affine2Mtx(&gDynamicP->mtxEffect[effectcount], spA8);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_0D008D10);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    IceMode(g_colorPlayer0R[playerId], bright_g[playerId], bright_b[playerId],
                  adjust_r[playerId], adjust_g[playerId], adjust_b[playerId],
                  0x00000040);
    gDPSetRenderMode(gDisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));

    gDPLoadTextureBlock(gDisplayListHead++, sKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);

    gDPLoadTextureBlock(gDisplayListHead++, sKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset + 4], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    effectcount += 1;
}

void draw_reflectkart(Player* player, s8 playerId, s8 screenId, s8 flipOffset) {
    Mat4 spA8;
    Vec3f sp9C;
    Vec3s sp94;

    sp94[0] = 0;
    sp94[1] = player->angle_y[screenId];
    sp94[2] = player->angle_z[screenId] + 0x8000; // invert Y
    sp9C[0] = player->position[0];
    sp9C[1] = player->ground + (4.0f * player->offsetsize);
    sp9C[2] = player->position[2];
    if (!(player->status & (P1_FLIP_SPRITE << (screenId * 4)))) {
        flipOffset = 8;
    } else {
        flipOffset = 0;
    }

    createaffine(spA8, sp9C, sp94);
    scale_matrix(spA8, gCharacterSize[player->kart] * player->offsetsize);
    Affine2Mtx(&gDynamicP->mtxEffect[effectcount], spA8);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, common_setting_render_character);
    gDPSetTextureLUT(gDisplayListHead++, G_TT_RGBA16);
    IceMode(g_colorPlayer0R[playerId], bright_g[playerId], bright_b[playerId],
                  adjust_r[playerId], adjust_g[playerId], adjust_b[playerId],
                  (s16) player->erase / 2);
    gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    gDPLoadTextureBlock(gDisplayListHead++, sKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gDPLoadTextureBlock(gDisplayListHead++, sKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(gDisplayListHead++, &KartVtx[playerId][flipOffset + 4], 4, 0);
    gSPDisplayList(gDisplayListHead++, common_square_plain_render);
    gSPTexture(gDisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    effectcount += 1;
}

void Doobj(Player* player, s8 playerId, s8 screenId) {
    UNUSED s32 pad[2];
    s32 temp_t1;
    s32 flipOffset;
    OSMesg* sp34;

    change_tirecol_Pa(player, playerId, screenId, draw_number[screenId][playerId]);
    if (!(player->status & (P1_FLIP_SPRITE << (screenId * 4)))) {
        flipOffset = 0;
    } else {
        flipOffset = 8;
    }
    check_vtx(player, playerId, screenId, flipOffset);
    temp_t1 = P1_DRAW_PARTICLES << (screenId * 4);
    if ((temp_t1 == (player->status & temp_t1)) && (player->bump.distance_zx <= 50.0f) &&
        (player->bump_status != ICE)) {
        if ((player->slip_flag & TERESA) == TERESA) {
            if (playerId == screenId) {
                draw_shadow(player, playerId, screenId);
            }
        } else {
            draw_shadow(player, playerId, screenId);
        }
    }
    if ((player->flag & IS_GHOST) != IS_GHOST) {
        draw_mainkart(player, playerId, screenId, flipOffset);
    } else {
        draw_ghostkart(player, playerId, screenId, flipOffset);
    }
    osRecvMesg(&gDmaMesgQueue, (OSMesg*) &sp34, OS_MESG_BLOCK);
    if ((temp_t1 == (player->status & temp_t1)) && (player->bump_status == ICE) &&
        ((player->jugemu_flag & IS_IN_WATER) != IS_IN_WATER) &&
        (player->bump.distance_zx <= 30.0f)) {
        draw_reflectkart(player, playerId, screenId, flipOffset);
    }
    if (player->turbo_power >= 2.0f) {
        draw_turbokart(player, playerId, screenId, flipOffset);
    }
}

void change_tirecol_calc(Player* player, s8 playerIndex) {
    f32 temp_f0;

    if (((player->slip_flag & RAPID_ACC) == RAPID_ACC) &&
        ((player->flag & NO_CONTROLS) == 0)) {
        player->tcol_count += D_800DDE74[8];
        if (player->tcol_count >= 0x400) {
            player->tcol_count = 0;
        }
        return;
    }

    temp_f0 = ((player->speed * (1.0f + player->power_cont)) / 18.0f) * 216.0f;
    if ((temp_f0 <= 1.0f) || (full_brake_accele[playerIndex] == true)) {
        player->tcol_count = 0;
    } else {
        player->tcol_count += D_800DDE74[(s32) (temp_f0 / 12.0f)];
    }
    if (player->tcol_count >= 0x400) {
        player->tcol_count = 0;
    }
}

// Properly define struct pointers, see buffers.h comment for more information.
#ifdef AVOID_UB
#define D_802F1F80_WHEEL(a, b, c) &gPlayerPalettesList[a][b][c].wheel_palette
#else
#define D_802F1F80_WHEEL(a, b, c) &gPlayerPalettesList[a][b][(c * 0x100) + 0xC0]
#endif

void change_tirecol_Pa(Player* player, s8 playerId, s8 screenId, s8 arg3) {
    s16 frameId = old_dir[screenId][playerId];
    s16 groupId = old_deg[screenId][playerId];
    s16 temp_t2 = player->tcol_count;
    s16 temp_num = 0x40; // setting this as a variable gets rid of regalloc

    if (((player->slip_flag & RAPID_ACC) == RAPID_ACC) &&
        ((player->flag & NO_CONTROLS) == 0)) {
        if (((player->slip_flag & SPIN_L) != SPIN_L) &&
            ((player->slip_flag & SPIN_R) != SPIN_R) &&
            ((player->slip_flag & THUNDER_SPIN) != THUNDER_SPIN) &&
            ((player->slip_flag & WING) != WING) &&
            ((player->slip_flag & STORM) != STORM) &&
            ((player->handling_flag & TROPHYJUMP_END) == 0)) {

            if (frameId <= 20) {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels0[player->kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            } else {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels1[player->kart][groupId] +
                                                     (frameId - 21) * (temp_num * 4) + ((temp_t2 >> 8) * 0x40) + 0x600),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            }
        } else {
            if (frameId == 0) {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels0[player->kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            } else {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels1[player->kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            }
        }
    } else {
        if (((player->slip_flag & SPIN_L) != SPIN_L) &&
            ((player->slip_flag & SPIN_R) != SPIN_R) &&
            ((player->slip_flag & WING) != WING) &&
            ((player->slip_flag & STORM) != STORM) &&
            ((player->slip_flag & THUNDER_SPIN) != THUNDER_SPIN) &&
            ((player->handling_flag & TROPHYJUMP_END) == 0)) {

            if (frameId <= 20) {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels0[player->kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            } else {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels1[player->kart][groupId] +
                                                     (frameId - 21) * (temp_num * 4) + ((temp_t2 >> 8) * 0x40) + 0x600),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            }
        } else {
            if (frameId == 0) {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels0[player->kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            } else {
                tirecol_dma_Pa(player,
                                              (s32) (gKartWheels1[player->kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(arg3, screenId, playerId), 0x80);
            }
        }
    }
}

#undef D_802F1F80_WHEEL

UNUSED void tire_pos_calc(void) {
}

UNUSED void debug_tire_cube(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2) {
}
