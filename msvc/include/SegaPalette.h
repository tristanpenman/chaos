///////////////////////////////////////////////////////////////////////////////
//
// SegaPalette.h
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __SEGA_PALETTE_H
#define __SEGA_PALETTE_H

#include <fstream>
#include <cstring>

#define PALETTE_SIZE 16
#define BYTES_PER_COLOR 2

#pragma warning(disable: 4290)

///////////////////////////////////////////////////////////////////////////////
//
// RGB_t structure
//
///////////////////////////////////////////////////////////////////////////////

struct RGB_t
{
    RGB_t()
      : R(0)
      , G(0)
      , B(0)
      , A__unused(0)
    {

    }

    RGB_t(unsigned char r, unsigned char g, unsigned char b)
      : R(r)
      , G(g)
      , B(b)
      , A__unused(0)
    {

    }

    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A__unused;	// PCs may use 32-bit color modes
};


///////////////////////////////////////////////////////////////////////////////
//
// SegaPalette class definition
//
// Provides an interface by which to read/write SEGA palettes from/to a file.
//
// Colors stored in memory are in RGB_t format, suitable for display and
// manipulation on a PC.
//
///////////////////////////////////////////////////////////////////////////////

class SegaPalette
{
public:
    SegaPalette();
    SegaPalette(const SegaPalette&) = delete;
    virtual ~SegaPalette() = default;

    SegaPalette& operator=(const SegaPalette&) = delete;

    const RGB_t& getRGB(unsigned char index) const throw(ChaosException);

    void setRGB(unsigned char index, RGB_t new_color) throw(ChaosException);

    bool readFromFile(std::fstream& file);
    bool writeToFile(std::fstream& file, std::streamoff address) const;

    static void convertSegaToRGB(char bytes[BYTES_PER_COLOR], RGB_t& RGB_t);
    static void convertRGBToSega(RGB_t& RGB_t, char bytes[BYTES_PER_COLOR]);

    static unsigned int getPaletteSize();

private:
    RGB_t m_colors[PALETTE_SIZE];
};

inline unsigned int SegaPalette::getPaletteSize()
{
    return PALETTE_SIZE;
}

inline const RGB_t& SegaPalette::getRGB(unsigned char index) const throw(ChaosException)
{
    if (index >= PALETTE_SIZE)
    {
        throw ChaosException("Invalid palette index");
    }

    return m_colors[index];
}

inline void SegaPalette::setRGB(unsigned char index, RGB_t new_color) throw(ChaosException)
{
    if (index >= PALETTE_SIZE)
    {
        throw ChaosException("Invalid palette index");
    }

    memcpy(&m_colors[index], &new_color, sizeof(struct RGB_t));
}

inline void SegaPalette::convertSegaToRGB(char bytes[BYTES_PER_COLOR], RGB_t& rgb)
{
    rgb.R = (bytes[1] & 0x0F) * 0x10;
    rgb.G = (bytes[1] & 0xF0);
    rgb.B = (bytes[0] * 0x10);
}

inline void SegaPalette::convertRGBToSega(RGB_t& rgb, char bytes[BYTES_PER_COLOR])
{
    throw 0;	// NOT IMPLEMENTED
}

#endif
