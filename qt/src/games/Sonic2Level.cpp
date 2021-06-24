#include "../Palette.h"
#include "../Rom.h"

#include "Sonic2.h"
#include "Sonic2Level.h"

Sonic2Level::Sonic2Level(Rom& rom,
                         uint32_t palettesAddr,
                         uint32_t)
{
  m_palettes = new Palette[4];

  auto buffer = rom.readBytes(palettesAddr, Palette::PALETTE_SIZE_IN_ROM * 4);

  for (int i = 0; i < 4; i++) {
    m_palettes[i].fromSegaFormat(&buffer[Palette::PALETTE_SIZE_IN_ROM * i]);
  }
}
