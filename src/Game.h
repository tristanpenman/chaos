#pragma once

#include <memory>
#include <string>
#include <vector>

class Level;

class Game
{
public:
  virtual ~Game() = default;

  virtual bool isCompatible() = 0;

  virtual const char* getIdentifier() const = 0;

  virtual std::vector<std::string> getTitleCards() = 0;

  virtual std::shared_ptr<Level> loadLevel(unsigned int levelIdx) = 0;

  virtual bool canRelocateLevels() const = 0;
  virtual bool canSave() const = 0;

  virtual bool relocateLevels(bool unsafe) = 0;
  virtual bool save(unsigned int levelIdx, Level&) = 0;
};
