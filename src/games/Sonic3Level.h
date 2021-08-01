#pragma once

#include <cstdint>

#include "../Level.h"

class Rom;

class Sonic3Level: public Level
{
  static constexpr size_t PALETTE_COUNT = 4;

public:
  Sonic3Level(Rom& rom,
              uint32_t characterPaletteAddr,
              uint32_t levelPalettesAddr,
              uint32_t patternsAddr,
              uint32_t extendedPatternsAddr,
              uint32_t chunksAddr,
              uint32_t extendedChunksAddr,
              uint32_t blocksAddr,
              uint32_t extendedBlocksAddr,
              uint32_t mapAddr);

  size_t getPaletteCount() const override;
  const Palette& getPalette(size_t index) const override;

  size_t getPatternCount() const override;
  const Pattern& getPattern(size_t index) const override;

  size_t getChunkCount() const override;
  const Chunk& getChunk(size_t index) const override;

  size_t getBlockCount() const override;
  const Block& getBlock(size_t index) const override;

  Map& getMap() override;

private:
  Sonic3Level(const Sonic3Level&) = delete;
  Sonic3Level& operator=(const Sonic3Level&) = delete;

  void loadPalettes(Rom& rom, uint32_t characterPaletteAddr, uint32_t levelPalettesAddr);
  void loadPatterns(Rom& rom, uint32_t patternsAddr, uint32_t extendedPatternsAddr);
  void loadChunks(Rom& rom, uint32_t chunksAddr, uint32_t extendedChunksAddr);
  void loadBlocks(Rom& rom, uint32_t blocksAddr, uint32_t extendedBlocksAddr);
  void loadMap(Rom& rom, uint32_t mapAddr);

  Palette* m_palettes;
  Pattern* m_patterns;
  Chunk* m_chunks;
  Block* m_blocks;
  Map* m_map;

  size_t m_patternCount;
  size_t m_chunkCount;
  size_t m_blockCount;
};

inline size_t Sonic3Level::getPaletteCount() const
{
  return PALETTE_COUNT;
}

inline size_t Sonic3Level::getPatternCount() const
{
  return m_patternCount;
}

inline size_t Sonic3Level::getChunkCount() const
{
  return m_chunkCount;
}

inline size_t Sonic3Level::getBlockCount() const
{
  return m_blockCount;
}
