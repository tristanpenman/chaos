#include "../Kosinski.h"
#include "../Logger.h"
#include "../Rom.h"

#include "Sonic2.h"
#include "Sonic2Level.h"

#define LOG Logger("Sonic2")

using namespace std;

static constexpr uint32_t defaultRomSize = 0x100000;            // Size of a standard Sonic 2 ROM (1MB)
static constexpr uint32_t defaultLevelLayoutDirAddr = 0x045A80;
static constexpr uint32_t levelLayoutDirAddrLoc = 0xE46E;       // Pointer to directory of layout pointers
static constexpr uint32_t levelLayoutDirSize = 68;
static constexpr uint32_t levelSelectAddr = 0x9454;             // Level select order
static constexpr uint32_t levelDataDir = 0x42594;               // Level data pointers (patterns, chunks, blocks)
static constexpr uint32_t levelDataDirEntrySize = 12;           // Each pointer is 4 bytes, total of 3 pointers
static constexpr uint32_t levelPaletteDir = 0x2782;             // Directory of palette pointers
static constexpr uint32_t sonicTailsPaletteAddr = 0x29E2;       // Default palette used for Sonic and Tails

Sonic2::Sonic2(shared_ptr<Rom>& rom)
  : m_rom(rom)
{

}

bool Sonic2::isCompatible()
{
  const auto name = m_rom->readDomesticName();

  return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 2") != name.npos;
}

const char* Sonic2::getIdentifier() const
{
  return "Sonic2";
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

shared_ptr<Level> Sonic2::loadLevel(unsigned int levelIdx)
{
  const auto characterPaletteAddr = getCharacterPaletteAddr();
  const auto levelPalettesAddr = getLevelPalettesAddr(levelIdx);
  const auto patternsAddr = getPatternsAddr(levelIdx);
  const auto chunksAddr = getChunksAddr(levelIdx);
  const auto blocksAddr = getBlocksAddr(levelIdx);
  const auto mapAddr = getTilesAddr(levelIdx);

  LOG << "Character palette addr: 0x" << hex << characterPaletteAddr;
  LOG << "Level palettes addr: 0x" << hex << levelPalettesAddr;
  LOG << "Patterns addr: 0x" << hex << patternsAddr;
  LOG << "Chunks addr: 0x" << hex << chunksAddr;
  LOG << "Blocks addr: 0x" << hex << blocksAddr;
  LOG << "Map addr: 0x" << hex << mapAddr;

  return make_shared<Sonic2Level>(*m_rom,
                                  characterPaletteAddr,
                                  levelPalettesAddr,
                                  patternsAddr,
                                  chunksAddr,
                                  blocksAddr,
                                  mapAddr);
}

bool Sonic2::canRelocateLevels() const
{
  return true;
}

bool Sonic2::relocateLevels(bool unsafe)
{
  LOG << "Relocating levels in " << (unsafe ? "unsafe" : "safe") << " mode";

  // check rom size
  const auto romSize = m_rom->getSize();
  if (romSize != defaultRomSize && !unsafe) {
    LOG << "Rom size does not match default; giving up";
    return false;
  }

  // check level layout directory address
  const auto levelLayoutDirAddr = m_rom->read32BitAddr(levelLayoutDirAddrLoc);
  if (levelLayoutDirAddr != defaultLevelLayoutDirAddr && !unsafe) {
    LOG << "Level layout directory is not at expected location; giving up";
    return false;
  }

  // relocate levels using level select order
  const auto maxMapSize = 2 * 128 * 16;
  const auto maxMapCount = levelLayoutDirSize / 2;
  const auto bufferSize = levelLayoutDirSize + maxMapSize * maxMapCount;
  vector<uint8_t> buffer(bufferSize);

  // setup decompression
  auto& file = m_rom->getFile();
  Kosinski kosinski(file);
  vector<uint8_t> mapBuffer(0xFFFFF);

  for (int levelIdx = 0; levelIdx < 20; levelIdx++) {
    LOG << "Relocating level " << levelIdx;

    const uint32_t zoneIdxLoc = levelSelectAddr + levelIdx * 2;
    const uint8_t zoneIdx = m_rom->readByte(zoneIdxLoc);

    const uint32_t actIdxLoc = zoneIdxLoc + 1;
    const uint8_t actIdx = m_rom->readByte(actIdxLoc);

    const uint32_t levelLayoutDirAddr = m_rom->read32BitAddr(levelLayoutDirAddrLoc);
    const uint32_t levelOffsetLoc = levelLayoutDirAddr + zoneIdx * 4 + actIdx * 2;
    const uint16_t levelOffset = m_rom->read16BitAddr(levelOffsetLoc);

    const uint32_t tilesAddr = levelLayoutDirAddr + levelOffset;

    // figure out how many bytes to copy
    file.seekg(tilesAddr);
    const auto result = kosinski.decompress(mapBuffer.data(), mapBuffer.size());
    if (!result.first) {
      stringstream ss;
      ss << "Unable to relocate levels; level decompression failed on level " << levelIdx;
      throw std::runtime_error(ss.str());
    }

    // write new location to buffer
    const uint16_t newLevelOffset = 68 + maxMapSize * levelIdx;
    buffer[zoneIdx * 4 + actIdx * 2] = (newLevelOffset >> 8) & 0xFF;
    buffer[zoneIdx * 4 + actIdx * 2 + 1] = newLevelOffset & 0xFF;

    // copy however much data was read by the decompressor
    const auto bytesToCopy = static_cast<uint32_t>(file.tellg()) - tilesAddr;
    LOG << "Copying " << bytesToCopy << " bytes to 0x" << hex << newLevelOffset;
    file.seekg(tilesAddr);
    file.read(reinterpret_cast<char*>(buffer.data() + newLevelOffset), bytesToCopy);
  }

  m_rom->write32BitAddr(romSize, levelLayoutDirAddrLoc);
  file.seekp(romSize);
  file.write(reinterpret_cast<char*>(buffer.data()), bufferSize);

  // write new rom size
  const auto newAddrRange = romSize + bufferSize - 1;
  LOG << "Writing new address range: " << newAddrRange;
  m_rom->writeSize(newAddrRange);

  // write new checksum
  const auto checksum = m_rom->calculateChecksum();
  LOG << "Writing new checksum: " << checksum;
  m_rom->writeChecksum(checksum);

  return true;
}

uint32_t Sonic2::getDataAddress(unsigned int levelIdx, unsigned int entryOffset)
{
  const uint32_t levelDataIdxLoc = levelSelectAddr + levelIdx * 2;
  const uint8_t levelDataIdx = m_rom->readByte(levelDataIdxLoc);

  const uint32_t dataAddrLoc = levelDataDir +
      levelDataIdx * levelDataDirEntrySize +
      entryOffset;

  return m_rom->read32BitAddr(dataAddrLoc);
}

uint32_t Sonic2::getCharacterPaletteAddr()
{
  return sonicTailsPaletteAddr;
}

uint32_t Sonic2::getLevelPalettesAddr(unsigned int levelIdx)
{
  const uint32_t dataAddr = getDataAddress(levelIdx, 8);
  const uint32_t paletteIndex = dataAddr >> 24;
  const uint32_t paletteAddrLoc = levelPaletteDir + paletteIndex * 8;

  return m_rom->read32BitAddr(paletteAddrLoc);
}

uint32_t Sonic2::getBlocksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 8) & 0xFFFFFF;
}

uint32_t Sonic2::getChunksAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 4) & 0xFFFFFF;
}

uint32_t Sonic2::getPatternsAddr(unsigned int levelIdx)
{
  return getDataAddress(levelIdx, 0) & 0xFFFFFF;
}

uint32_t Sonic2::getTilesAddr(unsigned int levelIdx)
{
  const uint32_t zoneIdxLoc = levelSelectAddr + levelIdx * 2;
  const uint8_t zoneIdx = m_rom->readByte(zoneIdxLoc);

  const uint32_t actIdxLoc = zoneIdxLoc + 1;
  const uint8_t actIdx = m_rom->readByte(actIdxLoc);

  const uint32_t levelLayoutDirAddr = m_rom->read32BitAddr(levelLayoutDirAddrLoc);
  const uint32_t levelOffsetLoc = levelLayoutDirAddr + zoneIdx * 4 + actIdx * 2;
  const uint16_t levelOffset = m_rom->read16BitAddr(levelOffsetLoc);

  return levelLayoutDirAddr + levelOffset;
}
