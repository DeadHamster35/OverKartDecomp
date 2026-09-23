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
//! @todo Move DynamicP out of Main.h
// Unfortunately that's not a small effort due to weird import structure in this project
#include "main.h"
#include "OverKartHooks.h"

void guMtxCatL(Mtx* m, Mtx* n, Mtx* res);

u16* TexBuff;
u32* MeltBuff;
u8* KWork;
u8* AdditionalPoint;
void* KdlBuff;
/**
 * List of bytes indexed by character ID
 * Indicates Number of Grand Prix points that character
 * has scored
 */
s8 PlayerGPpoints[8];
s8 DPointsRank[8];
s8 KSkipFlg;
s8 KSkipStatus;
ObjBlock MenuItems[MENU_ITEMS_MAX];
struct_8018DEE0_entry SEQDataBlock[D_8018DEE0_SIZE];
struct_8018E060_entry ChangeTexBuffBlock[D_8018E060_SIZE];
UNUSED u8 MenuItemBssPadding0[8];
struct_8018E0E8_entry ChangeTexBuffBlockVS[D_8018E0E8_SIZE];
s32 TexBuffNum;
TextureMap SMenuTextureMap[TEXTURE_MAP_MAX];
s32 AllTexBlockNum;
Gfx* KdlBuffPtr;
s32 AllKdlBlockNum;
struct_8018E768_entry KdlBuffBlock[D_8018E768_SIZE];
s32 KFlashCounter;
s8 FadingFlag[5];
u32 FadeCounter[5];
u32 FadeCounter2[4];
s32 D_8018E7E0;
struct UnkStruct_8018E7E8 OldScreenPosition[D_8018E7E8_SIZE];
struct UnkStruct_8018E7E8 OldScreenSize[D_8018E810_SIZE];
s8 ThunderFlg[4]; // Import to C was required for matching.
s32 D_8018E83C;

s32 ThunderCounter[4]; // This may all be one big array.
s32 VcColorNum[2]; // This is probably incorrect. Fix after decomping Code.
s32 VcColorStep[2];
s8 AfontColor;
s32 D8018E864Pad;
OSPfs ControllerPak1FileHandle;
OSPfs ControllerPak2FileHandle;
OSPfsState PfsState[16];
s32 PfsError[16]; // 0 = Ok, anything else = error.
s32 ControllerPak1NumFilesUsed;
s32 ControllerPak1MaxWriteableFiles;

s32 ControllerPak1NumPagesFree;
s32 ControllerPak1FileNote;
s32 ControllerPak2FileNote;
s32 MenuItemBssPad2;
ALIGNED8 SaveData EEPRomSaveData;

u8 RankinFanfare;
u8 EndingMessageFlg;
s32 LogoTimer;

POSXY LgselDefaultPositions[] = {
    { 0x3d, 0x11, 0x00, 0x00 }, { 0x15, 0x3e, 0x00, 0x00 }, { 0x5c, 0x3e, 0x00, 0x00 },
    { 0xa3, 0x3e, 0x00, 0x00 }, { 0xea, 0x3e, 0x00, 0x00 }, { 0x10a, 0xc8, 0x00, 0x00 },
    { 0x15, 0xc8, 0x00, 0x00 }, { 0x55, 0xc8, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },
};

POSXY GselPopupPositions[] = {
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

POSXY MselCupboxPosition[] = {
    { 0x17, 0x3b, 0x00, 0x00 },
    { 0x5d, 0x3b, 0x00, 0x00 },
    { 0xa2, 0x3b, 0x00, 0x00 },
    { 0xe8, 0x3b, 0x00, 0x00 },
};

POSXY MselMap4BoxPosition[] = {
    { 0x17, 0x70, 0x00, 0x00 },
    { 0x57, 0x70, 0x00, 0x00 },
    { 0x17, 0x97, 0x00, 0x00 },
    { 0x57, 0x97, 0x00, 0x00 },
};

// In a perfect world this would be `POSXY LpselCharacterOKPosition[][4]`
POSXY LpselCharacterOKPosition[] = {
    { 0x80, 0x58, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x80, 0x3f, 0x00, 0x00 }, { 0x80, 0x91, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x5a, 0x58, 0x00, 0x00 }, { 0xa6, 0x58, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x5a, 0x3f, 0x00, 0x00 }, { 0xa6, 0x3f, 0x00, 0x00 }, { 0x5a, 0x91, 0x00, 0x00 }, { 0xa6, 0x91, 0x00, 0x00 },
};

POSXY LmselCircuitPosition[][2] = {
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

POSXY MselBestboxDefaultPosition[] = {
    { 0xff6a, 0x3b, 0x00, 0x00 },
    { 0x172, 0x3b, 0x00, 0x00 },
};

POSXY MselBestboxTargetPosition[] = {
    { 0x17, 0x3b, 0x00, 0x00 },
    { 0xc5, 0x3b, 0x00, 0x00 },
};

POSXY MselCupPositions[] = {
    { 0x28, 0x73, 0x00, 0x00 },
    { 0x28, 0x3c, 0x00, 0x00 },
};

POSXY CpCursorPosition[] = {
    { 0x3e, 0x43, 0x00, 0x00 },     { 0xa1, 0x43, 0x00, 0x00 },
    { 0x3e, 0xc5, 0x00, 0x00 },     { 0xa1, 0xc5, 0x00, 0x00 },

    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0x140, 0xf0, 0x00, 0x00 },
    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0xffc0, 0xffc0, 0x00, 0x00 },

    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0x140, 0xffc0, 0x00, 0x00 },
    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0xffc0, 0xffc0, 0x00, 0x00 },

    { 0xffc0, 0xffc0, 0x00, 0x00 }, { 0x140, 0xffc0, 0x00, 0x00 },
    { 0xffc0, 0xf0, 0x00, 0x00 },   { 0x140, 0xf0, 0x00, 0x00 },
};

POSXY VSFaceStartPosition2 = { 0x140, 0x23, 0x00, 0x00 };

// In a perfect world this would be `POSXY VSFaceTargetPosition[][4]`
POSXY VSFaceTargetPosition[] = {
    { 0x50, 0x23, 0x00, 0x00 }, { 0xb0, 0x23, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x32, 0x23, 0x00, 0x00 }, { 0x80, 0x23, 0x00, 0x00 }, { 0xce, 0x23, 0x00, 0x00 }, { 0x00, 0x00, 0x00, 0x00 },

    { 0x18, 0x23, 0x00, 0x00 }, { 0x5d, 0x23, 0x00, 0x00 }, { 0xa2, 0x23, 0x00, 0x00 }, { 0xe7, 0x23, 0x00, 0x00 },
};

// Versus menu coordinates
POSXY VSResultCursorPosition[] = {
    { 0x61, 0xa7, 0x00, 0x00 },
    { 0x61, 0xb6, 0x00, 0x00 },
    { 0x61, 0xc5, 0x00, 0x00 },
    { 0x61, 0xd4, 0x00, 0x00 },
};

POSXY VSResultWinlosePosition[] = {
    { 0x30, 0x4b, 0x00, 0x00 },
    { 0x109, 0x4b, 0x00, 0x00 },
};

POSXY TaResultCursorPosition[] = {
    { 0xad, 0x8d, 0x00, 0x00 }, { 0xad, 0x9a, 0x00, 0x00 }, { 0xad, 0xa7, 0x00, 0x00 },
    { 0xad, 0xb4, 0x00, 0x00 }, { 0xad, 0xc1, 0x00, 0x00 }, { 0xad, 0xce, 0x00, 0x00 },
};

POSXY TaGhostbCursorPosition[] = {
    { 0xac, 0xa5, 0x00, 0x00 },
    { 0xac, 0xc3, 0x00, 0x00 },
};

POSXY TaGhostcCursorPosition[] = {
    { 0xc0, 0xb3, 0x00, 0x00 },
    { 0xc0, 0xc2, 0x00, 0x00 },
};

POSXY TarSelectCursorPosition[] = {
    { 0x61, 0x94, 0x00, 0x00 }, { 0x61, 0xa1, 0x00, 0x00 }, { 0x61, 0xae, 0x00, 0x00 },
    { 0x61, 0xbb, 0x00, 0x00 }, { 0x61, 0xc8, 0x00, 0x00 }, { 0x61, 0xd5, 0x00, 0x00 },
};

POSXY TarGhostbCursorPosition[] = {
    { 0x52, 0x90, 0x00, 0x00 },
    { 0x52, 0xa4, 0x00, 0x00 },
};

POSXY TarGhostcCursorPosition[] = {
    { 0x76, 0x95, 0x00, 0x00 },
    { 0x76, 0xa4, 0x00, 0x00 },
};

POSXY TselCupPositions[] = {
    { 0x17, 0xa, 0x00, 0x00 }, { 0x5d, 0xa, 0x00, 0x00 }, { 0xa2, 0xa, 0x00, 0x00 },
    { 0xe8, 0xa, 0x00, 0x00 }, { 0x17, 0xa, 0x00, 0x00 },
};

POSXY Ending1MessagesStartPosition[] = {
    { 0x14a, 0x32, 0x00, 0x00 },  { 0xff60, 0xd4, 0x00, 0x00 }, { 0xa0, 0x10e, 0x00, 0x00 },
    { 0xff60, 0xbe, 0x00, 0x00 }, { 0x143, 0x5a, 0x00, 0x00 },
};

POSXY Ending1MessagesTargetPosition[] = {
    { 0xa0, 0x32, 0x00, 0x00 }, { 0x9b, 0xd4, 0x00, 0x00 }, { 0xa0, 0x50, 0x00, 0x00 },
    { 0x9b, 0xbe, 0x00, 0x00 }, { 0x80, 0x5a, 0x00, 0x00 },
};

RGBA16 PselCursorColor[] = {
    { 0x00, 0xf3, 0xf3, 0xff }, { 0xff, 0xa8, 0xc3, 0xff }, { 0xff, 0xfe, 0x7a, 0xff },
    { 0x7b, 0xfc, 0x7b, 0xff }, { 0xff, 0xff, 0x00, 0xff },
};

RGBA16 TselCursorColor[] = {
    { 0x00, 0xf3, 0xf3, 0xff },
    { 0xff, 0xa8, 0xc3, 0xff },
    { 0xff, 0xff, 0x00, 0xff },
};

RGBA16 TitlebackColor[] = {
    { 0xff, 0xaf, 0xaf, 0xff },
    { 0xaf, 0xff, 0xaf, 0xff },
    { 0xaf, 0xaf, 0xff, 0xff },
};

const s16 FontWidth[] = {
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

char* CupName[] = {
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
char* StringTableCourseGP[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};

char* StringTableCourse[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};
#else

#endif

char* CircuitNameSemilongJ[] = {
#include "assets/course_metadata/gCourseNames.inc.c"
};

#if !ENABLE_CUSTOM_COURSE_ENGINE
// Used in debug menu at splash screen
char* CircuitNameShort[] = {
#include "assets/course_metadata/gCourseDebugNames.inc.c"
};
#else

#endif

const s8 Map2CircuitChart[] = {
#include "assets/course_metadata/gPerCupIndexByCourseId.inc.c"
};

// @todo Increase this array for more than eight players
const s8 Driver2Pselset[] = { 0, 1, 4, 3, 5, 6, 2, 7 };

// Maps course IDs (as defined in the COURSES enum) to the cup they belong to
s8 Map2CupChart[] = {
#include "assets/course_metadata/gCupSelectionByCourseId.inc.c"
};

char* CupTypeNames[] = {
    "none",
    "bronze",
    "silver",
    "gold",
};

char* DriversName[] = {
    "MARIO", "LUIGI", "YOSHI", "KINOPIO", "D.KONG", "WARIO", "PEACH", "KOOPA",
};

char* DriversNameJ[] = {
    "MARIO",    "LUIGI", "YOSHI", "TOAD", "D.K.", "WARIO", "PEACH", "BOWSER",
    "ーーーー", // NOT HYPHENS!!! These are EUC-JP characters (0xa1 0xbc)
};

char* ClassName[] = {
    "50(",
    "100(",
    "150(",
    "extra",
};

char* ClassNameJ[] = {
    "50(",
    "100(",
    "150(",
    "extra",
};

char* ScreenModeNames[] = {
    "1p", "2players UD", "2players LR", "3players", "4players",
};

char* SoundModeName[] = {
    "stereo",
    "head phone",
    "xxx",
    "monaural",
};

char* SoundModeNameJ[NUM_SOUND_MODES] = { "STEREO", "HEADPHONE", "", "MONO" };

char* WinLoseJ[] = {
    "WINNER!",
    "LOSER!",
};

char* BestrecJ[] = {
    "BEST RECORDS",
    "BEST LAP",
};

// Might need a const?
char* LapTimeText = "LAP TIME";

char* LaptimeJ[] = {
    "LAP 1",
    "LAP 2",
    "LAP 3",
    "TOTAL",
};

char* RecordtimeJ[] = {
    // The s/n/r/t here are not ASCII, they are EUC-JP characters
    // 0xae 0xf3/0xae 0xee/0xae 0xf2/0xae 0xf4
    "1 ｓ", "2 ｎ", "3 ｒ", "4 ｔ", "5 ｔ", " ",
};

char* TaResultSelectJ[] = {
    "CONTINUE GAME", "RETRY", "COURSE CHANGE", "DRIVER CHANGE", "QUIT", "REPLAY", "SAVE GHOST",
};

char* VSResultTitleJ[] = {
    "VS MATCH RANKING",
    "BATTLE RANKING",
};

// This is plain data, it should not end up in rodata
char TaRomghostMessageJ[] = "NOW-MEET THE COURSE GHOST!!!";

char* NoControllerMessage[] = { "CONNECT A CONTROLLER TO SOCKET 1,", "THEN POWER ON AGAIN" };

char* MselBattleMessagesJ[] = {
    "BATTLE GAME",
    "POP OPPOSING PLAYER'S BALLOONS",
    "WHEN ALL 3 ARE GONE,THEY ARE OUT!",
};

// This is plain data, it should not end up in rodata
char TselTitleJ[] = "a BUTTON*SEE DATA  B BUTTON*EXIT";

// This is plain data, it should not end up in rodata
char DselLengthJ[] = "distance";

char* DselLengthdataJ[] = {
#include "assets/course_metadata/sCourseLengths.inc.c"
};

char* DselSelect1J[] = {
    "return to menu",
    "erase records for this course",
    "erase ghost from this course",
};

char* DselSelect2J[] = {
    "quit",
    "erase",
};

// Why oh why is this array flat? It should be DselSelect2MessagesJ[][3]
char* DselSelect2MessagesJ[] = {
    "THE BEST RECORDS AND BEST", "LAP FOR THIS COURSE WILL BE", "ERASED.  IS THIS OK?",

    "GHOST DATA FOR THIS",       "COURSE WILL BE ERASED.",      "IS THIS OK?",
};

char* DselSelect3MessagesJ[] = {
    "UNABLE TO ERASE ",
    "GHOST DATA",
};

char* OptionSelect1MessagesJ[] = {
    "RETURN TO GAME SELECT",
    "SOUND MODE",
    "COPY N64 CONTROLLER PAK",
    "ERASE ALL DATA",
};

char* OptionSelect3MessagesJ[] = {
    "ALL SAVED DATA WILL BE",
    "PERMANENTLY ERASED.",
    "ARE YOU REALLY SURE?",
};

char* OptionSelect3EMessagesJ[] = {
    "",
    "ALL SAVED DATA",
    "HAS BEEN NOW ERASED.",
};

// In a perfect world this would be `char *OptionCopyPak1ErrMessagesJ[][4]`
char* OptionCopyPak1ErrMessagesJ[] = {
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

// In a perfect world this would be `char *OptionCopyPak2ErrMessagesJ[][3]`
char* OptionCopyPak2ErrMessagesJ[] = {
    "NO GHOST DATA ",         "IN CONTROLLER 2 ",         "N64 CONTROLLER PAK",

    "NO MARIO KART 64 DATA ", "PRESENT IN CONTROLLER 2 ", "N64 CONTROLLER PAK",

    "CONTROLLER 2 ",          "DOES NOT HAVE ",           "N64 CONTROLLER PAK SET",

    "UNABLE TO READ DATA ",   "FROM CONTROLLER 2 ",       "N64 CONTROLLER PAK",
};

// In a perfect world this would be `char *OptionCopyCopyerrMessagesJ[][4]`
char* OptionCopyCopyerrMessagesJ[] = {
    "UNABLE TO COPY DATA ", "FROM CONTROLLER 1 ", "N64 CONTROLLER PAK",

    "UNABLE TO READ DATA ", "FROM CONTROLLER 2 ", "N64 CONTROLLER PAK",
};

char* OptionCopyPakJ[] = {
    "CONTROLLER 1",
    "CONTROLLER 2",
};

char* OptionCopyPak2SelMessageJ[] = {
    "WHICH FILE DO YOU WANT TO MAKE A COPY OF?",
    "TO WHICH FILE DO YOU WANT TO COPY?",
};

char* OptionCopySureMessagesJ[] = {
    "CURRENT DATA WILL BE ERASED,",
    "IS THIS OK?",
};

char* OptionCopySure2MessagesJ[] = {
    "QUIT",
    "COPY",
};

char* OptionCopyCopyMessagesJ[] = {
    "COPYING",
    "DATA COPY COMPLETED",
};

// In a perfect world this would be `char *GselCautionMessagesJ[][4]`
char* GselCautionMessagesJ[] = {
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

// In a perfect world this would be `char *TaGhostaMessagesJ[][7]`
char* TaGhostaMessagesJ[] = {
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

char* TaLostGhostMessageJ[] = {
    "RACE DATA CANNOT ",
    "BE SAVED FOR GHOST",
};

char* TaGhostbMessagesJ[] = {
    "SELECT THE FILE ",
    "YOU WANT TO SAVE",
};

// Might need a const?
char* D_800E7A44 = "NO DATA";

char* TaGhostb3MessagesJ[] = {
    "CREATING ",
    "MARIO KART 64 ",
    "GAME DATA",
};

char* D_800E7A54[] = {
    "CANNOT CREATE GAME DATA",
    "",
    "",
};

char* TaGhostcMessagesJ[] = {
    "THE PREVIOUS DATA ",
    "WILL BE ERASED, ",
    "IS THIS OK?",
};

char* TaGhostcSelectMessagesJ[] = {
    "QUIT",
    "SAVE",
};

char* TaGhostdMessagesJ[] = {
    "SAVING GHOST DATA",
    "",
    "PLEASE WAIT",
};

char* TaGhostd2MessagesJ[] = {
    "UNABLE TO SAVE ",
    "THE GHOST",
};

char* EndingMessage2[] = {
    "YOU ARE AWARDED THE",
    "GOLD CUP",
    "SILVER CUP",
    "BRONZE CUP",
};

// Might need a const?
char* D_800E7A98 = "MAYBE NEXT TIME!";

char* EndingMessage3[] = {
    "CONGRATULATIONS!",
    "WHAT A PITY!",
};

char* EndingMessage4[] = {
    "YOU PLACED", "    st", "    nd", "    rd", "    th", "    th", "    th", "    th", "    th",
};

const s8 AdditionalPointDefaults[] = { 9, 6, 3, 1 };
const s8 ScreenMode2Num[] = {
    0, 0, 1, 0, 1, 0, 1, 2, 0, 1, 2, 3,
};
const s8 ThunderOnoff[] = {
    0, 1, 2, 1, 2, 1, 2, 1, 2, 0, 0, 1, 2, 2, 1, 2, 2, 1, 2, 2,
    1, 2, 2, 1, 2, 2, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
};

const s8 StartPauseSEQ[] = { 0x1f, 0x0b, 0x15, 0x29 };
const s8 BottomPauseSEQ[] = { 0x20, 0x0f, 0x18, 0x2c };

RGBA16 KThunderColor[] = {
    { 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0xff },
    { 0x00, 0x00, 0x50, 0xff },
    { 0xff, 0xff, 0xff, 0xff },
};

RGBA16 FadeColorBw[] = {
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

MenuTexture* MenuTexturesBackground[] = {
    Seg2BlueSkyBackgroundTexture,
    Seg2SunsetBackgroundTexture,
};

MenuTexture* SelectNamePlateTable[] = {
    D_02001A8C, D_02001A64, D_02001AB4, D_02001A14, D_02001B04, D_020019EC, D_02001ADC, D_02001A3C,
};

MenuTexture* D_800E7D74[] = {
    Seg2MarioRacewayPreviewTexture,
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
    Seg2MarioRacewayTitleTexture,
    Seg2ChocoMountainTitleTexture,
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

MenuTexture* GlyphTextureLUT[] = {
    Seg2TextureFontLetterA,
    Seg2TextureFontLetterB,
    Seg2TextureFontLetterC,
    Seg2TextureFontLetterD,
    Seg2TextureFontLetterE,
    Seg2TextureFontLetterF,
    Seg2TextureFontLetterG,
    Seg2TextureFontLetterH,
    Seg2TextureFontLetterI,
    Seg2TextureFontLetterJ,
    Seg2TextureFontLetterK,
    Seg2TextureFontLetterL,
    Seg2TextureFontLetterM,
    Seg2TextureFontLetterN,
    Seg2TextureFontLetterO,
    Seg2TextureFontLetterP,
    Seg2TextureFontLetterQ,
    Seg2TextureFontLetterR,
    Seg2TextureFontLetterS,
    Seg2TextureFontLetterT,
    Seg2TextureFontLetterU,
    Seg2TextureFontLetterV,
    Seg2TextureFontLetterW,
    Seg2TextureFontLetterX,
    Seg2TextureFontLetterY,
    Seg2TextureFontLetterZ,
    Seg2TextureFontExclamationMark,
    Seg2TextureFontMinus,
    Seg2TextureFontInterogationMark,
    Seg2TextureFontSimpleQuote,
    Seg2TextureFontEmpty,
    Seg2TextureFontDot,
    Seg2TextureFontNumberZero,
    Seg2TextureFontNumberOne,
    Seg2TextureFontNumberTwo,
    Seg2TextureFontNumberThree,
    Seg2TextureFontNumberFour,
    Seg2TextureFontNumberFive,
    Seg2TextureFontNumberSix,
    Seg2TextureFontNumberSeven,
    Seg2TextureFontNumberEight,
    Seg2TextureFontNumberNine,
    Seg2TextureFontDoubleQuote,
    Seg2TextureFontFourDote,
    Seg2TextureFontPlus,
    Seg2TextureFontCC,
    Seg2TextureFontComma,
    Seg2TextureFontEmpty,
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
    Seg2TextureFontComma,
    D_0200455C,
};

// In a perfect world this would be `MenuTexture *D_800E8234[][2]`
MenuTexture* D_800E8234[] = {
    Seg2Menu1PColumn, D_020047DC, Seg2Menu2PColumn, D_02004804,
    Seg2Menu3PColumn, D_0200482C, Seg2Menu4PColumn, D_02004854,
};

MenuTexture* D_800E8254[] = {
    Seg2GameSelectTexture, Seg2Menu1PColumn, Seg2Menu2PColumn, Seg2Menu3PColumn,
    Seg2Menu4PColumn,      D_0200487C,          TextureMenuLOption, Seg2TextureMenuRData,
};

// CC Textures
MenuTexture* D_800E8274[] = {
    Seg250CCTexture,
    Seg2100CCTexture,
    Seg2150CCTexture,
    Seg2ExtraCCTexture,
};

// Versus and battle Textures
MenuTexture* D_800E8284[] = { D_020049BC, Seg2MenuNoItemTexture };

// Time trials begin and data Textures
MenuTexture* D_800E828C[] = {
    D_020049E4,
    Seg2DataTexture,
};

// Duplicates of the above 3 arrays
// CC Textures
MenuTexture* D_800E8294[] = {
    Seg250CCTexture,
    Seg2100CCTexture,
    Seg2150CCTexture,
    Seg2ExtraCCTexture,
};

// Versus and battle Textures
MenuTexture* D_800E82A4[] = { D_020049BC, Seg2MenuNoItemTexture };

// Time trials begin and data Textures
MenuTexture* D_800E82AC[] = {
    D_020049E4,
    Seg2DataTexture,
};

MenuTexture* MenuTexturesBorderPlayer[] = {
    Seg2P1BorderTexture,
    Seg2P2BorderTexture,
    Seg2P3BorderTexture,
    Seg2P4BorderTexture,
};

MenuTexture* MenuTexturesTrackSelection[] = {
    Seg2MenuSelectTexture,        Seg2MushroomCupTexture,     Seg2FlowerCupTexture,
    Seg2StarCupTexture,           Seg2SpecialCupTexture,      Seg2MarioRacewayPreviewSmallTexture,
    Seg2MushroomCupTitleTexture, Seg2FlowerCupTitleTexture, Seg2StarCupTitleTexture,
    Seg2SpecialCupTitleTexture,  Seg2BattleTitleTexture,     D_02004E80,
};

MenuTexture* D_800E82F4[] = {
    Seg2MushroomCupTexture,
    Seg2FlowerCupTexture,
    Seg2StarCupTexture,
    Seg2SpecialCupTexture,
    Seg2MarioRacewayPreviewSmallTexture,
    Seg2MushroomCupTitleTexture,
    Seg2FlowerCupTitleTexture,
    Seg2StarCupTitleTexture,
    Seg2SpecialCupTitleTexture,
    Seg2BattleTitleTexture,
    D_02004E80,
};

MkAnimation* D_800E8320[] = {
    D_02006708, D_020068F0, D_02006CC0, D_02006EA8, D_02007090, D_02007280, D_02007468, D_02006AD8,
};

MkAnimation* TBSeqPselReturn[] = {
    D_02006778, D_02006960, D_02006D30, D_02006F18, D_02007100, D_020072F0, D_020074D8, D_02006B48,
};

MkAnimation* TBSeqPselP0[] = {
    D_02006708, D_020068F0, D_02006CC0, D_02006EA8, D_02007090, D_02007280, D_02007468, D_02006AD8,
};

MkAnimation* TBSeqPselP1[] = {
    D_02006718, D_02006900, D_02006CD0, D_02006EB8, D_020070A0, D_02007290, D_02007478, D_02006AE8,
};

MkAnimation* TBSeqPselP2[] = {
    D_02006778, D_02006960, D_02006D30, D_02006F18, D_02007100, D_020072F0, D_020074D8, D_02006B48,
};

MkAnimation* TBSeqPselP3[] = {
    D_02006788, D_02006970, D_02006D40, D_02006F28, D_02007110, D_02007300, D_020074E8, D_02006B58,
};

MkAnimation* TBSeqPselP4[] = {
    D_020067E0, D_020069C8, D_02006D98, D_02006F80, D_02007170, D_02007358, D_02007540, D_02006BB0,
};

MkAnimation* TBSeqPselP5[] = {
    D_02006838, D_02006A20, D_02006DF0, D_02006FD8, D_020071C8, D_020073B0, D_02007598, D_02006C08,
};

MkAnimation* TBSeqPselP6[] = {
    D_020068E0, D_02006AC8, D_02006E98, D_02007080, D_02007270, D_02007458, D_02007640, D_02006CB0,
};

s32 P1PatNum[] = {
    0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
};

s32 P3PatNum[] = {
    0x09, 0x09, 0x09, 0x09, 0x0a, 0x09, 0x09, 0x09,
};

s32 P4PatNum[] = {
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
};

s32 P5PatNum[] = {
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

s8 FirstTimeOnlyFlg = 1;

// Speed of the "motion" effect intro logo Model
f32 LogoDelaySize = 0.0f;

// Speed of the intro logo Model
f32 LogoYangSpeed = 3.0f;

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
 * This function appears to multiply some `Value`
 * by 2 ^ `exponent`, even if that exponent is negative
 **/
f64 Lldexp(f64 Value, s32 exponent) {
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
                Value *= base;
            }
            exponent >>= 1;
            base *= base;
        } while (exponent != 0);
    }
    return Value;
}

/**
 * Its unclear what exact purpose this function serves
 * The function multiplies/divides arg0 by 2 until its in
 * the range (0.5, 1.0].
 * Arg2 appears to track the exponent in the Power-of-2
 * that would undo the Changes to arg0
 **/
f64 Ffrexp(f64 arg0, s32* Arg2) {
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
    *Arg2 = exponent;
    return arg0;
}

void SwapUint(s32* arg0, s32* arg1) {
    s32 temp;

    temp = *arg0;
    *arg0 = *arg1;
    *arg1 = temp;
}

extern s8 FirstTimeOnlyFlg;

void InitMKCode(void) {
    s32 why = 0;
    s32 i;

    if (FirstTimeOnlyFlg) {
        FirstTimeOnlyFlg = why;
        VSResultCursor = 10;
        TaResultCursor = 5;
        BtResultCursor = 10;
        if (osEepromProbe(&SIEventMesgQueue) != 0) {
            LoadFunc800B4A10Default();
        }
        if (CheckContPackMenu() != 0) {
            KBGNumber = CONTROLLER_PAK_MENU;
        }
    }
    if (KBGNumber == LOGO_INTRO_MENU) {
        FreeMemoryPointer = StaticMemoryPointer;
        SetSegment(6, LoadPressData((u8*) STARTUP_LOGO_ROM_START, (u8*) STARTUP_LOGO_ROM_END));
    }
    FreeMemoryPointer = StaticMemoryPointer;
    // Hypothetically, this should be a ptr... But only hypothetically.
    TexBuff = AllocateMemory(0x000900B0);
    MeltBuff = AllocateMemory(0x0000CE00);
    KWork = (u8*) AllocateMemory(SCREEN_WIDTH * SCREEN_HEIGHT);
    KdlBuff = (struct_8018EE10_entry*) AllocateMemory(0x00001000);
    AllocFlagobj();
    FBClearFlg = 0;

    for (i = 0; i < 5; i++) {
        FadingFlag[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        ThunderFlg[i] = 0;
    }

    D_800DC5EC->Posx = 160;
    D_800DC5EC->Posy = 120;
    D_800DC5EC->Width = SCREEN_WIDTH;
    D_800DC5EC->Height = SCREEN_HEIGHT;
    KBGChange = FADE_MODE_MAIN;
    TexDataChange();

    for (i = 0; i < 4; i++) {
        NaPlyEngStop((u8) i);
    }

    for (i = 1; i < 8; i++) {
        NaEnmLevelStop((u8) i);
    }

    SettingSoundOutMode();
    osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);
}

s32 CheckContPackMenu(void) {
    UNUSED s32 pad[10];
    s32 i;

    if (!ControllerBits) {
        return 0;
    }
    if ((!ControllerStatuses[0].status) & CONT_CARD_ON) {
        return 0;
    }
    if (!(ControllerOne->ButtonHeld & START_BUTTON)) {
        return 0;
    }
    if (ProbePakType(CONTROLLER_1) != PAK) {
        return 0;
    }
    if (osPfsInit(&SIEventMesgQueue, &ControllerPak1FileHandle, 0)) {
        return 0;
    }

    if (osPfsNumFiles(&ControllerPak1FileHandle, &ControllerPak1NumFilesUsed, &ControllerPak1MaxWriteableFiles)) {
        return 0;
    }

    for (i = 0; i < 16; i++) {
        PfsError[i] = osPfsFileState(&ControllerPak1FileHandle, i, PfsState + i);
    }

    if (osPfsFreeBlocks(&ControllerPak1FileHandle, &ControllerPak1NumPagesFree)) {
        return 0;
    }
    ControllerPak1NumPagesFree >>= 8;
    return 1;
}

void InitGhostData(void) {
    s32 temp_s0;
    s32 temp_s2;
    s32 tmp;

    ControllerPak1State = BAD;
    tmp = InitPak();

    if ((InGameTT != 0) && (tmp == 0)) {
        temp_s2 = (CupSelect * 4) + CourseSelect;
        LoadGameNoteKartData();

        for (temp_s0 = 0; temp_s0 < 2; ++temp_s0) {
            if ((D_8018EE10[temp_s0].GhostDataSaved != 0) && (temp_s2 == D_8018EE10[temp_s0].CourseIndex)) {
                LoadGameNoteKartGhost(temp_s0);
                temp_s0 = 2;
                InGameTT = 0;
            }
        }
    }
}

void InitKawanoGame(void) {
    s32 i;

    //! @todo These sizes need to be sizeof() for shiftability if possible
    MeltBuff = AllocateMemory(0x00002800);
    TexBuff = (u16*) AllocateMemory(0x000124F8);
    KWork = (u8*) AllocateMemory(0x00001000);
    AdditionalPoint = AllocateMemory(4);

    for (i = 0; i < 5; i++) {
        FadingFlag[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        ThunderFlg[i] = 0;
    }

    InitTexBuff();
    InitSeqData();
    InitObjBlock();
    InitVCKawano();
    SetObjBlock(MENU_ITEM_TYPE_096, 0x00000064, 0x00000024, MENU_ITEM_PRIORITY_1);
    SetObjBlock(MENU_ITEM_TYPE_097, 0x00000064, 0x000000DD, MENU_ITEM_PRIORITY_1);
    SetObjBlock(MENU_ITEM_TYPE_098, 0, 0, MENU_ITEM_PRIORITY_0);
    SetObjBlock(MENU_ITEM_PAUSE, 0, 0, MENU_ITEM_PRIORITY_0);
    if (GameMode == TIME_TRIALS) {
        SetObjBlock(MENU_ITEM_TYPE_0BE, 0, 0, MENU_ITEM_PRIORITY_0);
        SetObjBlock(MENU_ITEM_TYPE_10E, 0, 0, MENU_ITEM_PRIORITY_0);
    }
    if ((ReplayFlag != 0) && (GameMode == TIME_TRIALS)) {
        SetObjBlock(MENU_ITEM_END_COURSE_OPTION, 0, 0, MENU_ITEM_PRIORITY_0);
    }
    if (!(ControllerBits & 1) && (DemoPlayFlag != 0)) {
        SetObjBlock(MENU_ITEM_UI_NO_CONTROLLER, 0, 0, MENU_ITEM_PRIORITY_2);
    }
    InitPak();
}

void SkipManager(void) {
    struct Controller* controller = ControllerFive;
    s32 isHeld;

    isHeld = controller->ButtonHeld ? 1 : 0;

    if (controller->ButtonPressed) {
        KSkipStatus = 1;
    }
    if ((isHeld) && (KSkipStatus)) {
        KSkipFlg = 1;
        return;
    }
    KSkipFlg = 0;
}

void SkipReset(void) {
    KSkipStatus = 0;
}

// if selected and detect Kind menu
void SetCursorMessageColor(s32 test, s32 Target, s32 alternative_color) {
    if (test == Target) {
        if (FadeChecker() != 0) {
            SetTextColor(FrameCounter % 3);
        } else {
            SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
        }
    } else {
        SetTextColor(alternative_color);
    }
}

void SetCursorMessageColor4Option(s32 test, s32 Target, s32 alternative_color) {
    if (test == Target) {
        SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
    } else {
        SetTextColor(alternative_color);
    }
}

void InitVCKawano(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        VcColorNum[i] = i;
        VcColorStep[i] = 0;
    }
}

void VCAutoChanger(s32 arg0, s32* arg1, s32* Arg2) {
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

    idx = (((arg0 * 4) + ((FrameCounter % 2) * 2)) - 6);

    *Arg2 += 16;
    if (*Arg2 >= 256) {
        *Arg2 = 0;
        *arg1 = (s32) (*arg1 + 1) % 3;
    }

    for (i = 0; i < 3; i++) {
        vtx = (Vtx*) GetVAddress(D_800E84C0[i]);

        temp_v1 = (*arg1 * 2) + 2;

        temp_t6 = (vtx + temp_v1)->v.cn[0] * (256 - *Arg2);
        temp_t9 = (vtx + temp_v1)->v.cn[1] * (256 - *Arg2);
        temp_t7 = (vtx + temp_v1)->v.cn[2] * (256 - *Arg2);
        temp_t8_2 = (vtx + temp_v1)->v.cn[3] * (256 - *Arg2);

        temp_v1 = (((*arg1 * 2) + 2) % 6) + 2;
        a = ((vtx + temp_v1)->v.cn[0] * *Arg2);
        b = ((vtx + temp_v1)->v.cn[1] * *Arg2);
        c = ((vtx + temp_v1)->v.cn[2] * *Arg2);
        d = ((vtx + temp_v1)->v.cn[3] * *Arg2);

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

    switch (GameMode) {
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
    KwgoalRankScroll();
    OgaGPRoseErhnamDjinn();
}

void DispInfo3P(void) {
    SetObjBlock(MENU_ITEM_TYPE_0AF, 0, 0, MENU_ITEM_PRIORITY_0);
}

void EraseInfo3P(void) {
    ObjBlock* temp = GetOBJBlockPtr(MENU_ITEM_TYPE_0AF);
    if (temp->Pattern == 2) {
        temp->Pattern = 3;
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

void StaffRollKawano1(void) {
    EndingMessageFlg = 1;
}

// Originally func_80092C90
// Some Kind of lookup function, seems to return an Index to be Used
// to get a character's Width in pixels
s32 GetFontNumber(char* character) {
    s32 Index;
    s8 temp_v0;

    temp_v0 = *character;
    Index = 1;
    if ((temp_v0 >= 'a') && (temp_v0 <= 'z')) {
        Index = temp_v0 - 0x61;
    } else if ((temp_v0 >= 'A') && (temp_v0 <= 'Z')) {
        Index = temp_v0 - 0x41;
    } else if ((temp_v0 >= '0') && (temp_v0 <= '9')) {
        Index = temp_v0 - 0x10;
    } else if (temp_v0 == ' ') {
        Index = -1;
    } else if (temp_v0 < 0) {
        // Handling EUC-JUP characters
        switch (temp_v0) { /* irregular */
            case -92:      // 0xA4
                Index = GetFontNumberH(character + 1);
                break;
            case -91: // 0xA5
                Index = GetFontNumberK(character + 1);
                break;
            case -95: // 0xA1
            case -93: // 0xA3
            case -85: // 0xAB
                Index = GetFontNumberJ(character);
                break;
        }
    } else {
        switch (temp_v0) {
            case '!':
                Index = 0x0000001A;
                break;
            case '-':
                Index = 0x0000001B;
                break;
            case '?':
                Index = 0x0000001C;
                break;
            case '\'':
                Index = 0x0000001D;
                break;
            case '$':
                Index = 0x0000001E;
                break;
            case '.':
                Index = 0x0000001F;
                break;
            case '\"':
                Index = 0x0000002A;
                break;
            case '*':
                Index = 0x0000002B;
                break;
            case '+':
                Index = 0x0000002C;
                break;
            // Displayed as "cc"
            case '(':
                Index = 0x0000002D;
                break;
            case ',':
                Index = 0x0000002E;
                break;
            default:
                Index = -2;
                break;
        }
    }
    return Index;
}

s32 GetFontNumberK(char* arg) {
    return GetFontNumberH(arg) + 0x50;
}

s32 GetFontNumberH(char* character) {
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

s32 GetFontNumberJ(char* character) {
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
s32 GetWordLength(char* Buffer) {
    s32 glyphIndex;
    s32 stringWidth = 0;

    if (*Buffer != 0) {
        do {
            glyphIndex = GetFontNumber(Buffer);
            if (glyphIndex >= 0) {
                stringWidth += FontWidth[glyphIndex];
            } else if (glyphIndex == -1) {
                stringWidth += 7;
            }
            if (glyphIndex >= 0x30) {
                Buffer += 2;
            } else {
                Buffer += 1;
            }
        } while (*Buffer != 0);
    }
    return stringWidth;
}

void SetTextColor(s32 arg0) {
    AfontColor = arg0;
}

UNUSED void SetWord(s32 arg0, s32 arg1, char* Arg2) {
    SetTextColor(TEXT_BLUE);
    SetWord2A(arg0, arg1, Arg2, 0, 1.0, 1.0);
}

// "tracking" is a uniform spacing between all characters in a given word
void SetWord2(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 Mode) {
    s32 stringWidth = 0;
    s32 glyphIndex;

    gSPDisplayList(DisplayListHead++, D_020077A8);
    if (*text != 0) {
        do {
            glyphIndex = GetFontNumber(text);
            if (glyphIndex >= 0) {
                TexBuffLoad((MenuTexture*) GetTBPointer((const void*) GlyphTextureLUT[glyphIndex]));
                DisplayListHead =
                    DoTexList4(DisplayListHead,
                                 (MenuTexture*) GetTBPointer((const void*) GlyphTextureLUT[glyphIndex]),
                                 column + (stringWidth * scaleX), row, Mode, scaleX, scaleY);
                stringWidth += FontWidth[glyphIndex] + tracking;
            } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
                stringWidth += tracking + 7;
            } else {
                gSPDisplayList(DisplayListHead++, D_020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }
        } while (*text != 0);
    }
    gSPDisplayList(DisplayListHead++, D_020077D8);
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
            stringWidth += ((FontWidth[glyphIndex] + tracking) * scaleX);
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

    gSPDisplayList(DisplayListHead++, D_020077A8);
    while (*text != 0) {
        glyphIndex = GetFontNumber(text);
        if (glyphIndex >= 0) {
            TexBuffLoad(GetTBPointer(GlyphTextureLUT[glyphIndex]));
            DisplayListHead = DoTexList4(DisplayListHead, GetTBPointer(GlyphTextureLUT[glyphIndex]),
                                            column, row, sp60, scaleX, scaleY);
            column = column + (s32) ((FontWidth[glyphIndex] + tracking) * scaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * scaleX);
        } else {
            gSPDisplayList(DisplayListHead++, D_020077D8);
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }
    }
    gSPDisplayList(DisplayListHead++, D_020077D8);
}

void SetWord3A(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, LEFT_TEXT);
}

void PrintText1CenterMode1(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, CENTER_TEXT_MODE_1);
}

void SetWord3AP(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, RIGHT_TEXT);
}

void SetWord3ACP(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    SetWord3(column, row, text, tracking, scaleX, scaleY, CENTER_TEXT_MODE_2);
}

void PrintText2(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY, s32 arg6) {
    MenuTexture* glyphTexture;
    s32 characterWidth;
    s32 glyphIndex;

    gSPDisplayList(DisplayListHead++, D_020077A8);
    if (*text != 0) {
        do {
            glyphIndex = GetFontNumber(text);
            if (glyphIndex >= 0) {
                glyphTexture = (MenuTexture*) GetTBPointer((const void*) GlyphTextureLUT[glyphIndex]);
                TexBuffLoad(glyphTexture);
                DisplayListHead =
                    DoTexList4(DisplayListHead, glyphTexture, column - (FontWidth[glyphIndex] / 2), row,
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
                gSPDisplayList(DisplayListHead++, D_020077D8);
                return;
            }
            if (glyphIndex >= 0x30) {
                text += 2;
            } else {
                text += 1;
            }
        } while (*text != 0);
    }

    gSPDisplayList(DisplayListHead++, D_020077D8);
}

void SetWord4A(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    PrintText2(column, row, text, tracking, scaleX, scaleY, 1);
}

void TextDraw(s32 column, s32 row, char* text, s32 tracking, f32 scaleX, f32 scaleY) {
    PrintText2(column, row, text, tracking, scaleX, scaleY, 2);
}

void KawanoDrawAfterSky(s32 arg0) {
    ThunderMain(ScreenMode2Num[arg0]);
}

void KawanoDraw2(u32 arg0) {
    if (ScreenCounter == 0) {
        BeforeFadeMain();
    }
    switch (arg0) {
        case RENDER_SCREEN_MODE_1P_PLAYER_ONE:
            KawanoDraw((s32) ScreenMode2Num[arg0]);
            break;
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_ONE:
        case RENDER_SCREEN_MODE_2P_HORIZONTAL_PLAYER_TWO:
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_ONE:
        case RENDER_SCREEN_MODE_2P_VERTICAL_PLAYER_TWO:
            if (ScreenCounter == 0) {
                KawanoOnlyFade((s32) ScreenMode2Num[arg0]);
            } else {
                KawanoDraw((s32) ScreenMode2Num[arg0]);
            }
            break;
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_ONE:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_TWO:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_THREE:
        case RENDER_SCREEN_MODE_3P_4P_PLAYER_FOUR:
            if (ScreenCounter == 3) {
                KawanoDraw((s32) ScreenMode2Num[arg0]);
            } else {
                KawanoOnlyFade((s32) ScreenMode2Num[arg0]);
            }
            break;
    }
    gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
}

UNUSED void KawanoDraw2Backup(u32 arg0) {
    if ((arg0 == 0) || (arg0 == 2) || (arg0 == 3) || (arg0 == 8)) {
        BeforeFadeMain();
    }
    switch (arg0) {
        case 0:
            KawanoDraw(0);
            break;
        case 2:
        case 3:
        case 8:
            KawanoOnlyFade(0);
            break;
        case 1:
        case 4:
        case 9:
            KawanoDraw(1);
            break;
        case 10:
            KawanoOnlyFade(2);
            break;
        case 11:
            KawanoDraw(3);
            break;
        default:
            break;
    }
}

void KawanoOnlyFade(s32 arg0) {
    gSPDisplayList(DisplayListHead++, D_02007F18);
    FadeMain2(arg0);
    gSPDisplayList(DisplayListHead++, D_02007F48);
}

UNUSED void Randomf(void) {
    return;
}

UNUSED void Testprog(void) {
    return;
}

void KawanoDrawFinalEx(s32 arg0) {
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&DynamicP->MTXEffect[Effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(DisplayListHead++, D_02007F18);
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    DoOBJBlock2();
    if (arg0 == 0) {
        DispOBPauseCursor();
        FadeMain2(4);
        D_80165754 = Effectcount;
        Effectcount = 0;
    }
}

void KawanoDrawFinal(void) {
    KawanoDrawFinalEx(0);
}

void KawanoDrawFinal4Select(void) {
    KawanoDrawFinalEx(1);
}

void InitKawanoResult(void) {
    s32 i;

    MeltBuff = AllocateMemory(0x00002800);
    TexBuff = (u16*) AllocateMemory(0x000124F8);
    KWork = AllocateMemory(0x00001000);
    AdditionalPoint = AllocateMemory(4U);

    for (i = 0; i < 5; i++) {
        FadingFlag[i] = 0;
    }

    for (i = 0; i < 4; i++) {
        ThunderFlg[i] = 0;
    }

    InitTexBuff();
    InitSeqData();
    InitObjBlock();
    InitVCKawano();
    EndingMessageFlg = 0;
}

void KawanoDrawResult(void) {
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&DynamicP->MTXEffect[Effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(DisplayListHead++, D_02007F18);
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, 320, 240);
    VCAutoChanger(4, VcColorNum, VcColorStep);
    VCAutoChanger(5, (s32*) &VcColorNum[1], (s32*) &VcColorStep[1]);
    BeforeFadeMain();
    InitChangeTexData();
    InitChangeTexDataVS();
    DoOBJBlock1();
    DoChangeTexData();
    DoChangeTexDataVS();
    FadeMain();
    gSPDisplayList(DisplayListHead++, D_02007F48);
    Effectcount = 0;
}

void KawanoDraw(s32 arg0) {
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    guOrtho(&DynamicP->MTXEffect[Effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(DisplayListHead++, D_02007F18);
    VCAutoChanger(4, VcColorNum, VcColorStep);
    VCAutoChanger(5, (s32*) &VcColorNum[1], (s32*) &VcColorStep[1]);
    SkipManager();
    InitChangeTexData();
    InitChangeTexDataVS();
    DoOBJBlock1();
    DoChangeTexData();
    DoChangeTexDataVS();
    FadeMain2(arg0);
    gSPDisplayList(DisplayListHead++, D_02007F48);
    KWReturnViewport();
}

void LogoProc(void) {
    Mtx* test;
    f32 var_f26;
    s32 var_s2;
    s32 thing;
    test = &DynamicP->MTXObject[0];
    gSPMatrix(DisplayListHead++, &DynamicP->MTXScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, &DynamicP->MTXLookAt[0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    guRotate(test, LogoRotX, 1.0f, 0.0f, 0.0f);
    guRotate(test + 1, LogoRotY, 0.0f, 1.0f, 0.0f);
    guScale(test + 2, 1.0f, 1.0f, LogoScaleZ);
    gSPMatrix(DisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(DisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(DisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPSetEnvColor(DisplayListHead++, 0x00, 0x00, 0x00, 0x00);
    //gSPDisplayList(DisplayListHead++, D_02007F60);
    gSPDisplayList(DisplayListHead++, 0x080052A0);
    
    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
    if (LogoDelaySize > 0) {
        var_f26 = LogoYangSpeed;
        if (var_f26 > 10.0f) {
            var_f26 = 10.0f;
        }
        for (var_s2 = 0, thing = 0xC0; var_s2 < 0xC; var_s2++, thing -= 0x10) {
            guRotate(test, 0.0f, 1.0f, 0.0f, 0.0f);
            guRotate(test + 1, (var_s2 + 1) * LogoDelaySize * var_f26, 0.0f, 1.0f, 0.0f);
            guScale(test + 2, 1.0f, 1.0f, 2.0f);
            gSPMatrix(DisplayListHead++, test++, G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(DisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gSPMatrix(DisplayListHead++, test++, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            gDPSetRenderMode(DisplayListHead++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
            gDPSetEnvColor(DisplayListHead++, 0x00, 0x00, 0x00, thing);
            gSPDisplayList(DisplayListHead++, startup_texture_dl4);
            gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
        }
    }
}

void DoNintenLogo(struct GfxPool* arg0, UNUSED s32 arg1) {
    u16 perspNorm;
    StoreSegments();
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    guPerspective(&arg0->MTXScreen, &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    guLookAt(&arg0->MTXLookAt[0], 0.0f, 0.0f, (f32) LogoCamZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    LogoProc();
    gDPPipeSync(DisplayListHead++);
    gDPSetTexturePersp(DisplayListHead++, G_TP_NONE);
    gDPSetTextureFilter(DisplayListHead++, G_TF_BILERP);
}

void DoCheckerFlag(struct GfxPool* arg0, UNUSED s32 arg1) {
    u16 perspNorm;
    StoreSegments();
    guPerspective(&arg0->MTXPersp[0], &perspNorm, 45.0f, 1.3333334f, 100.0f, 12800.0f, 1.0f);
    gSPPerspNormalize(DisplayListHead++, perspNorm);
    guLookAt(&arg0->MTXLookAt[1], 0.0f, 0.0f, (f32) LogoCamZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    guRotate(&arg0->MTXObject[0], LogoRotX, 1.0f, 0, 0);
    guRotate(&arg0->MTXObject[1], LogoRotY, 0, 1.0f, 0);
    guRotate(&arg0->MTXObject[2], LogoRotZ, 0, 0, 1.0f);
    guScale(&arg0->MTXObject[3], LogoScaleZ, LogoScaleZ, LogoScaleZ);
    guTranslate(&arg0->MTXObject[4], LogoPosX, LogoPosY, LogoPosZ);
    gSPMatrix(DisplayListHead++, &arg0->MTXPersp[0], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(DisplayListHead++, &arg0->MTXLookAt[1], G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPMatrix(DisplayListHead++, &arg0->MTXObject[0], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(DisplayListHead++, &arg0->MTXObject[1], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(DisplayListHead++, &arg0->MTXObject[2], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(DisplayListHead++, &arg0->MTXObject[3], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPMatrix(DisplayListHead++, &arg0->MTXObject[4], G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    gSPDisplayList(DisplayListHead++, D_02007FC8);
    DrawFlag();
    gSPDisplayList(DisplayListHead++, D_02007650);
}

void DoGameSelect(struct GfxPool* Pool) {
    Modelingnum = 0;
    Effectcount = 0;
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&D_802B8880));
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    guOrtho(&Pool->MTXScreen, 0.0f, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(DisplayListHead++, &Pool->MTXScreen, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPDisplayList(DisplayListHead++, D_02007650);
    TexDataChange();
    VCAutoChanger(4, VcColorNum, VcColorStep);
    VCAutoChanger(5, &VcColorNum[1], &VcColorStep[1]);
    InitChangeTexData();
    BeforeFadeMain();
    switch (KBGNumber) {
        case START_MENU:
            DoGameSelect10();
            KawanoDrawFinal4Select();
            break;
        case OPTIONS_MENU:
        case DATA_MENU:
        case COURSE_DATA_MENU:
        case LOGO_INTRO_MENU:
        case CONTROLLER_PAK_MENU:
        case MAIN_MENU:
        case CHARACTER_SELECT_MENU:
        case COURSE_SELECT_MENU:
            DoOBJBlock1();
            DoChangeTexData();
            break;
    }
    TitleMenuDefault();
    FadeMain();
    KFlashCounter += 1;
    gDPPipeSync(DisplayListHead++);
    gSPDisplayList(DisplayListHead++, D_020076B0);
}

void TexDataChange(void) {
    if (KBGChange != FADE_MODE_NONE) {
        InitTexBuff();
        InitSeqData();
        InitObjBlock();
        InitKDL();
        InitVCKawano();
        InitPak();
        InitPak2();
        KVarInit(KBGNumber);
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
                if (ControllerBits & 1) {
                    SetObjBlock(MENU_ITEM_UI_PUSH_START_BUTTON, 0, 0, MENU_ITEM_PRIORITY_2);
                } else {
                    SetObjBlock(MENU_ITEM_UI_NO_CONTROLLER, 0, 0, MENU_ITEM_PRIORITY_2);
                }
                DemoFlag = 0;
                DemoPlayFlag = 0;
                break;
            case MAIN_MENU:
                SetObjBlock(MAIN_MENU_BACKGROUND, 0, 0, MENU_ITEM_PRIORITY_2);
                SetObjBlock(MENU_ITEM_UI_GAME_SELECT, 0x0000015E, 0x00000011, MENU_ITEM_PRIORITY_1);
                SetObjBlock(MENU_ITEM_UI_4P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_3P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_2P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_1P_GAME, 0x0000015E, 0x0000003E, MENU_ITEM_PRIORITY_6);
                SetObjBlock(MENU_ITEM_UI_OK, 0x0000015E, 0x000000C8, MENU_ITEM_PRIORITY_6);
                if (Check4GoldCup150CC() != 0) {
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
                if (GameMode != BATTLE) {
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
                if (GameMode == TIME_TRIALS) {
                    SetObjBlock(MENU_ITEM_TYPE_065, 0, 0, MENU_ITEM_PRIORITY_1);
                    SetObjBlock(MENU_ITEM_TYPE_066, 0, 0, MENU_ITEM_PRIORITY_1);
                    SetObjBlock(MENU_ITEM_TYPE_069, 0, 0, MENU_ITEM_PRIORITY_1);
                }
                if (GameMode == GRAND_PRIX) {
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

void DoGameSelect10(void) {
    s32 var_v0;

    if ((FBClearFlg < 3) || (FadingFlag[4] != 0)) {
        DoOBJBlock1();
    }
    if (TitleMode > DEBUG_MENU_DISABLED) { // If not Disabled
        LoadFont();
        PrintString(0x00000050, 0x00000064, "debug_mode");
        switch (TitleMode) {
            case DEBUG_MENU_DEBUG_MODE:
                PrintString(0x00000046, 0x00000064, "*");
                break;
            case DEBUG_MENU_COURSE:
                PrintString(0x00000046, 0x0000006E, "*");
                break;
            case DEBUG_MENU_SCREEN_MODE:
                PrintString(0x00000046, 0x00000078, "*");
                break;
            case DEBUG_MENU_PLAYER:
                PrintString(0x00000046, 0x00000082, "*");
                break;
            case DEBUG_MENU_SOUND_MODE:
                PrintString(0x00000046, 0x0000008C, "*");
                break;
            case DEBUG_MENU_GIVE_ALL_GOLD_CUP:
                PrintString(0x00000046, 0x00000096, "*");
                break;
        }
        if (DebugMode) {
            PrintString(0x000000AA, 0x00000064, "on");
        } else {
            PrintString(0x000000AA, 0x00000064, "off");
        }
        if ((CourseID >= (NUM_COURSES - 1)) || (CourseID < 0)) {
            CourseID = 0;
        }
        PrintStringNumber(0x00000050, 0x0000006E, "map_number", CourseID);
        if (CourseID < 0xA) {
            var_v0 = 0;
        } else {
            var_v0 = 8;
        }
        PrintString(var_v0 + 0xB9, 0x0000006E, GET_COURSE_debugName);
        PrintString(0x00000050, 0x00000078, "screen_mode");
        PrintString(0x000000AA, 0x00000078, ScreenModeNames[ScreenModeListIndex]);
        PrintString(0x00000050, 0x00000082, "player");
        PrintString(0x000000AA, 0x00000082, DriversName[CharacterSelections[0]]);
        PrintString(0x00000050, 0x0000008C, "sound mode");
        PrintString(0x000000AA, 0x0000008C, SoundModeName[SoundOutput]);
        if (TitleMode == DEBUG_MENU_GIVE_ALL_GOLD_CUP) {
            PrintString(0x00000050, 0x00000096, "push b to get all goldcup");
        }
        KwprintmodeFinish();
    }
    if (TitleMode == DEBUG_MENU_DISABLED) {
        TitleDemo += 1;
    } else {
        TitleDemo = 3;
    }
    if (TitleDemo == 2) {
        PlaySound(SOUND_INTRO_WELCOME);
    }
    /**
     * @bug Pressing START in the last moments before the demo timeout plays the
     * menu-enter chime but then drops into the demo with no music. The attract
     * Timer fires while the press-Start fade is still in flight and overwrites
     * the fade Target, so the fade completes as a demo entry instead of the
     * Main menu. Roughly a 0.7 second window every attract cycle.
     */
#ifdef AVOID_UB
    if ((TitleDemo > 300) && (FadeChecker() == 0)) {
#else
    if (TitleDemo > 300) {
#endif
        SetFadeOutD();
        NaSeMuteTitle();
    }
    gSPDisplayList(DisplayListHead++, D_020076E0);
}

// While this matches, its a little screwy
// This function seemingly needs to return a Gfx*, but doing that explicity doesn't match
// Instead we depend on the fact that the result of FillRect1Color is Left
// in v0 which means it is returned, sort of.
// Its also weird that the displayListHead argument goes entirely Unused. What's up with that?
Gfx* FlashRectangle2Ex(UNUSED Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 Speed) {
    s32 greyscale;

    greyscale = ((KFlashCounter % Speed) << 9) / Speed;
    if (greyscale > 0x100) {
        greyscale = 0x200 - greyscale;
    }

    if (greyscale > 0xFF) { // set max greyscale to 0xFF
        greyscale = 0xFF;
    }

#if AVOID_UB
    return DisplayListHead =
               FillRect1Color(DisplayListHead, ulx, uly, lrx, lry, greyscale, greyscale, greyscale, 0xFF);
#else
    DisplayListHead = FillRect1Color(DisplayListHead, ulx, uly, lrx, lry, greyscale, greyscale, greyscale, 0xFF);
#endif
}

Gfx* FlashRectangle2(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return FlashRectangle2Ex(displayListHead, ulx, uly, lrx, lry, 64);
}

Gfx* FlashRectangle2f(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return FlashRectangle2Ex(displayListHead, ulx, uly, lrx, lry, 4);
}

Gfx* SetAfontVertex(Gfx* displayListHead, Vtx* arg1) {
    s32 Index;

    if ((s32) AfontColor < TEXT_BLUE_GREEN_RED_CYCLE_1) {
        Index = AfontColor;
    } else {
        Index = ((AfontColor * 2) + ((s32) FrameCounter % 2)) - 4;
    }
#ifdef AVOID_UB
    gSPVertex(displayListHead++, arg1, 2, 0);
    gSPVertex(displayListHead++, &arg1[(Index + 1) * 2], 2, 2);
    gSPDisplayList(displayListHead++, common_rectangle_display);
#else
    if (arg1 == D_02007BB8) {
        gSPDisplayList(displayListHead++, D_800E84CC[Index]);
    } else if (arg1 == D_02007CD8) {
        gSPDisplayList(displayListHead++, D_800E84EC[Index]);
    } else if (arg1 == D_02007DF8) {
        gSPDisplayList(displayListHead++, D_800E850C[Index]);
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
void SetmtxScaletrans(MTX_TYPE* arg0, f32 arg1, f32 Arg2, f32 Arg3, f32 arg4) {
#ifdef AVOID_UB
    // Use Mat4 array to set Matrix values using guMtxF2L. This helps little-endian systems.
    Mat4 src;
    src[0][0] = Arg3;
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
    src[3][1] = Arg2;
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

    sp14.w = Arg3 * 65536.0f;
    sp10.w = arg4 * 65536.0f;
    spC.w = arg1 * 65536.0f;
    sp8.w = Arg2 * 65536.0f;
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

Gfx* SetIAFont(Gfx* displayListHead, u8* arg1, f32 Arg2, f32 Arg3, u32 arg4, u32 arg5, f32 arg6, f32 arg7) {
    Vtx* var_a1;
    Mtx* sp28;

    // A match is a match, but why are goto's required here?
    if (Effectcount >= 0x2F7) {
        goto func_80095BD0_label1;
    }
    sp28 = &DynamicP->MTXEffect[Effectcount];
    if (Effectcount < 0) {
        rmonPrintf("effectcount < 0 !!!!!!(kawano)\n");
    }
    goto func_80095BD0_label2;
func_80095BD0_label1:
    rmonPrintf("MAX effectcount(760) over!!!!(kawano)\n");
    return displayListHead;
func_80095BD0_label2:
    SetmtxScaletrans((void*) sp28, Arg2, Arg3, arg6, arg7);
    gSPMatrix(displayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Effectcount += 1;
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

Gfx* AutoTextureRect(Gfx* displayListHead, s8 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
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
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, Arg2, Arg3);

            arg8 += var_t0;
        }

        arg8 = sp7C;
        arg9 += temp_lo;
    }
    return displayListHead;
}

Gfx* AutoTextureRectZoom(Gfx* displayListHead, s8 arg1, s32 Arg2, s32 Arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8,
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
    Arg2 /= arg4;
    Arg3 /= arg5;

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
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, Arg2, Arg3);

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
 * This function is responsible for drawing a near unnoticeable static Pattern
 * over the course images when loading the cup selection screen.
 * It may or may not use gTextureTitleChocoMountain to create that Pattern.
 * Try locking the word at `8018DC80` to see something like 0x20 just before confirming character selection to make it
 * last longer
 */
Gfx* AutoTextureRectNoise(Gfx* displayListHead, s32 xPos, s32 YPos, u32 Width, u32 Height) {
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

    while ((u32) tileWidth < Width) {
        tileWidth *= 2;
    }

    tileHeight = 1024 / tileWidth;
    while ((u32) (tileHeight / 2) > Height) {
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
        Width -= xPos;
        xPos = 0;
    } else if ((xPos + Width) > SCREEN_WIDTH) {
        Width = SCREEN_WIDTH - xPos;
    }
    if (YPos < 0) {
        Height -= YPos;
        YPos = 0;
    } else if ((YPos + Height) > SCREEN_HEIGHT) {
        Height = SCREEN_HEIGHT - YPos;
    }

    if (Width == 0) {
        return displayListHead;
    }
    if (Height == 0) {
        return displayListHead;
    }

    rnd = MakeRandomLimmit(100);
    displayListHead = FillRect1ColorF(displayListHead, xPos, YPos, xPos + Width, YPos + Height, 0, 0, 0, rnd);
    rnd += 150;

    gDPPipeSync(displayListHead++);
    gDPSetRenderMode(displayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(displayListHead++, 0, 0, rnd, rnd, rnd, rnd);
    gDPSetCombineMode(displayListHead++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    for (y = YPos; y < (YPos + Height); y += tileHeight) {
        if ((y + tileHeight) > (YPos + Height)) {
            rectYoffset = YPos + Height - y;
            if (rectYoffset == 0) {
                break;
            }
        } else {
            rectYoffset = tileHeight;
        }
        for (x = xPos; x < xPos + Width; x += tileWidth) {
            if (x + tileWidth > xPos + Width) {
                rectXoffset = xPos + Width - x;
                if (rectXoffset == 0) {
                    break;
                }
            } else
                rectXoffset = tileWidth;

            gDPLoadTextureTile(displayListHead++, (D_0B002A00 + MakeRandomLimmit(128) * 2), G_IM_FMT_IA, G_IM_SIZ_16b, Width,
                               Height, x, y, x + rectXoffset, y + rectYoffset, 0, G_TX_WRAP, G_TX_WRAP, masks, maskt,
                               G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(displayListHead++, x << 2, y << 2, (x + rectXoffset) << 2, (y + rectYoffset) << 2,
                                G_TX_RENDERTILE, (x * 32) & 0xFFFF, (y * 32) & 0xFFFF, 1024, 1024);
        }
    }

    return displayListHead;
}

Gfx* AutoTextureRect2Cyc(Gfx* displayListHead, s8 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
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
    gDPSetPrimColor(displayListHead++, 0, 0, 0, 0, 0, FrameCounter % 256);
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
                                (var_a1_2 * 32) & 0xFFFF, (var_s3 * 32) & 0xFFFF, Arg2, Arg3);
            arg8 += var_t0;
        }
        arg8 = sp7C;
        arg9 += temp_lo;
    }
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}

Gfx* AutoTextureRectC(Gfx* displayListHead, s8 arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, u8* arg8,
                   u32 arg9, u32 argA) {
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_COPY);
    displayListHead = AutoTextureRect(displayListHead, arg1, 0x00001000, 0x00000400, Arg2, Arg3, arg4, arg5, arg6, arg7,
                                    arg8, arg9, argA);
    gDPPipeSync(displayListHead++);
    gDPSetCycleType(displayListHead++, G_CYC_1CYCLE);
    return displayListHead;
}

Gfx* AutoTextureRect64(Gfx* displayListHead, s8 fmt, s32 Arg2, s32 Arg3, u8* arg4, s32 Width) {
    s32 i;
    s32 temp;
    s32 arg2Copy;
    s32 dsdx;

    if (Width >= 32) {
        return displayListHead;
    }

    arg2Copy = Arg2;

    for (i = 0; i < 64; i += 32) {
        temp = 0;
        dsdx = 0x8000 / (32 - Width);
        gDPLoadTextureTile(displayListHead++, arg4, fmt, G_IM_SIZ_16b, 64, 64, temp, i, temp + 32, i + 32, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (Arg2 + Width) << 2, Arg3 << 2, (Arg2 + 32) << 2, (Arg3 + 32) << 2, 0, 0,
                            0, dsdx, 1024);

        Arg2 += 32;

        gDPLoadTextureTile(displayListHead++, arg4, fmt, G_IM_SIZ_16b, 64, 64, temp + 32, i, temp + 64, i + 32, 0,
                           G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, Arg2 << 2, Arg3 << 2, ((Arg2 - Width) + 32) << 2, (Arg3 + 32) << 2, 0, 0,
                            0, dsdx, 1024);

        Arg2 = arg2Copy;
        Arg3 += 32;
    }
    return displayListHead;
}

Gfx* AutoTextureRectW(Gfx* displayListHead, s8 fmt, UNUSED u32 Arg2, u32 Arg3, UNUSED u32 arg4, u32 arg5, s32 arg6,
                   s32 arg7, u8* someTexture, u32 arg9, UNUSED u32 argA, s32 Width) {
    u32 ult;
    u32 temp;
    s32 arg6Copy;
    s32 temp_v1;
    s32 var_s2;
    s32 lrs;
    s32 spDC;
    s32 temp2 = 32;

    if (Width >= 32) {
        return displayListHead;
    }

    arg6Copy = arg6;

    lrs = arg9 / 2;
    spDC = arg9 - lrs;
    for (ult = Arg3; ult < arg5; ult += 32) {
        temp = 0;
        if ((ult + temp2) > arg5) {
            var_s2 = arg5 - ult;
            if (!var_s2) {
                break;
            }
        } else {
            var_s2 = temp2;
        }
        temp_v1 = ((32 * lrs) << 10) / (lrs * (32 - Width));

        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp, ult, temp + lrs,
                           ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, (arg6 + lrs * Width / 32) << 2, arg7 << 2, (arg6 + lrs) << 2,
                            (arg7 + var_s2) << 2, 0, 0, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 += lrs;
        temp_v1 = ((32 * spDC) << 10) / (spDC * (32 - Width));
        gDPLoadTextureTile(displayListHead++, someTexture, fmt, G_IM_SIZ_16b, arg9, argA, temp + lrs, ult, temp + arg9,
                           ult + var_s2, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD,
                           G_TX_NOLOD);
        gSPTextureRectangle(displayListHead++, arg6 << 2, arg7 << 2, (arg6 + spDC * (32 - Width) / 32) << 2,
                            (arg7 + var_s2) << 2, 0, (lrs << 5) & 0xFFFF, (ult << 5) & 0xFFFF, temp_v1, 1024);
        arg6 = arg6Copy;
        arg7 += temp2;
    }
    return displayListHead;
}

Gfx* AutoTextureRectSimple(Gfx* displayListHead, u32 arg1, u32 Arg2, u32 Arg3, u32 arg4, u32 arg5, u32 arg6, UNUSED s32 arg7,
                   s32 arg8) {
    u32 var_a3;
    u32 var_v0;
    s32 arg5Copy;

    arg5Copy = arg5;
    for (var_v0 = Arg2; var_v0 < arg4; var_v0 += 0x20) {
        for (var_a3 = arg1; var_a3 < Arg3; var_a3 += 0x20) {
            gDPLoadTextureTile(displayListHead++, TexBuff, G_IM_FMT_RGBA, G_IM_SIZ_16b, arg8, 0, var_a3,
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

Gfx* AutoTextureRectR(Gfx* displayListHead, u32 arg1, u32 Arg2, u32 Width, u32 Height, s32 column, s32 row,
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

    if (FadeCounter[0] == 0) {
        FadeCounter[0] = 1;
    }
    temp_f24 = SinT(((FadeCounter2[0] * 0x4E20) / FadeCounter[0]) % 20000U);
    temp_f0 = CosT(((FadeCounter2[0] * 0x4E20) / FadeCounter[0]) % 20000U);
    temp_f18 = (((f32) FadeCounter2[0] * 0.5) / FadeCounter[0]) + 1.0;
    columnCopy = column;
    for (var_v0_2 = Arg2; (u32) var_v0_2 < Height; var_v0_2 += 0x20) {
        for (var_a2 = arg1; (u32) var_a2 < Width; var_a2 += 0x20) {
            gDPLoadTextureTile(displayListHead++, TexBuff, G_IM_FMT_RGBA, G_IM_SIZ_16b, textureWidth, 0,
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
Gfx* FillRect1Color(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, s32 red, s32 Green, s32 Blue, s32 Alpha) {
    red &= 0xFF;
    Green &= 0xFF;
    Blue &= 0xFF;
    Alpha &= 0xFF;
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
    gDPSetFillColor(displayListHead++, (GPACK_RGBA5551(red, Green, (u32) Blue, Alpha) << 0x10 |
                                        GPACK_RGBA5551(red, Green, (u32) Blue, Alpha)));
    gDPFillRectangle(displayListHead++, ulx, uly, lrx, lry);
    gSPDisplayList(displayListHead++, D_02008058);
    return displayListHead;
}

// draw a box with a solid outline
Gfx* FillRect1ColorF(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry, u32 red, u32 Green, u32 Blue, u32 Alpha) {
    red &= 0xFF;
    Green &= 0xFF;
    Blue &= 0xFF;
    Alpha &= 0xFF;
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
    gDPSetPrimColor(displayListHead++, 0, 0, red, Green, Blue, Alpha);
    gDPFillRectangle(displayListHead++, ulx, uly, lrx, lry);
    gDPPipeSync(displayListHead++);
    return displayListHead;
}

Gfx* FillRectBlack(Gfx* displayListHead, s32 ulx, s32 uly, s32 lrx, s32 lry) {
    return FillRect1Color(displayListHead, ulx, uly, lrx, lry, 0, 0, 0, 0xFF);
}

void DMABase729A30(u64* data, size_t nbytes, void* vaddr) {
    OSIoMesg mb;
    OSMesg msg;

    osInvalDCache(vaddr, nbytes);
    osPiStartDma(&mb, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) &_textures_0aSegmentRomStart[SEGMENT_OFFSET(data)],
                 vaddr, nbytes, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &msg, OS_MESG_BLOCK);
}

void TexDataDmaS(u64* data, size_t nbytes, void* vaddr) {
    OSIoMesg mb;
    OSMesg msg;

    osInvalDCache(vaddr, nbytes);
    osPiStartDma(&mb, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) &_textures_0bSegmentRomStart[SEGMENT_OFFSET(data)],
                 vaddr, nbytes, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &msg, OS_MESG_BLOCK);
}

void InitTexBuff(void) {
    TexBuffNum = 0;
    AllTexBlockNum = 0;
}

/**
 * Differs from memory.c with `+ 0x8` instead of `| 0x8`
 *
 * @Param Addr
 * @return void*
 */
void* GetTBPointer(const void* Addr) {
    size_t segment = (uintptr_t) Addr >> 24;
    size_t Offset = (uintptr_t) Addr & 0x00FFFFFF;

    return (void*) ((SegmentTable[segment] + Offset) + 0x80000000);
}

void* GetVAddress(const void* Addr) {
    size_t segment = (uintptr_t) Addr >> 24;
    size_t Offset = (uintptr_t) Addr & 0x00FFFFFF;

    return (void*) ((SegmentTable[segment] + Offset) + 0x80000000);
}

void TexBuffLoad(MenuTexture* Addr) {
    u16 Size;
    s32 i;
    s32 imgLoaded;
    MenuTexture* texAddr;
    TextureMap* texMap = &SMenuTextureMap[0];

    texAddr = GetTBPointer(Addr);
    while (texAddr->TextureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < AllTexBlockNum; i++) {
            if (texAddr->TextureData == (texMap + i)->TextureData) {
                imgLoaded = true;
                break;
            }
        }

        if (imgLoaded == false) {
            if (texAddr->Type == 3) {
                if (texAddr->Size != 0) {
                    Size = texAddr->Size;
                } else {
                    Size = 0x1000;
                }
                if (Size % 8) {
                    Size = ((Size / 8) * 8) + 8;
                }
                DMABase729A30(texAddr->TextureData, Size, MeltBuff);
                decodeMIO0((u8*) MeltBuff, (u8*) &TexBuff[TexBuffNum]);
            } else {
                DMABase729A30(texAddr->TextureData, (texAddr->Height * texAddr->Width) * 2,
                                      &TexBuff[TexBuffNum]);
            }
            texMap[AllTexBlockNum].TextureData = texAddr->TextureData;
            texMap[AllTexBlockNum].Offset = TexBuffNum;
            TexBuffNum += texAddr->Height * texAddr->Width;
            TexBuffNum = ((TexBuffNum / 8) * 8) + 8;
            AllTexBlockNum += 1;
        }
        texAddr++;
    }
}

void TexBuffLoadFont(MenuTexture* Addr) {
    s32 i;
    s32 imgLoaded;
    MenuTexture* texAddr;
    TextureMap* texMap = &SMenuTextureMap[0];

    texAddr = GetTBPointer(Addr);
    while (texAddr->TextureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < AllTexBlockNum; i++) {
            if (texAddr->TextureData == (texMap + i)->TextureData) {
                imgLoaded = true;
                break;
            }
        }

        if (imgLoaded == false) {
            if (texAddr->Type == 5) {
                DMABase729A30(texAddr->TextureData, (u32) (((s32) (texAddr->Height * texAddr->Width)) / 2),
                                      &TexBuff[TexBuffNum]);
            }
            texMap[AllTexBlockNum].TextureData = texAddr->TextureData;
            texMap[AllTexBlockNum].Offset = TexBuffNum;
            TexBuffNum += texAddr->Height * texAddr->Width;
            TexBuffNum = ((TexBuffNum / 8) * 8) + 8;
            AllTexBlockNum += 1;
        }
        texAddr++;
    }
}

void TexBuffLoadC(MenuTexture* Addr) {
    s32 i;
    s32 imgLoaded;
    MenuTexture* texAddr;
    TextureMap* texMap = &SMenuTextureMap[0];

    texAddr = GetTBPointer(Addr);
    while (texAddr->TextureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < AllTexBlockNum; i++) {
            if (texAddr->TextureData == (texMap + i)->TextureData) {
                imgLoaded = true;
                break;
            }
        }

        if (imgLoaded == false) {
            DMABase729A30(texAddr->TextureData, 0x00008000U, MeltBuff);
            decodeMIO0((u8*) MeltBuff, (u8*) &TexBuff[TexBuffNum]);
            texMap[AllTexBlockNum].TextureData = texAddr->TextureData;
            texMap[AllTexBlockNum].Offset = TexBuffNum;
            TexBuffNum += texAddr->Height * texAddr->Width;
            TexBuffNum = ((TexBuffNum / 8) * 8) + 8;
            AllTexBlockNum += 1;
        }
        texAddr++;
    }
}

void TexBuffLoadPw(MenuTexture* Addr) {
    TexBuffLoadP(Addr, LOAD_MENU_IMG_MIO0_FORCE);
}

void TexBuffLoadP(MenuTexture* Addr, s32 compType) {
    u16 Size;
    s32 i;
    s32 imgLoaded;
    u8 clearBit;
    MenuTexture* texAddr;
    TextureMap* texMap = &SMenuTextureMap[0];

    texAddr = GetTBPointer(Addr);
    while (texAddr->TextureData != NULL) {
        imgLoaded = false;
        for (i = 0; i < AllTexBlockNum; i++) {
            if (texAddr->TextureData == (texMap + i)->TextureData) {
                imgLoaded = true;
                break;
            }
        }

        if ((imgLoaded == false) || (compType > LOAD_MENU_IMG_FORCE)) {
            if (texAddr->Size != 0) {
                Size = texAddr->Size;
            } else {
                Size = 0x1000;
            }
            if (Size % 8) {
                Size = ((Size / 8) * 8) + 8;
            }
            switch (compType) {
                case LOAD_MENU_IMG_MIO0_ONCE:
                case LOAD_MENU_IMG_MIO0_FORCE:
                    DMABase729A30(texAddr->TextureData, Size, MeltBuff);
                    break;
                case LOAD_MENU_IMG_TKMK00_ONCE:
                case LOAD_MENU_IMG_TKMK00_FORCE:
                    TexDataDmaS(texAddr->TextureData, Size, MeltBuff);
                    break;
            }

            switch (compType) {
                case LOAD_MENU_IMG_MIO0_ONCE:
                case LOAD_MENU_IMG_MIO0_FORCE:
                    decodeMIO0((u8*) MeltBuff, (u8*) &TexBuff[TexBuffNum]);
                    break;
                case LOAD_MENU_IMG_TKMK00_ONCE:
                case LOAD_MENU_IMG_TKMK00_FORCE:
                    if (texAddr->Type == 1) {
                        clearBit = 0xBE;
                    } else {
                        clearBit = 1;
                    }
                    if (1) {}
                    decodeTKMK(MeltBuff, KWork,
                                 &TexBuff[TexBuffNum], clearBit);
                    break;
            }

            texMap[AllTexBlockNum].TextureData = texAddr->TextureData;
            texMap[AllTexBlockNum].Offset = TexBuffNum;
            TexBuffNum += texAddr->Height * texAddr->Width;
            TexBuffNum = ((TexBuffNum / 8) * 8) + 8;
            AllTexBlockNum += 1;
        }
        texAddr++;
    }
}

void TexBuffLoadVS(MenuTexture* Addr, s32 arg1, s32 Arg2) {
    u16 Size;
    MenuTexture* texAddr;

    texAddr = GetTBPointer(Addr);
    while (texAddr->TextureData != NULL) {
        if (texAddr->Size != 0) {
            Size = texAddr->Size;
        } else {
            Size = 0x1400;
        }
        if (Size % 8) {
            // Round up to the Next multiple of eight
            Size = ((Size / 8) * 8) + 8;
        }
        DMABase729A30(texAddr->TextureData, Size, MeltBuff);
        decodeMIO0((u8*) MeltBuff,
                   (u8*) D_802BFB80.arraySize4[Arg2][arg1 / 2][(arg1 % 2) + 2].PixelIndexArray);
        texAddr++;
    }
}

// Possibly a debug print function?
void InitChangeTexData(void) {
    s32 i;
    ChangeTexBuffBlock[0].Texture = NULL;
    for (i = 0; i < D_8018E060_SIZE; i++) {}
}

void SetChangeTexData(MenuTexture* arg0, s32 arg1) {
    struct_8018E060_entry* var_v1;

    var_v1 = &ChangeTexBuffBlock[0];
    while (var_v1->Texture != NULL) {
        var_v1++;
    }
    var_v1->Texture = GetTBPointer(arg0);
    var_v1->TexNum = arg1;
}

void DoChangeTexData(void) {
    s32 some_var;
    s8 texEnd;
    struct_8018E060_entry* var_s1;
    TextureMap* entry;
    MenuTexture* texPtr;
    OSIoMesg mb;
    OSMesg sp64;
    s32 cacheSize;
    s32 BufSize;

    if (GameState == RACING) {
        BufSize = 0x500;
    } else {
        BufSize = 0x1000;
    }

    texEnd = 0;
    entry = &SMenuTextureMap[0];
    var_s1 = &ChangeTexBuffBlock[0];
    texPtr = var_s1->Texture;

    if (texPtr == NULL) {
        return;
    }

    if (texPtr->Size) {
        cacheSize = texPtr->Size;
    } else {
        cacheSize = 0x1400;
    }
    if (cacheSize % 8) {
        cacheSize = ((cacheSize / 8) * 8) + 8;
    }

    osInvalDCache(MeltBuff, cacheSize);
    osPiStartDma(&mb, 0, 0, (uintptr_t) _textures_0aSegmentRomStart + SEGMENT_OFFSET(texPtr->TextureData),
                 MeltBuff, cacheSize, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &sp64, 1);

    while (1) {
        if ((var_s1 + 1)->Texture == NULL) {
            texEnd += 1;
        } else {
            texPtr = (var_s1 + 1)->Texture;
            if (texPtr->Size) {
                cacheSize = texPtr->Size;
            } else {
                cacheSize = 0x1400;
            }
            if (cacheSize % 8) {
                cacheSize = ((cacheSize / 8) * 8) + 8;
            }
            osInvalDCache(&MeltBuff[BufSize], cacheSize);
            osPiStartDma(&mb, 0, 0, (uintptr_t) _textures_0aSegmentRomStart + SEGMENT_OFFSET(texPtr->TextureData),
                         &MeltBuff[BufSize], cacheSize, &DmaMesgQueue);
        }

        some_var = (entry + var_s1->TexNum)->Offset;

        decodeMIO0((u8*) MeltBuff, (u8*) &TexBuff[some_var]);

        var_s1->Texture = NULL;
        var_s1++;
        if (texEnd) {
            break;
        }

        osRecvMesg(&DmaMesgQueue, &sp64, 1);

        if ((var_s1 + 1)->Texture == NULL) {
            texEnd += 1;
        } else {
            texPtr = (var_s1 + 1)->Texture;
            if (texPtr->Size) {
                cacheSize = texPtr->Size;
            } else {
                cacheSize = 0x1400;
            }
            if (cacheSize % 8) {
                cacheSize = ((cacheSize / 8) * 8) + 8;
            }
            osInvalDCache(MeltBuff, cacheSize);
            osPiStartDma(&mb, 0, 0, (uintptr_t) _textures_0aSegmentRomStart + SEGMENT_OFFSET(texPtr->TextureData),
                         MeltBuff, cacheSize, &DmaMesgQueue);
        }

        some_var = (entry + var_s1->TexNum)->Offset;
        decodeMIO0((u8*) &MeltBuff[BufSize], (u8*) &TexBuff[some_var]);
        var_s1->Texture = NULL;
        var_s1++;
        if (texEnd) {
            break;
        }
        osRecvMesg(&DmaMesgQueue, &sp64, 1);
    }
}

void InitChangeTexDataVS(void) {
    ChangeTexBuffBlockVS[0].Mk64Texture = NULL;
}

void SetChangeTexDataVS(MenuTexture* arg0, s32 arg1, s32 Arg2) {
    struct_8018E0E8_entry* var_v1;

    var_v1 = ChangeTexBuffBlockVS;
    while (var_v1->Mk64Texture != NULL) {
        var_v1++;
    }
    var_v1->Mk64Texture = GetTBPointer(arg0);
    var_v1->unk4 = arg1;
    var_v1->unk6 = Arg2;
}

void DoChangeTexDataVS(void) {
    s8 var_s4;
    s32 var_s0;
    UNUSED s32 pad[2];
    OSIoMesg sp68;
    OSMesg sp64;
    s32 huh;
    MenuTexture* temp_s2;
    struct_8018E0E8_entry* var_s1;

    var_s4 = 0;
    var_s1 = ChangeTexBuffBlockVS;
    temp_s2 = var_s1->Mk64Texture;

    if (temp_s2 == NULL)
        return;

    huh = temp_s2->Size;
    if (huh != 0) {
        var_s0 = huh;
    } else {
        var_s0 = 0x1400;
    }
    if (var_s0 % 8) {
        var_s0 = ((var_s0 / 8) * 8) + 8;
    }
    osInvalDCache((void*) MeltBuff, var_s0);
    osPiStartDma(&sp68, 0, 0, (u32) _textures_0aSegmentRomStart + SEGMENT_OFFSET(temp_s2->TextureData),
                 MeltBuff, var_s0, &DmaMesgQueue);
    if ((var_s0 && var_s0) && var_s0) {}
    osRecvMesg(&DmaMesgQueue, &sp64, 1);
    while (1) {
        if ((var_s1 + 1)->Mk64Texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->Mk64Texture;
            huh = (var_s1 + 1)->Mk64Texture->Size;
            if (huh != 0) {
                var_s0 = huh;
            } else {
                var_s0 = 0x1400;
            }
            if (var_s0 % 8) {
                var_s0 = ((var_s0 / 8) * 8) + 8;
            }
            osInvalDCache(MeltBuff + 0x500, var_s0);
            osPiStartDma(&sp68, 0, 0, (u32) _textures_0aSegmentRomStart + SEGMENT_OFFSET(temp_s2->TextureData),
                         MeltBuff + 0x500, var_s0, &DmaMesgQueue);
        }
        decodeMIO0((u8*) MeltBuff,
                   D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].PixelIndexArray);
        var_s1->Mk64Texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&DmaMesgQueue, &sp64, 1);
        if ((var_s1 + 1)->Mk64Texture == NULL) {
            var_s4 += 1;
        } else {
            temp_s2 = (var_s1 + 1)->Mk64Texture;
            huh = (var_s1 + 1)->Mk64Texture->Size;
            if (huh != 0) {
                var_s0 = huh;
            } else {
                var_s0 = 0x1400;
            }
            if (var_s0 % 8) {
                var_s0 = ((var_s0 / 8) * 8) + 8;
            }
            osInvalDCache(MeltBuff, var_s0);
            osPiStartDma(&sp68, 0, 0, (u32) _textures_0aSegmentRomStart + SEGMENT_OFFSET(temp_s2->TextureData),
                         MeltBuff, var_s0, &DmaMesgQueue);
        }
        decodeMIO0((u8*) (MeltBuff + 0x500),
                   D_802BFB80.arraySize4[var_s1->unk6][var_s1->unk4 / 2][(var_s1->unk4 % 2) + 2].PixelIndexArray);
        var_s1->Mk64Texture = NULL;
        var_s1++;
        if (var_s4 != 0)
            break;
        osRecvMesg(&DmaMesgQueue, &sp64, 1);
    }
}

void ChangeTexData(MenuTexture* arg0, s32 arg1) {
    s32 var_a3;
    s32 temp_v1;
    u64* sp24;
    UNUSED TextureMap* temp_v0;

    temp_v1 = SMenuTextureMap[arg1].Offset;
    sp24 = arg0->TextureData;
    var_a3 = arg0->Size;
    if (var_a3 % 8) {
        var_a3 = ((var_a3 / 8) * 8) + 8;
    }
    TexDataDmaS(sp24, var_a3, MeltBuff);
    decodeTKMK(MeltBuff, KWork, &TexBuff[temp_v1], 1);
    SMenuTextureMap[arg1].TextureData = sp24;
}

void func_8009A2F0(struct_8018E0E8_entry* arg0) {
    MenuTexture* var_a0;
    struct_8018E0E8_entry* temp_v0;

    temp_v0 = GetVAddress(arg0);
    var_a0 = temp_v0->Mk64Texture;
    while (var_a0 != NULL) {
        if (var_a0 == NULL) {
            break;
        }
        TexBuffLoadP(var_a0, LOAD_MENU_IMG_TKMK00_ONCE);
        if (1) {}
        temp_v0++;
        var_a0 = temp_v0->Mk64Texture;
    }
}

void InitSeqData(void) {
    s32 Index;
    for (Index = 0; Index < D_8018DEE0_SIZE; Index++) {
        SEQDataBlock[Index].Visible = 0;
    }
}

s32 SetSeqData(MkAnimation* anim) {
    s32 i;
    struct_8018DEE0_entry* entry;

    anim = GetVAddress(anim);
    i = 0;
    while (SEQDataBlock[i].Visible) {
        i++;
        if (i >= 0x10) {
            // No more space.
            while (1) {
                ;
            }
        }
    }

    entry = &SEQDataBlock[i];
    entry->TextureSequence = anim;
    entry->SequenceIndex = -1;
    entry->FrameCountDown = 0;
    entry->Visible = 0x80000000;
    entry->MenuTextureIndex = AllTexBlockNum;

    if (anim[0].Mk64Texture) {
        TexBuffLoadPw(anim[0].Mk64Texture);
    }
    if (anim[1].Mk64Texture) {
        TexBuffLoadPw(anim[1].Mk64Texture);
    } else {
        TexBuffLoadPw(anim[0].Mk64Texture);
    }

    entry->unk14 = 0;
    return i;
}

s32 SetSeqData2(MkAnimation* anim, s32 arg1) {
    s32 i;
    struct_8018DEE0_entry* entry;

    anim = GetVAddress(anim);
    i = 0;
    while (SEQDataBlock[i].Visible) {
        i++;
        if (i >= 0x10) {
            // No more space.
            while (1) {
                ;
            }
        }
    }

    entry = &SEQDataBlock[i];
    entry->TextureSequence = anim;
    entry->SequenceIndex = -1;
    entry->FrameCountDown = 0;
    entry->Visible = 0x80000000;
    entry->MenuTextureIndex = AllTexBlockNum;
    if (anim[0].Mk64Texture) {
        TexBuffLoadVS(anim[0].Mk64Texture, arg1, 0);
    }
    if (anim[1].Mk64Texture) {
        TexBuffLoadVS(anim[1].Mk64Texture, arg1, 1);
    } else {
        TexBuffLoadVS(anim[0].Mk64Texture, arg1, 1);
    }
    entry->unk14 = 0;
    return i;
}

void ChangeSeqData(s32 arg0, s32 arg1, MkAnimation* Arg2) {
    MkAnimation* temp_v0;
    MenuTexture* temp_a0;

    temp_v0 = GetVAddress(Arg2);
    SEQDataBlock[arg0].TextureSequence = temp_v0;
    SEQDataBlock[arg0].SequenceIndex = arg1;
    // All hail the fake match gods who, in their infinite grace, have blessed us
    // with this enigma of a match on the first iteration of permutation
    SEQDataBlock[arg0].FrameCountDown = (temp_v0 + arg1)->FrameLength;
    temp_a0 = GetTBPointer(temp_v0[arg1].Mk64Texture);
    if (SEQDataBlock[arg0].unk14 != 0) {
        SetChangeTexData(temp_a0, SEQDataBlock[arg0].MenuTextureIndex);
        SEQDataBlock[arg0].unk14 = 0;
    } else {
        SetChangeTexData(temp_a0, SEQDataBlock[arg0].MenuTextureIndex + 1);
        SEQDataBlock[arg0].unk14 = 1;
    }
}

void ChangeSeqDataVS(s32 arg0, s32 arg1, s32 Arg2, MkAnimation* Arg3) {
    MkAnimation* temp_v0;
    MenuTexture* temp_a0;

    temp_v0 = GetVAddress(Arg3);
    SEQDataBlock[arg0].TextureSequence = temp_v0;
    SEQDataBlock[arg0].SequenceIndex = arg1;
    SEQDataBlock[arg0].FrameCountDown = (temp_v0 + arg1)->FrameLength;
    temp_a0 = GetTBPointer(temp_v0[arg1].Mk64Texture);
    SEQDataBlock[arg0].unk14 ^= 1;
    SetChangeTexDataVS(temp_a0, Arg2, SEQDataBlock[arg0].unk14);
}

UNUSED void DoSeqData(void) {
    s32 Index;
    for (Index = 0; Index < D_8018DEE0_SIZE; Index++) {
        if ((SEQDataBlock[Index].Visible & 0x80000000) != 0) {
            SelectTex(&SEQDataBlock[Index]);
            DisplayListHead = DoTexListW(DisplayListHead, &SEQDataBlock[Index], 0, 0, 0);
        }
    }
    DoChangeTexData();
}

void DoSeqData2(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3) {
    struct_8018DEE0_entry* temp = &SEQDataBlock[arg0];
    if (temp->Visible & 0x80000000) {
        SelectTex(temp);
        DisplayListHead = DoTexListW(DisplayListHead, temp, arg1, Arg2, Arg3);
    }
}

void DoSeqData2VS(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    struct_8018DEE0_entry* temp = &SEQDataBlock[arg0];
    if (temp->Visible & 0x80000000) {
        SelectTexVS(temp, Arg3);
        DisplayListHead = DoTexListVS(DisplayListHead, temp, arg1, Arg2, Arg3, arg4);
    }
}

MenuTexture* SelectTex(struct_8018DEE0_entry* arg0) {
    MkAnimation* temp_v1;
    MkAnimation* var_v0;
    MkAnimation* test;
    MenuTexture* temp_a0;

    temp_v1 = arg0->TextureSequence;
    if (arg0->SequenceIndex < 0) {
        arg0->SequenceIndex = 0;
        arg0->FrameCountDown = 0;
    }
    arg0->FrameCountDown--;
    if (arg0->FrameCountDown <= 0) {
        arg0->SequenceIndex++;
        // Again, hail the fake match gods
        var_v0 = ((test = temp_v1) + arg0->SequenceIndex);
        if (var_v0->Mk64Texture == NULL) {
            arg0->SequenceIndex = 0;
        }
        var_v0 = (test + arg0->SequenceIndex);
        arg0->FrameCountDown = var_v0->FrameLength;
        temp_a0 = GetTBPointer(var_v0->Mk64Texture);
        if (arg0->unk14 != 0) {
            SetChangeTexData(temp_a0, arg0->MenuTextureIndex);
            arg0->unk14 = 0;
        } else {
            SetChangeTexData(temp_a0, arg0->MenuTextureIndex + 1);
            arg0->unk14 = 1;
        }
    }
    return arg0->TextureSequence[arg0->SequenceIndex].Mk64Texture;
}

MenuTexture* SelectTexVS(struct_8018DEE0_entry* arg0, s32 arg1) {
    MkAnimation* temp_v1;
    MkAnimation* var_v0;
    MkAnimation* test;
    MenuTexture* temp_a0;

    temp_v1 = arg0->TextureSequence;
    if (arg0->SequenceIndex < 0) {
        arg0->SequenceIndex = 0;
        arg0->FrameCountDown = 0;
    }
    arg0->FrameCountDown--;
    if (arg0->FrameCountDown <= 0) {
        arg0->SequenceIndex++;
        var_v0 = ((test = temp_v1) + arg0->SequenceIndex);
        if (var_v0->Mk64Texture == NULL) {
            arg0->SequenceIndex = 0;
        }
        var_v0 = (test + arg0->SequenceIndex);
        arg0->FrameCountDown = var_v0->FrameLength;
        temp_a0 = GetTBPointer(var_v0->Mk64Texture);
        arg0->unk14 ^= 1;
        SetChangeTexDataVS(temp_a0, arg1, arg0->unk14);
    }
    return arg0->TextureSequence[arg0->SequenceIndex].Mk64Texture;
}

void GrayScaleTexBuff256(s32 arg0, s32 arg1, u32 Arg2, s32 Arg3) {
    s32 red;
    s32 Green;
    s32 Blue;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    s32 Alpha;
    s32 temp_t9;
    u16 temp_a0;
    s32 var_t1;
    u16* color0;
    u16* color1;

    color0 = &TexBuff[SMenuTextureMap[arg0].Offset];
    color1 = &TexBuff[SMenuTextureMap[arg1].Offset];
    for (var_t1 = 0; (u32) var_t1 < Arg2; var_t1++) {
        temp_a0 = *color0++;
        red = (temp_a0 & 0xF800) >> 0xB;
        Green = (temp_a0 & 0x7C0) >> 6;
        Blue = (temp_a0 & 0x3E) >> 1;
        Alpha = temp_a0 & 0x1;
        if (Alpha) {}
        temp_t9 = ((red * 0x4D) + (Green * 0x96) + (Blue * 0x1D)) >> 8;
        newred = (((((temp_t9 - red) * Arg3) >> 8) + red) << 0xB);
        newgreen = (((((((temp_t9 * 7) / 8) - Green) * Arg3) >> 8) + Green) << 6);
        newblue = (((((((temp_t9 * 6) / 8) - Blue) * Arg3) >> 8) + Blue) << 1);
        *color1++ = newblue + newgreen + newred + Alpha;
    }
}

void GrayScaleTexBuff(s32 arg0) {
    s32 red;
    s32 Green;
    s32 Blue;
    s32 Alpha;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    u16* color;

    color = &TexBuff[SMenuTextureMap[arg0].Offset];
    for (var_v1 = 0; var_v1 < 0x4B000; var_v1++) {
        red = ((*color & 0xF800) >> 0xB) * 0x4D;
        Green = ((*color & 0x7C0) >> 6) * 0x96;
        Blue = ((*color & 0x3E) >> 1) * 0x1D;
        Alpha = *color & 0x1;
        temp_t9 = red + Green + Blue;
        temp_t9 >>= 8;
        newred = temp_t9 << 0xB;
        newgreen = temp_t9 << 6;
        newblue = temp_t9 << 1;
        *color++ = newblue + newgreen + newred + Alpha;
    }
}

void GrayScaleTexBuff2(s32 arg0, s32 arg1) {
    s32 red;
    s32 Green;
    s32 Blue;
    s32 Alpha;
    UNUSED s32 newred;
    UNUSED s32 newgreen;
    UNUSED s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    s32 Size;
    UNUSED u16 temp_a0;
    u16* color;

    color = &TexBuff[SMenuTextureMap[arg0].Offset];
    Size = SMenuTextureMap[arg0 + 1].Offset - SMenuTextureMap[arg0].Offset;
    for (var_v1 = 0; var_v1 != Size; var_v1++) {
        red = ((*color & 0xF800) >> 0xB) * 0x4D;
        Green = ((*color & 0x7C0) >> 6) * 0x96;
        Blue = ((*color & 0x3E) >> 1) * 0x1D;
        Alpha = *color & 0x1;
        temp_t9 = red + Green + Blue;
        temp_t9 = temp_t9 >> 8;
        temp_t9 += ((0x20 - temp_t9) * arg1) >> 8;
        *color++ = (temp_t9 << 1) + (temp_t9 << 6) + (temp_t9 << 0xB) + Alpha;
    }
}

void GrayScaleTexBuf3(s32 arg0, u32 arg1) {
    u32 i;
    s32 red;
    s32 Green;
    s32 Blue;
    s32 Alpha;
    u32 temp_t9;
    s32 Size;
    u16* color;
    f32 sp48[32];

    for (i = 0; i < 32; i++) {
        sp48[i] = Power(i / 32.0, (arg1 * 1.5 / 256.0) + 0.25);
    }
    color = &TexBuff[SMenuTextureMap[arg0].Offset];
    Size = SMenuTextureMap[arg0 + 1].Offset - SMenuTextureMap[arg0].Offset;
    for (i = 0; i < (u32) Size; i++) {
        red = ((*color & 0xF800) >> 0xB) * 0x55;
        Green = ((*color & 0x7C0) >> 6) * 0x4B;
        Blue = ((*color & 0x3E) >> 1) * 0x5F;
        Alpha = *color & 0x1;
        temp_t9 = red + Green + Blue;
        temp_t9 /= 256;
        temp_t9 = sp48[temp_t9] * 32.0f;
        if (temp_t9 >= 32) {
            temp_t9 = 31;
        }
        *color++ = (temp_t9 << 1) + (temp_t9 << 6) + (temp_t9 << 0xB) + Alpha;
    }
}

void GrayScaleTexBufRGB(s32 arg0, s32 arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    s32 red;
    s32 Green;
    s32 Blue;
    s32 Alpha;
    s32 newred;
    s32 newgreen;
    s32 newblue;
    u32 temp_t9;
    s32 var_v1;
    u16* color;

    color = &TexBuff[SMenuTextureMap[arg0].Offset];
    for (var_v1 = 0; var_v1 != arg1; var_v1++) {
        red = ((*color & 0xF800) >> 0xB) * 0x4D;
        Green = ((*color & 0x7C0) >> 6) * 0x96;
        Blue = ((*color & 0x3E) >> 1) * 0x1D;
        Alpha = *color & 0x1;
        temp_t9 = red + Green + Blue;
        temp_t9 = temp_t9 / 256;
        newred = ((temp_t9 * Arg2) / 256) << 0xB;
        newgreen = ((temp_t9 * Arg3) / 256) << 6;
        newblue = ((temp_t9 * arg4) / 256) << 1;
        *color++ = newred + newgreen + newblue + Alpha;
    }
}

u16* GetTexBuffPointer(u64* arg0) {
    UNUSED s32 pad[2];
    s32 Offset;
    s32 found;
    s32 someIndex;

    found = 0;
    for (someIndex = 0; someIndex < AllTexBlockNum; someIndex++) {
        if (arg0 == SMenuTextureMap[someIndex].TextureData) {
            found = 1;
            Offset = SMenuTextureMap[someIndex].Offset;
            break;
        }
    }

    if (found != 0) {
        return &TexBuff[Offset];
    }
    return NULL;
}

// KdlBuff is a little weird. In code_800AF9B0 its treated as a
// struct_8018EE10_entry Pointer. But here its being treated as a
// Gfx Pointer. It seems to be multi use.
void InitKDL(void) {
    KdlBuffPtr = (Gfx*) KdlBuff;
    AllKdlBlockNum = 0;
}

void KDLMaker1(MenuTexture* arg0) {
    KdlBuffBlock[AllKdlBlockNum].Textures = GetTBPointer(arg0);
    KdlBuffBlock[AllKdlBlockNum].DisplayList = KdlBuffPtr;
}

void KDLMaker2(void) {
    gSPEndDisplayList(KdlBuffPtr++);
    AllKdlBlockNum += 1;
}

// I don't get how this compiles, given that there's a Code path
// that has no explicit return Value.
// Based on the Target assembly, in the event that it never finds
// the desired entry, it'll treat the return Value of GetTBPointer
// as the return of this function. Which seems like a bug to me
Gfx* KDLCall(Gfx* displayListHead, MenuTexture* Textures) {
    Gfx* DisplayList;
    UNUSED s32 pad;
    bool found;
    s32 Index;

    found = false;
    for (Index = 0; Index < D_8018E768_SIZE; Index++) {
        if (KdlBuffBlock[Index].Textures == GetTBPointer(Textures)) {
            DisplayList = KdlBuffBlock[Index].DisplayList;
            found = true;
            break;
        }
    }
    if (found) {
        gSPDisplayList(displayListHead++, DisplayList);
        return displayListHead;
    }
}

Gfx* DoTexList(Gfx* arg0, MenuTexture* arg1, s32 column, s32 row) {
    MenuTexture* temp_v0;
    u8* temp_v0_3;
    s8 var_s4;

    temp_v0 = GetTBPointer(arg1);
    while (temp_v0->TextureData != NULL) {
        var_s4 = 0;
        switch (temp_v0->Type) {
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
        temp_v0_3 = (u8*) GetTexBuffPointer(temp_v0->TextureData);
        if (temp_v0_3 != 0) {
            if (FadingFlag[4] != 4) {
                arg0 =
                    AutoTextureRect(arg0, var_s4, 0x00000400, 0x00000400, 0, 0, temp_v0->Width, temp_v0->Height,
                                  temp_v0->dX + column, temp_v0->dY + row, temp_v0_3, temp_v0->Width, temp_v0->Height);
            } else {
                arg0 = AutoTextureRectR(arg0, 0U, 0U, temp_v0->Width, temp_v0->Height, temp_v0->dX + column,
                                     temp_v0->dY + row, temp_v0_3, temp_v0->Width, temp_v0->Height);
            }
        }
        temp_v0++;
    }
    return arg0;
}

Gfx* DoTexList2(Gfx* arg0, MenuTexture* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5) {
    MenuTexture* var_s0;
    u8* temp_v0_3;

    var_s0 = GetTBPointer(arg1);
    while (var_s0->TextureData != NULL) {
        switch (var_s0->Type) { /* irregular */
            case 0:
                gSPDisplayList(arg0++, D_02007708);
                break;
            case 1:
                gSPDisplayList(arg0++, D_02007728);
                break;
        }
        temp_v0_3 = (u8*) GetTexBuffPointer(var_s0->TextureData);
        if (temp_v0_3 != 0) {
            switch (arg4) {
                case 1:
                    arg0 = AutoTextureRect64(arg0, 0, var_s0->dX + Arg2, var_s0->dY + Arg3, temp_v0_3, arg5);
                    break;
                case 2:
                    arg0 = AutoTextureRectW(arg0, 0, 0, 0U, var_s0->Width, var_s0->Height, var_s0->dX + Arg2,
                                         var_s0->dY + Arg3, temp_v0_3, var_s0->Width, var_s0->Height, arg5);
                    break;
                case 3:
                    arg0 = AutoTextureRectC(arg0, 0, 0, 0, var_s0->Width, var_s0->Height, var_s0->dX + Arg2,
                                         var_s0->dY + Arg3, temp_v0_3, var_s0->Width, var_s0->Height);
                    break;
                case 4:
                    arg0 = AutoTextureRect2Cyc(arg0, 0, 0x00000400, 0x00000400, 0, 0, var_s0->Width, var_s0->Height,
                                         var_s0->dX + Arg2, var_s0->dY + Arg3, (u16*) temp_v0_3, var_s0->Width,
                                         var_s0->Height, arg5);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}

Gfx* DoTexList4(Gfx* arg0, MenuTexture* glyphTexture, f32 Arg2, f32 Arg3, s32 Mode, f32 scaleX, f32 scaleY) {
    s32 var_v0;
    u8* temp_v0_2;
    f32 Thing0;
    f32 Thing1;
    MenuTexture* var_s0;

    var_s0 = GetTBPointer(glyphTexture);
    while (var_s0->TextureData != NULL) {
        var_v0 = 0;

        Thing0 = var_s0->dX + Arg2;
        if (Thing0 > 320.0f) {
            var_v0 = 1;
        }
        Thing0 += var_s0->Width * scaleX;
        if (Thing0 < 0.0f) {
            var_v0 += 1;
        }
        Thing1 = var_s0->dY + Arg3;
        if (Thing1 < 0.0f) {
            var_v0 += 1;
        }
        Thing1 -= var_s0->Height * scaleY;
        if (Thing1 > 240.0f) {
            var_v0 += 1;
        }

        if (var_v0 != 0) {
            var_s0++;
        } else {
            temp_v0_2 = (u8*) GetTexBuffPointer(var_s0->TextureData);
            if (temp_v0_2 != 0) {
                switch (Mode) { /* irregular */
                    case 1:
                        gSPDisplayList(arg0++, D_020077F8);
                        arg0 = SetIAFont(arg0, temp_v0_2, var_s0->dX + Arg2, var_s0->dY + Arg3, var_s0->Width,
                                             var_s0->Height, scaleX, scaleY);
                        break;
                    case 2:
                        gSPDisplayList(arg0++, D_02007818);
                        arg0 = SetIAFont(arg0, temp_v0_2, var_s0->dX + Arg2, var_s0->dY + Arg3, var_s0->Width,
                                             var_s0->Height, scaleX, scaleY);
                        break;
                }
            }
            var_s0++;
        }
    }
    return arg0;
}

Gfx* DoTexList5(Gfx* arg0, MenuTexture* arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    s32 var_s2;
    u8* temp_t0;
    MenuTexture* var_s1;

    var_s1 = GetTBPointer(arg1);
    while (var_s1->TextureData != NULL) {
        var_s2 = 0;
        switch (var_s1->Type) {
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
        temp_t0 = (u8*) GetTexBuffPointer(var_s1->TextureData);
        switch (arg4) {
            case 2:
                arg0 =
                    AutoTextureRectZoom(arg0, var_s2, 0x00000400, 0x00000400, 0.5f, 0.5f, 0, 0, var_s1->Width, var_s1->Height,
                                  var_s1->dX + Arg2, var_s1->dY + Arg3, temp_t0, var_s1->Width, var_s1->Height);
                break;
            case 3:
                arg0 = AutoTextureRectZoom(arg0, var_s2, 0x00000400, 0x00000400, 0.457f, 0.5f, 0, 0, var_s1->Width,
                                     var_s1->Height, var_s1->dX + Arg2, var_s1->dY + Arg3, temp_t0, var_s1->Width,
                                     var_s1->Height);
                break;
        }
        var_s1++;
    }
    return arg0;
}

Gfx* DoTexListW(Gfx* arg0, struct_8018DEE0_entry* arg1, s32 Arg2, s32 Arg3, s32 arg4) {
    s32 var_t0;
    s32 var_t1;
    Gfx* temp;
    MenuTexture* var_s0;

    var_s0 = GetTBPointer(arg1->TextureSequence[arg1->SequenceIndex].Mk64Texture);
    temp = D_02007728;
    while (var_s0->TextureData != NULL) {
        var_t1 = 0;
        switch (var_s0->Type) { /* irregular */
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
            var_t0 = SMenuTextureMap[arg1->MenuTextureIndex + 1].Offset;
        } else {
            var_t0 = SMenuTextureMap[arg1->MenuTextureIndex].Offset;
            if (1) {}
            if (1) {}
            if (1) {}
        }
        if (arg4 >= 0) {
            arg0 =
                AutoTextureRectW(arg0, var_t1, 0, 0U, var_s0->Width, var_s0->Height, var_s0->dX + Arg2, var_s0->dY + Arg3,
                              (u8*) &TexBuff[var_t0], var_s0->Width, var_s0->Height, (u32) arg4);
        } else {
            switch (arg4) {
                case -1:
                    arg0 = AutoTextureRect(arg0, var_t1, 0x00000400, 0x00000400, 0, 0, var_s0->Width, var_s0->Height,
                                         var_s0->dX + Arg2, var_s0->dY + Arg3, (u8*) &TexBuff[var_t0],
                                         var_s0->Width, var_s0->Height);
                    break;
                case -2:
                    arg0 = AutoTextureRectZoom(arg0, var_t1, 0x00000400, 0x00000400, 0.5f, 0.5f, 0, 0, var_s0->Width,
                                         var_s0->Height, var_s0->dX + Arg2, var_s0->dY + Arg3,
                                         (u8*) &TexBuff[var_t0], var_s0->Width, var_s0->Height);
                    break;
            }
        }
        var_s0++;
    }
    return arg0;
}

Gfx* DoTexListVS(Gfx* arg0, struct_8018DEE0_entry* arg1, s32 Arg2, s32 Arg3, s32 arg4, s32 arg5) {
    s32 var_t0;
    UNUSED s32 thing;
    Gfx* temp;
    MenuTexture* var_s1;

    var_s1 = GetTBPointer(arg1->TextureSequence[arg1->SequenceIndex].Mk64Texture);
    temp = D_02007728;
    while (var_s1->TextureData != NULL) {
        var_t0 = 0;
        switch (var_s1->Type) { /* irregular */
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
                AutoTextureRectW(arg0, var_t0, 0, 0U, var_s1->Width, var_s1->Height, var_s1->dX + Arg2, var_s1->dY + Arg3,
                              D_802BFB80.arraySize4[arg1->unk14][arg4 / 2][(arg4 % 2) + 2].PixelIndexArray,
                              var_s1->Width, var_s1->Height, (u32) arg5);
        }
        var_s1++;
    }
    return arg0;
}

void BeforeFadeMain(void) {
    s32 someIndex;

    for (someIndex = 0; someIndex < 4; someIndex++) {
        OldScreenPosition[someIndex].x = D_8015F480[someIndex].Posx;
        OldScreenPosition[someIndex].y = D_8015F480[someIndex].Posy;
        OldScreenSize[someIndex].x = D_8015F480[someIndex].Width;
        OldScreenSize[someIndex].y = D_8015F480[someIndex].Height;
    }

    OldScreenPosition[4].x = 0x00A0;
    OldScreenPosition[4].y = 0x0078;
    OldScreenSize[4].x = 0x0140;
    OldScreenSize[4].y = 0x00F0;
}

void FadeMain(void) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        FadeMain2(var_s0);
    }
}

void FadeMain2(s32 arg0) {
    s32 var_a1;

    if ((arg0 == 4) || ((GetOBJBlockPtr2(0x000000AA) == NULL) && (GetOBJBlockPtr2(0x000000AB) == NULL) &&
                        (GetOBJBlockPtr2(0x000000B9) == NULL) && (GetOBJBlockPtr2(0x000000BA) == NULL) &&
                        (GetOBJBlockPtr2(0x000000AC) == NULL) && (GetOBJBlockPtr2(0x000000B0) == NULL))) {
        var_a1 = 0;
        gSPDisplayList(DisplayListHead++, D_0D0076F8);
        if ((arg0 != 4) && (PauseFlag != 0)) {
            var_a1 = 1;
        }
        switch (FadingFlag[arg0]) {
            case 1:
                FadeIn(arg0, var_a1);
                return;
            case 2:
                FadeOut(arg0, var_a1);
                return;
            case 3:
                FadeOutM();
                return;
            case 4:
                FadeOutR();
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

void FadeInEx(s32 arg0, s32 arg1, s32 Arg2) {
    RGBA16* color;
    s16 x, y, w, h;
    UNUSED s32 pad[3];
    Screen* unk;
    struct UnkStruct_8018E7E8 *Size, *Start;

    if ((GameMode == GRAND_PRIX) || (GameMode == TIME_TRIALS)) {
        Start = &(OldScreenPosition[arg0]);
        Size = &(OldScreenSize[arg0]);
        x = Start->x;
        y = Start->y;
        w = Size->x;
        h = Size->y;
    } else if (arg0 >= 4) {
        Start = &(OldScreenPosition[arg0]);
        Size = &(OldScreenSize[arg0]);
        x = Start->x;
        y = Start->y;
        w = Size->x;
        h = Size->y;
    } else {
        unk = &D_8015F480[arg0];
        x = unk->Posx;
        y = unk->Posy;
        w = unk->Width;
        h = unk->Height;
    }
    color = &FadeColorBw[Arg2];
    DisplayListHead =
        FillRect1ColorF(DisplayListHead, x - (w / 2), y - (h / 2), (w / 2) + x, (h / 2) + y, color->red, color->Green,
                 color->Blue, 0xFF - (FadeCounter2[arg0] * 0xFF / FadeCounter[arg0]));

    if ((arg1 == 0) &&
        (FadeCounter2[arg0] += 1, (FadeCounter2[arg0] >= FadeCounter[arg0]))) {
        if (GameState == RACING) {
            FadingFlag[arg0] = 6;
            return;
        }
        FadingFlag[arg0] = 0;
        FBClearFlg = 0;
    }
}

void FadeIn(s32 arg0, s32 arg1) {
    FadeInEx(arg0, arg1, 0);
}

void FadeInWhite(s32 arg0, s32 arg1) {
    FadeInEx(arg0, arg1, 1);
}

void SettingSoundOutMode4Multiplayer(void) {
    if ((SoundOutput != 3) && (PlayerCount >= 2)) {
        NaiSeqBufFlagCheck(0xE0000002);
    }
}

void EndFadeOut(s32 arg0) {
    s32 thing;
    s32 var_a1;
    UNUSED s32 stackPadding0;
    ObjBlock* temp_v0;

    var_a1 = 0;
    if (GameState == 5) {
        if (2 != RaceClass) {
            thing = RaceClass;
            if (thing != 3) {
                goto func_8009CE64_label1;
            }
            goto func_8009CE64_label2;
        }
    func_8009CE64_label2:
        if ((Resultstat.Rank < 3) && (CupSelect == 3)) {
            var_a1 = 1;
        }
    func_8009CE64_label1:
        if (var_a1) {
            GotoMenu = 9;
            CreditsCourseId = 8;
        } else {
            GotoMenu = 1;
            KBGNumber = 0x0000000B;
        }
    } else if (GameState == RACING) {
        if (FadingFlag[arg0] == 2) {
            if (arg0 != 4) {
                FadingFlag[arg0] = 5;
            } else {
                var_a1 = 0;
                temp_v0 = GetOBJBlockPtr2(0x000000B0);
                if (temp_v0 != NULL) {
                    switch (temp_v0->Pattern) { /* switch 8; irregular */
                        case 10:              /* switch 8 */
                            GotoRacing();
                            break;
                        case 11: /* switch 8 */
                            GotoMapselect();
                            break;
                        case 12: /* switch 8 */
                            GoToGameSelect();
                            break;
                        default: /* switch 8 */
                        case 13: /* switch 8 */
                            GotoGameselect();
                            break;
                    }
                } else {
                    var_a1 = 0;
                    temp_v0 = GetOBJBlockPtr2(0x000000AC);
                    if (temp_v0 != NULL) {
                        switch (temp_v0->Pattern) { /* switch 7; irregular */
                            case 11:              /* switch 7 */
                                GotoRacing();
                                D_8016556E = 1;
                                break;
                            default: /* switch 7 */
                            case 12: /* switch 7 */
                                GotoGameselect();
                                break;
                        }
                    } else {
                        var_a1 = 0;
                        temp_v0 = GetOBJBlockPtr2(0x000000C7);
                        if (temp_v0 != NULL) {
                            switch (temp_v0->Pattern) {
                                case 12:
                                    GotoRacing();
                                    var_a1 = 1;
                                    break;
                                case 13:
                                case 22:
                                case 42:
                                    GotoMapselect();
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
                                    GotoGameselect();
                                    var_a1 = 1;
                                    break;
                                default:
                                    FadingFlag[arg0] = 5;
                                    break;
                            }
                            if (var_a1 != 0) {
                                PauseFlag = 0;
                            }
                        }
                        temp_v0 = GetOBJBlockPtr2(0x000000BD);
                        if (temp_v0 != NULL) {
                            switch (temp_v0->Pattern) { /* switch 2 */
                                case 11:              /* switch 2 */
                                    RetryFlag = 1;
                                    ReplayFlag = 0;
                                    GotoRacing();
                                    break;
                                case 12: /* switch 2 */
                                    RetryFlag = 0;
                                    ReplayFlag = 0;
                                    GotoMapselect();
                                    break;
                                case 13: /* switch 2 */
                                    RetryFlag = 0;
                                    ReplayFlag = 0;
                                    GoToGameSelect();
                                    break;
                                case 14: /* switch 2 */
                                    RetryFlag = 0;
                                    ReplayFlag = 0;
                                    GotoGameselect();
                                    break;
                                case 15: /* switch 2 */
                                    RetryFlag = 0;
                                    ReplayFlag = 1;
                                    GotoRacing();
                                    break;
                                default: /* switch 2 */
                                    break;
                            }
                        } else {
                            FadingFlag[arg0] = 5;
                        }
                    }
                }
            }
        }
    } else {
        FadingFlag[arg0] = 0;
        if (TitleMode != DEBUG_MENU_OPTION_SELECTED) {
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
                    DemoFlag = 1;
                    DemoPlayFlag = 1;
                    NewSequenceMode = 4;
                    RaceClass = (s32) 1;
                    switch (NextDemoId) { /* switch 4 */
                        case 0:            /* switch 4 */
                            CourseID = COURSE_MARIO_RACEWAY;
                            ScreenSplitB = 0;
                            PlayerCount = 1;
                            MenuMultiplayerSelection = 1;
                            CharacterSelections[0] = 0;
                            GameMode = 0;
                            break;
                        case 1: /* switch 4 */
                            CourseID = COURSE_CHOCO_MOUNTAIN;
                            ScreenSplitB = (s32) 1;
                            PlayerCount = 2;
                            MenuMultiplayerSelection = (s8) 2;
                            CharacterSelections[0] = (s8) 2;
                            CharacterSelections[1] = (s8) 4;
                            GameMode = 2;
                            break;
                        case 2: /* switch 4 */
                            CourseID = COURSE_KALAMARI_DESERT;
                            ScreenSplitB = 0;
                            PlayerCount = (s32) 1;
                            MenuMultiplayerSelection = 1;
                            CharacterSelections[0] = 1;
                            GameMode = 0;
                            break;
                        case 3: /* switch 4 */
                            CourseID = COURSE_WARIO_STADIUM;
                            ScreenSplitB = 3;
                            PlayerCount = 3;
                            MenuMultiplayerSelection = (s8) 3;
                            CharacterSelections[0] = 5;
                            CharacterSelections[1] = 2;
                            CharacterSelections[2] = 7;
                            GameMode = (s32) 2;
                            break;
                        case 4: /* switch 4 */
                            CourseID = COURSE_BOWSER_CASTLE;
                            ScreenSplitB = 0;
                            PlayerCount = (s32) 1;
                            MenuMultiplayerSelection = 1;
                            CharacterSelections[0] = 7;
                            GameMode = 0;
                            break;
                        case 5: /* switch 4 */
                            CourseID = COURSE_SHERBET_LAND;
                            ScreenSplitB = 3;
                            PlayerCount = 4;
                            MenuMultiplayerSelection = 4;
                            CharacterSelections[0] = 0;
                            CharacterSelections[1] = 1;
                            CharacterSelections[2] = 6;
                            CharacterSelections[3] = 3;
                            GameMode = 2;
                            break;
                        default:
                            break;
                    }
                    NextDemoId += 1;
                    if (NextDemoId >= 6) {
                        NextDemoId = 0;
                    }
                    CupSelect = Map2CupChart[CourseID];
                    CupLevel = (s32) CupSelect;
                    CourseSelect = (s8) Map2CircuitChart[CourseID];
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
                NewSequenceMode = 4;
                if (GameMode == 1) {
                    InGameTT = (s8) 1;
                }
                SettingSoundOutMode4Multiplayer();
            }
            FBClearFlg = 0;
        } else {
            switch (DebugGotoScene) {  /* switch 5; irregular */
                case DEBUG_GOTO_ENDING: /* switch 5 */
                    NewSequenceMode = (s32) 5;
                    break;
                case DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT: /* switch 5 */
                case DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA:   /* switch 5 */
                    NewSequenceMode = 9;
                    CreditsCourseId = 8;
                    break;
                default: /* switch 5 */
                    NewSequenceMode = 4;
                    if (GameMode == (s32) 1) {
                        InGameTT = 1;
                    }
                    break;
            }
            SetRivalCarName();
            if (ScreenSplitB == 3) {
                switch (GameMode) {
                    case 0:
                    case 1:
                        GameMode = 2;
                        break;
                }
            }
            switch (CourseID) {
                case COURSE_BLOCK_FORT:
                case COURSE_SKYSCRAPER:
                case COURSE_DOUBLE_DECK:
                case COURSE_BIG_DONUT:
                    GameMode = 3;
                    if (PlayerCount == 1) {
                        MenuMultiplayerSelection = 2;
                        ScreenSplitB = 1;
                        PlayerCount = MenuMultiplayerSelection;
                    }
                    break;
                default:
                    if (GameMode == 3) {
                        GameMode = 0;
                    }
                    if ((GameMode == 2) && (PlayerCount == 1)) {
                        GameMode = 0;
                    }
            }
            CupSelect = Map2CupChart[CourseID];
            CupLevel = CupSelect;
            CourseSelect = Map2CircuitChart[CourseID];
            switch (DebugGotoScene) {  /* switch 6; irregular */
                case DEBUG_GOTO_ENDING: /* switch 6 */
                    break;
                case DEBUG_GOTO_CREDITS_SEQUENCE_DEFAULT: /* switch 6 */
                    RaceClass = 0;
                    break;
                case DEBUG_GOTO_CREDITS_SEQUENCE_EXTRA: /* switch 6 */
                    RaceClass = 3;
                    break;
                default: /* switch 6 */
                    if (RaceClass == 3) {
                        ScreenFlip = 1;
                    } else {
                        ScreenFlip = 0;
                    }
                    break;
            }
        }
    }
}

void FadeOutEx(s32 arg0, s32 arg1, s32 Arg2) {
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

    if ((GameMode == 0) || (GameMode == 1)) {
        var_t3 = OldScreenPosition[arg0].x;
        var_t4 = OldScreenPosition[arg0].y;
        var_ra = OldScreenSize[arg0].x;
        sp44 = OldScreenSize[arg0].y;
    } else if (arg0 >= 4) {
        var_t3 = OldScreenPosition[arg0].x;
        var_t4 = OldScreenPosition[arg0].y;
        var_ra = OldScreenSize[arg0].x;
        sp44 = OldScreenSize[arg0].y;
    } else {
        var_t3 = D_8015F480[arg0].Posx;
        var_t4 = D_8015F480[arg0].Posy;
        var_ra = D_8015F480[arg0].Width;
        sp44 = D_8015F480[arg0].Height;
    }
    var_t2 = (FadeCounter2[arg0] * 0xFF) / FadeCounter[arg0];
    if (var_t2 >= 0x100) {
        var_t2 = 0x000000FF;
    }
    temp_v1 = var_ra / 2;
    temp_t8 = sp44 / 2;
    temp_v0_2 = &FadeColorBw[Arg2];
    // Why does it have to written like this to match?
    someMath0 = temp_v1;
    someMath0 += var_t3;
    someMath1 = temp_t8;
    someMath1 += var_t4;
    DisplayListHead = FillRect1ColorF(DisplayListHead, var_t3 - temp_v1, var_t4 - temp_t8, someMath0, someMath1,
                                temp_v0_2->red, temp_v0_2->Green, temp_v0_2->Blue, var_t2);
    if (arg1 == 0) {
        FadeCounter2[arg0]++;
        if ((FadeCounter[arg0] + 1) < FadeCounter2[arg0]) {
            EndFadeOut(arg0);
        }
    }
}

void FadeOut(s32 arg0, s32 arg1) {
    FadeOutEx(arg0, arg1, 0);
}

void FadeOutWhite(s32 arg0, s32 arg1) {
    FadeOutEx(arg0, arg1, 1);
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

    if ((GameMode == 0) || (GameMode == 1)) {
        var_t0 = OldScreenPosition[arg0].x;
        var_t1 = OldScreenPosition[arg0].y;
        var_t2 = OldScreenSize[arg0].x;
        var_t3 = OldScreenSize[arg0].y;
    } else if (arg0 >= 4) {
        var_t0 = OldScreenPosition[arg0].x;
        var_t1 = OldScreenPosition[arg0].y;
        var_t2 = OldScreenSize[arg0].x;
        var_t3 = OldScreenSize[arg0].y;
    } else {
        var_t0 = D_8015F480[arg0].Posx;
        var_t1 = D_8015F480[arg0].Posy;
        var_t2 = D_8015F480[arg0].Width;
        var_t3 = D_8015F480[arg0].Height;
    }
    temp_v0 = var_t2 / 2;
    temp_v1 = var_t3 / 2;
    // Why does it have to written like this to match?
    someMath0 = temp_v0;
    someMath0 += var_t0;
    someMath1 = temp_v1;
    someMath1 += var_t1;
    DisplayListHead =
        FillRect1ColorF(DisplayListHead, var_t0 - temp_v0, var_t1 - temp_v1, someMath0, someMath1, 0, 0, 0, 0x000000FF);
}

void FadeOutR(void) {
    u32 var_t0;

    FadeCounter2[4]++;
    if (FadeCounter2[4] >= (FadeCounter[4] + 1)) {
        EndFadeOut(4);
    }
    gDPPipeSync(DisplayListHead++);
    var_t0 = (FadeCounter2[4] * 255) / FadeCounter[4];
    if ((s32) var_t0 >= 0x100) {
        var_t0 = 0x000000FF;
    }
    DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, (s32) var_t0);
}

void FadeOutM(void) {
    s32 temp_t4;
    s32 temp_t5;
    s32 var_s0;
    u32 var_s3;
    s32 var_v1;

    FadeCounter2[4]++;
    // why?
    var_v1 = FadeCounter2[4];
    if ((u32) var_v1 >= FadeCounter[4]) {
        if ((u32) var_v1 == FadeCounter[4]) {
            for (var_s0 = 0; var_s0 < 0x4B0; var_s0++) {
                KWork[var_s0] = 1;
            }
        } else {
            EndFadeOut(4);
        }
    } else {
        var_s0 = 0;
        var_s3 = 0;
        while (var_s3 < (0x4B0U / FadeCounter[4])) {
            if ((KWork[var_s0] == 0) &&
                (MakeRandomLimmit((0x4B0U - FadeCounter2[4]) / FadeCounter[4]) == 0)) {
                var_s3 += 1;
                KWork[var_s0] = 1;
            }
            var_s0 += 1;
            if (var_s0 >= 0x4B0) {
                var_s0 = 0;
            }
        }
    }
    gDPPipeSync(DisplayListHead++);
    gDPSetRenderMode(DisplayListHead++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
    gDPSetCombineMode(DisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    for (var_s0 = 0; var_s0 < 0x4B0; var_s0++) {
        if (KWork[var_s0] != 0) {
            temp_t4 = (var_s0 % 40) * 8;
            temp_t5 = (var_s0 / 40) * 8;
            gDPFillRectangle(DisplayListHead++, temp_t4, temp_t5, temp_t4 + 8, temp_t5 + 8);
        }
    }
    gDPPipeSync(DisplayListHead++);
    var_v1 = (FadeCounter2[4] * 255) / FadeCounter[4];
    if (var_v1 >= 0x100) {
        var_v1 = 0x000000FF;
    }
    DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, var_v1);
}

void SetFadeInEx(u32 arg0, u32 arg1) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    if ((FadingFlag[4] != 1) && (FadingFlag[4] != 6)) {
        FadingFlag[4] = arg1;
        FadeCounter[4] = arg0;
        if (FadeCounter[4] >= 0x100U) {
            FadeCounter[4] = 0xFFU;
        }
        D_8018E7E0 = 0;
    }
}

void SetFadeIn(s32 arg0) {
    SetFadeInEx(arg0, 1);
}

void SetFadeInWhite(s32 arg0) {
    SetFadeInEx(arg0, 8);
}

void SetFadeOutEx(u32 arg0, u32 arg1) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    if ((FadingFlag[4] != 2) && (FadingFlag[4] != 5)) {
        FadingFlag[4] = arg1;
        FadeCounter[4] = arg0;
        if (FadeCounter[4] >= 0x100U) {
            FadeCounter[4] = 0xFFU;
        }
        D_8018E7E0 = 0;
    }
}

void SetFadeOut(s32 arg0) {
    SetFadeOutEx(arg0, 2);
}

void SetFadeOutWhite(s32 arg0) {
    SetFadeOutEx(arg0, 7);
}

void SetFadeInScreen(s32 arg0, s32 arg1) {
    s32 temp;

    if (arg1 == 0) {
        arg1 = 1;
    }

    temp = FadingFlag[arg0];
    if ((temp != 1) && (temp != 6)) {
        FadingFlag[arg0] = 1;
        FadeCounter[arg0] = arg1;
        if ((u32) arg1 >= 0x100U) {
            FadeCounter[arg0] = 0xFF;
        }
        FadeCounter2[arg0] = 0;
    }
}

void SetFadeOutScreen(s32 arg0, s32 arg1) {
    s32 temp;

    if (arg1 == 0) {
        arg1 = 1;
    }

    temp = FadingFlag[arg0];
    if ((temp != 2) && (temp != 5)) {
        FadingFlag[arg0] = 2;
        FadeCounter[arg0] = arg1;
        if ((u32) arg1 >= 0x100U) {
            FadeCounter[arg0] = 0xFF;
        }
        FadeCounter2[arg0] = 0;
    }
}

void SetFadeOutM(s32 arg0) {
    s32 var_v0;

    if (FadingFlag[4] != 3) {
        FadingFlag[4] = 3;
        FadeCounter[4] = arg0;
        if (FadeCounter[4] >= 0x100U) {
            FadeCounter[4] = 0x000000FF;
        }
        D_8018E7E0 = 0;
        for (var_v0 = 0; var_v0 < 0x4B0; var_v0++) {
            KWork[var_v0] = 0;
        }
    }
}

void SetFadeOutR(u32 arg0) {
    if (FadingFlag[4] != 4) {
        FadingFlag[4] = 4;
        FadeCounter[4] = arg0;
        if (FadeCounter[4] >= 0x100U) {
            FadeCounter[4] = 0x000000FFU;
        }
        D_8018E7E0 = 0;
    }
}

void SetFadeOutF(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_MAIN;
}

void SetFadeOutFw(void) {
    SetFadeOutWhite(10);
    KBGNumberNext = MENU_FADE_TYPE_MAIN;
}

void SetFadeOutB(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_BACK;
}

void SetFadeOutD(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_DEMO;
}

void SetFadeOutTaData(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_DATA;
}

void SetFadeOutOption(void) {
    SetFadeOut(10);
    KBGNumberNext = MENU_FADE_TYPE_OPTION;
}

void ThunderMain(s32 arg0) {
    switch (ThunderFlg[arg0]) {
        case 0:
            break;
        case 1:
            KThunder(arg0);
            break;
        default:
            ThunderFlg[arg0] = 0;
            break;
    }
}

void KThunder(s32 arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 someIndex;
    s32 temp_t7;
    f32 temp_t7_2;
    RGBA16* temp_v0;
    struct UnkStruct_8018E7E8* temp_t0;
    struct UnkStruct_8018E7E8* temp_t1;

    temp_t7 = ThunderOnoff[ThunderCounter[arg0]];
    if (temp_t7 != 0) {
        temp_t1 = &OldScreenPosition[arg0];
        temp_t0 = &OldScreenSize[arg0];
        temp_v0 = &KThunderColor[temp_t7];
        if ((u32) ThunderCounter[arg0] < 0x1BU) {
            DisplayListHead = FillRect1ColorF(DisplayListHead, temp_t1->x - (temp_t0->x / 2), temp_t1->y - (temp_t0->y / 2),
                                        temp_t1->x + (temp_t0->x / 2), temp_t1->y + (temp_t0->y / 2), temp_v0->red,
                                        temp_v0->Green, temp_v0->Blue, temp_v0->Alpha);
        } else {
            temp_t7_2 = ((u32) (38 - ThunderCounter[arg0])) / 11.0;
            DisplayListHead = FillRect1ColorF(DisplayListHead, temp_t1->x - (temp_t0->x / 2), temp_t1->y - (temp_t0->y / 2),
                                        temp_t1->x + (temp_t0->x / 2), temp_t1->y + (temp_t0->y / 2), temp_v0->red,
                                        temp_v0->Green, temp_v0->Blue, (u32) (temp_v0->Alpha * temp_t7_2));
        }
    }
    ThunderCounter[arg0]++;
    if ((u32) ThunderCounter[arg0] >= 0x26U) {
        for (someIndex = 0; someIndex < 4; someIndex++) {
            ThunderFlg[someIndex] = 0;
        }
    }
}

void ThunderWorld(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        SetThunderEx(i);
    }
}

void SetThunderEx(s32 arg0) {
    ThunderFlg[arg0] = 1;
    ThunderCounter[arg0] = 0;
}

void InitObjBlock(void) {
    s32 Index;
    for (Index = 0; Index < ARRAY_COUNT(MenuItems); Index++) {
        MenuItems[Index].Kind = 0;
    }
}

void SetObjBlock(s32 Type, s32 column, s32 row, s8 Priority) {
    ObjBlock* menuItem;
    s8 temp_a1;
    s32 i;
    MkAnimation* var_a0;

    i = 0;
    menuItem = MenuItems;
    while (true) {
        if (menuItem->Kind == 0) {
            break;
        }
        i++;
        if (i > ARRAY_COUNT(MenuItems)) {
            while (true) {}
        }
        menuItem++;
    }
    menuItem->Kind = Type;
    menuItem->Pattern = 0;
    menuItem->Pattern2 = 0;
    menuItem->DispX = column;
    menuItem->DispY = row;
    menuItem->pri = Priority;
    menuItem->DispOn = 1;
    menuItem->Work1 = 0;
    menuItem->Work2 = 0;
    switch (Type) {
        case MENU_ITEM_UI_LOGO_INTRO:
            LogoTimer = 0;
            LogoDelaySize = 0;
            LogoYangSpeed = 3.0f;
            LogoCamZ = 2500;
            LogoRotX = 0.0f;
            LogoRotY = -270.0f;
            LogoRotZ = 0.0f;
            LogoPosX = 0.0f;
            LogoPosY = 0.0f;
            LogoPosZ = 0.0f;
            LogoScaleZ = 3;
            menuItem->Work1 = -1;
            menuItem->Work2 = 1;
            break;
        case START_MENU_FLAG:
            LogoCamZ = 1800;
            LogoRotX = -51.0f;
            LogoRotY = -12.0f;
            LogoRotZ = -18.0f;
            LogoPosX = -270.0f;
            LogoPosY = 750.0f;
            LogoPosZ = 0.0f;
            LogoScaleZ = 1.0f;
            menuItem->Work1 = -1;
            menuItem->Work2 = 1;
            break;
        case MENU_ITEM_TYPE_0D2:
            TexBuffLoadP(D_020014C8, LOAD_MENU_IMG_TKMK00_ONCE);
            KDLMaker1(D_020014C8);
            KdlBuffPtr = DoTexList(KdlBuffPtr, D_020014C8, menuItem->DispX, menuItem->DispY);
            KDLMaker2();
            break;
        case MENU_ITEM_TYPE_0D3:
            TexBuffLoadP(D_02001540, LOAD_MENU_IMG_TKMK00_ONCE);
            KDLMaker1(D_02001540);
            KdlBuffPtr = DoTexList(KdlBuffPtr, D_02001540, menuItem->DispX, menuItem->DispY);
            KDLMaker2();
            break;
        case MENU_ITEM_TYPE_0D4:
            TexBuffLoadP(D_0200157C, LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoad(D_02001874);
            menuItem->DispY = 0x69;
            for (i = 0; i < 133; i++) {
                TexBuffLoad(GetTBPointer(D_800E7AF8[i]));
            }
            break;
        case MENU_ITEM_TYPE_0D5:
            TexBuffLoad(D_020015A4);
            KDLMaker1(D_020015A4);
            KdlBuffPtr = DoTexList(KdlBuffPtr, D_020015A4, menuItem->DispX, menuItem->DispY);
            gDPLoadTextureBlock(KdlBuffPtr++, GetTexBuffPointer(gTexture7ED50C), G_IM_FMT_IA, G_IM_SIZ_16b, 256, 5, 0,
                                G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK,
                                G_TX_NOLOD, G_TX_NOLOD);
            gSPTextureRectangle(KdlBuffPtr++, 0x80, 0x2C0, 0x480, 0x2D4, G_TX_RENDERTILE, 0, 0x80, 0x0400, 0xFC00);
            KDLMaker2();
            TexBuffLoad(D_020015CC);
            KDLMaker1(D_020015CC);
            KdlBuffPtr = DoTexList(KdlBuffPtr, D_020015CC, menuItem->DispX, menuItem->DispY);
            KDLMaker2();
            TexBuffLoad(D_02001630);
            KDLMaker1(D_02001630);
            KdlBuffPtr = DoTexList(KdlBuffPtr, D_02001630, menuItem->DispX, menuItem->DispY);
            KDLMaker2();
            TexBuffLoad(D_02001658);
            KDLMaker1(D_02001658);
            KdlBuffPtr = DoTexList(KdlBuffPtr, D_02001658, menuItem->DispX, menuItem->DispY);
            KDLMaker2();
            break;
        case MENU_ITEM_TYPE_0D6:
            menuItem->SdNumber = SetSeqData(GetVAddress(D_800E7D34[0]));
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
            TexBuffLoadP(MenuTexturesBackground[Check4GoldCup150CC()], LOAD_MENU_IMG_TKMK00_ONCE);
#endif
            break;
        case MENU_ITEM_UI_LOGO_AND_COPYRIGHT:
            KwmeltTitlelogo();
            TexBuffNum += 0x10000;
            TexBuffLoad(Seg2Copyright1996Texture);
            break;
        case MENU_ITEM_UI_PUSH_START_BUTTON:
            TexBuffLoad(Seg2PushStartButtonTexture);
            break;
        case MAIN_MENU_BACKGROUND:
        case CHARACTER_SELECT_BACKGROUND:
        case COURSE_SELECT_BACKGROUND:
            TexBuffLoadP(MenuTexturesBackground[Check4GoldCup150CC()], LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoadP(D_02004B74, LOAD_MENU_IMG_TKMK00_ONCE);
            GrayScaleTexBuf3(0, 0x00000019);
            GrayScaleTexBufRGB(0, SCREEN_WIDTH * SCREEN_HEIGHT, TitlebackColor[Type - MAIN_MENU_BACKGROUND].red,
                              TitlebackColor[Type - MAIN_MENU_BACKGROUND].Green, TitlebackColor[Type - MAIN_MENU_BACKGROUND].Blue);
            break;
        case MENU_ITEM_UI_OK:
            menuItem->Work1 = 0x20;
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
            TexBuffLoadP(GetTBPointer(D_800E8274[Type - 0x12]), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_UI_1P_GAME:
        case MENU_ITEM_UI_2P_GAME:
        case MENU_ITEM_UI_3P_GAME:
        case MENU_ITEM_UI_4P_GAME:
            TexBuffLoadP(GetTBPointer(D_800E8234[((Type - 0xB) * 2) + 0]),
                                    LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoad(GetTBPointer(D_800E8234[((Type - 0xB) * 2) + 1]));
            break;
        case CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER:
            TexBuffLoadP(D_02004B4C, LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case CHARACTER_SELECT_MENU_OK:
            TexBuffLoadP(D_02004B74, LOAD_MENU_IMG_TKMK00_ONCE);
            menuItem->Work1 = 0x00000020;
            break;
        case CHARACTER_SELECT_MENU_1P_CURSOR:
        case CHARACTER_SELECT_MENU_2P_CURSOR:
        case CHARACTER_SELECT_MENU_3P_CURSOR:
        case CHARACTER_SELECT_MENU_4P_CURSOR:
            TexBuffLoad(GetTBPointer(MenuTexturesBorderPlayer[Type - CHARACTER_SELECT_MENU_1P_CURSOR]));
            break;
        case CHARACTER_SELECT_MENU_MARIO:
        case CHARACTER_SELECT_MENU_LUIGI:
        case CHARACTER_SELECT_MENU_TOAD:
        case CHARACTER_SELECT_MENU_PEACH:
        case CHARACTER_SELECT_MENU_YOSHI:
        case CHARACTER_SELECT_MENU_DK:
        case CHARACTER_SELECT_MENU_WARIO:
        case CHARACTER_SELECT_MENU_BOWSER:
            menuItem->SdNumber =
                SetSeqData(GetVAddress(D_800E8320[Type - 0x2B]));
            TexBuffLoadP(GetTBPointer(SelectNamePlateTable[Type - 0x2B]), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_0A0:
        case MENU_ITEM_TYPE_0A1:
            menuItem->SdNumber =
                SetSeqData(GetVAddress(D_800E8320[Type - 0xA0]));
            break;
        case COURSE_SELECT_OK:
            menuItem->Work1 = 0x00000020;
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
            TexBuffLoadP(GetTBPointer(MenuTexturesTrackSelection[Type - 0x52]),
                                    LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_05F:
        case MENU_ITEM_TYPE_060:
        case MENU_ITEM_TYPE_061:
        case MENU_ITEM_TYPE_062:
            menuItem->SdNumber = SetSeqData(
                GetVAddress(D_800E7E34[CupCourseOrder[0][menuItem->Kind - 0x5F]]));
            break;
        case MENU_ITEM_TYPE_05E:
            menuItem->Work2 = MakeRandomLimmit(4) + 2;
            break;
        case MENU_ITEM_TYPE_065:
        case MENU_ITEM_TYPE_066:
            menuItem->DispX = MselBestboxDefaultPosition[Type - 0x65].x;
            menuItem->DispY = MselBestboxDefaultPosition[Type - 0x65].y;
            break;
        case MENU_ITEM_TYPE_067:
            menuItem->Work1 = (s32) CupSelect;
            menuItem->Work2 = GetCupStatus(CupSelect, RaceClass);
            menuItem->SdNumber = SetSeqData(
                GetVAddress(D_800E7E20[((RaceClass / 2) * 4) - menuItem->Work2]));
            menuItem->DispX = MselCupPositions[0].x;
            menuItem->DispY = MselCupPositions[0].y;
            break;
        case MENU_ITEM_TYPE_068:
            TexBuffLoadP(GetTBPointer(D_800E8294[RaceClass]), LOAD_MENU_IMG_TKMK00_ONCE);
            menuItem->DispX = 0x37;
            menuItem->DispY = 0xC3;
            break;
        case MENU_ITEM_TYPE_069:
            TexBuffLoadP(GetTBPointer(D_02004A0C), LOAD_MENU_IMG_TKMK00_ONCE);
            if (CheckPakStatus() == 0) {
                LoadGameNoteKartData();
            } else {
                D_8018EE10[0].GhostDataSaved = 0;
                D_8018EE10[1].GhostDataSaved = 0;
            }
            break;
        case MENU_ITEM_TYPE_078:
        case MENU_ITEM_TYPE_079:
        case MENU_ITEM_TYPE_07A:
        case MENU_ITEM_TYPE_07B:
            TexBuffLoadP(GetTBPointer(D_800E82F4[Type - 0x78]), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_08C:
            TexBuffLoadP(GetTBPointer(Seg2DataTexture), LOAD_MENU_IMG_TKMK00_ONCE);
            if (CheckPakStatus() == 0) {
                LoadGameNoteKartData();
            } else {
                D_8018EE10[0].GhostDataSaved = 0;
                D_8018EE10[1].GhostDataSaved = 0;
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
                GetTBPointer(D_800E7D74[CupCourseOrder[(menuItem->Kind - MENU_ITEM_TYPE_07C) / 4]
                                                                    [(menuItem->Kind - MENU_ITEM_TYPE_07C) % 4]]),
                LOAD_MENU_IMG_MIO0_ONCE);
            TexBuffLoadP(
                GetTBPointer(D_800E7DC4[CupCourseOrder[(menuItem->Kind - MENU_ITEM_TYPE_07C) / 4]
                                                                    [(menuItem->Kind - MENU_ITEM_TYPE_07C) % 4]]),
                LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoadP(GetTBPointer(D_02004A0C), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_0B1:
        case MENU_ITEM_TYPE_0B2:
        case MENU_ITEM_TYPE_0B3:
        case MENU_ITEM_TYPE_0B4: {
            bool var_v1_3;
            UNUSED s32 Pad2;
            s32 temp_a3 = Type - MENU_ITEM_TYPE_0B1;
            UNUSED s32 pad[0x3];
            temp_a1 = Driver2Pselset[CharacterSelections[Type - MENU_ITEM_TYPE_0B1]];
            var_v1_3 = false;
            switch (GameMode) {
                case VERSUS:
                    if (GPCurrentRaceRankByPlayerId[Type - MENU_ITEM_TYPE_0B1] != 0) {
                        var_v1_3 = true;
                    }
                    break;
                case BATTLE:
                    if ((Type - MENU_ITEM_TYPE_0B1) != WinKart) {
                        var_v1_3 = true;
                    }
                    break;
                default:
                    break;
            }
            if (var_v1_3) {
                var_a0 = TBSeqPselP6[temp_a1];
            } else {
                var_a0 = D_800E8320[temp_a1];
            }
            menuItem->SdNumber = SetSeqData2(GetVAddress(var_a0), temp_a3);
            TexBuffLoadP(GetTBPointer(SelectNamePlateTable[temp_a1]), LOAD_MENU_IMG_TKMK00_ONCE);
            TexBuffLoad(GetTBPointer(MenuTexturesBorderPlayer[Type - MENU_ITEM_TYPE_0B1]));
            break;
        }
        case MENU_ITEM_TYPE_0BB:
            menuItem->Work1 = SaveCourseRecord(PlayerHUD[0].Totaltime, CharacterSelections[0]);
            menuItem->Work2 = SaveCourseLapRecord();
            if (GhostStatus != 1) {
                if (MakePressGhost() > 0x3C00) {
                    GhostStatus = 1;
                }
            }
            if ((menuItem->Work1 == 0) || (menuItem->Work2 != 0)) {
                SaveFunc800B559CDefault((CupSelect * 4) + CourseSelect);
            }
            break;
        case MENU_ITEM_DATA_COURSE_IMAGE:
            menuItem->SdNumber = SetSeqData(GetVAddress(
                D_800E7E34[CupCourseOrder[TimeTrialDataCourseIndex / 4][TimeTrialDataCourseIndex % 4]]));
            menuItem->Work1 = TimeTrialDataCourseIndex;
            TexBuffLoadP(GetTBPointer(D_02004A0C), LOAD_MENU_IMG_TKMK00_ONCE);
            KwmeltKawanoroadmap();
            if (CheckPakStatus() == 0) {
                LoadGameNoteKartData();
            } else {
                D_8018EE10[0].GhostDataSaved = 0;
                D_8018EE10[1].GhostDataSaved = 0;
            }
            break;
        case MENU_ITEM_TYPE_0F0:
            menuItem->Pattern = (s32) SoundOutput;
            break;
        case MENU_ITEM_TYPE_0F1:
            TexBuffLoadP(GetTBPointer(D_02004638), LOAD_MENU_IMG_TKMK00_ONCE);
            break;
        case MENU_ITEM_TYPE_0BE:
            RankinFanfare = 0;
            break;
        case MENU_ITEM_TYPE_130: {
            bool var_v0_2;
            temp_a1 = Driver2Pselset[Resultstat.Kart];
            if (Resultstat.Rank >= 3) {
                var_v0_2 = true;
            } else {
                var_v0_2 = false;
            }
            if (var_v0_2) {
                var_a0 = TBSeqPselP6[temp_a1];
            } else {
                var_a0 = D_800E8320[temp_a1];
            }
            menuItem->SdNumber = SetSeqData2(GetVAddress(var_a0), 0);
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
    MenuTexture* Texture;
    s32 temp_a0;
    s32 temp_t2;
    s32 temp_t5;
    s32 temp_t9;
    s32 temp_v1;
    UNUSED s32 Pad2;
    char sp80[3];
    f32 why = 0.75f;
    s32 one = 1;
    UNUSED s32 Pad3;

    if (arg0->DispOn) {
        gDPPipeSync(DisplayListHead++);
        switch (arg0->Kind) {             /* switch 6; irregular */
            case MENU_ITEM_UI_LOGO_INTRO: /* switch 6 */
                DoNintenLogo(DynamicP, arg0->Work1);
                break;
            case START_MENU_FLAG: /* switch 6 */
                DoCheckerFlag(DynamicP, arg0->Work1);
                break;
            case MENU_ITEM_TYPE_0D2: /* switch 6 */
                DisplayListHead = KDLCall(DisplayListHead, D_020014C8);
                break;
            case MENU_ITEM_TYPE_0D3: /* switch 6 */
                DisplayListHead = KDLCall(DisplayListHead, D_02001540);
                break;
            case MENU_ITEM_TYPE_0D4: /* switch 6 */
                DispOBCpGnote(arg0);
                DispOBCpNotecursor(arg0);
                DispOBCpGnoteName(arg0);
                break;
            case MENU_ITEM_TYPE_0D5: /* switch 6 */
                                     // Sets the text color of the text on the Controller Pak menu
                                     // Also sets the color of the shading at the top and bottom of the menu
                                     // Does not set color of the text in the Table itself
                gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0xFF);
                DisplayListHead = KDLCall(DisplayListHead, D_020015A4);
                gDPSetPrimColor(DisplayListHead++, 0, 0, 0x32, 0x00, 0x00, 0xFF);
                DisplayListHead = KDLCall(DisplayListHead, D_020015CC);
                gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x32, 0x00, 0xFF);
                DisplayListHead = KDLCall(DisplayListHead, D_02001630);
                gDPSetPrimColor(DisplayListHead++, 0, 0, 0x32, 0x32, 0x00, 0xFF);
                DisplayListHead = KDLCall(DisplayListHead, D_02001658);
                break;
            case MENU_ITEM_TYPE_0D6: /* switch 6 */
                DoSeqData2(arg0->SdNumber, arg0->DispX, arg0->DispY, -1);
                break;
            case MENU_ITEM_TYPE_0D7: /* switch 6 */
                DispOBCpFree();
                break;
            case MENU_ITEM_TYPE_0D8: /* switch 6 */
            case MENU_ITEM_TYPE_0D9: /* switch 6 */
                DispOBCpCursor(arg0, arg0->Kind - 0xD8);
                break;
            case MENU_ITEM_UI_START_BACKGROUND: /* switch 6 */
#if !OverKartBuild
                DisplayListHead = DoTexList(
                    DisplayListHead, MenuTexturesBackground[Check4GoldCup150CC()], arg0->DispX, arg0->DispY);
#endif
                break;
            case MENU_ITEM_UI_LOGO_AND_COPYRIGHT: /* switch 6 */
                KwdisplayTitlelogo((arg0->DispX + 0xA0), (arg0->DispY + 0x47));
                DisplayListHead =
                    DoTexList(DisplayListHead, Seg2Copyright1996Texture, arg0->DispX, arg0->DispY);
                break;
            case MENU_ITEM_UI_PUSH_START_BUTTON: /* switch 6 */
                if (((FrameCounter / 8) % 3) != 0) {
                    DisplayListHead =
                        DoTexList(DisplayListHead, Seg2PushStartButtonTexture, arg0->DispX, arg0->DispY);
                }
                break;
            case MENU_ITEM_UI_START_RECORD_TIME: {
                s32 strWidth;
                strWidth = (s32) ((f32) (GetWordLength(StringTableCourse[0]) + 5) * 0.9f) / 2;
                DisplayListHead = FillRect1ColorF(DisplayListHead, 0xA0 - strWidth, 0x0000007B, strWidth + 0xA0, 0x000000A4,
                                            0, 0, 0, 0x00000096);
                SetTextColor(TEXT_GREEN);
                PrintText1CenterMode1(0x0000009B, 0x0000008C, StringTableCourse[0], 0, 0.9f, 0.9f);
                temp_v1 = GetRecordTime3(0, 7) & 0xFFFFF;
                if (temp_v1 < 0x1EAA) {
                    SetTextColor((s32) FrameCounter % 2);
                } else if (temp_v1 < 0x2329) {
                    SetTextColor((s32) FrameCounter % 3);
                } else {
                    SetTextColor(TEXT_YELLOW);
                }
                KawanoItoaM(temp_v1, sp80);
                SetWord4A(0x00000077, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                SetWord2A(0x0000008B, 0x000000A0, "'", 0, 1.0f, 1.0f);
                KawanoItoaS(temp_v1, sp80);
                SetWord4A(0x00000094, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                SetWord2A(0x000000A7, 0x000000A0, "\"", 0, 1.0f, 1.0f);
                KawanoItoaC(temp_v1, sp80);
                SetWord4A(0x000000B4, 0x000000A0, sp80, 0, 1.0f, 1.0f);
                break;
            }
            case MENU_ITEM_UI_NO_CONTROLLER: {
                s32 strWidth;
                UNUSED s32 padCont[2];
                strWidth = GetWordLength(NoControllerMessage[0]);
                temp_v1 = GetWordLength(NoControllerMessage[1]);
                if (strWidth < temp_v1) {
                    strWidth = temp_v1;
                }
                temp_t2 = (s32) (strWidth * why) / 2;
                temp_t5 = (s32) (((why * 2) + 0.5) * 16.0) / 2;
                DisplayListHead = FillRect1ColorF(DisplayListHead, 0xA0 - temp_t2, 0xB6 - temp_t5, temp_t2 + 0xA0,
                                            temp_t5 + 0xB6, 0, 0, 0, 0x00000096);
                SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
                for (strWidth = 0; strWidth < 2; strWidth++) {
                    PrintText1CenterMode1(0xA0 * one - 1 * why,
                                              (s32) (0xB4 * one + ((f32) (strWidth * 0x12) * why)),
                                              NoControllerMessage[strWidth], 0, why, why);
                }
                break;
            }
            case MAIN_MENU_BACKGROUND:
            case CHARACTER_SELECT_BACKGROUND:
            case COURSE_SELECT_BACKGROUND:
                DisplayListHead = DoTexList2(DisplayListHead, MenuTexturesBackground[Check4GoldCup150CC()],
                                                 arg0->DispX, arg0->DispY, 3, 0);
                break;
            case MENU_ITEM_UI_GAME_SELECT:
                DisplayListHead =
                    DoTexList(DisplayListHead, Seg2GameSelectTexture, arg0->DispX, arg0->DispY);
                break;
            case MENU_ITEM_UI_1P_GAME:
            case MENU_ITEM_UI_2P_GAME:
            case MENU_ITEM_UI_3P_GAME:
            case MENU_ITEM_UI_4P_GAME:
                var_a1 = arg0->Kind - MENU_ITEM_UI_1P_GAME;
                GselSub1(var_a1, arg0);
                DispOBSubGsel1(arg0, var_a1);
                break;
            case MENU_ITEM_UI_OK:
                SelOKSub(arg0);
                DisplayListHead =
                    DoTexList2(DisplayListHead, D_0200487C, arg0->DispX, arg0->DispY, 2, arg0->Work1);
                break;
            case MAIN_MENU_OPTION_GFX:
            case MAIN_MENU_DATA_GFX:
                var_a1 = arg0->Kind - 0xF;
                if (arg0->Work1 < 0x20) {
                    temp_t9 = (arg0->Work1 * 0x3A) / 64;
                    if (var_a1 == MenuScreenB) {
                        DisplayListHead =
                            FlashRectangle2f(DisplayListHead, arg0->DispX + temp_t9, (u32) arg0->DispY,
                                                        (arg0->DispX - temp_t9) + 0x39, arg0->DispY + 0x12);
                    } else {
                        DisplayListHead =
                            FillRect1Color(DisplayListHead, arg0->DispX + temp_t9, arg0->DispY,
                                          (arg0->DispX - temp_t9) + 0x39, arg0->DispY + 0x12, 1, 1, 1, 0x000000FF);
                    }
                }
                var_v1 = arg0->Kind - 0xA;
                DisplayListHead =
                    DoTexList2(DisplayListHead, D_800E8254[var_v1], arg0->DispX, arg0->DispY, 2, arg0->Work1);
                break;
            case MAIN_MENU_50CC:              /* switch 6 */
            case MAIN_MENU_100CC:             /* switch 6 */
            case MAIN_MENU_150CC:             /* switch 6 */
            case MAIN_MENU_EXTRA_CC:          /* switch 6 */
            case MENU_ITEM_TYPE_016:          /* switch 6 */
            case MENU_ITEM_TYPE_017:          /* switch 6 */
            case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 6 */
            case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 6 */
                var_a1 = GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]];
                var_v1 = GameModePlayerSelection[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]];
                switch (arg0->Kind) {        /* switch 5 */
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
                        Texture = GetTBPointer(D_800E8294[arg0->Kind - MAIN_MENU_50CC]);
                        break;
                    case MENU_ITEM_TYPE_016: /* switch 5 */
                    case MENU_ITEM_TYPE_017: /* switch 5 */
                        if (var_v1 != 2) {
                            var_a1 = -1;
                            break;
                        } else {
                            var_v1 = MENU_ITEM_TYPE_016;
                            Texture = GetTBPointer(D_800E82A4[arg0->Kind - MENU_ITEM_TYPE_016]);
                        }
                        break;
                    case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 5 */
                    case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 5 */
                        if (var_v1 != 1) {
                            var_a1 = -1;
                            break;
                        } else {
                            var_v1 = MAIN_MENU_TIME_TRIALS_BEGIN;
                            Texture = GetTBPointer(D_800E82AC[arg0->Kind - MAIN_MENU_TIME_TRIALS_BEGIN]);
                        }
                        break;
                    default:
                        break;
                }
                if (var_a1 == -1) {
                    break;
                }
                if ((MenuScreenB >= MAIN_MENU_MODE_SUB_SELECT) && (var_a1 == (arg0->Kind - var_v1))) {
                    if (MenuScreenB > MAIN_MENU_MODE_SUB_SELECT) {
                        DisplayListHead =
                            FillRect1Color(DisplayListHead, arg0->DispX, arg0->DispY, arg0->DispX + 0x3F,
                                          arg0->DispY + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                    } else {
                        DisplayListHead = FlashRectangle2(
                            DisplayListHead, arg0->DispX ^ 0, arg0->DispY ^ 0, arg0->DispX + 0x3F, arg0->DispY + 0x11);
                    }
                } else {
                    DisplayListHead = FillRect1Color(DisplayListHead, arg0->DispX, arg0->DispY, arg0->DispX + 0x3F,
                                                     arg0->DispY + 0x11, 1, 1, 1, 0x000000FF);
                }
                DisplayListHead = DoTexList(DisplayListHead, Texture, arg0->DispX, arg0->DispY);
                break;
            case MENU_ITEM_TYPE_01B: /* switch 6 */
                DispOBSubGselCaution(arg0);
                break;
            case CHARACTER_SELECT_MENU_PLAYER_SELECT_BANNER: /* switch 6 */
                DisplayListHead = DoTexList(DisplayListHead, D_02004B4C, arg0->DispX, arg0->DispY);
                break;
            case CHARACTER_SELECT_MENU_1P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_2P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_3P_CURSOR: /* switch 6 */
            case CHARACTER_SELECT_MENU_4P_CURSOR: /* switch 6 */
                temp_a0 = arg0->Kind - CHARACTER_SELECT_MENU_1P_CURSOR;
                if (OKPSelSkipCursor[temp_a0] == 0) {
                    if (Player1Character[temp_a0]) {
                        if (CharacterGridIsSelected[temp_a0] == 0) {
                            temp_t2 = 255;
                        } else {
                            temp_t2 = FrameCounter % 16;
                            if (temp_t2 >= 8) {
                                temp_t2 = (-temp_t2 * 8) + 0x80;
                            } else {
                                temp_t2 *= 8;
                            }
                            temp_t2 += 191;
                        }
                        DispOBSubPselCursor(arg0, temp_a0, temp_t2);
                    }
                }
                break;
            case CHARACTER_SELECT_MENU_OK: /* switch 6 */
                SelOKSub(arg0);
                DisplayListHead =
                    DoTexList2(DisplayListHead, D_02004B74, arg0->DispX, arg0->DispY, 2, arg0->Work1);
                break;
            case CHARACTER_SELECT_MENU_MARIO:  /* switch 6 */
            case CHARACTER_SELECT_MENU_LUIGI:  /* switch 6 */
            case CHARACTER_SELECT_MENU_TOAD:   /* switch 6 */
            case CHARACTER_SELECT_MENU_PEACH:  /* switch 6 */
            case CHARACTER_SELECT_MENU_YOSHI:  /* switch 6 */
            case CHARACTER_SELECT_MENU_DK:     /* switch 6 */
            case CHARACTER_SELECT_MENU_WARIO:  /* switch 6 */
            case CHARACTER_SELECT_MENU_BOWSER: /* switch 6 */
                if (OKPSelSkipPortrait[arg0->Kind - CHARACTER_SELECT_MENU_MARIO] != 0) {
                    break;
                }
                DispOBSubPsel1(arg0, GetTBPointer(SelectNamePlateTable[arg0->Kind - 0x2B]));
                /* fallthrough */
            case MENU_ITEM_TYPE_0A0: /* switch 6 */
            case MENU_ITEM_TYPE_0A1: /* switch 6 */
                DoSeqData2(arg0->SdNumber, arg0->DispX, arg0->DispY, arg0->Work1);
                break;
            case MENU_ITEM_TYPE_058:         /* switch 6 */
            case COURSE_SELECT_COURSE_NAMES: /* switch 6 */
            case MENU_ITEM_TYPE_05A:         /* switch 6 */
            case MENU_ITEM_TYPE_05B:         /* switch 6 */
            case COURSE_SELECT_BATTLE_NAMES: /* switch 6 */
                MselSub2(arg0);
                DisplayListHead = DoTexList(
                    DisplayListHead,
                    GetTBPointer(MenuTexturesTrackSelection[arg0->Kind - COURSE_SELECT_MAP_SELECT]),
                    arg0->DispX, arg0->DispY);
                MselSub4(arg0);
                break;
            case COURSE_SELECT_MAP_SELECT: /* switch 6 */
                DisplayListHead = DoTexList(
                    DisplayListHead,
                    GetTBPointer(MenuTexturesTrackSelection[arg0->Kind - COURSE_SELECT_MAP_SELECT]),
                    arg0->DispX, arg0->DispY);
                break;
            case MENU_ITEM_TYPE_05F: /* switch 6 */
            case MENU_ITEM_TYPE_060: /* switch 6 */
            case MENU_ITEM_TYPE_061: /* switch 6 */
            case MENU_ITEM_TYPE_062: /* switch 6 */
                DispOBSubMselMap(arg0);
                break;
            case COURSE_SELECT_MUSHROOM_CUP: /* switch 6 */
            case COURSE_SELECT_FLOWER_CUP:   /* switch 6 */
            case COURSE_SELECT_STAR_CUP:     /* switch 6 */
            case COURSE_SELECT_SPECIAL_CUP:  /* switch 6 */
                var_a1 = arg0->Kind - COURSE_SELECT_MUSHROOM_CUP;
                MselSub1(var_a1, arg0);
                DispOBSubMsel1(arg0, var_a1);
                break;
            case COURSE_SELECT_OK: /* switch 6 */
                SelOKSub(arg0);
                DisplayListHead =
                    DoTexList2(DisplayListHead, D_02004E80, arg0->DispX, arg0->DispY, 2, arg0->Work1);
                break;
            case MENU_ITEM_TYPE_065: /* switch 6 */
            case MENU_ITEM_TYPE_066: {
                f32 scaleX;
                if (arg0->Kind == MENU_ITEM_TYPE_065) {
                    scaleX = 0.6f;
                } else {
                    scaleX = 0.8f;
                }
                MselBboxSub(arg0);
                SetTextColor(TEXT_YELLOW);
                SetWord2A(arg0->DispX + 8, arg0->DispY + 0x10, BestrecJ[arg0->Kind - 0x65], 0, scaleX,
                                  0.8f);
                MselBboxSub2(arg0);
                break;
            }
            case MENU_ITEM_TYPE_06E: /* switch 6 */
                MselBattleSub(arg0);
                break;
            case MENU_ITEM_TYPE_067: /* switch 6 */
                DispOBSubMselCup(arg0);
                break;
            case MENU_ITEM_TYPE_068: /* switch 6 */
                DisplayListHead = FillRect1Color(DisplayListHead, arg0->DispX, arg0->DispY, arg0->DispX + 0x3F,
                                                 arg0->DispY + 0x11, 1, 1, 1, 0x000000FF);
                DisplayListHead = DoTexList(
                    DisplayListHead, GetTBPointer(D_800E8294[RaceClass]), arg0->DispX, arg0->DispY);
                break;
            case MENU_ITEM_TYPE_069: /* switch 6 */
                DispOBSubMselGhost(arg0);
                break;
            case MENU_ITEM_TYPE_078: /* switch 6 */
            case MENU_ITEM_TYPE_079: /* switch 6 */
            case MENU_ITEM_TYPE_07A: /* switch 6 */
            case MENU_ITEM_TYPE_07B: /* switch 6 */
                var_a1 = arg0->Kind - MENU_ITEM_TYPE_078;
                TselSub1(var_a1, arg0);
                DispOBSubMsel1(arg0, var_a1);
                break;
            case MENU_ITEM_TYPE_08C: /* switch 6 */
                if ((MenuScreenB >= MAIN_MENU_MODE_SUB_SELECT) && (var_a1 == (arg0->Kind - var_v1))) {
                    if (MenuScreenB > MAIN_MENU_MODE_SUB_SELECT) {
                        DisplayListHead =
                            FillRect1Color(DisplayListHead, arg0->DispX, arg0->DispY, arg0->DispX + 0x3F,
                                          arg0->DispY + 0x11, 0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                    } else {
                        DisplayListHead =
                            FlashRectangle2(DisplayListHead, arg0->DispX ^ 0, one = arg0->DispY ^ 0,
                                                        arg0->DispX + 0x3F, arg0->DispY + 0x11);
                    }
                } else {
                    DisplayListHead = FillRect1Color(DisplayListHead, arg0->DispX, arg0->DispY, arg0->DispX + 0x3F,
                                                     arg0->DispY + 0x11, 1, 1, 1, 0x000000FF);
                }
                DisplayListHead = DoTexList(DisplayListHead, Seg2DataTexture, arg0->DispX, arg0->DispY);
                SetTextColor(TEXT_YELLOW);
                SetWord3A(0x00000125, 0x0000001C, TselTitleJ, 0, 0.55f, 0.55f);
                break;
            case MENU_ITEM_TYPE_08D: /* switch 6 */
                DispOBSubTselCursor(arg0);
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
                DispOBSubTsel1(arg0);
                break;
            case MENU_ITEM_TYPE_096: /* switch 6 */
                SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
                SetWord3A(arg0->DispX, arg0->DispY, CupName[CupLevel], arg0->Work1, arg0->Workf1, 1.0f);
                break;
            case MENU_ITEM_TYPE_097: /* switch 6 */
                SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
                SetWord2A(arg0->DispX, arg0->DispY, StringTableCourseGP[CourseID], arg0->Work1, arg0->Workf1,
                                  1.0f);
                break;
            case MENU_ITEM_TYPE_098: /* switch 6 */
                CutUdSub(arg0);
                break;
            case MENU_ITEM_TYPE_05E: /* switch 6 */
                DisplayListHead = AutoTextureRectNoise(DisplayListHead, 0x00000019, 0x00000072, 0x0000007CU, 0x0000004AU);
                break;
            case MENU_ITEM_TYPE_0AA: /* switch 6 */
                DispOBResultTime(arg0);
                break;
            case MENU_ITEM_TYPE_0AB: /* switch 6 */
                DispOBResultPoint(arg0);
                break;
            case MENU_ITEM_TYPE_0AC: /* switch 6 */
                DispOBGPLose(arg0);
                break;
            case MENU_ITEM_TYPE_0AF: /* switch 6 */
                DispOBInfo3P(arg0);
                break;
            case MENU_ITEM_TYPE_0B0: /* switch 6 */
                DispOBVSResult(arg0);
                break;
            case MENU_ITEM_TYPE_0B1: /* switch 6 */
            case MENU_ITEM_TYPE_0B2: /* switch 6 */
            case MENU_ITEM_TYPE_0B3: /* switch 6 */
            case MENU_ITEM_TYPE_0B4: /* switch 6 */
                if (arg0->Pattern != 0) {
                    var_v1 = arg0->Kind - MENU_ITEM_TYPE_0B1;
                    one = Driver2Pselset[CharacterSelections[var_v1]];
                    DisplayListHead = DoTexList(
                        DisplayListHead, GetTBPointer(SelectNamePlateTable[one]), arg0->DispX, arg0->DispY);
                    DoSeqData2VS(arg0->SdNumber, arg0->DispX, arg0->DispY, var_v1, arg0->Work1);
                    DispOBSubPselCursor(arg0, var_v1, 0x000000FF);
                }
                break;
            case MENU_ITEM_TYPE_0B9: /* switch 6 */
                DispOBTimeattack1(arg0);
                break;
            case MENU_ITEM_TYPE_0BA: /* switch 6 */
                DispOBTimeattack2(arg0);
                break;
            case MENU_ITEM_ANNOUNCE_GHOST: /* switch 6 */
                DispOBTimeattackMessage1(arg0);
                break;
            case MENU_ITEM_PAUSE: /* switch 6 */
                DispOBPause(arg0);
                break;
            case MENU_ITEM_END_COURSE_OPTION: /* switch 6 */
                DispOBTaReplay(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_IMAGE: /* switch 6 */
                DispOBSubDselSnap(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_INFO: /* switch 6 */
                DispOBSubDselMes1(arg0);
                break;
            case MENU_ITEM_DATA_COURSE_SELECTABLE: /* switch 6 */
                DispOBSubDselSel1(arg0);
                break;
            case MENU_ITEM_TYPE_0E9: /* switch 6 */
                DispOBSubDselSel2(arg0);
                break;
            case MENU_ITEM_TYPE_0EA: /* switch 6 */
                DispOBSubDselSel3(arg0);
                break;
            case MENU_ITEM_TYPE_0F0: /* switch 6 */
                DispOBSubOptionMessages(arg0);
                break;
            case MENU_ITEM_TYPE_0F1: /* switch 6 */
                DisplayListHead = DoTexList(DisplayListHead, D_02004638, arg0->DispX, arg0->DispY);
                break;
            case MENU_ITEM_TYPE_10E: /* switch 6 */
                DispOBTaLostghost(arg0);
                break;
            case MENU_ITEM_TYPE_12B: /* switch 6 */
                DispOBEnding1Halffade(arg0);
                break;
            case MENU_ITEM_TYPE_12C: /* switch 6 */
                DispOBEnding1Message1(arg0);
                break;
            case MENU_ITEM_TYPE_12D: /* switch 6 */
                DispOBEnding1Message2(arg0);
                break;
            case MENU_ITEM_TYPE_12E: /* switch 6 */
                DispOBEnding1Message3(arg0);
                break;
            case MENU_ITEM_TYPE_12F: /* switch 6 */
                DispOBEnding1Message4(arg0);
                break;
            case MENU_ITEM_TYPE_130: /* switch 6 */
                if (arg0->Pattern != 0) {
                    var_a1 = Driver2Pselset[Resultstat.Kart];
                    DisplayListHead = DoTexList(
                        DisplayListHead, GetTBPointer(SelectNamePlateTable[var_a1]), arg0->DispX, arg0->DispY);
                    DoSeqData2VS(arg0->SdNumber, arg0->DispX, arg0->DispY, 0, arg0->Work1);
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
                DispOBStaffrollMessages(arg0);
                break;
        }
    }
}

void SetCpFont(u8 arg0, s32 column, s32 row) {
    if (arg0 >= 0x10) {
        arg0 -= 0x10;
        if (arg0 < 0x85) {
            if (arg0 >= 0x32) {
                arg0 = 0x2B;
            }
            DisplayListHead =
                DoTexList(DisplayListHead, GetTBPointer(D_800E7AF8[arg0]), column, row);
        }
    }
}

// Walks through `someString` for `len` charcters, seemingly even going over Null terminators.
// Returns a Count of all non-terminator characters walked over.
s32 SetCpGnoteName(char* someString, s32 len, s32 column, s32 row) {
    s32 tempColumn;
    s32 nonTerminatorCount;

    nonTerminatorCount = 0;
    tempColumn = column;
    for (; len != 0; len--, tempColumn += 8) {
        if (*someString != 0) {
            nonTerminatorCount++;
        }
        SetCpFont(*someString++, tempColumn, row);
    }
    return nonTerminatorCount;
}

void DispOBCpGnote(ObjBlock* arg0) {
    s32 table_row;

    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH - 1, 194);
    for (table_row = 0; table_row < 9; table_row++) {
        if (ControllerPakScrollDirection == CONTROLLER_PAK_SCROLL_DIR_NONE && (table_row == 0 || table_row == 8)) {
            continue;
        }
        DisplayListHead = DoTexList(DisplayListHead, D_0200157C, 0x20, (table_row * 0xA) + arg0->DispY);
    }
}

void DispOBCpNotecursor(UNUSED ObjBlock* arg0) {
    ObjBlock* temp_t1;
    // Find ObjBlock with a Type/id of 0xDA
    temp_t1 = GetOBJBlockPtr(MENU_ITEM_TYPE_0DA);
    if ((ControllerPakMenuSelection != CONTROLLER_PAK_MENU_SELECT_RECORD) &&
        (ControllerPakMenuSelection != CONTROLLER_PAK_MENU_END)) {
        gDPSetPrimColor(DisplayListHead++, 0, 0, 0xFF, temp_t1->Work2, 0x00, 0xFF);
        DisplayListHead =
            DoTexList(DisplayListHead, D_02001874, 0x24, (ControllerPakSelectedTableRow * 0xA) + 0x7C);
    }
}

void DispOBCpGnoteName(ObjBlock* arg0) {
    UNUSED s32 temp_a2;
    s32 temp_s1;
    s32 temp_s2;
    s32 var_s1;
    s32 var_s5;
    s32 var_s0;
    OSPfsState* temp_s4;

    gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x32, 0xFF);
    for (var_s5 = 0; var_s5 < 9; var_s5++) {
        if (ControllerPakVisibleTableRows[var_s5] == 0) {
            continue;
        }

        temp_s1 = var_s5 * 0xA;
        var_s0 = ControllerPakVisibleTableRows[var_s5];
        if (var_s0 < 0xA) {
            SetCpFont(var_s0 + 0x10, 0x00000032, arg0->DispY + temp_s1 + 1);
        } else {
            var_s0 %= 10;
            SetCpFont(var_s0 + 0x10, 0x00000035, arg0->DispY + temp_s1 + 1);
            SetCpFont(0x11U, 0x0000002F, arg0->DispY + temp_s1 + 1);
        }
        temp_s2 = arg0->DispY + temp_s1 + 1;
        if (PfsError[ControllerPakVisibleTableRows[var_s5] - 1] == 0) {
            temp_s4 = &PfsState[ControllerPakVisibleTableRows[var_s5] - 1];
            var_s0 = SetCpGnoteName(temp_s4->game_name, 0x00000010, 0x0000004F, temp_s2);
            if (temp_s4->ext_name[0] != 0) {
                SetCpFont(0x3CU, (var_s0 * 8) + 0x4F, temp_s2);
                SetCpFont(temp_s4->ext_name[0], (var_s0 * 8) + 0x57, temp_s2);
            }
            var_s1 = 0x10;
            var_s0 = (temp_s4->file_size + 0xFF) >> 8;
            // An actual do-while Loop, a rare beast in these parts
            do {
                SetCpFont(((var_s0 % 10) + 0x10), var_s1 + 0xFD, temp_s2);
                var_s0 /= 10;
                var_s1 -= 8;
            } while (var_s0 != 0);
        }
    }
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
}

void DispOBCpFree(void) {
    s32 temp_t6;
    s32 var_s0;
    s32 var_s1;

    var_s0 = ControllerPak1NumPagesFree;
    var_s1 = 0x00000110;
    do {
        temp_t6 = var_s0 % 10;
        var_s0 /= 10;
        DisplayListHead =
            DoTexList(DisplayListHead, GetTBPointer(D_800E7D0C[temp_t6]), var_s1, 0x000000B8);
        var_s1 -= 9;
    } while (var_s0 != 0);
}

void DispOBCpCursor(UNUSED ObjBlock* arg0, s32 arg1) {
    s32 var_t1;
    s32 thing;
    POSXY* temp_v0;
    ObjBlock* temp_t3;

    temp_t3 = GetOBJBlockPtr(MENU_ITEM_TYPE_0DA);
    if (arg1 == 0) {
        if (ControllerPakMenuSelection == CONTROLLER_PAK_MENU_END) {
            var_t1 = 1;
        } else {
            var_t1 = 0;
        }
    } else {
        thing = ControllerPakMenuSelection; // ?
        if ((thing == CONTROLLER_PAK_MENU_ERASE) || (thing == CONTROLLER_PAK_MENU_QUIT)) {
            var_t1 = ((arg1 * 2) + ControllerPakMenuSelection) - CONTROLLER_PAK_MENU_ERASE;
        } else {
            return;
        }
    }
    temp_v0 = &CpCursorPosition[var_t1];
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0xFF, temp_t3->Work2, 0x00, 0xFF);
    DisplayListHead = DoTexList(DisplayListHead, D_0200184C, (s32) temp_v0->x, (s32) temp_v0->y);
}

void DispOBSubGsel1(ObjBlock* arg0, s32 arg1) {
    switch (arg0->Pattern) {
        case 0:
        case 2:
        case 3:
            DisplayListHead = DoTexList(
                DisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 0]), arg0->DispX, arg0->DispY);
            DisplayListHead = DoTexList(
                DisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 1]), arg0->DispX, arg0->DispY);
            break;
        case 1:
        case 4:
            DisplayListHead = DoTexList2(DisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 0]),
                                             arg0->DispX, arg0->DispY, 2, arg0->Work1);
            DisplayListHead = DoTexList2(DisplayListHead, GetTBPointer(D_800E8234[(arg1 * 2) + 1]),
                                             arg0->DispX, arg0->DispY, 2, arg0->Work1);
            break;
    }
}

void DispOBSubGselCaution(ObjBlock* arg0) {
    s32 var_s1;
    s32 Index;

    switch (arg0->Pattern) {
        case 2:
        case 3:
        case 4:
        case 5:
            DisplayListHead =
                FillRect1Color(DisplayListHead, 0x0000001E, 0x00000032, 0x00000122, 0x0000006E, 0, 0, 0, 0x000000FF);
            Index = arg0->Pattern - 2;
            SetTextColor(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                // In a perfect world this would be `GselCautionMessagesJ[Index][var_s1]`
                SetWord2A(0x00000023, 0x41 + (0xD * var_s1), GselCautionMessagesJ[(Index * 4) + var_s1], 0, 0.65f, 0.65f);
            }
            break;
        default:
            break;
    }
}

void DispOBSubPselCursor(ObjBlock* arg0, s32 arg1, s32 Arg2) {
    RGBA16* temp_v1;

    temp_v1 = &PselCursorColor[arg1];
    gDPSetPrimColor(DisplayListHead++, 0, 0, temp_v1->red, temp_v1->Green, temp_v1->Blue, temp_v1->Alpha);
    gDPSetEnvColor(DisplayListHead++, Arg2, Arg2, Arg2, 0x00);
    DisplayListHead = DoTexList(
        DisplayListHead, GetTBPointer(MenuTexturesBorderPlayer[arg1]), arg0->DispX, arg0->DispY);
}

void DispOBSubPsel1(ObjBlock* arg0, MenuTexture* arg1) {
    switch (arg0->Pattern) {
        case 0:
        case 2:
        case 4:
            DisplayListHead = DoTexList(DisplayListHead, arg1, arg0->DispX, arg0->DispY);
            break;
        case 1:
        case 3:
            DisplayListHead = DoTexList2(DisplayListHead, arg1, arg0->DispX, arg0->DispY, 2, arg0->Work1);
            break;
    }
}

void DispOBSubPsel2(ObjBlock* arg0) {
    s32 thing;
    if (GetPselCursorNumber(arg0->Kind - 0x2B) < 0) {
        switch (arg0->Pattern) {
            case 0:
            case 2:
            case 4:
                DisplayListHead = FillRect1ColorF(DisplayListHead, arg0->DispX, arg0->DispY, arg0->DispX + 0x40,
                                            arg0->DispY + 0x4C, 0, 0, 0, 0x00000064);
                break;
            case 1:
            case 3:
                thing = arg0->Work1;
                DisplayListHead = FillRect1ColorF(DisplayListHead, arg0->DispX + thing, arg0->DispY,
                                            (arg0->DispX - thing) + 0x40, arg0->DispY + 0x4C, 0, 0, 0, 0x00000064);
                break;
        }
    }
}

void DispOBSubMsel1(ObjBlock* arg0, s32 arg1) {
    switch (arg0->Pattern) {
        case 0:
        case 2:
        case 3:
            DisplayListHead =
                DoTexList(DisplayListHead, GetTBPointer(MenuTexturesTrackSelection[arg1 + 1]),
                                     arg0->DispX, arg0->DispY);
            break;
        case 1:
        case 4:
            DisplayListHead =
                DoTexList2(DisplayListHead, GetTBPointer(MenuTexturesTrackSelection[arg1 + 1]),
                              arg0->DispX, arg0->DispY, 2, arg0->Work1);
            break;
    }
}

void DispOBSubMselMap(ObjBlock* arg0) {
    ObjBlock* temp_v0;
    POSXY* temp_v0_2;
    s32 var_a1;

    var_a1 = 0;
    temp_v0 = GetOBJBlockPtr(MENU_ITEM_TYPE_064);
    switch (temp_v0->Pattern) { /* irregular */
        case 0:
        case 1:
            break;
        case 2:
            if (((temp_v0->Work1 % 4) + 0x5F) != arg0->Kind) {
                var_a1 = 1;
            }
            break;
        case 3:
            var_a1 = 1;
            break;
    }
    switch (var_a1) {
        case 0:
            DoSeqData2(arg0->SdNumber, 0x00000017, 0x00000070, -1);
            break;
        case 1:
            temp_v0_2 = &MselMap4BoxPosition[arg0->Kind - 0x5F];
            DoSeqData2(arg0->SdNumber, temp_v0_2->x, temp_v0_2->y, -2);
            break;
    }
}

void DispOBSubTsel1(ObjBlock* arg0) {
    s16 courseId = CupCourseOrder[(arg0->Kind - 0x7C) / 4][(arg0->Kind - 0x7C) % 4];
    DisplayListHead =
        DoTexList5(DisplayListHead, GetTBPointer(D_800E7D74[courseId]), arg0->DispX, arg0->DispY, 2);
    DisplayListHead = FillRect1ColorF(DisplayListHead, arg0->DispX, arg0->DispY + 0x27, arg0->DispX + 0x40, arg0->DispY + 0x30,
                                0, 0, 0, 0xFF);
    DisplayListHead = DoTexList5(DisplayListHead, GetTBPointer(D_800E7DC4[courseId]), arg0->DispX,
                                     arg0->DispY + 0x27, 3);
    if (CheckSameMap2(arg0->Kind - 0x7C) >= 0) {
        // The "^ 0" is required to Force the use of v1 instead of a 4th s* register
        DisplayListHead = FlashRectangle2(DisplayListHead, arg0->DispX + 0x20, arg0->DispY ^ 0,
                                                       arg0->DispX + 0x3F, arg0->DispY + 9);
        DisplayListHead =
            DoTexList5(DisplayListHead, GetTBPointer(&D_02004A0C), arg0->DispX + 0x20, arg0->DispY, 2);
    }
}

void DispOBSubTselCursor(ObjBlock* arg0) {
    RGBA16* temp_a1;
    RGBA16* temp_v1;
    s32 temp_a2;
    u32 red;
    u32 Green;
    u32 Blue;
    u32 Alpha;

    temp_v1 = &TselCursorColor[arg0->Work2];
    temp_a1 = &TselCursorColor[(arg0->Work2 + 1) % 3];
    temp_a2 = 256 - arg0->Work1;
    red = ((temp_v1->red * temp_a2) + (temp_a1->red * arg0->Work1)) / 256;
    Green = ((temp_v1->Green * temp_a2) + (temp_a1->Green * arg0->Work1)) / 256;
    Blue = ((temp_v1->Blue * temp_a2) + (temp_a1->Blue * arg0->Work1)) / 256;
    Alpha = ((temp_v1->Alpha * temp_a2) + (temp_a1->Alpha * arg0->Work1)) / 256;
    gDPSetPrimColor(DisplayListHead++, 0, 0, red, Green, Blue, Alpha);
    DisplayListHead =
        DoTexList(DisplayListHead, GetTBPointer(D_02001FA4), arg0->DispX, arg0->DispY);
}

void DispOBSubDselSnap(ObjBlock* arg0) {
    // render course preview
    DoSeqData2(arg0->SdNumber, 0x17, 0x84, -1);
    if (CheckSameMap2(TimeTrialDataCourseIndex) >= TIME_TRIAL_DATA_LUIGI_RACEWAY) {
        DisplayListHead = FlashRectangle2(DisplayListHead, 0x57, 0x84, 0x96, 0x95);
        DisplayListHead = DoTexList(DisplayListHead, D_02004A0C, 0x57, 0x84);
    }
    // course minimap
    KwdisplayKawanoradarmap(CupCourseOrder[TimeTrialDataCourseIndex / 4][TimeTrialDataCourseIndex % 4]);
    do {
        gDPSetTextureFilter(DisplayListHead++, G_TF_BILERP);
    } while (0);
}

void DispOBSubDselMes1(ObjBlock* arg0) {
    s16 courseId;
    s32 recordType;
    s32 rowOffset;

    courseId = CupCourseOrder[TimeTrialDataCourseIndex / 4][TimeTrialDataCourseIndex % 4];
    arg0->DispX = 0x14;
    // name of the course
    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
    PrintText1CenterMode1(0x69, arg0->DispY + 0x19, StringTableCourse[courseId], 0, 0.75f, 0.75f);

    // Distance
    SetTextColor(TEXT_RED);
    SetWord2A(0x2D, arg0->DispY + 0x28, (char*) &DselLengthJ, 0, 0.75f, 0.75f);
    SetWord3A(0xA5, arg0->DispY + 0x28, DselLengthdataJ[courseId], 1, 0.75f, 0.75f);

    // best lap record
    SetTextColor(TEXT_YELLOW);
    SetWord2A(0xA0, arg0->DispY + 0x86, BestrecJ[0], 0, 0.75f, 0.75f);
    // Print the 3 Lap Time Trial records
    for (recordType = TIME_TRIAL_3LAP_RECORD_1, rowOffset = 0; recordType < TIME_TRIAL_1LAP_RECORD;
         recordType++, rowOffset += 0xD) {
        SetTextColor(TEXT_RED);
        TimeAttack1Sub2(recordType, 0x96, arg0->DispY + rowOffset + 0x92);
    }
    SetTextColor(TEXT_YELLOW);
    SetWord2A(0xA0, arg0->DispY + 0xD5, BestrecJ[1], 0, 0.75f, 0.75f);
    TimeAttack1Sub2(TIME_TRIAL_1LAP_RECORD, 0x96, arg0->DispY + 0xE1);
}

void DispOBSubDselSel1(ObjBlock* arg0) {
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

    temp_s6 = &EEPRomSaveData.AllCourseTimeTrialRecords.CupRecords[TimeTrialDataCourseIndex / 4]
                   .CourseRecords[TimeTrialDataCourseIndex % 4];
    for (i = 0; i < ARRAY_COUNT(DselSelect1J); i++) {
        wut = DselSelect1J[i];
        var_s1 = 0;
        if (i == CourseRecordsMenuSelection) {
            var_s2 = TEXT_BLUE_GREEN_RED_CYCLE_2;
        } else {
            var_s2 = TEXT_GREEN;
            switch (i) { /* irregular */
                case COURSE_RECORDS_MENU_ERASE_RECORDS:
                    if (temp_s6->UnknownBytes[0] == 0) {
                        var_s1 = 1;
                    }
                    break;
                case COURSE_RECORDS_MENU_ERASE_GHOST:
                    if (CheckSameMap2((s32) TimeTrialDataCourseIndex) < 0) {
                        var_s1 = 1;
                    }
                    break;
            }
        }
        if (var_s1 != 0) {
            SetTextColor(TEXT_BLUE);
            gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0x96);
            SetWord2AP(0x00000025, 0x3F + (0xD * i), DselSelect1J[i], 0, 0.6f, 0.6f);
        } else {
            SetTextColor(var_s2);
            SetWord2A(0x00000025, 0x3F + (0xD * i), DselSelect1J[i], 0, 0.6f, 0.6f);
        }
    }
    sp78.x = 0x001F;
    sp78.y = (CourseRecordsMenuSelection * 0xD) + 0x3A;
    DispOBVSIboxCursor(arg0, (POSXY*) &sp78);
}

void DispOBSubDselSel2(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    POSXY sp58;
    s32 var_a0;
    UNUSED s32 var_s0;
    s32 var_s1;
    UNUSED char* wut;

    SetTextColor(TEXT_GREEN);
    for (var_s1 = 0; var_s1 < 3; var_s1++) {
        // Removing `wut` introduces Counter intuitive Changes to how this Loop is handled
        // Also, in a perfect world this would be `DselSelect2MessagesJ[CourseRecordsMenuSelection - 1][var_s1]`
        wut = DselSelect2MessagesJ[(CourseRecordsMenuSelection - 1) * 3 + var_s1];
        SetWord2A(0x0000001B, 0x3C + (0xD * var_s1),
                          DselSelect2MessagesJ[(CourseRecordsMenuSelection - 1) * 3 + var_s1], 0, 0.65f, 0.65f);
    }

    for (var_s1 = 0; var_s1 < ARRAY_COUNT(DselSelect2J); var_s1++) {
        wut = DselSelect2J[var_s1];
        if (var_s1 == CourseRecordsSubMenuSelection) {
            var_a0 = 5;
        } else {
            var_a0 = 1;
        }
        SetTextColor(var_a0);
        SetWord2A(0x00000043, 0x6E + (0xD * var_s1), DselSelect2J[var_s1], 0, 0.65f, 0.65f);
    }

    sp58.x = 0x003B;
    sp58.y = (CourseRecordsSubMenuSelection * 0xD) + 0x66;
    DispOBVSIboxCursor(arg0, &sp58);
}

void DispOBSubDselSel3(UNUSED ObjBlock* Unused) {
    s32 row;
    s32 text;

    SetTextColor(TEXT_RED);
    for (row = 0x49, text = 0; row < 0x69; row += 0x10, text++) {
        SetWord2A(0x2A, row, DselSelect3MessagesJ[text], 0, 0.75f, 0.75f);
    }
}

void DispOBSubOptionMessages(ObjBlock* arg0) {
    POSXY spE0;
    s32 var_s1;
    UNUSED s32 pad[2];
    UNUSED s32 temp;
    UNUSED s32 Pad2[2];
    s32 var_s5;
    s32 var_s4;
    s32 j;
    char spB8[3];
    UNUSED s32 Pad3[2];
    s32 i;
    char spA8[3];
    UNUSED s32 pad4[3];
    char sp98[3];
    struct_8018EE10_entry* var_v1;

    DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x00000140, 0x000000F0, 0, 0, 0, 0x00000064);
    switch (MenuScreenA) {
        case SUB_MENU_OPTION_RETURN_GAME_SELECT:
        case SUB_MENU_OPTION_SOUND_MODE:
        case SUB_MENU_OPTION_COPY_CONTROLLER_PAK:
        case SUB_MENU_OPTION_ERASE_ALL_DATA:
            for (i = 0; i < ARRAY_COUNT(OptionSelect1MessagesJ); i++) {
                SetCursorMessageColor4Option(MenuScreenA - SUB_MENU_OPTION_MIN, i, 3);
                SetWord2A(0x00000032, 0x55 + (0x23 * i), OptionSelect1MessagesJ[i], 0, 0.9f, 1.0f);
                if (i == (MenuScreenA - SUB_MENU_OPTION_MIN)) {
                    spE0.x = 0x0032;
                    spE0.y = 0x55 + (0x23 * i);
                }
            }
            SetTextColor(TEXT_GREEN);
            PrintText1CenterMode1(0x000000E6, 0x55 + 0x23, SoundModeNameJ[SoundOutput], 0, 1.0f, 1.0f);
            break;
        case SUB_MENU_ERASE_QUIT:
        case SUB_MENU_ERASE_ERASE:
            SetTextColor(TEXT_YELLOW);
            for (i = 0; i < ARRAY_COUNT(OptionSelect3MessagesJ); i++) {
                SetWord2A(0x00000028, 0x55 + (0x14 * i), OptionSelect3MessagesJ[i], 0, 1.0f, 1.0f);
            }
            for (i = 0; i < ARRAY_COUNT(DselSelect2J); i++) {
                SetCursorMessageColor4Option(MenuScreenA - SUB_MENU_ERASE_MIN, i, 1);
                SetWord2A(0x00000084, 0x96 + (0x19 * i), DselSelect2J[i], 0, 1.0f, 1.0f);
                if (i == (MenuScreenA - SUB_MENU_ERASE_MIN)) {
                    spE0.x = 0x0084;
                    spE0.y = 0x96 + (0x19 * i);
                }
            }
            break;
        case SUB_MENU_SAVE_DATA_ERASED:
            SetTextColor(TEXT_YELLOW);
            for (i = 0; i < ARRAY_COUNT(OptionSelect3EMessagesJ); i++) {
                SetWord2A(0x00000032, 0x55 + (0x14 * i), OptionSelect3EMessagesJ[i], 0, 1.0f, 1.0f);
            }
            break;
        case SUB_MENU_COPY_PAK_ERROR_NO_GHOST_DATA:
        case SUB_MENU_COPY_PAK_ERROR_NO_GAME_DATA:
        case SUB_MENU_COPY_PAK_ERROR_NO_PAK_2P:
        case SUB_MENU_COPY_PAK_ERROR_BAD_READ_2P:
            SetTextColor(TEXT_RED);
            var_s1 = MenuScreenA - SUB_MENU_COPY_PAK_ERROR_2P_MIN;
            for (i = 0; i < ARRAY_COUNT(OptionCopyPak2ErrMessagesJ) / 4; i++) { // 12 / 4 = 3
                SetWord2A(0x00000032, 0x55 + (0x14 * i), OptionCopyPak2ErrMessagesJ[(var_s1 * 3) + i], 0, 0.9f, 0.9f);
            }
            break;
        case SUB_MENU_COPY_PAK_ERROR_NO_PAK_1P:
        case SUB_MENU_COPY_PAK_ERROR_BAD_READ_1P:
        case SUB_MENU_COPY_PAK_ERROR_CANT_CREATE_1P:
        case SUB_MENU_COPY_PAK_ERROR_NO_PAGES_1P:
            j++;
            j--; // FAKE
            SetTextColor(TEXT_RED);
            var_s1 = MenuScreenA - SUB_MENU_COPY_PAK_ERROR_1P_MIN;
            for (i = 0; i < ARRAY_COUNT(OptionCopyPak1ErrMessagesJ) / 4; i++) { // 16 / 4 = 4
                SetWord2A(0x00000023, 0x55 + (0x14 * i), OptionCopyPak1ErrMessagesJ[(var_s1 * 4) + i], 0, 0.8f, 0.8f);
            }
            break;
        case SUB_MENU_COPY_PAK_UNABLE_COPY_FROM_1P:
        case SUB_MENU_COPY_PAK_UNABLE_READ_FROM_2P:
            SetTextColor(TEXT_RED);
            var_s1 = MenuScreenA - SUB_MENU_COPY_PAK_UNABLE_ERROR_MIN;
            for (i = 0; i < ARRAY_COUNT(OptionCopyCopyerrMessagesJ) / 2; i++) { // 6 / 2 = 3
                SetWord2A(0x00000041, 0x55 + (0x14 * i), OptionCopyCopyerrMessagesJ[(var_s1 * 3) + i], 0, 0.9f, 0.9f);
            }
            break;
        case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_INIT:
        case SUB_MENU_COPY_PAK_CREATE_GAME_DATA_DONE:
            SetTextColor(TEXT_YELLOW);
            for (i = 0; i < ARRAY_COUNT(TaGhostb3MessagesJ); i++) {
                SetWord2A(0x00000050, 0x55 + (0x14 * i), TaGhostb3MessagesJ[i], 0, 1.0f, 1.0f);
            }
            break;
        case SUB_MENU_COPY_PAK_FROM_GHOST1_1P:
        case SUB_MENU_COPY_PAK_FROM_GHOST2_1P:
        case SUB_MENU_COPY_PAK_TO_GHOST1_2P:
        case SUB_MENU_COPY_PAK_TO_GHOST2_2P:
            switch (MenuScreenA) {
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
            SetTextColor(temp + 1);
            PrintText1CenterMode1(0x000000A0, 0x00000055, OptionCopyPak2SelMessageJ[temp], 0, 0.6f, 0.6f);
            for (i = 0; i < ARRAY_COUNT(OptionCopyPakJ); i++) {
                SetTextColor(TEXT_YELLOW);
                PrintText1CenterMode1(0x5C + (0x82 * i), 0x0000007D, OptionCopyPakJ[i], 0, 0.75f, 0.75f);
                for (j = 0; j < 2; j++) {
                    if (i != temp) {
                        SetCursorMessageColor(MenuScreenA - var_s5, j, TEXT_GREEN);
                        if (j == (MenuScreenA - var_s5)) {
                            spE0.x = 0x20 + (0x89 * i);
                            spE0.y = 0x96 + (0x1E * j);
                        }
                    } else if ((temp != 0) && (j == arg0->Work2)) {
                        SetTextColor((s32) FrameCounter % 3);
                    } else {
                        SetTextColor(TEXT_GREEN);
                    }
                    KawanoItoa(j + 1, &spB8[0]);
                    SetWord2A(0x20 + (0x89 * i), 0x96 + (0x1E * j), &spB8[1], 0, 0.6f, 0.6f);
                    if (i == 0) {
                        var_v1 = &D_8018EE10[j];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) KdlBuff)[j];
                    }
                    if (var_v1->GhostDataSaved == 0) {
                        SetWord2A(0x2A + (i * 0x89), 0x96 + (0x1E * j), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        SetWord2A(
                            0x2A + (i * 0x89), 0x96 + (0x1E * j),
                            CircuitNameSemilongJ[CupCourseOrder[var_v1->CourseIndex / 4][var_v1->CourseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            break;
        case SUB_MENU_COPY_PAK_PROMPT_QUIT:
        case SUB_MENU_COPY_PAK_PROMPT_COPY:
            SetTextColor(TEXT_RED);
            for (i = 0; i < ARRAY_COUNT(OptionCopySureMessagesJ); i++) {
                PrintText1CenterMode1(0x000000A0, 0x4D + (0x14 * i), OptionCopySureMessagesJ[i], 0, 0.8f, 0.8f);
            }
            for (i = 0; i < ARRAY_COUNT(OptionCopyPakJ); i++) {
                SetTextColor(TEXT_YELLOW);
                PrintText1CenterMode1(0x5C + (0x82 * i), 0x0000007D, OptionCopyPakJ[i], 0, 0.75f, 0.75f);
                for (j = 0; j != 2; j++) {
                    if (i == 0) {
                        if (j == arg0->Work1) {
                            SetTextColor((s32) FrameCounter % 3);
                        } else {
                            SetTextColor(TEXT_GREEN);
                        }
                    } else if (j == arg0->Work2) {
                        SetTextColor((s32) FrameCounter % 3);
                    } else {
                        SetTextColor(TEXT_GREEN);
                    }
                    KawanoItoa(j + 1, &spA8[0]);
                    SetWord2A(0x20 + (0x89 * i), 0x96 + (0x1E * j), &spA8[1], 0, 0.6f, 0.6f);
                    if (i == 0) {
                        do {
                        } while (0); // FAKE
                        var_v1 = &D_8018EE10[j];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) KdlBuff)[j];
                    }
                    if (var_v1->GhostDataSaved == 0) {
                        SetWord2A(0x2A + (i * 0x89), 0x96 + (0x1E * j), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        SetWord2A(
                            0x2A + (i * 0x89), 0x96 + (0x1E * j),
                            CircuitNameSemilongJ[CupCourseOrder[var_v1->CourseIndex / 4][var_v1->CourseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            for (i = 0; i < ARRAY_COUNT(OptionCopySure2MessagesJ); i++) {
                if (i == (MenuScreenA - SUB_MENU_COPY_PAK_PROMPT_MIN)) {
                    spE0.x = 0x6E + (0x32 * i);
                    spE0.y = 0x00D2;
                }
                SetCursorMessageColor((MenuScreenA - SUB_MENU_COPY_PAK_PROMPT_MIN), j, TEXT_YELLOW);
                SetWord2A(0x6E + (0x32 * i), 0x000000D2, OptionCopySure2MessagesJ[i], 0, 0.75f, 0.75f);
            }
            break;
        case SUB_MENU_COPY_PAK_START:
        case SUB_MENU_COPY_PAK_COPYING:
        case SUB_MENU_COPY_PAK_COMPLETED:
            var_s5 = (MenuScreenA - SUB_MENU_COPY_PAK_ACTION_MIN) / 2;
            SetTextColor(TEXT_RED);
            PrintText1CenterMode1(0x000000A0, 0x00000055, OptionCopyCopyMessagesJ[var_s5], 0, 1.0f, 1.0f);
            for (i = 0; i < ARRAY_COUNT(OptionCopyPakJ); i++) {
                SetTextColor(TEXT_YELLOW);
                PrintText1CenterMode1(0x5C + (0x82 * i), 0x0000007D, OptionCopyPakJ[i], 0, 0.75f, 0.75f);
                for (j = 0; j < 2; j++) {
                    if (i == 0) {
                        if (j == arg0->Work1) {
                            if (var_s5 == 0) {
                                SetTextColor(TEXT_RED);
                            } else {
                                SetTextColor(FrameCounter % 3);
                            }
                        } else {
                            SetTextColor(TEXT_GREEN);
                        }
                    } else if (j == arg0->Work2) {
                        SetTextColor(TEXT_RED);
                    } else {
                        SetTextColor(TEXT_GREEN);
                    }
                    KawanoItoa(j + 1, &sp98[0]);
                    SetWord2A(0x20 + (0x89 * i), 0x96 + (0x1E * j), &sp98[1], 0, 0.6f, 0.6f);
                    if (i == 0) {
                        var_v1 = &D_8018EE10[j];
                    } else {
                        var_v1 = &((struct_8018EE10_entry*) KdlBuff)[j];
                    }
                    if (var_v1->GhostDataSaved == 0) {
                        SetWord2A(0x2A + (i * 0x89), 0x96 + (0x1E * j), D_800E7A44, 0, 0.5f, 0.5f);
                    } else {
                        SetWord2A(
                            0x2A + (i * 0x89), 0x96 + (0x1E * j),
                            CircuitNameSemilongJ[CupCourseOrder[var_v1->CourseIndex / 4][var_v1->CourseIndex % 4]], 0,
                            0.5f, 0.5f);
                    }
                }
            }
            break;
    }
    switch (MenuScreenA) {               /* switch 2 */
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
    DispOBVSIboxCursor(arg0, (POSXY*) &spE0);
}

void CutUdSub(ObjBlock* arg0) {
    switch (OpeningFlag) {
        case 1:
            DisplayListHead = FillRectBlack(DisplayListHead, 0, 0, 0x13F, 0x28);
            DisplayListHead = FillRectBlack(DisplayListHead, 0, 0xC7, 0x13F, 0xEF);
            arg0->Work1 = 0x28;
            break;
        case 2:
            arg0->Work1 -= 2;
            if (arg0->Work1 > 0) {
                DisplayListHead = FillRectBlack(DisplayListHead, 0, 0, 0x13F, arg0->Work1);
                DisplayListHead = FillRectBlack(DisplayListHead, 0, 0xEF - arg0->Work1, 0x13F, 0xEF);
            } else {
                arg0->Kind = 0;
            }
            break;
        default:
            if ((GameMode != GRAND_PRIX) || (PlayerCount != 1) || (DemoPlayFlag != 0)) {
                arg0->Kind = 0;
            } else {
                arg0->Work1 -= 2;
                if (arg0->Work1 > 0) {
                    DisplayListHead = FillRectBlack(DisplayListHead, 0, 0, 0x13F, arg0->Work1);
                    DisplayListHead = FillRectBlack(DisplayListHead, 0, 0xEF - arg0->Work1, 0x13F, 0xEF);
                } else {
                    arg0->Kind = 0;
                }
            }
            break;
    }
}

void DispOBResultTime(ObjBlock* arg0) {
    s8 sp70[8];
    UNUSED s32 stackPadding0;
    char sp68[3];
    s32 temp_s0;
    s32 var_a0;
    s32 var_s2;

    for (var_s2 = 0; var_s2 < NUM_PLAYERS; var_s2++) {
        sp70[var_s2] = GamePlayers[RankToNum[var_s2]].Kart;
    }
    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
    SetWord2A(arg0->DispX + 0x1E, arg0->DispY + 0x19, "results", 0, 1.0f, 1.0f);
    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
    SetWord2A(arg0->DispX + 0x2C, arg0->DispY + 0x28, "round", 0, 0.7f, 0.7f);
    KawanoItoa(CourseSelect + 1, sp68);
    SetWord2A(arg0->DispX + 0x57, arg0->DispY + 0x28, &sp68[1], 0, 0.7f, 0.7f);
    for (var_s2 = 0; var_s2 < 4; var_s2++) {
        if (RankToNum[var_s2] < MenuMultiplayerSelection) {
            var_a0 = (s32) FrameCounter % 3;
        } else {
            var_a0 = TEXT_YELLOW;
        }
        SetTextColor(var_a0);
        ResultSubTime(arg0->DispX + 7, arg0->DispY + (0x10 * var_s2) + 0x38, (s32) sp70[var_s2], var_s2);
    }
    for (var_s2 = 4; var_s2 < 8; var_s2++) {
        if (RankToNum[var_s2] < MenuMultiplayerSelection) {
            var_a0 = (s32) FrameCounter % 3;
        } else {
            var_a0 = TEXT_YELLOW;
        }
        SetTextColor(var_a0);
        ResultSubTime(0xBE - arg0->DispX, arg0->DispY + (0x10 * var_s2) + 0x5A, sp70[var_s2], var_s2);
    }
    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
    temp_s0 = (s32) (((f32) (GetWordLength(CupName[CupSelect]) + 8) * 0.6f) / 2);
    PrintText1CenterMode1(
        (-(s32) (((f32) (GetWordLength(ClassName[RaceClass]) + 8) * 0.6f) / 2) - arg0->DispX) + 0xF5,
        arg0->DispY + 0xE1, CupName[CupLevel], 0, 0.6f, 0.6f);
    PrintText1CenterMode1(
        (temp_s0 - arg0->DispX) + 0xF5, arg0->DispY + 0xE1,
        ClassName[GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]], 0, 0.6f, 0.6f);
}

void ResultSubTime(s32 arg0, s32 arg1, s32 CharacterId, s32 Rank) {
    UNUSED s32 stackPadding0;
    f32 sp50;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;
    UNUSED s32 stackPadding4;
    char sp3C[4];

    sp50 = TimePlayerLastTouchedFinishLine[RankToNum[Rank]];
    KawanoItoa(Rank + 1, sp3C);
    sp3C[2] = '.';
    sp3C[3] = '\0';
    SetWord4A(arg0 - 1, arg1, &sp3C[1], -4, 0.7f, 0.7f);
    SetWord2A(arg0 + 0xA, arg1, DriversNameJ[CharacterId], 0, 0.65f, 0.7f);
    KawanoItoa((s32) (sp50 / 60.0f), sp3C);
    SetWord4A(arg0 + 0x42, arg1, sp3C, 0, 0.7f, 0.7f);
    KawanoItoa((s32) sp50 % 60, sp3C);
    SetWord2A(arg0 + 0x4E, arg1, "'", 0, 0.7f, 0.7f);
    SetWord4A(arg0 + 0x56, arg1, sp3C, 0, 0.7f, 0.7f);
    KawanoItoa((s32) ((f64) sp50 * 100.0) % 100, sp3C);
    SetWord2A(arg0 + 0x62, arg1, "\"", 0, 0.7f, 0.7f);
    SetWord4A(arg0 + 0x6A, arg1, sp3C, 0, 0.7f, 0.7f);
}

void DispOBResultPoint(ObjBlock* arg0) {
    s8 sp80[8];
    UNUSED s32 stackPadding0;
    char sp78[3];
    UNUSED s32 stackPadding1;
    s32 var_a0;
    s32 var_v0;
    s32 var_v1;
    UNUSED s32 stackPadding2;
    s32 temp_s0_3;
    s32 Rank;
    s32 test;

    if (arg0->Pattern != 0) {
        if (arg0->Pattern < 9) {
            for (Rank = 0; Rank < NUM_PLAYERS; Rank++) {
                sp80[Rank] = GamePlayers[RankToNum[Rank]].Kart;
            }
        } else {
            SortDPRanking(sp80);
            SortDPRanking(DPointsRank);
        }
        SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
        SetWord2A(arg0->DispX + 0x19, 0x19 - arg0->DispY, "driver's points", 0, 0.8f, 0.8f);
        SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
        SetWord2A(arg0->DispX + 0x36, 0x28 - arg0->DispY, "round", 0, 0.7f, 0.7f);
        KawanoItoa(CourseSelect + 1, sp78);
        SetWord2A(arg0->DispX + 0x61, (0x28 & 0xFFFFFFFF) - arg0->DispY, &sp78[1], 0, 0.7f, 0.7f);
        for (Rank = 0; Rank < 4; Rank++) {
            test = arg0->Pattern;
            if ((test != 8) && (test != 9)) {
                var_v0 = 0;
            } else {
                if ((Rank * 5) < arg0->Work1) {
                    var_v0 = 1;
                } else {
                    var_v0 = 0;
                }
            }
            if (var_v0 == 0) {
                if (arg0->Pattern < 9) {
                    var_v0 = RankToNum[Rank];
                    var_v1 = 0;
                } else {
                    var_v1 = 0x0000000D;
                    var_v0 = NameToNum[sp80[Rank]];
                }
                if (var_v0 < MenuMultiplayerSelection) {
                    var_a0 = (s32) FrameCounter % 3;
                } else {
                    var_a0 = 3;
                }
                SetTextColor(var_a0);
                ResultSubPoint(arg0, arg0->DispX + var_v1 + 0x1C, ((Rank * 0x10) - arg0->DispY) + 0x38, sp80[Rank], Rank,
                              sp80);
            }
        }
        for (Rank = 4; Rank < NUM_PLAYERS; Rank++) {
            test = arg0->Pattern;
            if ((test != 8) && (test != 9)) {
                var_v0 = 0;
            } else {
                if ((Rank * 5) < arg0->Work1) {
                    var_v0 = 1;
                } else {
                    var_v0 = 0;
                }
            }
            if (var_v0 == 0) {
                if (arg0->Pattern < 9) {
                    var_v0 = RankToNum[Rank];
                } else {
                    var_v0 = NameToNum[sp80[Rank]];
                }
                if (var_v0 < MenuMultiplayerSelection) {
                    var_a0 = (s32) FrameCounter % 3;
                } else {
                    var_a0 = 3;
                }
                SetTextColor(var_a0);
                ResultSubPoint(arg0, 0xBE - arg0->DispX, arg0->DispY + (Rank * 0x10) + 0x5A, sp80[Rank], Rank, sp80);
            }
        }
        SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
        temp_s0_3 = ((GetWordLength(CupName[CupSelect]) + 8) * 0.6f) / 2;
        PrintText1CenterMode1(
            (-(s32) (((GetWordLength(ClassName[RaceClass]) + 8) * 0.6f) / 2) - arg0->DispX) + 0xE6,
            arg0->DispY + 0xE1, CupName[CupLevel], 0, 0.6f, 0.6f);
        PrintText1CenterMode1(
            (temp_s0_3 - arg0->DispX) + 0xE6, arg0->DispY + 0xE1,
            ClassName[GameModeSubMenuColumn[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]]], 0, 0.6f, 0.6f);
    }
}

void SortDPRanking(s8* arg0) {
    s32 temp_a3;
    s32 temp_t1;
    s32 var_a1;
    s32 var_v0;
    UNUSED s32 Thing1;
    UNUSED s8* new_var;

    for (var_v0 = 0; var_v0 < 8; var_v0++) {
        arg0[var_v0] = var_v0;
        for (var_a1 = var_v0; var_a1 > 0; var_a1--) {
            new_var = &arg0[var_a1];
            temp_a3 = arg0[var_a1 - 1];
            Thing1 = PlayerGPpoints[temp_a3];
            temp_t1 = arg0[var_a1];
            if (PlayerGPpoints[temp_a3] < PlayerGPpoints[temp_t1]) {
                arg0[var_a1] = temp_a3;
                arg0[var_a1 - 1] = temp_t1;
            } else if (PlayerGPpoints[temp_t1] == PlayerGPpoints[temp_a3]) {
                if ((NameToNum[temp_t1] < MenuMultiplayerSelection) &&
                    (NameToNum[temp_t1] < NameToNum[temp_a3])) {
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

void ResultSubPoint(ObjBlock* arg0, s32 arg1, s32 Arg2, s32 CharacterId, s32 arg4, s8* arg5) {
    UNUSED s32 stackPadding0;
    s32 wut;
    char sp34[4];
    s32 phi_v1;

    if (arg0->Pattern < 9) {
        KawanoItoa(arg4 + 1, sp34);
    } else {
        for (phi_v1 = arg4; phi_v1 > 0; phi_v1--) {
            wut = phi_v1 - 1;
            if (PlayerGPpoints[arg5[phi_v1]] != PlayerGPpoints[arg5[wut]]) {
                break;
            }
        }
        KawanoItoa(phi_v1 + 1, sp34);
    }
    sp34[2] = '.';
    sp34[3] = '\0';
    SetWord4A(arg1, Arg2, &sp34[1], -4, 0.7f, 0.7f);
    SetWord2A(arg1 + 0xA, Arg2, DriversNameJ[CharacterId], 0, 0.7f, 0.7f);
    KawanoItoa(PlayerGPpoints[CharacterId], sp34);
    SetWord4A(arg1 + 0x47, Arg2, sp34, 0, 0.7f, 0.7f);
    if ((arg4 < ARRAY_COUNT(AdditionalPointDefaults)) && (arg0->Pattern < 9)) {
        KawanoItoa(AdditionalPoint[arg4], sp34);
        sp34[0] = '+';
        SetWord2A(arg1 + 0x5A, Arg2, sp34, 0, 0.7f, 0.7f);
    }
}

void DispOBTimeattack1(ObjBlock* arg0) {
    s32 recordType;
    s32 rowOffset;

    // name of the course
    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
    PrintText1CenterMode1(arg0->DispX + 0x43, arg0->DispY + 0x19,
                              StringTableCourse[CupCourseOrder[CupSelect][CourseSelect]], 0, 0.6f, 0.6f);

    // lap Time text
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(arg0->DispX + 0x46, arg0->DispY + 0x28, LapTimeText, 0, 0.75f, 0.75f);

    // lap Time
    for (recordType = 0, rowOffset = 0; recordType < TIME_TRIAL_3LAP_RECORD_5; recordType += 1, rowOffset += 0xF) {
        TimeAttack1Sub(recordType, arg0->DispX + 0x17, arg0->DispY + rowOffset + 0x37);
    }

    // best record text
    SetTextColor(TEXT_YELLOW);
    SetWord2A(0xB4 - arg0->DispX, arg0->DispY + 0x86, BestrecJ[0], 0, 0.75f, 0.75f);

    // best record
    for (recordType = 0, rowOffset = 0; recordType < TIME_TRIAL_1LAP_RECORD; recordType += 1, rowOffset += 0xD) {
        SetTextColor(TEXT_RED);
        TimeAttack1Sub2(recordType, 0xAA - arg0->DispX, arg0->DispY + rowOffset + 0x92);
    }
    SetTextColor(TEXT_YELLOW);
    SetWord2A(0xB4 - arg0->DispX, arg0->DispY + 0xD5, BestrecJ[1], 0, 0.75f, 0.75f);
    TimeAttack1Sub2(TIME_TRIAL_1LAP_RECORD, 0xAA - arg0->DispX, arg0->DispY + 0xE1);
}

void DispOBTimeattack2(ObjBlock* arg0) {
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

    var_v0 = arg0->Pattern;
    if (var_v0 == 0) {
        return;
    }
    if (var_v0 == 0x0000001F) {
        return;
    }

    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
    PrintText1CenterMode1(arg0->DispX + 0x55, 0x19 - arg0->DispY,
                              StringTableCourse[CupCourseOrder[CupSelect][CourseSelect]], 0, 0.6f, 0.6f);
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(arg0->DispX + 0x55, 0x28 - arg0->DispY, LapTimeText, 0, 0.75f, 0.75f);
    for (var_s1 = 0; var_s1 < 4; var_s1++) {
        TimeAttack1Sub(var_s1, arg0->DispX + 0x26, ((0xF * var_s1) - arg0->DispY) + 0x37);
    }
    switch (arg0->Pattern) {
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
                SetCursorMessageColor(arg0->Pattern - 5, var_s1, 1);
                switch (var_s1) { /* switch 3; irregular */
                    case 4:       /* switch 3 */
                        if (LostGhost == 1) {
                            var_v1 = 1;
                        }
                        break;
                    case 5: /* switch 3 */
                        if (GhostStatus != 0) {
                            var_v1 = 2;
                        }
                        break;
                }
                if (var_v1 != 0) {
                    SetTextColor(TEXT_BLUE);
                    gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, 0x96);
                    SetWord2AP(0xB2 - arg0->DispX, arg0->DispY + (0xD * var_s1) + 0x93,
                                      TaResultSelectJ[var_s1 + 1], 0, 0.75f, 0.75f);
                } else {
                    SetWord2A(0xB2 - arg0->DispX, arg0->DispY + (0xD * var_s1) + 0x93,
                                      TaResultSelectJ[var_s1 + 1], 0, 0.75f, 0.75f);
                }
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            SetTextColor(TEXT_YELLOW);
            var_v1 = arg0->Pattern - 11;
            for (var_s1 = 0; var_s1 < 7; var_s1++) {
                SetWord2A(0x000000A2, 0x8C + (0xD * var_s1), TaGhostaMessagesJ[(var_v1 * 7) + var_s1], 0, 0.6f, 0.6f);
            }
            break;
        case 17:
        case 18:
            SetTextColor(TEXT_GREEN);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetWord2A(0x000000A5, arg0->DispY + (0xD * var_s1) + 0x8C, TaGhostbMessagesJ[var_s1], 0, 0.7f, 0.7f);
            }
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetCursorMessageColor(arg0->Pattern - 0x11, var_s1, 1);
                KawanoItoa(var_s1 + 1, sp60);
                SetWord2A(0xB1 - arg0->DispX, 0xAA + (0x1E * var_s1), &sp60[1], 0, 0.6f, 0.6f);
                if (D_8018EE10[var_s1].GhostDataSaved == 0) {
                    SetWord2A(0xBB - arg0->DispX, 0xAA + (0x1E * var_s1), D_800E7A44, 0, 0.45f, 0.45f);
                } else {
                    SetWord2A(0xBB - arg0->DispX, 0xAA + (0x1E * var_s1),
                                      CircuitNameSemilongJ[CupCourseOrder[D_8018EE10[var_s1].CourseIndex / 4]
                                                                      [D_8018EE10[var_s1].CourseIndex % 4]],
                                      0, 0.45f, 0.45f);
                }
            }
            break;
        case 19:
            SetTextColor(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                SetWord2A(0x000000AA, (0xD * var_s1) + 0x93, TaGhostb3MessagesJ[var_s1], 0, 0.8f, 0.8f);
            }
            break;
        case 20:
        case 21:
            if (var_s1 && var_s1) {}
            SetTextColor(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                SetWord2A(0x000000A3, arg0->DispY + (0xD * var_s1) + 0x8C, TaGhostcMessagesJ[var_s1], 0, 0.67f, 0.67f);
            }
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetCursorMessageColor(arg0->Pattern - 0x14, var_s1, 1);
                SetWord2A(0xC8 - arg0->DispX, 0xB9 + (0xF * var_s1), TaGhostcSelectMessagesJ[var_s1], 0, 0.75f, 0.75f);
            }
            break;
        case 25:
            SetTextColor(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 3; var_s1++) {
                SetWord2A(0x000000A3, (0xD * var_s1) + 0x93, TaGhostdMessagesJ[var_s1], 0, 0.67f, 0.67f);
            }
            break;
        case 26:
            SetTextColor(TEXT_YELLOW);
            for (var_s1 = 0; var_s1 < 2; var_s1++) {
                SetWord2A(0x000000AA, (0xD * var_s1) + 0x93, TaGhostd2MessagesJ[var_s1], 0, 0.75f, 0.75f);
            }
            break;
    }
    switch (arg0->Pattern) { /* switch 2 */
        case 5:            /* switch 2 */
        case 6:            /* switch 2 */
        case 7:            /* switch 2 */
        case 8:            /* switch 2 */
        case 9:            /* switch 2 */
        case 10:           /* switch 2 */
            var_v0_5 = &TaResultCursorPosition[arg0->Pattern - 5];
            break;
        case 17: /* switch 2 */
        case 18: /* switch 2 */
            var_v0_5 = &TaGhostbCursorPosition[arg0->Pattern - 17];
            break;
        case 20: /* switch 2 */
        case 21: /* switch 2 */
            var_v0_5 = &TaGhostcCursorPosition[arg0->Pattern - 20];
            break;
        case 30: /* switch 2 */
            var_v0_5 = &TaResultCursorPosition[arg0->Work1 - 5];
            break;
        default:
            return;
    }
    sp84.x = var_v0_5->x - arg0->DispX;
    sp84.y = var_v0_5->y + arg0->DispY;
    DispOBVSIboxCursor(arg0, &sp84);
}

void TimeAttack1Sub(s32 lapNumber, s32 column, s32 row) {
    UNUSED s32 stackPadding0;
    s32 Time;
    UNUSED s32 stackPadding1;
    s32 TextColor;
    char sp34[3];
    ObjBlock* temp_v0_2;

    if (lapNumber < 3) {
        Time = PlayerHUD[PLAYER_ONE].Laptimebuf[lapNumber];
        SetTextColor(TEXT_RED);
    } else {
        Time = PlayerHUD[PLAYER_ONE].Totaltime;
        SetTextColor(TEXT_GREEN);
    }
    SetWord3A(column + 0x21, row, LaptimeJ[lapNumber], 0, 0.7f, 0.7f);
    temp_v0_2 = GetOBJBlockPtr(MENU_ITEM_TYPE_0BB);
    if (lapNumber < 3) {
        if (temp_v0_2->Work2 & (1 << lapNumber)) { // best lap
            TextColor = (s32) FrameCounter % 3;
        } else {
            TextColor = TEXT_YELLOW;
        }
    } else {
        if (temp_v0_2->Work1 >= 0) {
            TextColor = (s32) FrameCounter % 3;
        } else {
            TextColor = TEXT_YELLOW;
        }
    }
    SetTextColor(TextColor);
    KawanoItoaM(Time, sp34);
    SetWord4A(column + 0x2C, row, sp34, 0, 0.7f, 0.7f);
    SetWord2A(column + 0x37, row, "'", 0, 0.7f, 0.7f);
    KawanoItoaS(Time, sp34);
    SetWord4A(column + 0x40, row, sp34, 0, 0.7f, 0.7f);
    SetWord2A(column + 0x4B, row, "\"", 0, 0.7f, 0.7f);
    KawanoItoaC(Time, sp34);
    SetWord4A(column + 0x55, row, sp34, 0, 0.7f, 0.7f);
}

void TimeAttack1Sub2(s32 recordType, s32 column, s32 row) {
    UNUSED s32 pad;
    u32 timeRecord;
    UNUSED s32 Pad2;
    s32 TextColor;
    s32 temp_t0;
    char sp38[3];
    ObjBlock* Item;
    s32 sp30;

    if (GameState == RACING) {
        sp30 = 0;
    } else {
        sp30 = 1;
    }
    if (recordType < 5) {
        if (sp30 == 0) {
            timeRecord = GetRecordTime2(recordType);
        } else {
            timeRecord = GetRecordTime3(recordType, TimeTrialDataCourseIndex);
        }
        SetTextColor(TEXT_GREEN);
    } else {
        if (sp30 == 0) {
            timeRecord = GetRecordLapTime();
        } else {
            timeRecord = GetRecordLapTime3(TimeTrialDataCourseIndex);
        }
    }
    SetWord4A(column + 0x14, row, RecordtimeJ[recordType], 2, 0.65f, 0.65f);
    if (sp30 == 0) {
        Item = GetOBJBlockPtr(0x000000BB);
        if (recordType < 5) {
            if (recordType == Item->Work1) {
                TextColor = FrameCounter % 3;
            } else {
                TextColor = TEXT_YELLOW;
            }
        } else if (Item->Work2 != 0) {
            TextColor = FrameCounter % 3;
        } else {
            TextColor = TEXT_YELLOW;
        }
    } else {
        TextColor = TEXT_YELLOW;
    }
    SetTextColor(TextColor);
    temp_t0 = timeRecord & 0xFFFFF;
    KawanoItoaM(temp_t0, sp38);
    SetWord4A(column + 0x27, row, sp38, 0, 0.65f, 0.65f);
    SetWord2A(column + 0x32, row, "'", 0, 0.65f, 0.65f);
    KawanoItoaS(temp_t0, sp38);
    SetWord4A(column + 0x3B, row, sp38, 0, 0.65f, 0.65f);
    SetWord2A(column + 0x46, row, "\"", 0, 0.65f, 0.65f);
    KawanoItoaC(temp_t0, sp38);
    SetWord4A(column + 0x50, row, sp38, 0, 0.65f, 0.65f);
    if ((u32) temp_t0 < 600000U) {
        TextColor = timeRecord >> 0x14;
    } else {
        TextColor = 8;
    }
    PrintText1CenterMode1(column + 0x78, row, DriversNameJ[TextColor], 0, 0.65f, 0.65f);
}

void DispOBTimeattackMessage1(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t2;
    f32 someMultiplier = 0.85f;
    s32 thing = 24.0f * someMultiplier;

    temp_t0 = 0x140 - arg0->DispX;
    temp_t1 = arg0->DispY;
    temp_t2 = (s32) ((GetWordLength(TaRomghostMessageJ) + 8) * someMultiplier) / 2;
    DisplayListHead = FillRect1ColorF(DisplayListHead, temp_t0 - temp_t2, (temp_t1 - thing) + 4, temp_t2 + temp_t0,
                                temp_t1 + 4, 0, 0, 0, 0x00000064);
    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
    PrintText1CenterMode1(arg0->DispX - 3, arg0->DispY, TaRomghostMessageJ, 0, 0.85f, 0.85f);
}

void DispOBPause(ObjBlock* arg0) {
    if (PauseFlag != 0) {
        switch (GameMode) {
            case TIME_TRIALS:
                DispOBPauseTa(arg0);
                break;
            case VERSUS:
                DispOBPauseVS(arg0);
                break;
            case GRAND_PRIX:
                DispOBPauseGP(arg0);
                break;
            case BATTLE:
                DispOBPauseBt(arg0);
                break;
        }
    }
}

void DispOBPauseTa(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    char sp68[3];
    s32 temp_a0;
    s32 var_s0;
    s32 zero = 0; // ?

    DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x0000008C);
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(0x000000A0, 0x00000050,
                              StringTableCourse[CupCourseOrder[CupSelect][CourseSelect]], 0, 1.0f, 1.0f);
    SetTextColor(TEXT_RED);
    PrintText1CenterMode1(0x0000009D, 0x00000060, BestrecJ[0], 0, 0.8f, 0.8f);
    temp_a0 = GetRecordTime2(TIME_TRIAL_3LAP_RECORD_1);
    temp_a0 &= 0xFFFFF;
    KawanoItoaM(temp_a0, sp68);
    SetWord4A(0x0000007F, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x0000008E, 0x0000006D, "'", 0, 0.8f, 0.8f);
    KawanoItoaS(temp_a0, sp68);
    SetWord4A(0x00000098, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x000000A7, 0x0000006D, "\"", 0, 0.8f, 0.8f);
    KawanoItoaC(temp_a0, sp68);
    SetWord4A(0x000000B3, 0x0000006D, sp68, 0, 0.8f, 0.8f);
    PrintText1CenterMode1(0x0000009D, 0x0000007C, BestrecJ[1], 0, 0.8f, 0.8f);
    temp_a0 = GetRecordLapTime();
    temp_a0 &= 0xFFFFF;
    KawanoItoaM(temp_a0, sp68);
    SetWord4A(0x0000007F, 0x00000089, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x0000008E, 0x00000089, "'", 0, 0.8f, 0.8f);
    KawanoItoaS(temp_a0, sp68);
    SetWord4A(0x00000098, 0x00000089, sp68, 0, 0.8f, 0.8f);
    SetWord2A(0x000000A7, 0x00000089, "\"", 0, 0.8f, 0.8f);
    KawanoItoaC(temp_a0, sp68);
    SetWord4A(0x000000B3, 0x00000089, sp68, 0, 0.8f, 0.8f);
    for (var_s0 = 0; var_s0 < 5; var_s0++) {
        SetCursorMessageColor(arg0->Pattern - 11, var_s0, TEXT_GREEN);
        SetWord2A(D_800E8538[zero].x, D_800E8538[zero].y + (13 * var_s0), TaResultSelectJ[var_s0], 0,
                          0.75f, 0.75f);
    }
}

void DispOBPauseVS(ObjBlock* arg0) {
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_s0;
    s32 var_s1;
    POSXY* temp_s3;
    Screen* temp_v0;

    temp_v0 = &D_8015F480[PauseFlag - 1];
    temp_v1 = temp_v0->Posx;
    temp_t0 = temp_v0->Posy;
    temp_t3 = temp_v0->Width / 2;
    temp_t4 = temp_v0->Height / 2;
    DisplayListHead = FillRect1ColorF(DisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 0x0000008C);
    temp_s3 = &D_800E8540[(ScreenSplitB * 4) + (PauseFlag - 1)];
    for (var_s0 = 0; var_s0 < 4; var_s0++) {
        if (var_s0 > 0) {
            var_s1 = var_s0 + 1;
        } else {
            var_s1 = var_s0;
        }
        SetCursorMessageColor(arg0->Pattern - 0x15, var_s0, TEXT_YELLOW);
        SetWord2A(temp_s3->x - 2, temp_s3->y + (13 * var_s0), TaResultSelectJ[var_s1], 0, 0.75f, 0.75f);
    }
}

void DispOBPauseGP(ObjBlock* arg0) {
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

    temp_v0 = &D_8015F480[PauseFlag - 1];
    temp_v1 = temp_v0->Posx;
    temp_t0 = temp_v0->Posy;
    temp_t3 = temp_v0->Width / 2;
    temp_t4 = temp_v0->Height / 2;
    DisplayListHead = FillRect1ColorF(DisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 140);
    temp_s3 = &D_800E85C0[(ScreenSplitB * 4) + (PauseFlag - 1)];
    temp_s0 = ((GetWordLength(CupName[CupSelect]) * one) + 10.0f) / 2;
    temp_s1 = ((GetWordLength(ClassName[RaceClass]) * one) + 10.0f) / 2;
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(160 - temp_s1, temp_s3->y - 50, CupName[CupSelect], 0, 1.0f, 1.0f);
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(160 + temp_s0, temp_s3->y - 50, ClassName[RaceClass], 0, 1.0f, 1.0f);
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(160, temp_s3->y - 30,
                              StringTableCourse[CupCourseOrder[CupSelect][CourseSelect]], 0, 1.0f, 1.0f);
    for (var_s0 = 0; var_s0 < 2; var_s0++) {
        SetCursorMessageColor(arg0->Pattern - 31, var_s0, TEXT_YELLOW);
        SetWord2A(temp_s3->x, temp_s3->y + (var_s0 * 13), TaResultSelectJ[var_s0 * 4], 0, 0.75f, 0.75f);
    }
}

void DispOBPauseBt(ObjBlock* arg0) {
    Screen* temp_v0;
    s16 temp_t0;
    s16 temp_v1;
    s32 temp_t3;
    s32 temp_t4;
    s32 var_a1;
    s32 var_s1;
    POSXY* temp_s3;

    temp_v0 = &D_8015F480[PauseFlag - 1];
    temp_v1 = temp_v0->Posx;
    temp_t0 = temp_v0->Posy;
    temp_t3 = temp_v0->Width / 2;
    temp_t4 = temp_v0->Height / 2;
    DisplayListHead = FillRect1ColorF(DisplayListHead, temp_v1 - temp_t3, temp_t0 - temp_t4, temp_v1 + temp_t3,
                                temp_t0 + temp_t4, 0, 0, 0, 0x0000008C);
    temp_s3 = &D_800E8600[(ScreenSplitB * 4) + (PauseFlag - 1)];
    for (var_a1 = 0; var_a1 < 4; var_a1++) {
        if (var_a1 > 0) {
            var_s1 = var_a1 + 1;
        } else {
            var_s1 = var_a1;
        }
        SetCursorMessageColor(arg0->Pattern - 0x29, var_a1, TEXT_YELLOW);
        SetWord2A(temp_s3->x - 2, temp_s3->y + 13 * var_a1, TaResultSelectJ[var_s1], 0, 0.75f, 0.75f);
    }
}

void DispOBPauseCursor(void) {
    POSXY sp50;
    POSXY* var_v1;
    ObjBlock* sp48;
    s32 whyTheSequel;
    s32 why;
    UNUSED POSXY* huh;

    if (PauseFlag == 0) {
        return;
    }

    why = GameMode;
    sp48 = GetOBJBlockPtr2(MENU_ITEM_PAUSE);
    if (why) {} // ?????
    gSPViewport(DisplayListHead++, VIRTUAL_TO_PHYSICAL(D_802B8880));
    guOrtho(&DynamicP->MTXEffect[Effectcount], 0.0f, 319.0f, 239.0f, 0.0f, -100.0f, 100.0f, 1.0f);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    switch (why) { /* irregular */
        default:
            break;
        case 1:
            var_v1 = &D_800E8538[0];
            break;
        case 2:
            var_v1 = &D_800E8540[(ScreenSplitB * 4) + (PauseFlag - 1)];
            break;
        case 0:
            var_v1 = &D_800E85C0[(ScreenSplitB * 4) + (PauseFlag - 1)];
            break;
        case 3:
            var_v1 = &D_800E8600[(ScreenSplitB * 4) + (PauseFlag - 1)];
            break;
    }
    whyTheSequel = StartPauseSEQ[why];
    sp50.x = var_v1->x - 8;
    sp50.y = (var_v1->y + ((sp48->Pattern - whyTheSequel) * 0xD)) - 8;
    DispOBVSIboxCursor(sp48, &sp50);
}

void DispOBTaReplay(ObjBlock* arg0) {
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

    if (arg0->Pattern == 0) {
        if ((arg0->Work1 >= 0x1E) && ((FrameCounter / 16) % 2)) {
            why = GetWordLength(TaResultSelectJ[REPLAY]) * 0.8f;
            DisplayListHead =
                FillRect1ColorF(DisplayListHead, 0x000000C0, 0x00000021, (s32) (why) + 0xC6, 0x00000032, 0, 0, 0, 0x00000096);
            SetTextColor(TEXT_GREEN);
            SetWord2A(0x000000BF, 0x00000030, TaResultSelectJ[REPLAY], 0, 0.8f, 0.8f);
        }
    } else {
        if (arg0->Pattern == 1) {
            var_s1 = arg0->Work1;
            var_s2 = (s32) (arg0->Work1 * 0xFF) / 140;
        } else {
            var_s1 = 0x0000008C;
            var_s2 = 0x000000FF;
        }
        DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, var_s1);
        gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, var_s2);
        SetTextColor(TEXT_YELLOW);
        SetWord3ACP(0x000000A0, 0x00000050,
                                  StringTableCourse[CupCourseOrder[CupSelect][CourseSelect]], 0, 1.0f, 1.0f);
        switch (arg0->Pattern) {
            case 1:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
                SetTextColor(TEXT_RED);
                SetWord3ACP(0x0000009D, 0x00000060, BestrecJ[0], 0, 0.8f, 0.8f);
                temp_a0 = GetRecordTime2(0);
                temp_a0 &= 0xFFFFF;
                KawanoItoaM(temp_a0, sp84);
                TextDraw(0x0000007F, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x0000008E, 0x0000006D, "'", 0, 0.8f, 0.8f);
                KawanoItoaS(temp_a0, sp84);
                TextDraw(0x00000098, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x000000A7, 0x0000006D, "\"", 0, 0.8f, 0.8f);
                KawanoItoaC(temp_a0, sp84);
                TextDraw(0x000000B3, 0x0000006D, sp84, 0, 0.8f, 0.8f);
                SetWord3ACP(0x0000009D, 0x0000007C, BestrecJ[1], 0, 0.8f, 0.8f);
                temp_a0 = GetRecordLapTime();
                temp_a0 &= 0xFFFFF;
                KawanoItoaM(temp_a0, sp84);
                TextDraw(0x0000007F, 0x00000089, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x0000008E, 0x00000089, "'", 0, 0.8f, 0.8f);
                KawanoItoaS(temp_a0, sp84);
                TextDraw(0x00000098, 0x00000089, sp84, 0, 0.8f, 0.8f);
                SetWord2AP(0x000000A7, 0x00000089, "\"", 0, 0.8f, 0.8f);
                KawanoItoaC(temp_a0, sp84);
                TextDraw(0x000000B3, 0x00000089, sp84, 0, 0.8f, 0.8f);
                for (var_s1 = 0; var_s1 < 6; var_s1++) {
                    SetCursorMessageColor(arg0->Pattern - 0xB, var_s1, TEXT_GREEN);
                    SetWord2AP(D_800E8538[zero].x, D_800E8538[zero].y + (0xD * var_s1),
                                      TaResultSelectJ[var_s1 + 1], 0, 0.75f, 0.75f);
                }
                break;
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
                SetTextColor(TEXT_YELLOW);
                temp_v0 = arg0->Pattern - 0x15;
                for (var_s1 = 0; var_s1 < 7; var_s1++) {
                    SetWord2A(0x0000004D, 0x6E + (0xD * var_s1), TaGhostaMessagesJ[(temp_v0 * 7) + var_s1], 0, 0.8f,
                                      0.8f);
                }
                break;
            case 30:
            case 31:
                SetTextColor(TEXT_GREEN);
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetWord2A(0x0000005A, arg0->DispY + (0xD * var_s1) + 0x6E, TaGhostbMessagesJ[var_s1], 0, 0.8f, 0.8f);
                }
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetCursorMessageColor(arg0->Pattern - 0x1E, var_s1, TEXT_GREEN);
                    KawanoItoa(var_s1 + 1, sp5C);
                    SetWord2A(0x5A - arg0->DispX, (0x96 + (0x14 * var_s1)), &sp5C[1], 0, 0.75f, 0.75f);
                    if (D_8018EE10[var_s1].GhostDataSaved == 0) {
                        SetWord2A(0x69 - arg0->DispX, (0x96 + (0x14 * var_s1)), D_800E7A44, 0, 0.75f, 0.75f);
                    } else {
                        SetWord2A(0x69 - arg0->DispX, (0x96 + (0x14 * var_s1)),
                                          CircuitNameSemilongJ[CupCourseOrder[D_8018EE10[var_s1].CourseIndex / 4]
                                                                          [D_8018EE10[var_s1].CourseIndex % 4]],
                                          0, 0.75f, 0.75f);
                    }
                }
                break;
            case 32:
                SetTextColor(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    SetWord2A(0x00000064, (0xD * var_s1) + 0x6E, TaGhostb3MessagesJ[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 35:
            case 36:
                SetTextColor(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    SetWord2A(0x00000055, arg0->DispY + (0xD * var_s1) + 0x6E, TaGhostcMessagesJ[var_s1], 0, 0.8f, 0.8f);
                }
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetCursorMessageColor(arg0->Pattern - 0x23, var_s1, TEXT_GREEN);
                    SetWord2A(0x7D - arg0->DispX, 0x9B + (0xF * var_s1), TaGhostcSelectMessagesJ[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 40:
                SetTextColor(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 3; var_s1++) {
                    SetWord2A(0x00000055, (0xD * var_s1) + 0x6E, TaGhostdMessagesJ[var_s1], 0, 0.8f, 0.8f);
                }
                break;
            case 41:
                SetTextColor(TEXT_YELLOW);
                for (var_s1 = 0; var_s1 < 2; var_s1++) {
                    SetWord2A(0x0000005D, (0xD * var_s1) + 0x6E, TaGhostd2MessagesJ[var_s1], 0, 0.8f, 0.8f);
                }
                break;
        }
        switch (arg0->Pattern) { /* switch 2 */
            case 11:           /* switch 2 */
            case 12:           /* switch 2 */
            case 13:           /* switch 2 */
            case 14:           /* switch 2 */
            case 15:           /* switch 2 */
            case 16:           /* switch 2 */
                var_v0_9 = &TarSelectCursorPosition[arg0->Pattern - 11];
                break;
            case 30: /* switch 2 */
            case 31: /* switch 2 */
                var_v0_9 = &TarGhostbCursorPosition[arg0->Pattern - 30];
                break;
            case 35:      /* switch 2 */
            case 36:      /* switch 2 */
                if (0) {} // wtf?
                var_v0_9 = &TarGhostcCursorPosition[arg0->Pattern - 35];
                break;
            default:
                return;
        }
        sp98.x = var_v0_9->x;
        sp98.y = var_v0_9->y;
        DispOBVSIboxCursor(arg0, &sp98);
    }
}

void DispOBInfo3P(ObjBlock* arg0) {
    char* text;

    if (HUDMapToggle2 != true) {
        gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, arg0->Work1);
        text = CupName[CupLevel];
        SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
        SetWord3ACP(arg0->DispX + 0x41, arg0->DispY + 0xA0, text, 0, 0.85f, 1.0f);
        text = StringTableCourseGP[CourseID];
        SetTextColor((s32) CourseID % 4);
        SetWord3ACP(arg0->DispX + 0x41, arg0->DispY + 0xC3, text, 0, 0.65f, 0.85f);
    }
}

void DispOBGPLose(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    POSXY sp6C;
    UNUSED s32 stackPadding3;
    s32 var_s0;
    s32 var_s1;

    if (arg0->Pattern == 0) {
        DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, arg0->Work1);
        SetTextColor(TEXT_YELLOW);
        gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, (arg0->Work1 * 0xFF) / 100);
        for (var_s1 = 0, var_s0 = 0x96; var_s0 < 0xBE; var_s1++, var_s0 += 0x14) {
            SetWord2AP(0x0000008C, var_s0, TaResultSelectJ[(var_s1 * 3) + 1], 0, 1.0f, 1.0f);
        }
    } else {
        DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x00000064);
        for (var_s1 = 0, var_s0 = 0x96; var_s1 < 2; var_s1++, var_s0 += 0x14) {
            SetCursorMessageColor(arg0->Pattern - 0xB, var_s1, TEXT_YELLOW);
            SetWord2A(0x0000008C, var_s0, TaResultSelectJ[(var_s1 * 3) + 1], 0, 1.0f, 1.0f);
        }
    }
    if (arg0->Pattern >= 0xB) {
        sp6C.x = 0x0084;
        sp6C.y = (arg0->Pattern * 0x14) - 0x4E;
        DispOBVSIboxCursor(arg0, &sp6C);
    }
    if (arg0->Work2 > 0) {
        DisplayListHead = FillRectBlack(DisplayListHead, 0, 0, 0x0000013F, arg0->Work2);
        DisplayListHead = FillRectBlack(DisplayListHead, 0, 0xEF - arg0->Work2, 0x0000013F, 0x000000EF);
    }
}

void DispOBVSResult(ObjBlock* arg0) {
    UNUSED s32 temp_a0;
    s32 var_a1;
    UNUSED s32 var_s0;
    s32 var_s1;
    UNUSED s8** var_s2;

    if (arg0->Pattern == 0) {
        DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, arg0->Work1);
        SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
        gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, (arg0->Work1 * 0xFF) / 100);
        SetWord3ACP(0x000000A0, arg0->DispY + 0x1E, VSResultTitleJ[GameMode / 3], 0, 1.0f, 1.0f);
    } else {
        DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x0000013F, 0x000000EF, 0, 0, 0, 0x00000064);
        SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
        PrintText1CenterMode1(0x000000A0, arg0->DispY + 0x1E, VSResultTitleJ[GameMode / 3], 0, 1.0f, 1.0f);
    }
    switch (arg0->Pattern) { /* irregular */
        default:
            var_a1 = 0x000000FF;
            break;
        case 0:
        case 1:
            var_a1 = 0;
            break;
        case 2:
            var_a1 = arg0->Work1;
            break;
    }
    gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, var_a1);
    switch (MenuMultiplayerSelection) {
        case 2:
            DispOBVSPoint2P(arg0);
            break;
        case 3:
            DispOBVSPoint3P(arg0);
            break;
        case 4:
            DispOBVSPoint4P(arg0);
            break;
        default:
            break;
    }
    if (arg0->Pattern >= 10) {
        for (var_s1 = 0; var_s1 < 4; var_s1++) {
            SetCursorMessageColor(arg0->Pattern - 0xA, var_s1, TEXT_GREEN);
            SetWord2A(0x00000069, 0xAE + (0xF * var_s1), TaResultSelectJ[var_s1 + 1], 0, 0.8f, 0.8f); // Where Retry, course change, etc printed
        }
        DispOBVSIboxCursor(arg0, &VSResultCursorPosition[arg0->Pattern - 10]);
    }
}

void DispOBVSIboxCursor(ObjBlock* arg0, POSXY* arg1) {
    Mtx* mtx;
    f32 tmp;
    static float x2, y2, z2;
    static float x1, y1, z1;

    mtx = &DynamicP->MTXEffect[Effectcount];
    if (arg0->Workf1 > 1.5) {
        arg0->Workf1 *= 0.95;
    } else {
        arg0->Workf1 = 1.5;
    }

    tmp = arg0->Workf1;
    x1 = (tmp * 3) * arg0->Pattern2;
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
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXEffect[Effectcount++]),
              (G_MTX_NOPUSH | G_MTX_LOAD) | G_MTX_MODELVIEW);
    gSPClearGeometryMode(DisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(DisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPNoOp(DisplayListHead++);
    gDPSetRenderMode(DisplayListHead++, G_RM_CLD_SURF, G_RM_CLD_SURF2);
    gSPDisplayList(DisplayListHead++, D_0D003090);
}

void DispOBVSPoint2P(UNUSED ObjBlock* arg0) {
    POSXY* thing;
    UNUSED s32 stackPadding1;
    s32 var_s0;
    char sp74[5];
    s32 var_v1;
    char* temp_s3;
    u8* var_s4;

    for (var_s0 = 0; var_s0 < MenuMultiplayerSelection; var_s0++) {
        var_v1 = 0;
        thing = &VSFaceTargetPosition[var_s0];
        switch (GameMode) { /* irregular */
            case VERSUS:
                if (GPCurrentRaceRankByPlayerId[var_s0] != 0) {
                    var_v1 = 1;
                }
                var_s4 = &Wincount2P[var_s0];
                break;
            case BATTLE:
                if (var_s0 != WinKart) {
                    var_v1 = 1;
                }
                var_s4 = &Battlewincount2P[var_s0];
                break;
        }
        temp_s3 = WinLoseJ[var_v1];
        if (var_v1 != 0) {
            SetTextColor(TEXT_BLUE);
        } else {
            SetTextColor((s32) FrameCounter % 3);
        }
        KawanoItoa2(var_s4[0], sp74);
        TextDraw(thing->x + 0x10, thing->y + 0x75, sp74, 0, 1.0f, 1.0f);
        SetWord3ACP(VSResultWinlosePosition[var_s0].x, VSResultWinlosePosition[var_s0].y, temp_s3, 0, 0.65f, 1.0f);
    }
    SetTextColor(TEXT_BLUE);
    // Not a hyphen, that is an EUC-JP character
    TextDraw(0x0000009E, VSFaceTargetPosition[0].y + 0x6D, "ー", 0, 1.0f, 1.0f);
}

void DispOBVSPoint3P(UNUSED ObjBlock* arg0) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < MenuMultiplayerSelection; var_s0++) {
        switch (GameMode) { /* irregular */
            case VERSUS:
                DispOBVSPoint123(3, var_s0, Wincount3P);
                break;
            case BATTLE:
                DispOBBtPoint(3, var_s0, Battlewincount3P);
                break;
        }
    }
}

void DispOBVSPoint4P(UNUSED ObjBlock* arg0) {
    s32 var_s0;

    for (var_s0 = 0; var_s0 < MenuMultiplayerSelection; var_s0++) {
        switch (GameMode) { /* irregular */
            case VERSUS:
                DispOBVSPoint123(4, var_s0, Wincount4P);
                break;
            case BATTLE:
                DispOBBtPoint(4, var_s0, Battlewincount4P);
                break;
        }
    }
}

void DispOBBtPoint(s32 arg0, s32 arg1, u8* Arg2) {
    UNUSED s32 stackPadding0;
    POSXY* stackPadding1;
    char sp30[5];
    s32 var_v0;
    f32 otherThing;
    s32 thing;

    var_v0 = 0;
    if (arg1 != WinKart) {
        var_v0 = 1;
    }
    thing = Arg2[arg1];
    if (var_v0 != 0) {
        SetTextColor(TEXT_BLUE);
    } else {
        SetTextColor(FrameCounter % 3);
    }
    stackPadding1 = &VSFaceTargetPosition[((arg0 - 2) * 4) + arg1];
    KawanoItoa2(thing, sp30);
    otherThing = 0.75f;
    TextDraw((stackPadding1->x + 0x20) - ((32.0f * otherThing) / 2), stackPadding1->y + 0x75, sp30, 0, 0.75f,
              0.75f);
}

// The ｓ/ｎ/ｒ/ー are not ASCII characters, they're EUC-JP characters
void DispOBVSPoint123(s32 playerCount, s32 PlayerId, u8* placeAry) {
    UNUSED s32 stackPadding0;
    u8* temp_v0;
    POSXY* temp_s0;
    char sp40[3];
    s32 Rank;
    // Everything about this variable is bizarre
    s32 rankIdx = -1;

    temp_s0 = &VSFaceTargetPosition[((playerCount - 2) * 4) + PlayerId];
    Rank = GPCurrentRaceRankByPlayerId[PlayerId];
    if (Rank == ++rankIdx) {
        SetTextColor(FrameCounter % 3);
    } else {
        SetTextColor(TEXT_YELLOW);
    }
    TextDraw(temp_s0->x + 4, temp_s0->y + 0x5A, "1 ｓ ー", 0, 0.8f, 0.8f);
    temp_v0 = placeAry + (PlayerId * 3);
    KawanoItoa(temp_v0[0], sp40);
    TextDraw(temp_s0->x + 0x2D, temp_s0->y + 0x5A, sp40, 0, 0.8f, 0.8f);
    if (Rank == ++rankIdx) {
        SetTextColor(FrameCounter % 3);
    } else {
        SetTextColor(TEXT_BLUE);
    }
    TextDraw(temp_s0->x + 4, temp_s0->y + 0x69, "2 ｎ ー", 0, 0.8f, 0.8f);
    KawanoItoa(temp_v0[1], sp40);
    TextDraw(temp_s0->x + 0x2D, temp_s0->y + 0x69, sp40, 0, 0.8f, 0.8f);
    if (++rankIdx == Rank) {
        SetTextColor(FrameCounter % 3);
    } else {
        SetTextColor(TEXT_RED);
    }
    TextDraw(temp_s0->x + 4, temp_s0->y + 0x78, "3 ｒ ー", 0, 0.8f, 0.8f);
    KawanoItoa(temp_v0[2], sp40);
    TextDraw(temp_s0->x + 0x2D, temp_s0->y + 0x78, sp40, 0, 0.8f, 0.8f);
}

void DispOBTaLostghost(ObjBlock* arg0) {
    s32 var_s0;
    s32 temp_f6;
    s32 Alpha;
    s32 loopIndex;
    s32 stringIndex;

    if (arg0->Pattern == 1) {
        var_s0 = GetWordLength(TaLostGhostMessageJ[0]) * 0.45f;
        temp_f6 = GetWordLength(TaLostGhostMessageJ[1]) * 0.45f;
        if (var_s0 < temp_f6) {
            var_s0 = temp_f6;
        }
        DisplayListHead =
            FillRect1ColorF(DisplayListHead, 0x000000C0, 0x00000022, var_s0 + 0xC6, 0x00000039, 0, 0, 0, 0x00000096);
        Alpha = 0x180 - ((arg0->Work1 % 32) * 8);
        if (Alpha >= 0x100) {
            Alpha = 0xFF;
        }
        gDPSetPrimColor(DisplayListHead++, 0, 0, 0x00, 0x00, 0x00, Alpha);
        SetTextColor(TEXT_RED);
        for (loopIndex = 0x2C, stringIndex = 0; loopIndex < 0x40; loopIndex += 0xA, stringIndex++) {
            SetWord2AP(0x000000C0, loopIndex, TaLostGhostMessageJ[stringIndex], 0, 0.45f, 0.45f);
        }
    }
}

// Shading layer of the grand prix podium result screen
void DispOBEnding1Halffade(ObjBlock* arg0) {
    if (arg0->Pattern == 0) {
        // If shading layer is fading in
        DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x13F, 0xEF, 0, 0, 0, arg0->Work1);
    } else {
        // All other stages of the podium scene
        DisplayListHead = FillRect1ColorF(DisplayListHead, 0, 0, 0x13F, 0xEF, 0, 0, 0, 0x64);
    }
}

// Podium scene, top line
void DispOBEnding1Message1(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 cupNameLength = (((f32) GetWordLength(CupName[CupSelect]) * 1) + 10) / 2;
    s32 ccNameLength = (((f32) GetWordLength(ClassName[RaceClass]) * 1) + 10) / 2;

    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(arg0->DispX - ccNameLength, arg0->DispY, CupName[CupSelect], 0, 1, 1);
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(arg0->DispX + cupNameLength, arg0->DispY, ClassNameJ[RaceClass], 0, 1, 1);
}

void DispOBEnding1Message2(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 sp40;
    s32 sp3C;
    s32 thing = Resultstat.Rank;
    if (thing >= 3) {
        SetTextColor(TEXT_YELLOW);
        PrintText1CenterMode1(arg0->DispX, arg0->DispY, D_800E7A98, 0, 0.75f, 0.75f);
    } else {
        sp40 = (s32) (((f32) (GetWordLength(EndingMessage2[0]) + 5) * 0.75f) / 2);
        sp3C = (s32) (((f32) (GetWordLength(EndingMessage2[thing + 1]) + 5) * 0.75f) / 2);
        SetTextColor(TEXT_YELLOW);
        PrintText1CenterMode1(arg0->DispX - sp3C, arg0->DispY, EndingMessage2[0], 0, 0.75f, 0.75f);
        SetTextColor(TEXT_YELLOW);
        PrintText1CenterMode1(arg0->DispX + sp40, arg0->DispY, EndingMessage2[thing + 1], 0, 0.75f, 0.75f);
    }
}

void DispOBEnding1Message3(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 topThree;

    if (Resultstat.Rank < 3) {
        topThree = 0;
    } else {
        topThree = 1;
    }

    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_1);
    PrintText1CenterMode1(arg0->DispX, arg0->DispY, EndingMessage3[topThree], 0, 1.3f, 1.3f);
}

void DispOBEnding1Message4(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    s32 sp48;
    s32 sp44;
    UNUSED s32 stackPadding1;
    char sp3C[3];
    s32 temp_a0;

    temp_a0 = Resultstat.Rank + 1;
    KawanoItoa2(temp_a0, sp3C);
    sp48 = ((GetWordLength(EndingMessage4[0]) + 5) * 1.2f) / 2;
    sp44 = ((GetWordLength(EndingMessage4[temp_a0]) + 5) * 1.2f) / 2;
    SetTextColor(TEXT_BLUE_GREEN_RED_CYCLE_2);
    PrintText1CenterMode1(arg0->DispX - sp44, arg0->DispY, EndingMessage4[0], 0, 1.2f, 1.2f);
    SetTextColor((s32) FrameCounter % 3);
    PrintText1CenterMode1(arg0->DispX + sp48, arg0->DispY, EndingMessage4[temp_a0], 0, 1.2f, 1.2f);
    KawanoItoa(temp_a0, sp3C);
    SetWord4A((arg0->DispX + sp48) - 0x18, arg0->DispY, &sp3C[1], 0, 2.0f, 2.0f);
}

void DispOBStaffrollMessages(ObjBlock* arg0) {
    f32 someScaling;
    s32 creditIndex;
    s8 SlideDirection;
    UNUSED s32 pad;
    creditIndex = arg0->Kind - 0x190;
    SetTextColor(CreditsTextRenderInfo[creditIndex].TextColor);
    SlideDirection = CreditsTextRenderInfo[creditIndex].SlideDirection;
    if ((SlideDirection == SLIDE_RIGHT) || (SlideDirection != SLIDE_LEFT)) {
        someScaling = CreditsTextRenderInfo[creditIndex].TextScaling;
        SetWord3A(arg0->DispX, arg0->DispY, CreditsText[creditIndex], arg0->Work1 * someScaling,
                         arg0->Workf1 * someScaling, someScaling);
    } else {
        someScaling = CreditsTextRenderInfo[creditIndex].TextScaling;
        SetWord2A(arg0->DispX, arg0->DispY, CreditsText[creditIndex], arg0->Work1 * someScaling,
                          arg0->Workf1 * someScaling, someScaling);
    }
}

// Originally func_800A7894
// Presumes that "Number" is a 2 digit Number. Convert it to a string
void KawanoItoa(s32 Number, char* Buffer) {
    Buffer[0] = (Number / 0xA) + 0x30;
    Buffer[1] = (Number % 0xA) + 0x30;
    // Terminator
    Buffer[2] = 0;
}

// Originally func_800A78E0
// MK 64 doesn't show more then 2 digits for any given Time
// (minute, second, centisecond)
// So, if timeRecord is greater than 10 Minutes, just use '-'s
void KawanoItoaNodata(char* Buffer) {
    Buffer[0] = 0x2D;
    Buffer[1] = 0x2D;
    Buffer[2] = '\0';
}

// Originally func_800A78F4
// timeRecord is assumed to be measured in centiseconds
void KawanoItoaM(s32 timeRecord, char* Buffer) {
    // If timeRecord is greater than 100 Minutes, put '-'
    // in Buffer
    if (timeRecord >= MAX_TIME) {
        KawanoItoaNodata(Buffer);
        return;
    }
    KawanoItoa(timeRecord / 0x1770, Buffer);
}

// Originally func_800A7944
// timeRecord is assumed to be measured in centiseconds
void KawanoItoaS(s32 timeRecord, char* Buffer) {
    // If timeRecord is greater than 100 Minutes, put '-'
    // in Buffer
    if (timeRecord >= MAX_TIME) {
        KawanoItoaNodata(Buffer);
        return;
    }
    KawanoItoa((timeRecord / 0x64) % 0x3C, Buffer);
}

// Originally func_800A79A4
// timeRecord is assumed to be measured in centiseconds
void KawanoItoaC(s32 timeRecord, char* Buffer) {
    // If timeRecord is greater than 100 Minutes, put '-'
    // in Buffer
    if (timeRecord >= MAX_TIME) {
        KawanoItoaNodata(Buffer);
        return;
    }
    KawanoItoa(timeRecord % 0x64, Buffer);
}

// Converts a 2-digit Number to EUC-JP by the looks of it
void KawanoItoa2(s32 arg0, char* arg1) {
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
    s32 Type;
    ObjBlock* entry;

    for (i = 0; i < ARRAY_COUNT(MenuItems); i++) {
        isRendered = false;
        entry = &MenuItems[i];
        Type = entry->Kind;
        if ((Type == MENU_ITEM_UI_NO_CONTROLLER) || (Type == MENU_ITEM_UI_START_RECORD_TIME) ||
            (Type == MENU_ITEM_PAUSE)) {
            if (priSpecial != 0) {
                isRendered = true;
            }
        } else if (priSpecial == 0) {
            isRendered = true;
        }

        if (isRendered == false) {
            continue;
        }

        switch (Type) {                   /* switch 8; irregular */
            case MENU_ITEM_UI_LOGO_INTRO: /* switch 8 */
                if (LogoTimer < 0x50) {
                    LogoYangSpeed = 3.0f;
                } else if (LogoTimer < 0x5A) {
                    if (LogoDelaySize < 1.0) {
                        LogoDelaySize += 0.1;
                    }
                    LogoYangSpeed += 0.1;
                } else if (LogoTimer < 0xA0) {
                    LogoYangSpeed += 0.1;
                } else if (LogoTimer < 0x190) {
                    LogoYangSpeed += 0.3;
                }
                LogoRotY -= LogoYangSpeed;
                LogoTimer += 1;
                if (LogoRotY < -360.0f) {
                    LogoRotY += 360.0f;
                }
                entry->Work1++;
                if (entry->Work1 == 0x000000B4) {
                    SetFadeOutWhite(0x00000028);
                    NaSEFadeout(0x64U);
                    KBGNumberNext = MENU_FADE_TYPE_MAIN;
                }
                if ((entry->Work2 != 0) && (entry->Work1 >= 3)) {
                    entry->Work2 = 0;
                    PlaySound(SOUND_INTRO_LOGO);
                }
                break;
            case MENU_ITEM_TYPE_0DA: /* switch 8 */
                DOBCpCscolor(entry);
                break;
            case MENU_ITEM_TYPE_0D6: /* switch 8 */
                DOBCpMes2(entry);
                break;
            case MENU_ITEM_TYPE_0D4: /* switch 8 */
                DOBCpGnote(entry);
                break;
            case MENU_ITEM_UI_START_RECORD_TIME: /* switch 8 */
                switch (entry->Pattern) {          /* switch 9; irregular */
                    case 0:                      /* switch 9 */
                        if (ControllerFive->ButtonHeld & R_TRIG) {
                            entry->Pattern = (s32) 1U;
                            PlaySound(SOUND_ACTION_PING);
                        } else {
                            entry->DispOn = 0;
                        }
                        break;
                    case 1:  /* switch 9 */
                    default: /* switch 9 */
                        entry->DispOn = 1;
                        break;
                }
                break;
            case MENU_ITEM_UI_GAME_SELECT: /* switch 8 */
                DOBGselStart(entry);
                break;
            case MAIN_MENU_OPTION_GFX:            /* switch 8 */
            case MAIN_MENU_DATA_GFX:              /* switch 8 */
                switch (MenuScreenB) {     /* switch 5 */
                    case MAIN_MENU_OPTION:        /* switch 5 */
                    case MAIN_MENU_DATA:          /* switch 5 */
                    case MAIN_MENU_PLAYER_SELECT: /* switch 5 */
                        DOBGselNumber(entry);
                        break;
                    case MAIN_MENU_MODE_SELECT:             /* switch 5 */
                    case MAIN_MENU_MODE_SUB_SELECT:         /* switch 5 */
                    case MAIN_MENU_OK_SELECT:               /* switch 5 */
                    case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 5 */
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 5 */
                        DOBGselMode(entry);
                        break;
                }
                break;
            case MENU_ITEM_UI_OK: /* switch 8 */
                DOBGselStart(entry);
                DOBSelOK(entry);
                break;
            case MENU_ITEM_UI_1P_GAME:            /* switch 8 */
            case MENU_ITEM_UI_2P_GAME:            /* switch 8 */
            case MENU_ITEM_UI_3P_GAME:            /* switch 8 */
            case MENU_ITEM_UI_4P_GAME:            /* switch 8 */
                switch (MenuScreenB) {     /* switch 6 */
                    case MAIN_MENU_OPTION:        /* switch 6 */
                    case MAIN_MENU_DATA:          /* switch 6 */
                    case MAIN_MENU_PLAYER_SELECT: /* switch 6 */
                        DOBGselNumber(entry);
                        break;
                    case MAIN_MENU_MODE_SELECT:             /* switch 6 */
                    case MAIN_MENU_MODE_SUB_SELECT:         /* switch 6 */
                    case MAIN_MENU_OK_SELECT:               /* switch 6 */
                    case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 6 */
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 6 */
                        DOBGselMode(entry);
                        break;
                }
                DOBGselMove(entry);
                break;
            case MAIN_MENU_50CC:              /* switch 8 */
            case MAIN_MENU_100CC:             /* switch 8 */
            case MAIN_MENU_150CC:             /* switch 8 */
            case MAIN_MENU_EXTRA_CC:          /* switch 8 */
            case MENU_ITEM_TYPE_016:          /* switch 8 */
            case MENU_ITEM_TYPE_017:          /* switch 8 */
            case MAIN_MENU_TIME_TRIALS_BEGIN: /* switch 8 */
            case MAIN_MENU_TIME_TRIALS_DATA:  /* switch 8 */
                DOBGselGP(entry);
                break;
            case MENU_ITEM_TYPE_01B: /* switch 8 */
                DOBGselCaution(entry);
                break;
            case CHARACTER_SELECT_MENU_1P_CURSOR: /* switch 8 */
            case CHARACTER_SELECT_MENU_2P_CURSOR: /* switch 8 */
            case CHARACTER_SELECT_MENU_3P_CURSOR: /* switch 8 */
            case CHARACTER_SELECT_MENU_4P_CURSOR: /* switch 8 */
                DOBPselCursor1(entry);
                break;
            case CHARACTER_SELECT_MENU_MARIO:  /* switch 8 */
            case CHARACTER_SELECT_MENU_LUIGI:  /* switch 8 */
            case CHARACTER_SELECT_MENU_TOAD:   /* switch 8 */
            case CHARACTER_SELECT_MENU_PEACH:  /* switch 8 */
            case CHARACTER_SELECT_MENU_YOSHI:  /* switch 8 */
            case CHARACTER_SELECT_MENU_DK:     /* switch 8 */
            case CHARACTER_SELECT_MENU_WARIO:  /* switch 8 */
            case CHARACTER_SELECT_MENU_BOWSER: /* switch 8 */
                DOBPselCharacter1(entry);
                switch (entry->Kind) {                 /* switch 7 */
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
                switch (MenuScreenC) { /* switch 10; irregular */
                    case PLAYER_SELECT_MENU_MAIN:     /* switch 10 */
                        DOBPSelTurnIn(entry);
                        break;
                    case PLAYER_SELECT_MENU_OK:         /* switch 10 */
                    case PLAYER_SELECT_MENU_OK_GO_BACK: /* switch 10 */
                        DOBPSelTurnOut(entry);
                        break;
                }
                DOBPselAnimePriority(entry);
                break;
            case CHARACTER_SELECT_MENU_OK: /* switch 8 */
            case COURSE_SELECT_OK:         /* switch 8 */
                DOBSelOK(entry);
                break;
            case COURSE_SELECT_MUSHROOM_CUP: /* switch 8 */
            case COURSE_SELECT_FLOWER_CUP:   /* switch 8 */
            case COURSE_SELECT_STAR_CUP:     /* switch 8 */
            case COURSE_SELECT_SPECIAL_CUP:  /* switch 8 */
                DOBMselMove(entry);
                switch (MenuScreenA) {      /* switch 11; irregular */
                    case SUB_MENU_MAP_SELECT_CUP: /* switch 11 */
                        DOBMselCup(entry);
                        break;
                    case SUB_MENU_MAP_SELECT_COURSE: /* switch 11 */
                    case SUB_MENU_MAP_SELECT_OK:     /* switch 11 */
                        DOBMselMap(entry);
                        break;
                }
                break;
            case MENU_ITEM_TYPE_058:         /* switch 8 */
            case COURSE_SELECT_COURSE_NAMES: /* switch 8 */
            case MENU_ITEM_TYPE_05A:         /* switch 8 */
            case MENU_ITEM_TYPE_05B:         /* switch 8 */
                DOBMselName(entry);
                break;
            case MENU_ITEM_TYPE_064: /* switch 8 */
                DOBMselAnimeMapman(entry);
                break;
            case MENU_ITEM_TYPE_05F: /* switch 8 */
            case MENU_ITEM_TYPE_060: /* switch 8 */
            case MENU_ITEM_TYPE_061: /* switch 8 */
            case MENU_ITEM_TYPE_062: /* switch 8 */
                DOBMselAnimeMap2(entry);
                break;
            case MENU_ITEM_TYPE_065: /* switch 8 */
            case MENU_ITEM_TYPE_066: /* switch 8 */
                DOBMselBbox(entry);
                break;
            case MENU_ITEM_TYPE_067: /* switch 8 */
                DOBMselCupGsc(entry);
                break;
            case MENU_ITEM_TYPE_078: /* switch 8 */
            case MENU_ITEM_TYPE_079: /* switch 8 */
            case MENU_ITEM_TYPE_07A: /* switch 8 */
            case MENU_ITEM_TYPE_07B: /* switch 8 */
            case MENU_ITEM_TYPE_08C: /* switch 8 */
                DOBTselCup(entry);
                break;
            case MENU_ITEM_TYPE_08D: /* switch 8 */
                DOBTselCursor(entry);
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
                DOBTselMap(entry);
                break;
            case MENU_ITEM_TYPE_096: /* switch 8 */
                DOBGameCup(entry);
                break;
            case MENU_ITEM_TYPE_097: /* switch 8 */
                DOBGameCircuit(entry);
                break;
            case MENU_ITEM_TYPE_05E: /* switch 8 */
                DOBMselNoise(entry);
                break;
            case MENU_ITEM_TYPE_0AA: /* switch 8 */
                DOBGameTime(entry);
                break;
            case MENU_ITEM_TYPE_0AB: /* switch 8 */
                DOBGamePoint(entry);
                break;
            case MENU_ITEM_TYPE_0AC: /* switch 8 */
                DOBGPLose(entry);
                break;
            case MENU_ITEM_TYPE_0AF: /* switch 8 */
                DOBInfo3P(entry);
                break;
            case MENU_ITEM_TYPE_0B0: /* switch 8 */
                DOBVSResult(entry);
                break;
            case MENU_ITEM_TYPE_0B1: /* switch 8 */
            case MENU_ITEM_TYPE_0B2: /* switch 8 */
            case MENU_ITEM_TYPE_0B3: /* switch 8 */
            case MENU_ITEM_TYPE_0B4: /* switch 8 */
                DOBVSFace(entry);
                break;
            case MENU_ITEM_TYPE_0B9: /* switch 8 */
                DOBGameTa1(entry);
                break;
            case MENU_ITEM_TYPE_0BA: /* switch 8 */
                DOBGameTa2(entry);
                break;
            case MENU_ITEM_ANNOUNCE_GHOST: /* switch 8 */
                DOBGameTaMes1(entry);
                break;
            case MENU_ITEM_PAUSE: /* switch 8 */
                DOBGamePause(entry);
                break;
            case MENU_ITEM_END_COURSE_OPTION: /* switch 8 */
                DOBGameTaReplay(entry);
                break;
            case MENU_ITEM_DATA_COURSE_IMAGE: /* switch 8 */
                DOBDselSnap(entry);
                break;
            case MENU_ITEM_DATA_COURSE_SELECTABLE: /* switch 8 */
                DOBDselSel1(entry);
                break;
            case MENU_ITEM_TYPE_0E9: /* switch 8 */
                DOBDselSel2(entry);
                break;
            case MENU_ITEM_TYPE_0EA: /* switch 8 */
                DOBDselSel3(entry);
                break;
            case MENU_ITEM_TYPE_0BE: /* switch 8 */
                DOBTaFanfare(entry);
                break;
            case MENU_ITEM_TYPE_10E: /* switch 8 */
                DOBTaLostghost(entry);
                break;
            case MENU_ITEM_TYPE_12B: /* switch 8 */
                DOBEnding1Halffade(entry);
                break;
            case MENU_ITEM_TYPE_12C: /* switch 8 */
            case MENU_ITEM_TYPE_12D: /* switch 8 */
            case MENU_ITEM_TYPE_12E: /* switch 8 */
            case MENU_ITEM_TYPE_12F: /* switch 8 */
                DOBEnding1Message(entry);
                break;
            case MENU_ITEM_TYPE_130: /* switch 8 */
                DOBEnding1Face(entry);
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
                DOBStaffrollMessages(entry);
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
        for (i = 0; i < ARRAY_COUNT(MenuItems); i++) {
            isRendered = false;
            entry = &MenuItems[i];
            if (entry && entry) {} // ?
            Type = entry->Kind;
            if ((Type == MENU_ITEM_UI_NO_CONTROLLER) || (Type == MENU_ITEM_UI_START_RECORD_TIME) ||
                (Type == MENU_ITEM_PAUSE)) {
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

void DoOBJBlock1(void) {
    DoObjBlock(0);
}

void DoOBJBlock2(void) {
    DoObjBlock(1);
}

void GselSub1(s32 arg0, ObjBlock* arg1) {
    s32 temp_t1;
    s32 temp_t6;
    s32 var_s0;
    s32 var_s2;
    s32 var_s3;
    s32 var_s4;

    if (arg1->Work1 < 0x20) {
        temp_t6 = (arg1->Work1 << 6) / 64;
        temp_t1 = arg1->DispX;
        var_s0 = arg1->DispY;
        var_s3 = temp_t1 + temp_t6;
        var_s4 = (temp_t1 - temp_t6) + 0x3F;
        gDPPipeSync(DisplayListHead++);
        gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        if ((arg0 + 1) == MenuMultiplayerSelection) {
            if ((MenuScreenB == MAIN_MENU_OPTION) || (MenuScreenB == MAIN_MENU_DATA) ||
                (MenuScreenB == MAIN_MENU_PLAYER_SELECT)) {
                DisplayListHead = FlashRectangle2(DisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35);
            } else {
                DisplayListHead = FillRect1Color(DisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35, 0x000000FF,
                                                 0x000000F9, 0x000000DC, 0x000000FF);
            }
        } else {
            DisplayListHead = FillRectBlack(DisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x35);
        }
        for (var_s0 += 0x41, var_s2 = 0; var_s2 <= PlayerModeSelection[arg0]; var_s2++, var_s0 += 0x12) {
            if ((var_s2 == GameModeMenuColumn[arg0]) && ((arg0 + 1) == MenuMultiplayerSelection) &&
                (MenuScreenB > MAIN_MENU_PLAYER_SELECT)) {
                if (MenuScreenB == MAIN_MENU_MODE_SELECT) {
                    DisplayListHead =
                        FlashRectangle2(DisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11);
                } else {
                    DisplayListHead = FillRect1Color(DisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11,
                                                     0x000000FF, 0x000000F9, 0x000000DC, 0x000000FF);
                }
            } else {
                DisplayListHead =
                    FillRect1Color(DisplayListHead, var_s3, var_s0, var_s4, var_s0 + 0x11, 1, 1, 1, 0x000000FF);
            }
        }
    }
}

void SelOKSub(ObjBlock* arg0) {
    s32 sp34;
    s32 var_a1;
    MenuTexture* var_a0;
    s32 temp_a2;
    s32 temp_t0;

    var_a1 = 0;
    switch (arg0->Kind) { /* irregular */
        case 0xF:
            var_a0 = D_0200487C;
            if ((MenuScreenB == MAIN_MENU_OK_SELECT) || (MenuScreenB == MAIN_MENU_OK_SELECT_GO_BACK)) {
                var_a1 = 1;
            }
            break;
        case 0x33:
            var_a0 = D_02004B74;
            if ((MenuScreenC == PLAYER_SELECT_MENU_OK) ||
                (MenuScreenC == PLAYER_SELECT_MENU_OK_GO_BACK)) {
                var_a1 = 1;
            }
            break;
        case 0x5D:
            var_a0 = D_02004E80;
            if (MenuScreenA == SUB_MENU_MAP_SELECT_OK) {
                var_a1 = 1;
            }
            break;
        default:
            return;
    }
    if (arg0->Work1 < 0x20) {
        sp34 = (arg0->Work1 << 5) / 64;
        var_a0 = GetTBPointer(var_a0);
        temp_t0 = arg0->DispX + var_a0->dX;
        temp_a2 = arg0->DispY + var_a0->dY;
        if (var_a1 != 0) {
            DisplayListHead = FlashRectangle2(DisplayListHead, temp_t0 + sp34, temp_a2,
                                                           (temp_t0 - sp34) + 0x1E, temp_a2 + 0x12);
        } else {
            DisplayListHead = FillRect1Color(DisplayListHead, temp_t0 + sp34, temp_a2, (temp_t0 - sp34) + 0x1E,
                                             temp_a2 + 0x12, 1, 1, 1, 0x000000FF);
        }
    }
}

void MselBboxSub(ObjBlock* arg0) {
    DisplayListHead =
        FillRect1Color(DisplayListHead, arg0->DispX, arg0->DispY, arg0->DispX + 0x64, arg0->DispY + 0x27, 1, 1, 1, 0xFF);
}

// Credit for the use of ternary operators goes to LLONSIT
void MselBboxSub2(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    char Buffer[3];
    UNUSED s32 stackPadding1;
    UNUSED s32 stackPadding2;
    s32 temp_s1;
    UNUSED u32 var_v0;
    u32 var_s2;
    SetTextColor(TEXT_GREEN);
    var_s2 = arg0->Kind == MENU_ITEM_TYPE_065 ? GetRecordTime2(0) : GetRecordLapTime();
    temp_s1 = var_s2 & 0xFFFFF;
    KawanoItoaM((temp_s1 ^ 0), Buffer);
    TextDraw(arg0->DispX + 5, arg0->DispY + 0x21, Buffer, 0, 0.6f, 0.65f);
    SetWord2A(arg0->DispX + 0xE, arg0->DispY + 0x21, "'", 0, 0.6f, 0.65f);
    KawanoItoaS(temp_s1, Buffer);
    TextDraw(arg0->DispX + 0x16, arg0->DispY + 0x21, Buffer, 0, 0.6f, 0.65f);
    SetWord2A(arg0->DispX + 0x20, arg0->DispY + 0x21, "\"", 0, 0.6f, 0.65f);
    KawanoItoaC(temp_s1, Buffer);
    TextDraw(arg0->DispX + 0x29, arg0->DispY + 0x21, Buffer, 0, 0.6f, 0.65f);
    var_s2 = (u32) temp_s1 < 0x927C0U ? var_s2 >> 0x14 : 8;
    SetWord3A(arg0->DispX + 0x60, arg0->DispY + 0x21, DriversNameJ[var_s2], 0, 0.6f, 0.65f);
}

void MselSub1(s32 arg0, ObjBlock* arg1) {
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t7;

    if (arg1->Work1 < 32) {
        if (1) {}
        temp_t7 = (arg1->Work1 * 65) / 64;
        temp_t1 = arg1->DispX;
        temp_a2 = arg1->DispY;
        gDPPipeSync(DisplayListHead++);
        gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        if (arg0 == CupSelect) {
            if (MenuScreenA == SUB_MENU_MAP_SELECT_CUP) {
                DisplayListHead = FlashRectangle2(DisplayListHead, temp_t1 + temp_t7, temp_a2,
                                                               (temp_t1 - temp_t7) + 64, temp_a2 + 39);
            } else {
                DisplayListHead = FillRect1Color(DisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 64,
                                                 temp_a2 + 39, 255, 249, 220, 255);
            }
        } else {
            DisplayListHead = FillRect1Color(DisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 64,
                                             temp_a2 + 39, 1, 1, 1, 255);
        }
    }
}

void MselSub2(ObjBlock* arg0) {
    s32 temp_s2;
    s32 temp_s3;
    s32 someIndex;

    temp_s2 = arg0->DispX;
    temp_s3 = arg0->DispY;
    gDPPipeSync(DisplayListHead++);
    gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    for (someIndex = 0; someIndex < NUM_COURSES_PER_CUP; someIndex++) {
        if ((someIndex == CourseSelect) && (MenuScreenA > SUB_MENU_MAP_SELECT_CUP) &&
            (GameMode != GRAND_PRIX)) {
            if ((MenuScreenA == SUB_MENU_MAP_SELECT_COURSE) ||
                (MenuScreenA == SUB_MENU_MAP_SELECT_BATTLE_COURSE)) {
                DisplayListHead = FlashRectangle2(
                    DisplayListHead, LmselCircuitPosition[someIndex][0].x + temp_s2, LmselCircuitPosition[someIndex][0].y + temp_s3,
                    LmselCircuitPosition[someIndex][1].x + temp_s2, LmselCircuitPosition[someIndex][1].y + temp_s3);
            } else {
                DisplayListHead = FillRect1Color(
                    DisplayListHead, LmselCircuitPosition[someIndex][0].x + temp_s2, LmselCircuitPosition[someIndex][0].y + temp_s3,
                    LmselCircuitPosition[someIndex][1].x + temp_s2, LmselCircuitPosition[someIndex][1].y + temp_s3, 0x000000FF,
                    0x000000F9, 0x000000DC, 0x000000FF);
            }
        } else {
            DisplayListHead = FillRect1Color(
                DisplayListHead, LmselCircuitPosition[someIndex][0].x + temp_s2, LmselCircuitPosition[someIndex][0].y + temp_s3,
                LmselCircuitPosition[someIndex][1].x + temp_s2, LmselCircuitPosition[someIndex][1].y + temp_s3, 1, 1, 1, 0x000000FF);
        }
    }
}

void MselSub4(ObjBlock* arg0) {
    s32 temp_s2;
    s32 temp_s3;
    s32 var_s0;
    ObjBlock* temp_v0;

    temp_v0 = GetOBJBlockPtr(MENU_ITEM_TYPE_064);
    temp_s2 = arg0->DispX;
    temp_s3 = arg0->DispY;
    gDPPipeSync(DisplayListHead++);
    gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    if (GameMode == GRAND_PRIX) {
        if (MenuScreenA != SUB_MENU_MAP_SELECT_OK) {
            for (var_s0 = 0; var_s0 < 4; var_s0++) {
                // Wut?
                if ((var_s0 != (temp_v0->Work1 % 4)) != 0) {
                    DisplayListHead =
                        FillRect1ColorF(DisplayListHead, LmselCircuitPosition[var_s0][0].x + temp_s2,
                                 LmselCircuitPosition[var_s0][0].y + temp_s3, LmselCircuitPosition[var_s0][1].x + temp_s2,
                                 LmselCircuitPosition[var_s0][1].y + temp_s3, 0, 0, 0, 0x00000064);
                }
            }
        }
    }
}

void MselBattleSub(UNUSED ObjBlock* arg0) {
    SetTextColor(TEXT_YELLOW);
    PrintText1CenterMode1(0x98, 0x44, MselBattleMessagesJ[0], 0, 1.0f, 1.0f);
    SetWord2A(0x17, 0x58, MselBattleMessagesJ[1], 0, 0.7f, 0.8f);
    SetWord2A(0x17, 0x6A, MselBattleMessagesJ[2], 0, 0.7f, 0.8f);
}

void DispOBSubMselCup(ObjBlock* arg0) {
    if (arg0->Work2 != 0) {
        DoSeqData2(arg0->SdNumber, arg0->DispX, arg0->DispY, -1);
        SetTextColor(TEXT_YELLOW);
        SetWord2A(arg0->DispX + 0x20, arg0->DispY + 0x28, CupTypeNames[arg0->Work2], 0, 0.7f, 0.7f);
    }
}

void DispOBSubMselGhost(UNUSED ObjBlock* arg0) {
    UNUSED Gfx* temp_v0_2;
    POSXY* temp_v0;
    s16 temp_s0;
    s16 temp_v1;
    s32 temp_s2;
    s32 var_s1;

    switch (MenuScreenA) { /* irregular */
        case SUB_MENU_MAP_SELECT_CUP:
            for (var_s1 = 0; var_s1 < 4; var_s1++) {
                if (CheckSameMap2((CupSelect * 4) + var_s1) >= 0) {
                    temp_v0 = &MselMap4BoxPosition[var_s1];
                    temp_v1 = temp_v0->x;
                    temp_s0 = temp_v0->y;
                    temp_s2 = temp_v1 + 0x20;
                    DisplayListHead =
                        FillRectBlack(DisplayListHead, temp_s2, (s32) temp_s0, temp_v1 + 0x3F, temp_s0 + 9);
                    DisplayListHead = DoTexList5(DisplayListHead, GetTBPointer(D_02004A0C), temp_s2,
                                                     (s32) temp_s0, 2);
                }
            }
            break;
        case SUB_MENU_MAP_SELECT_COURSE:
        default:
            if (CheckSameMap2((CupSelect * 4) + CourseSelect) >= 0) {
                DisplayListHead = FillRectBlack(DisplayListHead, 0x00000057, 0x00000070, 0x00000096, 0x00000081);
                DisplayListHead = DoTexList(DisplayListHead, D_02004A0C, 0x00000057, 0x00000070);
            }
            break;
    }
}

void TselSub1(UNUSED s32 arg0, ObjBlock* arg1) {
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t7;

    if (arg1->Work1 < 0x20) {
        if (1) {}
        temp_t7 = (arg1->Work1 * 0x41) / 0x40;
        temp_t1 = arg1->DispX;
        temp_a2 = arg1->DispY;
        gDPPipeSync(DisplayListHead++);
        gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
        gDPSetCombineMode(DisplayListHead++, G_CC_DECALRGBA, G_CC_DECALRGBA);
        DisplayListHead = FillRect1Color(DisplayListHead, temp_t1 + temp_t7, temp_a2, (temp_t1 - temp_t7) + 0x40,
                                         temp_a2 + 0x27, 1, 1, 1, 0xFF);
    }
}

void DOBMoving1(ObjBlock* arg0, s32 columnTarget, s32 rowTarget) {
    DOBMoving1X(arg0, columnTarget);
    DOBMoving1Y(arg0, rowTarget);
}

void DOBMoving1X(ObjBlock* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->DispX;

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
    arg0->DispX += step;
}

void DOBMoving1Y(ObjBlock* arg0, s32 rowTarget) {
    s32 step = rowTarget - arg0->DispY;

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
    arg0->DispY += step;
}

void DOBMoving2X(ObjBlock* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->DispX;

    if (step != 0) {
        if (step > 0) {
            step = (step / 4) + 1;
            if (step >= 0x11) {
                step = 0x10;
            }
            if (step < 8) {
                step = 8;
            }
            arg0->DispX += step;
            if (columnTarget < arg0->DispX) {
                arg0->DispX = columnTarget;
            }
        } else {
            step = (step / 4) - 1;
            if (step < -0x10) {
                step = -0x10;
            }
            if (step < -8) {
                step = -8;
            }
            arg0->DispX += step;
            if (arg0->DispX < columnTarget) {
                arg0->DispX = columnTarget;
            }
        }
    }
}

// Appears to be a copy of DOBMoving1Y
UNUSED void DOBMoving2Y(ObjBlock* arg0, s32 rowTarget) {
    s32 step = rowTarget - arg0->DispY;

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
    arg0->DispY += step;
}

void DOBMoving3X(ObjBlock* arg0, s32 columnTarget) {
    s32 step = columnTarget - arg0->DispX;

    if (step != 0) {
        step = 0xC8 / step;
        if (step > 0) {
            if (step >= 0x19) {
                step = 0x18;
            }
            if (step < 0x10) {
                step = 0x10;
            }
            arg0->DispX += step;
            if (columnTarget < arg0->DispX) {
                arg0->DispX = columnTarget;
            }
        } else {
            if (step < -0x18) {
                step = -0x18;
            }
            if (step >= -0xF) {
                step = -0x10;
            }
            arg0->DispX += step;
            if (arg0->DispX < columnTarget) {
                arg0->DispX = columnTarget;
            }
        }
    }
}

void DOBMoving4X(ObjBlock* arg0, s32 columnTarget, s32 Arg2) {
    s32 step;

    if (columnTarget == arg0->DispX) {
        arg0->DispX += Arg2;
    } else {
        step = columnTarget - arg0->DispX;
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
        arg0->DispX -= step;
    }
}

void DOBCpCscolor(ObjBlock* arg0) {
    // Cycle lasts 26 (0x1A) frames
    if (arg0->Pattern == 0) {
        // Move highlight from yellow to red
        arg0->Work2 = (s32) (u32) ((((f32) (0xC - arg0->Work1) * 127.0f) / 12.0f) + 128.0f);
    } else {
        // Move highlight from red to  yellow
        arg0->Work2 = (s32) (u32) ((((f64) (f32) arg0->Work1 * 127.0) / 12.0) + 128.0);
    }
    arg0->Work1++;
    if (arg0->Work1 >= 0xC) {
        // Every 13 (0xC) frames reverse Direction
        arg0->Work1 = 0;
        arg0->Pattern ^= 1;
    }
}

void DOBCpMes2(ObjBlock* arg0) {
    s32 phi_v0;

    switch (ControllerPakMenuSelection) {
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
    if (phi_v0 != arg0->Pattern) {
        arg0->Pattern = phi_v0;
        ChangeSeqData(arg0->SdNumber, 0, GetVAddress(D_800E7D34[phi_v0]));
    }
}

void DOBCpGnote(ObjBlock* arg0) {
    s32 i;

    switch (ControllerPakScrollDirection) {
        case CONTROLLER_PAK_SCROLL_DIR_DOWN:
            arg0->DispY -= 2;
            if (arg0->DispY < 0x60) {
                arg0->DispY = 0x69;
                ControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE;
                for (i = 0; i < 8; i++) {
                    if (i < 7) {
                        ControllerPakVisibleTableRows[i] = ControllerPakVisibleTableRows[i + 1];
                    } else {
                        if ((ControllerPakVisibleTableRows[i - 1] == 0x10) ||
                            (ControllerPakVisibleTableRows[i - 1] == 0)) {
                            ControllerPakVisibleTableRows[i] = 0;
                        } else {
                            ControllerPakVisibleTableRows[i] = ControllerPakVisibleTableRows[i - 1] + 1;
                        }
                    }
                }
                ControllerPakVisibleTableRows[0] = ControllerPakVisibleTableRows[8] = 0;
            }

            break;
        case CONTROLLER_PAK_SCROLL_DIR_UP:
            arg0->DispY += 2;
            if (arg0->DispY >= 0x73) {
                arg0->DispY = 0x69;
                ControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE;
                for (i = 8; i > 0; i--) {
                    if (i > 1) {
                        ControllerPakVisibleTableRows[i] = ControllerPakVisibleTableRows[i - 1];
                    } else {
                        if ((ControllerPakVisibleTableRows[i + 1] == 1) ||
                            (ControllerPakVisibleTableRows[i + 1] == 0)) {
                            ControllerPakVisibleTableRows[i] = 0;
                        } else {
                            ControllerPakVisibleTableRows[i] = ControllerPakVisibleTableRows[i + 1] - 1;
                        }
                    }
                }
                ControllerPakVisibleTableRows[0] = ControllerPakVisibleTableRows[8] = 0;
                break;
                default:
                    ControllerPakScrollDirection = CONTROLLER_PAK_SCROLL_DIR_NONE;
                    break;
            }
    }
} // todo: <-- fix these brackets

// Don't know if this belongs here or inside DOBSelOK
//   as a `static const`. But this matches so we'll leave
//   it here for now.
const s8 GselModeToOKMode[] = { 0x03, 0x03, 0x03, 0x02, 0x00, 0x02, 0x02, 0x01 };

const s8 PselModeToOKMode[] = { 0x03, 0x02, 0x00 };

const s8 MselModeToOKMode[] = { 0x03, 0x03, 0x02 };

void DOBSelOK(ObjBlock* arg0) {
    s32 sp4;
    s32 var_v0;

    switch (arg0->Kind) {
        default:
            var_v0 = sp4; // wut?
            break;
        case MENU_ITEM_UI_OK:
            var_v0 = GselModeToOKMode[MenuScreenB - 1];
            break;
        case COURSE_SELECT_OK:
            var_v0 = MselModeToOKMode[MenuScreenA - 1];
            break;
        case CHARACTER_SELECT_MENU_OK:
            var_v0 = PselModeToOKMode[MenuScreenC - 1];
            break;
    }
    switch (var_v0) {
        case 0:
            arg0->Work1 = 0;
            break;
        case 1:
            arg0->Work1 = 0x00000020;
            break;
        case 2:
            if (arg0->Work1 > 0) {
                arg0->Work1 = (arg0->Work1 - (arg0->Work1 / 12)) - 2;
                if (arg0->Work1 < 0) {
                    arg0->Work1 = 0;
                }
            }
            break;
        case 3:
            if (arg0->Work1 < 0x20) {
                arg0->Work1 += 2;
                if (arg0->Work1 >= 0x20) {
                    arg0->Work1 = 0x00000020;
                }
            }
            break;
    }
}

void DOBGselNumber(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            DOBGselStart(arg0);
            break;
        case 1:
            DOBGselStart(arg0);
            arg0->Pattern = 4;
            /* fallthrough */
        case 4:
            if (arg0->Work1 > 0) {
                arg0->Work1 = (arg0->Work1 - (arg0->Work1 / 12)) - 2;
                if (arg0->Work1 < 0) {
                    arg0->Work1 = 0;
                }
            } else {
                arg0->Work1 = 0;
                arg0->Pattern = 0;
            }
            break;
        case 2:
            arg0->Pattern = 3;
            break;
        case 3:
        default:
            break;
    }
}

void DOBGselMode(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            DOBGselStart(arg0);
            if ((MenuMultiplayerSelection + 0xA) == arg0->Kind) {
                arg0->Pattern = 2;
            } else {
                arg0->Pattern = 1;
            }
            break;
        case 4:
            if ((MenuMultiplayerSelection + 0xA) == arg0->Kind) {
                arg0->Pattern = 2;
                arg0->Work1 = 0;
                break;
            }
            arg0->Pattern = 1;
        case 1:
            DOBGselStart(arg0);
            if ((MenuScreenB == MAIN_MENU_OK_SELECT_GO_BACK) ||
                (MenuScreenB == MAIN_MENU_MODE_SUB_SELECT_GO_BACK)) {
                arg0->Work1 = 0x00000020;
            } else {
                if (arg0->Work1 < 0x20) {
                    arg0->Work1 += 2;
                    if (arg0->Work1 >= 0x20) {
                        arg0->Work1 = 0x00000020;
                    }
                }
            }
            break;
        case 3:
            if ((MenuMultiplayerSelection + 0xA) == arg0->Kind) {
                arg0->Pattern = 2;
            }
            break;
        case 2:
        default:
            break;
    }
}

void DOBGselMove(ObjBlock* arg0) {
    POSXY* temp_v0;

    if ((MenuMultiplayerSelection + 0xA) == arg0->Kind) {
        arg0->pri = 0x0A;
    } else {
        arg0->pri = 6;
    }

    switch (arg0->Pattern) {
        case 2:
            if ((MenuScreenB == MAIN_MENU_OK_SELECT_GO_BACK) ||
                (MenuScreenB == MAIN_MENU_MODE_SUB_SELECT_GO_BACK)) {
                arg0->DispX = 0x00000080;
                arg0->DispY = 0x0000003E;
            } else {
                DOBMoving1(arg0, 0x00000080, 0x0000003E);
            }
            break;
        case 3:
            temp_v0 = &LgselDefaultPositions[arg0->Kind - 0xA];
            DOBMoving1(arg0, temp_v0->x, temp_v0->y);
            if ((arg0->DispX == temp_v0->x) && (arg0->DispY == temp_v0->y)) {
                arg0->Pattern = 0;
            }
            /* fallthrough */
        case 0:
        case 1:
        case 4:
        default:
            break;
    }
}

void DOBGselGP(ObjBlock* arg0) {
    ObjBlock* temp_v0;
    POSXY* temp_v1_2;
    s32 sp24;
    s32 sp20;
    s32 sp1C;
    s32 temp_a1;

    switch (arg0->Kind) {
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

    temp_a1 = GameModePlayerSelection[MenuMultiplayerSelection - 1][GameModeMenuColumn[MenuMultiplayerSelection - 1]];
    switch (arg0->Pattern) { /* switch 5; irregular */
        case 0:            /* switch 5 */
            if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                arg0->DispOn = 0;
            } else {
                arg0->Work2 = GameModeMenuColumn[MenuMultiplayerSelection - 1];
                switch (MenuScreenB) {       /* switch 2 */
                    case MAIN_MENU_MODE_SELECT:     /* switch 2 */
                    case MAIN_MENU_MODE_SUB_SELECT: /* switch 2 */
                    case MAIN_MENU_OK_SELECT:       /* switch 2 */
                        arg0->DispOn = 1;
                        temp_v0 = GetGselCursorPtr();
                        arg0->DispX = temp_v0->DispX;
                        arg0->DispY = (GameModeMenuColumn[MenuMultiplayerSelection - 1] * 0x12) + temp_v0->DispY + 0x41;
                        arg0->Work1 = 0;
                        arg0->Pattern = 1;
                        break;
                    case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 2 */
                    case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 2 */
                        temp_v0 = GetGselCursorPtr();
                        temp_v1_2 = &GselPopupPositions[arg0->Kind - sp24];
                        arg0->DispX = temp_v0->DispX + temp_v1_2->x;
                        arg0->DispY = (GameModeMenuColumn[MenuMultiplayerSelection - 1] * 0x12) + temp_v0->DispY + temp_v1_2->y;
                        arg0->Work1 = arg0->DispY - temp_v0->DispY;
                        arg0->DispOn = 1;
                        arg0->Pattern = 2;
                        break;
                    default: /* switch 2 */
                        arg0->DispOn = 0;
                        break;
                }
            }
            break;
        case 1:                                         /* switch 5 */
            switch (MenuScreenB) {               /* switch 3 */
                case MAIN_MENU_MODE_SELECT:             /* switch 3 */
                case MAIN_MENU_MODE_SUB_SELECT:         /* switch 3 */
                case MAIN_MENU_OK_SELECT:               /* switch 3 */
                case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 3 */
                case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 3 */
                    if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                        arg0->DispOn = 0;
                        arg0->Pattern = 0;
                    } else {
                        if (arg0->Work2 != GameModeMenuColumn[MenuMultiplayerSelection - 1]) {
                            arg0->Pattern = 0;
                        }
                        temp_v0 = GetGselCursorPtr();
                        arg0->DispX = temp_v0->DispX + arg0->Work1;
                        temp_v1_2 = &GselPopupPositions[arg0->Kind - sp24];
                        DOBMoving2X(arg0, temp_v0->DispX + temp_v1_2->x);
                        arg0->Work1 = arg0->DispX - temp_v0->DispX;
                        if (arg0->Work1 == temp_v1_2->x) {
                            arg0->Pattern = 2;
                            arg0->Work1 = arg0->DispY - temp_v0->DispY;
                        }
                    }
                    break;
                default: /* switch 3 */
                    arg0->DispOn = 0;
                    arg0->Pattern = 0;
                    break;
            }
            break;
        case 2:                                         /* switch 5 */
            switch (MenuScreenB) {               /* switch 4 */
                case MAIN_MENU_MODE_SELECT:             /* switch 4 */
                case MAIN_MENU_MODE_SUB_SELECT:         /* switch 4 */
                case MAIN_MENU_OK_SELECT:               /* switch 4 */
                case MAIN_MENU_OK_SELECT_GO_BACK:       /* switch 4 */
                case MAIN_MENU_MODE_SUB_SELECT_GO_BACK: /* switch 4 */
                    if ((temp_a1 != sp20) && (temp_a1 != sp1C)) {
                        arg0->DispOn = 0;
                        arg0->Pattern = 0;
                    } else {
                        if (arg0->Work2 != GameModeMenuColumn[MenuMultiplayerSelection - 1]) {
                            arg0->Pattern = 0;
                        }
                        temp_v0 = GetGselCursorPtr();
                        temp_v1_2 = &GselPopupPositions[arg0->Kind - sp24];
                        arg0->DispX = temp_v0->DispX + temp_v1_2->x;
                        arg0->DispY = temp_v0->DispY + arg0->Work1;
                        DOBMoving1(arg0, arg0->DispX,
                                      (GameModeMenuColumn[MenuMultiplayerSelection - 1] * 0x12) + temp_v0->DispY + temp_v1_2->y);
                        arg0->Work1 = arg0->DispY - temp_v0->DispY;
                    }
                    break;
                default: /* switch 4 */
                    arg0->DispOn = 0;
                    arg0->Pattern = 0;
                    break;
            }
            break;
    }
}

void DOBGselStart(ObjBlock* arg0) {
    POSXY* temp_v0;

    temp_v0 = &LgselDefaultPositions[arg0->Kind - 0xA];
    if ((MenuScreenB == MAIN_MENU_OK_SELECT_GO_BACK) ||
        (MenuScreenB == MAIN_MENU_MODE_SUB_SELECT_GO_BACK)) {
        arg0->DispX = temp_v0->x;
        arg0->DispY = temp_v0->y;
    } else {
        DOBMoving1(arg0, temp_v0->x, temp_v0->y);
    }
}

void DOBGselCaution(ObjBlock* arg0) {
    s32 temp_v0;
    s32 var_t1;

    var_t1 = 0;
    switch (MenuScreenB) {
        case MAIN_MENU_OPTION:
        case MAIN_MENU_DATA:
        case MAIN_MENU_PLAYER_SELECT:
        case MAIN_MENU_MODE_SELECT:
            arg0->Pattern = 0;
            break;
        case MAIN_MENU_MODE_SUB_SELECT:
        case MAIN_MENU_OK_SELECT:
        case MAIN_MENU_OK_SELECT_GO_BACK:
        case MAIN_MENU_MODE_SUB_SELECT_GO_BACK:
            if (arg0->Pattern != 0) {
                break;
            }
            if (MenuMultiplayerSelection != 1) {
                break;
            }
            if (GameModeMenuColumn[MenuMultiplayerSelection - 1] != 1) {
                break;
            }

            if (ControllerPak1State != 0) {
                var_t1 = 0;
                switch (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName,
                                      (u8*) ExtCode, &ControllerPak1FileNote)) {
                    case 5:
                        break;
                    case 0:
                        arg0->Pattern = 1;
                        var_t1 = 1;
                        break;
                    case 2:
                        ControllerPak1State = 0;
                        InvalidatePakType(CONTROLLER_1);
                        break;
                    default:
                        ControllerPak1State = 0;
                        InvalidatePakType(CONTROLLER_1);
                        break;
                }
            }
            if (var_t1 == 0) {
                if (ControllerPak1State == 0) {
                    if (GetExistPak(0) != PAK) {
                        arg0->Pattern = 2;
                        break;
                    }
                    temp_v0 = osPfsInit(&SIEventMesgQueue, &ControllerPak1FileHandle, 0);
                    if (temp_v0 != 0) {
                        switch (temp_v0) {
                            case PFS_ERR_NOPACK:
                            case PFS_ERR_DEVICE:
                                arg0->Pattern = 2;
                                break;
                            case PFS_ERR_ID_FATAL:
                                arg0->Pattern = 3;
                                break;
                            case PFS_ERR_CONTRFAIL:
                            default:
                                arg0->Pattern = 3;
                                break;
                        }
                        return;
                    } else {
                        ControllerPak1State = 1;
                    }
                    if (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName,
                                      (u8*) ExtCode, &ControllerPak1FileNote) == 0) {
                        arg0->Pattern = 1;
                        break;
                    }
                    if (osPfsNumFiles(&ControllerPak1FileHandle, &ControllerPak1NumFilesUsed,
                                      &ControllerPak1MaxWriteableFiles) != 0) {
                        arg0->Pattern = 3;
                        break;
                    }
                    if (osPfsFreeBlocks(&ControllerPak1FileHandle, &ControllerPak1NumPagesFree) != 0) {
                        arg0->Pattern = 3;
                        break;
                    }
                    ControllerPak1NumPagesFree = (s32) ControllerPak1NumPagesFree >> 8;
                }
                if (ControllerPak1MaxWriteableFiles >= ControllerPak1NumFilesUsed) {
                    arg0->Pattern = 5;
                    break;
                }
                if (ControllerPak1NumPagesFree >= 0x79) {
                    arg0->Pattern = 1;
                    break;
                }
                arg0->Pattern = 5;
            }
            break;
        default:
            break;
    }
}

void DOBPselAnimeMario(ObjBlock* arg0, s8 arg1) {
    s32 temp_v1;

    temp_v1 = arg0->Kind - 0x2B;
    switch (arg0->Pattern2) { /* irregular */
        case 0:
            if (GetPselCursorNumber((s32) arg1) >= 0) {
                arg0->Pattern2 = 2;
                ChangeSeqData(arg0->SdNumber, 0, GetVAddress(TBSeqPselReturn[temp_v1]));
            }
            break;
        case 2:
            if (GetPselCursorNumber((s32) arg1) < 0) {
                arg0->Pattern2 = 0;
                ChangeSeqData(arg0->SdNumber, 0, GetVAddress(D_800E8320[temp_v1]));
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

    temp_a0 = arg0->Kind - 0x2B;
    temp_v0 = GetPselCursorNumber(arg0->Kind - 0x2B);
    if (temp_v0 >= 0) {
        var_a0 = 1;
    } else {
        var_a0 = 0;
    }
    if (OKPSelSkipAnime != 0) {
        var_a0 = 0;
    }
    switch (arg0->Pattern2) {
        case 0:
            if ((CharacterGridIsSelected[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->Pattern2 = 1;
                ChangeSeqData(arg0->SdNumber, 0,
                              GetVAddress(TBSeqPselP1[temp_a0]));
            } else {
                temp_v0 = MakeRandomLimmit(0x00C8U);
                if (temp_v0 >= 0xC6) {
                    arg0->Pattern2 = 4;
                    ChangeSeqData(arg0->SdNumber, 0,
                                  GetVAddress(TBSeqPselP4[temp_a0]));
                } else if (temp_v0 >= 0xC5) {
                    arg0->Pattern2 = 5;
                    ChangeSeqData(arg0->SdNumber, 0,
                                  GetVAddress(TBSeqPselP5[temp_a0]));
                }
            }
            break;
        case 1:
            if (SEQDataBlock[arg0->SdNumber].SequenceIndex >= P1PatNum[temp_a0]) {
                arg0->Pattern2 = 2;
                ChangeSeqData(arg0->SdNumber, 0, GetVAddress(TBSeqPselP2[temp_a0]));
            } else if ((CharacterGridIsSelected[temp_v0] == 0) && (var_a0 != 0)) {
                arg0->Pattern2 = 3;
                ChangeSeqData(arg0->SdNumber,
                              P3PatNum[temp_a0] - SEQDataBlock[arg0->SdNumber].SequenceIndex,
                              GetVAddress(TBSeqPselP3[temp_a0]));
            }
            break;
        case 2:
            if ((CharacterGridIsSelected[temp_v0] == 0) && (var_a0 != 0)) {
                arg0->Pattern2 = 3;
                ChangeSeqData(arg0->SdNumber, 0,
                              GetVAddress(TBSeqPselP3[temp_a0]));
            }
            break;
        case 3:
            if (SEQDataBlock[arg0->SdNumber].SequenceIndex >= P3PatNum[temp_a0]) {
                arg0->Pattern2 = 0;
                ChangeSeqData(arg0->SdNumber, 0, GetVAddress(TBSeqPselP0[temp_a0]));
            } else if ((CharacterGridIsSelected[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->Pattern2 = 1;
                ChangeSeqData(arg0->SdNumber,
                              P3PatNum[temp_a0] - SEQDataBlock[arg0->SdNumber].SequenceIndex,
                              GetVAddress(TBSeqPselP1[temp_a0]));
            }
            break;
        case 4:
        case 5:
            if ((CharacterGridIsSelected[temp_v0] != 0) && (var_a0 != 0)) {
                arg0->Pattern2 = 1;
                ChangeSeqData(arg0->SdNumber, 0,
                              GetVAddress(TBSeqPselP1[temp_a0]));
            } else {
                if (arg0->Pattern2 == 4) {
                    var_v0 = P4PatNum[temp_a0];
                } else {
                    var_v0 = P5PatNum[temp_a0];
                }
                if (SEQDataBlock[arg0->SdNumber].SequenceIndex >= var_v0) {
                    arg0->Pattern2 = 0;
                    ChangeSeqData(arg0->SdNumber, 0, GetVAddress(TBSeqPselP0[temp_a0]));
                }
            }
            break;
        default:
            break;
    }
}

void DOBPSelTurnOut(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 3:
            arg0->Pattern = 1;
            /* fallthrough */
        case 1:
            if (MenuScreenC == PLAYER_SELECT_MENU_OK_GO_BACK) {
                arg0->Work1 = 0x00000020;
            } else {
                if (arg0->Work1 < 0x20) {
                    arg0->Work1 += (arg0->Work1 / 12) + 2;
                    if (arg0->Work1 >= 0x20) {
                        arg0->Work1 = 0x00000020;
                    }
                }
            }
            /* fallthrough */
        case 0:
            if (GetPselCursorNumber(arg0->Kind - 0x2B) >= 0) {
                arg0->Pattern = 2;
                arg0->Work1 = 0;
            } else {
                arg0->Pattern = 1;
            }
            break;
        case 4:
            if (GetPselCursorNumber(arg0->Kind - 0x2B) >= 0) {
                arg0->Pattern = 2;
                arg0->Work1 = 0;
            }
            break;
        case 2:
        default:
            break;
    }
}

void DOBPSelTurnIn(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 1:
            if (arg0->Work1 > 0) {
                arg0->Pattern = 3;
            }
            break;
        case 2:
            arg0->Pattern = 4;
            break;
        case 3:
            if (arg0->Work1 > 0) {
                arg0->Work1 = (arg0->Work1 - (arg0->Work1 / 12)) - 2;
                if (arg0->Work1 < 0) {
                    arg0->Work1 = 0;
                }
            } else {
                arg0->Work1 = 0;
                arg0->Pattern = 0;
            }
            break;
        case 0:
        case 4:
        default:
            break;
    }
}

void DOBPselCharacter1(ObjBlock* arg0) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    s32 temp_a1;
    s32 temp_v0;
    UNUSED s32 stackPadding2;
    POSXY* var_t0;

    temp_a1 = arg0->Kind - 0x2B;
    switch (arg0->Pattern) {
        case 0:
            if (MenuScreenC == PLAYER_SELECT_MENU_OK_GO_BACK) {
                temp_v0 = GetPselCursorNumber(temp_a1);
                if (temp_v0 >= 0) {
                    var_t0 = &LpselCharacterOKPosition[(ScreenSplitB * 4) + temp_v0];
                    arg0->DispX = (s32) var_t0->x;
                    arg0->DispY = (s32) var_t0->y;
                    arg0->Pattern = 2;
                    arg0->Pattern2 = 2;
                    ChangeSeqData(arg0->SdNumber, 0, GetVAddress(TBSeqPselReturn[temp_a1]));
                }
                break;
            }
        case 1:
        case 3:
            var_t0 = &PlayerSelectPositions[0][temp_a1];
            arg0->DispX = (s32) var_t0->x;
            arg0->DispY = (s32) var_t0->y;
            break;
        case 2:
        case 4:
            if (arg0->Pattern == 2) {
                temp_v0 = GetPselCursorNumber(temp_a1);
                if (temp_v0 >= 0) {
                    var_t0 = &LpselCharacterOKPosition[(ScreenSplitB * 4) + temp_v0];
                }
            } else {
                var_t0 = &PlayerSelectPositions[0][temp_a1];
                if ((var_t0->x == arg0->DispX) && (var_t0->y == arg0->DispY)) {
                    arg0->Pattern = 0;
                    return;
                }
            }
            if ((arg0->Pattern != 2) || (arg0->Pattern2 != 1)) {
                DOBMoving1(arg0, (s32) var_t0->x, (s32) var_t0->y);
            }
            break;
        default:
            break;
    }
}

void DOBPselCursor1(ObjBlock* arg0) {
    s32 PlayerId;
    s8 characterSelectionIndex;

    PlayerId = arg0->Kind - CHARACTER_SELECT_MENU_1P_CURSOR;
    characterSelectionIndex = Player1Character[PlayerId];
    arg0->pri = 0xE - (PlayerId * 2);
    GetPselCursorPosition(arg0, characterSelectionIndex - 1);
}

void DOBPselAnimePriority(ObjBlock* arg0) {
    s32 temp_v0;

    temp_v0 = GetPselCursorNumber(arg0->Kind - CHARACTER_SELECT_MENU_MARIO);
    if (temp_v0 >= 0) {
        arg0->pri = 0xE - (temp_v0 * 2);
    } else {
        arg0->pri = 6;
    }
}

/**
 * Similar to GetOBJBlockPtr, there is potential for a
 * hard lock in the function if no appropriate MenuItems entry
 * is found.
 **/
ObjBlock* GetGselCursorPtr(void) {
    ObjBlock* entry = MenuItems;
    s32 nbPlayer = MenuMultiplayerSelection - 1;

    for (; !(entry > &MenuItems[MENU_ITEMS_MAX]); entry++) {
        if ((nbPlayer + MENU_ITEM_UI_1P_GAME) == entry->Kind) {
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
 * Similar to GetOBJBlockPtr, there is potential for a
 * hard lock in the function if no appropriate MenuItems entry
 * is found.
 **/
ObjBlock* GetPselCharacterPtr(s32 CharacterId) {
    ObjBlock* entry = MenuItems;

    for (; !(entry > &MenuItems[MENU_ITEMS_MAX]); entry++) {
        if ((CharacterId + CHARACTER_SELECT_MENU_MARIO) == entry->Kind) {
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
 * `arg0` is the desired "Type" of the 8018D9E0 entry to
 * search for. If no entry with that "Type" is found, this
 * function will enter a `while(1)` Loop, hard-locking the
 * game.
 *
 * In practice this never appears to happen, but that is
 * probably as much a matter of luck as it is good
 * reasoning on the original author(s) part.
 **/
ObjBlock* GetOBJBlockPtr(s32 Type) {
    ObjBlock* entry = MenuItems;
    for (; !(entry > (&MenuItems[MENU_ITEMS_MAX])); entry++) {
        if (entry->Kind == Type) {
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

ObjBlock* GetOBJBlockPtr2(s32 Type) {
    ObjBlock* entry = MenuItems;
    for (; !(entry > (&MenuItems[MENU_ITEMS_MAX])); entry++) {
        if (entry->Kind == Type) {
            goto escape;
        }
    }

    return NULL;
escape:
    return entry;
}

UNUSED s32 GetPselCharacterPattern(s32 CharacterId) {
    ObjBlock* temp;
    temp = GetPselCharacterPtr(CharacterId);
    return temp->Pattern;
}

void GetPselCursorPosition(ObjBlock* arg0, s32 CharacterId) {
    ObjBlock* temp_v0;

    temp_v0 = GetPselCharacterPtr(CharacterId);
    arg0->DispX = temp_v0->DispX;
    arg0->DispY = temp_v0->DispY;
}

s32 GetPselCursorNumber(s32 CharacterId) {
    s32 someIndex = 0;
    bool ret = false;

    for (; someIndex < ARRAY_COUNT(Player1Character); someIndex++) {
        if ((CharacterId + 1) == Player1Character[someIndex]) {
            ret = true;
            break;
        }
    }

    if (ret != false) {
        return someIndex;
    }

    return -1;
}

void DOBMselCup(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 1:
            arg0->Pattern = 4;
            /* fallthrough */
        case 4:
            if (arg0->Work1 > 0) {
                arg0->Work1 = (arg0->Work1 - (arg0->Work1 / 12)) - 2;
                if (arg0->Work1 < 0) {
                    arg0->Work1 = 0;
                }
            } else {
                arg0->Work1 = 0;
                arg0->Pattern = 0;
            }
            break;
        case 2:
            arg0->Pattern = 3;
            break;
        case 0:
        case 3:
        default:
            break;
    }
}

void DOBMselMap(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            if ((CupSelect + 0x53) == arg0->Kind) {
                arg0->Pattern = 2;
            } else {
                arg0->Pattern = 1;
            }
            break;
        case 4:
            if ((CupSelect + 0x53) == arg0->Kind) {
                arg0->Pattern = 2;
                arg0->Work1 = 0;
                break;
            } else {
                arg0->Pattern = 1;
            }
        // Purposeful fallthrough
        case 1:
            if (arg0->Work1 < 32) {
                arg0->Work1 += 2;
                if (arg0->Work1 >= 32) {
                    arg0->Work1 = 32;
                }
            }
            break;
        case 3:
            if ((CupSelect + 0x53) == arg0->Kind) {
                arg0->Pattern = 2;
            }
            break;
        case 2:
        default:
            break;
    }
}

void DOBMselMove(ObjBlock* arg0) {
    POSXY* thing = &MselCupboxPosition[arg0->Kind - 0x53];

    if ((CupSelect + 0x53) == arg0->Kind) {
        arg0->pri = 0x0A;
    } else {
        arg0->pri = 4;
    }

    switch (arg0->Pattern) {
        case 0:
            thing = &MselCupboxPosition[arg0->Kind - 0x53];
            arg0->DispX = thing->x;
            arg0->DispY = thing->y;
            break;
        case 2:
            DOBMoving1(arg0, 0x00000080, 0x0000003B);
            break;
        case 3:
            thing = &MselCupboxPosition[arg0->Kind - 0x53];
            DOBMoving1(arg0, thing->x, thing->y);
            if ((arg0->DispX == thing->x) && (arg0->DispY == thing->y)) {
                arg0->Pattern = 0;
            }
            break;
        case 1:
        case 4:
        default:
            break;
    }
}

void DOBMselName(ObjBlock* arg0) {
    s32 temp = (arg0->Kind - 0x58);
    if (temp == CupSelect) {
        arg0->DispOn = 1;
    } else {
        arg0->DispOn = 0;
    }
}

void DOBMselAnimeMap2(ObjBlock* arg0) {
    if (arg0->Work1 != CupSelect) {
        arg0->Work1 = CupSelect;
        ChangeSeqData(arg0->SdNumber, 0,
                      GetVAddress(D_800E7E34[CupCourseOrder[CupSelect][arg0->Kind - 0x5F]]));
    }
}

void DOBMselAnimeMapman(ObjBlock* Item) {
    s32 i;
    UNUSED s32 pad[2];
    ObjBlock* _items[4]; // Added _ to help distinguish between the function argument and this

    for (i = 0; i < 4; i++) {
        _items[i] = GetOBJBlockPtr(i + 0x5F);
    }
    if (GameMode != 0) {
        if (MenuScreenA != SUB_MENU_MAP_SELECT_CUP) {
            Item->Pattern = 0;
            Item->Work2 = 0;

            for (i = 0; i < 4; i++) {
                if (CourseSelect == i) {
                    _items[i]->DispOn = 1;
                    if (Item->Work1 != i) {
                        Item->Work1 = i;
                    }
                } else {
                    _items[i]->DispOn = 0;
                }
            }
        } else {
            Item->Pattern = 3;
            for (i = 0; i < 4; i++) {
                _items[i]->DispOn = 1;
                _items[i]->pri = 6;
            }
        }
    } else {
        switch (Item->Pattern) {
            case 0:
                if (CupSelect == (Item->Work1 / 4)) {
                    if (++Item->Work2 > 50) {
                        Item->Pattern = 1;
                        Item->Work2 = 0;
                        Item->Work1 = (CupSelect * 4) + 1;
                    }
                } else {
                    Item->Work2 = 0;
                    Item->Work1 = CupSelect * 4;
                }
                if (MenuScreenA == SUB_MENU_MAP_SELECT_OK) {
                    Item->Pattern = 2;
                    Item->Work2 = 0;
                }
                break;
            case 1:
                if (CupSelect != (Item->Work1 / 4)) {
                    Item->Pattern = 0;
                    Item->Work2 = 0;
                    Item->Work1 = 0;
                    break;
                }

                if (++Item->Work2 > 30) {
                    Item->Work2 = 0;
                    Item->Work1 = (CupSelect * 4) + (((Item->Work1 % 4) + 1) % 4);
                    break;
                }
                if (MenuScreenA == SUB_MENU_MAP_SELECT_OK) {
                    Item->Pattern = 2;
                    Item->Work2 = 0;
                }
                break;
            case 2:
                if (++Item->Work2 > 25) {
                    Item->Pattern = 3;
                    Item->Work2 = 0;
                }
                if (MenuScreenA != SUB_MENU_MAP_SELECT_OK) {
                    Item->Pattern = 0;
                    Item->Work2 = 0;
                    Item->Work1 = 0;
                }
                break;
            case 3:
                if (MenuScreenA != SUB_MENU_MAP_SELECT_OK) {
                    Item->Pattern = 0;
                    Item->Work2 = 0;
                    Item->Work1 = 0;
                }
                break;
        }

        switch (Item->Pattern) {
            case 0:
            case 1:
                for (i = 0; i < 4; i++) {
                    if ((Item->Work1 % 4) == i) {
                        _items[i]->DispOn = 1;
                    } else {
                        _items[i]->DispOn = 0;
                    }
                    _items[i]->pri = 6;
                }
                break;
            case 2:
                for (i = 0; i < 4; i++) {
                    if ((Item->Work1 % 4) == i) {
                        _items[i]->pri = 6;
                    } else if (Item->Work2 < (i * 5)) {
                        _items[i]->pri = 4;
                    } else {
                        _items[i]->pri = 8;
                    }
                    _items[i]->DispOn = 1;
                }
                break;
            case 3:
                for (i = 0; i < 4; i++) {
                    _items[i]->DispOn = 1;
                    _items[i]->pri = 6;
                }
                break;
        }
    }
}

void DOBMselBbox(ObjBlock* arg0) {
    POSXY* temp_a1;

    switch (MenuScreenA) { /* irregular */
        case SUB_MENU_MAP_SELECT_CUP:
            temp_a1 = &MselBestboxDefaultPosition[arg0->Kind - 0x65];
            if (arg0->DispX != temp_a1->x) {
                DOBMoving1X(arg0, temp_a1->x);
            }
            break;
        case SUB_MENU_MAP_SELECT_COURSE:
        case SUB_MENU_MAP_SELECT_OK:
            temp_a1 = &MselBestboxTargetPosition[arg0->Kind - 0x65];
            if (arg0->DispX != temp_a1->x) {
                DOBMoving1X(arg0, temp_a1->x);
            }
            break;
    }
}

void DOBMselCupGsc(ObjBlock* arg0) {
    POSXY* temp_v1;

    if (arg0->Work1 != CupSelect) {
        arg0->Work1 = CupSelect;
        arg0->Work2 = GetCupStatus((s32) CupSelect, RaceClass);
        ChangeSeqData(arg0->SdNumber, 0,
                      GetVAddress(D_800E7E20[((RaceClass / 2) * 4) - arg0->Work2]));
        arg0->DispX = (s32) MselCupPositions->x;
        arg0->DispY = MselCupPositions->y;
    }
    temp_v1 = &MselCupPositions[arg0->Pattern];
    switch (arg0->Pattern) { /* irregular */
        case 0:
            DOBMoving1(arg0, (s32) temp_v1->x, (s32) temp_v1->y);
            if (MenuScreenA == SUB_MENU_MAP_SELECT_OK) {
                arg0->Pattern = 1;
            }
            break;
        case 1:
            DOBMoving1(arg0, (s32) temp_v1->x, (s32) temp_v1->y);
            if (MenuScreenA == SUB_MENU_MAP_SELECT_CUP) {
                arg0->Pattern = 0;
            }
            break;
    }
}

void DOBTselCup(ObjBlock* arg0) {
    s32 Index;

    if (arg0->Kind == 0x8C) {
        Index = 4;
    } else {
        Index = arg0->Kind - 0x78;
    }
    arg0->DispX = TselCupPositions[Index].x;
    arg0->DispY = TselCupPositions[Index].y;
}

void DOBTselCursor(ObjBlock* arg0) {
    POSXY* temp_v1;
    s32 thing = TimeTrialDataCourseIndex;

    temp_v1 = &TselCupPositions[thing / 4];
    arg0->DispX = temp_v1->x - 2;
    arg0->DispY = temp_v1->y + ((thing % 4) * 0x32) + 0x13;
    arg0->Work1 += 0x10;
    if (arg0->Work1 >= 0x100) {
        arg0->Work1 -= 0x100;
        arg0->Work2 = (s32) (arg0->Work2 + 1) % 3;
    }
}

void DOBTselMap(ObjBlock* arg0) {
    s32 temp_v0;
    POSXY* temp_v1;

    temp_v0 = arg0->Kind - 0x7C;
    temp_v1 = &TselCupPositions[temp_v0 / 4];
    arg0->DispX = (s32) temp_v1->x;
    arg0->DispY = temp_v1->y + ((temp_v0 % 4) * 0x32) + 0x14;
}

void DOBGameCup(ObjBlock* arg0) {
    // Huh?
    s32 one = 1;
    DOBGameCupMode1(arg0);
    switch (OpeningFlag) { /* irregular */
        case 1:
            arg0->DispOn = one;
            break;
        case 2:
            if (arg0->DispY >= -0x13) {
                arg0->DispY -= 2;
            } else {
                arg0->Kind = 0;
            }
            break;
        default:
            if ((GameMode != GRAND_PRIX) || (PlayerCount != (s32) 1U) || (DemoPlayFlag != 0)) {
                arg0->Kind = 0;
            } else {
                if (arg0->DispY >= -0x13) {
                    arg0->DispY -= 2;
                } else {
                    arg0->Kind = 0;
                }
            }
            break;
    }
}

void DOBGameCupMode1(ObjBlock* arg0) {
    f64 temp_f0;

    switch (arg0->Pattern) { /* irregular */
        case 0:
            arg0->DispX = 0;
            arg0->Pattern = 1;
            arg0->Work2 = (GetWordLength(CupName[CupLevel]) / 2) + 0xA0;
            /* fallthrough */
        case 1:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (s32) (arg0->Work2 - arg0->DispX) / 4;
            if (arg0->Work1 >= 9) {
                arg0->Work1 = 8;
            }
            arg0->Workf1 = (f32) (((f64) arg0->Work1 * 0.05) + 1.0);
            if (arg0->DispX >= (arg0->Work2 - 0x14)) {
                arg0->Pattern = 2;
                arg0->SdNumber = 0;
            }
            break;
        case 2:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (s32) (arg0->Work2 - arg0->DispX) / 4;
            arg0->SdNumber++;
            temp_f0 = (f64) (arg0->SdNumber - 0xA);
            arg0->Workf1 = (f32) ((temp_f0 * 0.0085 * temp_f0) + 0.4);
            if ((arg0->SdNumber >= 9) && ((f64) arg0->Workf1 > 1.0)) {
                arg0->Workf1 = 1.0f;
            }
            break;
    }
}

void DOBGameCircuit(ObjBlock* arg0) {
    s32 why = 1;
    DOBGameCircuitMode1(arg0);
    switch (OpeningFlag) {
        case 1:
            arg0->DispOn = why;
            break;
        case 2:
            if (arg0->DispY < 0x104) {
                arg0->DispY += 2;
            } else {
                arg0->Kind = 0;
            }
            break;
        default:
            if ((GameMode != GRAND_PRIX) || (PlayerCount != why) || (DemoPlayFlag != 0)) {
                arg0->Kind = 0;
            } else {
                if (arg0->DispY < 0x104) {
                    arg0->DispY += 2;
                } else {
                    arg0->Kind = 0;
                }
            }
            break;
    }
}

void DOBGameCircuitMode1(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = 0x140;
            arg0->Pattern = 1;
            arg0->Work2 = 0xA0 - (GetWordLength(StringTableCourseGP[CourseID]) / 2);
            /* fallthrough */
        case 1:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (arg0->DispX - arg0->Work2) / 4;
            if (arg0->Work1 >= 9) {
                arg0->Work1 = 8;
            }
            arg0->Workf1 = (arg0->Work1 * 0.05) + 1.0;
            if ((arg0->Work2 + 0x14) >= arg0->DispX) {
                arg0->Pattern = 2;
                arg0->SdNumber = 0;
            }
            break;
        case 2:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (arg0->DispX - arg0->Work2) / 4;
            arg0->SdNumber++;
            arg0->Workf1 = ((arg0->SdNumber - 0xA) * 0.0085 * (arg0->SdNumber - 0xA)) + 0.4;
            if ((arg0->SdNumber >= 9) && ((f64) arg0->Workf1 > 1.0)) {
                arg0->Workf1 = 1.0f;
            }
            break;
    }
}

void DOBGameCircuitMode2(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = 0x00000140;
            arg0->Pattern = 1;
            /* fallthrough */
        case 1:
            DOBMoving3X(arg0, 0x00000064);
            arg0->Work1 = (s32) (arg0->DispX - 0x64) / 6;
            if (arg0->Work1 >= 9) {
                arg0->Work1 = 8;
            }
            arg0->Workf1 = (f32) (((f64) arg0->Work1 * 0.07) + 0.6);
            if (arg0->DispX == 0x00000064) {
                arg0->Pattern = 2;
                arg0->SdNumber = 0;
            }
            break;
        case 2:
            arg0->SdNumber++;
            arg0->Work1 = 0;
            arg0->Workf1 = (f32) (1.5 - ((arg0->SdNumber - 0xF) * 0.004 * (arg0->SdNumber - 0xF)));
            if ((arg0->SdNumber >= 0x10) && ((f64) arg0->Workf1 < 0.8)) {
                arg0->Pattern = 3;
                arg0->SdNumber = 0;
            }
            break;
        case 3:
            arg0->SdNumber++;
            arg0->Work1 = 0;
            arg0->Workf1 = (f32) (1.25 - ((arg0->SdNumber - 0xF) * 0.002 * (arg0->SdNumber - 0xF)));
            if ((arg0->SdNumber >= 0xD) && ((f64) arg0->Workf1 < 1.0)) {
                arg0->Workf1 = 1.0f;
            }
            break;
    }
}

void DOBMselNoise(ObjBlock* arg0) {
    if (arg0->Work2 < ++arg0->Work1) {
        arg0->Kind = 0;
    }
}

void DOBGameTime(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = 0x14A;
            arg0->Pattern = 1;
            SkipReset();
            break;
        case 1:
            DOBMoving1X(arg0, 0xA0);
            if (arg0->DispX == 0xA0) {
                arg0->Pattern = 2;
                arg0->Work2 = 0;
            }
            break;
        case 2:
            arg0->Work2++;
            if (((KSkipFlg != 0) || (arg0->Work2 >= 0x5B)) && (D_800DDB24 != 0)) {
                arg0->Pattern = 3;
                arg0->Work1 = arg0->DispX;
                SetObjBlock(MENU_ITEM_TYPE_0AB, 0, 0, MENU_ITEM_PRIORITY_0);
            }
            break;
        case 3:
            arg0->DispX = arg0->Work1;
            if (arg0->Work1 < 0x14A) {
                if (KSkipFlg != 0) {
                    arg0->Work1 += 0x20;
                } else {
                    arg0->Work1 += 0x10;
                }
            } else {
                arg0->Kind = 0;
            }
            break;
    }
}

void DOBGamePoint(ObjBlock* arg0) {
    s32 var_a1;
    s32 var_t1;
    s32 temp;

    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = -0x000000A0;
            arg0->Pattern = 1;
            for (var_a1 = 0; var_a1 < ARRAY_COUNT(AdditionalPointDefaults); var_a1++) {
                AdditionalPoint[var_a1] = AdditionalPointDefaults[var_a1];
            }
            arg0->Work2 = arg0->DispX;
            break;
        case 1:
            arg0->DispX = arg0->Work2;

            temp = (KSkipFlg != 0) ? 0x20 : 0x10;

            if ((arg0->Work2 + temp) < 0) {
                arg0->Work2 += temp;
                D_800DC5EC->Posx += temp;
                D_800DC5F0->Posx -= temp;
            } else {
                arg0->Work2 = 0;
                arg0->DispX = 0;
                arg0->Pattern = 2;
                arg0->Work1 = 0;
                D_800DC5EC->Posx = 0x00F0;
                D_800DC5F0->Posx = 0x0050;
            }
            break;
        case 2:
            arg0->DispX = 0;
            arg0->Work1++;
            if (((KSkipFlg != 0) || (arg0->Work1 >= 0x1F)) && (D_800DDB24 != 0)) {
                arg0->Pattern = 3;
                arg0->Work1 = 0;
                arg0->Work2 = 0;
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            var_t1 = 0;
            var_a1 = arg0->Pattern - 3;
            arg0->Work1++;
            if (((arg0->Work1 % 3) == 0) || (KSkipFlg != 0)) {
                if (AdditionalPoint[var_a1] > 0) {
                    AdditionalPoint[var_a1]--;
                    PlayerGPpoints[GamePlayers[RankToNum[var_a1]].Kart] += 1;
                    PlaySound(SOUND_ACTION_COUNT_SCORE);
                    var_t1 = 0;
                    if ((AdditionalPoint[var_a1] == 0) && (arg0->Work2 == 0)) {
                        arg0->Work2 = 1;
                        arg0->Work1 = 0;
                    }
                }
            }
            if ((arg0->Work2 != 0) && ((arg0->Work1 > 0xA) || ((KSkipFlg != 0) && (arg0->Work1 >= 4)))) {
                var_t1 = 1;
            }
            if (var_t1 != 0) {
                arg0->Work2 = 0;
                arg0->Work1 = 0;
                if (arg0->Pattern < 6) {
                    arg0->Pattern++;
                } else {
                    arg0->Pattern = 7;
                }
            }
            break;
        case 7:
            arg0->Work1++;
            if ((((KSkipFlg != 0) && (arg0->Work1 >= 0xB)) || (arg0->Work1 >= 0x3D)) && (D_800DDB24 != 0)) {
                arg0->Pattern = 8;
                arg0->Work1 = 0;
            }
            break;
        case 8:
            arg0->Work1++;
            if (KSkipFlg != 0) {
                arg0->Work1 += 5;
            }
            if (arg0->Work1 >= 0x29) {
                arg0->Pattern = 9;
            }
            break;
        case 9:
            arg0->Work1--;
            if (KSkipFlg != 0) {
                arg0->Work1 -= 5;
            }
            if (arg0->Work1 <= 0) {
                arg0->Pattern = 0x0000000A;
                arg0->Work1 = 0;
                if (CourseSelect == 3) {
                    for (var_a1 = 0; var_a1 < 8; var_a1++) {
                        if (NameToNum[DPointsRank[var_a1]] < MenuMultiplayerSelection) {
                            SaveCupStatus(var_a1);
                            break;
                        }
                    }
                }
            }
            break;
        case 10:
            arg0->Work1++;
            if (arg0->Work1 > 0) {
                arg0->Pattern = 0x0000000B;
                arg0->Work1 = 0;
                SkipReset();
            }
            break;
        case 11:
            if ((KSkipFlg != 0) && (D_800DDB24 != 0)) {
                arg0->Pattern = 0x0000000C;
                arg0->Work2 = arg0->DispY;
                PlaySound(SOUND_ACTION_NEXT_COURSE);
            }
            break;
        case 12:
            arg0->DispY = arg0->Work2;
            if (arg0->Work2 < 0xF0) {
                arg0->Work2 += 0x10;
                D_800DC5EC->Posy += 0x10;
                D_800DC5F0->Posy -= 0x10;
            } else {
                arg0->Work2 = 0;
                arg0->Pattern = 0x0000000D;
                arg0->Work1 = 0;
                D_800DC5EC->Posy = 0x012C;
                D_800DC5F0->Posy = -0x003C;
                FinishZoomFlag = 4;
                NaSeqFadeout(0x19U);
            }
            break;
        case 13:
        default:
            break;
    }
}

void DOBInfo3P(ObjBlock* arg0) {
    switch (arg0->Pattern) { /* irregular */
        case 0:
            arg0->DispX = 0x14A;
            arg0->Pattern = 1;
            arg0->Work1 = 0xFF;
            break;
        case 1:
            DOBMoving1X(arg0, 0xA0);
            if (arg0->DispX == 0xA0) {
                arg0->Pattern = 2;
            }
            break;
        case 2:
            break;
        case 3:
            if (arg0->Work1 != 0) {
                arg0->Work1 -= 0x33;
            }
            break;
    }
}

void DOBGPLose(ObjBlock* arg0) {
    switch (arg0->Pattern) { /* irregular */
        case 0:
            if (arg0->Work2 >= 0xB) {
                arg0->Work1 += 3;
            }
            if (arg0->Work1 >= 0x65) {
                arg0->Pattern = 0x0000000B;
                arg0->Work1 = 0;
            }
            break;
        case 11:
        case 12:
            if (FadeChecker()) {
                break;
            }

            if ((ControllerFive->ButtonPressed | ControllerFive->AnalogPressed) & U_JPAD) {
                if (arg0->Pattern >= 0xC) {
                    arg0->Pattern--;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->Workf1 < 4.2) {
                        arg0->Workf1 += 4.0;
                    }
                    arg0->Pattern2 = -1;
                }
            }
            if ((ControllerFive->ButtonPressed | ControllerFive->AnalogPressed) & D_JPAD) {
                if (arg0->Pattern < 0xC) {
                    arg0->Pattern++;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->Workf1 < 4.2) {
                        arg0->Workf1 += 4.0;
                    }
                    arg0->Pattern2 = 1;
                }
            }
            if (ControllerFive->ButtonPressed & (A_BUTTON | START_BUTTON)) {
                SetFadeOut(0x0000001E);
                NaSeqFadeout(0x19U);
                PlaySound(SOUND_ACTION_CONTINUE_UNKNOWN);
                if (arg0->Workf1 < 4.2) {
                    arg0->Workf1 += 4.0;
                }
            }
            break;
        default:
            break;
    }
    if (arg0->Work2 < 0x28) {
        arg0->Work2++;
    }
}

void DOBVSResult(ObjBlock* arg0) {
    s32 i;

    switch (arg0->Pattern) {
        case 0:
            arg0->Work1 += 3;
            if (arg0->Work1 >= 0x65) {
                arg0->Pattern = 1;
                arg0->Work1 = 0;
                for (i = 0; i < MenuMultiplayerSelection; i++) {
                    SetObjBlock(i + MENU_ITEM_TYPE_0B1, 0, 0, (s8) (MENU_ITEM_PRIORITY_5 - i));
                }
            }
            break;
        case 1:
            if (GetOBJBlockPtr(0x000000B1)->Pattern >= 2) {
                arg0->Pattern = 2;
            }
            break;
        case 2:
            arg0->Work1 += 0x20;
            if (arg0->Work1 >= 0x100) {
                if (GameMode == VERSUS) {
                    arg0->Pattern = (s32) VSResultCursor;
                } else {
                    arg0->Pattern = (s32) BtResultCursor;
                }
                arg0->Work1 = 0;
            }
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            if (FadeChecker() == 0) {
                if ((ControllerFive->ButtonPressed | ControllerFive->AnalogPressed) & U_JPAD) {
                    if (arg0->Pattern >= 0xB) {
                        arg0->Pattern--;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = -1;
                    }
                }
                if ((ControllerFive->ButtonPressed | ControllerFive->AnalogPressed) & D_JPAD) {
                    if (arg0->Pattern < 0xD) {
                        arg0->Pattern++;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = 1;
                    }
                }
                if (ControllerFive->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                    SetFadeOut(0x0000001E);
                    PlaySound(SOUND_MENU_OK_CLICKED);
                    if (GameMode == VERSUS) {
                        VSResultCursor = (s8) arg0->Pattern;
                    } else {
                        BtResultCursor = (s8) arg0->Pattern;
                    }
                    if (arg0->Workf1 < 4.2) {
                        arg0->Workf1 += 4.0;
                    }
                }
            }
            break;
        default:
            break;
    }
}

void DOBVSFace(ObjBlock* arg0) {
    POSXY* temp_v0_2;
    s32 somePlayerIndex;
    s32 temp_a1;
    s32 var_v1;
    UNUSED s32 stackPadding0;

    somePlayerIndex = arg0->Kind - 0xB1;
    temp_a1 = Driver2Pselset[CharacterSelections[arg0->Kind - 0xB1]];
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = VSFaceStartPosition2.x;
            arg0->DispY = VSFaceStartPosition2.y;
            arg0->Pattern = 1;
            break;
        case 1:
            temp_v0_2 = &VSFaceTargetPosition[((MenuMultiplayerSelection - 2) * 4) + somePlayerIndex];
            DOBMoving1X(arg0, temp_v0_2->x);
            DOBMoving1Y(arg0, temp_v0_2->y);
            if (arg0->DispX == temp_v0_2->x) {
                arg0->Pattern = 2;
                arg0->Work2 = 0;
            }
            break;
        case 2:
            var_v1 = 0;
            switch (GameMode) {
                case 2:
                    if (GPCurrentRaceRankByPlayerId[somePlayerIndex] != 0) {
                        var_v1 = 1;
                    }
                    break;
                case 3:
                    if (somePlayerIndex != WinKart) {
                        var_v1 = 1;
                    }
                    break;
            }
            if (var_v1 == 0) {
                arg0->Work2++;
                if (arg0->Work2 >= 0x1F) {
                    if (GetOBJBlockPtr(0x000000B0)->Pattern >= 2) {
                        ChangeSeqDataVS(arg0->SdNumber, 0, somePlayerIndex,
                                      GetVAddress(TBSeqPselP1[temp_a1]));
                        arg0->Pattern = 3;
                        NaSeMuteOff(somePlayerIndex);
                        NAPlyVoiceStart(somePlayerIndex, (CharacterSelections[somePlayerIndex] * 0x10) + 0x29008007);
                    }
                }
            }
            break;
        case 3:
            if (SEQDataBlock[arg0->SdNumber].SequenceIndex >= P1PatNum[temp_a1]) {
                ChangeSeqDataVS(arg0->SdNumber, 0, somePlayerIndex, GetVAddress(TBSeqPselP2[temp_a1]));
                arg0->Pattern = 4;
            }
            break;
        case 4:
        default:
            break;
    }
}

void DOBGameTa1(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = 0x0000014A;
            arg0->Pattern = 1;
            SkipReset();
            SetObjBlock(MENU_ITEM_TYPE_0BB, 0, 0, MENU_ITEM_PRIORITY_0);
            break;
        case 1:
            DOBMoving1X(arg0, 0x000000A0);
            if (arg0->DispX == 0x000000A0) {
                arg0->Pattern = 2;
                arg0->Work2 = 0;
            }
            break;
        case 2:
            arg0->Work2++;
            if (arg0->Work2 >= 0x15) {
                arg0->Pattern = 3;
            }
            break;
        case 3:
            if (KSkipFlg != 0) {
                SkipReset();
                arg0->Pattern = 4;
                arg0->Work1 = arg0->DispX;
                SetObjBlock(MENU_ITEM_TYPE_0BA, 0, 0, MENU_ITEM_PRIORITY_0);
            }
            break;
        case 4:
            arg0->DispX = arg0->Work1;
            if (arg0->Work1 < 0x14A) {
                if (KSkipFlg != 0) {
                    arg0->Work1 += 0x20;
                } else {
                    arg0->Work1 += 0x10;
                }
            } else {
                arg0->Kind = 0;
            }
            break;
        default:
            break;
    }
}

void DOBGameTa2(ObjBlock* arg0) {
    struct_8018EE10_entry* thing;
    s32 var_v1;
    s32 var_a1;
    s32 Index;

    switch (arg0->Pattern) { /* switch 3; irregular */
        case 0:            /* switch 3 */
            arg0->DispX = -0x000000A0;
            arg0->Pattern = 1;
            for (Index = 0; Index < ARRAY_COUNT(AdditionalPointDefaults); Index++) {
                AdditionalPoint[Index] = AdditionalPointDefaults[Index];
            }
            arg0->Work2 = arg0->DispX;
            break;
            ;
        case 1: /* switch 3 */
            arg0->DispX = arg0->Work2;
            if (KSkipFlg != 0) {
                var_a1 = 0x20;
            } else {
                var_a1 = 0x10;
            }
            if ((arg0->Work2 + var_a1) < 0) {
                arg0->Work2 += var_a1;
                D_800DC5EC->Posx += var_a1;
                D_800DC5F0->Posx -= var_a1;
            } else {
                arg0->Work2 = 0;
                arg0->DispX = 0;
                arg0->Pattern = TaResultCursor;
                if ((arg0->Pattern == 9) && (LostGhost == 1)) {
                    arg0->Pattern--;
                }
                D_800DC5EC->Posx = 0x00F0;
                D_800DC5F0->Posx = 0x0050;
            }
            break;
        case 5:  /* switch 3 */
        case 6:  /* switch 3 */
        case 7:  /* switch 3 */
        case 8:  /* switch 3 */
        case 9:  /* switch 3 */
        case 10: /* switch 3 */
            if (FadeChecker() == 0) {
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->Pattern >= 6) {
                        arg0->Pattern--;
                        if ((LostGhost == 1) && (arg0->Pattern == 9)) {
                            arg0->Pattern--;
                        }
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = -1;
                    }
                }
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->Pattern < 0xA) {
                        arg0->Pattern++;
                        if ((LostGhost == 1) && (arg0->Pattern == 9)) {
                            arg0->Pattern++;
                        }
                        if ((arg0->Pattern == 0x0000000A) && (GhostStatus != 0)) {
                            arg0->Pattern -= 2;
                        } else {
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->Workf1 < 4.2) {
                                arg0->Workf1 += 4.0;
                            }
                            arg0->Pattern2 = 1;
                        }
                    }
                }
                if (ControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                    if (arg0->Pattern == 0x0000000A) {
                        var_v1 = 0;
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        if (ControllerPak1State != 0) {
                            var_v1 = 0;
                            switch (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName,
                                                  (u8*) ExtCode, &ControllerPak1FileNote)) { /* switch 4; irregular */
                                case 5:                                                        /* switch 4 */
                                    break;
                                case 0: /* switch 4 */
                                    LoadGameNoteKartData();
                                    arg0->Pattern = CheckSameMap((CupSelect * 4) + CourseSelect) + 0x11;
                                    var_v1 = 1;
                                    PlaySound(SOUND_MENU_SELECT);
                                    break;
                                case 2: /* switch 4 */
                                    ControllerPak1State = 0;
                                    InvalidatePakType(CONTROLLER_1);
                                    break;
                                default: /* switch 4 */
                                    ControllerPak1State = 0;
                                    InvalidatePakType(CONTROLLER_1);
                                    break;
                            }
                        }
                        if (var_v1 == 0) {
                            if (ControllerPak1State == 0) {
                                switch (InitPak()) { /* switch 2 */
                                    case -1:               /* switch 2 */
                                        arg0->Pattern = 0x0000000B;
                                        var_v1 = 1;
                                        break;
                                    case -3: /* switch 2 */
                                    case -2: /* switch 2 */
                                        arg0->Pattern = 0x0000000C;
                                        var_v1 = 1;
                                        break;
                                    case 1:  /* switch 2 */
                                    case 11: /* switch 2 */
                                        arg0->Pattern = 0x0000000B;
                                        var_v1 = 1;
                                        break;
                                    case 10: /* switch 2 */
                                        arg0->Pattern = 0x0000000C;
                                        var_v1 = 1;
                                        break;
                                    default: /* switch 2 */
                                        var_v1 = 1;
                                        arg0->Pattern = 0x0000000C;
                                        break;
                                    case 0:
                                        break;
                                }
                                if (var_v1 != 0) {
                                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                    return;
                                }
                                if (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName,
                                                  (u8*) ExtCode, &ControllerPak1FileNote) == 0) {
                                    LoadGameNoteKartData();
                                    arg0->Pattern = CheckSameMap((CupSelect * 4) + CourseSelect) + 0x11;
                                    PlaySound(SOUND_MENU_SELECT);
                                    return;
                                }
                            }
                            if (ControllerPak1MaxWriteableFiles >= ControllerPak1NumFilesUsed) {
                                arg0->Pattern = 0x0000000E;
                                PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                return;
                            }
                            if (ControllerPak1NumPagesFree >= 0x79) {
                                arg0->Pattern = 0x00000013;
                                arg0->Work1 = 0;
                                PlaySound(SOUND_MENU_SELECT);
                                return;
                            }
                            arg0->Pattern = 0x0000000E;
                            PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                        }
                    } else {
                        arg0->Work1 = arg0->Pattern;
                        TaResultCursor = arg0->Pattern;
                        arg0->Pattern = 0x0000001E;
                        arg0->Work2 = arg0->DispY;
                        PlaySound(SOUND_ACTION_NEXT_COURSE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
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
            if (ControllerOne->ButtonPressed & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                arg0->Pattern = 0x0000000A;
                PlaySound(SOUND_MENU_GO_BACK);
                if (arg0->Workf1 < 4.2) {
                    arg0->Workf1 += 4.0;
                }
            }
            break;
        case 17:
        case 18:
            arg0->Work2 = arg0->Pattern - 0x11;
            if (CheckSameMap2((CupSelect * 4) + CourseSelect) != arg0->Work2) {
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->Pattern >= 0x12) {
                        arg0->Pattern--;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = -1;
                    }
                }
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->Pattern < 0x12) {
                        arg0->Pattern++;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = 1;
                    }
                }
            }
            if (ControllerOne->ButtonPressed & B_BUTTON) {
                arg0->Pattern = 0x0000000A;
                PlaySound(SOUND_MENU_GO_BACK);
                return;
            }
            if (ControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                thing = &D_8018EE10[arg0->Work2];
                if (thing->GhostDataSaved == 0) {
                    arg0->Pattern = 0x00000019;
                    arg0->Work1 = 0;
                } else if (CheckSameGhost(arg0->Work2) == 0) {
                    arg0->Pattern = 0x00000010;
                } else {
                    arg0->Pattern = 0x00000014;
                }
                PlaySound(SOUND_MENU_SELECT);
                if (arg0->Workf1 < 4.2) {
                    arg0->Workf1 += 4.0;
                }
            }
            break;
        case 19:
            if ((arg0->Work1 == 1) && (MakeGameNoteKartOnly() != 0)) {
                arg0->Pattern = 0x0000000F;
                return;
            } else {
                arg0->Work1++;
                if (arg0->Work1 >= 2) {
                    arg0->Pattern = 0x00000011;
                }
            }
            break;
        case 20:
        case 21:
            if (((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & U_JPAD) && (arg0->Pattern >= 0x15)) {
                arg0->Pattern--;
                PlaySound(SOUND_MENU_CURSOR_MOVE);
                if (arg0->Workf1 < 4.2) {
                    arg0->Workf1 += 4.0;
                }
                arg0->Pattern2 = -1;
            }
            if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & D_JPAD) {
                if (arg0->Pattern < 0x15) {
                    arg0->Pattern++;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->Workf1 < 4.2) {
                        arg0->Workf1 += 4.0;
                    }
                    arg0->Pattern2 = 1;
                }
            }
            if (ControllerOne->ButtonPressed & B_BUTTON) {
                arg0->Pattern = arg0->Work2 + 0x11;
                PlaySound(SOUND_MENU_GO_BACK);
                return;
            }
            if (ControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                if (arg0->Pattern == 0x00000015) {
                    arg0->Pattern = 0x00000019;
                    arg0->Work1 = 0;
                    PlaySound(SOUND_MENU_SELECT);
                    if (arg0->Workf1 < 4.2) {
                        arg0->Workf1 += 4.0;
                    }
                } else {
                    arg0->Pattern = arg0->Work2 + 0x11;
                    PlaySound(SOUND_MENU_GO_BACK);
                    return;
                }
            }
            break;
        case 25:
            if (arg0->Work1 == 1) {
                if (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName, (u8*) ExtCode,
                                  &ControllerPak1FileNote) != 0) {
                    arg0->Pattern = 0x0000001A;
                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                if (SaveGameNoteKartGhost(arg0->Work2) != 0) {
                    arg0->Pattern = 0x0000001A;
                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
            }
            arg0->Work1++;
            if (arg0->Work1 >= 2) {
                arg0->Pattern = 0x0000000A;
                return;
            }
            break;
        case 30:
            arg0->DispY = arg0->Work2;
            if (arg0->Work2 < 0xF0) {
                arg0->Work2 += 0x10;
                D_800DC5EC->Posy += 0x10;
                D_800DC5F0->Posy -= 0x10;
                return;
            }
            switch (arg0->Work1) { /* switch 3 */
                case 5:             /* switch 3 */
                    ReplayFlag = 0;
                    RetryFlag = 1;
                    GotoRacing();
                    break;
                case 6: /* switch 3 */
                    GotoMapselect();
                    break;
                case 7: /* switch 3 */
                    GoToGameSelect();
                    break;
                case 8: /* switch 3 */
                    GotoGameselect();
                    break;
                case 9: /* switch 3 */
                    ReplayFlag = 1;
                    RetryFlag = 0;
                    GotoRacing();
                    break;
            }
            arg0->Work2 = 0;
            arg0->Pattern = 0x0000001F;
            D_800DC5EC->Posy = 0x012C;
            D_800DC5F0->Posy = -0x003C;
            FinishZoomFlag = 4;
            NaSeqFadeout(0x19U);
            break;
        case 31:
            arg0->Kind = 0;
            break;
    }
}
#ifdef VERSION_EU
#define FUNC_800ADF48DEF 70
#else
#define FUNC_800ADF48DEF 60
#endif
void DOBGamePause(ObjBlock* arg0) {
    UNUSED s32 stackPadding;
    struct Controller* controller;

    if (PauseFlag != 0) {
        switch (arg0->Pattern) {
            case 0:
                arg0->Pattern = StartPauseSEQ[GameMode];
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
                    controller = &Controllers[PauseFlag - 1];
                    if ((controller->ButtonPressed | controller->AnalogPressed) & U_JPAD) {
                        if (StartPauseSEQ[GameMode] < arg0->Pattern) {
                            arg0->Pattern--;
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->Workf1 < 4.2) {
                                arg0->Workf1 += 4.0;
                            }
                            arg0->Pattern2 = -1;
                        }
                    }
                    if ((controller->ButtonPressed | controller->AnalogPressed) & D_JPAD) {
                        if (arg0->Pattern < BottomPauseSEQ[GameMode]) {
                            arg0->Pattern++;
                            PlaySound(SOUND_MENU_CURSOR_MOVE);
                            if (arg0->Workf1 < 4.2) {
                                arg0->Workf1 += 4.0;
                            }
                            arg0->Pattern2 = 1;
                        }
                    }
                    if (controller->ButtonPressed & B_BUTTON) {
                        if (arg0->Pattern != StartPauseSEQ[GameMode]) {
                            arg0->Pattern = StartPauseSEQ[GameMode];
                            PlaySound(SOUND_MENU_GO_BACK);
                            return;
                        }
                    }
                    if (controller->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                        if (arg0->Pattern == StartPauseSEQ[GameMode]) {
                            arg0->Pattern = 0;
                            PauseFlag = 0;
                            LoadKeyStatus();
                            NaPauseSet(0U);
                        } else {
                            SetFadeOut(30);
                            PlaySound(SOUND_ACTION_CONTINUE_UNKNOWN);
                            NaSeqFadeout(FUNC_800ADF48DEF);
                            if (arg0->Workf1 < 4.2) {
                                arg0->Workf1 += 4.0;
                            }
                        }
                    }
                }
                break;
            default:
                break;
        }
    } else {
        arg0->Pattern = 0;
    }
}

void DOBGameTaReplay(ObjBlock* arg0) {
    struct_8018EE10_entry* thing;
    s32 var_v1;

    if (arg0->Pattern != 0) {
        TwoDFlag = 0;
    }
    switch (arg0->Pattern) {
        case 0:
            if (arg0->Work1 < 0x1E) {
                arg0->Work1++;
            }
            if (ControllerOne->ButtonPressed & START_BUTTON) {
                arg0->Pattern = 0x0000000F;
                PlaySound(SOUND_ACTION_GO_BACK_2);
            } else if (PlayerHUD[PLAYER_ONE].Goalsw != 0) {
                arg0->Pattern = 1;
                arg0->Work1 = 0;
            }
            break;
        default:
            break;
        case 1:
            arg0->Work1 += 3;
            if (arg0->Work1 >= 0x8D) {
                arg0->Pattern = 0x0000000F;
            }
            break;
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            if (FadeChecker() == 0) {
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->Pattern >= 0xC) {
                        arg0->Pattern--;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = -1;
                    }
                }
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->Pattern < 0x10) {
                        arg0->Pattern++;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = 1;
                    }
                }
                if (ControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                    if (arg0->Pattern == 0x00000010) {
                        var_v1 = 0;
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        if (ControllerPak1State != 0) {
                            var_v1 = 0;
                            switch (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName,
                                                  (u8*) ExtCode, &ControllerPak1FileNote)) { /* switch 3; irregular */
                                case PFS_ERR_INVALID:                                          /* switch 3 */
                                    break;
                                case PFS_NO_ERROR: /* switch 3 */
                                    LoadGameNoteKartData();
                                    arg0->Pattern = CheckSameMap((CupSelect * 4) + CourseSelect) + 0x1E;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_NEW_PACK: /* switch 3 */
                                    ControllerPak1State = 0;
                                    InvalidatePakType(CONTROLLER_1);
                                    break;
                                default: /* switch 3 */
                                    ControllerPak1State = 0;
                                    InvalidatePakType(CONTROLLER_1);
                                    break;
                            }
                        }
                        if (var_v1 != 0) {
                            PlaySound(SOUND_MENU_SELECT);
                            return;
                        }
                        if (ControllerPak1State == 0) {
                            switch (InitPak()) { /* switch 2 */
                                case PFS_INVALID_DATA: /* switch 2 */
                                    arg0->Pattern = 0x00000015;
                                    var_v1 = 1;
                                    break;
                                case PFS_FREE_BLOCKS_ERROR: /* switch 2 */
                                case PFS_NUM_FILES_ERROR:   /* switch 2 */
                                    arg0->Pattern = 0x00000016;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_NOPACK: /* switch 2 */
                                case PFS_ERR_DEVICE: /* switch 2 */
                                    arg0->Pattern = 0x00000015;
                                    var_v1 = 1;
                                    break;
                                case PFS_ERR_ID_FATAL: /* switch 2 */
                                    arg0->Pattern = 0x00000016;
                                    var_v1 = 1;
                                    break;
                                default: /* switch 2 */
                                    var_v1 = 1;
                                    arg0->Pattern = 0x00000016;
                                    break;
                                case 0:
                                    break;
                            }
                            if (var_v1 != 0) {
                                PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                                return;
                            }
                            if (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName,
                                              (u8*) ExtCode, &ControllerPak1FileNote) == 0) {
                                LoadGameNoteKartData();
                                arg0->Pattern = CheckSameMap((CupSelect * 4) + CourseSelect) + 0x1E;
                                PlaySound(SOUND_MENU_SELECT);
                                return;
                            }
                        }
                        if (ControllerPak1MaxWriteableFiles >= ControllerPak1NumFilesUsed) {
                            arg0->Pattern = 0x00000018;
                            PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                        } else if (ControllerPak1NumPagesFree >= 0x79) {
                            arg0->Pattern = 0x00000020;
                            arg0->Work1 = 0;
                            PlaySound(SOUND_MENU_SELECT);
                        } else {
                            arg0->Pattern = 0x00000018;
                            PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                        }
                    } else {
                        SetFadeOut(0x0000001E);
                        PlaySound(SOUND_MENU_OK_CLICKED);
                        NaSeqFadeout(0x19U);
                        NaSEFadeout(0x19U);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
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
            if (ControllerOne->ButtonPressed & (A_BUTTON | B_BUTTON | START_BUTTON)) {
                arg0->Pattern = 0x00000010;
                PlaySound(SOUND_MENU_GO_BACK);
                if (arg0->Workf1 < 4.2) {
                    arg0->Workf1 += 4.0;
                }
            }
            break;
        case 30:
        case 31:
            arg0->Work2 = (u32) arg0->Pattern - 0x1E;
            if (CheckSameMap2((CupSelect * 4) + CourseSelect) != arg0->Work2) {
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & U_JPAD) {
                    if (arg0->Pattern >= 0x1F) {
                        arg0->Pattern--;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = -1;
                    }
                }
                if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & D_JPAD) {
                    if (arg0->Pattern < 0x1F) {
                        arg0->Pattern++;
                        PlaySound(SOUND_MENU_CURSOR_MOVE);
                        if (arg0->Workf1 < 4.2) {
                            arg0->Workf1 += 4.0;
                        }
                        arg0->Pattern2 = 1;
                    }
                }
            }
            if (ControllerOne->ButtonPressed & B_BUTTON) {
                arg0->Pattern = 0x00000010;
                PlaySound(SOUND_MENU_GO_BACK);
            } else if (ControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                thing = &D_8018EE10[arg0->Work2];
                if (thing->GhostDataSaved == 0) {
                    arg0->Pattern = 0x00000028;
                    arg0->Work1 = 0;
                } else if (CheckSameGhost(arg0->Work2) == 0) {
                    arg0->Pattern = 0x0000001A;
                } else {
                    arg0->Pattern = 0x00000023;
                }
                PlaySound(SOUND_MENU_SELECT);
                if (arg0->Workf1 < 4.2) {
                    arg0->Workf1 += 4.0;
                }
            }
            break;
        case 32:
            if ((arg0->Work1 == 1) && (MakeGameNoteKartOnly() != 0)) {
                arg0->Pattern = 0x00000019;
            } else {
                arg0->Work1++;
                if (arg0->Work1 >= 2) {
                    arg0->Pattern = 0x0000001E;
                }
            }
            break;
        case 35:
        case 36:
            if (((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & U_JPAD) &&
                ((s32) (u32) arg0->Pattern >= 0x24)) {
                arg0->Pattern--;
                PlaySound(SOUND_MENU_CURSOR_MOVE);
                if (arg0->Workf1 < 4.2) {
                    arg0->Workf1 += 4.0;
                }
                arg0->Pattern2 = -1;
            }
            if ((ControllerOne->ButtonPressed | ControllerOne->AnalogPressed) & D_JPAD) {
                if (arg0->Pattern < 0x24) {
                    arg0->Pattern++;
                    PlaySound(SOUND_MENU_CURSOR_MOVE);
                    if (arg0->Workf1 < 4.2) {
                        arg0->Workf1 += 4.0;
                    }
                    arg0->Pattern2 = 1;
                }
            }
            if (ControllerOne->ButtonPressed & B_BUTTON) {
                arg0->Pattern = arg0->Work2 + 0x1E;
                PlaySound(SOUND_MENU_GO_BACK);
            } else if (ControllerOne->ButtonPressed & (START_BUTTON | A_BUTTON)) {
                if (arg0->Pattern == 0x00000024) {
                    arg0->Pattern = 0x00000028;
                    arg0->Work1 = 0;
                    PlaySound(SOUND_MENU_SELECT);
                    if (arg0->Workf1 < 4.2) {
                        arg0->Workf1 += 4.0;
                    }
                } else {
                    arg0->Pattern = arg0->Work2 + 0x1E;
                    PlaySound(SOUND_MENU_GO_BACK);
                }
            }
            break;
        case 40:
            if (arg0->Work1 == 1) {
                if (osPfsFindFile(&ControllerPak1FileHandle, CompanyCode, PakGameCode, (u8*) GameName, (u8*) ExtCode,
                                  &ControllerPak1FileNote) != 0) {
                    arg0->Pattern = 0x00000029;
                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
                if (SaveGameNoteKartGhost(arg0->Work2) != 0) {
                    arg0->Pattern = 0x00000029;
                    PlaySound(SOUND_MENU_FILE_NOT_FOUND);
                    return;
                }
            }
            arg0->Work1++;
            if (arg0->Work1 >= 2) {
                arg0->Pattern = 0x00000010;
            }
            break;
    }
}

void DOBGameTaMes1(ObjBlock* arg0) {
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = (GetWordLength(TaRomghostMessageJ) / 2) + 0x140;
            arg0->DispY = 0x000000DA;
            arg0->Pattern = 1;
            NAPlyVoiceStart(0U, (RomghostKart * 0x10) + 0x29008001);
            break;
        case 1:
            DOBMoving1X(arg0, 0x000000A0);
            if (arg0->DispX == 0x000000A0) {
                arg0->Pattern = 2;
                arg0->Work1 = 0;
            }
            break;
        case 2:
            arg0->Work1++;
            if (arg0->Work1 >= 0x3D) {
                arg0->Pattern = 3;
                arg0->Work1 = 0;
            }
            break;
        case 4:
            arg0->Work1++;
            if (arg0->Work1 >= 6) {
                arg0->Kind = 0;
                break;
            }
            // Purposeful fallthrough
        case 3:
            DOBMoving4X(arg0, 0x000000A0, -1);
            if (((arg0->DispX + 0x14) == -(GetWordLength(TaRomghostMessageJ) / 2)) && (arg0->Pattern == 3)) {
                arg0->Pattern = 4;
            }
            break;
        default:
            break;
    }
}

void DOBDselSnap(ObjBlock* arg0) {
    if (arg0->Work1 != TimeTrialDataCourseIndex) {
        arg0->Work1 = (s32) TimeTrialDataCourseIndex;
        ChangeSeqData(arg0->SdNumber, 0,
                      GetVAddress(
                          D_800E7E34[CupCourseOrder[TimeTrialDataCourseIndex / 4][TimeTrialDataCourseIndex % 4]]));
        if (CheckPakStatus() == PFS_NO_ERROR) {
            LoadGameNoteKartData();
        } else {
            D_8018EE10[0].GhostDataSaved = 0;
            D_8018EE10[1].GhostDataSaved = 0;
        }
    }
}

void DOBDselSel1(ObjBlock* arg0) {
    if (MenuScreenA != SUB_MENU_DATA_OPTIONS) {
        arg0->DispOn = 0;
    } else {
        arg0->DispOn = 1;
    }
}

void DOBDselSel2(ObjBlock* arg0) {
    if (MenuScreenA != SUB_MENU_DATA_ERASE_CONFIRM) {
        arg0->DispOn = 0;
    } else {
        arg0->DispOn = 1;
    }
}

void DOBDselSel3(ObjBlock* arg0) {
    if (MenuScreenA != SUB_MENU_DATA_CANT_ERASE) {
        arg0->DispOn = 0;
    } else {
        arg0->DispOn = 1;
    }
}

void DOBTaFanfare(ObjBlock* arg0) {
    if (PlayerHUD[PLAYER_ONE].Goalsw != 0) {
        if ((u32) PlayerHUD[PLAYER_ONE].Totaltime < (u32) (GetRecordTime2(4) & 0xFFFFF)) {
            RankinFanfare = 1;
        }
        arg0->Kind = 0;
    }
}

void DOBTaLostghost(ObjBlock* arg0) {
    switch (arg0->Pattern) { /* irregular */
        case 0:
            if (LostGhost == 1) {
                arg0->Pattern = 1;
                arg0->Work1 = 0;
            } else if (PlayerHUD[PLAYER_ONE].Goalsw == (s8) 1) {
                arg0->Pattern = 2;
            }
            break;
        case 2:
            break;
        case 1:
            arg0->Work1 += 1;
            if (PlayerHUD[PLAYER_ONE].Goalsw == 1) {
                arg0->Pattern = 2;
            }
            break;
    }
}

void DOBEnding1Halffade(ObjBlock* arg0) {
    UNUSED s32 temp_t1;

    switch (arg0->Pattern) {
        case 0:
            arg0->Work1 += 3;
            if (arg0->Work1 >= 0x65) {
                arg0->Work1 = 0;
                arg0->Pattern = 1;
                CupSelect %= 4;
                RaceClass %= 4;
                SetObjBlock(MENU_ITEM_TYPE_12C, 0, 0, MENU_ITEM_PRIORITY_4);
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            arg0->Work1 += 1;
            if (arg0->Work1 >= 9) {
                arg0->Work1 = 0;
                arg0->Pattern++;
                SetObjBlock(arg0->Pattern + MENU_ITEM_TYPE_12B, 0, 0, MENU_ITEM_PRIORITY_4);
            }
            break;
        case 5:
            arg0->Work1 += 1;
            if ((arg0->Work1 >= 0x65) &&
                ((ControllerFive->ButtonPressed != 0) || (ControllerFive->AnalogPressed != 0))) {
                arg0->Pattern = 6;
                arg0->Work1 = 0;
                if (Resultstat.Rank < 3) {
                    PlaySound(SOUND_MENU_OK_CLICKED);
                } else {
                    PlaySound(SOUND_ACTION_CONTINUE_UNKNOWN);
                }
            }
            break;
        case 6:
            SetFadeOut(0x0000001E);
            NaSeqFadeout(0x19U);
            NaSEFadeout(0x19U);
            arg0->Pattern = 7;
            break;
        case 7:
        default:
            break;
    }
}

void DOBEnding1Message(ObjBlock* arg0) {
    POSXY* temp_v0_2;
    s32 idx = arg0->Kind - 0x12C;

    switch (arg0->Pattern) { /* irregular */
        case 0:
            temp_v0_2 = &Ending1MessagesStartPosition[idx];
            arg0->DispX = temp_v0_2->x;
            arg0->DispY = temp_v0_2->y;
            arg0->Pattern = 1;
            break;
        case 1:
            temp_v0_2 = &Ending1MessagesTargetPosition[idx];
            DOBMoving1(arg0, temp_v0_2->x, temp_v0_2->y);
            if ((arg0->DispX == temp_v0_2->x) && (arg0->DispY == temp_v0_2->y)) {
                arg0->Pattern = 2;
            }
            break;
        case 2:
            break;
    }
}

void DOBEnding1Face(ObjBlock* arg0) {
    s32 temp_v1;
    s32 sp30;
    s32 temp_v0;
    POSXY* thing;

    temp_v1 = arg0->Kind - 0x12C;
    sp30 = Resultstat.Kart;
    temp_v0 = Driver2Pselset[sp30];
    switch (arg0->Pattern) {
        case 0:
            thing = &Ending1MessagesStartPosition[temp_v1];
            arg0->DispX = thing->x;
            arg0->DispY = thing->y;
            arg0->Pattern = 1;
            break;
        case 1:
            thing = &Ending1MessagesTargetPosition[temp_v1];
            DOBMoving1(arg0, thing->x, thing->y);
            if ((arg0->DispX == thing->x) && (arg0->DispY == thing->y)) {
                arg0->Pattern = 2;
                arg0->Work2 = 0;
            }
            break;
        case 2:
            arg0->Work2++;
            if (arg0->Work2 >= 0x1F) {
                if (Resultstat.Rank >= 3) {
                    arg0->Pattern = 4;
                    NaSeMuteTitleOff();
                    NAPlyVoiceStart(0U, (sp30 * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03));
                    NaSeMuteTitle();
                } else {
                    arg0->Pattern = 3;
                    ChangeSeqDataVS(arg0->SdNumber, 0, sp30,
                                  GetVAddress(TBSeqPselP1[temp_v0]));
                    NaSeMuteTitleOff();
                    NAPlyVoiceStart(0U, (sp30 * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    NaSeMuteTitle();
                }
            }
            break;
        case 3:
            if (SEQDataBlock[arg0->SdNumber].SequenceIndex >= P1PatNum[temp_v0]) {
                ChangeSeqDataVS(arg0->SdNumber, 0, sp30, GetVAddress(TBSeqPselP2[temp_v0]));
                arg0->Pattern = 4;
            }
            break;
        case 4:
            break;
    }
}

void DOBStaffrollMessages(ObjBlock* arg0) {
    s32 idx = arg0->Kind - 0x190;

    if ((CreditsTextRenderInfo[idx].SlideDirection == 0) || (CreditsTextRenderInfo[idx].SlideDirection != 1)) {
        DOBStaffrollType0(arg0);
    } else {
        DOBStaffrollType1(arg0);
    }
}

void DOBStaffrollType0(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 temp_v0;
    CreditsRenderInfo* temp_v1;

    temp_v0 = arg0->Kind - 0x190;
    temp_v1 = &CreditsTextRenderInfo[temp_v0];
    arg0->DispY = temp_v1->row;
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = temp_v1->StartingColumn;
            arg0->Pattern = 1;
            arg0->Work2 = temp_v1->ColumnExtra + (GetWordLength(CreditsText[temp_v0]) * temp_v1->TextScaling / 2);
            /* fallthrough */
        case 1:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (s32) (arg0->Work2 - arg0->DispX) / 4;
            if (arg0->Work1 >= 9) {
                arg0->Work1 = 8;
            }
            arg0->Workf1 = (arg0->Work1 * 0.05) + 1.0;
            if (arg0->DispX >= (arg0->Work2 - 0x14)) {
                arg0->Pattern = 2;
                arg0->SdNumber = 0;
            }
            break;
        case 2:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (arg0->Work2 - arg0->DispX) / 4;
            arg0->SdNumber += 1;
            arg0->Workf1 = ((arg0->SdNumber - 0xA) * 0.0085 * (arg0->SdNumber - 0xA)) + 0.4;
            if ((arg0->SdNumber >= 9) && ((f64) arg0->Workf1 > 1)) {
                arg0->Workf1 = 1.0f;
                arg0->Pattern = 3;
            }
            break;
        case 3:
            if ((u8) EndingMessageFlg != 0) {
                arg0->Pattern = 4;
            }
            break;
        case 4:
            DOBMoving4X(arg0, arg0->Work2, 1);
            if (arg0->DispY > 480.0) {
                arg0->Kind = 0;
            }
            break;
        default:
            break;
    }
}

void DOBStaffrollType1(ObjBlock* arg0) {
    UNUSED s32 pad;
    s32 temp_v0;
    CreditsRenderInfo* temp_v1;

    temp_v0 = arg0->Kind - 0x190;
    temp_v1 = &CreditsTextRenderInfo[temp_v0];
    arg0->DispY = temp_v1->row;
    switch (arg0->Pattern) {
        case 0:
            arg0->DispX = temp_v1->StartingColumn;
            arg0->Pattern = 1;
            arg0->Work2 = temp_v1->ColumnExtra - (GetWordLength(CreditsText[temp_v0]) * temp_v1->TextScaling / 2);
            /* fallthrough */
        case 1:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (s32) (arg0->DispX - arg0->Work2) / 4;
            if (arg0->Work1 >= 9) {
                arg0->Work1 = 8;
            }
            arg0->Workf1 = (arg0->Work1 * 0.05) + 1.0;
            if ((arg0->Work2 + 0x14) >= arg0->DispX) {
                arg0->Pattern = 2;
                arg0->SdNumber = 0;
            }
            break;
        case 2:
            DOBMoving1X(arg0, arg0->Work2);
            arg0->Work1 = (arg0->DispX - arg0->Work2) / 4;
            arg0->SdNumber += 1;
            arg0->Workf1 = ((arg0->SdNumber - 0xA) * 0.0085 * (arg0->SdNumber - 0xA)) + 0.4;
            if ((arg0->SdNumber >= 9) && ((f64) arg0->Workf1 > 1)) {
                arg0->Workf1 = 1.0f;
                arg0->Pattern = 3;
            }
            break;
        case 3:
            if ((u8) EndingMessageFlg != 0) {
                arg0->Pattern = 4;
            }
            break;
        case 4:
            DOBMoving4X(arg0, arg0->Work2, -1);
            if (arg0->DispY > 480.0) {
                arg0->Kind = 0;
            }
            break;
        default:
            break;
    }
}
