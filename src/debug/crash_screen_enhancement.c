#include <ultra64.h>
#include <macros.h>
#include <mk64.h>
#include <stdarg.h>
#include <string.h>
#include "../crash_screen.h"
#include "crash_screen_enhancement.h"

s32 _Printf(char* (*prout)(char*, const char*, size_t), char* dst, const char* fmt, va_list args);

u32 CrashScreenFont2[7 * 9 + 1] = {
    0x70871c30, 0x8988a250, 0x88808290, 0x88831c90, 0x888402f8, 0x88882210, 0x71cf9c10, 0xf9cf9c70, 0x8228a288,
    0xf200a288, 0x0bc11c78, 0x0a222208, 0x8a222288, 0x71c21c70, 0x23c738f8, 0x5228a480, 0x8a282280, 0x8bc822f0,
    0xfa282280, 0x8a28a480, 0x8bc738f8, 0xf9c89c08, 0x82288808, 0x82088808, 0xf2ef8808, 0x82288888, 0x82288888,
    0x81c89c70, 0x8a08a270, 0x920da288, 0xa20ab288, 0xc20aaa88, 0xa208a688, 0x9208a288, 0x8be8a270, 0xf1cf1cf8,
    0x8a28a220, 0x8a28a020, 0xf22f1c20, 0x82aa0220, 0x82492220, 0x81a89c20, 0x8a28a288, 0x8a28a288, 0x8a289488,
    0x8a2a8850, 0x894a9420, 0x894aa220, 0x70852220, 0xf8011000, 0x08020800, 0x10840400, 0x20040470, 0x40840400,
    0x80020800, 0xf8011000, 0x70800000, 0x88822200, 0x08820400, 0x108f8800, 0x20821000, 0x00022200, 0x20800020
};

extern u64 osClockRate;

u8 CrashScreenCharToGlyph[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, 41, -1, -1, -1, 43, -1, -1, 37, 38, -1, 42, -1, 39, 44, -1, 0,  1,  2,  3,
    4,  5,  6,  7,  8,  9,  36, -1, -1, -1, -1, 40, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
};

char* CauseDesc[18] = {
    "Interrupt",
    "TLB modification",
    "TLB exception on load",
    "TLB exception on store",
    "Address error on load",
    "Address error on store",
    "Bus error on inst.",
    "Bus error on data",
    "System call exception",
    "Breakpoint exception",
    "Reserved instruction",
    "Coprocessor unusable",
    "Arithmetic overflow",
    "Trap exception",
    "Virtual coherency on inst.",
    "Floating point exception",
    "Watchpoint exception",
    "Virtual coherency on data",
};

char* FpcsrDesc[6] = {
    "Unimplemented operation", "Invalid operation", "Division by zero", "Overflow", "Underflow", "Inexact operation",
};

void CrashScreenDrawGlyphEnhancement(s32 x, s32 y, s32 glyph) {
    const u32* data;
    u16* ptr;
    u32 bit;
    u32 rowMask;
    s32 i, j;

    data = &CrashScreenFont2[glyph / 5 * 7];
    ptr = PFramebuffer + SCREEN_WIDTH * y + x;

    for (i = 0; i < 7; i++) {
        bit = 0x80000000U >> ((glyph % 5) * 6);
        rowMask = *data++;

        for (j = 0; j < 6; j++) {
            *ptr++ = (bit & rowMask) ? 0xffff : 1;
            bit >>= 1;
        }
        ptr += SCREEN_WIDTH - 6;
    }
}

void CrashScreenDrawRect(s32 x, s32 y, s32 w, s32 h) {
    u16* ptr;
    s32 i, j;

    ptr = PFramebuffer + SCREEN_WIDTH * y + x;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            // 0xe738 = 0b1110011100111000
            *ptr = ((*ptr & 0xe738) >> 2) | 1;
            ptr++;
        }
        ptr += SCREEN_WIDTH - w;
    }
}

static char* write_to_buf(char* Buffer, const char* data, size_t Size) {
    return (char*) memcpy(Buffer, data, Size) + Size;
}

void CrashScreenPrint(s32 x, s32 y, const char* fmt, ...) {
    char* ptr;
    u32 glyph;
    s32 Size;
    char buf[0x100];

    va_list args;
    va_start(args, fmt);

    Size = _Printf(write_to_buf, buf, fmt, args);

    if (Size > 0) {
        ptr = buf;

        while (Size > 0) {

            glyph = CrashScreenCharToGlyph[*ptr & 0x7f];

            if (glyph != 0xff) {
                CrashScreenDrawGlyphEnhancement(x, y, glyph);
            }

            Size--;

            ptr++;
            x += 6;
        }
    }

    va_end(args);
}

void CrashScreenSleep(s32 ms) {
    u64 cycles = ms * 1000LL * osClockRate / 1000000ULL;
    osSetTime(0);
    while (osGetTime() < cycles) {}
}

void CrashScreenPrintFloatReg(s32 x, s32 y, s32 regNum, void* Addr) {
    u32 bits;
    s32 exponent;

    bits = *(u32*) Addr;
    exponent = ((bits & 0x7f800000U) >> 0x17) - 0x7f;
    if ((exponent >= -0x7e && exponent <= 0x7f) || bits == 0) {
        CrashScreenPrint(x, y, "F%02d:%.3e", regNum, *(f32*) Addr);
    } else {
        CrashScreenPrint(x, y, "F%02d:---------", regNum);
    }
}

void CrashScreenPrintFpcsr(u32 fpcsr) {
    s32 i;
    u32 bit;

    bit = 1 << 17;
    CrashScreenPrint(30, 155, "FPCSR:%08XH", fpcsr);
    for (i = 0; i < 6; i++) {
        if (fpcsr & bit) {
            CrashScreenPrint(132, 155, "(%s)", FpcsrDesc[i]);
            return;
        }
        bit >>= 1;
    }
}

void CrashScreenDraw(OSThread* thread) {
    s16 cause;
    __OSThreadContext* tc = &thread->context;

    cause = (tc->cause >> 2) & 0x1f;
    if (cause == 23) { // EXC_WATCH
        cause = 16;
    }
    if (cause == 31) { // EXC_VCED
        cause = 17;
    }

    osWritebackDCacheAll();

    CrashScreenDrawRect(25, 20, 270, 25);
    CrashScreenPrint(30, 25, "THREAD:%d  (%s)", thread->id, CauseDesc[cause]);
    CrashScreenPrint(30, 35, "PC:%08XH   SR:%08XH   VA:%08XH", tc->pc, tc->sr, tc->badvaddr);
    CrashScreenSleep(2000);
    CrashScreenDrawRect(25, 45, 270, 185);
    CrashScreenPrint(30, 50, "AT:%08XH   V0:%08XH   V1:%08XH", (u32) tc->at, (u32) tc->v0, (u32) tc->v1);
    CrashScreenPrint(30, 60, "A0:%08XH   A1:%08XH   A2:%08XH", (u32) tc->a0, (u32) tc->a1, (u32) tc->a2);
    CrashScreenPrint(30, 70, "A3:%08XH   T0:%08XH   T1:%08XH", (u32) tc->a3, (u32) tc->t0, (u32) tc->t1);
    CrashScreenPrint(30, 80, "T2:%08XH   T3:%08XH   T4:%08XH", (u32) tc->t2, (u32) tc->t3, (u32) tc->t4);
    CrashScreenPrint(30, 90, "T5:%08XH   T6:%08XH   T7:%08XH", (u32) tc->t5, (u32) tc->t6, (u32) tc->t7);
    CrashScreenPrint(30, 100, "S0:%08XH   S1:%08XH   S2:%08XH", (u32) tc->s0, (u32) tc->s1, (u32) tc->s2);
    CrashScreenPrint(30, 110, "S3:%08XH   S4:%08XH   S5:%08XH", (u32) tc->s3, (u32) tc->s4, (u32) tc->s5);
    CrashScreenPrint(30, 120, "S6:%08XH   S7:%08XH   T8:%08XH", (u32) tc->s6, (u32) tc->s7, (u32) tc->t8);
    CrashScreenPrint(30, 130, "T9:%08XH   GP:%08XH   SP:%08XH", (u32) tc->t9, (u32) tc->gp, (u32) tc->sp);
    CrashScreenPrint(30, 140, "S8:%08XH   RA:%08XH", (u32) tc->s8, (u32) tc->ra);
    CrashScreenPrintFpcsr(tc->fpcsr);
    CrashScreenPrintFloatReg(30, 170, 0, &tc->fp0.f.f_even);
    CrashScreenPrintFloatReg(120, 170, 2, &tc->fp2.f.f_even);
    CrashScreenPrintFloatReg(210, 170, 4, &tc->fp4.f.f_even);
    CrashScreenPrintFloatReg(30, 180, 6, &tc->fp6.f.f_even);
    CrashScreenPrintFloatReg(120, 180, 8, &tc->fp8.f.f_even);
    CrashScreenPrintFloatReg(210, 180, 10, &tc->fp10.f.f_even);
    CrashScreenPrintFloatReg(30, 190, 12, &tc->fp12.f.f_even);
    CrashScreenPrintFloatReg(120, 190, 14, &tc->fp14.f.f_even);
    CrashScreenPrintFloatReg(210, 190, 16, &tc->fp16.f.f_even);
    CrashScreenPrintFloatReg(30, 200, 18, &tc->fp18.f.f_even);
    CrashScreenPrintFloatReg(120, 200, 20, &tc->fp20.f.f_even);
    CrashScreenPrintFloatReg(210, 200, 22, &tc->fp22.f.f_even);
    CrashScreenPrintFloatReg(30, 210, 24, &tc->fp24.f.f_even);
    CrashScreenPrintFloatReg(120, 210, 26, &tc->fp26.f.f_even);
    CrashScreenPrintFloatReg(210, 210, 28, &tc->fp28.f.f_even);
    CrashScreenPrintFloatReg(30, 220, 30, &tc->fp30.f.f_even);
    osViBlack(false);
    osViSwapBuffer(PFramebuffer);
}
