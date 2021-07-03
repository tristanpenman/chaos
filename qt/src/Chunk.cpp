#include <cstring>
#include <stdexcept>

#include "Pattern.h"

#include "Chunk.h"

Chunk::Chunk()
{
  memset(m_patternDescs, 0, sizeof(PatternDesc) * PATTERNS_PER_CHUNK);
}

void Chunk::fromSegaFormat(uint8_t *buffer)
{
  for (unsigned int i = 0; i < PATTERNS_PER_CHUNK; i++) {
    uint16_t index = (static_cast<uint16_t>(buffer[0]) << 8) & 0xFF00;
    index |= (buffer[1]) & 0x00FF;

    // Set index
    m_patternDescs[i].set(index);

    buffer += PatternDesc::getIndexSize();
  }
}

const PatternDesc& Chunk::getPatternDesc(uint8_t x, uint8_t y) const
{
  if (x > 1 || y > 1) {
    throw std::runtime_error("Invalid pattern index");
  }

  return m_patternDescs[y * 2 + x];
}
