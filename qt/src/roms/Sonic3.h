#pragma once

#include "../Rom.h"

class Sonic3 : public Rom
{
public:
  Sonic3(const std::shared_ptr<std::fstream>& file);

  ~Sonic3() override = default;

  uint32_t getBlocksAddr(unsigned int levelIdx) override;
  uint32_t getChunksAddr(unsigned int levelIdx) override;
  uint32_t getPalettesAddr(unsigned int levelIdx) override;
  uint32_t getPatternsAddr(unsigned int levelIdx) override;
  uint32_t getTilesAddr(unsigned int levelIdx) override;

  bool isCompatible() override;
  bool parseLevelData() override;
};

inline Sonic3::Sonic3(const std::shared_ptr<std::fstream>& file)
  : Rom(file)
{

}
