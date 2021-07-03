#include "Precompiled.h"

#include "Rom.h"
#include "Sonic2Level.h"

#include "Sonic2Rom.h"

using namespace std;

#define PALETTE_COUNT 4

#define LEVEL_BUFFER_SIZE   0xFFFF
#define BLOCK_BUFFER_SIZE   0xFFFF
#define CHUNK_BUFFER_SIZE   0xFFFF
#define PATTERN_BUFFER_SIZE 0xFFFF  // 64kB

Sonic2Rom::Sonic2Rom(fstream& f)
  : Rom(f)
  , m_level_layout_dir_address_ptr(0xE46E)    // Pointer to directory of layout pointers
  , m_level_select_index(0x9454)              // Level select order
  , m_level_data_dir(0x42594)                 // Level data pointers (patterns, chunks, blocks)
  , m_level_data_dir_entry_size(12)           // Each pointer is 4 bytes, total of 3 pointers
  , m_level_palette_dir(0x2782)               // Directory of palette pointers
{

}

bool Sonic2Rom::validateROM()
{
    const string name = getDomesticName();

    return name.find("SONIC THE") != name.npos && name.find("HEDGEHOG 2") != name.npos;
}

Rom::LevelNames_t Sonic2Rom::getLevelNames()
{
    //
    // TODO: Level names and locations could be read from the ROM, to be more 'pure'.
    //

    LevelNames_t level_names;

    level_names[0]  = "Emerald Hill Zone - Act 1";
    level_names[1]  = "Emerald Hill Zone - Act 2";
    level_names[2]  = "Chemical Plant Zone - Act 1";
    level_names[3]  = "Chemical Plant Zone - Act 2";
    level_names[4]  = "Aquatic Ruins Zone - Act 1";
    level_names[5]  = "Aquatic Ruins Zone - Act 2";
    level_names[6]  = "Casino Night Zone - Act 1";
    level_names[7]  = "Casino Night Zone - Act 1";
    level_names[8]  = "Hill Top Zone - Act 1";
    level_names[9]  = "Hill Top Zone - Act 2";
    level_names[10] = "Mystic Cave Zone - Act 1";
    level_names[11] = "Mystic Zone - Act 2";
    level_names[12] = "Oil Ocean Zone - Act 1";
    level_names[13] = "Oil Ocean Zone - Act 2";
    level_names[14] = "Metropolis Zone - Act 1";
    level_names[15] = "Metropolis Zone - Act 2";
    level_names[16] = "Metropolis Zone - Act 3";
    level_names[17] = "Sky Chase Zone - Act 1";
    level_names[18] = "Wing Fortress Zone - Act 1";
    level_names[19] = "Death Egg Zone - Act 1";

    return level_names;
}

Level* Sonic2Rom::instantiateLevel()
{
    return new Sonic2Level(*this);
}

streamoff Sonic2Rom::getPaletteIndex(unsigned int level_index)
{
    const streamoff data_addr = getDataAddress(level_index, 8);

    return data_addr >> 24;
}

streamoff Sonic2Rom::getPalettesAddress(unsigned int level_index)
{
    const streamoff palette_index = getPaletteIndex(level_index);
    const streamoff palette_addr_loc = m_level_palette_dir + palette_index * 8;

    return read32BitAddr(palette_addr_loc);
}

streamoff Sonic2Rom::getPatternsAddress(unsigned int level_index)
{
    return getDataAddress(level_index, 0) & 0xFFFFFF;
}

streamoff Sonic2Rom::getChunksAddress(unsigned int level_index)
{
    return getDataAddress(level_index, 4) & 0xFFFFFF;
}

streamoff Sonic2Rom::getBlocksAddress(unsigned int level_index)
{
    return getDataAddress(level_index, 8) & 0xFFFFFF;
}

streamoff Sonic2Rom::getMapAddress(unsigned int level_index)
{
    m_file.seekg(m_level_select_index + level_index * 2);

    const unsigned char zone_index = m_file.get();
    const unsigned char act_index = m_file.get();

    const streamoff dir_addr_loc = m_level_layout_dir_address_ptr;
    const streamoff dir_addr = read32BitAddr(dir_addr_loc);

    const streamoff level_offset_loc = dir_addr + zone_index * 4 + act_index * 2;
    const streamoff level_offset = read16BitAddr(level_offset_loc);

    return dir_addr + level_offset;
}

streamoff Sonic2Rom::getDataAddress(unsigned int level_index, streamoff entry_offset)
{
    m_file.seekg(m_level_select_index + level_index * 2);

    const unsigned char dir_index = m_file.get();
    const streamoff data_addr_loc = m_level_data_dir +
        dir_index * m_level_data_dir_entry_size +
        entry_offset;

    return read32BitAddr(data_addr_loc);
}
