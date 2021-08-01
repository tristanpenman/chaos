#pragma once

#include <memory>

#include "../Game.h"

class Rom;

class Sonic3 : public Game
{
public:
  Sonic3(std::shared_ptr<Rom>& rom);

  bool isCompatible() override;
  const char* getIdentifier() const override;
  std::vector<std::string> getTitleCards() override;
  std::shared_ptr<Level> loadLevel(unsigned int levelIdx) override;
  bool canRelocateLevels() const override;
  bool canSave() const override;
  bool relocateLevels(bool unsafe) override;
  bool save(unsigned int levelIdx, Level&) override;

private:
  uint32_t getDataAddress(uint32_t levelIdx, uint32_t entryOffset);

  uint32_t getCharacterPaletteAddr();
  uint32_t getLevelPalettesAddr(uint32_t levelIdx);

  uint32_t getBlocksAddr(uint32_t levelIdx);
  uint32_t getChunksAddr(uint32_t levelIdx);
  uint32_t getPatternsAddr(uint32_t levelIdx);
  uint32_t getTilesAddr(uint32_t levelIdx);

  uint32_t getExtendedBlocksAddr(uint32_t levelIdx);
  uint32_t getExtendedChunksAddr(uint32_t levelIdx);
  uint32_t getExtendedPatternsAddr(uint32_t levelIdx);

  std::shared_ptr<Rom> m_rom;
};
