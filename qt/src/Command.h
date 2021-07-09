#pragma once

#include <memory>
#include <vector>

class Command
{
public:
  struct Change
  {
    int layer;
    int x;
    int y;
    int value;
  };

  struct Result
  {
    std::shared_ptr<Command> undoCommand;
    std::vector<Change> changes;
  };

  virtual ~Command() = default;

  virtual Result commit() = 0;
};
