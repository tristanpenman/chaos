#pragma once

#include <memory>

#include "../Level.h"

class Palette;
class Pattern;
class Rom;

class Sonic2Level : public Level
{
  static constexpr size_t PALETTE_COUNT = 4;

public:
  Sonic2Level(Rom& rom,
              uint32_t palettesAddr,
              uint32_t patternsAddr);

  virtual size_t getPaletteCount() const;
  virtual const Palette& getPalette(size_t index) const;

  virtual size_t getPatternCount() const;
  virtual const Pattern& getPattern(size_t index) const;

private:
  Sonic2Level(const Sonic2Level&) = delete;
  Sonic2Level& operator=(const Sonic2Level&) = delete;

  void loadPalettes(Rom& rom, uint32_t palettesAddr);
  void loadPatterns(Rom& rom, uint32_t patternsAddr);

  Palette* m_palettes;
  Pattern* m_patterns;

  size_t m_numPatterns;
};

inline size_t Sonic2Level::getPaletteCount() const
{
  return Sonic2Level::PALETTE_COUNT;
}

inline size_t Sonic2Level::getPatternCount() const
{
  return m_numPatterns;
}
