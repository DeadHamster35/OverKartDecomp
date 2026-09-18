# Host integration post-mortem (OverKart5 → LibraryDecomp)

**Audience:** Cursor agents integrating a **new** host project into a fresh OverKart Library / mk64-decomp fork. Assume you do **not** have the OverKart5-specific edits from this tree yet — only the Library defaults contract and a historical GCC/Armips host as a blueprint.

**Context:** OverKart5 was the first host linked natively (IDO ELF, no Armips/`BUILD.asm`/ROM `.org`). This document records what worked, what broke, root causes, and how to avoid repeating those failures.

Related reading once a plan exists: `docs/host_overrides.md` (RAM map, `*Default` contract). OverKart5-specific plans that already exist in *this* repo (`docs/overkart5_host_handoff.md`, `docs/overkart5_integration.md`) are examples of a good batch plan — write the equivalent for your host before importing files.

---

## 1. Verdict

| Outcome | Detail |
|---|---|
| Success | Custom courses load (TT/VS/GP), host menus replace stock, Library hooks go through `*Default` forwarders, personal `OverKartBuild=1` can link closed-source cheat code from out-of-tree IDO sources. |
| Cost | Multiple multi-session bug loops after “link succeeded,” mostly in **load path**, **menu graphics**, **GP-only cameras**, and **symbol/type mismatches** — not in Makefile plumbing. |
| Main lesson | Linking the host is the easy half. The hard half is every place the old ROM-mod assumed **stock ROM bases, stock RAM labels, 4 human cameras, or GCC types**. |

---

## 2. What went right (repeat these)

1. **Batch plan before import.** Measure payload room first. Do not import large host TUs until ROM assets and menu storage replacements exist.
2. **One toolchain (IDO), one ELF.** Host under `src/<HostName>/`, Library under `src/OverKartLibrary/`. Vanilla includes `OverKartHooks.h` only.
3. **`BUILD.asm` as blueprint, not as patcher.** Convert each `.org`/`JAL` to a decomp call site or `*Default` body. Never reintroduce Armips into the decomp tree.
4. **Delete `StockAliases.h` `#define`s in the same batch as host definitions.** Half-migrated aliases cause silent wrong symbols or dual definitions.
5. **Out-of-tree private sources.** Never commit closed-source `.c` into the decomp repo. Prefer an IDO-specific copy (`Cheat_ido.c`) so GCC host trees stay untouched.
6. **Approval → implement → present diff.** Prevents drive-by refactors and keeps the user as the review gate.
7. **Smoke both product flags** when the host has a personal/community switch (`OVERKART_BUILD=0` and `=1`).

---

## 3. Process failures (how the work went wrong)

### 3.1 “It links” was treated as “custom levels work”

Custom course play depended on Library paths that were already present, plus host cup/banner/load wiring. Agents assumed Batch N complete meant gameplay. Several load crashes only appeared after full map select → race.

**Mitigation:** Define explicit smoke gates per batch, e.g.:

- Boot to host title  
- Map select + stock course race  
- HotSwap custom course TT  
- Custom GP (8 karts)  
- 2P custom GP  
- Banner restore C-Left / C-Right  

Do not start the next integration batch until the current smoke gate passes.

### 3.2 Fixes without a progress meter wasted time

Early load crash dumps (AddressError in MIO0-ish code) were ambiguous. Forcing course IDs caused a **full emulator crash with no dump**. Only a stage+name poke at a known safe RAM window (`0x80550000` in the host overflow, below FB0) identified the failing function (`MapTex` / `decodemapimage2`).

**Mitigation:** On the first mysterious load crash, install a **write-before-call** progress meter (stage word + short ASCII name) in a documented safe scratch region. Write stage **first**. Remove the meter after the bug is fixed. Do not “force IDs everywhere” as a first diagnostic.

### 3.3 Stock RAM labels from `GameOffsets.asm` were treated as live

`GameOffsets.asm` is a **stock NTSC RAM map** (`0x80xxxxxx`), not this build’s BSS. `COMPARE=0` / `AVOID_UB=1` moved objects. Copying `.definelabel` values into C, `undefined_syms.txt`, or assuming they match live pointers causes undefs or wrong stores.

**Mitigation:** Resolve Library names to **decomp symbols** (or a `StockAliases.h` `#define` onto a decomp symbol). Mid-function stock RAM is a code site (SMC), not BSS. Never add stock RAM literals to `undefined_syms.txt` “to make the link succeed.”

### 3.4 Temporary integration scripts littered the tree

Import/parse helpers (`_ok5_*.py`, `_parse_*.py`) accumulated and outlived their usefulness.

**Mitigation:** Put one-shot converters under `tools/scratch/` or delete them when the batch that needed them merges. Do not leave underscore-prefixed scripts in the repo root.

### 3.5 Context loss across sessions

Long integrations span many chats. Summaries omitted that MapStartup already forced course 0, or that banner dest mapping had already been corrected by the user.

**Mitigation:** Keep a living `docs/<host>_host_handoff.md` with **Done / Blocked / Do not re-litigate** sections. Agents must read it before proposing the next batch.

---

## 4. Bug catalog — root causes and mitigations

These are the failure modes that cost the most time on OverKart5. Future hosts will hit analogs even if filenames differ.

### 4.1 Custom course texture DMA used the wrong ROM base

**Symptom:** AddressError / TLB during `decodeMIO0` after geometry loaded. Progress meter last stage: map textures (`decodemapimage2` / `MapTex`).

**Root cause:** Stock `decodemapimage2` always does `_other_texturesSegmentRomStart + SEGMENT_OFFSET(ptr)`. Tarmac writes Segment 9 texture pointers as `(trueROM - 0x641F70)` — the frozen **stock** start of the standalone texture bank — not as `0x0F` segmented addresses relative to the **live** linker symbol. On a shifted ROM, those are different. Adding the live base double-applied the base and fed garbage to MIO0.

**What made it worse:** Applying the same HotSwap remap to `DecodeMapImage1` (stock object/particle textures, true `0x0F` pointers). Course load then finished, then crashed later on trees/cows.

**Mitigation:**

- Document the authoring tool’s address convention (absolute ROM, stock-base offset, or segmented).
- Branch **only** the loader used by course Segment 9 tables.
- Leave stock particle/object texture loaders on `_other_texturesSegmentRomStart`.
- Prefer `stock_base + stored_offset` for tool-authored offsets; do not assume `SEGMENT_OFFSET` is enough if offsets can exceed 24 bits.

### 4.2 Cup banners loaded into the wrong RAM banks

**Symptom:** Custom banners in wrong slots; restore stomped wrong courses.

**Root cause:** Agents assumed banner dest = cup table course ID. Original OverKart copied banners **sequentially** into fixed menu banks (starting at Luigi / slot 8), not by course ID. Restore must use the **live** cup table to pick stock TKMK sources, while dest stays sequential.

**Mitigation:** Before changing menu graphics, read the host’s original load loop (or disassemble the Armips-era path). Prefer “what did the ROM mod actually DMA?” over “what would a clean design do?”

### 4.3 Course header in row 0 while `load_course` used cup table IDs

**Symptom:** Wrong course data / MIO0 crash when HotSwap > 0; skip path if `g_courseID == g_loadedcourseFlag`.

**Root cause:** Custom headers patch `g_courseTable[0]` (battle row 15), but GP race init still assigned `g_courseID` from `gCupCourseOrder` and set `g_loadedcourseFlag` before `MapStartupDefault`’s force-to-0/15. Timing holes caused stock Choco (etc.) to load with a custom header, or skipped reload.

**Mitigation:** Trace `g_courseID` / `g_loadedcourseFlag` / HotSwap across **menu frame**, **race init**, and **MapStartup**. Smoke GP after TT. Do not “force course 0 everywhere” without understanding skip logic — that caused blank emulator crashes here.

### 4.4 GP crash in `SetCamShiftUp` (not OKObjects)

**Symptom:** AddressError on store `VA …85E` (unaligned), PC in OverKart payload, after course appeared loaded. VS/TT fine; GP / 2P GP fail. Sprites OK.

**Root cause:** `CheckPaths` loops players `0..7` and calls `SetCamShiftUp`, which writes `GlobalCamera[playerID]->camera_vector[1]`. Only **human** players have cameras (`GlobalCamera[4]`). Dump `A0 == 5` was the smoking gun. User refinement: gate on `flag & IS_PLAYER`, not merely `playerID <= 3`, so AI in slots 0–3 also never poke cameras.

**Mitigation:**

- Any host/Library path that loops **8** racers must separate “exists” from “has camera / is human.”
- Prefer `IS_PLAYER` (and ghost checks where needed) at the camera poke site, not only at the call site.
- Map crash PC/RA with `mk64.us.map` before guessing “OKObjects.”

### 4.5 Closed-source cheat TU failed IDO link/compile

**Symptoms:**

- `FreeSpaceAddress` undeclared (`ok_Storage` removed in this tree).
- `CameraCheckFunc` `char` vs `s8` conflict with decomp headers.
- `g_SequenceMode` undefined at link.

**Root causes:** GCC host still used removed storage names and loose types; stock sequence name ≠ decomp `gGamestate` (same stock RAM as `g_resetToggle`). Local `#define RESULTSEQUENCE 5` matches decomp `ENDING`.

**Mitigation:**

- Maintain a separate IDO source file for private code (`Cheat_ido.c`); do not force GCC trees to match IDO.
- Replace removed storage with the host’s documented buffer (`ok5_MenuStorage` pattern).
- Match decomp prototypes (`s8`, `Player*`, etc.).
- Map stock names via `StockAliases.h` → decomp symbols; delete conflicting `GameOffsets.h` externs when adding `#define`s.
- Never invent `undefined_syms.txt` entries for stock BSS.

### 4.6 Font toggle confusion / duplicate blobs

**Symptom:** Three `AMPEDUP_FONT` options appeared to be the same font.

**Root cause:** `AmpedUpFont.mio0.bin` and OverKart5 `NewFont.MIO0` were byte-identical; a third path (`BigFontROM`) duplicated NewFont; `NiceFontROM` was an unused ROM source (RAM dest `nicefont` is unrelated).

**Mitigation:** Hash font blobs before wiring toggles. Keep two options max until content actually differs. Copy host assets into Library asset dirs if the Library segment owns the label. Keep RAM dest names (`nicefont`) even when ROM sources change.

### 4.7 Type / header dual declarations

**Symptom:** Conflicting types for `g_courseTable` (`struct CourseTable[]` vs `extern long`); `g_courseID` undeclared in a TU that only saw OverKart hooks.

**Root cause:** Mixing decomp `courseTable.h` with Library `GameOffsets.h` in the same TU; debug code used Library names in vanilla TUs without includes.

**Mitigation:** One naming world per TU. Vanilla → `OverKartHooks.h`. Library/host → `MainInclude.h`. Do not include both course table headers. Prefer passing already-local parameters into debug helpers instead of reaching for Library globals from vanilla files.

### 4.8 Premature “force the ID” diagnostics

**Symptom:** Full emulator exit, no AddressError screen.

**Root cause:** Aggressive course-ID overrides interacted badly with load/skip/state.

**Mitigation:** Progress meter first; surgical hypothesis second; global force last and only with a revert plan.

---

## 5. Recommended workflow for the next host (from scratch)

Use this as the agent checklist on a **clean** LibraryDecomp-style fork.

### Phase A — Contracts (no host code yet)

1. Read `docs/host_overrides.md` (or recreate it): payload base, FB/Z bounds, `ok_HeaderROM`, FreeMemory, object pool.
2. Inventory the host: C TUs, Armips hooks, ROM blobs, private out-of-tree sources, product flags.
3. Write `docs/<host>_host_handoff.md` with batched work, Done/Blocked, smoke gates.
4. Measure `.overkart` room vs estimated host size. Stop if budget is negative.

### Phase B — Plumbing

1. Makefile / linker: host SRC dirs, `OKHOST_EACH`, include paths, product `-D` flags.
2. Import smallest TU; forward one `*Default`.
3. Delete matching `StockAliases.h` defines **in the same batch**.

### Phase C — Assets and RAM hazards

1. ROM labels the host DMA (fonts, splash, piracy, segment blobs).
2. Replace every removed buffer (`ok_Storage`, hardcoded `0x8069…`) with host-owned BSS in the allowed window.
3. Convert all SMC / instruction pokes to native C or flags.

### Phase D — Hooks and menus

1. Forward remaining `*Default`s; wire menu controllers from decomp call sites.
2. Convert `BUILD.asm` sites using baserom disassembly + live map — not stock RAM guesses.

### Phase E — Custom course smoke

1. HotSwap TT → VS → GP → 2P GP.
2. Banner/preview restore.
3. On first load crash: progress meter before guessing.

### Phase F — Product extras

1. Personal-build flag, private IDO object, gated linker entry.
2. Both configurations must link.

---

## 6. Smoke gate table (copy into your host handoff)

| Gate | Pass criteria |
|---|---|
| G0 | `make AVOID_UB=1 COMPARE=0 -j` links; payload assert OK |
| G1 | Boot to host title / menu |
| G2 | Stock course race (TT) |
| G3 | Custom course TT (HotSwap > 0) |
| G4 | Custom VS |
| G5 | Custom GP (8 racers, no camera crash) |
| G6 | Custom 2P GP |
| G7 | Banner/preview swap restore |
| G8 | Personal build flag links (if any) |

---

## 7. Anti-patterns (do not do these)

- Reintroduce Armips / `.org` into the decomp tree “temporarily.”
- Put closed-source `.c` under `src/` even if gitignored.
- Copy `GameOffsets.asm` addresses into C or `undefined_syms.txt`.
- Assume cup table order == texture bank order == course table row.
- Loop 8 players into camera APIs without `IS_PLAYER`.
- Remap stock `0x0F` texture DMA the same way as Tarmac course textures.
- Leave load-path debug meters in the tree after the bug is fixed.
- Start Amped Up (or a second host) in the same session as the first host link.

---

## 8. What this OverKart5 integration does *not* prove

- That Amped Up or a third host will fit the same payload budget.
- That Tarmac’s `0x641F70` convention will last forever (tooling change would simplify loaders).
- That `ProtectMode` / flashcart detect is complete (stub left unwired on purpose).
- That matching / `COMPARE=1` is restored (explicitly not a goal).

---

## 9. One-page summary for the next agent

1. Plan batches and smoke gates first.  
2. Native IDO ELF only; host beside Library; `*Default` forwarders.  
3. Resolve every stock Library name to a **decomp symbol**.  
4. Course load bugs: trust the authoring tool’s address math; split stock vs custom texture DMA.  
5. GP bugs: cameras are for humans (`IS_PLAYER`).  
6. Diagnose with a progress meter, not shotgun ID forces.  
7. Private IDO sources stay out of tree; product flags must both build.  
8. Update the host handoff’s Done/Blocked list every batch.

When in doubt, prefer the behavior of the shipping ROM mod over a cleaner redesign — silent contract breaks cost more than ugly C.
