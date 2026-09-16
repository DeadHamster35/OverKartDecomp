# Hosting a project on OverKart Library (LibraryDecomp)

This tree is a mk64 decomp with OverKart Library linked natively (one IDO toolchain). Vanilla C already calls Library. A **host** (OverKart5, Amped Up, a new project) supplies UI, KartCheck/CameraCheck, and any extra objects — it does not re-do Armips ROM hooks.

Start-here for hosts: **this file**. Source of the defaults: `src/OverKartLibrary/OverKartDefaults.c`. Vanilla-safe decls: `src/OverKartLibrary/OverKartHooks.h` (vanilla C must not include `MainInclude.h`).

Sibling `Library/` is the historical GCC/Armips copy. `KimuraDecomp` is the frozen matching tree. Do not edit either from here.

NTSC (USA) only for custom courses. Tarmac `ok_HeaderROM` stays `0xBE9178`.

## Build

```
make AVOID_UB=1 COMPARE=0 -j
```

Those are the Makefile defaults. Compare ROM is not a goal. `make AMPEDUP_FONT=1` selects the Amped Up big-font blob; default `0` is Library `big_font.mio0.bin`. Need `baserom.us.z64` in the repo root.

## How a host overrides

Vanilla already JALs the `*Default` names. There is one ELF, so there can be only one definition of each.

Replace **`src/OverKartLibrary/OverKartDefaults.c`** (keep the same function names and signatures) or drop that `.o` from the link and supply your own. Do not add a second `gameCode` next to `gameCodeDefault`.

Empty bodies (`titleMenuDefault`, `DisplayObjectDefault`, `PrintMenuTest`) are intentional. The host fills them.

Library C uses the short names via `StockAliases.h` (Library TUs only — vanilla C includes `OverKartHooks.h`):

| Library name | Resolves to |
|---|---|
| `gameCode` | `gameCodeDefault` |
| `titleMenu` | `titleMenuDefault` |
| `allRun` / `GlobalCustomCode` | `allRunDefault` |
| `PrintMenuFunction` | `PrintMenuFunctionDefault` |
| `DisplayObject` | `DisplayObjectDefault` |
| `CollideObject` | `CollideObjectDefault` |
| `ItemboxCollideCheck` | `ItemboxCollideCheckDefault` |
| `DrawPerScreen` | `DrawPerScreenDefault` |
| `ExecuteItemHook` | `ExecuteItemHookDefault` |
| `MiniMapDraw` | `MiniMapDrawDefault` |
| `MapStartup` | `MapStartupDefault` |
| `KWKumo_Alloc_Hook` | `KWKumo_Alloc_Hook_Default` |
| `KWChart_Kumo_Hook` | `KWChart_Kumo_Hook_Default` |
| `RubberBandMan` | `RubberBandManDefault` |

## RAM map

8MB RDRAM. Each color/depth buffer is `320×240×u16` = `0x25800` (150 KB). Banks are 1MB.

```
80000400   main code/data
801978D0   FreeMemory 768KB — boot seg 2+0xD, then course 6/9, collision table, actors, IceKage
802578D0   (pool end)
80280000   ending
8028DF00   racing + trig + kart textures/palettes
80300000   Z-buffer
80325800   gfx output + audio, then ok_ObjectPool (LoadOKData) → 80400000
80400000   OverKart .overkart + OKHeader BSS  (may enter 80500000, stop before FB0)
80500000   host / payload overflow   (~872 KB)
805DA800   FB0   (flush against 80600000)
80600000   FB1
80625800   gFaceBuffer → grows forward
           Vtx (seg 4) ← grows back from 80700000
80700000   FB2
80725800   F3DEX + map textures ← from 80800000
80800000   top
```

| Range | Use |
|---|---|
| `0x801978D0`–`0x802578D0` | 768 KB `FreeMemory` hole. Boot: segment 2 + `0xD`. After `StaticMemoryPointer`: course seg 6/9, collision table, actors, IceKage. |
| `0x80300000` | `gZBuffer` (`0x25800`) |
| after Z–`0x80400000` | gfx output + audio, then **`ok_ObjectPool`** (`LoadOKData` / OK objects). `ok_ObjectPoolStart`..`ok_ObjectPoolEnd`. |
| `0x80400000` then into `0x80500000` | Library `.overkart` then OKHeader BSS. May overflow into bank 5; must not reach FB0 at `0x805DA800`. |
| `0x80500000`–`0x805DA800` | Host / payload overflow (front of bank 5). |
| `0x805DA800` | `gFramebuffer0` (`0x25800`, flush against FB1) |
| `0x80600000` | `gFramebuffer1` |
| `0x80625800`–`0x80700000` | `gFaceBuffer` grows forward; decoded Vtx (seg 4) grow back from `0x80700000` |
| `0x80700000` | `gFramebuffer2` |
| `0x80725800`–`0x80800000` | F3DEX + map textures, `LastMemoryPointer` grows back from `0x80800000` |
| `ok_FreeSpace` | OKHeader BSS scratch (`0x20000` after `ok_TKMSpace`). Temp pad for **non-course** compressed data (fonts, HUD buttons, banners, ghosts). |
| `ok_HeaderROM` | `0xBE9178` (Tarmac table). Frozen. |

`LoadOKData` uses `ObjectMemoryPointer` in `ok_ObjectPool`. `MapStartupDefault` resets course pointers. Keep `OKHeader.h` and `OKHeader.asm` in lockstep if you add dest labels.

Course load (`load_course`): DMA geography (0xF) onto FreeMemory as throwaway → verts (4) from `VertexMemoryPointer` → packed DLs (7) from `LastMemoryPointer` → rewind FreeMemory → eat-own-tail seg 6 → offsets (9) → map textures (5). `MakeCollision` leaves faces in `gFaceBuffer`; the u16 `collision_table` stays on FreeMemory.

Runtime high-water (`CheckCourseMemoryHighWater`): hang if `FreeMemoryPointer` `>` `0x802578D0`, `VertexMemoryPointer` `<` `0x80625800`, `LastMemoryPointer` `<` `0x80725800` (ending/credits: `<` pool end), face end `>=` `0x80700000` or overlapping Vtx, or `ObjectMemoryPointer` `>=` `0x80400000`.

Boot: `init_overkart()` in `src/main.c` DMAs `.overkart` and calls `loadBigFont()`.

---

## `*Default` functions

All live in `OverKartDefaults.c` unless noted. “Vanilla site” is the decomp C that already calls them.

### Race / main loop

| Function | Vanilla site | Library default | Host typically |
|---|---|---|---|
| `gameCodeDefault` | `main.c` race tick | `CheckOKObjects()` | Keep the call; add project tick |
| `allRunDefault` | `main.c` main loop (`GlobalCustomCode`) | `gBackgroundFlag = 1` | Extra per-frame work |
| `titleMenuDefault` | `menu_items.c` title tick | **empty** | Title UI / splash / menu |
| `PrintMenuFunctionDefault` | `main.c` | `PrintMenuTest()` (also **empty**) | Menu print / overlays |

### Course load / select

| Function | Vanilla site | Library default | Host typically |
|---|---|---|---|
| `MapStartupDefault` | `code_800029B0.c` (`load_course` path) | Reset pool, `LoadCustomHeader` / `SetCustomData` / `LoadMapData` / `LoadIceKage`; custom scroll kill | Keep; wrap if the host loads extra blobs |
| `DrawMapSelectDefault` | **not called from vanilla** | Custom-set banner + C-left/C-right sprites | Call from `titleMenuDefault` (or replace) |
| `MapSelectMenuDefault` | **not called from vanilla** | HotSwap C-left/C-right, cup/course index | Same |

### Objects / items / draw

| Function | Vanilla site | Library default | Host typically |
|---|---|---|---|
| `DisplayObjectDefault` | `actors.c` hop `default:` | **empty** (custom object IDs) | Draw host object IDs |
| `CollideObjectDefault` | `actors.c` hop `default:` | Item box → `ItemboxCollideCheck` | Extra collide IDs |
| `ItemboxCollideCheckDefault` | via `StockAliases` `ItemboxCollideCheck` | Sphere hit → roulette | Keep or wrap |
| `DrawPerScreenDefault` | `actors.c` after actor draw | `DrawOKObjects` when `scrollLock` and `HotSwapID > 0` | Extra per-screen draw |
| `ExecuteItemHookDefault` | `actors_extended.c` | stock `ExecuteItem` | Custom items |
| `MiniMapDrawDefault` | `main.c` | `KWReturnViewport` + `KawanoDrawFinal` | Minimap chrome |

`BalloonCheckDefault` exists and calls `BalloonDisp`, but vanilla calls **`BalloonCheck`** in `GameTypesGeneric.c` (CTF/Soccer early-out, then `BalloonDisp`). Override `BalloonCheck` if you need that path; wiring `BalloonCheckDefault` is optional.

### Clouds / weather

| Function | Vanilla site | Library default |
|---|---|---|
| `KWKumo_Alloc_Hook_Default` | `code_8006E9C0.c` (called twice per screen setup) | Custom header sky/weather, then stock `KWKumo_Alloc` |
| `KWChart_Kumo_Hook_Default` | `code_80057C60.c` | Stock `KWChart_Kumo`; custom weather velocity/scale |

### Ghosts / CPU / save

| Function | Vanilla site | Library default |
|---|---|---|
| `check_romghostDefault` | `replays.c` | Stock `check_romghost` only if `HotSwapID == 0`; else `romghost_status = 1` |
| `DMAROMGhostDefault` | `replays.c` | Stock DMA only if `HotSwapID == 0` |
| `RubberBandManDefault` | `cpu_speed_control.inc.c` (`oga_accele`) | stock `enemy_tekagen_sub` |
| `SaveFunc800B45E0Default` | `save.c`, `menus.c`, `menu_items.c` | stock `SaveFunc800B45E0` |
| `SaveFunc800B4670Default` | same | stock |
| `LoadFunc800B4A10Default` | `menu_items.c` | stock `LoadFunc800B4A10` |
| `SaveFunc800B559CDefault` | `save.c`, `menus.c`, `menu_items.c` | stock |
| `SaveFunc800B5948Default` | same | stock |

Save wrappers exist so a host can intercept EEPROM without editing `save.c`.

---

## Already native (not `*Default`)

These are Library functions vanilla already calls in-line. Do not add Armips JALs for them. Change behavior by editing the Library `.c`, or by wrapping at the existing call site.

Examples: `LoadPressDataBypass` / `LoadDataBypass`, `AddGravityEdit`, `custom_check_bump_2`, `custom_SpinKart`, `CheckMapBG_ZX_Hook`, `DisplayTree1Bypass`, `DisplayPiranhaBypass`, `PakkunStrategyOverride`, `SearchListHook`, `DisplayKT1Hook`, `CheckDirectionFunc`, `SprintLapCheck`, `WrapPathIndexAtFinishCheck`, `InitRndSmokeHook`, `EventDisplay` / `_After`, `LakituSpawnBypass`, `PlayStarMusicHook` / `PlayFinalLapMusicHook`, `DecodeVertex2_OK`, `XLUDisplay`, `SnowCustomCheck`, `SetGhostData`, `Zanzou2`, `KWDisplay2P_*` / `4P_*`, `RadarFinishLine`, `ProStickAngleHook`.

Decls: `OverKartHooks.h`.

Dead on purpose: `DetectEmulator` (uncalled). Do not revive BootCode / PreCode DMA over `.overkart`.

## KartCheck / CameraCheck

Unresolved by design. The host project owns them. Do not import OverKart5 `.o` here.

## Other host projects

Amped Up, OverKart5, and new projects stay **guests**. Do not overlay another project's `Library/` onto `src/OverKartLibrary/`. Put project UI, KartCheck, CameraCheck, EEPROM blobs, and extra sprites in the host. Shared Library keeps the Tarmac course runtime.

**Amped Up Library gaps (current):** `docs/ampedup_library_handoff.md` then `docs/ampedup_integration.md`. Fill OverKart Library vs Amped Up’s older fork. Do not overlay that `Library/` tree and do not link the Amped Up host yet.

When Amped Up is later a guest: do not write `0x80600000` as a course/storage pool (FB1), `0x805F0000` as Knowledge (inside FB0), or `0x80410800` as scratch (inside `.overkart`). Host overflow is `0x80500000`–`0x805DA800` after `OKHeaderEND`. Course DLs still grow back from `0x80800000` but must stay above `0x80725800`. `ok_Storage` / `ok_Knowledge` / `SwopCheck` are gone from this tree.

## Contracts not to break

- Tarmac course header / object layout / `ok_HeaderROM` `0xBE9178`
- `sizeof(Player)` `0xDD8`, Object `0x70`, AnmObject `0xE0`
- Player shape already uses Library names (`tire_FL`, `sus`, `up_vector`, Smoke `trx`/`try`/`trz`)
- Single compiler: **IDO**. `COMPARE=0`, `AVOID_UB=1`
- Do not strip “unused” Library files; other hosts may still call them
