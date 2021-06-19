#include "Sonic2.h"

constexpr uint32_t levelLayoutDirAddrLoc(0xE46E);  // Pointer to directory of layout pointers
constexpr uint32_t levelLayoutDirAddr(0x45A80);    // Layout pointers are found here
constexpr uint32_t levelSelectIndex(0x9454);       // Level select order
constexpr uint32_t levelDataDir(0x42594);          // Level data pointers (patterns, chunks, blocks)
constexpr uint32_t levelDataDirEntrySize(12);      // Each pointer is 4 bytes, total of 3 pointers
constexpr uint32_t levelPaletteDir(0x2782);        // Directory of palette pointers

using namespace std;

bool Sonic2::isCompatible()
{
  const auto name = readDomesticName();

  return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 2") != name.npos;
}

bool Sonic2::parseLevelData()
{
  // TODO
  return true;
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

  return read32BitAddr(paletteAddrLoc);
}

uint32_t Sonic2::getPatternsAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 0) & 0xFFFFFF;
}

uint32_t Sonic2::getTilesAddr(unsigned int levelIdx)
{
  const uint8_t zoneIdx = readByte(levelSelectIndex + levelIdx * 2);
  const uint8_t actIdx = readByte(levelSelectIndex + levelIdx * 2 + 1);

  const uint32_t dirAddr = read32BitAddr(levelLayoutDirAddrLoc);
  const uint16_t levelOffset = read16BitAddr(dirAddr + zoneIdx * 4 + actIdx * 2);

  return dirAddr + levelOffset;
}

uint32_t Sonic2::getDataAddress(unsigned int levelIdx, unsigned int entryOffset)
{
  const uint8_t dirIndex = readByte(levelSelectIndex + levelIdx * 2);
  const uint32_t dataAddrLoc = levelDataDir + dirIndex * levelDataDirEntrySize + entryOffset;

  return read32BitAddr(dataAddrLoc);
}
