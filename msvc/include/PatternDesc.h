#pragma once

/**
 * Pattern descriptor class definition
 *
 * A pattern descriptor is used to specify which pattern to draw, how it
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
class PatternDesc
{
public:
    PatternDesc();
    explicit PatternDesc(const PatternDesc& desc);
    PatternDesc(unsigned short patternIndex, unsigned short paletteIndex);

    PatternDesc& operator=(const PatternDesc&);

    unsigned short get() const;

    unsigned short getPaletteIndex() const;
    unsigned short getPatternIndex() const;

    bool getFlag_Priority() const;
    bool getHFlip() const;
    bool getVFlip() const;

    static size_t getIndexSize();

    void set(unsigned short); // naive (assumes correct bit layout)

private:
    unsigned short m_index;
};

inline PatternDesc::PatternDesc()
  : m_index(0)
{

}

inline PatternDesc::PatternDesc(const PatternDesc& desc)
  : m_index(desc.m_index)
{

}

inline PatternDesc::PatternDesc(unsigned short patternIndex, unsigned short paletteIndex)
  : m_index(0)
{
    m_index |= (paletteIndex & 0x3) << 13;
    m_index |= (patternIndex & 0x7FF);
}

inline PatternDesc& PatternDesc::operator=(const PatternDesc& rhs)
{
    m_index = rhs.m_index;
    return *this;
}

inline unsigned short PatternDesc::get() const
{
    return m_index;
}

inline size_t PatternDesc::getIndexSize()
{
    return sizeof(unsigned short);
}

inline unsigned short PatternDesc::getPaletteIndex() const
{
    return (m_index >> 13) & 0x3;
}

inline unsigned short PatternDesc::getPatternIndex() const
{
    return m_index & 0x7FF;
}

inline bool PatternDesc::getFlag_Priority() const
{
    return (m_index & 0x8000) != 0;
}

inline bool PatternDesc::getHFlip() const
{
    return (m_index & 0x800) != 0;
}

inline bool PatternDesc::getVFlip() const
{
    return (m_index & 0x1000) != 0;
}

inline void PatternDesc::set(unsigned short new_index)
{
    m_index = new_index;
}
