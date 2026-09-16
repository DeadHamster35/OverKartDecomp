# Amped Up Library gaps — agent handoff

**This is the current Amped Up workstream start-here** (2026-09-15). If the user says **continue** in the Amped Up / Library-gap context, that is the next **approved** batch below. Host ELF wiring is **not** this workstream.

Read first: this file, then `docs/ampedup_integration.md` (gaps / parked host work), then `docs/host_overrides.md` (RAM map, `*Default` contract). Do not overlay Amped Up `Library/` onto `src/OverKartLibrary/`.

---

## Who this is for

A later Cursor instance in **LibraryDecomp**. Goal: OverKart Library contains every **shared** function / struct / header-load behavior Amped Up’s older Library fork still needs, so a later Amped Up host retarget is not blocked by a missing Library symbol.

LibraryDecomp **wins**. Do not copy Amped Up RAM hacks, Litro labels, menus, or `PlayerExtra`.

| Path | Role |
|---|---|
| `LibraryDecomp/` | **Only place to edit** |
| `Library/` | Historical GCC copy. Read-only. |
| `AmpedUp Master/Library/` | Older fork to **read** for bodies/layouts. Do not copy wholesale. |
| `AmpedUp Master/OverKart/` | Host. Do not edit this stage. |
| `KimuraDecomp/` | Frozen. Never edit |

---

## Locked

- NTSC. Tarmac `ok_HeaderROM` `0xBE9178`. Live `sizeof(OKHeader)` **`0xD0`**. Do not shrink it.
- IDO, `COMPARE=0`, `AVOID_UB=1`. C89 (`int i;` at block start, no `for (int`). LF only.
- Vanilla C includes `OverKartHooks.h`, not `MainInclude.h`. These additions are Library TUs (`MainInclude.h` is fine).
- Do not rename decomp `Player` members. Do not restore `BEHAVIOR_FLOAT == 1` (collides with `BEHAVIOR_PATH`).
- Do not revive `DetectEmulator`, PreCode/BootCode DMA, `ok_Knowledge`, `ok_Storage`, `ChangeLastMemoryPointer`.
- Approval: explain the batch (files, what, why), wait, present the diff. “Go ahead” is not a waiver of review. `.cursor/rules/require-approval.mdc`.
- One family per batch. KartBot Compare ROM is not a goal. IDO errors still matter.

---

## Already in OverKart Library (do not re-port)

`copyCourseTable(int)`, `swapHS`, `SurfaceSFX` (body + file-local `SFX_*` in `CustomSurfaces.c`), Tarmac surface IDs, `AddGravityEdit` (live surface dispatcher), `SetPlayerColor` / `MakeBodyColor`, `SetGhostEffect(player, active, UseCount)`, `OKObject` / `OKRAMHeader`, `MapSelectMenuDefault`, `LoadOKData`, `ok_FreeSpace`.

Amped Up `CustomBodyColor` is host (`PlayerExtra`). `loadCoin` is host. `GameSelectMenu` / `PlayerSelectMenu*` are host UI (calls commented in Amped Up `OverKart.c`).

---

## Action plan

User intent (2026-09-15): fill Library gaps only. If they say **continue**, implement the next unchecked batch after approval.

### Batch 0 — confirm (no source edits)

**Why:** Overlay and behavior-ID gating need a real `Version` integer.

1. Find an Amped Up custom course header (ROM table at `0xBE9178`, or a Tarmac export). Repo `RawAssets/` may not have a packed `OKHeader`.
2. Record: first `int` (`Version`), total header size (`0xC0` vs `0xD0`).
3. If no file: **ask the user**. Do not guess `Version == 6` as `0xD0` — existing `Version <= 6` branches are **object-table offsets**, not struct size.
4. Spot-check one Amped Up OK object behavior byte vs current `BEHAVIOR_PATH == 1` (`OKStruct.h`). If old files use FLOAT=1, note it for batch 1; do not change macros.

**Done when:** Version + size written into the batch 1 proposal (or user confirmed).

---

### Batch 1 — `LoadCustomHeader` overlay

**Files:** `src/OverKartLibrary/CustomCourses/CustomLevels.c` (`LoadCustomHeader` ~1965). Optional: a **local** old-layout struct in that `.c` only — not `include/`, not replacing `OKStruct.h` `OKHeader`.

**Amped Up live layout** (`AmpedUp Master/Library/OKStruct.h`, second `typedef`, DMA `0xC0`):

Shared prefix through `EchoEnd`, then:

```
char GoalBannerToggle, BackgroundToggle, ManualTempo, Padding;
char PathTrigger[4];
uint MusicID;
… same PathLength / water / scroll / fog …
/* no Padding[8] */
```

**Current** (`OKStruct.h`): same prefix, then `uchar LapCount` (no `PathTrigger`), then `MusicID` … then `uint Padding[8]`. BSS size `ok_CourseHeaderSize` `0xD0`.

**Do not** `memcpy` a `0xC0` blob onto `OverKartHeader` as if it were `0xD0`.

**Implement:**

1. DMA 4 bytes, read `Version`.
2. Old size (expect `Version <= 5` after batch 0): DMA **`0xC0`** into a local `OKHeaderOld` (or `char[0xC0]` + field assigns). Copy shared fields into `OverKartHeader`. Map `PathTrigger[4]` (document what you do: ignore vs stash; live struct has `LapCount` only — default `LapCount` if unknown). Zero `Padding[8]`.
3. New size: DMA `sizeof(OKHeader)` into `&ok_CourseHeader` as today.
4. Then existing `g_courseTable` `runRAM` of `MapHeader`.
5. Keep `Version > 4` XLU and `Version <= 6` static-object offsets as they are unless batch 0 proves they must change.
6. If old objects use FLOAT=1, add a **Version-gated** remap in `OKBehaviors.c` in a **later** approved batch — not by redefining `BEHAVIOR_PATH`.

**Done when:** `make AVOID_UB=1 COMPARE=0` still links. New Tarmac courses unchanged. Old `0xC0` headers do not smear `MusicID` into `LapCount`.

**Reference:** Amped Up `LoadCustomHeader` DMA `0xC0` in `AmpedUp Master/Library/CustomCourses/CustomLevels.c` ~1328.

---

### Batch 2 — missing Library functions

Three small ports (+ optional wrapper). IDO. No Amped Up includes.

#### 2a `setHS(int HS_ID)`

**Files:** `CustomLevels.c` / `CustomLevels.h` next to `swapHS` (~2312).

Amped Up body: `AmpedUp Master/Library/CustomCourses/CustomLevels.c` ~1651. Current `swapHS` already uses `copyCourseTable` + `g_CupArray[Index] = Index` instead of `g_cup0Array0 = 8` (that path is `#if 0` as `stockTableSet`).

**Port using current helpers**, not the old cup literals:

- `HS_ID == 0`: if leaving custom, `copyCourseTable(0)`; set `HotSwapID = 0`.
- else: if `HotSwapID == 0`, `copyCourseTable(1)`; set `HotSwapID = HS_ID`; fill `g_CupArray[0..19] = index`.
- Then Amped Up also called `setSky()`; then `setPreviews()`, `previewRefresh()`, `setBanners()`, `courseValue = -1` (same as `swapHS`).

Declare loop `int Index` at block start. Prototype in `CustomLevels.h`.

Amped Up host callers (later, do not edit now): `OverKart.c`, `LitroFunc.c`, `UnlockMenu.c`.

#### 2b `MusSeqSetNext`

**Files:** `MusicSound/MusicControl.c` / `.h`.

Amped Up (`MusicControl.c` ~96):

```
void MusSeqSetNext(uchar Group, ushort SeqID, uchar Interp)
{
	NAISeqFlagEntry(0x20000000 | ((uchar)Group << 24) | ((uchar)Interp << 16) | (ushort)SeqID);
}
```

Insert with the other `MusSeq*` wrappers. `NAISeqFlagEntry` already used in that file.

#### 2c `MakeVertex`

**Files:** `SharedFunctions.c` / `.h`.

Amped Up (`SharedFunctions.c` ~281): fill `vtx[n].v.ob / flag / tc / cn`. Match `Vtx` used elsewhere in this tree (GBI union). Prototype in the header.

#### 2d optional `SetGhostEffect` 2-arg

**Files:** `Player/PlayerEffects.c` / `.h`.

Keep `SetGhostEffect(char, bool, int)` as canonical. If the batch includes the wrapper: `active ? 0xFFFF : 0` for `UseCount` (matches `CustomSurfaces.c` on/off). IDO cannot overload — use a distinct name (`SetGhostEffectSimple`) **or** a macro only if the user prefers the old 2-arg name. Ask in the proposal; do not break existing 3-arg call sites.

**Done when:** declared, compiled into `.overkart` (`OKLIB_EACH` already lists these `.c` files).

---

### Batch 3 — surfaces

**Files:** `CustomCourses/CustomSurfaces.h` / `.c`. Read-only compare: Amped Up `CustomSurfaces.h`.

Facts:

- Amped Up header `SFX_*` are **sound IDs** for `SurfaceSFX`, not Tarmac surface types.
- OverKart Library already has those `#define SFX_*` **inside** `CustomSurfaces.c` (~11–35) and `SurfaceSFX` / `AddGravityEdit` (`bump_status` cases: `GetItem`, `BooSurface`, `FastOoB`, …).
- Amped Up `GetSurfaceID` is mostly commented; `OverKart.c` call is commented. Live dispatcher is `AddGravityEdit`.
- Amped Up `LitroFunc.c` calls `SurfaceSFX(..., SFX_WET_SAND_CENTER)` — host will need `SFX_*` **public**.

**Do:**

1. Table Amped Up `SFX_*` vs `.c` locals; promote missing ones to `CustomSurfaces.h` so a later host can include them. Deduplicate `.c` (include header, drop local copies).
2. Table Amped Up specials (`Boo` vs `BooSurface`, etc.) vs current `CustomSurfaces.h` IDs. Current IDs stay canonical.
3. Restore `GetSurfaceID` **only** if batch 0/user shows Amped Up courses still depend on that entry. Do not paste the commented Amped Up loop over `AddGravityEdit`.

**Done when:** public `SFX_*` complete; written note of any surface-ID rename Amped Up must use later (`Boo` → `BooSurface`). No change to Tarmac numbers unless a missing ID is proven.

---

### Batch 4 — player padding (no decomp rename)

**Files (optional code):** new `src/OverKartLibrary/Player/PlayerPadding.h` included only from OverKartLibrary / future host — **not** `include/common_structs.h`.

**Do not** rename `hoot_flag` / `hamp_flag` / `litro_flag` in `include/common_structs.h`.

| Offset | Decomp (`common_structs.h`) | Amped Up Library `Player` |
|---|---|---|
| `0x0A` | `char hoot_flag[2]` | `ushort litro_flag` |
| `0x12` | `s16 hamp_flag` | `short effect_timer` |
| `0x32` | `char litro_flag[2]` | `short shellpathinghack` |

Amped Up `car->litro_flag` is **`0x0A`**, not Library `litro_flag` at `0x32`.

Accessors: read/write `u16`/`s16` via the existing members (`*(u16*)player->hoot_flag`, `player->hamp_flag`, `*(s16*)player->litro_flag`). Names must not be Amped Up’s `litro_flag` for `0x0A`. Comment the Amped Up aliases in that header only.

If the user wants doc-only, add the table to `ampedup_integration.md` and skip the `.h`.

**Done when:** mapping is in-tree; decomp `Player` sizeof still `0xDD8`.

---

## Compile / review (every code batch)

```
make AVOID_UB=1 COMPARE=0 -j
```

No `make clean` unless flags/linker force it. User pastes IDO errors. Present the diff; wait for review.

Skip-list / `OKLIB_EACH` does not need new `.o` for batches 1–3. Batch 4 header-only unless you add a `.c`.

---

## Do not do (parked host work)

See `ampedup_integration.md` “Later”. No Amped Up `SRC_DIRS`, no `OverKartDefaults.c` replace, no Knowledge, no `0x80600000` storage, no PreCode, no EEPROM `SaveGame`, no Litro `OKExternal.h` merge.

---

## Related

| File | Use |
|---|---|
| `docs/ampedup_integration.md` | Gap table, parked host RAM collisions |
| `docs/host_overrides.md` | Host contract, RAM map, `*Default` |
| `src/OverKartLibrary/OKStruct.h` | Live `OKHeader` / `BEHAVIOR_*` |
| `AmpedUp Master/Library/` | Read-only fork bodies |

Do not treat `symbols/library_next_phase.md` as present — that workstream is finished; `symbols/README.md` points at `host_overrides.md`.
