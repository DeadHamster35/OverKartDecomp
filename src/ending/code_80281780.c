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

u8 defaultCharacterIds[] = { 1, 2, 3, 4, 5, 6, 7, 0 };

void debug_switch_character_ceremony_cutscene(void) {
    if (g_DebugMode) {
        if (gControllerOne->ButtonHeld & HOLD_ALL_DPAD_AND_C_BUTTONS) {
            // Allows to switch character in debug mode?
            if (gControllerOne->ButtonHeld & U_CBUTTONS) {
                g_CharacterSelections[0] = LUIGI;
            } else if (gControllerOne->ButtonHeld & L_CBUTTONS) {
                g_CharacterSelections[0] = YOSHI;
            } else if (gControllerOne->ButtonHeld & R_CBUTTONS) {
                g_CharacterSelections[0] = TOAD;
            } else if (gControllerOne->ButtonHeld & D_CBUTTONS) {
                g_CharacterSelections[0] = DK;
            } else if (gControllerOne->ButtonHeld & U_JPAD) {
                g_CharacterSelections[0] = WARIO;
            } else if (gControllerOne->ButtonHeld & L_JPAD) {
                g_CharacterSelections[0] = PEACH;
            } else if (gControllerOne->ButtonHeld & R_JPAD) {
                g_CharacterSelections[0] = BOWSER;
            } else {
                g_CharacterSelections[0] = MARIO;
            }
            //! @todo confirm this.
            // Resets d_points_rank to default?
            bcopy(&defaultCharacterIds, &d_points_rank, 8);
        }
    }
}

s32 func_80281880(s32 arg0) {
    s32 i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        if (d_points_rank[i] == g_CharacterSelections[arg0]) {
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

    if (g_menuMultiplayerSelection != TWO_PLAYERS_SELECTED) {
        resultstat.rank = func_80281880(0);
        resultstat.kart = g_CharacterSelections[0];
        return;
    }
    // weird pattern but if it matches it matches
    temp_v0 = sp1C = func_80281880(0);
    temp_v0_2 = func_80281880(1);
    if (sp1C < temp_v0_2) {
        resultstat.kart = g_CharacterSelections[0];
        resultstat.rank = temp_v0;
    } else {
        resultstat.kart = g_CharacterSelections[1];
        resultstat.rank = temp_v0_2;
    }
}

void initial_result_sequence(void) {
    Camera* camera = &cameras[0];

    g_courseID = COURSE_ROYAL_RACEWAY;
    gBackgroundFlag = (u16) 1;
    g_ScreenFlip = 0;
    gGotoMenu = 0xFFFF;
    D_80287554 = 0;
    initital_perspective();
    initial_screen_data();
    camera->screen_view_angle = 60.0f;
    gCameraZoom[0] = 60.0f;
    D_800DC5EC->width = SCREEN_WIDTH;
    D_800DC5EC->height = SCREEN_HEIGHT;
    D_800DC5EC->posx = 160;
    D_800DC5EC->posy = 120;
    g_ScreenSplitB = SCREEN_MODE_1P;
    FreeMemoryPointer = (s32) StaticMemoryPointer;
    VertexMemoryPointer = VERTEX_MEMORY_TOP;
    ObjectMemoryPointer = (uintptr_t) ok_ObjectPoolStart;
    g_ScreenSplitA = SCREEN_MODE_1P;
    g_gameMode = GRAND_PRIX;
    load_course(g_courseID);
    map_memory_pointer = (s32) FreeMemoryPointer;
    SetSegment(0xB, (void*) load_press_data((u8*) CEREMONY_DATA_ROM_START, (u8*) CEREMONY_DATA_ROM_END));
    SetSegment(6, (void*) load_press_data((u8*) &_course_banshee_boardwalk_dl_mio0SegmentRomStart,
                                                         (u8*) &_course_yoshi_valley_dl_mio0SegmentRomStart));
    g_waterHeight = -2000.0f;

    minimum_x = -0x15A1;
    minimum_y = -0x15A1;
    minimum_z = -0x15A1;

    maximum_x = 0x15A1;
    maximum_y = 0x15A1;
    maximum_z = 0x15A1;

    wall_flag = 0;
    flat_flag = 0;
    facenumber = 0;
    g_courseFaceMaxIndex = (u16) 0;
    g_skyToggle = (u16) 0;
    g_fadeOutCounter2 = (u16) 0;
    gFaceBuffer = (FaceStruct*) FACE_BUFFER_START;
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
    debug_switch_character_ceremony_cutscene();
    func_802818BC();
    initial_game();
    initialize_params();
    oga_hyousyou_init();
    balloons_and_fireworks_init();
    init_camera_podium_ceremony();
    init_kawanoResult();
    D_801625F8 = (s32) LastMemoryPointer - FreeMemoryPointer;
    D_801625FC = ((f32) D_801625F8 / 1000.0f);
}
