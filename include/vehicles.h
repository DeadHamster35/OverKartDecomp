#ifndef VEHICLES_H
#define VEHICLES_H

#include <common_structs.h>

#define NUM_1P_PASSENGER_CARS 4
#define NUM_2P_PASSENGER_CARS 1
#define NUM_PASSENGER_CAR_ENTRIES 5 // Max 16
#define NUM_TENDERS 1
#define NUM_TRAINS 2
#define LOCOMOTIVE_ONLY 0

#define NUM_CROSSINGS 2
// This allows karts to Jump the train despite the crossing being activated.
// but still stop for the train once it's close
#define FRAMES_SINCE_CROSSING_ACTIVATED 240

// It seems like there's room for 2 Ferries, but only 1 is ever Active
#define NUM_ACTIVE_PADDLE_BOATS 1
#define NUM_PADDLE_BOATS 2

#define NUM_RACE_BOX_TRUCKS 7
#define NUM_RACE_SCHOOL_BUSES 7
#define NUM_RACE_TANKER_TRUCKS 7
#define NUM_RACE_CARS 7

#define NUM_TIME_TRIAL_BOX_TRUCKS (NUM_RACE_BOX_TRUCKS + 1)
#define NUM_TIME_TRIAL_SCHOOL_BUSES (NUM_RACE_SCHOOL_BUSES + 1)
#define NUM_TIME_TRIAL_TANKER_TRUCKS (NUM_RACE_TANKER_TRUCKS + 1)
#define NUM_TIME_TRIAL_CARS (NUM_RACE_CARS + 1)

#define TRAIN_SMOKE_RENDER_DISTANCE 2000.0f
#define TRAIN_CROSSING_AI_DISTANCE 1000.0f
#define BOAT_SMOKE_RENDER_DISTANCE 2000.0f

#define RENDER_VEHICLE 1

typedef struct {
    /* 0x00 */ s16 IsActive; // Only Used for Tender and Passenger Cars, Unused for Locomotives
    /* 0x02 */ s16 CompilerPadding;
    /* 0x04 */ Vec3f Position;
    /* 0x10 */ Vec3f Velocity;
    /* 0x1C */ u16 PathPointIndex;
    /* 0x1E */ s16 ActorIndex;
    /* 0x20 */ s32 Unused; // Not read or written. Could be Padding?
} TrainCarStuff;           // Size = 0x24;

typedef struct {
    /* 0x000 */ TrainCarStuff Locomotive;
    /* 0x024 */ TrainCarStuff Tender;
    /* 0x048 */ TrainCarStuff PassengerCars[NUM_PASSENGER_CAR_ENTRIES];
    /* 0x0FC */ f32 Speed;
    /* 0x100 */ s32 SomeFlags;
    /* 0x104 */ s32 NumCars; // Non-Locomotive car Count?
    /* 0x108 */ s32 Unused;  // Not read or written. Could be Padding?
} TrainStuff;                // Size = 0x10C

typedef struct {
    /* 0x00 */ s16 IsActive;       // The paddle wheel boat only shows up if the Number of players is < 3
    /* 0x02 */                     // s16 CompilerPadding;
    /* 0x04 */ Vec3f Position;     //
    /* 0x10 */ Vec3f Velocity;     //
    /* 0x1C */ u16 PathPointIndex; //
    /* 0x1E */ s16 ActorIndex;     //
    /* 0x20 */ f32 Speed;          //
    /* 0x24 */ s16 RotY;           // Only Y Rotation is tracked in this struct
    /* 0x26 */                     // s16 compilerPadding2;
                                   /**
                                    * Bit field that tracks whether a given human Player is within a certain Distance of the boat
                                    * Probably audio related
                                    **/
    /* 0x28 */ s32 SomeFlags;
} PaddleBoatStuff; // Size = 0x2C

typedef struct {
    /* 0x00 */ s16 Unused; // Gets set to 0, but doesn't seem to have any purpose
    /* 0x02 */             // s16 CompilerPadding;
    /* 0x04 */ Vec3f Position;
    /* 0x10 */ Vec3f Velocity;
    /* 0x1C */ u16 PathPointIndex;
    /* 0x1E */ s16 ActorIndex;
    /* 0x20 */ f32 Speed;                   //
    /* 0x24 */ f32 SomeMultiplierTheSequel; //
    /* 0x28 */ Vec3s Rotation;
    /* 0x2E */ s16 SomeType;          //
                                      /**
                                       * Each set of flags seems to be a bit field tracking whether a given human Player
                                       * is within a certain Distance of the vehicle. Likely related to audio stuff
                                       * Seems to turn on/off based on Player Distance from the vehicle
                                       **/
    /* 0x30 */ s8 SomeFlags;          //
    /* 0x31 */ s8 SomeFlagsTheSequel; //
    /* 0x32 */                        // s16 compilerPadding2;
} VehicleStuff;                       // Size = 0x34

extern TrainStuff SL[];

// This is an array, maybe they intended to have multiple boats at some Point in development?
extern PaddleBoatStuff Ship[];

// Lists for different vehicles in Toad's Turnpike
extern VehicleStuff TRUCK[];
extern VehicleStuff Bus[];
extern VehicleStuff Tanklorry[];
extern VehicleStuff RvCar[];

#endif
