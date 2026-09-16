#include <ultra64.h>
#include <macros.h>
#include <mk64.h>
#include <course.h>

#include "math_util.h"
#include <common_structs.h>
#include "player_controller.h"
#include "effects.h"
#include "code_8003DC40.h"
#include "defines.h"

void norm_gravity(Player* player) {
    player->sus.jump_firstspeed = 1.0f;
}

UNUSED void add_xygravity(Player* player, Vec3f arg1) {
    s32 thing1 = 0;
    s32 thing2 = 0;
    // Praise Fake Match!!
    if (player) {};
    if (player->mid_left <= player->mid_right) {
        if (player->bump.bump_xy[1] >= 0.1736) {
            arg1[0] = (player->kartroll / DEGREES(1)) * 0x78;
            arg1[2] = -(player->kartpitch / DEGREES(1)) * 0x78;
        } else {
            arg1[0] = thing1;
            arg1[2] = thing2;
        }
    } else if (player->bump.bump_xy[1] >= 0.1736) {
        arg1[0] = (player->kartroll / DEGREES(1)) * 0x78;
        arg1[2] = -(player->kartpitch / DEGREES(1)) * 0x78;
    } else {
        arg1[0] = thing1;
        arg1[2] = thing2;
    }
    arg1[1] = 0.0f;
    MultipleMatrixByVector(arg1, player->align);
}

UNUSED void add_yzgravity(Player* player, Vec3f arg1) {
    s32 thing1 = 0;
    s32 thing2 = 0;
    // Praise Fake Match!!
    if (player) {};
    if (player->mid_left <= player->mid_right) {
        if (player->bump.bump_yz[1] >= 0.1736) {
            arg1[0] = (player->kartroll / DEGREES(1)) * 0x78;
            arg1[2] = -(player->kartpitch / DEGREES(1)) * 0x78;
        } else {
            arg1[0] = thing1;
            arg1[2] = thing2;
        }
    } else if (player->bump.bump_yz[1] >= 0.1736) {
        arg1[0] = (player->kartroll / DEGREES(1)) * 0x78;
        arg1[2] = -(player->kartpitch / DEGREES(1)) * 0x78;
    } else {
        arg1[0] = thing1;
        arg1[2] = thing2;
    }
    arg1[1] = 0.0f;
    MultipleMatrixByVector(arg1, player->align);
}

// Stick to ground?
void map_0(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * player->bump.distance_zx * 1;
    *arg6 += arg1[1] * player->bump.distance_zx * 0.1;
    *arg7 += arg1[2] * player->bump.distance_zx * 1;
    hit_velocity(arg1, *arg4, arg2, 1, 2);
    if (player->bump.bump_zx[1] <= 0.8829f) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0xB4);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(player, 4.0f);
        player->sus.jump_firstspeed = 0.5f;
        if ((player->slip_flag & TURBO) != 0) {
            reset_turbo(player);
            player->accelcount /= 2;
            player->force /= 2;
        }
    } else if ((((player->speed / 18.0f) * 216.0f) > 20.0f) ||
               ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0x32);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x3C);
    } else {
        arg3[0] = 0.0f;
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x32);
    }
    arg3[1] = 0.0f;
    MultipleMatrixByVector(arg3, player->align);
}

void map_1(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * player->bump.distance_zx * 1;
    *arg6 += arg1[1] * player->bump.distance_zx * 0.2;
    *arg7 += arg1[2] * player->bump.distance_zx * 1;
    hit_velocity(arg1, *arg4, arg2, 0.5f, 2);
    if ((player->bump.bump_zx[1] <= 0.7318f) || (player->bump_status == CLIFF)) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0xB4);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0xB4);
        if (((player->speed / 18.0f) * 216.0f) >= 8.0f) {
            AccelOff(player, 5.0f);
        }
        player->sus.jump_firstspeed = 0.5f;
        if ((player->slip_flag & TURBO) != 0) {
            reset_turbo(player);
            player->accelcount /= 2;
            player->force /= 2;
        }
    } else if ((((player->speed / 18.0f) * 216.0f) > 20.0f) ||
               ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0x32);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x32);
    } else {
        arg3[0] = 0.0f;
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x32);
    }
    arg3[1] = 0.0f;
    MultipleMatrixByVector(arg3, player->align);
}

void map_2(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * player->bump.distance_zx * 1;
    *arg6 += arg1[1] * player->bump.distance_zx * 0.1;
    *arg7 += arg1[2] * player->bump.distance_zx * 1;
    hit_velocity(arg1, *arg4, arg2, 0.5f, 2);
    if (player->bump.bump_zx[1] <= 0.8829f) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0xB4);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(player, 4.0f);
        norm_gravity(player);
    } else if ((((player->speed / 18.0f) * 216.0f) > 20.0f) ||
               ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0x32);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x3C);
    } else {
        arg3[0] = 0.0f;
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x32);
    }
    arg3[1] = 0.0f;
    MultipleMatrixByVector(arg3, player->align);
}

void map_8(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * player->bump.distance_zx * 1;
    *arg6 += arg1[1] * player->bump.distance_zx * 0.1;
    *arg7 += arg1[2] * player->bump.distance_zx * 1;
    hit_velocity(arg1, *arg4, arg2, 1.2f, 2);
    if (player->bump.bump_zx[1] <= 0.8357f) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0x78);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(player, 4.0f);
        norm_gravity(player);
    } else {
        if ((((player->speed / 18.0f) * 216.0f) > 20.0f) ||
            ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
            if ((player->tire_RL.Status == ASPHALT) ||
                (player->tire_RR.Status == ASPHALT) ||
                (player->tire_FR.Status == ASPHALT) ||
                (player->tire_FL.Status == ASPHALT)) {
                arg3[0] = ((player->kartroll / DEGREES(1)) * 5);
            } else {
                arg3[0] = ((player->kartroll / DEGREES(1)) * 0x28);
            }
            arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x28);
        } else {
            arg3[0] = 0.0f;
            arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x32);
        }
        if ((player->slip_flag & ROLLOVER_FALL) != 0) {
            arg3[0] = ((player->kartroll / DEGREES(1)) * 0x78);
            arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0xB4);
        }
    }
    arg3[1] = 0.0f;
    MultipleMatrixByVector(arg3, player->align);
}

void map_14(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * player->bump.distance_zx * 1;
    *arg6 += arg1[1] * player->bump.distance_zx * 0.1;
    *arg7 += arg1[2] * player->bump.distance_zx * 1;
    hit_velocity(arg1, *arg4, arg2, 0.5f, 2);
    if (player->bump.bump_zx[1] <= 0.8357f) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0x78);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(player, 4.0f);
        norm_gravity(player);
    } else if ((((player->speed / 18.0f) * 216.0f) > 20.0f) ||
               ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0x32);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x3C);
    } else {
        arg3[0] = 0.0f;
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x32);
    }
    arg3[1] = 0.0f;
    MultipleMatrixByVector(arg3, player->align);
}

void map_18(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * player->bump.distance_zx * 1;
    *arg6 += arg1[1] * player->bump.distance_zx * 0.1;
    *arg7 += arg1[2] * player->bump.distance_zx * 1;
    hit_velocity(arg1, *arg4, arg2, 0.5f, 2);
    if (player->bump_status == GRASS) {
        player->handling_flag &= ~REVERSE_GEAR;
    }
    if (player->bump.bump_zx[1] <= 0.8357f) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0xC8);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0xC8);
        AccelOff(player, 4.0f);
        player->sus.jump_firstspeed = 0.5f;
        arg3[0] = 0;
    } else if ((((player->speed / 18.0f) * 216.0f) > 20.0f) ||
               ((player->slip_flag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        arg3[0] = ((player->kartroll / DEGREES(1)) * 0x78);
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x78);
        arg3[0] = 0;
    } else {
        arg3[0] = 0.0f;
        arg3[2] = (-(player->kartpitch / DEGREES(1)) * 0x32);
    }
    arg3[1] = 0.0f;
    arg3[2] = 0.0f;
    MultipleMatrixByVector(arg3, player->align);
}

void CheckMapBG_ZX(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    arg1[0] = -player->bump.bump_zx[0];
    arg1[1] = -player->bump.bump_zx[1];
    arg1[2] = -player->bump.bump_zx[2];
    if ((player->bump.bump_zx[1] < 0.0f) && ((player->jugemu_flag & ON_LAKITU_ROD) == 0)) {
        *arg5 += arg1[0] * player->bump.distance_zx * 1;
        *arg6 += arg1[1] * player->bump.distance_zx * 1;
        *arg7 += arg1[2] * player->bump.distance_zx * 1;
        hit_velocity(arg1, *arg4, arg2, 1.2f, 0.0f);
        player->jmp_acc = 0.0f;
        player->jmp_speed = 0.0f;
        player->jump = 0.0f;
        return;
    } else {
#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (g_courseID) {
            case COURSE_MARIO_RACEWAY:
                map_0(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_CHOCO_MOUNTAIN:
            case COURSE_KOOPA_BEACH:
                map_1(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_BOWSER_CASTLE:
                map_2(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_LUIGI_RACEWAY:
                map_8(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_WARIO_STADIUM:
                map_14(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_DK_JUNGLE:
                map_18(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                break;
            default:
                map_0(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                break;
        }
#else

#endif
        if (player->slip_flag & ROLLOVER_FALL) {
            player->sus.jump_firstspeed = 0.5f;
        }
    }
}

void CheckMapBG_XY(Player* player, Vec3f arg1, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6) {
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f0_2;

    arg1[0] = -player->bump.bump_xy[0];
    arg1[1] = -player->bump.bump_xy[1];
    arg1[2] = -player->bump.bump_xy[2];
    if (player->bump.bump_xy[1] == 0) {
        *arg4 += arg1[0] * player->bump.distance_xy * 1;
        *arg5 += arg1[1] * player->bump.distance_xy * 0.1;
        *arg6 += arg1[2] * player->bump.distance_xy * 1;
        if ((player->kartpitch < 0) && (((player->speed / 18.0f) * 216.0f) < 10.0f)) {
            hit_velocity(arg1, *arg3, arg2, 2.5f, 0);
        } else {
            hit_velocity(arg1, *arg3, arg2, 0.5f, 0);
        }
    } else if (player->bump.bump_xy[1] <= 0.5) {
        *arg4 += arg1[0] * player->bump.distance_xy * 1;
        *arg5 += arg1[1] * player->bump.distance_xy * 0.1;
        *arg6 += arg1[2] * player->bump.distance_xy * 1;
        hit_velocity(arg1, *arg3, arg2, 1, 0);
        if ((!(player->slip_flag & ROLLOVER_FALL)) && ((player->slip_flag & N_JUMP) == 0)) {
            arg2[1] *= -1e-05;
        }
    } else {
        *arg4 += arg1[0] * player->bump.distance_xy * 1;
        temp_f0_2 = player->bump.distance_xy * arg1[1];
        if (temp_f0_2 < 0) {
            *arg5 += temp_f0_2 * 0.1;
        } else {
            *arg5 += temp_f0_2 * 0;
        }
        *arg6 += arg1[2] * player->bump.distance_xy * 1;
        hit_velocity(arg1, *arg3, arg2, 1.2f, 0);
        if ((!(player->slip_flag & ROLLOVER_FALL)) && ((player->slip_flag & N_JUMP) == 0)) {
            arg2[1] *= -1e-05;
        }
    }
    player->slip_flag &= ~DRIFT;
    temp_f12 = player->bump.distance_xy * arg1[0];
    temp_f14 = player->bump.distance_xy * arg1[2];
    if (((temp_f12 >= 0) && (temp_f14 >= 0)) || ((temp_f12 < 0) && (temp_f14 >= 0))) {
        temp_f0_2 = player->radius / 2;
        player->spark_x = *arg4 - temp_f12 - temp_f0_2;
        player->spark_z = *arg6 - temp_f14 - temp_f0_2;
    }
    if (((temp_f12 < 0) && (temp_f14 < 0)) || ((temp_f12 >= 0) && (temp_f14 < 0))) {
        temp_f0_2 = player->radius / 2;
        player->spark_x = *arg4 + temp_f12 + temp_f0_2;
        player->spark_z = *arg6 + temp_f14 + temp_f0_2;
    }
}

void CheckMapBG_YZ(Player* player, Vec3f arg1, Vec3f arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6) {
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f14;

    arg1[0] = -player->bump.bump_yz[0];
    arg1[1] = -player->bump.bump_yz[1];
    arg1[2] = -player->bump.bump_yz[2];
    if (player->bump.bump_yz[1] == 0) {
        *arg4 += arg1[0] * player->bump.distance_yz * 1;
        *arg5 += arg1[1] * player->bump.distance_yz * 0.1;
        *arg6 += arg1[2] * player->bump.distance_yz * 1;
        if ((player->kartpitch < 0) && (((player->speed / 18.0f) * 216.0f) < 10.0f)) {
            hit_velocity(arg1, *arg3, arg2, 1.5f, 0);
        } else {
            hit_velocity(arg1, *arg3, arg2, 0.5f, 0);
        }
    } else if (player->bump.bump_yz[1] <= 0.5) {
        *arg4 += arg1[0] * player->bump.distance_yz * 1;
        *arg5 += arg1[1] * player->bump.distance_yz * 0.1;
        *arg6 += arg1[2] * player->bump.distance_yz * 1;
        hit_velocity(arg1, *arg3, arg2, 1, 0);
        if ((!(player->slip_flag & ROLLOVER_FALL)) && ((player->slip_flag & N_JUMP) == 0)) {
            arg2[1] *= -1e-05;
        }
    } else {
        *arg4 += arg1[0] * player->bump.distance_yz * 1;
        temp_f0_2 = player->bump.distance_yz * arg1[1];
        if (temp_f0_2 < 0) {
            *arg5 += temp_f0_2 * 0.1;
        } else {
            *arg5 += temp_f0_2 * 0;
        }
        *arg6 += arg1[2] * player->bump.distance_yz * 1;
        hit_velocity(arg1, *arg3, arg2, 1.2f, 0);
        if ((!(player->slip_flag & ROLLOVER_FALL)) && ((player->slip_flag & N_JUMP) == 0)) {
            arg2[1] *= -1e-05;
        }
    }
    player->slip_flag &= ~DRIFT;
    temp_f12 = player->bump.distance_yz * arg1[0];
    temp_f14 = player->bump.distance_yz * arg1[2];
    if (((temp_f12 >= 0) && (temp_f14 >= 0)) || ((temp_f12 >= 0) && (temp_f14 < 0))) {
        temp_f0_2 = player->radius / 2;
        player->spark_x = *arg4 - temp_f12 - temp_f0_2;
        player->spark_z = *arg6 - temp_f14 - temp_f0_2;
    }
    if (((temp_f12 < 0) && (temp_f14 >= 0)) || ((temp_f12 < 0) && (temp_f14 < 0))) {
        temp_f0_2 = player->radius / 2;
        player->spark_x = *arg4 + temp_f12 + temp_f0_2;
        player->spark_z = *arg6 + temp_f14 + temp_f0_2;
    }
}
