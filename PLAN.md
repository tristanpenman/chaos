# Plan

- Tidy up original MSVC version
  + ~~code style~~
  + ~~warnings~~
  + ~~remove ROM details from Level Select~~
  + ~~Normalise variable naming~~
  + ~~Rename SonicReader to Kosinski~~
  + Cleaner separation of core and WinAPI classes
  + Tidy up exception handling
  + Tidy up Sonic 3 loader code
- ~~Port simple map viewer interface to Mac and Linux using Qt~~
  + ~~Palette inspector~~
  + ~~Pattern inspector~~
  + ~~Chunk inspector~~
  + ~~Block inspector~~
  + ~~Read-only map editor~~
  + ~~Read Sonic 3 levels~~
  + ~~zoom in/out using mouse wheel~~
  + ~~rename SonicReader to Kosinski~~
- Level modification support
  + ~~Block selector~~
  + ~~Pencil mode~~
  + ~~Relocate levels in Sonic 2 ROM~~
  * ~~Undo/redo~~
  + ~~RLE compression implementation~~
  + ~~Save level~~
  + Toolbar
  + Relocate levels in Sonic 3 ROM
  + Selection rectangle, replace all tiles
  + Copy and paste
- General improvements
  + ~~Logging interface~~
  + ~~Restructure project so that Qt port is at root level~~
  * Keyboard shortcuts
  + Zoom in/out menu items
  + Figure out how to read level names and locations from ROM files
  + Support for loading levels from decompilations
- Sprite preview support
  + Implement nemesis de-/compression
- New functinality
  + Sonic 2 disassembly support (in progress)
  + Sonic and Knuckles disassembly support
  + Sonic 3 and Knuckles disassembly support
  + Sonic 1 disassembly support
  + Sonic Mania support
  + Pattern editing
  + Live preview for pattern and chunk editing
  + Audio extraction (maybe)
