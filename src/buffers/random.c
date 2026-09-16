#include <segments.h>

u16 g_RNG;

#ifdef VERSION_EU
u8 randomSeedPadding[16];
#else
u8 randomSeedPadding[216];
#endif
