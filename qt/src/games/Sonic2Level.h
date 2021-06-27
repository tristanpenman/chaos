#pragma once

#include <memory>

#include "../Level.h"

class Block;
class Chunk;
class Palette;
class Pattern;
class Rom;

class Sonic2Level : public Level
{
  static constexpr size_t PALETTE_COUNT = 4;

public:
  Sonic2Level(Rom& rom,
              uint32_t characterPaletteAddr,
              uint32_t levelPalettesAddr,
              uint32_t patternsAddr,
              uint32_t chunksAddr,
              uint32_t blocksAddr);

  size_t getPaletteCount() const override;
  const Palette& getPalette(size_t index) const override;

  size_t getPatternCount() const override;
  const Pattern& getPattern(size_t index) const override;

  size_t getChunkCount() const override;
  const Chunk& getChunk(size_t index) const override;

  size_t getBlockCount() const override;
  const Block& getBlock(size_t index) const override;

private:
  Sonic2Level(const Sonic2Level&) = delete;
  Sonic2Level& operator=(const Sonic2Level&) = delete;

  void loadPalettes(Rom& rom, uint32_t characterPaletteAddr, uint32_t levelPalettesAddr);
  void loadPatterns(Rom& rom, uint32_t patternsAddr);
  void loadChunks(Rom& rom, uint32_t chunksAddr);
  void loadBlocks(Rom& rom, uint32_t blocksAddr);

  Palette* m_palettes;
  Pattern* m_patterns;
  Chunk* m_chunks;
  Block* m_blocks;

  size_t m_patternCount;
  size_t m_chunkCount;
  size_t m_blockCount;
};

inline size_t Sonic2Level::getPaletteCount() const
{
  return Sonic2Level::PALETTE_COUNT;
}

inline size_t Sonic2Level::getPatternCount() const
{
  return m_patternCount;
}

inline size_t Sonic2Level::getChunkCount() const
{
  return m_chunkCount;
}

inline size_t Sonic2Level::getBlockCount() const
{
  return m_blockCount;
}
