#pragma once

#include "../Command.h"

class Map;

class PencilCommand : public Command
{
public:
  PencilCommand(Map& map, int x, int y, int block);

  Result perform() override;

private:
  Map& m_map;
  int m_x;
  int m_y;
  int m_block;
};
