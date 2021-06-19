#include "Sonic3.h"

using namespace std;

uint32_t Sonic3::getBlocksAddr(unsigned int levelIdx)
{
  return 0;
}

uint32_t Sonic3::getChunksAddr(unsigned int levelIdx)
{
  return 0;
}

uint32_t Sonic3::getPalettesAddr(unsigned int levelIdx)
{
  return 0;
}

uint32_t Sonic3::getPatternsAddr(unsigned int levelIdx)
{
  return 0;
}

uint32_t Sonic3::getTilesAddr(unsigned int levelIdx)
{
  return 0;
}

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
