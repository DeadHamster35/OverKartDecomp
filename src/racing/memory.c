#include <ultra64.h>
#include <PR/ultratypes.h>
#include <macros.h>
#include <common_structs.h>
#include <segments.h>
#include <decode.h>

#include "memory.h"
#include "main.h"
#include "code_800029B0.h"
#include "math_util.h"
#include "courses/courseTable.h"
#include "defines.h"
#include "OverKartHooks.h"

s32 g_GFXCounter;
s32 g_PKCounter;

uintptr_t sPoolFreeSpace;
struct MainPoolBlock* sPoolListHeadL;
struct MainPoolBlock* sPoolListHeadR;

struct MainPoolState* gMainPoolState = NULL;

struct UnkStruct_802B8CD4 D_802B8CD4[] = { 0 };
s32 D_802B8CE4 = 0; // pad
s32 memoryPadding[2];

enum PackedOp {
    PG_LIGHTS_0               = 0x00, /* 0..0x14 mappés sur decodelight */
    /* Presets de combine renommés pour refléter les macros G_CC_* */
    PG_SETCOMBINE_CC_MODULATERGBA      = 0x15,
    PG_SETCOMBINE_CC_MODULATERGBDECALA = 0x16,
    PG_SETCOMBINE_CC_SHADE             = 0x17,
    PG_RMODE_OPA             = 0x18,
    PG_RMODE_TEXEDGE         = 0x19,
    PG_TILECFG_A             = 0x1A,
    PG_TILECFG_B             = 0x1B,
    PG_TILECFG_C             = 0x1C,
    PG_TILECFG_D             = 0x1D,
    PG_TILECFG_E             = 0x1E,
    PG_TILECFG_F             = 0x1F,
    PG_TIMG_LOADBLOCK_0      = 0x20,
    PG_TIMG_LOADBLOCK_1      = 0x21,
    PG_TIMG_LOADBLOCK_2      = 0x22,
    PG_TIMG_LOADBLOCK_3      = 0x23,
    PG_TIMG_LOADBLOCK_4      = 0x24,
    PG_TIMG_LOADBLOCK_5      = 0x25,
    PG_TEXTURE_ON            = 0x26,
    PG_TEXTURE_OFF           = 0x27,
    PG_VTX1                  = 0x28,
    PG_TRI1                  = 0x29,
    PG_ENDDL                 = 0x2A,
    PG_DL                    = 0x2B,
    PG_TILECFG_G             = 0x2C,
    PG_CULLDL                = 0x2D,
    PG_SETCOMBINE_ALT        = 0x2E,
    PG_RMODE_XLU             = 0x2F,
    PG_SPLINE3D              = 0x30,
    PG_VTX_BASE              = 0x32, /* 0x33..0x52 → variant vtx2 */
    PG_SETCOMBINE_CC_DECALRGBA  = 0x53,
    PG_RMODE_OPA_DECAL       = 0x54,
    PG_RMODE_XLU_DECAL       = 0x55,
    PG_SETGEOMETRYMODE       = 0x56,
    PG_CLEARGEOMETRYMODE     = 0x57,
    PG_TRI2                  = 0x58,
    PG_EOF                   = 0xFF,
};

/**
 * @brief Returns the address of the next available memory location and updates the memory pointer
 * to reference the next location of available memory based provided size to allocate.
 * @param size of memory to allocate.
 * @return Address of free memory
 */
void* AllocateMemory(uintptr_t size) {
    uintptr_t freeSpace = (uintptr_t) FreeMemoryPointer;
    size = ALIGN16(size);
    FreeMemoryPointer += size;
    return (void*) freeSpace;
}

/**
 * @brief Stores the physical memory addr for segmented memory in `SegmentTable` using the segment number as an index.
 *
 * This function takes a segment number and a pointer to a memory address, and stores the address in the `SegmentTable`
 * array at the specified segment index. The stored address is truncated to a 29-bit value to ensure that it fits within
 * the memory address. This allows converting between segmented memory and physical memory.
 *
 * @param segment A segment number from 0x0 to 0xF to set the base address.
 * @param addr A pointer containing the physical memory address of the data.
 * @return The stored base address, truncated to a 29-bit value.
 */
uintptr_t SetSegment(s32 segment, void* addr) {
    SegmentTable[segment] = (uintptr_t) addr & 0x1FFFFFFF;
    return SegmentTable[segment];
}

/**
 * @brief Returns the physical memory location of a segment.
 * @param permits segment numbers from 0x0 to 0xF.
 */
void* GetSegment(s32 segment) {
    return (void*) (SegmentTable[segment] | 0x80000000);
}

/**
 * @brief converts an RSP segment + offset address to a normal memory address
 */
void* SegmentToVirtual(const void* addr) {
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    return (void*) ((SegmentTable[segment] + offset) | 0x80000000);
}

void StoreSegments(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        gSPSegment(gDisplayListHead++, i, SegmentTable[i]);
    }
}

/**
 * @brief Sets the starting location for allocating memory and calculates pool size.
 *
 * Default memory size, 701.984 Kilobytes.
 */
void initial_free_memory(uintptr_t poolStart, uintptr_t poolEnd) {

    poolStart = ALIGN16(poolStart);
    // Truncate to a 16-byte boundary.
    poolEnd &= ~0xF;

    free_memory_size = (poolEnd - poolStart) - 0x10;
    FreeMemoryPointer = poolStart;
}

/**
 * @brief Allocates memory and adjusts free_memory_size.
 */
void* allocate_free_memory(size_t size) {
    uintptr_t freeSpace;

    size = ALIGN16(size);
    free_memory_size -= size;
    freeSpace = FreeMemoryPointer;
    FreeMemoryPointer += size;

    return (void*) freeSpace;
}

UNUSED void set_segment_record(s32 arg0, s32 arg1) {
    gD_80150158[arg0].unk0 = arg0;
    gD_80150158[arg0].unk8 = arg1;
}

/**
 * @brief Allocate and DMA.
 */
void* load_data(uintptr_t startAddr, uintptr_t endAddr) {
    void* allocated;
    uintptr_t size = endAddr - startAddr;

    allocated = allocate_free_memory(size);
    if (allocated != 0) {
        DMA((u8*) allocated, (u8*) startAddr, size);
    }
    return (void*) allocated;
}

UNUSED void InitFreeZone(uintptr_t start, uintptr_t end) {
    start = ALIGN16(start);
    end = ALIGN16(end - 15);

    sPoolFreeSpace = (end - start) - 16;

    sPoolListHeadL = (struct MainPoolBlock*) start;
    sPoolListHeadR = (struct MainPoolBlock*) end;
    sPoolListHeadL->prev = NULL;
    sPoolListHeadL->next = NULL;
    sPoolListHeadR->prev = NULL;
    sPoolListHeadR->next = NULL;
}

/**
 * Allocate a block of memory from the pool of given size, and from the
 * specified side of the pool (MEMORY_POOL_LEFT or MEMORY_POOL_RIGHT).
 * If there is not enough space, return NULL.
 */
UNUSED void* AllocFreeZone(uintptr_t size, uintptr_t side) {
    struct MainPoolBlock* newListHead;
    void* addr = NULL;

    size = ALIGN16(size) + 8;
    if (sPoolFreeSpace >= size) {
        sPoolFreeSpace -= size;
        if (side == MEMORY_POOL_LEFT) {
            newListHead = (struct MainPoolBlock*) ((u8*) sPoolListHeadL + size);
            sPoolListHeadL->next = newListHead;
            newListHead->prev = sPoolListHeadL;
            addr = (u8*) sPoolListHeadL + 8;
            sPoolListHeadL = newListHead;
        } else {
            newListHead = (struct MainPoolBlock*) ((u8*) sPoolListHeadR - size);
            sPoolListHeadR->prev = newListHead;
            newListHead->next = sPoolListHeadR;
            sPoolListHeadR = newListHead;
            addr = (u8*) sPoolListHeadR + 8;
        }
    }
    return addr;
}
/**
 * Free a block of memory that was allocated from the pool. The block must be
 * the most recently allocated block from its end of the pool, otherwise all
 * newer blocks are freed as well.
 * Return the amount of free space left in the pool.
 */
UNUSED uintptr_t PurgeFreeZone(void* addr) {
    struct MainPoolBlock* block = (struct MainPoolBlock*) ((u8*) addr - 8);
    struct MainPoolBlock* oldListHead = (struct MainPoolBlock*) ((u8*) addr - 8);

    if (oldListHead < sPoolListHeadL) {
        while (oldListHead->next != NULL) {
            oldListHead = oldListHead->next;
        }
        sPoolListHeadL = block;
        sPoolListHeadL->next = NULL;
        sPoolFreeSpace += (uintptr_t) oldListHead - (uintptr_t) sPoolListHeadL;
    } else {
        while (oldListHead->prev != NULL) {
            oldListHead = oldListHead->prev;
        }
        sPoolListHeadR = block->next;
        sPoolListHeadR->prev = NULL;
        sPoolFreeSpace += (uintptr_t) sPoolListHeadR - (uintptr_t) oldListHead;
    }
    return sPoolFreeSpace;
}
// ResizeFreeZone
UNUSED void* ResizeFreeZone(void* addr, uintptr_t size) {
    void* newAddr = NULL;
    struct MainPoolBlock* block = (struct MainPoolBlock*) ((u8*) addr - 8);

    if (block->next == sPoolListHeadL) {
        PurgeFreeZone(addr);
        newAddr = AllocFreeZone(size, MEMORY_POOL_LEFT);
    }
    return newAddr;
}

UNUSED uintptr_t FreeZoneSize(void) {
    return sPoolFreeSpace - 8;
}

UNUSED uintptr_t LinkFreeZone(void) {
    struct MainPoolState* prevState = gMainPoolState;
    uintptr_t freeSpace = sPoolFreeSpace;
    struct MainPoolBlock* lhead = sPoolListHeadL;
    struct MainPoolBlock* rhead = sPoolListHeadR;

    gMainPoolState = AllocFreeZone(sizeof(*gMainPoolState), MEMORY_POOL_LEFT);
    gMainPoolState->freeSpace = freeSpace;
    gMainPoolState->listHeadL = lhead;
    gMainPoolState->listHeadR = rhead;
    gMainPoolState->prev = prevState;
    return sPoolFreeSpace;
}

/**
 * Restore pool state from a previous call to LinkFreeZone. Return the
 * amount of free space left in the pool.
 */
UNUSED uintptr_t UnlinkFreeZone(void) {
    sPoolFreeSpace = gMainPoolState->freeSpace;
    sPoolListHeadL = gMainPoolState->listHeadL;
    sPoolListHeadR = gMainPoolState->listHeadR;
    gMainPoolState = gMainPoolState->prev;
    return sPoolFreeSpace;
}
// similar to sm64 dma_read
UNUSED void* LoadData(u8* dest, u8* srcStart, u8* srcEnd) {
    void* addr;
    uintptr_t size = srcStart - dest;
    addr = AllocFreeZone(size, (uintptr_t) srcEnd);

    if (addr != 0) {

        osInvalDCache(addr, size);
        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) dest, addr, size, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    }
    return addr;
}

// replaces call to dynamic_dma_read with dma_read.
UNUSED void* LoadSegment(s32 segment, u8* srcStart, u8* srcEnd, u8* side) {
    void* addr = LoadData(srcStart, srcEnd, side);

    if (addr != NULL) {
        SetSegment(segment, addr);
    }
    return addr;
}

// Similar to sm64 load_to_fixed_pool_addr?
UNUSED void* LoadSegmentIndex(s32 arg0, u8* arg1) {
    // uintptr_t srcSize = ALIGN16(srcEnd - srcStart);
    // uintptr_t destSize = ALIGN16((u8 *) sPoolListHeadR - destAddr);
    void* addr;
    uintptr_t temp_v0 = D_802B8CD4[arg0].unk4;
    uintptr_t temp_v1 = D_802B8CD4[arg0].unk8;
    uintptr_t temp_v2 = D_802B8CD4[arg0].unk2;
    addr = LoadData((u8*) temp_v0, (u8*) temp_v1, arg1);

    // dest = AllocFreeZone(destSize, MEMORY_POOL_RIGHT);
    if (addr != 0) {
        SetSegment(temp_v2, addr);
    }
    return (void*) addr;
}

UNUSED void BootSegment(void) {
    s32 temp_s0;
    s16* phi_s1;
    s32 phi_s0;

    phi_s1 = (s16*) &D_802B8CD4;
    phi_s0 = 0;
    do {
        if ((*phi_s1 & 1) != 0) {
            LoadSegmentIndex(phi_s0, 0);
        }
        temp_s0 = phi_s0 + 1;
        phi_s1 += 8;
        phi_s0 = temp_s0;
    } while (phi_s0 != 3);
}

UNUSED struct AllocOnlyPool* InitHeap(uintptr_t size, uintptr_t side) {
    void* addr;
    struct AllocOnlyPool* subPool = NULL;

    size = ALIGN4(size);
    addr = AllocFreeZone(size + sizeof(struct AllocOnlyPool), side);
    if (addr != NULL) {
        subPool = (struct AllocOnlyPool*) addr;
        subPool->totalSpace = size;
        subPool->usedSpace = (s32) addr + sizeof(struct AllocOnlyPool);
        subPool->startPtr = 0;
        subPool->freePtr = (u8*) addr + sizeof(struct AllocOnlyPool);
    }
    return subPool;
}

UNUSED uintptr_t AllocDynamic(s32 arg0) {
    uintptr_t temp_v0;
    uintptr_t phi_v1;

    temp_v0 = D_801502A0 - arg0;
    phi_v1 = 0;
    if (temp_v0 >= (uintptr_t) gDisplayListHead) {
        D_801502A0 = temp_v0;
        phi_v1 = temp_v0;
    }
    return phi_v1;
}

/**
 * @brief Returns pointer to mio0 compressed Vtx.
 */
u8* transgfxdata(u8* start, u8* end) {
    u8* freeSpace;
    uintptr_t size;

    size = ALIGN16(end - start);
    freeSpace = (u8*) FreeMemoryPointer;
    DMA(freeSpace, start, size);
    FreeMemoryPointer += size;
    return freeSpace;
}

// unused mio0 decode func.
UNUSED uintptr_t decodeimage(s32 arg0, s32 arg1, s32 arg2) {
    uintptr_t offset;
    UNUSED void* pad;
    uintptr_t oldAddr;
    void* newAddr;

    offset = ALIGN16(arg1 * arg2);
    oldAddr = FreeMemoryPointer;
    newAddr = (void*) (oldAddr + offset);
    pad = &newAddr;
    osInvalDCache(newAddr, offset);
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], newAddr, offset,
                 &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);

    slidec((u8*) newAddr, (u8*) oldAddr);
    FreeMemoryPointer += offset;
    return oldAddr;
}

UNUSED u8* decodemapimage(u8* arg0, s32 arg1, s32 arg2) {
    u8* temp_v0;
    void* temp_a0;
    temp_v0 = (u8*) FreeMemoryPointer;
    temp_a0 = temp_v0 + arg2;
    arg1 = ALIGN16(arg1);
    arg2 = ALIGN16(arg2);

    osInvalDCache(temp_a0, arg1);
    osPiStartDma(&gDmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], temp_a0, arg1,
                 &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    slidec((u8*) temp_a0, temp_v0);
    FreeMemoryPointer += arg2;
    return temp_v0;
}

/*
 * Tarmac writes Segment 9 texture addrs as (trueROM - 0x641F70).
 * HotSwap course loads add that base back in decodemapimage2 only.
 * DecodeMapImage1 is stock other_textures (objects/particles); never remap.
 */
#define STOCK_OTHER_TEXTURES_ROM 0x641F70

u8* DecodeMapImage1(u8* romaddress, uintptr_t romsize, uintptr_t ramsize) {
    u8* temp_v0;
    void* temp_a0;

    temp_v0 = (u8*) FreeMemoryPointer;
    temp_a0 = temp_v0 + ramsize;
    romsize = ALIGN16(romsize);
    ramsize = ALIGN16(ramsize);
    osInvalDCache((void*) temp_a0, romsize);
    osPiStartDma(&gDmaIoMesg, 0, 0,
                 (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(romaddress)],
                 (void*) temp_a0, romsize, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, (int) 1);
    decodeMIO0((u8*) temp_a0, temp_v0);
    FreeMemoryPointer += ramsize;
    return temp_v0;
}

uintptr_t decodemapimage2(u8* romaddress, uintptr_t romsize, uintptr_t ramsize) {
    uintptr_t oldHeapEndPtr;
    void* temp_v0;
    uintptr_t romAddr;

    romsize = ALIGN16(romsize);
    ramsize = ALIGN16(ramsize);
    oldHeapEndPtr = LastMemoryPointer;
    temp_v0 = (void*) FreeMemoryPointer;

    osInvalDCache(temp_v0, romsize);
    if (HotSwapID > 0) {
        romAddr = STOCK_OTHER_TEXTURES_ROM + (uintptr_t) romaddress;
    } else {
        romAddr = (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(romaddress)];
    }
    osPiStartDma(&gDmaIoMesg, 0, 0, romAddr, temp_v0, romsize, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    decodeMIO0((u8*) temp_v0, (u8*) oldHeapEndPtr);
    LastMemoryPointer += ramsize;
    return oldHeapEndPtr;
}

void decodevertex2(CourseVtx* data, u32 arg1) {
    CourseVtx* courseVtx = data;
    Vtx* vtx;
    s32 tmp = ALIGN16(arg1 * 0x10);
#ifdef AVOID_UB
    u32 i;
#else
    s32 i;
#endif
    s8 temp_a0;
    s8 temp_a3;
    s8 flags;

    LastMemoryPointer -= tmp;
    vtx = (Vtx*) LastMemoryPointer;

    // s32 to u32 comparison required for matching.
    for (i = 0; i < arg1; i++) {
        if (g_ScreenFlip) {
            vtx->v.ob[0] = -courseVtx->ob[0];
        } else {
            vtx->v.ob[0] = courseVtx->ob[0];
        }

        vtx->v.ob[1] = (courseVtx->ob[1] * g_ScreenStretch);
        temp_a0 = courseVtx->ca[0];
        temp_a3 = courseVtx->ca[1];

        flags = temp_a0 & 3;
        flags |= (temp_a3 << 2) & 0xC;

        vtx->v.ob[2] = courseVtx->ob[2];
        vtx->v.tc[0] = courseVtx->tc[0];
        vtx->v.tc[1] = courseVtx->tc[1];
        vtx->v.cn[0] = (temp_a0 & 0xFC);
        vtx->v.cn[1] = (temp_a3 & 0xFC);
        vtx->v.cn[2] = courseVtx->ca[2];
        vtx->v.flag = flags;
        vtx->v.cn[3] = 0xFF;
        vtx++;
        courseVtx++;
    }
}

void decodevertex(CourseVtx* arg0, u32 vertexCount) {
    u32 segment = SEGMENT_NUMBER2(arg0);
    u32 offset = SEGMENT_OFFSET(arg0);
    u8*     vtxCompressed = VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);

    decodeMIO0(vtxCompressed, (u8*) FreeMemoryPointer);
    DecodeVertex2_OK((char*) FreeMemoryPointer, vertexCount);
    SetSegment(4, (void*) VertexMemoryPointer);
}

UNUSED void decodematerial(void) {
}

void decodelight(Gfx* arg0, UNUSED u8* arg1, s8 arg2) {
    UNUSED s32 pad;
    s32 a = (arg2 * 0x18) + 0x9000008;
    s32 b = (arg2 * 0x18) + 0x9000000;
    Gfx macro[] = { gsSPNumLights(NUMLIGHTS_1) };

    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;

    g_GFXCounter++;
    arg0[g_GFXCounter].words.w0 = 0x3860010;

    arg0[g_GFXCounter].words.w1 = a;

    g_GFXCounter++;
    arg0[g_GFXCounter].words.w0 = 0x3880010;
    arg0[g_GFXCounter].words.w1 = b;
    g_GFXCounter++;
}

void decodespdisplaylist(Gfx* arg0, u8* args, UNUSED s8 opcode) {
    uintptr_t temp_v0 = args[g_PKCounter++];
    uintptr_t temp_t7 = ((args[g_PKCounter++]) << 8 | temp_v0) * 8;
    arg0[g_GFXCounter].words.w0 = 0x06000000;
    // Segment seven addr
    arg0[g_GFXCounter].words.w1 = 0x07000000 + temp_t7;
    g_GFXCounter++;
}

// end displaylist
void decodespenddisplaylist(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    arg0[g_GFXCounter].words.w0 = (uintptr_t) (uint8_t) G_ENDDL << 24;
    arg0[g_GFXCounter].words.w1 = 0;
    g_GFXCounter++;
}

void decodesetcullback(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPSetGeometryMode(G_CULL_BACK) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodeclearcullback(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPClearGeometryMode(G_CULL_BACK) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodespculldisplaylist(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPCullDisplayList(0, 7) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodecombine1(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodecombine2(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodecombine3(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodecombine4(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodecombine5(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decoderender1(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decoderender2(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decoderender3(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decoderender4(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_OPA_DECAL, G_RM_AA_ZB_OPA_DECAL) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decoderender5(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL) };
    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodesetup(Gfx* gfx, u8* args, s8 opcode) {
    Gfx tileSync[] = { gsDPTileSync() };
    uintptr_t temp_a0;
    uintptr_t lo;
    uintptr_t hi;

    s32 width;
    s32 height;
    s32 fmt;
    s32 siz;
    s32 line;
    s32 tmem;
    s32 cms;
    s32 masks;
    s32 cmt;
    s32 maskt;
    s32 lrs;
    s32 lrt;
    UNUSED s32 pad[4];

    tmem = 0;
    switch (opcode) {
        case PG_TILECFG_A:
            width = 32;
            height = 32;
            fmt = 0;
            break;
        case PG_TILECFG_G:
            width = 32;
            height = 32;
            fmt = 0;
            tmem = 256;
            break;
        case PG_TILECFG_B:
            width = 64;
            height = 32;
            fmt = 0;
            break;
        case PG_TILECFG_C:
            width = 32;
            height = 64;
            fmt = 0;
            break;
        case PG_TILECFG_D:
            width = 32;
            height = 32;
            fmt = 3;
            break;
        case PG_TILECFG_E:
            width = 64;
            height = 32;
            fmt = 3;
            break;
        case PG_TILECFG_F:
            width = 32;
            height = 64;
            fmt = 3;
            break;
    }

    // Set arguments

    siz = G_IM_SIZ_16b_BYTES;
    line = ((((width * 2) + 7) >> 3));

    temp_a0 = args[g_PKCounter++];
    cms = temp_a0 & 0xF;
    masks = (temp_a0 & 0xF0) >> 4;

    temp_a0 = args[g_PKCounter++];
    cmt = temp_a0 & 0xF;
    maskt = (temp_a0 & 0xF0) >> 4;

    // Generate gfx

    gfx[g_GFXCounter].words.w0 = tileSync->words.w0;
    gfx[g_GFXCounter].words.w1 = tileSync->words.w1;
    g_GFXCounter++;

    lo = ((uintptr_t) (uint8_t) G_SETTILE << 24) | (fmt << 21) | (siz << 19) | (line << 9) | tmem;
    hi = ((cmt) << 18) | ((maskt) << 14) | ((cms) << 8) | ((masks) << 4);

    gfx[g_GFXCounter].words.w0 = lo;
    gfx[g_GFXCounter].words.w1 = hi;
    g_GFXCounter++;

    lrs = (width - 1) << 2;
    lrt = (height - 1) << 2;

    lo = ((uintptr_t) (uint8_t) G_SETTILESIZE << 24);
    hi = (lrs << 12) | lrt;

    gfx[g_GFXCounter].words.w0 = lo;
    gfx[g_GFXCounter].words.w1 = hi;
    g_GFXCounter++;
}

void decodeload(Gfx* gfx, u8* args, s8 opcode) {
    UNUSED uintptr_t var;
    Gfx tileSync[] = { gsDPTileSync() };
    Gfx loadSync[] = { gsDPLoadSync() };

    uintptr_t arg;
    uintptr_t lo;
    uintptr_t hi;
    uintptr_t addr;
    uintptr_t width;
    uintptr_t height;
    uintptr_t fmt;
    uintptr_t siz;
    uintptr_t tmem;
    uintptr_t tile;

    switch (opcode) {
        case PG_TIMG_LOADBLOCK_0:
            width = 32;
            height = 32;
            fmt = 0;
            break;
        case PG_TIMG_LOADBLOCK_1:
            width = 64;
            height = 32;
            fmt = 0;
            break;
        case PG_TIMG_LOADBLOCK_2:
            width = 32;
            height = 64;
            fmt = 0;
            break;
        case PG_TIMG_LOADBLOCK_3:
            width = 32;
            height = 32;
            fmt = 3;
            break;
        case PG_TIMG_LOADBLOCK_4:
            width = 64;
            height = 32;
            fmt = 3;
            break;
        case PG_TIMG_LOADBLOCK_5:
            width = 32;
            height = 64;
            fmt = 3;
            break;
    }

    // Set arguments

    // Waa?
    var = args[g_PKCounter];
    // Generates a texture address.
    addr = SEGMENT_ADDR(0x05, args[g_PKCounter++] << 11);
    g_PKCounter++;
    arg = args[g_PKCounter++];
    siz = G_IM_SIZ_16b;
    tmem = (arg & 0xF);
    tile = (arg & 0xF0) >> 4;

    // Generate gfx

    lo = ((uintptr_t) (uint8_t) G_SETTIMG << 24) | (fmt << 21) | (siz << 19);
    gfx[g_GFXCounter].words.w0 = lo;
    gfx[g_GFXCounter].words.w1 = addr;
    g_GFXCounter++;

    gfx[g_GFXCounter].words.w0 = tileSync->words.w0;
    gfx[g_GFXCounter].words.w1 = tileSync->words.w1;
    g_GFXCounter++;

    lo = ((uintptr_t) (uint8_t) G_SETTILE << 24) | (fmt << 21) | (siz << 19) | tmem;
    hi = tile << 24;

    gfx[g_GFXCounter].words.w0 = lo;
    gfx[g_GFXCounter].words.w1 = hi;
    g_GFXCounter++;

    gfx[g_GFXCounter].words.w0 = loadSync->words.w0;
    gfx[g_GFXCounter].words.w1 = loadSync->words.w1;
    g_GFXCounter++;

    lo = (uintptr_t) (uint8_t) G_LOADBLOCK << 24;
    hi = (tile << 24) | (MIN((width * height) - 1, 0x7FF) << 12) | CALC_DXT(width, G_IM_SIZ_16b_BYTES);

    gfx[g_GFXCounter].words.w0 = lo;
    gfx[g_GFXCounter].words.w1 = hi;
    g_GFXCounter++;
}

void decodesptextureon(Gfx* arg0, UNUSED u8* args, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON) };

    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodesptextureoff(Gfx* arg0, UNUSED u8* args, UNUSED s8 arg2) {
    Gfx macro[] = { gsSPTexture(0x1, 0x1, 0, G_TX_RENDERTILE, G_OFF) };

    arg0[g_GFXCounter].words.w0 = macro->words.w0;
    arg0[g_GFXCounter].words.w1 = macro->words.w1;
    g_GFXCounter++;
}

void decodespvertex(Gfx* gfx, u8* args, UNUSED s8 arg2) {
    uintptr_t temp_t7;
    uintptr_t temp_t7_2;

    uintptr_t temp = args[g_PKCounter++];
    uintptr_t temp2 = ((args[g_PKCounter++] << 8) | temp) * 0x10;

    temp = args[g_PKCounter++];
    temp_t7 = temp & 0x3F;
    temp = args[g_PKCounter++];
    temp_t7_2 = temp & 0x3F;

    gfx[g_GFXCounter].words.w0 =
        ((uintptr_t) (uint8_t) G_VTX << 24) | (temp_t7_2 * 2 << 16) | (((temp_t7 << 10) + ((0x10 * temp_t7) - 1)));
    gfx[g_GFXCounter].words.w1 = 0x04000000 + temp2;
    g_GFXCounter++;
}

void decodespvertex_n(Gfx* gfx, u8* args, s8 arg2) {
    uintptr_t temp_t9;
    uintptr_t temp_v1;
    uintptr_t temp_v2;

    temp_v1 = args[g_PKCounter++];
    temp_v2 = ((args[g_PKCounter++] << 8) | temp_v1) * 0x10;

    temp_t9 = arg2 - PG_VTX_BASE;

    gfx[g_GFXCounter].words.w0 = ((uintptr_t) (uint8_t) G_VTX << 24) | ((temp_t9 << 10) + (((temp_t9) * 0x10) - 1));
    gfx[g_GFXCounter].words.w1 = 0x4000000 + temp_v2;
    g_GFXCounter++;
}

void decodesp1triangle(Gfx* gfx, u8* args, UNUSED s8 arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_a0;
    uintptr_t phi_a2;
    uintptr_t phi_a3;

    temp_v0 = args[g_PKCounter++];

    if (g_ScreenFlip) {
        phi_a3 = temp_v0 & 0x1F;
        phi_a2 = (temp_v0 >> 5) & 7;
        temp_v0 = args[g_PKCounter++];
        phi_a2 |= (temp_v0 & 3) * 8;
        phi_a0 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_a0 = temp_v0 & 0x1F;
        phi_a2 = (temp_v0 >> 5) & 7;
        temp_v0 = args[g_PKCounter++];
        phi_a2 |= (temp_v0 & 3) * 8;
        phi_a3 = (temp_v0 >> 2) & 0x1F;
    }
    gfx[g_GFXCounter].words.w0 = ((uintptr_t) (uint8_t) G_TRI1 << 24);
    gfx[g_GFXCounter].words.w1 = ((phi_a0 * 2) << 16) | ((phi_a2 * 2) << 8) | (phi_a3 * 2);
    g_GFXCounter++;
}

void decodesp2triangle(Gfx* gfx, u8* args, UNUSED s8 arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_t0;
    uintptr_t phi_a3;
    uintptr_t phi_a0;
    uintptr_t phi_t2;
    uintptr_t phi_t1;
    uintptr_t phi_a2;

    temp_v0 = args[g_PKCounter++];

    if (g_ScreenFlip) {
        phi_t0 = temp_v0 & 0x1F;
        phi_a3 = (temp_v0 >> 5) & 7;
        temp_v0 = args[g_PKCounter++];
        phi_a3 |= (temp_v0 & 3) * 8;
        phi_a0 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_a0 = temp_v0 & 0x1F;
        phi_a3 = (temp_v0 >> 5) & 7;
        temp_v0 = args[g_PKCounter++];
        phi_a3 |= (temp_v0 & 3) * 8;
        phi_t0 = (temp_v0 >> 2) & 0x1F;
    }

    temp_v0 = args[g_PKCounter++];

    if (g_ScreenFlip) {
        phi_a2 = temp_v0 & 0x1F;
        phi_t1 = (temp_v0 >> 5) & 7;
        temp_v0 = args[g_PKCounter++];
        phi_t1 |= (temp_v0 & 3) * 8;
        phi_t2 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_t2 = temp_v0 & 0x1F;
        phi_t1 = (temp_v0 >> 5) & 7;
        temp_v0 = args[g_PKCounter++];
        phi_t1 |= (temp_v0 & 3) * 8;
        phi_a2 = (temp_v0 >> 2) & 0x1F;
    }
    gfx[g_GFXCounter].words.w0 =
        ((uintptr_t) (uint8_t) G_TRI2 << 24) | ((phi_a0 * 2) << 16) | ((phi_a3 * 2) << 8) | (phi_t0 * 2);
    gfx[g_GFXCounter].words.w1 = ((phi_t2 * 2) << 16) | ((phi_t1 * 2) << 8) | (phi_a2 * 2);
    g_GFXCounter++;
}

void decodesp1quadrangle(Gfx* gfx, u8* arg1, UNUSED s8 arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_a0;
    uintptr_t phi_t0;
    uintptr_t phi_a3;
    uintptr_t phi_a2;

    temp_v0 = arg1[g_PKCounter++];

    if (g_ScreenFlip != 0) {
        phi_a0 = temp_v0 & 0x1F;
        phi_a2 = ((temp_v0 >> 5) & 7);
        temp_v0 = arg1[g_PKCounter++];
        phi_a2 |= ((temp_v0 & 3) * 8);
        phi_a3 = (temp_v0 >> 2) & 0x1F;
        phi_t0 = ((temp_v0 >> 7) & 1);
        temp_v0 = arg1[g_PKCounter++];
        phi_t0 |= (temp_v0 & 0xF) * 2;
    } else {
        phi_t0 = temp_v0 & 0x1F;
        phi_a3 = ((temp_v0 >> 5) & 7);
        temp_v0 = arg1[g_PKCounter++];
        phi_a3 |= ((temp_v0 & 3) * 8);
        phi_a2 = (temp_v0 >> 2) & 0x1F;
        phi_a0 = ((temp_v0 >> 7) & 1);
        temp_v0 = arg1[g_PKCounter++];
        phi_a0 |= (temp_v0 & 0xF) * 2;
    }
    gfx[g_GFXCounter].words.w0 = ((uintptr_t) (uint8_t) G_QUAD << 24);
    gfx[g_GFXCounter].words.w1 = ((phi_a0 * 2) << 24) | ((phi_t0 * 2) << 16) | ((phi_a3 * 2) << 8) | (phi_a2 * 2);
    g_GFXCounter++;
}

UNUSED void decodeXXXXXXX(void) {
}

/**
 * Unpacks course packed displaylists by iterating through each byte of the packed file.
 * Each packed displaylist entry has an opcode and any number of arguments.
 * The opcodes range from 0 to 87 which are used to run the relevant unpack function.
 * The file pointer increments when arguments are used. This way,
 * decodedisplaylist will always read an opcode and not an argument by accident.
 *
 * @warning opcodes that do not contain a definition in the switch are ignored. If an undefined opcode
 * contained arguments the unpacker might try to unpack those arguments.
 * This issue is prevented so long as the packed file adheres to correct opcodes and unpack code
 * increments the file pointer the correct number of times.
 */
void decodedisplaylist(uintptr_t* data, uintptr_t finalDisplaylistOffset, u32 arg2) {
    uintptr_t segment = SEGMENT_NUMBER2(data);
    uintptr_t offset = SEGMENT_OFFSET(data);
    u8* packed_dl = VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);

    Gfx* gfx;
    u32 addr;

    u8 opcode;

    finalDisplaylistOffset = ALIGN16(finalDisplaylistOffset) + 8;
    LastMemoryPointer -= finalDisplaylistOffset;
    addr = LastMemoryPointer;
    gfx = (Gfx*) LastMemoryPointer;
    g_GFXCounter = 0;
    g_PKCounter = 0;

    while (true) {

        // Seek to the next byte
        opcode = packed_dl[g_PKCounter++];

        // Break when the eof has been reached denoted by opcode 0xFF
        if (opcode == 0xFF) {
            break;
        }

        switch (opcode) {
            case PG_LIGHTS_0 + 0x0:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x1:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x2:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x3:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x4:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x5:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x6:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x7:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x8:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x9:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xA:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xB:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xC:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xD:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xE:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xF:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x10:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x11:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x12:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x13:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x14:
                decodelight(gfx, packed_dl, opcode);
                break;
            case PG_SETCOMBINE_CC_MODULATERGBA:
                decodecombine1(gfx, packed_dl, arg2);
                break;
            case PG_SETCOMBINE_CC_MODULATERGBDECALA:
                decodecombine2(gfx, packed_dl, arg2);
                break;
            case PG_SETCOMBINE_CC_SHADE:
                decodecombine3(gfx, packed_dl, arg2);
                break;
            case 0x2E:
                decodecombine4(gfx, packed_dl, arg2);
                break;
            case PG_SETCOMBINE_CC_DECALRGBA:
                decodecombine5(gfx, packed_dl, arg2);
                break;
            case PG_RMODE_OPA:
                decoderender1(gfx, packed_dl, arg2);
                break;
            case PG_RMODE_TEXEDGE:
                decoderender2(gfx, packed_dl, arg2);
                break;
            case PG_RMODE_XLU:
                decoderender3(gfx, packed_dl, arg2);
                break;
            case PG_RMODE_OPA_DECAL:
                decoderender4(gfx, packed_dl, arg2);
                break;
            case PG_RMODE_XLU_DECAL:
                decoderender5(gfx, packed_dl, arg2);
                break;
            case PG_TILECFG_A:
                decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_G:
                decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_B:
                decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_C:
                decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_D:
                decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_E:
                decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_F:
                decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_0:
                decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_1:
                decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_2:
                decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_3:
                decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_4:
                decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_5:
                decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TEXTURE_ON:
                decodesptextureon(gfx, packed_dl, opcode);
                break;
            case PG_TEXTURE_OFF:
                decodesptextureoff(gfx, packed_dl, opcode);
                break;
            case PG_VTX1:
                decodespvertex(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x01:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x02:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x03:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x04:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x05:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x06:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x07:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x08:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x09:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0A:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0B:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0C:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0D:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0E:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0F:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x10:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x11:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x12:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x13:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x14:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x15:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x16:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x17:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x18:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x19:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1A:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1B:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1C:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1D:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1E:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1F:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x20:
                decodespvertex_n(gfx, packed_dl, opcode);
                break;
            case PG_TRI1:
                DecodeSP1Triangle_OK(gfx, packed_dl, opcode);
                break;
            case PG_TRI2:
                DecodeSP2Triangle_OK(gfx, packed_dl, opcode);
                break;
            case PG_SPLINE3D:
                DecodeSP1Quadrangle_OK(gfx, packed_dl, opcode);
                break;
            case PG_CULLDL:
                decodespculldisplaylist(gfx, packed_dl, opcode);
                break;
            case PG_ENDDL:
                decodespenddisplaylist(gfx, packed_dl, opcode);
                break;
            case PG_SETGEOMETRYMODE:
                decodesetcullback(gfx, packed_dl, opcode);
                break;
            case PG_CLEARGEOMETRYMODE:
                decodeclearcullback(gfx, packed_dl, opcode);
                break;
            case PG_DL:
                decodespdisplaylist(gfx, packed_dl, opcode);
                break;
            default:
                // Skip unknown values
                break;
        }
    }
    SetSegment(0x7, (void*) addr);
}

struct UnkStr_802AA7C8 {
    u8* unk0;
    uintptr_t unk4;
    uintptr_t unk8;
    uintptr_t unkC;
};

void decodemaptexture(u32* arg0) {
    u32 segment = SEGMENT_NUMBER2(arg0);
    u32 offset = SEGMENT_OFFSET(arg0);
    struct UnkStr_802AA7C8* phi_s0 = (struct UnkStr_802AA7C8*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + offset);
    struct UnkStr_802AA7C8* temp_s0;
    uintptr_t temp_t2;
    u8* temp_a0;
    uintptr_t phi_v0;
    uintptr_t sp20;

    phi_v0 = 0;
    temp_s0 = phi_s0;
    while (true) {
        temp_a0 = phi_s0->unk0;
        if ((temp_a0) == 0) {
            break;
        }
        phi_v0 += phi_s0->unk8;
        phi_s0++;
    }
    phi_s0 = temp_s0;
    LastMemoryPointer -= phi_v0;
    sp20 = LastMemoryPointer;

    while (true) {
        temp_a0 = phi_s0->unk0;
        if ((temp_a0) == 0) {
            break;
        }
        decodemapimage2(temp_a0, phi_s0->unk4, phi_s0->unk8);
        phi_s0++;
    }
    LastMemoryPointer = sp20;
    temp_t2 = LastMemoryPointer;
    SetSegment(0x5, (void*) temp_t2);
}

void* load_press_data(u8* start, u8* end) {
    UNUSED u32 pad;
    u32 sp28;
    u32 size = ALIGN16(end - start);
    u8* heapEnd;
    u32* freeSpace;

    heapEnd = (u8*) LastMemoryPointer - size;
    // sp20 = temp_a0;
    DMA(heapEnd, start, size);
    sp28 = *(u32*) (heapEnd + 4);
    sp28 = ALIGN16(sp28);
    freeSpace = (u32*) FreeMemoryPointer;
    decodeMIO0(heapEnd, (u8*) freeSpace);
    FreeMemoryPointer += sp28;
    return (void*) freeSpace;
}

void CheckCourseMemoryHighWater(void) {
    uintptr_t faceEnd;

    if (FreeMemoryPointer > MEMORY_POOL_END) {
        while (1) {}
    }
    if (VertexMemoryPointer < VERTEX_MEMORY_FLOOR) {
        while (1) {}
    }
    if (ObjectMemoryPointer >= (uintptr_t) ok_ObjectPoolEnd) {
        while (1) {}
    }
    faceEnd = (uintptr_t) gFaceBuffer + (g_courseFaceMaxIndex * sizeof(FaceStruct));
    if (faceEnd >= FACE_BUFFER_END) {
        while (1) {}
    }
    if ((faceEnd > FACE_BUFFER_START) && (faceEnd > VertexMemoryPointer)) {
        while (1) {}
    }
    if ((gGamestate == ENDING) || (gGamestate == CREDITS_SEQUENCE)) {
        if (LastMemoryPointer < MEMORY_POOL_END) {
            while (1) {}
        }
    } else if (LastMemoryPointer < LAST_MEMORY_FLOOR) {
        while (1) {}
    }
}

/**
 * @brief Loads & DMAs course data. Vtx, textures, displaylists, etc.
 * @param courseId
 *
 * Order: throwaway geography (0xF) on FreeMemory, decode verts (4) from VertexMemoryPointer
 * and packed DLs (7) from LastMemoryPointer, rewind FreeMemory (drops 0xF), eat-own-tail
 * MIO0 course data (6), offsets (9), then map textures (5).
 * Segment 9 must be live before decodemaptexture — the texture table is linked at 0x09000000.
 */
u8* load_course(s32 courseId) {
    u8* vtxCompressed;
    u8* courseDataRomStart;
    u8* courseDataRomEnd;
    u8* vertexRomStart;
    u8* vertexRomEnd;
    u32* textures;
    CourseVtx* vertexStart;
    u8* packedStart;
    u32 vertexCount;
    u8* finalDisplaylistOffset;
    u32 unknown1;
    u8* offsetRomStart;
    u8* offsetRomEnd;
    uintptr_t geoSize;
    uintptr_t compSize;
    u8* compDest;

    courseDataRomStart = g_courseTable[courseId].dlRomStart;
    courseDataRomEnd = g_courseTable[courseId].dlRomEnd;
    offsetRomStart = g_courseTable[courseId].offsetRomStart;
    offsetRomEnd = g_courseTable[courseId].offsetRomEnd;
    vertexRomStart = g_courseTable[courseId].vertexRomStart;
    vertexRomEnd = g_courseTable[courseId].vertexRomEnd;
    textures = g_courseTable[courseId].textures;
    vertexStart = g_courseTable[courseId].vertexStart;
    packedStart = g_courseTable[courseId].packedStart;
    vertexCount = g_courseTable[courseId].vertexCount;
    finalDisplaylistOffset = g_courseTable[courseId].finalDisplaylistOffset;
    unknown1 = g_courseTable[courseId].unknown1;

    if (g_gameMode == GRAND_PRIX) {
        if ((gpCourseIndex > asm_CupCount) && (HotSwapID > 0)) {
            HotSwapGP = HotSwapID;
            HotSwapID = 0;
        }
    }

    if ((gGamestate == ENDING) || (gGamestate == CREDITS_SEQUENCE)) {
        LastMemoryPointer = SEG_ENDING;
    } else {
        LastMemoryPointer = LAST_MEMORY_TOP;
    }

    FreeMemoryPointer = StaticMemoryPointer;

    geoSize = ALIGN16((uintptr_t) vertexRomEnd - (uintptr_t) vertexRomStart);
    vtxCompressed = (u8*) FreeMemoryPointer;
    DMA(vtxCompressed, vertexRomStart, geoSize);
    FreeMemoryPointer += geoSize;
    SetSegment(0xF, (void*) vtxCompressed);

    decodevertex(vertexStart, vertexCount);
    decodedisplaylist((uintptr_t*) packedStart, (uintptr_t) finalDisplaylistOffset, unknown1);

    FreeMemoryPointer = StaticMemoryPointer;

    if (gGamestate != ENDING) {
        compSize = ALIGN16((uintptr_t) courseDataRomEnd - (uintptr_t) courseDataRomStart);
        compDest = (u8*) (MEMORY_POOL_END - compSize);
        DMA(compDest, courseDataRomStart, compSize);
        FreeMemoryPointer = ALIGN16((uintptr_t) decodeMIO0(compDest, (u8*) StaticMemoryPointer));
        SetSegment(6, (void*) StaticMemoryPointer);
    }

    SetSegment(9, load_data((uintptr_t) offsetRomStart, (uintptr_t) offsetRomEnd));
    decodemaptexture(textures);
    CheckCourseMemoryHighWater();
    return vtxCompressed;
}
