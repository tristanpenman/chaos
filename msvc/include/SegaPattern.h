///////////////////////////////////////////////////////////////////////////////
//
// SegaPattern.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SEGA_PATTERN_H
#define __SEGA_PATTERN_H

#include <cstddef>

#define PATTERN_WIDTH 8
#define PATTERN_HEIGHT 8

#define PIXELS_PER_BYTE 2

#define PATTERN_SIZE_IN_MEM PATTERN_WIDTH * PATTERN_HEIGHT 
#define PATTERN_SIZE_IN_ROM PATTERN_WIDTH * PATTERN_HEIGHT / PIXELS_PER_BYTE


///////////////////////////////////////////////////////////////////////////////
//
// SegaPattern class definition
//
// Patterns are 8x8 pixels in size.
//
// In a ROM, two pixels are stored per byte (each pixel is one nibble). The
// pixel value is a palette column index. Fifteen colors may be referenced,
// with a zero value used to indicate transparent pixels.
//
// While in memory, the pixels are stored 1-per-byte.
//
///////////////////////////////////////////////////////////////////////////////

class SegaPattern 
{
public:

    SegaPattern();

    virtual             ~SegaPattern();

    void                 loadFromBuffer(unsigned char[PATTERN_SIZE_IN_ROM]);

    unsigned char        getPixel(unsigned char x, unsigned char y) const;

    static unsigned int  getPatternSize();
    static unsigned char getPatternHeight();
    static unsigned char getPatternWidth();

    void                 setPixel(unsigned char x, unsigned char y, unsigned char value);

private:

    SegaPattern(const SegaPattern&);
    SegaPattern& operator=(const SegaPattern&);

    unsigned char m_pixels[PATTERN_SIZE_IN_MEM];
};


///////////////////////////////////////////////////////////////////////////////
//
// SegaPattern inline functions
//
///////////////////////////////////////////////////////////////////////////////

inline unsigned int SegaPattern::getPatternSize()
{
    return PATTERN_SIZE_IN_ROM;
}

inline unsigned char SegaPattern::getPatternWidth()
{
    return PATTERN_WIDTH;
}

inline unsigned char SegaPattern::getPatternHeight()
{
    return PATTERN_HEIGHT;
}


///////////////////////////////////////////////////////////////////////////////
//
// SegaPatternDescriptor class definition
//
// A SonicPatternDescriptor is used to specify which pattern to draw, how it
// should be drawn, and which palette to use. A pattern may be horizontally
// and/or vertically flipped, and must be drawn using 1 of 4 palettes.
//
// A pattern descriptor is defined as a 16-bit bitmask, in the form:
//   PCCVHIII IIIIIIII
//     P    - priority flag
//     CC   - palette row index     [0 - 3]
//     V    - vertical flip flag
//     H    - horizontal flip flag
//     I... - pattern index         [0 - 2047]
//
///////////////////////////////////////////////////////////////////////////////

class SegaPatternDescriptor
{
public:

    SegaPatternDescriptor();

    explicit SegaPatternDescriptor(const SegaPatternDescriptor& desc);
    explicit SegaPatternDescriptor(unsigned short patternIndex, unsigned short paletteIndex);

    SegaPatternDescriptor& operator=(const SegaPatternDescriptor&);

    virtual       ~SegaPatternDescriptor();

    unsigned short get() const;

    unsigned short getPaletteIndex() const;
    unsigned short getPatternIndex() const;

    bool           getFlag_Priority() const;
    bool           getFlag_HFlip() const;
    bool           getFlag_VFlip() const;

    static size_t  getIndexSize();

    void           set(unsigned short); // naive (assumes correct bit layout)

private:
    unsigned short m_index;

};


///////////////////////////////////////////////////////////////////////////////
//
// SegaPatternDescriptor inline functions
//
///////////////////////////////////////////////////////////////////////////////

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

#endif
