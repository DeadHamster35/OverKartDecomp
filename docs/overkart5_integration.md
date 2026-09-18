# OverKart5 integration (LibraryDecomp) — reference

**Agent handoff (implement):** `docs/overkart5_host_handoff.md`. If the user says **continue** on OverKart5, that file is start-here (batches 0–8). This file is the inventory, patch tables, and hazard list.

Survey date 2026-09-16, against `OverKart5/` at `BUILD.asm` `OKBuild = 0` / `Build.h` `OverKartBuild = 0`.

NTSC only. `ok_HeaderROM` `0xBE9178`. `sizeof(OKHeader)` `0xD0`. Approval/review; one batch at a time.

---

## Goal

OverKart5 becomes a **guest** in this tree: its C compiles under IDO, links into `.overkart`, and reaches the game through the `*Default` seam and direct decomp edits. `BUILD.asm` is retired as a build input and kept only as documentation of intent.

**Both products are in scope.** `OKBuild` and `OverKartBuild` are one switch: `0` = Tarmac Patch (community), `1` = OverKart (personal). Keep every `#if OverKartBuild` block and drive the flag from the Makefile.

Not in scope: Tarmac changes, Amped Up, PAL/JP, `TitleMenu.c` (verified unreferenced), `Protec.c` / `DPR` / `DPRSave`.

---

## File inventory

Line counts are logical lines. **Every** file is CRLF today. Destination is relative to this tree.

### Compiled into the payload (`BUILD.asm:302-334` `.importobj` order)

| Source | Lines | Destination | Notes |
|---|---|---|---|
| `OverKartVariables.c` / `.h` | 246 / 76 | `src/OverKart5/` | 26 `__attribute__((aligned(16)))`. Menu tables, `SaveGame`. |
| `MarioKartAI.c` / `.h` | 206 / 8 | `src/OverKart5/` | Smallest real TU. Start here. |
| `OKMenu.c` / `.h` | 1,697 / 28 | `src/OverKart5/` | Biggest behavioral TU. Menu controllers, `MiniMapDraw`. |
| `MarioKartPractice.c` / `.h` | 746 / 10 | `src/OverKart5/` | Flycam + save states. Contains the instruction pokes. |
| `LitroFunc.c` / `.h` | 69 / 16 | `src/OverKart5/` | Sprite DMA from ROM labels. |
| `OverKart.c` / `.h` | 1,254 / 20 | `src/OverKart5/` | Entry points. Import **last**. |
| `OverKartObjectHandler.c` / `.h` | 466 / 14 | `src/OverKart5/` | Draw/collide/coin overrides. |
| `data/ModelData/ModelData.c` / `.h` | 7,291 / 52 | `src/OverKart5/data/ModelData/` | Generated Gfx + textures, 446 KB. |
| ~~`TitleMenu.c` / `.h`~~ | 4,953 / 19 | **do not import** | Verified unreferenced. Superseded by the `Splash3D` ROM blob at `OverKart.c:1028-1037`. See "TitleMenu" below. |

### Headers and data-only

| Source | Lines | Destination | Notes |
|---|---|---|---|
| `OKInclude.h` | 16 | `src/OverKart5/` | Umbrella. Drop the `TitleMenu.h` include at `:19`. |
| `Build.h` | 9 | `src/OverKart5/` | Product flags. Keep verbatim. |
| `OverKartStruct.h` | 30 | `src/OverKart5/` | `SaveData` = EEPROM layout. Frozen. |
| `BlockFortPaths.C` / `.H` | 1,252 / 50 | `src/OverKart5/` as `.c` / `.h` | Rename case; Makefile globs `*.c`. |
| `CustomMenu/GFX/MenuGFX.h` | 6 | `src/OverKart5/CustomMenu/GFX/` | Offset macros only. |
| `data/SplashLogo/TitleMenu.h` | 3 | `src/OverKart5/data/SplashLogo/` | Size macros for the `Splash3D` blob. **Line 1 is `extern const int 1;` — malformed.** Fix or delete that line; keep the two `#define`s. |
| `textures/menu/output/MenuGFX.h`, `ImageData.h` | 6 each | — | Tarmac output duplicates of the CustomMenu header. Probably skip. |

### TitleMenu — verified droppable

`TitleMenu.c:4067-5033` defines 17 `Gfx Draw_*` display lists. Nothing outside `TitleMenu.c` references any of them: the only other matches in the tree are `TitleMenu.h`'s own externs and three stale duplicate declarations of `Draw_DefaultMaterial_T` at `data/ModelData/ModelData.h:1,22,41` — and `ModelData.c` never uses that symbol. The definition at `TitleMenu.c:4067` is an empty list (`gsSPEndDisplayList()` only).

The live title geometry comes from `data/SplashLogo/TitleMenu.bin` (69,992 B, MIO0), DMA'd and decompressed into segment 8 at `OverKart.c:1028-1037` under `#if OverKartBuild`. Drop the C file, drop `OKInclude.h:19`, and delete the three stale `ModelData.h` externs. Saves 4,953 lines and 318 KB of source plus its payload footprint.

### Not in `BUILD.asm` — confirm before importing

| Source | Lines | Why it is questionable |
|---|---|---|
| `grayscale.c` | 23 | Declared `OverKart.h:6`, never called. |
| `SpareCode.c` | 12 | Debug hex printer reading `0x80700000` (= FB2 in this tree). |
| `Repro.c` | 53 | Flash detect stub. `return;` in an `int` function at `:17-18`. |
| `Notes.txt` | 38 | Stale RAM/ROM notes. Superseded by `docs/host_overrides.md`. |
| `Stock3.asm`, `PALOverKart3.asm`, `JP10.asm`, `ObjectArray.asm` | 142 / 130 / 173 / 405 | Legacy Armips. `ObjectArray.asm:5` points an object array at `0x80500000` = host window now. |
| `CustomMenu/MenuBuild.asm` | 5 | Alternate `MenuIconsROM` import, not in main `BUILD.asm`. |

---

## IDO defect counts

`-Xcpluscomm` (`Makefile:389`) permits `//`. `include/PR/ultratypes.h:8-20` defines `bool` / `true` / `false`. Float `f` suffixes are valid C89. Nothing else on this list is optional.

| Defect | Count | Representative sites |
|---|---|---|
| CRLF line endings | all 37 files | — |
| `for (int i = ...)` | ~40 | `OverKart.c:67,81,91,99,310,377,443,454,465,474,515,1059,1066`; `OKMenu.c:70,167,860,922,1170,1197,1215,1226,1240,1246,1465,1470,1506,1826`; `MarioKartPractice.c:282,308,497,890`; `MarioKartAI.c:170,208`; `OverKartObjectHandler.c:75,469,490,518`; `grayscale.c:8`; `SpareCode.c:5` |
| Declaration after statement | ~15 | `MarioKartAI.c:116-117`; `OverKart.c:406,419`; `OverKartObjectHandler.c:11,193,386,411`; `OKMenu.c:94,853,1083,1150,1825,1832-1837` |
| `__attribute__((aligned(16)))` | 26 | `OverKartVariables.c:106,108,110,112,115,118,120,122,126,128,131,143,155,166,179,181,185,198,212,225,240,249,259,268,281,289` |
| Malformed declaration | 1 | `data/SplashLogo/TitleMenu.h:1` |
| `return;` in non-void | 1 | `Repro.c:17-18` |
| Declared, never defined | 1 | `LitroFunc.h:10` `loadTitleMenuFrame` |
| Signature mismatch vs Library | 1 | `Draw3DRacer` — `OKCustomObjects.h:4` says `(void)`, `OverKartObjectHandler.c:278` defines `(uint, uint)` |

Not present anywhere in the tree, so no work needed: designated initializers, compound literals, `inline`, `long long`, variadic macros, `#pragma`, `__asm__`, GCC builtins, VLAs, zero-length arrays, anonymous unions, trailing enum commas.

Counts above cover `OverKart5/` only. The private `Cheat.c` needs the same treatment and has its own table under "Private sources".

---

## Symbol map

### Host overrides a `*Default` (11)

The twelve host-owned `#define`s at `StockAliases.h:90-104` get **deleted**; the host defines the real short name and the `*Default` body forwards to it. See "Aliases" in the handoff for the ordering constraint.

| Host symbol | Host file | `*Default` | Decomp call site |
|---|---|---|---|
| `gameCode` | `OverKart.c:551` | `gameCodeDefault` | `src/main.c:602` |
| `allRun` | `OverKart.c:802` | `allRunDefault` | `src/main.c:1220` |
| `PrintMenuFunction` | `OverKart.c:1226` | `PrintMenuFunctionDefault` | `src/main.c:926` |
| `ExecuteItemHook` | `OverKart.c:389` | `ExecuteItemHookDefault` | `src/racing/actors_extended.c:978` |
| `titleMenu` | `OKMenu.c:782` | `titleMenuDefault` | `src/menu_items.c:2395` |
| `MiniMapDraw` | `OKMenu.c:1820` | `MiniMapDrawDefault` | `src/main.c:867` |
| `DrawPerScreen` | `OverKartObjectHandler.c:6` | `DrawPerScreenDefault` | `src/racing/actors.c:2609` |
| `CheckHit` | `OverKartObjectHandler.c:90` | `CollideObjectDefault` | `src/racing/actors.c:2277` |
| `ItemboxCollideCheck` | `OverKartObjectHandler.c:191` | `ItemboxCollideCheckDefault` | via `CollideObjectDefault` |
| `DisplayObject` | `OverKartObjectHandler.c` | `DisplayObjectDefault` | `src/racing/actors.c:2597` |
| `RubberBandMan` | `MarioKartAI.c:112` | `RubberBandManDefault` | `cpu_speed_control.inc.c:208` |

### Host-owned, no vanilla caller — host must invoke

`DrawMapSelectDefault`, `MapSelectMenuDefault` (both in `OverKartDefaults.c`, documented as "not called from vanilla" in `docs/host_overrides.md:112-113`).

### Replaced whole vanilla functions — no seam exists

| Host symbol | Host file | `BUILD.asm` | Vanilla target |
|---|---|---|---|
| `DataMenuController` | `OKMenu.c:1716` | `:264` | DATA-menu branch of the controller dispatch, `menus.c:206-207` |
| `TitleMenuSwitch` | `OKMenu.c:1731` | `:267` | `TitleController`, `menus.c:1028+` |
| `GameSelectSwitch` | `OKMenu.c:1689` | `:270` | `GSelController`, `menus.c:1273+` |
| `MapSelectSwitch` | `OKMenu.c:1680` | `:276` | `MSelController`, `menus.c:234` |
| `PlayerSelectSwitch` | `OKMenu.c:1664` | `:273` **commented out** | `PSelController` — leave vanilla alone |

### Already native — delete the host copy

| Host symbol | Host file | Native equivalent |
|---|---|---|
| `ClearFramebufferCheck` | `OverKart.c:1374` | `g_ClearCFBFlag` guards in `src/main.c:703,749,821` and `skybox_and_splitscreen.c:285` |
| `ScreenDrawHook` | `OverKart.c:1366` | `PrintMenuFunctionDefault` is called directly from `src/main.c:926` |

### Already in the Library — do not duplicate

`ModularMenu` (`MarioKartMenu.c:235`), `OKMenu` / `OKPanel` / `OKOption` (`OKStruct.h:249-269`), `printMap` (`MarioKartMenu.c:131`), `DrawFPS` (`MarioKartMenu.c:220`), `DisplayCoinSprite` (`OKCustomObjects.c:16`), `copyCourseTable`, `swapHS`, `setSky`, `setWater`, `loadMinimap`, `LoadCustomHeader`, `SetCustomData`, `loadBigFont`, `runDMA` / `runMIO` / `runRAM`, `SetSegment`, `GetRealAddress`.

### Host-owned, Library has only a declaration

`printDetails` (`MarioKartMenu.h:5`), `Draw3DRacer` (`OKCustomObjects.h:4`, wrong arity), `CheckHit` (`LIBRARYBUILD4.asm:57-58` weak alias, unreferenced in this tree's Library C), `loadCoin`, `RedCoinChallenge`, `GoldCoinChallenge`, `DropCoins`, `printAnticheat`.

---

## `BUILD.asm` patch table

`OKBuild = 0` at `BUILD.asm:14`, so only the always-on patches below are live. RAM addresses use the per-segment mapping from `mk64.ld:6,425-426,495-496`:

| ROM range | RAM |
|---|---|
| `0x001000`–`0x0F7510` | `0x80000400 + (ROM - 0x1000)` |
| `0x0F7510`–`0x123640` | `0x8028DF00 + (ROM - 0xF7510)` (racing) |
| `0x123640`–`0x12AAE0` | `0x80280000 + (ROM - 0x123640)` (ending) |

`ROM + 0x7FFFF400` is main-segment only. Using it on a racing offset is wrong by ~0x96000.

### Live JAL patches

| ROM | RAM | Target | Status here |
|---|---|---|---|
| `0x10EA20` | `0x802A5410` | `ClearFramebufferCheck` | **Already native** — `g_ClearCFBFlag` |
| `0x009400` | `0x80008800` | `RubberBandMan` | **Already native** — `cpu_speed_control.inc.c:208` |
| `0x0B105C` | `0x800B005C` | `DataMenuController` | Needs `menus.c` edit |
| `0x0B10C0` | `0x800B00C0` | `TitleMenuSwitch` | Needs `menus.c` edit |
| `0x0B10D8` | `0x800B00D8` | `GameSelectSwitch` | Needs `menus.c` edit |
| `0x0B1108` | `0x800B0108` | `MapSelectSwitch` | Needs `menus.c` edit |
| `0x0B10F0` | `0x800B00F0` | `PlayerSelectSwitch` | Commented out. Skip. |

### Live non-JAL patches

Confidence is about the **decomp target**, not the patch itself. Anything below High needs `baserom.us.z64` disassembly plus `build/us/mk64.map` before you propose an edit.

| ROM | RAM | Patch | Vanilla behavior removed | Decomp target | Conf. |
|---|---|---|---|---|---|
| `0x001584` | `0x80001584` | `ori $t4, $a1, 0` | C-buttons merged into Z | `check_controller`, `src/main.c:324-326` | High |
| `0x0090CC` | `0x800084CC` | `.word 0x100000CA` | Minimum-speed auto-accel | `cpu_speed_control.inc.c:161-163` (`AccelOn` low-speed block) | Med-High |
| `0x0B3806` | `0x800B2806` | `.halfword 0x0080` | L → Option menu (impossible mask) | `GSelController`, `menus.c:1324-1334` | Med-High |
| `0x0B385E` | `0x800B285E` | `.halfword 0x0080` | R → Data menu | same | Med-High |
| `0x09593C` | `0x8009493C` | `.word 0x100001FC` + NOP | DATA menu entry; background only | `SetFadeOutTaData` / `KBGNumber = DATA_MENU`, `menu_items.c:5330-5332` | Med |
| `0x0A8970` | `0x800A8870` | NOP | R Mario Raceway time on title | draw call reading `g_mracewayTime` | Med |
| `0x095F40` | `0x80094F40` | `LI $a3, 1` | Map-select texture | menu OBJ draw dispatch | Low |
| `0x095E0C` | `0x80094E0C` | `LI $a3, 1` | Player-select texture | same | Low |
| `0x095CB8` | `0x80094CB8` | `LI $a3, 1` | Game-select texture | same | Low |
| `0x0960D0` / `E4` / `F8` | `0x800950D0`… | `LI $a3, 1` ×3 | Map-select TT / ghost overlays | same | Low |
| `0x096118` | `0x80095118` | `LI $a3, 1` | GP CC overlay label | same | Low |
| `0x095D2C` / `D40` | `0x80094D2C` / `D40` | NOP ×2 | Options/Data menu icons | icon draw JALs | Low |
| `0x0B386C` / `C4` | `0x800B286C` / `C4` | NOP ×2 | Options/Data menu sounds | sound JALs in those handlers | Low |
| `0x095CA4` | `0x80094CA4` | commented | — | Not applied. Skip. | — |

### `.if OKBuild` block — in scope, and simpler than the offsets suggest

`BUILD.asm:26-149`: `ResultsCheck` ×1, `CameraCheckFunc` ×16, `KartCheckFuncA` ×5, `KartCheckFuncA2` ×5, `KartCheckFuncB` ×3, `KartCheckFuncB2` ×3, `KartCheckFuncC`/`C2`/`D`/`D2` ×1 each, plus `0x095C64` NOP (title flip) and `0x09F8C4` NOP (default title image).

All eleven functions are defined in the private `Code/Protec/Cheat.c` — see "Private sources" below. Each is a thin guard around a vanilla call the decomp already makes by name, so **none of these 39 ROM offsets need to be resolved**:

| Symbol | `Cheat.c` | Body | Decomp target |
|---|---|---|---|
| `CameraCheckFunc` | `:513` | `CameraControl(...)` then headcam (`CHEATF`) or `QuickCamCode(...)` | the `CameraControl` call sites: `src/main.c:624,686,688,732,734,800,802,804,806`; `src/racing/race_logic.c:1265,1269,1270,1273-1276` |
| `KartCheckFuncA` | `:557` | `if (CHEATE == 0) DrawKart()` | 1P `DrawKart` sites, `skybox_and_splitscreen.c`; ending `code_80281C40.c:85` |
| `KartCheckFuncB` | `:566` | `… DrawKart2P()` | 2P sites |
| `KartCheckFuncC` | `:575` | `… DrawKart3P()` | 3P sites |
| `KartCheckFuncD` | `:584` | `… DrawKart4P()` | 4P sites |
| `KartCheckFuncA2` | `:593` | `… DrawKartAfter()` | paired; ending `code_80281C40.c:89` |
| `KartCheckFuncB2` | `:602` | `… DrawKart2PAfter()` | paired |
| `KartCheckFuncC2` | `:611` | `… DrawKart3PAfter()` | paired |
| `KartCheckFuncD2` | `:620` | `… DrawKart4PAfter()` | paired |
| `ResultsCheck` | `:711` | `DrawCheatCode` on the results screen for ~900 frames | `ResultsSequence`, `podium_ceremony_actors.c:448-461` |

**Correction to the earlier survey:** A/B/C/D are per-player-count draw variants (1P/2P/3P/4P), not successive passes of one routine. The `CameraCheckFunc` sites are `CameraControl` calls, which is why there are 16 of them across the main and racing segments.

`docs/host_overrides.md:162-164` reserves KartCheck / CameraCheck for the host. Still true — the host owns them, and they arrive via the private object.

---

## Private sources — `Code/Protec/`

Closed-source, kept **outside both project folders** on purpose so it cannot be committed by accident. Not part of this repo and must not become part of it. Only the compiled object ships; `OverKart5/.gitignore` already does this with `*.o` + `!Protec.o`.

| File | Lines | Status |
|---|---|---|
| `Code/Protec/Cheat.c` | 945 | Live. Supplies the whole cheat API **and** all eleven `.if OKBuild` hook functions. |
| `Code/Protec/Protec.c` | 47 | **Stub.** Defines only `DPR2()`, with `return;` in a `uint` function (`:9,19,30,44`) and references to `usb_io_read`, `usb_cart`, `buff`, `D64_*`/`ED_*`/`SC64_*` that exist in neither tree. Does **not** define `DPR` / `DPRSave`. |
| `Code/Protec/Untitled-1.txt` | 40 | Unrelated scratch. Ignore. |

`Cheat.c` includes `../Library/MainInclude.h` and `../OverKart5/OKInclude.h`, so under this tree those become `MainInclude.h` and `OKInclude.h` on the include path.

### `Cheat.c` symbol inventory

| Group | Symbols |
|---|---|
| Declared in `OverKart.h:13-19` | `CheatSetup` `:5`, `CheckCheat` `:48`, `DrawCheatMenu` `:321`, `CheatMenuHandler` `:254`, `ApplyCheats` `:862`, `LoadCheatData` `:474`, `DrawCheat3D` `:726` |
| File-internal | `CheckFinalKey` `:194`, `ResetKey` `:214`, `CheckKeyEntry` `:224`, `DrawCheatCode` `:630`, `CopyBump` `:486` |
| `.if OKBuild` hooks | `CameraCheckFunc` `:513`, `KartCheckFuncA/B/C/D` `:557,566,575,584`, `KartCheckFuncA2/B2/C2/D2` `:593,602,611,620`, `ResultsCheck` `:711` |
| Data | `CheatKey[]` `:13`, `PlayerModels[]` `:31`, `StoredKey[6]` `:73`, `JumboCheat`/`TinyCheat`/`TurboCheat`/`SlowCheat`/`ThreeDCheat`/`FirstPerson` `:89-147`, `CheatKeys[]` `:149`, `CheatUnlocked[]` `:159`, `CheatActive[]` `:169`, `CheatNames[]` `:179`, `ActiveValue[]` `:188` |

Consumes `SaveGame.CheatSettings.CHEATA`–`CHEATF` (`OverKartStruct.h:24-26`), `cheatMode[]`, `LastMenuID`, `HotSwapGP`, `g_SequenceMode`, and host `QuickCamCode` (`OverKart.c:488`, which `return;`s immediately — the non-headcam path is a no-op).

Needs `Seg12BIN` / `Seg12END` at `:473-483` → `data/Segment12.bin`.

### IDO defects in `Cheat.c`

| Defect | Sites |
|---|---|
| `for (int …)` | `:7,196,198,372,389,500,527,635,742,885,898,914,928` |
| Mid-block declarations | `:354-355` (`int PlotX/PlotY`), `:535-537` (`float ux/uy/uz`), `:716` (`int CheatSelect`), `:732` (`short PlayerCountLocal`), `:744` (`float Distance`), `:870` (`short PlayerCountLocal` after a statement) |
| CRLF | whole file |

### Build handling

Source stays external; only a prebuilt object is committed. Sketch:

- `PROTEC_DIR ?= ../Protec` in the Makefile. If `$(PROTEC_DIR)/Cheat.c` exists, compile it to `$(BUILD_DIR)/src/OverKart5/Cheat.o`; otherwise copy a committed `lib/private/Cheat.o` to that path. `mk64.ld` only ever names the build path.
- `.gitignore:2` ignores `*.o` tree-wide, so add `!/lib/private/*.o` in the style of the existing negations at `.gitignore:94-96`.
- Add defensive ignores for `/src/OverKart5/Cheat.c`, `/src/OverKart5/Protec.c`, `/src/OverKart5/private/*`.
- Build the committed object with **IDO**, not OverKart5's GCC. This tree is `-G 0 -non_shared`; object-level flag mismatches fail at link or at runtime, not with a diagnostic.
- With `OverKartBuild=0` nothing references these symbols — the community build must not require the object at all.

A committed `.o` withholds source but is still disassemblable. Say so in the proposal rather than implying it is protected.

---

## ROM asset labels

`src/OverKartLibrary/OKExternal.h` already declares these. The table below is definition status **in this tree**.

| Label | Blob | Defined here? |
|---|---|---|
| `Crash` / `CrashEnd` | `test/Crash512.bin` | Yes — `data/overkart/ok_rom_assets.s:40-42` |
| `NiceFontROM` | `nice_font.mio0.bin` | Yes — `:22-23` |
| `HudButtonsROM` | `hud_buttons.mio0.bin` | Yes — `:25-26` |
| `LibraryFont` / `AmpedUpFont` | `big_font.mio0.bin` / `AmpedUpFont.mio0.bin` | Yes — `:29-37` |
| `IceKageROM`, `previewN`, `previewU` | — | Yes |
| `LogoROM` | `data/KimuraBless.bin` (14,644 B) | **No** |
| `RCIconMap` | `data/RedSquare.png.RAW` (128 B) | **No** |
| `RCSpriteROM` | `data/RedCoinSprite16.png.MIO0` (148 B) | **No** |
| `ArrowsSpriteROM` | `data/arrows.png.MIO0` (476 B) | **No** |
| `NumbersSpriteROM` | `data/number_sprites.png.MIO0` (1,116 B) | **No** |
| `ROptionROM` / `ROptionEnd` | `data/ROption.MIO0` (1,276 B) | **No** |
| `MenuIconsROM` / `MenuIconsEnd` | `data/SplashLogo/OKLogo.bin` (40,296 B) | **No** |
| `Splash3D` / `Splash3DEnd` | `data/SplashLogo/TitleMenu.bin` (69,992 B) | **No** |
| `Pirate` / `PirateEnd` | `data/PiracyWarning.MIO0` (43,912 B) | **No** — `OverKartBuild=1` |
| `Seg12BIN` / `Seg12END` | `data/Segment12.bin` (60,872 B) | **No**, and not declared in `OKExternal.h`. `OverKartBuild=1`; `Cheat.c:473` declares it locally. |
| `JP_Bank` / `JP_Audio` | `data/JP_Bank.bin` (17,728 B) / `data/JP_Audio.bin` (2,409,856 B) | **No** — 2.4 MB, confirm first |

Not declared in `OKExternal.h`, referenced only by `BUILD.asm`: `BigFontROM` (`data/Newfont.MIO0`, a third font next to `LibraryFont` / `AmpedUpFont`), `RAMCheck` (debug).

C consumers: `LitroFunc.c:9-43` (`ArrowsSpriteROM`, `RCSpriteROM`, `NumbersSpriteROM`), `OverKart.c:23` (`LogoROM`), `OverKart.c:939,1029,1039,1113,1179` (`Pirate`, `Splash3D`, `MenuIconsROM`, `ROptionROM`), `OKMenu.c:1842` (`RCIconMap`).

---

## RAM hazards

| Host reference | Problem | Fix |
|---|---|---|
| `ok_Storage` — `OverKart.c:180,943,1033,1109,1118,1184,1251` | Removed from this tree (`docs/host_overrides.md:172`) | New host BSS `ok5_MenuStorage` in `0x80500000`–`0x805DA800`, sized for the largest decompressed blob. Cannot alias `ok_FreeSpace` — several sites use that as the DMA source in the same call. |
| `GlobalAddressD = 0x80690000` — `OverKart.c:568` | Inside `gFaceBuffer` → Vtx (`0x80625800`–`0x80700000`) | Give it real storage. Do not keep the literal. |
| `SpareCode.c:7-9` reads `0x80700000` | That is FB2 | Drop the file, or repoint. |
| `ObjectArray.asm:5` `Object_Array = 0x80500000` | Now the host overflow window | Legacy; out of scope. |
| `MarioKartPractice.c:697-733` instruction pokes | Non-matching build; addresses not guaranteed | Batch 6c in the handoff — all four decoded. |
| `MarioKartPractice.c:461,470` `0x8007AC54` | Branch-word poke around `RouletteStart` | Convert to a flag. |
| `MarioKartPractice.c:708,732` `0x8018CAB0` | `playerHUD[0] + 0x40` (`g_hudStruct` = `0x8018CA70`) | Typed struct copy through `playerHUD`. |
| `OKMenu.c` `asm_DispOBSubPSelCursor1`, `asm_BlinkCheck` (20 sites) | Function-pointer-table and branch-word writes into player-select code | In `undefined_syms.txt:52-53`, so they link. Verify against the map; prefer native. |
| `OverKart.c:155-156` `asm_SongA` / `asm_SongB` | Declared `GameOffsets.h:612-613`, **absent from `undefined_syms.txt`** — link error | Find the decomp statement rather than adding the symbols. |
| `OKMenu.c:894-1249` literals `0x800A08C8`, `0x800A00FC`, `0x800A01BC` | Vanilla function addresses stored into a table | Resolve to symbols via the map; use `&function`. |

Payload budget: `.overkart` starts at `0x80400000`, OKHeader BSS follows, and `mk64.ld:871` asserts `OKHeaderEND <= 0x805DA800`. OKHeader BSS is roughly `0x8E000`. Record real numbers from `build/us/mk64.map` in batch 0, and again after importing `ModelData.c` — it is the single largest contributor now that `TitleMenu.c` is dropped.

---

## Order

```
0 confirm payload budget + file scope
  → 1 build plumbing + delete 12 aliases + MarioKartAI
  → 2 data/leaf TUs (Variables, LitroFunc, ModelData, BlockFortPaths; skip TitleMenu)
  → 3 gameplay TUs (ObjectHandler, OKMenu, Practice, OverKart.c last)
  → 4 entry-point rewiring (*Default forwarders + menu controllers)
  → 5 ROM assets
  → 6 RAM + self-modifying-code hazards
  → 7a OKBuild hooks via Cheat.c bodies  → 7b non-JAL UI suppression
  → 8 OverKartBuild=1 (private object + Segment12 + PiracyWarning)
```

Batches 5 and 6a are prerequisites for 3d — `OverKart.c` will not link or run without the ROM labels and an `ok_Storage` replacement.

Alias deletion (batch 1) cannot land before the host exists: `OverKartDefaults.c:40` and `SharedFunctions.h:35-39` depend on it.
