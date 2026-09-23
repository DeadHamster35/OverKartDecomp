#include <ultra64.h>
#include <macros.h>
#include <mk64.h>
#include <stdarg.h>
#include <string.h>

#include "crash_screen.h"
#include "main.h"
#include "debug/crash_screen_enhancement.h"

OSThread D_80162790;
ALIGNED8 u8 DebugThreadStack[0x400];
OSMesgQueue D_80162D40;
OSMesg D_80162D58;
u16* PFramebuffer;
s32 SButtonSequenceIndex;

#define DRAW_CODE 0xFFFF
#define CHARACTER_DASH 16

extern void osSetEventMesg(OSEvent, OSMesgQueue*, OSMesg);
extern s32 osRecvMesg(OSMesgQueue*, OSMesg*, s32);
extern u8 Crash[];
extern u8 CrashEnd[];

s32 SCounter = 0;

u8 CrashScreenFont[][8] = {
#include "textures/crash_screen/crash_screen_font.ia1.inc.c"
};

u16 SCrashScreenButtonSequence[] = { L_TRIG, U_JPAD, L_JPAD,   D_JPAD,   R_JPAD,
                                     R_TRIG, L_TRIG, B_BUTTON, A_BUTTON, DRAW_CODE };

void DrawErrorNum(u16* framebuffer, s32 x, s32 y, s32 glyph) {
    s32 data;
    s32 ptr;
    s32 i, j;

    for (i = 0; i < 8; i++) {

        data = CrashScreenFont[glyph][i];

        for (j = 5; j >= 0; j--) {

            ptr = (y + i) * 320 + (x + j);

            if (data & 1) {
                framebuffer[ptr] = 0xffff;
            }
            data = data >> 1;
        }
    }
}

void DrawErrorBox(u16* framebuffer);

#define WHITE_COLOUR 0xFFFF
#define RED_COLOUR 0xF801

// (x,y) of top Left pixel of square
#define SQUARE_X 40
#define SQUARE_Y 40

#define SQUARE_SIZE_X 6
#define SQUARE_SIZE_Y 6

// Width of the square's border being drawn.
#define BORDER_WIDTH 1

#define SQUARE_X2 SQUARE_X + SQUARE_SIZE_Y
#define SQUARE_Y2 SQUARE_Y + SQUARE_SIZE_X

// Here's to you, Yoshimoto, for writing this stupid function. 3 years. 3 years to match.
void DrawErrorBox(u16* framebuffer) {
    s32 h;
    s32 i;
    s32 j;

    for (h = 0; h < 2; h++) {
        for (i = (h * BORDER_WIDTH) + SQUARE_Y; i < (SQUARE_Y2 - (h * BORDER_WIDTH)); i++) {
            for (j = (h * BORDER_WIDTH) + SQUARE_X; j < (SQUARE_X2 - (h * BORDER_WIDTH)); j++) {
                framebuffer[(i * 320) + j] = (h == 0) ? (RED_COLOUR) : (WHITE_COLOUR);
            }
        }
    }

    osWritebackDCacheAll();
    osViSwapBuffer(framebuffer);
}

/**
 * Draws three black boxes then prints crash info in the following format:
 * Line 1: threadId - Address of faulted instruction - error Code
 * Line 2: Address in the return Address register
 * Line 3: Machine Code of faulted instruction
 *
 * The R4300i manual discusses exceptions in more Depth.
 *
 * @Param framebuffer
 * @Param faulted thread
 **/

//                     0xRGBA
#define BLACK_COLOUR 0x0001

void CrashScreenDrawInfo(u16* framebuffer, OSThread* thread) {
    __OSThreadContext* context = &thread->context;
    s32 i, j, x, y, h;
    uintptr_t faultedAddr;
    u32 exception;
    s32 math;
    u32 crashInfo;
    s32 temp;

    // Draw three black boxes
    for (h = 0; h < 3; h++) {

        math = (48 + (SCounter * 24)) + h * 20;

        for (i = 0; i < 16; i++) {
            for (j = 0; j < 120; j++) {
                // Set pixels to black
                framebuffer[((i + math) * 320) + (j + 100)] = BLACK_COLOUR;
            }
        }
    }

    // Draw crash information over the black boxes.
    temp = (SCounter * 24);
    y = temp + 53;
    DrawErrorNum(framebuffer, 108, y, thread->id & 0xF);
    DrawErrorNum(framebuffer, 116, y, CHARACTER_DASH);

    // Address of faulted instruction.
    crashInfo = context->pc;

    for (x = 180; x >= 124; x -= 8) {
        DrawErrorNum(framebuffer, x, y, crashInfo & 0xF);
        crashInfo >>= 4;
    }

    exception = (context->cause >> 2) & 0x1F;
    DrawErrorNum(framebuffer, 188, y, CHARACTER_DASH);
    DrawErrorNum(framebuffer, 196, y, exception >> 4);
    DrawErrorNum(framebuffer, 204, y, exception & 0xF);

    // Address in the Return Address register.
    crashInfo = context->ra;

    for (x = 180; x >= 124; x -= 8) {
        DrawErrorNum(framebuffer, x, 73, crashInfo & 0xF);
        crashInfo >>= 4;
    }

    // Address of faulted instruction.
    faultedAddr = context->pc & (~3);

    // Ensure the Address to the faulted instruction is a memory Address.
    //! @bug if this check fails, the ra register is printed a second Time.
    if ((faultedAddr > 0x80000000) && (faultedAddr < 0x803FFF7F)) {
        // Cast the Address to its Value; the faulty machine Code.
        crashInfo = *(u32*) faultedAddr;
    }
    for (x = 180; x >= 124; x -= 4) {
        DrawErrorNum(framebuffer, x, 93, crashInfo & 0xF);
        x -= 4;
        crashInfo >>= 4;
    }
    osWritebackDCacheAll();
    osViSwapBuffer(framebuffer);
}

OSThread* FindFaultedThreads(void) {
    OSThread* thread;

    thread = __osGetCurrFaultedThread();
    while (thread->priority != -1) {
        if (thread->priority > OS_PRIORITY_IDLE && thread->priority <= OS_PRIORITY_APPMAX && (thread->flags & 3) != 0) {
            return thread;
        }
        thread = thread->tlnext;
    }
    return NULL;
}

/**
 * Retrieves the faulted thread and draws the decomp enhanced crash dump immediately.
 * Stock required a Button sequence and only the small Hex boxes.
 **/
void FaultProcess(UNUSED void* arg0) {
    static OSThread* thread;
    OSMesg mesg;

    osSetEventMesg(12, &D_80162D40, (OSMesg) 16);
    osSetEventMesg(10, &D_80162D40, (OSMesg) 16);
    SButtonSequenceIndex = 0;

    while (true) {
        osRecvMesg(&D_80162D40, &mesg, 1);
        thread = FindFaultedThreads();

        if (thread) {
            if (SCounter == 0) {
                if (PFramebuffer != NULL) {
                    DMA((u8*) PFramebuffer, Crash, (size_t) (CrashEnd - Crash));
                }
                CrashScreenDraw(thread);
            }
            if (SCounter < 5) {
                SCounter++;
            }
        }
    }
}

void SetFaultFrameBuffer(u16* framebuffer) {
    PFramebuffer = framebuffer;
}

extern void FaultProcess(void*);

void CreateDebugThread(void) {
    osCreateMesgQueue(&D_80162D40, &D_80162D58, 1);
    osCreateThread((OSThread*) &D_80162790, 9, (void*) FaultProcess, 0, &D_80162D40, 0x7F);
}

void StartDebugThread(void) {
    osStartThread(&D_80162790);
}
