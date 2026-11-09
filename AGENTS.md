# OverKartDecomp — Agent Entry Point

This tree is the **OverKart5** host on the mk64 decomp with OverKart Library linked natively (one IDO ELF). It is a sibling of AmpedUpDecomp and LibraryDecomp. `src/OverKartLibrary/` must be identical across all three.

Read this file first. Then open only the living docs you need. The docs are a map, not a substitute for the code.

## This tree

- Host lives in `src/OverKart5/`. Defaults body: `src/OverKart5/OverKartDefaults.c`.
- Two products, one switch: `OVERKART_BUILD=0` is Tarmac Patch (community), `=1` is OverKart (personal). Both are targets. Drive the flag from the Makefile, not by hand-editing `Build.h`.
- Payload ROM uses the Library default at `0xC00000` (wrong for Amped Up; correct here).
- Faces grow forward from `0x80625800`; verts grow back from `0x80700000`; DLs/textures grow back from `0x80800000`.
- Vanilla C includes `OverKartHooks.h`, not `MainInclude.h`. Host TUs may include `MainInclude.h`.
- `Code/Protec/Cheat.c` is closed-source and lives outside every repo. Never copy it into the tree. Ship only the IDO-built object (`lib/private/Cheat.o`). Prefer `Cheat_ido.c` in Protec/ so the GCC host tree stays untouched. Community build must not require that object.
- `GameOffsets.asm` is stock NTSC RAM. A `GameOffsets.h` name must resolve to a decomp symbol (struct member, array, function, or the C statement at that text offset). Look up the stock address in `GameOffsets.asm`, then find it in KimuraDecomp (`symbols/Stock.map`, `proposed_renames_ram.csv`, `conflicts.csv`, or function+offset). Mid-function RAM is SMC, not BSS.
- `SaveData` (`OverKartStruct.h`) is the EEPROM layout. Do not reorder or resize it.
- Full RAM/ROM map: [docs/memory_map.md](docs/memory_map.md). Host contract: [docs/host_overrides.md](docs/host_overrides.md). Private object notes: [lib/private/README.md](lib/private/README.md).

## How to start a task

1. Read this file.
2. Open [docs/host_overrides.md](docs/host_overrides.md) and/or [docs/memory_map.md](docs/memory_map.md) if the work touches hosts, RAM, ROM, or `*Default`.
3. Open the source those docs point at.
4. Propose the exact files, what will change, and why. Wait for approval. After the edit, present the full diff for review.

## Operating rules

These override convenience. Do not weaken them because a task is small.

1. **Approval then review.** Explain files / what / why, wait, implement, present the diff. “Go ahead and implement” authorizes the edit; it does not waive review. One approved batch at a time.
2. **Work only in this repo.** Do not edit KimuraDecomp. Sibling `Library/` is historical GCC/Armips copy source only. Do not edit OverKart5/ to make this build work — import into `src/OverKart5/`.
3. **One Library.** Do not fork `src/OverKartLibrary/`. Host discrepancies live in `src/OverKart5/`, vanilla call sites, or the host Defaults body. A Library fix is made the same way in all three copies, or not at all. `OverKartDefaults.h` stays in Library.
4. **Prefer the original working path** (OverKart5, Library, or vanilla) unless it is wrong for this ABI or RAM map. Do not invent a parallel helper to paper over a bug.
5. **NTSC only** for custom courses. Tarmac `ok_HeaderROM` stays `0xBE9178`.
6. **IDO only.** `COMPARE=0`, `AVOID_UB=1`. One ELF. Do not reintroduce Armips, `BUILD.asm` as a patcher, BootCode/PreCode DMA, or whole-Library DMA.
7. Do not strip “unused” Library files. KartCheck / CameraCheck are host-owned (they arrive via `Cheat.o`).
8. Do not nest leftover stock shape (`USbit`, Shake `0x18`, Camera `velocity[3]`, Hud `tx[5]`) unless asked. Smoke: `.number` is the u16 kind; `.type` is the f32. Do not flatten `tire_FL` / `sus` / Smoke `trx` back to stock arrays.
9. Do not “fix” unmatched vanilla C or RSP unless asked. This is not a matching ROM.
10. End lengthy replies with a separator line (`---`), then one short paragraph summarizing what was asked, what was done, and what is still open.

## Contracts

- `sizeof(Player)` `0xDD8`, Object `0x70`, AnmObject `0xE0`. `sizeof(OKHeader)` stays `0xD0`.
- Payload (`.overkart`) at `0x80400000`. OKHeader BSS may overflow into `0x80500000`; must not reach FB0 at `0x805DA800`.
- Z at `0x80300000`. Color FBs at `0x805DA800` (FB0) / `0x80600000` (FB1) / `0x80700000` (FB2). **Do not relocate FB or Z.**
- `FreeMemory` is 768 KB (`0x801978D0`–`0x802578D0`).
- `ok_ObjectPool` is leftover after gfx/audio up to `0x80400000` (`LoadOKData`). Anchor to `ok_ObjectPoolStart`, never a literal.
- `ok_FreeSpace` is a temp pad for **non-course** compressed data. Do not park course pools there.
- `ok_Storage` / `ok_Knowledge` / `SwopCheck` are gone. Do not revive them. Host menu storage is host BSS in the bank-5 window after `OKHeaderEND`.
- Keep `OKHeader.h` and any ASM mirror in lockstep.

`GameOffsets.asm` is stock NTSC RAM (`0x80xxxxxx`), not this ELF’s layout. Never copy a `.definelabel` into C, never add one to `undefined_syms.txt`, never poke a stock RAM literal as if it were live.

A historical Armips `JAL HostFn` overlay means “host runs here,” not “store host `v0` into the stock field.” If stock assigned that return, keep the stock call for the assignment and run the host as extra work.

Cameras exist for humans (`flag & IS_PLAYER`), not `playerID <= 3`. Do not let CPU slots poke `GlobalCamera`.

Sine/cosine are one `AVOID_UB` table with a 90° offset. Sky `Top` and `Bot` are two independent arrays. Do not overlay either the wrong way.

Expansion pak required. If `osMemSize < 0x800000`, show the warning and hang. Never poke FB1 as a probe.

A successful link is not a successful race. Smoke both `OVERKART_BUILD=0` and `=1` when the change can affect either product.

## Naming

`PascalCase`. No underscores, no leading `g`, acronyms stay capitalised (`KWViewportCall`). Pipeline lives in LibraryDecomp `tools/naming/`. A rename must not change ROM bytes. Never rename `src/os`, `include/PR`, `src/OverKart5/data/ModelData/`, or the 108 symbols `Cheat.o` links against. Apply a tier to all three repos before the next tier.

## Build

```
make AVOID_UB=1 COMPARE=0 -j
make AVOID_UB=1 COMPARE=0 OVERKART_BUILD=1 -j
```

Windows: toolchain at `tools/mingw64`, then `make assets && make -j`. Need `baserom.us.z64` in the repo root. Rebuild after `.inc.c` edits by saving the parent `.c`. Paste IDO/link errors verbatim.
