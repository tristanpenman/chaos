#include "../Rom.h"

#include "Sonic2.h"

using namespace std;

constexpr uint32_t levelLayoutDirAddrLoc = 0xE46E;  // Pointer to directory of layout pointers
constexpr uint32_t levelSelectIndex = 0x9454;       // Level select order
constexpr uint32_t levelDataDir = 0x42594;          // Level data pointers (patterns, chunks, blocks)
constexpr uint32_t levelDataDirEntrySize = 12;      // Each pointer is 4 bytes, total of 3 pointers
constexpr uint32_t levelPaletteDir = 0x2782;        // Directory of palette pointers

Sonic2::Sonic2(std::shared_ptr<Rom>& rom)
  : m_rom(rom)
{

}


bool Sonic2::isCompatible()
{
  const auto name = m_rom->readDomesticName();

  return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 2") != name.npos;
}

vector<string> Sonic2::getTitleCards()
{
  return {
    "Emerald Hill Zone - Act 1",
    "Emerald Hill Zone - Act 2",
    "Chemical Plant Zone - Act 1",
    "Chemical Plant Zone - Act 2",
    "Aquatic Ruins Zone - Act 1",
    "Aquatic Ruins Zone - Act 2",
    "Casino Night Zone - Act 1",
    "Casino Night Zone - Act 1",
    "Hill Top Zone - Act 1",
    "Hill Top Zone - Act 2",
    "Mystic Cave Zone - Act 1",
    "Mystic Zone - Act 2",
    "Oil Ocean Zone - Act 1",
    "Oil Ocean Zone - Act 2",
    "Metropolis Zone - Act 1",
    "Metropolis Zone - Act 2",
    "Metropolis Zone - Act 3",
    "Sky Chase Zone - Act 1",
    "Wing Fortress Zone - Act 1",
    "Death Egg Zone - Act 1"
  };
}

uint32_t Sonic2::getBlocksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 8) & 0xFFFFFF;
}

uint32_t Sonic2::getChunksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 4) & 0xFFFFFF;
}

uint32_t Sonic2::getPalettesAddr(unsigned int levelIdx)
{
  const uint32_t dataAddr = getDataAddress(levelIdx, 8);
  const uint32_t paletteIndex = dataAddr >> 24;
  const uint32_t paletteAddrLoc = levelPaletteDir + paletteIndex * 8;

  return m_rom->read32BitAddr(paletteAddrLoc);
}

uint32_t Sonic2::getPatternsAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 0) & 0xFFFFFF;
}

uint32_t Sonic2::getTilesAddr(unsigned int levelIdx)
{
  const uint32_t zoneIdxLoc = levelSelectIndex + levelIdx * 2;
  const uint8_t zoneIdx = m_rom->readByte(zoneIdxLoc);

  const uint32_t actIdxLoc = zoneIdxLoc + 1;
  const uint8_t actIdx = m_rom->readByte(actIdxLoc);

  const uint32_t levelLayoutDirAddr = m_rom->read32BitAddr(levelLayoutDirAddrLoc);
  const uint32_t levelOffsetLoc = levelLayoutDirAddr + zoneIdx * 4 + actIdx * 2;
  const uint16_t levelOffset = m_rom->read16BitAddr(levelOffsetLoc);

  return levelLayoutDirAddr + levelOffset;
}

optional<uint32_t> Sonic2::getExtendedBlocksAddr(unsigned int)
{
  return {};
}

optional<uint32_t> Sonic2::getExtendedChunksAddr(unsigned int)
{
  return {};
}

optional<uint32_t> Sonic2::getExtendedPatternsAddr(unsigned int)
{
  return {};
}

uint32_t Sonic2::getDataAddress(unsigned int levelIdx, unsigned int entryOffset)
{
  const uint32_t levelDataIdxLoc = levelSelectIndex + levelIdx * 2;
  const uint8_t levelDataIdx = m_rom->readByte(levelDataIdxLoc);

  const uint32_t dataAddrLoc = levelDataDir +
      levelDataIdx * levelDataDirEntrySize +
      entryOffset;

  return m_rom->read32BitAddr(dataAddrLoc);
}
