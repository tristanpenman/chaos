#include <cstdint>
#include <iostream>
#include <vector>

#include "../Chunk.h"
#include "../Palette.h"
#include "../Pattern.h"
#include "../Rom.h"
#include "../SonicReader.h"

#include "Sonic2.h"
#include "Sonic2Level.h"

using namespace std;

Sonic2Level::Sonic2Level(Rom& rom,
                         uint32_t sonicPaletteAddr,
                         uint32_t levelPalettesAddr,
                         uint32_t patternsAddr,
                         uint32_t chunksAddr)
  : m_palettes(nullptr)
  , m_patterns(nullptr)
  , m_chunks(nullptr)
  , m_patternCount(0)
  , m_chunkCount(0)
{
  loadPalettes(rom, sonicPaletteAddr, levelPalettesAddr);
  loadPatterns(rom, patternsAddr);
  loadChunks(rom, chunksAddr);
}

const Palette& Sonic2Level::getPalette(size_t index) const
{
  return m_palettes[index];
}

const Pattern& Sonic2Level::getPattern(size_t index) const
{
  return m_patterns[index];
}

const Chunk& Sonic2Level::getChunk(size_t index) const
{
  return m_chunks[index];
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

void Sonic2Level::loadChunks(Rom &rom, uint32_t chunksAddr)
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
