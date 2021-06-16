#ifndef __CHAOS_ROM_SONIC2_H
#define __CHAOS_ROM_SONIC2_H

/******************************************************************************
 *
 * ROM wrapper for Sonic The Hedgehog 2 (Megadrive)
 *
 * Specifies common data locations and implements the ChaosRom interface
 * necessary to instantiate level and sprite editors.
 *
 *****************************************************************************/

class ChaosRom_Sonic2: public ChaosRom 
{
public:
	explicit ChaosRom_Sonic2(std::fstream& rom);
	virtual ~ChaosRom_Sonic2();

/******************************************************************************
 *
 * Implementation of ChaosRom abstract interface
 *
 *****************************************************************************/

public:

	virtual bool validateROM();

	virtual LevelNames_t getLevelNames();

protected:

	virtual Instance* instantiateLevel();
	virtual Instance* instantiateSprites();

/******************************************************************************
 *
 * Data layout utility methods
 *
 *****************************************************************************/

public:

	virtual std::streamoff getPalettesAddress(unsigned int level_index);
	virtual std::streamoff getPatternsAddress(unsigned int level_index);
	virtual std::streamoff getChunksAddress(unsigned int level_index);
	virtual std::streamoff getBlocksAddress(unsigned int level_index);
	virtual std::streamoff getMapAddress(unsigned int level_index);

protected:

	virtual std::streamoff getPaletteIndex(unsigned int level_index);
	virtual std::streamoff getDataAddress(unsigned int level_index, std::streamoff entry_offset);

/******************************************************************************
 *
 * Data locations
 *
 *****************************************************************************/

private:

	std::streamoff m_level_layout_dir_address_ptr;   // Location of pointer to level layout dir address
	std::streamoff m_level_layout_dir_address;       // Default level layout directory address
	std::streamoff m_level_select_index;             // Level select order
	std::streamoff m_level_data_dir;                 // Level data pointers (patterns, chunks, blocks)
	std::streamoff m_level_data_dir_entry_size;      // Each pointer is 4 bytes, total of 3 pointers
	std::streamoff m_level_palette_dir;	             // Directory of palette pointers

};

#endif