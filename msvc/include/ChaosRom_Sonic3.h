#ifndef __CHAOS_ROM_SONIC3_H
#define __CHAOS_ROM_SONIC3_H

/******************************************************************************
 *
 * ROM wrapper for Sonic The Hedgehog 3 (Megadrive)
 *
 * Specifies common data locations and implements the ChaosRom interface
 * necessary to instantiate level and sprite editors.
 *
 *****************************************************************************/

class ChaosRom_Sonic3: public ChaosRom
{
public:
    explicit ChaosRom_Sonic3(std::fstream& rom);
    virtual ~ChaosRom_Sonic3();

/******************************************************************************
 *
 * Implementation of ChaosRom abstract interface
 *
 *****************************************************************************/

    virtual bool validateROM();
    virtual LevelNames_t getLevelNames();

/******************************************************************************
 *
 * Data layout utility methods
 *
 *****************************************************************************/

    virtual uint32_t getPalettesAddress(unsigned int level_index);
    virtual uint32_t getPatternsAddress(unsigned int level_index);
    virtual uint32_t getPatternsAddress_extended(unsigned int level_index);
    virtual uint32_t getChunksAddress(unsigned int level_index);
    virtual uint32_t getChunksAddress_extended(unsigned int level_index);
    virtual uint32_t getBlocksAddress(unsigned int level_index);
    virtual uint32_t getBlocksAddress_extended(unsigned int level_index);
    virtual uint32_t getMapAddress(unsigned int level_index);

protected:

    virtual Instance* instantiateLevel();
    virtual Instance* instantiateSprites();

    virtual uint32_t getDataAddress(unsigned int level_index, std::streamoff entry_offset);
    virtual uint32_t getPaletteIndex(unsigned int level_index);

private:

    uint32_t m_level_layout_dir_address;       // Default level layout directory address
    uint32_t m_level_select_index;             // Level select order
    uint32_t m_level_data_dir;                 // Level data pointers (patterns, chunks, blocks)
    uint32_t m_level_data_dir_entry_size;      // Each pointer is 4 bytes, total of 3 pointers
    uint32_t m_level_palette_dir;	           // Directory of palette pointers
};

#endif