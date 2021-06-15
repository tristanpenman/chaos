module RLE
  ( decompress
  , ControlBits(NoControlBits, ControlBits)
  , loadControlBits
  , getBit
  ) where

import Data.Bits
import Data.Word

import qualified Data.ByteString.Lazy as BL

type Bit = Integer
type Bitfield = Int
type Bitcount = Integer

data ControlBits = NoControlBits | ControlBits Bitcount Bitfield deriving Show

loadControlBits :: BL.ByteString -> (ControlBits, BL.ByteString)
loadControlBits input = (controlBits, remaining)
    where bytes = BL.unpack (BL.take 2 input)
          bitfield = fromIntegral (head bytes) .|. shiftL (fromIntegral (head (tail bytes))) 8
          controlBits = ControlBits 16 bitfield
          remaining = BL.drop 2 input

-- Takes a set of control bits, which consists of a bitfield and a bitcount,
-- and extracts the value of the next bit in the bitfield. If extracting the
-- bit exhausts the bitfield, then the next 16 bits will be read from the
-- provided ByteString.
getBit :: ControlBits -> BL.ByteString -> (Bit, ControlBits, BL.ByteString)
getBit (ControlBits 1 bitfield) input = (bit, controlBits, remaining)
    where bit = fromIntegral $ bitfield .&. 1
          (controlBits, remaining) = loadControlBits input
getBit (ControlBits bitcount bitfield) input = (bit, ControlBits newBitcount newBitfield, input)
    where bit = fromIntegral $ bitfield .&. 1
          newBitfield = shiftR bitfield 1
          newBitcount = bitcount - 1

nextTwoBitDescriptor controlBits input = (value, sndControlBits, sndRemaining)
    where (fstBit, fstControlBits, fstRemaining) = getBit controlBits input
          (sndBit, sndControlBits, sndRemaining) = getBit fstControlBits fstRemaining
          value = shiftL (fromIntegral fstBit) 1 + fromIntegral sndBit

-- Takes a list, an index, and a number of elements to copy, and creates a new
-- list containing the copied elements. The index can be negative,
-- in which case it will be treated as relative to the end of the list.
-- TODO: Consider rewriting using take/drop
partOfList :: (Show a) => [a] -> Int -> Int -> [a]
partOfList buffer index count
    | count == 0 = buffer
    | index >= 0 = take count $ drop index buffer
    | otherwise =
        let pattern = drop (length buffer + index) buffer
        in take count $ cycle pattern

decompressInline :: [Int] -> ControlBits -> BL.ByteString -> ([Int], ControlBits, BL.ByteString)
decompressInline buffer controlBits input =
    (partOfList buffer offset (count + 1), newControlBits, BL.drop 1 remainingInput)
    where (count, newControlBits, remainingInput) = nextTwoBitDescriptor controlBits input
          offsetByte = head (BL.unpack (BL.take 1 remainingInput))
          offsetRaw = fromIntegral offsetByte
          offsetComplement = complement offsetRaw .&. 0xFF
          offset = negate offsetComplement - 1

fromTwosComplement :: (Num a, Bits a) => a -> a -> a
fromTwosComplement raw mask = negate (complement raw .&. mask) - 1

nextByteAsInt :: BL.ByteString -> Int
nextByteAsInt input = fromIntegral $ head $ BL.unpack $ BL.take 1 input

decompressRegular :: [Int] -> ControlBits -> BL.ByteString -> ([Int], ControlBits, BL.ByteString)
decompressRegular buffer controlBits input
    | count == 0 =
        let actualCount = fromIntegral $ head $ BL.unpack $ BL.take 1 remainingInput
        in  if actualCount == 0
            then ([], controlBits, BL.drop 1 remainingInput)
            else (partOfList buffer offset (actualCount + 1), controlBits, BL.drop 1 remainingInput)
    | otherwise  =
        (partOfList buffer offset (count + 2), controlBits, remainingInput)
    where lo = nextByteAsInt input
          hi = nextByteAsInt $ BL.drop 1 input
          remainingInput = BL.drop 2 input
          rawOffset = (shiftL hi 5 .&. 0x1F00) .|. (lo .&. 0xFF)   -- 13-bit two's complement representation
          offset = fromTwosComplement rawOffset 0x1FFF
          count = hi .&. 7

decompressRLE :: [Int] -> ControlBits -> BL.ByteString -> ([Int], ControlBits, BL.ByteString)
decompressRLE buffer controlBits input
    | bit == 0  = decompressInline buffer remainingControlBits remainingInput
    | otherwise = decompressRegular buffer remainingControlBits remainingInput
    where (bit, remainingControlBits, remainingInput) = getBit controlBits input

decompress :: [Int] -> ControlBits -> BL.ByteString -> [Int]
decompress buffer NoControlBits input =
    decompress buffer controlBits remaining
    where (controlBits, remaining) = loadControlBits input
decompress buffer controlBits input
    | bit == 0  = let (prefix, remainingControlBits2, remainingInput2) = decompressRLE buffer remainingControlBits remainingInput
                  in  prefix ++ decompress (buffer ++ prefix) remainingControlBits2 remainingInput2
    | otherwise = let prefix = fromIntegral $ head $ BL.unpack $ BL.take 1 remainingInput
                  in  prefix : decompress (buffer ++ [prefix]) remainingControlBits (BL.drop 1 remainingInput)
    where (bit, remainingControlBits, remainingInput) = getBit controlBits input
