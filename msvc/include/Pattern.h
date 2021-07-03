#pragma once

#include <cstddef>

#define PATTERN_WIDTH 8
#define PATTERN_HEIGHT 8

#define PIXELS_PER_BYTE 2

#define PATTERN_SIZE_IN_MEM PATTERN_WIDTH * PATTERN_HEIGHT
#define PATTERN_SIZE_IN_ROM PATTERN_WIDTH * PATTERN_HEIGHT / PIXELS_PER_BYTE

/**
 * Pattern class definition
 *
 * Patterns are 8x8 pixels in size.
 *
 * In a ROM, two pixels are stored per byte (each pixel is one nibble). The
 * pixel value is a palette column index. Fifteen colors may be referenced,
 * with a zero value used to indicate transparent pixels.
 *
 * Once loaded into memory, the pixels are stored 1-per-byte.
 */
class Pattern
{
public:
    Pattern();

    void loadFromBuffer(unsigned char[PATTERN_SIZE_IN_ROM]);

    unsigned char getPixel(unsigned char x, unsigned char y) const;

    static unsigned int getPatternSize();
    static unsigned char getPatternHeight();
    static unsigned char getPatternWidth();

    void setPixel(unsigned char x, unsigned char y, unsigned char value);

private:
    Pattern(const Pattern&) DELETED;
    Pattern& operator=(const Pattern&) DELETED;

    unsigned char m_pixels[PATTERN_SIZE_IN_MEM];
};

inline unsigned int Pattern::getPatternSize()
{
    return PATTERN_SIZE_IN_ROM;
}

inline unsigned char Pattern::getPatternWidth()
{
    return PATTERN_WIDTH;
}

inline unsigned char Pattern::getPatternHeight()
{
    return PATTERN_HEIGHT;
}
