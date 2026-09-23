#include <ultra64.h>
#include <macros.h>
#include <defines.h>
#include <segments.h>
#include <mk64.h>
#include <course.h>

#include "code_80281780.h"
#include "memory.h"
#include "camera.h"
#include "camera_junk.h"
#include "spawn_players.h"
#include "skybox_and_splitscreen.h"
#include "code_8006E9C0.h"
#include "podium_ceremony_actors.h"
#include "cpu_vehicles_camera_path.h"
#include "collision.h"
#include "code_80281C40.h"
#include "code_800029B0.h"
#include "menu_items.h"
#include "main.h"
#include "menus.h"
#include "render_courses.h"

u8 DefaultCharacterIds[] = { 1, 2, 3, 4, 5, 6, 7, 0 };

void DebugSwitchCharacterCeremonyCutscene(void) {
    if (DebugMode) {
        if (ControllerOne->ButtonHeld & HOLD_ALL_DPAD_AND_C_BUTTONS) {
            // Allows to switch character in debug Mode?
            if (ControllerOne->ButtonHeld & U_CBUTTONS) {
                CharacterSelections[0] = LUIGI;
            } else if (ControllerOne->ButtonHeld & L_CBUTTONS) {
                CharacterSelections[0] = YOSHI;
            } else if (ControllerOne->ButtonHeld & R_CBUTTONS) {
                CharacterSelections[0] = TOAD;
            } else if (ControllerOne->ButtonHeld & D_CBUTTONS) {
                CharacterSelections[0] = DK;
            } else if (ControllerOne->ButtonHeld & U_JPAD) {
                CharacterSelections[0] = WARIO;
            } else if (ControllerOne->ButtonHeld & L_JPAD) {
                CharacterSelections[0] = PEACH;
            } else if (ControllerOne->ButtonHeld & R_JPAD) {
                CharacterSelections[0] = BOWSER;
            } else {
                CharacterSelections[0] = MARIO;
            }
            //! @todo confirm this.
            // Resets DPointsRank to default?
            bcopy(&DefaultCharacterIds, &DPointsRank, 8);
        }
    }
}

s32 func_80281880(s32 arg0) {
    s32 i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (DPointsRank[i] == CharacterSelections[arg0]) {
            break;
        }
    }
    return i;
}

void func_802818BC(void) {
    s32 temp_v0;
    UNUSED s32 pad;
    s32 sp1C;
    s32 temp_v0_2;

    if (MenuMultiplayerSelection != TWO_PLAYERS_SELECTED) {
        Resultstat.Rank = func_80281880(0);
        Resultstat.Kart = CharacterSelections[0];
        return;
    }
    // weird Pattern but if it matches it matches
    temp_v0 = sp1C = func_80281880(0);
    temp_v0_2 = func_80281880(1);
    if (sp1C < temp_v0_2) {
        Resultstat.Kart = CharacterSelections[0];
        Resultstat.Rank = temp_v0;
    } else {
        Resultstat.Kart = CharacterSelections[1];
        Resultstat.Rank = temp_v0_2;
    }
}

void InitialResultSequence(void) {
    Camera* Camera = &Cameras[0];

    CourseID = COURSE_ROYAL_RACEWAY;
    BackgroundFlag = (u16) 1;
    ScreenFlip = 0;
    GotoMenu = 0xFFFF;
    D_80287554 = 0;
    InititalPerspective();
    InitialScreenData();
    Camera->ScreenViewAngle = 60.0f;
    CameraZoom[0] = 60.0f;
    D_800DC5EC->Width = SCREEN_WIDTH;
    D_800DC5EC->Height = SCREEN_HEIGHT;
    D_800DC5EC->Posx = 160;
    D_800DC5EC->Posy = 120;
    ScreenSplitB = SCREEN_MODE_1P;
    FreeMemoryPointer = (s32) StaticMemoryPointer;
    VertexMemoryPointer = VERTEX_MEMORY_TOP;
    ObjectMemoryPointer = (uintptr_t) ok_ObjectPoolStart;
    ScreenSplitA = SCREEN_MODE_1P;
    GameMode = GRAND_PRIX;
    LoadCourse(CourseID);
    MapMemoryPointer = (s32) FreeMemoryPointer;
    SetSegment(0xB, (void*) LoadPressData((u8*) CEREMONY_DATA_ROM_START, (u8*) CEREMONY_DATA_ROM_END));
    SetSegment(6, (void*) LoadPressData((u8*) &_course_banshee_boardwalk_dl_mio0SegmentRomStart,
                                                         (u8*) &_course_yoshi_valley_dl_mio0SegmentRomStart));
    WaterHeight = -2000.0f;

    MinimumX = -0x15A1;
    MinimumY = -0x15A1;
    MinimumZ = -0x15A1;

    MaximumX = 0x15A1;
    MaximumY = 0x15A1;
    MaximumZ = 0x15A1;

    WallFlag = 0;
    FlatFlag = 0;
    Facenumber = 0;
    CourseFaceMaxIndex = (u16) 0;
    SkyToggle = (u16) 0;
    FadeOutCounter2 = (u16) 0;
    FaceBuffer = (FaceStruct*) FACE_BUFFER_START;
    //! @bug these segmented addresses need to be symbols for mobility
    // d_course_royal_raceway_packed_dl_67E8
    SearchList2((Gfx*) 0x070067E8, -1);
    // d_course_royal_raceway_packed_dl_AEF8
    SearchList2((Gfx*) 0x0700AEF8, -1);
    // d_course_royal_raceway_packed_dl_A970
    SearchList2((Gfx*) 0x0700A970, 8);
    // d_course_royal_raceway_packed_dl_AC30
    SearchList2((Gfx*) 0x0700AC30, 8);
    // d_course_royal_raceway_packed_dl_CE0
    SearchList2((Gfx*) 0x07000CE0, 0x10);
    // d_course_royal_raceway_packed_dl_E88
    SearchList2((Gfx*) 0x07000E88, 0x10);
    // d_course_royal_raceway_packed_dl_A618
    SearchList2((Gfx*) 0x0700A618, -1);
    // d_course_royal_raceway_packed_dl_A618
    SearchList2((Gfx*) 0x0700A618, -1);
    // d_course_royal_raceway_packed_dl_23F8
    SearchList2((Gfx*) 0x070023F8, 1);
    // d_course_royal_raceway_packed_dl_2478
    SearchList2((Gfx*) 0x07002478, 1);
    MakeCollision();
    DebugSwitchCharacterCeremonyCutscene();
    func_802818BC();
    InitialGame();
    InitializeParams();
    OgaHyousyouInit();
    BalloonsAndFireworksInit();
    InitCameraPodiumCeremony();
    InitKawanoResult();
    D_801625F8 = (s32) LastMemoryPointer - FreeMemoryPointer;
    D_801625FC = ((f32) D_801625F8 / 1000.0f);
}
