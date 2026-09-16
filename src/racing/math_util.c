#include <ultra64.h>
#include <mk64.h>
#include <macros.h>
#include <math_util.h>
#include <main.h>
#include "buffers.h"
#include <PR/rcp.h>
#include "buffers/trig_tables.h"
#include "math.h"
#include "memory.h"

#pragma intrinsic(sqrtf, fabs)

UNUSED s32 D_802B91C0[2] = { 13, 13 };
Vec3f D_802B91C8 = { 0.0f, 0.0f, 0.0f };

// This functions looks similar to a segment of code from DisplayBackground in skybox_and_splitscreen.c
UNUSED s32 CalcDisplayPosition(UNUSED s32 arg0, Vec3f arg1, UNUSED s32 arg2, UNUSED f32 arg3, UNUSED f32 arg4) {
    Mat4 sp30;
    f32 sp2C;
    f32 sp28;
    Vec3f sp1C;
    CopyVector(sp1C, arg1);
    sp28 = sp1C[0];
    sp2C = sp1C[1];
    // wut?
    if (sp2C && sp2C) {};
    sp2C = ((sp30[0][3] * sp28) + (sp30[1][3] * sp2C) + (sp30[2][3] * sp1C[2])) + sp30[3][3];
    // double wut?
    if (sp28 && sp28) {};
    MultipleAffineMtxByVector(sp1C, sp30);
    if (0.0f >= sp2C) {
        return 0;
    } else {
        return 1;
    }
}

UNUSED void func_802B4FF0() {
}

/**
 * Inserts matrix into the rsp. Position, rotation and mode of where to render the next object and check number of
 * object already render Note that gMatrixCount gets reset at the beginning of the game loop. So no cleanup needs
 * to be performed.
 */
s32 SetMatrix(Mat4 mtx, s32 mode) {
    if (gMatrixCount >= MTX_OBJECT_POOL_SIZE) {
        return 0;
    }
    AffineToMtx(&gDynamicP->mtxObject[gMatrixCount], mtx);
    switch (mode) { /* irregular */
        case 0:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxObject[gMatrixCount++]),
                      G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            break;
        case 1:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxObject[gMatrixCount++]),
                      G_MTX_PUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            break;
        case 3:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxObject[gMatrixCount++]),
                      G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            break;
        case 2:
            gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(&gDynamicP->mtxObject[gMatrixCount++]),
                      G_MTX_NOPUSH | G_MTX_MUL | G_MTX_MODELVIEW);
            break;
    }
    return 1;
}

/*
 * @brief      Finds the squared distance between two points, but contains a bug when handling the z-axis
 * @param from A point in 3D space
 * @param to   A point in 3D space
 * @return     Roughly the squared distance between from and to. (x**2 + y**2 + 2*z) instead of (x**2 + y**2 + z**2)
 */
f32 CalcDistance(Vec3f from, Vec3f to) {
    f32 deltaY;
    f32 deltaZ;
    f32 deltaX;

    deltaX = to[0] - from[0];
    deltaY = to[1] - from[1];
    deltaZ = to[2] - from[2];
    // If the last plus was a multiplication symbol, we'd have a correct dist_squared formula
    return (deltaX * deltaX) + (deltaY * deltaY) + deltaZ + deltaZ;
}

/*
 * @brief           Finds the angle within the XZ-plane between two points (while ignoring any difference in Y)
 * @param pointFrom A point in 3D space
 * @param pointTo   A point in 3D space
 * @return          Angle (N64-units) in XZ-plane between pointFrom and pointTo
 */
s32 CalcDirection(Vec3f pointFrom, Vec3f pointTo) {
    f32 deltaX;
    f32 deltaZ;
    deltaX = pointTo[0] - pointFrom[0];
    deltaZ = pointTo[2] - pointFrom[2];

    return Atan2t(deltaX, deltaZ);
}

// copy of CalcDirection
UNUSED u32 CalcDirectionS(Vec3f arg0, Vec3s arg1) {
    f32 temp_v1;
    f32 temp_v2;
    temp_v1 = arg1[0] - arg0[0];
    temp_v2 = arg1[2] - arg0[2];

    return Atan2t(temp_v1, temp_v2);
}

/*
 * @brief        sets a vector to the given coordinates
 * @param dest   The vector to be overriden
 * @param coordX The X coordinate of the desired vector
 * @param coordY The Y coordinate of the desired vector
 * @param coordZ The Z coordinate of the desired vector
 */
void vec3f_set(Vec3f dest, f32 coordX, f32 coordY, f32 coordZ) {
    dest[0] = coordX;
    dest[1] = coordY;
    dest[2] = coordZ;
}

/*
 * @brief        sets a vector to the given coordinates
 * @param dest   The vector to be overriden
 * @param coordX The X coordinate of the desired vector
 * @param coordY The Y coordinate of the desired vector
 * @param coordZ The Z coordinate of the desired vector
 */
void SetSVector(Vec3s dest, s16 coordX, s16 coordY, s16 coordZ) {
    dest[0] = coordX;
    dest[1] = coordY;
    dest[2] = coordZ;
}

// These functions have bogus return values.
// Disable the compiler warning.
#pragma GCC diagnostic push

#ifdef __GNUC__
#if defined(__clang__)
#pragma GCC diagnostic ignored "-Wreturn-stack-address"
#else
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#endif
#endif

/*
 * @brief      Copies the coordinates of a vector to another vector
 * @param dest The vector to be overriden
 * @param src  The vector to be copied
 * @return     local address of destination vector
 */
void* CopyVector(Vec3f dest, Vec3f src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    //! @warning function returns address of local variable
    return &dest;
}

/*
 * @brief      copies the coordinates of a vector to another vector
 * @param dest The vector to be overriden
 * @param src  The vector to be copied
 */
void CopySVector(Vec3s dest, Vec3s src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

UNUSED void* vec3f_set_return(Vec3f dest, f32 x, f32 y, f32 z) {
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return &dest;
}

/*
 * @brief      copies the values of a matrix to another matrix
 * @param src  The matrix to be copied
 * @param dest The matrix to be overriden
 */
void CopyAffine(Mat4 src, Mat4 dest) {
    s32 row;
    s32 column;

    for (row = 0; row < 4; row++) {
        for (column = 0; column < 4; column++) {
            dest[row][column] = src[row][column];
        }
    }
}

/*
 * @brief      copies the first n values of a matrix to another matrix
 * @param dest The matrix to be overriden
 * @param src  The matrix to be copied
 * @param n    The number of values to be copied
 */
void CopyLongArray(s32* dest, s32* src, s32 n) {
    while (n-- > 0) {
        *dest++ = *src++;
    }
}

/*
 * @brief        Transform a matrix to an identity matrix
 * @param Matrix The matrix to be changed to an identity matrix
 */
void LoadIdentAffineMtx(Mat4 mtx) {
    register s32 row;
    register s32 col;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            mtx[row][col] = (row == col) ? 1.0f : 0.0f;
        }
    }
}

/*
 * @brief           Add a translation vector to a matrix
 * @param scr       The matrix to be copied
 * @param dest      The matrix to be overriden with the result
 * @param translate The translation vector to be added
 */
void TranslateMatrix(Mat4 src, Mat4 dest, Vec3f translate) {
    dest[3][0] = src[3][0] + translate[0];
    dest[3][1] = src[3][1] + translate[1];
    dest[3][2] = src[3][2] + translate[2];
    dest[3][3] = src[3][3];
    dest[0][0] = src[0][0];
    dest[0][1] = src[0][1];
    dest[0][2] = src[0][2];
    dest[0][3] = src[0][3];
    dest[1][0] = src[1][0];
    dest[1][1] = src[1][1];
    dest[1][2] = src[1][2];
    dest[1][3] = src[1][3];
    dest[2][0] = src[2][0];
    dest[2][1] = src[2][1];
    dest[2][2] = src[2][2];
    dest[2][3] = src[2][3];

    /*
     * src(0,0)              src(0,1)              src(0,2)              src(0,3)
     * src(1,0)              src(1,1)              src(1,2)              src(1,3)
     * src(2,0)              src(2,1)              src(2,2)              src(2,3)
     * src(3,0)+translate(0) src(3,1)+translate(1) src(3,2)+translate(2) src(3,3)
     */
}

// Light version of TranslateMatrix
UNUSED void TranslateMatrix2(Mat4 mat, Mat4 dest, Vec3f pos) {
    dest[3][0] = mat[3][0] + pos[0];
    dest[3][1] = mat[3][1] + pos[1];
    dest[3][2] = mat[3][2] + pos[2];
}

/*
 * @brief           Creates a translation matrix
 * @param dest      The matrix to be overriden with the translation matrix
 * @param translate The translation vector to be added
 */
void CreateTransAffineMtx(Mat4 dest, Vec3f translate) {
    LoadIdentAffineMtx(dest);
    dest[3][0] = translate[0];
    dest[3][1] = translate[1];
    dest[3][2] = translate[2];
    /*            1            0            0           0
     *            0            1            0           0
     *            0            0            1           0
     * translate[0] translate[1] translate[2]           1
     */
}
/*
 * @brief                  Creates a projection matrix based on specified frustrum (i.e. where the camera can see)
 * @param projMtx          A dummy variable that will be overwritten with the projection matrix
 * @param arg1             Unknown dummy variable (will be overwritten)
 * @param vertFov          vertical field of view (in degrees)
 * @param aspectRatio      Width / Height of player screen
 * @param near             near clipping distance
 * @param far              far clipping distance
 * @param homogeneousScale Scaling factor for homogeneous coordinates. Always 1.0 in game
 * Note the use of `2` which generates diff asm than just using floats (2.0f).
 */
void MakeGuPerspective(Mat4 projMtx, u16* arg1, f32 vertFov, f32 aspectRatio, f32 near, f32 far, f32 homogeneousScale) {
    f32 halfCot;
    s32 rowIdx, colIdx;
    LoadIdentAffineMtx(projMtx);
    vertFov *= 0.017453292222222222; // convert from degrees to radians
    halfCot = cosf(vertFov / 2) / sinf(vertFov / 2);
    projMtx[0][0] = halfCot / aspectRatio;
    projMtx[1][1] = halfCot;
    // Literature usually prefers the clearer equivalent -(near + far) / (far - near)
    projMtx[2][2] = (near + far) / (near - far);
    projMtx[2][3] = -1.0f;
    projMtx[3][2] = (2 * near * far) / (near - far);
    projMtx[3][3] = 0.0f;

    for (rowIdx = 0; rowIdx < 4; rowIdx++) {
        for (colIdx = 0; colIdx < 4; colIdx++) {
            projMtx[rowIdx][colIdx] *= homogeneousScale;
        }
    }

    // sets arg1 to 2**16 / (midpoint of near and far), then clamped to [1, 2**16 - 1]
    if (arg1 != 0) {
        if ((near + far) <= 2.0) {
            *arg1 = 0xFFFF;
        } else {
            *arg1 = 131072.0 / (near + far);
            if (*arg1 <= 0) {
                *arg1 = 1;
            }
        }
    }
}

// Appears to only be for the skybox. MakeGuLookAt from sm64 with some modifications.
/**
 * @brief      Create a lookat matrix (convert to coordinates relative to camera)
 * @param mtx  Dummy matrix overwritten with lookat matrix
 * @param from Where the camera is looking from
 * @param to   Where the camera is looking to
 */
void MakeGuLookAt(Mat4 mtx, Vec3f from, Vec3f to) {
    // register from sm64 but not required for matching.
    register f32 invLength;

    // forward: direction camera lens is facing
    // up: direction toward top of frame
    // right: direction toward right of frame
    f32 upX;
    f32 upY;
    f32 upZ;
    f32 forwardX;
    f32 forwardY;
    f32 forwardZ;
    f32 rightX;
    f32 rightY;
    f32 rightZ;

    upX = 0.0f;
    upY = 1.0f;
    upZ = 0.0f;

    forwardX = to[0] - from[0];
    forwardY = to[1] - from[1];
    forwardZ = to[2] - from[2];

    // normalize forward vector
    invLength = -1.0 / sqrtf(forwardX * forwardX + forwardY * forwardY + forwardZ * forwardZ);
    forwardX *= invLength;
    forwardY *= invLength;
    forwardZ *= invLength;

    // Cross product (creates vector perpendicular to forward and up)
    rightX = upY * forwardZ - upZ * forwardY;
    rightY = upZ * forwardX - upX * forwardZ;
    rightZ = upX * forwardY - upY * forwardX;

    invLength = 1.0 / sqrtf(rightX * rightX + rightY * rightY + rightZ * rightZ);

    rightX *= invLength;
    rightY *= invLength;
    rightZ *= invLength;

    // Cross product
    upX = forwardY * rightZ - forwardZ * rightY;
    upY = forwardZ * rightX - forwardX * rightZ;
    upZ = forwardX * rightY - forwardY * rightX;

    invLength = 1.0 / sqrtf(upX * upX + upY * upY + upZ * upZ);
    upX *= invLength;
    upY *= invLength;
    upZ *= invLength;

    mtx[0][0] = rightX;
    mtx[1][0] = rightY;
    mtx[2][0] = rightZ;
    mtx[3][0] = -(from[0] * rightX + from[1] * rightY + from[2] * rightZ);

    mtx[0][1] = upX;
    mtx[1][1] = upY;
    mtx[2][1] = upZ;
    mtx[3][1] = -(from[0] * upX + from[1] * upY + from[2] * upZ);

    mtx[0][2] = forwardX;
    mtx[1][2] = forwardY;
    mtx[2][2] = forwardZ;
    mtx[3][2] = -(from[0] * forwardX + from[1] * forwardY + from[2] * forwardZ);

    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[3][3] = 1.0f;
}

/*
 * @brief       Create a rotation matrix for rotating about the X axis
 * @param mtx   Dummy matrix overwritten with x-axis rotation matrix
 * @param angle Angle to rotate by (in N64 units)
 */
void CreateMtxRotateX(Mat4 mtx, s16 angle) {
    f32 sinAngle = sinT(angle);
    f32 cosAngle = cosT(angle);

    LoadIdentAffineMtx(mtx);
    mtx[1][1] = cosAngle;
    mtx[1][2] = sinAngle;
    mtx[2][1] = -sinAngle;
    mtx[2][2] = cosAngle;

    /*
     * 1,         0,        0, 0,
     * 0,  cosAngle, sinAngle, 0,
     * 0, -sinAngle, cosAngle, 0,
     * 0,         0,        0, 1
     */
}

/*
 * @brief       Create a rotation matrix for rotating about the Y axis
 * @param mtx   Dummy matrix overwritten with Y axis rotation matrix
 * @param angle Angle to rotate by (in N64 units)
 */
void CreateMtxRotateY(Mat4 mtx, s16 angle) {
    f32 sinAngle = sinT(angle);
    f32 cosAngle = cosT(angle);

    LoadIdentAffineMtx(mtx);
    mtx[0][0] = cosAngle;
    mtx[0][2] = -sinAngle;
    mtx[2][0] = sinAngle;
    mtx[2][2] = cosAngle;

    /*
     * cosAngle, 0, -sinAngle, 0,
     *        0, 1,         0, 0,
     * sinAngle, 0,  cosAngle, 0,
     *        0, 0,         0, 1
     */
}

/*
 * @brief       Create a rotation matrix for rotating about the Z axis
 * @param mtx   Dummy matrix overwritten with Z axis rotation matrix
 * @param angle Angle to rotate by (in N64 units)
 */
void CreateMtxRotateZ(Mat4 mtx, s16 angle) {
    f32 sinAngle = sinT(angle);
    f32 cosAngle = cosT(angle);

    LoadIdentAffineMtx(mtx);
    mtx[0][0] = cosAngle;
    mtx[0][1] = sinAngle;
    mtx[1][0] = -sinAngle;
    mtx[1][1] = cosAngle;

    /*
     *  cosAngle, sinAngle, 0, 0,
     * -sinAngle, cosAngle, 0, 0,
     *         0,        0, 1, 0,
     *         0,        0, 0, 1
     */
}

UNUSED void RotateVector(Vec3f b, Vec3s rotate) {
    Mat4 mtx;
    Vec3f copy;

    f32 sx = sinT(rotate[0]);
    f32 cx = cosT(rotate[0]);

    f32 sy = sinT(rotate[1]);
    f32 cy = cosT(rotate[1]);

    f32 sz = sinT(rotate[2]);
    f32 cz = cosT(rotate[2]);

    copy[0] = b[0];
    copy[1] = b[1];

    mtx[0][0] = cy * cz + sx * sy * sz;
    mtx[1][0] = -cy * sz + sx * sy * cz;
    mtx[2][0] = cx * sy;

    mtx[0][1] = cx * sz;
    mtx[1][1] = cx * cz;
    mtx[2][1] = -sx;

    mtx[0][2] = -sy * cz + sx * cy * sz;
    mtx[1][2] = sy * sz + sx * cy * cz;
    mtx[2][2] = cx * cy;

    b[0] = copy[0] * mtx[0][0] + copy[1] * mtx[0][1] + copy[1] * mtx[0][2];
    b[1] = copy[0] * mtx[1][0] + copy[1] * mtx[1][1] + copy[1] * mtx[1][2];
    b[2] = copy[0] * mtx[2][0] + copy[1] * mtx[2][1] + copy[1] * mtx[2][2];
}

// rotates (0, 0, -1) about the x and y axis, in that order
void InitialLight(s16 rotY, s16 rotX, Vec3f arg2) {
    f32 sinX = sinT(rotX);
    f32 cosX = cosT(rotX);
    f32 sinY = sinT(rotY);
    f32 cosY = cosT(rotY);

    arg2[0] = cosX * sinY;
    arg2[1] = sinX;
    arg2[2] = -(cosX * cosY);
    // (0, 0, -1) -> (0, sinX, -cosX) -> (cosX * sinY, sinX, -(cosX * cosY))
}

UNUSED void MakeMapLight(s16 arg0, s16 arg1, s32 arg2) {
    MakeLight((Lights1*) 0x9000000, arg0, arg1, arg2);
}

/*
 * @brief Set the course lighting object
 * Uses a directional light, rotates by Y, then X
 *
 * @param lightAddr
 * @param rotateAngleY
 * @param rotateAngleX
 * @param lightCount Always 1 in practice
 */
void MakeLight(Lights1* lightAddr, s16 rotateAngleY, s16 rotateAngleX, s32 lightCount) {
    u32 segment = SEGMENT_NUMBER2(lightAddr);
    u32 offset = SEGMENT_OFFSET(lightAddr);
    UNUSED s32 pad;
    f32 sinAngleX;
    f32 cosAngleX;
    f32 sinAngleY;
    UNUSED s32 pad2[2];
    f32 cosAngleY;
    s32 lightIdx;
    s8 lightAngle[3];
    Lights1* lights;

    lights = (Lights1*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    sinAngleX = sinT(rotateAngleX);
    cosAngleX = cosT(rotateAngleX);
    sinAngleY = sinT(rotateAngleY);
    cosAngleY = cosT(rotateAngleY);
    // take (0, 0, 1), rotate by Y, then X
    lightAngle[0] = cosAngleX * sinAngleY * 120.0f;
    lightAngle[1] = 120.0f * sinAngleX;
    lightAngle[2] = cosAngleX * cosAngleY * -120.0f;
    for (lightIdx = 0; lightIdx < lightCount; lightIdx++, lights++) {
        lights->l[0].l.dir[0] = lightAngle[0];
        lights->l[0].l.dir[1] = lightAngle[1];
        lights->l[0].l.dir[2] = lightAngle[2];
    }
}

/*
 * @brief      Scale a matrix by a given coefficient
 * @param mtx  Matrix to scale
 * @param coef Coefficient to use when scaling
 */
void ScalingMatrix(Mat4 mtx, f32 coef) {
    mtx[0][0] *= coef;
    mtx[1][0] *= coef;
    mtx[2][0] *= coef;
    mtx[0][1] *= coef;
    mtx[1][1] *= coef;
    mtx[2][1] *= coef;
    mtx[0][2] *= coef;
    mtx[1][2] *= coef;
    mtx[2][2] *= coef;
}

/*
 * @brief             Matrix for rotating about Z, X, Y axes (in order) then translating
 * @param dest        Matrix to output
 * @param translate   Vector to use for translation
 * @param orientation Vector of 3 rotation angles (Rz, Rx, Ry)
 */
void CreateModelingMatrix(Mat4 dest, Vec3f translate, Vec3s orientation) {
    f32 sinX;
    f32 cosX;
    f32 sinY;
    f32 cosY;
    f32 sinZ;
    f32 cosZ;

    sinX = sinT(orientation[0]);
    cosX = cosT(orientation[0]);
    sinY = sinT(orientation[1]);
    cosY = cosT(orientation[1]);
    sinZ = sinT(orientation[2]);
    cosZ = cosT(orientation[2]);
    dest[0][0] = (cosY * cosZ) + ((sinX * sinY) * sinZ);
    dest[1][0] = (-cosY * sinZ) + ((sinX * sinY) * cosZ);
    dest[2][0] = cosX * sinY;
    dest[3][0] = translate[0];
    dest[0][1] = cosX * sinZ;
    dest[1][1] = cosX * cosZ;
    dest[2][1] = -sinX;
    dest[3][1] = translate[1];
    dest[0][2] = (-sinY * cosZ) + ((sinX * cosY) * sinZ);
    dest[1][2] = (sinY * sinZ) + ((sinX * cosY) * cosZ);
    dest[2][2] = cosX * cosY;
    dest[3][2] = translate[2];
    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}
// Product of Z, X and Y rotation matrices and a translation matrix
/* | Cz  Sz   0   0||  1   0   0   0|| Cy   0 -Sy   0||  1   0   0   0|
   |-Sz  Cz   0   0||  0  Cx  Sx   0||  0   1   0   0||  0   1   0   0|
   |  0   0   1   0||  0 -Sx  Cx   0|| Sy   0  Cy   0||  0   0   1   0|
   |  0   0   0   1||  0   0   0   1||  0   0   0   1|| V0  V1  V2   1|*/
/* | CyCz + SxSySz         CxSz -SyCz + SxCySz              0|
  =|-CySz + SxSyCz         CxCz  SySz + SxCyCz              0|
   |          CxSy          -Sx           CxCy              0|
   |            V0           V1             V2              1|*/
UNUSED void CreateCameraAffineMtx(Mat4 arg0, Vec3s arg1, Vec3s arg2) {
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = sinT(arg2[0]);
    cosine1 = cosT(arg2[0]);
    sine2 = sinT(arg2[1]);
    cosine2 = cosT(arg2[1]);
    sine3 = sinT(arg2[2]);
    cosine3 = cosT(arg2[2]);
    arg0[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    arg0[0][1] = (-cosine2 * sine3) + ((sine1 * sine2) * cosine3);
    arg0[0][2] = cosine1 * sine2;
    arg0[0][3] = (f32) arg1[0];
    arg0[1][0] = (f32) (cosine1 * sine3);
    arg0[1][1] = (f32) (cosine1 * cosine3);
    arg0[1][2] = (f32) -sine1;
    arg0[1][3] = (f32) arg1[1];
    arg0[2][0] = (f32) ((-sine2 * cosine3) + ((sine1 * cosine2) * sine3));
    arg0[2][1] = (f32) ((sine2 * sine3) + ((sine1 * cosine2) * cosine3));
    arg0[2][2] = (f32) (cosine1 * cosine2);
    arg0[2][3] = (f32) arg1[2];
    arg0[3][0] = 0.0f;
    arg0[3][1] = 0.0f;
    arg0[3][2] = 0.0f;
    arg0[3][3] = 1.0f;
}

UNUSED void CreateModelAffineMtx2(Mat4 arg0, Vec3s arg1, Vec3s arg2) {
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = sinT(arg2[0]);
    cosine1 = cosT(arg2[0]);
    sine2 = sinT(arg2[1]);
    cosine2 = cosT(arg2[1]);
    sine3 = sinT(arg2[2]);
    cosine3 = cosT(arg2[2]);
    arg0[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    arg0[1][0] = (-cosine2 * sine3) + ((sine1 * sine2) * cosine3);
    arg0[2][0] = cosine1 * sine2;
    arg0[3][0] = arg1[0];
    arg0[0][1] = cosine1 * sine3;
    arg0[1][1] = cosine1 * cosine3;
    arg0[2][1] = -sine1;
    arg0[3][1] = arg1[1];
    arg0[0][2] = (-sine2 * cosine3) + ((sine1 * cosine2) * sine3);
    arg0[1][2] = (sine2 * sine3) + ((sine1 * cosine2) * cosine3);
    arg0[2][2] = cosine1 * cosine2;
    arg0[3][2] = arg1[2];
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;
}

// look like to normalise vector
UNUSED void NomralizeVector(Vec3f arg0) {
    f32 temp_f0;
    temp_f0 = sqrtf((arg0[0] * arg0[0]) + (arg0[1] * arg0[1]) + (arg0[2] * arg0[2]));
    arg0[0] /= temp_f0;
    arg0[1] /= temp_f0;
    arg0[2] /= temp_f0;
}

/*
 * @brief     Given matrix M and vector v, calculates Mv
 * @param vec Vector to transform
 * @param mtx Matrix to use in transformation
 */
void MultipleMatrixByVector(Vec3f vec, Mat3 mtx) {
    f32 newX;
    f32 newY;
    f32 newZ;

    newX = (mtx[0][0] * vec[0]) + (mtx[0][1] * vec[1]) + (mtx[0][2] * vec[2]);
    newY = (mtx[1][0] * vec[0]) + (mtx[1][1] * vec[1]) + (mtx[1][2] * vec[2]);
    newZ = (mtx[2][0] * vec[0]) + (mtx[2][1] * vec[1]) + (mtx[2][2] * vec[2]);

    vec[0] = newX;
    vec[1] = newY;
    vec[2] = newZ;
}

/*
 * @brief     Given matrix M and vector v, calculates Mv
 * @param vec Vector to transform
 * @param mtx Matrix to use in transformation
 */
void MultipleAffineMtxByVector(Vec3f vec, Mat4 mat) {
    f32 newX;
    f32 newY;
    f32 newZ;

    newX = (mat[0][0] * vec[0]) + (mat[0][1] * vec[1]) + (mat[0][2] * vec[2]);
    newY = (mat[1][0] * vec[0]) + (mat[1][1] * vec[1]) + (mat[1][2] * vec[2]);
    newZ = (mat[2][0] * vec[0]) + (mat[2][1] * vec[1]) + (mat[2][2] * vec[2]);

    vec[0] = newX;
    vec[1] = newY;
    vec[2] = newZ;
}

UNUSED void MakeTopMatrix(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s32 arg3) {
}

/*
 * @brief rotates a given vector about the Y axis by amount specified
 * @param vec Vector to rotate
 * @param angle
 */
void MakeAlignVector(Vec3f vec, s16 rotAngleY) {
    f32 sinAngleY = sinT(rotAngleY);
    f32 cosAngleY = cosT(rotAngleY);

    f32 vecX = vec[0];
    f32 vecY = vec[1];
    f32 vecZ = vec[2];

    vec[0] = cosAngleY * vecX - (sinAngleY * vecZ);
    vec[1] = vecY;
    vec[2] = sinAngleY * vecX + (cosAngleY * vecZ);
}
// Standard Y-axis rotation matrix multiplication
/* |Cy   0 -Sy||Vx|
 * | 0   1   0||Vy| = |CyVx - SyVz, Vy, SyVx + CyVz|
 * |Sy   0  Cy||Vz|
 */

/* If cosAxisY is 1, this is just rotating around the Y axis, like in CreateMtxRotateY. Otherwise,
   it intends to rotate about any axis by composing with a rotation matrix for an XZ axis rotation.
   However, if Y is not 1 or -1, angle calculations break. It looks like cosAxisY might always
   be 1, but that is not completely confirmed.
 */
void MakeAlignMatrix(Mat3 dest, f32 axisZ, f32 cosAxisY, f32 axisX, s16 rotationAngle) {
    Mat3 mtxRotY;
    Mat3 mtxRotXZ;
    s32 row, col;
    f32 a;
    f32 axisNormedX;
    UNUSED f32 c;
    f32 axisNormedZ;
    UNUSED s32 pad[3];
    f32 sinValue;
    f32 cosValue;

    sinValue = sinT(rotationAngle);
    cosValue = cosT(rotationAngle);
    mtxRotY[0][0] = cosValue;
    mtxRotY[2][1] = 0;
    mtxRotY[1][2] = 0;

    mtxRotY[1][1] = 1;
    mtxRotY[2][0] = sinValue;
    mtxRotY[0][2] = -sinValue;

    mtxRotY[2][2] = cosValue;
    mtxRotY[1][0] = 0;
    mtxRotY[0][1] = 0;

    /* Standard rotation matrix
      [cos, 0, -sin]
      [  0, 1,    0]
      [sin, 0,  cos]
    */

    if (cosAxisY == 1) { // set matrix to identity

        for (row = 0; row < 3; row++) {
            for (col = 0; col < 3; col++) {
                mtxRotXZ[row][col] = (row == col) ? 1.0f : 0.0f;
            }
        }

    } else if (cosAxisY == -1) { // set matrix to identity with the second column negative

        for (row = 0; row < 3; row++) {
            for (col = 0; col < 3; col++) {
                mtxRotXZ[row][col] = (row == col) ? 1.0f : 0.0f;
            }
        }

        mtxRotXZ[1][1] = -1;

    } else {
        /* This looks like it is meant to convert from radians to degrees, but Acosf returns N64-units, not radians
        suggesting this is never used because it would give wildly incorrect values. Not sure this ever actually gets
        called because cosAxisY is usually 1*/
        a = (f32) - (360.0 - ((f64) (Acosf(cosAxisY) * 180.0f) / M_PI));
        axisNormedX = -axisX / sqrtf((axisZ * axisZ) + (axisX * axisX));
        axisNormedZ = axisZ / sqrtf((axisZ * axisZ) + (axisX * axisX));
        MakeRotate(mtxRotXZ, a, axisNormedX, 0,
                                  axisNormedZ); // rotates around something in the x-z plane
    }
    // mtxRotY * matrixRotXZ
    dest[0][0] = (mtxRotY[0][0] * mtxRotXZ[0][0]) + (mtxRotY[0][1] * mtxRotXZ[1][0]) + (mtxRotY[0][2] * mtxRotXZ[2][0]);
    dest[1][0] = (mtxRotY[1][0] * mtxRotXZ[0][0]) + (mtxRotY[1][1] * mtxRotXZ[1][0]) + (mtxRotY[1][2] * mtxRotXZ[2][0]);
    dest[2][0] = (mtxRotY[2][0] * mtxRotXZ[0][0]) + (mtxRotY[2][1] * mtxRotXZ[1][0]) + (mtxRotY[2][2] * mtxRotXZ[2][0]);

    dest[0][1] = (mtxRotY[0][0] * mtxRotXZ[0][1]) + (mtxRotY[0][1] * mtxRotXZ[1][1]) + (mtxRotY[0][2] * mtxRotXZ[2][1]);
    dest[1][1] = (mtxRotY[1][0] * mtxRotXZ[0][1]) + (mtxRotY[1][1] * mtxRotXZ[1][1]) + (mtxRotY[1][2] * mtxRotXZ[2][1]);
    dest[2][1] = (mtxRotY[2][0] * mtxRotXZ[0][1]) + (mtxRotY[2][1] * mtxRotXZ[1][1]) + (mtxRotY[2][2] * mtxRotXZ[2][1]);

    dest[0][2] = (mtxRotY[0][0] * mtxRotXZ[0][2]) + (mtxRotY[0][1] * mtxRotXZ[1][2]) + (mtxRotY[0][2] * mtxRotXZ[2][2]);
    dest[1][2] = (mtxRotY[1][0] * mtxRotXZ[0][2]) + (mtxRotY[1][1] * mtxRotXZ[1][2]) + (mtxRotY[1][2] * mtxRotXZ[2][2]);
    dest[2][2] = (mtxRotY[2][0] * mtxRotXZ[0][2]) + (mtxRotY[2][1] * mtxRotXZ[1][2]) + (mtxRotY[2][2] * mtxRotXZ[2][2]);
}

// include in MakeAlignMatrix
UNUSED void MakeTopAlign(Mat3 matrix, f32 arg1, f32 arg2, f32 arg3) {
    s32 i, j;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    UNUSED f32 pad;

    if (arg2 == 1) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                matrix[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    } else if (arg2 == -1) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                matrix[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
        matrix[1][1] = -1.0f;
    } else {
        a = (f32) - (360.0 - ((f64) (Acosf(arg2) * 180.0f) / M_PI));
        b = -arg3 / sqrtf((arg1 * arg1) + (arg3 * arg3));
        c = 0;
        d = arg1 / sqrtf((arg1 * arg1) + (arg3 * arg3));
        MakeRotate(matrix, a, b, c, d);
    }
}

/*
 * @brief               Rotates a given vector about a given axis by amount specified
 * @param destMatrix    Overriden with the resulting matrix
 * @param rotationAngle Angle to rotate (in N64 units)
 * @param axisX         The X component of the axis to rotate around
 * @param axisY         The Y component of the axis to rotate around
 * @param axisZ         The Z component of the axis to rotate around
 */
// Standard algorithm, but unintuitive. "Rotation matrix from axis and angle" brings up info online
void MakeRotate(Mat3 destMatrix, s16 rotationAngle, f32 axisX, f32 axisY, f32 axisZ) {
    f32 sinValue;
    f32 cosValue;
    f32 temp;
    f32 valueZX;
    f32 valueYZ;
    f32 valueXY;
    UNUSED s32 pad[2];

    sinValue = sinT((u16) rotationAngle);
    cosValue = cosT((u16) rotationAngle);

    temp = 1.0f - cosValue;

    valueZX = (axisZ * axisX) * temp;
    valueYZ = (axisY * axisZ) * temp;
    valueXY = (axisX * axisY) * temp;

    temp = axisX * axisX;
    destMatrix[0][0] = ((1.0f - temp) * cosValue) + temp;
    destMatrix[2][1] = valueYZ - (axisX * sinValue);
    destMatrix[1][2] = valueYZ + (axisX * sinValue);

    temp = axisY * axisY;
    destMatrix[1][1] = (((1.0f - temp) * cosValue) + temp);
    destMatrix[2][0] = valueZX + (axisY * sinValue);
    destMatrix[0][2] = valueZX - (axisY * sinValue);

    temp = axisZ * axisZ;
    destMatrix[2][2] = (((1.0f - temp) * cosValue) + temp);
    destMatrix[1][0] = valueXY - (axisZ * sinValue);
    destMatrix[0][1] = valueXY + (axisZ * sinValue);
}

UNUSED void MakeAlign(Mat4 arg0, s16 arg1, f32 arg2, f32 arg3, f32 arg4) {
    f32 sine;
    f32 cosine;
    f32 temp_f0;
    f32 temp_f12;

    sine = sinT(arg1);
    cosine = cosT(arg1);
    temp_f0 = sqrtf((arg2 * arg2) + (arg4 * arg4));
    if (temp_f0 != 0.0f) {
        temp_f12 = 1.0f / temp_f0;
        arg0[0][0] = ((-arg4 * cosine) - ((sine * arg3) * arg2)) * temp_f12;
        arg0[0][3] = ((arg4 * sine) - ((cosine * arg3) * arg2)) * temp_f12;
        arg0[1][2] = -arg2;
        arg0[0][1] = sine * temp_f0;
        arg0[1][0] = cosine * temp_f0;
        arg0[1][3] = -arg3;
        arg0[0][2] = ((cosine * arg2) - ((sine * arg3) * arg4)) * temp_f12;
        arg0[1][1] = ((-sine * arg2) - ((cosine * arg3) * arg4)) * temp_f12;
        arg0[2][0] = -arg4;
        arg0[0][3] = 0.0f;
        arg0[1][2] = 0.0f;
        arg0[2][1] = 0.0f;
    } else {
        arg0[0][1] = 0.0f;
        arg0[1][3] = 0.0f;
        arg0[0][2] = 0.0f;
        arg0[1][1] = 0.0f;
        arg0[0][3] = 0.0f;
        arg0[1][2] = 0.0f;
        arg0[2][1] = 0.0f;
        arg0[0][0] = 1.0f;
        arg0[1][0] = 1.0f;
        arg0[2][0] = 1.0f;
    }
}

// look like create a translation and rotation matrix with arg1 position and arg2 rotation
UNUSED void CreateModelAffineMtx(Mat4 arg0, Vec3f arg1, Vec3f arg2) {
    f32 sine1;
    f32 cosine1;
    f32 sine2;
    f32 cosine2;
    f32 sine3;
    f32 cosine3;

    sine1 = sinT(arg2[0]);
    cosine1 = cosT(arg2[0]);
    sine2 = sinT(arg2[1]);
    cosine2 = cosT(arg2[1]);
    sine3 = sinT(arg2[2]);
    cosine3 = cosT(arg2[2]);
    arg0[0][0] = (cosine2 * cosine3) + ((sine1 * sine2) * sine3);
    arg0[1][0] = (-cosine2 * sine3) + (sine1 * sine2) * cosine3;
    arg0[2][0] = cosine1 * sine2;
    arg0[3][0] = arg1[0];
    arg0[0][1] = cosine1 * sine3;
    arg0[1][1] = cosine1 * cosine3;
    arg0[2][1] = -sine1;
    arg0[3][1] = arg1[1];
    arg0[0][2] = (-sine2 * cosine3) + ((sine1 * cosine2) * sine3);
    arg0[1][2] = (sine2 * sine3) + ((sine1 * cosine2) * cosine3);
    arg0[2][2] = cosine1 * cosine2;
    arg0[3][2] = arg1[2];
    arg0[0][3] = 0.0f;
    arg0[1][3] = 0.0f;
    arg0[2][3] = 0.0f;
    arg0[3][3] = 1.0f;
}

/**
 * @brief          Multiply two 4x4 matrices
 * @param dest     Result of multiplication is saved here
 * @param mtxLeft  Left matrix in product
 * @param mtxRight Right matrix to product
 */
void MultiAffineMtx(Mat4 dest, Mat4 mtxLeft, Mat4 mtxRight) {
    Mat4 product;
    product[0][0] = (mtxLeft[0][0] * mtxRight[0][0]) + (mtxLeft[0][1] * mtxRight[1][0]) +
                    (mtxLeft[0][2] * mtxRight[2][0]) + (mtxLeft[0][3] * mtxRight[3][0]);
    product[0][1] = (mtxLeft[0][0] * mtxRight[0][1]) + (mtxLeft[0][1] * mtxRight[1][1]) +
                    (mtxLeft[0][2] * mtxRight[2][1]) + (mtxLeft[0][3] * mtxRight[3][1]);
    product[0][2] = (mtxLeft[0][0] * mtxRight[0][2]) + (mtxLeft[0][1] * mtxRight[1][2]) +
                    (mtxLeft[0][2] * mtxRight[2][2]) + (mtxLeft[0][3] * mtxRight[3][2]);
    product[0][3] = (mtxLeft[0][0] * mtxRight[0][3]) + (mtxLeft[0][1] * mtxRight[1][3]) +
                    (mtxLeft[0][2] * mtxRight[2][3]) + (mtxLeft[0][3] * mtxRight[3][3]);
    product[1][0] = (mtxLeft[1][0] * mtxRight[0][0]) + (mtxLeft[1][1] * mtxRight[1][0]) +
                    (mtxLeft[1][2] * mtxRight[2][0]) + (mtxLeft[1][3] * mtxRight[3][0]);
    product[1][1] = (mtxLeft[1][0] * mtxRight[0][1]) + (mtxLeft[1][1] * mtxRight[1][1]) +
                    (mtxLeft[1][2] * mtxRight[2][1]) + (mtxLeft[1][3] * mtxRight[3][1]);
    product[1][2] = (mtxLeft[1][0] * mtxRight[0][2]) + (mtxLeft[1][1] * mtxRight[1][2]) +
                    (mtxLeft[1][2] * mtxRight[2][2]) + (mtxLeft[1][3] * mtxRight[3][2]);
    product[1][3] = (mtxLeft[1][0] * mtxRight[0][3]) + (mtxLeft[1][1] * mtxRight[1][3]) +
                    (mtxLeft[1][2] * mtxRight[2][3]) + (mtxLeft[1][3] * mtxRight[3][3]);
    product[2][0] = (mtxLeft[2][0] * mtxRight[0][0]) + (mtxLeft[2][1] * mtxRight[1][0]) +
                    (mtxLeft[2][2] * mtxRight[2][0]) + (mtxLeft[2][3] * mtxRight[3][0]);
    product[2][1] = (mtxLeft[2][0] * mtxRight[0][1]) + (mtxLeft[2][1] * mtxRight[1][1]) +
                    (mtxLeft[2][2] * mtxRight[2][1]) + (mtxLeft[2][3] * mtxRight[3][1]);
    product[2][2] = (mtxLeft[2][0] * mtxRight[0][2]) + (mtxLeft[2][1] * mtxRight[1][2]) +
                    (mtxLeft[2][2] * mtxRight[2][2]) + (mtxLeft[2][3] * mtxRight[3][2]);
    product[2][3] = (mtxLeft[2][0] * mtxRight[0][3]) + (mtxLeft[2][1] * mtxRight[1][3]) +
                    (mtxLeft[2][2] * mtxRight[2][3]) + (mtxLeft[2][3] * mtxRight[3][3]);
    product[3][0] = (mtxLeft[3][0] * mtxRight[0][0]) + (mtxLeft[3][1] * mtxRight[1][0]) +
                    (mtxLeft[3][2] * mtxRight[2][0]) + (mtxLeft[3][3] * mtxRight[3][0]);
    product[3][1] = (mtxLeft[3][0] * mtxRight[0][1]) + (mtxLeft[3][1] * mtxRight[1][1]) +
                    (mtxLeft[3][2] * mtxRight[2][1]) + (mtxLeft[3][3] * mtxRight[3][1]);
    product[3][2] = (mtxLeft[3][0] * mtxRight[0][2]) + (mtxLeft[3][1] * mtxRight[1][2]) +
                    (mtxLeft[3][2] * mtxRight[2][2]) + (mtxLeft[3][3] * mtxRight[3][2]);
    product[3][3] = (mtxLeft[3][0] * mtxRight[0][3]) + (mtxLeft[3][1] * mtxRight[1][3]) +
                    (mtxLeft[3][2] * mtxRight[2][3]) + (mtxLeft[3][3] * mtxRight[3][3]);
    CopyLongArray((s32*) dest, (s32*) product, 16);
}

/**
 * Convert float matrix 'src' to fixed point matrix 'dest'.
 * The float matrix may not contain entries larger than 65536 or the console
 * crashes. The fixed point matrix has entries with a 16-bit integer part, so
 * the floating point numbers are multiplied by 2^16 before being cast to a s32
 * integer. If this doesn't fit, the N64 and iQue consoles will throw an
 * exception. On Wii and Wii U Virtual Console the value will simply be clamped
 * and no crashes occur.
 */
void AffineToMtx(Mtx* dest, Mat4 src) {
#ifdef AVOID_UB
    // Avoid type-casting which is technically UB by calling the equivalent
    // guMtxF2L function. This helps little-endian systems, as well.
    guMtxF2L(src, dest);
#else
    s32 asFixedPoint;
    register s32 i;
    register s16* a3 = (s16*) dest;      // all integer parts stored in first 16 bytes
    register s16* t0 = (s16*) dest + 16; // all fraction parts stored in last 16 bytes
    register f32* t1 = (f32*) src;

    for (i = 0; i < 16; i++) {
        asFixedPoint = *t1++ * (1 << 16);         //! float-to-integer conversion responsible for PU crashes
        *a3++ = GET_HIGH_S16_OF_32(asFixedPoint); // integer part
        *t0++ = GET_LOW_S16_OF_32(asFixedPoint);  // fraction part
    }
#endif
}

/*
 * Helper function for Atan2t. Does a look up of the arctangent of y/x assuming
 * the resulting angle is in range [0, 0x2000] (1/8 of a circle).
 * Note that this is only called by Atan2t, guaranteeing that y <= x
 *   If y > x, it will cause out of bounds issues
 *
 * @brief   Finds the arctan angle (in N64 units) given x, y coordinates
 * @param y y coordinate
 * @param x x coordinate
 * @return  arctan(y/x) (in N64 units)
 */
u16 GetAtanTable(f32 y, f32 x) {
    u16 ret;

    if (x == 0) {
        /* In a vacuum this would be incorrect. But, it works with how Atan2t
           is implemented, which is the only place this function is called*/
        ret = AtnTable[0];
    } else {
        if (1000000.0f < y / x) {
            if (y > 0.0f) {
                ret = 0x4000;
            } else {
                ret = 0xC000;
            }
        } else {
            ret = AtnTable[(s32) (y / x * 1024 + 0.5f)];
        }
    }
    return ret;
}

/*
 * Compute the angle from (0, 0) to (x, y) as a u16. Given that terrain is in
 * the xz-plane, this is commonly called with (z, x) to get a yaw angle.
 * sm64 but x, y swapped and returns u16.

 * @brief   Finds the arctan angle (in N64 units) given x, y coordinates
 * @param y y coordinate
 * @param x x coordinate
 * @return  arctan(y/x) (in N64 units)
 */
u16 Atan2t(f32 y, f32 x) {
    u16 ret;
    if (y >= 0) {
        if (x >= 0) {
            if (x >= y) {
                ret = GetAtanTable(y, x);
            } else {
                ret = 0x4000 - GetAtanTable(x, y);
            }
        } else {
            x = -x;
            if (x < y) {
                ret = 0x4000 + GetAtanTable(x, y);
            } else {
                ret = 0x8000 - GetAtanTable(y, x);
            }
        }
    } else {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x >= y) {
                ret = 0x8000 + GetAtanTable(y, x);
            } else {
                ret = 0xC000 - GetAtanTable(x, y);
            }
        } else {
            if (x < y) {
                ret = 0xC000 + GetAtanTable(x, y);
            } else {
                ret = -GetAtanTable(y, x);
            }
        }
    }
    return ret;
}

// @brief see Atan2t
f32 Atan2f(f32 y, f32 x) {
    return Atan2t(y, x);
}

#ifndef NON_MATCHING // The decomp does not support fabs
UNUSED f32 Atan2fx(f32 arg0, f32 arg1) {
    f64 halfpi;
    f32 temp_f0;
    UNUSED f32 pad;
    f32 temp_f2;
    f32 var_f16;
    f32 var_f2;
    s32 var_v0;

    var_f16 = arg0 / arg1;
    var_v0 = 0;
    if (fabs(arg1) < fabs(arg0)) {
        var_v0 = 1;
    }
    if (var_v0 != 0) {
        var_f16 = arg1 / arg0;
    }
    temp_f0 = var_f16 * var_f16;
    temp_f2 = temp_f0 * temp_f0;
    var_f16 +=
        ((((((((temp_f2 * ((-0.01600503f) + (temp_f0 * 0.00283406f))) + (-0.07495445f)) + (temp_f0 * 0.04258761f)) *
             (temp_f2 * temp_f2)) +
            (((-0.14202571f) + (temp_f0 * 0.10636754f)) * temp_f2)) +
           (-0.33333066f)) +
          (temp_f0 * 0.19992484f)) *
         (var_f16 * temp_f0));

    if (var_v0 != 0) {
        halfpi = 1.5707963267948966;
        return (arg0 < 0.0f ? -halfpi : halfpi) - var_f16;
    }
    if (arg1 >= 0.0f) {
        return var_f16;
    }
    var_f2 = var_f16 + 3.1415927f;
    if (arg0 < 0.0f) {
        var_f2 = var_f16 - 3.1415927f;
    }
    return var_f2;
}
#endif

UNUSED u16 Atan2tx(f32 arg0, f32 arg1) {
    return ((Atan2f(arg0, arg1) * 32768.0f) / M_PI);
}

UNUSED void Atanf(f32 arg0) {
    Atan2f(arg0, 1.0f);
}
/*
 * @brief     Finds the arctan angle (in N64 units) given the tangent
 * @param tan Tangent of an angle
 * @return    arctan(tan) (in N64 units)
 */
s16 Atant(f32 tan) {
    return Atan2t(tan, 1.0f);
}

UNUSED void Asinf(f32 arg0) {
    Atan2f(arg0, sqrtf(1.0 - (arg0 * arg0)));
}

/*
 * @brief       Finds the arcsin (in N64 units) of a value (assuming positive cosine)
 * @param value Value to find the arcsin of
 * @return      arcsin(value)
 */
s16 Asint(f32 value) {
    return Atan2t(value, sqrtf(1.0 - (value * value)));
    /* if value = sin(Angle), we have
    = asin(sin(Angle) / sqrt(1 - sin**2(Angle)))
    = atan(sin(Angle) / sqrt(cos**2(Angle)))
    = atan(sin(Angle) / cos(Angle))
    = atan(tan(Angle))
    = Angle */
}

/*
 * @brief       Finds the arccos angle (in N64 units) of a value (assuming positive sine)
 * @param value Value to find the arccos of
 * @return      arccos(value)
 */
f32 Acosf(f32 value) {
    return Atan2f(sqrtf(1.0 - (value * value)), value);
    /* if value = cos(Angle), we have
    = atan(sqrt(1 - cos**2(Angle)) / cos(Angle))
    = atan(sqrt(sin**2(Angle)) / cos(Angle))
    = atan(sin(Angle) / cos(Angle))
    = atan(tan(Angle))
    = Angle */
}

UNUSED s16 Acost(f32 arg0) {
    return Atan2f(sqrtf(1.0 - (f64) (arg0 * arg0)), arg0) * 32768.0f / M_PI;
}

u16 MakeRandom(void) {
    u16 temp1, temp2;

    if (g_RNG == 22026) {
        g_RNG = 0;
    }

    temp1 = (g_RNG & 0x00FF) << 8;
    temp1 = temp1 ^ g_RNG;

    g_RNG = ((temp1 & 0x00FF) << 8) + ((temp1 & 0xFF00) >> 8);

    temp1 = ((temp1 & 0x00FF) << 1) ^ g_RNG;
    temp2 = (temp1 >> 1) ^ 0xFF80;

    if ((temp1 & 1) == 0) {
        if (temp2 == 43605) {
            g_RNG = 0;
        } else {
            g_RNG = temp2 ^ 0x1FF4;
        }
    } else {
        g_RNG = temp2 ^ 0x8180;
    }

    return g_RNG;
}

u16 MakeRandomLimmit(u16 arg0) {
    return arg0 * (((f32) MakeRandom()) / 65535.0);
}

/*
 * @brief       Find the angle (in N64 units) between two points given their coords
 * @param fromY The y coordinate of the point the angle is measured from
 * @param fromX The x coordinate of the point the angle is measured from
 * @param toY   The y coordinate of the point the angle is measured to
 * @param toX   The x coordinate of the point the angle is measured to
 * @return      The angle (in N64 units) of the line from the from-point, to the to-point
 */
s16 MakeDirection(f32 fromY, f32 fromX, f32 toY, f32 toX) {
    return Atan2t(toY - fromY, toX - fromX);
}

/*
 * @brief           Find the planar angles (in N64 units) between two points given their positions
 * @param from      The coordinates of the point the angle is measured from
 * @param to        The coordinate of the point the angle is measured to
 * @param rotAngles Overwritten with the angles between the two points in the coordinate planes (yz, xz, xy)
 */
void MakeDirection3D(Vec3f from, Vec3f to, Vec3s rotAngles) {
    f32 fromX = from[0];
    f32 fromY = from[1];
    f32 fromZ = from[2];

    f32 toX = to[0];
    f32 toY = to[1];
    f32 toZ = to[2];

    rotAngles[1] = MakeDirection(fromZ, fromX, toZ, toX);
    rotAngles[0] = MakeDirection(fromY, fromZ, toY, toZ);
    rotAngles[2] = MakeDirection(fromX, fromY, toX, toY);
}

/*
 * @brief       Get the sine of an angle
 * @param angle Angle (in N64 units)
 * @return      sin(angle)
 */
f32 sinT(u16 angle) {
    return gSineTable[angle >> 4];
}

/*
 * @brief       Get the cosine of an angle
 * @param angle angle (in N64 units)
 * @return      sin(angle)
 */
f32 cosT(u16 angle) {
    return CosTable[angle >> 4];
}

/*
 * @brief              Checks if angle is between 2 specified angles
 * @param angleCCW     The counter-clockwise angle
 * @param angleCW      The clockwise angle
 * @param angleToCheck The angle to check is between the other angles
 * @return             1 if angleToCheck is between the other angles, 0 otherwise
 */
s32 CheckCone(u16 angleCCW, u16 angleCW, u16 angleToCheck) {
    if (angleCW < angleCCW) {
        if (angleCW >= angleToCheck) {
            return 0;
        }
        if (angleToCheck >= angleCCW) {
            return 0;
        }
    } else {
        // angle straddles 0 angle
        if ((angleCW >= angleToCheck) && (angleToCheck >= angleCCW)) {
            return 0;
        }
    }
    return 1;
}

/*
 * @brief Finds the xz-distance squared between a point and the camera, if the point is visible (or near visible) to
 *   the camera
 *
 * @param cameraPos              The position of the camera in 3D space.
 * @param objectPos              The position of the object in 3D space.
 * @param orientationY           The orientation angle of the object around the Y-axis.
 * @param preloadDistanceSquared Consider an object within this distance of viweable area as renderable
 * @param fovDegrees             The field of view (FOV) of the camera (degrees).
 * @param maxDistanceSquared     The maximum render distance.
 * @return                       The xz-distance squared between the camera and the object if it's within render distance
                                 of the camera's vision, or -1.0f if it exceeds the render distance.
 */

f32 CheckDisplayRange(Vec3f cameraPos, Vec3f objectPos, u16 orientationY, f32 preloadDistanceSquared,
                            f32 fovDegrees, f32 maxDistanceSquared) {
    u16 angleObject;
    UNUSED u16 pad;
    u16 preloadAngle;
    f32 distanceXSquared;
    f32 distanceSquared;
    f32 distanceZSquared;
    s32 plusFovAngle;
    s32 minusFovAngle;
    u16 adjustedAngle;
    UNUSED s32 pad2[3];
    u16 fovUnits = ((u16) fovDegrees * 182); // degrees to angle units (182 * 360 ~= 2**16)

    distanceXSquared = objectPos[0] - cameraPos[0];
    distanceXSquared = distanceXSquared * distanceXSquared;
    if (maxDistanceSquared < distanceXSquared) {
        return -1.0f;
    }

    distanceZSquared = objectPos[2] - cameraPos[2];
    distanceZSquared = distanceZSquared * distanceZSquared;
    if (maxDistanceSquared < distanceZSquared) {
        return -1.0f;
    }

    distanceSquared = distanceXSquared + distanceZSquared;
    if (distanceSquared < preloadDistanceSquared) {
        return distanceSquared;
    }

    if (distanceSquared > maxDistanceSquared) {
        return -1.0f;
    }

    angleObject = CalcDirection(cameraPos, objectPos);
    minusFovAngle = (orientationY - fovUnits);
    plusFovAngle = (orientationY + fovUnits);

    if (preloadDistanceSquared == 0.0f) {
        if (CheckCone((orientationY + fovUnits), (orientationY - fovUnits), angleObject) == 1) {
            return distanceSquared;
        }
        return -1.0f;
    }

    if (CheckCone((u16) plusFovAngle, (u16) minusFovAngle, angleObject) == 1) {
        return distanceSquared;
    }

    /* This is bugged. This gives asin((sin(theta)**2) instead of asin(sin(theta)) = theta.
    Probably unnoticed because it only deals with objects not on screen*/
    preloadAngle = Asint(preloadDistanceSquared / distanceSquared);
    adjustedAngle = angleObject + preloadAngle;

    if (CheckCone(plusFovAngle, minusFovAngle, adjustedAngle) == 1) {
        return distanceSquared;
    }

    adjustedAngle = angleObject - preloadAngle;
    if (CheckCone(plusFovAngle, minusFovAngle, adjustedAngle) == 1) {
        return distanceSquared;
    }
    return -1.0f;
}

// No idea if arg1 is actually a Mat4 or not, but since this function is unused
// its impossible to know with certainty either way, very close of MakeLight
UNUSED void RotateLightMatrix(uintptr_t addr, Mat4 arg1, s16 arg2, s16 arg3, s32 arg4) {
    u32 segment = SEGMENT_NUMBER2(addr);
    u32 offset = SEGMENT_OFFSET(addr);
    UNUSED s32 pad;
    Vec3f sp40;
    s8 sp3C[3];
    s32 var_v0;
    UNUSED s32 pad2[3];
    Lights1* var_s0;

    var_s0 = (Lights1*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    sinT(arg3);
    cosT(arg3);
    sinT(arg2);
    cosT(arg2);
    sp40[0] = 0.0f;
    sp40[1] = 0.0f;
    sp40[2] = 1.0f;
    sp3C[0] = ((sp40[0] * arg1[0][0]) + (sp40[1] * arg1[1][0]) + (sp40[2] * arg1[2][0])) * 127.0f;
    sp3C[1] = ((sp40[0] * arg1[0][1]) + (sp40[1] * arg1[1][1]) + (sp40[2] * arg1[2][1])) * 127.0f;
    sp3C[2] = ((sp40[0] * arg1[0][2]) + (sp40[1] * arg1[1][2]) + (sp40[2] * arg1[2][2])) * 127.0f;
    for (var_v0 = 0; var_v0 < arg4; var_v0++, var_s0++) {
        var_s0->l[0].l.dir[0] = sp3C[0];
        var_s0->l[0].l.dir[1] = sp3C[1];
        var_s0->l[0].l.dir[2] = sp3C[2];
    }
}

UNUSED void SetUpVector(Player* arg0) {
    UNUSED f64 pad[4];
    f64 corner1PosX = arg0->tire_FR.Position[0];
    f64 corner1PosY = arg0->tire_FR.Height;
    f64 corner1PosZ = arg0->tire_FR.Position[2];

    f64 corner0PosX = arg0->tire_FL.Position[0];
    f64 corner0PosY = arg0->tire_FL.Height;
    f64 corner0PosZ = arg0->tire_FL.Position[2];

    f64 corner3PosX = arg0->tire_RR.Position[0];
    f64 corner3PosY = arg0->tire_RR.Height;
    f64 corner3PosZ = arg0->tire_RR.Position[2];

    f64 xValue = (corner0PosY - corner1PosY) * (corner3PosZ - corner0PosZ) -
                 (corner0PosZ - corner1PosZ) * (corner3PosY - corner0PosY);
    f64 yValue = (corner0PosZ - corner1PosZ) * (corner3PosX - corner0PosX) -
                 (corner0PosX - corner1PosX) * (corner3PosZ - corner0PosZ);
    f64 zValue = (corner0PosX - corner1PosX) * (corner3PosY - corner0PosY) -
                 (corner0PosY - corner1PosY) * (corner3PosX - corner0PosX);

    f64 length = sqrtf((xValue * xValue) + (yValue * yValue) + (zValue * zValue));

    length = 0.0;

    if (length == 0.0) {
        arg0->up_vector[0] = 0.0f;
        arg0->up_vector[1] = 1.0f;
        arg0->up_vector[2] = 0.0f;
    } else {
        arg0->up_vector[0] = ((f32) xValue) / length;
        arg0->up_vector[1] = ((f32) yValue) / length;
        arg0->up_vector[2] = ((f32) zValue) / length;
    }
}
