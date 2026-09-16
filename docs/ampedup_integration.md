# Amped Up integration (LibraryDecomp)

**Agent handoff (implement gaps):** `docs/ampedup_library_handoff.md`. If the user says **continue** on Amped Up Library completeness, that file is start-here (batches 0–4). This file is the gap table and parked host work.

Do not overlay `AmpedUp Master/Library/` onto `src/OverKartLibrary/`. Do not link Amped Up `.o` yet. Host RAM, EEPROM blobs, Litro labels, and `*Default` replacements wait until that project is named as a guest (`docs/host_overrides.md`).

NTSC only. Tarmac `ok_HeaderROM` `0xBE9178`. Current `sizeof(OKHeader)` `0xD0` stays. Approval/review; one batch at a time.

---

## Goal

When this stage is done, OverKart Library has no holes versus the Amped Up Library fork:

- Shared course/object/player APIs Amped Up Library exposed
- Course-header fields the old `Version` layout used (`PathTrigger[4]` vs `LapCount`)
- Surface IDs / helpers Amped Up Library used, mapped onto the current Tarmac table where they still apply
- Player padding bytes Amped Up hacks (names stay Amped Up–only)

Not in scope here: Amped Up `OverKart/` UI, costumes, Litro ROM labels, `SaveData`, PreCode, `ok_Storage`, Knowledge, BUILD.asm project JALs, KartCheck/CameraCheck, ELF wiring.

---

## Gap snapshot (2026-09-15)

Already in OverKart Library (do not re-copy Amped Up): `copyCourseTable(int)`, `swapHS`, `SurfaceSFX`, Tarmac surface IDs, `SetPlayerColor` / `MakeBodyColor`, `SetGhostEffect` **3-arg**, `OKObject` / `OKRAMHeader`, `MapSelectMenuDefault`, `LoadOKData`, `ok_FreeSpace`.

**Add or overlay in LibraryDecomp**

| Item | Amped Up Library | OverKart Library | Action |
|---|---|---|---|
| `OKHeader` `0xC0` / `PathTrigger[4]` | Live struct; DMA `0xC0` | `0xD0`, `LapCount`, `Padding[8]`; DMA `sizeof` | Version-gated load. Confirm `Version` (likely 5). Do not shrink the live struct. |
| `setHS(int)` | Sets `HotSwapID` + cup/banner refresh | Only `swapHS(int)` | Port `setHS` (Amped Up `OverKart.c` / `LitroFunc.c` call it). |
| `MusSeqSetNext` | `MusicControl.c` | Missing | Port the small wrapper. |
| `MakeVertex` | `SharedFunctions.c` | Missing | Port the helper. |
| `GetSurfaceID` / `SFX_*` vs Tarmac IDs | Header still exports; body mostly commented; `OverKart.c` call commented | Live loop in `CustomSurfaces.c`; `GetSurfaceID` commented out | Inventory (step 5). Keep current IDs. Restore a dispatcher only if Amped Up courses still need the old entry point. |
| `SetGhostEffect` | 2-arg | 3-arg (`UseCount`) | Keep 3-arg as canonical. Optional 2-arg wrapper that passes `0xFFFF` / `0` so old call sites compile later. |
| Player `0x0A` / `0x12` / `0x32` | `litro_flag` / `effect_timer` / `shellpathinghack` | `hoot_flag` / `hamp_flag` / `litro_flag` | Bytes already exist. Do not rename decomp Player. Overlay / accessors for the host later. |
| `BEHAVIOR_FLOAT` | `#define` 1 (old numbering) | Commented; `BEHAVIOR_PATH` is 1 | Do not restore old IDs. If old object files still stamp FLOAT=1, version-gate the interpreter with the header `Version`. |

**Do not add to Library** (host, dead, or superseded)

- `CheckEmulator` / `CheckPlatform` — `DetectEmulator` is dead
- `ChangeLastMemoryPointer`, `LoadData`, `ok_Knowledge`, `ok_Storage` — old RAM map
- `CustomBodyColor` — Amped Up `PlayerExtra` guards around `MakeBodyColor`; host keeps that wrapper
- `GameSelectMenu` / `PlayerSelectMenu*` — commented in Amped Up `OverKart.c`; Library already has `MapSelectMenuDefault`
- `loadCoin` — defined in Amped Up `OverKartObjectHandler.c`
- `OKObjectHeader` as a second object type — live type is `OKObject` / `OKRAMHeader`
- Litro `OKExternal.h` labels, EEPROM `SaveGame`, PreCode poke at `0x80600000`
- GameOffsets-only names (`KWSpriteDiv`, `osViBlack`, …) — decomp + `StockAliases.h` at host compile time

---

## Task list (this stage)

### 0. Confirm (no code)

- [ ] Read one Amped Up course file: `Version` integer and header size (`0xC0`).
- [ ] Spot-check an Amped Up OK object / behavior byte vs current `BEHAVIOR_PATH == 1`.
- [ ] Keep the gap table above as the absorb list; do not import unused Amped Up Library `.c`.

### 1. Course header overlay (`LoadCustomHeader` + `OKStruct.h`)

LibraryDecomp only.

- [ ] If `Version` is in the old range, DMA `0xC0` and copy into live `OKHeader`.
- [ ] Map `PathTrigger[4]` onto the current `LapCount` / padding slot; new Tarmac courses keep `LapCount`.
- [ ] Leave `ok_HeaderROM` `0xBE9178` and `sizeof(OKHeader)` `0xD0` unchanged.
- [ ] If step 0 shows old `BEHAVIOR_*` numbering, same `Version` selects the old object-behavior table (do not change current macros).

### 2. Missing Library functions

Port into existing OverKart Library files (IDO, current types). No Amped Up `PlayerExtra` or Litro includes.

- [ ] `setHS(int)` next to `swapHS` in `CustomLevels.c` / `.h`
- [ ] `MusSeqSetNext` in `MusicControl.c` / `.h`
- [ ] `MakeVertex` in `SharedFunctions.c` / `.h`
- [ ] Optional: `SetGhostEffect` 2-arg wrapper → 3-arg

### 3. Surfaces

- [ ] Diff Amped Up `CustomSurfaces.h` `SFX_*` and specials (`Boo`, `FastOoB`, …) against current Tarmac IDs (`ASPHALT` … `LavaSurface` / `BooSurface`).
- [ ] Current table stays canonical. Add only IDs or a `GetSurfaceID` entry that Amped Up courses still use and OverKart Library does not implement.
- [ ] Do not copy commented Amped Up `GetSurfaceID` over the live `CustomSurfaces.c` loop.

### 4. Player padding (Library-side ready, Amped Up names stay out)

- [ ] Document the offset map (`0x0A` `hoot_flag` = Amped Up `litro_flag`, `0x12` `hamp_flag` = `effect_timer`, `0x32` `litro_flag` = `shellpathinghack`).
- [ ] Optional OverKartLibrary accessors that read/write those bytes **without** renaming decomp `Player`. Host uses the accessors later; Amped Up `car->litro_flag` must not be assumed.

---

## Later (host Amped Up) — parked, not this stage

Still required when Amped Up is linked. The new RAM map makes several Amped Up addresses illegal.

| Amped Up | Now |
|---|---|
| `FreeMemoryPointer = 0x80600000` | FB1 |
| `ok_Knowledge` `0x80410800` / `0x805F0000` | `.overkart` / inside FB0 |
| `ok_Storage` | Removed; use `ok_FreeSpace` or host window `0x80500000`–`0x805DA800` |
| `ChangeLastMemoryPointer` / `LastMemoryPointer = 0x80800000` | `MapStartupDefault`; DLs stay `>= 0x80725800` |
| PreCode write `0x80600000` | Do not port |
| Alpaca/heart in OKHeader BSS | Host BSS in `0x80500000`–`0x805DA800` |

Then: peel Litro `OKExternal` / `SaveGame` into `OverKart/`, compile Amped Up as extra `.o`, replace `OverKartDefaults.c`, KartCheck/CameraCheck, `AMPEDUP_FONT=1`. Retarget `setHS` call sites if needed, `SetGhostEffect` arity, `copyCourseTable`, `CustomBodyColor` → host wrapper around `MakeBodyColor`.

---

## Order

```
0 confirm Version + behavior IDs
  → 1 LoadCustomHeader overlay
  → 2 setHS / MusSeqSetNext / MakeVertex [+ SetGhostEffect wrapper]
  → 3 surfaces inventory
  → 4 player padding accessors / doc
```

Host RAM / ELF work stays parked until Amped Up is the named guest.
