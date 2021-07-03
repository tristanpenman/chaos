#include <fstream>

#include "../Block.h"
#include "../Chunk.h"
#include "../Kosinski.h"
#include "../Logger.h"
#include "../Map.h"
#include "../Palette.h"
#include "../Pattern.h"
#include "../Rom.h"

#include "Sonic3Level.h"

#define LOG Logger("Sonic3Level")

using namespace std;

static constexpr uint8_t MAP_LAYERS = 2;

Sonic3Level::Sonic3Level(Rom& rom,
                         uint32_t sonicPaletteAddr,
                         uint32_t levelPalettesAddr,
                         uint32_t patternsAddr,
                         uint32_t extendedPatternsAddr,
                         uint32_t chunksAddr,
                         uint32_t extendedChunksAddr,
                         uint32_t blocksAddr,
                         uint32_t extendedBlocksAddr,
                         uint32_t mapAddr)
  : m_palettes(nullptr)
  , m_patterns(nullptr)
  , m_chunks(nullptr)
  , m_map(nullptr)
  , m_patternCount(0)
  , m_chunkCount(0)
  , m_blockCount(0)
{
  loadPalettes(rom, sonicPaletteAddr, levelPalettesAddr);
  loadPatterns(rom, patternsAddr, extendedPatternsAddr);
  loadChunks(rom, chunksAddr, extendedChunksAddr);
  loadBlocks(rom, blocksAddr, extendedBlocksAddr);
  loadMap(rom, mapAddr);
}

const Palette& Sonic3Level::getPalette(size_t index) const
{
  if (index >= PALETTE_COUNT) {
    throw runtime_error("Invalid palette index");
  }

  return m_palettes[index];
}

const Pattern& Sonic3Level::getPattern(size_t index) const
{
  if (index >= m_patternCount) {
    throw runtime_error("Invalid pattern index");
  }

  return m_patterns[index];
}

const Chunk& Sonic3Level::getChunk(size_t index) const
{
  if (index >= m_chunkCount) {
    throw runtime_error("Invalid chunk index");
  }

  return m_chunks[index];
}

const Block& Sonic3Level::getBlock(size_t index) const
{
  if (index >= m_blockCount) {
    throw runtime_error("Invalid block index");
  }

  return m_blocks[index];
}

const Map& Sonic3Level::getMap() const
{
  return *m_map;
}

void Sonic3Level::loadPalettes(Rom& rom, uint32_t characterPaletteAddr, uint32_t levelPalettesAddr)
{
  m_palettes = new Palette[4];

  {
    auto buffer = rom.readBytes(characterPaletteAddr, Palette::PALETTE_SIZE_IN_ROM);
    m_palettes[0].fromSegaFormat(buffer.data());
  }

  {
    auto buffer = rom.readBytes(levelPalettesAddr, Palette::PALETTE_SIZE_IN_ROM * 3);
    for (int i = 0; i < 3; i++) {
      m_palettes[i + 1].fromSegaFormat(&buffer[Palette::PALETTE_SIZE_IN_ROM * i]);
    }
  }
}

void Sonic3Level::loadPatterns(Rom& rom, uint32_t basePatternsAddr, uint32_t extPatternsAddr)
{
  static constexpr size_t PATTERN_BUFFER_SIZE = 0xFFFFF; // 64KB

  // length of uncompressed data
  const uint16_t baseDataSize = rom.read16BitAddr(basePatternsAddr);
  const uint16_t extDataSize = rom.read16BitAddr(extPatternsAddr);

  // total number of patterns
  m_patternCount = (baseDataSize + extDataSize) / Pattern::PATTERN_SIZE_IN_ROM;
  m_patterns = new Pattern[m_patternCount];

  // setup decompression
  auto& file = rom.getFile();
  Kosinski kosinski(file);
  vector<uint8_t> buffer(PATTERN_BUFFER_SIZE);
  size_t total = 0;
  int patternIndex = 0;

  {
    // base patterns
    file.seekg(basePatternsAddr + 2);
    while (total < baseDataSize) {
      // decompress module
      auto result = kosinski.decompress(buffer.data(), PATTERN_BUFFER_SIZE);
      if (!result.first) {
        throw runtime_error("Base pattern decompression error");
      }

      if (result.second % Pattern::PATTERN_SIZE_IN_ROM != 0) {
        throw runtime_error("Inconsistent base pattern data");
      }

      const auto patternCount = result.second / Pattern::PATTERN_SIZE_IN_ROM;
      for (size_t i = 0; i < patternCount; i++) {
        m_patterns[patternIndex++].fromSegaFormat(&buffer[i * Pattern::PATTERN_SIZE_IN_ROM]);
      }

      // Find the beginning of the next module...
      // modules are padded with zeroes
      char b = 0;
      while (b == 0) {
        b = static_cast<char>(file.get());
      }

      // Set read address to the next packet/module
      file.seekg(-1, ios::cur);

      total += result.second;
    }
  }

  {
    // extended patterns
    file.seekg(extPatternsAddr + 2);
    while (total < baseDataSize + extDataSize) {
      auto result = kosinski.decompress(buffer.data(), PATTERN_BUFFER_SIZE);
      if (!result.first) {
        throw runtime_error("Extended pattern decompression error");
      }

      if (result.second % Pattern::PATTERN_SIZE_IN_ROM != 0) {
        throw runtime_error("Inconsistent extended pattern data");
      }

      const auto patternCount = result.second / Pattern::PATTERN_SIZE_IN_ROM;
      for (size_t i = 0; i < patternCount; i++) {
        m_patterns[patternIndex++].fromSegaFormat(&buffer[i * Pattern::PATTERN_SIZE_IN_ROM]);
      }

      char b = 0;
      while (b == 0) {
        b = static_cast<char>(file.get());
      }

      // Set read address to the next packet/module
      file.seekg(-1, ios::cur);

      total += result.second;
    }
  }

  LOG << "Pattern count: " << m_patternCount << " (total: " << total << " bytes)";
}

void Sonic3Level::loadChunks(Rom& rom, uint32_t baseChunksAddr, uint32_t extChunksAddr)
{
  static constexpr size_t CHUNK_BUFFER_SIZE = 0xFFFF; // 64KB

  // setup decompression
  auto& file = rom.getFile();
  Kosinski kosinski(file);
  vector<uint8_t> buffer(CHUNK_BUFFER_SIZE);
  size_t total = 0;

  {
    // decompress base chunks
    file.seekg(baseChunksAddr);
    auto result = kosinski.decompress(buffer.data(), CHUNK_BUFFER_SIZE);
    if (!result.first) {
      throw runtime_error("Base chunk decompression error");
    }

    if (result.second % Chunk::CHUNK_SIZE_IN_ROM != 0) {
      throw runtime_error("Inconsistent base chunk data");
    }

    m_chunkCount = result.second / Chunk::CHUNK_SIZE_IN_ROM;
    total += result.second;
  }

  {
    // decompress extended chunks
    file.seekg(extChunksAddr);
    auto result = kosinski.decompress(buffer.data() + total, CHUNK_BUFFER_SIZE - total);
    if (!result.first) {
      throw runtime_error("Extended chunk decompression error");
    }

    if (result.second % Chunk::CHUNK_SIZE_IN_ROM != 0) {
      throw runtime_error("Inconsistent extended chunk data");
    }

    m_chunkCount += result.second / Chunk::CHUNK_SIZE_IN_ROM;
    m_chunks = new Chunk[m_chunkCount];

    for (size_t i = 0; i < m_chunkCount; i++) {
      m_chunks[i].fromSegaFormat(&buffer[i * Chunk::CHUNK_SIZE_IN_ROM]);
    }

    total += result.second;
  }

  LOG << "Chunk count: " << m_chunkCount << " (total: " << total << " bytes)";
}

void Sonic3Level::loadBlocks(Rom& rom, uint32_t baseBlocksAddr, uint32_t extBlocksAddr)
{
  static constexpr size_t BLOCK_BUFFER_SIZE = 0xFFFFF; // 64KB

  // setup decompression
  auto& file = rom.getFile();
  Kosinski kosinski(file);
  vector<uint8_t> buffer(BLOCK_BUFFER_SIZE);
  size_t total = 0;

  {
    // decompress base blocks
    file.seekg(baseBlocksAddr);
    auto result = kosinski.decompress(buffer.data(), BLOCK_BUFFER_SIZE);
    if (!result.first) {
      throw runtime_error("Base block decompression error");
    }

    if (result.second % Block::BLOCK_SIZE_IN_ROM != 0) {
      throw runtime_error("Inconsistent base block data");
    }

    m_blockCount = result.second / Block::BLOCK_SIZE_IN_ROM;
    total += result.second;
  }

  {
    // decompress extended blocks
    file.seekg(extBlocksAddr);
    auto result = kosinski.decompress(buffer.data() + total, BLOCK_BUFFER_SIZE - total);
    if (!result.first) {
      throw runtime_error("Extended block decompression error");
    }

    if (result.second % Block::BLOCK_SIZE_IN_ROM != 0) {
      throw runtime_error("Inconsistent extended block data");
    }

    m_blockCount += result.second / Block::BLOCK_SIZE_IN_ROM;
    m_blocks = new Block[m_blockCount];

    for (size_t i = 0; i < m_blockCount; i++) {
      m_blocks[i].fromSegaFormat(&buffer[i * Block::BLOCK_SIZE_IN_ROM]);
    }
  }

  LOG << "Block count: " << m_blockCount << " (total: " << total << " bytes)";
}

void Sonic3Level::loadMap(Rom& rom, uint32_t mapAddr)
{
  // read map header
  const uint16_t rowSizeFg = rom.read16BitAddr(mapAddr);
  const uint16_t rowSizeBg = rom.read16BitAddr(mapAddr + 2);
  const uint16_t rowCountFg = rom.read16BitAddr(mapAddr + 4);
  const uint16_t rowCountBg = rom.read16BitAddr(mapAddr + 6);

  // create map
  const uint16_t mapWidth = max(rowSizeBg, rowSizeFg);
  const uint16_t mapHeight = max(rowCountBg, rowCountFg);
  m_map = new Map(MAP_LAYERS, mapWidth, mapHeight);

  // setup for reading values
  auto& file = rom.getFile();
  const size_t bufferSize = sizeof(uint8_t) * mapWidth;
  vector<uint8_t> buffer(bufferSize);
  const uint32_t ptrTableAddr = mapAddr + 8;

  // read rows
  for (uint16_t rowIndex = 0; rowIndex < rowCountFg; rowIndex++) {
    const streamoff rowOffset = rom.read16BitAddr(ptrTableAddr + rowIndex * 4) - 0x8000;
    file.seekg(ptrTableAddr + rowOffset);
    file.read(reinterpret_cast<char*>(buffer.data()), bufferSize);

    // set tiles
    for (uint16_t colIndex = 0; colIndex < rowSizeFg; colIndex++) {
       m_map->setValue(0, colIndex, rowIndex, buffer[colIndex]);
    }
  }

  LOG << "Map size: " << mapWidth << "x" << mapHeight;
}
