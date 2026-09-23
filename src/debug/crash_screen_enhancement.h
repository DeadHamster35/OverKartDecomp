#ifndef CRASH_SCREEN_ENHANCEMENT_H
#define CRASH_SCREEN_ENHANCEMENT_H

#include <ultra64.h>
#include <macros.h>

void CrashScreenDraw(OSThread* thread);

// Add this to the top of Main.c or crash_screen.c
// #define CRASH_SCREEN_ENHANCEMENT

/**
 * Example of how to Force crash screen to run.
 * Make sure to include the header crash_screen_enhancement.h in Main.h
 */

// void FlushDisplaylist(void) {
//     SetGraphCPUTime(BEFORE_DISPLAY_LISTS);
//     osRecvMesg(&GfxVblankQueue, &MainReceivedMesg, OS_MESG_BLOCK);
//     StartGraphicTask(&DynamicP->SPTask);
//     SetGraphCPUTime(AFTER_DISPLAY_LISTS);
//     osRecvMesg(&GameVblankQueue, &MainReceivedMesg, OS_MESG_BLOCK);
//     osViSwapBuffer((void *) PHYSICAL_TO_VIRTUAL(CfbPtrs[DispFrame]));
//     SetGraphCPUTime(THREAD5_END);
//     osRecvMesg(&GameVblankQueue, &MainReceivedMesg, OS_MESG_BLOCK);
//     SetFaultFrameBuffer((uintptr_t *) CfbPtrs[DispFrame]);

//     CrashScreenDraw((u16 *) CfbPtrs[DispFrame], &GameLoopThread);
//     Add this line ^

//     if (++DispFrame == 3) {
//         DispFrame = 0;
//     }
//     if (++DrawFrame == 3) {
//         DrawFrame = 0;
//     }
//     FrameCounter++;
// }

#endif // CRASH_SCREEN_ENHANCEMENT_H
