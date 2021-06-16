#pragma once

class ChaosRom_Sonic2;

/**
 * Implementation of the Instance_Level abstract interface for ROMs based on
 * the Sonic The Hedgehog 2 engine.
 */
class Instance_Level_S2G : public Instance_Level
{
public:
    explicit Instance_Level_S2G(ChaosRom_Sonic2& rom);
    virtual ~Instance_Level_S2G();

    virtual bool hasUnsavedChanges() const;
    virtual bool saveChanges();

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
    ChaosRom_Sonic2& m_rom;

    unsigned int m_level_index;
    bool m_level_loaded;

    SegaPalette* m_palettes;
    SegaPattern* m_patterns;
    SonicChunk* m_chunks;
    SonicBlock** m_blockPtrs;

    SonicMap* m_pMap;

    unsigned int m_palette_count;
    unsigned int m_pattern_count;
    unsigned int m_chunk_count;
    unsigned int m_blockCount;

    Buffer_Patterns* m_pBufferPatterns;
    Buffer_Blocks* m_pBufferBlocks;
};
