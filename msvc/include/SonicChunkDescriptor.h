#pragma once

/**
 * SonicChunkDescriptor class definition
 *
 * A Sonic chunk descriptor is used to specify which chunk to draw and how it
 * should be drawn. A chunk may be horizontally and/or vertically flipped.
 *
 *  ???? YXII IIII IIII
 *
 *  Masks:
 *   0x3FF chunk index
 *   0x400 X flip
 *   0x800 Y flip
 */
class SonicChunkDescriptor
{
public:
    SonicChunkDescriptor();

    unsigned short get() const;
    unsigned short getChunkIndex() const;

    bool getHFlip() const;
    bool getVFlip() const;

    void set(unsigned short);
    void set(SonicChunkDescriptor& desc);

    static size_t getIndexSize();

private:
    SonicChunkDescriptor(const SonicChunkDescriptor&) DELETED;
    SonicChunkDescriptor& operator=(const SonicChunkDescriptor&) DELETED;

    unsigned short m_index;
};

inline SonicChunkDescriptor::SonicChunkDescriptor()
  : m_index(0)
{

}

inline unsigned short SonicChunkDescriptor::get() const
{
    return m_index;
}

inline unsigned short SonicChunkDescriptor::getChunkIndex() const
{
    return m_index & 0x3FF;
}

inline bool SonicChunkDescriptor::getHFlip() const
{
    return (m_index & 0x400) != 0;
}

inline bool SonicChunkDescriptor::getVFlip() const
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

inline size_t SonicChunkDescriptor::getIndexSize()
{
    return sizeof(unsigned short);
}
