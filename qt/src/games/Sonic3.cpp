#include <iostream>

#include "../Rom.h"

#include "Sonic3.h"
#include "Sonic3Level.h"

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

vector<string> Sonic3::getTitleCards()
{
  return {
    "Angel Island Zone - Act 1",
    "Angel Island Zone - Act 2",
    "Hydrocity Zone - Act 1",
    "Hydrocity Zone - Act 2",
    "Marble Garden Zone - Act 1",
    "Marble Garden Zone - Act 2",
    "Carnival Night Zone - Act 1",
    "Carnival Night Zone - Act 1",
    "--",
    "--",
    "Ice Cap Zone - Act 1",
    "Ice Cap Zone - Act 2",
    "Launch Base Zone - Act 1",
    "Launch Base Zone - Act 2"
  };
}

shared_ptr<Level> Sonic3::loadLevel(unsigned int levelIdx)
{
  const uint32_t characterPaletteAddr = getCharacterPaletteAddr();
  const uint32_t levelPalettesAddr = getLevelPalettesAddr(levelIdx);
  const uint32_t patternsAddr = getPatternsAddr(levelIdx);
  const uint32_t extendedPatternsAddr = getExtendedPatternsAddr(levelIdx);
  const uint32_t chunksAddr = getChunksAddr(levelIdx);
  const uint32_t extendedChunksAddr = getExtendedChunksAddr(levelIdx);
  const uint32_t blocksAddr = getBlocksAddr(levelIdx);
  const uint32_t extendedBlocksAddr = getExtendedBlocksAddr(levelIdx);
  const uint32_t mapAddr = getTilesAddr(levelIdx);

  cout << "[Sonic3] Character palette addr: 0x" << hex << characterPaletteAddr << dec << endl;
  cout << "[Sonic3] Level palettes addr: 0x" << hex << levelPalettesAddr << dec << endl;
  cout << "[Sonic3] Patterns addr: 0x" << hex << patternsAddr << dec << endl;
  cout << "[Sonic3] Extended patterns addr: 0x" << hex << extendedPatternsAddr << dec << endl;
  cout << "[Sonic3] Chunks addr: 0x" << hex << chunksAddr << dec << endl;
  cout << "[Sonic3] Extended chunks addr: 0x" << hex << extendedChunksAddr << dec << endl;
  cout << "[Sonic3] Blocks addr: 0x" << hex << blocksAddr << dec << endl;
  cout << "[Sonic3] Extended blocks addr: 0x" << hex << extendedBlocksAddr << dec << endl;
  cout << "[Sonic3] Map addr: 0x" << hex << mapAddr << dec << endl;

  return std::make_shared<Sonic3Level>(*m_rom,
                                       characterPaletteAddr,
                                       levelPalettesAddr,
                                       patternsAddr,
                                       extendedPatternsAddr,
                                       chunksAddr,
                                       extendedChunksAddr,
                                       blocksAddr,
                                       extendedBlocksAddr,
                                       mapAddr);
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

uint32_t Sonic3::getCharacterPaletteAddr()
{
  return 0x8C234;
}

uint32_t Sonic3::getLevelPalettesAddr(unsigned int levelIdx)
{
  const uint32_t dataAddr = getDataAddress(levelIdx, 8);
  const uint32_t paletteIdx = dataAddr >> 24;
  const uint32_t paletteAddrLoc = levelPaletteDir + paletteIdx * 8;

  return m_rom->read32BitAddr(paletteAddrLoc);
}

uint32_t Sonic3::getBlocksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 16) & 0xFFFFFF;
}

uint32_t Sonic3::getChunksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 8) & 0xFFFFFF;
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

uint32_t Sonic3::getExtendedBlocksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 20) & 0xFFFFFF;
}

uint32_t Sonic3::getExtendedChunksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 12) & 0xFFFFFF;
}

uint32_t Sonic3::getExtendedPatternsAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 4) & 0xFFFFFF;
}
