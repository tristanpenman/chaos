#pragma once

#include <memory>

#include "../Game.h"

class Rom;

class Sonic2 : public Game
{
public:
  Sonic2(std::shared_ptr<Rom>& rom);

  bool isCompatible() override;
  const char* getIdentifier() const override;
  std::vector<std::string> getTitleCards() override;
  std::shared_ptr<Level> loadLevel(unsigned int levelIdx) override;
  bool canRelocateLevels() const override;
  bool relocateLevels() override;

private:
  uint32_t getDataAddress(uint32_t levelIdx, uint32_t entryOffset);

  // uncompressed
  uint32_t getCharacterPaletteAddr();
  uint32_t getLevelPalettesAddr(uint32_t levelIdx);

  // Kosinski compressed
  uint32_t getBlocksAddr(uint32_t levelIdx);
  uint32_t getChunksAddr(uint32_t levelIdx);
  uint32_t getPatternsAddr(uint32_t levelIdx);
  uint32_t getTilesAddr(uint32_t levelIdx);

  std::shared_ptr<Rom> m_rom;
};
