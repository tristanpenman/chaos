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
  Palette* m_palettes;
};
