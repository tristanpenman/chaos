#pragma once

#include <cstdint>

/**
 * SEGA Pattern implementation
 *
 * Provides a means by which to read/write SEGA palette data. This should be portable across various SEGA Mega Drive
 * and Genesis titles, since it is tied closely to the palette implementation in hardware.
 *
 * Patterns are 8x8 pixels in size. In a ROM, two pixels are stored per byte (each pixel is one nibble). The
 * pixel value is a palette column index. Fifteen colors may be referenced, with zero used to mark transparent
 * pixels.
 *
 * Once loaded into memory, pixels are stored 1-per-byte (i.e. as a uint8_t index into a palette).
 */
class Pattern
{
public:
  static constexpr uint8_t PATTERN_WIDTH = 8;
  static constexpr uint8_t PATTERN_HEIGHT = 8;
  static constexpr uint8_t PIXELS_PER_BYTE = 2;
  static constexpr uint8_t PATTERN_SIZE_IN_MEM = PATTERN_WIDTH * PATTERN_HEIGHT;
  static constexpr uint8_t PATTERN_SIZE_IN_ROM = PATTERN_SIZE_IN_MEM / PIXELS_PER_BYTE;

  Pattern();

  void fromSegaFormat(uint8_t bytes[PATTERN_SIZE_IN_ROM]);

  uint8_t getPixel(uint8_t x, uint8_t y) const;
  void setPixel(uint8_t x, uint8_t y, uint8_t value);

private:
  Pattern(const Pattern&) = delete;
  Pattern& operator=(const Pattern&) = delete;

  uint8_t m_pixels[PATTERN_SIZE_IN_MEM];
};
