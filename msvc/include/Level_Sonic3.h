#pragma once

#include "Level.h"

class ChaosRom_Sonic3;

class Level_Sonic3 : public Level
{
public:
    explicit Level_Sonic3(ChaosRom_Sonic3& rom);
    virtual ~Level_Sonic3();

    virtual bool loadLevel(unsigned int level_index);

    virtual const SegaPalette& getPalette(unsigned int index) const;
    virtual const SegaPalette* getPalettes() const;
    virtual unsigned int getPaletteCount() const;

    virtual const SegaPattern& getPattern(unsigned int index) const;
    virtual const SegaPattern* getPatterns() const;
    virtual unsigned int getPatternCount() const;

    virtual const SonicBlock& getBlock(unsigned int index) const;
    virtual const SonicBlock** getBlocks() const;
    virtual unsigned int getBlockCount() const;
    virtual unsigned int getBlockHeight() const;
    virtual unsigned int getBlockWidth() const;

    virtual const SonicMap& getMap() const;

    virtual const Buffer_Blocks& getBlockBuffer() const;
    virtual const Buffer_Patterns& getPatternBuffer() const;

    const SonicChunk& getChunk(unsigned int index) const;
    const SonicChunk* getChunks() const;
    unsigned int getChunkCount() const;

protected:
    bool loadPalettes();
    bool loadPatterns();
    bool loadChunks();
    bool loadBlocks();
    bool loadMap();

    bool bufferPatterns();
    bool bufferBlocks();

    void cleanup();

    void unloadPalettes();
    void unloadPatterns();
    void unloadChunks();
    void unloadBlocks();
    void unloadMap();

    void unloadPatternBuffer();
    void unloadBlockBuffer();

private:
    ChaosRom_Sonic3& m_rom;

    unsigned int m_level_index;
    bool         m_level_loaded;

    SegaPalette* m_palettes;
    SegaPattern* m_patterns;
    SonicChunk*  m_chunks;
    SonicBlock** m_block_ptrs;

    SonicMap*    m_map;

    unsigned int m_palette_count;
    unsigned int m_pattern_count;
    unsigned int m_chunk_count;
    unsigned int m_block_count;

    Buffer_Patterns* m_buffer_patterns;
    Buffer_Blocks*   m_buffer_blocks;
};
