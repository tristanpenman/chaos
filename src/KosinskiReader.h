#pragma once

#include <algorithm>
#include <cstdint>
#include <iosfwd>

/**
 * RLE decompressor for level and art data
 *
 * See doc/kosinski.txt for details on the structure of compressed data
 */
class KosinskiReader
{
public:
  typedef std::pair<bool, size_t> Result;

  KosinskiReader();

  /**
   * Decompress data from an input stream
   *
   * Compressed data will be streamed in from the get position of the provided \c std::istream.
   *
   * If \c bytesWritten is not null, it will be used to return the number of bytes written to the
   * output buffer.
   *
   * The first part of the return value will be \c true if decompression is successful, or \c false
   * if the buffer is too small. The second part of the return value will be the number of bytes
   * that were actually written to the buffer.
   *
   * Any other errors will result in an exception being thrown.
   */
  Result decompress(std::istream&, uint8_t buffer[], size_t bufferSize);

private:
  /// Load the next bitfield from the input stream
  void loadBitfield(std::istream& file);

  /// Get one bit from the bitfield, and reload the next bitfield if we run out of bits
  uint8_t getBit(std::istream& file);

  /// Read one byte from the input stream, and throw an exception if we reach eof
  uint8_t readByte(std::istream& file);

  uint16_t m_bitfield;
  uint16_t m_bitcount;
};
