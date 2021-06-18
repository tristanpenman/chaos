#include <istream>

#include "RomFactory.h"

#include "roms/Sonic2.h"
#include "roms/Sonic3.h"

using namespace std;

shared_ptr<Rom> RomFactory::build(shared_ptr<fstream>& m_file)
{
  // try Sonic2
  shared_ptr<Rom> rom = make_shared<Sonic2>(m_file);
  if (rom->isCompatible()) {
    return rom;
  }

  // try Sonic3
  rom.reset(new Sonic3(m_file));
  if (rom->isCompatible()) {
    return rom;
  }

  return nullptr;
}
