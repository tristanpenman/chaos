#include "Precompiled.h"

#include "Rom.h"
#include "Sonic2Rom.h"
#include "Sonic3Rom.h"

#include "RomFactory.h"

using namespace std;

Rom* RomFactory::loadROM(fstream& rom)
{
    Rom* pRom = NULL;

    // Check for Sonic 2 ROM
    pRom = new Sonic2Rom(rom);
    if (pRom->validateROM())
    {
        // Found a Sonic2 ROM
        return pRom;
    }

    // Not Sonic 2
    delete pRom;

    // Check for Sonic 3 ROM
    pRom = new Sonic3Rom(rom);
    if (pRom->validateROM())
    {
        return pRom;  // Found a Sonic 3 ROM
    }

    // Not Sonic 3
    delete pRom;

    // ROM not supported
    throw std::runtime_error("Could not load selected ROM");
}
