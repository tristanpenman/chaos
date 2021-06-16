#include "Precompiled.h"
#include "ChaosException.h"
#include "ChaosLog.h"

#include "SegaPalette.h"

using namespace std;

#define PALETTE_BUFFER_SIZE PALETTE_SIZE * BYTES_PER_COLOR

SegaPalette::SegaPalette()
{
    memset(m_colors, 0, sizeof(RGB_t) * PALETTE_SIZE);
}

bool SegaPalette::readFromFile(fstream& file)
{
    try
    {
        char buffer[PALETTE_BUFFER_SIZE];

        file.read(buffer, PALETTE_BUFFER_SIZE);
        if (file.gcount() != PALETTE_BUFFER_SIZE)
        {
            throw ChaosException("Invalid palette data");
        }

        unsigned char color_index = 0;
        unsigned char bufferPos = 0;

        while (color_index < PALETTE_SIZE)
        {
            convertSegaToRGB(&buffer[bufferPos], m_colors[color_index]);

            color_index++;
            bufferPos += 2;
        }

        return true;
    }
    catch (exception &e)
    {
        ChaosLog() << "Palette read exception: " << e.what();
    }
    catch (...)
    {
        ChaosLog() << "Palette read exception: Unknown exception";
    }

    return false;
}

bool SegaPalette::writeToFile(fstream& file, streamoff address) const
{
    return false;
}
