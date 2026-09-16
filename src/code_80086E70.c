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

void kwmv_start(s32 objectIndex) {
    g_DynamicObjects[objectIndex].mvptr = 1; // * 0xE0)) = 1;
    kwanm_clrflag(objectIndex, 8);
}

void kwmv_start_normal(s32 objectIndex, s32 arg1, s16 arg2) {
    g_DynamicObjects[objectIndex].spptr = arg1;
    g_DynamicObjects[objectIndex].mvptr = arg2;
    kwanm_clrflag(objectIndex, 8);
}

void kwmv_start_strategy(s32 objectIndex) {
    kwmv_start(objectIndex);
}

void kwmv_start_spline(s32 objectIndex, s32 arg1, SplineData* spline) {
    kwmv_start(objectIndex);
    g_DynamicObjects[objectIndex].mappoint = arg1;
    g_DynamicObjects[objectIndex].sptbtopaddr = spline;
}

void kwmv_finish(s32 objectIndex) {
    g_DynamicObjects[objectIndex].mvptr = 0;
    g_DynamicObjects[objectIndex].spptr = 0;
    g_DynamicObjects[objectIndex].mappoint = 0;
    kwanm_clrflag(objectIndex, 8);
}

bool kwmv_checkfinish(s32 objectIndex) {
    bool ret = false;
    if (g_DynamicObjects[objectIndex].mvptr == 0) {
        ret = true;
    }
    return ret;
}

void KWAnmNext(s32 objectIndex) {
    kwanm_clrflag(objectIndex, 8);
    g_DynamicObjects[objectIndex].mvptr += 1;
}

void kwmv_jump(s32 objectIndex, s32 arg1) {
    kwanm_clrflag(objectIndex, 8);
    g_DynamicObjects[objectIndex].mvptr = arg1;
}

s32 kwmv_wait(s32 objectIndex, s32 arg1) {
    s32 sp1C;

    sp1C = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].mvtm = arg1;
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        sp1C = 1;
    }
    return sp1C;
}

s32 kwmv_randomwait(s32 objectIndex, u16 arg1) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].mvtm = MakeRandomLimmit(arg1);
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        sp24 = 1;
    }
    return sp24;
}

s32 kwmv_waitnext(s32 objectIndex, s32 arg1) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].mvtm = (s16) arg1;
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        KWAnmNext(objectIndex);
        sp24 = 1;
    }
    return sp24;
}

UNUSED void kwmv_addfvar_cnt(s32 objectIndex, UNUSED s32 arg1, f32 arg2) {
    g_DynamicObjects[objectIndex].pos_spline[1] += arg2;
    g_DynamicObjects[objectIndex].pos_spline[2] -= arg2;
}

UNUSED s32 kwlookplayer_xz(s32 objectIndex, Player* player) {
    return kwcalc_destangley(player->position[0], g_DynamicObjects[objectIndex].pos[0], player->position[2],
                                g_DynamicObjects[objectIndex].pos[2]);
}

s32 kwlookcamera_xz(s32 objectIndex, Camera* camera) {
    return Atan2t(g_DynamicObjects[objectIndex].pos[0] - camera->camera_pos[0], g_DynamicObjects[objectIndex].pos[2] - camera->camera_pos[2]);
}

u16 kwmv_velyztoangx(s32 objectIndex) {
    return -Atan2t(g_DynamicObjects[objectIndex].velocity[1], g_DynamicObjects[objectIndex].velocity[2]);
}

s32 kwmv_velxztoangy(s32 objectIndex) {
    return Atan2t(g_DynamicObjects[objectIndex].velocity[0], g_DynamicObjects[objectIndex].velocity[2]);
}

UNUSED void kwchase_velyztoangx(s32 objectIndex) {
    g_DynamicObjects[objectIndex].ang[0] =
        kwchase_angle(g_DynamicObjects[objectIndex].ang[0], kwmv_velyztoangx(objectIndex));
}

void kwchase_velxztoangy(s32 objectIndex) {
    g_DynamicObjects[objectIndex].ang[1] =
        kwchase_angle(g_DynamicObjects[objectIndex].ang[1], kwmv_velxztoangy(objectIndex));
}

UNUSED void kwmv_vx(s32 objectIndex) {
    g_DynamicObjects[objectIndex].velocity[0] =
        g_DynamicObjects[objectIndex].vel * sinT(g_DynamicObjects[objectIndex].ang[1]);
}

UNUSED void kwmv_vy(s32 objectIndex) {
    g_DynamicObjects[objectIndex].velocity[1] =
        g_DynamicObjects[objectIndex].vel * cosT(g_DynamicObjects[objectIndex].ang[0]);
}

UNUSED void kwmv_vz(s32 objectIndex) {
    g_DynamicObjects[objectIndex].velocity[2] =
        g_DynamicObjects[objectIndex].vel * cosT(g_DynamicObjects[objectIndex].ang[1]);
}

void kwmv_vxz(s32 objectIndex) {
    g_DynamicObjects[objectIndex].velocity[0] =
        g_DynamicObjects[objectIndex].vel * sinT(g_DynamicObjects[objectIndex].ang[1]);
    g_DynamicObjects[objectIndex].velocity[2] =
        g_DynamicObjects[objectIndex].vel * cosT(g_DynamicObjects[objectIndex].ang[1]);
}

void kwmv_vxyz(s32 objectIndex) {
    f32 sp24;

    sp24 = cosT(g_DynamicObjects[objectIndex].ang[0]);
    g_DynamicObjects[objectIndex].velocity[0] =
        (g_DynamicObjects[objectIndex].vel * sp24) * sinT(g_DynamicObjects[objectIndex].ang[1]);
    g_DynamicObjects[objectIndex].velocity[1] =
        -g_DynamicObjects[objectIndex].vel * sinT(g_DynamicObjects[objectIndex].ang[0]);
    sp24 = cosT(g_DynamicObjects[objectIndex].ang[0]);
    g_DynamicObjects[objectIndex].velocity[2] =
        (g_DynamicObjects[objectIndex].vel * sp24) * cosT(g_DynamicObjects[objectIndex].ang[1]);
}

void kwmv_vxz_reverse(s32 objectIndex) {
    g_DynamicObjects[objectIndex].velocity[0] =
        g_DynamicObjects[objectIndex].vel * sinT(g_DynamicObjects[objectIndex].ang[1] + 0x8000);
    g_DynamicObjects[objectIndex].velocity[2] =
        g_DynamicObjects[objectIndex].vel * cosT(g_DynamicObjects[objectIndex].ang[1] + 0x8000);
}

void kwmv_velangytopos(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos_spline[0] +=
        g_DynamicObjects[objectIndex].vel * sinT(g_DynamicObjects[objectIndex].ang[1]);
    g_DynamicObjects[objectIndex].pos_spline[2] +=
        g_DynamicObjects[objectIndex].vel * cosT(g_DynamicObjects[objectIndex].ang[1]);
}

void kwmv_addveltopos(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos_spline[0] += g_DynamicObjects[objectIndex].velocity[0];
    g_DynamicObjects[objectIndex].pos_spline[1] += g_DynamicObjects[objectIndex].velocity[1];
    g_DynamicObjects[objectIndex].pos_spline[2] += g_DynamicObjects[objectIndex].velocity[2];
}

void kwmv_addvelxztopos(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos_spline[0] += g_DynamicObjects[objectIndex].velocity[0];
    g_DynamicObjects[objectIndex].pos_spline[2] += g_DynamicObjects[objectIndex].velocity[2];
}

UNUSED void kwmv_addvelxtopos(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos_spline[0] += g_DynamicObjects[objectIndex].velocity[0];
}

void kwmv_addvelytopos(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos_spline[1] += g_DynamicObjects[objectIndex].velocity[1];
}

UNUSED void kwmv_addvelztopos(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos_spline[2] += g_DynamicObjects[objectIndex].velocity[2];
}

void kwmv_velangytoposxz(s32 arg0) {
    kwmv_vxz(arg0);
    kwmv_addvelxztopos(arg0);
}

void kwmv_velangxytoposxyz(s32 arg0) {
    kwmv_vxyz(arg0);
    kwmv_addveltopos(arg0);
}

f32 kwmove_straight(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4) {
    return (((arg4 - arg2) / (arg3 - arg1)) * (arg0 - arg1)) + arg2;
}

s32 kwmv_walkxz(s32 objectIndex, s32 arg1) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        kwmv_vxz(objectIndex);
        g_DynamicObjects[objectIndex].mvtm = arg1;
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        sp24 = 1;
    } else {
        kwmv_addvelxztopos(objectIndex);
    }
    return sp24;
}

s32 kwmv_walkxz_reverse(s32 objectIndex, s32 arg1) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        kwmv_vxz_reverse(objectIndex);
        g_DynamicObjects[objectIndex].mvtm = arg1;
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        sp24 = 1;
    } else {
        kwmv_addvelxztopos(objectIndex);
    }
    return sp24;
}

bool kwmv_walkxz_2pt(s32 objectIndex, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s16 dist;
    s16 temp_a0;
    s16 temp_v0;
    bool sp2C;

    sp2C = false;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        temp_v0 = arg2 - arg1;
        temp_a0 = arg4 - arg3;
        dist = sqrtf((temp_v0 * temp_v0) + (temp_a0 * temp_a0));
        g_DynamicObjects[objectIndex].pos_start[1] = 0.0f;
        g_DynamicObjects[objectIndex].ang[1] = Atan2t(temp_v0, temp_a0);
        kwmv_vxz(objectIndex);
        g_DynamicObjects[objectIndex].mvtm = dist / g_DynamicObjects[objectIndex].vel;
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        sp2C = true;
    } else {
        kwmv_addvelxztopos(objectIndex);
    }
    return sp2C;
}

s32 kwmv_trakka_ground(s32 objectIndex, f32 arg1, f32 arg2) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].velocity[1] = -arg1;
    }
    kwmv_addvelytopos(objectIndex);
    if (g_DynamicObjects[objectIndex].pos[1] <= arg2) {
        kwanm_clrflag(objectIndex, 8);
        KWAnmNext(objectIndex);
        sp24 = 1;
    }
    return sp24;
}

s32 kwmv_nageage(s32 objectIndex, f32 arg1, f32 arg2, s32 arg3) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != false) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].velocity[1] = arg1;
        g_DynamicObjects[objectIndex].mvtm = (s16) arg3;
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        KWAnmNext(objectIndex);
        sp24 = 1;
    } else {
        g_DynamicObjects[objectIndex].velocity[1] -= arg2;
        kwmv_addvelytopos(objectIndex);
    }
    return sp24;
}

s32 kwmv_nageage_ground(s32 objectIndex, f32 arg1, f32 arg2, f32 arg3) {
    s32 sp24;

    sp24 = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != 0) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].velocity[1] = arg1;
    }
    g_DynamicObjects[objectIndex].velocity[1] -= arg2;
    kwmv_addvelytopos(objectIndex);
    if (g_DynamicObjects[objectIndex].pos_spline[1] <= arg3) {
        kwanm_clrflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].pos_spline[1] = arg3;
        KWAnmNext(objectIndex);
        sp24 = 1;
    }
    return sp24;
}

bool kwmv_nanamenage(s32 objectIndex, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s32 arg5) {
    bool sp2C;
    UNUSED s32 pad;

    sp2C = false;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != 0) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].pos_spline[2] = 0.0f;
        g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
        g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
        g_DynamicObjects[objectIndex].vel = arg3;
        g_DynamicObjects[objectIndex].velocity[1] = arg1;
        g_DynamicObjects[objectIndex].ang[1] = arg4;
        kwmv_vxz(objectIndex);
        g_DynamicObjects[objectIndex].mvtm = arg5;
    }
    g_DynamicObjects[objectIndex].mvtm--;
    if (g_DynamicObjects[objectIndex].mvtm < 0) {
        kwanm_clrflag(objectIndex, 8);
        KWAnmNext(objectIndex);
        sp2C = true;
    } else {
        g_DynamicObjects[objectIndex].velocity[1] -= arg2;
        kwmv_addveltopos(objectIndex);
    }
    return sp2C;
}

UNUSED s32 kwmv_nanamenage_ground(s32 objectIndex, f32 arg1, f32 arg2, f32 arg3, s16 arg4, s32 arg5) {
    s32 sp2C;
    UNUSED s32 stackPadding;

    sp2C = 0;
    if (kwanm_bitoffcheckflag(objectIndex, 8) != 0) {
        kwanm_setflag(objectIndex, 8);
        g_DynamicObjects[objectIndex].pos_spline[2] = 0.0f;
        g_DynamicObjects[objectIndex].pos_spline[1] = 0.0f;
        g_DynamicObjects[objectIndex].pos_spline[0] = 0.0f;
        g_DynamicObjects[objectIndex].vel = arg3;
        g_DynamicObjects[objectIndex].velocity[1] = arg1;
        g_DynamicObjects[objectIndex].ang[1] = arg4;
        kwmv_vxz(objectIndex);
        g_DynamicObjects[objectIndex].mvtm = timer;
    }
    if (g_DynamicObjects[objectIndex].pos_spline[1] <= arg5) {
        kwanm_clrflag(objectIndex, 8);
        KWAnmNext(objectIndex);
        sp2C = 1;
    } else {
        g_DynamicObjects[objectIndex].velocity[1] -= arg2;
        kwmv_addveltopos(objectIndex);
    }
    return sp2C;
}

void kwmv_circle(s32 objectIndex, f32 arg1, u16 arg2) {
    f32 temp_f4;
    f32 sp20;

    temp_f4 = g_DynamicObjects[objectIndex].pos_spline[0];
    sp20 = g_DynamicObjects[objectIndex].pos_spline[2];
    g_DynamicObjects[objectIndex].addangy += arg2;
    g_DynamicObjects[objectIndex].pos_spline[0] = sinT(g_DynamicObjects[objectIndex].addangy) * arg1;
    g_DynamicObjects[objectIndex].pos_spline[2] = cosT(g_DynamicObjects[objectIndex].addangy) * arg1;
    g_DynamicObjects[objectIndex].velocity[0] = g_DynamicObjects[objectIndex].pos_spline[0] - temp_f4;
    g_DynamicObjects[objectIndex].velocity[2] = g_DynamicObjects[objectIndex].pos_spline[2] - sp20;
}

UNUSED void kwchase_circle(void) {
}

void kwmv_setchasepoint(s32 objectIndex) {
    SplineControlPoint* phi_v0;
    s32 someIndex;
    phi_v0 = g_DynamicObjects[objectIndex].sptbaddr;
    for (someIndex = 0; someIndex < 2; someIndex++, phi_v0++) {
        D_80165760[someIndex] = phi_v0->pos[0];
        D_80165770[someIndex] = phi_v0->pos[1];
        D_80165780[someIndex] = phi_v0->pos[2];
    }
}

void kwmv_nextchasepoint(s32 arg0) {
    g_DynamicObjects[arg0].sptbaddr++;
}

void kwmv_setdirection(s32 objectIndex, s32 arg1) {
    s16 temp_a1;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    temp_a1 = Atan2t(D_80165760[1] - D_80165760[0], D_80165780[1] - D_80165780[0]);
    temp_a1 -= object->ang[1];
    if (temp_a1 > 0) {
        object->ang[1] += (arg1 << 8);
    } else if (temp_a1 < 0) {
        object->ang[1] -= (arg1 << 8);
    }
}

// if the code is too ugly for you, then go fix it without tampering with the register allocations
void kwmv_chase3D_init(s32 objectIndex) {
    AnmObject* object;
    object = &g_DynamicObjects[objectIndex];
    object->stack[9] = 0;
    object->sptbaddr = object->sptbtopaddr->controlPoints;
    /*
    This is INCREDIBLY stupid. This should really be
    temp_v0->stack[8] = temp_v0->sptbtopaddr->numControlPoints;
    but for some unholy reason that doesn't match
    */
    object->stack[8] = *((s16*) object->sptbaddr - 1);
    object->pos_spline[0] = object->sptbaddr[0].pos[0];
    object->pos_spline[1] = object->sptbaddr[0].pos[1];
    object->pos_spline[2] = object->sptbaddr[0].pos[2];
    KWAnmNext(objectIndex);
}

s32 kwmv_checkcollision(s32 objectIndex, s32 arg1) {
    s32 temp_f0;
    s32 temp_f12;
    s32 temp_f2;
    s32 var_v1;

    var_v1 = 0;
    temp_f0 = (s32) ((D_80165760[1] - g_DynamicObjects[objectIndex].pos_spline[0]) *
                     (D_80165760[1] - g_DynamicObjects[objectIndex].pos_spline[0]));
    temp_f12 = (s32) ((D_80165770[1] - g_DynamicObjects[objectIndex].pos_spline[1]) *
                      (D_80165770[1] - g_DynamicObjects[objectIndex].pos_spline[1]));
    temp_f2 = (s32) ((D_80165780[1] - g_DynamicObjects[objectIndex].pos_spline[2]) *
                     (D_80165780[1] - g_DynamicObjects[objectIndex].pos_spline[2]));
    if ((temp_f0 + temp_f12 + temp_f2 - (arg1 * arg1)) <= 0) {
        var_v1 = 1;
    }
    return var_v1;
}

void kwmv_chase3D(s32 objectIndex) {
    AnmObject* object;

    kwmv_setchasepoint(objectIndex);
    kwmv_setdirection(objectIndex, 1);
    kwmv_velangytopos(objectIndex);
    if (kwmv_checkcollision(objectIndex, 0x0000000A) != 0) {
        object = &g_DynamicObjects[objectIndex];
        object->stack[9] = (u16) object->stack[9] + 1;
        if (((u16) object->stack[9] + 3) == (u16) object->stack[8]) {
            object->mvptr += 1;
        } else {
            kwmv_nextchasepoint(objectIndex);
        }
    }
}

void kwchase_strategy3D(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            kwmv_chase3D_init(objectIndex);
            break;
        case 2:
            kwmv_chase3D(objectIndex);
            break;
        case 3:
            kwmv_finish(objectIndex);
            break;
    }
}

s32 kwanm_check_ground(s32 objectIndex) {
    s32 sp2C;

    sp2C = 0;
    kwanm_clrflag(objectIndex, 0x00800000);
    if (kwanm_bitcheckflag(objectIndex, 0x00000400) != 0) {
        CheckBump(&D_8018C3B0, 10.0f, g_DynamicObjects[objectIndex].pos[0], 20.0f,
                                 g_DynamicObjects[objectIndex].pos[2]);
        if (D_8018C3B0.flag_zx == 1) {
            sp2C = 1;
            kwanm_setflag(objectIndex, 0x00800000);
        }
        g_DynamicObjects[objectIndex].ground = CalcHeight(
            g_DynamicObjects[objectIndex].pos[0], 0.0f, g_DynamicObjects[objectIndex].pos[2], D_8018C3B0.last_zx);
    }
    return sp2C;
}

s32 kwanm_seton_ground(s32 objectIndex) {
    s32 sp2C;

    sp2C = 0;
    kwanm_clrflag(objectIndex, 0x00800000);
    if (kwanm_bitcheckflag(objectIndex, 0x00000400) != 0) {
        CheckBump(&D_8018C3B0, 10.0f, g_DynamicObjects[objectIndex].pos[0], 20.0f,
                                 g_DynamicObjects[objectIndex].pos[2]);
        if (D_8018C3B0.flag_zx == 1) {
            sp2C = 1;
            kwanm_setflag(objectIndex, 0x00800000);
        }
        g_DynamicObjects[objectIndex].ground = CalcHeight(
            g_DynamicObjects[objectIndex].pos[0], 0.0f, g_DynamicObjects[objectIndex].pos[2], D_8018C3B0.last_zx);
        g_DynamicObjects[objectIndex].kageang[0] =
            Atan2t(D_8018C3B0.bump_zx[2], D_8018C3B0.bump_zx[1]) + 0x4000;
        g_DynamicObjects[objectIndex].kageang[2] = Atan2t(D_8018C3B0.bump_zx[0], D_8018C3B0.bump_zx[1]);
    }
    return sp2C;
}

s32 kwanm_seton_groundbump(s32 objectIndex) {
    s32 sp2C;

    sp2C = 0;
    kwanm_clrflag(objectIndex, 0x00800000);
    if (kwanm_bitcheckflag(objectIndex, 0x00000400) != 0) {
        CheckBump(&D_8018C3B0, 10.0f, g_DynamicObjects[objectIndex].pos[0], 20.0f,
                                 g_DynamicObjects[objectIndex].pos[2]);
        if (D_8018C3B0.flag_zx == 1) {
            sp2C = 1;
            kwanm_setflag(objectIndex, 0x00800000);
        }
        g_DynamicObjects[objectIndex].ground = CalcHeight(
            g_DynamicObjects[objectIndex].pos[0], 0.0f, g_DynamicObjects[objectIndex].pos[2], D_8018C3B0.last_zx);
        g_DynamicObjects[objectIndex].pos_ende[0] = D_8018C3B0.bump_zx[0];
        g_DynamicObjects[objectIndex].pos_ende[1] = D_8018C3B0.bump_zx[1];
        g_DynamicObjects[objectIndex].pos_ende[2] = D_8018C3B0.bump_zx[2];
    }
    return sp2C;
}

void kwanm_setkage_params(s32 objectIndex) {
    CheckBump(&D_8018C3B0, 10.0f, g_DynamicObjects[objectIndex].pos[0], 20.0f,
                             g_DynamicObjects[objectIndex].pos[2]);
    if (D_8018C3B0.flag_zx == 1) {
        kwanm_setflag(objectIndex, 0x00800000);
        g_DynamicObjects[objectIndex].ground = CalcHeight(
            g_DynamicObjects[objectIndex].pos[0], 0.0f, g_DynamicObjects[objectIndex].pos[2], D_8018C3B0.last_zx);
        g_DynamicObjects[objectIndex].kageang[0] =
            Atan2t(D_8018C3B0.bump_zx[2], D_8018C3B0.bump_zx[1]) + 0x4000;
        g_DynamicObjects[objectIndex].kageang[2] = Atan2t(D_8018C3B0.bump_zx[0], D_8018C3B0.bump_zx[1]);
        return;
    }
    kwanm_clrflag(objectIndex, 0x00800000);
}

void kwanm_sethole_params(s32 objectIndex) {
    CheckBump(&D_8018C3B0, 10.0f, g_DynamicObjects[objectIndex].pos[0], 20.0f,
                             g_DynamicObjects[objectIndex].pos[2]);
    if (D_8018C3B0.flag_zx == 1) {
        kwanm_setflag(objectIndex, 0x00800000);
        g_DynamicObjects[objectIndex].ground = CalcHeight(
            g_DynamicObjects[objectIndex].pos[0], 0.0f, g_DynamicObjects[objectIndex].pos[2], D_8018C3B0.last_zx);
        g_DynamicObjects[objectIndex].velocity[0] = D_8018C3B0.bump_zx[0];
        g_DynamicObjects[objectIndex].velocity[1] = D_8018C3B0.bump_zx[1];
        g_DynamicObjects[objectIndex].velocity[2] = D_8018C3B0.bump_zx[2];
        return;
    }
    kwanm_clrflag(objectIndex, 0x00800000);
}

/**
 * @brief Returns the distance between the object and the player.
 *
 * @param objectIndex
 * @param player
 * @return UNUSED
 */
UNUSED s32 kwanm_calc_distance_xz(s32 objectIndex, Player* player) {
    s32 x;
    s32 y;

    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    y = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    return (x * x) + (y * y);
}

/**
 * @brief Returns the distance between the object and the player.
 *
 * @param objectIndex
 * @param player
 * @return UNUSED
 */
UNUSED s32 kwanm_calc_distance_xyz(s32 objectIndex, Player* player) {
    s32 x;
    s32 z;
    s32 y;

    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    y = g_DynamicObjects[objectIndex].pos[1] - player->position[1];
    z = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    return (x * x) + (y * y) + (z * z);
}

/**
 * @brief Returns the distance between the object and the camera.
 *
 * @param objectIndex
 * @param camera
 * @return UNUSED
 */
u32 kwanm_calc_distance_camera_xz(s32 objectIndex, Camera* camera) {
    s32 x;
    s32 y;

    x = g_DynamicObjects[objectIndex].pos[0] - camera->camera_pos[0];
    y = g_DynamicObjects[objectIndex].pos[2] - camera->camera_pos[2];
    return (x * x) + (y * y);
}

/**
 * @brief Returns the distance between the object and the camera.
 *
 * @param objectIndex
 * @param camera
 * @return UNUSED
 */
UNUSED s32 kwanm_calc_distance_camera_xyz(s32 objectIndex, Camera* camera) {
    s32 x;
    s32 z;
    s32 y;

    x = g_DynamicObjects[objectIndex].pos[0] - camera->camera_pos[0];
    y = g_DynamicObjects[objectIndex].pos[1] - camera->camera_pos[1];
    z = g_DynamicObjects[objectIndex].pos[2] - camera->camera_pos[2];
    return (x * x) + (y * y) + (z * z);
}

/**
 * @brief Returns if the object is closer than the distance to the player.
 *
 * @param objectIndex
 * @param player
 * @param distance
 * @return true
 * @return false
 */
bool kwanm_checkradius_player_xz(s32 objectIndex, Player* player, f32 distance) {
    f32 x;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    y = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    if (((x * x) + (y * y)) <= (distance * distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is between the distance_min and distance_max to the player.
 *
 * @param objectIndex
 * @param player
 * @param distance_min
 * @param distance_max
 * @return true
 * @return false
 */
bool kwanm_checkradius2_player_xz(s32 objectIndex, Player* player, f32 distance_min, f32 distance_max) {
    f32 x;
    f32 distance;
    f32 z;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    z = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    distance = (x * x) + (z * z);
    if (((distance_min * distance_min) <= distance) && (distance <= (distance_max * distance_max))) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is closer than the distance to the player.
 *
 * @param objectIndex
 * @param player
 * @param distance
 * @return true
 * @return false
 */
bool kwanm_checkradius_player_xyz(s32 objectIndex, Player* player, f32 distance) {
    f32 x;
    f32 z;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    y = g_DynamicObjects[objectIndex].pos[1] - player->position[1];
    z = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    if (((x * x) + (y * y) + (z * z)) <= (distance * distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is between the distance_min and distance_max to the camera.
 *
 * @param objectIndex
 * @param camera
 * @param distance
 * @return true
 * @return false
 */
bool kwanm_checkradius_camera_xz(s32 objectIndex, Camera* camera, f32 distance) {
    f32 x;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - camera->camera_pos[0];
    y = g_DynamicObjects[objectIndex].pos[2] - camera->camera_pos[2];
    if (((x * x) + (y * y)) <= (distance * distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is between the distance_min and distance_max to the camera.
 *
 * @param objectIndex
 * @param camera
 * @param distance_min
 * @param distance_max
 * @return UNUSED
 */
UNUSED bool kwanm_checkradius2_camera_xz(s32 objectIndex, Camera* camera, f32 distance_min, f32 distance_max) {
    f32 x;
    f32 distance;
    f32 z;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - camera->camera_pos[0];
    z = g_DynamicObjects[objectIndex].pos[2] - camera->camera_pos[2];
    distance = (x * x) + (z * z);
    if (((distance_min * distance_min) <= distance) && (distance <= (distance_max * distance_max))) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object is closer than the distance to the camera.
 *
 * @param objectIndex
 * @param camera
 * @param distance
 * @return UNUSED
 */
UNUSED bool kwanm_checkradius_camera_xyz(s32 objectIndex, Camera* camera, f32 distance) {
    f32 x;
    f32 z;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - camera->camera_pos[0];
    y = g_DynamicObjects[objectIndex].pos[1] - camera->camera_pos[1];
    z = g_DynamicObjects[objectIndex].pos[2] - camera->camera_pos[2];
    if (((x * x) + (y * y) + (z * z)) <= (distance * distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the player.
 *
 * @param objectIndex
 * @param player
 * @return true
 * @return false
 */
bool kwanm_checkcoll_player_xz(s32 objectIndex, Player* player) {
    f32 x;
    f32 distance;
    f32 z;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    z = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    distance = g_DynamicObjects[objectIndex].r + player->radius;
    if (((x * x) + (z * z)) <= (distance * distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the player.
 *
 * @param objectIndex
 * @param player
 * @return true
 * @return false
 */
bool kwanm_checkcoll_player_xyz(s32 objectIndex, Player* player) {
    f32 x;
    f32 z;
    f32 distance;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    y = g_DynamicObjects[objectIndex].pos[1] - player->position[1];
    z = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    distance = g_DynamicObjects[objectIndex].r + player->radius;
    if (((x * x) + (y * y) + (z * z)) <= (distance * distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the player.
 *
 * @param objectIndex
 * @param player
 * @param distance
 * @return UNUSED
 */
UNUSED bool kwanm_checkhight_player(s32 objectIndex, Player* player, f32 distance) {
    f32 x;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[1] - player->position[1];
    if ((x * x) <= (distance * distance)) {
        var_v1 = true;
    }
    return var_v1;
}

/**
 * @brief Returns if the object collided with the player and is closer than the distance in the y axis.
 *
 * @param objectIndex
 * @param player
 * @param distance_y
 * @return true
 * @return false
 */
bool kwanm_checkcoll_pillar_player(s32 objectIndex, Player* player, f32 distance_y) {
    f32 x;
    f32 z;
    f32 distance;
    f32 y;
    bool var_v1;

    var_v1 = false;
    x = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    y = g_DynamicObjects[objectIndex].pos[1] - player->position[1];
    z = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    distance = g_DynamicObjects[objectIndex].r + player->radius;
    // abs(y)
    if (y < 0.0f) {
        y = -y;
    }
    if ((((x * x) + (z * z)) <= (distance * distance)) && (y <= distance_y)) {
        var_v1 = true;
    }
    return var_v1;
}

f32 kwanm_setheight_ground(s32 objectIndex, Player* player) {
    f32 distance;

    distance = g_DynamicObjects[objectIndex].pos[1] - player->ground;
    // abs(distance)
    if (distance < 0.0f) {
        distance = -distance;
    }
    return distance;
}

UNUSED bool kwanm_checkheight_ground(s32 objectIndex, Player* player, f32 arg2) {
    f32 distance;
    bool var_v1;

    distance = g_DynamicObjects[objectIndex].pos[1] - player->ground;
    var_v1 = false;
    // abs(distance)
    if (distance < 0.0f) {
        distance = -distance;
    }
    if (distance <= arg2) {
        var_v1 = true;
    }
    return var_v1;
}

void kwstop_kart(Player* player) {
    player->force = 0.0f;
    player->accelcount = 0.0f;
    player->velocity[0] = 0.0f;
    player->velocity[2] = 0.0f;
}

UNUSED void kwclear_accel_kart(Player* player) {
    player->force = 0.0f;
    player->accelcount = 0.0f;
}

void kwchase_boundvel_sub(s32 playerId, f32* arg1) {
    f32 var_f0;
    f32 var_f2;
    Player* player = &gPlayerOne[playerId];

    if (*arg1 >= 0.0f) {
        var_f2 = *arg1;
    } else {
        var_f2 = -*arg1;
    }
    if (player->slip_flag & (SPIN_L | SPIN_R)) {
        if (g_courseID == COURSE_SHERBET_LAND) {
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
    } else if (g_courseID == COURSE_SHERBET_LAND) {
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

void kwchase_boundvel_zero(s32 playerId) {
    kwchase_boundvel_sub(playerId, &D_8018CE10[playerId].unk_04[0]);
    kwchase_boundvel_sub(playerId, &D_8018CE10[playerId].unk_04[2]);
    if (D_8018CE10[playerId].unk_18[6] > 0) {
        D_8018CE10[playerId].unk_18[6]--;
    }
}

f32 kwrebound_kartplas(Player* player, s32 objectIndex, f32 arg2, f32 arg3) {
    f32 var_f2;
    f32 something;
    s32 playerId;
    AnmObject* object;
    struct_D_8018CE10* temp_v1;

    playerId = player - gPlayerOne;
    temp_v1 = &D_8018CE10[playerId];
    var_f2 = 0.0f;
    if (temp_v1->unk_18[6] == 0) {
        object = &g_DynamicObjects[objectIndex];
        player->hit_flag |= CRITTER_TOUCH;
        player->slip_flag |= CARHIT;
        temp_v1->unk_18[6] = 4;
        something = (player->position[0] - object->pos[0]) * object->velocity[0];
        if (something >= 0.0f) {
            temp_v1->unk_04[0] = (-player->velocity[0] * arg2) + (object->velocity[0] * arg3);
        } else {
            temp_v1->unk_04[0] = -player->velocity[0] * arg2;
        }
        something = (player->position[2] - object->pos[2]) * object->velocity[2];
        if (something >= 0.0f) {
            temp_v1->unk_04[2] = (-player->velocity[2] * arg2) + (object->velocity[2] * arg3);
        } else {
            temp_v1->unk_04[2] = -player->velocity[2] * arg2;
        }
        var_f2 = (temp_v1->unk_04[0] * temp_v1->unk_04[0]) + (temp_v1->unk_04[2] * temp_v1->unk_04[2]);
    }
    return var_f2;
}

void kwreboundsnd_player(s32 objectIndex, s32 playerId, f32 arg2, f32 arg3, u32 soundBits) {
    UNUSED s32 stackPadding;
    Player* player;

    player = &gPlayerOne[playerId];
    if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
        kwlost_ghost();
    }
    if ((kwrebound_kartplas(player, objectIndex, arg2, arg3) >= 4.0) && ((player->flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
        NAPlyTrgStart(playerId, soundBits);
    }
}

void kwreboundsnd_player_nolostghost(s32 objectIndex, s32 playerId, f32 arg2, f32 arg3, u32 soundBits) {
    UNUSED s32 stackPadding;
    Player* player;

    player = &gPlayerOne[playerId];
    if ((kwrebound_kartplas(player, objectIndex, arg2, arg3) >= 4.0) && ((player->flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
        NAPlyTrgStart((u8) playerId, soundBits);
    }
}

s32 kwhitcheck_player(s32 objectIndex) {
    Player* player;
    s32 playerIndex;
    s32 var_s6;

    var_s6 = 0;
    player = gPlayerOne;
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) &&
                (kwanm_checkcoll_player_xz(objectIndex, player) != 0)) {
                if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                    kwlost_ghost();
                }
                var_s6 = 1;
            }
        }
    }
    return var_s6;
}

void kwcheck_rebound_player_2D(s32 objectIndex, f32 arg1, f32 arg2) {
    Player* player;
    s32 playerIndex;

    player = gPlayerOne;
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) && !(player->slip_flag & (STAR | TERESA)) &&
                (kwanm_checkcoll_player_xz(objectIndex, player) != 0)) {
                kwrebound_kartplas(player, objectIndex, arg1, arg2 * 1.1);
                if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                    kwlost_ghost();
                }
            }
        }
    }
}

void kwcheck_reboundsnd_player_2D(s32 objectIndex, f32 arg1, f32 arg2, u32 arg3) {
    Player* player;
    s32 playerIndex;

    player = gPlayerOne;
    kwanm_clrflag(objectIndex, 0x02000000);
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) && !(player->slip_flag & TERESA)) {
                if ((player->flag & EXISTS) && !(player->flag & IS_GHOST) &&
                    (kwanm_checkcoll_player_xz(objectIndex, player) != 0)) {
                    if (player->slip_flag & STAR) {
                        kwanm_setflag(objectIndex, 0x02000000);
                    } else {
                        if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                            kwlost_ghost();
                        }
                        if ((kwrebound_kartplas(player, objectIndex, arg1, arg2 * 1.1) >= 4.0) &&
                            ((player->flag & IS_CPU_PLAYER) != IS_CPU_PLAYER)) {
                            NAPlyTrgStart(playerIndex, arg3);
                        }
                    }
                }
            }
        }
    }
}

void kwcheck_rebound_player_3D(s32 objectIndex, f32 arg1, f32 arg2) {
    Player* player;
    s32 playerIndex;

    player = gPlayerOne;
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) && !(player->slip_flag & (TERESA | STAR)) &&
                (kwanm_checkcoll_player_xyz(objectIndex, player) != 0)) {
                kwrebound_kartplas(player, objectIndex, arg1, arg2 * 1.1);
                if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                    kwlost_ghost();
                }
            }
        }
    }
}

s32 kwcheck_bomb_player_2D(s32 objectIndex) {
    Player* player;
    s32 sp40;
    s32 playerIndex;
    s32 test;

    test = 0;
    sp40 = 0;
    player = gPlayerOne;
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++, test++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) && !(player->slip_flag & (TERESA | EXPLODE)) &&
                (player->flag & EXISTS) && !(player->flag & IS_GHOST) &&
                (kwanm_checkcoll_player_xz(objectIndex, player) != 0)) {
                if (!(player->slip_flag & STAR)) {
                    player->weapon |= HIT_BOMB;
                    if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                        kwlost_ghost();
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

s32 kwcheck_bomb_player_3D(s32 objectIndex, f32 arg1) {
    Player* player;
    s32 playerIndex;
    s32 var_s7;

    var_s7 = 0;
    player = gPlayerOne;
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) && !(player->slip_flag & (TERESA | EXPLODE))) {
                if ((player->flag & EXISTS) && !(player->flag & IS_GHOST) &&
                    (kwanm_checkcoll_pillar_player(objectIndex, player, arg1) != 0)) {
                    if (!(player->slip_flag & STAR)) {
                        player->weapon |= HIT_BOMB;
                        if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                            kwlost_ghost();
                        }
                    }
                    var_s7 = 1;
                }
            }
        }
    }
    return var_s7;
}

s32 kwcheck_slip_player(s32 objectIndex) {
    Player* player;
    s32 playerIndex;
    s32 var_s6;

    var_s6 = 0;
    player = gPlayerOne;
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) &&
                !(player->slip_flag & (TERESA | SPIN_L | SPIN_R)) &&
                (kwanm_checkcoll_player_xz(objectIndex, player) != 0)) {
                if (player->slip_flag & STAR) {
                    var_s6 = 1;
                } else {
                    player->weapon |= HIT_BANANA;
                }
            }
        }
    }
    return var_s6;
}

s32 kwcheck_slip_kani_player(s32 objectIndex) {
    Player* player;
    s32 playerIndex;
    s32 var_s7;

    var_s7 = 0;
    player = gPlayerOne;
    if (kwanm_bitcheckflag(objectIndex, 0x00000200) != 0) {
        for (playerIndex = 0; playerIndex < kwkartnum; playerIndex++, player++) {
            if ((g_DynamicObjects[objectIndex].anmptr != 0) &&
                !(player->slip_flag & (TERESA | STAR | SPIN_L | SPIN_R))) {
                if ((player->flag & EXISTS) && !(player->flag & IS_GHOST) &&
                    (kwanm_checkcoll_player_xz(objectIndex, player) != 0)) {
                    var_s7 = 1;
                    if (kwanm_bitcheckflag(objectIndex, 0x04000000) != 0) {
                        kwlost_ghost();
                    }
                    player->weapon |= HIT_CRITTER;
                }
            }
        }
    }
    return var_s7;
}

s32 kwanm_checkobjang_camera_xz(s32 objectIndex, Camera* camera, u16 arg2) {
    u16 temp_t3;
    s32 var_v1;

    var_v1 = 0;
    temp_t3 = (((u16) camera->camera_direction[1] - g_DynamicObjects[objectIndex].ang[1]) + (arg2 >> 1));

    //! @warning Always true
    if ((temp_t3 >= 0) && (arg2 >= temp_t3)) {
        var_v1 = 1;
    }
    return var_v1;
}

s32 kwanm_vischeck_player_xz(s32 objectIndex, Player* player, Camera* camera, u16 arg3) {
    u16 temp_t3;
    f32 x_diff;
    f32 z_diff;
    s32 var_t0;

    var_t0 = 0;
    x_diff = g_DynamicObjects[objectIndex].pos[0] - player->position[0];
    z_diff = g_DynamicObjects[objectIndex].pos[2] - player->position[2];
    temp_t3 = ((u16) camera->camera_direction[1] - Atan2t(x_diff, z_diff)) + (arg3 >> 1);
    if ((temp_t3 >= 0) && (arg3 >= temp_t3)) {
        var_t0 = 1;
    }
    return var_t0;
}

bool kwanm_vischeck_camera_xz(s32 objectIndex, Camera* camera, u16 angle) {
    u16 temp_t2;
    s32 var_t0;

    var_t0 = false;
    temp_t2 = (kwcalc_destangley(camera->camera_pos[0], g_DynamicObjects[objectIndex].pos[0], camera->camera_pos[2],
                                    g_DynamicObjects[objectIndex].pos[2]) +
               ((s32) angle / 2)) -
              camera->camera_direction[1];
    if ((temp_t2 >= 0) && (angle >= temp_t2)) {
        var_t0 = true;
    }
    return var_t0;
}

void kwcheck_lod_texture(s32 objectIndex, s32 cameraId, s32 arg2, s32 arg3) {
    u32 temp_v0;
    u16 var_a2;
    Camera* camera;

    camera = &camera1[cameraId];
    kwanm_clrflag(objectIndex, 0x00100000 | VISIBLE);
    temp_v0 = kwanm_calc_distance_camera_xz(objectIndex, camera);
    if (temp_v0 < 0x2711U) {
        var_a2 = 0x5555;
    } else if (temp_v0 < 0x9C41U) {
        var_a2 = 0x4000;
    } else {
        var_a2 = 0x2AAB;
    }
    if ((kwanm_vischeck_camera_xz(objectIndex, camera, var_a2) != 0) && ((u32) (arg3 * arg3) >= temp_v0)) {
        kwanm_setflag(objectIndex, VISIBLE);
        if (temp_v0 >= (u32) (arg2 * arg2)) {
            kwanm_setflag(objectIndex, 0x00100000);
        }
    }
}

// This function is really cool, it tests the value of an unitialized local variable
UNUSED void kwcheck_visible(s32 objectIndex, s32 cameraId, u16 arg2) {
    Camera* camera;
    u32 no_init;
    u16 var_a2;

    camera = &camera1[cameraId];
    kwanm_clrflag(objectIndex, VISIBLE);
    if (no_init < 0x2711U) {
        var_a2 = 0x5555;
    } else if (no_init < 0x9C41U) {
        var_a2 = 0x4000;
    } else {
        var_a2 = arg2;
    }
    if (kwanm_vischeck_camera_xz(objectIndex, camera, var_a2) != 0) {
        kwanm_setflag(objectIndex, VISIBLE);
    }
}

s32 kwcheck_vis_area(s32 objectIndex, s32 cameraId, u16 arg2, s32 arg3) {
    Camera* camera;
    u32 dist;
    u16 var_a2;

    camera = &camera1[cameraId];
    kwanm_clrflag(objectIndex, 0x00020000 | VISIBLE);
    dist = kwanm_calc_distance_camera_xz(objectIndex, camera);
    if (dist < (arg3 * arg3)) {
        kwanm_setflag(objectIndex, 0x00020000);
        if (dist < 0x2711U) {
            var_a2 = 0x5555;
        } else if (dist < 0x9C41U) {
            var_a2 = 0x4000;
        } else {
            var_a2 = arg2;
        }
        if (kwanm_vischeck_camera_xz(objectIndex, camera, var_a2) != 0) {
            kwanm_setflag(objectIndex, VISIBLE);
        }
    }
    return dist;
}

void kwcheck_display_kage(s32 objectIndex, s32 cameraId, s32 arg2) {
    if (kwanm_calc_distance_camera_xz(objectIndex, &camera1[cameraId]) < (u32) (arg2 * arg2)) {
        kwanm_setflag(objectIndex, 0x00000020);
    } else {
        kwanm_clrflag(objectIndex, 0x00000020);
    }
}

void kwcheck_areain_camera(s32 objectIndex) {
    s32 loopIndex;
    Camera* camera;

    kwanm_clrflag(objectIndex, 0x00070000);
    for (loopIndex = 0, camera = camera1; loopIndex < g_playerCount; loopIndex++, camera++) {
        if (g_DynamicObjects[objectIndex].anmptr != 0) {
            if ((D_8018CF68[loopIndex] >= (g_DynamicObjects[objectIndex].unk_0DF - 1)) &&
                ((g_DynamicObjects[objectIndex].unk_0DF + 1) >= D_8018CF68[loopIndex])) {
                kwanm_setflag(objectIndex, 0x00010000);
                if (D_8018CF68[loopIndex] == g_DynamicObjects[objectIndex].unk_0DF) {
                    kwanm_setflag(objectIndex, 0x00020000);
                }
                if (kwanm_vischeck_camera_xz(objectIndex, camera, 0x2AABU) != 0) {
                    kwanm_setflag(objectIndex, VISIBLE);
                }
            }
        }
    }
}

UNUSED void kwcheck_visible_camera(s32 objectIndex) {
    s32 loopIndex;
    Camera* camera;

    kwanm_clrflag(objectIndex, VISIBLE);
    for (camera = camera1, loopIndex = 0; loopIndex < g_playerCount; loopIndex++, camera++) {
        if ((g_DynamicObjects[objectIndex].anmptr != 0) && (kwanm_vischeck_camera_xz(objectIndex, camera, 0x2AABU) != 0)) {
            kwanm_setflag(objectIndex, VISIBLE);
        }
    }
}

void kwcheck_visible_area_camera(s32 objectIndex, f32 arg1) {
    u16 var_a2;
    s32 loopIndex;
    Camera* camera;

    kwanm_clrflag(objectIndex, 0x00060000);
    for (camera = camera1, loopIndex = 0; loopIndex < g_playerCount; loopIndex++, camera++) {
        if ((g_DynamicObjects[objectIndex].anmptr != 0) &&
            (kwanm_checkradius_camera_xz(objectIndex, camera, arg1) != 0)) {
            kwanm_setflag(objectIndex, 0x00020000);
            if (arg1 <= 500.0) {
                var_a2 = 0x4000;
            } else {
                var_a2 = 0x2AAB;
            }
            if (kwanm_vischeck_camera_xz(objectIndex, camera, var_a2) != 0) {
                kwanm_setflag(objectIndex, VISIBLE);
            }
        }
    }
}

UNUSED void kwcheck_visible_near_camera(s32 arg0) {
    kwcheck_visible_area_camera(arg0, 500.0f);
}

UNUSED void kwcheck_visible_middle_camera(s32 arg0) {
    kwcheck_visible_area_camera(arg0, 1000.0f);
}

UNUSED void kwcheck_visible_far_camera(s32 arg0) {
    kwcheck_visible_area_camera(arg0, 2000.0f);
}

UNUSED s32 kwcheck_maparea_kart(Player* player) {
    return CheckArea(player->bump.last_zx);
}

s32 kwcheck_maparea_camera(Camera* camera) {
    return CheckArea(camera->bump.last_zx);
}

s32 kwcheck_mapareain(s16 arg0, s16 arg1) {
    s32 var_v1;
    s16* var_v0;
    s32 i;
    var_v1 = 0;
    for (i = 0; i < g_playerCount; i++) {
        var_v0 = &D_8018CF68[i];
        if ((*var_v0 >= arg0) && (arg1 >= *var_v0)) {
            var_v1 = 1;
        }
    }
    return var_v1;
}

void kwmv_setvel_line(s32 objectIndex) {
    SplineControlPoint* temp_v0;

    temp_v0 = g_DynamicObjects[objectIndex].sptbaddr;
    g_DynamicObjects[objectIndex].velocity[0] = (f32) (temp_v0[1].pos[0] - temp_v0[0].pos[0]) / (f32) temp_v0[0].velocity;
    g_DynamicObjects[objectIndex].velocity[1] = (f32) (temp_v0[1].pos[1] - temp_v0[0].pos[1]) / (f32) temp_v0[0].velocity;
    g_DynamicObjects[objectIndex].velocity[2] = (f32) (temp_v0[1].pos[2] - temp_v0[0].pos[2]) / (f32) temp_v0[0].velocity;
}

void kwmv_nextlinepoint(s32 objectIndex) {
    UNUSED s32 temp_t9;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->sptbaddr++;
    object->spstepadd = (s16) (10000 / (s16) (object->sptbaddr[0].velocity));
    object->spstep = 0;
    kwmv_setvel_line(objectIndex);
}

void kwmv_line3D_init(s32 objectIndex) {
    AnmObject* object;
    object = &g_DynamicObjects[objectIndex];
    object->sptbaddr = object->sptbtopaddr->controlPoints;
    object->stack[9] = 0;
    object->spstep = 0;
    /*
    This is INCREDIBLY stupid. This should really be
    temp_v0->stack[8] = temp_v0->sptbtopaddr->numControlPoints;
    but for some unholy reason that doesn't match
    */
    object->stack[8] = *((s16*) object->sptbaddr - 1);
    object->pos_spline[0] = object->sptbaddr[0].pos[0];
    object->pos_spline[1] = object->sptbaddr[0].pos[1];
    object->pos_spline[2] = object->sptbaddr[0].pos[2];
    object->spstepadd = (s16) (10000 / object->sptbaddr[0].velocity);
    kwmv_setvel_line(objectIndex);
    KWAnmNext(objectIndex);
}

void kwmv_line3D(s32 objectIndex) {
    UNUSED s16 temp_t3;
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->pos_spline[0] += object->velocity[0];
    object->pos_spline[1] += object->velocity[1];
    object->pos_spline[2] += object->velocity[2];
    object->spstep += (u16) object->spstepadd;
    if (object->spstep >= 0x2710) {
        object->stack[9] = (u16) object->stack[9] + 1;
        if (((u16) object->stack[9] + 1) == (u16) object->stack[8]) {
            object->mvptr += 1;
        } else {
            kwmv_nextlinepoint(objectIndex);
        }
    }
}

UNUSED void kwst_strategy3D(s32 arg0) {
    switch (g_DynamicObjects[arg0].mvptr) {
        case 1:
            kwmv_line3D_init(arg0);
            break;
        case 2:
            kwmv_line3D(arg0);
            break;
        case 3:
            kwmv_finish(arg0);
            /* fallthrough */
        case 0:
            break;
    }
}

UNUSED void kwst_strategy3D_loop(s32 arg0) {
    switch (g_DynamicObjects[arg0].mvptr) {
        case 1:
            kwmv_line3D_init(arg0);
            break;
        case 2:
            kwmv_line3D(arg0);
            break;
        case 3:
            kwmv_jump(arg0, 1);
            /* fallthrough */
        case 0:
            break;
    }
}

UNUSED void kwbsplinemat1(void) {
}

UNUSED void kwbsplinemat2(void) {
}

UNUSED void kwbsplinematl2(void) {
}

UNUSED void kwbsplinematl1(void) {
}

// Evaluate the Bernstein polynomials for a B-spline at `arg1` (which is `t`)
void kwbsplinematn(f32 arg0[], f32 arg1) {
    arg0[0] = (f32) ((f64) ((f32) (1.0 - arg1) * (f32) (1.0 - arg1) * (f32) (1.0 - arg1)) / 6.0);
    arg0[1] = (f32) ((((f64) (arg1 * arg1 * arg1) * 0.5) - arg1 * arg1) + 0.6666666666666666);
    arg0[2] = (f32) (((f64) (arg1 * arg1 * arg1) * -0.5) + (0.5 * (arg1 * arg1)) + (0.5 * arg1) + 0.16666666666666666);
    arg0[3] = (f32) ((f64) (arg1 * arg1 * arg1) / 6.0);
}

UNUSED void kwbsplinevelmat1(void) {
}

UNUSED void kwbsplinevelmat2(void) {
}

// Evaluate the Bernstein polynomials for the derivative of a B-spline at `arg1` (which is `t`)
void kwbsplinevelmatn(f32 arg0[], f32 arg1) {
    arg0[0] = (f32) (1.0 - arg1) * -0.5 * (f32) (1.0 - arg1);
    arg0[1] = arg1 * arg1 * 1.5 - 2.0 * arg1;
    arg0[2] = (arg1 * arg1 * 3.0 - 2.0 * arg1 - (f32) 1.0) * -0.5;
    arg0[3] = arg1 * arg1 * 0.5;
}

UNUSED void kwbsplinevelmatl2(void) {
}

UNUSED void kwbsplinevelmatl1(void) {
}

// D_80183DC8 is where the spline's derivative polynomial calculations are stored
void kwset3Dsplinevel(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->velocity[0] = (D_80183DC8[0] * D_80165760[0]) + (D_80183DC8[1] * D_80165760[1]) +
                          (D_80183DC8[2] * D_80165760[2]) + (D_80183DC8[3] * D_80165760[3]);
    object->velocity[1] = (D_80183DC8[0] * D_80165770[0]) + (D_80183DC8[1] * D_80165770[1]) +
                          (D_80183DC8[2] * D_80165770[2]) + (D_80183DC8[3] * D_80165770[3]);
    object->velocity[2] = (D_80183DC8[0] * D_80165780[0]) + (D_80183DC8[1] * D_80165780[1]) +
                          (D_80183DC8[2] * D_80165780[2]) + (D_80183DC8[3] * D_80165780[3]);
}

void kw3Dsplinevel(s32 objectIndex, f32 arg1) {
    kwbsplinevelmatn(D_80183DC8, arg1);
    kwset3Dsplinevel(objectIndex);
}

UNUSED void kw3Dsplinevel1(void) {
}

UNUSED void kw3Dsplinevel2(void) {
}

UNUSED void kw3Dsplinevell2(void) {
}

UNUSED void kw3Dsplinevell1(void) {
}

// D_80183DA8 is where the spline's polynomial calculations are stored
void kwset3Dsplinezahyou(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->pos_spline[0] = (D_80183DA8[0] * D_80165760[0]) + (D_80183DA8[1] * D_80165760[1]) +
                        (D_80183DA8[2] * D_80165760[2]) + (D_80183DA8[3] * D_80165760[3]);
    object->pos_spline[1] = (D_80183DA8[0] * D_80165770[0]) + (D_80183DA8[1] * D_80165770[1]) +
                        (D_80183DA8[2] * D_80165770[2]) + (D_80183DA8[3] * D_80165770[3]);
    object->pos_spline[2] = (D_80183DA8[0] * D_80165780[0]) + (D_80183DA8[1] * D_80165780[1]) +
                        (D_80183DA8[2] * D_80165780[2]) + (D_80183DA8[3] * D_80165780[3]);
}

void kw3Dsplinezahyou(s32 objectIndex, f32 arg1) {
    kwbsplinematn(D_80183DA8, arg1);
    kwset3Dsplinezahyou(objectIndex);
}

UNUSED void kw3Dsplinezahyou1(void) {
}

UNUSED void kw3Dsplinezahyou2(void) {
}

UNUSED void kw3Dsplinezahyoul2(void) {
}

UNUSED void kw3Dsplinezahyoul1(void) {
}

void kwmv_setsplinepoint(s32 objectIndex) {
    s32 someIndex;
    SplineControlPoint* test;

    test = g_DynamicObjects[objectIndex].sptbaddr;
    for (someIndex = 0; someIndex < 4; someIndex++) {
        D_80165760[someIndex] = test->pos[0];
        D_80165770[someIndex] = test->pos[1];
        D_80165780[someIndex] = test->pos[2];
        test++;
    }
}

void kwmv_setsplinepoint_loop(s32 objectIndex) {
    s32 someIndex;
    s32 sp0;
    s32 temp_a1;
    s32 temp_a2;
    SplineControlPoint* test;

    test = g_DynamicObjects[objectIndex].sptbaddr;
    temp_a1 = g_DynamicObjects[objectIndex].stack[9];
    temp_a2 = (u16) g_DynamicObjects[objectIndex].stack[8];
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
            // Reset back to start of the spline path
            test = g_DynamicObjects[objectIndex].sptbtopaddr->controlPoints;
        } else {
            test++;
        }
    }
}

void kwmv_spline3D_init(s32 objectIndex) {
    AnmObject* object;
    UNUSED SplineData* spline;

    if (kwanm_bitoffcheckflag(objectIndex, 8) != 0) {
        object = &g_DynamicObjects[objectIndex];
        object->stack[9] = 0;
        object->spstep = 0;
        object->sptbaddr = object->sptbtopaddr->controlPoints;
        /*
        This is INCREDIBLY stupid. This should really be
        temp_v0->stack[8] = temp_v0->sptbtopaddr->numControlPoints;
        but for some unholy reason that doesn't match
        */
        object->stack[8] = *(((s16*) object->sptbaddr) - 1);

        kwanm_setflag(objectIndex, 8);
    }
}

void kwmv_nextsplinepoint(s32 objectIndex) {
    g_DynamicObjects[objectIndex].spstep = 0;
    g_DynamicObjects[objectIndex].sptbaddr++;
}

void kwmv_spline3D_common(s32 objectIndex, s32 arg1) {
    f32 sp34;
    f32 temp;
    UNUSED f32 temp2;
    f32 var_f6;

    kwmv_spline3D_init(objectIndex);
    if (arg1 != 0) {
        kwmv_setsplinepoint_loop(objectIndex);
    } else {
        kwmv_setsplinepoint(objectIndex);
    }

    // I think the game treats each spline as being having a lenght of 10000
    // This is getting the percent along the spline we want to reach,
    // which is then treated as the `t` value given to the curve calculations
    sp34 = ((f32) g_DynamicObjects[objectIndex].spstep / 10000.0);
    // Calculate the curve at `t`
    kw3Dsplinezahyou(objectIndex, sp34);
    if (kwanm_bitcheckflag(objectIndex, 0x800) != 0) {
        // Calculate the curve's derivative at `t`
        kw3Dsplinevel(objectIndex, sp34);
    }

    // These values somehow control how fast we travel along the curve
    var_f6 = g_DynamicObjects[objectIndex].sptbaddr[0].velocity;
    temp = g_DynamicObjects[objectIndex].sptbaddr[1].velocity;

    g_DynamicObjects[objectIndex].spstepadd = 10000.0 / (((temp - var_f6) * sp34) + var_f6);
    g_DynamicObjects[objectIndex].spstep += g_DynamicObjects[objectIndex].spstepadd;
}

void kwmv_spline3D(s32 objectIndex) {
    UNUSED s16 temp_t0;
    AnmObject* object;

    kwmv_spline3D_common(objectIndex, 0);
    object = &g_DynamicObjects[objectIndex];
    if (object->spstep >= 0x2710) {
        // Have to do it this way due to the u16 cast
        object->stack[9] = (u16) object->stack[9] + 1;
        if (((u16) object->stack[9] + 3) == (u16) object->stack[8]) {
            object->mvptr += 1;
        } else {
            kwmv_nextsplinepoint(objectIndex);
        }
    }
}

void kwmv_spline3D_loop(s32 objectIndex) {
    AnmObject* object;

    kwmv_spline3D_common(objectIndex, 1);
    object = &g_DynamicObjects[objectIndex];
    if (object->spstep >= 0x2710) {
        // Have to do it this way due to the u16 cast
        object->stack[9] = (u16) object->stack[9] + 1;
        if ((u16) object->stack[9] == (u16) object->stack[8]) {
            kwanm_clrflag(objectIndex, 8);
        } else {
            kwmv_nextsplinepoint(objectIndex);
        }
    }
}

void kwsp_strategy3D(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            kwmv_spline3D(objectIndex);
            break;
        case 2:
            kwmv_finish(objectIndex);
            break;
    }
}

void kwsp_strategy3D_loop(s32 objectIndex) {
    switch (g_DynamicObjects[objectIndex].mvptr) {
        case 0:
            break;
        case 1:
            kwmv_spline3D_loop(objectIndex);
            break;
    }
}

void kwset_init3D_spos(s32 objectIndex, f32 arg1, f32 arg2, f32 arg3) {
    g_DynamicObjects[objectIndex].pos_start[0] = arg1;
    g_DynamicObjects[objectIndex].pos_start[1] = arg2;
    g_DynamicObjects[objectIndex].pos_start[2] = arg3;
}

void kwset_init3D_sppos(s32 objectIndex, f32 arg1, f32 arg2, f32 arg3) {
    g_DynamicObjects[objectIndex].pos_spline[0] = arg1;
    g_DynamicObjects[objectIndex].pos_spline[1] = arg2;
    g_DynamicObjects[objectIndex].pos_spline[2] = arg3;
}

void kwset_init3D_pos(s32 objectIndex) {
    f32 temp_f0 = g_DynamicObjects[objectIndex].pos_start[0];

    g_DynamicObjects[objectIndex].pos[0] = g_DynamicObjects[objectIndex].pos_spline[0] + temp_f0;
    g_DynamicObjects[objectIndex].pos[1] = g_DynamicObjects[objectIndex].pos_spline[1] + temp_f0;
    g_DynamicObjects[objectIndex].pos[2] = g_DynamicObjects[objectIndex].pos_spline[2] + temp_f0;
}

void kwset_init3D_ang(s32 objectIndex, u16 arg1, u16 arg2, u16 arg3) {
    g_DynamicObjects[objectIndex].ang[0] = arg1;
    g_DynamicObjects[objectIndex].ang[1] = arg2;
    g_DynamicObjects[objectIndex].ang[2] = arg3;
}

void kwset_init3D_texang(s32 objectIndex, u16 arg1, u16 arg2, u16 arg3) {
    g_DynamicObjects[objectIndex].texang[0] = arg1;
    g_DynamicObjects[objectIndex].texang[1] = arg2;
    g_DynamicObjects[objectIndex].texang[2] = arg3;
}

void kwset_init3D_vel(s32 objectIndex, f32 arg1, f32 arg2, f32 arg3) {
    g_DynamicObjects[objectIndex].velocity[0] = arg1;
    g_DynamicObjects[objectIndex].velocity[1] = arg2;
    g_DynamicObjects[objectIndex].velocity[2] = arg3;
}

void kwset_init3D_posang(s32 objectIndex, s16 arg1, s16 arg2, s16 arg3, SplineData* spline) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    object->pos_start[0] = (f32) arg1;
    object->pos_start[1] = (f32) arg2;
    object->sptbtopaddr = spline;
    object->pos_start[2] = (f32) arg3;
    object->velocity[0] = (f32) (spline->controlPoints[1].pos[0] - spline->controlPoints[0].pos[0]);
    object->velocity[1] = (f32) (spline->controlPoints[1].pos[1] - spline->controlPoints[0].pos[1]);
    object->velocity[2] = (f32) (spline->controlPoints[1].pos[2] - spline->controlPoints[0].pos[2]);
    object->ang[1] = kwmv_velxztoangy(objectIndex);
    object->velocity[2] = kwrotate2D_x(object->velocity[2], object->velocity[0], -object->ang[1]);
    object->ang[0] = kwmv_velyztoangx(objectIndex);
}

UNUSED void kwset_randomx(s32 objectIndex, s16 x, u16 arg2) {
    s16 offset_x;

    offset_x = MakeRandomLimmit(arg2) - (arg2 / 2);
    g_DynamicObjects[objectIndex].pos_start[0] = x + offset_x;
}

void kwset_randomy(s32 objectIndex, s16 y, u16 arg2) {
    s16 offset_y;

    offset_y = MakeRandomLimmit(arg2) - (arg2 / 2);
    g_DynamicObjects[objectIndex].pos_start[1] = y + offset_y;
}

UNUSED void kwset_randomz(s32 objectIndex, s16 z, u16 arg2) {
    s16 offset_z;

    offset_z = MakeRandomLimmit(arg2) - (arg2 / 2);
    g_DynamicObjects[objectIndex].pos_start[2] = z + offset_z;
}

UNUSED void kwset_randomxy(s32 objectIndex, s16 x, s16 y, u16 arg3, u16 arg4) {
    s16 offset_x;
    s16 offset_y;

    offset_x = MakeRandomLimmit(arg3) - ((s32) arg3 / 2);
    offset_y = MakeRandomLimmit(arg4) - ((s32) arg4 / 2);
    g_DynamicObjects[objectIndex].pos_start[0] = x + offset_x;
    g_DynamicObjects[objectIndex].pos_start[1] = y + offset_y;
}

UNUSED void kwset_randomxz(s32 objectIndex, s16 x, s16 z, u16 arg3, u16 arg4) {
    s16 offset_x;
    s16 offset_z;

    offset_x = MakeRandomLimmit(arg3) - ((s32) arg3 / 2);
    offset_z = MakeRandomLimmit(arg4) - ((s32) arg4 / 2);
    g_DynamicObjects[objectIndex].pos_start[0] = x + offset_x;
    g_DynamicObjects[objectIndex].pos_start[2] = z + offset_z;
}

void kwset_randomxyz(s32 objectIndex, s16 x, s16 y, s16 z, u16 arg4, u16 arg5, u16 arg6) {
    s16 offset_x;
    s16 offset_y;
    s16 offset_z;

    offset_x = MakeRandomLimmit(arg4) - ((s32) arg4 / 2);
    offset_y = MakeRandomLimmit(arg5) - ((s32) arg5 / 2);
    offset_z = MakeRandomLimmit(arg6) - ((s32) arg6 / 2);
    g_DynamicObjects[objectIndex].pos_start[0] = x + offset_x;
    g_DynamicObjects[objectIndex].pos_start[1] = y + offset_y;
    g_DynamicObjects[objectIndex].pos_start[2] = z + offset_z;
}

void kwset_relplayer_xz(s32 objectIndex, s16 dist, u16 angle) {
    g_DynamicObjects[objectIndex].pos_start[0] = gPlayerOneCopy->position[0] + (sinT(angle) * dist);
    g_DynamicObjects[objectIndex].pos_start[2] = gPlayerOneCopy->position[2] + (cosT(angle) * dist);
}

UNUSED void kwset_relplayer_randomxz(s32 objectIndex, u16 arg1, u16 arg2) {
    u16 thing;
    u16 thing2;

    thing = MakeRandomLimmit(arg1);
    thing2 = camera1->camera_direction[1] + MakeRandomLimmit(arg2) - ((s32) arg2 / 2);
    kwset_relplayer_xz(objectIndex, thing, thing2);
}

void kwset_splinepos3D(s32 objectIndex) {
    g_DynamicObjects[objectIndex].pos[0] = g_DynamicObjects[objectIndex].pos_start[0] + g_DynamicObjects[objectIndex].pos_spline[0];
    g_DynamicObjects[objectIndex].pos[1] = g_DynamicObjects[objectIndex].pos_start[1] + g_DynamicObjects[objectIndex].pos_spline[1];
    g_DynamicObjects[objectIndex].pos[2] = g_DynamicObjects[objectIndex].pos_start[2] + g_DynamicObjects[objectIndex].pos_spline[2];
}

void kwset_posang3D(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    D_80183E40[0] = object->pos[0];
    D_80183E40[1] = object->pos[1];
    D_80183E40[2] = object->pos[2];
    D_80183E80[0] = object->ang[0];
    D_80183E80[1] = object->ang[1];
    D_80183E80[2] = object->ang[2];
}

void kwset_pos3Dto2D(s32 objectIndex) {
    g_DynamicObjects[objectIndex].ix = g_DynamicObjects[objectIndex].pos[0];
    g_DynamicObjects[objectIndex].iy = g_DynamicObjects[objectIndex].pos[1];
}

void kwjugemu_strategy3D(s32 objectIndex) {
    AnmObject* object;

    object = &g_DynamicObjects[objectIndex];
    switch (object->mappoint) {
        case 1:
            if (object->mvptr == 1) {
                kwmv_spline3D(objectIndex);
            }
            break;
        case 2:
            switch (object->mvptr) {
                case 0:
                    break;
                case 1:
                    kwmv_spline3D_loop(objectIndex);
                    break;
            }
            break;
        case 3:
            switch (object->mvptr) {
                case 0:
                    break;
                case 1:
                    kwmv_spline3D(objectIndex);
                    break;
            }
            break;
        case 4:
            switch (object->mvptr) { /* switch 1; irregular */
                case 0:                /* switch 1 */
                    break;
                case 1: /* switch 1 */
                    kwmv_spline3D(objectIndex);
                    break;
                case 2: /* switch 1 */
                    kwmv_finish(objectIndex);
                    break;
            }
            break;
        case 5:
            switch (object->mvptr) { /* switch 2; irregular */
                case 0:                /* switch 2 */
                    break;
                case 1: /* switch 2 */
                    kwmv_spline3D(objectIndex);
                    break;
                case 2: /* switch 2 */
                    kwmv_finish(objectIndex);
                    break;
            }
            break;
        case 6:
            switch (object->mvptr) {
                case 0:
                    break;
                case 1:
                    kwmv_spline3D(objectIndex);
                    break;
            }
            break;
        case 7:
            switch (object->mvptr) { /* switch 3; irregular */
                case 0:                /* switch 3 */
                    break;
                case 1: /* switch 3 */
                    kwmv_chase3D_init(objectIndex);
                    break;
                case 2: /* switch 3 */
                    kwmv_chase3D(objectIndex);
                    break;
            }
            break;
        case 0:
        default:
            break;
    }
}

UNUSED void kwmawaru_tellesa(UNUSED s32 arg0) {
}

UNUSED void kwmv_tellesa_oufuku(UNUSED s32 arg0) {
}
