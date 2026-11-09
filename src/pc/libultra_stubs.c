#include <ultra64.h>
#include <string.h>
#include "rom.h"

void bzero(void* s, size_t n) {
    memset(s, 0, n);
}

void bcopy(const void* src, void* dst, size_t n) {
    memmove(dst, src, n);
}

/* N64 OS pieces LUS does not provide for this decomp. Threads are cooperative:
 * Game.cpp drives GameprocFrame instead of osStartThread. */

void osInitialize(void) {
}

void osCreateThread(OSThread* thread, OSId id, void (*entry)(void*), void* arg, void* sp, OSPri pri) {
    (void) thread;
    (void) id;
    (void) entry;
    (void) arg;
    (void) sp;
    (void) pri;
}

void osStartThread(OSThread* thread) {
    (void) thread;
}

void osSetThreadPri(OSThread* thread, OSPri pri) {
    (void) thread;
    (void) pri;
}

OSPri osGetThreadPri(OSThread* thread) {
    (void) thread;
    return 0;
}

void osYieldThread(void) {
}

void osDestroyThread(OSThread* thread) {
    (void) thread;
}

void osSpTaskLoad(OSTask* task) {
    (void) task;
}

void osSpTaskStartGo(OSTask* task) {
    (void) task;
}

OSYieldResult osSpTaskYielded(OSTask* task) {
    (void) task;
    return 0;
}

void osSpTaskYield(void) {
}

s32 osAfterPreNMI(void) {
    return 0;
}

void osUnmapTLBAll(void) {
}

s32 osTvType = 1;
u32 osMemSize = 0x2000000;

/* Replaces LUS osPiStartDma (memcpy from a host pointer). ROM offsets and
 * N64 cart addresses are copied from the loaded baserom; 0x80xxxxxx is not
 * used as a pointer. */
s32 osPiStartDma(OSIoMesg* mb, s32 priority, s32 direction, uintptr_t devAddr, void* vAddr, size_t nbytes,
                 OSMesgQueue* mq) {
    const uint8_t* src = NULL;

    (void) priority;
    (void) direction;
    PcRomResolveSrc(devAddr, &src, nbytes);
    if (vAddr != NULL && nbytes != 0) {
        if (src != NULL) {
            memcpy(vAddr, src, nbytes);
        } else {
            memset(vAddr, 0, nbytes);
        }
    }
    if (mb != NULL) {
        mb->dramAddr = vAddr;
        mb->devAddr = devAddr;
        mb->size = nbytes;
        mb->hdr.retQueue = mq;
    }
    if (mq != NULL) {
        osSendMesg(mq, (OSMesg) mb, OS_MESG_NOBLOCK);
    }
    return 0;
}
