#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <PR/ultratypes.h>

// #define sinT(x) gSineTable[(u16) (x) >> 4]
// #define cosT(x) CosTable[(u16) (x) >> 4]

// #define min(a, b) ((a) <= (b) ? (a) : (b))
// #define max(a, b) ((a) > (b) ? (a) : (b))

#define sqr(x) ((x) * (x))

// Here to appease the pragma gods
double fabs(double x);

void func_802B5794(Mat4, Vec3f, Vec3f);       // Unused
s32 CalcDisplayPosition(s32, Vec3f, s32, f32, f32); // Unused
s32 SetMatrix(Mat4, s32);
f32 CalcDistance(Vec3f, Vec3f);
s32 CalcDirection(Vec3f, Vec3f);
u32 CalcDirectionS(Vec3f, Vec3s); // Unused
void vec3f_set(Vec3f, f32, f32, f32);
void SetSVector(Vec3s, s16, s16, s16);
void* CopyVector(Vec3f, Vec3f);
void CopySVector(Vec3s, Vec3s);
void* vec3f_set_return(Vec3f, f32, f32, f32);
void CopyAffine(Mat4, Mat4);
void CopyLongArray(s32*, s32*, s32);
void LoadIdentAffineMtx(Mat4);
void TranslateMatrix(Mat4, Mat4, Vec3f);
void TranslateMatrix2(Mat4, Mat4, Vec3f);
void CreateTransAffineMtx(Mat4, Vec3f);
void MakeGuPerspective(Mat4, u16*, f32, f32, f32, f32, f32);
void MakeGuLookAt(Mat4, Vec3f, Vec3f);
void CreateMtxRotateX(Mat4, s16);
void CreateMtxRotateY(Mat4, s16);
void CreateMtxRotateZ(Mat4, s16);
void RotateVector(Vec3f b, Vec3s rotate); // Unused
void func_802B5CAC(s16, s16, Vec3f);       // Unused
void MakeMapLight(s16, s16, s32);         // Unused
void MakeLight(Lights1*, s16, s16, s32);
void ScalingMatrix(Mat4, f32);
void CreateModelingMatrix(Mat4, Vec3f, Vec3s);
void MultipleMatrixByVector(Vec3f, Mat3);
void MultipleAffineMtxByVector(Vec3f, Mat4);
void MakeAlignVector(Vec3f, s16);
void MakeAlignMatrix(Mat3, f32, f32, f32, s16);
void MakeRotate(Mat3, s16, f32, f32, f32);
void MakeAlign(Mat4, s16, f32, f32, f32); // Unused
void CreateModelAffineMtx(Mat4, Vec3f, Vec3f);       // Unused
void MultiAffineMtx(Mat4, Mat4, Mat4);
void AffineToMtx(Mtx*, Mat4);
u16 GetAtanTable(f32, f32);
u16 Atan2t(f32, f32);
f32 Atan2f(f32, f32);
s16 Atant(f32);
s16 Asint(f32);
f32 Acosf(f32);
u16 MakeRandom(void);
u16 MakeRandomLimmit(u16);
s16 angle_from_coords(f32, f32, f32, f32);
void MakeDirection3D(Vec3f, Vec3f, Vec3s);
f32 sinT(u16);
f32 cosT(u16);
s32 CheckCone(u16, u16, u16);
f32 CheckDisplayRange(Vec3f, Vec3f, u16, f32, f32, f32);

extern s32 D_802B91C0[]; // Unused
extern Vec3f D_802B91C8;

#endif // MATH_UTIL_H
