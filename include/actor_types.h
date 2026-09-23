#ifndef ACTOR_TYPES_H
#define ACTOR_TYPES_H

#include <ultra64.h>
#include <macros.h>
#include <common_structs.h>

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wmicrosoft-extension"
// #pragma GCC diagnostic ignored "-Wmissing-member-name-in-structure-/-union"

/**
 * SimpleObjectArray should be understood to be populated by generic Actor structs.
 * However, for human readability, many functions interacting with actor list elements expect one of the many
 * specialized types found in this file.
 *
 * Note that specialized types must be the same Size as a plain Actor. Don't be mislead into thinking that
 * because its a separate Type that it can modified separately from plain Actor. If you modify/add an actor Type
 * and its Size is different from plain Actor's, you WILL run into buggy (potentially crash inducing) behaviour.
 *
 * Specialized structs are customizable so long as the following member specifications are met:
 *
 * In general:
 *     0x00 -> s16 Type
 *     0x02 -> s16 flags
 *     0x30 -> Collision unk30
 *
 * If Player can collide with the actor:
 *     0x0C -> f32 boundingBoxSize
 *
 * If the actor makes Sound (necessary for doppler/Volume stuff):
 *     0x18 -> Vec3f pos
 *     0x24 -> Vec3f Velocity
 *
 * Other members are more flexible, and even the non-general specifications can be ignored IF AND ONLY IF you know
 * exactly what you're doing.
 */
enum ActorType {
    ACTOR_UNKNOWN_0x01 = 0x01,
    ACTOR_TREE_MARIO_RACEWAY,
    ACTOR_TREE_YOSHI_VALLEY,
    ACTOR_TREE_ROYAL_RACEWAY,
    ACTOR_FALLING_ROCK,
    ACTOR_BANANA,
    ACTOR_GREEN_SHELL,
    ACTOR_RED_SHELL,
    ACTOR_YOSHI_EGG,
    ACTOR_PIRANHA_PLANT,
    ACTOR_UNKNOWN_0x0B,
    ACTOR_ITEM_BOX,
    ACTOR_FAKE_ITEM_BOX,
    ACTOR_BANANA_BUNCH,
    ACTOR_TRAIN_ENGINE,
    ACTOR_TRAIN_TENDER,
    ACTOR_TRAIN_PASSENGER_CAR,
    ACTOR_COW,
    ACTOR_TREE_MOO_MOO_FARM,
    ACTOR_UNKNOWN_0x14,
    ACTOR_TRIPLE_GREEN_SHELL,
    ACTOR_TRIPLE_RED_SHELL,
    ACTOR_MARIO_SIGN,
    ACTOR_UNKNOWN_0x18,
    ACTOR_PALM_TREE,
    ACTOR_UNKNOWN_0x1A,
    ACTOR_UNKNOWN_0x1B,
    ACTOR_TREE_BOWSERS_CASTLE,
    ACTOR_TREE_FRAPPE_SNOWLAND,
    ACTOR_CACTUS1_KALAMARI_DESERT,
    ACTOR_CACTUS2_KALAMARI_DESERT,
    ACTOR_CACTUS3_KALAMARI_DESERT,
    ACTOR_BUSH_BOWSERS_CASTLE,
    ACTOR_UNKNOWN_0x21,
    ACTOR_WARIO_SIGN,
    ACTOR_UNKNOWN_0x23,
    ACTOR_BOX_TRUCK,
    ACTOR_PADDLE_BOAT,
    ACTOR_RAILROAD_CROSSING,
    ACTOR_SCHOOL_BUS,
    ACTOR_TANKER_TRUCK,
    ACTOR_BLUE_SPINY_SHELL,
    ACTOR_HOT_AIR_BALLOON_ITEM_BOX,
    ACTOR_CAR,
    ACTOR_KIWANO_FRUIT
};

#define ACTOR_LIST_SIZE 100

// Object flags (Library EXISTOBJ family). ACTOR_IS_NOT_EXPIRED is ONSCREEN1|2|3|4.
#define EXISTOBJ 0x8000
#define HITOBJ 0x4000
#define MOVEOBJ 0x2000
#define MYOBJ 0x1000
#define HIDEOBJ 0x800
#define FLYINGOBJ 0x400
#define SEBANANAOBJ 0x200
#define SEKAMEHITOBJ 0x100
#define SEKAMEOUTOBJ 0x80
#define ONSCREEN1 0x1
#define ONSCREEN2 0x2
#define ONSCREEN3 0x4
#define ONSCREEN4 0x8
#define ACTOR_IS_NOT_EXPIRED 0xF // The actor possesses some Kind of collision and can be removed

// Actor shell->State (Green, red and Blue)
enum ShellState {
    HELD_SHELL,                  // Single shell that has not been dropped. (probably holding Z).
    RELEASED_SHELL,              // This is the short window where single shells aren't being held or launched.
    MOVING_SHELL,                // Moving towards its Target after being shot.
    RED_SHELL_LOCK_ON,           // Red shell is targeting.
    TRIPLE_GREEN_SHELL,          // Loses triple shell State when shot.
    GREEN_SHELL_HIT_A_RACER,     // A racer has been hit by a Green shell.
    TRIPLE_RED_SHELL,            // Loses triple shell State when shot.
    DESTROYED_SHELL,             // Collision with the shell.
    BLUE_SHELL_LOCK_ON,          // A Blue shell has found a Target and is hastily approaching it.
    BLUE_SHELL_TARGET_ELIMINATED // Mission completed, well done boss.
};

// Actor banana->State
enum BananaState {
    HELD_BANANA,               // Single banana that has not been dropped.
    DROPPED_BANANA,            // A banana in the State of being dropped on the Ground (it only last for a few frames).
    FIRST_BANANA_BUNCH_BANANA, // The first banana of the banana bunch
    BANANA_BUNCH_BANANA,       // Every banana of the banana bunch except the first one.
    BANANA_ON_GROUND,          // A banana sitting on the Ground.
    DESTROYED_BANANA           // Collision with the banana.
};

// Actor fakeItemBox->State
#define HELD_FAKE_ITEM_BOX 0      // Item box is being held be Z.
#define FAKE_ITEM_BOX_ON_GROUND 1 // Item box is on the Ground.
#define DESTROYED_FAKE_ITEM_BOX 2 // Collision with fake Item box.

typedef struct Object {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 Counter;
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 Radius;
    /* 0x10 */ Vec3s Angle;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
} Object; // Size = 0x70
typedef Object Actor;

// Duplicate declare for simplicity when externing actors & packed files.
extern Object SimpleObjectArray[ACTOR_LIST_SIZE]; // D_8015F9B8

/*
Specialized actor types
*/

/*
Used by the Locomotive, Tender, and passenger car
*/
struct TrainCar {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 Counter;
    /* 0x06 */ s16 WheelRot;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ Vec3s Angle;
    /* 0x10 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct RailroadCrossing {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 SomeTimer;
    /* 0x06 */ s16 CrossingId;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ Vec3s Angle;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct FallingRock {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 RespawnTimer;
    /* 0x06 */ s16 Unk06;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 Radius;
    /* 0x10 */ Vec3s Angle;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct ActorSpawnData {
    /* 0x00 */ Vec3s pos;
    union {
        /* 0x06 */ u16 SomeId; // Usually populated, but not necessarily Used by all actors types
        /* 0x06 */ s16 SignedSomeId;
    };
};

// Required for CheckColJungleTree due to diff Size.
// members unverified. data located at d_course_dks_jungle_parkway_tree_spawn
/**
 * There are nearly 100 trees in DK Jungle Parkway. If they were put into the actor list proper
 * they would Fill it up, leaving no space for stuff like Item boxes, shells, bananas, kiwano fruits,
 * etc.
 * So, this struct Type acts as both spawn data AND a stripped down Actor for those trees.
 * Give the tree a Position, a byte for flags stuffed into an s16 Used to indicate tree sub-Type,
 * and an s16 containing as the tree's original Y Position.
 **/
struct UnkActorSpawnData {
    /* 0x00 */ Vec3s pos;
    // Techinically only the bottom byte of SomeId is the "id". The top byte is Used for flags.
    /* 0x06 */ s16 SomeId;
    // Stores the tree's original Y Position.
    /* 0x08 */ s16 Unk8;
};

struct YoshiValleyEgg {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 Counter;
    /* 0x06 */ s16 Unk06;
    /* 0x08 */ f32 PathRadius;
    /* 0x0C */ f32 Radius;
    /* 0x10 */ s16 PathRot;
    /* 0x12 */ s16 EggRot;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    // Note, PathCenter[1] should be understood to be the Y Velocity of the egg
    // PathCenter[0] and PathCenter[2] are the X,Z coordinates of the center of the path
    /* 0x24 */ Vec3f PathCenter;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct KiwanoFruit {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16
        TargetPlayer; // Id of the Player this actor tracks. Each Player has their own kiwano actor just for them
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 BonkTimer; // bonkState? Not sure what this is tracking, but its some form of Count down that starts
                              // after the fruit hits you
    /* 0x0C */ f32 Radius;
    /* 0x10 */ s16 AnimState;
    /* 0x12 */ s16 AnimTimer;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct PaddleWheelBoat {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 Counter;
    /* 0x06 */ s16 WheelRot;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ Vec3s BoatRot;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct PakkunObject {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ Vec4s VisibilityStates; // A per-Camera visibilty State tracker
    /* 0x0C */ f32 Radius;
    /* 0x10 */ Vec4s unk10;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec4s Timers; // A per-Camera Timer. Might be more appropriate to call this State
    /* 0x2C */ f32 Unk02C;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70
typedef struct PakkunObject PakkunObject;
typedef PakkunObject PiranhaPlant;

struct PalmTree {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 Variant; // four different types of palm trees
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 Radius;
    /* 0x10 */ Vec3s Angle;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

typedef struct {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 ShellsAvailable;
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ s16 RotVelocity;
    /* 0x12 */ s16 RotAngle;
    /* 0x14 */ s16 PlayerId; // Id of the Player that "owns" the shells
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f unk_18;
    /* 0x24 */ Vec3f ShellIndices; // Indices in SimpleObjectArray for the shells "owned" by this parent
    /* 0x30 */ Bump Bump;
} TripleShellParent; // Size = 0x70

struct ShellActor {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    // Index in SimpleObjectArray for the parent actor of this shell
    // Seems to pull double duty as a Timer
    union {
        /* 0x04 */ s16 ParentIndex;
        /* 0x04 */ s16 SomeTimer;
        // Red Shells only (maybe Blue shells?)
        /* 0x04 */ s16 TargetPlayer; // Player the shell is after
    };
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 shellId; // 0, 1, or 2. Indicates which shell in the triplet this one is
    /* 0x0C */ f32 Radius;
    /* 0x10 */ s16 RotVelocity; // Change in RotAngle on a per-update basis
    union {
        /* 0x12 */ s16 RotAngle;  // Angle of Rotation around Player (or parent?), not the Rotation of the shell itself
        /* 0x12 */ u16 PathIndex; // Index in the set of points that make up the "path" the red/Blue shell follows (may
                                  // be GP Mode exclusive)
    };
    /* 0x14 */ s16 PlayerId; // Id of the Player that "owns" the shell
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity; // All 0 until the shell is fired
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct ItemBox {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 SomeTimer;
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 ResetDistance; // Value added to the Y Position when box is touched. Expected to be negative
                                  // Distance at which a Player can activate the Item box
                                  // Named "bounding box" to match the name Used for the "Size" of a Kart
    /* 0x0C */ f32 Radius;
    /* 0x10 */ Vec3s Angle;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ f32 OrigY; // Original Y Position. Basically the Y Position the box will reset to after being touched
    /* 0x28 */ f32 Unk028;
    /* 0x2C */ f32 Unk02C;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct FakeItemBox {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 SomeTimer;
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 SizeScaling; // Controls the Size of the box
    /* 0x0C */ f32 Radius;
    /* 0x10 */ Vec3s Angle;
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ f32 PlayerId;
    /* 0x28 */ f32 TargetY;
    /* 0x2C */ f32 Unk02C;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct BananaBunchParent {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 Counter;
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ f32 Fparam;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ s16 PlayerId;         // Player that own the bananas
    /* 0x12 */ s16 BananaIndices[5]; // Indices in SimpleObjectArray for the bananas owned by this parent
    /* 0x1C */ s16 BananasAvailable;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ f32 unk_20[4];
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

struct BananaActor {
    /* 0x00 */ s16 Category;
    /* 0x02 */ s16 Flag;
    /* 0x04 */ s16 Counter;
    /* 0x06 */ s16 Sparam;
    /* 0x08 */ s16 ParentIndex;
    /* 0x0A */ s16 bananaId; // ? Appears to indiciate which banana of the bunch this one is
    /* 0x0C */ f32 Radius;
    union {
        /* 0x10 */ Vec3s Angle;
        struct {
            /* 0x10 */ s16 PlayerId;     // Id of the Player that owns this banana
            /* 0x12 */ s16 ElderIndex;   // Index in SimpleObjectArray of the Next-oldest banana in the bunch
            /* 0x14 */ s16 YoungerIndex; // Index in SimpleObjectArray of the Next-youngest banana in the bunch
        };
    };
    /* 0x16 */ s16 Unk16;
    /* 0x18 */ Vec3f Position;
    /* 0x24 */ Vec3f Velocity;
    /* 0x30 */ Bump Bump;
}; // Size = 0x70

// #pragma GCC diagnostic pop

#endif // ACTOR_TYPES_H
