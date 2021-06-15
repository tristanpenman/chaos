module NiceOpt
  ( formatOptError ) where

import Text.Regex

-- Format getopt errors to be more readable, and ensure that error messages
-- end with a newline character
formatOptError input = ensureNewline $ "Error: " ++ subRegex (mkRegex pattern) input replacement
    where pattern = "option `-([a-zA-Z0-9])' requires an argument.*"
          replacement = "option '-\\1' missing required argument"
          ensureNewline n = subRegex (mkRegexWithOpts "[^\n]$" True False) n "\\0\n"
