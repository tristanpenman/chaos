#include "Precompiled.h"

#include "Palette.h"
#include "Pattern.h"

#include "Chunk.h"

Chunk::Chunk()
{
    memset(m_pattern_indices, 0, sizeof(PatternDesc) * PATTERNS_PER_CHUNK);
}

void Chunk::loadFromBuffer(unsigned char buffer[CHUNK_SIZE_IN_ROM])
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
        buffer += PatternDesc::getIndexSize();
    }
}

const PatternDesc& Chunk::getPatternDescriptor(unsigned int x, unsigned int y) const
{
    if (x > 1 || y > 1)
    {
        throw std::runtime_error("Invalid pattern index");
    }

    return m_pattern_indices[y * 2 + x];
}
