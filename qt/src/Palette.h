#pragma once

#include <cstdint>
#include <iosfwd>
#include <stdexcept>

/**
 * SEGA Palette implementation
 *
 * Provides a means by which to read/write SEGA palette data. This should be portable across various SEGA Mega Drive
 * and Genesis titles, since it is tied closely to the palette implementation in hardware.
 *
 * Once loaded, colors are stored in memory in RGB format, suitable for display and manipulation on a PC.
 *
 * TODO: Document palette format here
 */
class Palette
{
  static constexpr uint8_t BYTES_PER_COLOR = 2;
  static constexpr uint8_t PALETTE_SIZE = 16;
  static constexpr uint8_t PALETTE_SIZE_IN_ROM = BYTES_PER_COLOR * PALETTE_SIZE;

public:
  struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    void fromSegaFormat(char bytes[BYTES_PER_COLOR]);
  };

  Palette();

  void fromSegaFormat(char bytes[PALETTE_SIZE_IN_ROM]);

  Color getColor(uint8_t index) const;
  void setColor(uint8_t index, Color& color);

private:
  Palette(const Palette&) = delete;
  Palette& operator=(const Palette&) = delete;

  Color m_colors[PALETTE_SIZE];
};
