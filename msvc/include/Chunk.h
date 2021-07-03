#pragma once

#include <cstddef>

#include "Pattern.h"
#include "PatternDesc.h"

#define PATTERNS_PER_CHUNK 4
#define BYTES_PER_PATTERN 2
#define CHUNK_SIZE_IN_ROM PATTERNS_PER_CHUNK * BYTES_PER_PATTERN

/**
 * Chunk class definition
 *
 * A Sonic chunk represents a 16x16 tile, composed of 4 8x8 SEGA patterns.
 * Pattern indices are defined using the standard SEGA pattern descriptor -
 * see Pattern.h for more details.
 */
class Chunk
{
public:
    Chunk();

    virtual ~Chunk() {}

    void loadFromBuffer(unsigned char buffer[CHUNK_SIZE_IN_ROM]);

    const PatternDesc& getPatternDescriptor(unsigned int x, unsigned int y) const;

    static unsigned int getChunkHeight();
    static unsigned int getChunkWidth();
    static unsigned int getChunkSize();

private:
    Chunk(const Chunk&) DELETED;
    Chunk& operator=(const Chunk&) DELETED;

    PatternDesc m_pattern_indices[PATTERNS_PER_CHUNK];
};

inline unsigned int Chunk::getChunkSize()
{
    return CHUNK_SIZE_IN_ROM;
}

inline unsigned int Chunk::getChunkHeight()
{
    return Pattern::getPatternHeight() * 2;
}

inline unsigned int Chunk::getChunkWidth()
{
    return Pattern::getPatternWidth() * 2;
}
