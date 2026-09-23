#include <ultra64.h>
#include <macros.h>
#include <common_structs.h>
#include "math_util_2.h"
#include "main.h"
#include "math_util.h"
#include "objects.h"

#include "memory.h"
#include "collision.h"
#include "render_player.h"
#include "code_80057C60.h"
#include "defines.h"
#include "camera.h"

#pragma intrinsic(sqrtf)

UNUSED void KWSetflag(s32* arg0, s32 arg1) {
    *arg0 = (s32) (*arg0 | arg1);
}

UNUSED void KWClrflag(s32* arg0, s32 arg1) {
    *arg0 = (s32) (*arg0 & ~arg1);
}

UNUSED void KWEorflag(s32* arg0, s32 arg1) {
    *arg0 = (s32) (*arg0 ^ arg1);
}

UNUSED bool KWBitcheckflag(s32* arg0, s32 arg1) {
    bool phi_v1;

    phi_v1 = false;
    if ((*arg0 & arg1) != 0) {
        phi_v1 = true;
    }
    return phi_v1;
}

UNUSED s32 KWBitoffcheckflag(s32* arg0, s32 arg1) {
    s32 phi_v1;

    phi_v1 = 0;
    if ((*arg0 & arg1) == 0) {
        phi_v1 = 1;
    }
    return phi_v1;
}

void KwcopyFval(Vec3f dest, Vec3f arg1) {
    dest[0] = arg1[0];
    dest[1] = arg1[1];
    dest[2] = arg1[2];
}

s32 KwaddFval(f32* Value, f32 Target, f32 step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        *Value += step;
        if (Target <= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KwsubFval(f32* Value, f32 Target, f32 step) {
    s32 targetReached = 0;

    if (Target < *Value) {
        *Value -= step;
        if (*Value <= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KwaddIval(s32* Value, s32 Target, s32 step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        *Value = *Value + step;
        targetReached = 0;
        if (*Value >= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KwsubIval(s32* Value, s32 Target, s32 step) {
    s32 targetReached = 0;

    if (Target < *Value) {
        *Value = *Value - step;
        ;
        if (Target >= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KwaddSval(s16* Value, s16 Target, s16 step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        *Value = *Value + step;
        if (*Value >= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KwaddUsval(u16* Value, u16 Target, u16 step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        *Value += step;
        if (*Value >= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KwsubSval(s16* Value, s16 Target, s16 step) {
    s32 targetReached = 0;

    if (Target < *Value) {
        *Value -= step;
        if (Target >= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

// wtf is up with the argument types for this one?
s32 KwsubUsval(u16* Value, s32 Target, s32 step) {
    s32 targetReached = 0;
    s32 temp = *Value;

    if (Target < temp) {
        temp -= step;
        if (Target >= temp) {
            temp = Target;
            targetReached = 1;
        }
        *Value = temp;
    }
    return targetReached;
}

UNUSED s32 KwaddFvar(f32* Value, f32 Target, f32* step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        *Value += *step;
        if (Target <= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 KwsubFvar(f32* Value, f32 Target, f32* step) {
    s32 targetReached = 0;

    if (Target < *Value) {
        *Value -= *step;
        if (*Value <= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 KwaddIvar(s32* Value, s32 Target, s32* step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        *Value += *step;
        if (*Value >= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 KwsubIvar(s32* Value, s32 Target, s32* step) {
    s32 targetReached = 0;

    if (Target < *Value) {
        *Value -= *step;
        if (Target >= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 KwaddSvar(s16* Value, s16 Target, s16* step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        *Value += *step;
        if (*Value >= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 KwsubSvar(s16* Value, s16 Target, s16* step) {
    s32 targetReached = 0;

    if (Target < *Value) {
        *Value -= *step;
        if (Target >= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KWChaseSVal(s16* Value, s16 Target, s16 step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        if (step >= 0) {
            *Value += step;
        } else {
            *Value -= step;
        }
        if (*Value >= Target) {
            *Value = Target;
            targetReached = 1;
        }
    } else if (Target < *Value) {
        if (step >= 0) {
            *Value -= step;
        } else {
            *Value += step;
        }
        if (Target >= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

UNUSED s32 KWChaseIVal(s32* Value, s32 Target, s32 step) {
    s32 temp_v0;
    s32 targetReached;

    temp_v0 = *Value;
    targetReached = 0;
    if (temp_v0 < Target) {
        if (step >= 0) {
            *Value = (s32) (temp_v0 + step);
        } else {
            *Value = (s32) (temp_v0 - step);
        }
        if (*Value >= Target) {
            *Value = Target;
            targetReached = 1;
        }

    } else if (Target < temp_v0) {
        if (step >= 0) {
            *Value = (s32) (temp_v0 - step);
        } else {
            *Value = (s32) (temp_v0 + step);
        }
        if (Target >= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

s32 KWChaseFVal(f32* Value, f32 Target, f32 step) {
    s32 targetReached = 0;

    if (*Value < Target) {
        if (step >= 0.0f) {
            *Value += step;
        } else {
            *Value -= step;
        }
        if (Target <= *Value) {
            *Value = Target;
            targetReached = 1;
        }
    } else if (Target < *Value) {
        if (step >= 0.0f) {
            *Value -= step;
        } else {
            *Value += step;
        }
        if (*Value <= Target) {
            *Value = Target;
            targetReached = 1;
        }
    }
    return targetReached;
}

void KwoufukuSval(s16* arg0, s16 arg1, s16 Arg2, s16* Arg3) {
    *arg0 += *Arg3;
    if (*Arg3 >= 0) {
        if (*arg0 >= Arg2) {
            *arg0 = Arg2;
            *Arg3 = -*Arg3;
        }
    } else if (arg1 >= *arg0) {
        *arg0 = arg1;
        *Arg3 = -*Arg3;
    }
}

Vec3f* Vec3FSetXYZ(Vec3f arg0, f32 arg1, f32 Arg2, f32 Arg3) {
    arg0[0] = arg1;
    arg0[1] = Arg2;
    arg0[2] = Arg3;
    return (Vec3f*) &arg0;
}

Vec3f* NormalizeFVector(Vec3f dest) {
    f32 invsqrt = 1.0f / sqrtf(dest[0] * dest[0] + dest[1] * dest[1] + dest[2] * dest[2]);

    dest[0] = dest[0] * invsqrt;
    dest[1] = dest[1] * invsqrt;
    dest[2] = dest[2] * invsqrt;
    return (Vec3f*) &dest;
}

Vec3f* CrossProduct(Vec3f dest, Vec3f arg1, Vec3f Arg2) {

    dest[0] = (arg1[1] * Arg2[2]) - (Arg2[1] * arg1[2]);
    dest[1] = (arg1[2] * Arg2[0]) - (Arg2[2] * arg1[0]);
    dest[2] = (arg1[0] * Arg2[1]) - (Arg2[0] * arg1[1]);

    return (Vec3f*) &dest;
}

UNUSED s32 KWCheckRadiusXZ(f32 x1, f32 y1, f32 x2, f32 y2, f32 Distance) {
    f32 x;
    f32 y;
    s32 ret = 0;

    x = x2 - x1;
    y = y2 - y1;
    if (((x * x) + (y * y)) <= (Distance * Distance)) {
        ret = 1;
    }
    return ret;
}

s32 KwsetVelyztoangx(f32 arg0, f32 arg1) {
    return -Atan2t(arg0, arg1) & 0xFFFF;
}

UNUSED s32 KwsetVelxztoangy(f32 arg0, f32 arg1) {
    return Atan2t(arg1, arg0);
}

UNUSED s32 KwsetVelxytoangz(f32 arg0, f32 arg1) {
    return Atan2t(arg1, arg0);
}

f32 Kwrotate2DX(f32 x, f32 z, u16 Angle) {
    f32 cosAngle;

    cosAngle = CosT(Angle);
    return (cosAngle * x) - (SinT(Angle) * z);
}

f32 Kwrotate2DY(f32 x, f32 z, u16 Angle) {
    f32 sinAngle;

    sinAngle = SinT(Angle);
    return (CosT(Angle) * z) + (sinAngle * x);
}

s32 KwcalcDestangley(f32 x1, f32 x2, f32 y1, f32 y2) {
    return Atan2t(x2 - x1, y2 - y1);
}

u16 KwchaseAngle(u16 angle1, u16 angle2) {
    u16 out_angle;

    if ((angle1 >> 8) != (angle2 >> 8)) {
        out_angle = angle2 - angle1;
        if (out_angle < 0x400) {
            out_angle = angle1 + 0x80;
        } else if (out_angle < 0x800) {
            out_angle = angle1 + 0x200;
        } else if (out_angle < 0x4000) {
            out_angle = angle1 + 0x400;
        } else if (out_angle < 0x8000) {
            out_angle = angle1 + 0x700;
        } else if (out_angle < 0xC000) {
            out_angle = angle1 - 0x700;
        } else if (out_angle < 0xF800) {
            out_angle = angle1 - 0x400;
        } else if (out_angle < 0xFC00) {
            out_angle = angle1 - 0x200;
        } else {
            out_angle = angle1 - 0x80;
        }
    } else {
        out_angle = angle2;
    }
    return out_angle;
}

s32 KWLookCamera(f32 arg0, f32 arg1, Vec3f Arg2) {
    return Atan2t(arg0 - Arg2[0], arg1 - Arg2[2]);
}

s32 KWLookCameraPitch(f32 arg0, f32 arg1, Vec3f Arg2) {
    return Atan2t(arg0 - Arg2[1], arg1 - Arg2[2]);
}

s32 KwcalcBump(Collision* arg0, Vec3f arg1) {
    s32 ret = 0;

    CheckBump(arg0, 10.0f, arg1[0], arg1[1], arg1[2]);
    if (arg0->FlagZX == 1) {
        ret = 1;
    }
    return ret;
}

bool KwvischeckCameraXz(Vec3f arg0, Camera* arg1, u16 Arg2) {
    u16 temp_t9;
    s32 ret;

    ret = false;
    temp_t9 = (KwcalcDestangley(arg1->CameraPos[0], arg0[0], arg1->CameraPos[2], arg0[2]) + (Arg2 / 2)) - arg1->CameraDirection[1];
    if ((temp_t9 >= 0) && (Arg2 >= temp_t9)) {
        ret = true;
    }
    return ret;
}

void KwcalcLightvector(void) {
    Vec3f pos;
    Vec3f vec;

    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 120.0f;
    KwrotateYx(vec, pos, (s16*) D_80165834);
    D_80165840[0] = vec[0];
    D_80165840[1] = vec[1];
    D_80165840[2] = vec[2];
}

UNUSED void func_80041A70(void) {
}

void Translate2DMat(Mat4 arg0, s32 x, s32 y) {
    arg0[0][0] = 1.0f;
    arg0[1][1] = 1.0f;
    arg0[2][2] = 1.0f;
    arg0[1][0] = 0.0f;
    arg0[2][0] = 0.0f;
    arg0[0][1] = 0.0f;
    arg0[3][0] = x;
    arg0[2][1] = 0.0f;
    arg0[0][2] = 0.0f;
    arg0[1][2] = 0.0f;
    arg0[3][2] = 0.0f;
    arg0[3][1] = y;
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;

    /*
     * 1 0 0 x
     * 0 1 0 y
     * 0 0 1 0
     * 0 0 0 1
     */
}

void Zrotate2DMat(Mat4 dest, u16 Angle) {
    f32 sin_theta = SinT(Angle);
    f32 cos_theta = CosT(Angle);

    dest[0][0] = cos_theta;
    dest[1][0] = -sin_theta;
    dest[1][1] = cos_theta;
    dest[0][1] = sin_theta;
    dest[2][0] = 0.0f;
    dest[3][0] = 0.0f;
    dest[2][1] = 0.0f;
    dest[3][1] = 0.0f;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
}

void Scale2DMat(Mat4 dest, f32 Scale) {
    dest[1][0] = 0.0f;
    dest[2][0] = 0.0f;
    dest[3][0] = 0.0f;
    dest[0][1] = 0.0f;
    dest[2][1] = 0.0f;
    dest[3][1] = 0.0f;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
    dest[0][0] = Scale;
    dest[1][1] = Scale;
}

UNUSED void Zrotatescale2DMat(Mat4 dest, u16 Angle, f32 Scale) {
    f32 sin_theta = SinT(Angle);
    f32 cos_theta = CosT(Angle) * Scale;

    dest[2][0] = 0.0f;
    dest[0][0] = cos_theta;
    dest[1][1] = cos_theta;
    dest[2][1] = 0.0f;
    dest[1][0] = -sin_theta * Scale;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[0][1] = sin_theta * Scale;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][0] = 1.0f;
    dest[3][1] = 1.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
}

/**
 * @brief arg1 and Arg2 are s32's into floats?!?
 *
 * @Param dest
 * @Param x
 * @Param y
 * @Param Angle
 * @Param Scale
 */
void Createaffine2D(Mat4 dest, s32 x, s32 y, u16 Angle, f32 Scale) {
    f32 sin_theta = SinT(Angle);
    f32 cos_theta = CosT(Angle) * Scale;

    dest[2][0] = 0.0f;
    dest[0][0] = cos_theta;
    dest[1][0] = (-sin_theta) * Scale;
    dest[3][0] = (f32) x;
    dest[1][1] = cos_theta;
    dest[0][1] = sin_theta * Scale;
    dest[2][1] = 0.0f;
    dest[3][1] = (f32) y;
    dest[0][2] = 0.0f;
    dest[1][2] = 0.0f;
    dest[2][2] = 1.0f;
    dest[3][3] = 1.0f;
    dest[3][2] = 0.0f;
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
}

// Likely D_801658**[Index] = 1; * denotes wildcard
void KwswitchDemo2Pgp(void) {
    D_801658FE = 1;
}

void guOrtho(Mtx*, f32, f32, f32, f32, f32, f32, f32); /* extern */
extern s8 D_801658FE;

void KwsetProjection2DMatrix(void) {
    guOrtho(&D_80183D60, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
    switch (ScreenSplitA) {
        case SCREEN_MODE_1P:
            guOrtho(&DynamicP->MTXOrtho, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            guOrtho(&DynamicP->MTXOrtho, 0.0f, 160.0f, 120.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            break;
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
            if (D_801658FE == 0) {
                guOrtho(&DynamicP->MTXOrtho, 0.0f, 320.0f, 120.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            } else {
                guOrtho(&DynamicP->MTXOrtho, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            }
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            guOrtho(&DynamicP->MTXOrtho, 0.0f, 320.0f, 240.0f, 0.0f, -1.0f, 1.0f, 1.0f);
            break;
    }
}

void KW2DMatrixInit(void) {
    gDPSetTexturePersp(DisplayListHead++, G_TP_PERSP);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXOrtho),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
}

// void Affine2Mtx(Mtx*, Mat4);

UNUSED void Kwtranslate2D(s32 x, s32 y) {
    Mat4 Matrix;

    Translate2DMat(Matrix, x, y);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void KWZrotate2D(u16 AngleZ) {
    Mat4 Matrix;

    Zrotate2DMat(Matrix, AngleZ);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void Kwscale2D(f32 Scale) {
    Mat4 Matrix;

    Scale2DMat(Matrix, Scale);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void KWZrotatescale2D(u16 Angle, f32 Scale) {
    Mat4 Matrix;

    Zrotatescale2DMat(Matrix, Angle, Scale);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void Kwscaletranslate2D(s32 x, s32 y, f32 Scale) {
    Mat4 Matrix;

    Translate2DMat(Matrix, x, y);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    Scale2DMat(Matrix, Scale);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
}

void Kwcreateaffine2D(s32 x, s32 y, u16 Angle, f32 Scale) {
    Mat4 Matrix;
    Createaffine2D(Matrix, x, y, Angle, Scale);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void func_800423F0(Mat4 arg0, u16 arg1, u16 Arg2, u16 Arg3) {
    f32 sp3C;
    f32 temp_f20;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 temp_f0;

    sp3C = SinT(arg1);
    temp_f20 = CosT(arg1);
    sp34 = SinT(Arg2);
    sp30 = CosT(Arg2);
    sp2C = SinT(Arg3);
    temp_f0 = CosT(Arg3);

    arg0[0][0] = (f32) (sp30 * temp_f0 + (sp3C * sp34 * sp2C));
    arg0[1][0] = (f32) ((-sp30 * sp2C) + (sp3C * sp34 * temp_f0));
    arg0[2][0] = (f32) (temp_f20 * sp34);
    arg0[3][0] = 0.0f;
    arg0[0][1] = (f32) (temp_f20 * sp2C);
    arg0[1][1] = (f32) (temp_f20 * temp_f0);
    arg0[2][1] = (f32) -sp3C;
    arg0[3][1] = 0.0f;
    arg0[0][2] = (f32) ((-sp34 * temp_f0) + (sp3C * sp30 * sp2C));
    arg0[1][2] = (f32) ((sp34 * sp2C) + (sp3C * sp30 * temp_f0));
    arg0[2][2] = (f32) (temp_f20 * sp30);
    arg0[3][2] = 0.0f;
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;
}

UNUSED void KwrotateMTXYx(Mat4 arg0, u16 arg1, u16 Arg2) {
    f32 sp2C = SinT(arg1);
    f32 sp28 = CosT(arg1);
    f32 sin_theta_y = SinT(Arg2);
    f32 cos_theta_y = CosT(Arg2);

    arg0[1][0] = sp2C * sin_theta_y;
    arg0[2][0] = sp28 * sin_theta_y;
    arg0[0][1] = 0.0f;
    arg0[0][0] = cos_theta_y;
    arg0[2][1] = -sp2C;
    arg0[0][2] = -sin_theta_y;
    arg0[1][1] = sp28;
    arg0[1][2] = sp2C * cos_theta_y;
    arg0[2][2] = sp28 * cos_theta_y;
}

void KwcreateModelingMatrix(Mat4 transformMatrix, Vec3f translationVector, Vec3su rotationVector,
                                    f32 scalingFactor) {
    f32 sinX = SinT(rotationVector[0]);
    f32 cosX = CosT(rotationVector[0]);
    f32 sinY = SinT(rotationVector[1]);
    f32 cosY = CosT(rotationVector[1]);
    f32 sinZ = SinT(rotationVector[2]);
    f32 cosZ = CosT(rotationVector[2]);

    transformMatrix[0][0] = ((cosY * cosZ) + (sinX * sinY * sinZ)) * scalingFactor;
    transformMatrix[1][0] = ((-cosY * sinZ) + (sinX * sinY * cosZ)) * scalingFactor;
    transformMatrix[2][0] = (cosX * sinY) * scalingFactor;
    transformMatrix[3][0] = translationVector[0];
    transformMatrix[0][1] = cosX * sinZ * scalingFactor;
    transformMatrix[1][1] = cosX * cosZ * scalingFactor;
    transformMatrix[2][1] = -sinX * scalingFactor;
    transformMatrix[3][1] = translationVector[1];
    transformMatrix[0][2] = ((-sinY * cosZ) + (sinX * cosY * sinZ)) * scalingFactor;
    transformMatrix[1][2] = ((sinY * sinZ) + (sinX * cosY * cosZ)) * scalingFactor;
    transformMatrix[2][2] = cosX * cosY * scalingFactor;
    transformMatrix[3][2] = translationVector[2];
    transformMatrix[0][3] = 0.0f;
    transformMatrix[1][3] = 0.0f;
    transformMatrix[2][3] = 0.0f;
    transformMatrix[3][3] = 1.0f;
}

void KwcreateBoardMatrix(Mat4 transformMatrix, Vec3f vec1, Vec3f vec2, f32 Scale) {
    transformMatrix[0][0] = Scale;
    transformMatrix[1][0] = 0.0f;
    transformMatrix[2][0] = 0.0f;
    transformMatrix[3][0] = vec1[0] - vec2[0];
    transformMatrix[0][1] = 0.0f;
    transformMatrix[1][1] = -Scale;
    transformMatrix[2][1] = 0.0f;
    transformMatrix[3][1] = vec1[1] - vec2[1];
    transformMatrix[0][2] = 0.0f;
    transformMatrix[1][2] = 0.0f;
    transformMatrix[2][2] = -Scale;
    transformMatrix[3][2] = vec1[2] - vec2[2];
    transformMatrix[0][3] = 0.0f;
    transformMatrix[1][3] = 0.0f;
    transformMatrix[2][3] = 0.0f;
    transformMatrix[3][3] = 1.0f;
}

/**
 * @brief Tried to put the definitions in the declares. However, sp3C wants to be at the top.
 * Something may be possible with some Padding. Couldn't find a way though. So we get big mess.
 *
 * @Param arg0
 * @Param arg1
 **/

void KwcreateModelingMatrixAnm(s32 ObjectIndex, Mat4 transformMatrix) {
    f32 sinX;
    AnmObject* object = &DynamicObjects[ObjectIndex];
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;
    f32 cosX;

    sinX = SinT(object->Texang[0]);
    cosX = CosT(object->Texang[0]);
    sinY = SinT(object->Texang[1]);
    cosY = CosT(object->Texang[1]);
    sinZ = SinT(object->Texang[2]);
    cosZ = CosT(object->Texang[2]);

    transformMatrix[0][0] = object->Scale * ((cosY * cosZ) + (sinX * sinY * sinZ));
    transformMatrix[1][0] = object->Scale * ((-cosY * sinZ) + sinX * sinY * cosZ);
    transformMatrix[2][0] = object->Scale * (cosX * sinY);
    transformMatrix[3][0] = object->pos[0];
    transformMatrix[0][1] = object->Scale * (cosX * sinZ);
    transformMatrix[1][1] = object->Scale * (cosX * cosZ);
    transformMatrix[2][1] = object->Scale * -sinX;
    transformMatrix[3][1] = object->pos[1];
    transformMatrix[0][2] = object->Scale * ((-sinY * cosZ) + (sinX * cosY * sinZ));
    transformMatrix[1][2] = object->Scale * ((sinY * sinZ) + (sinX * cosY * cosZ));
    transformMatrix[2][2] = object->Scale * (cosX * cosY);
    transformMatrix[3][2] = object->pos[2];
    transformMatrix[0][3] = 0.0f;
    transformMatrix[1][3] = 0.0f;
    transformMatrix[2][3] = 0.0f;
    transformMatrix[3][3] = 1.0f;
}

UNUSED void KwscalingMatrixX(Mat4 arg0, f32 arg1) {
    arg0[0][0] *= arg1;
    arg0[1][0] *= arg1;
    arg0[2][0] *= arg1;
}

UNUSED void KwscalingMatrixY(Mat4 arg0, f32 arg1) {
    arg0[0][1] *= arg1;
    arg0[1][1] *= arg1;
    arg0[2][1] *= arg1;
}

UNUSED void KwscalingMatrixZ(Mat4 arg0, f32 arg1) {
    arg0[0][2] *= arg1;
    arg0[1][2] *= arg1;
    arg0[2][2] *= arg1;
}

void KwcreateRotationMatrix(Mat4 dest, Vec3f orientationVector, Vec3f positionVector, u16 rotationAngle,
                          f32 scaleFactor) {
    Vec3f sp44;
    Vec3f sp38;
    Vec3f sp2C;

    Vec3FSetXYZ(sp44, SinT(rotationAngle), 0.0f, CosT(rotationAngle));
    NormalizeFVector(orientationVector);
    CrossProduct(sp38, orientationVector, sp44);
    NormalizeFVector(sp38);
    CrossProduct(sp2C, sp38, orientationVector);
    NormalizeFVector(sp2C);
    dest[0][0] = sp38[0] * scaleFactor;
    dest[0][1] = sp38[1] * scaleFactor;
    dest[0][2] = sp38[2] * scaleFactor;
    dest[3][0] = positionVector[0];
    dest[1][0] = orientationVector[0] * scaleFactor;
    dest[1][1] = orientationVector[1] * scaleFactor;
    dest[1][2] = orientationVector[2] * scaleFactor;
    dest[3][1] = positionVector[1];
    dest[2][0] = sp2C[0] * scaleFactor;
    dest[2][1] = sp2C[1] * scaleFactor;
    dest[2][2] = sp2C[2] * scaleFactor;
    dest[3][2] = positionVector[2];
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}

// aplly to Position a Rotation and put in dest
UNUSED void KwrotateYxz(Vec3f dest, Vec3f pos, Vec3s rot) {
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 temp_f4;
    f32 sp64;
    f32 sp60;
    f32 temp_f8;
    f32 sp58;
    f32 sp54;
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = SinT(rot[0]);
    cosine1 = CosT(rot[0]);
    sine2 = SinT(rot[1]);
    cosine2 = CosT(rot[1]);
    sine3 = SinT(rot[2]);
    cosine3 = CosT(rot[2]);
    // it's a Matrix multiplication
    sp74 = pos[0] * ((cosine2 * cosine3) + ((sine1 * sine2) * sine3));
    temp_f4 = pos[1] * ((-cosine2 * sine3) + ((sine1 * sine2) * cosine3));
    temp_f8 = pos[2] * (cosine1 * sine2);
    sp70 = pos[0] * (cosine1 * sine3);
    sp64 = pos[1] * (cosine1 * cosine3);
    sp58 = pos[2] * -sine1;
    sp6C = pos[0] * ((-sine2 * cosine3) + ((sine1 * cosine2) * sine3));
    sp60 = pos[1] * ((sine2 * sine3) + ((sine1 * cosine2) * cosine3));
    sp54 = pos[2] * (cosine1 * cosine2);
    dest[0] = sp74 + temp_f4 + temp_f8;
    dest[1] = sp70 + sp64 + sp58;
    dest[2] = sp6C + sp60 + sp54;
}

// apply to Position a Rotation x y only and put in dest
void KwrotateYx(Vec3f dest, Vec3f pos, Vec3s rot) {
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;

    sp2C = pos[0];
    sp28 = pos[1];
    sp24 = pos[2];
    sine1 = SinT(rot[0]);
    cosine1 = CosT(rot[0]);
    sine2 = SinT(rot[1]);
    cosine2 = CosT(rot[1]);
    dest[0] = (sp2C * cosine2) - (sp24 * sine2);
    dest[1] = (sp2C * sine1 * sine2) + (sp28 * cosine1) + (sp24 * sine1 * cosine2);
    dest[2] = ((sp2C * cosine1 * sine2) - (sp28 * sine1)) + (sp24 * cosine1 * cosine2);
}

/**
 * @brief set the Matrix to a transformation Matrix
 *
 * @Param translate or Position
 * @Param orientation
 * @Param Scale
 */
void KWCreateAffine3D(Vec3f translate, Vec3su orientation, f32 Scale) {
    Mat4 Matrix;

    KwcreateModelingMatrix(Matrix, translate, orientation, Scale);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

UNUSED void KWCreateAffine3DBoard(Vec3f pos1, Vec3f pos2, f32 Scale) {
    Mat4 Matrix;

    KwcreateBoardMatrix(Matrix, pos1, pos2, Scale);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void KWCreateAffine3DIceKage(Vec3f arg0, Vec3su arg1, f32 Arg2) {
    Mat4 Matrix;
    Vec3su orientation;

    orientation[0] = arg1[0] + 0x8000; // change the sign
    orientation[1] = arg1[1] + 0x8000; // change the sign
    orientation[2] = arg1[2];
    KwcreateModelingMatrix(Matrix, arg0, orientation, Arg2);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void KWCreateAffine3DHole(Vec3f arg0, Vec3f arg1, f32 Arg2) {
    Mat4 Matrix;

    KwcreateRotationMatrix(Matrix, arg1, arg0, 0, Arg2);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}

void KWCreateAffine3DAnm(s32 transformIndex) {
    Mat4 Matrix;

    KwcreateModelingMatrixAnm(transformIndex, Matrix);
    Affine2Mtx(&DynamicP->MTXHud[Modelingnum], Matrix);

    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
}
