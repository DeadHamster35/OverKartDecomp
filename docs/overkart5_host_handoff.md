# OverKart5 host integration — agent handoff

**This is the current OverKart5 workstream start-here** (2026-09-16). If the user says **continue** in the OverKart5 / host-integration context, that is the next **approved** batch below.

Read first: this file, then `docs/overkart5_integration.md` (inventory, patch tables, hazards), then `docs/host_overrides.md` (RAM map, `*Default` contract).

This is the **first** host project linked into LibraryDecomp. Amped Up stays parked (`docs/ampedup_library_handoff.md`) — do not interleave the two.

---

## Start here

Nothing has been implemented yet. These docs are the plan; the tree is untouched apart from them.

Your first session:

1. Read this file top to bottom, then skim `docs/overkart5_integration.md` for the tables you will need.
2. Run **batch 0**. It is measurement and confirmation only — no source edits, so no approval needed to *look*.
3. Propose **batch 1** to the user: the exact files, what changes in each, and why. Wait for approval. Then implement, then present the diff for review.

Do not start by importing files. The payload budget in batch 0 can invalidate the whole import order, and `OverKart.c` cannot link until batches 5 and 6a are done.

Three things that will bite you if you skip them: the twelve `StockAliases.h` deletions must land in the same batch as the host definitions; `ok_Storage` no longer exists in this tree; and OverKart5's flycam writes MIPS instruction words into vanilla functions at addresses that a `COMPARE=0` build does not guarantee.

---

## Who this is for

A later Cursor instance in **LibraryDecomp**. Goal: OverKart5's C sources compile with IDO inside this tree, link into the `.overkart` segment, and reach the game through native decomp call sites — no Armips, no `BUILD.asm`, no ROM `.org` patching.

| Path | Role |
|---|---|
| `LibraryDecomp/` (this tree) | **Only place to edit** |
| `OverKart5/` | Host source to **read and import**. Do not edit it to make this build work. |
| `OverKart5/BUILD.asm` | Blueprint only. Tells you *what* was hooked, not how to hook it here. |
| `Library/` | Historical GCC copy. Read-only. |
| `KimuraDecomp/` | Frozen. Never edit. |

Host code lands in **`src/OverKart5/`**, sibling to `src/OverKartLibrary/`. Do not flatten it into the Library.

---

## Locked

- NTSC only. Tarmac `ok_HeaderROM` stays `0xBE9178`. `sizeof(OKHeader)` stays `0xD0`.
- One toolchain: **IDO**. `COMPARE=0`, `AVOID_UB=1`. C89 (`int i;` at block start, no `for (int`). **LF line endings** — every imported file is CRLF today.
- Vanilla C includes `OverKartHooks.h`, not `MainInclude.h`. Host TUs are like Library TUs: `MainInclude.h` is fine.
- `sizeof(Player)` `0xDD8`, Object `0x70`, AnmObject `0xE0`. Do not rename decomp `Player` members.
- `SaveData` (`OverKartStruct.h`, 512 bytes with `char PAD[449]`) is the **EEPROM layout**. Do not reorder or resize it; existing saves depend on it.
- Do not revive `ok_Storage`, `ok_Knowledge`, PreCode/BootCode DMA, `DetectEmulator`, `ChangeLastMemoryPointer`.
- Approval: explain the batch (files, what, why), wait, present the diff. "Go ahead" is not a waiver of review. `.cursor/rules/require-approval.mdc`.
- One batch at a time. KartBot Compare ROM is not a goal. IDO errors still matter — paste them to the user.

---

## Two products, one tree

OverKart5 is **two** builds gated by flags, not one:

| Flag | Where | `0` | `1` |
|---|---|---|---|
| `OverKartBuild` | `Build.h:5` | **Tarmac Patch** (community) | **OverKart** (personal) |
| `OKBuild` | `BUILD.asm:14` | same, Armips side | same |
| `DebugMode` / `DEBUGBUILD` | `Build.h:7,9` | off | dev shortcuts |
| `ProtectMode` | `Build.h:8` | — | anti-piracy path armed |

**Both builds are targets.** `OKBuild` and `OverKartBuild` are the same switch and must stay linked — one flag, toggled to build either product. Keep every `#if OverKartBuild` block and import all of the gated material: cheat menu, piracy screen, `Segment12.bin`, JP audio bank, splash logo.

Drive the flag from the Makefile (`make OVERKART_BUILD=1` → `-DOverKartBuild=1`) rather than hand-editing `Build.h`, so both configurations are reachable without a source edit. `Build.h:5` becomes the default only.

---

## Private sources — `Protec/`, out of tree, never committed

`OverKartBuild=1` depends on a **closed-source** translation unit that deliberately lives **outside both project folders**:

```
Code/Protec/Cheat.c      945 lines — the cheat implementation
Code/Protec/Protec.c     47 lines  — flashcart detect stub (incomplete; see below)
```

It is kept out-of-tree specifically so it cannot be uploaded by mistake. **Preserve that.** Do not copy `Cheat.c` into `src/OverKart5/`, not even into a `.gitignore`d path — an ignore rule is a weaker guarantee than the file not being in the repo at all. OverKart5 already establishes the intended shape: `OverKart5/.gitignore` reads `*.o` plus `!Protec.o`, so only the compiled object ships.

### Build design

1. **Source stays external.** Add a Makefile variable, e.g. `PROTEC_DIR ?= ../Protec`. If `$(PROTEC_DIR)/Cheat.c` exists, compile it into `$(BUILD_DIR)/src/OverKart5/Cheat.o` with the normal IDO rule.
2. **Fall back to a committed object.** If the source is absent (any contributor without the private folder), copy a prebuilt `lib/private/Cheat.o` into the same build path instead. `mk64.ld` references only the build path, so it never needs to know which branch ran.
3. **Commit the object, ignore everything else.** `.gitignore:2` ignores `*.o` tree-wide, so the prebuilt needs a negation in the style already used at `.gitignore:94-96`:

```
!/lib/private/*.o
```

4. **Add defensive ignores** in case a copy ever lands in-tree by accident:

```
/src/OverKart5/Cheat.c
/src/OverKart5/Protec.c
/src/OverKart5/private/*
```

5. **Rebuild the object with IDO once** and commit that. Do not reuse OverKart5's GCC-built `Protec.o`; this tree is `-G 0`, IDO, `-non_shared`, and object-level flag mismatches surface as link or runtime faults, not diagnostics.
6. With `OverKartBuild=0`, nothing references these symbols — the object must not be required for the community build. Gate the link, not just the calls.

State plainly in the proposal that a committed `.o` is still disassemblable. It withholds source, which is the goal; it is not encryption.

### What `Cheat.c` actually provides

Everything previously unaccounted for. `Cheat.c` includes `../Library/MainInclude.h` and `../OverKart5/OKInclude.h`, and defines:

| Group | Symbols |
|---|---|
| Cheat API (declared `OverKart.h:13-19`) | `CheatSetup`, `CheckCheat`, `DrawCheatMenu`, `CheatMenuHandler`, `ApplyCheats`, `LoadCheatData`, `DrawCheat3D` |
| Internal | `CheckFinalKey`, `ResetKey`, `CheckKeyEntry`, `DrawCheatCode`, `CopyBump` |
| Draw guards | `KartCheckFuncA`/`B`/`C`/`D` and `A2`/`B2`/`C2`/`D2` |
| Camera | `CameraCheckFunc` |
| Results | `ResultsCheck` |
| Data | `CheatKey[]`, `PlayerModels[]`, `StoredKey[]`, six per-cheat key arrays, `CheatKeys[]`, `CheatUnlocked[]`, `CheatActive[]`, `CheatNames[]`, `ActiveValue[]` |

This **removes the blocker** on the `.if OKBuild` hook block and collapses 39 ROM patch sites into three small edits — see batch 7a.

`Cheat.c` needs the same IDO conversion as the host files: `for (int …)` at `:7,196,198,372,389,500,527,635,742,885,898,914,928`, mid-block declarations at `:354-355,535-537,716,732,870`, and `extern long Seg12BIN, Seg12END;` at `:473` requires the `Segment12.bin` ROM blob (batch 5).

### `Protec.c` is a stub — do not wire it

`Code/Protec/Protec.c` defines only `DPR2()`, which has `return;` in a `uint` function (`:9,19,30,44`) and references `usb_io_read`, `usb_cart`, `buff`, and `D64_*`/`ED_*`/`SC64_*` macros that exist nowhere in these trees. It does **not** define `DPR` or `DPRSave`, which is what `OverKart.c:1079-1080` names — and those calls are already commented out. `Untitled-1.txt` in that folder is unrelated scratch.

So: `ProtectMode` (`Build.h:8`) is inert in C today. Leave it that way. If the real anti-piracy object is needed later, ask the user for it — the `Protec.c` in that folder is not it.

---

## Aliases: delete them, do not work around them

`src/OverKartLibrary/GameVariables/NTSC/StockAliases.h:90-104` currently redirects the host-owned names onto the `*Default` names:

```c
#define gameCode gameCodeDefault
#define allRun allRunDefault
#define titleMenu titleMenuDefault
/* ...9 more... */
```

That layer exists only because there was no host. **Delete the twelve host-owned lines.** The host then defines the real `gameCode`, and `gameCodeDefault` — still the name vanilla calls — forwards to it:

```c
void gameCodeDefault(void)
{
    gameCode();
}
```

**Delete (12):** `gameCode`, `titleMenu`, `DisplayObject`, `CollideObject`, `ItemboxCollideCheck`, `DrawPerScreen`, `allRun`, `GlobalCustomCode`, `PrintMenuFunction`, `ExecuteItemHook`, `MiniMapDraw`, `RubberBandMan`.

**Keep (3):** `MapStartup`, `KWKumo_Alloc_Hook`, `KWChart_Kumo_Hook`. OverKart5 does not define these, so the Library keeps calling the `*Default` bodies.

`GlobalCustomCode` is a Library-era second name for `allRun`. Nothing in this tree calls it — only the label at `LIBRARYBUILD3.asm:124` — so drop it, or repoint it to `allRun`.

### Ordering constraint

Alias deletion and the host definitions must land in the **same batch**. Two places break the moment the aliases go away, and only two:

- `OverKartDefaults.c:40` calls `ItemboxCollideCheck(Car, Target)` inside `CollideObjectDefault`. Today that expands to `ItemboxCollideCheckDefault`. After deletion it resolves to the host function, so the host must already be linked.
- `SharedFunctions.h:35-39` declares `CollideObject`, `ItemboxCollideCheck`, `DrawPerScreen`, `allRun`, `ExecuteItemHook`. Today the preprocessor rewrites those into duplicate `*Default` declarations. After deletion they become the real host prototypes — correct, but they must match the host signatures exactly.

Nothing else in the Library C calls the twelve short names. That is the whole blast radius.

### Keep `OverKartDefaults.c`

Do **not** drop it from `OKLIB_EACH`. The stock fall-throughs must survive: `MapStartupDefault`, `KWKumo_Alloc_Hook_Default`, `KWChart_Kumo_Hook_Default`, `BalloonCheckDefault`, `check_romghostDefault`, `DMAROMGhostDefault`, and all five `Save*`/`Load*` wrappers. Only the twelve overridden bodies become forwarders.

Once the aliases are gone this tree no longer links without the host — that is the intended bridge. If you need a host-less build for bisecting, wrap the forwarders in `#if HOST_OVERKART5` with the old stock bodies in the `#else`; do not restore the aliases.

---

## Action plan

| Batch | What | Gated on |
|---|---|---|
| 0 | Measure payload budget, confirm `PROTEC_DIR` | — |
| 1 | Build plumbing, delete 12 aliases, import `MarioKartAI` | 0 |
| 2 | Data/leaf TUs: `OverKartVariables`, `LitroFunc`, `ModelData`, `BlockFortPaths` | 1 |
| 3 | Gameplay TUs: `OverKartObjectHandler`, `OKMenu`, `MarioKartPractice`, `OverKart.c` last | 2, and 5 + 6a before 3d |
| 4 | Entry-point rewiring: `*Default` forwarders, menu controllers | 3 |
| 5 | ROM assets | 1 |
| 6 | RAM hazards: `ok_Storage`, flycam pokes, missing `asm_*` symbols | 3 |
| 7 | `BUILD.asm` → decomp C. 7a `.if OKBuild` hooks, 7b UI suppression | 4 |
| 8 | `OverKartBuild=1`: private object, `Segment12`, piracy screen | 5, 7 |

### Batch 0 — confirm (no source edits)

**Why:** the `.overkart` size budget decides whether batches 2 and 3 need an expansion-RAM overflow segment, and that changes the linker work in batch 1.

1. `make AVOID_UB=1 COMPARE=0 -j` on the tree as-is. Record from `build/us/mk64.map`: `_overkartSegmentStart/End/Size`, `_overkartSegmentNoloadEnd`, `OKHeaderEND`.
2. Compute remaining room: `0x805DA800 - OKHeaderEND`. Write the number into the batch 1 proposal.
3. Estimate host size. The one big TU is `data/ModelData/ModelData.c` (7,291 lines, 446 KB source) — generated Gfx/texture data, so most of it becomes `.data`/`.rodata` in the payload. `TitleMenu.c` is **not** imported (see batch 2), which removes another 318 KB.
4. If step 2 minus step 3 is negative, **stop and ask** before importing. The overflow target is the host window `0x80500000`–`0x805DA800`, then expansion RAM — not FB1 at `0x80600000`.
5. Confirm `PROTEC_DIR` resolves — check that `../Protec/Cheat.c` is reachable from this tree. If it is not, `OverKartBuild=1` needs the prebuilt object instead, and batch 8 changes shape.

**Done when:** the size budget is written down and the `Protec` path is confirmed.

Already decided, do not re-litigate: `src/OverKart5/` as the host dir, alias deletion over `#undef`, both build flags preserved, `TitleMenu.c` dropped, `Protec.c` unwired. Genuinely still open: whether `Repro.c`, `SpareCode.c`, `grayscale.c`, `Notes.txt`, and the legacy `.asm` files come across as uncompiled reference or get left behind — none are in `BUILD.asm`'s object list, and none are needed to build either product.

---

### Batch 1 — build plumbing + one trivial TU

**Why:** prove the toolchain path with the smallest possible surface before touching 15,000 lines.

**Files:**

- `Makefile` — add `OVERKART5_SRC_DIRS := src/OverKart5 src/OverKart5/data/ModelData`, append to `SRC_DIRS`, add `src/OverKart5` to `INCLUDE_DIRS`, and add the `OVERKART_BUILD` flag (`-DOverKartBuild=$(OVERKART_BUILD)`, default `0`). `HOST_OVERKART5` is optional — add it only if you want the `#if` escape hatch for host-less bisecting described above.
- `mk64.ld` — an `OK5_EACH(M)` macro next to `OKLIB_EACH`, expanded into the existing `BEGIN_SEG(overkart, LINKER_OVERKART)` block for `.text`/`.data`/`.rodata` and into `.overkart.noload` for `.bss`. Host `.o` must land **after** the Library's so the Library's own layout does not shift.
- `src/OverKartLibrary/GameVariables/NTSC/StockAliases.h` — delete `#define RubberBandMan RubberBandManDefault` (line 104). Delete the other eleven as their host definitions arrive, or all twelve now if you land batch 1 with stubs.
- `src/OverKart5/MarioKartAI.c` / `.h` — 206 lines, smallest real TU. Only `RubberBandMan`, `SetAIDifficulty`, `aiSetup`, `fastAI`.
- `src/OverKart5/OKInclude.h`, `Build.h`, `OverKartStruct.h`, `MarioKartAI.h` — the minimum header set. Drop the `TitleMenu.h` include; stub the not-yet-imported includes and restore them one batch at a time.
- `src/OverKartLibrary/OverKartDefaults.c` — forward `RubberBandManDefault` to host `RubberBandMan` only.

**Import rules for every file, every batch (do not skip):**

1. CRLF → **LF**.
2. `#include "../Library/MainInclude.h"` → `#include "MainInclude.h"`. `src/OverKartLibrary` is already on `INCLUDE_DIRS` (`Makefile:360`).
3. Backslash include paths → forward slashes: `Data\ModelData\ModelData.h` → `data/ModelData/ModelData.h`, `CustomMenu\GFX\MenuGFX.h` → `CustomMenu/GFX/MenuGFX.h`.
4. `BlockFortPaths.C` → `.c` and `BlockFortPaths.H` → `.h`. The Makefile globs `*.c` only, and `OKInclude.h:15` already spells the include lowercase.
5. `for (int i = 0; ...)` → `int i;` at block start. ~40 sites.
6. Declarations after statements → hoist to block start. ~15 sites.
7. `__attribute__((aligned(16)))` → **delete**. IDO has no equivalent. 26 sites in `OverKartVariables.c` alone. If an object genuinely needs 16-byte alignment for DMA, note it in the proposal and align via the linker script, not the compiler.
8. Leave `//` comments and `bool`/`true`/`false` alone — `-Xcpluscomm` (`Makefile:389`) and `include/PR/ultratypes.h:8-20` cover both.
9. Leave float `f` suffixes alone; valid C89.

**Done when:** `make AVOID_UB=1 COMPARE=0 -j` links, `RubberBandManDefault` reaches host `RubberBandMan`, and the payload assert at `mk64.ld:871` still passes.

---

### Batch 2 — data and leaf TUs

Order matters: smallest and least entangled first, so IDO errors arrive in small readable piles.

| Sub-batch | Files | Notes |
|---|---|---|
| 2a | `OverKartVariables.c` / `.h` | 26 `__attribute__` deletions. Menu string tables. |
| 2b | `LitroFunc.c` / `.h` | Needs ROM labels — see batch 5. `LitroFunc.h:10` declares `loadTitleMenuFrame` with no definition; drop the declaration or stub it. |
| 2c | `data/ModelData/ModelData.c` / `.h` | **Required.** 7,291 lines, generated. Fix the `../../../Library/MainInclude.h` include depth. Expect a long IDO compile. |
| 2d | `BlockFortPaths.c` / `.h` | Pure data. `Pathfinding.c` references are commented out; confirm something still calls it or leave it out. |

### Do not import `TitleMenu.c` / `TitleMenu.h`

Verified unreferenced. `TitleMenu.c` defines 17 `Gfx Draw_*` display lists at `:4067-5033`, and **nothing outside `TitleMenu.c` itself uses any of them**. The title 3D geometry is loaded at runtime from the `Splash3D` ROM blob (`data/SplashLogo/TitleMenu.bin`, MIO0 → segment 8) at `OverKart.c:1028-1037`, which supersedes the compiled-in version.

One loose end: `data/ModelData/ModelData.h:1,22,41` declares `extern Gfx Draw_DefaultMaterial_T[];` three times (a generator artifact). `ModelData.c` never references it, and the definition in `TitleMenu.c:4067` is an empty list — just `gsSPEndDisplayList()`. Delete those three stale externs while importing `ModelData.h`.

That drops 4,953 lines and 318 KB of source, plus its payload footprint. If a title-screen regression appears later, the blob path at `OverKart.c:1028` is the thing to check, not these display lists.

**Done when:** each sub-batch links and payload size is re-recorded from the map after 2c.

---

### Batch 3 — gameplay TUs

| Sub-batch | Files | Watch for |
|---|---|---|
| 3a | `OverKartObjectHandler.c` / `.h` | `Draw3DRacer` signature clash: `OKCustomObjects.h:4` declares `void Draw3DRacer(void)`, host defines `void Draw3DRacer(uint, uint)`. Fix the Library declaration to match the host. `loadCoin`, `RedCoinChallenge`, `GoldCoinChallenge`, `DropCoins`, `CheckHit` are all host-owned — the Library has no definitions. |
| 3b | `OKMenu.c` / `.h` | 1,697 lines, the biggest behavioral TU. Uses `asm_DispOBSubPSelCursor1` and `asm_BlinkCheck` self-modifying writes (batch 6). `ModularMenu` / `OKPanel` / `OKOption` / `OKMenu` come from the Library (`OKStruct.h:249-269`, `MarioKartMenu.c:235`) — do not duplicate them. `printMap`, `DrawFPS`, `DisplayCoinSprite` are also already Library. |
| 3c | `MarioKartPractice.c` / `.h` | Do not import the instruction-patching bodies yet. Stub `RunFlyCam` / `FlyCamInit` and land the rest; batch 6 converts them. |
| 3d | `OverKart.c` / `.h` | Last. `ok_Storage` (7 sites) and `GlobalAddressD = 0x80690000` must be resolved first — see batch 6. Delete `ScreenDrawHook` (`OverKart.c:1366`) and `ClearFramebufferCheck` (`:1374`): both are already native in this tree. |

**Done when:** all host TUs compile and link with `OverKartBuild=0`.

---

### Batch 4 — entry-point rewiring

No new hooks. Every host entry point already has a decomp call site:

| Host function | Forwarded from | Decomp call site |
|---|---|---|
| `gameCode` | `gameCodeDefault` | `src/main.c:602` |
| `allRun` | `allRunDefault` | `src/main.c:1220` |
| `PrintMenuFunction` | `PrintMenuFunctionDefault` | `src/main.c:926` |
| `MiniMapDraw` | `MiniMapDrawDefault` | `src/main.c:867` |
| `titleMenu` | `titleMenuDefault` | `src/menu_items.c:2395` |
| `DrawPerScreen` | `DrawPerScreenDefault` | `src/racing/actors.c:2609` |
| `DisplayObject` | `DisplayObjectDefault` | `src/racing/actors.c:2597` |
| `CheckHit` | `CollideObjectDefault` | `src/racing/actors.c:2277` |
| `ItemboxCollideCheck` | `ItemboxCollideCheckDefault` | via `CollideObjectDefault` |
| `ExecuteItemHook` | `ExecuteItemHookDefault` | `src/racing/actors_extended.c:978` |
| `RubberBandMan` | `RubberBandManDefault` | `cpu_vehicles_camera_path/cpu_speed_control.inc.c:208` |

**Not called by vanilla — the host must call them itself:** `DrawMapSelectDefault` and `MapSelectMenuDefault`. OverKart5 reached these through its own `DrawMapSelect` / `MapSelectSwitch`; wire them from `titleMenu` / `PrintMenuFunction`.

**Menu controller JALs have no `*Default` seam at all.** `DataMenuController`, `TitleMenuSwitch`, `GameSelectSwitch`, `MapSelectSwitch` replaced whole vanilla controller functions at `BUILD.asm:264-276`. In this tree they must be called from, or replace, `menus.c` — `TitleController`, `GSelController`, `MSelController`, and the DATA-menu branch of the controller dispatch. `PlayerSelectSwitch` was already commented out at `BUILD.asm:273`; leave it out.

**Done when:** the game boots to the OverKart title menu.

---

### Batch 5 — ROM assets

OverKart5's `.import` labels are already **declared** in `src/OverKartLibrary/OKExternal.h` but most have no **definition** in this tree. Add them the way the Library does it: a `.s` with `.incbin`, assembled into the ROM-assets segment.

**Files:** new `data/overkart5/ok5_rom_assets.s`, new `assets/OverKart5/` blob directory, `Makefile` (`ASM_DIRS`), `mk64.ld` (extend `BEGIN_SEG(oklib_romassets, ...)` or add a sibling segment).

| Label | Blob (from `OverKart5/`) | Declared at |
|---|---|---|
| `LogoROM` | `data/KimuraBless.bin` | `OKExternal.h:28` |
| `RCIconMap` | `data/RedSquare.png.RAW` | `:29` |
| `RCSpriteROM` | `data/RedCoinSprite16.png.MIO0` | `:30` |
| `ArrowsSpriteROM` | `data/arrows.png.MIO0` | `:31` |
| `NumbersSpriteROM` | `data/number_sprites.png.MIO0` | `:32` |
| `ROptionROM` / `ROptionEnd` | `data/ROption.MIO0` | `:37` |
| `MenuIconsROM` / `MenuIconsEnd` | `data/SplashLogo/OKLogo.bin` | `:49-50` |
| `Splash3D` / `Splash3DEnd` | `data/SplashLogo/TitleMenu.bin` | `:76` |
| `Pirate` / `PirateEnd` | `data/PiracyWarning.MIO0` | `:60-61` — `OverKartBuild=1` |
| `Seg12BIN` / `Seg12END` | `data/Segment12.bin` (60,872 B) | not declared — add. `OverKartBuild=1`; required by `Cheat.c:473-483` |
| `JP_Bank` / `JP_Audio` | `data/JP_Bank.bin`, `data/JP_Audio.bin` | `:54-55` — 2.4 MB; confirm before importing |

`Crash` / `CrashEnd`, `NiceFontROM`, `HudButtonsROM` already exist in `data/overkart/ok_rom_assets.s` — **do not add second definitions.** OverKart5's `BigFontROM` (`data/Newfont.MIO0`) is a third font option next to `LibraryFont` and `AmpedUpFont`; extend the `AMPEDUP_FONT` pattern rather than replacing either.

`RAMCheck.bin` is a debug blob — ask before importing.

Keep `ok_HeaderROM` at `0xBE9178` and the `--pad-to=0x1000000` fill (`Makefile:395`).

**Done when:** every label `LitroFunc.c` and `OverKart.c` reference resolves, and the ROM still builds to 16 MB.

---

### Batch 6 — RAM and self-modifying-code hazards

This is the batch most likely to produce a silent crash. Nothing here is a mechanical port.

#### 6a `ok_Storage` is gone

`OverKart.c` uses it as a decompression destination at `:180` (`FreeSpaceAddress`), `:943`, `:1033`, `:1109`, `:1118`, `:1184`, `:1251`. Per `docs/host_overrides.md:172`, replacements are `ok_FreeSpace` (temp pad, **non-course** compressed data only) or a new host BSS window in `0x80500000`–`0x805DA800` after `OKHeaderEND`.

These are menu-time splash/logo/piracy/R-option graphics — non-course, so `ok_FreeSpace` is legal, but several sites already use `ok_FreeSpace` as the DMA **source** and `ok_Storage` as the MIO0 **destination**. They cannot be the same buffer. Add a host-owned `ok5_MenuStorage` in the host window and size it from the largest blob (`OKLogo.bin`, 40 KB; `PiracyWarning`, 240 KB decompressed at 512×240×2).

#### 6b `GlobalAddressD = 0x80690000`

`OverKart.c:568`. That is inside the `gFaceBuffer` → Vtx window (`0x80625800`–`0x80700000`). Find what reads `GlobalAddressD` and give it real storage. Do not keep the literal.

#### 6c Flycam instruction patching → native C

`MarioKartPractice.c:697-733` writes instruction words into vanilla functions. All four are now identified — convert each to a guarded call, not a poke:

| Patch | Decoded | Native equivalent |
|---|---|---|
| `*(uint*)0x802A5C98 = 0` / restore `0x0C01636D` | `JAL KWDisplay2DAfter` (`0x80058DB4`) in the 1P draw path | `src/racing/skybox_and_splitscreen.c:863` — guard `KWDisplay2DAfter(RENDER_SCREEN_MODE_1P_PLAYER_ONE)` on the flycam flag |
| `*(uint*)0x8001EE98 = 0x03E00008` / restore `0x27BDFFC8` | `jr $ra` over the `CameraControl` prologue (`addiu $sp,$sp,-0x38`) | `src/camera.c:909` — early return in `CameraControl`, or guard its call sites in `src/main.c:624-806` and `src/racing/race_logic.c:1265-1276` |
| `*(uint*)0x80291154 = LUI` / `0x80291164 = LH`, restore `0x01E72021` / `0x2484FFFC` | redirects a path-section lookup (`addu $a0,$t7,$a3` / `addiu $a0,$a0,-4`) to read `&FlyCamSection` | racing segment `0x8028DF00 + 0x3254`. Identify the function from `build/us/mk64.map`, then read the section index from `FlyCamSection` behind the flag |
| `*(uint*)0x800382DC = 0x03E00008`, `0x800382E0 = 0` / restore `0x3C02800E` / `0x8C42C52C` | `jr $ra` over a function whose first act is `lw g_ScreenSplitA` (`0x800DC52C`) | main segment; identify from the map, then early-return behind the flag |

`RunFlyCam` also swaps `0x20` bytes at `0x8018CAB0` through `ok_FreeCam` / `ok_FreeCamBackup`. `0x8018CA70` is `g_hudStruct` / `playerHUD[0]`, so that is `playerHUD[0] + 0x40` — replace with a typed struct copy through `playerHUD`, not an address.

`enhancements/flycam.patch` is a starting point for the camera side.

#### 6d Practice-mode pokes

`MarioKartPractice.c:461,470` write `0x8007AC54` (0, then restore `0x10400009` = a `beq $v0, $zero` branch) around a `RouletteStart` call — it is suppressing a branch in the item path for one frame. Convert to a flag.

#### 6e Menu self-modifying writes

`OKMenu.c` writes `asm_DispOBSubPSelCursor1[0..3]` and `asm_BlinkCheck` (20 sites). Both **are** in `undefined_syms.txt:52-53` (`0x800F1AB8`, `0x800AA754`), so they link — but they are function-pointer-table and branch-word writes into vanilla player-select code. Verify each against the map, then prefer the native equivalent.

#### 6f Missing symbols

`asm_SongA` / `asm_SongB` are declared at `GameOffsets.h:612-613` but **absent from `undefined_syms.txt`**, so `OverKart.c:155-156` will not link. They are instruction words at `0x8028EC9C` / `0x8028F9C4` being set to `0x240E0001` (`li $t6, 1`). Find the decomp statement instead of adding the symbols.

**Done when:** no host TU writes to a hardcoded code address, and every remaining `0x80…` literal is a documented data address inside the RAM map.

---

### Batch 7 — `BUILD.asm` patches → decomp C

#### 7a The `.if OKBuild` hook block — now trivial

Reading `Cheat.c` collapses 39 `.org` sites into three small edits, because every one of those hooks is a thin guard around a vanilla call the decomp already makes by name. Do **not** chase the ROM offsets for these.

| Hook | `Cheat.c` body | Decomp edit |
|---|---|---|
| `CameraCheckFunc(player, camera, num)` | calls `CameraControl(...)`, then a headcam block when `CheatSettings.CHEATF`, else `QuickCamCode(...)` | Replace the `CameraControl(...)` calls with `CameraCheckFunc(...)` at `src/main.c:624,686,688,732,734,800,802,804,806` and `src/racing/race_logic.c:1265,1269,1270,1273,1274,1275,1276`. That is the 16 `CameraCheckFunc` JALs. |
| `KartCheckFuncA`/`B`/`C`/`D` | `if (CheatSettings.CHEATE == 0) DrawKart()` / `DrawKart2P()` / `DrawKart3P()` / `DrawKart4P()` | Guard or replace the matching `DrawKart*` calls in `src/racing/skybox_and_splitscreen.c`, plus `src/ending/code_80281C40.c:85` |
| `KartCheckFuncA2`/`B2`/`C2`/`D2` | same guard around `DrawKartAfter()` / `DrawKart2PAfter()` / `DrawKart3PAfter()` / `DrawKart4PAfter()` | same, paired sites; ending at `code_80281C40.c:89` |
| `ResultsCheck` | draws the cheat code on the results screen for ~900 frames | Call from the results/ceremony draw — `ResultsSequence`, `podium_ceremony_actors.c:448-461` |

`QuickCamCode` is host (`OverKart.c:488`) and currently `return;`s on its first line, so the non-headcam path is a no-op. Leave that alone.

Note the letter/screen-count mapping: A = 1P, B = 2P, C = 3P, D = 4P. The earlier guess that B/C/D were later passes of the same routine was wrong — they are per-player-count draw variants.

#### 7b Non-JAL UI suppression

About 20 sites. The full table with computed RAM addresses and confidence marks is in `docs/overkart5_integration.md`.

**Method — do not guess.** The ROM→RAM mapping is per-segment (`mk64.ld:6,425-426,495-496`):

| ROM range | RAM |
|---|---|
| `0x001000`–`0x0F7510` | `0x80000400 + (ROM - 0x1000)` |
| `0x0F7510`–`0x123640` | `0x8028DF00 + (ROM - 0xF7510)` (racing) |
| `0x123640`–`0x12AAE0` | `0x80280000 + (ROM - 0x123640)` (ending) |

`ROM + 0x7FFFF400` is **main-segment only** and will silently misplace racing and ending patches.

For each site: disassemble `baserom.us.z64` at that ROM offset to get the real instruction, resolve the RAM address to a symbol via `build/us/mk64.map`, then find the C statement. Only then propose the edit.

Highest-confidence conversions first: the C→Z controller merge (`0x001584` → `check_controller`, `src/main.c:324-326`), the minimum-speed skip (`0x0090CC` → `cpu_speed_control.inc.c:161-163`), and the L/R Option-toggle button masks (`0x0B3806` / `0x0B385E` → `GSelController`, `menus.c:1324-1334`). Leave the `LI $a3, 1` texture-suppression sites for last; they need disassembly to identify the argument.

**Done when:** the menu behaves as OverKart5 does, with no `.org` patching anywhere in the tree.

---

### Batch 8 — personal OverKart build (`OverKartBuild=1`)

Land after batch 7. Prerequisites, in order:

1. The private-object plumbing from "Private sources" above, with `PROTEC_DIR` pointing at `Code/Protec/`. `mk64.ld` needs a `Cheat` entry in `OK5_EACH` gated on the flag, so the community link never requires it.
2. `Cheat.c` converted to C89 and compiling under IDO.
3. `Segment12.bin` and `PiracyWarning.MIO0` in the ROM-asset segment (batch 5), plus `Seg12BIN` / `Seg12END` declared.
4. `SaveGame.CheatSettings` — already in the frozen `SaveData` layout (`OverKartStruct.h:24-38`), so no EEPROM change.
5. The 7a hook conversions, which `Cheat.c` supplies the bodies for.

Then flip `make OVERKART_BUILD=1` and confirm both configurations still build. Do not let the community build start depending on the private object.

---

## Out of scope

PAL/JP (`PALOverKart3.asm`, `JP10.asm`), `Stock3.asm`, `ObjectArray.asm` (patches an object array to `0x80500000`, which is now the host window), `TitleMenu.c` / `.h` (verified unreferenced — see batch 2), Amped Up, and any Tarmac-side change.

`Protec.c` / `DPR` / `DPRSave` stay unwired; `ProtectMode` is inert in C today and the available `Protec.c` is an incomplete stub.

`docs/host_overrides.md:162-164` reserves KartCheck / CameraCheck for the host. That stays true — the host does own them; they now arrive via the private `Cheat.o` rather than being unimplemented.

---

## Compile / review (every code batch)

```
make AVOID_UB=1 COMPARE=0 -j
```

Once batch 8 lands, every code batch must build **both** configurations:

```
make AVOID_UB=1 COMPARE=0 -j
make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j
```

No `make clean` unless flags or the linker script force it. After `.inc.c` edits, save the parent `.c`. Re-check payload size in `build/us/mk64.map` after any batch that adds data. The `mk64.ld:871` assert is the backstop, not the plan.

Present the diff and wait for review. IDO errors go to the user verbatim — do not guess at a fix across more than one attempt.

---

## Related

| File | Use |
|---|---|
| `docs/overkart5_integration.md` | File inventory, patch table, symbol map, hazards |
| `docs/host_overrides.md` | Host contract, RAM map, `*Default` list |
| `src/OverKartLibrary/GameVariables/NTSC/StockAliases.h` | The twelve aliases to delete |
| `Code/Protec/` | Private, out-of-tree. `Cheat.c` only. Never commit the source. |
| `src/OverKartLibrary/OKExternal.h` | ROM label declarations |
| `OverKart5/BUILD.asm` | Blueprint for what was hooked |
| `docs/ampedup_library_handoff.md` | Parked, different workstream |
