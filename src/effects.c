#include <ultra64.h>
#include <macros.h>
#include <decode.h>
#include <mk64.h>
#include <defines.h>
#include <sounds.h>
#include "code_800029B0.h"
#include "math_util.h"
#include "kart_attributes.h"
#include "path.h"
#include "cpu_vehicles_camera_path.h"
#include "render_player.h"
#include "player_controller.h"
#include "render_objects.h"
#include "code_80057C60.h"
#include "effects.h"
#include "audio/external.h"
#include "spawn_players.h"
#include "menu_items.h"
#include <course.h>
#include "OverKartHooks.h"

s32 D_8018D900[8];
s16 D_8018D920[8];
s32 g_StarUseCounter[8];
s32 g_GhostUseCounter[8];
s32 g_GhostUseTimer[8]; // Used for the alpha of the other screens in split-screen mode
s32 D_8018D990[8];

UNUSED void kwGetTimeInit(void) {
}

s32 kwStartTime(UNUSED s32* arg0) {
}

void kwGetTime(UNUSED s32* arg0, UNUSED s32 arg1) {
    arg1 = 4;
}

UNUSED void kwAddTime(UNUSED s32 arg0) {
}

UNUSED void kwGetLine(UNUSED s32 arg0) {
}

UNUSED void kwSetTime(void) {
}

void kwdebug(void) {
}

UNUSED void kwdebug_items(void) {
}

UNUSED void kwdebug_rcp(void) {
}

UNUSED void debugsyori_test(void) {
}

UNUSED void kwdebug_anmbreak(void) {
}

UNUSED void kwdebug_mtxkazu(void) {
}

UNUSED void kwdebug_gptrkazu(void) {
}

UNUSED void kwdebug_kumoram(void) {
}

UNUSED void kwdebug_rmonprint_1p(void) {
}
UNUSED void debugsyori_1p(void) {
}
UNUSED void debugsyori_2p_lr(void) {
}
UNUSED void debugsyori_2p_ud(void) {
}
UNUSED void debugsyori_4p_kleft(void) {
}
UNUSED void debugsyori_4p_kright(void) {
}
UNUSED void debugsyori_4p_kup(void) {
}
UNUSED void debugsyori_4p_kdown(void) {
}
UNUSED void debugsyori_4p(void) {
}
UNUSED void kwdisplay_debug(void) {
}
UNUSED void kwdebug_print_common(void) {
}
UNUSED void kwdebugprint_player_sub(void) {
}
UNUSED void kwdebugprint_ogawa(void) {
}

UNUSED void kwdebugprint_player(UNUSED s32 arg0, UNUSED s32 arg1) {
    arg1 = 4;
}

UNUSED void kwdebugprint_syori(void) {
}

UNUSED void kwdebugprint_test(void) {
}

UNUSED void kwdebugprint_1p(void) {
}

UNUSED void kwdebugprint_2p_left(void) {
}

UNUSED void kwdebugprint_2p_right(void) {
}

UNUSED void kwdebugprint_2p_up(void) {
}

UNUSED void kwdebugprint_2p_down(void) {
}

UNUSED void kwdebugprint_4p_upleft(void) {
}

UNUSED void kwdebugprint_4p_upright(void) {
}

UNUSED void kwdebugprint_4p_downleft(void) {
}

UNUSED void kwdebugprint_4p_downright(void) {
}

void check_hit_letter(Player* player) {
    // The << 9 is a hacky way to check for HIT_BOMB
    if ((player->weapon & HIT_REDSHELL) || (player->weapon & HIT_GREENSHELL) ||
        ((player->weapon << 9) < 0) || (player->weapon & HIT_BOMB_THROW)) {
        player->talk = ((u16) player->talk | FLASH);
    }
}

UNUSED void stop_strat(void) {
}

void kill_strat(Player* player, s8 playerIndex) {

    if ((player->slip_flag & ROLLOVER) == ROLLOVER) {
        reset_rollover(player, playerIndex);
    }

    if (((player->slip_flag & SPIN_L) == SPIN_L) ||
        (player->slip_flag & SPIN_R) == SPIN_R) {
        reset_spin(player, playerIndex);
    }
    if ((player->slip_flag & WHEELSPIN) == WHEELSPIN) {
        reset_wheelspin(player, playerIndex);
    }
    if ((player->handling_flag & SPINOUT_SWERVE) != 0) {
        reset_pro_wheelspin(player, playerIndex);
    }
    if ((player->slip_flag & TURBO) == TURBO) {
        reset_turbo(player);
    }
    if ((player->slip_flag & WING) == WING) {
        ResetWing(player);
    }
    if ((player->slip_flag & STORM) == STORM) {
        reset_storm(player);
    }
    if ((player->slip_flag & EXPLODE) == EXPLODE) {
        reset_bomb(player, playerIndex);
    }
    if ((player->slip_flag & THROW_EXPLODE) == THROW_EXPLODE) {
        reset_bomb_throw(player, playerIndex);
    }
    if ((player->slip_flag & DASH_JUMP) == DASH_JUMP) {
        reset_dashjump(player);
    }
    if ((player->slip_flag & DASH_MIDJUMP) == DASH_MIDJUMP) {
        reset_dash_midjump(player);
    }
    if ((player->slip_flag & RAPID_ACC) == RAPID_ACC) {
        reset_rapidacc(player);
    }
    if ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL) {
        ResetRolloverFall(player, playerIndex);
    }
    if ((player->slip_flag & HEIGHT_JUMP) == HEIGHT_JUMP) {
        reset_height_jump(player, playerIndex);
    }
    player->handling_flag = (s16) (player->handling_flag & ~REVERSE_GEAR);
    player->slip_flag = (s32) (player->slip_flag & ~SPIN_TURN);
}

void SetRollover(Player* player, s8 playerIndex) {
    UNUSED s32 sp24;
    s32 temp_v1;
    kill_strat(player, playerIndex);
    check_hit_letter(player);
    temp_v1 = player->kart;
    player->jumpcount = 0;
    player->jmp_acc = D_800E37B0[temp_v1];
    player->jmp_speed = 0.0f;

    player->jump = D_800E3790[temp_v1];
    player->slipcount = 0;
    player->slip_flag = player->slip_flag | ROLLOVER;
    player->slip_flag = player->slip_flag & ~DRIFT;
    player->slipang = 0;
    player->roliover_time = 2;
    player->kage_ang = 0;
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (temp_v1 * 0x10) + 0x29008005);
        NAPlyTrgStart(playerIndex, SOUND_ACTION_EXPLOSION);
    } else {
        oga_enemy_voice(playerIndex, player);
    }
    player->weapon = (s32) (player->weapon & ~HIT_GREENSHELL);
}

void rollover(Player* player, s8 playerIndex) {

    AccelOff(player, 5.0f);
    player->slipcount += (s16) 0xA0;
    player->kage_ang += (s16) DEGREES(10);
    if (player->slipcount >= 0x2000) {
        player->slipcount = 0;
        player->roliover_time = (s16) (player->roliover_time - 1);
        if (player->roliover_time == 0) {
            player->slipcount = 0x2000;
            reset_rollover(player, playerIndex);
            if (g_gameMode == BATTLE) {
                cut_balloon(player, playerIndex);
            }
        }
    }
}

void reset_rollover(Player* player, s8 playerIndex) {

    player->kartroll = 0;
    player->kartpitch = 0;
    player->slip_flag = (s32) (player->slip_flag & ~ROLLOVER);
    player->slipcount = 0;
    player->slipang = 0;
    player->nowsterr = 0;
    player->roliover_time = 0;
    player->sterrangle = 0;
    player->accelcount = 0.0f;
    compel_flag[0][playerIndex] = 1;
    compel_flag[1][playerIndex] = 1;
    compel_flag[2][playerIndex] = 1;
    compel_flag[3][playerIndex] = 1;
    player->kage_ang = 0;
}

void SetSpin(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);
    if (((player->slip_flag & SPIN_L) != SPIN_L) &&
        ((player->slip_flag & SPIN_R) != SPIN_R)) {
        player->slip_flag &= ~DRIFT;

        if ((player->slipang / DEGREES(1)) >= 0) {
            player->slip_flag |= SPIN_R;
        } else {
            player->slip_flag |= SPIN_L;
        }

        player->talk |= WHIRRR;
        // clang-format off
        player->slipang = 0; player->nowsterr = 0; player->sterrangle = 0; player->old_direction = player->direction[1]; player->spin_timer = 2;
        // clang-format on
        compel_flag[0][playerIndex] = 1;
        compel_flag[1][playerIndex] = 1;
        compel_flag[2][playerIndex] = 1;
        compel_flag[3][playerIndex] = 1;
        accele_count[playerIndex] = player->accelcount;
        power_band_timer[playerIndex] = 0;
        power_band_accele[playerIndex] = false;
        accele_flag[playerIndex] = 0;
        accele_time[playerIndex] = 0;
        D_8018D920[playerIndex] = 0;

        if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
            ((player->flag & IS_GHOST) != IS_GHOST)) {
            NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008003);
        } else {
            oga_enemy_voice(playerIndex, player);
        }
    }
}

void reset_spin(Player* player, s8 playerId) {
    player->slip_flag &= ~SPIN_L;
    player->slip_flag &= ~SPIN_R;
    player->slipcount = 0;
    player->direction[1] = player->old_direction;
    player->nowsterr = 0;
    player->slipang = 0;
    player->slip_flag &= ~WHEELSPIN;

    compel_flag[0][playerId] = 1;
    compel_flag[1][playerId] = 1;
    compel_flag[2][playerId] = 1;
    compel_flag[3][playerId] = 1;

    player->hit_flag &= ~INSTANT_SPINOUT;

    if ((power_band_accele[playerId] == true) && ((player->flag & IS_PLAYER) == IS_PLAYER)) {
        player->accelcount = (f32) (player->accelcount + 100.0f);
    }
    if ((g_gameMode == VERSUS) && ((player->flag & IS_CPU_PLAYER) == IS_CPU_PLAYER) && (!g_DemoFlag) &&
        ((player->jugemu_flag & ON_LAKITU_ROD) == 0) && (gGPCurrentRaceRankByPlayerId[playerId] != 0)) {
        player->weapon = (s32) (player->weapon | HIT_BOMB);
    }
}

void spin(Player* player, s8 playerIndex) {
    s16 stackPadding1;
    s16 stackPadding2;
    s16 sp30[5] = { DEGREES(6), DEGREES(6), DEGREES(12), DEGREES(9), DEGREES(10) };

    player->kartroll = 0;
    player->kartpitch = 0;
    if ((player->hit_flag & INSTANT_SPINOUT) == INSTANT_SPINOUT) {
        AccelOff(player, 100.0f);
    } else {
        if ((player->flag & IS_PLAYER) == IS_PLAYER) {
            AccelOff(player, 1.0f);
        } else {
            AccelOff(player, 4.0f);
        }
        if (!(player->flag & IS_PLAYER)) {
            AccelOff(player, 30.0f);
        }
    }
    if ((player->slip_flag & SPIN_L) == SPIN_L) {
        player->direction[1] -= sp30[player->spin_timer];
        D_8018D920[playerIndex] -= sp30[player->spin_timer];
        stackPadding1 = (u16) D_8018D920[playerIndex] / (0x10000 / (0x168 / (sp30[player->spin_timer] / DEGREES(1))));
        if (stackPadding1 == 0) {
            player->spin_timer--;
            if (player->spin_timer <= 0) {
                if (g_gameMode == BATTLE) {
                    cut_balloon(player, playerIndex);
                }
                reset_spin(player, playerIndex);
            }
        }
    } else {
        player->direction[1] += sp30[player->spin_timer];
        D_8018D920[playerIndex] -= sp30[player->spin_timer];
        stackPadding2 = (u16) D_8018D920[playerIndex] / (0x10000 / (0x168 / (sp30[player->spin_timer] / DEGREES(1))));
        if (stackPadding2 == 0) {
            player->spin_timer--;
            if (player->spin_timer <= 0) {
                reset_spin(player, playerIndex);
                if (g_gameMode == BATTLE) {
                    cut_balloon(player, playerIndex);
                }
            }
        }
    }
    if ((power_band_accele[playerIndex] == true) && ((player->flag & IS_PLAYER) == IS_PLAYER)) {
        power_band_timer[playerIndex] = 0x00000078;
        if (player->accelcount <= 90.0f) {
            player->accelcount = 90.0f;
        }
    }
}

void SetWheelspin(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->weapon &= ~HIT_BANANA;
    player->offsetsterr_timer = 0;
    player->offsetsterr_speed = 3.0f;
    player->offsetsterr_flag = 1;
    player->slip_flag &= ~DRIFT;

    if (((player->nowsterr >> 16) >= 20) || ((player->nowsterr >> 16) <= -20) ||
        (((player->speed / 18.0f) * 216.0f) <= 30.0f) || ((player->slip_flag & N_JUMP) != 0) ||
        (((player->flag & IS_PLAYER) == 0) && ((player->slip_flag & CENTER_LINE) == 0))) {
        SetSpin(player, playerIndex);
    } else {
        player->slip_flag |= WHEELSPIN;
    }
}

// almost identical to pro_wheelspin, see there for more documentation
void wheelspin(Player* player, s8 playerIndex) {
    f32 swerve_accel_init;
    s16 swerve_timer;
    s16 swerve_direction;
    s16 swerve_velo_current;

    swerve_accel_init = player->offsetsterr_speed;
    swerve_timer = player->offsetsterr_timer;
    swerve_direction = player->offsetsterr_flag;
    swerve_timer++;
    swerve_velo_current = (swerve_timer * swerve_accel_init) - (0.2 * (swerve_timer * swerve_timer));
    if ((swerve_timer != 0) && (swerve_velo_current < 0)) {
        swerve_timer = 0;
        swerve_direction = -swerve_direction;
        swerve_accel_init *= 0.8;
        // requires braking to recover. A driving spinout allows just releasing gas as well
        if ((player->slip_flag & BREAKE) == BREAKE) {
            player->slip_flag |= DISABLE_SPIN;
        }
        if (swerve_accel_init <= 1.0f) {
            player->slip_flag &= ~WHEELSPIN;
            if ((player->slip_flag & DISABLE_SPIN) != DISABLE_SPIN) {
                SetSpin(player, playerIndex);
                swerve_timer = 0;
            } else {
                player->talk |= MUSIC_NOTE;
                player->slip_flag &= ~DISABLE_SPIN;
                if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                    NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008008);
                    swerve_timer = 0;
                }
            }
        }
    }
    swerve_velo_current *= swerve_direction;
    if ((swerve_velo_current <= 0) && (swerve_direction == 1)) {
        swerve_velo_current = 0;
    }
    if ((swerve_velo_current >= 0) && (swerve_direction == -1)) {
        swerve_velo_current = 0;
    }
    player->sterrangle += swerve_velo_current * 18;
    player->offsetsterr_speed = swerve_accel_init;
    player->offsetsterr_timer = swerve_timer;
    player->offsetsterr_flag = swerve_direction;
    if (player->slip_flag & N_JUMP) {
        SetSpin(player, playerIndex);
        player->slip_flag &= ~WHEELSPIN;
    }
}

void reset_wheelspin(Player* player, UNUSED s8 playerIndex) {
    player->slip_flag &= ~WHEELSPIN;
}

void SetProWheelSpin(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->weapon &= ~PRESS_SAVESPIN;
    player->offsetsterr_timer = 0;
    player->offsetsterr_speed = 2.0f;
    player->offsetsterr_flag = 1;
    player->slip_flag &= ~DRIFT;
    player->handling_flag |= SPINOUT_SWERVE;
}

void pro_wheelspin(Player* player, s8 playerIndex) {
    f32 swerve_accel_init;
    s16 swerve_timer;
    s16 swerve_direction;
    s16 swerve_velo_current;

    // These properties are only used for swerving before spinouts
    swerve_accel_init = player->offsetsterr_speed;
    swerve_timer = player->offsetsterr_timer;
    swerve_direction = player->offsetsterr_flag;
    swerve_timer++;

    // Standard physics formula: Velo_current = velo_init + (accel_init * time) + (accel_jerk * time**2) / 2
    swerve_velo_current = (swerve_accel_init * swerve_timer) - (0.1 * (swerve_timer * swerve_timer)); 

    // Once one swerve finishes, setup to start a smaller one in the opposite direction
    if ((swerve_timer != 0) && (swerve_velo_current < 0)) { // (10 * swerve_accel_init < swerve_timer))
        swerve_timer = 0;
        swerve_direction = -swerve_direction;
        swerve_accel_init *= 0.9;
        if (((player->slip_flag & BREAKE) == BREAKE) || !(player->handling_flag & ACCELERATE)) {
            player->slip_flag |= DISABLE_SPIN;
        }
        // stop swerving once they are small enough
        if (swerve_accel_init <= 1.3) {
            player->handling_flag &= ~SPINOUT_SWERVE;
            if ((player->slip_flag & DISABLE_SPIN) != DISABLE_SPIN) {
                SetSpin(player, playerIndex);
                swerve_timer = 0;
            } else {
                player->talk |= MUSIC_NOTE;
                player->slip_flag &= ~DISABLE_SPIN;
                if ((player->flag & IS_PLAYER) == IS_PLAYER) {
                    NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008008);
                    swerve_timer = 0;
                }
            }
        }
    }
    swerve_velo_current *= swerve_direction;
    if ((swerve_velo_current <= 0) && (swerve_direction == 1)) {
        swerve_velo_current = 0;
    }
    if ((swerve_velo_current >= 0) && (swerve_direction == -1)) {
        swerve_velo_current = 0;
    }
    /* unk_078 contributes to rotational velocity (spin). It looks to be set each frame in steering code
    (e.g. ProStickAngle), so it does not accumulate values from swerve_velo_current over multiple frames */
    player->sterrangle += swerve_velo_current * 20;
    player->offsetsterr_speed = swerve_accel_init;
    player->offsetsterr_timer = swerve_timer;
    player->offsetsterr_flag = swerve_direction;
    if (player->slip_flag & N_JUMP) {
        SetSpin(player, playerIndex);
        player->handling_flag &= ~SPINOUT_SWERVE;
    }
}

void reset_pro_wheelspin(Player* player, UNUSED s8 playerIndex) {
    player->handling_flag &= ~SPINOUT_SWERVE;
}

void SetTurbo(Player* player, s8 playerIndex) {

    kill_strat(player, playerIndex);

    player->slip_flag |= TURBO;
    player->weapon &= ~USE_MUSHROOM;
    player->sus.gachon_timer = 0;
    player->sus.gachon_firstspeed = 8.0f;

    if (replay_flag != 1) {
        if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
            ((player->flag & IS_GHOST) != IS_GHOST)) {
            Na_PlyDash_Start(playerIndex);
            NAPlyTrgStart(playerIndex, 0x1900A40B);
        }
    } else {
        if (player == gPlayerOne) {
            Na_PlyDash_Start(playerIndex);
            NAPlyTrgStart(playerIndex, 0x1900A40B);
        }
    }

    player->turbo_timer = 0x50;
}

void now_turbo(Player* player) {
    player->accelcount = (f32) player->acc_maxcount;
    if (player->turbo_timer > 0) {
        --player->turbo_timer;
    }

    if (player->turbo_timer != 0) {
        chase_Fnumber(&player->turbo_power, 400.0f, 0.5f);
    } else {
        chase_Fnumber(&player->turbo_power, 0.0f, 0.1f);
    }

    if (player->turbo_power <= 1.0f) {
        player->slip_flag &= ~TURBO;
    }
}

void reset_turbo(Player* player) {
    player->slip_flag &= ~TURBO;
    player->turbo_power = 0.0f;
}

void SetWing(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->old_direction = player->direction[1];
    player->slip_flag |= WING;
    player->slip_flag &= ~DRIFT;
    player->weapon &= ~USE_FEATHER;
    player->jmp_acc = D_800E3730[player->kart];
    player->jmp_speed = 0.0f;
    player->jump = (f32) D_800E3710[player->kart];
    player->spin_timer = 1;
    player->slipang = 0;
    player->nowsterr = 0;
    player->sterrangle = 0;
    D_8018D920[playerIndex] = 0;

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyTrgStart(playerIndex, 0x19008002);
    }

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x2900800C);
    }
}

void wing(Player* player, s8 playerIndex) {
    s16 temp;

    if (player->spin_timer == 0) {
        player->direction[1] = player->old_direction;
        temp = 0;
    } else {
        player->direction[1] -= DEGREES(10);
        D_8018D920[playerIndex] -= DEGREES(10);
        temp = ((u16) D_8018D920[playerIndex] / DEGREES(10));
    }
    if (temp == 0) {
        --player->spin_timer;
        if (player->spin_timer <= 0) {
            player->spin_timer = 0;
        }
        if ((player->spin_timer == 0) && ((player->slip_flag & N_JUMP) != N_JUMP)) {
            ResetWing(player);
        }
    }
}

void ResetWing(Player* player) {
    player->slipcount = 0;
    player->nowsterr = 0;
    player->slipang = 0;
    player->direction[1] = player->old_direction;
    player->slip_flag &= ~WING;
    player->gravity = gravity_1[player->kart];
    player->flag &= ~PLAYER_UNKNOWN_0x80;
}

void SetStorm(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->old_direction = player->direction[1];
    player->slip_flag |= STORM;
    player->slip_flag &= ~DRIFT;
    player->weapon &= ~HIT_TORNADO;
    player->jmp_acc = D_800E3770[player->kart];
    player->jmp_speed = 0.0f;
    player->jump = D_800E3750[player->kart];
    D_8018D920[playerIndex] = 0;
    player->spin_timer = 4;
    player->slipang = 0;
    player->nowsterr = 0;
    player->sterrangle = 0;

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008003);
    }
}

void storm(Player* player, s8 playerIndex) {
    s16 temp;

    if (player->spin_timer == 0) {
        player->direction[1] = player->old_direction;
        temp = 0;
    } else {
        player->direction[1] -= DEGREES(10);
        D_8018D920[playerIndex] -= DEGREES(10);
        temp = ((u16) (D_8018D920[playerIndex]) / DEGREES(10));
    }
    if (temp == 0) {
        --player->spin_timer;
        if (player->spin_timer <= 0) {
            player->spin_timer = 0;
        }
        if ((player->spin_timer == 0) && ((player->slip_flag & N_JUMP) != N_JUMP)) {
            reset_storm(player);
        }
    }
}

void reset_storm(Player* player) {
    player->slipcount = 0;
    player->nowsterr = 0;
    player->slipang = 0;
    player->direction[1] = player->old_direction;
    player->slip_flag &= ~STORM;
    player->gravity = gravity_1[player->kart];
}

void roll_brokenkart(Player* player) {
    player->sus.accele += 8.0f;
    if (player->sus.accele >= 140.0f) {
        player->sus.accele = 140.0f;
    }

    if (player->sus.flag == 1) {
        player->sus.position += player->sus.accele;
        if (2002.0f <= player->sus.position) {
            player->sus.accele = 10.0f;
            player->sus.flag *= -1;
        }
    }
    if (player->sus.flag == -1) {
        player->sus.position -= player->sus.accele;
        if (player->sus.position <= -2002.0f) {
            player->sus.accele = 10.0f;
            player->sus.flag *= -1;
        }
    }
}

void SetBroken(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    if ((player->slip_flag & BROKEN) == 0) {
        player->sus.broken_timer = 0;
        player->broken_timer = 0;
        player->sus.broken_firstspeed = 4.5f;
        D_8018D990[playerIndex] = 0;
        player->slip_flag &= ~(RESTORE | DRIFT);
        compel_flag[0][playerIndex] = 1;
        compel_flag[1][playerIndex] = 1;
        compel_flag[2][playerIndex] = 1;
        compel_flag[3][playerIndex] = 1;
        player->sus.flag = 1;
        player->sus.position = 0.0f;
        player->sus.accele = 65.0f;

        if ((player->weapon & PRESS_SQUISH) != 0) {
            player->hit_flag |= 0x80;
        }

        if (((player->flag & IS_PLAYER) != 0) && ((player->slip_flag & BROKEN) == 0)) {
            NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05));
        }

        player->slip_flag |= BROKEN;
        if (((player->flag) & IS_CPU_PLAYER) != 0) {
            oga_enemy_voice(playerIndex, player);
        }
    }
}

void broken(Player* player, s8 playerIndex) {
    player->jumpcount = 0;
    player->slipcount = 0;
    player->nowsterr = 0;
    player->slipang = 0;
    player->force = 0.0f;
    player->accelcount = 0.0f;
    // clang-format off
    if ((player->bump.distance_zx >= 600.0f) || ((player->slip_flag & CENTER_LINE) != 0)) { D_8018D990[playerIndex] = 3; } // placed block on same line to match
    // clang-format on

    switch (D_8018D990[playerIndex]) {
        case 0:
            player->sus.broken_firstspeed = 4.5f;
            if (player->broken_timer < 0x3D) {
                ++player->broken_timer;
            }

            if ((player->hit_flag & 0x80) != 0) {
                if ((player->weapon & PRESS_SQUISH) == 0) {
                    D_8018D990[playerIndex] = 1;
                    player->broken_timer = 0;
                    if ((player->flag & IS_PLAYER) != 0) {
                        NAPlyTrgStart(playerIndex, 0x1901904B);
                        break;
                    }
                }
            } else {
                ++player->broken_timer;
                if (player->broken_timer >= 0x1E) {
                    D_8018D990[playerIndex] = 1;
                    player->broken_timer = 0;
                    if ((player->flag & IS_PLAYER) != 0) {
                        NAPlyTrgStart(playerIndex, 0x1901904B);
                        break;
                    }
                }
                break;
            }

            break;
        case 1:
            player->sus.broken_firstspeed = 4.5f;
            player->position[1] += 0.13;
            ++player->broken_timer;

            if ((player->hit_flag & 0x80) != 0) {
                if (player->broken_timer >= 0x32) {
                    D_8018D990[playerIndex] = 2;
                    player->broken_timer = 0;
                    player->hit_flag &= 0xFF7F;
                }
            } else if (player->broken_timer >= 0x50) {
                D_8018D990[playerIndex] = 2;
                player->broken_timer = 0;
            }

            player->sus.accele += 6.0f;
            if (player->sus.accele >= 90.0f) {
                player->sus.accele = 90.0f;
            }

            player->sus.position += player->sus.accele;
            if (3458.0f <= player->sus.position) {
                player->sus.accele = 0.0f;
                break;
            }
            break;
        case 2:
            ++player->broken_timer;
            if (player->broken_timer >= 0x259) {
                D_8018D990[playerIndex] = 3;
                player->broken_timer = 0;
            }

            if (player->bump.distance_zx >= 600.0f) {
                D_8018D990[playerIndex] = 3;
            }

            player->sus.broken_firstspeed = 4.5f;
            player->position[1] -= 0.085;

            if ((player->slip_flag & N_JUMP) != N_JUMP) {
                D_8018D990[playerIndex] = 3;
                player->broken_timer = 0;
            }

            roll_brokenkart(player);
            break;
        case 3:
            player->sus.broken_firstspeed = 3.0f;
            player->slip_flag &= ~BROKEN;
            player->sus.broken_timer = 0;
            player->slip_flag |= RESTORE;
            player->offsetsize = 1.0f;
            player->radius = g_charRadiusTbl[player->kart];
            compel_flag[0][playerIndex] = 1;
            compel_flag[1][playerIndex] = 1;
            compel_flag[2][playerIndex] = 1;
            compel_flag[3][playerIndex] = 1;

            if ((player->flag & IS_PLAYER) != 0) {
                NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008008);
            }
            break;
    }
}

void SetThunder(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->weapon &= ~HIT_LIGHTNING;
    player->slip_flag |= (THUNDER | THUNDER_SPIN);
    player->slip_flag &= ~DRIFT;
    player->force *= 0.6;
    player->thunder_timer = 0;
    player->offsetsize = 1.0f;
    g_StarUseCounter[playerIndex] = g_gameTimer;
    player->old_direction = player->direction[1];
    player->spin_timer = 2;
    player->slipang = 0;
    player->nowsterr = 0;
    player->sterrangle = 0;

    compel_flag[0][playerIndex] = 1;
    compel_flag[1][playerIndex] = 1;
    compel_flag[2][playerIndex] = 1;
    compel_flag[3][playerIndex] = 1;

    D_8018D920[playerIndex] = 0;

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008003);
    } else {
        oga_enemy_voice(playerIndex, player);
    }
    if (g_gameMode == BATTLE) {
        cut_balloon(player, playerIndex);
    }
}

void thunder(Player* player, s8 playerIndex) {
    s16 test;
    if (((player->slip_flag & CARHIT) == CARHIT) &&
        ((player->slip_flag & BROKEN) != BROKEN)) {
        player->slip_flag &= ~THUNDER_SPIN;
        player->slipcount = 0;
        player->nowsterr = 0;
        player->slipang = 0;
        player->direction[1] = player->old_direction;
        reset_thunder(player, playerIndex);
        compel_flag[0][playerIndex] = 1;
        compel_flag[1][playerIndex] = 1;
        compel_flag[2][playerIndex] = 1;
        compel_flag[3][playerIndex] = 1;
        SetBroken(player, playerIndex);
    } else if ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) {
        player->direction[1] -= DEGREES(8);
        D_8018D920[playerIndex] -= DEGREES(8);
        test = (u16) D_8018D920[playerIndex] / DEGREES(8);
        if (test == 0) {
            player->spin_timer--;
            if (player->spin_timer <= 0) {
                player->slipcount = 0;
                player->slip_flag &= ~THUNDER_SPIN;
                player->nowsterr = 0;
                player->slipang = 0;
                player->direction[1] = player->old_direction;
                compel_flag[0][playerIndex] = 1;
                compel_flag[1][playerIndex] = 1;
                compel_flag[2][playerIndex] = 1;
                compel_flag[3][playerIndex] = 1;
            }
        }
        AccelOff(player, 1.0f);
    } else {
        player->thunder_timer += 1;
        player->force = (f32) ((f64) player->force * 0.6);
        if ((player->thunder_timer == 1) && (player->flag & IS_PLAYER)) {
            NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008005);
        }
        if ((player->thunder_timer >= 0) && (player->thunder_timer < 0x1CC)) {
            chase_Fnumber(&player->offsetsize, 0.7f, 0.1f);
            chase_Fnumber(&player->radius,
                             (f32) ((f64) g_charRadiusTbl[player->kart] * 0.9), 0.1f);
        } else {
            reset_thunder(player, playerIndex);
            if (player->flag & IS_PLAYER) {
                NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008008);
            }
        }
    }
}

void reset_thunder(Player* player, UNUSED s8 playerIndex) {
    chase_Fnumber(&player->offsetsize, 1.0f, 0.1f);
    chase_Fnumber(&player->radius, g_charRadiusTbl[player->kart], 0.1f);

    player->slip_flag &= ~THUNDER;
    player->offsetsize = 1.0f;
    player->radius = g_charRadiusTbl[player->kart];
    player->sus.broken_firstspeed = 3.0f;
    player->sus.broken_timer = 0;
    player->slip_flag |= RESTORE;

    if ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) {
        player->direction[1] = player->old_direction;
    }

    player->slip_flag &= ~THUNDER_SPIN;
}

void bomb(Player* player, s8 playerIndex) {
    player->kartroll = 0;
    player->kartpitch = 0;
    player->slipang = 0;
    player->nowsterr = 0;
    player->jumpcount = 0xF;
    player->kage_ang += DEGREES(15);
    player->force = 0.0f;
    player->accelcount = 0.0f;
    player->velocity[0] = 0.0f;
    player->velocity[2] = 0.0f;
    player->slip_flag &= ~(SPIN_L | SPIN_R);

    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        ++player->bomb_timer;
    }

    if (player->bomb_timer == 3) {
        player->slip_flag &= ~EXPLODE;
        player->slipcount = 0;
        player->roliover_time = 0;
        compel_flag[0][playerIndex] = 1;
        compel_flag[1][playerIndex] = 1;
        compel_flag[2][playerIndex] = 1;
        compel_flag[3][playerIndex] = 1;
        player->kage_ang = 0;
        player->flag &= ~PLAYER_UNKNOWN_0x80;

        if ((power_band_accele[playerIndex] == true) && ((player->flag & IS_PLAYER) == IS_PLAYER)) {
            player->accelcount += 100.0f;
        }
        if (g_gameMode == BATTLE) {
            cut_balloon(player, playerIndex);
        }
    } else {
        player->slipcount += 0x80;
        if (player->slipcount >= 0x2000) {
            player->slipcount = 0;
            --player->roliover_time;
            if (player->roliover_time == 0) {
                player->slip_flag &= ~EXPLODE;
                player->roliover_time = 0;
                compel_flag[0][playerIndex] = 1;
                compel_flag[1][playerIndex] = 1;
                compel_flag[2][playerIndex] = 1;
                compel_flag[3][playerIndex] = 1;
                player->kage_ang = 0;

                if (g_gameMode == BATTLE) {
                    cut_balloon(player, playerIndex);
                }
                if ((power_band_accele[playerIndex] == true) &&
                    ((player->flag & IS_PLAYER) == IS_PLAYER)) {
                    player->accelcount += 100.0f;
                }

                player->flag &= ~PLAYER_UNKNOWN_0x80;
            }
        }
    }
}

void SetBombRollover(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);
    check_hit_letter(player);

    player->slipcount = 0;
    player->slip_flag |= EXPLODE;
    player->slip_flag &= ~DRIFT;
    player->jmp_acc = 0.0f;
    player->jmp_speed = 0.0f;
    player->jump = 0.0f;
    player->jmp_acc = D_800E3730[player->kart];
    player->jump = D_800E3710[player->kart];
    player->roliover_time = 4;
    player->kage_ang = 0;
    player->bomb_timer = 0;

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        if (((g_gameMode == VERSUS) && ((player->flag & IS_CPU_PLAYER) != 0)) && (!g_DemoFlag)) {
            Na_SeMute_Off(playerIndex);
        }

        if (1) {}

        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008005);
        if (((g_gameMode == VERSUS) && ((player->flag & IS_CPU_PLAYER) != 0)) && (!g_DemoFlag)) {
            Na_SeMute_Off(playerIndex);
        }
        NAPlyTrgStart(playerIndex, SOUND_ACTION_EXPLOSION);
    } else {
        oga_enemy_voice(playerIndex, player);
    }

    player->weapon &= ~(HIT_BOMB | HIT_BOAT);
    player->talk |= CRASH;
    power_band_timer[playerIndex] = 0;
    power_band_accele[playerIndex] = false;
    accele_flag[playerIndex] = 0;
    accele_time[playerIndex] = 0;
}

void reset_bomb(Player* player, s8 playerIndex) {
    player->slip_flag &= ~EXPLODE;
    player->slipcount = 0;
    player->roliover_time = 0;
    compel_flag[0][playerIndex] = 1;
    compel_flag[1][playerIndex] = 1;
    compel_flag[2][playerIndex] = 1;
    compel_flag[3][playerIndex] = 1;
    player->kage_ang = 0;
}

void bomb_throw(Player* player, s8 playerIndex) {
    player->kartroll = 0;
    player->kartpitch = 0;
    player->slipang = 0;
    player->nowsterr = 0;
    player->jumpcount = 0xF;
    player->kage_ang += DEGREES(15);
    player->force /= 2;
    player->accelcount = 0.0f;
    player->slip_flag &= ~(SPIN_L | SPIN_R);

    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        ++player->bomb_timer;
    }

    if (player->bomb_timer == 4) {
        player->slip_flag &= ~THROW_EXPLODE;
        player->slipcount = 0;
        player->roliover_time = 0;
        compel_flag[3][playerIndex] = 1;
        compel_flag[0][playerIndex] = 1;
        compel_flag[1][playerIndex] = 1;
        compel_flag[2][playerIndex] = 1;
        player->kage_ang = 0;

        if ((power_band_accele[playerIndex] == true) && ((player->flag & IS_PLAYER) == IS_PLAYER)) {
            player->accelcount += 100.0f;
        }

        if (g_gameMode == BATTLE) {
            cut_balloon(player, playerIndex);
        }
    } else {
        player->slipcount = (s16) (player->slipcount + 0x90);
        if (((s32) player->slipcount) >= 0x2000) {
            player->slipcount = 0;
            --player->roliover_time;
            if (player->roliover_time == 0) {
                player->slip_flag &= ~THROW_EXPLODE;
                player->roliover_time = 0;
                compel_flag[0][playerIndex] = 1;
                compel_flag[1][playerIndex] = 1;
                compel_flag[2][playerIndex] = 1;
                compel_flag[3][playerIndex] = 1;
                player->kage_ang = 0;
                if ((power_band_accele[playerIndex] == true) &&
                    ((player->flag & IS_PLAYER) == IS_PLAYER)) {
                    player->accelcount += 100.0f;
                }

                if (g_gameMode == BATTLE) {
                    cut_balloon(player, playerIndex);
                }
            }
        }
    }
}

void SetBombThrowRollover(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);
    check_hit_letter(player);

    player->slipcount = 0;
    player->slip_flag &= ~DRIFT;
    player->jmp_acc = 0.0f;
    player->jmp_speed = 0.0f;
    player->jump = 0.0f;
    player->jmp_acc = D_800E3730[player->kart];
    player->jump = D_800E3710[player->kart];
    player->roliover_time = 4;
    player->kage_ang = 0;
    player->bomb_timer = 0;

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008005);
        NAPlyTrgStart(playerIndex, SOUND_ACTION_EXPLOSION);
    } else {
        oga_enemy_voice(playerIndex, player);
    }

    player->slip_flag |= THROW_EXPLODE;
    player->talk |= CRASH;
    player->weapon &= ~(HIT_BOMB_THROW | HIT_REDSHELL);

    power_band_timer[playerIndex] = 0;
    power_band_accele[playerIndex] = false;
    accele_flag[playerIndex] = 0;
    accele_time[playerIndex] = 0;
}

void reset_bomb_throw(Player* player, s8 playerIndex) {
    player->slip_flag &= ~THROW_EXPLODE;
    player->slipcount = 0;
    player->roliover_time = 0;
    compel_flag[0][playerIndex] = 1;
    compel_flag[1][playerIndex] = 1;
    compel_flag[2][playerIndex] = 1;
    compel_flag[3][playerIndex] = 1;
    player->kage_ang = 0;
}

void set_dashjump(Player* player, s8 playerId) {
    kill_strat(player, playerId);

    player->slip_flag |= DASH_JUMP;
    player->weapon &= ~PRESS_BOOSTPAD;
    player->sus.gachon_timer = 0;
    player->sus.gachon_firstspeed = 8.0f;
    if (replay_flag != 1) {
        if (((player->flag & IS_PLAYER) == IS_PLAYER) && ((player->flag & IS_GHOST) == 0)) {
            NAPlyVoiceStart(playerId, (player->kart * 0x10) + 0x29008001);
            NAPlyTrgStart(playerId, 0x1900A40B);
        }
    } else if (player == gPlayerOne) {
        NAPlyVoiceStart(playerId, (player->kart * 0x10) + 0x29008001);
        NAPlyTrgStart(playerId, 0x1900A40B);
    }
    player->handling_flag &= ~REVERSE_GEAR;
    player->slip_flag &= ~SPIN_TURN;
}

void now_dashjump(Player* player) {
    f64 temp_f0;

    player->accelcount = player->acc_maxcount;
    if ((u16) player->wallhitcount > 0) {
        player->accelcount = 0.0f;
    }
    if ((player->bump_status != BOOST_RAMP_ASPHALT) && ((player->slip_flag & N_JUMP) != N_JUMP)) {
        chase_Fnumber(&player->turbo_power, 0, 1.0f);
    } else {
        chase_Fnumber(&player->turbo_power, 400.0f, 0.01f);
    }
    if (player->turbo_power <= 1.0f) {
        player->slip_flag &= ~DASH_JUMP;
        player->turbo_power = 0.0f;
        if (player->jumpcount >= 0x33) {
            temp_f0 = 0.7;
            player->accelcount = (player->accelcount * temp_f0);
            player->force = (player->force * temp_f0);
        }
    }
}

void reset_dashjump(Player* player) {
    player->slip_flag &= ~DASH_JUMP;
    player->turbo_power = 0.0f;
}

void set_dash_midjump(Player* player, s8 playerId) {
    kill_strat(player, playerId);

    player->slip_flag |= DASH_MIDJUMP;
    player->weapon &= ~PRESS_BOOSTJUMP;

    if (replay_flag != 1) {
        if (((player->flag & IS_PLAYER) == IS_PLAYER) && ((player->flag & IS_GHOST) == 0)) {
            NAPlyVoiceStart(playerId, (player->kart * 0x10) + 0x29008001);
            NAPlyTrgStart(playerId, 0x1900A40B);
        }
    } else if (player == gPlayerOne) {
        NAPlyVoiceStart(playerId, (player->kart * 0x10) + 0x29008001);
        NAPlyTrgStart(playerId, 0x1900A40B);
    }

    player->handling_flag &= ~REVERSE_GEAR;
    player->slip_flag &= ~SPIN_TURN;
}

void now_dash_midjump(Player* player) {
    player->accelcount = gTopSpeedTable[0][player->kart];

    if ((player->bump_status != BOOST_RAMP_WOOD) && ((player->slip_flag & N_JUMP) != N_JUMP)) {
        chase_Fnumber(&player->turbo_power, 0, 1.0f);
    } else {
        chase_Fnumber(&player->turbo_power, 300.0f, 0.1f);
    }

    if (player->turbo_power <= 1.0f) {
        player->slip_flag &= ~DASH_MIDJUMP;
        player->turbo_power = 0.0f;
        player->accelcount /= 2;
        player->force /= 2;
    }
}

void reset_dash_midjump(Player* player) {
    player->slip_flag &= ~DASH_MIDJUMP;
    player->turbo_power = 0.0f;
}

void set_rapidacc(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->old_direction = player->direction[1];
    player->spin_timer = 2;
    player->slipang = 0;
    player->nowsterr = 0;
    player->slip_flag |= RAPID_ACC;
    player->sterrangle = 0;
    D_8018D920[playerIndex] = -0x8000;

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008003);
    }
}

void rapidacc(Player* player, s8 arg1) {
    s16 temp;

    player->force = (player->max_power * 0.05);
    if (player->spin_timer < 0) {
        if ((player->handling_flag & START_SPIN_LEFT) == START_SPIN_LEFT) {
            player->direction[1] += DEGREES(1);
            D_8018D920[arg1] += DEGREES(1);

            temp = ((u16) D_8018D920[arg1] / DEGREES(1));
            if (temp == 180) {
                player->slip_flag &= ~RAPID_ACC;
                player->flag &= ~PLAYER_UNKNOWN_0x80;
                player->accelcount /= 3.0f;
            }
        } else {

            player->direction[1] -= DEGREES(1);
            D_8018D920[arg1] -= DEGREES(1);
            temp = ((u16) D_8018D920[arg1] / DEGREES(1));
            if (temp == 180) {
                player->slip_flag &= ~RAPID_ACC;
                player->flag &= ~PLAYER_UNKNOWN_0x80;
                player->accelcount /= 3.0f;
            }
        }
    } else {
        if ((player->spin_timer & 1) != 0) {
            player->direction[1] -= DEGREES(2);
            D_8018D920[arg1] -= DEGREES(2);
            temp = ((u16) D_8018D920[arg1] / DEGREES(2));
            if (temp < 71) {
                --player->spin_timer;
            }
            player->handling_flag |= START_SPIN_LEFT;
            player->handling_flag &= ~START_SPIN_RIGHT;
            return;
        }
        player->direction[1] += DEGREES(2);
        D_8018D920[arg1] += DEGREES(2);
        temp = ((u16) D_8018D920[arg1] / DEGREES(2));
        if (temp >= 110) {
            --player->spin_timer;
        }
        player->handling_flag |= START_SPIN_RIGHT;
        player->handling_flag &= ~START_SPIN_LEFT;
    }
}

void reset_rapidacc(Player* player) {
    player->slip_flag &= ~RAPID_ACC;
}

void rollover_fall(Player* player, UNUSED s8 arg1) {
    player->slipcount += 0x80;
    player->kage_ang += DEGREES(10);
    player->nowsterr = 0;
    player->accelcount = 0.0f;
    player->force /= 2;
    if (player->slipcount >= 0x2000) {
        player->slipcount = 0;
        --player->roliover_time;
        if (player->roliover_time == 0) {
            player->slip_flag &= ~ROLLOVER_FALL;
            ExplorerSetJugemu(player);
            ExplorerCallJugemu(player);
        }
    }
}

void SetRolloverFall(Player* player, s8 playerIndex) {
    if ((((player->slip_flag & SPIN_L) != 0) || ((player->slip_flag & SPIN_R) != 0) ||
         ((player->slip_flag & EXPLODE)) || ((player->slip_flag & THROW_EXPLODE)) ||
         ((player->slip_flag & ROLLOVER) != 0)) &&
        (g_gameMode == BATTLE)) {
        player->handling_flag |= BALLOON_CUT;
    }

    kill_strat(player, playerIndex);
    ResetStar(player, playerIndex);

    player->slipcount = 0;
    player->slip_flag |= ROLLOVER_FALL;
    player->slip_flag &= ~DRIFT;
    player->roliover_time = 0x1E;
    player->kage_ang = 0;

    if (((player->flag & IS_PLAYER) != 0) && ((player->flag & IS_GHOST) == 0) &&
        ((player->jugemu_flag & ON_LAKITU_ROD) == 0) && ((player->water_flag & SUBMERGED) == 0) &&
        ((player->water_flag & HALF_SUBMERGED) == 0)) {
        NAPlyVoiceStart(playerIndex, (player->kart * 0x10) + 0x29008004);
    }
}

void ResetRolloverFall(Player* player, s8 playerIndex) {

    if ((player->handling_flag & BALLOON_CUT) != 0) {
        cut_balloon(player, playerIndex);
        player->handling_flag &= ~BALLOON_CUT;
    }

    player->kartroll = 0;
    player->kartpitch = 0;
    player->slip_flag &= ~ROLLOVER_FALL;
    player->slipcount = 0;
    player->slipang = 0;
    player->nowsterr = 0;
    player->roliover_time = 0;
    player->sterrangle = 0;
    player->accelcount = 0.0f;

    compel_flag[0][playerIndex] = 1;
    compel_flag[1][playerIndex] = 1;
    compel_flag[2][playerIndex] = 1;
    compel_flag[3][playerIndex] = 1;
    player->kage_ang = 0;
}

void star(Player* player, s8 playerIndex) {
    if (((s32) g_gameTimer - g_StarUseCounter[playerIndex]) >= STAR_EFFECT_DURATION - 1) {
        D_8018D900[playerIndex] = 1;

        if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
            ((player->flag & IS_GHOST) != IS_GHOST)) {
            if (D_8018D900[playerIndex] == 1) {
                StopStarMusicHook(playerIndex);
                D_8018D900[playerIndex] = 0;
            }
        } else if (D_8018D900[playerIndex] == 1) {
            Na_EnmMuteki_Stop((u8) playerIndex);
            D_8018D900[playerIndex] = 0;
        }
    }

    if (((s32) g_gameTimer - g_StarUseCounter[playerIndex]) >= STAR_EFFECT_DURATION) {
        player->slip_flag &= ~STAR;
    }
}

// Star item
void SetStar(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->slip_flag |= STAR;
    player->weapon &= ~USE_STAR;
    g_StarUseCounter[playerIndex] = g_gameTimer;
    D_8018D900[playerIndex] = 1;

    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        if (D_8018D900[playerIndex] == 1) {
            PlayStarMusicHook(playerIndex);
            D_8018D900[playerIndex] = 2;
        }
        // This may be in charge of ending the star sound. Still unknown though.
    } else if (D_8018D900[playerIndex] == 1) {
        Na_EnmMuteki_Start(playerIndex);
        D_8018D900[playerIndex] = 2;
    }
}

void ResetStar(Player* player, s8 playerIndex) {
    player->slip_flag &= ~STAR;
    if (((player->flag & IS_PLAYER) == IS_PLAYER) &&
        ((player->flag & IS_GHOST) != IS_GHOST)) {
        StopStarMusicHook(playerIndex);
        return;
    }
    Na_EnmMuteki_Stop(playerIndex);
}

void vs_ghost(Player* player, s8 playerIndex) {
    s32 time_elapsed;
    time_elapsed = ((s32) g_gameTimer) - g_GhostUseCounter[playerIndex];
    if (time_elapsed < BOO_EFFECT_DURATION) {
        player->erase -= 2;

        if (player->erase <= ALPHA_BOO_EFFECT) {
            player->erase = ALPHA_BOO_EFFECT;
        }
        // Player becomes invisible to other players
        g_GhostUseTimer[playerIndex] -= 2;
        if (g_GhostUseTimer[playerIndex] <= 0) {
            g_GhostUseTimer[playerIndex] = 0;
        }
    } else {
        // Player returns to normal visibility
        player->erase += 4;
        if (player->erase >= 0xF0) {
            player->erase = ALPHA_MAX;
            g_GhostUseTimer[playerIndex] = ALPHA_MAX;
            player->slip_flag &= ~TERESA;
            if ((player->flag & IS_PLAYER) != 0) {
                Na_PlyObake_Stop(playerIndex);
            }
        }

        g_GhostUseTimer[playerIndex] += 8;
        if (g_GhostUseTimer[playerIndex] >= 0xF0) {
            g_GhostUseTimer[playerIndex] = ALPHA_MAX;
            player->erase = ALPHA_MAX;
            player->slip_flag &= ~TERESA;
            if ((player->flag & IS_PLAYER) != 0) {
                Na_PlyObake_Stop(playerIndex);
            }
        }
    }
}

void SetVSGhost(Player* player, s8 playerIndex) {
    s16 temp_v1;
    // become boo
    if ((player->flag & IS_PLAYER) != 0) {
        player->handling_flag |= BOO_SMOKE_EFFECT;

        for (temp_v1 = 0; temp_v1 < 10; ++temp_v1) {
            player->gass[temp_v1].flag = 0;
            player->gass[temp_v1].timer = 0;
            player->gass[temp_v1].number = 0;
        }
    }

    kill_strat(player, playerIndex);

    player->slip_flag |= TERESA;
    player->weapon &= ~USE_BOO;
    g_GhostUseCounter[playerIndex] = g_gameTimer;
    g_GhostUseTimer[playerIndex] = ALPHA_MAX;

    if ((player->flag & IS_PLAYER) != 0) {
        Na_PlyObake_Start(playerIndex);
    }
}

void ResetVSGhost(Player* player, s8 playerIndex) {
    player->erase += 8;
    if (player->erase >= 0xF0) {
        player->erase = ALPHA_MAX;
        g_GhostUseTimer[playerIndex] = ALPHA_MAX;

        player->slip_flag &= ~TERESA;
        if ((player->flag & IS_PLAYER) != 0) {
            Na_PlyObake_Stop(playerIndex);
        }
    }

    g_GhostUseTimer[playerIndex] += 0x10;
    if (g_GhostUseTimer[playerIndex] >= 0xE0) {
        g_GhostUseTimer[playerIndex] = ALPHA_MAX;
        player->erase = ALPHA_MAX;
        player->slip_flag &= ~TERESA;
        if ((player->flag & IS_PLAYER) != 0) {
            Na_PlyObake_Stop(playerIndex);
        }
    }
}

void kill_bomb_kart(Player* player) {
    s32 playerIndex;

    if ((player->flag & IS_BOMB) != 0) {
        playerIndex = check_kart_number(player);
        player->flag = (IS_PLAYER | NO_CONTROLS | IS_CPU_PLAYER);
        kwfinish_bombkart(playerIndex);
    }
}

void change_bomb(Player* player, s8 arg1) {
    player->erase -= 4;
    if (player->erase < 5) {
        player->erase = ALPHA_MIN;
        player->weapon &= ~PRESS_BECOME_BOMB;
        player->weapon |= PRESS_IS_BOMB;
        player->flag |= IS_BOMB;

        set_real_bomb(player, arg1);
        kwstart_bombkart(arg1);
    }
}

void real_bomb(Player* player, s8 playerIndex) {
    player->erase += 2;
    if (player->erase >= 0xF0) {
        player->erase = ALPHA_MAX;
        player->weapon &= ~PRESS_IS_BOMB;
    }

    kwset_bombkart_alpha(playerIndex, (u32) player->erase);
}

void set_change_bomb(Player* player, UNUSED s8 arg1) {
    s16 temp_v0;

    player->weapon |= PRESS_BECOME_BOMB;
    player->handling_flag |= BOO_SMOKE_EFFECT;

    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        player->gass[temp_v0].flag = 0;
        player->gass[temp_v0].timer = 0;
        player->gass[temp_v0].number = 0;
    }
}
// become bomb
void set_real_bomb(Player* player, UNUSED s8 arg1) {
    s16 temp_v0;
    player->handling_flag |= BOO_SMOKE_EFFECT;
    for (temp_v0 = 0; temp_v0 < 10; ++temp_v0) {
        player->gass[temp_v0].flag = 0;
        player->gass[temp_v0].timer = 0;
        player->gass[temp_v0].number = 0;
    }
}

void SetHeightJump(Player* player, s8 playerIndex) {
    kill_strat(player, playerIndex);

    player->slip_flag &= ~DRIFT;
    player->jmp_acc = D_800E37F0[player->kart];
    player->jmp_speed = 0.0f;
    player->jump = D_800E37D0[player->kart];
    player->weapon &= ~UNUSED_TRIGGER_0x10000;
    player->slip_flag |= HEIGHT_JUMP;
}

void height_jump(Player* player, UNUSED s8 playerIndex) {
    player->slip_flag &= ~DRIFT;
    if ((player->slip_flag & N_JUMP) != N_JUMP) {
        player->slip_flag &= ~HEIGHT_JUMP;
        player->accelcount /= 2;
        player->force /= 2;
    }
}

void reset_height_jump(Player* player, UNUSED s8 playerIndex) {
    player->slip_flag &= ~HEIGHT_JUMP;
    player->jmp_acc = 0.0f;
    player->jump = 0.0f;
    player->jmp_speed = 0.0f;
}

void make_chasepoint(Player* player, s8 playerId) {
    s16 pathPoint;

#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_BOWSER_CASTLE:
            pathPoint = gNearestPathPointByPlayerId[playerId];
            if ((pathPoint >= 0x235) && (pathPoint < 0x247)) {
                player->chasepoint = 0x214;
            } else if ((pathPoint >= 0x267) && (pathPoint < 0x277)) {
                player->chasepoint = 0x25B;
            } else {
                player->chasepoint = gNearestPathPointByPlayerId[playerId];
                if (player->chasepoint < 0) {
                    player->chasepoint = g_courseTotalPathPoints[0] + player->chasepoint;
                }
            }
            break;
        case COURSE_BANSHEE_BOARDWALK:
            pathPoint = gNearestPathPointByPlayerId[playerId];
            if ((pathPoint >= 0x12C) && (pathPoint < 0x13C)) {
                player->chasepoint = 0x12CU;
            } else {
                player->chasepoint = gNearestPathPointByPlayerId[playerId];
                if (player->chasepoint < 0) {
                    player->chasepoint = g_courseTotalPathPoints[0] + player->chasepoint;
                }
            }
            break;
        case COURSE_YOSHI_VALLEY:
        case COURSE_RAINBOW_ROAD:
            player->chasepoint = g_playerPathPointCopy[playerId];
            break;
        case COURSE_FRAPPE_SNOWLAND:
            pathPoint = gNearestPathPointByPlayerId[playerId];
#ifdef VERSION_EU
            if (((pathPoint >= 0xF0) && (pathPoint < 0x11E)) || ((g_playerPathPointCopy[playerId] >= 0xF0) &&
                                                                 (g_playerPathPointCopy[playerId] < 0x11E)))
#else
            if ((pathPoint >= 0xF0) && (pathPoint < 0x105))
#endif
            {
                player->chasepoint = 0xF0U;
            } else {
                player->chasepoint = g_playerPathPointCopy[playerId];
                if (player->chasepoint < 0) {
                    player->chasepoint = g_courseTotalPathPoints[0] + player->chasepoint;
                }
            }
            break;
        case COURSE_ROYAL_RACEWAY:
            pathPoint = gNearestPathPointByPlayerId[playerId];
            if ((pathPoint >= 0x258) && (pathPoint < 0x2A4)) {
                player->chasepoint = 0x258U;
            } else {
                player->chasepoint = g_playerPathPointCopy[playerId];
                if (player->chasepoint < 0) {
                    player->chasepoint = g_courseTotalPathPoints[0] + player->chasepoint;
                }
            }
            break;
        case COURSE_DK_JUNGLE:
            pathPoint = gNearestPathPointByPlayerId[playerId];
            if ((pathPoint >= 0xB9) && (pathPoint < 0x119)) {
                player->chasepoint = 0xB9U;
            } else {
                player->chasepoint = gNearestPathPointByPlayerId[playerId];
                if (player->chasepoint < 0) {
                    player->chasepoint = g_courseTotalPathPoints[0] + player->chasepoint;
                }
            }
            break;
        case COURSE_BLOCK_FORT:
        case COURSE_SKYSCRAPER:
        case COURSE_DOUBLE_DECK:
        case COURSE_BIG_DONUT:
            player->chasepoint = 0U;
            break;
        default:
            player->chasepoint = gNearestPathPointByPlayerId[playerId];
            if (player->chasepoint < 0) {
                player->chasepoint = g_courseTotalPathPoints[0] + player->chasepoint;
            }
            break;
    }
#else

#endif
}

void func_80090178(Player* player, s8 playerId, Vec3f arg2, Vec3f arg3) {
    u16 test;
    CenterPathStruct* temp_v1;
    f32 spF8[4] = { 0.0f, 0.0f, -700.0f, 700.0f };
    f32 spE8[4] = { 700.0f, -700.0f, 0.0f, 0.0f };
    f32 spD8[4] = { 0.0f, 0.0f, -650.0f, 650.0f };
    f32 spC8[4] = { 650.0f, -650.0f, 0.0f, 0.0f };
    f32 spB8[4] = { 0.0f, 0.0f, -400.0f, 400.0f };
    f32 spA8[4] = { 400.0f, -400.0f, 0.0f, 0.0f };
    f32 sp98[4] = { 0.0f, 0.0f, -350.0f, 350.0f };
    f32 sp88[4] = { 350.0f, -350.0f, 0.0f, 0.0f };
    f32 sp78[4] = { 0.0f, 0.0f, -675.0f, 675.0f };
    f32 sp68[4] = { 675.0f, -675.0f, 0.0f, 0.0f };
    f32 sp58[4] = { 0.0f, 0.0f, -550.0f, 550.0f };
    f32 sp48[4] = { 550.0f, -550.0f, 0.0f, 0.0f };
    f32 sp38[4] = { 0.0f, 0.0f, -575.0f, 575.0f };
    f32 sp28[4] = { 575.0f, -575.0f, 0.0f, 0.0f };
    f32 sp18[4] = { 10.0f, -10.0f, -575.0f, 575.0f };
    f32 sp08[4] = { 575.0f, -575.0f, 10.0f, -10.0f };

    switch (g_courseID) {
        case COURSE_YOSHI_VALLEY:
            test = player->chasepoint;
            temp_v1 = &CenterPathBP[jugemu_line[playerId]][test];
            arg2[0] = temp_v1->pointx;
            arg2[1] = temp_v1->pointy;
            arg2[2] = temp_v1->pointz;
            temp_v1 = &CenterPathBP[jugemu_line[playerId]]
                                  [(player->chasepoint + 5) %
                                   (g_courseTotalPathPoints[jugemu_line[playerId]] + 1)];
            arg3[0] = temp_v1->pointx;
            arg3[1] = temp_v1->pointy;
            arg3[2] = temp_v1->pointz;
            break;
        case COURSE_BLOCK_FORT:
            arg2[0] = spF8[playerId];
            arg2[1] = 0.0f;
            arg2[2] = spE8[playerId];
            arg3[0] = spD8[playerId];
            arg3[1] = 0.0f;
            arg3[2] = spC8[playerId];
            break;
        case COURSE_SKYSCRAPER:
            arg2[0] = spB8[playerId];
            arg2[1] = 480.0f;
            arg2[2] = spA8[playerId];
            arg3[0] = sp98[playerId];
            arg3[1] = 480.0f;
            arg3[2] = sp88[playerId];
            break;
        case COURSE_DOUBLE_DECK:
            arg2[0] = sp78[playerId];
            arg2[1] = 0.0f;
            arg2[2] = sp68[playerId];
            arg3[0] = sp58[playerId];
            arg3[1] = 0.0f;
            arg3[2] = sp48[playerId];
            break;
        case COURSE_BIG_DONUT:
            arg2[0] = sp38[playerId];
            arg2[1] = 200.0f;
            arg2[2] = sp28[playerId];
            arg3[0] = sp18[playerId];
            arg3[1] = 200.0f;
            arg3[2] = sp08[playerId];
            break;
        default:
            test = player->chasepoint;
            temp_v1 = &CenterPathBP[0][test];
            arg2[0] = temp_v1->pointx;
            arg2[1] = temp_v1->pointy;
            arg2[2] = temp_v1->pointz;
            temp_v1 = &CenterPathBP[0][(player->chasepoint + 5) % (g_courseTotalPathPoints[0] + 1)];
            arg3[0] = temp_v1->pointx;
            arg3[1] = temp_v1->pointy;
            arg3[2] = temp_v1->pointz;
            break;
    }
}

void SetLakitu(Player* player) {
    s32 playerIndex = check_kart_number(player);

    player->sterrangle = 0;
    player->nowsterr = 0;
    player->slipang = 0;
    player->jugemu_flag |= LAKITU_SCENE;
    player->slip_flag &= ~DRIFT;
    player->hangflag = 0;
    player->force = 0.0f;

    kill_strat(player, playerIndex);
    ResetStar(player, playerIndex);

    player->sus.gachon_timer = 0;
    player->jumpcount = 0;
    player->sus.gachon_firstspeed = 0.0f;
    if ((player->slip_flag & THUNDER) == THUNDER) {
        if ((player->slip_flag & THUNDER_SPIN) == THUNDER_SPIN) {
            player->slip_flag &= ~THUNDER_SPIN;
            player->slipcount = 0;
            player->nowsterr = 0;
            player->slipang = 0;
            player->direction[1] = player->old_direction;
        }
        reset_thunder(player, playerIndex);
    }
    player->slip_flag &= ~SPIN_TURN;
}

void func_80090868(Player* player) {
    s32 playerIndex;

    player->sterrangle = 0;
    player->nowsterr = 0;
    player->slipang = 0;
    player->force = 0.0f;
    playerIndex = check_kart_number(player);

    if ((player->jugemu_flag & ON_LAKITU_ROD) != ON_LAKITU_ROD) {
        player->sus.flag = 1;
        player->sus.position = 0.0f;
        player->sus.accele = 0.5f;
        make_chasepoint(player, playerIndex);
        player->hangflag = 0;
        player->jugemu_flag |= ON_LAKITU_ROD;
        player->jugemu_timer = 0;
        if ((player->water_flag & SUBMERGED) == SUBMERGED) {
            if ((g_courseID == COURSE_BOWSER_CASTLE) || (g_courseID == COURSE_BIG_DONUT)) {
                player->jugemu_flag |= LAVA_EFFECT;
            } else {
                player->jugemu_flag |= WATER_EFFECT;
            }
            // removing the water effect for Sherbet Land makes sense. Perhaps rainbow road and skyscraper
            // had lava instead of an abyss initially?
            if ((g_courseID == COURSE_SHERBET_LAND) || (g_courseID == COURSE_SKYSCRAPER) ||
                (g_courseID == COURSE_RAINBOW_ROAD)) {
                player->jugemu_flag &= ~(LAVA_EFFECT | WATER_EFFECT);
            }
        }
    }
}

void HangLakitu(Player* player, s8 playerId, s8 arg2) {
    UNUSED s32 stackPadding0;
    UNUSED s32 stackPadding1;
    Vec3f sp44;
    Vec3f sp38;
    CenterPathStruct* pathPoint;
    UNUSED s32 stackPadding2;
    UNUSED s32 stackPadding3;

    player->jumpcount = 0x000C;
    player->sterrangle = 0;
    player->nowsterr = 0;
    player->slipang = 0;
    player->force = 0.0f;
    kill_strat(player, playerId);
    switch (player->hangflag) {
        case 0:
            if ((player->jugemu_flag & IS_IN_WATER) == IS_IN_WATER) {
                if ((player->jugemu_timer < 0x3C) || ((player->jugemu_flag & ON_LAKITU_ROD) != ON_LAKITU_ROD)) {
                    player->jugemu_timer++;
                    if (player->jugemu_timer >= 0x3C) {
                        player->jugemu_timer = 0x003C;
                    }
                } else {
                    chase_Fnumber(&player->position[1], g_waterlevelPlayer[playerId] + 100.0f, 0.012f);
                    chase_Snumber(&player->roll[arg2], 0, 0.2f);
                    if ((g_waterlevelPlayer[playerId] + 40.0f) <= player->position[1]) {
                        player->hangflag = 1;
                        player->jugemu_flag |= IS_FADING_OUT;
                        player->erase = 0x00FF;
                    }
                }
            } else if ((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) {
                chase_Fnumber(&player->position[1], player->ground + 100.0f, 0.025f);
                chase_Snumber(&player->roll[arg2], 0, 0.2f);
                if ((player->ground + 40.0f) <= player->position[1]) {
                    player->hangflag = 1;
                    player->jugemu_flag |= IS_FADING_OUT;
                    player->erase = 0x00FF;
                }
            }
            if ((player->slip_flag & TERESA) == TERESA) {
                ResetVSGhost(player, playerId);
            }
            break;
        case 1:
            if (((player->flag & IS_PLAYER) == IS_PLAYER) && ((player->flag & IS_CPU_PLAYER) == 0)) {
                SetFadeOutScreen(playerId, 0xA);
            }
            if ((player->jugemu_flag & IS_IN_WATER) == IS_IN_WATER) {
                chase_Fnumber(&player->position[1], g_waterlevelPlayer[playerId] + 40.0f, 0.02f);
                player->erase -= 8;
                if (player->erase < 9) {
                    player->erase = 0;
                    player->hangflag = 2;
                    player->jugemu_flag &= ~IS_IN_WATER;
                }
            } else {
                chase_Fnumber(&player->position[1], player->old_position[1] + 40.0f, 0.02f);
                player->erase -= 8;
                if (player->erase < 9) {
                    player->erase = 0;
                    player->hangflag = 2;
                }
            }
            player->jugemu_flag &= ~WATER_EFFECT;
            break;
        case 2:
            LakituSpawnBypass(player, playerId, sp44, sp38);
            // Fakematch found by Verti, who knows what's going on here
            player->direction[1] = (u16) -CalcDirection(sp44, sp38) & 0xFFFF;
            player->position[0] = sp44[0];
            player->position[1] = sp44[1] + 40.0f;
            player->position[2] = sp44[2];
            player->hangflag = 3;
            break;
        case 3:
            g_offroadFlagPlayer1[playerId] = 0;
            if (((player->flag & IS_PLAYER) == IS_PLAYER) && ((player->flag & IS_CPU_PLAYER) == 0)) {
                SetFadeInScreen(playerId, 0x14);
            }
            LakituSpawnBypass(player, playerId, sp44, sp38);
            player->position[0] = sp44[0];
            player->position[1] = sp44[1] + 40.0f;
            player->position[2] = sp44[2];
            player->position[2] = player->position[2] + cosT((playerId * 0x1C70) - player->direction[1]) * -5.0f;
            player->position[0] = player->position[0] + sinT((playerId * 0x1C70) - player->direction[1]) * -5.0f;
            player->erase += 8;
            if (player->erase >= 0xF0) {
                player->erase = 0x00FF;
                player->hangflag = 4;
                player->jugemu_flag &= ~IS_FADING_OUT;
                player->jugemu_timer = 0;
            }
            break;
        case 4:
            if ((player->jugemu_timer == 0x0096) || (player->jugemu_timer == 0x00C8) || (player->jugemu_timer == 0x00FA)) {
                player->position[2] = player->position[2] + cosT(-player->direction[1]) * -10.0f;
                player->position[0] = player->position[0] + sinT(-player->direction[1]) * -10.0f;
            }
            if (player->jugemu_timer == 0x00FC) {
                pathPoint = CenterPathBP[0];
                player->position[0] = pathPoint->pointx;
                player->position[1] = pathPoint->pointy;
                player->position[2] = pathPoint->pointz;
            }
            chase_Fnumber(&player->position[1], (player->ground + player->radius) - 2.0f, 0.04f);
            player->jugemu_timer++;
            if (((player->slip_flag & N_JUMP) != N_JUMP) || (player->slip_flag & CARHIT)) {
                player->jugemu_flag &= ~LAVA_EFFECT;
                if (player->jugemu_timer >= 0x5B) {
                    if (player->flag & IS_PLAYER) {
                        NaPlyLevelStop(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
                    }
                    if (g_gameMode == BATTLE) {
                        cut_balloon(player, playerId);
                    }
                    player->jugemu_flag &= ~ON_LAKITU_ROD;
                    player->water_flag &= ~SPLASH_START;
                    if ((player->jugemu_flag & HAS_ICE_CUBE) != HAS_ICE_CUBE) {
                        player->jugemu_flag &= ~LAKITU_SCENE;
                        if ((player->acc_maxcount * 0.9) <= player->accelcount) {
                            set_rapidacc(player, playerId);
                        }
                    }
                }
            }
            break;
    }
    player->sus.accele += 8.0f;
    if (player->sus.accele >= 180.0f) {
        player->sus.accele = 180.0f;
    }
    if (player->sus.flag == 1) {
        player->sus.position += player->sus.accele;
        if (player->sus.position >= (f32) DEGREES(10)) {
            player->sus.accele = 0.0f;
            player->sus.flag *= -1;
        }
    }
    if (player->sus.flag == -1) {
        player->sus.position -= player->sus.accele;
        if (player->sus.position <= (f32) -DEGREES(10)) {
            player->sus.accele = 0.0f;
            player->sus.flag *= -1;
        }
    }
}

#define BLOCK_ITEM_USE_EFFECTS                                                                                        \
    THUNDER | HEIGHT_JUMP | RESTORE | BROKEN | THROW_EXPLODE |          \
        EXPLODE | STORM | DASH_JUMP | THUNDER_SPIN |      \
        ROLLOVER_FALL | RAPID_ACC | WHEELSPIN | ROLLOVER | \
        STAR | SPIN_L | SPIN_R | DASH_MIDJUMP

bool check_itembutton(Player* player) {
    s32 phi_v0 = 0;
    if ((((((player->jugemu_flag & ON_LAKITU_ROD) == ON_LAKITU_ROD) ||
           ((player->jugemu_flag & LAKITU_SCENE) == LAKITU_SCENE)) ||
          ((player->flag & IS_BOMB) != 0)) ||
         ((player->flag & IS_RACE_FINISH) != 0)) ||
        ((player->flag & EXISTS) == 0)) {
        return true;
    }

    switch (player->item) {
        case ITEM_MUSHROOM:
        case ITEM_DOUBLE_MUSHROOM:
        case ITEM_TRIPLE_MUSHROOM:
        case ITEM_SUPER_MUSHROOM:
            if ((player->slip_flag & N_JUMP) != 0) {
                return true;
            }
            phi_v0 = BLOCK_ITEM_USE_EFFECTS;
            goto prevent_item_use_label;
        case ITEM_STAR:
            phi_v0 = TERESA | BLOCK_ITEM_USE_EFFECTS;
        case ITEM_BOO:
            phi_v0 = phi_v0 | (TERESA | BLOCK_ITEM_USE_EFFECTS);
        prevent_item_use_label:
        default:
            if ((player->slip_flag & phi_v0) != 0) {
                return true;
            }
            return false;
    }
}

//UNUSED
void set_result_jump(Player* player, s8 arg1) {
    s32 temp_v0;

    player->old_direction = player->direction[1];
    player->handling_flag |= (TROPHYJUMP_STARS | TROPHYJUMP_END);
    player->handling_flag &= ~TROPHYJUMP_START;
    player->handling_flag |= TROPHYJUMP_KEEP;
    player->jmp_acc = 0.002f;
    player->jmp_speed = 0.0f;
    player->jump = 2.6f;
    player->spin_timer = 2;
    player->slipang = 0;
    player->nowsterr = 0;
    player->sterrangle = 0;
    D_8018D920[arg1] = 0;

    player->spark[1].number = 0;
    player->spark[1].timer = 0;
    player->spark[1].flag = 0;
    player->spark[0].number = 0;
    player->spark[0].timer = 0;
    player->spark[0].flag = 0;

    // clang-format off
    temp_v0 = 2; do {
        // clang-format on
        player->spark[1 + temp_v0].flag = 0;
        player->spark[1 + temp_v0].timer = 0;
        player->spark[1 + temp_v0].number = 0;
        player->spark[2 + temp_v0].flag = 0;
        player->spark[2 + temp_v0].timer = 0;
        player->spark[2 + temp_v0].number = 0;
        player->spark[3 + temp_v0].flag = 0;
        player->spark[3 + temp_v0].timer = 0;
        player->spark[3 + temp_v0].number = 0;

        temp_v0 += 4;
        player->letter[6 + temp_v0].flag = 0;
        player->letter[6 + temp_v0].timer = 0;
        player->letter[6 + temp_v0].number = 0;
    } while (temp_v0 < 10);
}

// unused
void result_jump(Player* player, s8 arg1) {
    s16 var_v1;
    UNUSED s32 stackPadding1;
    Vec3f spC = { 27.167f, 25.167f, 23.167f };

    player->handling_flag |= TROPHYJUMP_KEEP;
    if (player->spin_timer == 0) {
        var_v1 = 0;
    } else {
        player->direction[1] -= DEGREES(20);
        D_8018D920[arg1] -= DEGREES(20);
        var_v1 = (u16) D_8018D920[arg1] / DEGREES(20);
    }
    if (((var_v1 == 9) && (player->spin_timer == 1)) || ((var_v1 == 0) && (player->spin_timer == 2)) ||
        (player->spin_timer == 0)) {
        player->spin_timer--;
        if (player->spin_timer <= 0) {
            player->spin_timer = 0;
        }
        if (player->spin_timer == 0) {
            if ((player->position[1] - (player->radius + 1.0f)) <= spC[arg1]) {
                player->position[1] = (f32) ((f64) (spC[arg1] + player->radius) + 1.08);
                player->sus.bound_timer = 0;
                player->slipcount = 0;
                player->nowsterr = 0;
                player->slipang = 0;
                player->sus.bound_firstspeed = 3.0f;
                player->handling_flag &= ~TROPHYJUMP_END;
                player->gravity = gravity_1[player->kart];
                player->pitch[0] = 0;
                player->flag |= NO_CONTROLS;
                player->speed = 0.0f;
                player->force = 0.0f;
                player->accelcount = 0.0f;
                if (arg1 == 0) {
                    D_801658BC = 1;
                }
            }
        }
    }
}

void result_kart_jump_start(s8 arg0) {
    if ((gPlayers[arg0].handling_flag & TROPHYJUMP_END) == 0) {
        gPlayers[arg0].handling_flag |= (TROPHYJUMP_KEEP | TROPHYJUMP_START);
        gPlayers[arg0].flag &= ~NO_CONTROLS;
    }
}
