#include "../Rom.h"

#include "Sonic3.h"

const uint32_t levelLayoutDirAddr = 0x81360;  // Layout pointers are found here
const uint32_t levelSelectIndex = 0x6A8E;     // Level select order
const uint32_t levelDataDir = 0x5AB0C;        // Level data pointers (patterns, chunks, blocks)
const uint32_t levelDataDirEntrySize = 24;    // Each pointer is 4 bytestotal of 3 pointers
const uint32_t levelPaletteDir = 0x8BF54;     // Directory of palette pointers

using namespace std;

Sonic3::Sonic3(std::shared_ptr<Rom>& rom)
  : m_rom(rom)
{

}

bool Sonic3::isCompatible()
{
  const auto name = m_rom->readDomesticName();

  return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 3") != name.npos;
}

bool Sonic3::parseLevelData()
{
  // TODO
  return true;
}

uint32_t Sonic3::getBlocksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 16) & 0xFFFFFF;
}

uint32_t Sonic3::getChunksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 8) & 0xFFFFFF;
}

uint32_t Sonic3::getPalettesAddr(unsigned int levelIdx)
{
  const uint32_t dataAddr = getDataAddress(levelIdx, 8);
  const uint32_t paletteIdx = dataAddr >> 24;
  const uint32_t paletteAddrLoc = levelPaletteDir + paletteIdx * 8;

  return m_rom->read32BitAddr(paletteAddrLoc);
}

uint32_t Sonic3::getPatternsAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 0) & 0xFFFFFF;
}

uint32_t Sonic3::getTilesAddr(unsigned int levelIdx)
{
  const uint32_t tilesAddrLoc = levelLayoutDirAddr + levelIdx * 4;

  return m_rom->read32BitAddr(tilesAddrLoc);
}

optional<uint32_t> Sonic3::getExtendedBlocksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 20) & 0xFFFFFF;
}

optional<uint32_t> Sonic3::getExtendedChunksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 12) & 0xFFFFFF;
}

optional<uint32_t> Sonic3::getExtendedPatternsAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 4) & 0xFFFFFF;
}

uint32_t Sonic3::getDataAddress(unsigned int levelIdx, unsigned int entryOffset)
{
  const uint32_t zoneIndexLoc = levelSelectIndex + levelIdx * 2;
  const uint32_t zoneIndex = m_rom->readByte(zoneIndexLoc);

  const uint32_t actIndexLoc = zoneIndexLoc + 1;
  const uint32_t actIndex = m_rom->readByte(actIndexLoc);

  const uint32_t dataAddrLoc = levelDataDir +
      zoneIndex * levelDataDirEntrySize * 2 +
      actIndex * levelDataDirEntrySize +
      entryOffset;

  return m_rom->read32BitAddr(dataAddrLoc);
}
