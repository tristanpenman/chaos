#pragma once

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <vector>

/**
 * RLE compressor for level and art data
 *
 * See doc/kosinski.txt for details on the structure of compressed data
 */
class KosinskiWriter
{
public:
  typedef std::pair<bool, size_t> Result;

  /**
   * Compress data to an output stream
   *
   * Compressed data will be written to the put position of the provided \c std::fstream. An
   * optional byte limit can be provided, and compression will fail if this limit would be exceeded.
   *
   * The return value will be \c true if compression was successful, or \c false if the byte limit
   * would be exceeded. Data will only be written to the output stream if the byte limit is not
   * exceeded. Any other errors will result in an exception being thrown.
   */
  Result compress(std::ostream&,
                  const uint8_t data[],
                  size_t dataSize,
                  std::optional<size_t> byteLimit = std::nullopt);

private:
  void writeByte(int byte);

  /// Write a control bit to the output stream
  void addBit(int bit);

  /// Compare two segments to see if they are equal
  bool segmentsEqual(const uint8_t data[], int32_t start, int32_t length, int32_t offset);

  /// Attempt to find a copy of a segment
  int32_t findSegment(const uint8_t data[], int32_t pos, int32_t length);

  int m_bitcount;
  uint16_t m_bitfield;
  int m_bitfields;
  std::vector<uint8_t> m_buffer;

  std::streamoff m_bitfieldPos;
};
