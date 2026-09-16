#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <common_structs.h>
#include <mk64.h>
#include <course.h>

#include "menus.h"
#include "main.h"
#include "code_800029B0.h"
#include "actors.h"
#include "audio/external.h"
#include "code_800029B0.h"
#include "cpu_vehicles_camera_path.h"
#include "menu_items.h"
#include "code_800AF9B0.h"
#include "save.h"
#include "OverKartHooks.h"
#include "replays.h"
#include "save_data.h"
#include <sounds.h>
#include "spawn_players.h"
#include "seq_ids.h"

#if ENABLE_DEBUG_MODE
#define DEBUG_MODE_TOGGLE true
#define DEBUG_MENU_SELECTION DEBUG_MENU_DEBUG_MODE
#else
#define DEBUG_MODE_TOGGLE false
#define DEBUG_MENU_SELECTION DEBUG_MENU_DISABLED
#endif

/** BSS **/
// Variables used to maniplate the model for Intro Logo
// and checkerboard on the start screen
s32 logo_cam_z;
f32 logo_scale_z; // XYZ scale on checkerboard flag, Z scale on intro logo
f32 logo_rot_x;
f32 logo_rot_y;
f32 logo_rot_z;
f32 logo_pos_x;
f32 logo_pos_y;
f32 logo_pos_z;

s32 KBGNumberNext;
s8 g_player1Character[4];    // Map from each player to current grid position (1-4 top, 5-8 bottom)
bool8 gCharacterGridIsSelected[4]; // Sets true if a character is selected for each player
s8 menuScreenA;              // Map Select states, Options and Ghost Data text selection
s8 menuScreenB;
s8 menuScreenC;
s8 title_mode;
s8 gControllerPakMenuSelection;
s8 gScreenModeListIndex; // 0-4 index, selects a screen mode in sScreenModePlayerTable
u8 sound_output;
s8 g_menuMultiplayerSelection;
s8 vs_result_cursor;     // 4 options indexed (10-13), gets set when selecting an option
s8 ta_result_cursor; // 5 options indexed (5-9), gets set when selecting an option (excluding Save
                                     // Ghost)
s8 bt_result_cursor;     // 4 options indexed (10-13), gets set when selecting an option
s8 gTimeTrialDataCourseIndex;
s8 gCourseRecordsMenuSelection;    // Used for selecting an option in course record data
s8 gCourseRecordsSubMenuSelection; // Used for erase records and ghosts (Quit - Erase)
s8 gDebugGotoScene;
bool8 g_InGameTT;
bool8 g_InGame;
s32 titleDemo;
s32 title_counter2;
s8 demo_play_flag; // Sets true alongside g_DemoFlag, controller related
s8 g_cupSelect;
s8 before_cup_select; // Same as g_cupSelect but it's only set in map select, not referenced
s8 g_courseSelect;
s8 fb_clear_flg; // Set to 0 but never referenced

/** Data **/
s32 KBGNumber = LOGO_INTRO_MENU;
s32 KBGChange = FADE_MODE_NONE;

// Default selected character for each player
s8 g_CharacterSelections[4] = { MARIO, LUIGI, YOSHI, TOAD };

// The current row selected in the mode column for each player indexed
// 0-1 1p / 0-2 2p´/ 0-1 3p / 0-1 4p
s8 gGameModeMenuColumn[4] = { 0, 0, 0, 0 };

// For Grand Prix and Versus, this will be the CC mode selected. For Time Trials, it will
// be whether 'Begin' or 'Data' is selected. Not used for Battle.
// indexed as [column][row]
s8 gGameModeSubMenuColumn[4][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

s8 gNextDemoId = 0;
s8 gControllerPakSelectedTableRow = 0; // 0-4 index, value of the current visible row select

// Numbers starting from the second 0 to number 6 get altered
// as you move up or down the page table of content (min value is 0, max value is 16
s8 gControllerPakVisibleTableRows[12] = { 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0 };

s8 gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE; // 1 is down, 2 is up
s8 unref_D_800E86D4[12] = { 0 };
s8 unref_D_800E86E0[4] = { 0, 0, 0, 1 };

u32 sVIGammaOffDitherOn = (OS_VI_GAMMA_OFF | OS_VI_DITHER_FILTER_ON);

/** RoData **/

// Sets the actual screen mode based on values set in sScreenModePlayerCount
const s8 sScreenModePlayerTable[] = { SCREEN_MODE_1P, SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL,
                                      SCREEN_MODE_2P_SPLITSCREEN_VERTICAL, SCREEN_MODE_3P_4P_SPLITSCREEN,
                                      SCREEN_MODE_3P_4P_SPLITSCREEN };

// Sets how many players can load on each screen mode set in sScreenModePlayerTable
const s8 sScreenModePlayerCount[] = { 1, 2, 2, 3, 4 };

// Set indexed slots numbers for one-two-three-four mode selection
const s8 gPlayerModeSelection[] = { 1, 2, 1, 1 };

// Limit for each index column in one-two-three-four mode selection
const s8 sGameModePlayerColumnDefault[][3] = {
    { 2, 1, 0 }, // 1p (GP options, TT options, ...)
    { 2, 2, 0 }, // 2p (GP options, VS options, Battle)
    { 2, 0, 0 }, // 3p (VS options, Battle, ...)
    { 2, 0, 0 }, // 4p (VS options, Battle, ...)
};

// Limit for each index column in one-two-three-four mode selection
// for extra mode (mirror mode), hence the extra value (3 instead of 2)
const s8 sGameModePlayerColumnExtra[][3] = {
    { 3, 1, 0 }, // 1p (GP options, TT options, ...)
    { 3, 3, 0 }, // 2p (GP options, VS options, Battle)
    { 3, 0, 0 }, // 3p (VS options, Battle, ...)
    { 3, 0, 0 }, // 4p (VS options, Battle, ...)
};

// Modes to select in one-two-three-four mode selection
const s32 gGameModePlayerSelection[][3] = {
    { GRAND_PRIX, TIME_TRIALS, 0x00000000 }, // 1p game modes
    { GRAND_PRIX, VERSUS, BATTLE },          // 2p game modes
    { VERSUS, BATTLE, 0x00000000 },          // 3p game modes
    { VERSUS, BATTLE, 0x00000000 },          // 4p game modes
};

// Map from character grid position id to character id
// Note: changing order doesn't affect graphics, only the selection
const s8 sCharacterGridOrder[] = {
    MARIO, LUIGI, PEACH, TOAD, YOSHI, DK, WARIO, BOWSER,
};

const s16 gCupCourseOrder[5][4] = {
    // mushroom cup
    { COURSE_LUIGI_RACEWAY, COURSE_MOO_MOO_FARM, COURSE_KOOPA_BEACH, COURSE_KALAMARI_DESERT },
    // flower cup
    { COURSE_TOADS_TURNPIKE, COURSE_FRAPPE_SNOWLAND, COURSE_CHOCO_MOUNTAIN, COURSE_MARIO_RACEWAY },
    // star cup
    { COURSE_WARIO_STADIUM, COURSE_SHERBET_LAND, COURSE_ROYAL_RACEWAY, COURSE_BOWSER_CASTLE },
    // special cup
    { COURSE_DK_JUNGLE, COURSE_YOSHI_VALLEY, COURSE_BANSHEE_BOARDWALK, COURSE_RAINBOW_ROAD },
    // battle mode
    { COURSE_BIG_DONUT, COURSE_BLOCK_FORT, COURSE_DOUBLE_DECK, COURSE_SKYSCRAPER },
};

const s8 unref_800F2BDC[4] = { 1, 0, 0, 0 };

// Uses player count to set gScreenModeListIndex, the latter variable then selects a mode
// from sScreenModePlayerTable, note the 2 is not set since that's for vertical 2p screen
const s8 sScreenModeIdxFromPlayerMode[4] = { 0, 1, 3, 4 };

const union GameModePack sSoundMenuPack = { { SOUND_STEREO, SOUND_HEADPHONES, SOUND_UNUSED, SOUND_MONO } };

/**************************/

/**
 * General menu main handler
 * Includes opening logo and splash screens
 */
void UpdateController(void) {
    u16 controllerIdx;

    if (KBGChange == FADE_MODE_NONE) {
        for (controllerIdx = 0; controllerIdx < 4; controllerIdx++) {
            // Debug, quick jump through menus using the start button.
            if ((FadeChecker() == 0) && (g_DebugMode) &&
                ((gControllers[controllerIdx].ButtonPressed & START_BUTTON) != 0)) {
                // this is certainly a way to write these...
                switch (KBGNumber) {
                    case COURSE_SELECT_MENU:
                        NaSeqFadeout(0x19);
                        // deliberate (?) fallthru
                    case MAIN_MENU:
                    case CHARACTER_SELECT_MENU:
                        playSound(SOUND_MENU_OK_CLICKED);
                        break;
                }

                switch (KBGNumber) {
                    case CONTROLLER_PAK_MENU:
                    case START_MENU:
                        break;
                    default:
                        SetFadeOut_f();
                }
            }
            osViSetSpecialFeatures(sVIGammaOffDitherOn);
            switch (KBGNumber) {
                case OPTIONS_MENU:
                    optionController(&gControllers[controllerIdx], controllerIdx);
                    break;
                case DATA_MENU:
                    taselController(&gControllers[controllerIdx], controllerIdx);
                    break;
                case COURSE_DATA_MENU:
                    dselController(&gControllers[controllerIdx], controllerIdx);
                    break;
                case LOGO_INTRO_MENU:
                    nintenLogoControler(&gControllers[controllerIdx], controllerIdx);
                    break;
                case CONTROLLER_PAK_MENU:
                    if (controllerIdx == PLAYER_ONE) {
                        cpControler(&gControllers[controllerIdx], controllerIdx);
                    }
                    break;
                case START_MENU_FROM_QUIT:
                case START_MENU:
                    TitleController(&gControllers[controllerIdx], controllerIdx);
                    break;
                case MAIN_MENU_FROM_QUIT:
                case MAIN_MENU:
                    GSelController(&gControllers[controllerIdx], controllerIdx);
                    break;
                case PLAYER_SELECT_MENU_FROM_QUIT:
                case CHARACTER_SELECT_MENU:
                    PSelController(&gControllers[controllerIdx], controllerIdx);
                    break;
                case COURSE_SELECT_MENU_FROM_QUIT:
                case COURSE_SELECT_MENU:
                    MSelController(&gControllers[controllerIdx], controllerIdx);
                    break;
            }
        }
    }
}

/**
 * Navigation of the options menu
 */
void optionController(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick; // sp3E
    ObjBlock* sp38;
    s32 res;
    struct_8018EE10_entry* sp30;
    bool tempVar; // cursorWasMoved or communicateStoredAction
    UNUSED u32 pad;

    btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    if (!g_DebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        sp38 = Get_OBJBlock_ptr(0xF0);
        sp30 = (struct_8018EE10_entry*) kdl_buff;
        switch (menuScreenA) {
            case SUB_MENU_OPTION_RETURN_GAME_SELECT:
            case SUB_MENU_OPTION_SOUND_MODE:
            case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
            case SUB_MENU_OPTION_ERASE_ALL_DATA: {
                tempVar = false;
                if ((btnAndStick & D_JPAD) && (menuScreenA < SUB_MENU_OPTION_MAX)) {
                    menuScreenA += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    sp38->pattern2 = 1;
                    tempVar = true;
                }
                if ((btnAndStick & U_JPAD) && (menuScreenA > SUB_MENU_OPTION_MIN)) {
                    menuScreenA -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    tempVar = true;
                    sp38->pattern2 = -1;
                }
                if (tempVar && sound_output != sp38->pattern) {
                    eeprom_save_data.main.saveInfo.soundMode = sound_output;
                    SaveFunc800B4670Default();
                    SaveFunc800B5948Default();
                    sp38->pattern = sound_output;
                }
                if (btnAndStick & B_BUTTON) {
                    SetFadeOut_option();
                    playSound(SOUND_MENU_GO_BACK);
                    if (sound_output != sp38->pattern) {
                        eeprom_save_data.main.saveInfo.soundMode = sound_output;
                        SaveFunc800B4670Default();
                        SaveFunc800B5948Default();
                        sp38->pattern = sound_output;
                    }
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (menuScreenA) {
                        case SUB_MENU_OPTION_SOUND_MODE:
                            if (sound_output < 3) {
                                sound_output += 1;
                            } else {
                                sound_output = SOUND_STEREO;
                            }
                            if (sound_output == SOUND_UNUSED) {
                                sound_output = SOUND_MONO;
                            }
                            Setting_SoundOutMode();
                            switch (sound_output) {
                                case SOUND_STEREO:
                                    playSound(SOUND_MENU_STEREO);
                                    return;
                                case SOUND_HEADPHONES:
                                    playSound(SOUND_MENU_HEADPHONES);
                                    return;
                                case SOUND_MONO:
                                    playSound(SOUND_MENU_MONO);
                                    return;
                            }
                            break;
                        case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
                            switch (CheckPakStatus2()) {
                                case PFS_INVALID_DATA:
                                    menuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA;
                                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_NO_ERROR:
                                    LoadGameNoteKart_data2();
                                    tempVar = CheckPakStatus();
                                    switch (tempVar) {
                                        case PFS_INVALID_DATA:
                                            menuScreenA = SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT;
                                            sp38->pattern = 0;
                                            playSound(SOUND_MENU_SELECT);
                                            break;
                                        case PFS_NO_ERROR:
                                            LoadGameNoteKart_data();
                                            break;
                                        case PFS_NO_PAK_INSERTED:
                                            menuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P;
                                            playSound(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_FILE_OVERFLOW:
                                            menuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P;
                                            playSound(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_PAK_BAD_READ:
                                        case PFS_PAK_CORRUPTED: // unreachable, bad reads always returns previous case
                                        default:
                                            menuScreenA = SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P;
                                            playSound(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                    }
                                    if (tempVar == PFS_INVALID_DATA && !sp30[PLAYER_ONE].ghostDataSaved &&
                                        !sp30[PLAYER_TWO].ghostDataSaved) {
                                        menuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA;
                                        playSound(SOUND_MENU_FILE_NOT_FOUND);
                                        return;
                                    }
                                    if (tempVar == PFS_NO_ERROR) {
                                        if (sp30[PLAYER_ONE].ghostDataSaved) {
                                            menuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST1_1P;
                                            playSound(SOUND_MENU_SELECT);
                                        } else if (sp30[PLAYER_TWO].ghostDataSaved) {
                                            menuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST2_1P;
                                            playSound(SOUND_MENU_SELECT);
                                        } else {
                                            menuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA;
                                            playSound(SOUND_MENU_FILE_NOT_FOUND);
                                        }
                                    }
                                    // else return?
                                    return;
                                case PFS_NO_PAK_INSERTED:
                                    menuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P;
                                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_PAK_BAD_READ:
                                default:
                                    menuScreenA = SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P;
                                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                            }
                        case SUB_MENU_OPTION_ERASE_ALL_DATA: {
                            menuScreenA = SUB_MENU_ERASE_QUIT;
                            playSound(SOUND_MENU_SELECT);
                            return;
                        }
                        case SUB_MENU_OPTION_RETURN_GAME_SELECT: {
                            SetFadeOut_option();
                            playSound(SOUND_MENU_GO_BACK);
                            return;
                        }
                    }
                }
                // maybe else return?;
                break;
            }
            case SUB_MENU_ERASE_QUIT:
            case SUB_MENU_ERASE_ERASE: {
                if ((btnAndStick & D_JPAD) && (menuScreenA < SUB_MENU_ERASE_MAX)) {
                    menuScreenA += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    sp38->pattern2 = 1;
                }
                if ((btnAndStick & U_JPAD) && (menuScreenA > SUB_MENU_ERASE_MIN)) {
                    menuScreenA -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    sp38->pattern2 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    menuScreenA = SUB_MENU_OPTION_ERASE_ALL_DATA;
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (menuScreenA) {
                        case SUB_MENU_ERASE_QUIT:
                            menuScreenA = SUB_MENU_OPTION_ERASE_ALL_DATA;
                            playSound(SOUND_MENU_GO_BACK);
                            break;
                        case SUB_MENU_ERASE_ERASE:
                            menuScreenA = SUB_MENU_SAVE_DATA_ERASED;
                            initEEPRom();
                            D_800DC5AC = 0;
                            playSound(SOUND_MENU_EXPLOSION);
                            break;
                    }
                }
                break; // or return?
            }
            case SUB_MENU_SAVE_DATA_ERASED: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    menuScreenA = SUB_MENU_OPTION_ERASE_ALL_DATA;
                    playSound(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_FROM_GHOST1_1P:
            case SUB_MENU_COPY_PAK_FROM_GHOST2_1P: {
                if ((btnAndStick & D_JPAD) && (menuScreenA < SUB_MENU_COPY_PAK_FROM_GHOST_MAX) &&
                    (sp30[PLAYER_TWO].ghostDataSaved)) {
                    menuScreenA += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    sp38->pattern2 = 1;
                }
                if ((btnAndStick & U_JPAD) && (menuScreenA > SUB_MENU_COPY_PAK_FROM_GHOST_MIN) &&
                    sp30[PLAYER_ONE].ghostDataSaved) {
                    menuScreenA -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    sp38->pattern2 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    menuScreenA = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    sp38->work2 = menuScreenA - SUB_MENU_COPY_PAK_FROM_GHOST_MIN;
                    if (sp30[sp38->work2].courseIndex == D_8018EE10[PLAYER_TWO].courseIndex &&
                        D_8018EE10[PLAYER_TWO].ghostDataSaved) {
                        menuScreenA = SUB_MENU_COPY_PAK_TO_GHOST2_2P;
                    } else {
                        menuScreenA = SUB_MENU_COPY_PAK_TO_GHOST1_2P;
                    }
                    playSound(SOUND_MENU_SELECT);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_TO_GHOST1_2P:
            case SUB_MENU_COPY_PAK_TO_GHOST2_2P: {
                // bit of a fake match, but if it works it works?
                if ((sp30[sp38->work2].courseIndex !=
                     ((0, (D_8018EE10 + (menuScreenA - SUB_MENU_COPY_PAK_TO_GHOST_MIN))->courseIndex))) ||
                    ((D_8018EE10 + (menuScreenA - SUB_MENU_COPY_PAK_TO_GHOST_MIN))->ghostDataSaved == 0)) {
                    if ((btnAndStick & D_JPAD) && (menuScreenA < SUB_MENU_COPY_PAK_TO_GHOST_MAX)) {
                        menuScreenA += 1;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->workf1 < 4.2) {
                            sp38->workf1 += 4.0;
                        }
                        sp38->pattern2 = 1;
                    }
                    if ((btnAndStick & U_JPAD) && (menuScreenA > SUB_MENU_COPY_PAK_TO_GHOST_MIN)) {
                        menuScreenA -= 1;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->workf1 < 4.2) {
                            sp38->workf1 += 4.0;
                        }
                        sp38->pattern2 = -1;
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    menuScreenA = sp38->work2 + SUB_MENU_COPY_PAK_FROM_GHOST_MIN;
                    playSound(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    sp38->work1 = menuScreenA - SUB_MENU_COPY_PAK_TO_GHOST_MIN;
                    if (D_8018EE10[(sp38->work1)].ghostDataSaved) {
                        menuScreenA = SUB_MENU_COPY_PAK_PROMPT_QUIT;
                    } else {
                        menuScreenA = SUB_MENU_COPY_PAK_START;
                        sp38->pattern = 0;
                    }
                    playSound(SOUND_MENU_SELECT);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA:
            case SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA:
            case SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P:
            case SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P:
            case SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P:
            case SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P:
            case SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P:
            case SUB_MENU_COPY_PAK_COMPLETED:
            case SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P:
            case SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    menuScreenA = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                    playSound(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_PROMPT_QUIT:
            case SUB_MENU_COPY_PAK_PROMPT_COPY: {
                if ((btnAndStick & R_JPAD) && menuScreenA < SUB_MENU_COPY_PAK_PROMPT_MAX) {
                    menuScreenA += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    sp38->pattern2 = 1;
                }
                if ((btnAndStick & L_JPAD) && menuScreenA > SUB_MENU_COPY_PAK_PROMPT_MIN) {
                    menuScreenA -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->workf1 < 4.2) {
                        sp38->workf1 += 4.0;
                    }
                    sp38->pattern2 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    menuScreenA = sp38->work1 + SUB_MENU_COPY_PAK_TO_GHOST_MIN;
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    if (menuScreenA == SUB_MENU_COPY_PAK_PROMPT_QUIT) {
                        menuScreenA = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                        playSound(SOUND_MENU_GO_BACK);
                    } else {
                        menuScreenA = SUB_MENU_COPY_PAK_START;
                        playSound(SOUND_MENU_SELECT);
                        sp38->pattern = 0;
                    }
                }
                // return?
                break;
            }
            case SUB_MENU_COPY_PAK_START: {
                if (controllerIdx == PLAYER_ONE) {
                    sp38->pattern += 1;
                }
                if (sp38->pattern >= 3) {
                    menuScreenA = SUB_MENU_COPY_PAK_COPYING;
                }
                break;
            }
            case SUB_MENU_COPY_PAK_COPYING: {
                res = CheckPakStatus2();
                if (res == PFS_NO_ERROR) {
                    res = LoadGameNoteKart_ghost2(sp38->work2, sp38->work1);
                }
                if (res != 0) {
                    menuScreenA = SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P;
                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                res = osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                    (u8*) gExtCode, &gControllerPak1FileNote);
                if (res == PFS_NO_ERROR) {
                    res = SaveGameNoteKart_ghost(sp38->work1);
                }
                if (res != 0) {
                    menuScreenA = SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P;
                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                menuScreenA = SUB_MENU_COPY_PAK_COMPLETED;
                D_8018EE10[sp38->work1].courseIndex = (sp30 + sp38->work2)->courseIndex;
                SaveGameNoteKart_data(sp38->work1);
                break;
            }
            case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT: {
                if (controllerIdx == PLAYER_ONE) {
                    sp38->pattern += 1;
                }
                if (sp38->pattern >= 3) {
                    menuScreenA = SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE;
                }
                break;
            }
            case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE: {
                if (makeGameNoteKartOnly()) {
                    menuScreenA = SUB_MENU_COPY_PAK_ERROR_CANT_CREATE_1P;
                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                } else if (sp30[0].ghostDataSaved) {
                    menuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST1_1P;
                } else {
                    menuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST2_1P;
                }
                break;
            }
            default:
                break;
        }
    }
}

/**
 * Navigation of the data menu
 */
void taselController(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    // Make pressing Start have the same effect as pressing A
    if (!g_DebugMode && ((btnAndStick & START_BUTTON) != 0)) {
        btnAndStick |= A_BUTTON;
    }

    if (FadeChecker() == 0) {
        if (menuScreenA == SUB_MENU_DATA) {
            // If DPad/Stick down pressed, move selection down if not already in bottom row
            if ((btnAndStick & D_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex % 4) != 3) {
                    ++gTimeTrialDataCourseIndex;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick up pressed, move selection up if not already in top row
            if ((btnAndStick & U_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex % 4) != 0) {
                    --gTimeTrialDataCourseIndex;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick right pressed, move selection right if not already in right-most column
            if ((btnAndStick & R_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex / 4) != 3) {
                    gTimeTrialDataCourseIndex += 4;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick left pressed, move selection left if not already in left-most column
            if ((btnAndStick & L_JPAD) != 0) {
                if ((gTimeTrialDataCourseIndex / 4) != 0) {
                    gTimeTrialDataCourseIndex -= 4;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If B pressed, go to main menu
            if ((btnAndStick & B_BUTTON) != 0) {
                SetFadeOutTaData();
                playSound(SOUND_MENU_GO_BACK);
                return;
            }
            // If A pressed, go to selected course's records
            if ((btnAndStick & A_BUTTON) != 0) {
                gCourseRecordsMenuSelection = COURSE_RECORDS_MENU_RETURN_MENU;
                SetFadeOut_f();
                playSound(SOUND_MENU_OK_CLICKED);
            }
        }
        // If menuScreenA is not SUB_MENU_DATA and A pressed, go to main menu
        // This condition is not reachable but this failsafe was added nonetheless
        else if ((btnAndStick & A_BUTTON) != 0) {
            SetFadeOutTaData();
            playSound(SOUND_MENU_OK_CLICKED);
        }
    }
}

/**
 * Navigation of the course records data menu
 */
void dselController(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick; // sp2E
    ObjBlock* sp28;
    CourseTimeTrialRecords* sp24;
    s32 res;

    btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    if (!g_DebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        switch (menuScreenA) {
            case SUB_MENU_DATA_OPTIONS: {
                if ((btnAndStick & L_JPAD) && (gTimeTrialDataCourseIndex > 0)) {
                    gTimeTrialDataCourseIndex -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }

                if ((btnAndStick & R_JPAD) && (gTimeTrialDataCourseIndex < 15)) {
                    gTimeTrialDataCourseIndex += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }

                sp28 = Get_OBJBlock_ptr(0xE8);
                sp24 = &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[gTimeTrialDataCourseIndex / 4]
                            .courseRecords[gTimeTrialDataCourseIndex % 4];
                if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_GHOST &&
                    CheckSameMap2(gTimeTrialDataCourseIndex) < 0) {
                    gCourseRecordsMenuSelection -= 1;
                }

                if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_RECORDS && sp24->unknownBytes[0] == 0) {
                    gCourseRecordsMenuSelection -= 1;
                }

                if ((btnAndStick & U_JPAD) && (gCourseRecordsMenuSelection > COURSE_RECORDS_MENU_MIN)) {
                    gCourseRecordsMenuSelection -= 1;
                    if (gCourseRecordsMenuSelection == 1 && sp24->unknownBytes[0] == 0) {
                        gCourseRecordsMenuSelection -= 1;
                    }
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->workf1 < 4.2) {
                        sp28->workf1 += 4.0;
                    }
                    sp28->pattern2 = -1;
                }

                if ((btnAndStick & D_JPAD) && (gCourseRecordsMenuSelection < COURSE_RECORDS_MENU_MAX)) {
                    gCourseRecordsMenuSelection += 1;
                    if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_RECORDS &&
                        sp24->unknownBytes[0] == 0) {
                        gCourseRecordsMenuSelection += 1;
                    }

                    if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_GHOST &&
                        CheckSameMap2(gTimeTrialDataCourseIndex) < 0) {
                        if (sp24->unknownBytes[0] == 0) {
                            gCourseRecordsMenuSelection = COURSE_RECORDS_MENU_RETURN_MENU;
                        } else {
                            gCourseRecordsMenuSelection = COURSE_RECORDS_MENU_ERASE_RECORDS;
                        }
                    } else {
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (sp28->workf1 < 4.2) {
                            sp28->workf1 += 4.0;
                        }
                        sp28->pattern2 = 1;
                    }
                }

                if (btnAndStick & B_BUTTON) {
                    SetFadeOutB();
                    playSound(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (sp28->workf1 < 4.2) {
                        sp28->workf1 += 4.0;
                    }
                    if (gCourseRecordsMenuSelection == COURSE_RECORDS_MENU_RETURN_MENU) {
                        SetFadeOutB();
                        playSound(SOUND_MENU_GO_BACK);
                    } else {
                        menuScreenA = SUB_MENU_DATA_ERASE_CONFIRM;
                        gCourseRecordsSubMenuSelection = COURSE_RECORDS_SUB_MENU_QUIT;
                        playSound(SOUND_MENU_SELECT);
                    }
                }
                break;
            }
            case SUB_MENU_DATA_ERASE_CONFIRM: {
                sp28 = Get_OBJBlock_ptr(0xE9);
                if ((btnAndStick & U_JPAD) && (gCourseRecordsSubMenuSelection > COURSE_RECORDS_SUB_MENU_MIN)) {
                    gCourseRecordsSubMenuSelection -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->workf1 < 4.2) {
                        sp28->workf1 += 4.0;
                    }
                    sp28->pattern2 = -1;
                }

                if ((btnAndStick & D_JPAD) && (gCourseRecordsSubMenuSelection < COURSE_RECORDS_SUB_MENU_MAX)) {
                    gCourseRecordsSubMenuSelection += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->workf1 < 4.2) {
                        sp28->workf1 += 4.0;
                    }
                    sp28->pattern2 = 1;
                }

                if (btnAndStick & B_BUTTON) {
                    menuScreenA = SUB_MENU_DATA_OPTIONS;
                    playSound(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (gCourseRecordsSubMenuSelection != COURSE_RECORDS_SUB_MENU_QUIT) {
                        res = 0;
                        switch (gCourseRecordsMenuSelection) {
                            case COURSE_RECORDS_MENU_ERASE_RECORDS: {
                                initEEPRomCourseData(gTimeTrialDataCourseIndex);
                                SaveFunc800B559CDefault(gTimeTrialDataCourseIndex);
                                playSound(SOUND_MENU_EXPLOSION);
                                res = -1;
                                break;
                            }
                            case COURSE_RECORDS_MENU_ERASE_GHOST: {
                                res = CheckSameMap2(gTimeTrialDataCourseIndex);
                                if (res >= 0) {
                                    if (initGameNoteKart(res) != 0) {
                                        menuScreenA = SUB_MENU_DATA_CANT_ERASE;
                                        playSound(SOUND_MENU_FILE_NOT_FOUND);
                                    } else {
                                        playSound(SOUND_MENU_EXPLOSION);
                                        menuScreenA = SUB_MENU_DATA_OPTIONS;
                                    }
                                }
                                break;
                            }
                        }

                        if (!(res + 1)) {
                            menuScreenA = SUB_MENU_DATA_OPTIONS;
                        }
                    } else {
                        playSound(SOUND_MENU_GO_BACK);
                        menuScreenA = SUB_MENU_DATA_OPTIONS;
                    }
                }
                break;
            }
            case SUB_MENU_DATA_CANT_ERASE: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    menuScreenA = SUB_MENU_DATA_OPTIONS;
                }
                break;
            }
        }
    }
}

/**
 * On input skip logo screen
 **/
void nintenLogoControler(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    // If any button is pressed then fade audio out
    if ((FadeChecker() == 0) && (btnAndStick)) {
        // TODO: Label audio functions
        Na_SE_Fadeout(0x3C);

        SetFadeOut_fw();
    }
}

/**
 * Navigation of the controller pak table data
 */
void cpControler(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick;
    OSPfsState* osPfsState;
    s32 selectedTableRow;
    UNUSED s8 pad;

    btnAndStick = controller->ButtonPressed | controller->AnalogPressed;
    if (FadeChecker() == 0) {
        switch (gControllerPakMenuSelection) {
            case CONTROLLER_PAK_MENU_SELECT_RECORD:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    playSound(SOUND_MENU_SELECT);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_END;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_END:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    playSound(SOUND_MENU_SELECT);
                    SetFadeOut_f();
                    gControllerPak1State = BAD;
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_TABLE_GAME_DATA:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    selectedTableRow = gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] - 1;
                    if (pfsError[selectedTableRow] == 0) {
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                        playSound(SOUND_MENU_SELECT);
                        return;
                    }
                } else if ((btnAndStick & B_BUTTON) != 0) {
                    if (gControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE) {
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                        playSound(SOUND_MENU_GO_BACK);
                        return;
                    }
                } else if ((btnAndStick & U_JPAD) != 0) {
                    if (gControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE) {
                        --gControllerPakSelectedTableRow;
                        if (gControllerPakSelectedTableRow < 0) {
                            gControllerPakSelectedTableRow = 0;
                            if (gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] != 1) {
                                gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_UP;
                                playSound(SOUND_MENU_CURSOR_MOVE);
                                return;
                            }
                        } else {
                            playSound(SOUND_MENU_CURSOR_MOVE);
                            return;
                        }
                    }
                } else if (((btnAndStick & D_JPAD) != 0) &&
                           (gControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE)) {
                    ++gControllerPakSelectedTableRow;
                    if (gControllerPakSelectedTableRow >= CONTROLLER_PAK_MENU_TABLE_GAME_DATA) {
                        gControllerPakSelectedTableRow = CONTROLLER_PAK_MENU_QUIT;
                        if (gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] != 16) {
                            gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_DOWN;
                            playSound(SOUND_MENU_CURSOR_MOVE);
                            return;
                        }
                    } else {
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        return;
                    }
                }
                break;
            case CONTROLLER_PAK_MENU_QUIT:
                if ((btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_GO_TO_ERASING;
                    playSound(SOUND_MENU_SELECT);
                    return;
                }
                if ((btnAndStick & B_BUTTON) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_GO_TO_ERASING:
                gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASING;
                return;
            case CONTROLLER_PAK_MENU_ERASING:
                selectedTableRow = gControllerPakVisibleTableRows[gControllerPakSelectedTableRow + 2] - 1;
                osPfsState = &pfsState[selectedTableRow];

                switch (osPfsDeleteFile(&gControllerPak1FileHandle, osPfsState->company_code, osPfsState->game_code,
                                        (u8*) &osPfsState->game_name, (u8*) &osPfsState->ext_name)) {
                    default:
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED;
                        return;
                    case 0:
                        pfsError[selectedTableRow] = -1;
                        gControllerPak1NumPagesFree += (((osPfsState->file_size + 0xFF) >> 8) & 0xFF);
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                        return;
                    case PFS_ERR_NOPACK:
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK;
                        return;
                    case PFS_ERR_NEW_PACK: /* switch 1 */
                        gControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED;
                        return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    gControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                }
                break;
        }
    }
}

/**
 * Navigation of the main splash start screen menu
 * Also handles debug menu options
 */
void TitleController(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick;
    u16 i;
    s32 isDebug;

    isDebug = true;
    btnAndStick = controller->ButtonPressed | controller->AnalogPressed;

    if (FadeChecker() == 0) {
        if (controllerIdx == PLAYER_ONE) {
            title_counter2 += 1;
        }
        switch (title_mode) {
            case DEBUG_MENU_DISABLED: {
                isDebug = false;
                if ((title_counter2 >= 46) && (btnAndStick & (A_BUTTON | START_BUTTON))) {
                    SetFadeOut_f();
                    NaSeqFadeout(0x19);
                    playSound(SOUND_INTRO_ENTER_MENU);
                } else {
                    break;
                }
                break;
            }
            case DEBUG_MENU_DEBUG_MODE: {
                if (btnAndStick & (R_JPAD | L_JPAD)) {
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (g_DebugMode) {
                        g_DebugMode = DEBUG_MODE_TOGGLE;
                    } else {
                        g_DebugMode = true;
                    }
                }
                if (btnAndStick & D_JPAD) {
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    title_mode = DEBUG_MENU_COURSE;
                }
                break;
            }
            case DEBUG_MENU_COURSE: {
                if (btnAndStick & R_JPAD) {
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (g_courseID < (NUM_COURSES - 2)) {
                        g_courseID += 1;
                    } else {
                        g_courseID = 0;
                    }
                }
                if (btnAndStick & L_JPAD) {
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (g_courseID > 0) {
                        g_courseID -= 1;
                    } else {
                        g_courseID = (NUM_COURSES - 2);
                    }
                }
                if (btnAndStick & U_JPAD) {
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    title_mode = DEBUG_MENU_DEBUG_MODE;
                }
                if (btnAndStick & D_JPAD) {
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    title_mode = DEBUG_MENU_SCREEN_MODE;
                }
                break;
            }
            case DEBUG_MENU_SCREEN_MODE: {
                if ((btnAndStick & R_JPAD) && (gScreenModeListIndex < 4)) {
                    gScreenModeListIndex += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    g_ScreenSplitB = sScreenModePlayerTable[gScreenModeListIndex];
                }
                if ((btnAndStick & L_JPAD) && (gScreenModeListIndex > 0)) {
                    gScreenModeListIndex -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    g_ScreenSplitB = sScreenModePlayerTable[gScreenModeListIndex];
                }
                if (btnAndStick & U_JPAD) {
                    title_mode = DEBUG_MENU_COURSE;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    title_mode = DEBUG_MENU_PLAYER;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_PLAYER: {
                if ((btnAndStick & R_JPAD) && (g_CharacterSelections[0] < 7)) {
                    g_CharacterSelections[0] += 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && (g_CharacterSelections[0] > 0)) {
                    g_CharacterSelections[0] -= 1;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & U_JPAD) {
                    title_mode = DEBUG_MENU_SCREEN_MODE;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    title_mode = DEBUG_MENU_SOUND_MODE;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_SOUND_MODE: {
                if ((btnAndStick & R_JPAD) && (sound_output < 3)) {
                    sound_output += 1;
                    if (sound_output == SOUND_UNUSED) {
                        sound_output = SOUND_MONO;
                    }
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    Setting_SoundOutMode();
                    eeprom_save_data.main.saveInfo.soundMode = sound_output;
                    SaveFunc800B4670Default();
                    SaveFunc800B5948Default();
                }
                if ((btnAndStick & L_JPAD) && (sound_output > 0)) {
                    sound_output -= 1;
                    if (sound_output == SOUND_UNUSED) {
                        sound_output = SOUND_HEADPHONES;
                    }
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    Setting_SoundOutMode();
                    eeprom_save_data.main.saveInfo.soundMode = sound_output;
                    SaveFunc800B4670Default();
                }
                if (btnAndStick & U_JPAD) {
                    title_mode = DEBUG_MENU_PLAYER;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    title_mode = DEBUG_MENU_GIVE_ALL_GOLD_CUP;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_GIVE_ALL_GOLD_CUP: {
                if (btnAndStick & U_JPAD) {
                    title_mode = DEBUG_MENU_SOUND_MODE;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & B_BUTTON) {
                    for (i = 0; i < 16; i++) {
                        SaveCupStatus2(0, i);
                    }
                    playSound(SOUND_MENU_SELECT);
                    break;
                } else if (btnAndStick & L_TRIG) {
                    initEEPRomFlgs1Data();
                    for (i = 0; i < 16; i++) {
                        SaveCupStatus2(i / 4, i);
                    }
                    playSound(SOUND_MENU_SELECT);
                    break;
                } else if (btnAndStick & L_JPAD) {
                    initEEPRomFlgs1Data();
                    for (i = 0; i < 16; i++) {
                        if (i % 4 == 2) {
                            SaveCupStatus2(0, i);
                        } else {
                            SaveCupStatus2(i / 4, i);
                        }
                    }
                    playSound(SOUND_MENU_SELECT);
                } else {
                    break;
                }
                break;
            }
            default:
                break;
        }

        g_playerCount = g_menuMultiplayerSelection = sScreenModePlayerCount[gScreenModeListIndex];

        if (isDebug) {
            if (btnAndStick & (A_BUTTON | START_BUTTON)) {
                SetFadeOut_f();
                NaSeqFadeout(0x19);
                title_mode = DEBUG_MENU_OPTION_SELECTED;

                if (controller->ButtonHeld & L_TRIG) {
                    g_DemoFlag = DEMO_MODE_ACTIVE;
                } else {
                    g_DemoFlag = DEMO_MODE_INACTIVE;
                }

                if (controller->ButtonHeld & Z_TRIG) {
                    if (btnAndStick & A_BUTTON) {
                        gDebugGotoScene = DEBUG_GOTO_ENDING;
                    } else {
                        gDebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA;
                    }
                }
                playSound(SOUND_MENU_OK_CLICKED);
            } else if ((btnAndStick & B_BUTTON) && (controller->ButtonHeld & Z_TRIG)) {
                SetFadeOut_f();
                NaSeqFadeout(0x19);
                title_mode = DEBUG_MENU_OPTION_SELECTED;
                gDebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT;
                playSound(SOUND_MENU_OK_CLICKED);
            } else if (btnAndStick & R_TRIG) {
                title_mode = DEBUG_MENU_DISABLED;
                playSound(SOUND_MENU_SELECT);
            }
        }
    }
}

void gsel_popup_data(void) {
    // For Grand Prix and Versus, this will be the CC mode selected. For Time Trials, it will
    // be whether 'Begin' or 'Data' is selected. Not used for Battle.
    s8 subMenuMode = gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]];
    // Determine which game mode was selected based on the number of players and the row selected on the main menu
    switch (gGameModePlayerSelection[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]) {
        case GRAND_PRIX:
            g_raceClass = subMenuMode;
            g_ItemSetFlag = 1;
            g_ScreenFlip = (subMenuMode == CC_EXTRA) ? 1 : 0;
            break;
        case VERSUS:
            g_raceClass = subMenuMode;
            g_ItemSetFlag = 1;
            g_ScreenFlip = (subMenuMode == CC_EXTRA) ? 1 : 0;
            break;
        case BATTLE:
            g_ItemSetFlag = 1;
            g_ScreenFlip = 0;
            break;
        case TIME_TRIALS:
            g_raceClass = CC_100;
            g_ScreenFlip = 0;
            g_ItemSetFlag = 0;

            if ((subMenuMode && subMenuMode) && subMenuMode) {}

            break;
    }
}

/**
 * Navigation of the main game mode select screen
 */
void GSelController(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick;
    s32 subMode; // subMode
    bool cursorMoved; // cursorMoved

    btnAndStick = controller->ButtonPressed | controller->AnalogPressed;
    if ((g_DebugMode == 0) && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (FadeChecker() == 0) {
        switch (menuScreenB) {
            case MAIN_MENU_NONE:
                break;
            case MAIN_MENU_PLAYER_SELECT:
                if ((btnAndStick & R_JPAD) && (g_menuMultiplayerSelection < 4)) {
                    g_menuMultiplayerSelection += 1;
                    FlashCounterReset();
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && (g_menuMultiplayerSelection >= 2)) {
                    g_menuMultiplayerSelection -= 1;
                    FlashCounterReset();
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                g_playerCount = g_menuMultiplayerSelection;
                switch (g_playerCount) {
                    case 1:
                        g_ScreenSplitB = SCREEN_MODE_1P;
                        break;
                    case 2:
                        g_ScreenSplitB = SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL;
                        break;
                    case 3:
                    case 4:
                        g_ScreenSplitB = SCREEN_MODE_3P_4P_SPLITSCREEN;
                        break;
                }
                if (btnAndStick & B_BUTTON) {
                    SetFadeOut_m(0x14);
                    NaSeqFadeout(0x19);
                    KBGNumberNext = MENU_FADE_TYPE_BACK;
                    playSound(SOUND_MENU_GO_BACK);
                    break;
                }
                if (btnAndStick & A_BUTTON) {
                    menuScreenB = MAIN_MENU_MODE_SELECT;
                    FlashCounterReset();
                    playSound(SOUND_MENU_SELECT);
                    break;
                }
                if (btnAndStick & L_TRIG) {
                    menuScreenB = MAIN_MENU_OPTION;
                    SetFadeOut_option();
                    playSound(SOUND_MENU_OPTION);
                    break;
                }
                if (btnAndStick & R_TRIG) {
                    menuScreenB = MAIN_MENU_DATA;
                    SetFadeOutTaData();
                    playSound(SOUND_MENU_DATA);
                    break;
                }
                break;
            case MAIN_MENU_MODE_SELECT:
                if (btnAndStick & D_JPAD) {
                    if (gGameModeMenuColumn[g_menuMultiplayerSelection - 1] < gPlayerModeSelection[g_menuMultiplayerSelection - 1]) {
                        gGameModeMenuColumn[g_menuMultiplayerSelection - 1] += 1;
                        FlashCounterReset();
                        playSound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                if (btnAndStick & U_JPAD) {
                    if (gGameModeMenuColumn[g_menuMultiplayerSelection - 1] > 0) {
                        gGameModeMenuColumn[g_menuMultiplayerSelection - 1] -= 1;
                        FlashCounterReset();
                        playSound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    menuScreenB = MAIN_MENU_PLAYER_SELECT;
                    FlashCounterReset();
                    playSound(SOUND_MENU_GO_BACK);
                    break;
                }
                
                if (btnAndStick & A_BUTTON) {
                    switch (gGameModePlayerSelection[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]) {
                        default:
                            menuScreenB = MAIN_MENU_OK_SELECT;
                            break;
                        case 0:
                            menuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            playSound(SOUND_MENU_GP);
                            break;
                        case 2:
                            menuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            playSound(SOUND_MENU_VERSUS);
                            break;
                        case 1:
                            menuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            playSound(SOUND_MENU_TIME_TRIALS);
                            break;
                        case 3:
                            menuScreenB = MAIN_MENU_OK_SELECT;
                            playSound(SOUND_MENU_BATTLE);
                            break;
                    }
                    FlashCounterReset();
                    titleDemo = 0;
                    break;
                }
                break;
            case MAIN_MENU_MODE_SUB_SELECT:
            case MAIN_MENU_MODE_SUB_SELECT_GO_BACK:
                if (controllerIdx == PLAYER_ONE) {
                    titleDemo++;
                    if ((titleDemo == 100) || !(titleDemo % 300)) {
                        switch (gGameModePlayerSelection[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]) {
                            case 0:
                            case 2:
                                playSound(SOUND_MENU_SELECT_LEVEL);
                                break;
                            default:
                                break;
                        }
                    }
                }

                subMode = gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]];
                if ((btnAndStick & U_JPAD) && (subMode > 0)) {
                    gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]] -= 1;
                    FlashCounterReset();
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    cursorMoved = false;
                    if (Check4GoldCup_150cc()) {
                        if (subMode < sGameModePlayerColumnExtra[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]) {
                            cursorMoved = true;
                        }
                    } else {
                        if (subMode < sGameModePlayerColumnDefault[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]) {
                            cursorMoved = true;
                        }
                    }
                    if (cursorMoved) {
                        gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]++;
                        FlashCounterReset();
                        playSound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                subMode = gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]];

                if (btnAndStick & B_BUTTON) {
                    menuScreenB = MAIN_MENU_MODE_SELECT;
                    FlashCounterReset();
                    playSound(SOUND_MENU_GO_BACK);
                    break;
                } 
                if (btnAndStick & A_BUTTON) {
                    FlashCounterReset();
                    if ((g_menuMultiplayerSelection == 1) && ((gGameModeMenuColumn - 1)[g_menuMultiplayerSelection] == 1) && (subMode == 1)) {
                        SetFadeOutTaData();
                        playSound(SOUND_MENU_DATA);
                    } else {
                        menuScreenB = MAIN_MENU_OK_SELECT;
                        playSound(SOUND_MENU_SELECT);
                        titleDemo = 0;
                    }
                    break;
                }
                break;
            case MAIN_MENU_OK_SELECT:
            case MAIN_MENU_OK_SELECT_GO_BACK:
                if (controllerIdx == PLAYER_ONE) {
                    titleDemo++;
                    if ((titleDemo == 60) || !(titleDemo % 300)) {
                        playSound(SOUND_MENU_OK);
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    switch (gGameModePlayerSelection[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]) {
                        case 0:
                        case 1:
                        case 2:
                            menuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            break;
                        default:
                        case 3:
                            menuScreenB = MAIN_MENU_MODE_SELECT;
                            break;
                    }
                    FlashCounterReset();
                    playSound(SOUND_MENU_GO_BACK);
                    titleDemo = 0;
                    break;
                }
                if (btnAndStick & A_BUTTON) {
                    SetFadeOut_f();
                    playSound(SOUND_MENU_OK_CLICKED);
                    gsel_popup_data();
                    break;
                }
                break;
            case MAIN_MENU_OPTION:
            case MAIN_MENU_DATA:
                break;
            default:
                break;
        }
        g_gameMode = gGameModePlayerSelection[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]];
    }
}

/**
 * Check if there is no currently selected and/or
 * hovered character at grid position `gridId`
 */
bool psel_check(s32 gridId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(g_player1Character); i++) {
        if (gridId == g_player1Character[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Navigation of the player select screen
 * Grid positions are from right to left, then top to bottom
 */
void PSelController(struct Controller* controller, u16 controllerIdx) {
    s8 i;
    s8 j;
    s32 selected;
    u16 btnAndStick;

    btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);
    if (!g_DebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        switch (menuScreenC) {
            case PLAYER_SELECT_MENU_MAIN:
                if (g_player1Character[controllerIdx] == 0) {
                    if (btnAndStick & B_BUTTON) {
                        SetFadeOutB();
                        playSound(SOUND_MENU_GO_BACK);
                    }
                    return;
                }

                if (btnAndStick & B_BUTTON) {
                    if (gCharacterGridIsSelected[controllerIdx] != false) {
                        gCharacterGridIsSelected[controllerIdx] = false;
                        playSound(SOUND_MENU_GO_BACK);
                    } else {
                        SetFadeOutB();
                        playSound(SOUND_MENU_GO_BACK);
                    }
                }
            
                if ((btnAndStick & A_BUTTON) && (gCharacterGridIsSelected[controllerIdx] == 0)) {
                    gCharacterGridIsSelected[controllerIdx] = true;
                    NAPlyVoiceStart(controllerIdx, ((sCharacterGridOrder - 1)[g_player1Character[controllerIdx]] * 0x10) + 0x2900800E);
                }

                selected = false;
                for (i = 0; i < ARRAY_COUNT(g_player1Character); i++) {
                    if ((g_player1Character[i] != 0) && (gCharacterGridIsSelected[i] == false)) {
                        selected = true;
                        break;
                    }
                }

                if (!selected) {
                    menuScreenC = PLAYER_SELECT_MENU_OK;
                    FlashCounterReset();
                    titleDemo = 0;
                }

                if (gCharacterGridIsSelected[controllerIdx] != false) {
                    break;
                }
                j = g_player1Character[controllerIdx];
                if ((btnAndStick & R_JPAD) && (btnAndStick & D_JPAD)) {
                    if ((g_player1Character[controllerIdx] == 1U) || (g_player1Character[controllerIdx] == 2U) || (g_player1Character[controllerIdx] == 3U)) {
                        j = g_player1Character[controllerIdx] + 5;
                        if (psel_check(j)) {
                            g_player1Character[controllerIdx] = j;
                            playSound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                }
                if ((btnAndStick & L_JPAD) && (btnAndStick & D_JPAD)) {
                    if ((g_player1Character[controllerIdx] == 2U) || (g_player1Character[controllerIdx] == 3U) || (g_player1Character[controllerIdx] == 4U)) {
                        j = g_player1Character[controllerIdx] + 3;
                        if (psel_check(j)) {
                            g_player1Character[controllerIdx] = j;
                            playSound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                }
                if ((btnAndStick & R_JPAD) && (btnAndStick & U_JPAD)) {
                    if ((g_player1Character[controllerIdx] == 5U) || (g_player1Character[controllerIdx] == 6U) || (g_player1Character[controllerIdx] == 7U)) {
                        j = g_player1Character[controllerIdx] - 3;
                        if (psel_check(j)) {
                            g_player1Character[controllerIdx] = j;
                            playSound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                }

                if ((btnAndStick & L_JPAD) && (btnAndStick & U_JPAD)) {
                    if ((g_player1Character[controllerIdx] == 6U) || (g_player1Character[controllerIdx] == 7U) || (g_player1Character[controllerIdx] == 8U)) {
                        j = g_player1Character[controllerIdx] - 5;
                        if (psel_check(j)) {
                            g_player1Character[controllerIdx] = j;
                            playSound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                    
                }
                if (btnAndStick & R_JPAD) {
                    if ((g_player1Character[controllerIdx] != 4U) && (g_player1Character[controllerIdx] != 8U)) {
                        j = g_player1Character[controllerIdx] + 1;
                        do {
                            if (psel_check(j)) {
                                g_player1Character[controllerIdx] = j;
                                playSound(SOUND_MENU_CURSOR_MOVE);
                                return;
                            }

                            j++;
                            if (j == 5 || j == 9) {
                                break;
                            }
                        } while (j < 10);
                    }
                    return;
                }
                if (btnAndStick & L_JPAD) {
                    if ((g_player1Character[controllerIdx] != 1U) && (g_player1Character[controllerIdx] != 5U)) {
                        j = g_player1Character[controllerIdx] - 1;
                        do {
                            if (psel_check(j)) {
                                g_player1Character[controllerIdx] = j;
                                playSound(SOUND_MENU_CURSOR_MOVE);
                                return;
                            }
                            
                            j--;
                            if (j == 0 || j == 4) {
                                break;
                            }
                        } while (j >= 0);
                    }
                    return;
                }

                if ((btnAndStick & U_JPAD) && (g_player1Character[controllerIdx] >= 5)) {
                    j = g_player1Character[controllerIdx] - 4;
                }
                if ((btnAndStick & D_JPAD) && (g_player1Character[controllerIdx] < 5)) {
                    j = g_player1Character[controllerIdx] + 4;
                }
                if (psel_check(j)) {
                    g_player1Character[controllerIdx] = j;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            case PLAYER_SELECT_MENU_OK:
            case PLAYER_SELECT_MENU_OK_GO_BACK:
                if (controllerIdx == 0) {
                    titleDemo++;
                    if (titleDemo == 0x3C || !(titleDemo % 300)) {
                        playSound(SOUND_MENU_OK);
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    menuScreenC = PLAYER_SELECT_MENU_MAIN;
                    gCharacterGridIsSelected[controllerIdx] = false;
                    playSound(SOUND_MENU_GO_BACK);
                    break;
                }
                if (btnAndStick & A_BUTTON) {
                    SetFadeOut_f();
                    playSound(SOUND_MENU_OK_CLICKED);
                    set_rival_car_name();
                }
                break;
            default:
                break;
        }

        if (g_player1Character[controllerIdx] != 0) {
            g_CharacterSelections[controllerIdx] = (sCharacterGridOrder - 1)[g_player1Character[controllerIdx]];
        }
    }
}

/**
 * Navigation of the map select course menu screen
 */
void MSelController(struct Controller* arg0, u16 controllerIdx) {
    u16 btnAndStick = (arg0->ButtonPressed | arg0->AnalogPressed);

    if ((!g_DebugMode) && ((btnAndStick & START_BUTTON) != 0)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        switch (menuScreenA) {
            case SUB_MENU_MAP_SELECT_CUP:
                if ((btnAndStick & R_JPAD) != 0) {
                    if (g_cupSelect < SPECIAL_CUP) {
                        before_cup_select = g_cupSelect;
                        ++g_cupSelect;
                        FlashCounterReset();
                        playSound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                if (((btnAndStick & L_JPAD) != 0) && (g_cupSelect > MUSHROOM_CUP)) {
                    before_cup_select = g_cupSelect;
                    --g_cupSelect;
                    FlashCounterReset();
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }

                cup_level = g_cupSelect;
                g_courseID = gCupCourseOrder[g_cupSelect][g_courseSelect];
                if ((btnAndStick & B_BUTTON) != 0) {
                    SetFadeOutB();
                    playSound(SOUND_MENU_GO_BACK);
                } else if ((btnAndStick & A_BUTTON) != 0) {
                    if (g_gameMode != GRAND_PRIX) {
                        menuScreenA = SUB_MENU_MAP_SELECT_COURSE;
                        playSound(SOUND_MENU_SELECT);
                    } else {
                        menuScreenA = SUB_MENU_MAP_SELECT_OK;
                        playSound(SOUND_MENU_SELECT);
                        g_courseID = gCupCourseOrder[g_cupSelect][COURSE_ONE];
                        titleDemo = 0;
                    }
                    FlashCounterReset();
                }
                break;
            case SUB_MENU_MAP_SELECT_COURSE:
            case SUB_MENU_MAP_SELECT_BATTLE_COURSE:
                if (((btnAndStick & D_JPAD) != 0) && (g_courseSelect < COURSE_FOUR)) {
                    ++g_courseSelect;
                    FlashCounterReset();
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }
                if (((btnAndStick & U_JPAD) != 0) && (g_courseSelect > COURSE_ONE)) {
                    --g_courseSelect;
                    FlashCounterReset();
                    playSound(SOUND_MENU_CURSOR_MOVE);
                }

                g_courseID = gCupCourseOrder[g_cupSelect][g_courseSelect];
                if ((btnAndStick & B_BUTTON) != 0) {
                    if (menuScreenA == SUB_MENU_MAP_SELECT_COURSE) {
                        menuScreenA = SUB_MENU_MAP_SELECT_CUP;
                    } else {
                        SetFadeOutB();
                    }
                    FlashCounterReset();
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & A_BUTTON) != 0) {
                    menuScreenA = SUB_MENU_MAP_SELECT_OK;
                    playSound(SOUND_MENU_SELECT);
                    FlashCounterReset();
                    titleDemo = 0;
                }
                break;
            case SUB_MENU_MAP_SELECT_OK:
                if ((controllerIdx == PLAYER_ONE) &&
                    ((++titleDemo == 0x3C) || ((titleDemo % 300) == 0))) {
                    playSound(SOUND_MENU_OK);
                }

                if ((btnAndStick & B_BUTTON) != 0) {
                    switch (g_gameMode) {
                        case GRAND_PRIX:
                            menuScreenA = SUB_MENU_MAP_SELECT_CUP;
                            break;
                        case BATTLE:
                            menuScreenA = SUB_MENU_MAP_SELECT_BATTLE_COURSE;
                            break;
                        default:
                            menuScreenA = SUB_MENU_MAP_SELECT_COURSE;
                            break;
                    }

                    FlashCounterReset();
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & A_BUTTON) != 0) {
                    SetFadeOut_f();
                    NaSeqFadeout(0x19);
                    playSound(SOUND_MENU_OK_CLICKED);
                }
                break;
        }
    }
}

/**
 * Loads menu states so they are preserved between menu changes
 */
void k_var_init(s32 menuSelection) {
    s32 i;

    title_mode = DEBUG_MENU_SELECTION;
    titleDemo = 0;
    title_counter2 = 0;
    demo_play_flag = 0;
    replay_flag = 0;
    retry_flag = 0;
    gDebugGotoScene = DEBUG_GOTO_RACING;
    g_InGameTT = 0;
    D_8016556E = 0;
    ghost_status = 1;
    replayghost_status = 1;
    D_80162E00 = 0;
    read_pointer = 1;
    write_pointer = 0;

    switch (menuSelection) {
        case OPTIONS_MENU:
            menuScreenA = SUB_MENU_OPTION_RETURN_GAME_SELECT;
            break;
        case DATA_MENU:
            menuScreenA = SUB_MENU_DATA;
            break;
        case COURSE_DATA_MENU:
            menuScreenA = SUB_MENU_DATA_OPTIONS;
            break;
        case LOGO_INTRO_MENU:
            Na_ChangeSoundMode(0, 0);
            break;
        case CONTROLLER_PAK_MENU: {
            gControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
            Na_ChangeSoundMode(0, 0);
            break;
        }
        case 0:
        case START_MENU: {
            g_ScreenFlip = 0;
            g_DebugMode = DEBUG_MODE_TOGGLE;
            g_cupSelect = MUSHROOM_CUP;
            g_courseSelect = 0;
            gTimeTrialDataCourseIndex = 0;
            if (g_menuMultiplayerSelection <= 0) {
                g_menuMultiplayerSelection = 1;
            }
            if (g_menuMultiplayerSelection >= 5) {
                g_menuMultiplayerSelection = 4;
            }
            gScreenModeListIndex = sScreenModeIdxFromPlayerMode[g_menuMultiplayerSelection - 1];
            Na_ChangeSoundMode(0, 0);
            NaSeqStart(SEQ_MENU_TITLE_SCREEN);
            g_InGame = 0;
            break;
        }
        case 1:
        case MAIN_MENU: {
            g_DebugMode = DEBUG_MODE_TOGGLE;
            g_ScreenFlip = 0;
            g_InGame = 0;
            initPak();
            init_enemy_game_mode();

            if (gGamestate != 0) {
                Na_ChangeSoundMode(0, 0);
                Nas_AudioInput();
                gGamestate = 0;
                g_NewSequenceMode = 0;
                NaSeqStart(SEQ_MENU_MAIN_MENU);
            }

            switch (KBGNumberNext) {
                case MENU_FADE_TYPE_MAIN: {
                    menuScreenB = MAIN_MENU_PLAYER_SELECT;
                    NaSeqStart(SEQ_MENU_MAIN_MENU);
                    g_menuMultiplayerSelection = 1;
                    if (g_ScreenSplitB >= NUM_SCREEN_MODES || g_ScreenSplitB < 0) {
                        g_ScreenSplitB = SCREEN_MODE_1P;
                    }
                    break;
                }
                case MENU_FADE_TYPE_BACK: {
                    menuScreenB = MAIN_MENU_OK_SELECT_GO_BACK;
                    break;
                }
                case MENU_FADE_TYPE_DATA: {
                    // why...
                    switch (menuScreenB) {
                        default:
                            menuScreenB = MAIN_MENU_MODE_SUB_SELECT_GO_BACK;
                            break;
                        case MAIN_MENU_OPTION:
                        case MAIN_MENU_DATA:
                            menuScreenB = MAIN_MENU_PLAYER_SELECT;
                            break;
                    }
                    break;
                }
                case MENU_FADE_TYPE_OPTION: {
                    menuScreenB = MAIN_MENU_PLAYER_SELECT;
                    break;
                }
            }
            break;
        }
        case 2:
        case CHARACTER_SELECT_MENU: {
            switch (KBGNumberNext) {
                case MENU_FADE_TYPE_MAIN: {
                    menuScreenC = PLAYER_SELECT_MENU_MAIN;
                    if (gGamestate == 0) {
                        for (i = 0; i < ARRAY_COUNT(g_player1Character); i++) {
                            if (i < g_menuMultiplayerSelection) {
                                g_player1Character[i] = i + 1;
                            } else {
                                g_player1Character[i] = 0;
                            }
                            gCharacterGridIsSelected[i] = false;
                            g_CharacterSelections[i] = i;
                        }
                        playSound(SOUND_MENU_SELECT_PLAYER);
                    } else {
                        Na_ChangeSoundMode(0, 0);
                        Nas_AudioInput();
                        gGamestate = 0;
                        g_NewSequenceMode = 0;
                        NaSeqStart(SEQ_MENU_MAIN_MENU);
                        for (i = 0; i < ARRAY_COUNT(gCharacterGridIsSelected); i++) {
                            gCharacterGridIsSelected[i] = false;
                        }
                    }
                    break;
                }
                case MENU_FADE_TYPE_BACK: {
                    menuScreenC = PLAYER_SELECT_MENU_OK_GO_BACK;
                    for (i = 0; i < ARRAY_COUNT(gCharacterGridIsSelected); i++) {
                        if (g_menuMultiplayerSelection > i) {
                            gCharacterGridIsSelected[i] = true;
                        } else {
                            gCharacterGridIsSelected[i] = false;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 3:
        case COURSE_SELECT_MENU: {
            if (g_gameMode == BATTLE) {
                g_cupSelect = BATTLE_CUP;
                cup_level = 4;
                menuScreenA = SUB_MENU_MAP_SELECT_BATTLE_COURSE;
            } else {
                if (g_cupSelect == BATTLE_CUP) {
                    g_cupSelect = MUSHROOM_CUP;
                }
                menuScreenA = SUB_MENU_MAP_SELECT_CUP;
            }
            if (gGamestate != 0) {
                Na_ChangeSoundMode(0, 0);
                Nas_AudioInput();
                gGamestate = 0;
                g_NewSequenceMode = 0;
                NaSeqStart(SEQ_MENU_MAIN_MENU);
            }
            playSound(SOUND_MENU_SELECT_MAP);
            before_cup_select = 0;
            if (g_gameMode == GRAND_PRIX) {
                g_courseSelect = 0;
            }

            for (i = 0; i < ARRAY_COUNT(g_playerGPpoints); i++) {
                g_playerGPpoints[i] = 0;
            }
            break;
        }
    }
    FlashCounterReset();
}

/**
 * Self explanatory, gets reset when moving cursor option or after a fade
 */
void FlashCounterReset(void) {
    k_flash_counter = 0x20;
}

/**
 * Self explanatory, changes sound mode pack
 */
void Setting_SoundOutMode(void) {
    UNUSED u32 pad;
    union GameModePack pack;

    pack = sSoundMenuPack;
    if ((sound_output == SOUND_STEREO) || (sound_output == SOUND_HEADPHONES) || (sound_output == SOUND_MONO)) {
        Nai_SeqBufFlagCheck(pack.modes[sound_output] | 0xE0000000);
    }
}

/**
 * Checks is a fade render mode is active so menus can't be
 * interacted while a fade transition is active
 */
bool FadeChecker(void) {
    if ((g_FadingFlag[4] == 2) || (g_FadingFlag[4] == 3) || (g_FadingFlag[4] == 4) ||
        (g_FadingFlag[4] == 7)) {
        return true;
    }
    return false;
}

/**
 * Unused debug function, prints the character id for the player and both controller pak ghosts
 */
UNUSED void GhostChecker(s32 arg0, s32 arg1) {
    struct_8018EE10_entry* pak1 = D_8018EE10;
    struct_8018EE10_entry* pak2 = (struct_8018EE10_entry*) kdl_buff;

    rmonPrintf("ghost_kart=%d,", ghost_kart);
    rmonPrintf("pak1_ghost_kart=%d,", (pak1 + arg0)->characterId);
    rmonPrintf("pak2_ghost_kart=%d\n", (pak2 + arg1)->characterId);
}
