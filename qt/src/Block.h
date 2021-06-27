#pragma once

#include <cstdint>

#include "ChunkDesc.h"

class Chunk;

class Block
{
public:
  static constexpr uint8_t BLOCK_HEIGHT = 128;
  static constexpr uint8_t BLOCK_WIDTH = 128;
  static constexpr uint8_t BYTES_PER_CHUNK = 2;
  static constexpr uint8_t CHUNKS_PER_BLOCK = 64;
  static constexpr uint8_t BLOCK_SIZE_IN_ROM = CHUNKS_PER_BLOCK * BYTES_PER_CHUNK;

  Block();

  void fromSegaFormat(uint8_t buffer[BLOCK_SIZE_IN_ROM]);

  const ChunkDesc& getChunkDesc(uint8_t x, uint8_t y) const;

private:
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;

  ChunkDesc m_chunkDescs[CHUNKS_PER_BLOCK];
};
