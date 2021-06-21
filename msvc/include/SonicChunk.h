#pragma once

#include <cstddef>

#include "SegaPattern.h"
#include "SegaPatternDescriptor.h"

#define PATTERNS_PER_CHUNK 4
#define BYTES_PER_PATTERN 2
#define CHUNK_SIZE_IN_ROM PATTERNS_PER_CHUNK * BYTES_PER_PATTERN

/**
 * SonicChunk class definition
 *
 * A Sonic chunk represents a 16x16 tile, composed of 4 8x8 SEGA patterns.
 * Pattern indices are defined using the standard SEGA pattern descriptor -
 * see SegaPattern.h for more details.
 */
class SonicChunk
{
public:
    SonicChunk();

    virtual ~SonicChunk() {}

    void loadFromBuffer(unsigned char buffer[CHUNK_SIZE_IN_ROM]);

    const SegaPatternDescriptor& getPatternDescriptor(unsigned int x, unsigned int y) const;

    static unsigned int getChunkHeight();
    static unsigned int getChunkWidth();
    static unsigned int getChunkSize();

private:
    SonicChunk(const SonicChunk&) DELETED;
    SonicChunk& operator=(const SonicChunk&) DELETED;

    SegaPatternDescriptor m_pattern_indices[PATTERNS_PER_CHUNK];
};

inline unsigned int SonicChunk::getChunkSize()
{
    return CHUNK_SIZE_IN_ROM;
}

inline unsigned int SonicChunk::getChunkHeight()
{
    return SegaPattern::getPatternHeight() * 2;
}

inline unsigned int SonicChunk::getChunkWidth()
{
    return SegaPattern::getPatternWidth() * 2;
}
