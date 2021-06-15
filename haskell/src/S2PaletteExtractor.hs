module S2PaletteExtractor
  ( main ) where

import NiceOpt
import S2Data

import qualified Data.Map as Map
import System.Exit
import Data.Bits
import Data.Char
import Data.List.Split
import Data.Maybe
import System.IO
import System.Console.GetOpt
import System.Environment
import Text.Regex
import Data.Word
import Control.Monad
import Numeric
import Debug.Trace

import qualified Data.ByteString.Lazy as BL

import RLE

data Flag = Help | Output String | Level String deriving Show

-- Define the help/usage information for the program
usage argv0 = unlines [
    "",
    "Usage:",
    "  " ++ argv0 ++ " <ROM file> [options]",
    "",
    "Other options:",
    "  -h",
    "  -l <level>   Level to extract palettes from",
    "  -o <output>  File name for PNG format output",
    "",
    "Example:",
    "  " ++ argv0 ++ " s2.bin -l 2 -o palette.png",
    ""
    ]

-- Define the options that will be accepted by the program
options :: [OptDescr Flag]
options = [
    Option "h" omitted (NoArg Help) omitted,
    Option "l" omitted (ReqArg Level omitted) omitted,
    Option "o" omitted (ReqArg Output omitted) omitted]
    where omitted = []

isEmpty :: String -> Bool
isEmpty [] = True
isEmpty _ = False

-- Check whether or not a string is a valid integer value
isInteger s = case reads s :: [(Integer, String)] of
  [(_, "")] -> True
  _         -> False

-- Parse the command line options, validate any arguments provided, and return
-- a list of errors that were detected
parseOptions :: [String] -> ([Flag],[String])
parseOptions argv =
    case getOpt Permute options argv of
        (options, _, errors) -> (options, validate options errors)
    where validate :: [Flag] -> [String] -> [String]
          validate [] errors = errors
          validate (Level x:xs) errors = if isInteger x then errors else
            errors ++ ["could not parse level index '" ++ x ++ "'"]
          validate (Output x:xs) errors = if not (isEmpty x) then errors else
            errors ++ ["invalid output file '" ++ x ++ "'"]
          validate (x:xs) errors = validate xs errors

-- Return true if the -h flag is present in a list of flags
shouldShowHelp :: [Flag] -> Bool
shouldShowHelp [] = False
shouldShowHelp (Help:xs) = True
shouldShowHelp (_:xs) = shouldShowHelp xs

getLevel :: [Flag] -> Word
getLevel [] = 0
getLevel (Level x:xs) = read x
getLevel (x:xs) = getLevel xs

getOutput :: [Flag] -> String
getOutput [] = "0"
getOutput (Output x:xs) = x
getOutput (x:xs) = getOutput xs

main = do
    argv0 <- getProgName
    argv <- getArgs
    when (null argv) $
      do hPutStr stderr (usage argv0)
         exitFailure
    let (flags, errors) = parseOptions (tail argv) in do
        when (shouldShowHelp flags) $
            do  hPutStr stderr (usage argv0)
                exitSuccess
        unless (null errors) $
            do  hPutStr stderr $ concatMap formatOptError errors ++ usage argv0
                exitFailure
        let levelIndex = getLevel flags in
            withBinaryFile (head argv) ReadMode (\handle -> do
                input <- BL.hGetContents handle
                let levelDataTableIndexAddress = getLevelDataTableIndexAddress levelIndex
                let levelDataTableIndex = readByte input levelDataTableIndexAddress
                let palettePtrTableIndexAddress = getLevelDataEntryAddress levelDataTableIndex +
                        palettePtrTableIndexOffset
                let palettePtrTableIndex = readByte input palettePtrTableIndexAddress
                let palettePtrAddress = getPalettePtrAddress palettePtrTableIndex
                let palettePtr = readAddress input palettePtrAddress
                let paletteData = loadPalettes input palettePtr
                putStr $ show paletteData
                return ())