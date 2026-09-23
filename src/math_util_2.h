#ifndef MATH_UTIL_2_H
#define MATH_UTIL_2_H

#include <common_structs.h>
#include "camera.h"

/* Function Prototypes */

// Unused functions
void KWSetflag(s32*, s32);
void KWClrflag(s32*, s32);
void KWEorflag(s32*, s32);
UNUSED bool KWBitcheckflag(s32*, s32);
s32 KWBitoffcheckflag(s32*, s32);
s32 KwaddFvar(f32*, f32, f32*);
s32 KwsubFvar(f32*, f32, f32*);
s32 KwaddIvar(s32*, s32, s32*);
s32 KwsubIvar(s32*, s32, s32*);
s32 KwaddSvar(s16*, s16, s16*);
s32 KwsubSvar(s16*, s16, s16*);
s32 KWChaseIVal(s32*, s32, s32);
s32 KWCheckRadiusXZ(f32, f32, f32, f32, f32);
s32 KwsetVelxztoangy(f32, f32);
s32 KwsetVelxytoangz(f32, f32);
void Kwtranslate2D(s32, s32);
void KWZrotate2D(u16);
void Kwscale2D(f32);
void KWZrotatescale2D(u16, f32);
void Kwscaletranslate2D(s32, s32, f32);
void func_800423F0(Mat4, u16, u16, u16);
void KwrotateMTXYx(Mat4, u16, u16);
void KwscalingMatrixX(Mat4, f32);
void KwscalingMatrixY(Mat4, f32);
void KwscalingMatrixZ(Mat4, f32);
void KwrotateYxz(Vec3f, Vec3f, Vec3s);
void KWCreateAffine3DBoard(Vec3f, Vec3f, f32);

void KwcopyFval(Vec3f, Vec3f);
s32 KwaddFval(f32*, f32, f32);
s32 KwsubFval(f32*, f32, f32);
s32 KwaddIval(s32*, s32, s32);
s32 KwsubIval(s32*, s32, s32);
s32 KwaddSval(s16*, s16, s16);
s32 KwaddUsval(u16*, u16, u16);
s32 KwsubSval(s16*, s16, s16);
s32 KwsubUsval(u16*, s32, s32);
s32 KWChaseSVal(s16*, s16, s16);
s32 KWChaseFVal(f32*, f32, f32);
void KwoufukuSval(s16*, s16, s16, s16*);
Vec3f* Vec3FSetXYZ(Vec3f, f32, f32, f32);
Vec3f* NormalizeFVector(Vec3f dest);
Vec3f* CrossProduct(Vec3f, Vec3f, Vec3f);
s32 KwsetVelyztoangx(f32, f32);
f32 Kwrotate2DX(f32, f32, u16);
f32 Kwrotate2DY(f32, f32, u16);
s32 KwcalcDestangley(f32, f32, f32, f32);
u16 KwchaseAngle(u16, u16);
s32 KWLookCamera(f32, f32, Vec3f);
s32 KWLookCameraPitch(f32, f32, Vec3f);
s32 KwcalcBump(Collision*, Vec3f);
bool KwvischeckCameraXz(Vec3f, Camera*, u16);
void KwcalcLightvector(void);
void Translate2DMat(Mat4, s32, s32);
void Zrotate2DMat(Mat4, u16);
void Scale2DMat(Mat4, f32);
void Zrotatescale2DMat(Mat4, u16, f32);
void Createaffine2D(Mat4, s32, s32, u16, f32);
void KwswitchDemo2Pgp(void);
void KwsetProjection2DMatrix(void);
void KW2DMatrixInit(void);
void Kwcreateaffine2D(s32, s32, u16, f32);
void KwcreateModelingMatrix(Mat4, Vec3f, Vec3su, f32);
void KwcreateBoardMatrix(Mat4, Vec3f, Vec3f, f32);
void KwcreateModelingMatrixAnm(s32, Mat4);
void KwcreateRotationMatrix(Mat4, Vec3f, Vec3f, u16, f32);
void KwrotateYx(Vec3f, Vec3f, Vec3s);
void KWCreateAffine3D(Vec3f, Vec3su, f32);
void KWCreateAffine3DIceKage(Vec3f, Vec3su, f32);
void KWCreateAffine3DHole(Vec3f, Vec3f, f32);
void KWCreateAffine3DAnm(s32);

/* This is where I'd put my static data, if I had any */
extern s8 D_801658FE;

#endif // MATH_UTIL_2_H
