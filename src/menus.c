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
// Variables Used to maniplate the Model for Intro Logo
// and checkerboard on the Start screen
s32 LogoCamZ;
f32 LogoScaleZ; // XYZ Scale on checkerboard Flag, Z Scale on intro logo
f32 LogoRotX;
f32 LogoRotY;
f32 LogoRotZ;
f32 LogoPosX;
f32 LogoPosY;
f32 LogoPosZ;

s32 KBGNumberNext;
s8 Player1Character[4];    // Map from each Player to Current grid Position (1-4 top, 5-8 bottom)
bool8 CharacterGridIsSelected[4]; // Sets true if a character is selected for each Player
s8 MenuScreenA;              // Map Select states, Options and Ghost Data text selection
s8 MenuScreenB;
s8 MenuScreenC;
s8 TitleMode;
s8 ControllerPakMenuSelection;
s8 ScreenModeListIndex; // 0-4 Index, selects a screen Mode in SScreenModePlayerTable
u8 SoundOutput;
s8 MenuMultiplayerSelection;
s8 VSResultCursor;     // 4 options indexed (10-13), gets set when selecting an option
s8 TaResultCursor; // 5 options indexed (5-9), gets set when selecting an option (excluding Save
                                     // Ghost)
s8 BtResultCursor;     // 4 options indexed (10-13), gets set when selecting an option
s8 TimeTrialDataCourseIndex;
s8 CourseRecordsMenuSelection;    // Used for selecting an option in course record data
s8 CourseRecordsSubMenuSelection; // Used for Erase records and ghosts (Quit - Erase)
s8 DebugGotoScene;
bool8 InGameTT;
bool8 InGame;
s32 TitleDemo;
s32 TitleCounter2;
s8 DemoPlayFlag; // Sets true alongside DemoFlag, controller related
s8 CupSelect;
s8 BeforeCupSelect; // Same as CupSelect but it's only set in Map Select, not referenced
s8 CourseSelect;
s8 FBClearFlg; // Set to 0 but never referenced

/** Data **/
s32 KBGNumber = LOGO_INTRO_MENU;
s32 KBGChange = FADE_MODE_NONE;

// Default selected character for each Player
s8 CharacterSelections[4] = { MARIO, LUIGI, YOSHI, TOAD };

// The Current row selected in the Mode column for each Player indexed
// 0-1 1p / 0-2 2p´/ 0-1 3p / 0-1 4p
s8 GameModeMenuColumn[4] = { 0, 0, 0, 0 };

// For Grand Prix and Versus, this will be the CC Mode selected. For Time Trials, it will
// be whether 'Begin' or 'Data' is selected. Not Used for Battle.
// indexed as [column][row]
s8 GameModeSubMenuColumn[4][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

s8 NextDemoId = 0;
s8 ControllerPakSelectedTableRow = 0; // 0-4 Index, Value of the Current Visible row Select

// Numbers starting from the second 0 to Number 6 get altered
// as you move up or down the page Table of content (min Value is 0, max Value is 16
s8 ControllerPakVisibleTableRows[12] = { 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0 };

s8 ControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE; // 1 is down, 2 is up
s8 UnrefD800E86D4[12] = { 0 };
s8 UnrefD800E86E0[4] = { 0, 0, 0, 1 };

u32 SVIGammaOffDitherOn = (OS_VI_GAMMA_OFF | OS_VI_DITHER_FILTER_ON);

/** RoData **/

// Sets the actual screen Mode based on values set in SScreenModePlayerCount
const s8 SScreenModePlayerTable[] = { SCREEN_MODE_1P, SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL,
                                      SCREEN_MODE_2P_SPLITSCREEN_VERTICAL, SCREEN_MODE_3P_4P_SPLITSCREEN,
                                      SCREEN_MODE_3P_4P_SPLITSCREEN };

// Sets how many players can load on each screen Mode set in SScreenModePlayerTable
const s8 SScreenModePlayerCount[] = { 1, 2, 2, 3, 4 };

// Set indexed slots numbers for one-two-three-four Mode selection
const s8 PlayerModeSelection[] = { 1, 2, 1, 1 };

// Limit for each Index column in one-two-three-four Mode selection
const s8 SGameModePlayerColumnDefault[][3] = {
    { 2, 1, 0 }, // 1p (GP options, TT options, ...)
    { 2, 2, 0 }, // 2p (GP options, VS options, Battle)
    { 2, 0, 0 }, // 3p (VS options, Battle, ...)
    { 2, 0, 0 }, // 4p (VS options, Battle, ...)
};

// Limit for each Index column in one-two-three-four Mode selection
// for extra Mode (mirror Mode), hence the extra Value (3 instead of 2)
const s8 SGameModePlayerColumnExtra[][3] = {
    { 3, 1, 0 }, // 1p (GP options, TT options, ...)
    { 3, 3, 0 }, // 2p (GP options, VS options, Battle)
    { 3, 0, 0 }, // 3p (VS options, Battle, ...)
    { 3, 0, 0 }, // 4p (VS options, Battle, ...)
};

// Modes to Select in one-two-three-four Mode selection
const s32 GameModePlayerSelection[][3] = {
    { GRAND_PRIX, TIME_TRIALS, 0x00000000 }, // 1p game Modes
    { GRAND_PRIX, VERSUS, BATTLE },          // 2p game Modes
    { VERSUS, BATTLE, 0x00000000 },          // 3p game Modes
    { VERSUS, BATTLE, 0x00000000 },          // 4p game Modes
};

// Map from character grid Position id to character id
// Note: changing Order doesn't affect graphics, only the selection
const s8 SCharacterGridOrder[] = {
    MARIO, LUIGI, PEACH, TOAD, YOSHI, DK, WARIO, BOWSER,
};

s16 CupCourseOrder[5][4] = {
    // mushroom cup
    { COURSE_LUIGI_RACEWAY, COURSE_MOO_MOO_FARM, COURSE_KOOPA_BEACH, COURSE_KALAMARI_DESERT },
    // flower cup
    { COURSE_TOADS_TURNPIKE, COURSE_FRAPPE_SNOWLAND, COURSE_CHOCO_MOUNTAIN, COURSE_MARIO_RACEWAY },
    // Star cup
    { COURSE_WARIO_STADIUM, COURSE_SHERBET_LAND, COURSE_ROYAL_RACEWAY, COURSE_BOWSER_CASTLE },
    // special cup
    { COURSE_DK_JUNGLE, COURSE_YOSHI_VALLEY, COURSE_BANSHEE_BOARDWALK, COURSE_RAINBOW_ROAD },
    // battle Mode
    { COURSE_BIG_DONUT, COURSE_BLOCK_FORT, COURSE_DOUBLE_DECK, COURSE_SKYSCRAPER },
};

const s8 Unref800F2BDC[4] = { 1, 0, 0, 0 };

// Uses Player Count to set ScreenModeListIndex, the latter variable then selects a Mode
// from SScreenModePlayerTable, Note the 2 is not set since that's for vertical 2p screen
const s8 SScreenModeIdxFromPlayerMode[4] = { 0, 1, 3, 4 };

const union GameModePack SSoundMenuPack = { { SOUND_STEREO, SOUND_HEADPHONES, SOUND_UNUSED, SOUND_MONO } };

/**************************/

/**
 * General menu Main handler
 * Includes opening logo and splash screens
 */
void UpdateController(void) {
    u16 controllerIdx;

    if (KBGChange == FADE_MODE_NONE) {
        for (controllerIdx = 0; controllerIdx < 4; controllerIdx++) {
            // Debug, quick Jump through menus using the Start Button.
            if ((FadeChecker() == 0) && (DebugMode) &&
                ((Controllers[controllerIdx].ButtonPressed & START_BUTTON) != 0)) {
                // this is certainly a way to write these...
                switch (KBGNumber) {
                    case COURSE_SELECT_MENU:
                        NaSeqFadeout(0x19);
                        // deliberate (?) fallthru
                    case MAIN_MENU:
                    case CHARACTER_SELECT_MENU:
                        PlaySound(SOUND_MENU_OK_CLICKED);
                        break;
                }

                switch (KBGNumber) {
                    case CONTROLLER_PAK_MENU:
                    case START_MENU:
                        break;
                    default:
                        SetFadeOutF();
                }
            }
            osViSetSpecialFeatures(SVIGammaOffDitherOn);
            switch (KBGNumber) {
                case OPTIONS_MENU:
                    OptionController(&Controllers[controllerIdx], controllerIdx);
                    break;
                case DATA_MENU:
                    DataMenuController(&Controllers[controllerIdx], controllerIdx);
                    break;
                case COURSE_DATA_MENU:
                    DselController(&Controllers[controllerIdx], controllerIdx);
                    break;
                case LOGO_INTRO_MENU:
                    NintenLogoControler(&Controllers[controllerIdx], controllerIdx);
                    break;
                case CONTROLLER_PAK_MENU:
                    if (controllerIdx == PLAYER_ONE) {
                        CpControler(&Controllers[controllerIdx], controllerIdx);
                    }
                    break;
                case START_MENU_FROM_QUIT:
                case START_MENU:
                    TitleMenuSwitch(&Controllers[controllerIdx], controllerIdx);
                    break;
                case MAIN_MENU_FROM_QUIT:
                case MAIN_MENU:
                    GameSelectSwitch(&Controllers[controllerIdx], controllerIdx);
                    break;
                case PLAYER_SELECT_MENU_FROM_QUIT:
                case CHARACTER_SELECT_MENU:
                    PSelController(&Controllers[controllerIdx], controllerIdx);
                    break;
                case COURSE_SELECT_MENU_FROM_QUIT:
                case COURSE_SELECT_MENU:
                    MapSelectSwitch(&Controllers[controllerIdx], controllerIdx);
                    break;
            }
        }
    }
}

/**
 * Navigation of the options menu
 */
void OptionController(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick; // sp3E
    ObjBlock* sp38;
    s32 res;
    struct_8018EE10_entry* sp30;
    bool tempVar; // cursorWasMoved or communicateStoredAction
    UNUSED u32 pad;

    btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    if (!DebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        sp38 = GetOBJBlockPtr(0xF0);
        sp30 = (struct_8018EE10_entry*) KdlBuff;
        switch (MenuScreenA) {
            case SUB_MENU_OPTION_RETURN_GAME_SELECT:
            case SUB_MENU_OPTION_SOUND_MODE:
            case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
            case SUB_MENU_OPTION_ERASE_ALL_DATA: {
                tempVar = false;
                if ((btnAndStick & D_JPAD) && (MenuScreenA < SUB_MENU_OPTION_MAX)) {
                    MenuScreenA += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    sp38->Pattern2 = 1;
                    tempVar = true;
                }
                if ((btnAndStick & U_JPAD) && (MenuScreenA > SUB_MENU_OPTION_MIN)) {
                    MenuScreenA -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    tempVar = true;
                    sp38->Pattern2 = -1;
                }
                if (tempVar && SoundOutput != sp38->Pattern) {
                    EEPRomSaveData.Main.SaveInfo.SoundMode = SoundOutput;
                    SaveFunc800B4670Default();
                    SaveFunc800B5948Default();
                    sp38->Pattern = SoundOutput;
                }
                if (btnAndStick & B_BUTTON) {
                    SetFadeOutOption();
                    PlaySound(SOUND_MENU_GO_BACK);
                    if (SoundOutput != sp38->Pattern) {
                        EEPRomSaveData.Main.SaveInfo.SoundMode = SoundOutput;
                        SaveFunc800B4670Default();
                        SaveFunc800B5948Default();
                        sp38->Pattern = SoundOutput;
                    }
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (MenuScreenA) {
                        case SUB_MENU_OPTION_SOUND_MODE:
                            if (SoundOutput < 3) {
                                SoundOutput += 1;
                            } else {
                                SoundOutput = SOUND_STEREO;
                            }
                            if (SoundOutput == SOUND_UNUSED) {
                                SoundOutput = SOUND_MONO;
                            }
                            SettingSoundOutMode();
                            switch (SoundOutput) {
                                case SOUND_STEREO:
                                    PlaySound(SOUND_MENU_STEREO);
                                    return;
                                case SOUND_HEADPHONES:
                                    PlaySound(SOUND_MENU_HEADPHONES);
                                    return;
                                case SOUND_MONO:
                                    PlaySound(SOUND_MENU_MONO);
                                    return;
                            }
                            break;
                        case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
                            switch (CheckPakStatus2()) {
                                case PFS_INVALID_DATA:
                                    MenuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA;
                                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_NO_ERROR:
                                    LoadGameNoteKartData2();
                                    tempVar = CheckPakStatus();
                                    switch (tempVar) {
                                        case PFS_INVALID_DATA:
                                            MenuScreenA = SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT;
                                            sp38->Pattern = 0;
                                            PlaySound(SOUND_MENU_SELECT);
                                            break;
                                        case PFS_NO_ERROR:
                                            LoadGameNoteKartData();
                                            break;
                                        case PFS_NO_PAK_INSERTED:
                                            MenuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P;
                                            PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_FILE_OVERFLOW:
                                            MenuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P;
                                            PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                        case PFS_PAK_BAD_READ:
                                        case PFS_PAK_CORRUPTED: // unreachable, bad reads always returns previous case
                                        default:
                                            MenuScreenA = SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P;
                                            PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                            break;
                                    }
                                    if (tempVar == PFS_INVALID_DATA && !sp30[PLAYER_ONE].GhostDataSaved &&
                                        !sp30[PLAYER_TWO].GhostDataSaved) {
                                        MenuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA;
                                        PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                        return;
                                    }
                                    if (tempVar == PFS_NO_ERROR) {
                                        if (sp30[PLAYER_ONE].GhostDataSaved) {
                                            MenuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST1_1P;
                                            PlaySound(SOUND_MENU_SELECT);
                                        } else if (sp30[PLAYER_TWO].GhostDataSaved) {
                                            MenuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST2_1P;
                                            PlaySound(SOUND_MENU_SELECT);
                                        } else {
                                            MenuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA;
                                            PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                        }
                                    }
                                    // else return?
                                    return;
                                case PFS_NO_PAK_INSERTED:
                                    MenuScreenA = SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P;
                                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                case PFS_PAK_BAD_READ:
                                default:
                                    MenuScreenA = SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P;
                                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                            }
                        case SUB_MENU_OPTION_ERASE_ALL_DATA: {
                            MenuScreenA = SUB_MENU_ERASE_QUIT;
                            PlaySound(SOUND_MENU_SELECT);
                            return;
                        }
                        case SUB_MENU_OPTION_RETURN_GAME_SELECT: {
                            SetFadeOutOption();
                            PlaySound(SOUND_MENU_GO_BACK);
                            return;
                        }
                    }
                }
                // maybe else return?;
                break;
            }
            case SUB_MENU_ERASE_QUIT:
            case SUB_MENU_ERASE_ERASE: {
                if ((btnAndStick & D_JPAD) && (MenuScreenA < SUB_MENU_ERASE_MAX)) {
                    MenuScreenA += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    sp38->Pattern2 = 1;
                }
                if ((btnAndStick & U_JPAD) && (MenuScreenA > SUB_MENU_ERASE_MIN)) {
                    MenuScreenA -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    sp38->Pattern2 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    MenuScreenA = SUB_MENU_OPTION_ERASE_ALL_DATA;
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    switch (MenuScreenA) {
                        case SUB_MENU_ERASE_QUIT:
                            MenuScreenA = SUB_MENU_OPTION_ERASE_ALL_DATA;
                            PlaySound(SOUND_MENU_GO_BACK);
                            break;
                        case SUB_MENU_ERASE_ERASE:
                            MenuScreenA = SUB_MENU_SAVE_DATA_ERASED;
                            InitEEPRom();
                            D_800DC5AC = 0;
                            PlaySound(SOUND_MENU_EXPLOSION);
                            break;
                    }
                }
                break; // or return?
            }
            case SUB_MENU_SAVE_DATA_ERASED: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    MenuScreenA = SUB_MENU_OPTION_ERASE_ALL_DATA;
                    PlaySound(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_FROM_GHOST1_1P:
            case SUB_MENU_COPY_PAK_FROM_GHOST2_1P: {
                if ((btnAndStick & D_JPAD) && (MenuScreenA < SUB_MENU_COPY_PAK_FROM_GHOST_MAX) &&
                    (sp30[PLAYER_TWO].GhostDataSaved)) {
                    MenuScreenA += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    sp38->Pattern2 = 1;
                }
                if ((btnAndStick & U_JPAD) && (MenuScreenA > SUB_MENU_COPY_PAK_FROM_GHOST_MIN) &&
                    sp30[PLAYER_ONE].GhostDataSaved) {
                    MenuScreenA -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    sp38->Pattern2 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    MenuScreenA = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    sp38->Work2 = MenuScreenA - SUB_MENU_COPY_PAK_FROM_GHOST_MIN;
                    if (sp30[sp38->Work2].CourseIndex == D_8018EE10[PLAYER_TWO].CourseIndex &&
                        D_8018EE10[PLAYER_TWO].GhostDataSaved) {
                        MenuScreenA = SUB_MENU_COPY_PAK_TO_GHOST2_2P;
                    } else {
                        MenuScreenA = SUB_MENU_COPY_PAK_TO_GHOST1_2P;
                    }
                    PlaySound(SOUND_MENU_SELECT);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_TO_GHOST1_2P:
            case SUB_MENU_COPY_PAK_TO_GHOST2_2P: {
                // bit of a fake match, but if it works it works?
                if ((sp30[sp38->Work2].CourseIndex !=
                     ((0, (D_8018EE10 + (MenuScreenA - SUB_MENU_COPY_PAK_TO_GHOST_MIN))->CourseIndex))) ||
                    ((D_8018EE10 + (MenuScreenA - SUB_MENU_COPY_PAK_TO_GHOST_MIN))->GhostDataSaved == 0)) {
                    if ((btnAndStick & D_JPAD) && (MenuScreenA < SUB_MENU_COPY_PAK_TO_GHOST_MAX)) {
                        MenuScreenA += 1;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->Workf1 < 4.2) {
                            sp38->Workf1 += 4.0;
                        }
                        sp38->Pattern2 = 1;
                    }
                    if ((btnAndStick & U_JPAD) && (MenuScreenA > SUB_MENU_COPY_PAK_TO_GHOST_MIN)) {
                        MenuScreenA -= 1;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (sp38->Workf1 < 4.2) {
                            sp38->Workf1 += 4.0;
                        }
                        sp38->Pattern2 = -1;
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    MenuScreenA = sp38->Work2 + SUB_MENU_COPY_PAK_FROM_GHOST_MIN;
                    PlaySound(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    sp38->Work1 = MenuScreenA - SUB_MENU_COPY_PAK_TO_GHOST_MIN;
                    if (D_8018EE10[(sp38->Work1)].GhostDataSaved) {
                        MenuScreenA = SUB_MENU_COPY_PAK_PROMPT_QUIT;
                    } else {
                        MenuScreenA = SUB_MENU_COPY_PAK_START;
                        sp38->Pattern = 0;
                    }
                    PlaySound(SOUND_MENU_SELECT);
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
                    MenuScreenA = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                    PlaySound(SOUND_MENU_GO_BACK);
                }
                break;
            }
            case SUB_MENU_COPY_PAK_PROMPT_QUIT:
            case SUB_MENU_COPY_PAK_PROMPT_COPY: {
                if ((btnAndStick & R_JPAD) && MenuScreenA < SUB_MENU_COPY_PAK_PROMPT_MAX) {
                    MenuScreenA += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    sp38->Pattern2 = 1;
                }
                if ((btnAndStick & L_JPAD) && MenuScreenA > SUB_MENU_COPY_PAK_PROMPT_MIN) {
                    MenuScreenA -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp38->Workf1 < 4.2) {
                        sp38->Workf1 += 4.0;
                    }
                    sp38->Pattern2 = -1;
                }
                if (btnAndStick & B_BUTTON) {
                    MenuScreenA = sp38->Work1 + SUB_MENU_COPY_PAK_TO_GHOST_MIN;
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
                if (btnAndStick & A_BUTTON) {
                    if (MenuScreenA == SUB_MENU_COPY_PAK_PROMPT_QUIT) {
                        MenuScreenA = SUB_MENU_OPTION_COPY_CONTROLLER_PAK;
                        PlaySound(SOUND_MENU_GO_BACK);
                    } else {
                        MenuScreenA = SUB_MENU_COPY_PAK_START;
                        PlaySound(SOUND_MENU_SELECT);
                        sp38->Pattern = 0;
                    }
                }
                // return?
                break;
            }
            case SUB_MENU_COPY_PAK_START: {
                if (controllerIdx == PLAYER_ONE) {
                    sp38->Pattern += 1;
                }
                if (sp38->Pattern >= 3) {
                    MenuScreenA = SUB_MENU_COPY_PAK_COPYING;
                }
                break;
            }
            case SUB_MENU_COPY_PAK_COPYING: {
                res = CheckPakStatus2();
                if (res == PFS_NO_ERROR) {
                    res = LoadGameNoteKartGhost2(sp38->Work2, sp38->Work1);
                }
                if (res != 0) {
                    MenuScreenA = SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P;
                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                res = osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName,
                                    (u8*) ExtCode, &ControllerPak1FileNote);
                if (res == PFS_NO_ERROR) {
                    res = SaveGameNoteKartGhost(sp38->Work1);
                }
                if (res != 0) {
                    MenuScreenA = SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P;
                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                MenuScreenA = SUB_MENU_COPY_PAK_COMPLETED;
                D_8018EE10[sp38->Work1].CourseIndex = (sp30 + sp38->Work2)->CourseIndex;
                SaveGameNoteKartData(sp38->Work1);
                break;
            }
            case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT: {
                if (controllerIdx == PLAYER_ONE) {
                    sp38->Pattern += 1;
                }
                if (sp38->Pattern >= 3) {
                    MenuScreenA = SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE;
                }
                break;
            }
            case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE: {
                if (MakeGameNoteKartOnly()) {
                    MenuScreenA = SUB_MENU_COPY_PAK_ERROR_CANT_CREATE_1P;
                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                } else if (sp30[0].GhostDataSaved) {
                    MenuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST1_1P;
                } else {
                    MenuScreenA = SUB_MENU_COPY_PAK_FROM_GHOST2_1P;
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
void TaselController(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    // Make pressing Start have the same effect as pressing A
    if (!DebugMode && ((btnAndStick & START_BUTTON) != 0)) {
        btnAndStick |= A_BUTTON;
    }

    if (FadeChecker() == 0) {
        if (MenuScreenA == SUB_MENU_DATA) {
            // If DPad/Stick down pressed, move selection down if not already in bottom row
            if ((btnAndStick & D_JPAD) != 0) {
                if ((TimeTrialDataCourseIndex % 4) != 3) {
                    ++TimeTrialDataCourseIndex;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick up pressed, move selection up if not already in top row
            if ((btnAndStick & U_JPAD) != 0) {
                if ((TimeTrialDataCourseIndex % 4) != 0) {
                    --TimeTrialDataCourseIndex;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick Right pressed, move selection Right if not already in Right-most column
            if ((btnAndStick & R_JPAD) != 0) {
                if ((TimeTrialDataCourseIndex / 4) != 3) {
                    TimeTrialDataCourseIndex += 4;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If DPad/Stick Left pressed, move selection Left if not already in Left-most column
            if ((btnAndStick & L_JPAD) != 0) {
                if ((TimeTrialDataCourseIndex / 4) != 0) {
                    TimeTrialDataCourseIndex -= 4;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
            }
            // If B pressed, go to Main menu
            if ((btnAndStick & B_BUTTON) != 0) {
                SetFadeOutTaData();
                PlaySound(SOUND_MENU_GO_BACK);
                return;
            }
            // If A pressed, go to selected course's records
            if ((btnAndStick & A_BUTTON) != 0) {
                CourseRecordsMenuSelection = COURSE_RECORDS_MENU_RETURN_MENU;
                SetFadeOutF();
                PlaySound(SOUND_MENU_OK_CLICKED);
            }
        }
        // If MenuScreenA is not SUB_MENU_DATA and A pressed, go to Main menu
        // This condition is not reachable but this failsafe was added nonetheless
        else if ((btnAndStick & A_BUTTON) != 0) {
            SetFadeOutTaData();
            PlaySound(SOUND_MENU_OK_CLICKED);
        }
    }
}

/**
 * Navigation of the course records data menu
 */
void DselController(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick; // sp2E
    ObjBlock* sp28;
    CourseTimeTrialRecords* sp24;
    s32 res;

    btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    if (!DebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        switch (MenuScreenA) {
            case SUB_MENU_DATA_OPTIONS: {
                if ((btnAndStick & L_JPAD) && (TimeTrialDataCourseIndex > 0)) {
                    TimeTrialDataCourseIndex -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }

                if ((btnAndStick & R_JPAD) && (TimeTrialDataCourseIndex < 15)) {
                    TimeTrialDataCourseIndex += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }

                sp28 = GetOBJBlockPtr(0xE8);
                sp24 = &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[TimeTrialDataCourseIndex / 4]
                            .CourseRecords[TimeTrialDataCourseIndex % 4];
                if (CourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_GHOST &&
                    CheckSameMap2(TimeTrialDataCourseIndex) < 0) {
                    CourseRecordsMenuSelection -= 1;
                }

                if (CourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_RECORDS && sp24->UnknownBytes[0] == 0) {
                    CourseRecordsMenuSelection -= 1;
                }

                if ((btnAndStick & U_JPAD) && (CourseRecordsMenuSelection > COURSE_RECORDS_MENU_MIN)) {
                    CourseRecordsMenuSelection -= 1;
                    if (CourseRecordsMenuSelection == 1 && sp24->UnknownBytes[0] == 0) {
                        CourseRecordsMenuSelection -= 1;
                    }
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->Workf1 < 4.2) {
                        sp28->Workf1 += 4.0;
                    }
                    sp28->Pattern2 = -1;
                }

                if ((btnAndStick & D_JPAD) && (CourseRecordsMenuSelection < COURSE_RECORDS_MENU_MAX)) {
                    CourseRecordsMenuSelection += 1;
                    if (CourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_RECORDS &&
                        sp24->UnknownBytes[0] == 0) {
                        CourseRecordsMenuSelection += 1;
                    }

                    if (CourseRecordsMenuSelection == COURSE_RECORDS_MENU_ERASE_GHOST &&
                        CheckSameMap2(TimeTrialDataCourseIndex) < 0) {
                        if (sp24->UnknownBytes[0] == 0) {
                            CourseRecordsMenuSelection = COURSE_RECORDS_MENU_RETURN_MENU;
                        } else {
                            CourseRecordsMenuSelection = COURSE_RECORDS_MENU_ERASE_RECORDS;
                        }
                    } else {
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (sp28->Workf1 < 4.2) {
                            sp28->Workf1 += 4.0;
                        }
                        sp28->Pattern2 = 1;
                    }
                }

                if (btnAndStick & B_BUTTON) {
                    SetFadeOutB();
                    PlaySound(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (sp28->Workf1 < 4.2) {
                        sp28->Workf1 += 4.0;
                    }
                    if (CourseRecordsMenuSelection == COURSE_RECORDS_MENU_RETURN_MENU) {
                        SetFadeOutB();
                        PlaySound(SOUND_MENU_GO_BACK);
                    } else {
                        MenuScreenA = SUB_MENU_DATA_ERASE_CONFIRM;
                        CourseRecordsSubMenuSelection = COURSE_RECORDS_SUB_MENU_QUIT;
                        PlaySound(SOUND_MENU_SELECT);
                    }
                }
                break;
            }
            case SUB_MENU_DATA_ERASE_CONFIRM: {
                sp28 = GetOBJBlockPtr(0xE9);
                if ((btnAndStick & U_JPAD) && (CourseRecordsSubMenuSelection > COURSE_RECORDS_SUB_MENU_MIN)) {
                    CourseRecordsSubMenuSelection -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->Workf1 < 4.2) {
                        sp28->Workf1 += 4.0;
                    }
                    sp28->Pattern2 = -1;
                }

                if ((btnAndStick & D_JPAD) && (CourseRecordsSubMenuSelection < COURSE_RECORDS_SUB_MENU_MAX)) {
                    CourseRecordsSubMenuSelection += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (sp28->Workf1 < 4.2) {
                        sp28->Workf1 += 4.0;
                    }
                    sp28->Pattern2 = 1;
                }

                if (btnAndStick & B_BUTTON) {
                    MenuScreenA = SUB_MENU_DATA_OPTIONS;
                    PlaySound(SOUND_MENU_GO_BACK);
                } else if (btnAndStick & A_BUTTON) {
                    if (CourseRecordsSubMenuSelection != COURSE_RECORDS_SUB_MENU_QUIT) {
                        res = 0;
                        switch (CourseRecordsMenuSelection) {
                            case COURSE_RECORDS_MENU_ERASE_RECORDS: {
                                InitEEPRomCourseData(TimeTrialDataCourseIndex);
                                SaveFunc800B559CDefault(TimeTrialDataCourseIndex);
                                PlaySound(SOUND_MENU_EXPLOSION);
                                res = -1;
                                break;
                            }
                            case COURSE_RECORDS_MENU_ERASE_GHOST: {
                                res = CheckSameMap2(TimeTrialDataCourseIndex);
                                if (res >= 0) {
                                    if (InitGameNoteKart(res) != 0) {
                                        MenuScreenA = SUB_MENU_DATA_CANT_ERASE;
                                        PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                    } else {
                                        PlaySound(SOUND_MENU_EXPLOSION);
                                        MenuScreenA = SUB_MENU_DATA_OPTIONS;
                                    }
                                }
                                break;
                            }
                        }

                        if (!(res + 1)) {
                            MenuScreenA = SUB_MENU_DATA_OPTIONS;
                        }
                    } else {
                        PlaySound(SOUND_MENU_GO_BACK);
                        MenuScreenA = SUB_MENU_DATA_OPTIONS;
                    }
                }
                break;
            }
            case SUB_MENU_DATA_CANT_ERASE: {
                if (btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                    MenuScreenA = SUB_MENU_DATA_OPTIONS;
                }
                break;
            }
        }
    }
}

/**
 * On Input skip logo screen
 **/
void NintenLogoControler(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);

    // If any Button is pressed then fade audio out
    if ((FadeChecker() == 0) && (btnAndStick)) {
        // TODO: Label audio functions
        NaSEFadeout(0x3C);

        SetFadeOutFw();
    }
}

/**
 * Navigation of the controller pak Table data
 */
void CpControler(struct Controller* controller, UNUSED u16 controllerIdx) {
    u16 btnAndStick;
    OSPfsState* osPfsState;
    s32 selectedTableRow;
    UNUSED s8 pad;

    btnAndStick = controller->ButtonPressed | controller->AnalogPressed;
    if (FadeChecker() == 0) {
        switch (ControllerPakMenuSelection) {
            case CONTROLLER_PAK_MENU_SELECT_RECORD:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    PlaySound(SOUND_MENU_SELECT);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_END;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_END:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    PlaySound(SOUND_MENU_SELECT);
                    SetFadeOutF();
                    ControllerPak1State = BAD;
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_TABLE_GAME_DATA:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    selectedTableRow = ControllerPakVisibleTableRows[ControllerPakSelectedTableRow + 2] - 1;
                    if (PfsError[selectedTableRow] == 0) {
                        ControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                        PlaySound(SOUND_MENU_SELECT);
                        return;
                    }
                } else if ((btnAndStick & B_BUTTON) != 0) {
                    if (ControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE) {
                        ControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
                        PlaySound(SOUND_MENU_GO_BACK);
                        return;
                    }
                } else if ((btnAndStick & U_JPAD) != 0) {
                    if (ControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE) {
                        --ControllerPakSelectedTableRow;
                        if (ControllerPakSelectedTableRow < 0) {
                            ControllerPakSelectedTableRow = 0;
                            if (ControllerPakVisibleTableRows[ControllerPakSelectedTableRow + 2] != 1) {
                                ControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_UP;
                                PlaySound(SOUND_MENU_CURSOR_MOVE);
                                return;
                            }
                        } else {
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                            return;
                        }
                    }
                } else if (((btnAndStick & D_JPAD) != 0) &&
                           (ControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE)) {
                    ++ControllerPakSelectedTableRow;
                    if (ControllerPakSelectedTableRow >= CONTROLLER_PAK_MENU_TABLE_GAME_DATA) {
                        ControllerPakSelectedTableRow = CONTROLLER_PAK_MENU_QUIT;
                        if (ControllerPakVisibleTableRows[ControllerPakSelectedTableRow + 2] != 16) {
                            ControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_DOWN;
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                            return;
                        }
                    } else {
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        return;
                    }
                }
                break;
            case CONTROLLER_PAK_MENU_QUIT:
                if ((btnAndStick & (A_BUTTON | B_BUTTON | START_BUTTON)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_GO_TO_ERASING;
                    PlaySound(SOUND_MENU_SELECT);
                    return;
                }
                if ((btnAndStick & B_BUTTON) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & (L_JPAD | R_JPAD)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_QUIT;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    return;
                }
                break;
            case CONTROLLER_PAK_MENU_GO_TO_ERASING:
                ControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASING;
                return;
            case CONTROLLER_PAK_MENU_ERASING:
                selectedTableRow = ControllerPakVisibleTableRows[ControllerPakSelectedTableRow + 2] - 1;
                osPfsState = &PfsState[selectedTableRow];

                switch (osPfsDeleteFile(&ControllerPak1FileHandle, osPfsState->company_code, osPfsState->game_code,
                                        (u8*) &osPfsState->game_name, (u8*) &osPfsState->ext_name)) {
                    default:
                        ControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED;
                        return;
                    case 0:
                        PfsError[selectedTableRow] = -1;
                        ControllerPak1NumPagesFree += (((osPfsState->file_size + 0xFF) >> 8) & 0xFF);
                        ControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                        return;
                    case PFS_ERR_NOPACK:
                        ControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK;
                        return;
                    case PFS_ERR_NEW_PACK: /* switch 1 */
                        ControllerPakMenuSelection = CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED;
                        return;
                }
                break;
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK:
            case CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED:
                if ((btnAndStick & (A_BUTTON | START_BUTTON)) != 0) {
                    ControllerPakMenuSelection = CONTROLLER_PAK_MENU_TABLE_GAME_DATA;
                }
                break;
        }
    }
}

/**
 * Navigation of the Main splash Start screen menu
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
            TitleCounter2 += 1;
        }
        switch (TitleMode) {
            case DEBUG_MENU_DISABLED: {
                isDebug = false;
                if ((TitleCounter2 >= 46) && (btnAndStick & (A_BUTTON | START_BUTTON))) {
                    SetFadeOutF();
                    NaSeqFadeout(0x19);
                    PlaySound(SOUND_INTRO_ENTER_MENU);
                } else {
                    break;
                }
                break;
            }
            case DEBUG_MENU_DEBUG_MODE: {
                if (btnAndStick & (R_JPAD | L_JPAD)) {
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (DebugMode) {
                        DebugMode = DEBUG_MODE_TOGGLE;
                    } else {
                        DebugMode = true;
                    }
                }
                if (btnAndStick & D_JPAD) {
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    TitleMode = DEBUG_MENU_COURSE;
                }
                break;
            }
            case DEBUG_MENU_COURSE: {
                if (btnAndStick & R_JPAD) {
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (CourseID < (NUM_COURSES - 2)) {
                        CourseID += 1;
                    } else {
                        CourseID = 0;
                    }
                }
                if (btnAndStick & L_JPAD) {
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (CourseID > 0) {
                        CourseID -= 1;
                    } else {
                        CourseID = (NUM_COURSES - 2);
                    }
                }
                if (btnAndStick & U_JPAD) {
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    TitleMode = DEBUG_MENU_DEBUG_MODE;
                }
                if (btnAndStick & D_JPAD) {
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    TitleMode = DEBUG_MENU_SCREEN_MODE;
                }
                break;
            }
            case DEBUG_MENU_SCREEN_MODE: {
                if ((btnAndStick & R_JPAD) && (ScreenModeListIndex < 4)) {
                    ScreenModeListIndex += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    ScreenSplitB = SScreenModePlayerTable[ScreenModeListIndex];
                }
                if ((btnAndStick & L_JPAD) && (ScreenModeListIndex > 0)) {
                    ScreenModeListIndex -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    ScreenSplitB = SScreenModePlayerTable[ScreenModeListIndex];
                }
                if (btnAndStick & U_JPAD) {
                    TitleMode = DEBUG_MENU_COURSE;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    TitleMode = DEBUG_MENU_PLAYER;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_PLAYER: {
                if ((btnAndStick & R_JPAD) && (CharacterSelections[0] < 7)) {
                    CharacterSelections[0] += 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && (CharacterSelections[0] > 0)) {
                    CharacterSelections[0] -= 1;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & U_JPAD) {
                    TitleMode = DEBUG_MENU_SCREEN_MODE;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    TitleMode = DEBUG_MENU_SOUND_MODE;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_SOUND_MODE: {
                if ((btnAndStick & R_JPAD) && (SoundOutput < 3)) {
                    SoundOutput += 1;
                    if (SoundOutput == SOUND_UNUSED) {
                        SoundOutput = SOUND_MONO;
                    }
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    SettingSoundOutMode();
                    EEPRomSaveData.Main.SaveInfo.SoundMode = SoundOutput;
                    SaveFunc800B4670Default();
                    SaveFunc800B5948Default();
                }
                if ((btnAndStick & L_JPAD) && (SoundOutput > 0)) {
                    SoundOutput -= 1;
                    if (SoundOutput == SOUND_UNUSED) {
                        SoundOutput = SOUND_HEADPHONES;
                    }
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    SettingSoundOutMode();
                    EEPRomSaveData.Main.SaveInfo.SoundMode = SoundOutput;
                    SaveFunc800B4670Default();
                }
                if (btnAndStick & U_JPAD) {
                    TitleMode = DEBUG_MENU_PLAYER;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    TitleMode = DEBUG_MENU_GIVE_ALL_GOLD_CUP;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            }
            case DEBUG_MENU_GIVE_ALL_GOLD_CUP: {
                if (btnAndStick & U_JPAD) {
                    TitleMode = DEBUG_MENU_SOUND_MODE;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & B_BUTTON) {
                    for (i = 0; i < 16; i++) {
                        SaveCupStatus2(0, i);
                    }
                    PlaySound(SOUND_MENU_SELECT);
                    break;
                } else if (btnAndStick & L_TRIG) {
                    InitEEPRomFlgs1Data();
                    for (i = 0; i < 16; i++) {
                        SaveCupStatus2(i / 4, i);
                    }
                    PlaySound(SOUND_MENU_SELECT);
                    break;
                } else if (btnAndStick & L_JPAD) {
                    InitEEPRomFlgs1Data();
                    for (i = 0; i < 16; i++) {
                        if (i % 4 == 2) {
                            SaveCupStatus2(0, i);
                        } else {
                            SaveCupStatus2(i / 4, i);
                        }
                    }
                    PlaySound(SOUND_MENU_SELECT);
                } else {
                    break;
                }
                break;
            }
            default:
                break;
        }

        PlayerCount = MenuMultiplayerSelection = SScreenModePlayerCount[ScreenModeListIndex];

        if (isDebug) {
            if (btnAndStick & (A_BUTTON | START_BUTTON)) {
                SetFadeOutF();
                NaSeqFadeout(0x19);
                TitleMode = DEBUG_MENU_OPTION_SELECTED;

                if (controller->ButtonHeld & L_TRIG) {
                    DemoFlag = DEMO_MODE_ACTIVE;
                } else {
                    DemoFlag = DEMO_MODE_INACTIVE;
                }

                if (controller->ButtonHeld & Z_TRIG) {
                    if (btnAndStick & A_BUTTON) {
                        DebugGotoScene = DEBUG_GOTO_ENDING;
                    } else {
                        DebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA;
                    }
                }
                PlaySound(SOUND_MENU_OK_CLICKED);
            } else if ((btnAndStick & B_BUTTON) && (controller->ButtonHeld & Z_TRIG)) {
                SetFadeOutF();
                NaSeqFadeout(0x19);
                TitleMode = DEBUG_MENU_OPTION_SELECTED;
                DebugGotoScene = DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT;
                PlaySound(SOUND_MENU_OK_CLICKED);
            } else if (btnAndStick & R_TRIG) {
                TitleMode = DEBUG_MENU_DISABLED;
                PlaySound(SOUND_MENU_SELECT);
            }
        }
    }
}

void GselPopupData(void) {
    // For Grand Prix and Versus, this will be the CC Mode selected. For Time Trials, it will
    // be whether 'Begin' or 'Data' is selected. Not Used for Battle.
    s8 subMenuMode = GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]];
    // Determine which game Mode was selected based on the Number of players and the row selected on the Main menu
    switch (GameModePlayerSelection[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]) {
        case GRAND_PRIX:
            RaceClass = subMenuMode;
            ItemSetFlag = 1;
            ScreenFlip = (subMenuMode == CC_EXTRA) ? 1 : 0;
            break;
        case VERSUS:
            RaceClass = subMenuMode;
            ItemSetFlag = 1;
            ScreenFlip = (subMenuMode == CC_EXTRA) ? 1 : 0;
            break;
        case BATTLE:
            ItemSetFlag = 1;
            ScreenFlip = 0;
            break;
        case TIME_TRIALS:
            RaceClass = CC_100;
            ScreenFlip = 0;
            ItemSetFlag = 0;

            if ((subMenuMode && subMenuMode) && subMenuMode) {}

            break;
    }
}

/**
 * Navigation of the Main game Mode Select screen
 */
void GSelController(struct Controller* controller, u16 controllerIdx) {
    u16 btnAndStick;
    s32 subMode; // subMode
    bool cursorMoved; // cursorMoved

    btnAndStick = controller->ButtonPressed | controller->AnalogPressed;
    if ((DebugMode == 0) && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (FadeChecker() == 0) {
        switch (MenuScreenB) {
            case MAIN_MENU_NONE:
                break;
            case MAIN_MENU_PLAYER_SELECT:
                if ((btnAndStick & R_JPAD) && (MenuMultiplayerSelection < 4)) {
                    MenuMultiplayerSelection += 1;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if ((btnAndStick & L_JPAD) && (MenuMultiplayerSelection >= 2)) {
                    MenuMultiplayerSelection -= 1;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                PlayerCount = MenuMultiplayerSelection;
                switch (PlayerCount) {
                    case 1:
                        ScreenSplitB = SCREEN_MODE_1P;
                        break;
                    case 2:
                        ScreenSplitB = SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL;
                        break;
                    case 3:
                    case 4:
                        ScreenSplitB = SCREEN_MODE_3P_4P_SPLITSCREEN;
                        break;
                }
                if (btnAndStick & B_BUTTON) {
                    SetFadeOutM(0x14);
                    NaSeqFadeout(0x19);
                    KBGNumberNext = MENU_FADE_TYPE_BACK;
                    PlaySound(SOUND_MENU_GO_BACK);
                    break;
                }
                if (btnAndStick & A_BUTTON) {
                    MenuScreenB = MAIN_MENU_MODE_SELECT;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_SELECT);
                    break;
                }
                break;
            case MAIN_MENU_MODE_SELECT:
                if (btnAndStick & D_JPAD) {
                    if (GameModeMenuColumn[MenuMultiplayerSelection - 1] < PlayerModeSelection[MenuMultiplayerSelection - 1]) {
                        GameModeMenuColumn[MenuMultiplayerSelection - 1] += 1;
                        FlashCounterReset();
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                if (btnAndStick & U_JPAD) {
                    if (GameModeMenuColumn[MenuMultiplayerSelection - 1] > 0) {
                        GameModeMenuColumn[MenuMultiplayerSelection - 1] -= 1;
                        FlashCounterReset();
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    MenuScreenB = MAIN_MENU_PLAYER_SELECT;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_GO_BACK);
                    break;
                }
                
                if (btnAndStick & A_BUTTON) {
                    switch (GameModePlayerSelection[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]) {
                        default:
                            MenuScreenB = MAIN_MENU_OK_SELECT;
                            break;
                        case 0:
                            MenuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            PlaySound(SOUND_MENU_GP);
                            break;
                        case 2:
                            MenuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            PlaySound(SOUND_MENU_VERSUS);
                            break;
                        case 1:
                            MenuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            PlaySound(SOUND_MENU_TIME_TRIALS);
                            break;
                        case 3:
                            MenuScreenB = MAIN_MENU_OK_SELECT;
                            PlaySound(SOUND_MENU_BATTLE);
                            break;
                    }
                    FlashCounterReset();
                    TitleDemo = 0;
                    break;
                }
                break;
            case MAIN_MENU_MODE_SUB_SELECT:
            case MAIN_MENU_MODE_SUB_SELECT_GO_BACK:
                if (controllerIdx == PLAYER_ONE) {
                    TitleDemo++;
                    if ((TitleDemo == 100) || !(TitleDemo % 300)) {
                        switch (GameModePlayerSelection[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]) {
                            case 0:
                            case 2:
                                PlaySound(SOUND_MENU_SELECT_LEVEL);
                                break;
                            default:
                                break;
                        }
                    }
                }

                subMode = GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]];
                if ((btnAndStick & U_JPAD) && (subMode > 0)) {
                    GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]] -= 1;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if (btnAndStick & D_JPAD) {
                    cursorMoved = false;
                    if (Check4GoldCup150CC()) {
                        if (subMode < SGameModePlayerColumnExtra[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]) {
                            cursorMoved = true;
                        }
                    } else {
                        if (subMode < SGameModePlayerColumnDefault[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]) {
                            cursorMoved = true;
                        }
                    }
                    if (cursorMoved) {
                        GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]++;
                        FlashCounterReset();
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                subMode = GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]];

                if (btnAndStick & B_BUTTON) {
                    MenuScreenB = MAIN_MENU_MODE_SELECT;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_GO_BACK);
                    break;
                } 
                if (btnAndStick & A_BUTTON) {
                    FlashCounterReset();
                    if ((MenuMultiplayerSelection == 1) && ((GameModeMenuColumn - 1)[MenuMultiplayerSelection] == 1) && (subMode == 1)) {
                        SetFadeOutTaData();
                        PlaySound(SOUND_MENU_DATA);
                    } else {
                        MenuScreenB = MAIN_MENU_OK_SELECT;
                        PlaySound(SOUND_MENU_SELECT);
                        TitleDemo = 0;
                    }
                    break;
                }
                break;
            case MAIN_MENU_OK_SELECT:
            case MAIN_MENU_OK_SELECT_GO_BACK:
                if (controllerIdx == PLAYER_ONE) {
                    TitleDemo++;
                    if ((TitleDemo == 60) || !(TitleDemo % 300)) {
                        PlaySound(SOUND_MENU_OK);
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    switch (GameModePlayerSelection[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]) {
                        case 0:
                        case 1:
                        case 2:
                            MenuScreenB = MAIN_MENU_MODE_SUB_SELECT;
                            break;
                        default:
                        case 3:
                            MenuScreenB = MAIN_MENU_MODE_SELECT;
                            break;
                    }
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_GO_BACK);
                    TitleDemo = 0;
                    break;
                }
                if (btnAndStick & A_BUTTON) {
                    SetFadeOutF();
                    PlaySound(SOUND_MENU_OK_CLICKED);
                    GselPopupData();
                    break;
                }
                break;
            case MAIN_MENU_OPTION:
            case MAIN_MENU_DATA:
                break;
            default:
                break;
        }
        GameMode = GameModePlayerSelection[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]];
    }
}

/**
 * Check if there is no currently selected and/or
 * hovered character at grid Position `gridId`
 */
bool PselCheck(s32 gridId) {
    s32 i;
    for (i = 0; i < ARRAY_COUNT(Player1Character); i++) {
        if (gridId == Player1Character[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Navigation of the Player Select screen
 * Grid positions are from Right to Left, then top to bottom
 */
void PSelController(struct Controller* controller, u16 controllerIdx) {
    s8 i;
    s8 j;
    s32 selected;
    u16 btnAndStick;

    btnAndStick = (controller->ButtonPressed | controller->AnalogPressed);
    if (!DebugMode && (btnAndStick & START_BUTTON)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        switch (MenuScreenC) {
            case PLAYER_SELECT_MENU_MAIN:
                if (Player1Character[controllerIdx] == 0) {
                    if (btnAndStick & B_BUTTON) {
                        SetFadeOutB();
                        PlaySound(SOUND_MENU_GO_BACK);
                    }
                    return;
                }

                if (btnAndStick & B_BUTTON) {
                    if (CharacterGridIsSelected[controllerIdx] != false) {
                        CharacterGridIsSelected[controllerIdx] = false;
                        PlaySound(SOUND_MENU_GO_BACK);
                    } else {
                        SetFadeOutB();
                        PlaySound(SOUND_MENU_GO_BACK);
                    }
                }
            
                if ((btnAndStick & A_BUTTON) && (CharacterGridIsSelected[controllerIdx] == 0)) {
                    CharacterGridIsSelected[controllerIdx] = true;
                    NAPlyVoiceStart(controllerIdx, ((SCharacterGridOrder - 1)[Player1Character[controllerIdx]] * 0x10) + 0x2900800E);
                }

                selected = false;
                for (i = 0; i < ARRAY_COUNT(Player1Character); i++) {
                    if ((Player1Character[i] != 0) && (CharacterGridIsSelected[i] == false)) {
                        selected = true;
                        break;
                    }
                }

                if (!selected) {
                    MenuScreenC = PLAYER_SELECT_MENU_OK;
                    FlashCounterReset();
                    TitleDemo = 0;
                }

                if (CharacterGridIsSelected[controllerIdx] != false) {
                    break;
                }
                j = Player1Character[controllerIdx];
                if ((btnAndStick & R_JPAD) && (btnAndStick & D_JPAD)) {
                    if ((Player1Character[controllerIdx] == 1U) || (Player1Character[controllerIdx] == 2U) || (Player1Character[controllerIdx] == 3U)) {
                        j = Player1Character[controllerIdx] + 5;
                        if (PselCheck(j)) {
                            Player1Character[controllerIdx] = j;
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                }
                if ((btnAndStick & L_JPAD) && (btnAndStick & D_JPAD)) {
                    if ((Player1Character[controllerIdx] == 2U) || (Player1Character[controllerIdx] == 3U) || (Player1Character[controllerIdx] == 4U)) {
                        j = Player1Character[controllerIdx] + 3;
                        if (PselCheck(j)) {
                            Player1Character[controllerIdx] = j;
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                }
                if ((btnAndStick & R_JPAD) && (btnAndStick & U_JPAD)) {
                    if ((Player1Character[controllerIdx] == 5U) || (Player1Character[controllerIdx] == 6U) || (Player1Character[controllerIdx] == 7U)) {
                        j = Player1Character[controllerIdx] - 3;
                        if (PselCheck(j)) {
                            Player1Character[controllerIdx] = j;
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                }

                if ((btnAndStick & L_JPAD) && (btnAndStick & U_JPAD)) {
                    if ((Player1Character[controllerIdx] == 6U) || (Player1Character[controllerIdx] == 7U) || (Player1Character[controllerIdx] == 8U)) {
                        j = Player1Character[controllerIdx] - 5;
                        if (PselCheck(j)) {
                            Player1Character[controllerIdx] = j;
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                        }
                    }
                    return;
                    
                }
                if (btnAndStick & R_JPAD) {
                    if ((Player1Character[controllerIdx] != 4U) && (Player1Character[controllerIdx] != 8U)) {
                        j = Player1Character[controllerIdx] + 1;
                        do {
                            if (PselCheck(j)) {
                                Player1Character[controllerIdx] = j;
                                PlaySound(SOUND_MENU_CURSOR_MOVE);
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
                    if ((Player1Character[controllerIdx] != 1U) && (Player1Character[controllerIdx] != 5U)) {
                        j = Player1Character[controllerIdx] - 1;
                        do {
                            if (PselCheck(j)) {
                                Player1Character[controllerIdx] = j;
                                PlaySound(SOUND_MENU_CURSOR_MOVE);
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

                if ((btnAndStick & U_JPAD) && (Player1Character[controllerIdx] >= 5)) {
                    j = Player1Character[controllerIdx] - 4;
                }
                if ((btnAndStick & D_JPAD) && (Player1Character[controllerIdx] < 5)) {
                    j = Player1Character[controllerIdx] + 4;
                }
                if (PselCheck(j)) {
                    Player1Character[controllerIdx] = j;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                break;
            case PLAYER_SELECT_MENU_OK:
            case PLAYER_SELECT_MENU_OK_GO_BACK:
                if (controllerIdx == 0) {
                    TitleDemo++;
                    if (TitleDemo == 0x3C || !(TitleDemo % 300)) {
                        PlaySound(SOUND_MENU_OK);
                    }
                }
                if (btnAndStick & B_BUTTON) {
                    MenuScreenC = PLAYER_SELECT_MENU_MAIN;
                    CharacterGridIsSelected[controllerIdx] = false;
                    PlaySound(SOUND_MENU_GO_BACK);
                    break;
                }
                if (btnAndStick & A_BUTTON) {
                    SetFadeOutF();
                    PlaySound(SOUND_MENU_OK_CLICKED);
                    SetRivalCarName();
                }
                break;
            default:
                break;
        }

        if (Player1Character[controllerIdx] != 0) {
            CharacterSelections[controllerIdx] = (SCharacterGridOrder - 1)[Player1Character[controllerIdx]];
        }
    }
}

/**
 * Navigation of the Map Select course menu screen
 */
void MSelController(struct Controller* arg0, u16 controllerIdx) {
    u16 btnAndStick = (arg0->ButtonPressed | arg0->AnalogPressed);

    if ((!DebugMode) && ((btnAndStick & START_BUTTON) != 0)) {
        btnAndStick |= A_BUTTON;
    }

    if (!FadeChecker()) {
        switch (MenuScreenA) {
            case SUB_MENU_MAP_SELECT_CUP:
                if ((btnAndStick & R_JPAD) != 0) {
                    if (CupSelect < SPECIAL_CUP) {
                        BeforeCupSelect = CupSelect;
                        ++CupSelect;
                        FlashCounterReset();
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                    }
                }
                if (((btnAndStick & L_JPAD) != 0) && (CupSelect > MUSHROOM_CUP)) {
                    BeforeCupSelect = CupSelect;
                    --CupSelect;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }

                CupLevel = CupSelect;
                CourseID = CupCourseOrder[CupSelect][CourseSelect];
                if ((btnAndStick & B_BUTTON) != 0) {
                    SetFadeOutB();
                    PlaySound(SOUND_MENU_GO_BACK);
                } else if ((btnAndStick & A_BUTTON) != 0) {
                    if (GameMode != GRAND_PRIX) {
                        MenuScreenA = SUB_MENU_MAP_SELECT_COURSE;
                        PlaySound(SOUND_MENU_SELECT);
                    } else {
                        MenuScreenA = SUB_MENU_MAP_SELECT_OK;
                        PlaySound(SOUND_MENU_SELECT);
                        CourseID = CupCourseOrder[CupSelect][COURSE_ONE];
                        TitleDemo = 0;
                    }
                    FlashCounterReset();
                }
                break;
            case SUB_MENU_MAP_SELECT_COURSE:
            case SUB_MENU_MAP_SELECT_BATTLE_COURSE:
                if (((btnAndStick & D_JPAD) != 0) && (CourseSelect < COURSE_FOUR)) {
                    ++CourseSelect;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }
                if (((btnAndStick & U_JPAD) != 0) && (CourseSelect > COURSE_ONE)) {
                    --CourseSelect;
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                }

                CourseID = CupCourseOrder[CupSelect][CourseSelect];
                if ((btnAndStick & B_BUTTON) != 0) {
                    if (MenuScreenA == SUB_MENU_MAP_SELECT_COURSE) {
                        MenuScreenA = SUB_MENU_MAP_SELECT_CUP;
                    } else {
                        SetFadeOutB();
                    }
                    FlashCounterReset();
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & A_BUTTON) != 0) {
                    MenuScreenA = SUB_MENU_MAP_SELECT_OK;
                    PlaySound(SOUND_MENU_SELECT);
                    FlashCounterReset();
                    TitleDemo = 0;
                }
                break;
            case SUB_MENU_MAP_SELECT_OK:
                if ((controllerIdx == PLAYER_ONE) &&
                    ((++TitleDemo == 0x3C) || ((TitleDemo % 300) == 0))) {
                    PlaySound(SOUND_MENU_OK);
                }

                if ((btnAndStick & B_BUTTON) != 0) {
                    switch (GameMode) {
                        case GRAND_PRIX:
                            MenuScreenA = SUB_MENU_MAP_SELECT_CUP;
                            break;
                        case BATTLE:
                            MenuScreenA = SUB_MENU_MAP_SELECT_BATTLE_COURSE;
                            break;
                        default:
                            MenuScreenA = SUB_MENU_MAP_SELECT_COURSE;
                            break;
                    }

                    FlashCounterReset();
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
                if ((btnAndStick & A_BUTTON) != 0) {
                    SetFadeOutF();
                    NaSeqFadeout(0x19);
                    PlaySound(SOUND_MENU_OK_CLICKED);
                }
                break;
        }
    }
}

/**
 * Loads menu states so they are preserved between menu Changes
 */
void KVarInit(s32 menuSelection) {
    s32 i;

    TitleMode = DEBUG_MENU_SELECTION;
    TitleDemo = 0;
    TitleCounter2 = 0;
    DemoPlayFlag = 0;
    ReplayFlag = 0;
    RetryFlag = 0;
    DebugGotoScene = DEBUG_GOTO_RACING;
    InGameTT = 0;
    D_8016556E = 0;
    GhostStatus = 1;
    ReplayghostStatus = 1;
    D_80162E00 = 0;
    ReadPointer = 1;
    WritePointer = 0;

    switch (menuSelection) {
        case OPTIONS_MENU:
            MenuScreenA = SUB_MENU_OPTION_RETURN_GAME_SELECT;
            break;
        case DATA_MENU:
            MenuScreenA = SUB_MENU_DATA;
            break;
        case COURSE_DATA_MENU:
            MenuScreenA = SUB_MENU_DATA_OPTIONS;
            break;
        case LOGO_INTRO_MENU:
            NaChangeSoundMode(0, 0);
            break;
        case CONTROLLER_PAK_MENU: {
            ControllerPakMenuSelection = CONTROLLER_PAK_MENU_SELECT_RECORD;
            NaChangeSoundMode(0, 0);
            break;
        }
        case 0:
        case START_MENU: {
            ScreenFlip = 0;
            DebugMode = DEBUG_MODE_TOGGLE;
            CupSelect = MUSHROOM_CUP;
            CourseSelect = 0;
            TimeTrialDataCourseIndex = 0;
            if (MenuMultiplayerSelection <= 0) {
                MenuMultiplayerSelection = 1;
            }
            if (MenuMultiplayerSelection >= 5) {
                MenuMultiplayerSelection = 4;
            }
            ScreenModeListIndex = SScreenModeIdxFromPlayerMode[MenuMultiplayerSelection - 1];
            NaChangeSoundMode(0, 0);
            NaSeqStart(SEQ_MENU_TITLE_SCREEN);
            InGame = 0;
            break;
        }
        case 1:
        case MAIN_MENU: {
            DebugMode = DEBUG_MODE_TOGGLE;
            ScreenFlip = 0;
            InGame = 0;
            InitPak();
            InitEnemyGameMode();

            if (GameState != 0) {
                NaChangeSoundMode(0, 0);
                NasAudioInput();
                GameState = 0;
                NewSequenceMode = 0;
                NaSeqStart(SEQ_MENU_MAIN_MENU);
            }

            switch (KBGNumberNext) {
                case MENU_FADE_TYPE_MAIN: {
                    MenuScreenB = MAIN_MENU_PLAYER_SELECT;
                    NaSeqStart(SEQ_MENU_MAIN_MENU);
                    MenuMultiplayerSelection = 1;
                    if (ScreenSplitB >= NUM_SCREEN_MODES || ScreenSplitB < 0) {
                        ScreenSplitB = SCREEN_MODE_1P;
                    }
                    break;
                }
                case MENU_FADE_TYPE_BACK: {
                    MenuScreenB = MAIN_MENU_OK_SELECT_GO_BACK;
                    break;
                }
                case MENU_FADE_TYPE_DATA: {
                    // why...
                    switch (MenuScreenB) {
                        default:
                            MenuScreenB = MAIN_MENU_MODE_SUB_SELECT_GO_BACK;
                            break;
                        case MAIN_MENU_OPTION:
                        case MAIN_MENU_DATA:
                            MenuScreenB = MAIN_MENU_PLAYER_SELECT;
                            break;
                    }
                    break;
                }
                case MENU_FADE_TYPE_OPTION: {
                    MenuScreenB = MAIN_MENU_PLAYER_SELECT;
                    break;
                }
            }
            break;
        }
        case 2:
        case CHARACTER_SELECT_MENU: {
            switch (KBGNumberNext) {
                case MENU_FADE_TYPE_MAIN: {
                    MenuScreenC = PLAYER_SELECT_MENU_MAIN;
                    if (GameState == 0) {
                        for (i = 0; i < ARRAY_COUNT(Player1Character); i++) {
                            if (i < MenuMultiplayerSelection) {
                                Player1Character[i] = i + 1;
                            } else {
                                Player1Character[i] = 0;
                            }
                            CharacterGridIsSelected[i] = false;
                            CharacterSelections[i] = i;
                        }
                        PlaySound(SOUND_MENU_SELECT_PLAYER);
                    } else {
                        NaChangeSoundMode(0, 0);
                        NasAudioInput();
                        GameState = 0;
                        NewSequenceMode = 0;
                        NaSeqStart(SEQ_MENU_MAIN_MENU);
                        for (i = 0; i < ARRAY_COUNT(CharacterGridIsSelected); i++) {
                            CharacterGridIsSelected[i] = false;
                        }
                    }
                    break;
                }
                case MENU_FADE_TYPE_BACK: {
                    MenuScreenC = PLAYER_SELECT_MENU_OK_GO_BACK;
                    for (i = 0; i < ARRAY_COUNT(CharacterGridIsSelected); i++) {
                        if (MenuMultiplayerSelection > i) {
                            CharacterGridIsSelected[i] = true;
                        } else {
                            CharacterGridIsSelected[i] = false;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case 3:
        case COURSE_SELECT_MENU: {
            if (GameMode == BATTLE) {
                CupSelect = BATTLE_CUP;
                CupLevel = 4;
                MenuScreenA = SUB_MENU_MAP_SELECT_BATTLE_COURSE;
            } else {
                if (CupSelect == BATTLE_CUP) {
                    CupSelect = MUSHROOM_CUP;
                }
                MenuScreenA = SUB_MENU_MAP_SELECT_CUP;
            }
            if (GameState != 0) {
                NaChangeSoundMode(0, 0);
                NasAudioInput();
                GameState = 0;
                NewSequenceMode = 0;
                NaSeqStart(SEQ_MENU_MAIN_MENU);
            }
            PlaySound(SOUND_MENU_SELECT_MAP);
            BeforeCupSelect = 0;
            if (GameMode == GRAND_PRIX) {
                CourseSelect = 0;
            }

            for (i = 0; i < ARRAY_COUNT(PlayerGPpoints); i++) {
                PlayerGPpoints[i] = 0;
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
    KFlashCounter = 0x20;
}

/**
 * Self explanatory, Changes Sound Mode pack
 */
void SettingSoundOutMode(void) {
    UNUSED u32 pad;
    union GameModePack pack;

    pack = SSoundMenuPack;
    if ((SoundOutput == SOUND_STEREO) || (SoundOutput == SOUND_HEADPHONES) || (SoundOutput == SOUND_MONO)) {
        NaiSeqBufFlagCheck(pack.Modes[SoundOutput] | 0xE0000000);
    }
}

/**
 * Checks is a fade render Mode is Active so menus can't be
 * interacted while a fade transition is Active
 */
bool FadeChecker(void) {
    if ((FadingFlag[4] == 2) || (FadingFlag[4] == 3) || (FadingFlag[4] == 4) ||
        (FadingFlag[4] == 7)) {
        return true;
    }
    return false;
}

/**
 * Unused debug function, prints the character id for the Player and both controller pak ghosts
 */
UNUSED void GhostChecker(s32 arg0, s32 arg1) {
    struct_8018EE10_entry* pak1 = D_8018EE10;
    struct_8018EE10_entry* pak2 = (struct_8018EE10_entry*) KdlBuff;

    rmonPrintf("ghost_kart=%d,", GhostKart);
    rmonPrintf("pak1_ghost_kart=%d,", (pak1 + arg0)->CharacterId);
    rmonPrintf("pak2_ghost_kart=%d\n", (pak2 + arg1)->CharacterId);
}
