#pragma once

class SegaPalette;
class SegaPattern;

class SonicChunk;
class SonicBlock;
class SonicMap;

class Buffer_Blocks;
class Buffer_Patterns;

class Instance_Level : public Instance
{
public:
    virtual bool hasUnsavedChanges() const = 0;
    virtual bool saveChanges() = 0;

    virtual bool loadLevel(unsigned int level_index) = 0;

    virtual const SegaPalette& getPalette(unsigned int index) const = 0;
    virtual const SegaPalette* getPalettes() const = 0;
    virtual unsigned int getPaletteCount() const = 0;

    virtual const SegaPattern& getPattern(unsigned int index) const = 0;
    virtual const SegaPattern* getPatterns() const = 0;
    virtual unsigned int getPatternCount() const = 0;

    virtual const SonicBlock& getBlock(unsigned int index) const = 0;
    virtual const SonicBlock** getBlocks() const = 0;
    virtual unsigned int getBlockCount() const = 0;
    virtual unsigned int getBlockHeight() const = 0;
    virtual unsigned int getBlockWidth() const = 0;

    virtual const SonicMap& getMap() const = 0;

    virtual const Buffer_Patterns& getPatternBuffer() const = 0;
    virtual const Buffer_Blocks& getBlockBuffer() const = 0;
};
