#pragma once

#include <memory>
#include <vector>

class Command
{
public:
  struct Change
  {
    uint8_t layer;
    uint16_t x;
    uint16_t y;
    uint8_t value;
  };

  struct Result
  {
    std::shared_ptr<Command> undoCommand;
    std::vector<Change> changes;
  };

  virtual ~Command() = default;

  virtual Result commit() = 0;
};
