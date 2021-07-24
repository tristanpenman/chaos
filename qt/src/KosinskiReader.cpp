#include <fstream>

#include "KosinskiReader.h"

using namespace std;

KosinskiReader::KosinskiReader()
  : m_bitfield(0)
  , m_bitcount(0)
{

}

uint8_t KosinskiReader::getBit(istream& file)
{
  const uint8_t bit = static_cast<uint8_t>(m_bitfield) & 1;

  m_bitfield >>= 1;
  m_bitcount--;

  // Ensure that there are more bits to read
  if (m_bitcount == 0) {
    loadBitfield(file);
    if (file.eof()) {
      throw std::runtime_error("Unexpected end of file");
    }
  }

  return bit;
}

void KosinskiReader::loadBitfield(istream& file)
{
  m_bitfield  = static_cast<uint16_t>(readByte(file));
  m_bitfield |= static_cast<uint16_t>(readByte(file)) << 8;

  m_bitcount = 16;
}

uint8_t KosinskiReader::readByte(istream& file)
{
  const auto byte = static_cast<uint8_t>(file.get());

  if (file.eof()) {
    throw runtime_error("Unexpected end of stream");
  }

  return byte;
}

KosinskiReader::Result KosinskiReader::decompress(istream& file, uint8_t buffer[], size_t bufferSize)
{
  if (buffer == nullptr) {
    return Result(false, 0);
  }

  uint16_t pos = 0;
  uint16_t count = 0;
  int16_t offset = 0;

  loadBitfield(file);

  while (1) {
    if (getBit(file) == 1) {
      buffer[pos++] = static_cast<uint8_t>(readByte(file));

      // Don't write any more bytes if the buffer is full
      if (pos >= bufferSize) {
        return Result(false, pos);
      }

      continue;
    }

    if (getBit(file) == 1) {
      const uint8_t lo = static_cast<uint8_t>(readByte(file));
      const uint8_t hi = static_cast<uint8_t>(readByte(file));

      // ---hi--- ---lo---
      // OOOOOCCC OOOOOOOO [CCCCCCCC]<- optional
      // O - Offset bit
      // C - Count bit

      // Offsets are negative numbers stored using a 13-bit two's complement representation. Before the offset
      // can be applied to a 16-bit offset, we have to convert it to 16-bit two's complement representation.
      // Positive numbers are not used, so a naive conversion is okay.
      offset = (0xFF00 | hi) << 5;
      offset = (offset & 0xFF00) | lo;

      // Mask off the count bits
      count = hi & 0x7;

      if (count == 0) {
        count = static_cast<uint16_t>(readByte(file));

        if (count == 0) {
          break;
        }

        if (count <= 1) {
          continue;
        }
      } else {
        count++;
      }
    } else {
      count = (getBit(file) << 1) | getBit(file);
      count++;

      // Convert 8-bit two's complement representation to 16-bit representation
      offset = static_cast<int16_t>(readByte(file)) | 0xFF00;
    }

    count++;

    // Copy 'count' bytes from the specified 'offset', relative to the current buffer position
    while (count > 0) {
      buffer[pos] = buffer[pos + offset];
      pos++;
      count--;

      // Don't write any more bytes if the buffer is full
      if (pos > bufferSize) {
        return Result(false, pos);
      }
    }
  }

  return Result(true, pos);
}
