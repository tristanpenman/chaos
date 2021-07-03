#pragma once

#include <cstring>
#include <fstream>

#include "NativeColor.h"

#define PALETTE_SIZE 16
#define BYTES_PER_COLOR 2

/**
 * Palette class definition
 *
 * Provides an interface by which to read/write SEGA palettes from/to a file.
 *
 * Colors stored in memory are in RGB format, suitable for display and
 * manipulation on a PC.
 */
class Palette
{
public:
    Palette();

    virtual ~Palette() {}

    const NativeColor& getNativeColor(unsigned char index) const;

    void setNativeColor(unsigned char index, NativeColor color);

    bool readFromFile(std::fstream& file);
    bool writeToFile(std::fstream& file, std::streamoff address) const;

    static void convertSegaToNativeColor(char bytes[BYTES_PER_COLOR], NativeColor& nativeColor);

    static unsigned int getPaletteSize();

private:
    Palette(const Palette&) DELETED;
    Palette& operator=(const Palette&) DELETED;

    NativeColor m_colors[PALETTE_SIZE];
};

inline unsigned int Palette::getPaletteSize()
{
    return PALETTE_SIZE;
}

inline const NativeColor& Palette::getNativeColor(unsigned char index) const
{
    if (index >= PALETTE_SIZE)
    {
        throw std::runtime_error("Invalid palette index");
    }

    return m_colors[index];
}

inline void Palette::setNativeColor(unsigned char index, NativeColor color)
{
    if (index >= PALETTE_SIZE)
    {
        throw std::runtime_error("Invalid palette index");
    }

    memcpy(&m_colors[index], &color, sizeof(NativeColor));
}

inline void Palette::convertSegaToNativeColor(char bytes[BYTES_PER_COLOR], NativeColor& color)
{
    color.r = (bytes[1] & 0x0F) * 0x10;
    color.g = (bytes[1] & 0xF0);
    color.b = (bytes[0] * 0x10);
}
