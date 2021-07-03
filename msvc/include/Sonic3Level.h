#pragma once

#include "Level.h"

class Sonic3Rom;

class Sonic3Level : public Level
{
public:
    explicit Sonic3Level(Sonic3Rom& rom);

    virtual ~Sonic3Level();

    virtual bool loadLevel(unsigned int level_index);

    virtual const Palette& getPalette(unsigned int index) const;
    virtual const Palette* getPalettes() const;
    virtual unsigned int getPaletteCount() const;

    virtual const Pattern& getPattern(unsigned int index) const;
    virtual const Pattern* getPatterns() const;
    virtual unsigned int getPatternCount() const;

    virtual const Block& getBlock(unsigned int index) const;
    virtual const Block** getBlocks() const;
    virtual unsigned int getBlockCount() const;
    virtual unsigned int getBlockHeight() const;
    virtual unsigned int getBlockWidth() const;

    virtual const Map& getMap() const;

    virtual const BlockBuffer& getBlockBuffer() const;
    virtual const PatternBuffer& getPatternBuffer() const;

    const Chunk& getChunk(unsigned int index) const;
    const Chunk* getChunks() const;
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
    Sonic3Rom& m_rom;

    unsigned int m_level_index;
    bool         m_level_loaded;

    Palette* m_palettes;
    Pattern* m_patterns;
    Chunk*  m_chunks;
    Block** m_block_ptrs;

    Map*    m_map;

    unsigned int m_palette_count;
    unsigned int m_pattern_count;
    unsigned int m_chunk_count;
    unsigned int m_block_count;

    PatternBuffer* m_buffer_patterns;
    BlockBuffer*   m_buffer_blocks;
};
