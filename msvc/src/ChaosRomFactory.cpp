#include "Precompiled.h"

#include "ChaosRom.h"
#include "ChaosRom_Sonic2.h"
#include "ChaosRom_Sonic3.h"
#include "ChaosRomFactory.h"

using namespace std;

ChaosRom* ChaosRomFactory::loadROM(fstream& rom)
{
    ChaosRom* pRom = NULL;

    // Check for Sonic 2 ROM
    pRom = new ChaosRom_Sonic2(rom);
    if (pRom->validateROM())
    {
        // Found a Sonic2 ROM
        return pRom;
    }

    // Not Sonic 2
    delete pRom;

    // Check for Sonic 3 ROM
    pRom = new ChaosRom_Sonic3(rom);
    if (pRom->validateROM())
    {
        return pRom;  // Found a Sonic 3 ROM
    }

    // Not Sonic 3
    delete pRom;

    // ROM not supported
    throw std::runtime_error("Could not load selected ROM");
}
