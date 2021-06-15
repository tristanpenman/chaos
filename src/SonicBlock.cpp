///////////////////////////////////////////////////////////////////////////////
//
// SonicBlock.cpp
//
///////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"

#include "ChaosException.h"

#include "SegaPalette.h"
#include "SegaPattern.h"

#include "SonicChunk.h"
#include "SonicBlock.h"


///////////////////////////////////////////////////////////////////////////////
//
// SonicBlock class implementation
//
///////////////////////////////////////////////////////////////////////////////

SonicBlock::SonicBlock(unsigned int w, unsigned int h)
    : m_chunkDescriptors(NULL)
    , m_chunksAcross(w / SonicChunk::getChunkWidth())
    , m_chunksDown(h / SonicChunk::getChunkHeight())
    , m_chunkCount(m_chunksAcross * m_chunksDown)
{
    m_chunkDescriptors = new SonicChunkDescriptor[m_chunkCount];

    memset(m_chunkDescriptors, 0, sizeof(SonicChunkDescriptor) * m_chunkCount);
}

SonicBlock::~SonicBlock()
{
    if (m_chunkDescriptors)
    {
        delete[] m_chunkDescriptors;
        m_chunkDescriptors = NULL;
    }
}

bool SonicBlock::loadFromBuffer(unsigned char *buffer)
{
    unsigned short index = 0;
    unsigned int bufferPos = 0;

    for (unsigned int chunkIndex = 0; chunkIndex < m_chunkCount; chunkIndex++)
    {
        // Build index
        index  = (buffer[bufferPos] << 8) & 0xFF00;
        index |= (buffer[bufferPos + 1])      & 0x00FF;

        bufferPos += BYTES_PER_CHUNK;

        m_chunkDescriptors[chunkIndex].set(index);
    }

    return true;
}

const SonicChunkDescriptor& SonicBlock::getChunkDescriptor(unsigned int x, unsigned int y) const
{
    if (x >= m_chunksAcross || y >= m_chunksAcross)
    {
        throw ChaosException("Invalid chunk index");
    }

    return m_chunkDescriptors[y * m_chunksAcross + x];
}

unsigned int SonicBlock::calculateBlockSize(unsigned int w, unsigned int h)
{
    const unsigned int chunkCount =
            w / SonicChunk::getChunkWidth() *
            h / SonicChunk::getChunkHeight();

    return chunkCount * BYTES_PER_CHUNK;
}

