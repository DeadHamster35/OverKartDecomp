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

s32 GFXCounter;
s32 PKCounter;

uintptr_t SPoolFreeSpace;
struct MainPoolBlock* SPoolListHeadL;
struct MainPoolBlock* SPoolListHeadR;

struct MainPoolState* MainPoolState = NULL;

struct UnkStruct_802B8CD4 D_802B8CD4[] = { 0 };
s32 D_802B8CE4 = 0; // pad
s32 MemoryPadding[2];

enum PackedOp {
    PG_LIGHTS_0               = 0x00, /* 0..0x14 mappés sur Decodelight */
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
    PG_VTX_BASE              = 0x32, /* 0x33..0x52 → Variant vtx2 */
    PG_SETCOMBINE_CC_DECALRGBA  = 0x53,
    PG_RMODE_OPA_DECAL       = 0x54,
    PG_RMODE_XLU_DECAL       = 0x55,
    PG_SETGEOMETRYMODE       = 0x56,
    PG_CLEARGEOMETRYMODE     = 0x57,
    PG_TRI2                  = 0x58,
    PG_EOF                   = 0xFF,
};

/**
 * @brief Returns the Address of the Next available memory location and updates the memory Pointer
 * to reference the Next location of available memory based provided Size to allocate.
 * @Param Size of memory to allocate.
 * @return Address of Free memory
 */
void* AllocateMemory(uintptr_t Size) {
    uintptr_t FreeSpace = (uintptr_t) FreeMemoryPointer;
    Size = ALIGN16(Size);
    FreeMemoryPointer += Size;
    return (void*) FreeSpace;
}

/**
 * @brief Stores the physical memory Addr for segmented memory in `SegmentTable` using the segment Number as an Index.
 *
 * This function takes a segment Number and a Pointer to a memory Address, and stores the Address in the `SegmentTable`
 * array at the specified segment Index. The stored Address is truncated to a 29-bit Value to ensure that it fits within
 * the memory Address. This allows converting between segmented memory and physical memory.
 *
 * @Param segment A segment Number from 0x0 to 0xF to set the base Address.
 * @Param Addr A Pointer containing the physical memory Address of the data.
 * @return The stored base Address, truncated to a 29-bit Value.
 */
uintptr_t SetSegment(s32 segment, void* Addr) {
    SegmentTable[segment] = (uintptr_t) Addr & 0x1FFFFFFF;
    return SegmentTable[segment];
}

/**
 * @brief Returns the physical memory location of a segment.
 * @Param permits segment numbers from 0x0 to 0xF.
 */
void* GetSegment(s32 segment) {
    return (void*) (SegmentTable[segment] | 0x80000000);
}

/**
 * @brief converts an RSP segment + Offset Address to a normal memory Address
 */
void* SegmentToVirtual(const void* Addr) {
    size_t segment = (uintptr_t) Addr >> 24;
    size_t Offset = (uintptr_t) Addr & 0x00FFFFFF;

    return (void*) ((SegmentTable[segment] + Offset) | 0x80000000);
}

void StoreSegments(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        gSPSegment(DisplayListHead++, i, SegmentTable[i]);
    }
}

/**
 * @brief Sets the starting location for allocating memory and calculates Pool Size.
 *
 * Default memory Size, 701.984 Kilobytes.
 */
void InitialFreeMemory(uintptr_t poolStart, uintptr_t poolEnd) {

    poolStart = ALIGN16(poolStart);
    // Truncate to a 16-byte boundary.
    poolEnd &= ~0xF;

    FreeMemorySize = (poolEnd - poolStart) - 0x10;
    FreeMemoryPointer = poolStart;
}

/**
 * @brief Allocates memory and adjusts FreeMemorySize.
 */
void* AllocateFreeMemory(size_t Size) {
    uintptr_t FreeSpace;

    Size = ALIGN16(Size);
    FreeMemorySize -= Size;
    FreeSpace = FreeMemoryPointer;
    FreeMemoryPointer += Size;

    return (void*) FreeSpace;
}

UNUSED void SetSegmentRecord(s32 arg0, s32 arg1) {
    D80150158[arg0].Unk0 = arg0;
    D80150158[arg0].Unk8 = arg1;
}

/**
 * @brief Allocate and DMA.
 */
void* LoadSegment(uintptr_t startAddr, uintptr_t endAddr) {
    void* allocated;
    uintptr_t Size = endAddr - startAddr;

    allocated = AllocateFreeMemory(Size);
    if (allocated != 0) {
        DMA((u8*) allocated, (u8*) startAddr, Size);
    }
    return (void*) allocated;
}

UNUSED void InitFreeZone(uintptr_t Start, uintptr_t end) {
    Start = ALIGN16(Start);
    end = ALIGN16(end - 15);

    SPoolFreeSpace = (end - Start) - 16;

    SPoolListHeadL = (struct MainPoolBlock*) Start;
    SPoolListHeadR = (struct MainPoolBlock*) end;
    SPoolListHeadL->Prev = NULL;
    SPoolListHeadL->Next = NULL;
    SPoolListHeadR->Prev = NULL;
    SPoolListHeadR->Next = NULL;
}

/**
 * Allocate a block of memory from the Pool of given Size, and from the
 * specified side of the Pool (MEMORY_POOL_LEFT or MEMORY_POOL_RIGHT).
 * If there is not enough space, return NULL.
 */
UNUSED void* AllocFreeZone(uintptr_t Size, uintptr_t side) {
    struct MainPoolBlock* newListHead;
    void* Addr = NULL;

    Size = ALIGN16(Size) + 8;
    if (SPoolFreeSpace >= Size) {
        SPoolFreeSpace -= Size;
        if (side == MEMORY_POOL_LEFT) {
            newListHead = (struct MainPoolBlock*) ((u8*) SPoolListHeadL + Size);
            SPoolListHeadL->Next = newListHead;
            newListHead->Prev = SPoolListHeadL;
            Addr = (u8*) SPoolListHeadL + 8;
            SPoolListHeadL = newListHead;
        } else {
            newListHead = (struct MainPoolBlock*) ((u8*) SPoolListHeadR - Size);
            SPoolListHeadR->Prev = newListHead;
            newListHead->Next = SPoolListHeadR;
            SPoolListHeadR = newListHead;
            Addr = (u8*) SPoolListHeadR + 8;
        }
    }
    return Addr;
}
/**
 * Free a block of memory that was allocated from the Pool. The block must be
 * the most recently allocated block from its end of the Pool, otherwise all
 * newer blocks are freed as well.
 * Return the amount of Free space Left in the Pool.
 */
UNUSED uintptr_t PurgeFreeZone(void* Addr) {
    struct MainPoolBlock* block = (struct MainPoolBlock*) ((u8*) Addr - 8);
    struct MainPoolBlock* oldListHead = (struct MainPoolBlock*) ((u8*) Addr - 8);

    if (oldListHead < SPoolListHeadL) {
        while (oldListHead->Next != NULL) {
            oldListHead = oldListHead->Next;
        }
        SPoolListHeadL = block;
        SPoolListHeadL->Next = NULL;
        SPoolFreeSpace += (uintptr_t) oldListHead - (uintptr_t) SPoolListHeadL;
    } else {
        while (oldListHead->Prev != NULL) {
            oldListHead = oldListHead->Prev;
        }
        SPoolListHeadR = block->Next;
        SPoolListHeadR->Prev = NULL;
        SPoolFreeSpace += (uintptr_t) SPoolListHeadR - (uintptr_t) oldListHead;
    }
    return SPoolFreeSpace;
}
// ResizeFreeZone
UNUSED void* ResizeFreeZone(void* Addr, uintptr_t Size) {
    void* newAddr = NULL;
    struct MainPoolBlock* block = (struct MainPoolBlock*) ((u8*) Addr - 8);

    if (block->Next == SPoolListHeadL) {
        PurgeFreeZone(Addr);
        newAddr = AllocFreeZone(Size, MEMORY_POOL_LEFT);
    }
    return newAddr;
}

UNUSED uintptr_t FreeZoneSize(void) {
    return SPoolFreeSpace - 8;
}

UNUSED uintptr_t LinkFreeZone(void) {
    struct MainPoolState* prevState = MainPoolState;
    uintptr_t FreeSpace = SPoolFreeSpace;
    struct MainPoolBlock* lhead = SPoolListHeadL;
    struct MainPoolBlock* rhead = SPoolListHeadR;

    MainPoolState = AllocFreeZone(sizeof(*MainPoolState), MEMORY_POOL_LEFT);
    MainPoolState->FreeSpace = FreeSpace;
    MainPoolState->ListHeadL = lhead;
    MainPoolState->ListHeadR = rhead;
    MainPoolState->Prev = prevState;
    return SPoolFreeSpace;
}

/**
 * Restore Pool State from a previous call to LinkFreeZone. Return the
 * amount of Free space Left in the Pool.
 */
UNUSED uintptr_t UnlinkFreeZone(void) {
    SPoolFreeSpace = MainPoolState->FreeSpace;
    SPoolListHeadL = MainPoolState->ListHeadL;
    SPoolListHeadR = MainPoolState->ListHeadR;
    MainPoolState = MainPoolState->Prev;
    return SPoolFreeSpace;
}
// similar to sm64 dma_read
UNUSED void* LoadData(u8* dest, u8* srcStart, u8* srcEnd) {
    void* Addr;
    uintptr_t Size = srcStart - dest;
    Addr = AllocFreeZone(Size, (uintptr_t) srcEnd);

    if (Addr != 0) {

        osInvalDCache(Addr, Size);
        osPiStartDma(&DmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, (uintptr_t) dest, Addr, Size, &DmaMesgQueue);
        osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, OS_MESG_BLOCK);
    }
    return Addr;
}

// replaces call to dynamic_dma_read with dma_read.
UNUSED void* LoadSegmentAndSet(s32 segment, u8* srcStart, u8* srcEnd, u8* side) {
    void* Addr = LoadData(srcStart, srcEnd, side);

    if (Addr != NULL) {
        SetSegment(segment, Addr);
    }
    return Addr;
}

// Similar to sm64 load_to_fixed_pool_addr?
UNUSED void* LoadSegmentIndex(s32 arg0, u8* arg1) {
    // uintptr_t srcSize = ALIGN16(srcEnd - srcStart);
    // uintptr_t destSize = ALIGN16((u8 *) SPoolListHeadR - destAddr);
    void* Addr;
    uintptr_t temp_v0 = D_802B8CD4[arg0].unk4;
    uintptr_t temp_v1 = D_802B8CD4[arg0].Unk8;
    uintptr_t temp_v2 = D_802B8CD4[arg0].Unk2;
    Addr = LoadData((u8*) temp_v0, (u8*) temp_v1, arg1);

    // dest = AllocFreeZone(destSize, MEMORY_POOL_RIGHT);
    if (Addr != 0) {
        SetSegment(temp_v2, Addr);
    }
    return (void*) Addr;
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

UNUSED struct AllocOnlyPool* InitHeap(uintptr_t Size, uintptr_t side) {
    void* Addr;
    struct AllocOnlyPool* subPool = NULL;

    Size = ALIGN4(Size);
    Addr = AllocFreeZone(Size + sizeof(struct AllocOnlyPool), side);
    if (Addr != NULL) {
        subPool = (struct AllocOnlyPool*) Addr;
        subPool->TotalSpace = Size;
        subPool->UsedSpace = (s32) Addr + sizeof(struct AllocOnlyPool);
        subPool->StartPtr = 0;
        subPool->FreePtr = (u8*) Addr + sizeof(struct AllocOnlyPool);
    }
    return subPool;
}

UNUSED uintptr_t AllocDynamic(s32 arg0) {
    uintptr_t temp_v0;
    uintptr_t phi_v1;

    temp_v0 = D_801502A0 - arg0;
    phi_v1 = 0;
    if (temp_v0 >= (uintptr_t) DisplayListHead) {
        D_801502A0 = temp_v0;
        phi_v1 = temp_v0;
    }
    return phi_v1;
}

/**
 * @brief Returns Pointer to mio0 compressed Vtx.
 */
u8* Transgfxdata(u8* Start, u8* end) {
    u8* FreeSpace;
    uintptr_t Size;

    Size = ALIGN16(end - Start);
    FreeSpace = (u8*) FreeMemoryPointer;
    DMA(FreeSpace, Start, Size);
    FreeMemoryPointer += Size;
    return FreeSpace;
}

// Unused mio0 decode func.
UNUSED uintptr_t Decodeimage(s32 arg0, s32 arg1, s32 Arg2) {
    uintptr_t Offset;
    UNUSED void* pad;
    uintptr_t oldAddr;
    void* newAddr;

    Offset = ALIGN16(arg1 * Arg2);
    oldAddr = FreeMemoryPointer;
    newAddr = (void*) (oldAddr + Offset);
    pad = &newAddr;
    osInvalDCache(newAddr, Offset);
    osPiStartDma(&DmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], newAddr, Offset,
                 &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, 1);

    slidec((u8*) newAddr, (u8*) oldAddr);
    FreeMemoryPointer += Offset;
    return oldAddr;
}

UNUSED u8* Decodemapimage(u8* arg0, s32 arg1, s32 Arg2) {
    u8* temp_v0;
    void* temp_a0;
    temp_v0 = (u8*) FreeMemoryPointer;
    temp_a0 = temp_v0 + Arg2;
    arg1 = ALIGN16(arg1);
    Arg2 = ALIGN16(Arg2);

    osInvalDCache(temp_a0, arg1);
    osPiStartDma(&DmaIoMesg, 0, 0, (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(arg0)], temp_a0, arg1,
                 &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, 1);
    slidec((u8*) temp_a0, temp_v0);
    FreeMemoryPointer += Arg2;
    return temp_v0;
}

/*
 * Tarmac writes Segment 9 Texture addrs as (trueROM - 0x641F70).
 * HotSwap course loads add that base back in Decodemapimage2 only.
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
    osPiStartDma(&DmaIoMesg, 0, 0,
                 (uintptr_t) &_other_texturesSegmentRomStart[SEGMENT_OFFSET(romaddress)],
                 (void*) temp_a0, romsize, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, (int) 1);
    decodeMIO0((u8*) temp_a0, temp_v0);
    FreeMemoryPointer += ramsize;
    return temp_v0;
}

uintptr_t Decodemapimage2(u8* romaddress, uintptr_t romsize, uintptr_t ramsize) {
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
    osPiStartDma(&DmaIoMesg, 0, 0, romAddr, temp_v0, romsize, &DmaMesgQueue);
    osRecvMesg(&DmaMesgQueue, &MainReceivedMesg, 1);
    decodeMIO0((u8*) temp_v0, (u8*) oldHeapEndPtr);
    LastMemoryPointer += ramsize;
    return oldHeapEndPtr;
}

void Decodevertex2(CourseVtx* data, u32 arg1) {
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
        if (ScreenFlip) {
            vtx->v.ob[0] = -courseVtx->ob[0];
        } else {
            vtx->v.ob[0] = courseVtx->ob[0];
        }

        vtx->v.ob[1] = (courseVtx->ob[1] * ScreenStretch);
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

void Decodevertex(CourseVtx* arg0, u32 vertexCount) {
    u32 segment = SEGMENT_NUMBER2(arg0);
    u32 Offset = SEGMENT_OFFSET(arg0);
    u8*     vtxCompressed = VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);

    decodeMIO0(vtxCompressed, (u8*) FreeMemoryPointer);
    DecodeVertex2OK((char*) FreeMemoryPointer, vertexCount);
    SetSegment(4, (void*) VertexMemoryPointer);
}

UNUSED void Decodematerial(void) {
}

void Decodelight(Gfx* arg0, UNUSED u8* arg1, s8 Arg2) {
    UNUSED s32 pad;
    s32 a = (Arg2 * 0x18) + 0x9000008;
    s32 b = (Arg2 * 0x18) + 0x9000000;
    Gfx macro[] = { gsSPNumLights(NUMLIGHTS_1) };

    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;

    GFXCounter++;
    arg0[GFXCounter].words.w0 = 0x3860010;

    arg0[GFXCounter].words.w1 = a;

    GFXCounter++;
    arg0[GFXCounter].words.w0 = 0x3880010;
    arg0[GFXCounter].words.w1 = b;
    GFXCounter++;
}

void Decodespdisplaylist(Gfx* arg0, u8* args, UNUSED s8 opcode) {
    uintptr_t temp_v0 = args[PKCounter++];
    uintptr_t temp_t7 = ((args[PKCounter++]) << 8 | temp_v0) * 8;
    arg0[GFXCounter].words.w0 = 0x06000000;
    // Segment seven Addr
    arg0[GFXCounter].words.w1 = 0x07000000 + temp_t7;
    GFXCounter++;
}

// end displaylist
void Decodespenddisplaylist(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 Arg2) {
    arg0[GFXCounter].words.w0 = (uintptr_t) (uint8_t) G_ENDDL << 24;
    arg0[GFXCounter].words.w1 = 0;
    GFXCounter++;
}

void Decodesetcullback(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 Arg2) {
    Gfx macro[] = { gsSPSetGeometryMode(G_CULL_BACK) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodeclearcullback(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 Arg2) {
    Gfx macro[] = { gsSPClearGeometryMode(G_CULL_BACK) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodespculldisplaylist(Gfx* arg0, UNUSED u8* arg1, UNUSED s8 Arg2) {
    Gfx macro[] = { gsSPCullDisplayList(0, 7) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodecombine1(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodecombine2(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodecombine3(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodecombine4(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodecombine5(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decoderender1(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decoderender2(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decoderender3(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decoderender4(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_OPA_DECAL, G_RM_AA_ZB_OPA_DECAL) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decoderender5(Gfx* arg0, UNUSED u8* arg1, UNUSED uintptr_t Arg2) {
    Gfx macro[] = { gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL) };
    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodesetup(Gfx* gfx, u8* args, s8 opcode) {
    Gfx tileSync[] = { gsDPTileSync() };
    uintptr_t temp_a0;
    uintptr_t lo;
    uintptr_t hi;

    s32 Width;
    s32 Height;
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
            Width = 32;
            Height = 32;
            fmt = 0;
            break;
        case PG_TILECFG_G:
            Width = 32;
            Height = 32;
            fmt = 0;
            tmem = 256;
            break;
        case PG_TILECFG_B:
            Width = 64;
            Height = 32;
            fmt = 0;
            break;
        case PG_TILECFG_C:
            Width = 32;
            Height = 64;
            fmt = 0;
            break;
        case PG_TILECFG_D:
            Width = 32;
            Height = 32;
            fmt = 3;
            break;
        case PG_TILECFG_E:
            Width = 64;
            Height = 32;
            fmt = 3;
            break;
        case PG_TILECFG_F:
            Width = 32;
            Height = 64;
            fmt = 3;
            break;
    }

    // Set arguments

    siz = G_IM_SIZ_16b_BYTES;
    line = ((((Width * 2) + 7) >> 3));

    temp_a0 = args[PKCounter++];
    cms = temp_a0 & 0xF;
    masks = (temp_a0 & 0xF0) >> 4;

    temp_a0 = args[PKCounter++];
    cmt = temp_a0 & 0xF;
    maskt = (temp_a0 & 0xF0) >> 4;

    // Generate gfx

    gfx[GFXCounter].words.w0 = tileSync->words.w0;
    gfx[GFXCounter].words.w1 = tileSync->words.w1;
    GFXCounter++;

    lo = ((uintptr_t) (uint8_t) G_SETTILE << 24) | (fmt << 21) | (siz << 19) | (line << 9) | tmem;
    hi = ((cmt) << 18) | ((maskt) << 14) | ((cms) << 8) | ((masks) << 4);

    gfx[GFXCounter].words.w0 = lo;
    gfx[GFXCounter].words.w1 = hi;
    GFXCounter++;

    lrs = (Width - 1) << 2;
    lrt = (Height - 1) << 2;

    lo = ((uintptr_t) (uint8_t) G_SETTILESIZE << 24);
    hi = (lrs << 12) | lrt;

    gfx[GFXCounter].words.w0 = lo;
    gfx[GFXCounter].words.w1 = hi;
    GFXCounter++;
}

void Decodeload(Gfx* gfx, u8* args, s8 opcode) {
    UNUSED uintptr_t var;
    Gfx tileSync[] = { gsDPTileSync() };
    Gfx loadSync[] = { gsDPLoadSync() };

    uintptr_t arg;
    uintptr_t lo;
    uintptr_t hi;
    uintptr_t Addr;
    uintptr_t Width;
    uintptr_t Height;
    uintptr_t fmt;
    uintptr_t siz;
    uintptr_t tmem;
    uintptr_t tile;

    switch (opcode) {
        case PG_TIMG_LOADBLOCK_0:
            Width = 32;
            Height = 32;
            fmt = 0;
            break;
        case PG_TIMG_LOADBLOCK_1:
            Width = 64;
            Height = 32;
            fmt = 0;
            break;
        case PG_TIMG_LOADBLOCK_2:
            Width = 32;
            Height = 64;
            fmt = 0;
            break;
        case PG_TIMG_LOADBLOCK_3:
            Width = 32;
            Height = 32;
            fmt = 3;
            break;
        case PG_TIMG_LOADBLOCK_4:
            Width = 64;
            Height = 32;
            fmt = 3;
            break;
        case PG_TIMG_LOADBLOCK_5:
            Width = 32;
            Height = 64;
            fmt = 3;
            break;
    }

    // Set arguments

    // Waa?
    var = args[PKCounter];
    // Generates a Texture Address.
    Addr = SEGMENT_ADDR(0x05, args[PKCounter++] << 11);
    PKCounter++;
    arg = args[PKCounter++];
    siz = G_IM_SIZ_16b;
    tmem = (arg & 0xF);
    tile = (arg & 0xF0) >> 4;

    // Generate gfx

    lo = ((uintptr_t) (uint8_t) G_SETTIMG << 24) | (fmt << 21) | (siz << 19);
    gfx[GFXCounter].words.w0 = lo;
    gfx[GFXCounter].words.w1 = Addr;
    GFXCounter++;

    gfx[GFXCounter].words.w0 = tileSync->words.w0;
    gfx[GFXCounter].words.w1 = tileSync->words.w1;
    GFXCounter++;

    lo = ((uintptr_t) (uint8_t) G_SETTILE << 24) | (fmt << 21) | (siz << 19) | tmem;
    hi = tile << 24;

    gfx[GFXCounter].words.w0 = lo;
    gfx[GFXCounter].words.w1 = hi;
    GFXCounter++;

    gfx[GFXCounter].words.w0 = loadSync->words.w0;
    gfx[GFXCounter].words.w1 = loadSync->words.w1;
    GFXCounter++;

    lo = (uintptr_t) (uint8_t) G_LOADBLOCK << 24;
    hi = (tile << 24) | (MIN((Width * Height) - 1, 0x7FF) << 12) | CALC_DXT(Width, G_IM_SIZ_16b_BYTES);

    gfx[GFXCounter].words.w0 = lo;
    gfx[GFXCounter].words.w1 = hi;
    GFXCounter++;
}

void Decodesptextureon(Gfx* arg0, UNUSED u8* args, UNUSED s8 Arg2) {
    Gfx macro[] = { gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON) };

    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodesptextureoff(Gfx* arg0, UNUSED u8* args, UNUSED s8 Arg2) {
    Gfx macro[] = { gsSPTexture(0x1, 0x1, 0, G_TX_RENDERTILE, G_OFF) };

    arg0[GFXCounter].words.w0 = macro->words.w0;
    arg0[GFXCounter].words.w1 = macro->words.w1;
    GFXCounter++;
}

void Decodespvertex(Gfx* gfx, u8* args, UNUSED s8 Arg2) {
    uintptr_t temp_t7;
    uintptr_t temp_t7_2;

    uintptr_t temp = args[PKCounter++];
    uintptr_t temp2 = ((args[PKCounter++] << 8) | temp) * 0x10;

    temp = args[PKCounter++];
    temp_t7 = temp & 0x3F;
    temp = args[PKCounter++];
    temp_t7_2 = temp & 0x3F;

    gfx[GFXCounter].words.w0 =
        ((uintptr_t) (uint8_t) G_VTX << 24) | (temp_t7_2 * 2 << 16) | (((temp_t7 << 10) + ((0x10 * temp_t7) - 1)));
    gfx[GFXCounter].words.w1 = 0x04000000 + temp2;
    GFXCounter++;
}

void DecodespvertexN(Gfx* gfx, u8* args, s8 Arg2) {
    uintptr_t temp_t9;
    uintptr_t temp_v1;
    uintptr_t temp_v2;

    temp_v1 = args[PKCounter++];
    temp_v2 = ((args[PKCounter++] << 8) | temp_v1) * 0x10;

    temp_t9 = Arg2 - PG_VTX_BASE;

    gfx[GFXCounter].words.w0 = ((uintptr_t) (uint8_t) G_VTX << 24) | ((temp_t9 << 10) + (((temp_t9) * 0x10) - 1));
    gfx[GFXCounter].words.w1 = 0x4000000 + temp_v2;
    GFXCounter++;
}

void Decodesp1Triangle(Gfx* gfx, u8* args, UNUSED s8 Arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_a0;
    uintptr_t phi_a2;
    uintptr_t phi_a3;

    temp_v0 = args[PKCounter++];

    if (ScreenFlip) {
        phi_a3 = temp_v0 & 0x1F;
        phi_a2 = (temp_v0 >> 5) & 7;
        temp_v0 = args[PKCounter++];
        phi_a2 |= (temp_v0 & 3) * 8;
        phi_a0 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_a0 = temp_v0 & 0x1F;
        phi_a2 = (temp_v0 >> 5) & 7;
        temp_v0 = args[PKCounter++];
        phi_a2 |= (temp_v0 & 3) * 8;
        phi_a3 = (temp_v0 >> 2) & 0x1F;
    }
    gfx[GFXCounter].words.w0 = ((uintptr_t) (uint8_t) G_TRI1 << 24);
    gfx[GFXCounter].words.w1 = ((phi_a0 * 2) << 16) | ((phi_a2 * 2) << 8) | (phi_a3 * 2);
    GFXCounter++;
}

void Decodesp2Triangle(Gfx* gfx, u8* args, UNUSED s8 Arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_t0;
    uintptr_t phi_a3;
    uintptr_t phi_a0;
    uintptr_t phi_t2;
    uintptr_t phi_t1;
    uintptr_t phi_a2;

    temp_v0 = args[PKCounter++];

    if (ScreenFlip) {
        phi_t0 = temp_v0 & 0x1F;
        phi_a3 = (temp_v0 >> 5) & 7;
        temp_v0 = args[PKCounter++];
        phi_a3 |= (temp_v0 & 3) * 8;
        phi_a0 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_a0 = temp_v0 & 0x1F;
        phi_a3 = (temp_v0 >> 5) & 7;
        temp_v0 = args[PKCounter++];
        phi_a3 |= (temp_v0 & 3) * 8;
        phi_t0 = (temp_v0 >> 2) & 0x1F;
    }

    temp_v0 = args[PKCounter++];

    if (ScreenFlip) {
        phi_a2 = temp_v0 & 0x1F;
        phi_t1 = (temp_v0 >> 5) & 7;
        temp_v0 = args[PKCounter++];
        phi_t1 |= (temp_v0 & 3) * 8;
        phi_t2 = (temp_v0 >> 2) & 0x1F;
    } else {
        phi_t2 = temp_v0 & 0x1F;
        phi_t1 = (temp_v0 >> 5) & 7;
        temp_v0 = args[PKCounter++];
        phi_t1 |= (temp_v0 & 3) * 8;
        phi_a2 = (temp_v0 >> 2) & 0x1F;
    }
    gfx[GFXCounter].words.w0 =
        ((uintptr_t) (uint8_t) G_TRI2 << 24) | ((phi_a0 * 2) << 16) | ((phi_a3 * 2) << 8) | (phi_t0 * 2);
    gfx[GFXCounter].words.w1 = ((phi_t2 * 2) << 16) | ((phi_t1 * 2) << 8) | (phi_a2 * 2);
    GFXCounter++;
}

void Decodesp1Quadrangle(Gfx* gfx, u8* arg1, UNUSED s8 Arg2) {
    uintptr_t temp_v0;
    uintptr_t phi_a0;
    uintptr_t phi_t0;
    uintptr_t phi_a3;
    uintptr_t phi_a2;

    temp_v0 = arg1[PKCounter++];

    if (ScreenFlip != 0) {
        phi_a0 = temp_v0 & 0x1F;
        phi_a2 = ((temp_v0 >> 5) & 7);
        temp_v0 = arg1[PKCounter++];
        phi_a2 |= ((temp_v0 & 3) * 8);
        phi_a3 = (temp_v0 >> 2) & 0x1F;
        phi_t0 = ((temp_v0 >> 7) & 1);
        temp_v0 = arg1[PKCounter++];
        phi_t0 |= (temp_v0 & 0xF) * 2;
    } else {
        phi_t0 = temp_v0 & 0x1F;
        phi_a3 = ((temp_v0 >> 5) & 7);
        temp_v0 = arg1[PKCounter++];
        phi_a3 |= ((temp_v0 & 3) * 8);
        phi_a2 = (temp_v0 >> 2) & 0x1F;
        phi_a0 = ((temp_v0 >> 7) & 1);
        temp_v0 = arg1[PKCounter++];
        phi_a0 |= (temp_v0 & 0xF) * 2;
    }
    gfx[GFXCounter].words.w0 = ((uintptr_t) (uint8_t) G_QUAD << 24);
    gfx[GFXCounter].words.w1 = ((phi_a0 * 2) << 24) | ((phi_t0 * 2) << 16) | ((phi_a3 * 2) << 8) | (phi_a2 * 2);
    GFXCounter++;
}

UNUSED void DecodeXXXXXXX(void) {
}

/**
 * Unpacks course packed displaylists by iterating through each byte of the packed file.
 * Each packed displaylist entry has an opcode and any Number of arguments.
 * The opcodes range from 0 to 87 which are Used to run the relevant unpack function.
 * The file Pointer increments when arguments are Used. This way,
 * Decodedisplaylist will always read an opcode and not an argument by accident.
 *
 * @warning opcodes that do not contain a definition in the switch are ignored. If an undefined opcode
 * contained arguments the unpacker might try to unpack those arguments.
 * This issue is prevented so long as the packed file adheres to correct opcodes and unpack Code
 * increments the file Pointer the correct Number of times.
 */
void Decodedisplaylist(uintptr_t* data, uintptr_t finalDisplaylistOffset, u32 Arg2) {
    uintptr_t segment = SEGMENT_NUMBER2(data);
    uintptr_t Offset = SEGMENT_OFFSET(data);
    u8* packed_dl = VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);

    Gfx* gfx;
    u32 Addr;

    u8 opcode;

    finalDisplaylistOffset = ALIGN16(finalDisplaylistOffset) + 8;
    LastMemoryPointer -= finalDisplaylistOffset;
    Addr = LastMemoryPointer;
    gfx = (Gfx*) LastMemoryPointer;
    GFXCounter = 0;
    PKCounter = 0;

    while (true) {

        // Seek to the Next byte
        opcode = packed_dl[PKCounter++];

        // Break when the eof has been reached denoted by opcode 0xFF
        if (opcode == 0xFF) {
            break;
        }

        switch (opcode) {
            case PG_LIGHTS_0 + 0x0:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x1:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x2:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x3:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x4:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x5:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x6:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x7:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x8:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x9:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xA:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xB:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xC:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xD:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xE:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0xF:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x10:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x11:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x12:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x13:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_LIGHTS_0 + 0x14:
                Decodelight(gfx, packed_dl, opcode);
                break;
            case PG_SETCOMBINE_CC_MODULATERGBA:
                Decodecombine1(gfx, packed_dl, Arg2);
                break;
            case PG_SETCOMBINE_CC_MODULATERGBDECALA:
                Decodecombine2(gfx, packed_dl, Arg2);
                break;
            case PG_SETCOMBINE_CC_SHADE:
                Decodecombine3(gfx, packed_dl, Arg2);
                break;
            case 0x2E:
                Decodecombine4(gfx, packed_dl, Arg2);
                break;
            case PG_SETCOMBINE_CC_DECALRGBA:
                Decodecombine5(gfx, packed_dl, Arg2);
                break;
            case PG_RMODE_OPA:
                Decoderender1(gfx, packed_dl, Arg2);
                break;
            case PG_RMODE_TEXEDGE:
                Decoderender2(gfx, packed_dl, Arg2);
                break;
            case PG_RMODE_XLU:
                Decoderender3(gfx, packed_dl, Arg2);
                break;
            case PG_RMODE_OPA_DECAL:
                Decoderender4(gfx, packed_dl, Arg2);
                break;
            case PG_RMODE_XLU_DECAL:
                Decoderender5(gfx, packed_dl, Arg2);
                break;
            case PG_TILECFG_A:
                Decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_G:
                Decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_B:
                Decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_C:
                Decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_D:
                Decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_E:
                Decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TILECFG_F:
                Decodesetup(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_0:
                Decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_1:
                Decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_2:
                Decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_3:
                Decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_4:
                Decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TIMG_LOADBLOCK_5:
                Decodeload(gfx, packed_dl, opcode);
                break;
            case PG_TEXTURE_ON:
                Decodesptextureon(gfx, packed_dl, opcode);
                break;
            case PG_TEXTURE_OFF:
                Decodesptextureoff(gfx, packed_dl, opcode);
                break;
            case PG_VTX1:
                Decodespvertex(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x01:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x02:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x03:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x04:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x05:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x06:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x07:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x08:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x09:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0A:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0B:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0C:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0D:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0E:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x0F:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x10:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x11:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x12:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x13:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x14:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x15:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x16:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x17:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x18:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x19:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1A:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1B:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1C:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1D:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1E:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x1F:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_VTX_BASE + 0x20:
                DecodespvertexN(gfx, packed_dl, opcode);
                break;
            case PG_TRI1:
                DecodeSP1TriangleOK(gfx, packed_dl, opcode);
                break;
            case PG_TRI2:
                DecodeSP2TriangleOK(gfx, packed_dl, opcode);
                break;
            case PG_SPLINE3D:
                DecodeSP1QuadrangleOK(gfx, packed_dl, opcode);
                break;
            case PG_CULLDL:
                Decodespculldisplaylist(gfx, packed_dl, opcode);
                break;
            case PG_ENDDL:
                Decodespenddisplaylist(gfx, packed_dl, opcode);
                break;
            case PG_SETGEOMETRYMODE:
                Decodesetcullback(gfx, packed_dl, opcode);
                break;
            case PG_CLEARGEOMETRYMODE:
                Decodeclearcullback(gfx, packed_dl, opcode);
                break;
            case PG_DL:
                Decodespdisplaylist(gfx, packed_dl, opcode);
                break;
            default:
                // Skip Unknown values
                break;
        }
    }
    SetSegment(0x7, (void*) Addr);
}

struct UnkStr_802AA7C8 {
    u8* Unk0;
    uintptr_t unk4;
    uintptr_t Unk8;
    uintptr_t UnkC;
};

void Decodemaptexture(u32* arg0) {
    u32 segment = SEGMENT_NUMBER2(arg0);
    u32 Offset = SEGMENT_OFFSET(arg0);
    struct UnkStr_802AA7C8* phi_s0 = (struct UnkStr_802AA7C8*) VIRTUAL_TO_PHYSICAL2(SegmentTable[segment] + Offset);
    struct UnkStr_802AA7C8* temp_s0;
    uintptr_t temp_t2;
    u8* temp_a0;
    uintptr_t phi_v0;
    uintptr_t sp20;

    phi_v0 = 0;
    temp_s0 = phi_s0;
    while (true) {
        temp_a0 = phi_s0->Unk0;
        if ((temp_a0) == 0) {
            break;
        }
        phi_v0 += phi_s0->Unk8;
        phi_s0++;
    }
    phi_s0 = temp_s0;
    LastMemoryPointer -= phi_v0;
    sp20 = LastMemoryPointer;

    while (true) {
        temp_a0 = phi_s0->Unk0;
        if ((temp_a0) == 0) {
            break;
        }
        Decodemapimage2(temp_a0, phi_s0->unk4, phi_s0->Unk8);
        phi_s0++;
    }
    LastMemoryPointer = sp20;
    temp_t2 = LastMemoryPointer;
    SetSegment(0x5, (void*) temp_t2);
}

void* LoadPressData(u8* Start, u8* end) {
    UNUSED u32 pad;
    u32 sp28;
    u32 Size = ALIGN16(end - Start);
    u8* heapEnd;
    u32* FreeSpace;

    heapEnd = (u8*) LastMemoryPointer - Size;
    // sp20 = temp_a0;
    DMA(heapEnd, Start, Size);
    sp28 = *(u32*) (heapEnd + 4);
    sp28 = ALIGN16(sp28);
    FreeSpace = (u32*) FreeMemoryPointer;
    decodeMIO0(heapEnd, (u8*) FreeSpace);
    FreeMemoryPointer += sp28;
    return (void*) FreeSpace;
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
    faceEnd = (uintptr_t) FaceBuffer + (CourseFaceMaxIndex * sizeof(FaceStruct));
    if (faceEnd >= FACE_BUFFER_END) {
        while (1) {}
    }
    if ((faceEnd > FACE_BUFFER_START) && (faceEnd > VertexMemoryPointer)) {
        while (1) {}
    }
    if ((GameState == ENDING) || (GameState == CREDITS_SEQUENCE)) {
        if (LastMemoryPointer < MEMORY_POOL_END) {
            while (1) {}
        }
    } else if (LastMemoryPointer < LAST_MEMORY_FLOOR) {
        while (1) {}
    }
}

/**
 * @brief Loads & DMAs course data. Vtx, Textures, displaylists, etc.
 * @Param courseId
 *
 * Order: throwaway geography (0xF) on FreeMemory, decode verts (4) from VertexMemoryPointer
 * and packed DLs (7) from LastMemoryPointer, rewind FreeMemory (drops 0xF), eat-own-tail
 * MIO0 course data (6), offsets (9), then Map Textures (5).
 * Segment 9 must be live before Decodemaptexture — the Texture Table is linked at 0x09000000.
 */
u8* LoadCourse(s32 courseId) {
    u8* vtxCompressed;
    u8* courseDataRomStart;
    u8* courseDataRomEnd;
    u8* vertexRomStart;
    u8* vertexRomEnd;
    u32* Textures;
    CourseVtx* vertexStart;
    u8* packedStart;
    u32 vertexCount;
    u8* finalDisplaylistOffset;
    u32 Unknown1;
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
    Textures = g_courseTable[courseId].Textures;
    vertexStart = g_courseTable[courseId].vertexStart;
    packedStart = g_courseTable[courseId].packedStart;
    vertexCount = g_courseTable[courseId].vertexCount;
    finalDisplaylistOffset = g_courseTable[courseId].finalDisplaylistOffset;
    Unknown1 = g_courseTable[courseId].Unknown1;

    if (GameMode == GRAND_PRIX) {
        if ((GPCourseIndex > AsmCupCount) && (HotSwapID > 0)) {
            HotSwapGP = HotSwapID;
            HotSwapID = 0;
        }
    }

    if ((GameState == ENDING) || (GameState == CREDITS_SEQUENCE)) {
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

    Decodevertex(vertexStart, vertexCount);
    Decodedisplaylist((uintptr_t*) packedStart, (uintptr_t) finalDisplaylistOffset, Unknown1);

    FreeMemoryPointer = StaticMemoryPointer;

    if (GameState != ENDING) {
        compSize = ALIGN16((uintptr_t) courseDataRomEnd - (uintptr_t) courseDataRomStart);
        compDest = (u8*) (MEMORY_POOL_END - compSize);
        DMA(compDest, courseDataRomStart, compSize);
        FreeMemoryPointer = ALIGN16((uintptr_t) decodeMIO0(compDest, (u8*) StaticMemoryPointer));
        SetSegment(6, (void*) StaticMemoryPointer);
    }

    SetSegment(9, LoadSegment((uintptr_t) offsetRomStart, (uintptr_t) offsetRomEnd));
    Decodemaptexture(Textures);
    CheckCourseMemoryHighWater();
    return vtxCompressed;
}
