#pragma once

#include <iosfwd>

class Rom;

class RomFactory
{
public:
    static Rom* loadROM(std::fstream& rom);
};
