#include "Precompiled.h"

#include "SegaPalette.h"
#include "SegaPattern.h"
#include "SonicChunk.h"
#include "SonicChunkDescriptor.h"
#include "SonicBlock.h"
#include "Buffer.h"
#include "Buffer_Patterns.h"
#include "Buffer_Blocks.h"
#include "Level.h"

Buffer_Blocks::Buffer_Blocks(
    const Level* level,
    const SonicBlock** blocks_array, unsigned int block_count,
    const SonicChunk* chunks_array, unsigned int chunk_count,
    const Buffer_Patterns* pattern_buffer, HDC hdc)
{
    refresh(level, blocks_array, block_count, chunks_array, chunk_count, pattern_buffer, hdc);
}

void Buffer_Blocks::drawBlock(unsigned int block_index, HDC dest, int x, int y) const
{
    BitBlt(dest, x, y, m_block_width, m_block_width, m_hdc, block_index * m_block_width, 0, SRCCOPY);
}

void Buffer_Blocks::renderChunkIntoBuffer(HDC hdc, int x, int y, const SonicChunk& chunk, bool h_flip, bool v_flip,
    const Buffer_Patterns* pattern_buffer) const
{
    const unsigned int pattern_width = SegaPattern::getPatternWidth();
    const unsigned int pattern_height = SegaPattern::getPatternHeight();

    const int base_x = x + (h_flip ? pattern_width : 0);

    int px = 0;
    int py = y + (v_flip ? pattern_height : 0);

    for (int pat_y = 0; pat_y < 2; pat_y++)
    {
        px = base_x;

        for (int pat_x = 0; pat_x < 2; pat_x++)
        {
            pattern_buffer->drawPattern(
                chunk.getPatternDescriptor(pat_x, pat_y),
                hdc,
                px,
                py,
                h_flip,
                v_flip);

            px = h_flip ? px - pattern_width : px + pattern_width;
        }

        py = v_flip ? py - pattern_height : py + pattern_height;
    }
}

void Buffer_Blocks::renderBlockIntoBuffer(const SonicBlock& block, HDC hdc, int x, int y,
    const SonicChunk* chunks_array, unsigned int chunk_count,
    const Buffer_Patterns* pattern_buffer) const
{
    const unsigned int chunk_width = SonicChunk::getChunkWidth();
    const unsigned int chunk_height = SonicChunk::getChunkHeight();

    for (unsigned int chunk_y = 0; chunk_y < 8; chunk_y++)
    {
        for (unsigned int chunk_x = 0; chunk_x < 8; chunk_x++)
        {
            const SonicChunkDescriptor& desc = block.getChunkDescriptor(chunk_x, chunk_y);
            const SonicChunk& chunk = chunks_array[desc.getChunkIndex()];

            renderChunkIntoBuffer(hdc,
                    x + chunk_x * chunk_width,
                    y + chunk_y * chunk_height,
                    chunk,
                    desc.getHFlip(),
                    desc.getVFlip(),
                    pattern_buffer);
        }
    }
}

void Buffer_Blocks::refresh(
    const Level* level,
    const SonicBlock** blocks_array, unsigned int block_count,
    const SonicChunk* chunks_array, unsigned int chunk_count, const Buffer_Patterns* pattern_buffer, HDC hdc)
{
    m_block_width = level->getBlockWidth();

    reset(hdc, block_count * m_block_width, m_block_width);
    SelectObject(m_hdc, m_hbitmap);

    for (unsigned int blockIndex = 0; blockIndex < block_count; blockIndex++)
    {
        renderBlockIntoBuffer(*blocks_array[blockIndex], m_hdc, blockIndex * m_block_width, 0,
            chunks_array, chunk_count, pattern_buffer);
    }
}
