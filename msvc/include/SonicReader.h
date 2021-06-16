#pragma once

/**
 * Decompressor for RLE-compressed level data
 * 
 * See doc/kosinski.txt for details of how the (de-)compression algorithm works
 */
class SonicReader
{
public:
    typedef std::pair<bool, size_t> result_t;

    explicit SonicReader(std::fstream& rom);

    result_t decompress(unsigned char buffer[], size_t buffer_size, std::streamoff offset = 0);

protected:
    void loadBitfield();
    unsigned char getBit();

private:
    unsigned int m_bitfield;
    unsigned char m_bitcount;
    std::fstream& m_rom;
};
