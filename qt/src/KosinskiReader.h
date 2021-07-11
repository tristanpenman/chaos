#pragma once

#include <algorithm>
#include <iosfwd>

/**
 * Decompressor for RLE-compressed level data
 *
 * See doc/kosinski.txt for details of how the (de-)compression algorithm works
 */
class KosinskiReader
{
public:
  typedef std::pair<bool, size_t> Result;

  explicit KosinskiReader();

  Result decompress(std::istream&, uint8_t buffer[], size_t bufferSize);

private:
  // load the next bitfield from the input stream
  void loadBitfield(std::istream& file);

  // get one bit from the bitfield, and reload the next bitfield if we run out of bits
  uint8_t getBit(std::istream& file);

  // read one byte from the input stream, and throw an exception if we reach eof
  uint8_t readByte(std::istream& file);

  uint16_t m_bitfield;
  uint16_t m_bitcount;
};
