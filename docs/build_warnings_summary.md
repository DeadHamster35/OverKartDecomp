# OverKartDecomp build warnings summary

**Build:** 2026-10-25 10:46 ET (MobileFOB, `sh _build_run.sh`)
**Result:** SUCCESS (MAKE_EXIT:0), 65 files compiled, ROM linked (identical to previous)
**ROM:** `build/us/OverKart_V8.z64` SHA1 `e1b3358af54473bce94afd173b79e47e3b1d885e` (previous `e1b3358af54473bce94afd173b79e47e3b1d885e`)

## Totals
- GCC warnings: 67
- Compile errors: 0
- Undefined references: 0
- Multiple definitions: 0
- Linker warnings: 0

## Warnings by file
| File | Count |
|---|---|
| `src/OverKart5/OKMenu.c` | 12 |
| `src/OverKartLibrary/CustomObjects/ObjectHandler.c` | 8 |
| `src/OverKartLibrary/Player/Pathfinding.c` | 6 |
| `build/us/src/cpu_vehicles_camera_path.jp.c` | 6 |
| `src/math_util_2.c` | 4 |
| `src/OverKartLibrary/CustomCourses/CustomLevels.c` | 4 |
| `src/code_80086E70.c` | 3 |
| `src/OverKartLibrary/CustomCourses/CustomSurfaces.c` | 3 |
| `src/update_objects.c` | 2 |
| `src/OverKartLibrary/MarioKart3D.c` | 2 |
| `src/OverKartLibrary/Gametypes/GameTypesGeneric.c` | 2 |
| `src/OverKart5/OverKartObjectHandler.c` | 2 |
| `src/player_controller.c` | 1 |
| `src/render_objects.c` | 1 |
| `src/racing/actors_extended.c` | 1 |
| `src/actors/green_shell/update.inc.c` | 1 |
| `src/OverKartLibrary/MarioKartObjects.c` | 1 |
| `src/OverKartLibrary/SharedFunctions.c` | 1 |
| `src/OverKartLibrary/CustomObjects/OKAnime.c` | 1 |
| `src/OverKartLibrary/Player/MarioKartStats.c` | 1 |
| `src/OverKart5/OverKart.c` | 1 |
| `src/OverKart5/Gametypes/CTF.c` | 1 |
| `src/OverKart5/Gametypes/Soccer.c` | 1 |
| `src/cpu_vehicles_camera_path/cpu_speed_control.inc.c` | 1 |
| `src/cpu_vehicles_camera_path/bomb_kart.inc.c` | 1 |

## Warnings by type
| Type | Count |
|---|---|
| `-Wunused-parameter` | 37 |
| `-Wsign-compare` | 10 |
| `-Wunused-variable` | 7 |
| `-Wtype-limits` | 6 |
| `-Wreturn-local-addr` | 3 |
| `-Wincompatible-pointer-types` | 2 |
| `-Wunused-value` | 2 |

## IDO cfe notices (non-fatal)
- `src/OverKartLibrary/OKRumble.h`: 210
- `src/OverKartLibrary/OKRumble.c`: 115
- `src/OverKartLibrary/CustomObjects/../OKRumble.h`: 40
- `src/OverKartLibrary/Player/../OKRumble.h`: 40
- `src/OverKartLibrary/MusicSound/../OKRumble.h`: 20
- `src/OverKartLibrary/CustomCourses/../OKRumble.h`: 20
- `src/OverKart5/OverKart.c`: 14
- `src/OverKartLibrary/CustomParticles/../OKRumble.h`: 10
- `src/OverKartLibrary/Gametypes/../OKRumble.h`: 10
- `src/OverKart5/OKMenu.c`: 7
- `../Protec/Cheat_ido.c`: 6
- `src/update_objects.c`: 2
- `src/racing/memory.c`: 1
- `src/racing/actors.c`: 1
- `src/OverKart5/MarioKartPractice.c`: 1

## All warnings
```
src/code_80086E70.c:1284:18: comparison is always true due to limited range of data type [-Wtype-limits]
src/code_80086E70.c:1300:18: comparison is always true due to limited range of data type [-Wtype-limits]
src/code_80086E70.c:1315:18: comparison is always true due to limited range of data type [-Wtype-limits]
src/math_util_2.c:345:12: function returns address of local variable [-Wreturn-local-addr]
src/math_util_2.c:354:12: function returns address of local variable [-Wreturn-local-addr]
src/math_util_2.c:363:12: function returns address of local variable [-Wreturn-local-addr]
src/math_util_2.c:461:18: comparison is always true due to limited range of data type [-Wtype-limits]
src/player_controller.c:757:38: comparison is always true due to limited range of data type [-Wtype-limits]
src/update_objects.c:7813:31: passing argument 2 of 'KwanmInitCI8' from incompatible pointer type [-Wincompatible-pointer-types]
src/render_objects.c:2948:13: unused variable 'Dummy' [-Wunused-variable]
src/update_objects.c:7814:25: passing argument 3 of 'KwanmInitCI8' from incompatible pointer type [-Wincompatible-pointer-types]
src/racing/actors_extended.c:849:18: left-hand operand of comma expression has no effect [-Wunused-value]
src/actors/green_shell/update.inc.c:186:37: left-hand operand of comma expression has no effect [-Wunused-value]
src/OverKartLibrary/MarioKart3D.c:424:42: unused parameter 'Distance' [-Wunused-parameter]
src/OverKartLibrary/MarioKart3D.c:477:21: unused parameter 'Distance' [-Wunused-parameter]
src/OverKartLibrary/MarioKartObjects.c:14:78: unused parameter 'localVelocity' [-Wunused-parameter]
src/OverKartLibrary/SharedFunctions.c:366:52: unused parameter 'v_buff' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/OKAnime.c:112:40: unused parameter 'Scale' [-Wunused-parameter]
src/OverKartLibrary/CustomCourses/CustomLevels.c:1125:58: unused parameter 'Code' [-Wunused-parameter]
src/OverKartLibrary/CustomCourses/CustomLevels.c:1149:58: unused parameter 'Code' [-Wunused-parameter]
src/OverKartLibrary/CustomCourses/CustomLevels.c:1192:60: unused parameter 'Code' [-Wunused-parameter]
src/OverKartLibrary/CustomCourses/CustomLevels.c:1226:28: unused parameter 'ramaddress' [-Wunused-parameter]
src/OverKartLibrary/CustomCourses/CustomSurfaces.c:722:29: unused parameter 'Null' [-Wunused-parameter]
src/OverKartLibrary/CustomCourses/CustomSurfaces.c:802:56: unused parameter 'pos_F' [-Wunused-parameter]
src/OverKartLibrary/CustomCourses/CustomSurfaces.c:1187:46: unused parameter 'Camera' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:5:47: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:13:38: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:21:39: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:28:36: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:36:37: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:43:34: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:49:42: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/CustomObjects/ObjectHandler.c:56:42: unused parameter 'Kno' [-Wunused-parameter]
src/OverKartLibrary/Player/Pathfinding.c:201:83: unused parameter 'TargetY' [-Wunused-parameter]
src/OverKartLibrary/Player/Pathfinding.c:201:120: unused parameter 'MarkerCounts' [-Wunused-parameter]
src/OverKartLibrary/Player/Pathfinding.c:247:103: unused parameter 'MarkerCounts' [-Wunused-parameter]
src/OverKartLibrary/Player/Pathfinding.c:313:53: unused parameter 'FirstMarkerDistance' [-Wunused-parameter]
src/OverKartLibrary/Player/Pathfinding.c:731:38: unused parameter 'TargetPlayer' [-Wunused-parameter]
src/OverKartLibrary/Player/Pathfinding.c:767:39: unused parameter 'TargetPosition' [-Wunused-parameter]
src/OverKartLibrary/Player/MarioKartStats.c:322:22: unused parameter 'Mode' [-Wunused-parameter]
src/OverKartLibrary/Gametypes/GameTypesGeneric.c:165:27: unused parameter 'LocalCamera' [-Wunused-parameter]
src/OverKartLibrary/Gametypes/GameTypesGeneric.c:182:28: unused parameter 'LocalCamera' [-Wunused-parameter]
src/OverKart5/OKMenu.c:411:101: comparison of integer expressions of different signedness: 'char' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:439:40: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:477:40: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:497:40: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:551:35: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:561:66: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:581:66: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:1311:30: comparison of integer expressions of different signedness: 'short int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OKMenu.c:1465:29: unused parameter 'StatsMode' [-Wunused-parameter]
src/OverKart5/OKMenu.c:1677:36: unused parameter 'pad' [-Wunused-parameter]
src/OverKart5/OKMenu.c:1677:51: unused parameter 'newbutton' [-Wunused-parameter]
src/OverKart5/OKMenu.c:1750:30: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/OverKartObjectHandler.c:95:36: unused parameter 'HitType' [-Wunused-parameter]
src/OverKart5/OverKartObjectHandler.c:304:26: unused parameter 'Car' [-Wunused-parameter]
src/OverKart5/OverKart.c:1260:39: comparison of integer expressions of different signedness: 'int' and 'Uint' {aka 'unsigned int'} [-Wsign-compare]
src/OverKart5/Gametypes/CTF.c:98:65: unused parameter 'BattleBaseF3D' [-Wunused-parameter]
src/OverKart5/Gametypes/Soccer.c:82:42: unused parameter 'BattleBaseF3D' [-Wunused-parameter]
src/cpu_vehicles_camera_path/cpu_speed_control.inc.c:129:34: unused parameter 'targetSpeed' [-Wunused-parameter]
build/us/src/cpu_vehicles_camera_path.jp.c:969:10: unused variable 'var_v1' [-Wunused-variable]
build/us/src/cpu_vehicles_camera_path.jp.c:962:10: unused variable 'var_a0_3' [-Wunused-variable]
src/cpu_vehicles_camera_path/bomb_kart.inc.c:218:31: comparison is always false due to limited range of data type [-Wtype-limits]
build/us/src/cpu_vehicles_camera_path.jp.c:2840:9: unused variable 'stackPadding3' [-Wunused-variable]
build/us/src/cpu_vehicles_camera_path.jp.c:2839:9: unused variable 'stackPadding2' [-Wunused-variable]
build/us/src/cpu_vehicles_camera_path.jp.c:2835:9: unused variable 'stackPadding1' [-Wunused-variable]
build/us/src/cpu_vehicles_camera_path.jp.c:2834:9: unused variable 'stackPadding0' [-Wunused-variable]
```
