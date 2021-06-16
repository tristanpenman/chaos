///////////////////////////////////////////////////////////////////////////////
//
// SegaPattern.cpp
//
///////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"

#include "SegaPattern.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
//
// SegaPattern implementation
//
///////////////////////////////////////////////////////////////////////////////

SegaPattern::SegaPattern()
{
    memset(m_pixels, 0, sizeof(unsigned char) * PATTERN_SIZE_IN_MEM);
}

SegaPattern::~SegaPattern()
{

}

void SegaPattern::loadFromBuffer(unsigned char buffer[PATTERN_SIZE_IN_ROM])
{
    unsigned char bufferPos = 0;

    for (unsigned char row = 0; row < PATTERN_HEIGHT; row++)
    {
        for (unsigned char col = 0; col < PATTERN_WIDTH; col += 2)
        {
            m_pixels[row * PATTERN_WIDTH + col] = (buffer[bufferPos] >> 4) & 0x0F;
            m_pixels[row * PATTERN_WIDTH + col + 1] = buffer[bufferPos] & 0x0F;

            bufferPos++;
        }
    }
}

unsigned char SegaPattern::getPixel(unsigned char x, unsigned char y) const
{
    return m_pixels[y * PATTERN_WIDTH + x];
}

void SegaPattern::setPixel(unsigned char x, unsigned char y, unsigned char value)
{
    m_pixels[y * PATTERN_WIDTH + x] = value;
}


///////////////////////////////////////////////////////////////////////////////
//
// SegaPatternDescriptor implementation
//
///////////////////////////////////////////////////////////////////////////////

SegaPatternDescriptor::SegaPatternDescriptor()
: m_index(0)
{

}

SegaPatternDescriptor::SegaPatternDescriptor(const SegaPatternDescriptor& desc)
: m_index(desc.m_index)
{

}

SegaPatternDescriptor::SegaPatternDescriptor(unsigned short patternIndex, unsigned short paletteIndex)
: m_index(0)
{
    m_index |= (paletteIndex & 0x3) << 13;
    m_index |= (patternIndex & 0x7FF);
}

SegaPatternDescriptor& SegaPatternDescriptor::operator=(const SegaPatternDescriptor& rhs)
{
    m_index = rhs.m_index;
    return *this;
}

SegaPatternDescriptor::~SegaPatternDescriptor()
{

}
