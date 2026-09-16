#ifndef CAMERA_H
#define CAMERA_H

#include <common_structs.h>

/**
 * @todo put this define in types.h or similar.
 * Certain functions are marked as having return values, but do not
 * actually return a value. This causes undefined behavior, which we'd rather
 * avoid on modern GCC. This only impacts -O2 and can matter for both the function
 * itself and functions that call it.
 */
#ifdef AVOID_UB
#define BAD_RETURN(cmd) void
#else
#define BAD_RETURN(cmd) cmd
#endif

typedef struct Shake {
    /* 0x00 */ f32 cam_firstspeed;
    /* 0x04 */ s16 unk_4;
    /* 0x06 */ s16 unk_6;
    /* 0x08 */ s16 cam_timer;
} Shake; /* size = 0x0C. Library overlay is 0x18; this tree keeps 0x0C. */
typedef Shake UnkCameraInner;

typedef struct {
    /* 0x00 */ Vec3f camera_pos;
    /* 0x0C */ Vec3f lookat_pos;
    // This is expected to be a normalized vector, indicates what direction is "up" for the camera
    /* 0x18 */ Vec3f up_vector;
    // I think these are the "nautical angles" between camera_pos and lookat_pos
    // camera_direction[0] = roll? Does nothing?, [1] = yaw, [2] = pitch
    /* 0x24 */ Vec3s camera_direction;
    /* 0x2A */ u16 flag;
    /* 0x2C */ s16 chase_direction;
    /* 0x2E */ s16 unk_2E;
    /* 0x30 */ Vec3f camera_vector;
    /* 0x3C */ Vec3f lookat_vector;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ s32 unk_50;
    /* 0x54 */ Bump bump;
    // When you hit a wall (or another driver) the camera's pos and lookAt bounce up and down. This is the velocity(?)
    // of that bouncing. 0x0C blob; Library overlay Shake is 0x18.
    /* 0x94 */ Shake shake;
    // Timer for wall-hit bounce. Counts up instead of down
    /* 0xA0 */ f32 offset_look;
    /* 0xA4 */ s32 z2;
    /* 0xA8 */ s32 z3;
    /* 0xAC */ s16 old_chase_direction;
    /* 0xAE */ s16 watch;
    // Seems related to camera movement during drifting
    /* 0xB0 */ s16 stickdeg;
    /* 0xB2 */ s16 unk_B2;
    /* 0xB4 */ f32 screen_view_angle;
} Camera; /* size = 0xB8 */

void initial_camera(f32, f32, f32, s16, u32, s32);
void ShakeCamera(Camera*);
void ShakeHitCamera(Player*, f32);
void opendirection_camera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s32, s32);
void direction_camera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s32*, s16, s16);
void direction_camera_lookup(Player*, Camera*, Vec3f, f32*, f32*, f32*, s16, s16);
void direction_fixcamera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s16);
void direction_battlecamera(Player*, Camera*, Vec3f, f32*, f32*, f32*, s32*, s16, s16);
void openingview(Camera*, Player*, s8);
void KartView(Camera*, Player*, s8);
void lookup_view(Camera*, Player*, s8);
void battleview(Camera*, Player*, s8);
void CameraControl(Player*, Camera*, s8);
void make_view_ang(Player*, f32*);
void change_opneview(s32);

extern f32 D_800DDB30[];

extern Camera cameras[];
extern Camera* camera1;
extern Camera* camera2;
extern Camera* camera3;
extern Camera* camera4;

// end of camera.c variables

extern s8 D_80164A89;

extern s32 D_80164A08[4];

extern s32 opening_flag;
extern s32 D_80164A2C;
extern f32 D_80164A30;
extern f32 D_80164A90[];
extern f32 D_80164AA0[];

#endif
