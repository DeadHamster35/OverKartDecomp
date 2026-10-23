#ifndef MAININCLUDE_H
#define MAININCLUDE_H

/*
 * Vanilla types come from decomp headers (Library Names, stock IDO Shape).
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
#define GlobalPlayer GamePlayers
#include <decode.h>
#include "memory.h"
#include <string.h>
#include <math.h>
#define Sqrtf sqrtf
#define sinF sinf
#define cosF cosf
#define fabsf fabs
#include <defines.h>

typedef s64 Int64;
typedef u64 Uint64;
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
#include "kart_attributes.h"
#include "menus.h"
#include "audio/external.h"
#include <assets/common_data.h>

#include "CustomCourses/CustomLevels.h"
#include "CustomCourses/CustomSurfaces.h"

#include "CustomObjects/OKBehaviors.h"
#include "CustomObjects/OKCustomObjects.h"
#include "CustomObjects/OKAnime.h"

#include "CustomParticles/ParticleStruct.h"
#include "CustomParticles/CustomParticles.h"

#include "GameVariables/NTSC/4.1Header.h"

/* Decomp owners for Class A names removed from GameOffsets.h in Stage 1.5b. */
#include "render_player.h"
#include "code_8003DC40.h"
#include "race_logic.h"
#include "buffers/trig_tables.h"
#include "code_80280000.h"
#include "actors_extended.h"
#include "buffers.h"
#include "code_80086E70.h"
#include "profiler.h"
#include "replays.h"
#include "collision.h"
#include "animation.h"
#include "podium_ceremony_actors.h"
#include "math_util.h"
#include "effects.h"
#include "player_controller.h"
#include "actors.h"
#include "course.h"
#include "bomb_kart.h"
#include "render_courses.h"
#include "skybox_and_splitscreen.h"
#include "spawn_players.h"
#include "math_util_2.h"
#include "cpu_vehicles_camera_path.h"
#include "update_objects.h"
#include "code_8006E9C0.h"
#include "code_80057C60.h"
#include "render_objects.h"

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

#include "Gametypes/GameTypesGeneric.h"

#include "LibraryVariables.h"
#include "MarioKart3D.h"
#include "MarioKartMenu.h"
#include "MarioKartObjects.h"
#include "OKExternal.h"
#include "OKHeader.h"
#include "OKRumble.h"
#include "SharedFunctions.h"
#include "OverKartDefaults.h"
#include "SubProgram.h"

#include "LapCounters.h"

#endif
