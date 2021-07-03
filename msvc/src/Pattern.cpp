#include "Precompiled.h"

#include "Pattern.h"

using namespace std;

Pattern::Pattern()
{
    memset(m_pixels, 0, sizeof(unsigned char) * PATTERN_SIZE_IN_MEM);
}

void Pattern::loadFromBuffer(unsigned char buffer[PATTERN_SIZE_IN_ROM])
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

unsigned char Pattern::getPixel(unsigned char x, unsigned char y) const
{
    return m_pixels[y * PATTERN_WIDTH + x];
}

void Pattern::setPixel(unsigned char x, unsigned char y, unsigned char value)
{
    m_pixels[y * PATTERN_WIDTH + x] = value;
}
