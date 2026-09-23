#ifndef CAMERA_H
#define CAMERA_H

#include <common_structs.h>

/**
 * @todo put this define in types.h or similar.
 * Certain functions are marked as having return values, but do not
 * actually return a Value. This causes undefined behavior, which we'd rather
 * avoid on modern GCC. This only impacts -O2 and can matter for both the function
 * itself and functions that call it.
 */
#ifdef AVOID_UB
#define BAD_RETURN(cmd) void
#else
#define BAD_RETURN(cmd) cmd
#endif

typedef struct Shake {
    /* 0x00 */ f32 CamFirstspeed;
    /* 0x04 */ s16 unk_4;
    /* 0x06 */ s16 unk_6;
    /* 0x08 */ s16 CamTimer;
} Shake; /* Size = 0x0C. Library overlay is 0x18; this tree keeps 0x0C. */
typedef Shake UnkCameraInner;

typedef struct {
    /* 0x00 */ Vec3f CameraPos;
    /* 0x0C */ Vec3f LookatPos;
    // This is expected to be a normalized vector, indicates what Direction is "up" for the Camera
    /* 0x18 */ Vec3f UpVector;
    // I think these are the "nautical angles" between CameraPos and LookatPos
    // CameraDirection[0] = Roll? Does nothing?, [1] = yaw, [2] = Pitch
    /* 0x24 */ Vec3s CameraDirection;
    /* 0x2A */ u16 Flag;
    /* 0x2C */ s16 ChaseDirection;
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ Vec3f CameraVector;
    /* 0x3C */ Vec3f LookatVector;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 Unk4C;
    /* 0x50 */ s32 Unk50;
    /* 0x54 */ Bump Bump;
    // When you hit a wall (or another Driver) the Camera's pos and LookAt bounce up and down. This is the Velocity(?)
    // of that bouncing. 0x0C blob; Library overlay Shake is 0x18.
    /* 0x94 */ Shake Shake;
    // Timer for wall-hit bounce. Counts up instead of down
    /* 0xA0 */ f32 OffsetLook;
    /* 0xA4 */ s32 z2;
    /* 0xA8 */ s32 z3;
    /* 0xAC */ s16 OldChaseDirection;
    /* 0xAE */ s16 Watch;
    // Seems related to Camera movement during drifting
    /* 0xB0 */ s16 Stickdeg;
    /* 0xB2 */ s16 UnkB2;
    /* 0xB4 */ f32 ScreenViewAngle;
} Camera; /* Size = 0xB8 */

void InitialCamera(f32, f32, f32, s16, u32, s32);
void ShakeCamera(Camera*);
void ShakeHitCamera(Player*, f32);
void OpendirectionCamera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s32, s32);
void DirectionCamera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s32*, s16, s16);
void DirectionCameraLookup(Player*, Camera*, Vec3f, f32*, f32*, f32*, s16, s16);
void DirectionFixcamera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s16);
void DirectionBattlecamera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s32*, s16, s16);
void Openingview(Camera*, Player*, s8);
void KartView(Camera*, Player*, s8);
void LookupView(Camera*, Player*, s8);
void Battleview(Camera*, Player*, s8);
void CameraControl(Player*, Camera*, s8);
void MakeViewAng(Player*, f32*);
void ChangeOpneview(s32);

extern f32 D_800DDB30[];

extern Camera Cameras[];
extern Camera* Camera1;
extern Camera* Camera2;
extern Camera* Camera3;
extern Camera* Camera4;

// end of Camera.c variables

extern s8 D_80164A89;

extern s32 D_80164A08[4];

extern s32 OpeningFlag;
extern s32 D_80164A2C;
extern f32 D_80164A30;
extern f32 D_80164A90[];
extern f32 D_80164AA0[];

#endif
