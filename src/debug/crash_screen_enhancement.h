#ifndef CRASH_SCREEN_ENHANCEMENT_H
#define CRASH_SCREEN_ENHANCEMENT_H

#include <ultra64.h>
#include <macros.h>

void crash_screen_draw(OSThread* thread);

// Add this to the top of main.c or crash_screen.c
// #define CRASH_SCREEN_ENHANCEMENT

/**
 * Example of how to force crash screen to run.
 * Make sure to include the header crash_screen_enhancement.h in main.h
 */

// void flush_displaylist(void) {
//     SetGraphCPUTime(BEFORE_DISPLAY_LISTS);
//     osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
//     start_graphic_task(&gDynamicP->spTask);
//     SetGraphCPUTime(AFTER_DISPLAY_LISTS);
//     osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
//     osViSwapBuffer((void *) PHYSICAL_TO_VIRTUAL(g_CfbPtrs[g_DispFrame]));
//     SetGraphCPUTime(THREAD5_END);
//     osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
//     SetFaultFrameBuffer((uintptr_t *) g_CfbPtrs[g_DispFrame]);

//     crash_screen_draw((u16 *) g_CfbPtrs[g_DispFrame], &gGameLoopThread);
//     Add this line ^

//     if (++g_DispFrame == 3) {
//         g_DispFrame = 0;
//     }
//     if (++draw_frame == 3) {
//         draw_frame = 0;
//     }
//     frame_counter++;
// }

#endif // CRASH_SCREEN_ENHANCEMENT_H
