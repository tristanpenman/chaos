#pragma once

#include <unordered_map>

#include "../Command.h"

class Map;

class PencilCommand : public Command
{
public:
  PencilCommand(Map& map);

  void addChange(uint8_t layer, uint16_t x, uint16_t y, uint8_t value);

  Result commit() override;

private:
  struct Location
  {
    uint8_t layer;
    uint16_t x;
    uint16_t y;

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

  std::unordered_map<Location, uint8_t, LocationHash> m_changes;
};
