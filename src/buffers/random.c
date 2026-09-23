#include <segments.h>

u16 RNG;

#ifdef VERSION_EU
u8 RandomSeedPadding[16];
#else
u8 RandomSeedPadding[216];
#endif
