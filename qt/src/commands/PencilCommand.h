#pragma once

#include <unordered_map>

#include "../Command.h"

class Map;

class PencilCommand : public Command
{
public:
  PencilCommand(Map& map);

  void addChange(int layer, int x, int y, int value);

  Result commit() override;

private:
  struct Location
  {
    int layer;
    int x;
    int y;

    bool operator==(const Location &o)  const
    {
      return layer == o.layer && x == o.x && y == o.y;
    }
  };

  struct LocationHash
  {
    std::size_t operator()(const Location& location) const
    {
      return (
          (std::hash<int>()(location.layer) ^
          (std::hash<int>()(location.x) << 1)) >> 1) ^
          (std::hash<int>()(location.y) << 1);
    }
  };

  Map& m_map;

  std::unordered_map<Location, int, LocationHash> m_changes;
};
