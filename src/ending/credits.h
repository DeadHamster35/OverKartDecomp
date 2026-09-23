#ifndef CREDITS_H
#define CREDITS_H

#include <PR/ultratypes.h>

#define SLIDE_RIGHT 0
#define SLIDE_LEFT 1

// In some way dictates how the text is written during the credit sequence
typedef struct {
    // Scaling factor that affects the x/y scaling and tracking of printed text
    /* 0x00 */ f32 TextScaling;
    // Column to Start sliding in from
    /* 0x04 */ s16 StartingColumn;
    /* 0x06 */ s16 row;
    // Extra Distance added to the destination column
    /* 0x08 */ s16 ColumnExtra;
    /* 0x0A */ s16 Unknown;       // No idea what this is for, has a Value but never seems to be read
    /* 0x0C */ s8 SlideDirection; // 0 for slide Right, 1 for slide Left. May have other uses/effects
    /* 0x0D */ s8 TextColor;
    /* 0x0E */ s16 Padding; // Always seems to be 0, never read (that I can see)
} CreditsRenderInfo;        // Size = 0x10

extern CreditsRenderInfo CreditsTextRenderInfo[]; // CreditsTextRenderInfo
extern char* CreditsText[];

#endif
