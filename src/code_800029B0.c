#include <ultra64.h>
#include <macros.h>
#include <mk64.h>

#include "code_800029B0.h"
#include "memory.h"
#include "path.h"
#include "actors.h"
#include "actor_types.h"
#include "math_util.h"
#include "audio/external.h"
#include <defines.h>
#include "collision.h"
#include "memory.h"
#include "menu_items.h"
#include "skybox_and_splitscreen.h"
#include "code_8006E9C0.h"
#include "spawn_players.h"
#include "replays.h"
#include "render_courses.h"
#include "main.h"
#include "courses/all_course_data.h"
#include "courses/all_course_packed.h"
#include "menus.h"
#include "data/other_textures.h"
#include "OverKartHooks.h"

extern s32 finish_count;
extern s16 check_2p_GP;
#if !ENABLE_CUSTOM_COURSE_ENGINE
s16 g_courseID = 0;
#endif
s16 g_loadedcourseFlag = 0xFF;
u16 g_musicUserVolumeFlag = 0;
s32 D_800DC5AC = 0;
u16 g_ClearCFBFlag = 1;
u16 gBackgroundFlag = 0;
u16 two_d_flag = 0;
u16 g_skyToggle = 0;
u16 g_fadeOutFlag = 0;
u16 g_fadeOutCounter = 0;
u16 g_fadeOutCounter2 = 0;
UNUSED u16 D_800DC5CC = 0;
s32 D_800DC5D0 = 0;
s32 D_800DC5D4 = 0;
s32 D_800DC5D8 = 0;
s32 D_800DC5DC = 64;

s32 D_800DC5E0 = 32;

// This is tracking which credit "state" we're in, decides which credits are shown (and probably other stuff)
u16 D_800DC5E4 = 0;

//! @todo win_kart (D_800DC5E8) accessed as word, D_800DC5EB as u8
s32 win_kart = 0;

ALIGNED16 Screen D_8015F480[4];
Screen* D_800DC5EC = &D_8015F480[0];
Screen* D_800DC5F0 = &D_8015F480[1];
Screen* D_800DC5F4 = &D_8015F480[2];
Screen* D_800DC5F8 = &D_8015F480[3];
u16 pause_flag = 0; // 1 if the game is paused and 0 if the game is not paused
u8* pAppNmiBuffer = (u8*) &osAppNmiBuffer;

s32 g_ScreenFlip = 0;
f32 g_ScreenStretch = 1.0f;
Lights1 D_800DC610[] = {
    gdSPDefLights1(175, 175, 175, 255, 255, 255, 0, 0, 120),
    gdSPDefLights1(115, 115, 115, 255, 255, 255, 0, 0, 120),
};
UNUSED s32 pad_800029B0 = 0x80000000;
s16 gCreditsCourseId = COURSE_LUIGI_RACEWAY;
s16 g_ItemSetFlag = 1;

// Technically a pointer to an array, but declaring it so creates regalloc issues.
FaceStruct* gFaceBuffer;
u16* collision_table;
u16 g_courseFaceMaxIndex; // Number of entries in gFaceBuffer
u16 collision_pointer;
u32 facenumber;

Vec3f D_8015F590;
s32 wall_flag;
s32 flat_flag;
s32 D_8015F5A4;
s32 code_800029B0_bss_pad[48];
Vtx* vertex_buffer2[32];

s16 maximum_x;
s16 minimum_x;

s16 maximum_y; // s16 or u16?
s16 minimum_y;

s16 maximum_z;
s16 minimum_z;

s16 maxlimmity;
s16 minlimmity;
u16 point_number;
s16 shadowface;
s16 outsideface;
/**
 * Incremented by one every time the player spawns a new shell actor.
 * A cleaning routine runs once twenty-one shells have spawned.
 */
u16 kame_counter;

u16 g_mooSoundCounter;
u16 g_mooSoundPointer;
f32 g_mooSoundLastDistance;
Vec3f g_mooSoundPosition;
UNUSED u32 D_8015F718[3]; // Likely held ptrs to segmented data.
size_t free_memory_size;
uintptr_t FreeMemoryPointer;
uintptr_t LastMemoryPointer;
uintptr_t VertexMemoryPointer;
uintptr_t ObjectMemoryPointer;
u32 map_memory_pointer;
uintptr_t StaticMemoryPointer;
Vec3f g_waterfallSoundPos;
Vec3f g_crowdSoundPos1;
Vec3f g_crowdSoundPos2;
Vec3f g_crowdSoundPos3;
Vec3f g_crowdSoundPos4;

f32 g_mirrorValue; // Extra mode, flips vertices.
s32 g_ScreenCounter;
s32 D_8015F790[64]; // Unknown data, potentially not used.
u16 replay_flag;
u16 retry_flag;
u16 finish_zoom_flag;
// Indexed by Player ID. Track time in seconds since player has last crossed the finish line
f32 gTimePlayerLastTouchedFinishLine[8];

u8* wincount2p;
u8* wincount3p;
u8* wincount4p;
u8* battlewincount2p;
u8* battlewincount3p;
u8* battlewincount4p;

Vec3f g_goalBannerPos;
s32 itembox_flag;

s32 tree_flag;
f32 g_waterHeight;
f32 g_waterVelocity;
s16 rank_buffer[8]; // Player index at each position
u16 g_StaticObjectCount;
s32 checkmatrix[44];

Object g_SimpleObjectArray[ACTOR_LIST_SIZE];
//! @warning todo: Is this apart of the actor array?
UNUSED u8 D_80162578[sizeof(Object)];

s16 gDebugPathCount;
s16 sIsController1Unplugged;
s32 fog_red;
s32 fog_green;
s32 fog_blue;
s32 D_801625F8;
f32 D_801625FC;

void set_music_volume(void) {
    switch (g_musicUserVolumeFlag) {
        case 0:
            NAMusicVolume(127);
            break;
        case 1:
            NAMusicVolume(75);
            break;
        case 2:
            NAMusicVolume(0);
            break;
    }
}

void InitialRaceSequence(void) {
    struct Controller* controller;
    int i;

    g_playerCount = g_menuMultiplayerSelection;
    if (gGamestate != RACING) {
        g_ScreenFlip = 0;
    }
    if (g_ScreenFlip) {
        g_mirrorValue = -1.0f;
    } else {
        g_mirrorValue = 1.0f;
    }
    if (g_gameMode == GRAND_PRIX) {
        g_courseID = gCupCourseOrder[g_cupSelect][g_courseSelect];
    }
    g_ScreenSplitA = g_ScreenSplitB;
    OkDbgLoad(1, g_courseID, 0, 0, "RaceInit");
    if (g_courseID != g_loadedcourseFlag) {
        race_clear_flag = 0;
        g_loadedcourseFlag = g_courseID;
        MapStartupDefault(g_courseID);
        InitialMapCode();
        map_memory_pointer = FreeMemoryPointer;
    } else {
        FreeMemoryPointer = map_memory_pointer;
    }
    initial_map2();
    initial_ghost();
    initial_game();
    initialize_params();
    g_startingIndicator = RACE_NONE;
    kame_counter = 0;
    two_d_flag = 0;
    camera_point = 0;
    finish_count = -1;
    check_2p_GP = 0;
    initial_screen_data();
    initital_perspective();
    init_kawano_game();
    initial_object();

    if (g_gameMode != BATTLE) {
        g_goalBannerPos[1] = (f32) (g_pathPointPointer->pointy - 15);
        g_goalBannerPos[2] = g_pathPointPointer->pointz;
        if (g_courseID == COURSE_TOADS_TURNPIKE) {
            g_goalBannerPos[0] = (g_ScreenFlip != 0) ? g_pathPointPointer->pointx + 138.0f : g_pathPointPointer->pointx - 138.0f;
        } else if (g_courseID == COURSE_WARIO_STADIUM) {
            g_goalBannerPos[0] = (g_ScreenFlip != 0) ? g_pathPointPointer->pointx + 12.0f : g_pathPointPointer->pointx - 12.0f;
        } else {
            g_goalBannerPos[0] = g_pathPointPointer->pointx;
        }
    }
    if (!g_DemoFlag) {
        Na_ChangeSoundMode(g_playerCount - 1, g_courseID + 4);
        Nas_AudioInput();
    }

    controller = gControllerOne;

    for (i = 0; i < 7; i++, controller++) {
        controller->AnalogX = 0;
        controller->AnalogY = 0;
        controller->ButtonPressed = 0;
        controller->ButtonReleased = 0;
        controller->ButtonHeld = 0;
    }
}

// sound related
void DelayInitialMap(void) {
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            vec3f_set(g_crowdSoundPos1, -223.0f, 94.0f, -155.0f);
            NaSceneLevelStart(g_crowdSoundPos1, D_802B91C8, 0x5103700B);
            break;
        case COURSE_ROYAL_RACEWAY:
            vec3f_set(g_crowdSoundPos1, 177.0f, 87.0f, -393.0f);
            NaSceneLevelStart(g_crowdSoundPos1, D_802B91C8, 0x5103700B);
            break;
        case COURSE_LUIGI_RACEWAY:
            vec3f_set(g_crowdSoundPos1, 85.0f, 21.0f, -219.0f);
            NaSceneLevelStart(g_crowdSoundPos1, D_802B91C8, 0x5103700B);
            break;
        case COURSE_WARIO_STADIUM:
            vec3f_set(g_crowdSoundPos1, 298.0f, 202.0f, -850.0f);
            NaSceneLevelStart(g_crowdSoundPos1, D_802B91C8, 0x5103700B);
            vec3f_set(g_crowdSoundPos2, -1600.0f, 202.0f, -2430.0f);
            NaSceneLevelStart(g_crowdSoundPos2, D_802B91C8, 0x5103700B);
            vec3f_set(g_crowdSoundPos3, -2708.0f, 202.0f, 1762.0f);
            NaSceneLevelStart(g_crowdSoundPos3, D_802B91C8, 0x5103700B);
            vec3f_set(g_crowdSoundPos4, -775.0f, 202.0f, 1930.0f);
            NaSceneLevelStart(g_crowdSoundPos4, D_802B91C8, 0x5103700B);
            break;
        case COURSE_KOOPA_BEACH:
            vec3f_set(g_waterfallSoundPos, 153.0f, 0.0f, 2319.0f);
            NaSceneLevelStart(g_waterfallSoundPos, D_802B91C8, 0x51028001);
            break;
        case COURSE_DK_JUNGLE:
            vec3f_set(g_waterfallSoundPos, -790.0f, -255.0f, -447.0f);
            NaSceneLevelStart(g_waterfallSoundPos, D_802B91C8, 0x51028001);
            break;
        default:
            break;
    }
#else

#endif
}

/**
 * The nmi buffer allows developers to save 64 bytes of data through a soft reset.
 */
void initialize_first_once(void) {
    s32 i;
    for (i = 0; i < 16; i++) {
        osAppNmiBuffer[i] = 0;
    }
}

void initial_ending_map(void) {
    Vec3f position;
    Vec3f velocity = { 0, 0, 0 };
    Vec3s rotation = { 0, 0, 0 };

    g_skyToggle = 0;
    g_fadeOutCounter2 = 0;
    g_simpleObjectCount = 0;
    g_ScreenFlip = 0;
    g_mirrorValue = 1.0f;

    g_playerCount = 1;

    //! @warning Grand Prix loads some textures that credits does not
    // Therefore, this calculation allows future texture allocations to skip 0x9000
    // this allows hard-coded data addresses to sync with memory data.
    // In other words, the first texture in segment 3 for credits should start at 0x9000
    // This is only required for moo moo farm.
    // This is also bad memory management practice as this could result in overwriting the wrong memory.
    SetSegment(0x3, (void*) (FreeMemoryPointer - 0x9000));
    initial_object_buffer();
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            DecodeMapImage1(gTextureTrees1, 0x35B, 0x800);
            SetTreeObject(d_course_mario_raceway_tree_spawns);
            break;
        case COURSE_BOWSER_CASTLE:
            // d_course_bowsers_castle_packed_dl_1350
            MakeWaterVertex(0x07001350, 0x32, 0, 0, 0);
            break;
        case COURSE_BANSHEE_BOARDWALK:
            // d_course_banshee_boardwalk_packed_dl_878
            MakeWaterVertex(0x07000878, 128, 0, 0, 0);
            break;
        case COURSE_YOSHI_VALLEY:
            vec3f_set(position, -2300.0f, 0.0f, 634.0f);
            position[0] *= g_mirrorValue;
            addObjectBuffer(position, rotation, velocity, ACTOR_YOSHI_EGG);
            break;
        case COURSE_MOO_MOO_FARM:
            DecodeMapImage1(gTextureTrees4Left, 0x3E8, 0x800);
            DecodeMapImage1(gTextureTrees4Right, 0x3E8, 0x800);
            DecodeMapImage1(gTextureCow01Left, 0x400, 0x800);
            DecodeMapImage1(gTextureCow01Right, 0x400, 0x800);
            DecodeMapImage1(gTextureCow02Left, 0x400, 0x800);
            DecodeMapImage1(gTextureCow02Right, 0x400, 0x800);
            DecodeMapImage1(gTextureCow03Left, 0x400, 0x800);
            DecodeMapImage1(gTextureCow03Right, 0x400, 0x800);
            DecodeMapImage1(gTextureCow04Left, 0x400, 0x800);
            DecodeMapImage1(gTextureCow04Right, 0x400, 0x800);
            DecodeMapImage1(gTextureCow05Left, 0x400, 0x800);
            DecodeMapImage1(gTextureCow05Right, 0x400, 0x800);
            SetTreeObject(d_course_moo_moo_farm_tree_spawn);
            break;
        case COURSE_SHERBET_LAND:
            // d_course_sherbet_land_packed_dl_1EB8
            MakeWaterVertex(0x07001EB8, 180, 0xFF, 0xFF, 0xFF);
            // d_course_sherbet_land_packed_dl_2308
            MakeWaterVertex(0x07002308, 150, 0xFF, 0xFF, 0xFF);
            break;
        case COURSE_RAINBOW_ROAD:
            // d_course_rainbow_road_packed_dl_2068
            MakeWaterVertex(0x07002068, 150, 0xFF, 0xFF, 0xFF);
            // d_course_rainbow_road_packed_dl_1E18
            MakeWaterVertex(0x07001E18, 150, 0xFF, 0xFF, 0xFF);
            // d_course_rainbow_road_packed_dl_1318
            MakeWaterVertex(0x07001318, 255, 0xFF, 0xFF, 0);
            break;
        case COURSE_WARIO_STADIUM:
            vec3f_set(position, -131.0f, 83.0f, 286.0f);
            addObjectBuffer(position, rotation, velocity, ACTOR_WARIO_SIGN);
            vec3f_set(position, -2353.0f, 72.0f, -1608.0f);
            addObjectBuffer(position, rotation, velocity, ACTOR_WARIO_SIGN);
            vec3f_set(position, -2622.0f, 79.0f, 739.0f);
            addObjectBuffer(position, rotation, velocity, ACTOR_WARIO_SIGN);
            // d_course_wario_stadium_packed_dl_C50
            MakeWaterVertex(0x07000C50, 0x64, 0xFF, 0xFF, 0xFF);
            // d_course_wario_stadium_packed_dl_BD8
            MakeWaterVertex(0x07000BD8, 0x64, 0xFF, 0xFF, 0xFF);
            // d_course_wario_stadium_packed_dl_B60
            MakeWaterVertex(0x07000B60, 0x64, 0xFF, 0xFF, 0xFF);
            // d_course_wario_stadium_packed_dl_AE8
            MakeWaterVertex(0x07000AE8, 0x64, 0xFF, 0xFF, 0xFF);
            // d_course_wario_stadium_packed_dl_CC8
            MakeWaterVertex(0x07000CC8, 0x64, 0xFF, 0xFF, 0xFF);
            // d_course_wario_stadium_packed_dl_D50
            MakeWaterVertex(0x07000D50, 0x64, 0xFF, 0xFF, 0xFF);
            // d_course_wario_stadium_packed_dl_DD0
            MakeWaterVertex(0x07000DD0, 0x64, 0xFF, 0xFF, 0xFF);
            // d_course_wario_stadium_packed_dl_E48
            MakeWaterVertex(0x07000E48, 0x64, 0xFF, 0xFF, 0xFF);
            break;
        case COURSE_DK_JUNGLE:
            // d_course_dks_jungle_parkway_packed_dl_3FA8
            MakeWaterVertex(0x07003FA8, 0x78, 0xFF, 0xFF, 0xFF);
            break;
        default:
            break;
    }
#else

#endif
    g_StaticObjectCount = g_simpleObjectCount;
}
