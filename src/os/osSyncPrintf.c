#include "libultra_internal.h"
#include "libc/stdarg.h"
#include "printf.h"

/* DEBUG=1: is_debug.c is linked (mk64.ld #if DEBUG) and provides these.
 * DEBUG=0: is_debug.o is not in the ELF; these stubs keep rmonPrintf callers linked. */
#ifndef DEBUG

char* osSyncPrintf(UNUSED char* arg0, UNUSED const char* arg1, UNUSED size_t size) {
    return (char*) (1);
}

void rmonPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    _Printf(osSyncPrintf, NULL, fmt, args);
    va_end(args);
}

#endif /* DEBUG */
