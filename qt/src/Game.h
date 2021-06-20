#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class Game
{
public:
  virtual ~Game() = default;

  virtual bool isCompatible() = 0;

  virtual std::vector<std::string> getTitleCards() = 0;

  virtual uint32_t getPalettesAddr(uint32_t levelIdx) = 0;
  virtual uint32_t getPatternsAddr(uint32_t levelIdx) = 0;
  virtual uint32_t getChunksAddr(uint32_t levelIdx) = 0;
  virtual uint32_t getBlocksAddr(uint32_t levelIdx) = 0;
  virtual uint32_t getTilesAddr(uint32_t levelIdx) = 0;

  virtual std::optional<uint32_t> getExtendedBlocksAddr(uint32_t levelIdx) = 0;
  virtual std::optional<uint32_t> getExtendedChunksAddr(uint32_t levelIdx) = 0;
  virtual std::optional<uint32_t> getExtendedPatternsAddr(uint32_t levelIdx) = 0;
};
