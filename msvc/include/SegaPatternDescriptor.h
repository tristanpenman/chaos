#pragma once

/**
 * SegaPatternDescriptor class definition
 *
 * A SonicPatternDescriptor is used to specify which pattern to draw, how it
 * should be drawn, and which palette to use. A pattern may be horizontally
 * and/or vertically flipped, and must be drawn using 1 of 4 palettes.
 *
 * A pattern descriptor is defined as a 16-bit bitmask, in the form:
 *   PCCVHIII IIIIIIII
 *     P    - priority flag
 *     CC   - palette row index     [0 - 3]
 *     V    - vertical flip flag
 *     H    - horizontal flip flag
 *     I... - pattern index         [0 - 2047]
 */
class SegaPatternDescriptor
{
public:
    SegaPatternDescriptor();
    explicit SegaPatternDescriptor(const SegaPatternDescriptor& desc);
    SegaPatternDescriptor(unsigned short patternIndex, unsigned short paletteIndex);

    virtual ~SegaPatternDescriptor() {}

    SegaPatternDescriptor& operator=(const SegaPatternDescriptor&);

    unsigned short get() const;

    unsigned short getPaletteIndex() const;
    unsigned short getPatternIndex() const;

    bool getFlag_Priority() const;
    bool getFlag_HFlip() const;
    bool getFlag_VFlip() const;

    static size_t getIndexSize();

    void set(unsigned short); // naive (assumes correct bit layout)

private:
    unsigned short m_index;
};

inline SegaPatternDescriptor::SegaPatternDescriptor()
  : m_index(0)
{

}

inline SegaPatternDescriptor::SegaPatternDescriptor(const SegaPatternDescriptor& desc)
  : m_index(desc.m_index)
{

}

inline SegaPatternDescriptor::SegaPatternDescriptor(unsigned short patternIndex, unsigned short paletteIndex)
  : m_index(0)
{
    m_index |= (paletteIndex & 0x3) << 13;
    m_index |= (patternIndex & 0x7FF);
}

inline SegaPatternDescriptor& SegaPatternDescriptor::operator=(const SegaPatternDescriptor& rhs)
{
    m_index = rhs.m_index;
    return *this;
}

inline unsigned short SegaPatternDescriptor::get() const
{
    return m_index;
}

inline size_t SegaPatternDescriptor::getIndexSize()
{
    return sizeof(unsigned short);
}

inline unsigned short SegaPatternDescriptor::getPaletteIndex() const
{
    return (m_index >> 13) & 0x3;
}

inline unsigned short SegaPatternDescriptor::getPatternIndex() const
{
    return m_index & 0x7FF;
}

inline bool SegaPatternDescriptor::getFlag_Priority() const
{
    return (m_index & 0x8000) != 0;
}

inline bool SegaPatternDescriptor::getFlag_HFlip() const
{
    return (m_index & 0x800) != 0;
}

inline bool SegaPatternDescriptor::getFlag_VFlip() const
{
    return (m_index & 0x1000) != 0;
}

inline void SegaPatternDescriptor::set(unsigned short new_index)
{
    m_index = new_index;
}
