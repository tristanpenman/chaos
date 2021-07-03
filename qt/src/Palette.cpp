#include <cstring>
#include <fstream>
#include <stdexcept>

#include "Palette.h"

using namespace std;

Palette::Palette()
{
  memset(m_colors, 0, sizeof(Color) * PALETTE_SIZE);
}

void Palette::fromSegaFormat(char bytes[PALETTE_SIZE_IN_ROM])
{
  for (int index = 0; index < PALETTE_SIZE; index++) {
    m_colors[index].fromSegaFormat(&bytes[index * BYTES_PER_COLOR]);
  }
}

const Palette::Color& Palette::getColor(size_t index) const
{
  if (index >= PALETTE_SIZE)
  {
    throw runtime_error("Invalid palette index");
  }

  return m_colors[index];
}

void Palette::setColor(size_t index, Color& color)
{
  if (index >= PALETTE_SIZE)
  {
    throw runtime_error("Invalid palette index");
  }

  memcpy(&m_colors[index], &color, sizeof(Color));
}

void Palette::Color::fromSegaFormat(char bytes[BYTES_PER_COLOR])
{
  r = static_cast<uint8_t>((bytes[1] & 0x0F) * 0x10);
  g = static_cast<uint8_t>(bytes[1] & 0xF0);
  b = static_cast<uint8_t>(bytes[0] * 0x10);
}
