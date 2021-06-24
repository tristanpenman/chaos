#pragma once

#include <memory>

#include "../Game.h"

class Rom;

class Sonic2 : public Game
{
public:
  Sonic2(std::shared_ptr<Rom>& rom);

  ~Sonic2() override = default;

  bool isCompatible() override;

  std::vector<std::string> getTitleCards() override;

  std::shared_ptr<Level> loadLevel(unsigned int levelIdx) override;

private:
  uint32_t getDataAddress(uint32_t levelIdx, uint32_t entryOffset);

  uint32_t getBlocksAddr(uint32_t levelIdx);
  uint32_t getChunksAddr(uint32_t levelIdx);
  uint32_t getPalettesAddr(uint32_t levelIdx);
  uint32_t getPatternsAddr(uint32_t levelIdx);
  uint32_t getTilesAddr(uint32_t levelIdx);

  std::shared_ptr<Rom> m_rom;
};
