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

  explicit SonicReader(std::istream& rom);

  Result decompress(unsigned char buffer[], size_t bufferSize, std::streamoff readFrom);

private:
  void loadBitfield();
  unsigned char getBit();

  unsigned int m_bitfield;
  unsigned char m_bitcount;

  std::istream& m_rom;
};
