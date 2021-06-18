#include "Sonic2.h"

bool Sonic2::isCompatible()
{
  const auto name = readDomesticName();

  return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 2") != name.npos;
}

bool Sonic2::parseLevelData()
{
  // TODO
  return false;
}
