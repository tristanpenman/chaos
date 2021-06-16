#pragma once

class Buffer_Patterns;
class Instance_Level;
class SonicChunk;
class SonicBlock;

/**
 * Blocks image buffer class
 *
 * This class is pretty raw, and doesn't provide much protection. Block indices
 * should be checked against sane values to ensure that BitBlts don't copy data
 * from uninitialised locations in memory.
 */
class Buffer_Blocks : public Buffer
{
public:
    explicit Buffer_Blocks(const Instance_Level* level,
        const SonicBlock** blocks, unsigned int blockCount,
        const SonicChunk* chunks, unsigned int chunkCount,
        const Buffer_Patterns*, HDC hdc);

    void drawBlock(unsigned int blockIndex, HDC dest, int x, int y) const;

    void refresh(const Instance_Level* level,
        const SonicBlock** blocks, unsigned int blockCount,
        const SonicChunk* chunks, unsigned int chunkCount, const Buffer_Patterns*, HDC hdc);

private:
    void renderChunkIntoBuffer(HDC hdc, int x, int y, const SonicChunk& chunk,
        bool h_flip, bool v_flip,
        const Buffer_Patterns* pPatternBuffer) const;

    void renderBlockIntoBuffer(const SonicBlock& block, HDC hdc, int x, int y,
        const SonicChunk* chunks, unsigned int chunkCount,
        const Buffer_Patterns* pPatternBuffer) const;

    unsigned int m_blockWidth;
};
