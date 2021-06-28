#pragma once

#include <cstddef>

class Block;
class Chunk;
class Map;
class Palette;
class Pattern;

class Level
{
public:
  virtual ~Level() {}

  virtual size_t getPaletteCount() const = 0;
  virtual const Palette& getPalette(size_t index) const = 0;

  virtual size_t getPatternCount() const = 0;
  virtual const Pattern& getPattern(size_t index) const = 0;

  virtual size_t getChunkCount() const = 0;
  virtual const Chunk& getChunk(size_t index) const = 0;

  virtual size_t getBlockCount() const = 0;
  virtual const Block& getBlock(size_t index) const = 0;

  virtual const Map& getMap() const = 0;
};
