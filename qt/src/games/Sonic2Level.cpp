#include <cstdint>
#include <iostream>
#include <vector>

#include "../Block.h"
#include "../Chunk.h"
#include "../Map.h"
#include "../Palette.h"
#include "../Pattern.h"
#include "../Rom.h"
#include "../SonicReader.h"

#include "Sonic2.h"
#include "Sonic2Level.h"

static constexpr uint8_t MAP_LAYERS = 2;
static constexpr uint8_t MAP_HEIGHT = 16;
static constexpr uint8_t MAP_WIDTH = 128;

using namespace std;

Sonic2Level::Sonic2Level(Rom& rom,
                         uint32_t sonicPaletteAddr,
                         uint32_t levelPalettesAddr,
                         uint32_t patternsAddr,
                         uint32_t chunksAddr,
                         uint32_t blocksAddr,
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
  loadPatterns(rom, patternsAddr);
  loadChunks(rom, chunksAddr);
  loadBlocks(rom, blocksAddr);
  loadMap(rom, mapAddr);
}

const Palette& Sonic2Level::getPalette(size_t index) const
{
  if (index >= PALETTE_COUNT) {
    throw std::runtime_error("Invalid palette index");
  }

  return m_palettes[index];
}

const Pattern& Sonic2Level::getPattern(size_t index) const
{
  if (index >= m_patternCount) {
    throw std::runtime_error("Invalid pattern index");
  }

  return m_patterns[index];
}

const Chunk& Sonic2Level::getChunk(size_t index) const
{
  if (index >= m_chunkCount) {
    throw std::runtime_error("Invalid chunk index");
  }

  return m_chunks[index];
}

const Block& Sonic2Level::getBlock(size_t index) const
{
  if (index >= m_blockCount) {
    throw std::runtime_error("Invalid block index");
  }

  return m_blocks[index];
}

const Map& Sonic2Level::getMap() const
{
  return *m_map;
}

void Sonic2Level::loadPalettes(Rom& rom, uint32_t characterPaletteAddr, uint32_t levelPalettesAddr)
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

void Sonic2Level::loadPatterns(Rom& rom, uint32_t patternsAddr)
{
  static constexpr size_t PATTERN_BUFFER_SIZE = 0xFFFF; // 64KB

  // decompress patterns
  auto& file = rom.getFile();
  file.seekg(patternsAddr);
  SonicReader sr(file);
  std::vector<uint8_t> buffer(PATTERN_BUFFER_SIZE);
  auto result = sr.decompress(buffer.data(), PATTERN_BUFFER_SIZE);
  if (!result.first) {
    throw std::runtime_error("Pattern decompression failed");
  }

  // check data
  m_patternCount = result.second / Pattern::PATTERN_SIZE_IN_ROM;
  if (result.second % Pattern::PATTERN_SIZE_IN_ROM != 0) {
    throw std::runtime_error("Inconsistent pattern data");
  }

  // convert pattern data
  m_patterns = new Pattern[m_patternCount];
  for (size_t i = 0; i < m_patternCount; i++) {
    m_patterns[i].fromSegaFormat(&buffer[i * Pattern::PATTERN_SIZE_IN_ROM]);
  }

  cout << "[Sonic2Level] Pattern count: " << m_patternCount << " (" << result.second << " bytes)" << endl;
}

void Sonic2Level::loadChunks(Rom& rom, uint32_t chunksAddr)
{
  static constexpr size_t CHUNK_BUFFER_SIZE = 0xFFFF; // 64KB

  // decompress chunks
  auto& file = rom.getFile();
  file.seekg(chunksAddr);
  SonicReader sr(file);
  vector<uint8_t> buffer(CHUNK_BUFFER_SIZE);
  SonicReader::Result result = sr.decompress(buffer.data(), CHUNK_BUFFER_SIZE);
  if (!result.first) {
    throw std::runtime_error("Chunk decompression error");
  }

  // check data
  m_chunkCount = result.second / Chunk::CHUNK_SIZE_IN_ROM;
  if (result.second % Chunk::CHUNK_SIZE_IN_ROM != 0) {
    throw std::runtime_error("Inconsistent chunk data");
  }

  // convert chunk data
  m_chunks = new Chunk[m_chunkCount];
  for (size_t i = 0; i < m_chunkCount; i++) {
    m_chunks[i].fromSegaFormat(&buffer[i * Chunk::CHUNK_SIZE_IN_ROM]);
  }

  cout << "[Sonic2Level] Chunk count: " << m_chunkCount << " (" << result.second << " bytes)" << endl;
}

void Sonic2Level::loadBlocks(Rom& rom, uint32_t blocksAddr)
{
  static constexpr size_t BLOCK_BUFFER_SIZE = 0xFFFF; // 64KB

  // decompress blocks
  auto& file = rom.getFile();
  file.seekg(blocksAddr);
  SonicReader sr(file);
  vector<uint8_t> buffer(BLOCK_BUFFER_SIZE);
  SonicReader::Result result = sr.decompress(buffer.data(), BLOCK_BUFFER_SIZE);
  if (!result.first) {
    throw std::runtime_error("Block decompression error");
  }

  // check data
  m_blockCount = result.second / Block::BLOCK_SIZE_IN_ROM;
  if (result.second % Block::BLOCK_SIZE_IN_ROM != 0) {
    throw std::runtime_error("Inconsistent block data");
  }

  m_blocks = new Block[m_blockCount];
  for (size_t i = 0; i < m_blockCount; i++) {
    m_blocks[i].fromSegaFormat(&buffer[i * Block::BLOCK_SIZE_IN_ROM]);
  }

  cout << "[Sonic2Level] Block count: " << m_blockCount << " (" << result.second << " bytes)" << endl;
}

void Sonic2Level::loadMap(Rom& rom, uint32_t mapAddr)
{
  static constexpr size_t MAP_BUFFER_SIZE = 0xFFFF; // 64KB

  auto& file = rom.getFile();
  file.seekg(mapAddr);
  vector<unsigned char> buffer(MAP_BUFFER_SIZE);

  SonicReader reader(file);
  SonicReader::Result result = reader.decompress(buffer.data(), MAP_BUFFER_SIZE);
  if (!result.first) {
    throw std::runtime_error("Map decompression error");
  }

  // check data
  if (result.second != MAP_LAYERS * MAP_HEIGHT * MAP_WIDTH) {
    throw std::runtime_error("Inconsistent map data");
  }

  m_map = new Map(MAP_LAYERS, MAP_WIDTH, MAP_HEIGHT, buffer.data());
}
