#include <ultra64.h>
#include <stdint.h>
#include <string.h>

/* N64 ld-script symbols. Host BSS, not 0x80 literals. Sizes from mk64.ld comments. */

u8 _memoryPoolSegmentNoloadStart[0xC0000];
u8 _memoryPoolSegmentNoloadEnd[1];
u8 _endingSegmentStart[0xDF00];
u8 _endingSegmentNoloadEnd[1];
u8 _endingSegmentRomStart[1];
u8 _endingSegmentRomEnd[1];
u8 _racingSegmentStart[0x2C470];
u8 _racingSegmentNoloadEnd[1];
u8 _racingSegmentRomStart[1];
u8 _racingSegmentRomEnd[1];
u8 _overkartSegmentStart[0x10000];
u8 _overkartSegmentNoloadEnd[1];
u8 _overkartSegmentRomStart[1];
u8 _overkartSegmentRomEnd[1];
u8 au_FrontDestStart[0x1000];
u8 _au_displaylistsSegmentStart[0x10000];
u8 _au_displaylistsSegmentNoloadEnd[1];
u8 _au_displaylistsSegmentRomStart[1];
u8 _au_displaylistsSegmentRomEnd[1];
u8 _trigTablesSegmentStart[0x5810];
u8 _trigTablesSegmentRomStart[1];
u8 _data_segment2SegmentRomStart[1];
u8 _data_segment2SegmentRomEnd[1];
u8 _common_texturesSegmentRomStart[1];
u8 _common_texturesSegmentRomEnd[1];
u8 _ceremonyDataSegmentRomStart[1];
u8 _ceremonyDataSegmentRomEnd[1];
u8 _startupLogoSegmentRomStart[1];
u8 _startupLogoSegmentRomEnd[1];
u8 ok_ObjectPoolStart[0x100000];

/* Linker sets this symbol's *address* to the size. On PC the value is the size. */
u8 _trigTablesSegmentSize[0x5810];

int ok_HeaderROM;

s8 GamePausedFlag = 0;
u8 SaveGame[0x1000];

s32 osPfsAllocateFile(void* a, u16 b, u32 c, u8* d, u8* e, int f, int* g) {
    (void) a;
    (void) b;
    (void) c;
    (void) d;
    (void) e;
    (void) f;
    (void) g;
    return 0;
}

s32 osPfsReadWriteFile(void* a, s32 b, u8 c, int d, int e, u8* f) {
    (void) a;
    (void) b;
    (void) c;
    (void) d;
    (void) e;
    (void) f;
    return 0;
}

s32 osPfsIsPlug(OSMesgQueue* mq, u8* pattern) {
    (void) mq;
    if (pattern) {
        *pattern = 0;
    }
    return 0;
}

s32 osMotorStop(void* pfs) {
    (void) pfs;
    return 0;
}

s32 osMotorStart(void* pfs) {
    (void) pfs;
    return 0;
}

void encpass1(void) {
}

void encpass2(void) {
}

#define MIO0_GET_BIT(buf, bit) ((buf)[(bit) / 8] & (1 << (7 - ((bit) % 8))))

static u32 mio0_u32be(const u8* p) {
    return ((u32) p[0] << 24) | ((u32) p[1] << 16) | ((u32) p[2] << 8) | (u32) p[3];
}

u32 decodeMIO0(u8* src, u8* dst) {
    u32 destSize;
    u32 compOff;
    u32 uncompOff;
    u32 written = 0;
    int bitIdx = 0;
    int compIdx = 0;
    int uncompIdx = 0;

    if (src == NULL || dst == NULL) {
        return 0;
    }
    if (src[0] != 'M' || src[1] != 'I' || src[2] != 'O' || src[3] != '0') {
        return 0;
    }
    destSize = mio0_u32be(src + 4);
    compOff = mio0_u32be(src + 8);
    uncompOff = mio0_u32be(src + 12);

    while (written < destSize) {
        if (MIO0_GET_BIT(src + 16, bitIdx)) {
            dst[written++] = src[uncompOff + uncompIdx];
            uncompIdx++;
        } else {
            int length;
            int idx;
            int i;
            const u8* vals = src + compOff + compIdx;
            compIdx += 2;
            length = ((vals[0] & 0xF0) >> 4) + 3;
            idx = ((vals[0] & 0x0F) << 8) + vals[1] + 1;
            for (i = 0; i < length; i++) {
                dst[written] = dst[written - idx];
                written++;
            }
        }
        bitIdx++;
    }
    return written;
}
