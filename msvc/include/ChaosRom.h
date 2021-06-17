#pragma once

class Level;

/**
 * ROM wrapper base class
 *
 * Provides an interface used by the ChaosRomFactory class to select the
 * correct subclass for a given ROM.
 *
 * Also provides an interface by which a subclass can instantiate level editor
 * and sprite editor instances.
 */
class ChaosRom
{
public:
    typedef std::map<unsigned int, std::string> LevelNames_t;

    explicit ChaosRom(std::fstream& rom);
    virtual ~ChaosRom();

    std::fstream& getFile();

    // abstract
    virtual bool validateROM() = 0;
    virtual LevelNames_t getLevelNames() = 0;

    Level* getLevelInstance(HWND hwnd);
    Level* initLevelInstance(HWND hwnd);

    bool destroyInstance(HWND hwnd);

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
    virtual Level* instantiateLevel() = 0;

    std::fstream& m_file;

protected:
    typedef std::map<HWND, Level*> LevelInstances;

    LevelInstances m_levelInstances;
};

inline std::fstream& ChaosRom::getFile()
{
    return m_file;
}
