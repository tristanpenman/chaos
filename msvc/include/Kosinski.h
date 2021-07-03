#pragma once

#include <algorithm>
#include <iosfwd>

/**
 * Decompressor for RLE-compressed level data
 *
 * See doc/kosinski.txt for details of how the algorithm works
 */
class Kosinski
{
public:
    typedef std::pair<bool, size_t> result_t;

    explicit Kosinski(std::fstream& rom);

    result_t decompress(unsigned char buffer[], size_t buffer_size, std::streamoff rom_offset);

protected:
    void loadBitfield();
    unsigned char getBit();

private:
    unsigned int m_bitfield;
    unsigned char m_bitcount;
    std::fstream& m_rom;
};
