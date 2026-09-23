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

/* Library / Kimura aliases (Struct.h). Do not Mass-replace Vec3f in the tree. */
typedef u8 Uchar;
typedef u16 Ushort;
typedef u32 Uint;
typedef Vec3f Vector;
typedef Vec3s SVector;
typedef Vec3su USVector;
typedef Mat3 Matrix;
typedef Mat3 RotateMtx;
typedef Mat4 AffineMtx;
typedef Vp Viewport;

// might not be real, Used by CheckWall
typedef struct {
    f32 x, y, z;
} Vec3fs;

// This was added as a silly idea:
// In the data to use "A, B, Z, R" instead of Hex numbers.
typedef enum { A = 0x80, B = 0x40, Z = 0x20, R = 0x10 } GhostController;

/***  types.h  ***/

typedef struct {
    /* 0x0 */ s16 Start;
    /* 0x2 */ s16 end;
    /* 0x4 */ s32 Action;
} ActionData; // Size = 0x8
typedef ActionData CPUBehaviour;

enum SpTaskState {
    SPTASK_STATE_NOT_STARTED,
    SPTASK_STATE_RUNNING,
    SPTASK_STATE_INTERRUPTED,
    SPTASK_STATE_FINISHED,
    SPTASK_STATE_FINISHED_DP
};

struct SPTask {
    /*0x00*/ OSTask Task;
    /*0x40*/ OSMesgQueue* Msgqueue;
    /*0x44*/ OSMesg msg;
    /*0x48*/ enum SpTaskState State;
}; // Size = 0x4C, align = 0x8

struct VblankHandler {
    OSMesgQueue* Queue;
    OSMesg msg;
};

struct D_80150158 {
    s16 Unk0;
    s32 unk4;
    s32 Unk8;
    s32 UnkC;
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
}; // Size = 0x10
typedef struct Controller Controller;

// Camera path struct? Or something like that. For GP race won scene?
struct UnkStruct_80287500 {
    Vec3f Unk0; // xyz coordinates?
    f32 UnkC;
    f32 unk10;
    f32 unk14; // Rotation?
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
    u32 Unk0;
    u32 unk4;
    u32 Unk8;
    u32 UnkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u16 unk24;
    u16 Unk26;
};

struct UnkStruct_802B53C8 {
    f32 Unk0;
    f32 unk4;
    f32 Unk8;
    f32 UnkC;
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
    u8 Button;
    s8 FrameDuration;
    s8 StickY;
    s8 StickX;
} StaffGhost;
// 80160ADC Banana's remaining

typedef struct Bump {
    /* 0x00 */ u16 FlagXY;
    /* 0x02 */ u16 FlagYZ;
    /* 0x04 */ u16 FlagZX;
    /* 0x06 */ u16 LastXY;
    /* 0x08 */ u16 LastYZ;
    /* 0x0A */ u16 LastZX;
    /* 0x0C */ f32 DistanceXY;
    /* 0x10 */ f32 DistanceYZ;
    /* 0x14 */ f32 DistanceZX;
    /* 0x18 */ Vec3f BumpXY;
    /* 0x24 */ Vec3f BumpYZ;
    /* 0x30 */ Vec3f BumpZX;
    /* 0x3C */ f32 Dummy;
} Bump; // Size = 0x40
typedef Bump Collision;

typedef struct Smoke {
    /* 0x00 */ f32 trx;
    /* 0x04 */ f32 try;
    /* 0x08 */ f32 trz;
    /* 0x0C */ f32 Scale;
    /* 0x10 */ u16 Lrflag;
    /* 0x12 */ u16 Number; /* stock Particle.Type: Kind enum. Never confuse with .Type below. */
    /* 0x14 */ f32 Type;   /* stock Particle.Status. A second apply of Type→Number will smash this. */
    /* 0x18 */ f32 dev;
    /* 0x1C */ s16 Flag;
    /* 0x1E */ s16 Timer;
    /* 0x20 */ s16 ang;
    /* 0x22 */ s16 Unk022;
    /* 0x24 */ f32 Fwork1;
    /* 0x28 */ f32 Fwork2;
    /* 0x2C */ s16 Unk02C;
    /* 0x2E */ s16 Unk02E;
    /* 0x30 */ s16 Unk030;
    /* 0x32 */ s16 Unk032;
    /* 0x34 */ s16 Unk034;
    /* 0x36 */ s16 Unk036;
    /* 0x38 */ s16 Swork1;
    /* 0x3A */ s16 Swork2;
    /* 0x3C */ s16 Swork3;
    /* 0x3E */ s16 Swork4;
    /* 0x40 */ s16 Swork5;
    /* 0x42 */ s16 Unk042;
    /* 0x44 */ s16 Unk044;
    /* 0x46 */ s16 Unk046;
} Smoke; // Size = 0x48
typedef Smoke Particle;

// This struct is almost identical to the GBI Vtx_t Type,
// except that its missing the "Flag" member.
typedef struct {
    s16 ob[3]; /* x, y, z */
    s16 tc[2]; /* Texture coord */
    s8 ca[4];  /* color & Alpha */
} CourseVtx;

/*
This struct has been copied (with only minor modifications) from
https://github.com/micro500/mariokart64/wiki/Surface-Map
on January 23rd, 2022
The original author is assumed to be RenaKunisaki
*/
typedef struct {
    /* 0x00 */ u16 Flag;
    // Top bytes is a collections of flags, bottom byte is of Unknown purpose
    // bit 7: 1 = only tangible if landed on, not if driven onto?
    //       very weird. game crashes sometimes when playing with this.
    // bit 6: 1 = Lakitu can drop you here (XXX verify)
    // bit 4: 1 = out of bounds
    // bit 3: 1 = Player tumbles upon contact (may fall Right through)
    /* 0x02 */ u16 Status;

    // For AABB bounding-box style collision. Box style collision is cheaper than checking each vtx.
    /* 0x04 */ s16 Minx;  // Minimum x coordinate
    s16 Miny;             // Minimum y coordinate
    s16 Minz;             // Minimum z coordinate
    s16 Maxx;             // Maximum x coordinate
    s16 Maxy;             // Maximum y coordinate
    /* 0x0A */ s16 Maxz;  // Maximum z coordinate
    /* 0x10 */ Vtx* p1; // Pointer to the 3 vertices of this poly
    Vtx* p2;
    Vtx* p3;
    // Face normal. Should really be Vec3f normal
    /* 0x1C */ f32 Sola;
    /* 0x20 */ f32 Solb;
    /* 0x24 */ f32 Solc;
    /* 0x28 */ f32 Sold;
} FaceStruct; // Size = 0x2C
typedef FaceStruct CollisionTriangle;

typedef struct Tire {
    /* 0x00 */ Vec3f Position;
    /* 0x0C */ u8 Status;
    /* 0x0D */ u8 LastAxis;
    /* 0x0E */ u16 LastPointer;
    /* 0x10 */ f32 Height;
    /* 0x14 */ s32 Dummy;
} Tire; // Size = 0x18
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
    Ushort Offroad : 1;
    Ushort bot : 1;
    Ushort SuperRival : 1;
    Ushort NoTireDMA : 1;
    Ushort MegaRival : 1;
    Ushort Autopilot : 1;
    Ushort r5 : 1;
    Ushort r6 : 1;
    Ushort r7 : 1;
    Ushort r8 : 1;
    Ushort r9 : 1;
    Ushort RealKart : 5;
} USbit;
// clang-format on

typedef struct Sus {
    /* 0x00 */ s16 Flag;
    /* 0x02 */ s16 Flag1;
    /* 0x04 */ f32 Position;
    /* 0x08 */ f32 Accele;
    /* 0x0C */ s16 Broken;
    /* 0x0E */ s16 JumpTimer;
    /* 0x10 */ f32 JumpL;
    /* 0x14 */ f32 JumpFirstspeed;
    /* 0x18 */ f32 JmpAccL;
    /* 0x1C */ s16 GachonTimer;
    /* 0x1E */ s16 BrokenTimer;
    /* 0x20 */ f32 SwingFirstspeed;
    /* 0x24 */ f32 GachonFirstspeed;
    /* 0x28 */ f32 BoundFirstspeed;
    /* 0x2C */ f32 BrokenFirstspeed;
    /* 0x30 */ f32 YGrv;
    /* 0x34 */ s16 BoundTimer;
    /* 0x36 */ s16 SwingTimer;
    /* 0x38 */ s16 SwingFlag;
    /* 0x3A */ s16 Bound;
    /* 0x3C */ s16 Swing;
} Sus; // Size = 0x40

/* Matching flatten of Sus 0x1C+. Unused after sus1; kept so the Type still exists. */
struct UnkPlayerInner {
    /* 0x00 */ s16 GachonTimer;
    /* 0x02 */ s16 BrokenTimer;
    /* 0x04 */ f32 SwingFirstspeed;
    /* 0x08 */ f32 GachonFirstspeed;
    /* 0x0C */ f32 BoundFirstspeed;
    /* 0x10 */ f32 BrokenFirstspeed;
    /* 0x14 */ f32 YGrv;
    /* 0x18 */ s16 BoundTimer;
    /* 0x1A */ s16 SwingTimer;
    /* 0x1C */ s16 SwingFlag;
    /* 0x1E */ s16 Bound;
    /* 0x20 */ s16 Swing;
};

typedef struct {
    /* 0x0000 */ u16 Flag;
    /* 0x0002 */ u16 Status;
    /* 0x0004 */ s16 Rank;
    /* 0x0006 */ u16 Point;
    /* 0x0008 */ s16 rap;
    /* 0x000A */ char HootFlag[0x2];
    /* 0x000C */ s32 Weapon;
    /* 0x0010 */ s16 Item;
    /* 0x0012 */ s16 HampFlag;
    /* 0x0014 */ Vec3f Position;
    /* 0x0020 */ Vec3f OldPosition;
    /* 0x002C */ Vec3s Direction;
    /* 0x0032 */ char LitroFlag[0x2];
    /* 0x0034 */ Vec3f Velocity;
    /* 0x0040 */ s16 LinkDoshin;
    /* 0x0042 */ s16 KageAng;
    /* 0x0044 */ s16 HandlingFlag;
    /* 0x0046 */ u16 HitFlag;
    /* 0x0048 */ Vec4s AngleY;
    /* 0x0050 */ Vec4s AngleZ;
    /* 0x0058 */ f32 UpVector[3];
    /* 0x0064 */ Vec3f GravityXz;
    /* 0x0070 */ f32 Radius;
    /* 0x0074 */ f32 Ground;
    /* 0x0078 */ s16 Sterrangle;
    /* 0x007A */ s16 ShakeFlag;
    /* 0x007C */ s32 Nowsterr;
    /* 0x0080 */ f32 TurboPower;
    /* 0x0084 */ f32 FGrip;
    /* 0x0088 */ f32 RGrip;
    /* 0x008C */ f32 Force;
    /* 0x0090 */ f32 Grip;
    /* 0x0094 */ f32 Speed;
    /* 0x0098 */ f32 Taco;
    /* 0x009C */ f32 Accelcount;
    /* 0x00A0 */ f32 PowerDown;
    /* 0x00A4 */ f32 Power;
    /* 0x00A8 */ s16 Slipcount;
    /* 0x00AA */ s16 SterrCount;
    /* 0x00AC */ s16 OffsetsterrFlag;
    /* 0x00AE */ s16 OldDirection;
    /* 0x00B0 */ s16 ThunderTimer;
    /* 0x00B2 */ s16 SpinTimer;
    /* 0x00B4 */ u16 OffsetsterrTimer;
    /* 0x00B6 */ u16 Talk;
    /* 0x00B8 */ f32 OffsetsterrSpeed;
    /* 0x00BC */ u32 SlipFlag;
    /* 0x00C0 */ s16 Slipang;
    /* 0x00C2 */ s16 Jumpcount;
    /* 0x00C4 */ s16 Kartpitch;
    /* 0x00C6 */ s16 Erase;
    /* 0x00C8 */ s16 JugemuTimer;
    /* 0x00CA */ s16 JugemuFlag;
    /* 0x00CC */ Vec4s Roll;
    /* 0x00D4 */ Vec4s Pitch;
    /* 0x00DC */ s16 TurboTimer;
    /* 0x00DE */ u16 WaterFlag;
    /* 0x00E0 */ s16 BombTimer;
    /* 0x00E2 */ s16 SlipstreamTimer;
    /* 0x00E4 */ f32 SlipstreamPower;
    /* 0x00E8 */ f32 MaxaddPower;
    /* 0x00EC */ f32 Jump;
    /* 0x00F0 */ f32 JmpAcc;
    /* 0x00F4 */ f32 JmpSpeed;
    /* 0x00F8 */ u16 BumpStatus;
    /* 0x00FA */ s16 Sterracc;
    /* 0x00FC */ f32 Mass;
    /* 0x0100 */ f32 Gravity;
    /* 0x0104 */ f32 PowerCont;
    /* 0x0108 */ f32 Shake;
    /* 0x010C */ s16 Hitcount;
    /* 0x010E */ char Unk10E[0x2];
    /* 0x0110 */ Bump Bump;
    /* 0x0150 */ Mat3 Align2;
    /* 0x0174 */ Mat3 align;
    /* 0x0198 */ Tire TireFL;
    /* 0x01B0 */ Tire TireFR;
    /* 0x01C8 */ Tire TireRL;
    /* 0x01E0 */ Tire TireRR;
    /* 0x01F8 */ f32 MidFront;
    /* 0x01FC */ f32 MidRear;
    /* 0x0200 */ u32 ChasespSterr;
    /* 0x0204 */ s16 DriftCount;
    /* 0x0206 */ s16 Kartroll;
    /* 0x0208 */ f32 AllrearGrip;
    /* 0x020C */ f32 BrakeTime;
    /* 0x0210 */ f32 MaxPower;
    /* 0x0214 */ f32 AccMaxcount;
    /* 0x0218 */ f32 SparkX;
    /* 0x021C */ f32 SparkZ;
    /* 0x0220 */ s16 Chasepoint;
    /* 0x0222 */ s16 Hangflag;
    /* 0x0224 */ f32 Offsetsize;
    /* 0x0228 */ s16 StopsetrrCount;
    /* 0x022A */ s16 PowerstackCount;
    /* 0x022C */ f32 OldSpeed;
    /* 0x0230 */ f32 MidLeft;
    /* 0x0234 */ s16 HitSterr;
    /* 0x0236 */ s16 RolioverTime;
    /* 0x0238 */ s16 BrokenTimer;
    /* 0x023A */ s16 DriftTurboTimer;
    /* 0x023C */ f32 MidRight;
    /* 0x0240 */ s32 TcolCount;
    /* 0x0244 */ u16 dir[4];
    /* 0x024C */ u16 deg[4];
    /* 0x0254 */ u16 Kart;
    /* 0x0256 */ u16 Wallhitcount;
    /* 0x0258 */ Smoke Gass[10];
    /* 0x0528 */ Smoke smoke[10];
    /* 0x07F8 */ Smoke Letter[10];
    /* 0x0AC8 */ Smoke Spark[10];
    /* 0x0D98 */ Sus sus;
} Player; // Size = 0xDD8

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
    // Something related to Time trial ghost data?
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8 GhostDataSaved;
    /* 0x05 */ s8 CourseIndex;
    /* 0x06 */ u8 CharacterId;
    /* 0x07 */ u8 Unk07[0x3C];
    /* 0x43 */ u8 Pad43[0x7F - 0x43];
    /* 0x7F */ u8 Checksum;
} struct_8018EE10_entry; // Size = 0x80

typedef struct {
    /* 0x00 */ u16 red;
    /* 0x02 */ u16 Green;
    /* 0x04 */ u16 Blue;
    /* 0x06 */ u16 Alpha;
} RGBA16; // Size = 0x08

// Something related to human players
// Also might be Used for the Camera during the post race screens
typedef struct {
    /* 0x00 */ f32 is; // Looks to be some Type of scaling, Unknown use
    /* 0x04 */ f32 rs; // Scaling done on the Rank text in the bottom Left corner of the screen
                       // All Time measurements are in centiseconds
    /* 0x08 */ u32 Totaltime; // The someTimers seem to always have the same Value, the total Time since race Start
    /* 0x0C */ u32 Laptime;
    /* 0x10 */ u32 Gettime; // Sum of Time of all completed laps
    // Times at which each lap was completed
    union {
        struct {
            /* 0x14 */ u32 Lap1CompletionTime;
            /* 0x18 */ u32 Lap2CompletionTime;
            /* 0x1C */ u32 Lap3CompletionTime;
        };
        u32 Timebuf[3];
    };
    // Time each lap took to complete
    union {
        struct {
            /* 0x20 */ u32 Lap1Duration;
            /* 0x24 */ u32 Lap2Duration;
            /* 0x28 */ u32 Lap3Duration;
        };
        u32 Laptimebuf[3];
    };
    // Integer parts of the Player's X/Y/X coordinates
    /* 0x2C */ s32 plx;
    /* 0x30 */ s32 ply;
    /* 0x34 */ s32 plz;
    /* 0x38 */ s32 Flag; // Library Uint; keep s32
    // Timer for how long the on screen should blink on lap completion
    // Blinking occurs as long as this is a non-zero Value.
    // Other on screen effects may override the blinking
    /* 0x3C */ s16 Flashtm;
    /* 0x3E */ s16 mx;
    /* 0x40 */ s16 my;
    /* 0x42 */ s16 ix;
    /* 0x44 */ s16 iy;
    /* 0x46 */ s16 Addix;
    /* 0x48 */ s16 Addiy;
    // These 2 s16's have values, but I have yet to identify any places that read them
    /* 0x4A */ s16 px;
    /* 0x4C */ s16 py;
    /* 0x4E */ s16 timerX; // X coordinate of the on screen Timer. Library overlay uses tx[5]; this tree keeps the named s16s.
    // These 4 X coordinates are "slide" values
    union {
        struct {
            /* 0x50 */ s16 Lap1CompletionTimeX; // Pulls double-duty as timerAfterImage1X
            /* 0x52 */ s16 Lap2CompletionTimeX; // Pulls double-duty as timerAfterImage2X
            /* 0x54 */ s16 Lap3CompletionTimeX;
        };
        /* 0x50 */ s16 lapCompletionTimeXs[3];
    };
    /* 0x56 */ s16 totalTimeX;
    /* 0x58 */ s16 ty; // Y coordinate of the on screen Timer (Used as Y coordinate for lap completion times in
                       // post-race screen)
    /* 0x5A */ s16 cx; // X coordinate of the on screen lap Counter
                       // 0x5C and 0x5E seem to be relative to cx
    /* 0x5C */ s16 cx2;
    /* 0x5E */ s16 cx3;
    /* 0x60 */ s16 cy; // Y coordinate of the on screen lap Counter
    /* 0x62 */ s16 rx; // X coordinate of the on screen Rank indicator
    /* 0x64 */ s16 ry; // Y coordinate of the on screen Rank indicator
    /* 0x66 */ s16
        rgx; // Adds with the other rx, Used during post race screen to make the Rank "slide" into place
    /* 0x68 */ s16
        rgy; // Adds with the other ry, Used during post race screen to make the Rank "slide" into place
    /* 0x6A */ s16 Rank; // Position to take during race staging
    // These s16's occasionally have values, but I have yet to identify any places that read them
    // They appear to have values when in 3/4 Player split screen Mode, otherwise they're 0
    /* 0x6C */ s16 Bombx;
    /* 0x6E */ s16 Bomby;
    /* 0x70 */ s8 Goalsw; // Indicates if race is over?
    /* 0x71 */ s8 Goal;   // Increases to 3 when a race is over, while Lapcnt stays at 2
    /* 0x72 */ s8 Lapcnt;
    // Related to the Timer blinking on lap completion
    // If Flashtm is counting down:
    //     0: Show Timer
    //     1: Hide Timer
    //     2: ? Seems to be a transition State, never lasts a long enough for Flashtm to tick
    /* 0x73 */ s8 Dispsw;
    /* 0x74 */ s8 Bomb;
    /* 0x75 */ s8 Ghostcnt;
    /* 0x76 */ u8 Item; // Something related to Item generation. If non-zero, it determines the Item you get
    /* 0x77 */ s8 SPItem; // Library Uchar; keep s8
    // 0x78 to 0x7F appear to be some Type of "State" trackers for the lap and Timer text during a race Start
    // When a race starts those texts (and their afterimages) slide in and "bounce" a bit. These states control the
    // bouncing (somehow)
    /* 0x78 */ u8 Mptr;
    /* 0x79 */ u8 Hptr;
    /* 0x7A */ u8 Tptr;
    /* 0x7B */ u8 Tptr2;
    /* 0x7C */ u8 Tptr3;
    /* 0x7D */ u8 Cptr;
    /* 0x7E */ u8 Cptr2;
    /* 0x7F */ u8 Cptr3;
    /* 0x80 */ u8 Iptr;
    /* 0x81 */ u8 Ranksw;
    /* 0x82 */ s8 unk_82;
    /* 0x83 */ s8 unk_83;
} Hud; // Size = 0x84

typedef Hud KWLapStruct;

#define HUD_PLAYERS_SIZE 4

#endif
