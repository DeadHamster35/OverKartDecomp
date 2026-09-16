#ifndef MAININCLUDE_H
#define MAININCLUDE_H

/*
 * Vanilla types come from decomp headers (Library names, stock IDO shape).
 * F3DEX_GBI / F3D_OLD are set by the Makefile. Do not pull host stdbool/math.
 */
#include <ultra64.h>
#include <mk64.h>
#include <common_structs.h>
#include <objects.h>
#include <actor_types.h>
#include <path.h>
#include "camera.h"
#include "code_800029B0.h"
#include "main.h"
#include <decode.h>
#include "memory.h"
#include <string.h>
#include <defines.h>

typedef s64 int64;
typedef u64 uint64;
typedef Vec3f FVector;
typedef struct GfxPool Dynamic;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#include "Struct.h"
#include "OKStruct.h"

#include "GameVariables/NTSC/StockAliases.h"

#include "CustomCourses/CustomLevels.h"
#include "CustomCourses/CustomSurfaces.h"

#include "CustomObjects/OKBehaviors.h"
#include "CustomObjects/OKCustomObjects.h"
#include "CustomObjects/OKAnime.h"

#include "CustomParticles/ParticleStruct.h"
#include "CustomParticles/CustomParticles.h"

#include "GameVariables/NTSC/4.1Header.h"
#include "GameVariables/NTSC/GameOffsets.h"
#include "GameVariables/NTSC/OKAssembly.h"
#include "GameVariables/NTSC/StatsOffsets.h"

#include "MusicSound/CustomSounds.h"
#include "MusicSound/MusicControl.h"

#include "Player/MarioKartStats.h"
#include "Player/PlayerChecks.h"
#include "Player/PlayerEffects.h"
#include "Player/PlayerPadding.h"
#include "Player/Pathfinding.h"

#include "Gametypes/CTF.h"
#include "Gametypes/GameTypesGeneric.h"
#include "Gametypes/Slayer.h"
#include "Gametypes/Soccer.h"

#include "LibraryVariables.h"
#include "MarioKart3D.h"
#include "MarioKartMenu.h"
#include "MarioKartObjects.h"
#include "OKExternal.h"
#include "OKHeader.h"
#include "SharedFunctions.h"
#include "OverKartDefaults.h"
#include "SubProgram.h"

#include "LapCounters.h"

#endif
