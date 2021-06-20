#pragma once

#include "../Level.h"

class Sonic3Level: public Level
{
public:
  bool loadLevel(unsigned int levelIdx) override;
};
