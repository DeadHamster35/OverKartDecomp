#include <libultraship/libultraship.h>

#define F3DEX_GBI 1
#define F3D_OLD 1
#define _LANGUAGE_C

extern "C" {

#undef gSPDisplayList
#undef gSPVertex
#undef gSPInvalidateTexCache

void gSPDisplayList(Gfx* pkt, Gfx* dl) {
    __gSPDisplayList(pkt, dl);
}

void gSPVertex(Gfx* pkt, uintptr_t v, int n, int v0) {
    __gSPVertex(pkt, v, n, v0);
}

void gSPInvalidateTexCache(Gfx* pkt, uintptr_t texAddr) {
    __gSPInvalidateTexCache(pkt, texAddr);
}

}
