///////////////////////////////////////////////////////////////////////////////
//
// SonicChunk.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SONIC_CHUNK_H
#define __SONIC_CHUNK_H

#include <cstddef>

#include "SegaPattern.h"

#define PATTERNS_PER_CHUNK 4
#define BYTES_PER_PATTERN 2
#define CHUNK_SIZE_IN_ROM PATTERNS_PER_CHUNK * BYTES_PER_PATTERN


///////////////////////////////////////////////////////////////////////////////
//
// SonicChunk class definition
//
// A Sonic chunk represents a 16x16 tile, composed of 4 8x8 SEGA patterns.
// Pattern indices are defined using the standard SEGA pattern descriptor -
// see SegaPattern.h for more details.
//
///////////////////////////////////////////////////////////////////////////////

class SonicChunk
{
public:

    SonicChunk();

    virtual ~SonicChunk();

    void loadFromBuffer(unsigned char buffer[CHUNK_SIZE_IN_ROM]);

    const SegaPatternDescriptor& getPatternDescriptor(unsigned int x, unsigned int y) const;

    static unsigned int getChunkHeight();
    static unsigned int getChunkWidth();
    static unsigned int getChunkSize();

private:
    SonicChunk(const SonicChunk&);
    SonicChunk& operator=(const SonicChunk&);

    SegaPatternDescriptor m_pattern_indices[PATTERNS_PER_CHUNK];
};


///////////////////////////////////////////////////////////////////////////////
//
// SonicChunk inline functions
//
///////////////////////////////////////////////////////////////////////////////

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


///////////////////////////////////////////////////////////////////////////////
//
// SonicChunkDescriptor class definition
//
// A Sonic chunk descriptor is used to specify which chunk to draw and how it
// should be drawn. A chunk may be horizontally and/or vertically flipped.
//
//  ???? YXII IIII IIII
//
//  Masks:
//   0x3FF chunk index
//   0x400 X flip
//   0x800 Y flip
//
///////////////////////////////////////////////////////////////////////////////

class SonicChunkDescriptor
{
public:

    SonicChunkDescriptor();

    virtual ~SonicChunkDescriptor();

    unsigned short get() const;
    unsigned short getChunkIndex() const;

    bool           getFlag_HFlip() const;
    bool           getFlag_VFlip() const;

    static size_t  getIndexSize();

    void           set(unsigned short);
    void           set(SonicChunkDescriptor& desc);

private:
    SonicChunkDescriptor(const SonicChunkDescriptor&);
    SonicChunkDescriptor& operator=(const SonicChunkDescriptor&);

    unsigned short m_index;
};


///////////////////////////////////////////////////////////////////////////////
//
// SonicChunkDescriptor inline functions
//
///////////////////////////////////////////////////////////////////////////////

inline size_t SonicChunkDescriptor::getIndexSize()
{
    return sizeof(unsigned short);
}

inline unsigned short SonicChunkDescriptor::get() const
{
    return m_index;
}

inline unsigned short SonicChunkDescriptor::getChunkIndex() const
{
    return m_index & 0x3FF;
}

inline bool SonicChunkDescriptor::getFlag_HFlip() const
{
    return (m_index & 0x400) != 0;
}

inline bool SonicChunkDescriptor::getFlag_VFlip() const
{
    return (m_index & 0x800) != 0;
}

inline void SonicChunkDescriptor::set(unsigned short value)
{
    m_index = value;
}

inline void SonicChunkDescriptor::set(SonicChunkDescriptor& desc)
{
    m_index = desc.m_index;
}

#endif
