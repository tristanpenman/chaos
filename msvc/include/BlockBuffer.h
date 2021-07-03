#pragma once

#include "Buffer.h"

class Block;
class Chunk;
class Level;
class PatternBuffer;

/**
 * Blocks image buffer class
 *
 * This class is pretty raw, and doesn't provide much protection. Block indices
 * should be checked against sane values to ensure that BitBlts don't copy data
 * from uninitialised locations in memory.
 */
class BlockBuffer : public Buffer
{
public:
    BlockBuffer(
        const Level* level,
        const Block** blocks, unsigned int block_count,
        const Chunk* chunks, unsigned int chunk_Count,
        const PatternBuffer*, HDC hdc);

    void drawBlock(unsigned int block_index, HDC dest, int x, int y) const;

    void refresh(const Level* level,
        const Block** blocks, unsigned int block_count,
        const Chunk* chunks, unsigned int chunk_count, const PatternBuffer*, HDC hdc);

private:
    void renderChunkIntoBuffer(HDC hdc, int x, int y, const Chunk& chunk,
        bool h_flip, bool v_flip,
        const PatternBuffer* pattern_buffer) const;

    void renderBlockIntoBuffer(const Block& block, HDC hdc, int x, int y,
        const Chunk* chunks, unsigned int chunk_count,
        const PatternBuffer* pattern_buffer) const;

    unsigned int m_block_width;
};
