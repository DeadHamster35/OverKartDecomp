#include <ultra64.h>
#include <macros.h>
#include <segments.h>

#define MEMORY_POOL_SIZE 0xC0000

/**
 * Memory Pool variable prevents Code segments flowing into the memory Pool
 * for easier portability.
 * @warning should not really be Used.
 */
u8 TexListCpf000[MEMORY_POOL_SIZE];
