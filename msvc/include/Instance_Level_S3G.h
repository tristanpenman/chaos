#ifndef __INSTANCE_LEVEL_S3G_H
#define __INSTANCE_LEVEL_S3G_H

class ChaosRom_Sonic3;

/******************************************************************************
 *
 * Implementation of the Instance_Level abstract interface for roms based on 
 * the third generation engine used in Sonic 3 and Sonic & Knuckles.
 *
 *****************************************************************************/

class Instance_Level_S3G : public Instance_Level
{
public:

	explicit Instance_Level_S3G(ChaosRom_Sonic3& rom);

	virtual ~Instance_Level_S3G();

/******************************************************************************
 *
 * Implementation of the Instance interface
 *
 *****************************************************************************/

	virtual bool hasUnsavedChanges() const;
	virtual bool saveChanges();

/******************************************************************************
 *
 * Implementation of the Instance_Level interface
 *
 *****************************************************************************/

	virtual bool                   loadLevel(unsigned int level_index);

	virtual const SegaPalette&     getPalette(unsigned int index) const;
	virtual const SegaPalette*     getPalettes() const;
	virtual unsigned int           getPaletteCount() const;
	
	virtual const SegaPattern&     getPattern(unsigned int index) const;
	virtual const SegaPattern*     getPatterns() const;
	virtual unsigned int           getPatternCount() const;

	virtual const SonicBlock&      getBlock(unsigned int index) const;
	virtual const SonicBlock**     getBlocks() const;
	virtual unsigned int           getBlockCount() const;
	virtual unsigned int           getBlockHeight() const;
	virtual unsigned int           getBlockWidth() const;

	virtual const SonicMap&        getMap() const;

#ifdef WIN32

	virtual const Buffer_Blocks&   getBlockBuffer() const;
	virtual const Buffer_Patterns& getPatternBuffer() const;

#endif

/******************************************************************************
 *
 * Extension of the Instance_Level interface
 *
 *****************************************************************************/

	const SonicChunk&              getChunk(unsigned int index) const;
	const SonicChunk*              getChunks() const;
	unsigned int                   getChunkCount() const;

protected:

/******************************************************************************
 *
 * Loader methods
 *
 *****************************************************************************/

	bool loadPalettes();
	bool loadPatterns();
	bool loadChunks();
	bool loadBlocks();
	bool loadMap();



/******************************************************************************
 *
 * Cleanup methods
 *
 *****************************************************************************/

	void cleanup();

	void unloadPalettes();
	void unloadPatterns();
	void unloadChunks();
	void unloadBlocks();
	void unloadMap();	

/******************************************************************************
 *
 * Windows-specific buffer methods
 *
 *****************************************************************************/

#ifdef WIN32

	bool bufferPatterns();
	bool bufferBlocks();

	void unloadPatternBuffer();
	void unloadBlockBuffer();

#endif

private:

/******************************************************************************
 *
 * Private variables
 *
 *****************************************************************************/

	ChaosRom_Sonic3& m_rom;

	unsigned int     m_level_index;
	bool             m_level_loaded;

	SegaPalette*     m_palettes;
	SegaPattern*     m_patterns;
	SonicChunk*      m_chunks;
	SonicBlock**     m_blockPtrs;

	SonicMap*        m_pMap;

	unsigned int     m_palette_count;
	unsigned int     m_pattern_count;
	unsigned int     m_chunk_count;
	unsigned int     m_blockCount;

	Buffer_Patterns* m_pBufferPatterns;
	Buffer_Blocks*   m_pBufferBlocks;

};

#endif
