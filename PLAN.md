# Plan

- Tidy up original MSVC version
  + ~~code style~~
  + ~~warnings~~
  + remove unnecessary classes/interfaces
  + cleaner separation of core and WinAPI classes
  + normalise variable naming
  + cleaner use of exceptions
  + ROM details in Level Select (in progress)
  + tidy up Sonic 3 loader code
- Port simple map viewer interface to Mac and Linux using Qt
  + ~~Palette inspector~~
  + ~~Pattern inspector~~
  + ~~Chunk inspector~~
  + ~~Block inspector~~
  + ~~Read-only map editor~~
  + ROM details in Level Select (in progress)
  + ~~Read Sonic 3 levels~~
  + Move module reading code to SonicReader/Kosinski
  + Implement nemesis compression
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
- Sonic and Knuckles support
- Sonic 3 and Knuckles support
- Sonic 1 support
- Sonic Mania support
- Pattern editing
- Live preview for pattern and chunk editing
- Audio extraction (maybe)
