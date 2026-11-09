#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int PcRomLoad(const char* path);
const uint8_t* PcRomData(void);
size_t PcRomSize(void);
void* PcRomPtr(uint32_t romOffset);
int PcDmaFromRom(void* dest, uint32_t romOffset, size_t nbytes);

/* Map a DMA source to a ROM file offset. Returns 1 if dest should be filled
 * from the loaded ROM. Never treats a 32-bit N64 KSEG address as a host pointer. */
int PcRomResolveSrc(uintptr_t devAddr, const uint8_t** srcOut, size_t nbytes);

#ifdef __cplusplus
}
#endif
