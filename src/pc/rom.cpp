#include "rom.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" {
extern uint8_t _common_texturesSegmentRomStart[];
extern uint8_t _trigTablesSegmentRomStart[];
extern uint8_t _data_segment2SegmentRomStart[];
extern uint8_t _endingSegmentRomStart[];
extern uint8_t _racingSegmentRomStart[];
extern uint8_t _overkartSegmentRomStart[];
extern uint8_t _au_displaylistsSegmentRomStart[];
extern uint8_t _startupLogoSegmentRomStart[];
extern uint8_t _ceremonyDataSegmentRomStart[];
extern int ok_HeaderROM;
}

static std::vector<uint8_t> sRom;

int PcRomLoad(const char* path) {
    FILE* f = fopen(path, "rb");
    if (f == NULL) {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) {
        fclose(f);
        return 0;
    }
    sRom.resize((size_t) sz);
    size_t n = fread(sRom.data(), 1, (size_t) sz, f);
    fclose(f);
    return n == (size_t) sz;
}

const uint8_t* PcRomData(void) {
    return sRom.empty() ? NULL : sRom.data();
}

size_t PcRomSize(void) {
    return sRom.size();
}

void* PcRomPtr(uint32_t romOffset) {
    if (sRom.empty() || (size_t) romOffset >= sRom.size()) {
        return NULL;
    }
    return sRom.data() + romOffset;
}

int PcDmaFromRom(void* dest, uint32_t romOffset, size_t nbytes) {
    if (dest == NULL || sRom.empty()) {
        return 0;
    }
    if ((size_t) romOffset >= sRom.size()) {
        memset(dest, 0, nbytes);
        return 0;
    }
    size_t avail = sRom.size() - (size_t) romOffset;
    size_t copy = avail < nbytes ? avail : nbytes;
    memcpy(dest, sRom.data() + romOffset, copy);
    if (copy < nbytes) {
        memset((uint8_t*) dest + copy, 0, nbytes - copy);
    }
    return 1;
}

static int romOffForSymbol(uintptr_t addr, uint32_t* off) {
    struct {
        uintptr_t sym;
        uint32_t rom;
    } map[] = {
        { (uintptr_t) _common_texturesSegmentRomStart, 0x132B50 },
        { (uintptr_t) _trigTablesSegmentRomStart, 0x724220 },
        { (uintptr_t) _data_segment2SegmentRomStart, 0x12AAE0 },
        { (uintptr_t) &ok_HeaderROM, 0xBE9178 },
        { (uintptr_t) _startupLogoSegmentRomStart, 0x825800 },
    };
    size_t i;
    for (i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
        if (addr == map[i].sym) {
            *off = map[i].rom;
            return 1;
        }
    }
    return 0;
}

int PcRomResolveSrc(uintptr_t devAddr, const uint8_t** srcOut, size_t nbytes) {
    uint32_t off;
    (void) nbytes;
    if (sRom.empty()) {
        *srcOut = (const uint8_t*) (void*) devAddr;
        return 0;
    }
    if (devAddr >= (uintptr_t) sRom.data() &&
        devAddr < (uintptr_t) sRom.data() + sRom.size()) {
        *srcOut = (const uint8_t*) (void*) devAddr;
        return 1;
    }
    if (romOffForSymbol(devAddr, &off)) {
        *srcOut = sRom.data() + off;
        return 1;
    }
    /* Cartridge / physical (32-bit N64). Use as ROM offset, never as a host pointer. */
    if ((devAddr & 0xFF000000u) == 0xB0000000u || (devAddr & 0xFF000000u) == 0xA0000000u) {
        off = (uint32_t) (devAddr & 0x0FFFFFFFu);
        *srcOut = (off < sRom.size()) ? sRom.data() + off : NULL;
        return 1;
    }
    /* 0x80xxxxxx / 0x90xxxxxx are N64 virtual RAM — not host pointers. */
    if ((devAddr & 0xFF000000u) == 0x80000000u || (devAddr & 0xFF000000u) == 0x90000000u) {
        *srcOut = NULL;
        return 0;
    }
    /* Raw ROM offset (ok_HeaderROM 0xBE9178, course island 0xC00000). */
    if (devAddr < sRom.size()) {
        *srcOut = sRom.data() + (size_t) devAddr;
        return 1;
    }
    *srcOut = (const uint8_t*) (void*) devAddr;
    return 0;
}
