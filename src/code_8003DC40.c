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

void NormGravity(Player* Player) {
    Player->sus.JumpFirstspeed = 1.0f;
}

UNUSED void AddXygravity(Player* Player, Vec3f arg1) {
    s32 Thing1 = 0;
    s32 thing2 = 0;
    // Praise Fake Match!!
    if (Player) {};
    if (Player->MidLeft <= Player->MidRight) {
        if (Player->Bump.BumpXY[1] >= 0.1736) {
            arg1[0] = (Player->Kartroll / DEGREES(1)) * 0x78;
            arg1[2] = -(Player->Kartpitch / DEGREES(1)) * 0x78;
        } else {
            arg1[0] = Thing1;
            arg1[2] = thing2;
        }
    } else if (Player->Bump.BumpXY[1] >= 0.1736) {
        arg1[0] = (Player->Kartroll / DEGREES(1)) * 0x78;
        arg1[2] = -(Player->Kartpitch / DEGREES(1)) * 0x78;
    } else {
        arg1[0] = Thing1;
        arg1[2] = thing2;
    }
    arg1[1] = 0.0f;
    MultipleMatrixByVector(arg1, Player->align);
}

UNUSED void AddYzgravity(Player* Player, Vec3f arg1) {
    s32 Thing1 = 0;
    s32 thing2 = 0;
    // Praise Fake Match!!
    if (Player) {};
    if (Player->MidLeft <= Player->MidRight) {
        if (Player->Bump.BumpYZ[1] >= 0.1736) {
            arg1[0] = (Player->Kartroll / DEGREES(1)) * 0x78;
            arg1[2] = -(Player->Kartpitch / DEGREES(1)) * 0x78;
        } else {
            arg1[0] = Thing1;
            arg1[2] = thing2;
        }
    } else if (Player->Bump.BumpYZ[1] >= 0.1736) {
        arg1[0] = (Player->Kartroll / DEGREES(1)) * 0x78;
        arg1[2] = -(Player->Kartpitch / DEGREES(1)) * 0x78;
    } else {
        arg1[0] = Thing1;
        arg1[2] = thing2;
    }
    arg1[1] = 0.0f;
    MultipleMatrixByVector(arg1, Player->align);
}

// Stick to Ground?
void Map0(Player* Player, Vec3f arg1, Vec3f Arg2, Vec3f Arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * Player->Bump.DistanceZX * 1;
    *arg6 += arg1[1] * Player->Bump.DistanceZX * 0.1;
    *arg7 += arg1[2] * Player->Bump.DistanceZX * 1;
    HitVelocity(arg1, *arg4, Arg2, 1, 2);
    if (Player->Bump.BumpZX[1] <= 0.8829f) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0xB4);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(Player, 4.0f);
        Player->sus.JumpFirstspeed = 0.5f;
        if ((Player->SlipFlag & TURBO) != 0) {
            ResetTurbo(Player);
            Player->Accelcount /= 2;
            Player->Force /= 2;
        }
    } else if ((((Player->Speed / 18.0f) * 216.0f) > 20.0f) ||
               ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x32);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x3C);
    } else {
        Arg3[0] = 0.0f;
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x32);
    }
    Arg3[1] = 0.0f;
    MultipleMatrixByVector(Arg3, Player->align);
}

void Map1(Player* Player, Vec3f arg1, Vec3f Arg2, Vec3f Arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * Player->Bump.DistanceZX * 1;
    *arg6 += arg1[1] * Player->Bump.DistanceZX * 0.2;
    *arg7 += arg1[2] * Player->Bump.DistanceZX * 1;
    HitVelocity(arg1, *arg4, Arg2, 0.5f, 2);
    if ((Player->Bump.BumpZX[1] <= 0.7318f) || (Player->BumpStatus == CLIFF)) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0xB4);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0xB4);
        if (((Player->Speed / 18.0f) * 216.0f) >= 8.0f) {
            AccelOff(Player, 5.0f);
        }
        Player->sus.JumpFirstspeed = 0.5f;
        if ((Player->SlipFlag & TURBO) != 0) {
            ResetTurbo(Player);
            Player->Accelcount /= 2;
            Player->Force /= 2;
        }
    } else if ((((Player->Speed / 18.0f) * 216.0f) > 20.0f) ||
               ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x32);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x32);
    } else {
        Arg3[0] = 0.0f;
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x32);
    }
    Arg3[1] = 0.0f;
    MultipleMatrixByVector(Arg3, Player->align);
}

void Map2(Player* Player, Vec3f arg1, Vec3f Arg2, Vec3f Arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * Player->Bump.DistanceZX * 1;
    *arg6 += arg1[1] * Player->Bump.DistanceZX * 0.1;
    *arg7 += arg1[2] * Player->Bump.DistanceZX * 1;
    HitVelocity(arg1, *arg4, Arg2, 0.5f, 2);
    if (Player->Bump.BumpZX[1] <= 0.8829f) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0xB4);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(Player, 4.0f);
        NormGravity(Player);
    } else if ((((Player->Speed / 18.0f) * 216.0f) > 20.0f) ||
               ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x32);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x3C);
    } else {
        Arg3[0] = 0.0f;
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x32);
    }
    Arg3[1] = 0.0f;
    MultipleMatrixByVector(Arg3, Player->align);
}

void Map8(Player* Player, Vec3f arg1, Vec3f Arg2, Vec3f Arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * Player->Bump.DistanceZX * 1;
    *arg6 += arg1[1] * Player->Bump.DistanceZX * 0.1;
    *arg7 += arg1[2] * Player->Bump.DistanceZX * 1;
    HitVelocity(arg1, *arg4, Arg2, 1.2f, 2);
    if (Player->Bump.BumpZX[1] <= 0.8357f) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x78);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(Player, 4.0f);
        NormGravity(Player);
    } else {
        if ((((Player->Speed / 18.0f) * 216.0f) > 20.0f) ||
            ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
            if ((Player->TireRL.Status == ASPHALT) ||
                (Player->TireRR.Status == ASPHALT) ||
                (Player->TireFR.Status == ASPHALT) ||
                (Player->TireFL.Status == ASPHALT)) {
                Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 5);
            } else {
                Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x28);
            }
            Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x28);
        } else {
            Arg3[0] = 0.0f;
            Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x32);
        }
        if ((Player->SlipFlag & ROLLOVER_FALL) != 0) {
            Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x78);
            Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0xB4);
        }
    }
    Arg3[1] = 0.0f;
    MultipleMatrixByVector(Arg3, Player->align);
}

void Map14(Player* Player, Vec3f arg1, Vec3f Arg2, Vec3f Arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * Player->Bump.DistanceZX * 1;
    *arg6 += arg1[1] * Player->Bump.DistanceZX * 0.1;
    *arg7 += arg1[2] * Player->Bump.DistanceZX * 1;
    HitVelocity(arg1, *arg4, Arg2, 0.5f, 2);
    if (Player->Bump.BumpZX[1] <= 0.8357f) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x78);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0xB4);
        AccelOff(Player, 4.0f);
        NormGravity(Player);
    } else if ((((Player->Speed / 18.0f) * 216.0f) > 20.0f) ||
               ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x32);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x3C);
    } else {
        Arg3[0] = 0.0f;
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x32);
    }
    Arg3[1] = 0.0f;
    MultipleMatrixByVector(Arg3, Player->align);
}

void Map18(Player* Player, Vec3f arg1, Vec3f Arg2, Vec3f Arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    *arg5 += arg1[0] * Player->Bump.DistanceZX * 1;
    *arg6 += arg1[1] * Player->Bump.DistanceZX * 0.1;
    *arg7 += arg1[2] * Player->Bump.DistanceZX * 1;
    HitVelocity(arg1, *arg4, Arg2, 0.5f, 2);
    if (Player->BumpStatus == GRASS) {
        Player->HandlingFlag &= ~REVERSE_GEAR;
    }
    if (Player->Bump.BumpZX[1] <= 0.8357f) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0xC8);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0xC8);
        AccelOff(Player, 4.0f);
        Player->sus.JumpFirstspeed = 0.5f;
        Arg3[0] = 0;
    } else if ((((Player->Speed / 18.0f) * 216.0f) > 20.0f) ||
               ((Player->SlipFlag & ROLLOVER_FALL) == ROLLOVER_FALL)) {
        Arg3[0] = ((Player->Kartroll / DEGREES(1)) * 0x78);
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x78);
        Arg3[0] = 0;
    } else {
        Arg3[0] = 0.0f;
        Arg3[2] = (-(Player->Kartpitch / DEGREES(1)) * 0x32);
    }
    Arg3[1] = 0.0f;
    Arg3[2] = 0.0f;
    MultipleMatrixByVector(Arg3, Player->align);
}

void CheckMapBGZX(Player* Player, Vec3f arg1, Vec3f Arg2, Vec3f Arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    arg1[0] = -Player->Bump.BumpZX[0];
    arg1[1] = -Player->Bump.BumpZX[1];
    arg1[2] = -Player->Bump.BumpZX[2];
    if ((Player->Bump.BumpZX[1] < 0.0f) && ((Player->JugemuFlag & ON_LAKITU_ROD) == 0)) {
        *arg5 += arg1[0] * Player->Bump.DistanceZX * 1;
        *arg6 += arg1[1] * Player->Bump.DistanceZX * 1;
        *arg7 += arg1[2] * Player->Bump.DistanceZX * 1;
        HitVelocity(arg1, *arg4, Arg2, 1.2f, 0.0f);
        Player->JmpAcc = 0.0f;
        Player->JmpSpeed = 0.0f;
        Player->Jump = 0.0f;
        return;
    } else {
#if !ENABLE_CUSTOM_COURSE_ENGINE
        switch (CourseID) {
            case COURSE_MARIO_RACEWAY:
                Map0(Player, arg1, Arg2, Arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_CHOCO_MOUNTAIN:
            case COURSE_KOOPA_BEACH:
                Map1(Player, arg1, Arg2, Arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_BOWSER_CASTLE:
                Map2(Player, arg1, Arg2, Arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_LUIGI_RACEWAY:
                Map8(Player, arg1, Arg2, Arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_WARIO_STADIUM:
                Map14(Player, arg1, Arg2, Arg3, arg4, arg5, arg6, arg7);
                break;
            case COURSE_DK_JUNGLE:
                Map18(Player, arg1, Arg2, Arg3, arg4, arg5, arg6, arg7);
                break;
            default:
                Map0(Player, arg1, Arg2, Arg3, arg4, arg5, arg6, arg7);
                break;
        }
#else

#endif
        if (Player->SlipFlag & ROLLOVER_FALL) {
            Player->sus.JumpFirstspeed = 0.5f;
        }
    }
}

void CheckMapBGXY(Player* Player, Vec3f arg1, Vec3f Arg2, f32* Arg3, f32* arg4, f32* arg5, f32* arg6) {
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f0_2;

    arg1[0] = -Player->Bump.BumpXY[0];
    arg1[1] = -Player->Bump.BumpXY[1];
    arg1[2] = -Player->Bump.BumpXY[2];
    if (Player->Bump.BumpXY[1] == 0) {
        *arg4 += arg1[0] * Player->Bump.DistanceXY * 1;
        *arg5 += arg1[1] * Player->Bump.DistanceXY * 0.1;
        *arg6 += arg1[2] * Player->Bump.DistanceXY * 1;
        if ((Player->Kartpitch < 0) && (((Player->Speed / 18.0f) * 216.0f) < 10.0f)) {
            HitVelocity(arg1, *Arg3, Arg2, 2.5f, 0);
        } else {
            HitVelocity(arg1, *Arg3, Arg2, 0.5f, 0);
        }
    } else if (Player->Bump.BumpXY[1] <= 0.5) {
        *arg4 += arg1[0] * Player->Bump.DistanceXY * 1;
        *arg5 += arg1[1] * Player->Bump.DistanceXY * 0.1;
        *arg6 += arg1[2] * Player->Bump.DistanceXY * 1;
        HitVelocity(arg1, *Arg3, Arg2, 1, 0);
        if ((!(Player->SlipFlag & ROLLOVER_FALL)) && ((Player->SlipFlag & N_JUMP) == 0)) {
            Arg2[1] *= -1e-05;
        }
    } else {
        *arg4 += arg1[0] * Player->Bump.DistanceXY * 1;
        temp_f0_2 = Player->Bump.DistanceXY * arg1[1];
        if (temp_f0_2 < 0) {
            *arg5 += temp_f0_2 * 0.1;
        } else {
            *arg5 += temp_f0_2 * 0;
        }
        *arg6 += arg1[2] * Player->Bump.DistanceXY * 1;
        HitVelocity(arg1, *Arg3, Arg2, 1.2f, 0);
        if ((!(Player->SlipFlag & ROLLOVER_FALL)) && ((Player->SlipFlag & N_JUMP) == 0)) {
            Arg2[1] *= -1e-05;
        }
    }
    Player->SlipFlag &= ~DRIFT;
    temp_f12 = Player->Bump.DistanceXY * arg1[0];
    temp_f14 = Player->Bump.DistanceXY * arg1[2];
    if (((temp_f12 >= 0) && (temp_f14 >= 0)) || ((temp_f12 < 0) && (temp_f14 >= 0))) {
        temp_f0_2 = Player->Radius / 2;
        Player->SparkX = *arg4 - temp_f12 - temp_f0_2;
        Player->SparkZ = *arg6 - temp_f14 - temp_f0_2;
    }
    if (((temp_f12 < 0) && (temp_f14 < 0)) || ((temp_f12 >= 0) && (temp_f14 < 0))) {
        temp_f0_2 = Player->Radius / 2;
        Player->SparkX = *arg4 + temp_f12 + temp_f0_2;
        Player->SparkZ = *arg6 + temp_f14 + temp_f0_2;
    }
}

void CheckMapBGYZ(Player* Player, Vec3f arg1, Vec3f Arg2, f32* Arg3, f32* arg4, f32* arg5, f32* arg6) {
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f14;

    arg1[0] = -Player->Bump.BumpYZ[0];
    arg1[1] = -Player->Bump.BumpYZ[1];
    arg1[2] = -Player->Bump.BumpYZ[2];
    if (Player->Bump.BumpYZ[1] == 0) {
        *arg4 += arg1[0] * Player->Bump.DistanceYZ * 1;
        *arg5 += arg1[1] * Player->Bump.DistanceYZ * 0.1;
        *arg6 += arg1[2] * Player->Bump.DistanceYZ * 1;
        if ((Player->Kartpitch < 0) && (((Player->Speed / 18.0f) * 216.0f) < 10.0f)) {
            HitVelocity(arg1, *Arg3, Arg2, 1.5f, 0);
        } else {
            HitVelocity(arg1, *Arg3, Arg2, 0.5f, 0);
        }
    } else if (Player->Bump.BumpYZ[1] <= 0.5) {
        *arg4 += arg1[0] * Player->Bump.DistanceYZ * 1;
        *arg5 += arg1[1] * Player->Bump.DistanceYZ * 0.1;
        *arg6 += arg1[2] * Player->Bump.DistanceYZ * 1;
        HitVelocity(arg1, *Arg3, Arg2, 1, 0);
        if ((!(Player->SlipFlag & ROLLOVER_FALL)) && ((Player->SlipFlag & N_JUMP) == 0)) {
            Arg2[1] *= -1e-05;
        }
    } else {
        *arg4 += arg1[0] * Player->Bump.DistanceYZ * 1;
        temp_f0_2 = Player->Bump.DistanceYZ * arg1[1];
        if (temp_f0_2 < 0) {
            *arg5 += temp_f0_2 * 0.1;
        } else {
            *arg5 += temp_f0_2 * 0;
        }
        *arg6 += arg1[2] * Player->Bump.DistanceYZ * 1;
        HitVelocity(arg1, *Arg3, Arg2, 1.2f, 0);
        if ((!(Player->SlipFlag & ROLLOVER_FALL)) && ((Player->SlipFlag & N_JUMP) == 0)) {
            Arg2[1] *= -1e-05;
        }
    }
    Player->SlipFlag &= ~DRIFT;
    temp_f12 = Player->Bump.DistanceYZ * arg1[0];
    temp_f14 = Player->Bump.DistanceYZ * arg1[2];
    if (((temp_f12 >= 0) && (temp_f14 >= 0)) || ((temp_f12 >= 0) && (temp_f14 < 0))) {
        temp_f0_2 = Player->Radius / 2;
        Player->SparkX = *arg4 - temp_f12 - temp_f0_2;
        Player->SparkZ = *arg6 - temp_f14 - temp_f0_2;
    }
    if (((temp_f12 < 0) && (temp_f14 >= 0)) || ((temp_f12 < 0) && (temp_f14 < 0))) {
        temp_f0_2 = Player->Radius / 2;
        Player->SparkX = *arg4 + temp_f12 + temp_f0_2;
        Player->SparkZ = *arg6 + temp_f14 + temp_f0_2;
    }
}
