#ifndef MATH_UTIL_2_H
#define MATH_UTIL_2_H

#include <common_structs.h>
#include "camera.h"

/* Function Prototypes */

// Unused functions
void kw_setflag(s32*, s32);
void kw_clrflag(s32*, s32);
void kw_eorflag(s32*, s32);
UNUSED bool kw_bitcheckflag(s32*, s32);
s32 kw_bitoffcheckflag(s32*, s32);
s32 kwadd_fvar(f32*, f32, f32*);
s32 kwsub_fvar(f32*, f32, f32*);
s32 kwadd_ivar(s32*, s32, s32*);
s32 kwsub_ivar(s32*, s32, s32*);
s32 kwadd_svar(s16*, s16, s16*);
s32 kwsub_svar(s16*, s16, s16*);
s32 KWChaseIVal(s32*, s32, s32);
s32 KWCheckRadiusXZ(f32, f32, f32, f32, f32);
s32 kwset_velxztoangy(f32, f32);
s32 kwset_velxytoangz(f32, f32);
void kwtranslate2D(s32, s32);
void kwZrotate2D(u16);
void kwscale2D(f32);
void kwZrotatescale2D(u16, f32);
void kwscaletranslate2D(s32, s32, f32);
void func_800423F0(Mat4, u16, u16, u16);
void kwrotate_mtx_yx(Mat4, u16, u16);
void kwscaling_matrix_x(Mat4, f32);
void kwscaling_matrix_y(Mat4, f32);
void kwscaling_matrix_z(Mat4, f32);
void kwrotate_yxz(Vec3f, Vec3f, Vec3s);
void KWCreateAffine3D_Board(Vec3f, Vec3f, f32);

void kwcopy_fval(Vec3f, Vec3f);
s32 kwadd_fval(f32*, f32, f32);
s32 kwsub_fval(f32*, f32, f32);
s32 kwadd_ival(s32*, s32, s32);
s32 kwsub_ival(s32*, s32, s32);
s32 kwadd_sval(s16*, s16, s16);
s32 kwadd_usval(u16*, u16, u16);
s32 kwsub_sval(s16*, s16, s16);
s32 kwsub_usval(u16*, s32, s32);
s32 KWChaseSVal(s16*, s16, s16);
s32 KWChaseFVal(f32*, f32, f32);
void kwoufuku_sval(s16*, s16, s16, s16*);
Vec3f* vec3f_set_xyz(Vec3f, f32, f32, f32);
Vec3f* NormalizeFVector(Vec3f dest);
Vec3f* CrossProduct(Vec3f, Vec3f, Vec3f);
s32 kwset_velyztoangx(f32, f32);
f32 kwrotate2D_x(f32, f32, u16);
f32 kwrotate2D_y(f32, f32, u16);
s32 kwcalc_destangley(f32, f32, f32, f32);
u16 kwchase_angle(u16, u16);
s32 KWLookCamera(f32, f32, Vec3f);
s32 KWLookCameraPitch(f32, f32, Vec3f);
s32 kwcalc_bump(Collision*, Vec3f);
bool kwvischeck_camera_xz(Vec3f, Camera*, u16);
void kwcalc_lightvector(void);
void translate2D_mat(Mat4, s32, s32);
void Zrotate2D_mat(Mat4, u16);
void scale2D_mat(Mat4, f32);
void Zrotatescale2D_mat(Mat4, u16, f32);
void createaffine2D(Mat4, s32, s32, u16, f32);
void kwswitch_demo2pgp(void);
void kwset_projection2D_matrix(void);
void KW2DMatrixInit(void);
void kwcreateaffine2D(s32, s32, u16, f32);
void kwcreate_modeling_matrix(Mat4, Vec3f, Vec3su, f32);
void kwcreate_board_matrix(Mat4, Vec3f, Vec3f, f32);
void kwcreate_modeling_matrix_anm(s32, Mat4);
void kwcreate_rotation_matrix(Mat4, Vec3f, Vec3f, u16, f32);
void kwrotate_yx(Vec3f, Vec3f, Vec3s);
void KWCreateAffine3D(Vec3f, Vec3su, f32);
void KWCreateAffine3D_IceKage(Vec3f, Vec3su, f32);
void KWCreateAffine3D_Hole(Vec3f, Vec3f, f32);
void KWCreateAffine3D_Anm(s32);

/* This is where I'd put my static data, if I had any */
extern s8 D_801658FE;

#endif // MATH_UTIL_2_H
