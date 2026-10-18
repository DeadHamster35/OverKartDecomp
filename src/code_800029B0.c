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

extern s32 FinishCount;
extern s16 Check2PGP;
#if !ENABLE_CUSTOM_COURSE_ENGINE
s16 CourseID = 0;
#endif
s16 LoadedcourseFlag = 0xFF;
u16 MusicUserVolumeFlag = 0;
s32 D_800DC5AC = 0;
u16 ClearCFBFlag = 1;
u16 BackgroundFlag = 0;
u16 TwoDFlag = 0;
u16 SkyToggle = 0;
u16 FadeOutFlag = 0;
u16 FadeOutCounter = 0;
u16 FadeOutCounter2 = 0;
UNUSED u16 D_800DC5CC = 0;
s32 D_800DC5D0 = 0;
s32 D_800DC5D4 = 0;
s32 D_800DC5D8 = 0;
s32 D_800DC5DC = 64;

s32 D_800DC5E0 = 32;

// This is tracking which credit "State" we're in, decides which credits are shown (and probably other stuff)
u16 D_800DC5E4 = 0;

//! @todo WinKart (D_800DC5E8) accessed as word, D_800DC5EB as u8
s32 WinKart = 0;

ALIGNED16 Screen D_8015F480[4];
Screen* D_800DC5EC = &D_8015F480[0];
Screen* D_800DC5F0 = &D_8015F480[1];
Screen* D_800DC5F4 = &D_8015F480[2];
Screen* D_800DC5F8 = &D_8015F480[3];
u16 PauseFlag = 0; // 1 if the game is paused and 0 if the game is not paused
u8* PAppNmiBuffer = (u8*) &osAppNmiBuffer;

s32 ScreenFlip = 0;
f32 ScreenStretch = 1.0f;
Lights1 D_800DC610[] = {
    gdSPDefLights1(175, 175, 175, 255, 255, 255, 0, 0, 120),
    gdSPDefLights1(115, 115, 115, 255, 255, 255, 0, 0, 120),
};
UNUSED s32 Pad800029B0 = 0x80000000;
s16 CreditsCourseId = COURSE_LUIGI_RACEWAY;
s16 ItemSetFlag = 1;

// Technically a Pointer to an array, but declaring it so creates regalloc issues.
FaceStruct* FaceBuffer;
u16* CollisionTable;
u16 CourseFaceMaxIndex; // Number of Entries in FaceBuffer
u16 CollisionPointer;
u32 Facenumber;

Vec3f D_8015F590;
s32 WallFlag;
s32 FlatFlag;
s32 D_8015F5A4;
s32 Code800029B0BssPad[48];
Vtx* VertexBuffer2[32];

s16 MaximumX;
s16 MinimumX;

s16 MaximumY; // s16 or u16?
s16 MinimumY;

s16 MaximumZ;
s16 MinimumZ;

s16 Maxlimmity;
s16 Minlimmity;
u16 PointNumber;
s16 Shadowface;
s16 Outsideface;
/**
 * Incremented by one every Time the Player spawns a new shell actor.
 * A cleaning routine runs once twenty-one shells have spawned.
 */
u16 KameCounter;

u16 MooSoundCounter;
u16 MooSoundPointer;
f32 MooSoundLastDistance;
Vec3f MooSoundPosition;
UNUSED u32 D_8015F718[3]; // Likely held ptrs to segmented data.
size_t FreeMemorySize;
uintptr_t FreeMemoryPointer;
uintptr_t LastMemoryPointer;
uintptr_t VertexMemoryPointer;
uintptr_t ObjectMemoryPointer;
u32 MapMemoryPointer;
uintptr_t StaticMemoryPointer;
Vec3f WaterfallSoundPos;
Vec3f CrowdSoundPos1;
Vec3f CrowdSoundPos2;
Vec3f CrowdSoundPos3;
Vec3f CrowdSoundPos4;

f32 MirrorValue; // Extra Mode, flips vertices.
s32 ScreenCounter;
s32 D_8015F790[64]; // Unknown data, potentially not Used.
u16 ReplayFlag;
u16 RetryFlag;
u16 FinishZoomFlag;
// Indexed by Player ID. Track Time in Seconds since Player has last crossed the finish line
f32 TimePlayerLastTouchedFinishLine[8];

u8* Wincount2P;
u8* Wincount3P;
u8* Wincount4P;
u8* Battlewincount2P;
u8* Battlewincount3P;
u8* Battlewincount4P;

Vec3f GoalBannerPos;
s32 ItemboxFlag;

s32 TreeFlag;
f32 WaterHeight;
f32 WaterVelocity;
s16 RankBuffer[8]; // Player Index at each Position
u16 StaticObjectCount;
s32 Checkmatrix[44];

Object SimpleObjectArray[ACTOR_LIST_SIZE];
//! @warning todo: Is this apart of the actor array?
UNUSED u8 D_80162578[sizeof(Object)];

s16 DebugPathCount;
s16 SIsController1Unplugged;
s32 FogRed;
s32 FogGreen;
s32 FogBlue;
s32 D_801625F8;
f32 D_801625FC;

void SetMusicVolume(void) {
    switch (MusicUserVolumeFlag) {
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

    PlayerCount = MenuMultiplayerSelection;
    if (GameState != RACING) {
        ScreenFlip = 0;
    }
    if (ScreenFlip) {
        MirrorValue = -1.0f;
    } else {
        MirrorValue = 1.0f;
    }
    if (GameMode == GRAND_PRIX) {
        CourseID = CupCourseOrder[CupSelect][CourseSelect];
    }
    ScreenSplitA = ScreenSplitB;
    /* HotSwap GP pins CourseID to 0, so the stock skip would reuse the first
     * course's decoded verts while CourseSelect-driven objects still update. */
    if ((HotSwapID > 0) || (CourseID != LoadedcourseFlag)) {
        RaceClearFlag = 0;
        LoadedcourseFlag = CourseID;
        MapStartupDefault(CourseID);
        InitialMapCode();
        MapMemoryPointer = FreeMemoryPointer;
    } else {
        FreeMemoryPointer = MapMemoryPointer;
    }
    InitialMap2();
    InitialGhost();
    InitialGame();
    InitializeParams();
    StartingIndicator = RACE_NONE;
    KameCounter = 0;
    TwoDFlag = 0;
    CameraPoint = 0;
    FinishCount = -1;
    Check2PGP = 0;
    InitialScreenData();
    InititalPerspective();
    InitKawanoGame();
    InitialObject();

    if (GameMode != BATTLE) {
        GoalBannerPos[1] = (f32) (PathPointPointer->Pointy - 15);
        GoalBannerPos[2] = PathPointPointer->Pointz;
        if (CourseID == COURSE_TOADS_TURNPIKE) {
            GoalBannerPos[0] = (ScreenFlip != 0) ? PathPointPointer->Pointx + 138.0f : PathPointPointer->Pointx - 138.0f;
        } else if (CourseID == COURSE_WARIO_STADIUM) {
            GoalBannerPos[0] = (ScreenFlip != 0) ? PathPointPointer->Pointx + 12.0f : PathPointPointer->Pointx - 12.0f;
        } else {
            GoalBannerPos[0] = PathPointPointer->Pointx;
        }
    }
    if (!DemoFlag) {
        NaChangeSoundMode(PlayerCount - 1, CourseID + 4);
        NasAudioInput();
    }

    controller = ControllerOne;

    for (i = 0; i < 7; i++, controller++) {
        controller->AnalogX = 0;
        controller->AnalogY = 0;
        controller->ButtonPressed = 0;
        controller->ButtonReleased = 0;
        controller->ButtonHeld = 0;
    }
}

// Sound related
void DelayInitialMap(void) {
    if (HotSwapID != 0) {
        return;
    }
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            Vec3FSet(CrowdSoundPos1, -223.0f, 94.0f, -155.0f);
            NaSceneLevelStart(CrowdSoundPos1, D_802B91C8, 0x5103700B);
            break;
        case COURSE_ROYAL_RACEWAY:
            Vec3FSet(CrowdSoundPos1, 177.0f, 87.0f, -393.0f);
            NaSceneLevelStart(CrowdSoundPos1, D_802B91C8, 0x5103700B);
            break;
        case COURSE_LUIGI_RACEWAY:
            Vec3FSet(CrowdSoundPos1, 85.0f, 21.0f, -219.0f);
            NaSceneLevelStart(CrowdSoundPos1, D_802B91C8, 0x5103700B);
            break;
        case COURSE_WARIO_STADIUM:
            Vec3FSet(CrowdSoundPos1, 298.0f, 202.0f, -850.0f);
            NaSceneLevelStart(CrowdSoundPos1, D_802B91C8, 0x5103700B);
            Vec3FSet(CrowdSoundPos2, -1600.0f, 202.0f, -2430.0f);
            NaSceneLevelStart(CrowdSoundPos2, D_802B91C8, 0x5103700B);
            Vec3FSet(CrowdSoundPos3, -2708.0f, 202.0f, 1762.0f);
            NaSceneLevelStart(CrowdSoundPos3, D_802B91C8, 0x5103700B);
            Vec3FSet(CrowdSoundPos4, -775.0f, 202.0f, 1930.0f);
            NaSceneLevelStart(CrowdSoundPos4, D_802B91C8, 0x5103700B);
            break;
        case COURSE_KOOPA_BEACH:
            Vec3FSet(WaterfallSoundPos, 153.0f, 0.0f, 2319.0f);
            NaSceneLevelStart(WaterfallSoundPos, D_802B91C8, 0x51028001);
            break;
        case COURSE_DK_JUNGLE:
            Vec3FSet(WaterfallSoundPos, -790.0f, -255.0f, -447.0f);
            NaSceneLevelStart(WaterfallSoundPos, D_802B91C8, 0x51028001);
            break;
        default:
            break;
    }
#else

#endif
}

/**
 * The nmi Buffer allows developers to save 64 bytes of data through a soft reset.
 */
void InitializeFirstOnce(void) {
    s32 i;
    for (i = 0; i < 16; i++) {
        osAppNmiBuffer[i] = 0;
    }
}

void InitialEndingMap(void) {
    Vec3f Position;
    Vec3f Velocity = { 0, 0, 0 };
    Vec3s Rotation = { 0, 0, 0 };

    SkyToggle = 0;
    FadeOutCounter2 = 0;
    SimpleObjectCount = 0;
    ScreenFlip = 0;
    MirrorValue = 1.0f;

    PlayerCount = 1;

    //! @warning Grand Prix loads some Textures that credits does not
    // Therefore, this calculation allows future Texture allocations to skip 0x9000
    // this allows hard-coded data addresses to sync with memory data.
    // In other words, the first Texture in segment 3 for credits should Start at 0x9000
    // This is only required for moo moo farm.
    // This is also bad memory management practice as this could result in overwriting the wrong memory.
    SetSegment(0x3, (void*) (FreeMemoryPointer - 0x9000));
    InitialObjectBuffer();
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
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
            Vec3FSet(Position, -2300.0f, 0.0f, 634.0f);
            Position[0] *= MirrorValue;
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_YOSHI_EGG);
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
            Vec3FSet(Position, -131.0f, 83.0f, 286.0f);
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_WARIO_SIGN);
            Vec3FSet(Position, -2353.0f, 72.0f, -1608.0f);
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_WARIO_SIGN);
            Vec3FSet(Position, -2622.0f, 79.0f, 739.0f);
            AddObjectBuffer(Position, Rotation, Velocity, ACTOR_WARIO_SIGN);
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
    StaticObjectCount = SimpleObjectCount;
}
