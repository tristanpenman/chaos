#pragma once

#include <cstdint>

#include "PatternDesc.h"

/**
 * Representation of a 16x16 tile, composed of 4 8x8 SEGA patterns
 *
 * Pattern are defined using a common descriptor for SEGA patterns, which specifies properties such as how
 * the pattern is flipped. See PatternDesc.h for more information.
 */
class Chunk
{
public:
  static constexpr uint8_t CHUNK_HEIGHT = 16;
  static constexpr uint8_t CHUNK_WIDTH = 16;
  static constexpr uint8_t PATTERNS_PER_CHUNK = 4;
  static constexpr uint8_t BYTES_PER_PATTERN = 2;
  static constexpr uint8_t CHUNK_SIZE_IN_ROM = PATTERNS_PER_CHUNK * BYTES_PER_PATTERN;

  Chunk();

  void fromSegaFormat(uint8_t buffer[CHUNK_SIZE_IN_ROM]);

  const PatternDesc& getPatternDesc(uint8_t x, uint8_t y) const;

private:
  Chunk(const Chunk&) = delete;
  Chunk& operator=(const Chunk&) = delete;

  PatternDesc m_patternDescs[PATTERNS_PER_CHUNK];
};
