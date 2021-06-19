#pragma once

#include "../Rom.h"

class Sonic2 : public Rom
{
public:
  Sonic2(const std::shared_ptr<std::fstream>& file);

  ~Sonic2() override = default;

  bool isCompatible() override;
  bool parseLevelData() override;

  uint32_t getBlocksAddr(unsigned int levelIdx) override;
  uint32_t getChunksAddr(unsigned int levelIdx) override;
  uint32_t getPalettesAddr(unsigned int levelIdx) override;
  uint32_t getPatternsAddr(unsigned int levelIdx) override;
  uint32_t getTilesAddr(unsigned int levelIdx) override;

  std::optional<uint32_t> getExtendedBlocksAddr(unsigned int levelIdx) override;
  std::optional<uint32_t> getExtendedChunksAddr(unsigned int levelIdx) override;
  std::optional<uint32_t> getExtendedPatternsAddr(unsigned int levelIdx) override;

private:
  uint32_t getDataAddress(unsigned int levelIdx, unsigned int entryOffset);
};

inline Sonic2::Sonic2(const std::shared_ptr<std::fstream>& file)
  : Rom(file)
{

}
