#include "Precompiled.h"

#include "SegaPattern.h"

using namespace std;

SegaPattern::SegaPattern()
{
    memset(m_pixels, 0, sizeof(unsigned char) * PATTERN_SIZE_IN_MEM);
}

void SegaPattern::loadFromBuffer(unsigned char buffer[PATTERN_SIZE_IN_ROM])
{
    unsigned char buffer_pos = 0;

    for (unsigned char row = 0; row < PATTERN_HEIGHT; row++)
    {
        for (unsigned char col = 0; col < PATTERN_WIDTH; col += 2)
        {
            m_pixels[row * PATTERN_WIDTH + col] = (buffer[buffer_pos] >> 4) & 0x0F;
            m_pixels[row * PATTERN_WIDTH + col + 1] = buffer[buffer_pos] & 0x0F;

            buffer_pos++;
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
