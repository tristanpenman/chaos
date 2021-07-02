# Plan

- Tidy up original MSVC version
  + ~~code style~~
  + ~~warnings~~
  + ~~remove ROM details from Level Select~~
  + cleaner separation of core and WinAPI classes
  + normalise variable naming
  + tidy up exception handling
  + tidy up Sonic 3 loader code
  + rename SonicReader to Kosinski
- Port simple map viewer interface to Mac and Linux using Qt
  + ~~Palette inspector~~
  + ~~Pattern inspector~~
  + ~~Chunk inspector~~
  + ~~Block inspector~~
  + ~~Read-only map editor~~
  + ~~Read Sonic 3 levels~~
  + rename SonicReader to Kosinski
- Level modification support
  + Block selector (in progress)
  + RLE compression implementation
  + resize Sonic 2 ROM
  + resize Sonic 3 ROM
  + pencil
  + selection rectangle
  + copy and paste
- General improvements
  + Figure out how to read level names and locations from ROM files
  + Support for loading levels from decompilations
- Sprite preview support
  + Implement nemesis de-/compression
- Sonic and Knuckles support
- Sonic 3 and Knuckles support
- Sonic 1 support
- Sonic Mania support
- Pattern editing
- Live preview for pattern and chunk editing
- Audio extraction (maybe)
