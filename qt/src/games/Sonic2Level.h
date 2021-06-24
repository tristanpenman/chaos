#pragma once

#include <memory>

#include "../Level.h"

class Palette;
class Pattern;
class Rom;

class Sonic2Level: public Level
{
public:
  Sonic2Level(Rom& rom,
              uint32_t palettesAddr,
              uint32_t patternsAddr);

private:
  void loadPalettes(Rom& rom, uint32_t palettesAddr);
  void loadPatterns(Rom& rom, uint32_t patternsAddr);

  Palette* m_palettes;
  Pattern* m_patterns;

  size_t m_numPatterns;
};
