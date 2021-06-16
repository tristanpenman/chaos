#ifndef __CHAOS_ROM_FACTORY_H
#define __CHAOS_ROM_FACTORY_H

class ChaosRom;

class ChaosRomFactory
{
public:
    static ChaosRom* loadROM(std::fstream& rom);
};

#endif
