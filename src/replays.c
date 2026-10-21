/*
 * @file Replays
 * Handles 3 types of Time trial replays:
 * 1. Post Time trial replays, which a Player can Watch after a Time trial
 * 2. Player ghosts, which were driven by a Player and can be raced against
 * 3. Staff ghosts, which can be unlocked on the raceway tracks with a fast enough Time
 * All 3 use the same system for storing / replaying the inputs to reproduce a Time trial
 * See ReplayreadKeyData for additional technical details
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
#include "OverKart5Hooks.h"

extern s32 encpass2(s32 Input, s32, s32);
extern s32 encpass1(void*, s32, s32);

u8* SReplayGhostBuffer;
s16 SReplayGhostBufferSize;
s16 D_80162D86;

static u16 sPlayerGhostButtonsPrev;
static u32 sPlayerGhostFramesRemaining;
static s16 sPlayerGhostReplayIdx;
static u32* sPlayerGhostReplay;

static u16 sButtonsPrevCourseGhost;
u32 sCourseGhostFramesRemaining;
s16 sCourseGhostReplayIdx;
u32* sCourseGhostReplay;

static u16 sPostTTButtonsPrev;
static s32 sPostTTFramesRemaining;
static s16 sPostTTReplayIdx;
static u32* sPostTTReplay;

static s16 sPlayerInputIdx;
static u32* sPlayerInputs;

void SaveStateGetReplayCursor(SaveStateReplayCursor *out) {
    out->playerButtonsPrev = sPlayerGhostButtonsPrev;
    out->playerFramesRemaining = sPlayerGhostFramesRemaining;
    out->playerReplayIdx = sPlayerGhostReplayIdx;
    out->courseButtonsPrev = sButtonsPrevCourseGhost;
    out->courseFramesRemaining = sCourseGhostFramesRemaining;
    out->courseReplayIdx = sCourseGhostReplayIdx;
    out->postTTButtonsPrev = sPostTTButtonsPrev;
    out->postTTFramesRemaining = sPostTTFramesRemaining;
    out->postTTReplayIdx = sPostTTReplayIdx;
}

void SaveStateSetReplayCursor(SaveStateReplayCursor *in) {
    sPlayerGhostButtonsPrev = in->playerButtonsPrev;
    sPlayerGhostFramesRemaining = in->playerFramesRemaining;
    sPlayerGhostReplayIdx = in->playerReplayIdx;
    sButtonsPrevCourseGhost = in->courseButtonsPrev;
    sCourseGhostFramesRemaining = in->courseFramesRemaining;
    sCourseGhostReplayIdx = in->courseReplayIdx;
    sPostTTButtonsPrev = in->postTTButtonsPrev;
    sPostTTFramesRemaining = in->postTTFramesRemaining;
    sPostTTReplayIdx = in->postTTReplayIdx;
}

static u16 sPrevCourseId;
u32 Map;
s32 ReadPointer;
s32 WritePointer;
s32 ReplayPointer;
u16 GhostStatus;
u16 RomghostStatus;
u16 ReplayghostStatus;
s32 GhostSuccess;
s32 GhostKart; // ghost Kart id?
s32 RomghostKart;
s32 ReplayghostKart;
UNUSED static s32 sUnusedReplayCounter;
s32 GhostWrite;
UNUSED static s32 bUnusedCourseGhostDisabled;
s32 LostGhost;
s32 D_80162DFC;

s32 D_80162E00;

u32* Keystock1 = (u32*) &D_802BFB80.arraySize8[0][2][3];
u32* Keystock2 = (u32*) &D_802BFB80.arraySize8[1][1][3];

extern s32 LapCountByPlayerId[];

extern StaffGhost* d_mario_raceway_staff_ghost;
extern StaffGhost* d_royal_raceway_staff_ghost;
extern StaffGhost* d_luigi_raceway_staff_ghost;

void DMAROMGhost(void) {
    sCourseGhostReplay = (u32*) &D_802BFB80.arraySize8[0][2][3];
    osInvalDCache(&sCourseGhostReplay[0], 0x4000);
    osPiStartDma(&DmaIoMesg, 0, 0, (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(Map)],
                 sCourseGhostReplay, 0x4000, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    sCourseGhostFramesRemaining = (*sCourseGhostReplay & REPLAY_FRAME_COUNTER);
    sCourseGhostReplayIdx = 0;
}

void ReplayGhost(void) {
    sPostTTReplay = (u32*) &D_802BFB80.arraySize8[0][ReplayPointer][3];
    sPostTTFramesRemaining = *sPostTTReplay & REPLAY_FRAME_COUNTER;
    sPostTTReplayIdx = 0;
}

void ReadGhost(void) {
    sPlayerGhostReplay = (u32*) &D_802BFB80.arraySize8[0][ReadPointer][3];
    sPlayerGhostFramesRemaining = (s32) *sPlayerGhostReplay & REPLAY_FRAME_COUNTER;
    sPlayerGhostReplayIdx = 0;
}
/**
 * Activates staff ghost if Time trial lap Time is low enough
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

void CheckRomghost(void) {
    u32 bestTime; // Appears to be best Player 3lap Time.
#if !ENABLE_CUSTOM_COURSE_ENGINE
    switch (CourseID) {
        case COURSE_MARIO_RACEWAY:
            bestTime = GetRecordTime2(0) & 0xfffff;
            if (bestTime <= GHOST_UNLOCK_MARIO) {
                RomghostStatus = 0;
                bUnusedCourseGhostDisabled = 0;
            } else {
                RomghostStatus = 1;
                bUnusedCourseGhostDisabled = 1;
            }
            Map = (u32) &d_mario_raceway_staff_ghost;
            RomghostKart = 0;
            break;
        case COURSE_ROYAL_RACEWAY:
            bestTime = GetRecordTime2(0) & 0xfffff;
            if (bestTime <= GHOST_UNLOCK_ROYAL) {
                RomghostStatus = 0;
                bUnusedCourseGhostDisabled = 0;
            } else {
                RomghostStatus = 1;
                bUnusedCourseGhostDisabled = 1;
            }
            Map = (u32) &d_royal_raceway_staff_ghost;
            RomghostKart = 6;
            break;
        case COURSE_LUIGI_RACEWAY:
            bestTime = GetRecordTime2(0) & 0xfffff;
            if (bestTime <= GHOST_UNLOCK_LUIGI) {
                RomghostStatus = 0;
                bUnusedCourseGhostDisabled = 0;
            } else {
                RomghostStatus = 1;
                bUnusedCourseGhostDisabled = 1;
            }
            Map = (u32) &d_luigi_raceway_staff_ghost;
            RomghostKart = 1;
            break;
        default:
            RomghostStatus = 1;
            bUnusedCourseGhostDisabled = 1;
    }
#else

#endif
}

s32 MakePressGhost(void) {
    s32 phi_v0;

    if (SReplayGhostBufferSize != 0) {
        // encpass1 in mio0_decode.s
        encpass1((void*) SReplayGhostBuffer, (SReplayGhostBufferSize * 4) + 0x20, (s32) Keystock1);
        phi_v0 =
            encpass2((s32) Keystock1, (SReplayGhostBufferSize * 4) + 0x20, (s32) Keystock2);
        return phi_v0 + 0x1e;
    }
}

void MakeMeltGhost(void) {
    sPlayerGhostReplay = (u32*) &D_802BFB80.arraySize8[0][ReadPointer][3];
    decodeMIO0((u8*) Keystock2, (u8*) sPlayerGhostReplay);
    sPlayerGhostFramesRemaining = (s32) (*sPlayerGhostReplay & REPLAY_FRAME_COUNTER);
    sPlayerGhostReplayIdx = 0;
    D_80162E00 = 1;
}

void SwapGhostbuffer(void) {
    s16 temp_v0;

    if (ReadPointer == 1) {
        ReadPointer = 0;
        WritePointer = 1;
    } else {
        ReadPointer = 1;
        WritePointer = 0;
    }
    temp_v0 = sPlayerInputIdx;
    SReplayGhostBuffer = (void*) &D_802BFB80.arraySize8[0][ReadPointer][3];
    SReplayGhostBufferSize = temp_v0;
    D_80162D86 = temp_v0;
}

void InitialGhost(void) {

    if (GameMode == TIME_TRIALS) {

        CheckRomghostDefault();

        if (sPrevCourseId != CourseID) {
            GhostStatus = 1;
        }

        sPrevCourseId = (u16) CourseID;
        GhostWrite = 0;
        sUnusedReplayCounter = 0;
        LostGhost = 0;

        if (GameMode == TIME_TRIALS && ScreenSplitA == SCREEN_MODE_1P) {

            if (ReplayFlag == 1) {
                ReplayGhost();
                if (ReplayghostStatus == 0) {
                    ReadGhost();
                }
                if (RomghostStatus == 0) {
                    DMAROMGhostDefault();
                }
            } else {

                ReplayghostStatus = 1U;
                sPlayerInputs = (u32*) &D_802BFB80.arraySize8[0][WritePointer][3];
                sPlayerInputs[0] = -1;
                sPlayerInputIdx = 0;
                GhostSuccess = 0;
                InitGhostData();
                if (GhostStatus == 0) {
                    ReadGhost();
                }
                if (RomghostStatus == 0) {
                    DMAROMGhostDefault();
                }
            }
        }
    }
    SetGhostData();
}

/* Special handling for buttons saved in replays. The listing of L_TRIG here is odd
 * because it is not saved in the replay data structure. Possibly, L was initially deleted
 * here to make way for the frame Counter, but then the format changed when the stick
 * coordinates were added */
#define REPLAY_MASK (ALL_BUTTONS ^ (A_BUTTON | B_BUTTON | Z_TRIG | R_TRIG | L_TRIG))

/* Inputs for replays (including Player and course ghosts) are saved in a s32[] where
   each entry is a combination of the inputs and  how long those inputs were held for.
   In essence it's "These buttons were pressed and the joystick was in this Position.
   This was the case for X frames".

   bits 1-8: Stick X
   bits 9-16: Stick Y
   bits 17-24: Frame Counter
   bits 25-28: Unused
   bit 29: R
   bit 30: Z
   bit 31: B
   bit 32: A
*/
void ReplayreadKeyData(void) {
    u32 inputs;
    u32 stickBytes;
    UNUSED u16 unk;
    u16 buttons_temp;
    s16 stickVal;
    s16 buttons = 0;

    if (sPostTTReplayIdx >= 0x1000) {
        PlayerOne->Flag = IS_RACE_FINISH | NO_CONTROLS | IS_CPU_PLAYER;
        return;
    }

    inputs = sPostTTReplay[sPostTTReplayIdx];
    stickBytes = inputs & REPLAY_STICK_X;

    // twos complement trick, converting singned 8-bit Value to signed 16 bit
    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }

    stickBytes = (u32) (inputs & REPLAY_STICK_Y) >> 8;
    ControllerEight->AnalogX = stickVal;

    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }
    ControllerEight->AnalogY = stickVal;
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
    buttons_temp = ControllerEight->ButtonPressed & REPLAY_MASK;
    ControllerEight->ButtonPressed = (buttons & (buttons ^ sPostTTButtonsPrev)) | buttons_temp;
    buttons_temp = ControllerEight->ButtonReleased & REPLAY_MASK;
    ControllerEight->ButtonReleased = (sPostTTButtonsPrev & (buttons ^ sPostTTButtonsPrev)) | buttons_temp;
    sPostTTButtonsPrev = buttons;
    ControllerEight->ButtonHeld = buttons;

    if (sPostTTFramesRemaining == 0) {
        sPostTTReplayIdx++;
        sPostTTFramesRemaining = (s32) (sPostTTReplay[sPostTTReplayIdx] & REPLAY_FRAME_COUNTER);
    } else {
        sPostTTFramesRemaining -= REPLAY_FRAME_INCREMENT;
    }
}

// See ReplayreadKeyData comment
void RomreadKeyData(void) {
    u32 inputs;
    u32 stickBytes;
    UNUSED u16 unk;
    u16 buttonsTemp;
    s16 stickVal;
    s16 buttons = 0;
    if (sCourseGhostReplayIdx >= 0x1000) {
        KillGhost(PlayerThree);
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
    ControllerSeven->AnalogX = stickVal;

    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }
    ControllerSeven->AnalogY = stickVal;

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
    buttonsTemp = ControllerSeven->ButtonPressed & REPLAY_MASK;
    ControllerSeven->ButtonPressed = (buttons & (buttons ^ sButtonsPrevCourseGhost)) | buttonsTemp;
    buttonsTemp = ControllerSeven->ButtonReleased & REPLAY_MASK;
    ControllerSeven->ButtonReleased = (sButtonsPrevCourseGhost & (buttons ^ sButtonsPrevCourseGhost)) | buttonsTemp;
    sButtonsPrevCourseGhost = buttons;
    ControllerSeven->ButtonHeld = buttons;
    if (sCourseGhostFramesRemaining == 0) {
        sCourseGhostReplayIdx++;
        sCourseGhostFramesRemaining = (s32) (sCourseGhostReplay[sCourseGhostReplayIdx] & REPLAY_FRAME_COUNTER);
    } else {
        sCourseGhostFramesRemaining -= (s32) REPLAY_FRAME_INCREMENT;
    }
}

// See ReplayreadKeyData comment
void ReadKeyData(void) {
    u32 inputs;
    u32 stickBytes;
    UNUSED u16 unk;
    u16 buttons_temp;
    s16 stickVal;
    s16 buttons = 0;

    if (sPlayerGhostReplayIdx >= 0x1000) {
        KillGhost(PlayerTwo);
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

    ControllerSix->AnalogX = stickVal;

    if (stickBytes < 0x80U) {
        stickVal = (s16) (stickBytes & 0xFF);
    } else {
        stickVal = (s16) (stickBytes | (~0xFF));
    }

    ControllerSix->AnalogY = stickVal;

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
    buttons_temp = ControllerSix->ButtonPressed & REPLAY_MASK;
    ControllerSix->ButtonPressed = (buttons & (buttons ^ sPlayerGhostButtonsPrev)) | buttons_temp;

    buttons_temp = ControllerSix->ButtonReleased & REPLAY_MASK;
    ControllerSix->ButtonReleased = (sPlayerGhostButtonsPrev & (buttons ^ sPlayerGhostButtonsPrev)) | buttons_temp;
    sPlayerGhostButtonsPrev = buttons;
    ControllerSix->ButtonHeld = buttons;

    if (sPlayerGhostFramesRemaining == 0) {
        sPlayerGhostReplayIdx++;
        sPlayerGhostFramesRemaining = (s32) (sPlayerGhostReplay[sPlayerGhostReplayIdx] & REPLAY_FRAME_COUNTER);
    } else {
        sPlayerGhostFramesRemaining -= (s32) REPLAY_FRAME_INCREMENT;
    }
}

// See ReplayreadKeyData comment
void WriteKeyData(void) {
    s16 buttons;
    u32 inputs;
    u32 StickX;
    u32 StickY;
    u32 inputCounter;
    u32 prevInputsWCounter;
    u32 prevInputs;
    /* Input file is too long or picked up by lakitu or Out of bounds
    Not sure if there is any way to be considered out of bounds without lakitu getting called */
    if (((sPlayerInputIdx >= 0x1000) || ((PlayerOne->JugemuFlag & ON_LAKITU_ROD) != 0)) ||
        ((PlayerOne->JugemuFlag & LAKITU_SCENE) != 0)) {
        LostGhost = 1;
        return;
    }

    StickX = ControllerOne->AnalogX;
    StickX &= 0xFF;
    StickY = ControllerOne->AnalogY;
    StickY = (StickY & 0xFF) << 8;
    buttons = ControllerOne->ButtonHeld;
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
    inputs |= StickX;
    inputs |= StickY;
    prevInputsWCounter = sPlayerInputs[sPlayerInputIdx];
    /* The 5th and 6th bytes from the Right are counters. Instead of saving the same inputs over and over,
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
            // increment Counter by 1
            prevInputsWCounter += REPLAY_FRAME_INCREMENT;
            sPlayerInputs[sPlayerInputIdx] = prevInputsWCounter;
        }
    } else {
        sPlayerInputIdx++;
        sPlayerInputs[sPlayerInputIdx] = inputs;
    }
}

// sets Player to AI? (unconfirmed)
void KillGhost(Player* ply) {
    if (((ply->Flag & IS_GHOST) != 0) && (ply != PlayerOne)) {
        ply->Flag = IS_RACE_FINISH | NO_CONTROLS | IS_CPU_PLAYER;
    }
}

void NormDataControl(void) {
    if (GameMode == TIME_TRIALS) {
        if ((LapCountByPlayerId[0] == 3) && (GhostSuccess == 0) && (LostGhost != 1)) {
            if (GhostStatus == 1) {
                ReplayPointer = WritePointer;
                SwapGhostbuffer();
                GhostStatus = 0;
                GhostSuccess = 1;
                GhostKart = PlayerOne->Kart;
                ReplayghostKart = PlayerOne->Kart;
                D_80162E00 = 0;
                D_80162DFC = PlayerHUD[PLAYER_ONE].Totaltime;
                KillGhost(PlayerTwo);
                KillGhost(PlayerThree);
            } else if (LapCountByPlayerId[1] != 3) {
                ReplayPointer = WritePointer;
                SwapGhostbuffer();
                GhostSuccess = 1;
                GhostKart = PlayerOne->Kart;
                D_80162DFC = PlayerHUD[PLAYER_ONE].Totaltime;
                D_80162E00 = 0;
                ReplayghostKart = PlayerOne->Kart;
                KillGhost(PlayerTwo);
                KillGhost(PlayerThree);
            } else {
                SReplayGhostBuffer = D_802BFB80.arraySize8[0][ReadPointer][3].PixelIndexArray;
                SReplayGhostBufferSize = D_80162D86;
                ReplayPointer = WritePointer;
                ReplayghostKart = PlayerOne->Kart;
                ReplayghostStatus = 0;
                GhostStatus = 0;
                GhostSuccess = 1;
                KillGhost(PlayerTwo);
                KillGhost(PlayerThree);
            }
        } else {
            if ((LapCountByPlayerId[0] == 3) && (GhostSuccess == 0) && (LostGhost == 1)) {
                SReplayGhostBuffer = D_802BFB80.arraySize8[0][ReadPointer][3].PixelIndexArray;
                SReplayGhostBufferSize = D_80162D86;
                GhostSuccess = 1;
            }
            if ((PlayerOne->Flag & IS_RACE_FINISH) == IS_RACE_FINISH) {
                KillGhost(PlayerTwo);
                KillGhost(PlayerThree);
            } else {
                sUnusedReplayCounter += 1;
                if (sUnusedReplayCounter > 100) {
                    sUnusedReplayCounter = 100;
                }
                if ((GameMode == TIME_TRIALS) && (ScreenSplitA == SCREEN_MODE_1P)) {
                    if ((GhostStatus == 0) && (LapCountByPlayerId[1] != 3)) {
                        ReadKeyData();
                    }
                    if ((RomghostStatus == 0) && (LapCountByPlayerId[2] != 3)) {
                        RomreadKeyData();
                    }
                    if (!(PlayerOne->Flag & IS_RACE_FINISH)) {
                        WriteKeyData();
                    }
                }
            }
        }
    }
}

void ReplayDataControl(void) {
    if ((GameMode == TIME_TRIALS) && (GameMode == TIME_TRIALS) && (ScreenSplitA == SCREEN_MODE_1P)) {
        if ((ReplayghostStatus == 0) && (LapCountByPlayerId[1] != 3)) {
            ReadKeyData(); // 3
        }
        if ((RomghostStatus == 0) && (LapCountByPlayerId[2] != 3)) {
            RomreadKeyData(); // 2
        }
        if ((PlayerOne->Flag & IS_RACE_FINISH) != IS_RACE_FINISH) {
            ReplayreadKeyData(); // 1
            return;
        }
        KillGhost(PlayerTwo);
        KillGhost(PlayerThree);
    }
}

void KeyDataControl(void) {
    if (ReplayFlag == 1) {
        ReplayDataControl();
        return;
    }
    if (!GhostWrite) {
        NormDataControl();
        return;
    }
    /* This only gets triggered when the previous if-statements are not met
       Seems like just for pausing */
    LostGhost = 1;
}
