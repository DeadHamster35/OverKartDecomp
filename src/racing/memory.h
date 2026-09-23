#ifndef MEMORY_H
#define MEMORY_H

struct MainPoolBlock {
    struct MainPoolBlock* Prev;
    struct MainPoolBlock* Next;
};

struct MainPoolState {
    uintptr_t FreeSpace;
    struct MainPoolBlock* ListHeadL;
    struct MainPoolBlock* ListHeadR;
    struct MainPoolState* Prev;
};

struct UnkStruct802AF7B4 {
    s32 Unk0;
    s32 unk4;
};

struct UnkStruct_802B8CD4 {
    s16 Unk0;
    s16 Unk2;
    s32 unk4;
    s32 Unk8;
    s32 Fill;
};

struct AllocOnlyPool {
    s32 TotalSpace;
    s32 UsedSpace;
    u8* StartPtr;
    u8* FreePtr;
};

#define MEMORY_POOL_LEFT 0
#define MEMORY_POOL_RIGHT 1

#define ALIGN4(val) (((val) + 0x3) & ~0x3)

extern f32 ScreenStretch;

void* AllocateMemory(uintptr_t);
uintptr_t SetSegment(s32, void*);
void* GetSegment(s32);
void* SegmentToVirtual(const void*);
void StoreSegments(void);
void InitialFreeMemory(uintptr_t, uintptr_t);
void* LoadPressData(u8*, u8*);
void* AllocateFreeMemory(size_t);
void* LoadSegment(uintptr_t, uintptr_t);
void SetSegmentRecord(s32, s32);

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
uintptr_t Decodeimage(s32, s32, s32);
u8* DecodeMapImage1(u8*, uintptr_t, uintptr_t);
uintptr_t Decodemapimage2(u8*, uintptr_t, uintptr_t);
void Decodematerial(void);
void Decodelight(Gfx*, u8*, s8);
void Decodespdisplaylist(Gfx*, u8*, s8);
void Decodespenddisplaylist(Gfx*, u8*, s8);
void Decodesetcullback(Gfx*, u8*, s8);
void Decodeclearcullback(Gfx*, u8*, s8);
void Decodespculldisplaylist(Gfx*, u8*, s8);
void Decodecombine1(Gfx*, u8*, uintptr_t);
void Decodecombine2(Gfx*, u8*, uintptr_t);
void Decodecombine3(Gfx*, u8*, uintptr_t);
void Decodecombine4(Gfx*, u8*, uintptr_t);
void Decodecombine5(Gfx*, u8*, uintptr_t);
void Decoderender1(Gfx*, u8*, uintptr_t);
void Decoderender2(Gfx*, u8*, uintptr_t);
void Decoderender3(Gfx*, u8*, uintptr_t);
void Decoderender4(Gfx*, u8*, uintptr_t);
void Decoderender5(Gfx*, u8*, uintptr_t);
void Decodesetup(Gfx*, u8*, s8);
void Decodeload(Gfx*, u8*, s8);
void Decodesptextureon(Gfx*, u8*, s8);
void Decodesptextureoff(Gfx*, u8*, s8);
u8* LoadCourse(s32);
void CheckCourseMemoryHighWater(void);

extern u8 _other_texturesSegmentRomStart[];

#endif // MEMORY_H
