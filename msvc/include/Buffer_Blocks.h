#pragma once

class Buffer_Patterns;
class Level;
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
    Buffer_Blocks(
        const Level* level,
        const SonicBlock** blocks, unsigned int block_count,
        const SonicChunk* chunks, unsigned int chunk_Count,
        const Buffer_Patterns*, HDC hdc);

    void drawBlock(unsigned int block_index, HDC dest, int x, int y) const;

    void refresh(const Level* level,
        const SonicBlock** blocks, unsigned int block_count,
        const SonicChunk* chunks, unsigned int chunk_count, const Buffer_Patterns*, HDC hdc);

private:
    void renderChunkIntoBuffer(HDC hdc, int x, int y, const SonicChunk& chunk,
        bool h_flip, bool v_flip,
        const Buffer_Patterns* pattern_buffer) const;

    void renderBlockIntoBuffer(const SonicBlock& block, HDC hdc, int x, int y,
        const SonicChunk* chunks, unsigned int chunk_count,
        const Buffer_Patterns* pattern_buffer) const;

    unsigned int m_block_width;
};
