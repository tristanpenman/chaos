#include "Precompiled.h"

#include "ChaosRom.h"
#include "ChaosRom_Sonic3.h"
#include "Level_Sonic3.h"

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF	// 64kB

using namespace std;

ChaosRom_Sonic3::ChaosRom_Sonic3(fstream& rom)
  : ChaosRom(rom)
  , m_level_layout_dir_address(0x81360)       // Layout pointers are found here
  , m_level_select_index(0x6A8E)              // Level select order
  , m_level_data_dir(0x5AB0C)                 // Level data pointers (patterns, chunks, blocks)
  , m_level_data_dir_entry_size(24)           // Each pointer is 4 bytestotal of 3 pointers
  , m_level_palette_dir(0x8BF54)              // Directory of palette pointers
{

}

bool ChaosRom_Sonic3::validateROM()
{
    const string name = getDomesticName();

    return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 3") != name.npos;
}

ChaosRom::LevelNames_t ChaosRom_Sonic3::getLevelNames()
{
    //
    // TODO: Level names and locations could be read from the ROM, to be more 'pure'.
    //

    LevelNames_t levelNames;

    levelNames[0]  = "Angel Island Zone - Act 1";
    levelNames[1]  = "Angel Island Zone - Act 2";
    levelNames[2]  = "Hydrocity Zone - Act 1";
    levelNames[3]  = "Hydrocity Zone - Act 2";
    levelNames[4]  = "Marble Garden Zone - Act 1";
    levelNames[5]  = "Marble Garden Zone - Act 2";
    levelNames[6]  = "Carnival Night Zone - Act 1";
    levelNames[7]  = "Carnival Night Zone - Act 1";
    levelNames[10]  = "Ice Cap Zone - Act 1";
    levelNames[11]  = "Ice Cap Zone - Act 2";
    levelNames[12] = "Launch Base Zone - Act 1";
    levelNames[13] = "Launch Base Zone - Act 2";

    return levelNames;
}

Level* ChaosRom_Sonic3::instantiateLevel()
{
    return new Level_Sonic3(*this);
}

uint32_t ChaosRom_Sonic3::getPalettesAddress(unsigned int level_index)
{
    return read32BitAddr(m_level_palette_dir + getPaletteIndex(level_index) * 8);
}

uint32_t ChaosRom_Sonic3::getPaletteIndex(unsigned int level_index)
{
    return ((getDataAddress(level_index, 8) & 0xFF000000) >> 24);
}

uint32_t ChaosRom_Sonic3::getPatternsAddress(unsigned int level_index)
{
    return (getDataAddress(level_index, 0) & 0xFFFFFF);
}

uint32_t ChaosRom_Sonic3::getPatternsAddress_extended(unsigned int level_index)
{
    return (getDataAddress(level_index, 4) & 0xFFFFFF);
}

uint32_t ChaosRom_Sonic3::getChunksAddress(unsigned int level_index)
{
    return (getDataAddress(level_index, 8) & 0xFFFFFF);
}

uint32_t ChaosRom_Sonic3::getChunksAddress_extended(unsigned int level_index)
{
    return (getDataAddress(level_index, 12) & 0xFFFFFF);
}

uint32_t ChaosRom_Sonic3::getBlocksAddress(unsigned int level_index)
{
    return (getDataAddress(level_index, 16) & 0xFFFFFF);
}

uint32_t ChaosRom_Sonic3::getBlocksAddress_extended(unsigned int level_index)
{
    return (getDataAddress(level_index, 20) & 0xFFFFFF);
}

uint32_t ChaosRom_Sonic3::getMapAddress(unsigned int level_index)
{
    return read32BitAddr(m_level_layout_dir_address + level_index * 4);
}

uint32_t ChaosRom_Sonic3::getDataAddress(unsigned int level_index, streamoff entry_offset)
{
    uint32_t addr = 0;
    streamoff p = m_file.tellg();

    // Find the directory index for the specified level
    m_file.seekg(m_level_select_index + level_index * 2);
    int zone_index = m_file.get();
    int act_index = m_file.get();

    addr = read32BitAddr(m_level_data_dir + zone_index * 2 * m_level_data_dir_entry_size + act_index * m_level_data_dir_entry_size + entry_offset);

    m_file.seekg(p);

    return addr;
}
