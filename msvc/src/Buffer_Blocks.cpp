#include "Precompiled.h"

#include "ChaosException.h"
#include "SegaPalette.h"
#include "SegaPattern.h"
#include "SonicChunk.h"
#include "SonicChunkDescriptor.h"
#include "SonicBlock.h"
#include "Buffer.h"
#include "Buffer_Patterns.h"
#include "Buffer_Blocks.h"
#include "Instance.h"
#include "Instance_Level.h"

/******************************************************************************
 *
 * Constructors and destructor methods
 *
 *****************************************************************************/

Buffer_Blocks::Buffer_Blocks(
    const Instance_Level* level,
    const SonicBlock** blocksArray, unsigned int blockCount,
    const SonicChunk* chunksArray, unsigned int chunkCount,
    const Buffer_Patterns* pPatternBuffer, HDC hdc)
{
    refresh(level, blocksArray, blockCount, chunksArray, chunkCount, pPatternBuffer, hdc);
}

/******************************************************************************
 *
 * Public interface
 *
 *****************************************************************************/

void Buffer_Blocks::drawBlock(unsigned int blockIndex, HDC dest, int x, int y) const
{
    BitBlt(dest, x, y, m_blockWidth, m_blockWidth, m_hDC, blockIndex * m_blockWidth, 0, SRCCOPY);
}


/******************************************************************************
 *
 * Private interface
 *
 *****************************************************************************/

void Buffer_Blocks::renderChunkIntoBuffer(HDC hdc, int x, int y, const SonicChunk& chunk, bool h_flip, bool v_flip,
    const Buffer_Patterns* pPatternBuffer) const
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
            pPatternBuffer->drawPattern(
                chunk.getPatternDescriptor(pat_x, pat_y), hdc,
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
    const SonicChunk* chunksArray, unsigned int chunkCount,
    const Buffer_Patterns* pPatternBuffer) const
{
    const unsigned int chunkWidth = SonicChunk::getChunkWidth();
    const unsigned int chunkHeight = SonicChunk::getChunkHeight();

    for (unsigned int chunk_y = 0; chunk_y < 8; chunk_y++)
    {
        for (unsigned int chunk_x = 0; chunk_x < 8; chunk_x++)
        {
            const SonicChunkDescriptor& desc = block.getChunkDescriptor(chunk_x, chunk_y);
            const SonicChunk& chunk = chunksArray[desc.getChunkIndex()];

            renderChunkIntoBuffer(hdc,
                    x + chunk_x * chunkWidth,
                    y + chunk_y * chunkHeight,
                    chunk,
                    desc.getFlag_HFlip(),
                    desc.getFlag_VFlip(),
                    pPatternBuffer);
        }
    }
}

void Buffer_Blocks::refresh(
    const Instance_Level* level,
    const SonicBlock** blocksArray, unsigned int blockCount,
    const SonicChunk* chunksArray, unsigned int chunkCount, const Buffer_Patterns* pPatternBuffer, HDC hdc)
{
    m_blockWidth = level->getBlockWidth();

    reset(hdc, blockCount * m_blockWidth, m_blockWidth);
    SelectObject(m_hDC, m_hBitmap);

    for (unsigned int blockIndex = 0; blockIndex < blockCount; blockIndex++)
    {
        renderBlockIntoBuffer(*blocksArray[blockIndex], m_hDC, blockIndex * m_blockWidth, 0,
            chunksArray, chunkCount, pPatternBuffer);
    }
}
