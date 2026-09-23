#ifndef COURSE_OFFSETS_H
#define COURSE_OFFSETS_H

#include <PR/ultratypes.h>

typedef struct {
    u8* Addr;      // segmented Address Texture file
    u32 FileSize; // compressed file Size
    u32 DataSize; // uncompressed Texture Size
    u32 Padding;   // always zero
} CourseTexture;

#endif // COURSE_OFFSETS_H
