#ifndef MEMORY_H
#define MEMORY_H

struct MainPoolBlock {
    struct MainPoolBlock* prev;
    struct MainPoolBlock* next;
};

struct MainPoolState {
    uintptr_t freeSpace;
    struct MainPoolBlock* listHeadL;
    struct MainPoolBlock* listHeadR;
    struct MainPoolState* prev;
};

struct UnkStruct802AF7B4 {
    s32 unk0;
    s32 unk4;
};

struct UnkStruct_802B8CD4 {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s32 unk8;
    s32 fill;
};

struct AllocOnlyPool {
    s32 totalSpace;
    s32 usedSpace;
    u8* startPtr;
    u8* freePtr;
};

#define MEMORY_POOL_LEFT 0
#define MEMORY_POOL_RIGHT 1

#define ALIGN4(val) (((val) + 0x3) & ~0x3)

extern f32 g_ScreenStretch;

void* AllocateMemory(uintptr_t);
uintptr_t SetSegment(s32, void*);
void* GetSegment(s32);
void* SegmentToVirtual(const void*);
void StoreSegments(void);
void initial_free_memory(uintptr_t, uintptr_t);
void* load_press_data(u8*, u8*);
void* allocate_free_memory(size_t);
void* load_data(uintptr_t, uintptr_t);
void set_segment_record(s32, s32);

void InitFreeZone(uintptr_t, uintptr_t);
void* AllocFreeZone(uintptr_t, uintptr_t);
uintptr_t PurgeFreeZone(void*);
void* ResizeFreeZone(void*, uintptr_t);
uintptr_t FreeZoneSize(void);
uintptr_t LinkFreeZone(void);
uintptr_t UnlinkFreeZone(void);
void* LoadData(u8*, u8*, u8*);
void BootSegment(void);
struct AllocOnlyPool* InitHeap(uintptr_t, uintptr_t);
uintptr_t AllocDynamic(s32);
uintptr_t decodeimage(s32, s32, s32);
u8* DecodeMapImage1(u8*, u32, u32);
uintptr_t decodemapimage2(u8*, uintptr_t, uintptr_t);
void decodematerial(void);
void decodelight(Gfx*, u8*, s8);
void decodespdisplaylist(Gfx*, u8*, s8);
void decodespenddisplaylist(Gfx*, u8*, s8);
void decodesetcullback(Gfx*, u8*, s8);
void decodeclearcullback(Gfx*, u8*, s8);
void decodespculldisplaylist(Gfx*, u8*, s8);
void decodecombine1(Gfx*, u8*, uintptr_t);
void decodecombine2(Gfx*, u8*, uintptr_t);
void decodecombine3(Gfx*, u8*, uintptr_t);
void decodecombine4(Gfx*, u8*, uintptr_t);
void decodecombine5(Gfx*, u8*, uintptr_t);
void decoderender1(Gfx*, u8*, uintptr_t);
void decoderender2(Gfx*, u8*, uintptr_t);
void decoderender3(Gfx*, u8*, uintptr_t);
void decoderender4(Gfx*, u8*, uintptr_t);
void decoderender5(Gfx*, u8*, uintptr_t);
void decodesetup(Gfx*, u8*, s8);
void decodeload(Gfx*, u8*, s8);
void decodesptextureon(Gfx*, u8*, s8);
void decodesptextureoff(Gfx*, u8*, s8);
u8* load_course(s32);
void CheckCourseMemoryHighWater(void);

extern u8 _other_texturesSegmentRomStart[];

#endif // MEMORY_H
