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

s8 RenderingFramebufferByPlayer[] = { 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02 };

s32 PlayersToRenderCount = 0;

// Can't find anything that actually references these...
UNUSED void* D_800DDB5C[3] = { Framebuffer0, Framebuffer1, Framebuffer2 };

s16 PlayersToRenderPlayerId[8];
s16 PlayersToRenderScreenId[8];
Player* BufferCar[8];
s16 Effectcount;
s32 D_80164AF4[3];
struct_D_802F1F80* PlayerPalette;
u8* SKartUpperTexture;
u8* SKartLowerTexture;
u16 ColorPlayer0R[8];
u16 BrightG[8];
u16 BrightB[8];
u16 AdjustR[8];
u16 AdjustG[8];
u16 AdjustB[8];
// Likely an Unused colour effect.
UNUSED u16 AdjustA[8];
s32 ColorTimer[296];
s16 D_80165020[40];
// Used to calculate difference between previous and Current Player Velocity.
Vec3f OldVelocity[8];
s16 OldDir[4][8];
s16 OldDeg[4][8];
s16 OldSlipcount[4][8];
s16 CompelFlag[4][8];
s16 DrawNumber[4][8];

void CheckFrame(s32* arg0, s32* arg1) {
    if ((DemoFlag == 1) || (OpeningFlag != 0) || (FinishZoomFlag != 0)) {
        *arg0 = 0xAA;
    } else {
        *arg0 = 0;
    }
    if (OpeningFlag != 0) {
        *arg1 = 0xAA;
        return;
    }
    *arg1 = 0;
}

void CheckViewport(Player* Player, Camera* Camera, s8 screenId) {
    UNUSED s32 pad;
    s32 sp30;
    s32 sp2C;
    UNUSED s32 Pad2;

    CheckKartNumber(Player);
    CheckFrame(&sp30, &sp2C);

    Player->Status &= ~(P1_DRAW_SPRITE << (screenId * 4));
    Player->Status &= ~(P1_DRAW_PARTICLES << (screenId * 4));

    if (IsInViewport(Player, Camera, (f32) (SelectViewKart + sp30), (f32) (SelectRearviewKart + sp2C)) == 1) {
        Player->Status |= P1_DRAW_SPRITE << (screenId * 4);
    }
    if (IsInViewport(Player, Camera, (f32) SelectViewObj, (f32) SelectRearviewObj) == 1) {
        Player->Status |= P1_DRAW_PARTICLES << (screenId * 4);
    }
}

u16 IsInViewport(Player* Player, Camera* Camera, f32 Arg2, f32 Arg3) {
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
    switch (ScreenSplitA) { /* irregular */
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
    sp4C = (Arg2 * CosT((Camera->CameraDirection[1] - var_v0))) + Camera->CameraPos[2];
    sp58 = (Arg2 * SinT((Camera->CameraDirection[1] - var_v0))) + Camera->CameraPos[0];
    sp48 = (Arg2 * CosT((Camera->CameraDirection[1] + var_v0))) + Camera->CameraPos[2];
    sp54 = (Arg2 * SinT((Camera->CameraDirection[1] + var_v0))) + Camera->CameraPos[0];
    sp44 = (Arg3 * CosT((Camera->CameraDirection[1] + (135 * DEGREES(1))))) + Camera->CameraPos[2];
    sp50 = (Arg3 * SinT((Camera->CameraDirection[1] + (135 * DEGREES(1))))) + Camera->CameraPos[0];

    sp64 = ((sp4C - Player->Position[2]) * (sp54 - Player->Position[0])) - ((sp48 - Player->Position[2]) * (sp58 - Player->Position[0]));
    sp60 = ((sp48 - Player->Position[2]) * (sp50 - Player->Position[0])) - ((sp44 - Player->Position[2]) * (sp54 - Player->Position[0]));
    sp5C = ((sp44 - Player->Position[2]) * (sp58 - Player->Position[0])) - ((sp4C - Player->Position[2]) * (sp50 - Player->Position[0]));

    if (((sp64 >= 0) && (sp60 >= 0) && (sp5C >= 0)) || (((sp64) <= 0) && (sp60 <= 0) && (sp5C <= 0))) {
        ret = true;
    }
    return ret;
}

u16 SlipstreamKart(Player* Player, f32 PosX, UNUSED f32 Arg2, f32 PosZ) {
    f32 sp64;
    f32 sp60;
    f32 sp5c;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 temp_f14;
    f32 Thing0;
    f32 Thing1;
    u16 ret;

    ret = false;

    sp58 = (70.0f * CosT(((Player->Slipang - Player->Direction[1]) - (10 * DEGREES(1))))) + Player->Position[2];
    sp64 = (70.0f * SinT(((Player->Slipang - Player->Direction[1]) - (10 * DEGREES(1))))) + Player->Position[0];
    sp54 = (70.0f * CosT(((Player->Slipang - Player->Direction[1]) + (10 * DEGREES(1))))) + Player->Position[2];
    sp60 = (70.0f * SinT(((Player->Slipang - Player->Direction[1]) + (10 * DEGREES(1))))) + Player->Position[0];
    sp50 = (10.0f * CosT(((Player->Slipang - Player->Direction[1]) + (40 * DEGREES(1))))) + Player->Position[2];
    sp5c = (10.0f * SinT(((Player->Slipang - Player->Direction[1]) + (40 * DEGREES(1))))) + Player->Position[0];

    temp_f14 = ((sp58 - PosZ) * (sp60 - PosX)) - ((sp54 - PosZ) * (sp64 - PosX));
    Thing0 = ((sp54 - PosZ) * (sp5c - PosX)) - ((sp50 - PosZ) * (sp60 - PosX));
    Thing1 = ((sp50 - PosZ) * (sp64 - PosX)) - ((sp58 - PosZ) * (sp5c - PosX));
    if (((temp_f14 >= 0) && (Thing0 >= 0) && (Thing1 >= 0)) || ((temp_f14 <= 0) && (Thing0 <= 0) && (Thing1 <= 0))) {
        ret = true;
    }
    return ret;
}

/**
 * @brief one of his function is to put the frame to load in to a list to load it to be only applied for the Next frame
 */
void InitRenderPlayer(Player* Player, Camera* Camera, s8 PlayerId, s8 screenId) {
    UNUSED s32 pad[4];
    s32 sp4C;
    s32 sp48;
    UNUSED s32 Pad2;
    s32 temp_v0;
    s32 temp_v0_2;

    if ((Player->Flag & EXISTS) == EXISTS) {
        CheckViewport(Player, Camera, screenId);
        temp_v0 = P1_DRAW_SPRITE << (screenId << 2);
        if (temp_v0 == (Player->Status & temp_v0)) {
            if (!(Player->Flag & NO_CONTROLS)) {
                KartPosset(Player, Camera, screenId, PlayerId);
            } else {
                KartPosset(Player, Camera, screenId, PlayerId);
                Player->Kartpitch = 0;
                Player->Kartroll = 0;
                Player->AngleZ[screenId] = 0;
            }
        }
        CheckFrame(&sp4C, &sp48);
        temp_v0_2 = P1_DMA_SPRITE << (screenId << 2);
        if ((temp_v0 == (Player->Status & temp_v0)) && (temp_v0_2 == (Player->Status & temp_v0_2))) {
            if ((IsInViewport(Player, Camera, SelectDMAKart + sp4C, SelectReardmaKart + sp48) == 1) & 0xFFFF) {
                PlayersToRenderPlayerId[PlayersToRenderCount] = (s16) PlayerId;
                PlayersToRenderScreenId[PlayersToRenderCount] = (s16) screenId;
                BufferCar[PlayersToRenderCount] = Player;
                PlayersToRenderCount += 1;
                CompelFlag[screenId][PlayerId] = 0;
                OldDir[screenId][PlayerId] = Player->dir[screenId];
                OldDeg[screenId][PlayerId] = Player->deg[screenId];
                OldSlipcount[screenId][PlayerId] = Player->Slipcount;
                DrawNumber[screenId][PlayerId] += 1;
                if (DrawNumber[screenId][PlayerId] == 2) {
                    DrawNumber[screenId][PlayerId] = 0;
                }
            } else {
                if ((IsInViewport(Player, Camera, SelectNondmaKart + sp4C, SelectRearNondmaKart) == 1) & 0xFFFF) {
                    if ((DrawFrame == RenderingFramebufferByPlayer[PlayerId]) ||
                        ((OldDir[screenId][PlayerId] - Player->dir[screenId]) > 0x13) ||
                        ((OldDir[screenId][PlayerId] - Player->dir[screenId]) < -0x13) ||
                        (CompelFlag[screenId][PlayerId] == (s16) 1U)) {
                        PlayersToRenderPlayerId[PlayersToRenderCount] = (s16) PlayerId;
                        PlayersToRenderScreenId[PlayersToRenderCount] = (s16) screenId;
                        BufferCar[PlayersToRenderCount] = Player;
                        PlayersToRenderCount += 1;
                        OldDir[screenId][PlayerId] = Player->dir[screenId];
                        OldDeg[screenId][PlayerId] = Player->deg[screenId];
                        OldSlipcount[screenId][PlayerId] = Player->Slipcount;
                        CompelFlag[screenId][PlayerId] = 0;
                        DrawNumber[screenId][PlayerId] += 1;
                        if (DrawNumber[screenId][PlayerId] == 2) {
                            DrawNumber[screenId][PlayerId] = 0;
                        }
                    }
                } else {
                    if (((OldDir[screenId][PlayerId] - Player->dir[screenId]) > 0x13) ||
                        ((OldDir[screenId][PlayerId] - Player->dir[screenId]) < -0x13) ||
                        (CompelFlag[screenId][PlayerId] == (s16) 1U)) {
                        PlayersToRenderPlayerId[PlayersToRenderCount] = (s16) PlayerId;
                        PlayersToRenderScreenId[PlayersToRenderCount] = (s16) screenId;
                        BufferCar[PlayersToRenderCount] = Player;
                        PlayersToRenderCount += 1;
                        OldDir[screenId][PlayerId] = (s16) Player->dir[screenId];
                        OldDeg[screenId][PlayerId] = Player->deg[screenId];
                        OldSlipcount[screenId][PlayerId] = Player->Slipcount;
                        CompelFlag[screenId][PlayerId] = 0;
                        DrawNumber[screenId][PlayerId] += 1;
                        if (DrawNumber[screenId][PlayerId] == 2) {
                            DrawNumber[screenId][PlayerId] = 0;
                        }
                    }
                }
            }
        }
    }
}

void CheckDMA(void) {
    s16 i;

    Pakartdma(BufferCar[0], PlayersToRenderPlayerId[0], PlayersToRenderScreenId[0],
                                   PlayersToRenderScreenId[0],
                                   DrawNumber[PlayersToRenderScreenId[0]][PlayersToRenderPlayerId[0]]);
    SmokeDisp(PlayerOneCopy, PLAYER_ONE, PLAYER_ONE);
    SmokeDisp(PlayerTwo, PLAYER_TWO, PLAYER_ONE);
    SmokeDisp(PlayerThree, PLAYER_THREE, PLAYER_ONE);
    SmokeDisp(PlayerFour, PLAYER_FOUR, PLAYER_ONE);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDisp(PlayerFive, PLAYER_FIVE, PLAYER_ONE);
        SmokeDisp(PlayerSix, PLAYER_SIX, PLAYER_ONE);
        SmokeDisp(PlayerSeven, PLAYER_SEVEN, PLAYER_ONE);
        SmokeDisp(PlayerEight, PLAYER_EIGHT, PLAYER_ONE);
    }
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);

    for (i = 1; i < PlayersToRenderCount; i++) {

        Pakartdma(BufferCar[i], PlayersToRenderPlayerId[i],
                                       PlayersToRenderScreenId[i], PlayersToRenderScreenId[i],
                                       DrawNumber[PlayersToRenderScreenId[i]][PlayersToRenderPlayerId[i]]);

        decodeMIO0(
            (u8*) EncodedKartTexture[DrawNumber[PlayersToRenderScreenId[i - 1]][PlayersToRenderPlayerId[i - 1]]]
                                     [PlayersToRenderScreenId[i - 1]][PlayersToRenderPlayerId[i - 1]]
                                         .unk_00,
            D_802BFB80
                .arraySize8[DrawNumber[PlayersToRenderScreenId[i - 1]][PlayersToRenderPlayerId[i - 1]]]
                           [PlayersToRenderScreenId[i - 1]][PlayersToRenderPlayerId[i - 1]]
                .PixelIndexArray);
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    }

    decodeMIO0((u8*) EncodedKartTexture[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                                   [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                                        [PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                        [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                         [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                              [PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                              [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]
                   .PixelIndexArray);
}

void CheckDMA2P(void) {
    s16 var_s0;

    Pakartdma(BufferCar[0], PlayersToRenderPlayerId[0], PlayersToRenderScreenId[0],
                                   PlayersToRenderScreenId[0],
                                   DrawNumber[PlayersToRenderScreenId[0]][PlayersToRenderPlayerId[0]]);
    SmokeDisp2P(PlayerOneCopy, PLAYER_ONE, PLAYER_TWO);
    SmokeDisp2P(PlayerTwo, PLAYER_TWO, PLAYER_TWO);
    SmokeDisp2P(PlayerThree, PLAYER_THREE, PLAYER_TWO);
    SmokeDisp2P(PlayerFour, PLAYER_FOUR, PLAYER_TWO);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDisp2P(PlayerFive, PLAYER_FIVE, PLAYER_TWO);
        SmokeDisp2P(PlayerSix, PLAYER_SIX, PLAYER_TWO);
        SmokeDisp2P(PlayerSeven, PLAYER_SEVEN, PLAYER_TWO);
        SmokeDisp2P(PlayerEight, PLAYER_EIGHT, PLAYER_TWO);
    }
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    for (var_s0 = 1; var_s0 < PlayersToRenderCount; var_s0++) {
        Pakartdma(BufferCar[var_s0], PlayersToRenderPlayerId[var_s0],
                                       PlayersToRenderScreenId[var_s0], PlayersToRenderScreenId[var_s0],
                                       DrawNumber[PlayersToRenderScreenId[var_s0]][PlayersToRenderPlayerId[var_s0]]);
        decodeMIO0(
            (u8*) EncodedKartTexture[DrawNumber[PlayersToRenderScreenId[var_s0 - 1]]
                                                [PlayersToRenderPlayerId[var_s0 - 1]]]
                                     [PlayersToRenderScreenId[var_s0 - 1]][PlayersToRenderPlayerId[var_s0 - 1]]
                                         .unk_00,
            D_802BFB80
                .arraySize8[DrawNumber[PlayersToRenderScreenId[var_s0 - 1]][PlayersToRenderPlayerId[var_s0 - 1]]]
                           [PlayersToRenderScreenId[var_s0 - 1]][PlayersToRenderPlayerId[var_s0 - 1]]
                .PixelIndexArray);
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    }
    decodeMIO0((u8*) EncodedKartTexture[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                                   [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                                        [PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                        [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                         [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                              [PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                              [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]
                   .PixelIndexArray);
}

void CheckDMA3P(void) {
    s16 var_s0;

    Pakartdma(BufferCar[0], PlayersToRenderPlayerId[0] + 4,
                                   PlayersToRenderScreenId[0], PlayersToRenderScreenId[0] - 2,
                                   DrawNumber[PlayersToRenderScreenId[0]][PlayersToRenderPlayerId[0]]);
    SmokeDisp3P(PlayerOneCopy, PLAYER_ONE, PLAYER_THREE);
    SmokeDisp3P(PlayerTwo, PLAYER_TWO, PLAYER_THREE);
    SmokeDisp3P(PlayerThree, PLAYER_THREE, PLAYER_THREE);
    SmokeDisp3P(PlayerFour, PLAYER_FOUR, PLAYER_THREE);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    for (var_s0 = 1; var_s0 < PlayersToRenderCount; var_s0++) {
        Pakartdma(BufferCar[var_s0], PlayersToRenderPlayerId[var_s0] + 4,
                                       PlayersToRenderScreenId[var_s0], PlayersToRenderScreenId[var_s0] - 2,
                                       DrawNumber[PlayersToRenderScreenId[var_s0]][PlayersToRenderPlayerId[var_s0]]);
        decodeMIO0(
            (u8*) EncodedKartTexture
                [DrawNumber[PlayersToRenderScreenId[var_s0 - 1]][PlayersToRenderPlayerId[var_s0 - 1]]]
                [PlayersToRenderScreenId[var_s0 - 1] - 2][PlayersToRenderPlayerId[var_s0 - 1] + 4]
                    .unk_00,
            D_802BFB80
                .arraySize8[DrawNumber[PlayersToRenderScreenId[var_s0 - 1]][PlayersToRenderPlayerId[var_s0 - 1]]]
                           [PlayersToRenderScreenId[var_s0 - 1] - 2][PlayersToRenderPlayerId[var_s0 - 1] + 4]
                .PixelIndexArray);
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    }
    decodeMIO0((u8*) EncodedKartTexture[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                                   [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                                        [PlayersToRenderScreenId[PlayersToRenderCount - 1] - 2]
                                        [PlayersToRenderPlayerId[PlayersToRenderCount - 1] + 4]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                         [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                              [PlayersToRenderScreenId[PlayersToRenderCount - 1] - 2]
                              [PlayersToRenderPlayerId[PlayersToRenderCount - 1] + 4]
                   .PixelIndexArray);
}

void CheckDMA4P(void) {
    s16 var_s0;

    Pakartdma(BufferCar[0], PlayersToRenderPlayerId[0] + 4,
                                   PlayersToRenderScreenId[0], PlayersToRenderScreenId[0] - 2,
                                   DrawNumber[PlayersToRenderScreenId[0]][PlayersToRenderPlayerId[0]]);
    SmokeDisp4P(PlayerOneCopy, PLAYER_ONE, PLAYER_FOUR);
    SmokeDisp4P(PlayerTwo, PLAYER_TWO, PLAYER_FOUR);
    SmokeDisp4P(PlayerThree, PLAYER_THREE, PLAYER_FOUR);
    SmokeDisp4P(PlayerFour, PLAYER_FOUR, PLAYER_FOUR);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    for (var_s0 = 1; var_s0 < PlayersToRenderCount; var_s0++) {
        Pakartdma(BufferCar[var_s0], PlayersToRenderPlayerId[var_s0] + 4,
                                       PlayersToRenderScreenId[var_s0], PlayersToRenderScreenId[var_s0] - 2,
                                       DrawNumber[PlayersToRenderScreenId[var_s0]][PlayersToRenderPlayerId[var_s0]]);
        decodeMIO0(
            (u8*) EncodedKartTexture
                [DrawNumber[PlayersToRenderScreenId[var_s0 - 1]][PlayersToRenderPlayerId[var_s0 - 1]]]
                [PlayersToRenderScreenId[var_s0 - 1] - 2][PlayersToRenderPlayerId[var_s0 - 1] + 4]
                    .unk_00,
            D_802BFB80
                .arraySize8[DrawNumber[PlayersToRenderScreenId[var_s0 - 1]][PlayersToRenderPlayerId[var_s0 - 1]]]
                           [PlayersToRenderScreenId[var_s0 - 1] - 2][PlayersToRenderPlayerId[var_s0 - 1] + 4]
                .PixelIndexArray);
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    }
    decodeMIO0((u8*) EncodedKartTexture[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                                   [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                                        [PlayersToRenderScreenId[PlayersToRenderCount - 1] - 2]
                                        [PlayersToRenderPlayerId[PlayersToRenderCount - 1] + 4]
                                            .unk_00,
               D_802BFB80
                   .arraySize8[DrawNumber[PlayersToRenderScreenId[PlayersToRenderCount - 1]]
                                         [PlayersToRenderPlayerId[PlayersToRenderCount - 1]]]
                              [PlayersToRenderScreenId[PlayersToRenderCount - 1] - 2]
                              [PlayersToRenderPlayerId[PlayersToRenderCount - 1] + 4]
                   .PixelIndexArray);
}

void TryRenderingPlayer(Player* Player, s8 PlayerId, s8 Arg2) {

    if (((Player->Flag & EXISTS) == EXISTS) && ((Player->Flag & IS_BOMB) == 0)) {
        if ((Player->Status & P1_DRAW_SPRITE << (Arg2 * 4)) == P1_DRAW_SPRITE << (Arg2 * 4)) {
            Doobj(Player, PlayerId, Arg2);
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

    PlayersToRenderCount = 0;
    InitRenderPlayer(PlayerOneCopy, Camera1, PLAYER_ONE, PLAYER_ONE);
    InitRenderPlayer(PlayerTwo, Camera1, PLAYER_TWO, PLAYER_ONE);
    InitRenderPlayer(PlayerThree, Camera1, PLAYER_THREE, PLAYER_ONE);
    InitRenderPlayer(PlayerFour, Camera1, PLAYER_FOUR, PLAYER_ONE);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        InitRenderPlayer(PlayerFive, Camera1, PLAYER_FIVE, PLAYER_ONE);
        InitRenderPlayer(PlayerSix, Camera1, PLAYER_SIX, PLAYER_ONE);
        InitRenderPlayer(PlayerSeven, Camera1, PLAYER_SEVEN, PLAYER_ONE);
        InitRenderPlayer(PlayerEight, Camera1, PLAYER_EIGHT, PLAYER_ONE);
    }
    TryRenderingPlayer(PlayerOne, PLAYER_ONE, PLAYER_ONE);
    TryRenderingPlayer(PlayerTwo, PLAYER_TWO, PLAYER_ONE);
    TryRenderingPlayer(PlayerThree, PLAYER_THREE, PLAYER_ONE);
    TryRenderingPlayer(PlayerFour, PLAYER_FOUR, PLAYER_ONE);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        TryRenderingPlayer(PlayerFive, PLAYER_FIVE, PLAYER_ONE);
        TryRenderingPlayer(PlayerSix, PLAYER_SIX, PLAYER_ONE);
        TryRenderingPlayer(PlayerSeven, PLAYER_SEVEN, PLAYER_ONE);
        TryRenderingPlayer(PlayerEight, PLAYER_EIGHT, PLAYER_ONE);
    }
    if (PlayersToRenderCount != 0) {
        CheckDMA();
    } else {
        SmokeDisp(PlayerOneCopy, PLAYER_ONE, PLAYER_ONE);
        SmokeDisp(PlayerTwo, PLAYER_TWO, PLAYER_ONE);
        SmokeDisp(PlayerThree, PLAYER_THREE, PLAYER_ONE);
        SmokeDisp(PlayerFour, PLAYER_FOUR, PLAYER_ONE);
        if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
            SmokeDisp(PlayerFive, PLAYER_FIVE, PLAYER_ONE);
            SmokeDisp(PlayerSix, PLAYER_SIX, PLAYER_ONE);
            SmokeDisp(PlayerSeven, PLAYER_SEVEN, PLAYER_ONE);
            SmokeDisp(PlayerEight, PLAYER_EIGHT, PLAYER_ONE);
        }
    }
    PlayersToRenderCount = 0;
}

// Hello, you've found the location of the bulk of the data section for this file
// For reasons beyound human comprehension it MUST be placed somewhere below
// DrawKart

s32 Junk[] = { 0, 0, 0 };

Vtx* KartVtx[] = { PlayerOneVtx, PlayerTwoVtx, PlayerThreeVtx, PlayerFourVtx, PlayerFiveVtx, PlayerSixVtx, PlayerSevenVtx, PlayerEightVtx };

f32 CharacterSize[] = { MARIO_SIZE, LUIGI_SIZE, YOSHI_SIZE, TOAD_SIZE, DK_SIZE, WARIO_SIZE, PEACH_SIZE, BOWSER_SIZE };

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

s32 MaybeCompilerPadding = 0;

void DrawKart2P(void) {
    PlayersToRenderCount = 0;
    InitRenderPlayer(PlayerOneCopy, Camera2, PLAYER_ONE, PLAYER_TWO);
    InitRenderPlayer(PlayerTwo, Camera2, PLAYER_TWO, PLAYER_TWO);
    InitRenderPlayer(PlayerThree, Camera2, PLAYER_THREE, PLAYER_TWO);
    InitRenderPlayer(PlayerFour, Camera2, PLAYER_FOUR, PLAYER_TWO);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        InitRenderPlayer(PlayerFive, Camera2, PLAYER_FIVE, PLAYER_TWO);
        InitRenderPlayer(PlayerSix, Camera2, PLAYER_SIX, PLAYER_TWO);
        InitRenderPlayer(PlayerSeven, Camera2, PLAYER_SEVEN, PLAYER_TWO);
        InitRenderPlayer(PlayerEight, Camera2, PLAYER_EIGHT, PLAYER_TWO);
    }
    TryRenderingPlayer(PlayerOne, PLAYER_ONE, PLAYER_TWO);
    TryRenderingPlayer(PlayerTwo, PLAYER_TWO, PLAYER_TWO);
    TryRenderingPlayer(PlayerThree, PLAYER_THREE, PLAYER_TWO);
    TryRenderingPlayer(PlayerFour, PLAYER_FOUR, PLAYER_TWO);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        TryRenderingPlayer(PlayerFive, PLAYER_FIVE, PLAYER_TWO);
        TryRenderingPlayer(PlayerSix, PLAYER_SIX, PLAYER_TWO);
        TryRenderingPlayer(PlayerSeven, PLAYER_SEVEN, PLAYER_TWO);
        TryRenderingPlayer(PlayerEight, PLAYER_EIGHT, PLAYER_TWO);
    }
    if (PlayersToRenderCount != 0) {
        CheckDMA2P();
    } else {
        SmokeDisp2P(PlayerOneCopy, PLAYER_ONE, PLAYER_TWO);
        SmokeDisp2P(PlayerTwo, PLAYER_TWO, PLAYER_TWO);
        SmokeDisp2P(PlayerThree, PLAYER_THREE, PLAYER_TWO);
        SmokeDisp2P(PlayerFour, PLAYER_FOUR, PLAYER_TWO);
        if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
            SmokeDisp2P(PlayerFive, PLAYER_FIVE, PLAYER_TWO);
            SmokeDisp2P(PlayerSix, PLAYER_SIX, PLAYER_TWO);
            SmokeDisp2P(PlayerSeven, PLAYER_SEVEN, PLAYER_TWO);
            SmokeDisp2P(PlayerEight, PLAYER_EIGHT, PLAYER_TWO);
        }
    }
    PlayersToRenderCount = 0;
}

void DrawKart3P(void) {
    PlayersToRenderCount = 0;
    InitRenderPlayer(PlayerOneCopy, Camera3, PLAYER_ONE, PLAYER_THREE);
    InitRenderPlayer(PlayerTwo, Camera3, PLAYER_TWO, PLAYER_THREE);
    InitRenderPlayer(PlayerThree, Camera3, PLAYER_THREE, PLAYER_THREE);
    InitRenderPlayer(PlayerFour, Camera3, PLAYER_FOUR, PLAYER_THREE);
    TryRenderingPlayer(PlayerOne, PLAYER_ONE, PLAYER_THREE);
    TryRenderingPlayer(PlayerTwo, PLAYER_TWO, PLAYER_THREE);
    TryRenderingPlayer(PlayerThree, PLAYER_THREE, PLAYER_THREE);
    TryRenderingPlayer(PlayerFour, PLAYER_FOUR, PLAYER_THREE);
    if (PlayersToRenderCount != 0) {
        CheckDMA3P();
    } else {
        SmokeDisp3P(PlayerOneCopy, PLAYER_ONE, PLAYER_THREE);
        SmokeDisp3P(PlayerTwo, PLAYER_TWO, PLAYER_THREE);
        SmokeDisp3P(PlayerThree, PLAYER_THREE, PLAYER_THREE);
        SmokeDisp3P(PlayerFour, PLAYER_FOUR, PLAYER_THREE);
    }
    PlayersToRenderCount = 0;
}

void DrawKart4P(void) {
    PlayersToRenderCount = 0;
    InitRenderPlayer(PlayerOneCopy, Camera4, PLAYER_ONE, PLAYER_FOUR);
    InitRenderPlayer(PlayerTwo, Camera4, PLAYER_TWO, PLAYER_FOUR);
    InitRenderPlayer(PlayerThree, Camera4, PLAYER_THREE, PLAYER_FOUR);
    InitRenderPlayer(PlayerFour, Camera4, PLAYER_FOUR, PLAYER_FOUR);
    TryRenderingPlayer(PlayerOne, PLAYER_ONE, PLAYER_FOUR);
    TryRenderingPlayer(PlayerTwo, PLAYER_TWO, PLAYER_FOUR);
    TryRenderingPlayer(PlayerThree, PLAYER_THREE, PLAYER_FOUR);
    TryRenderingPlayer(PlayerFour, PLAYER_FOUR, PLAYER_FOUR);
    if (PlayersToRenderCount != 0) {
        CheckDMA4P();
    } else {
        SmokeDisp4P(PlayerOneCopy, PLAYER_ONE, PLAYER_FOUR);
        SmokeDisp4P(PlayerTwo, PLAYER_TWO, PLAYER_FOUR);
        SmokeDisp4P(PlayerThree, PLAYER_THREE, PLAYER_FOUR);
        SmokeDisp4P(PlayerFour, PLAYER_FOUR, PLAYER_FOUR);
    }
    PlayersToRenderCount = 0;
}

void DrawKartAfter(void) {
    SmokeDispAfter(PlayerOneCopy, 0, 0);
    SmokeDispAfter(PlayerTwo, 1, 0);
    SmokeDispAfter(PlayerThree, 2, 0);
    SmokeDispAfter(PlayerFour, 3, 0);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDispAfter(PlayerFive, 4, 0);
        SmokeDispAfter(PlayerSix, 5, 0);
        SmokeDispAfter(PlayerSeven, 6, 0);
        SmokeDispAfter(PlayerEight, 7, 0);
    }
    if (GameState == ENDING) {
        if (PlayerOne->HandlingFlag & TROPHYJUMP_KEEP) {
            DrawResultShadow(PlayerOne, 0, 0);
        }
        if (PlayerTwo->HandlingFlag & TROPHYJUMP_KEEP) {
            DrawResultShadow(PlayerTwo, 1, 0);
        }
        if (PlayerThree->HandlingFlag & TROPHYJUMP_KEEP) {
            DrawResultShadow(PlayerThree, 2, 0);
        }
        if (PlayerFour->HandlingFlag & TROPHYJUMP_KEEP) {
            DrawResultShadow(PlayerFour, 3, 0);
        }
    }
}

void DrawKart2PAfter(void) {
    SmokeDispAfter2P(PlayerOneCopy, 0, 1);
    SmokeDispAfter2P(PlayerTwo, 1, 1);
    SmokeDispAfter2P(PlayerThree, 2, 1);
    SmokeDispAfter2P(PlayerFour, 3, 1);
    if (ScreenSplitA != SCREEN_MODE_3P_4P_SPLITSCREEN) {
        SmokeDispAfter2P(PlayerFive, 4, 1);
        SmokeDispAfter2P(PlayerSix, 5, 1);
        SmokeDispAfter2P(PlayerSeven, 6, 1);
        SmokeDispAfter2P(PlayerEight, 7, 1);
    }
}

void DrawKart3PAfter(void) {
    SmokeDispAfter3P(PlayerOneCopy, 0, 2);
    SmokeDispAfter3P(PlayerTwo, 1, 2);
    SmokeDispAfter3P(PlayerThree, 2, 2);
    SmokeDispAfter3P(PlayerFour, 3, 2);
}

void DrawKart4PAfter(void) {
    SmokeDispAfter4P(PlayerOneCopy, 0, 3);
    SmokeDispAfter4P(PlayerTwo, 1, 3);
    SmokeDispAfter4P(PlayerThree, 2, 3);
    SmokeDispAfter4P(PlayerFour, 3, 3);
}

void Createaffine(Mat4 dest, Vec3f pos, Vec3s orientation) {
    UNUSED f32 pad[3];
    f32 sinX = SinT(orientation[0]);
    f32 cosX = CosT(orientation[0]);
    f32 sinY = SinT(orientation[1]);
    f32 cosY = CosT(orientation[1]);
    f32 sinZ = SinT(orientation[2]);
    f32 cosZ = CosT(orientation[2]);

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

UNUSED void TransMatrix(Mat4 arg0, Vec3f arg1) {
    arg0[3][0] += arg1[0];
    arg0[3][1] += arg1[1];
    arg0[3][2] += arg1[2];
}

void ScaleMatrix(Mat4 arg0, f32 Scale) {
    arg0[0][0] *= Scale;
    arg0[1][0] *= Scale;
    arg0[2][0] *= Scale;
    arg0[0][1] *= Scale;
    arg0[1][1] *= Scale;
    arg0[2][1] *= Scale;
    arg0[0][2] *= Scale;
    arg0[1][2] *= Scale;
    arg0[2][2] *= Scale;
}

/**
 * This function writes a fixed-Point Value to each Mtx entry. This is not how the Mtx struct works.
 * The first half of Mtx only holds s16 whole numbers and the second half holds the s16 decimal (fractional) parts.
 * See Affine2Mtx() for correct calculations. Note that each Mtx entry is the Size of s32.
 * This means each Mtx entry holds two s16 values.
 * The first sixteen Entries contain only the integer parts and the second sixteen Entries hold only the decimal
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
 * Takes a floating-Point Matrix and converts it to an s15.16 internal Matrix.
 * Each Mtx entry is a Size of s32 that holds two values.
 * The first 16 Entries hold only the integer values and the second 16 Entries hold only the decimal (fractional) parts.
 * In simpler words, the integer and decimal gets split up and stored in their own section.
 * Mtx is setup this way due to hardware restrictions of the n64 or as an optimization.
 *
 * @Param Mtx A new internal fixed-Point Matrix.
 * @Param Mat4 An array of f32
 * Mat4 to Mtx explanation: https://blarg.ca/2020/10/11/fixed-Point-math.
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

bool ChaseDir(s16* Angle, s16 targetAngle, s16 step) {
    s16 temp_v0;

    temp_v0 = targetAngle - *Angle;
    if (step < 0) { // abs of step
        step *= -1;
    }

    if (temp_v0 > 0) {
        temp_v0 -= step;
        if (temp_v0 >= 0) {
            *Angle = targetAngle - temp_v0;
        } else {
            *Angle = targetAngle;
        }
    } else {
        temp_v0 += step;
        if (temp_v0 <= 0) {
            *Angle = targetAngle - temp_v0;
        } else {
            *Angle = targetAngle;
        }
    }
    if (targetAngle == *Angle) {
        return false;
    }
    return true;
}

void ChaseInumber(s32* startingValue, s32 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

/**
  * Function: ChaseFnumber

  * Parameters:
  *     f32 *startingValue - Pointer to the float that will be modified
  *     f32 targetValue    - Float Value to move startingValue towards
  *     f32 somePercent    - The percent of the difference between startingValue
                            and targetValue to actually move

  * Moves a given startingValue the given somePercent towards the targetValue

  * f32 *thing = 2500;
  * ChaseFnumber(thing, 500, 0.75f);
  * thing now has a Value of 1000

  * If after the move startingValue is inside of the range [-0.001, 0.001],
  * Force it to exactly 0.0f

  * This is probably a precision thing. The scaling with somePercent likely
  * can't hit exactly 0 with any reliability, so they Force it to 0 if you're
  * in a small range around it. Why they only do this for 0 is anyone's guess though
**/
void ChaseFnumber(f32* startingValue, f32 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
    if ((*startingValue < 0.001) && (-0.001 < *startingValue)) {
        *startingValue = 0.0f;
    }
}

void ChaseSnumber(s16* startingValue, s16 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

void ChaseUSnumber(u16* startingValue, s16 targetValue, f32 somePercent) {
    *startingValue -= ((*startingValue - targetValue) * somePercent);
}

void KenStartegy(void) {
    KartEffectStrategy();
    VTXStrategy();
}

void VTXStrategy(void) {
    switch (ScreenSplitA) { /* irregular */
        case SCREEN_MODE_1P:
            switch (GameMode) { /* switch 1; irregular */
                case GRAND_PRIX:      /* switch 1 */
                    VTXCalc(PlayerOne, 0);
                    VTXCalc(PlayerTwo, 1);
                    VTXCalc(PlayerThree, 2);
                    VTXCalc(PlayerFour, 3);
                    VTXCalc(PlayerFive, 4);
                    VTXCalc(PlayerSix, 5);
                    VTXCalc(PlayerSeven, 6);
                    VTXCalc(PlayerEight, 7);
                    break;
                case TIME_TRIALS: /* switch 1 */
                    VTXCalc(PlayerOne, 0);
                    if ((PlayerTwo->Flag & IS_GHOST) == IS_GHOST) {
                        VTXCalc(PlayerTwo, 1);
                    }
                    if ((PlayerThree->Flag & IS_GHOST) == IS_GHOST) {
                        VTXCalc(PlayerThree, 2);
                    }
                    break;
                case VERSUS: /* switch 1 */
                case BATTLE: /* switch 1 */
                    VTXCalc(PlayerOne, 0);
                    VTXCalc(PlayerTwo, 1);
                    if (PlayerCount >= 3) {
                        VTXCalc(PlayerThree, 2);
                    }
                    if (PlayerCount == 4) {
                        VTXCalc(PlayerFour, 3);
                    }
                    break;
            }
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            switch (GameMode) { /* switch 2; irregular */
                case GRAND_PRIX:      /* switch 2 */
                    VTXCalc(PlayerOne, 0);
                    VTXCalc(PlayerTwo, 1);
                    VTXCalc(PlayerThree, 2);
                    VTXCalc(PlayerFour, 3);
                    VTXCalc(PlayerFive, 4);
                    VTXCalc(PlayerSix, 5);
                    VTXCalc(PlayerSeven, 6);
                    VTXCalc(PlayerEight, 7);
                    break;
                case VERSUS: /* switch 2 */
                case BATTLE: /* switch 2 */
                    VTXCalc(PlayerOne, 0);
                    VTXCalc(PlayerTwo, 1);
                    break;
                case TIME_TRIALS: /* switch 2 */
                    VTXCalc(PlayerOne, 0);
                    break;
            }
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if ((VERSUS == GameMode) || (BATTLE == GameMode)) {
                VTXCalc(PlayerOne, 0);
                VTXCalc(PlayerTwo, 1);
                VTXCalc(PlayerThree, 2);
                if (PlayerCount == 4) {
                    VTXCalc(PlayerFour, 3);
                }
            }
            break;
    }
}

void VTXCalc(Player* Player, s8 playerIndex) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        ChangeTirecolCalc(Player, playerIndex);
        BrightCalc(Player, playerIndex);
        if (((Player->SlipFlag & BROKEN) == BROKEN) ||
            ((Player->SlipFlag & RESTORE) == RESTORE)) {
            if ((Player->SlipFlag & BROKEN) == BROKEN) {
                BrokenCalc(Player, playerIndex);
            }
            if ((Player->SlipFlag & RESTORE) == RESTORE) {
                RestoreCalc(Player, playerIndex);
            }
        } else {
            BoundCalc(Player, playerIndex);
        }
        ShakeSet(Player);
    }
}

void BrokenCalc(Player* Player, UNUSED s8 playerIndex) {
    f32 temp_f0;
    s16 var_v0;

    var_v0 = Player->sus.BrokenTimer;
    temp_f0 = Player->sus.BrokenFirstspeed;
    if (var_v0 < 5) {
        var_v0++;
    }

    Player->sus.Broken = (var_v0 * temp_f0) - (0.7 * (var_v0 * var_v0));
    Player->sus.BrokenFirstspeed = temp_f0;
    Player->sus.BrokenTimer = var_v0;
}

void RestoreCalc(Player* Player, UNUSED s8 playerIndex) {
    f32 temp_f0 = Player->sus.BrokenFirstspeed;
    s16 temp_v0 = Player->sus.BrokenTimer;
    s16 temp_f16;

    temp_v0++;

    temp_f16 = (temp_v0 * temp_f0) - (0.5 * (temp_v0 * temp_v0));

    if ((temp_v0 != 0) && (temp_f16 < 0)) {
        temp_f0 *= 0.8;
        temp_v0 = 0;
        if (temp_f0 <= 0.1) {
            Player->SlipFlag &= ~RESTORE;
            temp_f0 = 0.0f;
        }
    }
    if (temp_f16 <= 0) {
        temp_f16 = 0;
    }
    Player->sus.Broken = temp_f16;
    Player->sus.BrokenFirstspeed = temp_f0;
    Player->sus.BrokenTimer = temp_v0;
}

// look like related to flatten animation
void BrokenSet(Player* Player, s8 PlayerId, UNUSED s8 screenId, s8 flipOffset) {
    s16 temp_v0 = Player->sus.Broken;

    KartVtx[PlayerId][flipOffset + 0x0].v.ob[1] = 18 - (temp_v0 * 2.3);
    KartVtx[PlayerId][flipOffset + 0x1].v.ob[1] = 9 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x2].v.ob[1] = 9 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x3].v.ob[1] = 18 - (temp_v0 * 2.3);
    KartVtx[PlayerId][flipOffset + 0x4].v.ob[1] = 9 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x7].v.ob[1] = 9 - temp_v0;
}

/**
 * Seems to stretch/warp a specific players Texture for a
 * short period of Time. Perhaps does not do anything
 **/
void JugemuSet(UNUSED Player* Player, s8 PlayerId, UNUSED s8 screenId, s8 flipOffset) {
    KartVtx[PlayerId][flipOffset].v.ob[1] = 21;
    KartVtx[PlayerId][flipOffset + 0x3].v.ob[1] = 21;
}

void BoundCalc(Player* Player, UNUSED s8 playerIndex) {
    f32 temp_f0 = Player->sus.BoundFirstspeed;
    s16 temp_v0 = Player->sus.BoundTimer;
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
    Player->sus.Bound = temp_f16;
    Player->sus.BoundFirstspeed = temp_f0;
    Player->sus.BoundTimer = temp_v0;
}

void BoundSet(Player* Player, s8 PlayerId, UNUSED s8 screenId, s8 flipOffset) {
    s16 temp_v0 = Player->sus.Bound;

    KartVtx[PlayerId][flipOffset + 0x0].v.ob[1] = 18 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x1].v.ob[1] = 9 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x2].v.ob[1] = 9 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x3].v.ob[1] = 18 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x4].v.ob[1] = 9 - temp_v0;
    KartVtx[PlayerId][flipOffset + 0x7].v.ob[1] = 9 - temp_v0;
}

/**
 * Sets Player shading/colour.
 */
void MakeBodyColor(UNUSED Player* Player, s8 playerIndex, s32 Arg2, f32 Alpha) {
    ColorPlayer0R[playerIndex] =
        (s16) ((f32) ColorPlayer0R[playerIndex] - ((ColorPlayer0R[playerIndex] - ((Arg2 >> 16) & 0xFF)) * Alpha));

    BrightG[playerIndex] =
        (s16) ((f32) BrightG[playerIndex] - ((BrightG[playerIndex] - ((Arg2 >> 8) & 0xFF)) * Alpha));

    BrightB[playerIndex] =
        (s16) ((f32) BrightB[playerIndex] - ((BrightB[playerIndex] - (Arg2 & 0xFF)) * Alpha));
}

void MakeBodyColorAdjust(UNUSED Player* Player, s8 playerIndex, s32 Arg2, f32 Arg3) {
    ChaseUSnumber(&AdjustR[playerIndex], (Arg2 >> 16) & 0xFF, Arg3);
    ChaseUSnumber(&AdjustG[playerIndex], (Arg2 >> 8) & 0xFF, Arg3);
    ChaseUSnumber(&AdjustB[playerIndex], Arg2 & 0xFF, Arg3);
}

/**
 * Activates in the tunnel to shade the Player a bit darker
 * Sort of an atmospheric effect.
 */
bool CheckMapBrightBefore(Player* Player, s8 playerIndex) {
    switch (CourseID) {
        case COURSE_LUIGI_RACEWAY:
            if (((NearestPathPointByPlayerId[playerIndex] >= 0x14F) && (NearestPathPointByPlayerId[playerIndex] < 0x158)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x15E) && (NearestPathPointByPlayerId[playerIndex] < 0x164)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x169) && (NearestPathPointByPlayerId[playerIndex] < 0x170)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x174) && (NearestPathPointByPlayerId[playerIndex] < 0x17A)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x17E) &&
                 (NearestPathPointByPlayerId[playerIndex] < 0x184))) { // under a light in the tunnel
                MakeBodyColor(Player, playerIndex, COLOR_LIGHT, 0.3f);
                MakeBodyColorAdjust(Player, playerIndex, 0xE0, 0.3f);
                ColorTimer[playerIndex] = 0;
                return true;
            }
            return false;

        default:
            return false;
    }
}

void CheckMapBrightAfter(Player* Player, s8 playerIndex) {
    switch (CourseID) {
        case COURSE_BOWSER_CASTLE:
            if (((NearestPathPointByPlayerId[playerIndex] >= 0x15) && (NearestPathPointByPlayerId[playerIndex] < 0x2A)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x14D) && (NearestPathPointByPlayerId[playerIndex] < 0x15C)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x1D1) && (NearestPathPointByPlayerId[playerIndex] < 0x1E4)) ||
                (Player->Bump.DistanceZX >= 500.0f)) { // over lava
                MakeBodyColor(Player, playerIndex, COLOR_LAVA, 0.3f);
                MakeBodyColorAdjust(Player, playerIndex, 0x004040, 0.3f);
                ColorTimer[playerIndex] = 0;
            } else if (((NearestPathPointByPlayerId[playerIndex] >= 0xF1) && (NearestPathPointByPlayerId[playerIndex] < 0xF5)) ||
                       ((NearestPathPointByPlayerId[playerIndex] >= 0xFB) && (NearestPathPointByPlayerId[playerIndex] < 0xFF)) ||
                       ((NearestPathPointByPlayerId[playerIndex] >= 0x105) && (NearestPathPointByPlayerId[playerIndex] < 0x109)) ||
                       ((NearestPathPointByPlayerId[playerIndex] >= 0x10F) && (NearestPathPointByPlayerId[playerIndex] < 0x113)) ||
                       ((NearestPathPointByPlayerId[playerIndex] >= 0x145) && (NearestPathPointByPlayerId[playerIndex] < 0x14A)) ||
                       ((NearestPathPointByPlayerId[playerIndex] >= 0x15E) &&
                        (NearestPathPointByPlayerId[playerIndex] < 0x163))) { // under a lamp
                MakeBodyColor(Player, playerIndex, COLOR_LIGHT, 0.3f);
                MakeBodyColorAdjust(Player, playerIndex, 0xE0, 0.3f);
                ColorTimer[playerIndex] = 0;
            } else { // normal color
                MakeBodyColor(Player, playerIndex, COLOR_BLACK, 0.3f);
                MakeBodyColorAdjust(Player, playerIndex, 0, 0.3f);
                ColorTimer[playerIndex] = 0;
            }
            break;
        case COURSE_BANSHEE_BOARDWALK:
            if (((NearestPathPointByPlayerId[playerIndex] >= 0xD) && (NearestPathPointByPlayerId[playerIndex] < 0x15)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x29) && (NearestPathPointByPlayerId[playerIndex] < 0x39)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x46) && (NearestPathPointByPlayerId[playerIndex] < 0x4E)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x5F) && (NearestPathPointByPlayerId[playerIndex] < 0x67)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x7B) && (NearestPathPointByPlayerId[playerIndex] < 0x86)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x9D) && (NearestPathPointByPlayerId[playerIndex] < 0xA6)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0xB9) && (NearestPathPointByPlayerId[playerIndex] < 0xC3)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0xB9) && (NearestPathPointByPlayerId[playerIndex] < 0xC3)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0xD7) && (NearestPathPointByPlayerId[playerIndex] < 0xE1)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x10E) && (NearestPathPointByPlayerId[playerIndex] < 0x119)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x154) && (NearestPathPointByPlayerId[playerIndex] < 0x15F)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x1EF) && (NearestPathPointByPlayerId[playerIndex] < 0x1F7)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x202) && (NearestPathPointByPlayerId[playerIndex] < 0x209)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x216) && (NearestPathPointByPlayerId[playerIndex] < 0x21D)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x230) && (NearestPathPointByPlayerId[playerIndex] < 0x23A)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x24C) && (NearestPathPointByPlayerId[playerIndex] < 0x256)) ||
                // This is the second last lamp. This condition is bugged and it does not glow on the Player.
                // The Value 0x228 was probably meant to be 0x25F.
                ((NearestPathPointByPlayerId[playerIndex] >= 0x288) && (NearestPathPointByPlayerId[playerIndex] < 0x269)) ||
                ((NearestPathPointByPlayerId[playerIndex] >= 0x274) &&
                 (NearestPathPointByPlayerId[playerIndex] < 0x27E))) { // under a lamp
                MakeBodyColor(Player, playerIndex, COLOR_LIGHT, 0.3f);
                MakeBodyColorAdjust(Player, playerIndex, 0x0000E0, 0.3f);
                ColorTimer[playerIndex] = 0;
            } else {
                MakeBodyColor(Player, playerIndex, COLOR_BLACK, 0.3f);
                MakeBodyColorAdjust(Player, playerIndex, 0, 0.3f);
                ColorTimer[playerIndex] = 0;
            }
            break;
        default:
            MakeBodyColor(Player, playerIndex, COLOR_BLACK, 0.3f);
            MakeBodyColorAdjust(Player, playerIndex, 0, 0.3f);
            ColorTimer[playerIndex] = 0;
            break;
    }
}

void BrightCalc(Player* Player, s8 playerIndex) {
    s32 time_elapsed;

    if (((Player->Flag & IS_GHOST) == IS_GHOST) && (Player == PlayerThree)) {
        MakeBodyColor(Player, playerIndex, COLOR_LIGHT, 0.3f);
        MakeBodyColorAdjust(Player, playerIndex, 0xE0, 0.3f);
        ColorTimer[playerIndex] = 0;
        return;
    }

    if (((Player->JugemuFlag & IS_FROZEN) == IS_FROZEN) &&
        ((Player->JugemuFlag & IS_FADING_OUT) == IS_FADING_OUT)) {
        MakeBodyColor(Player, playerIndex, 0x646464, 0.5f);
        MakeBodyColorAdjust(Player, playerIndex, 0xFF0000, 0.1f);
        return;
    }
    if ((Player->JugemuFlag & IS_FADING_OUT) == IS_FADING_OUT) {
        MakeBodyColor(Player, playerIndex, COLOR_BLACK, 1.0f);
        MakeBodyColorAdjust(Player, playerIndex, 0, 1.0f);
        return;
    }
    if ((Player->JugemuFlag & IS_FROZEN) == IS_FROZEN) {
        MakeBodyColor(Player, playerIndex, 0x646464, 0.5f);
        MakeBodyColorAdjust(Player, playerIndex, 0xFF0000, 0.1f);
        return;
    }
    if ((Player->JugemuFlag & IS_MELTING) == IS_MELTING) {
        MakeBodyColor(Player, playerIndex, COLOR_BLACK, 0.1f);
        MakeBodyColorAdjust(Player, playerIndex, 0, 0.1f);
        return;
    }

    if (((Player->SlipFlag & THUNDER) == THUNDER) && ((s32) Player->ThunderTimer < 0x78)) {
        ColorTimer[playerIndex] += 5;
        if (ColorTimer[playerIndex] >= 0x1E) {
            ColorTimer[playerIndex] = 0;
        }
        if ((ColorTimer[playerIndex] >= 0) && (ColorTimer[playerIndex] < 0xB)) {
            MakeBodyColor(Player, playerIndex, 0x808080, 0.8f);
            MakeBodyColorAdjust(Player, playerIndex, 0, 0.8f);
        }
        if ((ColorTimer[playerIndex] >= 0xB) && (ColorTimer[playerIndex] < 0x15)) {
            MakeBodyColor(Player, playerIndex, 0x70, 0.8f);
            MakeBodyColorAdjust(Player, playerIndex, 0, 0.8f);
        }
        if ((ColorTimer[playerIndex] >= 0x15) && (ColorTimer[playerIndex] < 0x1F)) {
            MakeBodyColor(Player, playerIndex, 0x8F8F00, 0.8f);
            MakeBodyColorAdjust(Player, playerIndex, 0, 0.8f);
        }
        return;
    }
    if ((Player->SlipFlag & STAR) != 0) {
        time_elapsed = (s32) GameTimer - StarUseCounter[playerIndex];
        if (time_elapsed <= 8) {

            if (time_elapsed >= 7) {
                ColorTimer[playerIndex] += 10;
            } else {
                ColorTimer[playerIndex] += 5;
            }
            if (ColorTimer[playerIndex] >= 40) {
                ColorTimer[playerIndex] = 0;
            }
            if ((ColorTimer[playerIndex] >= 0) && (ColorTimer[playerIndex] <= 10)) {
                MakeBodyColor(Player, playerIndex, 0x70, 0.8f);
                MakeBodyColorAdjust(Player, playerIndex, 0, 0.8f);
            }
            if ((ColorTimer[playerIndex] >= 0xB) && (ColorTimer[playerIndex] <= 20)) {
                MakeBodyColor(Player, playerIndex, 0x707000, 0.8f);
                MakeBodyColorAdjust(Player, playerIndex, 0, 0.8f);
            }
            if ((ColorTimer[playerIndex] >= 0x15) && (ColorTimer[playerIndex] <= 30)) {
                MakeBodyColor(Player, playerIndex, 0x700000, 0.8f);
                MakeBodyColorAdjust(Player, playerIndex, 0, 0.8f);
            }
            if (ColorTimer[playerIndex] >= 0x1F) {
                MakeBodyColor(Player, playerIndex, 0x7000, 0.8f);
                MakeBodyColorAdjust(Player, playerIndex, 0, 0.8f);
            }
            return;
        }
    }
    if (CheckMapBrightBefore(Player, playerIndex) != true) {
        if (((Player->TireRR.Dummy & 1) == 1) || ((Player->TireRR.Dummy & 2) == 2) ||
            ((Player->TireFL.Dummy & 3) == 3)) {
            MakeBodyColor(Player, playerIndex, COLOR_BLACK, 0.3f);
            MakeBodyColorAdjust(Player, playerIndex, 0x6F6F6F, 0.3f);
            return;
        }
        CheckMapBrightAfter(Player, playerIndex);
        if ((Player->JugemuFlag & LAVA_EFFECT) == LAVA_EFFECT) {
            MakeBodyColor(Player, playerIndex, COLOR_BLACK, 0.3f);
            MakeBodyColorAdjust(Player, playerIndex, 0xF0F0F0, 0.3f);
        }
    }
}

void CheckVTX(Player* Player, s8 PlayerId, s8 screenId, s8 Arg3) {
    if (((Player->SlipFlag & BROKEN) == BROKEN) ||
        ((Player->SlipFlag & RESTORE) == RESTORE)) {
        BrokenSet(Player, PlayerId, screenId, Arg3);
    } else {
        BoundSet(Player, PlayerId, screenId, Arg3);
    }
    if ((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) {
        JugemuSet(Player, PlayerId, screenId, Arg3);
    }
}

void DrawShadow(Player* Player, s8 PlayerId, s8 screenId) {
    Mat4 sp118;
    UNUSED Mat4 pad;
    Vec3f spCC;
    Vec3s spC4;
    s16 temp_t9;
    s16 spC0;
    Vec3f spB4;
    f32 spB0;
    f32 spAC;
    UNUSED Vec3f Pad2;
    f32 var_f2;

    temp_t9 = (u16) (Player->AngleY[screenId] + Player->Direction[1] + Player->Slipang) / 128; // << 7) & 0xFFFF;
    spC0 = -Player->Direction[1] - Player->Slipang;

    spB0 = -CosT(temp_t9 << 7) * 2;
    spAC = -SinT(temp_t9 << 7) * 2;

    if (((Player->SlipFlag & EXPLODE) == EXPLODE) ||
        ((Player->SlipFlag & ROLLOVER) == ROLLOVER) ||
        ((Player->SlipFlag & WING) == WING) ||
        ((Player->SlipFlag & STORM) == STORM) ||
        ((Player->SlipFlag & ROLLOVER) == ROLLOVER) ||
        ((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) ||
        ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL) ||
        ((Player->SlipFlag & N_JUMP) == N_JUMP)) {

        var_f2 = (f32) (1.0 - ((f64) Player->Bump.DistanceZX * 0.02));
        if (var_f2 < 0.0f) {
            var_f2 = 0.0f;
        }
        if (var_f2 > 1.0f) {
            var_f2 = 1.0f;
        }
        spB4[0] = Player->Bump.BumpZX[0];
        spB4[2] = Player->Bump.BumpZX[2];
        spB4[1] = Player->Bump.BumpZX[1];

        spCC[0] = Player->Position[0] + ((spB0 * SinT(spC0)) + (spAC * CosT(spC0)));
        spCC[1] = Player->Ground + 1.0f;
        spCC[2] = Player->Position[2] + ((spB0 * CosT(spC0)) - (spAC * SinT(spC0)));
        KwcreateRotationMatrix(sp118, spB4, spCC, (spC0 + Player->KageAng),
                             CharacterSize[Player->Kart] * Player->Offsetsize * var_f2);
    } else {
        spC4[0] = Player->Kartpitch;
        spC4[1] = spC0;
        spC4[2] = Player->Kartroll * 2;

        spCC[0] = Player->Position[0] + ((spB0 * SinT(spC0)) + (spAC * CosT(spC0)));
        spCC[1] = Player->Ground + 1.0f;
        spCC[2] = Player->Position[2] + ((spB0 * CosT(spC0)) - (spAC * SinT(spC0)));
        Createaffine(sp118, spCC, spC4);
        ScaleMatrix(sp118, CharacterSize[Player->Kart] * Player->Offsetsize);
    }
    Affine2Mtx(&DynamicP->MTXShadow[PlayerId + (screenId * 8)], sp118);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXShadow[PlayerId + (screenId * 8)]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(DisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(DisplayListHead++, KageBitmap, G_IM_FMT_I, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    SplashMode(0, 0, 0, 0xFF);
    gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(DisplayListHead++, &D_800E51D0[0], 4, 0);

    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gDPLoadTextureBlock(DisplayListHead++, (KageBitmap + SOME_TEXTURE_POINTER_MATH), G_IM_FMT_I,
                        G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    SplashMode(0, 0, 0, 0xFF);
    gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(DisplayListHead++, &D_800E5210[0], 4, 0);

    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void DrawResultShadow(Player* Player, s8 PlayerId, s8 Arg2) {
    Mat4 sp118;
    UNUSED Mat4 pad;
    Vec3f spCC;
    Vec3s spC4;
    s16 temp_t9;
    s16 spC0;
    UNUSED Vec3f Pad2;
    f32 spB0;
    f32 spAC;
    UNUSED Vec3f Pad3;
    Vec3f sp94 = { 9.0f, 7.0f, 5.0f };

    temp_t9 = (u16) (Player->AngleY[Arg2] + Player->Direction[1] + Player->Slipang) / 128;
    spC0 = -Player->Direction[1] - Player->Slipang;

    spB0 = -CosT(temp_t9 << 7) * 3;
    spAC = -SinT(temp_t9 << 7) * 3;

    spC4[0] = 0;
    spC4[1] = spC0;
    spC4[2] = 0;

    spCC[0] = Player->Position[0] + ((spB0 * SinT(spC0)) + (spAC * CosT(spC0)));
    spCC[2] = Player->Position[2] + ((spB0 * CosT(spC0)) - (spAC * SinT(spC0)));
    spCC[1] = DynamicObjects[Objallocptr[PlayerId]].pos[1] + sp94[PlayerId];

    Createaffine(sp118, spCC, spC4);
    ScaleMatrix(sp118, CharacterSize[Player->Kart] * Player->Offsetsize);
    Affine2Mtx(&DynamicP->MTXShadow[PlayerId + (Arg2 * 8)], sp118);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXShadow[PlayerId + (Arg2 * 8)]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(DisplayListHead++, D_0D008D58);
    gDPSetTextureLUT(DisplayListHead++, G_TT_NONE);
    gDPLoadTextureBlock(DisplayListHead++, KageBitmap, G_IM_FMT_I, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    SplashMode(0, 0, 0, 0x000000D0);
    gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(DisplayListHead++, &D_800E51D0[0], 4, 0);

    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gDPLoadTextureBlock(DisplayListHead++, (KageBitmap + SOME_TEXTURE_POINTER_MATH), G_IM_FMT_I,
                        G_IM_SIZ_8b, 64, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK,
                        G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    SplashMode(0, 0, 0, 0x000000D0);
    gDPSetRenderMode(DisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
    gSPVertex(DisplayListHead++, &D_800E5210[0], 4, 0);

    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
}

void DrawMainkart(Player* Player, s8 PlayerId, s8 Arg2, s8 flipOffset) {
    UNUSED s32 pad;
    Mat4 sp1A4;
    UNUSED s32 Pad2[17];
    Vec3f sp154;
    Vec3s sp14C;
    f32 sp148;
    f32 sp144;
    f32 sp140;
    s16 temp_v1;
    s16 thing;

    if (Player->HandlingFlag & TROPHYJUMP_KEEP) {
        sp14C[0] = 0;
        sp14C[1] = Player->AngleY[Arg2];
        sp14C[2] = 0;
        MakeSmokepos(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -Player->AngleY[Arg2], Player->AngleZ[Arg2]);
        sp154[1] = (Player->Position[1] - Player->Radius) + (sp144 - 2.0);
        sp154[0] = Player->Position[0] + sp148;
        sp154[2] = Player->Position[2] + sp140;
    } else {
        thing = (u16) (Player->AngleY[Arg2] + Player->Direction[1] + Player->Slipang);
        temp_v1 = Player->Roll[Arg2] * SinT(thing);
        if ((Player->SlipFlag & N_JUMP) == N_JUMP) {
            sp14C[0] = Cameras[Arg2].CameraDirection[0] - DEGREES(90);
        } else {
            sp14C[0] = -temp_v1 * 0.8;
        }
        sp14C[1] = Player->AngleY[Arg2];
        sp14C[2] = Player->AngleZ[Arg2];
        if (((s32) Player->SlipFlag & BROKEN) == BROKEN) {
            MakeSmokepos(&sp148, &sp144, &sp140, 0.0f, 8.0f, 0.0f, -Player->AngleY[Arg2], Player->AngleZ[Arg2]);
            sp154[1] = (Player->Position[1] - Player->Radius) + Player->Shake;
            sp154[0] = Player->Position[0] + sp148;
            sp154[2] = Player->Position[2] + sp140;
        } else {
            MakeSmokepos(&sp148, &sp144, &sp140, 0.0f, 1.5f, 0.0f, -Player->AngleY[Arg2], Player->AngleZ[Arg2]);
            sp154[1] = (Player->Position[1] - Player->Radius) + Player->Shake + (sp144 - 2.0);
            sp154[0] = Player->Position[0] + sp148;
            sp154[2] = Player->Position[2] + sp140;
        }
    }
#ifdef AVOID_UB
    PlayerPalette = &PlayerPalettesList[DrawNumber[Arg2][PlayerId]][Arg2][PlayerId];
#else
    PlayerPalette = (struct_D_802F1F80*) &PlayerPalettesList[DrawNumber[Arg2][PlayerId]][Arg2][PlayerId * 0x100];
#endif
    if ((Arg2 == 0) || (Arg2 == 1)) {
        SKartUpperTexture = &D_802BFB80.arraySize8[DrawNumber[Arg2][PlayerId]][Arg2][PlayerId].PixelIndexArray[0];
        SKartLowerTexture = &D_802BFB80.arraySize8[DrawNumber[Arg2][PlayerId]][Arg2][PlayerId].PixelIndexArray[0x7C0];
    } else {
        SKartUpperTexture =
            &D_802BFB80.arraySize8[DrawNumber[Arg2][PlayerId]][Arg2 - 1][PlayerId - 4].PixelIndexArray[0];
        SKartLowerTexture =
            &D_802BFB80.arraySize8[DrawNumber[Arg2][PlayerId]][Arg2 - 1][PlayerId - 4].PixelIndexArray[0x7C0];
    }
    Createaffine(sp1A4, sp154, sp14C);
    ScaleMatrix(sp1A4, CharacterSize[Player->Kart] * Player->Offsetsize);
    Affine2Mtx(&DynamicP->MTXKart[PlayerId + (Arg2 * 8)], sp1A4);

    if ((Player->SlipFlag & TERESA) == TERESA) {
        if (Arg2 == PlayerId) {
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXKart[PlayerId + (Arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(DisplayListHead++, CommonSettingRenderCharacter);
            gDPLoadTLUT_pal256(DisplayListHead++, PlayerPalette);
            gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
            IceMode(ColorPlayer0R[PlayerId], BrightG[PlayerId], BrightB[PlayerId],
                          AdjustR[PlayerId], AdjustG[PlayerId], AdjustB[PlayerId],
                          (s32) Player->Erase);
            gDPSetRenderMode(DisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        } else {
            gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXKart[PlayerId + (Arg2 * 8)]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(DisplayListHead++, CommonSettingRenderCharacter);
            gDPLoadTLUT_pal256(DisplayListHead++, PlayerPalette);
            gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
            IceMode(ColorPlayer0R[PlayerId], BrightG[PlayerId], BrightB[PlayerId],
                          AdjustR[PlayerId], AdjustG[PlayerId], AdjustB[PlayerId],
                          GhostUseTimer[PlayerId]);
            gDPSetRenderMode(DisplayListHead++,
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                             AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                                 GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));
        }
    } else if (((Player->JugemuFlag & IS_FADING_OUT) == IS_FADING_OUT) || (Player->Weapon & PRESS_IS_BOMB) ||
               (Player->Weapon & PRESS_BECOME_BOMB)) {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXKart[PlayerId + (Arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(DisplayListHead++, CommonSettingRenderCharacter);
        gDPLoadTLUT_pal256(DisplayListHead++, PlayerPalette);
        gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
        IceMode(ColorPlayer0R[PlayerId], BrightG[PlayerId], BrightB[PlayerId],
                      AdjustR[PlayerId], AdjustG[PlayerId], AdjustB[PlayerId],
                      (s32) Player->Erase);
        gDPSetAlphaCompare(DisplayListHead++, G_AC_DITHER);
        gDPSetRenderMode(DisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    } else {
        gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXKart[PlayerId + (Arg2 * 8)]),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(DisplayListHead++, CommonSettingRenderCharacter);
        gDPLoadTLUT_pal256(DisplayListHead++, PlayerPalette);
        gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
        IceMode(ColorPlayer0R[PlayerId], BrightG[PlayerId], BrightB[PlayerId],
                      AdjustR[PlayerId], AdjustG[PlayerId], AdjustB[PlayerId],
                      (s32) Player->Erase);
        gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    }

    gDPLoadTextureBlock(DisplayListHead++, SKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);

    gDPLoadTextureBlock(DisplayListHead++, SKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset + 4], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
}

void DrawGhostkart(Player* Player, s8 PlayerId, s8 screenId, s8 flipOffset) {
    UNUSED s32 pad;
    Mat4 sp12C;
    UNUSED s32 Pad2[17];
    Vec3f spDC;
    Vec3s spD4;
    f32 spD0;
    f32 spCC;
    f32 spC8;
    UNUSED s32 Pad3;
    s16 spC2;
    s16 thing;

    if (screenId) {}
    if (ReplayFlag == 1) {
        spC2 = 0x00FF;
    } else {
        spC2 = 0x0070;
    }
    thing = (u16) (Player->AngleY[screenId] - Player->Direction[1]);
    spD4[0] = (-(s16) (SinT(thing) * (0.0f * 0.0f)) * 0.8);
    spD4[1] = Player->AngleY[screenId];
    spD4[2] = Player->AngleZ[screenId];
    MakeSmokepos(&spD0, &spCC, &spC8, 0, 1.5f, 0, -Player->AngleY[screenId], Player->AngleZ[screenId]);
    spDC[1] = (Player->Position[1] - Player->Radius) + (spCC - 2.0);
    spDC[0] = Player->Position[0] + spD0;
    spDC[2] = Player->Position[2] + spC8;
#ifdef AVOID_UB
    PlayerPalette = &PlayerPalettesList[DrawNumber[screenId][PlayerId]][screenId][PlayerId];
#else
    PlayerPalette =
        (struct_D_802F1F80*) &PlayerPalettesList[DrawNumber[screenId][PlayerId]][screenId][PlayerId * 0x100];
#endif
    if ((screenId == 0) || (screenId == 1)) {
        SKartUpperTexture =
            &D_802BFB80.arraySize8[DrawNumber[screenId][PlayerId]][screenId][PlayerId].PixelIndexArray[0];
        SKartLowerTexture =
            &D_802BFB80.arraySize8[DrawNumber[screenId][PlayerId]][screenId][PlayerId].PixelIndexArray[0x7C0];
    } else {
        SKartUpperTexture =
            &D_802BFB80.arraySize8[DrawNumber[screenId][PlayerId]][screenId - 1][PlayerId - 4].PixelIndexArray[0];
        SKartLowerTexture =
            &D_802BFB80.arraySize8[DrawNumber[screenId][PlayerId]][screenId - 1][PlayerId - 4].PixelIndexArray[0x7C0];
    }

    Createaffine(sp12C, spDC, spD4);
    ScaleMatrix(sp12C, CharacterSize[Player->Kart] * Player->Offsetsize);
    Affine2Mtx(&DynamicP->MTXKart[PlayerId + (screenId * 8)], sp12C);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXKart[PlayerId + (screenId * 8)]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, CommonSettingRenderCharacter);
    gDPLoadTLUT_pal256(DisplayListHead++, PlayerPalette);
    gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
    IceMode(ColorPlayer0R[PlayerId], BrightG[PlayerId], BrightB[PlayerId],
                  AdjustR[PlayerId], AdjustG[PlayerId], AdjustB[PlayerId], spC2);
    gDPSetRenderMode(DisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));

    gDPLoadTextureBlock(DisplayListHead++, SKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);

    gDPLoadTextureBlock(DisplayListHead++, SKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset + 4], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    gDPSetAlphaCompare(DisplayListHead++, G_AC_NONE);
}

void DrawTurbokart(Player* Player, s8 PlayerId, s8 screenId, s8 flipOffset) {
    Mat4 spA8;
    Vec3f sp9C;
    Vec3s sp94;

    sp9C[0] = Player->Position[0] + (SinT(-Player->Direction[1]) * -1.5);
    sp9C[1] = ((Player->Position[1] - Player->Radius) + Player->Shake) + 0.1;
    sp9C[2] = Player->Position[2] + (CosT(-Player->Direction[1]) * -1.5);
    sp94[0] = -DEGREES(1);
    sp94[1] = Player->AngleY[screenId];
    sp94[2] = Player->AngleZ[screenId];

    Createaffine(spA8, sp9C, sp94);
    ScaleMatrix(spA8, CharacterSize[Player->Kart] * Player->Offsetsize);
    Affine2Mtx(&DynamicP->MTXEffect[Effectcount], spA8);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, D_0D008D10);
    gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
    IceMode(ColorPlayer0R[PlayerId], BrightG[PlayerId], BrightB[PlayerId],
                  AdjustR[PlayerId], AdjustG[PlayerId], AdjustB[PlayerId],
                  0x00000040);
    gDPSetRenderMode(DisplayListHead++,
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c1(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA),
                     AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_DST_WRAP | ZMODE_XLU | CVG_X_ALPHA | FORCE_BL |
                         GBL_c2(G_BL_CLR_IN, G_BL_A_IN, G_BL_CLR_MEM, G_BL_1MA));

    gDPLoadTextureBlock(DisplayListHead++, SKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);

    gDPLoadTextureBlock(DisplayListHead++, SKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset + 4], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    Effectcount += 1;
}

void DrawReflectkart(Player* Player, s8 PlayerId, s8 screenId, s8 flipOffset) {
    Mat4 spA8;
    Vec3f sp9C;
    Vec3s sp94;

    sp94[0] = 0;
    sp94[1] = Player->AngleY[screenId];
    sp94[2] = Player->AngleZ[screenId] + 0x8000; // invert Y
    sp9C[0] = Player->Position[0];
    sp9C[1] = Player->Ground + (4.0f * Player->Offsetsize);
    sp9C[2] = Player->Position[2];
    if (!(Player->Status & (P1_FLIP_SPRITE << (screenId * 4)))) {
        flipOffset = 8;
    } else {
        flipOffset = 0;
    }

    Createaffine(spA8, sp9C, sp94);
    ScaleMatrix(spA8, CharacterSize[Player->Kart] * Player->Offsetsize);
    Affine2Mtx(&DynamicP->MTXEffect[Effectcount], spA8);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, CommonSettingRenderCharacter);
    gDPSetTextureLUT(DisplayListHead++, G_TT_RGBA16);
    IceMode(ColorPlayer0R[PlayerId], BrightG[PlayerId], BrightB[PlayerId],
                  AdjustR[PlayerId], AdjustG[PlayerId], AdjustB[PlayerId],
                  (s16) Player->Erase / 2);
    gDPSetRenderMode(DisplayListHead++, G_RM_ZB_XLU_SURF, G_RM_ZB_XLU_SURF2);
    gDPLoadTextureBlock(DisplayListHead++, SKartUpperTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gDPLoadTextureBlock(DisplayListHead++, SKartLowerTexture, G_IM_FMT_CI, G_IM_SIZ_8b, 64, 32, 0,
                        G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                        G_TX_NOLOD);
    gSPVertex(DisplayListHead++, &KartVtx[PlayerId][flipOffset + 4], 4, 0);
    gSPDisplayList(DisplayListHead++, CommonSquarePlainRender);
    gSPTexture(DisplayListHead++, 1, 1, 0, G_TX_RENDERTILE, G_OFF);
    Effectcount += 1;
}

void Doobj(Player* Player, s8 PlayerId, s8 screenId) {
    UNUSED s32 pad[2];
    s32 temp_t1;
    s32 flipOffset;
    OSMesg* sp34;

    ChangeTirecolPa(Player, PlayerId, screenId, DrawNumber[screenId][PlayerId]);
    if (!(Player->Status & (P1_FLIP_SPRITE << (screenId * 4)))) {
        flipOffset = 0;
    } else {
        flipOffset = 8;
    }
    CheckVTX(Player, PlayerId, screenId, flipOffset);
    temp_t1 = P1_DRAW_PARTICLES << (screenId * 4);
    if ((temp_t1 == (Player->Status & temp_t1)) && (Player->Bump.DistanceZX <= 50.0f) &&
        (Player->BumpStatus != ICE)) {
        if ((Player->SlipFlag & TERESA) == TERESA) {
            if (PlayerId == screenId) {
                DrawShadow(Player, PlayerId, screenId);
            }
        } else {
            DrawShadow(Player, PlayerId, screenId);
        }
    }
    if ((Player->Flag & IS_GHOST) != IS_GHOST) {
        DrawMainkart(Player, PlayerId, screenId, flipOffset);
    } else {
        DrawGhostkart(Player, PlayerId, screenId, flipOffset);
    }
    osRecvMesg(&DmaMesgQueue, (OSMesg*) &sp34, OS_MESG_BLOCK);
    if ((temp_t1 == (Player->Status & temp_t1)) && (Player->BumpStatus == ICE) &&
        ((Player->JugemuFlag & IS_IN_WATER) != IS_IN_WATER) &&
        (Player->Bump.DistanceZX <= 30.0f)) {
        DrawReflectkart(Player, PlayerId, screenId, flipOffset);
    }
    if (Player->TurboPower >= 2.0f) {
        DrawTurbokart(Player, PlayerId, screenId, flipOffset);
    }
}

void ChangeTirecolCalc(Player* Player, s8 playerIndex) {
    f32 temp_f0;

    if (((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) &&
        ((Player->Flag & NO_CONTROLS) == 0)) {
        Player->TcolCount += D_800DDE74[8];
        if (Player->TcolCount >= 0x400) {
            Player->TcolCount = 0;
        }
        return;
    }

    temp_f0 = ((Player->Speed * (1.0f + Player->PowerCont)) / 18.0f) * 216.0f;
    if ((temp_f0 <= 1.0f) || (FullBrakeAccele[playerIndex] == true)) {
        Player->TcolCount = 0;
    } else {
        Player->TcolCount += D_800DDE74[(s32) (temp_f0 / 12.0f)];
    }
    if (Player->TcolCount >= 0x400) {
        Player->TcolCount = 0;
    }
}

// Properly define struct pointers, see buffers.h comment for more information.
#ifdef AVOID_UB
#define D_802F1F80_WHEEL(a, b, c) &PlayerPalettesList[a][b][c].WheelPalette
#else
#define D_802F1F80_WHEEL(a, b, c) &PlayerPalettesList[a][b][(c * 0x100) + 0xC0]
#endif

void ChangeTirecolPa(Player* Player, s8 PlayerId, s8 screenId, s8 Arg3) {
    s16 frameId = OldDir[screenId][PlayerId];
    s16 groupId = OldDeg[screenId][PlayerId];
    s16 temp_t2 = Player->TcolCount;
    s16 temp_num = 0x40; // setting this as a variable gets rid of regalloc

    if (((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) &&
        ((Player->Flag & NO_CONTROLS) == 0)) {
        if (((Player->SlipFlag & SPIN_L) != SPIN_L) &&
            ((Player->SlipFlag & SPIN_R) != SPIN_R) &&
            ((Player->SlipFlag & THUNDER_SPIN) != THUNDER_SPIN) &&
            ((Player->SlipFlag & WING) != WING) &&
            ((Player->SlipFlag & STORM) != STORM) &&
            ((Player->HandlingFlag & TROPHYJUMP_END) == 0)) {

            if (frameId <= 20) {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels0[Player->Kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            } else {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels1[Player->Kart][groupId] +
                                                     (frameId - 21) * (temp_num * 4) + ((temp_t2 >> 8) * 0x40) + 0x600),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            }
        } else {
            if (frameId == 0) {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels0[Player->Kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            } else {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels1[Player->Kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            }
        }
    } else {
        if (((Player->SlipFlag & SPIN_L) != SPIN_L) &&
            ((Player->SlipFlag & SPIN_R) != SPIN_R) &&
            ((Player->SlipFlag & WING) != WING) &&
            ((Player->SlipFlag & STORM) != STORM) &&
            ((Player->SlipFlag & THUNDER_SPIN) != THUNDER_SPIN) &&
            ((Player->HandlingFlag & TROPHYJUMP_END) == 0)) {

            if (frameId <= 20) {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels0[Player->Kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            } else {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels1[Player->Kart][groupId] +
                                                     (frameId - 21) * (temp_num * 4) + ((temp_t2 >> 8) * 0x40) + 0x600),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            }
        } else {
            if (frameId == 0) {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels0[Player->Kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            } else {
                TirecolDMAPa(Player,
                                              (s32) (gKartWheels1[Player->Kart][groupId] +
                                                     (frameId * temp_num * 4) + ((temp_t2 >> 8) * 0x40)),
                                              D_802F1F80_WHEEL(Arg3, screenId, PlayerId), 0x80);
            }
        }
    }
}

#undef D_802F1F80_WHEEL

UNUSED void TirePosCalc(void) {
}

UNUSED void DebugTireCube(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 Arg2) {
}
