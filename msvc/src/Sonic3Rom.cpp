#include "Precompiled.h"

#include "Rom.h"
#include "Sonic3Level.h"

#include "Sonic3Rom.h"

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

using namespace std;

Sonic3Rom::Sonic3Rom(fstream& rom)
  : Rom(rom)
  , m_level_layout_dir_address(0x81360)       // Layout pointers are found here
  , m_level_select_index(0x6A8E)              // Level select order
  , m_level_data_dir(0x5AB0C)                 // Level data pointers (patterns, chunks, blocks)
  , m_level_data_dir_entry_size(24)           // Each pointer is 4 bytestotal of 3 pointers
  , m_level_palette_dir(0x8BF54)              // Directory of palette pointers
{

}

bool Sonic3Rom::validateROM()
{
    const string name = getDomesticName();

    return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 3") != name.npos;
}

Rom::LevelNames_t Sonic3Rom::getLevelNames()
{
    //
    // TODO: Level names and locations could be read from the ROM, to be more 'pure'.
    //

    LevelNames_t level_names;

    level_names[0]  = "Angel Island Zone - Act 1";
    level_names[1]  = "Angel Island Zone - Act 2";
    level_names[2]  = "Hydrocity Zone - Act 1";
    level_names[3]  = "Hydrocity Zone - Act 2";
    level_names[4]  = "Marble Garden Zone - Act 1";
    level_names[5]  = "Marble Garden Zone - Act 2";
    level_names[6]  = "Carnival Night Zone - Act 1";
    level_names[7]  = "Carnival Night Zone - Act 1";
    level_names[10] = "Ice Cap Zone - Act 1";
    level_names[11] = "Ice Cap Zone - Act 2";
    level_names[12] = "Launch Base Zone - Act 1";
    level_names[13] = "Launch Base Zone - Act 2";

    return level_names;
}

Level* Sonic3Rom::instantiateLevel()
{
    return new Sonic3Level(*this);
}

uint32_t Sonic3Rom::getPalettesAddress(unsigned int level_index)
{
    return read32BitAddr(m_level_palette_dir + getPaletteIndex(level_index) * 8);
}

uint32_t Sonic3Rom::getPaletteIndex(unsigned int level_index)
{
    return getDataAddress(level_index, 8) >> 24;
}

uint32_t Sonic3Rom::getPatternsAddress(unsigned int level_index)
{
    return getDataAddress(level_index, 0) & 0xFFFFFF;
}

uint32_t Sonic3Rom::getPatternsAddress_extended(unsigned int level_index)
{
    return getDataAddress(level_index, 4) & 0xFFFFFF;
}

uint32_t Sonic3Rom::getChunksAddress(unsigned int level_index)
{
    return getDataAddress(level_index, 8) & 0xFFFFFF;
}

uint32_t Sonic3Rom::getChunksAddress_extended(unsigned int level_index)
{
    return getDataAddress(level_index, 12) & 0xFFFFFF;
}

uint32_t Sonic3Rom::getBlocksAddress(unsigned int level_index)
{
    return getDataAddress(level_index, 16) & 0xFFFFFF;
}

uint32_t Sonic3Rom::getBlocksAddress_extended(unsigned int level_index)
{
    return getDataAddress(level_index, 20) & 0xFFFFFF;
}

uint32_t Sonic3Rom::getMapAddress(unsigned int level_index)
{
    return read32BitAddr(m_level_layout_dir_address + level_index * 4);
}

uint32_t Sonic3Rom::getDataAddress(unsigned int level_index, streamoff entry_offset)
{
    m_file.seekg(m_level_select_index + level_index * 2);

    const unsigned char zone_index = m_file.get();
    const unsigned char act_index = m_file.get();

    const streamoff data_addr_loc = m_level_data_dir +
        zone_index * m_level_data_dir_entry_size * 2 +
        act_index * m_level_data_dir_entry_size +
        entry_offset;

    return read32BitAddr(data_addr_loc);
}
