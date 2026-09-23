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

s16 CPUForMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 CPUForLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 CPUForYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH, BOWSER, 0 };

s16 CPUForToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER, 0 };

s16 CPUForDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER, 0 };

s16 CPUForWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER, 0 };

s16 CPUForPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER, 0 };

s16 CPUForBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO, PEACH, 0 };

s16* CPUForPlayer[] = { CPUForMario, CPUForLuigi, CPUForYoshi, CPUForToad,
                         CPUForDk,    CPUForWario, CPUForPeach, CPUForBowser };

s16 CPUForMarioAndLuigi[] = { YOSHI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 CPUForMarioAndYoshi[] = { LUIGI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 CPUForMarioAndToad[] = { LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 CPUForMarioAndDk[] = { LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 CPUForMarioAndWario[] = { LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 CPUForMarioAndPeach[] = { LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 CPUForMarioAndBowser[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH };

s16 CPUForLuigiAndMario[] = { YOSHI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 CPUForLuigiAndYoshi[] = { MARIO, TOAD, DK, WARIO, PEACH, BOWSER };

s16 CPUForLuigiAndToad[] = { MARIO, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 CPUForLuigiAndDk[] = { MARIO, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 CPUForLuigiAndWario[] = { MARIO, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 CPUForLuigiAndPeach[] = { MARIO, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 CPUForLuigiAndBowser[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH };

s16 CPUForYoshiAndMario[] = { LUIGI, TOAD, DK, WARIO, PEACH, BOWSER };

s16 CPUForYoshiAndLuigi[] = { MARIO, TOAD, DK, WARIO, PEACH, BOWSER };

s16 CPUForYoshiAndToad[] = { MARIO, LUIGI, DK, WARIO, PEACH, BOWSER };

s16 CPUForYoshiAndDk[] = { MARIO, LUIGI, TOAD, WARIO, PEACH, BOWSER };

s16 CPUForYoshiAndWario[] = { MARIO, LUIGI, TOAD, DK, PEACH, BOWSER };

s16 CPUForYoshiAndPeach[] = { MARIO, LUIGI, TOAD, DK, WARIO, BOWSER };

s16 CPUForYoshiAndBowser[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH };

s16 CPUForToadAndMario[] = { LUIGI, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 CPUForToadAndLuigi[] = { MARIO, YOSHI, DK, WARIO, PEACH, BOWSER };

s16 CPUForToadAndYoshi[] = { MARIO, LUIGI, DK, WARIO, PEACH, BOWSER };

s16 CPUForToadAndDk[] = { MARIO, LUIGI, YOSHI, WARIO, PEACH, BOWSER };

s16 CPUForToadAndWario[] = { MARIO, LUIGI, YOSHI, DK, PEACH, BOWSER };

s16 CPUForToadAndPeach[] = { MARIO, LUIGI, YOSHI, DK, WARIO, BOWSER };

s16 CPUForToadAndBowser[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH };

s16 CPUForDkAndMario[] = { LUIGI, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 CPUForDkAndLuigi[] = { MARIO, YOSHI, TOAD, WARIO, PEACH, BOWSER };

s16 CPUForDkAndToad[] = { MARIO, LUIGI, TOAD, WARIO, PEACH, BOWSER };

s16 CPUForDkAndYoshi[] = { MARIO, LUIGI, YOSHI, WARIO, PEACH, BOWSER };

s16 CPUForDkAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, PEACH, BOWSER };

s16 CPUForDkAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, BOWSER };

s16 CPUForDkAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH };

s16 CPUForWarioAndMario[] = { LUIGI, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 CPUForWarioAndLuigi[] = { MARIO, YOSHI, TOAD, DK, PEACH, BOWSER };

s16 CPUForWarioAndYoshi[] = { MARIO, LUIGI, TOAD, DK, PEACH, BOWSER };

s16 CPUForWarioAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, PEACH, BOWSER };

s16 CPUForWarioAndToad[] = { MARIO, LUIGI, YOSHI, DK, PEACH, BOWSER };

s16 CPUForWarioAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, BOWSER };

s16 CPUForWarioAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH };

s16 CPUForPeachAndMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 CPUForPeachAndLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, BOWSER };

s16 CPUForPeachAndYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, BOWSER };

s16 CPUForPeachAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, BOWSER };

s16 CPUForPeachAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, BOWSER };

s16 CPUForPeachAndToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, BOWSER };

s16 CPUForPeachAndBowser[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO };

s16 CPUForBowserAndMario[] = { LUIGI, YOSHI, TOAD, DK, WARIO, PEACH };

s16 CPUForBowserAndLuigi[] = { MARIO, YOSHI, TOAD, DK, WARIO, PEACH };

s16 CPUForBowserAndYoshi[] = { MARIO, LUIGI, TOAD, DK, WARIO, PEACH };

s16 CPUForBowserAndDk[] = { MARIO, LUIGI, YOSHI, TOAD, WARIO, PEACH };

s16 CPUForBowserAndWario[] = { MARIO, LUIGI, YOSHI, TOAD, DK, PEACH };

s16 CPUForBowserAndToad[] = { MARIO, LUIGI, YOSHI, DK, WARIO, PEACH };

s16 CPUForBowserAndPeach[] = { MARIO, LUIGI, YOSHI, TOAD, DK, WARIO };

s16* ListCPUforMario[] = { CPUForMarioAndLuigi, CPUForMarioAndLuigi, CPUForMarioAndYoshi, CPUForMarioAndToad,
                            CPUForMarioAndDk,    CPUForMarioAndWario, CPUForMarioAndPeach, CPUForMarioAndBowser };

s16* ListCPUforLuigi[] = { CPUForLuigiAndMario, CPUForLuigiAndMario, CPUForLuigiAndYoshi, CPUForLuigiAndToad,
                            CPUForLuigiAndDk,    CPUForLuigiAndWario, CPUForLuigiAndPeach, CPUForLuigiAndBowser };

s16* ListCPUforYoshi[] = { CPUForYoshiAndMario, CPUForYoshiAndLuigi, CPUForYoshiAndLuigi, CPUForYoshiAndToad,
                            CPUForYoshiAndDk,    CPUForYoshiAndWario, CPUForYoshiAndPeach, CPUForYoshiAndBowser };

s16* ListCPUforToad[] = { CPUForToadAndMario, CPUForToadAndLuigi, CPUForToadAndYoshi, CPUForToadAndYoshi,
                           CPUForToadAndDk,    CPUForToadAndWario, CPUForToadAndPeach, CPUForToadAndBowser };

s16* ListCPUforDk[] = { CPUForDkAndMario, CPUForDkAndLuigi, CPUForDkAndToad,  CPUForDkAndYoshi,
                         CPUForDkAndYoshi, CPUForDkAndWario, CPUForDkAndPeach, CPUForDkAndBowser };

s16* ListCPUforWario[] = { CPUForWarioAndMario, CPUForWarioAndLuigi, CPUForWarioAndYoshi, CPUForWarioAndToad,
                            CPUForWarioAndDk,    CPUForWarioAndDk,    CPUForWarioAndPeach, CPUForWarioAndBowser };

s16* ListCPUforPeach[] = { CPUForPeachAndMario, CPUForPeachAndLuigi, CPUForPeachAndYoshi, CPUForPeachAndToad,
                            CPUForPeachAndDk,    CPUForPeachAndWario, CPUForPeachAndDk,    CPUForPeachAndBowser };

s16* ListCPUforBowser[] = {
    CPUForBowserAndMario, CPUForBowserAndLuigi, CPUForBowserAndYoshi, CPUForBowserAndToad,
    CPUForBowserAndDk,    CPUForBowserAndWario, CPUForBowserAndPeach, CPUForBowserAndPeach
};

s16** CPUForTwoPlayer[] = { ListCPUforMario, ListCPUforLuigi, ListCPUforYoshi, ListCPUforToad,
                             ListCPUforDk,    ListCPUforWario, ListCPUforPeach, ListCPUforBowser };

// func_80027D00
s32 CheckKartNumber(Player* Player) {
    s32 Index;

    if (Player == PlayerOne) {
        Index = 0;
    }
    if (Player == PlayerTwo) {
        Index = 1;
    }
    if (Player == PlayerThree) {
        Index = 2;
    }
    if (Player == PlayerFour) {
        Index = 3;
    }
    if (Player == PlayerFive) {
        Index = 4;
    }
    if (Player == PlayerSix) {
        Index = 5;
    }
    if (Player == PlayerSeven) {
        Index = 6;
    }
    if (Player == PlayerEight) {
        Index = 7;
    }
    return Index;
}

void StartKartEngine(Player* Player, s8 PlayerId) {
    if (ReplayFlag != 1) {
        if ((Player->Flag & MOTOR_ON) != MOTOR_ON) {
            if (((D_8018D168 == 1) && ((Player->Flag & IS_PLAYER) == IS_PLAYER)) &&
                ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                NaPlyEngStart(PlayerId);
                Player->Flag |= MOTOR_ON;
            } else if ((Player->Flag & NO_CONTROLS) == 0) {
                NaEnmLevelStart(PlayerId);
                Player->Flag |= MOTOR_ON;
            }
        }
    } else if ((Player->Flag & MOTOR_ON) != MOTOR_ON) {
        if ((D_8018D168 == 1) && (Player == PlayerOne)) {
            NaPlyEngStart(PlayerId);
            Player->Flag |= MOTOR_ON;
        } else if ((Player->Flag & NO_CONTROLS) == 0) {
            NaEnmLevelStart(PlayerId);
            Player->Flag |= MOTOR_ON;
        }
    }
}

void KartEngineEcho(Player* Player, s8 PlayerId) {
    UNUSED s32 pad;
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {

#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (CourseID) {
            case COURSE_MARIO_RACEWAY:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= EchoStart) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < EchoStop)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NAPlyEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NAPlyEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_CHOCO_MOUNTAIN:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0xA0) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0xB4)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NAPlyEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NAPlyEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_BOWSER_CASTLE:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x29) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x1D2)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NAPlyEchoOn(PlayerId, 0x41);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NAPlyEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_BANSHEE_BOARDWALK:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x180) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x1E1)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NAPlyEchoOn(PlayerId, 0x41);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NAPlyEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_LUIGI_RACEWAY:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x145) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x18B)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NAPlyEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NAPlyEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_TOADS_TURNPIKE:
                if ((Player->Flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
                    if (EchoFlag[PlayerId] != 1) {
                        NAPlyEchoOn(PlayerId, 0x1e);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NAPlyEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_SHERBET_LAND:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x11C) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x209)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NAPlyEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NAPlyEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_DK_JUNGLE:
                if ((((s16) NearestPathPointByPlayerId[PlayerId] >= 0) &&
                     ((s16) NearestPathPointByPlayerId[PlayerId] < 0x65)) ||
                    (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x14A) &&
                     ((s16) NearestPathPointByPlayerId[PlayerId] < 0x21F))) {
                    if (EchoFlag[PlayerId] != 2) {
                        NaPlyLevelStart(PlayerId, 0x0170802D);
                    }
                    EchoFlag[PlayerId] = 2;
                } else {
                    if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x288) &&
                        ((s16) NearestPathPointByPlayerId[PlayerId] < 0x305)) {
                        if (EchoFlag[PlayerId] != 1) {
                            NAPlyEchoOn(PlayerId, 0x55);
                        }
                        EchoFlag[PlayerId] = 1;
                    } else {
                        if (EchoFlag[PlayerId] != 0) {
                            if (EchoFlag[PlayerId] == 1) {
                                NAPlyEchoOff(PlayerId);
                            }
                            if (EchoFlag[PlayerId] == 2) {
                                NaPlyLevelStop(PlayerId, SOUND_ARG_LOAD(0x01, 0x70, 0x80, 0x2D));
                            }
                            EchoFlag[PlayerId] = 0;
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
        switch (CourseID) {
            case COURSE_MARIO_RACEWAY:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= EchoStart) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < EchoStop)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_CHOCO_MOUNTAIN:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0xA0) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0xB4)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_BOWSER_CASTLE:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x29) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x1D2)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x41);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_BANSHEE_BOARDWALK:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x180) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x1E1)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x41);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_LUIGI_RACEWAY:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x145) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x18B)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_TOADS_TURNPIKE:
                if ((Player->Flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x1E);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_SHERBET_LAND:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x11C) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x209)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
                    }
                }
                break;
            case COURSE_DK_JUNGLE:
                if (((s16) NearestPathPointByPlayerId[PlayerId] >= 0x288) &&
                    ((s16) NearestPathPointByPlayerId[PlayerId] < 0x305)) {
                    if (EchoFlag[PlayerId] != 1) {
                        NaEnmEchoOn(PlayerId, 0x55);
                    }
                    EchoFlag[PlayerId] = 1;
                } else {
                    if (EchoFlag[PlayerId] != 0) {
                        NaEnmEchoOff(PlayerId);
                        EchoFlag[PlayerId] = 0;
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

void MariogpMovekart(Player* Player, Camera* Camera, s8 PlayerId, s8 screenId) {
    u16 isVisible;

    if (!(Player->Flag & NO_CONTROLS)) {
        switch (ScreenSplitA) {
            case SCREEN_MODE_1P:
                isVisible = IsInViewport(Player, Camera1, (f32) SelectBumpKart, 0.0f);
                break;
            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                isVisible = IsInViewport(Player, Camera1, (f32) SelectBumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = IsInViewport(Player, Camera2, (f32) SelectBumpKart, 0.0f);
                break;
            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                isVisible = IsInViewport(Player, Camera1, (f32) SelectBumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = IsInViewport(Player, Camera2, (f32) SelectBumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = IsInViewport(Player, Camera3, (f32) SelectBumpKart, 0.0f);
                if (isVisible == true) {
                    break;
                }
                isVisible = IsInViewport(Player, Camera4, (f32) SelectBumpKart, 0.0f);
                break;
        }
        if ((isVisible == 1) || ((Player->Flag & IS_GHOST) == IS_GHOST) ||
            (GameMode == BATTLE) || ((Player->JugemuFlag & ON_LAKITU_ROD) != 0) ||
            (Player->JugemuFlag & LAKITU_SCENE) ||
            //! @todo make a proper match
            ((*(NoSimpleKartFlag + (PlayerId))) == ((s16) 1U))) {
            Player->SlipFlag &= ~CENTER_LINE;
            if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
                ((Player->SlipFlag & SPIN_R) == SPIN_R) ||
                ((Player->SlipFlag & ROLLOVER) == ROLLOVER) ||
                ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) ||
                ((Player->SlipFlag & WING) == WING) ||
                ((Player->SlipFlag & STORM) == STORM) ||
                ((Player->SlipFlag & EXPLODE) == EXPLODE) ||
                ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) ||
                ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) ||
                (Player->HandlingFlag & TROPHYJUMP_END)) {
                CustomSpinKart(Player, Camera, screenId, PlayerId);
            } else {
                RunKart(Player, Camera, screenId, PlayerId);
            }
        } else {
            RunKartSimple(Player, Camera, screenId, PlayerId);
        }
    } else if ((Player->Flag & IS_GP_OPENING) == IS_GP_OPENING) {
        OpeningKart(Player, PlayerId);
        Openkart(Player, Camera, screenId, PlayerId);
    } else if (Player->Flag & PLAYER_UNKNOWN_0x80) {
        RunKart(Player, Camera, screenId, PlayerId);
    } else {
        if ((Player->Flag & IS_PLAYER) != IS_PLAYER) {
            Player->Accelcount = 50.0f;
        }
        Player->SlipFlag &= ~N_JUMP;
    }
}

void VSTimeMovekart(Player* Player, Camera* Camera, s8 PlayerId, s8 screenId) {
    if ((Player->Flag & NO_CONTROLS) == 0) {
        Player->SlipFlag &= ~CENTER_LINE;
        if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
            ((Player->SlipFlag & SPIN_R) == SPIN_R) ||
            ((Player->SlipFlag & ROLLOVER) == ROLLOVER) ||
            ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) ||
            ((Player->SlipFlag & WING) == WING) ||
            ((Player->SlipFlag & STORM) == STORM) ||
            ((Player->SlipFlag & EXPLODE) == EXPLODE) ||
            ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) ||
            ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) ||
            ((Player->HandlingFlag & TROPHYJUMP_END) != 0)) {
            CustomSpinKart(Player, Camera, screenId, PlayerId);
        } else {
            RunKart(Player, Camera, screenId, PlayerId);
        }
    } else {
        Player->SlipFlag &= ~N_JUMP;
    }
}

void BattleMovekart(Player* Player, Camera* Camera, s8 PlayerId, s8 screenId) {
    if ((((Player->Flag & NO_CONTROLS) == 0) && (StartingIndicator != RACE_DONE)) || (Player->JugemuFlag & 2) != 0 ||
        (Player->JugemuFlag & LAKITU_SCENE) != 0 ||
        (Player->SlipFlag & (THUNDER | EXPLODE | THROW_EXPLODE | BROKEN |
                            RESTORE | ROLLOVER_FALL | 0xC00 | 0xC0)) != 0) {
        Player->SlipFlag &= ~CENTER_LINE;
        if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
            ((Player->SlipFlag & SPIN_R) == SPIN_R) ||
            ((Player->SlipFlag & ROLLOVER) == ROLLOVER) ||
            ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) ||
            ((Player->SlipFlag & WING) == WING) ||
            ((Player->SlipFlag & STORM) == STORM) ||
            ((Player->SlipFlag & EXPLODE) == EXPLODE) ||
            ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) ||
            ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) ||
            ((Player->HandlingFlag & TROPHYJUMP_END) != 0)) {
            CustomSpinKart(Player, Camera, screenId, PlayerId);
        } else {
            RunKart(Player, Camera, screenId, PlayerId);
        }
    } else {
        Player->SlipFlag = Player->SlipFlag & ~N_JUMP;
    }
}

void Movekart(Player* Player, Camera* Camera, s8 PlayerId, s8 screenId) {
    if ((Player->Flag & EXISTS) == EXISTS) {
        switch (GameState) {
            case ENDING:
                if (!(Player->Flag & NO_CONTROLS)) {
                    ResultRunkart(Player, Camera, screenId, PlayerId);
                } else {
                    Player->SlipFlag &= ~N_JUMP;
                }
                break;
            default:
                StartKartEngine(Player, PlayerId);
                switch (GameMode) {
                    case TIME_TRIALS:
                    case VERSUS:
                        VSTimeMovekart(Player, Camera, PlayerId, screenId);
                        break;
                    case BATTLE:
                        BattleMovekart(Player, Camera, PlayerId, screenId);
                        break;
                    default:
                        MariogpMovekart(Player, Camera, PlayerId, screenId);
                        break;
                }
                break;
        }
    }
}

UNUSED void func_80028F5C(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 Arg2, UNUSED s32 Arg3) {
}

void KartposControlFull(void) {
    Effectcount = 0;
    Movekart(PlayerOneCopy, Camera1, 0, 0);
    Movekart(PlayerTwo, Camera1, 1, 0);
    Movekart(PlayerThree, Camera1, 2, 0);
    Movekart(PlayerFour, Camera1, 3, 0);
    Movekart(PlayerFive, Camera1, 4, 0);
    Movekart(PlayerSix, Camera1, 5, 0);
    Movekart(PlayerSeven, Camera1, 6, 0);
    Movekart(PlayerEight, Camera1, 7, 0);
}

void KartposControlRu(void) {
    Effectcount = 0;
    Movekart(PlayerOneCopy, Camera1, 0, 0);
    Movekart(PlayerTwo, Camera1, 1, 0);
    Movekart(PlayerThree, Camera1, 2, 0);
    Movekart(PlayerFour, Camera1, 3, 0);
    Movekart(PlayerFive, Camera1, 4, 0);
    Movekart(PlayerSix, Camera1, 5, 0);
    Movekart(PlayerSeven, Camera1, 6, 0);
    Movekart(PlayerEight, Camera1, 7, 0);
}

void KartposControlLd(void) {
}

void KartpostControl1P(void) {
    Effectcount = 0;
    Movekart(PlayerOneCopy, Camera1, 0, 0);
    Movekart(PlayerTwo, Camera1, 1, 0);
    Movekart(PlayerThree, Camera1, 2, 0);
    Movekart(PlayerFour, Camera1, 3, 0);
}

void KartpostControl2P(void) {
}

void KartposControl3P(void) {
}

void KartposControl4P(void) {
}

void MakeDegpict(Player* Player, s8 screenId) {
    if ((s32) Player->Kartpitch < -0x71B) {
        Player->deg[screenId] = 0;
    }
    if (((s32) Player->Kartpitch < -0x4F9) && ((s32) Player->Kartpitch > -DEGREES(10))) {
        Player->deg[screenId] = 1;
    }
    if ((Player->Kartpitch <= -DEGREES(3)) && (Player->Kartpitch > -DEGREES(7))) {
        Player->deg[screenId] = 2;
    }
    if ((Player->Kartpitch <= -DEGREES(2)) && (Player->Kartpitch > -DEGREES(3))) {
        Player->deg[screenId] = 3;
    }
    if ((Player->Kartpitch < DEGREES(2)) && (Player->Kartpitch > -DEGREES(2))) {
        Player->deg[screenId] = 4;
    }
    if ((Player->Kartpitch >= DEGREES(2)) && (Player->Kartpitch < DEGREES(3))) {
        Player->deg[screenId] = 5;
    }
    if ((Player->Kartpitch >= DEGREES(3)) && (Player->Kartpitch < DEGREES(7))) {
        Player->deg[screenId] = 6;
    }
    if ((Player->Kartpitch >= DEGREES(7)) && (Player->Kartpitch < DEGREES(10))) {
        Player->deg[screenId] = 7;
    }
    if (Player->Kartpitch >= DEGREES(10)) {
        Player->deg[screenId] = 8;
    }
}

void KartPosset(Player* Player, Camera* Camera, s8 screenId, s8 PlayerId) {
    UNUSED s32 pad[2];
    f32 temp_f0;
    f32 temp_f2;
    UNUSED s32 Pad2[3];
    f32 var_f0;
    s16 temp_a0;
    s32 temp_a0_2;
    s32 var_a1;
    s32 var_t0;
    u16 var_a0;

    Player->AngleY[screenId] = Atan2t(Player->Position[0] - Camera->CameraPos[0], Player->Position[2] - Camera->CameraPos[2]);
    Player->dir[screenId] =
        (u16) ((((Player->AngleY[screenId]) + Player->Direction[1] + Player->Slipang))) / 128;

    temp_f2 = (CharacterSize[Player->Kart] * 18.0f) * Player->Offsetsize;
    temp_f0 = Player->MidLeft - Player->MidRight;
    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        if ((Player->SlipFlag & THUNDER) == THUNDER) {
            Player->Roll[screenId] = (s16) ((s32) (((f64) Atant(temp_f0 / temp_f2)) * 1.6));
        } else {
            Player->Roll[screenId] = Atant(temp_f0 / temp_f2) * 2;
        }
    }
    if ((Player->SlipFlag & BROKEN) == BROKEN) {
        Player->Roll[screenId] = (s16) ((s32) Player->sus.Position);
    }
    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        temp_f0 = Player->MidFront - Player->MidRear;
        Player->Pitch[screenId] = (((Atant(temp_f0 / temp_f2)) * 0.9));
    } else {
        if (((Player->dir[screenId]) >= 0) && ((Player->dir[screenId]) < 0x101)) {
            var_f0 = Player->OldPosition[1] - Player->Position[1];
        } else {
            var_f0 = Player->Position[1] - Player->OldPosition[1];
        }
        Player->Pitch[screenId] = (s16) ((s32) (((f64) Atant(var_f0 / temp_f2)) * 0.5));
    }
    if ((Player->SlipFlag & BROKEN) == BROKEN) {
        Player->Pitch[screenId] = (s16) ((s32) Player->sus.Position);
    }
    MakeDegpict(Player, screenId);
    temp_a0 = ((Player->AngleY[screenId] + Player->Direction[1]) + Player->Slipang);
    temp_a0 = (s16) Player->Pitch[screenId] * SinT((u16) temp_a0) + Player->Roll[screenId] * CosT((u16) temp_a0);
    ChaseSnumber(&Player->AngleZ[screenId], temp_a0, 0.5f);
    var_a0 = Player->dir[screenId];
    Player->Status = Player->Status & (~(P1_FLIP_SPRITE << (screenId * 4)));
    if (var_a0 >= 0x101) {
        var_a0 = 0x201 - var_a0;
        Player->Status |= (P1_FLIP_SPRITE << (screenId * 4));
    }
    if (((Player->SlipFlag & SPIN_L) != SPIN_L) &&
        ((Player->SlipFlag & SPIN_R) != SPIN_R) &&
        ((Player->SlipFlag & WING) != WING) &&
        ((Player->SlipFlag & STORM) != STORM) &&
        ((Player->SlipFlag & THUNDER_SPIN) != THUNDER_SPIN) &&
        (!(Player->HandlingFlag & TROPHYJUMP_END))) {
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
    if (((Player->SlipFlag & WING) == WING) ||
        ((Player->SlipFlag & STORM) == STORM) ||
        (Player->HandlingFlag & TROPHYJUMP_END)) {
        Player->AngleZ[screenId] = 0;
    }
    if (((Player->SlipFlag & N_JUMP) == N_JUMP) &&
        ((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
        Player->AngleZ[screenId] = 0;
    }
    var_a0 = (Player->AngleY[screenId] + Player->Direction[1] + Player->Slipang);
    if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
        ((Player->SlipFlag & SPIN_R) == SPIN_R) ||
        ((Player->SlipFlag & WING) == WING) ||
        ((Player->SlipFlag & STORM) == STORM) ||
        ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) ||
        (Player->HandlingFlag & TROPHYJUMP_END)) {
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
    Player->dir[screenId] = var_a0 + var_t0;
    if ((Player->dir[screenId]) >= 0x23) {
        Player->dir[screenId] = 0x22;
    }
    if ((Player->SlipFlag & SPIN_L) || (Player->SlipFlag & SPIN_R) ||
        (Player->SlipFlag & WING) || (Player->SlipFlag & STORM) ||
        (Player->SlipFlag & THUNDER_SPIN) || (Player->HandlingFlag & TROPHYJUMP_END)) {

        if ((Player->dir[screenId]) >= 0x14) {
            Player->dir[screenId] = 0;
        }
    }
    if ((Player->deg[screenId]) >= 9) {
        Player->deg[screenId] = 4;
    }
    if (((Player->SlipFlag & WING) == WING) ||
        ((Player->SlipFlag & STORM) == STORM) ||
        (Player->HandlingFlag & TROPHYJUMP_END)) {

        Player->deg[screenId] = 4;
    }
    if (((Player->SlipFlag & ROLLOVER) == ROLLOVER) ||
        ((Player->SlipFlag & EXPLODE) == EXPLODE) ||
        ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) || (Player->SlipFlag & ROLLOVER_FALL) ||
        (Player->SlipFlag & SPIN_L) || (Player->SlipFlag & SPIN_R)) {

        Player->Status |= P1_DMA_SPRITE << (screenId * 4);
        CompelFlag[screenId][PlayerId] = 1;

        if ((Player->SlipFlag & SPIN_L) || (Player->SlipFlag & SPIN_R)) {
            if ((Player->dir[screenId] == OldDir[screenId][PlayerId]) &&
                (Player->deg[screenId] == OldDeg[screenId][PlayerId])) {
                Player->Status &= ~(P1_DMA_SPRITE << (screenId * 4));
                CompelFlag[screenId][PlayerId] = 1;
            }
        } else if (((Player->Slipcount) >> 8) == OldSlipcount[screenId][PlayerId] >> 8) {
            Player->Status &= ~(P1_DMA_SPRITE << (screenId * 4));
        }
    } else {
        Player->Status |= P1_DMA_SPRITE << (screenId * 4);
        if (((Player->dir[screenId] == OldDir[screenId][PlayerId]) &&
             (Player->deg[screenId] == OldDeg[screenId][PlayerId])) &&
            ((CompelFlag[screenId][PlayerId]) == 0)) {
            Player->Status &= ~(P1_DMA_SPRITE << (screenId * 4));
        }
    }
    temp_a0_2 = OldDir[screenId][PlayerId] - Player->dir[screenId];
    if ((temp_a0_2 >= 0x14) || (temp_a0_2 < (-0x13))) {
        Player->Status |= P1_DMA_SPRITE << (screenId * 4);
    }
}

void TirePosition(Player* Player, UNUSED f32 arg1, f32 Arg2, UNUSED f32 Arg3) {
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

    if ((Player->SlipFlag & THUNDER) == THUNDER) {
        var_f12 = 18.0f * ((((CharacterSize[Player->Kart] / 2)) * ((Player->Offsetsize) * 1.5)));
    } else {
        var_f12 = 18.0f * (CharacterSize[Player->Kart] / 2);
    }

    MakeAlignMatrix(sp5C, 0.0f, 1.0f, 0.0f, (Player->Direction[1] + Player->Slipang));
    sp8C[0] = var_f12 - 3.6;
    sp8C[1] = -Player->Radius;
    sp8C[2] = var_f12 - 2.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = Player->TireFL.Position[0];
    sp80[1] = Player->TireFL.Position[1];
    sp80[2] = Player->TireFL.Position[2];
    Player->TireFL.Position[0] = Player->Position[0] + sp8C[0];
    Player->TireFL.Position[1] = Player->Position[1] + sp8C[1];
    Player->TireFL.Position[2] = Player->Position[2] + sp8C[2];
    CheckTire2(Player, &Player->TireFL, sp80[0], sp80[1], sp80[2]);
    sp8C[0] = (-var_f12) + 3.6;
    sp8C[1] = -Player->Radius;
    sp8C[2] = var_f12 - 2.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = Player->TireFR.Position[0];
    sp80[1] = Player->TireFR.Position[1];
    sp80[2] = Player->TireFR.Position[2];
    Player->TireFR.Position[0] = Player->Position[0] + sp8C[0];
    Player->TireFR.Position[1] = Player->Position[1] + sp8C[1];
    Player->TireFR.Position[2] = Player->Position[2] + sp8C[2];
    CheckTire2(Player, &Player->TireFR, sp80[0], sp80[1], sp80[2]);
    sp8C[0] = var_f12 - 2.6;
    sp8C[1] = -Player->Radius;
    sp8C[2] = (-var_f12) + 4.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = Player->TireRL.Position[0];
    sp80[1] = Player->TireRL.Position[1];
    sp80[2] = Player->TireRL.Position[2];
    Player->TireRL.Position[0] = Player->Position[0] + sp8C[0];
    Player->TireRL.Position[1] = Player->Position[1] + sp8C[1];
    Player->TireRL.Position[2] = Player->Position[2] + sp8C[2];
    CheckTire2(Player, &Player->TireRL, sp80[0], sp80[1], sp80[2]);
    sp8C[0] = (-var_f12) + 2.6;
    sp8C[1] = -Player->Radius;
    sp8C[2] = (-var_f12) + 4.0f;
    MultipleMatrixByVector(sp8C, sp5C);
    sp80[0] = Player->TireRR.Position[0];
    sp80[1] = Player->TireRR.Position[1];
    sp80[2] = Player->TireRR.Position[2];
    Player->TireRR.Position[0] = Player->Position[0] + sp8C[0];
    Player->TireRR.Position[1] = Player->Position[1] + sp8C[1];
    Player->TireRR.Position[2] = Player->Position[2] + sp8C[2];
    CheckTire2(Player, &Player->TireRR, sp80[0], sp80[1], sp80[2]);
    if (!(Player->SlipFlag & N_JUMP)) {
        a = (Player->TireRL.Height + Player->TireFL.Height) / 2;
        ChaseFnumber(&Player->MidLeft, a, 0.5f);

        b = (Player->TireRR.Height + Player->TireFR.Height) / 2;
        ChaseFnumber(&Player->MidRight, b, 0.5f);

        c = (Player->TireFR.Height + Player->TireFL.Height) / 2;
        ChaseFnumber(&Player->MidRear, c, 0.5f);

        d = (Player->TireRR.Height + Player->TireRL.Height) / 2;
        ChaseFnumber(&Player->MidFront, d, 0.5f);
    }
    temp_f2_3 = ((CharacterSize[Player->Kart] * 18.0f) + 1.0f) * Player->Offsetsize;
    temp_f0_2 = Player->MidRight - Player->MidLeft;
    Player->Kartroll = -Atant(temp_f0_2 / temp_f2_3);
    if (((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) || (Player->SlipFlag & N_JUMP)) {
        Player->Kartroll = 0;
    }
    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        temp_f0_2 = Player->MidFront - Player->MidRear;
        ChaseSnumber(&Player->Kartpitch, Atant(temp_f0_2 / temp_f2_3), 0.5f);
    } else {
        temp_f0_2 = Player->OldPosition[1] - Arg2;
        temp_v0 = Atant(temp_f0_2 / temp_f2_3);
        if (temp_f0_2 >= 0.0f) {
            temp_v0 /= 4;
        } else {
            temp_v0 *= 10;
        }
        ChaseSnumber(&Player->Kartpitch, temp_v0, 0.5f);
    }
    if (((Player->SlipFlag & N_JUMP) == N_JUMP) &&
        ((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD)) {
        Player->Kartpitch = (s16) ((s32) Player->sus.Position);
    }
    Player->BumpStatus = CheckStatus(Player->Bump.LastZX) & 0xFF;
    if (Player->BumpStatus == BOOST_RAMP_ASPHALT) {
        if (((Player->SlipFlag & DASH_JUMP) != DASH_JUMP) &&
            ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
            Player->Weapon |= PRESS_BOOSTPAD;
        }
    }
    if (Player->BumpStatus == BOOST_RAMP_WOOD) {
        if (((Player->SlipFlag & DASH_MIDJUMP) != DASH_MIDJUMP) &&
            ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
            Player->Weapon |= PRESS_BOOSTJUMP;
        }
    }
}

void EnemyTirePosition(Player* Player, f32 x, f32 y, f32 z) {
    UNUSED s32 pad[2];
    f32 temp_f12;
    f32 var_f20;
    s32 temp_v0;
    s16 temp_v1;
    s16 var_a1;
    UNUSED s32 Pad2;
    f32 temp_f0;

    temp_v1 = -Player->Direction[1] - Player->Slipang;
    if ((Player->SlipFlag & THUNDER) == THUNDER) {
        var_f20 = (((CharacterSize[Player->Kart] * 18) / 2) * (Player->Offsetsize * 1.5)) - 1;
    } else {
        var_f20 = (((CharacterSize[Player->Kart] * 18) / 2) * Player->Offsetsize) - 1;
    }

    Player->TireFL.Position[2] = (CosT(temp_v1 + 0x2000) * var_f20) + z;
    temp_f12 = (SinT(temp_v1 + 0x2000) * var_f20) + x;
    Player->TireFL.Position[0] = temp_f12;
    Player->TireFL.Height =
        CalcHeight(temp_f12, y, Player->TireFL.Position[2], Player->Bump.LastZX);

    Player->TireFR.Position[2] = (CosT(temp_v1 - 0x2000) * var_f20) + z;
    temp_f12 = (SinT(temp_v1 - 0x2000) * var_f20) + x;
    Player->TireFR.Position[0] = temp_f12;
    Player->TireFR.Height =
        CalcHeight(temp_f12, y, Player->TireFR.Position[2], Player->Bump.LastZX);

    Player->TireRL.Position[2] = (CosT(temp_v1 + 0x6000) * var_f20) + z;
    temp_f12 = (SinT(temp_v1 + 0x6000) * var_f20) + x;
    Player->TireRL.Position[0] = temp_f12;
    Player->TireRL.Height =
        CalcHeight(temp_f12, y, Player->TireRL.Position[2], Player->Bump.LastZX);

    Player->TireRR.Position[2] = (CosT(temp_v1 - 0x6000) * var_f20) + z;
    Player->TireRR.Position[0] = (SinT(temp_v1 - 0x6000) * var_f20) + x;
    Player->TireRR.Height = CalcHeight(
        Player->TireRL.Position[0], y, Player->TireRL.Position[2], Player->Bump.LastZX);

    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        Player->MidLeft = (Player->TireRL.Height + Player->TireFL.Height) / 2;
        Player->MidRight = (Player->TireRR.Height + Player->TireFR.Height) / 2;
        Player->MidRear = (Player->TireFR.Height + Player->TireFL.Height) / 2;
        Player->MidFront = (Player->TireRR.Height + Player->TireRL.Height) / 2;
    }
    Player->BumpStatus = (u8) CheckStatus(Player->Bump.LastZX);
    Player->TireRR.Status = Player->BumpStatus;
    Player->TireRL.Status = Player->BumpStatus;
    Player->TireFR.Status = Player->BumpStatus;
    Player->TireFL.Status = Player->BumpStatus;
    var_f20 = (CharacterSize[Player->Kart] * 18) + 1;
    temp_f0 = (Player->MidRight - Player->MidLeft);
    Player->Kartroll = -Atant(temp_f0 / var_f20);
    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        temp_f0 = (Player->MidFront - Player->MidRear);
        ChaseSnumber(&Player->Kartpitch, Atant(temp_f0 / var_f20), 0.5f);
    } else {
        temp_f0 = Player->OldPosition[1] - y;
        temp_v0 = Atant(temp_f0 / var_f20);
        if (temp_f0 >= 0.0f) {
            var_a1 = temp_v0 * 2;
        } else {
            var_a1 = temp_v0 * 0xA;
        }
        ChaseSnumber(&Player->Kartpitch, var_a1, 0.5f);
    }
    if (CheckShadow(Player->Bump.LastZX) != 0) {
        Player->TireRR.Dummy |= 1;
    } else {
        Player->TireRR.Dummy &= ~1;
    }
    if (Player->BumpStatus == BOOST_RAMP_ASPHALT) {
        if (((Player->SlipFlag & DASH_JUMP) != DASH_JUMP) &&
            ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
            Player->Weapon |= PRESS_BOOSTPAD;
        }
    }
    if (Player->BumpStatus == BOOST_RAMP_WOOD) {
        if (((Player->SlipFlag & DASH_MIDJUMP) != DASH_MIDJUMP) &&
            ((Player->SlipFlag & N_JUMP) != N_JUMP)) {
            Player->Weapon |= PRESS_BOOSTJUMP;
        }
    }
}

// Near identical to BumpVelocity in memory.c
void HitVelocity(Vec3f arg0, f32 arg1, Vec3f Arg2, f32 Arg3, f32 arg4) {
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

    temp_f0 = Arg2[0];
    temp_f2 = Arg2[1];
    temp_f12 = Arg2[2];
    temp_f14 = -arg0[0];
    temp_f16 = -arg0[1];
    temp_f18 = -arg0[2];
    temp_f20 = (temp_f14 * temp_f0) + (temp_f16 * temp_f2) + (temp_f18 * temp_f12);
    tmp1 = temp_f0 - (temp_f20 * temp_f14);
    tmp2 = temp_f2 - (temp_f20 * temp_f16);
    tmp3 = temp_f12 - (temp_f20 * temp_f18);
    if (arg1 < -arg4) {
        Arg2[0] = tmp1 - (temp_f20 * temp_f14 * Arg3);
        Arg2[1] = tmp2 - (temp_f20 * temp_f16 * Arg3);
        Arg2[2] = tmp3 - (temp_f20 * temp_f18 * Arg3);
    } else {
        Arg2[0] = tmp1;
        Arg2[1] = tmp2;
        Arg2[2] = tmp3;
    }
}

void SetStartDush(Player* Player, s8 playerIndex) {
    Player->SlipFlag |= TURBO;
    Player->Weapon &= ~USE_STARTDASH;
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(0U, (Player->Kart * 0x10) + 0x29008001);
        NAPlyTrgStart(playerIndex, 0x1900A40BU);
    }
    Player->TurboTimer = 0x0050;
}

void DriftTurbo(Player* Player, s8 playerIndex) {
    if (((Player->SlipFlag & DRIFT_TURBO) != DRIFT_TURBO) &&
        ((Player->SlipFlag & DRIFT) != DRIFT) && (Player->PowerstackCount >= 2)) {
        Player->SlipFlag |= DRIFT_TURBO;
        Player->DriftTurboTimer = 0;
        Player->PowerstackCount = 0;
        Player->StopsetrrCount = 0;
        if (ReplayFlag != 1) {
            if ((Player->Flag & IS_PLAYER) && !(Player->Flag & IS_GHOST)) {
                NaPlyDashStart(playerIndex);
            }
        } else if (Player == PlayerOne) {
            NaPlyDashStart(playerIndex);
        }
    } else if ((Player->SlipFlag & DRIFT_TURBO) == DRIFT_TURBO) {
        Player->DriftTurboTimer += 1;
        if (Player->DriftTurboTimer >= 0x1F) {
            Player->DriftTurboTimer = 0;
            Player->SlipFlag &= ~DRIFT_TURBO;
            Player->PowerstackCount = 0;
            Player->StopsetrrCount = 0;
        }
    }
}

void CheckDriftTurbo(Player* Player, s8 playerIndex) {
    if (((s16) Player->Slipang / DEGREES(1)) > 0) {
        if (((s32) Player->Nowsterr >> 16) <= -10) {
            if (Player->StopsetrrCount <= 100) {
                Player->StopsetrrCount++;
            }
            if ((Player->StopsetrrCount == 100) && (Player->Flag & IS_PLAYER)) {
                NAPlyTrgStart(playerIndex, 0x1900851EU);
            }
        } else {
            if ((Player->StopsetrrCount >= 18) && (Player->StopsetrrCount < 100)) {
                if (Player->PowerstackCount < 3) {
                    Player->PowerstackCount++;
                }
            }
            if ((Player->StopsetrrCount >= 10) && (Player->StopsetrrCount < 100)) {
                Player->StopsetrrCount = 10;
            } else {
                Player->StopsetrrCount = 0;
                Player->PowerstackCount = 0;
            }
        }
    } else if (((s32) Player->Nowsterr >> 16) >= 10) {
        if (Player->StopsetrrCount <= 100) {
            Player->StopsetrrCount++;
        }
        if ((Player->StopsetrrCount == 100) && (Player->Flag & IS_PLAYER)) {
            NAPlyTrgStart(playerIndex, 0x1900851EU);
        }
    } else {
        if ((Player->StopsetrrCount >= 18) && (Player->StopsetrrCount < 100)) {
            if (Player->PowerstackCount < 3) {
                Player->PowerstackCount++;
            }
        }
        if ((Player->StopsetrrCount >= 10) && (Player->StopsetrrCount < 100)) {
            Player->StopsetrrCount = 10;
        } else {
            Player->StopsetrrCount = 0;
            Player->PowerstackCount = 0;
        }
    }
}

void JumpSet(Player* Player) {
    Player->JmpAcc = gKartHopJerkTable[Player->Kart];
    Player->JmpSpeed = 0.0f;
    Player->Jump = gKartHopInitialVelocityTable[Player->Kart];
    Player->SlipFlag |= D_JUMP;
    Player->sus.JumpFirstspeed = 3.0f;
    Player->Gravity = 500.0f;
    DriftSet(Player);
}

/**
  * Function: DriftJump

  * Parameters:
  *     Player *Player - A Pointer to a Player

  * First kartHopJerk is subtracted from kartHopAcceleration
  * Then kartHopAcceleration is added to kartHopVelocity.

  * If kartHopVelocity is less than or equal to zero, all three
  * values (kartHop{Jerk,Acceleration,Velocity}) are set to 0.0f,
  * thereby ending the rising portion of the hop.

  * kartHopVelocity is restricted to values in [-INF, 15.0f]

  * kartHopAcceleration is restricted to values in [-9.0f, 9.0f]
**/
void DriftJump(Player* Player) {
    Player->JmpSpeed -= Player->JmpAcc;
    if (Player->JmpSpeed >= 9.0f) {
        Player->JmpSpeed = 9.0f;
    }

    if (Player->JmpSpeed <= -9.0f) {
        Player->JmpSpeed = -9.0f;
    }

    Player->Jump += Player->JmpSpeed;
    if (Player->Jump >= 15.0f) {
        Player->Jump = 15.0f;
    }

    if (Player->Jump <= 0.0f) {
        Player->JmpAcc = 0.0f;
        Player->JmpSpeed = 0.0f;
        Player->Jump = 0.0f;
    }
}

void AddGravity(Player* Player) {
    UNUSED s32 pad[2];
    if (((Player->SlipFlag & N_JUMP) != N_JUMP) && (Player->Force > 0.0f)) {
        if (((Player->Kartpitch / DEGREES(1)) < -1) && ((Player->Kartpitch / DEGREES(1)) >= -0x14) &&
            (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
            ChaseFnumber(&Player->Gravity, 500.0f, 1.0f);
            ChaseFnumber(&Player->sus.JumpFirstspeed, 3.0f, 0.05f);
        } else {
            ChaseFnumber(&Player->Gravity, Gravity1[Player->Kart], 0.1f);
            ChaseFnumber(&Player->sus.JumpFirstspeed, 1.0f, 0.07f);
        }
    } else {
        if (Player->Bump.DistanceZX >= 50.0f) {
            Player->sus.JumpFirstspeed = 2.0f;
        }
        ChaseFnumber(&Player->Gravity, Gravity1[Player->Kart], 0.02f);
        if ((Player->SlipFlag & D_JUMP) == D_JUMP) {
            ChaseFnumber(&Player->sus.JumpFirstspeed, 1.0f, 0.07f);
        } else {
            ChaseFnumber(&Player->sus.JumpFirstspeed, 1.0f, 0.07f);
        }
    }
    if ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP) {
        ChaseFnumber(&Player->sus.JumpFirstspeed, 20.0f, 1.0f);
        Player->Gravity = 3500.0f;
    }
    if ((Player->SlipFlag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        ChaseFnumber(&Player->sus.JumpFirstspeed, 25.0f, 1.0f);
        Player->Gravity = 1800.0f;
    }
    if ((Player->SlipFlag & ROLLOVER) == ROLLOVER) {
        Player->Gravity = 1100.0f;
    }
    if (Player->SlipFlag & WING) {
        Player->Gravity = 1500.0f;
    }
    if ((Player->HandlingFlag & TROPHYJUMP_END) != 0) {
        Player->Gravity = 1900.0f;
    }
    if ((Player->SlipFlag & STORM) == STORM) {
        Player->Gravity = 300.0f;
    }
    if ((Player->SlipFlag & EXPLODE) == EXPLODE) {
        Player->Gravity = 550.0f;
    }
    if ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) {
        Player->Gravity = 800.0f;
    }
}

UNUSED void SetNatureVector(void) {
}

UNUSED void CheckNatureVector(void) {
}

UNUSED void CheckDirection(void) {
}

void SetSlipAngle(Player* Player, s8 arg1, f32 Arg2, f32 Arg3, f32 arg4, f32 arg5) {
    UNUSED s32 pad[4];
    s16 temp_v0_3;
    f32 sp28;
    f32 temp_f16;
    s16 temp_a0;
    s32 var_v1;

    sp28 = (SinT(-Player->Direction[1]) * Player->Speed) + Arg2;
    temp_f16 = (CosT(-Player->Direction[1]) * Player->Speed) + Arg3;
    if (((Player->SlipFlag & WHEELSPIN) != WHEELSPIN) &&
        ((Player->SlipFlag & DRIFT) != DRIFT) && !(Player->HandlingFlag & SPINOUT_SWERVE) &&
        ((((Player->Speed / 18.0f) * 216.0f) <= 8.0f) ||
         (((Player->Nowsterr >> 16) < 5) && ((Player->Nowsterr >> 16) > -5)))) {
        if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
            Player->Slipang = (f32) (Player->Slipang - (Player->Slipang / 10));
        } else {
            temp_v0_3 = Player->Slipang;
            Player->Slipang = Player->Sterrangle * 9;
            temp_a0 = Player->Slipang - temp_v0_3;
            Player->Slipang = (f32) (temp_v0_3 + (temp_a0 / 15));
        }
    } else {
        temp_v0_3 = Player->Slipang;
        if (SterrCounter[arg1] & 8) {
            var_v1 = 2;
        } else {
            var_v1 = 0;
        }
        if ((Player->Accelcount >= 200.0f) && (var_v1 == 2) &&
            (((Player->Slipang / DEGREES(1)) >= 0x10) || ((Player->Slipang / DEGREES(1)) < -0xF))) {
            Player->Slipang = Atan2t(Arg2 - arg4, Arg3 - arg5) - Atan2t(Arg2 - sp28, Arg3 - temp_f16);
        } else {
            Player->Slipang = (Atan2t(Arg2 - arg4, Arg3 - arg5) - Atan2t(Arg2 - sp28, Arg3 - temp_f16)) * 2;
        }
        if (((Player->SlipFlag & DRIFT) != DRIFT) &&
            ((((Player->Nowsterr >> 16) > 0) && (Player->Slipang < 0)) ||
             (((Player->Nowsterr >> 16) < 0) && (Player->Slipang > 0)))) {
            if (Player->Slipang > 0) {
                Player->Slipang = Player->Sterrangle * 0x14;
            }
            if (Player->Slipang < 0) {
                Player->Slipang = Player->Sterrangle * 0x14;
            }
            temp_a0 = Player->Slipang - temp_v0_3;
            Player->Slipang = (f32) (temp_v0_3 + (temp_a0 / 12));
        } else {
            if (Player->Slipang >= 0x1C71) {
                Player->Slipang = 0x1C70;
            }
            if (Player->Slipang < -0x1C70) {
                Player->Slipang = -0x1C70;
            }
            temp_a0 = Player->Slipang - temp_v0_3;
            Player->Slipang = (f32) (temp_v0_3 + (temp_a0 / 12));
        }
    }
}

void EnemyCheckdrift(Player* Player) {
    u16 someIndex;
    u16 sp38[10] = { 0x0003, 0x0016, 0x0026, 0x003c, 0x0050, 0x0069, 0x0090, 0x009d, 0x00a9, 0x00cc };
    u16 sp24[10] = { 0x000c, 0x0021, 0x002f, 0x0045, 0x005f, 0x007a, 0x0098, 0x00a5, 0x00b3, 0x00d5 };

    for (someIndex = 0; someIndex < 10; someIndex++) {
        if (Player->Point == sp38[someIndex]) {
            Player->SlipFlag |= DRIFT;
            JumpSet(Player);
            Player->DriftCount = 0;
            break;
        }

        if (Player->Point == sp24[someIndex]) {
            Player->SlipFlag &= ~DRIFT;
            break;
        }
    }
}

void SetWeapon(Player* Player, s8 PlayerId, UNUSED s8 screenId) {
    if ((Player->Weapon & HIT_REDSHELL) == HIT_REDSHELL) {
        BombThrowRolloverWrap(Player, PlayerId);
    }
    if ((Player->Weapon & HIT_GREENSHELL) == HIT_GREENSHELL) {
        RolloverWrap(Player, PlayerId);
    }
    if ((Player->Weapon & HIT_BANANA) == HIT_BANANA) {
        WheelSpinWrap(Player, PlayerId);
    }
    if ((Player->Weapon & USE_MUSHROOM) == USE_MUSHROOM) {
        SetTurbo(Player, PlayerId);
    }
    if ((Player->Weapon & USE_STARTDASH) == USE_STARTDASH) {
        SetStartDush(Player, PlayerId);
    }
    if ((Player->Weapon & USE_FEATHER) == USE_FEATHER) {
        SetWing(Player, PlayerId);
    }
    if ((Player->Weapon & HIT_TORNADO) == HIT_TORNADO) {
        SetStorm(Player, PlayerId);
    }
    if ((Player->Weapon & PRESS_SQUISH) == PRESS_SQUISH) {
        BrokenWrap(Player, PlayerId);
    }
    if ((Player->Weapon & HIT_LIGHTNING) == HIT_LIGHTNING) {
        ThunderWrap(Player, PlayerId);
    }
    if ((Player->Weapon & HIT_CRITTER) == HIT_CRITTER) {
        SpinWrap(Player, PlayerId);
    }
    if ((Player->Weapon & HIT_BOMB) == HIT_BOMB) {
        BombRolloverWrap(Player, PlayerId);
    }
    if ((Player->Weapon & HIT_BOMB_THROW) == HIT_BOMB_THROW) {
        BombThrowRolloverWrap(Player, PlayerId);
    }
    if ((Player->Weapon & PRESS_BOOSTPAD) == PRESS_BOOSTPAD) {
        SetDashjump(Player, PlayerId);
    }
    if ((Player->Weapon & PRESS_BOOSTJUMP) == PRESS_BOOSTJUMP) {
        SetDashMidjump(Player, PlayerId);
    }
    if ((Player->Weapon & USE_STAR) == USE_STAR) {
        SetStar(Player, PlayerId);
    }
    if ((Player->Weapon & USE_BOO) == USE_BOO) {
        SetVSGhost(Player, PlayerId);
    }
    if (Player->Weapon & PRESS_SAVESPIN) {
        ProWheelSpinWrap(Player, PlayerId);
    }
    if (Player->Weapon & HIT_BOAT) {
        BombRolloverWrap(Player, PlayerId);
    }
}

void CheckWeapon(Player* Player, UNUSED s8 PlayerId, UNUSED s8 screenId) {
    if (((Player->JugemuFlag & LAKITU_SCENE) != 0) || ((Player->JugemuFlag & ON_LAKITU_ROD) != 0)) {
        Player->Weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // Green shell
    if ((Player->SlipFlag & ROLLOVER) == ROLLOVER) {
        Player->Weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // Spinout (banana or driving)
    if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
        ((Player->SlipFlag & SPIN_R) == SPIN_R)) {
        Player->Weapon &=
            (ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS)) |
            HIT_TORNADO;
    }
    // Near spinout (banana)
    if ((Player->SlipFlag & WHEELSPIN) == WHEELSPIN) {
        Player->Weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    if ((Player->HandlingFlag & SPINOUT_SWERVE) != 0) {
        Player->Weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // unclear
    if ((Player->SlipFlag & WING) == WING) {
        Player->Weapon &= ALL_TRIGGERS & ~((HIT_TRIGGERS ^ HIT_LIGHTNING) | ANY_BOOST_TRIGGERS |
                                             RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // unclear
    if ((Player->SlipFlag & STORM) == STORM) {
        Player->Weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // squished
    if ((Player->SlipFlag & BROKEN) == BROKEN) {
        Player->Weapon &= (ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS |
                                              STATE_TRANSITION_TRIGGERS)) |
                            PRESS_SQUISH;
    }
    // explosion crash
    if ((Player->SlipFlag & EXPLODE) == EXPLODE) {
        Player->Weapon &= ALL_TRIGGERS & ~((HIT_TRIGGERS ^ HIT_LIGHTNING) | ANY_BOOST_TRIGGERS |
                                             RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // hit by Star or red shell
    if ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) {
        Player->Weapon &= ALL_TRIGGERS & ~((HIT_TRIGGERS ^ HIT_LIGHTNING) | ANY_BOOST_TRIGGERS |
                                             RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // boost asphalt
    if ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP) {
        Player->Weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // boost ramp
    if ((Player->SlipFlag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        Player->Weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // Terrain tumble
    if ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL) {
        Player->Weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // Star
    if ((Player->SlipFlag & STAR) == STAR) {
        Player->Weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | USE_MUSHROOM | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // boo
    if ((Player->SlipFlag & TERESA) == TERESA) {
        Player->Weapon &= ALL_TRIGGERS & ~(HIT_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
    // early Start spinout
    if ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) {
        Player->Weapon &= ALL_TRIGGERS & ~(ANY_BOOST_TRIGGERS | HIT_CRITTER | STATE_TRANSITION_TRIGGERS);
    }
    // CPU_FAST_EFFECTS
    if ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN) {
        Player->Weapon &=
            ALL_TRIGGERS & ~(HIT_TRIGGERS | ANY_BOOST_TRIGGERS | RACING_SPINOUT_TRIGGERS | STATE_TRANSITION_TRIGGERS);
    }
}

void WeaponStatus(Player* Player, s8 PlayerId, s8 screenId) {
    if (Player->Weapon != 0) {
        CheckWeapon(Player, PlayerId, screenId);
    }
    if (Player->Weapon != 0) {
        SetWeapon(Player, PlayerId, screenId);
    }
    if ((Player->HandlingFlag & TROPHYJUMP_START) != 0) { // can never be true
        SetResultJump(Player, PlayerId);
    }
}

UNUSED void SetHitptr(Player* player_one, Player* player_two) {
    s32 var_v1;

    // clang-format off
    /*
    if's are being done bracket-less on purpose,
    Technically only the `player_one == PlayerEight` NEEDS to be
    bracket-less that looks weird
    */
    if (player_one == PlayerOne) {   var_v1 = 0;
}
    if (player_one == PlayerTwo) {   var_v1 = 1;
}
    if (player_one == PlayerThree) { var_v1 = 2;
}
    if (player_one == PlayerFour) {  var_v1 = 3;
}
    if (player_one == PlayerFive) {  var_v1 = 4;
}
    if (player_one == PlayerSix) {   var_v1 = 5;
}
    if (player_one == PlayerSeven) { var_v1 = 6;
}
    if (player_one == PlayerEight) { var_v1 = 7;
}
    HitPtr[var_v1] = player_two;
    if (player_two == PlayerOne) {   var_v1 = 0;
}
    if (player_two == PlayerTwo) {   var_v1 = 1;
}
    if (player_two == PlayerThree) { var_v1 = 2;
}
    if (player_two == PlayerFour) {  var_v1 = 3;
}
    if (player_two == PlayerFive) {  var_v1 = 4;
}
    if (player_two == PlayerSix) {   var_v1 = 5;
}
    if (player_two == PlayerSeven) { var_v1 = 6;
}
    if (player_two == PlayerEight) { var_v1 = 7;
}
    HitPtr[var_v1] = player_one;
    // clang-format on
}

void CheckImpact(Player* Player, s8 playerIndex, UNUSED s32 Arg2) {
    f32 temp_f0;
    f32 temp_f2;
    f32 temp_f14;
    s16 temp;
    s16 temp2;

    if ((Player->HitFlag & CRITTER_TOUCH) == CRITTER_TOUCH) {
        temp_f0 = D_8018CE10[playerIndex].unk_04[0];
        temp_f2 = 0;
        temp_f14 = D_8018CE10[playerIndex].unk_04[2];
        if (sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f14 * temp_f14)) >= 6.5) {
            Player->Force /= 4;
            Player->Accelcount /= 4;
            if (!(Player->SlipFlag & SPIN_L) && !(Player->SlipFlag & SPIN_R)) {
                SetSpin(Player, playerIndex);
            }
        }
    } else {
        temp_f0 = OldVelocity[playerIndex][0] - Player->Velocity[0];
        temp_f2 = OldVelocity[playerIndex][1] - Player->Velocity[1];
        temp_f14 = OldVelocity[playerIndex][2] - Player->Velocity[2];
        if (sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f14 * temp_f14)) >= 4.2) {
            Player->Force /= 4;
            Player->Accelcount /= 4;
            if (!(Player->SlipFlag & SPIN_L) && !(Player->SlipFlag & SPIN_R)) {
                SetSpin(Player, playerIndex);
            }
        }
        temp = (-(s16) CalcDirection(Player->Position, &Player->OldPosition[0]));
        temp2 = (Player->Direction[1] - Player->Slipang);
        temp = temp - temp2;
        Player->HitSterr = temp / DEGREES(1);
    }
}

void CheckImpactWall(Player* Player, f32* arg1, f32* Arg2, UNUSED s8 Arg3, UNUSED s8 arg4, UNUSED Vec3f arg5) {
    Mat3 sp64;
    Vec3f sp58;
    Vec3f sp4C;
    s16 var_v0;
    s16 t0;
    UNUSED s32 pad;
    s16 sp30[10] = { 0, DEGREES(1), DEGREES(2), DEGREES(3), DEGREES(4), DEGREES(5), DEGREES(6), DEGREES(7), DEGREES(8), DEGREES(9) };

    if (((u16) Player->Wallhitcount) <= 0) {
        return;
    }

    if (((u16) Player->Wallhitcount) > 2) {
        return;
    }

    if (!(Player->HitFlag & 0x20)) {
        return;
    }

    MakeAlignMatrix(sp64, 0, 1, 0, (s16) 0);

    sp58[0] = *arg1;
    sp58[1] = 0;
    sp58[2] = *Arg2;

    MultipleMatrixByVector(sp58, sp64);

    sp4C[0] = Player->OldPosition[0];
    sp4C[1] = 0;
    sp4C[2] = Player->OldPosition[2];

    MultipleMatrixByVector(sp4C, sp64);

    var_v0 = -(s16) CalcDirection(sp58, sp4C);
    t0 = Player->Direction[1];
    var_v0 = 0x10000 + (t0 - var_v0);
    var_v0 /= DEGREES(1);

    if (var_v0 < 0x97 && (var_v0 > -0x97)) {
        return;
    }

    var_v0 = (Player->Nowsterr >> 16) / 6;

    if (var_v0 < 0) {
        var_v0 *= -1;
    }

    if (var_v0 >= 8) {
        var_v0 = 8;
    }

    if ((Player->Nowsterr >> 16) < 0) {
        Player->Direction[1] -= sp30[var_v0];
    } else {
        Player->Direction[1] += sp30[var_v0];
    }
}

void CheckImpactsterr(Player* Player) {
    s32 sp2C[7] = { 47 << 16, 48 << 16, 49 << 16, 50 << 16, 50 << 16, 50 << 16, 50 << 16 };
    s32 spC[8] = { 40 << 16, 44 << 16, 48 << 16, 50 << 16, 50 << 16, 50 << 16, 50 << 16, 50 << 16 };
    s16 temp_t5;

    if (Player->HitSterr >= 0) {
        if ((Player->HitSterr >= 5) && (Player->HitSterr < 30)) {
            Player->Nowsterr = sp2C[Player->HitSterr / 6];
        }
        if ((Player->HitSterr >= 30) && (Player->HitSterr < 80)) {
            Player->Nowsterr = spC[(s32) (Player->HitSterr - 30) / 12];
        }
        if (((Player->HitSterr < 80) || (Player->HitSterr > 90)) && (Player->HitSterr > 90) &&
            (Player->HitSterr <= 160)) {
            Player->Nowsterr = spC[1];
        }
    } else {
        temp_t5 = -Player->HitSterr;
        if ((Player->HitSterr < -4) && (Player->HitSterr >= -30)) {
            Player->Nowsterr = sp2C[temp_t5 / 6] * -1;
        }
        if ((Player->HitSterr < -30) && (Player->HitSterr >= -80)) {
            Player->Nowsterr = spC[(s32) (temp_t5 - 30) / 12] * -1;
        }
        if (((Player->HitSterr >= -80) || (Player->HitSterr < -90)) && (Player->HitSterr < -90) &&
            (Player->HitSterr >= -160)) {
            Player->Nowsterr = spC[1] * -1;
        }
    }
}

void CheckSlipstream(Player* player, s8 playerIndex) {
    UNUSED s32 pad[3];
    UNUSED s32 uselessAssignment;
    s32 i;
    s32 var_a2;
    Player* playerBorrow;
    Player* players = PlayerOne;

    var_a2 = 0;

    if (((player->Speed / 18.0f) * 216.0f) < 50.0f) {
        player->SlipstreamTimer = 0;
        player->SlipFlag &= ~SLIP_STREAM;
        return;
    }
    if ((player->SlipFlag & SLIP_STREAM) == SLIP_STREAM) {
        player->SlipstreamTimer -= 1;
        if (player->SlipstreamTimer <= 0) {
            player->SlipFlag &= ~SLIP_STREAM;
        }
    } else {
        for (i = 0; i < NUM_PLAYERS; i++) {
            playerBorrow = &players[i];
            if (((player != playerBorrow) && ((playerBorrow->Flag & IS_GHOST) == 0) &&
                 (playerBorrow->Flag & EXISTS)) &&
                ((var_a2 = SlipstreamKart(player, playerBorrow->Position[0], playerBorrow->Position[1], playerBorrow->Position[2]),
                  var_a2 == 1))) {
                player->SlipstreamTimer += 1;
                if (player->SlipstreamTimer >= 0x3D) {
                    player->SlipFlag |= SLIP_STREAM;
                    if ((player->Flag & IS_GHOST) != IS_GHOST) {
                        uselessAssignment = player->Flag & IS_GHOST;
                        NAPlyVoiceStart(playerIndex, (player->Kart * 0x10) + 0x29008001);
                    }
                    if ((player->Flag & IS_GHOST) != IS_GHOST) {
                        uselessAssignment = var_a2;
                        NAPlyTrgStart(playerIndex, 0x19008011);
                    }
                }
                break;
            }
        }

        if (var_a2 == 0) {
            player->SlipstreamTimer = 0;
        }
    }
}

void DriftCounter(Player* Player) {
    if ((Player->SlipFlag & DRIFT) == DRIFT) {
        Player->DriftCount += 1;
        if (Player->DriftCount > 100) {
            Player->DriftCount = 100;
        }
    } else {
        Player->DriftCount -= 1;
        if (Player->DriftCount < 0) {
            Player->DriftCount = 0;
        }
    }
}

void LakituCheck(Player* Player, s8 PlayerId) {
    switch (CourseID) { /* irregular */
        case COURSE_YOSHI_VALLEY:
            if ((Player->Bump.DistanceZX >= 600.0f) && (OffroadFlagPlayer1[PlayerId] == 0)) {
                OffroadFlagPlayer1[PlayerId] = 1;
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            } else if (OffroadFlagPlayer1[PlayerId] == 0) {
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            } else if (!(Player->SlipFlag & N_JUMP)) {
                if (OoBCheck(Player->Bump.LastZX) == 0) {
                    OffroadFlagPlayer1[PlayerId] = 0;
                }
            }
            break;
        case COURSE_FRAPPE_SNOWLAND:
            if ((Player->BumpStatus == SNOW_OFFROAD) && (OffroadFlagPlayer1[PlayerId] == 0)) {
                OffroadFlagPlayer1[PlayerId] = 1;
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            } else if (Player->BumpStatus != SNOW_OFFROAD) {
                OffroadFlagPlayer1[PlayerId] = 0;
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            if (((Player->SlipFlag & DASH_JUMP) != 0) && (OffroadFlagPlayer1[PlayerId] == 0)) {
                OffroadFlagPlayer1[PlayerId] = 1;
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            } else if (((Player->SlipFlag & DASH_JUMP) == 0) && !(Player->SlipFlag & N_JUMP)) {
                OffroadFlagPlayer1[PlayerId] = 0;
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            }
            break;
        case COURSE_RAINBOW_ROAD:
            if ((Player->Bump.DistanceZX >= 600.0f) && (OffroadFlagPlayer1[PlayerId] == 0)) {
                OffroadFlagPlayer1[PlayerId] = 1;
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            } else if (OffroadFlagPlayer1[PlayerId] == 0) {
                PlayerPathPointCopy[PlayerId] = NearestPathPointByPlayerId[PlayerId];
                JugemuLine[PlayerId] = CurrentPathID[PlayerId];
            } else if (!((Player->SlipFlag & N_JUMP) || (Player->JugemuFlag & IS_IN_WATER))) {
                OffroadFlagPlayer1[PlayerId] = 0;
            }
            break;
        default:
            OffroadFlagPlayer1[PlayerId] = 0;
            if (1) {}
            break;
    }
}

void CheckSplash(Player* Player, s8 playerIndex) {
    WaterlevelPlayer[playerIndex] = CheckWaterLevel(Player);
    if (Player->Position[1] <= WaterlevelPlayer[playerIndex]) {
        Player->WaterFlag |= HALF_SUBMERGED;
    } else {
        Player->WaterFlag &= ~HALF_SUBMERGED;
    }
    if (Player->Radius < (WaterlevelPlayer[playerIndex] - Player->Position[1])) {
        Player->WaterFlag |= SUBMERGED;
        Player->WaterFlag &= ~HALF_SUBMERGED;
    } else {
        Player->WaterFlag &= ~SUBMERGED;
    }
    if (Player->Radius < (WaterlevelPlayer[playerIndex] - Player->Position[1])) {
        if ((Player->WaterFlag & SPLASH_START) != SPLASH_START) {
            Player->WaterFlag |= SPLASH_DIVE;
            Player->WaterFlag |= SPLASH_START;
            if ((CourseID != COURSE_KOOPA_BEACH) && (CourseID != COURSE_SKYSCRAPER) &&
                (CourseID != COURSE_RAINBOW_ROAD) && ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
                if ((CourseID == COURSE_BOWSER_CASTLE) || (CourseID == COURSE_BIG_DONUT)) {
                    NAPlyTrgStart((u8) playerIndex, 0x1900801CU);
                } else {
                    NAPlyTrgStart((u8) playerIndex, 0x19008008U);
                }
            }
        }
    }
    if ((CourseID == COURSE_KOOPA_BEACH) || (CourseID == COURSE_SKYSCRAPER) ||
        (CourseID == COURSE_RAINBOW_ROAD)) {
        Player->WaterFlag &= ~(SPLASH_DIVE | SPLASH_START);
    }
    if ((Player->Radius < (WaterlevelPlayer[playerIndex] - Player->Position[1])) &&
        (Player->Bump.DistanceZX >= 600.0f)) {
        Player->JugemuFlag |= IS_IN_WATER;
    }
    if (Player->Bump.DistanceZX >= 600.0f) {
        Player->JugemuFlag |= LAKITU_CAMERA;
    } else if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        Player->JugemuFlag &= ~LAKITU_CAMERA;
    }
    if ((Player->Flag & IS_CPU_PLAYER) &&
        ((OoBCheck(Player->Bump.LastZX) != 0) || (Player->JugemuFlag & IS_IN_WATER))) {
        if (!(Player->JugemuFlag & ON_LAKITU_ROD) && !(Player->JugemuFlag & LAKITU_SCENE) &&
            !(Player->SlipFlag & CENTER_LINE)) {
            ExplorerSetJugemu(Player);
            ExplorerCallJugemu(Player);
        }
    }
    if ((Player->Flag & IS_CPU_PLAYER) && (Player->BumpStatus == OUT_OF_BOUNDS) && !(Player->SlipFlag & N_JUMP)) {
        ExplorerSetJugemu(Player);
        ExplorerCallJugemu(Player);
    }
    LakituCheck(Player, playerIndex);
}

void CheckKartHit(Player* Player, s8 playerIndex, s8 Arg2) {
    if ((Player->HitFlag & 1) != 1) {
        if ((Player->SlipFlag & CARHIT) == CARHIT) {
            if ((Player->SlipFlag & TURBO) != TURBO) {
                CheckImpact(Player, playerIndex, Arg2);
            }
            Player->HandlingFlag &= ~REVERSE_GEAR;
            Player->HitFlag |= 1;
            Player->HitFlag |= 8;
            if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                ShakeHitCamera(Player, 2.8f);
            }
            if ((Player->HitFlag & CRITTER_TOUCH) == CRITTER_TOUCH) {
                if ((Player->HitFlag & CRITTER_TOUCH_GATE) != CRITTER_TOUCH_GATE) {
                    Player->HitFlag |= CRITTER_TOUCH_GATE;
                    Player->HitFlag |= INSTANT_SPINOUT;
                    if (Player->SlipFlag & TURBO) {
                        ResetTurbo(Player);
                    }
                }
            }
        }
    }
    if ((Player->SlipFlag & CARHIT) == CARHIT) {
        Player->SlipFlag &= ~CARHIT;
        Player->Hitcount = 1;
        Player->HandlingFlag &= ~REVERSE_GEAR;
        return;
    }
    Player->HitFlag &= ~0x0001;
    Player->SlipFlag &= ~CARHIT;
    if (Player->Hitcount > 0) {
        Player->Hitcount += 1;
    }
    if (Player->Hitcount >= 0xA) {
        Player->Hitcount = 0;
    }
}

void CheckWall(Player* Player, s8 PlayerId, Vec3f Velocity) {
    f32 temp_f0;
    f32 var_f14;
    f32 xdist;
    f32 ydist;
    f32 zdist;

    temp_f0 = Player->Position[1] - Player->Ground;

    if (((((Player->SlipFlag & ROLLOVER_FALL) != ROLLOVER_FALL) &&
          ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP)) ||
         ((((temp_f0 >= 20.0f) || (temp_f0 < (-1.0f))) && ((Player->SlipFlag & ROLLOVER_FALL) == 0)) &&
          (Player->SlipFlag & N_JUMP)) ||
         ((Player->Bump.FlagZX == 0) && ((Player->SlipFlag & ROLLOVER_FALL) == 0))) &&
        (((Player->JugemuFlag & ON_LAKITU_ROD) == 0) || (!(Player->JugemuFlag & LAKITU_SCENE)))) {
        SetRolloverFall(Player, PlayerId);
    }
    if ((Player->HitFlag & 0x20) != 0x20) {
        if ((Player->Bump.DistanceXY < (-1.0f)) || (Player->Bump.DistanceYZ < (-1.0f))) {
            Player->Wallhitcount = 1;
        }
        Player->HitFlag |= 0x20;
    }
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) && (((Player->Speed / 18.0f) * 216.0f) > 30.0f)) {
        ShakeHitCamera(Player, 3.0f);
    }
    Player->HitFlag |= 0x10;
    Player->Wallhitcount++;
    if (Player->Wallhitcount >= 0xA) {
        Player->Wallhitcount = 0;
    }
    if ((Player->Kartpitch >= 0) && (((Player->Speed / 18.0f) * 216.0f) > 5.0f)) {
        AccelOff(Player, 18.0f);
    }
    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
        xdist = OldVelocity[PlayerId][0] - Velocity[0];
        var_f14 = OldVelocity[PlayerId][1] - Velocity[1];
        ydist = var_f14; // okay
        zdist = OldVelocity[PlayerId][2] - Velocity[2];
        var_f14 = sqrtf((xdist * xdist) + (ydist * ydist) + (zdist * zdist)) / 3;
        if (var_f14 >= 1.0) {
            var_f14 = 1.0f;
        }
        if ((var_f14 <= 0.6) && (((Player->Speed / 18.0f) * 216.0f) >= 40.0f) &&
            (!(Player->Flag & IS_GHOST))) {
            NaPlyCrushStart(PlayerId, 0.6F);
        } else if (!(Player->Flag & IS_GHOST)) {
            if ((var_f14 <= 0.6) && (((Player->Speed / 18.0f) * 216.0f) < 40.0f) &&
                (((Player->Speed / 18.0f) * 216.0f) >= 10.0f)) {
                NaPlyCrushStart(PlayerId, 0.3F);
            } else {
                NaPlyCrushStart(PlayerId, var_f14);
            }
        }
    }
    if (Player->SlipFlag & TURBO) {
        ResetTurbo(Player);
        Player->Force /= 2;
    }
}

void SetStrategy(Player* Player, s8 playerIndex, s8 Arg2) {
    if (((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((Player->JugemuFlag & LAKITU_SCENE) == LAKITU_SCENE)) {
        ExplorerHangJugemu(Player, playerIndex, Arg2);
    }
    if ((Player->SlipFlag & WHEELSPIN) == WHEELSPIN) {
        Wheelspin(Player, playerIndex);
    }
    if (Player->HandlingFlag & SPINOUT_SWERVE) {
        ProWheelspin(Player, playerIndex);
    }
    if ((Player->SlipFlag & TURBO) == TURBO) {
        NowTurbo(Player);
    }
    if ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP) {
        NowDashjump(Player);
    }
    if ((Player->SlipFlag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        NowDashMidjump(Player);
    }
    if ((s32) (Player->SlipFlag & BROKEN) == BROKEN) {
        Broken(Player, playerIndex);
    }
    if ((Player->SlipFlag & THUNDER) == THUNDER) {
        Thunder(Player, playerIndex);
    }
    if ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL) {
        RolloverFall(Player, playerIndex);
    }
    if ((Player->SlipFlag & STAR) == STAR) {
        Star(Player, playerIndex);
    }
    if ((Player->SlipFlag & TERESA) == TERESA) {
        VSGhost(Player, playerIndex);
    }
    if (((Player->SlipFlag & STOPSTERR) == STOPSTERR) && (Player->StopsetrrCount >= 100)) {
        AccelOff(Player, 4.0f);
    }
    if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
        ((Player->SlipFlag & SPIN_R) == SPIN_R)) {
        Spin(Player, playerIndex);
    }
    if ((Player->SlipFlag & ROLLOVER) == ROLLOVER) {
        Rollover(Player, playerIndex);
    }
    if ((Player->SlipFlag & EXPLODE) == EXPLODE) {
        Bomb(Player, playerIndex);
    }
    if ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE) {
        BombThrow(Player, playerIndex);
    }
    if ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) {
        Rapidacc(Player, playerIndex);
    }
    if ((Player->SlipFlag & WING) == WING) {
        Wing(Player, playerIndex);
    }
    if ((Player->SlipFlag & STORM) == STORM) {
        Storm(Player, playerIndex);
        AccelOff(Player, 10.0f);
    }
    if (StartingIndicator != RACE_DONE) {
        if (Player->Weapon & PRESS_BECOME_BOMB) {
            ChangeBomb(Player, playerIndex);
        }
        if (Player->Weapon & PRESS_IS_BOMB) {
            RealBomb(Player, playerIndex);
        }
    }
    if (Player->HandlingFlag & TROPHYJUMP_END) { // never true
        ResultJump(Player, playerIndex);
    }
}

void OpeningKart(Player* Player, s8 playerIndex) {
    Vec3f sp4C;
    f32 temp_f18;
    s16 temp_t1;
    s16 temp;
    s16 temp2;
    f32 Thing0;
    UNUSED s32 pad;

    sp4C[0] = GridX[Kartrank[playerIndex]];
    sp4C[1] = 0;
    sp4C[2] = GridZ[Kartrank[playerIndex]];

    temp = -(s16) CalcDirection(Player->Position, sp4C);
    temp2 = Player->Direction[1];
    temp = (temp - temp2);

    Thing0 = 8;

    if (temp > ((s16) (Thing0 * DEGREES(1)))) {
        temp = (Thing0 * DEGREES(1));
    }
    if (temp < ((s16) (-Thing0 * DEGREES(1)))) {
        temp = (-Thing0 * DEGREES(1));
    }

    temp_t1 = (D_80165020[playerIndex] + ((s16) ((temp * 0x35) / (Thing0 * DEGREES(1))))) / 2;
    EnemyStickAngle(Player, (s16) temp_t1);
    D_80165020[playerIndex] = (s16) temp_t1;

    temp_f18 = sqrtf((sp4C[0] - Player->Position[0]) * (sp4C[0] - Player->Position[0]) +
                     (sp4C[2] - Player->Position[2]) * (sp4C[2] - Player->Position[2]));
    if (temp_f18 <= 8.0f) {
        ChaseDir(&Player->Direction[1], -DEGREES(180), DEGREES(2));
        if ((Player->Direction[1] <= (-179 * DEGREES(1))) || (Player->Direction[1] >= (179 * DEGREES(1)))) {
            Player->Flag &= ~IS_GP_OPENING;
        }
        Player->Force = 0;
        Player->Speed = 0;
        Player->PowerCont = 0;
        Player->TcolCount = 0;
        Player->Nowsterr = 0;
        Player->Velocity[0] = 0;
        Player->Velocity[1] = 0;
        Player->Velocity[2] = 0;
        Player->Slipang = 0;
        Player->Sterrangle = 0;
    } else {
        Player->Force = 1200;
    }
}

void RunKart(Player* Player, UNUSED Camera* Camera, s8 screenId, s8 PlayerId) {
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
    f32 PosX;
    f32 PosY;
    f32 PosZ;
    f32 temp2;
    s32 temp_v0_3;
    s32 temp3;
    f32 temp_f2_2;
    UNUSED s32 pad[8];
    f32 gravityX;
    f32 gravityY;
    f32 gravityZ;
    f32 surfaceDistance;
    UNUSED s32 Pad2;
    Vec3f newVelocity;
    Vec3f sp8C;
    UNUSED s32 Pad3[3];
    s32 sp7C = 0;
    UNUSED s32 pad4[6];

    KartEngineEcho(Player, PlayerId);
    DriftCounter(Player);
    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
        DriftTurbo(Player, PlayerId);
    }
    WeaponStatus(Player, PlayerId, screenId);
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_GHOST) != IS_GHOST)) {
        CheckSlipstream(Player, PlayerId);
    }
    SetStrategy(Player, PlayerId, screenId);
    if (((Player->SlipFlag & STOPSTERR) == STOPSTERR) && (Player->StopsetrrCount >= 100)) {
        sp7C = 2;
    }
    YokoG(Player, sp160);
    AddGravityEdit(Player);
    SlipCheck(Player, PlayerId);
    if (Player->HandlingFlag & REVERSE_GEAR) {
        Player->GravityXz[0] *= -1.0f;
        Player->GravityXz[2] *= -1.0f;
    }
    if ((Player->TireRL.Status == ASPHALT) && (Player->TireRR.Status == ASPHALT)) {
        gravityX = (-1 * (Player->GravityXz[0] + sp16C[0])) +
               ((-Player->Bump.BumpZX[0] * Player->Gravity) * 0.925);
        gravityY = (-Player->Bump.BumpZX[1] * Player->Gravity);
        gravityZ = (-1 * (Player->GravityXz[2] + sp16C[2])) +
               ((-Player->Bump.BumpZX[2] * Player->Gravity) * 0.925);
    } else {
        temp3 = (((Player->Speed / 18.0f) * 216.0f) / 10.0f);
        if (temp3 >= 10) {
            temp3 = 10;
        }
        gravityX = -1 * (Player->GravityXz[0] + sp16C[0]) +
                   ((-Player->Bump.BumpZX[0] * Player->Gravity) * sp104[temp3]);
        gravityY = (-Player->Bump.BumpZX[1] * Player->Gravity);
        gravityZ = -1 * (Player->GravityXz[2] + sp16C[2]) +
                   ((-Player->Bump.BumpZX[2] * Player->Gravity) * sp104[temp3]);
    }
    if (((Player->SlipFlag & N_JUMP) != N_JUMP) &&
        ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN)) {
        gravityX = 0 * (Player->GravityXz[0] + sp16C[0]);
        gravityY = -1 * Player->Gravity / 4;
        gravityZ = 0 * (Player->GravityXz[2] + sp16C[2]);
    }
    if ((Player->SlipFlag & N_JUMP) == N_JUMP) {
        gravityX = 0 * (Player->GravityXz[0] + sp16C[0]);
        gravityY = -1 * Player->Gravity;
        gravityZ = 0 * (Player->GravityXz[2] + sp16C[2]);
    }
    temp_f2_2 = ((Player->OldPosition[2] - Player->Position[2]) * CosT(Player->Direction[1] + Player->Slipang)) +
                (-(Player->OldPosition[0] - Player->Position[0]) * SinT(Player->Direction[1] + Player->Slipang));
    if (temp_f2_2 > 0.1) {
        Player->HandlingFlag |= REVERSE_ACCEL;
    } else {
        Player->HandlingFlag &= ~REVERSE_ACCEL;
    }
    if (((Player->Force <= 0.0f) &&
         ((temp_v0_3 = Player->SlipFlag, (temp_v0_3 & BREAKE) == BREAKE))) &&
        ((temp_v0_3 & SPIN_TURN) != SPIN_TURN)) {
        sp178[2] = temp_f2_2 * 4500.0f;
    } else {
        sp178[2] = 0.0f;
    }
    sp178[1] = 0.0f;
    MultipleMatrixByVector(sp178, Player->align);
    gravityX += sp178[0];
    gravityZ += sp178[2];
    CheckKartHit(Player, PlayerId, screenId);
    sp184[2] = PowerCheck(Player, PlayerId);
    MultipleMatrixByVector(sp184, Player->align);
    newVelocity[0] = Player->Velocity[0];
    newVelocity[1] = Player->Velocity[1];
    newVelocity[2] = Player->Velocity[2];
    if (((Player->Hitcount < 3) && (((s32) Player->Wallhitcount) < 3)) ||
        ((Player->SlipFlag & TURBO) == TURBO)) {

        if (((Player->Nowsterr >> 16) >= 40) || ((Player->Nowsterr >> 16) <= -40)) {

            newVelocity[0] += (((((f64) ((sp184[0] + gravityX) + sp160[0])) -
                                 (newVelocity[0] * (0.12 * ((f64) Player->Mass)))) /
                                6000.0) /
                               (((((f64) Player->BrakeTime) * 0.6) + 1.0) + sp7C));
            newVelocity[2] += (((((f64) ((sp184[2] + gravityZ) + sp160[2])) -
                                 (newVelocity[2] * (0.12 * ((f64) Player->Mass)))) /
                                6000.0) /
                               (((((f64) Player->BrakeTime) * 0.6) + 1.0) + sp7C));
        } else {
            newVelocity[0] += (((((f64) ((sp184[0] + gravityX) + sp160[0])) -
                                 (newVelocity[0] * (0.12 * ((f64) Player->Mass)))) /
                                6000.0) /
                               (sp7C + 1));
            newVelocity[2] += (((((f64) ((sp184[2] + gravityZ) + sp160[2])) -
                                 (newVelocity[2] * (0.12 * ((f64) Player->Mass)))) /
                                6000.0) /
                               (sp7C + 1));
        }
    } else {
        newVelocity[0] +=
            (((((f64) ((sp184[0] + gravityX) + sp160[0])) - (newVelocity[0] * (0.12 * ((f64) Player->Mass)))) /
              6000.0) /
             30.0);
        newVelocity[2] +=
            (((((f64) ((sp184[2] + gravityZ) + sp160[2])) - (newVelocity[2] * (0.12 * ((f64) Player->Mass)))) /
              6000.0) /
             30.0);
    }
    newVelocity[1] +=
        (((((f64) ((sp184[1] + gravityY) + sp160[1])) - (newVelocity[1] * (0.12 * ((f64) Player->Mass)))) /
          6000.0) /
         ((f64) Player->sus.JumpFirstspeed));
    if (((((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
          ((Player->JugemuFlag & LAKITU_SCENE) == LAKITU_SCENE)) ||
         ((Player->SlipFlag & BROKEN) == BROKEN)) ||
        (Player->JugemuFlag & IS_IN_WATER)) {
        newVelocity[0] = 0.0f;
        newVelocity[1] = 0.0f;
        newVelocity[2] = 0.0f;
    }
    if ((Player->HandlingFlag & CPU_SIMPLE_KART) == CPU_SIMPLE_KART) {
        Player->HandlingFlag &= ~CPU_SIMPLE_KART;
    }

    PosX = Player->Position[0];
    PosY = Player->Position[1];
    PosZ = Player->Position[2];

    Player->OldPosition[0] = Player->Position[0];
    Player->OldPosition[2] = Player->Position[2];
    Player->OldPosition[1] = Player->Position[1];
    nextX = PosX + Player->Velocity[0] + D_8018CE10[PlayerId].unk_04[0];
    nextY = PosY + Player->Velocity[1];
    nextZ = PosZ + Player->Velocity[2] + D_8018CE10[PlayerId].unk_04[2];

    if (((((Player->JugemuFlag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
          ((Player->JugemuFlag & LAKITU_SCENE) != LAKITU_SCENE)) &&
         ((Player->SlipFlag & BROKEN) != BROKEN)) &&
        (!(Player->JugemuFlag & IS_IN_WATER))) {
        DriftJump(Player);
        nextY += Player->Jump;
        nextY -= 0.02;
    }
    CustomCheckBump2(&Player->Bump, Player->Radius, nextX, nextY, nextZ, Player->OldPosition[0],
                            Player->OldPosition[1], Player->OldPosition[2]);
    Player->UpVector[0] = 0.0f;
    Player->UpVector[2] = 0.0f;
    Player->UpVector[1] = 1.0f;
    if ((Player->HandlingFlag & REVERSE_GEAR) != REVERSE_GEAR) {
        MakeAlignMatrix(Player->align, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                     Player->Direction[1]);
    } else {
        MakeAlignMatrix(Player->align, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                     Player->Direction[1] + DEGREES(180));
    }
    Player->SlipFlag |= N_JUMP;
    Player->Jumpcount += 1;
    surfaceDistance = Player->Bump.DistanceZX;
    if (surfaceDistance <= 0.0f) {
        Player->SlipFlag = Player->SlipFlag & (~D_JUMP);
        Player->SlipFlag = Player->SlipFlag & (~N_JUMP);
        if (Player->Jumpcount >= 35) {
            if (Player->Jumpcount >= 0x32) {
                Player->Jumpcount = 0x32;
            }
            Player->sus.BoundFirstspeed = 3.0f;
            Player->sus.BoundTimer = 0;
            Player->Talk |= POOMP;
            if ((((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                 ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP)) &&
                ((Player->Flag & IS_GHOST) != IS_GHOST)) {

                NAPlyTrgStart(PlayerId, 0x1900A60AU);
            } else if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                       ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                NaPlyLandStart((u8) PlayerId, ((f32) Player->Jumpcount) / 35.0f);
            }
            if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                Player->HandlingFlag |= LANDING_DUST;
            }
        }
        if (((Player->Jumpcount < 0x23) && (Player->Jumpcount >= 0x1C)) && (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
            Player->sus.BoundFirstspeed = 2.8f;
            Player->sus.BoundTimer = 0;
            if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                NaPlyLandStart((u8) PlayerId, ((f32) Player->Jumpcount) / 35.0f);
            }
            if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                Player->HandlingFlag |= LANDING_DUST;
            }
        }
        if (((Player->Jumpcount < 0x1C) && (Player->Jumpcount >= 4)) && (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
            Player->sus.BoundTimer = 0;
            Player->sus.BoundFirstspeed = 1.5f;
            if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                if (((Player->Jumpcount < 0xB) && (Player->Jumpcount >= 4)) && (CourseID == COURSE_BOWSER_CASTLE)) {
                    NaPlyLandStart((u8) PlayerId, Player->Jumpcount / 14.0f);
                } else {
                    NaPlyLandStart((u8) PlayerId, Player->Jumpcount / 25.0f);
                }
            }
        }
        Player->Jumpcount = 0;
        Player->Jump = Player->Jumpcount;
    }
    surfaceDistance = Player->Bump.DistanceZX;
    if (surfaceDistance <= 0.0f) {
        CheckMapBGZXHook(Player, sp8C, newVelocity, sp178, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    surfaceDistance = Player->Bump.DistanceXY;
    if (surfaceDistance < 0.0f) {
        CheckMapBGXY(Player, sp8C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(Player, PlayerId, newVelocity);
    }
    surfaceDistance = Player->Bump.DistanceYZ;
    if (surfaceDistance < 0.0f) {
        CheckMapBGYZ(Player, sp8C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(Player, PlayerId, newVelocity);
    }
    surfaceDistance = Player->Bump.DistanceXY;
    if (surfaceDistance >= 0.0f) {
        surfaceDistance = Player->Bump.DistanceYZ;
        if (surfaceDistance >= 0.0f) {
            Player->HitFlag &= 0xFFDF;
            if (Player->Wallhitcount != 0) {
                Player->Wallhitcount++;
                if (Player->Wallhitcount >= 10) {
                    Player->Wallhitcount = 0;
                }
            }
        }
    }
    if (((!(Player->SlipFlag & N_JUMP)) && (CheckSlope(Player->Bump.LastZX) != 0)) &&
        ((Player->SlipFlag & ROLLOVER_FALL) != ROLLOVER_FALL)) {
        if ((!(Player->JugemuFlag & ON_LAKITU_ROD)) || (!(Player->JugemuFlag & LAKITU_SCENE))) {
            SetRolloverFall(Player, PlayerId);
        }
    } else if (((!(Player->SlipFlag & N_JUMP)) && (CheckSlope(Player->Bump.LastZX) == 0)) &&
               (Player->SlipFlag & ROLLOVER_FALL)) {
        ResetRolloverFall(Player, PlayerId);
    }
    Player->Ground = CalcHeight(nextX, nextY, nextZ, Player->Bump.LastZX);
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        (((ScreenSplitA == SCREEN_MODE_1P) || (ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL)) ||
         (ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        TirePosition(Player, nextX, nextY, nextZ);
    } else {
        EnemyTirePosition(Player, nextX, nextY, nextZ);
    }
    SetSlipAngle(Player, PlayerId, PosX, PosZ, nextX, nextZ);

    temp2 = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    Player->OldSpeed = Player->Speed;
    Player->Speed = sqrtf(temp2);

    if ((Player->Force <= 0.0f) && (Player->Speed <= 0.08) && (D_8018CE10[PlayerId].unk_04[0] == 0.0f) &&
        (D_8018CE10[PlayerId].unk_04[2] == 0.0f)) {
        newVelocity[0] = newVelocity[0] + (-1 * newVelocity[0]);
        newVelocity[2] = newVelocity[2] + (-1 * newVelocity[2]);
    } else {
        Player->Position[0] = nextX;
        Player->Position[2] = nextZ;
    }
    Player->Position[1] = nextY;
    if ((Player->Flag & IS_PLAYER) && (!(Player->Flag & IS_CPU_PLAYER))) {
        CheckImpactWall(Player, &nextX, &nextZ, screenId, PlayerId, newVelocity);
    }
    Player->GravityXz[0] = sp178[0];
    Player->GravityXz[2] = sp178[2];
    Player->Velocity[0] = newVelocity[0];
    Player->Velocity[1] = newVelocity[1];
    Player->Velocity[2] = newVelocity[2];
    OldVelocity[PlayerId][0] = newVelocity[0];
    OldVelocity[PlayerId][1] = newVelocity[1];
    OldVelocity[PlayerId][2] = newVelocity[2];
    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[Player->Kart] < Player->Speed) {
            temp = gKartTopSpeedTable[Player->Kart] / Player->Speed;
            Player->Velocity[0] *= temp;
            Player->Velocity[1] *= temp;
            Player->Velocity[2] *= temp;
            Player->Speed = gKartTopSpeedTable[Player->Kart];
        }
    }
    if ((Player->HandlingFlag & REVERSE_GEAR) == REVERSE_GEAR) {
        if (Player->Speed > 1) {
            temp = 1 / Player->Speed;
            Player->Velocity[0] *= temp;
            Player->Velocity[1] *= temp;
            Player->Velocity[2] *= temp;
            Player->Speed = 1;
        }
    }
    if (Player->Bump.DistanceZX >= 500.0f) {
        Player->Sterrangle = (s16) (((s16) Player->Sterrangle) / 2);
    }
    if (OKCheckSplashEnabled) {
        CheckSplash(Player, PlayerId);
    }
}

void SetHight(Player* Player) {
    s32 player_index;

    player_index = CheckKartNumber(Player);
    Player->JmpAcc = 0.0f;
    Player->JmpSpeed = 0.0f;
    Player->Jump = 0.0f;
    Player->Position[1] = CheckHight(Player->Position[0], EnemyTenukiY[player_index] + 10.0f, Player->Position[2]) +
                     Player->Radius;
    if (((Player->Position[1] - EnemyTenukiY[player_index]) > 1200.0f) ||
        ((Player->Position[1] - EnemyTenukiY[player_index]) < -1200.0f)) {
        Player->Position[1] = Player->OldPosition[1];
    }
    Player->Velocity[1] = 0.0f;
}

void SpinKart(Player* Player, UNUSED Camera* Camera, s8 screenId, s8 PlayerId) {
    Vec3f spEC = { 0.0f, 0.0f, 1.0f };
    Vec3f spE0 = { 0.0f, 0.0f, 0.0f };
    Vec3f spD4 = { 0.0f, 0.0f, 0.0f };
    f32 nextX;
    f32 nextY;
    f32 nextZ;
    f32 topSpeedMultiplier;
    UNUSED s32 pad;
    f32 PosX;
    f32 PosY;
    f32 PosZ;
    UNUSED s32 Pad2[12];
    f32 gravityX;
    f32 gravityY;
    f32 gravityZ;
    f32 surfaceDistance;
    UNUSED s32 Pad3[4];
    f32 temp;
    Vec3f newVelocity;
    Vec3f sp48;
    s16 sp46;
    WeaponStatus(Player, PlayerId, screenId);
    if ((((((((Player->SlipFlag & SPIN_L) == SPIN_L) ||
             ((Player->SlipFlag & SPIN_R) == SPIN_R)) ||
            ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC)) ||
           ((Player->SlipFlag & WING) == WING)) ||
          ((Player->SlipFlag & STORM) == STORM)) ||
         ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN)) ||
        (Player->HandlingFlag & TROPHYJUMP_END)) {
        sp46 = 1;
    } else {
        sp46 = 0;
    }
    SetStrategy(Player, PlayerId, screenId);
    AddGravityEdit(Player);
    SlipCheck(Player, PlayerId);
    if ((((Player->SlipFlag & SPIN_L) == SPIN_L) ||
         ((Player->SlipFlag & SPIN_R) == SPIN_R)) ||
        ((Player->SlipFlag & THUNDER_SPIN) == THUNDER_SPIN)) {
        gravityX =
            -1 * (Player->GravityXz[0]) + (((-Player->Bump.BumpZX[0]) * Player->Gravity) * 0.1);
        gravityY = (-Player->Bump.BumpZX[1]) * Player->Gravity;
        gravityZ =
            -1 * (Player->GravityXz[2]) + (((-Player->Bump.BumpZX[2]) * Player->Gravity) * 0.1);
    } else {
        gravityX = -1 * Player->GravityXz[0];
        gravityY = -1 * Player->Gravity;
        gravityZ = -1 * Player->GravityXz[2];
    }
    CheckKartHit(Player, PlayerId, screenId);
    if (sp46 == 1) {
        MakeAlignMatrix(Player->align, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                     (s16) ((s32) Player->Direction[1]));
        MakeAlignMatrix(Player->Align2, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                     (s16) ((s32) Player->OldDirection));
    } else {
        MakeAlignMatrix(Player->align, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                     (s16) ((s32) Player->Direction[1]));
    }
    spEC[2] = PowerCheck(Player, PlayerId);
    if (sp46 == 1) {
        MultipleMatrixByVector(spEC, Player->Align2);
    } else {
        MultipleMatrixByVector(spEC, Player->align);
    }
    newVelocity[0] = Player->Velocity[0];
    newVelocity[1] = Player->Velocity[1];
    newVelocity[2] = Player->Velocity[2];
    if ((Player->Hitcount < 3) && ((Player->Wallhitcount) < 3) &&
        ((Player->SlipFlag & ROLLOVER) != ROLLOVER) &&
        ((Player->SlipFlag & EXPLODE) != EXPLODE) &&
        ((Player->SlipFlag & THROW_EXPLODE) != THROW_EXPLODE)) {
        newVelocity[0] +=
            (((((spEC[0] + gravityX) + spD4[0])) - (newVelocity[0] * (0.12 * Player->Mass))) / 6000) /
            ((Player->BrakeTime * 5.0f) + 1.0f);
        newVelocity[2] +=
            (((((spEC[2] + gravityZ) + spD4[2])) - (newVelocity[2] * (0.12 * Player->Mass))) / 6000) /
            ((Player->BrakeTime * 5.0f) + 1.0f);
    } else {
        newVelocity[0] +=
            ((((f64) (spEC[0] + gravityX + spD4[0]) - (newVelocity[0] * (0.2 * (f64) Player->Mass))) / 6000) *
             0.08);
        newVelocity[2] +=
            ((((f64) (spEC[2] + gravityZ + spD4[2]) - (newVelocity[2] * (0.2 * (f64) Player->Mass))) / 6000) *
             0.08);
    }
    newVelocity[1] += (((((spEC[1] + gravityY) + spD4[1])) - (newVelocity[1] * (0.12 * Player->Mass))) / 6000) /
                      Player->sus.JumpFirstspeed;

    if (((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((Player->JugemuFlag & LAKITU_SCENE) == LAKITU_SCENE)) {
        newVelocity[0] = 0.0f;
        newVelocity[1] = 0.0f;
        newVelocity[2] = 0.0f;
    }
    PosX = Player->Position[0];
    PosY = Player->Position[1];
    PosZ = Player->Position[2];

    Player->OldPosition[0] = Player->Position[0];
    Player->OldPosition[1] = Player->Position[1];
    Player->OldPosition[2] = Player->Position[2];

    nextX = PosX + Player->Velocity[0] + D_8018CE10[PlayerId].unk_04[0];
    nextY = PosY + Player->Velocity[1];
    nextZ = PosZ + Player->Velocity[2] + D_8018CE10[PlayerId].unk_04[2];
    DriftJump(Player);
    nextY += Player->Jump;
    CustomCheckBump2(&Player->Bump, Player->Radius, nextX, nextY, nextZ, Player->OldPosition[0],
                            Player->OldPosition[1], Player->OldPosition[2]);
    Player->SlipFlag |= N_JUMP;
    Player->Jumpcount += 1;
    Player->UpVector[0] = 0.0f;
    Player->UpVector[2] = 0.0f;
    Player->UpVector[1] = 1.0f;
    surfaceDistance = Player->Bump.DistanceZX;
    if (surfaceDistance <= 0.0f) {
        Player->SlipFlag &= ~D_JUMP;
        Player->SlipFlag &= ~N_JUMP;
        if ((((Player->SlipFlag & ROLLOVER) != ROLLOVER) &&
             ((Player->SlipFlag & EXPLODE) != EXPLODE)) &&
            ((Player->SlipFlag & THROW_EXPLODE) != THROW_EXPLODE)) {
            if (Player->Jumpcount >= 0x1C) {
                if (Player->Jumpcount >= 0x32) {
                    Player->Jumpcount = 0x0032;
                }
                Player->sus.BoundTimer = 0;
                Player->Talk |= POOMP;
                Player->sus.BoundFirstspeed = 3.0f;
                if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                    ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                    NaPlyLandStart((u8) PlayerId, ((f32) Player->Jumpcount) / 50.0f);
                }
                if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                    Player->HandlingFlag |= LANDING_DUST;
                }
            }
            if (((Player->Jumpcount < 0x1C) && (Player->Jumpcount >= 0xA)) &&
                (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
                Player->sus.BoundFirstspeed = 2.0f;
                Player->sus.BoundTimer = 0;
                if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                    ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                    NaPlyLandStart((u8) PlayerId, ((f32) Player->Jumpcount) / 50.0f);
                }
                if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                    Player->HandlingFlag |= LANDING_DUST;
                }
            }
            Player->Jumpcount = 0;
        } else {
            if (Player->Jumpcount >= 0xA) {
                if (Player->Jumpcount >= 0x32) {
                    Player->Jumpcount = 0x0032;
                }
                if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                    ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                    NaPlyLandStart((u8) PlayerId, ((f32) Player->Jumpcount) / 20.0f);
                }
                if (Player->Jumpcount >= 0x28) {
                    Player->Jumpcount = 0x0014;
                }
                if ((Player->SlipFlag & ROLLOVER) == ROLLOVER) {
                    Player->Jumpcount /= 6.5;
                    Player->JmpAcc = 0.06f;
                    Player->JmpSpeed = 0.0f;
                } else {
                    Player->Jumpcount /= 7.5;
                    Player->JmpAcc = 0.06f;
                    Player->JmpSpeed = 0.0f;
                    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
                        Player->HandlingFlag |= LANDING_DUST;
                    }
                }
            } else {
                Player->Jumpcount = 0;
            }
        }
        Player->Jump = (f32) Player->Jumpcount;
    }
    surfaceDistance = Player->Bump.DistanceZX;
    if (surfaceDistance <= 0.0f) {
        CheckMapBGZXHook(Player, sp48, newVelocity, spE0, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    surfaceDistance = Player->Bump.DistanceXY;
    if (surfaceDistance < 0.0f) {
        CheckMapBGXY(Player, sp48, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(Player, PlayerId, newVelocity);
        AccelOff(Player, 6.0f);
    }
    surfaceDistance = Player->Bump.DistanceYZ;
    if (surfaceDistance < 0.0f) {
        CheckMapBGYZ(Player, sp48, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(Player, PlayerId, newVelocity);
        AccelOff(Player, 6.0f);
    }
    surfaceDistance = Player->Bump.DistanceXY;
    if (surfaceDistance >= 0.0f) {
        surfaceDistance = Player->Bump.DistanceYZ;
        if (surfaceDistance >= 0.0f) {
            Player->HitFlag &= 0xFFDF;
            if (Player->Wallhitcount != 0) {
                Player->Wallhitcount++;
                if ((Player->Wallhitcount) >= 0xA) {
                    Player->Wallhitcount = 0;
                }
            }
        }
    }
    if (((CheckSlope(Player->Bump.LastZX) != 0) &&
         ((Player->SlipFlag & ROLLOVER_FALL) != ROLLOVER_FALL)) &&
        (((Player->Speed / 18.0f) * 216.0f) >= 20.0f)) {
        if ((!(Player->JugemuFlag & ON_LAKITU_ROD)) || (!(Player->JugemuFlag & LAKITU_SCENE))) {
            SetRolloverFall(Player, PlayerId);
        }
    } else if (((!(Player->SlipFlag & N_JUMP)) && (CheckSlope(Player->Bump.LastZX) == 0)) &&
               (Player->SlipFlag & ROLLOVER_FALL)) {
        ResetRolloverFall(Player, PlayerId);
    }
    Player->Ground = CalcHeight(nextX, nextY, nextZ, Player->Bump.LastZX);
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        (((ScreenSplitA == SCREEN_MODE_1P) || (ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL)) ||
         (ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        TirePosition(Player, nextX, nextY, nextZ);
    } else {
        EnemyTirePosition(Player, nextX, nextY, nextZ);
    }
    SetSlipAngle(Player, PlayerId, PosX, PosZ, nextX, nextZ);
    temp = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    Player->OldSpeed = Player->Speed;
    Player->Speed = sqrtf(temp);
    if ((((Player->SlipFlag & ROLLOVER) != ROLLOVER) && (Player->Force <= 0) &&
         (Player->Speed < 0.13)) ||
        (((Player->SlipFlag & ROLLOVER) != ROLLOVER) && (Player->Force <= 0) &&
         (Player->Speed < 0.20) && ((Player->SlipFlag & BREAKE) == BREAKE))) {
        newVelocity[0] = newVelocity[0] + (-1 * newVelocity[0]);
        newVelocity[2] = newVelocity[2] + (-1 * newVelocity[2]);
    } else {
        Player->Position[0] = nextX;
        Player->Position[2] = nextZ;
    }
    Player->Position[1] = nextY;
    Player->GravityXz[0] = spE0[0];
    Player->GravityXz[2] = spE0[2];
    Player->Velocity[0] = newVelocity[0];
    Player->Velocity[1] = newVelocity[1];
    Player->Velocity[2] = newVelocity[2];
    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[Player->Kart] < Player->Speed) {
            topSpeedMultiplier = gKartTopSpeedTable[Player->Kart] / Player->Speed;
            Player->Velocity[0] *= topSpeedMultiplier;
            Player->Velocity[1] *= topSpeedMultiplier;
            Player->Velocity[2] *= topSpeedMultiplier;
            Player->Speed = gKartTopSpeedTable[Player->Kart];
        }
    }
    if (OKCheckSplashEnabled) {
        CheckSplash(Player, PlayerId);
    }
}

void RunKartSimple(Player* Player, UNUSED Camera* Camera, s8 screenId, s8 PlayerId) {
    Vec3f spF4 = { 0.0f, 0.0f, 1.0f };
    UNUSED Vec3f spE8 = { 0.0f, 0.0f, 0.0f };
    Vec3f spDC = { 0.0f, 0.0f, 0.0f };
    Vec3f spD0 = { 0.0f, 0.0f, 0.0f };
    f32 nextX;
    UNUSED s32 pad;
    f32 nextZ;
    UNUSED s32 Pad2[15];
    f32 sp84;
    UNUSED s32 Pad3;
    f32 sp7C;
    UNUSED s32 pad4[2];
    Vec3f newVelocity;
    UNUSED f32 pad5[7];
    f32 toSqrt;
    f32 topSpeedMultiplier;
    f32 nextY;
    Player->SlipFlag |= CENTER_LINE;
    Player->HandlingFlag |= CPU_SIMPLE_KART;
    nextY = EnemyTenukiY[PlayerId];
    Player->DriftCount = 0;
    Player->SlipFlag &= ~DRIFT;
    WeaponStatus(Player, PlayerId, screenId);
    SetStrategy(Player, PlayerId, screenId);
    sp84 = 0 * Player->GravityXz[0] + spDC[0];
    sp7C = 0 * Player->GravityXz[2] + spDC[2];
    Player->Hitcount = 0;
    Player->Wallhitcount = 0;
    Player->SlipFlag &= ~CARHIT;
    spF4[2] = PowerCheck(Player, PlayerId);
    MultipleMatrixByVector(spF4, Player->align);
    newVelocity[0] = Player->Velocity[0];
    newVelocity[1] = 0;
    newVelocity[2] = Player->Velocity[2];
    newVelocity[0] += (((spF4[0] + sp84) + spD0[0]) - (newVelocity[0] * (0.12 * Player->Mass))) / 6000.0;
    newVelocity[2] += (((spF4[2] + sp7C) + spD0[2]) - (newVelocity[2] * (0.12 * Player->Mass))) / 6000.0;
    Player->OldPosition[0] = Player->Position[0];
    Player->OldPosition[1] = nextY;
    Player->OldPosition[2] = Player->Position[2];
    nextX = Player->Position[0] + Player->Velocity[0];
    nextZ = Player->Position[2] + Player->Velocity[2];
    Player->Slipang = 0;
    Player->JmpAcc = 0;
    Player->JmpSpeed = 0;
    Player->Jump = 0;
    MakeAlignMatrix(Player->align, Player->UpVector[0], Player->UpVector[1], Player->UpVector[2],
                                 Player->Direction[1]);
    Player->Jumpcount = 0;
    Player->SlipFlag &= ~D_JUMP;
    Player->SlipFlag &= ~N_JUMP;
    Player->Kartpitch = 0;
    Player->Kartroll = 0;
    toSqrt = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    Player->OldSpeed = Player->Speed;
    Player->Speed = sqrtf(toSqrt);
    Player->Position[0] = nextX;
    Player->Position[2] = nextZ;
    Player->Position[1] = nextY;
    Player->GravityXz[0] = 0;
    Player->GravityXz[2] = 0;
    Player->Velocity[0] = newVelocity[0];
    Player->Velocity[1] = newVelocity[1];
    Player->Velocity[2] = newVelocity[2];
    OldVelocity[PlayerId][0] = newVelocity[0];
    OldVelocity[PlayerId][1] = newVelocity[1];
    OldVelocity[PlayerId][2] = newVelocity[2];
    if (gKartTopSpeedTable[Player->Kart] < Player->Speed) {
        topSpeedMultiplier = gKartTopSpeedTable[Player->Kart] / Player->Speed;
        Player->Velocity[0] *= topSpeedMultiplier;
        Player->Velocity[1] *= topSpeedMultiplier;
        Player->Velocity[2] *= topSpeedMultiplier;
        Player->Speed = gKartTopSpeedTable[Player->Kart];
    }
}

void Openkart(Player* Player, UNUSED Camera* Camera, UNUSED s8 screenId, s8 PlayerId) {
    Vec3f spF4 = { 0.0f, 0.0f, 1.0f };
    Vec3f spE8 = { 0.0f, 0.0f, 0.0f };
    UNUSED Vec3f spDC = { 0.0f, 0.0f, 0.0f };
    Vec3f spD0 = { 0.0f, 0.0f, 0.0f };
    // Next Position is Current Position + Current Velocity
    f32 nextX;
    f32 nextY;
    f32 nextZ;

    f32 PosX;
    f32 PosY;
    f32 PosZ;
    f32 topSpeedMultiplier;
    UNUSED s32 pad[11];
    f32 sp84;
    UNUSED s32 Pad2;
    f32 sp7C;
    f32 surfaceDistance;
    f32 sqrt;
    Vec3f newVelocity;
    Vec3f sp5C;
    UNUSED s32 Pad3[3];

    YokoG(Player, spD0);
    sp84 = Player->GravityXz[0] * 0;
    sp7C = Player->GravityXz[2] * 0;
    spF4[2] = PowerCheck(Player, PlayerId);

    MultipleMatrixByVector(spF4, Player->align);

    newVelocity[0] = Player->Velocity[0];
    newVelocity[1] = Player->Velocity[1];
    newVelocity[2] = Player->Velocity[2];

    newVelocity[0] += (((f64) (spF4[0] + sp84) - (newVelocity[0] * 780.0)) / 6500.0);
    newVelocity[2] += (((f64) (spF4[2] + sp7C) - (newVelocity[2] * 780.0)) / 6500.0);
    newVelocity[1] += (((f64) (spF4[1] + -1100.0f) - (newVelocity[1] * 780.0)) / 6500.0);

    PosX = Player->Position[0];
    PosY = Player->Position[1];
    PosZ = Player->Position[2];

    Player->OldPosition[0] = Player->Position[0];
    Player->OldPosition[1] = Player->Position[1];
    Player->OldPosition[2] = Player->Position[2];

    nextX = Player->Velocity[0] + PosX;
    nextY = Player->Velocity[1] + PosY;
    nextZ = Player->Velocity[2] + PosZ;

    DriftJump(Player);

    nextY += Player->Jump;
    CustomCheckBump2(&Player->Bump, Player->Radius, nextX, nextY, nextZ, Player->OldPosition[0],
                            Player->OldPosition[1], Player->OldPosition[2]);
    Player->UpVector[0] = 0.0f;
    Player->UpVector[1] = 1.0f;
    Player->UpVector[2] = 0.0f;
    MakeAlignMatrix(Player->align, 0.0f, 1.0f, 0.0f, (s16) (s32) Player->Direction[1]);
    Player->SlipFlag &= ~N_JUMP;
    surfaceDistance = Player->Bump.DistanceZX;
    if (surfaceDistance <= 0.0f) {
        if (1) {};
        CheckMapBGZXHook(Player, sp5C, newVelocity, spE8, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    Player->Ground = CalcHeight(nextX, nextY, nextZ, Player->Bump.LastZX);
    if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((ScreenSplitA == SCREEN_MODE_1P) || (ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_VERTICAL) ||
         (ScreenSplitA == SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL))) {
        TirePosition(Player, nextX, nextY, nextZ);
    } else {
        EnemyTirePosition(Player, nextX, nextY, nextZ);
    }
    SetSlipAngle(Player, PlayerId, PosX, PosZ, nextX, nextZ);
    sqrt = (newVelocity[0] * newVelocity[0]) + (newVelocity[1] * newVelocity[1]) + (newVelocity[2] * newVelocity[2]);
    Player->OldSpeed = Player->Speed;
    Player->Speed = sqrtf(sqrt);
    if (((Player->Force <= 0.0f) && ((f64) Player->Speed < 0.13)) ||
        ((Player->Force <= 0.0f) && ((f64) Player->Speed < 0.2) &&
         ((Player->SlipFlag & BREAKE) == BREAKE))) {
        newVelocity[0] = newVelocity[0] + (newVelocity[0] * -1.0f);
        newVelocity[2] = newVelocity[2] + (newVelocity[2] * -1.0f);
    } else {
        Player->Position[0] = nextX;
        Player->Position[2] = nextZ;
    }
    Player->Position[1] = nextY - 0.018;

    Player->GravityXz[0] = spE8[0];
    Player->GravityXz[2] = spE8[2];

    Player->Velocity[0] = newVelocity[0];
    Player->Velocity[1] = newVelocity[1];
    Player->Velocity[2] = newVelocity[2];

    OldVelocity[PlayerId][0] = newVelocity[0];
    OldVelocity[PlayerId][1] = newVelocity[1];
    OldVelocity[PlayerId][2] = newVelocity[2];

    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[Player->Kart] < Player->Speed) {
            topSpeedMultiplier = gKartTopSpeedTable[Player->Kart] / Player->Speed;
            Player->Velocity[0] *= topSpeedMultiplier;
            Player->Velocity[1] *= topSpeedMultiplier;
            Player->Velocity[2] *= topSpeedMultiplier;
            Player->Speed = gKartTopSpeedTable[Player->Kart];
        }
    }
}

void SlipCheck(Player* Player, s8 playerIndex) {
    f32 var_f0;
    f32 var_f12;
    s32 temp_lo;
    s32 var_v1;

    var_f0 = 0.0f;
    if ((Player->SlipFlag & STAR) != STAR) {
        if ((s32) Player->TireRR.Status < 0xF) {
            var_f0 += D_800E2A90[Player->Kart][Player->TireRR.Status];
        }
        if ((s32) Player->TireRL.Status < 0xF) {
            var_f0 += D_800E2A90[Player->Kart][Player->TireRL.Status];
        }
        if ((s32) Player->TireFR.Status < 0xF) {
            var_f0 += D_800E2AB0[Player->Kart][Player->TireFR.Status];
        }
        if ((s32) Player->TireFL.Status < 0xF) {
            var_f0 += D_800E2AB0[Player->Kart][Player->TireFL.Status];
        }
    }
    if (SterrCounter[playerIndex] & 4) {
        var_v1 = 2;
    } else {
        var_v1 = 0;
    }
    if ((Player->Accelcount >= 200.0f) && (var_v1 == 2)) {
        temp_lo = (s16) Player->Slipang / DEGREES(1);
        if ((temp_lo > 0xF) || (temp_lo < -0xF)) {
            var_f0 += 1.0;
        }
    }
    if (((Player->SlipFlag & SLIP_STREAM) == SLIP_STREAM) && ((Player->Flag & IS_PLAYER) != IS_PLAYER)) {
        var_f0 = -3.0f;
    }

    if (Player->RGrip >= 0.0f) {
        var_f12 = Player->RGrip * var_f0;
    } else {
        var_f12 = -Player->RGrip * var_f0;
    }
    Player->AllrearGrip = Player->RGrip - var_f12;
}

void MakeTaco(Player* Player, f32 arg1) {
    f32 temp_f0_3;
    f32 var_f0;
    s16 temp_lo;
    s32 test;

    if ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) {
        Player->Taco = ((Player->Accelcount * Player->Accelcount) / 25.0f) * 1.1;
        return;
    }

    // Huh?
    if (((Player->SlipFlag & ALL_EFFECTS) & N_JUMP) == N_JUMP) {
        Player->Taco = ((Player->Accelcount * Player->Accelcount) / 25.0f) * 1.1;
        return;
    }

    var_f0 = 0.0f;
    Player->Taco = arg1;
    temp_lo = Player->Kartpitch / DEGREES(1);
    if ((temp_lo > 0x11) || (temp_lo < -0x11)) {
        var_f0 += (temp_lo * 0.0125) / 1.2;
    } else {
        var_f0 += (temp_lo * 0.025) / 1.2;
    }
    Player->Taco = arg1 * (1.0f - var_f0);
    if (Player->TireRR.Status == GRASS) {
        var_f0 += D_800E2E90[Player->Kart][Player->TireRR.Status] * 0.7;
    }
    if (Player->TireRL.Status == GRASS) {
        // The unecessary "* 1.0" here is to Force the compiler to save this 0.7 as a separate RO Value from the 0.7
        // just above this comment
        var_f0 += D_800E2E90[Player->Kart][Player->TireRL.Status] * (0.7 * 1.0);
    }
    test = Player->Slipang / DEGREES(1);
    if (test < 0) {
        var_f0 += -test * 0.004;
    } else {
        var_f0 += test * 0.004;
    }
    Player->Taco = arg1 * (1.0 + (var_f0 * 0.7));
    if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
        temp_f0_3 = Player->Accelcount + 180.0f;
        Player->Taco = (temp_f0_3 * temp_f0_3) / 25.0f;
    }
}

f32 PowerCheck(Player* Player, s8 playerIndex) {
    f32 var_f0;
    s16 temp_lo;
    f32 var_f2;
    s32 var_v0;

    var_f0 = 0.0f;
    var_f2 = (Player->Speed / 18.0f) * 216.0f;
    if (var_f2 >= 8.0f) {
        if ((Player->SlipFlag & STAR) != STAR) {
            if ((s32) Player->TireRR.Status >= 0xF) {
                // ???????
                if (1) {}
            } else {
                var_f0 += D_800E2E90[Player->Kart][Player->TireRR.Status];
            }

            if ((s32) Player->TireRL.Status < 0xF) {
                var_f0 += D_800E2E90[Player->Kart][Player->TireRL.Status];
            }

            if ((s32) Player->TireFR.Status < 0xF) {
                var_f0 += D_800E2EB0[Player->Kart][Player->TireFR.Status];
            }

            if ((s32) Player->TireFL.Status < 0xF) {
                var_f0 += D_800E2E90[Player->Kart][Player->TireFL.Status];
            }
        }
        if (((Player->SlipFlag & N_JUMP) != N_JUMP) &&
            ((Player->JugemuFlag & ON_LAKITU_ROD) != ON_LAKITU_ROD)) {
            temp_lo = Player->Kartpitch / DEGREES(1);
            if (var_f2 >= 20.0f) {
                if ((temp_lo > 0x11) || (temp_lo < -0x11)) {
                    var_f0 -= ((temp_lo * 0.0126) / 3.0);
                } else {
                    var_f0 -= ((temp_lo * 0.026) / 3.0);
                }
            } else {
                var_f0 += -0.2;
                if ((Player->SlipFlag & THUNDER) == THUNDER) {
                    var_f0 += -0.55;
                }
            }
            if (((Player->SlipFlag & DRIFT) == DRIFT) || (Player->DriftCount > 0)) {
                var_v0 = (s16) Player->Slipang / DEGREES(1);
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * 0.004;
                } else {
                    var_f0 += var_v0 * 0.004;
                }
            } else {
                var_v0 = (s16) Player->Slipang / DEGREES(1);
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * (0.01 + gKartTurnSpeedReductionTable0[Player->Kart]);
                } else {
                    var_f0 += var_v0 * (0.01 + gKartTurnSpeedReductionTable0[Player->Kart]);
                }
            }
            if (((Player->SlipFlag & STOPSTERR) == STOPSTERR) && (Player->StopsetrrCount < 10)) {
                if (var_v0 < 0) {
                    var_f0 += -var_v0 * 0.008;
                } else {
                    var_f0 += var_v0 * 0.008;
                }
            }
            if ((Player->SlipFlag & STAR) == STAR) {
                var_f0 += -0.25;
            }
        }
        if ((Player->WaterFlag & SUBMERGED) == SUBMERGED) {
            var_f0 += 0.3;
        } else {
            if ((Player->WaterFlag & HALF_SUBMERGED) == HALF_SUBMERGED) {
                var_f0 += 0.15;
            }
            if (((WaterlevelPlayer[playerIndex] - Player->TireRL.Height) >= 3.5) ||
                ((WaterlevelPlayer[playerIndex] - Player->TireRR.Height) >= 3.5)) {
                var_f0 += 0.05;
            }
        }
        if ((Player->SlipFlag & N_JUMP) != 0) {
            ChaseFnumber(&Player->PowerDown, Player->Force * 0.04, 0.15f);
        } else {
            ChaseFnumber(&Player->PowerDown, 0.0f, 0.1f);
        }
    } else {
        Player->PowerDown = 0.0f;
        Player->MaxaddPower = 0.0f;
        if (((s16) Player->Kartpitch / DEGREES(1)) < 0) {
            var_f0 += -0.85;
            if (Player->SlipFlag & THUNDER) {
                var_f0 += -0.55;
            }
        }
    }
    if ((Player->Flag & IS_PLAYER) != IS_PLAYER) {
        if ((Player->SlipFlag & SLIP_STREAM) == SLIP_STREAM) {
            ChaseFnumber(&Player->MaxaddPower, 380.0f, 0.5f);
        } else {
            ChaseFnumber(&Player->MaxaddPower, 0.0f, 0.1f);
        }
    }
    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
        if (((Player->SlipFlag & DRIFT_TURBO) == DRIFT_TURBO) &&
            ((Player->SlipFlag & DRIFT) != DRIFT)) {
            ChaseFnumber(&Player->MaxaddPower, 580.0f, 0.2f);
        } else {
            ChaseFnumber(&Player->MaxaddPower, 0.0f, 0.01f);
        }
        if ((Player->SlipFlag & SLIP_STREAM) == SLIP_STREAM) {
            ChaseFnumber(&Player->SlipstreamPower, 580.0f, 0.01f);
        } else {
            ChaseFnumber(&Player->SlipstreamPower, 0.0f, 0.01f);
        }
    }
    ChaseFnumber(&Player->PowerCont, var_f0, gKartTurnSpeedReductionTable1[Player->Kart] + 0.05);
    var_f2 = (Player->Force + Player->MaxaddPower + Player->TurboPower + Player->SlipstreamPower) - Player->PowerDown;
    if (var_f2 < 0.0f) {
        var_f2 = 0.0f;
    }
    if (((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
        ((Player->JugemuFlag & LAKITU_SCENE) == LAKITU_SCENE) ||
        ((Player->Flag & NO_CONTROLS) == NO_CONTROLS)) {
        return (1.0f - Player->PowerCont) * var_f2;
    }
    if (((Player->SlipFlag & SPIN_L) == SPIN_L) ||
        ((Player->SlipFlag & SPIN_R) == SPIN_R) ||
        ((Player->SlipFlag & EXPLODE) == EXPLODE) ||
        ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE)) {
        return (1.0f - Player->PowerCont) * var_f2;
    }
    if (((Player->SlipFlag & TURBO) == TURBO) ||
        ((Player->SlipFlag & DASH_JUMP) == DASH_JUMP) ||
        ((Player->SlipFlag & DASH_MIDJUMP) == DASH_MIDJUMP)) {
        MakeTaco(Player, Player->TurboPower + Player->Force);
        return Player->TurboPower + Player->Force;
    }
    MakeTaco(Player, var_f2);
    return (1.0f - Player->PowerCont) * var_f2;
}

void ShakeSet(Player* Player) {
    f32 var_f0;
    f32 var_f2;

    if (((Player->JugemuFlag & ON_LAKITU_ROD) != ON_LAKITU_ROD) &&
        ((Player->JugemuFlag & LAKITU_SCENE) != LAKITU_SCENE)) {
        if ((((Player->Speed / 18.0f) * 216.0f) >= 8.0f) && (Player->sus.BoundFirstspeed < 1.0f)) {
            switch (Player->BumpStatus) { /* irregular */
                case ASPHALT:
                    if (MakeRandomLimmit(0x000AU) != 8) {
                        var_f0 = 0.35f;
                        var_f2 = 0.55f;
                    } else {
                        Player->ShakeFlag = 0;
                        Player->Shake = 0.0f;
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
            Player->ShakeFlag = 0;
            Player->Shake = 0.0f;
            var_f0 = 0.0f;
            var_f2 = 0.0f;
        }
        Player->ShakeFlag += 1;
        Player->Shake = (Player->ShakeFlag * var_f0) - (0.5 * var_f2 * (Player->ShakeFlag * Player->ShakeFlag));
        if ((Player->ShakeFlag != 0) && (Player->Shake < 0.0f)) {
            Player->ShakeFlag = 0;
        }
        if (Player->Shake <= 0.0f) {
            Player->Shake = 0.0f;
        }
    }
}

void CheckAcceleOff(Player* Player) {
    s32 playerIndex;

    if (Player == PlayerOne) {
        playerIndex = 0;
    }
    if (Player == PlayerTwo) {
        playerIndex = 1;
    }
    if (Player == PlayerThree) {
        playerIndex = 2;
    }
    if (Player == PlayerFour) {
        playerIndex = 3;
    }
    if (Player == PlayerFive) {
        playerIndex = 4;
    }
    if (Player == PlayerSix) {
        playerIndex = 5;
    }
    if (Player == PlayerSeven) {
        playerIndex = 6;
    }
    if (Player == PlayerEight) {
        playerIndex = 7;
    }
    if (PowerBandAccele[playerIndex] == false) {
        if (AcceleOnoffFlag[playerIndex] == true) {
            if ((AcceleTime[playerIndex] < 2) || (AcceleTime[playerIndex] >= 9)) {
                AcceleFlag[playerIndex] = 0;
            }
            AcceleTime[playerIndex] = 0;
            AcceleIncFlag[playerIndex] = 0;
        }
        AcceleOnoffFlag[playerIndex] = false;
        AcceleTime[playerIndex]++;
        if (AcceleTime[playerIndex] >= 9) {
            AcceleTime[playerIndex] = 9;
        }
        if ((AcceleTime[playerIndex] >= 2) && (AcceleTime[playerIndex] < 9)) {
            if (AcceleIncFlag[playerIndex] == 0) {
                AcceleFlag[playerIndex] += 1;
            }
            AcceleIncFlag[playerIndex] = 1;
        }
        if (AcceleFlag[playerIndex] == 5) {
            PowerBandAccele[playerIndex] = true;
            PowerBandTimer[playerIndex] = 120;
            AcceleFlag[playerIndex] = 0;
            AcceleTime[playerIndex] = 0;
        }
    } else {
        PowerBandTimer[playerIndex]--;
        if (PowerBandTimer[playerIndex] <= 0) {
            PowerBandAccele[playerIndex] = false;
        }
    }
}

void CheckAcceleOn(Player* Player) {
    s32 playerIndex;

    if (Player == PlayerOne) {
        playerIndex = 0;
    }
    if (Player == PlayerTwo) {
        playerIndex = 1;
    }
    if (Player == PlayerThree) {
        playerIndex = 2;
    }
    if (Player == PlayerFour) {
        playerIndex = 3;
    }
    if (Player == PlayerFive) {
        playerIndex = 4;
    }
    if (Player == PlayerSix) {
        playerIndex = 5;
    }
    if (Player == PlayerSeven) {
        playerIndex = 6;
    }
    if (Player == PlayerEight) {
        playerIndex = 7;
    }
    if (PowerBandAccele[playerIndex] == false) {
        if (AcceleOnoffFlag[playerIndex] == false) {
            if ((AcceleTime[playerIndex] < 2) || (AcceleTime[playerIndex] >= 9)) {
                AcceleFlag[playerIndex] = 0;
            }
            AcceleTime[playerIndex] = 0;
            AcceleIncFlag[playerIndex] = 0;
        }
        AcceleOnoffFlag[playerIndex] = true;
        AcceleTime[playerIndex]++;
        if (AcceleTime[playerIndex] >= 9) {
            AcceleTime[playerIndex] = 9;
        }
        if ((AcceleTime[playerIndex] >= 2) && (AcceleTime[playerIndex] < 9)) {
            if (AcceleIncFlag[playerIndex] == 0) {
                AcceleFlag[playerIndex] += 1;
            }
            AcceleIncFlag[playerIndex] = 1;
        }
        if (AcceleFlag[playerIndex] == 5) {
            PowerBandAccele[playerIndex] = true;
            PowerBandTimer[playerIndex] = 120;
            AcceleFlag[playerIndex] = 0;
            AcceleTime[playerIndex] = 0;
        }
    } else {
        PowerBandTimer[playerIndex]--;
        if (PowerBandTimer[playerIndex] <= 0) {
            PowerBandAccele[playerIndex] = false;
        }
    }
}

void AccelOn(Player* Player) {
    s32 player_index;

    player_index = CheckKartNumber(Player);
    if (PowerBandAccele[player_index] == false) {
        if ((0.0 <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.1))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][0] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.1) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.2))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][1] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.2) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.3))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][2] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.3) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.4))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][3] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.4) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.5))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][4] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.5) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.6))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][5] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.6) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.7))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][6] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.7) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.8))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][7] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.8) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.9))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][8] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
        if (((Player->AccMaxcount * 0.9) <= Player->Accelcount) && (Player->Accelcount <= (Player->AccMaxcount * 1.0))) {
            Player->Accelcount +=
                gKartAccelerationTables[Player->Kart][9] + (0.05 * (Player->Kartpitch / DEGREES(1)));
        }
    } else {
        if ((0.0 <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.1))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][0] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.1) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.2))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][1] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.2) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.3))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][2] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.3) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.4))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][3] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.4) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.5))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][4] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.5) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.6))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][5] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.6) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.7))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][6] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.7) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.8))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][7] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.8) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.9))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][8] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
        if (((Player->AccMaxcount * 0.9) <= Player->Accelcount) && (Player->Accelcount <= (Player->AccMaxcount * 1.0))) {
            Player->Accelcount +=
                (gKartAccelerationTables[Player->Kart][9] + (0.05 * (Player->Kartpitch / DEGREES(1)))) *
                gKartTripleABoost[Player->Kart];
        }
    }
    if (Player->Accelcount < 0.0f) {
        Player->Accelcount = 0.0f;
    }
    if (Player->AccMaxcount <= Player->Accelcount) {
        Player->Accelcount = Player->AccMaxcount;
    }
    if (!((Player->SlipFlag & N_JUMP)) || ((Player->SlipFlag & THUNDER))) {
        Player->Force = (Player->Accelcount * Player->Accelcount) / 25.0f;
    }
    Player->HandlingFlag |= ACCELERATE;
    // Hacky way to check for PRESS_STARTSPIN
    if ((Player->Weapon * 8) < 0) {
        SetRapidacc(Player, player_index);
        Player->Weapon &= ~PRESS_STARTSPIN;
    }
}

void AccelOff(Player* Player, f32 Speed) {
    s32 player_index;
    player_index = CheckKartNumber(Player);

    Player->Accelcount -= Speed;
    if (Player->Accelcount <= 0.0f) {
        Player->Accelcount = 0.0f;
    }
    if (Player->Speed < 0.2) {
        Player->Force = 0.0f;
    }
    if (Player->AccMaxcount <= Player->Accelcount) {
        Player->Accelcount = Player->AccMaxcount;
    }
    if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
        Player->Force = (Player->Accelcount * Player->Accelcount) / 25.0f;
    }
    Player->HandlingFlag &= ~ACCELERATE;
    // Hacky way to check for PRESS_STARTSPIN
    if ((Player->Weapon * 8) < 0) {
        SetRapidacc(Player, player_index);
        Player->Weapon &= ~PRESS_STARTSPIN;
    }
}

void CheckBrakeOff(Player* Player) {
    s32 playerIndex;

    if (Player == PlayerOne) {
        playerIndex = 0;
    }
    if (Player == PlayerTwo) {
        playerIndex = 1;
    }
    if (Player == PlayerThree) {
        playerIndex = 2;
    }
    if (Player == PlayerFour) {
        playerIndex = 3;
    }
    if (Player == PlayerFive) {
        playerIndex = 4;
    }
    if (Player == PlayerSix) {
        playerIndex = 5;
    }
    if (Player == PlayerSeven) {
        playerIndex = 6;
    }
    if (Player == PlayerEight) {
        playerIndex = 7;
    }

    if (FullBrakeAccele[playerIndex] == false) {
        if (BrakeOnoffFlag[playerIndex] == true) {
            if ((BrakeTimer[playerIndex] < 2) || (BrakeTimer[playerIndex] >= 9)) {
                BrakeFlag[playerIndex] = 0;
            }
            BrakeTimer[playerIndex] = 0;
            BrakeIncFlag[playerIndex] = 0;
        }
        BrakeOnoffFlag[playerIndex] = false;
        BrakeTimer[playerIndex]++;
        if (BrakeTimer[playerIndex] >= 9) {
            BrakeTimer[playerIndex] = 9;
        }
        if ((BrakeTimer[playerIndex] >= 2) && (BrakeTimer[playerIndex] < 9)) {
            if (BrakeIncFlag[playerIndex] == 0) {
                BrakeFlag[playerIndex]++;
            }
            BrakeIncFlag[playerIndex] = 1;
        }
        if (BrakeFlag[playerIndex] == 5) {
            FullBrakeAccele[playerIndex] = true;
            FullBrakeTimer[playerIndex] = 120;
            BrakeFlag[playerIndex] = 0;
            BrakeTimer[playerIndex] = 0;
        }
    } else {
        FullBrakeTimer[playerIndex]--;
        if (FullBrakeTimer[playerIndex] <= 0) {
            FullBrakeAccele[playerIndex] = false;
        }
    }
}

void CheckBrakeOn(Player* Player) {
    s32 playerIndex;

    if (Player == PlayerOne) {
        playerIndex = 0;
    }
    if (Player == PlayerTwo) {
        playerIndex = 1;
    }
    if (Player == PlayerThree) {
        playerIndex = 2;
    }
    if (Player == PlayerFour) {
        playerIndex = 3;
    }
    if (Player == PlayerFive) {
        playerIndex = 4;
    }
    if (Player == PlayerSix) {
        playerIndex = 5;
    }
    if (Player == PlayerSeven) {
        playerIndex = 6;
    }
    if (Player == PlayerEight) {
        playerIndex = 7;
    }

    if (FullBrakeAccele[playerIndex] == false) {
        if (BrakeOnoffFlag[playerIndex] == false) {
            if ((BrakeTimer[playerIndex] < 2) || (BrakeTimer[playerIndex] >= 9)) {
                BrakeFlag[playerIndex] = 0;
            }
            BrakeTimer[playerIndex] = 0;
            BrakeIncFlag[playerIndex] = 0;
        }
        BrakeOnoffFlag[playerIndex] = true;
        BrakeTimer[playerIndex]++;
        if (BrakeTimer[playerIndex] >= 9) {
            BrakeTimer[playerIndex] = 9;
        }
        if ((BrakeTimer[playerIndex] >= 2) && (BrakeTimer[playerIndex] < 9)) {
            if (BrakeIncFlag[playerIndex] == 0) {
                BrakeFlag[playerIndex]++;
            }
            BrakeIncFlag[playerIndex] = 1;
        }
        if (BrakeFlag[playerIndex] == 5) {
            FullBrakeAccele[playerIndex] = true;
            FullBrakeTimer[playerIndex] = 120;
            BrakeFlag[playerIndex] = 0;
            BrakeTimer[playerIndex] = 0;
        }
    } else {
        FullBrakeTimer[playerIndex]--;
        if (FullBrakeTimer[playerIndex] <= 0) {
            FullBrakeAccele[playerIndex] = false;
        }
    }
}

void BreakeOn(Player* Player) {
    s32 playerIndex;
    f32 test;
    f32 var_f2;

    var_f2 = 0.0f;
    if (Player == PlayerOne) {
        playerIndex = 0;
    }
    if (Player == PlayerTwo) {
        playerIndex = 1;
    }
    if (Player == PlayerThree) {
        playerIndex = 2;
    }
    if (Player == PlayerFour) {
        playerIndex = 3;
    }
    if (Player == PlayerFive) {
        playerIndex = 4;
    }
    if (Player == PlayerSix) {
        playerIndex = 5;
    }
    if (Player == PlayerSeven) {
        playerIndex = 6;
    }
    if (Player == PlayerEight) {
        playerIndex = 7;
    }
    Player->SlipFlag |= BREAKE;
    // This check will never be true, why is it here?
    if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
        AccelOff(Player, 1.0f);
        Player->BrakeTime = var_f2;
    } else {
        if ((s32) Player->TireRR.Status < 0xF) {
            var_f2 += D_800E3210[Player->Kart][Player->TireRR.Status];
        }
        if ((s32) Player->TireRL.Status < 0xF) {
            var_f2 += D_800E3210[Player->Kart][Player->TireRL.Status];
        }
        test = Player->OldSpeed - Player->Speed;
        if (test <= 0.0f) {
            Player->BrakeTime = 0.0f;
        } else {
            Player->BrakeTime += 0.02;
            if (Player->BrakeTime >= 2.0f) {
                Player->BrakeTime = 2.0f;
            }
        }
        if (FullBrakeAccele[playerIndex] == true) {
            if (Player->BrakeTime >= 2.0f) {
                AccelOff(Player, (1.0f - var_f2) * 5.0f);
            } else {
                AccelOff(Player, (1.0f - var_f2) * 3.0f);
            }
        } else {
            if (((Player->Speed / 18.0f) * 216.0f) <= 20.0f) {
                AccelOff(Player, (1.0f - var_f2) * 4.0f);
            }
            if (Player->BrakeTime >= 2.0f) {
                AccelOff(Player, (1.0f - var_f2) * 2.5);
            } else {
                AccelOff(Player, (1.0f - var_f2) * 1.2);
            }
        }
    }
}

void PlayerNocontacceleOn(Player* Player) {
    s32 playerIndex;
    s32 var_v0;
    s32 time_delta;

    playerIndex = CheckKartNumber(Player);
    if ((Player->Accelcount >= 0.0) && (Player->Accelcount < (Player->AccMaxcount * 0.1))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][0] * 3.0;
    }
    if (((Player->AccMaxcount * 0.1) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.2))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][1] * 3.0;
    }
    if (((Player->AccMaxcount * 0.2) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.3))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][2] * 3.0;
    }
    if (((Player->AccMaxcount * 0.3) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.4))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][3] * 3.0;
    }
    if (((Player->AccMaxcount * 0.4) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.5))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][4] * 3.0;
    }
    if (((Player->AccMaxcount * 0.5) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.6))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][5] * 3.0;
    }
    if (((Player->AccMaxcount * 0.6) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.7))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][6] * 2.5;
    }
    if (((Player->AccMaxcount * 0.7) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.8))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][7] * 2.5;
    }
    if (((Player->AccMaxcount * 0.8) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.9))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][8] * 2.5;
    }
    if (((Player->AccMaxcount * 0.9) <= Player->Accelcount) && (Player->Accelcount <= Player->AccMaxcount * 1.0)) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][9] * 2.5;
    }
    if (D_801656F0 == 1) {
        time_delta = Kwvideoframetimer - GreenTimer[playerIndex];
        if (GameMode == TIME_TRIALS) {
            var_v0 = 0x14;
        } else {
            var_v0 = 8;
        }
        if ((time_delta < var_v0) && ((Player->HandlingFlag & ACCELERATE) != ACCELERATE)) {
            Player->Weapon |= USE_STARTDASH;
        } else if ((Player->AccMaxcount * 0.9f) <= Player->Accelcount) {
            if ((Player->Weapon & USE_STARTDASH) != USE_STARTDASH) {
                Player->Weapon |= PRESS_STARTSPIN;
                Player->Weapon &= ~USE_STARTDASH;
            }
        }
    }
    Player->HandlingFlag |= ACCELERATE;
    Player->Taco = (Player->Accelcount * Player->Accelcount) / 25.0f;
}

void PlayerNocontacceleOff(Player* Player, f32 speedReduction) {
    Player->Accelcount -= speedReduction;
    if (Player->Accelcount <= 0.0f) {
        Player->Accelcount = 0.0f;
    }
    if (Player->Speed < 0.2) {
        Player->Force = 0.0f;
    }
    if (Player->AccMaxcount <= Player->Accelcount) {
        Player->Accelcount = Player->AccMaxcount;
    }
    if ((f64) Player->Accelcount <= (Player->AccMaxcount * 0.7)) {
        Player->Weapon &= ~PRESS_STARTSPIN;
    }
    Player->Weapon &= ~USE_STARTDASH;
    Player->HandlingFlag &= ~ACCELERATE;
    Player->Taco = (Player->Accelcount * Player->Accelcount) / 25.0f;
}

void JugemuAcceleOn(Player* Player) {
    UNUSED s32 player_index;

    player_index = CheckKartNumber(Player);
    if ((0.0 <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.1))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][0] * 3.2;
    }
    if (((Player->AccMaxcount * 0.1) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.2))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][1] * 3.2;
    }
    if (((Player->AccMaxcount * 0.2) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.3))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][2] * 3.2;
    }
    if (((Player->AccMaxcount * 0.3) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.4))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][3] * 3.2;
    }
    if (((Player->AccMaxcount * 0.4) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.5))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][4] * 3.2;
    }
    if (((Player->AccMaxcount * 0.5) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.6))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][5] * 3.2;
    }
    if (((Player->AccMaxcount * 0.6) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.7))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][6] * 3.2;
    }
    if (((Player->AccMaxcount * 0.7) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.8))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][7] * 2.8;
    }
    if (((Player->AccMaxcount * 0.8) <= Player->Accelcount) && (Player->Accelcount < (Player->AccMaxcount * 0.9))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][8] * 2.8;
    }
    if (((Player->AccMaxcount * 0.9) <= Player->Accelcount) && (Player->Accelcount <= (Player->AccMaxcount * 1.0))) {
        Player->Accelcount += gKartAccelerationTables[Player->Kart][9] * 2.8;
    }
    if (Player->Accelcount < 0.0f) {
        Player->Accelcount = 0.0f;
    }
    Player->Taco = (Player->Accelcount * Player->Accelcount) / 25.0f;
}

void JugemuAcceleOff(Player* Player, f32 speedReduction) {
    Player->Accelcount -= speedReduction;
    if (Player->Accelcount <= 0.0f) {
        Player->Accelcount = 0.0f;
    }
    if (Player->AccMaxcount <= Player->Accelcount) {
        Player->Accelcount = Player->AccMaxcount;
    }
    Player->Taco = (Player->Accelcount * Player->Accelcount) / 25.0f;
}

void EmptyAcceleOn(Player* Player, s32 playerIndex) {
    if ((AcceleCount[playerIndex] >= 0.0) && (AcceleCount[playerIndex] < ((f64) Player->AccMaxcount * 0.1))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][0] * 3.2;
    }
    if (((Player->AccMaxcount * 0.1) <= AcceleCount[playerIndex]) &&
        (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.2))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][1] * 3.2;
    }
    if (((Player->AccMaxcount * 0.2) <= AcceleCount[playerIndex]) &&
        (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.3))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][2] * 3.2;
    }
    if (((Player->AccMaxcount * 0.3) <= AcceleCount[playerIndex]) &&
        (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.4))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][3] * 3.2;
    }
    if (((Player->AccMaxcount * 0.4) <= AcceleCount[playerIndex]) &&
        (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.5))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][4] * 3.2;
    }
    if (((Player->AccMaxcount * 0.5) <= AcceleCount[playerIndex]) && (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.6))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][5] * 3.2;
    }
    if (((Player->AccMaxcount * 0.6) <= AcceleCount[playerIndex]) && (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.7))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][6] * 3.2;
    }
    if (((Player->AccMaxcount * 0.7) <= AcceleCount[playerIndex]) && (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.8))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][7] * 2.8;
    }
    if (((Player->AccMaxcount * 0.8) <= AcceleCount[playerIndex]) && (AcceleCount[playerIndex] < (Player->AccMaxcount * 0.9))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][8] * 2.8;
    }
    if (((Player->AccMaxcount * 0.9) <= AcceleCount[playerIndex]) && (AcceleCount[playerIndex] <= (Player->AccMaxcount * 1.0))) {
        AcceleCount[playerIndex] += gKartAccelerationTables[Player->Kart][9] * 2.8;
    }
    Player->HandlingFlag |= ACCELERATE;
    if (AcceleCount[playerIndex] < 0.0f) {
        AcceleCount[playerIndex] = 0.0f;
    }
    Player->Taco = (AcceleCount[playerIndex] * AcceleCount[playerIndex]) / 25.0f;
}

void EmptyAcceleOff(Player* Player, f32 speedReduction, s32 playerIndex) {
    Player->HandlingFlag &= ~ACCELERATE;
    AcceleCount[playerIndex] -= speedReduction;
    if (AcceleCount[playerIndex] <= 0.0f) {
        AcceleCount[playerIndex] = 0.0f;
    }
    if (Player->AccMaxcount <= AcceleCount[playerIndex]) {
        AcceleCount[playerIndex] = Player->AccMaxcount;
    }
    Player->Taco = (AcceleCount[playerIndex] * AcceleCount[playerIndex]) / 25.0f;
}

void RecoverGrip(Player* arg0, f32 arg1) {
    arg0->Grip += arg1;
    if (arg0->Grip >= 0.0f) {
        arg0->Grip = 0.0f;
    }
}

void CheckSterrSpeedgrip(Player* Player, s32* desired_steering_change, s32* current_steering, s32 desired_steering, s32 steering_change_threshold, s32 steering_change_minimum, s32 arg6) {
    s32 current_steering_change_increment;

    // if (abs(desired_steering_change) >= steering_change_threshold)
    if ((*desired_steering_change >= steering_change_threshold) || (-steering_change_threshold >= *desired_steering_change)) {
        current_steering_change_increment = Player->ChasespSterr;
        Player->ChasespSterr -= 1 << 11;
        //signed vs unsigned weirdness, actually checking if negative
        if (Player->ChasespSterr >= 0xF0000000) {
            Player->ChasespSterr = current_steering_change_increment;
        }
        if (steering_change_minimum >= (s32) Player->ChasespSterr) {
            Player->ChasespSterr = steering_change_minimum;
        }

        // move current_steering toward desired_steering
        *current_steering = (desired_steering < *current_steering) ? *current_steering - Player->ChasespSterr : *current_steering + Player->ChasespSterr;

        //arg6 is always positive and unk_090 is 0 or negative, so this if statement is always true
        if (Player->Grip < ((f32) arg6)) {
            Player->Grip = (f32) -arg6;
        }
    }
}

UNUSED void CheckSterrSpeedgrip1(Player* Player, s32* arg1, s32 Arg2, s32 Arg3, f32 arg4) {
    u32 temp_v1;

    temp_v1 = Player->ChasespSterr;
    Player->ChasespSterr -= 0x800;
    if (!(Player->ChasespSterr < 0xF0000000)) {
        Player->ChasespSterr = temp_v1;
    }
    if (Arg3 >= (s32) Player->ChasespSterr) {
        Player->ChasespSterr = Arg3;
    }

    *arg1 = (Arg2 < *arg1) ? *arg1 - Player->ChasespSterr : *arg1 + Player->ChasespSterr;

    if (Player->Grip < arg4) {
        Player->Grip = (f32) -arg4;
    }
}

UNUSED void CheckSterrSpeed1(Player* Player, s32* arg1, s32 Arg2, s32 Arg3, f32 arg4) {
    s32 temp_v0;

    temp_v0 = Player->ChasespSterr;
    Player->ChasespSterr -= 0x800;
    if (Player->ChasespSterr >= 0xF0000000) {
        Player->ChasespSterr = temp_v0;
    }
    if (Arg3 >= (s32) Player->ChasespSterr) {
        Player->ChasespSterr = Arg3;
    }

    *arg1 = (Arg2 < *arg1) ? *arg1 - Player->ChasespSterr : *arg1 + Player->ChasespSterr;

    RecoverGrip(Player, arg4);
}

void CheckSterrSpeed(Player* Player, s32* desired_steering_change, s32* current_steering, s32 desired_steering, s32 steering_change_threshold, s32 steering_change_minimum, f32 arg6) {
    s32 current_steering_change_increment;

    if ((*desired_steering_change >= steering_change_threshold) || (-steering_change_threshold >= *desired_steering_change)) {
        current_steering_change_increment = Player->ChasespSterr;
        Player->ChasespSterr -= 1 << 11;
        if (Player->ChasespSterr >= 0xF0000000) {
            Player->ChasespSterr = current_steering_change_increment;
        }
        if (steering_change_minimum >= (s32) Player->ChasespSterr) {
            Player->ChasespSterr = steering_change_minimum;
        }

        *current_steering = (desired_steering < *current_steering) ? *current_steering - Player->ChasespSterr : *current_steering + Player->ChasespSterr;
        // identical to CheckSterrSpeedgrip before this Point

        // set Player->Grip = min(0, Player->unk_90 + arg6)
        RecoverGrip(Player, arg6);
    }
}

//On or near Ground
void ProStickAngle(Player* Player, struct Controller* controller, s8 playerIndex) {
    s32 steer_position;
    s32 clamped_x;
    UNUSED s32 pad[2];
    UNUSED s16 Pad2;
    s16 var_s1_2;
    s32 steer_position_delta;
    s32 steer_resistance_small_turn;
    s32 steer_resistance_large_turn;
    f32 var_f2_2;
    f32 var_f12 = 0.0f;
    f32 zero = 0;
    UNUSED s32 Pad3;
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
           ((Player->SlipFlag & D_JUMP) != D_JUMP) &&
           (
             ((Player->Slipang / DEGREES(1) <= 6) && (Player->Slipang / DEGREES(1) >= -6)) ||
             ((controller->ButtonHeld & R_TRIG) != R_TRIG)
           )
         ) ||
         (((Player->Speed / 18.0f) * 216.0f) <= 20.0f) ||
         ((Player->SlipFlag & CARHIT) == CARHIT)
       ) {
       DriftReset(Player);
    }
    if ((Player->Slipang / DEGREES(1)) < (-5)) {
        Player->HandlingFlag |= STEER_LEFT;
        Player->HandlingFlag &= ~STEER_RIGHT;
        SterrCounter[playerIndex]++;
    } else if ((Player->Slipang / DEGREES(1)) > 5) {
        Player->HandlingFlag |= STEER_RIGHT;
        Player->HandlingFlag &= ~STEER_LEFT;
        SterrCounter[playerIndex]++;
    } else {
        Player->HandlingFlag &= ~(STEER_LEFT | STEER_RIGHT);
        SterrCounter[playerIndex] = 0;
    }
    if (((Player->SlipFlag & D_JUMP) == D_JUMP) || ((Player->SlipFlag & DRIFT) == DRIFT)) {
        Player->HandlingFlag &= ~(STEER_LEFT | STEER_RIGHT);
    }
    steer_position = Player->Nowsterr;
    clamped_x = CheckStickX(controller);
    if (((Player->HandlingFlag & REVERSE_GEAR) == REVERSE_GEAR) || ((Player->HandlingFlag & REVERSE_ACCEL) == REVERSE_ACCEL)) {
        clamped_x = -clamped_x;
    }
    Player->Nowsterr = (clamped_x << 16) & 0xFFFF0000;
    steer_position_delta = steer_position - Player->Nowsterr; // x change
    steer_position_delta = steer_position_delta >> 16;
    Player->Sterracc = (s16) steer_position_delta;
    if (((steer_position_delta >= 90) || (steer_position_delta <= -90)) && (!(Player->HandlingFlag & SPINOUT_SWERVE))) {
        if ((((((!(Player->SlipFlag & DRIFT)) && (RaceClass == CC_150)) && (GameMode != BATTLE)) &&
              (!(Player->SlipFlag & N_JUMP))) &&
             (((Player->Speed / 18.0f) * 216.0f) >= 40.0f)) &&
            (Player->DriftCount == 0)) {
            Player->Weapon |= PRESS_SAVESPIN;
        }
    }
    if (((s32) Player->TireRR.Status) < 0xF) {
        //this Table is just zeros
        zero += D_800E3610[Player->Kart][Player->TireRR.Status];
    }
    if (((s32) Player->TireRL.Status) < 0xF) {
        zero += D_800E3610[Player->Kart][Player->TireRL.Status];
    }
    if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
        steer_resistance_large_turn = 10;
        steer_resistance_small_turn = 10;
    } else {
        if (((Player->SlipFlag & DRIFT) == DRIFT) &&
            ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
            var_a0 = 3;
        } else {
            var_a0 = 0;
        }
        if (((Player->Speed / 18.0f) * 216.0f) >= 15.0f) {
            if ((Player->HandlingFlag & STEER_RIGHT) == STEER_RIGHT) {
                if ((steer_position_delta <= 35) && (steer_position_delta >= 0)) {
                    //gKartTable800E3650 is always 0
                    steer_resistance_large_turn =
                        (gKartTable800E3650[Player->Kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero)); //18 or 15
                    steer_resistance_small_turn =
                        (gKartTable800E3650[Player->Kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero));
                } else {
                    steer_resistance_large_turn = (s32) (((f32) (var_a0 + 5)) * (1.0f + zero)); // 8 or 5
                    steer_resistance_small_turn = (s32) (((f32) (var_a0 + 9)) * (1.0f + zero)); //12 or 9
                }
            } else if ((Player->HandlingFlag & STEER_LEFT) == STEER_LEFT) {
                if ((steer_position_delta >= -35) && (steer_position_delta <= 0)) {
                    steer_resistance_large_turn =
                        (gKartTable800E3650[Player->Kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero)); // 18 or 15
                    steer_resistance_small_turn =
                        (gKartTable800E3650[Player->Kart] + 1.0f) * (((f32) (var_a0 + 15)) * (1.0f + zero));
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
    if ((Player->WaterFlag & SUBMERGED) == SUBMERGED) {
        steer_resistance_large_turn *= 1.5;
        steer_resistance_small_turn *= 1.5;
    } else {
        if ((Player->WaterFlag & HALF_SUBMERGED) == HALF_SUBMERGED) {
            steer_resistance_large_turn *= 1.2;
            steer_resistance_small_turn *= 1.2;
        }
        // Driving in water?
        if ((((f64) (WaterlevelPlayer[playerIndex] - Player->TireRL.Height)) >= 3.5) ||
            (((f64) (WaterlevelPlayer[playerIndex] - Player->TireRR.Height)) >= 3.5)) {
            steer_resistance_large_turn *= 1.05;
            steer_resistance_small_turn *= 1.05;
        }
    }

    /*
    This big group of function calls does 2 things. 
    First, when you steer it sets steer_position (which starts as just your x Input) part way between your x Input and your previous Player->Nowsterr
    (basically, you cannot go from full Left to Right steering instantaneously)
    Second, it sets the Value of Player->Grip, which has to do with your karts sideways Velocity. The Code looks like it is intended
    to set this to larger negative values when you have a large steer_position_delta. However, due to an apparent bug, CheckSterrSpeedgrip
    overwrites this too frequently. The end effect is that Player->Grip ends up always getting set to -59.85 when steer_position_delta > 30
    and set to 0 otherwise. -59.85 is -100 (the last Value provided to CheckSterrSpeedgrip) + the sum of the values provided to CheckSterrSpeed
    */
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 90, (120 << 12) / steer_resistance_large_turn, 450);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 89, (118 << 12) / steer_resistance_large_turn, 440);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 88, (116 << 12) / steer_resistance_large_turn, 430);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 87, (114 << 12) / steer_resistance_large_turn, 420);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 86, (112 << 12) / steer_resistance_large_turn, 410);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 85,  (88 << 12) / steer_resistance_large_turn, 400);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 84,  (86 << 12) / steer_resistance_large_turn, 395);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 83,  (80 << 12) / steer_resistance_large_turn, 390);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 82,  (79 << 12) / steer_resistance_large_turn, 390);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 81,  (78 << 12) / steer_resistance_large_turn, 380);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 80,  (77 << 12) / steer_resistance_large_turn, 370);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 79,  (76 << 12) / steer_resistance_large_turn, 360);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 78,  (76 << 12) / steer_resistance_large_turn, 360);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 77,  (75 << 12) / steer_resistance_large_turn, 350);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 76,  (74 << 12) / steer_resistance_large_turn, 340);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 75,  (73 << 12) / steer_resistance_large_turn, 330);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 74,  (73 << 12) / steer_resistance_large_turn, 330);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 73,  (73 << 12) / steer_resistance_large_turn, 330);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 72,  (72 << 12) / steer_resistance_large_turn, 320);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 71,  (71 << 12) / steer_resistance_large_turn, 315);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 70,  (71 << 12) / steer_resistance_large_turn, 315);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 69,  (70 << 12) / steer_resistance_large_turn, 305);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 68,  (70 << 12) / steer_resistance_large_turn, 305);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 67,  (69 << 12) / steer_resistance_large_turn, 280);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 66,  (70 << 12) / steer_resistance_large_turn, 270);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 65,  (69 << 12) / steer_resistance_large_turn, 270);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 64,  (68 << 12) / steer_resistance_large_turn, 260);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 63,  (67 << 12) / steer_resistance_large_turn, 250);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 62,  (67 << 12) / steer_resistance_large_turn, 250);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 61,  (67 << 12) / steer_resistance_large_turn, 250);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 60,  (61 << 12) / steer_resistance_large_turn, 245);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 59,  (60 << 12) / steer_resistance_large_turn, 245);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 58,  (59 << 12) / steer_resistance_large_turn, 245);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 57,  (58 << 12) / steer_resistance_large_turn, 245);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 56,  (56 << 12) / steer_resistance_large_turn, 245);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 55,  (56 << 12) / steer_resistance_large_turn, 230);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 54,  (56 << 12) / steer_resistance_large_turn, 230);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 53,  (56 << 12) / steer_resistance_large_turn, 230);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 52,  (56 << 12) / steer_resistance_large_turn, 230);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 51,  (56 << 12) / steer_resistance_large_turn, 230);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 50,  (50 << 12) / steer_resistance_large_turn, 220);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 49,  (50 << 12) / steer_resistance_large_turn, 220);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 48,  (50 << 12) / steer_resistance_large_turn, 220);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 47,  (50 << 12) / steer_resistance_large_turn, 220);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 46,  (50 << 12) / steer_resistance_large_turn, 220);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 45,  (48 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 44,  (46 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 43,  (46 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 42,  (46 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 41,  (46 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 40,  (46 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 39,  (44 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 38,  (40 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 37,  (40 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 36,  (36 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 35,  (36 << 12) / steer_resistance_large_turn, 110);
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 34,  (34 << 12) / steer_resistance_large_turn, 110); 
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 32,  (32 << 12) / steer_resistance_large_turn, 100); // this skips 33
    CheckSterrSpeedgrip(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 31,  (32 << 12) / steer_resistance_large_turn, 100);

    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 30,  (31 << 12) / steer_resistance_small_turn, 0.9f); // 0.9 * 5
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 29,  (30 << 12) / steer_resistance_small_turn, 0.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 28,  (29 << 12) / steer_resistance_small_turn, 0.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 27,  (28 << 12) / steer_resistance_small_turn, 0.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 26,  (27 << 12) / steer_resistance_small_turn, 0.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 25,  (26 << 12) / steer_resistance_small_turn, 1.0f); // 1.0 * 5
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 24,  (25 << 12) / steer_resistance_small_turn, 1.0f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 23,  (24 << 12) / steer_resistance_small_turn, 1.0f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 22,  (23 << 12) / steer_resistance_small_turn, 1.0f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 21,  (22 << 12) / steer_resistance_small_turn, 1.0f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 20,  (21 << 12) / steer_resistance_small_turn, 1.05f);// 1.05* 5
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 19,  (20 << 12) / steer_resistance_small_turn, 1.05f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 18,  (19 << 12) / steer_resistance_small_turn, 1.05f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 17,  (18 << 12) / steer_resistance_small_turn, 1.05f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 16,  (17 << 12) / steer_resistance_small_turn, 1.05f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 15,  (16 << 12) / steer_resistance_small_turn, 1.2f); // 1.2 * 5
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 14,  (15 << 12) / steer_resistance_small_turn, 1.2f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 13,  (14 << 12) / steer_resistance_small_turn, 1.2f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 12,  (13 << 12) / steer_resistance_small_turn, 1.2f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 11,  (12 << 12) / steer_resistance_small_turn, 1.2f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr, 10,  (14 << 12) / steer_resistance_small_turn, 1.6f); // 1.6 * 5
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  9,  (13 << 12) / steer_resistance_small_turn, 1.6f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  8,  (12 << 12) / steer_resistance_small_turn, 1.6f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  7,  (11 << 12) / steer_resistance_small_turn, 1.6f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  6,  (10 << 12) / steer_resistance_small_turn, 1.6f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  5,   (9 << 12) / steer_resistance_small_turn, 1.9f); // 1.9 * 6
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  4,   (8 << 12) / steer_resistance_small_turn, 1.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  3,   (7 << 12) / steer_resistance_small_turn, 1.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  2,   (6 << 12) / steer_resistance_small_turn, 1.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  1,   (5 << 12) / steer_resistance_small_turn, 1.9f);
    CheckSterrSpeed(Player, &steer_position_delta, &steer_position, Player->Nowsterr,  0,           0 / steer_resistance_small_turn, 1.9f); // sum is 40.15
    if ((Player->SlipFlag & DRIFT) == DRIFT) {
        var_f2_2 = (f32) ((steer_position >> 16) / 8);
    } else if (((Player->Speed / 18.0f) * 216.0f) <= 25.0f) {
        var_f2_2 = (f32) ((steer_position >> 16) / 12);
    } else {
        var_f2_2 = ((f32) (steer_position >> 16)) / (8.0f + (Player->Accelcount / 50.0f));
    }
    if (var_f2_2 < 0.0f) {
        var_f2_2 = -var_f2_2;
    }
    if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
        var_f2_2 = var_f2_2 * (sp44[((s16) ((Player->Speed / 18.0f) * 216.0f)) + 10] * 1.5f);
    } else if ((Player->SlipFlag & DRIFT) == DRIFT) {
        var_f2_2 = var_f2_2 * sp44[(s16) ((Player->Speed / 18.0f) * 216.0f)];
    } else {
        var_f2_2 = var_f2_2 * (sp44[(s16) ((Player->Speed / 18.0f) * 216.0f)] * 1.5f);
    }
    Player->Nowsterr = steer_position;
    if (Player->Hitcount != 0) {
        CheckImpactsterr(Player);
    }
    Player->SlipFlag &= ~STOPSTERR;
    if (((s32) Player->TireRR.Status) > 0xE) {
        var_f12 = var_f12;
    } else {
        var_f12 += D_800E3410[Player->Kart][Player->TireRR.Status];
    }
    if (((s32) Player->TireRL.Status) < 0xF) {
        var_f12 += D_800E3410[Player->Kart][Player->TireRL.Status];
    }
    if (((Player->SlipFlag & D_JUMP) != D_JUMP) && ((Player->SlipFlag & DRIFT) != DRIFT)) {
        if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
            Player->Sterrangle = (Player->Nowsterr >> 16) * 5 * var_f2_2;
        } else {
            if ((Player->SlipFlag & BREAKE) != BREAKE) {
                if (((Player->Nowsterr >> 16) >= 45) || ((Player->Nowsterr >> 16) <= -45)) {
                    Player->Sterrangle = ((Player->Nowsterr >> 16) * (var_f2_2 + (var_f2_2 * var_f12))) *
                                      (0.15 + gKartHandlingTable[Player->Kart]);
                } else {
                    Player->Sterrangle = ((Player->Nowsterr >> 16) * (var_f2_2 + (var_f2_2 * var_f12))) *
                                      gKartHandlingTable[Player->Kart];
                }
            } else {
                if ((((Player->Speed / 18.0f) * 216.0f) >= 0.0f) && (((Player->Speed / 18.0f) * 216.0f) < 8.0f)) {
                    Player->Sterrangle = (Player->Nowsterr >> 16) * (var_f2_2 + (var_f2_2 * var_f12));
                }
                if ((((Player->Speed / 18.0f) * 216.0f) >= 8.0f) && (((Player->Speed / 18.0f) * 216.0f) < 65.0f)) {
                    Player->Sterrangle = (Player->Nowsterr >> 16) * ((var_f2_2 + 1.5) + (var_f2_2 * var_f12));
                }
                if (((Player->Speed / 18.0f) * 216.0f) >= 65.0f) {
                    Player->Sterrangle = (Player->Nowsterr >> 16) * ((var_f2_2 + 1.6) + (var_f2_2 * var_f12));
                }
            }
            Player->StopsetrrCount = 0;
            if (Player->PowerstackCount < 2) {
                Player->PowerstackCount = 0;
            }
        }
    } else if (((Player->SlipFlag & N_JUMP) != N_JUMP) && ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
        if ((((s16) Player->Slipang) / DEGREES(1)) > 0) {
            // linear Map, sets -53 to 40 and 53 to 53
            var_s1_2 = (((s32) (((Player->Nowsterr >> 16) * 13) + (13 * 53))) / (2 * 53)) + 40;
            if ((Player->Nowsterr >> 16) <= -40) {
                Player->SlipFlag |= STOPSTERR;
                if ((Player->Nowsterr >> 16) <= -50) { // always true, but has no effect
                    Player->SlipFlag |= STOPSTERR;
                }
            }
            CheckDriftTurbo(Player, playerIndex);
        } else {
            // linear Map, sets -53 to -53 and 53 to -40
            var_s1_2 = (((s32) (((Player->Nowsterr >> 16) * 13) + (13 * 53))) / (2 * 53)) - 53;
            if ((Player->Nowsterr >> 16) >= 40) {
                Player->SlipFlag |= STOPSTERR;
                if ((Player->Nowsterr >> 16) <= -50) { // never true, but would have no effect
                    Player->SlipFlag |= STOPSTERR;
                }
            }
            CheckDriftTurbo(Player, playerIndex);
        }
        if ((((Player->Speed / 18.0f) * 216.0f) >= 0.0f) && (((Player->Speed / 18.0f) * 216.0f) < 8.0f)) {
            Player->Sterrangle = (s16) ((s32) (var_s1_2 * ((var_f2_2 + 2.0f) + (var_f2_2 * var_f12))));
        }
        if ((((Player->Speed / 18.0f) * 216.0f) >= 8.0f) && (((Player->Speed / 18.0f) * 216.0f) < 65.0f)) {
            Player->Sterrangle = var_s1_2 * ((var_f2_2 + 3) + (var_f2_2 * var_f12));
        }
        if (((Player->Speed / 18.0f) * 216.0f) >= 65.0f) {
            Player->Sterrangle = var_s1_2 * ((((f64) var_f2_2) + 3.5) + (var_f2_2 * var_f12));
        }
        if ((Player->SlipFlag & STOPSTERR) == STOPSTERR) {
            Player->Sterrangle *= 0.9;
        } else {
            Player->Sterrangle *= 0.65;
        }
    } else {
        var_s1_2 = Player->Nowsterr >> 16;
        if (clamped_x == 0) {
            var_s1_2 = 0;
        }
        if (((Player->Speed / 18.0f) * 216.0f) <= 5.0f) {
            Player->Sterrangle = (s16) ((s32) (((f32) var_s1_2) * (var_f2_2 + 6.0f)));
        } else {
            Player->Sterrangle = ((s16) var_s1_2) * (var_f2_2 + 1.5f);
        }
    }
    if (GameMode == BATTLE) {
        Player->Sterrangle *= 1.7;
    }
}

void EnemyStickAngle(Player* Player, s16 targetAngle) {
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

    if (!((Player->SlipFlag & SPIN_L) || (Player->SlipFlag & SPIN_R) ||
          (Player->SlipFlag & ROLLOVER) || (Player->SlipFlag & ROLLOVER_FALL) ||
          (Player->SlipFlag & THUNDER_SPIN) || (Player->SlipFlag & EXPLODE) ||
          (Player->SlipFlag & THROW_EXPLODE) || (Player->SlipFlag & BROKEN))) {
        if (!(((Player->Speed / 18.0f) * 216.0f) >= 110.0f)) {
            Player->SlipFlag &= ~STOPSTERR;
            Player->StopsetrrCount = 0;
            if (!(Player->SlipFlag & SPIN_L) && !(Player->SlipFlag & SPIN_R)) {
                sp304 = (s32) Player->Nowsterr >> 16;
                ChaseInumber(&sp304, (s32) targetAngle, 0.35f);
                sp304 <<= 0x10;
                if ((Player->SlipFlag & DRIFT) == DRIFT) {
                    var_f0 = (sp304 >> 0x10) / 5;
                } else {
                    var_f0 = (f32) (sp304 >> 0x10) / (8.0f + (Player->Accelcount / 50.0f));
                }
                if (var_f0 < 0.0f) {
                    var_f0 = -var_f0;
                }

                // Apply Speed and character multipliers
                if ((Player->SlipFlag & DRIFT) == DRIFT) {
                    var_f0 = speedTurn[(s16) ((Player->Speed / 18.0f) * 216.0f)] * var_f0;
                } else {
                    var_f0 = speedTurn[(s16) ((Player->Speed / 18.0f) * 216.0f)] * characterTurn[Player->Kart] *
                             var_f0;
                }
                Player->Nowsterr = sp304;
                if (((Player->SlipFlag & D_JUMP) != D_JUMP) &&
                    ((Player->SlipFlag & DRIFT) != DRIFT)) {
                    if ((Player->SlipFlag & BREAKE) != BREAKE) {
                        Player->Sterrangle = (Player->Nowsterr >> 16) * var_f0;
                    } else {
                        Player->Sterrangle = (Player->Nowsterr >> 16) * (var_f0 + 1.5);
                    }
                } else if ((Player->SlipFlag & N_JUMP) != N_JUMP) {
                    if (((s16) Player->Slipang / DEGREES(1)) > 0) {
                        var_v0 = Player->Nowsterr >> 16;
                    } else {
                        var_v0 = Player->Nowsterr >> 16;
                    }
                    Player->Sterrangle = var_v0 * (var_f0 + 3.0);
                    Player->Sterrangle *= 0.8;
                } else {
                    var_v0 = (s16) ((s32) Player->Nowsterr >> 16);
                    if (targetAngle == 0) {
                        var_v0 = 0;
                    }
                    Player->Sterrangle = var_v0 * var_f0;
                }
                if ((((Player->SlipFlag & D_JUMP) != D_JUMP) && (Player->Slipang <= 60) && (Player->Slipang >= -60)) ||
                    (((Player->Speed / 18.0f) * 216.0f) <= 20.0f) ||
                    ((Player->SlipFlag & CARHIT) == CARHIT)) {
                    DriftReset(Player);
                }
            }
        }
    }
}

void DriftSet(Player* Player) {
    if (((Player->Speed / 18.0f) * 216.0f) > 20.0f) {
        Player->DriftCount = 0;
        Player->SlipFlag |= DRIFT;
        Player->Talk |= BOING;
    }
}

void DriftReset(Player* Player) {
    s32 steer_position_new;

    if (((Player->SlipFlag & DRIFT) == DRIFT) && ((Player->Flag & IS_PLAYER) == IS_PLAYER)) {
        if ((Player->Slipang / DEGREES(1)) > 0) {
            // linear Map, sets -53 to 40 and 53 to 53
            steer_position_new = ((((Player->Nowsterr >> 16) * 13) + (13*53)) / (2*53)) + 40;
            Player->Nowsterr = steer_position_new << 16;
        }
        if ((Player->Slipang / DEGREES(1)) < 0) {
            // linear Map, sets -53 to -53 and 53 to -40
            steer_position_new = ((((Player->Nowsterr >> 16) * 13) + (13*53)) / (2*53)) - 53;
            Player->Nowsterr = steer_position_new << 16;
        }
        Player->SlipFlag &= ~DRIFT;
    }
    if (((Player->SlipFlag & DRIFT) == DRIFT) && ((Player->Flag & IS_PLAYER) != IS_PLAYER)) {
        Player->SlipFlag &= ~DRIFT;
    }
}

void SetvectorL(Player* Player, Vec3f arg1, Vec3f Arg2) {
    s16 steer_position_delta;
    UNUSED s16 pad;
    f32 sp20;
    f32 var_f18;
    s32 steer_position;

    if (((Player->SlipFlag & CENTER_LINE) == CENTER_LINE) ||
        ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, Player->align);
    } else {
        if (((Player->SlipFlag & DRIFT) == DRIFT) &&
            ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
            var_f18 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) * 3.0f) + (-Player->BrakeTime * 10.0f);
            sp20 = Player->FGrip * 3.0f;
        } else if (!(Player->SlipFlag & WHEELSPIN) && !(Player->HandlingFlag & SPINOUT_SWERVE)) {
            steer_position_delta = Player->Sterracc;
            if (steer_position_delta > 0) {
                steer_position_delta *= -1;
            }
            steer_position = Player->Nowsterr >> 16;
            if ((steer_position <= 20) && (steer_position >= -20)) {
                if (steer_position_delta < 20) {
                    var_f18 = (Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f)) +
                              (-Player->Accelcount * 0.02) + (-Player->BrakeTime * 50.0f);
                } else {
                    var_f18 = (Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f)) +
                              ((steer_position * 0.01) + (-Player->Accelcount * 0.05)) + (-Player->BrakeTime * 50.0f);
                }
            } else {
                var_f18 = (Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f)) +
                          ((steer_position * 0.1) + (-Player->Accelcount * 0.15)) + (-Player->BrakeTime * 50.0f);
            }
            sp20 = Player->FGrip;
        } else {
            var_f18 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) * 1.5) +
                      (((Player->Nowsterr >> 16) * 0.1) + (-Player->Accelcount * 0.05)) + (-Player->BrakeTime * 50.0f);
            sp20 = Player->FGrip;
        }
        if ((Player->SlipFlag & STAR) == STAR) {
            if (((Player->SlipFlag & DRIFT) == DRIFT) &&
                ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
                var_f18 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) * 3.0f) + (-Player->BrakeTime * 10.0f);
                sp20 = Player->FGrip * 3.0f;
            } else {
                var_f18 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f);
                sp20 = Player->FGrip;
            }
        }
        arg1[0] = (Player->Grip + var_f18) * Player->Speed;
        arg1[1] = 0.0f;
        arg1[2] = Player->Speed * sp20;
        MultipleMatrixByVector(arg1, Player->align);
    }
    Arg2[0] = arg1[0];
    Arg2[1] = arg1[1];
    Arg2[2] = arg1[2];
}

void SetvectorR(Player* Player, Vec3f arg1, Vec3f Arg2) {
    s16 steer_position_delta;
    f32 sp20;
    f32 var_f18;
    s32 steer_position;

    if (((Player->SlipFlag & CENTER_LINE) == CENTER_LINE) ||
        ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, Player->align);
    } else {
        if (((Player->SlipFlag & DRIFT) == DRIFT) &&
            ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
            var_f18 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) * 3.0f) + (-Player->BrakeTime * 50.0f);
            sp20 = Player->FGrip * 3.0f;
        } else if (!(Player->SlipFlag & WHEELSPIN) && !(Player->HandlingFlag & SPINOUT_SWERVE)) {
            steer_position_delta = Player->Sterracc;
            if (steer_position_delta > 0) {
                steer_position_delta *= -1;
            }
            steer_position = (s32) Player->Nowsterr >> 16;
            if ((steer_position <= 20) && (steer_position >= -20)) {
                if (steer_position_delta < 20) {
                    var_f18 = (Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f)) +
                              (-Player->Accelcount * 0.02) + (-Player->BrakeTime * 50.0f);
                } else {
                    var_f18 = ((Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f)) -
                               ((steer_position * 0.01) + (Player->Accelcount * 0.05))) +
                              (-Player->BrakeTime * 50.0f);
                }
            } else {
                var_f18 = ((Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f)) -
                           ((steer_position * 0.1) + (Player->Accelcount * 0.15))) +
                          (-Player->BrakeTime * 50.0f);
            }
            sp20 = Player->FGrip;
        } else {
            var_f18 = ((Player->AllrearGrip + ((f64) (-(Player->Speed / 18.0f) * 216.0f) * 1.5)) -
                       (((Player->Nowsterr >> 0x10) * 0.1) + (Player->Accelcount * 0.05))) +
                      (-Player->BrakeTime * 50.0f);
            sp20 = Player->FGrip;
        }
        if ((Player->SlipFlag & STAR) == STAR) {
            if (((Player->SlipFlag & DRIFT) == DRIFT) &&
                ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
                var_f18 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) * 3.0f) + (-Player->BrakeTime * 50.0f);
                sp20 = Player->FGrip * 3.0f;
            } else {
                var_f18 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 3.0f);
                sp20 = Player->FGrip;
            }
        }
        arg1[0] = -(Player->Grip + var_f18) * Player->Speed;
        arg1[1] = 0.0f;
        arg1[2] = Player->Speed * sp20;
        MultipleMatrixByVector(arg1, Player->align);
    }
    Arg2[0] = arg1[0];
    Arg2[1] = arg1[1];
    Arg2[2] = arg1[2];
}

void EnemySetvectorL(Player* Player, Vec3f arg1, Vec3f Arg2) {
    f32 var_f12;
    f32 var_f2;

    if (((Player->SlipFlag & CENTER_LINE) == CENTER_LINE) ||
        ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, Player->align);
    } else {
        if (((Player->SlipFlag & DRIFT) == DRIFT) &&
            ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
            var_f2 = ((-(Player->Speed / 18.0f) * 216.0f) * 2) + -80.0f;
            var_f12 = -80.0f;
        } else {
            var_f2 = ((-(Player->Speed / 18.0f) * 216.0f) / 2) + -20.0f;
            var_f12 = -40.0f;
        }
        arg1[0] = (var_f2 + 28.0f) * Player->Speed;
        arg1[1] = 0.0f;
        arg1[2] = var_f12 * Player->Speed;
        MultipleMatrixByVector(arg1, Player->align);
    }
    Arg2[0] = arg1[0];
    Arg2[1] = arg1[1];
    Arg2[2] = arg1[2];
}


void EnemySetvectorR(Player* Player, Vec3f arg1, Vec3f Arg2) {
    f32 var_f12;
    f32 var_f2;

    if (((Player->SlipFlag & CENTER_LINE) == CENTER_LINE) ||
        ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN)) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, Player->align);
    } else {
        if (((Player->SlipFlag & DRIFT) == DRIFT) &&
            ((Player->SlipFlag & D_JUMP) != D_JUMP)) {
            var_f2 = ((-(Player->Speed / 18.0f) * 216.0f) * 2) + -80.0f;
            var_f12 = -80.0f;
        } else {
            var_f2 = ((-(Player->Speed / 18.0f) * 216.0f) / 2) + -20.0f;
            var_f12 = -40.0f;
        }
        arg1[0] = -(var_f2 + 28.0f) * Player->Speed;
        arg1[1] = 0.0f;
        arg1[2] = var_f12 * Player->Speed;
        MultipleMatrixByVector(arg1, Player->align);
    }
    Arg2[0] = arg1[0];
    Arg2[1] = arg1[1];
    Arg2[2] = arg1[2];
}

void BattleSetvectorL(Player* Player, Vec3f arg1, Vec3f Arg2) {
    f32 var_f12;
    f32 var_f2;

    if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, Player->align);
    } else {
        if ((Player->SlipFlag & DRIFT) == DRIFT) {
            var_f2 = Player->AllrearGrip + (-(Player->Speed / 18.0f) * 216.0f * 5.0f) + (-Player->BrakeTime * 10.0f);
            var_f12 = -100.0f;
        } else {
            var_f2 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 40.0f) + (-Player->BrakeTime * 50.0f);
            var_f12 = Player->FGrip;
        }
        arg1[0] = (Player->Grip + var_f2) * Player->Speed;
        arg1[1] = 0.0f;
        arg1[2] = Player->Speed * var_f12;
        MultipleMatrixByVector(arg1, Player->align);
    }
    Arg2[0] = arg1[0];
    Arg2[1] = arg1[1];
    Arg2[2] = arg1[2];
}

void BattleSetvectorR(Player* Player, Vec3f arg1, Vec3f Arg2) {
    f32 var_f12;
    f32 var_f2;

    if ((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
        MultipleMatrixByVector(arg1, Player->align);
    } else {
        if ((Player->SlipFlag & DRIFT) == DRIFT) {
            var_f2 = Player->AllrearGrip + (-(Player->Speed / 18.0f) * 216.0f * 5.0f) + (-Player->BrakeTime * 50.0f);
            var_f12 = -100.0f;
        } else {
            var_f2 = Player->AllrearGrip + ((-(Player->Speed / 18.0f) * 216.0f) / 40.0f) + (-Player->BrakeTime * 50.0f);
            var_f12 = Player->FGrip;
        }
        arg1[0] = -(Player->Grip + var_f2) * Player->Speed;
        arg1[1] = 0.0f;
        arg1[2] = Player->Speed * var_f12;
        MultipleMatrixByVector(arg1, Player->align);
    }
    Arg2[0] = arg1[0];
    Arg2[1] = arg1[1];
    Arg2[2] = arg1[2];
}

void YokoG(Player* Player, Vec3f arg1) {
    UNUSED s32 pad[3];
    Vec3f sp20;

    if (Player->Sterrangle == 0) {
        arg1[0] = 0.0f;
        arg1[1] = 0.0f;
        arg1[2] = 0.0f;
    } else {
        if (Player->Sterrangle < 0) {
            if (((Player->SlipFlag & STOPSTERR) != STOPSTERR) || (Player->StopsetrrCount >= 100)) {
                Player->Direction[1] += Player->Sterrangle;
            }
            if (!(Player->Flag & IS_CPU_PLAYER)) {
                if (GameMode == BATTLE) {
                    BattleSetvectorL(Player, sp20, arg1);
                } else {
                    SetvectorL(Player, sp20, arg1);
                }
            } else {
                EnemySetvectorL(Player, sp20, arg1);
            }
        } else {
            if (((Player->SlipFlag & STOPSTERR) != STOPSTERR) || (Player->StopsetrrCount >= 100)) {
                Player->Direction[1] += Player->Sterrangle;
            }
            if (!(Player->Flag & IS_CPU_PLAYER)) {
                if (GameMode == BATTLE) {
                    BattleSetvectorR(Player, sp20, arg1);
                } else {
                    SetvectorR(Player, sp20, arg1);
                }
            } else {
                EnemySetvectorR(Player, sp20, arg1);
            }
        }
    }
}

void ManualStickDrive(Player* Player, struct Controller* controller, s8 playerIndex) {
    if (((Player->SlipFlag & SPIN_L) != SPIN_L) &&
        ((Player->SlipFlag & SPIN_R) != SPIN_R) &&
        ((Player->SlipFlag & ROLLOVER) != ROLLOVER) &&
        ((Player->SlipFlag & RAPID_ACC) != RAPID_ACC) &&
        ((Player->SlipFlag & EXPLODE) != EXPLODE) &&
        ((Player->SlipFlag & THROW_EXPLODE) != THROW_EXPLODE) &&
        ((Player->SlipFlag & ROLLOVER_FALL) != ROLLOVER_FALL) &&
        ((Player->SlipFlag & THUNDER_SPIN) != THUNDER_SPIN)) {
        if (((Player->SlipFlag & BROKEN) != BROKEN) &&
            ((Player->SlipFlag & N_JUMP) != N_JUMP) && ((Player->SlipFlag & D_JUMP) != D_JUMP) &&
            ((Player->SlipFlag & DRIFT) != DRIFT) && (controller->ButtonPressed & R_TRIG)) {
            JumpSet(Player);
            if (((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
                ((Player->Flag & IS_GHOST) != IS_GHOST)) {
                NAPlyTrgStart(playerIndex, 0x19008000);
            }
        }
        ProStickAngleHook(Player, controller, playerIndex);
        Player->SlipFlag &= ~BREAKE;
        if ((!(Player->SlipFlag & DASH_JUMP)) && (!(Player->SlipFlag & DASH_MIDJUMP))) {
            if (((Player->Speed / 18.0f) * 216.0f) <= 12.0f) {
                if (controller->ButtonHeld & A_BUTTON) {
                    if (controller->ButtonHeld & B_BUTTON) {
                        Player->SlipFlag |= SPIN_TURN;
                        if ((Player->SlipFlag & SPIN_TURN) != SPIN_TURN) {
                            Player->Accelcount += 100.0f;
                        }
                    }
                }
            }
            if (((Player->SlipFlag & SPIN_TURN) == SPIN_TURN) &&
                (((controller->ButtonHeld & B_BUTTON) == 0) || (!(controller->ButtonHeld & A_BUTTON)))) {
                Player->SlipFlag &= ~SPIN_TURN;
            }
        }
        if ((Player->HandlingFlag & REVERSE_GEAR) != REVERSE_GEAR) {
            if (controller->ButtonHeld & A_BUTTON) {
                AccelOn(Player);
                CheckAcceleOn(Player);
            } else {
                if (GameMode == BATTLE) {
                    AccelOff(Player, 2.0f);
                } else {
                    AccelOff(Player, 1.0f);
                }
                CheckAcceleOff(Player);
            }
            if (controller->ButtonHeld & B_BUTTON) {
                BreakeOn(Player);
                CheckBrakeOn(Player);
            } else {
                Player->BrakeTime = 0.0f;
                CheckBrakeOff(Player);
            }
        }
        if ((!(Player->SlipFlag & DASH_JUMP)) && (!(Player->SlipFlag & DASH_MIDJUMP))) {
            if (((CheckStickY(controller) < (-0x31)) && (((Player->Speed / 18.0f) * 216.0f) <= 5.0f)) &&
                (controller->ButtonHeld & B_BUTTON)) {
                Player->Accelcount = 140.0f;
                Player->HandlingFlag |= REVERSE_GEAR;
                Player->Force = (Player->Accelcount * Player->Accelcount) / 25.0f;
                Player->BrakeTime = 0.0f;
            }
            if ((CheckStickY(controller) >= -0x1D) || (!(controller->ButtonHeld & B_BUTTON))) {
                if ((Player->HandlingFlag & REVERSE_GEAR) == REVERSE_GEAR) {
                    Player->HandlingFlag &= ~(REVERSE_GEAR);
                    Player->Accelcount = 0.0f;
                }
            }
        }
    } else {
        if ((Player->SlipFlag & RAPID_ACC) == RAPID_ACC) {
            if (controller->ButtonHeld & A_BUTTON) {
                AccelOn(Player);
            } else {
                AccelOff(Player, 5.0f);
            }
        }
        if (((((Player->SlipFlag & SPIN_L) == SPIN_L) ||
              ((Player->SlipFlag & SPIN_R) == SPIN_R)) ||
             ((Player->SlipFlag & EXPLODE) == EXPLODE)) ||
            ((Player->SlipFlag & THROW_EXPLODE) == THROW_EXPLODE)) {
            if (controller->ButtonHeld & A_BUTTON) {
                CheckAcceleOn(Player);
                EmptyAcceleOn(Player, playerIndex);
                return;
            }
            CheckAcceleOff(Player);
            EmptyAcceleOff(Player, 5.0f, playerIndex);
        }
    }
}

void DriveStickKindcontrol(Player* Player, struct Controller* controller, s8 playerIndex) {
    if (((Player->Flag & EXISTS) == EXISTS) && ((Player->Flag & IS_PLAYER) == IS_PLAYER) &&
        ((Player->Flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
        // If not Start sequence
        if ((Player->Flag & NO_CONTROLS) != NO_CONTROLS) {
            if (((Player->JugemuFlag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
                ((Player->JugemuFlag & LAKITU_SCENE) == LAKITU_SCENE)) {
                if (controller->ButtonHeld & A_BUTTON) {
                    JugemuAcceleOn(Player);
                } else {
                    JugemuAcceleOff(Player, 5.0f);
                }
            } else {
                ManualStickDrive(Player, controller, playerIndex);
            }
            D_80164A89 = 1;
            // If Start sequence and Unknown condition
        } else if (D_8018D168 == 1) {
            if (D_801656F0 == 1) {
                if (GreenTimer[playerIndex] == 0) {
                    GreenTimer[playerIndex] = Kwvideoframetimer;
                }
            }
            if (controller->ButtonHeld & A_BUTTON) {
                PlayerNocontacceleOn(Player);
            } else {
                PlayerNocontacceleOff(Player, 5.0f);
            }
        }
    }
}

void DriveStickControl(void) {
    u16 temp_v0_3;
    u16 temp_v0_4;
    u16 temp_v0_5;
    u16 temp_v0_6;

    if (FlyCamToggle > 0) {
        return;
    }
    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            switch (GameMode) {
                case GRAND_PRIX:
                    DriveStickKindcontrol(PlayerOne, ControllerOne, 0);
                    return;
                case TIME_TRIALS:
                    if (ReplayFlag != 1) {
                        DriveStickKindcontrol(PlayerOne, ControllerOne, 0);
                        temp_v0_3 = PlayerTwo->Flag;
                        if (((temp_v0_3 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_3 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            DriveStickKindcontrol(PlayerTwo, ControllerSix, 1);
                        }
                        temp_v0_4 = PlayerThree->Flag;
                        if (((temp_v0_4 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_4 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            DriveStickKindcontrol(PlayerThree, ControllerSeven, 2);
                            return;
                        }
                    } else {
                        if ((PlayerOne->Flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
                            DriveStickKindcontrol(PlayerOne, ControllerEight, 0);
                        }
                        temp_v0_5 = PlayerTwo->Flag;
                        if (((temp_v0_5 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_5 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            DriveStickKindcontrol(PlayerTwo, ControllerSix, 1);
                        }
                        temp_v0_6 = PlayerThree->Flag;
                        if (((temp_v0_6 & IS_GHOST) == IS_GHOST) &&
                            ((temp_v0_6 & IS_RACE_FINISH) != IS_RACE_FINISH)) {
                            DriveStickKindcontrol(PlayerThree, ControllerSeven, 2);
                            return;
                        }
                        return;
                    }

                    break;
            }
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            DriveStickKindcontrol(PlayerOne, ControllerOne, 0);
            DriveStickKindcontrol(PlayerTwo, ControllerTwo, 1);
            return;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            DriveStickKindcontrol(PlayerOne, ControllerOne, 0);
            DriveStickKindcontrol(PlayerTwo, ControllerTwo, 1);
            DriveStickKindcontrol(PlayerThree, ControllerThree, 2);
            if (PlayerCount == 4) {
                DriveStickKindcontrol(PlayerFour, ControllerFour, 3);
            }
            break;
    }
}

s16 CheckStickX(struct Controller* controller) {
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

s16 CheckStickY(struct Controller* controller) {
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

void ResultAcceleOn(Player* Player, s16 arg1) {
    Player->Accelcount += (f32) arg1;
    if (Player->Accelcount < 0.0f) {
        Player->Accelcount = 0.0f;
    }
    if (Player->Accelcount >= 250.0f) {
        Player->Accelcount = 250.0f;
    }
    Player->HandlingFlag |= ACCELERATE;
    Player->Force = (Player->Accelcount * Player->Accelcount) / 25.0f;
}

void ResultRunkart(Player* Player, UNUSED Camera* Camera, s8 screenId, s8 PlayerId) {
    Vec3f sp114 = { 0.0, 0.0, 1.0 };
    Vec3f sp108 = { 0.0, 0.0, 0.0 };
    Vec3f spFC = { 0.0, 0.0, 0.0 };
    Vec3f spF0 = { 0.0, 0.0, 0.0 };
    f32 nextX;
    f32 nextY;
    f32 nextZ;
    f32 PosX;
    f32 PosY;
    f32 PosZ;
    f32 sqrt;

    f32 divOptimize;
    UNUSED s32 Pad2[10];
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 surfaceDistance;
    UNUSED s32 Pad3;
    Vec3f newVelocity;
    Vec3f sp7C;
    UNUSED s32 pad[10];

    Player->FGrip = -10.0f;
    Player->RGrip = 28.0f;
    Player->AccMaxcount = 250.0f;
    WeaponStatus(Player, PlayerId, screenId);
    SetStrategy(Player, PlayerId, screenId);
    Player->Direction[1] += Player->Sterrangle;
    spF0[0] = 0;
    spF0[1] = 0;
    spF0[2] = 0;
    AddGravityEdit(Player);
    spA4 = 0 * (Player->GravityXz[0] + spFC[0]);
    spA0 = -1 * Player->Gravity;
    sp9C = 0 * (Player->GravityXz[2] + spFC[2]);
    sp108[2] = 0;
    sp108[1] = 0;
    sp108[0] = 0;
    MultipleMatrixByVector(sp108, Player->align);
    spA4 += sp108[0];
    sp9C += sp108[2];
    sp114[2] = Player->Force;
    MultipleMatrixByVector(sp114, Player->align);

    newVelocity[0] = Player->Velocity[0];
    newVelocity[1] = Player->Velocity[1];
    newVelocity[2] = Player->Velocity[2];

    newVelocity[0] += ((((((sp114[0] + spA4) + spF0[0])) - (newVelocity[0] * (0.12 * (Player->Mass)))) / 6000.0) / 1);
    newVelocity[2] += ((((((sp114[2] + sp9C) + spF0[2])) - (newVelocity[2] * (0.12 * (Player->Mass)))) / 6000.0) / 1);
    newVelocity[1] += ((((((sp114[1] + spA0) + spF0[1])) - (newVelocity[1] * (0.12 * (Player->Mass)))) / 6000.0) / 1);
    if ((Player->HandlingFlag & CPU_SIMPLE_KART) == CPU_SIMPLE_KART) {
        Player->HandlingFlag &= ~CPU_SIMPLE_KART;
    }

    PosX = Player->Position[0];
    PosY = Player->Position[1];
    PosZ = Player->Position[2];

    Player->OldPosition[0] = Player->Position[0];
    Player->OldPosition[1] = Player->Position[1];
    Player->OldPosition[2] = Player->Position[2];

    nextX = PosX + Player->Velocity[0];
    nextY = PosY + Player->Velocity[1];
    nextZ = PosZ + Player->Velocity[2];
    DriftJump(Player);
    nextY += Player->Jump;
    nextY -= 0.02;
    CustomCheckBump2(&Player->Bump, Player->Radius, nextX, nextY, nextZ, Player->OldPosition[0],
                            Player->OldPosition[1], Player->OldPosition[2]);
    Player->UpVector[0] = 0;
    Player->UpVector[2] = 0;
    Player->UpVector[1] = 1.0f;
    MakeAlignMatrix(Player->align, 0, 1.0f, 0, Player->Direction[1]);
    Player->SlipFlag |= N_JUMP;
    Player->Jumpcount += 1;
    surfaceDistance = Player->Bump.DistanceZX;
    if (surfaceDistance <= 0) {
        Player->Jumpcount = 0;
        Player->SlipFlag &= ~D_JUMP;
        Player->SlipFlag &= ~N_JUMP;
        Player->Jump = Player->Jumpcount;
    }
    surfaceDistance = Player->Bump.DistanceZX;
    if (surfaceDistance <= 0) {
        CheckMapBGZXHook(Player, sp7C, newVelocity, sp108, &surfaceDistance, &nextX, &nextY, &nextZ);
    }
    surfaceDistance = Player->Bump.DistanceXY;
    if (surfaceDistance < 0) {
        CheckMapBGXY(Player, sp7C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(Player, PlayerId, newVelocity);
    }
    surfaceDistance = Player->Bump.DistanceYZ;
    if (surfaceDistance < 0) {
        CheckMapBGYZ(Player, sp7C, newVelocity, &surfaceDistance, &nextX, &nextY, &nextZ);
        CheckWall(Player, PlayerId, newVelocity);
    }
    surfaceDistance = Player->Bump.DistanceXY;
    if (surfaceDistance >= 0) {
        surfaceDistance = Player->Bump.DistanceYZ;
        if (surfaceDistance >= 0) {
            Player->HitFlag &= 0xFFDF;
        }
    }
    Player->Ground = CalcHeight(nextX, nextY, nextZ, Player->Bump.LastZX);
    TirePosition(Player, nextX, nextY, nextZ);
    SetSlipAngle(Player, PlayerId, PosX, PosZ, nextX, nextZ);
    sqrt = (newVelocity[0] * newVelocity[0]) + (newVelocity[2] * newVelocity[2]);
    Player->OldSpeed = Player->Speed;
    Player->Speed = sqrtf(sqrt);

    Player->Position[0] = nextX;
    Player->Position[2] = nextZ;
    Player->Position[1] = nextY;

    Player->GravityXz[0] = sp108[0];
    Player->GravityXz[2] = sp108[2];

    Player->Velocity[0] = newVelocity[0];
    Player->Velocity[1] = newVelocity[1];
    Player->Velocity[2] = newVelocity[2];

    OldVelocity[PlayerId][0] = newVelocity[0];
    OldVelocity[PlayerId][1] = newVelocity[1];
    OldVelocity[PlayerId][2] = newVelocity[2];

    if ((Player->Flag & IS_PLAYER) == IS_PLAYER) {
        if (gKartTopSpeedTable[Player->Kart] < Player->Speed) {
            divOptimize = gKartTopSpeedTable[Player->Kart] / Player->Speed;
            Player->Velocity[0] *= divOptimize;
            Player->Velocity[1] *= divOptimize;
            Player->Velocity[2] *= divOptimize;
            Player->Speed = gKartTopSpeedTable[Player->Kart];
        }
    }
    if ((Player->HandlingFlag & REVERSE_GEAR) == REVERSE_GEAR) {
        if (Player->Speed > 1.0f) {
            Player->Velocity[0] *= 1.0f / Player->Speed;
            Player->Velocity[1] *= 1.0f / Player->Speed;
            Player->Velocity[2] *= 1.0f / Player->Speed;
            Player->Speed = 1.0f;
        }
    }
    if (Player->Bump.DistanceZX >= 500.0f) {
        Player->Sterrangle /= 2;
    }
    if (OKCheckSplashEnabled) {
        CheckSplash(Player, PlayerId);
    }
}
