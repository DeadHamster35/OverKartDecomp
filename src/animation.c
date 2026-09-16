#include <ultra64.h>
#include <macros.h>
#include <mk64.h>
#include "math_util.h"
#include "animation.h"
#include "memory.h"
#include <main.h>
#include <PR/gbi.h>
#include "code_80057C60.h"

Vec3s shapepos_offset;
s16 shapepos_flag;
s16 push_flag;
s16 pushcount;

void SetSkeletonMatrix(Mtx* dest, Mat4 src) {
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

void CreateJointAffineMtx(Mat4 dest, Vec3f pos, Vec3s angle) {
    register f32 sx = sinT(angle[0]);
    register f32 cx = cosT(angle[0]);

    register f32 sy = sinT(angle[1]);
    register f32 cy = cosT(angle[1]);

    register f32 sz = sinT(angle[2]);
    register f32 cz = cosT(angle[2]);

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

void hmsdraw_joint(Armature* arg0, s16* arg1, AnimationLimbVector arg2, s32 timeCycle) {
    Vec3f pos;
    Vec3s angle;
    Mat4 modelMatrix;
    s32 i;
    s32 some_offset;
    Gfx* model;
    Gfx* virtualModel;
    virtualModel = arg0->model;
    if (shapepos_flag == 0) {
        for (i = 0; i < 3; i++) {
            pos[i] = shapepos_offset[i] + arg0->pos[i];
        }
        shapepos_flag += 1;
    } else {
        for (i = 0; i < 3; i++) {
            pos[i] = arg0->pos[i];
        }
    }
    for (i = 0; i < 3; i++) {
        if (timeCycle < arg2[i].animation_length) {
            some_offset = timeCycle;
        } else {
            some_offset = 0;
        }
        angle[i] = arg1[arg2[i].indexCycle + some_offset];
    }

    CreateJointAffineMtx(modelMatrix, pos, angle);
    SetSkeletonMatrix(&gDynamicP->mtxHud[modelingnum], modelMatrix);
    pushcount += 1;
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL2(&gDynamicP->mtxHud[modelingnum++]),
              G_MTX_PUSH | G_MTX_MUL | G_MTX_MODELVIEW);
    if (virtualModel != NULL) {
        model = SegmentToVirtual(virtualModel);
        gSPDisplayList(gDisplayListHead++, model);
    }
}

void draw_skeleton_entry(Armature* animation, Animation* arg1, s16 timeCycle) {
    UNUSED u32* temp;
    s16* angle_array;
    s32 some_offset;
    AnimationLimbVector* animation_cycle_list;
    s32 animation_type;
    s32 someIndex;

    angle_array = SegmentToVirtual(arg1->angle_array);
    animation_cycle_list = SegmentToVirtual(arg1->animation_cycle_spec_vector);
    pushcount = 0;
    shapepos_flag = 0;
    for (someIndex = 0; someIndex < 3; someIndex++) {
        if (timeCycle < (*animation_cycle_list)[someIndex].animation_length) {
            some_offset = timeCycle;
        } else {
            some_offset = 0;
        }
        shapepos_offset[someIndex] = angle_array[(*animation_cycle_list)[someIndex].indexCycle + some_offset];
    }
    animation_cycle_list++;
    push_flag = 0;
    do {
        animation_type = animation->type;
        switch (animation_type) { /* irregular */
            case STOP_ANIMATION:
                break;
            case DISABLE_AUTOMATIC_POP_MATRIX:
                push_flag = 1;
                break;
            case POP_MATRIX:
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                pushcount -= 1;
                break;
            case RENDER_MODEL_OR_ADD_POS:
                if (push_flag == 0) {
                    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                    pushcount -= 1;
                }
                hmsdraw_joint(animation, angle_array, *animation_cycle_list, (s32) timeCycle);
                push_flag = 0;
                animation_cycle_list++;
                break;
        }
        animation = (Armature*) ((u32*) animation + animation->size);
    } while (animation_type != STOP_ANIMATION);
}

s16 DrawLocalSkeletonShape(Armature* virtualArmature, Animation** virtualListAnimation, s16 animationIndex,
                          s16 timeCycle) {
    Armature* armature;
    Animation* animation;
    Animation** listAnimation;

    armature = SegmentToVirtual(virtualArmature);
    listAnimation = SegmentToVirtual(virtualListAnimation);      // Convert the array's address
    animation = SegmentToVirtual(listAnimation[animationIndex]); // Convert an array element's address
    if (timeCycle >= animation->animation_length) {
        timeCycle = 0;
    }
    draw_skeleton_entry(armature, animation, timeCycle);
    timeCycle++;
    if (timeCycle >= animation->animation_length) {
        timeCycle = 0;
    }
    return timeCycle;
}

s16 check_skelanime_end(Animation** addr, s16 offset) {
    Animation** item = SegmentToVirtual(addr);
    Animation* temp = (Animation*) SegmentToVirtual((void*) item[offset]);

    return temp->animation_length - 1;
}
