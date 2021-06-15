module SegaDecompressor
  ( main ) where

import NiceOpt

import qualified Data.Map as Map
import System.Exit

import Data.Bits
import Data.Char
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

data Flag = Help | Offset String deriving Show

-- Define the help/usage information for the program
usage argv0 = unlines [
    "",
    "Usage:",
    "  " ++ argv0 ++ " <ROM file> [options]",
    "",
    "Other options:",
    "  -h",
    "  -o <hex>    Hex offset at which to begin decompression",
    "",
    "Example:",
    "  " ++ argv0 ++ " s2.bin -o 45AC4",
    ""
    ]

-- Define the options that will be accepted by the program
options :: [OptDescr Flag]
options = [
    Option "h" omitted (NoArg Help) omitted,
    Option "o" omitted (ReqArg Offset omitted) omitted ]
    where omitted = []

-- Check whether or not a string is a valid hexadecimal value
isHex = isJust . matchRegex (mkRegex "[a-fA-F0-9]")

-- Parse the command line options, validate any arguments provided, and return
-- a list of errors that were detected
parseOptions :: [String] -> ([Flag],[String])
parseOptions argv =
    case getOpt Permute options argv of
        (options, _, errors) -> (options, validate options errors)
    where validate :: [Flag] -> [String] -> [String]
          validate [] errors = errors
          validate (Offset x:xs) errors = if isHex x then errors else
            errors ++ ["could not parse offset '" ++ x ++ "'"]
          validate (x:xs) errors = validate xs errors

-- Return true if the -h flag is present in a list of flags
shouldShowHelp :: [Flag] -> Bool
shouldShowHelp [] = False
shouldShowHelp (Help:xs) = True
shouldShowHelp (_:xs) = shouldShowHelp xs

getOffset :: [Flag] -> String
getOffset [] = "0"
getOffset (Offset x:xs) = x
getOffset (x:xs) = getOffset xs

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
        let ((offset,_):_) = readHex (getOffset flags)
        withBinaryFile (head argv) ReadMode (\handle -> do
            input <- BL.hGetContents handle
            putStr $ show (take 384 (decompress [] NoControlBits (BL.drop offset input)))
            return ())

