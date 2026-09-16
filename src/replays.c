/*
 * @file Replays
 * Handles 3 types of time trial replays:
 * 1. Post time trial replays, which a player can watch after a time trial
 * 2. Player ghosts, which were driven by a player and can be raced against
 * 3. Staff ghosts, which can be unlocked on the raceway tracks with a fast enough time
 * All 3 use the same system for storing / replaying the inputs to reproduce a time trial
 * See replayread_key_data for additional technical details
 */

#include <ultra64.h>
#include <macros.h>
#include <common_structs.h>
#include <defines.h>
#include <decode.h>
#include <mk64.h>
#include <course.h>

#include "main.h"
#include "code_800029B0.h"
#include "buffers.h"
#include "save.h"
#include "replays.h"
#include "code_8006E9C0.h"
#include "menu_items.h"
#include "code_80057C60.h"
#include "kart_dma.h"
#include "OverKartHooks.h"

extern s32 encpass2(s32 input, s32, s32);
extern s32 encpass1(void*, s32, s32);

u8* sReplayGhostBuffer;
s16 sReplayGhostBufferSize;
s16 D_80162D86;

static u16 sPlayerGhostButtonsPrev;
static u32 sPlayerGhostFramesRemaining;
static s16 sPlayerGhostReplayIdx;
static u32* sPlayerGhostReplay;

static u16 sButtonsPrevCourseGhost;
static u32 sCourseGhostFramesRemaining;
static s16 sCourseGhostReplayIdx;
static u32* sCourseGhostReplay;

static u16 sPostTTButtonsPrev;
static s32 sPostTTFramesRemaining;
static s16 sPostTTReplayIdx;
static u32* sPostTTReplay;

static s16 sPlayerInputIdx;
static u32* sPlayerInputs;

static u16 sPrevCourseId;
u32 map;
s32 read_pointer;
s32 write_pointer;
s32 replay_pointer;
u16 ghost_status;
u16 romghost_status;
u16 replayghost_status;
s32 ghost_success;
s32 ghost_kart; // ghost kart id?
s32 romghost_kart;
s32 replayghost_kart;
UNUSED static s32 sUnusedReplayCounter;
s32 ghost_write;
UNUSED static s32 bUnusedCourseGhostDisabled;
s32 lost_ghost;
s32 D_80162DFC;

s32 D_80162E00;

u32* keystock1 = (u32*) &D_802BFB80.arraySize8[0][2][3];
u32* keystock2 = (u32*) &D_802BFB80.arraySize8[1][1][3];

extern s32 gLapCountByPlayerId[];

extern StaffGhost* d_mario_raceway_staff_ghost;
extern StaffGhost* d_royal_raceway_staff_ghost;
extern StaffGhost* d_luigi_raceway_staff_ghost;

void DMAROMGhost(void) {
    sCourseGhostReplay = (u32*) &D_802BFB80.arraySize8[0][2][3];
    osInvalDCache(&sCourseGhostReplay[0], 0x4000);
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(map)],
                 sCourseGhostReplay, 0x4000, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    sCourseGhostFramesRemaining = (*sCourseGhostReplay & REPLAY_FRAME_COUNTER);
    sCourseGhostReplayIdx = 0;
}

void replay_ghost(void) {
    sPostTTReplay = (u32*) &D_802BFB80.arraySize8[0][replay_pointer][3];
    sPostTTFramesRemaining = *sPostTTReplay & REPLAY_FRAME_COUNTER;
    sPostTTReplayIdx = 0;
}

void read_ghost(void) {
    sPlayerGhostReplay = (u32*) &D_802BFB80.arraySize8[0][read_pointer][3];
    sPlayerGhostFramesRemaining = (s32) *sPlayerGhostReplay & REPLAY_FRAME_COUNTER;
    sPlayerGhostReplayIdx = 0;
}
/**
 * Activates staff ghost if time trial lap time is low enough
 *
 */
#ifdef VERSION_EU
#define GHOST_UNLOCK_MARIO 10700
#define GHOST_UNLOCK_ROYAL 19300
#define GHOST_UNLOCK_LUIGI 13300
#else
#define GHOST_UNLOCK_MARIO 9000
#define GHOST_UNLOCK_ROYAL 16000
#define GHOST_UNLOCK_LUIGI 11200

#endif

void check_romghost(void) {
    u32 bestTime; // Appears to be best player 3lap time.
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (g_courseID) {
        case COURSE_MARIO_RACEWAY:
            bestTime = GetRecordTime2(0) & 0xfffff;
            if (bestTime <= GHOST_UNLOCK_MARIO) {
                romghost_status = 0;
                bUnusedCourseGhostDisabled = 0;
            } else {
                romghost_status = 1;
                bUnusedCourseGhostDisabled = 1;
            }
            map = (u32) &d_mario_raceway_staff_ghost;
            romghost_kart = 0;
            break;
        case COURSE_ROYAL_RACEWAY:
            bestTime = GetRecordTime2(0) & 0xfffff;
            if (bestTime <= GHOST_UNLOCK_ROYAL) {
                romghost_status = 0;
                bUnusedCourseGhostDisabled = 0;
            } else {
                romghost_status = 1;
                bUnusedCourseGhostDisabled = 1;
            }
            map = (u32) &d_royal_raceway_staff_ghost;
            romghost_kart = 6;
            break;
        case COURSE_LUIGI_RACEWAY:
            bestTime = GetRecordTime2(0) & 0xfffff;
            if (bestTime <= GHOST_UNLOCK_LUIGI) {
                romghost_status = 0;
                bUnusedCourseGhostDisabled = 0;
            } else {
                romghost_status = 1;
                bUnusedCourseGhostDisabled = 1;
            }
            map = (u32) &d_luigi_raceway_staff_ghost;
            romghost_kart = 1;
            break;
        default:
            romghost_status = 1;
            bUnusedCourseGhostDisabled = 1;
    }
#else

#endif
}

s32 make_press_ghost(void) {
    s32 phi_v0;

    if (sReplayGhostBufferSize != 0) {
        // encpass1 in mio0_decode.s
        encpass1((void*) sReplayGhostBuffer, (sReplayGhostBufferSize * 4) + 0x20, (s32) keystock1);
        phi_v0 =
            encpass2((s32) keystock1, (sReplayGhostBufferSize * 4) + 0x20, (s32) keystock2);
        return phi_v0 + 0x1e;
    }
}

void make_melt_ghost(void) {
    sPlayerGhostReplay = (u32*) &D_802BFB80.arraySize8[0][read_pointer][3];
    decodeMIO0((u8*) keystock2, (u8*) sPlayerGhostReplay);
    sPlayerGhostFramesRemaining = (s32) (*sPlayerGhostReplay & REPLAY_FRAME_COUNTER);
    sPlayerGhostReplayIdx = 0;
    D_80162E00 = 1;
}

void swap_ghostbuffer(void) {
    s16 temp_v0;

    if (read_pointer == 1) {
        read_pointer = 0;
        write_pointer = 1;
    } else {
        read_pointer = 1;
        write_pointer = 0;
    }
    temp_v0 = sPlayerInputIdx;
    sReplayGhostBuffer = (void*) &D_802BFB80.arraySize8[0][read_pointer][3];
    sReplayGhostBufferSize = temp_v0;
    D_80162D86 = temp_v0;
}

void initial_ghost(void) {

    if (g_gameMode == TIME_TRIALS) {

        check_romghostDefault();

        if (sPrevCourseId != g_courseID) {
            ghost_status = 1;
        }

        sPrevCourseId = (u16) g_courseID;
        ghost_write = 0;
        sUnusedReplayCounter = 0;
        lost_ghost = 0;

        if (g_gameMode == TIME_TRIALS && g_ScreenSplitA == SCREEN_MODE_1P) {

            if (replay_flag == 1) {
                replay_ghost();
                if (replayghost_status == 0) {
                    read_ghost();
                }
                if (romghost_status == 0) {
                    DMAROMGhostDefault();
                }
            } else {

                replayghost_status = 1U;
                sPlayerInputs = (u32*) &D_802BFB80.arraySize8[0][write_pointer][3];
                sPlayerInputs[0] = -1;
                sPlayerInputIdx = 0;
                ghost_success = 0;
                InitGhostData();
                if (ghost_status == 0) {
                    read_ghost();
                }
                if (romghost_status == 0) {
                    DMAROMGhostDefault();
                }
            }
        }
    }
    SetGhostData();
}

/* Special handling for buttons saved in replays. The listing of L_TRIG here is odd
 * because it is not saved in the replay data structure. Possibly, L was initially deleted
 * here to make way for the frame counter, but then the format changed when the stick
 * coordinates were added */
#define REPLAY_MASK (ALL_BUTTONS ^ (A_BUTTON | B_BUTTON | Z_TRIG | R_TRIG | L_TRIG))

/* Inputs for replays (including player and course ghosts) are saved in a s32[] where
   each entry is a combination of the inputs and  how long those inputs were held for.
   In essence it's "These buttons were pressed and the joystick was in this position.
   This was the case for X frames".

   bits 1-8: Stick X
   bits 9-16: Stick Y
   bits 17-24: Frame counter
   bits 25-28: Unused
   bit 29: R
   bit 30: Z
   bit 31: B
   bit 32: A
*/
void replayread_key_data(void) {
    u32 inputs;
    u32 stickBytes;
    UNUSED u16 unk;
    u16 buttons_temp;
    s16 stickVal;
    s16 buttons = 0;

    if (sPostTTReplayIdx >= 0x1000) {
        gPlayerOne->flag = IS_RACE_FINISH | NO_CONTROLS | IS_CPU_PLAYER;
        return;
    }

    inputs = sPostTTReplay[sPostTTReplayIdx];
    stickBytes = inputs & REPLAY_STICK_X;

    // twos complement trick, converting singned 8-bit value to signed 16 bit
    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }

    stickBytes = (u32) (inputs & REPLAY_STICK_Y) >> 8;
    gControllerEight->AnalogX = stickVal;

    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }
    gControllerEight->AnalogY = stickVal;
    if (inputs & REPLAY_A_BUTTON) {
        buttons |= A_BUTTON;
    }
    if (inputs & REPLAY_B_BUTTON) {
        buttons |= B_BUTTON;
    }
    if (inputs & REPLAY_Z_TRIG) {
        buttons |= Z_TRIG;
    }
    if (inputs & REPLAY_R_TRIG) {
        buttons |= R_TRIG;
    }
    buttons_temp = gControllerEight->ButtonPressed & REPLAY_MASK;
    gControllerEight->ButtonPressed = (buttons & (buttons ^ sPostTTButtonsPrev)) | buttons_temp;
    buttons_temp = gControllerEight->ButtonReleased & REPLAY_MASK;
    gControllerEight->ButtonReleased = (sPostTTButtonsPrev & (buttons ^ sPostTTButtonsPrev)) | buttons_temp;
    sPostTTButtonsPrev = buttons;
    gControllerEight->ButtonHeld = buttons;

    if (sPostTTFramesRemaining == 0) {
        sPostTTReplayIdx++;
        sPostTTFramesRemaining = (s32) (sPostTTReplay[sPostTTReplayIdx] & REPLAY_FRAME_COUNTER);
    } else {
        sPostTTFramesRemaining -= REPLAY_FRAME_INCREMENT;
    }
}

// See replayread_key_data comment
void romread_key_data(void) {
    u32 inputs;
    u32 stickBytes;
    UNUSED u16 unk;
    u16 buttonsTemp;
    s16 stickVal;
    s16 buttons = 0;
    if (sCourseGhostReplayIdx >= 0x1000) {
        kill_ghost(gPlayerThree);
        return;
    }

    inputs = sCourseGhostReplay[sCourseGhostReplayIdx];
    stickBytes = inputs & REPLAY_STICK_X;
    // converting signed 8-bit values to signed 16-bit values
    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }
    stickBytes = (u32) (inputs & REPLAY_STICK_Y) >> 8;
    gControllerSeven->AnalogX = stickVal;

    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }
    gControllerSeven->AnalogY = stickVal;

    if (inputs & REPLAY_A_BUTTON) {
        buttons = A_BUTTON;
    }
    if (inputs & REPLAY_B_BUTTON) {
        buttons |= B_BUTTON;
    }
    if (inputs & REPLAY_Z_TRIG) {
        buttons |= Z_TRIG;
    }
    if (inputs & REPLAY_R_TRIG) {
        buttons |= R_TRIG;
    }
    // Blanks the A, B, Z, R and L buttons
    buttonsTemp = gControllerSeven->ButtonPressed & REPLAY_MASK;
    gControllerSeven->ButtonPressed = (buttons & (buttons ^ sButtonsPrevCourseGhost)) | buttonsTemp;
    buttonsTemp = gControllerSeven->ButtonReleased & REPLAY_MASK;
    gControllerSeven->ButtonReleased = (sButtonsPrevCourseGhost & (buttons ^ sButtonsPrevCourseGhost)) | buttonsTemp;
    sButtonsPrevCourseGhost = buttons;
    gControllerSeven->ButtonHeld = buttons;
    if (sCourseGhostFramesRemaining == 0) {
        sCourseGhostReplayIdx++;
        sCourseGhostFramesRemaining = (s32) (sCourseGhostReplay[sCourseGhostReplayIdx] & REPLAY_FRAME_COUNTER);
    } else {
        sCourseGhostFramesRemaining -= (s32) REPLAY_FRAME_INCREMENT;
    }
}

// See replayread_key_data comment
void read_key_data(void) {
    u32 inputs;
    u32 stickBytes;
    UNUSED u16 unk;
    u16 buttons_temp;
    s16 stickVal;
    s16 buttons = 0;

    if (sPlayerGhostReplayIdx >= 0x1000) {
        kill_ghost(gPlayerTwo);
        return;
    }
    inputs = sPlayerGhostReplay[sPlayerGhostReplayIdx];
    stickBytes = inputs & REPLAY_STICK_X;
    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | ~0xFF);
    }

    stickBytes = (u32) (inputs & REPLAY_STICK_Y) >> 8;

    gControllerSix->AnalogX = stickVal;

    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }

    gControllerSix->AnalogY = stickVal;

    if (inputs & REPLAY_A_BUTTON) {
        buttons = A_BUTTON;
    }
    if (inputs & REPLAY_B_BUTTON) {
        buttons |= B_BUTTON;
    }
    if (inputs & REPLAY_Z_TRIG) {
        buttons |= Z_TRIG;
    }
    if (inputs & REPLAY_R_TRIG) {
        buttons |= R_TRIG;
    }
    buttons_temp = gControllerSix->ButtonPressed & REPLAY_MASK;
    gControllerSix->ButtonPressed = (buttons & (buttons ^ sPlayerGhostButtonsPrev)) | buttons_temp;

    buttons_temp = gControllerSix->ButtonReleased & REPLAY_MASK;
    gControllerSix->ButtonReleased = (sPlayerGhostButtonsPrev & (buttons ^ sPlayerGhostButtonsPrev)) | buttons_temp;
    sPlayerGhostButtonsPrev = buttons;
    gControllerSix->ButtonHeld = buttons;

    if (sPlayerGhostFramesRemaining == 0) {
        sPlayerGhostReplayIdx++;
        sPlayerGhostFramesRemaining = (s32) (sPlayerGhostReplay[sPlayerGhostReplayIdx] & REPLAY_FRAME_COUNTER);
    } else {
        sPlayerGhostFramesRemaining -= (s32) REPLAY_FRAME_INCREMENT;
    }
}

// See replayread_key_data comment
void write_key_data(void) {
    s16 buttons;
    u32 inputs;
    u32 stickX;
    u32 stickY;
    u32 inputCounter;
    u32 prevInputsWCounter;
    u32 prevInputs;
    /* Input file is too long or picked up by lakitu or Out of bounds
    Not sure if there is any way to be considered out of bounds without lakitu getting called */
    if (((sPlayerInputIdx >= 0x1000) || ((gPlayerOne->jugemu_flag & ON_LAKITU_ROD) != 0)) ||
        ((gPlayerOne->jugemu_flag & LAKITU_SCENE) != 0)) {
        lost_ghost = 1;
        return;
    }

    stickX = gControllerOne->AnalogX;
    stickX &= 0xFF;
    stickY = gControllerOne->AnalogY;
    stickY = (stickY & 0xFF) << 8;
    buttons = gControllerOne->ButtonHeld;
    inputs = 0;
    if (buttons & A_BUTTON) {
        inputs |= REPLAY_A_BUTTON;
    }
    if (buttons & B_BUTTON) {
        inputs |= REPLAY_B_BUTTON;
    }
    if (buttons & Z_TRIG) {
        inputs |= REPLAY_Z_TRIG;
    }
    if (buttons & R_TRIG) {
        inputs |= REPLAY_R_TRIG;
    }
    inputs |= stickX;
    inputs |= stickY;
    prevInputsWCounter = sPlayerInputs[sPlayerInputIdx];
    /* The 5th and 6th bytes from the right are counters. Instead of saving the same inputs over and over,
    it says "these inputs were played for __ frames" */
    prevInputs = prevInputsWCounter & REPLAY_CLEAR_FRAME_COUNTER;
    // first frame of inputs
    if ((*sPlayerInputs) == -1) {

        sPlayerInputs[sPlayerInputIdx] = inputs;

    } else if (prevInputs == inputs) {

        inputCounter = prevInputsWCounter & REPLAY_FRAME_COUNTER;

        if (inputCounter == REPLAY_FRAME_COUNTER) {

            sPlayerInputIdx++;
            sPlayerInputs[sPlayerInputIdx] = inputs;

        } else {
            // increment counter by 1
            prevInputsWCounter += REPLAY_FRAME_INCREMENT;
            sPlayerInputs[sPlayerInputIdx] = prevInputsWCounter;
        }
    } else {
        sPlayerInputIdx++;
        sPlayerInputs[sPlayerInputIdx] = inputs;
    }
}

// sets player to AI? (unconfirmed)
void kill_ghost(Player* ply) {
    if (((ply->flag & IS_GHOST) != 0) && (ply != gPlayerOne)) {
        ply->flag = IS_RACE_FINISH | NO_CONTROLS | IS_CPU_PLAYER;
    }
}

void norm_data_control(void) {
    if (g_gameMode == TIME_TRIALS) {
        if ((gLapCountByPlayerId[0] == 3) && (ghost_success == 0) && (lost_ghost != 1)) {
            if (ghost_status == 1) {
                replay_pointer = write_pointer;
                swap_ghostbuffer();
                ghost_status = 0;
                ghost_success = 1;
                ghost_kart = gPlayerOne->kart;
                replayghost_kart = gPlayerOne->kart;
                D_80162E00 = 0;
                D_80162DFC = playerHUD[PLAYER_ONE].totaltime;
                kill_ghost(gPlayerTwo);
                kill_ghost(gPlayerThree);
            } else if (gLapCountByPlayerId[1] != 3) {
                replay_pointer = write_pointer;
                swap_ghostbuffer();
                ghost_success = 1;
                ghost_kart = gPlayerOne->kart;
                D_80162DFC = playerHUD[PLAYER_ONE].totaltime;
                D_80162E00 = 0;
                replayghost_kart = gPlayerOne->kart;
                kill_ghost(gPlayerTwo);
                kill_ghost(gPlayerThree);
            } else {
                sReplayGhostBuffer = D_802BFB80.arraySize8[0][read_pointer][3].pixel_index_array;
                sReplayGhostBufferSize = D_80162D86;
                replay_pointer = write_pointer;
                replayghost_kart = gPlayerOne->kart;
                replayghost_status = 0;
                ghost_status = 0;
                ghost_success = 1;
                kill_ghost(gPlayerTwo);
                kill_ghost(gPlayerThree);
            }
        } else {
            if ((gLapCountByPlayerId[0] == 3) && (ghost_success == 0) && (lost_ghost == 1)) {
                sReplayGhostBuffer = D_802BFB80.arraySize8[0][read_pointer][3].pixel_index_array;
                sReplayGhostBufferSize = D_80162D86;
                ghost_success = 1;
            }
            if ((gPlayerOne->flag & IS_RACE_FINISH) == IS_RACE_FINISH) {
                kill_ghost(gPlayerTwo);
                kill_ghost(gPlayerThree);
            } else {
                sUnusedReplayCounter += 1;
                if (sUnusedReplayCounter > 100) {
                    sUnusedReplayCounter = 100;
                }
                if ((g_gameMode == TIME_TRIALS) && (g_ScreenSplitA == SCREEN_MODE_1P)) {
                    if ((ghost_status == 0) && (gLapCountByPlayerId[1] != 3)) {
                        read_key_data();
                    }
                    if ((romghost_status == 0) && (gLapCountByPlayerId[2] != 3)) {
                        romread_key_data();
                    }
                    if (!(gPlayerOne->flag & IS_RACE_FINISH)) {
                        write_key_data();
                    }
                }
            }
        }
    }
}

void replay_data_control(void) {
    if ((g_gameMode == TIME_TRIALS) && (g_gameMode == TIME_TRIALS) && (g_ScreenSplitA == SCREEN_MODE_1P)) {
        if ((replayghost_status == 0) && (gLapCountByPlayerId[1] != 3)) {
            read_key_data(); // 3
        }
        if ((romghost_status == 0) && (gLapCountByPlayerId[2] != 3)) {
            romread_key_data(); // 2
        }
        if ((gPlayerOne->flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
            replayread_key_data(); // 1
            return;
        }
        kill_ghost(gPlayerTwo);
        kill_ghost(gPlayerThree);
    }
}

void key_data_control(void) {
    if (replay_flag == 1) {
        replay_data_control();
        return;
    }
    if (!ghost_write) {
        norm_data_control();
        return;
    }
    /* This only gets triggered when the previous if-statements are not met
       Seems like just for pausing */
    lost_ghost = 1;
}
