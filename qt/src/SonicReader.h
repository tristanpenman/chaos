#pragma once

#include <algorithm>
#include <iosfwd>

/**
 * Decompressor for RLE-compressed level data
 *
 * See doc/kosinski.txt for details of how the (de-)compression algorithm works
 */
class SonicReader
{
public:
  typedef std::pair<bool, size_t> Result;

  explicit SonicReader(std::fstream& rom);

  Result decompress(uint8_t buffer[], size_t bufferSize);

private:
  void loadBitfield();
  uint8_t getBit();

  uint16_t m_bitfield;
  uint16_t m_bitcount;

  std::fstream& m_file;
};
