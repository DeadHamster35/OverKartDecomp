#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <mk64.h>
#include <course.h>

#include "player_controller.h"
#include "code_800029B0.h"
#include "kart_attributes.h"
#include "racing/memory.h"
#include "math_util.h"
#include "render_player.h"
#include "effects.h"
#include "collision.h"
#include "path.h"
#include "audio/external.h"
#include "code_8003DC40.h"
#include "main.h"
#include "camera.h"
#include "spawn_players.h"
#include "code_80057C60.h"
#include "cpu_vehicles_camera_path.h"
#include "sounds.h"
#include "OverKartHooks.h"

extern s32 D_8018D168;

s16 cpu_forMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER, 0 };

s16 cpu_forWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER, 0 };

s16 cpu_forPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER, 0 };

s16 cpu_forBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO, PEACH, 0 };

s16* cpu_forPlayer[] = { cpu_forMario, cpu_forLuigi, cpu_forYoshi, cpu_forToad,
                         cpu_forDk,    cpu_forWario, cpu_forPeach, cpu_forBowser };

s16 cpu_forMarioAndLuigi[] = { YOSHI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndYoshi[] = { LUIGI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndToad[] = { LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndDk[] = { LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forMarioAndWario[] = { LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forMarioAndPeach[] = { LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forMarioAndBowser[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forLuigiAndMario[] = { YOSHI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndYoshi[] = { MARIO, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndToad[] = { MARIO, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndDk[] = { MARIO, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forLuigiAndWario[] = { MARIO, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forLuigiAndPeach[] = { MARIO, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forLuigiAndBowser[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forYoshiAndMario[] = { LUIGI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndLuigi[] = { MARIO, TOAD, DK, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndToad[] = { MARIO, LUIGI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndDk[] = { MARIO, LUIGI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forYoshiAndWario[] = { MARIO, LUIGI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forYoshiAndPeach[] = { MARIO, LUIGI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forYoshiAndBowser[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH };

s16 cpu_forToadAndMario[] = { LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndLuigi[] = { MARIO, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndYoshi[] = { MARIO, LUIGI, DK, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndDk[] = { MARIO, LUIGI, YOSHI, WARIO, PEACH, BOWSER };

s16 cpu_forToadAndWario[] = { MARIO, LUIGI, YOSHI, DK, PEACH, BOWSER };

s16 cpu_forToadAndPeach[] = { MARIO, LUIGI, YOSHI, DK, WARIO, BOWSER };

s16 cpu_forToadAndBowser[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH };

s16 cpu_forDkAndMario[] = { LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndLuigi[] = { MARIO, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndToad[] = { MARIO, LUIGI, TOAD, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndYoshi[] = { MARIO, LUIGI, YOSHI, WARIO, PEACH, BOWSER };

s16 cpu_forDkAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, PEACH, BOWSER };

s16 cpu_forDkAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, BOWSER };

s16 cpu_forDkAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH };

s16 cpu_forWarioAndMario[] = { LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forWarioAndLuigi[] = { MARIO, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forWarioAndYoshi[] = { MARIO, LUIGI, TOAD, DK, PEACH, BOWSER };

s16 cpu_forWarioAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, PEACH, BOWSER };

s16 cpu_forWarioAndToad[] = { MARIO, LUIGI, YOSHI, DK, PEACH, BOWSER };

s16 cpu_forWarioAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, BOWSER };

s16 cpu_forWarioAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH };

s16 cpu_forPeachAndMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forPeachAndLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forPeachAndYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, BOWSER };

s16 cpu_forPeachAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, BOWSER };

s16 cpu_forPeachAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, BOWSER };

s16 cpu_forPeachAndToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, BOWSER };

s16 cpu_forPeachAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO };

s16 cpu_forBowserAndMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forBowserAndLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH };

s16 cpu_forBowserAndYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH };

s16 cpu_forBowserAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH };

s16 cpu_forBowserAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH };

s16 cpu_forBowserAndToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH };

s16 cpu_forBowserAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO };

s16* gListCPUforMario[] = { cpu_forMarioAndLuigi, cpu_forMarioAndLuigi, cpu_forMarioAndYoshi, cpu_forMarioAndToad,
                            cpu_forMarioAndDk,    cpu_forMarioAndWario, cpu_forMarioAndPeach, cpu_forMarioAndBowser };

s16* gListCPUforLuigi[] = { cpu_forLuigiAndMario, cpu_forLuigiAndMario, cpu_forLuigiAndYoshi, cpu_forLuigiAndToad,
                            cpu_forLuigiAndDk,    cpu_forLuigiAndWario, cpu_forLuigiAndPeach, cpu_forLuigiAndBowser };

s16* gListCPUforYoshi[] = { cpu_forYoshiAndMario, cpu_forYoshiAndLuigi, cpu_forYoshiAndLuigi, cpu_forYoshiAndToad,
                            cpu_forYoshiAndDk,    cpu_forYoshiAndWario, cpu_forYoshiAndPeach, cpu_forYoshiAndBowser };

s16* gListCPUforToad[] = { cpu_forToadAndMario, cpu_forToadAndLuigi, cpu_forToadAndYoshi, cpu_forToadAndYoshi,
                           cpu_forToadAndDk,    cpu_forToadAndWario, cpu_forToadAndPeach, cpu_forToadAndBowser };

s16* gListCPUforDk[] = { cpu_forDkAndMario, cpu_forDkAndLuigi, cpu_forDkAndToad,  cpu_forDkAndYoshi,
                         cpu_forDkAndYoshi, cpu_forDkAndWario, cpu_forDkAndPeach, cpu_forDkAndBowser };

s16* gListCPUforWario[] = { cpu_forWarioAndMario, cpu_forWarioAndLuigi, cpu_forWarioAndYoshi, cpu_forWarioAndToad,
                            cpu_forWarioAndDk,    cpu_forWarioAndDk,    cpu_forWarioAndPeach, cpu_forWarioAndBowser };

s16* gListCPUforPeach[] = { cpu_forPeachAndMario, cpu_forPeachAndLuigi, cpu_forPeachAndYoshi, cpu_forPeachAndToad,
                            cpu_forPeachAndDk,    cpu_forPeachAndWario, cpu_forPeachAndDk,    cpu_forPeachAndBowser };

s16* gListCPUforBowser[] = {
    cpu_forBowserAndMario, cpu_forBowserAndLuigi, cpu_forBowserAndYoshi, cpu_forBowserAndToad,
    cpu_forBowserAndDk,    cpu_forBowserAndWario, cpu_forBowserAndPeach, cpu_forBowserAndPeach
};

s16** cpu_forTwoPlayer[] = { gListCPUforMario, gListCPUforLuigi, gListCPUforYoshi, gListCPUforToad,
                             gListCPUforDk,    gListCPUforWario, gListCPUforPeach, gListCPUforBowser };

// func_80027D00
s32 check_kart_number(Player* player) {
    s32 index;

    if (player == gPlayerOne) {
        index = 0;
    }
    if (player == gPlayerTwo) {
        index = 1;
    }
    if (player == gPlayerThree) {
        index = 2;
    }
    if (player == gPlayerFour) {
        index = 3;
    }
    if (player == gPlayerFive) {
        index = 4;
    }
    if (player == gPlayerSix) {
        index = 5;
    }
    if (player == gPlayerSeven) {
        index = 6;
    }
    if (player == gPlayerEight) {
        index = 7;
    }
    return index;
}

void start_kart_engine(Player* player, s8 playerId) {
    if (replay_flag != 1) {
        if ((player->flag & MOTOR_ON) != MOTOR_ON) {
            if (((D_8018D168 == 1) && ((player->flag & IS_PLAYER) == IS_PLAYER)) &&
                ((player->flag & IS_GHOST) != IS_GHOST)) {
                Na_PlyEng_Start(playerId);
                player->flag |= MOTOR_ON;
            } else if ((player->flag & NO_CONTROLS) == 0) {
                Na_EnmLevel_Start(playerId);
                player->flag |= MOTOR_ON;
            }
        }
    } else if ((player->flag & MOTOR_ON) != MOTOR_ON) {
        if ((D_8018D168 == 1) && (player == gPlayerOne)) {
            Na_PlyEng_Start(playerId);
            player->flag |= MOTOR_ON;
        } else if ((player->flag & NO_CONTROLS) == 0) {
            Na_EnmLevel_Start(playerId);
            player->flag |= MOTOR_ON;
        }
    }
}

void kart_engine_echo(Player* player, s8 playerId) {
    UNUSED s32 pad;
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {

#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (g_courseID) {
            case COURSE_MARIO_RACEWAY:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= g_EchoStart) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < g_EchoStop)) {
                    if (echo_flag[playerId] != 1) {
                        NA_PlyEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        NA_PlyEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_CHOCO_MOUNTAIN:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0xA0) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0xB4)) {
                    if (echo_flag[playerId] != 1) {
                        NA_PlyEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        NA_PlyEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_BOWSER_CASTLE:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x29) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x1D2)) {
                    if (echo_flag[playerId] != 1) {
                        NA_PlyEcho_On(playerId, 0x41);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        NA_PlyEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_BANSHEE_BOARDWALK:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x180) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x1E1)) {
                    if (echo_flag[playerId] != 1) {
                        NA_PlyEcho_On(playerId, 0x41);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        NA_PlyEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_LUIGI_RACEWAY:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
                    if (echo_flag[playerId] != 1) {
                        NA_PlyEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        NA_PlyEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_TOADS_TURNPIKE:
                if ((player->flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
                    if (echo_flag[playerId] != 1) {
                        NA_PlyEcho_On(playerId, 0x1e);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        NA_PlyEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_SHERBET_LAND:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x11C) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x209)) {
                    if (echo_flag[playerId] != 1) {
                        NA_PlyEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        NA_PlyEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_DK_JUNGLE:
                if ((((s16) gNearestPathPointByPlayerId[playerId] >= 0) &&
                     ((s16) gNearestPathPointByPlayerId[playerId] < 0x65)) ||
                    (((s16) gNearestPathPointByPlayerId[playerId] >= 0x14A) &&
                     ((s16) gNearestPathPointByPlayerId[playerId] < 0x21F))) {
                    if (echo_flag[playerId] != 2) {
                        NaPlyLevelStart(playerId, 0x0170802D);
                    }
                    echo_flag[playerId] = 2;
                } else {
                    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x288) &&
                        ((s16) gNearestPathPointByPlayerId[playerId] < 0x305)) {
                        if (echo_flag[playerId] != 1) {
                            NA_PlyEcho_On(playerId, 0x55);
                        }
                        echo_flag[playerId] = 1;
                    } else {
                        if (echo_flag[playerId] != 0) {
                            if (echo_flag[playerId] == 1) {
                                NA_PlyEcho_Off(playerId);
                            }
                            if (echo_flag[playerId] == 2) {
                                NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x70, 0x80, 0x2D));
                            }
                            echo_flag[playerId] = 0;
                        }
                    }
                }
                break;
            default:
                break;
        }
#else

#endif
    } else {
#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (g_courseID) {
            case COURSE_MARIO_RACEWAY:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= g_EchoStart) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < g_EchoStop)) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_CHOCO_MOUNTAIN:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0xA0) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0xB4)) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_BOWSER_CASTLE:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x29) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x1D2)) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x41);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_BANSHEE_BOARDWALK:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x180) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x1E1)) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x41);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_LUIGI_RACEWAY:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x145) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x18B)) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_TOADS_TURNPIKE:
                if ((player->flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x1E);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_SHERBET_LAND:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x11C) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x209)) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            case COURSE_DK_JUNGLE:
                if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x288) &&
                    ((s16) gNearestPathPointByPlayerId[playerId] < 0x305)) {
                    if (echo_flag[playerId] != 1) {
                        Na_EnmEcho_On(playerId, 0x55);
                    }
                    echo_flag[playerId] = 1;
                } else {
                    if (echo_flag[playerId] != 0) {
                        Na_EnmEcho_Off(playerId);
                        echo_flag[playerId] = 0;
                    }
                }
                break;
            default:
                break;
        }
#else

#endif
    }
}

void mariogp_movekart(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    u16 isVisible;

    if (!(player->flag & NO_CONTROLS)) {
        switch (g_ScreenSplitA) {
            case SCREEN_MODE_1P:
                isVisible = viewport(player, camera1, (f32) select_bumpKart, 0.0f);
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                isVisible = viewport(player, camera1, (f32) select_bumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = viewport(player, camera2, (f32) select_bumpKart, 0.0f);
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                isVisible = viewport(player, camera1, (f32) select_bumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = viewport(player, camera2, (f32) select_bumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = viewport(player, camera3, (f32) select_bumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = viewport(player, camera4, (f32) select_bumpKart, 0.0f);
                break;
        }
        if ((isVisible == 1) || ((player->flag & IS_GHOST) == IS_GHOST) ||
            (g_gameMode == BATTLE) || ((player->jugemu_flag & ON_LAKITU_ROD) != 0) ||
            (player->jugemu_flag & LAKITU_SCENE) ||
            //! @todo make a proper match
            ((*(g_noSimpleKartFlag + (playerId))) == ((s16) 1U))) {
            player->slip_flag &= ~CENTER_LINE;
            if (((player->slip_flag & SPIN_L) == SPIN_L) ||
                ((player->slip_flag & SPIN_R) == SPIN_R) ||
                ((player->slip_flag & ROLLOVER) == ROLLOVER) ||
                ((player->slip_flag & RAPID_ACC) == RAPID_ACC) ||
                ((player->slip_flag & WING) == WING) ||
                ((player->slip_flag & STORM) == STORM) ||
                ((player->slip_flag & EXPLODE) == EXPLODE) ||
                ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) ||
                ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) ||
                (player->handling_flag & TROPHYJUMP_END)) {
                custom_SpinKart(player, camera, screenId, playerId);
            } else {
                RunKart(player, camera, screenId, playerId);
            }
        } else {
            RunKartSimple(player, camera, screenId, playerId);
        }
    } else if ((player->flag & IS_GP_OPENING) == IS_GP_OPENING) {
        opening_kart(player, playerId);
        openkart(player, camera, screenId, playerId);
    } else if (player->flag & PLAYER_UNKNOWN_0x80) {
        RunKart(player, camera, screenId, playerId);
    } else {
        if ((player->flag & IS_PLAYER) != IS_PLAYER) {
            player->accelcount = 50.0f;
        }
        player->slip_flag &= ~N_JUMP;
    }
}

void vs_time_movekart(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if ((player->flag & NO_CONTROLS) == 0) {
        player->slip_flag &= ~CENTER_LINE;
        if (((player->slip_flag & SPIN_L) == SPIN_L) ||
            ((player->slip_flag & SPIN_R) == SPIN_R) ||
            ((player->slip_flag & ROLLOVER) == ROLLOVER) ||
            ((player->slip_flag & RAPID_ACC) == RAPID_ACC) ||
            ((player->slip_flag & WING) == WING) ||
            ((player->slip_flag & STORM) == STORM) ||
            ((player->slip_flag & EXPLODE) == EXPLODE) ||
            ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) ||
            ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) ||
            ((player->handling_flag & TROPHYJUMP_END) != 0)) {
            custom_SpinKart(player, camera, screenId, playerId);
        } else {
            RunKart(player, camera, screenId, playerId);
        }
    } else {
        player->slip_flag &= ~N_JUMP;
    }
}

void battle_movekart(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if ((((player->flag & NO_CONTROLS) == 0) && (g_startingIndicator != RACE_DONE)) || (player->jugemu_flag & 2) != 0 ||
        (player->jugemu_flag & LAKITU_SCENE) != 0 ||
        (player->slip_flag & (THUNDER | EXPLODE | THROW_EXPLODE | BROKEN |
                            RESTORE | ROLLOVER_FALL | 0xC00 | 0xC0)) != 0) {
        player->slip_flag &= ~CENTER_LINE;
        if (((player->slip_flag & SPIN_L) == SPIN_L) ||
            ((player->slip_flag & SPIN_R) == SPIN_R) ||
            ((player->slip_flag & ROLLOVER) == ROLLOVER) ||
            ((player->slip_flag & RAPID_ACC) == RAPID_ACC) ||
            ((player->slip_flag & WING) == WING) ||
            ((player->slip_flag & STORM) == STORM) ||
            ((player->slip_flag & EXPLODE) == EXPLODE) ||
            ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) ||
            ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) ||
            ((player->handling_flag & TROPHYJUMP_END) != 0)) {
            custom_SpinKart(player, camera, screenId, playerId);
        } else {
            RunKart(player, camera, screenId, playerId);
        }
    } else {
        player->slip_flag = player->slip_flag & ~N_JUMP;
    }
}

void movekart(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if ((player->flag & EXISTS) == EXISTS) {
        switch (gGamestate) {
            case ENDING:
                if (!(player->flag & NO_CONTROLS)) {
                    result_runkart(player, camera, screenId, playerId);
                } else {
                    player->slip_flag &= ~N_JUMP;
                }
                break;
            default:
                start_kart_engine(player, playerId);
                switch (g_gameMode) {
                    case TIME_TRIALS:
                    case VERSUS:
                        vs_time_movekart(player, camera, playerId, screenId);
                        break;
                    case BATTLE:
                        battle_movekart(player, camera, playerId, screenId);
                        break;
                    default:
                        mariogp_movekart(player, camera, playerId, screenId);
                        break;
                }
                break;
        }
    }
}

UNUSED void func_80028F5C(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s32 arg3) {
}

void kartpos_control_full(void) {
    effectcount = 0;
    movekart(gPlayerOneCopy, camera1, 0, 0);
    movekart(gPlayerTwo, camera1, 1, 0);
    movekart(gPlayerThree, camera1, 2, 0);
    movekart(gPlayerFour, camera1, 3, 0);
    movekart(gPlayerFive, camera1, 4, 0);
    movekart(gPlayerSix, camera1, 5, 0);
    movekart(gPlayerSeven, camera1, 6, 0);
    movekart(gPlayerEight, camera1, 7, 0);
}

void kartpos_control_ru(void) {
    effectcount = 0;
    movekart(gPlayerOneCopy, camera1, 0, 0);
    movekart(gPlayerTwo, camera1, 1, 0);
    movekart(gPlayerThree, camera1, 2, 0);
    movekart(gPlayerFour, camera1, 3, 0);
    movekart(gPlayerFive, camera1, 4, 0);
    movekart(gPlayerSix, camera1, 5, 0);
    movekart(gPlayerSeven, camera1, 6, 0);
    movekart(gPlayerEight, camera1, 7, 0);
}

void kartpos_control_ld(void) {
}

void kartpost_control_1P(void) {
    effectcount = 0;
    movekart(gPlayerOneCopy, camera1, 0, 0);
    movekart(gPlayerTwo, camera1, 1, 0);
    movekart(gPlayerThree, camera1, 2, 0);
    movekart(gPlayerFour, camera1, 3, 0);
}

void kartpost_control_2P(void) {
}

void kartpos_control_3P(void) {
}

void kartpos_control_4P(void) {
}

void make_degpict(Player* player, s8 screenId) {
    if ((s32) player->kartpitch < -0x71B) {
        player->deg[screenId] = 0;
    }
    if (((s32) player->kartpitch < -0x4F9) && ((s32) player->kartpitch > -DEGREES(10))) {
        player->deg[screenId] = 1;
    }
    if ((player->kartpitch <= -DEGREES(3)) && (player->kartpitch > -DEGREES(7))) {
        player->deg[screenId] = 2;
    }
    if ((player->kartpitch <= -DEGREES(2)) && (player->kartpitch > -DEGREES(3))) {
        player->deg[screenId] = 3;
    }
    if ((player->kartpitch < DEGREES(2)) && (player->kartpitch > -DEGREES(2))) {
        player->deg[screenId] = 4;
    }
    if ((player->kartpitch >= DEGREES(2)) && (player->kartpitch < DEGREES(3))) {
        player->deg[screenId] = 5;
    }
    if ((player->kartpitch >= DEGREES(3)) && (player->kartpitch < DEGREES(7))) {
        player->deg[screenId] = 6;
    }
    if ((player->kartpitch >= DEGREES(7)) && (player->kartpitch < DEGREES(10))) {
        player->deg[screenId] = 7;
    }
    if (player->kartpitch >= DEGREES(10)) {
        player->deg[screenId] = 8;
    }
}

void kart_posset(Player* player, Camera* camera, s8 screenId, s8 playerId) {
    UNUSED s32 pad[2];
    f32 temp_f0;
    f32 temp_f2;
    UNUSED s32 pad2[3];
    f32 var_f0;
    s16 temp_a0;
    s32 temp_a0_2;
    s32 var_a1;
    s32 var_t0;
    u16 var_a0;

    player->angle_y[screenId] = Atan2t(player->position[0] - camera->camera_pos[0], player->position[2] - camera->camera_pos[2]);
    player->dir[screenId] =
        (u16) ((((player->angle_y[screenId]) + player->direction[1] + player->slipang))) / 128;

    temp_f2 = (gCharacterSize[player->kart] * 18.0f) * player->offsetsize;
    temp_f0 = player->mid_left - player->mid_right;
    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        if ((player->slip_flag & THUNDER) == THUNDER) {
            player->roll[screenId] = (s16) ((s32) (((f64) Atant(temp_f0 / temp_f2)) * 1.6));
        } else {
            player->roll[screenId] = Atant(temp_f0 / temp_f2) * 2;
        }
    }
    if ((player->slip_flag & BROKEN) == BROKEN) {
        player->roll[screenId] = (s16) ((s32) player->sus.position);
    }
    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        temp_f0 = player->mid_front - player->mid_rear;
        player->pitch[screenId] = (((Atant(temp_f0 / temp_f2)) * 0.9));
    } else {
        if (((player->dir[screenId]) >= 0) && ((player->dir[screenId]) < 0x101)) {
            var_f0 = player->old_position[1] - player->position[1];
        } else {
            var_f0 = player->position[1] - player->old_position[1];
        }
        player->pitch[screenId] = (s16) ((s32) (((f64) Atant(var_f0 / temp_f2)) * 0.5));
    }
    if ((player->slip_flag & BROKEN) == BROKEN) {
        player->pitch[screenId] = (s16) ((s32) player->sus.position);
    }
    make_degpict(player, screenId);
    temp_a0 = ((player->angle_y[screenId] + player->direction[1]) + player->slipang);
    temp_a0 = (s16) player->pitch[screenId] * sinT((u16) temp_a0) + player->roll[screenId] * cosT((u16) temp_a0);
    chase_Snumber(&player->angle_z[screenId], temp_a0, 0.5f);
    var_a0 = player->dir[screenId];
    player->status = player->status & (~(P1_FLIP_SPRITE << (screenId * 4)));
    if (var_a0 >= 0x101) {
        var_a0 = 0x201 - var_a0;
        player->status |= (P1_FLIP_SPRITE << (screenId * 4));
    }
    if (((player->slip_flag & SPIN_L) != SPIN_L) &&
        ((player->slip_flag & SPIN_R) != SPIN_R) &&
        ((player->slip_flag & WING) != WING) &&
        ((player->slip_flag & STORM) != STORM) &&
        ((player->slip_flag & THUNDER_SPIN) != THUNDER_SPIN) &&
        (!(player->handling_flag & TROPHYJUMP_END))) {
        if (var_a0 < 0x51) {
            var_a1 = 0x208;
            var_t0 = 0;
        } else {
            var_a1 = DEGREES(9);
            var_t0 = 0xF;
        }
    } else {
        var_a1 = DEGREES(9);
        var_t0 = 0;
    }
    if (((player->slip_flag & WING) == WING) ||
        ((player->slip_flag & STORM) == STORM) ||
        (player->handling_flag & TROPHYJUMP_END)) {
        player->angle_z[screenId] = 0;
    }
    if (((player->slip_flag & N_JUMP) == N_JUMP) &&
        ((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
        player->angle_z[screenId] = 0;
    }
    var_a0 = (player->angle_y[screenId] + player->direction[1] + player->slipang);
    if (((player->slip_flag & SPIN_L) == SPIN_L) ||
        ((player->slip_flag & SPIN_R) == SPIN_R) ||
        ((player->slip_flag & WING) == WING) ||
        ((player->slip_flag & STORM) == STORM) ||
        ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) ||
        (player->handling_flag & TROPHYJUMP_END)) {
        if (var_a0 >= 0x7FF9) {
            var_a0 = -var_a0;
            var_a0 /= var_a1;
            if (var_a0 == 0) {
                var_a0 = 1;
            }
        } else {
            var_a0 /= var_a1;
        }
    } else {
        if (var_a0 >= 0x7FF9) {
            var_a0 = (-var_a0);
        }
        var_a0 /= var_a1;
    }
    player->dir[screenId] = var_a0 + var_t0;
    if ((player->dir[screenId]) >= 0x23) {
        player->dir[screenId] = 0x22;
    }
    if ((player->slip_flag & SPIN_L) || (player->slip_flag & SPIN_R) ||
        (player->slip_flag & WING) || (player->slip_flag & STORM) ||
        (player->slip_flag & THUNDER_SPIN) || (player->handling_flag & TROPHYJUMP_END)) {

        if ((player->dir[screenId]) >= 0x14) {
            player->dir[screenId] = 0;
        }
    }
    if ((player->deg[screenId]) >= 9) {
        player->deg[screenId] = 4;
    }
    if (((player->slip_flag & WING) == WING) ||
        ((player->slip_flag & STORM) == STORM) ||
        (player->handling_flag & TROPHYJUMP_END)) {

        player->deg[screenId] = 4;
    }
    if (((player->slip_flag & ROLLOVER) == ROLLOVER) ||
        ((player->slip_flag & EXPLODE) == EXPLODE) ||
        ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) || (player->slip_flag & ROLLOVER_FALL) ||
        (player->slip_flag & SPIN_L) || (player->slip_flag & SPIN_R)) {

        player->status |= P1_DMA_SPRITE << (screenId * 4);
        compel_flag[screenId][playerId] = 1;

        if ((player->slip_flag & SPIN_L) || (player->slip_flag & SPIN_R)) {
            if ((player->dir[screenId] == old_dir[screenId][playerId]) &&
                (player->deg[screenId] == old_deg[screenId][playerId])) {
                player->status &= ~(P1_DMA_SPRITE << (screenId * 4));
                compel_flag[screenId][playerId] = 1;
            }
        } else if (((player->slipcount) >> 8) == old_slipcount[screenId][playerId] >> 8) {
            player->status &= ~(P1_DMA_SPRITE << (screenId * 4));
        }
    } else {
        player->status |= P1_DMA_SPRITE << (screenId * 4);
        if (((player->dir[screenId] == old_dir[screenId][playerId]) &&
             (player->deg[screenId] == old_deg[screenId][playerId])) &&
            ((compel_flag[screenId][playerId]) == 0)) {
            player->status &= ~(P1_DMA_SPRITE << (screenId * 4));
        }
    }
    temp_a0_2 = old_dir[screenId][playerId] - player->dir[screenId];
    if ((temp_a0_2 >= 0x14) || (temp_a0_2 < (-0x13))) {
        player->status |= P1_DMA_SPRITE << (screenId * 4);
    }
}

void TirePosition(Player* player, UNUSED f32 arg1, f32 arg2, UNUSED f32 arg3) {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    Vec3f sp8C;
    Vec3f sp80;
    Mat3 sp5C;
    UNUSED s32 pad;
    f32 temp_f0_2;
    f32 temp_f2_3;
    s16 temp_v0;
    f32 var_f12;

    if ((player->slip_flag & THUNDER) == THUNDER) {
        var_f12 = 18.0f * ((((gCharacterSize[player->kart] / 2)) * ((player->offsetsize) * 1.5)));
    } else {
        var_f12 = 18.0f * (gCharacterSize[player->kart] / 2);
    }

    MakeAlignMatrix(sp5C, 0.0f, 1.0f, 0.0f, (player->direction[1] + player->slipang));
    sp8C[0] = var_f12 - 3.6;
    sp8C[1] = -player->radius;
    sp8C[2] = var_f12 - 2.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = player->tire_FL.Position[0];
    sp80[1] = player->tire_FL.Position[1];
    sp80[2] = player->tire_FL.Position[2];
    player->tire_FL.Position[0] = player->position[0] + sp8C[0];
    player->tire_FL.Position[1] = player->position[1] + sp8C[1];
    player->tire_FL.Position[2] = player->position[2] + sp8C[2];
    check_tire_2(player, &player->tire_FL, sp80[0], sp80[1], sp80[2]);
    sp8C[0] = (-var_f12) + 3.6;
    sp8C[1] = -player->radius;
    sp8C[2] = var_f12 - 2.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = player->tire_FR.Position[0];
    sp80[1] = player->tire_FR.Position[1];
    sp80[2] = player->tire_FR.Position[2];
    player->tire_FR.Position[0] = player->position[0] + sp8C[0];
    player->tire_FR.Position[1] = player->position[1] + sp8C[1];
    player->tire_FR.Position[2] = player->position[2] + sp8C[2];
    check_tire_2(player, &player->tire_FR, sp80[0], sp80[1], sp80[2]);
    sp8C[0] = var_f12 - 2.6;
    sp8C[1] = -player->radius;
    sp8C[2] = (-var_f12) + 4.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = player->tire_RL.Position[0];
    sp80[1] = player->tire_RL.Position[1];
    sp80[2] = player->tire_RL.Position[2];
    player->tire_RL.Position[0] = player->position[0] + sp8C[0];
    player->tire_RL.Position[1] = player->position[1] + sp8C[1];
    player->tire_RL.Position[2] = player->position[2] + sp8C[2];
    check_tire_2(player, &player->tire_RL, sp80[0], sp80[1], sp80[2]);
    sp8C[0] = (-var_f12) + 2.6;
    sp8C[1] = -player->radius;
    sp8C[2] = (-var_f12) + 4.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = player->tire_RR.Position[0];
    sp80[1] = player->tire_RR.Position[1];
    sp80[2] = player->tire_RR.Position[2];
    player->tire_RR.Position[0] = player->position[0] + sp8C[0];
    player->tire_RR.Position[1] = player->position[1] + sp8C[1];
    player->tire_RR.Position[2] = player->position[2] + sp8C[2];
    check_tire_2(player, &player->tire_RR, sp80[0], sp80[1], sp80[2]);
    if (!(player->slip_flag & N_JUMP)) {
        a = (player->tire_RL.Height + player->tire_FL.Height) / 2;
        chase_Fnumber(&player->mid_left, a, 0.5f);

        b = (player->tire_RR.Height + player->tire_FR.Height) / 2;
        chase_Fnumber(&player->mid_right, b, 0.5f);

        c = (player->tire_FR.Height + player->tire_FL.Height) / 2;
        chase_Fnumber(&player->mid_rear, c, 0.5f);

        d = (player->tire_RR.Height + player->tire_RL.Height) / 2;
        chase_Fnumber(&player->mid_front, d, 0.5f);
    }
    temp_f2_3 = ((gCharacterSize[player->kart] * 18.0f) + 1.0f) * player->offsetsize;
    temp_f0_2 = player->mid_right - player->mid_left;
    player->kartroll = -Atant(temp_f0_2 / temp_f2_3);
    if (((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) || (player->slip_flag & N_JUMP)) {
        player->kartroll = 0;
    }
    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        temp_f0_2 = player->mid_front - player->mid_rear;
        chase_Snumber(&player->kartpitch, Atant(temp_f0_2 / temp_f2_3), 0.5f);
    } else {
        temp_f0_2 = player->old_position[1] - arg2;
        temp_v0 = Atant(temp_f0_2 / temp_f2_3);
        if (temp_f0_2 >= 0.0f) {
            temp_v0 /= 4;
        } else {
            temp_v0 *= 10;
        }
        chase_Snumber(&player->kartpitch, temp_v0, 0.5f);
    }
    if (((player->slip_flag & N_JUMP) == N_JUMP) &&
        ((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
        player->kartpitch = (s16) ((s32) player->sus.position);
    }
    player->bump_status = check_status(player->bump.last_zx) & 0xFF;
    if (player->bump_status == BOOST_RAMP_ASPHALT) {
        if (((player->slip_flag & DASH_JUMP) != DASH_JUMP) &&
            ((player->slip_flag & N_JUMP) != N_JUMP)) {
            player->weapon |= PRESS_BOOSTPAD;
        }
    }
    if (player->bump_status == BOOST_RAMP_WOOD) {
        if (((player->slip_flag & DASH_MIDJUMP) != DASH_MIDJUMP) &&
            ((player->slip_flag & N_JUMP) != N_JUMP)) {
            player->weapon |= PRESS_BOOSTJUMP;
        }
    }
}

void EnemyTirePosition(Player* player, f32 x, f32 y, f32 z) {
    UNUSED s32 pad[2];
    f32 temp_f12;
    f32 var_f20;
    s32 temp_v0;
    s16 temp_v1;
    s16 var_a1;
    UNUSED s32 pad2;
    f32 temp_f0;

    temp_v1 = -player->direction[1] - player->slipang;
    if ((player->slip_flag & THUNDER) == THUNDER) {
        var_f20 = (((gCharacterSize[player->kart] * 18) / 2) * (player->offsetsize * 1.5)) - 1;
    } else {
        var_f20 = (((gCharacterSize[player->kart] * 18) / 2) * player->offsetsize) - 1;
    }

    player->tire_FL.Position[2] = (cosT(temp_v1 + 0x2000) * var_f20) + z;
    temp_f12 = (sinT(temp_v1 + 0x2000) * var_f20) + x;
    player->tire_FL.Position[0] = temp_f12;
    player->tire_FL.Height =
        CalcHeight(temp_f12, y, player->tire_FL.Position[2], player->bump.last_zx);

    player->tire_FR.Position[2] = (cosT(temp_v1 - 0x2000) * var_f20) + z;
    temp_f12 = (sinT(temp_v1 - 0x2000) * var_f20) + x;
    player->tire_FR.Position[0] = temp_f12;
    player->tire_FR.Height =
        CalcHeight(temp_f12, y, player->tire_FR.Position[2], player->bump.last_zx);

    player->tire_RL.Position[2] = (cosT(temp_v1 + 0x6000) * var_f20) + z;
    temp_f12 = (sinT(temp_v1 + 0x6000) * var_f20) + x;
    player->tire_RL.Position[0] = temp_f12;
    player->tire_RL.Height =
        CalcHeight(temp_f12, y, player->tire_RL.Position[2], player->bump.last_zx);

    player->tire_RR.Position[2] = (cosT(temp_v1 - 0x6000) * var_f20) + z;
    player->tire_RR.Position[0] = (sinT(temp_v1 - 0x6000) * var_f20) + x;
    player->tire_RR.Height = CalcHeight(
        player->tire_RL.Position[0], y, player->tire_RL.Position[2], player->bump.last_zx);

    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        player->mid_left = (player->tire_RL.Height + player->tire_FL.Height) / 2;
        player->mid_right = (player->tire_RR.Height + player->tire_FR.Height) / 2;
        player->mid_rear = (player->tire_FR.Height + player->tire_FL.Height) / 2;
        player->mid_front = (player->tire_RR.Height + player->tire_RL.Height) / 2;
    }
    player->bump_status = (u8) check_status(player->bump.last_zx);
    player->tire_RR.Status = player->bump_status;
    player->tire_RL.Status = player->bump_status;
    player->tire_FR.Status = player->bump_status;
    player->tire_FL.Status = player->bump_status;
    var_f20 = (gCharacterSize[player->kart] * 18) + 1;
    temp_f0 = (player->mid_right - player->mid_left);
    player->kartroll = -Atant(temp_f0 / var_f20);
    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        temp_f0 = (player->mid_front - player->mid_rear);
        chase_Snumber(&player->kartpitch, Atant(temp_f0 / var_f20), 0.5f);
    } else {
        temp_f0 = player->old_position[1] - y;
        temp_v0 = Atant(temp_f0 / var_f20);
        if (temp_f0 >= 0.0f) {
            var_a1 = temp_v0 * 2;
        } else {
            var_a1 = temp_v0 * 0xA;
        }
        chase_Snumber(&player->kartpitch, var_a1, 0.5f);
    }
    if (check_shadow(player->bump.last_zx) != 0) {
        player->tire_RR.Dummy |= 1;
    } else {
        player->tire_RR.Dummy &= ~1;
    }
    if (player->bump_status == BOOST_RAMP_ASPHALT) {
        if (((player->slip_flag & DASH_JUMP) != DASH_JUMP) &&
            ((player->slip_flag & N_JUMP) != N_JUMP)) {
            player->weapon |= PRESS_BOOSTPAD;
        }
    }
    if (player->bump_status == BOOST_RAMP_WOOD) {
        if (((player->slip_flag & DASH_MIDJUMP) != DASH_MIDJUMP) &&
            ((player->slip_flag & N_JUMP) != N_JUMP)) {
            player->weapon |= PRESS_BOOSTJUMP;
        }
    }
}

// Near identical to BumpVelocity in memory.c
void hit_velocity(Vec3f arg0, f32 arg1, Vec3f arg2, f32 arg3, f32 arg4) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f20;
    f32 tmp1;
    f32 tmp2;
    f32 tmp3;

    temp_f0 = arg2[0];
    temp_f2 = arg2[1];
    temp_f12 = arg2[2];
    temp_f14 = -arg0[0];
    temp_f16 = -arg0[1];
    temp_f18 = -arg0[2];
    temp_f20 = (temp_f14 * temp_f0) + (temp_f16 * temp_f2) + (temp_f18 * temp_f12);
    tmp1 = temp_f0 - (temp_f20 * temp_f14);
    tmp2 = temp_f2 - (temp_f20 * temp_f16);
    tmp3 = temp_f12 - (temp_f20 * temp_f18);
    if (arg1 < -arg4) {
        arg2[0] = tmp1 - (temp_f20 * temp_f14 * arg3);
        arg2[1] = tmp2 - (temp_f20 * temp_f16 * arg3);
        arg2[2] = tmp3 - (temp_f20 * temp_f18 * arg3);
    } else {
        arg2[0] = tmp1;
        arg2[1] = tmp2;
        arg2[2] = tmp3;
    }
}

void set_start_dush(Player* player, s8 playerIndex) {
    player->slip_flag |= TURBO;
    player->weapon &= ~USE_STARTDASH;
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(0U, (player->kart * 0x10) + 0x29008001);
        NAPlyTrgStart(playerIndex, 0x1900A40BU);
    }
    player->turbo_timer = 0x0050;
}

void drift_turbo(Player* player, s8 playerIndex) {
    if (((player->slip_flag & DRIFT_TURBO) != DRIFT_TURBO) &&
        ((player->slip_flag & DRIFT) != DRIFT) && (player->powerstack_count >= 2)) {
        player->slip_flag |= DRIFT_TURBO;
        player->drift_turbo_timer = 0;
        player->powerstack_count = 0;
        player->stopsetrr_count = 0;
        if (replay_flag != 1) {
            if ((player->flag & IS_PLAYER) && !(player->flag & IS_GHOST)) {
                Na_PlyDash_Start(playerIndex);
            }
        } else if (player == gPlayerOne) {
            Na_PlyDash_Start(playerIndex);
        }
    } else if ((player->slip_flag & DRIFT_TURBO) == DRIFT_TURBO) {
        player->drift_turbo_timer += 1;
        if (player->drift_turbo_timer >= 0x1F) {
            player->drift_turbo_timer = 0;
            player->slip_flag &= ~DRIFT_TURBO;
            player->powerstack_count = 0;
            player->stopsetrr_count = 0;
        }
    }
}

void check_drift_turbo(Player* player, s8 playerIndex) {
    if (((s16) player->slipang / DEGREES(1)) > 0) {
        if (((s32) player->nowsterr >> 16) <= -10) {
            if (player->stopsetrr_count <= 100) {
                player->stopsetrr_count++;
            }
            if ((player->stopsetrr_count == 100) && (player->flag & IS_PLAYER)) {
                NAPlyTrgStart(playerIndex, 0x1900851EU);
            }
        } else {
            if ((player->stopsetrr_count >= 18) && (player->stopsetrr_count < 100)) {
                if (player->powerstack_count < 3) {
                    player->powerstack_count++;
                }
            }
            if ((player->stopsetrr_count >= 10) && (player->stopsetrr_count < 100)) {
                player->stopsetrr_count = 10;
            } else {
                player->stopsetrr_count = 0;
                player->powerstack_count = 0;
            }
        }
    } else if (((s32) player->nowsterr >> 16) >= 10) {
        if (player->stopsetrr_count <= 100) {
            player->stopsetrr_count++;
        }
        if ((player->stopsetrr_count == 100) && (player->flag & IS_PLAYER)) {
            NAPlyTrgStart(playerIndex, 0x1900851EU);
        }
    } else {
        if ((player->stopsetrr_count >= 18) && (player->stopsetrr_count < 100)) {
            if (player->powerstack_count < 3) {
                player->powerstack_count++;
            }
        }
        if ((player->stopsetrr_count >= 10) && (player->stopsetrr_count < 100)) {
            player->stopsetrr_count = 10;
        } else {
            player->stopsetrr_count = 0;
            player->powerstack_count = 0;
        }
    }
}

void jump_set(Player* player) {
    player->jmp_acc = gKartHopJerkTable[player->kart];
    player->jmp_speed = 0.0f;
    player->jump = gKartHopInitialVelocityTable[player->kart];
    player->slip_flag |= D_JUMP;
    player->sus.jump_firstspeed = 3.0f;
    player->gravity = 500.0f;
    drift_set(player);
}

/**
  * Function: DriftJump

  * Parameters:
  *     Player *player - A pointer to a Player

  * First kartHopJerk is subtracted from kartHopAcceleration
  * Then kartHopAcceleration is added to kartHopVelocity.

  * If kartHopVelocity is less than or equal to zero, all three
  * values (kartHop{Jerk,Acceleration,Velocity}) are set to 0.0f,
  * thereby ending the rising portion of the hop.

  * kartHopVelocity is restricted to values in [-INF, 15.0f]

  * kartHopAcceleration is restricted to values in [-9.0f, 9.0f]
**/
void DriftJump(Player* player) {
    player->jmp_speed -= player->jmp_acc;
    if (player->jmp_speed >= 9.0f) {
        player->jmp_speed = 9.0f;
    }

    if (player->jmp_speed <= -9.0f) {
        player->jmp_speed = -9.0f;
    }

    player->jump += player->jmp_speed;
    if (player->jump >= 15.0f) {
        player->jump = 15.0f;
    }

    if (player->jump <= 0.0f) {
        player->jmp_acc = 0.0f;
        player->jmp_speed = 0.0f;
        player->jump = 0.0f;
    }
}

void AddGravity(Player* player) {
    UNUSED s32 pad[2];
    if (((player->slip_flag & N_JUMP) != N_JUMP) && (player->force > 0.0f)) {
        if (((player->kartpitch / DEGREES(1)) < -1) && ((player->kartpitch / DEGREES(1)) >= -0x14) &&
            (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            chase_Fnumber(&player->gravity, 500.0f, 1.0f);
            chase_Fnumber(&player->sus.jump_firstspeed, 3.0f, 0.05f);
        } else {
            chase_Fnumber(&player->gravity, gravity_1[player->kart], 0.1f);
            chase_Fnumber(&player->sus.jump_firstspeed, 1.0f, 0.07f);
        }
    } else {
        if (player->bump.distance_zx >= 50.0f) {
            player->sus.jump_firstspeed = 2.0f;
        }
        chase_Fnumber(&player->gravity, gravity_1[player->kart], 0.02f);
        if ((player->slip_flag & D_JUMP) == D_JUMP) {
            chase_Fnumber(&player->sus.jump_firstspeed, 1.0f, 0.07f);
        } else {
            chase_Fnumber(&player->sus.jump_firstspeed, 1.0f, 0.07f);
        }
    }
    if ((player->slip_flag & DASH_JUMP) == DASH_JUMP) {
        chase_Fnumber(&player->sus.jump_firstspeed, 20.0f, 1.0f);
        player->gravity = 3500.0f;
    }
    if ((player->slip_flag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        chase_Fnumber(&player->sus.jump_firstspeed, 25.0f, 1.0f);
        player->gravity = 1800.0f;
    }
    if ((player->slip_flag & ROLLOVER) == ROLLOVER) {
        player->gravity = 1100.0f;
    }
    if (player->slip_flag & WING) {
        player->gravity = 1500.0f;
    }
    if ((player->handling_flag & TROPHYJUMP_END) != 0) {
        player->gravity = 1900.0f;
    }
    if ((player->slip_flag & STORM) == STORM) {
        player->gravity = 300.0f;
    }
    if ((player->slip_flag & EXPLODE) == EXPLODE) {
        player->gravity = 550.0f;
    }
    if ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) {
        player->gravity = 800.0f;
    }
}

UNUSED void set_nature_vector(void) {
}

UNUSED void check_nature_vector(void) {
}

UNUSED void check_direction(void) {
}

void SetSlipAngle(Player* player, s8 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    UNUSED s32 pad[4];
    s16 temp_v0_3;
    f32 sp28;
    f32 temp_f16;
    s16 temp_a0;
    s32 var_v1;

    sp28 = (sinT(-player->direction[1]) * player->speed) + arg2;
    temp_f16 = (cosT(-player->direction[1]) * player->speed) + arg3;
    if (((player->slip_flag & WHEELSPIN) != WHEELSPIN) &&
        ((player->slip_flag & DRIFT) != DRIFT) && !(player->handling_flag & SPINOUT_SWERVE) &&
        ((((player->speed / 18.0f) * 216.0f) <= 8.0f) ||
         (((player->nowsterr >> 16) < 5) && ((player->nowsterr >> 16) > -5)))) {
        if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
            player->slipang = (f32) (player->slipang - (player->slipang / 10));
        } else {
            temp_v0_3 = player->slipang;
            player->slipang = player->sterrangle * 9;
            temp_a0 = player->slipang - temp_v0_3;
            player->slipang = (f32) (temp_v0_3 + (temp_a0 / 15));
        }
    } else {
        temp_v0_3 = player->slipang;
        if (sterr_counter[arg1] & 8) {
            var_v1 = 2;
        } else {
            var_v1 = 0;
        }
        if ((player->accelcount >= 200.0f) && (var_v1 == 2) &&
            (((player->slipang / DEGREES(1)) >= 0x10) || ((player->slipang / DEGREES(1)) < -0xF))) {
            player->slipang = Atan2t(arg2 - arg4, arg3 - arg5) - Atan2t(arg2 - sp28, arg3 - temp_f16);
        } else {
            player->slipang = (Atan2t(arg2 - arg4, arg3 - arg5) - Atan2t(arg2 - sp28, arg3 - temp_f16)) * 2;
        }
        if (((player->slip_flag & DRIFT) != DRIFT) &&
            ((((player->nowsterr >> 16) > 0) && (player->slipang < 0)) ||
             (((player->nowsterr >> 16) < 0) && (player->slipang > 0)))) {
            if (player->slipang > 0) {
                player->slipang = player->sterrangle * 0x14;
            }
            if (player->slipang < 0) {
                player->slipang = player->sterrangle * 0x14;
            }
            temp_a0 = player->slipang - temp_v0_3;
            player->slipang = (f32) (temp_v0_3 + (temp_a0 / 12));
        } else {
            if (player->slipang >= 0x1C71) {
                player->slipang = 0x1C70;
            }
            if (player->slipang < -0x1C70) {
                player->slipang = -0x1C70;
            }
            temp_a0 = player->slipang - temp_v0_3;
            player->slipang = (f32) (temp_v0_3 + (temp_a0 / 12));
        }
    }
}

void enemy_checkdrift(Player* player) {
    u16 someIndex;
    u16 sp38[10] = { 0x0003, 0x0016, 0x0026, 0x003c, 0x0050, 0x0069, 0x0090, 0x009d, 0x00a9, 0x00cc };
    u16 sp24[10] = { 0x000c, 0x0021, 0x002f, 0x0045, 0x005f, 0x007a, 0x0098, 0x00a5, 0x00b3, 0x00d5 };

    for (someIndex = 0; someIndex < 10; someIndex++) {
        if (player->point == sp38[someIndex]) {
            player->slip_flag |= DRIFT;
            jump_set(player);
            player->drift_count = 0;
            break;
        }

        if (player->point == sp24[someIndex]) {
            player->slip_flag &= ~DRIFT;
            break;
        }
    }
}

void set_weapon(Player* player, s8 playerId, UNUSED s8 screenId) {
    if ((player->weapon & HIT_REDSHELL) == HIT_REDSHELL) {
        BombThrowRolloverWrap(player, playerId);
    }
    if ((player->weapon & HIT_GREENSHELL) == HIT_GREENSHELL) {
        RolloverWrap(player, playerId);
    }
    if ((player->weapon & HIT_BANANA) == HIT_BANANA) {
        WheelSpinWrap(player, playerId);
    }
    if ((player->weapon & USE_MUSHROOM) == USE_MUSHROOM) {
        SetTurbo(player, playerId);
    }
    if ((player->weapon & USE_STARTDASH) == USE_STARTDASH) {
        set_start_dush(player, playerId);
    }
    if ((player->weapon & USE_FEATHER) == USE_FEATHER) {
        SetWing(player, playerId);
    }
    if ((player->weapon & HIT_TORNADO) == HIT_TORNADO) {
        SetStorm(player, playerId);
    }
    if ((player->weapon & PRESS_SQUISH) == PRESS_SQUISH) {
        BrokenWrap(player, playerId);
    }
    if ((player->weapon & HIT_LIGHTNING) == HIT_LIGHTNING) {
        ThunderWrap(player, playerId);
    }
    if ((player->weapon & HIT_CRITTER) == HIT_CRITTER) {
        SpinWrap(player, playerId);
    }
    if ((player->weapon & HIT_BOMB) == HIT_BOMB) {
        BombRolloverWrap(player, playerId);
    }
    if ((player->weapon & HIT_BOMB_THROW) == HIT_BOMB_THROW) {
        BombThrowRolloverWrap(player, playerId);
    }
    if ((player->weapon & PRESS_BOOSTPAD) == PRESS_BOOSTPAD) {
        set_dashjump(player, playerId);
    }
    if ((player->weapon & PRESS_BOOSTJUMP) == PRESS_BOOSTJUMP) {
        set_dash_midjump(player, playerId);
    }
    if ((player->weapon & USE_STAR) == USE_STAR) {
        SetStar(player, playerId);
    }
    if ((player->weapon & USE_BOO) == USE_BOO) {
        SetVSGhost(player, playerId);
    }
    if (player->weapon & PRESS_SAVESPIN) {
        ProWheelSpinWrap(player, playerId);
    }
    if (player->weapon & HIT_BOAT) {
        BombRolloverWrap(player, playerId);
    }
}

void check_weapon(Player* player, UNUSED s8 playerId, UNUSED s8 screenId) {
    if (((player->jugemu_flag & LAKITU_SCENE) != 0) || ((player->jugemu_flag & ON_LAKITU_ROD) != 0)) {
        player->weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // Green shell
    if ((player->slip_flag & ROLLOVER) == ROLLOVER) {
        player->weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // Spinout (banana or driving)
    if (((player->slip_flag & SPIN_L) == SPIN_L) ||
        ((player->slip_flag & SPIN_R) == SPIN_R)) {
        player->weapon &=
            (ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS)) |
            HIT_TORNADO;
    }
    // Near spinout (banana)
    if ((player->slip_flag & WHEELSPIN) == WHEELSPIN) {
        player->weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    if ((player->handling_flag & SPINOUT_SWERVE) != 0) {
        player->weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // unclear
    if ((player->slip_flag & WING) == WING) {
        player->weapon &= ALL_TRIGGERS & ~((HIT_TRIGGERS ^ HIT_LIGHTNING) | ANY_BOOST_TRIGGERS |
                                             RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // unclear
    if ((player->slip_flag & STORM) == STORM) {
        player->weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // squished
    if ((player->slip_flag & BROKEN) == BROKEN) {
        player->weapon &= (ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS |
                                              STATE_TRANSITION_TRIGGERS)) |
                            PRESS_SQUISH;
    }
    // explosion crash
    if ((player->slip_flag & EXPLODE) == EXPLODE) {
        player->weapon &= ALL_TRIGGERS & ~((HIT_TRIGGERS ^ HIT_LIGHTNING) | ANY_BOOST_TRIGGERS |
                                             RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // hit by star or red shell
    if ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) {
        player->weapon &= ALL_TRIGGERS & ~((HIT_TRIGGERS ^ HIT_LIGHTNING) | ANY_BOOST_TRIGGERS |
                                             RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // boost asphalt
    if ((player->slip_flag & DASH_JUMP) == DASH_JUMP) {
        player->weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // boost ramp
    if ((player->slip_flag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        player->weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // Terrain tumble
    if ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL) {
        player->weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // star
    if ((player->slip_flag & STAR) == STAR) {
        player->weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | USE_MUSHROOM | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // boo
    if ((player->slip_flag & TERESA) == TERESA) {
        player->weapon &= ALL_TRIGGERS & ~(HIT_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // early start spinout
    if ((player->slip_flag & RAPID_ACC) == RAPID_ACC) {
        player->weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | HIT_CRITTER | STATE_TRANSITION_TRIGGERS);
    }
    // CPU_FAST_EFFECTS
    if ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) {
        player->weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
}

void WeaponStatus(Player* player, s8 playerId, s8 screenId) {
    if (player->weapon != 0) {
        check_weapon(player, playerId, screenId);
    }
    if (player->weapon != 0) {
        set_weapon(player, playerId, screenId);
    }
    if ((player->handling_flag & TROPHYJUMP_START) != 0) { // can never be true
        set_result_jump(player, playerId);
    }
}

UNUSED void set_hitptr(Player* player_one, Player* player_two) {
    s32 var_v1;

    // clang-format off
    /*
    if's are being done bracket-less on purpose,
    Technically only the `player_one == gPlayerEight` NEEDS to be
    bracket-less that looks weird
    */
    if (player_one == gPlayerOne) {   var_v1 = 0;
}
    if (player_one == gPlayerTwo) {   var_v1 = 1;
}
    if (player_one == gPlayerThree) { var_v1 = 2;
}
    if (player_one == gPlayerFour) {  var_v1 = 3;
}
    if (player_one == gPlayerFive) {  var_v1 = 4;
}
    if (player_one == gPlayerSix) {   var_v1 = 5;
}
    if (player_one == gPlayerSeven) { var_v1 = 6;
}
    if (player_one == gPlayerEight) { var_v1 = 7;
}
    hitPtr[var_v1] = player_two;
    if (player_two == gPlayerOne) {   var_v1 = 0;
}
    if (player_two == gPlayerTwo) {   var_v1 = 1;
}
    if (player_two == gPlayerThree) { var_v1 = 2;
}
    if (player_two == gPlayerFour) {  var_v1 = 3;
}
    if (player_two == gPlayerFive) {  var_v1 = 4;
}
    if (player_two == gPlayerSix) {   var_v1 = 5;
}
    if (player_two == gPlayerSeven) { var_v1 = 6;
}
    if (player_two == gPlayerEight) { var_v1 = 7;
}
    hitPtr[var_v1] = player_one;
    // clang-format on
}

void check_impact(Player* player, s8 playerIndex, UNUSED s32 arg2) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f14;
    s16 temp;
    s16 temp2;

    if ((player->hit_flag & CRITTER_TOUCH) == CRITTER_TOUCH) {
        temp_f0 = D_8018CE10[playerIndex].unk_04[0];
        temp_f2 = 0;
        temp_f14 = D_8018CE10[playerIndex].unk_04[2];
        if (sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f14 * temp_f14)) >= 6.5) {
            player->force /= 4;
            player->accelcount /= 4;
            if (!(player->slip_flag & SPIN_L) && !(player->slip_flag & SPIN_R)) {
                SetSpin(player, playerIndex);
            }
        }
    } else {
        temp_f0 = old_velocity[playerIndex][0] - player->velocity[0];
        temp_f2 = old_velocity[playerIndex][1] - player->velocity[1];
        temp_f14 = old_velocity[playerIndex][2] - player->velocity[2];
        if (sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f14 * temp_f14)) >= 4.2) {
            player->force /= 4;
            player->accelcount /= 4;
            if (!(player->slip_flag & SPIN_L) && !(player->slip_flag & SPIN_R)) {
                SetSpin(player, playerIndex);
            }
        }
        temp = (-(s16) CalcDirection(player->position, &player->old_position[0]));
        temp2 = (player->direction[1] - player->slipang);
        temp = temp - temp2;
        player->hit_sterr = temp / DEGREES(1);
    }
}

void check_impact_wall(Player* player, f32* arg1, f32* arg2, UNUSED s8 arg3, UNUSED s8 arg4, UNUSED Vec3f arg5) {
    Mat3 sp64;
    Vec3f sp58;
    Vec3f sp4C;
    s16 var_v0;
    s16 t0;
    UNUSED s32 pad;
    s16 sp30[10] = { 0, DEGREES(1), DEGREES(2), DEGREES(3), DEGREES(4), DEGREES(5), DEGREES(6), DEGREES(7), DEGREES(8), DEGREES(9) };

    if (((u16) player->wallhitcount) <= 0) {
        return;
    }

    if (((u16) player->wallhitcount) > 2) {
        return;
    }

    if (!(player->hit_flag & 0x20)) {
        return;
    }

    MakeAlignMatrix(sp64, 0, 1, 0, (s16) 0);

    sp58[0] = *arg1;
    sp58[1] = 0;
    sp58[2] = *arg2;

    MultipleMatrixByVector(sp58, sp64);

    sp4C[0] = player->old_position[0];
    sp4C[1] = 0;
    sp4C[2] = player->old_position[2];

    MultipleMatrixByVector(sp4C, sp64);

    var_v0 = -(s16) CalcDirection(sp58, sp4C);
    t0 = player->direction[1];
    var_v0 = 0x10000 + (t0 - var_v0);
    var_v0 /= DEGREES(1);

    if (var_v0 < 0x97 && (var_v0 > -0x97)) {
        return;
    }

    var_v0 = (player->nowsterr >> 16) / 6;

    if (var_v0 < 0) {
        var_v0 *= -1;
    }

    if (var_v0 >= 8) {
        var_v0 = 8;
    }

    if ((player->nowsterr >> 16) < 0) {
        player->direction[1] -= sp30[var_v0];
    } else {
        player->direction[1] += sp30[var_v0];
    }
}

void check_impactsterr(Player* player) {
    s32 sp2C[7] = { 47 << 16, 48 << 16, 49 << 16, 50 << 16, 50 << 16, 50 << 16, 50 << 16 };
    s32 spC[8] = { 40 << 16, 44 << 16, 48 << 16, 50 << 16, 50 << 16, 50 << 16, 50 << 16, 50 << 16 };
    s16 temp_t5;

    if (player->hit_sterr >= 0) {
        if ((player->hit_sterr >= 5) && (player->hit_sterr < 30)) {
            player->nowsterr = sp2C[player->hit_sterr / 6];
        }
        if ((player->hit_sterr >= 30) && (player->hit_sterr < 80)) {
            player->nowsterr = spC[(s32) (player->hit_sterr - 30) / 12];
        }
        if (((player->hit_sterr < 80) || (player->hit_sterr > 90)) && (player->hit_sterr > 90) &&
            (player->hit_sterr <= 160)) {
            player->nowsterr = spC[1];
        }
    } else {
        temp_t5 = -player->hit_sterr;
        if ((player->hit_sterr < -4) && (player->hit_sterr >= -30)) {
            player->nowsterr = sp2C[temp_t5 / 6] * -1;
        }
        if ((player->hit_sterr < -30) && (player->hit_sterr >= -80)) {
            player->nowsterr = spC[(s32) (temp_t5 - 30) / 12] * -1;
        }
        if (((player->hit_sterr >= -80) || (player->hit_sterr < -90)) && (player->hit_sterr < -90) &&
            (player->hit_sterr >= -160)) {
            player->nowsterr = spC[1] * -1;
        }
    }
}

void check_slipstream(Player* player, s8 playerIndex) {
    UNUSED s32 pad[3];
    UNUSED s32 uselessAssignment;
    s32 i;
    s32 var_a2;
    Player* playerBorrow;
    Player* players = gPlayerOne;

    var_a2 = 0;

    if (((player->speed / 18.0f) * 216.0f) < 50.0f) {
        player->slipstream_timer = 0;
        player->slip_flag &= ~SLIP_STREAM;
        return;
    }
    if ((player->slip_flag & SLIP_STREAM) == SLIP_STREAM) {
        player->slipstream_timer -= 1;
        if (player->slipstream_timer <= 0) {
            player->slip_flag &= ~SLIP_STREAM;
        }
    } else {
        for (i = 0; i < NUM_PLAYERS; i++) {
            playerBorrow = &players[i];
            if (((player != playerBorrow) && ((playerBorrow->flag & IS_GHOST) == 0) &&
                 (playerBorrow->flag & EXISTS)) &&
                ((var_a2 = slipstream_kart(player, playerBorrow->position[0], playerBorrow->position[1], playerBorrow->position[2]),
                  var_a2 == 1))) {
                player->slipstream_timer += 1;
                if (player->slipstream_timer >= 0x3D) {
                    player->slip_flag |= SLIP_STREAM;
                    if ((player->flag & IS_GHOST) != IS_GHOST) {
                        uselessAssignment = player->flag & IS_GHOST;
                        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008001);
                    }
                    if ((player->flag & IS_GHOST) != IS_GHOST) {
                        uselessAssignment = var_a2;
                        NAPlyTrgStart(playerIndex, 0x19008011);
                    }
                }
                break;
            }
        }

        if (var_a2 == 0) {
            player->slipstream_timer = 0;
        }
    }
}

void drift_counter(Player* player) {
    if ((player->slip_flag & DRIFT) == DRIFT) {
        player->drift_count += 1;
        if (player->drift_count > 100) {
            player->drift_count = 100;
        }
    } else {
        player->drift_count -= 1;
        if (player->drift_count < 0) {
            player->drift_count = 0;
        }
    }
}

void LakituCheck(Player* player, s8 playerId) {
    switch (g_courseID) { /* irregular */
        case COURSE_YOSHI_VALLEY:
            if ((player->bump.distance_zx >= 600.0f) && (g_offroadFlagPlayer1[playerId] == 0)) {
                g_offroadFlagPlayer1[playerId] = 1;
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            } else if (g_offroadFlagPlayer1[playerId] == 0) {
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            } else if (!(player->slip_flag & N_JUMP)) {
                if (OoBCheck(player->bump.last_zx) == 0) {
                    g_offroadFlagPlayer1[playerId] = 0;
                }
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if ((player->bump_status == SNOW_OFFROAD) && (g_offroadFlagPlayer1[playerId] == 0)) {
                g_offroadFlagPlayer1[playerId] = 1;
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            } else if (player->bump_status != SNOW_OFFROAD) {
                g_offroadFlagPlayer1[playerId] = 0;
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            if (((player->slip_flag & DASH_JUMP) != 0) && (g_offroadFlagPlayer1[playerId] == 0)) {
                g_offroadFlagPlayer1[playerId] = 1;
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            } else if (((player->slip_flag & DASH_JUMP) == 0) && !(player->slip_flag & N_JUMP)) {
                g_offroadFlagPlayer1[playerId] = 0;
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            }
            break;
        case COURSE_RAINBOW_ROAD:
            if ((player->bump.distance_zx >= 600.0f) && (g_offroadFlagPlayer1[playerId] == 0)) {
                g_offroadFlagPlayer1[playerId] = 1;
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            } else if (g_offroadFlagPlayer1[playerId] == 0) {
                g_playerPathPointCopy[playerId] = gNearestPathPointByPlayerId[playerId];
                jugemu_line[playerId] = CurrentPathID[playerId];
            } else if (!((player->slip_flag & N_JUMP) || (player->jugemu_flag & IS_IN_WATER))) {
                g_offroadFlagPlayer1[playerId] = 0;
            }
            break;
        default:
            g_offroadFlagPlayer1[playerId] = 0;
            if (1) {}
            break;
    }
}

void CheckSplash(Player* player, s8 playerIndex) {
    g_waterlevelPlayer[playerIndex] = CheckWaterLevel(player);
    if (player->position[1] <= g_waterlevelPlayer[playerIndex]) {
        player->water_flag |= HALF_SUBMERGED;
    } else {
        player->water_flag &= ~HALF_SUBMERGED;
    }
    if (player->radius < (g_waterlevelPlayer[playerIndex] - player->position[1])) {
        player->water_flag |= SUBMERGED;
        player->water_flag &= ~HALF_SUBMERGED;
    } else {
        player->water_flag &= ~SUBMERGED;
    }
    if (player->radius < (g_waterlevelPlayer[playerIndex] - player->position[1])) {
        if ((player->water_flag & SPLASH_START) != SPLASH_START) {
            player->water_flag |= SPLASH_DIVE;
            player->water_flag |= SPLASH_START;
            if ((g_courseID != COURSE_KOOPA_BEACH) && (g_courseID != COURSE_SKYSCRAPER) &&
                (g_courseID != COURSE_RAINBOW_ROAD) && ((player->flag & IS_PLAYER) == IS_PLAYER)) {
                if ((g_courseID == COURSE_BOWSER_CASTLE) || (g_courseID == COURSE_BIG_DONUT)) {
                    NAPlyTrgStart((u8) playerIndex, 0x1900801CU);
                } else {
                    NAPlyTrgStart((u8) playerIndex, 0x19008008U);
                }
            }
        }
    }
    if ((g_courseID == COURSE_KOOPA_BEACH) || (g_courseID == COURSE_SKYSCRAPER) ||
        (g_courseID == COURSE_RAINBOW_ROAD)) {
        player->water_flag &= ~(SPLASH_DIVE | SPLASH_START);
    }
    if ((player->radius < (g_waterlevelPlayer[playerIndex] - player->position[1])) &&
        (player->bump.distance_zx >= 600.0f)) {
        player->jugemu_flag |= IS_IN_WATER;
    }
    if (player->bump.distance_zx >= 600.0f) {
        player->jugemu_flag |= LAKITU_CAMERA;
    } else if ((player->slip_flag & N_JUMP) != N_JUMP) {
        player->jugemu_flag &= ~LAKITU_CAMERA;
    }
    if ((player->flag & IS_CPU_PLAYER) &&
        ((OoBCheck(player->bump.last_zx) != 0) || (player->jugemu_flag & IS_IN_WATER))) {
        if (!(player->jugemu_flag & ON_LAKITU_ROD) && !(player->jugemu_flag & LAKITU_SCENE) &&
            !(player->slip_flag & CENTER_LINE)) {
            ExplorerSetJugemu(player);
            ExplorerCallJugemu(player);
        }
    }
    if ((player->flag & IS_CPU_PLAYER) && (player->bump_status == OUT_OF_BOUNDS) && !(player->slip_flag & N_JUMP)) {
        ExplorerSetJugemu(player);
        ExplorerCallJugemu(player);
    }
    LakituCheck(player, playerIndex);
}

void CheckKartHit(Player* player, s8 playerIndex, s8 arg2) {
    if ((player->hit_flag & 1) != 1) {
        if ((player->slip_flag & CARHIT) == CARHIT) {
            if ((player->slip_flag & TURBO) != TURBO) {
                check_impact(player, playerIndex, arg2);
            }
            player->handling_flag &= ~REVERSE_GEAR;
            player->hit_flag |= 1;
            player->hit_flag |= 8;
            if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                ShakeHitCamera(player, 2.8f);
            }
            if ((player->hit_flag & CRITTER_TOUCH) == CRITTER_TOUCH) {
                if ((player->hit_flag & CRITTER_TOUCH_GATE) != CRITTER_TOUCH_GATE) {
                    player->hit_flag |= CRITTER_TOUCH_GATE;
                    player->hit_flag |= INSTANT_SPINOUT;
                    if (player->slip_flag & TURBO) {
                        reset_turbo(player);
                    }
                }
            }
        }
    }
    if ((player->slip_flag & CARHIT) == CARHIT) {
        player->slip_flag &= ~CARHIT;
        player->hitcount = 1;
        player->handling_flag &= ~REVERSE_GEAR;
        return;
    }
    player->hit_flag &= ~0x0001;
    player->slip_flag &= ~CARHIT;
    if (player->hitcount > 0) {
        player->hitcount += 1;
    }
    if (player->hitcount >= 0xA) {
        player->hitcount = 0;
    }
}

void CheckWall(Player* player, s8 playerId, Vec3f velocity) {
    f32 temp_f0;
    f32 var_f14;
    f32 xdist;
    f32 ydist;
    f32 zdist;

    temp_f0 = player->position[1] - player->ground;

    if (((((player->slip_flag & ROLLOVER_FALL) != ROLLOVER_FALL) &&
          ((player->slip_flag & DASH_JUMP) == DASH_JUMP)) ||
         ((((temp_f0 >= 20.0f) || (temp_f0 < (-1.0f))) && ((player->slip_flag & ROLLOVER_FALL) == 0)) &&
          (player->slip_flag & N_JUMP)) ||
         ((player->bump.flag_zx == 0) && ((player->slip_flag & ROLLOVER_FALL) == 0))) &&
        (((player->jugemu_flag & ON_LAKITU_ROD) == 0) || (!(player->jugemu_flag & LAKITU_SCENE)))) {
        SetRolloverFall(player, playerId);
    }
    if ((player->hit_flag & 0x20) != 0x20) {
        if ((player->bump.distance_xy < (-1.0f)) || (player->bump.distance_yz < (-1.0f))) {
            player->wallhitcount = 1;
        }
        player->hit_flag |= 0x20;
    }
    if (((player->flag & IS_PLAYER) == IS_PLAYER) && (((player->speed / 18.0f) * 216.0f) > 30.0f)) {
        ShakeHitCamera(player, 3.0f);
    }
    player->hit_flag |= 0x10;
    player->wallhitcount++;
    if (player->wallhitcount >= 0xA) {
        player->wallhitcount = 0;
    }
    if ((player->kartpitch >= 0) && (((player->speed / 18.0f) * 216.0f) > 5.0f)) {
        AccelOff(player, 18.0f);
    }
    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
        xdist = old_velocity[playerId][0] - velocity[0];
        var_f14 = old_velocity[playerId][1] - velocity[1];
        ydist = var_f14; // okay
        zdist = old_velocity[playerId][2] - velocity[2];
        var_f14 = sqrtf((xdist * xdist) + (ydist * ydist) + (zdist * zdist)) / 3;
        if (var_f14 >= 1.0) {
            var_f14 = 1.0f;
        }
        if ((var_f14 <= 0.6) && (((player->speed / 18.0f) * 216.0f) >= 40.0f) &&
            (!(player->flag & IS_GHOST))) {
            Na_PlyCrush_Start(playerId, 0.6F);
        } else if (!(player->flag & IS_GHOST)) {
            if ((var_f14 <= 0.6) && (((player->speed / 18.0f) * 216.0f) < 40.0f) &&
                (((player->speed / 18.0f) * 216.0f) >= 10.0f)) {
                Na_PlyCrush_Start(playerId, 0.3F);
            } else {
                Na_PlyCrush_Start(playerId, var_f14);
            }
        }
    }
    if (player->slip_flag & TURBO) {
        reset_turbo(player);
        player->force /= 2;
    }
}

void SetStrategy(Player* player, s8 playerIndex, s8 arg2) {
    if (((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((player->jugemu_flag & LAKITU_SCENE) == LAKITU_SCENE)) {
        ExplorerHangJugemu(player, playerIndex, arg2);
    }
    if ((player->slip_flag & WHEELSPIN) == WHEELSPIN) {
        wheelspin(player, playerIndex);
    }
    if (player->handling_flag & SPINOUT_SWERVE) {
        pro_wheelspin(player, playerIndex);
    }
    if ((player->slip_flag & TURBO) == TURBO) {
        now_turbo(player);
    }
    if ((player->slip_flag & DASH_JUMP) == DASH_JUMP) {
        now_dashjump(player);
    }
    if ((player->slip_flag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        now_dash_midjump(player);
    }
    if ((s32) (player->slip_flag & BROKEN) == BROKEN) {
        broken(player, playerIndex);
    }
    if ((player->slip_flag & THUNDER) == THUNDER) {
        thunder(player, playerIndex);
    }
    if ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL) {
        rollover_fall(player, playerIndex);
    }
    if ((player->slip_flag & STAR) == STAR) {
        star(player, playerIndex);
    }
    if ((player->slip_flag & TERESA) == TERESA) {
        vs_ghost(player, playerIndex);
    }
    if (((player->slip_flag & STOPSTERR) == STOPSTERR) && (player->stopsetrr_count >= 100)) {
        AccelOff(player, 4.0f);
    }
    if (((player->slip_flag & SPIN_L) == SPIN_L) ||
        ((player->slip_flag & SPIN_R) == SPIN_R)) {
        spin(player, playerIndex);
    }
    if ((player->slip_flag & ROLLOVER) == ROLLOVER) {
        rollover(player, playerIndex);
    }
    if ((player->slip_flag & EXPLODE) == EXPLODE) {
        bomb(player, playerIndex);
    }
    if ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) {
        bomb_throw(player, playerIndex);
    }
    if ((player->slip_flag & RAPID_ACC) == RAPID_ACC) {
        rapidacc(player, playerIndex);
    }
    if ((player->slip_flag & WING) == WING) {
        wing(player, playerIndex);
    }
    if ((player->slip_flag & STORM) == STORM) {
        storm(player, playerIndex);
        AccelOff(player, 10.0f);
    }
    if (g_startingIndicator != RACE_DONE) {
        if (player->weapon & PRESS_BECOME_BOMB) {
            change_bomb(player, playerIndex);
        }
        if (player->weapon & PRESS_IS_BOMB) {
            real_bomb(player, playerIndex);
        }
    }
    if (player->handling_flag & TROPHYJUMP_END) { // never true
        result_jump(player, playerIndex);
    }
}

void opening_kart(Player* player, s8 playerIndex) {
    Vec3f sp4C;
    f32 temp_f18;
    s16 temp_t1;
    s16 temp;
    s16 temp2;
    f32 thing0;
    UNUSED s32 pad;

    sp4C[0] = grid_x[kartrank[playerIndex]];
    sp4C[1] = 0;
    sp4C[2] = grid_z[kartrank[playerIndex]];

    temp = -(s16) CalcDirection(player->position, sp4C);
    temp2 = player->direction[1];
    temp = (temp - temp2);

    thing0 = 8;

    if (temp > ((s16) (thing0 * DEGREES(1)))) {
        temp = (thing0 * DEGREES(1));
    }
    if (temp < ((s16) (-thing0 * DEGREES(1)))) {
        temp = (-thing0 * DEGREES(1));
    }

    temp_t1 = (D_80165020[playerIndex] + ((s16) ((temp * 0x35) / (thing0 * DEGREES(1))))) / 2;
    enemy_stick_angle(player, (s16) temp_t1);
    D_80165020[playerIndex] = (s16) temp_t1;

    temp_f18 = sqrtf((sp4C[0] - player->position[0]) * (sp4C[0] - player->position[0]) +
                     (sp4C[2] - player->position[2]) * (sp4C[2] - player->position[2]));
    if (temp_f18 <= 8.0f) {
        ChaseDir(&player->direction[1], -DEGREES(180), DEGREES(2));
        if ((player->direction[1] <= (-179 * DEGREES(1))) || (player->direction[1] >= (179 * DEGREES(1)))) {
            player->flag &= ~IS_GP_OPENING;
        }
        player->force = 0;
        player->speed = 0;
        player->power_cont = 0;
        player->tcol_count = 0;
        player->nowsterr = 0;
        player->velocity[0] = 0;
        player->velocity[1] = 0;
        player->velocity[2] = 0;
        player->slipang = 0;
        player->sterrangle = 0;
    } else {
        player->force = 1200;
    }
}

void RunKart(Player* player, UNUSED Camera* camera, s8 screenId, s8 playerId) {
    Vec3f sp184 = { 0.0, 0.0, 1.0 };
    Vec3f sp178 = { 0.0, 0.0, 0.0 };
    Vec3f sp16C = { 0.0, 0.0, 0.0 };
    Vec3f sp160 = { 0.0, 0.0, 0.0 };
    f32 sp104[] = { 0.825, 0.8, 0.725, 0.625, 0.425, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,
                    0.3,   0.3, 0.3,   0.3,   0.3,   0.3, 0.3, 0.3, 0.3, 0.3, 0.3 };
    f32 temp;
    f32 nextX;
    f32 nextY;
    f32 nextZ;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 temp2;
    s32 temp_v0_3;
    s32 temp3;
    f32 temp_f2_2;
    UNUSED s32 pad[8];
    f32 gravityX;
    f32 gravityY;
    f32 gravityZ;
    f32 surfaceDistance;
    UNUSED s32 pad2;
    Vec3f newVelocity;
    Vec3f sp8C;
    UNUSED s32 pad3[3];
    s32 sp7C = 0;
    UNUSED s32 pad4[6];

    kart_engine_echo(player, playerId);
    drift_counter(player);
    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
        drift_turbo(player, playerId);
    }
    WeaponStatus(player, playerId, screenId);
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        check_slipstream(player, playerId);
    }
    SetStrategy(player, playerId, screenId);
    if (((player->slip_flag & STOPSTERR) == STOPSTERR) && (player->stopsetrr_count >= 100)) {
        sp7C = 2;
    }
    yoko_g(player, sp160);
    AddGravityEdit(player);
    SlipCheck(player, playerId);
    if (player->handling_flag & REVERSE_GEAR) {
        player->gravity_xz[0] *= -1.0f;
        player->gravity_xz[2] *= -1.0f;
    }
    if ((player->tire_RL.Status == ASPHALT) && (player->tire_RR.Status == ASPHALT)) {
        gravityX = (-1 * (player->gravity_xz[0] + sp16C[0])) +
               ((-player->bump.bump_zx[0] * player->gravity) * 0.925);
        gravityY = (-player->bump.bump_zx[1] * player->gravity);
        gravityZ = (-1 * (player->gravity_xz[2] + sp16C[2])) +
               ((-player->bump.bump_zx[2] * player->gravity) * 0.925);
    } else {
        temp3 = (((player->speed / 18.0f) * 216.0f) / 10.0f);
        if (temp3 >= 10) {
            temp3 = 10;
        }
        gravityX = -1 * (player->gravity_xz[0] + sp16C[0]) +
                   ((-player->bump.bump_zx[0] * player->gravity) * sp104[temp3]);
        gravityY = (-player->bump.bump_zx[1] * player->gravity);
        gravityZ = -1 * (player->gravity_xz[2] + sp16C[2]) +
                   ((-player->bump.bump_zx[2] * player->gravity) * sp104[temp3]);
    }
    if (((player->slip_flag & N_JUMP) != N_JUMP) &&
        ((player->slip_flag & SPIN_TURN) == SPIN_TURN)) {
        gravityX = 0 * (player->gravity_xz[0] + sp16C[0]);
        gravityY = -1 * player->gravity / 4;
        gravityZ = 0 * (player->gravity_xz[2] + sp16C[2]);
    }
    if ((player->slip_flag & N_JUMP) == N_JUMP) {
        gravityX = 0 * (player->gravity_xz[0] + sp16C[0]);
        gravityY = -1 * player->gravity;
        gravityZ = 0 * (player->gravity_xz[2] + sp16C[2]);
    }
    temp_f2_2 = ((player->old_position[2] - player->position[2]) * cosT(player->direction[1] + player->slipang)) +
                (-(player->old_position[0] - player->position[0]) * sinT(player->direction[1] + player->slipang));
    if (temp_f2_2 > 0.1) {
        player->handling_flag |= REVERSE_ACCEL;
    } else {
        player->handling_flag &= ~REVERSE_ACCEL;
    }
    if (((player->force <= 0.0f) &&
         ((temp_v0_3 = player->slip_flag, (temp_v0_3 & BREAKE) == BREAKE))) &&
        ((temp_v0_3 & SPIN_TURN) != SPIN_TURN)) {
        sp178[2] = temp_f2_2 * 4500.0f;
    } else {
        sp178[2] = 0.0f;
    }
    sp178[1] = 0.0f;
    MultipleMatrixByVector(sp178, player->align);
    gravityX += sp178[0];
    gravityZ += sp178[2];
    CheckKartHit(player, playerId, screenId);
    sp184[2] = PowerCheck(player, playerId);
    MultipleMatrixByVector(sp184, player->align);
    newVelocity[0] = player->velocity[0];
    newVelocity[1] = player->velocity[1];
    newVelocity[2] = player->velocity[2];
    if (((player->hitcount < 3) && (((s32) player->wallhitcount) < 3)) ||
        ((player->slip_flag & TURBO) == TURBO)) {

        if (((player->nowsterr >> 16) >= 40) || ((player->nowsterr >> 16) <= -40)) {

            newVelocity[0] += (((((f64) ((sp184[0] + gravityX) + sp160[0])) -
                                 (newVelocity[0] * (0.12 * ((f64) player->mass)))) /
                                6000.0) /
                               (((((f64) player->brake_time) * 0.6) + 1.0) + sp7C));
            newVelocity[2] += (((((f64) ((sp184[2] + gravityZ) + sp160[2])) -
                                 (newVelocity[2] * (0.12 * ((f64) player->mass)))) /
                                6000.0) /
                               (((((f64) player->brake_time) * 0.6) + 1.0) + sp7C));
        } else {
            newVelocity[0] += (((((f64) ((sp184[0] + gravityX) + sp160[0])) -
                                 (newVelocity[0] * (0.12 * ((f64) player->mass)))) /
                                6000.0) /
                               (sp7C + 1));
            newVelocity[2] += (((((f64) ((sp184[2] + gravityZ) + sp160[2])) -
                                 (newVelocity[2] * (0.12 * ((f64) player->mass)))) /
                                6000.0) /
                               (sp7C + 1));
        }
    } else {
        newVelocity[0] +=
            (((((f64) ((sp184[0] + gravityX) + sp160[0])) - (newVelocity[0] * (0.12 * ((f64) player->mass)))) /
              6000.0) /
             30.0);
        newVelocity[2] +=
            (((((f64) ((sp184[2] + gravityZ) + sp160[2])) - (newVelocity[2] * (0.12 * ((f64) player->mass)))) /
              6000.0) /
             30.0);
    }
    newVelocity[1] +=
        (((((f64) ((sp184[1] + gravityY) + sp160[1])) - (newVelocity[1] * (0.12 * ((f64) player->mass)))) /
          6000.0) /
         ((f64) player->sus.jump_firstspeed));
    if (((((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
          ((player->jugemu_flag & LAKITU_SCENE) == LAKITU_SCENE)) ||
         ((player->slip_flag & BROKEN) == BROKEN)) ||
        (player->jugemu_flag & IS_IN_WATER)) {
        newVelocity[0] = 0.0f;
        newVelocity[1] = 0.0f;
        newVelocity[2] = 0.0f;
    }
    if ((player->handling_flag & CPU_SIMPLE_KART) == CPU_SIMPLE_KART) {
        player->handling_flag &= ~CPU_SIMPLE_KART;
    }

    posX = player->position[0];
    posY = player->position[1];
    posZ = player->position[2];

    player->old_position[0] = player->position[0];
    player->old_position[2] = player->position[2];
    player->old_position[1] = player->position[1];
    nextX = posX + player->velocity[0] + D_8018CE10[playerId].unk_04[0];
    nextY = posY + player->velocity[1];
    nextZ = posZ + player->velocity[2] + D_8018CE10[playerId].unk_04[2];

    if (((((player->jugemu_flag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
          ((player->jugemu_flag & LAKITU_SCENE) != LAKITU_SCENE)) &&
         ((player->slip_flag & BROKEN) != BROKEN)) &&
        (!(player->jugemu_flag & IS_IN_WATER))) {
        DriftJump(player);
        nextY += player->jump;
        nextY -= 0.02;
    }
    custom_check_bump_2(&player->bump, player->radius, nextX, nextY, nextZ, player->old_position[0],
                            player->old_position[1], player->old_position[2]);
    player->up_vector[0] = 0.0f;
    player->up_vector[2] = 0.0f;
    player->up_vector[1] = 1.0f;
    if ((player->handling_flag & REVERSE_GEAR) != REVERSE_GEAR) {
        MakeAlignMatrix(player->align, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                     player->direction[1]);
    } else {
        MakeAlignMatrix(player->align, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                     player->direction[1] + DEGREES(180));
    }
    player->slip_flag |= N_JUMP;
    player->jumpcount += 1;
    surfaceDistance = player->bump.distance_zx;
    if (surfaceDistance <= 0.0f) {
        player->slip_flag = player->slip_flag & (~D_JUMP);
        player->slip_flag = player->slip_flag & (~N_JUMP);
        if (player->jumpcount >= 35) {
            if (player->jumpcount >= 0x32) {
                player->jumpcount = 0x32;
            }
            player->sus.bound_firstspeed = 3.0f;
            player->sus.bound_timer = 0;
            player->talk |= POOMP;
            if ((((player->flag & IS_PLAYER) == IS_PLAYER) &&
                 ((player->slip_flag & DASH_JUMP) == DASH_JUMP)) &&
                ((player->flag & IS_GHOST) != IS_GHOST)) {

                NAPlyTrgStart(playerId, 0x1900A60AU);
            } else if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
                       ((player->flag & IS_GHOST) != IS_GHOST)) {
                NaPlyLandStart((u8) playerId, ((f32) player->jumpcount) / 35.0f);
            }
            if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                player->handling_flag |= LANDING_DUST;
            }
        }
        if (((player->jumpcount < 0x23) && (player->jumpcount >= 0x1C)) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            player->sus.bound_firstspeed = 2.8f;
            player->sus.bound_timer = 0;
            if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
                ((player->flag & IS_GHOST) != IS_GHOST)) {
                NaPlyLandStart((u8) playerId, ((f32) player->jumpcount) / 35.0f);
            }
            if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                player->handling_flag |= LANDING_DUST;
            }
        }
        if (((player->jumpcount < 0x1C) && (player->jumpcount >= 4)) && (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
            player->sus.bound_timer = 0;
            player->sus.bound_firstspeed = 1.5f;
            if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
                ((player->flag & IS_GHOST) != IS_GHOST)) {
                if (((player->jumpcount < 0xB) && (player->jumpcount >= 4)) && (g_courseID == COURSE_BOWSER_CASTLE)) {
                    NaPlyLandStart((u8) playerId, player->jumpcount / 14.0f);
                } else {
                    NaPlyLandStart((u8) playerId, player->jumpcount / 25.0f);
                }
            }
        }
        player->jumpcount = 0;
        player->jump = player->jumpcount;
    }
    surfaceDistance = player->bump.distance_zx;
    if (surfaceDistance <= 0.0f) {
        CheckMapBG_ZX_Hook(player, sp8C, newVelocity, sp178, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    surfaceDistance = player->bump.distance_xy;
    if (surfaceDistance < 0.0f) {
        CheckMapBG_XY(player, sp8C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(player, playerId, newVelocity);
    }
    surfaceDistance = player->bump.distance_yz;
    if (surfaceDistance < 0.0f) {
        CheckMapBG_YZ(player, sp8C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(player, playerId, newVelocity);
    }
    surfaceDistance = player->bump.distance_xy;
    if (surfaceDistance >= 0.0f) {
        surfaceDistance = player->bump.distance_yz;
        if (surfaceDistance >= 0.0f) {
            player->hit_flag &= 0xFFDF;
            if (player->wallhitcount != 0) {
                player->wallhitcount++;
                if (player->wallhitcount >= 10) {
                    player->wallhitcount = 0;
                }
            }
        }
    }
    if (((!(player->slip_flag & N_JUMP)) && (CheckSlope(player->bump.last_zx) != 0)) &&
        ((player->slip_flag & ROLLOVER_FALL) != ROLLOVER_FALL)) {
        if ((!(player->jugemu_flag & ON_LAKITU_ROD)) || (!(player->jugemu_flag & LAKITU_SCENE))) {
            SetRolloverFall(player, playerId);
        }
    } else if (((!(player->slip_flag & N_JUMP)) && (CheckSlope(player->bump.last_zx) == 0)) &&
               (player->slip_flag & ROLLOVER_FALL)) {
        ResetRolloverFall(player, playerId);
    }
    player->ground = CalcHeight(nextX, nextY, nextZ, player->bump.last_zx);
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        (((g_ScreenSplitA == SCREEN_MODE_1P) || (g_ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL)) ||
         (g_ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        TirePosition(player, nextX, nextY, nextZ);
    } else {
        EnemyTirePosition(player, nextX, nextY, nextZ);
    }
    SetSlipAngle(player, playerId, posX, posZ, nextX, nextZ);

    temp2 = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    player->old_speed = player->speed;
    player->speed = sqrtf(temp2);

    if ((player->force <= 0.0f) && (player->speed <= 0.08) && (D_8018CE10[playerId].unk_04[0] == 0.0f) &&
        (D_8018CE10[playerId].unk_04[2] == 0.0f)) {
        newVelocity[0] = newVelocity[0] + (-1 * newVelocity[0]);
        newVelocity[2] = newVelocity[2] + (-1 * newVelocity[2]);
    } else {
        player->position[0] = nextX;
        player->position[2] = nextZ;
    }
    player->position[1] = nextY;
    if ((player->flag & IS_PLAYER) && (!(player->flag & IS_CPU_PLAYER))) {
        check_impact_wall(player, &nextX, &nextZ, screenId, playerId, newVelocity);
    }
    player->gravity_xz[0] = sp178[0];
    player->gravity_xz[2] = sp178[2];
    player->velocity[0] = newVelocity[0];
    player->velocity[1] = newVelocity[1];
    player->velocity[2] = newVelocity[2];
    old_velocity[playerId][0] = newVelocity[0];
    old_velocity[playerId][1] = newVelocity[1];
    old_velocity[playerId][2] = newVelocity[2];
    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[player->kart] < player->speed) {
            temp = gKartTopSpeedTable[player->kart] / player->speed;
            player->velocity[0] *= temp;
            player->velocity[1] *= temp;
            player->velocity[2] *= temp;
            player->speed = gKartTopSpeedTable[player->kart];
        }
    }
    if ((player->handling_flag & REVERSE_GEAR) == REVERSE_GEAR) {
        if (player->speed > 1) {
            temp = 1 / player->speed;
            player->velocity[0] *= temp;
            player->velocity[1] *= temp;
            player->velocity[2] *= temp;
            player->speed = 1;
        }
    }
    if (player->bump.distance_zx >= 500.0f) {
        player->sterrangle = (s16) (((s16) player->sterrangle) / 2);
    }
    CheckSplash(player, playerId);
}

void set_hight(Player* player) {
    s32 player_index;

    player_index = check_kart_number(player);
    player->jmp_acc = 0.0f;
    player->jmp_speed = 0.0f;
    player->jump = 0.0f;
    player->position[1] = CheckHight(player->position[0], enemy_tenuki_y[player_index] + 10.0f, player->position[2]) +
                     player->radius;
    if (((player->position[1] - enemy_tenuki_y[player_index]) > 1200.0f) ||
        ((player->position[1] - enemy_tenuki_y[player_index]) < -1200.0f)) {
        player->position[1] = player->old_position[1];
    }
    player->velocity[1] = 0.0f;
}

void SpinKart(Player* player, UNUSED Camera* camera, s8 screenId, s8 playerId) {
    Vec3f spEC = { 0.0f, 0.0f, 1.0f };
    Vec3f spE0 = { 0.0f, 0.0f, 0.0f };
    Vec3f spD4 = { 0.0f, 0.0f, 0.0f };
    f32 nextX;
    f32 nextY;
    f32 nextZ;
    f32 topSpeedMultiplier;
    UNUSED s32 pad;
    f32 posX;
    f32 posY;
    f32 posZ;
    UNUSED s32 pad2[12];
    f32 gravityX;
    f32 gravityY;
    f32 gravityZ;
    f32 surfaceDistance;
    UNUSED s32 pad3[4];
    f32 temp;
    Vec3f newVelocity;
    Vec3f sp48;
    s16 sp46;
    WeaponStatus(player, playerId, screenId);
    if ((((((((player->slip_flag & SPIN_L) == SPIN_L) ||
             ((player->slip_flag & SPIN_R) == SPIN_R)) ||
            ((player->slip_flag & RAPID_ACC) == RAPID_ACC)) ||
           ((player->slip_flag & WING) == WING)) ||
          ((player->slip_flag & STORM) == STORM)) ||
         ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN)) ||
        (player->handling_flag & TROPHYJUMP_END)) {
        sp46 = 1;
    } else {
        sp46 = 0;
    }
    SetStrategy(player, playerId, screenId);
    AddGravityEdit(player);
    SlipCheck(player, playerId);
    if ((((player->slip_flag & SPIN_L) == SPIN_L) ||
         ((player->slip_flag & SPIN_R) == SPIN_R)) ||
        ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN)) {
        gravityX =
            -1 * (player->gravity_xz[0]) + (((-player->bump.bump_zx[0]) * player->gravity) * 0.1);
        gravityY = (-player->bump.bump_zx[1]) * player->gravity;
        gravityZ =
            -1 * (player->gravity_xz[2]) + (((-player->bump.bump_zx[2]) * player->gravity) * 0.1);
    } else {
        gravityX = -1 * player->gravity_xz[0];
        gravityY = -1 * player->gravity;
        gravityZ = -1 * player->gravity_xz[2];
    }
    CheckKartHit(player, playerId, screenId);
    if (sp46 == 1) {
        MakeAlignMatrix(player->align, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                     (s16) ((s32) player->direction[1]));
        MakeAlignMatrix(player->align2, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                     (s16) ((s32) player->old_direction));
    } else {
        MakeAlignMatrix(player->align, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                     (s16) ((s32) player->direction[1]));
    }
    spEC[2] = PowerCheck(player, playerId);
    if (sp46 == 1) {
        MultipleMatrixByVector(spEC, player->align2);
    } else {
        MultipleMatrixByVector(spEC, player->align);
    }
    newVelocity[0] = player->velocity[0];
    newVelocity[1] = player->velocity[1];
    newVelocity[2] = player->velocity[2];
    if ((player->hitcount < 3) && ((player->wallhitcount) < 3) &&
        ((player->slip_flag & ROLLOVER) != ROLLOVER) &&
        ((player->slip_flag & EXPLODE) != EXPLODE) &&
        ((player->slip_flag & THROW_EXPLODE) != THROW_EXPLODE)) {
        newVelocity[0] +=
            (((((spEC[0] + gravityX) + spD4[0])) - (newVelocity[0] * (0.12 * player->mass))) / 6000) /
            ((player->brake_time * 5.0f) + 1.0f);
        newVelocity[2] +=
            (((((spEC[2] + gravityZ) + spD4[2])) - (newVelocity[2] * (0.12 * player->mass))) / 6000) /
            ((player->brake_time * 5.0f) + 1.0f);
    } else {
        newVelocity[0] +=
            ((((f64) (spEC[0] + gravityX + spD4[0]) - (newVelocity[0] * (0.2 * (f64) player->mass))) / 6000) *
             0.08);
        newVelocity[2] +=
            ((((f64) (spEC[2] + gravityZ + spD4[2]) - (newVelocity[2] * (0.2 * (f64) player->mass))) / 6000) *
             0.08);
    }
    newVelocity[1] += (((((spEC[1] + gravityY) + spD4[1])) - (newVelocity[1] * (0.12 * player->mass))) / 6000) /
                      player->sus.jump_firstspeed;

    if (((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((player->jugemu_flag & LAKITU_SCENE) == LAKITU_SCENE)) {
        newVelocity[0] = 0.0f;
        newVelocity[1] = 0.0f;
        newVelocity[2] = 0.0f;
    }
    posX = player->position[0];
    posY = player->position[1];
    posZ = player->position[2];

    player->old_position[0] = player->position[0];
    player->old_position[1] = player->position[1];
    player->old_position[2] = player->position[2];

    nextX = posX + player->velocity[0] + D_8018CE10[playerId].unk_04[0];
    nextY = posY + player->velocity[1];
    nextZ = posZ + player->velocity[2] + D_8018CE10[playerId].unk_04[2];
    DriftJump(player);
    nextY += player->jump;
    custom_check_bump_2(&player->bump, player->radius, nextX, nextY, nextZ, player->old_position[0],
                            player->old_position[1], player->old_position[2]);
    player->slip_flag |= N_JUMP;
    player->jumpcount += 1;
    player->up_vector[0] = 0.0f;
    player->up_vector[2] = 0.0f;
    player->up_vector[1] = 1.0f;
    surfaceDistance = player->bump.distance_zx;
    if (surfaceDistance <= 0.0f) {
        player->slip_flag &= ~D_JUMP;
        player->slip_flag &= ~N_JUMP;
        if ((((player->slip_flag & ROLLOVER) != ROLLOVER) &&
             ((player->slip_flag & EXPLODE) != EXPLODE)) &&
            ((player->slip_flag & THROW_EXPLODE) != THROW_EXPLODE)) {
            if (player->jumpcount >= 0x1C) {
                if (player->jumpcount >= 0x32) {
                    player->jumpcount = 0x0032;
                }
                player->sus.bound_timer = 0;
                player->talk |= POOMP;
                player->sus.bound_firstspeed = 3.0f;
                if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
                    ((player->flag & IS_GHOST) != IS_GHOST)) {
                    NaPlyLandStart((u8) playerId, ((f32) player->jumpcount) / 50.0f);
                }
                if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                    player->handling_flag |= LANDING_DUST;
                }
            }
            if (((player->jumpcount < 0x1C) && (player->jumpcount >= 0xA)) &&
                (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
                player->sus.bound_firstspeed = 2.0f;
                player->sus.bound_timer = 0;
                if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
                    ((player->flag & IS_GHOST) != IS_GHOST)) {
                    NaPlyLandStart((u8) playerId, ((f32) player->jumpcount) / 50.0f);
                }
                if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                    player->handling_flag |= LANDING_DUST;
                }
            }
            player->jumpcount = 0;
        } else {
            if (player->jumpcount >= 0xA) {
                if (player->jumpcount >= 0x32) {
                    player->jumpcount = 0x0032;
                }
                if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
                    ((player->flag & IS_GHOST) != IS_GHOST)) {
                    NaPlyLandStart((u8) playerId, ((f32) player->jumpcount) / 20.0f);
                }
                if (player->jumpcount >= 0x28) {
                    player->jumpcount = 0x0014;
                }
                if ((player->slip_flag & ROLLOVER) == ROLLOVER) {
                    player->jumpcount /= 6.5;
                    player->jmp_acc = 0.06f;
                    player->jmp_speed = 0.0f;
                } else {
                    player->jumpcount /= 7.5;
                    player->jmp_acc = 0.06f;
                    player->jmp_speed = 0.0f;
                    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                        player->handling_flag |= LANDING_DUST;
                    }
                }
            } else {
                player->jumpcount = 0;
            }
        }
        player->jump = (f32) player->jumpcount;
    }
    surfaceDistance = player->bump.distance_zx;
    if (surfaceDistance <= 0.0f) {
        CheckMapBG_ZX_Hook(player, sp48, newVelocity, spE0, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    surfaceDistance = player->bump.distance_xy;
    if (surfaceDistance < 0.0f) {
        CheckMapBG_XY(player, sp48, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(player, playerId, newVelocity);
        AccelOff(player, 6.0f);
    }
    surfaceDistance = player->bump.distance_yz;
    if (surfaceDistance < 0.0f) {
        CheckMapBG_YZ(player, sp48, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(player, playerId, newVelocity);
        AccelOff(player, 6.0f);
    }
    surfaceDistance = player->bump.distance_xy;
    if (surfaceDistance >= 0.0f) {
        surfaceDistance = player->bump.distance_yz;
        if (surfaceDistance >= 0.0f) {
            player->hit_flag &= 0xFFDF;
            if (player->wallhitcount != 0) {
                player->wallhitcount++;
                if ((player->wallhitcount) >= 0xA) {
                    player->wallhitcount = 0;
                }
            }
        }
    }
    if (((CheckSlope(player->bump.last_zx) != 0) &&
         ((player->slip_flag & ROLLOVER_FALL) != ROLLOVER_FALL)) &&
        (((player->speed / 18.0f) * 216.0f) >= 20.0f)) {
        if ((!(player->jugemu_flag & ON_LAKITU_ROD)) || (!(player->jugemu_flag & LAKITU_SCENE))) {
            SetRolloverFall(player, playerId);
        }
    } else if (((!(player->slip_flag & N_JUMP)) && (CheckSlope(player->bump.last_zx) == 0)) &&
               (player->slip_flag & ROLLOVER_FALL)) {
        ResetRolloverFall(player, playerId);
    }
    player->ground = CalcHeight(nextX, nextY, nextZ, player->bump.last_zx);
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        (((g_ScreenSplitA == SCREEN_MODE_1P) || (g_ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL)) ||
         (g_ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        TirePosition(player, nextX, nextY, nextZ);
    } else {
        EnemyTirePosition(player, nextX, nextY, nextZ);
    }
    SetSlipAngle(player, playerId, posX, posZ, nextX, nextZ);
    temp = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    player->old_speed = player->speed;
    player->speed = sqrtf(temp);
    if ((((player->slip_flag & ROLLOVER) != ROLLOVER) && (player->force <= 0) &&
         (player->speed < 0.13)) ||
        (((player->slip_flag & ROLLOVER) != ROLLOVER) && (player->force <= 0) &&
         (player->speed < 0.20) && ((player->slip_flag & BREAKE) == BREAKE))) {
        newVelocity[0] = newVelocity[0] + (-1 * newVelocity[0]);
        newVelocity[2] = newVelocity[2] + (-1 * newVelocity[2]);
    } else {
        player->position[0] = nextX;
        player->position[2] = nextZ;
    }
    player->position[1] = nextY;
    player->gravity_xz[0] = spE0[0];
    player->gravity_xz[2] = spE0[2];
    player->velocity[0] = newVelocity[0];
    player->velocity[1] = newVelocity[1];
    player->velocity[2] = newVelocity[2];
    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[player->kart] < player->speed) {
            topSpeedMultiplier = gKartTopSpeedTable[player->kart] / player->speed;
            player->velocity[0] *= topSpeedMultiplier;
            player->velocity[1] *= topSpeedMultiplier;
            player->velocity[2] *= topSpeedMultiplier;
            player->speed = gKartTopSpeedTable[player->kart];
        }
    }
    CheckSplash(player, playerId);
}

void RunKartSimple(Player* player, UNUSED Camera* camera, s8 screenId, s8 playerId) {
    Vec3f spF4 = { 0.0f, 0.0f, 1.0f };
    UNUSED Vec3f spE8 = { 0.0f, 0.0f, 0.0f };
    Vec3f spDC = { 0.0f, 0.0f, 0.0f };
    Vec3f spD0 = { 0.0f, 0.0f, 0.0f };
    f32 nextX;
    UNUSED s32 pad;
    f32 nextZ;
    UNUSED s32 pad2[15];
    f32 sp84;
    UNUSED s32 pad3;
    f32 sp7C;
    UNUSED s32 pad4[2];
    Vec3f newVelocity;
    UNUSED f32 pad5[7];
    f32 toSqrt;
    f32 topSpeedMultiplier;
    f32 nextY;
    player->slip_flag |= CENTER_LINE;
    player->handling_flag |= CPU_SIMPLE_KART;
    nextY = enemy_tenuki_y[playerId];
    player->drift_count = 0;
    player->slip_flag &= ~DRIFT;
    WeaponStatus(player, playerId, screenId);
    SetStrategy(player, playerId, screenId);
    sp84 = 0 * player->gravity_xz[0] + spDC[0];
    sp7C = 0 * player->gravity_xz[2] + spDC[2];
    player->hitcount = 0;
    player->wallhitcount = 0;
    player->slip_flag &= ~CARHIT;
    spF4[2] = PowerCheck(player, playerId);
    MultipleMatrixByVector(spF4, player->align);
    newVelocity[0] = player->velocity[0];
    newVelocity[1] = 0;
    newVelocity[2] = player->velocity[2];
    newVelocity[0] += (((spF4[0] + sp84) + spD0[0]) - (newVelocity[0] * (0.12 * player->mass))) / 6000.0;
    newVelocity[2] += (((spF4[2] + sp7C) + spD0[2]) - (newVelocity[2] * (0.12 * player->mass))) / 6000.0;
    player->old_position[0] = player->position[0];
    player->old_position[1] = nextY;
    player->old_position[2] = player->position[2];
    nextX = player->position[0] + player->velocity[0];
    nextZ = player->position[2] + player->velocity[2];
    player->slipang = 0;
    player->jmp_acc = 0;
    player->jmp_speed = 0;
    player->jump = 0;
    MakeAlignMatrix(player->align, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                 player->direction[1]);
    player->jumpcount = 0;
    player->slip_flag &= ~D_JUMP;
    player->slip_flag &= ~N_JUMP;
    player->kartpitch = 0;
    player->kartroll = 0;
    toSqrt = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    player->old_speed = player->speed;
    player->speed = sqrtf(toSqrt);
    player->position[0] = nextX;
    player->position[2] = nextZ;
    player->position[1] = nextY;
    player->gravity_xz[0] = 0;
    player->gravity_xz[2] = 0;
    player->velocity[0] = newVelocity[0];
    player->velocity[1] = newVelocity[1];
    player->velocity[2] = newVelocity[2];
    old_velocity[playerId][0] = newVelocity[0];
    old_velocity[playerId][1] = newVelocity[1];
    old_velocity[playerId][2] = newVelocity[2];
    if (gKartTopSpeedTable[player->kart] < player->speed) {
        topSpeedMultiplier = gKartTopSpeedTable[player->kart] / player->speed;
        player->velocity[0] *= topSpeedMultiplier;
        player->velocity[1] *= topSpeedMultiplier;
        player->velocity[2] *= topSpeedMultiplier;
        player->speed = gKartTopSpeedTable[player->kart];
    }
}

void openkart(Player* player, UNUSED Camera* camera, UNUSED s8 screenId, s8 playerId) {
    Vec3f spF4 = { 0.0f, 0.0f, 1.0f };
    Vec3f spE8 = { 0.0f, 0.0f, 0.0f };
    UNUSED Vec3f spDC = { 0.0f, 0.0f, 0.0f };
    Vec3f spD0 = { 0.0f, 0.0f, 0.0f };
    // Next position is current position + current velocity
    f32 nextX;
    f32 nextY;
    f32 nextZ;

    f32 posX;
    f32 posY;
    f32 posZ;
    f32 topSpeedMultiplier;
    UNUSED s32 pad[11];
    f32 sp84;
    UNUSED s32 pad2;
    f32 sp7C;
    f32 surfaceDistance;
    f32 sqrt;
    Vec3f newVelocity;
    Vec3f sp5C;
    UNUSED s32 pad3[3];

    yoko_g(player, spD0);
    sp84 = player->gravity_xz[0] * 0;
    sp7C = player->gravity_xz[2] * 0;
    spF4[2] = PowerCheck(player, playerId);

    MultipleMatrixByVector(spF4, player->align);

    newVelocity[0] = player->velocity[0];
    newVelocity[1] = player->velocity[1];
    newVelocity[2] = player->velocity[2];

    newVelocity[0] += (((f64) (spF4[0] + sp84) - (newVelocity[0] * 780.0)) / 6500.0);
    newVelocity[2] += (((f64) (spF4[2] + sp7C) - (newVelocity[2] * 780.0)) / 6500.0);
    newVelocity[1] += (((f64) (spF4[1] + -1100.0f) - (newVelocity[1] * 780.0)) / 6500.0);

    posX = player->position[0];
    posY = player->position[1];
    posZ = player->position[2];

    player->old_position[0] = player->position[0];
    player->old_position[1] = player->position[1];
    player->old_position[2] = player->position[2];

    nextX = player->velocity[0] + posX;
    nextY = player->velocity[1] + posY;
    nextZ = player->velocity[2] + posZ;

    DriftJump(player);

    nextY += player->jump;
    custom_check_bump_2(&player->bump, player->radius, nextX, nextY, nextZ, player->old_position[0],
                            player->old_position[1], player->old_position[2]);
    player->up_vector[0] = 0.0f;
    player->up_vector[1] = 1.0f;
    player->up_vector[2] = 0.0f;
    MakeAlignMatrix(player->align, 0.0f, 1.0f, 0.0f, (s16) (s32) player->direction[1]);
    player->slip_flag &= ~N_JUMP;
    surfaceDistance = player->bump.distance_zx;
    if (surfaceDistance <= 0.0f) {
        if (1) {};
        CheckMapBG_ZX_Hook(player, sp5C, newVelocity, spE8, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    player->ground = CalcHeight(nextX, nextY, nextZ, player->bump.last_zx);
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((g_ScreenSplitA == SCREEN_MODE_1P) || (g_ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL) ||
         (g_ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        TirePosition(player, nextX, nextY, nextZ);
    } else {
        EnemyTirePosition(player, nextX, nextY, nextZ);
    }
    SetSlipAngle(player, playerId, posX, posZ, nextX, nextZ);
    sqrt = (newVelocity[0] * newVelocity[0]) + (newVelocity[1] * newVelocity[1]) + (newVelocity[2] * newVelocity[2]);
    player->old_speed = player->speed;
    player->speed = sqrtf(sqrt);
    if (((player->force <= 0.0f) && ((f64) player->speed < 0.13)) ||
        ((player->force <= 0.0f) && ((f64) player->speed < 0.2) &&
         ((player->slip_flag & BREAKE) == BREAKE))) {
        newVelocity[0] = newVelocity[0] + (newVelocity[0] * -1.0f);
        newVelocity[2] = newVelocity[2] + (newVelocity[2] * -1.0f);
    } else {
        player->position[0] = nextX;
        player->position[2] = nextZ;
    }
    player->position[1] = nextY - 0.018;

    player->gravity_xz[0] = spE8[0];
    player->gravity_xz[2] = spE8[2];

    player->velocity[0] = newVelocity[0];
    player->velocity[1] = newVelocity[1];
    player->velocity[2] = newVelocity[2];

    old_velocity[playerId][0] = newVelocity[0];
    old_velocity[playerId][1] = newVelocity[1];
    old_velocity[playerId][2] = newVelocity[2];

    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[player->kart] < player->speed) {
            topSpeedMultiplier = gKartTopSpeedTable[player->kart] / player->speed;
            player->velocity[0] *= topSpeedMultiplier;
            player->velocity[1] *= topSpeedMultiplier;
            player->velocity[2] *= topSpeedMultiplier;
            player->speed = gKartTopSpeedTable[player->kart];
        }
    }
}

void SlipCheck(Player* player, s8 playerIndex) {
    f32 var_f0;
    f32 var_f12;
    s32 temp_lo;
    s32 var_v1;

    var_f0 = 0.0f;
    if ((player->slip_flag & STAR) != STAR) {
        if ((s32) player->tire_RR.Status < 0xF) {
            var_f0 += D_800E2A90[player->kart][player->tire_RR.Status];
        }
        if ((s32) player->tire_RL.Status < 0xF) {
            var_f0 += D_800E2A90[player->kart][player->tire_RL.Status];
        }
        if ((s32) player->tire_FR.Status < 0xF) {
            var_f0 += D_800E2AB0[player->kart][player->tire_FR.Status];
        }
        if ((s32) player->tire_FL.Status < 0xF) {
            var_f0 += D_800E2AB0[player->kart][player->tire_FL.Status];
        }
    }
    if (sterr_counter[playerIndex] & 4) {
        var_v1 = 2;
    } else {
        var_v1 = 0;
    }
    if ((player->accelcount >= 200.0f) && (var_v1 == 2)) {
        temp_lo = (s16) player->slipang / DEGREES(1);
        if ((temp_lo > 0xF) || (temp_lo < -0xF)) {
            var_f0 += 1.0;
        }
    }
    if (((player->slip_flag & SLIP_STREAM) == SLIP_STREAM) && ((player->flag & IS_PLAYER) != IS_PLAYER)) {
        var_f0 = -3.0f;
    }

    if (player->r_grip >= 0.0f) {
        var_f12 = player->r_grip * var_f0;
    } else {
        var_f12 = -player->r_grip * var_f0;
    }
    player->allrear_grip = player->r_grip - var_f12;
}

void make_taco(Player* player, f32 arg1) {
    f32 temp_f0_3;
    f32 var_f0;
    s16 temp_lo;
    s32 test;

    if ((player->slip_flag & RAPID_ACC) == RAPID_ACC) {
        player->taco = ((player->accelcount * player->accelcount) / 25.0f) * 1.1;
        return;
    }

    // Huh?
    if (((player->slip_flag & ALL_EFFECTS) & N_JUMP) == N_JUMP) {
        player->taco = ((player->accelcount * player->accelcount) / 25.0f) * 1.1;
        return;
    }

    var_f0 = 0.0f;
    player->taco = arg1;
    temp_lo = player->kartpitch / DEGREES(1);
    if ((temp_lo > 0x11) || (temp_lo < -0x11)) {
        var_f0 += (temp_lo * 0.0125) / 1.2;
    } else {
        var_f0 += (temp_lo * 0.025) / 1.2;
    }
    player->taco = arg1 * (1.0f - var_f0);
    if (player->tire_RR.Status == GRASS) {
        var_f0 += D_800E2E90[player->kart][player->tire_RR.Status] * 0.7;
    }
    if (player->tire_RL.Status == GRASS) {
        // The unecessary "* 1.0" here is to force the compiler to save this 0.7 as a separate RO value from the 0.7
        // just above this comment
        var_f0 += D_800E2E90[player->kart][player->tire_RL.Status] * (0.7 * 1.0);
    }
    test = player->slipang / DEGREES(1);
    if (test < 0) {
        var_f0 += -test * 0.004;
    } else {
        var_f0 += test * 0.004;
    }
    player->taco = arg1 * (1.0 + (var_f0 * 0.7));
    if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
        temp_f0_3 = player->accelcount + 180.0f;
        player->taco = (temp_f0_3 * temp_f0_3) / 25.0f;
    }
}

f32 PowerCheck(Player* player, s8 playerIndex) {
    f32 var_f0;
    s16 temp_lo;
    f32 var_f2;
    s32 var_v0;

    var_f0 = 0.0f;
    var_f2 = (player->speed / 18.0f) * 216.0f;
    if (var_f2 >= 8.0f) {
        if ((player->slip_flag & STAR) != STAR) {
            if ((s32) player->tire_RR.Status >= 0xF) {
                // ???????
                if (1) {}
            } else {
                var_f0 += D_800E2E90[player->kart][player->tire_RR.Status];
            }

            if ((s32) player->tire_RL.Status < 0xF) {
                var_f0 += D_800E2E90[player->kart][player->tire_RL.Status];
            }

            if ((s32) player->tire_FR.Status < 0xF) {
                var_f0 += D_800E2EB0[player->kart][player->tire_FR.Status];
            }

            if ((s32) player->tire_FL.Status < 0xF) {
                var_f0 += D_800E2E90[player->kart][player->tire_FL.Status];
            }
        }
        if (((player->slip_flag & N_JUMP) != N_JUMP) &&
            ((player->jugemu_flag & ON_LAKITU_ROD) != ON_LAKITU_ROD)) {
            temp_lo = player->kartpitch / DEGREES(1);
            if (var_f2 >= 20.0f) {
                if ((temp_lo > 0x11) || (temp_lo < -0x11)) {
                    var_f0 -= ((temp_lo * 0.0126) / 3.0);
                } else {
                    var_f0 -= ((temp_lo * 0.026) / 3.0);
                }
            } else {
                var_f0 += -0.2;
                if ((player->slip_flag & THUNDER) == THUNDER) {
                    var_f0 += -0.55;
                }
            }
            if (((player->slip_flag & DRIFT) == DRIFT) || (player->drift_count > 0)) {
                var_v0 = (s16) player->slipang / DEGREES(1);
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * 0.004;
                } else {
                    var_f0 += var_v0 * 0.004;
                }
            } else {
                var_v0 = (s16) player->slipang / DEGREES(1);
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * (0.01 + gKartTurnSpeedReductionTable0[player->kart]);
                } else {
                    var_f0 += var_v0 * (0.01 + gKartTurnSpeedReductionTable0[player->kart]);
                }
            }
            if (((player->slip_flag & STOPSTERR) == STOPSTERR) && (player->stopsetrr_count < 10)) {
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * 0.008;
                } else {
                    var_f0 += var_v0 * 0.008;
                }
            }
            if ((player->slip_flag & STAR) == STAR) {
                var_f0 += -0.25;
            }
        }
        if ((player->water_flag & SUBMERGED) == SUBMERGED) {
            var_f0 += 0.3;
        } else {
            if ((player->water_flag & HALF_SUBMERGED) == HALF_SUBMERGED) {
                var_f0 += 0.15;
            }
            if (((g_waterlevelPlayer[playerIndex] - player->tire_RL.Height) >= 3.5) ||
                ((g_waterlevelPlayer[playerIndex] - player->tire_RR.Height) >= 3.5)) {
                var_f0 += 0.05;
            }
        }
        if ((player->slip_flag & N_JUMP) != 0) {
            chase_Fnumber(&player->power_down, player->force * 0.04, 0.15f);
        } else {
            chase_Fnumber(&player->power_down, 0.0f, 0.1f);
        }
    } else {
        player->power_down = 0.0f;
        player->maxadd_power = 0.0f;
        if (((s16) player->kartpitch / DEGREES(1)) < 0) {
            var_f0 += -0.85;
            if (player->slip_flag & THUNDER) {
                var_f0 += -0.55;
            }
        }
    }
    if ((player->flag & IS_PLAYER) != IS_PLAYER) {
        if ((player->slip_flag & SLIP_STREAM) == SLIP_STREAM) {
            chase_Fnumber(&player->maxadd_power, 380.0f, 0.5f);
        } else {
            chase_Fnumber(&player->maxadd_power, 0.0f, 0.1f);
        }
    }
    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
        if (((player->slip_flag & DRIFT_TURBO) == DRIFT_TURBO) &&
            ((player->slip_flag & DRIFT) != DRIFT)) {
            chase_Fnumber(&player->maxadd_power, 580.0f, 0.2f);
        } else {
            chase_Fnumber(&player->maxadd_power, 0.0f, 0.01f);
        }
        if ((player->slip_flag & SLIP_STREAM) == SLIP_STREAM) {
            chase_Fnumber(&player->slipstream_power, 580.0f, 0.01f);
        } else {
            chase_Fnumber(&player->slipstream_power, 0.0f, 0.01f);
        }
    }
    chase_Fnumber(&player->power_cont, var_f0, gKartTurnSpeedReductionTable1[player->kart] + 0.05);
    var_f2 = (player->force + player->maxadd_power + player->turbo_power + player->slipstream_power) - player->power_down;
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((player->jugemu_flag & LAKITU_SCENE) == LAKITU_SCENE) ||
        ((player->flag & NO_CONTROLS) == NO_CONTROLS)) {
        return (1.0f - player->power_cont) * var_f2;
    }
    if (((player->slip_flag & SPIN_L) == SPIN_L) ||
        ((player->slip_flag & SPIN_R) == SPIN_R) ||
        ((player->slip_flag & EXPLODE) == EXPLODE) ||
        ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE)) {
        return (1.0f - player->power_cont) * var_f2;
    }
    if (((player->slip_flag & TURBO) == TURBO) ||
        ((player->slip_flag & DASH_JUMP) == DASH_JUMP) ||
        ((player->slip_flag & DASH_MIDJUMP) == DASH_MIDJUMP)) {
        make_taco(player, player->turbo_power + player->force);
        return player->turbo_power + player->force;
    }
    make_taco(player, var_f2);
    return (1.0f - player->power_cont) * var_f2;
}

void shake_set(Player* player) {
    f32 var_f0;
    f32 var_f2;

    if (((player->jugemu_flag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
        ((player->jugemu_flag & LAKITU_SCENE) != LAKITU_SCENE)) {
        if ((((player->speed / 18.0f) * 216.0f) >= 8.0f) && (player->sus.bound_firstspeed < 1.0f)) {
            switch (player->bump_status) { /* irregular */
                case ASPHALT:
                    if (MakeRandomLimmit(0x000AU) != 8) {
                        var_f0 = 0.35f;
                        var_f2 = 0.55f;
                    } else {
                        player->shake_flag = 0;
                        player->shake = 0.0f;
                        var_f0 = 0.0f;
                        var_f2 = 0.0f;
                    }
                    break;
                case TRAIN_TRACK:
                case ROPE_BRIDGE:
                    var_f0 = 0.94f;
                    var_f2 = 0.85f;
                    break;
                default:
                    if (1) {}
                    var_f0 = 0.46f;
                    var_f2 = 0.48f;
                    break;
            }
        } else if (MakeRandomLimmit(0x000AU) != 8) {
            var_f0 = 0.3f;
            var_f2 = 0.54f;
        } else {
            player->shake_flag = 0;
            player->shake = 0.0f;
            var_f0 = 0.0f;
            var_f2 = 0.0f;
        }
        player->shake_flag += 1;
        player->shake = (player->shake_flag * var_f0) - (0.5 * var_f2 * (player->shake_flag * player->shake_flag));
        if ((player->shake_flag != 0) && (player->shake < 0.0f)) {
            player->shake_flag = 0;
        }
        if (player->shake <= 0.0f) {
            player->shake = 0.0f;
        }
    }
}

void check_accele_off(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }
    if (power_band_accele[playerIndex] == false) {
        if (accele_onoff_flag[playerIndex] == true) {
            if ((accele_time[playerIndex] < 2) || (accele_time[playerIndex] >= 9)) {
                accele_flag[playerIndex] = 0;
            }
            accele_time[playerIndex] = 0;
            accele_inc_flag[playerIndex] = 0;
        }
        accele_onoff_flag[playerIndex] = false;
        accele_time[playerIndex]++;
        if (accele_time[playerIndex] >= 9) {
            accele_time[playerIndex] = 9;
        }
        if ((accele_time[playerIndex] >= 2) && (accele_time[playerIndex] < 9)) {
            if (accele_inc_flag[playerIndex] == 0) {
                accele_flag[playerIndex] += 1;
            }
            accele_inc_flag[playerIndex] = 1;
        }
        if (accele_flag[playerIndex] == 5) {
            power_band_accele[playerIndex] = true;
            power_band_timer[playerIndex] = 120;
            accele_flag[playerIndex] = 0;
            accele_time[playerIndex] = 0;
        }
    } else {
        power_band_timer[playerIndex]--;
        if (power_band_timer[playerIndex] <= 0) {
            power_band_accele[playerIndex] = false;
        }
    }
}

void check_accele_on(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }
    if (power_band_accele[playerIndex] == false) {
        if (accele_onoff_flag[playerIndex] == false) {
            if ((accele_time[playerIndex] < 2) || (accele_time[playerIndex] >= 9)) {
                accele_flag[playerIndex] = 0;
            }
            accele_time[playerIndex] = 0;
            accele_inc_flag[playerIndex] = 0;
        }
        accele_onoff_flag[playerIndex] = true;
        accele_time[playerIndex]++;
        if (accele_time[playerIndex] >= 9) {
            accele_time[playerIndex] = 9;
        }
        if ((accele_time[playerIndex] >= 2) && (accele_time[playerIndex] < 9)) {
            if (accele_inc_flag[playerIndex] == 0) {
                accele_flag[playerIndex] += 1;
            }
            accele_inc_flag[playerIndex] = 1;
        }
        if (accele_flag[playerIndex] == 5) {
            power_band_accele[playerIndex] = true;
            power_band_timer[playerIndex] = 120;
            accele_flag[playerIndex] = 0;
            accele_time[playerIndex] = 0;
        }
    } else {
        power_band_timer[playerIndex]--;
        if (power_band_timer[playerIndex] <= 0) {
            power_band_accele[playerIndex] = false;
        }
    }
}

void AccelOn(Player* player) {
    s32 player_index;

    player_index = check_kart_number(player);
    if (power_band_accele[player_index] == false) {
        if ((0.0 <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.1))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][0] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.1) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.2))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][1] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.2) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.3))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][2] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.3) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.4))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][3] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.4) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.5))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][4] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.5) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.6))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][5] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.6) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.7))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][6] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.7) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.8))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][7] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.8) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.9))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][8] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
        if (((player->acc_maxcount * 0.9) <= player->accelcount) && (player->accelcount <= (player->acc_maxcount * 1.0))) {
            player->accelcount +=
                gKartAccelerationTables[player->kart][9] + (0.05 * (player->kartpitch / DEGREES(1)));
        }
    } else {
        if ((0.0 <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.1))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][0] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.1) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.2))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][1] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.2) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.3))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][2] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.3) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.4))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][3] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.4) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.5))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][4] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.5) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.6))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][5] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.6) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.7))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][6] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.7) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.8))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][7] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.8) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.9))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][8] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
        if (((player->acc_maxcount * 0.9) <= player->accelcount) && (player->accelcount <= (player->acc_maxcount * 1.0))) {
            player->accelcount +=
                (gKartAccelerationTables[player->kart][9] + (0.05 * (player->kartpitch / DEGREES(1)))) *
                gKartTripleABoost[player->kart];
        }
    }
    if (player->accelcount < 0.0f) {
        player->accelcount = 0.0f;
    }
    if (player->acc_maxcount <= player->accelcount) {
        player->accelcount = player->acc_maxcount;
    }
    if (!((player->slip_flag & N_JUMP)) || ((player->slip_flag & THUNDER))) {
        player->force = (player->accelcount * player->accelcount) / 25.0f;
    }
    player->handling_flag |= ACCELERATE;
    // Hacky way to check for PRESS_STARTSPIN
    if ((player->weapon * 8) < 0) {
        set_rapidacc(player, player_index);
        player->weapon &= ~PRESS_STARTSPIN;
    }
}

void AccelOff(Player* player, f32 speed) {
    s32 player_index;
    player_index = check_kart_number(player);

    player->accelcount -= speed;
    if (player->accelcount <= 0.0f) {
        player->accelcount = 0.0f;
    }
    if (player->speed < 0.2) {
        player->force = 0.0f;
    }
    if (player->acc_maxcount <= player->accelcount) {
        player->accelcount = player->acc_maxcount;
    }
    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        player->force = (player->accelcount * player->accelcount) / 25.0f;
    }
    player->handling_flag &= ~ACCELERATE;
    // Hacky way to check for PRESS_STARTSPIN
    if ((player->weapon * 8) < 0) {
        set_rapidacc(player, player_index);
        player->weapon &= ~PRESS_STARTSPIN;
    }
}

void check_brake_off(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }

    if (full_brake_accele[playerIndex] == false) {
        if (brake_onoff_flag[playerIndex] == true) {
            if ((brake_timer[playerIndex] < 2) || (brake_timer[playerIndex] >= 9)) {
                brake_flag[playerIndex] = 0;
            }
            brake_timer[playerIndex] = 0;
            brake_inc_flag[playerIndex] = 0;
        }
        brake_onoff_flag[playerIndex] = false;
        brake_timer[playerIndex]++;
        if (brake_timer[playerIndex] >= 9) {
            brake_timer[playerIndex] = 9;
        }
        if ((brake_timer[playerIndex] >= 2) && (brake_timer[playerIndex] < 9)) {
            if (brake_inc_flag[playerIndex] == 0) {
                brake_flag[playerIndex]++;
            }
            brake_inc_flag[playerIndex] = 1;
        }
        if (brake_flag[playerIndex] == 5) {
            full_brake_accele[playerIndex] = true;
            full_brake_timer[playerIndex] = 120;
            brake_flag[playerIndex] = 0;
            brake_timer[playerIndex] = 0;
        }
    } else {
        full_brake_timer[playerIndex]--;
        if (full_brake_timer[playerIndex] <= 0) {
            full_brake_accele[playerIndex] = false;
        }
    }
}

void check_brake_on(Player* player) {
    s32 playerIndex;

    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }

    if (full_brake_accele[playerIndex] == false) {
        if (brake_onoff_flag[playerIndex] == false) {
            if ((brake_timer[playerIndex] < 2) || (brake_timer[playerIndex] >= 9)) {
                brake_flag[playerIndex] = 0;
            }
            brake_timer[playerIndex] = 0;
            brake_inc_flag[playerIndex] = 0;
        }
        brake_onoff_flag[playerIndex] = true;
        brake_timer[playerIndex]++;
        if (brake_timer[playerIndex] >= 9) {
            brake_timer[playerIndex] = 9;
        }
        if ((brake_timer[playerIndex] >= 2) && (brake_timer[playerIndex] < 9)) {
            if (brake_inc_flag[playerIndex] == 0) {
                brake_flag[playerIndex]++;
            }
            brake_inc_flag[playerIndex] = 1;
        }
        if (brake_flag[playerIndex] == 5) {
            full_brake_accele[playerIndex] = true;
            full_brake_timer[playerIndex] = 120;
            brake_flag[playerIndex] = 0;
            brake_timer[playerIndex] = 0;
        }
    } else {
        full_brake_timer[playerIndex]--;
        if (full_brake_timer[playerIndex] <= 0) {
            full_brake_accele[playerIndex] = false;
        }
    }
}

void breake_on(Player* player) {
    s32 playerIndex;
    f32 test;
    f32 var_f2;

    var_f2 = 0.0f;
    if (player == gPlayerOne) {
        playerIndex = 0;
    }
    if (player == gPlayerTwo) {
        playerIndex = 1;
    }
    if (player == gPlayerThree) {
        playerIndex = 2;
    }
    if (player == gPlayerFour) {
        playerIndex = 3;
    }
    if (player == gPlayerFive) {
        playerIndex = 4;
    }
    if (player == gPlayerSix) {
        playerIndex = 5;
    }
    if (player == gPlayerSeven) {
        playerIndex = 6;
    }
    if (player == gPlayerEight) {
        playerIndex = 7;
    }
    player->slip_flag |= BREAKE;
    // This check will never be true, why is it here?
    if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
        AccelOff(player, 1.0f);
        player->brake_time = var_f2;
    } else {
        if ((s32) player->tire_RR.Status < 0xF) {
            var_f2 += D_800E3210[player->kart][player->tire_RR.Status];
        }
        if ((s32) player->tire_RL.Status < 0xF) {
            var_f2 += D_800E3210[player->kart][player->tire_RL.Status];
        }
        test = player->old_speed - player->speed;
        if (test <= 0.0f) {
            player->brake_time = 0.0f;
        } else {
            player->brake_time += 0.02;
            if (player->brake_time >= 2.0f) {
                player->brake_time = 2.0f;
            }
        }
        if (full_brake_accele[playerIndex] == true) {
            if (player->brake_time >= 2.0f) {
                AccelOff(player, (1.0f - var_f2) * 5.0f);
            } else {
                AccelOff(player, (1.0f - var_f2) * 3.0f);
            }
        } else {
            if (((player->speed / 18.0f) * 216.0f) <= 20.0f) {
                AccelOff(player, (1.0f - var_f2) * 4.0f);
            }
            if (player->brake_time >= 2.0f) {
                AccelOff(player, (1.0f - var_f2) * 2.5);
            } else {
                AccelOff(player, (1.0f - var_f2) * 1.2);
            }
        }
    }
}

void player_nocontaccele_on(Player* player) {
    s32 playerIndex;
    s32 var_v0;
    s32 time_delta;

    playerIndex = check_kart_number(player);
    if ((player->accelcount >= 0.0) && (player->accelcount < (player->acc_maxcount * 0.1))) {
        player->accelcount += gKartAccelerationTables[player->kart][0] * 3.0;
    }
    if (((player->acc_maxcount * 0.1) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.2))) {
        player->accelcount += gKartAccelerationTables[player->kart][1] * 3.0;
    }
    if (((player->acc_maxcount * 0.2) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.3))) {
        player->accelcount += gKartAccelerationTables[player->kart][2] * 3.0;
    }
    if (((player->acc_maxcount * 0.3) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.4))) {
        player->accelcount += gKartAccelerationTables[player->kart][3] * 3.0;
    }
    if (((player->acc_maxcount * 0.4) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.5))) {
        player->accelcount += gKartAccelerationTables[player->kart][4] * 3.0;
    }
    if (((player->acc_maxcount * 0.5) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.6))) {
        player->accelcount += gKartAccelerationTables[player->kart][5] * 3.0;
    }
    if (((player->acc_maxcount * 0.6) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.7))) {
        player->accelcount += gKartAccelerationTables[player->kart][6] * 2.5;
    }
    if (((player->acc_maxcount * 0.7) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.8))) {
        player->accelcount += gKartAccelerationTables[player->kart][7] * 2.5;
    }
    if (((player->acc_maxcount * 0.8) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.9))) {
        player->accelcount += gKartAccelerationTables[player->kart][8] * 2.5;
    }
    if (((player->acc_maxcount * 0.9) <= player->accelcount) && (player->accelcount <= player->acc_maxcount * 1.0)) {
        player->accelcount += gKartAccelerationTables[player->kart][9] * 2.5;
    }
    if (D_801656F0 == 1) {
        time_delta = kwvideoframetimer - green_timer[playerIndex];
        if (g_gameMode == TIME_TRIALS) {
            var_v0 = 0x14;
        } else {
            var_v0 = 8;
        }
        if ((time_delta < var_v0) && ((player->handling_flag & ACCELERATE) != ACCELERATE)) {
            player->weapon |= USE_STARTDASH;
        } else if ((player->acc_maxcount * 0.9f) <= player->accelcount) {
            if ((player->weapon & USE_STARTDASH) != USE_STARTDASH) {
                player->weapon |= PRESS_STARTSPIN;
                player->weapon &= ~USE_STARTDASH;
            }
        }
    }
    player->handling_flag |= ACCELERATE;
    player->taco = (player->accelcount * player->accelcount) / 25.0f;
}

void player_nocontaccele_off(Player* player, f32 speedReduction) {
    player->accelcount -= speedReduction;
    if (player->accelcount <= 0.0f) {
        player->accelcount = 0.0f;
    }
    if (player->speed < 0.2) {
        player->force = 0.0f;
    }
    if (player->acc_maxcount <= player->accelcount) {
        player->accelcount = player->acc_maxcount;
    }
    if ((f64) player->accelcount <= (player->acc_maxcount * 0.7)) {
        player->weapon &= ~PRESS_STARTSPIN;
    }
    player->weapon &= ~USE_STARTDASH;
    player->handling_flag &= ~ACCELERATE;
    player->taco = (player->accelcount * player->accelcount) / 25.0f;
}

void jugemu_accele_on(Player* player) {
    UNUSED s32 player_index;

    player_index = check_kart_number(player);
    if ((0.0 <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.1))) {
        player->accelcount += gKartAccelerationTables[player->kart][0] * 3.2;
    }
    if (((player->acc_maxcount * 0.1) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.2))) {
        player->accelcount += gKartAccelerationTables[player->kart][1] * 3.2;
    }
    if (((player->acc_maxcount * 0.2) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.3))) {
        player->accelcount += gKartAccelerationTables[player->kart][2] * 3.2;
    }
    if (((player->acc_maxcount * 0.3) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.4))) {
        player->accelcount += gKartAccelerationTables[player->kart][3] * 3.2;
    }
    if (((player->acc_maxcount * 0.4) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.5))) {
        player->accelcount += gKartAccelerationTables[player->kart][4] * 3.2;
    }
    if (((player->acc_maxcount * 0.5) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.6))) {
        player->accelcount += gKartAccelerationTables[player->kart][5] * 3.2;
    }
    if (((player->acc_maxcount * 0.6) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.7))) {
        player->accelcount += gKartAccelerationTables[player->kart][6] * 3.2;
    }
    if (((player->acc_maxcount * 0.7) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.8))) {
        player->accelcount += gKartAccelerationTables[player->kart][7] * 2.8;
    }
    if (((player->acc_maxcount * 0.8) <= player->accelcount) && (player->accelcount < (player->acc_maxcount * 0.9))) {
        player->accelcount += gKartAccelerationTables[player->kart][8] * 2.8;
    }
    if (((player->acc_maxcount * 0.9) <= player->accelcount) && (player->accelcount <= (player->acc_maxcount * 1.0))) {
        player->accelcount += gKartAccelerationTables[player->kart][9] * 2.8;
    }
    if (player->accelcount < 0.0f) {
        player->accelcount = 0.0f;
    }
    player->taco = (player->accelcount * player->accelcount) / 25.0f;
}

void jugemu_accele_off(Player* player, f32 speedReduction) {
    player->accelcount -= speedReduction;
    if (player->accelcount <= 0.0f) {
        player->accelcount = 0.0f;
    }
    if (player->acc_maxcount <= player->accelcount) {
        player->accelcount = player->acc_maxcount;
    }
    player->taco = (player->accelcount * player->accelcount) / 25.0f;
}

void empty_accele_on(Player* player, s32 playerIndex) {
    if ((accele_count[playerIndex] >= 0.0) && (accele_count[playerIndex] < ((f64) player->acc_maxcount * 0.1))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][0] * 3.2;
    }
    if (((player->acc_maxcount * 0.1) <= accele_count[playerIndex]) &&
        (accele_count[playerIndex] < (player->acc_maxcount * 0.2))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][1] * 3.2;
    }
    if (((player->acc_maxcount * 0.2) <= accele_count[playerIndex]) &&
        (accele_count[playerIndex] < (player->acc_maxcount * 0.3))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][2] * 3.2;
    }
    if (((player->acc_maxcount * 0.3) <= accele_count[playerIndex]) &&
        (accele_count[playerIndex] < (player->acc_maxcount * 0.4))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][3] * 3.2;
    }
    if (((player->acc_maxcount * 0.4) <= accele_count[playerIndex]) &&
        (accele_count[playerIndex] < (player->acc_maxcount * 0.5))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][4] * 3.2;
    }
    if (((player->acc_maxcount * 0.5) <= accele_count[playerIndex]) && (accele_count[playerIndex] < (player->acc_maxcount * 0.6))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][5] * 3.2;
    }
    if (((player->acc_maxcount * 0.6) <= accele_count[playerIndex]) && (accele_count[playerIndex] < (player->acc_maxcount * 0.7))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][6] * 3.2;
    }
    if (((player->acc_maxcount * 0.7) <= accele_count[playerIndex]) && (accele_count[playerIndex] < (player->acc_maxcount * 0.8))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][7] * 2.8;
    }
    if (((player->acc_maxcount * 0.8) <= accele_count[playerIndex]) && (accele_count[playerIndex] < (player->acc_maxcount * 0.9))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][8] * 2.8;
    }
    if (((player->acc_maxcount * 0.9) <= accele_count[playerIndex]) && (accele_count[playerIndex] <= (player->acc_maxcount * 1.0))) {
        accele_count[playerIndex] += gKartAccelerationTables[player->kart][9] * 2.8;
    }
    player->handling_flag |= ACCELERATE;
    if (accele_count[playerIndex] < 0.0f) {
        accele_count[playerIndex] = 0.0f;
    }
    player->taco = (accele_count[playerIndex] * accele_count[playerIndex]) / 25.0f;
}

void empty_accele_off(Player* player, f32 speedReduction, s32 playerIndex) {
    player->handling_flag &= ~ACCELERATE;
    accele_count[playerIndex] -= speedReduction;
    if (accele_count[playerIndex] <= 0.0f) {
        accele_count[playerIndex] = 0.0f;
    }
    if (player->acc_maxcount <= accele_count[playerIndex]) {
        accele_count[playerIndex] = player->acc_maxcount;
    }
    player->taco = (accele_count[playerIndex] * accele_count[playerIndex]) / 25.0f;
}

void recover_grip(Player* arg0, f32 arg1) {
    arg0->grip += arg1;
    if (arg0->grip >= 0.0f) {
        arg0->grip = 0.0f;
    }
}

void check_sterr_speedgrip(Player* player, s32* desired_steering_change, s32* current_steering, s32 desired_steering, s32 steering_change_threshold, s32 steering_change_minimum, s32 arg6) {
    s32 current_steering_change_increment;

    // if (abs(desired_steering_change) >= steering_change_threshold)
    if ((*desired_steering_change >= steering_change_threshold) || (-steering_change_threshold >= *desired_steering_change)) {
        current_steering_change_increment = player->chasesp_sterr;
        player->chasesp_sterr -= 1 << 11;
        //signed vs unsigned weirdness, actually checking if negative
        if (player->chasesp_sterr >= 0xF0000000) {
            player->chasesp_sterr = current_steering_change_increment;
        }
        if (steering_change_minimum >= (s32) player->chasesp_sterr) {
            player->chasesp_sterr = steering_change_minimum;
        }

        // move current_steering toward desired_steering
        *current_steering = (desired_steering < *current_steering) ? *current_steering - player->chasesp_sterr : *current_steering + player->chasesp_sterr;

        //arg6 is always positive and unk_090 is 0 or negative, so this if statement is always true
        if (player->grip < ((f32) arg6)) {
            player->grip = (f32) -arg6;
        }
    }
}

UNUSED void check_sterr_speedgrip1(Player* player, s32* arg1, s32 arg2, s32 arg3, f32 arg4) {
    u32 temp_v1;

    temp_v1 = player->chasesp_sterr;
    player->chasesp_sterr -= 0x800;
    if (!(player->chasesp_sterr < 0xF0000000)) {
        player->chasesp_sterr = temp_v1;
    }
    if (arg3 >= (s32) player->chasesp_sterr) {
        player->chasesp_sterr = arg3;
    }

    *arg1 = (arg2 < *arg1) ? *arg1 - player->chasesp_sterr : *arg1 + player->chasesp_sterr;

    if (player->grip < arg4) {
        player->grip = (f32) -arg4;
    }
}

UNUSED void check_sterr_speed1(Player* player, s32* arg1, s32 arg2, s32 arg3, f32 arg4) {
    s32 temp_v0;

    temp_v0 = player->chasesp_sterr;
    player->chasesp_sterr -= 0x800;
    if (player->chasesp_sterr >= 0xF0000000) {
        player->chasesp_sterr = temp_v0;
    }
    if (arg3 >= (s32) player->chasesp_sterr) {
        player->chasesp_sterr = arg3;
    }

    *arg1 = (arg2 < *arg1) ? *arg1 - player->chasesp_sterr : *arg1 + player->chasesp_sterr;

    recover_grip(player, arg4);
}

void check_sterr_speed(Player* player, s32* desired_steering_change, s32* current_steering, s32 desired_steering, s32 steering_change_threshold, s32 steering_change_minimum, f32 arg6) {
    s32 current_steering_change_increment;

    if ((*desired_steering_change >= steering_change_threshold) || (-steering_change_threshold >= *desired_steering_change)) {
        current_steering_change_increment = player->chasesp_sterr;
        player->chasesp_sterr -= 1 << 11;
        if (player->chasesp_sterr >= 0xF0000000) {
            player->chasesp_sterr = current_steering_change_increment;
        }
        if (steering_change_minimum >= (s32) player->chasesp_sterr) {
            player->chasesp_sterr = steering_change_minimum;
        }

        *current_steering = (desired_steering < *current_steering) ? *current_steering - player->chasesp_sterr : *current_steering + player->chasesp_sterr;
        // identical to check_sterr_speedgrip before this point

        // set player->grip = min(0, player->unk_90 + arg6)
        recover_grip(player, arg6);
    }
}

//On or near ground
void ProStickAngle(Player* player, struct Controller* controller, s8 playerIndex) {
    s32 steer_position;
    s32 clamped_x;
    UNUSED s32 pad[2];
    UNUSED s16 pad2;
    s16 var_s1_2;
    s32 steer_position_delta;
    s32 steer_resistance_small_turn;
    s32 steer_resistance_large_turn;
    f32 var_f2_2;
    f32 var_f12 = 0.0f;
    f32 zero = 0;
    UNUSED s32 pad3;
    s32 var_a0;
    f32 sp44[156] = { 0.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.6, 0.6, 0.6, 0.6, 0.6, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6,
                      0.5, 0.5, 0.5, 0.5, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.6, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.6, 0.6, 0.6, 0.6, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7, 0.7,
                      0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
                      0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8,
                      0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8, 0.8 };

    if ( 
         (
           ((player->slip_flag & D_JUMP) != D_JUMP) &&
           (
             ((player->slipang / DEGREES(1) <= 6) && (player->slipang / DEGREES(1) >= -6)) ||
             ((controller->ButtonHeld & R_TRIG) != R_TRIG)
           )
         ) ||
         (((player->speed / 18.0f) * 216.0f) <= 20.0f) ||
         ((player->slip_flag & CARHIT) == CARHIT)
       ) {
       drift_reset(player);
    }
    if ((player->slipang / DEGREES(1)) < (-5)) {
        player->handling_flag |= STEER_LEFT;
        player->handling_flag &= ~STEER_RIGHT;
        sterr_counter[playerIndex]++;
    } else if ((player->slipang / DEGREES(1)) > 5) {
        player->handling_flag |= STEER_RIGHT;
        player->handling_flag &= ~STEER_LEFT;
        sterr_counter[playerIndex]++;
    } else {
        player->handling_flag &= ~(STEER_LEFT | STEER_RIGHT);
        sterr_counter[playerIndex] = 0;
    }
    if (((player->slip_flag & D_JUMP) == D_JUMP) || ((player->slip_flag & DRIFT) == DRIFT)) {
        player->handling_flag &= ~(STEER_LEFT | STEER_RIGHT);
    }
    steer_position = player->nowsterr;
    clamped_x = check_stickX(controller);
    if (((player->handling_flag & REVERSE_GEAR) == REVERSE_GEAR) || ((player->handling_flag & REVERSE_ACCEL) == REVERSE_ACCEL)) {
        clamped_x = -clamped_x;
    }
    player->nowsterr = (clamped_x << 16) & 0xFFFF0000;
    steer_position_delta = steer_position - player->nowsterr; // x change
    steer_position_delta = steer_position_delta >> 16;
    player->sterracc = (s16) steer_position_delta;
    if (((steer_position_delta >= 90) || (steer_position_delta <= -90)) && (!(player->handling_flag & SPINOUT_SWERVE))) {
        if ((((((!(player->slip_flag & DRIFT)) && (g_raceClass == CC_150)) && (g_gameMode != BATTLE)) &&
              (!(player->slip_flag & N_JUMP))) &&
             (((player->speed / 18.0f) * 216.0f) >= 40.0f)) &&
            (player->drift_count == 0)) {
            player->weapon |= PRESS_SAVESPIN;
        }
    }
    if (((s32) player->tire_RR.Status) < 0xF) {
        //this table is just zeros
        zero += D_800E3610[player->kart][player->tire_RR.Status];
    }
    if (((s32) player->tire_RL.Status) < 0xF) {
        zero += D_800E3610[player->kart][player->tire_RL.Status];
    }
    if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
        steer_resistance_large_turn = 10;
        steer_resistance_small_turn = 10;
    } else {
        if (((player->slip_flag & DRIFT) == DRIFT) &&
            ((player->slip_flag & D_JUMP) != D_JUMP)) {
            var_a0 = 3;
        } else {
            var_a0 = 0;
        }
        if (((player->speed / 18.0f) * 216.0f) >= 15.0f) {
            if ((player->handling_flag & STEER_RIGHT) == STEER_RIGHT) {
                if ((steer_position_delta <= 35) && (steer_position_delta >= 0)) {
                    //gKartTable800E3650 is always 0
                    steer_resistance_large_turn =
                        (gKartTable800E3650[player->kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero)); //18 or 15
                    steer_resistance_small_turn =
                        (gKartTable800E3650[player->kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero));
                } else {
                    steer_resistance_large_turn = (s32) (((f32) (var_a0 + 5)) * (1.0f + zero)); // 8 or 5
                    steer_resistance_small_turn = (s32) (((f32) (var_a0 + 9)) * (1.0f + zero)); //12 or 9
                }
            } else if ((player->handling_flag & STEER_LEFT) == STEER_LEFT) {
                if ((steer_position_delta >= -35) && (steer_position_delta <= 0)) {
                    steer_resistance_large_turn =
                        (gKartTable800E3650[player->kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero)); // 18 or 15
                    steer_resistance_small_turn =
                        (gKartTable800E3650[player->kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero));
                } else {
                    steer_resistance_large_turn = (s32) (((f32) (var_a0 + 5)) * (1.0f + zero)); // 8 or 5
                    steer_resistance_small_turn = (s32) (((f32) (var_a0 + 9)) * (1.0f + zero)); // 12 or 9
                }
            } else {
                steer_resistance_large_turn = (s32) (((f32) (var_a0 + 3)) * (1.0f + zero)); // 6 or 3
                steer_resistance_small_turn = (s32) (((f32) (var_a0 + 6)) * (1.0f + zero)); // 6 or 9
            }
        } else {
            steer_resistance_large_turn = 8;
            steer_resistance_small_turn = 8;
        }
    }
    if ((player->water_flag & SUBMERGED) == SUBMERGED) {
        steer_resistance_large_turn *= 1.5;
        steer_resistance_small_turn *= 1.5;
    } else {
        if ((player->water_flag & HALF_SUBMERGED) == HALF_SUBMERGED) {
            steer_resistance_large_turn *= 1.2;
            steer_resistance_small_turn *= 1.2;
        }
        // Driving in water?
        if ((((f64) (g_waterlevelPlayer[playerIndex] - player->tire_RL.Height)) >= 3.5) ||
            (((f64) (g_waterlevelPlayer[playerIndex] - player->tire_RR.Height)) >= 3.5)) {
            steer_resistance_large_turn *= 1.05;
            steer_resistance_small_turn *= 1.05;
        }
    }

    /*
    This big group of function calls does 2 things. 
    First, when you steer it sets steer_position (which starts as just your x input) part way between your x input and your previous player->nowsterr
    (basically, you cannot go from full left to right steering instantaneously)
    Second, it sets the value of player->grip, which has to do with your karts sideways velocity. The code looks like it is intended
    to set this to larger negative values when you have a large steer_position_delta. However, due to an apparent bug, check_sterr_speedgrip
    overwrites this too frequently. The end effect is that player->grip ends up always getting set to -59.85 when steer_position_delta > 30
    and set to 0 otherwise. -59.85 is -100 (the last value provided to check_sterr_speedgrip) + the sum of the values provided to check_sterr_speed
    */
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 90, (120 << 12) / steer_resistance_large_turn, 450);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 89, (118 << 12) / steer_resistance_large_turn, 440);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 88, (116 << 12) / steer_resistance_large_turn, 430);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 87, (114 << 12) / steer_resistance_large_turn, 420);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 86, (112 << 12) / steer_resistance_large_turn, 410);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 85,  (88 << 12) / steer_resistance_large_turn, 400);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 84,  (86 << 12) / steer_resistance_large_turn, 395);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 83,  (80 << 12) / steer_resistance_large_turn, 390);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 82,  (79 << 12) / steer_resistance_large_turn, 390);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 81,  (78 << 12) / steer_resistance_large_turn, 380);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 80,  (77 << 12) / steer_resistance_large_turn, 370);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 79,  (76 << 12) / steer_resistance_large_turn, 360);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 78,  (76 << 12) / steer_resistance_large_turn, 360);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 77,  (75 << 12) / steer_resistance_large_turn, 350);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 76,  (74 << 12) / steer_resistance_large_turn, 340);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 75,  (73 << 12) / steer_resistance_large_turn, 330);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 74,  (73 << 12) / steer_resistance_large_turn, 330);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 73,  (73 << 12) / steer_resistance_large_turn, 330);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 72,  (72 << 12) / steer_resistance_large_turn, 320);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 71,  (71 << 12) / steer_resistance_large_turn, 315);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 70,  (71 << 12) / steer_resistance_large_turn, 315);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 69,  (70 << 12) / steer_resistance_large_turn, 305);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 68,  (70 << 12) / steer_resistance_large_turn, 305);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 67,  (69 << 12) / steer_resistance_large_turn, 280);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 66,  (70 << 12) / steer_resistance_large_turn, 270);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 65,  (69 << 12) / steer_resistance_large_turn, 270);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 64,  (68 << 12) / steer_resistance_large_turn, 260);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 63,  (67 << 12) / steer_resistance_large_turn, 250);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 62,  (67 << 12) / steer_resistance_large_turn, 250);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 61,  (67 << 12) / steer_resistance_large_turn, 250);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 60,  (61 << 12) / steer_resistance_large_turn, 245);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 59,  (60 << 12) / steer_resistance_large_turn, 245);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 58,  (59 << 12) / steer_resistance_large_turn, 245);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 57,  (58 << 12) / steer_resistance_large_turn, 245);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 56,  (56 << 12) / steer_resistance_large_turn, 245);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 55,  (56 << 12) / steer_resistance_large_turn, 230);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 54,  (56 << 12) / steer_resistance_large_turn, 230);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 53,  (56 << 12) / steer_resistance_large_turn, 230);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 52,  (56 << 12) / steer_resistance_large_turn, 230);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 51,  (56 << 12) / steer_resistance_large_turn, 230);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 50,  (50 << 12) / steer_resistance_large_turn, 220);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 49,  (50 << 12) / steer_resistance_large_turn, 220);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 48,  (50 << 12) / steer_resistance_large_turn, 220);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 47,  (50 << 12) / steer_resistance_large_turn, 220);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 46,  (50 << 12) / steer_resistance_large_turn, 220);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 45,  (48 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 44,  (46 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 43,  (46 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 42,  (46 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 41,  (46 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 40,  (46 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 39,  (44 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 38,  (40 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 37,  (40 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 36,  (36 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 35,  (36 << 12) / steer_resistance_large_turn, 110);
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 34,  (34 << 12) / steer_resistance_large_turn, 110); 
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 32,  (32 << 12) / steer_resistance_large_turn, 100); // this skips 33
    check_sterr_speedgrip(player, &steer_position_delta, &steer_position, player->nowsterr, 31,  (32 << 12) / steer_resistance_large_turn, 100);

    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 30,  (31 << 12) / steer_resistance_small_turn, 0.9f); // 0.9 * 5
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 29,  (30 << 12) / steer_resistance_small_turn, 0.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 28,  (29 << 12) / steer_resistance_small_turn, 0.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 27,  (28 << 12) / steer_resistance_small_turn, 0.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 26,  (27 << 12) / steer_resistance_small_turn, 0.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 25,  (26 << 12) / steer_resistance_small_turn, 1.0f); // 1.0 * 5
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 24,  (25 << 12) / steer_resistance_small_turn, 1.0f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 23,  (24 << 12) / steer_resistance_small_turn, 1.0f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 22,  (23 << 12) / steer_resistance_small_turn, 1.0f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 21,  (22 << 12) / steer_resistance_small_turn, 1.0f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 20,  (21 << 12) / steer_resistance_small_turn, 1.05f);// 1.05* 5
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 19,  (20 << 12) / steer_resistance_small_turn, 1.05f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 18,  (19 << 12) / steer_resistance_small_turn, 1.05f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 17,  (18 << 12) / steer_resistance_small_turn, 1.05f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 16,  (17 << 12) / steer_resistance_small_turn, 1.05f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 15,  (16 << 12) / steer_resistance_small_turn, 1.2f); // 1.2 * 5
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 14,  (15 << 12) / steer_resistance_small_turn, 1.2f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 13,  (14 << 12) / steer_resistance_small_turn, 1.2f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 12,  (13 << 12) / steer_resistance_small_turn, 1.2f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 11,  (12 << 12) / steer_resistance_small_turn, 1.2f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr, 10,  (14 << 12) / steer_resistance_small_turn, 1.6f); // 1.6 * 5
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  9,  (13 << 12) / steer_resistance_small_turn, 1.6f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  8,  (12 << 12) / steer_resistance_small_turn, 1.6f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  7,  (11 << 12) / steer_resistance_small_turn, 1.6f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  6,  (10 << 12) / steer_resistance_small_turn, 1.6f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  5,   (9 << 12) / steer_resistance_small_turn, 1.9f); // 1.9 * 6
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  4,   (8 << 12) / steer_resistance_small_turn, 1.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  3,   (7 << 12) / steer_resistance_small_turn, 1.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  2,   (6 << 12) / steer_resistance_small_turn, 1.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  1,   (5 << 12) / steer_resistance_small_turn, 1.9f);
    check_sterr_speed(player, &steer_position_delta, &steer_position, player->nowsterr,  0,           0 / steer_resistance_small_turn, 1.9f); // sum is 40.15
    if ((player->slip_flag & DRIFT) == DRIFT) {
        var_f2_2 = (f32) ((steer_position >> 16) / 8);
    } else if (((player->speed / 18.0f) * 216.0f) <= 25.0f) {
        var_f2_2 = (f32) ((steer_position >> 16) / 12);
    } else {
        var_f2_2 = ((f32) (steer_position >> 16)) / (8.0f + (player->accelcount / 50.0f));
    }
    if (var_f2_2 < 0.0f) {
        var_f2_2 = -var_f2_2;
    }
    if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
        var_f2_2 = var_f2_2 * (sp44[((s16) ((player->speed / 18.0f) * 216.0f)) + 10] * 1.5f);
    } else if ((player->slip_flag & DRIFT) == DRIFT) {
        var_f2_2 = var_f2_2 * sp44[(s16) ((player->speed / 18.0f) * 216.0f)];
    } else {
        var_f2_2 = var_f2_2 * (sp44[(s16) ((player->speed / 18.0f) * 216.0f)] * 1.5f);
    }
    player->nowsterr = steer_position;
    if (player->hitcount != 0) {
        check_impactsterr(player);
    }
    player->slip_flag &= ~STOPSTERR;
    if (((s32) player->tire_RR.Status) > 0xE) {
        var_f12 = var_f12;
    } else {
        var_f12 += D_800E3410[player->kart][player->tire_RR.Status];
    }
    if (((s32) player->tire_RL.Status) < 0xF) {
        var_f12 += D_800E3410[player->kart][player->tire_RL.Status];
    }
    if (((player->slip_flag & D_JUMP) != D_JUMP) && ((player->slip_flag & DRIFT) != DRIFT)) {
        if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
            player->sterrangle = (player->nowsterr >> 16) * 5 * var_f2_2;
        } else {
            if ((player->slip_flag & BREAKE) != BREAKE) {
                if (((player->nowsterr >> 16) >= 45) || ((player->nowsterr >> 16) <= -45)) {
                    player->sterrangle = ((player->nowsterr >> 16) * (var_f2_2 + (var_f2_2 * var_f12))) *
                                      (0.15 + gKartHandlingTable[player->kart]);
                } else {
                    player->sterrangle = ((player->nowsterr >> 16) * (var_f2_2 + (var_f2_2 * var_f12))) *
                                      gKartHandlingTable[player->kart];
                }
            } else {
                if ((((player->speed / 18.0f) * 216.0f) >= 0.0f) && (((player->speed / 18.0f) * 216.0f) < 8.0f)) {
                    player->sterrangle = (player->nowsterr >> 16) * (var_f2_2 + (var_f2_2 * var_f12));
                }
                if ((((player->speed / 18.0f) * 216.0f) >= 8.0f) && (((player->speed / 18.0f) * 216.0f) < 65.0f)) {
                    player->sterrangle = (player->nowsterr >> 16) * ((var_f2_2 + 1.5) + (var_f2_2 * var_f12));
                }
                if (((player->speed / 18.0f) * 216.0f) >= 65.0f) {
                    player->sterrangle = (player->nowsterr >> 16) * ((var_f2_2 + 1.6) + (var_f2_2 * var_f12));
                }
            }
            player->stopsetrr_count = 0;
            if (player->powerstack_count < 2) {
                player->powerstack_count = 0;
            }
        }
    } else if (((player->slip_flag & N_JUMP) != N_JUMP) && ((player->slip_flag & D_JUMP) != D_JUMP)) {
        if ((((s16) player->slipang) / DEGREES(1)) > 0) {
            // linear map, sets -53 to 40 and 53 to 53
            var_s1_2 = (((s32) (((player->nowsterr >> 16) * 13) + (13 * 53))) / (2 * 53)) + 40;
            if ((player->nowsterr >> 16) <= -40) {
                player->slip_flag |= STOPSTERR;
                if ((player->nowsterr >> 16) <= -50) { // always true, but has no effect
                    player->slip_flag |= STOPSTERR;
                }
            }
            check_drift_turbo(player, playerIndex);
        } else {
            // linear map, sets -53 to -53 and 53 to -40
            var_s1_2 = (((s32) (((player->nowsterr >> 16) * 13) + (13 * 53))) / (2 * 53)) - 53;
            if ((player->nowsterr >> 16) >= 40) {
                player->slip_flag |= STOPSTERR;
                if ((player->nowsterr >> 16) <= -50) { // never true, but would have no effect
                    player->slip_flag |= STOPSTERR;
                }
            }
            check_drift_turbo(player, playerIndex);
        }
        if ((((player->speed / 18.0f) * 216.0f) >= 0.0f) && (((player->speed / 18.0f) * 216.0f) < 8.0f)) {
            player->sterrangle = (s16) ((s32) (var_s1_2 * ((var_f2_2 + 2.0f) + (var_f2_2 * var_f12))));
        }
        if ((((player->speed / 18.0f) * 216.0f) >= 8.0f) && (((player->speed / 18.0f) * 216.0f) < 65.0f)) {
            player->sterrangle = var_s1_2 * ((var_f2_2 + 3) + (var_f2_2 * var_f12));
        }
        if (((player->speed / 18.0f) * 216.0f) >= 65.0f) {
            player->sterrangle = var_s1_2 * ((((f64) var_f2_2) + 3.5) + (var_f2_2 * var_f12));
        }
        if ((player->slip_flag & STOPSTERR) == STOPSTERR) {
            player->sterrangle *= 0.9;
        } else {
            player->sterrangle *= 0.65;
        }
    } else {
        var_s1_2 = player->nowsterr >> 16;
        if (clamped_x == 0) {
            var_s1_2 = 0;
        }
        if (((player->speed / 18.0f) * 216.0f) <= 5.0f) {
            player->sterrangle = (s16) ((s32) (((f32) var_s1_2) * (var_f2_2 + 6.0f)));
        } else {
            player->sterrangle = ((s16) var_s1_2) * (var_f2_2 + 1.5f);
        }
    }
    if (g_gameMode == BATTLE) {
        player->sterrangle *= 1.7;
    }
}

void enemy_stick_angle(Player* player, s16 targetAngle) {
    s32 sp304 = 0;
    UNUSED f32 pad[6];
    f32 var_f0;
    s16 var_v0;
    f32 speedTurn[168] = {
        0.0f, 0.1f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.7f, 0.7f, 0.7f, 0.7f, 0.7f,
        0.7f, 0.7f, 0.6f, 0.5f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f,
        0.4f, 0.4f, 0.5f, 0.5f, 0.5f, 0.5f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.7f,
        0.7f, 0.7f, 0.7f, 0.7f, 0.7f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
        0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f, 0.6f,
    };
    f32 characterTurn[8] = {
        3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f,
    };

    if (!((player->slip_flag & SPIN_L) || (player->slip_flag & SPIN_R) ||
          (player->slip_flag & ROLLOVER) || (player->slip_flag & ROLLOVER_FALL) ||
          (player->slip_flag & THUNDER_SPIN) || (player->slip_flag & EXPLODE) ||
          (player->slip_flag & THROW_EXPLODE) || (player->slip_flag & BROKEN))) {
        if (!(((player->speed / 18.0f) * 216.0f) >= 110.0f)) {
            player->slip_flag &= ~STOPSTERR;
            player->stopsetrr_count = 0;
            if (!(player->slip_flag & SPIN_L) && !(player->slip_flag & SPIN_R)) {
                sp304 = (s32) player->nowsterr >> 16;
                chase_Inumber(&sp304, (s32) targetAngle, 0.35f);
                sp304 <<= 0x10;
                if ((player->slip_flag & DRIFT) == DRIFT) {
                    var_f0 = (sp304 >> 0x10) / 5;
                } else {
                    var_f0 = (f32) (sp304 >> 0x10) / (8.0f + (player->accelcount / 50.0f));
                }
                if (var_f0 < 0.0f) {
                    var_f0 = -var_f0;
                }

                // Apply speed and character multipliers
                if ((player->slip_flag & DRIFT) == DRIFT) {
                    var_f0 = speedTurn[(s16) ((player->speed / 18.0f) * 216.0f)] * var_f0;
                } else {
                    var_f0 = speedTurn[(s16) ((player->speed / 18.0f) * 216.0f)] * characterTurn[player->kart] *
                             var_f0;
                }
                player->nowsterr = sp304;
                if (((player->slip_flag & D_JUMP) != D_JUMP) &&
                    ((player->slip_flag & DRIFT) != DRIFT)) {
                    if ((player->slip_flag & BREAKE) != BREAKE) {
                        player->sterrangle = (player->nowsterr >> 16) * var_f0;
                    } else {
                        player->sterrangle = (player->nowsterr >> 16) * (var_f0 + 1.5);
                    }
                } else if ((player->slip_flag & N_JUMP) != N_JUMP) {
                    if (((s16) player->slipang / DEGREES(1)) > 0) {
                        var_v0 = player->nowsterr >> 16;
                    } else {
                        var_v0 = player->nowsterr >> 16;
                    }
                    player->sterrangle = var_v0 * (var_f0 + 3.0);
                    player->sterrangle *= 0.8;
                } else {
                    var_v0 = (s16) ((s32) player->nowsterr >> 16);
                    if (targetAngle == 0) {
                        var_v0 = 0;
                    }
                    player->sterrangle = var_v0 * var_f0;
                }
                if ((((player->slip_flag & D_JUMP) != D_JUMP) && (player->slipang <= 60) && (player->slipang >= -60)) ||
                    (((player->speed / 18.0f) * 216.0f) <= 20.0f) ||
                    ((player->slip_flag & CARHIT) == CARHIT)) {
                    drift_reset(player);
                }
            }
        }
    }
}

void drift_set(Player* player) {
    if (((player->speed / 18.0f) * 216.0f) > 20.0f) {
        player->drift_count = 0;
        player->slip_flag |= DRIFT;
        player->talk |= BOING;
    }
}

void drift_reset(Player* player) {
    s32 steer_position_new;

    if (((player->slip_flag & DRIFT) == DRIFT) && ((player->flag & IS_PLAYER) == IS_PLAYER)) {
        if ((player->slipang / DEGREES(1)) > 0) {
            // linear map, sets -53 to 40 and 53 to 53
            steer_position_new = ((((player->nowsterr >> 16) * 13) + (13*53)) / (2*53)) + 40;
            player->nowsterr = steer_position_new << 16;
        }
        if ((player->slipang / DEGREES(1)) < 0) {
            // linear map, sets -53 to -53 and 53 to -40
            steer_position_new = ((((player->nowsterr >> 16) * 13) + (13*53)) / (2*53)) - 53;
            player->nowsterr = steer_position_new << 16;
        }
        player->slip_flag &= ~DRIFT;
    }
    if (((player->slip_flag & DRIFT) == DRIFT) && ((player->flag & IS_PLAYER) != IS_PLAYER)) {
        player->slip_flag &= ~DRIFT;
    }
}

void setvector_l(Player* player, Vec3f arg1, Vec3f arg2) {
    s16 steer_position_delta;
    UNUSED s16 pad;
    f32 sp20;
    f32 var_f18;
    s32 steer_position;

    if (((player->slip_flag & CENTER_LINE) == CENTER_LINE) ||
        ((player->slip_flag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, player->align);
    } else {
        if (((player->slip_flag & DRIFT) == DRIFT) &&
            ((player->slip_flag & D_JUMP) != D_JUMP)) {
            var_f18 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->brake_time * 10.0f);
            sp20 = player->f_grip * 3.0f;
        } else if (!(player->slip_flag & WHEELSPIN) && !(player->handling_flag & SPINOUT_SWERVE)) {
            steer_position_delta = player->sterracc;
            if (steer_position_delta > 0) {
                steer_position_delta *= -1;
            }
            steer_position = player->nowsterr >> 16;
            if ((steer_position <= 20) && (steer_position >= -20)) {
                if (steer_position_delta < 20) {
                    var_f18 = (player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                              (-player->accelcount * 0.02) + (-player->brake_time * 50.0f);
                } else {
                    var_f18 = (player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                              ((steer_position * 0.01) + (-player->accelcount * 0.05)) + (-player->brake_time * 50.0f);
                }
            } else {
                var_f18 = (player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                          ((steer_position * 0.1) + (-player->accelcount * 0.15)) + (-player->brake_time * 50.0f);
            }
            sp20 = player->f_grip;
        } else {
            var_f18 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) * 1.5) +
                      (((player->nowsterr >> 16) * 0.1) + (-player->accelcount * 0.05)) + (-player->brake_time * 50.0f);
            sp20 = player->f_grip;
        }
        if ((player->slip_flag & STAR) == STAR) {
            if (((player->slip_flag & DRIFT) == DRIFT) &&
                ((player->slip_flag & D_JUMP) != D_JUMP)) {
                var_f18 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->brake_time * 10.0f);
                sp20 = player->f_grip * 3.0f;
            } else {
                var_f18 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f);
                sp20 = player->f_grip;
            }
        }
        arg1[0] = (player->grip + var_f18) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * sp20;
        MultipleMatrixByVector(arg1, player->align);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void setvector_r(Player* player, Vec3f arg1, Vec3f arg2) {
    s16 steer_position_delta;
    f32 sp20;
    f32 var_f18;
    s32 steer_position;

    if (((player->slip_flag & CENTER_LINE) == CENTER_LINE) ||
        ((player->slip_flag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, player->align);
    } else {
        if (((player->slip_flag & DRIFT) == DRIFT) &&
            ((player->slip_flag & D_JUMP) != D_JUMP)) {
            var_f18 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->brake_time * 50.0f);
            sp20 = player->f_grip * 3.0f;
        } else if (!(player->slip_flag & WHEELSPIN) && !(player->handling_flag & SPINOUT_SWERVE)) {
            steer_position_delta = player->sterracc;
            if (steer_position_delta > 0) {
                steer_position_delta *= -1;
            }
            steer_position = (s32) player->nowsterr >> 16;
            if ((steer_position <= 20) && (steer_position >= -20)) {
                if (steer_position_delta < 20) {
                    var_f18 = (player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) +
                              (-player->accelcount * 0.02) + (-player->brake_time * 50.0f);
                } else {
                    var_f18 = ((player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) -
                               ((steer_position * 0.01) + (player->accelcount * 0.05))) +
                              (-player->brake_time * 50.0f);
                }
            } else {
                var_f18 = ((player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f)) -
                           ((steer_position * 0.1) + (player->accelcount * 0.15))) +
                          (-player->brake_time * 50.0f);
            }
            sp20 = player->f_grip;
        } else {
            var_f18 = ((player->allrear_grip + ((f64) (-(player->speed / 18.0f) * 216.0f) * 1.5)) -
                       (((player->nowsterr >> 0x10) * 0.1) + (player->accelcount * 0.05))) +
                      (-player->brake_time * 50.0f);
            sp20 = player->f_grip;
        }
        if ((player->slip_flag & STAR) == STAR) {
            if (((player->slip_flag & DRIFT) == DRIFT) &&
                ((player->slip_flag & D_JUMP) != D_JUMP)) {
                var_f18 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) * 3.0f) + (-player->brake_time * 50.0f);
                sp20 = player->f_grip * 3.0f;
            } else {
                var_f18 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 3.0f);
                sp20 = player->f_grip;
            }
        }
        arg1[0] = -(player->grip + var_f18) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * sp20;
        MultipleMatrixByVector(arg1, player->align);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void enemy_setvector_l(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if (((player->slip_flag & CENTER_LINE) == CENTER_LINE) ||
        ((player->slip_flag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, player->align);
    } else {
        if (((player->slip_flag & DRIFT) == DRIFT) &&
            ((player->slip_flag & D_JUMP) != D_JUMP)) {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) * 2) + -80.0f;
            var_f12 = -80.0f;
        } else {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) / 2) + -20.0f;
            var_f12 = -40.0f;
        }
        arg1[0] = (var_f2 + 28.0f) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = var_f12 * player->speed;
        MultipleMatrixByVector(arg1, player->align);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}


void enemy_setvector_r(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if (((player->slip_flag & CENTER_LINE) == CENTER_LINE) ||
        ((player->slip_flag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, player->align);
    } else {
        if (((player->slip_flag & DRIFT) == DRIFT) &&
            ((player->slip_flag & D_JUMP) != D_JUMP)) {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) * 2) + -80.0f;
            var_f12 = -80.0f;
        } else {
            var_f2 = ((-(player->speed / 18.0f) * 216.0f) / 2) + -20.0f;
            var_f12 = -40.0f;
        }
        arg1[0] = -(var_f2 + 28.0f) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = var_f12 * player->speed;
        MultipleMatrixByVector(arg1, player->align);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void battle_setvector_l(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, player->align);
    } else {
        if ((player->slip_flag & DRIFT) == DRIFT) {
            var_f2 = player->allrear_grip + (-(player->speed / 18.0f) * 216.0f * 5.0f) + (-player->brake_time * 10.0f);
            var_f12 = -100.0f;
        } else {
            var_f2 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 40.0f) + (-player->brake_time * 50.0f);
            var_f12 = player->f_grip;
        }
        arg1[0] = (player->grip + var_f2) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * var_f12;
        MultipleMatrixByVector(arg1, player->align);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void battle_setvector_r(Player* player, Vec3f arg1, Vec3f arg2) {
    f32 var_f12;
    f32 var_f2;

    if ((player->slip_flag & SPIN_TURN) == SPIN_TURN) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, player->align);
    } else {
        if ((player->slip_flag & DRIFT) == DRIFT) {
            var_f2 = player->allrear_grip + (-(player->speed / 18.0f) * 216.0f * 5.0f) + (-player->brake_time * 50.0f);
            var_f12 = -100.0f;
        } else {
            var_f2 = player->allrear_grip + ((-(player->speed / 18.0f) * 216.0f) / 40.0f) + (-player->brake_time * 50.0f);
            var_f12 = player->f_grip;
        }
        arg1[0] = -(player->grip + var_f2) * player->speed;
        arg1[1] = 0.0f;
        arg1[2] = player->speed * var_f12;
        MultipleMatrixByVector(arg1, player->align);
    }
    arg2[0] = arg1[0];
    arg2[1] = arg1[1];
    arg2[2] = arg1[2];
}

void yoko_g(Player* player, Vec3f arg1) {
    UNUSED s32 pad[3];
    Vec3f sp20;

    if (player->sterrangle == 0) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
    } else {
        if (player->sterrangle < 0) {
            if (((player->slip_flag & STOPSTERR) != STOPSTERR) || (player->stopsetrr_count >= 100)) {
                player->direction[1] += player->sterrangle;
            }
            if (!(player->flag & IS_CPU_PLAYER)) {
                if (g_gameMode == BATTLE) {
                    battle_setvector_l(player, sp20, arg1);
                } else {
                    setvector_l(player, sp20, arg1);
                }
            } else {
                enemy_setvector_l(player, sp20, arg1);
            }
        } else {
            if (((player->slip_flag & STOPSTERR) != STOPSTERR) || (player->stopsetrr_count >= 100)) {
                player->direction[1] += player->sterrangle;
            }
            if (!(player->flag & IS_CPU_PLAYER)) {
                if (g_gameMode == BATTLE) {
                    battle_setvector_r(player, sp20, arg1);
                } else {
                    setvector_r(player, sp20, arg1);
                }
            } else {
                enemy_setvector_r(player, sp20, arg1);
            }
        }
    }
}

void manual_stick_drive(Player* player, struct Controller* controller, s8 playerIndex) {
    if (((player->slip_flag & SPIN_L) != SPIN_L) &&
        ((player->slip_flag & SPIN_R) != SPIN_R) &&
        ((player->slip_flag & ROLLOVER) != ROLLOVER) &&
        ((player->slip_flag & RAPID_ACC) != RAPID_ACC) &&
        ((player->slip_flag & EXPLODE) != EXPLODE) &&
        ((player->slip_flag & THROW_EXPLODE) != THROW_EXPLODE) &&
        ((player->slip_flag & ROLLOVER_FALL) != ROLLOVER_FALL) &&
        ((player->slip_flag & THUNDER_SPIN) != THUNDER_SPIN)) {
        if (((player->slip_flag & BROKEN) != BROKEN) &&
            ((player->slip_flag & N_JUMP) != N_JUMP) && ((player->slip_flag & D_JUMP) != D_JUMP) &&
            ((player->slip_flag & DRIFT) != DRIFT) && (controller->ButtonPressed & R_TRIG)) {
            jump_set(player);
            if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
                ((player->flag & IS_GHOST) != IS_GHOST)) {
                NAPlyTrgStart(playerIndex, 0x19008000);
            }
        }
        ProStickAngleHook(player, controller, playerIndex);
        player->slip_flag &= ~BREAKE;
        if ((!(player->slip_flag & DASH_JUMP)) && (!(player->slip_flag & DASH_MIDJUMP))) {
            if (((player->speed / 18.0f) * 216.0f) <= 12.0f) {
                if (controller->ButtonHeld & A_BUTTON) {
                    if (controller->ButtonHeld & B_BUTTON) {
                        player->slip_flag |= SPIN_TURN;
                        if ((player->slip_flag & SPIN_TURN) != SPIN_TURN) {
                            player->accelcount += 100.0f;
                        }
                    }
                }
            }
            if (((player->slip_flag & SPIN_TURN) == SPIN_TURN) &&
                (((controller->ButtonHeld & B_BUTTON) == 0) || (!(controller->ButtonHeld & A_BUTTON)))) {
                player->slip_flag &= ~SPIN_TURN;
            }
        }
        if ((player->handling_flag & REVERSE_GEAR) != REVERSE_GEAR) {
            if (controller->ButtonHeld & A_BUTTON) {
                AccelOn(player);
                check_accele_on(player);
            } else {
                if (g_gameMode == BATTLE) {
                    AccelOff(player, 2.0f);
                } else {
                    AccelOff(player, 1.0f);
                }
                check_accele_off(player);
            }
            if (controller->ButtonHeld & B_BUTTON) {
                breake_on(player);
                check_brake_on(player);
            } else {
                player->brake_time = 0.0f;
                check_brake_off(player);
            }
        }
        if ((!(player->slip_flag & DASH_JUMP)) && (!(player->slip_flag & DASH_MIDJUMP))) {
            if (((check_stickY(controller) < (-0x31)) && (((player->speed / 18.0f) * 216.0f) <= 5.0f)) &&
                (controller->ButtonHeld & B_BUTTON)) {
                player->accelcount = 140.0f;
                player->handling_flag |= REVERSE_GEAR;
                player->force = (player->accelcount * player->accelcount) / 25.0f;
                player->brake_time = 0.0f;
            }
            if ((check_stickY(controller) >= -0x1D) || (!(controller->ButtonHeld & B_BUTTON))) {
                if ((player->handling_flag & REVERSE_GEAR) == REVERSE_GEAR) {
                    player->handling_flag &= ~(REVERSE_GEAR);
                    player->accelcount = 0.0f;
                }
            }
        }
    } else {
        if ((player->slip_flag & RAPID_ACC) == RAPID_ACC) {
            if (controller->ButtonHeld & A_BUTTON) {
                AccelOn(player);
            } else {
                AccelOff(player, 5.0f);
            }
        }
        if (((((player->slip_flag & SPIN_L) == SPIN_L) ||
              ((player->slip_flag & SPIN_R) == SPIN_R)) ||
             ((player->slip_flag & EXPLODE) == EXPLODE)) ||
            ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE)) {
            if (controller->ButtonHeld & A_BUTTON) {
                check_accele_on(player);
                empty_accele_on(player, playerIndex);
                return;
            }
            check_accele_off(player);
            empty_accele_off(player, 5.0f, playerIndex);
        }
    }
}

void drive_stick_kindcontrol(Player* player, struct Controller* controller, s8 playerIndex) {
    if (((player->flag & EXISTS) == EXISTS) && ((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
        // If not start sequence
        if ((player->flag & NO_CONTROLS) != NO_CONTROLS) {
            if (((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
                ((player->jugemu_flag & LAKITU_SCENE) == LAKITU_SCENE)) {
                if (controller->ButtonHeld & A_BUTTON) {
                    jugemu_accele_on(player);
                } else {
                    jugemu_accele_off(player, 5.0f);
                }
            } else {
                manual_stick_drive(player, controller, playerIndex);
            }
            D_80164A89 = 1;
            // If start sequence and unknown condition
        } else if (D_8018D168 == 1) {
            if (D_801656F0 == 1) {
                if (green_timer[playerIndex] == 0) {
                    green_timer[playerIndex] = kwvideoframetimer;
                }
            }
            if (controller->ButtonHeld & A_BUTTON) {
                player_nocontaccele_on(player);
            } else {
                player_nocontaccele_off(player, 5.0f);
            }
        }
    }
}

void drive_stick_control(void) {
    u16 temp_v0_3;
    u16 temp_v0_4;
    u16 temp_v0_5;
    u16 temp_v0_6;

    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            switch (g_gameMode) {
                case GRAND_PRIX:
                    drive_stick_kindcontrol(gPlayerOne, gControllerOne, 0);
                    return;
                case TIME_TRIALS:
                    if (replay_flag != 1) {
                        drive_stick_kindcontrol(gPlayerOne, gControllerOne, 0);
                        temp_v0_3 = gPlayerTwo->flag;
                        if (((temp_v0_3 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_3 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            drive_stick_kindcontrol(gPlayerTwo, gControllerSix, 1);
                        }
                        temp_v0_4 = gPlayerThree->flag;
                        if (((temp_v0_4 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_4 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            drive_stick_kindcontrol(gPlayerThree, gControllerSeven, 2);
                            return;
                        }
                    } else {
                        if ((gPlayerOne->flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
                            drive_stick_kindcontrol(gPlayerOne, gControllerEight, 0);
                        }
                        temp_v0_5 = gPlayerTwo->flag;
                        if (((temp_v0_5 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_5 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            drive_stick_kindcontrol(gPlayerTwo, gControllerSix, 1);
                        }
                        temp_v0_6 = gPlayerThree->flag;
                        if (((temp_v0_6 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_6 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            drive_stick_kindcontrol(gPlayerThree, gControllerSeven, 2);
                            return;
                        }
                        return;
                    }

                    break;
            }
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            drive_stick_kindcontrol(gPlayerOne, gControllerOne, 0);
            drive_stick_kindcontrol(gPlayerTwo, gControllerTwo, 1);
            return;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            drive_stick_kindcontrol(gPlayerOne, gControllerOne, 0);
            drive_stick_kindcontrol(gPlayerTwo, gControllerTwo, 1);
            drive_stick_kindcontrol(gPlayerThree, gControllerThree, 2);
            if (g_playerCount == 4) {
                drive_stick_kindcontrol(gPlayerFour, gControllerFour, 3);
            }
            break;
    }
}

s16 check_stickX(struct Controller* controller) {
    s16 temp_stickY;
    s16 temp_stickX2;
    s16 temp_stickY2;
    s16 temp_stickX;

    temp_stickX = controller->AnalogX;
    temp_stickY = controller->AnalogY;
    temp_stickX2 = temp_stickX;
    temp_stickY2 = temp_stickY;

    if (temp_stickX > 0xC) {
        temp_stickY2 = (temp_stickY * 0x000C) / temp_stickX;
        temp_stickX2 = 0x000C;
    }
    if (temp_stickX2 < -0xC) {
        temp_stickY2 = (temp_stickY2 * 0x000C) / -temp_stickX2;
        temp_stickX2 = -0x000C;
    }
    if (temp_stickY2 > 0xC) {
        temp_stickX2 = (temp_stickX2 * 0x000C) / temp_stickY2;
        temp_stickY2 = 0x000C;
    }
    if (temp_stickY2 < -0xC) {
        temp_stickX2 = (temp_stickX2 * 0x000C) / -temp_stickY2;
        temp_stickY2 = -0x000C;
    }
    if ((((controller->AnalogX > -0xD) && (controller->AnalogX < 0xD)) && (controller->AnalogY > -0xD)) &&
        (controller->AnalogY < 0xD)) {
        temp_stickX = 0;
        temp_stickY = 0;
    } else {
        temp_stickX -= temp_stickX2;
        temp_stickY -= temp_stickY2;
    }
    if (temp_stickX > 0x35) {
        temp_stickY = (temp_stickY * 0x0035) / temp_stickX;
        temp_stickX = 0x0035;
    }
    if (temp_stickX < -0x35) {
        temp_stickY = (temp_stickY * 0x0035) / -temp_stickX;
        temp_stickX = -0x0035;
    }
    if (temp_stickY > 0x35) {
        temp_stickX = (temp_stickX * 0x0035) / temp_stickY;
        temp_stickY = 0x0035;
    }
    if (temp_stickY < -0x35) {
        temp_stickX = (temp_stickX * 0x0035) / -temp_stickY;
    }
    return temp_stickX;
}

s16 check_stickY(struct Controller* controller) {
    s16 temp_StickY;
    s16 temp_StickX2;
    s16 temp_StickY2;
    s16 temp_StickX;

    temp_StickX = controller->AnalogX;
    temp_StickY = controller->AnalogY;
    temp_StickX2 = temp_StickX;
    temp_StickY2 = temp_StickY;

    if (temp_StickX > 0xC) {
        temp_StickY2 = (temp_StickY * 0x000C) / temp_StickX;
        temp_StickX2 = 0x000C;
    }
    if (temp_StickX2 < -0xC) {
        temp_StickY2 = (temp_StickY2 * 0x000C) / -temp_StickX2;
        temp_StickX2 = -0x000C;
    }
    if (temp_StickY2 > 0xC) {
        temp_StickX2 = (temp_StickX2 * 0x000C) / temp_StickY2;
        temp_StickY2 = 0x000C;
    }
    if (temp_StickY2 < -0xC) {
        temp_StickX2 = (temp_StickX2 * 0x000C) / -temp_StickY2;
        temp_StickY2 = -0x000C;
    }
    if ((((controller->AnalogX > -0xD) && (controller->AnalogX < 0xD)) && (controller->AnalogY > -0xD)) &&
        (controller->AnalogY < 0xD)) {
        temp_StickX = 0;
        temp_StickY = 0;
    } else {
        temp_StickX -= temp_StickX2;
        temp_StickY -= temp_StickY2;
    }
    if (temp_StickX > 0x35) {
        temp_StickY = (temp_StickY * 0x0035) / temp_StickX;
        temp_StickX = 0x0035;
    }
    if (temp_StickX < -0x35) {
        temp_StickY = (temp_StickY * 0x0035) / -temp_StickX;
        temp_StickX = -0x0035;
    }
    if (temp_StickY > 0x35) {
        temp_StickX = (temp_StickX * 0x0035) / temp_StickY;
        temp_StickY = 0x0035;
    }
    if (temp_StickY < -0x35) {
        temp_StickX = (temp_StickX * 0x0035) / -temp_StickY;
        temp_StickY = -0x0035;
    }
    return temp_StickY;
}

void result_accele_on(Player* player, s16 arg1) {
    player->accelcount += (f32) arg1;
    if (player->accelcount < 0.0f) {
        player->accelcount = 0.0f;
    }
    if (player->accelcount >= 250.0f) {
        player->accelcount = 250.0f;
    }
    player->handling_flag |= ACCELERATE;
    player->force = (player->accelcount * player->accelcount) / 25.0f;
}

void result_runkart(Player* player, UNUSED Camera* camera, s8 screenId, s8 playerId) {
    Vec3f sp114 = { 0.0, 0.0, 1.0 };
    Vec3f sp108 = { 0.0, 0.0, 0.0 };
    Vec3f spFC = { 0.0, 0.0, 0.0 };
    Vec3f spF0 = { 0.0, 0.0, 0.0 };
    f32 nextX;
    f32 nextY;
    f32 nextZ;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 sqrt;

    f32 divOptimize;
    UNUSED s32 pad2[10];
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 surfaceDistance;
    UNUSED s32 pad3;
    Vec3f newVelocity;
    Vec3f sp7C;
    UNUSED s32 pad[10];

    player->f_grip = -10.0f;
    player->r_grip = 28.0f;
    player->acc_maxcount = 250.0f;
    WeaponStatus(player, playerId, screenId);
    SetStrategy(player, playerId, screenId);
    player->direction[1] += player->sterrangle;
    spF0[0] = 0;
    spF0[1] = 0;
    spF0[2] = 0;
    AddGravityEdit(player);
    spA4 = 0 * (player->gravity_xz[0] + spFC[0]);
    spA0 = -1 * player->gravity;
    sp9C = 0 * (player->gravity_xz[2] + spFC[2]);
    sp108[2] = 0;
    sp108[1] = 0;
    sp108[0] = 0;
    MultipleMatrixByVector(sp108, player->align);
    spA4 += sp108[0];
    sp9C += sp108[2];
    sp114[2] = player->force;
    MultipleMatrixByVector(sp114, player->align);

    newVelocity[0] = player->velocity[0];
    newVelocity[1] = player->velocity[1];
    newVelocity[2] = player->velocity[2];

    newVelocity[0] += ((((((sp114[0] + spA4) + spF0[0])) - (newVelocity[0] * (0.12 * (player->mass)))) / 6000.0) / 1);
    newVelocity[2] += ((((((sp114[2] + sp9C) + spF0[2])) - (newVelocity[2] * (0.12 * (player->mass)))) / 6000.0) / 1);
    newVelocity[1] += ((((((sp114[1] + spA0) + spF0[1])) - (newVelocity[1] * (0.12 * (player->mass)))) / 6000.0) / 1);
    if ((player->handling_flag & CPU_SIMPLE_KART) == CPU_SIMPLE_KART) {
        player->handling_flag &= ~CPU_SIMPLE_KART;
    }

    posX = player->position[0];
    posY = player->position[1];
    posZ = player->position[2];

    player->old_position[0] = player->position[0];
    player->old_position[1] = player->position[1];
    player->old_position[2] = player->position[2];

    nextX = posX + player->velocity[0];
    nextY = posY + player->velocity[1];
    nextZ = posZ + player->velocity[2];
    DriftJump(player);
    nextY += player->jump;
    nextY -= 0.02;
    custom_check_bump_2(&player->bump, player->radius, nextX, nextY, nextZ, player->old_position[0],
                            player->old_position[1], player->old_position[2]);
    player->up_vector[0] = 0;
    player->up_vector[2] = 0;
    player->up_vector[1] = 1.0f;
    MakeAlignMatrix(player->align, 0, 1.0f, 0, player->direction[1]);
    player->slip_flag |= N_JUMP;
    player->jumpcount += 1;
    surfaceDistance = player->bump.distance_zx;
    if (surfaceDistance <= 0) {
        player->jumpcount = 0;
        player->slip_flag &= ~D_JUMP;
        player->slip_flag &= ~N_JUMP;
        player->jump = player->jumpcount;
    }
    surfaceDistance = player->bump.distance_zx;
    if (surfaceDistance <= 0) {
        CheckMapBG_ZX_Hook(player, sp7C, newVelocity, sp108, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    surfaceDistance = player->bump.distance_xy;
    if (surfaceDistance < 0) {
        CheckMapBG_XY(player, sp7C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(player, playerId, newVelocity);
    }
    surfaceDistance = player->bump.distance_yz;
    if (surfaceDistance < 0) {
        CheckMapBG_YZ(player, sp7C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(player, playerId, newVelocity);
    }
    surfaceDistance = player->bump.distance_xy;
    if (surfaceDistance >= 0) {
        surfaceDistance = player->bump.distance_yz;
        if (surfaceDistance >= 0) {
            player->hit_flag &= 0xFFDF;
        }
    }
    player->ground = CalcHeight(nextX, nextY, nextZ, player->bump.last_zx);
    TirePosition(player, nextX, nextY, nextZ);
    SetSlipAngle(player, playerId, posX, posZ, nextX, nextZ);
    sqrt = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    player->old_speed = player->speed;
    player->speed = sqrtf(sqrt);

    player->position[0] = nextX;
    player->position[2] = nextZ;
    player->position[1] = nextY;

    player->gravity_xz[0] = sp108[0];
    player->gravity_xz[2] = sp108[2];

    player->velocity[0] = newVelocity[0];
    player->velocity[1] = newVelocity[1];
    player->velocity[2] = newVelocity[2];

    old_velocity[playerId][0] = newVelocity[0];
    old_velocity[playerId][1] = newVelocity[1];
    old_velocity[playerId][2] = newVelocity[2];

    if ((player->flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[player->kart] < player->speed) {
            divOptimize = gKartTopSpeedTable[player->kart] / player->speed;
            player->velocity[0] *= divOptimize;
            player->velocity[1] *= divOptimize;
            player->velocity[2] *= divOptimize;
            player->speed = gKartTopSpeedTable[player->kart];
        }
    }
    if ((player->handling_flag & REVERSE_GEAR) == REVERSE_GEAR) {
        if (player->speed > 1.0f) {
            player->velocity[0] *= 1.0f / player->speed;
            player->velocity[1] *= 1.0f / player->speed;
            player->velocity[2] *= 1.0f / player->speed;
            player->speed = 1.0f;
        }
    }
    if (player->bump.distance_zx >= 500.0f) {
        player->sterrangle /= 2;
    }
    CheckSplash(player, playerId);
}
