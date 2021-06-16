#pragma once

class ChaosRom;

class ChaosRomFactory
{
public:
    static ChaosRom* loadROM(std::fstream& rom);
};
