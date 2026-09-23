#include <ultra64.h>
#include <macros.h>
#include <common_structs.h>
#include <mk64.h>
#include <course.h>

#include "code_80086E70.h"
#include "camera.h"
#include "objects.h"
#include "math_util.h"
#include "math_util_2.h"
#include "racing/memory.h"
#include "update_objects.h"
#include "collision.h"
#include "audio/external.h"
#include "main.h"
#include "code_80057C60.h"
#include "defines.h"

void KwmvStart(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Mvptr = 1; // * 0xE0)) = 1;
    KwanmClrflag(ObjectIndex, 8);
}

void KwmvStartNormal(s32 ObjectIndex, s32 arg1, s16 Arg2) {
    DynamicObjects[ObjectIndex].Spptr = arg1;
    DynamicObjects[ObjectIndex].Mvptr = Arg2;
    KwanmClrflag(ObjectIndex, 8);
}

void KwmvStartStrategy(s32 ObjectIndex) {
    KwmvStart(ObjectIndex);
}

void KwmvStartSpline(s32 ObjectIndex, s32 arg1, SplineData* spline) {
    KwmvStart(ObjectIndex);
    DynamicObjects[ObjectIndex].Mappoint = arg1;
    DynamicObjects[ObjectIndex].Sptbtopaddr = spline;
}

void KwmvFinish(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Mvptr = 0;
    DynamicObjects[ObjectIndex].Spptr = 0;
    DynamicObjects[ObjectIndex].Mappoint = 0;
    KwanmClrflag(ObjectIndex, 8);
}

bool KwmvCheckfinish(s32 ObjectIndex) {
    bool ret = false;
    if (DynamicObjects[ObjectIndex].Mvptr == 0) {
        ret = true;
    }
    return ret;
}

void KWAnmNext(s32 ObjectIndex) {
    KwanmClrflag(ObjectIndex, 8);
    DynamicObjects[ObjectIndex].Mvptr += 1;
}

void KwmvJump(s32 ObjectIndex, s32 arg1) {
    KwanmClrflag(ObjectIndex, 8);
    DynamicObjects[ObjectIndex].Mvptr = arg1;
}

s32 KwmvWait(s32 ObjectIndex, s32 arg1) {
    s32 sp1C;

    sp1C = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].Mvtm = arg1;
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        sp1C = 1;
    }
    return sp1C;
}

s32 KwmvRandomwait(s32 ObjectIndex, u16 arg1) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].Mvtm = MakeRandomLimmit(arg1);
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        sp24 = 1;
    }
    return sp24;
}

s32 KwmvWaitnext(s32 ObjectIndex, s32 arg1) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].Mvtm = (s16) arg1;
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        KWAnmNext(ObjectIndex);
        sp24 = 1;
    }
    return sp24;
}

UNUSED void KwmvAddfvarCnt(s32 ObjectIndex, UNUSED s32 arg1, f32 Arg2) {
    DynamicObjects[ObjectIndex].PosSpline[1] += Arg2;
    DynamicObjects[ObjectIndex].PosSpline[2] -= Arg2;
}

UNUSED s32 KwlookplayerXz(s32 ObjectIndex, Player* Player) {
    return KwcalcDestangley(Player->Position[0], DynamicObjects[ObjectIndex].pos[0], Player->Position[2],
                                DynamicObjects[ObjectIndex].pos[2]);
}

s32 KwlookcameraXz(s32 ObjectIndex, Camera* Camera) {
    return Atan2t(DynamicObjects[ObjectIndex].pos[0] - Camera->CameraPos[0], DynamicObjects[ObjectIndex].pos[2] - Camera->CameraPos[2]);
}

u16 KwmvVelyztoangx(s32 ObjectIndex) {
    return -Atan2t(DynamicObjects[ObjectIndex].Velocity[1], DynamicObjects[ObjectIndex].Velocity[2]);
}

s32 KwmvVelxztoangy(s32 ObjectIndex) {
    return Atan2t(DynamicObjects[ObjectIndex].Velocity[0], DynamicObjects[ObjectIndex].Velocity[2]);
}

UNUSED void KwchaseVelyztoangx(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].ang[0] =
        KwchaseAngle(DynamicObjects[ObjectIndex].ang[0], KwmvVelyztoangx(ObjectIndex));
}

void KwchaseVelxztoangy(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].ang[1] =
        KwchaseAngle(DynamicObjects[ObjectIndex].ang[1], KwmvVelxztoangy(ObjectIndex));
}

UNUSED void KwmvVx(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Velocity[0] =
        DynamicObjects[ObjectIndex].vel * SinT(DynamicObjects[ObjectIndex].ang[1]);
}

UNUSED void KwmvVy(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Velocity[1] =
        DynamicObjects[ObjectIndex].vel * CosT(DynamicObjects[ObjectIndex].ang[0]);
}

UNUSED void KwmvVz(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Velocity[2] =
        DynamicObjects[ObjectIndex].vel * CosT(DynamicObjects[ObjectIndex].ang[1]);
}

void KwmvVxz(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Velocity[0] =
        DynamicObjects[ObjectIndex].vel * SinT(DynamicObjects[ObjectIndex].ang[1]);
    DynamicObjects[ObjectIndex].Velocity[2] =
        DynamicObjects[ObjectIndex].vel * CosT(DynamicObjects[ObjectIndex].ang[1]);
}

void KwmvVxyz(s32 ObjectIndex) {
    f32 sp24;

    sp24 = CosT(DynamicObjects[ObjectIndex].ang[0]);
    DynamicObjects[ObjectIndex].Velocity[0] =
        (DynamicObjects[ObjectIndex].vel * sp24) * SinT(DynamicObjects[ObjectIndex].ang[1]);
    DynamicObjects[ObjectIndex].Velocity[1] =
        -DynamicObjects[ObjectIndex].vel * SinT(DynamicObjects[ObjectIndex].ang[0]);
    sp24 = CosT(DynamicObjects[ObjectIndex].ang[0]);
    DynamicObjects[ObjectIndex].Velocity[2] =
        (DynamicObjects[ObjectIndex].vel * sp24) * CosT(DynamicObjects[ObjectIndex].ang[1]);
}

void KwmvVxzReverse(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Velocity[0] =
        DynamicObjects[ObjectIndex].vel * SinT(DynamicObjects[ObjectIndex].ang[1] + 0x8000);
    DynamicObjects[ObjectIndex].Velocity[2] =
        DynamicObjects[ObjectIndex].vel * CosT(DynamicObjects[ObjectIndex].ang[1] + 0x8000);
}

void KwmvVelangytopos(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].PosSpline[0] +=
        DynamicObjects[ObjectIndex].vel * SinT(DynamicObjects[ObjectIndex].ang[1]);
    DynamicObjects[ObjectIndex].PosSpline[2] +=
        DynamicObjects[ObjectIndex].vel * CosT(DynamicObjects[ObjectIndex].ang[1]);
}

void KwmvAddveltopos(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].PosSpline[0] += DynamicObjects[ObjectIndex].Velocity[0];
    DynamicObjects[ObjectIndex].PosSpline[1] += DynamicObjects[ObjectIndex].Velocity[1];
    DynamicObjects[ObjectIndex].PosSpline[2] += DynamicObjects[ObjectIndex].Velocity[2];
}

void KwmvAddvelxztopos(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].PosSpline[0] += DynamicObjects[ObjectIndex].Velocity[0];
    DynamicObjects[ObjectIndex].PosSpline[2] += DynamicObjects[ObjectIndex].Velocity[2];
}

UNUSED void KwmvAddvelxtopos(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].PosSpline[0] += DynamicObjects[ObjectIndex].Velocity[0];
}

void KwmvAddvelytopos(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].PosSpline[1] += DynamicObjects[ObjectIndex].Velocity[1];
}

UNUSED void KwmvAddvelztopos(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].PosSpline[2] += DynamicObjects[ObjectIndex].Velocity[2];
}

void KwmvVelangytoposxz(s32 arg0) {
    KwmvVxz(arg0);
    KwmvAddvelxztopos(arg0);
}

void KwmvVelangxytoposxyz(s32 arg0) {
    KwmvVxyz(arg0);
    KwmvAddveltopos(arg0);
}

f32 KwmoveStraight(f32 arg0, f32 arg1, f32 Arg2, f32 Arg3, f32 arg4) {
    return (((arg4 - Arg2) / (Arg3 - arg1)) * (arg0 - arg1)) + Arg2;
}

s32 KwmvWalkxz(s32 ObjectIndex, s32 arg1) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        KwmvVxz(ObjectIndex);
        DynamicObjects[ObjectIndex].Mvtm = arg1;
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        sp24 = 1;
    } else {
        KwmvAddvelxztopos(ObjectIndex);
    }
    return sp24;
}

s32 KwmvWalkxzReverse(s32 ObjectIndex, s32 arg1) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        KwmvVxzReverse(ObjectIndex);
        DynamicObjects[ObjectIndex].Mvtm = arg1;
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        sp24 = 1;
    } else {
        KwmvAddvelxztopos(ObjectIndex);
    }
    return sp24;
}

bool KwmvWalkxz2Pt(s32 ObjectIndex, s16 arg1, s16 Arg2, s16 Arg3, s16 arg4) {
    s16 dist;
    s16 temp_a0;
    s16 temp_v0;
    bool sp2C;

    sp2C = false;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        temp_v0 = Arg2 - arg1;
        temp_a0 = arg4 - Arg3;
        dist = sqrtf((temp_v0 * temp_v0) + (temp_a0 * temp_a0));
        DynamicObjects[ObjectIndex].PosStart[1] = 0.0f;
        DynamicObjects[ObjectIndex].ang[1] = Atan2t(temp_v0, temp_a0);
        KwmvVxz(ObjectIndex);
        DynamicObjects[ObjectIndex].Mvtm = dist / DynamicObjects[ObjectIndex].vel;
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        sp2C = true;
    } else {
        KwmvAddvelxztopos(ObjectIndex);
    }
    return sp2C;
}

s32 KwmvTrakkaGround(s32 ObjectIndex, f32 arg1, f32 Arg2) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].Velocity[1] = -arg1;
    }
    KwmvAddvelytopos(ObjectIndex);
    if (DynamicObjects[ObjectIndex].pos[1] <= Arg2) {
        KwanmClrflag(ObjectIndex, 8);
        KWAnmNext(ObjectIndex);
        sp24 = 1;
    }
    return sp24;
}

s32 KwmvNageage(s32 ObjectIndex, f32 arg1, f32 Arg2, s32 Arg3) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != false) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].Velocity[1] = arg1;
        DynamicObjects[ObjectIndex].Mvtm = (s16) Arg3;
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        KWAnmNext(ObjectIndex);
        sp24 = 1;
    } else {
        DynamicObjects[ObjectIndex].Velocity[1] -= Arg2;
        KwmvAddvelytopos(ObjectIndex);
    }
    return sp24;
}

s32 KwmvNageageGround(s32 ObjectIndex, f32 arg1, f32 Arg2, f32 Arg3) {
    s32 sp24;

    sp24 = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != 0) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].Velocity[1] = arg1;
    }
    DynamicObjects[ObjectIndex].Velocity[1] -= Arg2;
    KwmvAddvelytopos(ObjectIndex);
    if (DynamicObjects[ObjectIndex].PosSpline[1] <= Arg3) {
        KwanmClrflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].PosSpline[1] = Arg3;
        KWAnmNext(ObjectIndex);
        sp24 = 1;
    }
    return sp24;
}

bool KwmvNanamenage(s32 ObjectIndex, f32 arg1, f32 Arg2, f32 Arg3, s16 arg4, s32 arg5) {
    bool sp2C;
    UNUSED s32 pad;

    sp2C = false;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != 0) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].PosSpline[2] = 0.0f;
        DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
        DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
        DynamicObjects[ObjectIndex].vel = Arg3;
        DynamicObjects[ObjectIndex].Velocity[1] = arg1;
        DynamicObjects[ObjectIndex].ang[1] = arg4;
        KwmvVxz(ObjectIndex);
        DynamicObjects[ObjectIndex].Mvtm = arg5;
    }
    DynamicObjects[ObjectIndex].Mvtm--;
    if (DynamicObjects[ObjectIndex].Mvtm < 0) {
        KwanmClrflag(ObjectIndex, 8);
        KWAnmNext(ObjectIndex);
        sp2C = true;
    } else {
        DynamicObjects[ObjectIndex].Velocity[1] -= Arg2;
        KwmvAddveltopos(ObjectIndex);
    }
    return sp2C;
}

UNUSED s32 KwmvNanamenageGround(s32 ObjectIndex, f32 arg1, f32 Arg2, f32 Arg3, s16 arg4, s32 arg5) {
    s32 sp2C;
    UNUSED s32 stackPadding;

    sp2C = 0;
    if (KwanmBitoffcheckflag(ObjectIndex, 8) != 0) {
        KwanmSetflag(ObjectIndex, 8);
        DynamicObjects[ObjectIndex].PosSpline[2] = 0.0f;
        DynamicObjects[ObjectIndex].PosSpline[1] = 0.0f;
        DynamicObjects[ObjectIndex].PosSpline[0] = 0.0f;
        DynamicObjects[ObjectIndex].vel = Arg3;
        DynamicObjects[ObjectIndex].Velocity[1] = arg1;
        DynamicObjects[ObjectIndex].ang[1] = arg4;
        KwmvVxz(ObjectIndex);
        DynamicObjects[ObjectIndex].Mvtm = Timer;
    }
    if (DynamicObjects[ObjectIndex].PosSpline[1] <= arg5) {
        KwanmClrflag(ObjectIndex, 8);
        KWAnmNext(ObjectIndex);
        sp2C = 1;
    } else {
        DynamicObjects[ObjectIndex].Velocity[1] -= Arg2;
        KwmvAddveltopos(ObjectIndex);
    }
    return sp2C;
}

void KwmvCircle(s32 ObjectIndex, f32 arg1, u16 Arg2) {
    f32 temp_f4;
    f32 sp20;

    temp_f4 = DynamicObjects[ObjectIndex].PosSpline[0];
    sp20 = DynamicObjects[ObjectIndex].PosSpline[2];
    DynamicObjects[ObjectIndex].Addangy += Arg2;
    DynamicObjects[ObjectIndex].PosSpline[0] = SinT(DynamicObjects[ObjectIndex].Addangy) * arg1;
    DynamicObjects[ObjectIndex].PosSpline[2] = CosT(DynamicObjects[ObjectIndex].Addangy) * arg1;
    DynamicObjects[ObjectIndex].Velocity[0] = DynamicObjects[ObjectIndex].PosSpline[0] - temp_f4;
    DynamicObjects[ObjectIndex].Velocity[2] = DynamicObjects[ObjectIndex].PosSpline[2] - sp20;
}

UNUSED void KwchaseCircle(void) {
}

void KwmvSetchasepoint(s32 ObjectIndex) {
    SplineControlPoint* phi_v0;
    s32 someIndex;
    phi_v0 = DynamicObjects[ObjectIndex].Sptbaddr;
    for (someIndex = 0; someIndex < 2; someIndex++, phi_v0++) {
        D_80165760[someIndex] = phi_v0->pos[0];
        D_80165770[someIndex] = phi_v0->pos[1];
        D_80165780[someIndex] = phi_v0->pos[2];
    }
}

void KwmvNextchasepoint(s32 arg0) {
    DynamicObjects[arg0].Sptbaddr++;
}

void KwmvSetdirection(s32 ObjectIndex, s32 arg1) {
    s16 temp_a1;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    temp_a1 = Atan2t(D_80165760[1] - D_80165760[0], D_80165780[1] - D_80165780[0]);
    temp_a1 -= object->ang[1];
    if (temp_a1 > 0) {
        object->ang[1] += (arg1 << 8);
    } else if (temp_a1 < 0) {
        object->ang[1] -= (arg1 << 8);
    }
}

// if the Code is too ugly for you, then go fix it without tampering with the register allocations
void KwmvChase3DInit(s32 ObjectIndex) {
    AnmObject* object;
    object = &DynamicObjects[ObjectIndex];
    object->Stack[9] = 0;
    object->Sptbaddr = object->Sptbtopaddr->ControlPoints;
    /*
    This is INCREDIBLY stupid. This should really be
    temp_v0->Stack[8] = temp_v0->Sptbtopaddr->NumControlPoints;
    but for some unholy reason that doesn't match
    */
    object->Stack[8] = *((s16*) object->Sptbaddr - 1);
    object->PosSpline[0] = object->Sptbaddr[0].pos[0];
    object->PosSpline[1] = object->Sptbaddr[0].pos[1];
    object->PosSpline[2] = object->Sptbaddr[0].pos[2];
    KWAnmNext(ObjectIndex);
}

s32 KwmvCheckcollision(s32 ObjectIndex, s32 arg1) {
    s32 temp_f0;
    s32 temp_f12;
    s32 temp_f2;
    s32 var_v1;

    var_v1 = 0;
    temp_f0 = (s32) ((D_80165760[1] - DynamicObjects[ObjectIndex].PosSpline[0]) *
                     (D_80165760[1] - DynamicObjects[ObjectIndex].PosSpline[0]));
    temp_f12 = (s32) ((D_80165770[1] - DynamicObjects[ObjectIndex].PosSpline[1]) *
                      (D_80165770[1] - DynamicObjects[ObjectIndex].PosSpline[1]));
    temp_f2 = (s32) ((D_80165780[1] - DynamicObjects[ObjectIndex].PosSpline[2]) *
                     (D_80165780[1] - DynamicObjects[ObjectIndex].PosSpline[2]));
    if ((temp_f0 + temp_f12 + temp_f2 - (arg1 * arg1)) <= 0) {
        var_v1 = 1;
    }
    return var_v1;
}

void KwmvChase3D(s32 ObjectIndex) {
    AnmObject* object;

    KwmvSetchasepoint(ObjectIndex);
    KwmvSetdirection(ObjectIndex, 1);
    KwmvVelangytopos(ObjectIndex);
    if (KwmvCheckcollision(ObjectIndex, 0x0000000A) != 0) {
        object = &DynamicObjects[ObjectIndex];
        object->Stack[9] = (u16) object->Stack[9] + 1;
        if (((u16) object->Stack[9] + 3) == (u16) object->Stack[8]) {
            object->Mvptr += 1;
        } else {
            KwmvNextchasepoint(ObjectIndex);
        }
    }
}

void KwchaseStrategy3D(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            KwmvChase3DInit(ObjectIndex);
            break;
        case 2:
            KwmvChase3D(ObjectIndex);
            break;
        case 3:
            KwmvFinish(ObjectIndex);
            break;
    }
}

s32 KwanmCheckGround(s32 ObjectIndex) {
    s32 sp2C;

    sp2C = 0;
    KwanmClrflag(ObjectIndex, 0x00800000);
    if (KwanmBitcheckflag(ObjectIndex, 0x00000400) != 0) {
        CheckBump(&D_8018C3B0, 10.0f, DynamicObjects[ObjectIndex].pos[0], 20.0f,
                                 DynamicObjects[ObjectIndex].pos[2]);
        if (D_8018C3B0.FlagZX == 1) {
            sp2C = 1;
            KwanmSetflag(ObjectIndex, 0x00800000);
        }
        DynamicObjects[ObjectIndex].Ground = CalcHeight(
            DynamicObjects[ObjectIndex].pos[0], 0.0f, DynamicObjects[ObjectIndex].pos[2], D_8018C3B0.LastZX);
    }
    return sp2C;
}

s32 KwanmSetonGround(s32 ObjectIndex) {
    s32 sp2C;

    sp2C = 0;
    KwanmClrflag(ObjectIndex, 0x00800000);
    if (KwanmBitcheckflag(ObjectIndex, 0x00000400) != 0) {
        CheckBump(&D_8018C3B0, 10.0f, DynamicObjects[ObjectIndex].pos[0], 20.0f,
                                 DynamicObjects[ObjectIndex].pos[2]);
        if (D_8018C3B0.FlagZX == 1) {
            sp2C = 1;
            KwanmSetflag(ObjectIndex, 0x00800000);
        }
        DynamicObjects[ObjectIndex].Ground = CalcHeight(
            DynamicObjects[ObjectIndex].pos[0], 0.0f, DynamicObjects[ObjectIndex].pos[2], D_8018C3B0.LastZX);
        DynamicObjects[ObjectIndex].Kageang[0] =
            Atan2t(D_8018C3B0.BumpZX[2], D_8018C3B0.BumpZX[1]) + 0x4000;
        DynamicObjects[ObjectIndex].Kageang[2] = Atan2t(D_8018C3B0.BumpZX[0], D_8018C3B0.BumpZX[1]);
    }
    return sp2C;
}

s32 KwanmSetonGroundbump(s32 ObjectIndex) {
    s32 sp2C;

    sp2C = 0;
    KwanmClrflag(ObjectIndex, 0x00800000);
    if (KwanmBitcheckflag(ObjectIndex, 0x00000400) != 0) {
        CheckBump(&D_8018C3B0, 10.0f, DynamicObjects[ObjectIndex].pos[0], 20.0f,
                                 DynamicObjects[ObjectIndex].pos[2]);
        if (D_8018C3B0.FlagZX == 1) {
            sp2C = 1;
            KwanmSetflag(ObjectIndex, 0x00800000);
        }
        DynamicObjects[ObjectIndex].Ground = CalcHeight(
            DynamicObjects[ObjectIndex].pos[0], 0.0f, DynamicObjects[ObjectIndex].pos[2], D_8018C3B0.LastZX);
        DynamicObjects[ObjectIndex].PosEnde[0] = D_8018C3B0.BumpZX[0];
        DynamicObjects[ObjectIndex].PosEnde[1] = D_8018C3B0.BumpZX[1];
        DynamicObjects[ObjectIndex].PosEnde[2] = D_8018C3B0.BumpZX[2];
    }
    return sp2C;
}

void KwanmSetkageParams(s32 ObjectIndex) {
    CheckBump(&D_8018C3B0, 10.0f, DynamicObjects[ObjectIndex].pos[0], 20.0f,
                             DynamicObjects[ObjectIndex].pos[2]);
    if (D_8018C3B0.FlagZX == 1) {
        KwanmSetflag(ObjectIndex, 0x00800000);
        DynamicObjects[ObjectIndex].Ground = CalcHeight(
            DynamicObjects[ObjectIndex].pos[0], 0.0f, DynamicObjects[ObjectIndex].pos[2], D_8018C3B0.LastZX);
        DynamicObjects[ObjectIndex].Kageang[0] =
            Atan2t(D_8018C3B0.BumpZX[2], D_8018C3B0.BumpZX[1]) + 0x4000;
        DynamicObjects[ObjectIndex].Kageang[2] = Atan2t(D_8018C3B0.BumpZX[0], D_8018C3B0.BumpZX[1]);
        return;
    }
    KwanmClrflag(ObjectIndex, 0x00800000);
}

void KwanmSetholeParams(s32 ObjectIndex) {
    CheckBump(&D_8018C3B0, 10.0f, DynamicObjects[ObjectIndex].pos[0], 20.0f,
                             DynamicObjects[ObjectIndex].pos[2]);
    if (D_8018C3B0.FlagZX == 1) {
        KwanmSetflag(ObjectIndex, 0x00800000);
        DynamicObjects[ObjectIndex].Ground = CalcHeight(
            DynamicObjects[ObjectIndex].pos[0], 0.0f, DynamicObjects[ObjectIndex].pos[2], D_8018C3B0.LastZX);
        DynamicObjects[ObjectIndex].Velocity[0] = D_8018C3B0.BumpZX[0];
        DynamicObjects[ObjectIndex].Velocity[1] = D_8018C3B0.BumpZX[1];
        DynamicObjects[ObjectIndex].Velocity[2] = D_8018C3B0.BumpZX[2];
        return;
    }
    KwanmClrflag(ObjectIndex, 0x00800000);
}

/**
 * @brief Returns the Distance between the object and the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @return UNUSED
 */
UNUSED s32 KwanmCalcDistanceXz(s32 ObjectIndex, Player* Player) {
    s32 x;
    s32 y;

    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    y = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    return (x * x) + (y * y);
}

/**
 * @brief Returns the Distance between the object and the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @return UNUSED
 */
UNUSED s32 KwanmCalcDistanceXYZ(s32 ObjectIndex, Player* Player) {
    s32 x;
    s32 z;
    s32 y;

    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    y = DynamicObjects[ObjectIndex].pos[1] - Player->Position[1];
    z = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    return (x * x) + (y * y) + (z * z);
}

/**
 * @brief Returns the Distance between the object and the Camera.
 *
 * @Param ObjectIndex
 * @Param Camera
 * @return UNUSED
 */
u32 KwanmCalcDistanceCameraXz(s32 ObjectIndex, Camera* Camera) {
    s32 x;
    s32 y;

    x = DynamicObjects[ObjectIndex].pos[0] - Camera->CameraPos[0];
    y = DynamicObjects[ObjectIndex].pos[2] - Camera->CameraPos[2];
    return (x * x) + (y * y);
}

/**
 * @brief Returns the Distance between the object and the Camera.
 *
 * @Param ObjectIndex
 * @Param Camera
 * @return UNUSED
 */
UNUSED s32 KwanmCalcDistanceCameraXYZ(s32 ObjectIndex, Camera* Camera) {
    s32 x;
    s32 z;
    s32 y;

    x = DynamicObjects[ObjectIndex].pos[0] - Camera->CameraPos[0];
    y = DynamicObjects[ObjectIndex].pos[1] - Camera->CameraPos[1];
    z = DynamicObjects[ObjectIndex].pos[2] - Camera->CameraPos[2];
    return (x * x) + (y * y) + (z * z);
}

/**
 * @brief Returns if the object is closer than the Distance to the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @Param Distance
 * @return true
 * @return false
 */
bool KwanmCheckradiusPlayerXz(s32 ObjectIndex, Player* Player, f32 Distance) {
    f32 x;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    y = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    if (((x * x) + (y * y)) <= (Distance * Distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is between the distance_min and distance_max to the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @Param distance_min
 * @Param distance_max
 * @return true
 * @return false
 */
bool KwanmCheckradius2PlayerXz(s32 ObjectIndex, Player* Player, f32 distance_min, f32 distance_max) {
    f32 x;
    f32 Distance;
    f32 z;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    z = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    Distance = (x * x) + (z * z);
    if (((distance_min * distance_min) <= Distance) && (Distance <= (distance_max * distance_max))) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is closer than the Distance to the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @Param Distance
 * @return true
 * @return false
 */
bool KwanmCheckradiusPlayerXYZ(s32 ObjectIndex, Player* Player, f32 Distance) {
    f32 x;
    f32 z;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    y = DynamicObjects[ObjectIndex].pos[1] - Player->Position[1];
    z = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    if (((x * x) + (y * y) + (z * z)) <= (Distance * Distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is between the distance_min and distance_max to the Camera.
 *
 * @Param ObjectIndex
 * @Param Camera
 * @Param Distance
 * @return true
 * @return false
 */
bool KwanmCheckradiusCameraXz(s32 ObjectIndex, Camera* Camera, f32 Distance) {
    f32 x;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Camera->CameraPos[0];
    y = DynamicObjects[ObjectIndex].pos[2] - Camera->CameraPos[2];
    if (((x * x) + (y * y)) <= (Distance * Distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is between the distance_min and distance_max to the Camera.
 *
 * @Param ObjectIndex
 * @Param Camera
 * @Param distance_min
 * @Param distance_max
 * @return UNUSED
 */
UNUSED bool KwanmCheckradius2CameraXz(s32 ObjectIndex, Camera* Camera, f32 distance_min, f32 distance_max) {
    f32 x;
    f32 Distance;
    f32 z;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Camera->CameraPos[0];
    z = DynamicObjects[ObjectIndex].pos[2] - Camera->CameraPos[2];
    Distance = (x * x) + (z * z);
    if (((distance_min * distance_min) <= Distance) && (Distance <= (distance_max * distance_max))) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is closer than the Distance to the Camera.
 *
 * @Param ObjectIndex
 * @Param Camera
 * @Param Distance
 * @return UNUSED
 */
UNUSED bool KwanmCheckradiusCameraXYZ(s32 ObjectIndex, Camera* Camera, f32 Distance) {
    f32 x;
    f32 z;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Camera->CameraPos[0];
    y = DynamicObjects[ObjectIndex].pos[1] - Camera->CameraPos[1];
    z = DynamicObjects[ObjectIndex].pos[2] - Camera->CameraPos[2];
    if (((x * x) + (y * y) + (z * z)) <= (Distance * Distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @return true
 * @return false
 */
bool KwanmCheckcollPlayerXz(s32 ObjectIndex, Player* Player) {
    f32 x;
    f32 Distance;
    f32 z;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    z = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    Distance = DynamicObjects[ObjectIndex].r + Player->Radius;
    if (((x * x) + (z * z)) <= (Distance * Distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @return true
 * @return false
 */
bool KwanmCheckcollPlayerXYZ(s32 ObjectIndex, Player* Player) {
    f32 x;
    f32 z;
    f32 Distance;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    y = DynamicObjects[ObjectIndex].pos[1] - Player->Position[1];
    z = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    Distance = DynamicObjects[ObjectIndex].r + Player->Radius;
    if (((x * x) + (y * y) + (z * z)) <= (Distance * Distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the Player.
 *
 * @Param ObjectIndex
 * @Param Player
 * @Param Distance
 * @return UNUSED
 */
UNUSED bool KwanmCheckhightPlayer(s32 ObjectIndex, Player* Player, f32 Distance) {
    f32 x;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[1] - Player->Position[1];
    if ((x * x) <= (Distance * Distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the Player and is closer than the Distance in the y axis.
 *
 * @Param ObjectIndex
 * @Param Player
 * @Param distance_y
 * @return true
 * @return false
 */
bool KwanmCheckcollPillarPlayer(s32 ObjectIndex, Player* Player, f32 distance_y) {
    f32 x;
    f32 z;
    f32 Distance;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    y = DynamicObjects[ObjectIndex].pos[1] - Player->Position[1];
    z = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    Distance = DynamicObjects[ObjectIndex].r + Player->Radius;
    // abs(y)
    if (y < 0.0f) {
        y = -y;
    }
    if ((((x * x) + (z * z)) <= (Distance * Distance)) && (y <= distance_y)) {
        var_v1 = true;
    }
    return var_v1;
}

f32 KwanmSetheightGround(s32 ObjectIndex, Player* Player) {
    f32 Distance;

    Distance = DynamicObjects[ObjectIndex].pos[1] - Player->Ground;
    // abs(Distance)
    if (Distance < 0.0f) {
        Distance = -Distance;
    }
    return Distance;
}

UNUSED bool KwanmCheckheightGround(s32 ObjectIndex, Player* Player, f32 Arg2) {
    f32 Distance;
    bool var_v1;

    Distance = DynamicObjects[ObjectIndex].pos[1] - Player->Ground;
    var_v1 = false;
    // abs(Distance)
    if (Distance < 0.0f) {
        Distance = -Distance;
    }
    if (Distance <= Arg2) {
        var_v1 = true;
    }
    return var_v1;
}

void KwstopKart(Player* Player) {
    Player->Force = 0.0f;
    Player->Accelcount = 0.0f;
    Player->Velocity[0] = 0.0f;
    Player->Velocity[2] = 0.0f;
}

UNUSED void KwclearAccelKart(Player* Player) {
    Player->Force = 0.0f;
    Player->Accelcount = 0.0f;
}

void KwchaseBoundvelSub(s32 PlayerId, f32* arg1) {
    f32 var_f0;
    f32 var_f2;
    Player* Player = &PlayerOne[PlayerId];

    if (*arg1 >= 0.0f) {
        var_f2 = *arg1;
    } else {
        var_f2 = -*arg1;
    }
    if (Player->SlipFlag & (SPIN_L | SPIN_R)) {
        if (CourseID == COURSE_SHERBET_LAND) {
            if (var_f2 <= 0.5) {
                var_f0 = 0.025f;
            } else if (var_f2 <= 2.0) {
                var_f0 = 0.075f;
            } else if (var_f2 <= 4.0) {
                var_f0 = 0.15f;
            } else {
                var_f0 = 0.25f;
            }
        } else {
            if (var_f2 <= 2.0) {
                var_f0 = 0.1f;
            } else if (var_f2 <= 3.0) {
                var_f0 = 0.15f;
            } else if (var_f2 <= 4.0) {
                var_f0 = 0.2f;
            } else {
                var_f0 = 0.25f;
            }
        }
    } else if (CourseID == COURSE_SHERBET_LAND) {
        if (var_f2 <= 0.5) {
            var_f0 = 0.025f;
        } else if (var_f2 <= 2.0) {
            var_f0 = 0.075f;
        } else if (var_f2 <= 4.0) {
            var_f0 = 0.1f;
        } else {
            var_f0 = 0.15f;
        }
    } else {
        if (var_f2 <= 2.0) {
            var_f0 = 0.06f;
        } else if (var_f2 <= 3.0) {
            var_f0 = 0.07f;
        } else if (var_f2 <= 4.0) {
            var_f0 = 0.075f;
        } else {
            var_f0 = 0.1f;
        }
    }
    KWChaseFVal(arg1, 0.0f, var_f0);
}

void KwchaseBoundvelZero(s32 PlayerId) {
    KwchaseBoundvelSub(PlayerId, &D_8018CE10[PlayerId].unk_04[0]);
    KwchaseBoundvelSub(PlayerId, &D_8018CE10[PlayerId].unk_04[2]);
    if (D_8018CE10[PlayerId].unk_18[6] > 0) {
        D_8018CE10[PlayerId].unk_18[6]--;
    }
}

f32 KwreboundKartplas(Player* Player, s32 ObjectIndex, f32 Arg2, f32 Arg3) {
    f32 var_f2;
    f32 something;
    s32 PlayerId;
    AnmObject* object;
    struct_D_8018CE10* temp_v1;

    PlayerId = Player - PlayerOne;
    temp_v1 = &D_8018CE10[PlayerId];
    var_f2 = 0.0f;
    if (temp_v1->unk_18[6] == 0) {
        object = &DynamicObjects[ObjectIndex];
        Player->HitFlag |= CRITTER_TOUCH;
        Player->SlipFlag |= CARHIT;
        temp_v1->unk_18[6] = 4;
        something = (Player->Position[0] - object->pos[0]) * object->Velocity[0];
        if (something >= 0.0f) {
            temp_v1->unk_04[0] = (-Player->Velocity[0] * Arg2) + (object->Velocity[0] * Arg3);
        } else {
            temp_v1->unk_04[0] = -Player->Velocity[0] * Arg2;
        }
        something = (Player->Position[2] - object->pos[2]) * object->Velocity[2];
        if (something >= 0.0f) {
            temp_v1->unk_04[2] = (-Player->Velocity[2] * Arg2) + (object->Velocity[2] * Arg3);
        } else {
            temp_v1->unk_04[2] = -Player->Velocity[2] * Arg2;
        }
        var_f2 = (temp_v1->unk_04[0] * temp_v1->unk_04[0]) + (temp_v1->unk_04[2] * temp_v1->unk_04[2]);
    }
    return var_f2;
}

void KwreboundsndPlayer(s32 ObjectIndex, s32 PlayerId, f32 Arg2, f32 Arg3, u32 SoundBits) {
    UNUSED s32 stackPadding;
    Player* Player;

    Player = &PlayerOne[PlayerId];
    if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
        KwlostGhost();
    }
    if ((KwreboundKartplas(Player, ObjectIndex, Arg2, Arg3) >= 4.0) && ((Player->Flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
        NAPlyTrgStart(PlayerId, SoundBits);
    }
}

void KwreboundsndPlayerNolostghost(s32 ObjectIndex, s32 PlayerId, f32 Arg2, f32 Arg3, u32 SoundBits) {
    UNUSED s32 stackPadding;
    Player* Player;

    Player = &PlayerOne[PlayerId];
    if ((KwreboundKartplas(Player, ObjectIndex, Arg2, Arg3) >= 4.0) && ((Player->Flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
        NAPlyTrgStart((u8) PlayerId, SoundBits);
    }
}

s32 KwhitcheckPlayer(s32 ObjectIndex) {
    Player* Player;
    s32 playerIndex;
    s32 var_s6;

    var_s6 = 0;
    Player = PlayerOne;
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) &&
                (KwanmCheckcollPlayerXz(ObjectIndex, Player) != 0)) {
                if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                    KwlostGhost();
                }
                var_s6 = 1;
            }
        }
    }
    return var_s6;
}

void KwcheckReboundPlayer2D(s32 ObjectIndex, f32 arg1, f32 Arg2) {
    Player* Player;
    s32 playerIndex;

    Player = PlayerOne;
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) && !(Player->SlipFlag & (STAR | TERESA)) &&
                (KwanmCheckcollPlayerXz(ObjectIndex, Player) != 0)) {
                KwreboundKartplas(Player, ObjectIndex, arg1, Arg2 * 1.1);
                if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                    KwlostGhost();
                }
            }
        }
    }
}

void KwcheckReboundsndPlayer2D(s32 ObjectIndex, f32 arg1, f32 Arg2, u32 Arg3) {
    Player* Player;
    s32 playerIndex;

    Player = PlayerOne;
    KwanmClrflag(ObjectIndex, 0x02000000);
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) && !(Player->SlipFlag & TERESA)) {
                if ((Player->Flag & EXISTS) && !(Player->Flag & IS_GHOST) &&
                    (KwanmCheckcollPlayerXz(ObjectIndex, Player) != 0)) {
                    if (Player->SlipFlag & STAR) {
                        KwanmSetflag(ObjectIndex, 0x02000000);
                    } else {
                        if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                            KwlostGhost();
                        }
                        if ((KwreboundKartplas(Player, ObjectIndex, arg1, Arg2 * 1.1) >= 4.0) &&
                            ((Player->Flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
                            NAPlyTrgStart(playerIndex, Arg3);
                        }
                    }
                }
            }
        }
    }
}

void KwcheckReboundPlayer3D(s32 ObjectIndex, f32 arg1, f32 Arg2) {
    Player* Player;
    s32 playerIndex;

    Player = PlayerOne;
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) && !(Player->SlipFlag & (TERESA | STAR)) &&
                (KwanmCheckcollPlayerXYZ(ObjectIndex, Player) != 0)) {
                KwreboundKartplas(Player, ObjectIndex, arg1, Arg2 * 1.1);
                if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                    KwlostGhost();
                }
            }
        }
    }
}

s32 KwcheckBombPlayer2D(s32 ObjectIndex) {
    Player* Player;
    s32 sp40;
    s32 playerIndex;
    s32 test;

    test = 0;
    sp40 = 0;
    Player = PlayerOne;
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++, test++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) && !(Player->SlipFlag & (TERESA | EXPLODE)) &&
                (Player->Flag & EXISTS) && !(Player->Flag & IS_GHOST) &&
                (KwanmCheckcollPlayerXz(ObjectIndex, Player) != 0)) {
                if (!(Player->SlipFlag & STAR)) {
                    Player->Weapon |= HIT_BOMB;
                    if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                        KwlostGhost();
                    }
                } else {
                    NAPlyTrgStart(test, 0x19018010U);
                }
                sp40 = 1;
            }
        }
    }
    return sp40;
}

s32 KwcheckBombPlayer3D(s32 ObjectIndex, f32 arg1) {
    Player* Player;
    s32 playerIndex;
    s32 var_s7;

    var_s7 = 0;
    Player = PlayerOne;
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) && !(Player->SlipFlag & (TERESA | EXPLODE))) {
                if ((Player->Flag & EXISTS) && !(Player->Flag & IS_GHOST) &&
                    (KwanmCheckcollPillarPlayer(ObjectIndex, Player, arg1) != 0)) {
                    if (!(Player->SlipFlag & STAR)) {
                        Player->Weapon |= HIT_BOMB;
                        if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                            KwlostGhost();
                        }
                    }
                    var_s7 = 1;
                }
            }
        }
    }
    return var_s7;
}

s32 KwcheckSlipPlayer(s32 ObjectIndex) {
    Player* Player;
    s32 playerIndex;
    s32 var_s6;

    var_s6 = 0;
    Player = PlayerOne;
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) &&
                !(Player->SlipFlag & (TERESA | SPIN_L | SPIN_R)) &&
                (KwanmCheckcollPlayerXz(ObjectIndex, Player) != 0)) {
                if (Player->SlipFlag & STAR) {
                    var_s6 = 1;
                } else {
                    Player->Weapon |= HIT_BANANA;
                }
            }
        }
    }
    return var_s6;
}

s32 KwcheckSlipKaniPlayer(s32 ObjectIndex) {
    Player* Player;
    s32 playerIndex;
    s32 var_s7;

    var_s7 = 0;
    Player = PlayerOne;
    if (KwanmBitcheckflag(ObjectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < Kwkartnum; playerIndex++, Player++) {
            if ((DynamicObjects[ObjectIndex].Anmptr != 0) &&
                !(Player->SlipFlag & (TERESA | STAR | SPIN_L | SPIN_R))) {
                if ((Player->Flag & EXISTS) && !(Player->Flag & IS_GHOST) &&
                    (KwanmCheckcollPlayerXz(ObjectIndex, Player) != 0)) {
                    var_s7 = 1;
                    if (KwanmBitcheckflag(ObjectIndex, 0x04000000) != 0) {
                        KwlostGhost();
                    }
                    Player->Weapon |= HIT_CRITTER;
                }
            }
        }
    }
    return var_s7;
}

s32 KwanmCheckobjangCameraXz(s32 ObjectIndex, Camera* Camera, u16 Arg2) {
    u16 temp_t3;
    s32 var_v1;

    var_v1 = 0;
    temp_t3 = (((u16) Camera->CameraDirection[1] - DynamicObjects[ObjectIndex].ang[1]) + (Arg2 >> 1));

    //! @warning Always true
    if ((temp_t3 >= 0) && (Arg2 >= temp_t3)) {
        var_v1 = 1;
    }
    return var_v1;
}

s32 KwanmVischeckPlayerXz(s32 ObjectIndex, Player* Player, Camera* Camera, u16 Arg3) {
    u16 temp_t3;
    f32 x_diff;
    f32 z_diff;
    s32 var_t0;

    var_t0 = 0;
    x_diff = DynamicObjects[ObjectIndex].pos[0] - Player->Position[0];
    z_diff = DynamicObjects[ObjectIndex].pos[2] - Player->Position[2];
    temp_t3 = ((u16) Camera->CameraDirection[1] - Atan2t(x_diff, z_diff)) + (Arg3 >> 1);
    if ((temp_t3 >= 0) && (Arg3 >= temp_t3)) {
        var_t0 = 1;
    }
    return var_t0;
}

bool KwanmVischeckCameraXz(s32 ObjectIndex, Camera* Camera, u16 Angle) {
    u16 temp_t2;
    s32 var_t0;

    var_t0 = false;
    temp_t2 = (KwcalcDestangley(Camera->CameraPos[0], DynamicObjects[ObjectIndex].pos[0], Camera->CameraPos[2],
                                    DynamicObjects[ObjectIndex].pos[2]) +
               ((s32) Angle / 2)) -
              Camera->CameraDirection[1];
    if ((temp_t2 >= 0) && (Angle >= temp_t2)) {
        var_t0 = true;
    }
    return var_t0;
}

void KwcheckLODTexture(s32 ObjectIndex, s32 CameraId, s32 Arg2, s32 Arg3) {
    u32 temp_v0;
    u16 var_a2;
    Camera* Camera;

    Camera = &Camera1[CameraId];
    KwanmClrflag(ObjectIndex, 0x00100000 | VISIBLE);
    temp_v0 = KwanmCalcDistanceCameraXz(ObjectIndex, Camera);
    if (temp_v0 < 0x2711U) {
        var_a2 = 0x5555;
    } else if (temp_v0 < 0x9C41U) {
        var_a2 = 0x4000;
    } else {
        var_a2 = 0x2AAB;
    }
    if ((KwanmVischeckCameraXz(ObjectIndex, Camera, var_a2) != 0) && ((u32) (Arg3 * Arg3) >= temp_v0)) {
        KwanmSetflag(ObjectIndex, VISIBLE);
        if (temp_v0 >= (u32) (Arg2 * Arg2)) {
            KwanmSetflag(ObjectIndex, 0x00100000);
        }
    }
}

// This function is really cool, it tests the Value of an unitialized local variable
UNUSED void KwcheckVisible(s32 ObjectIndex, s32 CameraId, u16 Arg2) {
    Camera* Camera;
    u32 no_init;
    u16 var_a2;

    Camera = &Camera1[CameraId];
    KwanmClrflag(ObjectIndex, VISIBLE);
    if (no_init < 0x2711U) {
        var_a2 = 0x5555;
    } else if (no_init < 0x9C41U) {
        var_a2 = 0x4000;
    } else {
        var_a2 = Arg2;
    }
    if (KwanmVischeckCameraXz(ObjectIndex, Camera, var_a2) != 0) {
        KwanmSetflag(ObjectIndex, VISIBLE);
    }
}

s32 KwcheckVisArea(s32 ObjectIndex, s32 CameraId, u16 Arg2, s32 Arg3) {
    Camera* Camera;
    u32 dist;
    u16 var_a2;

    Camera = &Camera1[CameraId];
    KwanmClrflag(ObjectIndex, 0x00020000 | VISIBLE);
    dist = KwanmCalcDistanceCameraXz(ObjectIndex, Camera);
    if (dist < (Arg3 * Arg3)) {
        KwanmSetflag(ObjectIndex, 0x00020000);
        if (dist < 0x2711U) {
            var_a2 = 0x5555;
        } else if (dist < 0x9C41U) {
            var_a2 = 0x4000;
        } else {
            var_a2 = Arg2;
        }
        if (KwanmVischeckCameraXz(ObjectIndex, Camera, var_a2) != 0) {
            KwanmSetflag(ObjectIndex, VISIBLE);
        }
    }
    return dist;
}

void KwcheckDisplayKage(s32 ObjectIndex, s32 CameraId, s32 Arg2) {
    if (KwanmCalcDistanceCameraXz(ObjectIndex, &Camera1[CameraId]) < (u32) (Arg2 * Arg2)) {
        KwanmSetflag(ObjectIndex, 0x00000020);
    } else {
        KwanmClrflag(ObjectIndex, 0x00000020);
    }
}

void KwcheckAreainCamera(s32 ObjectIndex) {
    s32 loopIndex;
    Camera* Camera;

    KwanmClrflag(ObjectIndex, 0x00070000);
    for (loopIndex = 0, Camera = Camera1; loopIndex < PlayerCount; loopIndex++, Camera++) {
        if (DynamicObjects[ObjectIndex].Anmptr != 0) {
            if ((D_8018CF68[loopIndex] >= (DynamicObjects[ObjectIndex].Unk0DF - 1)) &&
                ((DynamicObjects[ObjectIndex].Unk0DF + 1) >= D_8018CF68[loopIndex])) {
                KwanmSetflag(ObjectIndex, 0x00010000);
                if (D_8018CF68[loopIndex] == DynamicObjects[ObjectIndex].Unk0DF) {
                    KwanmSetflag(ObjectIndex, 0x00020000);
                }
                if (KwanmVischeckCameraXz(ObjectIndex, Camera, 0x2AABU) != 0) {
                    KwanmSetflag(ObjectIndex, VISIBLE);
                }
            }
        }
    }
}

UNUSED void KwcheckVisibleCamera(s32 ObjectIndex) {
    s32 loopIndex;
    Camera* Camera;

    KwanmClrflag(ObjectIndex, VISIBLE);
    for (Camera = Camera1, loopIndex = 0; loopIndex < PlayerCount; loopIndex++, Camera++) {
        if ((DynamicObjects[ObjectIndex].Anmptr != 0) && (KwanmVischeckCameraXz(ObjectIndex, Camera, 0x2AABU) != 0)) {
            KwanmSetflag(ObjectIndex, VISIBLE);
        }
    }
}

void KwcheckVisibleAreaCamera(s32 ObjectIndex, f32 arg1) {
    u16 var_a2;
    s32 loopIndex;
    Camera* Camera;

    KwanmClrflag(ObjectIndex, 0x00060000);
    for (Camera = Camera1, loopIndex = 0; loopIndex < PlayerCount; loopIndex++, Camera++) {
        if ((DynamicObjects[ObjectIndex].Anmptr != 0) &&
            (KwanmCheckradiusCameraXz(ObjectIndex, Camera, arg1) != 0)) {
            KwanmSetflag(ObjectIndex, 0x00020000);
            if (arg1 <= 500.0) {
                var_a2 = 0x4000;
            } else {
                var_a2 = 0x2AAB;
            }
            if (KwanmVischeckCameraXz(ObjectIndex, Camera, var_a2) != 0) {
                KwanmSetflag(ObjectIndex, VISIBLE);
            }
        }
    }
}

UNUSED void KwcheckVisibleNearCamera(s32 arg0) {
    KwcheckVisibleAreaCamera(arg0, 500.0f);
}

UNUSED void KwcheckVisibleMiddleCamera(s32 arg0) {
    KwcheckVisibleAreaCamera(arg0, 1000.0f);
}

UNUSED void KwcheckVisibleFarCamera(s32 arg0) {
    KwcheckVisibleAreaCamera(arg0, 2000.0f);
}

UNUSED s32 KwcheckMapareaKart(Player* Player) {
    return CheckArea(Player->Bump.LastZX);
}

s32 KwcheckMapareaCamera(Camera* Camera) {
    return CheckArea(Camera->Bump.LastZX);
}

s32 KwcheckMapareain(s16 arg0, s16 arg1) {
    s32 var_v1;
    s16* var_v0;
    s32 i;
    var_v1 = 0;
    for (i = 0; i < PlayerCount; i++) {
        var_v0 = &D_8018CF68[i];
        if ((*var_v0 >= arg0) && (arg1 >= *var_v0)) {
            var_v1 = 1;
        }
    }
    return var_v1;
}

void KwmvSetvelLine(s32 ObjectIndex) {
    SplineControlPoint* temp_v0;

    temp_v0 = DynamicObjects[ObjectIndex].Sptbaddr;
    DynamicObjects[ObjectIndex].Velocity[0] = (f32) (temp_v0[1].pos[0] - temp_v0[0].pos[0]) / (f32) temp_v0[0].Velocity;
    DynamicObjects[ObjectIndex].Velocity[1] = (f32) (temp_v0[1].pos[1] - temp_v0[0].pos[1]) / (f32) temp_v0[0].Velocity;
    DynamicObjects[ObjectIndex].Velocity[2] = (f32) (temp_v0[1].pos[2] - temp_v0[0].pos[2]) / (f32) temp_v0[0].Velocity;
}

void KwmvNextlinepoint(s32 ObjectIndex) {
    UNUSED s32 temp_t9;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Sptbaddr++;
    object->Spstepadd = (s16) (10000 / (s16) (object->Sptbaddr[0].Velocity));
    object->Spstep = 0;
    KwmvSetvelLine(ObjectIndex);
}

void KwmvLine3DInit(s32 ObjectIndex) {
    AnmObject* object;
    object = &DynamicObjects[ObjectIndex];
    object->Sptbaddr = object->Sptbtopaddr->ControlPoints;
    object->Stack[9] = 0;
    object->Spstep = 0;
    /*
    This is INCREDIBLY stupid. This should really be
    temp_v0->Stack[8] = temp_v0->Sptbtopaddr->NumControlPoints;
    but for some unholy reason that doesn't match
    */
    object->Stack[8] = *((s16*) object->Sptbaddr - 1);
    object->PosSpline[0] = object->Sptbaddr[0].pos[0];
    object->PosSpline[1] = object->Sptbaddr[0].pos[1];
    object->PosSpline[2] = object->Sptbaddr[0].pos[2];
    object->Spstepadd = (s16) (10000 / object->Sptbaddr[0].Velocity);
    KwmvSetvelLine(ObjectIndex);
    KWAnmNext(ObjectIndex);
}

void KwmvLine3D(s32 ObjectIndex) {
    UNUSED s16 temp_t3;
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->PosSpline[0] += object->Velocity[0];
    object->PosSpline[1] += object->Velocity[1];
    object->PosSpline[2] += object->Velocity[2];
    object->Spstep += (u16) object->Spstepadd;
    if (object->Spstep >= 0x2710) {
        object->Stack[9] = (u16) object->Stack[9] + 1;
        if (((u16) object->Stack[9] + 1) == (u16) object->Stack[8]) {
            object->Mvptr += 1;
        } else {
            KwmvNextlinepoint(ObjectIndex);
        }
    }
}

UNUSED void KwstStrategy3D(s32 arg0) {
    switch (DynamicObjects[arg0].Mvptr) {
        case 1:
            KwmvLine3DInit(arg0);
            break;
        case 2:
            KwmvLine3D(arg0);
            break;
        case 3:
            KwmvFinish(arg0);
            /* fallthrough */
        case 0:
            break;
    }
}

UNUSED void KwstStrategy3DLoop(s32 arg0) {
    switch (DynamicObjects[arg0].Mvptr) {
        case 1:
            KwmvLine3DInit(arg0);
            break;
        case 2:
            KwmvLine3D(arg0);
            break;
        case 3:
            KwmvJump(arg0, 1);
            /* fallthrough */
        case 0:
            break;
    }
}

UNUSED void Kwbsplinemat1(void) {
}

UNUSED void Kwbsplinemat2(void) {
}

UNUSED void Kwbsplinematl2(void) {
}

UNUSED void Kwbsplinematl1(void) {
}

// Evaluate the Bernstein polynomials for a B-spline at `arg1` (which is `t`)
void Kwbsplinematn(f32 arg0[], f32 arg1) {
    arg0[0] = (f32) ((f64) ((f32) (1.0 - arg1) * (f32) (1.0 - arg1) * (f32) (1.0 - arg1)) / 6.0);
    arg0[1] = (f32) ((((f64) (arg1 * arg1 * arg1) * 0.5) - arg1 * arg1) + 0.6666666666666666);
    arg0[2] = (f32) (((f64) (arg1 * arg1 * arg1) * -0.5) + (0.5 * (arg1 * arg1)) + (0.5 * arg1) + 0.16666666666666666);
    arg0[3] = (f32) ((f64) (arg1 * arg1 * arg1) / 6.0);
}

UNUSED void Kwbsplinevelmat1(void) {
}

UNUSED void Kwbsplinevelmat2(void) {
}

// Evaluate the Bernstein polynomials for the derivative of a B-spline at `arg1` (which is `t`)
void Kwbsplinevelmatn(f32 arg0[], f32 arg1) {
    arg0[0] = (f32) (1.0 - arg1) * -0.5 * (f32) (1.0 - arg1);
    arg0[1] = arg1 * arg1 * 1.5 - 2.0 * arg1;
    arg0[2] = (arg1 * arg1 * 3.0 - 2.0 * arg1 - (f32) 1.0) * -0.5;
    arg0[3] = arg1 * arg1 * 0.5;
}

UNUSED void Kwbsplinevelmatl2(void) {
}

UNUSED void Kwbsplinevelmatl1(void) {
}

// D_80183DC8 is where the spline's derivative polynomial calculations are stored
void Kwset3Dsplinevel(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->Velocity[0] = (D_80183DC8[0] * D_80165760[0]) + (D_80183DC8[1] * D_80165760[1]) +
                          (D_80183DC8[2] * D_80165760[2]) + (D_80183DC8[3] * D_80165760[3]);
    object->Velocity[1] = (D_80183DC8[0] * D_80165770[0]) + (D_80183DC8[1] * D_80165770[1]) +
                          (D_80183DC8[2] * D_80165770[2]) + (D_80183DC8[3] * D_80165770[3]);
    object->Velocity[2] = (D_80183DC8[0] * D_80165780[0]) + (D_80183DC8[1] * D_80165780[1]) +
                          (D_80183DC8[2] * D_80165780[2]) + (D_80183DC8[3] * D_80165780[3]);
}

void KW3Dsplinevel(s32 ObjectIndex, f32 arg1) {
    Kwbsplinevelmatn(D_80183DC8, arg1);
    Kwset3Dsplinevel(ObjectIndex);
}

UNUSED void KW3Dsplinevel1(void) {
}

UNUSED void KW3Dsplinevel2(void) {
}

UNUSED void KW3Dsplinevell2(void) {
}

UNUSED void KW3Dsplinevell1(void) {
}

// D_80183DA8 is where the spline's polynomial calculations are stored
void Kwset3Dsplinezahyou(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->PosSpline[0] = (D_80183DA8[0] * D_80165760[0]) + (D_80183DA8[1] * D_80165760[1]) +
                        (D_80183DA8[2] * D_80165760[2]) + (D_80183DA8[3] * D_80165760[3]);
    object->PosSpline[1] = (D_80183DA8[0] * D_80165770[0]) + (D_80183DA8[1] * D_80165770[1]) +
                        (D_80183DA8[2] * D_80165770[2]) + (D_80183DA8[3] * D_80165770[3]);
    object->PosSpline[2] = (D_80183DA8[0] * D_80165780[0]) + (D_80183DA8[1] * D_80165780[1]) +
                        (D_80183DA8[2] * D_80165780[2]) + (D_80183DA8[3] * D_80165780[3]);
}

void KW3Dsplinezahyou(s32 ObjectIndex, f32 arg1) {
    Kwbsplinematn(D_80183DA8, arg1);
    Kwset3Dsplinezahyou(ObjectIndex);
}

UNUSED void KW3Dsplinezahyou1(void) {
}

UNUSED void KW3Dsplinezahyou2(void) {
}

UNUSED void KW3Dsplinezahyoul2(void) {
}

UNUSED void KW3Dsplinezahyoul1(void) {
}

void KwmvSetsplinepoint(s32 ObjectIndex) {
    s32 someIndex;
    SplineControlPoint* test;

    test = DynamicObjects[ObjectIndex].Sptbaddr;
    for (someIndex = 0; someIndex < 4; someIndex++) {
        D_80165760[someIndex] = test->pos[0];
        D_80165770[someIndex] = test->pos[1];
        D_80165780[someIndex] = test->pos[2];
        test++;
    }
}

void KwmvSetsplinepointLoop(s32 ObjectIndex) {
    s32 someIndex;
    s32 sp0;
    s32 temp_a1;
    s32 temp_a2;
    SplineControlPoint* test;

    test = DynamicObjects[ObjectIndex].Sptbaddr;
    temp_a1 = DynamicObjects[ObjectIndex].Stack[9];
    temp_a2 = (u16) DynamicObjects[ObjectIndex].Stack[8];
    if ((temp_a2 - 4) >= temp_a1) {
        sp0 = 10000;
    } else if ((temp_a1 + 3) == temp_a2) {
        sp0 = 2;
    } else if ((temp_a1 + 2) == temp_a2) {
        sp0 = 1;
    } else if ((temp_a1 + 1) == temp_a2) {
        sp0 = 0;
    }
    for (someIndex = 0; someIndex < 4; someIndex++) {
        D_80165760[someIndex] = test->pos[0];
        D_80165770[someIndex] = test->pos[1];
        D_80165780[someIndex] = test->pos[2];
        if (sp0 == someIndex) {
            // Reset back to Start of the spline path
            test = DynamicObjects[ObjectIndex].Sptbtopaddr->ControlPoints;
        } else {
            test++;
        }
    }
}

void KwmvSpline3DInit(s32 ObjectIndex) {
    AnmObject* object;
    UNUSED SplineData* spline;

    if (KwanmBitoffcheckflag(ObjectIndex, 8) != 0) {
        object = &DynamicObjects[ObjectIndex];
        object->Stack[9] = 0;
        object->Spstep = 0;
        object->Sptbaddr = object->Sptbtopaddr->ControlPoints;
        /*
        This is INCREDIBLY stupid. This should really be
        temp_v0->Stack[8] = temp_v0->Sptbtopaddr->NumControlPoints;
        but for some unholy reason that doesn't match
        */
        object->Stack[8] = *(((s16*) object->Sptbaddr) - 1);

        KwanmSetflag(ObjectIndex, 8);
    }
}

void KwmvNextsplinepoint(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].Spstep = 0;
    DynamicObjects[ObjectIndex].Sptbaddr++;
}

void KwmvSpline3DCommon(s32 ObjectIndex, s32 arg1) {
    f32 sp34;
    f32 temp;
    UNUSED f32 temp2;
    f32 var_f6;

    KwmvSpline3DInit(ObjectIndex);
    if (arg1 != 0) {
        KwmvSetsplinepointLoop(ObjectIndex);
    } else {
        KwmvSetsplinepoint(ObjectIndex);
    }

    // I think the game treats each spline as being having a lenght of 10000
    // This is getting the percent along the spline we want to reach,
    // which is then treated as the `t` Value given to the Curve calculations
    sp34 = ((f32) DynamicObjects[ObjectIndex].Spstep / 10000.0);
    // Calculate the Curve at `t`
    KW3Dsplinezahyou(ObjectIndex, sp34);
    if (KwanmBitcheckflag(ObjectIndex, 0x800) != 0) {
        // Calculate the Curve's derivative at `t`
        KW3Dsplinevel(ObjectIndex, sp34);
    }

    // These values somehow control how fast we travel along the Curve
    var_f6 = DynamicObjects[ObjectIndex].Sptbaddr[0].Velocity;
    temp = DynamicObjects[ObjectIndex].Sptbaddr[1].Velocity;

    DynamicObjects[ObjectIndex].Spstepadd = 10000.0 / (((temp - var_f6) * sp34) + var_f6);
    DynamicObjects[ObjectIndex].Spstep += DynamicObjects[ObjectIndex].Spstepadd;
}

void KwmvSpline3D(s32 ObjectIndex) {
    UNUSED s16 temp_t0;
    AnmObject* object;

    KwmvSpline3DCommon(ObjectIndex, 0);
    object = &DynamicObjects[ObjectIndex];
    if (object->Spstep >= 0x2710) {
        // Have to do it this way due to the u16 cast
        object->Stack[9] = (u16) object->Stack[9] + 1;
        if (((u16) object->Stack[9] + 3) == (u16) object->Stack[8]) {
            object->Mvptr += 1;
        } else {
            KwmvNextsplinepoint(ObjectIndex);
        }
    }
}

void KwmvSpline3DLoop(s32 ObjectIndex) {
    AnmObject* object;

    KwmvSpline3DCommon(ObjectIndex, 1);
    object = &DynamicObjects[ObjectIndex];
    if (object->Spstep >= 0x2710) {
        // Have to do it this way due to the u16 cast
        object->Stack[9] = (u16) object->Stack[9] + 1;
        if ((u16) object->Stack[9] == (u16) object->Stack[8]) {
            KwanmClrflag(ObjectIndex, 8);
        } else {
            KwmvNextsplinepoint(ObjectIndex);
        }
    }
}

void KwspStrategy3D(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            KwmvSpline3D(ObjectIndex);
            break;
        case 2:
            KwmvFinish(ObjectIndex);
            break;
    }
}

void KwspStrategy3DLoop(s32 ObjectIndex) {
    switch (DynamicObjects[ObjectIndex].Mvptr) {
        case 0:
            break;
        case 1:
            KwmvSpline3DLoop(ObjectIndex);
            break;
    }
}

void KwsetInit3DSpos(s32 ObjectIndex, f32 arg1, f32 Arg2, f32 Arg3) {
    DynamicObjects[ObjectIndex].PosStart[0] = arg1;
    DynamicObjects[ObjectIndex].PosStart[1] = Arg2;
    DynamicObjects[ObjectIndex].PosStart[2] = Arg3;
}

void KwsetInit3DSppos(s32 ObjectIndex, f32 arg1, f32 Arg2, f32 Arg3) {
    DynamicObjects[ObjectIndex].PosSpline[0] = arg1;
    DynamicObjects[ObjectIndex].PosSpline[1] = Arg2;
    DynamicObjects[ObjectIndex].PosSpline[2] = Arg3;
}

void KwsetInit3DPos(s32 ObjectIndex) {
    f32 temp_f0 = DynamicObjects[ObjectIndex].PosStart[0];

    DynamicObjects[ObjectIndex].pos[0] = DynamicObjects[ObjectIndex].PosSpline[0] + temp_f0;
    DynamicObjects[ObjectIndex].pos[1] = DynamicObjects[ObjectIndex].PosSpline[1] + temp_f0;
    DynamicObjects[ObjectIndex].pos[2] = DynamicObjects[ObjectIndex].PosSpline[2] + temp_f0;
}

void KwsetInit3DAng(s32 ObjectIndex, u16 arg1, u16 Arg2, u16 Arg3) {
    DynamicObjects[ObjectIndex].ang[0] = arg1;
    DynamicObjects[ObjectIndex].ang[1] = Arg2;
    DynamicObjects[ObjectIndex].ang[2] = Arg3;
}

void KwsetInit3DTexang(s32 ObjectIndex, u16 arg1, u16 Arg2, u16 Arg3) {
    DynamicObjects[ObjectIndex].Texang[0] = arg1;
    DynamicObjects[ObjectIndex].Texang[1] = Arg2;
    DynamicObjects[ObjectIndex].Texang[2] = Arg3;
}

void KwsetInit3DVel(s32 ObjectIndex, f32 arg1, f32 Arg2, f32 Arg3) {
    DynamicObjects[ObjectIndex].Velocity[0] = arg1;
    DynamicObjects[ObjectIndex].Velocity[1] = Arg2;
    DynamicObjects[ObjectIndex].Velocity[2] = Arg3;
}

void KwsetInit3DPosang(s32 ObjectIndex, s16 arg1, s16 Arg2, s16 Arg3, SplineData* spline) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    object->PosStart[0] = (f32) arg1;
    object->PosStart[1] = (f32) Arg2;
    object->Sptbtopaddr = spline;
    object->PosStart[2] = (f32) Arg3;
    object->Velocity[0] = (f32) (spline->ControlPoints[1].pos[0] - spline->ControlPoints[0].pos[0]);
    object->Velocity[1] = (f32) (spline->ControlPoints[1].pos[1] - spline->ControlPoints[0].pos[1]);
    object->Velocity[2] = (f32) (spline->ControlPoints[1].pos[2] - spline->ControlPoints[0].pos[2]);
    object->ang[1] = KwmvVelxztoangy(ObjectIndex);
    object->Velocity[2] = Kwrotate2DX(object->Velocity[2], object->Velocity[0], -object->ang[1]);
    object->ang[0] = KwmvVelyztoangx(ObjectIndex);
}

UNUSED void KwsetRandomx(s32 ObjectIndex, s16 x, u16 Arg2) {
    s16 offset_x;

    offset_x = MakeRandomLimmit(Arg2) - (Arg2 / 2);
    DynamicObjects[ObjectIndex].PosStart[0] = x + offset_x;
}

void KwsetRandomy(s32 ObjectIndex, s16 y, u16 Arg2) {
    s16 offset_y;

    offset_y = MakeRandomLimmit(Arg2) - (Arg2 / 2);
    DynamicObjects[ObjectIndex].PosStart[1] = y + offset_y;
}

UNUSED void KwsetRandomz(s32 ObjectIndex, s16 z, u16 Arg2) {
    s16 offset_z;

    offset_z = MakeRandomLimmit(Arg2) - (Arg2 / 2);
    DynamicObjects[ObjectIndex].PosStart[2] = z + offset_z;
}

UNUSED void KwsetRandomxy(s32 ObjectIndex, s16 x, s16 y, u16 Arg3, u16 arg4) {
    s16 offset_x;
    s16 offset_y;

    offset_x = MakeRandomLimmit(Arg3) - ((s32) Arg3 / 2);
    offset_y = MakeRandomLimmit(arg4) - ((s32) arg4 / 2);
    DynamicObjects[ObjectIndex].PosStart[0] = x + offset_x;
    DynamicObjects[ObjectIndex].PosStart[1] = y + offset_y;
}

UNUSED void KwsetRandomxz(s32 ObjectIndex, s16 x, s16 z, u16 Arg3, u16 arg4) {
    s16 offset_x;
    s16 offset_z;

    offset_x = MakeRandomLimmit(Arg3) - ((s32) Arg3 / 2);
    offset_z = MakeRandomLimmit(arg4) - ((s32) arg4 / 2);
    DynamicObjects[ObjectIndex].PosStart[0] = x + offset_x;
    DynamicObjects[ObjectIndex].PosStart[2] = z + offset_z;
}

void KwsetRandomxyz(s32 ObjectIndex, s16 x, s16 y, s16 z, u16 arg4, u16 arg5, u16 arg6) {
    s16 offset_x;
    s16 offset_y;
    s16 offset_z;

    offset_x = MakeRandomLimmit(arg4) - ((s32) arg4 / 2);
    offset_y = MakeRandomLimmit(arg5) - ((s32) arg5 / 2);
    offset_z = MakeRandomLimmit(arg6) - ((s32) arg6 / 2);
    DynamicObjects[ObjectIndex].PosStart[0] = x + offset_x;
    DynamicObjects[ObjectIndex].PosStart[1] = y + offset_y;
    DynamicObjects[ObjectIndex].PosStart[2] = z + offset_z;
}

void KwsetRelplayerXz(s32 ObjectIndex, s16 dist, u16 Angle) {
    DynamicObjects[ObjectIndex].PosStart[0] = PlayerOneCopy->Position[0] + (SinT(Angle) * dist);
    DynamicObjects[ObjectIndex].PosStart[2] = PlayerOneCopy->Position[2] + (CosT(Angle) * dist);
}

UNUSED void KwsetRelplayerRandomxz(s32 ObjectIndex, u16 arg1, u16 Arg2) {
    u16 thing;
    u16 thing2;

    thing = MakeRandomLimmit(arg1);
    thing2 = Camera1->CameraDirection[1] + MakeRandomLimmit(Arg2) - ((s32) Arg2 / 2);
    KwsetRelplayerXz(ObjectIndex, thing, thing2);
}

void KwsetSplinepos3D(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].pos[0] = DynamicObjects[ObjectIndex].PosStart[0] + DynamicObjects[ObjectIndex].PosSpline[0];
    DynamicObjects[ObjectIndex].pos[1] = DynamicObjects[ObjectIndex].PosStart[1] + DynamicObjects[ObjectIndex].PosSpline[1];
    DynamicObjects[ObjectIndex].pos[2] = DynamicObjects[ObjectIndex].PosStart[2] + DynamicObjects[ObjectIndex].PosSpline[2];
}

void KwsetPosang3D(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    D_80183E40[0] = object->pos[0];
    D_80183E40[1] = object->pos[1];
    D_80183E40[2] = object->pos[2];
    D_80183E80[0] = object->ang[0];
    D_80183E80[1] = object->ang[1];
    D_80183E80[2] = object->ang[2];
}

void KwsetPos3Dto2D(s32 ObjectIndex) {
    DynamicObjects[ObjectIndex].ix = DynamicObjects[ObjectIndex].pos[0];
    DynamicObjects[ObjectIndex].iy = DynamicObjects[ObjectIndex].pos[1];
}

void KwjugemuStrategy3D(s32 ObjectIndex) {
    AnmObject* object;

    object = &DynamicObjects[ObjectIndex];
    switch (object->Mappoint) {
        case 1:
            if (object->Mvptr == 1) {
                KwmvSpline3D(ObjectIndex);
            }
            break;
        case 2:
            switch (object->Mvptr) {
                case 0:
                    break;
                case 1:
                    KwmvSpline3DLoop(ObjectIndex);
                    break;
            }
            break;
        case 3:
            switch (object->Mvptr) {
                case 0:
                    break;
                case 1:
                    KwmvSpline3D(ObjectIndex);
                    break;
            }
            break;
        case 4:
            switch (object->Mvptr) { /* switch 1; irregular */
                case 0:                /* switch 1 */
                    break;
                case 1: /* switch 1 */
                    KwmvSpline3D(ObjectIndex);
                    break;
                case 2: /* switch 1 */
                    KwmvFinish(ObjectIndex);
                    break;
            }
            break;
        case 5:
            switch (object->Mvptr) { /* switch 2; irregular */
                case 0:                /* switch 2 */
                    break;
                case 1: /* switch 2 */
                    KwmvSpline3D(ObjectIndex);
                    break;
                case 2: /* switch 2 */
                    KwmvFinish(ObjectIndex);
                    break;
            }
            break;
        case 6:
            switch (object->Mvptr) {
                case 0:
                    break;
                case 1:
                    KwmvSpline3D(ObjectIndex);
                    break;
            }
            break;
        case 7:
            switch (object->Mvptr) { /* switch 3; irregular */
                case 0:                /* switch 3 */
                    break;
                case 1: /* switch 3 */
                    KwmvChase3DInit(ObjectIndex);
                    break;
                case 2: /* switch 3 */
                    KwmvChase3D(ObjectIndex);
                    break;
            }
            break;
        case 0:
        default:
            break;
    }
}

UNUSED void KwmawaruTellesa(UNUSED s32 arg0) {
}

UNUSED void KwmvTellesaOufuku(UNUSED s32 arg0) {
}
