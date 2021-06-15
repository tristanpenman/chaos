module S2Data
  ( readAddress
  , readByte
  , readPatternPtr
  , loadPalettes
  , readPaletteData
  , getLevelDataTableIndexAddress
  , getLevelDataEntryAddress
  , getPalettePtrAddress
  , patternPtrOffset
  , palettePtrTableIndexOffset
  ) where

import qualified Data.ByteString.Lazy as BL
import Data.Word
import Data.Char
import Data.Bits
import Data.List.Split

readAddress :: BL.ByteString -> Word -> Word
readAddress input position =
    shiftL (fromIntegral (head bytes)) 24 .|.
    shiftL (fromIntegral (head (tail bytes))) 16 .|.
    shiftL (fromIntegral (head (tail (tail bytes)))) 8 .|.
    fromIntegral (head (tail (tail (tail bytes))))
    where readPosition = fromIntegral position
          bytes = BL.unpack $ BL.take 4 $ BL.drop readPosition input

readByte :: BL.ByteString -> Word -> Word
readByte input position =
    let readPosition = fromIntegral position
    in fromIntegral $ head $ BL.unpack $ BL.take 1 $ BL.drop readPosition input

-- Read a Pattern Pointer from the given position
readPatternPtr :: BL.ByteString -> Word -> Word
readPatternPtr input position =
    (readAddress input position) .&. 0xFFFFFF

loadPalettes :: BL.ByteString -> Word -> [[[Word]]]
loadPalettes input position = map (readPaletteData input position) [0..3]

-- Given an index into a palette data block, read the data for a single palette.
-- Palettes consist of 16 colors, with each color being 2 bytes in length.
readPaletteData :: BL.ByteString -> Word -> Word -> [[Word]]
readPaletteData input position paletteIndex =
    let readPosition = fromIntegral $ position + fromIntegral bytesPerPalette * paletteIndex
    in chunksOf 2 $ map fromIntegral $ BL.unpack $ BL.take bytesPerPalette $ BL.drop readPosition input
    where numColors = 16
          bytesPerColor = 2
          bytesPerPalette = numColors * bytesPerColor

-- Given an index into the Level Table, calculate the address at which an index
-- into the Level Data Table can found.
getLevelDataTableIndexAddress :: Word -> Word
getLevelDataTableIndexAddress levelTableIndex =
    levelTableEntrySize * levelTableIndex + levelTableAddress
    where levelTableAddress = 0x9454
          levelTableEntrySize = 2 -- bytes

-- Given an index into the Level Data Table, calculate the address at which the
-- entry can be found.
getLevelDataEntryAddress :: Word -> Word
getLevelDataEntryAddress levelDataTableIndex =
    levelDataTableAddress + levelDataTableIndex * levelDataTableEntrySize
    where levelDataTableAddress = 0x42594
          levelDataTableEntrySize = 12 -- bytes

-- Given an index into the Palette Pointer Table, calculate the address at which
-- the palette pointer can be found. A Palette Pointer points to an array of
-- four palettes that are associated with a level.
getPalettePtrAddress :: Word -> Word
getPalettePtrAddress palettePtrTableIndex =
    palettePtrTable + palettePtrTableIndex * palettePtrTableEntrySize
    where palettePtrTable = 0x2782
          palettePtrTableEntrySize = 8 -- bytes

-- Offset at which a pointer to pattern data can be found in a given Level
-- Data Table entry.
patternPtrOffset :: Word
patternPtrOffset = 0

-- Offset at which an index into the Palette Pointer Table can be found in a
-- Level Data Table entry.
palettePtrTableIndexOffset :: Word
palettePtrTableIndexOffset = 8