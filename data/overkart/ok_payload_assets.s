# OverKart Library payload blobs (RAM after init_overkart).
# Do not assemble src/OverKartLibrary/*.s leftover GCC dumps.

.include "macros.inc"

.section .data

    .balign 16
glabel Unknown33
.incbin "assets/OverKartLibrary/stats/Unknown33.bin"

    .balign 16
glabel Unknown41
.incbin "assets/OverKartLibrary/stats/Unknown41.bin"

    .balign 16
glabel AccelerationTable
.incbin "assets/OverKartLibrary/stats/AccelerationTable.bin"

    .balign 256
glabel LapCounterTextures
.incbin "assets/OverKartLibrary/LapCounters.RAW"
glabel LapCounterPalette

    .balign 16
glabel bannerN
.incbin "assets/OverKartLibrary/banner_n.mio0.bin"

    .balign 16
glabel bannerU
.incbin "assets/OverKartLibrary/banner_U.mio0.bin"

    .balign 16
glabel ParticlePopIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Pop_IA16_raw.bin"

    .balign 16
glabel ParticleMudSplatIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_MudSplat_IA16_raw.bin"

    .balign 16
glabel ParticleSparkleIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Sparkle_IA16_raw.bin"

    .balign 16
glabel ParticleSmokePuffIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_SmokePuff_IA16_raw.bin"

    .balign 16
glabel ParticlePuffIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Puff_IA16_raw.bin"

    .balign 16
glabel ParticleExplosionRGBA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Explosion_RGBA16_raw.bin"

    .balign 16
glabel ParticleBubblePopIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_BubblePop_IA16_raw.bin"

    .balign 16
glabel ParticlePebbleIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Pebble_IA16_raw.bin"

    .balign 16
glabel ParticleBubbleIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Bubble_IA16_raw.bin"

    .balign 16
glabel ParticleSplatterIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Splatter_IA16_raw.bin"

    .balign 16
glabel ParticleDirtIA16
.incbin "assets/OverKartLibrary/ParticleFiles/Particle_Dirt_IA16_raw.bin"

    .balign 16
