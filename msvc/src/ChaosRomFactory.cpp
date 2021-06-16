#include "Precompiled.h"

#include "SegaRom.h"

#include "ChaosException.h"
#include "ChaosRom.h"
#include "ChaosRom_Sonic2.h"
#include "ChaosRom_Sonic3.h"
#include "ChaosRom_Sonic3k.h"
#include "ChaosRomFactory.h"

using namespace std;

ChaosRom* ChaosRomFactory::loadROM(fstream& rom)
{
	ChaosRom* pRom = NULL;

	//
	// Check for Sonic 2 ROM
	//
	pRom = new ChaosRom_Sonic2(rom);
	if (pRom->validateROM())
	{
		return pRom;	// Found a Sonic2 ROM
	}
	else
	{
		delete pRom;	// Not Sonic 2
	}

	//
	// Check for Sonic 3 ROM
	//
	pRom = new ChaosRom_Sonic3(rom);
	if (pRom->validateROM())
	{
		return pRom;	// Found a Sonic 3 ROM
	}
	else
	{
		delete pRom;	// Not Sonic 3
	}

	//
	// Check for Sonic 3 and Knuckles Lock-On ROM
	//
	/*pRom = new ChaosRom_Sonic3k(rom);
	if (pRom->validateROM())
	{
		return pRom;	// Found a Sonic 3 ROM
	}
	else
	{
		delete pRom;	// Not Sonic 3
	}*/

	// ROM not supported
	throw ChaosException("Could not load selected ROM");
}
