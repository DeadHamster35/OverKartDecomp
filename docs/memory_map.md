# OverKart RDRAM Memory Map

Reference for agents integrating a **host** project on LibraryDecomp (OverKart5, Amped Up, new hosts). NTSC (USA) only. Do not invent new banks without asking.

Host override mechanics (which `*Default` to replace, build flags): **`docs/host_overrides.md`**.  
OverKart5 link steps: **`docs/overkart5_host_handoff.md`**.

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

## Ranges and ownership

| Range | What | Used for |
|---|---|---|
| `0x80000400` | Main ELF code/data | Stock + linked Library/host text/rodata that is not in `.overkart`. |
| `0x801978D0`–`0x802578D0` | **FreeMemory** (768 KB) | Boot: segment 2 + `0xD`. After `StaticMemoryPointer`: course seg 6/9, collision table, actors, IceKage. Grows forward (`FreeMemoryPointer`). |
| `0x80280000` | Ending | Credits / ending segment. |
| `0x8028DF00` | Racing + assets | Race code, trig, kart textures/palettes. |
| `0x80300000` | **gZBuffer** (`0x25800`) | Depth buffer. |
| after Z – `0x80400000` | GFX + audio, then **ok_ObjectPool** | Leftover after gfx/audio up to payload start. `LoadOKData` / custom OK objects (`ok_ObjectPoolStart`..`ok_ObjectPoolEnd`). |
| `0x80400000` into `0x80500000` | **`.overkart` + OKHeader BSS** | Library (+ host) payload DMA’d at boot (`init_overkart`). OKHeader BSS may overflow into bank 5. **Must not reach FB0 at `0x805DA800`.** |
| `0x80500000`–`0x805DA800` | Host / payload overflow | Front of bank 5 (~872 KB) if `.overkart` + BSS need room. |
| `0x805DA800` | **FB0** (`0x25800`) | Color framebuffer 0 (flush against FB1). |
| `0x80600000` | **FB1** | Color framebuffer 1. |
| `0x80625800`–`0x80700000` | Faces + verts | **`gFaceBuffer`** grows **forward**; decoded course vertices (seg 4) grow **back** from `0x80700000`. |
| `0x80700000` | **FB2** | Color framebuffer 2. |
| `0x80725800`–`0x80800000` | DLs / map textures | F3DEX + map textures; `LastMemoryPointer` grows **back** from `0x80800000`. |

## OverKart-owned pads (inside OKHeader BSS / payload)

| Symbol / label | Role |
|---|---|
| **`ok_FreeSpace`** | Temp scratch (`0x20000` after `ok_TKMSpace`) for **non-course** compressed data (fonts, HUD buttons, banners, ghosts). Do **not** put course pools here. |
| **`ok_ObjectPool`** | Custom object model DMA/decode destination (`LoadOKData`). Separate from FreeMemory and from course vtx/DL pools. |
| **`ok_HeaderROM`** | `0xBE9178` — Tarmac custom-course header table in ROM. **Frozen.** Do not move. |

Keep `OKHeader.h` and any ASM mirror in lockstep if you add destination labels.

## Course load (pointer directions)

`load_course` / `MapStartupDefault` roughly:

1. DMA geography (seg `0xF`) onto FreeMemory as throwaway.
2. Vertices (seg 4) from `VertexMemoryPointer` (back from `0x80700000`).
3. Packed DLs (seg 7) from `LastMemoryPointer` (back from `0x80800000`).
4. Rewind FreeMemory; eat-own-tail seg 6 → offsets (9) → map textures (5).
5. `MakeCollision` leaves faces in `gFaceBuffer`; u16 `collision_table` stays on FreeMemory.

`MapStartupDefault` resets course pointers. Hosts that load extra blobs wrap that path; they do not invent a second course heap.

## Hard limits (high-water)

`CheckCourseMemoryHighWater` hangs if any of these fail:

- `FreeMemoryPointer` `>` `0x802578D0`
- `VertexMemoryPointer` `<` `0x80625800`
- `LastMemoryPointer` `<` `0x80725800` (ending/credits: `<` pool end)
- Face end `>=` `0x80700000` or overlapping Vtx
- `ObjectMemoryPointer` `>=` `0x80400000`

## Agent rules when adding a host

- Prefer Library/host text in the main ELF; payload stays at `0x80400000`. If 4MB fills, overflow into expansion RAM per project policy — do not steal FB or course pools.
- Measure remaining room as `0x805DA800 - OKHeaderEND` (or payload end) after a real link; do not assume bank 5 is empty.
- Do not relocate Z, FBs, `gFaceBuffer`, or `ok_HeaderROM` for convenience.
- `GameOffsets.asm` labels are **stock** NTSC RAM from a matching build — not live addresses under `COMPARE=0` / `AVOID_UB=1`. Never copy those literals into C as if they were this ELF’s layout.
- Sibling historical `Library/` is copy source only; live tree is `src/OverKartLibrary/`.
