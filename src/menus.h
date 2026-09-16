#ifndef MENUS_H
#define MENUS_H

#include <PR/os.h>
#include <common_structs.h>
#include <defines.h>

// needs to be a union (or array...?) to go into rodata as a const
// terrible for endianness... Best guess as to what this is for..
union GameModePack {
    u8 modes[4];
    s32 word;
};

/**
 * @brief Options for KBGNumberNext
 */
enum MenuFadeTypes {
    MENU_FADE_TYPE_MAIN,
    MENU_FADE_TYPE_BACK,
    MENU_FADE_TYPE_DEMO,
    MENU_FADE_TYPE_DATA,
    MENU_FADE_TYPE_OPTION,
    MENU_FADE_TYPE_MAX
};

/**
 * @brief Options for menuScreenA
 */
enum SubMenuSelectionType {
    SUB_MENU_NONE,
    SUB_MENU_DATA, // 0x01, value repurposed in map select
    SUB_MENU_MAP_SELECT_CUP = 0x01,
    SUB_MENU_MAP_SELECT_COURSE,
    SUB_MENU_MAP_SELECT_OK,
    SUB_MENU_MAP_SELECT_BATTLE_COURSE,
    SUB_MENU_DATA_OPTIONS = 0x0B,
    SUB_MENU_DATA_ERASE_CONFIRM,
    SUB_MENU_DATA_CANT_ERASE,
    SUB_MENU_OPTION_MIN = 0x15,
    SUB_MENU_OPTION_RETURN_GAME_SELECT = SUB_MENU_OPTION_MIN,
    SUB_MENU_OPTION_SOUND_MODE,
    SUB_MENU_OPTION_COPY_CONTROLLER_PAK,
    SUB_MENU_OPTION_ERASE_ALL_DATA,
    SUB_MENU_OPTION_MAX = SUB_MENU_OPTION_ERASE_ALL_DATA, // 0x18
    SUB_MENU_ERASE_MIN = 0x1E,
    SUB_MENU_ERASE_QUIT = SUB_MENU_ERASE_MIN,
    SUB_MENU_ERASE_ERASE,
    SUB_MENU_ERASE_MAX = SUB_MENU_ERASE_ERASE, // 0x1F
    SUB_MENU_SAVE_DATA_ERASED,                 // 0x20
    SUB_MENU_COPY_PAK_FROM_GHOST_MIN = 0x28,
    SUB_MENU_COPY_PAK_FROM_GHOST1_1P = SUB_MENU_COPY_PAK_FROM_GHOST_MIN,
    SUB_MENU_COPY_PAK_FROM_GHOST2_1P,
    SUB_MENU_COPY_PAK_FROM_GHOST_MAX = SUB_MENU_COPY_PAK_FROM_GHOST2_1P, // 0x29
    SUB_MENU_COPY_PAK_ERROR_2P_MIN,                                      // 0x2A
    SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA = SUB_MENU_COPY_PAK_ERROR_2P_MIN,
    SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA,
    SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P,
    SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P,
    SUB_MENU_COPY_PAK_ERROR_2P_MAX = SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P, // 0x2D
    SUB_MENU_COPY_PAK_TO_GHOST_MIN = 0x32,
    SUB_MENU_COPY_PAK_TO_GHOST1_2P = SUB_MENU_COPY_PAK_TO_GHOST_MIN,
    SUB_MENU_COPY_PAK_TO_GHOST2_2P,
    SUB_MENU_COPY_PAK_TO_GHOST_MAX = SUB_MENU_COPY_PAK_TO_GHOST2_2P, // 0x33
    SUB_MENU_COPY_PAK_ERROR_1P_MIN,
    SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P = SUB_MENU_COPY_PAK_ERROR_1P_MIN, // 0x34
    SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P,
    SUB_MENU_COPY_PAK_ERROR_CANT_CREATE_1P,
    SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P,
    SUB_MENU_COPY_PAK_ERROR_1P_MAX,
    SUB_MENU_COPY_PAK_PROMPT_MIN = SUB_MENU_COPY_PAK_ERROR_1P_MAX, // 0x38
    SUB_MENU_COPY_PAK_PROMPT_QUIT = SUB_MENU_COPY_PAK_PROMPT_MIN,
    SUB_MENU_COPY_PAK_PROMPT_COPY,
    SUB_MENU_COPY_PAK_PROMPT_MAX = SUB_MENU_COPY_PAK_PROMPT_COPY, // 0x39
    SUB_MENU_COPY_PAK_ACTION_MIN,
    SUB_MENU_COPY_PAK_START = SUB_MENU_COPY_PAK_ACTION_MIN, // 0x3A
    SUB_MENU_COPY_PAK_COPYING,
    SUB_MENU_COPY_PAK_COMPLETED,
    SUB_MENU_COPY_PAK_ACTION_MAX = SUB_MENU_COPY_PAK_COMPLETED, // 0x3C
    SUB_MENU_COPY_PAK_UNABLE_ERROR_MIN = 0x41,
    SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P = SUB_MENU_COPY_PAK_UNABLE_ERROR_MIN,
    SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P,
    SUB_MENU_COPY_PAK_UNABLE_ERROR_MAX = SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P,
    SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT = 0x46,
    SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE
};

/**
 * @brief Options for menuScreenB
 */
enum MainMenuSelectionType {
    MAIN_MENU_NONE,
    MAIN_MENU_OPTION,
    MAIN_MENU_DATA,
    MAIN_MENU_PLAYER_SELECT,
    MAIN_MENU_MODE_SELECT,            // Mario GP - T. Trials - VS - Battle
    MAIN_MENU_MODE_SUB_SELECT,        // CC Selection and T.Trials options (Begin, Data)
    MAIN_MENU_OK_SELECT,              // OK option after selecting a submode
    MAIN_MENU_OK_SELECT_GO_BACK,      // OK option after going back from player select
    MAIN_MENU_MODE_SUB_SELECT_GO_BACK // Used in T.Trials after going back to Data option from course data
};

/**
 * @brief Options for menuScreenC
 */
enum PlayerSelectMenuSelectionTypes {
    PLAYER_SELECT_MENU_NONE,
    PLAYER_SELECT_MENU_MAIN,
    PLAYER_SELECT_MENU_OK,        // OK option after selecting characters
    PLAYER_SELECT_MENU_OK_GO_BACK // OK option after going back from map select
};

/**
 * @brief Options for title_mode
 */
enum DebugMenuSelectionTypes {
    DEBUG_MENU_NONE,
    DEBUG_MENU_DISABLED,
    DEBUG_MENU_DEBUG_MODE,
    DEBUG_MENU_COURSE,
    DEBUG_MENU_SCREEN_MODE,
    DEBUG_MENU_PLAYER,
    DEBUG_MENU_SOUND_MODE,
    DEBUG_MENU_GIVE_ALL_GOLD_CUP,
    DEBUG_MENU_OPTION_SELECTED = 0x40
};

/**
 * @brief Options for gControllerPakMenuSelection
 */
enum ControllerPakMenuSelectionTypes {
    CONTROLLER_PAK_MENU_NONE,
    CONTROLLER_PAK_MENU_SELECT_RECORD,
    CONTROLLER_PAK_MENU_END,
    CONTROLLER_PAK_MENU_ERASE,
    CONTROLLER_PAK_MENU_QUIT,
    CONTROLLER_PAK_MENU_TABLE_GAME_DATA,
    CONTROLLER_PAK_MENU_GO_TO_ERASING,
    CONTROLLER_PAK_MENU_ERASING,
    CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED,
    CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK,
    CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED
};

/**
 * @brief Options for gCourseRecordsMenuSelection
 */
enum CourseRecordsMenuSelectionTypes {
    COURSE_RECORDS_MENU_MIN,
    COURSE_RECORDS_MENU_RETURN_MENU = COURSE_RECORDS_MENU_MIN,
    COURSE_RECORDS_MENU_ERASE_RECORDS,
    COURSE_RECORDS_MENU_ERASE_GHOST,
    COURSE_RECORDS_MENU_MAX = COURSE_RECORDS_MENU_ERASE_GHOST
};

/**
 * @brief Options for gCourseRecordsSubMenuSelection
 */
enum CourseRecordsSubMenuSelectionTypes {
    COURSE_RECORDS_SUB_MENU_MIN,
    COURSE_RECORDS_SUB_MENU_QUIT = COURSE_RECORDS_SUB_MENU_MIN,
    COURSE_RECORDS_SUB_MENU_ERASE,
    COURSE_RECORDS_SUB_MENU_MAX = COURSE_RECORDS_SUB_MENU_ERASE
};

/**
 * @brief Options for gDebugGotoScene
 */
enum DebugGotoSceneTypes {
    DEBUG_GOTO_RACING,
    DEBUG_GOTO_ENDING,
    DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT,
    DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA
};

/**
 * @brief Options for KBGChange
 */
enum FadeModeSelectionTypes { FADE_MODE_NONE, FADE_MODE_MAIN, FADE_MODE_LOGO };

/**
 * @brief Options for gControllerPakScrollDirection
 */
enum ControllerPakScrollDirectionTypes {
    CONTROLLER_PAK_SCROLL_DIR_NONE,
    CONTROLLER_PAK_SCROLL_DIR_DOWN,
    CONTROLLER_PAK_SCROLL_DIR_UP
};

/* functions */
void UpdateController(void);
void optionController(struct Controller*, u16);
void taselController(struct Controller*, u16);
void dselController(struct Controller*, u16);
void nintenLogoControler(struct Controller*, u16);
void cpControler(struct Controller*, u16);
void TitleController(struct Controller*, u16);
void gsel_popup_data(void);
void GSelController(struct Controller*, u16);
bool psel_check(s32);
void PSelController(struct Controller*, u16);
void MSelController(struct Controller*, u16);
void k_var_init(s32);
void FlashCounterReset(void);
void Setting_SoundOutMode(void);
bool FadeChecker(void);

/* data */
extern s32 logo_cam_z;
extern f32 logo_scale_z;
extern f32 logo_rot_x;
extern f32 logo_rot_y;
extern f32 logo_rot_z;
extern f32 logo_pos_x;
extern f32 logo_pos_y;
extern f32 logo_pos_z;

extern s32 KBGNumberNext;

extern s8 g_player1Character[4];
extern s8 gCharacterGridIsSelected[4];
extern s8 menuScreenA;
extern s8 menuScreenB;
extern s8 menuScreenC;
extern s8 title_mode;
extern s8 gControllerPakMenuSelection;
extern s8 gScreenModeListIndex;
extern u8 sound_output;
extern s8 g_menuMultiplayerSelection;
extern s8 vs_result_cursor;
extern s8 ta_result_cursor;
extern s8 bt_result_cursor;
extern s8 gTimeTrialDataCourseIndex;
extern s8 gCourseRecordsMenuSelection;
extern s8 gCourseRecordsSubMenuSelection;
extern s8 gDebugGotoScene;
extern s8 g_InGameTT;
extern s8 g_InGame;
extern s32 titleDemo;
extern s32 title_counter2;
extern s8 demo_play_flag;
extern s8 g_cupSelect;
extern s8 g_courseSelect;
extern s8 fb_clear_flg;

extern s32 KBGNumber; // D_800E86A0
extern s32 KBGChange;
extern s8 g_CharacterSelections[];

extern s8 gGameModeMenuColumn[];
extern s8 gGameModeSubMenuColumn[4][3];
extern s8 gNextDemoId;
extern s8 gControllerPakSelectedTableRow;
extern s8 gControllerPakVisibleTableRows[];
extern s8 gControllerPakScrollDirection;

extern const s8 gPlayerModeSelection[];
extern const s32 gGameModePlayerSelection[][3];
extern const s16 gCupCourseOrder[NUM_CUPS][NUM_COURSES_PER_CUP];

// end of menus.c variables

#endif /* MENUS_H */
