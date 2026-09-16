#include <ultra64.h>
#include <macros.h>
#include <segments.h>

#define MEMORY_POOL_SIZE 0xC0000

/**
 * Memory pool variable prevents code segments flowing into the memory pool
 * for easier portability.
 * @warning should not really be used.
 */
u8 TexList_cpf_000[MEMORY_POOL_SIZE];
