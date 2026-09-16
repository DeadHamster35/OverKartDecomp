#ifndef DEFINES_H
#define DEFINES_H

/**
 * @brief Options for Controller Pak state
 */
#define OK 1
#define BAD 0

/**
 * @brief Enable debug mode
 *
 * Press start to skip through menus
 *
 * Toggle resource meters by holding R and tapping B. L must not be held.
 *
 * Reset to start screen by holding A, B, R, and L.
 *
 * View player direction and currentPathPoint in a single player race during staging tap L while holding A and B.
 * Turn off this UI by tapping R while holding A and B.
 * @bug This looks like it should work at any point in the race.
 *
 * Set player 1 to the final lap by tapping D-pad up.
 * Set player 1 and player 2 to the final lap by tapping D-pad right.
 * Set player all players to the final lap by tapping D-pad down.
 *
 * Immediately start the race if any controller presses Z.
 *
 * As the ceremony cutscene starts hold a C or D-pad to switch characters.
 * C UP        = LUIGI
 * C LEFT      = YOSHI
 * C RIGHT     = TOAD
 * C DOWN      = DK
 * D-pad UP    = WARIO
 * D-pad LEFT  = PEACH
 * D-pad RIGHT = BOWSER
 * D-pad DOWN  = MARIO
 *
 */
#if defined(GCC) || defined(DEBUG)
#define ENABLE_DEBUG_MODE 1
#else
#define ENABLE_DEBUG_MODE 0
#endif

#define HOLD_ALL_DPAD_AND_C_BUTTONS \
    (U_JPAD | L_JPAD | R_JPAD | D_JPAD | U_CBUTTONS | L_CBUTTONS | R_CBUTTONS | D_CBUTTONS)
#define ALL_BUTTONS                                                                                                   \
    (A_BUTTON | B_BUTTON | L_TRIG | R_TRIG | Z_TRIG | START_BUTTON | U_JPAD | L_JPAD | R_JPAD | D_JPAD | U_CBUTTONS | \
     L_CBUTTONS | R_CBUTTONS | D_CBUTTONS)
/**
 * Replay controller buttons
 * Used for time trial replays (including staff and player ghosts)
 * Each entry is converted to a u32 value
 * This allows access to the button struct member
 */
#define REPLAY_A_BUTTON (1 << 31) // 0x80000000
#define REPLAY_B_BUTTON (1 << 30) // 0x40000000
#define REPLAY_Z_TRIG (1 << 29)   // 0x20000000
#define REPLAY_R_TRIG (1 << 28)   // 0x10000000

#define REPLAY_FRAME_COUNTER 0xFF0000
#define REPLAY_CLEAR_FRAME_COUNTER (0xFFFFFFFF & ~REPLAY_FRAME_COUNTER)
#define REPLAY_STICK_Y 0xFF00
#define REPLAY_STICK_X 0xFF
#define REPLAY_FRAME_INCREMENT 0x10000

/**
 * @brief Jump to demo mode from the debug menu using L and A
 */
#define DEMO_MODE_ACTIVE 1
#define DEMO_MODE_INACTIVE 0

// Float version required for matching
#ifdef VERSION_EU
#define COURSE_TIMER_ITER 0.020041665999999999 // 1 / 50
#ifdef AVOID_UB
#define COURSE_TIMER_ITER_f 0.020041665999999999f // 1 / 50
#else
#define COURSE_TIMER_ITER_f 0.01666666f //! 1 / 60 - Float unchanged in EU
#endif
#else
#define COURSE_TIMER_ITER 0.01666666    // 1 / 60
#define COURSE_TIMER_ITER_f 0.01666666f // 1 / 60
#endif

#define V_BlANK_TIMER_ITER 0.01666666

/**
 * Racing terms:
 * Staging means aligning a racecar to the starting line.
 * Start sequence means waiting for the light to turn green.
 * Used in the Player struct's 'type' member: player->flag
 */
#define PLAYER_INACTIVE 0                 // 0x0000
#define MOTOR_ON (1 << 4)      // 0x0010 // unused?
#define IS_BOMB (1 << 6)      // 0x0040
#define PLAYER_UNKNOWN_0x80 (1 << 7)      // 0x0080 // UNUSED
#define IS_GHOST (1 << 8) // 0x0100
#define IS_GP_OPENING (1 << 9)           // 0x0200
#define PLAYER_UNKNOWN (1 << 10)          // 0x0400 // unused ?
#define IS_RACE_FINISH (1 << 11)   // 0x0800 // Also used to track eliminations in Battle mode.
#define IS_CPU_PLAYER (1 << 12)              // 0x1000
#define NO_CONTROLS (1 << 13)   // 0x2000
#define IS_PLAYER (1 << 14)            // 0x4000
#define EXISTS (1 << 15)           // 0x8000

// Compiles to -0x1000 in diff.py
#define PLAYER_HUMAN_AND_CPU EXISTS | IS_PLAYER | IS_CPU_PLAYER | NO_CONTROLS

#define ZERO_PLAYERS_SELECTED 0
#define ONE_PLAYERS_SELECTED 1
#define TWO_PLAYERS_SELECTED 2
#define THREE_PLAYERS_SELECTED 3
#define FOUR_PLAYERS_SELECTED 4
#define SELECTED_PLAYER_DEFINES_TOTAL 5

enum PlayerId {
    PLAYER_NONE = -1,
    PLAYER_ONE = 0,
    PLAYER_TWO = 1,
    PLAYER_THREE = 2,
    PLAYER_FOUR = 3,
    PLAYER_FIVE = 4,
    PLAYER_SIX = 5,
    PLAYER_SEVEN = 6,
    PLAYER_EIGHT = 7
};

#define NUM_PLAYERS 8

#define MARIO_SIZE 0.75f
#define LUIGI_SIZE 0.75f
#define YOSHI_SIZE 0.75f
#define TOAD_SIZE 0.75f
#define DK_SIZE 0.75f
#define WARIO_SIZE 0.75f
#define PEACH_SIZE 0.75f
#define BOWSER_SIZE 0.75f

// 2P Game has Grand Prix, VS, and Battle as available game types
#define MAX_NUM_MAIN_MENU_GAME_TYPES 3

/**
 * @brief Options for g_gameMode
 */
#define GRAND_PRIX 0
#define TIME_TRIALS 1
#define VERSUS 2
#define BATTLE 3

/**
 * @brief Options for g_raceClass
 * CC stands for cubic-centimetres.
 * It measures engine displacement composed from
 * cylinder volume.
 * Generally, the main determiner of horsepower output.
 */

#define CC_50 0
#define CC_100 1
#define CC_150 2
#define CC_EXTRA 3
#define CC_BATTLE 4

/**
 * @brief Options for g_cupSelect
 * There is a "cup" for battle mode, probably so that
 * the battle courses could be displayed in the same
 * way race courses are.
 */
enum { MUSHROOM_CUP, FLOWER_CUP, STAR_CUP, SPECIAL_CUP, BATTLE_CUP, NUM_CUPS };

#define NUM_COURSES_PER_CUP 4

/**
 * @brief Options for g_courseSelect
 */
enum { COURSE_ONE, COURSE_TWO, COURSE_THREE, COURSE_FOUR };

/**
 * @brief Character IDs
 * Note that these are not the same as the values
 * found in g_player1Character as those are
 * ordered by table
 */
#define MARIO 0
#define LUIGI 1
#define YOSHI 2
#define TOAD 3
#define DK 4
#define WARIO 5
#define PEACH 6
#define BOWSER 7

/**
 * @brief Options for KBGNumber
 */
#define OPTIONS_MENU 5
#define DATA_MENU 6
#define COURSE_DATA_MENU 7
#define LOGO_INTRO_MENU 8
#define CONTROLLER_PAK_MENU 9
#define START_MENU 10
#define MAIN_MENU 11
#define CHARACTER_SELECT_MENU 12
#define COURSE_SELECT_MENU 13
#define RACING_DUPLICATE 14

/**
 * @brief Options for gGameState next_sequence_mode and gGotoMenu
 */
#define START_MENU_FROM_QUIT 0
#define MAIN_MENU_FROM_QUIT 1
#define PLAYER_SELECT_MENU_FROM_QUIT 2
#define COURSE_SELECT_MENU_FROM_QUIT 3
#define RACING 4
#define ENDING 5
#define CREDITS_SEQUENCE 9

/**
 * @brief Options for g_ScreenSplitB and g_ScreenSplitA
 */
#define SCREEN_MODE_1P 0
#define SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL 1
#define SCREEN_MODE_2P_SPLITSCREEN_VERTICAL 2
#define SCREEN_MODE_3P_4P_SPLITSCREEN 3
#define NUM_SCREEN_MODES 4

/**
 * @brief Indexes for accessing Time Trial records
 */
#define TIME_TRIAL_3LAP_RECORD_1 0
#define TIME_TRIAL_3LAP_RECORD_2 1
#define TIME_TRIAL_3LAP_RECORD_3 2
#define TIME_TRIAL_3LAP_RECORD_4 3
#define TIME_TRIAL_3LAP_RECORD_5 4
#define TIME_TRIAL_1LAP_RECORD 5

/**
 * @brief Text color options
 * The 2 cycling options cycle through Blue -> Green -> Red
 * Cycle 2 appears to be one step ahead of Cycle 1
 * Other values like 6 and 7 sort of work, they cause rapid
 * flashing between 2 colors, but also cause individual characters
 * to be cutoff on their right side
 */
enum COLOR_ID {
    TEXT_BLUE,
    TEXT_GREEN,
    TEXT_RED,
    TEXT_YELLOW,
    TEXT_BLUE_GREEN_RED_CYCLE_1,
    TEXT_BLUE_GREEN_RED_CYCLE_2
};

/**
 * @brief Sound mode options
 * Option 2 appears to be unused, as such its probably not
 * a valid option
 */
#define SOUND_STEREO 0
#define SOUND_HEADPHONES 1
#define SOUND_UNUSED 2
#define SOUND_MONO 3
#define NUM_SOUND_MODES 4

/**
 * @brief Title screen demo options
 * All demos use 100 CC
 */
#define DEMO_ONE 0   // Mario,                  Mario Raceway,   Grand Prix
#define DEMO_TWO 1   // Yoshi DK,               Choco Mountain,  Versus
#define DEMO_THREE 2 // Luigi,                  Kalamari Desert, Grand Prix
#define DEMO_FOUR 3  // Wario Yoshi Bowser,     Wario Stadium,   Versus
#define DEMO_FIVE 4  // Bowser,                 Bowser Castle,   Grand Prix
#define DEMO_SIX 5   // Mario Luigi Peach Toad, Sherbert Land,   Versus
#define NUM_DEMOS 6

/**
 * @brief Item IDs
 */
enum ITEMS {
    /* 0x00 */ ITEM_NONE = 0,
    /* 0x01 */ ITEM_BANANA,
    /* 0x02 */ ITEM_BANANA_BUNCH,
    /* 0x03 */ ITEM_GREEN_SHELL,
    /* 0x04 */ ITEM_TRIPLE_GREEN_SHELL,
    /* 0x05 */ ITEM_RED_SHELL,
    /* 0x06 */ ITEM_TRIPLE_RED_SHELL,
    /* 0x07 */ ITEM_BLUE_SPINY_SHELL,
    /* 0x08 */ ITEM_THUNDERBOLT,
    /* 0x09 */ ITEM_FAKE_ITEM_BOX,
    /* 0x0A */ ITEM_STAR,
    /* 0x0B */ ITEM_BOO,
    /* 0x0C */ ITEM_MUSHROOM,
    /* 0x0D */ ITEM_DOUBLE_MUSHROOM,
    /* 0x0E */ ITEM_TRIPLE_MUSHROOM,
    /* 0x0F */ ITEM_SUPER_MUSHROOM
};

enum CPU_BEHAVIOURS {
    BEHAVIOUR_NONE = 0,
    BEHAVIOUR_1,
    BEHAVIOUR_HOP,
    BEHAVIOUR_DRIVE_CENTER,
    BEHAVIOUR_DRIVE_LEFT,
    BEHAVIOUR_DRIVE_OUTER,
    BEHAVIOUR_NORMAL_SPEED,
    BEHAVIOUR_FAST_SPEED,
    BEHAVIOUR_SLOW_SPEED,
    BEHAVIOUR_9,
    BEHAVIOUR_10,
    BEHAVIOUR_MAX_SPEED
};

enum DIRECTION { NORTH, EAST, SOUTH, WEST };

enum PLACE { FIRST_PLACE, SECOND_PLACE, THIRD_PLACE, FOURTH_PLACE };

/**
 * @brief Balloon status
 */
#define BALLOON_STATUS_GONE 0
#define BALLOON_STATUS_PRESENT 1
#define BALLOON_STATUS_DEPARTING 2

/**
 * @brief Max representable time, 100 minutes measured in centiseconds
 */
#define MAX_TIME 0x927C0

/* The codebase uses 16-bit representations of angles (either u16 or s16 depending on context). 
   Thus, values of [0, 2**16 - 1] (for u16) or [-2**15, 2**15 - 1] (for s16) represent evenly 
   spaced angles around a circle. (e.g. (u16) 0x8000 is 2**15, which represents half a circle, or 180 degrees)
   
   However, it is clear that the developers were thinking in terms of degrees and often worked
   with values that corresponded to a specific number of degrees. The DEGREES macro converts
   from the specified number of degrees to the 16-bit representation. This allows readers to
   think in degrees while leaving the compiled machine code unaffected.

   Note that 2**16 / 360 is not an integer. It has a value of ~182.04, but is rounded to 182 
   by the DEGREES function. There is some inconsistency with how the codebase handles this. 
   e.g. 70 degrees might be represented as DEGREES(70) or 70 * DEGREES(1), which are slightly
   different values due to this rounding. As a practical matter, this rounding is less than
   0.1 degrees in all cases */
#define DEGREES(degree) ((u16) (degree * 65536.0f / 360.0f))

// player->hit_flag

/* The first time you touch a penguin or bat, and anytime you touch a thwomp during a
   race will set a flag which will cause your next spinout to be instant (i.e. no 
   sliding forward). Probably half-implemented code to prevent sliding through enemies,
   but causes this bug */
#define CRITTER_TOUCH 0x2 // Touched a penguin, bat or thwomp
#define CRITTER_TOUCH_GATE 0x4
#define INSTANT_SPINOUT 0x40

// player->water_flag
/* Deals with the lower out of bounds (OOB) plane on levels. Represented by fluids (water / lava)
  or nothing for Rainbow Road and Skyscraper. */
#define SUBMERGED 0x1 // Set while mostly under the plane. Does not necessarily trigger Lakitu on Koopa Troopa Beach.
#define HALF_SUBMERGED 0x2 // Set when passing through the lower plane in either direction
// The next two are also activated when passing through the lower plane.
#define SPLASH_START 0x4 // Stays active until Lakitu places back on track
#define SPLASH_DIVE 0x8 // Active while under a non-fluid OOB plane. Is momentarily active when passing through fluids.


/* UNK_002 has something to do with player animations. Each player has a 32-bit
flag broken into 8 groups of 4 bits. Those 4 bits affect how each of the 8 players
appear to the specified player */
#define P1_DMA_SPRITE 0x1 // Seems to be set when the kart animation has to change.
#define P1_DRAW_SPRITE 0x2 
#define P1_FLIP_SPRITE 0x4 /* Unclear, but has to do with viewing the side of player. At least tends to change if target
player spins. Something  with avoding rollover of aniamation frame data? */
#define P1_DRAW_PARTICLES 0x8 // Seems to be whether you are in a rectangle shooting out from both sides of target player

#define MUSIC_NOTE 0x20     // Whistle spinout save graphic
#define CRASH 0x40       // Crash! graphic (vertical tumble)
#define WHIRRR 0x80      // Whirrr! graphic (spinning out)
#define POOMP 0x100      // Poomp! graphic (landing from a height)
#define BOING 0x800      // Boing! graphic (hopping)
#define FLASH 0x1000 // Big shock looking graphic when starting tumble

// player->jugemu_flag
#define IS_IN_WATER 0x1 // While lakitu is grabbing you, but before the scene transition of being placed on the track
#define ON_LAKITU_ROD   0x2
#define IS_FADING_OUT    0x4 // Disintegration and reintegration effect when transitioning from retrieval to placement
#define LAKITU_SCENE     0x8 // the whole segment from when lakitu is called to when you regain control
#define IS_FROZEN   0x10 // Cold colors on Sherbet Land after in frigid water
#define IS_MELTING  0x20 // Regaining usual colors post frigid effect
#define HAS_ICE_CUBE   0x80 // In the ice cube
#define LAKITU_CAMERA  0x100 // Player went over (or is on) an OOB area. Cancelled if touch back in bounds
#define LAVA_EFFECT   0x1000 // smoky effect when retrieved from lava
#define WATER_EFFECT  0x2000 // dripping effect when retreived from water

// player->handling_flag
#define REVERSE_GEAR               0x1
#define STEER_RIGHT            0x2 // non-drifting (more than 5 degrees)
#define STEER_LEFT             0x4 // non-drifting (more than 5 degrees)
#define REVERSE_ACCEL        0x8 // includes lakitu
#define CPU_SIMPLE_KART         0x10 // pointless, only unsets itself
#define ACCELERATE             0x20 // Closely tied to just pressing A. Possible exception for AB-spins
#define START_SPIN_RIGHT  0x40 // Spinning out while facing right (not actually used for anything)
#define START_SPIN_LEFT   0x80 // Spinning out while facing left
#define LANDING_DUST     0x100 // Causes particles after a vertical tumble, I think
#define BOO_SMOKE_EFFECT    0x200
#define TROPHYJUMP_START        0x400 // locked behind 0x800 (result_kart_jump_start)
#define TROPHYJUMP_END        0x800 // locked behind 0x400 (WeaponStatus -> set_result_jump)
#define TROPHYJUMP_STARS      0x1000 // 0x1000 locked behind 0x400 (WeaponStatus -> set_result_jump)
#define TROPHYJUMP_KEEP      0x2000 // 0x2000 locked behind 0x400 and 0x800 (WeaponStatus -> set_result_jump, SetStrategy -> result_jump,
                                  // result_kart_jump_start)
#define SPINOUT_SWERVE 0x4000
#define BALLOON_CUT 0x8000 // 0x8000 something battle related, unclear if ever set

/*
 * @brief triggers indicating that an effect should be applied to a kart
 */
#define HIT_BANANA              0x1 // hits a banana
#define HIT_REDSHELL             0x2 // hit by a red shell, blue shell, or hit a mole
#define HIT_GREENSHELL              0x4 // hit by a green shell
#define PRESS_SAVESPIN        0x80 // spinning out from erratic driving
#define PRESS_SQUISH         0x100 // stomped by thwomp
#define USE_MUSHROOM                0x200 // being boosted by trigger a mushroom
#define USE_BOO                   0x800 // being a boo
#define USE_FEATHER        0x1000 // Unused
#define USE_STAR                 0x2000 // Starting a star
#define HIT_LIGHTNING     0x4000 // Struck by lightning
#define PRESS_BOOSTJUMP      0x8000 // being boosted by a ramp
#define HIT_TORNADO      0x20000 // Unused
#define USE_BANANA            0x40000 // holding a non-shell item behind you
#define HIT_BOAT     0x80000 // hit paddle boat
#define UNUSED_TRIGGER_0x10000     0x100000 // Unused
#define HIT_CRITTER            0x200000 // hit crab or spiny spinout or losing versus race
#define HIT_BOMB    0x400000 // hitting a fake item / bomb / snowman / car / train
#define PRESS_BOOSTPAD 0x800000 // being boosted by a boost pad
#define HIT_BOMB_THROW       0x1000000 // being hit by a star
#define USE_STARTDASH       0x2000000 // Start boost
#define PRESS_BECOME_BOMB        0x4000000 // When losing battle mode
#define PRESS_IS_BOMB        0x8000000 // When becoming a bomb in battle mode
#define PRESS_STARTSPIN    0x10000000 // Spinning out by holding gas at start of race

#define ALL_TRIGGERS (0xFFFFFFFF)
#define RACING_SPINOUT_TRIGGERS (HIT_CRITTER | PRESS_SAVESPIN | HIT_BANANA) // 0x200081
#define RAMP_BOOST_TRIGGERS (PRESS_BOOSTPAD | PRESS_BOOSTJUMP)               // 0x00808000
#define ANY_BOOST_TRIGGERS (RAMP_BOOST_TRIGGERS | USE_MUSHROOM)                                // 0x00808200
#define STATE_TRANSITION_TRIGGERS \
    (USE_STAR | USE_BOO | USE_FEATHER | HIT_TORNADO) // 0x00023800
#define HIT_TRIGGERS                                                                                 \
    (HIT_BOMB_THROW | HIT_BOMB | HIT_LIGHTNING | HIT_GREENSHELL | \
     HIT_REDSHELL | PRESS_SQUISH) // 0x01404106

/**
 * @brief effect of player's
 * for effects
 */
// clang-format off
#define BREAKE                   0x1 // pressing brake
#define D_JUMP                       0x2 // from when you hop to when you land
#define DASH_MIDJUMP           0x4 // being boosted by DKJP ramp
#define N_JUMP                    0x8 // in midair
#define DRIFT                 0x10 // drifting
#define SPIN_TURN                  0x20 // spinning with a+b
#define SPIN_R          0x40 // spinout (from erratic driving) or crab
#define SPIN_L           0x80 // spinout (from hitting a banana or another driver)
#define DRIFT_TURBO              0x100 // mini-turbo
#define STAR                    0x200 // using a star
#define ROLLOVER      0x400 // tumbling after hit with a green shell
#define WHEELSPIN     0x800 // decreased steering sensitivity after hitting a banana, but before spinning out
#define CENTER_LINE              0x1000 // lost race in GP mode
#define TURBO               0x2000 // using a mushroom
#define RAPID_ACC    0x4000 // spinning out by pressing a too early at the startline or after lakitu retrieves you
#define CARHIT             0x8000 // bouncing off an enemy (penguin, thwomp, etc)
#define ROLLOVER_FALL        0x10000 // tumbling after hitting steep terrain
#define THUNDER_SPIN      0x20000 // spinning and shrinking during a lightning strike
#define DISABLE_SPIN   0x40000 // avoid spinning out by pressing b
#define WING       0x80000 // Only set when soundEffect 0x1000 is set. Unclear if it ever is
#define DASH_JUMP   0x100000 // being boosted by the Royal Raceway ramp
#define SLIP_STREAM             0x200000 // CPU Rubberbanding flag
#define REVERSE              0x400000 // facing backwards
#define STORM     0x800000 // Only set when soundEffect 0x20000 is set
#define EXPLODE     0x1000000 // vertical launch (hitting car, snowman, etc.)
#define THROW_EXPLODE         0x2000000 // being hit by a red shell or star
#define BROKEN              0x4000000 // hitting an object
#define RESTORE         0x8000000 // briefly after being squished (boulder, thwomp, etc.)
#define HEIGHT_JUMP 0x10000000 // UNUSED (set in unused SetHeightJump)
#define STOPSTERR      0x20000000 // drifting toward the outside of your turn
#define THUNDER          0x40000000 // suffering the effects of lightning
#define TERESA                0x80000000 // being a boo
#define ALL_EFFECTS               0xFFFFFFFF
// clang-format on

/**
 * @brief durations of effects
 */
#define STAR_EFFECT_DURATION 0xA
#define BOO_EFFECT_DURATION 0x7

/**
 * @brief alpha relates values
 */
#define ALPHA_MAX 0xFF
#define ALPHA_MIN 0x0
#define ALPHA_BOO_EFFECT 0x60

/**
 * @brief shell state
 *
 */
#define SPAWN_FIRST_SHELL 0
#define SPAWN_SECOND_SHELL 1
#define SPAWN_THIRD_SHELL 2

#define GPACK_RGB888(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define COLOR_LIGHT GPACK_RGB888(0x1C, 0x00, 0x00)
#define COLOR_LAVA GPACK_RGB888(0x34, 0x00, 0x00)
#define COLOR_BLACK GPACK_RGB888(0, 0, 0)

#endif // DEFINES_H

/**
 *
 * Collision mesh flags
 *
 */

#define GRID_SIZE 32

#define FACING_Y_AXIS 0x4000
#define FACING_X_AXIS 0x8000
#define FACING_Z_AXIS 0x2000

// race states

#define RACE_NONE 0
#define RACE_PREP 1
#define RACE_STAGING 2
#define RACE_IN_PROGRESS 3
#define RACE_HUMAN_FINISHED 4 // At least 1 human has finished
#define RACE_DONE 5
#define RACE_QUITTING 6
#define RACE_QUADRANT_RESULTS 7 // The results screen in GP and Time trials mode
