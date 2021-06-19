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
  bool parseLevelData() override;

  uint32_t getBlocksAddr(uint32_t levelIdx) override;
  uint32_t getChunksAddr(uint32_t levelIdx) override;
  uint32_t getPalettesAddr(uint32_t levelIdx) override;
  uint32_t getPatternsAddr(uint32_t levelIdx) override;
  uint32_t getTilesAddr(uint32_t levelIdx) override;

  std::optional<uint32_t> getExtendedBlocksAddr(uint32_t levelIdx) override;
  std::optional<uint32_t> getExtendedChunksAddr(uint32_t levelIdx) override;
  std::optional<uint32_t> getExtendedPatternsAddr(uint32_t levelIdx) override;

private:
  uint32_t getDataAddress(uint32_t levelIdx, uint32_t entryOffset);

  std::shared_ptr<Rom> m_rom;
};
