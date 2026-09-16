#ifndef _COMMON_STRUCTS_H_
#define _COMMON_STRUCTS_H_

#include <ultra64.h>

typedef f32 Vec3f[3];
typedef f32 Vec4f[4];

typedef s32 Vec3iu[3];

typedef s16 Vec3s[3];
typedef u16 Vec3su[3];
typedef s16 Vec4s[4];

typedef f32 Mat3[3][3];
typedef f32 Mat4[4][4];

/* Library / Kimura aliases (Struct.h). Do not mass-replace Vec3f in the tree. */
typedef u8 uchar;
typedef u16 ushort;
typedef u32 uint;
typedef Vec3f Vector;
typedef Vec3s SVector;
typedef Vec3su USVector;
typedef Mat3 Matrix;
typedef Mat3 RotateMtx;
typedef Mat4 AffineMtx;
typedef Vp Viewport;

// might not be real, used by CheckWall
typedef struct {
    f32 x, y, z;
} Vec3fs;

// This was added as a silly idea:
// In the data to use "A, B, Z, R" instead of hex numbers.
typedef enum { A = 0x80, B = 0x40, Z = 0x20, R = 0x10 } GhostController;

/***  types.h  ***/

typedef struct {
    /* 0x0 */ s16 start;
    /* 0x2 */ s16 end;
    /* 0x4 */ s32 action;
} ActionData; // size = 0x8
typedef ActionData CPUBehaviour;

enum SpTaskState {
    SPTASK_STATE_NOT_STARTED,
    SPTASK_STATE_RUNNING,
    SPTASK_STATE_INTERRUPTED,
    SPTASK_STATE_FINISHED,
    SPTASK_STATE_FINISHED_DP
};

struct SPTask {
    /*0x00*/ OSTask task;
    /*0x40*/ OSMesgQueue* msgqueue;
    /*0x44*/ OSMesg msg;
    /*0x48*/ enum SpTaskState state;
}; // size = 0x4C, align = 0x8

struct VblankHandler {
    OSMesgQueue* queue;
    OSMesg msg;
};

struct D_80150158 {
    s16 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

struct Controller {
    /* 0x00 */ s16 AnalogX;
    /* 0x02 */ s16 AnalogY;
    /* 0x04 */ u16 ButtonHeld;
    /* 0x06 */ u16 ButtonPressed;
    /* 0x08 */ u16 ButtonReleased;
    /* 0x0A */ u16 AnalogHeld;
    /* 0x0C */ u16 AnalogPressed;
    /* 0x0E */ u16 AnalogReleased;
}; // size = 0x10
typedef struct Controller Controller;

// Camera path struct? Or something like that. For GP race won scene?
struct UnkStruct_80287500 {
    Vec3f unk0; // xyz coordinates?
    f32 unkC;
    f32 unk10;
    f32 unk14; // rotation?
    s32 unk18;
    s32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
};

struct UnkStruct_800DDB40 {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u16 unk24;
    u16 unk26;
};

struct UnkStruct_802B53C8 {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
};
// end math util structs

struct UnkStruct_800DDB68 {
    s32* D_800ED600;
    s32* D_800ED608;
    s32* D_800ED610;
    s32* D_800ED618;
    s32* D_800ED620;
    s32* D_800ED628;
    s32* D_800ED630;
    s32* D_800ED638;
};
/*** Types.h end ***/

typedef struct {
    u8 button;
    s8 frameDuration;
    s8 stickY;
    s8 stickX;
} StaffGhost;
// 80160ADC Banana's remaining

typedef struct Bump {
    /* 0x00 */ u16 flag_xy;
    /* 0x02 */ u16 flag_yz;
    /* 0x04 */ u16 flag_zx;
    /* 0x06 */ u16 last_xy;
    /* 0x08 */ u16 last_yz;
    /* 0x0A */ u16 last_zx;
    /* 0x0C */ f32 distance_xy;
    /* 0x10 */ f32 distance_yz;
    /* 0x14 */ f32 distance_zx;
    /* 0x18 */ Vec3f bump_xy;
    /* 0x24 */ Vec3f bump_yz;
    /* 0x30 */ Vec3f bump_zx;
    /* 0x3C */ f32 dummy;
} Bump; // size = 0x40
typedef Bump Collision;

typedef struct Smoke {
    /* 0x00 */ f32 trx;
    /* 0x04 */ f32 try;
    /* 0x08 */ f32 trz;
    /* 0x0C */ f32 scale;
    /* 0x10 */ u16 lrflag;
    /* 0x12 */ u16 number; /* stock Particle.type: kind enum. Never confuse with .type below. */
    /* 0x14 */ f32 type;   /* stock Particle.status. A second apply of type→number will smash this. */
    /* 0x18 */ f32 dev;
    /* 0x1C */ s16 flag;
    /* 0x1E */ s16 timer;
    /* 0x20 */ s16 ang;
    /* 0x22 */ s16 unk_022;
    /* 0x24 */ f32 fwork1;
    /* 0x28 */ f32 fwork2;
    /* 0x2C */ s16 unk_02C;
    /* 0x2E */ s16 unk_02E;
    /* 0x30 */ s16 unk_030;
    /* 0x32 */ s16 unk_032;
    /* 0x34 */ s16 unk_034;
    /* 0x36 */ s16 unk_036;
    /* 0x38 */ s16 swork1;
    /* 0x3A */ s16 swork2;
    /* 0x3C */ s16 swork3;
    /* 0x3E */ s16 swork4;
    /* 0x40 */ s16 swork5;
    /* 0x42 */ s16 unk_042;
    /* 0x44 */ s16 unk_044;
    /* 0x46 */ s16 unk_046;
} Smoke; // size = 0x48
typedef Smoke Particle;

// This struct is almost identical to the GBI Vtx_t type,
// except that its missing the "flag" member.
typedef struct {
    s16 ob[3]; /* x, y, z */
    s16 tc[2]; /* texture coord */
    s8 ca[4];  /* color & alpha */
} CourseVtx;

/*
This struct has been copied (with only minor modifications) from
https://github.com/micro500/mariokart64/wiki/Surface-Map
on January 23rd, 2022
The original author is assumed to be RenaKunisaki
*/
typedef struct {
    /* 0x00 */ u16 flag;
    // Top bytes is a collections of flags, bottom byte is of unknown purpose
    // bit 7: 1 = only tangible if landed on, not if driven onto?
    //       very weird. game crashes sometimes when playing with this.
    // bit 6: 1 = Lakitu can drop you here (XXX verify)
    // bit 4: 1 = out of bounds
    // bit 3: 1 = player tumbles upon contact (may fall right through)
    /* 0x02 */ u16 status;

    // For AABB bounding-box style collision. Box style collision is cheaper than checking each vtx.
    /* 0x04 */ s16 minx;  // Minimum x coordinate
    s16 miny;             // Minimum y coordinate
    s16 minz;             // Minimum z coordinate
    s16 maxx;             // Maximum x coordinate
    s16 maxy;             // Maximum y coordinate
    /* 0x0A */ s16 maxz;  // Maximum z coordinate
    /* 0x10 */ Vtx* p1; // pointer to the 3 vertices of this poly
    Vtx* p2;
    Vtx* p3;
    // Face normal. Should really be Vec3f normal
    /* 0x1C */ f32 sola;
    /* 0x20 */ f32 solb;
    /* 0x24 */ f32 solc;
    /* 0x28 */ f32 sold;
} FaceStruct; // size = 0x2C
typedef FaceStruct CollisionTriangle;

typedef struct Tire {
    /* 0x00 */ Vec3f Position;
    /* 0x0C */ u8 Status;
    /* 0x0D */ u8 LastAxis;
    /* 0x0E */ u16 LastPointer;
    /* 0x10 */ f32 Height;
    /* 0x14 */ s32 Dummy;
} Tire; // size = 0x18
typedef Tire KartTire;

#define FRONT_LEFT 0
#define FRONT_RIGHT 1
#define BACK_LEFT 2
#define BACK_RIGHT 3

/*
 * OverKart overlays (Library/Struct.h).
 * USbit at 0x0A is still not a Player member (IDO bitfields != GCC).
 * Sus is embedded at 0xD98.
 */
// clang-format off
typedef struct USbit {
    ushort offroad : 1;
    ushort bot : 1;
    ushort superRival : 1;
    ushort noTireDMA : 1;
    ushort megaRival : 1;
    ushort autopilot : 1;
    ushort r5 : 1;
    ushort r6 : 1;
    ushort r7 : 1;
    ushort r8 : 1;
    ushort r9 : 1;
    ushort realKart : 5;
} USbit;
// clang-format on

typedef struct Sus {
    /* 0x00 */ s16 flag;
    /* 0x02 */ s16 flag1;
    /* 0x04 */ f32 position;
    /* 0x08 */ f32 accele;
    /* 0x0C */ s16 broken;
    /* 0x0E */ s16 jump_timer;
    /* 0x10 */ f32 jump_l;
    /* 0x14 */ f32 jump_firstspeed;
    /* 0x18 */ f32 jmp_acc_l;
    /* 0x1C */ s16 gachon_timer;
    /* 0x1E */ s16 broken_timer;
    /* 0x20 */ f32 swing_firstspeed;
    /* 0x24 */ f32 gachon_firstspeed;
    /* 0x28 */ f32 bound_firstspeed;
    /* 0x2C */ f32 broken_firstspeed;
    /* 0x30 */ f32 y_grv;
    /* 0x34 */ s16 bound_timer;
    /* 0x36 */ s16 swing_timer;
    /* 0x38 */ s16 swing_flag;
    /* 0x3A */ s16 bound;
    /* 0x3C */ s16 swing;
} Sus; // size = 0x40

/* Matching flatten of Sus 0x1C+. Unused after sus1; kept so the type still exists. */
struct UnkPlayerInner {
    /* 0x00 */ s16 gachon_timer;
    /* 0x02 */ s16 broken_timer;
    /* 0x04 */ f32 swing_firstspeed;
    /* 0x08 */ f32 gachon_firstspeed;
    /* 0x0C */ f32 bound_firstspeed;
    /* 0x10 */ f32 broken_firstspeed;
    /* 0x14 */ f32 y_grv;
    /* 0x18 */ s16 bound_timer;
    /* 0x1A */ s16 swing_timer;
    /* 0x1C */ s16 swing_flag;
    /* 0x1E */ s16 bound;
    /* 0x20 */ s16 swing;
};

typedef struct {
    /* 0x0000 */ u16 flag;
    /* 0x0002 */ u16 status;
    /* 0x0004 */ s16 rank;
    /* 0x0006 */ u16 point;
    /* 0x0008 */ s16 rap;
    /* 0x000A */ char hoot_flag[0x2];
    /* 0x000C */ s32 weapon;
    /* 0x0010 */ s16 item;
    /* 0x0012 */ s16 hamp_flag;
    /* 0x0014 */ Vec3f position;
    /* 0x0020 */ Vec3f old_position;
    /* 0x002C */ Vec3s direction;
    /* 0x0032 */ char litro_flag[0x2];
    /* 0x0034 */ Vec3f velocity;
    /* 0x0040 */ s16 link_doshin;
    /* 0x0042 */ s16 kage_ang;
    /* 0x0044 */ s16 handling_flag;
    /* 0x0046 */ u16 hit_flag;
    /* 0x0048 */ Vec4s angle_y;
    /* 0x0050 */ Vec4s angle_z;
    /* 0x0058 */ f32 up_vector[3];
    /* 0x0064 */ Vec3f gravity_xz;
    /* 0x0070 */ f32 radius;
    /* 0x0074 */ f32 ground;
    /* 0x0078 */ s16 sterrangle;
    /* 0x007A */ s16 shake_flag;
    /* 0x007C */ s32 nowsterr;
    /* 0x0080 */ f32 turbo_power;
    /* 0x0084 */ f32 f_grip;
    /* 0x0088 */ f32 r_grip;
    /* 0x008C */ f32 force;
    /* 0x0090 */ f32 grip;
    /* 0x0094 */ f32 speed;
    /* 0x0098 */ f32 taco;
    /* 0x009C */ f32 accelcount;
    /* 0x00A0 */ f32 power_down;
    /* 0x00A4 */ f32 power;
    /* 0x00A8 */ s16 slipcount;
    /* 0x00AA */ s16 sterr_count;
    /* 0x00AC */ s16 offsetsterr_flag;
    /* 0x00AE */ s16 old_direction;
    /* 0x00B0 */ s16 thunder_timer;
    /* 0x00B2 */ s16 spin_timer;
    /* 0x00B4 */ u16 offsetsterr_timer;
    /* 0x00B6 */ u16 talk;
    /* 0x00B8 */ f32 offsetsterr_speed;
    /* 0x00BC */ u32 slip_flag;
    /* 0x00C0 */ s16 slipang;
    /* 0x00C2 */ s16 jumpcount;
    /* 0x00C4 */ s16 kartpitch;
    /* 0x00C6 */ s16 erase;
    /* 0x00C8 */ s16 jugemu_timer;
    /* 0x00CA */ s16 jugemu_flag;
    /* 0x00CC */ Vec4s roll;
    /* 0x00D4 */ Vec4s pitch;
    /* 0x00DC */ s16 turbo_timer;
    /* 0x00DE */ u16 water_flag;
    /* 0x00E0 */ s16 bomb_timer;
    /* 0x00E2 */ s16 slipstream_timer;
    /* 0x00E4 */ f32 slipstream_power;
    /* 0x00E8 */ f32 maxadd_power;
    /* 0x00EC */ f32 jump;
    /* 0x00F0 */ f32 jmp_acc;
    /* 0x00F4 */ f32 jmp_speed;
    /* 0x00F8 */ u16 bump_status;
    /* 0x00FA */ s16 sterracc;
    /* 0x00FC */ f32 mass;
    /* 0x0100 */ f32 gravity;
    /* 0x0104 */ f32 power_cont;
    /* 0x0108 */ f32 shake;
    /* 0x010C */ s16 hitcount;
    /* 0x010E */ char unk_10E[0x2];
    /* 0x0110 */ Bump bump;
    /* 0x0150 */ Mat3 align2;
    /* 0x0174 */ Mat3 align;
    /* 0x0198 */ Tire tire_FL;
    /* 0x01B0 */ Tire tire_FR;
    /* 0x01C8 */ Tire tire_RL;
    /* 0x01E0 */ Tire tire_RR;
    /* 0x01F8 */ f32 mid_front;
    /* 0x01FC */ f32 mid_rear;
    /* 0x0200 */ u32 chasesp_sterr;
    /* 0x0204 */ s16 drift_count;
    /* 0x0206 */ s16 kartroll;
    /* 0x0208 */ f32 allrear_grip;
    /* 0x020C */ f32 brake_time;
    /* 0x0210 */ f32 max_power;
    /* 0x0214 */ f32 acc_maxcount;
    /* 0x0218 */ f32 spark_x;
    /* 0x021C */ f32 spark_z;
    /* 0x0220 */ s16 chasepoint;
    /* 0x0222 */ s16 hangflag;
    /* 0x0224 */ f32 offsetsize;
    /* 0x0228 */ s16 stopsetrr_count;
    /* 0x022A */ s16 powerstack_count;
    /* 0x022C */ f32 old_speed;
    /* 0x0230 */ f32 mid_left;
    /* 0x0234 */ s16 hit_sterr;
    /* 0x0236 */ s16 roliover_time;
    /* 0x0238 */ s16 broken_timer;
    /* 0x023A */ s16 drift_turbo_timer;
    /* 0x023C */ f32 mid_right;
    /* 0x0240 */ s32 tcol_count;
    /* 0x0244 */ u16 dir[4];
    /* 0x024C */ u16 deg[4];
    /* 0x0254 */ u16 kart;
    /* 0x0256 */ u16 wallhitcount;
    /* 0x0258 */ Smoke gass[10];
    /* 0x0528 */ Smoke smoke[10];
    /* 0x07F8 */ Smoke letter[10];
    /* 0x0AC8 */ Smoke spark[10];
    /* 0x0D98 */ Sus sus;
} Player; // size = 0xDD8

enum POOL_1_PARTICLE_TYPES {
    NO_PARTICLE,
    DRIFT_PARTICLE,
    GROUND_PARTICLE,
    GRASS_PARTICLE,
    POOL_1_PARTICLE_TYPE_4,
    POOL_1_PARTICLE_TYPE_5,
    POOL_1_PARTICLE_TYPE_6,
    POOL_1_PARTICLE_TYPE_7,
    POOL_1_PARTICLE_TYPE_8,
    POOL_1_PARTICLE_TYPE_9
};

typedef struct {
    // Something related to time trial ghost data?
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 ghostDataSaved;
    /* 0x05 */ s8 courseIndex;
    /* 0x06 */ u8 characterId;
    /* 0x07 */ u8 unk_07[0x3C];
    /* 0x43 */ u8 pad_43[0x7F - 0x43];
    /* 0x7F */ u8 checksum;
} struct_8018EE10_entry; // size = 0x80

typedef struct {
    /* 0x00 */ u16 red;
    /* 0x02 */ u16 green;
    /* 0x04 */ u16 blue;
    /* 0x06 */ u16 alpha;
} RGBA16; // size = 0x08

// Something related to human players
// Also might be used for the camera during the post race screens
typedef struct {
    /* 0x00 */ f32 is; // Looks to be some type of scaling, unknown use
    /* 0x04 */ f32 rs; // Scaling done on the rank text in the bottom left corner of the screen
                       // All time measurements are in centiseconds
    /* 0x08 */ u32 totaltime; // The someTimers seem to always have the same value, the total time since race start
    /* 0x0C */ u32 laptime;
    /* 0x10 */ u32 gettime; // Sum of time of all completed laps
    // Times at which each lap was completed
    union {
        struct {
            /* 0x14 */ u32 lap1CompletionTime;
            /* 0x18 */ u32 lap2CompletionTime;
            /* 0x1C */ u32 lap3CompletionTime;
        };
        u32 timebuf[3];
    };
    // Time each lap took to complete
    union {
        struct {
            /* 0x20 */ u32 lap1Duration;
            /* 0x24 */ u32 lap2Duration;
            /* 0x28 */ u32 lap3Duration;
        };
        u32 laptimebuf[3];
    };
    // Integer parts of the player's X/Y/X coordinates
    /* 0x2C */ s32 plx;
    /* 0x30 */ s32 ply;
    /* 0x34 */ s32 plz;
    /* 0x38 */ s32 flag; // Library uint; keep s32
    // Timer for how long the on screen should blink on lap completion
    // Blinking occurs as long as this is a non-zero value.
    // Other on screen effects may override the blinking
    /* 0x3C */ s16 flashtm;
    /* 0x3E */ s16 mx;
    /* 0x40 */ s16 my;
    /* 0x42 */ s16 ix;
    /* 0x44 */ s16 iy;
    /* 0x46 */ s16 addix;
    /* 0x48 */ s16 addiy;
    // These 2 s16's have values, but I have yet to identify any places that read them
    /* 0x4A */ s16 px;
    /* 0x4C */ s16 py;
    /* 0x4E */ s16 timerX; // X coordinate of the on screen timer. Library overlay uses tx[5]; this tree keeps the named s16s.
    // These 4 X coordinates are "slide" values
    union {
        struct {
            /* 0x50 */ s16 lap1CompletionTimeX; // Pulls double-duty as timerAfterImage1X
            /* 0x52 */ s16 lap2CompletionTimeX; // Pulls double-duty as timerAfterImage2X
            /* 0x54 */ s16 lap3CompletionTimeX;
        };
        /* 0x50 */ s16 lapCompletionTimeXs[3];
    };
    /* 0x56 */ s16 totalTimeX;
    /* 0x58 */ s16 ty; // Y coordinate of the on screen timer (used as Y coordinate for lap completion times in
                       // post-race screen)
    /* 0x5A */ s16 cx; // X coordinate of the on screen lap counter
                       // 0x5C and 0x5E seem to be relative to cx
    /* 0x5C */ s16 cx2;
    /* 0x5E */ s16 cx3;
    /* 0x60 */ s16 cy; // Y coordinate of the on screen lap counter
    /* 0x62 */ s16 rx; // X coordinate of the on screen rank indicator
    /* 0x64 */ s16 ry; // Y coordinate of the on screen rank indicator
    /* 0x66 */ s16
        rgx; // Adds with the other rx, used during post race screen to make the rank "slide" into place
    /* 0x68 */ s16
        rgy; // Adds with the other ry, used during post race screen to make the rank "slide" into place
    /* 0x6A */ s16 rank; // Position to take during race staging
    // These s16's occasionally have values, but I have yet to identify any places that read them
    // They appear to have values when in 3/4 player split screen mode, otherwise they're 0
    /* 0x6C */ s16 bombx;
    /* 0x6E */ s16 bomby;
    /* 0x70 */ s8 goalsw; // Indicates if race is over?
    /* 0x71 */ s8 goal;   // Increases to 3 when a race is over, while lapcnt stays at 2
    /* 0x72 */ s8 lapcnt;
    // Related to the timer blinking on lap completion
    // If flashtm is counting down:
    //     0: Show timer
    //     1: Hide timer
    //     2: ? Seems to be a transition state, never lasts a long enough for flashtm to tick
    /* 0x73 */ s8 dispsw;
    /* 0x74 */ s8 bomb;
    /* 0x75 */ s8 ghostcnt;
    /* 0x76 */ u8 item; // Something related to item generation. If non-zero, it determines the item you get
    /* 0x77 */ s8 sp_item; // Library uchar; keep s8
    // 0x78 to 0x7F appear to be some type of "state" trackers for the lap and timer text during a race start
    // When a race starts those texts (and their afterimages) slide in and "bounce" a bit. These states control the
    // bouncing (somehow)
    /* 0x78 */ u8 mptr;
    /* 0x79 */ u8 hptr;
    /* 0x7A */ u8 tptr;
    /* 0x7B */ u8 tptr2;
    /* 0x7C */ u8 tptr3;
    /* 0x7D */ u8 cptr;
    /* 0x7E */ u8 cptr2;
    /* 0x7F */ u8 cptr3;
    /* 0x80 */ u8 iptr;
    /* 0x81 */ u8 ranksw;
    /* 0x82 */ s8 unk_82;
    /* 0x83 */ s8 unk_83;
} Hud; // size = 0x84

typedef Hud KWLapStruct;

#define HUD_PLAYERS_SIZE 4

#endif
