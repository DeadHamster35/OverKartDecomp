# OverKart V8 
This decompilation of Mario Kart 64 integrates the OverKart V8 / Tarmac Community projects. This includes support for Tarmac64 Custom Courses. 

This repository does not contain assets. Compiling requires asset extraction from a prior copy of the game.


## Quick Start

```
git submodule update --init --recursive
OR
git clone https://github.com/n64decomp/mk64 --recurse-submodules
```
Extract assets
```
make -C tools -j
make assets -j
make -j
```

## Building

Build instructions located in the [API docs](https://n64decomp.github.io/mk64/compiling.html)  
See also, the comprehensive API and documentation which includes an overview of the code-base while also discussing significant concepts, features, and modding.

## Current State

The code-base may change significanty overtime as naming and documentation continues.
Adjustments to the game require compiling with AVOID_UB=1 to enable shiftability. Use DEBUG=1 to enable the games debug mode.

Some menu textures are compressed using a format called tkmk00. A byte-matching compressor/decompressor does not yet exist.   

## Project Structure
	
	mk64
	├── asm: Handwritten assembly code, rom header and boot
	│   ├── non_matchings: Assembly for non-matching sections
	│   └── os: Libultra handwritten assembly code
	├── assets: Textures
	├── courses: Course data, geography, display lists and staff ghosts
	├── build: Output directory
	├── data: Misc data, text, audio banks, and instrument sets
	├── docs: Build guides
	├── include: Header files
	├── music: Sequences
	├── src: C source code for the game
	|   ├── actors: Individual actors split out from other files
	│   ├── audio: Sample tables and audio code
	│   ├── data: Misc data referenced in other C files
	|   ├── debug: Custom debug code
	|   ├── ending: Podium ceremony and credits code
	│   ├── os: Libultra C code
	|   └── racing: Race and game engine code
	|   ├── OverKartLibrary: Shared library of custom code
	|   ├── OverKartV5: OverKart5 / Tarmac Host Project/ 
	├── textures: Texture data, bitmaps
	|   ├── common: Textures common to many courses
	|   ├── courses: Course specific textures
	|   ├── crash screen: Crash screen font image
	│   ├── raw: Raw textures
    │   ├── standalone: Whole textures
	|   ├── startup_logo: Reflection map
	|   └── trophy: Ceremony cutscene podium and trophy textures
	└── tools: build tools


## Contributing

Pull requests are welcome. For major changes, please discuss in the Discord.

Run `make format` to ensure it meets the project's coding standards.
If code needs to avoid formatting place `// clang-format off` before the code and `// clang-format on` after.

