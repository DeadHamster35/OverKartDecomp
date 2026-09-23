#ifndef PROFILER_H
#define PROFILER_H

extern u64 osClockRate;

struct ProfilerFrameData {
    /* 0x00 */ s16 NumSoundTimes;
    /* 0x02 */ s16 NumVblankTimes;
    // GameTimes:
    // 0: thread 5 Start
    // 1: level script execution
    // 2: render
    // 3: display lists
    // 4: thread 4 end (0 terminated)
    /* 0x08 */ OSTime GameTimes[5];
    // GFXTimes:
    // 0: processors queued
    // 1: rsp completed
    // 2: rdp completed
    /* 0x30 */ OSTime GFXTimes[3];
    /* 0x48 */ OSTime SoundTimes[8];
    /* 0x88 */ OSTime VblankTimes[8];
};

// thread event IDs
enum ProfilerGameEvent { THREAD5_START, LEVEL_SCRIPT_EXECUTE, BEFORE_DISPLAY_LISTS, AFTER_DISPLAY_LISTS, THREAD5_END };

enum ProfilerGfxEvent { TASKS_QUEUED, RSP_COMPLETE, RDP_COMPLETE };

void SetGraphCPUTime(enum ProfilerGameEvent eventID);
void SetAudioCPUTime(void);
void SetGraphRCPTime(enum ProfilerGfxEvent eventID);
void SetAudioRCPTime(void);
void draw_profiler(void);
void DisplayProcTime(void);

extern s32 DispTimeFlag;

#endif /* PROFILER_H */
