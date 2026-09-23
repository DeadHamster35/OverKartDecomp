#include <ultra64.h>
#include <macros.h>
#include "profiler.h"
#include <mk64.h>
#include "main.h"

struct ProfilerFrameData ProfilerFrameData[2];

s32 DispTimeFlag = 0;

s16 D_800DC664 = 0;
s16 D_800DC668 = 1;
s16 D_800DC66C = 0;

void SetGraphCPUTime(enum ProfilerGameEvent eventID) {
    ProfilerFrameData[D_800DC668].GameTimes[eventID] = osGetTime();

    if (eventID == THREAD5_END) {
        D_800DC668 ^= 1;
        ProfilerFrameData[D_800DC668].NumSoundTimes = 0;
    }
}

void SetAudioCPUTime(void) {
    struct ProfilerFrameData* profiler = &ProfilerFrameData[D_800DC668];

    if (profiler->NumSoundTimes < ARRAY_COUNT(profiler->SoundTimes)) {
        profiler->SoundTimes[profiler->NumSoundTimes++] = osGetTime();
    }
}

void SetGraphRCPTime(enum ProfilerGfxEvent eventID) {
    if (eventID == TASKS_QUEUED) {
        D_800DC66C ^= 1;
        ProfilerFrameData[D_800DC66C].NumVblankTimes = 0;
    }
    ProfilerFrameData[D_800DC66C].GFXTimes[eventID] = osGetTime();
}

void SetAudioRCPTime(void) {
    struct ProfilerFrameData* profiler = &ProfilerFrameData[D_800DC66C];

    if (profiler->NumVblankTimes < ARRAY_COUNT(profiler->VblankTimes)) {
        profiler->VblankTimes[profiler->NumVblankTimes++] = osGetTime();
    }
}

// Draw the specified profiler given the information passed.
// This function is the same as the one in SM64
void DisplayTimeBar(OSTime clockBase, OSTime clockStart, OSTime clockEnd, s16 PosY, u16 color) {
    s64 durationStart, durationEnd;
    s32 rectX1, rectX2;

    // set the Duration to Start, and floor to 0 if the result is below 0.
    if ((durationStart = clockStart - clockBase) < 0) {
        durationStart = 0;
    }
    // like the above, but with end.
    if ((durationEnd = clockEnd - clockBase) < 0) {
        durationEnd = 0;
    }

    // calculate the x coordinates of where Start and end begins, respectively.
    rectX1 = ((((durationStart * 1000000) / osClockRate * 3) / 1000) + 30);
    rectX2 = ((((durationEnd * 1000000) / osClockRate * 3) / 1000) + 30);

    //! I believe this is supposed to cap rectX1 and rectX2 to 320, but the
    //  Code seems to use the wrong variables... it's possible that the variable
    //  Names were very similar within a single Letter.
    if (rectX1 > 319) {
        clockStart = 319;
    }
    if (rectX2 > 319) {
        clockEnd = 319;
    }

    // perform the render if Start is less than end. in most cases, it should be.
    if (rectX1 < rectX2) {
        gDPPipeSync(DisplayListHead++);
        gDPSetFillColor(DisplayListHead++, color << 16 | color);
        gDPFillRectangle(DisplayListHead++, rectX1, PosY, rectX2, PosY + 2);
    }
}

// Also the same function as SM64
void DisplayTimeScale(void) {
    // Draws the reference "max" bars underneath the real thing.

    // Blue
    gDPPipeSync(DisplayListHead++);
    gDPSetFillColor(DisplayListHead++, GPACK_RGBA5551(40, 80, 255, 1) << 16 | GPACK_RGBA5551(40, 80, 255, 1));
    gDPFillRectangle(DisplayListHead++, 30, 220, 79, 222);

    // Yellow
    gDPPipeSync(DisplayListHead++);
    gDPSetFillColor(DisplayListHead++, GPACK_RGBA5551(255, 255, 40, 1) << 16 | GPACK_RGBA5551(255, 255, 40, 1));
    gDPFillRectangle(DisplayListHead++, 79, 220, 128, 222);

    // Orange
    gDPPipeSync(DisplayListHead++);
    gDPSetFillColor(DisplayListHead++, GPACK_RGBA5551(255, 120, 40, 1) << 16 | GPACK_RGBA5551(255, 120, 40, 1));
    gDPFillRectangle(DisplayListHead++, 128, 220, 177, 222);

    // Red
    gDPPipeSync(DisplayListHead++);
    gDPSetFillColor(DisplayListHead++, GPACK_RGBA5551(255, 40, 40, 1) << 16 | GPACK_RGBA5551(255, 40, 40, 1));
    gDPFillRectangle(DisplayListHead++, 177, 220, 226, 222);
}

/**
 * Likely identical to sm64 but in mk64 they likely commented it out.
 */
void DrawProfilerMode1(void) {
}

/*
  Draw Profiler Mode 0. This Mode renders bars over each other to make it
  easier to see which processes take the longest.

  This function differs from the one in SM64 by two lines

  Information:

  (red): Sound Updates
  (yellow): Level Script Execution
  (orange): Rendering
  (orange): RDP Duration
  (yellow): RSP Duration
  (red): VBlank Duration
*/
void DisplayProcInterval(void) {
    s32 i;
    struct ProfilerFrameData* profiler;

    u64 clockStart;
    // Does this naming apply to MK64?
    u64 levelScriptDuration;
    u64 renderDuration;
    u64 taskStart;
    u64 rspDuration;
    u64 rdpDuration;
    u64 vblank;
    u64 soundDuration;

    // get the last frame profiler. gCurrentFrameIndex1 has the Current frame being processed, so
    // xor it to get the last frame profiler.
    profiler = &ProfilerFrameData[D_800DC668 ^ 1];

    // was thread 5 ran before thread 4? set the lower one to be the clockStart.
    clockStart = profiler->GameTimes[0] <= profiler->SoundTimes[0] ? profiler->GameTimes[0] : profiler->SoundTimes[0];

    // set variables for Duration of tasks.
    levelScriptDuration = profiler->GameTimes[1] - clockStart;
    renderDuration = profiler->GameTimes[2] - profiler->GameTimes[1];
    taskStart = 0;
    rspDuration = profiler->GFXTimes[1] - profiler->GFXTimes[0];
    rdpDuration = profiler->GFXTimes[2] - profiler->GFXTimes[0];
    vblank = 0;

    // like above functions, toss the odd bit.
    profiler->NumSoundTimes &= 0xFFFE;

    // Sound Duration seems to be rendered with empty space and not actually drawn.
    for (i = 0; i < profiler->NumSoundTimes; i += 2) {
        // calculate Sound Duration of max - min
        soundDuration = profiler->SoundTimes[i + 1] - profiler->SoundTimes[i];
        taskStart += soundDuration;
        // was the Sound Time minimum less than level script execution?
        if (profiler->SoundTimes[i] < profiler->GameTimes[1]) {
            // overlay the levelScriptDuration onto the profiler by subtracting the soundDuration.
            levelScriptDuration -= soundDuration;
        } else if (profiler->SoundTimes[i] < profiler->GameTimes[2]) {
            // overlay the renderDuration onto the profiler by subtracting the soundDuration.
            renderDuration -= soundDuration;
        }
    }

    // same as above. toss the odd bit.
    profiler->NumSoundTimes &= 0xFFFE;

    //! wrong Index Used to retrieve VblankTimes, thus empty pairs can
    //  potentially be passed to DisplayTimeBar, because it could be sending
    //  pairs that are beyond the NumVblankTimes enforced non-odd Limit, due to
    //  using the wrong num Value.
    for (i = 0; i < profiler->NumSoundTimes; i += 2) {
        vblank += (profiler->VblankTimes[i + 1] - profiler->VblankTimes[i]);
    }

    // Draw top profilers.

    // draw Sound Duration as the first bar. (red)
    clockStart = 0;
    DisplayTimeBar(0, clockStart, clockStart + taskStart, 212, GPACK_RGBA5551(255, 40, 40, 1));

    // draw level script execution Duration. (yellow)
    clockStart += taskStart;
    DisplayTimeBar(0, clockStart, clockStart + levelScriptDuration, 212, GPACK_RGBA5551(255, 255, 40, 1));

    // draw render Duration. (orange)
    clockStart += levelScriptDuration;
    DisplayTimeBar(0, clockStart, clockStart + renderDuration, 212, GPACK_RGBA5551(255, 120, 40, 1));

    // Two added lines in Mario Kart 64
    D_800DC568 = (s32) (clockStart + renderDuration);
    D_800DC56C[0] = rdpDuration;

    // Draw bottom profilers.

    // rdp Duration (orange)
    DisplayTimeBar(0, 0, rdpDuration, 216, GPACK_RGBA5551(255, 120, 40, 1));
    // rsp Duration (yellow)
    DisplayTimeBar(0, 0, rspDuration, 216, GPACK_RGBA5551(255, 255, 40, 1));
    // vblank Duration (red)
    DisplayTimeBar(0, 0, vblank, 216, GPACK_RGBA5551(255, 40, 40, 1));

    DisplayTimeScale();
}

// Similar to draw_screen_borders from SM64, with a hint of draw_profiler
void DisplayProcTime(void) {
    gDPPipeSync(DisplayListHead++);
    gDPSetScissor(DisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetRenderMode(DisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(DisplayListHead++, G_CYC_FILL);
    gDPSetFillColor(DisplayListHead++, GPACK_RGBA5551(0, 0, 0, 0) << 16 | GPACK_RGBA5551(0, 0, 0, 0));

    if ((ControllerOne->ButtonPressed & L_TRIG) != 0) {
        D_800DC664 ^= 1;
    }
    DisplayProcInterval();
}
