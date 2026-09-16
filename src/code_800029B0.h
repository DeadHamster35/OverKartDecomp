#ifndef CODE_800029B0_H
#define CODE_800029B0_H

#include <macros.h>
#include <ultra64.h>
#include <actor_types.h>
#include "camera.h"

typedef struct Screen {
    /* 0x00 */ Controller* cont; // gControllers ptr 800F6910
    /* 0x04 */ Camera* camera;   // Player camera ptr
    /* 0x08 */ Player* kart;     // Player ptr 800F6990
    /* 0x0C */ s32* matrix;      // Library AffineMtx*; keep s32* (D_8015F790)
    /* 0x10 */ Viewport viewport;
    /* 0x20 */ f32 screen_aspect;
    /* 0x24 */ f32 screen_angle;
    /* 0x28 */ s16 horizon;
    /* 0x2A */ s16 pad;
    /* 0x2C */ s16 width;
    /* 0x2E */ s16 height;
    /* 0x30 */ s16 posx;
    /* 0x32 */ s16 posy;
    /* 0x34 */ s16 flag;
    /* 0x36 */ s16 camera_direction;
    /* 0x38 */ s16 camera_point;
    /* 0x3A */ s16 unk42;
    /* 0x3C */ s32 pad2;
} Screen; // size = 0x40

/* Function Prototypes */

void set_music_volume(void);
void InitialRaceSequence(void);
void DelayInitialMap(void);
void initialize_first_once(void);
void initial_ending_map(void);

extern s16 g_loadedcourseFlag;
extern u16 g_musicUserVolumeFlag;
extern s32 D_800DC5AC;
extern u16 g_ClearCFBFlag;
extern u16 gBackgroundFlag;
extern u16 two_d_flag;
extern u16 g_skyToggle;
extern u16 g_fadeOutFlag;
extern u16 g_fadeOutCounter;
extern u16 g_fadeOutCounter2;
extern u16 D_800DC5CC;
extern s32 D_800DC5D0;
extern s32 D_800DC5D4;
extern s32 D_800DC5D8;
extern s32 D_800DC5DC;
extern s32 D_800DC5E0;
// This is tracking which credit "state" we're in, decides which credits are shown (and probably other stuff)
extern u16 D_800DC5E4;
extern s32 win_kart;

extern Screen D_8015F480[4];
extern Screen* D_800DC5EC;
extern Screen* D_800DC5F0;
extern Screen* D_800DC5F4;
extern Screen* D_800DC5F8;
extern u16 pause_flag;
extern u8* pAppNmiBuffer;
extern s32 g_ScreenFlip; // D_800DC604
extern s16 gCreditsCourseId;
extern s16 g_ItemSetFlag;

extern FaceStruct* gFaceBuffer;
extern u16* collision_table;
extern u16 g_courseFaceMaxIndex;
extern u16 collision_pointer;
extern u32 facenumber;

extern Vec3f D_8015F590;
extern s32 wall_flag;
extern s32 flat_flag;
extern s32 D_8015F5A4;

extern Vtx* vertex_buffer2[];
extern s16 maximum_x;
extern s16 minimum_x;

extern s16 maximum_y;
extern s16 minimum_y;

extern s16 maximum_z;
extern s16 minimum_z;
extern s16 maxlimmity;
extern s16 minlimmity;
extern u16 point_number;
extern s16 shadowface;
extern s16 outsideface;
extern u16 kame_counter;

extern u16 g_mooSoundCounter;
extern u16 g_mooSoundPointer;
extern f32 g_mooSoundLastDistance;
extern Vec3f g_mooSoundPosition;
extern UNUSED u32 D_8015F718[3];
extern size_t free_memory_size;
extern uintptr_t FreeMemoryPointer;
extern uintptr_t LastMemoryPointer;
extern uintptr_t VertexMemoryPointer;
extern uintptr_t ObjectMemoryPointer;
extern u8 ok_ObjectPoolStart[];
extern u8 ok_ObjectPoolEnd[];

#define FACE_BUFFER_START 0x80625800
#define FACE_BUFFER_END 0x80700000
#define VERTEX_MEMORY_TOP 0x80700000
#define VERTEX_MEMORY_FLOOR 0x80625800
#define LAST_MEMORY_TOP 0x80800000
#define LAST_MEMORY_FLOOR 0x80725800

extern u32 map_memory_pointer;
extern uintptr_t StaticMemoryPointer;
extern Vec3f g_waterfallSoundPos;
extern Vec3f g_crowdSoundPos1;
extern Vec3f g_crowdSoundPos2;
extern Vec3f g_crowdSoundPos3;
extern Vec3f g_crowdSoundPos4;

extern f32 g_mirrorValue;
extern s32 g_ScreenCounter;

extern s32 D_8015F790[];
extern u16 replay_flag;
extern u16 retry_flag;
extern u16 finish_zoom_flag;
extern f32 gTimePlayerLastTouchedFinishLine[];

extern u8* wincount2p;
extern u8* wincount3p;
extern u8* wincount4p;
extern u8* battlewincount2p;
extern u8* battlewincount3p;
extern u8* battlewincount4p;

extern Vec3f g_goalBannerPos;
extern s32 itembox_flag;

extern s32 tree_flag;
extern f32 g_waterHeight;
extern f32 g_waterVelocity;
extern s16 rank_buffer[]; // Player index at each position
extern u16 g_StaticObjectCount;

extern UNUSED u8 D_80162578[];
extern s16 gDebugPathCount;
extern s16 sIsController1Unplugged;
extern s32 fog_red;
extern s32 fog_green;
extern s32 fog_blue;
extern s32 D_801625F8;
extern f32 D_801625FC;

#endif
