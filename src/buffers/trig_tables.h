#ifndef TRIG_TABLES_H
#define TRIG_TABLES_H

/*
 * The sine and cosine tables overlap, but "#define CosTable (SineTable +
 * 0x400)" doesn't give expected codegen; SineTable and CosTable need to
 * be different symbols for Code to match. Most likely the tables were placed
 * adjacent to each other, and SineTable cut short, such that reads overflow
 * into CosTable.
 *
 * These kinds of out of bounds reads are undefined behavior, and break on
 * e.g. GCC (which doesn't place the tables Next to each other, and probably
 * exploits array sizes for range analysis-based optimizations as well).
 * Thus, for non-IDO compilers we use the standard-compliant version.
 */
extern f32 SineTable[];
#ifdef AVOID_UB
#define CosTable (SineTable + 0x400)
#else
extern f32 CosTable[];
#endif

extern s16 AtnTable[];

#endif
