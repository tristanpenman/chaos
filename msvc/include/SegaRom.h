#pragma once

/**
 * SEGA ROM file wrapper
 *
 * Provides access to common features of Sega Genesis/Megadrive ROMs, and
 * methods to read/write big-endian addresses.
 */
class SegaRom
{
public:
    explicit SegaRom(std::fstream& file);
    virtual ~SegaRom() = default;

    std::fstream& getFile();

    void fixChecksum(uint32_t length);
    void fixHeader(uint32_t length);

    std::string getDomesticName();
    std::string getInternationalName();

    // BIG-ENDIAN ADDRESS READERS
    uint16_t readAddress_16bit();                    // Read address at current offset
    uint16_t readAddress_16bit_at(std::streamoff o); // Read address at offset 'o'
    uint32_t readAddress_32bit();
    uint32_t readAddress_32bit_at(std::streamoff o);

    // BIG-ENDIAN ADDRESS WRITERS
    void writeAddress_16bit(uint16_t address);
    void writeAddress_16bit_at(uint16_t address, std::streamoff offset);
    void writeAddress_32bit(uint32_t address);
    void writeAddress_32bit_at(uint32_t address, std::streamoff offset);

protected:
    std::fstream& m_file;
};

inline std::fstream& SegaRom::getFile()
{
    return m_file;
}
