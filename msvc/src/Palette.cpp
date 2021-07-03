#include "Precompiled.h"

#include "Palette.h"

using namespace std;

#define PALETTE_BUFFER_SIZE PALETTE_SIZE * BYTES_PER_COLOR

Palette::Palette()
{
    memset(m_colors, 0, sizeof(NativeColor) * PALETTE_SIZE);
}

bool Palette::readFromFile(fstream& file)
{
    char buffer[PALETTE_BUFFER_SIZE];

    file.read(buffer, PALETTE_BUFFER_SIZE);
    if (file.gcount() != PALETTE_BUFFER_SIZE)
    {
        throw std::runtime_error("Invalid palette data");
    }

    unsigned char color_index = 0;
    unsigned char buffer_pos = 0;

    while (color_index < PALETTE_SIZE)
    {
        convertSegaToNativeColor(&buffer[buffer_pos], m_colors[color_index]);

        color_index++;
        buffer_pos += 2;
    }

    return true;
}

bool Palette::writeToFile(fstream& file, streamoff address) const
{
    return false;
}
