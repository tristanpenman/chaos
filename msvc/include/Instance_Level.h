#ifndef __INSTANCE_LEVEL_H
#define __INSTANCE_LEVEL_H

class SegaPalette;
class SegaPattern;

class SonicChunk;
class SonicBlock;
class SonicMap;

class Buffer_Blocks;
class Buffer_Patterns;

/******************************************************************************
 *
 * Instance_Level abstract class
 *
 *****************************************************************************/

class Instance_Level : public Instance
{
public:

/******************************************************************************
 *
 * Public abstract interface for Instance
 *
 *****************************************************************************/

	virtual bool hasUnsavedChanges() const = 0;
	virtual bool saveChanges() = 0;

/******************************************************************************
 *
 * Public abstract interface for Instance_Level
 *
 *****************************************************************************/

	virtual bool                   loadLevel(unsigned int level_index) = 0;

	virtual const SegaPalette&     getPalette(unsigned int index) const = 0;
	virtual const SegaPalette*     getPalettes() const = 0;
	virtual unsigned int           getPaletteCount() const = 0;
	
	virtual const SegaPattern&     getPattern(unsigned int index) const = 0;
	virtual const SegaPattern*     getPatterns() const = 0;
	virtual unsigned int           getPatternCount() const = 0;

	virtual const SonicBlock&      getBlock(unsigned int index) const = 0;
	virtual const SonicBlock**     getBlocks() const = 0;
	virtual unsigned int           getBlockCount() const = 0;
	virtual unsigned int           getBlockHeight() const = 0;
	virtual unsigned int           getBlockWidth() const = 0;

	virtual const SonicMap&        getMap() const = 0;

#ifdef WIN32	// Buffers are used to accelerate drawing routines in Windows

	virtual const Buffer_Patterns& getPatternBuffer() const = 0;
	virtual const Buffer_Blocks&   getBlockBuffer() const = 0;

#endif

};

#endif
