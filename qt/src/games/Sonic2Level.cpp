#include <cstdint>
#include <vector>

#include "../Palette.h"
#include "../Pattern.h"
#include "../Rom.h"
#include "../SonicReader.h"

#include "Sonic2.h"
#include "Sonic2Level.h"

#define PATTERN_BUFFER_SIZE 0xFFFF  // 64KB

using namespace std;

Sonic2Level::Sonic2Level(Rom& rom,
                         uint32_t palettesAddr,
                         uint32_t patternsAddr)
  : m_palettes(nullptr)
  , m_patterns(nullptr)
  , m_numPatterns(0)
{
  loadPalettes(rom, palettesAddr);
  loadPatterns(rom, patternsAddr);
}

void Sonic2Level::loadPalettes(Rom& rom, uint32_t palettesAddr)
{
  m_palettes = new Palette[4];

  auto buffer = rom.readBytes(palettesAddr, Palette::PALETTE_SIZE_IN_ROM * 4);

  for (int i = 0; i < 4; i++) {
    m_palettes[i].fromSegaFormat(&buffer[Palette::PALETTE_SIZE_IN_ROM * i]);
  }
}

void Sonic2Level::loadPatterns(Rom& rom, uint32_t patternsAddr)
{
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
  m_numPatterns = result.second / Pattern::PATTERN_SIZE_IN_ROM;
  if (result.second % Pattern::PATTERN_SIZE_IN_ROM != 0) {
    throw std::runtime_error("Inconsistent pattern data");
  }

  // convert pattern data
  m_patterns = new Pattern[m_numPatterns];
  for (size_t i = 0; i < m_numPatterns; i++) {
    m_patterns[i].fromSegaFormat(&buffer[i * Pattern::PATTERN_SIZE_IN_ROM]);
  }
}
