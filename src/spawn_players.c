#include <defines.h>
#include <mk64.h>
#include <course.h>

#include "spawn_players.h"
#include "code_800029B0.h"
#include "kart_attributes.h"
#include "memory.h"
#include "path.h"
#include "buffers.h"
#include "kart_dma.h"
#include "camera.h"
#include "math_util.h"
#include "player_controller.h"
#include "code_80057C60.h"
#include "collision.h"
#include "render_courses.h"
#include "replays.h"
#include "cpu_vehicles_camera_path.h"
#include "render_player.h"
#include "podium_ceremony_actors.h"
#include "main.h"
#include "menus.h"
#include "render_player.h"
#include "menu_items.h"
#include "effects.h"
#include "decode.h"

f32 grid_x[8];
f32 grid_z[8];
UNUSED f32 grid_y[8];
s16 kartrank[8];
f32 accele_count[8];
f32 g_waterlevelPlayer[8];
s32 sterr_counter[8];
s32 green_timer[8];
s16 echo_flag[8];
// Shadows values from CurrentPathID, but is an array
u16 jugemu_line[8];
// Shadows values from gNearestPathPointByPlayerId, but is an array
s16 g_playerPathPointCopy[8];
s16 g_offroadFlagPlayer1[8];
s16 tire_hight;
UNUSED s32 D_80165348[29];
Player* hitPtr[8];

bool accele_onoff_flag[8];
s32 accele_inc_flag[8];
s32 accele_time[8];
s32 accele_flag[8];
bool power_band_accele[8];
s32 power_band_timer[8];

bool brake_onoff_flag[8];
s32 brake_inc_flag[8];
s32 brake_timer[8];
s32 brake_flag[8];
bool full_brake_accele[8];
s32 full_brake_timer[8];

s16 cpu_chooseCharacters[7];

s16 D_8016556E;
s16 select_dmaKart;
s16 select_reardmaKart;
s16 select_NondmaKart;
s16 select_rearNondmaKart;
s16 select_viewKart;
s16 select_rearviewKart;
s16 select_bumpKart;
s16 select_rearbumpKart;
s16 select_viewObj;
s16 select_rearviewObj;

// arg4 is height? Or something like that?
void initializePlayer(Player* player, s8 playerIndex, f32 startingRow, f32 startingColumn, f32 arg4, f32 arg5,
                  u16 characterId, s16 playerType) {
    f32 ret;
    s8 idx;

    player->flag = PLAYER_INACTIVE;
    player->force = 0;
    player->kart = characterId;
    player->talk = 0;
    player->mass = gKartFrictionTable[player->kart];
    player->radius = g_charRadiusTbl[player->kart];
    player->gravity = gravity_1[player->kart];

    switch (g_gameMode) {
        case GRAND_PRIX:
        case VERSUS:
            player->f_grip = D_800E2400[g_raceClass][player->kart];
            player->r_grip = D_800E24B4[g_raceClass][player->kart];
            player->max_power = D_800E2568[g_raceClass][player->kart];
            player->acc_maxcount = gTopSpeedTable[g_raceClass][player->kart];
            break;

        // Uses 100CC values
        case TIME_TRIALS:
            player->f_grip = D_800E2400[CC_100][player->kart];
            player->r_grip = D_800E24B4[CC_100][player->kart];
            player->max_power = D_800E2568[CC_100][player->kart];
            player->acc_maxcount = gTopSpeedTable[CC_100][player->kart];
            break;

        case BATTLE:
            player->f_grip = D_800E2400[CC_BATTLE][player->kart];
            player->r_grip = D_800E24B4[CC_BATTLE][player->kart];
            player->max_power = D_800E2568[CC_BATTLE][player->kart];
            player->acc_maxcount = gTopSpeedTable[CC_BATTLE][player->kart];
            break;
    }

    player->position[0] = startingRow;
    ret = CheckHight(startingRow, arg4 + 50.0f, startingColumn) + player->radius;
    player->position[2] = startingColumn;
    player->position[1] = ret;
    player->old_position[0] = startingRow;
    player->old_position[1] = ret;

    enemy_tenuki_y[playerIndex] = ret;

    player->direction[0] = 0;
    player->old_position[2] = startingColumn;
    player->up_vector[1] = 1.0f;
    player->up_vector[0] = 0.0f;
    player->up_vector[2] = 0.0f;
    player->velocity[0] = 0.0f;
    player->velocity[1] = 0.0f;
    player->velocity[2] = 0.0f;
    player->direction[1] = arg5;
    player->direction[2] = 0;
    player->sterracc = 0;
    player->status = 0;

    player->slip_flag = 0;
    player->slipang = 0;
    player->nowsterr = 0;
    player->shake_flag = 0;
    player->point = 0;
    player->rap = -1;
    player->force = 0.0f;
    player->grip = 0.0f;
    player->speed = 0.0f;
    player->ground = 0.0f;
    player->flag = playerType;
    player->jugemu_flag = 0;
    player->water_flag = 0;
    player->hitcount = 0;
    player->slipstream_timer = 0;
    player->maxadd_power = 0.0f;
    player->power_down = 0.0f;
    player->power_cont = 0.0f;
    player->accelcount = 0.0f;
    player->brake_time = 0.0f;
    player->sus.jump_firstspeed = 0.0f;
    player->handling_flag = 0;
    player->hit_flag = 0;
    player->weapon = 0;
    player->erase = ALPHA_MAX;

    player->kartroll = 0;
    player->kartpitch = 0;
    player->sus.flag = 0;
    player->sus.flag1 = 0;
    player->sus.broken = 0;
    player->sus.jump_timer = 0;
    player->sus.gachon_timer = 0;
    player->sus.broken_timer = 0;
    player->sus.bound_timer = 0;
    player->sus.swing_timer = 0;
    player->sus.swing_flag = 0;
    player->sus.bound = 0;
    player->sus.swing = 0;

    player->kage_ang = 0;
    player->sterrangle = 0;
    player->slipcount = 0;
    player->sterr_count = 0;
    player->offsetsterr_flag = 0;
    player->old_direction = 0;
    player->thunder_timer = 0;
    player->spin_timer = 0;
    player->offsetsterr_timer = 0;
    player->slipang = 0;
    player->jumpcount = 0;
    player->jugemu_timer = 0;
    player->jugemu_flag = 0;
    player->turbo_timer = 0;
    player->water_flag = 0;
    player->bomb_timer = 0;
    player->slipstream_timer = 0;
    player->hitcount = 0;
    player->chasesp_sterr = 0;
    player->drift_count = 0;
    player->chasepoint = 0;
    player->stopsetrr_count = 0;
    player->powerstack_count = 0;
    player->hit_sterr = 0;
    player->roliover_time = 0;
    player->broken_timer = 0;
    player->drift_turbo_timer = 0;
    player->tcol_count = 0;
    player->wallhitcount = 0;

    player->offsetsize = 1.0f;
    player->sus.jump_firstspeed = 1.0f;

    player->gravity_xz[0] = 0.0f;
    player->gravity_xz[1] = 0.0f;
    player->gravity_xz[2] = 0.0f;
    player->turbo_power = 0.0f;
    player->sus.position = 0.0f;
    player->sus.accele = 0.0f;
    player->sus.jump_l = 0.0f;
    player->sus.jmp_acc_l = 0.0f;
    player->sus.swing_firstspeed = 0.0f;
    player->sus.gachon_firstspeed = 0.0f;
    player->sus.bound_firstspeed = 0.0f;
    player->sus.broken_firstspeed = 0.0f;
    player->sus.y_grv = 0.0f;
    player->f_grip = 0.0f;
    player->r_grip = 0.0f;
    player->force = 0.0f;
    player->grip = 0.0f;
    player->speed = 0.0f;
    player->taco = 0.0f;
    player->accelcount = 0.0f;
    player->power_down = 0.0f;
    player->power = 0.0f;
    player->offsetsterr_speed = 0.0f;
    player->slipstream_power = 0.0f;
    player->maxadd_power = 0.0f;
    player->jump = 0.0f;
    player->jmp_acc = 0.0f;
    player->jmp_speed = 0.0f;
    player->power_cont = 0.0f;
    player->shake = 0.0f;
    player->mid_front = 0.0f;
    player->mid_rear = 0.0f;
    player->allrear_grip = 0.0f;
    player->brake_time = 0.0f;
    player->max_power = 0.0f;
    player->spark_x = 0.0f;
    player->spark_z = 0.0f;
    player->old_speed = 0.0f;
    player->mid_left = 0.0f;
    player->mid_right = 0.0f;

    idx = playerIndex;

    old_dir[0][idx] = 0;
    old_dir[1][idx] = 0;
    old_dir[2][idx] = 0;
    old_dir[3][idx] = 0;
    old_deg[0][idx] = 0;
    old_deg[1][idx] = 0;
    old_deg[2][idx] = 0;
    old_deg[3][idx] = 0;
    compel_flag[0][idx] = 0;
    compel_flag[1][idx] = 0;
    compel_flag[2][idx] = 0;
    compel_flag[3][idx] = 0;
    draw_number[0][idx] = 0;
    draw_number[1][idx] = 0;
    draw_number[2][idx] = 0;
    draw_number[3][idx] = 0;

    accele_time[idx] = 0;
    accele_flag[idx] = 0;
    power_band_accele[idx] = false;
    power_band_timer[idx] = 0;
    brake_timer[idx] = 0;
    brake_flag[idx] = 0;
    full_brake_accele[idx] = false;
    full_brake_timer[playerIndex] = 0;
    D_8018D900[0] = 0;

    green_timer[playerIndex] = 0;
    sterr_counter[playerIndex] = 0;
    D_80165020[playerIndex] = 0;
    old_velocity[playerIndex][0] = 0.0f;
    old_velocity[playerIndex][1] = 0.0f;
    old_velocity[playerIndex][2] = 0.0f;
    accele_count[playerIndex] = 0.0f;
    g_waterlevelPlayer[playerIndex] = 0.0f;
    accele_onoff_flag[playerIndex] = 0;
    accele_inc_flag[playerIndex] = 0;
    brake_onoff_flag[playerIndex] = 0;
    brake_inc_flag[playerIndex] = 0;
    tire_hight = 0;

    player->tire_FL.Status = 0;
    player->tire_FR.Status = 0;
    player->tire_RL.Status = 0;
    player->tire_RR.Status = 0;

    player->tire_FL.LastAxis = 0;
    player->tire_FR.LastAxis = 0;
    player->tire_RL.LastAxis = 0;
    player->tire_RR.LastAxis = 0;

    player->tire_FL.LastPointer = 0;
    player->tire_FR.LastPointer = 0;
    player->tire_RL.LastPointer = 0;
    player->tire_RR.LastPointer = 0;

    player->tire_FR.Dummy = 0;
    player->tire_FL.Dummy = 0;
    player->tire_RL.Dummy = 0;
    player->tire_RR.Dummy = 0;

    player->bump.flag_xy = 0;
    player->bump.flag_yz = 0;
    player->bump.flag_zx = 0;
    player->bump.last_xy = 0;
    player->bump.last_yz = 0;
    player->bump.last_zx = 0;

    player->tire_FL.Position[0] = 0.0f;
    player->tire_FL.Position[1] = 0.0f;
    player->tire_FL.Position[2] = 0.0f;

    player->tire_FR.Position[0] = 0.0f;
    player->tire_FR.Position[1] = 0.0f;
    player->tire_FR.Position[2] = 0.0f;

    player->tire_RL.Position[0] = 0.0f;
    player->tire_RL.Position[1] = 0.0f;
    player->tire_RL.Position[2] = 0.0f;

    player->tire_RR.Position[0] = 0.0f;
    player->tire_RR.Position[1] = 0.0f;
    player->tire_RR.Position[2] = 0.0f;

    player->tire_FL.Height = 0.0f;
    player->tire_FR.Height = 0.0f;
    player->tire_RL.Height = 0.0f;
    player->tire_RR.Height = 0.0f;

    player->bump.distance_xy = 0.0f;
    player->bump.distance_yz = 0.0f;
    player->bump.distance_zx = 0.0f;
    player->bump.bump_xy[0] = 0.0f;
    player->bump.bump_xy[1] = 0.0f;
    player->bump.bump_xy[2] = 0.0f;
    player->bump.bump_yz[0] = 0.0f;
    player->bump.bump_yz[1] = 0.0f;
    player->bump.bump_yz[2] = 0.0f;
    player->bump.bump_zx[0] = 0.0f;
    player->bump.bump_zx[1] = 0.0f;
    player->bump.bump_zx[2] = 0.0f;

    echo_flag[playerIndex] = 0;
    D_8018CE10[playerIndex].unk_04[0] = 0.0f;
    D_8018CE10[playerIndex].unk_04[2] = 0.0f;
    initial_bump_kart(playerIndex);
    effectinit(player);
    allkill_balloon(player, playerIndex);
    if (g_gameMode == BATTLE) {
        init_all_player_balloons(player, playerIndex);
    }
    MakeAlignMatrix(player->align2, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                 player->direction[1]);
    MakeAlignMatrix(player->align, player->up_vector[0], player->up_vector[1], player->up_vector[2],
                                 player->direction[1]);
}

void set_up_view_distance(void) {
    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            if (gGamestate == ENDING) {
                select_viewKart = 0x898;
                select_rearviewKart = 0;
                select_bumpKart = 0x384;
                select_rearbumpKart = 0;
                select_NondmaKart = 0x384;
                select_rearNondmaKart = 0;
                select_dmaKart = 0x35C;
                select_reardmaKart = 0;
                select_viewObj = 0x1F4;
                select_rearviewObj = 0;
            } else {
                select_viewKart = 0x4B0;
                select_rearviewKart = -0xA;
                select_bumpKart = 0x384;
                select_rearbumpKart = 0x32;
                select_NondmaKart = 0x1F4;
                select_rearNondmaKart = 0;
                select_dmaKart = 0x15E;
                select_reardmaKart = 0;
                select_viewObj = 0xFA;
                select_rearviewObj = 0;
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (g_gameMode == BATTLE) {
                select_viewKart = 0x898;
                select_rearviewKart = 0;
                select_bumpKart = 0x320;
                select_rearbumpKart = 0;
                select_NondmaKart = 0x190;
                select_rearNondmaKart = 0;
                select_dmaKart = 0xC8;
                select_reardmaKart = 0;
                select_viewObj = 0xC8;
                select_rearviewObj = 0;
            } else {
                select_viewKart = 0x4B0;
                select_rearviewKart = 0x32;
                select_bumpKart = 0x320;
                select_rearbumpKart = 0x32;
                select_NondmaKart = 0x190;
                select_rearNondmaKart = 0;
                select_dmaKart = 0x96;
                select_reardmaKart = 0;
                select_viewObj = 0x96;
                select_rearviewObj = 0;
            }
            break;

        default:
            if (g_gameMode == BATTLE) {
                select_viewKart = 0x898;
                select_rearviewKart = 0;
                select_bumpKart = 0x320;
                select_rearbumpKart = 0;
                select_NondmaKart = 0x190;
                select_rearNondmaKart = 0;
                select_dmaKart = 0xC8;
                select_reardmaKart = 0;
                select_viewObj = 0xC8;
                select_rearviewObj = 0;
            } else {
                select_viewKart = 0x3E8;
                select_rearviewKart = 0;
                select_bumpKart = 0x258;
                select_rearbumpKart = 0;
                select_NondmaKart = 0x15E;
                select_rearNondmaKart = 0;
                select_dmaKart = 0x96;
                select_reardmaKart = 0;
                select_viewObj = 0x96;
                select_rearviewObj = 0;
            }
            break;
    }
}

// typedef struct {
//     s32 unk00[8];
// } temp_80039DA4; // to be removed when data is compilable
// extern temp_80039DA4 D_800E4360;
// extern temp_80039DA4 D_800E4380;

void make_rank(void) {
    s32 i;

    s32 sp2C[] = {
        7, 6, 5, 4, 3, 2, 1, 0,
    };

    s32 spC[] = {
        0, 1, 2, 3, 4, 5, 6, 7,
    };

    if (((g_courseSelect == COURSE_ONE) && (D_8016556E == 0)) || (g_DemoFlag == 1) ||
        (title_mode == DEBUG_MENU_OPTION_SELECTED)) {
        for (i = 0; i < NUM_PLAYERS; i++) {
            kartrank[i] = sp2C[i];
        }
    } else {
        for (i = 0; i < NUM_PLAYERS; i++) {
            kartrank[i] = spC[gGPCurrentRaceRankByPlayerId[i]];
        }
    }
}

UNUSED f32 D_800E43A0 = 1.0f;
UNUSED s16 D_800E43A4 = 1;
UNUSED s16 D_800E43A8 = 0;

void mario_gp1P(f32* arg0, f32* arg1, f32 arg2) {
    make_rank();
    if (((g_courseSelect == COURSE_ONE) && (D_8016556E == 0)) || (g_DemoFlag == 1) ||
        (title_mode == DEBUG_MENU_OPTION_SELECTED)) {
        s16 rand;
        s16 i;

        do {
            rand = MakeRandomLimmit(7);
        } while (rand == g_CharacterSelections[0]);

        // Randomize gPlayerTwo
        cpu_chooseCharacters[0] = rand;

        // Chooses arr[0] as a fallback to prevent duplicating characters.
        // If it doesn't find the if, it will grab the final index as a fallback.
        for (i = 1; i < 7; i++) {
            u16* arr = (u16*) cpu_forPlayer[g_CharacterSelections[0]];
            if (rand == arr[i]) {
                cpu_chooseCharacters[i] = arr[0];
            } else {
                cpu_chooseCharacters[i] = arr[i];
            }
        }
    }

    D_8016556E = 0;
    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerOneCopy, 0, arg0[kartrank[0]], arg1[kartrank[0]], arg2, 32768.0f,
                     g_CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        initializePlayer(gPlayerTwo, 1, arg0[kartrank[1]], arg1[kartrank[1]], arg2, 32768.0f, cpu_chooseCharacters[0],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        initializePlayer(gPlayerThree, 2, arg0[kartrank[2]], arg1[kartrank[2]], arg2, 32768.0f, cpu_chooseCharacters[1],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        initializePlayer(gPlayerFour, 3, arg0[kartrank[3]], arg1[kartrank[3]], arg2, 32768.0f, cpu_chooseCharacters[2],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        initializePlayer(gPlayerFive, 4, arg0[kartrank[4]], arg1[kartrank[4]], arg2, 32768.0f, cpu_chooseCharacters[3],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        initializePlayer(gPlayerSix, 5, arg0[kartrank[5]], arg1[kartrank[5]], arg2, 32768.0f, cpu_chooseCharacters[4],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        initializePlayer(gPlayerSeven, 6, arg0[kartrank[6]], arg1[kartrank[6]], arg2, 32768.0f, cpu_chooseCharacters[5],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        initializePlayer(gPlayerEight, 7, arg0[kartrank[7]], arg1[kartrank[7]], arg2, 32768.0f, cpu_chooseCharacters[6],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
        opening_flag = 0;
    } else {
        initializePlayer(gPlayerOneCopy, 0, arg0[kartrank[0]], arg1[kartrank[0]] + 250.0f, arg2, 32768.0f,
                     g_CharacterSelections[0], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerTwo, 1, arg0[kartrank[1]], arg1[kartrank[1]] + 250.0f, arg2, 32768.0f,
                     cpu_chooseCharacters[0], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        initializePlayer(gPlayerThree, 2, arg0[kartrank[3]], arg1[kartrank[2]] + 250.0f, arg2, 32768.0f,
                     cpu_chooseCharacters[1], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        initializePlayer(gPlayerFour, 3, arg0[kartrank[2]], arg1[kartrank[3]] + 250.0f, arg2, 32768.0f,
                     cpu_chooseCharacters[2], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        initializePlayer(gPlayerFive, 4, arg0[kartrank[5]], arg1[kartrank[4]] + 250.0f, arg2, 32768.0f,
                     cpu_chooseCharacters[3], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        initializePlayer(gPlayerSix, 5, arg0[kartrank[4]], arg1[kartrank[5]] + 250.0f, arg2, 32768.0f,
                     cpu_chooseCharacters[4], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        initializePlayer(gPlayerSeven, 6, arg0[kartrank[7]], arg1[kartrank[6]] + 250.0f, arg2, 32768.0f,
                     cpu_chooseCharacters[5], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        initializePlayer(gPlayerEight, 7, arg0[kartrank[6]], arg1[kartrank[7]] + 250.0f, arg2, 32768.0f,
                     cpu_chooseCharacters[6], EXISTS | IS_GP_OPENING | NO_CONTROLS | IS_CPU_PLAYER);
        opening_flag = 1;
    }
    set_up_view_distance();
}

void time_attack(f32* arg0, f32* arg1, f32 arg2) {
    initializePlayer(gPlayerFour, 3, arg0[2], arg1[2], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerFive, 4, arg0[3], arg1[3], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSix, 5, arg0[4], arg1[4], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSeven, 6, arg0[5], arg1[5], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerEight, 7, arg0[6], arg1[6], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     PLAYER_HUMAN_AND_CPU);
        initializePlayer(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     NO_CONTROLS | IS_CPU_PLAYER);
        initializePlayer(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[0],
                     NO_CONTROLS | IS_CPU_PLAYER);
    } else if (replay_flag != 1) {
        initializePlayer(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        if (ghost_status == 0) {
            initializePlayer(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, ghost_kart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            initializePlayer(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
        if (romghost_status == 0) {
            initializePlayer(gPlayerThree, 2, arg0[0], arg1[0], arg2, 32768.0f, romghost_kart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            initializePlayer(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
    } else {
        initializePlayer(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, replayghost_kart,
                     EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        if (replayghost_status == 0) {
            initializePlayer(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, ghost_kart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            initializePlayer(gPlayerTwo, 1, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
        if (romghost_status == 0) {
            initializePlayer(gPlayerThree, 2, arg0[0], arg1[0], arg2, 32768.0f, romghost_kart,
                         EXISTS | IS_PLAYER | NO_CONTROLS | IS_GHOST);
        } else {
            initializePlayer(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[0],
                         NO_CONTROLS | IS_CPU_PLAYER);
        }
    }
    opening_flag = 0;
    set_up_view_distance();
}

void mario_gp2P(f32* arg0, f32* arg1, f32 arg2) {
    make_rank();
    if ((g_courseSelect == COURSE_ONE) || (g_DemoFlag == 1) || (title_mode == DEBUG_MENU_OPTION_SELECTED)) {
        s16 rand;
        s16 i;

        // @todo: this is a do-while loop
    getRand:
        rand = MakeRandomLimmit(7);
        if (g_CharacterSelections[0] == rand) {
            goto getRand;
        }
        if (g_CharacterSelections[1] == rand) {
            goto getRand;
        }

        cpu_chooseCharacters[0] = rand;

        for (i = 1; i < 6; i++) {
            u16* arr = (u16*) cpu_forTwoPlayer[g_CharacterSelections[0]][g_CharacterSelections[1]];
            if (rand == arr[i]) {
                cpu_chooseCharacters[i] = arr[0];
            } else {
                cpu_chooseCharacters[i] = arr[i];
            }
        }
    }

    initializePlayer(gPlayerThree, 2, arg0[kartrank[2]], arg1[kartrank[2]], arg2, 32768.0f, cpu_chooseCharacters[0],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    initializePlayer(gPlayerFour, 3, arg0[kartrank[3]], arg1[kartrank[3]], arg2, 32768.0f, cpu_chooseCharacters[1],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    initializePlayer(gPlayerFive, 4, arg0[kartrank[4]], arg1[kartrank[4]], arg2, 32768.0f, cpu_chooseCharacters[2],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    initializePlayer(gPlayerSix, 5, arg0[kartrank[5]], arg1[kartrank[5]], arg2, 32768.0f, cpu_chooseCharacters[3],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    initializePlayer(gPlayerSeven, 6, arg0[kartrank[6]], arg1[kartrank[6]], arg2, 32768.0f, cpu_chooseCharacters[4],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    initializePlayer(gPlayerEight, 7, arg0[kartrank[7]], arg1[kartrank[7]], arg2, 32768.0f, cpu_chooseCharacters[5],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);

    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerOneCopy, 0, arg0[kartrank[0]], arg1[kartrank[0]], arg2, 32768.0f,
                     g_CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
    } else {
        initializePlayer(gPlayerOneCopy, 0, arg0[kartrank[0]], arg1[kartrank[0]], arg2, 32768.0f,
                     g_CharacterSelections[0], EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerTwo, 1, arg0[kartrank[1]], arg1[kartrank[1]], arg2, 32768.0f, g_CharacterSelections[1],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    } else {
        initializePlayer(gPlayerTwo, 1, arg0[kartrank[1]], arg1[kartrank[1]], arg2, 32768.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }

    opening_flag = 0;
    set_up_view_distance();
}

void vs_race2P(f32* arg0, f32* arg1, f32 arg2) {
    initializePlayer(gPlayerThree, 2, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerFour, 3, arg0[2], arg1[2], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerFive, 4, arg0[3], arg1[3], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSix, 5, arg0[4], arg1[4], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSeven, 6, arg0[5], arg1[5], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerEight, 7, arg0[6], arg1[6], arg2, 32768.0f, g_CharacterSelections[0],
                 NO_CONTROLS | IS_CPU_PLAYER);
    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     PLAYER_HUMAN_AND_CPU);
    } else {
        initializePlayer(gPlayerOneCopy, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[1], PLAYER_HUMAN_AND_CPU);
    } else {
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    opening_flag = 0;
    set_up_view_distance();
}

void battle2P(f32* arg0, f32* arg1, f32 arg2) {
    if (g_courseID == COURSE_BIG_DONUT) {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, -16384.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 16384.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    } else {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 0.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, g_CharacterSelections[2],
                 NO_CONTROLS | IS_PLAYER);
    initializePlayer(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, g_CharacterSelections[3],
                 NO_CONTROLS | IS_PLAYER);
    initializePlayer(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    opening_flag = 0;
    set_up_view_distance();
}

void vs_race3P(f32* arg0, f32* arg1, f32 arg2) {
    initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[1],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, g_CharacterSelections[2],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[1], PLAYER_HUMAN_AND_CPU);
        initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, g_CharacterSelections[2], PLAYER_HUMAN_AND_CPU);
    }

    initializePlayer(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, 3, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    opening_flag = 0;
    set_up_view_distance();
}

void battle3P(f32* arg0, f32* arg1, f32 arg2) {
    if (g_courseID == COURSE_BIG_DONUT) {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, -16384.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 16384.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, 0.0f, g_CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    } else {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 0.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, -16384.0f, g_CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    initializePlayer(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, 3, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    opening_flag = 0;
    set_up_view_distance();
}

void vs_race4P(f32* arg0, f32* arg1, f32 arg2) {
    initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[1],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, g_CharacterSelections[2],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    initializePlayer(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, g_CharacterSelections[3],
                 EXISTS | NO_CONTROLS | IS_PLAYER);
    if (g_DemoFlag == 1) {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0], PLAYER_HUMAN_AND_CPU);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 32768.0f, g_CharacterSelections[1], PLAYER_HUMAN_AND_CPU);
        initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, 32768.0f, g_CharacterSelections[2], PLAYER_HUMAN_AND_CPU);
        initializePlayer(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, g_CharacterSelections[3], PLAYER_HUMAN_AND_CPU);
    }
    initializePlayer(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    opening_flag = 0;
    set_up_view_distance();
}

void battle4P(f32* arg0, f32* arg1, f32 arg2) {
    if (g_courseID == COURSE_BIG_DONUT) {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, -16384.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 16384.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, 0.0f, g_CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerFour, 3, arg0[3], arg1[3], arg2, 32768.0f, g_CharacterSelections[3],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    } else {
        initializePlayer(gPlayerOne, 0, arg0[0], arg1[0], arg2, 32768.0f, g_CharacterSelections[0],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerTwo, 1, arg0[1], arg1[1], arg2, 0.0f, g_CharacterSelections[1],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerThree, 2, arg0[2], arg1[2], arg2, -16384.0f, g_CharacterSelections[2],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
        initializePlayer(gPlayerFour, 3, arg0[3], arg1[3], arg2, 16384.0f, g_CharacterSelections[3],
                     EXISTS | NO_CONTROLS | IS_PLAYER);
    }
    initializePlayer(gPlayerFive, 4, arg0[4], arg1[4], arg2, 32768.0f, 4, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSix, 5, arg0[5], arg1[5], arg2, 32768.0f, 5, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerSeven, 6, arg0[6], arg1[6], arg2, 32768.0f, 6, NO_CONTROLS | IS_CPU_PLAYER);
    initializePlayer(gPlayerEight, 7, arg0[0], arg1[0], arg2, 32768.0f, 7, NO_CONTROLS | IS_CPU_PLAYER);
    opening_flag = 0;
    set_up_view_distance();
}

void result_kart_pos(void) {
    initializePlayer(gPlayerOne, 0, -2770.774f, -345.187f, -34.6f, 0.0f, d_points_rank[0],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    initializePlayer(gPlayerTwo, 1, -3691.506f, -6.822f, -6.95f, (f32) (200 * DEGREES(1)), d_points_rank[1],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    initializePlayer(gPlayerThree, 2, -3475.028f, -998.485f, -8.059f, (f32) (250 * DEGREES(1)), d_points_rank[2],
                 EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    if (resultstat.rank >= 3) {
        initializePlayer(gPlayerFour, 3, -3025.772f, 110.039f, -23.224f, (f32) (155 * DEGREES(1)), resultstat.kart,
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    } else {
        initializePlayer(gPlayerFour, 3, -3025.772f, 110.039f, -23.224f, (f32) (155 * DEGREES(1)), d_points_rank[3],
                     EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
    }
    initializePlayer(gPlayerFive, 4, -2770.774f, -345.187f, -34.6f, 0.0f, 0, 0x7000);
    initializePlayer(gPlayerSix, 5, -3691.506f, -6.822f, -6.95f, (f32) (200 * DEGREES(1)), 0, 0x7000);
    initializePlayer(gPlayerSeven, 6, -3475.028f, -998.485f, -8.059f, (f32) (250 * DEGREES(1)), 0, 0x7000);
    initializePlayer(gPlayerEight, 7, -3025.772f, 110.039f, -23.224f, (f32) (155 * DEGREES(1)), 0, 0x7000);
    opening_flag = 0;
    set_up_view_distance();
}

void reset_pos(void) {
    s16 sp5E;
    s16 sp5C;
    s16 sp5A;
    s32 temp;
    UNUSED s32 pad[4];
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
        case COURSE_CHOCO_MOUNTAIN:
        case COURSE_BOWSER_CASTLE:
        case COURSE_BANSHEE_BOARDWALK:
        case COURSE_YOSHI_VALLEY:
        case COURSE_FRAPPE_SNOWLAND:
        case COURSE_KOOPA_BEACH:
        case COURSE_ROYAL_RACEWAY:
        case COURSE_LUIGI_RACEWAY:
        case COURSE_MOO_MOO_FARM:
        case COURSE_TOADS_TURNPIKE:
        case COURSE_KALAMARI_DESERT:
        case COURSE_SHERBET_LAND:
        case COURSE_RAINBOW_ROAD:
        case COURSE_WARIO_STADIUM:
        case COURSE_DK_JUNGLE:
            InitCenterLine();
            sp5E = (f32) CenterPathBP[0][0].pointx;
            sp5C = (f32) CenterPathBP[0][0].pointz;
            sp5A = (f32) CenterPathBP[0][0].pointy;
            if (g_courseID == COURSE_TOADS_TURNPIKE) {
                sp5E = 0;
            }
            break;

        case COURSE_BLOCK_FORT:
        case COURSE_SKYSCRAPER:
        case COURSE_DOUBLE_DECK:
        case COURSE_BIG_DONUT:
            kame_hibasira_init();
            break;
    }

    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
        case COURSE_CHOCO_MOUNTAIN:
        case COURSE_BOWSER_CASTLE:
        case COURSE_BANSHEE_BOARDWALK:
        case COURSE_YOSHI_VALLEY:
        case COURSE_FRAPPE_SNOWLAND:
        case COURSE_KOOPA_BEACH:
        case COURSE_ROYAL_RACEWAY:
        case COURSE_LUIGI_RACEWAY:
        case COURSE_MOO_MOO_FARM:
        case COURSE_TOADS_TURNPIKE:
        case COURSE_KALAMARI_DESERT:
        case COURSE_SHERBET_LAND:
        case COURSE_RAINBOW_ROAD:
        case COURSE_WARIO_STADIUM:
        case COURSE_DK_JUNGLE:
            switch (g_ScreenSplitA) {
                case SCREEN_MODE_1P:
                    switch (g_gameMode) {
                        case GRAND_PRIX:
                            grid_x[0] = (grid_x[2] = (grid_x[4] = (grid_x[6] = sp5E + 0x14)));
                            grid_x[1] = (grid_x[3] = (grid_x[5] = (grid_x[7] = sp5E - 0x14)));
                            grid_z[0] = sp5C + 0x1E;
                            grid_z[1] = sp5C + 0x32;
                            grid_z[2] = sp5C + 0x46;
                            grid_z[3] = sp5C + 0x5A;
                            grid_z[4] = sp5C + 0x6E;
                            grid_z[5] = sp5C + 0x82;
                            grid_z[6] = sp5C + 0x96;
                            grid_z[7] = sp5C + 0xAA;
                            mario_gp1P(grid_x, grid_z, sp5A);
                            break;

                        case TIME_TRIALS:
                            grid_x[0] = (grid_x[2] = (grid_x[4] = (grid_x[6] = sp5E)));
                            grid_x[1] = (grid_x[3] = (grid_x[5] = (grid_x[7] = sp5E)));
                            grid_z[0] = sp5C + 0x1E;
                            grid_z[1] = sp5C + 0x1E;
                            grid_z[2] = sp5C + 0x1E;
                            grid_z[3] = sp5C + 0x1E;
                            grid_z[4] = sp5C + 0x1E;
                            grid_z[5] = sp5C + 0x1E;
                            grid_z[6] = sp5C + 0x1E;
                            grid_z[7] = sp5C + 0x1E;
                            time_attack(grid_x, grid_z, sp5A);
                            break;
                    }
                    break;

                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    switch (g_gameMode) {
                        case GRAND_PRIX:
                            grid_x[0] = (grid_x[2] = (grid_x[4] = (grid_x[6] = sp5E + 0x14)));
                            grid_x[1] = (grid_x[3] = (grid_x[5] = (grid_x[7] = sp5E - 0x14)));
                            grid_z[0] = sp5C + 0x1E;
                            grid_z[1] = sp5C + 0x32;
                            grid_z[2] = sp5C + 0x46;
                            grid_z[3] = sp5C + 0x5A;
                            grid_z[4] = sp5C + 0x6E;
                            grid_z[5] = sp5C + 0x82;
                            grid_z[6] = sp5C + 0x96;
                            grid_z[7] = sp5C + 0xAA;
                            mario_gp2P(grid_x, grid_z, sp5A);
                            break;

                        case VERSUS:
                            grid_x[0] = (grid_x[2] = (grid_x[4] = (grid_x[6] = sp5E + 0xA)));
                            grid_x[1] = (grid_x[3] = (grid_x[5] = (grid_x[7] = sp5E - 0xA)));
                            grid_z[0] = sp5C + 0x1E;
                            grid_z[1] = sp5C + 0x1E;
                            grid_z[2] = sp5C + 0x1E;
                            grid_z[3] = sp5C + 0x1E;
                            grid_z[4] = sp5C + 0x1E;
                            grid_z[5] = sp5C + 0x1E;
                            grid_z[6] = sp5C + 0x1E;
                            grid_z[7] = sp5C + 0x1E;
                            vs_race2P(grid_x, grid_z, sp5A);
                            break;
                    }
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    switch (g_gameMode) {
                        case VERSUS:
                            grid_x[0] = sp5E + 0x1E;
                            grid_x[6] = sp5E - 0xA;
                            grid_x[1] = sp5E + 0xA;
                            grid_x[7] = sp5E - 0x1E;
                            grid_x[4] = sp5E - 0xA;
                            grid_x[2] = sp5E - 0xA;
                            grid_x[5] = sp5E - 0x1E;
                            grid_x[3] = sp5E - 0x1E;
                            grid_z[0] = sp5C + 0x1E;
                            grid_z[1] = sp5C + 0x1E;
                            grid_z[2] = sp5C + 0x1E;
                            grid_z[3] = sp5C + 0x1E;
                            grid_z[4] = sp5C + 0x1E;
                            grid_z[5] = sp5C + 0x1E;
                            grid_z[6] = sp5C + 0x1E;
                            grid_z[7] = sp5C + 0x1E;
                            if (g_playerCount == 4) {
                                vs_race4P(grid_x, grid_z, sp5A);
                            } else {
                                vs_race3P(grid_x, grid_z, sp5A);
                            }
                            break;
                    }
                    break;
            }
            break;

        case COURSE_BLOCK_FORT:
            switch (g_ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 5;
                    if (1) {};
                    grid_x[0] = 0;
                    grid_x[1] = 0;
                    grid_z[1] = -200.0f;
                    grid_z[0] = 200.0f;
                    battle2P(grid_x, grid_z, temp);
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    temp = 5;
                    grid_x[2] = -200.0f;
                    grid_z[1] = -200.0f;
                    grid_x[0] = 0.0f;
                    grid_x[1] = 0.0f;
                    grid_z[2] = 0.0f;
                    grid_z[3] = 0.0f;
                    grid_x[3] = 200.0f;
                    grid_z[0] = 200.0f;
                    if (g_playerCount == 4) {
                        battle4P(grid_x, grid_z, temp);
                    } else {
                        battle3P(grid_x, grid_z, temp);
                    }
                    break;
            }
            break;

        case COURSE_SKYSCRAPER:
            switch (g_ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 0x1E0;
                    if (1) {};
                    grid_x[0] = 0.0f;
                    grid_x[1] = 0.0f;
                    grid_z[1] = -400.0f;
                    grid_z[0] = 400.0f;
                    battle2P(grid_x, grid_z, temp);
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    temp = 0x1E0;
                    grid_x[0] = 0.0f;
                    grid_x[1] = 0.0f;
                    grid_x[2] = -400.0f;
                    grid_x[3] = 400.0f;
                    grid_z[0] = 400.0f;
                    grid_z[1] = -400.0f;
                    grid_z[2] = 0.0f;
                    grid_z[3] = 0.0f;
                    if (g_playerCount == 4) {
                        battle4P(grid_x, grid_z, temp);
                    } else {
                        battle3P(grid_x, grid_z, temp);
                    }
                    break;
            }
            break;

        case COURSE_DOUBLE_DECK:
            switch (g_ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 0x37;
                    if (1) {};
                    grid_x[0] = 0.0f;
                    grid_x[1] = 0.0f;
                    grid_z[1] = -160.0f;
                    grid_z[0] = 160.0f;
                    battle2P(grid_x, grid_z, temp);
                    break;

                case 3:
                    temp = 0x37;
                    grid_x[0] = 0.0f;
                    grid_x[1] = 0.0f;
                    grid_x[2] = -160.0f;
                    grid_x[3] = 160.0f;
                    grid_z[0] = 160.0f;
                    grid_z[1] = -160.0f;
                    grid_z[2] = 0.0f;
                    grid_z[3] = 0.0f;
                    if (g_playerCount == 4) {
                        battle4P(grid_x, grid_z, temp);
                    } else {
                        battle3P(grid_x, grid_z, temp);
                    }
                    break;
            }
            break;

        case COURSE_BIG_DONUT:
            switch (g_ScreenSplitA) {
                case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
                case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                    temp = 0xC8;
                    if (1) {};
                    grid_x[0] = 0.0f;
                    grid_x[1] = 0.0f;
                    grid_z[1] = -575.0f;
                    grid_z[0] = 575.0f;
                    battle2P(grid_x, grid_z, temp);
                    break;

                case SCREEN_MODE_3P_4P_SPLITSCREEN:
                    temp = 0xC8;
                    grid_x[0] = 0.0f;
                    grid_x[1] = 0.0f;
                    grid_x[2] = -575.0f;
                    grid_x[3] = 575.0f;
                    grid_z[0] = 575.0f;
                    grid_z[1] = -575.0f;
                    grid_z[2] = 0.0f;
                    grid_z[3] = 0.0f;
                    if (g_playerCount == 4) {
                        battle4P(grid_x, grid_z, temp);
                    } else {
                        battle3P(grid_x, grid_z, temp);
                    }
                    break;
            }
            break;

        default:
            grid_x[0] = (grid_x[2] = (grid_x[4] = (grid_x[6] = 20.0f)));
            grid_x[1] = (grid_x[3] = (grid_x[5] = (grid_x[7] = -20.0f)));
            grid_z[0] = 30.0f;
            grid_z[1] = 50.0f;
            grid_z[2] = 70.0f;
            grid_z[3] = 90.0f;
            grid_z[4] = 110.0f;
            grid_z[5] = 130.0f;
            grid_z[6] = 150.0f;
            grid_z[7] = 170.0f;
            initializePlayer(gPlayerOneCopy, 0, grid_x[0], grid_z[0], sp5A, 32768.0f, g_CharacterSelections[0],
                         EXISTS | NO_CONTROLS | IS_PLAYER);
            initializePlayer(gPlayerTwo, 1, grid_x[1], grid_z[1], sp5A, 32768.0f, 1,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            initializePlayer(gPlayerThree, 2, grid_x[2], grid_z[2], sp5A, 32768.0f, 2,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            initializePlayer(gPlayerFour, 3, grid_x[3], grid_z[3], sp5A, 32768.0f, 3,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            initializePlayer(gPlayerFive, 4, grid_x[4], grid_z[4], sp5A, 32768.0f, 4,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            initializePlayer(gPlayerSix, 5, grid_x[5], grid_z[5], sp5A, 32768.0f, 5,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            initializePlayer(gPlayerSeven, 6, grid_x[6], grid_z[6], sp5A, 32768.0f, 6,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            initializePlayer(gPlayerEight, 7, grid_x[7], grid_z[7], sp5A, 32768.0f, 7,
                         EXISTS | IS_CPU_PLAYER | NO_CONTROLS);
            opening_flag = 0;
            break;
    }

    if (g_gameMode != BATTLE) {
        init_enemy_kart();
    }
}

void result_pos(void) {
    result_kart_pos();
}

void init_draw(Player* player, Camera* camera, s8 playerId, s8 screenId) {
    if (player->flag & EXISTS) {
        if (screenId == 0) {
            RunKart(player, camera, screenId, playerId);
        }
        kart_posset(player, camera, screenId, playerId);
        if ((screenId == 0) || (screenId == 1)) {
            Iinitcol_dma(player, playerId, screenId, 0);
            Iinitcol_dma(player, playerId, screenId, 1);
            Sikartdma(player, playerId, screenId, screenId, 0);
            decodeMIO0((u8*) &gEncodedKartTexture[0][screenId][playerId],
                       (u8*) &D_802BFB80.arraySize8[0][screenId][playerId]);
        } else {
            Iinitcol_dma(player, playerId, screenId, 0);
            Iinitcol_dma(player, playerId, screenId, 1);
            Sikartdma(player, (s8) (playerId + 4), screenId, (s8) (screenId - 2), 0);
            decodeMIO0((u8*) &gEncodedKartTexture[0][screenId - 2][playerId + 4],
                       (u8*) &D_802BFB80.arraySize8[0][screenId - 2][playerId + 4]);
        }

        old_dir[screenId][playerId] = player->dir[screenId];
        old_deg[screenId][playerId] = player->deg[screenId];
        old_slipcount[screenId][playerId] = player->slipcount;
        draw_number[screenId][playerId] = 0;
        Doobj(player, playerId, screenId);
    }
}

void initial_game(void) {
    UNUSED s32 pad;
    Player* player = &gPlayers[0];

    imageinit();
    if (gGamestate == ENDING) {
        result_pos();
    } else {
        reset_pos();
    }
    if (!g_DemoFlag) {
        switch (g_ScreenSplitA) {
            case SCREEN_MODE_1P:
                switch (g_gameMode) {
                    case GRAND_PRIX:
                        if (g_courseID == COURSE_TOADS_TURNPIKE) {
                            initial_camera(0.0f, player->position[1], grid_z[7], player->direction[1], 8, 0);
                        } else {
                            initial_camera((grid_x[7] + grid_x[6]) / 2, player->position[1], grid_z[7],
                                        player->direction[1], 8, 0);
                        }
                        break;

                    case TIME_TRIALS:
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 0);
                        break;

                    default:
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 10, 0);
                        break;
                }
                break;

            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                switch (g_gameMode) {
                    case GRAND_PRIX:
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 0);
                        player++;
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 1);
                        break;

                    case BATTLE:
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 9, 0);
                        player++;
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 9, 1);
                        break;

                    default:
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 0);
                        player++;
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 1);
                        break;
                }
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                if (g_gameMode == BATTLE) {
                    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 9, 0);
                    player++;
                    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 9, 1);
                    player++;
                    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 9, 2);
                    if (g_playerCount == 4) {
                        player++;
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 9, 3);
                    }
                } else {
                    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 0);
                    player++;
                    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 1);
                    player++;
                    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 2);
                    if (g_playerCount == 4) {
                        player++;
                        initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 1, 3);
                    }
                }
                break;
        }
    } else {
        switch (g_ScreenSplitA) {
            case SCREEN_MODE_1P:
                initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 0);
                break;

            case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
                initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 0);
                player++;
                initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 1);
                break;

            case SCREEN_MODE_3P_4P_SPLITSCREEN:
                initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 0);
                player++;
                initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 1);
                player++;
                initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 2);
                player++;
                initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 3);
                break;
        }
    }

    switch (g_ScreenSplitA) {
        case SCREEN_MODE_1P:
            init_draw(gPlayerOneCopy, camera1, 0, 0); // sic
            init_draw(gPlayerTwo, camera1, 1, 0);
            init_draw(gPlayerThree, camera1, 2, 0);
            init_draw(gPlayerFour, camera1, 3, 0);
            init_draw(gPlayerFive, camera1, 4, 0);
            init_draw(gPlayerSix, camera1, 5, 0);
            init_draw(gPlayerSeven, camera1, 6, 0);
            init_draw(gPlayerEight, camera1, 7, 0);
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            init_draw(gPlayerOneCopy, camera1, 0, 0);
            init_draw(gPlayerTwo, camera1, 1, 0);
            init_draw(gPlayerThree, camera1, 2, 0);
            init_draw(gPlayerFour, camera1, 3, 0);
            init_draw(gPlayerFive, camera1, 4, 0);
            init_draw(gPlayerSix, camera1, 5, 0);
            init_draw(gPlayerSeven, camera1, 6, 0);
            init_draw(gPlayerEight, camera1, 7, 0);
            init_draw(gPlayerOneCopy, camera2, 0, 1);
            init_draw(gPlayerTwo, camera2, 1, 1);
            init_draw(gPlayerThree, camera2, 2, 1);
            init_draw(gPlayerFour, camera2, 3, 1);
            init_draw(gPlayerFive, camera2, 4, 1);
            init_draw(gPlayerSix, camera2, 5, 1);
            init_draw(gPlayerSeven, camera2, 6, 1);
            init_draw(gPlayerEight, camera2, 7, 1);
            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            init_draw(gPlayerOneCopy, camera1, 0, 0);
            init_draw(gPlayerTwo, camera1, 1, 0);
            init_draw(gPlayerThree, camera1, 2, 0);
            init_draw(gPlayerFour, camera1, 3, 0);
            init_draw(gPlayerOneCopy, camera2, 0, 1);
            init_draw(gPlayerTwo, camera2, 1, 1);
            init_draw(gPlayerThree, camera2, 2, 1);
            init_draw(gPlayerFour, camera2, 3, 1);
            init_draw(gPlayerOneCopy, camera3, 0, 2);
            init_draw(gPlayerTwo, camera3, 1, 2);
            init_draw(gPlayerThree, camera3, 2, 2);
            init_draw(gPlayerFour, camera3, 3, 2);
            init_draw(gPlayerOneCopy, camera4, 0, 3);
            init_draw(gPlayerTwo, camera4, 1, 3);
            init_draw(gPlayerThree, camera4, 2, 3);
            init_draw(gPlayerFour, camera4, 3, 3);
            break;
    }
}

void init_result_kart(void) {
    Player* player = gPlayerOne;
    s32 playerId;

    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 0);
    initial_camera(player->position[0], player->position[1], player->position[2], player->direction[1], 3, 1);

    for (playerId = 0; playerId < NUM_PLAYERS; playerId++, player++) {
        Iinitcol_dma(player, playerId, 1, 0);
        Iinitcol_dma(player, playerId, 1, 1);
    }
}
