#include "Precompiled.h"

#include "SegaRom.h"

#include "ChaosRom.h"
#include "ChaosRom_Sonic2.h"

#include "Instance.h"
#include "Instance_Level.h"
#include "Instance_Level_S2G.h"

using namespace std;

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

ChaosRom_Sonic2::ChaosRom_Sonic2(fstream& f)
  : ChaosRom(f)
  , m_level_layout_dir_address_ptr(0xE46E)    // Pointer to directory of layout pointers
  , m_level_layout_dir_address(0x45A80)       // Layout pointers are found here
  , m_level_select_index(0x9454)              // Level select order
  , m_level_data_dir(0x42594)                 // Level data pointers (patterns, chunks, blocks)
  , m_level_data_dir_entry_size(12)           // Each pointer is 4 bytes, total of 3 pointers
  , m_level_palette_dir(0x2782)               // Directory of palette pointers
{

}

bool ChaosRom_Sonic2::validateROM()
{
    const string name = getDomesticName();

    return ((name.find("SONIC THE") != name.npos) && (name.find("HEDGEHOG 2") != name.npos));
}

ChaosRom::LevelNames_t ChaosRom_Sonic2::getLevelNames()
{
    //
    // TODO: Level names and locations could be read from the ROM, to be more 'pure'.
    //

    LevelNames_t levelNames;

    levelNames[0]  = "Emerald Hill Zone - Act 1";
    levelNames[1]  = "Emerald Hill Zone - Act 2";
    levelNames[2]  = "Chemical Plant Zone - Act 1";
    levelNames[3]  = "Chemical Plant Zone - Act 2";
    levelNames[4]  = "Aquatic Ruins Zone - Act 1";
    levelNames[5]  = "Aquatic Ruins Zone - Act 2";
    levelNames[6]  = "Casino Night Zone - Act 1";
    levelNames[7]  = "Casino Night Zone - Act 1";
    levelNames[8]  = "Hill Top Zone - Act 1";
    levelNames[9]  = "Hill Top Zone - Act 2";
    levelNames[10]  = "Mystic Cave Zone - Act 1";
    levelNames[11]  = "Mystic Zone - Act 2";
    levelNames[12] = "Oil Ocean Zone - Act 1";
    levelNames[13] = "Oil Ocean Zone - Act 2";
    levelNames[14] = "Metropolis Zone - Act 1";
    levelNames[15] = "Metropolis Zone - Act 2";
    levelNames[16] = "Metropolis Zone - Act 3";
    levelNames[17] = "Sky Chase Zone - Act 1";
    levelNames[18] = "Wing Fortress Zone - Act 1";
    levelNames[19] = "Death Egg Zone - Act 1";

    return levelNames;
}

Instance* ChaosRom_Sonic2::instantiateLevel()
{
    return new Instance_Level_S2G(*this);
}

streamoff ChaosRom_Sonic2::getPaletteIndex(unsigned int level_index)
{
    return ((getDataAddress(level_index, 8) & 0xFF000000) >> 24);
}

streamoff ChaosRom_Sonic2::getPalettesAddress(unsigned int level_index)
{
    return readAddress_32bit_at(m_level_palette_dir + getPaletteIndex(level_index) * 8);
}

streamoff ChaosRom_Sonic2::getPatternsAddress(unsigned int level_index)
{
    return (getDataAddress(level_index, 0) & 0xFFFFFF);
}

streamoff ChaosRom_Sonic2::getChunksAddress(unsigned int level_index)
{
    return (getDataAddress(level_index, 4) & 0xFFFFFF);
}

streamoff ChaosRom_Sonic2::getBlocksAddress(unsigned int level_index)
{
    return (getDataAddress(level_index, 8) & 0xFFFFFF);
}

streamoff ChaosRom_Sonic2::getMapAddress(unsigned int level_index)
{
    m_file.seekg(m_level_select_index + level_index * 2);

    const unsigned char zone_index = m_file.get();
    const unsigned char act_index = m_file.get();

    // Read level address from directory
    const streamoff dir_addr = readAddress_32bit_at(m_level_layout_dir_address_ptr);
    const streamoff level_offset = readAddress_16bit_at(dir_addr + zone_index * 4 + act_index * 2);

    return dir_addr + level_offset;
}

streamoff ChaosRom_Sonic2::getDataAddress(unsigned int level_index, streamoff entry_offset)
{
    streamoff addr = 0;
    streamoff p = m_file.tellg();

    // Find the directory index for the specified level
    m_file.seekg(m_level_select_index + level_index * 2);
    int dir_index = m_file.get();

    addr = readAddress_32bit_at(m_level_data_dir + dir_index * m_level_data_dir_entry_size + entry_offset);

    m_file.seekg(p);

    return addr;
}
