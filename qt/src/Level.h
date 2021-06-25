#pragma once

#include <cstddef>

class Palette;
class Pattern;

class Level
{
public:
  virtual ~Level() {}

  virtual size_t getPaletteCount() const = 0;
  virtual const Palette& getPalette(size_t index) const = 0;

  virtual size_t getPatternCount() const = 0;
  virtual const Pattern& getPattern(size_t index) const = 0;
};
