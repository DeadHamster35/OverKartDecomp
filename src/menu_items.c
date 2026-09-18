#include <ultra64.h>
#include <PR/ultratypes.h>
#include <macros.h>
#include <defines.h>
#include <segments.h>
#include <sounds.h>
#include <mk64.h>
#include <course.h>

#include "code_800029B0.h"
#include "menu_items.h"
#include "cpu_vehicles_camera_path.h"
#include "code_8006E9C0.h"
#include "menus.h"
#include "save.h"
#include "code_80057C60.h"
#include "credits.h"
#include "data_segment2.h"
#include "code_800AF9B0.h"
#include "code_80281780.h"
#include "memory.h"
#include "audio/external.h"
#include "render_objects.h"
#include "replays.h"
#include <assets/common_data.h>
#include "textures.h"
#include "math_util.h"
#include "save_data.h"
#include "podium_ceremony_actors.h"
#include "skybox_and_splitscreen.h"
#include <assets/startup_logo.h>
#include "buffers.h"
#include "racing/race_logic.h"
#include "ending/code_80281C40.h"
#include "spawn_players.h"
#include "render_player.h"
#include "decode.h"
//! @todo Move gDynamicP out of main.h
// Unfortunately that's not a small effort due to weird import structure in this project
#include "main.h"
#include "OverKartHooks.h"

void guMtxCatL(Mtx* m, Mtx* n, Mtx* res);

u16* tex_buff;
u32* melt_buff;
u8* k_work;
u8* additional_point;
void* kdl_buff;
/**
 * List of bytes indexed by character ID
 * Indicates number of Grand Prix points that character
 * has scored
 */
s8 g_playerGPpoints[8];
s8 d_points_rank[8];
s8 k_skip_flg;
s8 k_skip_status;
ObjBlock gMenuItems[MENU_ITEMS_MAX];
struct_8018DEE0_entry seq_data_block[D_8018DEE0_SIZE];
struct_8018E060_entry change_tex_buff_block[D_8018E060_SIZE];
UNUSED u8 menu_item_bss_padding0[8];
struct_8018E0E8_entry change_tex_buff_block_vs[D_8018E0E8_SIZE];
s32 tex_buff_num;
TextureMap sMenuTextureMap[TEXTURE_MAP_MAX];
s32 all_tex_block_num;
Gfx* kdl_buff_ptr;
s32 all_kdl_block_num;
struct_8018E768_entry kdl_buff_block[D_8018E768_SIZE];
s32 k_flash_counter;
s8 g_FadingFlag[5];
u32 g_FadeCounter[5];
u32 g_FadeCounter2[4];
s32 D_8018E7E0;
struct UnkStruct_8018E7E8 old_screen_position[D_8018E7E8_SIZE];
struct UnkStruct_8018E7E8 old_screen_size[D_8018E810_SIZE];
s8 thunder_flg[4]; // Import to C was required for matching.
s32 D_8018E83C;

s32 thunder_counter[4]; // This may all be one big array.
s32 vc_color_num[2]; // This is probably incorrect. Fix after decomping code.
s32 vc_color_step[2];
s8 afont_color;
s32 D_8018E864_pad;
OSPfs gControllerPak1FileHandle;
OSPfs gControllerPak2FileHandle;
OSPfsState pfsState[16];
s32 pfsError[16]; // 0 = Ok, anything else = error.
s32 gControllerPak1NumFilesUsed;
s32 gControllerPak1MaxWriteableFiles;

s32 gControllerPak1NumPagesFree;
s32 gControllerPak1FileNote;
s32 gControllerPak2FileNote;
s32 menu_item_bss_pad2;
ALIGNED8 SaveData eeprom_save_data;

u8 rankin_fanfare;
u8 ending_message_flg;
s32 logo_timer;

POSXY lgsel_default_positions[] = {
    { 0x3d, 0x11, 0x00, 0x00 }, { 0x15, 0x3e, 0x00, 0x00 }, { 0x5c, 0x3e, 0x00, 0x00 },
    { 0xa3, 0x3e, 0x00, 0x00 }, { 0xea, 0x3e, 0x00, 0x00 }, { 0x10a, 0xc8, 0x00, 0x00 },
    { 0x15, 0xc8, 0x00, 0x00 }, { 0x55, 0xc8, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },
};

POSXY gsel_popup_positions[] = {
    { 0x40, 0x41, 0x00, 0x00 },
    { 0x40, 0x53, 0x00, 0x00 },
    { 0x40, 0x65, 0x00, 0x00 },
    { 0x40, 0x77, 0x00, 0x00 },
};

POSXY PlayerSelectPositions[][4] = {
    {
        { 0x18, 0x3f, 0x00, 0x00 },
        { 0x5d, 0x3f, 0x00, 0x00 },
        { 0xa2, 0x3f, 0x00, 0x00 },
        { 0xe7, 0x3f, 0x00, 0x00 },
    },
    {
        { 0x18, 0x91, 0x00, 0x00 },
        { 0x5d, 0x91, 0x00, 0x00 },
        { 0xa2, 0x91, 0x00, 0x00 },
        { 0xe7, 0x91, 0x00, 0x00 },
    },
};

POSXY msel_cupbox_position[] = {
    { 0x17, 0x3b, 0x00, 0x00 },
    { 0x5d, 0x3b, 0x00, 0x00 },
    { 0xa2, 0x3b, 0x00, 0x00 },
    { 0xe8, 0x3b, 0x00, 0x00 },
};

POSXY msel_map4box_position[] = {
    { 0x17, 0x70, 0x00, 0x00 },
    { 0x57, 0x70, 0x00, 0x00 },
    { 0x17, 0x97, 0x00, 0x00 },
    { 0x57, 0x97, 0x00, 0x00 },
};

// In a perfect world this would be `POSXY lpsel_character_ok_position[][4]`
POSXY lpsel_character_ok_position[] = {
    { 0x80, 0x58, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x80, 0x3f, 0x00, 0x00 }, { 0x80, 0x91, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x5a, 0x58, 0x00, 0x00 }, { 0xa6, 0x58, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x5a, 0x3f, 0x00, 0x00 }, { 0xa6, 0x3f, 0x00, 0x00 }, { 0x5a, 0x91, 0x00, 0x00 }, { 0xa6, 0x91, 0x00, 0x00 },
};

POSXY lmsel_circuit_position[][2] = {
    {
        { 0x9d, 0x70, 0x00, 0x00 },
        { 0x128, 0x81, 0x00, 0x00 },
    },
    {
        { 0x9d, 0x88, 0x00, 0x00 },
        { 0x128, 0x99, 0x00, 0x00 },
    },
    {
        { 0x9d, 0xa0, 0x00, 0x00 },
        { 0x128, 0xb1, 0x00, 0x00 },
    },
    {
        { 0x9d, 0xb8, 0x00, 0x00 },
        { 0x128, 0xc9, 0x00, 0x00 },
    },
};

POSXY msel_bestbox_default_position[] = {
    { 0xff6a, 0x3b, 0x00, 0x00 },
    { 0x172, 0x3b, 0x00, 0x00 },
};

POSXY msel_bestbox_target_position[] = {
    { 0x17, 0x3b, 0x00, 0x00 },
    { 0xc5, 0x3b, 0x00, 0x00 },
};

POSXY msel_cup_positions[] = {
    { 0x28, 0x73, 0x00, 0x00 },
    { 0x28, 0x3c, 0x00, 0x00 },
};

POSXY cp_cursor_position[] = {
    { 0x3e, 0x43, 0x00, 0x00 },     { 0xa1, 0x43, 0x00, 0x00 },
    { 0x3e, 0xc5, 0x00, 0x00 },     { 0xa1, 0xc5, 0x00, 0x00 },

    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0x140, 0xf0, 0x00, 0x00 },
    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0xffc0, 0xffc0, 0x00, 0x00 },

    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0x140, 0xffc0, 0x00, 0x00 },
    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0xffc0, 0xffc0, 0x00, 0x00 },

    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0x140, 0xffc0, 0x00, 0x00 },
    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0x140, 0xf0, 0x00, 0x00 },
};

POSXY vs_face_start_position2 = { 0x140, 0x23, 0x00, 0x00 };

// In a perfect world this would be `POSXY vs_face_target_position[][4]`
POSXY vs_face_target_position[] = {
    { 0x50, 0x23, 0x00, 0x00 }, { 0xb0, 0x23, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x32, 0x23, 0x00, 0x00 }, { 0x80, 0x23, 0x00, 0x00 }, { 0xce, 0x23, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x18, 0x23, 0x00, 0x00 }, { 0x5d, 0x23, 0x00, 0x00 }, { 0xa2, 0x23, 0x00, 0x00 }, { 0xe7, 0x23, 0x00, 0x00 },
};

// Versus menu coordinates
POSXY vs_result_cursor_position[] = {
    { 0x61, 0xa7, 0x00, 0x00 },
    { 0x61, 0xb6, 0x00, 0x00 },
    { 0x61, 0xc5, 0x00, 0x00 },
    { 0x61, 0xd4, 0x00, 0x00 },
};

POSXY vs_result_winlose_position[] = {
    { 0x30, 0x4b, 0x00, 0x00 },
    { 0x109, 0x4b, 0x00, 0x00 },
};

POSXY ta_result_cursor_position[] = {
    { 0xad, 0x8d, 0x00, 0x00 }, { 0xad, 0x9a, 0x00, 0x00 }, { 0xad, 0xa7, 0x00, 0x00 },
    { 0xad, 0xb4, 0x00, 0x00 }, { 0xad, 0xc1, 0x00, 0x00 }, { 0xad, 0xce, 0x00, 0x00 },
};

POSXY ta_ghostb_cursor_position[] = {
    { 0xac, 0xa5, 0x00, 0x00 },
    { 0xac, 0xc3, 0x00, 0x00 },
};

POSXY ta_ghostc_cursor_position[] = {
    { 0xc0, 0xb3, 0x00, 0x00 },
    { 0xc0, 0xc2, 0x00, 0x00 },
};

POSXY tar_select_cursor_position[] = {
    { 0x61, 0x94, 0x00, 0x00 }, { 0x61, 0xa1, 0x00, 0x00 }, { 0x61, 0xae, 0x00, 0x00 },
    { 0x61, 0xbb, 0x00, 0x00 }, { 0x61, 0xc8, 0x00, 0x00 }, { 0x61, 0xd5, 0x00, 0x00 },
};

POSXY tar_ghostb_cursor_position[] = {
    { 0x52, 0x90, 0x00, 0x00 },
    { 0x52, 0xa4, 0x00, 0x00 },
};

POSXY tar_ghostc_cursor_position[] = {
    { 0x76, 0x95, 0x00, 0x00 },
    { 0x76, 0xa4, 0x00, 0x00 },
};

POSXY tsel_cup_positions[] = {
    { 0x17, 0xa, 0x00, 0x00 }, { 0x5d, 0xa, 0x00, 0x00 }, { 0xa2, 0xa, 0x00, 0x00 },
    { 0xe8, 0xa, 0x00, 0x00 }, { 0x17, 0xa, 0x00, 0x00 },
};

POSXY ending1_messages_start_position[] = {
    { 0x14a, 0x32, 0x00, 0x00 },  { 0xff60, 0xd4, 0x00, 0x00 }, { 0xa0, 0x10e, 0x00, 0x00 },
    { 0xff60, 0xbe, 0x00, 0x00 }, { 0x143, 0x5a, 0x00, 0x00 },
};

POSXY ending1_messages_target_position[] = {
    { 0xa0, 0x32, 0x00, 0x00 }, { 0x9b, 0xd4, 0x00, 0x00 }, { 0xa0, 0x50, 0x00, 0x00 },
    { 0x9b, 0xbe, 0x00, 0x00 }, { 0x80, 0x5a, 0x00, 0x00 },
};

RGBA16 psel_cursor_color[] = {
    { 0x00, 0xf3, 0xf3, 0xff }, { 0xff, 0xa8, 0xc3, 0xff }, { 0xff, 0xfe, 0x7a, 0xff },
    { 0x7b, 0xfc, 0x7b, 0xff }, { 0xff, 0xff, 0x00, 0xff },
};

RGBA16 tsel_cursor_color[] = {
    { 0x00, 0xf3, 0xf3, 0xff },
    { 0xff, 0xa8, 0xc3, 0xff },
    { 0xff, 0xff, 0x00, 0xff },
};

RGBA16 titleback_color[] = {
    { 0xff, 0xaf, 0xaf, 0xff },
    { 0xaf, 0xff, 0xaf, 0xff },
    { 0xaf, 0xaf, 0xff, 0xff },
};

const s16 font_width[] = {
    0x000c, 0x000d, 0x000b, 0x000b, 0x000a, 0x000b, 0x000b, 0x000d, 0x0007, 0x000a, 0x000c, 0x000a, 0x0012, 0x000d,
    0x000c, 0x000c, 0x000c, 0x000c, 0x000b, 0x000d, 0x000c, 0x000c, 0x0012, 0x000d, 0x000c, 0x000c, 0x000a, 0x000a,
    0x000a, 0x0006, 0x001e, 0x0006, 0x000a, 0x0008, 0x000b, 0x000c, 0x000c, 0x000d, 0x000a, 0x000b, 0x000a, 0x000a,
    0x0008, 0x001c, 0x000a, 0x0010, 0x000f, 0x0010, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000e,
    0x000f, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000e, 0x000e,
    0x000e, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000f, 0x000e, 0x000f, 0x000e, 0x000f, 0x000f,
    0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000e, 0x000e, 0x000e, 0x000e, 0x000b, 0x000f,
    0x000f, 0x000f, 0x000f, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x001d, 0x000f,
    0x000f, 0x0017, 0x000f, 0x0017, 0x0017, 0x0017, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
};

char* cup_name[] = {
    "mushroom cup",
    "flower cup",
    "star cup",
    "special cup",
    "battle",
    // ????
    "mushroom cup",
    "flower cup",
    "star cup",
    "special cup",
};

#if !ENABLE_CUSTOM_COURSE_ENGINE
// Displays at beginning of course
char* g_StringTableCourseGP[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};

char* g_StringTableCourse[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};
#else

#endif

char* circuit_name_semilong_j[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};

#if !ENABLE_CUSTOM_COURSE_ENGINE
// Used in debug menu at splash screen
char* circuit_name_short[] = {
#include "assets/course_metadata/gCourseDebugNames.inc.c"
};
#else

#endif

const s8 map2circuit_chart[] = {
#include "assets/course_metadata/gPerCupIndexByCourseId.inc.c"
};

// @todo Increase this array for more than eight players
const s8 driver2pselset[] = { 0, 1, 4, 3, 5, 6, 2, 7 };

// Maps course IDs (as defined in the COURSES enum) to the cup they belong to
s8 map2cup_chart[] = {
#include "assets/course_metadata/gCupSelectionByCourseId.inc.c"
};

char* cup_type_names[] = {
    "none",
    "bronze",
    "silver",
    "gold",
};

char* drivers_name[] = {
    "MARIO", "LUIGI", "YOSHI", "KINOPIO", "D.KONG", "WARIO", "PEACH", "KOOPA",
};

char* drivers_name_j[] = {
    "MARIO",    "LUIGI", "YOSHI", "TOAD", "D.K.", "WARIO", "PEACH", "BOWSER",
    "ーーーー", // NOT HYPHENS!!! These are EUC-JP characters (0xa1 0xbc)
};

char* class_name[] = {
    "50(",
    "100(",
    "150(",
    "extra",
};

char* class_name_j[] = {
    "50(",
    "100(",
    "150(",
    "extra",
};

char* screen_mode_names[] = {
    "1p", "2players UD", "2players LR", "3players", "4players",
};

char* sound_mode_name[] = {
    "stereo",
    "head phone",
    "xxx",
    "monaural",
};

char* sound_mode_name_j[NUM_SOUND_MODES] = { "STEREO", "HEADPHONE", "", "MONO" };

char* win_lose_j[] = {
    "WINNER!",
    "LOSER!",
};

char* bestrec_j[] = {
    "BEST RECORDS",
    "BEST LAP",
};

// Might need a const?
char* gLapTimeText = "LAP TIME";

char* laptime_j[] = {
    "LAP 1",
    "LAP 2",
    "LAP 3",
    "TOTAL",
};

char* recordtime_j[] = {
    // The s/n/r/t here are not ASCII, they are EUC-JP characters
    // 0xae 0xf3/0xae 0xee/0xae 0xf2/0xae 0xf4
    "1 ｓ", "2 ｎ", "3 ｒ", "4 ｔ", "5 ｔ", " ",
};

char* ta_result_select_j[] = {
    "CONTINUE GAME", "RETRY", "COURSE CHANGE", "DRIVER CHANGE", "QUIT", "REPLAY", "SAVE GHOST",
};

char* vs_result_title_j[] = {
    "VS MATCH RANKING",
    "BATTLE RANKING",
};

// This is plain data, it should not end up in rodata
char ta_romghost_message_j[] = "NOW-MEET THE COURSE GHOST!!!";

char* no_controller_message[] = { "CONNECT A CONTROLLER TO SOCKET 1,", "THEN POWER ON AGAIN" };

char* msel_battle_messages_j[] = {
    "BATTLE GAME",
    "POP OPPOSING PLAYER'S BALLOONS",
    "WHEN ALL 3 ARE GONE,THEY ARE OUT!",
};

// This is plain data, it should not end up in rodata
char tsel_title_j[] = "a BUTTON*SEE DATA  B BUTTON*EXIT";

// This is plain data, it should not end up in rodata
char dsel_length_j[] = "distance";

char* dsel_lengthdata_j[] = {
#include "assets/course_metadata/sCourseLengths.inc.c"
};

char* dsel_select1_j[] = {
    "return to menu",
    "erase records for this course",
    "erase ghost from this course",
};

char* dsel_select2_j[] = {
    "quit",
    "erase",
};

// Why oh why is this array flat? It should be dsel_select2_messages_j[][3]
char* dsel_select2_messages_j[] = {
    "THE BEST RECORDS AND BEST", "LAP FOR THIS COURSE WILL BE", "ERASED.  IS THIS OK?",

    "GHOST DATA FOR THIS",       "COURSE WILL BE ERASED.",      "IS THIS OK?",
};

char* dsel_select3_messages_j[] = {
    "UNABLE TO ERASE ",
    "GHOST DATA",
};

char* option_select1_messages_j[] = {
    "RETURN TO GAME SELECT",
    "SOUND MODE",
    "COPY N64 CONTROLLER PAK",
    "ERASE ALL DATA",
};

char* option_select3_messages_j[] = {
    "ALL SAVED DATA WILL BE",
    "PERMANENTLY ERASED.",
    "ARE YOU REALLY SURE?",
};

char* option_select3e_messages_j[] = {
    "",
    "ALL SAVED DATA",
    "HAS BEEN NOW ERASED.",
};

// In a perfect world this would be `char *option_copy_pak1err_messages_j[][4]`
char* option_copy_pak1err_messages_j[] = {
    "CONTROLLER 1 DOES NOT HAVE ",
    "N64 CONTROLLER PAK",
    "",
    "",

    "UNABLE TO READ ",
    "N64 CONTROLLER PAK DATA ",
    "FROM CONTROLLER 1",
    "",

    "UNABLE TO CREATE GAME DATA ",
    "FROM CONTROLLER 1 ",
    "N64 CONTROLLER PAK",
    "",

    "UNABLE TO COPY GHOST ",
    "-- INSUFFICIENT FREE PAGES ",
    "IN CONTROLLER 1 ",
    "N64 CONTROLLER PAK",
};

// In a perfect world this would be `char *option_copy_pak2err_messages_j[][3]`
char* option_copy_pak2err_messages_j[] = {
    "NO GHOST DATA ",         "IN CONTROLLER 2 ",         "N64 CONTROLLER PAK",

    "NO MARIO KART 64 DATA ", "PRESENT IN CONTROLLER 2 ", "N64 CONTROLLER PAK",

    "CONTROLLER 2 ",          "DOES NOT HAVE ",           "N64 CONTROLLER PAK SET",

    "UNABLE TO READ DATA ",   "FROM CONTROLLER 2 ",       "N64 CONTROLLER PAK",
};

// In a perfect world this would be `char *option_copy_copyerr_messages_j[][4]`
char* option_copy_copyerr_messages_j[] = {
    "UNABLE TO COPY DATA ", "FROM CONTROLLER 1 ", "N64 CONTROLLER PAK",

    "UNABLE TO READ DATA ", "FROM CONTROLLER 2 ", "N64 CONTROLLER PAK",
};

char* option_copy_pak_j[] = {
    "CONTROLLER 1",
    "CONTROLLER 2",
};

char* option_copy_pak2sel_message_j[] = {
    "WHICH FILE DO YOU WANT TO MAKE A COPY OF?",
    "TO WHICH FILE DO YOU WANT TO COPY?",
};

char* option_copy_sure_messages_j[] = {
    "CURRENT DATA WILL BE ERASED,",
    "IS THIS OK?",
};

char* option_copy_sure2_messages_j[] = {
    "QUIT",
    "COPY",
};

char* option_copy_copy_messages_j[] = {
    "COPYING",
    "DATA COPY COMPLETED",
};

// In a perfect world this would be `char *gsel_caution_messages_j[][4]`
char* gsel_caution_messages_j[] = {
    "NO N64 CONTROLLER PAK DETECTED",
    "TO SAVE GHOST DATA, ",
    "INSERT N64 CONTROLLER PAK ",
    "INTO CONTROLLER 1",

    "UNABLE TO READ ",
    "N64 CONTROLLER PAK DATA",
    "",
    "",

    "",
    "",
    "",
    "",

    "INSUFFICIENT FREE PAGES AVAILABLE ",
    "IN N64 CONTROLLER PAK TO CREATE ",
    "GAME DATA, PLEASE FREE 121 PAGES.",
    "SEE INSTRUCTION BOOKLET FOR DETAILS.",
};

// Unused?
char* D_800E7980[] = {
    "TO SAVE GHOST DATA, ",
    "INSERT N64 CONTROLLER PAK ",
    "INTO CONTROLLER 1",
};

// In a perfect world this would be `char *ta_ghosta_messages_j[][7]`
char* ta_ghosta_messages_j[] = {
    "N64 CONTROLLER PAK ",
    "NOT DETECTED. ",
    "IF YOU WANT TO SAVE ",
    "THE GHOST DATA, ",
    "PLEASE INSERT ",
    "N64 CONTROLLER PAK ",
    "INTO CONTROLLER 1",

    "",
    "UNABLE TO SAVE ",
    "     THE GHOST",
    "",
    "",
    "",
    "",

    "",
    "UNABLE TO SAVE ",
    "     THE GHOST",
    "",
    "",
    "",
    "",

    "INSUFFICIENT ",
    "FREE PAGES AVAILABLE ",
    "",
    "-- GHOST DATA ",
    "COULD NOT BE SAVED",
    "",
    "",

    "",
    "CANNOT CREATE ",
    "     GAME DATA",
    "",
    "",
    "",
    "",

    "",
    "THIS GHOST IS ",
    "     ALREADY SAVED",
    "",
    "",
    "",
    "",
};

char* ta_lost_ghost_message_j[] = {
    "RACE DATA CANNOT ",
    "BE SAVED FOR GHOST",
};

char* ta_ghostb_messages_j[] = {
    "SELECT THE FILE ",
    "YOU WANT TO SAVE",
};

// Might need a const?
char* D_800E7A44 = "NO DATA";

char* ta_ghostb3_messages_j[] = {
    "CREATING ",
    "MARIO KART 64 ",
    "GAME DATA",
};

char* D_800E7A54[] = {
    "CANNOT CREATE GAME DATA",
    "",
    "",
};

char* ta_ghostc_messages_j[] = {
    "THE PREVIOUS DATA ",
    "WILL BE ERASED, ",
    "IS THIS OK?",
};

char* ta_ghostc_select_messages_j[] = {
    "QUIT",
    "SAVE",
};

char* ta_ghostd_messages_j[] = {
    "SAVING GHOST DATA",
    "",
    "PLEASE WAIT",
};

char* ta_ghostd2_messages_j[] = {
    "UNABLE TO SAVE ",
    "THE GHOST",
};

char* ending_message2[] = {
    "YOU ARE AWARDED THE",
    "GOLD CUP",
    "SILVER CUP",
    "BRONZE CUP",
};

// Might need a const?
char* D_800E7A98 = "MAYBE NEXT TIME!";

char* ending_message3[] = {
    "CONGRATULATIONS!",
    "WHAT A PITY!",
};

char* ending_message4[] = {
    "YOU PLACED", "    st", "    nd", "    rd", "    th", "    th", "    th", "    th", "    th",
};

const s8 additional_point_defaults[] = { 9, 6, 3, 1 };
const s8 screen_mode2num[] = {
    0, 0, 1, 0, 1, 0, 1, 2, 0, 1, 2, 3,
};
const s8 thunder_onoff[] = {
    0, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 1, 2, 2, 1, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

const s8 start_pause_seq[] = { 0x1f, 0x0b, 0x15, 0x29 };
const s8 bottom_pause_seq[] = { 0x20, 0x0f, 0x18, 0x2c };

RGBA16 k_thunder_color[] = {
    { 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0xff },
    { 0x00, 0x00, 0x50, 0xff },
    { 0xff, 0xff, 0xff, 0xff },
};

RGBA16 fade_color_bw[] = {
    { 0x00, 0x00, 0x00, 0xff },
    { 0xff, 0xff, 0xff, 0xff },
};

MenuTexture* D_800E7AF8[] = {
    D_02000000, D_02000028, D_02000050, D_02000078, D_020000A0, D_020000C8, D_020000F0, D_02000118, D_02000140,
    D_02000168, D_02000190, D_020001B8, D_020001E0, D_02000208, D_02000230, D_02000258, D_02000280, D_020002A8,
    D_020002D0, D_020002F8, D_02000320, D_02000348, D_02000370, D_02000398, D_020003C0, D_020003E8, D_02000410,
    D_02000438, D_02000460, D_02000488, D_020004B0, D_020004D8, D_02000500, D_02000528, D_02000550, D_02000578,
    D_020005A0, D_020005C8, D_020005F0, D_02000618, D_02000640, D_02000668, D_02000690, D_020006B8, D_020006E0,
    D_02000708, D_02000730, D_02000758, D_02000780, D_020007A8, D_020007D0, D_020007F8, D_02000820, D_02000848,
    D_02000870, D_02000898, D_020008C0, D_020008E8, D_02000910, D_02000938, D_02000960, D_02000988, D_020009B0,
    D_020009D8, D_02000A00, D_02000A28, D_02000A50, D_02000A78, D_02000AA0, D_02000AC8, D_02000AF0, D_02000B18,
    D_02000B40, D_02000B68, D_02000B90, D_02000BB8, D_02000BE0, D_02000C08, D_02000C30, D_02000C58, D_02000C80,
    D_02000CA8, D_02000CD0, D_02000CF8, D_02000D20, D_02000D48, D_02000D70, D_02000D98, D_02000DC0, D_02000DE8,
    D_02000E10, D_02000E38, D_02000E60, D_02000E88, D_02000EB0, D_02000ED8, D_02000F00, D_02000F28, D_02000F50,
    D_02000F78, D_02000FA0, D_02000FC8, D_02000FF0, D_02001018, D_02001040, D_02001068, D_02001090, D_020010B8,
    D_020010E0, D_02001108, D_02001130, D_02001158, D_02001180, D_020011A8, D_020011D0, D_020011F8, D_02001220,
    D_02001248, D_02001270, D_02001298, D_020012C0, D_020012E8, D_02001310, D_02001338, D_02001360, D_02001388,
    D_020013B0, D_020013D8, D_02001400, D_02001428, D_02001450, D_02001478, D_020014A0,
};

MenuTexture* D_800E7D0C[] = {
    D_020016BC, D_020016E4, D_0200170C, D_02001734, D_0200175C,
    D_02001784, D_020017AC, D_020017D4, D_020017FC, D_02001824,
};

MkAnimation* D_800E7D34[] = {
    D_0200198C, D_0200199C, D_020019AC, D_020019BC, D_020019CC, D_020019DC,
};

MenuTexture* gMenuTexturesBackground[] = {
    seg2_blue_sky_background_texture,
    seg2_sunset_background_texture,
};

MenuTexture* SelectNamePlateTable[] = {
    D_02001A8C, D_02001A64, D_02001AB4, D_02001A14, D_02001B04, D_020019EC, D_02001ADC, D_02001A3C,
};

MenuTexture* D_800E7D74[] = {
    seg2_mario_raceway_preview_texture,
    D_02001B54,
    D_02001B7C,
    D_02001BA4,
    D_02001BCC,
    D_02001BF4,
    D_02001C1C,
    D_02001C44,
    D_02001C6C,
    D_02001C94,
    D_02001CBC,
    D_02001CE4,
    D_02001D0C,
    D_02001D34,
    D_02001D5C,
    D_02001D84,
    D_02001DAC,
    D_02001DD4,
    D_02001DFC,
    D_02001E24,
};

MenuTexture* D_800E7DC4[] = {
    seg2_mario_raceway_title_texture,
    seg2_choco_mountain_title_texture,
    D_02004EF8,
    D_02004F20,
    D_02004F48,
    D_02004F70,
    D_02004F98,
    D_02004FC0,
    D_02004FE8,
    D_02005010,
    D_02005038,
    D_02005060,
    D_02005088,
    D_020050B0,
    D_020050D8,
    D_02005100,
    D_02005128,
    D_02005150,
    D_02005178,
    D_020051A0,
};

// Unused?
MkAnimation* D_800E7E14[] = {
    D_020020BC,
    D_020020CC,
    D_020020DC,
};

MkAnimation* D_800E7E20[] = {
    D_020020DC, D_020020EC, D_020020FC, D_0200210C, D_0200210C,
};

MkAnimation* D_800E7E34[] = {
    D_02001E64, D_02001E74, D_02001E84, D_02001E94, D_02001EA4, D_02001EB4, D_02001EC4,
    D_02001ED4, D_02001EE4, D_02001EF4, D_02001F04, D_02001F14, D_02001F24, D_02001F34,
    D_02001F44, D_02001F54, D_02001F64, D_02001F74, D_02001F84, D_02001F94,
};

MenuTexture* gGlyphTextureLUT[] = {
    seg2_textureFontLetterA,
    seg2_textureFontLetterB,
    seg2_textureFontLetterC,
    seg2_textureFontLetterD,
    seg2_textureFontLetterE,
    seg2_textureFontLetterF,
    seg2_textureFontLetterG,
    seg2_textureFontLetterH,
    seg2_textureFontLetterI,
    seg2_textureFontLetterJ,
    seg2_textureFontLetterK,
    seg2_textureFontLetterL,
    seg2_textureFontLetterM,
    seg2_textureFontLetterN,
    seg2_textureFontLetterO,
    seg2_textureFontLetterP,
    seg2_textureFontLetterQ,
    seg2_textureFontLetterR,
    seg2_textureFontLetterS,
    seg2_textureFontLetterT,
    seg2_textureFontLetterU,
    seg2_textureFontLetterV,
    seg2_textureFontLetterW,
    seg2_textureFontLetterX,
    seg2_textureFontLetterY,
    seg2_textureFontLetterZ,
    seg2_textureFontExclamationMark,
    seg2_textureFontMinus,
    seg2_textureFontInterogationMark,
    seg2_textureFontSimpleQuote,
    seg2_textureFontEmpty,
    seg2_textureFontDot,
    seg2_textureFontNumberZero,
    seg2_textureFontNumberOne,
    seg2_textureFontNumberTwo,
    seg2_textureFontNumberThree,
    seg2_textureFontNumberFour,
    seg2_textureFontNumberFive,
    seg2_textureFontNumberSix,
    seg2_textureFontNumberSeven,
    seg2_textureFontNumberEight,
    seg2_textureFontNumberNine,
    seg2_textureFontDoubleQuote,
    seg2_textureFontFourDote,
    seg2_textureFontPlus,
    seg2_textureFontCC,
    seg2_textureFontComma,
    seg2_textureFontEmpty,
    D_02002824,
    D_0200284C,
    D_02002874,
    D_0200289C,
    D_020028C4,
    D_020028EC,
    D_02002F54,
    D_02002914,
    D_02002F7C,
    D_0200293C,
    D_02002FA4,
    D_02002964,
    D_02002FCC,
    D_0200298C,
    D_02002FF4,
    D_020029B4,
    D_0200301C,
    D_020029DC,
    D_02003044,
    D_02002A04,
    D_0200306C,
    D_02002A2C,
    D_02003094,
    D_02002A54,
    D_020030BC,
    D_02002A7C,
    D_020030E4,
    D_02002AA4,
    D_0200310C,
    D_020033B4,
    D_02002ACC,
    D_02003134,
    D_02002AF4,
    D_0200315C,
    D_02002B1C,
    D_02003184,
    D_02002B44,
    D_02002B6C,
    D_02002B94,
    D_02002BBC,
    D_02002BE4,
    D_02002C0C,
    D_020031AC,
};

MenuTexture* D_800E7FF0[] = {
    D_02003274, D_02002C34, D_020031D4, D_0200329C, D_02002C5C, D_020031FC, D_020032C4, D_02002C84, D_02003224,
    D_020032EC, D_02002CAC, D_0200324C, D_02003314, D_02002CD4, D_02002CFC, D_02002D24, D_02002D4C, D_02002D74,
    D_0200333C, D_02002D9C, D_02003364, D_02002DC4, D_0200338C, D_02002DEC, D_02002E14, D_02002E3C, D_02002E64,
    D_02002E8C, D_02002EB4, D_02002EDC, D_02002F04, D_02002F2C, D_020033DC, D_02003404, D_0200342C, D_02003454,
    D_0200347C, D_020034A4, D_020034CC, D_020034F4, D_0200351C, D_02003544, D_0200356C, D_02003BD4,
};

MenuTexture* D_800E80A0[] = {
    D_02003594, D_02003BFC, D_020035BC, D_02003C24, D_020035E4, D_02003C4C, D_0200360C, D_02003C74,
    D_02003634, D_02003C9C, D_0200365C, D_02003CC4, D_02003684, D_02003CEC, D_020036AC, D_02003D14,
    D_020036D4, D_02003D3C, D_020036FC, D_02003D64, D_02003724, D_02003D8C, D_02004034, D_0200374C,
    D_02003DB4, D_02003774, D_02003DDC, D_0200379C, D_02003E04,
};

MenuTexture* D_800E8114[] = {
    D_020037C4, D_020037EC, D_02003814, D_0200383C, D_02003864, D_0200388C, D_02003E2C, D_02003EF4,
    D_020038B4, D_02003E54, D_02003F1C, D_020038DC, D_02003E7C, D_02003F44, D_02003904, D_02003EA4,
    D_02003F6C, D_0200392C, D_02003ECC, D_02003F94, D_02003954, D_0200397C, D_020039A4, D_020039CC,
};

MenuTexture* D_800E8174[] = {
    D_020039F4,
    D_02003FBC,
};

MenuTexture* D_800E817C[] = {
    D_02003A1C, D_02003FE4, D_02003A44, D_0200400C, D_02003A6C, D_02003A94, D_02003ABC, D_02003AE4, D_02003B0C,
    D_02003B34, D_02003B5C, D_02003B84, D_02003BAC, D_0200405C, D_02004084, D_020040AC, D_020040D4, D_020040FC,
    D_020043CC, D_02004444, D_0200437C, D_020043F4, D_02004124, D_0200414C, D_02004174, D_0200419C,
};

MenuTexture* D_800E81E4[] = {
    D_020041C4, D_020041EC, D_02004214, D_0200423C, D_02004264, D_0200428C, D_020042B4, D_020042DC, D_02004354,
    D_020043A4, D_0200441C, D_0200446C, D_02004494, D_020044BC, D_02004304, D_0200432C, D_020044E4, D_0200450C,
};

MenuTexture* D_800E822C[] = {
    seg2_textureFontComma,
    D_0200455C,
};

// In a perfect world this would be `MenuTexture *D_800E8234[][2]`
MenuTexture* D_800E8234[] = {
    seg2_menu_1p_column, D_020047DC, seg2_menu_2p_column, D_02004804,
    seg2_menu_3p_column, D_0200482C, seg2_menu_4p_column, D_02004854,
};

MenuTexture* D_800E8254[] = {
    seg2_game_select_texture, seg2_menu_1p_column, seg2_menu_2p_column, seg2_menu_3p_column,
    seg2_menu_4p_column,      D_0200487C,          gTextureMenuLOption, seg2_textureMenuRData,
};

// CC textures
MenuTexture* D_800E8274[] = {
    seg2_50_CC_texture,
    seg2_100_CC_texture,
    seg2_150_CC_texture,
    seg2_extra_CC_texture,
};

// Versus and battle textures
MenuTexture* D_800E8284[] = { D_020049BC, seg2_menu_no_item_texture };

// Time trials begin and data textures
MenuTexture* D_800E828C[] = {
    D_020049E4,
    seg2_data_texture,
};

// Duplicates of the above 3 arrays
// CC textures
MenuTexture* D_800E8294[] = {
    seg2_50_CC_texture,
    seg2_100_CC_texture,
    seg2_150_CC_texture,
    seg2_extra_CC_texture,
};

// Versus and battle textures
MenuTexture* D_800E82A4[] = { D_020049BC, seg2_menu_no_item_texture };

// Time trials begin and data textures
MenuTexture* D_800E82AC[] = {
    D_020049E4,
    seg2_data_texture,
};

MenuTexture* gMenuTexturesBorderPlayer[] = {
    seg2_P1_border_texture,
    seg2_P2_border_texture,
    seg2_P3_border_texture,
    seg2_P4_border_texture,
};

MenuTexture* gMenuTexturesTrackSelection[] = {
    seg2_menu_select_texture,        seg2_mushroom_cup_texture,     seg2_flower_cup_texture,
    seg2_star_cup_texture,           seg2_special_cup_texture,      seg2_mario_raceway_preview_small_texture,
    seg2_mushroom_cup_title_texture, seg2_flower_cup_title_texture, seg2_star_cup_title_texture,
    seg2_special_cup_title_texture,  seg2_battle_title_texture,     D_02004E80,
};

MenuTexture* D_800E82F4[] = {
    seg2_mushroom_cup_texture,
    seg2_flower_cup_texture,
    seg2_star_cup_texture,
    seg2_special_cup_texture,
    seg2_mario_raceway_preview_small_texture,
    seg2_mushroom_cup_title_texture,
    seg2_flower_cup_title_texture,
    seg2_star_cup_title_texture,
    seg2_special_cup_title_texture,
    seg2_battle_title_texture,
    D_02004E80,
};

MkAnimation* D_800E8320[] = {
    D_02006708, D_020068F0, D_02006CC0, D_02006EA8, D_02007090, D_02007280, D_02007468, D_02006AD8,
};

MkAnimation* TBSeq_psel_return[] = {
    D_02006778, D_02006960, D_02006D30, D_02006F18, D_02007100, D_020072F0, D_020074D8, D_02006B48,
};

MkAnimation* TBSeq_psel_p0[] = {
    D_02006708, D_020068F0, D_02006CC0, D_02006EA8, D_02007090, D_02007280, D_02007468, D_02006AD8,
};

MkAnimation* TBSeq_psel_p1[] = {
    D_02006718, D_02006900, D_02006CD0, D_02006EB8, D_020070A0, D_02007290, D_02007478, D_02006AE8,
};

MkAnimation* TBSeq_psel_p2[] = {
    D_02006778, D_02006960, D_02006D30, D_02006F18, D_02007100, D_020072F0, D_020074D8, D_02006B48,
};

MkAnimation* TBSeq_psel_p3[] = {
    D_02006788, D_02006970, D_02006D40, D_02006F28, D_02007110, D_02007300, D_020074E8, D_02006B58,
};

MkAnimation* TBSeq_psel_p4[] = {
    D_020067E0, D_020069C8, D_02006D98, D_02006F80, D_02007170, D_02007358, D_02007540, D_02006BB0,
};

MkAnimation* TBSeq_psel_p5[] = {
    D_02006838, D_02006A20, D_02006DF0, D_02006FD8, D_020071C8, D_020073B0, D_02007598, D_02006C08,
};

MkAnimation* TBSeq_psel_p6[] = {
    D_020068E0, D_02006AC8, D_02006E98, D_02007080, D_02007270, D_02007458, D_02007640, D_02006CB0,
};

s32 p1_pat_num[] = {
    0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
};

s32 p3_pat_num[] = {
    0x09, 0x09, 0x09, 0x09, 0x0a, 0x09, 0x09, 0x09,
};

s32 p4_pat_num[] = {
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
};

s32 p5_pat_num[] = {
    0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13,
};

Vtx* D_800E84C0[] = {
    D_02007BB8,
    D_02007CD8,
    D_02007DF8,
};

#ifndef AVOID_UB
Gfx* D_800E84CC[] = {
    D_02007838, D_02007858, D_02007878, D_02007898, D_020078B8, D_020078D8, D_020078F8, D_02007918,
};

Gfx* D_800E84EC[] = {
    D_02007938, D_02007958, D_02007978, D_02007998, D_020079B8, D_020079D8, D_020079F8, D_02007A18,
};

Gfx* D_800E850C[] = {
    D_02007A38, D_02007A58, D_02007A78, D_02007A98, D_02007AB8, D_02007AD8, D_02007AF8, D_02007B18,
};
#endif

s8 first_time_only_flg = 1;

// Speed of the "motion" effect intro logo model
f32 logo_delay_size = 0.0f;

// Speed of the intro logo model
f32 logo_yang_speed = 3.0f;

POSXY D_800E8538[] = { { 0x69, 0x9b, 0x00, 0x00 } };

// In a perfect world this would be `POSXY D_800E8540[][4]`
POSXY D_800E8540[] = {
    { 0x82, 0x64, 0x00, 0x00 }, { 0x82, 0x64, 0x00, 0x00 }, { 0x82, 0x64, 0x00, 0x00 }, { 0x82, 0x64, 0x00, 0x00 },

    { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 }, { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 },

    { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 }, { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 },

    { 0x28, 0x28, 0x00, 0x00 }, { 0xb2, 0x28, 0x00, 0x00 }, { 0x28, 0xa0, 0x00, 0x00 }, { 0xb2, 0xa0, 0x00, 0x00 },
};

// In a perfect world this would be `POSXY D_800E85C0[][4]`
POSXY D_800E85C0[] = {
    { 0x69, 0x8c, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 },

    { 0x69, 0x55, 0x00, 0x00 }, { 0x69, 0xcd, 0x00, 0x00 }, { 0x78, 0x28, 0x00, 0x00 }, { 0x78, 0xa0, 0x00, 0x00 },
};

// In a perfect world this would be `POSXY D_800E8600[][4]`
POSXY D_800E8600[] = {
    { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 }, { 0x78, 0x64, 0x00, 0x00 },

    { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 }, { 0x6e, 0x28, 0x00, 0x00 }, { 0x6e, 0xa0, 0x00, 0x00 },

    { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 }, { 0x28, 0x64, 0x00, 0x00 }, { 0xb2, 0x64, 0x00, 0x00 },

    { 0x28, 0x28, 0x00, 0x00 }, { 0xb2, 0x28, 0x00, 0x00 }, { 0x28, 0xa0, 0x00, 0x00 }, { 0xb2, 0xa0, 0x00, 0x00 },
};

f64 Ipower(f64 base, s32 exponent) {
    s32 positive_exponent;
    f64 result;

    if (exponent < 0) {
        positive_exponent = -exponent;
    } else {
        positive_exponent = exponent;
    }

    result = 1.0;
    if (positive_exponent != 0) {
        do {
            if ((positive_exponent & 1) != 0) {
                result *= base;
            }
            positive_exponent >>= 1;
            base *= base;
        } while (positive_exponent != 0);
    }
    if (exponent >= 0) {
        return result;
    }
    return 1.0 / result;
}

f64 Power(f64 arg0, f64 arg1) {
    if ((arg1 <= 2147483647.0) && (-2147483647.0 <= arg1)) {
        if (arg1 == (s32) arg1) {
            return Ipower(arg0, arg1);
        }
    }
    if (arg0 > 0.0) {
        return Lexp(Llog(arg0) * arg1);
    }
    return 0.0;
}

f64 Llog(f64 arg0) {
    s32 var_v0;
    s32 sp38;
    f64 var_f2;
    f64 temp_f0;
    f64 temp_f12;

    if (arg0 <= 0.0) {
        return 0.0;
    }
    Ffrexp(arg0 / 1.414213562373095, &sp38);
    arg0 /= Lldexp(1.0, sp38);
    var_v0 = 1;
    arg0 = (arg0 - 1.0) / (arg0 + 1.0);
    temp_f12 = arg0 * arg0;
    var_f2 = arg0;
    do {
        var_v0 += 2;
        arg0 *= temp_f12;
        temp_f0 = var_f2;
        var_f2 += arg0 / (f64) var_v0;
    } while (temp_f0 != var_f2);
    return ((f64) sp38 * 0.6931471805599453) + (2 * var_f2);
}

f64 Lexp(f64 arg0) {
    s32 temp_f10;
    f64 temp_f2;
    f64 temp_f0;
    s32 i;

    temp_f10 = ((arg0 >= 0.0) ? 0.5 : -0.5) + (arg0 / 0.6931471805599453);
    arg0 -= (temp_f10 * 0.6931471805599453);
    temp_f2 = SQ(arg0);

    temp_f0 = temp_f2 / 22;
    for (i = 0; i < 4; i++) {
        temp_f0 = temp_f2 / ((18 - 4 * i) + temp_f0);
    }

    temp_f2 = 2 + temp_f0;

    return Lldexp((temp_f2 + arg0) / (temp_f2 - arg0), temp_f10);
}

/**
 * This function appears to multiply some `value`
 * by 2 ^ `exponent`, even if that exponent is negative
 **/
f64 Lldexp(f64 value, s32 exponent) {
    f64 base;

    if (exponent >= 0) {
        base = 2.0;
    } else {
        exponent = -exponent;
        base = 0.5;
    }

    if (exponent != 0) {
        do {
            if ((exponent & 1) != 0) {
                value *= base;
            }
            exponent >>= 1;
            base *= base;
        } while (exponent != 0);
    }
    return value;
}

/**
 * Its unclear what exact purpose this function serves
 * The function multiplies/divides arg0 by 2 until its in
 * the range (0.5, 1.0].
 * arg2 appears to track the exponent in the power-of-2
 * that would undo the changes to arg0
 **/
f64 Ffrexp(f64 arg0, s32* arg2) {
    const f64 const1 = 2.0;
    s32 exponent = 0;

    while ((arg0 < 0.5) || (arg0 >= 1.0)) {
        if (arg0 < 0.5) {
            arg0 *= const1;
            exponent -= 1;
        } else {
            arg0 /= const1;
            exponent += 1;
        }
    }
    *arg2 = exponent;
    return arg0;
}

void SwapUint(s32* arg0, s32* arg1) {
    s32 temp;

    temp = *arg0;
    *arg0 = *arg1;
    *arg1 = temp;
}

extern s8 first_time_only_flg;

void InitMKCode(void) {
    s32 why = 0;
    s32 i;

    if (first_time_only_flg) {
        first_time_only_flg = why;
        vs_result_cursor = 10;
        ta_result_cursor = 5;
        bt_result_cursor = 10;
        if (osEepromProbe(&gSIEventMesgQueue) != 0) {
            LoadFunc800B4A10Default();
        }
        if (CheckContPackMenu() != 0) {
            KBGNumber = CONTROLLER_PAK_MENU;
        }
    }
    if (KBGNumber == LOGO_INTRO_MENU) {
        FreeMemoryPointer = StaticMemoryPointer;
        SetSegment(6, load_press_data((u8*) STARTUP_LOGO_ROM_START, (u8*) STARTUP_LOGO_ROM_END));
    }
    FreeMemoryPointer = StaticMemoryPointer;
    // Hypothetically, this should be a ptr... But only hypothetically.
    tex_buff = AllocateMemory(0x000900B0);
    melt_buff = AllocateMemory(0x0000CE00);
    k_work = (u8*) AllocateMemory(SCREEN_WIDTH * SCREEN_HEIGHT);
    kdl_buff = (struct_8018EE10_entry*) AllocateMemory(0x00001000);
    alloc_flagobj();
    fb_clear_flg = 0;

    for (i = 0; i < 5; i++) {
        g_FadingFlag[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        thunder_flg[i] = 0;
    }

    D_800DC5EC->posx = 160;
    D_800DC5EC->posy = 120;
    D_800DC5EC->width = SCREEN_WIDTH;
    D_800DC5EC->height = SCREEN_HEIGHT;
    KBGChange = FADE_MODE_MAIN;
    TexDataChange();

    for (i = 0; i < 4; i++) {
        Na_PlyEng_Stop((u8) i);
    }

    for (i = 1; i < 8; i++) {
        Na_EnmLevel_Stop((u8) i);
    }

    Setting_SoundOutMode();
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
}

s32 CheckContPackMenu(void) {
    u8 sp67;
    UNUSED s32 pad[10];
    s32 i;

    if (!gControllerBits) {
        return 0;
    }
    if ((!gControllerStatuses[0].status) & CONT_CARD_ON) {
        return 0;
    }
    if (!(gControllerOne->ButtonHeld & START_BUTTON)) {
        return 0;
    }
    osPfsIsPlug(&gSIEventMesgQueue, &sp67);
    if (sp67 & 1) {
        if (osPfsInit(&gSIEventMesgQueue, &gControllerPak1FileHandle, 0)) {
            return 0;
        }
    } else {
        return 0;
    }

    if (osPfsNumFiles(&gControllerPak1FileHandle, &gControllerPak1NumFilesUsed, &gControllerPak1MaxWriteableFiles)) {
        return 0;
    }

    for (i = 0; i < 16; i++) {
        pfsError[i] = osPfsFileState(&gControllerPak1FileHandle, i, pfsState + i);
    }

    if (osPfsFreeBlocks(&gControllerPak1FileHandle, &gControllerPak1NumPagesFree)) {
        return 0;
    }
    gControllerPak1NumPagesFree >>= 8;
    return 1;
}

void InitGhostData(void) {
    s32 temp_s0;
    s32 temp_s2;
    s32 tmp;

    gControllerPak1State = BAD;
    tmp = initPak();

    if ((g_InGameTT != 0) && (tmp == 0)) {
        temp_s2 = (g_cupSelect * 4) + g_courseSelect;
        LoadGameNoteKart_data();

        for (temp_s0 = 0; temp_s0 < 2; ++temp_s0) {
            if ((D_8018EE10[temp_s0].ghostDataSaved != 0) && (temp_s2 == D_8018EE10[temp_s0].courseIndex)) {
                LoadGameNoteKart_ghost(temp_s0);
                temp_s0 = 2;
                g_InGameTT = 0;
            }
        }
    }
}

void init_kawano_game(void) {
    s32 i;

    //! @todo These sizes need to be sizeof() for shiftability if possible
    melt_buff = AllocateMemory(0x00002800);
    tex_buff = (u16*) AllocateMemory(0x000124F8);
    k_work = (u8*) AllocateMemory(0x00001000);
    additional_point = AllocateMemory(4);

    for (i = 0; i < 5; i++) {
        g_FadingFlag[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        thunder_flg[i] = 0;
    }

    InitTexBuff();
    InitSeqData();
    InitObjBlock();
    InitVC_kawano();
    SetObjBlock(MENU_ITEM_TYPE_096, 0x00000064, 0x00000024, MENU_ITEM_PRIORITY_1);
    SetObjBlock(MENU_ITEM_TYPE_097, 0x00000064, 0x000000DD, MENU_ITEM_PRIORITY_1);
    SetObjBlock(MENU_ITEM_TYPE_098, 0, 0, MENU_ITEM_PRIORITY_0);
    SetObjBlock(MENU_ITEM_PAUSE, 0, 0, MENU_ITEM_PRIORITY_0);
    if (g_gameMode == TIME_TRIALS) {
        SetObjBlock(MENU_ITEM_TYPE_0BE, 0, 0, MENU_ITEM_PRIORITY_0);
        SetObjBlock(MENU_ITEM_TYPE_10E, 0, 0, MENU_ITEM_PRIORITY_0);
    }
    if ((replay_flag != 0) && (g_gameMode == TIME_TRIALS)) {
        SetObjBlock(MENU_ITEM_END_COURSE_OPTION, 0, 0, MENU_ITEM_PRIORITY_0);
    }
    if (!(gControllerBits & 1) && (demo_play_flag != 0)) {
        SetObjBlock(MENU_ITEM_UI_NO_CONTROLLER, 0, 0, MENU_ITEM_PRIORITY_2);
    }
    initPak();
}

void SkipManager(void) {
    struct Controller* controller = gControllerFive;
    s32 isHeld;

    isHeld = controller->ButtonHeld ? 1 : 0;

    if (controller->ButtonPressed) {
        k_skip_status = 1;
    }
    if ((isHeld) && (k_skip_status)) {
        k_skip_flg = 1;
        return;
    }
    k_skip_flg = 0;
}

void SkipReset(void) {
    k_skip_status = 0;
}

// if selected and detect kind menu
void SetCursorMessageColor(s32 test, s32 target, s32 alternative_color) {
    if (test == target) {
        if (FadeChecker() != 0) {
            set_text_color(frame_counter % 3);
        } else {
            set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        }
    } else {
        set_text_color(alternative_color);
    }
}

void SetCursorMessageColor4Option(s32 test, s32 target, s32 alternative_color) {
    if (test == target) {
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
    } else {
        set_text_color(alternative_color);
    }
}

void InitVC_kawano(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        vc_color_num[i] = i;
        vc_color_step[i] = 0;
    }
}

void VCAutoChanger(s32 arg0, s32* arg1, s32* arg2) {
    s32 temp_v1;
    s32 i;
    s32 idx;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t8_2;
    s32 temp_t9;
    UNUSED s32 temp_t0;
    s32 a, b, c, d;
    Vtx* vtx;

    if ((arg0 < 4) || (arg0 >= 6)) {
        return;
    }

    idx = (((arg0 * 4) + ((frame_counter % 2) * 2)) - 6);

    *arg2 += 16;
    if (*arg2 >= 256) {
        *arg2 = 0;
        *arg1 = (s32) (*arg1 + 1) % 3;
    }

    for (i = 0; i < 3; i++) {
        vtx = (Vtx*) GetVAddress(D_800E84C0[i]);

        temp_v1 = (*arg1 * 2) + 2;

        temp_t6 = (vtx + temp_v1)->v.cn[0] * (256 - *arg2);
        temp_t9 = (vtx + temp_v1)->v.cn[1] * (256 - *arg2);
        temp_t7 = (vtx + temp_v1)->v.cn[2] * (256 - *arg2);
        temp_t8_2 = (vtx + temp_v1)->v.cn[3] * (256 - *arg2);

        temp_v1 = (((*arg1 * 2) + 2) % 6) + 2;
        a = ((vtx + temp_v1)->v.cn[0] * *arg2);
        b = ((vtx + temp_v1)->v.cn[1] * *arg2);
        c = ((vtx + temp_v1)->v.cn[2] * *arg2);
        d = ((vtx + temp_v1)->v.cn[3] * *arg2);

        (vtx + idx)->v.cn[0] = (temp_t6 + a) / 256;
        (vtx + idx)->v.cn[1] = (temp_t9 + b) / 256;
        (vtx + idx)->v.cn[2] = (temp_t7 + c) / 256;
        (vtx + idx)->v.cn[3] = (temp_t8_2 + d) / 256;

        (vtx + idx + 1)->v.cn[0] = (temp_t6 + a) / 256;
        (vtx + idx + 1)->v.cn[1] = (temp_t9 + b) / 256;
        (vtx + idx + 1)->v.cn[2] = (temp_t7 + c) / 256;
        (vtx + idx + 1)->v.cn[3] = (temp_t8_2 + d) / 256;
    }
}

void DispResultTime(void) {

    switch (g_gameMode) {
        case GRAND_PRIX:
            SetObjBlock(MENU_ITEM_TYPE_0AA, 0, 0, MENU_ITEM_PRIORITY_0);
            break;
        case TIME_TRIALS:
            SetObjBlock(MENU_ITEM_TYPE_0B9, 0, 0, MENU_ITEM_PRIORITY_0);
            break;
    }
}

void DispGPLose(void) {
    SetObjBlock(MENU_ITEM_TYPE_0AC, 0, 0, MENU_ITEM_PRIORITY_0);
    kwgoal_rank_scroll();
    oga_gp_rose_ErhnamDjinn();
}

void DispInfo_3P(void) {
    SetObjBlock(MENU_ITEM_TYPE_0AF, 0, 0, MENU_ITEM_PRIORITY_0);
}

void EraseInfo_3P(void) {
    ObjBlock* temp = Get_OBJBlock_ptr(MENU_ITEM_TYPE_0AF);
    if (temp->pattern == 2) {
        temp->pattern = 3;
    }
}

void DispVSResult(void) {
    SetObjBlock(MENU_ITEM_TYPE_0B0, 0, 0, MENU_ITEM_PRIORITY_0);
}

void DispMessageRomGhost(void) {
    SetObjBlock(MENU_ITEM_ANNOUNCE_GHOST, 0, 0, MENU_ITEM_PRIORITY_0);
}

void StartKawanoResult(void) {
    SetObjBlock(MENU_ITEM_TYPE_12B, 0, 0, MENU_ITEM_PRIORITY_2);
}

void StartKawanoEnding(void) {
    switch (D_800DC5E4) {
        case 0:
            SetObjBlock(MENU_ITEM_TYPE_1CE, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        default:
            SetObjBlock(MENU_ITEM_TYPE_190, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_191, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 2:
            SetObjBlock(MENU_ITEM_TYPE_192, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_193, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 3:
            SetObjBlock(MENU_ITEM_TYPE_194, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_195, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 4:
            SetObjBlock(MENU_ITEM_TYPE_196, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_197, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 5:
            SetObjBlock(MENU_ITEM_TYPE_198, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_199, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_19A, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_19B, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 6:
            SetObjBlock(MENU_ITEM_TYPE_198, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_19C, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_19D, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_19E, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 7:
            SetObjBlock(MENU_ITEM_TYPE_19F, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A0, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A1, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A2, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 8:
            SetObjBlock(MENU_ITEM_TYPE_1A3, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A4, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 9:
            SetObjBlock(MENU_ITEM_TYPE_1A5, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A6, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A7, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A8, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A9, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 10:
            SetObjBlock(MENU_ITEM_TYPE_1A5, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1A6, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1AA, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1AB, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1AC, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 11:
            SetObjBlock(MENU_ITEM_TYPE_1AD, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1AE, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1AF, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1B0, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 12:
            SetObjBlock(MENU_ITEM_TYPE_1B1, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1B2, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 13:
            SetObjBlock(MENU_ITEM_TYPE_1B3, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1B4, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1B5, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 14:
            SetObjBlock(MENU_ITEM_TYPE_1B6, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1B7, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1B8, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1B9, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1BA, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1BB, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1BC, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 15:
            SetObjBlock(MENU_ITEM_TYPE_1BD, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1BE, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1BF, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C0, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 16:
            SetObjBlock(MENU_ITEM_TYPE_1C1, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C2, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C3, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 17:
            SetObjBlock(MENU_ITEM_TYPE_1C4, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C5, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C6, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C7, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C8, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1C9, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1CA, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1CB, 0, 0, MENU_ITEM_PRIORITY_2);
            SetObjBlock(MENU_ITEM_TYPE_1CC, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
        case 18:
            SetObjBlock(MENU_ITEM_TYPE_1CD, 0, 0, MENU_ITEM_PRIORITY_2);
            return;
    }
}

void StaffRoll_kawano1(void) {
    ending_message_flg = 1;
}

// Originally func_80092C90
// Some kind of lookup function, seems to return an index to be used
// to get a character's width in pixels
s32 GetFontNumber(char* character) {
    s32 index;
    s8 temp_v0;

    temp_v0 = *character;
    index = 1;
    if ((temp_v0 >= 'a') && (temp_v0 <= 'z')) {
        index = temp_v0 - 0x61;
    } else if ((temp_v0 >= 'A') && (temp_v0 <= 'Z')) {
        index = temp_v0 - 0x41;
    } else if ((temp_v0 >= '0') && (temp_v0 <= '9')) {
        index = temp_v0 - 0x10;
    } else if (temp_v0 == ' ') {
        index = -1;
    } else if (temp_v0 < 0) {
        // Handling EUC-JUP characters
        switch (temp_v0) { /* irregular */
            case -92:      // 0xA4
                index = GetFontNumber_h(character + 1);
                break;
            case -91: // 0xA5
                index = GetFontNumber_k(character + 1);
                break;
            case -95: // 0xA1
            case -93: // 0xA3
            case -85: // 0xAB
                index = GetFontNumber_j(character);
                break;
        }
    } else {
        switch (temp_v0) {
            case '!':
                index = 0x0000001A;
                break;
            case '-':
                index = 0x0000001B;
                break;
            case '?':
                index = 0x0000001C;
                break;
            case '\'':
                index = 0x0000001D;
                break;
            case '$':
                index = 0x0000001E;
                break;
            case '.':
                index = 0x0000001F;
                break;
            case '\"':
                index = 0x0000002A;
                break;
            case '*':
                index = 0x0000002B;
                break;
            case '+':
                index = 0x0000002C;
                break;
            // Displayed as "cc"
            case '(':
                index = 0x0000002D;
                break;
            case ',':
                index = 0x0000002E;
                break;
            default:
                index = -2;
                break;
        }
    }
    return index;
}

s32 GetFontNumber_k(char* arg) {
    return GetFontNumber_h(arg) + 0x50;
}

s32 GetFontNumber_h(char* character) {
    s32 ret;
    UNUSED s32 test;
    u8 temp_t6;
    u8 temp;

    temp_t6 = (*character + 0x80);
    // Huh?
    temp = temp_t6;
    if (temp) {}
    if ((temp > 0x20) && (temp < 0x2B)) {
        if (temp % 2) {
            ret = ((temp - 0x21) / 2) + 0x7B;
        } else {
            ret = ((temp - 0x21) / 2) + 0x30;
        }
    } else if ((temp > 0x2A) && (temp < 0x6E)) {
        ret = temp + 0xA;
    } else {
        switch (temp) { /* irregular */
            case 0x6F:
                ret = 0x00000078;
                break;
            case 0x72:
            case 0x73:
                ret = temp + 7;
                break;
            default:
                ret = -2;
                break;
        }
    }
    return ret;
}

s32 GetFontNumber_j(char* character) {
    u8 temp_t6;
    s32 var_v1;

    temp_t6 = (character[1] + 0x80);
    var_v1 = 2;
    switch (character[0]) {
        case -95:
            switch (temp_t6) {
                case 0x22:
                case 0x24:
                    var_v1 = 0x000000EA;
                    break;
                case 0x23:
                    var_v1 = 0x000000E9;
                    break;
                case 0x25:
                    var_v1 = 0x000000D0;
                    break;
                case 0x2A:
                    var_v1 = 0x000000E8;
                    break;
                case 0x30:
                    var_v1 = 0x000000EB;
                    break;
                case 0x47:
                    var_v1 = 0x000000D1;
                    break;
                case 0x49:
                    var_v1 = 0x000000D2;
                    break;
                case 0x5C:
                    var_v1 = 0x000000D3;
                    break;
                case 0x3C:
                case 0x3D:
                case 0x5D:
                    var_v1 = 0x000000D4;
                    break;
                default:
                    break;
            }
            break;
        case -93: /* switch 3 */
            if ((temp_t6 >= 0x30) && (temp_t6 < 0x3A)) {
                var_v1 = temp_t6 + 0xA5;
            } else {
                switch (temp_t6) { /* switch 2 */
                    case 0x44:     /* switch 2 */
                        var_v1 = 0x000000DF;
                        break;
                    case 0x43: /* switch 2 */
                    case 0x63: /* switch 2 */
                        var_v1 = 0x000000E0;
                        break;
                    case 0x4E: /* switch 2 */
                    case 0x6E: /* switch 2 */
                        var_v1 = 0x000000E1;
                        break;
                    case 0x50: /* switch 2 */
                    case 0x70: /* switch 2 */
                        var_v1 = 0x000000E2;
                        break;
                    case 0x52: /* switch 2 */
                    case 0x72: /* switch 2 */
                        var_v1 = 0x000000E3;
                        break;
                    case 0x73: /* switch 2 */
                        var_v1 = 0x000000E4;
                        break;
                    case 0x54: /* switch 2 */
                    case 0x74: /* switch 2 */
                        var_v1 = 0x000000E5;
                        break;
                    case 0x53: /* switch 2 */
                        var_v1 = 0x000000E6;
                        break;
                    case 0x56: /* switch 2 */
                    case 0x76: /* switch 2 */
                        var_v1 = 0x000000E7;
                        break;
                    default: /* switch 2 */
                        break;
                }
            }
            break;
        case -85: /* switch 3 */
            if (temp_t6 == 0x2E) {
                var_v1 = 0x000000E0;
            }
            break;
        default: /* switch 3 */
            var_v1 = 2;
    }
    return var_v1;
}

// Originally func_80093034
s32 GetWordLength(char* buffer) {
    s32 glyphIndex;
    s32 stringWidth = 0;

    if (*buffer != 0) {
        do {
            glyphIndex = GetFontNumber(buffer);
            if (glyphIndex >= 0) {
                stringWidth += font_width[glyphIndex];
            } else if (glyphIndex == -1) {
                stringWidth += 7;
            }
            if (glyphIndex >= 0x30) {
                buffer += 2;
            } else {
                buffer += 1;
            }
        } while (*buffer != 0);
    }
    return stringWidth;
}

void set_text_color(s32 arg0) {
    afont_color = arg0;
}

UNUSED void SetWord(s32 arg0, s32 arg1, char* arg2) {
    set_text_color(TEXT_BLUE);
    SetWord2A(arg0, arg1, arg2, 0, 1.0, 1.0);
}

// "tracking" is a uniform spacing between all characters in a given word
void SetWord2(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 mode) {
    s32 stringWidth = 0;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, D_020077A8);
    if (*text != 0) {
        do {
            glyphIndex = GetFontNumber(text);
            if (glyphIndex >= 0) {
                TexBuffLoad((MenuTexture*) GetTBPointer((const void*) gGlyphTextureLUT[glyphIndex]));
                gDisplayListHead =
                    doTexList4(gDisplayListHead,
                                 (MenuTexture*) GetTBPointer((const void*) gGlyphTextureLUT[glyphIndex]),
                                 column + (stringWidth * scaleX), row, mode, scaleX, scaleY);
                stringWidth += font_width[glyphIndex] + tracking;
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                stringWidth += tracking + 7;
            } else {
                gSPDisplayList(gDisplayListHead++, D_020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }
        } while (*text != 0);
    }
    gSPDisplayList(gDisplayListHead++, D_020077D8);
}

void SetWord2A(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord2(column, row, text, tracking, scaleX, scaleY, 1);
}

void SetWord2AP(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord2(column, row, text, tracking, scaleX, scaleY, 2);
}

// "tracking" is a uniform spacing between all characters in a given word
void SetWord3(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    char* temp_string = text;
    s32 stringWidth = 0;
    s32 glyphIndex;
    s32 sp60;

    while (*temp_string != 0) {
        glyphIndex = GetFontNumber(temp_string);
        if (glyphIndex >= 0) {
            stringWidth += ((font_width[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            stringWidth += ((tracking + 7) * scaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            temp_string += 2;
        } else {
            temp_string += 1;
        }
    }

    switch (arg6) {
        case LEFT_TEXT:
            // ???
            do {
            } while (0);
        case RIGHT_TEXT:
            column -= stringWidth;
            break;
        case CENTER_TEXT_MODE_1:
        case CENTER_TEXT_MODE_2:
            column -= stringWidth / 2;
            break;
        default:
            break;
    }

    if (arg6 < 3) {
        sp60 = 1;
    } else {
        sp60 = 2;
    }

    gSPDisplayList(gDisplayListHead++, D_020077A8);
    while (*text != 0) {
        glyphIndex = GetFontNumber(text);
        if (glyphIndex >= 0) {
            TexBuffLoad(GetTBPointer(gGlyphTextureLUT[glyphIndex]));
            gDisplayListHead = doTexList4(gDisplayListHead, GetTBPointer(gGlyphTextureLUT[glyphIndex]),
                                            column, row, sp60, scaleX, scaleY);
            column = column + (s32) ((font_width[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * scaleX);
        } else {
            gSPDisplayList(gDisplayListHead++, D_020077D8);
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }
    }
    gSPDisplayList(gDisplayListHead++, D_020077D8);
}

void SetWord3A(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, LEFT_TEXT);
}

void print_text1_center_mode_1(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, CENTER_TEXT_MODE_1);
}

void SetWord3AP(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, RIGHT_TEXT);
}

void SetWord3ACP(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, CENTER_TEXT_MODE_2);
}

void print_text2(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    MenuTexture* glyphTexture;
    s32 characterWidth;
    s32 glyphIndex;

    gSPDisplayList(gDisplayListHead++, D_020077A8);
    if (*text != 0) {
        do {
            glyphIndex = GetFontNumber(text);
            if (glyphIndex >= 0) {
                glyphTexture = (MenuTexture*) GetTBPointer((const void*) gGlyphTextureLUT[glyphIndex]);
                TexBuffLoad(glyphTexture);
                gDisplayListHead =
                    doTexList4(gDisplayListHead, glyphTexture, column - (font_width[glyphIndex] / 2), row,
                                 arg6, scaleX, scaleY);
                if ((glyphIndex >= 0xD5) && (glyphIndex < 0xE0)) {
                    characterWidth = 0x20;
                } else {
                    characterWidth = 0xC;
                }
                column = column + (s32) ((characterWidth + tracking) * scaleX);
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                column = column + (s32) ((tracking + 7) * scaleX);
            } else {
                gSPDisplayList(gDisplayListHead++, D_020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }
        } while (*text != 0);
    }

    gSPDisplayList(gDisplayListHead++, D_020077D8);
}

void SetWord4A(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text2(column, row, text, tracking, scaleX, scaleY, 1);
}

void text_draw(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    print_text2(column, row, text, tracking, scaleX, scaleY, 2);
}

void kawanoDrawAfterSky(s32 arg0) {
    ThunderMain(screen_mode2num[arg0]);
}

void kawanoDraw2(u32 arg0) {
    if (g_ScreenCounter == 0) {
        BeforeFadeMain();
    }
    switch (arg0) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            kawanoDraw((s32) screen_mode2num[arg0]);
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            if (g_ScreenCounter == 0) {
                kawanoOnlyFade((s32) screen_mode2num[arg0]);
            } else {
                kawanoDraw((s32) screen_mode2num[arg0]);
            }
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            if (g_ScreenCounter == 3) {
                kawanoDraw((s32) screen_mode2num[arg0]);
            } else {
                kawanoOnlyFade((s32) screen_mode2num[arg0]);
            }
            break;
    }
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
}

UNUSED void kawanoDraw2_backup(u32 arg0) {
    if ((arg0 == 0) || (arg0 == 2) || (arg0 == 3) || (arg0 == 8)) {
        BeforeFadeMain();
    }
    switch (arg0) {
        case 0:
            kawanoDraw(0);
            break;
        case 2:
        case 3:
        case 8:
            kawanoOnlyFade(0);
            break;
        case 1:
        case 4:
        case 9:
            kawanoDraw(1);
            break;
        case 10:
            kawanoOnlyFade(2);
            break;
        case 11:
            kawanoDraw(3);
            break;
        default:
            break;
    }
}

void kawanoOnlyFade(s32 arg0) {
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    FadeMain2(arg0);
    gSPDisplayList(gDisplayListHead++, D_02007F48);
}

UNUSED void Randomf(void) {
    return;
}

UNUSED void testprog(void) {
    return;
}

void kawanoDrawFinal_(s32 arg0) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&gDynamicP->mtxEffect[effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    doOBJBlock2();
    if (arg0 == 0) {
        DispOB_pause_cursor();
        FadeMain2(4);
        D_80165754 = effectcount;
        effectcount = 0;
    }
}

void KawanoDrawFinal(void) {
    kawanoDrawFinal_(0);
}

void kawanoDrawFinal4select(void) {
    kawanoDrawFinal_(1);
}

void init_kawanoResult(void) {
    s32 i;

    melt_buff = AllocateMemory(0x00002800);
    tex_buff = (u16*) AllocateMemory(0x000124F8);
    k_work = AllocateMemory(0x00001000);
    additional_point = AllocateMemory(4U);

    for (i = 0; i < 5; i++) {
        g_FadingFlag[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        thunder_flg[i] = 0;
    }

    InitTexBuff();
    InitSeqData();
    InitObjBlock();
    InitVC_kawano();
    ending_message_flg = 0;
}

void kawanoDrawResult(void) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&gDynamicP->mtxEffect[effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, 320, 240);
    VCAutoChanger(4, vc_color_num, vc_color_step);
    VCAutoChanger(5, (s32*) &vc_color_num[1], (s32*) &vc_color_step[1]);
    BeforeFadeMain();
    InitChangeTexData();
    InitChangeTexData_vs();
    doOBJBlock1();
    doChangeTexData();
    doChangeTexData_vs();
    FadeMain();
    gSPDisplayList(gDisplayListHead++, D_02007F48);
    effectcount = 0;
}

void kawanoDraw(s32 arg0) {
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    guOrtho(&gDynamicP->mtxEffect[effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007F18);
    VCAutoChanger(4, vc_color_num, vc_color_step);
    VCAutoChanger(5, (s32*) &vc_color_num[1], (s32*) &vc_color_step[1]);
    SkipManager();
    InitChangeTexData();
    InitChangeTexData_vs();
    doOBJBlock1();
    doChangeTexData();
    doChangeTexData_vs();
    FadeMain2(arg0);
    gSPDisplayList(gDisplayListHead++, D_02007F48);
    KWReturnViewport();
}

void logo_proc(void) {
    Mtx* test;
    f32 var_f26;
    s32 var_s2;
    s32 thing;
    test = &gDynamicP->mtxObject[0];
    gSPMatrix(gDisplayListHead++, &gDynamicP->mtxScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, &gDynamicP->mtxLookAt[0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    guRotate(test, logo_rot_x, 1.0f, 0.0f, 0.0f);
    guRotate(test + 1, logo_rot_y, 0.0f, 1.0f, 0.0f);
    guScale(test + 2, 1.0f, 1.0f, logo_scale_z);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, 0x00);
    //gSPDisplayList(gDisplayListHead++, D_02007F60);
    gSPDisplayList(gDisplayListHead++, 0x080052A0);
    
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    if (logo_delay_size > 0) {
        var_f26 = logo_yang_speed;
        if (var_f26 > 10.0f) {
            var_f26 = 10.0f;
        }
        for (var_s2 = 0, thing = 0xC0; var_s2 < 0xC; var_s2++, thing -= 0x10) {
            guRotate(test, 0.0f, 1.0f, 0.0f, 0.0f);
            guRotate(test + 1, (var_s2 + 1) * logo_delay_size * var_f26, 0.0f, 1.0f, 0.0f);
            guScale(test + 2, 1.0f, 1.0f, 2.0f);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(gDisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
            gDPSetEnvColor(gDisplayListHead++, 0x00, 0x00, 0x00, thing);
            gSPDisplayList(gDisplayListHead++, startup_texture_dl4);
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}

void doNintenLogo(struct GfxPool* arg0, UNUSED s32 arg1) {
    u16 perspNorm;
    StoreSegments();
    gDPSetTexturePersp(gDisplayListHead++, G_TP_PERSP);
    guPerspective(&arg0->mtxScreen, &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    guLookAt(&arg0->mtxLookAt[0], 0.0f, 0.0f, (f32) logo_cam_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    logo_proc();
    gDPPipeSync(gDisplayListHead++);
    gDPSetTexturePersp(gDisplayListHead++, G_TP_NONE);
    gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
}

void doCheckerFlag(struct GfxPool* arg0, UNUSED s32 arg1) {
    u16 perspNorm;
    StoreSegments();
    guPerspective(&arg0->mtxPersp[0], &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(gDisplayListHead++, perspNorm);
    guLookAt(&arg0->mtxLookAt[1], 0.0f, 0.0f, (f32) logo_cam_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    guRotate(&arg0->mtxObject[0], logo_rot_x, 1.0f, 0, 0);
    guRotate(&arg0->mtxObject[1], logo_rot_y, 0, 1.0f, 0);
    guRotate(&arg0->mtxObject[2], logo_rot_z, 0, 0, 1.0f);
    guScale(&arg0->mtxObject[3], logo_scale_z, logo_scale_z, logo_scale_z);
    guTranslate(&arg0->mtxObject[4], logo_pos_x, logo_pos_y, logo_pos_z);
    gSPMatrix(gDisplayListHead++, &arg0->mtxPersp[0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gDisplayListHead++, &arg0->mtxLookAt[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[0], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[1], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[2], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[3], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(gDisplayListHead++, &arg0->mtxObject[4], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, D_02007FC8);
    draw_flag();
    gSPDisplayList(gDisplayListHead++, D_02007650);
}

void doGameSelect(struct GfxPool* pool) {
    modelingnum = 0;
    effectcount = 0;
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_802B8880));
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    guOrtho(&pool->mtxScreen, 0.0f, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, &pool->mtxScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(gDisplayListHead++, D_02007650);
    TexDataChange();
    VCAutoChanger(4, vc_color_num, vc_color_step);
    VCAutoChanger(5, &vc_color_num[1], &vc_color_step[1]);
    InitChangeTexData();
    BeforeFadeMain();
    switch (KBGNumber) {
        case START_MENU:
            doGameSelect10();
            kawanoDrawFinal4select();
            break;
        case OPTIONS_MENU:
        case DATA_MENU:
        case COURSE_DATA_MENU:
        case LOGO_INTRO_MENU:
        case CONTROLLER_PAK_MENU:
        case MAIN_MENU:
        case CHARACTER_SELECT_MENU:
        case COURSE_SELECT_MENU:
            doOBJBlock1();
            doChangeTexData();
            break;
    }
    titleMenuDefault();
    FadeMain();
    k_flash_counter += 1;
    gDPPipeSync(gDisplayListHead++);
    gSPDisplayList(gDisplayListHead++, D_020076B0);
}

void TexDataChange(void) {
    if (KBGChange != FADE_MODE_NONE) {
        InitTexBuff();
        InitSeqData();
        InitObjBlock();
        InitKDL();
        InitVC_kawano();
        initPak();
        initPak2();
        k_var_init(KBGNumber);
        switch (KBGNumber) {
            case OPTIONS_MENU:
                SetObjBlock(MAIN_MENU_BACKGROUND, 0, 0, MENU_ITEM_PRIORITY_2);
                SetObjBlock(MENU_ITEM_TYPE_0F1, 0, 0, MENU_ITEM_PRIORITY_4);
                SetObjBlock(MENU_ITEM_TYPE_0F0, 0, 0, MENU_ITEM_PRIORITY_2);
                break;
            case DATA_MENU:
                SetObjBlock(MAIN_MENU_BACKGROUND, 0, 0, MENU_ITEM_PRIORITY_2);
                break;
            case COURSE_DATA_MENU:
                SetObjBlock(MENU_ITEM_DATA_COURSE_IMAGE, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(MENU_ITEM_DATA_COURSE_INFO, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(MENU_ITEM_DATA_COURSE_SELECTABLE, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(MENU_ITEM_TYPE_0E9, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(MENU_ITEM_TYPE_0EA, 0, 0, MENU_ITEM_PRIORITY_8);
                break;
            case LOGO_INTRO_MENU:
                SetObjBlock(MENU_ITEM_UI_LOGO_INTRO, 0, 0, MENU_ITEM_PRIORITY_0);
                break;
            case CONTROLLER_PAK_MENU:
                SetObjBlock(MENU_ITEM_TYPE_0DA, 0, 0, MENU_ITEM_PRIORITY_0);
                SetObjBlock(MENU_ITEM_TYPE_0D2, 0, 0, MENU_ITEM_PRIORITY_4);
                SetObjBlock(MENU_ITEM_TYPE_0D4, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_TYPE_0D3, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(MENU_ITEM_TYPE_0D5, 0, 0, MENU_ITEM_PRIORITY_A);
                SetObjBlock(MENU_ITEM_TYPE_0D6, 0, 0, MENU_ITEM_PRIORITY_A);
                SetObjBlock(MENU_ITEM_TYPE_0D7, 0, 0, MENU_ITEM_PRIORITY_A);
                SetObjBlock(MENU_ITEM_TYPE_0D8, 0, 0, MENU_ITEM_PRIORITY_A);
                SetObjBlock(MENU_ITEM_TYPE_0D9, 0, 0, MENU_ITEM_PRIORITY_A);
                break;
            case START_MENU:
                SetObjBlock(MENU_ITEM_UI_LOGO_AND_COPYRIGHT, 0, 0, MENU_ITEM_PRIORITY_4);
                SetObjBlock(MENU_ITEM_UI_START_BACKGROUND, 0, 0, MENU_ITEM_PRIORITY_0);
#if !OverKartBuild
                SetObjBlock(START_MENU_FLAG, 0, 0, MENU_ITEM_PRIORITY_0);
#endif
                if (gControllerBits & 1) {
                    SetObjBlock(MENU_ITEM_UI_PUSH_START_BUTTON, 0, 0, MENU_ITEM_PRIORITY_2);
                } else {
                    SetObjBlock(MENU_ITEM_UI_NO_CONTROLLER, 0, 0, MENU_ITEM_PRIORITY_2);
                }
                g_DemoFlag = 0;
                demo_play_flag = 0;
                break;
            case MAIN_MENU:
                SetObjBlock(MAIN_MENU_BACKGROUND, 0, 0, MENU_ITEM_PRIORITY_2);
                SetObjBlock(MENU_ITEM_UI_GAME_SELECT, 0x0000015E, 0x00000011, MENU_ITEM_PRIORITY_1);
                SetObjBlock(MENU_ITEM_UI_4P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_3P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_2P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_1P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_OK, 0x0000015E, 0x000000C8, MENU_ITEM_PRIORITY_6);
                if (Check4GoldCup_150cc() != 0) {
                    SetObjBlock(MAIN_MENU_EXTRA_CC, 0, 0, MENU_ITEM_PRIORITY_6);
                }
                SetObjBlock(MAIN_MENU_150CC, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MAIN_MENU_100CC, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MAIN_MENU_50CC, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MAIN_MENU_TIME_TRIALS_DATA, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MAIN_MENU_TIME_TRIALS_BEGIN, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_TYPE_01B, 0, 0, MENU_ITEM_PRIORITY_C);
                break;
            case CHARACTER_SELECT_MENU:
                SetObjBlock(CHARACTER_SELECT_BACKGROUND, 0, 0, MENU_ITEM_PRIORITY_2);
                SetObjBlock(CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER, 0, 0, MENU_ITEM_PRIORITY_1);
                SetObjBlock(CHARACTER_SELECT_MENU_OK, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(CHARACTER_SELECT_MENU_MARIO, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(CHARACTER_SELECT_MENU_LUIGI, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(CHARACTER_SELECT_MENU_TOAD, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(CHARACTER_SELECT_MENU_PEACH, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(CHARACTER_SELECT_MENU_YOSHI, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(CHARACTER_SELECT_MENU_DK, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(CHARACTER_SELECT_MENU_WARIO, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(CHARACTER_SELECT_MENU_BOWSER, 0, 0, MENU_ITEM_PRIORITY_8);
                SetObjBlock(CHARACTER_SELECT_MENU_1P_CURSOR, 0, 0, MENU_ITEM_PRIORITY_C);
                SetObjBlock(CHARACTER_SELECT_MENU_2P_CURSOR, 0, 0, MENU_ITEM_PRIORITY_C);
                SetObjBlock(CHARACTER_SELECT_MENU_3P_CURSOR, 0, 0, MENU_ITEM_PRIORITY_C);
                SetObjBlock(CHARACTER_SELECT_MENU_4P_CURSOR, 0, 0, MENU_ITEM_PRIORITY_C);
                break;
            case COURSE_SELECT_MENU:
                SetObjBlock(COURSE_SELECT_BACKGROUND, 0, 0, MENU_ITEM_PRIORITY_2);
                SetObjBlock(COURSE_SELECT_MAP_SELECT, 0, 0, MENU_ITEM_PRIORITY_1);
                if (g_gameMode != BATTLE) {
                    SetObjBlock(COURSE_SELECT_MUSHROOM_CUP, 0, 0, MENU_ITEM_PRIORITY_4);
                    SetObjBlock(COURSE_SELECT_FLOWER_CUP, 0, 0, MENU_ITEM_PRIORITY_4);
                    SetObjBlock(COURSE_SELECT_STAR_CUP, 0, 0, MENU_ITEM_PRIORITY_4);
                    SetObjBlock(COURSE_SELECT_SPECIAL_CUP, 0, 0, MENU_ITEM_PRIORITY_4);
                    SetObjBlock(MENU_ITEM_TYPE_058, 0, 0, MENU_ITEM_PRIORITY_6);
                    SetObjBlock(COURSE_SELECT_COURSE_NAMES, 0, 0, MENU_ITEM_PRIORITY_6);
                    SetObjBlock(MENU_ITEM_TYPE_05A, 0, 0, MENU_ITEM_PRIORITY_6);
                    SetObjBlock(MENU_ITEM_TYPE_05B, 0, 0, MENU_ITEM_PRIORITY_6);
                } else {
                    SetObjBlock(COURSE_SELECT_BATTLE_NAMES, 0, 0, MENU_ITEM_PRIORITY_6);
                    SetObjBlock(MENU_ITEM_TYPE_06E, 0, 0, MENU_ITEM_PRIORITY_6);
                }
                SetObjBlock(MENU_ITEM_TYPE_064, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_TYPE_05F, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_TYPE_060, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_TYPE_061, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_TYPE_062, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(COURSE_SELECT_OK, 0, 0, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_TYPE_05E, 0, 0, MENU_ITEM_PRIORITY_8);
                if (g_gameMode == TIME_TRIALS) {
                    SetObjBlock(MENU_ITEM_TYPE_065, 0, 0, MENU_ITEM_PRIORITY_1);
                    SetObjBlock(MENU_ITEM_TYPE_066, 0, 0, MENU_ITEM_PRIORITY_1);
                    SetObjBlock(MENU_ITEM_TYPE_069, 0, 0, MENU_ITEM_PRIORITY_1);
                }
                if (g_gameMode == GRAND_PRIX) {
                    SetObjBlock(MENU_ITEM_TYPE_068, 0, 0, MENU_ITEM_PRIORITY_1);
                    SetObjBlock(MENU_ITEM_TYPE_067, 0, 0, MENU_ITEM_PRIORITY_5);
                }
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            default:
                break;
        }
        if (KBGChange != FADE_MODE_LOGO) {
            SetFadeIn(0x00000014);
        } else {
            SetFadeInWhite(0x00000014);
        }
        KBGChange = FADE_MODE_NONE;
    }
}

void doGameSelect10(void) {
    s32 var_v0;

    if ((fb_clear_flg < 3) || (g_FadingFlag[4] != 0)) {
        doOBJBlock1();
    }
    if (title_mode > DEBUG_MENU_DISABLED) { // If not disabled
        loadFont();
        printString(0x00000050, 0x00000064, "debug_mode");
        switch (title_mode) {
            case DEBUG_MENU_DEBUG_MODE:
                printString(0x00000046, 0x00000064, "*");
                break;
            case DEBUG_MENU_COURSE:
                printString(0x00000046, 0x0000006E, "*");
                break;
            case DEBUG_MENU_SCREEN_MODE:
                printString(0x00000046, 0x00000078, "*");
                break;
            case DEBUG_MENU_PLAYER:
                printString(0x00000046, 0x00000082, "*");
                break;
            case DEBUG_MENU_SOUND_MODE:
                printString(0x00000046, 0x0000008C, "*");
                break;
            case DEBUG_MENU_GIVE_ALL_GOLD_CUP:
                printString(0x00000046, 0x00000096, "*");
                break;
        }
        if (g_DebugMode) {
            printString(0x000000AA, 0x00000064, "on");
        } else {
            printString(0x000000AA, 0x00000064, "off");
        }
        if ((g_courseID >= (NUM_COURSES - 1)) || (g_courseID < 0)) {
            g_courseID = 0;
        }
        printStringNumber(0x00000050, 0x0000006E, "map_number", g_courseID);
        if (g_courseID < 0xA) {
            var_v0 = 0;
        } else {
            var_v0 = 8;
        }
        printString(var_v0 + 0xB9, 0x0000006E, GET_COURSE_debugName);
        printString(0x00000050, 0x00000078, "screen_mode");
        printString(0x000000AA, 0x00000078, screen_mode_names[gScreenModeListIndex]);
        printString(0x00000050, 0x00000082, "player");
        printString(0x000000AA, 0x00000082, drivers_name[g_CharacterSelections[0]]);
        printString(0x00000050, 0x0000008C, "sound mode");
        printString(0x000000AA, 0x0000008C, sound_mode_name[sound_output]);
        if (title_mode == DEBUG_MENU_GIVE_ALL_GOLD_CUP) {
            printString(0x00000050, 0x00000096, "push b to get all goldcup");
        }
        kwprintmode_finish();
    }
    if (title_mode == DEBUG_MENU_DISABLED) {
        titleDemo += 1;
    } else {
        titleDemo = 3;
    }
    if (titleDemo == 2) {
        playSound(SOUND_INTRO_WELCOME);
    }
    /**
     * @bug Pressing START in the last moments before the demo timeout plays the
     * menu-enter chime but then drops into the demo with no music. The attract
     * timer fires while the press-start fade is still in flight and overwrites
     * the fade target, so the fade completes as a demo entry instead of the
     * main menu. Roughly a 0.7 second window every attract cycle.
     */
#ifdef AVOID_UB
    if ((titleDemo > 300) && (FadeChecker() == 0)) {
#else
    if (titleDemo > 300) {
#endif
        SetFadeOut_d();
        Na_SeMute_Title();
    }
    gSPDisplayList(gDisplayListHead++, D_020076E0);
}

// While this matches, its a little screwy
// This function seemingly needs to return a Gfx*, but doing that explicity doesn't match
// Instead we depend on the fact that the result of FillRect1Color is left
// in v0 which means it is returned, sort of.
// Its also weird that the displayListHead argument goes entirely unused. What's up with that?
Gfx* FlashRectangle2_(UNUSED Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 speed) {
    s32 greyscale;

    greyscale = ((k_flash_counter % speed) << 9) / speed;
    if (greyscale > 0x100) {
        greyscale = 0x200 - greyscale;
    }

    if (greyscale > 0xFF) { // set max greyscale to 0xFF
        greyscale = 0xFF;
    }

#if AVOID_UB
    return gDisplayListHead =
               FillRect1Color(gDisplayListHead, ulx, uly, lrx, lry, greyscale, greyscale, greyscale, 0xFF);
#else
    gDisplayListHead = FillRect1Color(gDisplayListHead, ulx, uly, lrx, lry, greyscale, greyscale, greyscale, 0xFF);
#endif
}

Gfx* FlashRectangle2(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return FlashRectangle2_(displayListHead, ulx, uly, lrx, lry, 64);
}

Gfx* FlashRectangle2f(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return FlashRectangle2_(displayListHead, ulx, uly, lrx, lry, 4);
}

Gfx* SetAfontVertex(Gfx* displayListHead, Vtx* arg1) {
    s32 index;

    if ((s32) afont_color < TEXT_BLUE_GREEN_RED_CYCLE_1) {
        index = afont_color;
    } else {
        index = ((afont_color * 2) + ((s32) frame_counter % 2)) - 4;
    }
#ifdef AVOID_UB
    gSPVertex(displayListHead++, arg1, 2, 0);
    gSPVertex(displayListHead++, &arg1[(index + 1) * 2], 2, 2);
    gSPDisplayList(displayListHead++, common_rectangle_display);
#else
    if (arg1 == D_02007BB8) {
        gSPDisplayList(displayListHead++, D_800E84CC[index]);
    } else if (arg1 == D_02007CD8) {
        gSPDisplayList(displayListHead++, D_800E84EC[index]);
    } else if (arg1 == D_02007DF8) {
        gSPDisplayList(displayListHead++, D_800E850C[index]);
    }
#endif

    return displayListHead;
}

#ifdef AVOID_UB
#define MTX_TYPE Mtx
#else
typedef struct {
    u16 i[4][4];
    u16 f[4][4];
} Mtx_u;

typedef union {
    Mtx_u u;
    Mtx_t m;
    long long int force_structure_alignment;
} Mtx2;

typedef union {
    s16 s[2];
    s32 w;
} TheWhyUnion;

#define MTX_TYPE Mtx2
#endif

// Why... Why... Why... This function is so bad it's not going in the header.
void setmtx_scaletrans(MTX_TYPE* arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
#ifdef AVOID_UB
    // Use Mat4 array to set matrix values using guMtxF2L. This helps little-endian systems.
    Mat4 src;
    src[0][0] = arg3;
    src[0][1] = 0.0f;
    src[0][2] = 0.0f;
    src[0][3] = 0.0f;
    src[1][0] = 0.0f;
    src[1][1] = arg4;
    src[1][2] = 0.0f;
    src[1][3] = 0.0f;
    src[2][0] = 0.0f;
    src[2][1] = 0.0f;
    src[2][2] = 1.0f;
    src[2][3] = 0.0f;
    src[3][0] = arg1;
    src[3][1] = arg2;
    src[3][2] = 0.0f;
    src[3][3] = 1.0f;
    guMtxF2L(src, arg0);
#else
    TheWhyUnion sp14;
    TheWhyUnion sp10;
    TheWhyUnion spC;
    TheWhyUnion sp8;
    s32 i;

    // clang-format off
    // should be inline
    for(i = 0; i < 16; i++) { arg0->m[0][i] = 0; }
    // clang-format on

    sp14.w = arg3 * 65536.0f;
    sp10.w = arg4 * 65536.0f;
    spC.w = arg1 * 65536.0f;
    sp8.w = arg2 * 65536.0f;
    arg0->u.i[0][0] = sp14.s[0];
    arg0->u.i[1][1] = sp10.s[0];
    arg0->u.i[2][2] = 1;
    arg0->u.i[3][0] = spC.s[0];
    arg0->u.i[3][1] = sp8.s[0];
    arg0->u.i[3][3] = 1;
    arg0->u.f[0][0] = sp14.s[1];
    arg0->u.f[1][1] = sp10.s[1];
    arg0->u.f[3][0] = spC.s[1];
    arg0->u.f[3][1] = sp8.s[1];
#endif
}

#undef MTX_TYPE

Gfx* Set_IAFont(Gfx* displayListHead, u8* arg1, f32 arg2, f32 arg3, u32 arg4, u32 arg5, f32 arg6, f32 arg7) {
    Vtx* var_a1;
    Mtx* sp28;

    // A match is a match, but why are goto's required here?
    if (effectcount >= 0x2F7) {
        goto func_80095BD0_label1;
    }
    sp28 = &gDynamicP->mtxEffect[effectcount];
    if (effectcount < 0) {
        rmonPrintf("effectcount < 0 !!!!!!(kawano)\n");
    }
    goto func_80095BD0_label2;
func_80095BD0_label1:
    rmonPrintf("MAX effectcount(760) over!!!!(kawano)\n");
    return displayListHead;
func_80095BD0_label2:
    setmtx_scaletrans((void*) sp28, arg2, arg3, arg6, arg7);
    gSPMatrix(displayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    effectcount += 1;
    gDPLoadTextureTile_4b(displayListHead++, arg1, G_IM_FMT_I, arg4, 0, 0, 0, arg4, arg5, 0, G_TX_NOMIRROR | G_TX_WRAP,
                          G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    switch (arg4) {
        default:
            var_a1 = D_02007CD8;
            break;
        case 16:
            var_a1 = D_02007CD8;
            break;
        case 26:
            var_a1 = D_02007BB8;
            break;
        case 30:
            var_a1 = D_02007DF8;
            break;
    }

    return SetAfontVertex(displayListHead, var_a1);
}

Gfx* AutoTextureRect(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, u8* argA, u32 argB, u32 argC) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > SCREEN_WIDTH) {
        arg6 = (arg4 - arg8) + SCREEN_WIDTH;
    }

    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > SCREEN_HEIGHT) {
        arg7 = (arg5 - arg9) + SCREEN_HEIGHT;
    }

    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {

        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }

        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {

            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadTextureTile(displayListHead++, argA, arg1, G_IM_SIZ_16b, argB, 0, var_a1_2, var_s3,
                               var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, arg8 * 4, arg9 * 4, (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}

Gfx* AutoTextureRect_zoom(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 argA, s32 argB, u8* argC, u32 argD, u32 argE) {
    u32 var_a1_2 = arg6;
    u32 var_s3 = arg7;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    u32 a;
    u32 b;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;

    while ((u32) var_t0 < argD) {
        var_t0 *= 2;
    }

    temp_lo = 0x400 / var_t0;

    while ((u32) (temp_lo / 2) > argE) {
        temp_lo /= 2;
    }

    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;

    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }

    if (argA < 0) {
        arg6 -= argA;
        argA = 0;
    } else if ((argA + (arg8 - arg6) * arg4) > SCREEN_WIDTH) {
        arg8 -= argA + (arg8 - arg6) * arg4 - SCREEN_WIDTH;
    }

    if (argB < 0) {
        arg7 -= argB;
        argB = 0;
    } else if ((argB + (arg9 - arg7) * arg5) > SCREEN_HEIGHT) {
        arg9 -= argB + (arg9 - arg7) * arg5 - SCREEN_HEIGHT;
    }

    if (arg8 < arg6) {
        return displayListHead;
    }
    if (arg9 < arg7) {
        return displayListHead;
    }
    arg2 /= arg4;
    arg3 /= arg5;

    sp7C = argA;
    for (var_s3 = arg7; var_s3 < (u32) arg9; var_s3 += temp_lo) {

        if ((u32) arg9 < temp_lo + var_s3) {
            var_s4 = arg9 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }
        b = var_s4 * arg5;
        for (var_a1_2 = arg6; var_a1_2 < (u32) arg8; var_a1_2 += var_t0) {

            if ((u32) arg8 < (var_t0 + var_a1_2)) {
                var_s2 = arg8 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            a = var_s2 * arg4;

            gDPLoadTextureTile(displayListHead++, argC, arg1, G_IM_SIZ_16b, argD, argE, var_a1_2, var_s3,
                               var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, argA * 4, argB * 4, (argA + a) * 4, (argB + b) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);

            argA += var_t0 * arg4;
        }

        argA = sp7C;
        argB += temp_lo * arg5;
    }
    return displayListHead;
}

extern u8 D_0B002A00[];
/**
 *
 * This function is responsible for drawing a near unnoticeable static pattern
 * over the course images when loading the cup selection screen.
 * It may or may not use gTextureTitleChocoMountain to create that pattern.
 * Try locking the word at `8018DC80` to see something like 0x20 just before confirming character selection to make it
 * last longer
 */
Gfx* AutoTextureRect_noise(Gfx* displayListHead, s32 xPos, s32 yPos, u32 width, u32 height) {
    u32 x;
    u32 y;
    UNUSED s32 pad;
    u32 rectXoffset;
    u32 rectYoffset;
    s32 tileWidth = 1;
    s32 tileHeight;
    s32 masks = 0;
    s32 maskt = 0;
    s32 rnd;

    while ((u32) tileWidth < width) {
        tileWidth *= 2;
    }

    tileHeight = 1024 / tileWidth;
    while ((u32) (tileHeight / 2) > height) {
        tileHeight /= 2;
    }

    rnd = tileWidth;
    while (rnd > 1) {
        rnd /= 2;
        masks += 1;
    }
    rnd = tileHeight;
    while (rnd > 1) {
        rnd /= 2;
        maskt += 1;
    }

    if (xPos < 0) {
        width -= xPos;
        xPos = 0;
    } else if ((xPos + width) > SCREEN_WIDTH) {
        width = SCREEN_WIDTH - xPos;
    }
    if (yPos < 0) {
        height -= yPos;
        yPos = 0;
    } else if ((yPos + height) > SCREEN_HEIGHT) {
        height = SCREEN_HEIGHT - yPos;
    }

    if (width == 0) {
        return displayListHead;
    }
    if (height == 0) {
        return displayListHead;
    }

    rnd = MakeRandomLimmit(100);
    displayListHead = FillRect1ColorF(displayListHead, xPos, yPos, xPos + width, yPos + height, 0, 0, 0, rnd);
    rnd += 150;

    gDPPipeSync(displayListHead++);
    gDPSetRenderMode(displayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(displayListHead++, 0, 0, rnd, rnd, rnd, rnd);
    gDPSetCombineMode(displayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    for (y = yPos; y < (yPos + height); y += tileHeight) {
        if ((y + tileHeight) > (yPos + height)) {
            rectYoffset = yPos + height - y;
            if (rectYoffset == 0) {
                break;
            }
        } else {
            rectYoffset = tileHeight;
        }
        for (x = xPos; x < xPos + width; x += tileWidth) {
            if (x + tileWidth > xPos + width) {
                rectXoffset = xPos + width - x;
                if (rectXoffset == 0) {
                    break;
                }
            } else
                rectXoffset = tileWidth;

            gDPLoadTextureTile(displayListHead++, (D_0B002A00 + MakeRandomLimmit(128) * 2), G_IM_FMT_IA, G_IM_SIZ_16b, width,
                               height, x, y, x + rectXoffset, y + rectYoffset, 0, G_TX_WRAP, G_TX_WRAP, masks, maskt,
                               G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, x << 2, y << 2, (x + rectXoffset) << 2, (y + rectYoffset) << 2,
                                G_TX_RENDERTILE, (x * 32) & 0xFFFF, (y * 32) & 0xFFFF, 1024, 1024);
        }
    }

    return displayListHead;
}

Gfx* AutoTextureRect_2cyc(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, UNUSED u16* argA, u32 argB, u32 argC, UNUSED s32 argD) {
    u32 var_a1_2 = arg4;
    u32 var_s3 = arg5;
    s32 sp7C;
    u32 var_s2;
    u32 var_s4;
    s32 var_t0 = 1;
    s32 temp_lo;
    s32 sp68 = 0;
    s32 sp64 = 0;
    s32 var_v0_2;
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_2CYCLE);
    gDPSetTextureLOD(displayListHead++, G_TL_TILE);
    gDPSetPrimColor(displayListHead++, 0, 0, 0, 0, 0, frame_counter % 256);
    gDPSetCombineLERP(displayListHead++, TEXEL1, TEXEL0, PRIMITIVE_ALPHA, TEXEL0, TEXEL1, TEXEL0, PRIMITIVE, TEXEL0, 0,
                      0, 0, COMBINED, 0, 0, 0, COMBINED);
    while ((u32) var_t0 < argB) {
        var_t0 *= 2;
    }
    temp_lo = 0x400 / var_t0;
    while ((u32) (temp_lo / 2) > argC) {
        temp_lo /= 2;
    }
    var_v0_2 = var_t0;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp68 += 1;
    }
    var_v0_2 = temp_lo;
    while (var_v0_2 > 1) {
        var_v0_2 /= 2;
        sp64 += 1;
    }
    if (arg8 < 0) {
        arg4 -= arg8;
        arg8 = 0;
    } else if (((arg6 - arg4) + arg8) > 320) {
        arg6 = (arg4 - arg8) + 320;
    }
    if (arg9 < 0) {
        arg5 -= arg9;
        arg9 = 0;
    } else if (((arg7 - arg5) + arg9) > 240) {
        arg7 = (arg5 - arg9) + 240;
    }
    if (arg6 < arg4) {
        return displayListHead;
    }
    if (arg7 < arg5) {
        return displayListHead;
    }
    sp7C = arg8;
    for (var_s3 = arg5; var_s3 < (u32) arg7; var_s3 += temp_lo) {
        if ((u32) arg7 < temp_lo + var_s3) {
            var_s4 = arg7 - var_s3;
            if (!var_s4) {
                break;
            }
        } else {
            var_s4 = temp_lo;
        }
        for (var_a1_2 = arg4; var_a1_2 < (u32) arg6; var_a1_2 += var_t0) {
            if ((u32) arg6 < var_t0 + var_a1_2) {
                var_s2 = arg6 - var_a1_2;
                if (!var_s2) {
                    break;
                }
            } else {
                var_s2 = var_t0;
            }
            gDPLoadMultiTile(displayListHead++, argA, 0, G_TX_RENDERTILE, arg1, G_IM_SIZ_16b, argB, argC, var_a1_2,
                             var_s3, var_a1_2 + var_s2, var_s3 + var_s4, 0, G_TX_WRAP, G_TX_WRAP, sp68, sp64,
                             G_TX_NOLOD, G_TX_NOLOD);
            gDPLoadMultiTile(displayListHead++, D_0B002A00 + MakeRandomLimmit(128) * 2, 256, G_TX_RENDERTILE + 1, arg1,
                             G_IM_SIZ_16b, argB, argC, var_a1_2, var_s3, var_a1_2 + var_s2, var_s3 + var_s4, 0,
                             G_TX_WRAP, G_TX_WRAP, sp68, sp64, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, arg8 * 4, arg9 * 4, (arg8 + var_s2) * 4, (arg9 + var_s4) * 4, 0,
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, arg2, arg3);
            arg8 += var_t0;
        }
        arg8 = sp7C;
        arg9 += temp_lo;
    }
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}

Gfx* AutoTextureRect_c(Gfx* displayListHead, s8 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8* arg8,
                   u32 arg9, u32 argA) {
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_COPY);
    displayListHead = AutoTextureRect(displayListHead, arg1, 0x00001000, 0x00000400, arg2, arg3, arg4, arg5, arg6, arg7,
                                    arg8, arg9, argA);
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}

Gfx* AutoTextureRect64(Gfx* displayListHead, s8 fmt, s32 arg2, s32 arg3, u8* arg4, s32 width) {
    s32 i;
    s32 temp;
    s32 arg2Copy;
    s32 dsdx;

    if (width >= 32) {
        return displayListHead;
    }

    arg2Copy = arg2;

    for (i = 0; i < 64; i += 32) {
        temp = 0;
        dsdx = 0x8000 / (32 - width);
        gDPLoadTextureTile(displayListHead++, arg4, fmt, G_IM_SIZ_16b, 64, 64, temp, i, temp + 32, i + 32, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (arg2 + width) << 2, arg3 << 2, (arg2 + 32) << 2, (arg3 + 32) << 2, 0, 0,
                            0, dsdx, 1024);

        arg2 += 32;

        gDPLoadTextureTile(displayListHead++, arg4, fmt, G_IM_SIZ_16b, 64, 64, temp + 32, i, temp + 64, i + 32, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, arg2 << 2, arg3 << 2, ((arg2 - width) + 32) << 2, (arg3 + 32) << 2, 0, 0,
                            0, dsdx, 1024);

        arg2 = arg2Copy;
        arg3 += 32;
    }
    return displayListHead;
}

Gfx* AutoTextureRect_w(Gfx* displayListHead, s8 fmt, UNUSED u32 arg2, u32 arg3, UNUSED u32 arg4, u32 arg5, s32 arg6,
                   s32 arg7, u8* someTexture, u32 arg9, UNUSED u32 argA, s32 width) {
    u32 ult;
    u32 temp;
    s32 arg6Copy;
    s32 temp_v1;
    s32 var_s2;
    s32 lrs;
    s32 spDC;
    s32 temp2 = 32;

    if (width >= 32) {
        return displayListHead;
    }

    arg6Copy = arg6;

    lrs = arg9 / 2;
    spDC = arg9 - lrs;
    for (ult = arg3; ult < arg5; ult += 32) {
        temp = 0;
        if ((ult + temp2) > arg5) {
            var_s2 = arg5 - ult;
            if (!var_s2) {
                break;
            }
        } else {
            var_s2 = temp2;
        }
        temp_v1 = ((32 * lrs) << 10) / (lrs * (32 - width));

        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp, ult, temp + lrs,
                           ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (arg6 + lrs * width / 32) << 2, arg7 << 2, (arg6 + lrs) << 2,
                            (arg7 + var_s2) << 2, 0, 0, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 += lrs;
        temp_v1 = ((32 * spDC) << 10) / (spDC * (32 - width));
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp + lrs, ult, temp + arg9,
                           ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, arg6 << 2, arg7 << 2, (arg6 + spDC * (32 - width) / 32) << 2,
                            (arg7 + var_s2) << 2, 0, (lrs << 5) & 0xFFFF, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 = arg6Copy;
        arg7 += temp2;
    }
    return displayListHead;
}

Gfx* AutoTextureRectSimple(Gfx* displayListHead, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, UNUSED s32 arg7,
                   s32 arg8) {
    u32 var_a3;
    u32 var_v0;
    s32 arg5Copy;

    arg5Copy = arg5;
    for (var_v0 = arg2; var_v0 < arg4; var_v0 += 0x20) {
        for (var_a3 = arg1; var_a3 < arg3; var_a3 += 0x20) {
            gDPLoadTextureTile(displayListHead++, tex_buff, G_IM_FMT_RGBA, G_IM_SIZ_16b, arg8, 0, var_a3,
                               var_v0, var_a3 + 0x20, var_v0 + 0x20, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, arg5 << 2, arg6 << 2, (arg5 + 0x20) << 2, (arg6 + 0x20) << 2, 0, 0,
                                0, 1024, 1024);
            arg5 += 0x20;
        }
        arg5 = arg5Copy;
        arg6 += 0x20;
    }
    return displayListHead;
}

Gfx* AutoTextureRectR(Gfx* displayListHead, u32 arg1, u32 arg2, u32 width, u32 height, s32 column, s32 row,
                   UNUSED u8* arg7, u32 textureWidth, UNUSED s32 textureHeight) {
    s32 var_a2;
    s32 var_v0_2;
    s32 columnCopy;
    s32 temp_f4_2;
    UNUSED s32 stackPadding0;
    s32 temp_f6;
    f32 temp_f0;
    f32 temp_f18;
    f32 temp_f24;

    if (g_FadeCounter[0] == 0) {
        g_FadeCounter[0] = 1;
    }
    temp_f24 = sinT(((g_FadeCounter2[0] * 0x4E20) / g_FadeCounter[0]) % 20000U);
    temp_f0 = cosT(((g_FadeCounter2[0] * 0x4E20) / g_FadeCounter[0]) % 20000U);
    temp_f18 = (((f32) g_FadeCounter2[0] * 0.5) / g_FadeCounter[0]) + 1.0;
    columnCopy = column;
    for (var_v0_2 = arg2; (u32) var_v0_2 < height; var_v0_2 += 0x20) {
        for (var_a2 = arg1; (u32) var_a2 < width; var_a2 += 0x20) {
            gDPLoadTextureTile(displayListHead++, tex_buff, G_IM_FMT_RGBA, G_IM_SIZ_16b, textureWidth, 0,
                               var_a2, var_v0_2, var_a2 + 0x20, var_v0_2 + 0x20, 0, G_TX_NOMIRROR | G_TX_WRAP,
                               G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
            temp_f6 = (temp_f18 * ((temp_f0 * (column - 0xA0)) + (temp_f24 * (row - 0x78)))) + 160.0f;
            temp_f4_2 = (temp_f18 * ((-temp_f24 * (column - 0xA0)) + (temp_f0 * (row - 0x78)))) + 120.0f;
            gSPTextureRectangle(displayListHead++, temp_f6 * 4, temp_f4_2 * 4, ((temp_f6 + 0x20) ^ 0) * 4,
                                ((temp_f4_2 + 0x20) ^ 0) * 4, 0, 0, 0, 1024, 1024);
            column += 0x20;
        }
        column = columnCopy;
        row += 0x20;
    }
    return displayListHead;
}

// draw a box filled with a solid color
Gfx* FillRect1Color(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 red, s32 green, s32 blue, s32 alpha) {
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
    alpha &= 0xFF;
    if (lrx < ulx) {
        SwapUint(&ulx, &lrx);
    }
    if (lry < uly) {
        SwapUint(&uly, &lry);
    }
    if ((ulx >= 0x140) || (uly >= 0xF0)) {
        return displayListHead;
    }
    if (ulx < 0) {
        ulx = 0;
    }
    if (uly < 0) {
        uly = 0;
    }
    if ((lrx < 0) || (lry < 0)) {
        return displayListHead;
    }
    if (lrx >= 0x140) {
        lrx = 0x13F;
    }
    if (lry >= 0xF0) {
        lry = 0xEF;
    }
    gSPDisplayList(displayListHead++, D_02008030);
    gDPSetFillColor(displayListHead++, (GPACK_RGBA5551(red, green, (u32) blue, alpha) << 0x10 |
                                        GPACK_RGBA5551(red, green, (u32) blue, alpha)));
    gDPFillRectangle(displayListHead++, ulx, uly, lrx, lry);
    gSPDisplayList(displayListHead++, D_02008058);
    return displayListHead;
}

// draw a box with a solid outline
Gfx* FillRect1ColorF(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, u32 red, u32 green, u32 blue, u32 alpha) {
    red &= 0xFF;
    green &= 0xFF;
    blue &= 0xFF;
    alpha &= 0xFF;
    if (lrx < ulx) {
        SwapUint(&ulx, &lrx);
    }
    if (lry < uly) {
        SwapUint(&uly, &lry);
    }
    if ((ulx >= 0x140) || (uly >= 0xF0)) {
        return displayListHead;
    }
    if (ulx < 0) {
        ulx = 0;
    }
    if (uly < 0) {
        uly = 0;
    }
    if ((lrx < 0) || (lry < 0)) {
        return displayListHead;
    }
    if (lrx >= 0x141) {
        lrx = 0x140;
    }
    if (lry >= 0xF1) {
        lry = 0xF0;
    }
    gSPDisplayList(displayListHead++, D_02008008);
    gDPSetPrimColor(displayListHead++, 0, 0, red, green, blue, alpha);
    gDPFillRectangle(displayListHead++, ulx, uly, lrx, lry);
    gDPPipeSync(displayListHead++);
    return displayListHead;
}

Gfx* FillRectBlack(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return FillRect1Color(displayListHead, ulx, uly, lrx, lry, 0, 0, 0, 0xFF);
}

void DMA_Base729A30(u64* data, size_t nbytes, void* vaddr) {
    OSIoMesg mb;
    OSMesg msg;

    osInvalDCache(vaddr, nbytes);
    osPiStartDma(&mb, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(data)],
                 vaddr, nbytes, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &msg, OS_MESG_BLOCK);
}

void tex_data_Dma_s(u64* data, size_t nbytes, void* vaddr) {
    OSIoMesg mb;
    OSMesg msg;

    osInvalDCache(vaddr, nbytes);
    osPiStartDma(&mb, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) &_textures_0bSegmentRomStart[SEGMENT_OFFSET(data)],
                 vaddr, nbytes, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &msg, OS_MESG_BLOCK);
}

void InitTexBuff(void) {
    tex_buff_num = 0;
    all_tex_block_num = 0;
}

/**
 * Differs from memory.c with `+ 0x8` instead of `| 0x8`
 *
 * @param addr
 * @return void*
 */
void* GetTBPointer(const void* addr) {
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    return (void*) ((SegmentTable[segment] + offset) + 0x80000000);
}

void* GetVAddress(const void* addr) {
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    return (void*) ((SegmentTable[segment] + offset) + 0x80000000);
}

void TexBuffLoad(MenuTexture* addr) {
    u16 size;
    s32 i;
    s32 imgLoaded;
    MenuTexture* texAddr;
    TextureMap* texMap = &sMenuTextureMap[0];

    texAddr = GetTBPointer(addr);
    while (texAddr->textureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < all_tex_block_num; i++) {
            if (texAddr->textureData == (texMap + i)->textureData) {
                imgLoaded = true;
                break;
            }
        }

        if (imgLoaded == false) {
            if (texAddr->type == 3) {
                if (texAddr->size != 0) {
                    size = texAddr->size;
                } else {
                    size = 0x1000;
                }
                if (size % 8) {
                    size = ((size / 8) * 8) + 8;
                }
                DMA_Base729A30(texAddr->textureData, size, melt_buff);
                decodeMIO0((u8*) melt_buff, (u8*) &tex_buff[tex_buff_num]);
            } else {
                DMA_Base729A30(texAddr->textureData, (texAddr->height * texAddr->width) * 2,
                                      &tex_buff[tex_buff_num]);
            }
            texMap[all_tex_block_num].textureData = texAddr->textureData;
            texMap[all_tex_block_num].offset = tex_buff_num;
            tex_buff_num += texAddr->height * texAddr->width;
            tex_buff_num = ((tex_buff_num / 8) * 8) + 8;
            all_tex_block_num += 1;
        }
        texAddr++;
    }
}

void TexBuffLoad_font(MenuTexture* addr) {
    s32 i;
    s32 imgLoaded;
    MenuTexture* texAddr;
    TextureMap* texMap = &sMenuTextureMap[0];

    texAddr = GetTBPointer(addr);
    while (texAddr->textureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < all_tex_block_num; i++) {
            if (texAddr->textureData == (texMap + i)->textureData) {
                imgLoaded = true;
                break;
            }
        }

        if (imgLoaded == false) {
            if (texAddr->type == 5) {
                DMA_Base729A30(texAddr->textureData, (u32) (((s32) (texAddr->height * texAddr->width)) / 2),
                                      &tex_buff[tex_buff_num]);
            }
            texMap[all_tex_block_num].textureData = texAddr->textureData;
            texMap[all_tex_block_num].offset = tex_buff_num;
            tex_buff_num += texAddr->height * texAddr->width;
            tex_buff_num = ((tex_buff_num / 8) * 8) + 8;
            all_tex_block_num += 1;
        }
        texAddr++;
    }
}

void TexBuffLoad_c(MenuTexture* addr) {
    s32 i;
    s32 imgLoaded;
    MenuTexture* texAddr;
    TextureMap* texMap = &sMenuTextureMap[0];

    texAddr = GetTBPointer(addr);
    while (texAddr->textureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < all_tex_block_num; i++) {
            if (texAddr->textureData == (texMap + i)->textureData) {
                imgLoaded = true;
                break;
            }
        }

        if (imgLoaded == false) {
            DMA_Base729A30(texAddr->textureData, 0x00008000U, melt_buff);
            decodeMIO0((u8*) melt_buff, (u8*) &tex_buff[tex_buff_num]);
            texMap[all_tex_block_num].textureData = texAddr->textureData;
            texMap[all_tex_block_num].offset = tex_buff_num;
            tex_buff_num += texAddr->height * texAddr->width;
            tex_buff_num = ((tex_buff_num / 8) * 8) + 8;
            all_tex_block_num += 1;
        }
        texAddr++;
    }
}

void TexBuffLoad_pw(MenuTexture* addr) {
    TexBuffLoadP(addr, LOAD_MENU_IMG_MIO0_FORCE);
}

void TexBuffLoadP(MenuTexture* addr, s32 compType) {
    u16 size;
    s32 i;
    s32 imgLoaded;
    u8 clearBit;
    MenuTexture* texAddr;
    TextureMap* texMap = &sMenuTextureMap[0];

    texAddr = GetTBPointer(addr);
    while (texAddr->textureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < all_tex_block_num; i++) {
            if (texAddr->textureData == (texMap + i)->textureData) {
                imgLoaded = true;
                break;
            }
        }

        if ((imgLoaded == false) || (compType > LOAD_MENU_IMG_FORCE)) {
            if (texAddr->size != 0) {
                size = texAddr->size;
            } else {
                size = 0x1000;
            }
            if (size % 8) {
                size = ((size / 8) * 8) + 8;
            }
            switch (compType) {
                case LOAD_MENU_IMG_MIO0_ONCE:
                case LOAD_MENU_IMG_MIO0_FORCE:
                    DMA_Base729A30(texAddr->textureData, size, melt_buff);
                    break;
                case LOAD_MENU_IMG_TKMK00_ONCE:
                case LOAD_MENU_IMG_TKMK00_FORCE:
                    tex_data_Dma_s(texAddr->textureData, size, melt_buff);
                    break;
            }

            switch (compType) {
                case LOAD_MENU_IMG_MIO0_ONCE:
                case LOAD_MENU_IMG_MIO0_FORCE:
                    decodeMIO0((u8*) melt_buff, (u8*) &tex_buff[tex_buff_num]);
                    break;
                case LOAD_MENU_IMG_TKMK00_ONCE:
                case LOAD_MENU_IMG_TKMK00_FORCE:
                    if (texAddr->type == 1) {
                        clearBit = 0xBE;
                    } else {
                        clearBit = 1;
                    }
                    if (1) {}
                    decodeTKMK(melt_buff, k_work,
                                 &tex_buff[tex_buff_num], clearBit);
                    break;
            }

            texMap[all_tex_block_num].textureData = texAddr->textureData;
            texMap[all_tex_block_num].offset = tex_buff_num;
            tex_buff_num += texAddr->height * texAddr->width;
            tex_buff_num = ((tex_buff_num / 8) * 8) + 8;
            all_tex_block_num += 1;
        }
        texAddr++;
    }
}

void TexBuffLoad_vs(MenuTexture* addr, s32 arg1, s32 arg2) {
    u16 size;
    MenuTexture* texAddr;

    texAddr = GetTBPointer(addr);
    while (texAddr->textureData != NULL) {
        if (texAddr->size != 0) {
            size = texAddr->size;
        } else {
            size = 0x1400;
        }
        if (size % 8) {
            // Round up to the next multiple of eight
            size = ((size / 8) * 8) + 8;
        }
        DMA_Base729A30(texAddr->textureData, size, melt_buff);
        decodeMIO0((u8*) melt_buff,
                   (u8*) D_802BFB80.arraySize4[arg2][arg1 / 2][(arg1 % 2) + 2].pixel_index_array);
        texAddr++;
    }
}

// Possibly a debug print function?
void InitChangeTexData(void) {
    s32 i;
    change_tex_buff_block[0].texture = NULL;
    for (i = 0; i < D_8018E060_SIZE; i++) {}
}

void SetChangeTexData(MenuTexture* arg0, s32 arg1) {
    struct_8018E060_entry* var_v1;

    var_v1 = &change_tex_buff_block[0];
    while (var_v1->texture != NULL) {
        var_v1++;
    }
    var_v1->texture = GetTBPointer(arg0);
    var_v1->texNum = arg1;
}

void doChangeTexData(void) {
    s32 some_var;
    s8 texEnd;
    struct_8018E060_entry* var_s1;
    TextureMap* entry;
    MenuTexture* texPtr;
    OSIoMesg mb;
    OSMesg sp64;
    s32 cacheSize;
    s32 bufSize;

    if (gGamestate == RACING) {
        bufSize = 0x500;
    } else {
        bufSize = 0x1000;
    }

    texEnd = 0;
    entry = &sMenuTextureMap[0];
    var_s1 = &change_tex_buff_block[0];
    texPtr = var_s1->texture;

    if (texPtr == NULL) {
        return;
    }

    if (texPtr->size) {
        cacheSize = texPtr->size;
    } else {
        cacheSize = 0x1400;
    }
    if (cacheSize % 8) {
        cacheSize = ((cacheSize / 8) * 8) + 8;
    }

    osInvalDCache(melt_buff, cacheSize);
    osPiStartDma(&mb, 0, 0, (uintptr_t) _textures_0aSegmentRomStart + SEGMENT_OFFSET(texPtr->textureData),
                 melt_buff, cacheSize, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &sp64, 1);

    while (1) {
        if ((var_s1 + 1)->texture == NULL) {
            texEnd += 1;
        } else {
            texPtr = (var_s1 + 1)->texture;
            if (texPtr->size) {
                cacheSize = texPtr->size;
            } else {
                cacheSize = 0x1400;
            }
            if (cacheSize % 8) {
                cacheSize = ((cacheSize / 8) * 8) + 8;
            }
            osInvalDCache(&melt_buff[bufSize], cacheSize);
            osPiStartDma(&mb, 0, 0, (uintptr_t) _textures_0aSegmentRomStart + SEGMENT_OFFSET(texPtr->textureData),
                         &melt_buff[bufSize], cacheSize, &gDmaMesgQueue);
        }

        some_var = (entry + var_s1->texNum)->offset;

        decodeMIO0((u8*) melt_buff, (u8*) &tex_buff[some_var]);

        var_s1->texture = NULL;
        var_s1++;
        if (texEnd) {
            break;
        }

        osRecvMesg(&gDmaMesgQueue, &sp64, 1);

        if ((var_s1 + 1)->texture == NULL) {
            texEnd += 1;
        } else {
            texPtr = (var_s1 + 1)->texture;
            if (texPtr->size) {
                cacheSize = texPtr->size;
            } else {
                cacheSize = 0x1400;
            }
            if (cacheSize % 8) {
                cacheSize = ((cacheSize / 8) * 8) + 8;
            }
            osInvalDCache(melt_buff, cacheSize);
            osPiStartDma(&mb, 0, 0, (uintptr_t) _textures_0aSegmentRomStart + SEGMENT_OFFSET(texPtr->textureData),
                         melt_buff, cacheSize, &gDmaMesgQueue);
        }

        some_var = (entry + var_s1->texNum)->offset;
        decodeMIO0((u8*) &melt_buff[bufSize], (u8*) &tex_buff[some_var]);
        var_s1->texture = NULL;
        var_s1++;
        if (texEnd) {
            break;
        }
        osRecvMesg(&gDmaMesgQueue, &sp64, 1);
    }
}

void InitChangeTexData_vs(void) {
    change_tex_buff_block_vs[0].mk64Texture = NULL;
}

void SetChangeTexData_vs(MenuTexture* arg0, s32 arg1, s32 arg2) {
    struct_8018E0E8_entry* var_v1;

    var_v1 = change_tex_buff_block_vs;
    while (var_v1->mk64Texture != NULL) {
        var_v1++;
    }
    var_v1->mk64Texture = GetTBPointer(arg0);
    var_v1->unk4 = arg1;
    var_v1->unk6 = arg2;
}

void doChangeTexData_vs(void) {
    s8 var_s4;
    s32 var_s0;
    UNUSED s32 pad[2];
    OSIoMesg sp68;
    OSMesg sp64;
    s32 huh;
    MenuTexture* temp_s2;
    struct_8018E0E8_entry* var_s1;

    var_s4 = 0;
    var_s1 = change_tex_buff_block_vs;
    temp_s2 = var_s1->mk64Texture;

    if (temp_s2 == NULL)
        return;

    huh = temp_s2->size;
    if (huh != 0) {
        var_s0 = huh;
    } else {
        var_s0 = 0x1400;
    }
    if (var_s0 % 8) {
        var_s0 = ((var_s0 / 8) * 8) + 8;
    }
    osInvalDCache((void*) melt_buff, var_s0);
    osPiStartDma(&sp68, 0, 0, (u32) _textures_0aSegmentRomStart + SEGMENT_OFFSET(temp_s2->textureData),
                 melt_buff, var_s0, &gDmaMesgQueue);
    if ((var_s0 && var_s0) && var_s0) {}
    osRecvMesg(&gDmaMesgQueue, &sp64, 1);
    while (1) {
        if ((var_s1 + 1)->mk64Texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->mk64Texture;
            huh = (var_s1 + 1)->mk64Texture->size;
            if (huh != 0) {
                var_s0 = huh;
            } else {
                var_s0 = 0x1400;
            }
            if (var_s0 % 8) {
                var_s0 = ((var_s0 / 8) * 8) + 8;
            }
            osInvalDCache(melt_buff + 0x500, var_s0);
            osPiStartDma(&sp68, 0, 0, (u32) _textures_0aSegmentRomStart + SEGMENT_OFFSET(temp_s2->textureData),
                         melt_buff + 0x500, var_s0, &gDmaMesgQueue);
        }
        decodeMIO0((u8*) melt_buff,
                   D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].pixel_index_array);
        var_s1->mk64Texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&gDmaMesgQueue, &sp64, 1);
        if ((var_s1 + 1)->mk64Texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->mk64Texture;
            huh = (var_s1 + 1)->mk64Texture->size;
            if (huh != 0) {
                var_s0 = huh;
            } else {
                var_s0 = 0x1400;
            }
            if (var_s0 % 8) {
                var_s0 = ((var_s0 / 8) * 8) + 8;
            }
            osInvalDCache(melt_buff, var_s0);
            osPiStartDma(&sp68, 0, 0, (u32) _textures_0aSegmentRomStart + SEGMENT_OFFSET(temp_s2->textureData),
                         melt_buff, var_s0, &gDmaMesgQueue);
        }
        decodeMIO0((u8*) (melt_buff + 0x500),
                   D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].pixel_index_array);
        var_s1->mk64Texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&gDmaMesgQueue, &sp64, 1);
    }
}

void ChangeTexData(MenuTexture* arg0, s32 arg1) {
    s32 var_a3;
    s32 temp_v1;
    u64* sp24;
    UNUSED TextureMap* temp_v0;

    temp_v1 = sMenuTextureMap[arg1].offset;
    sp24 = arg0->textureData;
    var_a3 = arg0->size;
    if (var_a3 % 8) {
        var_a3 = ((var_a3 / 8) * 8) + 8;
    }
    tex_data_Dma_s(sp24, var_a3, melt_buff);
    decodeTKMK(melt_buff, k_work, &tex_buff[temp_v1], 1);
    sMenuTextureMap[arg1].textureData = sp24;
}

void func_8009A2F0(struct_8018E0E8_entry* arg0) {
    MenuTexture* var_a0;
    struct_8018E0E8_entry* temp_v0;

    temp_v0 = GetVAddress(arg0);
    var_a0 = temp_v0->mk64Texture;
    while (var_a0 != NULL) {
        if (var_a0 == NULL) {
            break;
        }
        TexBuffLoadP(var_a0, LOAD_MENU_IMG_TKMK00_ONCE);
        if (1) {}
        temp_v0++;
        var_a0 = temp_v0->mk64Texture;
    }
}

void InitSeqData(void) {
    s32 index;
    for (index = 0; index < D_8018DEE0_SIZE; index++) {
        seq_data_block[index].visible = 0;
    }
}

s32 SetSeqData(MkAnimation* anim) {
    s32 i;
    struct_8018DEE0_entry* entry;

    anim = GetVAddress(anim);
    i = 0;
    while (seq_data_block[i].visible) {
        i++;
        if (i >= 0x10) {
            // No more space.
            while (1) {
                ;
            }
        }
    }

    entry = &seq_data_block[i];
    entry->textureSequence = anim;
    entry->sequenceIndex = -1;
    entry->frameCountDown = 0;
    entry->visible = 0x80000000;
    entry->menuTextureIndex = all_tex_block_num;

    if (anim[0].mk64Texture) {
        TexBuffLoad_pw(anim[0].mk64Texture);
    }
    if (anim[1].mk64Texture) {
        TexBuffLoad_pw(anim[1].mk64Texture);
    } else {
        TexBuffLoad_pw(anim[0].mk64Texture);
    }

    entry->unk14 = 0;
    return i;
}

s32 SetSeqData2(MkAnimation* anim, s32 arg1) {
    s32 i;
    struct_8018DEE0_entry* entry;

    anim = GetVAddress(anim);
    i = 0;
    while (seq_data_block[i].visible) {
        i++;
        if (i >= 0x10) {
            // No more space.
            while (1) {
                ;
            }
        }
    }

    entry = &seq_data_block[i];
    entry->textureSequence = anim;
    entry->sequenceIndex = -1;
    entry->frameCountDown = 0;
    entry->visible = 0x80000000;
    entry->menuTextureIndex = all_tex_block_num;
    if (anim[0].mk64Texture) {
        TexBuffLoad_vs(anim[0].mk64Texture, arg1, 0);
    }
    if (anim[1].mk64Texture) {
        TexBuffLoad_vs(anim[1].mk64Texture, arg1, 1);
    } else {
        TexBuffLoad_vs(anim[0].mk64Texture, arg1, 1);
    }
    entry->unk14 = 0;
    return i;
}

void ChangeSeqData(s32 arg0, s32 arg1, MkAnimation* arg2) {
    MkAnimation* temp_v0;
    MenuTexture* temp_a0;

    temp_v0 = GetVAddress(arg2);
    seq_data_block[arg0].textureSequence = temp_v0;
    seq_data_block[arg0].sequenceIndex = arg1;
    // All hail the fake match gods who, in their infinite grace, have blessed us
    // with this enigma of a match on the first iteration of permutation
    seq_data_block[arg0].frameCountDown = (temp_v0 + arg1)->frame_length;
    temp_a0 = GetTBPointer(temp_v0[arg1].mk64Texture);
    if (seq_data_block[arg0].unk14 != 0) {
        SetChangeTexData(temp_a0, seq_data_block[arg0].menuTextureIndex);
        seq_data_block[arg0].unk14 = 0;
    } else {
        SetChangeTexData(temp_a0, seq_data_block[arg0].menuTextureIndex + 1);
        seq_data_block[arg0].unk14 = 1;
    }
}

void ChangeSeqData_vs(s32 arg0, s32 arg1, s32 arg2, MkAnimation* arg3) {
    MkAnimation* temp_v0;
    MenuTexture* temp_a0;

    temp_v0 = GetVAddress(arg3);
    seq_data_block[arg0].textureSequence = temp_v0;
    seq_data_block[arg0].sequenceIndex = arg1;
    seq_data_block[arg0].frameCountDown = (temp_v0 + arg1)->frame_length;
    temp_a0 = GetTBPointer(temp_v0[arg1].mk64Texture);
    seq_data_block[arg0].unk14 ^= 1;
    SetChangeTexData_vs(temp_a0, arg2, seq_data_block[arg0].unk14);
}

UNUSED void doSeqData(void) {
    s32 index;
    for (index = 0; index < D_8018DEE0_SIZE; index++) {
        if ((seq_data_block[index].visible & 0x80000000) != 0) {
            SelectTex(&seq_data_block[index]);
            gDisplayListHead = doTexList_w(gDisplayListHead, &seq_data_block[index], 0, 0, 0);
        }
    }
    doChangeTexData();
}

void doSeqData2(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    struct_8018DEE0_entry* temp = &seq_data_block[arg0];
    if (temp->visible & 0x80000000) {
        SelectTex(temp);
        gDisplayListHead = doTexList_w(gDisplayListHead, temp, arg1, arg2, arg3);
    }
}

void doSeqData2_vs(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    struct_8018DEE0_entry* temp = &seq_data_block[arg0];
    if (temp->visible & 0x80000000) {
        SelectTex_vs(temp, arg3);
        gDisplayListHead = doTexList_vs(gDisplayListHead, temp, arg1, arg2, arg3, arg4);
    }
}

MenuTexture* SelectTex(struct_8018DEE0_entry* arg0) {
    MkAnimation* temp_v1;
    MkAnimation* var_v0;
    MkAnimation* test;
    MenuTexture* temp_a0;

    temp_v1 = arg0->textureSequence;
    if (arg0->sequenceIndex < 0) {
        arg0->sequenceIndex = 0;
        arg0->frameCountDown = 0;
    }
    arg0->frameCountDown--;
    if (arg0->frameCountDown <= 0) {
        arg0->sequenceIndex++;
        // Again, hail the fake match gods
        var_v0 = ((test = temp_v1) + arg0->sequenceIndex);
        if (var_v0->mk64Texture == NULL) {
            arg0->sequenceIndex = 0;
        }
        var_v0 = (test + arg0->sequenceIndex);
        arg0->frameCountDown = var_v0->frame_length;
        temp_a0 = GetTBPointer(var_v0->mk64Texture);
        if (arg0->unk14 != 0) {
            SetChangeTexData(temp_a0, arg0->menuTextureIndex);
            arg0->unk14 = 0;
        } else {
            SetChangeTexData(temp_a0, arg0->menuTextureIndex + 1);
            arg0->unk14 = 1;
        }
    }
    return arg0->textureSequence[arg0->sequenceIndex].mk64Texture;
}

MenuTexture* SelectTex_vs(struct_8018DEE0_entry* arg0, s32 arg1) {
    MkAnimation* temp_v1;
    MkAnimation* var_v0;
    MkAnimation* test;
    MenuTexture* temp_a0;

    temp_v1 = arg0->textureSequence;
    if (arg0->sequenceIndex < 0) {
        arg0->sequenceIndex = 0;
        arg0->frameCountDown = 0;
    }
    arg0->frameCountDown--;
    if (arg0->frameCountDown <= 0) {
        arg0->sequenceIndex++;
        var_v0 = ((test = temp_v1) + arg0->sequenceIndex);
        if (var_v0->mk64Texture == NULL) {
            arg0->sequenceIndex = 0;
        }
        var_v0 = (test + arg0->sequenceIndex);
        arg0->frameCountDown = var_v0->frame_length;
        temp_a0 = GetTBPointer(var_v0->mk64Texture);
        arg0->unk14 ^= 1;
        SetChangeTexData_vs(temp_a0, arg1, arg0->unk14);
    }
    return arg0->textureSequence[arg0->sequenceIndex].mk64Texture;
}

void GrayScale_tex_buff256(s32 arg0, s32 arg1, u32 arg2, s32 arg3) {
    s32 red;
    s32 green;
    s32 blue;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    s32 alpha;
    s32 temp_t9;
    u16 temp_a0;
    s32 var_t1;
    u16* color0;
    u16* color1;

    color0 = &tex_buff[sMenuTextureMap[arg0].offset];
    color1 = &tex_buff[sMenuTextureMap[arg1].offset];
    for (var_t1 = 0; (u32) var_t1 < arg2; var_t1++) {
        temp_a0 = *color0++;
        red = (temp_a0 & 0xF800) >> 0xB;
        green = (temp_a0 & 0x7C0) >> 6;
        blue = (temp_a0 & 0x3E) >> 1;
        alpha = temp_a0 & 0x1;
        if (alpha) {}
        temp_t9 = ((red * 0x4D) + (green * 0x96) + (blue * 0x1D)) >> 8;
        newred = (((((temp_t9 - red) * arg3) >> 8) + red) << 0xB);
        newgreen = (((((((temp_t9 * 7) / 8) - green) * arg3) >> 8) + green) << 6);
        newblue = (((((((temp_t9 * 6) / 8) - blue) * arg3) >> 8) + blue) << 1);
        *color1++ = newblue + newgreen + newred + alpha;
    }
}

void GrayScale_tex_buff(s32 arg0) {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    u16* color;

    color = &tex_buff[sMenuTextureMap[arg0].offset];
    for (var_v1 = 0; var_v1 < 0x4B000; var_v1++) {
        red = ((*color & 0xF800) >> 0xB) * 0x4D;
        green = ((*color & 0x7C0) >> 6) * 0x96;
        blue = ((*color & 0x3E) >> 1) * 0x1D;
        alpha = *color & 0x1;
        temp_t9 = red + green + blue;
        temp_t9 >>= 8;
        newred = temp_t9 << 0xB;
        newgreen = temp_t9 << 6;
        newblue = temp_t9 << 1;
        *color++ = newblue + newgreen + newred + alpha;
    }
}

void GrayScale_tex_buff2(s32 arg0, s32 arg1) {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    UNUSED s32 newred;
    UNUSED s32 newgreen;
    UNUSED s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    s32 size;
    UNUSED u16 temp_a0;
    u16* color;

    color = &tex_buff[sMenuTextureMap[arg0].offset];
    size = sMenuTextureMap[arg0 + 1].offset - sMenuTextureMap[arg0].offset;
    for (var_v1 = 0; var_v1 != size; var_v1++) {
        red = ((*color & 0xF800) >> 0xB) * 0x4D;
        green = ((*color & 0x7C0) >> 6) * 0x96;
        blue = ((*color & 0x3E) >> 1) * 0x1D;
        alpha = *color & 0x1;
        temp_t9 = red + green + blue;
        temp_t9 = temp_t9 >> 8;
        temp_t9 += ((0x20 - temp_t9) * arg1) >> 8;
        *color++ = (temp_t9 << 1) + (temp_t9 << 6) + (temp_t9 << 0xB) + alpha;
    }
}

void GrayScaleTexBuf3(s32 arg0, u32 arg1) {
    u32 i;
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    u32 temp_t9;
    s32 size;
    u16* color;
    f32 sp48[32];

    for (i = 0; i < 32; i++) {
        sp48[i] = Power(i / 32.0, (arg1 * 1.5 / 256.0) + 0.25);
    }
    color = &tex_buff[sMenuTextureMap[arg0].offset];
    size = sMenuTextureMap[arg0 + 1].offset - sMenuTextureMap[arg0].offset;
    for (i = 0; i < (u32) size; i++) {
        red = ((*color & 0xF800) >> 0xB) * 0x55;
        green = ((*color & 0x7C0) >> 6) * 0x4B;
        blue = ((*color & 0x3E) >> 1) * 0x5F;
        alpha = *color & 0x1;
        temp_t9 = red + green + blue;
        temp_t9 /= 256;
        temp_t9 = sp48[temp_t9] * 32.0f;
        if (temp_t9 >= 32) {
            temp_t9 = 31;
        }
        *color++ = (temp_t9 << 1) + (temp_t9 << 6) + (temp_t9 << 0xB) + alpha;
    }
}

void GrayScaleTexBufRGB(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    u16* color;

    color = &tex_buff[sMenuTextureMap[arg0].offset];
    for (var_v1 = 0; var_v1 != arg1; var_v1++) {
        red = ((*color & 0xF800) >> 0xB) * 0x4D;
        green = ((*color & 0x7C0) >> 6) * 0x96;
        blue = ((*color & 0x3E) >> 1) * 0x1D;
        alpha = *color & 0x1;
        temp_t9 = red + green + blue;
        temp_t9 = temp_t9 / 256;
        newred = ((temp_t9 * arg2) / 256) << 0xB;
        newgreen = ((temp_t9 * arg3) / 256) << 6;
        newblue = ((temp_t9 * arg4) / 256) << 1;
        *color++ = newred + newgreen + newblue + alpha;
    }
}

u16* GetTexBuffPointer(u64* arg0) {
    UNUSED s32 pad[2];
    s32 offset;
    s32 found;
    s32 someIndex;

    found = 0;
    for (someIndex = 0; someIndex < all_tex_block_num; someIndex++) {
        if (arg0 == sMenuTextureMap[someIndex].textureData) {
            found = 1;
            offset = sMenuTextureMap[someIndex].offset;
            break;
        }
    }

    if (found != 0) {
        return &tex_buff[offset];
    }
    return NULL;
}

// kdl_buff is a little weird. In code_800AF9B0 its treated as a
// struct_8018EE10_entry pointer. But here its being treated as a
// Gfx pointer. It seems to be multi use.
void InitKDL(void) {
    kdl_buff_ptr = (Gfx*) kdl_buff;
    all_kdl_block_num = 0;
}

void KDL_maker1(MenuTexture* arg0) {
    kdl_buff_block[all_kdl_block_num].textures = GetTBPointer(arg0);
    kdl_buff_block[all_kdl_block_num].displayList = kdl_buff_ptr;
}

void KDL_maker2(void) {
    gSPEndDisplayList(kdl_buff_ptr++);
    all_kdl_block_num += 1;
}

// I don't get how this compiles, given that there's a code path
// that has no explicit return value.
// Based on the target assembly, in the event that it never finds
// the desired entry, it'll treat the return value of GetTBPointer
// as the return of this function. Which seems like a bug to me
Gfx* KDL_call(Gfx* displayListHead, MenuTexture* textures) {
    Gfx* displayList;
    UNUSED s32 pad;
    bool found;
    s32 index;

    found = false;
    for (index = 0; index < D_8018E768_SIZE; index++) {
        if (kdl_buff_block[index].textures == GetTBPointer(textures)) {
            displayList = kdl_buff_block[index].displayList;
            found = true;
            break;
        }
    }
    if (found) {
        gSPDisplayList(displayListHead++, displayList);
        return displayListHead;
    }
}

Gfx* doTexList(Gfx* arg0, MenuTexture* arg1, s32 column, s32 row) {
    MenuTexture* temp_v0;
    u8* temp_v0_3;
    s8 var_s4;

    temp_v0 = GetTBPointer(arg1);
    while (temp_v0->textureData != NULL) {
        var_s4 = 0;
        switch (temp_v0->type) {
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
            case 2:
                gSPDisplayList(arg0++, D_02007748);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_s4 = 3;
                break;
            case 4:
                gSPDisplayList(arg0++, D_02007788);
                break;
            default:
                gSPDisplayList(arg0++, D_02007728);
                break;
        }
        temp_v0_3 = (u8*) GetTexBuffPointer(temp_v0->textureData);
        if (temp_v0_3 != 0) {
            if (g_FadingFlag[4] != 4) {
                arg0 =
                    AutoTextureRect(arg0, var_s4, 0x00000400, 0x00000400, 0, 0, temp_v0->width, temp_v0->height,
                                  temp_v0->dX + column, temp_v0->dY + row, temp_v0_3, temp_v0->width, temp_v0->height);
            } else {
                arg0 = AutoTextureRectR(arg0, 0U, 0U, temp_v0->width, temp_v0->height, temp_v0->dX + column,
                                     temp_v0->dY + row, temp_v0_3, temp_v0->width, temp_v0->height);
            }
        }
        temp_v0++;
    }
    return arg0;
}

Gfx* doTexList2(Gfx* arg0, MenuTexture* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    MenuTexture* var_s0;
    u8* temp_v0_3;

    var_s0 = GetTBPointer(arg1);
    while (var_s0->textureData != NULL) {
        switch (var_s0->type) { /* irregular */
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
        }
        temp_v0_3 = (u8*) GetTexBuffPointer(var_s0->textureData);
        if (temp_v0_3 != 0) {
            switch (arg4) {
                case 1:
                    arg0 = AutoTextureRect64(arg0, 0, var_s0->dX + arg2, var_s0->dY + arg3, temp_v0_3, arg5);
                    break;
                case 2:
                    arg0 = AutoTextureRect_w(arg0, 0, 0, 0U, var_s0->width, var_s0->height, var_s0->dX + arg2,
                                         var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height, arg5);
                    break;
                case 3:
                    arg0 = AutoTextureRect_c(arg0, 0, 0, 0, var_s0->width, var_s0->height, var_s0->dX + arg2,
                                         var_s0->dY + arg3, temp_v0_3, var_s0->width, var_s0->height);
                    break;
                case 4:
                    arg0 = AutoTextureRect_2cyc(arg0, 0, 0x00000400, 0x00000400, 0, 0, var_s0->width, var_s0->height,
                                         var_s0->dX + arg2, var_s0->dY + arg3, (u16*) temp_v0_3, var_s0->width,
                                         var_s0->height, arg5);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}

Gfx* doTexList4(Gfx* arg0, MenuTexture* glyphTexture, f32 arg2, f32 arg3, s32 mode, f32 scaleX, f32 scaleY) {
    s32 var_v0;
    u8* temp_v0_2;
    f32 thing0;
    f32 thing1;
    MenuTexture* var_s0;

    var_s0 = GetTBPointer(glyphTexture);
    while (var_s0->textureData != NULL) {
        var_v0 = 0;

        thing0 = var_s0->dX + arg2;
        if (thing0 > 320.0f) {
            var_v0 = 1;
        }
        thing0 += var_s0->width * scaleX;
        if (thing0 < 0.0f) {
            var_v0 += 1;
        }
        thing1 = var_s0->dY + arg3;
        if (thing1 < 0.0f) {
            var_v0 += 1;
        }
        thing1 -= var_s0->height * scaleY;
        if (thing1 > 240.0f) {
            var_v0 += 1;
        }

        if (var_v0 != 0) {
            var_s0++;
        } else {
            temp_v0_2 = (u8*) GetTexBuffPointer(var_s0->textureData);
            if (temp_v0_2 != 0) {
                switch (mode) { /* irregular */
                    case 1:
                        gSPDisplayList(arg0++, D_020077F8);
                        arg0 = Set_IAFont(arg0, temp_v0_2, var_s0->dX + arg2, var_s0->dY + arg3, var_s0->width,
                                             var_s0->height, scaleX, scaleY);
                        break;
                    case 2:
                        gSPDisplayList(arg0++, D_02007818);
                        arg0 = Set_IAFont(arg0, temp_v0_2, var_s0->dX + arg2, var_s0->dY + arg3, var_s0->width,
                                             var_s0->height, scaleX, scaleY);
                        break;
                }
            }
            var_s0++;
        }
    }
    return arg0;
}

Gfx* doTexList5(Gfx* arg0, MenuTexture* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 var_s2;
    u8* temp_t0;
    MenuTexture* var_s1;

    var_s1 = GetTBPointer(arg1);
    while (var_s1->textureData != NULL) {
        var_s2 = 0;
        switch (var_s1->type) {
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_s2 = 3;
                break;
            default:
                gSPDisplayList(arg0++, D_02007728);
                break;
        }
        temp_t0 = (u8*) GetTexBuffPointer(var_s1->textureData);
        switch (arg4) {
            case 2:
                arg0 =
                    AutoTextureRect_zoom(arg0, var_s2, 0x00000400, 0x00000400, 0.5f, 0.5f, 0, 0, var_s1->width, var_s1->height,
                                  var_s1->dX + arg2, var_s1->dY + arg3, temp_t0, var_s1->width, var_s1->height);
                break;
            case 3:
                arg0 = AutoTextureRect_zoom(arg0, var_s2, 0x00000400, 0x00000400, 0.457f, 0.5f, 0, 0, var_s1->width,
                                     var_s1->height, var_s1->dX + arg2, var_s1->dY + arg3, temp_t0, var_s1->width,
                                     var_s1->height);
                break;
        }
        var_s1++;
    }
    return arg0;
}

Gfx* doTexList_w(Gfx* arg0, struct_8018DEE0_entry* arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 var_t0;
    s32 var_t1;
    Gfx* temp;
    MenuTexture* var_s0;

    var_s0 = GetTBPointer(arg1->textureSequence[arg1->sequenceIndex].mk64Texture);
    temp = D_02007728;
    while (var_s0->textureData != NULL) {
        var_t1 = 0;
        switch (var_s0->type) { /* irregular */
            default:
                gSPDisplayList(arg0++, temp);
                break;
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, temp);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_t1 = 3;
                break;
        }
        if (arg1->unk14 != 0) {
            var_t0 = sMenuTextureMap[arg1->menuTextureIndex + 1].offset;
        } else {
            var_t0 = sMenuTextureMap[arg1->menuTextureIndex].offset;
            if (1) {}
            if (1) {}
            if (1) {}
        }
        if (arg4 >= 0) {
            arg0 =
                AutoTextureRect_w(arg0, var_t1, 0, 0U, var_s0->width, var_s0->height, var_s0->dX + arg2, var_s0->dY + arg3,
                              (u8*) &tex_buff[var_t0], var_s0->width, var_s0->height, (u32) arg4);
        } else {
            switch (arg4) {
                case -1:
                    arg0 = AutoTextureRect(arg0, var_t1, 0x00000400, 0x00000400, 0, 0, var_s0->width, var_s0->height,
                                         var_s0->dX + arg2, var_s0->dY + arg3, (u8*) &tex_buff[var_t0],
                                         var_s0->width, var_s0->height);
                    break;
                case -2:
                    arg0 = AutoTextureRect_zoom(arg0, var_t1, 0x00000400, 0x00000400, 0.5f, 0.5f, 0, 0, var_s0->width,
                                         var_s0->height, var_s0->dX + arg2, var_s0->dY + arg3,
                                         (u8*) &tex_buff[var_t0], var_s0->width, var_s0->height);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}

Gfx* doTexList_vs(Gfx* arg0, struct_8018DEE0_entry* arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 var_t0;
    UNUSED s32 thing;
    Gfx* temp;
    MenuTexture* var_s1;

    var_s1 = GetTBPointer(arg1->textureSequence[arg1->sequenceIndex].mk64Texture);
    temp = D_02007728;
    while (var_s1->textureData != NULL) {
        var_t0 = 0;
        switch (var_s1->type) { /* irregular */
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, temp);
                break;
            case 2:
                gSPDisplayList(arg0++, D_02007748);
                break;
            case 3:
                gSPDisplayList(arg0++, D_02007768);
                var_t0 = 3;
                break;
            default:
                gSPDisplayList(arg0++, temp);
                break;
        }
        if (arg5 >= 0) {
            arg0 =
                AutoTextureRect_w(arg0, var_t0, 0, 0U, var_s1->width, var_s1->height, var_s1->dX + arg2, var_s1->dY + arg3,
                              D_802BFB80.arraySize4[arg1->unk14][arg4 / 2][(arg4 % 2) + 2].pixel_index_array,
                              var_s1->width, var_s1->height, (u32) arg5);
        }
        var_s1++;
    }
    return arg0;
}

void BeforeFadeMain(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 4; someIndex++) {
        old_screen_position[someIndex].x = D_8015F480[someIndex].posx;
        old_screen_position[someIndex].y = D_8015F480[someIndex].posy;
        old_screen_size[someIndex].x = D_8015F480[someIndex].width;
        old_screen_size[someIndex].y = D_8015F480[someIndex].height;
    }

    old_screen_position[4].x = 0x00A0;
    old_screen_position[4].y = 0x0078;
    old_screen_size[4].x = 0x0140;
    old_screen_size[4].y = 0x00F0;
}

void FadeMain(void) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        FadeMain2(var_s0);
    }
}

void FadeMain2(s32 arg0) {
    s32 var_a1;

    if ((arg0 == 4) || ((Get_OBJBlock_ptr2(0x000000AA) == NULL) && (Get_OBJBlock_ptr2(0x000000AB) == NULL) &&
                        (Get_OBJBlock_ptr2(0x000000B9) == NULL) && (Get_OBJBlock_ptr2(0x000000BA) == NULL) &&
                        (Get_OBJBlock_ptr2(0x000000AC) == NULL) && (Get_OBJBlock_ptr2(0x000000B0) == NULL))) {
        var_a1 = 0;
        gSPDisplayList(gDisplayListHead++, D_0D0076F8);
        if ((arg0 != 4) && (pause_flag != 0)) {
            var_a1 = 1;
        }
        switch (g_FadingFlag[arg0]) {
            case 1:
                FadeIn(arg0, var_a1);
                return;
            case 2:
                FadeOut(arg0, var_a1);
                return;
            case 3:
                FadeOut_m();
                return;
            case 4:
                FadeOut_r();
                return;
            case 5:
                FadeBlack(arg0);
                return;
            case 7:
                FadeOutWhite(arg0, var_a1);
                return;
            case 8:
                FadeInWhite(arg0, var_a1);
                break;
            case 0:
            default:
                break;
        }
    }
}

void FadeIn_(s32 arg0, s32 arg1, s32 arg2) {
    RGBA16* color;
    s16 x, y, w, h;
    UNUSED s32 pad[3];
    Screen* unk;
    struct UnkStruct_8018E7E8 *size, *start;

    if ((g_gameMode == GRAND_PRIX) || (g_gameMode == TIME_TRIALS)) {
        start = &(old_screen_position[arg0]);
        size = &(old_screen_size[arg0]);
        x = start->x;
        y = start->y;
        w = size->x;
        h = size->y;
    } else if (arg0 >= 4) {
        start = &(old_screen_position[arg0]);
        size = &(old_screen_size[arg0]);
        x = start->x;
        y = start->y;
        w = size->x;
        h = size->y;
    } else {
        unk = &D_8015F480[arg0];
        x = unk->posx;
        y = unk->posy;
        w = unk->width;
        h = unk->height;
    }
    color = &fade_color_bw[arg2];
    gDisplayListHead =
        FillRect1ColorF(gDisplayListHead, x - (w / 2), y - (h / 2), (w / 2) + x, (h / 2) + y, color->red, color->green,
                 color->blue, 0xFF - (g_FadeCounter2[arg0] * 0xFF / g_FadeCounter[arg0]));

    if ((arg1 == 0) &&
        (g_FadeCounter2[arg0] += 1, (g_FadeCounter2[arg0] >= g_FadeCounter[arg0]))) {
        if (gGamestate == RACING) {
            g_FadingFlag[arg0] = 6;
            return;
        }
        g_FadingFlag[arg0] = 0;
        fb_clear_flg = 0;
    }
}

void FadeIn(s32 arg0, s32 arg1) {
    FadeIn_(arg0, arg1, 0);
}

void FadeInWhite(s32 arg0, s32 arg1) {
    FadeIn_(arg0, arg1, 1);
}

void Setting_SoundOutMode4multiplayer(void) {
    if ((sound_output != 3) && (g_playerCount >= 2)) {
        Nai_SeqBufFlagCheck(0xE0000002);
    }
}

void EndFadeOut(s32 arg0) {
    s32 thing;
    s32 var_a1;
    UNUSED s32 stackPadding0;
    ObjBlock* temp_v0;

    var_a1 = 0;
    if (gGamestate == 5) {
        if (2 != g_raceClass) {
            thing = g_raceClass;
            if (thing != 3) {
                goto func_8009CE64_label1;
            }
            goto func_8009CE64_label2;
        }
    func_8009CE64_label2:
        if ((resultstat.rank < 3) && (g_cupSelect == 3)) {
            var_a1 = 1;
        }
    func_8009CE64_label1:
        if (var_a1) {
            gGotoMenu = 9;
            gCreditsCourseId = 8;
        } else {
            gGotoMenu = 1;
            KBGNumber = 0x0000000B;
        }
    } else if (gGamestate == RACING) {
        if (g_FadingFlag[arg0] == 2) {
            if (arg0 != 4) {
                g_FadingFlag[arg0] = 5;
            } else {
                var_a1 = 0;
                temp_v0 = Get_OBJBlock_ptr2(0x000000B0);
                if (temp_v0 != NULL) {
                    switch (temp_v0->pattern) { /* switch 8; irregular */
                        case 10:              /* switch 8 */
                            goto_racing();
                            break;
                        case 11: /* switch 8 */
                            goto_mapselect();
                            break;
                        case 12: /* switch 8 */
                            GoToGameSelect();
                            break;
                        default: /* switch 8 */
                        case 13: /* switch 8 */
                            goto_gameselect();
                            break;
                    }
                } else {
                    var_a1 = 0;
                    temp_v0 = Get_OBJBlock_ptr2(0x000000AC);
                    if (temp_v0 != NULL) {
                        switch (temp_v0->pattern) { /* switch 7; irregular */
                            case 11:              /* switch 7 */
                                goto_racing();
                                D_8016556E = 1;
                                break;
                            default: /* switch 7 */
                            case 12: /* switch 7 */
                                goto_gameselect();
                                break;
                        }
                    } else {
                        var_a1 = 0;
                        temp_v0 = Get_OBJBlock_ptr2(0x000000C7);
                        if (temp_v0 != NULL) {
                            switch (temp_v0->pattern) {
                                case 12:
                                    goto_racing();
                                    var_a1 = 1;
                                    break;
                                case 13:
                                case 22:
                                case 42:
                                    goto_mapselect();
                                    var_a1 = 1;
                                    break;
                                case 14:
                                case 23:
                                case 43:
                                    GoToGameSelect();
                                    var_a1 = 1;
                                    break;
                                case 15:
                                case 24:
                                case 32:
                                case 44:
                                    goto_gameselect();
                                    var_a1 = 1;
                                    break;
                                default:
                                    g_FadingFlag[arg0] = 5;
                                    break;
                            }
                            if (var_a1 != 0) {
                                pause_flag = 0;
                            }
                        }
                        temp_v0 = Get_OBJBlock_ptr2(0x000000BD);
                        if (temp_v0 != NULL) {
                            switch (temp_v0->pattern) { /* switch 2 */
                                case 11:              /* switch 2 */
                                    retry_flag = 1;
                                    replay_flag = 0;
                                    goto_racing();
                                    break;
                                case 12: /* switch 2 */
                                    retry_flag = 0;
                                    replay_flag = 0;
                                    goto_mapselect();
                                    break;
                                case 13: /* switch 2 */
                                    retry_flag = 0;
                                    replay_flag = 0;
                                    GoToGameSelect();
                                    break;
                                case 14: /* switch 2 */
                                    retry_flag = 0;
                                    replay_flag = 0;
                                    goto_gameselect();
                                    break;
                                case 15: /* switch 2 */
                                    retry_flag = 0;
                                    replay_flag = 1;
                                    goto_racing();
                                    break;
                                default: /* switch 2 */
                                    break;
                            }
                        } else {
                            g_FadingFlag[arg0] = 5;
                        }
                    }
                }
            }
        }
    } else {
        g_FadingFlag[arg0] = 0;
        if (title_mode != DEBUG_MENU_OPTION_SELECTED) {
            switch (KBGNumberNext) {      /* switch 3 */
                case MENU_FADE_TYPE_MAIN: /* switch 3 */
                    if (KBGNumber == LOGO_INTRO_MENU) {
                        KBGNumber = START_MENU;
                        KBGChange = FADE_MODE_LOGO;
                    } else {
                        KBGNumber++;
                    }
                    break;
                case MENU_FADE_TYPE_BACK: /* switch 3 */
                    KBGNumber -= 1;
                    break;
                case MENU_FADE_TYPE_DEMO: /* switch 3 */
                    g_DemoFlag = 1;
                    demo_play_flag = 1;
                    g_NewSequenceMode = 4;
                    g_raceClass = (s32) 1;
                    switch (gNextDemoId) { /* switch 4 */
                        case 0:            /* switch 4 */
                            g_courseID = COURSE_MARIO_RACEWAY;
                            g_ScreenSplitB = 0;
                            g_playerCount = 1;
                            g_menuMultiplayerSelection = 1;
                            g_CharacterSelections[0] = 0;
                            g_gameMode = 0;
                            break;
                        case 1: /* switch 4 */
                            g_courseID = COURSE_CHOCO_MOUNTAIN;
                            g_ScreenSplitB = (s32) 1;
                            g_playerCount = 2;
                            g_menuMultiplayerSelection = (s8) 2;
                            g_CharacterSelections[0] = (s8) 2;
                            g_CharacterSelections[1] = (s8) 4;
                            g_gameMode = 2;
                            break;
                        case 2: /* switch 4 */
                            g_courseID = COURSE_KALAMARI_DESERT;
                            g_ScreenSplitB = 0;
                            g_playerCount = (s32) 1;
                            g_menuMultiplayerSelection = 1;
                            g_CharacterSelections[0] = 1;
                            g_gameMode = 0;
                            break;
                        case 3: /* switch 4 */
                            g_courseID = COURSE_WARIO_STADIUM;
                            g_ScreenSplitB = 3;
                            g_playerCount = 3;
                            g_menuMultiplayerSelection = (s8) 3;
                            g_CharacterSelections[0] = 5;
                            g_CharacterSelections[1] = 2;
                            g_CharacterSelections[2] = 7;
                            g_gameMode = (s32) 2;
                            break;
                        case 4: /* switch 4 */
                            g_courseID = COURSE_BOWSER_CASTLE;
                            g_ScreenSplitB = 0;
                            g_playerCount = (s32) 1;
                            g_menuMultiplayerSelection = 1;
                            g_CharacterSelections[0] = 7;
                            g_gameMode = 0;
                            break;
                        case 5: /* switch 4 */
                            g_courseID = COURSE_SHERBET_LAND;
                            g_ScreenSplitB = 3;
                            g_playerCount = 4;
                            g_menuMultiplayerSelection = 4;
                            g_CharacterSelections[0] = 0;
                            g_CharacterSelections[1] = 1;
                            g_CharacterSelections[2] = 6;
                            g_CharacterSelections[3] = 3;
                            g_gameMode = 2;
                            break;
                        default:
                            break;
                    }
                    gNextDemoId += 1;
                    if (gNextDemoId >= 6) {
                        gNextDemoId = 0;
                    }
                    g_cupSelect = map2cup_chart[g_courseID];
                    cup_level = (s32) g_cupSelect;
                    g_courseSelect = (s8) map2circuit_chart[g_courseID];
                    break;
                case MENU_FADE_TYPE_DATA: /* switch 3 */
                    switch (KBGNumber) {
                        case 11:
                            KBGNumber = 6;
                            break;
                        case 6:
                            KBGNumber = 11;
                            break;
                    }
                    break;
                case MENU_FADE_TYPE_OPTION: /* switch 3 */
                    switch (KBGNumber) {
                        case 11:
                            KBGNumber = 5;
                            break;
                        case 5:
                            KBGNumber = 11;
                            break;
                    }
                    break;
            }
            if (KBGChange == FADE_MODE_NONE) {
                KBGChange = FADE_MODE_MAIN;
            }
            if (KBGNumber >= 0xE) {
                g_NewSequenceMode = 4;
                if (g_gameMode == 1) {
                    g_InGameTT = (s8) 1;
                }
                Setting_SoundOutMode4multiplayer();
            }
            fb_clear_flg = 0;
        } else {
            switch (gDebugGotoScene) {  /* switch 5; irregular */
                case DEBUG_GOTO_ENDING: /* switch 5 */
                    g_NewSequenceMode = (s32) 5;
                    break;
                case DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT: /* switch 5 */
                case DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA:   /* switch 5 */
                    g_NewSequenceMode = 9;
                    gCreditsCourseId = 8;
                    break;
                default: /* switch 5 */
                    g_NewSequenceMode = 4;
                    if (g_gameMode == (s32) 1) {
                        g_InGameTT = 1;
                    }
                    break;
            }
            set_rival_car_name();
            if (g_ScreenSplitB == 3) {
                switch (g_gameMode) {
                    case 0:
                    case 1:
                        g_gameMode = 2;
                        break;
                }
            }
            switch (g_courseID) {
                case COURSE_BLOCK_FORT:
                case COURSE_SKYSCRAPER:
                case COURSE_DOUBLE_DECK:
                case COURSE_BIG_DONUT:
                    g_gameMode = 3;
                    if (g_playerCount == 1) {
                        g_menuMultiplayerSelection = 2;
                        g_ScreenSplitB = 1;
                        g_playerCount = g_menuMultiplayerSelection;
                    }
                    break;
                default:
                    if (g_gameMode == 3) {
                        g_gameMode = 0;
                    }
                    if ((g_gameMode == 2) && (g_playerCount == 1)) {
                        g_gameMode = 0;
                    }
            }
            g_cupSelect = map2cup_chart[g_courseID];
            cup_level = g_cupSelect;
            g_courseSelect = map2circuit_chart[g_courseID];
            switch (gDebugGotoScene) {  /* switch 6; irregular */
                case DEBUG_GOTO_ENDING: /* switch 6 */
                    break;
                case DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT: /* switch 6 */
                    g_raceClass = 0;
                    break;
                case DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA: /* switch 6 */
                    g_raceClass = 3;
                    break;
                default: /* switch 6 */
                    if (g_raceClass == 3) {
                        g_ScreenFlip = 1;
                    } else {
                        g_ScreenFlip = 0;
                    }
                    break;
            }
        }
    }
}

void FadeOut_(s32 arg0, s32 arg1, s32 arg2) {
    s16 var_ra;
    s16 var_t3;
    s16 var_t4;
    s32 temp_t8;
    s32 temp_v1;
    s32 var_t2;
    s32 someMath0;
    s32 someMath1;
    RGBA16* temp_v0_2;
    s32 sp44;
    UNUSED s32 stackPadding0;

    if ((g_gameMode == 0) || (g_gameMode == 1)) {
        var_t3 = old_screen_position[arg0].x;
        var_t4 = old_screen_position[arg0].y;
        var_ra = old_screen_size[arg0].x;
        sp44 = old_screen_size[arg0].y;
    } else if (arg0 >= 4) {
        var_t3 = old_screen_position[arg0].x;
        var_t4 = old_screen_position[arg0].y;
        var_ra = old_screen_size[arg0].x;
        sp44 = old_screen_size[arg0].y;
    } else {
        var_t3 = D_8015F480[arg0].posx;
        var_t4 = D_8015F480[arg0].posy;
        var_ra = D_8015F480[arg0].width;
        sp44 = D_8015F480[arg0].height;
    }
    var_t2 = (g_FadeCounter2[arg0] * 0xFF) / g_FadeCounter[arg0];
    if (var_t2 >= 0x100) {
        var_t2 = 0x000000FF;
    }
    temp_v1 = var_ra / 2;
    temp_t8 = sp44 / 2;
    temp_v0_2 = &fade_color_bw[arg2];
    // Why does it have to written like this to match?
    someMath0 = temp_v1;
    someMath0 += var_t3;
    someMath1 = temp_t8;
    someMath1 += var_t4;
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, var_t3 - temp_v1, var_t4 - temp_t8, someMath0, someMath1,
                                temp_v0_2->red, temp_v0_2->green, temp_v0_2->blue, var_t2);
    if (arg1 == 0) {
        g_FadeCounter2[arg0]++;
        if ((g_FadeCounter[arg0] + 1) < g_FadeCounter2[arg0]) {
            EndFadeOut(arg0);
        }
    }
}

void FadeOut(s32 arg0, s32 arg1) {
    FadeOut_(arg0, arg1, 0);
}

void FadeOutWhite(s32 arg0, s32 arg1) {
    FadeOut_(arg0, arg1, 1);
}

void FadeBlack(s32 arg0) {
    s16 var_t0;
    s16 var_t1;
    s16 var_t2;
    s16 var_t3;
    s32 temp_v0;
    s32 temp_v1;
    s32 someMath0;
    s32 someMath1;

    if ((g_gameMode == 0) || (g_gameMode == 1)) {
        var_t0 = old_screen_position[arg0].x;
        var_t1 = old_screen_position[arg0].y;
        var_t2 = old_screen_size[arg0].x;
        var_t3 = old_screen_size[arg0].y;
    } else if (arg0 >= 4) {
        var_t0 = old_screen_position[arg0].x;
        var_t1 = old_screen_position[arg0].y;
        var_t2 = old_screen_size[arg0].x;
        var_t3 = old_screen_size[arg0].y;
    } else {
        var_t0 = D_8015F480[arg0].posx;
        var_t1 = D_8015F480[arg0].posy;
        var_t2 = D_8015F480[arg0].width;
        var_t3 = D_8015F480[arg0].height;
    }
    temp_v0 = var_t2 / 2;
    temp_v1 = var_t3 / 2;
    // Why does it have to written like this to match?
    someMath0 = temp_v0;
    someMath0 += var_t0;
    someMath1 = temp_v1;
    someMath1 += var_t1;
    gDisplayListHead =
        FillRect1ColorF(gDisplayListHead, var_t0 - temp_v0, var_t1 - temp_v1, someMath0, someMath1, 0, 0, 0, 0x000000FF);
}

void FadeOut_r(void) {
    u32 var_t0;

    g_FadeCounter2[4]++;
    if (g_FadeCounter2[4] >= (g_FadeCounter[4] + 1)) {
        EndFadeOut(4);
    }
    gDPPipeSync(gDisplayListHead++);
    var_t0 = (g_FadeCounter2[4] * 255) / g_FadeCounter[4];
    if ((s32) var_t0 >= 0x100) {
        var_t0 = 0x000000FF;
    }
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, (s32) var_t0);
}

void FadeOut_m(void) {
    s32 temp_t4;
    s32 temp_t5;
    s32 var_s0;
    u32 var_s3;
    s32 var_v1;

    g_FadeCounter2[4]++;
    // why?
    var_v1 = g_FadeCounter2[4];
    if ((u32) var_v1 >= g_FadeCounter[4]) {
        if ((u32) var_v1 == g_FadeCounter[4]) {
            for (var_s0 = 0; var_s0 < 0x4B0; var_s0++) {
                k_work[var_s0] = 1;
            }
        } else {
            EndFadeOut(4);
        }
    } else {
        var_s0 = 0;
        var_s3 = 0;
        while (var_s3 < (0x4B0U / g_FadeCounter[4])) {
            if ((k_work[var_s0] == 0) &&
                (MakeRandomLimmit((0x4B0U - g_FadeCounter2[4]) / g_FadeCounter[4]) == 0)) {
                var_s3 += 1;
                k_work[var_s0] = 1;
            }
            var_s0 += 1;
            if (var_s0 >= 0x4B0) {
                var_s0 = 0;
            }
        }
    }
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
    gDPSetCombineMode(gDisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    for (var_s0 = 0; var_s0 < 0x4B0; var_s0++) {
        if (k_work[var_s0] != 0) {
            temp_t4 = (var_s0 % 40) * 8;
            temp_t5 = (var_s0 / 40) * 8;
            gDPFillRectangle(gDisplayListHead++, temp_t4, temp_t5, temp_t4 + 8, temp_t5 + 8);
        }
    }
    gDPPipeSync(gDisplayListHead++);
    var_v1 = (g_FadeCounter2[4] * 255) / g_FadeCounter[4];
    if (var_v1 >= 0x100) {
        var_v1 = 0x000000FF;
    }
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, var_v1);
}

void SetFadeIn_(u32 arg0, u32 arg1) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    if ((g_FadingFlag[4] != 1) && (g_FadingFlag[4] != 6)) {
        g_FadingFlag[4] = arg1;
        g_FadeCounter[4] = arg0;
        if (g_FadeCounter[4] >= 0x100U) {
            g_FadeCounter[4] = 0xFFU;
        }
        D_8018E7E0 = 0;
    }
}

void SetFadeIn(s32 arg0) {
    SetFadeIn_(arg0, 1);
}

void SetFadeInWhite(s32 arg0) {
    SetFadeIn_(arg0, 8);
}

void SetFadeOut_(u32 arg0, u32 arg1) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    if ((g_FadingFlag[4] != 2) && (g_FadingFlag[4] != 5)) {
        g_FadingFlag[4] = arg1;
        g_FadeCounter[4] = arg0;
        if (g_FadeCounter[4] >= 0x100U) {
            g_FadeCounter[4] = 0xFFU;
        }
        D_8018E7E0 = 0;
    }
}

void SetFadeOut(s32 arg0) {
    SetFadeOut_(arg0, 2);
}

void SetFadeOutWhite(s32 arg0) {
    SetFadeOut_(arg0, 7);
}

void SetFadeInScreen(s32 arg0, s32 arg1) {
    s32 temp;

    if (arg1 == 0) {
        arg1 = 1;
    }

    temp = g_FadingFlag[arg0];
    if ((temp != 1) && (temp != 6)) {
        g_FadingFlag[arg0] = 1;
        g_FadeCounter[arg0] = arg1;
        if ((u32) arg1 >= 0x100U) {
            g_FadeCounter[arg0] = 0xFF;
        }
        g_FadeCounter2[arg0] = 0;
    }
}

void SetFadeOutScreen(s32 arg0, s32 arg1) {
    s32 temp;

    if (arg1 == 0) {
        arg1 = 1;
    }

    temp = g_FadingFlag[arg0];
    if ((temp != 2) && (temp != 5)) {
        g_FadingFlag[arg0] = 2;
        g_FadeCounter[arg0] = arg1;
        if ((u32) arg1 >= 0x100U) {
            g_FadeCounter[arg0] = 0xFF;
        }
        g_FadeCounter2[arg0] = 0;
    }
}

void SetFadeOut_m(s32 arg0) {
    s32 var_v0;

    if (g_FadingFlag[4] != 3) {
        g_FadingFlag[4] = 3;
        g_FadeCounter[4] = arg0;
        if (g_FadeCounter[4] >= 0x100U) {
            g_FadeCounter[4] = 0x000000FF;
        }
        D_8018E7E0 = 0;
        for (var_v0 = 0; var_v0 < 0x4B0; var_v0++) {
            k_work[var_v0] = 0;
        }
    }
}

void SetFadeOut_r(u32 arg0) {
    if (g_FadingFlag[4] != 4) {
        g_FadingFlag[4] = 4;
        g_FadeCounter[4] = arg0;
        if (g_FadeCounter[4] >= 0x100U) {
            g_FadeCounter[4] = 0x000000FFU;
        }
        D_8018E7E0 = 0;
    }
}

void SetFadeOut_f(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_MAIN;
}

void SetFadeOut_fw(void) {
    SetFadeOutWhite(10);
    KBGNumberNext = MENU_FADE_TYPE_MAIN;
}

void SetFadeOutB(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_BACK;
}

void SetFadeOut_d(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_DEMO;
}

void SetFadeOutTaData(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_DATA;
}

void SetFadeOut_option(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_OPTION;
}

void ThunderMain(s32 arg0) {
    switch (thunder_flg[arg0]) {
        case 0:
            break;
        case 1:
            k_thunder(arg0);
            break;
        default:
            thunder_flg[arg0] = 0;
            break;
    }
}

void k_thunder(s32 arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 someIndex;
    s32 temp_t7;
    f32 temp_t7_2;
    RGBA16* temp_v0;
    struct UnkStruct_8018E7E8* temp_t0;
    struct UnkStruct_8018E7E8* temp_t1;

    temp_t7 = thunder_onoff[thunder_counter[arg0]];
    if (temp_t7 != 0) {
        temp_t1 = &old_screen_position[arg0];
        temp_t0 = &old_screen_size[arg0];
        temp_v0 = &k_thunder_color[temp_t7];
        if ((u32) thunder_counter[arg0] < 0x1BU) {
            gDisplayListHead = FillRect1ColorF(gDisplayListHead, temp_t1->x - (temp_t0->x / 2), temp_t1->y - (temp_t0->y / 2),
                                        temp_t1->x + (temp_t0->x / 2), temp_t1->y + (temp_t0->y / 2), temp_v0->red,
                                        temp_v0->green, temp_v0->blue, temp_v0->alpha);
        } else {
            temp_t7_2 = ((u32) (38 - thunder_counter[arg0])) / 11.0;
            gDisplayListHead = FillRect1ColorF(gDisplayListHead, temp_t1->x - (temp_t0->x / 2), temp_t1->y - (temp_t0->y / 2),
                                        temp_t1->x + (temp_t0->x / 2), temp_t1->y + (temp_t0->y / 2), temp_v0->red,
                                        temp_v0->green, temp_v0->blue, (u32) (temp_v0->alpha * temp_t7_2));
        }
    }
    thunder_counter[arg0]++;
    if ((u32) thunder_counter[arg0] >= 0x26U) {
        for (someIndex = 0; someIndex < 4; someIndex++) {
            thunder_flg[someIndex] = 0;
        }
    }
}

void ThunderWorld(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        SetThunder_(i);
    }
}

void SetThunder_(s32 arg0) {
    thunder_flg[arg0] = 1;
    thunder_counter[arg0] = 0;
}

void InitObjBlock(void) {
    s32 index;
    for (index = 0; index < ARRAY_COUNT(gMenuItems); index++) {
        gMenuItems[index].kind = 0;
    }
}

void SetObjBlock(s32 type, s32 column, s32 row, s8 priority) {
    ObjBlock* menuItem;
    s8 temp_a1;
    s32 i;
    MkAnimation* var_a0;

    i = 0;
    menuItem = gMenuItems;
    while (true) {
        if (menuItem->kind == 0) {
            break;
        }
        i++;
        if (i > ARRAY_COUNT(gMenuItems)) {
            while (true) {}
        }
        menuItem++;
    }
    menuItem->kind = type;
    menuItem->pattern = 0;
    menuItem->pattern2 = 0;
    menuItem->disp_x = column;
    menuItem->disp_y = row;
    menuItem->pri = priority;
    menuItem->disp_on = 1;
    menuItem->work1 = 0;
    menuItem->work2 = 0;
    switch (type) {
        case MENU_ITEM_UI_LOGO_INTRO:
            logo_timer = 0;
            logo_delay_size = 0;
            logo_yang_speed = 3.0f;
            logo_cam_z = 2500;
            logo_rot_x = 0.0f;
            logo_rot_y = -270.0f;
            logo_rot_z = 0.0f;
            logo_pos_x = 0.0f;
            logo_pos_y = 0.0f;
            logo_pos_z = 0.0f;
            logo_scale_z = 3;
            menuItem->work1 = -1;
            menuItem->work2 = 1;
            break;
        case START_MENU_FLAG:
            logo_cam_z = 1800;
            logo_rot_x = -51.0f;
            logo_rot_y = -12.0f;
            logo_rot_z = -18.0f;
            logo_pos_x = -270.0f;
            logo_pos_y = 750.0f;
            logo_pos_z = 0.0f;
            logo_scale_z = 1.0f;
            menuItem->work1 = -1;
            menuItem->work2 = 1;
            break;
        case MENU_ITEM_TYPE_0D2:
            TexBuffLoadP(D_020014C8, LOAD_MENU_IMG_TKMK00_ONCE);
            KDL_maker1(D_020014C8);
            kdl_buff_ptr = doTexList(kdl_buff_ptr, D_020014C8, menuItem->disp_x, menuItem->disp_y);
            KDL_maker2();
            break;
        case MENU_ITEM_TYPE_0D3:
            TexBuffLoadP(D_02001540, LOAD_MENU_IMG_TKMK00_ONCE);
            KDL_maker1(D_02001540);
            kdl_buff_ptr = doTexList(kdl_buff_ptr, D_02001540, menuItem->disp_x, menuItem->disp_y);
            KDL_maker2();
            break;
        case MENU_ITEM_TYPE_0D4:
            TexBuffLoadP(D_0200157C, LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoad(D_02001874);
            menuItem->disp_y = 0x69;
            for (i = 0; i < 133; i++) {
                TexBuffLoad(GetTBPointer(D_800E7AF8[i]));
            }
            break;
        case MENU_ITEM_TYPE_0D5:
            TexBuffLoad(D_020015A4);
            KDL_maker1(D_020015A4);
            kdl_buff_ptr = doTexList(kdl_buff_ptr, D_020015A4, menuItem->disp_x, menuItem->disp_y);
            gDPLoadTextureBlock(kdl_buff_ptr++, GetTexBuffPointer(gTexture7ED50C), G_IM_FMT_IA, G_IM_SIZ_16b, 256, 5, 0,
                                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(kdl_buff_ptr++, 0x80, 0x2C0, 0x480, 0x2D4, G_TX_RENDERTILE, 0, 0x80, 0x0400, 0xFC00);
            KDL_maker2();
            TexBuffLoad(D_020015CC);
            KDL_maker1(D_020015CC);
            kdl_buff_ptr = doTexList(kdl_buff_ptr, D_020015CC, menuItem->disp_x, menuItem->disp_y);
            KDL_maker2();
            TexBuffLoad(D_02001630);
            KDL_maker1(D_02001630);
            kdl_buff_ptr = doTexList(kdl_buff_ptr, D_02001630, menuItem->disp_x, menuItem->disp_y);
            KDL_maker2();
            TexBuffLoad(D_02001658);
            KDL_maker1(D_02001658);
            kdl_buff_ptr = doTexList(kdl_buff_ptr, D_02001658, menuItem->disp_x, menuItem->disp_y);
            KDL_maker2();
            break;
        case MENU_ITEM_TYPE_0D6:
            menuItem->sd_number = SetSeqData(GetVAddress(D_800E7D34[0]));
            break;
        case MENU_ITEM_TYPE_0D7:
            for (i = 0; i < 10; i++) {
                TexBuffLoad(GetTBPointer(D_800E7D0C[i]));
            }
            break;
        case MENU_ITEM_TYPE_0D8:
        case MENU_ITEM_TYPE_0D9:
            TexBuffLoad(D_0200184C);
            break;
        case MENU_ITEM_UI_START_BACKGROUND:
#if !OverKartBuild
            TexBuffLoadP(gMenuTexturesBackground[Check4GoldCup_150cc()], LOAD_MENU_IMG_TKMK00_ONCE);
#endif
            break;
        case MENU_ITEM_UI_LOGO_AND_COPYRIGHT:
            kwmelt_titlelogo();
            tex_buff_num += 0x10000;
            TexBuffLoad(seg2_copyright_1996_texture);
            break;
        case MENU_ITEM_UI_PUSH_START_BUTTON:
            TexBuffLoad(seg2_push_start_button_texture);
            break;
        case MAIN_MENU_BACKGROUND:
        case CHARACTER_SELECT_BACKGROUND:
        case COURSE_SELECT_BACKGROUND:
            TexBuffLoadP(gMenuTexturesBackground[Check4GoldCup_150cc()], LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoadP(D_02004B74, LOAD_MENU_IMG_TKMK00_ONCE);
            GrayScaleTexBuf3(0, 0x00000019);
            GrayScaleTexBufRGB(0, SCREEN_WIDTH * SCREEN_HEIGHT, titleback_color[type - MAIN_MENU_BACKGROUND].red,
                              titleback_color[type - MAIN_MENU_BACKGROUND].green, titleback_color[type - MAIN_MENU_BACKGROUND].blue);
            break;
        case MENU_ITEM_UI_OK:
            menuItem->work1 = 0x20;
            /* fallthrough */
        case MENU_ITEM_UI_GAME_SELECT:
        case MAIN_MENU_DATA_GFX:
        case MAIN_MENU_OPTION_GFX:
        case MAIN_MENU_50CC:
        case MAIN_MENU_100CC:
        case MAIN_MENU_150CC:
        case 0x15:
        case 0x16:
        case 0x17:
        case MAIN_MENU_TIME_TRIALS_BEGIN:
        case MAIN_MENU_TIME_TRIALS_DATA:
            TexBuffLoadP(GetTBPointer(D_800E8274[type - 0x12]), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_UI_1P_GAME:
        case MENU_ITEM_UI_2P_GAME:
        case MENU_ITEM_UI_3P_GAME:
        case MENU_ITEM_UI_4P_GAME:
            TexBuffLoadP(GetTBPointer(D_800E8234[((type - 0xB) * 2) + 0]),
                                    LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoad(GetTBPointer(D_800E8234[((type - 0xB) * 2) + 1]));
            break;
        case CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER:
            TexBuffLoadP(D_02004B4C, LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case CHARACTER_SELECT_MENU_OK:
            TexBuffLoadP(D_02004B74, LOAD_MENU_IMG_TKMK00_ONCE);
            menuItem->work1 = 0x00000020;
            break;
        case CHARACTER_SELECT_MENU_1P_CURSOR:
        case CHARACTER_SELECT_MENU_2P_CURSOR:
        case CHARACTER_SELECT_MENU_3P_CURSOR:
        case CHARACTER_SELECT_MENU_4P_CURSOR:
            TexBuffLoad(GetTBPointer(gMenuTexturesBorderPlayer[type - CHARACTER_SELECT_MENU_1P_CURSOR]));
            break;
        case CHARACTER_SELECT_MENU_MARIO:
        case CHARACTER_SELECT_MENU_LUIGI:
        case CHARACTER_SELECT_MENU_TOAD:
        case CHARACTER_SELECT_MENU_PEACH:
        case CHARACTER_SELECT_MENU_YOSHI:
        case CHARACTER_SELECT_MENU_DK:
        case CHARACTER_SELECT_MENU_WARIO:
        case CHARACTER_SELECT_MENU_BOWSER:
            menuItem->sd_number =
                SetSeqData(GetVAddress(D_800E8320[type - 0x2B]));
            TexBuffLoadP(GetTBPointer(SelectNamePlateTable[type - 0x2B]), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_0A0:
        case MENU_ITEM_TYPE_0A1:
            menuItem->sd_number =
                SetSeqData(GetVAddress(D_800E8320[type - 0xA0]));
            break;
        case COURSE_SELECT_OK:
            menuItem->work1 = 0x00000020;
            /* fallthrough */
        case COURSE_SELECT_MAP_SELECT:
        case COURSE_SELECT_MUSHROOM_CUP:
        case COURSE_SELECT_FLOWER_CUP:
        case COURSE_SELECT_STAR_CUP:
        case COURSE_SELECT_SPECIAL_CUP:
        case MENU_ITEM_TYPE_058:
        case COURSE_SELECT_COURSE_NAMES:
        case MENU_ITEM_TYPE_05A:
        case MENU_ITEM_TYPE_05B:
        case COURSE_SELECT_BATTLE_NAMES:
            TexBuffLoadP(GetTBPointer(gMenuTexturesTrackSelection[type - 0x52]),
                                    LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_05F:
        case MENU_ITEM_TYPE_060:
        case MENU_ITEM_TYPE_061:
        case MENU_ITEM_TYPE_062:
            menuItem->sd_number = SetSeqData(
                GetVAddress(D_800E7E34[gCupCourseOrder[0][menuItem->kind - 0x5F]]));
            break;
        case MENU_ITEM_TYPE_05E:
            menuItem->work2 = MakeRandomLimmit(4) + 2;
            break;
        case MENU_ITEM_TYPE_065:
        case MENU_ITEM_TYPE_066:
            menuItem->disp_x = msel_bestbox_default_position[type - 0x65].x;
            menuItem->disp_y = msel_bestbox_default_position[type - 0x65].y;
            break;
        case MENU_ITEM_TYPE_067:
            menuItem->work1 = (s32) g_cupSelect;
            menuItem->work2 = GetCupStatus(g_cupSelect, g_raceClass);
            menuItem->sd_number = SetSeqData(
                GetVAddress(D_800E7E20[((g_raceClass / 2) * 4) - menuItem->work2]));
            menuItem->disp_x = msel_cup_positions[0].x;
            menuItem->disp_y = msel_cup_positions[0].y;
            break;
        case MENU_ITEM_TYPE_068:
            TexBuffLoadP(GetTBPointer(D_800E8294[g_raceClass]), LOAD_MENU_IMG_TKMK00_ONCE);
            menuItem->disp_x = 0x37;
            menuItem->disp_y = 0xC3;
            break;
        case MENU_ITEM_TYPE_069:
            TexBuffLoadP(GetTBPointer(D_02004A0C), LOAD_MENU_IMG_TKMK00_ONCE);
            if (CheckPakStatus() == 0) {
                LoadGameNoteKart_data();
            } else {
                D_8018EE10[0].ghostDataSaved = 0;
                D_8018EE10[1].ghostDataSaved = 0;
            }
            break;
        case MENU_ITEM_TYPE_078:
        case MENU_ITEM_TYPE_079:
        case MENU_ITEM_TYPE_07A:
        case MENU_ITEM_TYPE_07B:
            TexBuffLoadP(GetTBPointer(D_800E82F4[type - 0x78]), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_08C:
            TexBuffLoadP(GetTBPointer(seg2_data_texture), LOAD_MENU_IMG_TKMK00_ONCE);
            if (CheckPakStatus() == 0) {
                LoadGameNoteKart_data();
            } else {
                D_8018EE10[0].ghostDataSaved = 0;
                D_8018EE10[1].ghostDataSaved = 0;
            }
            break;
        case MENU_ITEM_TYPE_08D:
            TexBuffLoad(GetTBPointer(D_02001FA4));
            break;
        case MENU_ITEM_TYPE_07C:
        case MENU_ITEM_TYPE_07D:
        case MENU_ITEM_TYPE_07E:
        case MENU_ITEM_TYPE_07F:
        case MENU_ITEM_TYPE_080:
        case MENU_ITEM_TYPE_081:
        case MENU_ITEM_TYPE_082:
        case MENU_ITEM_TYPE_083:
        case MENU_ITEM_TYPE_084:
        case MENU_ITEM_TYPE_085:
        case MENU_ITEM_TYPE_086:
        case MENU_ITEM_TYPE_087:
        case MENU_ITEM_TYPE_088:
        case MENU_ITEM_TYPE_089:
        case MENU_ITEM_TYPE_08A:
        case MENU_ITEM_TYPE_08B:
            TexBuffLoadP(
                GetTBPointer(D_800E7D74[gCupCourseOrder[(menuItem->kind - MENU_ITEM_TYPE_07C) / 4]
                                                                    [(menuItem->kind - MENU_ITEM_TYPE_07C) % 4]]),
                LOAD_MENU_IMG_MIO0_ONCE);
            TexBuffLoadP(
                GetTBPointer(D_800E7DC4[gCupCourseOrder[(menuItem->kind - MENU_ITEM_TYPE_07C) / 4]
                                                                    [(menuItem->kind - MENU_ITEM_TYPE_07C) % 4]]),
                LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoadP(GetTBPointer(D_02004A0C), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_0B1:
        case MENU_ITEM_TYPE_0B2:
        case MENU_ITEM_TYPE_0B3:
        case MENU_ITEM_TYPE_0B4: {
            bool var_v1_3;
            UNUSED s32 pad2;
            s32 temp_a3 = type - MENU_ITEM_TYPE_0B1;
            UNUSED s32 pad[0x3];
            temp_a1 = driver2pselset[g_CharacterSelections[type - MENU_ITEM_TYPE_0B1]];
            var_v1_3 = false;
            switch (g_gameMode) {
                case VERSUS:
                    if (gGPCurrentRaceRankByPlayerId[type - MENU_ITEM_TYPE_0B1] != 0) {
                        var_v1_3 = true;
                    }
                    break;
                case BATTLE:
                    if ((type - MENU_ITEM_TYPE_0B1) != win_kart) {
                        var_v1_3 = true;
                    }
                    break;
                default:
                    break;
            }
            if (var_v1_3) {
                var_a0 = TBSeq_psel_p6[temp_a1];
            } else {
                var_a0 = D_800E8320[temp_a1];
            }
            menuItem->sd_number = SetSeqData2(GetVAddress(var_a0), temp_a3);
            TexBuffLoadP(GetTBPointer(SelectNamePlateTable[temp_a1]), LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoad(GetTBPointer(gMenuTexturesBorderPlayer[type - MENU_ITEM_TYPE_0B1]));
            break;
        }
        case MENU_ITEM_TYPE_0BB:
            menuItem->work1 = SaveCourseRecord(playerHUD[0].totaltime, g_CharacterSelections[0]);
            menuItem->work2 = SaveCourseLapRecord();
            if (ghost_status != 1) {
                if (make_press_ghost() > 0x3C00) {
                    ghost_status = 1;
                }
            }
            if ((menuItem->work1 == 0) || (menuItem->work2 != 0)) {
                SaveFunc800B559CDefault((g_cupSelect * 4) + g_courseSelect);
            }
            break;
        case MENU_ITEM_DATA_COURSE_IMAGE:
            menuItem->sd_number = SetSeqData(GetVAddress(
                D_800E7E34[gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4]]));
            menuItem->work1 = gTimeTrialDataCourseIndex;
            TexBuffLoadP(GetTBPointer(D_02004A0C), LOAD_MENU_IMG_TKMK00_ONCE);
            kwmelt_kawanoroadmap();
            if (CheckPakStatus() == 0) {
                LoadGameNoteKart_data();
            } else {
                D_8018EE10[0].ghostDataSaved = 0;
                D_8018EE10[1].ghostDataSaved = 0;
            }
            break;
        case MENU_ITEM_TYPE_0F0:
            menuItem->pattern = (s32) sound_output;
            break;
        case MENU_ITEM_TYPE_0F1:
            TexBuffLoadP(GetTBPointer(D_02004638), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_0BE:
            rankin_fanfare = 0;
            break;
        case MENU_ITEM_TYPE_130: {
            bool var_v0_2;
            temp_a1 = driver2pselset[resultstat.kart];
            if (resultstat.rank >= 3) {
                var_v0_2 = true;
            } else {
                var_v0_2 = false;
            }
            if (var_v0_2) {
                var_a0 = TBSeq_psel_p6[temp_a1];
            } else {
                var_a0 = D_800E8320[temp_a1];
            }
            menuItem->sd_number = SetSeqData2(GetVAddress(var_a0), 0);
            TexBuffLoadP(GetTBPointer(SelectNamePlateTable[temp_a1]), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        }
        case MENU_ITEM_TYPE_190:
        case MENU_ITEM_TYPE_191:
        case MENU_ITEM_TYPE_192:
        case MENU_ITEM_TYPE_193:
        case MENU_ITEM_TYPE_194:
        case MENU_ITEM_TYPE_195:
        case MENU_ITEM_TYPE_196:
        case MENU_ITEM_TYPE_197:
        case MENU_ITEM_TYPE_198:
        case MENU_ITEM_TYPE_199:
        case MENU_ITEM_TYPE_19A:
        case MENU_ITEM_TYPE_19B:
        case MENU_ITEM_TYPE_19C:
        case MENU_ITEM_TYPE_19D:
        case MENU_ITEM_TYPE_19E:
        case MENU_ITEM_TYPE_19F:
        case MENU_ITEM_TYPE_1A0:
        case MENU_ITEM_TYPE_1A1:
        case MENU_ITEM_TYPE_1A2:
        case MENU_ITEM_TYPE_1A3:
        case MENU_ITEM_TYPE_1A4:
        case MENU_ITEM_TYPE_1A5:
        case MENU_ITEM_TYPE_1A6:
        case MENU_ITEM_TYPE_1A7:
        case MENU_ITEM_TYPE_1A8:
        case MENU_ITEM_TYPE_1A9:
        case MENU_ITEM_TYPE_1AA:
        case MENU_ITEM_TYPE_1AB:
        case MENU_ITEM_TYPE_1AC:
        case MENU_ITEM_TYPE_1AD:
        case MENU_ITEM_TYPE_1AE:
        case MENU_ITEM_TYPE_1AF:
        case MENU_ITEM_TYPE_1B0:
        case MENU_ITEM_TYPE_1B1:
        case MENU_ITEM_TYPE_1B2:
        case MENU_ITEM_TYPE_1B3:
        case MENU_ITEM_TYPE_1B4:
        case MENU_ITEM_TYPE_1B5:
        case MENU_ITEM_TYPE_1B6:
        case MENU_ITEM_TYPE_1B7:
        case MENU_ITEM_TYPE_1B8:
        case MENU_ITEM_TYPE_1B9:
        case MENU_ITEM_TYPE_1BA:
        case MENU_ITEM_TYPE_1BB:
        case MENU_ITEM_TYPE_1BC:
        case MENU_ITEM_TYPE_1BD:
        case MENU_ITEM_TYPE_1BE:
        case MENU_ITEM_TYPE_1BF:
        case MENU_ITEM_TYPE_1C0:
        case MENU_ITEM_TYPE_1C1:
        case MENU_ITEM_TYPE_1C2:
        case MENU_ITEM_TYPE_1C3:
        case MENU_ITEM_TYPE_1C4:
        case MENU_ITEM_TYPE_1C5:
        case MENU_ITEM_TYPE_1C6:
        case MENU_ITEM_TYPE_1C7:
        case MENU_ITEM_TYPE_1C8:
        case MENU_ITEM_TYPE_1C9:
        case MENU_ITEM_TYPE_1CA:
        case MENU_ITEM_TYPE_1CB:
        case MENU_ITEM_TYPE_1CC:
        case MENU_ITEM_TYPE_1CD:
        case MENU_ITEM_TYPE_1CE:
        default:
            break;
    }
}

void DispOBJBlock(ObjBlock* arg0) {
    s32 var_a1;
    s32 var_v1;
    UNUSED s32 pad[2];
    MenuTexture* texture;
    s32 temp_a0;
    s32 temp_t2;
    s32 temp_t5;
    s32 temp_t9;
    s32 temp_v1;
    UNUSED s32 pad2;
    char sp80[3];
    f32 why = 0.75f;
    s32 one = 1;
    UNUSED s32 pad3;

    if (arg0->disp_on) {
        gDPPipeSync(gDisplayListHead++);
        switch (arg0->kind) {             /* switch 6; irregular */
            case MENU_ITEM_UI_LOGO_INTRO: /* switch 6 */
                doNintenLogo(gDynamicP, arg0->work1);
                break;
            case START_MENU_FLAG: /* switch 6 */
                doCheckerFlag(gDynamicP, arg0->work1);
                break;
            case MENU_ITEM_TYPE_0D2: /* switch 6 */
                gDisplayListHead = KDL_call(gDisplayListHead, D_020014C8);
                break;
            case MENU_ITEM_TYPE_0D3: /* switch 6 */
                gDisplayListHead = KDL_call(gDisplayListHead, D_02001540);
                break;
            case MENU_ITEM_TYPE_0D4: /* switch 6 */
                DispOB_cp_gnote(arg0);
                DispOB_cp_notecursor(arg0);
                DispOB_cp_gnote_name(arg0);
                break;
            case MENU_ITEM_TYPE_0D5: /* switch 6 */
                                     // Sets the text color of the text on the Controller Pak menu
                                     // Also sets the color of the shading at the top and bottom of the menu
                                     // Does not set color of the text in the table itself
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
                gDisplayListHead = KDL_call(gDisplayListHead, D_020015A4);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x32, 0x00, 0x00, 0xFF);
                gDisplayListHead = KDL_call(gDisplayListHead, D_020015CC);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x32, 0x00, 0xFF);
                gDisplayListHead = KDL_call(gDisplayListHead, D_02001630);
                gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x32, 0x32, 0x00, 0xFF);
                gDisplayListHead = KDL_call(gDisplayListHead, D_02001658);
                break;
            case MENU_ITEM_TYPE_0D6: /* switch 6 */
                doSeqData2(arg0->sd_number, arg0->disp_x, arg0->disp_y, -1);
                break;
            case MENU_ITEM_TYPE_0D7: /* switch 6 */
                DispOB_cp_free();
                break;
            case MENU_ITEM_TYPE_0D8: /* switch 6 */
            case MENU_ITEM_TYPE_0D9: /* switch 6 */
                DispOB_cp_cursor(arg0, arg0->kind - 0xD8);
                break;
            case MENU_ITEM_UI_START_BACKGROUND: /* switch 6 */
#if !OverKartBuild
                gDisplayListHead = doTexList(
                    gDisplayListHead, gMenuTexturesBackground[Check4GoldCup_150cc()], arg0->disp_x, arg0->disp_y);
#endif
                break;
            case MENU_ITEM_UI_LOGO_AND_COPYRIGHT: /* switch 6 */
                kwdisplay_titlelogo((arg0->disp_x + 0xA0), (arg0->disp_y + 0x47));
                gDisplayListHead =
                    doTexList(gDisplayListHead, seg2_copyright_1996_texture, arg0->disp_x, arg0->disp_y);
                break;
            case MENU_ITEM_UI_PUSH_START_BUTTON: /* switch 6 */
                if (((frame_counter / 8) % 3) != 0) {
                    gDisplayListHead =
                        doTexList(gDisplayListHead, seg2_push_start_button_texture, arg0->disp_x, arg0->disp_y);
                }
                break;
            case MENU_ITEM_UI_START_RECORD_TIME: {
                s32 strWidth;
                strWidth = (s32) ((f32) (GetWordLength(g_StringTableCourse[0]) + 5) * 0.9f) / 2;
                gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0xA0 - strWidth, 0x0000007B, strWidth + 0xA0, 0x000000A4,
                                            0, 0, 0, 0x00000096);
                set_text_color(TEXT_GREEN);
                print_text1_center_mode_1(0x0000009B, 0x0000008C, g_StringTableCourse[0], 0, 0.9f, 0.9f);
                temp_v1 = GetRecordTime3(0, 7) & 0xFFFFF;
                if (temp_v1 < 0x1EAA) {
                    set_text_color((s32) frame_counter % 2);
                } else if (temp_v1 < 0x2329) {
                    set_text_color((s32) frame_counter % 3);
                } else {
                    set_text_color(TEXT_YELLOW);
                }
                kawano_itoa_m(temp_v1, sp80);
                SetWord4A(0x00000077, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                SetWord2A(0x0000008B, 0x000000A0, "'", 0, 1.0f, 1.0f);
                kawano_itoa_s(temp_v1, sp80);
                SetWord4A(0x00000094, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                SetWord2A(0x000000A7, 0x000000A0, "\"", 0, 1.0f, 1.0f);
                kawano_itoa_c(temp_v1, sp80);
                SetWord4A(0x000000B4, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                break;
            }
            case MENU_ITEM_UI_NO_CONTROLLER: {
                s32 strWidth;
                UNUSED s32 padCont[2];
                strWidth = GetWordLength(no_controller_message[0]);
                temp_v1 = GetWordLength(no_controller_message[1]);
                if (strWidth < temp_v1) {
                    strWidth = temp_v1;
                }
                temp_t2 = (s32) (strWidth * why) / 2;
                temp_t5 = (s32) (((why * 2) + 0.5) * 16.0) / 2;
                gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0xA0 - temp_t2, 0xB6 - temp_t5, temp_t2 + 0xA0,
                                            temp_t5 + 0xB6, 0, 0, 0, 0x00000096);
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
                for (strWidth = 0; strWidth < 2; strWidth++) {
                    print_text1_center_mode_1(0xA0 * one - 1 * why,
                                              (s32) (0xB4 * one + ((f32) (strWidth * 0x12) * why)),
                                              no_controller_message[strWidth], 0, why, why);
                }
                break;
            }
            case MAIN_MENU_BACKGROUND:
            case CHARACTER_SELECT_BACKGROUND:
            case COURSE_SELECT_BACKGROUND:
                gDisplayListHead = doTexList2(gDisplayListHead, gMenuTexturesBackground[Check4GoldCup_150cc()],
                                                 arg0->disp_x, arg0->disp_y, 3, 0);
                break;
            case MENU_ITEM_UI_GAME_SELECT:
                gDisplayListHead =
                    doTexList(gDisplayListHead, seg2_game_select_texture, arg0->disp_x, arg0->disp_y);
                break;
            case MENU_ITEM_UI_1P_GAME:
            case MENU_ITEM_UI_2P_GAME:
            case MENU_ITEM_UI_3P_GAME:
            case MENU_ITEM_UI_4P_GAME:
                var_a1 = arg0->kind - MENU_ITEM_UI_1P_GAME;
                gsel_sub1(var_a1, arg0);
                DispOB_Sub_gsel1(arg0, var_a1);
                break;
            case MENU_ITEM_UI_OK:
                sel_ok_sub(arg0);
                gDisplayListHead =
                    doTexList2(gDisplayListHead, D_0200487C, arg0->disp_x, arg0->disp_y, 2, arg0->work1);
                break;
            case MAIN_MENU_OPTION_GFX:
            case MAIN_MENU_DATA_GFX:
                var_a1 = arg0->kind - 0xF;
                if (arg0->work1 < 0x20) {
                    temp_t9 = (arg0->work1 * 0x3A) / 64;
                    if (var_a1 == menuScreenB) {
                        gDisplayListHead =
                            FlashRectangle2f(gDisplayListHead, arg0->disp_x + temp_t9, (u32) arg0->disp_y,
                                                        (arg0->disp_x - temp_t9) + 0x39, arg0->disp_y + 0x12);
                    } else {
                        gDisplayListHead =
                            FillRect1Color(gDisplayListHead, arg0->disp_x + temp_t9, arg0->disp_y,
                                          (arg0->disp_x - temp_t9) + 0x39, arg0->disp_y + 0x12, 1, 1, 1, 0x000000FF);
                    }
                }
                var_v1 = arg0->kind - 0xA;
                gDisplayListHead =
                    doTexList2(gDisplayListHead, D_800E8254[var_v1], arg0->disp_x, arg0->disp_y, 2, arg0->work1);
                break;
            case MAIN_MENU_50CC:              /* switch 6 */
            case MAIN_MENU_100CC:             /* switch 6 */
            case MAIN_MENU_150CC:             /* switch 6 */
            case MAIN_MENU_EXTRA_CC:          /* switch 6 */
            case MENU_ITEM_TYPE_016:          /* switch 6 */
            case MENU_ITEM_TYPE_017:          /* switch 6 */
            case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 6 */
            case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 6 */
                var_a1 = gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]];
                var_v1 = gGameModePlayerSelection[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]];
                switch (arg0->kind) {        /* switch 5 */
                    case MAIN_MENU_50CC:     /* switch 5 */
                    case MAIN_MENU_100CC:    /* switch 5 */
                    case MAIN_MENU_150CC:    /* switch 5 */
                    case MAIN_MENU_EXTRA_CC: /* switch 5 */
                        switch (var_v1) {
                            case 0:
                            case 2:
                                break;
                            default:
                                var_a1 = -1;
                                break;
                        }

                        var_v1 = MAIN_MENU_50CC;
                        texture = GetTBPointer(D_800E8294[arg0->kind - MAIN_MENU_50CC]);
                        break;
                    case MENU_ITEM_TYPE_016: /* switch 5 */
                    case MENU_ITEM_TYPE_017: /* switch 5 */
                        if (var_v1 != 2) {
                            var_a1 = -1;
                            break;
                        } else {
                            var_v1 = MENU_ITEM_TYPE_016;
                            texture = GetTBPointer(D_800E82A4[arg0->kind - MENU_ITEM_TYPE_016]);
                        }
                        break;
                    case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 5 */
                    case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 5 */
                        if (var_v1 != 1) {
                            var_a1 = -1;
                            break;
                        } else {
                            var_v1 = MAIN_MENU_TIME_TRIALS_BEGIN;
                            texture = GetTBPointer(D_800E82AC[arg0->kind - MAIN_MENU_TIME_TRIALS_BEGIN]);
                        }
                        break;
                    default:
                        break;
                }
                if (var_a1 == -1) {
                    break;
                }
                if ((menuScreenB >= MAIN_MENU_MODE_SUB_SELECT) && (var_a1 == (arg0->kind - var_v1))) {
                    if (menuScreenB > MAIN_MENU_MODE_SUB_SELECT) {
                        gDisplayListHead =
                            FillRect1Color(gDisplayListHead, arg0->disp_x, arg0->disp_y, arg0->disp_x + 0x3F,
                                          arg0->disp_y + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                    } else {
                        gDisplayListHead = FlashRectangle2(
                            gDisplayListHead, arg0->disp_x ^ 0, arg0->disp_y ^ 0, arg0->disp_x + 0x3F, arg0->disp_y + 0x11);
                    }
                } else {
                    gDisplayListHead = FillRect1Color(gDisplayListHead, arg0->disp_x, arg0->disp_y, arg0->disp_x + 0x3F,
                                                     arg0->disp_y + 0x11, 1, 1, 1, 0x000000FF);
                }
                gDisplayListHead = doTexList(gDisplayListHead, texture, arg0->disp_x, arg0->disp_y);
                break;
            case MENU_ITEM_TYPE_01B: /* switch 6 */
                DispOB_Sub_gsel_caution(arg0);
                break;
            case CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER: /* switch 6 */
                gDisplayListHead = doTexList(gDisplayListHead, D_02004B4C, arg0->disp_x, arg0->disp_y);
                break;
            case CHARACTER_SELECT_MENU_1P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_2P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_3P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_4P_CURSOR: /* switch 6 */
                temp_a0 = arg0->kind - CHARACTER_SELECT_MENU_1P_CURSOR;
                if (ok_PSelSkipCursor[temp_a0] == 0) {
                    if (g_player1Character[temp_a0]) {
                        if (gCharacterGridIsSelected[temp_a0] == 0) {
                            temp_t2 = 255;
                        } else {
                            temp_t2 = frame_counter % 16;
                            if (temp_t2 >= 8) {
                                temp_t2 = (-temp_t2 * 8) + 0x80;
                            } else {
                                temp_t2 *= 8;
                            }
                            temp_t2 += 191;
                        }
                        DispOB_Sub_psel_cursor(arg0, temp_a0, temp_t2);
                    }
                }
                break;
            case CHARACTER_SELECT_MENU_OK: /* switch 6 */
                sel_ok_sub(arg0);
                gDisplayListHead =
                    doTexList2(gDisplayListHead, D_02004B74, arg0->disp_x, arg0->disp_y, 2, arg0->work1);
                break;
            case CHARACTER_SELECT_MENU_MARIO:  /* switch 6 */
            case CHARACTER_SELECT_MENU_LUIGI:  /* switch 6 */
            case CHARACTER_SELECT_MENU_TOAD:   /* switch 6 */
            case CHARACTER_SELECT_MENU_PEACH:  /* switch 6 */
            case CHARACTER_SELECT_MENU_YOSHI:  /* switch 6 */
            case CHARACTER_SELECT_MENU_DK:     /* switch 6 */
            case CHARACTER_SELECT_MENU_WARIO:  /* switch 6 */
            case CHARACTER_SELECT_MENU_BOWSER: /* switch 6 */
                if (ok_PSelSkipPortrait[arg0->kind - CHARACTER_SELECT_MENU_MARIO] != 0) {
                    break;
                }
                DispOB_Sub_psel1(arg0, GetTBPointer(SelectNamePlateTable[arg0->kind - 0x2B]));
                /* fallthrough */
            case MENU_ITEM_TYPE_0A0: /* switch 6 */
            case MENU_ITEM_TYPE_0A1: /* switch 6 */
                doSeqData2(arg0->sd_number, arg0->disp_x, arg0->disp_y, arg0->work1);
                break;
            case MENU_ITEM_TYPE_058:         /* switch 6 */
            case COURSE_SELECT_COURSE_NAMES: /* switch 6 */
            case MENU_ITEM_TYPE_05A:         /* switch 6 */
            case MENU_ITEM_TYPE_05B:         /* switch 6 */
            case COURSE_SELECT_BATTLE_NAMES: /* switch 6 */
                msel_sub2(arg0);
                gDisplayListHead = doTexList(
                    gDisplayListHead,
                    GetTBPointer(gMenuTexturesTrackSelection[arg0->kind - COURSE_SELECT_MAP_SELECT]),
                    arg0->disp_x, arg0->disp_y);
                msel_sub4(arg0);
                break;
            case COURSE_SELECT_MAP_SELECT: /* switch 6 */
                gDisplayListHead = doTexList(
                    gDisplayListHead,
                    GetTBPointer(gMenuTexturesTrackSelection[arg0->kind - COURSE_SELECT_MAP_SELECT]),
                    arg0->disp_x, arg0->disp_y);
                break;
            case MENU_ITEM_TYPE_05F: /* switch 6 */
            case MENU_ITEM_TYPE_060: /* switch 6 */
            case MENU_ITEM_TYPE_061: /* switch 6 */
            case MENU_ITEM_TYPE_062: /* switch 6 */
                DispOB_Sub_msel_map(arg0);
                break;
            case COURSE_SELECT_MUSHROOM_CUP: /* switch 6 */
            case COURSE_SELECT_FLOWER_CUP:   /* switch 6 */
            case COURSE_SELECT_STAR_CUP:     /* switch 6 */
            case COURSE_SELECT_SPECIAL_CUP:  /* switch 6 */
                var_a1 = arg0->kind - COURSE_SELECT_MUSHROOM_CUP;
                msel_sub1(var_a1, arg0);
                DispOB_Sub_msel1(arg0, var_a1);
                break;
            case COURSE_SELECT_OK: /* switch 6 */
                sel_ok_sub(arg0);
                gDisplayListHead =
                    doTexList2(gDisplayListHead, D_02004E80, arg0->disp_x, arg0->disp_y, 2, arg0->work1);
                break;
            case MENU_ITEM_TYPE_065: /* switch 6 */
            case MENU_ITEM_TYPE_066: {
                f32 scaleX;
                if (arg0->kind == MENU_ITEM_TYPE_065) {
                    scaleX = 0.6f;
                } else {
                    scaleX = 0.8f;
                }
                msel_bbox_sub(arg0);
                set_text_color(TEXT_YELLOW);
                SetWord2A(arg0->disp_x + 8, arg0->disp_y + 0x10, bestrec_j[arg0->kind - 0x65], 0, scaleX,
                                  0.8f);
                msel_bbox_sub2(arg0);
                break;
            }
            case MENU_ITEM_TYPE_06E: /* switch 6 */
                msel_battle_sub(arg0);
                break;
            case MENU_ITEM_TYPE_067: /* switch 6 */
                DispOB_Sub_msel_cup(arg0);
                break;
            case MENU_ITEM_TYPE_068: /* switch 6 */
                gDisplayListHead = FillRect1Color(gDisplayListHead, arg0->disp_x, arg0->disp_y, arg0->disp_x + 0x3F,
                                                 arg0->disp_y + 0x11, 1, 1, 1, 0x000000FF);
                gDisplayListHead = doTexList(
                    gDisplayListHead, GetTBPointer(D_800E8294[g_raceClass]), arg0->disp_x, arg0->disp_y);
                break;
            case MENU_ITEM_TYPE_069: /* switch 6 */
                DispOB_Sub_msel_ghost(arg0);
                break;
            case MENU_ITEM_TYPE_078: /* switch 6 */
            case MENU_ITEM_TYPE_079: /* switch 6 */
            case MENU_ITEM_TYPE_07A: /* switch 6 */
            case MENU_ITEM_TYPE_07B: /* switch 6 */
                var_a1 = arg0->kind - MENU_ITEM_TYPE_078;
                tsel_sub1(var_a1, arg0);
                DispOB_Sub_msel1(arg0, var_a1);
                break;
            case MENU_ITEM_TYPE_08C: /* switch 6 */
                if ((menuScreenB >= MAIN_MENU_MODE_SUB_SELECT) && (var_a1 == (arg0->kind - var_v1))) {
                    if (menuScreenB > MAIN_MENU_MODE_SUB_SELECT) {
                        gDisplayListHead =
                            FillRect1Color(gDisplayListHead, arg0->disp_x, arg0->disp_y, arg0->disp_x + 0x3F,
                                          arg0->disp_y + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                    } else {
                        gDisplayListHead =
                            FlashRectangle2(gDisplayListHead, arg0->disp_x ^ 0, one = arg0->disp_y ^ 0,
                                                        arg0->disp_x + 0x3F, arg0->disp_y + 0x11);
                    }
                } else {
                    gDisplayListHead = FillRect1Color(gDisplayListHead, arg0->disp_x, arg0->disp_y, arg0->disp_x + 0x3F,
                                                     arg0->disp_y + 0x11, 1, 1, 1, 0x000000FF);
                }
                gDisplayListHead = doTexList(gDisplayListHead, seg2_data_texture, arg0->disp_x, arg0->disp_y);
                set_text_color(TEXT_YELLOW);
                SetWord3A(0x00000125, 0x0000001C, tsel_title_j, 0, 0.55f, 0.55f);
                break;
            case MENU_ITEM_TYPE_08D: /* switch 6 */
                DispOB_Sub_tsel_cursor(arg0);
                break;
            case MENU_ITEM_TYPE_07C: /* switch 6 */
            case MENU_ITEM_TYPE_07D: /* switch 6 */
            case MENU_ITEM_TYPE_07E: /* switch 6 */
            case MENU_ITEM_TYPE_07F: /* switch 6 */
            case MENU_ITEM_TYPE_080: /* switch 6 */
            case MENU_ITEM_TYPE_081: /* switch 6 */
            case MENU_ITEM_TYPE_082: /* switch 6 */
            case MENU_ITEM_TYPE_083: /* switch 6 */
            case MENU_ITEM_TYPE_084: /* switch 6 */
            case MENU_ITEM_TYPE_085: /* switch 6 */
            case MENU_ITEM_TYPE_086: /* switch 6 */
            case MENU_ITEM_TYPE_087: /* switch 6 */
            case MENU_ITEM_TYPE_088: /* switch 6 */
            case MENU_ITEM_TYPE_089: /* switch 6 */
            case MENU_ITEM_TYPE_08A: /* switch 6 */
            case MENU_ITEM_TYPE_08B: /* switch 6 */
                DispOB_Sub_tsel1(arg0);
                break;
            case MENU_ITEM_TYPE_096: /* switch 6 */
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
                SetWord3A(arg0->disp_x, arg0->disp_y, cup_name[cup_level], arg0->work1, arg0->workf1, 1.0f);
                break;
            case MENU_ITEM_TYPE_097: /* switch 6 */
                set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
                SetWord2A(arg0->disp_x, arg0->disp_y, g_StringTableCourseGP[g_courseID], arg0->work1, arg0->workf1,
                                  1.0f);
                break;
            case MENU_ITEM_TYPE_098: /* switch 6 */
                Cut_ud_sub(arg0);
                break;
            case MENU_ITEM_TYPE_05E: /* switch 6 */
                gDisplayListHead = AutoTextureRect_noise(gDisplayListHead, 0x00000019, 0x00000072, 0x0000007CU, 0x0000004AU);
                break;
            case MENU_ITEM_TYPE_0AA: /* switch 6 */
                DispOB_result_time(arg0);
                break;
            case MENU_ITEM_TYPE_0AB: /* switch 6 */
                DispOB_result_point(arg0);
                break;
            case MENU_ITEM_TYPE_0AC: /* switch 6 */
                DispOB_gp_lose(arg0);
                break;
            case MENU_ITEM_TYPE_0AF: /* switch 6 */
                DispOB_info_3p(arg0);
                break;
            case MENU_ITEM_TYPE_0B0: /* switch 6 */
                DispOB_vs_result(arg0);
                break;
            case MENU_ITEM_TYPE_0B1: /* switch 6 */
            case MENU_ITEM_TYPE_0B2: /* switch 6 */
            case MENU_ITEM_TYPE_0B3: /* switch 6 */
            case MENU_ITEM_TYPE_0B4: /* switch 6 */
                if (arg0->pattern != 0) {
                    var_v1 = arg0->kind - MENU_ITEM_TYPE_0B1;
                    one = driver2pselset[g_CharacterSelections[var_v1]];
                    gDisplayListHead = doTexList(
                        gDisplayListHead, GetTBPointer(SelectNamePlateTable[one]), arg0->disp_x, arg0->disp_y);
                    doSeqData2_vs(arg0->sd_number, arg0->disp_x, arg0->disp_y, var_v1, arg0->work1);
                    DispOB_Sub_psel_cursor(arg0, var_v1, 0x000000FF);
                }
                break;
            case MENU_ITEM_TYPE_0B9: /* switch 6 */
                DispOB_timeattack1(arg0);
                break;
            case MENU_ITEM_TYPE_0BA: /* switch 6 */
                DispOB_timeattack2(arg0);
                break;
            case MENU_ITEM_ANNOUNCE_GHOST: /* switch 6 */
                DispOB_timeattack_message1(arg0);
                break;
            case MENU_ITEM_PAUSE: /* switch 6 */
                DispOB_pause(arg0);
                break;
            case MENU_ITEM_END_COURSE_OPTION: /* switch 6 */
                DispOB_ta_replay(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_IMAGE: /* switch 6 */
                DispOB_Sub_dsel_snap(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_INFO: /* switch 6 */
                DispOB_Sub_dsel_mes1(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_SELECTABLE: /* switch 6 */
                DispOB_Sub_dsel_sel1(arg0);
                break;
            case MENU_ITEM_TYPE_0E9: /* switch 6 */
                DispOB_Sub_dsel_sel2(arg0);
                break;
            case MENU_ITEM_TYPE_0EA: /* switch 6 */
                DispOB_Sub_dsel_sel3(arg0);
                break;
            case MENU_ITEM_TYPE_0F0: /* switch 6 */
                DispOB_Sub_option_messages(arg0);
                break;
            case MENU_ITEM_TYPE_0F1: /* switch 6 */
                gDisplayListHead = doTexList(gDisplayListHead, D_02004638, arg0->disp_x, arg0->disp_y);
                break;
            case MENU_ITEM_TYPE_10E: /* switch 6 */
                DispOB_ta_lostghost(arg0);
                break;
            case MENU_ITEM_TYPE_12B: /* switch 6 */
                DispOB_ending1_halffade(arg0);
                break;
            case MENU_ITEM_TYPE_12C: /* switch 6 */
                DispOB_ending1_message1(arg0);
                break;
            case MENU_ITEM_TYPE_12D: /* switch 6 */
                DispOB_ending1_message2(arg0);
                break;
            case MENU_ITEM_TYPE_12E: /* switch 6 */
                DispOB_ending1_message3(arg0);
                break;
            case MENU_ITEM_TYPE_12F: /* switch 6 */
                DispOB_ending1_message4(arg0);
                break;
            case MENU_ITEM_TYPE_130: /* switch 6 */
                if (arg0->pattern != 0) {
                    var_a1 = driver2pselset[resultstat.kart];
                    gDisplayListHead = doTexList(
                        gDisplayListHead, GetTBPointer(SelectNamePlateTable[var_a1]), arg0->disp_x, arg0->disp_y);
                    doSeqData2_vs(arg0->sd_number, arg0->disp_x, arg0->disp_y, 0, arg0->work1);
                }
                break;
            case MENU_ITEM_TYPE_190: /* switch 6 */
            case MENU_ITEM_TYPE_191: /* switch 6 */
            case MENU_ITEM_TYPE_192: /* switch 6 */
            case MENU_ITEM_TYPE_193: /* switch 6 */
            case MENU_ITEM_TYPE_194: /* switch 6 */
            case MENU_ITEM_TYPE_195: /* switch 6 */
            case MENU_ITEM_TYPE_196: /* switch 6 */
            case MENU_ITEM_TYPE_197: /* switch 6 */
            case MENU_ITEM_TYPE_198: /* switch 6 */
            case MENU_ITEM_TYPE_199: /* switch 6 */
            case MENU_ITEM_TYPE_19A: /* switch 6 */
            case MENU_ITEM_TYPE_19B: /* switch 6 */
            case MENU_ITEM_TYPE_19C: /* switch 6 */
            case MENU_ITEM_TYPE_19D: /* switch 6 */
            case MENU_ITEM_TYPE_19E: /* switch 6 */
            case MENU_ITEM_TYPE_19F: /* switch 6 */
            case MENU_ITEM_TYPE_1A0: /* switch 6 */
            case MENU_ITEM_TYPE_1A1: /* switch 6 */
            case MENU_ITEM_TYPE_1A2: /* switch 6 */
            case MENU_ITEM_TYPE_1A3: /* switch 6 */
            case MENU_ITEM_TYPE_1A4: /* switch 6 */
            case MENU_ITEM_TYPE_1A5: /* switch 6 */
            case MENU_ITEM_TYPE_1A6: /* switch 6 */
            case MENU_ITEM_TYPE_1A7: /* switch 6 */
            case MENU_ITEM_TYPE_1A8: /* switch 6 */
            case MENU_ITEM_TYPE_1A9: /* switch 6 */
            case MENU_ITEM_TYPE_1AA: /* switch 6 */
            case MENU_ITEM_TYPE_1AB: /* switch 6 */
            case MENU_ITEM_TYPE_1AC: /* switch 6 */
            case MENU_ITEM_TYPE_1AD: /* switch 6 */
            case MENU_ITEM_TYPE_1AE: /* switch 6 */
            case MENU_ITEM_TYPE_1AF: /* switch 6 */
            case MENU_ITEM_TYPE_1B0: /* switch 6 */
            case MENU_ITEM_TYPE_1B1: /* switch 6 */
            case MENU_ITEM_TYPE_1B2: /* switch 6 */
            case MENU_ITEM_TYPE_1B3: /* switch 6 */
            case MENU_ITEM_TYPE_1B4: /* switch 6 */
            case MENU_ITEM_TYPE_1B5: /* switch 6 */
            case MENU_ITEM_TYPE_1B6: /* switch 6 */
            case MENU_ITEM_TYPE_1B7: /* switch 6 */
            case MENU_ITEM_TYPE_1B8: /* switch 6 */
            case MENU_ITEM_TYPE_1B9: /* switch 6 */
            case MENU_ITEM_TYPE_1BA: /* switch 6 */
            case MENU_ITEM_TYPE_1BB: /* switch 6 */
            case MENU_ITEM_TYPE_1BC: /* switch 6 */
            case MENU_ITEM_TYPE_1BD: /* switch 6 */
            case MENU_ITEM_TYPE_1BE: /* switch 6 */
            case MENU_ITEM_TYPE_1BF: /* switch 6 */
            case MENU_ITEM_TYPE_1C0: /* switch 6 */
            case MENU_ITEM_TYPE_1C1: /* switch 6 */
            case MENU_ITEM_TYPE_1C2: /* switch 6 */
            case MENU_ITEM_TYPE_1C3: /* switch 6 */
            case MENU_ITEM_TYPE_1C4: /* switch 6 */
            case MENU_ITEM_TYPE_1C5: /* switch 6 */
            case MENU_ITEM_TYPE_1C6: /* switch 6 */
            case MENU_ITEM_TYPE_1C7: /* switch 6 */
            case MENU_ITEM_TYPE_1C8: /* switch 6 */
            case MENU_ITEM_TYPE_1C9: /* switch 6 */
            case MENU_ITEM_TYPE_1CA: /* switch 6 */
            case MENU_ITEM_TYPE_1CB: /* switch 6 */
            case MENU_ITEM_TYPE_1CC: /* switch 6 */
            case MENU_ITEM_TYPE_1CD: /* switch 6 */
            case MENU_ITEM_TYPE_1CE: /* switch 6 */
                DispOB_staffroll_messages(arg0);
                break;
        }
    }
}

void Set_cp_font(u8 arg0, s32 column, s32 row) {
    if (arg0 >= 0x10) {
        arg0 -= 0x10;
        if (arg0 < 0x85) {
            if (arg0 >= 0x32) {
                arg0 = 0x2B;
            }
            gDisplayListHead =
                doTexList(gDisplayListHead, GetTBPointer(D_800E7AF8[arg0]), column, row);
        }
    }
}

// Walks through `someString` for `len` charcters, seemingly even going over null terminators.
// Returns a count of all non-terminator characters walked over.
s32 Set_cp_gnote_name(char* someString, s32 len, s32 column, s32 row) {
    s32 tempColumn;
    s32 nonTerminatorCount;

    nonTerminatorCount = 0;
    tempColumn = column;
    for (; len != 0; len--, tempColumn += 8) {
        if (*someString != 0) {
            nonTerminatorCount++;
        }
        Set_cp_font(*someString++, tempColumn, row);
    }
    return nonTerminatorCount;
}

void DispOB_cp_gnote(ObjBlock* arg0) {
    s32 table_row;

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH - 1, 194);
    for (table_row = 0; table_row < 9; table_row++) {
        if (gControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE && (table_row == 0 || table_row == 8)) {
            continue;
        }
        gDisplayListHead = doTexList(gDisplayListHead, D_0200157C, 0x20, (table_row * 0xA) + arg0->disp_y);
    }
}

void DispOB_cp_notecursor(UNUSED ObjBlock* arg0) {
    ObjBlock* temp_t1;
    // Find ObjBlock with a type/id of 0xDA
    temp_t1 = Get_OBJBlock_ptr(MENU_ITEM_TYPE_0DA);
    if ((gControllerPakMenuSelection != CONTROLLER_PAK_MENU_SELECT_RECORD) &&
        (gControllerPakMenuSelection != CONTROLLER_PAK_MENU_END)) {
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, temp_t1->work2, 0x00, 0xFF);
        gDisplayListHead =
            doTexList(gDisplayListHead, D_02001874, 0x24, (gControllerPakSelectedTableRow * 0xA) + 0x7C);
    }
}

void DispOB_cp_gnote_name(ObjBlock* arg0) {
    UNUSED s32 temp_a2;
    s32 temp_s1;
    s32 temp_s2;
    s32 var_s1;
    s32 var_s5;
    s32 var_s0;
    OSPfsState* temp_s4;

    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x32, 0xFF);
    for (var_s5 = 0; var_s5 < 9; var_s5++) {
        if (gControllerPakVisibleTableRows[var_s5] == 0) {
            continue;
        }

        temp_s1 = var_s5 * 0xA;
        var_s0 = gControllerPakVisibleTableRows[var_s5];
        if (var_s0 < 0xA) {
            Set_cp_font(var_s0 + 0x10, 0x00000032, arg0->disp_y + temp_s1 + 1);
        } else {
            var_s0 %= 10;
            Set_cp_font(var_s0 + 0x10, 0x00000035, arg0->disp_y + temp_s1 + 1);
            Set_cp_font(0x11U, 0x0000002F, arg0->disp_y + temp_s1 + 1);
        }
        temp_s2 = arg0->disp_y + temp_s1 + 1;
        if (pfsError[gControllerPakVisibleTableRows[var_s5] - 1] == 0) {
            temp_s4 = &pfsState[gControllerPakVisibleTableRows[var_s5] - 1];
            var_s0 = Set_cp_gnote_name(temp_s4->game_name, 0x00000010, 0x0000004F, temp_s2);
            if (temp_s4->ext_name[0] != 0) {
                Set_cp_font(0x3CU, (var_s0 * 8) + 0x4F, temp_s2);
                Set_cp_font(temp_s4->ext_name[0], (var_s0 * 8) + 0x57, temp_s2);
            }
            var_s1 = 0x10;
            var_s0 = (temp_s4->file_size + 0xFF) >> 8;
            // An actual do-while loop, a rare beast in these parts
            do {
                Set_cp_font(((var_s0 % 10) + 0x10), var_s1 + 0xFD, temp_s2);
                var_s0 /= 10;
                var_s1 -= 8;
            } while (var_s0 != 0);
        }
    }
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
}

void DispOB_cp_free(void) {
    s32 temp_t6;
    s32 var_s0;
    s32 var_s1;

    var_s0 = gControllerPak1NumPagesFree;
    var_s1 = 0x00000110;
    do {
        temp_t6 = var_s0 % 10;
        var_s0 /= 10;
        gDisplayListHead =
            doTexList(gDisplayListHead, GetTBPointer(D_800E7D0C[temp_t6]), var_s1, 0x000000B8);
        var_s1 -= 9;
    } while (var_s0 != 0);
}

void DispOB_cp_cursor(UNUSED ObjBlock* arg0, s32 arg1) {
    s32 var_t1;
    s32 thing;
    POSXY* temp_v0;
    ObjBlock* temp_t3;

    temp_t3 = Get_OBJBlock_ptr(MENU_ITEM_TYPE_0DA);
    if (arg1 == 0) {
        if (gControllerPakMenuSelection == CONTROLLER_PAK_MENU_END) {
            var_t1 = 1;
        } else {
            var_t1 = 0;
        }
    } else {
        thing = gControllerPakMenuSelection; // ?
        if ((thing == CONTROLLER_PAK_MENU_ERASE) || (thing == CONTROLLER_PAK_MENU_QUIT)) {
            var_t1 = ((arg1 * 2) + gControllerPakMenuSelection) - CONTROLLER_PAK_MENU_ERASE;
        } else {
            return;
        }
    }
    temp_v0 = &cp_cursor_position[var_t1];
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0xFF, temp_t3->work2, 0x00, 0xFF);
    gDisplayListHead = doTexList(gDisplayListHead, D_0200184C, (s32) temp_v0->x, (s32) temp_v0->y);
}

void DispOB_Sub_gsel1(ObjBlock* arg0, s32 arg1) {
    switch (arg0->pattern) {
        case 0:
        case 2:
        case 3:
            gDisplayListHead = doTexList(
                gDisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 0]), arg0->disp_x, arg0->disp_y);
            gDisplayListHead = doTexList(
                gDisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 1]), arg0->disp_x, arg0->disp_y);
            break;
        case 1:
        case 4:
            gDisplayListHead = doTexList2(gDisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 0]),
                                             arg0->disp_x, arg0->disp_y, 2, arg0->work1);
            gDisplayListHead = doTexList2(gDisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 1]),
                                             arg0->disp_x, arg0->disp_y, 2, arg0->work1);
            break;
    }
}

void DispOB_Sub_gsel_caution(ObjBlock* arg0) {
    s32 var_s1;
    s32 index;

    switch (arg0->pattern) {
        case 2:
        case 3:
        case 4:
        case 5:
            gDisplayListHead =
                FillRect1Color(gDisplayListHead, 0x0000001E, 0x00000032, 0x00000122, 0x0000006E, 0, 0, 0, 0x000000FF);
            index = arg0->pattern - 2;
            set_text_color(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                // In a perfect world this would be `gsel_caution_messages_j[index][var_s1]`
                SetWord2A(0x00000023, 0x41 + (0xD * var_s1), gsel_caution_messages_j[(index * 4) + var_s1], 0, 0.65f, 0.65f);
            }
            break;
        default:
            break;
    }
}

void DispOB_Sub_psel_cursor(ObjBlock* arg0, s32 arg1, s32 arg2) {
    RGBA16* temp_v1;

    temp_v1 = &psel_cursor_color[arg1];
    gDPSetPrimColor(gDisplayListHead++, 0, 0, temp_v1->red, temp_v1->green, temp_v1->blue, temp_v1->alpha);
    gDPSetEnvColor(gDisplayListHead++, arg2, arg2, arg2, 0x00);
    gDisplayListHead = doTexList(
        gDisplayListHead, GetTBPointer(gMenuTexturesBorderPlayer[arg1]), arg0->disp_x, arg0->disp_y);
}

void DispOB_Sub_psel1(ObjBlock* arg0, MenuTexture* arg1) {
    switch (arg0->pattern) {
        case 0:
        case 2:
        case 4:
            gDisplayListHead = doTexList(gDisplayListHead, arg1, arg0->disp_x, arg0->disp_y);
            break;
        case 1:
        case 3:
            gDisplayListHead = doTexList2(gDisplayListHead, arg1, arg0->disp_x, arg0->disp_y, 2, arg0->work1);
            break;
    }
}

void DispOB_Sub_psel2(ObjBlock* arg0) {
    s32 thing;
    if (Get_psel_cursor_number(arg0->kind - 0x2B) < 0) {
        switch (arg0->pattern) {
            case 0:
            case 2:
            case 4:
                gDisplayListHead = FillRect1ColorF(gDisplayListHead, arg0->disp_x, arg0->disp_y, arg0->disp_x + 0x40,
                                            arg0->disp_y + 0x4C, 0, 0, 0, 0x00000064);
                break;
            case 1:
            case 3:
                thing = arg0->work1;
                gDisplayListHead = FillRect1ColorF(gDisplayListHead, arg0->disp_x + thing, arg0->disp_y,
                                            (arg0->disp_x - thing) + 0x40, arg0->disp_y + 0x4C, 0, 0, 0, 0x00000064);
                break;
        }
    }
}

void DispOB_Sub_msel1(ObjBlock* arg0, s32 arg1) {
    switch (arg0->pattern) {
        case 0:
        case 2:
        case 3:
            gDisplayListHead =
                doTexList(gDisplayListHead, GetTBPointer(gMenuTexturesTrackSelection[arg1 + 1]),
                                     arg0->disp_x, arg0->disp_y);
            break;
        case 1:
        case 4:
            gDisplayListHead =
                doTexList2(gDisplayListHead, GetTBPointer(gMenuTexturesTrackSelection[arg1 + 1]),
                              arg0->disp_x, arg0->disp_y, 2, arg0->work1);
            break;
    }
}

void DispOB_Sub_msel_map(ObjBlock* arg0) {
    ObjBlock* temp_v0;
    POSXY* temp_v0_2;
    s32 var_a1;

    var_a1 = 0;
    temp_v0 = Get_OBJBlock_ptr(MENU_ITEM_TYPE_064);
    switch (temp_v0->pattern) { /* irregular */
        case 0:
        case 1:
            break;
        case 2:
            if (((temp_v0->work1 % 4) + 0x5F) != arg0->kind) {
                var_a1 = 1;
            }
            break;
        case 3:
            var_a1 = 1;
            break;
    }
    switch (var_a1) {
        case 0:
            doSeqData2(arg0->sd_number, 0x00000017, 0x00000070, -1);
            break;
        case 1:
            temp_v0_2 = &msel_map4box_position[arg0->kind - 0x5F];
            doSeqData2(arg0->sd_number, temp_v0_2->x, temp_v0_2->y, -2);
            break;
    }
}

void DispOB_Sub_tsel1(ObjBlock* arg0) {
    s16 courseId = gCupCourseOrder[(arg0->kind - 0x7C) / 4][(arg0->kind - 0x7C) % 4];
    gDisplayListHead =
        doTexList5(gDisplayListHead, GetTBPointer(D_800E7D74[courseId]), arg0->disp_x, arg0->disp_y, 2);
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, arg0->disp_x, arg0->disp_y + 0x27, arg0->disp_x + 0x40, arg0->disp_y + 0x30,
                                0, 0, 0, 0xFF);
    gDisplayListHead = doTexList5(gDisplayListHead, GetTBPointer(D_800E7DC4[courseId]), arg0->disp_x,
                                     arg0->disp_y + 0x27, 3);
    if (CheckSameMap2(arg0->kind - 0x7C) >= 0) {
        // The "^ 0" is required to force the use of v1 instead of a 4th s* register
        gDisplayListHead = FlashRectangle2(gDisplayListHead, arg0->disp_x + 0x20, arg0->disp_y ^ 0,
                                                       arg0->disp_x + 0x3F, arg0->disp_y + 9);
        gDisplayListHead =
            doTexList5(gDisplayListHead, GetTBPointer(&D_02004A0C), arg0->disp_x + 0x20, arg0->disp_y, 2);
    }
}

void DispOB_Sub_tsel_cursor(ObjBlock* arg0) {
    RGBA16* temp_a1;
    RGBA16* temp_v1;
    s32 temp_a2;
    u32 red;
    u32 green;
    u32 blue;
    u32 alpha;

    temp_v1 = &tsel_cursor_color[arg0->work2];
    temp_a1 = &tsel_cursor_color[(arg0->work2 + 1) % 3];
    temp_a2 = 256 - arg0->work1;
    red = ((temp_v1->red * temp_a2) + (temp_a1->red * arg0->work1)) / 256;
    green = ((temp_v1->green * temp_a2) + (temp_a1->green * arg0->work1)) / 256;
    blue = ((temp_v1->blue * temp_a2) + (temp_a1->blue * arg0->work1)) / 256;
    alpha = ((temp_v1->alpha * temp_a2) + (temp_a1->alpha * arg0->work1)) / 256;
    gDPSetPrimColor(gDisplayListHead++, 0, 0, red, green, blue, alpha);
    gDisplayListHead =
        doTexList(gDisplayListHead, GetTBPointer(D_02001FA4), arg0->disp_x, arg0->disp_y);
}

void DispOB_Sub_dsel_snap(ObjBlock* arg0) {
    // render course preview
    doSeqData2(arg0->sd_number, 0x17, 0x84, -1);
    if (CheckSameMap2(gTimeTrialDataCourseIndex) >= TIME_TRIAL_DATA_LUIGI_RACEWAY) {
        gDisplayListHead = FlashRectangle2(gDisplayListHead, 0x57, 0x84, 0x96, 0x95);
        gDisplayListHead = doTexList(gDisplayListHead, D_02004A0C, 0x57, 0x84);
    }
    // course minimap
    kwdisplay_kawanoradarmap(gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4]);
    do {
        gDPSetTextureFilter(gDisplayListHead++, G_TF_BILERP);
    } while (0);
}

void DispOB_Sub_dsel_mes1(ObjBlock* arg0) {
    s16 courseId;
    s32 recordType;
    s32 rowOffset;

    courseId = gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4];
    arg0->disp_x = 0x14;
    // name of the course
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    print_text1_center_mode_1(0x69, arg0->disp_y + 0x19, g_StringTableCourse[courseId], 0, 0.75f, 0.75f);

    // distance
    set_text_color(TEXT_RED);
    SetWord2A(0x2D, arg0->disp_y + 0x28, (char*) &dsel_length_j, 0, 0.75f, 0.75f);
    SetWord3A(0xA5, arg0->disp_y + 0x28, dsel_lengthdata_j[courseId], 1, 0.75f, 0.75f);

    // best lap record
    set_text_color(TEXT_YELLOW);
    SetWord2A(0xA0, arg0->disp_y + 0x86, bestrec_j[0], 0, 0.75f, 0.75f);
    // Print the 3 Lap Time Trial records
    for (recordType = TIME_TRIAL_3LAP_RECORD_1, rowOffset = 0; recordType < TIME_TRIAL_1LAP_RECORD;
         recordType++, rowOffset += 0xD) {
        set_text_color(TEXT_RED);
        TimeAttack1Sub2(recordType, 0x96, arg0->disp_y + rowOffset + 0x92);
    }
    set_text_color(TEXT_YELLOW);
    SetWord2A(0xA0, arg0->disp_y + 0xD5, bestrec_j[1], 0, 0.75f, 0.75f);
    TimeAttack1Sub2(TIME_TRIAL_1LAP_RECORD, 0x96, arg0->disp_y + 0xE1);
}

void DispOB_Sub_dsel_sel1(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED char* wut;
    POSXY sp78;
    s32 i;
    s32 var_s1;
    s32 var_s2;
    UNUSED s32 thing;
    CourseTimeTrialRecords* temp_s6;

    temp_s6 = &eeprom_save_data.allCourseTimeTrialRecords.cupRecords[gTimeTrialDataCourseIndex / 4]
                   .courseRecords[gTimeTrialDataCourseIndex % 4];
    for (i = 0; i < ARRAY_COUNT(dsel_select1_j); i++) {
        wut = dsel_select1_j[i];
        var_s1 = 0;
        if (i == gCourseRecordsMenuSelection) {
            var_s2 = TEXT_BLUE_GREEN_RED_CYCLE_2;
        } else {
            var_s2 = TEXT_GREEN;
            switch (i) { /* irregular */
                case COURSE_RECORDS_MENU_ERASE_RECORDS:
                    if (temp_s6->unknownBytes[0] == 0) {
                        var_s1 = 1;
                    }
                    break;
                case COURSE_RECORDS_MENU_ERASE_GHOST:
                    if (CheckSameMap2((s32) gTimeTrialDataCourseIndex) < 0) {
                        var_s1 = 1;
                    }
                    break;
            }
        }
        if (var_s1 != 0) {
            set_text_color(TEXT_BLUE);
            gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0x96);
            SetWord2AP(0x00000025, 0x3F + (0xD * i), dsel_select1_j[i], 0, 0.6f, 0.6f);
        } else {
            set_text_color(var_s2);
            SetWord2A(0x00000025, 0x3F + (0xD * i), dsel_select1_j[i], 0, 0.6f, 0.6f);
        }
    }
    sp78.x = 0x001F;
    sp78.y = (gCourseRecordsMenuSelection * 0xD) + 0x3A;
    DispOB_vs_ibox_cursor(arg0, (POSXY*) &sp78);
}

void DispOB_Sub_dsel_sel2(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    POSXY sp58;
    s32 var_a0;
    UNUSED s32 var_s0;
    s32 var_s1;
    UNUSED char* wut;

    set_text_color(TEXT_GREEN);
    for (var_s1 = 0; var_s1 < 3; var_s1++) {
        // Removing `wut` introduces counter intuitive changes to how this loop is handled
        // Also, in a perfect world this would be `dsel_select2_messages_j[gCourseRecordsMenuSelection - 1][var_s1]`
        wut = dsel_select2_messages_j[(gCourseRecordsMenuSelection - 1) * 3 + var_s1];
        SetWord2A(0x0000001B, 0x3C + (0xD * var_s1),
                          dsel_select2_messages_j[(gCourseRecordsMenuSelection - 1) * 3 + var_s1], 0, 0.65f, 0.65f);
    }

    for (var_s1 = 0; var_s1 < ARRAY_COUNT(dsel_select2_j); var_s1++) {
        wut = dsel_select2_j[var_s1];
        if (var_s1 == gCourseRecordsSubMenuSelection) {
            var_a0 = 5;
        } else {
            var_a0 = 1;
        }
        set_text_color(var_a0);
        SetWord2A(0x00000043, 0x6E + (0xD * var_s1), dsel_select2_j[var_s1], 0, 0.65f, 0.65f);
    }

    sp58.x = 0x003B;
    sp58.y = (gCourseRecordsSubMenuSelection * 0xD) + 0x66;
    DispOB_vs_ibox_cursor(arg0, &sp58);
}

void DispOB_Sub_dsel_sel3(UNUSED ObjBlock* unused) {
    s32 row;
    s32 text;

    set_text_color(TEXT_RED);
    for (row = 0x49, text = 0; row < 0x69; row += 0x10, text++) {
        SetWord2A(0x2A, row, dsel_select3_messages_j[text], 0, 0.75f, 0.75f);
    }
}

void DispOB_Sub_option_messages(ObjBlock* arg0) {
    POSXY spE0;
    s32 var_s1;
    UNUSED s32 pad[2];
    UNUSED s32 temp;
    UNUSED s32 pad2[2];
    s32 var_s5;
    s32 var_s4;
    s32 j;
    char spB8[3];
    UNUSED s32 pad3[2];
    s32 i;
    char spA8[3];
    UNUSED s32 pad4[3];
    char sp98[3];
    struct_8018EE10_entry* var_v1;

    gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, 0x00000064);
    switch (menuScreenA) {
        case SUB_MENU_OPTION_RETURN_GAME_SELECT:
        case SUB_MENU_OPTION_SOUND_MODE:
        case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
        case SUB_MENU_OPTION_ERASE_ALL_DATA:
            for (i = 0; i < ARRAY_COUNT(option_select1_messages_j); i++) {
                SetCursorMessageColor4Option(menuScreenA - SUB_MENU_OPTION_MIN, i, 3);
                SetWord2A(0x00000032, 0x55 + (0x23 * i), option_select1_messages_j[i], 0, 0.9f, 1.0f);
                if (i == (menuScreenA - SUB_MENU_OPTION_MIN)) {
                    spE0.x = 0x0032;
                    spE0.y = 0x55 + (0x23 * i);
                }
            }
            set_text_color(TEXT_GREEN);
            print_text1_center_mode_1(0x000000E6, 0x55 + 0x23, sound_mode_name_j[sound_output], 0, 1.0f, 1.0f);
            break;
        case SUB_MENU_ERASE_QUIT:
        case SUB_MENU_ERASE_ERASE:
            set_text_color(TEXT_YELLOW);
            for (i = 0; i < ARRAY_COUNT(option_select3_messages_j); i++) {
                SetWord2A(0x00000028, 0x55 + (0x14 * i), option_select3_messages_j[i], 0, 1.0f, 1.0f);
            }
            for (i = 0; i < ARRAY_COUNT(dsel_select2_j); i++) {
                SetCursorMessageColor4Option(menuScreenA - SUB_MENU_ERASE_MIN, i, 1);
                SetWord2A(0x00000084, 0x96 + (0x19 * i), dsel_select2_j[i], 0, 1.0f, 1.0f);
                if (i == (menuScreenA - SUB_MENU_ERASE_MIN)) {
                    spE0.x = 0x0084;
                    spE0.y = 0x96 + (0x19 * i);
                }
            }
            break;
        case SUB_MENU_SAVE_DATA_ERASED:
            set_text_color(TEXT_YELLOW);
            for (i = 0; i < ARRAY_COUNT(option_select3e_messages_j); i++) {
                SetWord2A(0x00000032, 0x55 + (0x14 * i), option_select3e_messages_j[i], 0, 1.0f, 1.0f);
            }
            break;
        case SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA:
        case SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA:
        case SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P:
        case SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P:
            set_text_color(TEXT_RED);
            var_s1 = menuScreenA - SUB_MENU_COPY_PAK_ERROR_2P_MIN;
            for (i = 0; i < ARRAY_COUNT(option_copy_pak2err_messages_j) / 4; i++) { // 12 / 4 = 3
                SetWord2A(0x00000032, 0x55 + (0x14 * i), option_copy_pak2err_messages_j[(var_s1 * 3) + i], 0, 0.9f, 0.9f);
            }
            break;
        case SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P:
        case SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P:
        case SUB_MENU_COPY_PAK_ERROR_CANT_CREATE_1P:
        case SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P:
            j++;
            j--; // FAKE
            set_text_color(TEXT_RED);
            var_s1 = menuScreenA - SUB_MENU_COPY_PAK_ERROR_1P_MIN;
            for (i = 0; i < ARRAY_COUNT(option_copy_pak1err_messages_j) / 4; i++) { // 16 / 4 = 4
                SetWord2A(0x00000023, 0x55 + (0x14 * i), option_copy_pak1err_messages_j[(var_s1 * 4) + i], 0, 0.8f, 0.8f);
            }
            break;
        case SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P:
        case SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P:
            set_text_color(TEXT_RED);
            var_s1 = menuScreenA - SUB_MENU_COPY_PAK_UNABLE_ERROR_MIN;
            for (i = 0; i < ARRAY_COUNT(option_copy_copyerr_messages_j) / 2; i++) { // 6 / 2 = 3
                SetWord2A(0x00000041, 0x55 + (0x14 * i), option_copy_copyerr_messages_j[(var_s1 * 3) + i], 0, 0.9f, 0.9f);
            }
            break;
        case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT:
        case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE:
            set_text_color(TEXT_YELLOW);
            for (i = 0; i < ARRAY_COUNT(ta_ghostb3_messages_j); i++) {
                SetWord2A(0x00000050, 0x55 + (0x14 * i), ta_ghostb3_messages_j[i], 0, 1.0f, 1.0f);
            }
            break;
        case SUB_MENU_COPY_PAK_FROM_GHOST1_1P:
        case SUB_MENU_COPY_PAK_FROM_GHOST2_1P:
        case SUB_MENU_COPY_PAK_TO_GHOST1_2P:
        case SUB_MENU_COPY_PAK_TO_GHOST2_2P:
            switch (menuScreenA) {
                case SUB_MENU_COPY_PAK_FROM_GHOST1_1P:
                case SUB_MENU_COPY_PAK_FROM_GHOST2_1P:
                    var_s5 = SUB_MENU_COPY_PAK_FROM_GHOST_MIN;
                    var_s4 = 0;
                    break;
                case SUB_MENU_COPY_PAK_TO_GHOST1_2P:
                case SUB_MENU_COPY_PAK_TO_GHOST2_2P:
                    var_s5 = SUB_MENU_COPY_PAK_TO_GHOST_MIN;
                    var_s4 = 1;
                default:
                    break;
            }
            temp = var_s4; // only semi-fake
            set_text_color(temp + 1);
            print_text1_center_mode_1(0x000000A0, 0x00000055, option_copy_pak2sel_message_j[temp], 0, 0.6f, 0.6f);
            for (i = 0; i < ARRAY_COUNT(option_copy_pak_j); i++) {
                set_text_color(TEXT_YELLOW);
                print_text1_center_mode_1(0x5C + (0x82 * i), 0x0000007D, option_copy_pak_j[i], 0, 0.75f, 0.75f);
                for (j = 0; j < 2; j++) {
                    if (i != temp) {
                        SetCursorMessageColor(menuScreenA - var_s5, j, TEXT_GREEN);
                        if (j == (menuScreenA - var_s5)) {
                            spE0.x = 0x20 + (0x89 * i);
                            spE0.y = 0x96 + (0x1E * j);
                        }
                    } else if ((temp != 0) && (j == arg0->work2)) {
                        set_text_color((s32) frame_counter % 3);
                    } else {
                        set_text_color(TEXT_GREEN);
                    }
                    kawano_itoa(j + 1, &spB8[0]);
                    SetWord2A(0x20 + (0x89 * i), 0x96 + (0x1E * j), &spB8[1], 0, 0.6f, 0.6f);
                    if (i == 0) {
                        var_v1 = &D_8018EE10[j];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) kdl_buff)[j];
                    }
                    if (var_v1->ghostDataSaved == 0) {
                        SetWord2A(0x2A + (i * 0x89), 0x96 + (0x1E * j), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        SetWord2A(
                            0x2A + (i * 0x89), 0x96 + (0x1E * j),
                            circuit_name_semilong_j[gCupCourseOrder[var_v1->courseIndex / 4][var_v1->courseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            break;
        case SUB_MENU_COPY_PAK_PROMPT_QUIT:
        case SUB_MENU_COPY_PAK_PROMPT_COPY:
            set_text_color(TEXT_RED);
            for (i = 0; i < ARRAY_COUNT(option_copy_sure_messages_j); i++) {
                print_text1_center_mode_1(0x000000A0, 0x4D + (0x14 * i), option_copy_sure_messages_j[i], 0, 0.8f, 0.8f);
            }
            for (i = 0; i < ARRAY_COUNT(option_copy_pak_j); i++) {
                set_text_color(TEXT_YELLOW);
                print_text1_center_mode_1(0x5C + (0x82 * i), 0x0000007D, option_copy_pak_j[i], 0, 0.75f, 0.75f);
                for (j = 0; j != 2; j++) {
                    if (i == 0) {
                        if (j == arg0->work1) {
                            set_text_color((s32) frame_counter % 3);
                        } else {
                            set_text_color(TEXT_GREEN);
                        }
                    } else if (j == arg0->work2) {
                        set_text_color((s32) frame_counter % 3);
                    } else {
                        set_text_color(TEXT_GREEN);
                    }
                    kawano_itoa(j + 1, &spA8[0]);
                    SetWord2A(0x20 + (0x89 * i), 0x96 + (0x1E * j), &spA8[1], 0, 0.6f, 0.6f);
                    if (i == 0) {
                        do {
                        } while (0); // FAKE
                        var_v1 = &D_8018EE10[j];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) kdl_buff)[j];
                    }
                    if (var_v1->ghostDataSaved == 0) {
                        SetWord2A(0x2A + (i * 0x89), 0x96 + (0x1E * j), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        SetWord2A(
                            0x2A + (i * 0x89), 0x96 + (0x1E * j),
                            circuit_name_semilong_j[gCupCourseOrder[var_v1->courseIndex / 4][var_v1->courseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            for (i = 0; i < ARRAY_COUNT(option_copy_sure2_messages_j); i++) {
                if (i == (menuScreenA - SUB_MENU_COPY_PAK_PROMPT_MIN)) {
                    spE0.x = 0x6E + (0x32 * i);
                    spE0.y = 0x00D2;
                }
                SetCursorMessageColor((menuScreenA - SUB_MENU_COPY_PAK_PROMPT_MIN), j, TEXT_YELLOW);
                SetWord2A(0x6E + (0x32 * i), 0x000000D2, option_copy_sure2_messages_j[i], 0, 0.75f, 0.75f);
            }
            break;
        case SUB_MENU_COPY_PAK_START:
        case SUB_MENU_COPY_PAK_COPYING:
        case SUB_MENU_COPY_PAK_COMPLETED:
            var_s5 = (menuScreenA - SUB_MENU_COPY_PAK_ACTION_MIN) / 2;
            set_text_color(TEXT_RED);
            print_text1_center_mode_1(0x000000A0, 0x00000055, option_copy_copy_messages_j[var_s5], 0, 1.0f, 1.0f);
            for (i = 0; i < ARRAY_COUNT(option_copy_pak_j); i++) {
                set_text_color(TEXT_YELLOW);
                print_text1_center_mode_1(0x5C + (0x82 * i), 0x0000007D, option_copy_pak_j[i], 0, 0.75f, 0.75f);
                for (j = 0; j < 2; j++) {
                    if (i == 0) {
                        if (j == arg0->work1) {
                            if (var_s5 == 0) {
                                set_text_color(TEXT_RED);
                            } else {
                                set_text_color(frame_counter % 3);
                            }
                        } else {
                            set_text_color(TEXT_GREEN);
                        }
                    } else if (j == arg0->work2) {
                        set_text_color(TEXT_RED);
                    } else {
                        set_text_color(TEXT_GREEN);
                    }
                    kawano_itoa(j + 1, &sp98[0]);
                    SetWord2A(0x20 + (0x89 * i), 0x96 + (0x1E * j), &sp98[1], 0, 0.6f, 0.6f);
                    if (i == 0) {
                        var_v1 = &D_8018EE10[j];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) kdl_buff)[j];
                    }
                    if (var_v1->ghostDataSaved == 0) {
                        SetWord2A(0x2A + (i * 0x89), 0x96 + (0x1E * j), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        SetWord2A(
                            0x2A + (i * 0x89), 0x96 + (0x1E * j),
                            circuit_name_semilong_j[gCupCourseOrder[var_v1->courseIndex / 4][var_v1->courseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            break;
    }
    switch (menuScreenA) {               /* switch 2 */
        case SUB_MENU_COPY_PAK_FROM_GHOST1_1P: /* switch 2 */
        case SUB_MENU_COPY_PAK_FROM_GHOST2_1P: /* switch 2 */
        case SUB_MENU_COPY_PAK_TO_GHOST1_2P:   /* switch 2 */
        case SUB_MENU_COPY_PAK_TO_GHOST2_2P:   /* switch 2 */
        case SUB_MENU_COPY_PAK_PROMPT_QUIT:    /* switch 2 */
        case SUB_MENU_COPY_PAK_PROMPT_COPY:    /* switch 2 */
            spE0.x -= 5;
            spE0.y -= 6;
            break;
        default: /* switch 2 */
            spE0.x -= 0xA;
            spE0.y -= 8;
            break;
    }
    DispOB_vs_ibox_cursor(arg0, (POSXY*) &spE0);
}

void Cut_ud_sub(ObjBlock* arg0) {
    switch (opening_flag) {
        case 1:
            gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0, 0x13F, 0x28);
            gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0xC7, 0x13F, 0xEF);
            arg0->work1 = 0x28;
            break;
        case 2:
            arg0->work1 -= 2;
            if (arg0->work1 > 0) {
                gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0, 0x13F, arg0->work1);
                gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0xEF - arg0->work1, 0x13F, 0xEF);
            } else {
                arg0->kind = 0;
            }
            break;
        default:
            if ((g_gameMode != GRAND_PRIX) || (g_playerCount != 1) || (demo_play_flag != 0)) {
                arg0->kind = 0;
            } else {
                arg0->work1 -= 2;
                if (arg0->work1 > 0) {
                    gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0, 0x13F, arg0->work1);
                    gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0xEF - arg0->work1, 0x13F, 0xEF);
                } else {
                    arg0->kind = 0;
                }
            }
            break;
    }
}

void DispOB_result_time(ObjBlock* arg0) {
    s8 sp70[8];
    UNUSED s32 stackPadding0;
    char sp68[3];
    s32 temp_s0;
    s32 var_a0;
    s32 var_s2;

    for (var_s2 = 0; var_s2 < NUM_PLAYERS; var_s2++) {
        sp70[var_s2] = gPlayers[rank_to_num[var_s2]].kart;
    }
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    SetWord2A(arg0->disp_x + 0x1E, arg0->disp_y + 0x19, "results", 0, 1.0f, 1.0f);
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
    SetWord2A(arg0->disp_x + 0x2C, arg0->disp_y + 0x28, "round", 0, 0.7f, 0.7f);
    kawano_itoa(g_courseSelect + 1, sp68);
    SetWord2A(arg0->disp_x + 0x57, arg0->disp_y + 0x28, &sp68[1], 0, 0.7f, 0.7f);
    for (var_s2 = 0; var_s2 < 4; var_s2++) {
        if (rank_to_num[var_s2] < g_menuMultiplayerSelection) {
            var_a0 = (s32) frame_counter % 3;
        } else {
            var_a0 = TEXT_YELLOW;
        }
        set_text_color(var_a0);
        ResultSub_time(arg0->disp_x + 7, arg0->disp_y + (0x10 * var_s2) + 0x38, (s32) sp70[var_s2], var_s2);
    }
    for (var_s2 = 4; var_s2 < 8; var_s2++) {
        if (rank_to_num[var_s2] < g_menuMultiplayerSelection) {
            var_a0 = (s32) frame_counter % 3;
        } else {
            var_a0 = TEXT_YELLOW;
        }
        set_text_color(var_a0);
        ResultSub_time(0xBE - arg0->disp_x, arg0->disp_y + (0x10 * var_s2) + 0x5A, sp70[var_s2], var_s2);
    }
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
    temp_s0 = (s32) (((f32) (GetWordLength(cup_name[g_cupSelect]) + 8) * 0.6f) / 2);
    print_text1_center_mode_1(
        (-(s32) (((f32) (GetWordLength(class_name[g_raceClass]) + 8) * 0.6f) / 2) - arg0->disp_x) + 0xF5,
        arg0->disp_y + 0xE1, cup_name[cup_level], 0, 0.6f, 0.6f);
    print_text1_center_mode_1(
        (temp_s0 - arg0->disp_x) + 0xF5, arg0->disp_y + 0xE1,
        class_name[gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]], 0, 0.6f, 0.6f);
}

void ResultSub_time(s32 arg0, s32 arg1, s32 characterId, s32 rank) {
    UNUSED s32 stackPadding0;
    f32 sp50;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    char sp3C[4];

    sp50 = gTimePlayerLastTouchedFinishLine[rank_to_num[rank]];
    kawano_itoa(rank + 1, sp3C);
    sp3C[2] = '.';
    sp3C[3] = '\0';
    SetWord4A(arg0 - 1, arg1, &sp3C[1], -4, 0.7f, 0.7f);
    SetWord2A(arg0 + 0xA, arg1, drivers_name_j[characterId], 0, 0.65f, 0.7f);
    kawano_itoa((s32) (sp50 / 60.0f), sp3C);
    SetWord4A(arg0 + 0x42, arg1, sp3C, 0, 0.7f, 0.7f);
    kawano_itoa((s32) sp50 % 60, sp3C);
    SetWord2A(arg0 + 0x4E, arg1, "'", 0, 0.7f, 0.7f);
    SetWord4A(arg0 + 0x56, arg1, sp3C, 0, 0.7f, 0.7f);
    kawano_itoa((s32) ((f64) sp50 * 100.0) % 100, sp3C);
    SetWord2A(arg0 + 0x62, arg1, "\"", 0, 0.7f, 0.7f);
    SetWord4A(arg0 + 0x6A, arg1, sp3C, 0, 0.7f, 0.7f);
}

void DispOB_result_point(ObjBlock* arg0) {
    s8 sp80[8];
    UNUSED s32 stackPadding0;
    char sp78[3];
    UNUSED s32 stackPadding1;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;
    UNUSED s32 stackPadding2;
    s32 temp_s0_3;
    s32 rank;
    s32 test;

    if (arg0->pattern != 0) {
        if (arg0->pattern < 9) {
            for (rank = 0; rank < NUM_PLAYERS; rank++) {
                sp80[rank] = gPlayers[rank_to_num[rank]].kart;
            }
        } else {
            SortDPRanking(sp80);
            SortDPRanking(d_points_rank);
        }
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
        SetWord2A(arg0->disp_x + 0x19, 0x19 - arg0->disp_y, "driver's points", 0, 0.8f, 0.8f);
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        SetWord2A(arg0->disp_x + 0x36, 0x28 - arg0->disp_y, "round", 0, 0.7f, 0.7f);
        kawano_itoa(g_courseSelect + 1, sp78);
        SetWord2A(arg0->disp_x + 0x61, (0x28 & 0xFFFFFFFF) - arg0->disp_y, &sp78[1], 0, 0.7f, 0.7f);
        for (rank = 0; rank < 4; rank++) {
            test = arg0->pattern;
            if ((test != 8) && (test != 9)) {
                var_v0 = 0;
            } else {
                if ((rank * 5) < arg0->work1) {
                    var_v0 = 1;
                } else {
                    var_v0 = 0;
                }
            }
            if (var_v0 == 0) {
                if (arg0->pattern < 9) {
                    var_v0 = rank_to_num[rank];
                    var_v1 = 0;
                } else {
                    var_v1 = 0x0000000D;
                    var_v0 = name_to_num[sp80[rank]];
                }
                if (var_v0 < g_menuMultiplayerSelection) {
                    var_a0 = (s32) frame_counter % 3;
                } else {
                    var_a0 = 3;
                }
                set_text_color(var_a0);
                ResultSub_point(arg0, arg0->disp_x + var_v1 + 0x1C, ((rank * 0x10) - arg0->disp_y) + 0x38, sp80[rank], rank,
                              sp80);
            }
        }
        for (rank = 4; rank < NUM_PLAYERS; rank++) {
            test = arg0->pattern;
            if ((test != 8) && (test != 9)) {
                var_v0 = 0;
            } else {
                if ((rank * 5) < arg0->work1) {
                    var_v0 = 1;
                } else {
                    var_v0 = 0;
                }
            }
            if (var_v0 == 0) {
                if (arg0->pattern < 9) {
                    var_v0 = rank_to_num[rank];
                } else {
                    var_v0 = name_to_num[sp80[rank]];
                }
                if (var_v0 < g_menuMultiplayerSelection) {
                    var_a0 = (s32) frame_counter % 3;
                } else {
                    var_a0 = 3;
                }
                set_text_color(var_a0);
                ResultSub_point(arg0, 0xBE - arg0->disp_x, arg0->disp_y + (rank * 0x10) + 0x5A, sp80[rank], rank, sp80);
            }
        }
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        temp_s0_3 = ((GetWordLength(cup_name[g_cupSelect]) + 8) * 0.6f) / 2;
        print_text1_center_mode_1(
            (-(s32) (((GetWordLength(class_name[g_raceClass]) + 8) * 0.6f) / 2) - arg0->disp_x) + 0xE6,
            arg0->disp_y + 0xE1, cup_name[cup_level], 0, 0.6f, 0.6f);
        print_text1_center_mode_1(
            (temp_s0_3 - arg0->disp_x) + 0xE6, arg0->disp_y + 0xE1,
            class_name[gGameModeSubMenuColumn[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]]], 0, 0.6f, 0.6f);
    }
}

void SortDPRanking(s8* arg0) {
    s32 temp_a3;
    s32 temp_t1;
    s32 var_a1;
    s32 var_v0;
    UNUSED s32 thing1;
    UNUSED s8* new_var;

    for (var_v0 = 0; var_v0 < 8; var_v0++) {
        arg0[var_v0] = var_v0;
        for (var_a1 = var_v0; var_a1 > 0; var_a1--) {
            new_var = &arg0[var_a1];
            temp_a3 = arg0[var_a1 - 1];
            thing1 = g_playerGPpoints[temp_a3];
            temp_t1 = arg0[var_a1];
            if (g_playerGPpoints[temp_a3] < g_playerGPpoints[temp_t1]) {
                arg0[var_a1] = temp_a3;
                arg0[var_a1 - 1] = temp_t1;
            } else if (g_playerGPpoints[temp_t1] == g_playerGPpoints[temp_a3]) {
                if ((name_to_num[temp_t1] < g_menuMultiplayerSelection) &&
                    (name_to_num[temp_t1] < name_to_num[temp_a3])) {
                    arg0[var_a1] = temp_a3;
                    arg0[var_a1 - 1] = temp_t1;
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
}

void ResultSub_point(ObjBlock* arg0, s32 arg1, s32 arg2, s32 characterId, s32 arg4, s8* arg5) {
    UNUSED s32 stackPadding0;
    s32 wut;
    char sp34[4];
    s32 phi_v1;

    if (arg0->pattern < 9) {
        kawano_itoa(arg4 + 1, sp34);
    } else {
        for (phi_v1 = arg4; phi_v1 > 0; phi_v1--) {
            wut = phi_v1 - 1;
            if (g_playerGPpoints[arg5[phi_v1]] != g_playerGPpoints[arg5[wut]]) {
                break;
            }
        }
        kawano_itoa(phi_v1 + 1, sp34);
    }
    sp34[2] = '.';
    sp34[3] = '\0';
    SetWord4A(arg1, arg2, &sp34[1], -4, 0.7f, 0.7f);
    SetWord2A(arg1 + 0xA, arg2, drivers_name_j[characterId], 0, 0.7f, 0.7f);
    kawano_itoa(g_playerGPpoints[characterId], sp34);
    SetWord4A(arg1 + 0x47, arg2, sp34, 0, 0.7f, 0.7f);
    if ((arg4 < ARRAY_COUNT(additional_point_defaults)) && (arg0->pattern < 9)) {
        kawano_itoa(additional_point[arg4], sp34);
        sp34[0] = '+';
        SetWord2A(arg1 + 0x5A, arg2, sp34, 0, 0.7f, 0.7f);
    }
}

void DispOB_timeattack1(ObjBlock* arg0) {
    s32 recordType;
    s32 rowOffset;

    // name of the course
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    print_text1_center_mode_1(arg0->disp_x + 0x43, arg0->disp_y + 0x19,
                              g_StringTableCourse[gCupCourseOrder[g_cupSelect][g_courseSelect]], 0, 0.6f, 0.6f);

    // lap time text
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(arg0->disp_x + 0x46, arg0->disp_y + 0x28, gLapTimeText, 0, 0.75f, 0.75f);

    // lap time
    for (recordType = 0, rowOffset = 0; recordType < TIME_TRIAL_3LAP_RECORD_5; recordType += 1, rowOffset += 0xF) {
        TimeAttack1Sub(recordType, arg0->disp_x + 0x17, arg0->disp_y + rowOffset + 0x37);
    }

    // best record text
    set_text_color(TEXT_YELLOW);
    SetWord2A(0xB4 - arg0->disp_x, arg0->disp_y + 0x86, bestrec_j[0], 0, 0.75f, 0.75f);

    // best record
    for (recordType = 0, rowOffset = 0; recordType < TIME_TRIAL_1LAP_RECORD; recordType += 1, rowOffset += 0xD) {
        set_text_color(TEXT_RED);
        TimeAttack1Sub2(recordType, 0xAA - arg0->disp_x, arg0->disp_y + rowOffset + 0x92);
    }
    set_text_color(TEXT_YELLOW);
    SetWord2A(0xB4 - arg0->disp_x, arg0->disp_y + 0xD5, bestrec_j[1], 0, 0.75f, 0.75f);
    TimeAttack1Sub2(TIME_TRIAL_1LAP_RECORD, 0xAA - arg0->disp_x, arg0->disp_y + 0xE1);
}

void DispOB_timeattack2(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    POSXY sp84;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    s32 var_v0;
    s32 var_v1;
    s32 var_s1;
    POSXY* var_v0_5;
    char sp60[3];

    var_v0 = arg0->pattern;
    if (var_v0 == 0) {
        return;
    }
    if (var_v0 == 0x0000001F) {
        return;
    }

    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    print_text1_center_mode_1(arg0->disp_x + 0x55, 0x19 - arg0->disp_y,
                              g_StringTableCourse[gCupCourseOrder[g_cupSelect][g_courseSelect]], 0, 0.6f, 0.6f);
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(arg0->disp_x + 0x55, 0x28 - arg0->disp_y, gLapTimeText, 0, 0.75f, 0.75f);
    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        TimeAttack1Sub(var_s1, arg0->disp_x + 0x26, ((0xF * var_s1) - arg0->disp_y) + 0x37);
    }
    switch (arg0->pattern) {
        case 1:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 30:
            for (var_s1 = 0; var_s1 < 6; var_s1++) {
                var_v1 = 0;
                SetCursorMessageColor(arg0->pattern - 5, var_s1, 1);
                switch (var_s1) { /* switch 3; irregular */
                    case 4:       /* switch 3 */
                        if (lost_ghost == 1) {
                            var_v1 = 1;
                        }
                        break;
                    case 5: /* switch 3 */
                        if (ghost_status != 0) {
                            var_v1 = 2;
                        }
                        break;
                }
                if (var_v1 != 0) {
                    set_text_color(TEXT_BLUE);
                    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0x96);
                    SetWord2AP(0xB2 - arg0->disp_x, arg0->disp_y + (0xD * var_s1) + 0x93,
                                      ta_result_select_j[var_s1 + 1], 0, 0.75f, 0.75f);
                } else {
                    SetWord2A(0xB2 - arg0->disp_x, arg0->disp_y + (0xD * var_s1) + 0x93,
                                      ta_result_select_j[var_s1 + 1], 0, 0.75f, 0.75f);
                }
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            set_text_color(TEXT_YELLOW);
            var_v1 = arg0->pattern - 11;
            for (var_s1 = 0; var_s1 < 7; var_s1++) {
                SetWord2A(0x000000A2, 0x8C + (0xD * var_s1), ta_ghosta_messages_j[(var_v1 * 7) + var_s1], 0, 0.6f, 0.6f);
            }
            break;
        case 17:
        case 18:
            set_text_color(TEXT_GREEN);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetWord2A(0x000000A5, arg0->disp_y + (0xD * var_s1) + 0x8C, ta_ghostb_messages_j[var_s1], 0, 0.7f, 0.7f);
            }
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetCursorMessageColor(arg0->pattern - 0x11, var_s1, 1);
                kawano_itoa(var_s1 + 1, sp60);
                SetWord2A(0xB1 - arg0->disp_x, 0xAA + (0x1E * var_s1), &sp60[1], 0, 0.6f, 0.6f);
                if (D_8018EE10[var_s1].ghostDataSaved == 0) {
                    SetWord2A(0xBB - arg0->disp_x, 0xAA + (0x1E * var_s1), D_800E7A44, 0, 0.45f, 0.45f);
                } else {
                    SetWord2A(0xBB - arg0->disp_x, 0xAA + (0x1E * var_s1),
                                      circuit_name_semilong_j[gCupCourseOrder[D_8018EE10[var_s1].courseIndex / 4]
                                                                      [D_8018EE10[var_s1].courseIndex % 4]],
                                      0, 0.45f, 0.45f);
                }
            }
            break;
        case 19:
            set_text_color(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                SetWord2A(0x000000AA, (0xD * var_s1) + 0x93, ta_ghostb3_messages_j[var_s1], 0, 0.8f, 0.8f);
            }
            break;
        case 20:
        case 21:
            if (var_s1 && var_s1) {}
            set_text_color(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                SetWord2A(0x000000A3, arg0->disp_y + (0xD * var_s1) + 0x8C, ta_ghostc_messages_j[var_s1], 0, 0.67f, 0.67f);
            }
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetCursorMessageColor(arg0->pattern - 0x14, var_s1, 1);
                SetWord2A(0xC8 - arg0->disp_x, 0xB9 + (0xF * var_s1), ta_ghostc_select_messages_j[var_s1], 0, 0.75f, 0.75f);
            }
            break;
        case 25:
            set_text_color(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                SetWord2A(0x000000A3, (0xD * var_s1) + 0x93, ta_ghostd_messages_j[var_s1], 0, 0.67f, 0.67f);
            }
            break;
        case 26:
            set_text_color(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetWord2A(0x000000AA, (0xD * var_s1) + 0x93, ta_ghostd2_messages_j[var_s1], 0, 0.75f, 0.75f);
            }
            break;
    }
    switch (arg0->pattern) { /* switch 2 */
        case 5:            /* switch 2 */
        case 6:            /* switch 2 */
        case 7:            /* switch 2 */
        case 8:            /* switch 2 */
        case 9:            /* switch 2 */
        case 10:           /* switch 2 */
            var_v0_5 = &ta_result_cursor_position[arg0->pattern - 5];
            break;
        case 17: /* switch 2 */
        case 18: /* switch 2 */
            var_v0_5 = &ta_ghostb_cursor_position[arg0->pattern - 17];
            break;
        case 20: /* switch 2 */
        case 21: /* switch 2 */
            var_v0_5 = &ta_ghostc_cursor_position[arg0->pattern - 20];
            break;
        case 30: /* switch 2 */
            var_v0_5 = &ta_result_cursor_position[arg0->work1 - 5];
            break;
        default:
            return;
    }
    sp84.x = var_v0_5->x - arg0->disp_x;
    sp84.y = var_v0_5->y + arg0->disp_y;
    DispOB_vs_ibox_cursor(arg0, &sp84);
}

void TimeAttack1Sub(s32 lapNumber, s32 column, s32 row) {
    UNUSED s32 stackPadding0;
    s32 time;
    UNUSED s32 stackPadding1;
    s32 textColor;
    char sp34[3];
    ObjBlock* temp_v0_2;

    if (lapNumber < 3) {
        time = playerHUD[PLAYER_ONE].laptimebuf[lapNumber];
        set_text_color(TEXT_RED);
    } else {
        time = playerHUD[PLAYER_ONE].totaltime;
        set_text_color(TEXT_GREEN);
    }
    SetWord3A(column + 0x21, row, laptime_j[lapNumber], 0, 0.7f, 0.7f);
    temp_v0_2 = Get_OBJBlock_ptr(MENU_ITEM_TYPE_0BB);
    if (lapNumber < 3) {
        if (temp_v0_2->work2 & (1 << lapNumber)) { // best lap
            textColor = (s32) frame_counter % 3;
        } else {
            textColor = TEXT_YELLOW;
        }
    } else {
        if (temp_v0_2->work1 >= 0) {
            textColor = (s32) frame_counter % 3;
        } else {
            textColor = TEXT_YELLOW;
        }
    }
    set_text_color(textColor);
    kawano_itoa_m(time, sp34);
    SetWord4A(column + 0x2C, row, sp34, 0, 0.7f, 0.7f);
    SetWord2A(column + 0x37, row, "'", 0, 0.7f, 0.7f);
    kawano_itoa_s(time, sp34);
    SetWord4A(column + 0x40, row, sp34, 0, 0.7f, 0.7f);
    SetWord2A(column + 0x4B, row, "\"", 0, 0.7f, 0.7f);
    kawano_itoa_c(time, sp34);
    SetWord4A(column + 0x55, row, sp34, 0, 0.7f, 0.7f);
}

void TimeAttack1Sub2(s32 recordType, s32 column, s32 row) {
    UNUSED s32 pad;
    u32 timeRecord;
    UNUSED s32 pad2;
    s32 textColor;
    s32 temp_t0;
    char sp38[3];
    ObjBlock* item;
    s32 sp30;

    if (gGamestate == RACING) {
        sp30 = 0;
    } else {
        sp30 = 1;
    }
    if (recordType < 5) {
        if (sp30 == 0) {
            timeRecord = GetRecordTime2(recordType);
        } else {
            timeRecord = GetRecordTime3(recordType, gTimeTrialDataCourseIndex);
        }
        set_text_color(TEXT_GREEN);
    } else {
        if (sp30 == 0) {
            timeRecord = GetRecordLapTime();
        } else {
            timeRecord = GetRecordLapTime3(gTimeTrialDataCourseIndex);
        }
    }
    SetWord4A(column + 0x14, row, recordtime_j[recordType], 2, 0.65f, 0.65f);
    if (sp30 == 0) {
        item = Get_OBJBlock_ptr(0x000000BB);
        if (recordType < 5) {
            if (recordType == item->work1) {
                textColor = frame_counter % 3;
            } else {
                textColor = TEXT_YELLOW;
            }
        } else if (item->work2 != 0) {
            textColor = frame_counter % 3;
        } else {
            textColor = TEXT_YELLOW;
        }
    } else {
        textColor = TEXT_YELLOW;
    }
    set_text_color(textColor);
    temp_t0 = timeRecord & 0xFFFFF;
    kawano_itoa_m(temp_t0, sp38);
    SetWord4A(column + 0x27, row, sp38, 0, 0.65f, 0.65f);
    SetWord2A(column + 0x32, row, "'", 0, 0.65f, 0.65f);
    kawano_itoa_s(temp_t0, sp38);
    SetWord4A(column + 0x3B, row, sp38, 0, 0.65f, 0.65f);
    SetWord2A(column + 0x46, row, "\"", 0, 0.65f, 0.65f);
    kawano_itoa_c(temp_t0, sp38);
    SetWord4A(column + 0x50, row, sp38, 0, 0.65f, 0.65f);
    if ((u32) temp_t0 < 600000U) {
        textColor = timeRecord >> 0x14;
    } else {
        textColor = 8;
    }
    print_text1_center_mode_1(column + 0x78, row, drivers_name_j[textColor], 0, 0.65f, 0.65f);
}

void DispOB_timeattack_message1(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t2;
    f32 someMultiplier = 0.85f;
    s32 thing = 24.0f * someMultiplier;

    temp_t0 = 0x140 - arg0->disp_x;
    temp_t1 = arg0->disp_y;
    temp_t2 = (s32) ((GetWordLength(ta_romghost_message_j) + 8) * someMultiplier) / 2;
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, temp_t0 - temp_t2, (temp_t1 - thing) + 4, temp_t2 + temp_t0,
                                temp_t1 + 4, 0, 0, 0, 0x00000064);
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    print_text1_center_mode_1(arg0->disp_x - 3, arg0->disp_y, ta_romghost_message_j, 0, 0.85f, 0.85f);
}

void DispOB_pause(ObjBlock* arg0) {
    if (pause_flag != 0) {
        switch (g_gameMode) {
            case TIME_TRIALS:
                DispOB_pause_ta(arg0);
                break;
            case VERSUS:
                DispOB_pause_vs(arg0);
                break;
            case GRAND_PRIX:
                DispOB_pause_gp(arg0);
                break;
            case BATTLE:
                DispOB_pause_bt(arg0);
                break;
        }
    }
}

void DispOB_pause_ta(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    char sp68[3];
    s32 temp_a0;
    s32 var_s0;
    s32 zero = 0; // ?

    gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x0000008C);
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(0x000000A0, 0x00000050,
                              g_StringTableCourse[gCupCourseOrder[g_cupSelect][g_courseSelect]], 0, 1.0f, 1.0f);
    set_text_color(TEXT_RED);
    print_text1_center_mode_1(0x0000009D, 0x00000060, bestrec_j[0], 0, 0.8f, 0.8f);
    temp_a0 = GetRecordTime2(TIME_TRIAL_3LAP_RECORD_1);
    temp_a0 &= 0xFFFFF;
    kawano_itoa_m(temp_a0, sp68);
    SetWord4A(0x0000007F, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x0000008E, 0x0000006D, "'", 0, 0.8f, 0.8f);
    kawano_itoa_s(temp_a0, sp68);
    SetWord4A(0x00000098, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x000000A7, 0x0000006D, "\"", 0, 0.8f, 0.8f);
    kawano_itoa_c(temp_a0, sp68);
    SetWord4A(0x000000B3, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    print_text1_center_mode_1(0x0000009D, 0x0000007C, bestrec_j[1], 0, 0.8f, 0.8f);
    temp_a0 = GetRecordLapTime();
    temp_a0 &= 0xFFFFF;
    kawano_itoa_m(temp_a0, sp68);
    SetWord4A(0x0000007F, 0x00000089, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x0000008E, 0x00000089, "'", 0, 0.8f, 0.8f);
    kawano_itoa_s(temp_a0, sp68);
    SetWord4A(0x00000098, 0x00000089, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x000000A7, 0x00000089, "\"", 0, 0.8f, 0.8f);
    kawano_itoa_c(temp_a0, sp68);
    SetWord4A(0x000000B3, 0x00000089, sp68, 0, 0.8f, 0.8f);
    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        SetCursorMessageColor(arg0->pattern - 11, var_s0, TEXT_GREEN);
        SetWord2A(D_800E8538[zero].x, D_800E8538[zero].y + (13 * var_s0), ta_result_select_j[var_s0], 0,
                          0.75f, 0.75f);
    }
}

void DispOB_pause_vs(ObjBlock* arg0) {
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_s0;
    s32 var_s1;
    POSXY* temp_s3;
    Screen* temp_v0;

    temp_v0 = &D_8015F480[pause_flag - 1];
    temp_v1 = temp_v0->posx;
    temp_t0 = temp_v0->posy;
    temp_t3 = temp_v0->width / 2;
    temp_t4 = temp_v0->height / 2;
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 0x0000008C);
    temp_s3 = &D_800E8540[(g_ScreenSplitB * 4) + (pause_flag - 1)];
    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        if (var_s0 > 0) {
            var_s1 = var_s0 + 1;
        } else {
            var_s1 = var_s0;
        }
        SetCursorMessageColor(arg0->pattern - 0x15, var_s0, TEXT_YELLOW);
        SetWord2A(temp_s3->x - 2, temp_s3->y + (13 * var_s0), ta_result_select_j[var_s1], 0, 0.75f, 0.75f);
    }
}

void DispOB_pause_gp(ObjBlock* arg0) {
    s32 temp_t0;
    s32 temp_v1;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_s0;
    POSXY* temp_s3;
    Screen* temp_v0;
    f32 one = 1.0f;

    temp_v0 = &D_8015F480[pause_flag - 1];
    temp_v1 = temp_v0->posx;
    temp_t0 = temp_v0->posy;
    temp_t3 = temp_v0->width / 2;
    temp_t4 = temp_v0->height / 2;
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 140);
    temp_s3 = &D_800E85C0[(g_ScreenSplitB * 4) + (pause_flag - 1)];
    temp_s0 = ((GetWordLength(cup_name[g_cupSelect]) * one) + 10.0f) / 2;
    temp_s1 = ((GetWordLength(class_name[g_raceClass]) * one) + 10.0f) / 2;
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(160 - temp_s1, temp_s3->y - 50, cup_name[g_cupSelect], 0, 1.0f, 1.0f);
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(160 + temp_s0, temp_s3->y - 50, class_name[g_raceClass], 0, 1.0f, 1.0f);
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(160, temp_s3->y - 30,
                              g_StringTableCourse[gCupCourseOrder[g_cupSelect][g_courseSelect]], 0, 1.0f, 1.0f);
    for (var_s0 = 0; var_s0 < 2; var_s0++) {
        SetCursorMessageColor(arg0->pattern - 31, var_s0, TEXT_YELLOW);
        SetWord2A(temp_s3->x, temp_s3->y + (var_s0 * 13), ta_result_select_j[var_s0 * 4], 0, 0.75f, 0.75f);
    }
}

void DispOB_pause_bt(ObjBlock* arg0) {
    Screen* temp_v0;
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_a1;
    s32 var_s1;
    POSXY* temp_s3;

    temp_v0 = &D_8015F480[pause_flag - 1];
    temp_v1 = temp_v0->posx;
    temp_t0 = temp_v0->posy;
    temp_t3 = temp_v0->width / 2;
    temp_t4 = temp_v0->height / 2;
    gDisplayListHead = FillRect1ColorF(gDisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 0x0000008C);
    temp_s3 = &D_800E8600[(g_ScreenSplitB * 4) + (pause_flag - 1)];
    for (var_a1 = 0; var_a1 < 4; var_a1++) {
        if (var_a1 > 0) {
            var_s1 = var_a1 + 1;
        } else {
            var_s1 = var_a1;
        }
        SetCursorMessageColor(arg0->pattern - 0x29, var_a1, TEXT_YELLOW);
        SetWord2A(temp_s3->x - 2, temp_s3->y + 13 * var_a1, ta_result_select_j[var_s1], 0, 0.75f, 0.75f);
    }
}

void DispOB_pause_cursor(void) {
    POSXY sp50;
    POSXY* var_v1;
    ObjBlock* sp48;
    s32 whyTheSequel;
    s32 why;
    UNUSED POSXY* huh;

    if (pause_flag == 0) {
        return;
    }

    why = g_gameMode;
    sp48 = Get_OBJBlock_ptr2(MENU_ITEM_PAUSE);
    if (why) {} // ?????
    gSPViewport(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&gDynamicP->mtxEffect[effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    switch (why) { /* irregular */
        default:
            break;
        case 1:
            var_v1 = &D_800E8538[0];
            break;
        case 2:
            var_v1 = &D_800E8540[(g_ScreenSplitB * 4) + (pause_flag - 1)];
            break;
        case 0:
            var_v1 = &D_800E85C0[(g_ScreenSplitB * 4) + (pause_flag - 1)];
            break;
        case 3:
            var_v1 = &D_800E8600[(g_ScreenSplitB * 4) + (pause_flag - 1)];
            break;
    }
    whyTheSequel = start_pause_seq[why];
    sp50.x = var_v1->x - 8;
    sp50.y = (var_v1->y + ((sp48->pattern - whyTheSequel) * 0xD)) - 8;
    DispOB_vs_ibox_cursor(sp48, &sp50);
}

void DispOB_ta_replay(ObjBlock* arg0) {
    POSXY sp98;
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    f32 why;
    char sp84[3];
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    s32 temp_a0;
    UNUSED s32 var_v1;
    s32 var_s1;
    s32 var_s2;
    s32 temp_v0;
    s32 zero = 0;
    POSXY* var_v0_9;
    char sp5C[3];

    if (arg0->pattern == 0) {
        if ((arg0->work1 >= 0x1E) && ((frame_counter / 16) % 2)) {
            why = GetWordLength(ta_result_select_j[REPLAY]) * 0.8f;
            gDisplayListHead =
                FillRect1ColorF(gDisplayListHead, 0x000000C0, 0x00000021, (s32) (why) + 0xC6, 0x00000032, 0, 0, 0, 0x00000096);
            set_text_color(TEXT_GREEN);
            SetWord2A(0x000000BF, 0x00000030, ta_result_select_j[REPLAY], 0, 0.8f, 0.8f);
        }
    } else {
        if (arg0->pattern == 1) {
            var_s1 = arg0->work1;
            var_s2 = (s32) (arg0->work1 * 0xFF) / 140;
        } else {
            var_s1 = 0x0000008C;
            var_s2 = 0x000000FF;
        }
        gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, var_s1);
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, var_s2);
        set_text_color(TEXT_YELLOW);
        SetWord3ACP(0x000000A0, 0x00000050,
                                  g_StringTableCourse[gCupCourseOrder[g_cupSelect][g_courseSelect]], 0, 1.0f, 1.0f);
        switch (arg0->pattern) {
            case 1:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
                set_text_color(TEXT_RED);
                SetWord3ACP(0x0000009D, 0x00000060, bestrec_j[0], 0, 0.8f, 0.8f);
                temp_a0 = GetRecordTime2(0);
                temp_a0 &= 0xFFFFF;
                kawano_itoa_m(temp_a0, sp84);
                text_draw(0x0000007F, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x0000008E, 0x0000006D, "'", 0, 0.8f, 0.8f);
                kawano_itoa_s(temp_a0, sp84);
                text_draw(0x00000098, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x000000A7, 0x0000006D, "\"", 0, 0.8f, 0.8f);
                kawano_itoa_c(temp_a0, sp84);
                text_draw(0x000000B3, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                SetWord3ACP(0x0000009D, 0x0000007C, bestrec_j[1], 0, 0.8f, 0.8f);
                temp_a0 = GetRecordLapTime();
                temp_a0 &= 0xFFFFF;
                kawano_itoa_m(temp_a0, sp84);
                text_draw(0x0000007F, 0x00000089, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x0000008E, 0x00000089, "'", 0, 0.8f, 0.8f);
                kawano_itoa_s(temp_a0, sp84);
                text_draw(0x00000098, 0x00000089, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x000000A7, 0x00000089, "\"", 0, 0.8f, 0.8f);
                kawano_itoa_c(temp_a0, sp84);
                text_draw(0x000000B3, 0x00000089, sp84, 0, 0.8f, 0.8f);
                for (var_s1 = 0; var_s1 < 6; var_s1++) {
                    SetCursorMessageColor(arg0->pattern - 0xB, var_s1, TEXT_GREEN);
                    SetWord2AP(D_800E8538[zero].x, D_800E8538[zero].y + (0xD * var_s1),
                                      ta_result_select_j[var_s1 + 1], 0, 0.75f, 0.75f);
                }
                break;
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
                set_text_color(TEXT_YELLOW);
                temp_v0 = arg0->pattern - 0x15;
                for (var_s1 = 0; var_s1 < 7; var_s1++) {
                    SetWord2A(0x0000004D, 0x6E + (0xD * var_s1), ta_ghosta_messages_j[(temp_v0 * 7) + var_s1], 0, 0.8f,
                                      0.8f);
                }
                break;
            case 30:
            case 31:
                set_text_color(TEXT_GREEN);
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetWord2A(0x0000005A, arg0->disp_y + (0xD * var_s1) + 0x6E, ta_ghostb_messages_j[var_s1], 0, 0.8f, 0.8f);
                }
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetCursorMessageColor(arg0->pattern - 0x1E, var_s1, TEXT_GREEN);
                    kawano_itoa(var_s1 + 1, sp5C);
                    SetWord2A(0x5A - arg0->disp_x, (0x96 + (0x14 * var_s1)), &sp5C[1], 0, 0.75f, 0.75f);
                    if (D_8018EE10[var_s1].ghostDataSaved == 0) {
                        SetWord2A(0x69 - arg0->disp_x, (0x96 + (0x14 * var_s1)), D_800E7A44, 0, 0.75f, 0.75f);
                    } else {
                        SetWord2A(0x69 - arg0->disp_x, (0x96 + (0x14 * var_s1)),
                                          circuit_name_semilong_j[gCupCourseOrder[D_8018EE10[var_s1].courseIndex / 4]
                                                                          [D_8018EE10[var_s1].courseIndex % 4]],
                                          0, 0.75f, 0.75f);
                    }
                }
                break;
            case 32:
                set_text_color(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    SetWord2A(0x00000064, (0xD * var_s1) + 0x6E, ta_ghostb3_messages_j[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 35:
            case 36:
                set_text_color(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    SetWord2A(0x00000055, arg0->disp_y + (0xD * var_s1) + 0x6E, ta_ghostc_messages_j[var_s1], 0, 0.8f, 0.8f);
                }
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetCursorMessageColor(arg0->pattern - 0x23, var_s1, TEXT_GREEN);
                    SetWord2A(0x7D - arg0->disp_x, 0x9B + (0xF * var_s1), ta_ghostc_select_messages_j[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 40:
                set_text_color(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    SetWord2A(0x00000055, (0xD * var_s1) + 0x6E, ta_ghostd_messages_j[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 41:
                set_text_color(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetWord2A(0x0000005D, (0xD * var_s1) + 0x6E, ta_ghostd2_messages_j[var_s1], 0, 0.8f, 0.8f);
                }
                break;
        }
        switch (arg0->pattern) { /* switch 2 */
            case 11:           /* switch 2 */
            case 12:           /* switch 2 */
            case 13:           /* switch 2 */
            case 14:           /* switch 2 */
            case 15:           /* switch 2 */
            case 16:           /* switch 2 */
                var_v0_9 = &tar_select_cursor_position[arg0->pattern - 11];
                break;
            case 30: /* switch 2 */
            case 31: /* switch 2 */
                var_v0_9 = &tar_ghostb_cursor_position[arg0->pattern - 30];
                break;
            case 35:      /* switch 2 */
            case 36:      /* switch 2 */
                if (0) {} // wtf?
                var_v0_9 = &tar_ghostc_cursor_position[arg0->pattern - 35];
                break;
            default:
                return;
        }
        sp98.x = var_v0_9->x;
        sp98.y = var_v0_9->y;
        DispOB_vs_ibox_cursor(arg0, &sp98);
    }
}

void DispOB_info_3p(ObjBlock* arg0) {
    char* text;

    if (g_hudMapToggle2 != true) {
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, arg0->work1);
        text = cup_name[cup_level];
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        SetWord3ACP(arg0->disp_x + 0x41, arg0->disp_y + 0xA0, text, 0, 0.85f, 1.0f);
        text = g_StringTableCourseGP[g_courseID];
        set_text_color((s32) g_courseID % 4);
        SetWord3ACP(arg0->disp_x + 0x41, arg0->disp_y + 0xC3, text, 0, 0.65f, 0.85f);
    }
}

void DispOB_gp_lose(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    POSXY sp6C;
    UNUSED s32 stackPadding3;
    s32 var_s0;
    s32 var_s1;

    if (arg0->pattern == 0) {
        gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, arg0->work1);
        set_text_color(TEXT_YELLOW);
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, (arg0->work1 * 0xFF) / 100);
        for (var_s1 = 0, var_s0 = 0x96; var_s0 < 0xBE; var_s1++, var_s0 += 0x14) {
            SetWord2AP(0x0000008C, var_s0, ta_result_select_j[(var_s1 * 3) + 1], 0, 1.0f, 1.0f);
        }
    } else {
        gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x00000064);
        for (var_s1 = 0, var_s0 = 0x96; var_s1 < 2; var_s1++, var_s0 += 0x14) {
            SetCursorMessageColor(arg0->pattern - 0xB, var_s1, TEXT_YELLOW);
            SetWord2A(0x0000008C, var_s0, ta_result_select_j[(var_s1 * 3) + 1], 0, 1.0f, 1.0f);
        }
    }
    if (arg0->pattern >= 0xB) {
        sp6C.x = 0x0084;
        sp6C.y = (arg0->pattern * 0x14) - 0x4E;
        DispOB_vs_ibox_cursor(arg0, &sp6C);
    }
    if (arg0->work2 > 0) {
        gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0, 0x0000013F, arg0->work2);
        gDisplayListHead = FillRectBlack(gDisplayListHead, 0, 0xEF - arg0->work2, 0x0000013F, 0x000000EF);
    }
}

void DispOB_vs_result(ObjBlock* arg0) {
    UNUSED s32 temp_a0;
    s32 var_a1;
    UNUSED s32 var_s0;
    s32 var_s1;
    UNUSED s8** var_s2;

    if (arg0->pattern == 0) {
        gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, arg0->work1);
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, (arg0->work1 * 0xFF) / 100);
        SetWord3ACP(0x000000A0, arg0->disp_y + 0x1E, vs_result_title_j[g_gameMode / 3], 0, 1.0f, 1.0f);
    } else {
        gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x00000064);
        set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
        print_text1_center_mode_1(0x000000A0, arg0->disp_y + 0x1E, vs_result_title_j[g_gameMode / 3], 0, 1.0f, 1.0f);
    }
    switch (arg0->pattern) { /* irregular */
        default:
            var_a1 = 0x000000FF;
            break;
        case 0:
        case 1:
            var_a1 = 0;
            break;
        case 2:
            var_a1 = arg0->work1;
            break;
    }
    gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, var_a1);
    switch (g_menuMultiplayerSelection) {
        case 2:
            DispOB_vs_point2P(arg0);
            break;
        case 3:
            DispOB_vs_point3P(arg0);
            break;
        case 4:
            DispOB_vs_point4P(arg0);
            break;
        default:
            break;
    }
    if (arg0->pattern >= 10) {
        for (var_s1 = 0; var_s1 < 4; var_s1++) {
            SetCursorMessageColor(arg0->pattern - 0xA, var_s1, TEXT_GREEN);
            SetWord2A(0x00000069, 0xAE + (0xF * var_s1), ta_result_select_j[var_s1 + 1], 0, 0.8f, 0.8f); // Where Retry, course change, etc printed
        }
        DispOB_vs_ibox_cursor(arg0, &vs_result_cursor_position[arg0->pattern - 10]);
    }
}

void DispOB_vs_ibox_cursor(ObjBlock* arg0, POSXY* arg1) {
    Mtx* mtx;
    f32 tmp;
    static float x2, y2, z2;
    static float x1, y1, z1;

    mtx = &gDynamicP->mtxEffect[effectcount];
    if (arg0->workf1 > 1.5) {
        arg0->workf1 *= 0.95;
    } else {
        arg0->workf1 = 1.5;
    }

    tmp = arg0->workf1;
    x1 = (tmp * 3) * arg0->pattern2;
    y1 = tmp * 4;
    z1 = tmp * 2;
    x2 += x1;
    y2 += y1;
    z2 += z1;

    // clang-format off
    if (x2) {}; if (y2) {}; if (z2) {};
    // clang-format on

    guScale(mtx, 1.2f, 1.2f, 1.2f);
    guRotate(mtx + 1, y2, 0.0f, 1.0f, 0.0f);
    guMtxCatL(mtx, mtx + 1, mtx);
    guRotate(mtx + 1, z2, 0.0f, 0.0f, 1.0f);
    guMtxCatL(mtx, mtx + 1, mtx);
    guRotate(mtx + 1, x2, 1.0f, 0.0f, 0.0f);
    guMtxCatL(mtx, mtx + 1, mtx);
    guTranslate(mtx + 1, arg1->x, arg1->y, 0.0f);
    guMtxCatL(mtx, mtx + 1, mtx);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxEffect[effectcount++]),
              (G_MTX_NOPUSH | G_MTX_LOAD) | G_MTX_MODELVIEW);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPNoOp(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gSPDisplayList(gDisplayListHead++, D_0D003090);
}

void DispOB_vs_point2P(UNUSED ObjBlock* arg0) {
    POSXY* thing;
    UNUSED s32 stackPadding1;
    s32 var_s0;
    char sp74[5];
    s32 var_v1;
    char* temp_s3;
    u8* var_s4;

    for (var_s0 = 0; var_s0 < g_menuMultiplayerSelection; var_s0++) {
        var_v1 = 0;
        thing = &vs_face_target_position[var_s0];
        switch (g_gameMode) { /* irregular */
            case VERSUS:
                if (gGPCurrentRaceRankByPlayerId[var_s0] != 0) {
                    var_v1 = 1;
                }
                var_s4 = &wincount2p[var_s0];
                break;
            case BATTLE:
                if (var_s0 != win_kart) {
                    var_v1 = 1;
                }
                var_s4 = &battlewincount2p[var_s0];
                break;
        }
        temp_s3 = win_lose_j[var_v1];
        if (var_v1 != 0) {
            set_text_color(TEXT_BLUE);
        } else {
            set_text_color((s32) frame_counter % 3);
        }
        kawano_itoa2(var_s4[0], sp74);
        text_draw(thing->x + 0x10, thing->y + 0x75, sp74, 0, 1.0f, 1.0f);
        SetWord3ACP(vs_result_winlose_position[var_s0].x, vs_result_winlose_position[var_s0].y, temp_s3, 0, 0.65f, 1.0f);
    }
    set_text_color(TEXT_BLUE);
    // Not a hyphen, that is an EUC-JP character
    text_draw(0x0000009E, vs_face_target_position[0].y + 0x6D, "ー", 0, 1.0f, 1.0f);
}

void DispOB_vs_point3P(UNUSED ObjBlock* arg0) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < g_menuMultiplayerSelection; var_s0++) {
        switch (g_gameMode) { /* irregular */
            case VERSUS:
                DispOB_vs_point123(3, var_s0, wincount3p);
                break;
            case BATTLE:
                DispOB_bt_point(3, var_s0, battlewincount3p);
                break;
        }
    }
}

void DispOB_vs_point4P(UNUSED ObjBlock* arg0) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < g_menuMultiplayerSelection; var_s0++) {
        switch (g_gameMode) { /* irregular */
            case VERSUS:
                DispOB_vs_point123(4, var_s0, wincount4p);
                break;
            case BATTLE:
                DispOB_bt_point(4, var_s0, battlewincount4p);
                break;
        }
    }
}

void DispOB_bt_point(s32 arg0, s32 arg1, u8* arg2) {
    UNUSED s32 stackPadding0;
    POSXY* stackPadding1;
    char sp30[5];
    s32 var_v0;
    f32 otherThing;
    s32 thing;

    var_v0 = 0;
    if (arg1 != win_kart) {
        var_v0 = 1;
    }
    thing = arg2[arg1];
    if (var_v0 != 0) {
        set_text_color(TEXT_BLUE);
    } else {
        set_text_color(frame_counter % 3);
    }
    stackPadding1 = &vs_face_target_position[((arg0 - 2) * 4) + arg1];
    kawano_itoa2(thing, sp30);
    otherThing = 0.75f;
    text_draw((stackPadding1->x + 0x20) - ((32.0f * otherThing) / 2), stackPadding1->y + 0x75, sp30, 0, 0.75f,
              0.75f);
}

// The ｓ/ｎ/ｒ/ー are not ASCII characters, they're EUC-JP characters
void DispOB_vs_point123(s32 playerCount, s32 playerId, u8* placeAry) {
    UNUSED s32 stackPadding0;
    u8* temp_v0;
    POSXY* temp_s0;
    char sp40[3];
    s32 rank;
    // Everything about this variable is bizarre
    s32 rankIdx = -1;

    temp_s0 = &vs_face_target_position[((playerCount - 2) * 4) + playerId];
    rank = gGPCurrentRaceRankByPlayerId[playerId];
    if (rank == ++rankIdx) {
        set_text_color(frame_counter % 3);
    } else {
        set_text_color(TEXT_YELLOW);
    }
    text_draw(temp_s0->x + 4, temp_s0->y + 0x5A, "1 ｓ ー", 0, 0.8f, 0.8f);
    temp_v0 = placeAry + (playerId * 3);
    kawano_itoa(temp_v0[0], sp40);
    text_draw(temp_s0->x + 0x2D, temp_s0->y + 0x5A, sp40, 0, 0.8f, 0.8f);
    if (rank == ++rankIdx) {
        set_text_color(frame_counter % 3);
    } else {
        set_text_color(TEXT_BLUE);
    }
    text_draw(temp_s0->x + 4, temp_s0->y + 0x69, "2 ｎ ー", 0, 0.8f, 0.8f);
    kawano_itoa(temp_v0[1], sp40);
    text_draw(temp_s0->x + 0x2D, temp_s0->y + 0x69, sp40, 0, 0.8f, 0.8f);
    if (++rankIdx == rank) {
        set_text_color(frame_counter % 3);
    } else {
        set_text_color(TEXT_RED);
    }
    text_draw(temp_s0->x + 4, temp_s0->y + 0x78, "3 ｒ ー", 0, 0.8f, 0.8f);
    kawano_itoa(temp_v0[2], sp40);
    text_draw(temp_s0->x + 0x2D, temp_s0->y + 0x78, sp40, 0, 0.8f, 0.8f);
}

void DispOB_ta_lostghost(ObjBlock* arg0) {
    s32 var_s0;
    s32 temp_f6;
    s32 alpha;
    s32 loopIndex;
    s32 stringIndex;

    if (arg0->pattern == 1) {
        var_s0 = GetWordLength(ta_lost_ghost_message_j[0]) * 0.45f;
        temp_f6 = GetWordLength(ta_lost_ghost_message_j[1]) * 0.45f;
        if (var_s0 < temp_f6) {
            var_s0 = temp_f6;
        }
        gDisplayListHead =
            FillRect1ColorF(gDisplayListHead, 0x000000C0, 0x00000022, var_s0 + 0xC6, 0x00000039, 0, 0, 0, 0x00000096);
        alpha = 0x180 - ((arg0->work1 % 32) * 8);
        if (alpha >= 0x100) {
            alpha = 0xFF;
        }
        gDPSetPrimColor(gDisplayListHead++, 0, 0, 0x00, 0x00, 0x00, alpha);
        set_text_color(TEXT_RED);
        for (loopIndex = 0x2C, stringIndex = 0; loopIndex < 0x40; loopIndex += 0xA, stringIndex++) {
            SetWord2AP(0x000000C0, loopIndex, ta_lost_ghost_message_j[stringIndex], 0, 0.45f, 0.45f);
        }
    }
}

// Shading layer of the grand prix podium result screen
void DispOB_ending1_halffade(ObjBlock* arg0) {
    if (arg0->pattern == 0) {
        // If shading layer is fading in
        gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x13F, 0xEF, 0, 0, 0, arg0->work1);
    } else {
        // All other stages of the podium scene
        gDisplayListHead = FillRect1ColorF(gDisplayListHead, 0, 0, 0x13F, 0xEF, 0, 0, 0, 0x64);
    }
}

// Podium scene, top line
void DispOB_ending1_message1(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 cupNameLength = (((f32) GetWordLength(cup_name[g_cupSelect]) * 1) + 10) / 2;
    s32 ccNameLength = (((f32) GetWordLength(class_name[g_raceClass]) * 1) + 10) / 2;

    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(arg0->disp_x - ccNameLength, arg0->disp_y, cup_name[g_cupSelect], 0, 1, 1);
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(arg0->disp_x + cupNameLength, arg0->disp_y, class_name_j[g_raceClass], 0, 1, 1);
}

void DispOB_ending1_message2(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 sp40;
    s32 sp3C;
    s32 thing = resultstat.rank;
    if (thing >= 3) {
        set_text_color(TEXT_YELLOW);
        print_text1_center_mode_1(arg0->disp_x, arg0->disp_y, D_800E7A98, 0, 0.75f, 0.75f);
    } else {
        sp40 = (s32) (((f32) (GetWordLength(ending_message2[0]) + 5) * 0.75f) / 2);
        sp3C = (s32) (((f32) (GetWordLength(ending_message2[thing + 1]) + 5) * 0.75f) / 2);
        set_text_color(TEXT_YELLOW);
        print_text1_center_mode_1(arg0->disp_x - sp3C, arg0->disp_y, ending_message2[0], 0, 0.75f, 0.75f);
        set_text_color(TEXT_YELLOW);
        print_text1_center_mode_1(arg0->disp_x + sp40, arg0->disp_y, ending_message2[thing + 1], 0, 0.75f, 0.75f);
    }
}

void DispOB_ending1_message3(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 topThree;

    if (resultstat.rank < 3) {
        topThree = 0;
    } else {
        topThree = 1;
    }

    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_1);
    print_text1_center_mode_1(arg0->disp_x, arg0->disp_y, ending_message3[topThree], 0, 1.3f, 1.3f);
}

void DispOB_ending1_message4(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    s32 sp48;
    s32 sp44;
    UNUSED s32 stackPadding1;
    char sp3C[3];
    s32 temp_a0;

    temp_a0 = resultstat.rank + 1;
    kawano_itoa2(temp_a0, sp3C);
    sp48 = ((GetWordLength(ending_message4[0]) + 5) * 1.2f) / 2;
    sp44 = ((GetWordLength(ending_message4[temp_a0]) + 5) * 1.2f) / 2;
    set_text_color(TEXT_BLUE_GREEN_RED_CYCLE_2);
    print_text1_center_mode_1(arg0->disp_x - sp44, arg0->disp_y, ending_message4[0], 0, 1.2f, 1.2f);
    set_text_color((s32) frame_counter % 3);
    print_text1_center_mode_1(arg0->disp_x + sp48, arg0->disp_y, ending_message4[temp_a0], 0, 1.2f, 1.2f);
    kawano_itoa(temp_a0, sp3C);
    SetWord4A((arg0->disp_x + sp48) - 0x18, arg0->disp_y, &sp3C[1], 0, 2.0f, 2.0f);
}

void DispOB_staffroll_messages(ObjBlock* arg0) {
    f32 someScaling;
    s32 creditIndex;
    s8 slideDirection;
    UNUSED s32 pad;
    creditIndex = arg0->kind - 0x190;
    set_text_color(gCreditsTextRenderInfo[creditIndex].textColor);
    slideDirection = gCreditsTextRenderInfo[creditIndex].slideDirection;
    if ((slideDirection == SLIDE_RIGHT) || (slideDirection != SLIDE_LEFT)) {
        someScaling = gCreditsTextRenderInfo[creditIndex].textScaling;
        SetWord3A(arg0->disp_x, arg0->disp_y, gCreditsText[creditIndex], arg0->work1 * someScaling,
                         arg0->workf1 * someScaling, someScaling);
    } else {
        someScaling = gCreditsTextRenderInfo[creditIndex].textScaling;
        SetWord2A(arg0->disp_x, arg0->disp_y, gCreditsText[creditIndex], arg0->work1 * someScaling,
                          arg0->workf1 * someScaling, someScaling);
    }
}

// Originally func_800A7894
// Presumes that "number" is a 2 digit number. Convert it to a string
void kawano_itoa(s32 number, char* buffer) {
    buffer[0] = (number / 0xA) + 0x30;
    buffer[1] = (number % 0xA) + 0x30;
    // Terminator
    buffer[2] = 0;
}

// Originally func_800A78E0
// MK 64 doesn't show more then 2 digits for any given time
// (minute, second, centisecond)
// So, if timeRecord is greater than 10 minutes, just use '-'s
void kawano_itoa_nodata(char* buffer) {
    buffer[0] = 0x2D;
    buffer[1] = 0x2D;
    buffer[2] = '\0';
}

// Originally func_800A78F4
// timeRecord is assumed to be measured in centiseconds
void kawano_itoa_m(s32 timeRecord, char* buffer) {
    // If timeRecord is greater than 100 minutes, put '-'
    // in buffer
    if (timeRecord >= MAX_TIME) {
        kawano_itoa_nodata(buffer);
        return;
    }
    kawano_itoa(timeRecord / 0x1770, buffer);
}

// Originally func_800A7944
// timeRecord is assumed to be measured in centiseconds
void kawano_itoa_s(s32 timeRecord, char* buffer) {
    // If timeRecord is greater than 100 minutes, put '-'
    // in buffer
    if (timeRecord >= MAX_TIME) {
        kawano_itoa_nodata(buffer);
        return;
    }
    kawano_itoa((timeRecord / 0x64) % 0x3C, buffer);
}

// Originally func_800A79A4
// timeRecord is assumed to be measured in centiseconds
void kawano_itoa_c(s32 timeRecord, char* buffer) {
    // If timeRecord is greater than 100 minutes, put '-'
    // in buffer
    if (timeRecord >= MAX_TIME) {
        kawano_itoa_nodata(buffer);
        return;
    }
    kawano_itoa(timeRecord % 0x64, buffer);
}

// Converts a 2-digit number to EUC-JP by the looks of it
void kawano_itoa2(s32 arg0, char* arg1) {
    arg1[0] = 0xA3;
    arg1[1] = (arg0 / 0xA) - 0x50;
    arg1[2] = 0xA3;
    arg1[3] = (arg0 % 0xA) - 0x50;
    arg1[4] = '\0';
}

void DoObjBlock(s32 priSpecial) {
    s32 j;
    s32 isRendered;
    s32 i;
    s32 type;
    ObjBlock* entry;

    for (i = 0; i < ARRAY_COUNT(gMenuItems); i++) {
        isRendered = false;
        entry = &gMenuItems[i];
        type = entry->kind;
        if ((type == MENU_ITEM_UI_NO_CONTROLLER) || (type == MENU_ITEM_UI_START_RECORD_TIME) ||
            (type == MENU_ITEM_PAUSE)) {
            if (priSpecial != 0) {
                isRendered = true;
            }
        } else if (priSpecial == 0) {
            isRendered = true;
        }

        if (isRendered == false) {
            continue;
        }

        switch (type) {                   /* switch 8; irregular */
            case MENU_ITEM_UI_LOGO_INTRO: /* switch 8 */
                if (logo_timer < 0x50) {
                    logo_yang_speed = 3.0f;
                } else if (logo_timer < 0x5A) {
                    if (logo_delay_size < 1.0) {
                        logo_delay_size += 0.1;
                    }
                    logo_yang_speed += 0.1;
                } else if (logo_timer < 0xA0) {
                    logo_yang_speed += 0.1;
                } else if (logo_timer < 0x190) {
                    logo_yang_speed += 0.3;
                }
                logo_rot_y -= logo_yang_speed;
                logo_timer += 1;
                if (logo_rot_y < -360.0f) {
                    logo_rot_y += 360.0f;
                }
                entry->work1++;
                if (entry->work1 == 0x000000B4) {
                    SetFadeOutWhite(0x00000028);
                    Na_SE_Fadeout(0x64U);
                    KBGNumberNext = MENU_FADE_TYPE_MAIN;
                }
                if ((entry->work2 != 0) && (entry->work1 >= 3)) {
                    entry->work2 = 0;
                    playSound(SOUND_INTRO_LOGO);
                }
                break;
            case MENU_ITEM_TYPE_0DA: /* switch 8 */
                DOB_cp_cscolor(entry);
                break;
            case MENU_ITEM_TYPE_0D6: /* switch 8 */
                DOB_cp_mes2(entry);
                break;
            case MENU_ITEM_TYPE_0D4: /* switch 8 */
                DOB_cp_gnote(entry);
                break;
            case MENU_ITEM_UI_START_RECORD_TIME: /* switch 8 */
                switch (entry->pattern) {          /* switch 9; irregular */
                    case 0:                      /* switch 9 */
                        if (gControllerFive->ButtonHeld & R_TRIG) {
                            entry->pattern = (s32) 1U;
                            playSound(SOUND_ACTION_PING);
                        } else {
                            entry->disp_on = 0;
                        }
                        break;
                    case 1:  /* switch 9 */
                    default: /* switch 9 */
                        entry->disp_on = 1;
                        break;
                }
                break;
            case MENU_ITEM_UI_GAME_SELECT: /* switch 8 */
                DOB_gsel_start(entry);
                break;
            case MAIN_MENU_OPTION_GFX:            /* switch 8 */
            case MAIN_MENU_DATA_GFX:              /* switch 8 */
                switch (menuScreenB) {     /* switch 5 */
                    case MAIN_MENU_OPTION:        /* switch 5 */
                    case MAIN_MENU_DATA:          /* switch 5 */
                    case MAIN_MENU_PLAYER_SELECT: /* switch 5 */
                        DOB_gsel_number(entry);
                        break;
                    case MAIN_MENU_MODE_SELECT:             /* switch 5 */
                    case MAIN_MENU_MODE_SUB_SELECT:         /* switch 5 */
                    case MAIN_MENU_OK_SELECT:               /* switch 5 */
                    case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 5 */
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 5 */
                        DOB_gsel_mode(entry);
                        break;
                }
                break;
            case MENU_ITEM_UI_OK: /* switch 8 */
                DOB_gsel_start(entry);
                DOB_sel_ok(entry);
                break;
            case MENU_ITEM_UI_1P_GAME:            /* switch 8 */
            case MENU_ITEM_UI_2P_GAME:            /* switch 8 */
            case MENU_ITEM_UI_3P_GAME:            /* switch 8 */
            case MENU_ITEM_UI_4P_GAME:            /* switch 8 */
                switch (menuScreenB) {     /* switch 6 */
                    case MAIN_MENU_OPTION:        /* switch 6 */
                    case MAIN_MENU_DATA:          /* switch 6 */
                    case MAIN_MENU_PLAYER_SELECT: /* switch 6 */
                        DOB_gsel_number(entry);
                        break;
                    case MAIN_MENU_MODE_SELECT:             /* switch 6 */
                    case MAIN_MENU_MODE_SUB_SELECT:         /* switch 6 */
                    case MAIN_MENU_OK_SELECT:               /* switch 6 */
                    case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 6 */
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 6 */
                        DOB_gsel_mode(entry);
                        break;
                }
                DOB_gsel_move(entry);
                break;
            case MAIN_MENU_50CC:              /* switch 8 */
            case MAIN_MENU_100CC:             /* switch 8 */
            case MAIN_MENU_150CC:             /* switch 8 */
            case MAIN_MENU_EXTRA_CC:          /* switch 8 */
            case MENU_ITEM_TYPE_016:          /* switch 8 */
            case MENU_ITEM_TYPE_017:          /* switch 8 */
            case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 8 */
            case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 8 */
                DOB_gsel_gp(entry);
                break;
            case MENU_ITEM_TYPE_01B: /* switch 8 */
                DOB_gsel_caution(entry);
                break;
            case CHARACTER_SELECT_MENU_1P_CURSOR: /* switch 8 */
            case CHARACTER_SELECT_MENU_2P_CURSOR: /* switch 8 */
            case CHARACTER_SELECT_MENU_3P_CURSOR: /* switch 8 */
            case CHARACTER_SELECT_MENU_4P_CURSOR: /* switch 8 */
                DOB_psel_cursor1(entry);
                break;
            case CHARACTER_SELECT_MENU_MARIO:  /* switch 8 */
            case CHARACTER_SELECT_MENU_LUIGI:  /* switch 8 */
            case CHARACTER_SELECT_MENU_TOAD:   /* switch 8 */
            case CHARACTER_SELECT_MENU_PEACH:  /* switch 8 */
            case CHARACTER_SELECT_MENU_YOSHI:  /* switch 8 */
            case CHARACTER_SELECT_MENU_DK:     /* switch 8 */
            case CHARACTER_SELECT_MENU_WARIO:  /* switch 8 */
            case CHARACTER_SELECT_MENU_BOWSER: /* switch 8 */
                DOB_psel_character1(entry);
                switch (entry->kind) {                 /* switch 7 */
                    case CHARACTER_SELECT_MENU_MARIO:  /* switch 7 */
                    case CHARACTER_SELECT_MENU_LUIGI:  /* switch 7 */
                    case CHARACTER_SELECT_MENU_TOAD:   /* switch 7 */
                    case CHARACTER_SELECT_MENU_PEACH:  /* switch 7 */
                    case CHARACTER_SELECT_MENU_YOSHI:  /* switch 7 */
                    case CHARACTER_SELECT_MENU_DK:     /* switch 7 */
                    case CHARACTER_SELECT_MENU_WARIO:  /* switch 7 */
                    case CHARACTER_SELECT_MENU_BOWSER: /* switch 7 */
                        DOBPSelAnimeKuppa(entry);
                        break;
                }
                switch (menuScreenC) { /* switch 10; irregular */
                    case PLAYER_SELECT_MENU_MAIN:     /* switch 10 */
                        DOBPSelTurnIn(entry);
                        break;
                    case PLAYER_SELECT_MENU_OK:         /* switch 10 */
                    case PLAYER_SELECT_MENU_OK_GO_BACK: /* switch 10 */
                        DOBPSelTurnOut(entry);
                        break;
                }
                DOB_psel_anime_priority(entry);
                break;
            case CHARACTER_SELECT_MENU_OK: /* switch 8 */
            case COURSE_SELECT_OK:         /* switch 8 */
                DOB_sel_ok(entry);
                break;
            case COURSE_SELECT_MUSHROOM_CUP: /* switch 8 */
            case COURSE_SELECT_FLOWER_CUP:   /* switch 8 */
            case COURSE_SELECT_STAR_CUP:     /* switch 8 */
            case COURSE_SELECT_SPECIAL_CUP:  /* switch 8 */
                DOB_msel_move(entry);
                switch (menuScreenA) {      /* switch 11; irregular */
                    case SUB_MENU_MAP_SELECT_CUP: /* switch 11 */
                        DOB_msel_cup(entry);
                        break;
                    case SUB_MENU_MAP_SELECT_COURSE: /* switch 11 */
                    case SUB_MENU_MAP_SELECT_OK:     /* switch 11 */
                        DOB_msel_map(entry);
                        break;
                }
                break;
            case MENU_ITEM_TYPE_058:         /* switch 8 */
            case COURSE_SELECT_COURSE_NAMES: /* switch 8 */
            case MENU_ITEM_TYPE_05A:         /* switch 8 */
            case MENU_ITEM_TYPE_05B:         /* switch 8 */
                DOB_msel_name(entry);
                break;
            case MENU_ITEM_TYPE_064: /* switch 8 */
                DOB_msel_anime_mapman(entry);
                break;
            case MENU_ITEM_TYPE_05F: /* switch 8 */
            case MENU_ITEM_TYPE_060: /* switch 8 */
            case MENU_ITEM_TYPE_061: /* switch 8 */
            case MENU_ITEM_TYPE_062: /* switch 8 */
                DOB_msel_anime_map2(entry);
                break;
            case MENU_ITEM_TYPE_065: /* switch 8 */
            case MENU_ITEM_TYPE_066: /* switch 8 */
                DOB_msel_bbox(entry);
                break;
            case MENU_ITEM_TYPE_067: /* switch 8 */
                DOB_msel_cup_gsc(entry);
                break;
            case MENU_ITEM_TYPE_078: /* switch 8 */
            case MENU_ITEM_TYPE_079: /* switch 8 */
            case MENU_ITEM_TYPE_07A: /* switch 8 */
            case MENU_ITEM_TYPE_07B: /* switch 8 */
            case MENU_ITEM_TYPE_08C: /* switch 8 */
                DOB_tsel_cup(entry);
                break;
            case MENU_ITEM_TYPE_08D: /* switch 8 */
                DOB_tsel_cursor(entry);
                break;
            case MENU_ITEM_TYPE_07C: /* switch 8 */
            case MENU_ITEM_TYPE_07D: /* switch 8 */
            case MENU_ITEM_TYPE_07E: /* switch 8 */
            case MENU_ITEM_TYPE_07F: /* switch 8 */
            case MENU_ITEM_TYPE_080: /* switch 8 */
            case MENU_ITEM_TYPE_081: /* switch 8 */
            case MENU_ITEM_TYPE_082: /* switch 8 */
            case MENU_ITEM_TYPE_083: /* switch 8 */
            case MENU_ITEM_TYPE_084: /* switch 8 */
            case MENU_ITEM_TYPE_085: /* switch 8 */
            case MENU_ITEM_TYPE_086: /* switch 8 */
            case MENU_ITEM_TYPE_087: /* switch 8 */
            case MENU_ITEM_TYPE_088: /* switch 8 */
            case MENU_ITEM_TYPE_089: /* switch 8 */
            case MENU_ITEM_TYPE_08A: /* switch 8 */
            case MENU_ITEM_TYPE_08B: /* switch 8 */
                DOB_tsel_map(entry);
                break;
            case MENU_ITEM_TYPE_096: /* switch 8 */
                DOB_game_cup(entry);
                break;
            case MENU_ITEM_TYPE_097: /* switch 8 */
                DOB_game_circuit(entry);
                break;
            case MENU_ITEM_TYPE_05E: /* switch 8 */
                DOB_msel_noise(entry);
                break;
            case MENU_ITEM_TYPE_0AA: /* switch 8 */
                DOB_game_time(entry);
                break;
            case MENU_ITEM_TYPE_0AB: /* switch 8 */
                DOB_game_point(entry);
                break;
            case MENU_ITEM_TYPE_0AC: /* switch 8 */
                DOB_gp_lose(entry);
                break;
            case MENU_ITEM_TYPE_0AF: /* switch 8 */
                DOB_info_3p(entry);
                break;
            case MENU_ITEM_TYPE_0B0: /* switch 8 */
                DOB_vs_result(entry);
                break;
            case MENU_ITEM_TYPE_0B1: /* switch 8 */
            case MENU_ITEM_TYPE_0B2: /* switch 8 */
            case MENU_ITEM_TYPE_0B3: /* switch 8 */
            case MENU_ITEM_TYPE_0B4: /* switch 8 */
                DOB_vs_face(entry);
                break;
            case MENU_ITEM_TYPE_0B9: /* switch 8 */
                DOB_game_ta1(entry);
                break;
            case MENU_ITEM_TYPE_0BA: /* switch 8 */
                DOB_game_ta2(entry);
                break;
            case MENU_ITEM_ANNOUNCE_GHOST: /* switch 8 */
                DOB_game_ta_mes1(entry);
                break;
            case MENU_ITEM_PAUSE: /* switch 8 */
                DOB_game_pause(entry);
                break;
            case MENU_ITEM_END_COURSE_OPTION: /* switch 8 */
                DOB_game_ta_replay(entry);
                break;
            case MENU_ITEM_DATA_COURSE_IMAGE: /* switch 8 */
                DOB_dsel_snap(entry);
                break;
            case MENU_ITEM_DATA_COURSE_SELECTABLE: /* switch 8 */
                DOB_dsel_sel1(entry);
                break;
            case MENU_ITEM_TYPE_0E9: /* switch 8 */
                DOB_dsel_sel2(entry);
                break;
            case MENU_ITEM_TYPE_0EA: /* switch 8 */
                DOB_dsel_sel3(entry);
                break;
            case MENU_ITEM_TYPE_0BE: /* switch 8 */
                DOB_ta_fanfare(entry);
                break;
            case MENU_ITEM_TYPE_10E: /* switch 8 */
                DOB_ta_lostghost(entry);
                break;
            case MENU_ITEM_TYPE_12B: /* switch 8 */
                DOB_ending1_halffade(entry);
                break;
            case MENU_ITEM_TYPE_12C: /* switch 8 */
            case MENU_ITEM_TYPE_12D: /* switch 8 */
            case MENU_ITEM_TYPE_12E: /* switch 8 */
            case MENU_ITEM_TYPE_12F: /* switch 8 */
                DOB_ending1_message(entry);
                break;
            case MENU_ITEM_TYPE_130: /* switch 8 */
                DOB_ending1_face(entry);
                break;
            case MENU_ITEM_TYPE_190: /* switch 8 */
            case MENU_ITEM_TYPE_191: /* switch 8 */
            case MENU_ITEM_TYPE_192: /* switch 8 */
            case MENU_ITEM_TYPE_193: /* switch 8 */
            case MENU_ITEM_TYPE_194: /* switch 8 */
            case MENU_ITEM_TYPE_195: /* switch 8 */
            case MENU_ITEM_TYPE_196: /* switch 8 */
            case MENU_ITEM_TYPE_197: /* switch 8 */
            case MENU_ITEM_TYPE_198: /* switch 8 */
            case MENU_ITEM_TYPE_199: /* switch 8 */
            case MENU_ITEM_TYPE_19A: /* switch 8 */
            case MENU_ITEM_TYPE_19B: /* switch 8 */
            case MENU_ITEM_TYPE_19C: /* switch 8 */
            case MENU_ITEM_TYPE_19D: /* switch 8 */
            case MENU_ITEM_TYPE_19E: /* switch 8 */
            case MENU_ITEM_TYPE_19F: /* switch 8 */
            case MENU_ITEM_TYPE_1A0: /* switch 8 */
            case MENU_ITEM_TYPE_1A1: /* switch 8 */
            case MENU_ITEM_TYPE_1A2: /* switch 8 */
            case MENU_ITEM_TYPE_1A3: /* switch 8 */
            case MENU_ITEM_TYPE_1A4: /* switch 8 */
            case MENU_ITEM_TYPE_1A5: /* switch 8 */
            case MENU_ITEM_TYPE_1A6: /* switch 8 */
            case MENU_ITEM_TYPE_1A7: /* switch 8 */
            case MENU_ITEM_TYPE_1A8: /* switch 8 */
            case MENU_ITEM_TYPE_1A9: /* switch 8 */
            case MENU_ITEM_TYPE_1AA: /* switch 8 */
            case MENU_ITEM_TYPE_1AB: /* switch 8 */
            case MENU_ITEM_TYPE_1AC: /* switch 8 */
            case MENU_ITEM_TYPE_1AD: /* switch 8 */
            case MENU_ITEM_TYPE_1AE: /* switch 8 */
            case MENU_ITEM_TYPE_1AF: /* switch 8 */
            case MENU_ITEM_TYPE_1B0: /* switch 8 */
            case MENU_ITEM_TYPE_1B1: /* switch 8 */
            case MENU_ITEM_TYPE_1B2: /* switch 8 */
            case MENU_ITEM_TYPE_1B3: /* switch 8 */
            case MENU_ITEM_TYPE_1B4: /* switch 8 */
            case MENU_ITEM_TYPE_1B5: /* switch 8 */
            case MENU_ITEM_TYPE_1B6: /* switch 8 */
            case MENU_ITEM_TYPE_1B7: /* switch 8 */
            case MENU_ITEM_TYPE_1B8: /* switch 8 */
            case MENU_ITEM_TYPE_1B9: /* switch 8 */
            case MENU_ITEM_TYPE_1BA: /* switch 8 */
            case MENU_ITEM_TYPE_1BB: /* switch 8 */
            case MENU_ITEM_TYPE_1BC: /* switch 8 */
            case MENU_ITEM_TYPE_1BD: /* switch 8 */
            case MENU_ITEM_TYPE_1BE: /* switch 8 */
            case MENU_ITEM_TYPE_1BF: /* switch 8 */
            case MENU_ITEM_TYPE_1C0: /* switch 8 */
            case MENU_ITEM_TYPE_1C1: /* switch 8 */
            case MENU_ITEM_TYPE_1C2: /* switch 8 */
            case MENU_ITEM_TYPE_1C3: /* switch 8 */
            case MENU_ITEM_TYPE_1C4: /* switch 8 */
            case MENU_ITEM_TYPE_1C5: /* switch 8 */
            case MENU_ITEM_TYPE_1C6: /* switch 8 */
            case MENU_ITEM_TYPE_1C7: /* switch 8 */
            case MENU_ITEM_TYPE_1C8: /* switch 8 */
            case MENU_ITEM_TYPE_1C9: /* switch 8 */
            case MENU_ITEM_TYPE_1CA: /* switch 8 */
            case MENU_ITEM_TYPE_1CB: /* switch 8 */
            case MENU_ITEM_TYPE_1CC: /* switch 8 */
            case MENU_ITEM_TYPE_1CD: /* switch 8 */
            case MENU_ITEM_TYPE_1CE: /* switch 8 */
                DOB_staffroll_messages(entry);
                break;
            case MENU_ITEM_UI_NONE:
            case MENU_ITEM_UI_START_BACKGROUND:
            case MENU_ITEM_UI_LOGO_AND_COPYRIGHT:
            case MENU_ITEM_UI_PUSH_START_BUTTON:
            case MENU_ITEM_UI_NO_CONTROLLER:
                break;
        }
    }

    for (j = 0; j < MENU_ITEM_PRIORITY_MAX; j++) {
        for (i = 0; i < ARRAY_COUNT(gMenuItems); i++) {
            isRendered = false;
            entry = &gMenuItems[i];
            if (entry && entry) {} // ?
            type = entry->kind;
            if ((type == MENU_ITEM_UI_NO_CONTROLLER) || (type == MENU_ITEM_UI_START_RECORD_TIME) ||
                (type == MENU_ITEM_PAUSE)) {
                if (priSpecial != 0) {
                    isRendered = true;
                }
            } else if (priSpecial == 0) {
                isRendered = true;
            }
            if ((isRendered != 0) && (j == (s8) entry->pri)) {
                DispOBJBlock(entry);
            }
        }
    }
}

void doOBJBlock1(void) {
    DoObjBlock(0);
}

void doOBJBlock2(void) {
    DoObjBlock(1);
}

void gsel_sub1(s32 arg0, ObjBlock* arg1) {
    s32 temp_t1;
    s32 temp_t6;
    s32 var_s0;
    s32 var_s2;
    s32 var_s3;
    s32 var_s4;

    if (arg1->work1 < 0x20) {
        temp_t6 = (arg1->work1 << 6) / 64;
        temp_t1 = arg1->disp_x;
        var_s0 = arg1->disp_y;
        var_s3 = temp_t1 + temp_t6;
        var_s4 = (temp_t1 - temp_t6) + 0x3F;
        gDPPipeSync(gDisplayListHead++);
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        if ((arg0 + 1) == g_menuMultiplayerSelection) {
            if ((menuScreenB == MAIN_MENU_OPTION) || (menuScreenB == MAIN_MENU_DATA) ||
                (menuScreenB == MAIN_MENU_PLAYER_SELECT)) {
                gDisplayListHead = FlashRectangle2(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35);
            } else {
                gDisplayListHead = FillRect1Color(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35, 0x000000FF,
                                                 0x000000F9, 0x000000DC, 0x000000FF);
            }
        } else {
            gDisplayListHead = FillRectBlack(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35);
        }
        for (var_s0 += 0x41, var_s2 = 0; var_s2 <= gPlayerModeSelection[arg0]; var_s2++, var_s0 += 0x12) {
            if ((var_s2 == gGameModeMenuColumn[arg0]) && ((arg0 + 1) == g_menuMultiplayerSelection) &&
                (menuScreenB > MAIN_MENU_PLAYER_SELECT)) {
                if (menuScreenB == MAIN_MENU_MODE_SELECT) {
                    gDisplayListHead =
                        FlashRectangle2(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11);
                } else {
                    gDisplayListHead = FillRect1Color(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11,
                                                     0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                }
            } else {
                gDisplayListHead =
                    FillRect1Color(gDisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11, 1, 1, 1, 0x000000FF);
            }
        }
    }
}

void sel_ok_sub(ObjBlock* arg0) {
    s32 sp34;
    s32 var_a1;
    MenuTexture* var_a0;
    s32 temp_a2;
    s32 temp_t0;

    var_a1 = 0;
    switch (arg0->kind) { /* irregular */
        case 0xF:
            var_a0 = D_0200487C;
            if ((menuScreenB == MAIN_MENU_OK_SELECT) || (menuScreenB == MAIN_MENU_OK_SELECT_GO_BACK)) {
                var_a1 = 1;
            }
            break;
        case 0x33:
            var_a0 = D_02004B74;
            if ((menuScreenC == PLAYER_SELECT_MENU_OK) ||
                (menuScreenC == PLAYER_SELECT_MENU_OK_GO_BACK)) {
                var_a1 = 1;
            }
            break;
        case 0x5D:
            var_a0 = D_02004E80;
            if (menuScreenA == SUB_MENU_MAP_SELECT_OK) {
                var_a1 = 1;
            }
            break;
        default:
            return;
    }
    if (arg0->work1 < 0x20) {
        sp34 = (arg0->work1 << 5) / 64;
        var_a0 = GetTBPointer(var_a0);
        temp_t0 = arg0->disp_x + var_a0->dX;
        temp_a2 = arg0->disp_y + var_a0->dY;
        if (var_a1 != 0) {
            gDisplayListHead = FlashRectangle2(gDisplayListHead, temp_t0 + sp34, temp_a2,
                                                           (temp_t0 - sp34) + 0x1E, temp_a2 + 0x12);
        } else {
            gDisplayListHead = FillRect1Color(gDisplayListHead, temp_t0 + sp34, temp_a2, (temp_t0 - sp34) + 0x1E,
                                             temp_a2 + 0x12, 1, 1, 1, 0x000000FF);
        }
    }
}

void msel_bbox_sub(ObjBlock* arg0) {
    gDisplayListHead =
        FillRect1Color(gDisplayListHead, arg0->disp_x, arg0->disp_y, arg0->disp_x + 0x64, arg0->disp_y + 0x27, 1, 1, 1, 0xFF);
}

// Credit for the use of ternary operators goes to LLONSIT
void msel_bbox_sub2(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    char buffer[3];
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    s32 temp_s1;
    UNUSED u32 var_v0;
    u32 var_s2;
    set_text_color(TEXT_GREEN);
    var_s2 = arg0->kind == MENU_ITEM_TYPE_065 ? GetRecordTime2(0) : GetRecordLapTime();
    temp_s1 = var_s2 & 0xFFFFF;
    kawano_itoa_m((temp_s1 ^ 0), buffer);
    text_draw(arg0->disp_x + 5, arg0->disp_y + 0x21, buffer, 0, 0.6f, 0.65f);
    SetWord2A(arg0->disp_x + 0xE, arg0->disp_y + 0x21, "'", 0, 0.6f, 0.65f);
    kawano_itoa_s(temp_s1, buffer);
    text_draw(arg0->disp_x + 0x16, arg0->disp_y + 0x21, buffer, 0, 0.6f, 0.65f);
    SetWord2A(arg0->disp_x + 0x20, arg0->disp_y + 0x21, "\"", 0, 0.6f, 0.65f);
    kawano_itoa_c(temp_s1, buffer);
    text_draw(arg0->disp_x + 0x29, arg0->disp_y + 0x21, buffer, 0, 0.6f, 0.65f);
    var_s2 = (u32) temp_s1 < 0x927C0U ? var_s2 >> 0x14 : 8;
    SetWord3A(arg0->disp_x + 0x60, arg0->disp_y + 0x21, drivers_name_j[var_s2], 0, 0.6f, 0.65f);
}

void msel_sub1(s32 arg0, ObjBlock* arg1) {
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t7;

    if (arg1->work1 < 32) {
        if (1) {}
        temp_t7 = (arg1->work1 * 65) / 64;
        temp_t1 = arg1->disp_x;
        temp_a2 = arg1->disp_y;
        gDPPipeSync(gDisplayListHead++);
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        if (arg0 == g_cupSelect) {
            if (menuScreenA == SUB_MENU_MAP_SELECT_CUP) {
                gDisplayListHead = FlashRectangle2(gDisplayListHead, temp_t1 + temp_t7, temp_a2,
                                                               (temp_t1 - temp_t7) + 64, temp_a2 + 39);
            } else {
                gDisplayListHead = FillRect1Color(gDisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 64,
                                                 temp_a2 + 39, 255, 249, 220, 255);
            }
        } else {
            gDisplayListHead = FillRect1Color(gDisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 64,
                                             temp_a2 + 39, 1, 1, 1, 255);
        }
    }
}

void msel_sub2(ObjBlock* arg0) {
    s32 temp_s2;
    s32 temp_s3;
    s32 someIndex;

    temp_s2 = arg0->disp_x;
    temp_s3 = arg0->disp_y;
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    for (someIndex = 0; someIndex < NUM_COURSES_PER_CUP; someIndex++) {
        if ((someIndex == g_courseSelect) && (menuScreenA > SUB_MENU_MAP_SELECT_CUP) &&
            (g_gameMode != GRAND_PRIX)) {
            if ((menuScreenA == SUB_MENU_MAP_SELECT_COURSE) ||
                (menuScreenA == SUB_MENU_MAP_SELECT_BATTLE_COURSE)) {
                gDisplayListHead = FlashRectangle2(
                    gDisplayListHead, lmsel_circuit_position[someIndex][0].x + temp_s2, lmsel_circuit_position[someIndex][0].y + temp_s3,
                    lmsel_circuit_position[someIndex][1].x + temp_s2, lmsel_circuit_position[someIndex][1].y + temp_s3);
            } else {
                gDisplayListHead = FillRect1Color(
                    gDisplayListHead, lmsel_circuit_position[someIndex][0].x + temp_s2, lmsel_circuit_position[someIndex][0].y + temp_s3,
                    lmsel_circuit_position[someIndex][1].x + temp_s2, lmsel_circuit_position[someIndex][1].y + temp_s3, 0x000000FF,
                    0x000000F9, 0x000000DC, 0x000000FF);
            }
        } else {
            gDisplayListHead = FillRect1Color(
                gDisplayListHead, lmsel_circuit_position[someIndex][0].x + temp_s2, lmsel_circuit_position[someIndex][0].y + temp_s3,
                lmsel_circuit_position[someIndex][1].x + temp_s2, lmsel_circuit_position[someIndex][1].y + temp_s3, 1, 1, 1, 0x000000FF);
        }
    }
}

void msel_sub4(ObjBlock* arg0) {
    s32 temp_s2;
    s32 temp_s3;
    s32 var_s0;
    ObjBlock* temp_v0;

    temp_v0 = Get_OBJBlock_ptr(MENU_ITEM_TYPE_064);
    temp_s2 = arg0->disp_x;
    temp_s3 = arg0->disp_y;
    gDPPipeSync(gDisplayListHead++);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    if (g_gameMode == GRAND_PRIX) {
        if (menuScreenA != SUB_MENU_MAP_SELECT_OK) {
            for (var_s0 = 0; var_s0 < 4; var_s0++) {
                // Wut?
                if ((var_s0 != (temp_v0->work1 % 4)) != 0) {
                    gDisplayListHead =
                        FillRect1ColorF(gDisplayListHead, lmsel_circuit_position[var_s0][0].x + temp_s2,
                                 lmsel_circuit_position[var_s0][0].y + temp_s3, lmsel_circuit_position[var_s0][1].x + temp_s2,
                                 lmsel_circuit_position[var_s0][1].y + temp_s3, 0, 0, 0, 0x00000064);
                }
            }
        }
    }
}

void msel_battle_sub(UNUSED ObjBlock* arg0) {
    set_text_color(TEXT_YELLOW);
    print_text1_center_mode_1(0x98, 0x44, msel_battle_messages_j[0], 0, 1.0f, 1.0f);
    SetWord2A(0x17, 0x58, msel_battle_messages_j[1], 0, 0.7f, 0.8f);
    SetWord2A(0x17, 0x6A, msel_battle_messages_j[2], 0, 0.7f, 0.8f);
}

void DispOB_Sub_msel_cup(ObjBlock* arg0) {
    if (arg0->work2 != 0) {
        doSeqData2(arg0->sd_number, arg0->disp_x, arg0->disp_y, -1);
        set_text_color(TEXT_YELLOW);
        SetWord2A(arg0->disp_x + 0x20, arg0->disp_y + 0x28, cup_type_names[arg0->work2], 0, 0.7f, 0.7f);
    }
}

void DispOB_Sub_msel_ghost(UNUSED ObjBlock* arg0) {
    UNUSED Gfx* temp_v0_2;
    POSXY* temp_v0;
    s16 temp_s0;
    s16 temp_v1;
    s32 temp_s2;
    s32 var_s1;

    switch (menuScreenA) { /* irregular */
        case SUB_MENU_MAP_SELECT_CUP:
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                if (CheckSameMap2((g_cupSelect * 4) + var_s1) >= 0) {
                    temp_v0 = &msel_map4box_position[var_s1];
                    temp_v1 = temp_v0->x;
                    temp_s0 = temp_v0->y;
                    temp_s2 = temp_v1 + 0x20;
                    gDisplayListHead =
                        FillRectBlack(gDisplayListHead, temp_s2, (s32) temp_s0, temp_v1 + 0x3F, temp_s0 + 9);
                    gDisplayListHead = doTexList5(gDisplayListHead, GetTBPointer(D_02004A0C), temp_s2,
                                                     (s32) temp_s0, 2);
                }
            }
            break;
        case SUB_MENU_MAP_SELECT_COURSE:
        default:
            if (CheckSameMap2((g_cupSelect * 4) + g_courseSelect) >= 0) {
                gDisplayListHead = FillRectBlack(gDisplayListHead, 0x00000057, 0x00000070, 0x00000096, 0x00000081);
                gDisplayListHead = doTexList(gDisplayListHead, D_02004A0C, 0x00000057, 0x00000070);
            }
            break;
    }
}

void tsel_sub1(UNUSED s32 arg0, ObjBlock* arg1) {
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t7;

    if (arg1->work1 < 0x20) {
        if (1) {}
        temp_t7 = (arg1->work1 * 0x41) / 0x40;
        temp_t1 = arg1->disp_x;
        temp_a2 = arg1->disp_y;
        gDPPipeSync(gDisplayListHead++);
        gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(gDisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        gDisplayListHead = FillRect1Color(gDisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 0x40,
                                         temp_a2 + 0x27, 1, 1, 1, 0xFF);
    }
}

void DOB_moving1(ObjBlock* arg0, s32 columnTarget, s32 rowTarget) {
    DOB_moving1_x(arg0, columnTarget);
    DOB_moving1_y(arg0, rowTarget);
}

void DOB_moving1_x(ObjBlock* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->disp_x;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
        }
    }
    arg0->disp_x += step;
}

void DOB_moving1_y(ObjBlock* arg0, s32 rowTarget) {
    s32 step = rowTarget - arg0->disp_y;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
        }
    }
    arg0->disp_y += step;
}

void DOB_moving2_x(ObjBlock* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->disp_x;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
            if (step < 8) {
                step = 8;
            }
            arg0->disp_x += step;
            if (columnTarget < arg0->disp_x) {
                arg0->disp_x = columnTarget;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
            if (step < -8) {
                step = -8;
            }
            arg0->disp_x += step;
            if (arg0->disp_x < columnTarget) {
                arg0->disp_x = columnTarget;
            }
        }
    }
}

// Appears to be a copy of DOB_moving1_y
UNUSED void DOB_moving2_y(ObjBlock* arg0, s32 rowTarget) {
    s32 step = rowTarget - arg0->disp_y;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
        }
    }
    arg0->disp_y += step;
}

void DOB_moving3_x(ObjBlock* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->disp_x;

    if (step != 0) {
        step = 0xC8 / step;
        if (step > 0) {
            if (step >= 0x19) {
                step = 0x18;
            }
            if (step < 0x10) {
                step = 0x10;
            }
            arg0->disp_x += step;
            if (columnTarget < arg0->disp_x) {
                arg0->disp_x = columnTarget;
            }
        } else {
            if (step < -0x18) {
                step = -0x18;
            }
            if (step >= -0xF) {
                step = -0x10;
            }
            arg0->disp_x += step;
            if (arg0->disp_x < columnTarget) {
                arg0->disp_x = columnTarget;
            }
        }
    }
}

void DOB_moving4_x(ObjBlock* arg0, s32 columnTarget, s32 arg2) {
    s32 step;

    if (columnTarget == arg0->disp_x) {
        arg0->disp_x += arg2;
    } else {
        step = columnTarget - arg0->disp_x;
        if (step != 0) {
            if (step > 0) {
                step = (step / 4) + 1;
                if (step >= 0x11) {
                    step = 0x10;
                }
            } else {
                step = (step / 4) - 1;
                if (step < -0x10) {
                    step = -0x10;
                }
            }
        }
        arg0->disp_x -= step;
    }
}

void DOB_cp_cscolor(ObjBlock* arg0) {
    // Cycle lasts 26 (0x1A) frames
    if (arg0->pattern == 0) {
        // Move highlight from yellow to red
        arg0->work2 = (s32) (u32) ((((f32) (0xC - arg0->work1) * 127.0f) / 12.0f) + 128.0f);
    } else {
        // Move highlight from red to  yellow
        arg0->work2 = (s32) (u32) ((((f64) (f32) arg0->work1 * 127.0) / 12.0) + 128.0);
    }
    arg0->work1++;
    if (arg0->work1 >= 0xC) {
        // Every 13 (0xC) frames reverse direction
        arg0->work1 = 0;
        arg0->pattern ^= 1;
    }
}

void DOB_cp_mes2(ObjBlock* arg0) {
    s32 phi_v0;

    switch (gControllerPakMenuSelection) {
        case CONTROLLER_PAK_MENU_ERASE:
        case CONTROLLER_PAK_MENU_QUIT:
            phi_v0 = 1;
            break;
        case CONTROLLER_PAK_MENU_GO_TO_ERASING:
        case CONTROLLER_PAK_MENU_ERASING:
            phi_v0 = 5;
            break;
        case CONTROLLER_PAK_MENU_ERASE_ERROR_NOT_ERASED:
            phi_v0 = 2;
            break;
        case CONTROLLER_PAK_MENU_ERASE_ERROR_NO_PAK:
            phi_v0 = 3;
            break;
        case CONTROLLER_PAK_MENU_ERASE_ERROR_PAK_CHANGED:
            phi_v0 = 4;
            break;
        default:
            phi_v0 = 0;
            break;
    }
    if (phi_v0 != arg0->pattern) {
        arg0->pattern = phi_v0;
        ChangeSeqData(arg0->sd_number, 0, GetVAddress(D_800E7D34[phi_v0]));
    }
}

void DOB_cp_gnote(ObjBlock* arg0) {
    s32 i;

    switch (gControllerPakScrollDirection) {
        case CONTROLLER_PAK_SCROLL_DIR_DOWN:
            arg0->disp_y -= 2;
            if (arg0->disp_y < 0x60) {
                arg0->disp_y = 0x69;
                gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE;
                for (i = 0; i < 8; i++) {
                    if (i < 7) {
                        gControllerPakVisibleTableRows[i] = gControllerPakVisibleTableRows[i + 1];
                    } else {
                        if ((gControllerPakVisibleTableRows[i - 1] == 0x10) ||
                            (gControllerPakVisibleTableRows[i - 1] == 0)) {
                            gControllerPakVisibleTableRows[i] = 0;
                        } else {
                            gControllerPakVisibleTableRows[i] = gControllerPakVisibleTableRows[i - 1] + 1;
                        }
                    }
                }
                gControllerPakVisibleTableRows[0] = gControllerPakVisibleTableRows[8] = 0;
            }

            break;
        case CONTROLLER_PAK_SCROLL_DIR_UP:
            arg0->disp_y += 2;
            if (arg0->disp_y >= 0x73) {
                arg0->disp_y = 0x69;
                gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE;
                for (i = 8; i > 0; i--) {
                    if (i > 1) {
                        gControllerPakVisibleTableRows[i] = gControllerPakVisibleTableRows[i - 1];
                    } else {
                        if ((gControllerPakVisibleTableRows[i + 1] == 1) ||
                            (gControllerPakVisibleTableRows[i + 1] == 0)) {
                            gControllerPakVisibleTableRows[i] = 0;
                        } else {
                            gControllerPakVisibleTableRows[i] = gControllerPakVisibleTableRows[i + 1] - 1;
                        }
                    }
                }
                gControllerPakVisibleTableRows[0] = gControllerPakVisibleTableRows[8] = 0;
                break;
                default:
                    gControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE;
                    break;
            }
    }
} // todo: <-- fix these brackets

// Don't know if this belongs here or inside DOB_sel_ok
//   as a `static const`. But this matches so we'll leave
//   it here for now.
const s8 gsel_mode_to_ok_mode[] = { 0x03, 0x03, 0x03, 0x02, 0x00, 0x02, 0x02, 0x01 };

const s8 psel_mode_to_ok_mode[] = { 0x03, 0x02, 0x00 };

const s8 msel_mode_to_ok_mode[] = { 0x03, 0x03, 0x02 };

void DOB_sel_ok(ObjBlock* arg0) {
    s32 sp4;
    s32 var_v0;

    switch (arg0->kind) {
        default:
            var_v0 = sp4; // wut?
            break;
        case MENU_ITEM_UI_OK:
            var_v0 = gsel_mode_to_ok_mode[menuScreenB - 1];
            break;
        case COURSE_SELECT_OK:
            var_v0 = msel_mode_to_ok_mode[menuScreenA - 1];
            break;
        case CHARACTER_SELECT_MENU_OK:
            var_v0 = psel_mode_to_ok_mode[menuScreenC - 1];
            break;
    }
    switch (var_v0) {
        case 0:
            arg0->work1 = 0;
            break;
        case 1:
            arg0->work1 = 0x00000020;
            break;
        case 2:
            if (arg0->work1 > 0) {
                arg0->work1 = (arg0->work1 - (arg0->work1 / 12)) - 2;
                if (arg0->work1 < 0) {
                    arg0->work1 = 0;
                }
            }
            break;
        case 3:
            if (arg0->work1 < 0x20) {
                arg0->work1 += 2;
                if (arg0->work1 >= 0x20) {
                    arg0->work1 = 0x00000020;
                }
            }
            break;
    }
}

void DOB_gsel_number(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            DOB_gsel_start(arg0);
            break;
        case 1:
            DOB_gsel_start(arg0);
            arg0->pattern = 4;
            /* fallthrough */
        case 4:
            if (arg0->work1 > 0) {
                arg0->work1 = (arg0->work1 - (arg0->work1 / 12)) - 2;
                if (arg0->work1 < 0) {
                    arg0->work1 = 0;
                }
            } else {
                arg0->work1 = 0;
                arg0->pattern = 0;
            }
            break;
        case 2:
            arg0->pattern = 3;
            break;
        case 3:
        default:
            break;
    }
}

void DOB_gsel_mode(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            DOB_gsel_start(arg0);
            if ((g_menuMultiplayerSelection + 0xA) == arg0->kind) {
                arg0->pattern = 2;
            } else {
                arg0->pattern = 1;
            }
            break;
        case 4:
            if ((g_menuMultiplayerSelection + 0xA) == arg0->kind) {
                arg0->pattern = 2;
                arg0->work1 = 0;
                break;
            }
            arg0->pattern = 1;
        case 1:
            DOB_gsel_start(arg0);
            if ((menuScreenB == MAIN_MENU_OK_SELECT_GO_BACK) ||
                (menuScreenB == MAIN_MENU_MODE_SUB_SELECT_GO_BACK)) {
                arg0->work1 = 0x00000020;
            } else {
                if (arg0->work1 < 0x20) {
                    arg0->work1 += 2;
                    if (arg0->work1 >= 0x20) {
                        arg0->work1 = 0x00000020;
                    }
                }
            }
            break;
        case 3:
            if ((g_menuMultiplayerSelection + 0xA) == arg0->kind) {
                arg0->pattern = 2;
            }
            break;
        case 2:
        default:
            break;
    }
}

void DOB_gsel_move(ObjBlock* arg0) {
    POSXY* temp_v0;

    if ((g_menuMultiplayerSelection + 0xA) == arg0->kind) {
        arg0->pri = 0x0A;
    } else {
        arg0->pri = 6;
    }

    switch (arg0->pattern) {
        case 2:
            if ((menuScreenB == MAIN_MENU_OK_SELECT_GO_BACK) ||
                (menuScreenB == MAIN_MENU_MODE_SUB_SELECT_GO_BACK)) {
                arg0->disp_x = 0x00000080;
                arg0->disp_y = 0x0000003E;
            } else {
                DOB_moving1(arg0, 0x00000080, 0x0000003E);
            }
            break;
        case 3:
            temp_v0 = &lgsel_default_positions[arg0->kind - 0xA];
            DOB_moving1(arg0, temp_v0->x, temp_v0->y);
            if ((arg0->disp_x == temp_v0->x) && (arg0->disp_y == temp_v0->y)) {
                arg0->pattern = 0;
            }
            /* fallthrough */
        case 0:
        case 1:
        case 4:
        default:
            break;
    }
}

void DOB_gsel_gp(ObjBlock* arg0) {
    ObjBlock* temp_v0;
    POSXY* temp_v1_2;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 temp_a1;

    switch (arg0->kind) {
        case 18:
        case 19:
        case 20:
        case 21:
            sp24 = 18;
            sp1C = 2;
            sp20 = 0;
            break;
        case 22:
        case 23:
            sp24 = 22;
            sp20 = 2;
            sp1C = 2;
            break;
        case 24:
        case 25:
            sp24 = 24;
            sp20 = 1;
            sp1C = 1;
            break;
    }

    temp_a1 = gGameModePlayerSelection[g_menuMultiplayerSelection - 1][gGameModeMenuColumn[g_menuMultiplayerSelection - 1]];
    switch (arg0->pattern) { /* switch 5; irregular */
        case 0:            /* switch 5 */
            if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                arg0->disp_on = 0;
            } else {
                arg0->work2 = gGameModeMenuColumn[g_menuMultiplayerSelection - 1];
                switch (menuScreenB) {       /* switch 2 */
                    case MAIN_MENU_MODE_SELECT:     /* switch 2 */
                    case MAIN_MENU_MODE_SUB_SELECT: /* switch 2 */
                    case MAIN_MENU_OK_SELECT:       /* switch 2 */
                        arg0->disp_on = 1;
                        temp_v0 = Get_gsel_cursor_ptr();
                        arg0->disp_x = temp_v0->disp_x;
                        arg0->disp_y = (gGameModeMenuColumn[g_menuMultiplayerSelection - 1] * 0x12) + temp_v0->disp_y + 0x41;
                        arg0->work1 = 0;
                        arg0->pattern = 1;
                        break;
                    case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 2 */
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 2 */
                        temp_v0 = Get_gsel_cursor_ptr();
                        temp_v1_2 = &gsel_popup_positions[arg0->kind - sp24];
                        arg0->disp_x = temp_v0->disp_x + temp_v1_2->x;
                        arg0->disp_y = (gGameModeMenuColumn[g_menuMultiplayerSelection - 1] * 0x12) + temp_v0->disp_y + temp_v1_2->y;
                        arg0->work1 = arg0->disp_y - temp_v0->disp_y;
                        arg0->disp_on = 1;
                        arg0->pattern = 2;
                        break;
                    default: /* switch 2 */
                        arg0->disp_on = 0;
                        break;
                }
            }
            break;
        case 1:                                         /* switch 5 */
            switch (menuScreenB) {               /* switch 3 */
                case MAIN_MENU_MODE_SELECT:             /* switch 3 */
                case MAIN_MENU_MODE_SUB_SELECT:         /* switch 3 */
                case MAIN_MENU_OK_SELECT:               /* switch 3 */
                case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 3 */
                case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 3 */
                    if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                        arg0->disp_on = 0;
                        arg0->pattern = 0;
                    } else {
                        if (arg0->work2 != gGameModeMenuColumn[g_menuMultiplayerSelection - 1]) {
                            arg0->pattern = 0;
                        }
                        temp_v0 = Get_gsel_cursor_ptr();
                        arg0->disp_x = temp_v0->disp_x + arg0->work1;
                        temp_v1_2 = &gsel_popup_positions[arg0->kind - sp24];
                        DOB_moving2_x(arg0, temp_v0->disp_x + temp_v1_2->x);
                        arg0->work1 = arg0->disp_x - temp_v0->disp_x;
                        if (arg0->work1 == temp_v1_2->x) {
                            arg0->pattern = 2;
                            arg0->work1 = arg0->disp_y - temp_v0->disp_y;
                        }
                    }
                    break;
                default: /* switch 3 */
                    arg0->disp_on = 0;
                    arg0->pattern = 0;
                    break;
            }
            break;
        case 2:                                         /* switch 5 */
            switch (menuScreenB) {               /* switch 4 */
                case MAIN_MENU_MODE_SELECT:             /* switch 4 */
                case MAIN_MENU_MODE_SUB_SELECT:         /* switch 4 */
                case MAIN_MENU_OK_SELECT:               /* switch 4 */
                case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 4 */
                case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 4 */
                    if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                        arg0->disp_on = 0;
                        arg0->pattern = 0;
                    } else {
                        if (arg0->work2 != gGameModeMenuColumn[g_menuMultiplayerSelection - 1]) {
                            arg0->pattern = 0;
                        }
                        temp_v0 = Get_gsel_cursor_ptr();
                        temp_v1_2 = &gsel_popup_positions[arg0->kind - sp24];
                        arg0->disp_x = temp_v0->disp_x + temp_v1_2->x;
                        arg0->disp_y = temp_v0->disp_y + arg0->work1;
                        DOB_moving1(arg0, arg0->disp_x,
                                      (gGameModeMenuColumn[g_menuMultiplayerSelection - 1] * 0x12) + temp_v0->disp_y + temp_v1_2->y);
                        arg0->work1 = arg0->disp_y - temp_v0->disp_y;
                    }
                    break;
                default: /* switch 4 */
                    arg0->disp_on = 0;
                    arg0->pattern = 0;
                    break;
            }
            break;
    }
}

void DOB_gsel_start(ObjBlock* arg0) {
    POSXY* temp_v0;

    temp_v0 = &lgsel_default_positions[arg0->kind - 0xA];
    if ((menuScreenB == MAIN_MENU_OK_SELECT_GO_BACK) ||
        (menuScreenB == MAIN_MENU_MODE_SUB_SELECT_GO_BACK)) {
        arg0->disp_x = temp_v0->x;
        arg0->disp_y = temp_v0->y;
    } else {
        DOB_moving1(arg0, temp_v0->x, temp_v0->y);
    }
}

void DOB_gsel_caution(ObjBlock* arg0) {
    s32 temp_v0;
    s32 var_t1;

    var_t1 = 0;
    switch (menuScreenB) {
        case MAIN_MENU_OPTION:
        case MAIN_MENU_DATA:
        case MAIN_MENU_PLAYER_SELECT:
        case MAIN_MENU_MODE_SELECT:
            arg0->pattern = 0;
            break;
        case MAIN_MENU_MODE_SUB_SELECT:
        case MAIN_MENU_OK_SELECT:
        case MAIN_MENU_OK_SELECT_GO_BACK:
        case MAIN_MENU_MODE_SUB_SELECT_GO_BACK:
            if (arg0->pattern != 0) {
                break;
            }
            if (g_menuMultiplayerSelection != 1) {
                break;
            }
            if (gGameModeMenuColumn[g_menuMultiplayerSelection - 1] != 1) {
                break;
            }

            if (gControllerPak1State != 0) {
                var_t1 = 0;
                switch (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                      (u8*) gExtCode, &gControllerPak1FileNote)) {
                    case 5:
                        break;
                    case 0:
                        arg0->pattern = 1;
                        var_t1 = 1;
                        break;
                    case 2:
                        gControllerPak1State = 0;
                        break;
                    default:
                        gControllerPak1State = 0;
                        break;
                }
            }
            if (var_t1 == 0) {
                if (gControllerPak1State == 0) {
                    if (getExistPak(0) == 0) {
                        arg0->pattern = 2;
                        break;
                    }
                    temp_v0 = osPfsInit(&gSIEventMesgQueue, &gControllerPak1FileHandle, 0);
                    if (temp_v0 != 0) {
                        switch (temp_v0) {
                            case PFS_ERR_NOPACK:
                            case PFS_ERR_DEVICE:
                                arg0->pattern = 2;
                                break;
                            case PFS_ERR_ID_FATAL:
                                arg0->pattern = 3;
                                break;
                            case PFS_ERR_CONTRFAIL:
                            default:
                                arg0->pattern = 3;
                                break;
                        }
                        return;
                    } else {
                        gControllerPak1State = 1;
                    }
                    if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                      (u8*) gExtCode, &gControllerPak1FileNote) == 0) {
                        arg0->pattern = 1;
                        break;
                    }
                    if (osPfsNumFiles(&gControllerPak1FileHandle, &gControllerPak1NumFilesUsed,
                                      &gControllerPak1MaxWriteableFiles) != 0) {
                        arg0->pattern = 3;
                        break;
                    }
                    if (osPfsFreeBlocks(&gControllerPak1FileHandle, &gControllerPak1NumPagesFree) != 0) {
                        arg0->pattern = 3;
                        break;
                    }
                    gControllerPak1NumPagesFree = (s32) gControllerPak1NumPagesFree >> 8;
                }
                if (gControllerPak1MaxWriteableFiles >= gControllerPak1NumFilesUsed) {
                    arg0->pattern = 5;
                    break;
                }
                if (gControllerPak1NumPagesFree >= 0x79) {
                    arg0->pattern = 1;
                    break;
                }
                arg0->pattern = 5;
            }
            break;
        default:
            break;
    }
}

void DOB_psel_anime_mario(ObjBlock* arg0, s8 arg1) {
    s32 temp_v1;

    temp_v1 = arg0->kind - 0x2B;
    switch (arg0->pattern2) { /* irregular */
        case 0:
            if (Get_psel_cursor_number((s32) arg1) >= 0) {
                arg0->pattern2 = 2;
                ChangeSeqData(arg0->sd_number, 0, GetVAddress(TBSeq_psel_return[temp_v1]));
            }
            break;
        case 2:
            if (Get_psel_cursor_number((s32) arg1) < 0) {
                arg0->pattern2 = 0;
                ChangeSeqData(arg0->sd_number, 0, GetVAddress(D_800E8320[temp_v1]));
            }
            break;
    }
}

void DOBPSelAnimeKuppa(ObjBlock* arg0) {
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0;
    s32 temp_a0;
    UNUSED s32 stackPadding0;

    temp_a0 = arg0->kind - 0x2B;
    temp_v0 = Get_psel_cursor_number(arg0->kind - 0x2B);
    if (temp_v0 >= 0) {
        var_a0 = 1;
    } else {
        var_a0 = 0;
    }
    if (ok_PSelSkipAnime != 0) {
        var_a0 = 0;
    }
    switch (arg0->pattern2) {
        case 0:
            if ((gCharacterGridIsSelected[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->pattern2 = 1;
                ChangeSeqData(arg0->sd_number, 0,
                              GetVAddress(TBSeq_psel_p1[temp_a0]));
            } else {
                temp_v0 = MakeRandomLimmit(0x00C8U);
                if (temp_v0 >= 0xC6) {
                    arg0->pattern2 = 4;
                    ChangeSeqData(arg0->sd_number, 0,
                                  GetVAddress(TBSeq_psel_p4[temp_a0]));
                } else if (temp_v0 >= 0xC5) {
                    arg0->pattern2 = 5;
                    ChangeSeqData(arg0->sd_number, 0,
                                  GetVAddress(TBSeq_psel_p5[temp_a0]));
                }
            }
            break;
        case 1:
            if (seq_data_block[arg0->sd_number].sequenceIndex >= p1_pat_num[temp_a0]) {
                arg0->pattern2 = 2;
                ChangeSeqData(arg0->sd_number, 0, GetVAddress(TBSeq_psel_p2[temp_a0]));
            } else if ((gCharacterGridIsSelected[temp_v0] == 0) && (var_a0 != 0)) {
                arg0->pattern2 = 3;
                ChangeSeqData(arg0->sd_number,
                              p3_pat_num[temp_a0] - seq_data_block[arg0->sd_number].sequenceIndex,
                              GetVAddress(TBSeq_psel_p3[temp_a0]));
            }
            break;
        case 2:
            if ((gCharacterGridIsSelected[temp_v0] == 0) && (var_a0 != 0)) {
                arg0->pattern2 = 3;
                ChangeSeqData(arg0->sd_number, 0,
                              GetVAddress(TBSeq_psel_p3[temp_a0]));
            }
            break;
        case 3:
            if (seq_data_block[arg0->sd_number].sequenceIndex >= p3_pat_num[temp_a0]) {
                arg0->pattern2 = 0;
                ChangeSeqData(arg0->sd_number, 0, GetVAddress(TBSeq_psel_p0[temp_a0]));
            } else if ((gCharacterGridIsSelected[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->pattern2 = 1;
                ChangeSeqData(arg0->sd_number,
                              p3_pat_num[temp_a0] - seq_data_block[arg0->sd_number].sequenceIndex,
                              GetVAddress(TBSeq_psel_p1[temp_a0]));
            }
            break;
        case 4:
        case 5:
            if ((gCharacterGridIsSelected[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->pattern2 = 1;
                ChangeSeqData(arg0->sd_number, 0,
                              GetVAddress(TBSeq_psel_p1[temp_a0]));
            } else {
                if (arg0->pattern2 == 4) {
                    var_v0 = p4_pat_num[temp_a0];
                } else {
                    var_v0 = p5_pat_num[temp_a0];
                }
                if (seq_data_block[arg0->sd_number].sequenceIndex >= var_v0) {
                    arg0->pattern2 = 0;
                    ChangeSeqData(arg0->sd_number, 0, GetVAddress(TBSeq_psel_p0[temp_a0]));
                }
            }
            break;
        default:
            break;
    }
}

void DOBPSelTurnOut(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 3:
            arg0->pattern = 1;
            /* fallthrough */
        case 1:
            if (menuScreenC == PLAYER_SELECT_MENU_OK_GO_BACK) {
                arg0->work1 = 0x00000020;
            } else {
                if (arg0->work1 < 0x20) {
                    arg0->work1 += (arg0->work1 / 12) + 2;
                    if (arg0->work1 >= 0x20) {
                        arg0->work1 = 0x00000020;
                    }
                }
            }
            /* fallthrough */
        case 0:
            if (Get_psel_cursor_number(arg0->kind - 0x2B) >= 0) {
                arg0->pattern = 2;
                arg0->work1 = 0;
            } else {
                arg0->pattern = 1;
            }
            break;
        case 4:
            if (Get_psel_cursor_number(arg0->kind - 0x2B) >= 0) {
                arg0->pattern = 2;
                arg0->work1 = 0;
            }
            break;
        case 2:
        default:
            break;
    }
}

void DOBPSelTurnIn(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 1:
            if (arg0->work1 > 0) {
                arg0->pattern = 3;
            }
            break;
        case 2:
            arg0->pattern = 4;
            break;
        case 3:
            if (arg0->work1 > 0) {
                arg0->work1 = (arg0->work1 - (arg0->work1 / 12)) - 2;
                if (arg0->work1 < 0) {
                    arg0->work1 = 0;
                }
            } else {
                arg0->work1 = 0;
                arg0->pattern = 0;
            }
            break;
        case 0:
        case 4:
        default:
            break;
    }
}

void DOB_psel_character1(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 temp_a1;
    s32 temp_v0;
    UNUSED s32 stackPadding2;
    POSXY* var_t0;

    temp_a1 = arg0->kind - 0x2B;
    switch (arg0->pattern) {
        case 0:
            if (menuScreenC == PLAYER_SELECT_MENU_OK_GO_BACK) {
                temp_v0 = Get_psel_cursor_number(temp_a1);
                if (temp_v0 >= 0) {
                    var_t0 = &lpsel_character_ok_position[(g_ScreenSplitB * 4) + temp_v0];
                    arg0->disp_x = (s32) var_t0->x;
                    arg0->disp_y = (s32) var_t0->y;
                    arg0->pattern = 2;
                    arg0->pattern2 = 2;
                    ChangeSeqData(arg0->sd_number, 0, GetVAddress(TBSeq_psel_return[temp_a1]));
                }
                break;
            }
        case 1:
        case 3:
            var_t0 = &PlayerSelectPositions[0][temp_a1];
            arg0->disp_x = (s32) var_t0->x;
            arg0->disp_y = (s32) var_t0->y;
            break;
        case 2:
        case 4:
            if (arg0->pattern == 2) {
                temp_v0 = Get_psel_cursor_number(temp_a1);
                if (temp_v0 >= 0) {
                    var_t0 = &lpsel_character_ok_position[(g_ScreenSplitB * 4) + temp_v0];
                }
            } else {
                var_t0 = &PlayerSelectPositions[0][temp_a1];
                if ((var_t0->x == arg0->disp_x) && (var_t0->y == arg0->disp_y)) {
                    arg0->pattern = 0;
                    return;
                }
            }
            if ((arg0->pattern != 2) || (arg0->pattern2 != 1)) {
                DOB_moving1(arg0, (s32) var_t0->x, (s32) var_t0->y);
            }
            break;
        default:
            break;
    }
}

void DOB_psel_cursor1(ObjBlock* arg0) {
    s32 playerId;
    s8 characterSelectionIndex;

    playerId = arg0->kind - CHARACTER_SELECT_MENU_1P_CURSOR;
    characterSelectionIndex = g_player1Character[playerId];
    arg0->pri = 0xE - (playerId * 2);
    Get_psel_cursor_position(arg0, characterSelectionIndex - 1);
}

void DOB_psel_anime_priority(ObjBlock* arg0) {
    s32 temp_v0;

    temp_v0 = Get_psel_cursor_number(arg0->kind - CHARACTER_SELECT_MENU_MARIO);
    if (temp_v0 >= 0) {
        arg0->pri = 0xE - (temp_v0 * 2);
    } else {
        arg0->pri = 6;
    }
}

/**
 * Similar to Get_OBJBlock_ptr, there is potential for a
 * hard lock in the function if no appropriate gMenuItems entry
 * is found.
 **/
ObjBlock* Get_gsel_cursor_ptr(void) {
    ObjBlock* entry = gMenuItems;
    s32 nbPlayer = g_menuMultiplayerSelection - 1;

    for (; !(entry > &gMenuItems[MENU_ITEMS_MAX]); entry++) {
        if ((nbPlayer + MENU_ITEM_UI_1P_GAME) == entry->kind) {
            goto escape;
        }
    }

    // Something VERY wrong has occurred
    while (true) {
        ;
    }
escape:
    return entry;
}

/**
 * Similar to Get_OBJBlock_ptr, there is potential for a
 * hard lock in the function if no appropriate gMenuItems entry
 * is found.
 **/
ObjBlock* Get_psel_character_ptr(s32 characterId) {
    ObjBlock* entry = gMenuItems;

    for (; !(entry > &gMenuItems[MENU_ITEMS_MAX]); entry++) {
        if ((characterId + CHARACTER_SELECT_MENU_MARIO) == entry->kind) {
            goto escape;
        }
    }

    // Something VERY wrong has occurred
    while (true) {
        ;
    }
escape:
    return entry;
}

/**
 * `arg0` is the desired "type" of the 8018D9E0 entry to
 * search for. If no entry with that "type" is found, this
 * function will enter a `while(1)` loop, hard-locking the
 * game.
 *
 * In practice this never appears to happen, but that is
 * probably as much a matter of luck as it is good
 * reasoning on the original author(s) part.
 **/
ObjBlock* Get_OBJBlock_ptr(s32 type) {
    ObjBlock* entry = gMenuItems;
    for (; !(entry > (&gMenuItems[MENU_ITEMS_MAX])); entry++) {
        if (entry->kind == type) {
            goto escape;
        }
    }

    // Something VERY wrong has occurred
    while (true) {
        ;
    }
escape:
    return entry;
}

ObjBlock* Get_OBJBlock_ptr2(s32 type) {
    ObjBlock* entry = gMenuItems;
    for (; !(entry > (&gMenuItems[MENU_ITEMS_MAX])); entry++) {
        if (entry->kind == type) {
            goto escape;
        }
    }

    return NULL;
escape:
    return entry;
}

UNUSED s32 Get_psel_character_pattern(s32 characterId) {
    ObjBlock* temp;
    temp = Get_psel_character_ptr(characterId);
    return temp->pattern;
}

void Get_psel_cursor_position(ObjBlock* arg0, s32 characterId) {
    ObjBlock* temp_v0;

    temp_v0 = Get_psel_character_ptr(characterId);
    arg0->disp_x = temp_v0->disp_x;
    arg0->disp_y = temp_v0->disp_y;
}

s32 Get_psel_cursor_number(s32 characterId) {
    s32 someIndex = 0;
    bool ret = false;

    for (; someIndex < ARRAY_COUNT(g_player1Character); someIndex++) {
        if ((characterId + 1) == g_player1Character[someIndex]) {
            ret = true;
            break;
        }
    }

    if (ret != false) {
        return someIndex;
    }

    return -1;
}

void DOB_msel_cup(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 1:
            arg0->pattern = 4;
            /* fallthrough */
        case 4:
            if (arg0->work1 > 0) {
                arg0->work1 = (arg0->work1 - (arg0->work1 / 12)) - 2;
                if (arg0->work1 < 0) {
                    arg0->work1 = 0;
                }
            } else {
                arg0->work1 = 0;
                arg0->pattern = 0;
            }
            break;
        case 2:
            arg0->pattern = 3;
            break;
        case 0:
        case 3:
        default:
            break;
    }
}

void DOB_msel_map(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            if ((g_cupSelect + 0x53) == arg0->kind) {
                arg0->pattern = 2;
            } else {
                arg0->pattern = 1;
            }
            break;
        case 4:
            if ((g_cupSelect + 0x53) == arg0->kind) {
                arg0->pattern = 2;
                arg0->work1 = 0;
                break;
            } else {
                arg0->pattern = 1;
            }
        // Purposeful fallthrough
        case 1:
            if (arg0->work1 < 32) {
                arg0->work1 += 2;
                if (arg0->work1 >= 32) {
                    arg0->work1 = 32;
                }
            }
            break;
        case 3:
            if ((g_cupSelect + 0x53) == arg0->kind) {
                arg0->pattern = 2;
            }
            break;
        case 2:
        default:
            break;
    }
}

void DOB_msel_move(ObjBlock* arg0) {
    POSXY* thing = &msel_cupbox_position[arg0->kind - 0x53];

    if ((g_cupSelect + 0x53) == arg0->kind) {
        arg0->pri = 0x0A;
    } else {
        arg0->pri = 4;
    }

    switch (arg0->pattern) {
        case 0:
            thing = &msel_cupbox_position[arg0->kind - 0x53];
            arg0->disp_x = thing->x;
            arg0->disp_y = thing->y;
            break;
        case 2:
            DOB_moving1(arg0, 0x00000080, 0x0000003B);
            break;
        case 3:
            thing = &msel_cupbox_position[arg0->kind - 0x53];
            DOB_moving1(arg0, thing->x, thing->y);
            if ((arg0->disp_x == thing->x) && (arg0->disp_y == thing->y)) {
                arg0->pattern = 0;
            }
            break;
        case 1:
        case 4:
        default:
            break;
    }
}

void DOB_msel_name(ObjBlock* arg0) {
    s32 temp = (arg0->kind - 0x58);
    if (temp == g_cupSelect) {
        arg0->disp_on = 1;
    } else {
        arg0->disp_on = 0;
    }
}

void DOB_msel_anime_map2(ObjBlock* arg0) {
    if (arg0->work1 != g_cupSelect) {
        arg0->work1 = g_cupSelect;
        ChangeSeqData(arg0->sd_number, 0,
                      GetVAddress(D_800E7E34[gCupCourseOrder[g_cupSelect][arg0->kind - 0x5F]]));
    }
}

void DOB_msel_anime_mapman(ObjBlock* item) {
    s32 i;
    UNUSED s32 pad[2];
    ObjBlock* _items[4]; // Added _ to help distinguish between the function argument and this

    for (i = 0; i < 4; i++) {
        _items[i] = Get_OBJBlock_ptr(i + 0x5F);
    }
    if (g_gameMode != 0) {
        if (menuScreenA != SUB_MENU_MAP_SELECT_CUP) {
            item->pattern = 0;
            item->work2 = 0;

            for (i = 0; i < 4; i++) {
                if (g_courseSelect == i) {
                    _items[i]->disp_on = 1;
                    if (item->work1 != i) {
                        item->work1 = i;
                    }
                } else {
                    _items[i]->disp_on = 0;
                }
            }
        } else {
            item->pattern = 3;
            for (i = 0; i < 4; i++) {
                _items[i]->disp_on = 1;
                _items[i]->pri = 6;
            }
        }
    } else {
        switch (item->pattern) {
            case 0:
                if (g_cupSelect == (item->work1 / 4)) {
                    if (++item->work2 > 50) {
                        item->pattern = 1;
                        item->work2 = 0;
                        item->work1 = (g_cupSelect * 4) + 1;
                    }
                } else {
                    item->work2 = 0;
                    item->work1 = g_cupSelect * 4;
                }
                if (menuScreenA == SUB_MENU_MAP_SELECT_OK) {
                    item->pattern = 2;
                    item->work2 = 0;
                }
                break;
            case 1:
                if (g_cupSelect != (item->work1 / 4)) {
                    item->pattern = 0;
                    item->work2 = 0;
                    item->work1 = 0;
                    break;
                }

                if (++item->work2 > 30) {
                    item->work2 = 0;
                    item->work1 = (g_cupSelect * 4) + (((item->work1 % 4) + 1) % 4);
                    break;
                }
                if (menuScreenA == SUB_MENU_MAP_SELECT_OK) {
                    item->pattern = 2;
                    item->work2 = 0;
                }
                break;
            case 2:
                if (++item->work2 > 25) {
                    item->pattern = 3;
                    item->work2 = 0;
                }
                if (menuScreenA != SUB_MENU_MAP_SELECT_OK) {
                    item->pattern = 0;
                    item->work2 = 0;
                    item->work1 = 0;
                }
                break;
            case 3:
                if (menuScreenA != SUB_MENU_MAP_SELECT_OK) {
                    item->pattern = 0;
                    item->work2 = 0;
                    item->work1 = 0;
                }
                break;
        }

        switch (item->pattern) {
            case 0:
            case 1:
                for (i = 0; i < 4; i++) {
                    if ((item->work1 % 4) == i) {
                        _items[i]->disp_on = 1;
                    } else {
                        _items[i]->disp_on = 0;
                    }
                    _items[i]->pri = 6;
                }
                break;
            case 2:
                for (i = 0; i < 4; i++) {
                    if ((item->work1 % 4) == i) {
                        _items[i]->pri = 6;
                    } else if (item->work2 < (i * 5)) {
                        _items[i]->pri = 4;
                    } else {
                        _items[i]->pri = 8;
                    }
                    _items[i]->disp_on = 1;
                }
                break;
            case 3:
                for (i = 0; i < 4; i++) {
                    _items[i]->disp_on = 1;
                    _items[i]->pri = 6;
                }
                break;
        }
    }
}

void DOB_msel_bbox(ObjBlock* arg0) {
    POSXY* temp_a1;

    switch (menuScreenA) { /* irregular */
        case SUB_MENU_MAP_SELECT_CUP:
            temp_a1 = &msel_bestbox_default_position[arg0->kind - 0x65];
            if (arg0->disp_x != temp_a1->x) {
                DOB_moving1_x(arg0, temp_a1->x);
            }
            break;
        case SUB_MENU_MAP_SELECT_COURSE:
        case SUB_MENU_MAP_SELECT_OK:
            temp_a1 = &msel_bestbox_target_position[arg0->kind - 0x65];
            if (arg0->disp_x != temp_a1->x) {
                DOB_moving1_x(arg0, temp_a1->x);
            }
            break;
    }
}

void DOB_msel_cup_gsc(ObjBlock* arg0) {
    POSXY* temp_v1;

    if (arg0->work1 != g_cupSelect) {
        arg0->work1 = g_cupSelect;
        arg0->work2 = GetCupStatus((s32) g_cupSelect, g_raceClass);
        ChangeSeqData(arg0->sd_number, 0,
                      GetVAddress(D_800E7E20[((g_raceClass / 2) * 4) - arg0->work2]));
        arg0->disp_x = (s32) msel_cup_positions->x;
        arg0->disp_y = msel_cup_positions->y;
    }
    temp_v1 = &msel_cup_positions[arg0->pattern];
    switch (arg0->pattern) { /* irregular */
        case 0:
            DOB_moving1(arg0, (s32) temp_v1->x, (s32) temp_v1->y);
            if (menuScreenA == SUB_MENU_MAP_SELECT_OK) {
                arg0->pattern = 1;
            }
            break;
        case 1:
            DOB_moving1(arg0, (s32) temp_v1->x, (s32) temp_v1->y);
            if (menuScreenA == SUB_MENU_MAP_SELECT_CUP) {
                arg0->pattern = 0;
            }
            break;
    }
}

void DOB_tsel_cup(ObjBlock* arg0) {
    s32 index;

    if (arg0->kind == 0x8C) {
        index = 4;
    } else {
        index = arg0->kind - 0x78;
    }
    arg0->disp_x = tsel_cup_positions[index].x;
    arg0->disp_y = tsel_cup_positions[index].y;
}

void DOB_tsel_cursor(ObjBlock* arg0) {
    POSXY* temp_v1;
    s32 thing = gTimeTrialDataCourseIndex;

    temp_v1 = &tsel_cup_positions[thing / 4];
    arg0->disp_x = temp_v1->x - 2;
    arg0->disp_y = temp_v1->y + ((thing % 4) * 0x32) + 0x13;
    arg0->work1 += 0x10;
    if (arg0->work1 >= 0x100) {
        arg0->work1 -= 0x100;
        arg0->work2 = (s32) (arg0->work2 + 1) % 3;
    }
}

void DOB_tsel_map(ObjBlock* arg0) {
    s32 temp_v0;
    POSXY* temp_v1;

    temp_v0 = arg0->kind - 0x7C;
    temp_v1 = &tsel_cup_positions[temp_v0 / 4];
    arg0->disp_x = (s32) temp_v1->x;
    arg0->disp_y = temp_v1->y + ((temp_v0 % 4) * 0x32) + 0x14;
}

void DOB_game_cup(ObjBlock* arg0) {
    // Huh?
    s32 one = 1;
    DOB_game_cup_mode1(arg0);
    switch (opening_flag) { /* irregular */
        case 1:
            arg0->disp_on = one;
            break;
        case 2:
            if (arg0->disp_y >= -0x13) {
                arg0->disp_y -= 2;
            } else {
                arg0->kind = 0;
            }
            break;
        default:
            if ((g_gameMode != GRAND_PRIX) || (g_playerCount != (s32) 1U) || (demo_play_flag != 0)) {
                arg0->kind = 0;
            } else {
                if (arg0->disp_y >= -0x13) {
                    arg0->disp_y -= 2;
                } else {
                    arg0->kind = 0;
                }
            }
            break;
    }
}

void DOB_game_cup_mode1(ObjBlock* arg0) {
    f64 temp_f0;

    switch (arg0->pattern) { /* irregular */
        case 0:
            arg0->disp_x = 0;
            arg0->pattern = 1;
            arg0->work2 = (GetWordLength(cup_name[cup_level]) / 2) + 0xA0;
            /* fallthrough */
        case 1:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (s32) (arg0->work2 - arg0->disp_x) / 4;
            if (arg0->work1 >= 9) {
                arg0->work1 = 8;
            }
            arg0->workf1 = (f32) (((f64) arg0->work1 * 0.05) + 1.0);
            if (arg0->disp_x >= (arg0->work2 - 0x14)) {
                arg0->pattern = 2;
                arg0->sd_number = 0;
            }
            break;
        case 2:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (s32) (arg0->work2 - arg0->disp_x) / 4;
            arg0->sd_number++;
            temp_f0 = (f64) (arg0->sd_number - 0xA);
            arg0->workf1 = (f32) ((temp_f0 * 0.0085 * temp_f0) + 0.4);
            if ((arg0->sd_number >= 9) && ((f64) arg0->workf1 > 1.0)) {
                arg0->workf1 = 1.0f;
            }
            break;
    }
}

void DOB_game_circuit(ObjBlock* arg0) {
    s32 why = 1;
    DOB_game_circuit_mode1(arg0);
    switch (opening_flag) {
        case 1:
            arg0->disp_on = why;
            break;
        case 2:
            if (arg0->disp_y < 0x104) {
                arg0->disp_y += 2;
            } else {
                arg0->kind = 0;
            }
            break;
        default:
            if ((g_gameMode != GRAND_PRIX) || (g_playerCount != why) || (demo_play_flag != 0)) {
                arg0->kind = 0;
            } else {
                if (arg0->disp_y < 0x104) {
                    arg0->disp_y += 2;
                } else {
                    arg0->kind = 0;
                }
            }
            break;
    }
}

void DOB_game_circuit_mode1(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = 0x140;
            arg0->pattern = 1;
            arg0->work2 = 0xA0 - (GetWordLength(g_StringTableCourseGP[g_courseID]) / 2);
            /* fallthrough */
        case 1:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (arg0->disp_x - arg0->work2) / 4;
            if (arg0->work1 >= 9) {
                arg0->work1 = 8;
            }
            arg0->workf1 = (arg0->work1 * 0.05) + 1.0;
            if ((arg0->work2 + 0x14) >= arg0->disp_x) {
                arg0->pattern = 2;
                arg0->sd_number = 0;
            }
            break;
        case 2:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (arg0->disp_x - arg0->work2) / 4;
            arg0->sd_number++;
            arg0->workf1 = ((arg0->sd_number - 0xA) * 0.0085 * (arg0->sd_number - 0xA)) + 0.4;
            if ((arg0->sd_number >= 9) && ((f64) arg0->workf1 > 1.0)) {
                arg0->workf1 = 1.0f;
            }
            break;
    }
}

void DOB_game_circuit_mode2(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = 0x00000140;
            arg0->pattern = 1;
            /* fallthrough */
        case 1:
            DOB_moving3_x(arg0, 0x00000064);
            arg0->work1 = (s32) (arg0->disp_x - 0x64) / 6;
            if (arg0->work1 >= 9) {
                arg0->work1 = 8;
            }
            arg0->workf1 = (f32) (((f64) arg0->work1 * 0.07) + 0.6);
            if (arg0->disp_x == 0x00000064) {
                arg0->pattern = 2;
                arg0->sd_number = 0;
            }
            break;
        case 2:
            arg0->sd_number++;
            arg0->work1 = 0;
            arg0->workf1 = (f32) (1.5 - ((arg0->sd_number - 0xF) * 0.004 * (arg0->sd_number - 0xF)));
            if ((arg0->sd_number >= 0x10) && ((f64) arg0->workf1 < 0.8)) {
                arg0->pattern = 3;
                arg0->sd_number = 0;
            }
            break;
        case 3:
            arg0->sd_number++;
            arg0->work1 = 0;
            arg0->workf1 = (f32) (1.25 - ((arg0->sd_number - 0xF) * 0.002 * (arg0->sd_number - 0xF)));
            if ((arg0->sd_number >= 0xD) && ((f64) arg0->workf1 < 1.0)) {
                arg0->workf1 = 1.0f;
            }
            break;
    }
}

void DOB_msel_noise(ObjBlock* arg0) {
    if (arg0->work2 < ++arg0->work1) {
        arg0->kind = 0;
    }
}

void DOB_game_time(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = 0x14A;
            arg0->pattern = 1;
            SkipReset();
            break;
        case 1:
            DOB_moving1_x(arg0, 0xA0);
            if (arg0->disp_x == 0xA0) {
                arg0->pattern = 2;
                arg0->work2 = 0;
            }
            break;
        case 2:
            arg0->work2++;
            if (((k_skip_flg != 0) || (arg0->work2 >= 0x5B)) && (D_800DDB24 != 0)) {
                arg0->pattern = 3;
                arg0->work1 = arg0->disp_x;
                SetObjBlock(MENU_ITEM_TYPE_0AB, 0, 0, MENU_ITEM_PRIORITY_0);
            }
            break;
        case 3:
            arg0->disp_x = arg0->work1;
            if (arg0->work1 < 0x14A) {
                if (k_skip_flg != 0) {
                    arg0->work1 += 0x20;
                } else {
                    arg0->work1 += 0x10;
                }
            } else {
                arg0->kind = 0;
            }
            break;
    }
}

void DOB_game_point(ObjBlock* arg0) {
    s32 var_a1;
    s32 var_t1;
    s32 temp;

    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = -0x000000A0;
            arg0->pattern = 1;
            for (var_a1 = 0; var_a1 < ARRAY_COUNT(additional_point_defaults); var_a1++) {
                additional_point[var_a1] = additional_point_defaults[var_a1];
            }
            arg0->work2 = arg0->disp_x;
            break;
        case 1:
            arg0->disp_x = arg0->work2;

            temp = (k_skip_flg != 0) ? 0x20 : 0x10;

            if ((arg0->work2 + temp) < 0) {
                arg0->work2 += temp;
                D_800DC5EC->posx += temp;
                D_800DC5F0->posx -= temp;
            } else {
                arg0->work2 = 0;
                arg0->disp_x = 0;
                arg0->pattern = 2;
                arg0->work1 = 0;
                D_800DC5EC->posx = 0x00F0;
                D_800DC5F0->posx = 0x0050;
            }
            break;
        case 2:
            arg0->disp_x = 0;
            arg0->work1++;
            if (((k_skip_flg != 0) || (arg0->work1 >= 0x1F)) && (D_800DDB24 != 0)) {
                arg0->pattern = 3;
                arg0->work1 = 0;
                arg0->work2 = 0;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            var_t1 = 0;
            var_a1 = arg0->pattern - 3;
            arg0->work1++;
            if (((arg0->work1 % 3) == 0) || (k_skip_flg != 0)) {
                if (additional_point[var_a1] > 0) {
                    additional_point[var_a1]--;
                    g_playerGPpoints[gPlayers[rank_to_num[var_a1]].kart] += 1;
                    playSound(SOUND_ACTION_COUNT_SCORE);
                    var_t1 = 0;
                    if ((additional_point[var_a1] == 0) && (arg0->work2 == 0)) {
                        arg0->work2 = 1;
                        arg0->work1 = 0;
                    }
                }
            }
            if ((arg0->work2 != 0) && ((arg0->work1 > 0xA) || ((k_skip_flg != 0) && (arg0->work1 >= 4)))) {
                var_t1 = 1;
            }
            if (var_t1 != 0) {
                arg0->work2 = 0;
                arg0->work1 = 0;
                if (arg0->pattern < 6) {
                    arg0->pattern++;
                } else {
                    arg0->pattern = 7;
                }
            }
            break;
        case 7:
            arg0->work1++;
            if ((((k_skip_flg != 0) && (arg0->work1 >= 0xB)) || (arg0->work1 >= 0x3D)) && (D_800DDB24 != 0)) {
                arg0->pattern = 8;
                arg0->work1 = 0;
            }
            break;
        case 8:
            arg0->work1++;
            if (k_skip_flg != 0) {
                arg0->work1 += 5;
            }
            if (arg0->work1 >= 0x29) {
                arg0->pattern = 9;
            }
            break;
        case 9:
            arg0->work1--;
            if (k_skip_flg != 0) {
                arg0->work1 -= 5;
            }
            if (arg0->work1 <= 0) {
                arg0->pattern = 0x0000000A;
                arg0->work1 = 0;
                if (g_courseSelect == 3) {
                    for (var_a1 = 0; var_a1 < 8; var_a1++) {
                        if (name_to_num[d_points_rank[var_a1]] < g_menuMultiplayerSelection) {
                            SaveCupStatus(var_a1);
                            break;
                        }
                    }
                }
            }
            break;
        case 10:
            arg0->work1++;
            if (arg0->work1 > 0) {
                arg0->pattern = 0x0000000B;
                arg0->work1 = 0;
                SkipReset();
            }
            break;
        case 11:
            if ((k_skip_flg != 0) && (D_800DDB24 != 0)) {
                arg0->pattern = 0x0000000C;
                arg0->work2 = arg0->disp_y;
                playSound(SOUND_ACTION_NEXT_COURSE);
            }
            break;
        case 12:
            arg0->disp_y = arg0->work2;
            if (arg0->work2 < 0xF0) {
                arg0->work2 += 0x10;
                D_800DC5EC->posy += 0x10;
                D_800DC5F0->posy -= 0x10;
            } else {
                arg0->work2 = 0;
                arg0->pattern = 0x0000000D;
                arg0->work1 = 0;
                D_800DC5EC->posy = 0x012C;
                D_800DC5F0->posy = -0x003C;
                finish_zoom_flag = 4;
                NaSeqFadeout(0x19U);
            }
            break;
        case 13:
        default:
            break;
    }
}

void DOB_info_3p(ObjBlock* arg0) {
    switch (arg0->pattern) { /* irregular */
        case 0:
            arg0->disp_x = 0x14A;
            arg0->pattern = 1;
            arg0->work1 = 0xFF;
            break;
        case 1:
            DOB_moving1_x(arg0, 0xA0);
            if (arg0->disp_x == 0xA0) {
                arg0->pattern = 2;
            }
            break;
        case 2:
            break;
        case 3:
            if (arg0->work1 != 0) {
                arg0->work1 -= 0x33;
            }
            break;
    }
}

void DOB_gp_lose(ObjBlock* arg0) {
    switch (arg0->pattern) { /* irregular */
        case 0:
            if (arg0->work2 >= 0xB) {
                arg0->work1 += 3;
            }
            if (arg0->work1 >= 0x65) {
                arg0->pattern = 0x0000000B;
                arg0->work1 = 0;
            }
            break;
        case 11:
        case 12:
            if (FadeChecker()) {
                break;
            }

            if ((gControllerFive->ButtonPressed | gControllerFive->AnalogPressed) & U_JPAD) {
                if (arg0->pattern >= 0xC) {
                    arg0->pattern--;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->workf1 < 4.2) {
                        arg0->workf1 += 4.0;
                    }
                    arg0->pattern2 = -1;
                }
            }
            if ((gControllerFive->ButtonPressed | gControllerFive->AnalogPressed) & D_JPAD) {
                if (arg0->pattern < 0xC) {
                    arg0->pattern++;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->workf1 < 4.2) {
                        arg0->workf1 += 4.0;
                    }
                    arg0->pattern2 = 1;
                }
            }
            if (gControllerFive->ButtonPressed & (A_BUTTON | START_BUTTON)) {
                SetFadeOut(0x0000001E);
                NaSeqFadeout(0x19U);
                playSound(SOUND_ACTION_CONTINUE_UNKNOWN);
                if (arg0->workf1 < 4.2) {
                    arg0->workf1 += 4.0;
                }
            }
            break;
        default:
            break;
    }
    if (arg0->work2 < 0x28) {
        arg0->work2++;
    }
}

void DOB_vs_result(ObjBlock* arg0) {
    s32 i;

    switch (arg0->pattern) {
        case 0:
            arg0->work1 += 3;
            if (arg0->work1 >= 0x65) {
                arg0->pattern = 1;
                arg0->work1 = 0;
                for (i = 0; i < g_menuMultiplayerSelection; i++) {
                    SetObjBlock(i + MENU_ITEM_TYPE_0B1, 0, 0, (s8) (MENU_ITEM_PRIORITY_5 - i));
                }
            }
            break;
        case 1:
            if (Get_OBJBlock_ptr(0x000000B1)->pattern >= 2) {
                arg0->pattern = 2;
            }
            break;
        case 2:
            arg0->work1 += 0x20;
            if (arg0->work1 >= 0x100) {
                if (g_gameMode == VERSUS) {
                    arg0->pattern = (s32) vs_result_cursor;
                } else {
                    arg0->pattern = (s32) bt_result_cursor;
                }
                arg0->work1 = 0;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            if (FadeChecker() == 0) {
                if ((gControllerFive->ButtonPressed | gControllerFive->AnalogPressed) & U_JPAD) {
                    if (arg0->pattern >= 0xB) {
                        arg0->pattern--;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = -1;
                    }
                }
                if ((gControllerFive->ButtonPressed | gControllerFive->AnalogPressed) & D_JPAD) {
                    if (arg0->pattern < 0xD) {
                        arg0->pattern++;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = 1;
                    }
                }
                if (gControllerFive->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                    SetFadeOut(0x0000001E);
                    playSound(SOUND_MENU_OK_CLICKED);
                    if (g_gameMode == VERSUS) {
                        vs_result_cursor = (s8) arg0->pattern;
                    } else {
                        bt_result_cursor = (s8) arg0->pattern;
                    }
                    if (arg0->workf1 < 4.2) {
                        arg0->workf1 += 4.0;
                    }
                }
            }
            break;
        default:
            break;
    }
}

void DOB_vs_face(ObjBlock* arg0) {
    POSXY* temp_v0_2;
    s32 somePlayerIndex;
    s32 temp_a1;
    s32 var_v1;
    UNUSED s32 stackPadding0;

    somePlayerIndex = arg0->kind - 0xB1;
    temp_a1 = driver2pselset[g_CharacterSelections[arg0->kind - 0xB1]];
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = vs_face_start_position2.x;
            arg0->disp_y = vs_face_start_position2.y;
            arg0->pattern = 1;
            break;
        case 1:
            temp_v0_2 = &vs_face_target_position[((g_menuMultiplayerSelection - 2) * 4) + somePlayerIndex];
            DOB_moving1_x(arg0, temp_v0_2->x);
            DOB_moving1_y(arg0, temp_v0_2->y);
            if (arg0->disp_x == temp_v0_2->x) {
                arg0->pattern = 2;
                arg0->work2 = 0;
            }
            break;
        case 2:
            var_v1 = 0;
            switch (g_gameMode) {
                case 2:
                    if (gGPCurrentRaceRankByPlayerId[somePlayerIndex] != 0) {
                        var_v1 = 1;
                    }
                    break;
                case 3:
                    if (somePlayerIndex != win_kart) {
                        var_v1 = 1;
                    }
                    break;
            }
            if (var_v1 == 0) {
                arg0->work2++;
                if (arg0->work2 >= 0x1F) {
                    if (Get_OBJBlock_ptr(0x000000B0)->pattern >= 2) {
                        ChangeSeqData_vs(arg0->sd_number, 0, somePlayerIndex,
                                      GetVAddress(TBSeq_psel_p1[temp_a1]));
                        arg0->pattern = 3;
                        Na_SeMute_Off(somePlayerIndex);
                        NAPlyVoiceStart(somePlayerIndex, (g_CharacterSelections[somePlayerIndex] * 0x10) + 0x29008007);
                    }
                }
            }
            break;
        case 3:
            if (seq_data_block[arg0->sd_number].sequenceIndex >= p1_pat_num[temp_a1]) {
                ChangeSeqData_vs(arg0->sd_number, 0, somePlayerIndex, GetVAddress(TBSeq_psel_p2[temp_a1]));
                arg0->pattern = 4;
            }
            break;
        case 4:
        default:
            break;
    }
}

void DOB_game_ta1(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = 0x0000014A;
            arg0->pattern = 1;
            SkipReset();
            SetObjBlock(MENU_ITEM_TYPE_0BB, 0, 0, MENU_ITEM_PRIORITY_0);
            break;
        case 1:
            DOB_moving1_x(arg0, 0x000000A0);
            if (arg0->disp_x == 0x000000A0) {
                arg0->pattern = 2;
                arg0->work2 = 0;
            }
            break;
        case 2:
            arg0->work2++;
            if (arg0->work2 >= 0x15) {
                arg0->pattern = 3;
            }
            break;
        case 3:
            if (k_skip_flg != 0) {
                SkipReset();
                arg0->pattern = 4;
                arg0->work1 = arg0->disp_x;
                SetObjBlock(MENU_ITEM_TYPE_0BA, 0, 0, MENU_ITEM_PRIORITY_0);
            }
            break;
        case 4:
            arg0->disp_x = arg0->work1;
            if (arg0->work1 < 0x14A) {
                if (k_skip_flg != 0) {
                    arg0->work1 += 0x20;
                } else {
                    arg0->work1 += 0x10;
                }
            } else {
                arg0->kind = 0;
            }
            break;
        default:
            break;
    }
}

void DOB_game_ta2(ObjBlock* arg0) {
    struct_8018EE10_entry* thing;
    s32 var_v1;
    s32 var_a1;
    s32 index;

    switch (arg0->pattern) { /* switch 3; irregular */
        case 0:            /* switch 3 */
            arg0->disp_x = -0x000000A0;
            arg0->pattern = 1;
            for (index = 0; index < ARRAY_COUNT(additional_point_defaults); index++) {
                additional_point[index] = additional_point_defaults[index];
            }
            arg0->work2 = arg0->disp_x;
            break;
            ;
        case 1: /* switch 3 */
            arg0->disp_x = arg0->work2;
            if (k_skip_flg != 0) {
                var_a1 = 0x20;
            } else {
                var_a1 = 0x10;
            }
            if ((arg0->work2 + var_a1) < 0) {
                arg0->work2 += var_a1;
                D_800DC5EC->posx += var_a1;
                D_800DC5F0->posx -= var_a1;
            } else {
                arg0->work2 = 0;
                arg0->disp_x = 0;
                arg0->pattern = ta_result_cursor;
                if ((arg0->pattern == 9) && (lost_ghost == 1)) {
                    arg0->pattern--;
                }
                D_800DC5EC->posx = 0x00F0;
                D_800DC5F0->posx = 0x0050;
            }
            break;
        case 5:  /* switch 3 */
        case 6:  /* switch 3 */
        case 7:  /* switch 3 */
        case 8:  /* switch 3 */
        case 9:  /* switch 3 */
        case 10: /* switch 3 */
            if (FadeChecker() == 0) {
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->pattern >= 6) {
                        arg0->pattern--;
                        if ((lost_ghost == 1) && (arg0->pattern == 9)) {
                            arg0->pattern--;
                        }
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = -1;
                    }
                }
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->pattern < 0xA) {
                        arg0->pattern++;
                        if ((lost_ghost == 1) && (arg0->pattern == 9)) {
                            arg0->pattern++;
                        }
                        if ((arg0->pattern == 0x0000000A) && (ghost_status != 0)) {
                            arg0->pattern -= 2;
                        } else {
                            playSound(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->workf1 < 4.2) {
                                arg0->workf1 += 4.0;
                            }
                            arg0->pattern2 = 1;
                        }
                    }
                }
                if (gControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                    if (arg0->pattern == 0x0000000A) {
                        var_v1 = 0;
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        if (gControllerPak1State != 0) {
                            var_v1 = 0;
                            switch (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                                  (u8*) gExtCode, &gControllerPak1FileNote)) { /* switch 4; irregular */
                                case 5:                                                        /* switch 4 */
                                    break;
                                case 0: /* switch 4 */
                                    LoadGameNoteKart_data();
                                    arg0->pattern = CheckSameMap((g_cupSelect * 4) + g_courseSelect) + 0x11;
                                    var_v1 = 1;
                                    playSound(SOUND_MENU_SELECT);
                                    break;
                                case 2: /* switch 4 */
                                    gControllerPak1State = 0;
                                    break;
                                default: /* switch 4 */
                                    gControllerPak1State = 0;
                                    break;
                            }
                        }
                        if (var_v1 == 0) {
                            if (gControllerPak1State == 0) {
                                switch (initPak()) { /* switch 2 */
                                    case -1:               /* switch 2 */
                                        arg0->pattern = 0x0000000B;
                                        var_v1 = 1;
                                        break;
                                    case -3: /* switch 2 */
                                    case -2: /* switch 2 */
                                        arg0->pattern = 0x0000000C;
                                        var_v1 = 1;
                                        break;
                                    case 1:  /* switch 2 */
                                    case 11: /* switch 2 */
                                        arg0->pattern = 0x0000000B;
                                        var_v1 = 1;
                                        break;
                                    case 10: /* switch 2 */
                                        arg0->pattern = 0x0000000C;
                                        var_v1 = 1;
                                        break;
                                    default: /* switch 2 */
                                        var_v1 = 1;
                                        arg0->pattern = 0x0000000C;
                                        break;
                                    case 0:
                                        break;
                                }
                                if (var_v1 != 0) {
                                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                }
                                if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                                  (u8*) gExtCode, &gControllerPak1FileNote) == 0) {
                                    LoadGameNoteKart_data();
                                    arg0->pattern = CheckSameMap((g_cupSelect * 4) + g_courseSelect) + 0x11;
                                    playSound(SOUND_MENU_SELECT);
                                    return;
                                }
                            }
                            if (gControllerPak1MaxWriteableFiles >= gControllerPak1NumFilesUsed) {
                                arg0->pattern = 0x0000000E;
                                playSound(SOUND_MENU_FILE_NOT_FOUND);
                                return;
                            }
                            if (gControllerPak1NumPagesFree >= 0x79) {
                                arg0->pattern = 0x00000013;
                                arg0->work1 = 0;
                                playSound(SOUND_MENU_SELECT);
                                return;
                            }
                            arg0->pattern = 0x0000000E;
                            playSound(SOUND_MENU_FILE_NOT_FOUND);
                        }
                    } else {
                        arg0->work1 = arg0->pattern;
                        ta_result_cursor = arg0->pattern;
                        arg0->pattern = 0x0000001E;
                        arg0->work2 = arg0->disp_y;
                        playSound(SOUND_ACTION_NEXT_COURSE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                    }
                }
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 26:
            if (gControllerOne->ButtonPressed & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                arg0->pattern = 0x0000000A;
                playSound(SOUND_MENU_GO_BACK);
                if (arg0->workf1 < 4.2) {
                    arg0->workf1 += 4.0;
                }
            }
            break;
        case 17:
        case 18:
            arg0->work2 = arg0->pattern - 0x11;
            if (CheckSameMap2((g_cupSelect * 4) + g_courseSelect) != arg0->work2) {
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->pattern >= 0x12) {
                        arg0->pattern--;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = -1;
                    }
                }
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->pattern < 0x12) {
                        arg0->pattern++;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = 1;
                    }
                }
            }
            if (gControllerOne->ButtonPressed & B_BUTTON) {
                arg0->pattern = 0x0000000A;
                playSound(SOUND_MENU_GO_BACK);
                return;
            }
            if (gControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                thing = &D_8018EE10[arg0->work2];
                if (thing->ghostDataSaved == 0) {
                    arg0->pattern = 0x00000019;
                    arg0->work1 = 0;
                } else if (CheckSameGhost(arg0->work2) == 0) {
                    arg0->pattern = 0x00000010;
                } else {
                    arg0->pattern = 0x00000014;
                }
                playSound(SOUND_MENU_SELECT);
                if (arg0->workf1 < 4.2) {
                    arg0->workf1 += 4.0;
                }
            }
            break;
        case 19:
            if ((arg0->work1 == 1) && (makeGameNoteKartOnly() != 0)) {
                arg0->pattern = 0x0000000F;
                return;
            } else {
                arg0->work1++;
                if (arg0->work1 >= 2) {
                    arg0->pattern = 0x00000011;
                }
            }
            break;
        case 20:
        case 21:
            if (((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & U_JPAD) && (arg0->pattern >= 0x15)) {
                arg0->pattern--;
                playSound(SOUND_MENU_CURSOR_MOVE);
                if (arg0->workf1 < 4.2) {
                    arg0->workf1 += 4.0;
                }
                arg0->pattern2 = -1;
            }
            if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & D_JPAD) {
                if (arg0->pattern < 0x15) {
                    arg0->pattern++;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->workf1 < 4.2) {
                        arg0->workf1 += 4.0;
                    }
                    arg0->pattern2 = 1;
                }
            }
            if (gControllerOne->ButtonPressed & B_BUTTON) {
                arg0->pattern = arg0->work2 + 0x11;
                playSound(SOUND_MENU_GO_BACK);
                return;
            }
            if (gControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                if (arg0->pattern == 0x00000015) {
                    arg0->pattern = 0x00000019;
                    arg0->work1 = 0;
                    playSound(SOUND_MENU_SELECT);
                    if (arg0->workf1 < 4.2) {
                        arg0->workf1 += 4.0;
                    }
                } else {
                    arg0->pattern = arg0->work2 + 0x11;
                    playSound(SOUND_MENU_GO_BACK);
                    return;
                }
            }
            break;
        case 25:
            if (arg0->work1 == 1) {
                if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                                  &gControllerPak1FileNote) != 0) {
                    arg0->pattern = 0x0000001A;
                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                if (SaveGameNoteKart_ghost(arg0->work2) != 0) {
                    arg0->pattern = 0x0000001A;
                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
            }
            arg0->work1++;
            if (arg0->work1 >= 2) {
                arg0->pattern = 0x0000000A;
                return;
            }
            break;
        case 30:
            arg0->disp_y = arg0->work2;
            if (arg0->work2 < 0xF0) {
                arg0->work2 += 0x10;
                D_800DC5EC->posy += 0x10;
                D_800DC5F0->posy -= 0x10;
                return;
            }
            switch (arg0->work1) { /* switch 3 */
                case 5:             /* switch 3 */
                    replay_flag = 0;
                    retry_flag = 1;
                    goto_racing();
                    break;
                case 6: /* switch 3 */
                    goto_mapselect();
                    break;
                case 7: /* switch 3 */
                    GoToGameSelect();
                    break;
                case 8: /* switch 3 */
                    goto_gameselect();
                    break;
                case 9: /* switch 3 */
                    replay_flag = 1;
                    retry_flag = 0;
                    goto_racing();
                    break;
            }
            arg0->work2 = 0;
            arg0->pattern = 0x0000001F;
            D_800DC5EC->posy = 0x012C;
            D_800DC5F0->posy = -0x003C;
            finish_zoom_flag = 4;
            NaSeqFadeout(0x19U);
            break;
        case 31:
            arg0->kind = 0;
            break;
    }
}
#ifdef VERSION_EU
#define FUNC_800ADF48DEF 70
#else
#define FUNC_800ADF48DEF 60
#endif
void DOB_game_pause(ObjBlock* arg0) {
    UNUSED s32 stackPadding;
    struct Controller* controller;

    if (pause_flag != 0) {
        switch (arg0->pattern) {
            case 0:
                arg0->pattern = start_pause_seq[g_gameMode];
                break;
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 21:
            case 22:
            case 23:
            case 24:
            case 31:
            case 32:
            case 41:
            case 42:
            case 43:
            case 44:
                if (FadeChecker() == 0) {
                    controller = &gControllers[pause_flag - 1];
                    if ((controller->ButtonPressed | controller->AnalogPressed) & U_JPAD) {
                        if (start_pause_seq[g_gameMode] < arg0->pattern) {
                            arg0->pattern--;
                            playSound(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->workf1 < 4.2) {
                                arg0->workf1 += 4.0;
                            }
                            arg0->pattern2 = -1;
                        }
                    }
                    if ((controller->ButtonPressed | controller->AnalogPressed) & D_JPAD) {
                        if (arg0->pattern < bottom_pause_seq[g_gameMode]) {
                            arg0->pattern++;
                            playSound(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->workf1 < 4.2) {
                                arg0->workf1 += 4.0;
                            }
                            arg0->pattern2 = 1;
                        }
                    }
                    if (controller->ButtonPressed & B_BUTTON) {
                        if (arg0->pattern != start_pause_seq[g_gameMode]) {
                            arg0->pattern = start_pause_seq[g_gameMode];
                            playSound(SOUND_MENU_GO_BACK);
                            return;
                        }
                    }
                    if (controller->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                        if (arg0->pattern == start_pause_seq[g_gameMode]) {
                            arg0->pattern = 0;
                            pause_flag = 0;
                            LoadKeyStatus();
                            Na_PauseSet(0U);
                        } else {
                            SetFadeOut(30);
                            playSound(SOUND_ACTION_CONTINUE_UNKNOWN);
                            NaSeqFadeout(FUNC_800ADF48DEF);
                            if (arg0->workf1 < 4.2) {
                                arg0->workf1 += 4.0;
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    } else {
        arg0->pattern = 0;
    }
}

void DOB_game_ta_replay(ObjBlock* arg0) {
    struct_8018EE10_entry* thing;
    s32 var_v1;

    if (arg0->pattern != 0) {
        two_d_flag = 0;
    }
    switch (arg0->pattern) {
        case 0:
            if (arg0->work1 < 0x1E) {
                arg0->work1++;
            }
            if (gControllerOne->ButtonPressed & START_BUTTON) {
                arg0->pattern = 0x0000000F;
                playSound(SOUND_ACTION_GO_BACK_2);
            } else if (playerHUD[PLAYER_ONE].goalsw != 0) {
                arg0->pattern = 1;
                arg0->work1 = 0;
            }
            break;
        default:
            break;
        case 1:
            arg0->work1 += 3;
            if (arg0->work1 >= 0x8D) {
                arg0->pattern = 0x0000000F;
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            if (FadeChecker() == 0) {
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->pattern >= 0xC) {
                        arg0->pattern--;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = -1;
                    }
                }
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->pattern < 0x10) {
                        arg0->pattern++;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = 1;
                    }
                }
                if (gControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                    if (arg0->pattern == 0x00000010) {
                        var_v1 = 0;
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        if (gControllerPak1State != 0) {
                            var_v1 = 0;
                            switch (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                                  (u8*) gExtCode, &gControllerPak1FileNote)) { /* switch 3; irregular */
                                case PFS_ERR_INVALID:                                          /* switch 3 */
                                    break;
                                case PFS_NO_ERROR: /* switch 3 */
                                    LoadGameNoteKart_data();
                                    arg0->pattern = CheckSameMap((g_cupSelect * 4) + g_courseSelect) + 0x1E;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_NEW_PACK: /* switch 3 */
                                    gControllerPak1State = 0;
                                    break;
                                default: /* switch 3 */
                                    gControllerPak1State = 0;
                                    break;
                            }
                        }
                        if (var_v1 != 0) {
                            playSound(SOUND_MENU_SELECT);
                            return;
                        }
                        if (gControllerPak1State == 0) {
                            switch (initPak()) { /* switch 2 */
                                case PFS_INVALID_DATA: /* switch 2 */
                                    arg0->pattern = 0x00000015;
                                    var_v1 = 1;
                                    break;
                                case PFS_FREE_BLOCKS_ERROR: /* switch 2 */
                                case PFS_NUM_FILES_ERROR:   /* switch 2 */
                                    arg0->pattern = 0x00000016;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_NOPACK: /* switch 2 */
                                case PFS_ERR_DEVICE: /* switch 2 */
                                    arg0->pattern = 0x00000015;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_ID_FATAL: /* switch 2 */
                                    arg0->pattern = 0x00000016;
                                    var_v1 = 1;
                                    break;
                                default: /* switch 2 */
                                    var_v1 = 1;
                                    arg0->pattern = 0x00000016;
                                    break;
                                case 0:
                                    break;
                            }
                            if (var_v1 != 0) {
                                playSound(SOUND_MENU_FILE_NOT_FOUND);
                                return;
                            }
                            if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName,
                                              (u8*) gExtCode, &gControllerPak1FileNote) == 0) {
                                LoadGameNoteKart_data();
                                arg0->pattern = CheckSameMap((g_cupSelect * 4) + g_courseSelect) + 0x1E;
                                playSound(SOUND_MENU_SELECT);
                                return;
                            }
                        }
                        if (gControllerPak1MaxWriteableFiles >= gControllerPak1NumFilesUsed) {
                            arg0->pattern = 0x00000018;
                            playSound(SOUND_MENU_FILE_NOT_FOUND);
                        } else if (gControllerPak1NumPagesFree >= 0x79) {
                            arg0->pattern = 0x00000020;
                            arg0->work1 = 0;
                            playSound(SOUND_MENU_SELECT);
                        } else {
                            arg0->pattern = 0x00000018;
                            playSound(SOUND_MENU_FILE_NOT_FOUND);
                        }
                    } else {
                        SetFadeOut(0x0000001E);
                        playSound(SOUND_MENU_OK_CLICKED);
                        NaSeqFadeout(0x19U);
                        Na_SE_Fadeout(0x19U);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                    }
                }
            }
            break;
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 41:
            if (gControllerOne->ButtonPressed & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                arg0->pattern = 0x00000010;
                playSound(SOUND_MENU_GO_BACK);
                if (arg0->workf1 < 4.2) {
                    arg0->workf1 += 4.0;
                }
            }
            break;
        case 30:
        case 31:
            arg0->work2 = (u32) arg0->pattern - 0x1E;
            if (CheckSameMap2((g_cupSelect * 4) + g_courseSelect) != arg0->work2) {
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->pattern >= 0x1F) {
                        arg0->pattern--;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = -1;
                    }
                }
                if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->pattern < 0x1F) {
                        arg0->pattern++;
                        playSound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->workf1 < 4.2) {
                            arg0->workf1 += 4.0;
                        }
                        arg0->pattern2 = 1;
                    }
                }
            }
            if (gControllerOne->ButtonPressed & B_BUTTON) {
                arg0->pattern = 0x00000010;
                playSound(SOUND_MENU_GO_BACK);
            } else if (gControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                thing = &D_8018EE10[arg0->work2];
                if (thing->ghostDataSaved == 0) {
                    arg0->pattern = 0x00000028;
                    arg0->work1 = 0;
                } else if (CheckSameGhost(arg0->work2) == 0) {
                    arg0->pattern = 0x0000001A;
                } else {
                    arg0->pattern = 0x00000023;
                }
                playSound(SOUND_MENU_SELECT);
                if (arg0->workf1 < 4.2) {
                    arg0->workf1 += 4.0;
                }
            }
            break;
        case 32:
            if ((arg0->work1 == 1) && (makeGameNoteKartOnly() != 0)) {
                arg0->pattern = 0x00000019;
            } else {
                arg0->work1++;
                if (arg0->work1 >= 2) {
                    arg0->pattern = 0x0000001E;
                }
            }
            break;
        case 35:
        case 36:
            if (((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & U_JPAD) &&
                ((s32) (u32) arg0->pattern >= 0x24)) {
                arg0->pattern--;
                playSound(SOUND_MENU_CURSOR_MOVE);
                if (arg0->workf1 < 4.2) {
                    arg0->workf1 += 4.0;
                }
                arg0->pattern2 = -1;
            }
            if ((gControllerOne->ButtonPressed | gControllerOne->AnalogPressed) & D_JPAD) {
                if (arg0->pattern < 0x24) {
                    arg0->pattern++;
                    playSound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->workf1 < 4.2) {
                        arg0->workf1 += 4.0;
                    }
                    arg0->pattern2 = 1;
                }
            }
            if (gControllerOne->ButtonPressed & B_BUTTON) {
                arg0->pattern = arg0->work2 + 0x1E;
                playSound(SOUND_MENU_GO_BACK);
            } else if (gControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                if (arg0->pattern == 0x00000024) {
                    arg0->pattern = 0x00000028;
                    arg0->work1 = 0;
                    playSound(SOUND_MENU_SELECT);
                    if (arg0->workf1 < 4.2) {
                        arg0->workf1 += 4.0;
                    }
                } else {
                    arg0->pattern = arg0->work2 + 0x1E;
                    playSound(SOUND_MENU_GO_BACK);
                }
            }
            break;
        case 40:
            if (arg0->work1 == 1) {
                if (osPfsFindFile(&gControllerPak1FileHandle, gCompanyCode, gGameCode, (u8*) gGameName, (u8*) gExtCode,
                                  &gControllerPak1FileNote) != 0) {
                    arg0->pattern = 0x00000029;
                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                if (SaveGameNoteKart_ghost(arg0->work2) != 0) {
                    arg0->pattern = 0x00000029;
                    playSound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
            }
            arg0->work1++;
            if (arg0->work1 >= 2) {
                arg0->pattern = 0x00000010;
            }
            break;
    }
}

void DOB_game_ta_mes1(ObjBlock* arg0) {
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = (GetWordLength(ta_romghost_message_j) / 2) + 0x140;
            arg0->disp_y = 0x000000DA;
            arg0->pattern = 1;
            NAPlyVoiceStart(0U, (romghost_kart * 0x10) + 0x29008001);
            break;
        case 1:
            DOB_moving1_x(arg0, 0x000000A0);
            if (arg0->disp_x == 0x000000A0) {
                arg0->pattern = 2;
                arg0->work1 = 0;
            }
            break;
        case 2:
            arg0->work1++;
            if (arg0->work1 >= 0x3D) {
                arg0->pattern = 3;
                arg0->work1 = 0;
            }
            break;
        case 4:
            arg0->work1++;
            if (arg0->work1 >= 6) {
                arg0->kind = 0;
                break;
            }
            // Purposeful fallthrough
        case 3:
            DOB_moving4_x(arg0, 0x000000A0, -1);
            if (((arg0->disp_x + 0x14) == -(GetWordLength(ta_romghost_message_j) / 2)) && (arg0->pattern == 3)) {
                arg0->pattern = 4;
            }
            break;
        default:
            break;
    }
}

void DOB_dsel_snap(ObjBlock* arg0) {
    if (arg0->work1 != gTimeTrialDataCourseIndex) {
        arg0->work1 = (s32) gTimeTrialDataCourseIndex;
        ChangeSeqData(arg0->sd_number, 0,
                      GetVAddress(
                          D_800E7E34[gCupCourseOrder[gTimeTrialDataCourseIndex / 4][gTimeTrialDataCourseIndex % 4]]));
        if (CheckPakStatus() == PFS_NO_ERROR) {
            LoadGameNoteKart_data();
        } else {
            D_8018EE10[0].ghostDataSaved = 0;
            D_8018EE10[1].ghostDataSaved = 0;
        }
    }
}

void DOB_dsel_sel1(ObjBlock* arg0) {
    if (menuScreenA != SUB_MENU_DATA_OPTIONS) {
        arg0->disp_on = 0;
    } else {
        arg0->disp_on = 1;
    }
}

void DOB_dsel_sel2(ObjBlock* arg0) {
    if (menuScreenA != SUB_MENU_DATA_ERASE_CONFIRM) {
        arg0->disp_on = 0;
    } else {
        arg0->disp_on = 1;
    }
}

void DOB_dsel_sel3(ObjBlock* arg0) {
    if (menuScreenA != SUB_MENU_DATA_CANT_ERASE) {
        arg0->disp_on = 0;
    } else {
        arg0->disp_on = 1;
    }
}

void DOB_ta_fanfare(ObjBlock* arg0) {
    if (playerHUD[PLAYER_ONE].goalsw != 0) {
        if ((u32) playerHUD[PLAYER_ONE].totaltime < (u32) (GetRecordTime2(4) & 0xFFFFF)) {
            rankin_fanfare = 1;
        }
        arg0->kind = 0;
    }
}

void DOB_ta_lostghost(ObjBlock* arg0) {
    switch (arg0->pattern) { /* irregular */
        case 0:
            if (lost_ghost == 1) {
                arg0->pattern = 1;
                arg0->work1 = 0;
            } else if (playerHUD[PLAYER_ONE].goalsw == (s8) 1) {
                arg0->pattern = 2;
            }
            break;
        case 2:
            break;
        case 1:
            arg0->work1 += 1;
            if (playerHUD[PLAYER_ONE].goalsw == 1) {
                arg0->pattern = 2;
            }
            break;
    }
}

void DOB_ending1_halffade(ObjBlock* arg0) {
    UNUSED s32 temp_t1;

    switch (arg0->pattern) {
        case 0:
            arg0->work1 += 3;
            if (arg0->work1 >= 0x65) {
                arg0->work1 = 0;
                arg0->pattern = 1;
                g_cupSelect %= 4;
                g_raceClass %= 4;
                SetObjBlock(MENU_ITEM_TYPE_12C, 0, 0, MENU_ITEM_PRIORITY_4);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            arg0->work1 += 1;
            if (arg0->work1 >= 9) {
                arg0->work1 = 0;
                arg0->pattern++;
                SetObjBlock(arg0->pattern + MENU_ITEM_TYPE_12B, 0, 0, MENU_ITEM_PRIORITY_4);
            }
            break;
        case 5:
            arg0->work1 += 1;
            if ((arg0->work1 >= 0x65) &&
                ((gControllerFive->ButtonPressed != 0) || (gControllerFive->AnalogPressed != 0))) {
                arg0->pattern = 6;
                arg0->work1 = 0;
                if (resultstat.rank < 3) {
                    playSound(SOUND_MENU_OK_CLICKED);
                } else {
                    playSound(SOUND_ACTION_CONTINUE_UNKNOWN);
                }
            }
            break;
        case 6:
            SetFadeOut(0x0000001E);
            NaSeqFadeout(0x19U);
            Na_SE_Fadeout(0x19U);
            arg0->pattern = 7;
            break;
        case 7:
        default:
            break;
    }
}

void DOB_ending1_message(ObjBlock* arg0) {
    POSXY* temp_v0_2;
    s32 idx = arg0->kind - 0x12C;

    switch (arg0->pattern) { /* irregular */
        case 0:
            temp_v0_2 = &ending1_messages_start_position[idx];
            arg0->disp_x = temp_v0_2->x;
            arg0->disp_y = temp_v0_2->y;
            arg0->pattern = 1;
            break;
        case 1:
            temp_v0_2 = &ending1_messages_target_position[idx];
            DOB_moving1(arg0, temp_v0_2->x, temp_v0_2->y);
            if ((arg0->disp_x == temp_v0_2->x) && (arg0->disp_y == temp_v0_2->y)) {
                arg0->pattern = 2;
            }
            break;
        case 2:
            break;
    }
}

void DOB_ending1_face(ObjBlock* arg0) {
    s32 temp_v1;
    s32 sp30;
    s32 temp_v0;
    POSXY* thing;

    temp_v1 = arg0->kind - 0x12C;
    sp30 = resultstat.kart;
    temp_v0 = driver2pselset[sp30];
    switch (arg0->pattern) {
        case 0:
            thing = &ending1_messages_start_position[temp_v1];
            arg0->disp_x = thing->x;
            arg0->disp_y = thing->y;
            arg0->pattern = 1;
            break;
        case 1:
            thing = &ending1_messages_target_position[temp_v1];
            DOB_moving1(arg0, thing->x, thing->y);
            if ((arg0->disp_x == thing->x) && (arg0->disp_y == thing->y)) {
                arg0->pattern = 2;
                arg0->work2 = 0;
            }
            break;
        case 2:
            arg0->work2++;
            if (arg0->work2 >= 0x1F) {
                if (resultstat.rank >= 3) {
                    arg0->pattern = 4;
                    Na_SeMute_Title_Off();
                    NAPlyVoiceStart(0U, (sp30 * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03));
                    Na_SeMute_Title();
                } else {
                    arg0->pattern = 3;
                    ChangeSeqData_vs(arg0->sd_number, 0, sp30,
                                  GetVAddress(TBSeq_psel_p1[temp_v0]));
                    Na_SeMute_Title_Off();
                    NAPlyVoiceStart(0U, (sp30 * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    Na_SeMute_Title();
                }
            }
            break;
        case 3:
            if (seq_data_block[arg0->sd_number].sequenceIndex >= p1_pat_num[temp_v0]) {
                ChangeSeqData_vs(arg0->sd_number, 0, sp30, GetVAddress(TBSeq_psel_p2[temp_v0]));
                arg0->pattern = 4;
            }
            break;
        case 4:
            break;
    }
}

void DOB_staffroll_messages(ObjBlock* arg0) {
    s32 idx = arg0->kind - 0x190;

    if ((gCreditsTextRenderInfo[idx].slideDirection == 0) || (gCreditsTextRenderInfo[idx].slideDirection != 1)) {
        DOB_staffroll_type0(arg0);
    } else {
        DOB_staffroll_type1(arg0);
    }
}

void DOB_staffroll_type0(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 temp_v0;
    CreditsRenderInfo* temp_v1;

    temp_v0 = arg0->kind - 0x190;
    temp_v1 = &gCreditsTextRenderInfo[temp_v0];
    arg0->disp_y = temp_v1->row;
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = temp_v1->startingColumn;
            arg0->pattern = 1;
            arg0->work2 = temp_v1->columnExtra + (GetWordLength(gCreditsText[temp_v0]) * temp_v1->textScaling / 2);
            /* fallthrough */
        case 1:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (s32) (arg0->work2 - arg0->disp_x) / 4;
            if (arg0->work1 >= 9) {
                arg0->work1 = 8;
            }
            arg0->workf1 = (arg0->work1 * 0.05) + 1.0;
            if (arg0->disp_x >= (arg0->work2 - 0x14)) {
                arg0->pattern = 2;
                arg0->sd_number = 0;
            }
            break;
        case 2:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (arg0->work2 - arg0->disp_x) / 4;
            arg0->sd_number += 1;
            arg0->workf1 = ((arg0->sd_number - 0xA) * 0.0085 * (arg0->sd_number - 0xA)) + 0.4;
            if ((arg0->sd_number >= 9) && ((f64) arg0->workf1 > 1)) {
                arg0->workf1 = 1.0f;
                arg0->pattern = 3;
            }
            break;
        case 3:
            if ((u8) ending_message_flg != 0) {
                arg0->pattern = 4;
            }
            break;
        case 4:
            DOB_moving4_x(arg0, arg0->work2, 1);
            if (arg0->disp_y > 480.0) {
                arg0->kind = 0;
            }
            break;
        default:
            break;
    }
}

void DOB_staffroll_type1(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 temp_v0;
    CreditsRenderInfo* temp_v1;

    temp_v0 = arg0->kind - 0x190;
    temp_v1 = &gCreditsTextRenderInfo[temp_v0];
    arg0->disp_y = temp_v1->row;
    switch (arg0->pattern) {
        case 0:
            arg0->disp_x = temp_v1->startingColumn;
            arg0->pattern = 1;
            arg0->work2 = temp_v1->columnExtra - (GetWordLength(gCreditsText[temp_v0]) * temp_v1->textScaling / 2);
            /* fallthrough */
        case 1:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (s32) (arg0->disp_x - arg0->work2) / 4;
            if (arg0->work1 >= 9) {
                arg0->work1 = 8;
            }
            arg0->workf1 = (arg0->work1 * 0.05) + 1.0;
            if ((arg0->work2 + 0x14) >= arg0->disp_x) {
                arg0->pattern = 2;
                arg0->sd_number = 0;
            }
            break;
        case 2:
            DOB_moving1_x(arg0, arg0->work2);
            arg0->work1 = (arg0->disp_x - arg0->work2) / 4;
            arg0->sd_number += 1;
            arg0->workf1 = ((arg0->sd_number - 0xA) * 0.0085 * (arg0->sd_number - 0xA)) + 0.4;
            if ((arg0->sd_number >= 9) && ((f64) arg0->workf1 > 1)) {
                arg0->workf1 = 1.0f;
                arg0->pattern = 3;
            }
            break;
        case 3:
            if ((u8) ending_message_flg != 0) {
                arg0->pattern = 4;
            }
            break;
        case 4:
            DOB_moving4_x(arg0, arg0->work2, -1);
            if (arg0->disp_y > 480.0) {
                arg0->kind = 0;
            }
            break;
        default:
            break;
    }
}
