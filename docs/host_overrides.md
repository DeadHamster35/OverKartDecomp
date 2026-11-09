# Hosting a project on OverKart Library

This tree is a mk64 decomp with OverKart Library linked natively (one IDO toolchain). Vanilla C already calls Library. A **host** (OverKart5, Amped Up, a new project) supplies UI, KartCheck/CameraCheck, and any extra objects — it does not re-do Armips ROM hooks.

Start here for the override contract. Agent operating rules: **`AGENTS.md`**. RAM/ROM map: **`docs/memory_map.md`**. Vanilla-safe decls: `src/OverKartLibrary/OverKartHooks.h` (vanilla C must not include `MainInclude.h`).

`OverKartDefaults.h` stays in Library. The Defaults **body** is host-owned:

- AmpedUpDecomp: `src/AmpedUp/OverKartDefaults.c`
- OverKartDecomp: `src/OverKart5/OverKartDefaults.c`
- LibraryDecomp: `src/OverKartDefaults.c`

Sibling `Library/` is the historical GCC/Armips copy. `KimuraDecomp` is the frozen matching tree. Do not edit either from here.

NTSC (USA) only for custom courses. Tarmac `ok_HeaderROM` stays `0xBE9178`.

## This tree (OverKart5)

Host code lives in `src/OverKart5/`. `OVERKART_BUILD=0` is Tarmac Patch (community); `=1` is OverKart (personal). Drive the flag from the Makefile.

`make AMPEDUP_FONT=1` selects AmpedUpFont; default `0` is `LibraryFont` (`assets/OverKartLibrary/NewFont.MIO0`).

`GameOffsets.asm` is stock NTSC RAM (`0x80xxxxxx`), not live addresses: use it only to find the matching decomp symbol or statement, never as the value written in C.

`Code/Protec/Cheat.c` is closed-source and lives outside every repo. Never commit that source; only the compiled IDO `.o` (`lib/private/Cheat.o`). With `OVERKART_BUILD=0`, the object must not be required.

Do not overlay another project's `Library/` onto `src/OverKartLibrary/`. `ok_Storage` / `ok_Knowledge` / `SwopCheck` are gone. Host overflow is `0x80500000`–`0x805DA800` after `OKHeaderEND`.

## Build

```
make AVOID_UB=1 COMPARE=0 -j
make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j
```

Those are the Makefile defaults. Compare ROM is not a goal. Need `baserom.us.z64` in the repo root.

## How a host overrides

Vanilla already JALs the `*Default` names. There is one ELF, so there can be only one definition of each.

Replace the host-owned `OverKartDefaults.c` (keep the same function names and signatures). Do not add a second `gameCode` next to `gameCodeDefault`. Do not put the body back inside `src/OverKartLibrary/`.

Empty bodies (`titleMenuDefault`, `DisplayObjectDefault`, `PrintMenuTest`) are intentional. The host fills them.

Library C calls the `*Default` names directly (Library TUs only — vanilla C includes `OverKartHooks.h`). Host C keeps the short names (`GameCode`, `TitleMenu`, …) as real functions; `OverKartDefaults.c` trampolines `*Default` to those host bodies.

## RAM map

See **`docs/memory_map.md`**. Frozen labels:

- `ok_HeaderROM` `0xBE9178`
- `ok_FreeSpace` — non-course compressed scratch
- `ok_ObjectPool` — leftover after gfx/audio up to payload (`LoadOKData`)

`LoadOKData` uses `ObjectMemoryPointer` in `ok_ObjectPool`. `MapStartupDefault` resets course pointers. Keep `OKHeader.h` and `OKHeader.asm` in lockstep if you add dest labels.

Boot: `init_overkart()` in `src/main.c` DMAs `.overkart` and calls `loadBigFont()`.

---

## `*Default` functions

All live in the host-owned `OverKartDefaults.c` unless noted. “Vanilla site” is the decomp C that already calls them.

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
| `ItemboxCollideCheckDefault` | `CollideObjectDefault` IBOX path | Sphere hit → roulette | Keep or wrap |
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

Unresolved by design. The host project owns them. On this tree they arrive via the private `Cheat.o`.

## Contracts not to break

- Tarmac course header / object layout / `ok_HeaderROM` `0xBE9178`
- `sizeof(Player)` `0xDD8`, Object `0x70`, AnmObject `0xE0`
- Player shape already uses Library names (`tire_FL`, `sus`, `up_vector`, Smoke `trx`/`try`/`trz`)
- Single compiler: **IDO**. `COMPARE=0`, `AVOID_UB=1`
- Do not strip “unused” Library files; other hosts may still call them
- `src/OverKartLibrary/` must stay identical across AmpedUpDecomp, OverKartDecomp, and LibraryDecomp
- Do not copy closed-source `Cheat.c` into the tree
