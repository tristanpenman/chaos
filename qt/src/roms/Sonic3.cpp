#include "Sonic3.h"

bool Sonic3::isCompatible()
{
  const auto name = readDomesticName();

  return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 3") != name.npos;
}

bool Sonic3::parseLevelData()
{
  // TODO
  return false;
}
