#include <cstring>
#include <stdexcept>

#include "Block.h"

void Block::fromSegaFormat(uint8_t buffer[BLOCK_SIZE_IN_ROM])
{
  for (unsigned int i = 0; i < CHUNKS_PER_BLOCK; i++) {
    uint16_t index = (static_cast<uint16_t>(buffer[0]) << 8) & 0xFF00;
    index |= (buffer[1]) & 0x00FF;

    m_chunkDescs[i].set(index);

    buffer += ChunkDesc::getIndexSize();
  }
}

const ChunkDesc& Block::getChunkDesc(uint8_t x, uint8_t y) const
{
  if (x > 7 || y > 7) {
    throw std::runtime_error("Invalid chunk index");
  }

  return m_chunkDescs[y * 8 + x];
}
