#pragma once

#include "../Level.h"

class Sonic2Level: public Level
{
public:
  bool loadLevel(unsigned int levelIdx) override;
};
