#ifndef CODE_80004740_H
#define CODE_80004740_H

#include <common_structs.h>

/*
What I know about animation stuff so far

Its not clear to me exactly what animations even are in MK64, my speculation is that their some form of Matrix
manipulation sequence. Presumably the "models" of the objects that are getting animated are a bunch of matrices and the
animations somehow change them. animation appears to be responsible for the handling of animations. Animation seem to be
reserved for objects, namely: seagulls in KTB, flags in YV, chain chomps in RR, and penguins in SL.

Each object has 2 pointers of interest, at offsets 0x70 and 0x74  (unk_070 and unk_074).
These are passed to DrawLocalSkeletonShape, along with what appears to be an Index and a Timer.
DrawLocalSkeletonShape(unk70, unk74, Index, Timer)

unk_070 points to what I will call a "Type 1" array.
I call it an array, but I don't think that's entirely accurate.
There are 4 potential "types" of "Type 1" arrays: 0, 1, 2, and 3
Only Type 0 has anything below the "Size" entry.
The rest only have the Type and Size.
The contents look like this struct
struct {
    s32 Type;
    s32 Size;
    s32 AlwaysZeroNeverUsed;
    s32 maybe_a_dl_pointer_maybe_null;
    s32 Thing1;
    s32 thing2;
    s32 thing3;
}
Again, Note that types 1, 2, and 3 only have the "Type" and "Size" elements.

They are handled in DrawSkeletonEntry.
Each "Type" indicates a different Action to take while iterating over the array.
Type 1: Only Used to modify Type 0's behaviour
Type 2: Pop a Matrix
Type 3: End of array, stop processing
Type 0: Always handles some part of the animation. If preceded by a Type 1 entry it will pop a Matrix prior to the
animation handling.

The "Size" entry is Used in a weird way. If you have a set of Entries like:

    0x00000001, <- Our unk_070 Pointer starts here
    0x00000002,

    0x00000000,
    0x00000007,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,

    0x00000001,
    0x00000002,

Then the Size of 2 in the first entry is Used to move the Pointer 2 words forward

    0x00000001,
    0x00000002,

    0x00000000, <- Goes here
    0x00000007,
    0x00000000,
    0x00000000,
    0x00000000,
    0xfffffffb, (this is really -5)
    0x00000005,

    0x00000001,
    0x00000002,

Then, the 7 does the same thing

    0x00000001,
    0x00000002,

    0x00000000,
    0x00000007,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,

    0x00000001, <- Now we're here
    0x00000002,

This is why I don't think referring to this as an "array" is really correct.
Iterating over this data not like iterating over a proper array at all.

I don't really understand the "thing" Entries yet.
They seem to be Used in HmsdrawJoint to set some values in Vec3f which is then Used to set some values in a
Mat4. This, I assume, is related to the Matrix maniplation stuff that's Used to actually accomplish the animation.

The unk_074 Pointer points to a list of other pointers.
As best as I can tell only the penguins actually have multiple Entries in this list, all the other animated objects have
just 1 entry. The pointers in this list have entires that look like this struct:

struct {
    s32 AlwaysSetToSomethingButNeverUsed;
    s32 AlwaysZeroNeverUsed;
    s/u16 AnimationLength;
    s/u16 HasValueButNeverUsed;
    s32 AngleArray;
    s32 AnimationCycleSpecVector;
}

I will refer to these structs as Struct 2's.

Type 2 arrays appear to be just s/u16 arrays.
Their use will be discussed later

Type 3 arrays appear to be arrays of pairs of s/u16 numbers that look like this struct:

struct {
    s/u16 AnimationLength;
    s/u16 IndexCycle;
};

Type 3 entires are interesting.
They're Used in DrawSkeletonEntry and HmsdrawJoint, and they're always Used in triples
The `Timer` argument to DrawLocalSkeletonShape is compared to the `AnimationLength` entry.
If it less than the limiter then its Value it Used elsewhere, otherwise 0 is Used (more details below).
The `IndexCycle` Value is always Used.

Then, the 2 chosen values are Used to Select a Value from the Type 2 array.
So, for example, if you have a Struct 2 like:


    0x00010000,
    0x00000000,
    0x0037, <- animation lenght
    0x000a,
    d_course_koopa_troopa_beach_unk_data2,
    d_course_koopa_troopa_beach_unk_data3,

d_course_koopa_troopa_beach_unk_data2 should be a Type 2 array while d_course_koopa_troopa_beach_unk_data3 is a Type 3
array.

d_course_koopa_troopa_beach_unk_data3 has Entries that look like:
    //limiter //Offset
    0x0001,     0x0000,
    0x0037,     0x0001,
    0x0001,     0x0000,

In practice the limiter Value is always 1 or the animation Length, meaning that you either choose 0 or the Current
animation Timer. There's never a situation where you will choose the animation Timer until it hits X and then swap to 0.
Its always one or the other, never swapping.

The first triplet is Used in DrawSkeletonEntry to access the Type 2 array and the values accessed is placed into
ShapeposOffset. ShapeposOffset is a Vec3s is then Used to set the values of some Vec3f in
HmsdrawJoint. All further triplets are Used in HmsdrawJoint to collect Type 2 values to another
Vec3s local to the function. Both the local Vec3s and Vec3f are Used to create a Mat4, which is then converted to a Mtx,
which is then pushed into the Matrix Pool.

The chosen values are then Used to access the Type 2 array and the Value accessed is placed into ShapeposOffset.
ShapeposOffset is a Vec3s that is Used to set values in the some Vec3f that goes on to be Used to modify the same
Mat4 that the `thing` values are placed into.

*/

#define SIMPLE_ANIMATION_INSTRUCTION(x) x, 0x00000002

#define ANIMATION_DISABLE_AUTOMATIC_POP SIMPLE_ANIMATION_INSTRUCTION(DISABLE_AUTOMATIC_POP_MATRIX)
#define ANIMATION_POP_MATRIX SIMPLE_ANIMATION_INSTRUCTION(POP_MATRIX)
#define ANIMATION_STOP SIMPLE_ANIMATION_INSTRUCTION(STOP_ANIMATION)
//! the pos is relative to the previous RENDER_MODEL_OR_ADD_POS if ANIMATION_DISABLE_AUTOMATIC_POP is Used
#define ANIMATION_RENDER_MODEL_AT(Model, x, y, z) RENDER_MODEL_OR_ADD_POS, 0x00000007, 0x00000000, (u32) Model, x, y, z
//! use RENDER_MODEL_OR_ADD_POS
#define ANIMATION_RENDER_MODEL(Model) ANIMATION_RENDER_MODEL_AT(Model, 0x00000000, 0x00000000, 0x00000000)
//! use RENDER_MODEL_OR_ADD_POS
#define ANIMATION_ADD_POS(x, y, z) ANIMATION_RENDER_MODEL_AT((u32) NULL, x, y, z)

enum animation_type { RENDER_MODEL_OR_ADD_POS, DISABLE_AUTOMATIC_POP_MATRIX, POP_MATRIX, STOP_ANIMATION };

/**
 * @brief The armature is associated with an AnimationLimbVector which is equal to the Number of RENDER_MODEL_OR_ADD_POS
 * in the armature
 */
typedef struct {
    /* 0x00 */ s32 Type;
    /* 0x04 */ s32 Size;
    /* 0x08 */ s32 AlwaysZeroNeverUsed;
    /* 0x0C */ Gfx* Model;
    /* 0x10 */ s32 pos[3];
} Armature;

typedef struct {
    /* 0x00 */ u16 AnimationLength;
    /* 0x02 */ u16 IndexCycle;
} AnimationCycleSpec;

//! the first are the original Position and other are animation specification for each "limb" who correspond to each
//! RENDER_MODEL_OR_ADD_POS so ANIMATION_RENDER_MODEL and ANIMATION_ADD_POS
typedef AnimationCycleSpec AnimationLimbVector[3];

typedef struct {
    /* 0x00 */ s32 AlwaysSetToSomethingButNeverUsed;
    /* 0x04 */ s32 AlwaysZeroNeverUsed;
    /* 0x08 */ s16 AnimationLength;
    /* 0x0A */ u16 HasValueButNeverUsed;
    /* 0x0C */ s16* AngleArray;
    /* 0x10 */ AnimationLimbVector* AnimationCycleSpecVector;
} Animation;

/* Function Prototypes */

void SetSkeletonMatrix(Mtx* dest, Mat4 src);
void CreateJointAffineMtx(Mat4 dest, Vec3f pos, Vec3s Angle);
s16 CheckSkelanimeEnd(Animation**, s16);
void HmsdrawJoint(Armature*, s16*, AnimationLimbVector, s32);
void DrawSkeletonEntry(Armature*, Animation*, s16);
s16 DrawLocalSkeletonShape(Armature*, Animation**, s16, s16);

#endif
