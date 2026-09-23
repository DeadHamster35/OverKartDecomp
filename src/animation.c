#include <ultra64.h>
#include <macros.h>
#include <mk64.h>
#include "math_util.h"
#include "animation.h"
#include "memory.h"
#include <Main.h>
#include <PR/gbi.h>
#include "code_80057C60.h"

Vec3s ShapeposOffset;
s16 ShapeposFlag;
s16 PushFlag;
s16 Pushcount;

void SetSkeletonMatrix(Mtx* dest, Mat4 src) {
#ifdef AVOID_UB
    // Avoid Type-casting which is technically UB by calling the equivalent
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

void CreateJointAffineMtx(Mat4 dest, Vec3f pos, Vec3s Angle) {
    register f32 sx = SinT(Angle[0]);
    register f32 cx = CosT(Angle[0]);

    register f32 sy = SinT(Angle[1]);
    register f32 cy = CosT(Angle[1]);

    register f32 sz = SinT(Angle[2]);
    register f32 cz = CosT(Angle[2]);

    dest[0][0] = cy * cz;
    dest[0][1] = cy * sz;
    dest[0][2] = -sy;
    dest[0][3] = 0.0f;

    dest[1][0] = sx * sy * cz - cx * sz;
    dest[1][1] = sx * sy * sz + cx * cz;
    dest[1][2] = sx * cy;
    dest[1][3] = 0.0f;

    dest[2][0] = cx * sy * cz + sx * sz;
    dest[2][1] = cx * sy * sz - sx * cz;
    dest[2][2] = cx * cy;
    dest[2][3] = 0.0f;

    dest[3][0] = pos[0];
    dest[3][1] = pos[1];
    dest[3][2] = pos[2];
    dest[3][3] = 1.0f;
}

void HmsdrawJoint(Armature* arg0, s16* arg1, AnimationLimbVector Arg2, s32 timeCycle) {
    Vec3f pos;
    Vec3s Angle;
    Mat4 modelMatrix;
    s32 i;
    s32 some_offset;
    Gfx* Model;
    Gfx* virtualModel;
    virtualModel = arg0->Model;
    if (ShapeposFlag == 0) {
        for (i = 0; i < 3; i++) {
            pos[i] = ShapeposOffset[i] + arg0->pos[i];
        }
        ShapeposFlag += 1;
    } else {
        for (i = 0; i < 3; i++) {
            pos[i] = arg0->pos[i];
        }
    }
    for (i = 0; i < 3; i++) {
        if (timeCycle < Arg2[i].AnimationLength) {
            some_offset = timeCycle;
        } else {
            some_offset = 0;
        }
        Angle[i] = arg1[Arg2[i].IndexCycle + some_offset];
    }

    CreateJointAffineMtx(modelMatrix, pos, Angle);
    SetSkeletonMatrix(&DynamicP->MTXHud[Modelingnum], modelMatrix);
    Pushcount += 1;
    gSPMatrix(DisplayListHead++, VIRTUAL_TO_PHYSICAL2(&DynamicP->MTXHud[Modelingnum++]),
              G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    if (virtualModel != NULL) {
        Model = SegmentToVirtual(virtualModel);
        gSPDisplayList(DisplayListHead++, Model);
    }
}

void DrawSkeletonEntry(Armature* animation, Animation* arg1, s16 timeCycle) {
    UNUSED u32* temp;
    s16* AngleArray;
    s32 some_offset;
    AnimationLimbVector* animation_cycle_list;
    s32 animation_type;
    s32 someIndex;

    AngleArray = SegmentToVirtual(arg1->AngleArray);
    animation_cycle_list = SegmentToVirtual(arg1->AnimationCycleSpecVector);
    Pushcount = 0;
    ShapeposFlag = 0;
    for (someIndex = 0; someIndex < 3; someIndex++) {
        if (timeCycle < (*animation_cycle_list)[someIndex].AnimationLength) {
            some_offset = timeCycle;
        } else {
            some_offset = 0;
        }
        ShapeposOffset[someIndex] = AngleArray[(*animation_cycle_list)[someIndex].IndexCycle + some_offset];
    }
    animation_cycle_list++;
    PushFlag = 0;
    do {
        animation_type = animation->Type;
        switch (animation_type) { /* irregular */
            case STOP_ANIMATION:
                break;
            case DISABLE_AUTOMATIC_POP_MATRIX:
                PushFlag = 1;
                break;
            case POP_MATRIX:
                gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
                Pushcount -= 1;
                break;
            case RENDER_MODEL_OR_ADD_POS:
                if (PushFlag == 0) {
                    gSPPopMatrix(DisplayListHead++, G_MTX_MODELVIEW);
                    Pushcount -= 1;
                }
                HmsdrawJoint(animation, AngleArray, *animation_cycle_list, (s32) timeCycle);
                PushFlag = 0;
                animation_cycle_list++;
                break;
        }
        animation = (Armature*) ((u32*) animation + animation->Size);
    } while (animation_type != STOP_ANIMATION);
}

s16 DrawLocalSkeletonShape(Armature* virtualArmature, Animation** virtualListAnimation, s16 animationIndex,
                          s16 timeCycle) {
    Armature* armature;
    Animation* animation;
    Animation** listAnimation;

    armature = SegmentToVirtual(virtualArmature);
    listAnimation = SegmentToVirtual(virtualListAnimation);      // Convert the array's Address
    animation = SegmentToVirtual(listAnimation[animationIndex]); // Convert an array element's Address
    if (timeCycle >= animation->AnimationLength) {
        timeCycle = 0;
    }
    DrawSkeletonEntry(armature, animation, timeCycle);
    timeCycle++;
    if (timeCycle >= animation->AnimationLength) {
        timeCycle = 0;
    }
    return timeCycle;
}

s16 CheckSkelanimeEnd(Animation** Addr, s16 Offset) {
    Animation** Item = SegmentToVirtual(Addr);
    Animation* temp = (Animation*) SegmentToVirtual((void*) Item[Offset]);

    return temp->AnimationLength - 1;
}
