#include "Precompiled.h"

#include "ChaosException.h"

#include "SegaPalette.h"
#include "SegaPattern.h"

#include "SonicChunk.h"

SonicChunk::SonicChunk()
{
    memset(m_pattern_indices, 0, sizeof(SegaPatternDescriptor) * PATTERNS_PER_CHUNK);
}

SonicChunk::~SonicChunk()
{

}

void SonicChunk::loadFromBuffer(unsigned char buffer[CHUNK_SIZE_IN_ROM])
{
    unsigned short index = 0;

    for (unsigned int i = 0; i < PATTERNS_PER_CHUNK; i++)
    {
        // Build index
        index  = (buffer[0] << 8) & 0xFF00;
        index |= (buffer[1])      & 0x00FF;

        // Set index
        m_pattern_indices[i].set(index);

        // Next...
        buffer += SegaPatternDescriptor::getIndexSize();
    }
}

const SegaPatternDescriptor& SonicChunk::getPatternDescriptor(unsigned int x, unsigned int y) const
{
    if (x > 1 || y > 1)
    {
        throw ChaosException("Invalid pattern index");
    }

    return m_pattern_indices[y * 2 + x];
}
