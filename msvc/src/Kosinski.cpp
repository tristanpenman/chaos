#include "Precompiled.h"

#include "Kosinski.h"

using namespace std;

Kosinski::Kosinski(fstream& rom)
  : m_rom(rom)
  , m_bitcount(0)
  , m_bitfield(0)
{

}

unsigned char Kosinski::getBit()
{
    unsigned char bit = ((unsigned char)(m_bitfield)) & 1;

    m_bitfield >>= 1;

    // Ensure that there are more bits to read
    if (--m_bitcount == 0)
    {
        loadBitfield();

        if (m_rom.eof())
        {
            throw std::runtime_error("Unexpected end of file");
        }
    }

    return bit;
}

void Kosinski::loadBitfield()
{
    m_bitfield  = m_rom.get();
    m_bitfield |= m_rom.get() << 8;

    m_bitcount = 16;
}


Kosinski::result_t Kosinski::decompress(unsigned char buffer[], size_t buffer_size, streamoff rom_offset)
{
    uint16_t pos = 0;
    uint16_t count = 0;
    int16_t offset = 0;

    m_rom.seekg(rom_offset);

    loadBitfield();

    while (1)
    {
        if (getBit() == 1)
        {
            buffer[pos] = m_rom.get();
            pos++;

            // Don't write any more bytes if the buffer is full
            if (pos >= buffer_size)
            {
                return result_t(false, pos);
            }

            continue;
        }

        if (getBit() == 1)
        {
            const uint8_t lo = m_rom.get();
            const uint8_t hi = m_rom.get();

            // ---hi--- ---lo---
            // OOOOOCCC OOOOOOOO [CCCCCCCC]<- optional
            // O - Offset bit
            // C - Count bit

            // Offsets are negative numbers stored using a 13-bit two's complement representation. Before the offset
            // can be applied to a 16-bit offset, we have to convert it to 16-bit two's complement representation.
            // Positive numbers are not used, so a naive conversion is okay.
            offset = (0xFF00 | hi) << 5;
            offset = (offset & 0xFF00) | lo;

            // Mask off the count bits
            count = hi & 0x7;

            if (count == 0)
            {
                count = m_rom.get();

                if (count == 0)
                {
                    break;
                }

                if (count <= 1)
                {
                    continue;
                }
            }
            else
            {
                count++;
            }
        }
        else
        {
            count = (getBit() << 1) | getBit();
            count++;

            // Convert 8-bit two's complement representation to 16-bit representation
            offset = m_rom.get() | 0xFF00;
        }

        count++;

        // Copy 'count' bytes from the specified 'offset', relative to the current buffer position
        while (count > 0)
        {
            buffer[pos] = buffer[pos + offset];
            pos++;
            count--;

            // Don't write any more bytes if the buffer is full
            if (pos >= buffer_size)
            {
                return result_t(false, pos);
            }
        }
    }

    return result_t(true, pos);
}
