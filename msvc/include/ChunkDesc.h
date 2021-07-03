#pragma once

/**
 * Chunk descriptor class definition
 *
 * A chunk descriptor is used to specify which chunk to draw and how it
 * should be drawn. A chunk may be horizontally and/or vertically flipped.
 *
 *  ???? YXII IIII IIII
 *
 *  Masks:
 *   0x3FF chunk index
 *   0x400 X flip
 *   0x800 Y flip
 */
class ChunkDesc
{
public:
    ChunkDesc();

    unsigned short get() const;
    unsigned short getChunkIndex() const;

    bool getHFlip() const;
    bool getVFlip() const;

    void set(unsigned short);
    void set(ChunkDesc& desc);

    static size_t getIndexSize();

private:
    ChunkDesc(const ChunkDesc&) DELETED;
    ChunkDesc& operator=(const ChunkDesc&) DELETED;

    unsigned short m_index;
};

inline ChunkDesc::ChunkDesc()
  : m_index(0)
{

}

inline unsigned short ChunkDesc::get() const
{
    return m_index;
}

inline unsigned short ChunkDesc::getChunkIndex() const
{
    return m_index & 0x3FF;
}

inline bool ChunkDesc::getHFlip() const
{
    return (m_index & 0x400) != 0;
}

inline bool ChunkDesc::getVFlip() const
{
    return (m_index & 0x800) != 0;
}

inline void ChunkDesc::set(unsigned short value)
{
    m_index = value;
}

inline void ChunkDesc::set(ChunkDesc& desc)
{
    m_index = desc.m_index;
}

inline size_t ChunkDesc::getIndexSize()
{
    return sizeof(unsigned short);
}
